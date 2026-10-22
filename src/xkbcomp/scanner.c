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
number(struct scanner *s, int64_t *out, 정수 *out_tok)
{
    만약 (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        case -1:
            *out_tok = ERROR_TOK;
            반환 true;
        case 0:
            반환 false;
        default:
            *out_tok = INTEGER;
            반환 true;
        }
    }
    아니면 {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        case -1:
            *out_tok = ERROR_TOK;
            반환 true;
        case 0:
            반환 false;
        case 1:
            is_digit = true;
            break;
        default:
            ;
        }
        만약 (scanner_chr(s, DECIMAL_SEPARATOR)) {
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
            만약 (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                반환 true;
            }
            *out_tok = FLOAT;
        } 아니면 만약 (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } 아니면 {
            *out_tok = INTEGER;
        }
        반환 true;
    }
}

정수
_xkbcommon_lex(YYSTYPE *yylval, struct scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    동안 (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    만약 (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    만약 (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    만약 (scanner_eof(s)) 반환 END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    만약 (scanner_chr(s, '\"')) {
        동안 (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            만약 (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                만약      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                아니면 만약 (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                아니면 만약 (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                아니면 만약 (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                아니면 만약 (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                아니면 만약 (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                아니면 만약 (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                아니면 만약 (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                아니면 만약 (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                아니면 만약 (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    만약 (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } 아니면 {
                        scanner_warn(
                            s, XKB_WARNING_INVALID_UNICODE_ESCAPE_SEQUENCE,
                            "invalid Unicode escape sequence \"%.*s\" "
                            "in string literal",
                            (정수) (s->pos - start_pos + 1),
                            &s->s[start_pos - 1]
                        );
                        /* Ignore. */
                    }
                }
                아니면 만약 (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (char) o);
                }
                아니면 만약 (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (정수) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                아니면 {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } 아니면 {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        만약 (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            반환 ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        만약 (!yylval->str)
            반환 ERROR_TOK;
        반환 STRING;
    }

    /* Key name literal. */
    만약 (scanner_chr(s, '<')) {
        동안 (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        만약 (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            반환 ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const char *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        반환 KEYNAME;
    }

    /* Operators and punctuation. */
    만약 (scanner_chr(s, ';')) 반환 SEMI;
    만약 (scanner_chr(s, '{')) 반환 OBRACE;
    만약 (scanner_chr(s, '}')) 반환 CBRACE;
    만약 (scanner_chr(s, '=')) 반환 EQUALS;
    만약 (scanner_chr(s, '[')) 반환 OBRACKET;
    만약 (scanner_chr(s, ']')) 반환 CBRACKET;
    만약 (scanner_chr(s, '(')) 반환 OPAREN;
    만약 (scanner_chr(s, ')')) 반환 CPAREN;
    만약 (scanner_chr(s, '.')) 반환 DOT;
    만약 (scanner_chr(s, ',')) 반환 COMMA;
    만약 (scanner_chr(s, '+')) 반환 PLUS;
    만약 (scanner_chr(s, '-')) 반환 MINUS;
    만약 (scanner_chr(s, '*')) 반환 TIMES;
    만약 (scanner_chr(s, '/')) 반환 DIVIDE;
    만약 (scanner_chr(s, '!')) 반환 EXCLAM;
    만약 (scanner_chr(s, '~')) 반환 INVERT;

    정수 tok = ERROR_TOK;

    /* Identifier. */
    만약 (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        동안 (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const char *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        만약 (tok >= 0) 반환 tok;

        yylval->sval = SVAL(start, len);
        반환 IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    만약 (number(s, &yylval->num, &tok)) {
        만약 (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            반환 ERROR_TOK;
        }
        반환 tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    반환 ERROR_TOK;
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
    만약 (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        반환 false;
    }

    반환 true;
}

XkbFile *
XkbParseString(struct xkb_context *ctx,
               const struct parser_keymap_config *config,
               const char *string, size_t len,
               const char *file_name, const char *map)
{
    struct scanner scanner;
    만약 (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        반환 NULL;

    반환 parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(struct xkb_context *ctx,
                   const struct parser_keymap_config *config,
                   struct scanner *scanner, const char *map, XkbFile **out)
{
    만약 (map) {
        *out = parse(ctx, config, scanner, map);
        반환 !!(*out);
    } 아니면 {
        반환 parse_next(ctx, config, scanner, out);
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
    만약 (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        반환 NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    반환 xkb_file;
}
