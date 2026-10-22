/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <limits.h>
#شامل <stdint.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcomp-priv.h"
#شامل "context.h"
#شامل "keymap.h"
#شامل "messages-codes.h"
#شامل "rules.h"
#شامل "rmlvo.h"
#شامل "include.h"
#شامل "scanner-utils.h"
#شامل "darray.h"
#شامل "utils.h"
#شامل "util-numbers.h"
#شامل "util-paths.h"

#define MAX_INCLUDE_DEPTH 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
اجتماع lvalue {
    ساختار sval string;
};

شمارش rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_سطر,
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

ایستا شمارش rules_token
lex(ساختار scanner *s, اجتماع lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    وقتی (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    اگر (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    اگر (scanner_eol(s)) {
        وقتی (scanner_eol(s)) scanner_next(s);
        بازگشت TOK_END_OF_سطر;
    }

    /* Escaped line continuation */
    اگر (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        اگر (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            بازگشت TOK_ERROR;
        }
        scanner_next(s);
        برو skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    اگر (scanner_eof(s)) بازگشت TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    شمارش rules_token token = TOK_IDENTIFIER;

    ثابت نویسه next = scanner_next(s);
    انتخاب (next) {
    حالت '!':
        بازگشت TOK_BANG;
    حالت '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        بشکن;
    حالت '*':
        اگر (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            بشکن;
        } وگرنه {
            /* Legacy wild card */
            بازگشت TOK_WILD_CARD_STAR;
        }
    حالت '<':
        /* Extended wild cards */
        اگر (scanner_rules_lit_token(s, "none>")) بازگشت TOK_WILD_CARD_NONE;
        اگر (scanner_rules_lit_token(s, "some>")) بازگشت TOK_WILD_CARD_SOME;
        اگر (scanner_rules_lit_token(s, "any>")) بازگشت TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        أكد(scanner_rules_is_ident('<'));
        val->string.len = 1;
        بشکن;
    حالت '=':
        اگر (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            بشکن;
        } وگرنه {
            بازگشت TOK_EQUALS;
        }
    حالت 'i':
        اگر (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            بازگشت TOK_INCLUDE;
        }
        /* Identifier */
        أكد(scanner_rules_is_ident('i'));
        val->string.len = 1;
        بشکن;
    پیشفرض:
        اگر (likely(scanner_rules_is_ident(next))) {
            /* Identifier */
            val->string.len = 1;
        } وگرنه {
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

    اگر (unlikely(val->string.len == 0)) {
        اگر (token == TOK_GROUP_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } وگرنه {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unrecognized token");
        }
        بازگشت TOK_ERROR;
    }

    بازگشت token;
}

/***====================================================================***/

شمارش rules_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_رقم_ENTRIES
};

شمارش {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

تعریفنوع uint8_t mlvo_index_t;
تعریفنوع uint8_t mlvo_mask_t;


ایستا ثابت ساختار sval rules_mlvo_svals[MLVO_رقم_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

شمارش rules_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_رقم_ENTRIES
};

تعریفنوع uint8_t kccgst_index_t;
تعریفنوع uint8_t kccgst_mask_t;

ایستا ثابت ساختار sval rules_kccgst_svals[KCCGST_رقم_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

شمارش {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL ==
              (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
ساختار matched_sval {
    ساختار sval sval;
    /** Used for layout-specific options */
    xkb_layout_mask_t layouts;
    xkb_layout_mask_t matched;
};
تعریفنوع darray(ساختار matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
ساختار rule_names {
    ساختار matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

ساختار group {
    ساختار sval name;
    darray_sval elements;
};

ساختار mapping {
    شمارش rules_mlvo mlvo_at_pos[MLVO_رقم_ENTRIES];
    mlvo_index_t رقم_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    اجتماع {
        ساختار { xkb_layout_index_t layout_idx, variant_idx; };
        ساختار { xkb_layout_index_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    اجتماع {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    شمارش rules_kccgst kccgst_at_pos[KCCGST_رقم_ENTRIES];
    kccgst_index_t رقم_kccgst;
    kccgst_mask_t defined_kccgst_mask;
};

شمارش mlvo_match_type {
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

شمارش wildcard_match_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_MATCH_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_MATCH_ALL,
};

ساختار rule {
    ساختار sval mlvo_مقدار_at_pos[MLVO_رقم_ENTRIES];
    شمارش mlvo_match_type match_type_at_pos[MLVO_رقم_ENTRIES];
    ساختار sval kccgst_مقدار_at_pos[KCCGST_رقم_ENTRIES];
    mlvo_index_t رقم_mlvo_مقادیر;
    kccgst_index_t رقم_kccgst_مقادیر;
    bool skip;
};

#define SLICE_KCCGST_BIT_FIELD_SIZE 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_SIZE - 1)) &
                (KCCGST_رقم_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
ساختار kccgst_buffer_slice {
    uint32_t length:(32 - SLICE_KCCGST_BIT_FIELD_SIZE);
    شمارش rules_kccgst kccgst:SLICE_KCCGST_BIT_FIELD_SIZE;
    xkb_layout_index_t layout;
};

/* Buffer for pending KcCGST values */
ساختار kccgst_buffer {
    darray_char buffer;
    /* Slice corresponding to each value in the buffer */
    darray(ساختار kccgst_buffer_slice) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
ساختار matcher {
    ساختار xkb_context *ctx;
    /* Input.*/
    ساختار rule_names rmlvo;
    اجتماع lvalue val;
    darray(ساختار group) groups;
    /* Current mapping. */
    ساختار mapping mapping;
    /* Current rule. */
    ساختار rule rule;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    ساختار kccgst_buffer pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_رقم_ENTRIES];
};

ایستا ساختار sval
strip_spaces(ساختار sval v)
{
    وقتی (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    وقتی (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    بازگشت v;
}

ایستا darray_matched_sval
split_comma_separated_mlvo(ساختار xkb_context *ctx,
                           شمارش rules_mlvo mlvo, ثابت نویسه *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    اگر (!s) {
        ensure_at_least_one_مقدار:
        /* Label followed by a declaration is a C23 extension */ ;

        ساختار matched_sval val = {
            .sval = SVAL(پوچ, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        بازگشت arr;
    }

    وقتی (true) {
        ساختار matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        وقتی (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        اگر (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            ثابت نویسه* ثابت layout_start = s;
            xkb_layout_index_t layout = XKB_LAYOUT_INVALID;

            صحیح شمار = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            اگر (شمار > 0) {
                /* Note: 1-indexed layout */
                s += شمار;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                اگر (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (unsigned صحیح) val.sval.len, val.sval.start);
                } وگرنه اگر (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (unsigned صحیح) val.sval.len, val.sval.start);
                } وگرنه {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ثابت نویسه* ثابت layout_index_end = s;
            وقتی (*s != '\0' && *s != ',') { s++; }
            اگر (شمار <= 0 || layout_index_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (unsigned صحیح) (s - layout_start), layout_start,
                        (unsigned صحیح) val.sval.len, val.sval.start);
            }
        }

        اگر (val.sval.len || mlvo != MLVO_OPTION) {
            اگر (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                ساختار matched_sval *prev;
                darray_foreach(prev, arr) {
                    اگر (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        اگر (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } وگرنه اگر (prev->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        برو next_مقدار;
                    }
                }
            }
            darray_append(arr, val);
        }

next_مقدار:
        اگر (*s == '\0') بشکن;
        اگر (*s == ',') s++;
    }

    اگر (darray_empty(arr))
        برو ensure_at_least_one_مقدار;

    بازگشت arr;
}

ایستا ساختار matcher *
matcher_new_from_rmlvo(ثابت ساختار xkb_rmlvo_builder *rmlvo, ثابت نویسه **rules)
{
    ساختار matcher *m = calloc(1, اندازه(*m));
    اگر (!m)
        بازگشت پوچ;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    ساختار xkb_rule_names names = {
        .rules = rmlvo->rules,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_empty(rmlvo->layouts)) ? پوچ : "x",
        .variant = (darray_empty(rmlvo->layouts)) ? پوچ : "x",
        .options = (darray_empty(rmlvo->options)) ? پوچ : "x",
    };
    ثابت شمارش RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
                                                               &names);

    /* Initialize matcher with sanitized names, if relevant */
    اگر (changed & RMLVO_RULES) {
        *rules = names.rules;
    } وگرنه {
        *rules = rmlvo->rules;
    }

    اگر (changed & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = names.model;
    } وگرنه {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;

    أكد((changed & RMLVO_LAYOUT) || !(changed & RMLVO_VARIANT));
    اگر (changed & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = split_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      names.layout);
        m->rmlvo.variants = split_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       names.variant);
        اگر (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            اگر (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        } وگرنه اگر (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } وگرنه {
        ساختار xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            ساختار matched_sval val = {
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

    اگر (changed & RMLVO_OPTIONS) {
        m->rmlvo.options = split_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      names.options);
    } وگرنه {
        ساختار xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            ساختار matched_sval val = {
                .sval = SVAL(option->option, strlen_safe(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    بازگشت m;
}

ایستا ساختار matcher *
matcher_new_from_names(ساختار xkb_context *ctx,
            ثابت ساختار xkb_rule_names *rmlvo)
{
    ساختار matcher *m = calloc(1, اندازه(*m));
    اگر (!m)
        بازگشت پوچ;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    اگر (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        اگر (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
    } وگرنه اگر (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    بازگشت m;
}

ایستا تهی
matcher_free(ساختار matcher *m)
{
    اگر (!m)
        بازگشت;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    ساختار group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.buffer);
    darray_free(m->pending_kccgst.slices);
    برای (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_رقم_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    آزاد(m);
}

ایستا تهی
matcher_group_start_new(ساختار matcher *m, ساختار sval name)
{
    ساختار group group = { .name = name, .elements = darray_new() };
    darray_append(m->groups, group);
}

ایستا تهی
matcher_group_add_element(ساختار matcher *m, ساختار scanner *s,
                          ساختار sval element)
{
    darray_append(darray_item(m->groups, darray_size(m->groups) - 1).elements,
                  element);
}

ایستا bool
read_rules_file(ساختار xkb_context *ctx,
                ساختار matcher *matcher,
                unsigned صحیح include_depth,
                FILE *file,
                ثابت نویسه *path);

ایستا تهی
matcher_include(ساختار matcher *m, ساختار scanner *parent_scanner,
                unsigned صحیح include_depth,
                ساختار sval inc)
{
    اگر (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        بازگشت;
    }

    ثابت نویسه *stmt_file = inc.start;
    size_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    نویسه buf[PATH_MAX] = {0};
    ثابت ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, اندازه(buf));
    اگر (expanded < 0) {
        /* Error */
        بازگشت;
    } وگرنه اگر (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (size_t) expanded;
        أكد(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned صحیح offset = 0;
    ثابت bool absolute_path = is_absolute_path(stmt_file);
    اگر (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        اگر (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            اگر (stmt_file_len < اندازه(buf)) {
                memcpy(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } وگرنه {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, اندازه(buf),
                        (unsigned صحیح) stmt_file_len, stmt_file);
                بازگشت;
            }
        } وگرنه {
            /* %-expansion is always NULL-terminated */
            أكد(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } وگرنه {
        /* Relative path: lookup the first XKB path */
        اگر (unlikely(expanded)) {
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
            file = پوچ;
        } وگرنه {
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, اندازه(buf), &offset, true);
        }
    }

    وقتی (file) {
        أكد(strlen_safe(buf) < اندازه(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        fclose(file);
        اگر (ret)
            بازگشت;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        اگر (absolute_path) {
            /* There is no point to search further if the path is absolute */
            بشکن;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, اندازه(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (unsigned صحیح) stmt_file_len, stmt_file);
}

ایستا تهی
matcher_mapping_start_new(ساختار matcher *m)
{
    برای (mlvo_index_t i = 0; i < (mlvo_index_t) MLVO_رقم_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_رقم_ENTRIES;
    برای (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_رقم_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_رقم_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.رقم_mlvo = m->mapping.رقم_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
ایستا صحیح
parse_layout_int_index_tail(ثابت نویسه *s, size_t max_len,
                            xkb_layout_index_t *out)
{
    اگر (max_len < 3)
        بازگشت -1;
    uint32_t val = 0;
    ثابت صحیح شمار = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    اگر (شمار <= 0 || s[1 + شمار] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        بازگشت -1;
    /* To zero-based index. */
    *out = val - 1;
    بازگشت شمار + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
ایستا صحیح
extract_layout_index(ثابت نویسه *s, size_t max_len, xkb_layout_index_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    اگر (max_len < 3 || s[0] != '[')
        بازگشت -1;
    اگر (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_index_t)(s[1] - '1');
        بازگشت 3;
    }
    اگر (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        بازگشت 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    بازگشت parse_layout_int_index_tail(s, max_len, out);
}

/* Special layout indices */
شمارش layout_index_ranges {
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
ایستا bool
extract_mapping_layout_index(ثابت نویسه *s, size_t max_len,
                             xkb_layout_index_t *out)
{
    /* Check for minimal `[` + index + `]` */
    اگر (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        بازگشت false;
    }

    /* Small numeric index */
    اگر (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_index_t)(s[1] - '1');
        بازگشت true;
    }

    /* Named indices ranges */
    ایستا ثابت ساختار {
        ثابت نویسه* name;
        size_t size;
        xkb_layout_index_t مقدار;
    } names[] = {
        #define INDEX_RANGE(tail, مقدار) \
            { (tail), اندازه(tail), (xkb_layout_index_t)(مقدار) }
        INDEX_RANGE("single]",   LAYOUT_INDEX_SINGLE),
        INDEX_RANGE("first]",    LAYOUT_INDEX_FIRST),
        INDEX_RANGE("later]",    LAYOUT_INDEX_LATER),
        INDEX_RANGE("multiple]", LAYOUT_INDEX_MULTIPLE),
        INDEX_RANGE("any]",      LAYOUT_INDEX_ANY),
        #undef INDEX_RANGE
    };

    برای (size_t k = 0; k < ARRAY_SIZE(names); k++) {
        اگر (max_len == names[k].size &&
            memcmp(&s[1], names[k].name, names[k].size - 1) == 0) {
            *out = names[k].مقدار;
            بازگشت true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    ثابت صحیح شمار = parse_layout_int_index_tail(s, max_len, out);
    بازگشت شمار > 0 && (size_t)شمار == max_len;
}

ایستا درونخطی bool
is_mlvo_mask_defined(ساختار matcher *m, شمارش rules_mlvo mlvo)
{
    بازگشت m->mapping.defined_mlvo_mask & (1u << mlvo);
}

ایستا تهی
matcher_mapping_set_mlvo(ساختار matcher *m, ساختار scanner *s,
                         ساختار sval ident)
{
    شمارش rules_mlvo mlvo;
    ساختار sval mlvo_sval;

    برای (mlvo = 0; mlvo < MLVO_رقم_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        اگر (svaleq_prefix(mlvo_sval, ident))
            بشکن;
    }

    /* Not found. */
    اگر (mlvo >= MLVO_رقم_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned صحیح) ident.len, ident.start);
        m->mapping.active = false;
        بازگشت;
    }

    اگر (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned صحیح) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        بازگشت;
    }

    /* If there are leftovers still, it must be an index. */
    اگر (mlvo_sval.len < ident.len) {
        xkb_layout_index_t idx;
        اگر (!extract_mapping_layout_index(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (unsigned صحیح) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            بازگشت;
        }

        اگر (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        وگرنه اگر (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        وگرنه {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (unsigned صحیح) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            بازگشت;
        }
    } وگرنه اگر (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    } وگرنه اگر (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    اگر (((mlvo == MLVO_LAYOUT && is_mlvo_mask_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_mask_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.active = false;
        بازگشت;
    }

    m->mapping.mlvo_at_pos[m->mapping.رقم_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.رقم_mlvo++;
}

ایستا تهی
matcher_mapping_set_layout_bounds(ساختار matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_index_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    انتخاب (idx) {
        حالت XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            أكد(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            بشکن;
        حالت LAYOUT_INDEX_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            بشکن;
        حالت LAYOUT_INDEX_MULTIPLE:
        حالت LAYOUT_INDEX_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            بشکن;
        حالت LAYOUT_INDEX_SINGLE:
        حالت LAYOUT_INDEX_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        پیشفرض:
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

ایستا تهی
matcher_mapping_set_kccgst(ساختار matcher *m, ساختار scanner *s, ساختار sval ident)
{
    شمارش rules_kccgst kccgst;
    ساختار sval kccgst_sval;

    برای (kccgst = 0; kccgst < KCCGST_رقم_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        اگر (svaleq(rules_kccgst_svals[kccgst], ident))
            بشکن;
    }

    /* Not found. */
    اگر (kccgst >= KCCGST_رقم_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned صحیح) ident.len, ident.start);
        m->mapping.active = false;
        بازگشت;
    }

    اگر (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned صحیح) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        بازگشت;
    }

    m->mapping.kccgst_at_pos[m->mapping.رقم_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.رقم_kccgst++;
}

ایستا bool
matcher_mapping_verify(ساختار matcher *m, ساختار scanner *s)
{
    اگر (m->mapping.رقم_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        برو skip;
    }

    اگر (m->mapping.رقم_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        برو skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    اگر (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        أكد(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        انتخاب (m->mapping.layout_idx) {
            حالت LAYOUT_INDEX_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                اگر (darray_size(m->rmlvo.layouts) > 1)
                    برو skip;
                بشکن;
            حالت LAYOUT_INDEX_MULTIPLE:
            حالت LAYOUT_INDEX_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                اگر (darray_size(m->rmlvo.layouts) < 2)
                    برو skip;
                بشکن;
            حالت LAYOUT_INDEX_ANY:
            حالت LAYOUT_INDEX_FIRST:
                /* No restrictions */
                بشکن;
            پیشفرض:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                اگر (darray_size(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_size(m->rmlvo.layouts))
                    برو skip;
        }
    }

    اگر (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        أكد(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        انتخاب (m->mapping.variant_idx) {
            حالت LAYOUT_INDEX_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                اگر (darray_size(m->rmlvo.variants) > 1)
                    برو skip;
                بشکن;
            حالت LAYOUT_INDEX_ANY:
            حالت LAYOUT_INDEX_LATER:
            حالت LAYOUT_INDEX_FIRST:
                /* No restriction */
                بشکن;
            پیشفرض:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                اگر (darray_size(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_size(m->rmlvo.variants))
                    برو skip;
        }
    }

    بازگشت true;

skip:
    m->mapping.active = false;
    بازگشت false;
}

ایستا تهی
matcher_rule_start_new(ساختار matcher *m)
{
    memset(&m->rule, 0, اندازه(m->rule));
    m->rule.skip = !m->mapping.active;
}

ایستا تهی
matcher_rule_set_mlvo_common(ساختار matcher *m, ساختار scanner *s,
                             ساختار sval ident,
                             شمارش mlvo_match_type match_type)
{
    اگر (m->rule.رقم_mlvo_مقادیر >= m->mapping.رقم_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        بازگشت;
    }
    m->rule.match_type_at_pos[m->rule.رقم_mlvo_مقادیر] = match_type;
    m->rule.mlvo_مقدار_at_pos[m->rule.رقم_mlvo_مقادیر] = ident;
    m->rule.رقم_mlvo_مقادیر++;
}

ایستا تهی
matcher_rule_set_mlvo_wildcard(ساختار matcher *m, ساختار scanner *s,
                               شمارش mlvo_match_type match_type)
{
    ساختار sval dummy = SVAL(پوچ, 0);
    matcher_rule_set_mlvo_common(m, s, dummy, match_type);
}

ایستا تهی
matcher_rule_set_mlvo_group(ساختار matcher *m, ساختار scanner *s,
                            ساختار sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_GROUP);
}

ایستا تهی
matcher_rule_set_mlvo(ساختار matcher *m, ساختار scanner *s,
                      ساختار sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_NORMAL);
}

ایستا تهی
matcher_rule_set_kccgst(ساختار matcher *m, ساختار scanner *s,
                        ساختار sval ident)
{
    اگر (m->rule.رقم_kccgst_مقادیر >= m->mapping.رقم_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        بازگشت;
    }
    m->rule.kccgst_مقدار_at_pos[m->rule.رقم_kccgst_مقادیر] = ident;
    m->rule.رقم_kccgst_مقادیر++;
}

ایستا bool
match_group(ساختار matcher *m, ساختار sval group_name, ساختار sval to)
{
    ساختار group *group = پوچ;
    ساختار sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        اگر (svaleq(group->name, group_name)) {
            found = true;
            بشکن;
        }
    }

    اگر (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        بازگشت false;
    }

    darray_foreach(element, group->elements)
        اگر (svaleq(to, *element))
            بازگشت true;

    بازگشت false;
}

ایستا bool
match_مقدار(ساختار matcher *m, ثابت ساختار sval val, ثابت ساختار sval to,
            شمارش mlvo_match_type match_type,
            شمارش wildcard_match_type wildcard_type)
{
    انتخاب (match_type) {
        حالت MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            بازگشت wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        حالت MLVO_MATCH_WILDCARD_NONE:
            بازگشت !to.len;
        حالت MLVO_MATCH_WILDCARD_SOME:
            بازگشت !!to.len;
        حالت MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            بازگشت true;
        حالت MLVO_MATCH_GROUP:
            بازگشت match_group(m, val, to);
        پیشفرض:
            أكد(match_type == MLVO_MATCH_NORMAL);
            بازگشت svaleq(val, to);
    }
}

ایستا bool
match_مقدار_and_mark(ساختار matcher *m, ثابت ساختار sval val,
                     ساختار matched_sval *to, شمارش mlvo_match_type match_type,
                     شمارش wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_مقدار(m, val, to->sval, match_type, wildcard_type);
    اگر (matched)
        to->matched |= layouts;
    بازگشت matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
ایستا bool
expand_rmlvo_in_kccgst_مقدار(ساختار matcher *m, ساختار scanner *s,
                             ساختار sval مقدار, xkb_layout_index_t layout_idx,
                             darray_char *expanded, size_t *i)
{
    ثابت نویسه *str = مقدار.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    شمارش rules_mlvo mlv;
    xkb_layout_index_t idx;
    نویسه pfx, sfx;
    ساختار matched_sval *expanded_مقدار;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    اگر (str[*i] == 'i' &&
        (*i + 1 == مقدار.len || is_merge_mode_prefix(str[*i + 1])))
    {
        اگر (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_INDEX_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            برو error;
        }
        (*i)++;
        نویسه index_str[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
        ثابت صحیح شمار = snprintf(index_str, اندازه(index_str), "%"PRIu32,
                                   layout_idx + 1);
        أكد(شمار > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, index_str, (darray_size_t)شمار);
        بازگشت true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    اگر (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        اگر (str[*i] == '(') sfx = ')';
        اگر (++(*i) >= مقدار.len) برو error;
    }

    /* Mandatory model/layout/variant specifier. */
    انتخاب (str[(*i)++]) {
    حالت 'm': mlv = MLVO_MODEL; بشکن;
    حالت 'l': mlv = MLVO_LAYOUT; بشکن;
    حالت 'v': mlv = MLVO_VARIANT; بشکن;
    پیشفرض: برو error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_index = false;
    اگر (*i < مقدار.len && str[*i] == '[') {
        اگر (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            برو error;
        }

        صحیح consumed = extract_layout_index(str + (*i), مقدار.len - (*i), &idx);
        اگر (consumed <= 0) برو error;
        اگر (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            أكد(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_index = true;
        }
        *i += (size_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    اگر (sfx != 0) {
        اگر (*i >= مقدار.len) برو error;
        اگر (str[(*i)++] != sfx) برو error;
    }

    /* Get the expanded value. */
    expanded_مقدار = پوچ;

    اگر (mlv == MLVO_LAYOUT) {
        اگر (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            اگر (darray_size(m->rmlvo.layouts) == 1)
                expanded_مقدار = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } وگرنه اگر (idx < darray_size(m->rmlvo.layouts) &&
                   (variable_index || darray_size(m->rmlvo.layouts) > 1)) {
                expanded_مقدار = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    وگرنه اگر (mlv == MLVO_VARIANT) {
        اگر (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            اگر (darray_size(m->rmlvo.variants) == 1)
                expanded_مقدار = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } وگرنه اگر (idx < darray_size(m->rmlvo.variants) &&
                   (variable_index || darray_size(m->rmlvo.variants) > 1)) {
                expanded_مقدار = &darray_item(m->rmlvo.variants, idx);
        }
    }
    وگرنه اگر (mlv == MLVO_MODEL) {
        expanded_مقدار = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    اگر (!expanded_مقدار || expanded_مقدار->sval.len == 0) {
        بازگشت true;
    }

    اگر (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_مقدار->sval.start,
                                 (darray_size_t) expanded_مقدار->sval.len);
    اگر (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    اگر (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ثابت xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_مقدار->matched |= layouts;
    } وگرنه {
        expanded_مقدار->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    بازگشت true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    بازگشت false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
ایستا تهی
expand_qualifier_in_kccgst_مقدار(
    ساختار matcher *m, ساختار scanner *s,
    ساختار sval مقدار, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_size_t prefix_idx, size_t *i)
{
    ثابت نویسه *str = مقدار.start;

    /* “all” followed by nothing or by a layout separator */
    اگر (*i + 2 < مقدار.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == مقدار.len || is_merge_mode_prefix(str[*i + 3]))) {
        اگر (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_MESSAGE_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        اگر (darray_size(m->rmlvo.layouts) > 1) {
            نویسه layout_index[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
            ثابت darray_size_t prefix_length =
                darray_size(*expanded) - prefix_idx - 1;
            برای (xkb_layout_index_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_size(m->rmlvo.layouts));
                 l++)
            {
                اگر (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_length);
                /* Append index */
                ثابت صحیح شمار = snprintf(layout_index, اندازه(layout_index),
                                           "%"PRIu32, l + 1);
                أكد(شمار > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_index,
                                             (darray_size_t)شمار);
            }
        }
        *i += 3;
    }
}

ایستا درونخطی تهی
#ifdef _MSC_VER
concat_kccgst(darray_char *into, darray_size_t size, _In_reads_(size) ثابت نویسه* from)
#وگرنه
concat_kccgst(darray_char *into, darray_size_t size, ثابت نویسه from[ایستا size])
#endif
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ثابت bool from_plus = is_merge_mode_prefix(from[0]);
    اگر (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, size);
    } وگرنه {
        ثابت نویسه ch =
            (نویسه) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        ثابت bool into_plus = is_merge_mode_prefix(ch);
        اگر (into_plus)
            darray_prepends_nullterminate(*into, from, size);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
ایستا bool
append_expanded_kccgst_مقدار(ساختار matcher *m, ساختار scanner *s,
                             bool merge, darray_char *to, ساختار sval مقدار,
                             xkb_layout_index_t layout_idx)
{
    ثابت نویسه *str = مقدار.start;
    darray_char expanded = darray_new();
    darray_size_t last_item_idx = 0;
    bool has_separator = false;

    برای (size_t i = 0; i < مقدار.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        انتخاب (str[i]) {
            /* Qualifier */
            حالت ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_مقدار(m, s, مقدار, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                بشکن;
            /* Expansion */
            حالت '%':
                i++;
                اگر (i >= مقدار.len ||
                    !expand_rmlvo_in_kccgst_مقدار(m, s, مقدار, layout_idx,
                                                  &expanded, &i))
                        برو error;
                بشکن;
            /* New item */
            حالت MERGE_OVERRIDE_PREFIX:
            حالت MERGE_AUGMENT_PREFIX:
            حالت MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_size(expanded) - 1;
                has_separator = true;
                بشکن;
            /* Just a normal character. */
            پیشفرض:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    اگر (merge) {
        اگر (!darray_empty(expanded))
            concat_kccgst(to, darray_size(expanded), darray_اقلام(expanded));
    } وگرنه {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    بازگشت true;

error:
    darray_free(expanded);
    بازگشت false;
}

ایستا bool
matcher_append_pending_kccgst(ساختار matcher *m)
{
    اگر (!m->mapping.has_multiple_layouts)
        بازگشت true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    برای (kccgst_index_t i = 0; i < m->mapping.رقم_kccgst; i++) {
        ثابت شمارش rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        برای (xkb_layout_index_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            ثبات ثابت ساختار kccgst_buffer* ثابت buf = &m->pending_kccgst;
            size_t offset = 0;
            برای (darray_size_t k = 0; k < darray_size(buf->slices); k++) {
                ثبات ثابت ساختار kccgst_buffer_slice * ثابت slice =
                    &darray_item(buf->slices, k);
                اگر (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->length)
                    concat_kccgst(&m->kccgst[kccgst], slice->length,
                                  darray_اقلام(buf->buffer) + offset);
                offset += slice->length;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    بازگشت true;
}

ایستا تهی
matcher_rule_verify(ساختار matcher *m, ساختار scanner *s)
{
    اگر (m->rule.رقم_mlvo_مقادیر != m->mapping.رقم_mlvo ||
        m->rule.رقم_kccgst_مقادیر != m->mapping.رقم_kccgst) {
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
ایستا bool
matcher_rule_match_model(ساختار matcher *m, ثابت ساختار sval مقدار,
                         شمارش mlvo_match_type match_type)
{
    بازگشت match_مقدار_and_mark(
        m, مقدار, &m->rmlvo.model, match_type, WILDCARD_MATCH_ALL,
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
ایستا bool
matcher_rule_match_option(ساختار matcher *m, ثابت ساختار sval مقدار,
                          شمارش mlvo_match_type match_type,
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
    ساختار matched_sval *option;
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
        اگر (matchable && match_مقدار_and_mark(m, مقدار, option, match_type,
                                              WILDCARD_MATCH_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            اگر (!unmatched)
                بشکن;
        }
    }
    اگر (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    بازگشت matched;
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
ایستا bool
matcher_rule_match_layout_or_variant(ساختار matcher *m,
                                     darray_matched_sval *درون_داد,
                                     ثابت ساختار sval مقدار,
                                     شمارش mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_empty(*درون_داد));

    bool matched = false;
    /* Loop over the layout index range */
    برای (xkb_layout_index_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ثابت xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        اگر (layout & *candidates) {
            ساختار matched_sval *to = &darray_item(*درون_داد, idx);
            اگر (match_مقدار_and_mark(m, مقدار, to, match_type,
                                     WILDCARD_MATCH_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } وگرنه {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    بازگشت matched;
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
ایستا تهی
matcher_rule_apply_if_matches(ساختار matcher *m, ساختار scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_mask_t candidate_layouts = m->mapping.layouts_candidates_mask;

    /* Loop over MLVO pattern components */
    برای (mlvo_index_t i = 0; i < m->mapping.رقم_mlvo; i++) {
        ثابت شمارش rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ثابت ساختار sval مقدار = m->rule.mlvo_مقدار_at_pos[i];
        ثابت شمارش mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        انتخاب (mlvo) {
        حالت MLVO_MODEL:
            matched = matcher_rule_match_model(m, مقدار, match_type);
            بشکن;
        حالت MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, مقدار, match_type, &candidate_layouts
            );
            بشکن;
        حالت MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, مقدار, match_type, &candidate_layouts
            );
            بشکن;
        حالت MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, مقدار, match_type, &candidate_layouts
            );
            بشکن;
        پیشفرض: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_رقم_ENTRIES - 1,
                          "Unexpected MLVO field");
            أكد(!"Unreachable");
        }
        }

        اگر (!matched)
            بازگشت;
    }

    اگر (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        برای (xkb_layout_index_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            اگر (candidate_layouts & (UINT32_C(1) << idx)) {
                برای (kccgst_index_t i = 0; i < m->mapping.رقم_kccgst; i++) {
                    ثابت شمارش rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ثابت ساختار sval مقدار = m->rule.kccgst_مقدار_at_pos[i];
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
                    ساختار kccgst_buffer * ثابت buf = &m->pending_kccgst;
                    ثابت darray_size_t prev_buffer_length =
                        darray_size(buf->buffer);
                    append_expanded_kccgst_مقدار(m, s, false, &buf->buffer,
                                                 مقدار, idx);
                    ثابت uint32_t length = (uint32_t) (darray_size(buf->buffer)
                                          - prev_buffer_length);
                    ثابت ساختار kccgst_buffer_slice slice = {
                        .length = length,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } وگرنه {
        /* Numeric index or no index */
        برای (kccgst_index_t i = 0; i < m->mapping.رقم_kccgst; i++) {
            شمارش rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            ساختار sval مقدار = m->rule.kccgst_مقدار_at_pos[i];
            append_expanded_kccgst_مقدار(m, s, true, &m->kccgst[kccgst], مقدار,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    اگر (!(is_mlvo_mask_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_mask &= ~candidate_layouts;
    }
}

ایستا شمارش rules_token
gettok(ساختار matcher *m, ساختار scanner *s)
{
    بازگشت lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
ایستا bool
matcher_match(ساختار matcher *m, ساختار scanner *s,
              unsigned صحیح include_depth,
              ثابت نویسه *string, size_t len,
              ثابت نویسه *file_name)
{
    شمارش rules_token tok;

    اگر (!m)
        بازگشت false;

initial:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_BANG:
        برو bang;
    حالت TOK_END_OF_سطر:
        برو initial;
    حالت TOK_END_OF_FILE:
        برو finish;
    پیشفرض:
        برو unexpected;
    }

bang:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        برو group_name;
    حالت TOK_INCLUDE:
        برو include_statement;
    حالت TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        برو mapping_mlvo;
    پیشفرض:
        برو unexpected;
    }

group_name:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_EQUALS:
        برو group_element;
    پیشفرض:
        برو unexpected;
    }

group_element:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        برو group_element;
    حالت TOK_END_OF_سطر:
        برو initial;
    پیشفرض:
        برو unexpected;
    }

include_statement:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        برو include_statement_end;
    پیشفرض:
        برو unexpected;
    }

include_statement_end:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_END_OF_سطر:
        برو initial;
    پیشفرض:
        برو unexpected;
    }

mapping_mlvo:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_IDENTIFIER:
        اگر (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        برو mapping_mlvo;
    حالت TOK_EQUALS:
        برو mapping_kccgst;
    پیشفرض:
        برو unexpected;
    }

mapping_kccgst:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_IDENTIFIER:
        اگر (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        برو mapping_kccgst;
    حالت TOK_END_OF_سطر:
        اگر (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            اگر (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_size(m->pending_kccgst.buffer) = 0;
                darray_size(m->pending_kccgst.slices) = 0;
            }
        }
        برو rule_mlvo_first;
    پیشفرض:
        برو unexpected;
    }

rule_mlvo_first:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_BANG:
        matcher_append_pending_kccgst(m);
        برو bang;
    حالت TOK_END_OF_سطر:
        برو rule_mlvo_first;
    حالت TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        برو finish;
    پیشفرض:
        matcher_rule_start_new(m);
        برو rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    انتخاب (tok) {
    حالت TOK_IDENTIFIER:
        اگر (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        برو rule_mlvo;
    حالت TOK_WILD_CARD_STAR:
        اگر (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        برو rule_mlvo;
    حالت TOK_WILD_CARD_NONE:
        اگر (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        برو rule_mlvo;
    حالت TOK_WILD_CARD_SOME:
        اگر (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        برو rule_mlvo;
    حالت TOK_WILD_CARD_ANY:
        اگر (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        برو rule_mlvo;
    حالت TOK_GROUP_NAME:
        اگر (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        برو rule_mlvo;
    حالت TOK_EQUALS:
        برو rule_kccgst;
    پیشفرض:
        برو unexpected;
    }

rule_kccgst:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_IDENTIFIER:
        اگر (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        برو rule_kccgst;
    حالت TOK_END_OF_سطر:
        اگر (!m->rule.skip)
            matcher_rule_verify(m, s);
        اگر (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        برو rule_mlvo_first;
    پیشفرض:
        برو unexpected;
    }

unexpected:
    انتخاب (tok) {
    حالت TOK_ERROR:
        برو error;
    پیشفرض:
        برو state_error;
    }

finish:
    بازگشت true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    بازگشت false;
}
// NOLINTEND(readability-trivial-switch)

ایستا bool
read_rules_file(ساختار xkb_context *ctx,
                ساختار matcher *matcher,
                unsigned صحیح include_depth,
                FILE *file,
                ثابت نویسه *path)
{
    bool ret;
    نویسه *string;
    size_t size;
    ساختار scanner scanner;

    اگر (!map_file(file, &string, &size)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, strerror(errno));
        بازگشت false;
    }

    scanner_init(&scanner, matcher->ctx, string, size, path, پوچ);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    اگر (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, size);
        بازگشت false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, size, path);
    unmap_file(string, size);
    بازگشت ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
ایستا bool
xkb_resolve_partial_rules(ساختار xkb_context *ctx, نویسه *path, size_t path_size,
                          ثابت نویسه* rules, ثابت نویسه* suffix,
                          ساختار matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    نویسه partial_rules[60]; /* Arbitrary, but we do not expect long names */
    اگر (unlikely(!snprintf_safe(partial_rules, اندازه(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        بازگشت false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    unsigned صحیح offset = 0;
    FILE *file = پوچ;
    ثابت size_t len = strlen(partial_rules);
    وقتی ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_size, &offset, false)) != پوچ) {
        ثابت bool ok = read_rules_file(ctx, matcher, 0, file, path);
        fclose(file);
        اگر (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            بازگشت false;
        }
        offset++;
    }

    بازگشت true;
}

ایستا bool
xkb_resolve_rules(ساختار xkb_context *ctx,
                  ثابت نویسه* rules, ساختار matcher *matcher,
                  ساختار xkb_component_names *out,
                  xkb_layout_index_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    unsigned صحیح offset = 0;
    نویسه path[PATH_MAX];
    FILE * ثابت file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, strlen(rules), FILE_TYPE_RULES,
                                          path, اندازه(path), &offset, true);
    اگر (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        برو err_out;
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
    ret = xkb_resolve_partial_rules(ctx, path, اندازه(path),
                                    rules, ".pre", matcher);
    اگر (!ret)
        برو err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    اگر (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        برو err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, اندازه(path),
                                    rules, ".post", matcher);
    اگر (!ret)
        برو err_out;

    /* Check we got required components */
    اگر (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        برو err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, پوچ);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, پوچ);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, پوچ);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, پوچ);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, پوچ);

    ساختار matched_sval *mval = &matcher->rmlvo.model;
    اگر (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (unsigned صحیح) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        اگر (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned صحیح) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        اگر (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned صحیح) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        اگر (!mval->sval.len)
            ادامه;

        اگر (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned صحیح) mval->sval.len, mval->sval.start, rules);
        } وگرنه اگر (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ثابت xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            أكد(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (unsigned صحیح)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    اگر (out->symbols != پوچ && explicit_layouts != پوچ) {
        *explicit_layouts = 1; /* at least one group */
        ثابت نویسه *symbols = out->symbols;
        /* Take the highest modifier */
        وقتی ((symbols = strchr(symbols, ':')) != پوچ && symbols[1] != '\0') {
            xkb_layout_index_t group = 0;
            ثابت صحیح شمار = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            اگر (شمار > 0 && (symbols[شمار] == '\0' ||
                is_merge_mode_prefix(symbols[شمار])) &&
                group > 0 && group <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, group);
                symbols += شمار;
            }
        }
    }

err_out:
    اگر (file)
        fclose(file);

    بازگشت ret;
}

bool
xkb_components_from_rmlvo_builder(ثابت ساختار xkb_rmlvo_builder *rmlvo,
                                  ساختار xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts)
{
    ثابت نویسه *rules = rmlvo->rules;
    ساختار matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    اگر (!matcher)
        بازگشت false;

    ثابت bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    بازگشت ret;
}


bool
xkb_components_from_rules_names(ساختار xkb_context *ctx,
                                ثابت ساختار xkb_rule_names *rmlvo,
                                ساختار xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts)
{
    ساختار matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    اگر (!matcher)
        بازگشت false;

    ثابت bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    بازگشت ret;
}
