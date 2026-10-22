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
সংঘ lvalue {
    কাঠামো sval string;
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
lex(কাঠামো scanner *s, সংঘ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    যখন (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    যদি (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    যদি (scanner_eol(s)) {
        যখন (scanner_eol(s)) scanner_পরবর্তী(s);
        ফেরত TOK_END_OF_সারি;
    }

    /* Escaped line continuation */
    যদি (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        যদি (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            ফেরত TOK_ERROR;
        }
        scanner_পরবর্তী(s);
        যাও skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    যদি (scanner_eof(s)) ফেরত TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    গণনা rules_token token = TOK_IDENTIFIER;

    ধ্রুবক অক্ষর পরবর্তী = scanner_পরবর্তী(s);
    নির্বাচন (পরবর্তী) {
    ক্ষেত্র '!':
        ফেরত TOK_BANG;
    ক্ষেত্র '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        বিরতি;
    ক্ষেত্র '*':
        যদি (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            বিরতি;
        } অন্যথা {
            /* Legacy wild card */
            ফেরত TOK_WILD_CARD_STAR;
        }
    ক্ষেত্র '<':
        /* Extended wild cards */
        যদি (scanner_rules_lit_token(s, "none>")) ফেরত TOK_WILD_CARD_NONE;
        যদি (scanner_rules_lit_token(s, "some>")) ফেরত TOK_WILD_CARD_SOME;
        যদি (scanner_rules_lit_token(s, "any>")) ফেরত TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        প্রতিজ্ঞা(scanner_rules_is_ident('<'));
        val->string.len = 1;
        বিরতি;
    ক্ষেত্র '=':
        যদি (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            বিরতি;
        } অন্যথা {
            ফেরত TOK_EQUALS;
        }
    ক্ষেত্র 'i':
        যদি (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            ফেরত TOK_INCLUDE;
        }
        /* Identifier */
        প্রতিজ্ঞা(scanner_rules_is_ident('i'));
        val->string.len = 1;
        বিরতি;
    পূর্বনির্ধারিত:
        যদি (likely(scanner_rules_is_ident(পরবর্তী))) {
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
        ফেরত TOK_ERROR;
    }

    ফেরত token;
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

প্রকারলেখ uint8_t mlvo_সূচক_t;
প্রকারলেখ uint8_t mlvo_mask_t;


স্থির ধ্রুবক কাঠামো sval rules_mlvo_svals[MLVO_অঙ্ক_ENTRIES] = {
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

প্রকারলেখ uint8_t kccgst_সূচক_t;
প্রকারলেখ uint8_t kccgst_mask_t;

স্থির ধ্রুবক কাঠামো sval rules_kccgst_svals[KCCGST_অঙ্ক_ENTRIES] = {
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
কাঠামো matched_sval {
    কাঠামো sval sval;
    /** Used for layout-specific options */
    xkb_layout_mask_t layouts;
    xkb_layout_mask_t matched;
};
প্রকারলেখ darray(কাঠামো matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
কাঠামো rule_names {
    কাঠামো matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

কাঠামো group {
    কাঠামো sval name;
    darray_sval elements;
};

কাঠামো mapping {
    গণনা rules_mlvo mlvo_at_pos[MLVO_অঙ্ক_ENTRIES];
    mlvo_সূচক_t অঙ্ক_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    সংঘ {
        কাঠামো { xkb_layout_সূচক_t layout_idx, variant_idx; };
        কাঠামো { xkb_layout_সূচক_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    সংঘ {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    গণনা rules_kccgst kccgst_at_pos[KCCGST_অঙ্ক_ENTRIES];
    kccgst_সূচক_t অঙ্ক_kccgst;
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

কাঠামো rule {
    কাঠামো sval mlvo_মান_at_pos[MLVO_অঙ্ক_ENTRIES];
    গণনা mlvo_match_type match_type_at_pos[MLVO_অঙ্ক_ENTRIES];
    কাঠামো sval kccgst_মান_at_pos[KCCGST_অঙ্ক_ENTRIES];
    mlvo_সূচক_t অঙ্ক_mlvo_মানসমূহ;
    kccgst_সূচক_t অঙ্ক_kccgst_মানসমূহ;
    bool skip;
};

#define SLICE_KCCGST_BIT_FIELD_আকার 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_আকার - 1)) &
                (KCCGST_অঙ্ক_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
কাঠামো kccgst_বাফার_slice {
    uint32_t দৈর্ঘ্য:(32 - SLICE_KCCGST_BIT_FIELD_আকার);
    গণনা rules_kccgst kccgst:SLICE_KCCGST_BIT_FIELD_আকার;
    xkb_layout_সূচক_t layout;
};

/* Buffer for pending KcCGST values */
কাঠামো kccgst_বাফার {
    darray_char বাফার;
    /* Slice corresponding to each value in the buffer */
    darray(কাঠামো kccgst_বাফার_slice) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
কাঠামো matcher {
    কাঠামো xkb_context *ctx;
    /* Input.*/
    কাঠামো rule_names rmlvo;
    সংঘ lvalue val;
    darray(কাঠামো group) groups;
    /* Current mapping. */
    কাঠামো mapping mapping;
    /* Current rule. */
    কাঠামো rule rule;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    কাঠামো kccgst_বাফার pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_অঙ্ক_ENTRIES];
};

স্থির কাঠামো sval
strip_spaces(কাঠামো sval v)
{
    যখন (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    যখন (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    ফেরত v;
}

স্থির darray_matched_sval
split_comma_separated_mlvo(কাঠামো xkb_context *ctx,
                           গণনা rules_mlvo mlvo, ধ্রুবক অক্ষর *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    যদি (!s) {
        ensure_at_least_one_মান:
        /* Label followed by a declaration is a C23 extension */ ;

        কাঠামো matched_sval val = {
            .sval = SVAL(ফাঁকা, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        ফেরত arr;
    }

    যখন (true) {
        কাঠামো matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        যখন (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        যদি (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            ধ্রুবক অক্ষর* ধ্রুবক layout_start = s;
            xkb_layout_সূচক_t layout = XKB_LAYOUT_INVALID;

            পূর্ণসংখ্যা গুনতি = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            যদি (গুনতি > 0) {
                /* Note: 1-indexed layout */
                s += গুনতি;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                যদি (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক_2,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (unsigned পূর্ণসংখ্যা) val.sval.len, val.sval.start);
                } অন্যথা যদি (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (unsigned পূর্ণসংখ্যা) val.sval.len, val.sval.start);
                } অন্যথা {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ধ্রুবক অক্ষর* ধ্রুবক layout_সূচক_end = s;
            যখন (*s != '\0' && *s != ',') { s++; }
            যদি (গুনতি <= 0 || layout_সূচক_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক_2,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (unsigned পূর্ণসংখ্যা) (s - layout_start), layout_start,
                        (unsigned পূর্ণসংখ্যা) val.sval.len, val.sval.start);
            }
        }

        যদি (val.sval.len || mlvo != MLVO_OPTION) {
            যদি (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                কাঠামো matched_sval *পূর্ব;
                darray_foreach(পূর্ব, arr) {
                    যদি (svaleq(পূর্ব->sval, val.sval)) {
                        /* Merge options */
                        যদি (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            পূর্ব->layouts = val.layouts;
                        } অন্যথা যদি (পূর্ব->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            পূর্ব->layouts |= val.layouts;
                        }
                        যাও পরবর্তী_মান;
                    }
                }
            }
            darray_append(arr, val);
        }

পরবর্তী_মান:
        যদি (*s == '\0') বিরতি;
        যদি (*s == ',') s++;
    }

    যদি (darray_empty(arr))
        যাও ensure_at_least_one_মান;

    ফেরত arr;
}

স্থির কাঠামো matcher *
matcher_new_from_rmlvo(ধ্রুবক কাঠামো xkb_rmlvo_builder *rmlvo, ধ্রুবক অক্ষর **rules)
{
    কাঠামো matcher *m = calloc(1, আকার(*m));
    যদি (!m)
        ফেরত ফাঁকা;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    কাঠামো xkb_rule_names names = {
        .rules = rmlvo->rules,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_empty(rmlvo->layouts)) ? ফাঁকা : "x",
        .variant = (darray_empty(rmlvo->layouts)) ? ফাঁকা : "x",
        .options = (darray_empty(rmlvo->options)) ? ফাঁকা : "x",
    };
    ধ্রুবক গণনা RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
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
        যদি (darray_আকার(m->rmlvo.layouts) > darray_আকার(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            যদি (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_আকার(m->rmlvo.layouts));
        } অন্যথা যদি (darray_আকার(m->rmlvo.layouts) < darray_আকার(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_আকার(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } অন্যথা {
        কাঠামো xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            কাঠামো matched_sval val = {
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
        কাঠামো xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            কাঠামো matched_sval val = {
                .sval = SVAL(option->option, strlen_safe(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    ফেরত m;
}

স্থির কাঠামো matcher *
matcher_new_from_names(কাঠামো xkb_context *ctx,
            ধ্রুবক কাঠামো xkb_rule_names *rmlvo)
{
    কাঠামো matcher *m = calloc(1, আকার(*m));
    যদি (!m)
        ফেরত ফাঁকা;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    যদি (darray_আকার(m->rmlvo.layouts) > darray_আকার(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        যদি (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_আকার(m->rmlvo.layouts));
    } অন্যথা যদি (darray_আকার(m->rmlvo.layouts) < darray_আকার(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_আকার(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    ফেরত m;
}

স্থির শূন্য
matcher_free(কাঠামো matcher *m)
{
    যদি (!m)
        ফেরত;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    কাঠামো group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.বাফার);
    darray_free(m->pending_kccgst.slices);
    জন্য (kccgst_সূচক_t i = 0; i < (kccgst_সূচক_t) KCCGST_অঙ্ক_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    মুক্ত(m);
}

স্থির শূন্য
matcher_group_start_new(কাঠামো matcher *m, কাঠামো sval name)
{
    কাঠামো group group = { .name = name, .elements = darray_new() };
    darray_append(m->groups, group);
}

স্থির শূন্য
matcher_group_add_element(কাঠামো matcher *m, কাঠামো scanner *s,
                          কাঠামো sval element)
{
    darray_append(darray_item(m->groups, darray_আকার(m->groups) - 1).elements,
                  element);
}

স্থির bool
read_rules_file(কাঠামো xkb_context *ctx,
                কাঠামো matcher *matcher,
                unsigned পূর্ণসংখ্যা include_depth,
                FILE *file,
                ধ্রুবক অক্ষর *path);

স্থির শূন্য
matcher_include(কাঠামো matcher *m, কাঠামো scanner *parent_scanner,
                unsigned পূর্ণসংখ্যা include_depth,
                কাঠামো sval inc)
{
    যদি (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        ফেরত;
    }

    ধ্রুবক অক্ষর *stmt_file = inc.start;
    আকার_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    অক্ষর buf[PATH_MAX] = {0};
    ধ্রুবক ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, আকার(buf));
    যদি (expanded < 0) {
        /* Error */
        ফেরত;
    } অন্যথা যদি (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (আকার_t) expanded;
        প্রতিজ্ঞা(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned পূর্ণসংখ্যা offset = 0;
    ধ্রুবক bool absolute_path = is_absolute_path(stmt_file);
    যদি (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        যদি (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            যদি (stmt_file_len < আকার(buf)) {
                memcpy(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } অন্যথা {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, আকার(buf),
                        (unsigned পূর্ণসংখ্যা) stmt_file_len, stmt_file);
                ফেরত;
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
            file = ফাঁকা;
        } অন্যথা {
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, আকার(buf), &offset, true);
        }
    }

    যখন (file) {
        প্রতিজ্ঞা(strlen_safe(buf) < আকার(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        fclose(file);
        যদি (ret)
            ফেরত;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        যদি (absolute_path) {
            /* There is no point to search further if the path is absolute */
            বিরতি;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, আকার(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (unsigned পূর্ণসংখ্যা) stmt_file_len, stmt_file);
}

স্থির শূন্য
matcher_mapping_start_new(কাঠামো matcher *m)
{
    জন্য (mlvo_সূচক_t i = 0; i < (mlvo_সূচক_t) MLVO_অঙ্ক_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_অঙ্ক_ENTRIES;
    জন্য (kccgst_সূচক_t i = 0; i < (kccgst_সূচক_t) KCCGST_অঙ্ক_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_অঙ্ক_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.অঙ্ক_mlvo = m->mapping.অঙ্ক_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
স্থির পূর্ণসংখ্যা
parse_layout_int_সূচক_লেজ(ধ্রুবক অক্ষর *s, আকার_t max_len,
                            xkb_layout_সূচক_t *out)
{
    যদি (max_len < 3)
        ফেরত -1;
    uint32_t val = 0;
    ধ্রুবক পূর্ণসংখ্যা গুনতি = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    যদি (গুনতি <= 0 || s[1 + গুনতি] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        ফেরত -1;
    /* To zero-based index. */
    *out = val - 1;
    ফেরত গুনতি + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
স্থির পূর্ণসংখ্যা
extract_layout_সূচক(ধ্রুবক অক্ষর *s, আকার_t max_len, xkb_layout_সূচক_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    যদি (max_len < 3 || s[0] != '[')
        ফেরত -1;
    যদি (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_সূচক_t)(s[1] - '1');
        ফেরত 3;
    }
    যদি (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        ফেরত 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    ফেরত parse_layout_int_সূচক_লেজ(s, max_len, out);
}

/* Special layout indices */
গণনা layout_সূচক_ranges {
    LAYOUT_সূচক_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_সূচক_FIRST,
    LAYOUT_সূচক_LATER,
    LAYOUT_সূচক_MULTIPLE,
    LAYOUT_সূচক_ANY,
};

static_assert((xkb_layout_সূচক_t) XKB_MAX_GROUPS <
              (xkb_layout_সূচক_t) LAYOUT_সূচক_SINGLE,
              "Cannot define special indices");
static_assert((xkb_layout_সূচক_t) LAYOUT_সূচক_SINGLE <
              (xkb_layout_সূচক_t) LAYOUT_সূচক_FIRST &&
              (xkb_layout_সূচক_t) LAYOUT_সূচক_FIRST <
              (xkb_layout_সূচক_t) LAYOUT_সূচক_LATER &&
              (xkb_layout_সূচক_t) LAYOUT_সূচক_LATER <
              (xkb_layout_সূচক_t) LAYOUT_সূচক_MULTIPLE &&
              (xkb_layout_সূচক_t) LAYOUT_সূচক_MULTIPLE <
              (xkb_layout_সূচক_t) LAYOUT_সূচক_ANY &&
              (xkb_layout_সূচক_t) LAYOUT_সূচক_ANY <
              (xkb_layout_সূচক_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
স্থির bool
extract_mapping_layout_সূচক(ধ্রুবক অক্ষর *s, আকার_t max_len,
                             xkb_layout_সূচক_t *out)
{
    /* Check for minimal `[` + index + `]` */
    যদি (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        ফেরত false;
    }

    /* Small numeric index */
    যদি (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_সূচক_t)(s[1] - '1');
        ফেরত true;
    }

    /* Named indices ranges */
    স্থির ধ্রুবক কাঠামো {
        ধ্রুবক অক্ষর* name;
        আকার_t আকার_2;
        xkb_layout_সূচক_t মান;
    } names[] = {
        #define সূচক_RANGE(লেজ, মান) \
            { (লেজ), আকার(লেজ), (xkb_layout_সূচক_t)(মান) }
        সূচক_RANGE("single]",   LAYOUT_সূচক_SINGLE),
        সূচক_RANGE("first]",    LAYOUT_সূচক_FIRST),
        সূচক_RANGE("later]",    LAYOUT_সূচক_LATER),
        সূচক_RANGE("multiple]", LAYOUT_সূচক_MULTIPLE),
        সূচক_RANGE("any]",      LAYOUT_সূচক_ANY),
        #undef সূচক_RANGE
    };

    জন্য (আকার_t k = 0; k < ARRAY_আকার(names); k++) {
        যদি (max_len == names[k].আকার_2 &&
            memcmp(&s[1], names[k].name, names[k].আকার_2 - 1) == 0) {
            *out = names[k].মান;
            ফেরত true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    ধ্রুবক পূর্ণসংখ্যা গুনতি = parse_layout_int_সূচক_লেজ(s, max_len, out);
    ফেরত গুনতি > 0 && (আকার_t)গুনতি == max_len;
}

স্থির অন্তঃরেখ bool
is_mlvo_mask_defined(কাঠামো matcher *m, গণনা rules_mlvo mlvo)
{
    ফেরত m->mapping.defined_mlvo_mask & (1u << mlvo);
}

স্থির শূন্য
matcher_mapping_set_mlvo(কাঠামো matcher *m, কাঠামো scanner *s,
                         কাঠামো sval ident)
{
    গণনা rules_mlvo mlvo;
    কাঠামো sval mlvo_sval;

    জন্য (mlvo = 0; mlvo < MLVO_অঙ্ক_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        যদি (svaleq_prefix(mlvo_sval, ident))
            বিরতি;
    }

    /* Not found. */
    যদি (mlvo >= MLVO_অঙ্ক_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned পূর্ণসংখ্যা) ident.len, ident.start);
        m->mapping.active = false;
        ফেরত;
    }

    যদি (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned পূর্ণসংখ্যা) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        ফেরত;
    }

    /* If there are leftovers still, it must be an index. */
    যদি (mlvo_sval.len < ident.len) {
        xkb_layout_সূচক_t idx;
        যদি (!extract_mapping_layout_সূচক(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (unsigned পূর্ণসংখ্যা) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            ফেরত;
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
                        (unsigned পূর্ণসংখ্যা) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            ফেরত;
        }
    } অন্যথা যদি (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_সূচক_t) LAYOUT_সূচক_SINGLE;
    } অন্যথা যদি (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_সূচক_t) LAYOUT_সূচক_SINGLE;
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
        ফেরত;
    }

    m->mapping.mlvo_at_pos[m->mapping.অঙ্ক_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.অঙ্ক_mlvo++;
}

স্থির শূন্য
matcher_mapping_set_layout_bounds(কাঠামো matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_সূচক_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    নির্বাচন (idx) {
        ক্ষেত্র XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            প্রতিজ্ঞা(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            বিরতি;
        ক্ষেত্র LAYOUT_সূচক_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_আকার(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            বিরতি;
        ক্ষেত্র LAYOUT_সূচক_MULTIPLE:
        ক্ষেত্র LAYOUT_সূচক_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_আকার(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            বিরতি;
        ক্ষেত্র LAYOUT_সূচক_SINGLE:
        ক্ষেত্র LAYOUT_সূচক_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        পূর্বনির্ধারিত:
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
matcher_mapping_set_kccgst(কাঠামো matcher *m, কাঠামো scanner *s, কাঠামো sval ident)
{
    গণনা rules_kccgst kccgst;
    কাঠামো sval kccgst_sval;

    জন্য (kccgst = 0; kccgst < KCCGST_অঙ্ক_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        যদি (svaleq(rules_kccgst_svals[kccgst], ident))
            বিরতি;
    }

    /* Not found. */
    যদি (kccgst >= KCCGST_অঙ্ক_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned পূর্ণসংখ্যা) ident.len, ident.start);
        m->mapping.active = false;
        ফেরত;
    }

    যদি (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned পূর্ণসংখ্যা) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        ফেরত;
    }

    m->mapping.kccgst_at_pos[m->mapping.অঙ্ক_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.অঙ্ক_kccgst++;
}

স্থির bool
matcher_mapping_verify(কাঠামো matcher *m, কাঠামো scanner *s)
{
    যদি (m->mapping.অঙ্ক_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        যাও skip;
    }

    যদি (m->mapping.অঙ্ক_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        যাও skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    যদি (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        প্রতিজ্ঞা(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        নির্বাচন (m->mapping.layout_idx) {
            ক্ষেত্র LAYOUT_সূচক_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                যদি (darray_আকার(m->rmlvo.layouts) > 1)
                    যাও skip;
                বিরতি;
            ক্ষেত্র LAYOUT_সূচক_MULTIPLE:
            ক্ষেত্র LAYOUT_সূচক_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                যদি (darray_আকার(m->rmlvo.layouts) < 2)
                    যাও skip;
                বিরতি;
            ক্ষেত্র LAYOUT_সূচক_ANY:
            ক্ষেত্র LAYOUT_সূচক_FIRST:
                /* No restrictions */
                বিরতি;
            পূর্বনির্ধারিত:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                যদি (darray_আকার(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_আকার(m->rmlvo.layouts))
                    যাও skip;
        }
    }

    যদি (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        প্রতিজ্ঞা(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        নির্বাচন (m->mapping.variant_idx) {
            ক্ষেত্র LAYOUT_সূচক_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                যদি (darray_আকার(m->rmlvo.variants) > 1)
                    যাও skip;
                বিরতি;
            ক্ষেত্র LAYOUT_সূচক_ANY:
            ক্ষেত্র LAYOUT_সূচক_LATER:
            ক্ষেত্র LAYOUT_সূচক_FIRST:
                /* No restriction */
                বিরতি;
            পূর্বনির্ধারিত:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                যদি (darray_আকার(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_আকার(m->rmlvo.variants))
                    যাও skip;
        }
    }

    ফেরত true;

skip:
    m->mapping.active = false;
    ফেরত false;
}

স্থির শূন্য
matcher_rule_start_new(কাঠামো matcher *m)
{
    memset(&m->rule, 0, আকার(m->rule));
    m->rule.skip = !m->mapping.active;
}

স্থির শূন্য
matcher_rule_set_mlvo_common(কাঠামো matcher *m, কাঠামো scanner *s,
                             কাঠামো sval ident,
                             গণনা mlvo_match_type match_type)
{
    যদি (m->rule.অঙ্ক_mlvo_মানসমূহ >= m->mapping.অঙ্ক_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        ফেরত;
    }
    m->rule.match_type_at_pos[m->rule.অঙ্ক_mlvo_মানসমূহ] = match_type;
    m->rule.mlvo_মান_at_pos[m->rule.অঙ্ক_mlvo_মানসমূহ] = ident;
    m->rule.অঙ্ক_mlvo_মানসমূহ++;
}

স্থির শূন্য
matcher_rule_set_mlvo_wildcard(কাঠামো matcher *m, কাঠামো scanner *s,
                               গণনা mlvo_match_type match_type)
{
    কাঠামো sval dummy = SVAL(ফাঁকা, 0);
    matcher_rule_set_mlvo_common(m, s, dummy, match_type);
}

স্থির শূন্য
matcher_rule_set_mlvo_group(কাঠামো matcher *m, কাঠামো scanner *s,
                            কাঠামো sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_GROUP);
}

স্থির শূন্য
matcher_rule_set_mlvo(কাঠামো matcher *m, কাঠামো scanner *s,
                      কাঠামো sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_NORMAL);
}

স্থির শূন্য
matcher_rule_set_kccgst(কাঠামো matcher *m, কাঠামো scanner *s,
                        কাঠামো sval ident)
{
    যদি (m->rule.অঙ্ক_kccgst_মানসমূহ >= m->mapping.অঙ্ক_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        ফেরত;
    }
    m->rule.kccgst_মান_at_pos[m->rule.অঙ্ক_kccgst_মানসমূহ] = ident;
    m->rule.অঙ্ক_kccgst_মানসমূহ++;
}

স্থির bool
match_group(কাঠামো matcher *m, কাঠামো sval group_name, কাঠামো sval to)
{
    কাঠামো group *group = ফাঁকা;
    কাঠামো sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        যদি (svaleq(group->name, group_name)) {
            found = true;
            বিরতি;
        }
    }

    যদি (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        ফেরত false;
    }

    darray_foreach(element, group->elements)
        যদি (svaleq(to, *element))
            ফেরত true;

    ফেরত false;
}

স্থির bool
match_মান(কাঠামো matcher *m, ধ্রুবক কাঠামো sval val, ধ্রুবক কাঠামো sval to,
            গণনা mlvo_match_type match_type,
            গণনা wildcard_match_type wildcard_type)
{
    নির্বাচন (match_type) {
        ক্ষেত্র MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            ফেরত wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        ক্ষেত্র MLVO_MATCH_WILDCARD_NONE:
            ফেরত !to.len;
        ক্ষেত্র MLVO_MATCH_WILDCARD_SOME:
            ফেরত !!to.len;
        ক্ষেত্র MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            ফেরত true;
        ক্ষেত্র MLVO_MATCH_GROUP:
            ফেরত match_group(m, val, to);
        পূর্বনির্ধারিত:
            প্রতিজ্ঞা(match_type == MLVO_MATCH_NORMAL);
            ফেরত svaleq(val, to);
    }
}

স্থির bool
match_মান_and_mark(কাঠামো matcher *m, ধ্রুবক কাঠামো sval val,
                     কাঠামো matched_sval *to, গণনা mlvo_match_type match_type,
                     গণনা wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_মান(m, val, to->sval, match_type, wildcard_type);
    যদি (matched)
        to->matched |= layouts;
    ফেরত matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
স্থির bool
expand_rmlvo_in_kccgst_মান(কাঠামো matcher *m, কাঠামো scanner *s,
                             কাঠামো sval মান, xkb_layout_সূচক_t layout_idx,
                             darray_char *expanded, আকার_t *i)
{
    ধ্রুবক অক্ষর *str = মান.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    গণনা rules_mlvo mlv;
    xkb_layout_সূচক_t idx;
    অক্ষর pfx, sfx;
    কাঠামো matched_sval *expanded_মান;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    যদি (str[*i] == 'i' &&
        (*i + 1 == মান.len || is_merge_mode_prefix(str[*i + 1])))
    {
        যদি (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_সূচক_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            যাও error;
        }
        (*i)++;
        অক্ষর সূচক_str[MAX_LAYOUT_সূচক_STR_দৈর্ঘ্য + 1];
        ধ্রুবক পূর্ণসংখ্যা গুনতি = snprintf(সূচক_str, আকার(সূচক_str), "%"PRIu32,
                                   layout_idx + 1);
        প্রতিজ্ঞা(গুনতি > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, সূচক_str, (darray_আকার_t)গুনতি);
        ফেরত true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    যদি (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        যদি (str[*i] == '(') sfx = ')';
        যদি (++(*i) >= মান.len) যাও error;
    }

    /* Mandatory model/layout/variant specifier. */
    নির্বাচন (str[(*i)++]) {
    ক্ষেত্র 'm': mlv = MLVO_MODEL; বিরতি;
    ক্ষেত্র 'l': mlv = MLVO_LAYOUT; বিরতি;
    ক্ষেত্র 'v': mlv = MLVO_VARIANT; বিরতি;
    পূর্বনির্ধারিত: যাও error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_সূচক = false;
    যদি (*i < মান.len && str[*i] == '[') {
        যদি (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            যাও error;
        }

        পূর্ণসংখ্যা consumed = extract_layout_সূচক(str + (*i), মান.len - (*i), &idx);
        যদি (consumed <= 0) যাও error;
        যদি (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            প্রতিজ্ঞা(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_সূচক = true;
        }
        *i += (আকার_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    যদি (sfx != 0) {
        যদি (*i >= মান.len) যাও error;
        যদি (str[(*i)++] != sfx) যাও error;
    }

    /* Get the expanded value. */
    expanded_মান = ফাঁকা;

    যদি (mlv == MLVO_LAYOUT) {
        যদি (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            যদি (darray_আকার(m->rmlvo.layouts) == 1)
                expanded_মান = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } অন্যথা যদি (idx < darray_আকার(m->rmlvo.layouts) &&
                   (variable_সূচক || darray_আকার(m->rmlvo.layouts) > 1)) {
                expanded_মান = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    অন্যথা যদি (mlv == MLVO_VARIANT) {
        যদি (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            যদি (darray_আকার(m->rmlvo.variants) == 1)
                expanded_মান = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } অন্যথা যদি (idx < darray_আকার(m->rmlvo.variants) &&
                   (variable_সূচক || darray_আকার(m->rmlvo.variants) > 1)) {
                expanded_মান = &darray_item(m->rmlvo.variants, idx);
        }
    }
    অন্যথা যদি (mlv == MLVO_MODEL) {
        expanded_মান = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    যদি (!expanded_মান || expanded_মান->sval.len == 0) {
        ফেরত true;
    }

    যদি (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_মান->sval.start,
                                 (darray_আকার_t) expanded_মান->sval.len);
    যদি (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    যদি (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ধ্রুবক xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_মান->matched |= layouts;
    } অন্যথা {
        expanded_মান->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    ফেরত true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    ফেরত false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
স্থির শূন্য
expand_qualifier_in_kccgst_মান(
    কাঠামো matcher *m, কাঠামো scanner *s,
    কাঠামো sval মান, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_আকার_t prefix_idx, আকার_t *i)
{
    ধ্রুবক অক্ষর *str = মান.start;

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
        যদি (darray_আকার(m->rmlvo.layouts) > 1) {
            অক্ষর layout_সূচক[MAX_LAYOUT_সূচক_STR_দৈর্ঘ্য + 1];
            ধ্রুবক darray_আকার_t prefix_দৈর্ঘ্য =
                darray_আকার(*expanded) - prefix_idx - 1;
            জন্য (xkb_layout_সূচক_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_আকার(m->rmlvo.layouts));
                 l++)
            {
                যদি (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_দৈর্ঘ্য);
                /* Append index */
                ধ্রুবক পূর্ণসংখ্যা গুনতি = snprintf(layout_সূচক, আকার(layout_সূচক),
                                           "%"PRIu32, l + 1);
                প্রতিজ্ঞা(গুনতি > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_সূচক,
                                             (darray_আকার_t)গুনতি);
            }
        }
        *i += 3;
    }
}

স্থির অন্তঃরেখ শূন্য
#ifdef _MSC_VER
concat_kccgst(darray_char *into, darray_আকার_t আকার_2, _In_reads_(আকার_2) ধ্রুবক অক্ষর* from)
#অন্যথা
concat_kccgst(darray_char *into, darray_আকার_t আকার_2, ধ্রুবক অক্ষর from[স্থির আকার_2])
#endif
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ধ্রুবক bool from_plus = is_merge_mode_prefix(from[0]);
    যদি (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, আকার_2);
    } অন্যথা {
        ধ্রুবক অক্ষর ch =
            (অক্ষর) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        ধ্রুবক bool into_plus = is_merge_mode_prefix(ch);
        যদি (into_plus)
            darray_prepends_nullterminate(*into, from, আকার_2);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
স্থির bool
append_expanded_kccgst_মান(কাঠামো matcher *m, কাঠামো scanner *s,
                             bool merge, darray_char *to, কাঠামো sval মান,
                             xkb_layout_সূচক_t layout_idx)
{
    ধ্রুবক অক্ষর *str = মান.start;
    darray_char expanded = darray_new();
    darray_আকার_t last_item_idx = 0;
    bool has_separator = false;

    জন্য (আকার_t i = 0; i < মান.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        নির্বাচন (str[i]) {
            /* Qualifier */
            ক্ষেত্র ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_মান(m, s, মান, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                বিরতি;
            /* Expansion */
            ক্ষেত্র '%':
                i++;
                যদি (i >= মান.len ||
                    !expand_rmlvo_in_kccgst_মান(m, s, মান, layout_idx,
                                                  &expanded, &i))
                        যাও error;
                বিরতি;
            /* New item */
            ক্ষেত্র MERGE_OVERRIDE_PREFIX:
            ক্ষেত্র MERGE_AUGMENT_PREFIX:
            ক্ষেত্র MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_আকার(expanded) - 1;
                has_separator = true;
                বিরতি;
            /* Just a normal character. */
            পূর্বনির্ধারিত:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    যদি (merge) {
        যদি (!darray_empty(expanded))
            concat_kccgst(to, darray_আকার(expanded), darray_বস্তুসমূহ(expanded));
    } অন্যথা {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    ফেরত true;

error:
    darray_free(expanded);
    ফেরত false;
}

স্থির bool
matcher_append_pending_kccgst(কাঠামো matcher *m)
{
    যদি (!m->mapping.has_multiple_layouts)
        ফেরত true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    জন্য (kccgst_সূচক_t i = 0; i < m->mapping.অঙ্ক_kccgst; i++) {
        ধ্রুবক গণনা rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        জন্য (xkb_layout_সূচক_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            নিবন্ধ ধ্রুবক কাঠামো kccgst_বাফার* ধ্রুবক buf = &m->pending_kccgst;
            আকার_t offset = 0;
            জন্য (darray_আকার_t k = 0; k < darray_আকার(buf->slices); k++) {
                নিবন্ধ ধ্রুবক কাঠামো kccgst_বাফার_slice * ধ্রুবক slice =
                    &darray_item(buf->slices, k);
                যদি (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->দৈর্ঘ্য)
                    concat_kccgst(&m->kccgst[kccgst], slice->দৈর্ঘ্য,
                                  darray_বস্তুসমূহ(buf->বাফার) + offset);
                offset += slice->দৈর্ঘ্য;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    ফেরত true;
}

স্থির শূন্য
matcher_rule_verify(কাঠামো matcher *m, কাঠামো scanner *s)
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
matcher_rule_match_model(কাঠামো matcher *m, ধ্রুবক কাঠামো sval মান,
                         গণনা mlvo_match_type match_type)
{
    ফেরত match_মান_and_mark(
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
matcher_rule_match_option(কাঠামো matcher *m, ধ্রুবক কাঠামো sval মান,
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
    কাঠামো matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        ধ্রুবক xkb_layout_mask_t matchable = option->layouts
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
                বিরতি;
        }
    }
    যদি (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    ফেরত matched;
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
matcher_rule_match_layout_or_variant(কাঠামো matcher *m,
                                     darray_matched_sval *প্রবেশ,
                                     ধ্রুবক কাঠামো sval মান,
                                     গণনা mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    প্রতিজ্ঞা(!darray_empty(*প্রবেশ));

    bool matched = false;
    /* Loop over the layout index range */
    জন্য (xkb_layout_সূচক_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ধ্রুবক xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        যদি (layout & *candidates) {
            কাঠামো matched_sval *to = &darray_item(*প্রবেশ, idx);
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
    ফেরত matched;
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
matcher_rule_apply_if_matches(কাঠামো matcher *m, কাঠামো scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_mask_t candidate_layouts = m->mapping.layouts_candidates_mask;

    /* Loop over MLVO pattern components */
    জন্য (mlvo_সূচক_t i = 0; i < m->mapping.অঙ্ক_mlvo; i++) {
        ধ্রুবক গণনা rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ধ্রুবক কাঠামো sval মান = m->rule.mlvo_মান_at_pos[i];
        ধ্রুবক গণনা mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        নির্বাচন (mlvo) {
        ক্ষেত্র MLVO_MODEL:
            matched = matcher_rule_match_model(m, মান, match_type);
            বিরতি;
        ক্ষেত্র MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, মান, match_type, &candidate_layouts
            );
            বিরতি;
        ক্ষেত্র MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, মান, match_type, &candidate_layouts
            );
            বিরতি;
        ক্ষেত্র MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, মান, match_type, &candidate_layouts
            );
            বিরতি;
        পূর্বনির্ধারিত: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_অঙ্ক_ENTRIES - 1,
                          "Unexpected MLVO field");
            প্রতিজ্ঞা(!"Unreachable");
        }
        }

        যদি (!matched)
            ফেরত;
    }

    যদি (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        জন্য (xkb_layout_সূচক_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            যদি (candidate_layouts & (UINT32_C(1) << idx)) {
                জন্য (kccgst_সূচক_t i = 0; i < m->mapping.অঙ্ক_kccgst; i++) {
                    ধ্রুবক গণনা rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ধ্রুবক কাঠামো sval মান = m->rule.kccgst_মান_at_pos[i];
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
                    কাঠামো kccgst_বাফার * ধ্রুবক buf = &m->pending_kccgst;
                    ধ্রুবক darray_আকার_t পূর্ব_বাফার_দৈর্ঘ্য =
                        darray_আকার(buf->বাফার);
                    append_expanded_kccgst_মান(m, s, false, &buf->বাফার,
                                                 মান, idx);
                    ধ্রুবক uint32_t দৈর্ঘ্য = (uint32_t) (darray_আকার(buf->বাফার)
                                          - পূর্ব_বাফার_দৈর্ঘ্য);
                    ধ্রুবক কাঠামো kccgst_বাফার_slice slice = {
                        .দৈর্ঘ্য = দৈর্ঘ্য,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } অন্যথা {
        /* Numeric index or no index */
        জন্য (kccgst_সূচক_t i = 0; i < m->mapping.অঙ্ক_kccgst; i++) {
            গণনা rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            কাঠামো sval মান = m->rule.kccgst_মান_at_pos[i];
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
gettok(কাঠামো matcher *m, কাঠামো scanner *s)
{
    ফেরত lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
স্থির bool
matcher_match(কাঠামো matcher *m, কাঠামো scanner *s,
              unsigned পূর্ণসংখ্যা include_depth,
              ধ্রুবক অক্ষর *string, আকার_t len,
              ধ্রুবক অক্ষর *file_name)
{
    গণনা rules_token tok;

    যদি (!m)
        ফেরত false;

initial:
    নির্বাচন (tok = gettok(m, s)) {
    ক্ষেত্র TOK_BANG:
        যাও bang;
    ক্ষেত্র TOK_END_OF_সারি:
        যাও initial;
    ক্ষেত্র TOK_END_OF_FILE:
        যাও finish;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

bang:
    নির্বাচন (tok = gettok(m, s)) {
    ক্ষেত্র TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        যাও group_name;
    ক্ষেত্র TOK_INCLUDE:
        যাও include_statement;
    ক্ষেত্র TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        যাও mapping_mlvo;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

group_name:
    নির্বাচন (tok = gettok(m, s)) {
    ক্ষেত্র TOK_EQUALS:
        যাও group_element;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

group_element:
    নির্বাচন (tok = gettok(m, s)) {
    ক্ষেত্র TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        যাও group_element;
    ক্ষেত্র TOK_END_OF_সারি:
        যাও initial;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

include_statement:
    নির্বাচন (tok = gettok(m, s)) {
    ক্ষেত্র TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        যাও include_statement_end;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

include_statement_end:
    নির্বাচন (tok = gettok(m, s)) {
    ক্ষেত্র TOK_END_OF_সারি:
        যাও initial;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

mapping_mlvo:
    নির্বাচন (tok = gettok(m, s)) {
    ক্ষেত্র TOK_IDENTIFIER:
        যদি (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        যাও mapping_mlvo;
    ক্ষেত্র TOK_EQUALS:
        যাও mapping_kccgst;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

mapping_kccgst:
    নির্বাচন (tok = gettok(m, s)) {
    ক্ষেত্র TOK_IDENTIFIER:
        যদি (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        যাও mapping_kccgst;
    ক্ষেত্র TOK_END_OF_সারি:
        যদি (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            যদি (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_আকার(m->pending_kccgst.বাফার) = 0;
                darray_আকার(m->pending_kccgst.slices) = 0;
            }
        }
        যাও rule_mlvo_first;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

rule_mlvo_first:
    নির্বাচন (tok = gettok(m, s)) {
    ক্ষেত্র TOK_BANG:
        matcher_append_pending_kccgst(m);
        যাও bang;
    ক্ষেত্র TOK_END_OF_সারি:
        যাও rule_mlvo_first;
    ক্ষেত্র TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        যাও finish;
    পূর্বনির্ধারিত:
        matcher_rule_start_new(m);
        যাও rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    নির্বাচন (tok) {
    ক্ষেত্র TOK_IDENTIFIER:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        যাও rule_mlvo;
    ক্ষেত্র TOK_WILD_CARD_STAR:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        যাও rule_mlvo;
    ক্ষেত্র TOK_WILD_CARD_NONE:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        যাও rule_mlvo;
    ক্ষেত্র TOK_WILD_CARD_SOME:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        যাও rule_mlvo;
    ক্ষেত্র TOK_WILD_CARD_ANY:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        যাও rule_mlvo;
    ক্ষেত্র TOK_GROUP_NAME:
        যদি (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        যাও rule_mlvo;
    ক্ষেত্র TOK_EQUALS:
        যাও rule_kccgst;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

rule_kccgst:
    নির্বাচন (tok = gettok(m, s)) {
    ক্ষেত্র TOK_IDENTIFIER:
        যদি (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        যাও rule_kccgst;
    ক্ষেত্র TOK_END_OF_সারি:
        যদি (!m->rule.skip)
            matcher_rule_verify(m, s);
        যদি (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        যাও rule_mlvo_first;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

unexpected:
    নির্বাচন (tok) {
    ক্ষেত্র TOK_ERROR:
        যাও error;
    পূর্বনির্ধারিত:
        যাও state_error;
    }

finish:
    ফেরত true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    ফেরত false;
}
// NOLINTEND(readability-trivial-switch)

স্থির bool
read_rules_file(কাঠামো xkb_context *ctx,
                কাঠামো matcher *matcher,
                unsigned পূর্ণসংখ্যা include_depth,
                FILE *file,
                ধ্রুবক অক্ষর *path)
{
    bool ret;
    অক্ষর *string;
    আকার_t আকার_2;
    কাঠামো scanner scanner;

    যদি (!map_file(file, &string, &আকার_2)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, strerror(errno));
        ফেরত false;
    }

    scanner_init(&scanner, matcher->ctx, string, আকার_2, path, ফাঁকা);

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
        unmap_file(string, আকার_2);
        ফেরত false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, আকার_2, path);
    unmap_file(string, আকার_2);
    ফেরত ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
স্থির bool
xkb_resolve_partial_rules(কাঠামো xkb_context *ctx, অক্ষর *path, আকার_t path_আকার,
                          ধ্রুবক অক্ষর* rules, ধ্রুবক অক্ষর* suffix,
                          কাঠামো matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    অক্ষর partial_rules[60]; /* Arbitrary, but we do not expect long names */
    যদি (unlikely(!snprintf_safe(partial_rules, আকার(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        ফেরত false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    unsigned পূর্ণসংখ্যা offset = 0;
    FILE *file = ফাঁকা;
    ধ্রুবক আকার_t len = strlen(partial_rules);
    যখন ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_আকার, &offset, false)) != ফাঁকা) {
        ধ্রুবক bool ok = read_rules_file(ctx, matcher, 0, file, path);
        fclose(file);
        যদি (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            ফেরত false;
        }
        offset++;
    }

    ফেরত true;
}

স্থির bool
xkb_resolve_rules(কাঠামো xkb_context *ctx,
                  ধ্রুবক অক্ষর* rules, কাঠামো matcher *matcher,
                  কাঠামো xkb_component_names *out,
                  xkb_layout_সূচক_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    unsigned পূর্ণসংখ্যা offset = 0;
    অক্ষর path[PATH_MAX];
    FILE * ধ্রুবক file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, strlen(rules), FILE_TYPE_RULES,
                                          path, আকার(path), &offset, true);
    যদি (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        যাও err_out;
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
    ret = xkb_resolve_partial_rules(ctx, path, আকার(path),
                                    rules, ".pre", matcher);
    যদি (!ret)
        যাও err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    যদি (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        যাও err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, আকার(path),
                                    rules, ".post", matcher);
    যদি (!ret)
        যাও err_out;

    /* Check we got required components */
    যদি (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        যাও err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, ফাঁকা);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, ফাঁকা);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, ফাঁকা);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, ফাঁকা);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, ফাঁকা);

    কাঠামো matched_sval *mval = &matcher->rmlvo.model;
    যদি (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_মান,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (unsigned পূর্ণসংখ্যা) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        যদি (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_মান,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned পূর্ণসংখ্যা) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        যদি (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_মান,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned পূর্ণসংখ্যা) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        যদি (!mval->sval.len)
            চালু;

        যদি (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_মান,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned পূর্ণসংখ্যা) mval->sval.len, mval->sval.start, rules);
        } অন্যথা যদি (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ধ্রুবক xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            প্রতিজ্ঞা(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_মান,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (unsigned পূর্ণসংখ্যা)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    যদি (out->symbols != ফাঁকা && explicit_layouts != ফাঁকা) {
        *explicit_layouts = 1; /* at least one group */
        ধ্রুবক অক্ষর *symbols = out->symbols;
        /* Take the highest modifier */
        যখন ((symbols = strchr(symbols, ':')) != ফাঁকা && symbols[1] != '\0') {
            xkb_layout_সূচক_t group = 0;
            ধ্রুবক পূর্ণসংখ্যা গুনতি = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
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

    ফেরত ret;
}

bool
xkb_components_from_rmlvo_builder(ধ্রুবক কাঠামো xkb_rmlvo_builder *rmlvo,
                                  কাঠামো xkb_component_names *out,
                                  xkb_layout_সূচক_t *explicit_layouts)
{
    ধ্রুবক অক্ষর *rules = rmlvo->rules;
    কাঠামো matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    যদি (!matcher)
        ফেরত false;

    ধ্রুবক bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    ফেরত ret;
}


bool
xkb_components_from_rules_names(কাঠামো xkb_context *ctx,
                                ধ্রুবক কাঠামো xkb_rule_names *rmlvo,
                                কাঠামো xkb_component_names *out,
                                xkb_layout_সূচক_t *explicit_layouts)
{
    কাঠামো matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    যদি (!matcher)
        ফেরত false;

    ধ্রুবক bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    ফেরত ret;
}
