/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "rules.h"
#ಅನ್ತರ್ಭಾವ "rmlvo.h"
#ಅನ್ತರ್ಭಾವ "include.h"
#ಅನ್ತರ್ಭಾವ "scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-numbers.h"
#ಅನ್ತರ್ಭಾವ "util-paths.h"

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_INCLUDE_ಆಳ 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
ಒಕ್ಕೂಟ lvalue {
    ರಚನೆ sval string;
};

ಎಣಿಕೆ ನಿಯಮಗಳು_token {
    TOK_ಕೊನೆ_ದ_FILE = 0,
    TOK_ಕೊನೆ_ದ_ಸಾಲು,
    TOK_IDENTIFIER,
    TOK_ಗುಂಪು_NAME,
    TOK_BANG,
    TOK_EQUALS,
    TOK_WILD_CARD_ನಕ್ಷತ್ರ,
    TOK_WILD_CARD_NONE,
    TOK_WILD_CARD_SOME,
    TOK_WILD_CARD_ANY,
    TOK_INCLUDE,
    TOK_ERROR
};

ಸ್ಥಿರ ಎಣಿಕೆ ನಿಯಮಗಳು_token
lex(ರಚನೆ scanner *s, ಒಕ್ಕೂಟ lvalue *val)
{
skip_ಇನ್ನಷ್ಟು_whitespace_and_comments:
    /* Skip spaces */
    ಆಗ (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    ಆದರೆ (scanner_lit(s, "//")) {
        scanner_skip_ಗಮ್ಯ_eol(s);
    }

    /* New line */
    ಆದರೆ (scanner_eol(s)) {
        ಆಗ (scanner_eol(s)) scanner_ಮುಂದಿನ(s);
        ಹಿಂತಿರುಗಿ TOK_ಕೊನೆ_ದ_ಸಾಲು;
    }

    /* Escaped line continuation */
    ಆದರೆ (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        ಆದರೆ (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            ಹಿಂತಿರುಗಿ TOK_ERROR;
        }
        scanner_ಮುಂದಿನ(s);
        ಹೋಗು skip_ಇನ್ನಷ್ಟು_whitespace_and_comments;
    }

    /* See if we're done */
    ಆದರೆ (scanner_eof(s)) ಹಿಂತಿರುಗಿ TOK_ಕೊನೆ_ದ_FILE;

    /* New token */
    s->token_pos = s->pos;
    ಎಣಿಕೆ ನಿಯಮಗಳು_token token = TOK_IDENTIFIER;

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಮುಂದಿನ = scanner_ಮುಂದಿನ(s);
    ಆಯ್ಕೆ (ಮುಂದಿನ) {
    ಸ್ಥಿತಿ '!':
        ಹಿಂತಿರುಗಿ TOK_BANG;
    ಸ್ಥಿತಿ '$':
        /*ಗುಂಪು*/
        token = TOK_ಗುಂಪು_NAME;
        val->string.len = 0;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ '*':
        ಆದರೆ (scanner_ನಿಯಮಗಳು_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            ನಿಲ್ಲಿಸು;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Legacy wild card */
            ಹಿಂತಿರುಗಿ TOK_WILD_CARD_ನಕ್ಷತ್ರ;
        }
    ಸ್ಥಿತಿ '<':
        /* Extended wild cards */
        ಆದರೆ (scanner_ನಿಯಮಗಳು_lit_token(s, "none>")) ಹಿಂತಿರುಗಿ TOK_WILD_CARD_NONE;
        ಆದರೆ (scanner_ನಿಯಮಗಳು_lit_token(s, "some>")) ಹಿಂತಿರುಗಿ TOK_WILD_CARD_SOME;
        ಆದರೆ (scanner_ನಿಯಮಗಳು_lit_token(s, "any>")) ಹಿಂತಿರುಗಿ TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        ಪ್ರತಿಜ್ಞಾ(scanner_ನಿಯಮಗಳು_is_ident('<'));
        val->string.len = 1;
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ '=':
        ಆದರೆ (scanner_ನಿಯಮಗಳು_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            ನಿಲ್ಲಿಸು;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಹಿಂತಿರುಗಿ TOK_EQUALS;
        }
    ಸ್ಥಿತಿ 'i':
        ಆದರೆ (scanner_ನಿಯಮಗಳು_lit_token(s, "nclude")) {
            /* Include statement */
            ಹಿಂತಿರುಗಿ TOK_INCLUDE;
        }
        /* Identifier */
        ಪ್ರತಿಜ್ಞಾ(scanner_ನಿಯಮಗಳು_is_ident('i'));
        val->string.len = 1;
        ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (likely(scanner_ನಿಯಮಗಳು_is_ident(ಮುಂದಿನ))) {
            /* Identifier */
            val->string.len = 1;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.ಆರಂಭ = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    ಪ್ರತಿಜ್ಞಾ(scanner_ನಿಯಮಗಳು_is_ident(ವಿಲೀನ_OVERRIDE_ಪೂರ್ವಪ್ರತ್ಯಯ));
    ಪ್ರತಿಜ್ಞಾ(scanner_ನಿಯಮಗಳು_is_ident(ವಿಲೀನ_AUGMENT_ಪೂರ್ವಪ್ರತ್ಯಯ));
    ಪ್ರತಿಜ್ಞಾ(scanner_ನಿಯಮಗಳು_is_ident(ವಿಲೀನ_REPLACE_ಪೂರ್ವಪ್ರತ್ಯಯ));
    ಪ್ರತಿಜ್ಞಾ(scanner_ನಿಯಮಗಳು_is_ident('('));
    ಪ್ರತಿಜ್ಞಾ(scanner_ನಿಯಮಗಳು_is_ident(')'));
    val->string.len += scanner_ನಿಯಮಗಳು_ident_token(s);

    ಆದರೆ (unlikely(val->string.len == 0)) {
        ಆದರೆ (token == TOK_ಗುಂಪು_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } ಇಲ್ಲದಿದ್ದರೆ {
            scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                        "unrecognized token");
        }
        ಹಿಂತಿರುಗಿ TOK_ERROR;
    }

    ಹಿಂತಿರುಗಿ token;
}

/***====================================================================***/

ಎಣಿಕೆ ನಿಯಮಗಳು_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_ಅಂಕೆ_ENTRIES
};

ಎಣಿಕೆ {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint8_t mlvo_ಸೂಚ್ಯಂಕ_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint8_t mlvo_ಮುಖವಾಡ_t;


ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ sval ನಿಯಮಗಳು_mlvo_svals[MLVO_ಅಂಕೆ_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

ಎಣಿಕೆ ನಿಯಮಗಳು_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_ಅಂಕೆ_ENTRIES
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint8_t kccgst_ಸೂಚ್ಯಂಕ_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint8_t kccgst_ಮುಖವಾಡ_t;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ sval ನಿಯಮಗಳು_kccgst_svals[KCCGST_ಅಂಕೆ_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

ಎಣಿಕೆ {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_ಮುಖವಾಡ_t)XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL ==
              (xkb_layout_ಮುಖವಾಡ_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
ರಚನೆ matched_sval {
    ರಚನೆ sval sval;
    /** Used for layout-specific options */
    xkb_layout_ಮುಖವಾಡ_t layouts;
    xkb_layout_ಮುಖವಾಡ_t matched;
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray(ರಚನೆ matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
ರಚನೆ ನಿಯಮ_ಹೆಸರುಗಳು {
    ರಚನೆ matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

ರಚನೆ ಗುಂಪು {
    ರಚನೆ sval name;
    darray_sval elements;
};

ರಚನೆ mapping {
    ಎಣಿಕೆ ನಿಯಮಗಳು_mlvo mlvo_at_pos[MLVO_ಅಂಕೆ_ENTRIES];
    mlvo_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_mlvo;
    mlvo_ಮುಖವಾಡ_t defined_mlvo_ಮುಖವಾಡ;
    bool has_layout_idx_range;
    bool has_ಅಪವರ್ತ್ಯ_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    ಒಕ್ಕೂಟ {
        ರಚನೆ { xkb_layout_ಸೂಚ್ಯಂಕ_t layout_idx, variant_idx; };
        ರಚನೆ { xkb_layout_ಸೂಚ್ಯಂಕ_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    ಒಕ್ಕೂಟ {
        xkb_layout_ಮುಖವಾಡ_t ಸಕ್ರಿಯ;
        xkb_layout_ಮುಖವಾಡ_t layouts_candidates_ಮುಖವಾಡ;
    };
    ಎಣಿಕೆ ನಿಯಮಗಳು_kccgst kccgst_at_pos[KCCGST_ಅಂಕೆ_ENTRIES];
    kccgst_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_kccgst;
    kccgst_ಮುಖವಾಡ_t defined_kccgst_ಮುಖವಾಡ;
};

ಎಣಿಕೆ mlvo_ಹೊಂದಿಕೆ_type {
    /** Match the given plain value */
    MLVO_ಹೊಂದಿಕೆ_ಸಾಮಾನ್ಯ = 0,
    /** Match depending on the value of `wildcard_match_type` */
    MLVO_ಹೊಂದಿಕೆ_WILDCARD_LEGACY,
    /** Match empty value */
    MLVO_ಹೊಂದಿಕೆ_WILDCARD_NONE,
    /** Match non-empty value */
    MLVO_ಹೊಂದಿಕೆ_WILDCARD_SOME,
    /** Match any value, optionally empty */
    MLVO_ಹೊಂದಿಕೆ_WILDCARD_ANY,
    /** Match any entry in a group */
    MLVO_ಹೊಂದಿಕೆ_ಗುಂಪು,
};

ಎಣಿಕೆ wildcard_ಹೊಂದಿಕೆ_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_ಹೊಂದಿಕೆ_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_ಹೊಂದಿಕೆ_ALL,
};

ರಚನೆ ನಿಯಮ_2 {
    ರಚನೆ sval mlvo_ಮೌಲ್ಯ_at_pos[MLVO_ಅಂಕೆ_ENTRIES];
    ಎಣಿಕೆ mlvo_ಹೊಂದಿಕೆ_type ಹೊಂದಿಕೆ_type_at_pos[MLVO_ಅಂಕೆ_ENTRIES];
    ರಚನೆ sval kccgst_ಮೌಲ್ಯ_at_pos[KCCGST_ಅಂಕೆ_ENTRIES];
    mlvo_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_mlvo_ಮೌಲ್ಯಗಳು;
    kccgst_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_kccgst_ಮೌಲ್ಯಗಳು;
    bool skip;
};

#ವ್ಯಾಖ್ಯಾನಿಸು ತುಂಡು_KCCGST_ಬಿಟ್_ಕ್ಷೇತ್ರ_ಗಾತ್ರ 4
static_assert(!((1u << (ತುಂಡು_KCCGST_ಬಿಟ್_ಕ್ಷೇತ್ರ_ಗಾತ್ರ - 1)) &
                (KCCGST_ಅಂಕೆ_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
ರಚನೆ kccgst_ಬಫರ್_ತುಂಡು {
    uint32_t ಉದ್ದಳತೆ:(32 - ತುಂಡು_KCCGST_ಬಿಟ್_ಕ್ಷೇತ್ರ_ಗಾತ್ರ);
    ಎಣಿಕೆ ನಿಯಮಗಳು_kccgst kccgst:ತುಂಡು_KCCGST_ಬಿಟ್_ಕ್ಷೇತ್ರ_ಗಾತ್ರ;
    xkb_layout_ಸೂಚ್ಯಂಕ_t layout;
};

/* Buffer for pending KcCGST values */
ರಚನೆ kccgst_ಬಫರ್ {
    darray_char ಬಫರ್;
    /* Slice corresponding to each value in the buffer */
    darray(ರಚನೆ kccgst_ಬಫರ್_ತುಂಡು) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
ರಚನೆ matcher {
    ರಚನೆ xkb_context *ctx;
    /* Input.*/
    ರಚನೆ ನಿಯಮ_ಹೆಸರುಗಳು rmlvo;
    ಒಕ್ಕೂಟ lvalue val;
    darray(ರಚನೆ ಗುಂಪು) groups;
    /* Current mapping. */
    ರಚನೆ mapping mapping;
    /* Current rule. */
    ರಚನೆ ನಿಯಮ_2 ನಿಯಮ_2;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    ರಚನೆ kccgst_ಬಫರ್ pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_ಅಂಕೆ_ENTRIES];
};

ಸ್ಥಿರ ರಚನೆ sval
strip_spaces(ರಚನೆ sval v)
{
    ಆಗ (v.len > 0 && is_space(v.ಆರಂಭ[0])) { v.len--; v.ಆರಂಭ++; }
    ಆಗ (v.len > 0 && is_space(v.ಆರಂಭ[v.len - 1])) v.len--;
    ಹಿಂತಿರುಗಿ v;
}

ಸ್ಥಿರ darray_matched_sval
ವಿಭಜಿಸು_comma_separated_mlvo(ರಚನೆ xkb_context *ctx,
                           ಎಣಿಕೆ ನಿಯಮಗಳು_mlvo mlvo, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    ಆದರೆ (!s) {
        ensure_at_ಕನಿಷ್ಠ_one_ಮೌಲ್ಯ:
        /* Label followed by a declaration is a C23 extension */ ;

        ರಚನೆ matched_sval val = {
            .sval = SVAL(ಖಾಲಿ, 0),
            .layouts = (xkb_layout_ಮುಖವಾಡ_t)XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        ಹಿಂತಿರುಗಿ arr;
    }

    ಆಗ (true) {
        ರಚನೆ matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_ಮುಖವಾಡ_t)XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL,
            .matched = 0,
        };
        ಆಗ (*s != '\0' && *s != ',' && *s != OPTIONS_ಗುಂಪು_SPECIFIER_ಪೂರ್ವಪ್ರತ್ಯಯ) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        ಆದರೆ (*s == OPTIONS_ಗುಂಪು_SPECIFIER_ಪೂರ್ವಪ್ರತ್ಯಯ) {
            /* Handle numeric layout index */
            s++;
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ layout_ಆರಂಭ = s;
            xkb_layout_ಸೂಚ್ಯಂಕ_t layout = XKB_LAYOUT_INVALID;

            ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = parse_dec_ಗಮ್ಯ_uint32_t(s, SIZE_MAX, &layout);
            ಆದರೆ (ಎಣಿಕೆ_2 > 0) {
                /* Note: 1-indexed layout */
                s += ಎಣಿಕೆ_2;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                ಆದರೆ (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) val.sval.len, val.sval.ಆರಂಭ);
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) val.sval.len, val.sval.ಆರಂಭ);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ layout_ಸೂಚ್ಯಂಕ_ಕೊನೆ = s;
            ಆಗ (*s != '\0' && *s != ',') { s++; }
            ಆದರೆ (ಎಣಿಕೆ_2 <= 0 || layout_ಸೂಚ್ಯಂಕ_ಕೊನೆ != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) (s - layout_ಆರಂಭ), layout_ಆರಂಭ,
                        (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) val.sval.len, val.sval.ಆರಂಭ);
            }
        }

        ಆದರೆ (val.sval.len || mlvo != MLVO_OPTION) {
            ಆದರೆ (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                ರಚನೆ matched_sval *prev;
                darray_foreach(prev, arr) {
                    ಆದರೆ (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        ಆದರೆ (val.layouts ==
                            (xkb_layout_ಮುಖವಾಡ_t)XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (prev->layouts != (xkb_layout_ಮುಖವಾಡ_t)
                                   XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        ಹೋಗು ಮುಂದಿನ_ಮೌಲ್ಯ;
                    }
                }
            }
            darray_append(arr, val);
        }

ಮುಂದಿನ_ಮೌಲ್ಯ:
        ಆದರೆ (*s == '\0') ನಿಲ್ಲಿಸು;
        ಆದರೆ (*s == ',') s++;
    }

    ಆದರೆ (darray_ಖಾಲಿಗಳು(arr))
        ಹೋಗು ensure_at_ಕನಿಷ್ಠ_one_ಮೌಲ್ಯ;

    ಹಿಂತಿರುಗಿ arr;
}

ಸ್ಥಿರ ರಚನೆ matcher *
matcher_new_from_rmlvo(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder *rmlvo, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ನಿಯಮಗಳು)
{
    ರಚನೆ matcher *m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*m));
    ಆದರೆ (!m)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು ಹೆಸರುಗಳು = {
        .ನಿಯಮಗಳು = rmlvo->ನಿಯಮಗಳು,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_ಖಾಲಿಗಳು(rmlvo->layouts)) ? ಖಾಲಿ : "x",
        .variant = (darray_ಖಾಲಿಗಳು(rmlvo->layouts)) ? ಖಾಲಿ : "x",
        .options = (darray_ಖಾಲಿಗಳು(rmlvo->options)) ? ಖಾಲಿ : "x",
    };
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ RMLVO ಬದಲಾಯಿತು = xkb_context_sanitize_ನಿಯಮ_ಹೆಸರುಗಳು(rmlvo->ctx,
                                                               &ಹೆಸರುಗಳು);

    /* Initialize matcher with sanitized names, if relevant */
    ಆದರೆ (ಬದಲಾಯಿತು & RMLVO_ನಿಯಮಗಳು) {
        *ನಿಯಮಗಳು = ಹೆಸರುಗಳು.ನಿಯಮಗಳು;
    } ಇಲ್ಲದಿದ್ದರೆ {
        *ನಿಯಮಗಳು = rmlvo->ನಿಯಮಗಳು;
    }

    ಆದರೆ (ಬದಲಾಯಿತು & RMLVO_MODEL) {
        m->rmlvo.model.sval.ಆರಂಭ = ಹೆಸರುಗಳು.model;
    } ಇಲ್ಲದಿದ್ದರೆ {
        m->rmlvo.model.sval.ಆರಂಭ = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_ಸುರಕ್ಷಿತ(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_ಮುಖವಾಡ_t)XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL;

    ಪ್ರತಿಜ್ಞಾ((ಬದಲಾಯಿತು & RMLVO_LAYOUT) || !(ಬದಲಾಯಿತು & RMLVO_VARIANT));
    ಆದರೆ (ಬದಲಾಯಿತು & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = ವಿಭಜಿಸು_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      ಹೆಸರುಗಳು.layout);
        m->rmlvo.variants = ವಿಭಜಿಸು_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       ಹೆಸರುಗಳು.variant);
        ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.layouts) > darray_ಗಾತ್ರ(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            ಆದರೆ (!isempty(ಹೆಸರುಗಳು.variant))
                log_warn(m->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         ಹೆಸರುಗಳು.layout ? ಹೆಸರುಗಳು.layout : "(none)",
                         ಹೆಸರುಗಳು.variant ? ಹೆಸರುಗಳು.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_ಗಾತ್ರ(m->rmlvo.layouts));
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.layouts) < darray_ಗಾತ್ರ(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    ಹೆಸರುಗಳು.layout ? ಹೆಸರುಗಳು.layout : "(none)",
                    ಹೆಸರುಗಳು.variant ? ಹೆಸರುಗಳು.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_ಗಾತ್ರ(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        ರಚನೆ xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            ರಚನೆ matched_sval val = {
                .sval = SVAL(layout->layout, strlen_ಸುರಕ್ಷಿತ(layout->layout)),
                .layouts = (xkb_layout_ಮುಖವಾಡ_t)XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL,
                .matched = 0
            };
            darray_append(m->rmlvo.layouts, val);
            val.sval.ಆರಂಭ = layout->variant;
            val.sval.len = strlen_ಸುರಕ್ಷಿತ(layout->variant);
            darray_append(m->rmlvo.variants, val);
        }
    }

    ಆದರೆ (ಬದಲಾಯಿತು & RMLVO_OPTIONS) {
        m->rmlvo.options = ವಿಭಜಿಸು_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      ಹೆಸರುಗಳು.options);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ರಚನೆ xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            ರಚನೆ matched_sval val = {
                .sval = SVAL(option->option, strlen_ಸುರಕ್ಷಿತ(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    ಹಿಂತಿರುಗಿ m;
}

ಸ್ಥಿರ ರಚನೆ matcher *
matcher_new_from_ಹೆಸರುಗಳು(ರಚನೆ xkb_context *ctx,
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo)
{
    ರಚನೆ matcher *m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(*m));
    ಆದರೆ (!m)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    m->ctx = ctx;
    m->rmlvo.model.sval.ಆರಂಭ = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_ಸುರಕ್ಷಿತ(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_ಮುಖವಾಡ_t)XKB_OPTION_LAYOUT_ಮುಖವಾಡ_GLOBAL;
    m->rmlvo.layouts = ವಿಭಜಿಸು_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = ವಿಭಜಿಸು_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = ವಿಭಜಿಸು_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.layouts) > darray_ಗಾತ್ರ(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        ಆದರೆ (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_ಗಾತ್ರ(m->rmlvo.layouts));
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.layouts) < darray_ಗಾತ್ರ(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_ಗಾತ್ರ(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    ಹಿಂತಿರುಗಿ m;
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_free(ರಚನೆ matcher *m)
{
    ಆದರೆ (!m)
        ಹಿಂತಿರುಗಿ;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    ರಚನೆ ಗುಂಪು *ಗುಂಪು;
    darray_foreach(ಗುಂಪು, m->groups)
        darray_free(ಗುಂಪು->elements);
    darray_free(m->pending_kccgst.ಬಫರ್);
    darray_free(m->pending_kccgst.slices);
    ಗಾಗಿ (kccgst_ಸೂಚ್ಯಂಕ_t i = 0; i < (kccgst_ಸೂಚ್ಯಂಕ_t) KCCGST_ಅಂಕೆ_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    ಬಿಡುಗಡೆ(m);
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_ಗುಂಪು_ಆರಂಭ_new(ರಚನೆ matcher *m, ರಚನೆ sval name)
{
    ರಚನೆ ಗುಂಪು ಗುಂಪು = { .name = name, .elements = darray_new() };
    darray_append(m->groups, ಗುಂಪು);
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_ಗುಂಪು_ಕೂಡಿಸು_element(ರಚನೆ matcher *m, ರಚನೆ scanner *s,
                          ರಚನೆ sval element)
{
    darray_append(darray_ಅಂಶ(m->groups, darray_ಗಾತ್ರ(m->groups) - 1).elements,
                  element);
}

ಸ್ಥಿರ bool
read_ನಿಯಮಗಳು_file(ರಚನೆ xkb_context *ctx,
                ರಚನೆ matcher *matcher,
                ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ,
                FILE *file,
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ);

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_include(ರಚನೆ matcher *m, ರಚನೆ scanner *ಪೋಷಕ_scanner,
                ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ,
                ರಚನೆ sval inc)
{
    ಆದರೆ (include_ಆಳ >= MAX_INCLUDE_ಆಳ) {
        scanner_err(ಪೋಷಕ_scanner, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_ಆಳ);
        ಹಿಂತಿರುಗಿ;
    }

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *stmt_file = inc.ಆರಂಭ;
    ಗಾತ್ರ_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    ಅಕ್ಷರ buf[PATH_MAX] = {0};
    ಸ್ಥಿರಾಂಕ ssize_t expanded = expand_ಮಾರ್ಗ(m->ctx, ಪೋಷಕ_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_ನಿಯಮಗಳು,
                                         buf, ಗಾತ್ರ(buf));
    ಆದರೆ (expanded < 0) {
        /* Error */
        ಹಿಂತಿರುಗಿ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (ಗಾತ್ರ_t) expanded;
        ಪ್ರತಿಜ್ಞಾ(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 0;
    ಸ್ಥಿರಾಂಕ bool absolute_ಮಾರ್ಗ = is_absolute_ಮಾರ್ಗ(stmt_file);
    ಆದರೆ (absolute_ಮಾರ್ಗ) {
        /* Absolute path: no need for lookup in XKB paths */
        ಆದರೆ (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            ಆದರೆ (stmt_file_len < ಗಾತ್ರ(buf)) {
                ಸ್ಮರಣೆ_ನಕಲು(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } ಇಲ್ಲದಿದ್ದರೆ {
                log_err(m->ctx, XKB_ERROR_INVALID_ಮಾರ್ಗ,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, ಗಾತ್ರ(buf),
                        (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) stmt_file_len, stmt_file);
                ಹಿಂತಿರುಗಿ;
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* %-expansion is always NULL-terminated */
            ಪ್ರತಿಜ್ಞಾ(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Relative path: lookup the first XKB path */
        ಆದರೆ (unlikely(expanded)) {
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
            file = ಖಾಲಿ;
        } ಇಲ್ಲದಿದ್ದರೆ {
            file = ಕಂಡುಹಿಡಿ_File_ಒಳ_Xkb_ಮಾರ್ಗ(m->ctx, ಪೋಷಕ_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_ನಿಯಮಗಳು,
                                     buf, ಗಾತ್ರ(buf), &ಸ್ಥಾನಾಂತರ, true);
        }
    }

    ಆಗ (file) {
        ಪ್ರತಿಜ್ಞಾ(strlen_ಸುರಕ್ಷಿತ(buf) < ಗಾತ್ರ(buf));
        bool ret = read_ನಿಯಮಗಳು_file(m->ctx, m, include_ಆಳ + 1, file, buf);
        fclose(file);
        ಆದರೆ (ret)
            ಹಿಂತಿರುಗಿ;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        ಆದರೆ (absolute_ಮಾರ್ಗ) {
            /* There is no point to search further if the path is absolute */
            ನಿಲ್ಲಿಸು;
        }

        /* Try next XKB path */
        ಸ್ಥಾನಾಂತರ++;
        file = ಕಂಡುಹಿಡಿ_File_ಒಳ_Xkb_ಮಾರ್ಗ(m->ctx, ಪೋಷಕ_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_ನಿಯಮಗಳು,
                                 buf, ಗಾತ್ರ(buf), &ಸ್ಥಾನಾಂತರ, true);
    }

    log_err(m->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) stmt_file_len, stmt_file);
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_mapping_ಆರಂಭ_new(ರಚನೆ matcher *m)
{
    ಗಾಗಿ (mlvo_ಸೂಚ್ಯಂಕ_t i = 0; i < (mlvo_ಸೂಚ್ಯಂಕ_t) MLVO_ಅಂಕೆ_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_ಅಂಕೆ_ENTRIES;
    ಗಾಗಿ (kccgst_ಸೂಚ್ಯಂಕ_t i = 0; i < (kccgst_ಸೂಚ್ಯಂಕ_t) KCCGST_ಅಂಕೆ_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_ಅಂಕೆ_ENTRIES;
    m->mapping.has_ಅಪವರ್ತ್ಯ_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.ಅಂಕೆ_mlvo = m->mapping.ಅಂಕೆ_kccgst = 0;
    m->mapping.defined_mlvo_ಮುಖವಾಡ = 0;
    m->mapping.defined_kccgst_ಮುಖವಾಡ = 0;
    m->mapping.ಸಕ್ರಿಯ = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
parse_layout_int_ಸೂಚ್ಯಂಕ_ಬಾಲ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಗಾತ್ರ_t max_len,
                            xkb_layout_ಸೂಚ್ಯಂಕ_t *ಹೊರಹೋಗು)
{
    ಆದರೆ (max_len < 3)
        ಹಿಂತಿರುಗಿ -1;
    uint32_t val = 0;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = parse_dec_ಗಮ್ಯ_uint32_t(&s[1], max_len - 2, &val);
    ಆದರೆ (ಎಣಿಕೆ_2 <= 0 || s[1 + ಎಣಿಕೆ_2] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        ಹಿಂತಿರುಗಿ -1;
    /* To zero-based index. */
    *ಹೊರಹೋಗು = val - 1;
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2 + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಹೊರತೆಗೆ_layout_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಗಾತ್ರ_t max_len, xkb_layout_ಸೂಚ್ಯಂಕ_t *ಹೊರಹೋಗು)
{
    /* This function is pretty stupid, but works for now. */
    *ಹೊರಹೋಗು = XKB_LAYOUT_INVALID;
    ಆದರೆ (max_len < 3 || s[0] != '[')
        ಹಿಂತಿರುಗಿ -1;
    ಆದರೆ (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *ಹೊರಹೋಗು = (xkb_layout_ಸೂಚ್ಯಂಕ_t)(s[1] - '1');
        ಹಿಂತಿರುಗಿ 3;
    }
    ಆದರೆ (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        ಹಿಂತಿರುಗಿ 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    ಹಿಂತಿರುಗಿ parse_layout_int_ಸೂಚ್ಯಂಕ_ಬಾಲ(s, max_len, ಹೊರಹೋಗು);
}

/* Special layout indices */
ಎಣಿಕೆ layout_ಸೂಚ್ಯಂಕ_ranges {
    LAYOUT_ಸೂಚ್ಯಂಕ_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_ಸೂಚ್ಯಂಕ_ಮೊದಲ,
    LAYOUT_ಸೂಚ್ಯಂಕ_LATER,
    LAYOUT_ಸೂಚ್ಯಂಕ_ಅಪವರ್ತ್ಯ,
    LAYOUT_ಸೂಚ್ಯಂಕ_ANY,
};

static_assert((xkb_layout_ಸೂಚ್ಯಂಕ_t) XKB_MAX_GROUPS <
              (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_SINGLE,
              "Cannot define special indices");
static_assert((xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_SINGLE <
              (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_ಮೊದಲ &&
              (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_ಮೊದಲ <
              (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_LATER &&
              (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_LATER <
              (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_ಅಪವರ್ತ್ಯ &&
              (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_ಅಪವರ್ತ್ಯ <
              (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_ANY &&
              (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_ANY <
              (xkb_layout_ಸೂಚ್ಯಂಕ_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
ಸ್ಥಿರ bool
ಹೊರತೆಗೆ_mapping_layout_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, ಗಾತ್ರ_t max_len,
                             xkb_layout_ಸೂಚ್ಯಂಕ_t *ಹೊರಹೋಗು)
{
    /* Check for minimal `[` + index + `]` */
    ಆದರೆ (max_len < 3 || s[0] != '[') {
        *ಹೊರಹೋಗು = XKB_LAYOUT_INVALID;
        ಹಿಂತಿರುಗಿ false;
    }

    /* Small numeric index */
    ಆದರೆ (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *ಹೊರಹೋಗು = (xkb_layout_ಸೂಚ್ಯಂಕ_t)(s[1] - '1');
        ಹಿಂತಿರುಗಿ true;
    }

    /* Named indices ranges */
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name;
        ಗಾತ್ರ_t ಗಾತ್ರ_2;
        xkb_layout_ಸೂಚ್ಯಂಕ_t ಮೌಲ್ಯ;
    } ಹೆಸರುಗಳು[] = {
        #ವ್ಯಾಖ್ಯಾನಿಸು ಸೂಚ್ಯಂಕ_RANGE(ಬಾಲ, ಮೌಲ್ಯ) \
            { (ಬಾಲ), ಗಾತ್ರ(ಬಾಲ), (xkb_layout_ಸೂಚ್ಯಂಕ_t)(ಮೌಲ್ಯ) }
        ಸೂಚ್ಯಂಕ_RANGE("single]",   LAYOUT_ಸೂಚ್ಯಂಕ_SINGLE),
        ಸೂಚ್ಯಂಕ_RANGE("first]",    LAYOUT_ಸೂಚ್ಯಂಕ_ಮೊದಲ),
        ಸೂಚ್ಯಂಕ_RANGE("later]",    LAYOUT_ಸೂಚ್ಯಂಕ_LATER),
        ಸೂಚ್ಯಂಕ_RANGE("multiple]", LAYOUT_ಸೂಚ್ಯಂಕ_ಅಪವರ್ತ್ಯ),
        ಸೂಚ್ಯಂಕ_RANGE("any]",      LAYOUT_ಸೂಚ್ಯಂಕ_ANY),
        #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಸೂಚ್ಯಂಕ_RANGE
    };

    ಗಾಗಿ (ಗಾತ್ರ_t k = 0; k < ARRAY_ಗಾತ್ರ(ಹೆಸರುಗಳು); k++) {
        ಆದರೆ (max_len == ಹೆಸರುಗಳು[k].ಗಾತ್ರ_2 &&
            memcmp(&s[1], ಹೆಸರುಗಳು[k].name, ಹೆಸರುಗಳು[k].ಗಾತ್ರ_2 - 1) == 0) {
            *ಹೊರಹೋಗು = ಹೆಸರುಗಳು[k].ಮೌಲ್ಯ;
            ಹಿಂತಿರುಗಿ true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *ಹೊರಹೋಗು = XKB_LAYOUT_INVALID;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = parse_layout_int_ಸೂಚ್ಯಂಕ_ಬಾಲ(s, max_len, ಹೊರಹೋಗು);
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2 > 0 && (ಗಾತ್ರ_t)ಎಣಿಕೆ_2 == max_len;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
is_mlvo_ಮುಖವಾಡ_defined(ರಚನೆ matcher *m, ಎಣಿಕೆ ನಿಯಮಗಳು_mlvo mlvo)
{
    ಹಿಂತಿರುಗಿ m->mapping.defined_mlvo_ಮುಖವಾಡ & (1u << mlvo);
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_mapping_ಗಣ_mlvo(ರಚನೆ matcher *m, ರಚನೆ scanner *s,
                         ರಚನೆ sval ident)
{
    ಎಣಿಕೆ ನಿಯಮಗಳು_mlvo mlvo;
    ರಚನೆ sval mlvo_sval;

    ಗಾಗಿ (mlvo = 0; mlvo < MLVO_ಅಂಕೆ_ENTRIES; mlvo++) {
        mlvo_sval = ನಿಯಮಗಳು_mlvo_svals[mlvo];

        ಆದರೆ (svaleq_ಪೂರ್ವಪ್ರತ್ಯಯ(mlvo_sval, ident))
            ನಿಲ್ಲಿಸು;
    }

    /* Not found. */
    ಆದರೆ (mlvo >= MLVO_ಅಂಕೆ_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) ident.len, ident.ಆರಂಭ);
        m->mapping.ಸಕ್ರಿಯ = false;
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (is_mlvo_ಮುಖವಾಡ_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) mlvo_sval.len, mlvo_sval.ಆರಂಭ);
        m->mapping.ಸಕ್ರಿಯ = false;
        ಹಿಂತಿರುಗಿ;
    }

    /* If there are leftovers still, it must be an index. */
    ಆದರೆ (mlvo_sval.len < ident.len) {
        xkb_layout_ಸೂಚ್ಯಂಕ_t idx;
        ಆದರೆ (!ಹೊರತೆಗೆ_mapping_layout_ಸೂಚ್ಯಂಕ(ident.ಆರಂಭ + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) mlvo_sval.len, mlvo_sval.ಆರಂಭ);
            m->mapping.ಸಕ್ರಿಯ = false;
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) mlvo_sval.len, mlvo_sval.ಆರಂಭ);
            m->mapping.ಸಕ್ರಿಯ = false;
            ಹಿಂತಿರುಗಿ;
        }
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_SINGLE;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_ಸೂಚ್ಯಂಕ_t) LAYOUT_ಸೂಚ್ಯಂಕ_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    ಆದರೆ (((mlvo == MLVO_LAYOUT && is_mlvo_ಮುಖವಾಡ_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_ಮುಖವಾಡ_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.ಸಕ್ರಿಯ = false;
        ಹಿಂತಿರುಗಿ;
    }

    m->mapping.mlvo_at_pos[m->mapping.ಅಂಕೆ_mlvo] = mlvo;
    m->mapping.defined_mlvo_ಮುಖವಾಡ |= (mlvo_ಮುಖವಾಡ_t) 1u << mlvo;
    m->mapping.ಅಂಕೆ_mlvo++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_mapping_ಗಣ_layout_bounds(ರಚನೆ matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_ಸೂಚ್ಯಂಕ_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    ಆಯ್ಕೆ (idx) {
        ಸ್ಥಿತಿ XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            ಪ್ರತಿಜ್ಞಾ(!is_mlvo_ಮುಖವಾಡ_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_ಮುಖವಾಡ_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_ಮುಖವಾಡ = 0x1; /* active = true */
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_ಗಾತ್ರ(m->rmlvo.layouts));
            m->mapping.layouts_candidates_ಮುಖವಾಡ = (xkb_layout_ಮುಖವಾಡ_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_ಅಪವರ್ತ್ಯ:
        ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_ಗಾತ್ರ(m->rmlvo.layouts));
            m->mapping.layouts_candidates_ಮುಖವಾಡ = (xkb_layout_ಮುಖವಾಡ_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_SINGLE:
        ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_ಮೊದಲ:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        ಪೂರ್ವನಿಯೋಜಿತ:
            /* Mere layout index */
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = idx;
            m->mapping.layout_idx_max = idx + 1;
            m->mapping.layouts_candidates_ಮುಖವಾಡ = UINT32_C(1) << idx;
    }
    m->mapping.has_ಅಪವರ್ತ್ಯ_layouts = (
        m->mapping.layout_idx_max > m->mapping.layout_idx_min &&
        m->mapping.layout_idx_max - m->mapping.layout_idx_min > 1
    );
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_mapping_ಗಣ_kccgst(ರಚನೆ matcher *m, ರಚನೆ scanner *s, ರಚನೆ sval ident)
{
    ಎಣಿಕೆ ನಿಯಮಗಳು_kccgst kccgst;
    ರಚನೆ sval kccgst_sval;

    ಗಾಗಿ (kccgst = 0; kccgst < KCCGST_ಅಂಕೆ_ENTRIES; kccgst++) {
        kccgst_sval = ನಿಯಮಗಳು_kccgst_svals[kccgst];

        ಆದರೆ (svaleq(ನಿಯಮಗಳು_kccgst_svals[kccgst], ident))
            ನಿಲ್ಲಿಸು;
    }

    /* Not found. */
    ಆದರೆ (kccgst >= KCCGST_ಅಂಕೆ_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) ident.len, ident.ಆರಂಭ);
        m->mapping.ಸಕ್ರಿಯ = false;
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (m->mapping.defined_kccgst_ಮುಖವಾಡ & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) kccgst_sval.len, kccgst_sval.ಆರಂಭ);
        m->mapping.ಸಕ್ರಿಯ = false;
        ಹಿಂತಿರುಗಿ;
    }

    m->mapping.kccgst_at_pos[m->mapping.ಅಂಕೆ_kccgst] = kccgst;
    m->mapping.defined_kccgst_ಮುಖವಾಡ |= (kccgst_ಮುಖವಾಡ_t) 1u << kccgst;
    m->mapping.ಅಂಕೆ_kccgst++;
}

ಸ್ಥಿರ bool
matcher_mapping_verify(ರಚನೆ matcher *m, ರಚನೆ scanner *s)
{
    ಆದರೆ (m->mapping.ಅಂಕೆ_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        ಹೋಗು skip;
    }

    ಆದರೆ (m->mapping.ಅಂಕೆ_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        ಹೋಗು skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    ಆದರೆ (is_mlvo_ಮುಖವಾಡ_defined(m, MLVO_LAYOUT)) {
        ಪ್ರತಿಜ್ಞಾ(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        ಆಯ್ಕೆ (m->mapping.layout_idx) {
            ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.layouts) > 1)
                    ಹೋಗು skip;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_ಅಪವರ್ತ್ಯ:
            ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.layouts) < 2)
                    ಹೋಗು skip;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_ANY:
            ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_ಮೊದಲ:
                /* No restrictions */
                ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_ಗಾತ್ರ(m->rmlvo.layouts))
                    ಹೋಗು skip;
        }
    }

    ಆದರೆ (is_mlvo_ಮುಖವಾಡ_defined(m, MLVO_VARIANT)) {
        ಪ್ರತಿಜ್ಞಾ(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        ಆಯ್ಕೆ (m->mapping.variant_idx) {
            ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.variants) > 1)
                    ಹೋಗು skip;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_ANY:
            ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_LATER:
            ಸ್ಥಿತಿ LAYOUT_ಸೂಚ್ಯಂಕ_ಮೊದಲ:
                /* No restriction */
                ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_ಗಾತ್ರ(m->rmlvo.variants))
                    ಹೋಗು skip;
        }
    }

    ಹಿಂತಿರುಗಿ true;

skip:
    m->mapping.ಸಕ್ರಿಯ = false;
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_ನಿಯಮ_ಆರಂಭ_new(ರಚನೆ matcher *m)
{
    memset(&m->ನಿಯಮ_2, 0, ಗಾತ್ರ(m->ನಿಯಮ_2));
    m->ನಿಯಮ_2.skip = !m->mapping.ಸಕ್ರಿಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_ನಿಯಮ_ಗಣ_mlvo_common(ರಚನೆ matcher *m, ರಚನೆ scanner *s,
                             ರಚನೆ sval ident,
                             ಎಣಿಕೆ mlvo_ಹೊಂದಿಕೆ_type ಹೊಂದಿಕೆ_type)
{
    ಆದರೆ (m->ನಿಯಮ_2.ಅಂಕೆ_mlvo_ಮೌಲ್ಯಗಳು >= m->mapping.ಅಂಕೆ_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->ನಿಯಮ_2.skip = true;
        ಹಿಂತಿರುಗಿ;
    }
    m->ನಿಯಮ_2.ಹೊಂದಿಕೆ_type_at_pos[m->ನಿಯಮ_2.ಅಂಕೆ_mlvo_ಮೌಲ್ಯಗಳು] = ಹೊಂದಿಕೆ_type;
    m->ನಿಯಮ_2.mlvo_ಮೌಲ್ಯ_at_pos[m->ನಿಯಮ_2.ಅಂಕೆ_mlvo_ಮೌಲ್ಯಗಳು] = ident;
    m->ನಿಯಮ_2.ಅಂಕೆ_mlvo_ಮೌಲ್ಯಗಳು++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_ನಿಯಮ_ಗಣ_mlvo_wildcard(ರಚನೆ matcher *m, ರಚನೆ scanner *s,
                               ಎಣಿಕೆ mlvo_ಹೊಂದಿಕೆ_type ಹೊಂದಿಕೆ_type)
{
    ರಚನೆ sval dummy = SVAL(ಖಾಲಿ, 0);
    matcher_ನಿಯಮ_ಗಣ_mlvo_common(m, s, dummy, ಹೊಂದಿಕೆ_type);
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_ನಿಯಮ_ಗಣ_mlvo_ಗುಂಪು(ರಚನೆ matcher *m, ರಚನೆ scanner *s,
                            ರಚನೆ sval ident)
{
    matcher_ನಿಯಮ_ಗಣ_mlvo_common(m, s, ident, MLVO_ಹೊಂದಿಕೆ_ಗುಂಪು);
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_ನಿಯಮ_ಗಣ_mlvo(ರಚನೆ matcher *m, ರಚನೆ scanner *s,
                      ರಚನೆ sval ident)
{
    matcher_ನಿಯಮ_ಗಣ_mlvo_common(m, s, ident, MLVO_ಹೊಂದಿಕೆ_ಸಾಮಾನ್ಯ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_ನಿಯಮ_ಗಣ_kccgst(ರಚನೆ matcher *m, ರಚನೆ scanner *s,
                        ರಚನೆ sval ident)
{
    ಆದರೆ (m->ನಿಯಮ_2.ಅಂಕೆ_kccgst_ಮೌಲ್ಯಗಳು >= m->mapping.ಅಂಕೆ_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->ನಿಯಮ_2.skip = true;
        ಹಿಂತಿರುಗಿ;
    }
    m->ನಿಯಮ_2.kccgst_ಮೌಲ್ಯ_at_pos[m->ನಿಯಮ_2.ಅಂಕೆ_kccgst_ಮೌಲ್ಯಗಳು] = ident;
    m->ನಿಯಮ_2.ಅಂಕೆ_kccgst_ಮೌಲ್ಯಗಳು++;
}

ಸ್ಥಿರ bool
ಹೊಂದಿಕೆ_ಗುಂಪು(ರಚನೆ matcher *m, ರಚನೆ sval ಗುಂಪು_name, ರಚನೆ sval ಗಮ್ಯ)
{
    ರಚನೆ ಗುಂಪು *ಗುಂಪು = ಖಾಲಿ;
    ರಚನೆ sval *element;
    bool ಸಿಕ್ಕಿತು = false;

    darray_foreach(ಗುಂಪು, m->groups) {
        ಆದರೆ (svaleq(ಗುಂಪು->name, ಗುಂಪು_name)) {
            ಸಿಕ್ಕಿತು = true;
            ನಿಲ್ಲಿಸು;
        }
    }

    ಆದರೆ (!ಸಿಕ್ಕಿತು) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        ಹಿಂತಿರುಗಿ false;
    }

    darray_foreach(element, ಗುಂಪು->elements)
        ಆದರೆ (svaleq(ಗಮ್ಯ, *element))
            ಹಿಂತಿರುಗಿ true;

    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
ಹೊಂದಿಕೆ_ಮೌಲ್ಯ(ರಚನೆ matcher *m, ಸ್ಥಿರಾಂಕ ರಚನೆ sval val, ಸ್ಥಿರಾಂಕ ರಚನೆ sval ಗಮ್ಯ,
            ಎಣಿಕೆ mlvo_ಹೊಂದಿಕೆ_type ಹೊಂದಿಕೆ_type,
            ಎಣಿಕೆ wildcard_ಹೊಂದಿಕೆ_type wildcard_type)
{
    ಆಯ್ಕೆ (ಹೊಂದಿಕೆ_type) {
        ಸ್ಥಿತಿ MLVO_ಹೊಂದಿಕೆ_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            ಹಿಂತಿರುಗಿ wildcard_type == WILDCARD_ಹೊಂದಿಕೆ_ALL || !!ಗಮ್ಯ.len;
        ಸ್ಥಿತಿ MLVO_ಹೊಂದಿಕೆ_WILDCARD_NONE:
            ಹಿಂತಿರುಗಿ !ಗಮ್ಯ.len;
        ಸ್ಥಿತಿ MLVO_ಹೊಂದಿಕೆ_WILDCARD_SOME:
            ಹಿಂತಿರುಗಿ !!ಗಮ್ಯ.len;
        ಸ್ಥಿತಿ MLVO_ಹೊಂದಿಕೆ_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            ಹಿಂತಿರುಗಿ true;
        ಸ್ಥಿತಿ MLVO_ಹೊಂದಿಕೆ_ಗುಂಪು:
            ಹಿಂತಿರುಗಿ ಹೊಂದಿಕೆ_ಗುಂಪು(m, val, ಗಮ್ಯ);
        ಪೂರ್ವನಿಯೋಜಿತ:
            ಪ್ರತಿಜ್ಞಾ(ಹೊಂದಿಕೆ_type == MLVO_ಹೊಂದಿಕೆ_ಸಾಮಾನ್ಯ);
            ಹಿಂತಿರುಗಿ svaleq(val, ಗಮ್ಯ);
    }
}

ಸ್ಥಿರ bool
ಹೊಂದಿಕೆ_ಮೌಲ್ಯ_and_mark(ರಚನೆ matcher *m, ಸ್ಥಿರಾಂಕ ರಚನೆ sval val,
                     ರಚನೆ matched_sval *ಗಮ್ಯ, ಎಣಿಕೆ mlvo_ಹೊಂದಿಕೆ_type ಹೊಂದಿಕೆ_type,
                     ಎಣಿಕೆ wildcard_ಹೊಂದಿಕೆ_type wildcard_type,
                     xkb_layout_ಮುಖವಾಡ_t layouts)
{
    bool matched = ಹೊಂದಿಕೆ_ಮೌಲ್ಯ(m, val, ಗಮ್ಯ->sval, ಹೊಂದಿಕೆ_type, wildcard_type);
    ಆದರೆ (matched)
        ಗಮ್ಯ->matched |= layouts;
    ಹಿಂತಿರುಗಿ matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
ಸ್ಥಿರ bool
expand_rmlvo_ಒಳ_kccgst_ಮೌಲ್ಯ(ರಚನೆ matcher *m, ರಚನೆ scanner *s,
                             ರಚನೆ sval ಮೌಲ್ಯ, xkb_layout_ಸೂಚ್ಯಂಕ_t layout_idx,
                             darray_char *expanded, ಗಾತ್ರ_t *i)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = ಮೌಲ್ಯ.ಆರಂಭ;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    ಎಣಿಕೆ ನಿಯಮಗಳು_mlvo mlv;
    xkb_layout_ಸೂಚ್ಯಂಕ_t idx;
    ಅಕ್ಷರ pfx, sfx;
    ರಚನೆ matched_sval *expanded_ಮೌಲ್ಯ;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    ಆದರೆ (str[*i] == 'i' &&
        (*i + 1 == ಮೌಲ್ಯ.len || is_ವಿಲೀನ_ಬಹುಲಕ_ಪೂರ್ವಪ್ರತ್ಯಯ(str[*i + 1])))
    {
        ಆದರೆ (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_ನಿಯಮಗಳು_INVALID_LAYOUT_ಸೂಚ್ಯಂಕ_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            ಹೋಗು error;
        }
        (*i)++;
        ಅಕ್ಷರ ಸೂಚ್ಯಂಕ_str[MAX_LAYOUT_ಸೂಚ್ಯಂಕ_STR_ಉದ್ದಳತೆ + 1];
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = snprintf(ಸೂಚ್ಯಂಕ_str, ಗಾತ್ರ(ಸೂಚ್ಯಂಕ_str), "%"PRIu32,
                                   layout_idx + 1);
        ಪ್ರತಿಜ್ಞಾ(ಎಣಿಕೆ_2 > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, ಸೂಚ್ಯಂಕ_str, (darray_ಗಾತ್ರ_t)ಎಣಿಕೆ_2);
        ಹಿಂತಿರುಗಿ true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    ಆದರೆ (str[*i] == '(' ||
        is_ವಿಲೀನ_ಬಹುಲಕ_ಪೂರ್ವಪ್ರತ್ಯಯ(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        ಆದರೆ (str[*i] == '(') sfx = ')';
        ಆದರೆ (++(*i) >= ಮೌಲ್ಯ.len) ಹೋಗು error;
    }

    /* Mandatory model/layout/variant specifier. */
    ಆಯ್ಕೆ (str[(*i)++]) {
    ಸ್ಥಿತಿ 'm': mlv = MLVO_MODEL; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'l': mlv = MLVO_LAYOUT; ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'v': mlv = MLVO_VARIANT; ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ: ಹೋಗು error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_ಸೂಚ್ಯಂಕ = false;
    ಆದರೆ (*i < ಮೌಲ್ಯ.len && str[*i] == '[') {
        ಆದರೆ (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            ಹೋಗು error;
        }

        ಪೂರ್ಣಾಂಕ consumed = ಹೊರತೆಗೆ_layout_ಸೂಚ್ಯಂಕ(str + (*i), ಮೌಲ್ಯ.len - (*i), &idx);
        ಆದರೆ (consumed <= 0) ಹೋಗು error;
        ಆದರೆ (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            ಪ್ರತಿಜ್ಞಾ(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_ಸೂಚ್ಯಂಕ = true;
        }
        *i += (ಗಾತ್ರ_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    ಆದರೆ (sfx != 0) {
        ಆದರೆ (*i >= ಮೌಲ್ಯ.len) ಹೋಗು error;
        ಆದರೆ (str[(*i)++] != sfx) ಹೋಗು error;
    }

    /* Get the expanded value. */
    expanded_ಮೌಲ್ಯ = ಖಾಲಿ;

    ಆದರೆ (mlv == MLVO_LAYOUT) {
        ಆದರೆ (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.layouts) == 1)
                expanded_ಮೌಲ್ಯ = &darray_ಅಂಶ(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (idx < darray_ಗಾತ್ರ(m->rmlvo.layouts) &&
                   (variable_ಸೂಚ್ಯಂಕ || darray_ಗಾತ್ರ(m->rmlvo.layouts) > 1)) {
                expanded_ಮೌಲ್ಯ = &darray_ಅಂಶ(m->rmlvo.layouts, idx);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mlv == MLVO_VARIANT) {
        ಆದರೆ (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.variants) == 1)
                expanded_ಮೌಲ್ಯ = &darray_ಅಂಶ(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (idx < darray_ಗಾತ್ರ(m->rmlvo.variants) &&
                   (variable_ಸೂಚ್ಯಂಕ || darray_ಗಾತ್ರ(m->rmlvo.variants) > 1)) {
                expanded_ಮೌಲ್ಯ = &darray_ಅಂಶ(m->rmlvo.variants, idx);
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mlv == MLVO_MODEL) {
        expanded_ಮೌಲ್ಯ = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    ಆದರೆ (!expanded_ಮೌಲ್ಯ || expanded_ಮೌಲ್ಯ->sval.len == 0) {
        ಹಿಂತಿರುಗಿ true;
    }

    ಆದರೆ (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_ಮೌಲ್ಯ->sval.ಆರಂಭ,
                                 (darray_ಗಾತ್ರ_t) expanded_ಮೌಲ್ಯ->sval.len);
    ಆದರೆ (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    ಆದರೆ (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ಸ್ಥಿರಾಂಕ xkb_layout_ಮುಖವಾಡ_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_ಮೌಲ್ಯ->matched |= layouts;
    } ಇಲ್ಲದಿದ್ದರೆ {
        expanded_ಮೌಲ್ಯ->matched |= (xkb_layout_ಮುಖವಾಡ_t)GLOBAL_MATCHED_LAYOUTS;
    }

    ಹಿಂತಿರುಗಿ true;

error:
    scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                "invalid %%-expansion in value; not used");
    ಹಿಂತಿರುಗಿ false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
expand_qualifier_ಒಳ_kccgst_ಮೌಲ್ಯ(
    ರಚನೆ matcher *m, ರಚನೆ scanner *s,
    ರಚನೆ sval ಮೌಲ್ಯ, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_ಗಾತ್ರ_t ಪೂರ್ವಪ್ರತ್ಯಯ_idx, ಗಾತ್ರ_t *i)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = ಮೌಲ್ಯ.ಆರಂಭ;

    /* “all” followed by nothing or by a layout separator */
    ಆದರೆ (*i + 2 < ಮೌಲ್ಯ.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == ಮೌಲ್ಯ.len || is_ವಿಲೀನ_ಬಹುಲಕ_ಪೂರ್ವಪ್ರತ್ಯಯ(str[*i + 3]))) {
        ಆದರೆ (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        ಆದರೆ (darray_ಗಾತ್ರ(m->rmlvo.layouts) > 1) {
            ಅಕ್ಷರ layout_ಸೂಚ್ಯಂಕ[MAX_LAYOUT_ಸೂಚ್ಯಂಕ_STR_ಉದ್ದಳತೆ + 1];
            ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ =
                darray_ಗಾತ್ರ(*expanded) - ಪೂರ್ವಪ್ರತ್ಯಯ_idx - 1;
            ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_ಗಾತ್ರ(m->rmlvo.layouts));
                 l++)
            {
                ಆದರೆ (!has_separator)
                    darray_append(*expanded, ವಿಲೀನ_DEFAULT_ಪೂರ್ವಪ್ರತ್ಯಯ);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_ಅಂಶ(*expanded, ಪೂರ್ವಪ್ರತ್ಯಯ_idx),
                                             ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ);
                /* Append index */
                ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = snprintf(layout_ಸೂಚ್ಯಂಕ, ಗಾತ್ರ(layout_ಸೂಚ್ಯಂಕ),
                                           "%"PRIu32, l + 1);
                ಪ್ರತಿಜ್ಞಾ(ಎಣಿಕೆ_2 > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_ಸೂಚ್ಯಂಕ,
                                             (darray_ಗಾತ್ರ_t)ಎಣಿಕೆ_2);
            }
        }
        *i += 3;
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
concat_kccgst(darray_char *into, darray_ಗಾತ್ರ_t ಗಾತ್ರ_2, ಒಳ_reads(ಗಾತ್ರ_2) ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* from)
#ಇಲ್ಲದಿದ್ದರೆ
concat_kccgst(darray_char *into, darray_ಗಾತ್ರ_t ಗಾತ್ರ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ from[ಸ್ಥಿರ ಗಾತ್ರ_2])
#ಆದರೆ_ಅಂತ್ಯ
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ಸ್ಥಿರಾಂಕ bool from_plus = is_ವಿಲೀನ_ಬಹುಲಕ_ಪೂರ್ವಪ್ರತ್ಯಯ(from[0]);
    ಆದರೆ (from_plus || darray_ಖಾಲಿಗಳು(*into)) {
        darray_appends_nullterminate(*into, from, ಗಾತ್ರ_2);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ch =
            (ಅಕ್ಷರ) (darray_ಖಾಲಿಗಳು(*into) ? '\0' : darray_ಅಂಶ(*into, 0));
        ಸ್ಥಿರಾಂಕ bool into_plus = is_ವಿಲೀನ_ಬಹುಲಕ_ಪೂರ್ವಪ್ರತ್ಯಯ(ch);
        ಆದರೆ (into_plus)
            darray_prepends_nullterminate(*into, from, ಗಾತ್ರ_2);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
ಸ್ಥಿರ bool
append_expanded_kccgst_ಮೌಲ್ಯ(ರಚನೆ matcher *m, ರಚನೆ scanner *s,
                             bool ವಿಲೀನ, darray_char *ಗಮ್ಯ, ರಚನೆ sval ಮೌಲ್ಯ,
                             xkb_layout_ಸೂಚ್ಯಂಕ_t layout_idx)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = ಮೌಲ್ಯ.ಆರಂಭ;
    darray_char expanded = darray_new();
    darray_ಗಾತ್ರ_t ಕೊನೆಯ_ಅಂಶ_idx = 0;
    bool has_separator = false;

    ಗಾಗಿ (ಗಾತ್ರ_t i = 0; i < ಮೌಲ್ಯ.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        ಆಯ್ಕೆ (str[i]) {
            /* Qualifier */
            ಸ್ಥಿತಿ ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_ಒಳ_kccgst_ಮೌಲ್ಯ(m, s, ಮೌಲ್ಯ, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 ಕೊನೆಯ_ಅಂಶ_idx, &i);
                ನಿಲ್ಲಿಸು;
            /* Expansion */
            ಸ್ಥಿತಿ '%':
                i++;
                ಆದರೆ (i >= ಮೌಲ್ಯ.len ||
                    !expand_rmlvo_ಒಳ_kccgst_ಮೌಲ್ಯ(m, s, ಮೌಲ್ಯ, layout_idx,
                                                  &expanded, &i))
                        ಹೋಗು error;
                ನಿಲ್ಲಿಸು;
            /* New item */
            ಸ್ಥಿತಿ ವಿಲೀನ_OVERRIDE_ಪೂರ್ವಪ್ರತ್ಯಯ:
            ಸ್ಥಿತಿ ವಿಲೀನ_AUGMENT_ಪೂರ್ವಪ್ರತ್ಯಯ:
            ಸ್ಥಿತಿ ವಿಲೀನ_REPLACE_ಪೂರ್ವಪ್ರತ್ಯಯ:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                ಕೊನೆಯ_ಅಂಶ_idx = darray_ಗಾತ್ರ(expanded) - 1;
                has_separator = true;
                ನಿಲ್ಲಿಸು;
            /* Just a normal character. */
            ಪೂರ್ವನಿಯೋಜಿತ:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    ಆದರೆ (ವಿಲೀನ) {
        ಆದರೆ (!darray_ಖಾಲಿಗಳು(expanded))
            concat_kccgst(ಗಮ್ಯ, darray_ಗಾತ್ರ(expanded), darray_ವಸ್ತುಗಳು(expanded));
    } ಇಲ್ಲದಿದ್ದರೆ {
        darray_concat(*ಗಮ್ಯ, expanded);
    }
    darray_free(expanded);
    ಹಿಂತಿರುಗಿ true;

error:
    darray_free(expanded);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
matcher_append_pending_kccgst(ರಚನೆ matcher *m)
{
    ಆದರೆ (!m->mapping.has_ಅಪವರ್ತ್ಯ_layouts)
        ಹಿಂತಿರುಗಿ true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    ಗಾಗಿ (kccgst_ಸೂಚ್ಯಂಕ_t i = 0; i < m->mapping.ಅಂಕೆ_kccgst; i++) {
        ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ ನಿಯಮಗಳು_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            ನೋಂದಣಿ ಸ್ಥಿರಾಂಕ ರಚನೆ kccgst_ಬಫರ್* ಸ್ಥಿರಾಂಕ buf = &m->pending_kccgst;
            ಗಾತ್ರ_t ಸ್ಥಾನಾಂತರ = 0;
            ಗಾಗಿ (darray_ಗಾತ್ರ_t k = 0; k < darray_ಗಾತ್ರ(buf->slices); k++) {
                ನೋಂದಣಿ ಸ್ಥಿರಾಂಕ ರಚನೆ kccgst_ಬಫರ್_ತುಂಡು * ಸ್ಥಿರಾಂಕ ತುಂಡು =
                    &darray_ಅಂಶ(buf->slices, k);
                ಆದರೆ (ತುಂಡು->kccgst == kccgst && ತುಂಡು->layout == layout &&
                    ತುಂಡು->ಉದ್ದಳತೆ)
                    concat_kccgst(&m->kccgst[kccgst], ತುಂಡು->ಉದ್ದಳತೆ,
                                  darray_ವಸ್ತುಗಳು(buf->ಬಫರ್) + ಸ್ಥಾನಾಂತರ);
                ಸ್ಥಾನಾಂತರ += ತುಂಡು->ಉದ್ದಳತೆ;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_ಅಪವರ್ತ್ಯ_layouts = false;
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
matcher_ನಿಯಮ_verify(ರಚನೆ matcher *m, ರಚನೆ scanner *s)
{
    ಆದರೆ (m->ನಿಯಮ_2.ಅಂಕೆ_mlvo_ಮೌಲ್ಯಗಳು != m->mapping.ಅಂಕೆ_mlvo ||
        m->ನಿಯಮ_2.ಅಂಕೆ_kccgst_ಮೌಲ್ಯಗಳು != m->mapping.ಅಂಕೆ_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                    "invalid rule: must have same number of values "
                    "as mapping line; ignoring rule");
        m->ನಿಯಮ_2.skip = true;
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
ಸ್ಥಿರ bool
matcher_ನಿಯಮ_ಹೊಂದಿಕೆ_model(ರಚನೆ matcher *m, ಸ್ಥಿರಾಂಕ ರಚನೆ sval ಮೌಲ್ಯ,
                         ಎಣಿಕೆ mlvo_ಹೊಂದಿಕೆ_type ಹೊಂದಿಕೆ_type)
{
    ಹಿಂತಿರುಗಿ ಹೊಂದಿಕೆ_ಮೌಲ್ಯ_and_mark(
        m, ಮೌಲ್ಯ, &m->rmlvo.model, ಹೊಂದಿಕೆ_type, WILDCARD_ಹೊಂದಿಕೆ_ALL,
        /* layout-independent */
        (xkb_layout_ಮುಖವಾಡ_t)GLOBAL_MATCHED_LAYOUTS
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
ಸ್ಥಿರ bool
matcher_ನಿಯಮ_ಹೊಂದಿಕೆ_option(ರಚನೆ matcher *m, ಸ್ಥಿರಾಂಕ ರಚನೆ sval ಮೌಲ್ಯ,
                          ಎಣಿಕೆ mlvo_ಹೊಂದಿಕೆ_type ಹೊಂದಿಕೆ_type,
                          xkb_layout_ಮುಖವಾಡ_t *candidates)
{
    /* There is always at least one value */
    ಪ್ರತಿಜ್ಞಾ(!darray_ಖಾಲಿಗಳು(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_ಮುಖವಾಡ_t unmatched =
        (m->mapping.defined_mlvo_ಮುಖವಾಡ & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    ರಚನೆ matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        ಸ್ಥಿರಾಂಕ xkb_layout_ಮುಖವಾಡ_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_ಮುಖವಾಡ_t)GLOBAL_MATCHED_LAYOUTS;
        ಆದರೆ (matchable && ಹೊಂದಿಕೆ_ಮೌಲ್ಯ_and_mark(m, ಮೌಲ್ಯ, option, ಹೊಂದಿಕೆ_type,
                                              WILDCARD_ಹೊಂದಿಕೆ_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            ಆದರೆ (!unmatched)
                ನಿಲ್ಲಿಸು;
        }
    }
    ಆದರೆ (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    ಹಿಂತಿರುಗಿ matched;
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
ಸ್ಥಿರ bool
matcher_ನಿಯಮ_ಹೊಂದಿಕೆ_layout_or_variant(ರಚನೆ matcher *m,
                                     darray_matched_sval *ಒಳಹರಿವು,
                                     ಸ್ಥಿರಾಂಕ ರಚನೆ sval ಮೌಲ್ಯ,
                                     ಎಣಿಕೆ mlvo_ಹೊಂದಿಕೆ_type ಹೊಂದಿಕೆ_type,
                                     xkb_layout_ಮುಖವಾಡ_t *candidates)
{
    /* There is always at least one value */
    ಪ್ರತಿಜ್ಞಾ(!darray_ಖಾಲಿಗಳು(*ಒಳಹರಿವು));

    bool matched = false;
    /* Loop over the layout index range */
    ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ಸ್ಥಿರಾಂಕ xkb_layout_ಮುಖವಾಡ_t layout = (UINT32_C(1) << idx);
        ಆದರೆ (layout & *candidates) {
            ರಚನೆ matched_sval *ಗಮ್ಯ = &darray_ಅಂಶ(*ಒಳಹರಿವು, idx);
            ಆದರೆ (ಹೊಂದಿಕೆ_ಮೌಲ್ಯ_and_mark(m, ಮೌಲ್ಯ, ಗಮ್ಯ, ಹೊಂದಿಕೆ_type,
                                     WILDCARD_ಹೊಂದಿಕೆ_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    ಹಿಂತಿರುಗಿ matched;
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
ಸ್ಥಿರ ಶೂನ್ಯ
matcher_ನಿಯಮ_apply_if_matches(ರಚನೆ matcher *m, ರಚನೆ scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_ಮುಖವಾಡ_t candidate_layouts = m->mapping.layouts_candidates_ಮುಖವಾಡ;

    /* Loop over MLVO pattern components */
    ಗಾಗಿ (mlvo_ಸೂಚ್ಯಂಕ_t i = 0; i < m->mapping.ಅಂಕೆ_mlvo; i++) {
        ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ ನಿಯಮಗಳು_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ಸ್ಥಿರಾಂಕ ರಚನೆ sval ಮೌಲ್ಯ = m->ನಿಯಮ_2.mlvo_ಮೌಲ್ಯ_at_pos[i];
        ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ mlvo_ಹೊಂದಿಕೆ_type ಹೊಂದಿಕೆ_type = m->ನಿಯಮ_2.ಹೊಂದಿಕೆ_type_at_pos[i];
        bool matched = false;

        ಆಯ್ಕೆ (mlvo) {
        ಸ್ಥಿತಿ MLVO_MODEL:
            matched = matcher_ನಿಯಮ_ಹೊಂದಿಕೆ_model(m, ಮೌಲ್ಯ, ಹೊಂದಿಕೆ_type);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ MLVO_LAYOUT:
            matched = matcher_ನಿಯಮ_ಹೊಂದಿಕೆ_layout_or_variant(
                m, &m->rmlvo.layouts, ಮೌಲ್ಯ, ಹೊಂದಿಕೆ_type, &candidate_layouts
            );
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ MLVO_VARIANT:
            matched = matcher_ನಿಯಮ_ಹೊಂದಿಕೆ_layout_or_variant(
                m, &m->rmlvo.variants, ಮೌಲ್ಯ, ಹೊಂದಿಕೆ_type, &candidate_layouts
            );
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ MLVO_OPTION:
            matched = matcher_ನಿಯಮ_ಹೊಂದಿಕೆ_option(
                m, ಮೌಲ್ಯ, ಹೊಂದಿಕೆ_type, &candidate_layouts
            );
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_ಅಂಕೆ_ENTRIES - 1,
                          "Unexpected MLVO field");
            ಪ್ರತಿಜ್ಞಾ(!"Unreachable");
        }
        }

        ಆದರೆ (!matched)
            ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (m->mapping.has_ಅಪವರ್ತ್ಯ_layouts) {
        /* Special index: loop over the index range */
        ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            ಆದರೆ (candidate_layouts & (UINT32_C(1) << idx)) {
                ಗಾಗಿ (kccgst_ಸೂಚ್ಯಂಕ_t i = 0; i < m->mapping.ಅಂಕೆ_kccgst; i++) {
                    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ ನಿಯಮಗಳು_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ಸ್ಥಿರಾಂಕ ರಚನೆ sval ಮೌಲ್ಯ = m->ನಿಯಮ_2.kccgst_ಮೌಲ್ಯ_at_pos[i];
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
                    ರಚನೆ kccgst_ಬಫರ್ * ಸ್ಥಿರಾಂಕ buf = &m->pending_kccgst;
                    ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t prev_ಬಫರ್_ಉದ್ದಳತೆ =
                        darray_ಗಾತ್ರ(buf->ಬಫರ್);
                    append_expanded_kccgst_ಮೌಲ್ಯ(m, s, false, &buf->ಬಫರ್,
                                                 ಮೌಲ್ಯ, idx);
                    ಸ್ಥಿರಾಂಕ uint32_t ಉದ್ದಳತೆ = (uint32_t) (darray_ಗಾತ್ರ(buf->ಬಫರ್)
                                          - prev_ಬಫರ್_ಉದ್ದಳತೆ);
                    ಸ್ಥಿರಾಂಕ ರಚನೆ kccgst_ಬಫರ್_ತುಂಡು ತುಂಡು = {
                        .ಉದ್ದಳತೆ = ಉದ್ದಳತೆ,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, ತುಂಡು);
                }
            }
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Numeric index or no index */
        ಗಾಗಿ (kccgst_ಸೂಚ್ಯಂಕ_t i = 0; i < m->mapping.ಅಂಕೆ_kccgst; i++) {
            ಎಣಿಕೆ ನಿಯಮಗಳು_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            ರಚನೆ sval ಮೌಲ್ಯ = m->ನಿಯಮ_2.kccgst_ಮೌಲ್ಯ_at_pos[i];
            append_expanded_kccgst_ಮೌಲ್ಯ(m, s, true, &m->kccgst[kccgst], ಮೌಲ್ಯ,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    ಆದರೆ (!(is_mlvo_ಮುಖವಾಡ_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_ಮುಖವಾಡ &= ~candidate_layouts;
    }
}

ಸ್ಥಿರ ಎಣಿಕೆ ನಿಯಮಗಳು_token
gettok(ರಚನೆ matcher *m, ರಚನೆ scanner *s)
{
    ಹಿಂತಿರುಗಿ lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
ಸ್ಥಿರ bool
matcher_ಹೊಂದಿಕೆ(ರಚನೆ matcher *m, ರಚನೆ scanner *s,
              ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ,
              ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len,
              ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name)
{
    ಎಣಿಕೆ ನಿಯಮಗಳು_token tok;

    ಆದರೆ (!m)
        ಹಿಂತಿರುಗಿ false;

initial:
    ಆಯ್ಕೆ (tok = gettok(m, s)) {
    ಸ್ಥಿತಿ TOK_BANG:
        ಹೋಗು bang;
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_ಸಾಲು:
        ಹೋಗು initial;
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_FILE:
        ಹೋಗು ಮುಕ್ತಾಯ;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

bang:
    ಆಯ್ಕೆ (tok = gettok(m, s)) {
    ಸ್ಥಿತಿ TOK_ಗುಂಪು_NAME:
        matcher_ಗುಂಪು_ಆರಂಭ_new(m, m->val.string);
        ಹೋಗು ಗುಂಪು_name;
    ಸ್ಥಿತಿ TOK_INCLUDE:
        ಹೋಗು include_statement;
    ಸ್ಥಿತಿ TOK_IDENTIFIER:
        matcher_mapping_ಆರಂಭ_new(m);
        matcher_mapping_ಗಣ_mlvo(m, s, m->val.string);
        ಹೋಗು mapping_mlvo;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

ಗುಂಪು_name:
    ಆಯ್ಕೆ (tok = gettok(m, s)) {
    ಸ್ಥಿತಿ TOK_EQUALS:
        ಹೋಗು ಗುಂಪು_element;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

ಗುಂಪು_element:
    ಆಯ್ಕೆ (tok = gettok(m, s)) {
    ಸ್ಥಿತಿ TOK_IDENTIFIER:
        matcher_ಗುಂಪು_ಕೂಡಿಸು_element(m, s, m->val.string);
        ಹೋಗು ಗುಂಪು_element;
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_ಸಾಲು:
        ಹೋಗು initial;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

include_statement:
    ಆಯ್ಕೆ (tok = gettok(m, s)) {
    ಸ್ಥಿತಿ TOK_IDENTIFIER:
        matcher_include(m, s, include_ಆಳ, m->val.string);
        ಹೋಗು include_statement_ಕೊನೆ;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

include_statement_ಕೊನೆ:
    ಆಯ್ಕೆ (tok = gettok(m, s)) {
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_ಸಾಲು:
        ಹೋಗು initial;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

mapping_mlvo:
    ಆಯ್ಕೆ (tok = gettok(m, s)) {
    ಸ್ಥಿತಿ TOK_IDENTIFIER:
        ಆದರೆ (m->mapping.ಸಕ್ರಿಯ)
            matcher_mapping_ಗಣ_mlvo(m, s, m->val.string);
        ಹೋಗು mapping_mlvo;
    ಸ್ಥಿತಿ TOK_EQUALS:
        ಹೋಗು mapping_kccgst;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

mapping_kccgst:
    ಆಯ್ಕೆ (tok = gettok(m, s)) {
    ಸ್ಥಿತಿ TOK_IDENTIFIER:
        ಆದರೆ (m->mapping.ಸಕ್ರಿಯ)
            matcher_mapping_ಗಣ_kccgst(m, s, m->val.string);
        ಹೋಗು mapping_kccgst;
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_ಸಾಲು:
        ಆದರೆ (m->mapping.ಸಕ್ರಿಯ && matcher_mapping_verify(m, s)) {
            matcher_mapping_ಗಣ_layout_bounds(m);
            ಆದರೆ (m->mapping.has_ಅಪವರ್ತ್ಯ_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_ಗಾತ್ರ(m->pending_kccgst.ಬಫರ್) = 0;
                darray_ಗಾತ್ರ(m->pending_kccgst.slices) = 0;
            }
        }
        ಹೋಗು ನಿಯಮ_mlvo_ಮೊದಲ;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

ನಿಯಮ_mlvo_ಮೊದಲ:
    ಆಯ್ಕೆ (tok = gettok(m, s)) {
    ಸ್ಥಿತಿ TOK_BANG:
        matcher_append_pending_kccgst(m);
        ಹೋಗು bang;
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_ಸಾಲು:
        ಹೋಗು ನಿಯಮ_mlvo_ಮೊದಲ;
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_FILE:
        matcher_append_pending_kccgst(m);
        ಹೋಗು ಮುಕ್ತಾಯ;
    ಪೂರ್ವನಿಯೋಜಿತ:
        matcher_ನಿಯಮ_ಆರಂಭ_new(m);
        ಹೋಗು ನಿಯಮ_mlvo_no_tok;
    }

ನಿಯಮ_mlvo:
    tok = gettok(m, s);
ನಿಯಮ_mlvo_no_tok:
    ಆಯ್ಕೆ (tok) {
    ಸ್ಥಿತಿ TOK_IDENTIFIER:
        ಆದರೆ (!m->ನಿಯಮ_2.skip)
            matcher_ನಿಯಮ_ಗಣ_mlvo(m, s, m->val.string);
        ಹೋಗು ನಿಯಮ_mlvo;
    ಸ್ಥಿತಿ TOK_WILD_CARD_ನಕ್ಷತ್ರ:
        ಆದರೆ (!m->ನಿಯಮ_2.skip)
            matcher_ನಿಯಮ_ಗಣ_mlvo_wildcard(m, s, MLVO_ಹೊಂದಿಕೆ_WILDCARD_LEGACY);
        ಹೋಗು ನಿಯಮ_mlvo;
    ಸ್ಥಿತಿ TOK_WILD_CARD_NONE:
        ಆದರೆ (!m->ನಿಯಮ_2.skip)
            matcher_ನಿಯಮ_ಗಣ_mlvo_wildcard(m, s, MLVO_ಹೊಂದಿಕೆ_WILDCARD_NONE);
        ಹೋಗು ನಿಯಮ_mlvo;
    ಸ್ಥಿತಿ TOK_WILD_CARD_SOME:
        ಆದರೆ (!m->ನಿಯಮ_2.skip)
            matcher_ನಿಯಮ_ಗಣ_mlvo_wildcard(m, s, MLVO_ಹೊಂದಿಕೆ_WILDCARD_SOME);
        ಹೋಗು ನಿಯಮ_mlvo;
    ಸ್ಥಿತಿ TOK_WILD_CARD_ANY:
        ಆದರೆ (!m->ನಿಯಮ_2.skip)
            matcher_ನಿಯಮ_ಗಣ_mlvo_wildcard(m, s, MLVO_ಹೊಂದಿಕೆ_WILDCARD_ANY);
        ಹೋಗು ನಿಯಮ_mlvo;
    ಸ್ಥಿತಿ TOK_ಗುಂಪು_NAME:
        ಆದರೆ (!m->ನಿಯಮ_2.skip)
            matcher_ನಿಯಮ_ಗಣ_mlvo_ಗುಂಪು(m, s, m->val.string);
        ಹೋಗು ನಿಯಮ_mlvo;
    ಸ್ಥಿತಿ TOK_EQUALS:
        ಹೋಗು ನಿಯಮ_kccgst;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

ನಿಯಮ_kccgst:
    ಆಯ್ಕೆ (tok = gettok(m, s)) {
    ಸ್ಥಿತಿ TOK_IDENTIFIER:
        ಆದರೆ (!m->ನಿಯಮ_2.skip)
            matcher_ನಿಯಮ_ಗಣ_kccgst(m, s, m->val.string);
        ಹೋಗು ನಿಯಮ_kccgst;
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_ಸಾಲು:
        ಆದರೆ (!m->ನಿಯಮ_2.skip)
            matcher_ನಿಯಮ_verify(m, s);
        ಆದರೆ (!m->ನಿಯಮ_2.skip)
            matcher_ನಿಯಮ_apply_if_matches(m, s);
        ಹೋಗು ನಿಯಮ_mlvo_ಮೊದಲ;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

unexpected:
    ಆಯ್ಕೆ (tok) {
    ಸ್ಥಿತಿ TOK_ERROR:
        ಹೋಗು error;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು state_error;
    }

ಮುಕ್ತಾಯ:
    ಹಿಂತಿರುಗಿ true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_ನಿಯಮಗಳು_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    ಹಿಂತಿರುಗಿ false;
}
// NOLINTEND(readability-trivial-switch)

ಸ್ಥಿರ bool
read_ನಿಯಮಗಳು_file(ರಚನೆ xkb_context *ctx,
                ರಚನೆ matcher *matcher,
                ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ,
                FILE *file,
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ)
{
    bool ret;
    ಅಕ್ಷರ *string;
    ಗಾತ್ರ_t ಗಾತ್ರ_2;
    ರಚನೆ scanner scanner;

    ಆದರೆ (!map_file(file, &string, &ಗಾತ್ರ_2)) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                ಮಾರ್ಗ, strerror(errno));
        ಹಿಂತಿರುಗಿ false;
    }

    scanner_init(&scanner, matcher->ctx, string, ಗಾತ್ರ_2, ಮಾರ್ಗ, ಖಾಲಿ);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    ಆದರೆ (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, ಗಾತ್ರ_2);
        ಹಿಂತಿರುಗಿ false;
    }

    ret = matcher_ಹೊಂದಿಕೆ(matcher, &scanner, include_ಆಳ, string, ಗಾತ್ರ_2, ಮಾರ್ಗ);
    unmap_file(string, ಗಾತ್ರ_2);
    ಹಿಂತಿರುಗಿ ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
ಸ್ಥಿರ bool
xkb_resolve_partial_ನಿಯಮಗಳು(ರಚನೆ xkb_context *ctx, ಅಕ್ಷರ *ಮಾರ್ಗ, ಗಾತ್ರ_t ಮಾರ್ಗ_ಗಾತ್ರ,
                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ನಿಯಮಗಳು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* suffix,
                          ರಚನೆ matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    ಅಕ್ಷರ partial_ನಿಯಮಗಳು[60]; /* Arbitrary, but we do not expect long names */
    ಆದರೆ (unlikely(!snprintf_ಸುರಕ್ಷಿತ(partial_ನಿಯಮಗಳು, ಗಾತ್ರ(partial_ನಿಯಮಗಳು),
                                "%s%s", ನಿಯಮಗಳು, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", ನಿಯಮಗಳು, suffix);
        ಹಿಂತಿರುಗಿ false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 0;
    FILE *file = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(partial_ನಿಯಮಗಳು);
    ಆಗ ((file = ಕಂಡುಹಿಡಿ_File_ಒಳ_Xkb_ಮಾರ್ಗ(ctx, "(unknown)",
                                     partial_ನಿಯಮಗಳು, len, FILE_TYPE_ನಿಯಮಗಳು,
                                     ಮಾರ್ಗ, ಮಾರ್ಗ_ಗಾತ್ರ, &ಸ್ಥಾನಾಂತರ, false)) != ಖಾಲಿ) {
        ಸ್ಥಿರಾಂಕ bool ok = read_ನಿಯಮಗಳು_file(ctx, matcher, 0, file, ಮಾರ್ಗ);
        fclose(file);
        ಆದರೆ (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", ಮಾರ್ಗ);
            ಹಿಂತಿರುಗಿ false;
        }
        ಸ್ಥಾನಾಂತರ++;
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
xkb_resolve_ನಿಯಮಗಳು(ರಚನೆ xkb_context *ctx,
                  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ನಿಯಮಗಳು, ರಚನೆ matcher *matcher,
                  ರಚನೆ xkb_component_ಹೆಸರುಗಳು *ಹೊರಹೋಗು,
                  xkb_layout_ಸೂಚ್ಯಂಕ_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 0;
    ಅಕ್ಷರ ಮಾರ್ಗ[PATH_MAX];
    FILE * ಸ್ಥಿರಾಂಕ file = ಕಂಡುಹಿಡಿ_File_ಒಳ_Xkb_ಮಾರ್ಗ(ctx, "(unknown)",
                                          ನಿಯಮಗಳು, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ನಿಯಮಗಳು), FILE_TYPE_ನಿಯಮಗಳು,
                                          ಮಾರ್ಗ, ಗಾತ್ರ(ಮಾರ್ಗ), &ಸ್ಥಾನಾಂತರ, true);
    ಆದರೆ (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", ನಿಯಮಗಳು);
        ಹೋಗು err_ಹೊರಹೋಗು;
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
    ret = xkb_resolve_partial_ನಿಯಮಗಳು(ctx, ಮಾರ್ಗ, ಗಾತ್ರ(ಮಾರ್ಗ),
                                    ನಿಯಮಗಳು, ".pre", matcher);
    ಆದರೆ (!ret)
        ಹೋಗು err_ಹೊರಹೋಗು;

    /* Resolve main <rules> file */
    ret = read_ನಿಯಮಗಳು_file(ctx, matcher, 0, file, ಮಾರ್ಗ);
    ಆದರೆ (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", ಮಾರ್ಗ);
        ಹೋಗು err_ಹೊರಹೋಗು;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_ನಿಯಮಗಳು(ctx, ಮಾರ್ಗ, ಗಾತ್ರ(ಮಾರ್ಗ),
                                    ನಿಯಮಗಳು, ".post", matcher);
    ಆದರೆ (!ret)
        ಹೋಗು err_ಹೊರಹೋಗು;

    /* Check we got required components */
    ಆದರೆ (darray_ಖಾಲಿಗಳು(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_ಖಾಲಿಗಳು(matcher->kccgst[KCCGST_TYPES]) ||
        darray_ಖಾಲಿಗಳು(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_ಖಾಲಿಗಳು(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", ನಿಯಮಗಳು);
        ret = false;
        ಹೋಗು err_ಹೊರಹೋಗು;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &ಹೊರಹೋಗು->keycodes, ಖಾಲಿ);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &ಹೊರಹೋಗು->types, ಖಾಲಿ);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &ಹೊರಹೋಗು->compatibility, ಖಾಲಿ);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &ಹೊರಹೋಗು->symbols, ಖಾಲಿ);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &ಹೊರಹೋಗು->geometry, ಖಾಲಿ);

    ರಚನೆ matched_sval *mval = &matcher->rmlvo.model;
    ಆದರೆ (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ಮೌಲ್ಯ,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) mval->sval.len, mval->sval.ಆರಂಭ, ನಿಯಮಗಳು);
    darray_foreach(mval, matcher->rmlvo.layouts)
        ಆದರೆ (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ಮೌಲ್ಯ,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) mval->sval.len, mval->sval.ಆರಂಭ, ನಿಯಮಗಳು);
    darray_foreach(mval, matcher->rmlvo.variants)
        ಆದರೆ (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ಮೌಲ್ಯ,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) mval->sval.len, mval->sval.ಆರಂಭ, ನಿಯಮಗಳು);
    darray_foreach(mval, matcher->rmlvo.options) {
        ಆದರೆ (!mval->sval.len)
            ಮುಂದುವರಿ;

        ಆದರೆ (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ಮೌಲ್ಯ,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ) mval->sval.len, mval->sval.ಆರಂಭ, ನಿಯಮಗಳು);
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ಸ್ಥಿರಾಂಕ xkb_layout_ಮುಖವಾಡ_t unmatched = (mval->layouts & ~mval->matched);
            ಪ್ರತಿಜ್ಞಾ(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ಮೌಲ್ಯ,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)mval->sval.len, mval->sval.ಆರಂಭ,
                    unmatched, ನಿಯಮಗಳು);
        }
    }

    /* Set the number of explicit layouts */
    ಆದರೆ (ಹೊರಹೋಗು->symbols != ಖಾಲಿ && explicit_layouts != ಖಾಲಿ) {
        *explicit_layouts = 1; /* at least one group */
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *symbols = ಹೊರಹೋಗು->symbols;
        /* Take the highest modifier */
        ಆಗ ((symbols = strchr(symbols, ':')) != ಖಾಲಿ && symbols[1] != '\0') {
            xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0;
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = parse_dec_ಗಮ್ಯ_uint32_t(++symbols, SIZE_MAX, &ಗುಂಪು);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            ಆದರೆ (ಎಣಿಕೆ_2 > 0 && (symbols[ಎಣಿಕೆ_2] == '\0' ||
                is_ವಿಲೀನ_ಬಹುಲಕ_ಪೂರ್ವಪ್ರತ್ಯಯ(symbols[ಎಣಿಕೆ_2])) &&
                ಗುಂಪು > 0 && ಗುಂಪು <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, ಗುಂಪು);
                symbols += ಎಣಿಕೆ_2;
            }
        }
    }

err_ಹೊರಹೋಗು:
    ಆದರೆ (file)
        fclose(file);

    ಹಿಂತಿರುಗಿ ret;
}

bool
xkb_components_from_rmlvo_builder(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_rmlvo_builder *rmlvo,
                                  ರಚನೆ xkb_component_ಹೆಸರುಗಳು *ಹೊರಹೋಗು,
                                  xkb_layout_ಸೂಚ್ಯಂಕ_t *explicit_layouts)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ನಿಯಮಗಳು = rmlvo->ನಿಯಮಗಳು;
    ರಚನೆ matcher *matcher = matcher_new_from_rmlvo(rmlvo, &ನಿಯಮಗಳು);
    ಆದರೆ (!matcher)
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿರಾಂಕ bool ret = xkb_resolve_ನಿಯಮಗಳು(rmlvo->ctx, ನಿಯಮಗಳು, matcher,
                                       ಹೊರಹೋಗು, explicit_layouts);

    matcher_free(matcher);
    ಹಿಂತಿರುಗಿ ret;
}


bool
xkb_components_from_ನಿಯಮಗಳು_ಹೆಸರುಗಳು(ರಚನೆ xkb_context *ctx,
                                ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ನಿಯಮ_ಹೆಸರುಗಳು *rmlvo,
                                ರಚನೆ xkb_component_ಹೆಸರುಗಳು *ಹೊರಹೋಗು,
                                xkb_layout_ಸೂಚ್ಯಂಕ_t *explicit_layouts)
{
    ರಚನೆ matcher *matcher = matcher_new_from_ಹೆಸರುಗಳು(ctx, rmlvo);
    ಆದರೆ (!matcher)
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿರಾಂಕ bool ret = xkb_resolve_ನಿಯಮಗಳು(ctx, rmlvo->ನಿಯಮಗಳು, matcher,
                                       ಹೊರಹೋಗು, explicit_layouts);

    matcher_free(matcher);
    ಹಿಂತಿರುಗಿ ret;
}
