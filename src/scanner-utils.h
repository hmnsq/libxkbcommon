/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdbool.h>
#અન્તર્ભાવ <stddef.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "util-numbers.h"
#અન્તર્ભાવ "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
સંરચના sval {
    size_t len;
    અચલ અક્ષર *start ATTR_COUNTED_BY(len);
};
પ્રકારવ્યાખ્યા darray(સંરચના sval) darray_sval;

સ્થિર અંતઃરેખ bool
svaleq(સંરચના sval s1, સંરચના sval s2)
{
    પાછા s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

સ્થિર અંતઃરેખ bool
isvaleq(સંરચના sval s1, સંરચના sval s2)
{
    પાછા s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

સ્થિર અંતઃરેખ bool
svaleq_prefix(સંરચના sval s1, સંરચના sval s2)
{
    પાછા s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#define SVAL(__start, __len) (સંરચના sval){ .start = (__start), .len = (__len) }
#define SVAL_LIT(literal) SVAL(literal, કદ(literal) - 1)
#define SVAL_INIT(literal) { .start = (literal), .len = કદ(literal) - 1 }

/* A line:column location in the input string (1-based). */
સંરચના scanner_loc {
    size_t લીટી, column;
};

સંરચના scanner {
    size_t pos;
    size_t len;
    અચલ અક્ષર *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    અક્ષર buf[1024];
    size_t buf_pos;
    /* The position of the start of the current token. */
    size_t token_pos;
    /* Cached values used by scanner_token_location. */
    size_t cached_pos;
    સંરચના scanner_loc cached_loc;
    અચલ અક્ષર *file_name;
    સંરચના xkb_context *ctx;
    શૂન્ય *priv;
};

/* Compute the line:column location for the current token (slow). */
સંરચના scanner_loc
scanner_token_location(સંરચના scanner *s);

#define scanner_log_with_code(scanner, level, verbosity, log_સંદેશ_id, fmt, ...) કરો { \
    સંરચના scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_સંદેશ_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.લીટી,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} જ્યારે(0)

#define scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

સ્થિર અંતઃરેખ શૂન્ય
scanner_init(સંરચના scanner *s, સંરચના xkb_context *ctx,
             અચલ અક્ષર *string, size_t len, અચલ અક્ષર *file_name,
             શૂન્ય *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.લીટી = s->cached_loc.column = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

સ્થિર અંતઃરેખ અક્ષર
scanner_peek(સંરચના scanner *s)
{
    જો (unlikely(s->pos >= s->len))
        પાછા '\0';
    પાછા s->s[s->pos];
}

સ્થિર અંતઃરેખ bool
scanner_eof(સંરચના scanner *s)
{
    પાછા s->pos >= s->len;
}

સ્થિર અંતઃરેખ bool
scanner_eol(સંરચના scanner *s)
{
    પાછા scanner_peek(s) == '\n';
}

સ્થિર અંતઃરેખ શૂન્ય
scanner_skip_to_eol(સંરચના scanner *s)
{
    અચલ અક્ષર *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    અચલ size_t new_pos = nl ? (size_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

સ્થિર અંતઃરેખ અક્ષર
scanner_next(સંરચના scanner *s)
{
    જો (unlikely(scanner_eof(s)))
        પાછા '\0';
    પાછા s->s[s->pos++];
}

સ્થિર અંતઃરેખ bool
scanner_chr(સંરચના scanner *s, અક્ષર ch)
{
    જો (likely(scanner_peek(s) != ch))
        પાછા false;
    s->pos++;
    પાછા true;
}

સ્થિર અંતઃરેખ bool
scanner_str(સંરચના scanner *s, અચલ અક્ષર *string, size_t len)
{
    જો (s->len - s->pos < len)
        પાછા false;
    જો (memcmp(s->s + s->pos, string, len) != 0)
        પાછા false;
    s->pos += len;
    પાછા true;
}

#define scanner_lit(s, literal) scanner_str(s, literal, કદ(literal) - 1)

સ્થિર અંતઃરેખ bool
scanner_rules_is_ident(અક્ષર ch)
{
    પાછા is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
સ્થિર અંતઃરેખ bool
scanner_rules_str_token(સંરચના scanner *s, અચલ અક્ષર *string, size_t len)
{
    જો (!scanner_str(s, string, len)) {
        પાછા false;
    }
    જો (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        પાછા false;
    }
    પાછા true;
}

#define scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), કદ(literal) - 1)

સ્થિર અંતઃરેખ size_t
scanner_rules_ident_token(સંરચના scanner *s)
{
    size_t len = 0;
    જ્યારે (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    પાછા len;
}

સ્થિર અંતઃરેખ bool
scanner_buf_append(સંરચના scanner *s, અક્ષર ch)
{
    જો (s->buf_pos + 1 >= કદ(s->buf))
        પાછા false;
    s->buf[s->buf_pos++] = ch;
    પાછા true;
}

સ્થિર અંતઃરેખ bool
scanner_buf_appends(સંરચના scanner *s, અચલ અક્ષર *str)
{
    અચલ પૂર્ણાંક ret = snprintf(s->buf + s->buf_pos,
                             કદ(s->buf) - s->buf_pos,
                             "%s", str);
    જો (ret < 0 || (size_t)ret >= કદ(s->buf) - s->buf_pos)
        પાછા false;
    s->buf_pos += (size_t)ret;
    પાછા true;
}

સ્થિર અંતઃરેખ bool
scanner_buf_appends_code_point(સંરચના scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    જો (s->buf_pos + 5 <= કદ(s->buf)) {
        uint8_t ગણતરી = utf32_to_utf8(c, s->buf + s->buf_pos);
        જો (ગણતરી == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            ગણતરી = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        જો (ગણતરી == 0)
            પાછા false;
        /* `count` counts the NULL byte */
        s->buf_pos += (size_t)ગણતરી - 1;
        પાછા true;
    } નહીંતર {
        પાછા false;
    }
}

સ્થિર અંતઃરેખ bool
scanner_oct(સંરચના scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    માટે (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        જો (c < 040) {
            c = c * 8 + (uint8_t)(scanner_next(s) - '0');
        } નહીંતર {
            /* Consume valid digit, but mark result as invalid */
            scanner_next(s);
            *out = c;
            પાછા false;
        }
    }
    *out = c;
    પાછા i > 0;
}

સ્થિર અંતઃરેખ bool
scanner_hex(સંરચના scanner *s, uint8_t *out)
{
    uint8_t i;
    માટે (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        અચલ અક્ષર c = scanner_next(s);
        અચલ અક્ષર offset = (અક્ષર) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    પાછા i > 0;
}

સ્થિર અંતઃરેખ પૂર્ણાંક
scanner_dec_int64(સંરચના scanner *s, int64_t *out)
{
    uint64_t val = 0;
    અચલ પૂર્ણાંક ગણતરી =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    જો (ગણતરી > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        જો (val > INT64_MAX)
            પાછા -1;
        s->pos += (size_t)ગણતરી;
        *out = (int64_t)val;
    }
    પાછા ગણતરી;
}

સ્થિર અંતઃરેખ પૂર્ણાંક
scanner_hex_int64(સંરચના scanner *s, int64_t *out)
{
    uint64_t val = 0;
    અચલ પૂર્ણાંક ગણતરી =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    જો (ગણતરી > 0) {
        /* See comment in `scanner_dec_int64()` above */
        જો (val > INT64_MAX)
            પાછા -1;
        s->pos += (size_t)ગણતરી;
        *out = (int64_t) val;
    }
    પાછા ગણતરી;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
સ્થિર અંતઃરેખ bool
scanner_unicode_code_point(સંરચના scanner *s, uint32_t *out)
{
    જો (!scanner_chr(s, '{'))
        પાછા false;

    uint32_t cp = 0;
    અચલ પૂર્ણાંક ગણતરી =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    જો (ગણતરી > 0)
        s->pos += (size_t)ગણતરી;

    /* Try to consume everything within the string until the next `}` */
    અચલ size_t last_valid = s->pos;
    જ્યારે (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_next(s);
    }

    જો (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        પાછા (ગણતરી > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    પાછા false;
}

/* Basic detection of wrong character encoding based on the first bytes */
સ્થિર અંતઃરેખ bool
scanner_check_supported_char_encoding(સંરચના scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    જો (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        પાછા true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    જો (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        પાછા false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    જો (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        પાછા false;
    }

    પાછા true;
}
