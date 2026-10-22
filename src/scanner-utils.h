/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

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
ساخت sval {
    سائز_t len;
    ثابت حرف *آغاز ATTR_COUNTED_BY(len);
};
قسمتعریف darray(ساخت sval) darray_sval;

ساکن اندرونی bool
svaleq(ساخت sval s1, ساخت sval s2)
{
    واپس s1.len == s2.len && memcmp(s1.آغاز, s2.آغاز, s1.len) == 0;
}

ساکن اندرونی bool
isvaleq(ساخت sval s1, ساخت sval s2)
{
    واپس s1.len == s2.len && istrncmp(s1.آغاز, s2.آغاز, s1.len) == 0;
}

ساکن اندرونی bool
svaleq_سابقہ(ساخت sval s1, ساخت sval s2)
{
    واپس s1.len <= s2.len && memcmp(s1.آغاز, s2.آغاز, s1.len) == 0;
}

#تعریف_کرو SVAL(__start, __len) (ساخت sval){ .آغاز = (__start), .len = (__len) }
#تعریف_کرو SVAL_LIT(literal) SVAL(literal, حجم(literal) - 1)
#تعریف_کرو SVAL_INIT(literal) { .آغاز = (literal), .len = حجم(literal) - 1 }

/* A line:column location in the input string (1-based). */
ساخت scanner_loc {
    سائز_t سطر, عمود;
};

ساخت scanner {
    سائز_t pos;
    سائز_t len;
    ثابت حرف *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    حرف buf[1024];
    سائز_t buf_pos;
    /* The position of the start of the current token. */
    سائز_t token_pos;
    /* Cached values used by scanner_token_location. */
    سائز_t cached_pos;
    ساخت scanner_loc cached_loc;
    ثابت حرف *file_name;
    ساخت xkb_context *ctx;
    خالی *priv;
};

/* Compute the line:column location for the current token (slow). */
ساخت scanner_loc
scanner_token_location(ساخت scanner *s);

#تعریف_کرو scanner_log_with_رمز(scanner, level, verbosity, log_پیغام_id, fmt, ...) کرو { \
    ساخت scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_رمز((scanner)->ctx, (level), verbosity, log_پیغام_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.سطر,                                                     \
                      loc.عمود, ##__VA_ARGS__);                                   \
} جب(0)

#تعریف_کرو scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_رمز(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_اقل, id, fmt, ##__VA_ARGS__)

#تعریف_کرو scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_رمز(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_اقل, id, fmt, ##__VA_ARGS__)

#تعریف_کرو scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_رمز(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

