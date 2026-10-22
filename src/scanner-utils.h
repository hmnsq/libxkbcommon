/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdbool.h>
#அந்தர்பாவ <stddef.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "context.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "util-numbers.h"
#அந்தர்பாவ "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
கட்டமைப்பு sval {
    size_t len;
    மாறிலி எழுத்து *start ATTR_COUNTED_BY(len);
};
வகைவரை darray(கட்டமைப்பு sval) darray_sval;

நிலையான உள்வரி bool
svaleq(கட்டமைப்பு sval s1, கட்டமைப்பு sval s2)
{
    திரும்பு s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

நிலையான உள்வரி bool
isvaleq(கட்டமைப்பு sval s1, கட்டமைப்பு sval s2)
{
    திரும்பு s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

நிலையான உள்வரி bool
svaleq_prefix(கட்டமைப்பு sval s1, கட்டமைப்பு sval s2)
{
    திரும்பு s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#define SVAL(__start, __len) (கட்டமைப்பு sval){ .start = (__start), .len = (__len) }
#define SVAL_LIT(literal) SVAL(literal, அளவு(literal) - 1)
#define SVAL_INIT(literal) { .start = (literal), .len = அளவு(literal) - 1 }

/* A line:column location in the input string (1-based). */
கட்டமைப்பு scanner_loc {
    size_t வரி, column;
};

கட்டமைப்பு scanner {
    size_t pos;
    size_t len;
    மாறிலி எழுத்து *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    எழுத்து buf[1024];
    size_t buf_pos;
    /* The position of the start of the current token. */
    size_t token_pos;
    /* Cached values used by scanner_token_location. */
    size_t cached_pos;
    கட்டமைப்பு scanner_loc cached_loc;
    மாறிலி எழுத்து *file_name;
    கட்டமைப்பு xkb_context *ctx;
    வெறுமை *priv;
};

/* Compute the line:column location for the current token (slow). */
கட்டமைப்பு scanner_loc
scanner_token_location(கட்டமைப்பு scanner *s);

#define scanner_log_with_code(scanner, level, verbosity, log_செய்தி_id, fmt, ...) செய் { \
    கட்டமைப்பு scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_செய்தி_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.வரி,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} போது(0)

#define scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

நிலையான உள்வரி வெறுமை
scanner_init(கட்டமைப்பு scanner *s, கட்டமைப்பு xkb_context *ctx,
             மாறிலி எழுத்து *string, size_t len, மாறிலி எழுத்து *file_name,
             வெறுமை *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.வரி = s->cached_loc.column = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

நிலையான உள்வரி எழுத்து
scanner_peek(கட்டமைப்பு scanner *s)
{
    எனில் (unlikely(s->pos >= s->len))
        திரும்பு '\0';
    திரும்பு s->s[s->pos];
}

நிலையான உள்வரி bool
scanner_eof(கட்டமைப்பு scanner *s)
{
    திரும்பு s->pos >= s->len;
}

நிலையான உள்வரி bool
scanner_eol(கட்டமைப்பு scanner *s)
{
    திரும்பு scanner_peek(s) == '\n';
}

நிலையான உள்வரி வெறுமை
scanner_skip_to_eol(கட்டமைப்பு scanner *s)
{
    மாறிலி எழுத்து *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    மாறிலி size_t new_pos = nl ? (size_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

நிலையான உள்வரி எழுத்து
scanner_next(கட்டமைப்பு scanner *s)
{
    எனில் (unlikely(scanner_eof(s)))
        திரும்பு '\0';
    திரும்பு s->s[s->pos++];
}

நிலையான உள்வரி bool
scanner_chr(கட்டமைப்பு scanner *s, எழுத்து ch)
{
    எனில் (likely(scanner_peek(s) != ch))
        திரும்பு false;
    s->pos++;
    திரும்பு true;
}

நிலையான உள்வரி bool
scanner_str(கட்டமைப்பு scanner *s, மாறிலி எழுத்து *string, size_t len)
{
    எனில் (s->len - s->pos < len)
        திரும்பு false;
    எனில் (memcmp(s->s + s->pos, string, len) != 0)
        திரும்பு false;
    s->pos += len;
    திரும்பு true;
}

#define scanner_lit(s, literal) scanner_str(s, literal, அளவு(literal) - 1)

நிலையான உள்வரி bool
scanner_rules_is_ident(எழுத்து ch)
{
    திரும்பு is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
நிலையான உள்வரி bool
scanner_rules_str_token(கட்டமைப்பு scanner *s, மாறிலி எழுத்து *string, size_t len)
{
    எனில் (!scanner_str(s, string, len)) {
        திரும்பு false;
    }
    எனில் (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        திரும்பு false;
    }
    திரும்பு true;
}

#define scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), அளவு(literal) - 1)

நிலையான உள்வரி size_t
scanner_rules_ident_token(கட்டமைப்பு scanner *s)
{
    size_t len = 0;
    போது (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    திரும்பு len;
}

நிலையான உள்வரி bool
scanner_buf_append(கட்டமைப்பு scanner *s, எழுத்து ch)
{
    எனில் (s->buf_pos + 1 >= அளவு(s->buf))
        திரும்பு false;
    s->buf[s->buf_pos++] = ch;
    திரும்பு true;
}

நிலையான உள்வரி bool
scanner_buf_appends(கட்டமைப்பு scanner *s, மாறிலி எழுத்து *str)
{
    மாறிலி முழு ret = snprintf(s->buf + s->buf_pos,
                             அளவு(s->buf) - s->buf_pos,
                             "%s", str);
    எனில் (ret < 0 || (size_t)ret >= அளவு(s->buf) - s->buf_pos)
        திரும்பு false;
    s->buf_pos += (size_t)ret;
    திரும்பு true;
}

நிலையான உள்வரி bool
scanner_buf_appends_code_point(கட்டமைப்பு scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    எனில் (s->buf_pos + 5 <= அளவு(s->buf)) {
        uint8_t மொத்தம் = utf32_to_utf8(c, s->buf + s->buf_pos);
        எனில் (மொத்தம் == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            மொத்தம் = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        எனில் (மொத்தம் == 0)
            திரும்பு false;
        /* `count` counts the NULL byte */
        s->buf_pos += (size_t)மொத்தம் - 1;
        திரும்பு true;
    } இல்லையெனில் {
        திரும்பு false;
    }
}

நிலையான உள்வரி bool
scanner_oct(கட்டமைப்பு scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    க்கு (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        எனில் (c < 040) {
            c = c * 8 + (uint8_t)(scanner_next(s) - '0');
        } இல்லையெனில் {
            /* Consume valid digit, but mark result as invalid */
            scanner_next(s);
            *out = c;
            திரும்பு false;
        }
    }
    *out = c;
    திரும்பு i > 0;
}

நிலையான உள்வரி bool
scanner_hex(கட்டமைப்பு scanner *s, uint8_t *out)
{
    uint8_t i;
    க்கு (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        மாறிலி எழுத்து c = scanner_next(s);
        மாறிலி எழுத்து offset = (எழுத்து) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    திரும்பு i > 0;
}

நிலையான உள்வரி முழு
scanner_dec_int64(கட்டமைப்பு scanner *s, int64_t *out)
{
    uint64_t val = 0;
    மாறிலி முழு மொத்தம் =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    எனில் (மொத்தம் > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        எனில் (val > INT64_MAX)
            திரும்பு -1;
        s->pos += (size_t)மொத்தம்;
        *out = (int64_t)val;
    }
    திரும்பு மொத்தம்;
}

நிலையான உள்வரி முழு
scanner_hex_int64(கட்டமைப்பு scanner *s, int64_t *out)
{
    uint64_t val = 0;
    மாறிலி முழு மொத்தம் =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    எனில் (மொத்தம் > 0) {
        /* See comment in `scanner_dec_int64()` above */
        எனில் (val > INT64_MAX)
            திரும்பு -1;
        s->pos += (size_t)மொத்தம்;
        *out = (int64_t) val;
    }
    திரும்பு மொத்தம்;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
நிலையான உள்வரி bool
scanner_unicode_code_point(கட்டமைப்பு scanner *s, uint32_t *out)
{
    எனில் (!scanner_chr(s, '{'))
        திரும்பு false;

    uint32_t cp = 0;
    மாறிலி முழு மொத்தம் =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    எனில் (மொத்தம் > 0)
        s->pos += (size_t)மொத்தம்;

    /* Try to consume everything within the string until the next `}` */
    மாறிலி size_t last_valid = s->pos;
    போது (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_next(s);
    }

    எனில் (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        திரும்பு (மொத்தம் > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    திரும்பு false;
}

/* Basic detection of wrong character encoding based on the first bytes */
நிலையான உள்வரி bool
scanner_check_supported_char_encoding(கட்டமைப்பு scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    எனில் (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        திரும்பு true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    எனில் (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        திரும்பு false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    எனில் (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        திரும்பு false;
    }

    திரும்பு true;
}
