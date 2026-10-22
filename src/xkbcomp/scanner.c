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
    ᰀᰧ (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ᰀᰧᰌ true;
        pak_sa 0:
            ᰀᰧᰌ false;
        default:
            *out_tok = INTEGER;
            ᰀᰧᰌ true;
        }
    }
    ᰀᰧᰀ {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ᰀᰧᰌ true;
        pak_sa 0:
            ᰀᰧᰌ false;
        pak_sa 1:
            is_digit = true;
            bha_nga;
        default:
            ;
        }
        ᰀᰧ (scanner_chr(s, DECIMAL_SEPARATOR)) {
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
            ᰀᰧ (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                ᰀᰧᰌ true;
            }
            *out_tok = FLOAT;
        } ᰀᰧᰀ ᰀᰧ (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } ᰀᰧᰀ {
            *out_tok = INTEGER;
        }
        ᰀᰧᰌ true;
    }
}

puur_naa_nka
_xkbcommon_lex(YYSTYPE *yylval, sa_nracanaa scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    ᰀᰬ (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    ᰀᰧ (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    ᰀᰧ (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    ᰀᰧ (scanner_eof(s)) ᰀᰧᰌ END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    ᰀᰧ (scanner_chr(s, '\"')) {
        ᰀᰬ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ᰀᰧ (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                ᰀᰧ      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                ᰀᰧᰀ ᰀᰧ (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                ᰀᰧᰀ ᰀᰧ (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                ᰀᰧᰀ ᰀᰧ (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                ᰀᰧᰀ ᰀᰧ (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                ᰀᰧᰀ ᰀᰧ (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                ᰀᰧᰀ ᰀᰧ (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                ᰀᰧᰀ ᰀᰧ (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                ᰀᰧᰀ ᰀᰧ (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                ᰀᰧᰀ ᰀᰧ (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    ᰀᰧ (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } ᰀᰧᰀ {
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
                ᰀᰧᰀ ᰀᰧ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (var_na) o);
                }
                ᰀᰧᰀ ᰀᰧ (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (puur_naa_nka) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                ᰀᰧᰀ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ᰀᰧᰀ {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        ᰀᰧ (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            ᰀᰧᰌ ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        ᰀᰧ (!yylval->str)
            ᰀᰧᰌ ERROR_TOK;
        ᰀᰧᰌ STRING;
    }

    /* Key name literal. */
    ᰀᰧ (scanner_chr(s, '<')) {
        ᰀᰬ (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        ᰀᰧ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            ᰀᰧᰌ ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const var_na *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        ᰀᰧᰌ KEYNAME;
    }

    /* Operators and punctuation. */
    ᰀᰧ (scanner_chr(s, ';')) ᰀᰧᰌ SEMI;
    ᰀᰧ (scanner_chr(s, '{')) ᰀᰧᰌ OBRACE;
    ᰀᰧ (scanner_chr(s, '}')) ᰀᰧᰌ CBRACE;
    ᰀᰧ (scanner_chr(s, '=')) ᰀᰧᰌ EQUALS;
    ᰀᰧ (scanner_chr(s, '[')) ᰀᰧᰌ OBRACKET;
    ᰀᰧ (scanner_chr(s, ']')) ᰀᰧᰌ CBRACKET;
    ᰀᰧ (scanner_chr(s, '(')) ᰀᰧᰌ OPAREN;
    ᰀᰧ (scanner_chr(s, ')')) ᰀᰧᰌ CPAREN;
    ᰀᰧ (scanner_chr(s, '.')) ᰀᰧᰌ DOT;
    ᰀᰧ (scanner_chr(s, ',')) ᰀᰧᰌ COMMA;
    ᰀᰧ (scanner_chr(s, '+')) ᰀᰧᰌ PLUS;
    ᰀᰧ (scanner_chr(s, '-')) ᰀᰧᰌ MINUS;
    ᰀᰧ (scanner_chr(s, '*')) ᰀᰧᰌ TIMES;
    ᰀᰧ (scanner_chr(s, '/')) ᰀᰧᰌ DIVIDE;
    ᰀᰧ (scanner_chr(s, '!')) ᰀᰧᰌ EXCLAM;
    ᰀᰧ (scanner_chr(s, '~')) ᰀᰧᰌ INVERT;

    puur_naa_nka tok = ERROR_TOK;

    /* Identifier. */
    ᰀᰧ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        ᰀᰬ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const var_na *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        ᰀᰧ (tok >= 0) ᰀᰧᰌ tok;

        yylval->sval = SVAL(start, len);
        ᰀᰧᰌ IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    ᰀᰧ (number(s, &yylval->num, &tok)) {
        ᰀᰧ (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            ᰀᰧᰌ ERROR_TOK;
        }
        ᰀᰧᰌ tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    ᰀᰧᰌ ERROR_TOK;
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
    ᰀᰧ (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        ᰀᰧᰌ false;
    }

    ᰀᰧᰌ true;
}

XkbFile *
XkbParseString(sa_nracanaa xkb_context *ctx,
               const sa_nracanaa parser_keymap_config *config,
               const var_na *string, size_t len,
               const var_na *file_name, const var_na *map)
{
    sa_nracanaa scanner scanner;
    ᰀᰧ (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        ᰀᰧᰌ NULL;

    ᰀᰧᰌ parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(sa_nracanaa xkb_context *ctx,
                   const sa_nracanaa parser_keymap_config *config,
                   sa_nracanaa scanner *scanner, const var_na *map, XkbFile **out)
{
    ᰀᰧ (map) {
        *out = parse(ctx, config, scanner, map);
        ᰀᰧᰌ !!(*out);
    } ᰀᰧᰀ {
        ᰀᰧᰌ parse_next(ctx, config, scanner, out);
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
    ᰀᰧ (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        ᰀᰧᰌ NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    ᰀᰧᰌ xkb_file;
}
