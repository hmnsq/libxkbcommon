/*
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "scanner-utils.h"
#include "xkbcomp-priv.h"
#include "parser-priv.h"

static const char DECIMAL_SEPARATOR = '.';

static bool
number(struct scanner *s, int64_t *out, số_nguyên *out_tok)
{
    nếu (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        case -1:
            *out_tok = ERROR_TOK;
            trả_về true;
        case 0:
            trả_về false;
        default:
            *out_tok = INTEGER;
            trả_về true;
        }
    }
    không_thì {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        case -1:
            *out_tok = ERROR_TOK;
            trả_về true;
        case 0:
            trả_về false;
        case 1:
            is_digit = true;
            break;
        default:
            ;
        }
        nếu (scanner_chr(s, DECIMAL_SEPARATOR)) {
            /*
             * Since the parser does not use floats, we do not care of the
             * actual floating-point number value: we just need to ensure that
             * the syntax is correct. So just truncate it!
             *
             * Note that `strtold` would not work reliably in our context,
             * because it depends on the locale for e.g. the decimal separator
             * (e.g. a period or a comma) and requires a null-terminated string,
             * which we cannot guarantee.
             */
            int64_t dec;
            nếu (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                trả_về true;
            }
            *out_tok = FLOAT;
        } không_thì nếu (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } không_thì {
            *out_tok = INTEGER;
        }
        trả_về true;
    }
}

số_nguyên
_xkbcommon_lex(YYSTYPE *yylval, struct scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    trong_khi (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    nếu (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    nếu (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    nếu (scanner_eof(s)) trả_về END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    nếu (scanner_chr(s, '\"')) {
        trong_khi (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            nếu (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                nếu      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                không_thì nếu (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                không_thì nếu (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                không_thì nếu (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                không_thì nếu (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                không_thì nếu (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                không_thì nếu (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                không_thì nếu (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                không_thì nếu (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                không_thì nếu (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    nếu (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } không_thì {
                        scanner_warn(
                            s, XKB_WARNING_INVALID_UNICODE_ESCAPE_SEQUENCE,
                            "invalid Unicode escape sequence \"%.*s\" "
                            "in string literal",
                            (số_nguyên) (s->pos - start_pos + 1),
                            &s->s[start_pos - 1]
                        );
                        /* Ignore. */
                    }
                }
                không_thì nếu (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (char) o);
                }
                không_thì nếu (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (số_nguyên) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                không_thì {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } không_thì {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        nếu (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            trả_về ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        nếu (!yylval->str)
            trả_về ERROR_TOK;
        trả_về STRING;
    }

    /* Key name literal. */
    nếu (scanner_chr(s, '<')) {
        trong_khi (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        nếu (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            trả_về ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const char *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        trả_về KEYNAME;
    }

    /* Operators and punctuation. */
    nếu (scanner_chr(s, ';')) trả_về SEMI;
    nếu (scanner_chr(s, '{')) trả_về OBRACE;
    nếu (scanner_chr(s, '}')) trả_về CBRACE;
    nếu (scanner_chr(s, '=')) trả_về EQUALS;
    nếu (scanner_chr(s, '[')) trả_về OBRACKET;
    nếu (scanner_chr(s, ']')) trả_về CBRACKET;
    nếu (scanner_chr(s, '(')) trả_về OPAREN;
    nếu (scanner_chr(s, ')')) trả_về CPAREN;
    nếu (scanner_chr(s, '.')) trả_về DOT;
    nếu (scanner_chr(s, ',')) trả_về COMMA;
    nếu (scanner_chr(s, '+')) trả_về PLUS;
    nếu (scanner_chr(s, '-')) trả_về MINUS;
    nếu (scanner_chr(s, '*')) trả_về TIMES;
    nếu (scanner_chr(s, '/')) trả_về DIVIDE;
    nếu (scanner_chr(s, '!')) trả_về EXCLAM;
    nếu (scanner_chr(s, '~')) trả_về INVERT;

    số_nguyên tok = ERROR_TOK;

    /* Identifier. */
    nếu (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        trong_khi (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const char *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        nếu (tok >= 0) trả_về tok;

        yylval->sval = SVAL(start, len);
        trả_về IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    nếu (number(s, &yylval->num, &tok)) {
        nếu (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            trả_về ERROR_TOK;
        }
        trả_về tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    trả_về ERROR_TOK;
}

bool
XkbParseStringInit(struct xkb_context *ctx, struct scanner *scanner,
                   const char *string, size_t len,
                   const char *file_name, const char *map)
{
    scanner_init(scanner, ctx, string, len, file_name, NULL);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, section, comment */
    nếu (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        trả_về false;
    }

    trả_về true;
}

XkbFile *
XkbParseString(struct xkb_context *ctx,
               const struct parser_keymap_config *config,
               const char *string, size_t len,
               const char *file_name, const char *map)
{
    struct scanner scanner;
    nếu (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        trả_về NULL;

    trả_về parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(struct xkb_context *ctx,
                   const struct parser_keymap_config *config,
                   struct scanner *scanner, const char *map, XkbFile **out)
{
    nếu (map) {
        *out = parse(ctx, config, scanner, map);
        trả_về !!(*out);
    } không_thì {
        trả_về parse_next(ctx, config, scanner, out);
    }
}

XkbFile *
XkbParseFile(struct xkb_context *ctx,
             const struct parser_keymap_config *config,
             FILE *file, const char *file_name, const char *map)
{
    bool ok;
    XkbFile *xkb_file;
    char *string;
    size_t size;

    ok = map_file(file, &string, &size);
    nếu (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        trả_về NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    trả_về xkb_file;
}
