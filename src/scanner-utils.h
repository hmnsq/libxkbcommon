/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ضمن "config.h"

#ضمن <stdbool.h>
#ضمن <stddef.h>
#ضمن <stdint.h>
#ضمن <string.h>

#ضمن "context.h"
#ضمن "darray.h"
#ضمن "messages-codes.h"
#ضمن "utils.h"
#ضمن "util-numbers.h"
#ضمن "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
بنية sval {
    size_t len;
    ثابت حرف *start ATTR_COUNTED_BY(len);
};
تعريفنوع darray(بنية sval) darray_sval;

ساكن مضمن bool
svaleq(بنية sval s1, بنية sval s2)
{
    ܦܢܐ s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

ساكن مضمن bool
isvaleq(بنية sval s1, بنية sval s2)
{
    ܦܢܐ s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

ساكن مضمن bool
svaleq_prefix(بنية sval s1, بنية sval s2)
{
    ܦܢܐ s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#define SVAL(__start, __len) (بنية sval){ .start = (__start), .len = (__len) }
#define SVAL_LIT(literal) SVAL(literal, حجم(literal) - 1)
#define SVAL_INIT(literal) { .start = (literal), .len = حجم(literal) - 1 }

/* A line:column location in the input string (1-based). */
بنية scanner_loc {
    size_t line, column;
};

بنية scanner {
    size_t pos;
    size_t len;
    ثابت حرف *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    حرف buf[1024];
    size_t buf_pos;
    /* The position of the start of the current token. */
    size_t token_pos;
    /* Cached values used by scanner_token_location. */
    size_t cached_pos;
    بنية scanner_loc cached_loc;
    ثابت حرف *file_name;
    بنية xkb_context *ctx;
    فراغ *priv;
};

/* Compute the line:column location for the current token (slow). */
بنية scanner_loc
scanner_token_location(بنية scanner *s);

#define scanner_log_with_code(scanner, level, verbosity, log_msg_id, fmt, ...) ܥܒܕ { \
    بنية scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_msg_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.line,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} ܟܡܐ(0)

#define scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

ساكن مضمن فراغ
scanner_init(بنية scanner *s, بنية xkb_context *ctx,
             ثابت حرف *string, size_t len, ثابت حرف *file_name,
             فراغ *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.line = s->cached_loc.column = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

ساكن مضمن حرف
scanner_peek(بنية scanner *s)
{
    ܐܢ (unlikely(s->pos >= s->len))
        ܦܢܐ '\0';
    ܦܢܐ s->s[s->pos];
}

ساكن مضمن bool
scanner_eof(بنية scanner *s)
{
    ܦܢܐ s->pos >= s->len;
}

ساكن مضمن bool
scanner_eol(بنية scanner *s)
{
    ܦܢܐ scanner_peek(s) == '\n';
}

ساكن مضمن فراغ
scanner_skip_to_eol(بنية scanner *s)
{
    ثابت حرف *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    ثابت size_t new_pos = nl ? (size_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

ساكن مضمن حرف
scanner_next(بنية scanner *s)
{
    ܐܢ (unlikely(scanner_eof(s)))
        ܦܢܐ '\0';
    ܦܢܐ s->s[s->pos++];
}

ساكن مضمن bool
scanner_chr(بنية scanner *s, حرف ch)
{
    ܐܢ (likely(scanner_peek(s) != ch))
        ܦܢܐ false;
    s->pos++;
    ܦܢܐ true;
}

ساكن مضمن bool
scanner_str(بنية scanner *s, ثابت حرف *string, size_t len)
{
    ܐܢ (s->len - s->pos < len)
        ܦܢܐ false;
    ܐܢ (memcmp(s->s + s->pos, string, len) != 0)
        ܦܢܐ false;
    s->pos += len;
    ܦܢܐ true;
}

#define scanner_lit(s, literal) scanner_str(s, literal, حجم(literal) - 1)

ساكن مضمن bool
scanner_rules_is_ident(حرف ch)
{
    ܦܢܐ is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
ساكن مضمن bool
scanner_rules_str_token(بنية scanner *s, ثابت حرف *string, size_t len)
{
    ܐܢ (!scanner_str(s, string, len)) {
        ܦܢܐ false;
    }
    ܐܢ (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        ܦܢܐ false;
    }
    ܦܢܐ true;
}

#define scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), حجم(literal) - 1)

ساكن مضمن size_t
scanner_rules_ident_token(بنية scanner *s)
{
    size_t len = 0;
    ܟܡܐ (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    ܦܢܐ len;
}

ساكن مضمن bool
scanner_buf_append(بنية scanner *s, حرف ch)
{
    ܐܢ (s->buf_pos + 1 >= حجم(s->buf))
        ܦܢܐ false;
    s->buf[s->buf_pos++] = ch;
    ܦܢܐ true;
}

ساكن مضمن bool
scanner_buf_appends(بنية scanner *s, ثابت حرف *str)
{
    ثابت صحيح ret = snprintf(s->buf + s->buf_pos,
                             حجم(s->buf) - s->buf_pos,
                             "%s", str);
    ܐܢ (ret < 0 || (size_t)ret >= حجم(s->buf) - s->buf_pos)
        ܦܢܐ false;
    s->buf_pos += (size_t)ret;
    ܦܢܐ true;
}

ساكن مضمن bool
scanner_buf_appends_code_point(بنية scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    ܐܢ (s->buf_pos + 5 <= حجم(s->buf)) {
        uint8_t count = utf32_to_utf8(c, s->buf + s->buf_pos);
        ܐܢ (count == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            count = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        ܐܢ (count == 0)
            ܦܢܐ false;
        /* `count` counts the NULL byte */
        s->buf_pos += (size_t)count - 1;
        ܦܢܐ true;
    } ܐܚܪܢܐ {
        ܦܢܐ false;
    }
}

ساكن مضمن bool
scanner_oct(بنية scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    لكل (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        ܐܢ (c < 040) {
            c = c * 8 + (uint8_t)(scanner_next(s) - '0');
        } ܐܚܪܢܐ {
            /* Consume valid digit, but mark result as invalid */
            scanner_next(s);
            *out = c;
            ܦܢܐ false;
        }
    }
    *out = c;
    ܦܢܐ i > 0;
}

ساكن مضمن bool
scanner_hex(بنية scanner *s, uint8_t *out)
{
    uint8_t i;
    لكل (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        ثابت حرف c = scanner_next(s);
        ثابت حرف offset = (حرف) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    ܦܢܐ i > 0;
}

ساكن مضمن صحيح
scanner_dec_int64(بنية scanner *s, int64_t *out)
{
    uint64_t val = 0;
    ثابت صحيح count =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ܐܢ (count > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        ܐܢ (val > INT64_MAX)
            ܦܢܐ -1;
        s->pos += (size_t)count;
        *out = (int64_t)val;
    }
    ܦܢܐ count;
}

ساكن مضمن صحيح
scanner_hex_int64(بنية scanner *s, int64_t *out)
{
    uint64_t val = 0;
    ثابت صحيح count =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ܐܢ (count > 0) {
        /* See comment in `scanner_dec_int64()` above */
        ܐܢ (val > INT64_MAX)
            ܦܢܐ -1;
        s->pos += (size_t)count;
        *out = (int64_t) val;
    }
    ܦܢܐ count;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
ساكن مضمن bool
scanner_unicode_code_point(بنية scanner *s, uint32_t *out)
{
    ܐܢ (!scanner_chr(s, '{'))
        ܦܢܐ false;

    uint32_t cp = 0;
    ثابت صحيح count =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    ܐܢ (count > 0)
        s->pos += (size_t)count;

    /* Try to consume everything within the string until the next `}` */
    ثابت size_t last_valid = s->pos;
    ܟܡܐ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_next(s);
    }

    ܐܢ (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        ܦܢܐ (count > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    ܦܢܐ false;
}

/* Basic detection of wrong character encoding based on the first bytes */
ساكن مضمن bool
scanner_check_supported_char_encoding(بنية scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    ܐܢ (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        ܦܢܐ true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    ܐܢ (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        ܦܢܐ false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    ܐܢ (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        ܦܢܐ false;
    }

    ܦܢܐ true;
}
