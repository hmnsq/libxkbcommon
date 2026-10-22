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

#وصف_ڪريو MAX_INCLUDE_اونهائي 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
اتحاد lvalue {
    بنية sval string;
};

تعداد قاعدا_token {
    TOK_پڇاڙي_جو_FILE = 0,
    TOK_پڇاڙي_جو_سٽ,
    TOK_IDENTIFIER,
    TOK_گروهه_NAME,
    TOK_BANG,
    TOK_EQUALS,
    TOK_WILD_CARD_تارو,
    TOK_WILD_CARD_NONE,
    TOK_WILD_CARD_SOME,
    TOK_WILD_CARD_ANY,
    TOK_INCLUDE,
    TOK_ERROR
};

ساكن تعداد قاعدا_token
lex(بنية scanner *s, اتحاد lvalue *val)
{
skip_وڌيڪ_whitespace_and_comments:
    /* Skip spaces */
    طالما (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    إذا (scanner_lit(s, "//")) {
        scanner_skip_منزل_eol(s);
    }

    /* New line */
    إذا (scanner_eol(s)) {
        طالما (scanner_eol(s)) scanner_ايندڙ(s);
        أرجع TOK_پڇاڙي_جو_سٽ;
    }

    /* Escaped line continuation */
    إذا (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        إذا (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            أرجع TOK_ERROR;
        }
        scanner_ايندڙ(s);
        اذهب skip_وڌيڪ_whitespace_and_comments;
    }

    /* See if we're done */
    إذا (scanner_eof(s)) أرجع TOK_پڇاڙي_جو_FILE;

    /* New token */
    s->token_pos = s->pos;
    تعداد قاعدا_token token = TOK_IDENTIFIER;

    ثابت حرف ايندڙ = scanner_ايندڙ(s);
    بدل (ايندڙ) {
    حالة '!':
        أرجع TOK_BANG;
    حالة '$':
        /*گروهه*/
        token = TOK_گروهه_NAME;
        val->string.len = 0;
        اكسر;
    حالة '*':
        إذا (scanner_قاعدا_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            اكسر;
        } وإلا {
            /* Legacy wild card */
            أرجع TOK_WILD_CARD_تارو;
        }
    حالة '<':
        /* Extended wild cards */
        إذا (scanner_قاعدا_lit_token(s, "none>")) أرجع TOK_WILD_CARD_NONE;
        إذا (scanner_قاعدا_lit_token(s, "some>")) أرجع TOK_WILD_CARD_SOME;
        إذا (scanner_قاعدا_lit_token(s, "any>")) أرجع TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        أكد(scanner_قاعدا_is_ident('<'));
        val->string.len = 1;
        اكسر;
    حالة '=':
        إذا (scanner_قاعدا_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            اكسر;
        } وإلا {
            أرجع TOK_EQUALS;
        }
    حالة 'i':
        إذا (scanner_قاعدا_lit_token(s, "nclude")) {
            /* Include statement */
            أرجع TOK_INCLUDE;
        }
        /* Identifier */
        أكد(scanner_قاعدا_is_ident('i'));
        val->string.len = 1;
        اكسر;
    افتراضي:
        إذا (likely(scanner_قاعدا_is_ident(ايندڙ))) {
            /* Identifier */
            val->string.len = 1;
        } وإلا {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.شروعات = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    أكد(scanner_قاعدا_is_ident(ملايو_OVERRIDE_اڳياڙي));
    أكد(scanner_قاعدا_is_ident(ملايو_AUGMENT_اڳياڙي));
    أكد(scanner_قاعدا_is_ident(ملايو_REPLACE_اڳياڙي));
    أكد(scanner_قاعدا_is_ident('('));
    أكد(scanner_قاعدا_is_ident(')'));
    val->string.len += scanner_قاعدا_ident_token(s);

    إذا (unlikely(val->string.len == 0)) {
        إذا (token == TOK_گروهه_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } وإلا {
            scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                        "unrecognized token");
        }
        أرجع TOK_ERROR;
    }

    أرجع token;
}

/***====================================================================***/

تعداد قاعدا_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_انگ_ENTRIES
};

تعداد {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

تعريفنوع uint8_t mlvo_اشاريو_t;
تعريفنوع uint8_t mlvo_نقاب_t;


ساكن ثابت بنية sval قاعدا_mlvo_svals[MLVO_انگ_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

تعداد قاعدا_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_انگ_ENTRIES
};

تعريفنوع uint8_t kccgst_اشاريو_t;
تعريفنوع uint8_t kccgst_نقاب_t;

ساكن ثابت بنية sval قاعدا_kccgst_svals[KCCGST_انگ_ENTRIES] = {
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
بنية قاعدو_نالا {
    بنية matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

بنية گروهه {
    بنية sval name;
    darray_sval elements;
};

بنية mapping {
    تعداد قاعدا_mlvo mlvo_at_pos[MLVO_انگ_ENTRIES];
    mlvo_اشاريو_t انگ_mlvo;
    mlvo_نقاب_t defined_mlvo_نقاب;
    bool has_layout_idx_range;
    bool has_گڻو_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    اتحاد {
        بنية { xkb_layout_اشاريو_t layout_idx, variant_idx; };
        بنية { xkb_layout_اشاريو_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    اتحاد {
        xkb_layout_نقاب_t سرگرم;
        xkb_layout_نقاب_t layouts_candidates_نقاب;
    };
    تعداد قاعدا_kccgst kccgst_at_pos[KCCGST_انگ_ENTRIES];
    kccgst_اشاريو_t انگ_kccgst;
    kccgst_نقاب_t defined_kccgst_نقاب;
};

تعداد mlvo_ميلاپ_type {
    /** Match the given plain value */
    MLVO_ميلاپ_عام = 0,
    /** Match depending on the value of `wildcard_match_type` */
    MLVO_ميلاپ_WILDCARD_LEGACY,
    /** Match empty value */
    MLVO_ميلاپ_WILDCARD_NONE,
    /** Match non-empty value */
    MLVO_ميلاپ_WILDCARD_SOME,
    /** Match any value, optionally empty */
    MLVO_ميلاپ_WILDCARD_ANY,
    /** Match any entry in a group */
    MLVO_ميلاپ_گروهه,
};

تعداد wildcard_ميلاپ_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_ميلاپ_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_ميلاپ_ALL,
};

بنية قاعدو_2 {
    بنية sval mlvo_قدر_at_pos[MLVO_انگ_ENTRIES];
    تعداد mlvo_ميلاپ_type ميلاپ_type_at_pos[MLVO_انگ_ENTRIES];
    بنية sval kccgst_قدر_at_pos[KCCGST_انگ_ENTRIES];
    mlvo_اشاريو_t انگ_mlvo_قدرون;
    kccgst_اشاريو_t انگ_kccgst_قدرون;
    bool skip;
};

#وصف_ڪريو ٽڪرو_KCCGST_ٻٽو_انگ_ميدان_سائيز 4
static_assert(!((1u << (ٽڪرو_KCCGST_ٻٽو_انگ_ميدان_سائيز - 1)) &
                (KCCGST_انگ_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
بنية kccgst_بفر_ٽڪرو {
    uint32_t ڊيگهائي:(32 - ٽڪرو_KCCGST_ٻٽو_انگ_ميدان_سائيز);
    تعداد قاعدا_kccgst kccgst:ٽڪرو_KCCGST_ٻٽو_انگ_ميدان_سائيز;
    xkb_layout_اشاريو_t layout;
};

/* Buffer for pending KcCGST values */
بنية kccgst_بفر {
    darray_char بفر;
    /* Slice corresponding to each value in the buffer */
    darray(بنية kccgst_بفر_ٽڪرو) slices;
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
    بنية قاعدو_نالا rmlvo;
    اتحاد lvalue val;
    darray(بنية گروهه) groups;
    /* Current mapping. */
    بنية mapping mapping;
    /* Current rule. */
    بنية قاعدو_2 قاعدو_2;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    بنية kccgst_بفر pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_انگ_ENTRIES];
};

ساكن بنية sval
strip_spaces(بنية sval v)
{
    طالما (v.len > 0 && is_space(v.شروعات[0])) { v.len--; v.شروعات++; }
    طالما (v.len > 0 && is_space(v.شروعات[v.len - 1])) v.len--;
    أرجع v;
}

ساكن darray_matched_sval
ورهايو_comma_separated_mlvo(بنية xkb_context *ctx,
                           تعداد قاعدا_mlvo mlvo, ثابت حرف *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    إذا (!s) {
        ensure_at_گهٽ_کان_گهٽ_one_قدر:
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
        طالما (*s != '\0' && *s != ',' && *s != OPTIONS_گروهه_SPECIFIER_اڳياڙي) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        إذا (*s == OPTIONS_گروهه_SPECIFIER_اڳياڙي) {
            /* Handle numeric layout index */
            s++;
            ثابت حرف* ثابت layout_شروعات = s;
            xkb_layout_اشاريو_t layout = XKB_LAYOUT_INVALID;

            صحيح ڳڻپ = parse_dec_منزل_uint32_t(s, SIZE_MAX, &layout);
            إذا (ڳڻپ > 0) {
                /* Note: 1-indexed layout */
                s += ڳڻپ;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                إذا (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (بنانشان صحيح) val.sval.len, val.sval.شروعات);
                } وإلا إذا (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_پيغام_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (بنانشان صحيح) val.sval.len, val.sval.شروعات);
                } وإلا {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ثابت حرف* ثابت layout_اشاريو_پڇاڙي = s;
            طالما (*s != '\0' && *s != ',') { s++; }
            إذا (ڳڻپ <= 0 || layout_اشاريو_پڇاڙي != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (بنانشان صحيح) (s - layout_شروعات), layout_شروعات,
                        (بنانشان صحيح) val.sval.len, val.sval.شروعات);
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
                        اذهب ايندڙ_قدر;
                    }
                }
            }
            darray_append(arr, val);
        }

ايندڙ_قدر:
        إذا (*s == '\0') اكسر;
        إذا (*s == ',') s++;
    }

    إذا (darray_خالي_جايون(arr))
        اذهب ensure_at_گهٽ_کان_گهٽ_one_قدر;

    أرجع arr;
}

ساكن بنية matcher *
matcher_new_from_rmlvo(ثابت بنية xkb_rmlvo_builder *rmlvo, ثابت حرف **قاعدا)
{
    بنية matcher *m = صفر_يادگيري_ڏيو(1, حجم(*m));
    إذا (!m)
        أرجع عدم;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    بنية xkb_قاعدو_نالا نالا = {
        .قاعدا = rmlvo->قاعدا,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_خالي_جايون(rmlvo->layouts)) ? عدم : "x",
        .variant = (darray_خالي_جايون(rmlvo->layouts)) ? عدم : "x",
        .options = (darray_خالي_جايون(rmlvo->options)) ? عدم : "x",
    };
    ثابت تعداد RMLVO بدليو = xkb_context_sanitize_قاعدو_نالا(rmlvo->ctx,
                                                               &نالا);

    /* Initialize matcher with sanitized names, if relevant */
    إذا (بدليو & RMLVO_قاعدا) {
        *قاعدا = نالا.قاعدا;
    } وإلا {
        *قاعدا = rmlvo->قاعدا;
    }

    إذا (بدليو & RMLVO_MODEL) {
        m->rmlvo.model.sval.شروعات = نالا.model;
    } وإلا {
        m->rmlvo.model.sval.شروعات = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_محفوظ(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL;

    أكد((بدليو & RMLVO_LAYOUT) || !(بدليو & RMLVO_VARIANT));
    إذا (بدليو & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = ورهايو_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      نالا.layout);
        m->rmlvo.variants = ورهايو_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       نالا.variant);
        إذا (darray_سائيز(m->rmlvo.layouts) > darray_سائيز(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            إذا (!isempty(نالا.variant))
                log_warn(m->ctx, XKB_LOG_پيغام_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         نالا.layout ? نالا.layout : "(none)",
                         نالا.variant ? نالا.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_سائيز(m->rmlvo.layouts));
        } وإلا إذا (darray_سائيز(m->rmlvo.layouts) < darray_سائيز(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_پيغام_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    نالا.layout ? نالا.layout : "(none)",
                    نالا.variant ? نالا.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_سائيز(m->rmlvo.layouts));
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
            val.sval.شروعات = layout->variant;
            val.sval.len = strlen_محفوظ(layout->variant);
            darray_append(m->rmlvo.variants, val);
        }
    }

    إذا (بدليو & RMLVO_OPTIONS) {
        m->rmlvo.options = ورهايو_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      نالا.options);
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
matcher_new_from_نالا(بنية xkb_context *ctx,
            ثابت بنية xkb_قاعدو_نالا *rmlvo)
{
    بنية matcher *m = صفر_يادگيري_ڏيو(1, حجم(*m));
    إذا (!m)
        أرجع عدم;

    m->ctx = ctx;
    m->rmlvo.model.sval.شروعات = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_محفوظ(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_نقاب_t)XKB_OPTION_LAYOUT_نقاب_GLOBAL;
    m->rmlvo.layouts = ورهايو_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = ورهايو_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = ورهايو_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    إذا (darray_سائيز(m->rmlvo.layouts) > darray_سائيز(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        إذا (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_پيغام_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_سائيز(m->rmlvo.layouts));
    } وإلا إذا (darray_سائيز(m->rmlvo.layouts) < darray_سائيز(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_پيغام_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_سائيز(m->rmlvo.layouts));
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
    بنية گروهه *گروهه;
    darray_foreach(گروهه, m->groups)
        darray_free(گروهه->elements);
    darray_free(m->pending_kccgst.بفر);
    darray_free(m->pending_kccgst.slices);
    لكل (kccgst_اشاريو_t i = 0; i < (kccgst_اشاريو_t) KCCGST_انگ_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    حرر(m);
}

ساكن فراغ
matcher_گروهه_شروعات_new(بنية matcher *m, بنية sval name)
{
    بنية گروهه گروهه = { .name = name, .elements = darray_new() };
    darray_append(m->groups, گروهه);
}

ساكن فراغ
matcher_گروهه_جوڙيو_element(بنية matcher *m, بنية scanner *s,
                          بنية sval element)
{
    darray_append(darray_شي(m->groups, darray_سائيز(m->groups) - 1).elements,
                  element);
}

ساكن bool
read_قاعدا_file(بنية xkb_context *ctx,
                بنية matcher *matcher,
                بنانشان صحيح include_اونهائي,
                FILE *file,
                ثابت حرف *رستو);

ساكن فراغ
matcher_include(بنية matcher *m, بنية scanner *والد_scanner,
                بنانشان صحيح include_اونهائي,
                بنية sval inc)
{
    إذا (include_اونهائي >= MAX_INCLUDE_اونهائي) {
        scanner_err(والد_scanner, XKB_LOG_پيغام_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_اونهائي);
        أرجع;
    }

    ثابت حرف *stmt_file = inc.شروعات;
    سائيز_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    حرف buf[PATH_MAX] = {0};
    ثابت ssize_t expanded = expand_رستو(m->ctx, والد_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_قاعدا,
                                         buf, حجم(buf));
    إذا (expanded < 0) {
        /* Error */
        أرجع;
    } وإلا إذا (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (سائيز_t) expanded;
        أكد(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    بنانشان صحيح وٿي = 0;
    ثابت bool absolute_رستو = is_absolute_رستو(stmt_file);
    إذا (absolute_رستو) {
        /* Absolute path: no need for lookup in XKB paths */
        إذا (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            إذا (stmt_file_len < حجم(buf)) {
                يادگيري_نقل(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } وإلا {
                log_err(m->ctx, XKB_ERROR_INVALID_رستو,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, حجم(buf),
                        (بنانشان صحيح) stmt_file_len, stmt_file);
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
            file = ڳوليو_File_اندريون_Xkb_رستو(m->ctx, والد_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_قاعدا,
                                     buf, حجم(buf), &وٿي, true);
        }
    }

    طالما (file) {
        أكد(strlen_محفوظ(buf) < حجم(buf));
        bool ret = read_قاعدا_file(m->ctx, m, include_اونهائي + 1, file, buf);
        fclose(file);
        إذا (ret)
            أرجع;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_پيغام_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        إذا (absolute_رستو) {
            /* There is no point to search further if the path is absolute */
            اكسر;
        }

        /* Try next XKB path */
        وٿي++;
        file = ڳوليو_File_اندريون_Xkb_رستو(m->ctx, والد_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_قاعدا,
                                 buf, حجم(buf), &وٿي, true);
    }

    log_err(m->ctx, XKB_LOG_پيغام_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (بنانشان صحيح) stmt_file_len, stmt_file);
}

ساكن فراغ
matcher_mapping_شروعات_new(بنية matcher *m)
{
    لكل (mlvo_اشاريو_t i = 0; i < (mlvo_اشاريو_t) MLVO_انگ_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_انگ_ENTRIES;
    لكل (kccgst_اشاريو_t i = 0; i < (kccgst_اشاريو_t) KCCGST_انگ_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_انگ_ENTRIES;
    m->mapping.has_گڻو_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.انگ_mlvo = m->mapping.انگ_kccgst = 0;
    m->mapping.defined_mlvo_نقاب = 0;
    m->mapping.defined_kccgst_نقاب = 0;
    m->mapping.سرگرم = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
ساكن صحيح
parse_layout_int_اشاريو_پڇ(ثابت حرف *s, سائيز_t max_len,
                            xkb_layout_اشاريو_t *ٻاهريون)
{
    إذا (max_len < 3)
        أرجع -1;
    uint32_t val = 0;
    ثابت صحيح ڳڻپ = parse_dec_منزل_uint32_t(&s[1], max_len - 2, &val);
    إذا (ڳڻپ <= 0 || s[1 + ڳڻپ] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        أرجع -1;
    /* To zero-based index. */
    *ٻاهريون = val - 1;
    أرجع ڳڻپ + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
ساكن صحيح
ڪڍو_layout_اشاريو(ثابت حرف *s, سائيز_t max_len, xkb_layout_اشاريو_t *ٻاهريون)
{
    /* This function is pretty stupid, but works for now. */
    *ٻاهريون = XKB_LAYOUT_INVALID;
    إذا (max_len < 3 || s[0] != '[')
        أرجع -1;
    إذا (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *ٻاهريون = (xkb_layout_اشاريو_t)(s[1] - '1');
        أرجع 3;
    }
    إذا (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        أرجع 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    أرجع parse_layout_int_اشاريو_پڇ(s, max_len, ٻاهريون);
}

/* Special layout indices */
تعداد layout_اشاريو_ranges {
    LAYOUT_اشاريو_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_اشاريو_پهريون,
    LAYOUT_اشاريو_LATER,
    LAYOUT_اشاريو_گڻو,
    LAYOUT_اشاريو_ANY,
};

static_assert((xkb_layout_اشاريو_t) XKB_MAX_GROUPS <
              (xkb_layout_اشاريو_t) LAYOUT_اشاريو_SINGLE,
              "Cannot define special indices");
static_assert((xkb_layout_اشاريو_t) LAYOUT_اشاريو_SINGLE <
              (xkb_layout_اشاريو_t) LAYOUT_اشاريو_پهريون &&
              (xkb_layout_اشاريو_t) LAYOUT_اشاريو_پهريون <
              (xkb_layout_اشاريو_t) LAYOUT_اشاريو_LATER &&
              (xkb_layout_اشاريو_t) LAYOUT_اشاريو_LATER <
              (xkb_layout_اشاريو_t) LAYOUT_اشاريو_گڻو &&
              (xkb_layout_اشاريو_t) LAYOUT_اشاريو_گڻو <
              (xkb_layout_اشاريو_t) LAYOUT_اشاريو_ANY &&
              (xkb_layout_اشاريو_t) LAYOUT_اشاريو_ANY <
              (xkb_layout_اشاريو_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
ساكن bool
ڪڍو_mapping_layout_اشاريو(ثابت حرف *s, سائيز_t max_len,
                             xkb_layout_اشاريو_t *ٻاهريون)
{
    /* Check for minimal `[` + index + `]` */
    إذا (max_len < 3 || s[0] != '[') {
        *ٻاهريون = XKB_LAYOUT_INVALID;
        أرجع false;
    }

    /* Small numeric index */
    إذا (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *ٻاهريون = (xkb_layout_اشاريو_t)(s[1] - '1');
        أرجع true;
    }

    /* Named indices ranges */
    ساكن ثابت بنية {
        ثابت حرف* name;
        سائيز_t سائيز;
        xkb_layout_اشاريو_t قدر;
    } نالا[] = {
        #وصف_ڪريو اشاريو_RANGE(پڇ, قدر) \
            { (پڇ), حجم(پڇ), (xkb_layout_اشاريو_t)(قدر) }
        اشاريو_RANGE("single]",   LAYOUT_اشاريو_SINGLE),
        اشاريو_RANGE("first]",    LAYOUT_اشاريو_پهريون),
        اشاريو_RANGE("later]",    LAYOUT_اشاريو_LATER),
        اشاريو_RANGE("multiple]", LAYOUT_اشاريو_گڻو),
        اشاريو_RANGE("any]",      LAYOUT_اشاريو_ANY),
        #وصف_رد اشاريو_RANGE
    };

    لكل (سائيز_t k = 0; k < ARRAY_سائيز(نالا); k++) {
        إذا (max_len == نالا[k].سائيز &&
            memcmp(&s[1], نالا[k].name, نالا[k].سائيز - 1) == 0) {
            *ٻاهريون = نالا[k].قدر;
            أرجع true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *ٻاهريون = XKB_LAYOUT_INVALID;
    ثابت صحيح ڳڻپ = parse_layout_int_اشاريو_پڇ(s, max_len, ٻاهريون);
    أرجع ڳڻپ > 0 && (سائيز_t)ڳڻپ == max_len;
}

ساكن مضمن bool
is_mlvo_نقاب_defined(بنية matcher *m, تعداد قاعدا_mlvo mlvo)
{
    أرجع m->mapping.defined_mlvo_نقاب & (1u << mlvo);
}

ساكن فراغ
matcher_mapping_سيٽ_mlvo(بنية matcher *m, بنية scanner *s,
                         بنية sval ident)
{
    تعداد قاعدا_mlvo mlvo;
    بنية sval mlvo_sval;

    لكل (mlvo = 0; mlvo < MLVO_انگ_ENTRIES; mlvo++) {
        mlvo_sval = قاعدا_mlvo_svals[mlvo];

        إذا (svaleq_اڳياڙي(mlvo_sval, ident))
            اكسر;
    }

    /* Not found. */
    إذا (mlvo >= MLVO_انگ_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (بنانشان صحيح) ident.len, ident.شروعات);
        m->mapping.سرگرم = false;
        أرجع;
    }

    إذا (is_mlvo_نقاب_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (بنانشان صحيح) mlvo_sval.len, mlvo_sval.شروعات);
        m->mapping.سرگرم = false;
        أرجع;
    }

    /* If there are leftovers still, it must be an index. */
    إذا (mlvo_sval.len < ident.len) {
        xkb_layout_اشاريو_t idx;
        إذا (!ڪڍو_mapping_layout_اشاريو(ident.شروعات + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (بنانشان صحيح) mlvo_sval.len, mlvo_sval.شروعات);
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
            scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (بنانشان صحيح) mlvo_sval.len, mlvo_sval.شروعات);
            m->mapping.سرگرم = false;
            أرجع;
        }
    } وإلا إذا (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_اشاريو_t) LAYOUT_اشاريو_SINGLE;
    } وإلا إذا (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_اشاريو_t) LAYOUT_اشاريو_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    إذا (((mlvo == MLVO_LAYOUT && is_mlvo_نقاب_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_نقاب_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.سرگرم = false;
        أرجع;
    }

    m->mapping.mlvo_at_pos[m->mapping.انگ_mlvo] = mlvo;
    m->mapping.defined_mlvo_نقاب |= (mlvo_نقاب_t) 1u << mlvo;
    m->mapping.انگ_mlvo++;
}

ساكن فراغ
matcher_mapping_سيٽ_layout_bounds(بنية matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_اشاريو_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
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
        حالة LAYOUT_اشاريو_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_سائيز(m->rmlvo.layouts));
            m->mapping.layouts_candidates_نقاب = (xkb_layout_نقاب_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            اكسر;
        حالة LAYOUT_اشاريو_گڻو:
        حالة LAYOUT_اشاريو_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_سائيز(m->rmlvo.layouts));
            m->mapping.layouts_candidates_نقاب = (xkb_layout_نقاب_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            اكسر;
        حالة LAYOUT_اشاريو_SINGLE:
        حالة LAYOUT_اشاريو_پهريون:
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
    m->mapping.has_گڻو_layouts = (
        m->mapping.layout_idx_max > m->mapping.layout_idx_min &&
        m->mapping.layout_idx_max - m->mapping.layout_idx_min > 1
    );
}

ساكن فراغ
matcher_mapping_سيٽ_kccgst(بنية matcher *m, بنية scanner *s, بنية sval ident)
{
    تعداد قاعدا_kccgst kccgst;
    بنية sval kccgst_sval;

    لكل (kccgst = 0; kccgst < KCCGST_انگ_ENTRIES; kccgst++) {
        kccgst_sval = قاعدا_kccgst_svals[kccgst];

        إذا (svaleq(قاعدا_kccgst_svals[kccgst], ident))
            اكسر;
    }

    /* Not found. */
    إذا (kccgst >= KCCGST_انگ_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (بنانشان صحيح) ident.len, ident.شروعات);
        m->mapping.سرگرم = false;
        أرجع;
    }

    إذا (m->mapping.defined_kccgst_نقاب & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (بنانشان صحيح) kccgst_sval.len, kccgst_sval.شروعات);
        m->mapping.سرگرم = false;
        أرجع;
    }

    m->mapping.kccgst_at_pos[m->mapping.انگ_kccgst] = kccgst;
    m->mapping.defined_kccgst_نقاب |= (kccgst_نقاب_t) 1u << kccgst;
    m->mapping.انگ_kccgst++;
}

ساكن bool
matcher_mapping_verify(بنية matcher *m, بنية scanner *s)
{
    إذا (m->mapping.انگ_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        اذهب skip;
    }

    إذا (m->mapping.انگ_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
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
            حالة LAYOUT_اشاريو_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                إذا (darray_سائيز(m->rmlvo.layouts) > 1)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_اشاريو_گڻو:
            حالة LAYOUT_اشاريو_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                إذا (darray_سائيز(m->rmlvo.layouts) < 2)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_اشاريو_ANY:
            حالة LAYOUT_اشاريو_پهريون:
                /* No restrictions */
                اكسر;
            افتراضي:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                إذا (darray_سائيز(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_سائيز(m->rmlvo.layouts))
                    اذهب skip;
        }
    }

    إذا (is_mlvo_نقاب_defined(m, MLVO_VARIANT)) {
        أكد(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        بدل (m->mapping.variant_idx) {
            حالة LAYOUT_اشاريو_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                إذا (darray_سائيز(m->rmlvo.variants) > 1)
                    اذهب skip;
                اكسر;
            حالة LAYOUT_اشاريو_ANY:
            حالة LAYOUT_اشاريو_LATER:
            حالة LAYOUT_اشاريو_پهريون:
                /* No restriction */
                اكسر;
            افتراضي:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                إذا (darray_سائيز(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_سائيز(m->rmlvo.variants))
                    اذهب skip;
        }
    }

    أرجع true;

skip:
    m->mapping.سرگرم = false;
    أرجع false;
}

ساكن فراغ
matcher_قاعدو_شروعات_new(بنية matcher *m)
{
    memset(&m->قاعدو_2, 0, حجم(m->قاعدو_2));
    m->قاعدو_2.skip = !m->mapping.سرگرم;
}

ساكن فراغ
matcher_قاعدو_سيٽ_mlvo_common(بنية matcher *m, بنية scanner *s,
                             بنية sval ident,
                             تعداد mlvo_ميلاپ_type ميلاپ_type)
{
    إذا (m->قاعدو_2.انگ_mlvo_قدرون >= m->mapping.انگ_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->قاعدو_2.skip = true;
        أرجع;
    }
    m->قاعدو_2.ميلاپ_type_at_pos[m->قاعدو_2.انگ_mlvo_قدرون] = ميلاپ_type;
    m->قاعدو_2.mlvo_قدر_at_pos[m->قاعدو_2.انگ_mlvo_قدرون] = ident;
    m->قاعدو_2.انگ_mlvo_قدرون++;
}

ساكن فراغ
matcher_قاعدو_سيٽ_mlvo_wildcard(بنية matcher *m, بنية scanner *s,
                               تعداد mlvo_ميلاپ_type ميلاپ_type)
{
    بنية sval dummy = SVAL(عدم, 0);
    matcher_قاعدو_سيٽ_mlvo_common(m, s, dummy, ميلاپ_type);
}

ساكن فراغ
matcher_قاعدو_سيٽ_mlvo_گروهه(بنية matcher *m, بنية scanner *s,
                            بنية sval ident)
{
    matcher_قاعدو_سيٽ_mlvo_common(m, s, ident, MLVO_ميلاپ_گروهه);
}

ساكن فراغ
matcher_قاعدو_سيٽ_mlvo(بنية matcher *m, بنية scanner *s,
                      بنية sval ident)
{
    matcher_قاعدو_سيٽ_mlvo_common(m, s, ident, MLVO_ميلاپ_عام);
}

ساكن فراغ
matcher_قاعدو_سيٽ_kccgst(بنية matcher *m, بنية scanner *s,
                        بنية sval ident)
{
    إذا (m->قاعدو_2.انگ_kccgst_قدرون >= m->mapping.انگ_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->قاعدو_2.skip = true;
        أرجع;
    }
    m->قاعدو_2.kccgst_قدر_at_pos[m->قاعدو_2.انگ_kccgst_قدرون] = ident;
    m->قاعدو_2.انگ_kccgst_قدرون++;
}

ساكن bool
ميلاپ_گروهه(بنية matcher *m, بنية sval گروهه_name, بنية sval منزل)
{
    بنية گروهه *گروهه = عدم;
    بنية sval *element;
    bool مليو = false;

    darray_foreach(گروهه, m->groups) {
        إذا (svaleq(گروهه->name, گروهه_name)) {
            مليو = true;
            اكسر;
        }
    }

    إذا (!مليو) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        أرجع false;
    }

    darray_foreach(element, گروهه->elements)
        إذا (svaleq(منزل, *element))
            أرجع true;

    أرجع false;
}

ساكن bool
ميلاپ_قدر(بنية matcher *m, ثابت بنية sval val, ثابت بنية sval منزل,
            تعداد mlvo_ميلاپ_type ميلاپ_type,
            تعداد wildcard_ميلاپ_type wildcard_type)
{
    بدل (ميلاپ_type) {
        حالة MLVO_ميلاپ_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            أرجع wildcard_type == WILDCARD_ميلاپ_ALL || !!منزل.len;
        حالة MLVO_ميلاپ_WILDCARD_NONE:
            أرجع !منزل.len;
        حالة MLVO_ميلاپ_WILDCARD_SOME:
            أرجع !!منزل.len;
        حالة MLVO_ميلاپ_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            أرجع true;
        حالة MLVO_ميلاپ_گروهه:
            أرجع ميلاپ_گروهه(m, val, منزل);
        افتراضي:
            أكد(ميلاپ_type == MLVO_ميلاپ_عام);
            أرجع svaleq(val, منزل);
    }
}

ساكن bool
ميلاپ_قدر_and_mark(بنية matcher *m, ثابت بنية sval val,
                     بنية matched_sval *منزل, تعداد mlvo_ميلاپ_type ميلاپ_type,
                     تعداد wildcard_ميلاپ_type wildcard_type,
                     xkb_layout_نقاب_t layouts)
{
    bool matched = ميلاپ_قدر(m, val, منزل->sval, ميلاپ_type, wildcard_type);
    إذا (matched)
        منزل->matched |= layouts;
    أرجع matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
ساكن bool
expand_rmlvo_اندريون_kccgst_قدر(بنية matcher *m, بنية scanner *s,
                             بنية sval قدر, xkb_layout_اشاريو_t layout_idx,
                             darray_char *expanded, سائيز_t *i)
{
    ثابت حرف *str = قدر.شروعات;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    تعداد قاعدا_mlvo mlv;
    xkb_layout_اشاريو_t idx;
    حرف pfx, sfx;
    بنية matched_sval *expanded_قدر;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    إذا (str[*i] == 'i' &&
        (*i + 1 == قدر.len || is_ملايو_گهڻائي_اڳياڙي(str[*i + 1])))
    {
        إذا (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_قاعدا_INVALID_LAYOUT_اشاريو_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            اذهب error;
        }
        (*i)++;
        حرف اشاريو_str[MAX_LAYOUT_اشاريو_STR_ڊيگهائي + 1];
        ثابت صحيح ڳڻپ = snprintf(اشاريو_str, حجم(اشاريو_str), "%"PRIu32,
                                   layout_idx + 1);
        أكد(ڳڻپ > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, اشاريو_str, (darray_سائيز_t)ڳڻپ);
        أرجع true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    إذا (str[*i] == '(' ||
        is_ملايو_گهڻائي_اڳياڙي(str[*i]) ||
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
    bool variable_اشاريو = false;
    إذا (*i < قدر.len && str[*i] == '[') {
        إذا (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            اذهب error;
        }

        صحيح consumed = ڪڍو_layout_اشاريو(str + (*i), قدر.len - (*i), &idx);
        إذا (consumed <= 0) اذهب error;
        إذا (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            أكد(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_اشاريو = true;
        }
        *i += (سائيز_t)consumed;
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
            إذا (darray_سائيز(m->rmlvo.layouts) == 1)
                expanded_قدر = &darray_شي(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } وإلا إذا (idx < darray_سائيز(m->rmlvo.layouts) &&
                   (variable_اشاريو || darray_سائيز(m->rmlvo.layouts) > 1)) {
                expanded_قدر = &darray_شي(m->rmlvo.layouts, idx);
        }
    }
    وإلا إذا (mlv == MLVO_VARIANT) {
        إذا (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            إذا (darray_سائيز(m->rmlvo.variants) == 1)
                expanded_قدر = &darray_شي(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } وإلا إذا (idx < darray_سائيز(m->rmlvo.variants) &&
                   (variable_اشاريو || darray_سائيز(m->rmlvo.variants) > 1)) {
                expanded_قدر = &darray_شي(m->rmlvo.variants, idx);
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
                                 expanded_قدر->sval.شروعات,
                                 (darray_سائيز_t) expanded_قدر->sval.len);
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
    scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                "invalid %%-expansion in value; not used");
    أرجع false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
ساكن فراغ
expand_qualifier_اندريون_kccgst_قدر(
    بنية matcher *m, بنية scanner *s,
    بنية sval قدر, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_سائيز_t اڳياڙي_idx, سائيز_t *i)
{
    ثابت حرف *str = قدر.شروعات;

    /* “all” followed by nothing or by a layout separator */
    إذا (*i + 2 < قدر.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == قدر.len || is_ملايو_گهڻائي_اڳياڙي(str[*i + 3]))) {
        إذا (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_پيغام_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        إذا (darray_سائيز(m->rmlvo.layouts) > 1) {
            حرف layout_اشاريو[MAX_LAYOUT_اشاريو_STR_ڊيگهائي + 1];
            ثابت darray_سائيز_t اڳياڙي_ڊيگهائي =
                darray_سائيز(*expanded) - اڳياڙي_idx - 1;
            لكل (xkb_layout_اشاريو_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_سائيز(m->rmlvo.layouts));
                 l++)
            {
                إذا (!has_separator)
                    darray_append(*expanded, ملايو_DEFAULT_اڳياڙي);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_شي(*expanded, اڳياڙي_idx),
                                             اڳياڙي_ڊيگهائي);
                /* Append index */
                ثابت صحيح ڳڻپ = snprintf(layout_اشاريو, حجم(layout_اشاريو),
                                           "%"PRIu32, l + 1);
                أكد(ڳڻپ > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_اشاريو,
                                             (darray_سائيز_t)ڳڻپ);
            }
        }
        *i += 3;
    }
}

ساكن مضمن فراغ
#جيڪڏهن_وصف_ٿيل _MSC_VER
concat_kccgst(darray_char *into, darray_سائيز_t سائيز, اندريون_reads(سائيز) ثابت حرف* from)
#وإلا
concat_kccgst(darray_char *into, darray_سائيز_t سائيز, ثابت حرف from[ساكن سائيز])
#جيڪڏهن_پورو
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ثابت bool from_plus = is_ملايو_گهڻائي_اڳياڙي(from[0]);
    إذا (from_plus || darray_خالي_جايون(*into)) {
        darray_appends_nullterminate(*into, from, سائيز);
    } وإلا {
        ثابت حرف ch =
            (حرف) (darray_خالي_جايون(*into) ? '\0' : darray_شي(*into, 0));
        ثابت bool into_plus = is_ملايو_گهڻائي_اڳياڙي(ch);
        إذا (into_plus)
            darray_prepends_nullterminate(*into, from, سائيز);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
ساكن bool
append_expanded_kccgst_قدر(بنية matcher *m, بنية scanner *s,
                             bool ملايو, darray_char *منزل, بنية sval قدر,
                             xkb_layout_اشاريو_t layout_idx)
{
    ثابت حرف *str = قدر.شروعات;
    darray_char expanded = darray_new();
    darray_سائيز_t آخري_شي_idx = 0;
    bool has_separator = false;

    لكل (سائيز_t i = 0; i < قدر.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        بدل (str[i]) {
            /* Qualifier */
            حالة ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_اندريون_kccgst_قدر(m, s, قدر, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 آخري_شي_idx, &i);
                اكسر;
            /* Expansion */
            حالة '%':
                i++;
                إذا (i >= قدر.len ||
                    !expand_rmlvo_اندريون_kccgst_قدر(m, s, قدر, layout_idx,
                                                  &expanded, &i))
                        اذهب error;
                اكسر;
            /* New item */
            حالة ملايو_OVERRIDE_اڳياڙي:
            حالة ملايو_AUGMENT_اڳياڙي:
            حالة ملايو_REPLACE_اڳياڙي:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                آخري_شي_idx = darray_سائيز(expanded) - 1;
                has_separator = true;
                اكسر;
            /* Just a normal character. */
            افتراضي:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    إذا (ملايو) {
        إذا (!darray_خالي_جايون(expanded))
            concat_kccgst(منزل, darray_سائيز(expanded), darray_شيون(expanded));
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
    إذا (!m->mapping.has_گڻو_layouts)
        أرجع true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    لكل (kccgst_اشاريو_t i = 0; i < m->mapping.انگ_kccgst; i++) {
        ثابت تعداد قاعدا_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        لكل (xkb_layout_اشاريو_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            سجل ثابت بنية kccgst_بفر* ثابت buf = &m->pending_kccgst;
            سائيز_t وٿي = 0;
            لكل (darray_سائيز_t k = 0; k < darray_سائيز(buf->slices); k++) {
                سجل ثابت بنية kccgst_بفر_ٽڪرو * ثابت ٽڪرو =
                    &darray_شي(buf->slices, k);
                إذا (ٽڪرو->kccgst == kccgst && ٽڪرو->layout == layout &&
                    ٽڪرو->ڊيگهائي)
                    concat_kccgst(&m->kccgst[kccgst], ٽڪرو->ڊيگهائي,
                                  darray_شيون(buf->بفر) + وٿي);
                وٿي += ٽڪرو->ڊيگهائي;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_گڻو_layouts = false;
    أرجع true;
}

ساكن فراغ
matcher_قاعدو_verify(بنية matcher *m, بنية scanner *s)
{
    إذا (m->قاعدو_2.انگ_mlvo_قدرون != m->mapping.انگ_mlvo ||
        m->قاعدو_2.انگ_kccgst_قدرون != m->mapping.انگ_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                    "invalid rule: must have same number of values "
                    "as mapping line; ignoring rule");
        m->قاعدو_2.skip = true;
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
matcher_قاعدو_ميلاپ_model(بنية matcher *m, ثابت بنية sval قدر,
                         تعداد mlvo_ميلاپ_type ميلاپ_type)
{
    أرجع ميلاپ_قدر_and_mark(
        m, قدر, &m->rmlvo.model, ميلاپ_type, WILDCARD_ميلاپ_ALL,
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
matcher_قاعدو_ميلاپ_option(بنية matcher *m, ثابت بنية sval قدر,
                          تعداد mlvo_ميلاپ_type ميلاپ_type,
                          xkb_layout_نقاب_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_خالي_جايون(m->rmlvo.options));

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
        إذا (matchable && ميلاپ_قدر_and_mark(m, قدر, option, ميلاپ_type,
                                              WILDCARD_ميلاپ_ALL, matchable)) {
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
matcher_قاعدو_ميلاپ_layout_or_variant(بنية matcher *m,
                                     darray_matched_sval *داخلا_2,
                                     ثابت بنية sval قدر,
                                     تعداد mlvo_ميلاپ_type ميلاپ_type,
                                     xkb_layout_نقاب_t *candidates)
{
    /* There is always at least one value */
    أكد(!darray_خالي_جايون(*داخلا_2));

    bool matched = false;
    /* Loop over the layout index range */
    لكل (xkb_layout_اشاريو_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ثابت xkb_layout_نقاب_t layout = (UINT32_C(1) << idx);
        إذا (layout & *candidates) {
            بنية matched_sval *منزل = &darray_شي(*داخلا_2, idx);
            إذا (ميلاپ_قدر_and_mark(m, قدر, منزل, ميلاپ_type,
                                     WILDCARD_ميلاپ_NONEMPTY, layout)) {
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
matcher_قاعدو_apply_if_matches(بنية matcher *m, بنية scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_نقاب_t candidate_layouts = m->mapping.layouts_candidates_نقاب;

    /* Loop over MLVO pattern components */
    لكل (mlvo_اشاريو_t i = 0; i < m->mapping.انگ_mlvo; i++) {
        ثابت تعداد قاعدا_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ثابت بنية sval قدر = m->قاعدو_2.mlvo_قدر_at_pos[i];
        ثابت تعداد mlvo_ميلاپ_type ميلاپ_type = m->قاعدو_2.ميلاپ_type_at_pos[i];
        bool matched = false;

        بدل (mlvo) {
        حالة MLVO_MODEL:
            matched = matcher_قاعدو_ميلاپ_model(m, قدر, ميلاپ_type);
            اكسر;
        حالة MLVO_LAYOUT:
            matched = matcher_قاعدو_ميلاپ_layout_or_variant(
                m, &m->rmlvo.layouts, قدر, ميلاپ_type, &candidate_layouts
            );
            اكسر;
        حالة MLVO_VARIANT:
            matched = matcher_قاعدو_ميلاپ_layout_or_variant(
                m, &m->rmlvo.variants, قدر, ميلاپ_type, &candidate_layouts
            );
            اكسر;
        حالة MLVO_OPTION:
            matched = matcher_قاعدو_ميلاپ_option(
                m, قدر, ميلاپ_type, &candidate_layouts
            );
            اكسر;
        افتراضي: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_انگ_ENTRIES - 1,
                          "Unexpected MLVO field");
            أكد(!"Unreachable");
        }
        }

        إذا (!matched)
            أرجع;
    }

    إذا (m->mapping.has_گڻو_layouts) {
        /* Special index: loop over the index range */
        لكل (xkb_layout_اشاريو_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            إذا (candidate_layouts & (UINT32_C(1) << idx)) {
                لكل (kccgst_اشاريو_t i = 0; i < m->mapping.انگ_kccgst; i++) {
                    ثابت تعداد قاعدا_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ثابت بنية sval قدر = m->قاعدو_2.kccgst_قدر_at_pos[i];
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
                    ثابت darray_سائيز_t prev_بفر_ڊيگهائي =
                        darray_سائيز(buf->بفر);
                    append_expanded_kccgst_قدر(m, s, false, &buf->بفر,
                                                 قدر, idx);
                    ثابت uint32_t ڊيگهائي = (uint32_t) (darray_سائيز(buf->بفر)
                                          - prev_بفر_ڊيگهائي);
                    ثابت بنية kccgst_بفر_ٽڪرو ٽڪرو = {
                        .ڊيگهائي = ڊيگهائي,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, ٽڪرو);
                }
            }
        }
    } وإلا {
        /* Numeric index or no index */
        لكل (kccgst_اشاريو_t i = 0; i < m->mapping.انگ_kccgst; i++) {
            تعداد قاعدا_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            بنية sval قدر = m->قاعدو_2.kccgst_قدر_at_pos[i];
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

ساكن تعداد قاعدا_token
gettok(بنية matcher *m, بنية scanner *s)
{
    أرجع lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
ساكن bool
matcher_ميلاپ(بنية matcher *m, بنية scanner *s,
              بنانشان صحيح include_اونهائي,
              ثابت حرف *string, سائيز_t len,
              ثابت حرف *file_name)
{
    تعداد قاعدا_token tok;

    إذا (!m)
        أرجع false;

initial:
    بدل (tok = gettok(m, s)) {
    حالة TOK_BANG:
        اذهب bang;
    حالة TOK_پڇاڙي_جو_سٽ:
        اذهب initial;
    حالة TOK_پڇاڙي_جو_FILE:
        اذهب پڄاڻي;
    افتراضي:
        اذهب unexpected;
    }

bang:
    بدل (tok = gettok(m, s)) {
    حالة TOK_گروهه_NAME:
        matcher_گروهه_شروعات_new(m, m->val.string);
        اذهب گروهه_name;
    حالة TOK_INCLUDE:
        اذهب include_statement;
    حالة TOK_IDENTIFIER:
        matcher_mapping_شروعات_new(m);
        matcher_mapping_سيٽ_mlvo(m, s, m->val.string);
        اذهب mapping_mlvo;
    افتراضي:
        اذهب unexpected;
    }

گروهه_name:
    بدل (tok = gettok(m, s)) {
    حالة TOK_EQUALS:
        اذهب گروهه_element;
    افتراضي:
        اذهب unexpected;
    }

گروهه_element:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        matcher_گروهه_جوڙيو_element(m, s, m->val.string);
        اذهب گروهه_element;
    حالة TOK_پڇاڙي_جو_سٽ:
        اذهب initial;
    افتراضي:
        اذهب unexpected;
    }

include_statement:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        matcher_include(m, s, include_اونهائي, m->val.string);
        اذهب include_statement_پڇاڙي;
    افتراضي:
        اذهب unexpected;
    }

include_statement_پڇاڙي:
    بدل (tok = gettok(m, s)) {
    حالة TOK_پڇاڙي_جو_سٽ:
        اذهب initial;
    افتراضي:
        اذهب unexpected;
    }

mapping_mlvo:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        إذا (m->mapping.سرگرم)
            matcher_mapping_سيٽ_mlvo(m, s, m->val.string);
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
            matcher_mapping_سيٽ_kccgst(m, s, m->val.string);
        اذهب mapping_kccgst;
    حالة TOK_پڇاڙي_جو_سٽ:
        إذا (m->mapping.سرگرم && matcher_mapping_verify(m, s)) {
            matcher_mapping_سيٽ_layout_bounds(m);
            إذا (m->mapping.has_گڻو_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_سائيز(m->pending_kccgst.بفر) = 0;
                darray_سائيز(m->pending_kccgst.slices) = 0;
            }
        }
        اذهب قاعدو_mlvo_پهريون;
    افتراضي:
        اذهب unexpected;
    }

قاعدو_mlvo_پهريون:
    بدل (tok = gettok(m, s)) {
    حالة TOK_BANG:
        matcher_append_pending_kccgst(m);
        اذهب bang;
    حالة TOK_پڇاڙي_جو_سٽ:
        اذهب قاعدو_mlvo_پهريون;
    حالة TOK_پڇاڙي_جو_FILE:
        matcher_append_pending_kccgst(m);
        اذهب پڄاڻي;
    افتراضي:
        matcher_قاعدو_شروعات_new(m);
        اذهب قاعدو_mlvo_no_tok;
    }

قاعدو_mlvo:
    tok = gettok(m, s);
قاعدو_mlvo_no_tok:
    بدل (tok) {
    حالة TOK_IDENTIFIER:
        إذا (!m->قاعدو_2.skip)
            matcher_قاعدو_سيٽ_mlvo(m, s, m->val.string);
        اذهب قاعدو_mlvo;
    حالة TOK_WILD_CARD_تارو:
        إذا (!m->قاعدو_2.skip)
            matcher_قاعدو_سيٽ_mlvo_wildcard(m, s, MLVO_ميلاپ_WILDCARD_LEGACY);
        اذهب قاعدو_mlvo;
    حالة TOK_WILD_CARD_NONE:
        إذا (!m->قاعدو_2.skip)
            matcher_قاعدو_سيٽ_mlvo_wildcard(m, s, MLVO_ميلاپ_WILDCARD_NONE);
        اذهب قاعدو_mlvo;
    حالة TOK_WILD_CARD_SOME:
        إذا (!m->قاعدو_2.skip)
            matcher_قاعدو_سيٽ_mlvo_wildcard(m, s, MLVO_ميلاپ_WILDCARD_SOME);
        اذهب قاعدو_mlvo;
    حالة TOK_WILD_CARD_ANY:
        إذا (!m->قاعدو_2.skip)
            matcher_قاعدو_سيٽ_mlvo_wildcard(m, s, MLVO_ميلاپ_WILDCARD_ANY);
        اذهب قاعدو_mlvo;
    حالة TOK_گروهه_NAME:
        إذا (!m->قاعدو_2.skip)
            matcher_قاعدو_سيٽ_mlvo_گروهه(m, s, m->val.string);
        اذهب قاعدو_mlvo;
    حالة TOK_EQUALS:
        اذهب قاعدو_kccgst;
    افتراضي:
        اذهب unexpected;
    }

قاعدو_kccgst:
    بدل (tok = gettok(m, s)) {
    حالة TOK_IDENTIFIER:
        إذا (!m->قاعدو_2.skip)
            matcher_قاعدو_سيٽ_kccgst(m, s, m->val.string);
        اذهب قاعدو_kccgst;
    حالة TOK_پڇاڙي_جو_سٽ:
        إذا (!m->قاعدو_2.skip)
            matcher_قاعدو_verify(m, s);
        إذا (!m->قاعدو_2.skip)
            matcher_قاعدو_apply_if_matches(m, s);
        اذهب قاعدو_mlvo_پهريون;
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

پڄاڻي:
    أرجع true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_قاعدا_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    أرجع false;
}
// NOLINTEND(readability-trivial-switch)

ساكن bool
read_قاعدا_file(بنية xkb_context *ctx,
                بنية matcher *matcher,
                بنانشان صحيح include_اونهائي,
                FILE *file,
                ثابت حرف *رستو)
{
    bool ret;
    حرف *string;
    سائيز_t سائيز;
    بنية scanner scanner;

    إذا (!map_file(file, &string, &سائيز)) {
        log_err(ctx, XKB_LOG_پيغام_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                رستو, strerror(errno));
        أرجع false;
    }

    scanner_init(&scanner, matcher->ctx, string, سائيز, رستو, عدم);

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
        unmap_file(string, سائيز);
        أرجع false;
    }

    ret = matcher_ميلاپ(matcher, &scanner, include_اونهائي, string, سائيز, رستو);
    unmap_file(string, سائيز);
    أرجع ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
ساكن bool
xkb_resolve_partial_قاعدا(بنية xkb_context *ctx, حرف *رستو, سائيز_t رستو_سائيز,
                          ثابت حرف* قاعدا, ثابت حرف* suffix,
                          بنية matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    حرف partial_قاعدا[60]; /* Arbitrary, but we do not expect long names */
    إذا (unlikely(!snprintf_محفوظ(partial_قاعدا, حجم(partial_قاعدا),
                                "%s%s", قاعدا, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", قاعدا, suffix);
        أرجع false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    بنانشان صحيح وٿي = 0;
    FILE *file = عدم;
    ثابت سائيز_t len = لڙي_ڊيگهه(partial_قاعدا);
    طالما ((file = ڳوليو_File_اندريون_Xkb_رستو(ctx, "(unknown)",
                                     partial_قاعدا, len, FILE_TYPE_قاعدا,
                                     رستو, رستو_سائيز, &وٿي, false)) != عدم) {
        ثابت bool ok = read_قاعدا_file(ctx, matcher, 0, file, رستو);
        fclose(file);
        إذا (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", رستو);
            أرجع false;
        }
        وٿي++;
    }

    أرجع true;
}

ساكن bool
xkb_resolve_قاعدا(بنية xkb_context *ctx,
                  ثابت حرف* قاعدا, بنية matcher *matcher,
                  بنية xkb_component_نالا *ٻاهريون,
                  xkb_layout_اشاريو_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    بنانشان صحيح وٿي = 0;
    حرف رستو[PATH_MAX];
    FILE * ثابت file = ڳوليو_File_اندريون_Xkb_رستو(ctx, "(unknown)",
                                          قاعدا, لڙي_ڊيگهه(قاعدا), FILE_TYPE_قاعدا,
                                          رستو, حجم(رستو), &وٿي, true);
    إذا (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", قاعدا);
        اذهب err_ٻاهريون;
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
    ret = xkb_resolve_partial_قاعدا(ctx, رستو, حجم(رستو),
                                    قاعدا, ".pre", matcher);
    إذا (!ret)
        اذهب err_ٻاهريون;

    /* Resolve main <rules> file */
    ret = read_قاعدا_file(ctx, matcher, 0, file, رستو);
    إذا (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", رستو);
        اذهب err_ٻاهريون;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_قاعدا(ctx, رستو, حجم(رستو),
                                    قاعدا, ".post", matcher);
    إذا (!ret)
        اذهب err_ٻاهريون;

    /* Check we got required components */
    إذا (darray_خالي_جايون(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_خالي_جايون(matcher->kccgst[KCCGST_TYPES]) ||
        darray_خالي_جايون(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_خالي_جايون(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", قاعدا);
        ret = false;
        اذهب err_ٻاهريون;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &ٻاهريون->keycodes, عدم);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &ٻاهريون->types, عدم);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &ٻاهريون->compatibility, عدم);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &ٻاهريون->symbols, عدم);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &ٻاهريون->geometry, عدم);

    بنية matched_sval *mval = &matcher->rmlvo.model;
    إذا (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (بنانشان صحيح) mval->sval.len, mval->sval.شروعات, قاعدا);
    darray_foreach(mval, matcher->rmlvo.layouts)
        إذا (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (بنانشان صحيح) mval->sval.len, mval->sval.شروعات, قاعدا);
    darray_foreach(mval, matcher->rmlvo.variants)
        إذا (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (بنانشان صحيح) mval->sval.len, mval->sval.شروعات, قاعدا);
    darray_foreach(mval, matcher->rmlvo.options) {
        إذا (!mval->sval.len)
            استمر;

        إذا (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (بنانشان صحيح) mval->sval.len, mval->sval.شروعات, قاعدا);
        } وإلا إذا (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ثابت xkb_layout_نقاب_t unmatched = (mval->layouts & ~mval->matched);
            أكد(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_قدر,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (بنانشان صحيح)mval->sval.len, mval->sval.شروعات,
                    unmatched, قاعدا);
        }
    }

    /* Set the number of explicit layouts */
    إذا (ٻاهريون->symbols != عدم && explicit_layouts != عدم) {
        *explicit_layouts = 1; /* at least one group */
        ثابت حرف *symbols = ٻاهريون->symbols;
        /* Take the highest modifier */
        طالما ((symbols = strchr(symbols, ':')) != عدم && symbols[1] != '\0') {
            xkb_layout_اشاريو_t گروهه = 0;
            ثابت صحيح ڳڻپ = parse_dec_منزل_uint32_t(++symbols, SIZE_MAX, &گروهه);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            إذا (ڳڻپ > 0 && (symbols[ڳڻپ] == '\0' ||
                is_ملايو_گهڻائي_اڳياڙي(symbols[ڳڻپ])) &&
                گروهه > 0 && گروهه <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, گروهه);
                symbols += ڳڻپ;
            }
        }
    }

err_ٻاهريون:
    إذا (file)
        fclose(file);

    أرجع ret;
}

bool
xkb_components_from_rmlvo_builder(ثابت بنية xkb_rmlvo_builder *rmlvo,
                                  بنية xkb_component_نالا *ٻاهريون,
                                  xkb_layout_اشاريو_t *explicit_layouts)
{
    ثابت حرف *قاعدا = rmlvo->قاعدا;
    بنية matcher *matcher = matcher_new_from_rmlvo(rmlvo, &قاعدا);
    إذا (!matcher)
        أرجع false;

    ثابت bool ret = xkb_resolve_قاعدا(rmlvo->ctx, قاعدا, matcher,
                                       ٻاهريون, explicit_layouts);

    matcher_free(matcher);
    أرجع ret;
}


bool
xkb_components_from_قاعدا_نالا(بنية xkb_context *ctx,
                                ثابت بنية xkb_قاعدو_نالا *rmlvo,
                                بنية xkb_component_نالا *ٻاهريون,
                                xkb_layout_اشاريو_t *explicit_layouts)
{
    بنية matcher *matcher = matcher_new_from_نالا(ctx, rmlvo);
    إذا (!matcher)
        أرجع false;

    ثابت bool ret = xkb_resolve_قاعدا(ctx, rmlvo->قاعدا, matcher,
                                       ٻاهريون, explicit_layouts);

    matcher_free(matcher);
    أرجع ret;
}
