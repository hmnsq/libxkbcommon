/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "context.h"
#include "darray.h"
#include "messages-codes.h"
#include "utils.h"
#include "util-numbers.h"
#include "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
sa_nracanaa sval {
    size_t len;
    const var_na *start ATTR_COUNTED_BY(len);
};
typedef darray(sa_nracanaa sval) darray_sval;

static anta_hstha bool
svaleq(sa_nracanaa sval s1, sa_nracanaa sval s2)
{
    ပြန်ပေး s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

static anta_hstha bool
isvaleq(sa_nracanaa sval s1, sa_nracanaa sval s2)
{
    ပြန်ပေး s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

static anta_hstha bool
svaleq_prefix(sa_nracanaa sval s1, sa_nracanaa sval s2)
{
    ပြန်ပေး s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#define SVAL(__start, __len) (sa_nracanaa sval){ .start = (__start), .len = (__len) }
#define SVAL_LIT(literal) SVAL(literal, parimaa_na(literal) - 1)
#define SVAL_INIT(literal) { .start = (literal), .len = parimaa_na(literal) - 1 }

/* A line:column location in the input string (1-based). */
sa_nracanaa scanner_loc {
    size_t line, column;
};

sa_nracanaa scanner {
    size_t pos;
    size_t len;
    const var_na *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    var_na buf[1024];
    size_t buf_pos;
    /* The position of the start of the current token. */
    size_t token_pos;
    /* Cached values used by scanner_token_location. */
    size_t cached_pos;
    sa_nracanaa scanner_loc cached_loc;
    const var_na *file_name;
    sa_nracanaa xkb_context *ctx;
    void *priv;
};

/* Compute the line:column location for the current token (slow). */
sa_nracanaa scanner_loc
scanner_token_location(sa_nracanaa scanner *s);

#define scanner_log_with_code(scanner, level, verbosity, log_msg_id, fmt, ...) do { \
    sa_nracanaa scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_msg_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.line,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} နေစဉ်(0)

#define scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

static anta_hstha void
scanner_init(sa_nracanaa scanner *s, sa_nracanaa xkb_context *ctx,
             const var_na *string, size_t len, const var_na *file_name,
             void *priv)
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

static anta_hstha var_na
scanner_peek(sa_nracanaa scanner *s)
{
    အကယ်၍ (unlikely(s->pos >= s->len))
        ပြန်ပေး '\0';
    ပြန်ပေး s->s[s->pos];
}

static anta_hstha bool
scanner_eof(sa_nracanaa scanner *s)
{
    ပြန်ပေး s->pos >= s->len;
}

static anta_hstha bool
scanner_eol(sa_nracanaa scanner *s)
{
    ပြန်ပေး scanner_peek(s) == '\n';
}

static anta_hstha void
scanner_skip_to_eol(sa_nracanaa scanner *s)
{
    const var_na *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    const size_t new_pos = nl ? (size_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

static anta_hstha var_na
scanner_next(sa_nracanaa scanner *s)
{
    အကယ်၍ (unlikely(scanner_eof(s)))
        ပြန်ပေး '\0';
    ပြန်ပေး s->s[s->pos++];
}

static anta_hstha bool
scanner_chr(sa_nracanaa scanner *s, var_na ch)
{
    အကယ်၍ (likely(scanner_peek(s) != ch))
        ပြန်ပေး false;
    s->pos++;
    ပြန်ပေး true;
}

static anta_hstha bool
scanner_str(sa_nracanaa scanner *s, const var_na *string, size_t len)
{
    အကယ်၍ (s->len - s->pos < len)
        ပြန်ပေး false;
    အကယ်၍ (memcmp(s->s + s->pos, string, len) != 0)
        ပြန်ပေး false;
    s->pos += len;
    ပြန်ပေး true;
}

#define scanner_lit(s, literal) scanner_str(s, literal, parimaa_na(literal) - 1)

static anta_hstha bool
scanner_rules_is_ident(var_na ch)
{
    ပြန်ပေး is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
static anta_hstha bool
scanner_rules_str_token(sa_nracanaa scanner *s, const var_na *string, size_t len)
{
    အကယ်၍ (!scanner_str(s, string, len)) {
        ပြန်ပေး false;
    }
    အကယ်၍ (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        ပြန်ပေး false;
    }
    ပြန်ပေး true;
}

#define scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), parimaa_na(literal) - 1)

static anta_hstha size_t
scanner_rules_ident_token(sa_nracanaa scanner *s)
{
    size_t len = 0;
    နေစဉ် (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    ပြန်ပေး len;
}

static anta_hstha bool
scanner_buf_append(sa_nracanaa scanner *s, var_na ch)
{
    အကယ်၍ (s->buf_pos + 1 >= parimaa_na(s->buf))
        ပြန်ပေး false;
    s->buf[s->buf_pos++] = ch;
    ပြန်ပေး true;
}

static anta_hstha bool
scanner_buf_appends(sa_nracanaa scanner *s, const var_na *str)
{
    const ကိန်းပြည့် ret = snprintf(s->buf + s->buf_pos,
                             parimaa_na(s->buf) - s->buf_pos,
                             "%s", str);
    အကယ်၍ (ret < 0 || (size_t)ret >= parimaa_na(s->buf) - s->buf_pos)
        ပြန်ပေး false;
    s->buf_pos += (size_t)ret;
    ပြန်ပေး true;
}

static anta_hstha bool
scanner_buf_appends_code_point(sa_nracanaa scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    အကယ်၍ (s->buf_pos + 5 <= parimaa_na(s->buf)) {
        uint8_t count = utf32_to_utf8(c, s->buf + s->buf_pos);
        အကယ်၍ (count == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            count = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        အကယ်၍ (count == 0)
            ပြန်ပေး false;
        /* `count` counts the NULL byte */
        s->buf_pos += (size_t)count - 1;
        ပြန်ပေး true;
    } မဟုတ်ပါက {
        ပြန်ပေး false;
    }
}

static anta_hstha bool
scanner_oct(sa_nracanaa scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    အတွက် (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        အကယ်၍ (c < 040) {
            c = c * 8 + (uint8_t)(scanner_next(s) - '0');
        } မဟုတ်ပါက {
            /* Consume valid digit, but mark result as invalid */
            scanner_next(s);
            *out = c;
            ပြန်ပေး false;
        }
    }
    *out = c;
    ပြန်ပေး i > 0;
}

static anta_hstha bool
scanner_hex(sa_nracanaa scanner *s, uint8_t *out)
{
    uint8_t i;
    အတွက် (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        const var_na c = scanner_next(s);
        const var_na offset = (var_na) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    ပြန်ပေး i > 0;
}

static anta_hstha ကိန်းပြည့်
scanner_dec_int64(sa_nracanaa scanner *s, int64_t *out)
{
    uint64_t val = 0;
    const ကိန်းပြည့် count =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    အကယ်၍ (count > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        အကယ်၍ (val > INT64_MAX)
            ပြန်ပေး -1;
        s->pos += (size_t)count;
        *out = (int64_t)val;
    }
    ပြန်ပေး count;
}

static anta_hstha ကိန်းပြည့်
scanner_hex_int64(sa_nracanaa scanner *s, int64_t *out)
{
    uint64_t val = 0;
    const ကိန်းပြည့် count =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    အကယ်၍ (count > 0) {
        /* See comment in `scanner_dec_int64()` above */
        အကယ်၍ (val > INT64_MAX)
            ပြန်ပေး -1;
        s->pos += (size_t)count;
        *out = (int64_t) val;
    }
    ပြန်ပေး count;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
static anta_hstha bool
scanner_unicode_code_point(sa_nracanaa scanner *s, uint32_t *out)
{
    အကယ်၍ (!scanner_chr(s, '{'))
        ပြန်ပေး false;

    uint32_t cp = 0;
    const ကိန်းပြည့် count =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    အကယ်၍ (count > 0)
        s->pos += (size_t)count;

    /* Try to consume everything within the string until the next `}` */
    const size_t last_valid = s->pos;
    နေစဉ် (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_next(s);
    }

    အကယ်၍ (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        ပြန်ပေး (count > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    ပြန်ပေး false;
}

/* Basic detection of wrong character encoding based on the first bytes */
static anta_hstha bool
scanner_check_supported_char_encoding(sa_nracanaa scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    အကယ်၍ (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        ပြန်ပေး true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    အကယ်၍ (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        ပြန်ပေး false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    အကယ်၍ (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        ပြန်ပေး false;
    }

    ပြန်ပေး true;
}
