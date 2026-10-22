/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdbool.h>
#अन्तर्भाव <stddef.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "context.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-numbers.h"
#अन्तर्भाव "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
संरचना sval {
    size_t len;
    ध्रुव वर्ण *start ATTR_COUNTED_BY(len);
};
प्रकारनिर्देश darray(संरचना sval) darray_sval;

स्थिर अन्तःस्थ bool
svaleq(संरचना sval s1, संरचना sval s2)
{
    पटिनिवत्त s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

स्थिर अन्तःस्थ bool
isvaleq(संरचना sval s1, संरचना sval s2)
{
    पटिनिवत्त s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

स्थिर अन्तःस्थ bool
svaleq_prefix(संरचना sval s1, संरचना sval s2)
{
    पटिनिवत्त s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#define SVAL(__start, __len) (संरचना sval){ .start = (__start), .len = (__len) }
#define SVAL_LIT(literal) SVAL(literal, परिमाण(literal) - 1)
#define SVAL_INIT(literal) { .start = (literal), .len = परिमाण(literal) - 1 }

/* A line:column location in the input string (1-based). */
संरचना scanner_loc {
    size_t पङ्क्तिः, column;
};

संरचना scanner {
    size_t pos;
    size_t len;
    ध्रुव वर्ण *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    वर्ण buf[1024];
    size_t buf_pos;
    /* The position of the start of the current token. */
    size_t token_pos;
    /* Cached values used by scanner_token_location. */
    size_t cached_pos;
    संरचना scanner_loc cached_loc;
    ध्रुव वर्ण *file_name;
    संरचना xkb_context *ctx;
    शून्य *priv;
};

/* Compute the line:column location for the current token (slow). */
संरचना scanner_loc
scanner_token_location(संरचना scanner *s);

#define scanner_log_with_code(scanner, level, verbosity, log_सन्देशः_id, fmt, ...) करोहि { \
    संरचना scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_सन्देशः_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.पङ्क्तिः,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} याव(0)

#define scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

स्थिर अन्तःस्थ शून्य
scanner_init(संरचना scanner *s, संरचना xkb_context *ctx,
             ध्रुव वर्ण *string, size_t len, ध्रुव वर्ण *file_name,
             शून्य *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.पङ्क्तिः = s->cached_loc.column = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

स्थिर अन्तःस्थ वर्ण
scanner_peek(संरचना scanner *s)
{
    सचे (unlikely(s->pos >= s->len))
        पटिनिवत्त '\0';
    पटिनिवत्त s->s[s->pos];
}

स्थिर अन्तःस्थ bool
scanner_eof(संरचना scanner *s)
{
    पटिनिवत्त s->pos >= s->len;
}

स्थिर अन्तःस्थ bool
scanner_eol(संरचना scanner *s)
{
    पटिनिवत्त scanner_peek(s) == '\n';
}

स्थिर अन्तःस्थ शून्य
scanner_skip_to_eol(संरचना scanner *s)
{
    ध्रुव वर्ण *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    ध्रुव size_t new_pos = nl ? (size_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

स्थिर अन्तःस्थ वर्ण
scanner_next(संरचना scanner *s)
{
    सचे (unlikely(scanner_eof(s)))
        पटिनिवत्त '\0';
    पटिनिवत्त s->s[s->pos++];
}

स्थिर अन्तःस्थ bool
scanner_chr(संरचना scanner *s, वर्ण ch)
{
    सचे (likely(scanner_peek(s) != ch))
        पटिनिवत्त false;
    s->pos++;
    पटिनिवत्त true;
}

स्थिर अन्तःस्थ bool
scanner_str(संरचना scanner *s, ध्रुव वर्ण *string, size_t len)
{
    सचे (s->len - s->pos < len)
        पटिनिवत्त false;
    सचे (memcmp(s->s + s->pos, string, len) != 0)
        पटिनिवत्त false;
    s->pos += len;
    पटिनिवत्त true;
}

#define scanner_lit(s, literal) scanner_str(s, literal, परिमाण(literal) - 1)

स्थिर अन्तःस्थ bool
scanner_rules_is_ident(वर्ण ch)
{
    पटिनिवत्त is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
स्थिर अन्तःस्थ bool
scanner_rules_str_token(संरचना scanner *s, ध्रुव वर्ण *string, size_t len)
{
    सचे (!scanner_str(s, string, len)) {
        पटिनिवत्त false;
    }
    सचे (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        पटिनिवत्त false;
    }
    पटिनिवत्त true;
}

#define scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), परिमाण(literal) - 1)

स्थिर अन्तःस्थ size_t
scanner_rules_ident_token(संरचना scanner *s)
{
    size_t len = 0;
    याव (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    पटिनिवत्त len;
}

स्थिर अन्तःस्थ bool
scanner_buf_append(संरचना scanner *s, वर्ण ch)
{
    सचे (s->buf_pos + 1 >= परिमाण(s->buf))
        पटिनिवत्त false;
    s->buf[s->buf_pos++] = ch;
    पटिनिवत्त true;
}

स्थिर अन्तःस्थ bool
scanner_buf_appends(संरचना scanner *s, ध्रुव वर्ण *str)
{
    ध्रुव पूर्णाङ्क ret = snprintf(s->buf + s->buf_pos,
                             परिमाण(s->buf) - s->buf_pos,
                             "%s", str);
    सचे (ret < 0 || (size_t)ret >= परिमाण(s->buf) - s->buf_pos)
        पटिनिवत्त false;
    s->buf_pos += (size_t)ret;
    पटिनिवत्त true;
}

स्थिर अन्तःस्थ bool
scanner_buf_appends_code_point(संरचना scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    सचे (s->buf_pos + 5 <= परिमाण(s->buf)) {
        uint8_t गणनम् = utf32_to_utf8(c, s->buf + s->buf_pos);
        सचे (गणनम् == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            गणनम् = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        सचे (गणनम् == 0)
            पटिनिवत्त false;
        /* `count` counts the NULL byte */
        s->buf_pos += (size_t)गणनम् - 1;
        पटिनिवत्त true;
    } अञ्ञथा {
        पटिनिवत्त false;
    }
}

स्थिर अन्तःस्थ bool
scanner_oct(संरचना scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    अत्थाय (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        सचे (c < 040) {
            c = c * 8 + (uint8_t)(scanner_next(s) - '0');
        } अञ्ञथा {
            /* Consume valid digit, but mark result as invalid */
            scanner_next(s);
            *out = c;
            पटिनिवत्त false;
        }
    }
    *out = c;
    पटिनिवत्त i > 0;
}

स्थिर अन्तःस्थ bool
scanner_hex(संरचना scanner *s, uint8_t *out)
{
    uint8_t i;
    अत्थाय (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        ध्रुव वर्ण c = scanner_next(s);
        ध्रुव वर्ण offset = (वर्ण) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    पटिनिवत्त i > 0;
}

स्थिर अन्तःस्थ पूर्णाङ्क
scanner_dec_int64(संरचना scanner *s, int64_t *out)
{
    uint64_t val = 0;
    ध्रुव पूर्णाङ्क गणनम् =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    सचे (गणनम् > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        सचे (val > INT64_MAX)
            पटिनिवत्त -1;
        s->pos += (size_t)गणनम्;
        *out = (int64_t)val;
    }
    पटिनिवत्त गणनम्;
}

स्थिर अन्तःस्थ पूर्णाङ्क
scanner_hex_int64(संरचना scanner *s, int64_t *out)
{
    uint64_t val = 0;
    ध्रुव पूर्णाङ्क गणनम् =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    सचे (गणनम् > 0) {
        /* See comment in `scanner_dec_int64()` above */
        सचे (val > INT64_MAX)
            पटिनिवत्त -1;
        s->pos += (size_t)गणनम्;
        *out = (int64_t) val;
    }
    पटिनिवत्त गणनम्;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
स्थिर अन्तःस्थ bool
scanner_unicode_code_point(संरचना scanner *s, uint32_t *out)
{
    सचे (!scanner_chr(s, '{'))
        पटिनिवत्त false;

    uint32_t cp = 0;
    ध्रुव पूर्णाङ्क गणनम् =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    सचे (गणनम् > 0)
        s->pos += (size_t)गणनम्;

    /* Try to consume everything within the string until the next `}` */
    ध्रुव size_t last_valid = s->pos;
    याव (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_next(s);
    }

    सचे (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        पटिनिवत्त (गणनम् > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    पटिनिवत्त false;
}

/* Basic detection of wrong character encoding based on the first bytes */
स्थिर अन्तःस्थ bool
scanner_check_supported_char_encoding(संरचना scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    सचे (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        पटिनिवत्त true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    सचे (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        पटिनिवत्त false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    सचे (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        पटिनिवत्त false;
    }

    पटिनिवत्त true;
}
