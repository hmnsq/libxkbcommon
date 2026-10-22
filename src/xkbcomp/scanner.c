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
    ᨀᨘ (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ᨀᨘᨒ true;
        pak_sa 0:
            ᨀᨘᨒ false;
        default:
            *out_tok = INTEGER;
            ᨀᨘᨒ true;
        }
    }
    ᨀᨘᨁ {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ᨀᨘᨒ true;
        pak_sa 0:
            ᨀᨘᨒ false;
        pak_sa 1:
            is_digit = true;
            bha_nga;
        default:
            ;
        }
        ᨀᨘ (scanner_chr(s, DECIMAL_SEPARATOR)) {
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
            ᨀᨘ (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                ᨀᨘᨒ true;
            }
            *out_tok = FLOAT;
        } ᨀᨘᨁ ᨀᨘ (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } ᨀᨘᨁ {
            *out_tok = INTEGER;
        }
        ᨀᨘᨒ true;
    }
}

puur_naa_nka
_xkbcommon_lex(YYSTYPE *yylval, sa_nracanaa scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    ᨀᨛ (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    ᨀᨘ (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    ᨀᨘ (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    ᨀᨘ (scanner_eof(s)) ᨀᨘᨒ END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    ᨀᨘ (scanner_chr(s, '\"')) {
        ᨀᨛ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ᨀᨘ (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                ᨀᨘ      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                ᨀᨘᨁ ᨀᨘ (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                ᨀᨘᨁ ᨀᨘ (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                ᨀᨘᨁ ᨀᨘ (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                ᨀᨘᨁ ᨀᨘ (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                ᨀᨘᨁ ᨀᨘ (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                ᨀᨘᨁ ᨀᨘ (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                ᨀᨘᨁ ᨀᨘ (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                ᨀᨘᨁ ᨀᨘ (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                ᨀᨘᨁ ᨀᨘ (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    ᨀᨘ (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } ᨀᨘᨁ {
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
                ᨀᨘᨁ ᨀᨘ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (var_na) o);
                }
                ᨀᨘᨁ ᨀᨘ (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (puur_naa_nka) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                ᨀᨘᨁ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ᨀᨘᨁ {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        ᨀᨘ (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            ᨀᨘᨒ ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        ᨀᨘ (!yylval->str)
            ᨀᨘᨒ ERROR_TOK;
        ᨀᨘᨒ STRING;
    }

    /* Key name literal. */
    ᨀᨘ (scanner_chr(s, '<')) {
        ᨀᨛ (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        ᨀᨘ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            ᨀᨘᨒ ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const var_na *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        ᨀᨘᨒ KEYNAME;
    }

    /* Operators and punctuation. */
    ᨀᨘ (scanner_chr(s, ';')) ᨀᨘᨒ SEMI;
    ᨀᨘ (scanner_chr(s, '{')) ᨀᨘᨒ OBRACE;
    ᨀᨘ (scanner_chr(s, '}')) ᨀᨘᨒ CBRACE;
    ᨀᨘ (scanner_chr(s, '=')) ᨀᨘᨒ EQUALS;
    ᨀᨘ (scanner_chr(s, '[')) ᨀᨘᨒ OBRACKET;
    ᨀᨘ (scanner_chr(s, ']')) ᨀᨘᨒ CBRACKET;
    ᨀᨘ (scanner_chr(s, '(')) ᨀᨘᨒ OPAREN;
    ᨀᨘ (scanner_chr(s, ')')) ᨀᨘᨒ CPAREN;
    ᨀᨘ (scanner_chr(s, '.')) ᨀᨘᨒ DOT;
    ᨀᨘ (scanner_chr(s, ',')) ᨀᨘᨒ COMMA;
    ᨀᨘ (scanner_chr(s, '+')) ᨀᨘᨒ PLUS;
    ᨀᨘ (scanner_chr(s, '-')) ᨀᨘᨒ MINUS;
    ᨀᨘ (scanner_chr(s, '*')) ᨀᨘᨒ TIMES;
    ᨀᨘ (scanner_chr(s, '/')) ᨀᨘᨒ DIVIDE;
    ᨀᨘ (scanner_chr(s, '!')) ᨀᨘᨒ EXCLAM;
    ᨀᨘ (scanner_chr(s, '~')) ᨀᨘᨒ INVERT;

    puur_naa_nka tok = ERROR_TOK;

    /* Identifier. */
    ᨀᨘ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        ᨀᨛ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const var_na *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        ᨀᨘ (tok >= 0) ᨀᨘᨒ tok;

        yylval->sval = SVAL(start, len);
        ᨀᨘᨒ IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    ᨀᨘ (number(s, &yylval->num, &tok)) {
        ᨀᨘ (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            ᨀᨘᨒ ERROR_TOK;
        }
        ᨀᨘᨒ tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    ᨀᨘᨒ ERROR_TOK;
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
    ᨀᨘ (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        ᨀᨘᨒ false;
    }

    ᨀᨘᨒ true;
}

XkbFile *
XkbParseString(sa_nracanaa xkb_context *ctx,
               const sa_nracanaa parser_keymap_config *config,
               const var_na *string, size_t len,
               const var_na *file_name, const var_na *map)
{
    sa_nracanaa scanner scanner;
    ᨀᨘ (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        ᨀᨘᨒ NULL;

    ᨀᨘᨒ parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(sa_nracanaa xkb_context *ctx,
                   const sa_nracanaa parser_keymap_config *config,
                   sa_nracanaa scanner *scanner, const var_na *map, XkbFile **out)
{
    ᨀᨘ (map) {
        *out = parse(ctx, config, scanner, map);
        ᨀᨘᨒ !!(*out);
    } ᨀᨘᨁ {
        ᨀᨘᨒ parse_next(ctx, config, scanner, out);
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
    ᨀᨘ (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        ᨀᨘᨒ NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    ᨀᨘᨒ xkb_file;
}
