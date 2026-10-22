/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */
#आशय once

#समावेश "config.h"

#समावेश <stdbool.h>
#समावेश <stddef.h>
#समावेश <stdint.h>
#समावेश <string.h>

#समावेश "context.h"
#समावेश "darray.h"
#समावेश "messages-codes.h"
#समावेश "utils.h"
#समावेश "util-numbers.h"
#समावेश "utf8.h"

/* Point to some substring in the file; used to avoid copying. */
काष्ठा sval {
    माप_प्रकार len;
    स्थिर अक्षर *start ATTR_COUNTED_BY(len);
};
प्रकार darray(काष्ठा sval) darray_sval;

अटल अंतरभूत bool
svaleq(काष्ठा sval s1, काष्ठा sval s2)
{
    वापस s1.len == s2.len && स_भेद(s1.start, s2.start, s1.len) == 0;
}

अटल अंतरभूत bool
isvaleq(काष्ठा sval s1, काष्ठा sval s2)
{
    वापस s1.len == s2.len && istrncmp(s1.start, s2.start, s1.len) == 0;
}

अटल अंतरभूत bool
svaleq_prefix(काष्ठा sval s1, काष्ठा sval s2)
{
    वापस s1.len <= s2.len && स_भेद(s1.start, s2.start, s1.len) == 0;
}

#घोषणा SVAL(__start, __len) (काष्ठा sval){ .start = (__start), .len = (__len) }
#घोषणा SVAL_LIT(literal) SVAL(literal, माप(literal) - 1)
#घोषणा SVAL_INIT(literal) { .start = (literal), .len = माप(literal) - 1 }

/* A line:column location in the input string (1-based). */
काष्ठा scanner_loc {
    माप_प्रकार पंक्ति, column;
};

काष्ठा scanner {
    माप_प्रकार pos;
    माप_प्रकार len;
    स्थिर अक्षर *s ATTR_COUNTED_BY(len);
    /*
     * Internal buffer.
     * Since this is used to handle paths that are possibly absolute, in theory
     * we should size it to PATH_MAX. However it is very unlikely to reach such
     * long paths in our context.
     */
    अक्षर buf[1024];
    माप_प्रकार buf_pos;
    /* The position of the start of the current token. */
    माप_प्रकार token_pos;
    /* Cached values used by scanner_token_location. */
    माप_प्रकार cached_pos;
    काष्ठा scanner_loc cached_loc;
    स्थिर अक्षर *file_name;
    काष्ठा xkb_context *ctx;
    व्योम *priv;
};

/* Compute the line:column location for the current token (slow). */
काष्ठा scanner_loc
scanner_token_location(काष्ठा scanner *s);

#घोषणा scanner_log_with_code(scanner, level, verbosity, log_संदेश_id, fmt, ...) करो { \
    काष्ठा scanner_loc loc = scanner_token_location((scanner));                     \
    xkb_log_with_code((scanner)->ctx, (level), verbosity, log_संदेश_id,               \
                      "%s:%zu:%zu: " fmt "\n",                                      \
                      (scanner)->file_name,                                         \
                      loc.पंक्ति,                                                     \
                      loc.column, ##__VA_ARGS__);                                   \
} जबतक(0)

#घोषणा scanner_err(scanner, id, fmt, ...)              \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_ERROR, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#घोषणा scanner_warn(scanner, id, fmt, ...)               \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, \
                          XKB_LOG_VERBOSITY_MINIMAL, id, fmt, ##__VA_ARGS__)

