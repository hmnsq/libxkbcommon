/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#شامل "config.h"

#شامل <stdbool.h>
#شامل <stddef.h>
#شامل <stdint.h>
#شامل <string.h>

#شامل "context.h"
#شامل "darray.h"
#شامل "messages-codes.h"
#شامل "utils.h"
#شامل "util-numbers.h"
#شامل "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
ساختار sval {
    size_t len;
    ثابت نویسه *start ATTR_COUNTED_BY(len);
};
تعریفنوع darray(ساختار sval) darray_sval;

ایستا درونخطی bool
svaleq(ساختار sval s1, ساختار sval s2)
{
    بازگشت s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

ایستا درونخطی bool
isvaleq(ساختار sval s1, ساختار sval s2)
{
    بازگشت s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

ایستا درونخطی bool
svaleq_prefix(ساختار sval s1, ساختار sval s2)
{
    بازگشت s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#define SVAL(__start, __len) (ساختار sval){ .start = (__start), .len = (__len) }
#define SVAL_LIT(literal) SVAL(literal, اندازه(literal) - 1)
#define SVAL_INIT(literal) { .start = (literal), .len = اندازه(literal) - 1 }

/* A line:column location in the input string (1-based). */
ساختار scanner_loc {
    size_t سطر, column;
};

ساختار scanner {
    size_t pos;
    size_t len;
    ثابت نویسه *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    نویسه buf[1024];
    size_t buf_pos;
    /* The position of the start of the current token. */
    size_t token_pos;
    /* Cached values used by scanner_token_location. */
    size_t cached_pos;
    ساختار scanner_loc cached_loc;
    ثابت نویسه *file_name;
    ساختار xkb_context *ctx;
    تهی *priv;
};

/* Compute the line:column location for the current token (slow). */
ساختار scanner_loc
scanner_token_location(ساختار scanner *s);

#define scanner_log_with_code(scanner, level, verbosity, log_پیام_id, fmt, ...) انجامبده { \
    ساختار scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_پیام_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.سطر,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} وقتی(0)

#define scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

ایستا درونخطی تهی
scanner_init(ساختار scanner *s, ساختار xkb_context *ctx,
             ثابت نویسه *string, size_t len, ثابت نویسه *file_name,
             تهی *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.سطر = s->cached_loc.column = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

ایستا درونخطی نویسه
scanner_peek(ساختار scanner *s)
{
    اگر (unlikely(s->pos >= s->len))
        بازگشت '\0';
    بازگشت s->s[s->pos];
}

ایستا درونخطی bool
scanner_eof(ساختار scanner *s)
{
    بازگشت s->pos >= s->len;
}

ایستا درونخطی bool
scanner_eol(ساختار scanner *s)
{
    بازگشت scanner_peek(s) == '\n';
}

ایستا درونخطی تهی
scanner_skip_to_eol(ساختار scanner *s)
{
    ثابت نویسه *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    ثابت size_t new_pos = nl ? (size_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

ایستا درونخطی نویسه
scanner_next(ساختار scanner *s)
{
    اگر (unlikely(scanner_eof(s)))
        بازگشت '\0';
    بازگشت s->s[s->pos++];
}

ایستا درونخطی bool
scanner_chr(ساختار scanner *s, نویسه ch)
{
    اگر (likely(scanner_peek(s) != ch))
        بازگشت false;
    s->pos++;
    بازگشت true;
}

ایستا درونخطی bool
scanner_str(ساختار scanner *s, ثابت نویسه *string, size_t len)
{
    اگر (s->len - s->pos < len)
        بازگشت false;
    اگر (memcmp(s->s + s->pos, string, len) != 0)
        بازگشت false;
    s->pos += len;
    بازگشت true;
}

#define scanner_lit(s, literal) scanner_str(s, literal, اندازه(literal) - 1)

ایستا درونخطی bool
scanner_rules_is_ident(نویسه ch)
{
    بازگشت is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
ایستا درونخطی bool
scanner_rules_str_token(ساختار scanner *s, ثابت نویسه *string, size_t len)
{
    اگر (!scanner_str(s, string, len)) {
        بازگشت false;
    }
    اگر (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        بازگشت false;
    }
    بازگشت true;
}

#define scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), اندازه(literal) - 1)

ایستا درونخطی size_t
scanner_rules_ident_token(ساختار scanner *s)
{
    size_t len = 0;
    وقتی (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    بازگشت len;
}

ایستا درونخطی bool
scanner_buf_append(ساختار scanner *s, نویسه ch)
{
    اگر (s->buf_pos + 1 >= اندازه(s->buf))
        بازگشت false;
    s->buf[s->buf_pos++] = ch;
    بازگشت true;
}

ایستا درونخطی bool
scanner_buf_appends(ساختار scanner *s, ثابت نویسه *str)
{
    ثابت صحیح ret = snprintf(s->buf + s->buf_pos,
                             اندازه(s->buf) - s->buf_pos,
                             "%s", str);
    اگر (ret < 0 || (size_t)ret >= اندازه(s->buf) - s->buf_pos)
        بازگشت false;
    s->buf_pos += (size_t)ret;
    بازگشت true;
}

ایستا درونخطی bool
scanner_buf_appends_code_point(ساختار scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    اگر (s->buf_pos + 5 <= اندازه(s->buf)) {
        uint8_t شمار = utf32_to_utf8(c, s->buf + s->buf_pos);
        اگر (شمار == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            شمار = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        اگر (شمار == 0)
            بازگشت false;
        /* `count` counts the NULL byte */
        s->buf_pos += (size_t)شمار - 1;
        بازگشت true;
    } وگرنه {
        بازگشت false;
    }
}

ایستا درونخطی bool
scanner_oct(ساختار scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    برای (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        اگر (c < 040) {
            c = c * 8 + (uint8_t)(scanner_next(s) - '0');
        } وگرنه {
            /* Consume valid digit, but mark result as invalid */
            scanner_next(s);
            *out = c;
            بازگشت false;
        }
    }
    *out = c;
    بازگشت i > 0;
}

ایستا درونخطی bool
scanner_hex(ساختار scanner *s, uint8_t *out)
{
    uint8_t i;
    برای (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        ثابت نویسه c = scanner_next(s);
        ثابت نویسه offset = (نویسه) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    بازگشت i > 0;
}

ایستا درونخطی صحیح
scanner_dec_int64(ساختار scanner *s, int64_t *out)
{
    uint64_t val = 0;
    ثابت صحیح شمار =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    اگر (شمار > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        اگر (val > INT64_MAX)
            بازگشت -1;
        s->pos += (size_t)شمار;
        *out = (int64_t)val;
    }
    بازگشت شمار;
}

ایستا درونخطی صحیح
scanner_hex_int64(ساختار scanner *s, int64_t *out)
{
    uint64_t val = 0;
    ثابت صحیح شمار =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    اگر (شمار > 0) {
        /* See comment in `scanner_dec_int64()` above */
        اگر (val > INT64_MAX)
            بازگشت -1;
        s->pos += (size_t)شمار;
        *out = (int64_t) val;
    }
    بازگشت شمار;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
ایستا درونخطی bool
scanner_unicode_code_point(ساختار scanner *s, uint32_t *out)
{
    اگر (!scanner_chr(s, '{'))
        بازگشت false;

    uint32_t cp = 0;
    ثابت صحیح شمار =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    اگر (شمار > 0)
        s->pos += (size_t)شمار;

    /* Try to consume everything within the string until the next `}` */
    ثابت size_t last_valid = s->pos;
    وقتی (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_next(s);
    }

    اگر (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        بازگشت (شمار > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    بازگشت false;
}

/* Basic detection of wrong character encoding based on the first bytes */
ایستا درونخطی bool
scanner_check_supported_char_encoding(ساختار scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    اگر (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        بازگشت true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    اگر (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        بازگشت false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    اگر (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        بازگشت false;
    }

    بازگشت true;
}
