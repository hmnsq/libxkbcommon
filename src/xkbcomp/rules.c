/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <stdint.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "rules.h"
#અન્તર્ભાવ "rmlvo.h"
#અન્તર્ભાવ "include.h"
#અન્તર્ભાવ "scanner-utils.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "util-numbers.h"
#અન્તર્ભાવ "util-paths.h"

#define MAX_INCLUDE_DEPTH 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
સંઘ lvalue {
    સંરચના sval string;
};

ગણના rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_લીટી,
    TOK_IDENTIFIER,
    TOK_GROUP_NAME,
    TOK_BANG,
    TOK_EQUALS,
    TOK_WILD_CARD_STAR,
    TOK_WILD_CARD_NONE,
    TOK_WILD_CARD_SOME,
    TOK_WILD_CARD_ANY,
    TOK_INCLUDE,
    TOK_ERROR
};

સ્થિર ગણના rules_token
lex(સંરચના scanner *s, સંઘ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    જ્યારે (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    જો (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    જો (scanner_eol(s)) {
        જ્યારે (scanner_eol(s)) scanner_next(s);
        પાછા TOK_END_OF_લીટી;
    }

    /* Escaped line continuation */
    જો (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        જો (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            પાછા TOK_ERROR;
        }
        scanner_next(s);
        જાઓ skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    જો (scanner_eof(s)) પાછા TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    ગણના rules_token token = TOK_IDENTIFIER;

    અચલ અક્ષર next = scanner_next(s);
    પસંદગી (next) {
    સ્થિતિ '!':
        પાછા TOK_BANG;
    સ્થિતિ '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        તોડો;
    સ્થિતિ '*':
        જો (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            તોડો;
        } નહીંતર {
            /* Legacy wild card */
            પાછા TOK_WILD_CARD_STAR;
        }
    સ્થિતિ '<':
        /* Extended wild cards */
        જો (scanner_rules_lit_token(s, "none>")) પાછા TOK_WILD_CARD_NONE;
        જો (scanner_rules_lit_token(s, "some>")) પાછા TOK_WILD_CARD_SOME;
        જો (scanner_rules_lit_token(s, "any>")) પાછા TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        પ્રતિજ્ઞા(scanner_rules_is_ident('<'));
        val->string.len = 1;
        તોડો;
    સ્થિતિ '=':
        જો (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            તોડો;
        } નહીંતર {
            પાછા TOK_EQUALS;
        }
    સ્થિતિ 'i':
        જો (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            પાછા TOK_INCLUDE;
        }
        /* Identifier */
        પ્રતિજ્ઞા(scanner_rules_is_ident('i'));
        val->string.len = 1;
        તોડો;
    મૂળભૂત:
        જો (likely(scanner_rules_is_ident(next))) {
            /* Identifier */
            val->string.len = 1;
        } નહીંતર {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.start = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    પ્રતિજ્ઞા(scanner_rules_is_ident(MERGE_OVERRIDE_PREFIX));
    પ્રતિજ્ઞા(scanner_rules_is_ident(MERGE_AUGMENT_PREFIX));
    પ્રતિજ્ઞા(scanner_rules_is_ident(MERGE_REPLACE_PREFIX));
    પ્રતિજ્ઞા(scanner_rules_is_ident('('));
    પ્રતિજ્ઞા(scanner_rules_is_ident(')'));
    val->string.len += scanner_rules_ident_token(s);

    જો (unlikely(val->string.len == 0)) {
        જો (token == TOK_GROUP_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } નહીંતર {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unrecognized token");
        }
        પાછા TOK_ERROR;
    }

    પાછા token;
}

/***====================================================================***/

ગણના rules_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_અંક_ENTRIES
};

ગણના {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

પ્રકારવ્યાખ્યા uint8_t mlvo_index_t;
પ્રકારવ્યાખ્યા uint8_t mlvo_mask_t;


સ્થિર અચલ સંરચના sval rules_mlvo_svals[MLVO_અંક_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

ગણના rules_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_અંક_ENTRIES
};

પ્રકારવ્યાખ્યા uint8_t kccgst_index_t;
પ્રકારવ્યાખ્યા uint8_t kccgst_mask_t;

સ્થિર અચલ સંરચના sval rules_kccgst_svals[KCCGST_અંક_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

ગણના {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL ==
              (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
સંરચના matched_sval {
    સંરચના sval sval;
    /** Used for layout-specific options */
    xkb_layout_mask_t layouts;
    xkb_layout_mask_t matched;
};
પ્રકારવ્યાખ્યા darray(સંરચના matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
સંરચના rule_names {
    સંરચના matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

સંરચના group {
    સંરચના sval name;
    darray_sval elements;
};

સંરચના mapping {
    ગણના rules_mlvo mlvo_at_pos[MLVO_અંક_ENTRIES];
    mlvo_index_t અંક_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    સંઘ {
        સંરચના { xkb_layout_index_t layout_idx, variant_idx; };
        સંરચના { xkb_layout_index_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    સંઘ {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    ગણના rules_kccgst kccgst_at_pos[KCCGST_અંક_ENTRIES];
    kccgst_index_t અંક_kccgst;
    kccgst_mask_t defined_kccgst_mask;
};

ગણના mlvo_match_type {
    /** Match the given plain value */
    MLVO_MATCH_NORMAL = 0,
    /** Match depending on the value of `wildcard_match_type` */
    MLVO_MATCH_WILDCARD_LEGACY,
    /** Match empty value */
    MLVO_MATCH_WILDCARD_NONE,
    /** Match non-empty value */
    MLVO_MATCH_WILDCARD_SOME,
    /** Match any value, optionally empty */
    MLVO_MATCH_WILDCARD_ANY,
    /** Match any entry in a group */
    MLVO_MATCH_GROUP,
};

ગણના wildcard_match_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_MATCH_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_MATCH_ALL,
};

સંરચના rule {
    સંરચના sval mlvo_કિંમત_at_pos[MLVO_અંક_ENTRIES];
    ગણના mlvo_match_type match_type_at_pos[MLVO_અંક_ENTRIES];
    સંરચના sval kccgst_કિંમત_at_pos[KCCGST_અંક_ENTRIES];
    mlvo_index_t અંક_mlvo_કિંમતો;
    kccgst_index_t અંક_kccgst_કિંમતો;
    bool skip;
};

#define SLICE_KCCGST_BIT_FIELD_SIZE 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_SIZE - 1)) &
                (KCCGST_અંક_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
સંરચના kccgst_buffer_slice {
    uint32_t length:(32 - SLICE_KCCGST_BIT_FIELD_SIZE);
    ગણના rules_kccgst kccgst:SLICE_KCCGST_BIT_FIELD_SIZE;
    xkb_layout_index_t layout;
};

/* Buffer for pending KcCGST values */
સંરચના kccgst_buffer {
    darray_char buffer;
    /* Slice corresponding to each value in the buffer */
    darray(સંરચના kccgst_buffer_slice) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
સંરચના matcher {
    સંરચના xkb_context *ctx;
    /* Input.*/
    સંરચના rule_names rmlvo;
    સંઘ lvalue val;
    darray(સંરચના group) groups;
    /* Current mapping. */
    સંરચના mapping mapping;
    /* Current rule. */
    સંરચના rule rule;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    સંરચના kccgst_buffer pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_અંક_ENTRIES];
};

સ્થિર સંરચના sval
strip_spaces(સંરચના sval v)
{
    જ્યારે (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    જ્યારે (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    પાછા v;
}

સ્થિર darray_matched_sval
split_comma_separated_mlvo(સંરચના xkb_context *ctx,
                           ગણના rules_mlvo mlvo, અચલ અક્ષર *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    જો (!s) {
        ensure_at_least_one_કિંમત:
        /* Label followed by a declaration is a C23 extension */ ;

        સંરચના matched_sval val = {
            .sval = SVAL(ખાલી, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        પાછા arr;
    }

    જ્યારે (true) {
        સંરચના matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        જ્યારે (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        જો (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            અચલ અક્ષર* અચલ layout_start = s;
            xkb_layout_index_t layout = XKB_LAYOUT_INVALID;

            પૂર્ણાંક ગણતરી = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            જો (ગણતરી > 0) {
                /* Note: 1-indexed layout */
                s += ગણતરી;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                જો (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (unsigned પૂર્ણાંક) val.sval.len, val.sval.start);
                } નહીંતર જો (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (unsigned પૂર્ણાંક) val.sval.len, val.sval.start);
                } નહીંતર {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            અચલ અક્ષર* અચલ layout_index_end = s;
            જ્યારે (*s != '\0' && *s != ',') { s++; }
            જો (ગણતરી <= 0 || layout_index_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (unsigned પૂર્ણાંક) (s - layout_start), layout_start,
                        (unsigned પૂર્ણાંક) val.sval.len, val.sval.start);
            }
        }

        જો (val.sval.len || mlvo != MLVO_OPTION) {
            જો (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                સંરચના matched_sval *prev;
                darray_foreach(prev, arr) {
                    જો (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        જો (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } નહીંતર જો (prev->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        જાઓ next_કિંમત;
                    }
                }
            }
            darray_append(arr, val);
        }

next_કિંમત:
        જો (*s == '\0') તોડો;
        જો (*s == ',') s++;
    }

    જો (darray_empty(arr))
        જાઓ ensure_at_least_one_કિંમત;

    પાછા arr;
}

સ્થિર સંરચના matcher *
matcher_new_from_rmlvo(અચલ સંરચના xkb_rmlvo_builder *rmlvo, અચલ અક્ષર **rules)
{
    સંરચના matcher *m = calloc(1, કદ(*m));
    જો (!m)
        પાછા ખાલી;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    સંરચના xkb_rule_names names = {
        .rules = rmlvo->rules,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_empty(rmlvo->layouts)) ? ખાલી : "x",
        .variant = (darray_empty(rmlvo->layouts)) ? ખાલી : "x",
        .options = (darray_empty(rmlvo->options)) ? ખાલી : "x",
    };
    અચલ ગણના RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
                                                               &names);

    /* Initialize matcher with sanitized names, if relevant */
    જો (changed & RMLVO_RULES) {
        *rules = names.rules;
    } નહીંતર {
        *rules = rmlvo->rules;
    }

    જો (changed & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = names.model;
    } નહીંતર {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;

    પ્રતિજ્ઞા((changed & RMLVO_LAYOUT) || !(changed & RMLVO_VARIANT));
    જો (changed & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = split_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      names.layout);
        m->rmlvo.variants = split_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       names.variant);
        જો (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            જો (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        } નહીંતર જો (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } નહીંતર {
        સંરચના xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            સંરચના matched_sval val = {
                .sval = SVAL(layout->layout, strlen_safe(layout->layout)),
                .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
                .matched = 0
            };
            darray_append(m->rmlvo.layouts, val);
            val.sval.start = layout->variant;
            val.sval.len = strlen_safe(layout->variant);
            darray_append(m->rmlvo.variants, val);
        }
    }

    જો (changed & RMLVO_OPTIONS) {
        m->rmlvo.options = split_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      names.options);
    } નહીંતર {
        સંરચના xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            સંરચના matched_sval val = {
                .sval = SVAL(option->option, strlen_safe(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    પાછા m;
}

સ્થિર સંરચના matcher *
matcher_new_from_names(સંરચના xkb_context *ctx,
            અચલ સંરચના xkb_rule_names *rmlvo)
{
    સંરચના matcher *m = calloc(1, કદ(*m));
    જો (!m)
        પાછા ખાલી;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    જો (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        જો (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
    } નહીંતર જો (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    પાછા m;
}

સ્થિર શૂન્ય
matcher_free(સંરચના matcher *m)
{
    જો (!m)
        પાછા;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    સંરચના group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.buffer);
    darray_free(m->pending_kccgst.slices);
    માટે (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_અંક_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    મુક્ત(m);
}

સ્થિર શૂન્ય
matcher_group_start_new(સંરચના matcher *m, સંરચના sval name)
{
    સંરચના group group = { .name = name, .elements = darray_new() };
    darray_append(m->groups, group);
}

સ્થિર શૂન્ય
matcher_group_add_element(સંરચના matcher *m, સંરચના scanner *s,
                          સંરચના sval element)
{
    darray_append(darray_item(m->groups, darray_size(m->groups) - 1).elements,
                  element);
}

સ્થિર bool
read_rules_file(સંરચના xkb_context *ctx,
                સંરચના matcher *matcher,
                unsigned પૂર્ણાંક include_depth,
                FILE *file,
                અચલ અક્ષર *path);

સ્થિર શૂન્ય
matcher_include(સંરચના matcher *m, સંરચના scanner *parent_scanner,
                unsigned પૂર્ણાંક include_depth,
                સંરચના sval inc)
{
    જો (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        પાછા;
    }

    અચલ અક્ષર *stmt_file = inc.start;
    size_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    અક્ષર buf[PATH_MAX] = {0};
    અચલ ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, કદ(buf));
    જો (expanded < 0) {
        /* Error */
        પાછા;
    } નહીંતર જો (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (size_t) expanded;
        પ્રતિજ્ઞા(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned પૂર્ણાંક offset = 0;
    અચલ bool absolute_path = is_absolute_path(stmt_file);
    જો (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        જો (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            જો (stmt_file_len < કદ(buf)) {
                memcpy(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } નહીંતર {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, કદ(buf),
                        (unsigned પૂર્ણાંક) stmt_file_len, stmt_file);
                પાછા;
            }
        } નહીંતર {
            /* %-expansion is always NULL-terminated */
            પ્રતિજ્ઞા(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } નહીંતર {
        /* Relative path: lookup the first XKB path */
        જો (unlikely(expanded)) {
            /*
             * Relative path after expansion
             *
             * Unlikely to happen, because %-expansion is meant to use absolute
             * paths. Considering that:
             * - we do not resolve paths before expansion, leading to unexpected
             *   result here;
             * - we need the buffer afterwards, but it currently contains the
             *   expanded path;
             * - this is an edge case;
             * we simply make the lookup fail.
             */
            file = ખાલી;
        } નહીંતર {
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, કદ(buf), &offset, true);
        }
    }

    જ્યારે (file) {
        પ્રતિજ્ઞા(strlen_safe(buf) < કદ(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        fclose(file);
        જો (ret)
            પાછા;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        જો (absolute_path) {
            /* There is no point to search further if the path is absolute */
            તોડો;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, કદ(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (unsigned પૂર્ણાંક) stmt_file_len, stmt_file);
}

સ્થિર શૂન્ય
matcher_mapping_start_new(સંરચના matcher *m)
{
    માટે (mlvo_index_t i = 0; i < (mlvo_index_t) MLVO_અંક_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_અંક_ENTRIES;
    માટે (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_અંક_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_અંક_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.અંક_mlvo = m->mapping.અંક_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
સ્થિર પૂર્ણાંક
parse_layout_int_index_tail(અચલ અક્ષર *s, size_t max_len,
                            xkb_layout_index_t *out)
{
    જો (max_len < 3)
        પાછા -1;
    uint32_t val = 0;
    અચલ પૂર્ણાંક ગણતરી = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    જો (ગણતરી <= 0 || s[1 + ગણતરી] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        પાછા -1;
    /* To zero-based index. */
    *out = val - 1;
    પાછા ગણતરી + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
સ્થિર પૂર્ણાંક
extract_layout_index(અચલ અક્ષર *s, size_t max_len, xkb_layout_index_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    જો (max_len < 3 || s[0] != '[')
        પાછા -1;
    જો (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_index_t)(s[1] - '1');
        પાછા 3;
    }
    જો (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        પાછા 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    પાછા parse_layout_int_index_tail(s, max_len, out);
}

/* Special layout indices */
ગણના layout_index_ranges {
    LAYOUT_INDEX_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_INDEX_FIRST,
    LAYOUT_INDEX_LATER,
    LAYOUT_INDEX_MULTIPLE,
    LAYOUT_INDEX_ANY,
};

static_assert((xkb_layout_index_t) XKB_MAX_GROUPS <
              (xkb_layout_index_t) LAYOUT_INDEX_SINGLE,
              "Cannot define special indices");
static_assert((xkb_layout_index_t) LAYOUT_INDEX_SINGLE <
              (xkb_layout_index_t) LAYOUT_INDEX_FIRST &&
              (xkb_layout_index_t) LAYOUT_INDEX_FIRST <
              (xkb_layout_index_t) LAYOUT_INDEX_LATER &&
              (xkb_layout_index_t) LAYOUT_INDEX_LATER <
              (xkb_layout_index_t) LAYOUT_INDEX_MULTIPLE &&
              (xkb_layout_index_t) LAYOUT_INDEX_MULTIPLE <
              (xkb_layout_index_t) LAYOUT_INDEX_ANY &&
              (xkb_layout_index_t) LAYOUT_INDEX_ANY <
              (xkb_layout_index_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
સ્થિર bool
extract_mapping_layout_index(અચલ અક્ષર *s, size_t max_len,
                             xkb_layout_index_t *out)
{
    /* Check for minimal `[` + index + `]` */
    જો (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        પાછા false;
    }

    /* Small numeric index */
    જો (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_index_t)(s[1] - '1');
        પાછા true;
    }

    /* Named indices ranges */
    સ્થિર અચલ સંરચના {
        અચલ અક્ષર* name;
        size_t size;
        xkb_layout_index_t કિંમત;
    } names[] = {
        #define INDEX_RANGE(tail, કિંમત) \
            { (tail), કદ(tail), (xkb_layout_index_t)(કિંમત) }
        INDEX_RANGE("single]",   LAYOUT_INDEX_SINGLE),
        INDEX_RANGE("first]",    LAYOUT_INDEX_FIRST),
        INDEX_RANGE("later]",    LAYOUT_INDEX_LATER),
        INDEX_RANGE("multiple]", LAYOUT_INDEX_MULTIPLE),
        INDEX_RANGE("any]",      LAYOUT_INDEX_ANY),
        #undef INDEX_RANGE
    };

    માટે (size_t k = 0; k < ARRAY_SIZE(names); k++) {
        જો (max_len == names[k].size &&
            memcmp(&s[1], names[k].name, names[k].size - 1) == 0) {
            *out = names[k].કિંમત;
            પાછા true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    અચલ પૂર્ણાંક ગણતરી = parse_layout_int_index_tail(s, max_len, out);
    પાછા ગણતરી > 0 && (size_t)ગણતરી == max_len;
}

સ્થિર અંતઃરેખ bool
is_mlvo_mask_defined(સંરચના matcher *m, ગણના rules_mlvo mlvo)
{
    પાછા m->mapping.defined_mlvo_mask & (1u << mlvo);
}

સ્થિર શૂન્ય
matcher_mapping_set_mlvo(સંરચના matcher *m, સંરચના scanner *s,
                         સંરચના sval ident)
{
    ગણના rules_mlvo mlvo;
    સંરચના sval mlvo_sval;

    માટે (mlvo = 0; mlvo < MLVO_અંક_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        જો (svaleq_prefix(mlvo_sval, ident))
            તોડો;
    }

    /* Not found. */
    જો (mlvo >= MLVO_અંક_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned પૂર્ણાંક) ident.len, ident.start);
        m->mapping.active = false;
        પાછા;
    }

    જો (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned પૂર્ણાંક) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        પાછા;
    }

    /* If there are leftovers still, it must be an index. */
    જો (mlvo_sval.len < ident.len) {
        xkb_layout_index_t idx;
        જો (!extract_mapping_layout_index(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (unsigned પૂર્ણાંક) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            પાછા;
        }

        જો (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        નહીંતર જો (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        નહીંતર {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (unsigned પૂર્ણાંક) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            પાછા;
        }
    } નહીંતર જો (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    } નહીંતર જો (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    જો (((mlvo == MLVO_LAYOUT && is_mlvo_mask_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_mask_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.active = false;
        પાછા;
    }

    m->mapping.mlvo_at_pos[m->mapping.અંક_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.અંક_mlvo++;
}

સ્થિર શૂન્ય
matcher_mapping_set_layout_bounds(સંરચના matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_index_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    પસંદગી (idx) {
        સ્થિતિ XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            પ્રતિજ્ઞા(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            તોડો;
        સ્થિતિ LAYOUT_INDEX_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            તોડો;
        સ્થિતિ LAYOUT_INDEX_MULTIPLE:
        સ્થિતિ LAYOUT_INDEX_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            તોડો;
        સ્થિતિ LAYOUT_INDEX_SINGLE:
        સ્થિતિ LAYOUT_INDEX_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        મૂળભૂત:
            /* Mere layout index */
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = idx;
            m->mapping.layout_idx_max = idx + 1;
            m->mapping.layouts_candidates_mask = UINT32_C(1) << idx;
    }
    m->mapping.has_multiple_layouts = (
        m->mapping.layout_idx_max > m->mapping.layout_idx_min &&
        m->mapping.layout_idx_max - m->mapping.layout_idx_min > 1
    );
}

સ્થિર શૂન્ય
matcher_mapping_set_kccgst(સંરચના matcher *m, સંરચના scanner *s, સંરચના sval ident)
{
    ગણના rules_kccgst kccgst;
    સંરચના sval kccgst_sval;

    માટે (kccgst = 0; kccgst < KCCGST_અંક_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        જો (svaleq(rules_kccgst_svals[kccgst], ident))
            તોડો;
    }

    /* Not found. */
    જો (kccgst >= KCCGST_અંક_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned પૂર્ણાંક) ident.len, ident.start);
        m->mapping.active = false;
        પાછા;
    }

    જો (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned પૂર્ણાંક) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        પાછા;
    }

    m->mapping.kccgst_at_pos[m->mapping.અંક_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.અંક_kccgst++;
}

સ્થિર bool
matcher_mapping_verify(સંરચના matcher *m, સંરચના scanner *s)
{
    જો (m->mapping.અંક_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        જાઓ skip;
    }

    જો (m->mapping.અંક_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        જાઓ skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    જો (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        પ્રતિજ્ઞા(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        પસંદગી (m->mapping.layout_idx) {
            સ્થિતિ LAYOUT_INDEX_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                જો (darray_size(m->rmlvo.layouts) > 1)
                    જાઓ skip;
                તોડો;
            સ્થિતિ LAYOUT_INDEX_MULTIPLE:
            સ્થિતિ LAYOUT_INDEX_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                જો (darray_size(m->rmlvo.layouts) < 2)
                    જાઓ skip;
                તોડો;
            સ્થિતિ LAYOUT_INDEX_ANY:
            સ્થિતિ LAYOUT_INDEX_FIRST:
                /* No restrictions */
                તોડો;
            મૂળભૂત:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                જો (darray_size(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_size(m->rmlvo.layouts))
                    જાઓ skip;
        }
    }

    જો (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        પ્રતિજ્ઞા(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        પસંદગી (m->mapping.variant_idx) {
            સ્થિતિ LAYOUT_INDEX_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                જો (darray_size(m->rmlvo.variants) > 1)
                    જાઓ skip;
                તોડો;
            સ્થિતિ LAYOUT_INDEX_ANY:
            સ્થિતિ LAYOUT_INDEX_LATER:
            સ્થિતિ LAYOUT_INDEX_FIRST:
                /* No restriction */
                તોડો;
            મૂળભૂત:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                જો (darray_size(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_size(m->rmlvo.variants))
                    જાઓ skip;
        }
    }

    પાછા true;

skip:
    m->mapping.active = false;
    પાછા false;
}

સ્થિર શૂન્ય
matcher_rule_start_new(સંરચના matcher *m)
{
    memset(&m->rule, 0, કદ(m->rule));
    m->rule.skip = !m->mapping.active;
}

સ્થિર શૂન્ય
matcher_rule_set_mlvo_common(સંરચના matcher *m, સંરચના scanner *s,
                             સંરચના sval ident,
                             ગણના mlvo_match_type match_type)
{
    જો (m->rule.અંક_mlvo_કિંમતો >= m->mapping.અંક_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        પાછા;
    }
    m->rule.match_type_at_pos[m->rule.અંક_mlvo_કિંમતો] = match_type;
    m->rule.mlvo_કિંમત_at_pos[m->rule.અંક_mlvo_કિંમતો] = ident;
    m->rule.અંક_mlvo_કિંમતો++;
}

સ્થિર શૂન્ય
matcher_rule_set_mlvo_wildcard(સંરચના matcher *m, સંરચના scanner *s,
                               ગણના mlvo_match_type match_type)
{
    સંરચના sval dummy = SVAL(ખાલી, 0);
    matcher_rule_set_mlvo_common(m, s, dummy, match_type);
}

સ્થિર શૂન્ય
matcher_rule_set_mlvo_group(સંરચના matcher *m, સંરચના scanner *s,
                            સંરચના sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_GROUP);
}

સ્થિર શૂન્ય
matcher_rule_set_mlvo(સંરચના matcher *m, સંરચના scanner *s,
                      સંરચના sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_NORMAL);
}

સ્થિર શૂન્ય
matcher_rule_set_kccgst(સંરચના matcher *m, સંરચના scanner *s,
                        સંરચના sval ident)
{
    જો (m->rule.અંક_kccgst_કિંમતો >= m->mapping.અંક_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        પાછા;
    }
    m->rule.kccgst_કિંમત_at_pos[m->rule.અંક_kccgst_કિંમતો] = ident;
    m->rule.અંક_kccgst_કિંમતો++;
}

સ્થિર bool
match_group(સંરચના matcher *m, સંરચના sval group_name, સંરચના sval to)
{
    સંરચના group *group = ખાલી;
    સંરચના sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        જો (svaleq(group->name, group_name)) {
            found = true;
            તોડો;
        }
    }

    જો (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        પાછા false;
    }

    darray_foreach(element, group->elements)
        જો (svaleq(to, *element))
            પાછા true;

    પાછા false;
}

સ્થિર bool
match_કિંમત(સંરચના matcher *m, અચલ સંરચના sval val, અચલ સંરચના sval to,
            ગણના mlvo_match_type match_type,
            ગણના wildcard_match_type wildcard_type)
{
    પસંદગી (match_type) {
        સ્થિતિ MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            પાછા wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        સ્થિતિ MLVO_MATCH_WILDCARD_NONE:
            પાછા !to.len;
        સ્થિતિ MLVO_MATCH_WILDCARD_SOME:
            પાછા !!to.len;
        સ્થિતિ MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            પાછા true;
        સ્થિતિ MLVO_MATCH_GROUP:
            પાછા match_group(m, val, to);
        મૂળભૂત:
            પ્રતિજ્ઞા(match_type == MLVO_MATCH_NORMAL);
            પાછા svaleq(val, to);
    }
}

સ્થિર bool
match_કિંમત_and_mark(સંરચના matcher *m, અચલ સંરચના sval val,
                     સંરચના matched_sval *to, ગણના mlvo_match_type match_type,
                     ગણના wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_કિંમત(m, val, to->sval, match_type, wildcard_type);
    જો (matched)
        to->matched |= layouts;
    પાછા matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
સ્થિર bool
expand_rmlvo_in_kccgst_કિંમત(સંરચના matcher *m, સંરચના scanner *s,
                             સંરચના sval કિંમત, xkb_layout_index_t layout_idx,
                             darray_char *expanded, size_t *i)
{
    અચલ અક્ષર *str = કિંમત.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    ગણના rules_mlvo mlv;
    xkb_layout_index_t idx;
    અક્ષર pfx, sfx;
    સંરચના matched_sval *expanded_કિંમત;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    જો (str[*i] == 'i' &&
        (*i + 1 == કિંમત.len || is_merge_mode_prefix(str[*i + 1])))
    {
        જો (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_INDEX_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            જાઓ error;
        }
        (*i)++;
        અક્ષર index_str[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
        અચલ પૂર્ણાંક ગણતરી = snprintf(index_str, કદ(index_str), "%"PRIu32,
                                   layout_idx + 1);
        પ્રતિજ્ઞા(ગણતરી > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, index_str, (darray_size_t)ગણતરી);
        પાછા true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    જો (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        જો (str[*i] == '(') sfx = ')';
        જો (++(*i) >= કિંમત.len) જાઓ error;
    }

    /* Mandatory model/layout/variant specifier. */
    પસંદગી (str[(*i)++]) {
    સ્થિતિ 'm': mlv = MLVO_MODEL; તોડો;
    સ્થિતિ 'l': mlv = MLVO_LAYOUT; તોડો;
    સ્થિતિ 'v': mlv = MLVO_VARIANT; તોડો;
    મૂળભૂત: જાઓ error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_index = false;
    જો (*i < કિંમત.len && str[*i] == '[') {
        જો (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            જાઓ error;
        }

        પૂર્ણાંક consumed = extract_layout_index(str + (*i), કિંમત.len - (*i), &idx);
        જો (consumed <= 0) જાઓ error;
        જો (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            પ્રતિજ્ઞા(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_index = true;
        }
        *i += (size_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    જો (sfx != 0) {
        જો (*i >= કિંમત.len) જાઓ error;
        જો (str[(*i)++] != sfx) જાઓ error;
    }

    /* Get the expanded value. */
    expanded_કિંમત = ખાલી;

    જો (mlv == MLVO_LAYOUT) {
        જો (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            જો (darray_size(m->rmlvo.layouts) == 1)
                expanded_કિંમત = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } નહીંતર જો (idx < darray_size(m->rmlvo.layouts) &&
                   (variable_index || darray_size(m->rmlvo.layouts) > 1)) {
                expanded_કિંમત = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    નહીંતર જો (mlv == MLVO_VARIANT) {
        જો (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            જો (darray_size(m->rmlvo.variants) == 1)
                expanded_કિંમત = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } નહીંતર જો (idx < darray_size(m->rmlvo.variants) &&
                   (variable_index || darray_size(m->rmlvo.variants) > 1)) {
                expanded_કિંમત = &darray_item(m->rmlvo.variants, idx);
        }
    }
    નહીંતર જો (mlv == MLVO_MODEL) {
        expanded_કિંમત = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    જો (!expanded_કિંમત || expanded_કિંમત->sval.len == 0) {
        પાછા true;
    }

    જો (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_કિંમત->sval.start,
                                 (darray_size_t) expanded_કિંમત->sval.len);
    જો (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    જો (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        અચલ xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_કિંમત->matched |= layouts;
    } નહીંતર {
        expanded_કિંમત->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    પાછા true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    પાછા false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
સ્થિર શૂન્ય
expand_qualifier_in_kccgst_કિંમત(
    સંરચના matcher *m, સંરચના scanner *s,
    સંરચના sval કિંમત, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_size_t prefix_idx, size_t *i)
{
    અચલ અક્ષર *str = કિંમત.start;

    /* “all” followed by nothing or by a layout separator */
    જો (*i + 2 < કિંમત.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == કિંમત.len || is_merge_mode_prefix(str[*i + 3]))) {
        જો (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_MESSAGE_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        જો (darray_size(m->rmlvo.layouts) > 1) {
            અક્ષર layout_index[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
            અચલ darray_size_t prefix_length =
                darray_size(*expanded) - prefix_idx - 1;
            માટે (xkb_layout_index_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_size(m->rmlvo.layouts));
                 l++)
            {
                જો (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_length);
                /* Append index */
                અચલ પૂર્ણાંક ગણતરી = snprintf(layout_index, કદ(layout_index),
                                           "%"PRIu32, l + 1);
                પ્રતિજ્ઞા(ગણતરી > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_index,
                                             (darray_size_t)ગણતરી);
            }
        }
        *i += 3;
    }
}

સ્થિર અંતઃરેખ શૂન્ય
#ifdef _MSC_VER
concat_kccgst(darray_char *into, darray_size_t size, _In_reads_(size) અચલ અક્ષર* from)
#નહીંતર
concat_kccgst(darray_char *into, darray_size_t size, અચલ અક્ષર from[સ્થિર size])
#endif
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    અચલ bool from_plus = is_merge_mode_prefix(from[0]);
    જો (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, size);
    } નહીંતર {
        અચલ અક્ષર ch =
            (અક્ષર) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        અચલ bool into_plus = is_merge_mode_prefix(ch);
        જો (into_plus)
            darray_prepends_nullterminate(*into, from, size);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
સ્થિર bool
append_expanded_kccgst_કિંમત(સંરચના matcher *m, સંરચના scanner *s,
                             bool merge, darray_char *to, સંરચના sval કિંમત,
                             xkb_layout_index_t layout_idx)
{
    અચલ અક્ષર *str = કિંમત.start;
    darray_char expanded = darray_new();
    darray_size_t last_item_idx = 0;
    bool has_separator = false;

    માટે (size_t i = 0; i < કિંમત.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        પસંદગી (str[i]) {
            /* Qualifier */
            સ્થિતિ ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_કિંમત(m, s, કિંમત, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                તોડો;
            /* Expansion */
            સ્થિતિ '%':
                i++;
                જો (i >= કિંમત.len ||
                    !expand_rmlvo_in_kccgst_કિંમત(m, s, કિંમત, layout_idx,
                                                  &expanded, &i))
                        જાઓ error;
                તોડો;
            /* New item */
            સ્થિતિ MERGE_OVERRIDE_PREFIX:
            સ્થિતિ MERGE_AUGMENT_PREFIX:
            સ્થિતિ MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_size(expanded) - 1;
                has_separator = true;
                તોડો;
            /* Just a normal character. */
            મૂળભૂત:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    જો (merge) {
        જો (!darray_empty(expanded))
            concat_kccgst(to, darray_size(expanded), darray_વસ્તુઓ(expanded));
    } નહીંતર {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    પાછા true;

error:
    darray_free(expanded);
    પાછા false;
}

સ્થિર bool
matcher_append_pending_kccgst(સંરચના matcher *m)
{
    જો (!m->mapping.has_multiple_layouts)
        પાછા true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    માટે (kccgst_index_t i = 0; i < m->mapping.અંક_kccgst; i++) {
        અચલ ગણના rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        માટે (xkb_layout_index_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            નોંધણી અચલ સંરચના kccgst_buffer* અચલ buf = &m->pending_kccgst;
            size_t offset = 0;
            માટે (darray_size_t k = 0; k < darray_size(buf->slices); k++) {
                નોંધણી અચલ સંરચના kccgst_buffer_slice * અચલ slice =
                    &darray_item(buf->slices, k);
                જો (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->length)
                    concat_kccgst(&m->kccgst[kccgst], slice->length,
                                  darray_વસ્તુઓ(buf->buffer) + offset);
                offset += slice->length;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    પાછા true;
}

સ્થિર શૂન્ય
matcher_rule_verify(સંરચના matcher *m, સંરચના scanner *s)
{
    જો (m->rule.અંક_mlvo_કિંમતો != m->mapping.અંક_mlvo ||
        m->rule.અંક_kccgst_કિંમતો != m->mapping.અંક_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: must have same number of values "
                    "as mapping line; ignoring rule");
        m->rule.skip = true;
    }
}

/*
 * NOTE: The wildcard `*` behaves differently depending on the MLVO field:
 * - model and options: matches any value, including empty;
 * - layout and variant: matches any *non-empty* value.
 *
 * This aligns with the implementation in libxkbfile and xserver, with
 * the exception of options, where `*` is entirely ignored.
 *
 * The underlying rationale for this discrepancy across MLVO fields is
 * undocumented.
 *
 * In xkbcommon, `*` behaves identically for both model and options to
 * maintain consistency and simplicity. This divergence is unlikely
 * to have a practical impact: to the best of our knowledge, `*` has
 * no real-world use for the options field.
 *
 * | MLVO field | Wildcard `*` matches        |
 * | ---------- | --------------------------- |
 * | Model      | Any value (including empty) |
 * | Option     | Any value (including empty) |
 * | Layout     | Non-empty values only       |
 * | Variant    | Non-empty values only       |
 *
 */

/** Match the `model` MLVO field
 *
 * `model` input is a single value.
 */
સ્થિર bool
matcher_rule_match_model(સંરચના matcher *m, અચલ સંરચના sval કિંમત,
                         ગણના mlvo_match_type match_type)
{
    પાછા match_કિંમત_and_mark(
        m, કિંમત, &m->rmlvo.model, match_type, WILDCARD_MATCH_ALL,
        /* layout-independent */
        (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS
    );
}

/**
 * Match the `option` MLVO field.
 *
 * `option` input is a list of values (*not* layout-indexed).
 *
 * Matching an option field requires considering the entire list of input options
 * (e.g. `grp:menu_toggle,caps:swapescape`), because any number of them may
 * legitimately satisfy a single rule at once.
 *
 * - If the rule has no `layout` nor `variant` field, evaluation stops early on
 *   the first match.
 * - Otherwise each option potentially covers a different subset of the
 *   candidate layout indices. Candidates are narrowed by accumulating which
 *   candidates get covered:
 *
 *   - A layout-specific option (e.g. `grp:menu_toggle!1`, encoded as a non-zero
 *     `option->layouts` mask) can only cover candidates it applies to, so it
 *     is tried against `unmatched & option->layouts`.
 *   - A layout-generic option (e.g. `grp:menu_toggle`) is not restricted to
 *     specific layout indices, so a match covers every remaining candidate at
 *     once.
 *
 *   Matching stops early once every candidate is covered; otherwise every
 *   input option is tried. Any candidate left uncovered at the end cannot
 *   match the next RMLVO field (if any), so it is dropped from the candidates
 *   list.
 *
 * NOTE: The matching logic here differs from `model`, `layout`, and `variant`.
 * While the rule still provides a single value to match against, how that value
 * relates to candidate layout indices varies:
 *
 * | MLVO field | Input type                          | Candidates constraint   |
 * | ---------- | ----------------------------------- | ----------------------- |
 * | `model`    | single value                        | none                    |
 * | `layout`   | list of layout-indexed values       | 1-to-1                  |
 * | `variant`  | list of layout-indexed values       | 1-to-1                  |
 * | `option`   | list of values (not layout-indexed) | many-to-many            |
 *
 */
સ્થિર bool
matcher_rule_match_option(સંરચના matcher *m, અચલ સંરચના sval કિંમત,
                          ગણના mlvo_match_type match_type,
                          xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    પ્રતિજ્ઞા(!darray_empty(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_mask_t unmatched =
        (m->mapping.defined_mlvo_mask & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    સંરચના matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        અચલ xkb_layout_mask_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
        જો (matchable && match_કિંમત_and_mark(m, કિંમત, option, match_type,
                                              WILDCARD_MATCH_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            જો (!unmatched)
                તોડો;
        }
    }
    જો (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    પાછા matched;
}

/**
 * Match the `layout` or `variant` field
 *
 * Input is a list of layout-indexed values.
 *
 * The value of the rule’s field is tested against each remaining candidate
 * index using the corresponding RMLVO input list.
 *
 * - An index that fails to match can never satisfy this rule, so it is
 *   cleared from the candidates set. This narrows what later MLVO fields in
 *   the same rule still need to consider.
 * - An index that matches remains a candidate. Evaluation stops as soon as no
 *   candidates remain, since nothing left can satisfy the rule.
 */
સ્થિર bool
matcher_rule_match_layout_or_variant(સંરચના matcher *m,
                                     darray_matched_sval *આગત,
                                     અચલ સંરચના sval કિંમત,
                                     ગણના mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    પ્રતિજ્ઞા(!darray_empty(*આગત));

    bool matched = false;
    /* Loop over the layout index range */
    માટે (xkb_layout_index_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        અચલ xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        જો (layout & *candidates) {
            સંરચના matched_sval *to = &darray_item(*આગત, idx);
            જો (match_કિંમત_and_mark(m, કિંમત, to, match_type,
                                     WILDCARD_MATCH_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } નહીંતર {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    પાછા matched;
}

/**
 * Try to match the current rule against the RMLVO input; if every MLVO
 * field in the mapping matches, apply the rule’s KcCGST value(s).
 *
 * A mapping’s header line lists which MLVO fields it tests (some subset
 * of `model`, `layout`, `variant`, `option`, in any order) and which
 * KcCGST fields it produces. When the RMLVO input has several layouts
 * (e.g. `us,de,fr`), each rule needs to be checked against each layout
 * index independently. A candidate layout mask tracks the layout indices
 * valid for the current rule. Each MLVO field affects the candidates
 * layout indices differently:
 *
 * - `model` is layout-independent: it matches globally, for every
 *   candidate at once, or not at all.
 * - `layout` and `variant` narrow the mask directly: a candidate index
 *   survives only if the RMLVO value at that index matches the rule.
 * - `option` is different because the RMLVO input may contain multiple
 *   option values, possibly covering different candidates (or all of them,
 *   for a layout-generic option). A layout candidate survives only if some
 *   option matches and covers it.
 *
 * If any MLVO field fails to match, the rule cannot apply and this function
 * returns immediately, without changing the KcCGST output. For fields that
 * operate on layout indices (`layout`, `variant`, and `option`), this occurs
 * once no candidate layout indices remain.
 *
 * If every field matches, the KcCGST value(s) are processed for `%`-expansions
 * (e.g., `%l` for matched layout, `%v` for variant, `%m` for model, `%i` for
 * layout index) and applied against the remaining candidate layouts. If the
 * ruleset spans multiple layout indices and includes an `option` field, the
 * `%`-expanded values are buffered and subsequently merged to guarantee they
 * follow layout order first, rule order second.
 *
 * Within a rule set, a successful match normally terminates evaluation.
 * The only exception is rules containing an `option` field, which do not
 * terminate the set so that additional matching options may contribute.
 */
સ્થિર શૂન્ય
matcher_rule_apply_if_matches(સંરચના matcher *m, સંરચના scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_mask_t candidate_layouts = m->mapping.layouts_candidates_mask;

    /* Loop over MLVO pattern components */
    માટે (mlvo_index_t i = 0; i < m->mapping.અંક_mlvo; i++) {
        અચલ ગણના rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        અચલ સંરચના sval કિંમત = m->rule.mlvo_કિંમત_at_pos[i];
        અચલ ગણના mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        પસંદગી (mlvo) {
        સ્થિતિ MLVO_MODEL:
            matched = matcher_rule_match_model(m, કિંમત, match_type);
            તોડો;
        સ્થિતિ MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, કિંમત, match_type, &candidate_layouts
            );
            તોડો;
        સ્થિતિ MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, કિંમત, match_type, &candidate_layouts
            );
            તોડો;
        સ્થિતિ MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, કિંમત, match_type, &candidate_layouts
            );
            તોડો;
        મૂળભૂત: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_અંક_ENTRIES - 1,
                          "Unexpected MLVO field");
            પ્રતિજ્ઞા(!"Unreachable");
        }
        }

        જો (!matched)
            પાછા;
    }

    જો (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        માટે (xkb_layout_index_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            જો (candidate_layouts & (UINT32_C(1) << idx)) {
                માટે (kccgst_index_t i = 0; i < m->mapping.અંક_kccgst; i++) {
                    અચલ ગણના rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    અચલ સંરચના sval કિંમત = m->rule.kccgst_કિંમત_at_pos[i];
                    /*
                     * [NOTE] Layout index ranges and merging KcCGST values
                     *
                     * Layout indices match following first the order of the
                     * rules in the file, then their natural order. So do not
                     * merge with the output for now but buffer the resulting
                     * KcCGST value and wait reaching the end of the rule set.
                     *
                     * Because the rule set may also involve options, it may
                     * match multiple times for the *same* layout index. So
                     * buffer the result of *each* match.
                     *
                     * When the end of the rule set is reached, merge buffered
                     * KcCGST sequentially, following first the layouts order,
                     * then the order of the rules in the file.
                     *
                     * Example:
                     *
                     * ! model = symbols
                     *   *     = pc
                     * ! layout[any] option = symbols
                     *   C           1      = +c1:%i
                     *   C           2      = +c2:%i
                     *   B           3      = skip
                     *   B           4      = +b:%i
                     *
                     * The result of {layout: "A,B,C", options: "4,3,2,1"} is:
                     * symbols = pc+b:2+c1:3+c2:3.
                     *
                     * - `skip` was dropped because it has no explicit merge
                     *   mode;
                     * - although every rule was matched in order, the resulting
                     *   order of the symbols follows the order of the layouts,
                     *   so `+b` appears before `+c1` and `+c2`.
                     * - the relative order of the options for layout C follows
                     *   the order within the rule set, not the order of RMLVO.
                     */
                    સંરચના kccgst_buffer * અચલ buf = &m->pending_kccgst;
                    અચલ darray_size_t prev_buffer_length =
                        darray_size(buf->buffer);
                    append_expanded_kccgst_કિંમત(m, s, false, &buf->buffer,
                                                 કિંમત, idx);
                    અચલ uint32_t length = (uint32_t) (darray_size(buf->buffer)
                                          - prev_buffer_length);
                    અચલ સંરચના kccgst_buffer_slice slice = {
                        .length = length,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } નહીંતર {
        /* Numeric index or no index */
        માટે (kccgst_index_t i = 0; i < m->mapping.અંક_kccgst; i++) {
            ગણના rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            સંરચના sval કિંમત = m->rule.kccgst_કિંમત_at_pos[i];
            append_expanded_kccgst_કિંમત(m, s, true, &m->kccgst[kccgst], કિંમત,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    જો (!(is_mlvo_mask_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_mask &= ~candidate_layouts;
    }
}

સ્થિર ગણના rules_token
gettok(સંરચના matcher *m, સંરચના scanner *s)
{
    પાછા lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
સ્થિર bool
matcher_match(સંરચના matcher *m, સંરચના scanner *s,
              unsigned પૂર્ણાંક include_depth,
              અચલ અક્ષર *string, size_t len,
              અચલ અક્ષર *file_name)
{
    ગણના rules_token tok;

    જો (!m)
        પાછા false;

initial:
    પસંદગી (tok = gettok(m, s)) {
    સ્થિતિ TOK_BANG:
        જાઓ bang;
    સ્થિતિ TOK_END_OF_લીટી:
        જાઓ initial;
    સ્થિતિ TOK_END_OF_FILE:
        જાઓ finish;
    મૂળભૂત:
        જાઓ unexpected;
    }

bang:
    પસંદગી (tok = gettok(m, s)) {
    સ્થિતિ TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        જાઓ group_name;
    સ્થિતિ TOK_INCLUDE:
        જાઓ include_statement;
    સ્થિતિ TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        જાઓ mapping_mlvo;
    મૂળભૂત:
        જાઓ unexpected;
    }

group_name:
    પસંદગી (tok = gettok(m, s)) {
    સ્થિતિ TOK_EQUALS:
        જાઓ group_element;
    મૂળભૂત:
        જાઓ unexpected;
    }

group_element:
    પસંદગી (tok = gettok(m, s)) {
    સ્થિતિ TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        જાઓ group_element;
    સ્થિતિ TOK_END_OF_લીટી:
        જાઓ initial;
    મૂળભૂત:
        જાઓ unexpected;
    }

include_statement:
    પસંદગી (tok = gettok(m, s)) {
    સ્થિતિ TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        જાઓ include_statement_end;
    મૂળભૂત:
        જાઓ unexpected;
    }

include_statement_end:
    પસંદગી (tok = gettok(m, s)) {
    સ્થિતિ TOK_END_OF_લીટી:
        જાઓ initial;
    મૂળભૂત:
        જાઓ unexpected;
    }

mapping_mlvo:
    પસંદગી (tok = gettok(m, s)) {
    સ્થિતિ TOK_IDENTIFIER:
        જો (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        જાઓ mapping_mlvo;
    સ્થિતિ TOK_EQUALS:
        જાઓ mapping_kccgst;
    મૂળભૂત:
        જાઓ unexpected;
    }

mapping_kccgst:
    પસંદગી (tok = gettok(m, s)) {
    સ્થિતિ TOK_IDENTIFIER:
        જો (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        જાઓ mapping_kccgst;
    સ્થિતિ TOK_END_OF_લીટી:
        જો (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            જો (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_size(m->pending_kccgst.buffer) = 0;
                darray_size(m->pending_kccgst.slices) = 0;
            }
        }
        જાઓ rule_mlvo_first;
    મૂળભૂત:
        જાઓ unexpected;
    }

rule_mlvo_first:
    પસંદગી (tok = gettok(m, s)) {
    સ્થિતિ TOK_BANG:
        matcher_append_pending_kccgst(m);
        જાઓ bang;
    સ્થિતિ TOK_END_OF_લીટી:
        જાઓ rule_mlvo_first;
    સ્થિતિ TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        જાઓ finish;
    મૂળભૂત:
        matcher_rule_start_new(m);
        જાઓ rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    પસંદગી (tok) {
    સ્થિતિ TOK_IDENTIFIER:
        જો (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        જાઓ rule_mlvo;
    સ્થિતિ TOK_WILD_CARD_STAR:
        જો (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        જાઓ rule_mlvo;
    સ્થિતિ TOK_WILD_CARD_NONE:
        જો (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        જાઓ rule_mlvo;
    સ્થિતિ TOK_WILD_CARD_SOME:
        જો (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        જાઓ rule_mlvo;
    સ્થિતિ TOK_WILD_CARD_ANY:
        જો (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        જાઓ rule_mlvo;
    સ્થિતિ TOK_GROUP_NAME:
        જો (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        જાઓ rule_mlvo;
    સ્થિતિ TOK_EQUALS:
        જાઓ rule_kccgst;
    મૂળભૂત:
        જાઓ unexpected;
    }

rule_kccgst:
    પસંદગી (tok = gettok(m, s)) {
    સ્થિતિ TOK_IDENTIFIER:
        જો (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        જાઓ rule_kccgst;
    સ્થિતિ TOK_END_OF_લીટી:
        જો (!m->rule.skip)
            matcher_rule_verify(m, s);
        જો (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        જાઓ rule_mlvo_first;
    મૂળભૂત:
        જાઓ unexpected;
    }

unexpected:
    પસંદગી (tok) {
    સ્થિતિ TOK_ERROR:
        જાઓ error;
    મૂળભૂત:
        જાઓ state_error;
    }

finish:
    પાછા true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    પાછા false;
}
// NOLINTEND(readability-trivial-switch)

સ્થિર bool
read_rules_file(સંરચના xkb_context *ctx,
                સંરચના matcher *matcher,
                unsigned પૂર્ણાંક include_depth,
                FILE *file,
                અચલ અક્ષર *path)
{
    bool ret;
    અક્ષર *string;
    size_t size;
    સંરચના scanner scanner;

    જો (!map_file(file, &string, &size)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, strerror(errno));
        પાછા false;
    }

    scanner_init(&scanner, matcher->ctx, string, size, path, ખાલી);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    જો (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, size);
        પાછા false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, size, path);
    unmap_file(string, size);
    પાછા ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
સ્થિર bool
xkb_resolve_partial_rules(સંરચના xkb_context *ctx, અક્ષર *path, size_t path_size,
                          અચલ અક્ષર* rules, અચલ અક્ષર* suffix,
                          સંરચના matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    અક્ષર partial_rules[60]; /* Arbitrary, but we do not expect long names */
    જો (unlikely(!snprintf_safe(partial_rules, કદ(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        પાછા false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    unsigned પૂર્ણાંક offset = 0;
    FILE *file = ખાલી;
    અચલ size_t len = strlen(partial_rules);
    જ્યારે ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_size, &offset, false)) != ખાલી) {
        અચલ bool ok = read_rules_file(ctx, matcher, 0, file, path);
        fclose(file);
        જો (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            પાછા false;
        }
        offset++;
    }

    પાછા true;
}

સ્થિર bool
xkb_resolve_rules(સંરચના xkb_context *ctx,
                  અચલ અક્ષર* rules, સંરચના matcher *matcher,
                  સંરચના xkb_component_names *out,
                  xkb_layout_index_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    unsigned પૂર્ણાંક offset = 0;
    અક્ષર path[PATH_MAX];
    FILE * અચલ file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, strlen(rules), FILE_TYPE_RULES,
                                          path, કદ(path), &offset, true);
    જો (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        જાઓ err_out;
    }

    /*
     * Final rule set is equivalent to:
     *
     * ! include <include path 1>/rules/<rules>.pre  // only if defined
     * …
     * ! include <include path n>/rules/<rules>.pre  // only if defined
     * ! include <rules>                             // main rules file
     * ! include <include path 1>/rules/<rules>.post // only if defined
     * …
     * ! include <include path n>/rules/<rules>.post // only if defined
     */

    /* XKB extension: resolve optional <rules>.pre files */
    ret = xkb_resolve_partial_rules(ctx, path, કદ(path),
                                    rules, ".pre", matcher);
    જો (!ret)
        જાઓ err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    જો (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        જાઓ err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, કદ(path),
                                    rules, ".post", matcher);
    જો (!ret)
        જાઓ err_out;

    /* Check we got required components */
    જો (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        જાઓ err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, ખાલી);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, ખાલી);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, ખાલી);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, ખાલી);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, ખાલી);

    સંરચના matched_sval *mval = &matcher->rmlvo.model;
    જો (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_કિંમત,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (unsigned પૂર્ણાંક) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        જો (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_કિંમત,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned પૂર્ણાંક) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        જો (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_કિંમત,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned પૂર્ણાંક) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        જો (!mval->sval.len)
            ચાલુ;

        જો (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_કિંમત,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned પૂર્ણાંક) mval->sval.len, mval->sval.start, rules);
        } નહીંતર જો (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            અચલ xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            પ્રતિજ્ઞા(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_કિંમત,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (unsigned પૂર્ણાંક)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    જો (out->symbols != ખાલી && explicit_layouts != ખાલી) {
        *explicit_layouts = 1; /* at least one group */
        અચલ અક્ષર *symbols = out->symbols;
        /* Take the highest modifier */
        જ્યારે ((symbols = strchr(symbols, ':')) != ખાલી && symbols[1] != '\0') {
            xkb_layout_index_t group = 0;
            અચલ પૂર્ણાંક ગણતરી = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            જો (ગણતરી > 0 && (symbols[ગણતરી] == '\0' ||
                is_merge_mode_prefix(symbols[ગણતરી])) &&
                group > 0 && group <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, group);
                symbols += ગણતરી;
            }
        }
    }

err_out:
    જો (file)
        fclose(file);

    પાછા ret;
}

bool
xkb_components_from_rmlvo_builder(અચલ સંરચના xkb_rmlvo_builder *rmlvo,
                                  સંરચના xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts)
{
    અચલ અક્ષર *rules = rmlvo->rules;
    સંરચના matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    જો (!matcher)
        પાછા false;

    અચલ bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    પાછા ret;
}


bool
xkb_components_from_rules_names(સંરચના xkb_context *ctx,
                                અચલ સંરચના xkb_rule_names *rmlvo,
                                સંરચના xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts)
{
    સંરચના matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    જો (!matcher)
        પાછા false;

    અચલ bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    પાછા ret;
}
