/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#సూచన once

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <stddef.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "context.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "util-numbers.h"
#అన్తర్భావ "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
నిర్మాణం sval {
    కొలత_t len;
    స్థిరరాశి అక్షరం *ప్రారంభం ATTR_COUNTED_BY(len);
};
రకనిర్వచనం darray(నిర్మాణం sval) darray_sval;

స్థిర అంతఃవరుస bool
svaleq(నిర్మాణం sval s1, నిర్మాణం sval s2)
{
    తిరిగి s1.len == s2.len && memcmp(s1.ప్రారంభం, s2.ప్రారంభం, s1.len) == 0;
}

స్థిర అంతఃవరుస bool
isvaleq(నిర్మాణం sval s1, నిర్మాణం sval s2)
{
    తిరిగి s1.len == s2.len && istrncmp(s1.ప్రారంభం, s2.ప్రారంభం, s1.len) == 0;
}

స్థిర అంతఃవరుస bool
svaleq_ఉపసర్గ(నిర్మాణం sval s1, నిర్మాణం sval s2)
{
    తిరిగి s1.len <= s2.len && memcmp(s1.ప్రారంభం, s2.ప్రారంభం, s1.len) == 0;
}

#నిర్వచించు SVAL(__start, __len) (నిర్మాణం sval){ .ప్రారంభం = (__start), .len = (__len) }
#నిర్వచించు SVAL_LIT(literal) SVAL(literal, పరిమాణం(literal) - 1)
#నిర్వచించు SVAL_INIT(literal) { .ప్రారంభం = (literal), .len = పరిమాణం(literal) - 1 }

/* A line:column location in the input string (1-based). */
నిర్మాణం scanner_loc {
    కొలత_t పంక్తి, స్తంభం;
};

నిర్మాణం scanner {
    కొలత_t pos;
    కొలత_t len;
    స్థిరరాశి అక్షరం *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    అక్షరం buf[1024];
    కొలత_t buf_pos;
    /* The position of the start of the current token. */
    కొలత_t token_pos;
    /* Cached values used by scanner_token_location. */
    కొలత_t cached_pos;
    నిర్మాణం scanner_loc cached_loc;
    స్థిరరాశి అక్షరం *file_name;
    నిర్మాణం xkb_context *ctx;
    శూన్య *priv;
};

/* Compute the line:column location for the current token (slow). */
నిర్మాణం scanner_loc
scanner_token_location(నిర్మాణం scanner *s);

#నిర్వచించు scanner_log_with_సంకేతం(scanner, level, verbosity, log_సందేశం_id, fmt, ...) చేయి { \
    నిర్మాణం scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_సంకేతం((scanner)->ctx, (level), verbosity, log_సందేశం_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.పంక్తి,                                                     \
                      loc.స్తంభం, ##__VA_ARGS__);                                   \
} అయితే(0)

#నిర్వచించు scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_సంకేతం(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_కనీస, id, fmt, ##__VA_ARGS__)

#నిర్వచించు scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_సంకేతం(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_కనీస, id, fmt, ##__VA_ARGS__)

#నిర్వచించు scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_సంకేతం(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

