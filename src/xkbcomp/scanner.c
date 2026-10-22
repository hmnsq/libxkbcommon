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
number(struct scanner *s, int64_t *out, 整数 *out_tok)
{
    如果 (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        case -1:
            *out_tok = ERROR_TOK;
            返回 true;
        case 0:
            返回 false;
        default:
            *out_tok = INTEGER;
            返回 true;
        }
    }
    否则 {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        case -1:
            *out_tok = ERROR_TOK;
            返回 true;
        case 0:
            返回 false;
        case 1:
            is_digit = true;
            break;
        default:
            ;
        }
        如果 (scanner_chr(s, DECIMAL_SEPARATOR)) {
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
            如果 (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                返回 true;
            }
            *out_tok = FLOAT;
        } 否则 如果 (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } 否则 {
            *out_tok = INTEGER;
        }
        返回 true;
    }
}

整数
_xkbcommon_lex(YYSTYPE *yylval, struct scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    当 (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    如果 (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    如果 (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    如果 (scanner_eof(s)) 返回 END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    如果 (scanner_chr(s, '\"')) {
        当 (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            如果 (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                如果      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                否则 如果 (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                否则 如果 (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                否则 如果 (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                否则 如果 (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                否则 如果 (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                否则 如果 (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                否则 如果 (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                否则 如果 (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                否则 如果 (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    如果 (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } 否则 {
                        scanner_warn(
                            s, XKB_WARNING_INVALID_UNICODE_ESCAPE_SEQUENCE,
                            "invalid Unicode escape sequence \"%.*s\" "
                            "in string literal",
                            (整数) (s->pos - start_pos + 1),
                            &s->s[start_pos - 1]
                        );
                        /* Ignore. */
                    }
                }
                否则 如果 (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (char) o);
                }
                否则 如果 (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (整数) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                否则 {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } 否则 {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        如果 (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            返回 ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        如果 (!yylval->str)
            返回 ERROR_TOK;
        返回 STRING;
    }

    /* Key name literal. */
    如果 (scanner_chr(s, '<')) {
        当 (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        如果 (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            返回 ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const char *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        返回 KEYNAME;
    }

    /* Operators and punctuation. */
    如果 (scanner_chr(s, ';')) 返回 SEMI;
    如果 (scanner_chr(s, '{')) 返回 OBRACE;
    如果 (scanner_chr(s, '}')) 返回 CBRACE;
    如果 (scanner_chr(s, '=')) 返回 EQUALS;
    如果 (scanner_chr(s, '[')) 返回 OBRACKET;
    如果 (scanner_chr(s, ']')) 返回 CBRACKET;
    如果 (scanner_chr(s, '(')) 返回 OPAREN;
    如果 (scanner_chr(s, ')')) 返回 CPAREN;
    如果 (scanner_chr(s, '.')) 返回 DOT;
    如果 (scanner_chr(s, ',')) 返回 COMMA;
    如果 (scanner_chr(s, '+')) 返回 PLUS;
    如果 (scanner_chr(s, '-')) 返回 MINUS;
    如果 (scanner_chr(s, '*')) 返回 TIMES;
    如果 (scanner_chr(s, '/')) 返回 DIVIDE;
    如果 (scanner_chr(s, '!')) 返回 EXCLAM;
    如果 (scanner_chr(s, '~')) 返回 INVERT;

    整数 tok = ERROR_TOK;

    /* Identifier. */
    如果 (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        当 (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const char *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        如果 (tok >= 0) 返回 tok;

        yylval->sval = SVAL(start, len);
        返回 IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    如果 (number(s, &yylval->num, &tok)) {
        如果 (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            返回 ERROR_TOK;
        }
        返回 tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    返回 ERROR_TOK;
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
    如果 (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        返回 false;
    }

    返回 true;
}

XkbFile *
XkbParseString(struct xkb_context *ctx,
               const struct parser_keymap_config *config,
               const char *string, size_t len,
               const char *file_name, const char *map)
{
    struct scanner scanner;
    如果 (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        返回 NULL;

    返回 parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(struct xkb_context *ctx,
                   const struct parser_keymap_config *config,
                   struct scanner *scanner, const char *map, XkbFile **out)
{
    如果 (map) {
        *out = parse(ctx, config, scanner, map);
        返回 !!(*out);
    } 否则 {
        返回 parse_next(ctx, config, scanner, out);
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
    如果 (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        返回 NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    返回 xkb_file;
}
