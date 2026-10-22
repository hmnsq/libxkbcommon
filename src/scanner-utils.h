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
struct sval {
    size_t len;
    const char *start ATTR_COUNTED_BY(len);
};
typedef darray(struct sval) darray_sval;

static inline bool
svaleq(struct sval s1, struct sval s2)
{
    върни s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

static inline bool
isvaleq(struct sval s1, struct sval s2)
{
    върни s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

static inline bool
svaleq_prefix(struct sval s1, struct sval s2)
{
    върни s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#define SVAL(__start, __len) (struct sval){ .start = (__start), .len = (__len) }
#define SVAL_LIT(literal) SVAL(literal, sizeof(literal) - 1)
#define SVAL_INIT(literal) { .start = (literal), .len = sizeof(literal) - 1 }

/* A line:column location in the input string (1-based). */
struct scanner_loc {
    size_t line, column;
};

struct scanner {
    size_t pos;
    size_t len;
    const char *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    char buf[1024];
    size_t buf_pos;
    /* The position of the start of the current token. */
    size_t token_pos;
    /* Cached values used by scanner_token_location. */
    size_t cached_pos;
    struct scanner_loc cached_loc;
    const char *file_name;
    struct xkb_context *ctx;
    void *priv;
};

/* Compute the line:column location for the current token (slow). */
struct scanner_loc
scanner_token_location(struct scanner *s);

#define scanner_log_with_code(scanner, level, verbosity, log_msg_id, fmt, ...) do { \
    struct scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_msg_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.line,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} докато(0)

#define scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

static inline void
scanner_init(struct scanner *s, struct xkb_context *ctx,
             const char *string, size_t len, const char *file_name,
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

static inline char
scanner_peek(struct scanner *s)
{
    ако (unlikely(s->pos >= s->len))
        върни '\0';
    върни s->s[s->pos];
}

static inline bool
scanner_eof(struct scanner *s)
{
    върни s->pos >= s->len;
}

static inline bool
scanner_eol(struct scanner *s)
{
    върни scanner_peek(s) == '\n';
}

static inline void
scanner_skip_to_eol(struct scanner *s)
{
    const char *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    const size_t new_pos = nl ? (size_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

static inline char
scanner_next(struct scanner *s)
{
    ако (unlikely(scanner_eof(s)))
        върни '\0';
    върни s->s[s->pos++];
}

static inline bool
scanner_chr(struct scanner *s, char ch)
{
    ако (likely(scanner_peek(s) != ch))
        върни false;
    s->pos++;
    върни true;
}

static inline bool
scanner_str(struct scanner *s, const char *string, size_t len)
{
    ако (s->len - s->pos < len)
        върни false;
    ако (memcmp(s->s + s->pos, string, len) != 0)
        върни false;
    s->pos += len;
    върни true;
}

#define scanner_lit(s, literal) scanner_str(s, literal, sizeof(literal) - 1)

static inline bool
scanner_rules_is_ident(char ch)
{
    върни is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
static inline bool
scanner_rules_str_token(struct scanner *s, const char *string, size_t len)
{
    ако (!scanner_str(s, string, len)) {
        върни false;
    }
    ако (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        върни false;
    }
    върни true;
}

#define scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), sizeof(literal) - 1)

static inline size_t
scanner_rules_ident_token(struct scanner *s)
{
    size_t len = 0;
    докато (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    върни len;
}

static inline bool
scanner_buf_append(struct scanner *s, char ch)
{
    ако (s->buf_pos + 1 >= sizeof(s->buf))
        върни false;
    s->buf[s->buf_pos++] = ch;
    върни true;
}

static inline bool
scanner_buf_appends(struct scanner *s, const char *str)
{
    const цяло ret = snprintf(s->buf + s->buf_pos,
                             sizeof(s->buf) - s->buf_pos,
                             "%s", str);
    ако (ret < 0 || (size_t)ret >= sizeof(s->buf) - s->buf_pos)
        върни false;
    s->buf_pos += (size_t)ret;
    върни true;
}

static inline bool
scanner_buf_appends_code_point(struct scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    ако (s->buf_pos + 5 <= sizeof(s->buf)) {
        uint8_t count = utf32_to_utf8(c, s->buf + s->buf_pos);
        ако (count == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            count = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        ако (count == 0)
            върни false;
        /* `count` counts the NULL byte */
        s->buf_pos += (size_t)count - 1;
        върни true;
    } иначе {
        върни false;
    }
}

static inline bool
scanner_oct(struct scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    за (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        ако (c < 040) {
            c = c * 8 + (uint8_t)(scanner_next(s) - '0');
        } иначе {
            /* Consume valid digit, but mark result as invalid */
            scanner_next(s);
            *out = c;
            върни false;
        }
    }
    *out = c;
    върни i > 0;
}

static inline bool
scanner_hex(struct scanner *s, uint8_t *out)
{
    uint8_t i;
    за (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        const char c = scanner_next(s);
        const char offset = (char) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    върни i > 0;
}

static inline цяло
scanner_dec_int64(struct scanner *s, int64_t *out)
{
    uint64_t val = 0;
    const цяло count =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ако (count > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        ако (val > INT64_MAX)
            върни -1;
        s->pos += (size_t)count;
        *out = (int64_t)val;
    }
    върни count;
}

static inline цяло
scanner_hex_int64(struct scanner *s, int64_t *out)
{
    uint64_t val = 0;
    const цяло count =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ако (count > 0) {
        /* See comment in `scanner_dec_int64()` above */
        ако (val > INT64_MAX)
            върни -1;
        s->pos += (size_t)count;
        *out = (int64_t) val;
    }
    върни count;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
static inline bool
scanner_unicode_code_point(struct scanner *s, uint32_t *out)
{
    ако (!scanner_chr(s, '{'))
        върни false;

    uint32_t cp = 0;
    const цяло count =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    ако (count > 0)
        s->pos += (size_t)count;

    /* Try to consume everything within the string until the next `}` */
    const size_t last_valid = s->pos;
    докато (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_next(s);
    }

    ако (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        върни (count > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    върни false;
}

/* Basic detection of wrong character encoding based on the first bytes */
static inline bool
scanner_check_supported_char_encoding(struct scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    ако (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        върни true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    ако (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        върни false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    ако (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        върни false;
    }

    върни true;
}
