/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-numbers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
ਬਣਤਰ sval {
    ਮਾਪ_t len;
    ਅਚਲ ਅੱਖਰ *ਸ਼ੁਰੂ ATTR_COUNTED_BY(len);
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray(ਬਣਤਰ sval) darray_sval;

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
svaleq(ਬਣਤਰ sval s1, ਬਣਤਰ sval s2)
{
    ਵਾਪਸ s1.len == s2.len && memcmp(s1.ਸ਼ੁਰੂ, s2.ਸ਼ੁਰੂ, s1.len) == 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
isvaleq(ਬਣਤਰ sval s1, ਬਣਤਰ sval s2)
{
    ਵਾਪਸ s1.len == s2.len && istrncmp(s1.ਸ਼ੁਰੂ, s2.ਸ਼ੁਰੂ, s1.len) == 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
svaleq_ਅਗੇਤਰ(ਬਣਤਰ sval s1, ਬਣਤਰ sval s2)
{
    ਵਾਪਸ s1.len <= s2.len && memcmp(s1.ਸ਼ੁਰੂ, s2.ਸ਼ੁਰੂ, s1.len) == 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SVAL(__start, __len) (ਬਣਤਰ sval){ .ਸ਼ੁਰੂ = (__start), .len = (__len) }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SVAL_LIT(literal) SVAL(literal, ਆਕਾਰ(literal) - 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SVAL_INIT(literal) { .ਸ਼ੁਰੂ = (literal), .len = ਆਕਾਰ(literal) - 1 }

/* A line:column location in the input string (1-based). */
ਬਣਤਰ scanner_loc {
    ਮਾਪ_t ਸਤਰ, ਕਾਲਮ;
};

ਬਣਤਰ scanner {
    ਮਾਪ_t pos;
    ਮਾਪ_t len;
    ਅਚਲ ਅੱਖਰ *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    ਅੱਖਰ buf[1024];
    ਮਾਪ_t buf_pos;
    /* The position of the start of the current token. */
    ਮਾਪ_t token_pos;
    /* Cached values used by scanner_token_location. */
    ਮਾਪ_t cached_pos;
    ਬਣਤਰ scanner_loc cached_loc;
    ਅਚਲ ਅੱਖਰ *file_name;
    ਬਣਤਰ xkb_context *ctx;
    ਖਾਲੀ *priv;
};

/* Compute the line:column location for the current token (slow). */
ਬਣਤਰ scanner_loc
scanner_token_location(ਬਣਤਰ scanner *s);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scanner_log_with_ਕੋਡ(scanner, level, verbosity, log_ਸੁਨੇਹਾ_id, fmt, ...) ਕਰੋ { \
    ਬਣਤਰ scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_ਕੋਡ((scanner)->ctx, (level), verbosity, log_ਸੁਨੇਹਾ_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.ਸਤਰ,                                                     \
                      loc.ਕਾਲਮ, ##__VA_ARGS__);                                   \
} ਜਦੋਂ(0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_ਕੋਡ(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_ਨਿਊਨਤਮ, id, fmt, ##__VA_ARGS__)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_ਕੋਡ(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_ਨਿਊਨਤਮ, id, fmt, ##__VA_ARGS__)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_ਕੋਡ(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
scanner_init(ਬਣਤਰ scanner *s, ਬਣਤਰ xkb_context *ctx,
             ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len, ਅਚਲ ਅੱਖਰ *file_name,
             ਖਾਲੀ *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.ਸਤਰ = s->cached_loc.ਕਾਲਮ = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ
scanner_peek(ਬਣਤਰ scanner *s)
{
    ਜੇ (unlikely(s->pos >= s->len))
        ਵਾਪਸ '\0';
    ਵਾਪਸ s->s[s->pos];
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_eof(ਬਣਤਰ scanner *s)
{
    ਵਾਪਸ s->pos >= s->len;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_eol(ਬਣਤਰ scanner *s)
{
    ਵਾਪਸ scanner_peek(s) == '\n';
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
scanner_skip_ਮੰਜ਼ਿਲ_eol(ਬਣਤਰ scanner *s)
{
    ਅਚਲ ਅੱਖਰ *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    ਅਚਲ ਮਾਪ_t new_pos = nl ? (ਮਾਪ_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅੱਖਰ
scanner_ਅਗਲਾ(ਬਣਤਰ scanner *s)
{
    ਜੇ (unlikely(scanner_eof(s)))
        ਵਾਪਸ '\0';
    ਵਾਪਸ s->s[s->pos++];
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_chr(ਬਣਤਰ scanner *s, ਅੱਖਰ ch)
{
    ਜੇ (likely(scanner_peek(s) != ch))
        ਵਾਪਸ false;
    s->pos++;
    ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_str(ਬਣਤਰ scanner *s, ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len)
{
    ਜੇ (s->len - s->pos < len)
        ਵਾਪਸ false;
    ਜੇ (memcmp(s->s + s->pos, string, len) != 0)
        ਵਾਪਸ false;
    s->pos += len;
    ਵਾਪਸ true;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scanner_lit(s, literal) scanner_str(s, literal, ਆਕਾਰ(literal) - 1)

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_ਨਿਯਮਾਂ_is_ident(ਅੱਖਰ ch)
{
    ਵਾਪਸ is_ਗ੍ਰਾਫ(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_ਨਿਯਮਾਂ_str_token(ਬਣਤਰ scanner *s, ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len)
{
    ਜੇ (!scanner_str(s, string, len)) {
        ਵਾਪਸ false;
    }
    ਜੇ (scanner_ਨਿਯਮਾਂ_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        ਵਾਪਸ false;
    }
    ਵਾਪਸ true;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scanner_ਨਿਯਮਾਂ_lit_token(s, literal) \
    scanner_ਨਿਯਮਾਂ_str_token((s), (literal), ਆਕਾਰ(literal) - 1)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਮਾਪ_t
scanner_ਨਿਯਮਾਂ_ident_token(ਬਣਤਰ scanner *s)
{
    ਮਾਪ_t len = 0;
    ਜਦੋਂ (!scanner_eof(s) && scanner_ਨਿਯਮਾਂ_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    ਵਾਪਸ len;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_buf_append(ਬਣਤਰ scanner *s, ਅੱਖਰ ch)
{
    ਜੇ (s->buf_pos + 1 >= ਆਕਾਰ(s->buf))
        ਵਾਪਸ false;
    s->buf[s->buf_pos++] = ch;
    ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_buf_appends(ਬਣਤਰ scanner *s, ਅਚਲ ਅੱਖਰ *str)
{
    ਅਚਲ ਪੂਰਨ ret = snprintf(s->buf + s->buf_pos,
                             ਆਕਾਰ(s->buf) - s->buf_pos,
                             "%s", str);
    ਜੇ (ret < 0 || (ਮਾਪ_t)ret >= ਆਕਾਰ(s->buf) - s->buf_pos)
        ਵਾਪਸ false;
    s->buf_pos += (ਮਾਪ_t)ret;
    ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_buf_appends_ਕੋਡ_ਬਿੰਦੂ(ਬਣਤਰ scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    ਜੇ (s->buf_pos + 5 <= ਆਕਾਰ(s->buf)) {
        uint8_t ਗਿਣਤੀ_2 = utf32_ਮੰਜ਼ਿਲ_utf8(c, s->buf + s->buf_pos);
        ਜੇ (ਗਿਣਤੀ_2 == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            ਗਿਣਤੀ_2 = utf32_ਮੰਜ਼ਿਲ_utf8(0xfffd, s->buf + s->buf_pos);
        }
        ਜੇ (ਗਿਣਤੀ_2 == 0)
            ਵਾਪਸ false;
        /* `count` counts the NULL byte */
        s->buf_pos += (ਮਾਪ_t)ਗਿਣਤੀ_2 - 1;
        ਵਾਪਸ true;
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ false;
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_oct(ਬਣਤਰ scanner *s, uint8_t *ਬਾਹਰਲਾ)
{
    uint8_t i = 0;
    uint8_t c = 0;
    ਲਈ (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        ਜੇ (c < 040) {
            c = c * 8 + (uint8_t)(scanner_ਅਗਲਾ(s) - '0');
        } ਨਹੀਂਤਾਂ {
            /* Consume valid digit, but mark result as invalid */
            scanner_ਅਗਲਾ(s);
            *ਬਾਹਰਲਾ = c;
            ਵਾਪਸ false;
        }
    }
    *ਬਾਹਰਲਾ = c;
    ਵਾਪਸ i > 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_hex(ਬਣਤਰ scanner *s, uint8_t *ਬਾਹਰਲਾ)
{
    uint8_t i;
    ਲਈ (i = 0, *ਬਾਹਰਲਾ = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        ਅਚਲ ਅੱਖਰ c = scanner_ਅਗਲਾ(s);
        ਅਚਲ ਅੱਖਰ ਵਿੱਥ = (ਅੱਖਰ) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *ਬਾਹਰਲਾ = *ਬਾਹਰਲਾ * 16 + (uint8_t)(c - ਵਿੱਥ);
    }
    ਵਾਪਸ i > 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
scanner_dec_int64(ਬਣਤਰ scanner *s, int64_t *ਬਾਹਰਲਾ)
{
    uint64_t val = 0;
    ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 =
        parse_dec_ਮੰਜ਼ਿਲ_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ਜੇ (ਗਿਣਤੀ_2 > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        ਜੇ (val > INT64_MAX)
            ਵਾਪਸ -1;
        s->pos += (ਮਾਪ_t)ਗਿਣਤੀ_2;
        *ਬਾਹਰਲਾ = (int64_t)val;
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
scanner_hex_int64(ਬਣਤਰ scanner *s, int64_t *ਬਾਹਰਲਾ)
{
    uint64_t val = 0;
    ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 =
        parse_hex_ਮੰਜ਼ਿਲ_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ਜੇ (ਗਿਣਤੀ_2 > 0) {
        /* See comment in `scanner_dec_int64()` above */
        ਜੇ (val > INT64_MAX)
            ਵਾਪਸ -1;
        s->pos += (ਮਾਪ_t)ਗਿਣਤੀ_2;
        *ਬਾਹਰਲਾ = (int64_t) val;
    }
    ਵਾਪਸ ਗਿਣਤੀ_2;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_unicode_ਕੋਡ_ਬਿੰਦੂ(ਬਣਤਰ scanner *s, uint32_t *ਬਾਹਰਲਾ)
{
    ਜੇ (!scanner_chr(s, '{'))
        ਵਾਪਸ false;

    uint32_t cp = 0;
    ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 =
        parse_hex_ਮੰਜ਼ਿਲ_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    ਜੇ (ਗਿਣਤੀ_2 > 0)
        s->pos += (ਮਾਪ_t)ਗਿਣਤੀ_2;

    /* Try to consume everything within the string until the next `}` */
    ਅਚਲ ਮਾਪ_t ਆਖਰੀ_valid = s->pos;
    ਜਦੋਂ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_ਅਗਲਾ(s);
    }

    ਜੇ (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *ਬਾਹਰਲਾ = cp;
        ਵਾਪਸ (ਗਿਣਤੀ_2 > 0 && s->pos == ਆਖਰੀ_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = ਆਖਰੀ_valid;
    ਵਾਪਸ false;
}

/* Basic detection of wrong character encoding based on the first bytes */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
scanner_check_supported_char_encoding(ਬਣਤਰ scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    ਜੇ (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        ਵਾਪਸ true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    ਜੇ (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        ਵਾਪਸ false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    ਜੇ (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        ਵਾਪਸ false;
    }

    ਵਾਪਸ true;
}
