/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

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
ಸಂರಚನಾ sval {
    size_t len;
    ಧ್ರುವ ವರ್ಣ *start ATTR_COUNTED_BY(len);
};
ಪ್ರಕಾರನಿರ್ದೇಶ darray(ಸಂರಚನಾ sval) darray_sval;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
svaleq(ಸಂರಚನಾ sval s1, ಸಂರಚನಾ sval s2)
{
    ಪ್ರತ್ಯಾಗಮ s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
isvaleq(ಸಂರಚನಾ sval s1, ಸಂರಚನಾ sval s2)
{
    ಪ್ರತ್ಯಾಗಮ s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
svaleq_prefix(ಸಂರಚನಾ sval s1, ಸಂರಚನಾ sval s2)
{
    ಪ್ರತ್ಯಾಗಮ s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#define SVAL(__start, __len) (ಸಂರಚನಾ sval){ .start = (__start), .len = (__len) }
#define SVAL_LIT(literal) SVAL(literal, ಪರಿಮಾಣ(literal) - 1)
#define SVAL_INIT(literal) { .start = (literal), .len = ಪರಿಮಾಣ(literal) - 1 }

/* A line:column location in the input string (1-based). */
ಸಂರಚನಾ scanner_loc {
    size_t ಸಾಲು, column;
};

ಸಂರಚನಾ scanner {
    size_t pos;
    size_t len;
    ಧ್ರುವ ವರ್ಣ *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    ವರ್ಣ buf[1024];
    size_t buf_pos;
    /* The position of the start of the current token. */
    size_t token_pos;
    /* Cached values used by scanner_token_location. */
    size_t cached_pos;
    ಸಂರಚನಾ scanner_loc cached_loc;
    ಧ್ರುವ ವರ್ಣ *file_name;
    ಸಂರಚನಾ xkb_context *ctx;
    ಶೂನ್ಯ *priv;
};

/* Compute the line:column location for the current token (slow). */
ಸಂರಚನಾ scanner_loc
scanner_token_location(ಸಂರಚನಾ scanner *s);

#define scanner_log_with_code(scanner, level, verbosity, log_ಸಂದೇಶ_id, fmt, ...) ಕುರು { \
    ಸಂರಚನಾ scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_ಸಂದೇಶ_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.ಸಾಲು,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} ಯಾವತ್(0)

#define scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
scanner_init(ಸಂರಚನಾ scanner *s, ಸಂರಚನಾ xkb_context *ctx,
             ಧ್ರುವ ವರ್ಣ *string, size_t len, ಧ್ರುವ ವರ್ಣ *file_name,
             ಶೂನ್ಯ *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.ಸಾಲು = s->cached_loc.column = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ
scanner_peek(ಸಂರಚನಾ scanner *s)
{
    ಯದಿ (unlikely(s->pos >= s->len))
        ಪ್ರತ್ಯಾಗಮ '\0';
    ಪ್ರತ್ಯಾಗಮ s->s[s->pos];
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_eof(ಸಂರಚನಾ scanner *s)
{
    ಪ್ರತ್ಯಾಗಮ s->pos >= s->len;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_eol(ಸಂರಚನಾ scanner *s)
{
    ಪ್ರತ್ಯಾಗಮ scanner_peek(s) == '\n';
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
scanner_skip_to_eol(ಸಂರಚನಾ scanner *s)
{
    ಧ್ರುವ ವರ್ಣ *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    ಧ್ರುವ size_t new_pos = nl ? (size_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ವರ್ಣ
scanner_next(ಸಂರಚನಾ scanner *s)
{
    ಯದಿ (unlikely(scanner_eof(s)))
        ಪ್ರತ್ಯಾಗಮ '\0';
    ಪ್ರತ್ಯಾಗಮ s->s[s->pos++];
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_chr(ಸಂರಚನಾ scanner *s, ವರ್ಣ ch)
{
    ಯದಿ (likely(scanner_peek(s) != ch))
        ಪ್ರತ್ಯಾಗಮ false;
    s->pos++;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_str(ಸಂರಚನಾ scanner *s, ಧ್ರುವ ವರ್ಣ *string, size_t len)
{
    ಯದಿ (s->len - s->pos < len)
        ಪ್ರತ್ಯಾಗಮ false;
    ಯದಿ (memcmp(s->s + s->pos, string, len) != 0)
        ಪ್ರತ್ಯಾಗಮ false;
    s->pos += len;
    ಪ್ರತ್ಯಾಗಮ true;
}

#define scanner_lit(s, literal) scanner_str(s, literal, ಪರಿಮಾಣ(literal) - 1)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_rules_is_ident(ವರ್ಣ ch)
{
    ಪ್ರತ್ಯಾಗಮ is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_rules_str_token(ಸಂರಚನಾ scanner *s, ಧ್ರುವ ವರ್ಣ *string, size_t len)
{
    ಯದಿ (!scanner_str(s, string, len)) {
        ಪ್ರತ್ಯಾಗಮ false;
    }
    ಯದಿ (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        ಪ್ರತ್ಯಾಗಮ false;
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

#define scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), ಪರಿಮಾಣ(literal) - 1)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ size_t
scanner_rules_ident_token(ಸಂರಚನಾ scanner *s)
{
    size_t len = 0;
    ಯಾವತ್ (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    ಪ್ರತ್ಯಾಗಮ len;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_buf_append(ಸಂರಚನಾ scanner *s, ವರ್ಣ ch)
{
    ಯದಿ (s->buf_pos + 1 >= ಪರಿಮಾಣ(s->buf))
        ಪ್ರತ್ಯಾಗಮ false;
    s->buf[s->buf_pos++] = ch;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_buf_appends(ಸಂರಚನಾ scanner *s, ಧ್ರುವ ವರ್ಣ *str)
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ret = snprintf(s->buf + s->buf_pos,
                             ಪರಿಮಾಣ(s->buf) - s->buf_pos,
                             "%s", str);
    ಯದಿ (ret < 0 || (size_t)ret >= ಪರಿಮಾಣ(s->buf) - s->buf_pos)
        ಪ್ರತ್ಯಾಗಮ false;
    s->buf_pos += (size_t)ret;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_buf_appends_code_point(ಸಂರಚನಾ scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    ಯದಿ (s->buf_pos + 5 <= ಪರಿಮಾಣ(s->buf)) {
        uint8_t ಎಣಿಕೆ = utf32_to_utf8(c, s->buf + s->buf_pos);
        ಯದಿ (ಎಣಿಕೆ == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            ಎಣಿಕೆ = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        ಯದಿ (ಎಣಿಕೆ == 0)
            ಪ್ರತ್ಯಾಗಮ false;
        /* `count` counts the NULL byte */
        s->buf_pos += (size_t)ಎಣಿಕೆ - 1;
        ಪ್ರತ್ಯಾಗಮ true;
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ false;
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_oct(ಸಂರಚನಾ scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    ಪ್ರತಿ (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        ಯದಿ (c < 040) {
            c = c * 8 + (uint8_t)(scanner_next(s) - '0');
        } ಅನ್ಯಥಾ {
            /* Consume valid digit, but mark result as invalid */
            scanner_next(s);
            *out = c;
            ಪ್ರತ್ಯಾಗಮ false;
        }
    }
    *out = c;
    ಪ್ರತ್ಯಾಗಮ i > 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_hex(ಸಂರಚನಾ scanner *s, uint8_t *out)
{
    uint8_t i;
    ಪ್ರತಿ (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        ಧ್ರುವ ವರ್ಣ c = scanner_next(s);
        ಧ್ರುವ ವರ್ಣ offset = (ವರ್ಣ) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    ಪ್ರತ್ಯಾಗಮ i > 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
scanner_dec_int64(ಸಂರಚನಾ scanner *s, int64_t *out)
{
    uint64_t val = 0;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ಯದಿ (ಎಣಿಕೆ > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        ಯದಿ (val > INT64_MAX)
            ಪ್ರತ್ಯಾಗಮ -1;
        s->pos += (size_t)ಎಣಿಕೆ;
        *out = (int64_t)val;
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
scanner_hex_int64(ಸಂರಚನಾ scanner *s, int64_t *out)
{
    uint64_t val = 0;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ಯದಿ (ಎಣಿಕೆ > 0) {
        /* See comment in `scanner_dec_int64()` above */
        ಯದಿ (val > INT64_MAX)
            ಪ್ರತ್ಯಾಗಮ -1;
        s->pos += (size_t)ಎಣಿಕೆ;
        *out = (int64_t) val;
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_unicode_code_point(ಸಂರಚನಾ scanner *s, uint32_t *out)
{
    ಯದಿ (!scanner_chr(s, '{'))
        ಪ್ರತ್ಯಾಗಮ false;

    uint32_t cp = 0;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    ಯದಿ (ಎಣಿಕೆ > 0)
        s->pos += (size_t)ಎಣಿಕೆ;

    /* Try to consume everything within the string until the next `}` */
    ಧ್ರುವ size_t last_valid = s->pos;
    ಯಾವತ್ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_next(s);
    }

    ಯದಿ (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        ಪ್ರತ್ಯಾಗಮ (ಎಣಿಕೆ > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    ಪ್ರತ್ಯಾಗಮ false;
}

/* Basic detection of wrong character encoding based on the first bytes */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
scanner_check_supported_char_encoding(ಸಂರಚನಾ scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    ಯದಿ (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        ಪ್ರತ್ಯಾಗಮ true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    ಯದಿ (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        ಪ್ರತ್ಯಾಗಮ false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    ಯದಿ (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}
