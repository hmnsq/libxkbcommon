/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#لارښوونه once

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
    اندازه_t len;
    ثابت حرف *پیل ATTR_COUNTED_BY(len);
};
تعريفنوع darray(بنية sval) darray_sval;

ساكن مضمن bool
svaleq(بنية sval s1, بنية sval s2)
{
    أرجع s1.len == s2.len && memcmp(s1.پیل, s2.پیل, s1.len) == 0;
}

ساكن مضمن bool
isvaleq(بنية sval s1, بنية sval s2)
{
    أرجع s1.len == s2.len && istrncmp(s1.پیل, s2.پیل, s1.len) == 0;
}

ساكن مضمن bool
svaleq_مختاړی(بنية sval s1, بنية sval s2)
{
    أرجع s1.len <= s2.len && memcmp(s1.پیل, s2.پیل, s1.len) == 0;
}

#تعریف_کړه SVAL(__start, __len) (بنية sval){ .پیل = (__start), .len = (__len) }
#تعریف_کړه SVAL_LIT(literal) SVAL(literal, حجم(literal) - 1)
#تعریف_کړه SVAL_INIT(literal) { .پیل = (literal), .len = حجم(literal) - 1 }

/* A line:column location in the input string (1-based). */
بنية scanner_loc {
    اندازه_t کرښه, ستنه;
};

بنية scanner {
    اندازه_t pos;
    اندازه_t len;
    ثابت حرف *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    حرف buf[1024];
    اندازه_t buf_pos;
    /* The position of the start of the current token. */
    اندازه_t token_pos;
    /* Cached values used by scanner_token_location. */
    اندازه_t cached_pos;
    بنية scanner_loc cached_loc;
    ثابت حرف *file_name;
    بنية xkb_context *ctx;
    فراغ *priv;
};

/* Compute the line:column location for the current token (slow). */
بنية scanner_loc
scanner_token_location(بنية scanner *s);

#تعریف_کړه scanner_log_with_کوډ(scanner, level, verbosity, log_پیغام_id, fmt, ...) افعل { \
    بنية scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_کوډ((scanner)->ctx, (level), verbosity, log_پیغام_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.کرښه,                                                     \
                      loc.ستنه, ##__VA_ARGS__);                                   \
} طالما(0)

#تعریف_کړه scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_کوډ(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_اصغري, id, fmt, ##__VA_ARGS__)

#تعریف_کړه scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_کوډ(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_اصغري, id, fmt, ##__VA_ARGS__)

#تعریف_کړه scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_کوډ(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

