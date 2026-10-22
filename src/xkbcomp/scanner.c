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
number(sa_nracanaa scanner *s, int64_t *out, ຈຳນວນເຕັມ *out_tok)
{
    ຖ້າ (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ສົ່ງຄືນ true;
        pak_sa 0:
            ສົ່ງຄືນ false;
        default:
            *out_tok = INTEGER;
            ສົ່ງຄືນ true;
        }
    }
    ບໍ່ດັ່ງນັ້ນ {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ສົ່ງຄືນ true;
        pak_sa 0:
            ສົ່ງຄືນ false;
        pak_sa 1:
            is_digit = true;
            bha_nga;
        default:
            ;
        }
        ຖ້າ (scanner_chr(s, DECIMAL_SEPARATOR)) {
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
            ຖ້າ (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                ສົ່ງຄືນ true;
            }
            *out_tok = FLOAT;
        } ບໍ່ດັ່ງນັ້ນ ຖ້າ (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } ບໍ່ດັ່ງນັ້ນ {
            *out_tok = INTEGER;
        }
        ສົ່ງຄືນ true;
    }
}

ຈຳນວນເຕັມ
_xkbcommon_lex(YYSTYPE *yylval, sa_nracanaa scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    ໃນຂະນະທີ່ (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    ຖ້າ (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    ຖ້າ (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    ຖ້າ (scanner_eof(s)) ສົ່ງຄືນ END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    ຖ້າ (scanner_chr(s, '\"')) {
        ໃນຂະນະທີ່ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ຖ້າ (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                ຖ້າ      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    ຖ້າ (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } ບໍ່ດັ່ງນັ້ນ {
                        scanner_warn(
                            s, XKB_WARNING_INVALID_UNICODE_ESCAPE_SEQUENCE,
                            "invalid Unicode escape sequence \"%.*s\" "
                            "in string literal",
                            (ຈຳນວນເຕັມ) (s->pos - start_pos + 1),
                            &s->s[start_pos - 1]
                        );
                        /* Ignore. */
                    }
                }
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (var_na) o);
                }
                ບໍ່ດັ່ງນັ້ນ ຖ້າ (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (ຈຳນວນເຕັມ) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                ບໍ່ດັ່ງນັ້ນ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ບໍ່ດັ່ງນັ້ນ {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        ຖ້າ (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            ສົ່ງຄືນ ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        ຖ້າ (!yylval->str)
            ສົ່ງຄືນ ERROR_TOK;
        ສົ່ງຄືນ STRING;
    }

    /* Key name literal. */
    ຖ້າ (scanner_chr(s, '<')) {
        ໃນຂະນະທີ່ (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        ຖ້າ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            ສົ່ງຄືນ ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const var_na *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        ສົ່ງຄືນ KEYNAME;
    }

    /* Operators and punctuation. */
    ຖ້າ (scanner_chr(s, ';')) ສົ່ງຄືນ SEMI;
    ຖ້າ (scanner_chr(s, '{')) ສົ່ງຄືນ OBRACE;
    ຖ້າ (scanner_chr(s, '}')) ສົ່ງຄືນ CBRACE;
    ຖ້າ (scanner_chr(s, '=')) ສົ່ງຄືນ EQUALS;
    ຖ້າ (scanner_chr(s, '[')) ສົ່ງຄືນ OBRACKET;
    ຖ້າ (scanner_chr(s, ']')) ສົ່ງຄືນ CBRACKET;
    ຖ້າ (scanner_chr(s, '(')) ສົ່ງຄືນ OPAREN;
    ຖ້າ (scanner_chr(s, ')')) ສົ່ງຄືນ CPAREN;
    ຖ້າ (scanner_chr(s, '.')) ສົ່ງຄືນ DOT;
    ຖ້າ (scanner_chr(s, ',')) ສົ່ງຄືນ COMMA;
    ຖ້າ (scanner_chr(s, '+')) ສົ່ງຄືນ PLUS;
    ຖ້າ (scanner_chr(s, '-')) ສົ່ງຄືນ MINUS;
    ຖ້າ (scanner_chr(s, '*')) ສົ່ງຄືນ TIMES;
    ຖ້າ (scanner_chr(s, '/')) ສົ່ງຄືນ DIVIDE;
    ຖ້າ (scanner_chr(s, '!')) ສົ່ງຄືນ EXCLAM;
    ຖ້າ (scanner_chr(s, '~')) ສົ່ງຄືນ INVERT;

    ຈຳນວນເຕັມ tok = ERROR_TOK;

    /* Identifier. */
    ຖ້າ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        ໃນຂະນະທີ່ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const var_na *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        ຖ້າ (tok >= 0) ສົ່ງຄືນ tok;

        yylval->sval = SVAL(start, len);
        ສົ່ງຄືນ IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    ຖ້າ (number(s, &yylval->num, &tok)) {
        ຖ້າ (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            ສົ່ງຄືນ ERROR_TOK;
        }
        ສົ່ງຄືນ tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    ສົ່ງຄືນ ERROR_TOK;
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
    ຖ້າ (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        ສົ່ງຄືນ false;
    }

    ສົ່ງຄືນ true;
}

XkbFile *
XkbParseString(sa_nracanaa xkb_context *ctx,
               const sa_nracanaa parser_keymap_config *config,
               const var_na *string, size_t len,
               const var_na *file_name, const var_na *map)
{
    sa_nracanaa scanner scanner;
    ຖ້າ (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        ສົ່ງຄືນ NULL;

    ສົ່ງຄືນ parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(sa_nracanaa xkb_context *ctx,
                   const sa_nracanaa parser_keymap_config *config,
                   sa_nracanaa scanner *scanner, const var_na *map, XkbFile **out)
{
    ຖ້າ (map) {
        *out = parse(ctx, config, scanner, map);
        ສົ່ງຄືນ !!(*out);
    } ບໍ່ດັ່ງນັ້ນ {
        ສົ່ງຄືນ parse_next(ctx, config, scanner, out);
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
    ຖ້າ (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        ສົ່ງຄືນ NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    ສົ່ງຄືນ xkb_file;
}
