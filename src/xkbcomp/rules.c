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

#تعریف_کرو MAX_INCLUDE_گہرائی 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
اتحاد lvalue {
    ساخت sval string;
};

شمار قواعد_token {
    TOK_انتہا_کا_FILE = 0,
    TOK_انتہا_کا_سطر,
    TOK_IDENTIFIER,
    TOK_گروہ_NAME,
    TOK_BANG,
    TOK_EQUALS,
    TOK_WILD_CARD_ستارہ,
    TOK_WILD_CARD_NONE,
    TOK_WILD_CARD_SOME,
    TOK_WILD_CARD_ANY,
    TOK_INCLUDE,
    TOK_ERROR
};

ساکن شمار قواعد_token
lex(ساخت scanner *s, اتحاد lvalue *val)
{
skip_مزید_whitespace_and_comments:
    /* Skip spaces */
    جب (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    اگر (scanner_lit(s, "//")) {
        scanner_skip_مقصد_eol(s);
    }

    /* New line */
    اگر (scanner_eol(s)) {
        جب (scanner_eol(s)) scanner_اگلا(s);
        واپس TOK_انتہا_کا_سطر;
    }

    /* Escaped line continuation */
    اگر (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        اگر (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            واپس TOK_ERROR;
        }
        scanner_اگلا(s);
        جاؤ skip_مزید_whitespace_and_comments;
    }

    /* See if we're done */
    اگر (scanner_eof(s)) واپس TOK_انتہا_کا_FILE;

    /* New token */
    s->token_pos = s->pos;
    شمار قواعد_token token = TOK_IDENTIFIER;

    ثابت حرف اگلا = scanner_اگلا(s);
    انتخاب (اگلا) {
    حالت '!':
        واپس TOK_BANG;
    حالت '$':
        /*گروہ*/
        token = TOK_گروہ_NAME;
        val->string.len = 0;
        توڑو;
    حالت '*':
        اگر (scanner_قواعد_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            توڑو;
        } ورنہ {
            /* Legacy wild card */
            واپس TOK_WILD_CARD_ستارہ;
        }
    حالت '<':
        /* Extended wild cards */
        اگر (scanner_قواعد_lit_token(s, "none>")) واپس TOK_WILD_CARD_NONE;
        اگر (scanner_قواعد_lit_token(s, "some>")) واپس TOK_WILD_CARD_SOME;
        اگر (scanner_قواعد_lit_token(s, "any>")) واپس TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        أكد(scanner_قواعد_is_ident('<'));
        val->string.len = 1;
        توڑو;
    حالت '=':
        اگر (scanner_قواعد_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            توڑو;
        } ورنہ {
            واپس TOK_EQUALS;
        }
    حالت 'i':
        اگر (scanner_قواعد_lit_token(s, "nclude")) {
            /* Include statement */
            واپس TOK_INCLUDE;
        }
        /* Identifier */
        أكد(scanner_قواعد_is_ident('i'));
        val->string.len = 1;
        توڑو;
    طےشدہ:
        اگر (likely(scanner_قواعد_is_ident(اگلا))) {
            /* Identifier */
            val->string.len = 1;
        } ورنہ {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.آغاز = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    أكد(scanner_قواعد_is_ident(ملاؤ_OVERRIDE_سابقہ));
    أكد(scanner_قواعد_is_ident(ملاؤ_AUGMENT_سابقہ));
    أكد(scanner_قواعد_is_ident(ملاؤ_REPLACE_سابقہ));
    أكد(scanner_قواعد_is_ident('('));
    أكد(scanner_قواعد_is_ident(')'));
    val->string.len += scanner_قواعد_ident_token(s);

    اگر (unlikely(val->string.len == 0)) {
        اگر (token == TOK_گروہ_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } ورنہ {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "unrecognized token");
        }
        واپس TOK_ERROR;
    }

    واپس token;
}

/***====================================================================***/

شمار قواعد_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_عدد_ENTRIES
};

شمار {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

قسمتعریف uint8_t mlvo_اشاریہ_t;
قسمتعریف uint8_t mlvo_نقاب_t;


ساکن ثابت ساخت sval قواعد_mlvo_svals[MLVO_عدد_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

شمار قواعد_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_عدد_ENTRIES
};

قسمتعریف uint8_t kccgst_اشاریہ_t;
قسمتعریف uint8_t kccgst_نقاب_t;

ساکن ثابت ساخت sval قواعد_kccgst_svals[KCCGST_عدد_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

شمار {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL ==
              (xkb_layout_نقاب_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
ساخت matched_sval {
    ساخت sval sval;
    /** Used for layout-specific options */
    xkb_layout_نقاب_t layouts;
    xkb_layout_نقاب_t matched;
};
قسمتعریف darray(ساخت matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
ساخت قاعدہ_نام {
    ساخت matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

ساخت گروہ {
    ساخت sval name;
    darray_sval elements;
};

ساخت mapping {
    شمار قواعد_mlvo mlvo_at_pos[MLVO_عدد_ENTRIES];
    mlvo_اشاریہ_t عدد_mlvo;
    mlvo_نقاب_t defined_mlvo_نقاب;
    bool has_layout_idx_range;
    bool has_مضرب_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    اتحاد {
        ساخت { xkb_layout_اشاریہ_t layout_idx, variant_idx; };
        ساخت { xkb_layout_اشاریہ_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    اتحاد {
        xkb_layout_نقاب_t فعال;
        xkb_layout_نقاب_t layouts_candidates_نقاب;
    };
    شمار قواعد_kccgst kccgst_at_pos[KCCGST_عدد_ENTRIES];
    kccgst_اشاریہ_t عدد_kccgst;
    kccgst_نقاب_t defined_kccgst_نقاب;
};

شمار mlvo_مطابقت_type {
    /** Match the given plain value */
    MLVO_مطابقت_معمول = 0,
    /** Match depending on the value of `wildcard_match_type` */
    MLVO_مطابقت_WILDCARD_LEGACY,
    /** Match empty value */
    MLVO_مطابقت_WILDCARD_NONE,
    /** Match non-empty value */
    MLVO_مطابقت_WILDCARD_SOME,
    /** Match any value, optionally empty */
    MLVO_مطابقت_WILDCARD_ANY,
    /** Match any entry in a group */
    MLVO_مطابقت_گروہ,
};

شمار wildcard_مطابقت_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_مطابقت_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_مطابقت_ALL,
};

ساخت قاعدہ_2 {
    ساخت sval mlvo_قدر_at_pos[MLVO_عدد_ENTRIES];
    شمار mlvo_مطابقت_type مطابقت_type_at_pos[MLVO_عدد_ENTRIES];
    ساخت sval kccgst_قدر_at_pos[KCCGST_عدد_ENTRIES];
    mlvo_اشاریہ_t عدد_mlvo_اقدار;
    kccgst_اشاریہ_t عدد_kccgst_اقدار;
    bool skip;
};

#تعریف_کرو قاش_KCCGST_ثنائی_ہندسہ_میدان_سائز 4
static_assert(!((1u << (قاش_KCCGST_ثنائی_ہندسہ_میدان_سائز - 1)) &
                (KCCGST_عدد_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
ساخت kccgst_بفر_قاش {
    uint32_t لمبائی:(32 - قاش_KCCGST_ثنائی_ہندسہ_میدان_سائز);
    شمار قواعد_kccgst kccgst:قاش_KCCGST_ثنائی_ہندسہ_میدان_سائز;
    xkb_layout_اشاریہ_t layout;
};

/* Buffer for pending KcCGST values */
ساخت kccgst_بفر {
    darray_char بفر;
    /* Slice corresponding to each value in the buffer */
    darray(ساخت kccgst_بفر_قاش) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
ساخت matcher {
    ساخت xkb_context *ctx;
    /* Input.*/
    ساخت قاعدہ_نام rmlvo;
    اتحاد lvalue val;
    darray(ساخت گروہ) groups;
    /* Current mapping. */
    ساخت mapping mapping;
    /* Current rule. */
    ساخت قاعدہ_2 قاعدہ_2;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    ساخت kccgst_بفر pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_عدد_ENTRIES];
};

ساکن ساخت sval
strip_spaces(ساخت sval v)
{
    جب (v.len > 0 && is_space(v.آغاز[0])) { v.len--; v.آغاز++; }
    جب (v.len > 0 && is_space(v.آغاز[v.len - 1])) v.len--;
    واپس v;
}

ساکن darray_matched_sval
بانٹو_comma_separated_mlvo(ساخت xkb_context *ctx,
                           شمار قواعد_mlvo mlvo, ثابت حرف *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    اگر (!s) {
        ensure_at_کم_سے_کم_one_قدر:
        /* Label followed by a declaration is a C23 extension */ ;

        ساخت matched_sval val = {
            .sval = SVAL(تہی, 0),
            .layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        واپس arr;
    }

    جب (true) {
        ساخت matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL,
            .matched = 0,
        };
        جب (*s != '\0' && *s != ',' && *s != OPTIONS_گروہ_SPECIFIER_سابقہ) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        اگر (*s == OPTIONS_گروہ_SPECIFIER_سابقہ) {
            /* Handle numeric layout index */
            s++;
            ثابت حرف* ثابت layout_آغاز = s;
            xkb_layout_اشاریہ_t layout = XKB_LAYOUT_INVALID;

            صحیح گنتی = parse_dec_مقصد_uint32_t(s, SIZE_MAX, &layout);
            اگر (گنتی > 0) {
                /* Note: 1-indexed layout */
                s += گنتی;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                اگر (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (غیرعلامتی صحیح) val.sval.len, val.sval.آغاز);
                } ورنہ اگر (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (غیرعلامتی صحیح) val.sval.len, val.sval.آغاز);
                } ورنہ {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ثابت حرف* ثابت layout_اشاریہ_انتہا = s;
            جب (*s != '\0' && *s != ',') { s++; }
            اگر (گنتی <= 0 || layout_اشاریہ_انتہا != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (غیرعلامتی صحیح) (s - layout_آغاز), layout_آغاز,
                        (غیرعلامتی صحیح) val.sval.len, val.sval.آغاز);
            }
        }

        اگر (val.sval.len || mlvo != MLVO_OPTION) {
            اگر (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                ساخت matched_sval *پچھلا;
                darray_foreach(پچھلا, arr) {
                    اگر (svaleq(پچھلا->sval, val.sval)) {
                        /* Merge options */
                        اگر (val.layouts ==
                            (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            پچھلا->layouts = val.layouts;
                        } ورنہ اگر (پچھلا->layouts != (xkb_layout_نقاب_t)
                                   XKB_OPTION_LAYOUT_نقاب_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            پچھلا->layouts |= val.layouts;
                        }
                        جاؤ اگلا_قدر;
                    }
                }
            }
            darray_append(arr, val);
        }

اگلا_قدر:
        اگر (*s == '\0') توڑو;
        اگر (*s == ',') s++;
    }

    اگر (darray_خالی_خانے(arr))
        جاؤ ensure_at_کم_سے_کم_one_قدر;

    واپس arr;
}

ساکن ساخت matcher *
matcher_new_from_rmlvo(ثابت ساخت xkb_rmlvo_builder *rmlvo, ثابت حرف **قواعد)
{
    ساخت matcher *m = صفر_حافظہ_دو(1, حجم(*m));
    اگر (!m)
        واپس تہی;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    ساخت xkb_قاعدہ_نام نام = {
        .قواعد = rmlvo->قواعد,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_خالی_خانے(rmlvo->layouts)) ? تہی : "x",
        .variant = (darray_خالی_خانے(rmlvo->layouts)) ? تہی : "x",
        .options = (darray_خالی_خانے(rmlvo->options)) ? تہی : "x",
    };
    ثابت شمار RMLVO بدلا = xkb_context_sanitize_قاعدہ_نام(rmlvo->ctx,
                                                               &نام);

    /* Initialize matcher with sanitized names, if relevant */
    اگر (بدلا & RMLVO_قواعد) {
        *قواعد = نام.قواعد;
    } ورنہ {
        *قواعد = rmlvo->قواعد;
    }

    اگر (بدلا & RMLVO_MODEL) {
        m->rmlvo.model.sval.آغاز = نام.model;
    } ورنہ {
        m->rmlvo.model.sval.آغاز = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_محفوظ(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL;

    أكد((بدلا & RMLVO_LAYOUT) || !(بدلا & RMLVO_VARIANT));
    اگر (بدلا & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = بانٹو_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      نام.layout);
        m->rmlvo.variants = بانٹو_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       نام.variant);
        اگر (darray_سائز(m->rmlvo.layouts) > darray_سائز(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            اگر (!isempty(نام.variant))
                log_warn(m->ctx, XKB_LOG_پیغام_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         نام.layout ? نام.layout : "(none)",
                         نام.variant ? نام.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_سائز(m->rmlvo.layouts));
        } ورنہ اگر (darray_سائز(m->rmlvo.layouts) < darray_سائز(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_پیغام_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    نام.layout ? نام.layout : "(none)",
                    نام.variant ? نام.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_سائز(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } ورنہ {
        ساخت xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            ساخت matched_sval val = {
                .sval = SVAL(layout->layout, strlen_محفوظ(layout->layout)),
                .layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL,
                .matched = 0
            };
            darray_append(m->rmlvo.layouts, val);
            val.sval.آغاز = layout->variant;
            val.sval.len = strlen_محفوظ(layout->variant);
            darray_append(m->rmlvo.variants, val);
        }
    }

    اگر (بدلا & RMLVO_OPTIONS) {
        m->rmlvo.options = بانٹو_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      نام.options);
    } ورنہ {
        ساخت xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            ساخت matched_sval val = {
                .sval = SVAL(option->option, strlen_محفوظ(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    واپس m;
}

ساکن ساخت matcher *
matcher_new_from_نام(ساخت xkb_context *ctx,
            ثابت ساخت xkb_قاعدہ_نام *rmlvo)
{
    ساخت matcher *m = صفر_حافظہ_دو(1, حجم(*m));
    اگر (!m)
        واپس تہی;

    m->ctx = ctx;
    m->rmlvo.model.sval.آغاز = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_محفوظ(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL;
    m->rmlvo.layouts = بانٹو_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = بانٹو_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = بانٹو_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    اگر (darray_سائز(m->rmlvo.layouts) > darray_سائز(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        اگر (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_سائز(m->rmlvo.layouts));
    } ورنہ اگر (darray_سائز(m->rmlvo.layouts) < darray_سائز(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_سائز(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    واپس m;
}

ساکن خالی
matcher_free(ساخت matcher *m)
{
    اگر (!m)
        واپس;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    ساخت گروہ *گروہ;
    darray_foreach(گروہ, m->groups)
        darray_free(گروہ->elements);
    darray_free(m->pending_kccgst.بفر);
    darray_free(m->pending_kccgst.slices);
    برائے (kccgst_اشاریہ_t i = 0; i < (kccgst_اشاریہ_t) KCCGST_عدد_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    آزاد(m);
}

ساکن خالی
matcher_گروہ_آغاز_new(ساخت matcher *m, ساخت sval name)
{
    ساخت گروہ گروہ = { .name = name, .elements = darray_new() };
    darray_append(m->groups, گروہ);
}

ساکن خالی
matcher_گروہ_جوڑو_element(ساخت matcher *m, ساخت scanner *s,
                          ساخت sval element)
{
    darray_append(darray_شے(m->groups, darray_سائز(m->groups) - 1).elements,
                  element);
}

ساکن bool
read_قواعد_file(ساخت xkb_context *ctx,
                ساخت matcher *matcher,
                غیرعلامتی صحیح include_گہرائی,
                FILE *file,
                ثابت حرف *راستہ);

ساکن خالی
matcher_include(ساخت matcher *m, ساخت scanner *والد_scanner,
                غیرعلامتی صحیح include_گہرائی,
                ساخت sval inc)
{
    اگر (include_گہرائی >= MAX_INCLUDE_گہرائی) {
        scanner_err(والد_scanner, XKB_LOG_پیغام_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_گہرائی);
        واپس;
    }

    ثابت حرف *stmt_file = inc.آغاز;
    سائز_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    حرف buf[PATH_MAX] = {0};
    ثابت ssize_t expanded = expand_راستہ(m->ctx, والد_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_قواعد,
                                         buf, حجم(buf));
    اگر (expanded < 0) {
        /* Error */
        واپس;
    } ورنہ اگر (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (سائز_t) expanded;
        أكد(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    غیرعلامتی صحیح انحراف = 0;
    ثابت bool absolute_راستہ = is_absolute_راستہ(stmt_file);
    اگر (absolute_راستہ) {
        /* Absolute path: no need for lookup in XKB paths */
        اگر (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            اگر (stmt_file_len < حجم(buf)) {
                حافظہ_نقل(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } ورنہ {
                log_err(m->ctx, XKB_ERROR_INVALID_راستہ,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, حجم(buf),
                        (غیرعلامتی صحیح) stmt_file_len, stmt_file);
                واپس;
            }
        } ورنہ {
            /* %-expansion is always NULL-terminated */
            أكد(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } ورنہ {
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
            file = تہی;
        } ورنہ {
            file = ڈھونڈو_File_اندر_Xkb_راستہ(m->ctx, والد_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_قواعد,
                                     buf, حجم(buf), &انحراف, true);
        }
    }

    جب (file) {
        أكد(strlen_محفوظ(buf) < حجم(buf));
        bool ret = read_قواعد_file(m->ctx, m, include_گہرائی + 1, file, buf);
        fclose(file);
        اگر (ret)
            واپس;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_پیغام_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        اگر (absolute_راستہ) {
            /* There is no point to search further if the path is absolute */
            توڑو;
        }

        /* Try next XKB path */
        انحراف++;
        file = ڈھونڈو_File_اندر_Xkb_راستہ(m->ctx, والد_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_قواعد,
                                 buf, حجم(buf), &انحراف, true);
    }

    log_err(m->ctx, XKB_LOG_پیغام_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (غیرعلامتی صحیح) stmt_file_len, stmt_file);
}

ساکن خالی
matcher_mapping_آغاز_new(ساخت matcher *m)
{
    برائے (mlvo_اشاریہ_t i = 0; i < (mlvo_اشاریہ_t) MLVO_عدد_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_عدد_ENTRIES;
    برائے (kccgst_اشاریہ_t i = 0; i < (kccgst_اشاریہ_t) KCCGST_عدد_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_عدد_ENTRIES;
    m->mapping.has_مضرب_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.عدد_mlvo = m->mapping.عدد_kccgst = 0;
    m->mapping.defined_mlvo_نقاب = 0;
    m->mapping.defined_kccgst_نقاب = 0;
    m->mapping.فعال = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
ساکن صحیح
parse_layout_int_اشاریہ_دم(ثابت حرف *s, سائز_t max_len,
                            xkb_layout_اشاریہ_t *برآمد)
{
    اگر (max_len < 3)
        واپس -1;
    uint32_t val = 0;
    ثابت صحیح گنتی = parse_dec_مقصد_uint32_t(&s[1], max_len - 2, &val);
    اگر (گنتی <= 0 || s[1 + گنتی] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        واپس -1;
    /* To zero-based index. */
    *برآمد = val - 1;
    واپس گنتی + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
ساکن صحیح
نکالو_layout_اشاریہ(ثابت حرف *s, سائز_t max_len, xkb_layout_اشاریہ_t *برآمد)
{
    /* This function is pretty stupid, but works for now. */
    *برآمد = XKB_LAYOUT_INVALID;
    اگر (max_len < 3 || s[0] != '[')
        واپس -1;
    اگر (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *برآمد = (xkb_layout_اشاریہ_t)(s[1] - '1');
        واپس 3;
    }
    اگر (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        واپس 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    واپس parse_layout_int_اشاریہ_دم(s, max_len, برآمد);
}

/* Special layout indices */
شمار layout_اشاریہ_ranges {
    LAYOUT_اشاریہ_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_اشاریہ_پہلا,
    LAYOUT_اشاریہ_LATER,
    LAYOUT_اشاریہ_مضرب,
    LAYOUT_اشاریہ_ANY,
};

static_assert((xkb_layout_اشاریہ_t) XKB_MAX_GROUPS <
              (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_SINGLE,
              "Cannot define special indices");
static_assert((xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_SINGLE <
              (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_پہلا &&
              (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_پہلا <
              (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_LATER &&
              (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_LATER <
              (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_مضرب &&
              (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_مضرب <
              (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_ANY &&
              (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_ANY <
              (xkb_layout_اشاریہ_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
ساکن bool
نکالو_mapping_layout_اشاریہ(ثابت حرف *s, سائز_t max_len,
                             xkb_layout_اشاریہ_t *برآمد)
{
    /* Check for minimal `[` + index + `]` */
    اگر (max_len < 3 || s[0] != '[') {
        *برآمد = XKB_LAYOUT_INVALID;
        واپس false;
    }

    /* Small numeric index */
    اگر (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *برآمد = (xkb_layout_اشاریہ_t)(s[1] - '1');
        واپس true;
    }

    /* Named indices ranges */
    ساکن ثابت ساخت {
        ثابت حرف* name;
        سائز_t سائز;
        xkb_layout_اشاریہ_t قدر;
    } نام[] = {
        #تعریف_کرو اشاریہ_RANGE(دم, قدر) \
            { (دم), حجم(دم), (xkb_layout_اشاریہ_t)(قدر) }
        اشاریہ_RANGE("single]",   LAYOUT_اشاریہ_SINGLE),
        اشاریہ_RANGE("first]",    LAYOUT_اشاریہ_پہلا),
        اشاریہ_RANGE("later]",    LAYOUT_اشاریہ_LATER),
        اشاریہ_RANGE("multiple]", LAYOUT_اشاریہ_مضرب),
        اشاریہ_RANGE("any]",      LAYOUT_اشاریہ_ANY),
        #تعریف_منسوخ اشاریہ_RANGE
    };

    برائے (سائز_t k = 0; k < ARRAY_سائز(نام); k++) {
        اگر (max_len == نام[k].سائز &&
            memcmp(&s[1], نام[k].name, نام[k].سائز - 1) == 0) {
            *برآمد = نام[k].قدر;
            واپس true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *برآمد = XKB_LAYOUT_INVALID;
    ثابت صحیح گنتی = parse_layout_int_اشاریہ_دم(s, max_len, برآمد);
    واپس گنتی > 0 && (سائز_t)گنتی == max_len;
}

ساکن اندرونی bool
is_mlvo_نقاب_defined(ساخت matcher *m, شمار قواعد_mlvo mlvo)
{
    واپس m->mapping.defined_mlvo_نقاب & (1u << mlvo);
}

ساکن خالی
matcher_mapping_سیٹ_mlvo(ساخت matcher *m, ساخت scanner *s,
                         ساخت sval ident)
{
    شمار قواعد_mlvo mlvo;
    ساخت sval mlvo_sval;

    برائے (mlvo = 0; mlvo < MLVO_عدد_ENTRIES; mlvo++) {
        mlvo_sval = قواعد_mlvo_svals[mlvo];

        اگر (svaleq_سابقہ(mlvo_sval, ident))
            توڑو;
    }

    /* Not found. */
    اگر (mlvo >= MLVO_عدد_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (غیرعلامتی صحیح) ident.len, ident.آغاز);
        m->mapping.فعال = false;
        واپس;
    }

    اگر (is_mlvo_نقاب_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (غیرعلامتی صحیح) mlvo_sval.len, mlvo_sval.آغاز);
        m->mapping.فعال = false;
        واپس;
    }

    /* If there are leftovers still, it must be an index. */
    اگر (mlvo_sval.len < ident.len) {
        xkb_layout_اشاریہ_t idx;
        اگر (!نکالو_mapping_layout_اشاریہ(ident.آغاز + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (غیرعلامتی صحیح) mlvo_sval.len, mlvo_sval.آغاز);
            m->mapping.فعال = false;
            واپس;
        }

        اگر (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        ورنہ اگر (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        ورنہ {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (غیرعلامتی صحیح) mlvo_sval.len, mlvo_sval.آغاز);
            m->mapping.فعال = false;
            واپس;
        }
    } ورنہ اگر (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_SINGLE;
    } ورنہ اگر (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    اگر (((mlvo == MLVO_LAYOUT && is_mlvo_نقاب_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_نقاب_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.فعال = false;
        واپس;
    }

    m->mapping.mlvo_at_pos[m->mapping.عدد_mlvo] = mlvo;
    m->mapping.defined_mlvo_نقاب |= (mlvo_نقاب_t) 1u << mlvo;
    m->mapping.عدد_mlvo++;
}

ساکن خالی
matcher_mapping_سیٹ_layout_bounds(ساخت matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_اشاریہ_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    انتخاب (idx) {
        حالت XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            أكد(!is_mlvo_نقاب_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_نقاب_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_نقاب = 0x1; /* active = true */
            توڑو;
        حالت LAYOUT_اشاریہ_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_سائز(m->rmlvo.layouts));
            m->mapping.layouts_candidates_نقاب = (xkb_layout_نقاب_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            توڑو;
        حالت LAYOUT_اشاریہ_مضرب:
        حالت LAYOUT_اشاریہ_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_سائز(m->rmlvo.layouts));
            m->mapping.layouts_candidates_نقاب = (xkb_layout_نقاب_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            توڑو;
        حالت LAYOUT_اشاریہ_SINGLE:
        حالت LAYOUT_اشاریہ_پہلا:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        طےشدہ:
            /* Mere layout index */
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = idx;
            m->mapping.layout_idx_max = idx + 1;
            m->mapping.layouts_candidates_نقاب = UINT32_C(1) << idx;
    }
    m->mapping.has_مضرب_layouts = (
        m->mapping.layout_idx_max > m->mapping.layout_idx_min &&
        m->mapping.layout_idx_max - m->mapping.layout_idx_min > 1
    );
}

ساکن خالی
matcher_mapping_سیٹ_kccgst(ساخت matcher *m, ساخت scanner *s, ساخت sval ident)
{
    شمار قواعد_kccgst kccgst;
    ساخت sval kccgst_sval;

    برائے (kccgst = 0; kccgst < KCCGST_عدد_ENTRIES; kccgst++) {
        kccgst_sval = قواعد_kccgst_svals[kccgst];

        اگر (svaleq(قواعد_kccgst_svals[kccgst], ident))
            توڑو;
    }

    /* Not found. */
    اگر (kccgst >= KCCGST_عدد_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (غیرعلامتی صحیح) ident.len, ident.آغاز);
        m->mapping.فعال = false;
        واپس;
    }

    اگر (m->mapping.defined_kccgst_نقاب & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (غیرعلامتی صحیح) kccgst_sval.len, kccgst_sval.آغاز);
        m->mapping.فعال = false;
        واپس;
    }

    m->mapping.kccgst_at_pos[m->mapping.عدد_kccgst] = kccgst;
    m->mapping.defined_kccgst_نقاب |= (kccgst_نقاب_t) 1u << kccgst;
    m->mapping.عدد_kccgst++;
}

ساکن bool
matcher_mapping_verify(ساخت matcher *m, ساخت scanner *s)
{
    اگر (m->mapping.عدد_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        جاؤ skip;
    }

    اگر (m->mapping.عدد_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        جاؤ skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    اگر (is_mlvo_نقاب_defined(m, MLVO_LAYOUT)) {
        أكد(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        انتخاب (m->mapping.layout_idx) {
            حالت LAYOUT_اشاریہ_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                اگر (darray_سائز(m->rmlvo.layouts) > 1)
                    جاؤ skip;
                توڑو;
            حالت LAYOUT_اشاریہ_مضرب:
            حالت LAYOUT_اشاریہ_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                اگر (darray_سائز(m->rmlvo.layouts) < 2)
                    جاؤ skip;
                توڑو;
            حالت LAYOUT_اشاریہ_ANY:
            حالت LAYOUT_اشاریہ_پہلا:
                /* No restrictions */
                توڑو;
            طےشدہ:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                اگر (darray_سائز(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_سائز(m->rmlvo.layouts))
                    جاؤ skip;
        }
    }

    اگر (is_mlvo_نقاب_defined(m, MLVO_VARIANT)) {
        أكد(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        انتخاب (m->mapping.variant_idx) {
            حالت LAYOUT_اشاریہ_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                اگر (darray_سائز(m->rmlvo.variants) > 1)
                    جاؤ skip;
                توڑو;
            حالت LAYOUT_اشاریہ_ANY:
            حالت LAYOUT_اشاریہ_LATER:
            حالت LAYOUT_اشاریہ_پہلا:
                /* No restriction */
                توڑو;
            طےشدہ:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                اگر (darray_سائز(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_سائز(m->rmlvo.variants))
                    جاؤ skip;
        }
    }

    واپس true;

skip:
    m->mapping.فعال = false;
    واپس false;
}

ساکن خالی
matcher_قاعدہ_آغاز_new(ساخت matcher *m)
{
    memset(&m->قاعدہ_2, 0, حجم(m->قاعدہ_2));
    m->قاعدہ_2.skip = !m->mapping.فعال;
}

ساکن خالی
matcher_قاعدہ_سیٹ_mlvo_common(ساخت matcher *m, ساخت scanner *s,
                             ساخت sval ident,
                             شمار mlvo_مطابقت_type مطابقت_type)
{
    اگر (m->قاعدہ_2.عدد_mlvo_اقدار >= m->mapping.عدد_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->قاعدہ_2.skip = true;
        واپس;
    }
    m->قاعدہ_2.مطابقت_type_at_pos[m->قاعدہ_2.عدد_mlvo_اقدار] = مطابقت_type;
    m->قاعدہ_2.mlvo_قدر_at_pos[m->قاعدہ_2.عدد_mlvo_اقدار] = ident;
    m->قاعدہ_2.عدد_mlvo_اقدار++;
}

ساکن خالی
matcher_قاعدہ_سیٹ_mlvo_wildcard(ساخت matcher *m, ساخت scanner *s,
                               شمار mlvo_مطابقت_type مطابقت_type)
{
    ساخت sval dummy = SVAL(تہی, 0);
    matcher_قاعدہ_سیٹ_mlvo_common(m, s, dummy, مطابقت_type);
}

ساکن خالی
matcher_قاعدہ_سیٹ_mlvo_گروہ(ساخت matcher *m, ساخت scanner *s,
                            ساخت sval ident)
{
    matcher_قاعدہ_سیٹ_mlvo_common(m, s, ident, MLVO_مطابقت_گروہ);
}

ساکن خالی
matcher_قاعدہ_سیٹ_mlvo(ساخت matcher *m, ساخت scanner *s,
                      ساخت sval ident)
{
    matcher_قاعدہ_سیٹ_mlvo_common(m, s, ident, MLVO_مطابقت_معمول);
}

ساکن خالی
matcher_قاعدہ_سیٹ_kccgst(ساخت matcher *m, ساخت scanner *s,
                        ساخت sval ident)
{
    اگر (m->قاعدہ_2.عدد_kccgst_اقدار >= m->mapping.عدد_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->قاعدہ_2.skip = true;
        واپس;
    }
    m->قاعدہ_2.kccgst_قدر_at_pos[m->قاعدہ_2.عدد_kccgst_اقدار] = ident;
    m->قاعدہ_2.عدد_kccgst_اقدار++;
}

ساکن bool
مطابقت_گروہ(ساخت matcher *m, ساخت sval گروہ_name, ساخت sval مقصد)
{
    ساخت گروہ *گروہ = تہی;
    ساخت sval *element;
    bool ملا = false;

    darray_foreach(گروہ, m->groups) {
        اگر (svaleq(گروہ->name, گروہ_name)) {
            ملا = true;
            توڑو;
        }
    }

    اگر (!ملا) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        واپس false;
    }

    darray_foreach(element, گروہ->elements)
        اگر (svaleq(مقصد, *element))
            واپس true;

    واپس false;
}

ساکن bool
مطابقت_قدر(ساخت matcher *m, ثابت ساخت sval val, ثابت ساخت sval مقصد,
            شمار mlvo_مطابقت_type مطابقت_type,
            شمار wildcard_مطابقت_type wildcard_type)
{
    انتخاب (مطابقت_type) {
        حالت MLVO_مطابقت_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            واپس wildcard_type == WILDCARD_مطابقت_ALL || !!مقصد.len;
        حالت MLVO_مطابقت_WILDCARD_NONE:
            واپس !مقصد.len;
        حالت MLVO_مطابقت_WILDCARD_SOME:
            واپس !!مقصد.len;
        حالت MLVO_مطابقت_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            واپس true;
        حالت MLVO_مطابقت_گروہ:
            واپس مطابقت_گروہ(m, val, مقصد);
        طےشدہ:
            أكد(مطابقت_type == MLVO_مطابقت_معمول);
            واپس svaleq(val, مقصد);
    }
}

ساکن bool
مطابقت_قدر_and_mark(ساخت matcher *m, ثابت ساخت sval val,
                     ساخت matched_sval *مقصد, شمار mlvo_مطابقت_type مطابقت_type,
                     شمار wildcard_مطابقت_type wildcard_type,
                     xkb_layout_نقاب_t layouts)
{
    bool matched = مطابقت_قدر(m, val, مقصد->sval, مطابقت_type, wildcard_type);
    اگر (matched)
        مقصد->matched |= layouts;
    واپس matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
ساکن bool
expand_rmlvo_اندر_kccgst_قدر(ساخت matcher *m, ساخت scanner *s,
                             ساخت sval قدر, xkb_layout_اشاریہ_t layout_idx,
                             darray_char *expanded, سائز_t *i)
{
    ثابت حرف *str = قدر.آغاز;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    شمار قواعد_mlvo mlv;
    xkb_layout_اشاریہ_t idx;
    حرف pfx, sfx;
    ساخت matched_sval *expanded_قدر;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    اگر (str[*i] == 'i' &&
        (*i + 1 == قدر.len || is_ملاؤ_منوال_سابقہ(str[*i + 1])))
    {
        اگر (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_قواعد_INVALID_LAYOUT_اشاریہ_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            جاؤ error;
        }
        (*i)++;
        حرف اشاریہ_str[MAX_LAYOUT_اشاریہ_STR_لمبائی + 1];
        ثابت صحیح گنتی = snprintf(اشاریہ_str, حجم(اشاریہ_str), "%"PRIu32,
                                   layout_idx + 1);
        أكد(گنتی > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, اشاریہ_str, (darray_سائز_t)گنتی);
        واپس true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    اگر (str[*i] == '(' ||
        is_ملاؤ_منوال_سابقہ(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        اگر (str[*i] == '(') sfx = ')';
        اگر (++(*i) >= قدر.len) جاؤ error;
    }

    /* Mandatory model/layout/variant specifier. */
    انتخاب (str[(*i)++]) {
    حالت 'm': mlv = MLVO_MODEL; توڑو;
    حالت 'l': mlv = MLVO_LAYOUT; توڑو;
    حالت 'v': mlv = MLVO_VARIANT; توڑو;
    طےشدہ: جاؤ error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_اشاریہ = false;
    اگر (*i < قدر.len && str[*i] == '[') {
        اگر (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            جاؤ error;
        }

        صحیح consumed = نکالو_layout_اشاریہ(str + (*i), قدر.len - (*i), &idx);
        اگر (consumed <= 0) جاؤ error;
        اگر (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            أكد(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_اشاریہ = true;
        }
        *i += (سائز_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    اگر (sfx != 0) {
        اگر (*i >= قدر.len) جاؤ error;
        اگر (str[(*i)++] != sfx) جاؤ error;
    }

    /* Get the expanded value. */
    expanded_قدر = تہی;

    اگر (mlv == MLVO_LAYOUT) {
        اگر (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            اگر (darray_سائز(m->rmlvo.layouts) == 1)
                expanded_قدر = &darray_شے(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } ورنہ اگر (idx < darray_سائز(m->rmlvo.layouts) &&
                   (variable_اشاریہ || darray_سائز(m->rmlvo.layouts) > 1)) {
                expanded_قدر = &darray_شے(m->rmlvo.layouts, idx);
        }
    }
    ورنہ اگر (mlv == MLVO_VARIANT) {
        اگر (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            اگر (darray_سائز(m->rmlvo.variants) == 1)
                expanded_قدر = &darray_شے(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } ورنہ اگر (idx < darray_سائز(m->rmlvo.variants) &&
                   (variable_اشاریہ || darray_سائز(m->rmlvo.variants) > 1)) {
                expanded_قدر = &darray_شے(m->rmlvo.variants, idx);
        }
    }
    ورنہ اگر (mlv == MLVO_MODEL) {
        expanded_قدر = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    اگر (!expanded_قدر || expanded_قدر->sval.len == 0) {
        واپس true;
    }

    اگر (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_قدر->sval.آغاز,
                                 (darray_سائز_t) expanded_قدر->sval.len);
    اگر (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    اگر (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ثابت xkb_layout_نقاب_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_قدر->matched |= layouts;
    } ورنہ {
        expanded_قدر->matched |= (xkb_layout_نقاب_t)GLOBAL_MATCHED_LAYOUTS;
    }

    واپس true;

error:
    scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                "invalid %%-expansion in value; not used");
    واپس false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
ساکن خالی
expand_qualifier_اندر_kccgst_قدر(
    ساخت matcher *m, ساخت scanner *s,
    ساخت sval قدر, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_سائز_t سابقہ_idx, سائز_t *i)
{
    ثابت حرف *str = قدر.آغاز;

    /* “all” followed by nothing or by a layout separator */
    اگر (*i + 2 < قدر.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == قدر.len || is_ملاؤ_منوال_سابقہ(str[*i + 3]))) {
        اگر (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_پیغام_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        اگر (darray_سائز(m->rmlvo.layouts) > 1) {
            حرف layout_اشاریہ[MAX_LAYOUT_اشاریہ_STR_لمبائی + 1];
            ثابت darray_سائز_t سابقہ_لمبائی =
                darray_سائز(*expanded) - سابقہ_idx - 1;
            برائے (xkb_layout_اشاریہ_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_سائز(m->rmlvo.layouts));
                 l++)
            {
                اگر (!has_separator)
                    darray_append(*expanded, ملاؤ_DEFAULT_سابقہ);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_شے(*expanded, سابقہ_idx),
                                             سابقہ_لمبائی);
                /* Append index */
                ثابت صحیح گنتی = snprintf(layout_اشاریہ, حجم(layout_اشاریہ),
                                           "%"PRIu32, l + 1);
                أكد(گنتی > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_اشاریہ,
                                             (darray_سائز_t)گنتی);
            }
        }
        *i += 3;
    }
}

ساکن اندرونی خالی
#اگر_تعریف_شدہ _MSC_VER
concat_kccgst(darray_char *into, darray_سائز_t سائز, اندر_reads(سائز) ثابت حرف* from)
#ورنہ
concat_kccgst(darray_char *into, darray_سائز_t سائز, ثابت حرف from[ساکن سائز])
#اگر_ختم
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ثابت bool from_plus = is_ملاؤ_منوال_سابقہ(from[0]);
    اگر (from_plus || darray_خالی_خانے(*into)) {
        darray_appends_nullterminate(*into, from, سائز);
    } ورنہ {
        ثابت حرف ch =
            (حرف) (darray_خالی_خانے(*into) ? '\0' : darray_شے(*into, 0));
        ثابت bool into_plus = is_ملاؤ_منوال_سابقہ(ch);
        اگر (into_plus)
            darray_prepends_nullterminate(*into, from, سائز);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
ساکن bool
append_expanded_kccgst_قدر(ساخت matcher *m, ساخت scanner *s,
                             bool ملاؤ, darray_char *مقصد, ساخت sval قدر,
                             xkb_layout_اشاریہ_t layout_idx)
{
    ثابت حرف *str = قدر.آغاز;
    darray_char expanded = darray_new();
    darray_سائز_t آخری_شے_idx = 0;
    bool has_separator = false;

    برائے (سائز_t i = 0; i < قدر.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        انتخاب (str[i]) {
            /* Qualifier */
            حالت ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_اندر_kccgst_قدر(m, s, قدر, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 آخری_شے_idx, &i);
                توڑو;
            /* Expansion */
            حالت '%':
                i++;
                اگر (i >= قدر.len ||
                    !expand_rmlvo_اندر_kccgst_قدر(m, s, قدر, layout_idx,
                                                  &expanded, &i))
                        جاؤ error;
                توڑو;
            /* New item */
            حالت ملاؤ_OVERRIDE_سابقہ:
            حالت ملاؤ_AUGMENT_سابقہ:
            حالت ملاؤ_REPLACE_سابقہ:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                آخری_شے_idx = darray_سائز(expanded) - 1;
                has_separator = true;
                توڑو;
            /* Just a normal character. */
            طےشدہ:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    اگر (ملاؤ) {
        اگر (!darray_خالی_خانے(expanded))
            concat_kccgst(مقصد, darray_سائز(expanded), darray_اشیاء(expanded));
    } ورنہ {
        darray_concat(*مقصد, expanded);
    }
    darray_free(expanded);
    واپس true;

error:
    darray_free(expanded);
    واپس false;
}

ساکن bool
matcher_append_pending_kccgst(ساخت matcher *m)
{
    اگر (!m->mapping.has_مضرب_layouts)
        واپس true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    برائے (kccgst_اشاریہ_t i = 0; i < m->mapping.عدد_kccgst; i++) {
        ثابت شمار قواعد_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        برائے (xkb_layout_اشاریہ_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            اندراج ثابت ساخت kccgst_بفر* ثابت buf = &m->pending_kccgst;
            سائز_t انحراف = 0;
            برائے (darray_سائز_t k = 0; k < darray_سائز(buf->slices); k++) {
                اندراج ثابت ساخت kccgst_بفر_قاش * ثابت قاش =
                    &darray_شے(buf->slices, k);
                اگر (قاش->kccgst == kccgst && قاش->layout == layout &&
                    قاش->لمبائی)
                    concat_kccgst(&m->kccgst[kccgst], قاش->لمبائی,
                                  darray_اشیاء(buf->بفر) + انحراف);
                انحراف += قاش->لمبائی;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_مضرب_layouts = false;
    واپس true;
}

ساکن خالی
matcher_قاعدہ_verify(ساخت matcher *m, ساخت scanner *s)
{
    اگر (m->قاعدہ_2.عدد_mlvo_اقدار != m->mapping.عدد_mlvo ||
        m->قاعدہ_2.عدد_kccgst_اقدار != m->mapping.عدد_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid rule: must have same number of values "
                    "as mapping line; ignoring rule");
        m->قاعدہ_2.skip = true;
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
ساکن bool
matcher_قاعدہ_مطابقت_model(ساخت matcher *m, ثابت ساخت sval قدر,
                         شمار mlvo_مطابقت_type مطابقت_type)
{
    واپس مطابقت_قدر_and_mark(
        m, قدر, &m->rmlvo.model, مطابقت_type, WILDCARD_مطابقت_ALL,
        /* layout-independent */
        (xkb_layout_نقاب_t)GLOBAL_MATCHED_LAYOUTS
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
ساکن bool
matcher_قاعدہ_مطابقت_option(ساخت matcher *m, ثابت ساخت sval قدر,
                          شمار mlvo_مطابقت_type مطابقت_type,
                          xkb_layout_نقاب_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_خالی_خانے(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_نقاب_t unmatched =
        (m->mapping.defined_mlvo_نقاب & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    ساخت matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        ثابت xkb_layout_نقاب_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_نقاب_t)GLOBAL_MATCHED_LAYOUTS;
        اگر (matchable && مطابقت_قدر_and_mark(m, قدر, option, مطابقت_type,
                                              WILDCARD_مطابقت_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            اگر (!unmatched)
                توڑو;
        }
    }
    اگر (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    واپس matched;
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
ساکن bool
matcher_قاعدہ_مطابقت_layout_or_variant(ساخت matcher *m,
                                     darray_matched_sval *داخلہ,
                                     ثابت ساخت sval قدر,
                                     شمار mlvo_مطابقت_type مطابقت_type,
                                     xkb_layout_نقاب_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_خالی_خانے(*داخلہ));

    bool matched = false;
    /* Loop over the layout index range */
    برائے (xkb_layout_اشاریہ_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ثابت xkb_layout_نقاب_t layout = (UINT32_C(1) << idx);
        اگر (layout & *candidates) {
            ساخت matched_sval *مقصد = &darray_شے(*داخلہ, idx);
            اگر (مطابقت_قدر_and_mark(m, قدر, مقصد, مطابقت_type,
                                     WILDCARD_مطابقت_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } ورنہ {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    واپس matched;
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
ساکن خالی
matcher_قاعدہ_apply_if_matches(ساخت matcher *m, ساخت scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_نقاب_t candidate_layouts = m->mapping.layouts_candidates_نقاب;

    /* Loop over MLVO pattern components */
    برائے (mlvo_اشاریہ_t i = 0; i < m->mapping.عدد_mlvo; i++) {
        ثابت شمار قواعد_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ثابت ساخت sval قدر = m->قاعدہ_2.mlvo_قدر_at_pos[i];
        ثابت شمار mlvo_مطابقت_type مطابقت_type = m->قاعدہ_2.مطابقت_type_at_pos[i];
        bool matched = false;

        انتخاب (mlvo) {
        حالت MLVO_MODEL:
            matched = matcher_قاعدہ_مطابقت_model(m, قدر, مطابقت_type);
            توڑو;
        حالت MLVO_LAYOUT:
            matched = matcher_قاعدہ_مطابقت_layout_or_variant(
                m, &m->rmlvo.layouts, قدر, مطابقت_type, &candidate_layouts
            );
            توڑو;
        حالت MLVO_VARIANT:
            matched = matcher_قاعدہ_مطابقت_layout_or_variant(
                m, &m->rmlvo.variants, قدر, مطابقت_type, &candidate_layouts
            );
            توڑو;
        حالت MLVO_OPTION:
            matched = matcher_قاعدہ_مطابقت_option(
                m, قدر, مطابقت_type, &candidate_layouts
            );
            توڑو;
        طےشدہ: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_عدد_ENTRIES - 1,
                          "Unexpected MLVO field");
            أكد(!"Unreachable");
        }
        }

        اگر (!matched)
            واپس;
    }

    اگر (m->mapping.has_مضرب_layouts) {
        /* Special index: loop over the index range */
        برائے (xkb_layout_اشاریہ_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            اگر (candidate_layouts & (UINT32_C(1) << idx)) {
                برائے (kccgst_اشاریہ_t i = 0; i < m->mapping.عدد_kccgst; i++) {
                    ثابت شمار قواعد_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ثابت ساخت sval قدر = m->قاعدہ_2.kccgst_قدر_at_pos[i];
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
                    ساخت kccgst_بفر * ثابت buf = &m->pending_kccgst;
                    ثابت darray_سائز_t پچھلا_بفر_لمبائی =
                        darray_سائز(buf->بفر);
                    append_expanded_kccgst_قدر(m, s, false, &buf->بفر,
                                                 قدر, idx);
                    ثابت uint32_t لمبائی = (uint32_t) (darray_سائز(buf->بفر)
                                          - پچھلا_بفر_لمبائی);
                    ثابت ساخت kccgst_بفر_قاش قاش = {
                        .لمبائی = لمبائی,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, قاش);
                }
            }
        }
    } ورنہ {
        /* Numeric index or no index */
        برائے (kccgst_اشاریہ_t i = 0; i < m->mapping.عدد_kccgst; i++) {
            شمار قواعد_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            ساخت sval قدر = m->قاعدہ_2.kccgst_قدر_at_pos[i];
            append_expanded_kccgst_قدر(m, s, true, &m->kccgst[kccgst], قدر,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    اگر (!(is_mlvo_نقاب_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_نقاب &= ~candidate_layouts;
    }
}

ساکن شمار قواعد_token
gettok(ساخت matcher *m, ساخت scanner *s)
{
    واپس lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
ساکن bool
matcher_مطابقت(ساخت matcher *m, ساخت scanner *s,
              غیرعلامتی صحیح include_گہرائی,
              ثابت حرف *string, سائز_t len,
              ثابت حرف *file_name)
{
    شمار قواعد_token tok;

    اگر (!m)
        واپس false;

initial:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_BANG:
        جاؤ bang;
    حالت TOK_انتہا_کا_سطر:
        جاؤ initial;
    حالت TOK_انتہا_کا_FILE:
        جاؤ اختتام;
    طےشدہ:
        جاؤ unexpected;
    }

bang:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_گروہ_NAME:
        matcher_گروہ_آغاز_new(m, m->val.string);
        جاؤ گروہ_name;
    حالت TOK_INCLUDE:
        جاؤ include_statement;
    حالت TOK_IDENTIFIER:
        matcher_mapping_آغاز_new(m);
        matcher_mapping_سیٹ_mlvo(m, s, m->val.string);
        جاؤ mapping_mlvo;
    طےشدہ:
        جاؤ unexpected;
    }

گروہ_name:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_EQUALS:
        جاؤ گروہ_element;
    طےشدہ:
        جاؤ unexpected;
    }

گروہ_element:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_IDENTIFIER:
        matcher_گروہ_جوڑو_element(m, s, m->val.string);
        جاؤ گروہ_element;
    حالت TOK_انتہا_کا_سطر:
        جاؤ initial;
    طےشدہ:
        جاؤ unexpected;
    }

include_statement:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_IDENTIFIER:
        matcher_include(m, s, include_گہرائی, m->val.string);
        جاؤ include_statement_انتہا;
    طےشدہ:
        جاؤ unexpected;
    }

include_statement_انتہا:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_انتہا_کا_سطر:
        جاؤ initial;
    طےشدہ:
        جاؤ unexpected;
    }

mapping_mlvo:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_IDENTIFIER:
        اگر (m->mapping.فعال)
            matcher_mapping_سیٹ_mlvo(m, s, m->val.string);
        جاؤ mapping_mlvo;
    حالت TOK_EQUALS:
        جاؤ mapping_kccgst;
    طےشدہ:
        جاؤ unexpected;
    }

mapping_kccgst:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_IDENTIFIER:
        اگر (m->mapping.فعال)
            matcher_mapping_سیٹ_kccgst(m, s, m->val.string);
        جاؤ mapping_kccgst;
    حالت TOK_انتہا_کا_سطر:
        اگر (m->mapping.فعال && matcher_mapping_verify(m, s)) {
            matcher_mapping_سیٹ_layout_bounds(m);
            اگر (m->mapping.has_مضرب_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_سائز(m->pending_kccgst.بفر) = 0;
                darray_سائز(m->pending_kccgst.slices) = 0;
            }
        }
        جاؤ قاعدہ_mlvo_پہلا;
    طےشدہ:
        جاؤ unexpected;
    }

قاعدہ_mlvo_پہلا:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_BANG:
        matcher_append_pending_kccgst(m);
        جاؤ bang;
    حالت TOK_انتہا_کا_سطر:
        جاؤ قاعدہ_mlvo_پہلا;
    حالت TOK_انتہا_کا_FILE:
        matcher_append_pending_kccgst(m);
        جاؤ اختتام;
    طےشدہ:
        matcher_قاعدہ_آغاز_new(m);
        جاؤ قاعدہ_mlvo_no_tok;
    }

قاعدہ_mlvo:
    tok = gettok(m, s);
قاعدہ_mlvo_no_tok:
    انتخاب (tok) {
    حالت TOK_IDENTIFIER:
        اگر (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo(m, s, m->val.string);
        جاؤ قاعدہ_mlvo;
    حالت TOK_WILD_CARD_ستارہ:
        اگر (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo_wildcard(m, s, MLVO_مطابقت_WILDCARD_LEGACY);
        جاؤ قاعدہ_mlvo;
    حالت TOK_WILD_CARD_NONE:
        اگر (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo_wildcard(m, s, MLVO_مطابقت_WILDCARD_NONE);
        جاؤ قاعدہ_mlvo;
    حالت TOK_WILD_CARD_SOME:
        اگر (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo_wildcard(m, s, MLVO_مطابقت_WILDCARD_SOME);
        جاؤ قاعدہ_mlvo;
    حالت TOK_WILD_CARD_ANY:
        اگر (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo_wildcard(m, s, MLVO_مطابقت_WILDCARD_ANY);
        جاؤ قاعدہ_mlvo;
    حالت TOK_گروہ_NAME:
        اگر (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo_گروہ(m, s, m->val.string);
        جاؤ قاعدہ_mlvo;
    حالت TOK_EQUALS:
        جاؤ قاعدہ_kccgst;
    طےشدہ:
        جاؤ unexpected;
    }

قاعدہ_kccgst:
    انتخاب (tok = gettok(m, s)) {
    حالت TOK_IDENTIFIER:
        اگر (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_kccgst(m, s, m->val.string);
        جاؤ قاعدہ_kccgst;
    حالت TOK_انتہا_کا_سطر:
        اگر (!m->قاعدہ_2.skip)
            matcher_قاعدہ_verify(m, s);
        اگر (!m->قاعدہ_2.skip)
            matcher_قاعدہ_apply_if_matches(m, s);
        جاؤ قاعدہ_mlvo_پہلا;
    طےشدہ:
        جاؤ unexpected;
    }

unexpected:
    انتخاب (tok) {
    حالت TOK_ERROR:
        جاؤ error;
    طےشدہ:
        جاؤ state_error;
    }

اختتام:
    واپس true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    واپس false;
}
// NOLINTEND(readability-trivial-switch)

ساکن bool
read_قواعد_file(ساخت xkb_context *ctx,
                ساخت matcher *matcher,
                غیرعلامتی صحیح include_گہرائی,
                FILE *file,
                ثابت حرف *راستہ)
{
    bool ret;
    حرف *string;
    سائز_t سائز;
    ساخت scanner scanner;

    اگر (!map_file(file, &string, &سائز)) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                راستہ, strerror(errno));
        واپس false;
    }

    scanner_init(&scanner, matcher->ctx, string, سائز, راستہ, تہی);

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
        unmap_file(string, سائز);
        واپس false;
    }

    ret = matcher_مطابقت(matcher, &scanner, include_گہرائی, string, سائز, راستہ);
    unmap_file(string, سائز);
    واپس ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
ساکن bool
xkb_resolve_partial_قواعد(ساخت xkb_context *ctx, حرف *راستہ, سائز_t راستہ_سائز,
                          ثابت حرف* قواعد, ثابت حرف* suffix,
                          ساخت matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    حرف partial_قواعد[60]; /* Arbitrary, but we do not expect long names */
    اگر (unlikely(!snprintf_محفوظ(partial_قواعد, حجم(partial_قواعد),
                                "%s%s", قواعد, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", قواعد, suffix);
        واپس false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    غیرعلامتی صحیح انحراف = 0;
    FILE *file = تہی;
    ثابت سائز_t len = لڑی_لمبائی(partial_قواعد);
    جب ((file = ڈھونڈو_File_اندر_Xkb_راستہ(ctx, "(unknown)",
                                     partial_قواعد, len, FILE_TYPE_قواعد,
                                     راستہ, راستہ_سائز, &انحراف, false)) != تہی) {
        ثابت bool ok = read_قواعد_file(ctx, matcher, 0, file, راستہ);
        fclose(file);
        اگر (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", راستہ);
            واپس false;
        }
        انحراف++;
    }

    واپس true;
}

ساکن bool
xkb_resolve_قواعد(ساخت xkb_context *ctx,
                  ثابت حرف* قواعد, ساخت matcher *matcher,
                  ساخت xkb_component_نام *برآمد,
                  xkb_layout_اشاریہ_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    غیرعلامتی صحیح انحراف = 0;
    حرف راستہ[PATH_MAX];
    FILE * ثابت file = ڈھونڈو_File_اندر_Xkb_راستہ(ctx, "(unknown)",
                                          قواعد, لڑی_لمبائی(قواعد), FILE_TYPE_قواعد,
                                          راستہ, حجم(راستہ), &انحراف, true);
    اگر (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", قواعد);
        جاؤ err_برآمد;
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
    ret = xkb_resolve_partial_قواعد(ctx, راستہ, حجم(راستہ),
                                    قواعد, ".pre", matcher);
    اگر (!ret)
        جاؤ err_برآمد;

    /* Resolve main <rules> file */
    ret = read_قواعد_file(ctx, matcher, 0, file, راستہ);
    اگر (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", راستہ);
        جاؤ err_برآمد;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_قواعد(ctx, راستہ, حجم(راستہ),
                                    قواعد, ".post", matcher);
    اگر (!ret)
        جاؤ err_برآمد;

    /* Check we got required components */
    اگر (darray_خالی_خانے(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_خالی_خانے(matcher->kccgst[KCCGST_TYPES]) ||
        darray_خالی_خانے(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_خالی_خانے(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", قواعد);
        ret = false;
        جاؤ err_برآمد;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &برآمد->keycodes, تہی);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &برآمد->types, تہی);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &برآمد->compatibility, تہی);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &برآمد->symbols, تہی);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &برآمد->geometry, تہی);

    ساخت matched_sval *mval = &matcher->rmlvo.model;
    اگر (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (غیرعلامتی صحیح) mval->sval.len, mval->sval.آغاز, قواعد);
    darray_foreach(mval, matcher->rmlvo.layouts)
        اگر (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (غیرعلامتی صحیح) mval->sval.len, mval->sval.آغاز, قواعد);
    darray_foreach(mval, matcher->rmlvo.variants)
        اگر (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (غیرعلامتی صحیح) mval->sval.len, mval->sval.آغاز, قواعد);
    darray_foreach(mval, matcher->rmlvo.options) {
        اگر (!mval->sval.len)
            جاری;

        اگر (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (غیرعلامتی صحیح) mval->sval.len, mval->sval.آغاز, قواعد);
        } ورنہ اگر (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ثابت xkb_layout_نقاب_t unmatched = (mval->layouts & ~mval->matched);
            أكد(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (غیرعلامتی صحیح)mval->sval.len, mval->sval.آغاز,
                    unmatched, قواعد);
        }
    }

    /* Set the number of explicit layouts */
    اگر (برآمد->symbols != تہی && explicit_layouts != تہی) {
        *explicit_layouts = 1; /* at least one group */
        ثابت حرف *symbols = برآمد->symbols;
        /* Take the highest modifier */
        جب ((symbols = strchr(symbols, ':')) != تہی && symbols[1] != '\0') {
            xkb_layout_اشاریہ_t گروہ = 0;
            ثابت صحیح گنتی = parse_dec_مقصد_uint32_t(++symbols, SIZE_MAX, &گروہ);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            اگر (گنتی > 0 && (symbols[گنتی] == '\0' ||
                is_ملاؤ_منوال_سابقہ(symbols[گنتی])) &&
                گروہ > 0 && گروہ <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, گروہ);
                symbols += گنتی;
            }
        }
    }

err_برآمد:
    اگر (file)
        fclose(file);

    واپس ret;
}

bool
xkb_components_from_rmlvo_builder(ثابت ساخت xkb_rmlvo_builder *rmlvo,
                                  ساخت xkb_component_نام *برآمد,
                                  xkb_layout_اشاریہ_t *explicit_layouts)
{
    ثابت حرف *قواعد = rmlvo->قواعد;
    ساخت matcher *matcher = matcher_new_from_rmlvo(rmlvo, &قواعد);
    اگر (!matcher)
        واپس false;

    ثابت bool ret = xkb_resolve_قواعد(rmlvo->ctx, قواعد, matcher,
                                       برآمد, explicit_layouts);

    matcher_free(matcher);
    واپس ret;
}


bool
xkb_components_from_قواعد_نام(ساخت xkb_context *ctx,
                                ثابت ساخت xkb_قاعدہ_نام *rmlvo,
                                ساخت xkb_component_نام *برآمد,
                                xkb_layout_اشاریہ_t *explicit_layouts)
{
    ساخت matcher *matcher = matcher_new_from_نام(ctx, rmlvo);
    اگر (!matcher)
        واپس false;

    ثابت bool ret = xkb_resolve_قواعد(ctx, rmlvo->قواعد, matcher,
                                       برآمد, explicit_layouts);

    matcher_free(matcher);
    واپس ret;
}
