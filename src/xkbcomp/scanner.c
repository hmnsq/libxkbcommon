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
number(sa_nracanaa scanner *s, int64_t *out, ကိန်းပြည့် *out_tok)
{
    အကယ်၍ (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ပြန်ပေး true;
        pak_sa 0:
            ပြန်ပေး false;
        default:
            *out_tok = INTEGER;
            ပြန်ပေး true;
        }
    }
    မဟုတ်ပါက {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ပြန်ပေး true;
        pak_sa 0:
            ပြန်ပေး false;
        pak_sa 1:
            is_digit = true;
            bha_nga;
        default:
            ;
        }
        အကယ်၍ (scanner_chr(s, DECIMAL_SEPARATOR)) {
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
            အကယ်၍ (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                ပြန်ပေး true;
            }
            *out_tok = FLOAT;
        } မဟုတ်ပါက အကယ်၍ (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } မဟုတ်ပါက {
            *out_tok = INTEGER;
        }
        ပြန်ပေး true;
    }
}

ကိန်းပြည့်
_xkbcommon_lex(YYSTYPE *yylval, sa_nracanaa scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    နေစဉ် (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    အကယ်၍ (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    အကယ်၍ (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    အကယ်၍ (scanner_eof(s)) ပြန်ပေး END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    အကယ်၍ (scanner_chr(s, '\"')) {
        နေစဉ် (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            အကယ်၍ (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                အကယ်၍      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                မဟုတ်ပါက အကယ်၍ (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                မဟုတ်ပါက အကယ်၍ (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                မဟုတ်ပါက အကယ်၍ (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                မဟုတ်ပါက အကယ်၍ (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                မဟုတ်ပါက အကယ်၍ (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                မဟုတ်ပါက အကယ်၍ (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                မဟုတ်ပါက အကယ်၍ (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                မဟုတ်ပါက အကယ်၍ (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                မဟုတ်ပါက အကယ်၍ (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    အကယ်၍ (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } မဟုတ်ပါက {
                        scanner_warn(
                            s, XKB_WARNING_INVALID_UNICODE_ESCAPE_SEQUENCE,
                            "invalid Unicode escape sequence \"%.*s\" "
                            "in string literal",
                            (ကိန်းပြည့်) (s->pos - start_pos + 1),
                            &s->s[start_pos - 1]
                        );
                        /* Ignore. */
                    }
                }
                မဟုတ်ပါက အကယ်၍ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (var_na) o);
                }
                မဟုတ်ပါက အကယ်၍ (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (ကိန်းပြည့်) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                မဟုတ်ပါက {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } မဟုတ်ပါက {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        အကယ်၍ (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            ပြန်ပေး ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        အကယ်၍ (!yylval->str)
            ပြန်ပေး ERROR_TOK;
        ပြန်ပေး STRING;
    }

    /* Key name literal. */
    အကယ်၍ (scanner_chr(s, '<')) {
        နေစဉ် (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        အကယ်၍ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            ပြန်ပေး ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const var_na *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        ပြန်ပေး KEYNAME;
    }

    /* Operators and punctuation. */
    အကယ်၍ (scanner_chr(s, ';')) ပြန်ပေး SEMI;
    အကယ်၍ (scanner_chr(s, '{')) ပြန်ပေး OBRACE;
    အကယ်၍ (scanner_chr(s, '}')) ပြန်ပေး CBRACE;
    အကယ်၍ (scanner_chr(s, '=')) ပြန်ပေး EQUALS;
    အကယ်၍ (scanner_chr(s, '[')) ပြန်ပေး OBRACKET;
    အကယ်၍ (scanner_chr(s, ']')) ပြန်ပေး CBRACKET;
    အကယ်၍ (scanner_chr(s, '(')) ပြန်ပေး OPAREN;
    အကယ်၍ (scanner_chr(s, ')')) ပြန်ပေး CPAREN;
    အကယ်၍ (scanner_chr(s, '.')) ပြန်ပေး DOT;
    အကယ်၍ (scanner_chr(s, ',')) ပြန်ပေး COMMA;
    အကယ်၍ (scanner_chr(s, '+')) ပြန်ပေး PLUS;
    အကယ်၍ (scanner_chr(s, '-')) ပြန်ပေး MINUS;
    အကယ်၍ (scanner_chr(s, '*')) ပြန်ပေး TIMES;
    အကယ်၍ (scanner_chr(s, '/')) ပြန်ပေး DIVIDE;
    အကယ်၍ (scanner_chr(s, '!')) ပြန်ပေး EXCLAM;
    အကယ်၍ (scanner_chr(s, '~')) ပြန်ပေး INVERT;

    ကိန်းပြည့် tok = ERROR_TOK;

    /* Identifier. */
    အကယ်၍ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        နေစဉ် (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const var_na *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        အကယ်၍ (tok >= 0) ပြန်ပေး tok;

        yylval->sval = SVAL(start, len);
        ပြန်ပေး IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    အကယ်၍ (number(s, &yylval->num, &tok)) {
        အကယ်၍ (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            ပြန်ပေး ERROR_TOK;
        }
        ပြန်ပေး tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    ပြန်ပေး ERROR_TOK;
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
    အကယ်၍ (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        ပြန်ပေး false;
    }

    ပြန်ပေး true;
}

XkbFile *
XkbParseString(sa_nracanaa xkb_context *ctx,
               const sa_nracanaa parser_keymap_config *config,
               const var_na *string, size_t len,
               const var_na *file_name, const var_na *map)
{
    sa_nracanaa scanner scanner;
    အကယ်၍ (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        ပြန်ပေး NULL;

    ပြန်ပေး parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(sa_nracanaa xkb_context *ctx,
                   const sa_nracanaa parser_keymap_config *config,
                   sa_nracanaa scanner *scanner, const var_na *map, XkbFile **out)
{
    အကယ်၍ (map) {
        *out = parse(ctx, config, scanner, map);
        ပြန်ပေး !!(*out);
    } မဟုတ်ပါက {
        ပြန်ပေး parse_next(ctx, config, scanner, out);
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
    အကယ်၍ (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        ပြန်ပေး NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    ပြန်ပေး xkb_file;
}
