/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcomp-priv.h"
#ضمن "context.h"
#ضمن "keymap.h"
#ضمن "messages-codes.h"
#ضمن "rules.h"
#ضمن "rmlvo.h"
#ضمن "include.h"
#ضمن "scanner-utils.h"
#ضمن "darray.h"
#ضمن "utils.h"
#ضمن "util-numbers.h"
#ضمن "util-paths.h"

#define MAX_INCLUDE_DEPTH 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
اتحاد lvalue {
    بنية sval string;
};

تعداد rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_LINE,
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

ساكن تعداد rules_token
lex(بنية scanner *s, اتحاد lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    طالما (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    𐡄𐡍 (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    𐡄𐡍 (scanner_eol(s)) {
        طالما (scanner_eol(s)) scanner_next(s);
        أرجع TOK_END_OF_LINE;
    }

    /* Escaped line continuation */
    𐡄𐡍 (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        𐡄𐡍 (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            أرجع TOK_ERROR;
        }
        scanner_next(s);
        اذهب skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    𐡄𐡍 (scanner_eof(s)) أرجع TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    تعداد rules_token token = TOK_IDENTIFIER;

    ثابت حرف next = scanner_next(s);
    بدل (next) {
    حالة '!':
        أرجع TOK_BANG;
    حالة '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        اكسر;
    حالة '*':
        𐡄𐡍 (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            اكسر;
        } وإلا {
            /* Legacy wild card */
            أرجع TOK_WILD_CARD_STAR;
        }
    حالة '<':
        /* Extended wild cards */
        𐡄𐡍 (scanner_rules_lit_token(s, "none>")) أرجع TOK_WILD_CARD_NONE;
        𐡄𐡍 (scanner_rules_lit_token(s, "some>")) أرجع TOK_WILD_CARD_SOME;
        𐡄𐡍 (scanner_rules_lit_token(s, "any>")) أرجع TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        أكد(scanner_rules_is_ident('<'));
        val->string.len = 1;
        اكسر;
    حالة '=':
        𐡄𐡍 (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            اكسر;
        } وإلا {
            أرجع TOK_EQUALS;
        }
    حالة 'i':
        𐡄𐡍 (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            أرجع TOK_INCLUDE;
        }
        /* Identifier */
        أكد(scanner_rules_is_ident('i'));
        val->string.len = 1;
        اكسر;
    افتراضي:
        𐡄𐡍 (likely(scanner_rules_is_ident(next))) {
            /* Identifier */
            val->string.len = 1;
        } وإلا {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.start = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    أكد(scanner_rules_is_ident(MERGE_OVERRIDE_PREFIX));
    أكد(scanner_rules_is_ident(MERGE_AUGMENT_PREFIX));
    أكد(scanner_rules_is_ident(MERGE_REPLACE_PREFIX));
    أكد(scanner_rules_is_ident('('));
    أكد(scanner_rules_is_ident(')'));
    val->string.len += scanner_rules_ident_token(s);

    𐡄𐡍 (unlikely(val->string.len == 0)) {
        𐡄𐡍 (token == TOK_GROUP_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } وإلا {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unrecognized token");
        }
        أرجع TOK_ERROR;
    }

    أرجع token;
}

/***====================================================================***/

تعداد rules_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    _MLVO_NUM_ENTRIES
};

تعداد {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

تعريفنوع uint8_t mlvo_index_t;
تعريفنوع uint8_t mlvo_mask_t;


ساكن ثابت بنية sval rules_mlvo_svals[_MLVO_NUM_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

تعداد rules_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    _KCCGST_NUM_ENTRIES
};

تعريفنوع uint8_t kccgst_index_t;
تعريفنوع uint8_t kccgst_mask_t;

ساكن ثابت بنية sval rules_kccgst_svals[_KCCGST_NUM_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

تعداد {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL ==
              (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
بنية matched_sval {
    بنية sval sval;
    /** Used for layout-specific options */
    xkb_layout_mask_t layouts;
    xkb_layout_mask_t matched;
};
تعريفنوع darray(بنية matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
بنية rule_names {
    بنية matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

بنية group {
    بنية sval name;
    darray_sval elements;
};

بنية mapping {
    تعداد rules_mlvo mlvo_at_pos[_MLVO_NUM_ENTRIES];
    mlvo_index_t num_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    اتحاد {
        بنية { xkb_layout_index_t layout_idx, variant_idx; };
        بنية { xkb_layout_index_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    اتحاد {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    تعداد rules_kccgst kccgst_at_pos[_KCCGST_NUM_ENTRIES];
    kccgst_index_t num_kccgst;
    kccgst_mask_t defined_kccgst_mask;
};

تعداد mlvo_match_type {
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

تعداد wildcard_match_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_MATCH_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_MATCH_ALL,
};

بنية rule {
    بنية sval mlvo_value_at_pos[_MLVO_NUM_ENTRIES];
    تعداد mlvo_match_type match_type_at_pos[_MLVO_NUM_ENTRIES];
    بنية sval kccgst_value_at_pos[_KCCGST_NUM_ENTRIES];
    mlvo_index_t num_mlvo_values;
    kccgst_index_t num_kccgst_values;
    bool skip;
};

#define SLICE_KCCGST_BIT_FIELD_SIZE 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_SIZE - 1)) &
                (_KCCGST_NUM_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
بنية kccgst_buffer_slice {
    uint32_t length:(32 - SLICE_KCCGST_BIT_FIELD_SIZE);
    تعداد rules_kccgst kccgst:SLICE_KCCGST_BIT_FIELD_SIZE;
    xkb_layout_index_t layout;
};

/* Buffer for pending KcCGST values */
بنية kccgst_buffer {
    darray_char buffer;
    /* Slice corresponding to each value in the buffer */
    darray(بنية kccgst_buffer_slice) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
بنية matcher {
    بنية xkb_context *ctx;
    /* Input.*/
    بنية rule_names rmlvo;
    اتحاد lvalue val;
    darray(بنية group) groups;
    /* Current mapping. */
    بنية mapping mapping;
    /* Current rule. */
    بنية rule rule;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    بنية kccgst_buffer pending_kccgst;
    /* Output. */
    darray_char kccgst[_KCCGST_NUM_ENTRIES];
};

ساكن بنية sval
strip_spaces(بنية sval v)
{
    طالما (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    طالما (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    أرجع v;
}

ساكن darray_matched_sval
split_comma_separated_mlvo(بنية xkb_context *ctx,
                           تعداد rules_mlvo mlvo, ثابت حرف *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    𐡄𐡍 (!s) {
        ensure_at_least_one_value:
        /* Label followed by a declaration is a C23 extension */ ;

        بنية matched_sval val = {
            .sval = SVAL(عدم, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        أرجع arr;
    }

    طالما (true) {
        بنية matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        طالما (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        𐡄𐡍 (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            ثابت حرف* ثابت layout_start = s;
            xkb_layout_index_t layout = XKB_LAYOUT_INVALID;

            صحيح count = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            𐡄𐡍 (count > 0) {
                /* Note: 1-indexed layout */
                s += count;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                𐡄𐡍 (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (unsigned صحيح) val.sval.len, val.sval.start);
                } وإلا 𐡄𐡍 (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (unsigned صحيح) val.sval.len, val.sval.start);
                } وإلا {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ثابت حرف* ثابت layout_index_end = s;
            طالما (*s != '\0' && *s != ',') { s++; }
            𐡄𐡍 (count <= 0 || layout_index_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (unsigned صحيح) (s - layout_start), layout_start,
                        (unsigned صحيح) val.sval.len, val.sval.start);
            }
        }

        𐡄𐡍 (val.sval.len || mlvo != MLVO_OPTION) {
            𐡄𐡍 (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                بنية matched_sval *prev;
                darray_foreach(prev, arr) {
                    𐡄𐡍 (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        𐡄𐡍 (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } وإلا 𐡄𐡍 (prev->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        اذهب next_value;
                    }
                }
            }
            darray_append(arr, val);
        }

next_value:
        𐡄𐡍 (*s == '\0') اكسر;
        𐡄𐡍 (*s == ',') s++;
    }

    𐡄𐡍 (darray_empty(arr))
        اذهب ensure_at_least_one_value;

    أرجع arr;
}

ساكن بنية matcher *
matcher_new_from_rmlvo(ثابت بنية xkb_rmlvo_builder *rmlvo, ثابت حرف **rules)
{
    بنية matcher *m = calloc(1, حجم(*m));
    𐡄𐡍 (!m)
        أرجع عدم;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    بنية xkb_rule_names names = {
        .rules = rmlvo->rules,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_empty(rmlvo->layouts)) ? عدم : "x",
        .variant = (darray_empty(rmlvo->layouts)) ? عدم : "x",
        .options = (darray_empty(rmlvo->options)) ? عدم : "x",
    };
    ثابت تعداد RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
                                                               &names);

    /* Initialize matcher with sanitized names, if relevant */
    𐡄𐡍 (changed & RMLVO_RULES) {
        *rules = names.rules;
    } وإلا {
        *rules = rmlvo->rules;
    }

    𐡄𐡍 (changed & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = names.model;
    } وإلا {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;

    أكد((changed & RMLVO_LAYOUT) || !(changed & RMLVO_VARIANT));
    𐡄𐡍 (changed & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = split_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      names.layout);
        m->rmlvo.variants = split_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       names.variant);
        𐡄𐡍 (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            𐡄𐡍 (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        } وإلا 𐡄𐡍 (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } وإلا {
        بنية xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            بنية matched_sval val = {
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

    𐡄𐡍 (changed & RMLVO_OPTIONS) {
        m->rmlvo.options = split_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      names.options);
    } وإلا {
        بنية xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            بنية matched_sval val = {
                .sval = SVAL(option->option, strlen_safe(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    أرجع m;
}

ساكن بنية matcher *
matcher_new_from_names(بنية xkb_context *ctx,
            ثابت بنية xkb_rule_names *rmlvo)
{
    بنية matcher *m = calloc(1, حجم(*m));
    𐡄𐡍 (!m)
        أرجع عدم;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    𐡄𐡍 (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        𐡄𐡍 (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
    } وإلا 𐡄𐡍 (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    أرجع m;
}

ساكن فراغ
matcher_free(بنية matcher *m)
{
    𐡄𐡍 (!m)
        أرجع;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    بنية group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.buffer);
    darray_free(m->pending_kccgst.slices);
    لكل (kccgst_index_t i = 0; i < (kccgst_index_t) _KCCGST_NUM_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    حرر(m);
}

ساكن فراغ
matcher_group_start_new(بنية matcher *m, بنية sval name)
{
    بنية group group = { .name = name, .elements = darray_new() };
    darray_append(m->groups, group);
}

ساكن فراغ
matcher_group_add_element(بنية matcher *m, بنية scanner *s,
                          بنية sval element)
{
    darray_append(darray_item(m->groups, darray_size(m->groups) - 1).elements,
                  element);
}

ساكن bool
read_rules_file(بنية xkb_context *ctx,
                بنية matcher *matcher,
                unsigned صحيح include_depth,
                FILE *file,
                ثابت حرف *path);

ساكن فراغ
matcher_include(بنية matcher *m, بنية scanner *parent_scanner,
                unsigned صحيح include_depth,
                بنية sval inc)
{
    𐡄𐡍 (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        أرجع;
    }

    ثابت حرف *stmt_file = inc.start;
    size_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    حرف buf[PATH_MAX] = {0};
    ثابت ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, حجم(buf));
    𐡄𐡍 (expanded < 0) {
        /* Error */
        أرجع;
    } وإلا 𐡄𐡍 (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (size_t) expanded;
        أكد(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned صحيح offset = 0;
    ثابت bool absolute_path = is_absolute_path(stmt_file);
    𐡄𐡍 (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        𐡄𐡍 (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            𐡄𐡍 (stmt_file_len < حجم(buf)) {
                memcpy(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } وإلا {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, حجم(buf),
                        (unsigned صحيح) stmt_file_len, stmt_file);
                أرجع;
            }
        } وإلا {
            /* %-expansion is always NULL-terminated */
            أكد(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } وإلا {
        /* Relative path: lookup the first XKB path */
        𐡄𐡍 (unlikely(expanded)) {
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
            file = عدم;
        } وإلا {
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, حجم(buf), &offset, true);
        }
    }

    طالما (file) {
        أكد(strlen_safe(buf) < حجم(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        fclose(file);
        𐡄𐡍 (ret)
            أرجع;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        𐡄𐡍 (absolute_path) {
            /* There is no point to search further if the path is absolute */
            اكسر;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, حجم(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (unsigned صحيح) stmt_file_len, stmt_file);
}

ساكن فراغ
matcher_mapping_start_new(بنية matcher *m)
{
    لكل (mlvo_index_t i = 0; i < (mlvo_index_t) _MLVO_NUM_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = _MLVO_NUM_ENTRIES;
    لكل (kccgst_index_t i = 0; i < (kccgst_index_t) _KCCGST_NUM_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = _KCCGST_NUM_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.num_mlvo = m->mapping.num_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
ساكن صحيح
parse_layout_int_index_tail(ثابت حرف *s, size_t max_len,
                            xkb_layout_index_t *out)
{
    𐡄𐡍 (max_len < 3)
        أرجع -1;
    uint32_t val = 0;
    ثابت صحيح count = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    𐡄𐡍 (count <= 0 || s[1 + count] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        أرجع -1;
    /* To zero-based index. */
    *out = val - 1;
    أرجع count + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
ساكن صحيح
extract_layout_index(ثابت حرف *s, size_t max_len, xkb_layout_index_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    𐡄𐡍 (max_len < 3 || s[0] != '[')
        أرجع -1;
    𐡄𐡍 (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_index_t)(s[1] - '1');
        أرجع 3;
    }
    𐡄𐡍 (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        أرجع 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    أرجع parse_layout_int_index_tail(s, max_len, out);
}

/* Special layout indices */
تعداد layout_index_ranges {
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
ساكن bool
extract_mapping_layout_index(ثابت حرف *s, size_t max_len,
                             xkb_layout_index_t *out)
{
    /* Check for minimal `[` + index + `]` */
    𐡄𐡍 (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        أرجع false;
    }

    /* Small numeric index */
    𐡄𐡍 (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_index_t)(s[1] - '1');
        أرجع true;
    }

    /* Named indices ranges */
    ساكن ثابت بنية {
        ثابت حرف* name;
        size_t size;
        xkb_layout_index_t value;
    } names[] = {
        #define INDEX_RANGE(tail, value) \
            { (tail), حجم(tail), (xkb_layout_index_t)(value) }
        INDEX_RANGE("single]",   LAYOUT_INDEX_SINGLE),
        INDEX_RANGE("first]",    LAYOUT_INDEX_FIRST),
        INDEX_RANGE("later]",    LAYOUT_INDEX_LATER),
        INDEX_RANGE("multiple]", LAYOUT_INDEX_MULTIPLE),
        INDEX_RANGE("any]",      LAYOUT_INDEX_ANY),
        #undef INDEX_RANGE
    };

    لكل (size_t k = 0; k < ARRAY_SIZE(names); k++) {
        𐡄𐡍 (max_len == names[k].size &&
            memcmp(&s[1], names[k].name, names[k].size - 1) == 0) {
            *out = names[k].value;
            أرجع true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    ثابت صحيح count = parse_layout_int_index_tail(s, max_len, out);
    أرجع count > 0 && (size_t)count == max_len;
}

ساكن مضمن bool
is_mlvo_mask_defined(بنية matcher *m, تعداد rules_mlvo mlvo)
{
    أرجع m->mapping.defined_mlvo_mask & (1u << mlvo);
}

ساكن فراغ
matcher_mapping_set_mlvo(بنية matcher *m, بنية scanner *s,
                         بنية sval ident)
{
    تعداد rules_mlvo mlvo;
    بنية sval mlvo_sval;

    لكل (mlvo = 0; mlvo < _MLVO_NUM_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        𐡄𐡍 (svaleq_prefix(mlvo_sval, ident))
            اكسر;
    }

    /* Not found. */
    𐡄𐡍 (mlvo >= _MLVO_NUM_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned صحيح) ident.len, ident.start);
        m->mapping.active = false;
        أرجع;
    }

    𐡄𐡍 (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned صحيح) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        أرجع;
    }

    /* If there are leftovers still, it must be an index. */
    𐡄𐡍 (mlvo_sval.len < ident.len) {
        xkb_layout_index_t idx;
        𐡄𐡍 (!extract_mapping_layout_index(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (unsigned صحيح) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            أرجع;
        }

        𐡄𐡍 (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        وإلا 𐡄𐡍 (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        وإلا {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (unsigned صحيح) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            أرجع;
        }
    } وإلا 𐡄𐡍 (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    } وإلا 𐡄𐡍 (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    𐡄𐡍 (((mlvo == MLVO_LAYOUT && is_mlvo_mask_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_mask_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.active = false;
        أرجع;
    }

    m->mapping.mlvo_at_pos[m->mapping.num_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.num_mlvo++;
}

ساكن فراغ
matcher_mapping_set_layout_bounds(بنية matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_index_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    بدل (idx) {
        حالة XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            أكد(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            اكسر;
        حالة LAYOUT_INDEX_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            اكسر;
        حالة LAYOUT_INDEX_MULTIPLE:
        حالة LAYOUT_INDEX_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            اكسر;
        حالة LAYOUT_INDEX_SINGLE:
        حالة LAYOUT_INDEX_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        افتراضي:
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

ساكن فراغ
matcher_mapping_set_kccgst(بنية matcher *m, بنية scanner *s, بنية sval ident)
{
    تعداد rules_kccgst kccgst;
    بنية sval kccgst_sval;

    لكل (kccgst = 0; kccgst < _KCCGST_NUM_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        𐡄𐡍 (svaleq(rules_kccgst_svals[kccgst], ident))
            اكسر;
    }

    /* Not found. */
    𐡄𐡍 (kccgst >= _KCCGST_NUM_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned صحيح) ident.len, ident.start);
        m->mapping.active = false;
        أرجع;
    }

    𐡄𐡍 (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned صحيح) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        أرجع;
    }

    m->mapping.kccgst_at_pos[m->mapping.num_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.num_kccgst++;
}

ساكن bool
matcher_mapping_verify(بنية matcher *m, بنية scanner *s)
{
    𐡄𐡍 (m->mapping.num_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        اذهب skip;
    }

    𐡄𐡍 (m->mapping.num_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        اذهب skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    𐡄𐡍 (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        أكد(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        بدل (m->mapping.layout_idx) {
            حالة LAYOUT_INDEX_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                𐡄𐡍 (darray_size(m->rmlvo.layouts) > 1)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_INDEX_MULTIPLE:
            حالة LAYOUT_INDEX_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                𐡄𐡍 (darray_size(m->rmlvo.layouts) < 2)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_INDEX_ANY:
            حالة LAYOUT_INDEX_FIRST:
                /* No restrictions */
                اكسر;
            افتراضي:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                𐡄𐡍 (darray_size(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_size(m->rmlvo.layouts))
                    اذهب skip;
        }
    }

    𐡄𐡍 (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        أكد(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        بدل (m->mapping.variant_idx) {
            حالة LAYOUT_INDEX_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                𐡄𐡍 (darray_size(m->rmlvo.variants) > 1)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_INDEX_ANY:
            حالة LAYOUT_INDEX_LATER:
            حالة LAYOUT_INDEX_FIRST:
                /* No restriction */
                اكسر;
            افتراضي:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                𐡄𐡍 (darray_size(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_size(m->rmlvo.variants))
                    اذهب skip;
        }
    }

    أرجع true;

skip:
    m->mapping.active = false;
    أرجع false;
}

ساكن فراغ
matcher_rule_start_new(بنية matcher *m)
{
    memset(&m->rule, 0, حجم(m->rule));
    m->rule.skip = !m->mapping.active;
}

ساكن فراغ
matcher_rule_set_mlvo_common(بنية matcher *m, بنية scanner *s,
                             بنية sval ident,
                             تعداد mlvo_match_type match_type)
{
    𐡄𐡍 (m->rule.num_mlvo_values >= m->mapping.num_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        أرجع;
    }
    m->rule.match_type_at_pos[m->rule.num_mlvo_values] = match_type;
    m->rule.mlvo_value_at_pos[m->rule.num_mlvo_values] = ident;
    m->rule.num_mlvo_values++;
}

ساكن فراغ
matcher_rule_set_mlvo_wildcard(بنية matcher *m, بنية scanner *s,
                               تعداد mlvo_match_type match_type)
{
    بنية sval dummy = SVAL(عدم, 0);
    matcher_rule_set_mlvo_common(m, s, dummy, match_type);
}

ساكن فراغ
matcher_rule_set_mlvo_group(بنية matcher *m, بنية scanner *s,
                            بنية sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_GROUP);
}

ساكن فراغ
matcher_rule_set_mlvo(بنية matcher *m, بنية scanner *s,
                      بنية sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_NORMAL);
}

ساكن فراغ
matcher_rule_set_kccgst(بنية matcher *m, بنية scanner *s,
                        بنية sval ident)
{
    𐡄𐡍 (m->rule.num_kccgst_values >= m->mapping.num_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        أرجع;
    }
    m->rule.kccgst_value_at_pos[m->rule.num_kccgst_values] = ident;
    m->rule.num_kccgst_values++;
}

ساكن bool
match_group(بنية matcher *m, بنية sval group_name, بنية sval to)
{
    بنية group *group = عدم;
    بنية sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        𐡄𐡍 (svaleq(group->name, group_name)) {
            found = true;
            اكسر;
        }
    }

    𐡄𐡍 (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        أرجع false;
    }

    darray_foreach(element, group->elements)
        𐡄𐡍 (svaleq(to, *element))
            أرجع true;

    أرجع false;
}

ساكن bool
match_value(بنية matcher *m, ثابت بنية sval val, ثابت بنية sval to,
            تعداد mlvo_match_type match_type,
            تعداد wildcard_match_type wildcard_type)
{
    بدل (match_type) {
        حالة MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            أرجع wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        حالة MLVO_MATCH_WILDCARD_NONE:
            أرجع !to.len;
        حالة MLVO_MATCH_WILDCARD_SOME:
            أرجع !!to.len;
        حالة MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            أرجع true;
        حالة MLVO_MATCH_GROUP:
            أرجع match_group(m, val, to);
        افتراضي:
            أكد(match_type == MLVO_MATCH_NORMAL);
            أرجع svaleq(val, to);
    }
}

ساكن bool
match_value_and_mark(بنية matcher *m, ثابت بنية sval val,
                     بنية matched_sval *to, تعداد mlvo_match_type match_type,
                     تعداد wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_value(m, val, to->sval, match_type, wildcard_type);
    𐡄𐡍 (matched)
        to->matched |= layouts;
    أرجع matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
ساكن bool
expand_rmlvo_in_kccgst_value(بنية matcher *m, بنية scanner *s,
                             بنية sval value, xkb_layout_index_t layout_idx,
                             darray_char *expanded, size_t *i)
{
    ثابت حرف *str = value.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    تعداد rules_mlvo mlv;
    xkb_layout_index_t idx;
    حرف pfx, sfx;
    بنية matched_sval *expanded_value;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    𐡄𐡍 (str[*i] == 'i' &&
        (*i + 1 == value.len || is_merge_mode_prefix(str[*i + 1])))
    {
        𐡄𐡍 (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_INDEX_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            اذهب error;
        }
        (*i)++;
        حرف index_str[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
        ثابت صحيح count = snprintf(index_str, حجم(index_str), "%"PRIu32,
                                   layout_idx + 1);
        أكد(count > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, index_str, (darray_size_t)count);
        أرجع true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    𐡄𐡍 (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        𐡄𐡍 (str[*i] == '(') sfx = ')';
        𐡄𐡍 (++(*i) >= value.len) اذهب error;
    }

    /* Mandatory model/layout/variant specifier. */
    بدل (str[(*i)++]) {
    حالة 'm': mlv = MLVO_MODEL; اكسر;
    حالة 'l': mlv = MLVO_LAYOUT; اكسر;
    حالة 'v': mlv = MLVO_VARIANT; اكسر;
    افتراضي: اذهب error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_index = false;
    𐡄𐡍 (*i < value.len && str[*i] == '[') {
        𐡄𐡍 (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            اذهب error;
        }

        صحيح consumed = extract_layout_index(str + (*i), value.len - (*i), &idx);
        𐡄𐡍 (consumed <= 0) اذهب error;
        𐡄𐡍 (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            أكد(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_index = true;
        }
        *i += (size_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    𐡄𐡍 (sfx != 0) {
        𐡄𐡍 (*i >= value.len) اذهب error;
        𐡄𐡍 (str[(*i)++] != sfx) اذهب error;
    }

    /* Get the expanded value. */
    expanded_value = عدم;

    𐡄𐡍 (mlv == MLVO_LAYOUT) {
        𐡄𐡍 (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            𐡄𐡍 (darray_size(m->rmlvo.layouts) == 1)
                expanded_value = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } وإلا 𐡄𐡍 (idx < darray_size(m->rmlvo.layouts) &&
                   (variable_index || darray_size(m->rmlvo.layouts) > 1)) {
                expanded_value = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    وإلا 𐡄𐡍 (mlv == MLVO_VARIANT) {
        𐡄𐡍 (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            𐡄𐡍 (darray_size(m->rmlvo.variants) == 1)
                expanded_value = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } وإلا 𐡄𐡍 (idx < darray_size(m->rmlvo.variants) &&
                   (variable_index || darray_size(m->rmlvo.variants) > 1)) {
                expanded_value = &darray_item(m->rmlvo.variants, idx);
        }
    }
    وإلا 𐡄𐡍 (mlv == MLVO_MODEL) {
        expanded_value = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    𐡄𐡍 (!expanded_value || expanded_value->sval.len == 0) {
        أرجع true;
    }

    𐡄𐡍 (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_value->sval.start,
                                 (darray_size_t) expanded_value->sval.len);
    𐡄𐡍 (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    𐡄𐡍 (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ثابت xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_value->matched |= layouts;
    } وإلا {
        expanded_value->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    أرجع true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    أرجع false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
ساكن فراغ
expand_qualifier_in_kccgst_value(
    بنية matcher *m, بنية scanner *s,
    بنية sval value, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_size_t prefix_idx, size_t *i)
{
    ثابت حرف *str = value.start;

    /* “all” followed by nothing or by a layout separator */
    𐡄𐡍 (*i + 2 < value.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == value.len || is_merge_mode_prefix(str[*i + 3]))) {
        𐡄𐡍 (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_MESSAGE_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        𐡄𐡍 (darray_size(m->rmlvo.layouts) > 1) {
            حرف layout_index[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
            ثابت darray_size_t prefix_length =
                darray_size(*expanded) - prefix_idx - 1;
            لكل (xkb_layout_index_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_size(m->rmlvo.layouts));
                 l++)
            {
                𐡄𐡍 (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_length);
                /* Append index */
                ثابت صحيح count = snprintf(layout_index, حجم(layout_index),
                                           "%"PRIu32, l + 1);
                أكد(count > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_index,
                                             (darray_size_t)count);
            }
        }
        *i += 3;
    }
}

ساكن مضمن فراغ
#ifdef _MSC_VER
concat_kccgst(darray_char *into, darray_size_t size, _In_reads_(size) ثابت حرف* from)
#وإلا
concat_kccgst(darray_char *into, darray_size_t size, ثابت حرف from[ساكن size])
#endif
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ثابت bool from_plus = is_merge_mode_prefix(from[0]);
    𐡄𐡍 (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, size);
    } وإلا {
        ثابت حرف ch =
            (حرف) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        ثابت bool into_plus = is_merge_mode_prefix(ch);
        𐡄𐡍 (into_plus)
            darray_prepends_nullterminate(*into, from, size);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
ساكن bool
append_expanded_kccgst_value(بنية matcher *m, بنية scanner *s,
                             bool merge, darray_char *to, بنية sval value,
                             xkb_layout_index_t layout_idx)
{
    ثابت حرف *str = value.start;
    darray_char expanded = darray_new();
    darray_size_t last_item_idx = 0;
    bool has_separator = false;

    لكل (size_t i = 0; i < value.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        بدل (str[i]) {
            /* Qualifier */
            حالة ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_value(m, s, value, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                اكسر;
            /* Expansion */
            حالة '%':
                i++;
                𐡄𐡍 (i >= value.len ||
                    !expand_rmlvo_in_kccgst_value(m, s, value, layout_idx,
                                                  &expanded, &i))
                        اذهب error;
                اكسر;
            /* New item */
            حالة MERGE_OVERRIDE_PREFIX:
            حالة MERGE_AUGMENT_PREFIX:
            حالة MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_size(expanded) - 1;
                has_separator = true;
                اكسر;
            /* Just a normal character. */
            افتراضي:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    𐡄𐡍 (merge) {
        𐡄𐡍 (!darray_empty(expanded))
            concat_kccgst(to, darray_size(expanded), darray_items(expanded));
    } وإلا {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    أرجع true;

error:
    darray_free(expanded);
    أرجع false;
}

ساكن bool
matcher_append_pending_kccgst(بنية matcher *m)
{
    𐡄𐡍 (!m->mapping.has_multiple_layouts)
        أرجع true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    لكل (kccgst_index_t i = 0; i < m->mapping.num_kccgst; i++) {
        ثابت تعداد rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        لكل (xkb_layout_index_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            سجل ثابت بنية kccgst_buffer* ثابت buf = &m->pending_kccgst;
            size_t offset = 0;
            لكل (darray_size_t k = 0; k < darray_size(buf->slices); k++) {
                سجل ثابت بنية kccgst_buffer_slice * ثابت slice =
                    &darray_item(buf->slices, k);
                𐡄𐡍 (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->length)
                    concat_kccgst(&m->kccgst[kccgst], slice->length,
                                  darray_items(buf->buffer) + offset);
                offset += slice->length;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    أرجع true;
}

ساكن فراغ
matcher_rule_verify(بنية matcher *m, بنية scanner *s)
{
    𐡄𐡍 (m->rule.num_mlvo_values != m->mapping.num_mlvo ||
        m->rule.num_kccgst_values != m->mapping.num_kccgst) {
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
ساكن bool
matcher_rule_match_model(بنية matcher *m, ثابت بنية sval value,
                         تعداد mlvo_match_type match_type)
{
    أرجع match_value_and_mark(
        m, value, &m->rmlvo.model, match_type, WILDCARD_MATCH_ALL,
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
ساكن bool
matcher_rule_match_option(بنية matcher *m, ثابت بنية sval value,
                          تعداد mlvo_match_type match_type,
                          xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_empty(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_mask_t unmatched =
        (m->mapping.defined_mlvo_mask & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    بنية matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        ثابت xkb_layout_mask_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
        𐡄𐡍 (matchable && match_value_and_mark(m, value, option, match_type,
                                              WILDCARD_MATCH_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            𐡄𐡍 (!unmatched)
                اكسر;
        }
    }
    𐡄𐡍 (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    أرجع matched;
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
ساكن bool
matcher_rule_match_layout_or_variant(بنية matcher *m,
                                     darray_matched_sval *input,
                                     ثابت بنية sval value,
                                     تعداد mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_empty(*input));

    bool matched = false;
    /* Loop over the layout index range */
    لكل (xkb_layout_index_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ثابت xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        𐡄𐡍 (layout & *candidates) {
            بنية matched_sval *to = &darray_item(*input, idx);
            𐡄𐡍 (match_value_and_mark(m, value, to, match_type,
                                     WILDCARD_MATCH_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } وإلا {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    أرجع matched;
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
ساكن فراغ
matcher_rule_apply_if_matches(بنية matcher *m, بنية scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_mask_t candidate_layouts = m->mapping.layouts_candidates_mask;

    /* Loop over MLVO pattern components */
    لكل (mlvo_index_t i = 0; i < m->mapping.num_mlvo; i++) {
        ثابت تعداد rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ثابت بنية sval value = m->rule.mlvo_value_at_pos[i];
        ثابت تعداد mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        بدل (mlvo) {
        حالة MLVO_MODEL:
            matched = matcher_rule_match_model(m, value, match_type);
            اكسر;
        حالة MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, value, match_type, &candidate_layouts
            );
            اكسر;
        حالة MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, value, match_type, &candidate_layouts
            );
            اكسر;
        حالة MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, value, match_type, &candidate_layouts
            );
            اكسر;
        افتراضي: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == _MLVO_NUM_ENTRIES - 1,
                          "Unexpected MLVO field");
            أكد(!"Unreachable");
        }
        }

        𐡄𐡍 (!matched)
            أرجع;
    }

    𐡄𐡍 (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        لكل (xkb_layout_index_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            𐡄𐡍 (candidate_layouts & (UINT32_C(1) << idx)) {
                لكل (kccgst_index_t i = 0; i < m->mapping.num_kccgst; i++) {
                    ثابت تعداد rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ثابت بنية sval value = m->rule.kccgst_value_at_pos[i];
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
                    بنية kccgst_buffer * ثابت buf = &m->pending_kccgst;
                    ثابت darray_size_t prev_buffer_length =
                        darray_size(buf->buffer);
                    append_expanded_kccgst_value(m, s, false, &buf->buffer,
                                                 value, idx);
                    ثابت uint32_t length = (uint32_t) (darray_size(buf->buffer)
                                          - prev_buffer_length);
                    ثابت بنية kccgst_buffer_slice slice = {
                        .length = length,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } وإلا {
        /* Numeric index or no index */
        لكل (kccgst_index_t i = 0; i < m->mapping.num_kccgst; i++) {
            تعداد rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            بنية sval value = m->rule.kccgst_value_at_pos[i];
            append_expanded_kccgst_value(m, s, true, &m->kccgst[kccgst], value,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    𐡄𐡍 (!(is_mlvo_mask_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_mask &= ~candidate_layouts;
    }
}

ساكن تعداد rules_token
gettok(بنية matcher *m, بنية scanner *s)
{
    أرجع lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
ساكن bool
matcher_match(بنية matcher *m, بنية scanner *s,
              unsigned صحيح include_depth,
              ثابت حرف *string, size_t len,
              ثابت حرف *file_name)
{
    تعداد rules_token tok;

    𐡄𐡍 (!m)
        أرجع false;

initial:
    بدل (tok = gettok(m, s)) {
    حالة TOK_BANG:
        اذهب bang;
    حالة TOK_END_OF_LINE:
        اذهب initial;
    حالة TOK_END_OF_FILE:
        اذهب finish;
    افتراضي:
        اذهب unexpected;
    }

bang:
    بدل (tok = gettok(m, s)) {
    حالة TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        اذهب group_name;
    حالة TOK_INCLUDE:
        اذهب include_statement;
    حالة TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        اذهب mapping_mlvo;
    افتراضي:
        اذهب unexpected;
    }

group_name:
    بدل (tok = gettok(m, s)) {
    حالة TOK_EQUALS:
        اذهب group_element;
    افتراضي:
        اذهب unexpected;
    }

group_element:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        اذهب group_element;
    حالة TOK_END_OF_LINE:
        اذهب initial;
    افتراضي:
        اذهب unexpected;
    }

include_statement:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        اذهب include_statement_end;
    افتراضي:
        اذهب unexpected;
    }

include_statement_end:
    بدل (tok = gettok(m, s)) {
    حالة TOK_END_OF_LINE:
        اذهب initial;
    افتراضي:
        اذهب unexpected;
    }

mapping_mlvo:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        𐡄𐡍 (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        اذهب mapping_mlvo;
    حالة TOK_EQUALS:
        اذهب mapping_kccgst;
    افتراضي:
        اذهب unexpected;
    }

mapping_kccgst:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        𐡄𐡍 (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        اذهب mapping_kccgst;
    حالة TOK_END_OF_LINE:
        𐡄𐡍 (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            𐡄𐡍 (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_size(m->pending_kccgst.buffer) = 0;
                darray_size(m->pending_kccgst.slices) = 0;
            }
        }
        اذهب rule_mlvo_first;
    افتراضي:
        اذهب unexpected;
    }

rule_mlvo_first:
    بدل (tok = gettok(m, s)) {
    حالة TOK_BANG:
        matcher_append_pending_kccgst(m);
        اذهب bang;
    حالة TOK_END_OF_LINE:
        اذهب rule_mlvo_first;
    حالة TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        اذهب finish;
    افتراضي:
        matcher_rule_start_new(m);
        اذهب rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    بدل (tok) {
    حالة TOK_IDENTIFIER:
        𐡄𐡍 (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        اذهب rule_mlvo;
    حالة TOK_WILD_CARD_STAR:
        𐡄𐡍 (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        اذهب rule_mlvo;
    حالة TOK_WILD_CARD_NONE:
        𐡄𐡍 (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        اذهب rule_mlvo;
    حالة TOK_WILD_CARD_SOME:
        𐡄𐡍 (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        اذهب rule_mlvo;
    حالة TOK_WILD_CARD_ANY:
        𐡄𐡍 (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        اذهب rule_mlvo;
    حالة TOK_GROUP_NAME:
        𐡄𐡍 (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        اذهب rule_mlvo;
    حالة TOK_EQUALS:
        اذهب rule_kccgst;
    افتراضي:
        اذهب unexpected;
    }

rule_kccgst:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        𐡄𐡍 (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        اذهب rule_kccgst;
    حالة TOK_END_OF_LINE:
        𐡄𐡍 (!m->rule.skip)
            matcher_rule_verify(m, s);
        𐡄𐡍 (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        اذهب rule_mlvo_first;
    افتراضي:
        اذهب unexpected;
    }

unexpected:
    بدل (tok) {
    حالة TOK_ERROR:
        اذهب error;
    افتراضي:
        اذهب state_error;
    }

finish:
    أرجع true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    أرجع false;
}
// NOLINTEND(readability-trivial-switch)

ساكن bool
read_rules_file(بنية xkb_context *ctx,
                بنية matcher *matcher,
                unsigned صحيح include_depth,
                FILE *file,
                ثابت حرف *path)
{
    bool ret;
    حرف *string;
    size_t size;
    بنية scanner scanner;

    𐡄𐡍 (!map_file(file, &string, &size)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, strerror(errno));
        أرجع false;
    }

    scanner_init(&scanner, matcher->ctx, string, size, path, عدم);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    𐡄𐡍 (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, size);
        أرجع false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, size, path);
    unmap_file(string, size);
    أرجع ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
ساكن bool
xkb_resolve_partial_rules(بنية xkb_context *ctx, حرف *path, size_t path_size,
                          ثابت حرف* rules, ثابت حرف* suffix,
                          بنية matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    حرف partial_rules[60]; /* Arbitrary, but we do not expect long names */
    𐡄𐡍 (unlikely(!snprintf_safe(partial_rules, حجم(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        أرجع false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    unsigned صحيح offset = 0;
    FILE *file = عدم;
    ثابت size_t len = strlen(partial_rules);
    طالما ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_size, &offset, false)) != عدم) {
        ثابت bool ok = read_rules_file(ctx, matcher, 0, file, path);
        fclose(file);
        𐡄𐡍 (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            أرجع false;
        }
        offset++;
    }

    أرجع true;
}

ساكن bool
xkb_resolve_rules(بنية xkb_context *ctx,
                  ثابت حرف* rules, بنية matcher *matcher,
                  بنية xkb_component_names *out,
                  xkb_layout_index_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    unsigned صحيح offset = 0;
    حرف path[PATH_MAX];
    FILE * ثابت file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, strlen(rules), FILE_TYPE_RULES,
                                          path, حجم(path), &offset, true);
    𐡄𐡍 (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        اذهب err_out;
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
    ret = xkb_resolve_partial_rules(ctx, path, حجم(path),
                                    rules, ".pre", matcher);
    𐡄𐡍 (!ret)
        اذهب err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    𐡄𐡍 (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        اذهب err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, حجم(path),
                                    rules, ".post", matcher);
    𐡄𐡍 (!ret)
        اذهب err_out;

    /* Check we got required components */
    𐡄𐡍 (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        اذهب err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, عدم);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, عدم);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, عدم);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, عدم);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, عدم);

    بنية matched_sval *mval = &matcher->rmlvo.model;
    𐡄𐡍 (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_VALUE,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (unsigned صحيح) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        𐡄𐡍 (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_VALUE,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned صحيح) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        𐡄𐡍 (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_VALUE,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned صحيح) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        𐡄𐡍 (!mval->sval.len)
            استمر;

        𐡄𐡍 (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_VALUE,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned صحيح) mval->sval.len, mval->sval.start, rules);
        } وإلا 𐡄𐡍 (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ثابت xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            أكد(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_VALUE,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (unsigned صحيح)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    𐡄𐡍 (out->symbols != عدم && explicit_layouts != عدم) {
        *explicit_layouts = 1; /* at least one group */
        ثابت حرف *symbols = out->symbols;
        /* Take the highest modifier */
        طالما ((symbols = strchr(symbols, ':')) != عدم && symbols[1] != '\0') {
            xkb_layout_index_t group = 0;
            ثابت صحيح count = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            𐡄𐡍 (count > 0 && (symbols[count] == '\0' ||
                is_merge_mode_prefix(symbols[count])) &&
                group > 0 && group <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, group);
                symbols += count;
            }
        }
    }

err_out:
    𐡄𐡍 (file)
        fclose(file);

    أرجع ret;
}

bool
xkb_components_from_rmlvo_builder(ثابت بنية xkb_rmlvo_builder *rmlvo,
                                  بنية xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts)
{
    ثابت حرف *rules = rmlvo->rules;
    بنية matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    𐡄𐡍 (!matcher)
        أرجع false;

    ثابت bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    أرجع ret;
}


bool
xkb_components_from_rules_names(بنية xkb_context *ctx,
                                ثابت بنية xkb_rule_names *rmlvo,
                                بنية xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts)
{
    بنية matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    𐡄𐡍 (!matcher)
        أرجع false;

    ثابت bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    أرجع ret;
}
