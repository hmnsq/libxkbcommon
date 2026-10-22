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

#تعریف_کرو MAX_INCLUDE_ڈونگھائی 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
اتحاد lvalue {
    بنية sval string;
};

تعداد قاعدے_token {
    TOK_سرا_دا_FILE = 0,
    TOK_سرا_دا_سطر,
    TOK_IDENTIFIER,
    TOK_گروہ_NAME,
    TOK_BANG,
    TOK_EQUALS,
    TOK_WILD_CARD_تارا,
    TOK_WILD_CARD_NONE,
    TOK_WILD_CARD_SOME,
    TOK_WILD_CARD_ANY,
    TOK_INCLUDE,
    TOK_ERROR
};

ساكن تعداد قاعدے_token
lex(بنية scanner *s, اتحاد lvalue *val)
{
skip_ہور_whitespace_and_comments:
    /* Skip spaces */
    طالما (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    إذا (scanner_lit(s, "//")) {
        scanner_skip_منزل_eol(s);
    }

    /* New line */
    إذا (scanner_eol(s)) {
        طالما (scanner_eol(s)) scanner_اگلا(s);
        أرجع TOK_سرا_دا_سطر;
    }

    /* Escaped line continuation */
    إذا (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        إذا (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            أرجع TOK_ERROR;
        }
        scanner_اگلا(s);
        اذهب skip_ہور_whitespace_and_comments;
    }

    /* See if we're done */
    إذا (scanner_eof(s)) أرجع TOK_سرا_دا_FILE;

    /* New token */
    s->token_pos = s->pos;
    تعداد قاعدے_token token = TOK_IDENTIFIER;

    ثابت حرف اگلا = scanner_اگلا(s);
    بدل (اگلا) {
    حالة '!':
        أرجع TOK_BANG;
    حالة '$':
        /*گروہ*/
        token = TOK_گروہ_NAME;
        val->string.len = 0;
        اكسر;
    حالة '*':
        إذا (scanner_قاعدے_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            اكسر;
        } وإلا {
            /* Legacy wild card */
            أرجع TOK_WILD_CARD_تارا;
        }
    حالة '<':
        /* Extended wild cards */
        إذا (scanner_قاعدے_lit_token(s, "none>")) أرجع TOK_WILD_CARD_NONE;
        إذا (scanner_قاعدے_lit_token(s, "some>")) أرجع TOK_WILD_CARD_SOME;
        إذا (scanner_قاعدے_lit_token(s, "any>")) أرجع TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        أكد(scanner_قاعدے_is_ident('<'));
        val->string.len = 1;
        اكسر;
    حالة '=':
        إذا (scanner_قاعدے_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            اكسر;
        } وإلا {
            أرجع TOK_EQUALS;
        }
    حالة 'i':
        إذا (scanner_قاعدے_lit_token(s, "nclude")) {
            /* Include statement */
            أرجع TOK_INCLUDE;
        }
        /* Identifier */
        أكد(scanner_قاعدے_is_ident('i'));
        val->string.len = 1;
        اكسر;
    افتراضي:
        إذا (likely(scanner_قاعدے_is_ident(اگلا))) {
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
    أكد(scanner_قاعدے_is_ident(ملاؤ_OVERRIDE_اگیتر));
    أكد(scanner_قاعدے_is_ident(ملاؤ_AUGMENT_اگیتر));
    أكد(scanner_قاعدے_is_ident(ملاؤ_REPLACE_اگیتر));
    أكد(scanner_قاعدے_is_ident('('));
    أكد(scanner_قاعدے_is_ident(')'));
    val->string.len += scanner_قاعدے_ident_token(s);

    إذا (unlikely(val->string.len == 0)) {
        إذا (token == TOK_گروہ_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } وإلا {
            scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                        "unrecognized token");
        }
        أرجع TOK_ERROR;
    }

    أرجع token;
}

/***====================================================================***/

تعداد قاعدے_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_عدد_ENTRIES
};

تعداد {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

تعريفنوع uint8_t mlvo_اشاریہ_t;
تعريفنوع uint8_t mlvo_نقاب_t;


ساكن ثابت بنية sval قاعدے_mlvo_svals[MLVO_عدد_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

تعداد قاعدے_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_عدد_ENTRIES
};

تعريفنوع uint8_t kccgst_اشاریہ_t;
تعريفنوع uint8_t kccgst_نقاب_t;

ساكن ثابت بنية sval قاعدے_kccgst_svals[KCCGST_عدد_ENTRIES] = {
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

static_assert(~(xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL ==
              (xkb_layout_نقاب_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
بنية matched_sval {
    بنية sval sval;
    /** Used for layout-specific options */
    xkb_layout_نقاب_t layouts;
    xkb_layout_نقاب_t matched;
};
تعريفنوع darray(بنية matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
بنية قاعدہ_ناں {
    بنية matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

بنية گروہ {
    بنية sval name;
    darray_sval elements;
};

بنية mapping {
    تعداد قاعدے_mlvo mlvo_at_pos[MLVO_عدد_ENTRIES];
    mlvo_اشاریہ_t عدد_mlvo;
    mlvo_نقاب_t defined_mlvo_نقاب;
    bool has_layout_idx_range;
    bool has_مضرب_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    اتحاد {
        بنية { xkb_layout_اشاریہ_t layout_idx, variant_idx; };
        بنية { xkb_layout_اشاریہ_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    اتحاد {
        xkb_layout_نقاب_t سرگرم;
        xkb_layout_نقاب_t layouts_candidates_نقاب;
    };
    تعداد قاعدے_kccgst kccgst_at_pos[KCCGST_عدد_ENTRIES];
    kccgst_اشاریہ_t عدد_kccgst;
    kccgst_نقاب_t defined_kccgst_نقاب;
};

تعداد mlvo_میل_type {
    /** Match the given plain value */
    MLVO_میل_عام = 0,
    /** Match depending on the value of `wildcard_match_type` */
    MLVO_میل_WILDCARD_LEGACY,
    /** Match empty value */
    MLVO_میل_WILDCARD_NONE,
    /** Match non-empty value */
    MLVO_میل_WILDCARD_SOME,
    /** Match any value, optionally empty */
    MLVO_میل_WILDCARD_ANY,
    /** Match any entry in a group */
    MLVO_میل_گروہ,
};

تعداد wildcard_میل_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_میل_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_میل_ALL,
};

بنية قاعدہ_2 {
    بنية sval mlvo_قدر_at_pos[MLVO_عدد_ENTRIES];
    تعداد mlvo_میل_type میل_type_at_pos[MLVO_عدد_ENTRIES];
    بنية sval kccgst_قدر_at_pos[KCCGST_عدد_ENTRIES];
    mlvo_اشاریہ_t عدد_mlvo_اقدار;
    kccgst_اشاریہ_t عدد_kccgst_اقدار;
    bool skip;
};

#تعریف_کرو ٹکڑا_KCCGST_ثنائی_ہندسہ_میدان_ناپ 4
static_assert(!((1u << (ٹکڑا_KCCGST_ثنائی_ہندسہ_میدان_ناپ - 1)) &
                (KCCGST_عدد_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
بنية kccgst_بفر_ٹکڑا {
    uint32_t لمائی:(32 - ٹکڑا_KCCGST_ثنائی_ہندسہ_میدان_ناپ);
    تعداد قاعدے_kccgst kccgst:ٹکڑا_KCCGST_ثنائی_ہندسہ_میدان_ناپ;
    xkb_layout_اشاریہ_t layout;
};

/* Buffer for pending KcCGST values */
بنية kccgst_بفر {
    darray_char بفر;
    /* Slice corresponding to each value in the buffer */
    darray(بنية kccgst_بفر_ٹکڑا) slices;
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
    بنية قاعدہ_ناں rmlvo;
    اتحاد lvalue val;
    darray(بنية گروہ) groups;
    /* Current mapping. */
    بنية mapping mapping;
    /* Current rule. */
    بنية قاعدہ_2 قاعدہ_2;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    بنية kccgst_بفر pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_عدد_ENTRIES];
};

ساكن بنية sval
strip_spaces(بنية sval v)
{
    طالما (v.len > 0 && is_space(v.شروع[0])) { v.len--; v.شروع++; }
    طالما (v.len > 0 && is_space(v.شروع[v.len - 1])) v.len--;
    أرجع v;
}

ساكن darray_matched_sval
ونڈو_comma_separated_mlvo(بنية xkb_context *ctx,
                           تعداد قاعدے_mlvo mlvo, ثابت حرف *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    إذا (!s) {
        ensure_at_گھٹ_توں_گھٹ_one_قدر:
        /* Label followed by a declaration is a C23 extension */ ;

        بنية matched_sval val = {
            .sval = SVAL(عدم, 0),
            .layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        أرجع arr;
    }

    طالما (true) {
        بنية matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL,
            .matched = 0,
        };
        طالما (*s != '\0' && *s != ',' && *s != OPTIONS_گروہ_SPECIFIER_اگیتر) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        إذا (*s == OPTIONS_گروہ_SPECIFIER_اگیتر) {
            /* Handle numeric layout index */
            s++;
            ثابت حرف* ثابت layout_شروع = s;
            xkb_layout_اشاریہ_t layout = XKB_LAYOUT_INVALID;

            صحيح گنتی = parse_dec_منزل_uint32_t(s, SIZE_MAX, &layout);
            إذا (گنتی > 0) {
                /* Note: 1-indexed layout */
                s += گنتی;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                إذا (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (بنانشانی صحيح) val.sval.len, val.sval.شروع);
                } وإلا إذا (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_سنیہا_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (بنانشانی صحيح) val.sval.len, val.sval.شروع);
                } وإلا {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ثابت حرف* ثابت layout_اشاریہ_سرا = s;
            طالما (*s != '\0' && *s != ',') { s++; }
            إذا (گنتی <= 0 || layout_اشاریہ_سرا != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (بنانشانی صحيح) (s - layout_شروع), layout_شروع,
                        (بنانشانی صحيح) val.sval.len, val.sval.شروع);
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
                            (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } وإلا إذا (prev->layouts != (xkb_layout_نقاب_t)
                                   XKB_OPTION_LAYOUT_نقاب_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        اذهب اگلا_قدر;
                    }
                }
            }
            darray_append(arr, val);
        }

اگلا_قدر:
        إذا (*s == '\0') اكسر;
        إذا (*s == ',') s++;
    }

    إذا (darray_خالی_تھاواں(arr))
        اذهب ensure_at_گھٹ_توں_گھٹ_one_قدر;

    أرجع arr;
}

ساكن بنية matcher *
matcher_new_from_rmlvo(ثابت بنية xkb_rmlvo_builder *rmlvo, ثابت حرف **قاعدے)
{
    بنية matcher *m = صفر_حافظہ_دیو(1, حجم(*m));
    إذا (!m)
        أرجع عدم;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    بنية xkb_قاعدہ_ناں ناں = {
        .قاعدے = rmlvo->قاعدے,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_خالی_تھاواں(rmlvo->layouts)) ? عدم : "x",
        .variant = (darray_خالی_تھاواں(rmlvo->layouts)) ? عدم : "x",
        .options = (darray_خالی_تھاواں(rmlvo->options)) ? عدم : "x",
    };
    ثابت تعداد RMLVO بدلیا = xkb_context_sanitize_قاعدہ_ناں(rmlvo->ctx,
                                                               &ناں);

    /* Initialize matcher with sanitized names, if relevant */
    إذا (بدلیا & RMLVO_قاعدے) {
        *قاعدے = ناں.قاعدے;
    } وإلا {
        *قاعدے = rmlvo->قاعدے;
    }

    إذا (بدلیا & RMLVO_MODEL) {
        m->rmlvo.model.sval.شروع = ناں.model;
    } وإلا {
        m->rmlvo.model.sval.شروع = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_محفوظ(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL;

    أكد((بدلیا & RMLVO_LAYOUT) || !(بدلیا & RMLVO_VARIANT));
    إذا (بدلیا & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = ونڈو_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      ناں.layout);
        m->rmlvo.variants = ونڈو_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       ناں.variant);
        إذا (darray_ناپ(m->rmlvo.layouts) > darray_ناپ(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            إذا (!isempty(ناں.variant))
                log_warn(m->ctx, XKB_LOG_سنیہا_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         ناں.layout ? ناں.layout : "(none)",
                         ناں.variant ? ناں.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_ناپ(m->rmlvo.layouts));
        } وإلا إذا (darray_ناپ(m->rmlvo.layouts) < darray_ناپ(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_سنیہا_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    ناں.layout ? ناں.layout : "(none)",
                    ناں.variant ? ناں.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_ناپ(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } وإلا {
        بنية xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            بنية matched_sval val = {
                .sval = SVAL(layout->layout, strlen_محفوظ(layout->layout)),
                .layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL,
                .matched = 0
            };
            darray_append(m->rmlvo.layouts, val);
            val.sval.شروع = layout->variant;
            val.sval.len = strlen_محفوظ(layout->variant);
            darray_append(m->rmlvo.variants, val);
        }
    }

    إذا (بدلیا & RMLVO_OPTIONS) {
        m->rmlvo.options = ونڈو_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      ناں.options);
    } وإلا {
        بنية xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            بنية matched_sval val = {
                .sval = SVAL(option->option, strlen_محفوظ(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    أرجع m;
}

ساكن بنية matcher *
matcher_new_from_ناں(بنية xkb_context *ctx,
            ثابت بنية xkb_قاعدہ_ناں *rmlvo)
{
    بنية matcher *m = صفر_حافظہ_دیو(1, حجم(*m));
    إذا (!m)
        أرجع عدم;

    m->ctx = ctx;
    m->rmlvo.model.sval.شروع = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_محفوظ(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL;
    m->rmlvo.layouts = ونڈو_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = ونڈو_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = ونڈو_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    إذا (darray_ناپ(m->rmlvo.layouts) > darray_ناپ(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        إذا (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_سنیہا_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_ناپ(m->rmlvo.layouts));
    } وإلا إذا (darray_ناپ(m->rmlvo.layouts) < darray_ناپ(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_ناپ(m->rmlvo.layouts));
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
    بنية گروہ *گروہ;
    darray_foreach(گروہ, m->groups)
        darray_free(گروہ->elements);
    darray_free(m->pending_kccgst.بفر);
    darray_free(m->pending_kccgst.slices);
    لكل (kccgst_اشاریہ_t i = 0; i < (kccgst_اشاریہ_t) KCCGST_عدد_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    حرر(m);
}

ساكن فراغ
matcher_گروہ_شروع_new(بنية matcher *m, بنية sval name)
{
    بنية گروہ گروہ = { .name = name, .elements = darray_new() };
    darray_append(m->groups, گروہ);
}

ساكن فراغ
matcher_گروہ_جوڑو_element(بنية matcher *m, بنية scanner *s,
                          بنية sval element)
{
    darray_append(darray_شے(m->groups, darray_ناپ(m->groups) - 1).elements,
                  element);
}

ساكن bool
read_قاعدے_file(بنية xkb_context *ctx,
                بنية matcher *matcher,
                بنانشانی صحيح include_ڈونگھائی,
                FILE *file,
                ثابت حرف *رستہ);

ساكن فراغ
matcher_include(بنية matcher *m, بنية scanner *ماپا_scanner,
                بنانشانی صحيح include_ڈونگھائی,
                بنية sval inc)
{
    إذا (include_ڈونگھائی >= MAX_INCLUDE_ڈونگھائی) {
        scanner_err(ماپا_scanner, XKB_LOG_سنیہا_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_ڈونگھائی);
        أرجع;
    }

    ثابت حرف *stmt_file = inc.شروع;
    ناپ_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    حرف buf[PATH_MAX] = {0};
    ثابت ssize_t expanded = expand_رستہ(m->ctx, ماپا_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_قاعدے,
                                         buf, حجم(buf));
    إذا (expanded < 0) {
        /* Error */
        أرجع;
    } وإلا إذا (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (ناپ_t) expanded;
        أكد(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    بنانشانی صحيح وتھ = 0;
    ثابت bool absolute_رستہ = is_absolute_رستہ(stmt_file);
    إذا (absolute_رستہ) {
        /* Absolute path: no need for lookup in XKB paths */
        إذا (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            إذا (stmt_file_len < حجم(buf)) {
                حافظہ_نقل(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } وإلا {
                log_err(m->ctx, XKB_ERROR_INVALID_رستہ,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, حجم(buf),
                        (بنانشانی صحيح) stmt_file_len, stmt_file);
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
            file = لبھو_File_وچ_Xkb_رستہ(m->ctx, ماپا_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_قاعدے,
                                     buf, حجم(buf), &وتھ, true);
        }
    }

    طالما (file) {
        أكد(strlen_محفوظ(buf) < حجم(buf));
        bool ret = read_قاعدے_file(m->ctx, m, include_ڈونگھائی + 1, file, buf);
        fclose(file);
        إذا (ret)
            أرجع;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_سنیہا_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        إذا (absolute_رستہ) {
            /* There is no point to search further if the path is absolute */
            اكسر;
        }

        /* Try next XKB path */
        وتھ++;
        file = لبھو_File_وچ_Xkb_رستہ(m->ctx, ماپا_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_قاعدے,
                                 buf, حجم(buf), &وتھ, true);
    }

    log_err(m->ctx, XKB_LOG_سنیہا_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (بنانشانی صحيح) stmt_file_len, stmt_file);
}

ساكن فراغ
matcher_mapping_شروع_new(بنية matcher *m)
{
    لكل (mlvo_اشاریہ_t i = 0; i < (mlvo_اشاریہ_t) MLVO_عدد_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_عدد_ENTRIES;
    لكل (kccgst_اشاریہ_t i = 0; i < (kccgst_اشاریہ_t) KCCGST_عدد_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_عدد_ENTRIES;
    m->mapping.has_مضرب_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.عدد_mlvo = m->mapping.عدد_kccgst = 0;
    m->mapping.defined_mlvo_نقاب = 0;
    m->mapping.defined_kccgst_نقاب = 0;
    m->mapping.سرگرم = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
ساكن صحيح
parse_layout_int_اشاریہ_پوچھ(ثابت حرف *s, ناپ_t max_len,
                            xkb_layout_اشاریہ_t *باہرلا)
{
    إذا (max_len < 3)
        أرجع -1;
    uint32_t val = 0;
    ثابت صحيح گنتی = parse_dec_منزل_uint32_t(&s[1], max_len - 2, &val);
    إذا (گنتی <= 0 || s[1 + گنتی] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        أرجع -1;
    /* To zero-based index. */
    *باہرلا = val - 1;
    أرجع گنتی + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
ساكن صحيح
کڈھو_layout_اشاریہ(ثابت حرف *s, ناپ_t max_len, xkb_layout_اشاریہ_t *باہرلا)
{
    /* This function is pretty stupid, but works for now. */
    *باہرلا = XKB_LAYOUT_INVALID;
    إذا (max_len < 3 || s[0] != '[')
        أرجع -1;
    إذا (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *باہرلا = (xkb_layout_اشاریہ_t)(s[1] - '1');
        أرجع 3;
    }
    إذا (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        أرجع 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    أرجع parse_layout_int_اشاریہ_پوچھ(s, max_len, باہرلا);
}

/* Special layout indices */
تعداد layout_اشاریہ_ranges {
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
ساكن bool
کڈھو_mapping_layout_اشاریہ(ثابت حرف *s, ناپ_t max_len,
                             xkb_layout_اشاریہ_t *باہرلا)
{
    /* Check for minimal `[` + index + `]` */
    إذا (max_len < 3 || s[0] != '[') {
        *باہرلا = XKB_LAYOUT_INVALID;
        أرجع false;
    }

    /* Small numeric index */
    إذا (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *باہرلا = (xkb_layout_اشاریہ_t)(s[1] - '1');
        أرجع true;
    }

    /* Named indices ranges */
    ساكن ثابت بنية {
        ثابت حرف* name;
        ناپ_t ناپ;
        xkb_layout_اشاریہ_t قدر;
    } ناں[] = {
        #تعریف_کرو اشاریہ_RANGE(پوچھ, قدر) \
            { (پوچھ), حجم(پوچھ), (xkb_layout_اشاریہ_t)(قدر) }
        اشاریہ_RANGE("single]",   LAYOUT_اشاریہ_SINGLE),
        اشاریہ_RANGE("first]",    LAYOUT_اشاریہ_پہلا),
        اشاریہ_RANGE("later]",    LAYOUT_اشاریہ_LATER),
        اشاریہ_RANGE("multiple]", LAYOUT_اشاریہ_مضرب),
        اشاریہ_RANGE("any]",      LAYOUT_اشاریہ_ANY),
        #تعریف_رد اشاریہ_RANGE
    };

    لكل (ناپ_t k = 0; k < ARRAY_ناپ(ناں); k++) {
        إذا (max_len == ناں[k].ناپ &&
            memcmp(&s[1], ناں[k].name, ناں[k].ناپ - 1) == 0) {
            *باہرلا = ناں[k].قدر;
            أرجع true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *باہرلا = XKB_LAYOUT_INVALID;
    ثابت صحيح گنتی = parse_layout_int_اشاریہ_پوچھ(s, max_len, باہرلا);
    أرجع گنتی > 0 && (ناپ_t)گنتی == max_len;
}

ساكن مضمن bool
is_mlvo_نقاب_defined(بنية matcher *m, تعداد قاعدے_mlvo mlvo)
{
    أرجع m->mapping.defined_mlvo_نقاب & (1u << mlvo);
}

ساكن فراغ
matcher_mapping_سیٹ_mlvo(بنية matcher *m, بنية scanner *s,
                         بنية sval ident)
{
    تعداد قاعدے_mlvo mlvo;
    بنية sval mlvo_sval;

    لكل (mlvo = 0; mlvo < MLVO_عدد_ENTRIES; mlvo++) {
        mlvo_sval = قاعدے_mlvo_svals[mlvo];

        إذا (svaleq_اگیتر(mlvo_sval, ident))
            اكسر;
    }

    /* Not found. */
    إذا (mlvo >= MLVO_عدد_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (بنانشانی صحيح) ident.len, ident.شروع);
        m->mapping.سرگرم = false;
        أرجع;
    }

    إذا (is_mlvo_نقاب_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (بنانشانی صحيح) mlvo_sval.len, mlvo_sval.شروع);
        m->mapping.سرگرم = false;
        أرجع;
    }

    /* If there are leftovers still, it must be an index. */
    إذا (mlvo_sval.len < ident.len) {
        xkb_layout_اشاریہ_t idx;
        إذا (!کڈھو_mapping_layout_اشاریہ(ident.شروع + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (بنانشانی صحيح) mlvo_sval.len, mlvo_sval.شروع);
            m->mapping.سرگرم = false;
            أرجع;
        }

        إذا (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        وإلا إذا (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        وإلا {
            scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (بنانشانی صحيح) mlvo_sval.len, mlvo_sval.شروع);
            m->mapping.سرگرم = false;
            أرجع;
        }
    } وإلا إذا (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_SINGLE;
    } وإلا إذا (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_اشاریہ_t) LAYOUT_اشاریہ_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    إذا (((mlvo == MLVO_LAYOUT && is_mlvo_نقاب_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_نقاب_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.سرگرم = false;
        أرجع;
    }

    m->mapping.mlvo_at_pos[m->mapping.عدد_mlvo] = mlvo;
    m->mapping.defined_mlvo_نقاب |= (mlvo_نقاب_t) 1u << mlvo;
    m->mapping.عدد_mlvo++;
}

ساكن فراغ
matcher_mapping_سیٹ_layout_bounds(بنية matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_اشاریہ_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    بدل (idx) {
        حالة XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            أكد(!is_mlvo_نقاب_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_نقاب_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_نقاب = 0x1; /* active = true */
            اكسر;
        حالة LAYOUT_اشاریہ_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_ناپ(m->rmlvo.layouts));
            m->mapping.layouts_candidates_نقاب = (xkb_layout_نقاب_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            اكسر;
        حالة LAYOUT_اشاریہ_مضرب:
        حالة LAYOUT_اشاریہ_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_ناپ(m->rmlvo.layouts));
            m->mapping.layouts_candidates_نقاب = (xkb_layout_نقاب_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            اكسر;
        حالة LAYOUT_اشاریہ_SINGLE:
        حالة LAYOUT_اشاریہ_پہلا:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        افتراضي:
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

ساكن فراغ
matcher_mapping_سیٹ_kccgst(بنية matcher *m, بنية scanner *s, بنية sval ident)
{
    تعداد قاعدے_kccgst kccgst;
    بنية sval kccgst_sval;

    لكل (kccgst = 0; kccgst < KCCGST_عدد_ENTRIES; kccgst++) {
        kccgst_sval = قاعدے_kccgst_svals[kccgst];

        إذا (svaleq(قاعدے_kccgst_svals[kccgst], ident))
            اكسر;
    }

    /* Not found. */
    إذا (kccgst >= KCCGST_عدد_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (بنانشانی صحيح) ident.len, ident.شروع);
        m->mapping.سرگرم = false;
        أرجع;
    }

    إذا (m->mapping.defined_kccgst_نقاب & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (بنانشانی صحيح) kccgst_sval.len, kccgst_sval.شروع);
        m->mapping.سرگرم = false;
        أرجع;
    }

    m->mapping.kccgst_at_pos[m->mapping.عدد_kccgst] = kccgst;
    m->mapping.defined_kccgst_نقاب |= (kccgst_نقاب_t) 1u << kccgst;
    m->mapping.عدد_kccgst++;
}

ساكن bool
matcher_mapping_verify(بنية matcher *m, بنية scanner *s)
{
    إذا (m->mapping.عدد_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        اذهب skip;
    }

    إذا (m->mapping.عدد_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        اذهب skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    إذا (is_mlvo_نقاب_defined(m, MLVO_LAYOUT)) {
        أكد(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        بدل (m->mapping.layout_idx) {
            حالة LAYOUT_اشاریہ_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                إذا (darray_ناپ(m->rmlvo.layouts) > 1)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_اشاریہ_مضرب:
            حالة LAYOUT_اشاریہ_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                إذا (darray_ناپ(m->rmlvo.layouts) < 2)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_اشاریہ_ANY:
            حالة LAYOUT_اشاریہ_پہلا:
                /* No restrictions */
                اكسر;
            افتراضي:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                إذا (darray_ناپ(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_ناپ(m->rmlvo.layouts))
                    اذهب skip;
        }
    }

    إذا (is_mlvo_نقاب_defined(m, MLVO_VARIANT)) {
        أكد(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        بدل (m->mapping.variant_idx) {
            حالة LAYOUT_اشاریہ_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                إذا (darray_ناپ(m->rmlvo.variants) > 1)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_اشاریہ_ANY:
            حالة LAYOUT_اشاریہ_LATER:
            حالة LAYOUT_اشاریہ_پہلا:
                /* No restriction */
                اكسر;
            افتراضي:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                إذا (darray_ناپ(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_ناپ(m->rmlvo.variants))
                    اذهب skip;
        }
    }

    أرجع true;

skip:
    m->mapping.سرگرم = false;
    أرجع false;
}

ساكن فراغ
matcher_قاعدہ_شروع_new(بنية matcher *m)
{
    memset(&m->قاعدہ_2, 0, حجم(m->قاعدہ_2));
    m->قاعدہ_2.skip = !m->mapping.سرگرم;
}

ساكن فراغ
matcher_قاعدہ_سیٹ_mlvo_common(بنية matcher *m, بنية scanner *s,
                             بنية sval ident,
                             تعداد mlvo_میل_type میل_type)
{
    إذا (m->قاعدہ_2.عدد_mlvo_اقدار >= m->mapping.عدد_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->قاعدہ_2.skip = true;
        أرجع;
    }
    m->قاعدہ_2.میل_type_at_pos[m->قاعدہ_2.عدد_mlvo_اقدار] = میل_type;
    m->قاعدہ_2.mlvo_قدر_at_pos[m->قاعدہ_2.عدد_mlvo_اقدار] = ident;
    m->قاعدہ_2.عدد_mlvo_اقدار++;
}

ساكن فراغ
matcher_قاعدہ_سیٹ_mlvo_wildcard(بنية matcher *m, بنية scanner *s,
                               تعداد mlvo_میل_type میل_type)
{
    بنية sval dummy = SVAL(عدم, 0);
    matcher_قاعدہ_سیٹ_mlvo_common(m, s, dummy, میل_type);
}

ساكن فراغ
matcher_قاعدہ_سیٹ_mlvo_گروہ(بنية matcher *m, بنية scanner *s,
                            بنية sval ident)
{
    matcher_قاعدہ_سیٹ_mlvo_common(m, s, ident, MLVO_میل_گروہ);
}

ساكن فراغ
matcher_قاعدہ_سیٹ_mlvo(بنية matcher *m, بنية scanner *s,
                      بنية sval ident)
{
    matcher_قاعدہ_سیٹ_mlvo_common(m, s, ident, MLVO_میل_عام);
}

ساكن فراغ
matcher_قاعدہ_سیٹ_kccgst(بنية matcher *m, بنية scanner *s,
                        بنية sval ident)
{
    إذا (m->قاعدہ_2.عدد_kccgst_اقدار >= m->mapping.عدد_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->قاعدہ_2.skip = true;
        أرجع;
    }
    m->قاعدہ_2.kccgst_قدر_at_pos[m->قاعدہ_2.عدد_kccgst_اقدار] = ident;
    m->قاعدہ_2.عدد_kccgst_اقدار++;
}

ساكن bool
میل_گروہ(بنية matcher *m, بنية sval گروہ_name, بنية sval منزل)
{
    بنية گروہ *گروہ = عدم;
    بنية sval *element;
    bool لبھیا = false;

    darray_foreach(گروہ, m->groups) {
        إذا (svaleq(گروہ->name, گروہ_name)) {
            لبھیا = true;
            اكسر;
        }
    }

    إذا (!لبھیا) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        أرجع false;
    }

    darray_foreach(element, گروہ->elements)
        إذا (svaleq(منزل, *element))
            أرجع true;

    أرجع false;
}

ساكن bool
میل_قدر(بنية matcher *m, ثابت بنية sval val, ثابت بنية sval منزل,
            تعداد mlvo_میل_type میل_type,
            تعداد wildcard_میل_type wildcard_type)
{
    بدل (میل_type) {
        حالة MLVO_میل_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            أرجع wildcard_type == WILDCARD_میل_ALL || !!منزل.len;
        حالة MLVO_میل_WILDCARD_NONE:
            أرجع !منزل.len;
        حالة MLVO_میل_WILDCARD_SOME:
            أرجع !!منزل.len;
        حالة MLVO_میل_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            أرجع true;
        حالة MLVO_میل_گروہ:
            أرجع میل_گروہ(m, val, منزل);
        افتراضي:
            أكد(میل_type == MLVO_میل_عام);
            أرجع svaleq(val, منزل);
    }
}

ساكن bool
میل_قدر_and_mark(بنية matcher *m, ثابت بنية sval val,
                     بنية matched_sval *منزل, تعداد mlvo_میل_type میل_type,
                     تعداد wildcard_میل_type wildcard_type,
                     xkb_layout_نقاب_t layouts)
{
    bool matched = میل_قدر(m, val, منزل->sval, میل_type, wildcard_type);
    إذا (matched)
        منزل->matched |= layouts;
    أرجع matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
ساكن bool
expand_rmlvo_وچ_kccgst_قدر(بنية matcher *m, بنية scanner *s,
                             بنية sval قدر, xkb_layout_اشاریہ_t layout_idx,
                             darray_char *expanded, ناپ_t *i)
{
    ثابت حرف *str = قدر.شروع;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    تعداد قاعدے_mlvo mlv;
    xkb_layout_اشاریہ_t idx;
    حرف pfx, sfx;
    بنية matched_sval *expanded_قدر;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    إذا (str[*i] == 'i' &&
        (*i + 1 == قدر.len || is_ملاؤ_منوال_اگیتر(str[*i + 1])))
    {
        إذا (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_قاعدے_INVALID_LAYOUT_اشاریہ_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            اذهب error;
        }
        (*i)++;
        حرف اشاریہ_str[MAX_LAYOUT_اشاریہ_STR_لمائی + 1];
        ثابت صحيح گنتی = snprintf(اشاریہ_str, حجم(اشاریہ_str), "%"PRIu32,
                                   layout_idx + 1);
        أكد(گنتی > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, اشاریہ_str, (darray_ناپ_t)گنتی);
        أرجع true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    إذا (str[*i] == '(' ||
        is_ملاؤ_منوال_اگیتر(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        إذا (str[*i] == '(') sfx = ')';
        إذا (++(*i) >= قدر.len) اذهب error;
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
    bool variable_اشاریہ = false;
    إذا (*i < قدر.len && str[*i] == '[') {
        إذا (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            اذهب error;
        }

        صحيح consumed = کڈھو_layout_اشاریہ(str + (*i), قدر.len - (*i), &idx);
        إذا (consumed <= 0) اذهب error;
        إذا (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            أكد(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_اشاریہ = true;
        }
        *i += (ناپ_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    إذا (sfx != 0) {
        إذا (*i >= قدر.len) اذهب error;
        إذا (str[(*i)++] != sfx) اذهب error;
    }

    /* Get the expanded value. */
    expanded_قدر = عدم;

    إذا (mlv == MLVO_LAYOUT) {
        إذا (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            إذا (darray_ناپ(m->rmlvo.layouts) == 1)
                expanded_قدر = &darray_شے(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } وإلا إذا (idx < darray_ناپ(m->rmlvo.layouts) &&
                   (variable_اشاریہ || darray_ناپ(m->rmlvo.layouts) > 1)) {
                expanded_قدر = &darray_شے(m->rmlvo.layouts, idx);
        }
    }
    وإلا إذا (mlv == MLVO_VARIANT) {
        إذا (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            إذا (darray_ناپ(m->rmlvo.variants) == 1)
                expanded_قدر = &darray_شے(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } وإلا إذا (idx < darray_ناپ(m->rmlvo.variants) &&
                   (variable_اشاریہ || darray_ناپ(m->rmlvo.variants) > 1)) {
                expanded_قدر = &darray_شے(m->rmlvo.variants, idx);
        }
    }
    وإلا إذا (mlv == MLVO_MODEL) {
        expanded_قدر = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    إذا (!expanded_قدر || expanded_قدر->sval.len == 0) {
        أرجع true;
    }

    إذا (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_قدر->sval.شروع,
                                 (darray_ناپ_t) expanded_قدر->sval.len);
    إذا (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    إذا (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ثابت xkb_layout_نقاب_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_قدر->matched |= layouts;
    } وإلا {
        expanded_قدر->matched |= (xkb_layout_نقاب_t)GLOBAL_MATCHED_LAYOUTS;
    }

    أرجع true;

error:
    scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                "invalid %%-expansion in value; not used");
    أرجع false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
ساكن فراغ
expand_qualifier_وچ_kccgst_قدر(
    بنية matcher *m, بنية scanner *s,
    بنية sval قدر, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_ناپ_t اگیتر_idx, ناپ_t *i)
{
    ثابت حرف *str = قدر.شروع;

    /* “all” followed by nothing or by a layout separator */
    إذا (*i + 2 < قدر.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == قدر.len || is_ملاؤ_منوال_اگیتر(str[*i + 3]))) {
        إذا (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_سنیہا_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        إذا (darray_ناپ(m->rmlvo.layouts) > 1) {
            حرف layout_اشاریہ[MAX_LAYOUT_اشاریہ_STR_لمائی + 1];
            ثابت darray_ناپ_t اگیتر_لمائی =
                darray_ناپ(*expanded) - اگیتر_idx - 1;
            لكل (xkb_layout_اشاریہ_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_ناپ(m->rmlvo.layouts));
                 l++)
            {
                إذا (!has_separator)
                    darray_append(*expanded, ملاؤ_DEFAULT_اگیتر);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_شے(*expanded, اگیتر_idx),
                                             اگیتر_لمائی);
                /* Append index */
                ثابت صحيح گنتی = snprintf(layout_اشاریہ, حجم(layout_اشاریہ),
                                           "%"PRIu32, l + 1);
                أكد(گنتی > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_اشاریہ,
                                             (darray_ناپ_t)گنتی);
            }
        }
        *i += 3;
    }
}

ساكن مضمن فراغ
#جے_تعریف_شدہ _MSC_VER
concat_kccgst(darray_char *into, darray_ناپ_t ناپ, وچ_reads(ناپ) ثابت حرف* from)
#وإلا
concat_kccgst(darray_char *into, darray_ناپ_t ناپ, ثابت حرف from[ساكن ناپ])
#جے_انت
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ثابت bool from_plus = is_ملاؤ_منوال_اگیتر(from[0]);
    إذا (from_plus || darray_خالی_تھاواں(*into)) {
        darray_appends_nullterminate(*into, from, ناپ);
    } وإلا {
        ثابت حرف ch =
            (حرف) (darray_خالی_تھاواں(*into) ? '\0' : darray_شے(*into, 0));
        ثابت bool into_plus = is_ملاؤ_منوال_اگیتر(ch);
        إذا (into_plus)
            darray_prepends_nullterminate(*into, from, ناپ);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
ساكن bool
append_expanded_kccgst_قدر(بنية matcher *m, بنية scanner *s,
                             bool ملاؤ, darray_char *منزل, بنية sval قدر,
                             xkb_layout_اشاریہ_t layout_idx)
{
    ثابت حرف *str = قدر.شروع;
    darray_char expanded = darray_new();
    darray_ناپ_t آخری_شے_idx = 0;
    bool has_separator = false;

    لكل (ناپ_t i = 0; i < قدر.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        بدل (str[i]) {
            /* Qualifier */
            حالة ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_وچ_kccgst_قدر(m, s, قدر, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 آخری_شے_idx, &i);
                اكسر;
            /* Expansion */
            حالة '%':
                i++;
                إذا (i >= قدر.len ||
                    !expand_rmlvo_وچ_kccgst_قدر(m, s, قدر, layout_idx,
                                                  &expanded, &i))
                        اذهب error;
                اكسر;
            /* New item */
            حالة ملاؤ_OVERRIDE_اگیتر:
            حالة ملاؤ_AUGMENT_اگیتر:
            حالة ملاؤ_REPLACE_اگیتر:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                آخری_شے_idx = darray_ناپ(expanded) - 1;
                has_separator = true;
                اكسر;
            /* Just a normal character. */
            افتراضي:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    إذا (ملاؤ) {
        إذا (!darray_خالی_تھاواں(expanded))
            concat_kccgst(منزل, darray_ناپ(expanded), darray_اشیاء(expanded));
    } وإلا {
        darray_concat(*منزل, expanded);
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
    لكل (kccgst_اشاریہ_t i = 0; i < m->mapping.عدد_kccgst; i++) {
        ثابت تعداد قاعدے_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        لكل (xkb_layout_اشاریہ_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            سجل ثابت بنية kccgst_بفر* ثابت buf = &m->pending_kccgst;
            ناپ_t وتھ = 0;
            لكل (darray_ناپ_t k = 0; k < darray_ناپ(buf->slices); k++) {
                سجل ثابت بنية kccgst_بفر_ٹکڑا * ثابت ٹکڑا =
                    &darray_شے(buf->slices, k);
                إذا (ٹکڑا->kccgst == kccgst && ٹکڑا->layout == layout &&
                    ٹکڑا->لمائی)
                    concat_kccgst(&m->kccgst[kccgst], ٹکڑا->لمائی,
                                  darray_اشیاء(buf->بفر) + وتھ);
                وتھ += ٹکڑا->لمائی;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_مضرب_layouts = false;
    أرجع true;
}

ساكن فراغ
matcher_قاعدہ_verify(بنية matcher *m, بنية scanner *s)
{
    إذا (m->قاعدہ_2.عدد_mlvo_اقدار != m->mapping.عدد_mlvo ||
        m->قاعدہ_2.عدد_kccgst_اقدار != m->mapping.عدد_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
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
ساكن bool
matcher_قاعدہ_میل_model(بنية matcher *m, ثابت بنية sval قدر,
                         تعداد mlvo_میل_type میل_type)
{
    أرجع میل_قدر_and_mark(
        m, قدر, &m->rmlvo.model, میل_type, WILDCARD_میل_ALL,
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
ساكن bool
matcher_قاعدہ_میل_option(بنية matcher *m, ثابت بنية sval قدر,
                          تعداد mlvo_میل_type میل_type,
                          xkb_layout_نقاب_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_خالی_تھاواں(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_نقاب_t unmatched =
        (m->mapping.defined_mlvo_نقاب & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    بنية matched_sval *option;
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
        إذا (matchable && میل_قدر_and_mark(m, قدر, option, میل_type,
                                              WILDCARD_میل_ALL, matchable)) {
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
matcher_قاعدہ_میل_layout_or_variant(بنية matcher *m,
                                     darray_matched_sval *داخلہ,
                                     ثابت بنية sval قدر,
                                     تعداد mlvo_میل_type میل_type,
                                     xkb_layout_نقاب_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_خالی_تھاواں(*داخلہ));

    bool matched = false;
    /* Loop over the layout index range */
    لكل (xkb_layout_اشاریہ_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ثابت xkb_layout_نقاب_t layout = (UINT32_C(1) << idx);
        إذا (layout & *candidates) {
            بنية matched_sval *منزل = &darray_شے(*داخلہ, idx);
            إذا (میل_قدر_and_mark(m, قدر, منزل, میل_type,
                                     WILDCARD_میل_NONEMPTY, layout)) {
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
matcher_قاعدہ_apply_if_matches(بنية matcher *m, بنية scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_نقاب_t candidate_layouts = m->mapping.layouts_candidates_نقاب;

    /* Loop over MLVO pattern components */
    لكل (mlvo_اشاریہ_t i = 0; i < m->mapping.عدد_mlvo; i++) {
        ثابت تعداد قاعدے_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ثابت بنية sval قدر = m->قاعدہ_2.mlvo_قدر_at_pos[i];
        ثابت تعداد mlvo_میل_type میل_type = m->قاعدہ_2.میل_type_at_pos[i];
        bool matched = false;

        بدل (mlvo) {
        حالة MLVO_MODEL:
            matched = matcher_قاعدہ_میل_model(m, قدر, میل_type);
            اكسر;
        حالة MLVO_LAYOUT:
            matched = matcher_قاعدہ_میل_layout_or_variant(
                m, &m->rmlvo.layouts, قدر, میل_type, &candidate_layouts
            );
            اكسر;
        حالة MLVO_VARIANT:
            matched = matcher_قاعدہ_میل_layout_or_variant(
                m, &m->rmlvo.variants, قدر, میل_type, &candidate_layouts
            );
            اكسر;
        حالة MLVO_OPTION:
            matched = matcher_قاعدہ_میل_option(
                m, قدر, میل_type, &candidate_layouts
            );
            اكسر;
        افتراضي: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_عدد_ENTRIES - 1,
                          "Unexpected MLVO field");
            أكد(!"Unreachable");
        }
        }

        إذا (!matched)
            أرجع;
    }

    إذا (m->mapping.has_مضرب_layouts) {
        /* Special index: loop over the index range */
        لكل (xkb_layout_اشاریہ_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            إذا (candidate_layouts & (UINT32_C(1) << idx)) {
                لكل (kccgst_اشاریہ_t i = 0; i < m->mapping.عدد_kccgst; i++) {
                    ثابت تعداد قاعدے_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ثابت بنية sval قدر = m->قاعدہ_2.kccgst_قدر_at_pos[i];
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
                    بنية kccgst_بفر * ثابت buf = &m->pending_kccgst;
                    ثابت darray_ناپ_t prev_بفر_لمائی =
                        darray_ناپ(buf->بفر);
                    append_expanded_kccgst_قدر(m, s, false, &buf->بفر,
                                                 قدر, idx);
                    ثابت uint32_t لمائی = (uint32_t) (darray_ناپ(buf->بفر)
                                          - prev_بفر_لمائی);
                    ثابت بنية kccgst_بفر_ٹکڑا ٹکڑا = {
                        .لمائی = لمائی,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, ٹکڑا);
                }
            }
        }
    } وإلا {
        /* Numeric index or no index */
        لكل (kccgst_اشاریہ_t i = 0; i < m->mapping.عدد_kccgst; i++) {
            تعداد قاعدے_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            بنية sval قدر = m->قاعدہ_2.kccgst_قدر_at_pos[i];
            append_expanded_kccgst_قدر(m, s, true, &m->kccgst[kccgst], قدر,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    إذا (!(is_mlvo_نقاب_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_نقاب &= ~candidate_layouts;
    }
}

ساكن تعداد قاعدے_token
gettok(بنية matcher *m, بنية scanner *s)
{
    أرجع lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
ساكن bool
matcher_میل(بنية matcher *m, بنية scanner *s,
              بنانشانی صحيح include_ڈونگھائی,
              ثابت حرف *string, ناپ_t len,
              ثابت حرف *file_name)
{
    تعداد قاعدے_token tok;

    إذا (!m)
        أرجع false;

initial:
    بدل (tok = gettok(m, s)) {
    حالة TOK_BANG:
        اذهب bang;
    حالة TOK_سرا_دا_سطر:
        اذهب initial;
    حالة TOK_سرا_دا_FILE:
        اذهب انت;
    افتراضي:
        اذهب unexpected;
    }

bang:
    بدل (tok = gettok(m, s)) {
    حالة TOK_گروہ_NAME:
        matcher_گروہ_شروع_new(m, m->val.string);
        اذهب گروہ_name;
    حالة TOK_INCLUDE:
        اذهب include_statement;
    حالة TOK_IDENTIFIER:
        matcher_mapping_شروع_new(m);
        matcher_mapping_سیٹ_mlvo(m, s, m->val.string);
        اذهب mapping_mlvo;
    افتراضي:
        اذهب unexpected;
    }

گروہ_name:
    بدل (tok = gettok(m, s)) {
    حالة TOK_EQUALS:
        اذهب گروہ_element;
    افتراضي:
        اذهب unexpected;
    }

گروہ_element:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        matcher_گروہ_جوڑو_element(m, s, m->val.string);
        اذهب گروہ_element;
    حالة TOK_سرا_دا_سطر:
        اذهب initial;
    افتراضي:
        اذهب unexpected;
    }

include_statement:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        matcher_include(m, s, include_ڈونگھائی, m->val.string);
        اذهب include_statement_سرا;
    افتراضي:
        اذهب unexpected;
    }

include_statement_سرا:
    بدل (tok = gettok(m, s)) {
    حالة TOK_سرا_دا_سطر:
        اذهب initial;
    افتراضي:
        اذهب unexpected;
    }

mapping_mlvo:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        إذا (m->mapping.سرگرم)
            matcher_mapping_سیٹ_mlvo(m, s, m->val.string);
        اذهب mapping_mlvo;
    حالة TOK_EQUALS:
        اذهب mapping_kccgst;
    افتراضي:
        اذهب unexpected;
    }

mapping_kccgst:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        إذا (m->mapping.سرگرم)
            matcher_mapping_سیٹ_kccgst(m, s, m->val.string);
        اذهب mapping_kccgst;
    حالة TOK_سرا_دا_سطر:
        إذا (m->mapping.سرگرم && matcher_mapping_verify(m, s)) {
            matcher_mapping_سیٹ_layout_bounds(m);
            إذا (m->mapping.has_مضرب_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_ناپ(m->pending_kccgst.بفر) = 0;
                darray_ناپ(m->pending_kccgst.slices) = 0;
            }
        }
        اذهب قاعدہ_mlvo_پہلا;
    افتراضي:
        اذهب unexpected;
    }

قاعدہ_mlvo_پہلا:
    بدل (tok = gettok(m, s)) {
    حالة TOK_BANG:
        matcher_append_pending_kccgst(m);
        اذهب bang;
    حالة TOK_سرا_دا_سطر:
        اذهب قاعدہ_mlvo_پہلا;
    حالة TOK_سرا_دا_FILE:
        matcher_append_pending_kccgst(m);
        اذهب انت;
    افتراضي:
        matcher_قاعدہ_شروع_new(m);
        اذهب قاعدہ_mlvo_no_tok;
    }

قاعدہ_mlvo:
    tok = gettok(m, s);
قاعدہ_mlvo_no_tok:
    بدل (tok) {
    حالة TOK_IDENTIFIER:
        إذا (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo(m, s, m->val.string);
        اذهب قاعدہ_mlvo;
    حالة TOK_WILD_CARD_تارا:
        إذا (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo_wildcard(m, s, MLVO_میل_WILDCARD_LEGACY);
        اذهب قاعدہ_mlvo;
    حالة TOK_WILD_CARD_NONE:
        إذا (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo_wildcard(m, s, MLVO_میل_WILDCARD_NONE);
        اذهب قاعدہ_mlvo;
    حالة TOK_WILD_CARD_SOME:
        إذا (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo_wildcard(m, s, MLVO_میل_WILDCARD_SOME);
        اذهب قاعدہ_mlvo;
    حالة TOK_WILD_CARD_ANY:
        إذا (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo_wildcard(m, s, MLVO_میل_WILDCARD_ANY);
        اذهب قاعدہ_mlvo;
    حالة TOK_گروہ_NAME:
        إذا (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_mlvo_گروہ(m, s, m->val.string);
        اذهب قاعدہ_mlvo;
    حالة TOK_EQUALS:
        اذهب قاعدہ_kccgst;
    افتراضي:
        اذهب unexpected;
    }

قاعدہ_kccgst:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        إذا (!m->قاعدہ_2.skip)
            matcher_قاعدہ_سیٹ_kccgst(m, s, m->val.string);
        اذهب قاعدہ_kccgst;
    حالة TOK_سرا_دا_سطر:
        إذا (!m->قاعدہ_2.skip)
            matcher_قاعدہ_verify(m, s);
        إذا (!m->قاعدہ_2.skip)
            matcher_قاعدہ_apply_if_matches(m, s);
        اذهب قاعدہ_mlvo_پہلا;
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

انت:
    أرجع true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_قاعدے_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    أرجع false;
}
// NOLINTEND(readability-trivial-switch)

ساكن bool
read_قاعدے_file(بنية xkb_context *ctx,
                بنية matcher *matcher,
                بنانشانی صحيح include_ڈونگھائی,
                FILE *file,
                ثابت حرف *رستہ)
{
    bool ret;
    حرف *string;
    ناپ_t ناپ;
    بنية scanner scanner;

    إذا (!map_file(file, &string, &ناپ)) {
        log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                رستہ, strerror(errno));
        أرجع false;
    }

    scanner_init(&scanner, matcher->ctx, string, ناپ, رستہ, عدم);

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
        unmap_file(string, ناپ);
        أرجع false;
    }

    ret = matcher_میل(matcher, &scanner, include_ڈونگھائی, string, ناپ, رستہ);
    unmap_file(string, ناپ);
    أرجع ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
ساكن bool
xkb_resolve_partial_قاعدے(بنية xkb_context *ctx, حرف *رستہ, ناپ_t رستہ_ناپ,
                          ثابت حرف* قاعدے, ثابت حرف* suffix,
                          بنية matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    حرف partial_قاعدے[60]; /* Arbitrary, but we do not expect long names */
    إذا (unlikely(!snprintf_محفوظ(partial_قاعدے, حجم(partial_قاعدے),
                                "%s%s", قاعدے, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", قاعدے, suffix);
        أرجع false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    بنانشانی صحيح وتھ = 0;
    FILE *file = عدم;
    ثابت ناپ_t len = لڑی_لمبائی(partial_قاعدے);
    طالما ((file = لبھو_File_وچ_Xkb_رستہ(ctx, "(unknown)",
                                     partial_قاعدے, len, FILE_TYPE_قاعدے,
                                     رستہ, رستہ_ناپ, &وتھ, false)) != عدم) {
        ثابت bool ok = read_قاعدے_file(ctx, matcher, 0, file, رستہ);
        fclose(file);
        إذا (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", رستہ);
            أرجع false;
        }
        وتھ++;
    }

    أرجع true;
}

ساكن bool
xkb_resolve_قاعدے(بنية xkb_context *ctx,
                  ثابت حرف* قاعدے, بنية matcher *matcher,
                  بنية xkb_component_ناں *باہرلا,
                  xkb_layout_اشاریہ_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    بنانشانی صحيح وتھ = 0;
    حرف رستہ[PATH_MAX];
    FILE * ثابت file = لبھو_File_وچ_Xkb_رستہ(ctx, "(unknown)",
                                          قاعدے, لڑی_لمبائی(قاعدے), FILE_TYPE_قاعدے,
                                          رستہ, حجم(رستہ), &وتھ, true);
    إذا (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", قاعدے);
        اذهب err_باہرلا;
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
    ret = xkb_resolve_partial_قاعدے(ctx, رستہ, حجم(رستہ),
                                    قاعدے, ".pre", matcher);
    إذا (!ret)
        اذهب err_باہرلا;

    /* Resolve main <rules> file */
    ret = read_قاعدے_file(ctx, matcher, 0, file, رستہ);
    إذا (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", رستہ);
        اذهب err_باہرلا;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_قاعدے(ctx, رستہ, حجم(رستہ),
                                    قاعدے, ".post", matcher);
    إذا (!ret)
        اذهب err_باہرلا;

    /* Check we got required components */
    إذا (darray_خالی_تھاواں(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_خالی_تھاواں(matcher->kccgst[KCCGST_TYPES]) ||
        darray_خالی_تھاواں(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_خالی_تھاواں(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", قاعدے);
        ret = false;
        اذهب err_باہرلا;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &باہرلا->keycodes, عدم);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &باہرلا->types, عدم);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &باہرلا->compatibility, عدم);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &باہرلا->symbols, عدم);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &باہرلا->geometry, عدم);

    بنية matched_sval *mval = &matcher->rmlvo.model;
    إذا (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (بنانشانی صحيح) mval->sval.len, mval->sval.شروع, قاعدے);
    darray_foreach(mval, matcher->rmlvo.layouts)
        إذا (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (بنانشانی صحيح) mval->sval.len, mval->sval.شروع, قاعدے);
    darray_foreach(mval, matcher->rmlvo.variants)
        إذا (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (بنانشانی صحيح) mval->sval.len, mval->sval.شروع, قاعدے);
    darray_foreach(mval, matcher->rmlvo.options) {
        إذا (!mval->sval.len)
            استمر;

        إذا (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (بنانشانی صحيح) mval->sval.len, mval->sval.شروع, قاعدے);
        } وإلا إذا (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ثابت xkb_layout_نقاب_t unmatched = (mval->layouts & ~mval->matched);
            أكد(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (بنانشانی صحيح)mval->sval.len, mval->sval.شروع,
                    unmatched, قاعدے);
        }
    }

    /* Set the number of explicit layouts */
    إذا (باہرلا->symbols != عدم && explicit_layouts != عدم) {
        *explicit_layouts = 1; /* at least one group */
        ثابت حرف *symbols = باہرلا->symbols;
        /* Take the highest modifier */
        طالما ((symbols = strchr(symbols, ':')) != عدم && symbols[1] != '\0') {
            xkb_layout_اشاریہ_t گروہ = 0;
            ثابت صحيح گنتی = parse_dec_منزل_uint32_t(++symbols, SIZE_MAX, &گروہ);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            إذا (گنتی > 0 && (symbols[گنتی] == '\0' ||
                is_ملاؤ_منوال_اگیتر(symbols[گنتی])) &&
                گروہ > 0 && گروہ <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, گروہ);
                symbols += گنتی;
            }
        }
    }

err_باہرلا:
    إذا (file)
        fclose(file);

    أرجع ret;
}

bool
xkb_components_from_rmlvo_builder(ثابت بنية xkb_rmlvo_builder *rmlvo,
                                  بنية xkb_component_ناں *باہرلا,
                                  xkb_layout_اشاریہ_t *explicit_layouts)
{
    ثابت حرف *قاعدے = rmlvo->قاعدے;
    بنية matcher *matcher = matcher_new_from_rmlvo(rmlvo, &قاعدے);
    إذا (!matcher)
        أرجع false;

    ثابت bool ret = xkb_resolve_قاعدے(rmlvo->ctx, قاعدے, matcher,
                                       باہرلا, explicit_layouts);

    matcher_free(matcher);
    أرجع ret;
}


bool
xkb_components_from_قاعدے_ناں(بنية xkb_context *ctx,
                                ثابت بنية xkb_قاعدہ_ناں *rmlvo,
                                بنية xkb_component_ناں *باہرلا,
                                xkb_layout_اشاریہ_t *explicit_layouts)
{
    بنية matcher *matcher = matcher_new_from_ناں(ctx, rmlvo);
    إذا (!matcher)
        أرجع false;

    ثابت bool ret = xkb_resolve_قاعدے(ctx, rmlvo->قاعدے, matcher,
                                       باہرلا, explicit_layouts);

    matcher_free(matcher);
    أرجع ret;
}
