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

#تعریف_کن MAX_INCLUDE_عمق 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
اتحاد lvalue {
    بنية sval string;
};

تعداد قواعد_token {
    TOK_انتها_از_FILE = 0,
    TOK_انتها_از_سطر,
    TOK_IDENTIFIER,
    TOK_گروه_NAME,
    TOK_BANG,
    TOK_EQUALS,
    TOK_WILD_CARD_ستاره,
    TOK_WILD_CARD_NONE,
    TOK_WILD_CARD_SOME,
    TOK_WILD_CARD_ANY,
    TOK_INCLUDE,
    TOK_ERROR
};

ساكن تعداد قواعد_token
lex(بنية scanner *s, اتحاد lvalue *val)
{
skip_بیشتر_whitespace_and_comments:
    /* Skip spaces */
    طالما (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    إذا (scanner_lit(s, "//")) {
        scanner_skip_مقصد_eol(s);
    }

    /* New line */
    إذا (scanner_eol(s)) {
        طالما (scanner_eol(s)) scanner_بعدی(s);
        أرجع TOK_انتها_از_سطر;
    }

    /* Escaped line continuation */
    إذا (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        إذا (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            أرجع TOK_ERROR;
        }
        scanner_بعدی(s);
        اذهب skip_بیشتر_whitespace_and_comments;
    }

    /* See if we're done */
    إذا (scanner_eof(s)) أرجع TOK_انتها_از_FILE;

    /* New token */
    s->token_pos = s->pos;
    تعداد قواعد_token token = TOK_IDENTIFIER;

    ثابت حرف بعدی = scanner_بعدی(s);
    بدل (بعدی) {
    حالة '!':
        أرجع TOK_BANG;
    حالة '$':
        /*گروه*/
        token = TOK_گروه_NAME;
        val->string.len = 0;
        اكسر;
    حالة '*':
        إذا (scanner_قواعد_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            اكسر;
        } وإلا {
            /* Legacy wild card */
            أرجع TOK_WILD_CARD_ستاره;
        }
    حالة '<':
        /* Extended wild cards */
        إذا (scanner_قواعد_lit_token(s, "none>")) أرجع TOK_WILD_CARD_NONE;
        إذا (scanner_قواعد_lit_token(s, "some>")) أرجع TOK_WILD_CARD_SOME;
        إذا (scanner_قواعد_lit_token(s, "any>")) أرجع TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        أكد(scanner_قواعد_is_ident('<'));
        val->string.len = 1;
        اكسر;
    حالة '=':
        إذا (scanner_قواعد_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            اكسر;
        } وإلا {
            أرجع TOK_EQUALS;
        }
    حالة 'i':
        إذا (scanner_قواعد_lit_token(s, "nclude")) {
            /* Include statement */
            أرجع TOK_INCLUDE;
        }
        /* Identifier */
        أكد(scanner_قواعد_is_ident('i'));
        val->string.len = 1;
        اكسر;
    افتراضي:
        إذا (likely(scanner_قواعد_is_ident(بعدی))) {
            /* Identifier */
            val->string.len = 1;
        } وإلا {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.شروع = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    أكد(scanner_قواعد_is_ident(ادغام_OVERRIDE_پیشوند));
    أكد(scanner_قواعد_is_ident(ادغام_AUGMENT_پیشوند));
    أكد(scanner_قواعد_is_ident(ادغام_REPLACE_پیشوند));
    أكد(scanner_قواعد_is_ident('('));
    أكد(scanner_قواعد_is_ident(')'));
    val->string.len += scanner_قواعد_ident_token(s);

    إذا (unlikely(val->string.len == 0)) {
        إذا (token == TOK_گروه_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } وإلا {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "unrecognized token");
        }
        أرجع TOK_ERROR;
    }

    أرجع token;
}

/***====================================================================***/

تعداد قواعد_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_رقم_ENTRIES
};

تعداد {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

تعريفنوع uint8_t mlvo_شاخص_t;
تعريفنوع uint8_t mlvo_ماسک_t;


ساكن ثابت بنية sval قواعد_mlvo_svals[MLVO_رقم_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

تعداد قواعد_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_رقم_ENTRIES
};

تعريفنوع uint8_t kccgst_شاخص_t;
تعريفنوع uint8_t kccgst_ماسک_t;

ساكن ثابت بنية sval قواعد_kccgst_svals[KCCGST_رقم_ENTRIES] = {
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

static_assert(~(xkb_layout_ماسک_t)XKB_OPTION_LAYOUT_ماسک_GLOBAL ==
              (xkb_layout_ماسک_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
بنية matched_sval {
    بنية sval sval;
    /** Used for layout-specific options */
    xkb_layout_ماسک_t layouts;
    xkb_layout_ماسک_t matched;
};
تعريفنوع darray(بنية matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
بنية قاعده_نامها {
    بنية matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

بنية گروه {
    بنية sval name;
    darray_sval elements;
};

بنية mapping {
    تعداد قواعد_mlvo mlvo_at_pos[MLVO_رقم_ENTRIES];
    mlvo_شاخص_t رقم_mlvo;
    mlvo_ماسک_t defined_mlvo_ماسک;
    bool has_layout_idx_range;
    bool has_مضرب_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    اتحاد {
        بنية { xkb_layout_شاخص_t layout_idx, variant_idx; };
        بنية { xkb_layout_شاخص_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    اتحاد {
        xkb_layout_ماسک_t فعال;
        xkb_layout_ماسک_t layouts_candidates_ماسک;
    };
    تعداد قواعد_kccgst kccgst_at_pos[KCCGST_رقم_ENTRIES];
    kccgst_شاخص_t رقم_kccgst;
    kccgst_ماسک_t defined_kccgst_ماسک;
};

تعداد mlvo_تطابق_type {
    /** Match the given plain value */
    MLVO_تطابق_نرمال = 0,
    /** Match depending on the value of `wildcard_match_type` */
    MLVO_تطابق_WILDCARD_LEGACY,
    /** Match empty value */
    MLVO_تطابق_WILDCARD_NONE,
    /** Match non-empty value */
    MLVO_تطابق_WILDCARD_SOME,
    /** Match any value, optionally empty */
    MLVO_تطابق_WILDCARD_ANY,
    /** Match any entry in a group */
    MLVO_تطابق_گروه,
};

تعداد wildcard_تطابق_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_تطابق_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_تطابق_ALL,
};

بنية قاعده_2 {
    بنية sval mlvo_مقدار_at_pos[MLVO_رقم_ENTRIES];
    تعداد mlvo_تطابق_type تطابق_type_at_pos[MLVO_رقم_ENTRIES];
    بنية sval kccgst_مقدار_at_pos[KCCGST_رقم_ENTRIES];
    mlvo_شاخص_t رقم_mlvo_مقادیر;
    kccgst_شاخص_t رقم_kccgst_مقادیر;
    bool skip;
};

#تعریف_کن برش_KCCGST_بیت_فیلد_اندازه 4
static_assert(!((1u << (برش_KCCGST_بیت_فیلد_اندازه - 1)) &
                (KCCGST_رقم_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
بنية kccgst_میانگیر_برش {
    uint32_t درازی:(32 - برش_KCCGST_بیت_فیلد_اندازه);
    تعداد قواعد_kccgst kccgst:برش_KCCGST_بیت_فیلد_اندازه;
    xkb_layout_شاخص_t layout;
};

/* Buffer for pending KcCGST values */
بنية kccgst_میانگیر {
    darray_char میانگیر;
    /* Slice corresponding to each value in the buffer */
    darray(بنية kccgst_میانگیر_برش) slices;
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
    بنية قاعده_نامها rmlvo;
    اتحاد lvalue val;
    darray(بنية گروه) groups;
    /* Current mapping. */
    بنية mapping mapping;
    /* Current rule. */
    بنية قاعده_2 قاعده_2;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    بنية kccgst_میانگیر pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_رقم_ENTRIES];
};

ساكن بنية sval
strip_spaces(بنية sval v)
{
    طالما (v.len > 0 && is_space(v.شروع[0])) { v.len--; v.شروع++; }
    طالما (v.len > 0 && is_space(v.شروع[v.len - 1])) v.len--;
    أرجع v;
}

ساكن darray_matched_sval
تقسیم_comma_separated_mlvo(بنية xkb_context *ctx,
                           تعداد قواعد_mlvo mlvo, ثابت حرف *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    إذا (!s) {
        ensure_at_کمترین_one_مقدار:
        /* Label followed by a declaration is a C23 extension */ ;

        بنية matched_sval val = {
            .sval = SVAL(عدم, 0),
            .layouts = (xkb_layout_ماسک_t)XKB_OPTION_LAYOUT_ماسک_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        أرجع arr;
    }

    طالما (true) {
        بنية matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_ماسک_t)XKB_OPTION_LAYOUT_ماسک_GLOBAL,
            .matched = 0,
        };
        طالما (*s != '\0' && *s != ',' && *s != OPTIONS_گروه_SPECIFIER_پیشوند) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        إذا (*s == OPTIONS_گروه_SPECIFIER_پیشوند) {
            /* Handle numeric layout index */
            s++;
            ثابت حرف* ثابت layout_شروع = s;
            xkb_layout_شاخص_t layout = XKB_LAYOUT_INVALID;

            صحيح شمار = parse_dec_مقصد_uint32_t(s, SIZE_MAX, &layout);
            إذا (شمار > 0) {
                /* Note: 1-indexed layout */
                s += شمار;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                إذا (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (بیعلامت صحيح) val.sval.len, val.sval.شروع);
                } وإلا إذا (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_پیام_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (بیعلامت صحيح) val.sval.len, val.sval.شروع);
                } وإلا {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ثابت حرف* ثابت layout_شاخص_انتها = s;
            طالما (*s != '\0' && *s != ',') { s++; }
            إذا (شمار <= 0 || layout_شاخص_انتها != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (بیعلامت صحيح) (s - layout_شروع), layout_شروع,
                        (بیعلامت صحيح) val.sval.len, val.sval.شروع);
            }
        }

        إذا (val.sval.len || mlvo != MLVO_OPTION) {
            إذا (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                بنية matched_sval *prev;
                darray_foreach(prev, arr) {
                    إذا (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        إذا (val.layouts ==
                            (xkb_layout_ماسک_t)XKB_OPTION_LAYOUT_ماسک_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } وإلا إذا (prev->layouts != (xkb_layout_ماسک_t)
                                   XKB_OPTION_LAYOUT_ماسک_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        اذهب بعدی_مقدار;
                    }
                }
            }
            darray_append(arr, val);
        }

بعدی_مقدار:
        إذا (*s == '\0') اكسر;
        إذا (*s == ',') s++;
    }

    إذا (darray_خالی(arr))
        اذهب ensure_at_کمترین_one_مقدار;

    أرجع arr;
}

ساكن بنية matcher *
matcher_new_from_rmlvo(ثابت بنية xkb_rmlvo_builder *rmlvo, ثابت حرف **قواعد)
{
    بنية matcher *m = حافظه_صفر_بده(1, حجم(*m));
    إذا (!m)
        أرجع عدم;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    بنية xkb_قاعده_نامها نامها = {
        .قواعد = rmlvo->قواعد,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_خالی(rmlvo->layouts)) ? عدم : "x",
        .variant = (darray_خالی(rmlvo->layouts)) ? عدم : "x",
        .options = (darray_خالی(rmlvo->options)) ? عدم : "x",
    };
    ثابت تعداد RMLVO تغییریافته = xkb_context_sanitize_قاعده_نامها(rmlvo->ctx,
                                                               &نامها);

    /* Initialize matcher with sanitized names, if relevant */
    إذا (تغییریافته & RMLVO_قواعد) {
        *قواعد = نامها.قواعد;
    } وإلا {
        *قواعد = rmlvo->قواعد;
    }

    إذا (تغییریافته & RMLVO_MODEL) {
        m->rmlvo.model.sval.شروع = نامها.model;
    } وإلا {
        m->rmlvo.model.sval.شروع = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_امن(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_ماسک_t)XKB_OPTION_LAYOUT_ماسک_GLOBAL;

    أكد((تغییریافته & RMLVO_LAYOUT) || !(تغییریافته & RMLVO_VARIANT));
    إذا (تغییریافته & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = تقسیم_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      نامها.layout);
        m->rmlvo.variants = تقسیم_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       نامها.variant);
        إذا (darray_اندازه(m->rmlvo.layouts) > darray_اندازه(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            إذا (!isempty(نامها.variant))
                log_warn(m->ctx, XKB_LOG_پیام_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         نامها.layout ? نامها.layout : "(none)",
                         نامها.variant ? نامها.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_اندازه(m->rmlvo.layouts));
        } وإلا إذا (darray_اندازه(m->rmlvo.layouts) < darray_اندازه(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_پیام_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    نامها.layout ? نامها.layout : "(none)",
                    نامها.variant ? نامها.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_اندازه(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } وإلا {
        بنية xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            بنية matched_sval val = {
                .sval = SVAL(layout->layout, strlen_امن(layout->layout)),
                .layouts = (xkb_layout_ماسک_t)XKB_OPTION_LAYOUT_ماسک_GLOBAL,
                .matched = 0
            };
            darray_append(m->rmlvo.layouts, val);
            val.sval.شروع = layout->variant;
            val.sval.len = strlen_امن(layout->variant);
            darray_append(m->rmlvo.variants, val);
        }
    }

    إذا (تغییریافته & RMLVO_OPTIONS) {
        m->rmlvo.options = تقسیم_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      نامها.options);
    } وإلا {
        بنية xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            بنية matched_sval val = {
                .sval = SVAL(option->option, strlen_امن(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    أرجع m;
}

ساكن بنية matcher *
matcher_new_from_نامها(بنية xkb_context *ctx,
            ثابت بنية xkb_قاعده_نامها *rmlvo)
{
    بنية matcher *m = حافظه_صفر_بده(1, حجم(*m));
    إذا (!m)
        أرجع عدم;

    m->ctx = ctx;
    m->rmlvo.model.sval.شروع = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_امن(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_ماسک_t)XKB_OPTION_LAYOUT_ماسک_GLOBAL;
    m->rmlvo.layouts = تقسیم_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = تقسیم_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = تقسیم_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    إذا (darray_اندازه(m->rmlvo.layouts) > darray_اندازه(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        إذا (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_پیام_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_اندازه(m->rmlvo.layouts));
    } وإلا إذا (darray_اندازه(m->rmlvo.layouts) < darray_اندازه(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_اندازه(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    أرجع m;
}

ساكن فراغ
matcher_free(بنية matcher *m)
{
    إذا (!m)
        أرجع;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    بنية گروه *گروه;
    darray_foreach(گروه, m->groups)
        darray_free(گروه->elements);
    darray_free(m->pending_kccgst.میانگیر);
    darray_free(m->pending_kccgst.slices);
    لكل (kccgst_شاخص_t i = 0; i < (kccgst_شاخص_t) KCCGST_رقم_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    حرر(m);
}

ساكن فراغ
matcher_گروه_شروع_new(بنية matcher *m, بنية sval name)
{
    بنية گروه گروه = { .name = name, .elements = darray_new() };
    darray_append(m->groups, گروه);
}

ساكن فراغ
matcher_گروه_جمع_کن_element(بنية matcher *m, بنية scanner *s,
                          بنية sval element)
{
    darray_append(darray_قلم(m->groups, darray_اندازه(m->groups) - 1).elements,
                  element);
}

ساكن bool
read_قواعد_file(بنية xkb_context *ctx,
                بنية matcher *matcher,
                بیعلامت صحيح include_عمق,
                FILE *file,
                ثابت حرف *مسیر);

ساكن فراغ
matcher_include(بنية matcher *m, بنية scanner *والد_scanner,
                بیعلامت صحيح include_عمق,
                بنية sval inc)
{
    إذا (include_عمق >= MAX_INCLUDE_عمق) {
        scanner_err(والد_scanner, XKB_LOG_پیام_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_عمق);
        أرجع;
    }

    ثابت حرف *stmt_file = inc.شروع;
    اندازه_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    حرف buf[PATH_MAX] = {0};
    ثابت ssize_t expanded = expand_مسیر(m->ctx, والد_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_قواعد,
                                         buf, حجم(buf));
    إذا (expanded < 0) {
        /* Error */
        أرجع;
    } وإلا إذا (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (اندازه_t) expanded;
        أكد(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    بیعلامت صحيح جابه_جایی = 0;
    ثابت bool absolute_مسیر = is_absolute_مسیر(stmt_file);
    إذا (absolute_مسیر) {
        /* Absolute path: no need for lookup in XKB paths */
        إذا (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            إذا (stmt_file_len < حجم(buf)) {
                کپی_حافظه(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } وإلا {
                log_err(m->ctx, XKB_ERROR_INVALID_مسیر,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, حجم(buf),
                        (بیعلامت صحيح) stmt_file_len, stmt_file);
                أرجع;
            }
        } وإلا {
            /* %-expansion is always NULL-terminated */
            أكد(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } وإلا {
        /* Relative path: lookup the first XKB path */
        إذا (unlikely(expanded)) {
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
            file = پیدا_کن_File_درون_Xkb_مسیر(m->ctx, والد_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_قواعد,
                                     buf, حجم(buf), &جابه_جایی, true);
        }
    }

    طالما (file) {
        أكد(strlen_امن(buf) < حجم(buf));
        bool ret = read_قواعد_file(m->ctx, m, include_عمق + 1, file, buf);
        fclose(file);
        إذا (ret)
            أرجع;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_پیام_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        إذا (absolute_مسیر) {
            /* There is no point to search further if the path is absolute */
            اكسر;
        }

        /* Try next XKB path */
        جابه_جایی++;
        file = پیدا_کن_File_درون_Xkb_مسیر(m->ctx, والد_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_قواعد,
                                 buf, حجم(buf), &جابه_جایی, true);
    }

    log_err(m->ctx, XKB_LOG_پیام_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (بیعلامت صحيح) stmt_file_len, stmt_file);
}

ساكن فراغ
matcher_mapping_شروع_new(بنية matcher *m)
{
    لكل (mlvo_شاخص_t i = 0; i < (mlvo_شاخص_t) MLVO_رقم_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_رقم_ENTRIES;
    لكل (kccgst_شاخص_t i = 0; i < (kccgst_شاخص_t) KCCGST_رقم_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_رقم_ENTRIES;
    m->mapping.has_مضرب_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.رقم_mlvo = m->mapping.رقم_kccgst = 0;
    m->mapping.defined_mlvo_ماسک = 0;
    m->mapping.defined_kccgst_ماسک = 0;
    m->mapping.فعال = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
ساكن صحيح
parse_layout_int_شاخص_دم(ثابت حرف *s, اندازه_t max_len,
                            xkb_layout_شاخص_t *بیرون)
{
    إذا (max_len < 3)
        أرجع -1;
    uint32_t val = 0;
    ثابت صحيح شمار = parse_dec_مقصد_uint32_t(&s[1], max_len - 2, &val);
    إذا (شمار <= 0 || s[1 + شمار] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        أرجع -1;
    /* To zero-based index. */
    *بیرون = val - 1;
    أرجع شمار + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
ساكن صحيح
استخراج_layout_شاخص(ثابت حرف *s, اندازه_t max_len, xkb_layout_شاخص_t *بیرون)
{
    /* This function is pretty stupid, but works for now. */
    *بیرون = XKB_LAYOUT_INVALID;
    إذا (max_len < 3 || s[0] != '[')
        أرجع -1;
    إذا (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *بیرون = (xkb_layout_شاخص_t)(s[1] - '1');
        أرجع 3;
    }
    إذا (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        أرجع 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    أرجع parse_layout_int_شاخص_دم(s, max_len, بیرون);
}

/* Special layout indices */
تعداد layout_شاخص_ranges {
    LAYOUT_شاخص_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_شاخص_اولی,
    LAYOUT_شاخص_LATER,
    LAYOUT_شاخص_مضرب,
    LAYOUT_شاخص_ANY,
};

static_assert((xkb_layout_شاخص_t) XKB_MAX_GROUPS <
              (xkb_layout_شاخص_t) LAYOUT_شاخص_SINGLE,
              "Cannot define special indices");
static_assert((xkb_layout_شاخص_t) LAYOUT_شاخص_SINGLE <
              (xkb_layout_شاخص_t) LAYOUT_شاخص_اولی &&
              (xkb_layout_شاخص_t) LAYOUT_شاخص_اولی <
              (xkb_layout_شاخص_t) LAYOUT_شاخص_LATER &&
              (xkb_layout_شاخص_t) LAYOUT_شاخص_LATER <
              (xkb_layout_شاخص_t) LAYOUT_شاخص_مضرب &&
              (xkb_layout_شاخص_t) LAYOUT_شاخص_مضرب <
              (xkb_layout_شاخص_t) LAYOUT_شاخص_ANY &&
              (xkb_layout_شاخص_t) LAYOUT_شاخص_ANY <
              (xkb_layout_شاخص_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
ساكن bool
استخراج_mapping_layout_شاخص(ثابت حرف *s, اندازه_t max_len,
                             xkb_layout_شاخص_t *بیرون)
{
    /* Check for minimal `[` + index + `]` */
    إذا (max_len < 3 || s[0] != '[') {
        *بیرون = XKB_LAYOUT_INVALID;
        أرجع false;
    }

    /* Small numeric index */
    إذا (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *بیرون = (xkb_layout_شاخص_t)(s[1] - '1');
        أرجع true;
    }

    /* Named indices ranges */
    ساكن ثابت بنية {
        ثابت حرف* name;
        اندازه_t اندازه;
        xkb_layout_شاخص_t مقدار;
    } نامها[] = {
        #تعریف_کن شاخص_RANGE(دم, مقدار) \
            { (دم), حجم(دم), (xkb_layout_شاخص_t)(مقدار) }
        شاخص_RANGE("single]",   LAYOUT_شاخص_SINGLE),
        شاخص_RANGE("first]",    LAYOUT_شاخص_اولی),
        شاخص_RANGE("later]",    LAYOUT_شاخص_LATER),
        شاخص_RANGE("multiple]", LAYOUT_شاخص_مضرب),
        شاخص_RANGE("any]",      LAYOUT_شاخص_ANY),
        #لغو_تعریف شاخص_RANGE
    };

    لكل (اندازه_t k = 0; k < ARRAY_اندازه(نامها); k++) {
        إذا (max_len == نامها[k].اندازه &&
            memcmp(&s[1], نامها[k].name, نامها[k].اندازه - 1) == 0) {
            *بیرون = نامها[k].مقدار;
            أرجع true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *بیرون = XKB_LAYOUT_INVALID;
    ثابت صحيح شمار = parse_layout_int_شاخص_دم(s, max_len, بیرون);
    أرجع شمار > 0 && (اندازه_t)شمار == max_len;
}

ساكن مضمن bool
is_mlvo_ماسک_defined(بنية matcher *m, تعداد قواعد_mlvo mlvo)
{
    أرجع m->mapping.defined_mlvo_ماسک & (1u << mlvo);
}

ساكن فراغ
matcher_mapping_مجموعه_mlvo(بنية matcher *m, بنية scanner *s,
                         بنية sval ident)
{
    تعداد قواعد_mlvo mlvo;
    بنية sval mlvo_sval;

    لكل (mlvo = 0; mlvo < MLVO_رقم_ENTRIES; mlvo++) {
        mlvo_sval = قواعد_mlvo_svals[mlvo];

        إذا (svaleq_پیشوند(mlvo_sval, ident))
            اكسر;
    }

    /* Not found. */
    إذا (mlvo >= MLVO_رقم_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (بیعلامت صحيح) ident.len, ident.شروع);
        m->mapping.فعال = false;
        أرجع;
    }

    إذا (is_mlvo_ماسک_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (بیعلامت صحيح) mlvo_sval.len, mlvo_sval.شروع);
        m->mapping.فعال = false;
        أرجع;
    }

    /* If there are leftovers still, it must be an index. */
    إذا (mlvo_sval.len < ident.len) {
        xkb_layout_شاخص_t idx;
        إذا (!استخراج_mapping_layout_شاخص(ident.شروع + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (بیعلامت صحيح) mlvo_sval.len, mlvo_sval.شروع);
            m->mapping.فعال = false;
            أرجع;
        }

        إذا (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        وإلا إذا (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        وإلا {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (بیعلامت صحيح) mlvo_sval.len, mlvo_sval.شروع);
            m->mapping.فعال = false;
            أرجع;
        }
    } وإلا إذا (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_شاخص_t) LAYOUT_شاخص_SINGLE;
    } وإلا إذا (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_شاخص_t) LAYOUT_شاخص_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    إذا (((mlvo == MLVO_LAYOUT && is_mlvo_ماسک_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_ماسک_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.فعال = false;
        أرجع;
    }

    m->mapping.mlvo_at_pos[m->mapping.رقم_mlvo] = mlvo;
    m->mapping.defined_mlvo_ماسک |= (mlvo_ماسک_t) 1u << mlvo;
    m->mapping.رقم_mlvo++;
}

ساكن فراغ
matcher_mapping_مجموعه_layout_bounds(بنية matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_شاخص_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    بدل (idx) {
        حالة XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            أكد(!is_mlvo_ماسک_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_ماسک_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_ماسک = 0x1; /* active = true */
            اكسر;
        حالة LAYOUT_شاخص_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_اندازه(m->rmlvo.layouts));
            m->mapping.layouts_candidates_ماسک = (xkb_layout_ماسک_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            اكسر;
        حالة LAYOUT_شاخص_مضرب:
        حالة LAYOUT_شاخص_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_اندازه(m->rmlvo.layouts));
            m->mapping.layouts_candidates_ماسک = (xkb_layout_ماسک_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            اكسر;
        حالة LAYOUT_شاخص_SINGLE:
        حالة LAYOUT_شاخص_اولی:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        افتراضي:
            /* Mere layout index */
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = idx;
            m->mapping.layout_idx_max = idx + 1;
            m->mapping.layouts_candidates_ماسک = UINT32_C(1) << idx;
    }
    m->mapping.has_مضرب_layouts = (
        m->mapping.layout_idx_max > m->mapping.layout_idx_min &&
        m->mapping.layout_idx_max - m->mapping.layout_idx_min > 1
    );
}

ساكن فراغ
matcher_mapping_مجموعه_kccgst(بنية matcher *m, بنية scanner *s, بنية sval ident)
{
    تعداد قواعد_kccgst kccgst;
    بنية sval kccgst_sval;

    لكل (kccgst = 0; kccgst < KCCGST_رقم_ENTRIES; kccgst++) {
        kccgst_sval = قواعد_kccgst_svals[kccgst];

        إذا (svaleq(قواعد_kccgst_svals[kccgst], ident))
            اكسر;
    }

    /* Not found. */
    إذا (kccgst >= KCCGST_رقم_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (بیعلامت صحيح) ident.len, ident.شروع);
        m->mapping.فعال = false;
        أرجع;
    }

    إذا (m->mapping.defined_kccgst_ماسک & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (بیعلامت صحيح) kccgst_sval.len, kccgst_sval.شروع);
        m->mapping.فعال = false;
        أرجع;
    }

    m->mapping.kccgst_at_pos[m->mapping.رقم_kccgst] = kccgst;
    m->mapping.defined_kccgst_ماسک |= (kccgst_ماسک_t) 1u << kccgst;
    m->mapping.رقم_kccgst++;
}

ساكن bool
matcher_mapping_verify(بنية matcher *m, بنية scanner *s)
{
    إذا (m->mapping.رقم_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        اذهب skip;
    }

    إذا (m->mapping.رقم_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        اذهب skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    إذا (is_mlvo_ماسک_defined(m, MLVO_LAYOUT)) {
        أكد(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        بدل (m->mapping.layout_idx) {
            حالة LAYOUT_شاخص_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                إذا (darray_اندازه(m->rmlvo.layouts) > 1)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_شاخص_مضرب:
            حالة LAYOUT_شاخص_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                إذا (darray_اندازه(m->rmlvo.layouts) < 2)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_شاخص_ANY:
            حالة LAYOUT_شاخص_اولی:
                /* No restrictions */
                اكسر;
            افتراضي:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                إذا (darray_اندازه(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_اندازه(m->rmlvo.layouts))
                    اذهب skip;
        }
    }

    إذا (is_mlvo_ماسک_defined(m, MLVO_VARIANT)) {
        أكد(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        بدل (m->mapping.variant_idx) {
            حالة LAYOUT_شاخص_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                إذا (darray_اندازه(m->rmlvo.variants) > 1)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_شاخص_ANY:
            حالة LAYOUT_شاخص_LATER:
            حالة LAYOUT_شاخص_اولی:
                /* No restriction */
                اكسر;
            افتراضي:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                إذا (darray_اندازه(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_اندازه(m->rmlvo.variants))
                    اذهب skip;
        }
    }

    أرجع true;

skip:
    m->mapping.فعال = false;
    أرجع false;
}

ساكن فراغ
matcher_قاعده_شروع_new(بنية matcher *m)
{
    memset(&m->قاعده_2, 0, حجم(m->قاعده_2));
    m->قاعده_2.skip = !m->mapping.فعال;
}

ساكن فراغ
matcher_قاعده_مجموعه_mlvo_common(بنية matcher *m, بنية scanner *s,
                             بنية sval ident,
                             تعداد mlvo_تطابق_type تطابق_type)
{
    إذا (m->قاعده_2.رقم_mlvo_مقادیر >= m->mapping.رقم_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->قاعده_2.skip = true;
        أرجع;
    }
    m->قاعده_2.تطابق_type_at_pos[m->قاعده_2.رقم_mlvo_مقادیر] = تطابق_type;
    m->قاعده_2.mlvo_مقدار_at_pos[m->قاعده_2.رقم_mlvo_مقادیر] = ident;
    m->قاعده_2.رقم_mlvo_مقادیر++;
}

ساكن فراغ
matcher_قاعده_مجموعه_mlvo_wildcard(بنية matcher *m, بنية scanner *s,
                               تعداد mlvo_تطابق_type تطابق_type)
{
    بنية sval dummy = SVAL(عدم, 0);
    matcher_قاعده_مجموعه_mlvo_common(m, s, dummy, تطابق_type);
}

ساكن فراغ
matcher_قاعده_مجموعه_mlvo_گروه(بنية matcher *m, بنية scanner *s,
                            بنية sval ident)
{
    matcher_قاعده_مجموعه_mlvo_common(m, s, ident, MLVO_تطابق_گروه);
}

ساكن فراغ
matcher_قاعده_مجموعه_mlvo(بنية matcher *m, بنية scanner *s,
                      بنية sval ident)
{
    matcher_قاعده_مجموعه_mlvo_common(m, s, ident, MLVO_تطابق_نرمال);
}

ساكن فراغ
matcher_قاعده_مجموعه_kccgst(بنية matcher *m, بنية scanner *s,
                        بنية sval ident)
{
    إذا (m->قاعده_2.رقم_kccgst_مقادیر >= m->mapping.رقم_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->قاعده_2.skip = true;
        أرجع;
    }
    m->قاعده_2.kccgst_مقدار_at_pos[m->قاعده_2.رقم_kccgst_مقادیر] = ident;
    m->قاعده_2.رقم_kccgst_مقادیر++;
}

ساكن bool
تطابق_گروه(بنية matcher *m, بنية sval گروه_name, بنية sval مقصد)
{
    بنية گروه *گروه = عدم;
    بنية sval *element;
    bool یافت = false;

    darray_foreach(گروه, m->groups) {
        إذا (svaleq(گروه->name, گروه_name)) {
            یافت = true;
            اكسر;
        }
    }

    إذا (!یافت) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        أرجع false;
    }

    darray_foreach(element, گروه->elements)
        إذا (svaleq(مقصد, *element))
            أرجع true;

    أرجع false;
}

ساكن bool
تطابق_مقدار(بنية matcher *m, ثابت بنية sval val, ثابت بنية sval مقصد,
            تعداد mlvo_تطابق_type تطابق_type,
            تعداد wildcard_تطابق_type wildcard_type)
{
    بدل (تطابق_type) {
        حالة MLVO_تطابق_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            أرجع wildcard_type == WILDCARD_تطابق_ALL || !!مقصد.len;
        حالة MLVO_تطابق_WILDCARD_NONE:
            أرجع !مقصد.len;
        حالة MLVO_تطابق_WILDCARD_SOME:
            أرجع !!مقصد.len;
        حالة MLVO_تطابق_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            أرجع true;
        حالة MLVO_تطابق_گروه:
            أرجع تطابق_گروه(m, val, مقصد);
        افتراضي:
            أكد(تطابق_type == MLVO_تطابق_نرمال);
            أرجع svaleq(val, مقصد);
    }
}

ساكن bool
تطابق_مقدار_and_mark(بنية matcher *m, ثابت بنية sval val,
                     بنية matched_sval *مقصد, تعداد mlvo_تطابق_type تطابق_type,
                     تعداد wildcard_تطابق_type wildcard_type,
                     xkb_layout_ماسک_t layouts)
{
    bool matched = تطابق_مقدار(m, val, مقصد->sval, تطابق_type, wildcard_type);
    إذا (matched)
        مقصد->matched |= layouts;
    أرجع matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
ساكن bool
expand_rmlvo_درون_kccgst_مقدار(بنية matcher *m, بنية scanner *s,
                             بنية sval مقدار, xkb_layout_شاخص_t layout_idx,
                             darray_char *expanded, اندازه_t *i)
{
    ثابت حرف *str = مقدار.شروع;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    تعداد قواعد_mlvo mlv;
    xkb_layout_شاخص_t idx;
    حرف pfx, sfx;
    بنية matched_sval *expanded_مقدار;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    إذا (str[*i] == 'i' &&
        (*i + 1 == مقدار.len || is_ادغام_نما_پیشوند(str[*i + 1])))
    {
        إذا (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_قواعد_INVALID_LAYOUT_شاخص_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            اذهب error;
        }
        (*i)++;
        حرف شاخص_str[MAX_LAYOUT_شاخص_STR_درازی + 1];
        ثابت صحيح شمار = snprintf(شاخص_str, حجم(شاخص_str), "%"PRIu32,
                                   layout_idx + 1);
        أكد(شمار > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, شاخص_str, (darray_اندازه_t)شمار);
        أرجع true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    إذا (str[*i] == '(' ||
        is_ادغام_نما_پیشوند(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        إذا (str[*i] == '(') sfx = ')';
        إذا (++(*i) >= مقدار.len) اذهب error;
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
    bool variable_شاخص = false;
    إذا (*i < مقدار.len && str[*i] == '[') {
        إذا (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            اذهب error;
        }

        صحيح consumed = استخراج_layout_شاخص(str + (*i), مقدار.len - (*i), &idx);
        إذا (consumed <= 0) اذهب error;
        إذا (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            أكد(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_شاخص = true;
        }
        *i += (اندازه_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    إذا (sfx != 0) {
        إذا (*i >= مقدار.len) اذهب error;
        إذا (str[(*i)++] != sfx) اذهب error;
    }

    /* Get the expanded value. */
    expanded_مقدار = عدم;

    إذا (mlv == MLVO_LAYOUT) {
        إذا (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            إذا (darray_اندازه(m->rmlvo.layouts) == 1)
                expanded_مقدار = &darray_قلم(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } وإلا إذا (idx < darray_اندازه(m->rmlvo.layouts) &&
                   (variable_شاخص || darray_اندازه(m->rmlvo.layouts) > 1)) {
                expanded_مقدار = &darray_قلم(m->rmlvo.layouts, idx);
        }
    }
    وإلا إذا (mlv == MLVO_VARIANT) {
        إذا (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            إذا (darray_اندازه(m->rmlvo.variants) == 1)
                expanded_مقدار = &darray_قلم(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } وإلا إذا (idx < darray_اندازه(m->rmlvo.variants) &&
                   (variable_شاخص || darray_اندازه(m->rmlvo.variants) > 1)) {
                expanded_مقدار = &darray_قلم(m->rmlvo.variants, idx);
        }
    }
    وإلا إذا (mlv == MLVO_MODEL) {
        expanded_مقدار = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    إذا (!expanded_مقدار || expanded_مقدار->sval.len == 0) {
        أرجع true;
    }

    إذا (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_مقدار->sval.شروع,
                                 (darray_اندازه_t) expanded_مقدار->sval.len);
    إذا (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    إذا (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ثابت xkb_layout_ماسک_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_مقدار->matched |= layouts;
    } وإلا {
        expanded_مقدار->matched |= (xkb_layout_ماسک_t)GLOBAL_MATCHED_LAYOUTS;
    }

    أرجع true;

error:
    scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                "invalid %%-expansion in value; not used");
    أرجع false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
ساكن فراغ
expand_qualifier_درون_kccgst_مقدار(
    بنية matcher *m, بنية scanner *s,
    بنية sval مقدار, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_اندازه_t پیشوند_idx, اندازه_t *i)
{
    ثابت حرف *str = مقدار.شروع;

    /* “all” followed by nothing or by a layout separator */
    إذا (*i + 2 < مقدار.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == مقدار.len || is_ادغام_نما_پیشوند(str[*i + 3]))) {
        إذا (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_پیام_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        إذا (darray_اندازه(m->rmlvo.layouts) > 1) {
            حرف layout_شاخص[MAX_LAYOUT_شاخص_STR_درازی + 1];
            ثابت darray_اندازه_t پیشوند_درازی =
                darray_اندازه(*expanded) - پیشوند_idx - 1;
            لكل (xkb_layout_شاخص_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_اندازه(m->rmlvo.layouts));
                 l++)
            {
                إذا (!has_separator)
                    darray_append(*expanded, ادغام_DEFAULT_پیشوند);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_قلم(*expanded, پیشوند_idx),
                                             پیشوند_درازی);
                /* Append index */
                ثابت صحيح شمار = snprintf(layout_شاخص, حجم(layout_شاخص),
                                           "%"PRIu32, l + 1);
                أكد(شمار > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_شاخص,
                                             (darray_اندازه_t)شمار);
            }
        }
        *i += 3;
    }
}

ساكن مضمن فراغ
#اگر_تعریف_شده _MSC_VER
concat_kccgst(darray_char *into, darray_اندازه_t اندازه, درون_reads(اندازه) ثابت حرف* from)
#وإلا
concat_kccgst(darray_char *into, darray_اندازه_t اندازه, ثابت حرف from[ساكن اندازه])
#پایان_اگر
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ثابت bool from_plus = is_ادغام_نما_پیشوند(from[0]);
    إذا (from_plus || darray_خالی(*into)) {
        darray_appends_nullterminate(*into, from, اندازه);
    } وإلا {
        ثابت حرف ch =
            (حرف) (darray_خالی(*into) ? '\0' : darray_قلم(*into, 0));
        ثابت bool into_plus = is_ادغام_نما_پیشوند(ch);
        إذا (into_plus)
            darray_prepends_nullterminate(*into, from, اندازه);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
ساكن bool
append_expanded_kccgst_مقدار(بنية matcher *m, بنية scanner *s,
                             bool ادغام, darray_char *مقصد, بنية sval مقدار,
                             xkb_layout_شاخص_t layout_idx)
{
    ثابت حرف *str = مقدار.شروع;
    darray_char expanded = darray_new();
    darray_اندازه_t آخرین_قلم_idx = 0;
    bool has_separator = false;

    لكل (اندازه_t i = 0; i < مقدار.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        بدل (str[i]) {
            /* Qualifier */
            حالة ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_درون_kccgst_مقدار(m, s, مقدار, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 آخرین_قلم_idx, &i);
                اكسر;
            /* Expansion */
            حالة '%':
                i++;
                إذا (i >= مقدار.len ||
                    !expand_rmlvo_درون_kccgst_مقدار(m, s, مقدار, layout_idx,
                                                  &expanded, &i))
                        اذهب error;
                اكسر;
            /* New item */
            حالة ادغام_OVERRIDE_پیشوند:
            حالة ادغام_AUGMENT_پیشوند:
            حالة ادغام_REPLACE_پیشوند:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                آخرین_قلم_idx = darray_اندازه(expanded) - 1;
                has_separator = true;
                اكسر;
            /* Just a normal character. */
            افتراضي:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    إذا (ادغام) {
        إذا (!darray_خالی(expanded))
            concat_kccgst(مقصد, darray_اندازه(expanded), darray_اقلام(expanded));
    } وإلا {
        darray_concat(*مقصد, expanded);
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
    إذا (!m->mapping.has_مضرب_layouts)
        أرجع true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    لكل (kccgst_شاخص_t i = 0; i < m->mapping.رقم_kccgst; i++) {
        ثابت تعداد قواعد_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        لكل (xkb_layout_شاخص_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            سجل ثابت بنية kccgst_میانگیر* ثابت buf = &m->pending_kccgst;
            اندازه_t جابه_جایی = 0;
            لكل (darray_اندازه_t k = 0; k < darray_اندازه(buf->slices); k++) {
                سجل ثابت بنية kccgst_میانگیر_برش * ثابت برش =
                    &darray_قلم(buf->slices, k);
                إذا (برش->kccgst == kccgst && برش->layout == layout &&
                    برش->درازی)
                    concat_kccgst(&m->kccgst[kccgst], برش->درازی,
                                  darray_اقلام(buf->میانگیر) + جابه_جایی);
                جابه_جایی += برش->درازی;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_مضرب_layouts = false;
    أرجع true;
}

ساكن فراغ
matcher_قاعده_verify(بنية matcher *m, بنية scanner *s)
{
    إذا (m->قاعده_2.رقم_mlvo_مقادیر != m->mapping.رقم_mlvo ||
        m->قاعده_2.رقم_kccgst_مقادیر != m->mapping.رقم_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                    "invalid rule: must have same number of values "
                    "as mapping line; ignoring rule");
        m->قاعده_2.skip = true;
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
matcher_قاعده_تطابق_model(بنية matcher *m, ثابت بنية sval مقدار,
                         تعداد mlvo_تطابق_type تطابق_type)
{
    أرجع تطابق_مقدار_and_mark(
        m, مقدار, &m->rmlvo.model, تطابق_type, WILDCARD_تطابق_ALL,
        /* layout-independent */
        (xkb_layout_ماسک_t)GLOBAL_MATCHED_LAYOUTS
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
matcher_قاعده_تطابق_option(بنية matcher *m, ثابت بنية sval مقدار,
                          تعداد mlvo_تطابق_type تطابق_type,
                          xkb_layout_ماسک_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_خالی(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_ماسک_t unmatched =
        (m->mapping.defined_mlvo_ماسک & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    بنية matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        ثابت xkb_layout_ماسک_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_ماسک_t)GLOBAL_MATCHED_LAYOUTS;
        إذا (matchable && تطابق_مقدار_and_mark(m, مقدار, option, تطابق_type,
                                              WILDCARD_تطابق_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            إذا (!unmatched)
                اكسر;
        }
    }
    إذا (unmatched) {
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
matcher_قاعده_تطابق_layout_or_variant(بنية matcher *m,
                                     darray_matched_sval *درون_داد,
                                     ثابت بنية sval مقدار,
                                     تعداد mlvo_تطابق_type تطابق_type,
                                     xkb_layout_ماسک_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_خالی(*درون_داد));

    bool matched = false;
    /* Loop over the layout index range */
    لكل (xkb_layout_شاخص_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ثابت xkb_layout_ماسک_t layout = (UINT32_C(1) << idx);
        إذا (layout & *candidates) {
            بنية matched_sval *مقصد = &darray_قلم(*درون_داد, idx);
            إذا (تطابق_مقدار_and_mark(m, مقدار, مقصد, تطابق_type,
                                     WILDCARD_تطابق_NONEMPTY, layout)) {
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
matcher_قاعده_apply_if_matches(بنية matcher *m, بنية scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_ماسک_t candidate_layouts = m->mapping.layouts_candidates_ماسک;

    /* Loop over MLVO pattern components */
    لكل (mlvo_شاخص_t i = 0; i < m->mapping.رقم_mlvo; i++) {
        ثابت تعداد قواعد_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ثابت بنية sval مقدار = m->قاعده_2.mlvo_مقدار_at_pos[i];
        ثابت تعداد mlvo_تطابق_type تطابق_type = m->قاعده_2.تطابق_type_at_pos[i];
        bool matched = false;

        بدل (mlvo) {
        حالة MLVO_MODEL:
            matched = matcher_قاعده_تطابق_model(m, مقدار, تطابق_type);
            اكسر;
        حالة MLVO_LAYOUT:
            matched = matcher_قاعده_تطابق_layout_or_variant(
                m, &m->rmlvo.layouts, مقدار, تطابق_type, &candidate_layouts
            );
            اكسر;
        حالة MLVO_VARIANT:
            matched = matcher_قاعده_تطابق_layout_or_variant(
                m, &m->rmlvo.variants, مقدار, تطابق_type, &candidate_layouts
            );
            اكسر;
        حالة MLVO_OPTION:
            matched = matcher_قاعده_تطابق_option(
                m, مقدار, تطابق_type, &candidate_layouts
            );
            اكسر;
        افتراضي: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_رقم_ENTRIES - 1,
                          "Unexpected MLVO field");
            أكد(!"Unreachable");
        }
        }

        إذا (!matched)
            أرجع;
    }

    إذا (m->mapping.has_مضرب_layouts) {
        /* Special index: loop over the index range */
        لكل (xkb_layout_شاخص_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            إذا (candidate_layouts & (UINT32_C(1) << idx)) {
                لكل (kccgst_شاخص_t i = 0; i < m->mapping.رقم_kccgst; i++) {
                    ثابت تعداد قواعد_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ثابت بنية sval مقدار = m->قاعده_2.kccgst_مقدار_at_pos[i];
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
                    بنية kccgst_میانگیر * ثابت buf = &m->pending_kccgst;
                    ثابت darray_اندازه_t prev_میانگیر_درازی =
                        darray_اندازه(buf->میانگیر);
                    append_expanded_kccgst_مقدار(m, s, false, &buf->میانگیر,
                                                 مقدار, idx);
                    ثابت uint32_t درازی = (uint32_t) (darray_اندازه(buf->میانگیر)
                                          - prev_میانگیر_درازی);
                    ثابت بنية kccgst_میانگیر_برش برش = {
                        .درازی = درازی,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, برش);
                }
            }
        }
    } وإلا {
        /* Numeric index or no index */
        لكل (kccgst_شاخص_t i = 0; i < m->mapping.رقم_kccgst; i++) {
            تعداد قواعد_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            بنية sval مقدار = m->قاعده_2.kccgst_مقدار_at_pos[i];
            append_expanded_kccgst_مقدار(m, s, true, &m->kccgst[kccgst], مقدار,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    إذا (!(is_mlvo_ماسک_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_ماسک &= ~candidate_layouts;
    }
}

ساكن تعداد قواعد_token
gettok(بنية matcher *m, بنية scanner *s)
{
    أرجع lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
ساكن bool
matcher_تطابق(بنية matcher *m, بنية scanner *s,
              بیعلامت صحيح include_عمق,
              ثابت حرف *string, اندازه_t len,
              ثابت حرف *file_name)
{
    تعداد قواعد_token tok;

    إذا (!m)
        أرجع false;

initial:
    بدل (tok = gettok(m, s)) {
    حالة TOK_BANG:
        اذهب bang;
    حالة TOK_انتها_از_سطر:
        اذهب initial;
    حالة TOK_انتها_از_FILE:
        اذهب پایان;
    افتراضي:
        اذهب unexpected;
    }

bang:
    بدل (tok = gettok(m, s)) {
    حالة TOK_گروه_NAME:
        matcher_گروه_شروع_new(m, m->val.string);
        اذهب گروه_name;
    حالة TOK_INCLUDE:
        اذهب include_statement;
    حالة TOK_IDENTIFIER:
        matcher_mapping_شروع_new(m);
        matcher_mapping_مجموعه_mlvo(m, s, m->val.string);
        اذهب mapping_mlvo;
    افتراضي:
        اذهب unexpected;
    }

گروه_name:
    بدل (tok = gettok(m, s)) {
    حالة TOK_EQUALS:
        اذهب گروه_element;
    افتراضي:
        اذهب unexpected;
    }

گروه_element:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        matcher_گروه_جمع_کن_element(m, s, m->val.string);
        اذهب گروه_element;
    حالة TOK_انتها_از_سطر:
        اذهب initial;
    افتراضي:
        اذهب unexpected;
    }

include_statement:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        matcher_include(m, s, include_عمق, m->val.string);
        اذهب include_statement_انتها;
    افتراضي:
        اذهب unexpected;
    }

include_statement_انتها:
    بدل (tok = gettok(m, s)) {
    حالة TOK_انتها_از_سطر:
        اذهب initial;
    افتراضي:
        اذهب unexpected;
    }

mapping_mlvo:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        إذا (m->mapping.فعال)
            matcher_mapping_مجموعه_mlvo(m, s, m->val.string);
        اذهب mapping_mlvo;
    حالة TOK_EQUALS:
        اذهب mapping_kccgst;
    افتراضي:
        اذهب unexpected;
    }

mapping_kccgst:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        إذا (m->mapping.فعال)
            matcher_mapping_مجموعه_kccgst(m, s, m->val.string);
        اذهب mapping_kccgst;
    حالة TOK_انتها_از_سطر:
        إذا (m->mapping.فعال && matcher_mapping_verify(m, s)) {
            matcher_mapping_مجموعه_layout_bounds(m);
            إذا (m->mapping.has_مضرب_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_اندازه(m->pending_kccgst.میانگیر) = 0;
                darray_اندازه(m->pending_kccgst.slices) = 0;
            }
        }
        اذهب قاعده_mlvo_اولی;
    افتراضي:
        اذهب unexpected;
    }

قاعده_mlvo_اولی:
    بدل (tok = gettok(m, s)) {
    حالة TOK_BANG:
        matcher_append_pending_kccgst(m);
        اذهب bang;
    حالة TOK_انتها_از_سطر:
        اذهب قاعده_mlvo_اولی;
    حالة TOK_انتها_از_FILE:
        matcher_append_pending_kccgst(m);
        اذهب پایان;
    افتراضي:
        matcher_قاعده_شروع_new(m);
        اذهب قاعده_mlvo_no_tok;
    }

قاعده_mlvo:
    tok = gettok(m, s);
قاعده_mlvo_no_tok:
    بدل (tok) {
    حالة TOK_IDENTIFIER:
        إذا (!m->قاعده_2.skip)
            matcher_قاعده_مجموعه_mlvo(m, s, m->val.string);
        اذهب قاعده_mlvo;
    حالة TOK_WILD_CARD_ستاره:
        إذا (!m->قاعده_2.skip)
            matcher_قاعده_مجموعه_mlvo_wildcard(m, s, MLVO_تطابق_WILDCARD_LEGACY);
        اذهب قاعده_mlvo;
    حالة TOK_WILD_CARD_NONE:
        إذا (!m->قاعده_2.skip)
            matcher_قاعده_مجموعه_mlvo_wildcard(m, s, MLVO_تطابق_WILDCARD_NONE);
        اذهب قاعده_mlvo;
    حالة TOK_WILD_CARD_SOME:
        إذا (!m->قاعده_2.skip)
            matcher_قاعده_مجموعه_mlvo_wildcard(m, s, MLVO_تطابق_WILDCARD_SOME);
        اذهب قاعده_mlvo;
    حالة TOK_WILD_CARD_ANY:
        إذا (!m->قاعده_2.skip)
            matcher_قاعده_مجموعه_mlvo_wildcard(m, s, MLVO_تطابق_WILDCARD_ANY);
        اذهب قاعده_mlvo;
    حالة TOK_گروه_NAME:
        إذا (!m->قاعده_2.skip)
            matcher_قاعده_مجموعه_mlvo_گروه(m, s, m->val.string);
        اذهب قاعده_mlvo;
    حالة TOK_EQUALS:
        اذهب قاعده_kccgst;
    افتراضي:
        اذهب unexpected;
    }

قاعده_kccgst:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        إذا (!m->قاعده_2.skip)
            matcher_قاعده_مجموعه_kccgst(m, s, m->val.string);
        اذهب قاعده_kccgst;
    حالة TOK_انتها_از_سطر:
        إذا (!m->قاعده_2.skip)
            matcher_قاعده_verify(m, s);
        إذا (!m->قاعده_2.skip)
            matcher_قاعده_apply_if_matches(m, s);
        اذهب قاعده_mlvo_اولی;
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

پایان:
    أرجع true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_قواعد_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    أرجع false;
}
// NOLINTEND(readability-trivial-switch)

ساكن bool
read_قواعد_file(بنية xkb_context *ctx,
                بنية matcher *matcher,
                بیعلامت صحيح include_عمق,
                FILE *file,
                ثابت حرف *مسیر)
{
    bool ret;
    حرف *string;
    اندازه_t اندازه;
    بنية scanner scanner;

    إذا (!map_file(file, &string, &اندازه)) {
        log_err(ctx, XKB_LOG_پیام_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                مسیر, strerror(errno));
        أرجع false;
    }

    scanner_init(&scanner, matcher->ctx, string, اندازه, مسیر, عدم);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    إذا (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, اندازه);
        أرجع false;
    }

    ret = matcher_تطابق(matcher, &scanner, include_عمق, string, اندازه, مسیر);
    unmap_file(string, اندازه);
    أرجع ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
ساكن bool
xkb_resolve_partial_قواعد(بنية xkb_context *ctx, حرف *مسیر, اندازه_t مسیر_اندازه,
                          ثابت حرف* قواعد, ثابت حرف* suffix,
                          بنية matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    حرف partial_قواعد[60]; /* Arbitrary, but we do not expect long names */
    إذا (unlikely(!snprintf_امن(partial_قواعد, حجم(partial_قواعد),
                                "%s%s", قواعد, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", قواعد, suffix);
        أرجع false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    بیعلامت صحيح جابه_جایی = 0;
    FILE *file = عدم;
    ثابت اندازه_t len = طول_رشته(partial_قواعد);
    طالما ((file = پیدا_کن_File_درون_Xkb_مسیر(ctx, "(unknown)",
                                     partial_قواعد, len, FILE_TYPE_قواعد,
                                     مسیر, مسیر_اندازه, &جابه_جایی, false)) != عدم) {
        ثابت bool ok = read_قواعد_file(ctx, matcher, 0, file, مسیر);
        fclose(file);
        إذا (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", مسیر);
            أرجع false;
        }
        جابه_جایی++;
    }

    أرجع true;
}

ساكن bool
xkb_resolve_قواعد(بنية xkb_context *ctx,
                  ثابت حرف* قواعد, بنية matcher *matcher,
                  بنية xkb_component_نامها *بیرون,
                  xkb_layout_شاخص_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    بیعلامت صحيح جابه_جایی = 0;
    حرف مسیر[PATH_MAX];
    FILE * ثابت file = پیدا_کن_File_درون_Xkb_مسیر(ctx, "(unknown)",
                                          قواعد, طول_رشته(قواعد), FILE_TYPE_قواعد,
                                          مسیر, حجم(مسیر), &جابه_جایی, true);
    إذا (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", قواعد);
        اذهب err_بیرون;
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
    ret = xkb_resolve_partial_قواعد(ctx, مسیر, حجم(مسیر),
                                    قواعد, ".pre", matcher);
    إذا (!ret)
        اذهب err_بیرون;

    /* Resolve main <rules> file */
    ret = read_قواعد_file(ctx, matcher, 0, file, مسیر);
    إذا (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", مسیر);
        اذهب err_بیرون;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_قواعد(ctx, مسیر, حجم(مسیر),
                                    قواعد, ".post", matcher);
    إذا (!ret)
        اذهب err_بیرون;

    /* Check we got required components */
    إذا (darray_خالی(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_خالی(matcher->kccgst[KCCGST_TYPES]) ||
        darray_خالی(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_خالی(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", قواعد);
        ret = false;
        اذهب err_بیرون;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &بیرون->keycodes, عدم);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &بیرون->types, عدم);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &بیرون->compatibility, عدم);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &بیرون->symbols, عدم);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &بیرون->geometry, عدم);

    بنية matched_sval *mval = &matcher->rmlvo.model;
    إذا (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (بیعلامت صحيح) mval->sval.len, mval->sval.شروع, قواعد);
    darray_foreach(mval, matcher->rmlvo.layouts)
        إذا (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (بیعلامت صحيح) mval->sval.len, mval->sval.شروع, قواعد);
    darray_foreach(mval, matcher->rmlvo.variants)
        إذا (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (بیعلامت صحيح) mval->sval.len, mval->sval.شروع, قواعد);
    darray_foreach(mval, matcher->rmlvo.options) {
        إذا (!mval->sval.len)
            استمر;

        إذا (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (بیعلامت صحيح) mval->sval.len, mval->sval.شروع, قواعد);
        } وإلا إذا (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ثابت xkb_layout_ماسک_t unmatched = (mval->layouts & ~mval->matched);
            أكد(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_مقدار,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (بیعلامت صحيح)mval->sval.len, mval->sval.شروع,
                    unmatched, قواعد);
        }
    }

    /* Set the number of explicit layouts */
    إذا (بیرون->symbols != عدم && explicit_layouts != عدم) {
        *explicit_layouts = 1; /* at least one group */
        ثابت حرف *symbols = بیرون->symbols;
        /* Take the highest modifier */
        طالما ((symbols = strchr(symbols, ':')) != عدم && symbols[1] != '\0') {
            xkb_layout_شاخص_t گروه = 0;
            ثابت صحيح شمار = parse_dec_مقصد_uint32_t(++symbols, SIZE_MAX, &گروه);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            إذا (شمار > 0 && (symbols[شمار] == '\0' ||
                is_ادغام_نما_پیشوند(symbols[شمار])) &&
                گروه > 0 && گروه <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, گروه);
                symbols += شمار;
            }
        }
    }

err_بیرون:
    إذا (file)
        fclose(file);

    أرجع ret;
}

bool
xkb_components_from_rmlvo_builder(ثابت بنية xkb_rmlvo_builder *rmlvo,
                                  بنية xkb_component_نامها *بیرون,
                                  xkb_layout_شاخص_t *explicit_layouts)
{
    ثابت حرف *قواعد = rmlvo->قواعد;
    بنية matcher *matcher = matcher_new_from_rmlvo(rmlvo, &قواعد);
    إذا (!matcher)
        أرجع false;

    ثابت bool ret = xkb_resolve_قواعد(rmlvo->ctx, قواعد, matcher,
                                       بیرون, explicit_layouts);

    matcher_free(matcher);
    أرجع ret;
}


bool
xkb_components_from_قواعد_نامها(بنية xkb_context *ctx,
                                ثابت بنية xkb_قاعده_نامها *rmlvo,
                                بنية xkb_component_نامها *بیرون,
                                xkb_layout_شاخص_t *explicit_layouts)
{
    بنية matcher *matcher = matcher_new_from_نامها(ctx, rmlvo);
    إذا (!matcher)
        أرجع false;

    ثابت bool ret = xkb_resolve_قواعد(ctx, rmlvo->قواعد, matcher,
                                       بیرون, explicit_layouts);

    matcher_free(matcher);
    أرجع ret;
}
