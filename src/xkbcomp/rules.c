/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rules.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "rmlvo.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "include.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "scanner-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-numbers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-paths.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_INCLUDE_ਡੂੰਘਾਈ 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
ਸੰਘ lvalue {
    ਬਣਤਰ sval string;
};

ਗਿਣਤੀ ਨਿਯਮਾਂ_token {
    TOK_ਸਿਰਾ_ਦਾ_FILE = 0,
    TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ,
    TOK_IDENTIFIER,
    TOK_ਸਮੂਹ_NAME,
    TOK_BANG,
    TOK_EQUALS,
    TOK_WILD_CARD_ਤਾਰਾ,
    TOK_WILD_CARD_NONE,
    TOK_WILD_CARD_SOME,
    TOK_WILD_CARD_ANY,
    TOK_INCLUDE,
    TOK_ERROR
};

ਸਥਿਰ ਗਿਣਤੀ ਨਿਯਮਾਂ_token
lex(ਬਣਤਰ scanner *s, ਸੰਘ lvalue *val)
{
skip_ਹੋਰ_whitespace_and_comments:
    /* Skip spaces */
    ਜਦੋਂ (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    ਜੇ (scanner_lit(s, "//")) {
        scanner_skip_ਮੰਜ਼ਿਲ_eol(s);
    }

    /* New line */
    ਜੇ (scanner_eol(s)) {
        ਜਦੋਂ (scanner_eol(s)) scanner_ਅਗਲਾ(s);
        ਵਾਪਸ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ;
    }

    /* Escaped line continuation */
    ਜੇ (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        ਜੇ (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            ਵਾਪਸ TOK_ERROR;
        }
        scanner_ਅਗਲਾ(s);
        ਜਾਓ skip_ਹੋਰ_whitespace_and_comments;
    }

    /* See if we're done */
    ਜੇ (scanner_eof(s)) ਵਾਪਸ TOK_ਸਿਰਾ_ਦਾ_FILE;

    /* New token */
    s->token_pos = s->pos;
    ਗਿਣਤੀ ਨਿਯਮਾਂ_token token = TOK_IDENTIFIER;

    ਅਚਲ ਅੱਖਰ ਅਗਲਾ = scanner_ਅਗਲਾ(s);
    ਚੋਣ (ਅਗਲਾ) {
    ਹਾਲਤ '!':
        ਵਾਪਸ TOK_BANG;
    ਹਾਲਤ '$':
        /*ਸਮੂਹ*/
        token = TOK_ਸਮੂਹ_NAME;
        val->string.len = 0;
        ਰੋਕੋ;
    ਹਾਲਤ '*':
        ਜੇ (scanner_ਨਿਯਮਾਂ_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            ਰੋਕੋ;
        } ਨਹੀਂਤਾਂ {
            /* Legacy wild card */
            ਵਾਪਸ TOK_WILD_CARD_ਤਾਰਾ;
        }
    ਹਾਲਤ '<':
        /* Extended wild cards */
        ਜੇ (scanner_ਨਿਯਮਾਂ_lit_token(s, "none>")) ਵਾਪਸ TOK_WILD_CARD_NONE;
        ਜੇ (scanner_ਨਿਯਮਾਂ_lit_token(s, "some>")) ਵਾਪਸ TOK_WILD_CARD_SOME;
        ਜੇ (scanner_ਨਿਯਮਾਂ_lit_token(s, "any>")) ਵਾਪਸ TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        ਪ੍ਰਤਿਜ੍ਞਾ(scanner_ਨਿਯਮਾਂ_is_ident('<'));
        val->string.len = 1;
        ਰੋਕੋ;
    ਹਾਲਤ '=':
        ਜੇ (scanner_ਨਿਯਮਾਂ_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            ਰੋਕੋ;
        } ਨਹੀਂਤਾਂ {
            ਵਾਪਸ TOK_EQUALS;
        }
    ਹਾਲਤ 'i':
        ਜੇ (scanner_ਨਿਯਮਾਂ_lit_token(s, "nclude")) {
            /* Include statement */
            ਵਾਪਸ TOK_INCLUDE;
        }
        /* Identifier */
        ਪ੍ਰਤਿਜ੍ਞਾ(scanner_ਨਿਯਮਾਂ_is_ident('i'));
        val->string.len = 1;
        ਰੋਕੋ;
    ਮੂਲ:
        ਜੇ (likely(scanner_ਨਿਯਮਾਂ_is_ident(ਅਗਲਾ))) {
            /* Identifier */
            val->string.len = 1;
        } ਨਹੀਂਤਾਂ {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.ਸ਼ੁਰੂ = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    ਪ੍ਰਤਿਜ੍ਞਾ(scanner_ਨਿਯਮਾਂ_is_ident(ਮਿਲਾਓ_OVERRIDE_ਅਗੇਤਰ));
    ਪ੍ਰਤਿਜ੍ਞਾ(scanner_ਨਿਯਮਾਂ_is_ident(ਮਿਲਾਓ_AUGMENT_ਅਗੇਤਰ));
    ਪ੍ਰਤਿਜ੍ਞਾ(scanner_ਨਿਯਮਾਂ_is_ident(ਮਿਲਾਓ_REPLACE_ਅਗੇਤਰ));
    ਪ੍ਰਤਿਜ੍ਞਾ(scanner_ਨਿਯਮਾਂ_is_ident('('));
    ਪ੍ਰਤਿਜ੍ਞਾ(scanner_ਨਿਯਮਾਂ_is_ident(')'));
    val->string.len += scanner_ਨਿਯਮਾਂ_ident_token(s);

    ਜੇ (unlikely(val->string.len == 0)) {
        ਜੇ (token == TOK_ਸਮੂਹ_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } ਨਹੀਂਤਾਂ {
            scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                        "unrecognized token");
        }
        ਵਾਪਸ TOK_ERROR;
    }

    ਵਾਪਸ token;
}

/***====================================================================***/

ਗਿਣਤੀ ਨਿਯਮਾਂ_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_ਅੰਕ_ENTRIES
};

ਗਿਣਤੀ {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t mlvo_ਸੂਚਕਾਂਕ_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t mlvo_ਨਕਾਬ_t;


ਸਥਿਰ ਅਚਲ ਬਣਤਰ sval ਨਿਯਮਾਂ_mlvo_svals[MLVO_ਅੰਕ_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

ਗਿਣਤੀ ਨਿਯਮਾਂ_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_ਅੰਕ_ENTRIES
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t kccgst_ਸੂਚਕਾਂਕ_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uint8_t kccgst_ਨਕਾਬ_t;

ਸਥਿਰ ਅਚਲ ਬਣਤਰ sval ਨਿਯਮਾਂ_kccgst_svals[KCCGST_ਅੰਕ_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

ਗਿਣਤੀ {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_ਨਕਾਬ_t)XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL ==
              (xkb_layout_ਨਕਾਬ_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
ਬਣਤਰ matched_sval {
    ਬਣਤਰ sval sval;
    /** Used for layout-specific options */
    xkb_layout_ਨਕਾਬ_t layouts;
    xkb_layout_ਨਕਾਬ_t matched;
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray(ਬਣਤਰ matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
ਬਣਤਰ ਨਿਯਮ_ਨਾਂ {
    ਬਣਤਰ matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

ਬਣਤਰ ਸਮੂਹ {
    ਬਣਤਰ sval name;
    darray_sval elements;
};

ਬਣਤਰ mapping {
    ਗਿਣਤੀ ਨਿਯਮਾਂ_mlvo mlvo_at_pos[MLVO_ਅੰਕ_ENTRIES];
    mlvo_ਸੂਚਕਾਂਕ_t ਅੰਕ_mlvo;
    mlvo_ਨਕਾਬ_t defined_mlvo_ਨਕਾਬ;
    bool has_layout_idx_range;
    bool has_ਗੁਣਜ_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    ਸੰਘ {
        ਬਣਤਰ { xkb_layout_ਸੂਚਕਾਂਕ_t layout_idx, variant_idx; };
        ਬਣਤਰ { xkb_layout_ਸੂਚਕਾਂਕ_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    ਸੰਘ {
        xkb_layout_ਨਕਾਬ_t ਸਰਗਰਮ;
        xkb_layout_ਨਕਾਬ_t layouts_candidates_ਨਕਾਬ;
    };
    ਗਿਣਤੀ ਨਿਯਮਾਂ_kccgst kccgst_at_pos[KCCGST_ਅੰਕ_ENTRIES];
    kccgst_ਸੂਚਕਾਂਕ_t ਅੰਕ_kccgst;
    kccgst_ਨਕਾਬ_t defined_kccgst_ਨਕਾਬ;
};

ਗਿਣਤੀ mlvo_ਮੇਲ_type {
    /** Match the given plain value */
    MLVO_ਮੇਲ_ਸਾਧਾਰਨ = 0,
    /** Match depending on the value of `wildcard_match_type` */
    MLVO_ਮੇਲ_WILDCARD_LEGACY,
    /** Match empty value */
    MLVO_ਮੇਲ_WILDCARD_NONE,
    /** Match non-empty value */
    MLVO_ਮੇਲ_WILDCARD_SOME,
    /** Match any value, optionally empty */
    MLVO_ਮੇਲ_WILDCARD_ANY,
    /** Match any entry in a group */
    MLVO_ਮੇਲ_ਸਮੂਹ,
};

ਗਿਣਤੀ wildcard_ਮੇਲ_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_ਮੇਲ_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_ਮੇਲ_ALL,
};

ਬਣਤਰ ਨਿਯਮ_2 {
    ਬਣਤਰ sval mlvo_ਮੁੱਲ_at_pos[MLVO_ਅੰਕ_ENTRIES];
    ਗਿਣਤੀ mlvo_ਮੇਲ_type ਮੇਲ_type_at_pos[MLVO_ਅੰਕ_ENTRIES];
    ਬਣਤਰ sval kccgst_ਮੁੱਲ_at_pos[KCCGST_ਅੰਕ_ENTRIES];
    mlvo_ਸੂਚਕਾਂਕ_t ਅੰਕ_mlvo_ਮੁੱਲਾਂ;
    kccgst_ਸੂਚਕਾਂਕ_t ਅੰਕ_kccgst_ਮੁੱਲਾਂ;
    bool skip;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਟੁਕੜਾ_KCCGST_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ_ਮਾਪ 4
static_assert(!((1u << (ਟੁਕੜਾ_KCCGST_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ_ਮਾਪ - 1)) &
                (KCCGST_ਅੰਕ_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
ਬਣਤਰ kccgst_ਬਫਰ_ਟੁਕੜਾ {
    uint32_t ਲੰਮਾਈ:(32 - ਟੁਕੜਾ_KCCGST_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ_ਮਾਪ);
    ਗਿਣਤੀ ਨਿਯਮਾਂ_kccgst kccgst:ਟੁਕੜਾ_KCCGST_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ_ਮਾਪ;
    xkb_layout_ਸੂਚਕਾਂਕ_t layout;
};

/* Buffer for pending KcCGST values */
ਬਣਤਰ kccgst_ਬਫਰ {
    darray_char ਬਫਰ;
    /* Slice corresponding to each value in the buffer */
    darray(ਬਣਤਰ kccgst_ਬਫਰ_ਟੁਕੜਾ) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
ਬਣਤਰ matcher {
    ਬਣਤਰ xkb_context *ctx;
    /* Input.*/
    ਬਣਤਰ ਨਿਯਮ_ਨਾਂ rmlvo;
    ਸੰਘ lvalue val;
    darray(ਬਣਤਰ ਸਮੂਹ) groups;
    /* Current mapping. */
    ਬਣਤਰ mapping mapping;
    /* Current rule. */
    ਬਣਤਰ ਨਿਯਮ_2 ਨਿਯਮ_2;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    ਬਣਤਰ kccgst_ਬਫਰ pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_ਅੰਕ_ENTRIES];
};

ਸਥਿਰ ਬਣਤਰ sval
strip_spaces(ਬਣਤਰ sval v)
{
    ਜਦੋਂ (v.len > 0 && is_space(v.ਸ਼ੁਰੂ[0])) { v.len--; v.ਸ਼ੁਰੂ++; }
    ਜਦੋਂ (v.len > 0 && is_space(v.ਸ਼ੁਰੂ[v.len - 1])) v.len--;
    ਵਾਪਸ v;
}

ਸਥਿਰ darray_matched_sval
ਵੰਡੋ_comma_separated_mlvo(ਬਣਤਰ xkb_context *ctx,
                           ਗਿਣਤੀ ਨਿਯਮਾਂ_mlvo mlvo, ਅਚਲ ਅੱਖਰ *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    ਜੇ (!s) {
        ensure_at_ਘੱਟ_ਤੋਂ_ਘੱਟ_one_ਮੁੱਲ:
        /* Label followed by a declaration is a C23 extension */ ;

        ਬਣਤਰ matched_sval val = {
            .sval = SVAL(ਨਿਰਾ, 0),
            .layouts = (xkb_layout_ਨਕਾਬ_t)XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        ਵਾਪਸ arr;
    }

    ਜਦੋਂ (true) {
        ਬਣਤਰ matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_ਨਕਾਬ_t)XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL,
            .matched = 0,
        };
        ਜਦੋਂ (*s != '\0' && *s != ',' && *s != OPTIONS_ਸਮੂਹ_SPECIFIER_ਅਗੇਤਰ) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        ਜੇ (*s == OPTIONS_ਸਮੂਹ_SPECIFIER_ਅਗੇਤਰ) {
            /* Handle numeric layout index */
            s++;
            ਅਚਲ ਅੱਖਰ* ਅਚਲ layout_ਸ਼ੁਰੂ = s;
            xkb_layout_ਸੂਚਕਾਂਕ_t layout = XKB_LAYOUT_INVALID;

            ਪੂਰਨ ਗਿਣਤੀ_2 = parse_dec_ਮੰਜ਼ਿਲ_uint32_t(s, SIZE_MAX, &layout);
            ਜੇ (ਗਿਣਤੀ_2 > 0) {
                /* Note: 1-indexed layout */
                s += ਗਿਣਤੀ_2;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                ਜੇ (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) val.sval.len, val.sval.ਸ਼ੁਰੂ);
                } ਨਹੀਂਤਾਂ ਜੇ (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) val.sval.len, val.sval.ਸ਼ੁਰੂ);
                } ਨਹੀਂਤਾਂ {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ਅਚਲ ਅੱਖਰ* ਅਚਲ layout_ਸੂਚਕਾਂਕ_ਸਿਰਾ = s;
            ਜਦੋਂ (*s != '\0' && *s != ',') { s++; }
            ਜੇ (ਗਿਣਤੀ_2 <= 0 || layout_ਸੂਚਕਾਂਕ_ਸਿਰਾ != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (s - layout_ਸ਼ੁਰੂ), layout_ਸ਼ੁਰੂ,
                        (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) val.sval.len, val.sval.ਸ਼ੁਰੂ);
            }
        }

        ਜੇ (val.sval.len || mlvo != MLVO_OPTION) {
            ਜੇ (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                ਬਣਤਰ matched_sval *prev;
                darray_foreach(prev, arr) {
                    ਜੇ (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        ਜੇ (val.layouts ==
                            (xkb_layout_ਨਕਾਬ_t)XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } ਨਹੀਂਤਾਂ ਜੇ (prev->layouts != (xkb_layout_ਨਕਾਬ_t)
                                   XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        ਜਾਓ ਅਗਲਾ_ਮੁੱਲ;
                    }
                }
            }
            darray_append(arr, val);
        }

ਅਗਲਾ_ਮੁੱਲ:
        ਜੇ (*s == '\0') ਰੋਕੋ;
        ਜੇ (*s == ',') s++;
    }

    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(arr))
        ਜਾਓ ensure_at_ਘੱਟ_ਤੋਂ_ਘੱਟ_one_ਮੁੱਲ;

    ਵਾਪਸ arr;
}

ਸਥਿਰ ਬਣਤਰ matcher *
matcher_new_from_rmlvo(ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder *rmlvo, ਅਚਲ ਅੱਖਰ **ਨਿਯਮਾਂ)
{
    ਬਣਤਰ matcher *m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*m));
    ਜੇ (!m)
        ਵਾਪਸ ਨਿਰਾ;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ ਨਾਂ = {
        .ਨਿਯਮਾਂ = rmlvo->ਨਿਯਮਾਂ,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_ਖਾਲੀ_ਥਾਵਾਂ(rmlvo->layouts)) ? ਨਿਰਾ : "x",
        .variant = (darray_ਖਾਲੀ_ਥਾਵਾਂ(rmlvo->layouts)) ? ਨਿਰਾ : "x",
        .options = (darray_ਖਾਲੀ_ਥਾਵਾਂ(rmlvo->options)) ? ਨਿਰਾ : "x",
    };
    ਅਚਲ ਗਿਣਤੀ RMLVO ਬਦਲਿਆ = xkb_context_sanitize_ਨਿਯਮ_ਨਾਂ(rmlvo->ctx,
                                                               &ਨਾਂ);

    /* Initialize matcher with sanitized names, if relevant */
    ਜੇ (ਬਦਲਿਆ & RMLVO_ਨਿਯਮਾਂ) {
        *ਨਿਯਮਾਂ = ਨਾਂ.ਨਿਯਮਾਂ;
    } ਨਹੀਂਤਾਂ {
        *ਨਿਯਮਾਂ = rmlvo->ਨਿਯਮਾਂ;
    }

    ਜੇ (ਬਦਲਿਆ & RMLVO_MODEL) {
        m->rmlvo.model.sval.ਸ਼ੁਰੂ = ਨਾਂ.model;
    } ਨਹੀਂਤਾਂ {
        m->rmlvo.model.sval.ਸ਼ੁਰੂ = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_ਸੁਰੱਖਿਅਤ(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_ਨਕਾਬ_t)XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL;

    ਪ੍ਰਤਿਜ੍ਞਾ((ਬਦਲਿਆ & RMLVO_LAYOUT) || !(ਬਦਲਿਆ & RMLVO_VARIANT));
    ਜੇ (ਬਦਲਿਆ & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = ਵੰਡੋ_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      ਨਾਂ.layout);
        m->rmlvo.variants = ਵੰਡੋ_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       ਨਾਂ.variant);
        ਜੇ (darray_ਮਾਪ(m->rmlvo.layouts) > darray_ਮਾਪ(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            ਜੇ (!isempty(ਨਾਂ.variant))
                log_warn(m->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         ਨਾਂ.layout ? ਨਾਂ.layout : "(none)",
                         ਨਾਂ.variant ? ਨਾਂ.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_ਮਾਪ(m->rmlvo.layouts));
        } ਨਹੀਂਤਾਂ ਜੇ (darray_ਮਾਪ(m->rmlvo.layouts) < darray_ਮਾਪ(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    ਨਾਂ.layout ? ਨਾਂ.layout : "(none)",
                    ਨਾਂ.variant ? ਨਾਂ.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_ਮਾਪ(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } ਨਹੀਂਤਾਂ {
        ਬਣਤਰ xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            ਬਣਤਰ matched_sval val = {
                .sval = SVAL(layout->layout, strlen_ਸੁਰੱਖਿਅਤ(layout->layout)),
                .layouts = (xkb_layout_ਨਕਾਬ_t)XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL,
                .matched = 0
            };
            darray_append(m->rmlvo.layouts, val);
            val.sval.ਸ਼ੁਰੂ = layout->variant;
            val.sval.len = strlen_ਸੁਰੱਖਿਅਤ(layout->variant);
            darray_append(m->rmlvo.variants, val);
        }
    }

    ਜੇ (ਬਦਲਿਆ & RMLVO_OPTIONS) {
        m->rmlvo.options = ਵੰਡੋ_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      ਨਾਂ.options);
    } ਨਹੀਂਤਾਂ {
        ਬਣਤਰ xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            ਬਣਤਰ matched_sval val = {
                .sval = SVAL(option->option, strlen_ਸੁਰੱਖਿਅਤ(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    ਵਾਪਸ m;
}

ਸਥਿਰ ਬਣਤਰ matcher *
matcher_new_from_ਨਾਂ(ਬਣਤਰ xkb_context *ctx,
            ਅਚਲ ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo)
{
    ਬਣਤਰ matcher *m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(*m));
    ਜੇ (!m)
        ਵਾਪਸ ਨਿਰਾ;

    m->ctx = ctx;
    m->rmlvo.model.sval.ਸ਼ੁਰੂ = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_ਸੁਰੱਖਿਅਤ(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_ਨਕਾਬ_t)XKB_OPTION_LAYOUT_ਨਕਾਬ_GLOBAL;
    m->rmlvo.layouts = ਵੰਡੋ_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = ਵੰਡੋ_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = ਵੰਡੋ_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    ਜੇ (darray_ਮਾਪ(m->rmlvo.layouts) > darray_ਮਾਪ(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        ਜੇ (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_ਮਾਪ(m->rmlvo.layouts));
    } ਨਹੀਂਤਾਂ ਜੇ (darray_ਮਾਪ(m->rmlvo.layouts) < darray_ਮਾਪ(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_ਮਾਪ(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    ਵਾਪਸ m;
}

ਸਥਿਰ ਖਾਲੀ
matcher_free(ਬਣਤਰ matcher *m)
{
    ਜੇ (!m)
        ਵਾਪਸ;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    ਬਣਤਰ ਸਮੂਹ *ਸਮੂਹ;
    darray_foreach(ਸਮੂਹ, m->groups)
        darray_free(ਸਮੂਹ->elements);
    darray_free(m->pending_kccgst.ਬਫਰ);
    darray_free(m->pending_kccgst.slices);
    ਲਈ (kccgst_ਸੂਚਕਾਂਕ_t i = 0; i < (kccgst_ਸੂਚਕਾਂਕ_t) KCCGST_ਅੰਕ_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    ਮੁਕਤ(m);
}

ਸਥਿਰ ਖਾਲੀ
matcher_ਸਮੂਹ_ਸ਼ੁਰੂ_new(ਬਣਤਰ matcher *m, ਬਣਤਰ sval name)
{
    ਬਣਤਰ ਸਮੂਹ ਸਮੂਹ = { .name = name, .elements = darray_new() };
    darray_append(m->groups, ਸਮੂਹ);
}

ਸਥਿਰ ਖਾਲੀ
matcher_ਸਮੂਹ_ਜੋੜੋ_element(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
                          ਬਣਤਰ sval element)
{
    darray_append(darray_ਵਸਤੂ(m->groups, darray_ਮਾਪ(m->groups) - 1).elements,
                  element);
}

ਸਥਿਰ bool
read_ਨਿਯਮਾਂ_file(ਬਣਤਰ xkb_context *ctx,
                ਬਣਤਰ matcher *matcher,
                ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ,
                FILE *file,
                ਅਚਲ ਅੱਖਰ *ਰਸਤਾ);

ਸਥਿਰ ਖਾਲੀ
matcher_include(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *ਮਾਪਾ_scanner,
                ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ,
                ਬਣਤਰ sval inc)
{
    ਜੇ (include_ਡੂੰਘਾਈ >= MAX_INCLUDE_ਡੂੰਘਾਈ) {
        scanner_err(ਮਾਪਾ_scanner, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_ਡੂੰਘਾਈ);
        ਵਾਪਸ;
    }

    ਅਚਲ ਅੱਖਰ *stmt_file = inc.ਸ਼ੁਰੂ;
    ਮਾਪ_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    ਅੱਖਰ buf[PATH_MAX] = {0};
    ਅਚਲ ssize_t expanded = expand_ਰਸਤਾ(m->ctx, ਮਾਪਾ_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_ਨਿਯਮਾਂ,
                                         buf, ਆਕਾਰ(buf));
    ਜੇ (expanded < 0) {
        /* Error */
        ਵਾਪਸ;
    } ਨਹੀਂਤਾਂ ਜੇ (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (ਮਾਪ_t) expanded;
        ਪ੍ਰਤਿਜ੍ਞਾ(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = 0;
    ਅਚਲ bool absolute_ਰਸਤਾ = is_absolute_ਰਸਤਾ(stmt_file);
    ਜੇ (absolute_ਰਸਤਾ) {
        /* Absolute path: no need for lookup in XKB paths */
        ਜੇ (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            ਜੇ (stmt_file_len < ਆਕਾਰ(buf)) {
                ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } ਨਹੀਂਤਾਂ {
                log_err(m->ctx, XKB_ERROR_INVALID_ਰਸਤਾ,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, ਆਕਾਰ(buf),
                        (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) stmt_file_len, stmt_file);
                ਵਾਪਸ;
            }
        } ਨਹੀਂਤਾਂ {
            /* %-expansion is always NULL-terminated */
            ਪ੍ਰਤਿਜ੍ਞਾ(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } ਨਹੀਂਤਾਂ {
        /* Relative path: lookup the first XKB path */
        ਜੇ (unlikely(expanded)) {
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
            file = ਨਿਰਾ;
        } ਨਹੀਂਤਾਂ {
            file = ਲੱਭੋ_File_ਵਿੱਚ_Xkb_ਰਸਤਾ(m->ctx, ਮਾਪਾ_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_ਨਿਯਮਾਂ,
                                     buf, ਆਕਾਰ(buf), &ਵਿੱਥ, true);
        }
    }

    ਜਦੋਂ (file) {
        ਪ੍ਰਤਿਜ੍ਞਾ(strlen_ਸੁਰੱਖਿਅਤ(buf) < ਆਕਾਰ(buf));
        bool ret = read_ਨਿਯਮਾਂ_file(m->ctx, m, include_ਡੂੰਘਾਈ + 1, file, buf);
        fclose(file);
        ਜੇ (ret)
            ਵਾਪਸ;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        ਜੇ (absolute_ਰਸਤਾ) {
            /* There is no point to search further if the path is absolute */
            ਰੋਕੋ;
        }

        /* Try next XKB path */
        ਵਿੱਥ++;
        file = ਲੱਭੋ_File_ਵਿੱਚ_Xkb_ਰਸਤਾ(m->ctx, ਮਾਪਾ_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_ਨਿਯਮਾਂ,
                                 buf, ਆਕਾਰ(buf), &ਵਿੱਥ, true);
    }

    log_err(m->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) stmt_file_len, stmt_file);
}

ਸਥਿਰ ਖਾਲੀ
matcher_mapping_ਸ਼ੁਰੂ_new(ਬਣਤਰ matcher *m)
{
    ਲਈ (mlvo_ਸੂਚਕਾਂਕ_t i = 0; i < (mlvo_ਸੂਚਕਾਂਕ_t) MLVO_ਅੰਕ_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_ਅੰਕ_ENTRIES;
    ਲਈ (kccgst_ਸੂਚਕਾਂਕ_t i = 0; i < (kccgst_ਸੂਚਕਾਂਕ_t) KCCGST_ਅੰਕ_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_ਅੰਕ_ENTRIES;
    m->mapping.has_ਗੁਣਜ_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.ਅੰਕ_mlvo = m->mapping.ਅੰਕ_kccgst = 0;
    m->mapping.defined_mlvo_ਨਕਾਬ = 0;
    m->mapping.defined_kccgst_ਨਕਾਬ = 0;
    m->mapping.ਸਰਗਰਮ = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
ਸਥਿਰ ਪੂਰਨ
parse_layout_int_ਸੂਚਕਾਂਕ_ਪੂਛ(ਅਚਲ ਅੱਖਰ *s, ਮਾਪ_t max_len,
                            xkb_layout_ਸੂਚਕਾਂਕ_t *ਬਾਹਰਲਾ)
{
    ਜੇ (max_len < 3)
        ਵਾਪਸ -1;
    uint32_t val = 0;
    ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 = parse_dec_ਮੰਜ਼ਿਲ_uint32_t(&s[1], max_len - 2, &val);
    ਜੇ (ਗਿਣਤੀ_2 <= 0 || s[1 + ਗਿਣਤੀ_2] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        ਵਾਪਸ -1;
    /* To zero-based index. */
    *ਬਾਹਰਲਾ = val - 1;
    ਵਾਪਸ ਗਿਣਤੀ_2 + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
ਸਥਿਰ ਪੂਰਨ
ਕੱਢੋ_layout_ਸੂਚਕਾਂਕ(ਅਚਲ ਅੱਖਰ *s, ਮਾਪ_t max_len, xkb_layout_ਸੂਚਕਾਂਕ_t *ਬਾਹਰਲਾ)
{
    /* This function is pretty stupid, but works for now. */
    *ਬਾਹਰਲਾ = XKB_LAYOUT_INVALID;
    ਜੇ (max_len < 3 || s[0] != '[')
        ਵਾਪਸ -1;
    ਜੇ (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *ਬਾਹਰਲਾ = (xkb_layout_ਸੂਚਕਾਂਕ_t)(s[1] - '1');
        ਵਾਪਸ 3;
    }
    ਜੇ (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        ਵਾਪਸ 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    ਵਾਪਸ parse_layout_int_ਸੂਚਕਾਂਕ_ਪੂਛ(s, max_len, ਬਾਹਰਲਾ);
}

/* Special layout indices */
ਗਿਣਤੀ layout_ਸੂਚਕਾਂਕ_ranges {
    LAYOUT_ਸੂਚਕਾਂਕ_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_ਸੂਚਕਾਂਕ_ਪਹਿਲਾ,
    LAYOUT_ਸੂਚਕਾਂਕ_LATER,
    LAYOUT_ਸੂਚਕਾਂਕ_ਗੁਣਜ,
    LAYOUT_ਸੂਚਕਾਂਕ_ANY,
};

static_assert((xkb_layout_ਸੂਚਕਾਂਕ_t) XKB_MAX_GROUPS <
              (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_SINGLE,
              "Cannot define special indices");
static_assert((xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_SINGLE <
              (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_ਪਹਿਲਾ &&
              (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_ਪਹਿਲਾ <
              (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_LATER &&
              (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_LATER <
              (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_ਗੁਣਜ &&
              (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_ਗੁਣਜ <
              (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_ANY &&
              (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_ANY <
              (xkb_layout_ਸੂਚਕਾਂਕ_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
ਸਥਿਰ bool
ਕੱਢੋ_mapping_layout_ਸੂਚਕਾਂਕ(ਅਚਲ ਅੱਖਰ *s, ਮਾਪ_t max_len,
                             xkb_layout_ਸੂਚਕਾਂਕ_t *ਬਾਹਰਲਾ)
{
    /* Check for minimal `[` + index + `]` */
    ਜੇ (max_len < 3 || s[0] != '[') {
        *ਬਾਹਰਲਾ = XKB_LAYOUT_INVALID;
        ਵਾਪਸ false;
    }

    /* Small numeric index */
    ਜੇ (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *ਬਾਹਰਲਾ = (xkb_layout_ਸੂਚਕਾਂਕ_t)(s[1] - '1');
        ਵਾਪਸ true;
    }

    /* Named indices ranges */
    ਸਥਿਰ ਅਚਲ ਬਣਤਰ {
        ਅਚਲ ਅੱਖਰ* name;
        ਮਾਪ_t ਮਾਪ;
        xkb_layout_ਸੂਚਕਾਂਕ_t ਮੁੱਲ;
    } ਨਾਂ[] = {
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੂਚਕਾਂਕ_RANGE(ਪੂਛ, ਮੁੱਲ) \
            { (ਪੂਛ), ਆਕਾਰ(ਪੂਛ), (xkb_layout_ਸੂਚਕਾਂਕ_t)(ਮੁੱਲ) }
        ਸੂਚਕਾਂਕ_RANGE("single]",   LAYOUT_ਸੂਚਕਾਂਕ_SINGLE),
        ਸੂਚਕਾਂਕ_RANGE("first]",    LAYOUT_ਸੂਚਕਾਂਕ_ਪਹਿਲਾ),
        ਸੂਚਕਾਂਕ_RANGE("later]",    LAYOUT_ਸੂਚਕਾਂਕ_LATER),
        ਸੂਚਕਾਂਕ_RANGE("multiple]", LAYOUT_ਸੂਚਕਾਂਕ_ਗੁਣਜ),
        ਸੂਚਕਾਂਕ_RANGE("any]",      LAYOUT_ਸੂਚਕਾਂਕ_ANY),
        #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਸੂਚਕਾਂਕ_RANGE
    };

    ਲਈ (ਮਾਪ_t k = 0; k < ARRAY_ਮਾਪ(ਨਾਂ); k++) {
        ਜੇ (max_len == ਨਾਂ[k].ਮਾਪ &&
            memcmp(&s[1], ਨਾਂ[k].name, ਨਾਂ[k].ਮਾਪ - 1) == 0) {
            *ਬਾਹਰਲਾ = ਨਾਂ[k].ਮੁੱਲ;
            ਵਾਪਸ true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *ਬਾਹਰਲਾ = XKB_LAYOUT_INVALID;
    ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 = parse_layout_int_ਸੂਚਕਾਂਕ_ਪੂਛ(s, max_len, ਬਾਹਰਲਾ);
    ਵਾਪਸ ਗਿਣਤੀ_2 > 0 && (ਮਾਪ_t)ਗਿਣਤੀ_2 == max_len;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
is_mlvo_ਨਕਾਬ_defined(ਬਣਤਰ matcher *m, ਗਿਣਤੀ ਨਿਯਮਾਂ_mlvo mlvo)
{
    ਵਾਪਸ m->mapping.defined_mlvo_ਨਕਾਬ & (1u << mlvo);
}

ਸਥਿਰ ਖਾਲੀ
matcher_mapping_ਸੈੱਟ_mlvo(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
                         ਬਣਤਰ sval ident)
{
    ਗਿਣਤੀ ਨਿਯਮਾਂ_mlvo mlvo;
    ਬਣਤਰ sval mlvo_sval;

    ਲਈ (mlvo = 0; mlvo < MLVO_ਅੰਕ_ENTRIES; mlvo++) {
        mlvo_sval = ਨਿਯਮਾਂ_mlvo_svals[mlvo];

        ਜੇ (svaleq_ਅਗੇਤਰ(mlvo_sval, ident))
            ਰੋਕੋ;
    }

    /* Not found. */
    ਜੇ (mlvo >= MLVO_ਅੰਕ_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ident.len, ident.ਸ਼ੁਰੂ);
        m->mapping.ਸਰਗਰਮ = false;
        ਵਾਪਸ;
    }

    ਜੇ (is_mlvo_ਨਕਾਬ_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) mlvo_sval.len, mlvo_sval.ਸ਼ੁਰੂ);
        m->mapping.ਸਰਗਰਮ = false;
        ਵਾਪਸ;
    }

    /* If there are leftovers still, it must be an index. */
    ਜੇ (mlvo_sval.len < ident.len) {
        xkb_layout_ਸੂਚਕਾਂਕ_t idx;
        ਜੇ (!ਕੱਢੋ_mapping_layout_ਸੂਚਕਾਂਕ(ident.ਸ਼ੁਰੂ + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) mlvo_sval.len, mlvo_sval.ਸ਼ੁਰੂ);
            m->mapping.ਸਰਗਰਮ = false;
            ਵਾਪਸ;
        }

        ਜੇ (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        ਨਹੀਂਤਾਂ ਜੇ (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        ਨਹੀਂਤਾਂ {
            scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) mlvo_sval.len, mlvo_sval.ਸ਼ੁਰੂ);
            m->mapping.ਸਰਗਰਮ = false;
            ਵਾਪਸ;
        }
    } ਨਹੀਂਤਾਂ ਜੇ (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_SINGLE;
    } ਨਹੀਂਤਾਂ ਜੇ (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_ਸੂਚਕਾਂਕ_t) LAYOUT_ਸੂਚਕਾਂਕ_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    ਜੇ (((mlvo == MLVO_LAYOUT && is_mlvo_ਨਕਾਬ_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_ਨਕਾਬ_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.ਸਰਗਰਮ = false;
        ਵਾਪਸ;
    }

    m->mapping.mlvo_at_pos[m->mapping.ਅੰਕ_mlvo] = mlvo;
    m->mapping.defined_mlvo_ਨਕਾਬ |= (mlvo_ਨਕਾਬ_t) 1u << mlvo;
    m->mapping.ਅੰਕ_mlvo++;
}

ਸਥਿਰ ਖਾਲੀ
matcher_mapping_ਸੈੱਟ_layout_bounds(ਬਣਤਰ matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_ਸੂਚਕਾਂਕ_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    ਚੋਣ (idx) {
        ਹਾਲਤ XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            ਪ੍ਰਤਿਜ੍ਞਾ(!is_mlvo_ਨਕਾਬ_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_ਨਕਾਬ_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_ਨਕਾਬ = 0x1; /* active = true */
            ਰੋਕੋ;
        ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_ਮਾਪ(m->rmlvo.layouts));
            m->mapping.layouts_candidates_ਨਕਾਬ = (xkb_layout_ਨਕਾਬ_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            ਰੋਕੋ;
        ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_ਗੁਣਜ:
        ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_ਮਾਪ(m->rmlvo.layouts));
            m->mapping.layouts_candidates_ਨਕਾਬ = (xkb_layout_ਨਕਾਬ_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            ਰੋਕੋ;
        ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_SINGLE:
        ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_ਪਹਿਲਾ:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        ਮੂਲ:
            /* Mere layout index */
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = idx;
            m->mapping.layout_idx_max = idx + 1;
            m->mapping.layouts_candidates_ਨਕਾਬ = UINT32_C(1) << idx;
    }
    m->mapping.has_ਗੁਣਜ_layouts = (
        m->mapping.layout_idx_max > m->mapping.layout_idx_min &&
        m->mapping.layout_idx_max - m->mapping.layout_idx_min > 1
    );
}

ਸਥਿਰ ਖਾਲੀ
matcher_mapping_ਸੈੱਟ_kccgst(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s, ਬਣਤਰ sval ident)
{
    ਗਿਣਤੀ ਨਿਯਮਾਂ_kccgst kccgst;
    ਬਣਤਰ sval kccgst_sval;

    ਲਈ (kccgst = 0; kccgst < KCCGST_ਅੰਕ_ENTRIES; kccgst++) {
        kccgst_sval = ਨਿਯਮਾਂ_kccgst_svals[kccgst];

        ਜੇ (svaleq(ਨਿਯਮਾਂ_kccgst_svals[kccgst], ident))
            ਰੋਕੋ;
    }

    /* Not found. */
    ਜੇ (kccgst >= KCCGST_ਅੰਕ_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ident.len, ident.ਸ਼ੁਰੂ);
        m->mapping.ਸਰਗਰਮ = false;
        ਵਾਪਸ;
    }

    ਜੇ (m->mapping.defined_kccgst_ਨਕਾਬ & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) kccgst_sval.len, kccgst_sval.ਸ਼ੁਰੂ);
        m->mapping.ਸਰਗਰਮ = false;
        ਵਾਪਸ;
    }

    m->mapping.kccgst_at_pos[m->mapping.ਅੰਕ_kccgst] = kccgst;
    m->mapping.defined_kccgst_ਨਕਾਬ |= (kccgst_ਨਕਾਬ_t) 1u << kccgst;
    m->mapping.ਅੰਕ_kccgst++;
}

ਸਥਿਰ bool
matcher_mapping_verify(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s)
{
    ਜੇ (m->mapping.ਅੰਕ_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        ਜਾਓ skip;
    }

    ਜੇ (m->mapping.ਅੰਕ_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        ਜਾਓ skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    ਜੇ (is_mlvo_ਨਕਾਬ_defined(m, MLVO_LAYOUT)) {
        ਪ੍ਰਤਿਜ੍ਞਾ(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        ਚੋਣ (m->mapping.layout_idx) {
            ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                ਜੇ (darray_ਮਾਪ(m->rmlvo.layouts) > 1)
                    ਜਾਓ skip;
                ਰੋਕੋ;
            ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_ਗੁਣਜ:
            ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                ਜੇ (darray_ਮਾਪ(m->rmlvo.layouts) < 2)
                    ਜਾਓ skip;
                ਰੋਕੋ;
            ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_ANY:
            ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_ਪਹਿਲਾ:
                /* No restrictions */
                ਰੋਕੋ;
            ਮੂਲ:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                ਜੇ (darray_ਮਾਪ(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_ਮਾਪ(m->rmlvo.layouts))
                    ਜਾਓ skip;
        }
    }

    ਜੇ (is_mlvo_ਨਕਾਬ_defined(m, MLVO_VARIANT)) {
        ਪ੍ਰਤਿਜ੍ਞਾ(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        ਚੋਣ (m->mapping.variant_idx) {
            ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                ਜੇ (darray_ਮਾਪ(m->rmlvo.variants) > 1)
                    ਜਾਓ skip;
                ਰੋਕੋ;
            ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_ANY:
            ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_LATER:
            ਹਾਲਤ LAYOUT_ਸੂਚਕਾਂਕ_ਪਹਿਲਾ:
                /* No restriction */
                ਰੋਕੋ;
            ਮੂਲ:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                ਜੇ (darray_ਮਾਪ(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_ਮਾਪ(m->rmlvo.variants))
                    ਜਾਓ skip;
        }
    }

    ਵਾਪਸ true;

skip:
    m->mapping.ਸਰਗਰਮ = false;
    ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ
matcher_ਨਿਯਮ_ਸ਼ੁਰੂ_new(ਬਣਤਰ matcher *m)
{
    memset(&m->ਨਿਯਮ_2, 0, ਆਕਾਰ(m->ਨਿਯਮ_2));
    m->ਨਿਯਮ_2.skip = !m->mapping.ਸਰਗਰਮ;
}

ਸਥਿਰ ਖਾਲੀ
matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_common(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
                             ਬਣਤਰ sval ident,
                             ਗਿਣਤੀ mlvo_ਮੇਲ_type ਮੇਲ_type)
{
    ਜੇ (m->ਨਿਯਮ_2.ਅੰਕ_mlvo_ਮੁੱਲਾਂ >= m->mapping.ਅੰਕ_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->ਨਿਯਮ_2.skip = true;
        ਵਾਪਸ;
    }
    m->ਨਿਯਮ_2.ਮੇਲ_type_at_pos[m->ਨਿਯਮ_2.ਅੰਕ_mlvo_ਮੁੱਲਾਂ] = ਮੇਲ_type;
    m->ਨਿਯਮ_2.mlvo_ਮੁੱਲ_at_pos[m->ਨਿਯਮ_2.ਅੰਕ_mlvo_ਮੁੱਲਾਂ] = ident;
    m->ਨਿਯਮ_2.ਅੰਕ_mlvo_ਮੁੱਲਾਂ++;
}

ਸਥਿਰ ਖਾਲੀ
matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_wildcard(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
                               ਗਿਣਤੀ mlvo_ਮੇਲ_type ਮੇਲ_type)
{
    ਬਣਤਰ sval dummy = SVAL(ਨਿਰਾ, 0);
    matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_common(m, s, dummy, ਮੇਲ_type);
}

ਸਥਿਰ ਖਾਲੀ
matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_ਸਮੂਹ(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
                            ਬਣਤਰ sval ident)
{
    matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_common(m, s, ident, MLVO_ਮੇਲ_ਸਮੂਹ);
}

ਸਥਿਰ ਖਾਲੀ
matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
                      ਬਣਤਰ sval ident)
{
    matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_common(m, s, ident, MLVO_ਮੇਲ_ਸਾਧਾਰਨ);
}

ਸਥਿਰ ਖਾਲੀ
matcher_ਨਿਯਮ_ਸੈੱਟ_kccgst(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
                        ਬਣਤਰ sval ident)
{
    ਜੇ (m->ਨਿਯਮ_2.ਅੰਕ_kccgst_ਮੁੱਲਾਂ >= m->mapping.ਅੰਕ_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->ਨਿਯਮ_2.skip = true;
        ਵਾਪਸ;
    }
    m->ਨਿਯਮ_2.kccgst_ਮੁੱਲ_at_pos[m->ਨਿਯਮ_2.ਅੰਕ_kccgst_ਮੁੱਲਾਂ] = ident;
    m->ਨਿਯਮ_2.ਅੰਕ_kccgst_ਮੁੱਲਾਂ++;
}

ਸਥਿਰ bool
ਮੇਲ_ਸਮੂਹ(ਬਣਤਰ matcher *m, ਬਣਤਰ sval ਸਮੂਹ_name, ਬਣਤਰ sval ਮੰਜ਼ਿਲ)
{
    ਬਣਤਰ ਸਮੂਹ *ਸਮੂਹ = ਨਿਰਾ;
    ਬਣਤਰ sval *element;
    bool ਮਿਲਿਆ = false;

    darray_foreach(ਸਮੂਹ, m->groups) {
        ਜੇ (svaleq(ਸਮੂਹ->name, ਸਮੂਹ_name)) {
            ਮਿਲਿਆ = true;
            ਰੋਕੋ;
        }
    }

    ਜੇ (!ਮਿਲਿਆ) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        ਵਾਪਸ false;
    }

    darray_foreach(element, ਸਮੂਹ->elements)
        ਜੇ (svaleq(ਮੰਜ਼ਿਲ, *element))
            ਵਾਪਸ true;

    ਵਾਪਸ false;
}

ਸਥਿਰ bool
ਮੇਲ_ਮੁੱਲ(ਬਣਤਰ matcher *m, ਅਚਲ ਬਣਤਰ sval val, ਅਚਲ ਬਣਤਰ sval ਮੰਜ਼ਿਲ,
            ਗਿਣਤੀ mlvo_ਮੇਲ_type ਮੇਲ_type,
            ਗਿਣਤੀ wildcard_ਮੇਲ_type wildcard_type)
{
    ਚੋਣ (ਮੇਲ_type) {
        ਹਾਲਤ MLVO_ਮੇਲ_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            ਵਾਪਸ wildcard_type == WILDCARD_ਮੇਲ_ALL || !!ਮੰਜ਼ਿਲ.len;
        ਹਾਲਤ MLVO_ਮੇਲ_WILDCARD_NONE:
            ਵਾਪਸ !ਮੰਜ਼ਿਲ.len;
        ਹਾਲਤ MLVO_ਮੇਲ_WILDCARD_SOME:
            ਵਾਪਸ !!ਮੰਜ਼ਿਲ.len;
        ਹਾਲਤ MLVO_ਮੇਲ_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            ਵਾਪਸ true;
        ਹਾਲਤ MLVO_ਮੇਲ_ਸਮੂਹ:
            ਵਾਪਸ ਮੇਲ_ਸਮੂਹ(m, val, ਮੰਜ਼ਿਲ);
        ਮੂਲ:
            ਪ੍ਰਤਿਜ੍ਞਾ(ਮੇਲ_type == MLVO_ਮੇਲ_ਸਾਧਾਰਨ);
            ਵਾਪਸ svaleq(val, ਮੰਜ਼ਿਲ);
    }
}

ਸਥਿਰ bool
ਮੇਲ_ਮੁੱਲ_and_mark(ਬਣਤਰ matcher *m, ਅਚਲ ਬਣਤਰ sval val,
                     ਬਣਤਰ matched_sval *ਮੰਜ਼ਿਲ, ਗਿਣਤੀ mlvo_ਮੇਲ_type ਮੇਲ_type,
                     ਗਿਣਤੀ wildcard_ਮੇਲ_type wildcard_type,
                     xkb_layout_ਨਕਾਬ_t layouts)
{
    bool matched = ਮੇਲ_ਮੁੱਲ(m, val, ਮੰਜ਼ਿਲ->sval, ਮੇਲ_type, wildcard_type);
    ਜੇ (matched)
        ਮੰਜ਼ਿਲ->matched |= layouts;
    ਵਾਪਸ matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
ਸਥਿਰ bool
expand_rmlvo_ਵਿੱਚ_kccgst_ਮੁੱਲ(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
                             ਬਣਤਰ sval ਮੁੱਲ, xkb_layout_ਸੂਚਕਾਂਕ_t layout_idx,
                             darray_char *expanded, ਮਾਪ_t *i)
{
    ਅਚਲ ਅੱਖਰ *str = ਮੁੱਲ.ਸ਼ੁਰੂ;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    ਗਿਣਤੀ ਨਿਯਮਾਂ_mlvo mlv;
    xkb_layout_ਸੂਚਕਾਂਕ_t idx;
    ਅੱਖਰ pfx, sfx;
    ਬਣਤਰ matched_sval *expanded_ਮੁੱਲ;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    ਜੇ (str[*i] == 'i' &&
        (*i + 1 == ਮੁੱਲ.len || is_ਮਿਲਾਓ_ਬਹੁਲਕ_ਅਗੇਤਰ(str[*i + 1])))
    {
        ਜੇ (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_ਨਿਯਮਾਂ_INVALID_LAYOUT_ਸੂਚਕਾਂਕ_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            ਜਾਓ error;
        }
        (*i)++;
        ਅੱਖਰ ਸੂਚਕਾਂਕ_str[MAX_LAYOUT_ਸੂਚਕਾਂਕ_STR_ਲੰਮਾਈ + 1];
        ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 = snprintf(ਸੂਚਕਾਂਕ_str, ਆਕਾਰ(ਸੂਚਕਾਂਕ_str), "%"PRIu32,
                                   layout_idx + 1);
        ਪ੍ਰਤਿਜ੍ਞਾ(ਗਿਣਤੀ_2 > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, ਸੂਚਕਾਂਕ_str, (darray_ਮਾਪ_t)ਗਿਣਤੀ_2);
        ਵਾਪਸ true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    ਜੇ (str[*i] == '(' ||
        is_ਮਿਲਾਓ_ਬਹੁਲਕ_ਅਗੇਤਰ(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        ਜੇ (str[*i] == '(') sfx = ')';
        ਜੇ (++(*i) >= ਮੁੱਲ.len) ਜਾਓ error;
    }

    /* Mandatory model/layout/variant specifier. */
    ਚੋਣ (str[(*i)++]) {
    ਹਾਲਤ 'm': mlv = MLVO_MODEL; ਰੋਕੋ;
    ਹਾਲਤ 'l': mlv = MLVO_LAYOUT; ਰੋਕੋ;
    ਹਾਲਤ 'v': mlv = MLVO_VARIANT; ਰੋਕੋ;
    ਮੂਲ: ਜਾਓ error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_ਸੂਚਕਾਂਕ = false;
    ਜੇ (*i < ਮੁੱਲ.len && str[*i] == '[') {
        ਜੇ (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            ਜਾਓ error;
        }

        ਪੂਰਨ consumed = ਕੱਢੋ_layout_ਸੂਚਕਾਂਕ(str + (*i), ਮੁੱਲ.len - (*i), &idx);
        ਜੇ (consumed <= 0) ਜਾਓ error;
        ਜੇ (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            ਪ੍ਰਤਿਜ੍ਞਾ(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_ਸੂਚਕਾਂਕ = true;
        }
        *i += (ਮਾਪ_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    ਜੇ (sfx != 0) {
        ਜੇ (*i >= ਮੁੱਲ.len) ਜਾਓ error;
        ਜੇ (str[(*i)++] != sfx) ਜਾਓ error;
    }

    /* Get the expanded value. */
    expanded_ਮੁੱਲ = ਨਿਰਾ;

    ਜੇ (mlv == MLVO_LAYOUT) {
        ਜੇ (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            ਜੇ (darray_ਮਾਪ(m->rmlvo.layouts) == 1)
                expanded_ਮੁੱਲ = &darray_ਵਸਤੂ(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } ਨਹੀਂਤਾਂ ਜੇ (idx < darray_ਮਾਪ(m->rmlvo.layouts) &&
                   (variable_ਸੂਚਕਾਂਕ || darray_ਮਾਪ(m->rmlvo.layouts) > 1)) {
                expanded_ਮੁੱਲ = &darray_ਵਸਤੂ(m->rmlvo.layouts, idx);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ (mlv == MLVO_VARIANT) {
        ਜੇ (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            ਜੇ (darray_ਮਾਪ(m->rmlvo.variants) == 1)
                expanded_ਮੁੱਲ = &darray_ਵਸਤੂ(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } ਨਹੀਂਤਾਂ ਜੇ (idx < darray_ਮਾਪ(m->rmlvo.variants) &&
                   (variable_ਸੂਚਕਾਂਕ || darray_ਮਾਪ(m->rmlvo.variants) > 1)) {
                expanded_ਮੁੱਲ = &darray_ਵਸਤੂ(m->rmlvo.variants, idx);
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ (mlv == MLVO_MODEL) {
        expanded_ਮੁੱਲ = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    ਜੇ (!expanded_ਮੁੱਲ || expanded_ਮੁੱਲ->sval.len == 0) {
        ਵਾਪਸ true;
    }

    ਜੇ (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_ਮੁੱਲ->sval.ਸ਼ੁਰੂ,
                                 (darray_ਮਾਪ_t) expanded_ਮੁੱਲ->sval.len);
    ਜੇ (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    ਜੇ (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ਅਚਲ xkb_layout_ਨਕਾਬ_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_ਮੁੱਲ->matched |= layouts;
    } ਨਹੀਂਤਾਂ {
        expanded_ਮੁੱਲ->matched |= (xkb_layout_ਨਕਾਬ_t)GLOBAL_MATCHED_LAYOUTS;
    }

    ਵਾਪਸ true;

error:
    scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                "invalid %%-expansion in value; not used");
    ਵਾਪਸ false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
ਸਥਿਰ ਖਾਲੀ
expand_qualifier_ਵਿੱਚ_kccgst_ਮੁੱਲ(
    ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
    ਬਣਤਰ sval ਮੁੱਲ, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_ਮਾਪ_t ਅਗੇਤਰ_idx, ਮਾਪ_t *i)
{
    ਅਚਲ ਅੱਖਰ *str = ਮੁੱਲ.ਸ਼ੁਰੂ;

    /* “all” followed by nothing or by a layout separator */
    ਜੇ (*i + 2 < ਮੁੱਲ.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == ਮੁੱਲ.len || is_ਮਿਲਾਓ_ਬਹੁਲਕ_ਅਗੇਤਰ(str[*i + 3]))) {
        ਜੇ (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        ਜੇ (darray_ਮਾਪ(m->rmlvo.layouts) > 1) {
            ਅੱਖਰ layout_ਸੂਚਕਾਂਕ[MAX_LAYOUT_ਸੂਚਕਾਂਕ_STR_ਲੰਮਾਈ + 1];
            ਅਚਲ darray_ਮਾਪ_t ਅਗੇਤਰ_ਲੰਮਾਈ =
                darray_ਮਾਪ(*expanded) - ਅਗੇਤਰ_idx - 1;
            ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_ਮਾਪ(m->rmlvo.layouts));
                 l++)
            {
                ਜੇ (!has_separator)
                    darray_append(*expanded, ਮਿਲਾਓ_DEFAULT_ਅਗੇਤਰ);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_ਵਸਤੂ(*expanded, ਅਗੇਤਰ_idx),
                                             ਅਗੇਤਰ_ਲੰਮਾਈ);
                /* Append index */
                ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 = snprintf(layout_ਸੂਚਕਾਂਕ, ਆਕਾਰ(layout_ਸੂਚਕਾਂਕ),
                                           "%"PRIu32, l + 1);
                ਪ੍ਰਤਿਜ੍ਞਾ(ਗਿਣਤੀ_2 > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_ਸੂਚਕਾਂਕ,
                                             (darray_ਮਾਪ_t)ਗਿਣਤੀ_2);
            }
        }
        *i += 3;
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
concat_kccgst(darray_char *into, darray_ਮਾਪ_t ਮਾਪ, ਵਿੱਚ_reads(ਮਾਪ) ਅਚਲ ਅੱਖਰ* from)
#ਨਹੀਂਤਾਂ
concat_kccgst(darray_char *into, darray_ਮਾਪ_t ਮਾਪ, ਅਚਲ ਅੱਖਰ from[ਸਥਿਰ ਮਾਪ])
#ਜੇ_ਅੰਤ
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ਅਚਲ bool from_plus = is_ਮਿਲਾਓ_ਬਹੁਲਕ_ਅਗੇਤਰ(from[0]);
    ਜੇ (from_plus || darray_ਖਾਲੀ_ਥਾਵਾਂ(*into)) {
        darray_appends_nullterminate(*into, from, ਮਾਪ);
    } ਨਹੀਂਤਾਂ {
        ਅਚਲ ਅੱਖਰ ch =
            (ਅੱਖਰ) (darray_ਖਾਲੀ_ਥਾਵਾਂ(*into) ? '\0' : darray_ਵਸਤੂ(*into, 0));
        ਅਚਲ bool into_plus = is_ਮਿਲਾਓ_ਬਹੁਲਕ_ਅਗੇਤਰ(ch);
        ਜੇ (into_plus)
            darray_prepends_nullterminate(*into, from, ਮਾਪ);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
ਸਥਿਰ bool
append_expanded_kccgst_ਮੁੱਲ(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
                             bool ਮਿਲਾਓ, darray_char *ਮੰਜ਼ਿਲ, ਬਣਤਰ sval ਮੁੱਲ,
                             xkb_layout_ਸੂਚਕਾਂਕ_t layout_idx)
{
    ਅਚਲ ਅੱਖਰ *str = ਮੁੱਲ.ਸ਼ੁਰੂ;
    darray_char expanded = darray_new();
    darray_ਮਾਪ_t ਆਖਰੀ_ਵਸਤੂ_idx = 0;
    bool has_separator = false;

    ਲਈ (ਮਾਪ_t i = 0; i < ਮੁੱਲ.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        ਚੋਣ (str[i]) {
            /* Qualifier */
            ਹਾਲਤ ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_ਵਿੱਚ_kccgst_ਮੁੱਲ(m, s, ਮੁੱਲ, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 ਆਖਰੀ_ਵਸਤੂ_idx, &i);
                ਰੋਕੋ;
            /* Expansion */
            ਹਾਲਤ '%':
                i++;
                ਜੇ (i >= ਮੁੱਲ.len ||
                    !expand_rmlvo_ਵਿੱਚ_kccgst_ਮੁੱਲ(m, s, ਮੁੱਲ, layout_idx,
                                                  &expanded, &i))
                        ਜਾਓ error;
                ਰੋਕੋ;
            /* New item */
            ਹਾਲਤ ਮਿਲਾਓ_OVERRIDE_ਅਗੇਤਰ:
            ਹਾਲਤ ਮਿਲਾਓ_AUGMENT_ਅਗੇਤਰ:
            ਹਾਲਤ ਮਿਲਾਓ_REPLACE_ਅਗੇਤਰ:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                ਆਖਰੀ_ਵਸਤੂ_idx = darray_ਮਾਪ(expanded) - 1;
                has_separator = true;
                ਰੋਕੋ;
            /* Just a normal character. */
            ਮੂਲ:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    ਜੇ (ਮਿਲਾਓ) {
        ਜੇ (!darray_ਖਾਲੀ_ਥਾਵਾਂ(expanded))
            concat_kccgst(ਮੰਜ਼ਿਲ, darray_ਮਾਪ(expanded), darray_ਚੀਜ਼ਾਂ(expanded));
    } ਨਹੀਂਤਾਂ {
        darray_concat(*ਮੰਜ਼ਿਲ, expanded);
    }
    darray_free(expanded);
    ਵਾਪਸ true;

error:
    darray_free(expanded);
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
matcher_append_pending_kccgst(ਬਣਤਰ matcher *m)
{
    ਜੇ (!m->mapping.has_ਗੁਣਜ_layouts)
        ਵਾਪਸ true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    ਲਈ (kccgst_ਸੂਚਕਾਂਕ_t i = 0; i < m->mapping.ਅੰਕ_kccgst; i++) {
        ਅਚਲ ਗਿਣਤੀ ਨਿਯਮਾਂ_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            ਰਜਿਸਟਰ ਅਚਲ ਬਣਤਰ kccgst_ਬਫਰ* ਅਚਲ buf = &m->pending_kccgst;
            ਮਾਪ_t ਵਿੱਥ = 0;
            ਲਈ (darray_ਮਾਪ_t k = 0; k < darray_ਮਾਪ(buf->slices); k++) {
                ਰਜਿਸਟਰ ਅਚਲ ਬਣਤਰ kccgst_ਬਫਰ_ਟੁਕੜਾ * ਅਚਲ ਟੁਕੜਾ =
                    &darray_ਵਸਤੂ(buf->slices, k);
                ਜੇ (ਟੁਕੜਾ->kccgst == kccgst && ਟੁਕੜਾ->layout == layout &&
                    ਟੁਕੜਾ->ਲੰਮਾਈ)
                    concat_kccgst(&m->kccgst[kccgst], ਟੁਕੜਾ->ਲੰਮਾਈ,
                                  darray_ਚੀਜ਼ਾਂ(buf->ਬਫਰ) + ਵਿੱਥ);
                ਵਿੱਥ += ਟੁਕੜਾ->ਲੰਮਾਈ;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_ਗੁਣਜ_layouts = false;
    ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ
matcher_ਨਿਯਮ_verify(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s)
{
    ਜੇ (m->ਨਿਯਮ_2.ਅੰਕ_mlvo_ਮੁੱਲਾਂ != m->mapping.ਅੰਕ_mlvo ||
        m->ਨਿਯਮ_2.ਅੰਕ_kccgst_ਮੁੱਲਾਂ != m->mapping.ਅੰਕ_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                    "invalid rule: must have same number of values "
                    "as mapping line; ignoring rule");
        m->ਨਿਯਮ_2.skip = true;
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
ਸਥਿਰ bool
matcher_ਨਿਯਮ_ਮੇਲ_model(ਬਣਤਰ matcher *m, ਅਚਲ ਬਣਤਰ sval ਮੁੱਲ,
                         ਗਿਣਤੀ mlvo_ਮੇਲ_type ਮੇਲ_type)
{
    ਵਾਪਸ ਮੇਲ_ਮੁੱਲ_and_mark(
        m, ਮੁੱਲ, &m->rmlvo.model, ਮੇਲ_type, WILDCARD_ਮੇਲ_ALL,
        /* layout-independent */
        (xkb_layout_ਨਕਾਬ_t)GLOBAL_MATCHED_LAYOUTS
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
ਸਥਿਰ bool
matcher_ਨਿਯਮ_ਮੇਲ_option(ਬਣਤਰ matcher *m, ਅਚਲ ਬਣਤਰ sval ਮੁੱਲ,
                          ਗਿਣਤੀ mlvo_ਮੇਲ_type ਮੇਲ_type,
                          xkb_layout_ਨਕਾਬ_t *candidates)
{
    /* There is always at least one value */
    ਪ੍ਰਤਿਜ੍ਞਾ(!darray_ਖਾਲੀ_ਥਾਵਾਂ(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_ਨਕਾਬ_t unmatched =
        (m->mapping.defined_mlvo_ਨਕਾਬ & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    ਬਣਤਰ matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        ਅਚਲ xkb_layout_ਨਕਾਬ_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_ਨਕਾਬ_t)GLOBAL_MATCHED_LAYOUTS;
        ਜੇ (matchable && ਮੇਲ_ਮੁੱਲ_and_mark(m, ਮੁੱਲ, option, ਮੇਲ_type,
                                              WILDCARD_ਮੇਲ_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            ਜੇ (!unmatched)
                ਰੋਕੋ;
        }
    }
    ਜੇ (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    ਵਾਪਸ matched;
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
ਸਥਿਰ bool
matcher_ਨਿਯਮ_ਮੇਲ_layout_or_variant(ਬਣਤਰ matcher *m,
                                     darray_matched_sval *ਨਿਵੇਸ਼,
                                     ਅਚਲ ਬਣਤਰ sval ਮੁੱਲ,
                                     ਗਿਣਤੀ mlvo_ਮੇਲ_type ਮੇਲ_type,
                                     xkb_layout_ਨਕਾਬ_t *candidates)
{
    /* There is always at least one value */
    ਪ੍ਰਤਿਜ੍ਞਾ(!darray_ਖਾਲੀ_ਥਾਵਾਂ(*ਨਿਵੇਸ਼));

    bool matched = false;
    /* Loop over the layout index range */
    ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ਅਚਲ xkb_layout_ਨਕਾਬ_t layout = (UINT32_C(1) << idx);
        ਜੇ (layout & *candidates) {
            ਬਣਤਰ matched_sval *ਮੰਜ਼ਿਲ = &darray_ਵਸਤੂ(*ਨਿਵੇਸ਼, idx);
            ਜੇ (ਮੇਲ_ਮੁੱਲ_and_mark(m, ਮੁੱਲ, ਮੰਜ਼ਿਲ, ਮੇਲ_type,
                                     WILDCARD_ਮੇਲ_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } ਨਹੀਂਤਾਂ {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    ਵਾਪਸ matched;
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
ਸਥਿਰ ਖਾਲੀ
matcher_ਨਿਯਮ_apply_if_matches(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_ਨਕਾਬ_t candidate_layouts = m->mapping.layouts_candidates_ਨਕਾਬ;

    /* Loop over MLVO pattern components */
    ਲਈ (mlvo_ਸੂਚਕਾਂਕ_t i = 0; i < m->mapping.ਅੰਕ_mlvo; i++) {
        ਅਚਲ ਗਿਣਤੀ ਨਿਯਮਾਂ_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ਅਚਲ ਬਣਤਰ sval ਮੁੱਲ = m->ਨਿਯਮ_2.mlvo_ਮੁੱਲ_at_pos[i];
        ਅਚਲ ਗਿਣਤੀ mlvo_ਮੇਲ_type ਮੇਲ_type = m->ਨਿਯਮ_2.ਮੇਲ_type_at_pos[i];
        bool matched = false;

        ਚੋਣ (mlvo) {
        ਹਾਲਤ MLVO_MODEL:
            matched = matcher_ਨਿਯਮ_ਮੇਲ_model(m, ਮੁੱਲ, ਮੇਲ_type);
            ਰੋਕੋ;
        ਹਾਲਤ MLVO_LAYOUT:
            matched = matcher_ਨਿਯਮ_ਮੇਲ_layout_or_variant(
                m, &m->rmlvo.layouts, ਮੁੱਲ, ਮੇਲ_type, &candidate_layouts
            );
            ਰੋਕੋ;
        ਹਾਲਤ MLVO_VARIANT:
            matched = matcher_ਨਿਯਮ_ਮੇਲ_layout_or_variant(
                m, &m->rmlvo.variants, ਮੁੱਲ, ਮੇਲ_type, &candidate_layouts
            );
            ਰੋਕੋ;
        ਹਾਲਤ MLVO_OPTION:
            matched = matcher_ਨਿਯਮ_ਮੇਲ_option(
                m, ਮੁੱਲ, ਮੇਲ_type, &candidate_layouts
            );
            ਰੋਕੋ;
        ਮੂਲ: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_ਅੰਕ_ENTRIES - 1,
                          "Unexpected MLVO field");
            ਪ੍ਰਤਿਜ੍ਞਾ(!"Unreachable");
        }
        }

        ਜੇ (!matched)
            ਵਾਪਸ;
    }

    ਜੇ (m->mapping.has_ਗੁਣਜ_layouts) {
        /* Special index: loop over the index range */
        ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            ਜੇ (candidate_layouts & (UINT32_C(1) << idx)) {
                ਲਈ (kccgst_ਸੂਚਕਾਂਕ_t i = 0; i < m->mapping.ਅੰਕ_kccgst; i++) {
                    ਅਚਲ ਗਿਣਤੀ ਨਿਯਮਾਂ_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ਅਚਲ ਬਣਤਰ sval ਮੁੱਲ = m->ਨਿਯਮ_2.kccgst_ਮੁੱਲ_at_pos[i];
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
                    ਬਣਤਰ kccgst_ਬਫਰ * ਅਚਲ buf = &m->pending_kccgst;
                    ਅਚਲ darray_ਮਾਪ_t prev_ਬਫਰ_ਲੰਮਾਈ =
                        darray_ਮਾਪ(buf->ਬਫਰ);
                    append_expanded_kccgst_ਮੁੱਲ(m, s, false, &buf->ਬਫਰ,
                                                 ਮੁੱਲ, idx);
                    ਅਚਲ uint32_t ਲੰਮਾਈ = (uint32_t) (darray_ਮਾਪ(buf->ਬਫਰ)
                                          - prev_ਬਫਰ_ਲੰਮਾਈ);
                    ਅਚਲ ਬਣਤਰ kccgst_ਬਫਰ_ਟੁਕੜਾ ਟੁਕੜਾ = {
                        .ਲੰਮਾਈ = ਲੰਮਾਈ,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, ਟੁਕੜਾ);
                }
            }
        }
    } ਨਹੀਂਤਾਂ {
        /* Numeric index or no index */
        ਲਈ (kccgst_ਸੂਚਕਾਂਕ_t i = 0; i < m->mapping.ਅੰਕ_kccgst; i++) {
            ਗਿਣਤੀ ਨਿਯਮਾਂ_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            ਬਣਤਰ sval ਮੁੱਲ = m->ਨਿਯਮ_2.kccgst_ਮੁੱਲ_at_pos[i];
            append_expanded_kccgst_ਮੁੱਲ(m, s, true, &m->kccgst[kccgst], ਮੁੱਲ,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    ਜੇ (!(is_mlvo_ਨਕਾਬ_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_ਨਕਾਬ &= ~candidate_layouts;
    }
}

ਸਥਿਰ ਗਿਣਤੀ ਨਿਯਮਾਂ_token
gettok(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s)
{
    ਵਾਪਸ lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
ਸਥਿਰ bool
matcher_ਮੇਲ(ਬਣਤਰ matcher *m, ਬਣਤਰ scanner *s,
              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ,
              ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len,
              ਅਚਲ ਅੱਖਰ *file_name)
{
    ਗਿਣਤੀ ਨਿਯਮਾਂ_token tok;

    ਜੇ (!m)
        ਵਾਪਸ false;

initial:
    ਚੋਣ (tok = gettok(m, s)) {
    ਹਾਲਤ TOK_BANG:
        ਜਾਓ bang;
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ:
        ਜਾਓ initial;
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_FILE:
        ਜਾਓ ਅੰਤ;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

bang:
    ਚੋਣ (tok = gettok(m, s)) {
    ਹਾਲਤ TOK_ਸਮੂਹ_NAME:
        matcher_ਸਮੂਹ_ਸ਼ੁਰੂ_new(m, m->val.string);
        ਜਾਓ ਸਮੂਹ_name;
    ਹਾਲਤ TOK_INCLUDE:
        ਜਾਓ include_statement;
    ਹਾਲਤ TOK_IDENTIFIER:
        matcher_mapping_ਸ਼ੁਰੂ_new(m);
        matcher_mapping_ਸੈੱਟ_mlvo(m, s, m->val.string);
        ਜਾਓ mapping_mlvo;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

ਸਮੂਹ_name:
    ਚੋਣ (tok = gettok(m, s)) {
    ਹਾਲਤ TOK_EQUALS:
        ਜਾਓ ਸਮੂਹ_element;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

ਸਮੂਹ_element:
    ਚੋਣ (tok = gettok(m, s)) {
    ਹਾਲਤ TOK_IDENTIFIER:
        matcher_ਸਮੂਹ_ਜੋੜੋ_element(m, s, m->val.string);
        ਜਾਓ ਸਮੂਹ_element;
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ:
        ਜਾਓ initial;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

include_statement:
    ਚੋਣ (tok = gettok(m, s)) {
    ਹਾਲਤ TOK_IDENTIFIER:
        matcher_include(m, s, include_ਡੂੰਘਾਈ, m->val.string);
        ਜਾਓ include_statement_ਸਿਰਾ;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

include_statement_ਸਿਰਾ:
    ਚੋਣ (tok = gettok(m, s)) {
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ:
        ਜਾਓ initial;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

mapping_mlvo:
    ਚੋਣ (tok = gettok(m, s)) {
    ਹਾਲਤ TOK_IDENTIFIER:
        ਜੇ (m->mapping.ਸਰਗਰਮ)
            matcher_mapping_ਸੈੱਟ_mlvo(m, s, m->val.string);
        ਜਾਓ mapping_mlvo;
    ਹਾਲਤ TOK_EQUALS:
        ਜਾਓ mapping_kccgst;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

mapping_kccgst:
    ਚੋਣ (tok = gettok(m, s)) {
    ਹਾਲਤ TOK_IDENTIFIER:
        ਜੇ (m->mapping.ਸਰਗਰਮ)
            matcher_mapping_ਸੈੱਟ_kccgst(m, s, m->val.string);
        ਜਾਓ mapping_kccgst;
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ:
        ਜੇ (m->mapping.ਸਰਗਰਮ && matcher_mapping_verify(m, s)) {
            matcher_mapping_ਸੈੱਟ_layout_bounds(m);
            ਜੇ (m->mapping.has_ਗੁਣਜ_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_ਮਾਪ(m->pending_kccgst.ਬਫਰ) = 0;
                darray_ਮਾਪ(m->pending_kccgst.slices) = 0;
            }
        }
        ਜਾਓ ਨਿਯਮ_mlvo_ਪਹਿਲਾ;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

ਨਿਯਮ_mlvo_ਪਹਿਲਾ:
    ਚੋਣ (tok = gettok(m, s)) {
    ਹਾਲਤ TOK_BANG:
        matcher_append_pending_kccgst(m);
        ਜਾਓ bang;
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ:
        ਜਾਓ ਨਿਯਮ_mlvo_ਪਹਿਲਾ;
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_FILE:
        matcher_append_pending_kccgst(m);
        ਜਾਓ ਅੰਤ;
    ਮੂਲ:
        matcher_ਨਿਯਮ_ਸ਼ੁਰੂ_new(m);
        ਜਾਓ ਨਿਯਮ_mlvo_no_tok;
    }

ਨਿਯਮ_mlvo:
    tok = gettok(m, s);
ਨਿਯਮ_mlvo_no_tok:
    ਚੋਣ (tok) {
    ਹਾਲਤ TOK_IDENTIFIER:
        ਜੇ (!m->ਨਿਯਮ_2.skip)
            matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo(m, s, m->val.string);
        ਜਾਓ ਨਿਯਮ_mlvo;
    ਹਾਲਤ TOK_WILD_CARD_ਤਾਰਾ:
        ਜੇ (!m->ਨਿਯਮ_2.skip)
            matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_wildcard(m, s, MLVO_ਮੇਲ_WILDCARD_LEGACY);
        ਜਾਓ ਨਿਯਮ_mlvo;
    ਹਾਲਤ TOK_WILD_CARD_NONE:
        ਜੇ (!m->ਨਿਯਮ_2.skip)
            matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_wildcard(m, s, MLVO_ਮੇਲ_WILDCARD_NONE);
        ਜਾਓ ਨਿਯਮ_mlvo;
    ਹਾਲਤ TOK_WILD_CARD_SOME:
        ਜੇ (!m->ਨਿਯਮ_2.skip)
            matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_wildcard(m, s, MLVO_ਮੇਲ_WILDCARD_SOME);
        ਜਾਓ ਨਿਯਮ_mlvo;
    ਹਾਲਤ TOK_WILD_CARD_ANY:
        ਜੇ (!m->ਨਿਯਮ_2.skip)
            matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_wildcard(m, s, MLVO_ਮੇਲ_WILDCARD_ANY);
        ਜਾਓ ਨਿਯਮ_mlvo;
    ਹਾਲਤ TOK_ਸਮੂਹ_NAME:
        ਜੇ (!m->ਨਿਯਮ_2.skip)
            matcher_ਨਿਯਮ_ਸੈੱਟ_mlvo_ਸਮੂਹ(m, s, m->val.string);
        ਜਾਓ ਨਿਯਮ_mlvo;
    ਹਾਲਤ TOK_EQUALS:
        ਜਾਓ ਨਿਯਮ_kccgst;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

ਨਿਯਮ_kccgst:
    ਚੋਣ (tok = gettok(m, s)) {
    ਹਾਲਤ TOK_IDENTIFIER:
        ਜੇ (!m->ਨਿਯਮ_2.skip)
            matcher_ਨਿਯਮ_ਸੈੱਟ_kccgst(m, s, m->val.string);
        ਜਾਓ ਨਿਯਮ_kccgst;
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ:
        ਜੇ (!m->ਨਿਯਮ_2.skip)
            matcher_ਨਿਯਮ_verify(m, s);
        ਜੇ (!m->ਨਿਯਮ_2.skip)
            matcher_ਨਿਯਮ_apply_if_matches(m, s);
        ਜਾਓ ਨਿਯਮ_mlvo_ਪਹਿਲਾ;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

unexpected:
    ਚੋਣ (tok) {
    ਹਾਲਤ TOK_ERROR:
        ਜਾਓ error;
    ਮੂਲ:
        ਜਾਓ state_error;
    }

ਅੰਤ:
    ਵਾਪਸ true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_ਨਿਯਮਾਂ_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    ਵਾਪਸ false;
}
// NOLINTEND(readability-trivial-switch)

ਸਥਿਰ bool
read_ਨਿਯਮਾਂ_file(ਬਣਤਰ xkb_context *ctx,
                ਬਣਤਰ matcher *matcher,
                ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ,
                FILE *file,
                ਅਚਲ ਅੱਖਰ *ਰਸਤਾ)
{
    bool ret;
    ਅੱਖਰ *string;
    ਮਾਪ_t ਮਾਪ;
    ਬਣਤਰ scanner scanner;

    ਜੇ (!map_file(file, &string, &ਮਾਪ)) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                ਰਸਤਾ, strerror(errno));
        ਵਾਪਸ false;
    }

    scanner_init(&scanner, matcher->ctx, string, ਮਾਪ, ਰਸਤਾ, ਨਿਰਾ);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    ਜੇ (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, ਮਾਪ);
        ਵਾਪਸ false;
    }

    ret = matcher_ਮੇਲ(matcher, &scanner, include_ਡੂੰਘਾਈ, string, ਮਾਪ, ਰਸਤਾ);
    unmap_file(string, ਮਾਪ);
    ਵਾਪਸ ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
ਸਥਿਰ bool
xkb_resolve_partial_ਨਿਯਮਾਂ(ਬਣਤਰ xkb_context *ctx, ਅੱਖਰ *ਰਸਤਾ, ਮਾਪ_t ਰਸਤਾ_ਮਾਪ,
                          ਅਚਲ ਅੱਖਰ* ਨਿਯਮਾਂ, ਅਚਲ ਅੱਖਰ* suffix,
                          ਬਣਤਰ matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    ਅੱਖਰ partial_ਨਿਯਮਾਂ[60]; /* Arbitrary, but we do not expect long names */
    ਜੇ (unlikely(!snprintf_ਸੁਰੱਖਿਅਤ(partial_ਨਿਯਮਾਂ, ਆਕਾਰ(partial_ਨਿਯਮਾਂ),
                                "%s%s", ਨਿਯਮਾਂ, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", ਨਿਯਮਾਂ, suffix);
        ਵਾਪਸ false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = 0;
    FILE *file = ਨਿਰਾ;
    ਅਚਲ ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(partial_ਨਿਯਮਾਂ);
    ਜਦੋਂ ((file = ਲੱਭੋ_File_ਵਿੱਚ_Xkb_ਰਸਤਾ(ctx, "(unknown)",
                                     partial_ਨਿਯਮਾਂ, len, FILE_TYPE_ਨਿਯਮਾਂ,
                                     ਰਸਤਾ, ਰਸਤਾ_ਮਾਪ, &ਵਿੱਥ, false)) != ਨਿਰਾ) {
        ਅਚਲ bool ok = read_ਨਿਯਮਾਂ_file(ctx, matcher, 0, file, ਰਸਤਾ);
        fclose(file);
        ਜੇ (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", ਰਸਤਾ);
            ਵਾਪਸ false;
        }
        ਵਿੱਥ++;
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
xkb_resolve_ਨਿਯਮਾਂ(ਬਣਤਰ xkb_context *ctx,
                  ਅਚਲ ਅੱਖਰ* ਨਿਯਮਾਂ, ਬਣਤਰ matcher *matcher,
                  ਬਣਤਰ xkb_component_ਨਾਂ *ਬਾਹਰਲਾ,
                  xkb_layout_ਸੂਚਕਾਂਕ_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ = 0;
    ਅੱਖਰ ਰਸਤਾ[PATH_MAX];
    FILE * ਅਚਲ file = ਲੱਭੋ_File_ਵਿੱਚ_Xkb_ਰਸਤਾ(ctx, "(unknown)",
                                          ਨਿਯਮਾਂ, ਲੜੀ_ਲੰਬਾਈ(ਨਿਯਮਾਂ), FILE_TYPE_ਨਿਯਮਾਂ,
                                          ਰਸਤਾ, ਆਕਾਰ(ਰਸਤਾ), &ਵਿੱਥ, true);
    ਜੇ (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", ਨਿਯਮਾਂ);
        ਜਾਓ err_ਬਾਹਰਲਾ;
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
    ret = xkb_resolve_partial_ਨਿਯਮਾਂ(ctx, ਰਸਤਾ, ਆਕਾਰ(ਰਸਤਾ),
                                    ਨਿਯਮਾਂ, ".pre", matcher);
    ਜੇ (!ret)
        ਜਾਓ err_ਬਾਹਰਲਾ;

    /* Resolve main <rules> file */
    ret = read_ਨਿਯਮਾਂ_file(ctx, matcher, 0, file, ਰਸਤਾ);
    ਜੇ (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", ਰਸਤਾ);
        ਜਾਓ err_ਬਾਹਰਲਾ;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_ਨਿਯਮਾਂ(ctx, ਰਸਤਾ, ਆਕਾਰ(ਰਸਤਾ),
                                    ਨਿਯਮਾਂ, ".post", matcher);
    ਜੇ (!ret)
        ਜਾਓ err_ਬਾਹਰਲਾ;

    /* Check we got required components */
    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_ਖਾਲੀ_ਥਾਵਾਂ(matcher->kccgst[KCCGST_TYPES]) ||
        darray_ਖਾਲੀ_ਥਾਵਾਂ(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_ਖਾਲੀ_ਥਾਵਾਂ(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", ਨਿਯਮਾਂ);
        ret = false;
        ਜਾਓ err_ਬਾਹਰਲਾ;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &ਬਾਹਰਲਾ->keycodes, ਨਿਰਾ);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &ਬਾਹਰਲਾ->types, ਨਿਰਾ);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &ਬਾਹਰਲਾ->compatibility, ਨਿਰਾ);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &ਬਾਹਰਲਾ->symbols, ਨਿਰਾ);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &ਬਾਹਰਲਾ->geometry, ਨਿਰਾ);

    ਬਣਤਰ matched_sval *mval = &matcher->rmlvo.model;
    ਜੇ (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ਮੁੱਲ,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) mval->sval.len, mval->sval.ਸ਼ੁਰੂ, ਨਿਯਮਾਂ);
    darray_foreach(mval, matcher->rmlvo.layouts)
        ਜੇ (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ਮੁੱਲ,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) mval->sval.len, mval->sval.ਸ਼ੁਰੂ, ਨਿਯਮਾਂ);
    darray_foreach(mval, matcher->rmlvo.variants)
        ਜੇ (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ਮੁੱਲ,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) mval->sval.len, mval->sval.ਸ਼ੁਰੂ, ਨਿਯਮਾਂ);
    darray_foreach(mval, matcher->rmlvo.options) {
        ਜੇ (!mval->sval.len)
            ਜਾਰੀ;

        ਜੇ (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ਮੁੱਲ,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) mval->sval.len, mval->sval.ਸ਼ੁਰੂ, ਨਿਯਮਾਂ);
        } ਨਹੀਂਤਾਂ ਜੇ (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ਅਚਲ xkb_layout_ਨਕਾਬ_t unmatched = (mval->layouts & ~mval->matched);
            ਪ੍ਰਤਿਜ੍ਞਾ(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ਮੁੱਲ,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)mval->sval.len, mval->sval.ਸ਼ੁਰੂ,
                    unmatched, ਨਿਯਮਾਂ);
        }
    }

    /* Set the number of explicit layouts */
    ਜੇ (ਬਾਹਰਲਾ->symbols != ਨਿਰਾ && explicit_layouts != ਨਿਰਾ) {
        *explicit_layouts = 1; /* at least one group */
        ਅਚਲ ਅੱਖਰ *symbols = ਬਾਹਰਲਾ->symbols;
        /* Take the highest modifier */
        ਜਦੋਂ ((symbols = strchr(symbols, ':')) != ਨਿਰਾ && symbols[1] != '\0') {
            xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0;
            ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 = parse_dec_ਮੰਜ਼ਿਲ_uint32_t(++symbols, SIZE_MAX, &ਸਮੂਹ);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            ਜੇ (ਗਿਣਤੀ_2 > 0 && (symbols[ਗਿਣਤੀ_2] == '\0' ||
                is_ਮਿਲਾਓ_ਬਹੁਲਕ_ਅਗੇਤਰ(symbols[ਗਿਣਤੀ_2])) &&
                ਸਮੂਹ > 0 && ਸਮੂਹ <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, ਸਮੂਹ);
                symbols += ਗਿਣਤੀ_2;
            }
        }
    }

err_ਬਾਹਰਲਾ:
    ਜੇ (file)
        fclose(file);

    ਵਾਪਸ ret;
}

bool
xkb_components_from_rmlvo_builder(ਅਚਲ ਬਣਤਰ xkb_rmlvo_builder *rmlvo,
                                  ਬਣਤਰ xkb_component_ਨਾਂ *ਬਾਹਰਲਾ,
                                  xkb_layout_ਸੂਚਕਾਂਕ_t *explicit_layouts)
{
    ਅਚਲ ਅੱਖਰ *ਨਿਯਮਾਂ = rmlvo->ਨਿਯਮਾਂ;
    ਬਣਤਰ matcher *matcher = matcher_new_from_rmlvo(rmlvo, &ਨਿਯਮਾਂ);
    ਜੇ (!matcher)
        ਵਾਪਸ false;

    ਅਚਲ bool ret = xkb_resolve_ਨਿਯਮਾਂ(rmlvo->ctx, ਨਿਯਮਾਂ, matcher,
                                       ਬਾਹਰਲਾ, explicit_layouts);

    matcher_free(matcher);
    ਵਾਪਸ ret;
}


bool
xkb_components_from_ਨਿਯਮਾਂ_ਨਾਂ(ਬਣਤਰ xkb_context *ctx,
                                ਅਚਲ ਬਣਤਰ xkb_ਨਿਯਮ_ਨਾਂ *rmlvo,
                                ਬਣਤਰ xkb_component_ਨਾਂ *ਬਾਹਰਲਾ,
                                xkb_layout_ਸੂਚਕਾਂਕ_t *explicit_layouts)
{
    ਬਣਤਰ matcher *matcher = matcher_new_from_ਨਾਂ(ctx, rmlvo);
    ਜੇ (!matcher)
        ਵਾਪਸ false;

    ਅਚਲ bool ret = xkb_resolve_ਨਿਯਮਾਂ(ctx, rmlvo->ਨਿਯਮਾਂ, matcher,
                                       ਬਾਹਰਲਾ, explicit_layouts);

    matcher_free(matcher);
    ਵਾਪਸ ret;
}