#घोषणा scanner_vrb(scanner, verbosity, id, fmt, ...)                    \
    scanner_log_with_code(scanner, XKB_LOG_LEVEL_WARNING, verbosity, id, \
                          fmt, ##__VA_ARGS__)

अटल अंतरभूत व्योम
scanner_init(काष्ठा scanner *s, काष्ठा xkb_context *ctx,
             स्थिर अक्षर *string, माप_प्रकार len, स्थिर अक्षर *file_name,
             व्योम *priv)
{
    s->s = string;
    s->len = len;
    s->pos = 0;
    s->token_pos = 0;
    s->cached_pos = 0;
    s->cached_loc.पंक्ति = s->cached_loc.column = 1;
    s->file_name = file_name;
    s->ctx = ctx;
    s->priv = priv;
}

अटल अंतरभूत अक्षर
scanner_peek(काष्ठा scanner *s)
{
    अगर (unlikely(s->pos >= s->len))
        वापस '\0';
    वापस s->s[s->pos];
}

अटल अंतरभूत bool
scanner_eof(काष्ठा scanner *s)
{
    वापस s->pos >= s->len;
}

अटल अंतरभूत bool
scanner_eol(काष्ठा scanner *s)
{
    वापस scanner_peek(s) == '\n';
}

अटल अंतरभूत व्योम
scanner_skip_to_eol(काष्ठा scanner *s)
{
    स्थिर अक्षर *nl = स_प्रथम(s->s + s->pos, '\n', s->len - s->pos);
    स्थिर माप_प्रकार new_pos = nl ? (माप_प्रकार) (nl - s->s) : s->len;
    s->pos = new_pos;
}

अटल अंतरभूत अक्षर
scanner_अग्र(काष्ठा scanner *s)
{
    अगर (unlikely(scanner_eof(s)))
        वापस '\0';
    वापस s->s[s->pos++];
}

अटल अंतरभूत bool
scanner_chr(काष्ठा scanner *s, अक्षर ch)
{
    अगर (likely(scanner_peek(s) != ch))
        वापस false;
    s->pos++;
    वापस true;
}

अटल अंतरभूत bool
scanner_str(काष्ठा scanner *s, स्थिर अक्षर *string, माप_प्रकार len)
{
    अगर (s->len - s->pos < len)
        वापस false;
    अगर (स_भेद(s->s + s->pos, string, len) != 0)
        वापस false;
    s->pos += len;
    वापस true;
}

#घोषणा scanner_lit(s, literal) scanner_str(s, literal, माप(literal) - 1)

अटल अंतरभूत bool
scanner_rules_is_ident(अक्षर ch)
{
    वापस is_graph(ch) && ch != '\\';
}

/*
 * A keyword/operator is only valid when it is followed by a character
 * which cannot continue the token.
 */
अटल अंतरभूत bool
scanner_rules_str_token(काष्ठा scanner *s, स्थिर अक्षर *string, माप_प्रकार len)
{
    अगर (!scanner_str(s, string, len)) {
        वापस false;
    }
    अगर (scanner_rules_is_ident(scanner_peek(s))) {
        /* Backtrack */
        s->pos -= len;
        वापस false;
    }
    वापस true;
}

#घोषणा scanner_rules_lit_token(s, literal) \
    scanner_rules_str_token((s), (literal), माप(literal) - 1)

अटल अंतरभूत माप_प्रकार
scanner_rules_ident_token(काष्ठा scanner *s)
{
    माप_प्रकार len = 0;
    जबतक (!scanner_eof(s) && scanner_rules_is_ident(s->s[s->pos])) {
        s->pos++;
        len++;
    }
    वापस len;
}

अटल अंतरभूत bool
scanner_buf_append(काष्ठा scanner *s, अक्षर ch)
{
    अगर (s->buf_pos + 1 >= माप(s->buf))
        वापस false;
    s->buf[s->buf_pos++] = ch;
    वापस true;
}

अटल अंतरभूत bool
scanner_buf_appends(काष्ठा scanner *s, स्थिर अक्षर *str)
{
    स्थिर पूर्णांक ret = snprintf(s->buf + s->buf_pos,
                             माप(s->buf) - s->buf_pos,
                             "%s", str);
    अगर (ret < 0 || (माप_प्रकार)ret >= माप(s->buf) - s->buf_pos)
        वापस false;
    s->buf_pos += (माप_प्रकार)ret;
    वापस true;
}

अटल अंतरभूत bool
scanner_buf_appends_code_point(काष्ठा scanner *s, uint32_t c)
{
    /* Up to 4 bytes + NULL */
    अगर (s->buf_pos + 5 <= माप(s->buf)) {
        uint8_t गिनती = utf32_to_utf8(c, s->buf + s->buf_pos);
        अगर (गिनती == 0) {
            /* Handle encoding failure with U+FFFD REPLACEMENT CHARACTER */
            गिनती = utf32_to_utf8(0xfffd, s->buf + s->buf_pos);
        }
        अगर (गिनती == 0)
            वापस false;
        /* `count` counts the NULL byte */
        s->buf_pos += (माप_प्रकार)गिनती - 1;
        वापस true;
    } अन्यथा {
        वापस false;
    }
}

अटल अंतरभूत bool
scanner_oct(काष्ठा scanner *s, uint8_t *out)
{
    uint8_t i = 0;
    uint8_t c = 0;
    क्रम (; scanner_peek(s) >= '0' && scanner_peek(s) <= '7' && i < 4; i++) {
        /* Test overflow */
        अगर (c < 040) {
            c = c * 8 + (uint8_t)(scanner_अग्र(s) - '0');
        } अन्यथा {
            /* Consume valid digit, but mark result as invalid */
            scanner_अग्र(s);
            *out = c;
            वापस false;
        }
    }
    *out = c;
    वापस i > 0;
}

अटल अंतरभूत bool
scanner_hex(काष्ठा scanner *s, uint8_t *out)
{
    uint8_t i;
    क्रम (i = 0, *out = 0; is_xdigit(scanner_peek(s)) && i < 2; i++) {
        स्थिर अक्षर c = scanner_अग्र(s);
        स्थिर अक्षर offset = (अक्षर) (c >= '0' && c <= '9' ? '0' :
                                    c >= 'a' && c <= 'f' ? 'a' - 10
                                                         : 'A' - 10);
        *out = *out * 16 + (uint8_t)(c - offset);
    }
    वापस i > 0;
}

अटल अंतरभूत पूर्णांक
scanner_dec_int64(काष्ठा scanner *s, int64_t *out)
{
    uint64_t val = 0;
    स्थिर पूर्णांक गिनती =
        parse_dec_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    अगर (गिनती > 0) {
        /*
         * NOTE: Since the sign is handled as a token, we parse only *positive*
         *       values here. So that means that *we cannot parse INT64_MIN*,
         *       because abs(INT64_MIN) > INT64_MAX. But we use 64-bit integers
         *       only to avoid under/overflow in expressions: we do not use
         *       64-bit integers in the keymap, only up to 32 bits. So we expect
         *       to parse only 32 bits integers in realistic keymap files and
         *       the limitation should not be an issue.
         */
        अगर (val > INT64_MAX)
            वापस -1;
        s->pos += (माप_प्रकार)गिनती;
        *out = (int64_t)val;
    }
    वापस गिनती;
}

अटल अंतरभूत पूर्णांक
scanner_hex_int64(काष्ठा scanner *s, int64_t *out)
{
    uint64_t val = 0;
    स्थिर पूर्णांक गिनती =
        parse_hex_to_uint64_t(s->s + s->pos, s->len - s->pos, &val);
    अगर (गिनती > 0) {
        /* See comment in `scanner_dec_int64()` above */
        अगर (val > INT64_MAX)
            वापस -1;
        s->pos += (माप_प्रकार)गिनती;
        *out = (int64_t) val;
    }
    वापस गिनती;
}

/** Parser for the {NNNN} part of a Unicode escape sequences */
अटल अंतरभूत bool
scanner_unicode_code_point(काष्ठा scanner *s, uint32_t *out)
{
    अगर (!scanner_chr(s, '{'))
        वापस false;

    uint32_t cp = 0;
    स्थिर पूर्णांक गिनती =
        parse_hex_to_uint32_t(s->s + s->pos, s->len - s->pos, &cp);
    अगर (गिनती > 0)
        s->pos += (माप_प्रकार)गिनती;

    /* Try to consume everything within the string until the next `}` */
    स्थिर माप_प्रकार last_valid = s->pos;
    जबतक (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '"' &&
           scanner_peek(s) != '}') {
        scanner_अग्र(s);
    }

    अगर (scanner_chr(s, '}')) {
        /* End of the escape sequence; code point may be invalid */
        *out = cp;
        वापस (गिनती > 0 && s->pos == last_valid + 1 && cp <= 0x10ffff);
    }

    /* No closing `}` within the string: rollback to last valid position */
    s->pos = last_valid;
    वापस false;
}

/* Basic detection of wrong character encoding based on the first bytes */
अटल अंतरभूत bool
scanner_check_supported_char_encoding(काष्ठा scanner *scanner)
{
    /* Skip UTF-8 encoded BOM (U+FEFF)
     * See: https://www.unicode.org/faq/utf_bom.html#bom5 */
    अगर (scanner_str(scanner, "\xef\xbb\xbf", 3) || scanner->len < 2) {
        /* Assume UTF-8 encoding or trivial short input */
        वापस true;
    }

    /* Early detection of wrong file encoding, e.g. UTF-16 or UTF-32 */
    अगर (scanner->s[0] == '\0' || scanner->s[1] == '\0') {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected NULL character.");
        वापस false;
    }
    /* Enforce the first character to be ASCII.
       See the note before the use of this function, that explains the relevant
       parts of the grammars of rules, keymap components and Compose. */
    अगर (!is_ascii(scanner->s[0])) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "unexpected non-ASCII character.");
        वापस false;
    }

    वापस true;
}
