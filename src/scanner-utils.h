/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#pragma once

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdbool.h>
#ଅନ୍ତର୍ଭାଵ <stddef.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "util-numbers.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
ସଂରଚନା sval {
    size_t len;
    ଅଚଳ ଅକ୍ଷର *start ATTR_COUNTED_BY(len);
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା darray(ସଂରଚନା sval) darray_sval;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
svaleq(ସଂରଚନା sval s1, ସଂରଚନା sval s2)
{
    ଫେରନ୍ତୁ s1.len == s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
isvaleq(ସଂରଚନା sval s1, ସଂରଚନା sval s2)
{
    ଫେରନ୍ତୁ s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
svaleq_prefix(ସଂରଚନା sval s1, ସଂରଚନା sval s2)
{
    ଫେରନ୍ତୁ s1.len <= s2.len && memcmp(s1.start, s2.start, s1.len) == 0;
}

#define SVAL(__start, __len) (ସଂରଚନା sval){ .start = (__start), .len = (__len) }
#define SVAL_LIT(literal) SVAL(literal, ଆକାର(literal) - 1)
#define SVAL_INIT(literal) { .start = (literal), .len = ଆକାର(literal) - 1 }

/* A line:column location in the input string (1-based). */
ସଂରଚନା scanner_loc {
    size_t ଧାଡ଼ି, column;
};

ସଂରଚନା scanner {
    size_t pos;
    size_t len;
    ଅଚଳ ଅକ୍ଷର *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    ଅକ୍ଷର buf[1024];
    size_t buf_pos;
    /* The position of the start of the current token. */
    size_t token_pos;
    /* Cached values used by scanner_token_location. */
    size_t cached_pos;
    ସଂରଚନା scanner_loc cached_loc;
    ଅଚଳ ଅକ୍ଷର *file_name;
    ସଂରଚନା xkb_context *ctx;
    ଶୂନ୍ୟ *priv;
};

/* Compute the line:column location for the current token (slow). */
ସଂରଚନା scanner_loc
scanner_token_location(ସଂରଚନା scanner *s);

#define scanner_log_with_code(scanner, level, verbosity, log_ସନ୍ଦେଶ_id, fmt, ...) କର { \
    ସଂରଚନା scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_ସନ୍ଦେଶ_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.ଧାଡ଼ି,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} ଯେତେବେଳେ(0)

#define scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#define scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
scanner_init(ସଂରଚନା scanner *s, ସଂରଚନା xkb_context *ctx,
             ଅଚଳ ଅକ୍ଷର *string, size_t len, ଅଚଳ ଅକ୍ଷର *file_name,
             ଶୂନ୍ୟ *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.ଧାଡ଼ି = s->cached_loc.column = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର
scanner_peek(ସଂରଚନା scanner *s)
{
    ଯଦି (unlikely(s->pos >= s->len))
        ଫେରନ୍ତୁ '\0';
    ଫେରନ୍ତୁ s->s[s->pos];
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_eof(ସଂରଚନା scanner *s)
{
    ଫେରନ୍ତୁ s->pos >= s->len;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_eol(ସଂରଚନା scanner *s)
{
    ଫେରନ୍ତୁ scanner_peek(s) == '\n';
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
scanner_skip_to_eol(ସଂରଚନା scanner *s)
{
    ଅଚଳ ଅକ୍ଷର *nl = memchr(s->s + s->pos, '\n', s->len - s->pos);
    ଅଚଳ size_t new_pos = nl ? (size_t) (nl - s->s) : s->len;
    s->pos = new_pos;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅକ୍ଷର
scanner_next(ସଂରଚନା scanner *s)
{
    ଯଦି (unlikely(scanner_eof(s)))
        ଫେରନ୍ତୁ '\0';
    ଫେରନ୍ତୁ s->s[s->pos++];
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_chr(ସଂରଚନା scanner *s, ଅକ୍ଷର ch)
{
    ଯଦି (likely(scanner_peek(s) != ch))
        ଫେରନ୍ତୁ false;
    s->pos++;
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_str(ସଂରଚନା scanner *s, ଅଚଳ ଅକ୍ଷର *string, size_t len)
{
    ଯଦି (s->len - s->pos < len)
        ଫେରନ୍ତୁ false;
    ଯଦି (memcmp(s->s + s->pos, string, len) != 0)
        ଫେରନ୍ତୁ false;
    s->pos += len;
    ଫେରନ୍ତୁ true;
}

#define scanner_lit(s, literal) scanner_str(s, literal, ଆକାର(literal) - 1)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_rules_is_ident(ଅକ୍ଷର ch)
{
    ଫେରନ୍ତୁ is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_rules_str_token(ସଂରଚନା scanner *s, ଅଚଳ ଅକ୍ଷର *string, size_t len)
{
    ଯଦି (!scanner_str(s, string, len)) {
        ଫେରନ୍ତୁ false;
    }
    ଯଦି (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        ଫେରନ୍ତୁ false;
    }
    ଫେରନ୍ତୁ true;
}

#define scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), ଆକାର(literal) - 1)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ size_t
scanner_rules_ident_token(ସଂରଚନା scanner *s)
{
    size_t len = 0;
    ଯେତେବେଳେ (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    ଫେରନ୍ତୁ len;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_buf_append(ସଂରଚନା scanner *s, ଅକ୍ଷର ch)
{
    ଯଦି (s->buf_pos + 1 >= ଆକାର(s->buf))
        ଫେରନ୍ତୁ false;
    s->buf[s->buf_pos++] = ch;
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_buf_appends(ସଂରଚନା scanner *s, ଅଚଳ ଅକ୍ଷର *str)
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = snprintf(s->buf + s->buf_pos,
                             ଆକାର(s->buf) - s->buf_pos,
                             "%s", str);
    ଯଦି (ret < 0 || (size_t)ret >= ଆକାର(s->buf) - s->buf_pos)
        ଫେରନ୍ତୁ false;
    s->buf_pos += (size_t)ret;
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_buf_appends_code_point(ସଂରଚନା scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    ଯଦି (s->buf_pos + 5 <= ଆକାର(s->buf)) {
        uint8_t ଗଣନା_2 = utf32_to_utf8(c, s->buf + s->buf_pos);
        ଯଦି (ଗଣନା_2 == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            ଗଣନା_2 = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        ଯଦି (ଗଣନା_2 == 0)
            ଫେରନ୍ତୁ false;
        /* `count` counts the NULL byte */
        s->buf_pos += (size_t)ଗଣନା_2 - 1;
        ଫେରନ୍ତୁ true;
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ false;
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_oct(ସଂରଚନା scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    ପାଇଁ (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        ଯଦି (c < 040) {
            c = c * 8 + (uint8_t)(scanner_next(s) - '0');
        } ଅନ୍ୟଥା {
            /* Consume valid digit, but mark result as invalid */
            scanner_next(s);
            *out = c;
            ଫେରନ୍ତୁ false;
        }
    }
    *out = c;
    ଫେରନ୍ତୁ i > 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_hex(ସଂରଚନା scanner *s, uint8_t *out)
{
    uint8_t i;
    ପାଇଁ (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        ଅଚଳ ଅକ୍ଷର c = scanner_next(s);
        ଅଚଳ ଅକ୍ଷର offset = (ଅକ୍ଷର) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    ଫେରନ୍ତୁ i > 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
scanner_dec_int64(ସଂରଚନା scanner *s, int64_t *out)
{
    uint64_t val = 0;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ଯଦି (ଗଣନା_2 > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        ଯଦି (val > INT64_MAX)
            ଫେରନ୍ତୁ -1;
        s->pos += (size_t)ଗଣନା_2;
        *out = (int64_t)val;
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
scanner_hex_int64(ସଂରଚନା scanner *s, int64_t *out)
{
    uint64_t val = 0;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    ଯଦି (ଗଣନା_2 > 0) {
        /* See comment in `scanner_dec_int64()` above */
        ଯଦି (val > INT64_MAX)
            ଫେରନ୍ତୁ -1;
        s->pos += (size_t)ଗଣନା_2;
        *out = (int64_t) val;
    }
    ଫେରନ୍ତୁ ଗଣନା_2;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_unicode_code_point(ସଂରଚନା scanner *s, uint32_t *out)
{
    ଯଦି (!scanner_chr(s, '{'))
        ଫେରନ୍ତୁ false;

    uint32_t cp = 0;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    ଯଦି (ଗଣନା_2 > 0)
        s->pos += (size_t)ଗଣନା_2;

    /* Try to consume everything within the string until the next `}` */
    ଅଚଳ size_t last_valid = s->pos;
    ଯେତେବେଳେ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_next(s);
    }

    ଯଦି (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        ଫେରନ୍ତୁ (ଗଣନା_2 > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    ଫେରନ୍ତୁ false;
}

/* Basic detection of wrong character encoding based on the first bytes */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
scanner_check_supported_char_encoding(ସଂରଚନା scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    ଯଦି (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        ଫେରନ୍ତୁ true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    ଯଦି (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        ଫେରନ୍ତୁ false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    ଯଦି (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        ଫେରନ୍ତୁ false;
    }

    ଫେରନ୍ତୁ true;
}
