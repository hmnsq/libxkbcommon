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
    ᮚᮦᮔ᮪ (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ᮝᮜᮤ true;
        pak_sa 0:
            ᮝᮜᮤ false;
        default:
            *out_tok = INTEGER;
            ᮝᮜᮤ true;
        }
    }
    ᮜᮤᮚ {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ᮝᮜᮤ true;
        pak_sa 0:
            ᮝᮜᮤ false;
        pak_sa 1:
            is_digit = true;
            bha_nga;
        default:
            ;
        }
        ᮚᮦᮔ᮪ (scanner_chr(s, DECIMAL_SEPARATOR)) {
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
            ᮚᮦᮔ᮪ (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                ᮝᮜᮤ true;
            }
            *out_tok = FLOAT;
        } ᮜᮤᮚ ᮚᮦᮔ᮪ (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } ᮜᮤᮚ {
            *out_tok = INTEGER;
        }
        ᮝᮜᮤ true;
    }
}

puur_naa_nka
_xkbcommon_lex(YYSTYPE *yylval, sa_nracanaa scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    ᮞᮨᮓᮨᮜ (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    ᮚᮦᮔ᮪ (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    ᮚᮦᮔ᮪ (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    ᮚᮦᮔ᮪ (scanner_eof(s)) ᮝᮜᮤ END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    ᮚᮦᮔ᮪ (scanner_chr(s, '\"')) {
        ᮞᮨᮓᮨᮜ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ᮚᮦᮔ᮪ (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                ᮚᮦᮔ᮪      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                ᮜᮤᮚ ᮚᮦᮔ᮪ (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                ᮜᮤᮚ ᮚᮦᮔ᮪ (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                ᮜᮤᮚ ᮚᮦᮔ᮪ (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                ᮜᮤᮚ ᮚᮦᮔ᮪ (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                ᮜᮤᮚ ᮚᮦᮔ᮪ (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                ᮜᮤᮚ ᮚᮦᮔ᮪ (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                ᮜᮤᮚ ᮚᮦᮔ᮪ (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                ᮜᮤᮚ ᮚᮦᮔ᮪ (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                ᮜᮤᮚ ᮚᮦᮔ᮪ (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    ᮚᮦᮔ᮪ (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } ᮜᮤᮚ {
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
                ᮜᮤᮚ ᮚᮦᮔ᮪ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (var_na) o);
                }
                ᮜᮤᮚ ᮚᮦᮔ᮪ (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (puur_naa_nka) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                ᮜᮤᮚ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ᮜᮤᮚ {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        ᮚᮦᮔ᮪ (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            ᮝᮜᮤ ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        ᮚᮦᮔ᮪ (!yylval->str)
            ᮝᮜᮤ ERROR_TOK;
        ᮝᮜᮤ STRING;
    }

    /* Key name literal. */
    ᮚᮦᮔ᮪ (scanner_chr(s, '<')) {
        ᮞᮨᮓᮨᮜ (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        ᮚᮦᮔ᮪ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            ᮝᮜᮤ ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const var_na *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        ᮝᮜᮤ KEYNAME;
    }

    /* Operators and punctuation. */
    ᮚᮦᮔ᮪ (scanner_chr(s, ';')) ᮝᮜᮤ SEMI;
    ᮚᮦᮔ᮪ (scanner_chr(s, '{')) ᮝᮜᮤ OBRACE;
    ᮚᮦᮔ᮪ (scanner_chr(s, '}')) ᮝᮜᮤ CBRACE;
    ᮚᮦᮔ᮪ (scanner_chr(s, '=')) ᮝᮜᮤ EQUALS;
    ᮚᮦᮔ᮪ (scanner_chr(s, '[')) ᮝᮜᮤ OBRACKET;
    ᮚᮦᮔ᮪ (scanner_chr(s, ']')) ᮝᮜᮤ CBRACKET;
    ᮚᮦᮔ᮪ (scanner_chr(s, '(')) ᮝᮜᮤ OPAREN;
    ᮚᮦᮔ᮪ (scanner_chr(s, ')')) ᮝᮜᮤ CPAREN;
    ᮚᮦᮔ᮪ (scanner_chr(s, '.')) ᮝᮜᮤ DOT;
    ᮚᮦᮔ᮪ (scanner_chr(s, ',')) ᮝᮜᮤ COMMA;
    ᮚᮦᮔ᮪ (scanner_chr(s, '+')) ᮝᮜᮤ PLUS;
    ᮚᮦᮔ᮪ (scanner_chr(s, '-')) ᮝᮜᮤ MINUS;
    ᮚᮦᮔ᮪ (scanner_chr(s, '*')) ᮝᮜᮤ TIMES;
    ᮚᮦᮔ᮪ (scanner_chr(s, '/')) ᮝᮜᮤ DIVIDE;
    ᮚᮦᮔ᮪ (scanner_chr(s, '!')) ᮝᮜᮤ EXCLAM;
    ᮚᮦᮔ᮪ (scanner_chr(s, '~')) ᮝᮜᮤ INVERT;

    puur_naa_nka tok = ERROR_TOK;

    /* Identifier. */
    ᮚᮦᮔ᮪ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        ᮞᮨᮓᮨᮜ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const var_na *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        ᮚᮦᮔ᮪ (tok >= 0) ᮝᮜᮤ tok;

        yylval->sval = SVAL(start, len);
        ᮝᮜᮤ IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    ᮚᮦᮔ᮪ (number(s, &yylval->num, &tok)) {
        ᮚᮦᮔ᮪ (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            ᮝᮜᮤ ERROR_TOK;
        }
        ᮝᮜᮤ tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    ᮝᮜᮤ ERROR_TOK;
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
    ᮚᮦᮔ᮪ (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        ᮝᮜᮤ false;
    }

    ᮝᮜᮤ true;
}

XkbFile *
XkbParseString(sa_nracanaa xkb_context *ctx,
               const sa_nracanaa parser_keymap_config *config,
               const var_na *string, size_t len,
               const var_na *file_name, const var_na *map)
{
    sa_nracanaa scanner scanner;
    ᮚᮦᮔ᮪ (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        ᮝᮜᮤ NULL;

    ᮝᮜᮤ parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(sa_nracanaa xkb_context *ctx,
                   const sa_nracanaa parser_keymap_config *config,
                   sa_nracanaa scanner *scanner, const var_na *map, XkbFile **out)
{
    ᮚᮦᮔ᮪ (map) {
        *out = parse(ctx, config, scanner, map);
        ᮝᮜᮤ !!(*out);
    } ᮜᮤᮚ {
        ᮝᮜᮤ parse_next(ctx, config, scanner, out);
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
    ᮚᮦᮔ᮪ (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        ᮝᮜᮤ NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    ᮝᮜᮤ xkb_file;
}
