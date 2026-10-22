/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-numbers.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
ರಚನೆ sval {
    ಗಾತ್ರ_t len;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಆರಂಭ ATTR_COUNTED_BY(len);
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ darray(ರಚನೆ sval) darray_sval;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
svaleq(ರಚನೆ sval s1, ರಚನೆ sval s2)
{
    ಹಿಂತಿರುಗಿ s1.len == s2.len && memcmp(s1.ಆರಂಭ, s2.ಆರಂಭ, s1.len) == 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
isvaleq(ರಚನೆ sval s1, ರಚನೆ sval s2)
{
    ಹಿಂತಿರುಗಿ s1.len == s2.len && istrncmp(s1.ಆರಂಭ, s2.ಆರಂಭ, s1.len) == 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
svaleq_ಪೂರ್ವಪ್ರತ್ಯಯ(ರಚನೆ sval s1, ರಚನೆ sval s2)
{
    ಹಿಂತಿರುಗಿ s1.len <= s2.len && memcmp(s1.ಆರಂಭ, s2.ಆರಂಭ, s1.len) == 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು SVAL(__start, __len) (ರಚನೆ sval){ .ಆರಂಭ = (__start), .len = (__len) }
#ವ್ಯಾಖ್ಯಾನಿಸು SVAL_LIT(literal) SVAL(literal, ಗಾತ್ರ(literal) - 1)
#ವ್ಯಾಖ್ಯಾನಿಸು SVAL_INIT(literal) { .ಆರಂಭ = (literal), .len = ಗಾತ್ರ(literal) - 1 }

/* A line:column location in the input string (1-based). */
ರಚನೆ scanner_loc {
    ಗಾತ್ರ_t ಸಾಲು, ಸ್ತಂಭ;
};

ರಚನೆ scanner {
    ಗಾತ್ರ_t pos;
    ಗಾತ್ರ_t len;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    ಅಕ್ಷರ buf[1024];
    ಗಾತ್ರ_t buf_pos;
    /* The position of the start of the current token. */
    ಗಾತ್ರ_t token_pos;
    /* Cached values used by scanner_token_location. */
    ಗಾತ್ರ_t cached_pos;
    ರಚನೆ scanner_loc cached_loc;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name;
    ರಚನೆ xkb_context *ctx;
    ಶೂನ್ಯ *priv;
};

/* Compute the line:column location for the current token (slow). */
ರಚನೆ scanner_loc
scanner_token_location(ರಚನೆ scanner *s);

#ವ್ಯಾಖ್ಯಾನಿಸು scanner_log_with_ಸಂಕೇತ(scanner, level, verbosity, log_ಸಂದೇಶ_id, fmt, ...) ಮಾಡು { \
    ರಚನೆ scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_ಸಂಕೇತ((scanner)->ctx, (level), verbosity, log_ಸಂದೇಶ_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.ಸಾಲು,                                                     \
                      loc.ಸ್ತಂಭ, ##__VA_ARGS__);                                   \
} ಆಗ(0)

#ವ್ಯಾಖ್ಯಾನಿಸು scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_ಸಂಕೇತ(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_ಕನಿಷ್ಠತಮ, id, fmt, ##__VA_ARGS__)

#ವ್ಯಾಖ್ಯಾನಿಸು scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_ಸಂಕೇತ(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_ಕನಿಷ್ಠತಮ, id, fmt, ##__VA_ARGS__)

#ವ್ಯಾಖ್ಯಾನಿಸು scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_ಸಂಕೇತ(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
scanner_init(ರಚನೆ scanner *s, ರಚನೆ xkb_context *ctx,
             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name,
             ಶೂನ್ಯ *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.ಸಾಲು = s->cached_loc.ಸ್ತಂಭ = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ
scanner_peek(ರಚನೆ scanner *s)
{
    ಆದರೆ (unlikely(s->pos >= s->len))
        ಹಿಂತಿರುಗಿ '\0';
    ಹಿಂತಿರುಗಿ s->s[s->pos];
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_eof(ರಚನೆ scanner *s)
{
    ಹಿಂತಿರುಗಿ s->pos >= s->len;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_eol(ರಚನೆ scanner *s)
{
    ಹಿಂತಿರುಗಿ scanner_peek(s) == '\n';
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
scanner_skip_ಗಮ್ಯ_eol(ರಚನೆ scanner *s)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t new_pos = nl ? (ಗಾತ್ರ_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಅಕ್ಷರ
scanner_ಮುಂದಿನ(ರಚನೆ scanner *s)
{
    ಆದರೆ (unlikely(scanner_eof(s)))
        ಹಿಂತಿರುಗಿ '\0';
    ಹಿಂತಿರುಗಿ s->s[s->pos++];
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_chr(ರಚನೆ scanner *s, ಅಕ್ಷರ ch)
{
    ಆದರೆ (likely(scanner_peek(s) != ch))
        ಹಿಂತಿರುಗಿ false;
    s->pos++;
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_str(ರಚನೆ scanner *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len)
{
    ಆದರೆ (s->len - s->pos < len)
        ಹಿಂತಿರುಗಿ false;
    ಆದರೆ (memcmp(s->s + s->pos, string, len) != 0)
        ಹಿಂತಿರುಗಿ false;
    s->pos += len;
    ಹಿಂತಿರುಗಿ true;
}

#ವ್ಯಾಖ್ಯಾನಿಸು scanner_lit(s, literal) scanner_str(s, literal, ಗಾತ್ರ(literal) - 1)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_ನಿಯಮಗಳು_is_ident(ಅಕ್ಷರ ch)
{
    ಹಿಂತಿರುಗಿ is_ನಕ್ಷೆ(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_ನಿಯಮಗಳು_str_token(ರಚನೆ scanner *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len)
{
    ಆದರೆ (!scanner_str(s, string, len)) {
        ಹಿಂತಿರುಗಿ false;
    }
    ಆದರೆ (scanner_ನಿಯಮಗಳು_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        ಹಿಂತಿರುಗಿ false;
    }
    ಹಿಂತಿರುಗಿ true;
}

#ವ್ಯಾಖ್ಯಾನಿಸು scanner_ನಿಯಮಗಳು_lit_token(s, literal) \
    scanner_ನಿಯಮಗಳು_str_token((s), (literal), ಗಾತ್ರ(literal) - 1)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಗಾತ್ರ_t
scanner_ನಿಯಮಗಳು_ident_token(ರಚನೆ scanner *s)
{
    ಗಾತ್ರ_t len = 0;
    ಆಗ (!scanner_eof(s) && scanner_ನಿಯಮಗಳು_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    ಹಿಂತಿರುಗಿ len;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_buf_append(ರಚನೆ scanner *s, ಅಕ್ಷರ ch)
{
    ಆದರೆ (s->buf_pos + 1 >= ಗಾತ್ರ(s->buf))
        ಹಿಂತಿರುಗಿ false;
    s->buf[s->buf_pos++] = ch;
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_buf_appends(ರಚನೆ scanner *s, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ret = snprintf(s->buf + s->buf_pos,
                             ಗಾತ್ರ(s->buf) - s->buf_pos,
                             "%s", str);
    ಆದರೆ (ret < 0 || (ಗಾತ್ರ_t)ret >= ಗಾತ್ರ(s->buf) - s->buf_pos)
        ಹಿಂತಿರುಗಿ false;
    s->buf_pos += (ಗಾತ್ರ_t)ret;
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_buf_appends_ಸಂಕೇತ_ಬಿಂದು(ರಚನೆ scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    ಆದರೆ (s->buf_pos + 5 <= ಗಾತ್ರ(s->buf)) {
        uint8_t ಎಣಿಕೆ_2 = utf32_ಗಮ್ಯ_utf8(c, s->buf + s->buf_pos);
        ಆದರೆ (ಎಣಿಕೆ_2 == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            ಎಣಿಕೆ_2 = utf32_ಗಮ್ಯ_utf8(0xfffd, s->buf + s->buf_pos);
        }
        ಆದರೆ (ಎಣಿಕೆ_2 == 0)
            ಹಿಂತಿರುಗಿ false;
        /* `count` counts the NULL byte */
        s->buf_pos += (ಗಾತ್ರ_t)ಎಣಿಕೆ_2 - 1;
        ಹಿಂತಿರುಗಿ true;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ false;
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_oct(ರಚನೆ scanner *s, uint8_t *ಹೊರಹೋಗು)
{
    uint8_t i = 0;
    uint8_t c = 0;
    ಗಾಗಿ (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        ಆದರೆ (c < 040) {
            c = c * 8 + (uint8_t)(scanner_ಮುಂದಿನ(s) - '0');
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Consume valid digit, but mark result as invalid */
            scanner_ಮುಂದಿನ(s);
            *ಹೊರಹೋಗು = c;
            ಹಿಂತಿರುಗಿ false;
        }
    }
    *ಹೊರಹೋಗು = c;
    ಹಿಂತಿರುಗಿ i > 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_hex(ರಚನೆ scanner *s, uint8_t *ಹೊರಹೋಗು)
{
    uint8_t i;
    ಗಾಗಿ (i = 0, *ಹೊರಹೋಗು = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ c = scanner_ಮುಂದಿನ(s);
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ ಸ್ಥಾನಾಂತರ = (ಅಕ್ಷರ) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *ಹೊರಹೋಗು = *ಹೊರಹೋಗು * 16 + (uint8_t)(c - ಸ್ಥಾನಾಂತರ);
    }
    ಹಿಂತಿರುಗಿ i > 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
scanner_dec_int64(ರಚನೆ scanner *s, int64_t *ಹೊರಹೋಗು)
{
    uint64_t val = 0;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 =
        parse_dec_ಗಮ್ಯ_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ಆದರೆ (ಎಣಿಕೆ_2 > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        ಆದರೆ (val > INT64_MAX)
            ಹಿಂತಿರುಗಿ -1;
        s->pos += (ಗಾತ್ರ_t)ಎಣಿಕೆ_2;
        *ಹೊರಹೋಗು = (int64_t)val;
    }
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
scanner_hex_int64(ರಚನೆ scanner *s, int64_t *ಹೊರಹೋಗು)
{
    uint64_t val = 0;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 =
        parse_hex_ಗಮ್ಯ_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ಆದರೆ (ಎಣಿಕೆ_2 > 0) {
        /* See comment in `scanner_dec_int64()` above */
        ಆದರೆ (val > INT64_MAX)
            ಹಿಂತಿರುಗಿ -1;
        s->pos += (ಗಾತ್ರ_t)ಎಣಿಕೆ_2;
        *ಹೊರಹೋಗು = (int64_t) val;
    }
    ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_unicode_ಸಂಕೇತ_ಬಿಂದು(ರಚನೆ scanner *s, uint32_t *ಹೊರಹೋಗು)
{
    ಆದರೆ (!scanner_chr(s, '{'))
        ಹಿಂತಿರುಗಿ false;

    uint32_t cp = 0;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 =
        parse_hex_ಗಮ್ಯ_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    ಆದರೆ (ಎಣಿಕೆ_2 > 0)
        s->pos += (ಗಾತ್ರ_t)ಎಣಿಕೆ_2;

    /* Try to consume everything within the string until the next `}` */
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಕೊನೆಯ_valid = s->pos;
    ಆಗ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_ಮುಂದಿನ(s);
    }

    ಆದರೆ (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *ಹೊರಹೋಗು = cp;
        ಹಿಂತಿರುಗಿ (ಎಣಿಕೆ_2 > 0 && s->pos == ಕೊನೆಯ_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = ಕೊನೆಯ_valid;
    ಹಿಂತಿರುಗಿ false;
}

/* Basic detection of wrong character encoding based on the first bytes */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
scanner_check_supported_char_encoding(ರಚನೆ scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    ಆದರೆ (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        ಹಿಂತಿರುಗಿ true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    ಆದರೆ (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        ಹಿಂತಿರುಗಿ false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    ಆದರೆ (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        ಹಿಂತಿರುಗಿ false;
    }

    ಹಿಂತಿರುಗಿ true;
}
