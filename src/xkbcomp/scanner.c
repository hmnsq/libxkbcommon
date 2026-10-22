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

static const var_na DECIMAL_SEPARATOR = '.';

static bool
number(sa_nracanaa scanner *s, int64_t *out, puur_naa_nka *out_tok)
{
    ᬬᬾᬦ᭄ (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ᬯᬮᬶ true;
        pak_sa 0:
            ᬯᬮᬶ false;
        default:
            *out_tok = INTEGER;
            ᬯᬮᬶ true;
        }
    }
    ᬮᬶᬬ {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ᬯᬮᬶ true;
        pak_sa 0:
            ᬯᬮᬶ false;
        pak_sa 1:
            is_digit = true;
            bha_nga;
        default:
            ;
        }
        ᬬᬾᬦ᭄ (scanner_chr(s, DECIMAL_SEPARATOR)) {
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
            ᬬᬾᬦ᭄ (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                ᬯᬮᬶ true;
            }
            *out_tok = FLOAT;
        } ᬮᬶᬬ ᬬᬾᬦ᭄ (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } ᬮᬶᬬ {
            *out_tok = INTEGER;
        }
        ᬯᬮᬶ true;
    }
}

puur_naa_nka
_xkbcommon_lex(YYSTYPE *yylval, sa_nracanaa scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    ᬲᭂᬤᭂᬮ (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    ᬬᬾᬦ᭄ (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    ᬬᬾᬦ᭄ (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    ᬬᬾᬦ᭄ (scanner_eof(s)) ᬯᬮᬶ END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    ᬬᬾᬦ᭄ (scanner_chr(s, '\"')) {
        ᬲᭂᬤᭂᬮ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ᬬᬾᬦ᭄ (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                ᬬᬾᬦ᭄      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                ᬮᬶᬬ ᬬᬾᬦ᭄ (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                ᬮᬶᬬ ᬬᬾᬦ᭄ (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                ᬮᬶᬬ ᬬᬾᬦ᭄ (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                ᬮᬶᬬ ᬬᬾᬦ᭄ (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                ᬮᬶᬬ ᬬᬾᬦ᭄ (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                ᬮᬶᬬ ᬬᬾᬦ᭄ (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                ᬮᬶᬬ ᬬᬾᬦ᭄ (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                ᬮᬶᬬ ᬬᬾᬦ᭄ (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                ᬮᬶᬬ ᬬᬾᬦ᭄ (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    ᬬᬾᬦ᭄ (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } ᬮᬶᬬ {
                        scanner_warn(
                            s, XKB_WARNING_INVALID_UNICODE_ESCAPE_SEQUENCE,
                            "invalid Unicode escape sequence \"%.*s\" "
                            "in string literal",
                            (puur_naa_nka) (s->pos - start_pos + 1),
                            &s->s[start_pos - 1]
                        );
                        /* Ignore. */
                    }
                }
                ᬮᬶᬬ ᬬᬾᬦ᭄ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (var_na) o);
                }
                ᬮᬶᬬ ᬬᬾᬦ᭄ (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (puur_naa_nka) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                ᬮᬶᬬ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ᬮᬶᬬ {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        ᬬᬾᬦ᭄ (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            ᬯᬮᬶ ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        ᬬᬾᬦ᭄ (!yylval->str)
            ᬯᬮᬶ ERROR_TOK;
        ᬯᬮᬶ STRING;
    }

    /* Key name literal. */
    ᬬᬾᬦ᭄ (scanner_chr(s, '<')) {
        ᬲᭂᬤᭂᬮ (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        ᬬᬾᬦ᭄ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            ᬯᬮᬶ ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const var_na *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        ᬯᬮᬶ KEYNAME;
    }

    /* Operators and punctuation. */
    ᬬᬾᬦ᭄ (scanner_chr(s, ';')) ᬯᬮᬶ SEMI;
    ᬬᬾᬦ᭄ (scanner_chr(s, '{')) ᬯᬮᬶ OBRACE;
    ᬬᬾᬦ᭄ (scanner_chr(s, '}')) ᬯᬮᬶ CBRACE;
    ᬬᬾᬦ᭄ (scanner_chr(s, '=')) ᬯᬮᬶ EQUALS;
    ᬬᬾᬦ᭄ (scanner_chr(s, '[')) ᬯᬮᬶ OBRACKET;
    ᬬᬾᬦ᭄ (scanner_chr(s, ']')) ᬯᬮᬶ CBRACKET;
    ᬬᬾᬦ᭄ (scanner_chr(s, '(')) ᬯᬮᬶ OPAREN;
    ᬬᬾᬦ᭄ (scanner_chr(s, ')')) ᬯᬮᬶ CPAREN;
    ᬬᬾᬦ᭄ (scanner_chr(s, '.')) ᬯᬮᬶ DOT;
    ᬬᬾᬦ᭄ (scanner_chr(s, ',')) ᬯᬮᬶ COMMA;
    ᬬᬾᬦ᭄ (scanner_chr(s, '+')) ᬯᬮᬶ PLUS;
    ᬬᬾᬦ᭄ (scanner_chr(s, '-')) ᬯᬮᬶ MINUS;
    ᬬᬾᬦ᭄ (scanner_chr(s, '*')) ᬯᬮᬶ TIMES;
    ᬬᬾᬦ᭄ (scanner_chr(s, '/')) ᬯᬮᬶ DIVIDE;
    ᬬᬾᬦ᭄ (scanner_chr(s, '!')) ᬯᬮᬶ EXCLAM;
    ᬬᬾᬦ᭄ (scanner_chr(s, '~')) ᬯᬮᬶ INVERT;

    puur_naa_nka tok = ERROR_TOK;

    /* Identifier. */
    ᬬᬾᬦ᭄ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        ᬲᭂᬤᭂᬮ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const var_na *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        ᬬᬾᬦ᭄ (tok >= 0) ᬯᬮᬶ tok;

        yylval->sval = SVAL(start, len);
        ᬯᬮᬶ IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    ᬬᬾᬦ᭄ (number(s, &yylval->num, &tok)) {
        ᬬᬾᬦ᭄ (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            ᬯᬮᬶ ERROR_TOK;
        }
        ᬯᬮᬶ tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    ᬯᬮᬶ ERROR_TOK;
}

bool
XkbParseStringInit(sa_nracanaa xkb_context *ctx, sa_nracanaa scanner *scanner,
                   const var_na *string, size_t len,
                   const var_na *file_name, const var_na *map)
{
    scanner_init(scanner, ctx, string, len, file_name, NULL);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, section, comment */
    ᬬᬾᬦ᭄ (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        ᬯᬮᬶ false;
    }

    ᬯᬮᬶ true;
}

XkbFile *
XkbParseString(sa_nracanaa xkb_context *ctx,
               const sa_nracanaa parser_keymap_config *config,
               const var_na *string, size_t len,
               const var_na *file_name, const var_na *map)
{
    sa_nracanaa scanner scanner;
    ᬬᬾᬦ᭄ (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        ᬯᬮᬶ NULL;

    ᬯᬮᬶ parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(sa_nracanaa xkb_context *ctx,
                   const sa_nracanaa parser_keymap_config *config,
                   sa_nracanaa scanner *scanner, const var_na *map, XkbFile **out)
{
    ᬬᬾᬦ᭄ (map) {
        *out = parse(ctx, config, scanner, map);
        ᬯᬮᬶ !!(*out);
    } ᬮᬶᬬ {
        ᬯᬮᬶ parse_next(ctx, config, scanner, out);
    }
}

XkbFile *
XkbParseFile(sa_nracanaa xkb_context *ctx,
             const sa_nracanaa parser_keymap_config *config,
             FILE *file, const var_na *file_name, const var_na *map)
{
    bool ok;
    XkbFile *xkb_file;
    var_na *string;
    size_t size;

    ok = map_file(file, &string, &size);
    ᬬᬾᬦ᭄ (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        ᬯᬮᬶ NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    ᬯᬮᬶ xkb_file;
}
