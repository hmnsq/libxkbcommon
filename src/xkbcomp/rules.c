/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <stdint.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "context.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "rules.h"
#অন্তর্ভাব "rmlvo.h"
#অন্তর্ভাব "include.h"
#অন্তর্ভাব "scanner-utils.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "util-numbers.h"
#অন্তর্ভাব "util-paths.h"

#define MAX_INCLUDE_DEPTH 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
সঙ্ঘ lvalue {
    সংরচনা sval string;
};

গণনা rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_সারি,
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

স্থির গণনা rules_token
lex(সংরচনা scanner *s, সঙ্ঘ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    যাবত্ (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    যদি (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    যদি (scanner_eol(s)) {
        যাবত্ (scanner_eol(s)) scanner_next(s);
        প্রত্যাগম TOK_END_OF_সারি;
    }

    /* Escaped line continuation */
    যদি (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        যদি (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            প্রত্যাগম TOK_ERROR;
        }
        scanner_next(s);
        গচ্ছ skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    যদি (scanner_eof(s)) প্রত্যাগম TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    গণনা rules_token token = TOK_IDENTIFIER;

    ধ্রুব বর্ণ next = scanner_next(s);
    বিকল্প (next) {
    পক্ষ '!':
        প্রত্যাগম TOK_BANG;
    পক্ষ '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        ভঙ্গ;
    পক্ষ '*':
        যদি (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            ভঙ্গ;
        } অন্যথা {
            /* Legacy wild card */
            প্রত্যাগম TOK_WILD_CARD_STAR;
        }
    পক্ষ '<':
        /* Extended wild cards */
        যদি (scanner_rules_lit_token(s, "none>")) প্রত্যাগম TOK_WILD_CARD_NONE;
        যদি (scanner_rules_lit_token(s, "some>")) প্রত্যাগম TOK_WILD_CARD_SOME;
        যদি (scanner_rules_lit_token(s, "any>")) প্রত্যাগম TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        প্রতিজ্ঞা(scanner_rules_is_ident('<'));
        val->string.len = 1;
        ভঙ্গ;
    পক্ষ '=':
        যদি (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            ভঙ্গ;
        } অন্যথা {
            প্রত্যাগম TOK_EQUALS;
        }
    পক্ষ 'i':
        যদি (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            প্রত্যাগম TOK_INCLUDE;
        }
        /* Identifier */
        প্রতিজ্ঞা(scanner_rules_is_ident('i'));
        val->string.len = 1;
        ভঙ্গ;
    স্বভাব:
        যদি (likely(scanner_rules_is_ident(next))) {
            /* Identifier */
            val->string.len = 1;
        } অন্যথা {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.start = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    প্রতিজ্ঞা(scanner_rules_is_ident(MERGE_OVERRIDE_PREFIX));
    প্রতিজ্ঞা(scanner_rules_is_ident(MERGE_AUGMENT_PREFIX));
    প্রতিজ্ঞা(scanner_rules_is_ident(MERGE_REPLACE_PREFIX));
    প্রতিজ্ঞা(scanner_rules_is_ident('('));
    প্রতিজ্ঞা(scanner_rules_is_ident(')'));
    val->string.len += scanner_rules_ident_token(s);

    যদি (unlikely(val->string.len == 0)) {
        যদি (token == TOK_GROUP_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } অন্যথা {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unrecognized token");
        }
        প্রত্যাগম TOK_ERROR;
    }

    প্রত্যাগম token;
}

/***====================================================================***/

গণনা rules_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_অঙ্ক_ENTRIES
};

গণনা {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

প্রকারনির্দেশ uint8_t mlvo_index_t;
প্রকারনির্দেশ uint8_t mlvo_mask_t;


স্থির ধ্রুব সংরচনা sval rules_mlvo_svals[MLVO_অঙ্ক_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

গণনা rules_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_অঙ্ক_ENTRIES
};

প্রকারনির্দেশ uint8_t kccgst_index_t;
প্রকারনির্দেশ uint8_t kccgst_mask_t;

স্থির ধ্রুব সংরচনা sval rules_kccgst_svals[KCCGST_অঙ্ক_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

গণনা {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL ==
              (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
সংরচনা matched_sval {
    সংরচনা sval sval;
    /** Used for layout-specific options */
    xkb_layout_mask_t layouts;
    xkb_layout_mask_t matched;
};
প্রকারনির্দেশ darray(সংরচনা matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
সংরচনা rule_names {
    সংরচনা matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

সংরচনা group {
    সংরচনা sval name;
    darray_sval elements;
};

সংরচনা mapping {
    গণনা rules_mlvo mlvo_at_pos[MLVO_অঙ্ক_ENTRIES];
    mlvo_index_t অঙ্ক_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    সঙ্ঘ {
        সংরচনা { xkb_layout_index_t layout_idx, variant_idx; };
        সংরচনা { xkb_layout_index_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    সঙ্ঘ {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    গণনা rules_kccgst kccgst_at_pos[KCCGST_অঙ্ক_ENTRIES];
    kccgst_index_t অঙ্ক_kccgst;
    kccgst_mask_t defined_kccgst_mask;
};

গণনা mlvo_match_type {
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

গণনা wildcard_match_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_MATCH_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_MATCH_ALL,
};

সংরচনা rule {
    সংরচনা sval mlvo_মান_at_pos[MLVO_অঙ্ক_ENTRIES];
    গণনা mlvo_match_type match_type_at_pos[MLVO_অঙ্ক_ENTRIES];
    সংরচনা sval kccgst_মান_at_pos[KCCGST_অঙ্ক_ENTRIES];
    mlvo_index_t অঙ্ক_mlvo_মানসমূহ;
    kccgst_index_t অঙ্ক_kccgst_মানসমূহ;
    bool skip;
};

#define SLICE_KCCGST_BIT_FIELD_SIZE 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_SIZE - 1)) &
                (KCCGST_অঙ্ক_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
সংরচনা kccgst_buffer_slice {
    uint32_t length:(32 - SLICE_KCCGST_BIT_FIELD_SIZE);
    গণনা rules_kccgst kccgst:SLICE_KCCGST_BIT_FIELD_SIZE;
    xkb_layout_index_t layout;
};

/* Buffer for pending KcCGST values */
সংরচনা kccgst_buffer {
    darray_char buffer;
    /* Slice corresponding to each value in the buffer */
    darray(সংরচনা kccgst_buffer_slice) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
সংরচনা matcher {
    সংরচনা xkb_context *ctx;
    /* Input.*/
    সংরচনা rule_names rmlvo;
    সঙ্ঘ lvalue val;
    darray(সংরচনা group) groups;
    /* Current mapping. */
    সংরচনা mapping mapping;
    /* Current rule. */
    সংরচনা rule rule;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    সংরচনা kccgst_buffer pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_অঙ্ক_ENTRIES];
};

স্থির সংরচনা sval
strip_spaces(সংরচনা sval v)
{
    যাবত্ (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    যাবত্ (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    প্রত্যাগম v;
}

স্থির darray_matched_sval
split_comma_separated_mlvo(সংরচনা xkb_context *ctx,
                           গণনা rules_mlvo mlvo, ধ্রুব বর্ণ *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    যদি (!s) {
        ensure_at_least_one_মান:
        /* Label followed by a declaration is a C23 extension */ ;

        সংরচনা matched_sval val = {
            .sval = SVAL(রিক্ত, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        প্রত্যাগম arr;
    }

    যাবত্ (true) {
        সংরচনা matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        যাবত্ (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        যদি (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            ধ্রুব বর্ণ* ধ্রুব layout_start = s;
            xkb_layout_index_t layout = XKB_LAYOUT_INVALID;

            পূর্ণাঙ্ক গুনতি = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            যদি (গুনতি > 0) {
                /* Note: 1-indexed layout */
                s += গুনতি;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                যদি (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (unsigned পূর্ণাঙ্ক) val.sval.len, val.sval.start);
                } অন্যথা যদি (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (unsigned পূর্ণাঙ্ক) val.sval.len, val.sval.start);
                } অন্যথা {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ধ্রুব বর্ণ* ধ্রুব layout_index_end = s;
            যাবত্ (*s != '\0' && *s != ',') { s++; }
            যদি (গুনতি <= 0 || layout_index_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (unsigned পূর্ণাঙ্ক) (s - layout_start), layout_start,
                        (unsigned পূর্ণাঙ্ক) val.sval.len, val.sval.start);
            }
        }

        যদি (val.sval.len || mlvo != MLVO_OPTION) {
            যদি (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                সংরচনা matched_sval *prev;
                darray_foreach(prev, arr) {
                    যদি (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        যদি (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } অন্যথা যদি (prev->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        গচ্ছ next_মান;
                    }
                }
            }
            darray_append(arr, val);
        }

next_মান:
        যদি (*s == '\0') ভঙ্গ;
        যদি (*s == ',') s++;
    }

    যদি (darray_empty(arr))
        গচ্ছ ensure_at_least_one_মান;

    প্রত্যাগম arr;
}

স্থির সংরচনা matcher *
matcher_new_from_rmlvo(ধ্রুব সংরচনা xkb_rmlvo_builder *rmlvo, ধ্রুব বর্ণ **rules)
{
    সংরচনা matcher *m = calloc(1, পরিমাণ(*m));
    যদি (!m)
        প্রত্যাগম রিক্ত;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    সংরচনা xkb_rule_names names = {
        .rules = rmlvo->rules,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_empty(rmlvo->layouts)) ? রিক্ত : "x",
        .variant = (darray_empty(rmlvo->layouts)) ? রিক্ত : "x",
        .options = (darray_empty(rmlvo->options)) ? রিক্ত : "x",
    };
    ধ্রুব গণনা RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
                                                               &names);

    /* Initialize matcher with sanitized names, if relevant */
    যদি (changed & RMLVO_RULES) {
        *rules = names.rules;
    } অন্যথা {
        *rules = rmlvo->rules;
    }

    যদি (changed & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = names.model;
    } অন্যথা {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;

    প্রতিজ্ঞা((changed & RMLVO_LAYOUT) || !(changed & RMLVO_VARIANT));
    যদি (changed & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = split_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      names.layout);
        m->rmlvo.variants = split_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       names.variant);
        যদি (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            যদি (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        } অন্যথা যদি (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } অন্যথা {
        সংরচনা xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            সংরচনা matched_sval val = {
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

    যদি (changed & RMLVO_OPTIONS) {
        m->rmlvo.options = split_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      names.options);
    } অন্যথা {
        সংরচনা xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            সংরচনা matched_sval val = {
                .sval = SVAL(option->option, strlen_safe(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    প্রত্যাগম m;
}

স্থির সংরচনা matcher *
matcher_new_from_names(সংরচনা xkb_context *ctx,
            ধ্রুব সংরচনা xkb_rule_names *rmlvo)
{
    সংরচনা matcher *m = calloc(1, পরিমাণ(*m));
    যদি (!m)
        প্রত্যাগম রিক্ত;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    যদি (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        যদি (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
    } অন্যথা যদি (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    প্রত্যাগম m;
}

স্থির শূন্য
matcher_free(সংরচনা matcher *m)
{
    যদি (!m)
        প্রত্যাগম;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    সংরচনা group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.buffer);
    darray_free(m->pending_kccgst.slices);
    প্রতি (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_অঙ্ক_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    মুক্ত(m);
}

স্থির শূন্য
matcher_group_start_new(সংরচনা matcher *m, সংরচনা sval name)
{
    সংরচনা group group = { .name = name, .elements = darray_new() };
    darray_append(m->groups, group);
}

স্থির শূন্য
matcher_group_add_element(সংরচনা matcher *m, সংরচনা scanner *s,
                          সংরচনা sval element)
{
    darray_append(darray_item(m->groups, darray_size(m->groups) - 1).elements,
                  element);
}

স্থির bool
read_rules_file(সংরচনা xkb_context *ctx,
                সংরচনা matcher *matcher,
                unsigned পূর্ণাঙ্ক include_depth,
                FILE *file,
                ধ্রুব বর্ণ *path);

স্থির শূন্য
matcher_include(সংরচনা matcher *m, সংরচনা scanner *parent_scanner,
                unsigned পূর্ণাঙ্ক include_depth,
                সংরচনা sval inc)
{
    যদি (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        প্রত্যাগম;
    }

    ধ্রুব বর্ণ *stmt_file = inc.start;
    size_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    বর্ণ buf[PATH_MAX] = {0};
    ধ্রুব ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, পরিমাণ(buf));
    যদি (expanded < 0) {
        /* Error */
        প্রত্যাগম;
    } অন্যথা যদি (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (size_t) expanded;
        প্রতিজ্ঞা(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned পূর্ণাঙ্ক offset = 0;
    ধ্রুব bool absolute_path = is_absolute_path(stmt_file);
    যদি (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        যদি (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            যদি (stmt_file_len < পরিমাণ(buf)) {
                memcpy(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } অন্যথা {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, পরিমাণ(buf),
                        (unsigned পূর্ণাঙ্ক) stmt_file_len, stmt_file);
                প্রত্যাগম;
            }
        } অন্যথা {
            /* %-expansion is always NULL-terminated */
            প্রতিজ্ঞা(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } অন্যথা {
        /* Relative path: lookup the first XKB path */
        যদি (unlikely(expanded)) {
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
            file = রিক্ত;
        } অন্যথা {
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, পরিমাণ(buf), &offset, true);
        }
    }

    যাবত্ (file) {
        প্রতিজ্ঞা(strlen_safe(buf) < পরিমাণ(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        fclose(file);
        যদি (ret)
            প্রত্যাগম;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        যদি (absolute_path) {
            /* There is no point to search further if the path is absolute */
            ভঙ্গ;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, পরিমাণ(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (unsigned পূর্ণাঙ্ক) stmt_file_len, stmt_file);
}

স্থির শূন্য
matcher_mapping_start_new(সংরচনা matcher *m)
{
    প্রতি (mlvo_index_t i = 0; i < (mlvo_index_t) MLVO_অঙ্ক_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_অঙ্ক_ENTRIES;
    প্রতি (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_অঙ্ক_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_অঙ্ক_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.অঙ্ক_mlvo = m->mapping.অঙ্ক_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
স্থির পূর্ণাঙ্ক
parse_layout_int_index_tail(ধ্রুব বর্ণ *s, size_t max_len,
                            xkb_layout_index_t *out)
{
    যদি (max_len < 3)
        প্রত্যাগম -1;
    uint32_t val = 0;
    ধ্রুব পূর্ণাঙ্ক গুনতি = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    যদি (গুনতি <= 0 || s[1 + গুনতি] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        প্রত্যাগম -1;
    /* To zero-based index. */
    *out = val - 1;
    প্রত্যাগম গুনতি + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
স্থির পূর্ণাঙ্ক
extract_layout_index(ধ্রুব বর্ণ *s, size_t max_len, xkb_layout_index_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    যদি (max_len < 3 || s[0] != '[')
        প্রত্যাগম -1;
    যদি (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_index_t)(s[1] - '1');
        প্রত্যাগম 3;
    }
    যদি (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        প্রত্যাগম 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    প্রত্যাগম parse_layout_int_index_tail(s, max_len, out);
}

/* Special layout indices */
গণনা layout_index_ranges {
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
স্থির bool
extract_mapping_layout_index(ধ্রুব বর্ণ *s, size_t max_len,
                             xkb_layout_index_t *out)
{
    /* Check for minimal `[` + index + `]` */
    যদি (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        প্রত্যাগম false;
    }

    /* Small numeric index */
    যদি (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_index_t)(s[1] - '1');
        প্রত্যাগম true;
    }

    /* Named indices ranges */
    স্থির ধ্রুব সংরচনা {
        ধ্রুব বর্ণ* name;
        size_t size;
        xkb_layout_index_t মান;
    } names[] = {
        #define INDEX_RANGE(tail, মান) \
            { (tail), পরিমাণ(tail), (xkb_layout_index_t)(মান) }
        INDEX_RANGE("single]",   LAYOUT_INDEX_SINGLE),
        INDEX_RANGE("first]",    LAYOUT_INDEX_FIRST),
        INDEX_RANGE("later]",    LAYOUT_INDEX_LATER),
        INDEX_RANGE("multiple]", LAYOUT_INDEX_MULTIPLE),
        INDEX_RANGE("any]",      LAYOUT_INDEX_ANY),
        #undef INDEX_RANGE
    };

    প্রতি (size_t k = 0; k < ARRAY_SIZE(names); k++) {
        যদি (max_len == names[k].size &&
            memcmp(&s[1], names[k].name, names[k].size - 1) == 0) {
            *out = names[k].মান;
            প্রত্যাগম true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    ধ্রুব পূর্ণাঙ্ক গুনতি = parse_layout_int_index_tail(s, max_len, out);
    প্রত্যাগম গুনতি > 0 && (size_t)গুনতি == max_len;
}

স্থির অন্তঃস্থ bool
is_mlvo_mask_defined(সংরচনা matcher *m, গণনা rules_mlvo mlvo)
{
    প্রত্যাগম m->mapping.defined_mlvo_mask & (1u << mlvo);
}

স্থির শূন্য
matcher_mapping_set_mlvo(সংরচনা matcher *m, সংরচনা scanner *s,
                         সংরচনা sval ident)
{
    গণনা rules_mlvo mlvo;
    সংরচনা sval mlvo_sval;

    প্রতি (mlvo = 0; mlvo < MLVO_অঙ্ক_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        যদি (svaleq_prefix(mlvo_sval, ident))
            ভঙ্গ;
    }

    /* Not found. */
    যদি (mlvo >= MLVO_অঙ্ক_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned পূর্ণাঙ্ক) ident.len, ident.start);
        m->mapping.active = false;
        প্রত্যাগম;
    }

    যদি (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned পূর্ণাঙ্ক) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        প্রত্যাগম;
    }

    /* If there are leftovers still, it must be an index. */
    যদি (mlvo_sval.len < ident.len) {
        xkb_layout_index_t idx;
        যদি (!extract_mapping_layout_index(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (unsigned পূর্ণাঙ্ক) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            প্রত্যাগম;
        }

        যদি (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        অন্যথা যদি (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        অন্যথা {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (unsigned পূর্ণাঙ্ক) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            প্রত্যাগম;
        }
    } অন্যথা যদি (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    } অন্যথা যদি (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    যদি (((mlvo == MLVO_LAYOUT && is_mlvo_mask_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_mask_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.active = false;
        প্রত্যাগম;
    }

    m->mapping.mlvo_at_pos[m->mapping.অঙ্ক_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.অঙ্ক_mlvo++;
}

স্থির শূন্য
matcher_mapping_set_layout_bounds(সংরচনা matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_index_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    বিকল্প (idx) {
        পক্ষ XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            প্রতিজ্ঞা(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            ভঙ্গ;
        পক্ষ LAYOUT_INDEX_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            ভঙ্গ;
        পক্ষ LAYOUT_INDEX_MULTIPLE:
        পক্ষ LAYOUT_INDEX_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            ভঙ্গ;
        পক্ষ LAYOUT_INDEX_SINGLE:
        পক্ষ LAYOUT_INDEX_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        স্বভাব:
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

স্থির শূন্য
matcher_mapping_set_kccgst(সংরচনা matcher *m, সংরচনা scanner *s, সংরচনা sval ident)
{
    গণনা rules_kccgst kccgst;
    সংরচনা sval kccgst_sval;

    প্রতি (kccgst = 0; kccgst < KCCGST_অঙ্ক_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        যদি (svaleq(rules_kccgst_svals[kccgst], ident))
            ভঙ্গ;
    }

    /* Not found. */
    যদি (kccgst >= KCCGST_অঙ্ক_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned পূর্ণাঙ্ক) ident.len, ident.start);
        m->mapping.active = false;
        প্রত্যাগম;
    }

    যদি (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned পূর্ণাঙ্ক) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        প্রত্যাগম;
    }

    m->mapping.kccgst_at_pos[m->mapping.অঙ্ক_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.অঙ্ক_kccgst++;
}

স্থির bool
matcher_mapping_verify(সংরচনা matcher *m, সংরচনা scanner *s)
{
    যদি (m->mapping.অঙ্ক_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        গচ্ছ skip;
    }

    যদি (m->mapping.অঙ্ক_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        গচ্ছ skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    যদি (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        প্রতিজ্ঞা(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        বিকল্প (m->mapping.layout_idx) {
            পক্ষ LAYOUT_INDEX_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                যদি (darray_size(m->rmlvo.layouts) > 1)
                    গচ্ছ skip;
                ভঙ্গ;
            পক্ষ LAYOUT_INDEX_MULTIPLE:
            পক্ষ LAYOUT_INDEX_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                যদি (darray_size(m->rmlvo.layouts) < 2)
                    গচ্ছ skip;
                ভঙ্গ;
            পক্ষ LAYOUT_INDEX_ANY:
            পক্ষ LAYOUT_INDEX_FIRST:
                /* No restrictions */
                ভঙ্গ;
            স্বভাব:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                যদি (darray_size(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_size(m->rmlvo.layouts))
                    গচ্ছ skip;
        }
    }

    যদি (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        প্রতিজ্ঞা(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        বিকল্প (m->mapping.variant_idx) {
            পক্ষ LAYOUT_INDEX_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                যদি (darray_size(m->rmlvo.variants) > 1)
                    গচ্ছ skip;
                ভঙ্গ;
            পক্ষ LAYOUT_INDEX_ANY:
            পক্ষ LAYOUT_INDEX_LATER:
            পক্ষ LAYOUT_INDEX_FIRST:
                /* No restriction */
                ভঙ্গ;
            স্বভাব:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                যদি (darray_size(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_size(m->rmlvo.variants))
                    গচ্ছ skip;
        }
    }

    প্রত্যাগম true;

skip:
    m->mapping.active = false;
    প্রত্যাগম false;
}

স্থির শূন্য
matcher_rule_start_new(সংরচনা matcher *m)
{
    memset(&m->rule, 0, পরিমাণ(m->rule));
    m->rule.skip = !m->mapping.active;
}

স্থির শূন্য
matcher_rule_set_mlvo_common(সংরচনা matcher *m, সংরচনা scanner *s,
                             সংরচনা sval ident,
                             গণনা mlvo_match_type match_type)
{
    যদি (m->rule.অঙ্ক_mlvo_মানসমূহ >= m->mapping.অঙ্ক_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        প্রত্যাগম;
    }
    m->rule.match_type_at_pos[m->rule.অঙ্ক_mlvo_মানসমূহ] = match_type;
    m->rule.mlvo_মান_at_pos[m->rule.অঙ্ক_mlvo_মানসমূহ] = ident;
    m->rule.অঙ্ক_mlvo_মানসমূহ++;
}

স্থির শূন্য
matcher_rule_set_mlvo_wildcard(সংরচনা matcher *m, সংরচনা scanner *s,
                               গণনা mlvo_match_type match_type)
{
    সংরচনা sval dummy = SVAL(রিক্ত, 0);
    matcher_rule_set_mlvo_common(m, s, dummy, match_type);
}

স্থির শূন্য
matcher_rule_set_mlvo_group(সংরচনা matcher *m, সংরচনা scanner *s,
                            সংরচনা sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_GROUP);
}

স্থির শূন্য
matcher_rule_set_mlvo(সংরচনা matcher *m, সংরচনা scanner *s,
                      সংরচনা sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_NORMAL);
}

স্থির শূন্য
matcher_rule_set_kccgst(সংরচনা matcher *m, সংরচনা scanner *s,
                        সংরচনা sval ident)
{
    যদি (m->rule.অঙ্ক_kccgst_মানসমূহ >= m->mapping.অঙ্ক_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        প্রত্যাগম;
    }
    m->rule.kccgst_মান_at_pos[m->rule.অঙ্ক_kccgst_মানসমূহ] = ident;
    m->rule.অঙ্ক_kccgst_মানসমূহ++;
}

স্থির bool
match_group(সংরচনা matcher *m, সংরচনা sval group_name, সংরচনা sval to)
{
    সংরচনা group *group = রিক্ত;
    সংরচনা sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        যদি (svaleq(group->name, group_name)) {
            found = true;
            ভঙ্গ;
        }
    }

    যদি (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        প্রত্যাগম false;
    }

    darray_foreach(element, group->elements)
        যদি (svaleq(to, *element))
            প্রত্যাগম true;

    প্রত্যাগম false;
}

স্থির bool
match_মান(সংরচনা matcher *m, ধ্রুব সংরচনা sval val, ধ্রুব সংরচনা sval to,
            গণনা mlvo_match_type match_type,
            গণনা wildcard_match_type wildcard_type)
{
    বিকল্প (match_type) {
        পক্ষ MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            প্রত্যাগম wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        পক্ষ MLVO_MATCH_WILDCARD_NONE:
            প্রত্যাগম !to.len;
        পক্ষ MLVO_MATCH_WILDCARD_SOME:
            প্রত্যাগম !!to.len;
        পক্ষ MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            প্রত্যাগম true;
        পক্ষ MLVO_MATCH_GROUP:
            প্রত্যাগম match_group(m, val, to);
        স্বভাব:
            প্রতিজ্ঞা(match_type == MLVO_MATCH_NORMAL);
            প্রত্যাগম svaleq(val, to);
    }
}

স্থির bool
match_মান_and_mark(সংরচনা matcher *m, ধ্রুব সংরচনা sval val,
                     সংরচনা matched_sval *to, গণনা mlvo_match_type match_type,
                     গণনা wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_মান(m, val, to->sval, match_type, wildcard_type);
    যদি (matched)
        to->matched |= layouts;
    প্রত্যাগম matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
স্থির bool
expand_rmlvo_in_kccgst_মান(সংরচনা matcher *m, সংরচনা scanner *s,
                             সংরচনা sval মান, xkb_layout_index_t layout_idx,
                             darray_char *expanded, size_t *i)
{
    ধ্রুব বর্ণ *str = মান.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    গণনা rules_mlvo mlv;
    xkb_layout_index_t idx;
    বর্ণ pfx, sfx;
    সংরচনা matched_sval *expanded_মান;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    যদি (str[*i] == 'i' &&
        (*i + 1 == মান.len || is_merge_mode_prefix(str[*i + 1])))
    {
        যদি (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_INDEX_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            গচ্ছ error;
        }
        (*i)++;
        বর্ণ index_str[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
        ধ্রুব পূর্ণাঙ্ক গুনতি = snprintf(index_str, পরিমাণ(index_str), "%"PRIu32,
                                   layout_idx + 1);
        প্রতিজ্ঞা(গুনতি > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, index_str, (darray_size_t)গুনতি);
        প্রত্যাগম true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    যদি (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        যদি (str[*i] == '(') sfx = ')';
        যদি (++(*i) >= মান.len) গচ্ছ error;
    }

    /* Mandatory model/layout/variant specifier. */
    বিকল্প (str[(*i)++]) {
    পক্ষ 'm': mlv = MLVO_MODEL; ভঙ্গ;
    পক্ষ 'l': mlv = MLVO_LAYOUT; ভঙ্গ;
    পক্ষ 'v': mlv = MLVO_VARIANT; ভঙ্গ;
    স্বভাব: গচ্ছ error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_index = false;
    যদি (*i < মান.len && str[*i] == '[') {
        যদি (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            গচ্ছ error;
        }

        পূর্ণাঙ্ক consumed = extract_layout_index(str + (*i), মান.len - (*i), &idx);
        যদি (consumed <= 0) গচ্ছ error;
        যদি (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            প্রতিজ্ঞা(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_index = true;
        }
        *i += (size_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    যদি (sfx != 0) {
        যদি (*i >= মান.len) গচ্ছ error;
        যদি (str[(*i)++] != sfx) গচ্ছ error;
    }

    /* Get the expanded value. */
    expanded_মান = রিক্ত;

    যদি (mlv == MLVO_LAYOUT) {
        যদি (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            যদি (darray_size(m->rmlvo.layouts) == 1)
                expanded_মান = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } অন্যথা যদি (idx < darray_size(m->rmlvo.layouts) &&
                   (variable_index || darray_size(m->rmlvo.layouts) > 1)) {
                expanded_মান = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    অন্যথা যদি (mlv == MLVO_VARIANT) {
        যদি (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            যদি (darray_size(m->rmlvo.variants) == 1)
                expanded_মান = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } অন্যথা যদি (idx < darray_size(m->rmlvo.variants) &&
                   (variable_index || darray_size(m->rmlvo.variants) > 1)) {
                expanded_মান = &darray_item(m->rmlvo.variants, idx);
        }
    }
    অন্যথা যদি (mlv == MLVO_MODEL) {
        expanded_মান = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    যদি (!expanded_মান || expanded_মান->sval.len == 0) {
        প্রত্যাগম true;
    }

    যদি (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_মান->sval.start,
                                 (darray_size_t) expanded_মান->sval.len);
    যদি (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    যদি (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ধ্রুব xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_মান->matched |= layouts;
    } অন্যথা {
        expanded_মান->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    প্রত্যাগম true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    প্রত্যাগম false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
স্থির শূন্য
expand_qualifier_in_kccgst_মান(
    সংরচনা matcher *m, সংরচনা scanner *s,
    সংরচনা sval মান, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_size_t prefix_idx, size_t *i)
{
    ধ্রুব বর্ণ *str = মান.start;

    /* “all” followed by nothing or by a layout separator */
    যদি (*i + 2 < মান.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == মান.len || is_merge_mode_prefix(str[*i + 3]))) {
        যদি (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_MESSAGE_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        যদি (darray_size(m->rmlvo.layouts) > 1) {
            বর্ণ layout_index[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
            ধ্রুব darray_size_t prefix_length =
                darray_size(*expanded) - prefix_idx - 1;
            প্রতি (xkb_layout_index_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_size(m->rmlvo.layouts));
                 l++)
            {
                যদি (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_length);
                /* Append index */
                ধ্রুব পূর্ণাঙ্ক গুনতি = snprintf(layout_index, পরিমাণ(layout_index),
                                           "%"PRIu32, l + 1);
                প্রতিজ্ঞা(গুনতি > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_index,
                                             (darray_size_t)গুনতি);
            }
        }
        *i += 3;
    }
}

স্থির অন্তঃস্থ শূন্য
#ifdef _MSC_VER
concat_kccgst(darray_char *into, darray_size_t size, _In_reads_(size) ধ্রুব বর্ণ* from)
#অন্যথা
concat_kccgst(darray_char *into, darray_size_t size, ধ্রুব বর্ণ from[স্থির size])
#endif
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ধ্রুব bool from_plus = is_merge_mode_prefix(from[0]);
    যদি (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, size);
    } অন্যথা {
        ধ্রুব বর্ণ ch =
            (বর্ণ) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        ধ্রুব bool into_plus = is_merge_mode_prefix(ch);
        যদি (into_plus)
            darray_prepends_nullterminate(*into, from, size);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
স্থির bool
append_expanded_kccgst_মান(সংরচনা matcher *m, সংরচনা scanner *s,
                             bool merge, darray_char *to, সংরচনা sval মান,
                             xkb_layout_index_t layout_idx)
{
    ধ্রুব বর্ণ *str = মান.start;
    darray_char expanded = darray_new();
    darray_size_t last_item_idx = 0;
    bool has_separator = false;

    প্রতি (size_t i = 0; i < মান.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        বিকল্প (str[i]) {
            /* Qualifier */
            পক্ষ ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_মান(m, s, মান, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                ভঙ্গ;
            /* Expansion */
            পক্ষ '%':
                i++;
                যদি (i >= মান.len ||
                    !expand_rmlvo_in_kccgst_মান(m, s, মান, layout_idx,
                                                  &expanded, &i))
                        গচ্ছ error;
                ভঙ্গ;
            /* New item */
            পক্ষ MERGE_OVERRIDE_PREFIX:
            পক্ষ MERGE_AUGMENT_PREFIX:
            পক্ষ MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_size(expanded) - 1;
                has_separator = true;
                ভঙ্গ;
            /* Just a normal character. */
            স্বভাব:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    যদি (merge) {
        যদি (!darray_empty(expanded))
            concat_kccgst(to, darray_size(expanded), darray_বস্তুসমূহ(expanded));
    } অন্যথা {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    প্রত্যাগম true;

error:
    darray_free(expanded);
    প্রত্যাগম false;
}

স্থির bool
matcher_append_pending_kccgst(সংরচনা matcher *m)
{
    যদি (!m->mapping.has_multiple_layouts)
        প্রত্যাগম true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    প্রতি (kccgst_index_t i = 0; i < m->mapping.অঙ্ক_kccgst; i++) {
        ধ্রুব গণনা rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        প্রতি (xkb_layout_index_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            পঞ্জিকা ধ্রুব সংরচনা kccgst_buffer* ধ্রুব buf = &m->pending_kccgst;
            size_t offset = 0;
            প্রতি (darray_size_t k = 0; k < darray_size(buf->slices); k++) {
                পঞ্জিকা ধ্রুব সংরচনা kccgst_buffer_slice * ধ্রুব slice =
                    &darray_item(buf->slices, k);
                যদি (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->length)
                    concat_kccgst(&m->kccgst[kccgst], slice->length,
                                  darray_বস্তুসমূহ(buf->buffer) + offset);
                offset += slice->length;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    প্রত্যাগম true;
}

স্থির শূন্য
matcher_rule_verify(সংরচনা matcher *m, সংরচনা scanner *s)
{
    যদি (m->rule.অঙ্ক_mlvo_মানসমূহ != m->mapping.অঙ্ক_mlvo ||
        m->rule.অঙ্ক_kccgst_মানসমূহ != m->mapping.অঙ্ক_kccgst) {
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
স্থির bool
matcher_rule_match_model(সংরচনা matcher *m, ধ্রুব সংরচনা sval মান,
                         গণনা mlvo_match_type match_type)
{
    প্রত্যাগম match_মান_and_mark(
        m, মান, &m->rmlvo.model, match_type, WILDCARD_MATCH_ALL,
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
স্থির bool
matcher_rule_match_option(সংরচনা matcher *m, ধ্রুব সংরচনা sval মান,
                          গণনা mlvo_match_type match_type,
                          xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    প্রতিজ্ঞা(!darray_empty(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_mask_t unmatched =
        (m->mapping.defined_mlvo_mask & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    সংরচনা matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        ধ্রুব xkb_layout_mask_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
        যদি (matchable && match_মান_and_mark(m, মান, option, match_type,
                                              WILDCARD_MATCH_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            যদি (!unmatched)
                ভঙ্গ;
        }
    }
    যদি (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    প্রত্যাগম matched;
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
স্থির bool
matcher_rule_match_layout_or_variant(সংরচনা matcher *m,
                                     darray_matched_sval *প্রবেশ,
                                     ধ্রুব সংরচনা sval মান,
                                     গণনা mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    প্রতিজ্ঞা(!darray_empty(*প্রবেশ));

    bool matched = false;
    /* Loop over the layout index range */
    প্রতি (xkb_layout_index_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ধ্রুব xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        যদি (layout & *candidates) {
            সংরচনা matched_sval *to = &darray_item(*প্রবেশ, idx);
            যদি (match_মান_and_mark(m, মান, to, match_type,
                                     WILDCARD_MATCH_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } অন্যথা {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    প্রত্যাগম matched;
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
স্থির শূন্য
matcher_rule_apply_if_matches(সংরচনা matcher *m, সংরচনা scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_mask_t candidate_layouts = m->mapping.layouts_candidates_mask;

    /* Loop over MLVO pattern components */
    প্রতি (mlvo_index_t i = 0; i < m->mapping.অঙ্ক_mlvo; i++) {
        ধ্রুব গণনা rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ধ্রুব সংরচনা sval মান = m->rule.mlvo_মান_at_pos[i];
        ধ্রুব গণনা mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        বিকল্প (mlvo) {
        পক্ষ MLVO_MODEL:
            matched = matcher_rule_match_model(m, মান, match_type);
            ভঙ্গ;
        পক্ষ MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, মান, match_type, &candidate_layouts
            );
            ভঙ্গ;
        পক্ষ MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, মান, match_type, &candidate_layouts
            );
            ভঙ্গ;
        পক্ষ MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, মান, match_type, &candidate_layouts
            );
            ভঙ্গ;
        স্বভাব: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_অঙ্ক_ENTRIES - 1,
                          "Unexpected MLVO field");
            প্রতিজ্ঞা(!"Unreachable");
        }
        }

        যদি (!matched)
            প্রত্যাগম;
    }

    যদি (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        প্রতি (xkb_layout_index_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            যদি (candidate_layouts & (UINT32_C(1) << idx)) {
                প্রতি (kccgst_index_t i = 0; i < m->mapping.অঙ্ক_kccgst; i++) {
                    ধ্রুব গণনা rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ধ্রুব সংরচনা sval মান = m->rule.kccgst_মান_at_pos[i];
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
                    সংরচনা kccgst_buffer * ধ্রুব buf = &m->pending_kccgst;
                    ধ্রুব darray_size_t prev_buffer_length =
                        darray_size(buf->buffer);
                    append_expanded_kccgst_মান(m, s, false, &buf->buffer,
                                                 মান, idx);
                    ধ্রুব uint32_t length = (uint32_t) (darray_size(buf->buffer)
                                          - prev_buffer_length);
                    ধ্রুব সংরচনা kccgst_buffer_slice slice = {
                        .length = length,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } অন্যথা {
        /* Numeric index or no index */
        প্রতি (kccgst_index_t i = 0; i < m->mapping.অঙ্ক_kccgst; i++) {
            গণনা rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            সংরচনা sval মান = m->rule.kccgst_মান_at_pos[i];
            append_expanded_kccgst_মান(m, s, true, &m->kccgst[kccgst], মান,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    যদি (!(is_mlvo_mask_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_mask &= ~candidate_layouts;
    }
}

স্থির গণনা rules_token
gettok(সংরচনা matcher *m, সংরচনা scanner *s)
{
    প্রত্যাগম lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
স্থির bool
matcher_match(সংরচনা matcher *m, সংরচনা scanner *s,
              unsigned পূর্ণাঙ্ক include_depth,
              ধ্রুব বর্ণ *string, size_t len,
              ধ্রুব বর্ণ *file_name)
{
    গণনা rules_token tok;

    যদি (!m)
        প্রত্যাগম false;

initial:
    বিকল্প (tok = gettok(m, s)) {
    পক্ষ TOK_BANG:
        গচ্ছ bang;
    পক্ষ TOK_END_OF_সারি:
        গচ্ছ initial;
    পক্ষ TOK_END_OF_FILE:
        গচ্ছ finish;
    স্বভাব:
        গচ্ছ unexpected;
    }

bang:
    বিকল্প (tok = gettok(m, s)) {
    পক্ষ TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        গচ্ছ group_name;
    পক্ষ TOK_INCLUDE:
        গচ্ছ include_statement;
    পক্ষ TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        গচ্ছ mapping_mlvo;
    স্বভাব:
        গচ্ছ unexpected;
    }

group_name:
    বিকল্প (tok = gettok(m, s)) {
    পক্ষ TOK_EQUALS:
        গচ্ছ group_element;
    স্বভাব:
        গচ্ছ unexpected;
    }

group_element:
    বিকল্প (tok = gettok(m, s)) {
    পক্ষ TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        গচ্ছ group_element;
    পক্ষ TOK_END_OF_সারি:
        গচ্ছ initial;
    স্বভাব:
        গচ্ছ unexpected;
    }

include_statement:
    বিকল্প (tok = gettok(m, s)) {
    পক্ষ TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        গচ্ছ include_statement_end;
    স্বভাব:
        গচ্ছ unexpected;
    }

include_statement_end:
    বিকল্প (tok = gettok(m, s)) {
    পক্ষ TOK_END_OF_সারি:
        গচ্ছ initial;
    স্বভাব:
        গচ্ছ unexpected;
    }

mapping_mlvo:
    বিকল্প (tok = gettok(m, s)) {
    পক্ষ TOK_IDENTIFIER:
        যদি (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        গচ্ছ mapping_mlvo;
    পক্ষ TOK_EQUALS:
        গচ্ছ mapping_kccgst;
    স্বভাব:
        গচ্ছ unexpected;
    }

mapping_kccgst:
    বিকল্প (tok = gettok(m, s)) {
    পক্ষ TOK_IDENTIFIER:
        যদি (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        গচ্ছ mapping_kccgst;
    পক্ষ TOK_END_OF_সারি:
        যদি (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            যদি (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_size(m->pending_kccgst.buffer) = 0;
                darray_size(m->pending_kccgst.slices) = 0;
            }
        }
        গচ্ছ rule_mlvo_first;
    স্বভাব:
        গচ্ছ unexpected;
    }

rule_mlvo_first:
    বিকল্প (tok = gettok(m, s)) {
    পক্ষ TOK_BANG:
        matcher_append_pending_kccgst(m);
        গচ্ছ bang;
    পক্ষ TOK_END_OF_সারি:
        গচ্ছ rule_mlvo_first;
    পক্ষ TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        গচ্ছ finish;
    স্বভাব:
        matcher_rule_start_new(m);
        গচ্ছ rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    বিকল্প (tok) {
    পক্ষ TOK_IDENTIFIER:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        গচ্ছ rule_mlvo;
    পক্ষ TOK_WILD_CARD_STAR:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        গচ্ছ rule_mlvo;
    পক্ষ TOK_WILD_CARD_NONE:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        গচ্ছ rule_mlvo;
    পক্ষ TOK_WILD_CARD_SOME:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        গচ্ছ rule_mlvo;
    পক্ষ TOK_WILD_CARD_ANY:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        গচ্ছ rule_mlvo;
    পক্ষ TOK_GROUP_NAME:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        গচ্ছ rule_mlvo;
    পক্ষ TOK_EQUALS:
        গচ্ছ rule_kccgst;
    স্বভাব:
        গচ্ছ unexpected;
    }

rule_kccgst:
    বিকল্প (tok = gettok(m, s)) {
    পক্ষ TOK_IDENTIFIER:
        যদি (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        গচ্ছ rule_kccgst;
    পক্ষ TOK_END_OF_সারি:
        যদি (!m->rule.skip)
            matcher_rule_verify(m, s);
        যদি (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        গচ্ছ rule_mlvo_first;
    স্বভাব:
        গচ্ছ unexpected;
    }

unexpected:
    বিকল্প (tok) {
    পক্ষ TOK_ERROR:
        গচ্ছ error;
    স্বভাব:
        গচ্ছ state_error;
    }

finish:
    প্রত্যাগম true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    প্রত্যাগম false;
}
// NOLINTEND(readability-trivial-switch)

স্থির bool
read_rules_file(সংরচনা xkb_context *ctx,
                সংরচনা matcher *matcher,
                unsigned পূর্ণাঙ্ক include_depth,
                FILE *file,
                ধ্রুব বর্ণ *path)
{
    bool ret;
    বর্ণ *string;
    size_t size;
    সংরচনা scanner scanner;

    যদি (!map_file(file, &string, &size)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, strerror(errno));
        প্রত্যাগম false;
    }

    scanner_init(&scanner, matcher->ctx, string, size, path, রিক্ত);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    যদি (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, size);
        প্রত্যাগম false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, size, path);
    unmap_file(string, size);
    প্রত্যাগম ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
স্থির bool
xkb_resolve_partial_rules(সংরচনা xkb_context *ctx, বর্ণ *path, size_t path_size,
                          ধ্রুব বর্ণ* rules, ধ্রুব বর্ণ* suffix,
                          সংরচনা matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    বর্ণ partial_rules[60]; /* Arbitrary, but we do not expect long names */
    যদি (unlikely(!snprintf_safe(partial_rules, পরিমাণ(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        প্রত্যাগম false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    unsigned পূর্ণাঙ্ক offset = 0;
    FILE *file = রিক্ত;
    ধ্রুব size_t len = strlen(partial_rules);
    যাবত্ ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_size, &offset, false)) != রিক্ত) {
        ধ্রুব bool ok = read_rules_file(ctx, matcher, 0, file, path);
        fclose(file);
        যদি (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            প্রত্যাগম false;
        }
        offset++;
    }

    প্রত্যাগম true;
}

স্থির bool
xkb_resolve_rules(সংরচনা xkb_context *ctx,
                  ধ্রুব বর্ণ* rules, সংরচনা matcher *matcher,
                  সংরচনা xkb_component_names *out,
                  xkb_layout_index_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    unsigned পূর্ণাঙ্ক offset = 0;
    বর্ণ path[PATH_MAX];
    FILE * ধ্রুব file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, strlen(rules), FILE_TYPE_RULES,
                                          path, পরিমাণ(path), &offset, true);
    যদি (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        গচ্ছ err_out;
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
    ret = xkb_resolve_partial_rules(ctx, path, পরিমাণ(path),
                                    rules, ".pre", matcher);
    যদি (!ret)
        গচ্ছ err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    যদি (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        গচ্ছ err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, পরিমাণ(path),
                                    rules, ".post", matcher);
    যদি (!ret)
        গচ্ছ err_out;

    /* Check we got required components */
    যদি (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        গচ্ছ err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, রিক্ত);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, রিক্ত);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, রিক্ত);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, রিক্ত);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, রিক্ত);

    সংরচনা matched_sval *mval = &matcher->rmlvo.model;
    যদি (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_মান,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (unsigned পূর্ণাঙ্ক) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        যদি (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_মান,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned পূর্ণাঙ্ক) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        যদি (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_মান,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned পূর্ণাঙ্ক) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        যদি (!mval->sval.len)
            অনুবর্ত;

        যদি (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_মান,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned পূর্ণাঙ্ক) mval->sval.len, mval->sval.start, rules);
        } অন্যথা যদি (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ধ্রুব xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            প্রতিজ্ঞা(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_মান,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (unsigned পূর্ণাঙ্ক)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    যদি (out->symbols != রিক্ত && explicit_layouts != রিক্ত) {
        *explicit_layouts = 1; /* at least one group */
        ধ্রুব বর্ণ *symbols = out->symbols;
        /* Take the highest modifier */
        যাবত্ ((symbols = strchr(symbols, ':')) != রিক্ত && symbols[1] != '\0') {
            xkb_layout_index_t group = 0;
            ধ্রুব পূর্ণাঙ্ক গুনতি = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            যদি (গুনতি > 0 && (symbols[গুনতি] == '\0' ||
                is_merge_mode_prefix(symbols[গুনতি])) &&
                group > 0 && group <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, group);
                symbols += গুনতি;
            }
        }
    }

err_out:
    যদি (file)
        fclose(file);

    প্রত্যাগম ret;
}

bool
xkb_components_from_rmlvo_builder(ধ্রুব সংরচনা xkb_rmlvo_builder *rmlvo,
                                  সংরচনা xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts)
{
    ধ্রুব বর্ণ *rules = rmlvo->rules;
    সংরচনা matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    যদি (!matcher)
        প্রত্যাগম false;

    ধ্রুব bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    প্রত্যাগম ret;
}


bool
xkb_components_from_rules_names(সংরচনা xkb_context *ctx,
                                ধ্রুব সংরচনা xkb_rule_names *rmlvo,
                                সংরচনা xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts)
{
    সংরচনা matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    যদি (!matcher)
        প্রত্যাগম false;

    ধ্রুব bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    প্রত্যাগম ret;
}
