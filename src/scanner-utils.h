/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ <stddef.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "util-numbers.h"
#അന്തര്ഭാവ "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
ഘടന sval {
    size_t len;
    സ്ഥിരം അക്ഷരം *start ATTR_COUNTED_BY(len);
};
തരനിർവചനം darray(ഘടന sval) darray_sval;

സ്ഥിതം വരിയിൽ bool
svaleq(ഘടന sval s1, ഘടന sval s2)
{
    തിരികെ s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

സ്ഥിതം വരിയിൽ bool
isvaleq(ഘടന sval s1, ഘടന sval s2)
{
    തിരികെ s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

സ്ഥിതം വരിയിൽ bool
svaleq_prefix(ഘടന sval s1, ഘടന sval s2)
{
    തിരികെ s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#define SVAL(__start, __len) (ഘടന sval){ .start = (__start), .len = (__len) }
#define SVAL_LIT(literal) SVAL(literal, വലുപ്പം(literal) - 1)
#define SVAL_INIT(literal) { .start = (literal), .len = വലുപ്പം(literal) - 1 }

/* A line:column location in the input string (1-based). */
ഘടന scanner_loc {
    size_t വരി, column;
};

ഘടന scanner {
    size_t pos;
    size_t len;
    സ്ഥിരം അക്ഷരം *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    അക്ഷരം buf[1024];
    size_t buf_pos;
    /* The position of the start of the current token. */
    size_t token_pos;
    /* Cached values used by scanner_token_location. */
    size_t cached_pos;
    ഘടന scanner_loc cached_loc;
    സ്ഥിരം അക്ഷരം *file_name;
    ഘടന xkb_context *ctx;
    ശൂന്യം *priv;
};

/* Compute the line:column location for the current token (slow). */
ഘടന scanner_loc
scanner_token_location(ഘടന scanner *s);

#define scanner_log_with_code(scanner, level, verbosity, log_സന്ദേശം_id, fmt, ...) ചെയ്യുക { \
    ഘടന scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_സന്ദേശം_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.വരി,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} ആകുമ്പോൾ(0)

#define scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

സ്ഥിതം വരിയിൽ ശൂന്യം
scanner_init(ഘടന scanner *s, ഘടന xkb_context *ctx,
             സ്ഥിരം അക്ഷരം *string, size_t len, സ്ഥിരം അക്ഷരം *file_name,
             ശൂന്യം *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.വരി = s->cached_loc.column = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

സ്ഥിതം വരിയിൽ അക്ഷരം
scanner_peek(ഘടന scanner *s)
{
    എങ്കിൽ (unlikely(s->pos >= s->len))
        തിരികെ '\0';
    തിരികെ s->s[s->pos];
}

സ്ഥിതം വരിയിൽ bool
scanner_eof(ഘടന scanner *s)
{
    തിരികെ s->pos >= s->len;
}

സ്ഥിതം വരിയിൽ bool
scanner_eol(ഘടന scanner *s)
{
    തിരികെ scanner_peek(s) == '\n';
}

സ്ഥിതം വരിയിൽ ശൂന്യം
scanner_skip_to_eol(ഘടന scanner *s)
{
    സ്ഥിരം അക്ഷരം *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    സ്ഥിരം size_t new_pos = nl ? (size_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

സ്ഥിതം വരിയിൽ അക്ഷരം
scanner_next(ഘടന scanner *s)
{
    എങ്കിൽ (unlikely(scanner_eof(s)))
        തിരികെ '\0';
    തിരികെ s->s[s->pos++];
}

സ്ഥിതം വരിയിൽ bool
scanner_chr(ഘടന scanner *s, അക്ഷരം ch)
{
    എങ്കിൽ (likely(scanner_peek(s) != ch))
        തിരികെ false;
    s->pos++;
    തിരികെ true;
}

സ്ഥിതം വരിയിൽ bool
scanner_str(ഘടന scanner *s, സ്ഥിരം അക്ഷരം *string, size_t len)
{
    എങ്കിൽ (s->len - s->pos < len)
        തിരികെ false;
    എങ്കിൽ (memcmp(s->s + s->pos, string, len) != 0)
        തിരികെ false;
    s->pos += len;
    തിരികെ true;
}

#define scanner_lit(s, literal) scanner_str(s, literal, വലുപ്പം(literal) - 1)

സ്ഥിതം വരിയിൽ bool
scanner_rules_is_ident(അക്ഷരം ch)
{
    തിരികെ is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
സ്ഥിതം വരിയിൽ bool
scanner_rules_str_token(ഘടന scanner *s, സ്ഥിരം അക്ഷരം *string, size_t len)
{
    എങ്കിൽ (!scanner_str(s, string, len)) {
        തിരികെ false;
    }
    എങ്കിൽ (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        തിരികെ false;
    }
    തിരികെ true;
}

#define scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), വലുപ്പം(literal) - 1)

സ്ഥിതം വരിയിൽ size_t
scanner_rules_ident_token(ഘടന scanner *s)
{
    size_t len = 0;
    ആകുമ്പോൾ (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    തിരികെ len;
}

സ്ഥിതം വരിയിൽ bool
scanner_buf_append(ഘടന scanner *s, അക്ഷരം ch)
{
    എങ്കിൽ (s->buf_pos + 1 >= വലുപ്പം(s->buf))
        തിരികെ false;
    s->buf[s->buf_pos++] = ch;
    തിരികെ true;
}

സ്ഥിതം വരിയിൽ bool
scanner_buf_appends(ഘടന scanner *s, സ്ഥിരം അക്ഷരം *str)
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ ret = snprintf(s->buf + s->buf_pos,
                             വലുപ്പം(s->buf) - s->buf_pos,
                             "%s", str);
    എങ്കിൽ (ret < 0 || (size_t)ret >= വലുപ്പം(s->buf) - s->buf_pos)
        തിരികെ false;
    s->buf_pos += (size_t)ret;
    തിരികെ true;
}

സ്ഥിതം വരിയിൽ bool
scanner_buf_appends_code_point(ഘടന scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    എങ്കിൽ (s->buf_pos + 5 <= വലുപ്പം(s->buf)) {
        uint8_t എണ്ണം = utf32_to_utf8(c, s->buf + s->buf_pos);
        എങ്കിൽ (എണ്ണം == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            എണ്ണം = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        എങ്കിൽ (എണ്ണം == 0)
            തിരികെ false;
        /* `count` counts the NULL byte */
        s->buf_pos += (size_t)എണ്ണം - 1;
        തിരികെ true;
    } അല്ലെങ്കിൽ {
        തിരികെ false;
    }
}

സ്ഥിതം വരിയിൽ bool
scanner_oct(ഘടന scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    വേണ്ടി (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        എങ്കിൽ (c < 040) {
            c = c * 8 + (uint8_t)(scanner_next(s) - '0');
        } അല്ലെങ്കിൽ {
            /* Consume valid digit, but mark result as invalid */
            scanner_next(s);
            *out = c;
            തിരികെ false;
        }
    }
    *out = c;
    തിരികെ i > 0;
}

സ്ഥിതം വരിയിൽ bool
scanner_hex(ഘടന scanner *s, uint8_t *out)
{
    uint8_t i;
    വേണ്ടി (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        സ്ഥിരം അക്ഷരം c = scanner_next(s);
        സ്ഥിരം അക്ഷരം offset = (അക്ഷരം) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    തിരികെ i > 0;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
scanner_dec_int64(ഘടന scanner *s, int64_t *out)
{
    uint64_t val = 0;
    സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    എങ്കിൽ (എണ്ണം > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        എങ്കിൽ (val > INT64_MAX)
            തിരികെ -1;
        s->pos += (size_t)എണ്ണം;
        *out = (int64_t)val;
    }
    തിരികെ എണ്ണം;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
scanner_hex_int64(ഘടന scanner *s, int64_t *out)
{
    uint64_t val = 0;
    സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    എങ്കിൽ (എണ്ണം > 0) {
        /* See comment in `scanner_dec_int64()` above */
        എങ്കിൽ (val > INT64_MAX)
            തിരികെ -1;
        s->pos += (size_t)എണ്ണം;
        *out = (int64_t) val;
    }
    തിരികെ എണ്ണം;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
സ്ഥിതം വരിയിൽ bool
scanner_unicode_code_point(ഘടന scanner *s, uint32_t *out)
{
    എങ്കിൽ (!scanner_chr(s, '{'))
        തിരികെ false;

    uint32_t cp = 0;
    സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    എങ്കിൽ (എണ്ണം > 0)
        s->pos += (size_t)എണ്ണം;

    /* Try to consume everything within the string until the next `}` */
    സ്ഥിരം size_t last_valid = s->pos;
    ആകുമ്പോൾ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_next(s);
    }

    എങ്കിൽ (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        തിരികെ (എണ്ണം > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    തിരികെ false;
}

/* Basic detection of wrong character encoding based on the first bytes */
സ്ഥിതം വരിയിൽ bool
scanner_check_supported_char_encoding(ഘടന scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    എങ്കിൽ (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        തിരികെ true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    എങ്കിൽ (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        തിരികെ false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    എങ്കിൽ (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        തിരികെ false;
    }

    തിരികെ true;
}
