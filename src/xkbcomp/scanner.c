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
number(sa_nracanaa scanner *s, int64_t *out, පූර්ණ_සංඛ්‍යා *out_tok)
{
    නම් (scanner_lit(s, "0x")) {
        switch (scanner_hex_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ආපසු true;
        pak_sa 0:
            ආපසු false;
        default:
            *out_tok = INTEGER;
            ආපසු true;
        }
    }
    නැතහොත් {
        bool is_digit = false;
        switch (scanner_dec_int64(s, out)) {
        pak_sa -1:
            *out_tok = ERROR_TOK;
            ආපසු true;
        pak_sa 0:
            ආපසු false;
        pak_sa 1:
            is_digit = true;
            bha_nga;
        default:
            ;
        }
        නම් (scanner_chr(s, DECIMAL_SEPARATOR)) {
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
            නම් (scanner_dec_int64(s, &dec) < 0) {
                *out_tok = ERROR_TOK;
                ආපසු true;
            }
            *out_tok = FLOAT;
        } නැතහොත් නම් (is_digit) {
            *out_tok = DECIMAL_DIGIT;
        } නැතහොත් {
            *out_tok = INTEGER;
        }
        ආපසු true;
    }
}

පූර්ණ_සංඛ්‍යා
_xkbcommon_lex(YYSTYPE *yylval, sa_nracanaa scanner *s)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    අතරතුර (is_space(scanner_peek(s))) scanner_next(s);

    /*
     * Skip U+200E LEFT-TO-RIGHT MARK and U+200F RIGHT-TO-LEFT MARK, assuming
     * UTF-8 encoding. These Unicode code points are useful for forcing the text
     * directionality when displaying/editing an XKB file.
     */
    නම් (scanner_lit(s, u8"\u200E") || scanner_lit(s, u8"\u200F"))
        goto skip_more_whitespace_and_comments;

    /* Skip comments. */
    නම් (scanner_lit(s, "//") || scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    නම් (scanner_eof(s)) ආපසු END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* String literal. */
    නම් (scanner_chr(s, '\"')) {
        අතරතුර (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            නම් (scanner_chr(s, '\\')) {
                uint8_t o;
                const size_t start_pos = s->pos;
                නම්      (scanner_chr(s, '\\')){scanner_buf_append(s, '\\');}
                නැතහොත් නම් (scanner_chr(s, '"')) {scanner_buf_append(s, '"');}
                නැතහොත් නම් (scanner_chr(s, 'n')) {scanner_buf_append(s, '\n');}
                නැතහොත් නම් (scanner_chr(s, 't')) {scanner_buf_append(s, '\t');}
                නැතහොත් නම් (scanner_chr(s, 'r')) {scanner_buf_append(s, '\r');}
                නැතහොත් නම් (scanner_chr(s, 'b')) {scanner_buf_append(s, '\b');}
                නැතහොත් නම් (scanner_chr(s, 'f')) {scanner_buf_append(s, '\f');}
                නැතහොත් නම් (scanner_chr(s, 'v')) {scanner_buf_append(s, '\v');}
                නැතහොත් නම් (scanner_chr(s, 'e')) {scanner_buf_append(s, '\x1b');}
                නැතහොත් නම් (scanner_chr(s, 'u')) {
                    /* Unicode escape sequence */
                    uint32_t cp = 0;
                    නම් (scanner_unicode_code_point(s, &cp) &&
                        is_valid_char(cp)) {
                        /* Encode code point using UTF-8 */
                        scanner_buf_appends_code_point(s, cp);
                    } නැතහොත් {
                        scanner_warn(
                            s, XKB_WARNING_INVALID_UNICODE_ESCAPE_SEQUENCE,
                            "invalid Unicode escape sequence \"%.*s\" "
                            "in string literal",
                            (පූර්ණ_සංඛ්‍යා) (s->pos - start_pos + 1),
                            &s->s[start_pos - 1]
                        );
                        /* Ignore. */
                    }
                }
                නැතහොත් නම් (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (var_na) o);
                }
                නැතහොත් නම් (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "invalid octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (පූර්ණ_සංඛ්‍යා) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                නැතහොත් {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } නැතහොත් {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        නම් (!scanner_buf_append(s, '\0') || !scanner_chr(s, '\"')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated string literal");
            ආපසු ERROR_TOK;
        }
        yylval->str = strdup(s->buf);
        නම් (!yylval->str)
            ආපසු ERROR_TOK;
        ආපසු STRING;
    }

    /* Key name literal. */
    නම් (scanner_chr(s, '<')) {
        අතරතුර (is_graph(scanner_peek(s)) && scanner_peek(s) != '>')
            scanner_next(s);
        නම් (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unterminated key name literal");
            ආපසු ERROR_TOK;
        }
        /* Empty key name literals are allowed. */
        const var_na *start = s->s + s->token_pos + 1;
        const size_t len = s->pos - s->token_pos - 2;
        yylval->atom = xkb_atom_intern(s->ctx, start, len);
        ආපසු KEYNAME;
    }

    /* Operators and punctuation. */
    නම් (scanner_chr(s, ';')) ආපසු SEMI;
    නම් (scanner_chr(s, '{')) ආපසු OBRACE;
    නම් (scanner_chr(s, '}')) ආපසු CBRACE;
    නම් (scanner_chr(s, '=')) ආපසු EQUALS;
    නම් (scanner_chr(s, '[')) ආපසු OBRACKET;
    නම් (scanner_chr(s, ']')) ආපසු CBRACKET;
    නම් (scanner_chr(s, '(')) ආපසු OPAREN;
    නම් (scanner_chr(s, ')')) ආපසු CPAREN;
    නම් (scanner_chr(s, '.')) ආපසු DOT;
    නම් (scanner_chr(s, ',')) ආපසු COMMA;
    නම් (scanner_chr(s, '+')) ආපසු PLUS;
    නම් (scanner_chr(s, '-')) ආපසු MINUS;
    නම් (scanner_chr(s, '*')) ආපසු TIMES;
    නම් (scanner_chr(s, '/')) ආපසු DIVIDE;
    නම් (scanner_chr(s, '!')) ආපසු EXCLAM;
    නම් (scanner_chr(s, '~')) ආපසු INVERT;

    පූර්ණ_සංඛ්‍යා tok = ERROR_TOK;

    /* Identifier. */
    නම් (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        අතරතුර (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_next(s);

        const var_na *start = s->s + s->token_pos;
        const size_t len = s->pos - s->token_pos;

        /* Keyword. */
        tok = keyword_to_token(start, len);
        නම් (tok >= 0) ආපසු tok;

        yylval->sval = SVAL(start, len);
        ආපසු IDENT;
    }

    /* Number literal (hexadecimal / decimal / float). */
    නම් (number(s, &yylval->num, &tok)) {
        නම් (tok == ERROR_TOK) {
            scanner_err(s, XKB_ERROR_MALFORMED_NUMBER_LITERAL,
                        "malformed number literal");
            ආපසු ERROR_TOK;
        }
        ආපසු tok;
    }

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "unrecognized token");
    ආපසු ERROR_TOK;
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
    නම් (!scanner_check_supported_char_encoding(scanner)) {
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        ආපසු false;
    }

    ආපසු true;
}

XkbFile *
XkbParseString(sa_nracanaa xkb_context *ctx,
               const sa_nracanaa parser_keymap_config *config,
               const var_na *string, size_t len,
               const var_na *file_name, const var_na *map)
{
    sa_nracanaa scanner scanner;
    නම් (!XkbParseStringInit(ctx, &scanner, string, len, file_name, map))
        ආපසු NULL;

    ආපසු parse(ctx, config, &scanner, map);
}

bool
XkbParseStringNext(sa_nracanaa xkb_context *ctx,
                   const sa_nracanaa parser_keymap_config *config,
                   sa_nracanaa scanner *scanner, const var_na *map, XkbFile **out)
{
    නම් (map) {
        *out = parse(ctx, config, scanner, map);
        ආපසු !!(*out);
    } නැතහොත් {
        ආපසු parse_next(ctx, config, scanner, out);
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
    නම් (!ok) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read XKB file %s: %s\n",
                file_name, strerror(errno));
        ආපසු NULL;
    }

    xkb_file = XkbParseString(ctx, config, string, size, file_name, map);
    unmap_file(string, size);
    ආපසු xkb_file;
}
