/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#निर्देशः once

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
    आकार_t len;
    ध्रुव वर्ण *start ATTR_COUNTED_BY(len);
};
प्रकारनिर्देश darray(संरचना sval) darray_sval;

स्थिर अन्तःस्थ bool
svaleq(संरचना sval s1, संरचना sval s2)
{
    प्रत्यागम s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

स्थिर अन्तःस्थ bool
isvaleq(संरचना sval s1, संरचना sval s2)
{
    प्रत्यागम s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

स्थिर अन्तःस्थ bool
svaleq_उपसर्गः(संरचना sval s1, संरचना sval s2)
{
    प्रत्यागम s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#परिभाषय SVAL(__start, __len) (संरचना sval){ .start = (__start), .len = (__len) }
#परिभाषय SVAL_LIT(literal) SVAL(literal, परिमाण(literal) - 1)
#परिभाषय SVAL_INIT(literal) { .start = (literal), .len = परिमाण(literal) - 1 }

/* A line:column location in the input string (1-based). */
संरचना scanner_loc {
    आकार_t पङ्क्तिः, column;
};

संरचना scanner {
    आकार_t pos;
    आकार_t len;
    ध्रुव वर्ण *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    वर्ण buf[1024];
    आकार_t buf_pos;
    /* The position of the start of the current token. */
    आकार_t token_pos;
    /* Cached values used by scanner_token_location. */
    आकार_t cached_pos;
    संरचना scanner_loc cached_loc;
    ध्रुव वर्ण *file_name;
    संरचना xkb_context *ctx;
    शून्य *priv;
};

/* Compute the line:column location for the current token (slow). */
संरचना scanner_loc
scanner_token_location(संरचना scanner *s);

#परिभाषय scanner_log_with_संहिता(scanner, level, verbosity, log_सन्देशः_id, fmt, ...) कुरु { \
    संरचना scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_संहिता((scanner)->ctx, (level), verbosity, log_सन्देशः_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.पङ्क्तिः,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} यावत्(0)

#परिभाषय scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_संहिता(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_अल्पिष्ठम्, id, fmt, ##__VA_ARGS__)

#परिभाषय scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_संहिता(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_अल्पिष्ठम्, id, fmt, ##__VA_ARGS__)

#परिभाषय scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_संहिता(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

स्थिर अन्तःस्थ शून्य
scanner_init(संरचना scanner *s, संरचना xkb_context *ctx,
             ध्रुव वर्ण *string, आकार_t len, ध्रुव वर्ण *file_name,
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
    यदि (unlikely(s->pos >= s->len))
        प्रत्यागम '\0';
    प्रत्यागम s->s[s->pos];
}

स्थिर अन्तःस्थ bool
scanner_eof(संरचना scanner *s)
{
    प्रत्यागम s->pos >= s->len;
}

स्थिर अन्तःस्थ bool
scanner_eol(संरचना scanner *s)
{
    प्रत्यागम scanner_peek(s) == '\n';
}

स्थिर अन्तःस्थ शून्य
scanner_skip_गन्तव्यम्_eol(संरचना scanner *s)
{
    ध्रुव वर्ण *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    ध्रुव आकार_t new_pos = nl ? (आकार_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

स्थिर अन्तःस्थ वर्ण
scanner_अग्र(संरचना scanner *s)
{
    यदि (unlikely(scanner_eof(s)))
        प्रत्यागम '\0';
    प्रत्यागम s->s[s->pos++];
}

स्थिर अन्तःस्थ bool
scanner_chr(संरचना scanner *s, वर्ण ch)
{
    यदि (likely(scanner_peek(s) != ch))
        प्रत्यागम false;
    s->pos++;
    प्रत्यागम true;
}

स्थिर अन्तःस्थ bool
scanner_str(संरचना scanner *s, ध्रुव वर्ण *string, आकार_t len)
{
    यदि (s->len - s->pos < len)
        प्रत्यागम false;
    यदि (memcmp(s->s + s->pos, string, len) != 0)
        प्रत्यागम false;
    s->pos += len;
    प्रत्यागम true;
}

#परिभाषय scanner_lit(s, literal) scanner_str(s, literal, परिमाण(literal) - 1)

स्थिर अन्तःस्थ bool
scanner_नियमाः_is_ident(वर्ण ch)
{
    प्रत्यागम is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
स्थिर अन्तःस्थ bool
scanner_नियमाः_str_token(संरचना scanner *s, ध्रुव वर्ण *string, आकार_t len)
{
    यदि (!scanner_str(s, string, len)) {
        प्रत्यागम false;
    }
    यदि (scanner_नियमाः_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        प्रत्यागम false;
    }
    प्रत्यागम true;
}

#परिभाषय scanner_नियमाः_lit_token(s, literal) \
    scanner_नियमाः_str_token((s), (literal), परिमाण(literal) - 1)

स्थिर अन्तःस्थ आकार_t
scanner_नियमाः_ident_token(संरचना scanner *s)
{
    आकार_t len = 0;
    यावत् (!scanner_eof(s) && scanner_नियमाः_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    प्रत्यागम len;
}

स्थिर अन्तःस्थ bool
scanner_buf_append(संरचना scanner *s, वर्ण ch)
{
    यदि (s->buf_pos + 1 >= परिमाण(s->buf))
        प्रत्यागम false;
    s->buf[s->buf_pos++] = ch;
    प्रत्यागम true;
}

स्थिर अन्तःस्थ bool
scanner_buf_appends(संरचना scanner *s, ध्रुव वर्ण *str)
{
    ध्रुव पूर्णाङ्क ret = snprintf(s->buf + s->buf_pos,
                             परिमाण(s->buf) - s->buf_pos,
                             "%s", str);
    यदि (ret < 0 || (आकार_t)ret >= परिमाण(s->buf) - s->buf_pos)
        प्रत्यागम false;
    s->buf_pos += (आकार_t)ret;
    प्रत्यागम true;
}

स्थिर अन्तःस्थ bool
scanner_buf_appends_संहिता_point(संरचना scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    यदि (s->buf_pos + 5 <= परिमाण(s->buf)) {
        uint8_t गणनम् = utf32_गन्तव्यम्_utf8(c, s->buf + s->buf_pos);
        यदि (गणनम् == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            गणनम् = utf32_गन्तव्यम्_utf8(0xfffd, s->buf + s->buf_pos);
        }
        यदि (गणनम् == 0)
            प्रत्यागम false;
        /* `count` counts the NULL byte */
        s->buf_pos += (आकार_t)गणनम् - 1;
        प्रत्यागम true;
    } अन्यथा {
        प्रत्यागम false;
    }
}

स्थिर अन्तःस्थ bool
scanner_oct(संरचना scanner *s, uint8_t *बहिर्गमनम्)
{
    uint8_t i = 0;
    uint8_t c = 0;
    प्रति (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        यदि (c < 040) {
            c = c * 8 + (uint8_t)(scanner_अग्र(s) - '0');
        } अन्यथा {
            /* Consume valid digit, but mark result as invalid */
            scanner_अग्र(s);
            *बहिर्गमनम् = c;
            प्रत्यागम false;
        }
    }
    *बहिर्गमनम् = c;
    प्रत्यागम i > 0;
}

स्थिर अन्तःस्थ bool
scanner_hex(संरचना scanner *s, uint8_t *बहिर्गमनम्)
{
    uint8_t i;
    प्रति (i = 0, *बहिर्गमनम् = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        ध्रुव वर्ण c = scanner_अग्र(s);
        ध्रुव वर्ण विस्थापनम् = (वर्ण) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *बहिर्गमनम् = *बहिर्गमनम् * 16 + (uint8_t)(c - विस्थापनम्);
    }
    प्रत्यागम i > 0;
}

स्थिर अन्तःस्थ पूर्णाङ्क
scanner_dec_int64(संरचना scanner *s, int64_t *बहिर्गमनम्)
{
    uint64_t val = 0;
    ध्रुव पूर्णाङ्क गणनम् =
        parse_dec_गन्तव्यम्_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    यदि (गणनम् > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        यदि (val > INT64_MAX)
            प्रत्यागम -1;
        s->pos += (आकार_t)गणनम्;
        *बहिर्गमनम् = (int64_t)val;
    }
    प्रत्यागम गणनम्;
}

स्थिर अन्तःस्थ पूर्णाङ्क
scanner_hex_int64(संरचना scanner *s, int64_t *बहिर्गमनम्)
{
    uint64_t val = 0;
    ध्रुव पूर्णाङ्क गणनम् =
        parse_hex_गन्तव्यम्_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    यदि (गणनम् > 0) {
        /* See comment in `scanner_dec_int64()` above */
        यदि (val > INT64_MAX)
            प्रत्यागम -1;
        s->pos += (आकार_t)गणनम्;
        *बहिर्गमनम् = (int64_t) val;
    }
    प्रत्यागम गणनम्;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
स्थिर अन्तःस्थ bool
scanner_unicode_संहिता_point(संरचना scanner *s, uint32_t *बहिर्गमनम्)
{
    यदि (!scanner_chr(s, '{'))
        प्रत्यागम false;

    uint32_t cp = 0;
    ध्रुव पूर्णाङ्क गणनम् =
        parse_hex_गन्तव्यम्_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    यदि (गणनम् > 0)
        s->pos += (आकार_t)गणनम्;

    /* Try to consume everything within the string until the next `}` */
    ध्रुव आकार_t चरमः_valid = s->pos;
    यावत् (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_अग्र(s);
    }

    यदि (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *बहिर्गमनम् = cp;
        प्रत्यागम (गणनम् > 0 && s->pos == चरमः_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = चरमः_valid;
    प्रत्यागम false;
}

/* Basic detection of wrong character encoding based on the first bytes */
स्थिर अन्तःस्थ bool
scanner_check_supported_char_encoding(संरचना scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    यदि (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        प्रत्यागम true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    यदि (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        प्रत्यागम false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    यदि (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        प्रत्यागम false;
    }

    प्रत्यागम true;
}