ساکن اندرونی خالی
scanner_init(ساخت scanner *s, ساخت xkb_context *ctx,
             ثابت حرف *string, سائز_t len, ثابت حرف *file_name,
             خالی *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.سطر = s->cached_loc.عمود = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

ساکن اندرونی حرف
scanner_peek(ساخت scanner *s)
{
    اگر (unlikely(s->pos >= s->len))
        واپس '\0';
    واپس s->s[s->pos];
}

ساکن اندرونی bool
scanner_eof(ساخت scanner *s)
{
    واپس s->pos >= s->len;
}

ساکن اندرونی bool
scanner_eol(ساخت scanner *s)
{
    واپس scanner_peek(s) == '\n';
}

ساکن اندرونی خالی
scanner_skip_مقصد_eol(ساخت scanner *s)
{
    ثابت حرف *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    ثابت سائز_t new_pos = nl ? (سائز_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

ساکن اندرونی حرف
scanner_اگلا(ساخت scanner *s)
{
    اگر (unlikely(scanner_eof(s)))
        واپس '\0';
    واپس s->s[s->pos++];
}

ساکن اندرونی bool
scanner_chr(ساخت scanner *s, حرف ch)
{
    اگر (likely(scanner_peek(s) != ch))
        واپس false;
    s->pos++;
    واپس true;
}

ساکن اندرونی bool
scanner_str(ساخت scanner *s, ثابت حرف *string, سائز_t len)
{
    اگر (s->len - s->pos < len)
        واپس false;
    اگر (memcmp(s->s + s->pos, string, len) != 0)
        واپس false;
    s->pos += len;
    واپس true;
}

#تعریف_کرو scanner_lit(s, literal) scanner_str(s, literal, حجم(literal) - 1)

ساکن اندرونی bool
scanner_قواعد_is_ident(حرف ch)
{
    واپس is_گراف(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
ساکن اندرونی bool
scanner_قواعد_str_token(ساخت scanner *s, ثابت حرف *string, سائز_t len)
{
    اگر (!scanner_str(s, string, len)) {
        واپس false;
    }
    اگر (scanner_قواعد_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        واپس false;
    }
    واپس true;
}

#تعریف_کرو scanner_قواعد_lit_token(s, literal) \
    scanner_قواعد_str_token((s), (literal), حجم(literal) - 1)

ساکن اندرونی سائز_t
scanner_قواعد_ident_token(ساخت scanner *s)
{
    سائز_t len = 0;
    جب (!scanner_eof(s) && scanner_قواعد_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    واپس len;
}

ساکن اندرونی bool
scanner_buf_append(ساخت scanner *s, حرف ch)
{
    اگر (s->buf_pos + 1 >= حجم(s->buf))
        واپس false;
    s->buf[s->buf_pos++] = ch;
    واپس true;
}

ساکن اندرونی bool
scanner_buf_appends(ساخت scanner *s, ثابت حرف *str)
{
    ثابت صحیح ret = snprintf(s->buf + s->buf_pos,
                             حجم(s->buf) - s->buf_pos,
                             "%s", str);
    اگر (ret < 0 || (سائز_t)ret >= حجم(s->buf) - s->buf_pos)
        واپس false;
    s->buf_pos += (سائز_t)ret;
    واپس true;
}

ساکن اندرونی bool
scanner_buf_appends_رمز_نقطہ(ساخت scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    اگر (s->buf_pos + 5 <= حجم(s->buf)) {
        uint8_t گنتی = utf32_مقصد_utf8(c, s->buf + s->buf_pos);
        اگر (گنتی == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            گنتی = utf32_مقصد_utf8(0xfffd, s->buf + s->buf_pos);
        }
        اگر (گنتی == 0)
            واپس false;
        /* `count` counts the NULL byte */
        s->buf_pos += (سائز_t)گنتی - 1;
        واپس true;
    } ورنہ {
        واپس false;
    }
}

ساکن اندرونی bool
scanner_oct(ساخت scanner *s, uint8_t *برآمد)
{
    uint8_t i = 0;
    uint8_t c = 0;
    برائے (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        اگر (c < 040) {
            c = c * 8 + (uint8_t)(scanner_اگلا(s) - '0');
        } ورنہ {
            /* Consume valid digit, but mark result as invalid */
            scanner_اگلا(s);
            *برآمد = c;
            واپس false;
        }
    }
    *برآمد = c;
    واپس i > 0;
}

ساکن اندرونی bool
scanner_hex(ساخت scanner *s, uint8_t *برآمد)
{
    uint8_t i;
    برائے (i = 0, *برآمد = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        ثابت حرف c = scanner_اگلا(s);
        ثابت حرف انحراف = (حرف) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *برآمد = *برآمد * 16 + (uint8_t)(c - انحراف);
    }
    واپس i > 0;
}

ساکن اندرونی صحیح
scanner_dec_int64(ساخت scanner *s, int64_t *برآمد)
{
    uint64_t val = 0;
    ثابت صحیح گنتی =
        parse_dec_مقصد_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    اگر (گنتی > 0) {
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
            واپس -1;
        s->pos += (سائز_t)گنتی;
        *برآمد = (int64_t)val;
    }
    واپس گنتی;
}

ساکن اندرونی صحیح
scanner_hex_int64(ساخت scanner *s, int64_t *برآمد)
{
    uint64_t val = 0;
    ثابت صحیح گنتی =
        parse_hex_مقصد_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    اگر (گنتی > 0) {
        /* See comment in `scanner_dec_int64()` above */
        اگر (val > INT64_MAX)
            واپس -1;
        s->pos += (سائز_t)گنتی;
        *برآمد = (int64_t) val;
    }
    واپس گنتی;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
ساکن اندرونی bool
scanner_unicode_رمز_نقطہ(ساخت scanner *s, uint32_t *برآمد)
{
    اگر (!scanner_chr(s, '{'))
        واپس false;

    uint32_t cp = 0;
    ثابت صحیح گنتی =
        parse_hex_مقصد_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    اگر (گنتی > 0)
        s->pos += (سائز_t)گنتی;

    /* Try to consume everything within the string until the next `}` */
    ثابت سائز_t آخری_valid = s->pos;
    جب (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_اگلا(s);
    }

    اگر (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *برآمد = cp;
        واپس (گنتی > 0 && s->pos == آخری_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = آخری_valid;
    واپس false;
}

/* Basic detection of wrong character encoding based on the first bytes */
ساکن اندرونی bool
scanner_check_supported_char_encoding(ساخت scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    اگر (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        واپس true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    اگر (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        واپس false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    اگر (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        واپس false;
    }

    واپس true;
}
