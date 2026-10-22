/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <stdint.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "rules.h"
#அந்தர்பாவ "rmlvo.h"
#அந்தர்பாவ "include.h"
#அந்தர்பாவ "scanner-utils.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "util-numbers.h"
#அந்தர்பாவ "util-paths.h"

#define MAX_INCLUDE_DEPTH 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
ஒன்றிப்பு lvalue {
    கட்டமைப்பு sval string;
};

எண்ணிக்கை rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_வரி,
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

நிலையான எண்ணிக்கை rules_token
lex(கட்டமைப்பு scanner *s, ஒன்றிப்பு lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    போது (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    எனில் (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    எனில் (scanner_eol(s)) {
        போது (scanner_eol(s)) scanner_next(s);
        திரும்பு TOK_END_OF_வரி;
    }

    /* Escaped line continuation */
    எனில் (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        எனில் (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            திரும்பு TOK_ERROR;
        }
        scanner_next(s);
        செல் skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    எனில் (scanner_eof(s)) திரும்பு TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    எண்ணிக்கை rules_token token = TOK_IDENTIFIER;

    மாறிலி எழுத்து next = scanner_next(s);
    தேர்வு (next) {
    நிலை '!':
        திரும்பு TOK_BANG;
    நிலை '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        நிறுத்து;
    நிலை '*':
        எனில் (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            நிறுத்து;
        } இல்லையெனில் {
            /* Legacy wild card */
            திரும்பு TOK_WILD_CARD_STAR;
        }
    நிலை '<':
        /* Extended wild cards */
        எனில் (scanner_rules_lit_token(s, "none>")) திரும்பு TOK_WILD_CARD_NONE;
        எனில் (scanner_rules_lit_token(s, "some>")) திரும்பு TOK_WILD_CARD_SOME;
        எனில் (scanner_rules_lit_token(s, "any>")) திரும்பு TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        ப்ரதிஜ்ஞா(scanner_rules_is_ident('<'));
        val->string.len = 1;
        நிறுத்து;
    நிலை '=':
        எனில் (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            நிறுத்து;
        } இல்லையெனில் {
            திரும்பு TOK_EQUALS;
        }
    நிலை 'i':
        எனில் (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            திரும்பு TOK_INCLUDE;
        }
        /* Identifier */
        ப்ரதிஜ்ஞா(scanner_rules_is_ident('i'));
        val->string.len = 1;
        நிறுத்து;
    இயல்பு:
        எனில் (likely(scanner_rules_is_ident(next))) {
            /* Identifier */
            val->string.len = 1;
        } இல்லையெனில் {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.start = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    ப்ரதிஜ்ஞா(scanner_rules_is_ident(MERGE_OVERRIDE_PREFIX));
    ப்ரதிஜ்ஞா(scanner_rules_is_ident(MERGE_AUGMENT_PREFIX));
    ப்ரதிஜ்ஞா(scanner_rules_is_ident(MERGE_REPLACE_PREFIX));
    ப்ரதிஜ்ஞா(scanner_rules_is_ident('('));
    ப்ரதிஜ்ஞா(scanner_rules_is_ident(')'));
    val->string.len += scanner_rules_ident_token(s);

    எனில் (unlikely(val->string.len == 0)) {
        எனில் (token == TOK_GROUP_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } இல்லையெனில் {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unrecognized token");
        }
        திரும்பு TOK_ERROR;
    }

    திரும்பு token;
}

/***====================================================================***/

எண்ணிக்கை rules_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_இலக்கம்_ENTRIES
};

எண்ணிக்கை {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

வகைவரை uint8_t mlvo_index_t;
வகைவரை uint8_t mlvo_mask_t;


நிலையான மாறிலி கட்டமைப்பு sval rules_mlvo_svals[MLVO_இலக்கம்_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

எண்ணிக்கை rules_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_இலக்கம்_ENTRIES
};

வகைவரை uint8_t kccgst_index_t;
வகைவரை uint8_t kccgst_mask_t;

நிலையான மாறிலி கட்டமைப்பு sval rules_kccgst_svals[KCCGST_இலக்கம்_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

எண்ணிக்கை {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL ==
              (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
கட்டமைப்பு matched_sval {
    கட்டமைப்பு sval sval;
    /** Used for layout-specific options */
    xkb_layout_mask_t layouts;
    xkb_layout_mask_t matched;
};
வகைவரை darray(கட்டமைப்பு matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
கட்டமைப்பு rule_names {
    கட்டமைப்பு matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

கட்டமைப்பு group {
    கட்டமைப்பு sval name;
    darray_sval elements;
};

கட்டமைப்பு mapping {
    எண்ணிக்கை rules_mlvo mlvo_at_pos[MLVO_இலக்கம்_ENTRIES];
    mlvo_index_t இலக்கம்_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    ஒன்றிப்பு {
        கட்டமைப்பு { xkb_layout_index_t layout_idx, variant_idx; };
        கட்டமைப்பு { xkb_layout_index_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    ஒன்றிப்பு {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    எண்ணிக்கை rules_kccgst kccgst_at_pos[KCCGST_இலக்கம்_ENTRIES];
    kccgst_index_t இலக்கம்_kccgst;
    kccgst_mask_t defined_kccgst_mask;
};

எண்ணிக்கை mlvo_match_type {
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

எண்ணிக்கை wildcard_match_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_MATCH_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_MATCH_ALL,
};

கட்டமைப்பு rule {
    கட்டமைப்பு sval mlvo_மதிப்பு_at_pos[MLVO_இலக்கம்_ENTRIES];
    எண்ணிக்கை mlvo_match_type match_type_at_pos[MLVO_இலக்கம்_ENTRIES];
    கட்டமைப்பு sval kccgst_மதிப்பு_at_pos[KCCGST_இலக்கம்_ENTRIES];
    mlvo_index_t இலக்கம்_mlvo_மதிப்புகள்;
    kccgst_index_t இலக்கம்_kccgst_மதிப்புகள்;
    bool skip;
};

#define SLICE_KCCGST_BIT_FIELD_SIZE 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_SIZE - 1)) &
                (KCCGST_இலக்கம்_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
கட்டமைப்பு kccgst_buffer_slice {
    uint32_t length:(32 - SLICE_KCCGST_BIT_FIELD_SIZE);
    எண்ணிக்கை rules_kccgst kccgst:SLICE_KCCGST_BIT_FIELD_SIZE;
    xkb_layout_index_t layout;
};

/* Buffer for pending KcCGST values */
கட்டமைப்பு kccgst_buffer {
    darray_char buffer;
    /* Slice corresponding to each value in the buffer */
    darray(கட்டமைப்பு kccgst_buffer_slice) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
கட்டமைப்பு matcher {
    கட்டமைப்பு xkb_context *ctx;
    /* Input.*/
    கட்டமைப்பு rule_names rmlvo;
    ஒன்றிப்பு lvalue val;
    darray(கட்டமைப்பு group) groups;
    /* Current mapping. */
    கட்டமைப்பு mapping mapping;
    /* Current rule. */
    கட்டமைப்பு rule rule;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    கட்டமைப்பு kccgst_buffer pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_இலக்கம்_ENTRIES];
};

நிலையான கட்டமைப்பு sval
strip_spaces(கட்டமைப்பு sval v)
{
    போது (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    போது (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    திரும்பு v;
}

நிலையான darray_matched_sval
split_comma_separated_mlvo(கட்டமைப்பு xkb_context *ctx,
                           எண்ணிக்கை rules_mlvo mlvo, மாறிலி எழுத்து *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    எனில் (!s) {
        ensure_at_least_one_மதிப்பு:
        /* Label followed by a declaration is a C23 extension */ ;

        கட்டமைப்பு matched_sval val = {
            .sval = SVAL(வெற்று, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        திரும்பு arr;
    }

    போது (true) {
        கட்டமைப்பு matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        போது (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        எனில் (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            மாறிலி எழுத்து* மாறிலி layout_start = s;
            xkb_layout_index_t layout = XKB_LAYOUT_INVALID;

            முழு மொத்தம் = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            எனில் (மொத்தம் > 0) {
                /* Note: 1-indexed layout */
                s += மொத்தம்;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                எனில் (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (unsigned முழு) val.sval.len, val.sval.start);
                } இல்லையெனில் எனில் (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (unsigned முழு) val.sval.len, val.sval.start);
                } இல்லையெனில் {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            மாறிலி எழுத்து* மாறிலி layout_index_end = s;
            போது (*s != '\0' && *s != ',') { s++; }
            எனில் (மொத்தம் <= 0 || layout_index_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (unsigned முழு) (s - layout_start), layout_start,
                        (unsigned முழு) val.sval.len, val.sval.start);
            }
        }

        எனில் (val.sval.len || mlvo != MLVO_OPTION) {
            எனில் (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                கட்டமைப்பு matched_sval *prev;
                darray_foreach(prev, arr) {
                    எனில் (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        எனில் (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } இல்லையெனில் எனில் (prev->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        செல் next_மதிப்பு;
                    }
                }
            }
            darray_append(arr, val);
        }

next_மதிப்பு:
        எனில் (*s == '\0') நிறுத்து;
        எனில் (*s == ',') s++;
    }

    எனில் (darray_empty(arr))
        செல் ensure_at_least_one_மதிப்பு;

    திரும்பு arr;
}

நிலையான கட்டமைப்பு matcher *
matcher_new_from_rmlvo(மாறிலி கட்டமைப்பு xkb_rmlvo_builder *rmlvo, மாறிலி எழுத்து **rules)
{
    கட்டமைப்பு matcher *m = calloc(1, அளவு(*m));
    எனில் (!m)
        திரும்பு வெற்று;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    கட்டமைப்பு xkb_rule_names names = {
        .rules = rmlvo->rules,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_empty(rmlvo->layouts)) ? வெற்று : "x",
        .variant = (darray_empty(rmlvo->layouts)) ? வெற்று : "x",
        .options = (darray_empty(rmlvo->options)) ? வெற்று : "x",
    };
    மாறிலி எண்ணிக்கை RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
                                                               &names);

    /* Initialize matcher with sanitized names, if relevant */
    எனில் (changed & RMLVO_RULES) {
        *rules = names.rules;
    } இல்லையெனில் {
        *rules = rmlvo->rules;
    }

    எனில் (changed & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = names.model;
    } இல்லையெனில் {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;

    ப்ரதிஜ்ஞா((changed & RMLVO_LAYOUT) || !(changed & RMLVO_VARIANT));
    எனில் (changed & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = split_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      names.layout);
        m->rmlvo.variants = split_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       names.variant);
        எனில் (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            எனில் (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        } இல்லையெனில் எனில் (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } இல்லையெனில் {
        கட்டமைப்பு xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            கட்டமைப்பு matched_sval val = {
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

    எனில் (changed & RMLVO_OPTIONS) {
        m->rmlvo.options = split_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      names.options);
    } இல்லையெனில் {
        கட்டமைப்பு xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            கட்டமைப்பு matched_sval val = {
                .sval = SVAL(option->option, strlen_safe(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    திரும்பு m;
}

நிலையான கட்டமைப்பு matcher *
matcher_new_from_names(கட்டமைப்பு xkb_context *ctx,
            மாறிலி கட்டமைப்பு xkb_rule_names *rmlvo)
{
    கட்டமைப்பு matcher *m = calloc(1, அளவு(*m));
    எனில் (!m)
        திரும்பு வெற்று;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    எனில் (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        எனில் (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
    } இல்லையெனில் எனில் (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    திரும்பு m;
}

நிலையான வெறுமை
matcher_free(கட்டமைப்பு matcher *m)
{
    எனில் (!m)
        திரும்பு;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    கட்டமைப்பு group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.buffer);
    darray_free(m->pending_kccgst.slices);
    க்கு (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_இலக்கம்_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    விடுவி(m);
}

நிலையான வெறுமை
matcher_group_start_new(கட்டமைப்பு matcher *m, கட்டமைப்பு sval name)
{
    கட்டமைப்பு group group = { .name = name, .elements = darray_new() };
    darray_append(m->groups, group);
}

நிலையான வெறுமை
matcher_group_add_element(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
                          கட்டமைப்பு sval element)
{
    darray_append(darray_item(m->groups, darray_size(m->groups) - 1).elements,
                  element);
}

நிலையான bool
read_rules_file(கட்டமைப்பு xkb_context *ctx,
                கட்டமைப்பு matcher *matcher,
                unsigned முழு include_depth,
                FILE *file,
                மாறிலி எழுத்து *path);

நிலையான வெறுமை
matcher_include(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *parent_scanner,
                unsigned முழு include_depth,
                கட்டமைப்பு sval inc)
{
    எனில் (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        திரும்பு;
    }

    மாறிலி எழுத்து *stmt_file = inc.start;
    size_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    எழுத்து buf[PATH_MAX] = {0};
    மாறிலி ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, அளவு(buf));
    எனில் (expanded < 0) {
        /* Error */
        திரும்பு;
    } இல்லையெனில் எனில் (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (size_t) expanded;
        ப்ரதிஜ்ஞா(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned முழு offset = 0;
    மாறிலி bool absolute_path = is_absolute_path(stmt_file);
    எனில் (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        எனில் (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            எனில் (stmt_file_len < அளவு(buf)) {
                memcpy(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } இல்லையெனில் {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, அளவு(buf),
                        (unsigned முழு) stmt_file_len, stmt_file);
                திரும்பு;
            }
        } இல்லையெனில் {
            /* %-expansion is always NULL-terminated */
            ப்ரதிஜ்ஞா(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } இல்லையெனில் {
        /* Relative path: lookup the first XKB path */
        எனில் (unlikely(expanded)) {
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
            file = வெற்று;
        } இல்லையெனில் {
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, அளவு(buf), &offset, true);
        }
    }

    போது (file) {
        ப்ரதிஜ்ஞா(strlen_safe(buf) < அளவு(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        fclose(file);
        எனில் (ret)
            திரும்பு;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        எனில் (absolute_path) {
            /* There is no point to search further if the path is absolute */
            நிறுத்து;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, அளவு(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (unsigned முழு) stmt_file_len, stmt_file);
}

நிலையான வெறுமை
matcher_mapping_start_new(கட்டமைப்பு matcher *m)
{
    க்கு (mlvo_index_t i = 0; i < (mlvo_index_t) MLVO_இலக்கம்_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_இலக்கம்_ENTRIES;
    க்கு (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_இலக்கம்_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_இலக்கம்_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.இலக்கம்_mlvo = m->mapping.இலக்கம்_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
நிலையான முழு
parse_layout_int_index_tail(மாறிலி எழுத்து *s, size_t max_len,
                            xkb_layout_index_t *out)
{
    எனில் (max_len < 3)
        திரும்பு -1;
    uint32_t val = 0;
    மாறிலி முழு மொத்தம் = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    எனில் (மொத்தம் <= 0 || s[1 + மொத்தம்] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        திரும்பு -1;
    /* To zero-based index. */
    *out = val - 1;
    திரும்பு மொத்தம் + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
நிலையான முழு
extract_layout_index(மாறிலி எழுத்து *s, size_t max_len, xkb_layout_index_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    எனில் (max_len < 3 || s[0] != '[')
        திரும்பு -1;
    எனில் (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_index_t)(s[1] - '1');
        திரும்பு 3;
    }
    எனில் (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        திரும்பு 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    திரும்பு parse_layout_int_index_tail(s, max_len, out);
}

/* Special layout indices */
எண்ணிக்கை layout_index_ranges {
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
நிலையான bool
extract_mapping_layout_index(மாறிலி எழுத்து *s, size_t max_len,
                             xkb_layout_index_t *out)
{
    /* Check for minimal `[` + index + `]` */
    எனில் (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        திரும்பு false;
    }

    /* Small numeric index */
    எனில் (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_index_t)(s[1] - '1');
        திரும்பு true;
    }

    /* Named indices ranges */
    நிலையான மாறிலி கட்டமைப்பு {
        மாறிலி எழுத்து* name;
        size_t size;
        xkb_layout_index_t மதிப்பு;
    } names[] = {
        #define INDEX_RANGE(tail, மதிப்பு) \
            { (tail), அளவு(tail), (xkb_layout_index_t)(மதிப்பு) }
        INDEX_RANGE("single]",   LAYOUT_INDEX_SINGLE),
        INDEX_RANGE("first]",    LAYOUT_INDEX_FIRST),
        INDEX_RANGE("later]",    LAYOUT_INDEX_LATER),
        INDEX_RANGE("multiple]", LAYOUT_INDEX_MULTIPLE),
        INDEX_RANGE("any]",      LAYOUT_INDEX_ANY),
        #undef INDEX_RANGE
    };

    க்கு (size_t k = 0; k < ARRAY_SIZE(names); k++) {
        எனில் (max_len == names[k].size &&
            memcmp(&s[1], names[k].name, names[k].size - 1) == 0) {
            *out = names[k].மதிப்பு;
            திரும்பு true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    மாறிலி முழு மொத்தம் = parse_layout_int_index_tail(s, max_len, out);
    திரும்பு மொத்தம் > 0 && (size_t)மொத்தம் == max_len;
}

நிலையான உள்வரி bool
is_mlvo_mask_defined(கட்டமைப்பு matcher *m, எண்ணிக்கை rules_mlvo mlvo)
{
    திரும்பு m->mapping.defined_mlvo_mask & (1u << mlvo);
}

நிலையான வெறுமை
matcher_mapping_set_mlvo(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
                         கட்டமைப்பு sval ident)
{
    எண்ணிக்கை rules_mlvo mlvo;
    கட்டமைப்பு sval mlvo_sval;

    க்கு (mlvo = 0; mlvo < MLVO_இலக்கம்_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        எனில் (svaleq_prefix(mlvo_sval, ident))
            நிறுத்து;
    }

    /* Not found. */
    எனில் (mlvo >= MLVO_இலக்கம்_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned முழு) ident.len, ident.start);
        m->mapping.active = false;
        திரும்பு;
    }

    எனில் (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned முழு) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        திரும்பு;
    }

    /* If there are leftovers still, it must be an index. */
    எனில் (mlvo_sval.len < ident.len) {
        xkb_layout_index_t idx;
        எனில் (!extract_mapping_layout_index(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (unsigned முழு) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            திரும்பு;
        }

        எனில் (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        இல்லையெனில் எனில் (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        இல்லையெனில் {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (unsigned முழு) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            திரும்பு;
        }
    } இல்லையெனில் எனில் (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    } இல்லையெனில் எனில் (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    எனில் (((mlvo == MLVO_LAYOUT && is_mlvo_mask_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_mask_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.active = false;
        திரும்பு;
    }

    m->mapping.mlvo_at_pos[m->mapping.இலக்கம்_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.இலக்கம்_mlvo++;
}

நிலையான வெறுமை
matcher_mapping_set_layout_bounds(கட்டமைப்பு matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_index_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    தேர்வு (idx) {
        நிலை XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            ப்ரதிஜ்ஞா(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            நிறுத்து;
        நிலை LAYOUT_INDEX_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            நிறுத்து;
        நிலை LAYOUT_INDEX_MULTIPLE:
        நிலை LAYOUT_INDEX_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            நிறுத்து;
        நிலை LAYOUT_INDEX_SINGLE:
        நிலை LAYOUT_INDEX_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        இயல்பு:
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

நிலையான வெறுமை
matcher_mapping_set_kccgst(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s, கட்டமைப்பு sval ident)
{
    எண்ணிக்கை rules_kccgst kccgst;
    கட்டமைப்பு sval kccgst_sval;

    க்கு (kccgst = 0; kccgst < KCCGST_இலக்கம்_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        எனில் (svaleq(rules_kccgst_svals[kccgst], ident))
            நிறுத்து;
    }

    /* Not found. */
    எனில் (kccgst >= KCCGST_இலக்கம்_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned முழு) ident.len, ident.start);
        m->mapping.active = false;
        திரும்பு;
    }

    எனில் (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned முழு) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        திரும்பு;
    }

    m->mapping.kccgst_at_pos[m->mapping.இலக்கம்_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.இலக்கம்_kccgst++;
}

நிலையான bool
matcher_mapping_verify(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s)
{
    எனில் (m->mapping.இலக்கம்_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        செல் skip;
    }

    எனில் (m->mapping.இலக்கம்_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        செல் skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    எனில் (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        ப்ரதிஜ்ஞா(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        தேர்வு (m->mapping.layout_idx) {
            நிலை LAYOUT_INDEX_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                எனில் (darray_size(m->rmlvo.layouts) > 1)
                    செல் skip;
                நிறுத்து;
            நிலை LAYOUT_INDEX_MULTIPLE:
            நிலை LAYOUT_INDEX_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                எனில் (darray_size(m->rmlvo.layouts) < 2)
                    செல் skip;
                நிறுத்து;
            நிலை LAYOUT_INDEX_ANY:
            நிலை LAYOUT_INDEX_FIRST:
                /* No restrictions */
                நிறுத்து;
            இயல்பு:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                எனில் (darray_size(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_size(m->rmlvo.layouts))
                    செல் skip;
        }
    }

    எனில் (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        ப்ரதிஜ்ஞா(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        தேர்வு (m->mapping.variant_idx) {
            நிலை LAYOUT_INDEX_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                எனில் (darray_size(m->rmlvo.variants) > 1)
                    செல் skip;
                நிறுத்து;
            நிலை LAYOUT_INDEX_ANY:
            நிலை LAYOUT_INDEX_LATER:
            நிலை LAYOUT_INDEX_FIRST:
                /* No restriction */
                நிறுத்து;
            இயல்பு:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                எனில் (darray_size(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_size(m->rmlvo.variants))
                    செல் skip;
        }
    }

    திரும்பு true;

skip:
    m->mapping.active = false;
    திரும்பு false;
}

நிலையான வெறுமை
matcher_rule_start_new(கட்டமைப்பு matcher *m)
{
    memset(&m->rule, 0, அளவு(m->rule));
    m->rule.skip = !m->mapping.active;
}

நிலையான வெறுமை
matcher_rule_set_mlvo_common(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
                             கட்டமைப்பு sval ident,
                             எண்ணிக்கை mlvo_match_type match_type)
{
    எனில் (m->rule.இலக்கம்_mlvo_மதிப்புகள் >= m->mapping.இலக்கம்_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        திரும்பு;
    }
    m->rule.match_type_at_pos[m->rule.இலக்கம்_mlvo_மதிப்புகள்] = match_type;
    m->rule.mlvo_மதிப்பு_at_pos[m->rule.இலக்கம்_mlvo_மதிப்புகள்] = ident;
    m->rule.இலக்கம்_mlvo_மதிப்புகள்++;
}

நிலையான வெறுமை
matcher_rule_set_mlvo_wildcard(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
                               எண்ணிக்கை mlvo_match_type match_type)
{
    கட்டமைப்பு sval dummy = SVAL(வெற்று, 0);
    matcher_rule_set_mlvo_common(m, s, dummy, match_type);
}

நிலையான வெறுமை
matcher_rule_set_mlvo_group(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
                            கட்டமைப்பு sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_GROUP);
}

நிலையான வெறுமை
matcher_rule_set_mlvo(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
                      கட்டமைப்பு sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_NORMAL);
}

நிலையான வெறுமை
matcher_rule_set_kccgst(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
                        கட்டமைப்பு sval ident)
{
    எனில் (m->rule.இலக்கம்_kccgst_மதிப்புகள் >= m->mapping.இலக்கம்_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        திரும்பு;
    }
    m->rule.kccgst_மதிப்பு_at_pos[m->rule.இலக்கம்_kccgst_மதிப்புகள்] = ident;
    m->rule.இலக்கம்_kccgst_மதிப்புகள்++;
}

நிலையான bool
match_group(கட்டமைப்பு matcher *m, கட்டமைப்பு sval group_name, கட்டமைப்பு sval to)
{
    கட்டமைப்பு group *group = வெற்று;
    கட்டமைப்பு sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        எனில் (svaleq(group->name, group_name)) {
            found = true;
            நிறுத்து;
        }
    }

    எனில் (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        திரும்பு false;
    }

    darray_foreach(element, group->elements)
        எனில் (svaleq(to, *element))
            திரும்பு true;

    திரும்பு false;
}

நிலையான bool
match_மதிப்பு(கட்டமைப்பு matcher *m, மாறிலி கட்டமைப்பு sval val, மாறிலி கட்டமைப்பு sval to,
            எண்ணிக்கை mlvo_match_type match_type,
            எண்ணிக்கை wildcard_match_type wildcard_type)
{
    தேர்வு (match_type) {
        நிலை MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            திரும்பு wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        நிலை MLVO_MATCH_WILDCARD_NONE:
            திரும்பு !to.len;
        நிலை MLVO_MATCH_WILDCARD_SOME:
            திரும்பு !!to.len;
        நிலை MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            திரும்பு true;
        நிலை MLVO_MATCH_GROUP:
            திரும்பு match_group(m, val, to);
        இயல்பு:
            ப்ரதிஜ்ஞா(match_type == MLVO_MATCH_NORMAL);
            திரும்பு svaleq(val, to);
    }
}

நிலையான bool
match_மதிப்பு_and_mark(கட்டமைப்பு matcher *m, மாறிலி கட்டமைப்பு sval val,
                     கட்டமைப்பு matched_sval *to, எண்ணிக்கை mlvo_match_type match_type,
                     எண்ணிக்கை wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_மதிப்பு(m, val, to->sval, match_type, wildcard_type);
    எனில் (matched)
        to->matched |= layouts;
    திரும்பு matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
நிலையான bool
expand_rmlvo_in_kccgst_மதிப்பு(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
                             கட்டமைப்பு sval மதிப்பு, xkb_layout_index_t layout_idx,
                             darray_char *expanded, size_t *i)
{
    மாறிலி எழுத்து *str = மதிப்பு.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    எண்ணிக்கை rules_mlvo mlv;
    xkb_layout_index_t idx;
    எழுத்து pfx, sfx;
    கட்டமைப்பு matched_sval *expanded_மதிப்பு;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    எனில் (str[*i] == 'i' &&
        (*i + 1 == மதிப்பு.len || is_merge_mode_prefix(str[*i + 1])))
    {
        எனில் (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_INDEX_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            செல் error;
        }
        (*i)++;
        எழுத்து index_str[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
        மாறிலி முழு மொத்தம் = snprintf(index_str, அளவு(index_str), "%"PRIu32,
                                   layout_idx + 1);
        ப்ரதிஜ்ஞா(மொத்தம் > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, index_str, (darray_size_t)மொத்தம்);
        திரும்பு true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    எனில் (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        எனில் (str[*i] == '(') sfx = ')';
        எனில் (++(*i) >= மதிப்பு.len) செல் error;
    }

    /* Mandatory model/layout/variant specifier. */
    தேர்வு (str[(*i)++]) {
    நிலை 'm': mlv = MLVO_MODEL; நிறுத்து;
    நிலை 'l': mlv = MLVO_LAYOUT; நிறுத்து;
    நிலை 'v': mlv = MLVO_VARIANT; நிறுத்து;
    இயல்பு: செல் error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_index = false;
    எனில் (*i < மதிப்பு.len && str[*i] == '[') {
        எனில் (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            செல் error;
        }

        முழு consumed = extract_layout_index(str + (*i), மதிப்பு.len - (*i), &idx);
        எனில் (consumed <= 0) செல் error;
        எனில் (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            ப்ரதிஜ்ஞா(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_index = true;
        }
        *i += (size_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    எனில் (sfx != 0) {
        எனில் (*i >= மதிப்பு.len) செல் error;
        எனில் (str[(*i)++] != sfx) செல் error;
    }

    /* Get the expanded value. */
    expanded_மதிப்பு = வெற்று;

    எனில் (mlv == MLVO_LAYOUT) {
        எனில் (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            எனில் (darray_size(m->rmlvo.layouts) == 1)
                expanded_மதிப்பு = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } இல்லையெனில் எனில் (idx < darray_size(m->rmlvo.layouts) &&
                   (variable_index || darray_size(m->rmlvo.layouts) > 1)) {
                expanded_மதிப்பு = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    இல்லையெனில் எனில் (mlv == MLVO_VARIANT) {
        எனில் (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            எனில் (darray_size(m->rmlvo.variants) == 1)
                expanded_மதிப்பு = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } இல்லையெனில் எனில் (idx < darray_size(m->rmlvo.variants) &&
                   (variable_index || darray_size(m->rmlvo.variants) > 1)) {
                expanded_மதிப்பு = &darray_item(m->rmlvo.variants, idx);
        }
    }
    இல்லையெனில் எனில் (mlv == MLVO_MODEL) {
        expanded_மதிப்பு = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    எனில் (!expanded_மதிப்பு || expanded_மதிப்பு->sval.len == 0) {
        திரும்பு true;
    }

    எனில் (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_மதிப்பு->sval.start,
                                 (darray_size_t) expanded_மதிப்பு->sval.len);
    எனில் (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    எனில் (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        மாறிலி xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_மதிப்பு->matched |= layouts;
    } இல்லையெனில் {
        expanded_மதிப்பு->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    திரும்பு true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    திரும்பு false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
நிலையான வெறுமை
expand_qualifier_in_kccgst_மதிப்பு(
    கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
    கட்டமைப்பு sval மதிப்பு, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_size_t prefix_idx, size_t *i)
{
    மாறிலி எழுத்து *str = மதிப்பு.start;

    /* “all” followed by nothing or by a layout separator */
    எனில் (*i + 2 < மதிப்பு.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == மதிப்பு.len || is_merge_mode_prefix(str[*i + 3]))) {
        எனில் (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_MESSAGE_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        எனில் (darray_size(m->rmlvo.layouts) > 1) {
            எழுத்து layout_index[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
            மாறிலி darray_size_t prefix_length =
                darray_size(*expanded) - prefix_idx - 1;
            க்கு (xkb_layout_index_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_size(m->rmlvo.layouts));
                 l++)
            {
                எனில் (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_length);
                /* Append index */
                மாறிலி முழு மொத்தம் = snprintf(layout_index, அளவு(layout_index),
                                           "%"PRIu32, l + 1);
                ப்ரதிஜ்ஞா(மொத்தம் > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_index,
                                             (darray_size_t)மொத்தம்);
            }
        }
        *i += 3;
    }
}

நிலையான உள்வரி வெறுமை
#ifdef _MSC_VER
concat_kccgst(darray_char *into, darray_size_t size, _In_reads_(size) மாறிலி எழுத்து* from)
#இல்லையெனில்
concat_kccgst(darray_char *into, darray_size_t size, மாறிலி எழுத்து from[நிலையான size])
#endif
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    மாறிலி bool from_plus = is_merge_mode_prefix(from[0]);
    எனில் (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, size);
    } இல்லையெனில் {
        மாறிலி எழுத்து ch =
            (எழுத்து) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        மாறிலி bool into_plus = is_merge_mode_prefix(ch);
        எனில் (into_plus)
            darray_prepends_nullterminate(*into, from, size);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
நிலையான bool
append_expanded_kccgst_மதிப்பு(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
                             bool merge, darray_char *to, கட்டமைப்பு sval மதிப்பு,
                             xkb_layout_index_t layout_idx)
{
    மாறிலி எழுத்து *str = மதிப்பு.start;
    darray_char expanded = darray_new();
    darray_size_t last_item_idx = 0;
    bool has_separator = false;

    க்கு (size_t i = 0; i < மதிப்பு.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        தேர்வு (str[i]) {
            /* Qualifier */
            நிலை ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_மதிப்பு(m, s, மதிப்பு, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                நிறுத்து;
            /* Expansion */
            நிலை '%':
                i++;
                எனில் (i >= மதிப்பு.len ||
                    !expand_rmlvo_in_kccgst_மதிப்பு(m, s, மதிப்பு, layout_idx,
                                                  &expanded, &i))
                        செல் error;
                நிறுத்து;
            /* New item */
            நிலை MERGE_OVERRIDE_PREFIX:
            நிலை MERGE_AUGMENT_PREFIX:
            நிலை MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_size(expanded) - 1;
                has_separator = true;
                நிறுத்து;
            /* Just a normal character. */
            இயல்பு:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    எனில் (merge) {
        எனில் (!darray_empty(expanded))
            concat_kccgst(to, darray_size(expanded), darray_பொருட்கள்(expanded));
    } இல்லையெனில் {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    திரும்பு true;

error:
    darray_free(expanded);
    திரும்பு false;
}

நிலையான bool
matcher_append_pending_kccgst(கட்டமைப்பு matcher *m)
{
    எனில் (!m->mapping.has_multiple_layouts)
        திரும்பு true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    க்கு (kccgst_index_t i = 0; i < m->mapping.இலக்கம்_kccgst; i++) {
        மாறிலி எண்ணிக்கை rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        க்கு (xkb_layout_index_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            பதிவு மாறிலி கட்டமைப்பு kccgst_buffer* மாறிலி buf = &m->pending_kccgst;
            size_t offset = 0;
            க்கு (darray_size_t k = 0; k < darray_size(buf->slices); k++) {
                பதிவு மாறிலி கட்டமைப்பு kccgst_buffer_slice * மாறிலி slice =
                    &darray_item(buf->slices, k);
                எனில் (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->length)
                    concat_kccgst(&m->kccgst[kccgst], slice->length,
                                  darray_பொருட்கள்(buf->buffer) + offset);
                offset += slice->length;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    திரும்பு true;
}

நிலையான வெறுமை
matcher_rule_verify(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s)
{
    எனில் (m->rule.இலக்கம்_mlvo_மதிப்புகள் != m->mapping.இலக்கம்_mlvo ||
        m->rule.இலக்கம்_kccgst_மதிப்புகள் != m->mapping.இலக்கம்_kccgst) {
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
நிலையான bool
matcher_rule_match_model(கட்டமைப்பு matcher *m, மாறிலி கட்டமைப்பு sval மதிப்பு,
                         எண்ணிக்கை mlvo_match_type match_type)
{
    திரும்பு match_மதிப்பு_and_mark(
        m, மதிப்பு, &m->rmlvo.model, match_type, WILDCARD_MATCH_ALL,
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
நிலையான bool
matcher_rule_match_option(கட்டமைப்பு matcher *m, மாறிலி கட்டமைப்பு sval மதிப்பு,
                          எண்ணிக்கை mlvo_match_type match_type,
                          xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    ப்ரதிஜ்ஞா(!darray_empty(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_mask_t unmatched =
        (m->mapping.defined_mlvo_mask & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    கட்டமைப்பு matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        மாறிலி xkb_layout_mask_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
        எனில் (matchable && match_மதிப்பு_and_mark(m, மதிப்பு, option, match_type,
                                              WILDCARD_MATCH_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            எனில் (!unmatched)
                நிறுத்து;
        }
    }
    எனில் (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    திரும்பு matched;
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
நிலையான bool
matcher_rule_match_layout_or_variant(கட்டமைப்பு matcher *m,
                                     darray_matched_sval *உள்ளீடு,
                                     மாறிலி கட்டமைப்பு sval மதிப்பு,
                                     எண்ணிக்கை mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    ப்ரதிஜ்ஞா(!darray_empty(*உள்ளீடு));

    bool matched = false;
    /* Loop over the layout index range */
    க்கு (xkb_layout_index_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        மாறிலி xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        எனில் (layout & *candidates) {
            கட்டமைப்பு matched_sval *to = &darray_item(*உள்ளீடு, idx);
            எனில் (match_மதிப்பு_and_mark(m, மதிப்பு, to, match_type,
                                     WILDCARD_MATCH_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } இல்லையெனில் {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    திரும்பு matched;
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
நிலையான வெறுமை
matcher_rule_apply_if_matches(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_mask_t candidate_layouts = m->mapping.layouts_candidates_mask;

    /* Loop over MLVO pattern components */
    க்கு (mlvo_index_t i = 0; i < m->mapping.இலக்கம்_mlvo; i++) {
        மாறிலி எண்ணிக்கை rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        மாறிலி கட்டமைப்பு sval மதிப்பு = m->rule.mlvo_மதிப்பு_at_pos[i];
        மாறிலி எண்ணிக்கை mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        தேர்வு (mlvo) {
        நிலை MLVO_MODEL:
            matched = matcher_rule_match_model(m, மதிப்பு, match_type);
            நிறுத்து;
        நிலை MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, மதிப்பு, match_type, &candidate_layouts
            );
            நிறுத்து;
        நிலை MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, மதிப்பு, match_type, &candidate_layouts
            );
            நிறுத்து;
        நிலை MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, மதிப்பு, match_type, &candidate_layouts
            );
            நிறுத்து;
        இயல்பு: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_இலக்கம்_ENTRIES - 1,
                          "Unexpected MLVO field");
            ப்ரதிஜ்ஞா(!"Unreachable");
        }
        }

        எனில் (!matched)
            திரும்பு;
    }

    எனில் (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        க்கு (xkb_layout_index_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            எனில் (candidate_layouts & (UINT32_C(1) << idx)) {
                க்கு (kccgst_index_t i = 0; i < m->mapping.இலக்கம்_kccgst; i++) {
                    மாறிலி எண்ணிக்கை rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    மாறிலி கட்டமைப்பு sval மதிப்பு = m->rule.kccgst_மதிப்பு_at_pos[i];
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
                    கட்டமைப்பு kccgst_buffer * மாறிலி buf = &m->pending_kccgst;
                    மாறிலி darray_size_t prev_buffer_length =
                        darray_size(buf->buffer);
                    append_expanded_kccgst_மதிப்பு(m, s, false, &buf->buffer,
                                                 மதிப்பு, idx);
                    மாறிலி uint32_t length = (uint32_t) (darray_size(buf->buffer)
                                          - prev_buffer_length);
                    மாறிலி கட்டமைப்பு kccgst_buffer_slice slice = {
                        .length = length,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } இல்லையெனில் {
        /* Numeric index or no index */
        க்கு (kccgst_index_t i = 0; i < m->mapping.இலக்கம்_kccgst; i++) {
            எண்ணிக்கை rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            கட்டமைப்பு sval மதிப்பு = m->rule.kccgst_மதிப்பு_at_pos[i];
            append_expanded_kccgst_மதிப்பு(m, s, true, &m->kccgst[kccgst], மதிப்பு,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    எனில் (!(is_mlvo_mask_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_mask &= ~candidate_layouts;
    }
}

நிலையான எண்ணிக்கை rules_token
gettok(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s)
{
    திரும்பு lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
நிலையான bool
matcher_match(கட்டமைப்பு matcher *m, கட்டமைப்பு scanner *s,
              unsigned முழு include_depth,
              மாறிலி எழுத்து *string, size_t len,
              மாறிலி எழுத்து *file_name)
{
    எண்ணிக்கை rules_token tok;

    எனில் (!m)
        திரும்பு false;

initial:
    தேர்வு (tok = gettok(m, s)) {
    நிலை TOK_BANG:
        செல் bang;
    நிலை TOK_END_OF_வரி:
        செல் initial;
    நிலை TOK_END_OF_FILE:
        செல் finish;
    இயல்பு:
        செல் unexpected;
    }

bang:
    தேர்வு (tok = gettok(m, s)) {
    நிலை TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        செல் group_name;
    நிலை TOK_INCLUDE:
        செல் include_statement;
    நிலை TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        செல் mapping_mlvo;
    இயல்பு:
        செல் unexpected;
    }

group_name:
    தேர்வு (tok = gettok(m, s)) {
    நிலை TOK_EQUALS:
        செல் group_element;
    இயல்பு:
        செல் unexpected;
    }

group_element:
    தேர்வு (tok = gettok(m, s)) {
    நிலை TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        செல் group_element;
    நிலை TOK_END_OF_வரி:
        செல் initial;
    இயல்பு:
        செல் unexpected;
    }

include_statement:
    தேர்வு (tok = gettok(m, s)) {
    நிலை TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        செல் include_statement_end;
    இயல்பு:
        செல் unexpected;
    }

include_statement_end:
    தேர்வு (tok = gettok(m, s)) {
    நிலை TOK_END_OF_வரி:
        செல் initial;
    இயல்பு:
        செல் unexpected;
    }

mapping_mlvo:
    தேர்வு (tok = gettok(m, s)) {
    நிலை TOK_IDENTIFIER:
        எனில் (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        செல் mapping_mlvo;
    நிலை TOK_EQUALS:
        செல் mapping_kccgst;
    இயல்பு:
        செல் unexpected;
    }

mapping_kccgst:
    தேர்வு (tok = gettok(m, s)) {
    நிலை TOK_IDENTIFIER:
        எனில் (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        செல் mapping_kccgst;
    நிலை TOK_END_OF_வரி:
        எனில் (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            எனில் (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_size(m->pending_kccgst.buffer) = 0;
                darray_size(m->pending_kccgst.slices) = 0;
            }
        }
        செல் rule_mlvo_first;
    இயல்பு:
        செல் unexpected;
    }

rule_mlvo_first:
    தேர்வு (tok = gettok(m, s)) {
    நிலை TOK_BANG:
        matcher_append_pending_kccgst(m);
        செல் bang;
    நிலை TOK_END_OF_வரி:
        செல் rule_mlvo_first;
    நிலை TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        செல் finish;
    இயல்பு:
        matcher_rule_start_new(m);
        செல் rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    தேர்வு (tok) {
    நிலை TOK_IDENTIFIER:
        எனில் (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        செல் rule_mlvo;
    நிலை TOK_WILD_CARD_STAR:
        எனில் (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        செல் rule_mlvo;
    நிலை TOK_WILD_CARD_NONE:
        எனில் (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        செல் rule_mlvo;
    நிலை TOK_WILD_CARD_SOME:
        எனில் (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        செல் rule_mlvo;
    நிலை TOK_WILD_CARD_ANY:
        எனில் (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        செல் rule_mlvo;
    நிலை TOK_GROUP_NAME:
        எனில் (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        செல் rule_mlvo;
    நிலை TOK_EQUALS:
        செல் rule_kccgst;
    இயல்பு:
        செல் unexpected;
    }

rule_kccgst:
    தேர்வு (tok = gettok(m, s)) {
    நிலை TOK_IDENTIFIER:
        எனில் (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        செல் rule_kccgst;
    நிலை TOK_END_OF_வரி:
        எனில் (!m->rule.skip)
            matcher_rule_verify(m, s);
        எனில் (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        செல் rule_mlvo_first;
    இயல்பு:
        செல் unexpected;
    }

unexpected:
    தேர்வு (tok) {
    நிலை TOK_ERROR:
        செல் error;
    இயல்பு:
        செல் state_error;
    }

finish:
    திரும்பு true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    திரும்பு false;
}
// NOLINTEND(readability-trivial-switch)

நிலையான bool
read_rules_file(கட்டமைப்பு xkb_context *ctx,
                கட்டமைப்பு matcher *matcher,
                unsigned முழு include_depth,
                FILE *file,
                மாறிலி எழுத்து *path)
{
    bool ret;
    எழுத்து *string;
    size_t size;
    கட்டமைப்பு scanner scanner;

    எனில் (!map_file(file, &string, &size)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, strerror(errno));
        திரும்பு false;
    }

    scanner_init(&scanner, matcher->ctx, string, size, path, வெற்று);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    எனில் (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, size);
        திரும்பு false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, size, path);
    unmap_file(string, size);
    திரும்பு ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
நிலையான bool
xkb_resolve_partial_rules(கட்டமைப்பு xkb_context *ctx, எழுத்து *path, size_t path_size,
                          மாறிலி எழுத்து* rules, மாறிலி எழுத்து* suffix,
                          கட்டமைப்பு matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    எழுத்து partial_rules[60]; /* Arbitrary, but we do not expect long names */
    எனில் (unlikely(!snprintf_safe(partial_rules, அளவு(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        திரும்பு false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    unsigned முழு offset = 0;
    FILE *file = வெற்று;
    மாறிலி size_t len = strlen(partial_rules);
    போது ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_size, &offset, false)) != வெற்று) {
        மாறிலி bool ok = read_rules_file(ctx, matcher, 0, file, path);
        fclose(file);
        எனில் (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            திரும்பு false;
        }
        offset++;
    }

    திரும்பு true;
}

நிலையான bool
xkb_resolve_rules(கட்டமைப்பு xkb_context *ctx,
                  மாறிலி எழுத்து* rules, கட்டமைப்பு matcher *matcher,
                  கட்டமைப்பு xkb_component_names *out,
                  xkb_layout_index_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    unsigned முழு offset = 0;
    எழுத்து path[PATH_MAX];
    FILE * மாறிலி file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, strlen(rules), FILE_TYPE_RULES,
                                          path, அளவு(path), &offset, true);
    எனில் (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        செல் err_out;
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
    ret = xkb_resolve_partial_rules(ctx, path, அளவு(path),
                                    rules, ".pre", matcher);
    எனில் (!ret)
        செல் err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    எனில் (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        செல் err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, அளவு(path),
                                    rules, ".post", matcher);
    எனில் (!ret)
        செல் err_out;

    /* Check we got required components */
    எனில் (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        செல் err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, வெற்று);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, வெற்று);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, வெற்று);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, வெற்று);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, வெற்று);

    கட்டமைப்பு matched_sval *mval = &matcher->rmlvo.model;
    எனில் (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_மதிப்பு,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (unsigned முழு) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        எனில் (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_மதிப்பு,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned முழு) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        எனில் (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_மதிப்பு,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned முழு) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        எனில் (!mval->sval.len)
            தொடர்;

        எனில் (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_மதிப்பு,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned முழு) mval->sval.len, mval->sval.start, rules);
        } இல்லையெனில் எனில் (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            மாறிலி xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            ப்ரதிஜ்ஞா(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_மதிப்பு,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (unsigned முழு)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    எனில் (out->symbols != வெற்று && explicit_layouts != வெற்று) {
        *explicit_layouts = 1; /* at least one group */
        மாறிலி எழுத்து *symbols = out->symbols;
        /* Take the highest modifier */
        போது ((symbols = strchr(symbols, ':')) != வெற்று && symbols[1] != '\0') {
            xkb_layout_index_t group = 0;
            மாறிலி முழு மொத்தம் = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            எனில் (மொத்தம் > 0 && (symbols[மொத்தம்] == '\0' ||
                is_merge_mode_prefix(symbols[மொத்தம்])) &&
                group > 0 && group <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, group);
                symbols += மொத்தம்;
            }
        }
    }

err_out:
    எனில் (file)
        fclose(file);

    திரும்பு ret;
}

bool
xkb_components_from_rmlvo_builder(மாறிலி கட்டமைப்பு xkb_rmlvo_builder *rmlvo,
                                  கட்டமைப்பு xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts)
{
    மாறிலி எழுத்து *rules = rmlvo->rules;
    கட்டமைப்பு matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    எனில் (!matcher)
        திரும்பு false;

    மாறிலி bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    திரும்பு ret;
}


bool
xkb_components_from_rules_names(கட்டமைப்பு xkb_context *ctx,
                                மாறிலி கட்டமைப்பு xkb_rule_names *rmlvo,
                                கட்டமைப்பு xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts)
{
    கட்டமைப்பு matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    எனில் (!matcher)
        திரும்பு false;

    மாறிலி bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    திரும்பு ret;
}