ساكن مضمن فراغ
scanner_init(بنية scanner *s, بنية xkb_context *ctx,
             ثابت حرف *string, اندازه_t len, ثابت حرف *file_name,
             فراغ *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.کرښه = s->cached_loc.ستنه = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

ساكن مضمن حرف
scanner_peek(بنية scanner *s)
{
    إذا (unlikely(s->pos >= s->len))
        أرجع '\0';
    أرجع s->s[s->pos];
}

ساكن مضمن bool
scanner_eof(بنية scanner *s)
{
    أرجع s->pos >= s->len;
}

ساكن مضمن bool
scanner_eol(بنية scanner *s)
{
    أرجع scanner_peek(s) == '\n';
}

ساكن مضمن فراغ
scanner_skip_منزل_eol(بنية scanner *s)
{
    ثابت حرف *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    ثابت اندازه_t new_pos = nl ? (اندازه_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

ساكن مضمن حرف
scanner_بل(بنية scanner *s)
{
    إذا (unlikely(scanner_eof(s)))
        أرجع '\0';
    أرجع s->s[s->pos++];
}

ساكن مضمن bool
scanner_chr(بنية scanner *s, حرف ch)
{
    إذا (likely(scanner_peek(s) != ch))
        أرجع false;
    s->pos++;
    أرجع true;
}

ساكن مضمن bool
scanner_str(بنية scanner *s, ثابت حرف *string, اندازه_t len)
{
    إذا (s->len - s->pos < len)
        أرجع false;
    إذا (memcmp(s->s + s->pos, string, len) != 0)
        أرجع false;
    s->pos += len;
    أرجع true;
}

#تعریف_کړه scanner_lit(s, literal) scanner_str(s, literal, حجم(literal) - 1)

ساكن مضمن bool
scanner_قواعد_is_ident(حرف ch)
{
    أرجع is_ګراف(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
ساكن مضمن bool
scanner_قواعد_str_token(بنية scanner *s, ثابت حرف *string, اندازه_t len)
{
    إذا (!scanner_str(s, string, len)) {
        أرجع false;
    }
    إذا (scanner_قواعد_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        أرجع false;
    }
    أرجع true;
}

#تعریف_کړه scanner_قواعد_lit_token(s, literal) \
    scanner_قواعد_str_token((s), (literal), حجم(literal) - 1)

ساكن مضمن اندازه_t
scanner_قواعد_ident_token(بنية scanner *s)
{
    اندازه_t len = 0;
    طالما (!scanner_eof(s) && scanner_قواعد_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    أرجع len;
}

ساكن مضمن bool
scanner_buf_append(بنية scanner *s, حرف ch)
{
    إذا (s->buf_pos + 1 >= حجم(s->buf))
        أرجع false;
    s->buf[s->buf_pos++] = ch;
    أرجع true;
}

ساكن مضمن bool
scanner_buf_appends(بنية scanner *s, ثابت حرف *str)
{
    ثابت صحيح ret = snprintf(s->buf + s->buf_pos,
                             حجم(s->buf) - s->buf_pos,
                             "%s", str);
    إذا (ret < 0 || (اندازه_t)ret >= حجم(s->buf) - s->buf_pos)
        أرجع false;
    s->buf_pos += (اندازه_t)ret;
    أرجع true;
}

ساكن مضمن bool
scanner_buf_appends_کوډ_ټکی(بنية scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    إذا (s->buf_pos + 5 <= حجم(s->buf)) {
        uint8_t شمېر = utf32_منزل_utf8(c, s->buf + s->buf_pos);
        إذا (شمېر == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            شمېر = utf32_منزل_utf8(0xfffd, s->buf + s->buf_pos);
        }
        إذا (شمېر == 0)
            أرجع false;
        /* `count` counts the NULL byte */
        s->buf_pos += (اندازه_t)شمېر - 1;
        أرجع true;
    } وإلا {
        أرجع false;
    }
}

ساكن مضمن bool
scanner_oct(بنية scanner *s, uint8_t *بهر)
{
    uint8_t i = 0;
    uint8_t c = 0;
    لكل (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        إذا (c < 040) {
            c = c * 8 + (uint8_t)(scanner_بل(s) - '0');
        } وإلا {
            /* Consume valid digit, but mark result as invalid */
            scanner_بل(s);
            *بهر = c;
            أرجع false;
        }
    }
    *بهر = c;
    أرجع i > 0;
}

ساكن مضمن bool
scanner_hex(بنية scanner *s, uint8_t *بهر)
{
    uint8_t i;
    لكل (i = 0, *بهر = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        ثابت حرف c = scanner_بل(s);
        ثابت حرف دننه_واټن = (حرف) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *بهر = *بهر * 16 + (uint8_t)(c - دننه_واټن);
    }
    أرجع i > 0;
}

ساكن مضمن صحيح
scanner_dec_int64(بنية scanner *s, int64_t *بهر)
{
    uint64_t val = 0;
    ثابت صحيح شمېر =
        parse_dec_منزل_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    إذا (شمېر > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        إذا (val > INT64_MAX)
            أرجع -1;
        s->pos += (اندازه_t)شمېر;
        *بهر = (int64_t)val;
    }
    أرجع شمېر;
}

ساكن مضمن صحيح
scanner_hex_int64(بنية scanner *s, int64_t *بهر)
{
    uint64_t val = 0;
    ثابت صحيح شمېر =
        parse_hex_منزل_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    إذا (شمېر > 0) {
        /* See comment in `scanner_dec_int64()` above */
        إذا (val > INT64_MAX)
            أرجع -1;
        s->pos += (اندازه_t)شمېر;
        *بهر = (int64_t) val;
    }
    أرجع شمېر;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
ساكن مضمن bool
scanner_unicode_کوډ_ټکی(بنية scanner *s, uint32_t *بهر)
{
    إذا (!scanner_chr(s, '{'))
        أرجع false;

    uint32_t cp = 0;
    ثابت صحيح شمېر =
        parse_hex_منزل_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    إذا (شمېر > 0)
        s->pos += (اندازه_t)شمېر;

    /* Try to consume everything within the string until the next `}` */
    ثابت اندازه_t وروستی_valid = s->pos;
    طالما (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_بل(s);
    }

    إذا (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *بهر = cp;
        أرجع (شمېر > 0 && s->pos == وروستی_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = وروستی_valid;
    أرجع false;
}

/* Basic detection of wrong character encoding based on the first bytes */
ساكن مضمن bool
scanner_check_supported_char_encoding(بنية scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    إذا (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        أرجع true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    إذا (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        أرجع false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    إذا (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        أرجع false;
    }

    أرجع true;
}
