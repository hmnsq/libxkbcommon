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
number(struct scanner *s, int64_t *out, ціле *out_tok)
{
    якщо (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        case -1:
            *out_tok = ERROR_TOK;
            повернути true;
        case 0:
            повернути false;
        default:
            *out_tok = INTEGER;
            повернути true;
        }
    }
    інакше {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        case -1:
            *out_tok = ERROR_TOK;
            повернути true;
        case 0:
            повернути false;
        case 1:
            is_digit = true;
            break;
        default:
            ;
        }
        якщо (scanner_chr(s, DECIMAL_SEPARATOR)) {
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
            якщо (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                повернути true;
            }
            *out_tok = FLOAT;
        } інакше якщо (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } інакше {
            *out_tok = INTEGER;
        }
        повернути true;
    }
}

ціле
_xkbcommon_lex(YYSTYPE *yylval, struct scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    поки (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    якщо (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    якщо (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    якщо (scanner_eof(s)) повернути END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    якщо (scanner_chr(s, '\"')) {
        поки (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            якщо (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                якщо      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                інакше якщо (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                інакше якщо (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                інакше якщо (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                інакше якщо (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                інакше якщо (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                інакше якщо (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                інакше якщо (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                інакше якщо (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                інакше якщо (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    якщо (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } інакше {
                        scanner_warn(
                            s, XKB_WARNING_INVALID_UNICODE_ESCAPE_SEQUENCE,
                            "invalid Unicode escape sequence \"%.*s\" "
                            "in string literal",
                            (ціле) (s->pos - start_pos + 1),
                            &s->s[start_pos - 1]
                        );
                        /* Ignore. */
                    }
                }
                інакше якщо (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (char) o);
                }
                інакше якщо (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (ціле) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                інакше {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } інакше {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        якщо (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            повернути ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        якщо (!yylval->str)
            повернути ERROR_TOK;
        повернути STRING;
    }

    /* Key name literal. */
    якщо (scanner_chr(s, '<')) {
        поки (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        якщо (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            повернути ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const char *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        повернути KEYNAME;
    }

    /* Operators and punctuation. */
    якщо (scanner_chr(s, ';')) повернути SEMI;
    якщо (scanner_chr(s, '{')) повернути OBRACE;
    якщо (scanner_chr(s, '}')) повернути CBRACE;
    якщо (scanner_chr(s, '=')) повернути EQUALS;
    якщо (scanner_chr(s, '[')) повернути OBRACKET;
    якщо (scanner_chr(s, ']')) повернути CBRACKET;
    якщо (scanner_chr(s, '(')) повернути OPAREN;
    якщо (scanner_chr(s, ')')) повернути CPAREN;
    якщо (scanner_chr(s, '.')) повернути DOT;
    якщо (scanner_chr(s, ',')) повернути COMMA;
    якщо (scanner_chr(s, '+')) повернути PLUS;
    якщо (scanner_chr(s, '-')) повернути MINUS;
    якщо (scanner_chr(s, '*')) повернути TIMES;
    якщо (scanner_chr(s, '/')) повернути DIVIDE;
    якщо (scanner_chr(s, '!')) повернути EXCLAM;
    якщо (scanner_chr(s, '~')) повернути INVERT;

    ціле tok = ERROR_TOK;

    /* Identifier. */
    якщо (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        поки (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const char *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        якщо (tok >= 0) повернути tok;

        yylval->sval = SVAL(start, len);
        повернути IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    якщо (number(s, &yylval->num, &tok)) {
        якщо (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            повернути ERROR_TOK;
        }
        повернути tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    повернути ERROR_TOK;
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
    якщо (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        повернути false;
    }

    повернути true;
}

XkbFile *
XkbParseString(struct xkb_context *ctx,
               const struct parser_keymap_config *config,
               const char *string, size_t len,
               const char *file_name, const char *map)
{
    struct scanner scanner;
    якщо (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        повернути NULL;

    повернути parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(struct xkb_context *ctx,
                   const struct parser_keymap_config *config,
                   struct scanner *scanner, const char *map, XkbFile **out)
{
    якщо (map) {
        *out = parse(ctx, config, scanner, map);
        повернути !!(*out);
    } інакше {
        повернути parse_next(ctx, config, scanner, out);
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
    якщо (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        повернути NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    повернути xkb_file;
}