స్థిర అంతఃవరుస శూన్య
scanner_init(నిర్మాణం scanner *s, నిర్మాణం xkb_context *ctx,
             స్థిరరాశి అక్షరం *string, కొలత_t len, స్థిరరాశి అక్షరం *file_name,
             శూన్య *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.పంక్తి = s->cached_loc.స్తంభం = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

స్థిర అంతఃవరుస అక్షరం
scanner_peek(నిర్మాణం scanner *s)
{
    ఒకవేళ (unlikely(s->pos >= s->len))
        తిరిగి '\0';
    తిరిగి s->s[s->pos];
}

స్థిర అంతఃవరుస bool
scanner_eof(నిర్మాణం scanner *s)
{
    తిరిగి s->pos >= s->len;
}

స్థిర అంతఃవరుస bool
scanner_eol(నిర్మాణం scanner *s)
{
    తిరిగి scanner_peek(s) == '\n';
}

స్థిర అంతఃవరుస శూన్య
scanner_skip_గమ్యం_eol(నిర్మాణం scanner *s)
{
    స్థిరరాశి అక్షరం *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    స్థిరరాశి కొలత_t new_pos = nl ? (కొలత_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

స్థిర అంతఃవరుస అక్షరం
scanner_తదుపరి(నిర్మాణం scanner *s)
{
    ఒకవేళ (unlikely(scanner_eof(s)))
        తిరిగి '\0';
    తిరిగి s->s[s->pos++];
}

స్థిర అంతఃవరుస bool
scanner_chr(నిర్మాణం scanner *s, అక్షరం ch)
{
    ఒకవేళ (likely(scanner_peek(s) != ch))
        తిరిగి false;
    s->pos++;
    తిరిగి true;
}

స్థిర అంతఃవరుస bool
scanner_str(నిర్మాణం scanner *s, స్థిరరాశి అక్షరం *string, కొలత_t len)
{
    ఒకవేళ (s->len - s->pos < len)
        తిరిగి false;
    ఒకవేళ (memcmp(s->s + s->pos, string, len) != 0)
        తిరిగి false;
    s->pos += len;
    తిరిగి true;
}

#నిర్వచించు scanner_lit(s, literal) scanner_str(s, literal, పరిమాణం(literal) - 1)

స్థిర అంతఃవరుస bool
scanner_నియమాలు_is_ident(అక్షరం ch)
{
    తిరిగి is_లేఖాచిత్రం(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
స్థిర అంతఃవరుస bool
scanner_నియమాలు_str_token(నిర్మాణం scanner *s, స్థిరరాశి అక్షరం *string, కొలత_t len)
{
    ఒకవేళ (!scanner_str(s, string, len)) {
        తిరిగి false;
    }
    ఒకవేళ (scanner_నియమాలు_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        తిరిగి false;
    }
    తిరిగి true;
}

#నిర్వచించు scanner_నియమాలు_lit_token(s, literal) \
    scanner_నియమాలు_str_token((s), (literal), పరిమాణం(literal) - 1)

స్థిర అంతఃవరుస కొలత_t
scanner_నియమాలు_ident_token(నిర్మాణం scanner *s)
{
    కొలత_t len = 0;
    అయితే (!scanner_eof(s) && scanner_నియమాలు_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    తిరిగి len;
}

స్థిర అంతఃవరుస bool
scanner_buf_append(నిర్మాణం scanner *s, అక్షరం ch)
{
    ఒకవేళ (s->buf_pos + 1 >= పరిమాణం(s->buf))
        తిరిగి false;
    s->buf[s->buf_pos++] = ch;
    తిరిగి true;
}

స్థిర అంతఃవరుస bool
scanner_buf_appends(నిర్మాణం scanner *s, స్థిరరాశి అక్షరం *str)
{
    స్థిరరాశి పూర్ణాంకం ret = snprintf(s->buf + s->buf_pos,
                             పరిమాణం(s->buf) - s->buf_pos,
                             "%s", str);
    ఒకవేళ (ret < 0 || (కొలత_t)ret >= పరిమాణం(s->buf) - s->buf_pos)
        తిరిగి false;
    s->buf_pos += (కొలత_t)ret;
    తిరిగి true;
}

స్థిర అంతఃవరుస bool
scanner_buf_appends_సంకేతం_బిందువు(నిర్మాణం scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    ఒకవేళ (s->buf_pos + 5 <= పరిమాణం(s->buf)) {
        uint8_t లెక్క = utf32_గమ్యం_utf8(c, s->buf + s->buf_pos);
        ఒకవేళ (లెక్క == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            లెక్క = utf32_గమ్యం_utf8(0xfffd, s->buf + s->buf_pos);
        }
        ఒకవేళ (లెక్క == 0)
            తిరిగి false;
        /* `count` counts the NULL byte */
        s->buf_pos += (కొలత_t)లెక్క - 1;
        తిరిగి true;
    } లేకపోతే {
        తిరిగి false;
    }
}

స్థిర అంతఃవరుస bool
scanner_oct(నిర్మాణం scanner *s, uint8_t *బహిర్గతం)
{
    uint8_t i = 0;
    uint8_t c = 0;
    కొరకు (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        ఒకవేళ (c < 040) {
            c = c * 8 + (uint8_t)(scanner_తదుపరి(s) - '0');
        } లేకపోతే {
            /* Consume valid digit, but mark result as invalid */
            scanner_తదుపరి(s);
            *బహిర్గతం = c;
            తిరిగి false;
        }
    }
    *బహిర్గతం = c;
    తిరిగి i > 0;
}

స్థిర అంతఃవరుస bool
scanner_hex(నిర్మాణం scanner *s, uint8_t *బహిర్గతం)
{
    uint8_t i;
    కొరకు (i = 0, *బహిర్గతం = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        స్థిరరాశి అక్షరం c = scanner_తదుపరి(s);
        స్థిరరాశి అక్షరం స్థానభ్రంశం = (అక్షరం) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *బహిర్గతం = *బహిర్గతం * 16 + (uint8_t)(c - స్థానభ్రంశం);
    }
    తిరిగి i > 0;
}

స్థిర అంతఃవరుస పూర్ణాంకం
scanner_dec_int64(నిర్మాణం scanner *s, int64_t *బహిర్గతం)
{
    uint64_t val = 0;
    స్థిరరాశి పూర్ణాంకం లెక్క =
        parse_dec_గమ్యం_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ఒకవేళ (లెక్క > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        ఒకవేళ (val > INT64_MAX)
            తిరిగి -1;
        s->pos += (కొలత_t)లెక్క;
        *బహిర్గతం = (int64_t)val;
    }
    తిరిగి లెక్క;
}

స్థిర అంతఃవరుస పూర్ణాంకం
scanner_hex_int64(నిర్మాణం scanner *s, int64_t *బహిర్గతం)
{
    uint64_t val = 0;
    స్థిరరాశి పూర్ణాంకం లెక్క =
        parse_hex_గమ్యం_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ఒకవేళ (లెక్క > 0) {
        /* See comment in `scanner_dec_int64()` above */
        ఒకవేళ (val > INT64_MAX)
            తిరిగి -1;
        s->pos += (కొలత_t)లెక్క;
        *బహిర్గతం = (int64_t) val;
    }
    తిరిగి లెక్క;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
స్థిర అంతఃవరుస bool
scanner_unicode_సంకేతం_బిందువు(నిర్మాణం scanner *s, uint32_t *బహిర్గతం)
{
    ఒకవేళ (!scanner_chr(s, '{'))
        తిరిగి false;

    uint32_t cp = 0;
    స్థిరరాశి పూర్ణాంకం లెక్క =
        parse_hex_గమ్యం_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    ఒకవేళ (లెక్క > 0)
        s->pos += (కొలత_t)లెక్క;

    /* Try to consume everything within the string until the next `}` */
    స్థిరరాశి కొలత_t చివరి_valid = s->pos;
    అయితే (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_తదుపరి(s);
    }

    ఒకవేళ (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *బహిర్గతం = cp;
        తిరిగి (లెక్క > 0 && s->pos == చివరి_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = చివరి_valid;
    తిరిగి false;
}

/* Basic detection of wrong character encoding based on the first bytes */
స్థిర అంతఃవరుస bool
scanner_check_supported_char_encoding(నిర్మాణం scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    ఒకవేళ (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        తిరిగి true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    ఒకవేళ (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        తిరిగి false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    ఒకవేళ (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        తిరిగి false;
    }

    తిరిగి true;
}
