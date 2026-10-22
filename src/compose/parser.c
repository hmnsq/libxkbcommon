/*
 * For HPND-sell-variant:
 * Copyright 1992 by Oki Technosystems Laboratory, Inc.
 * Copyright 1992 by Fuji Xerox Co., Ltd.
 *
 * For MIT:
 * Copyright © 2013 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND-sell-variant AND MIT
 *
 * Author: Yasuhiro Kawai        Oki Technosystems Laboratory
 * Author: Kazunori Nishihara    Fuji Xerox
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-keysyms.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "constants.h"
#ಅನ್ತರ್ಭಾವ "table.h"
#ಅನ್ತರ್ಭಾವ "scanner-utils.h"
#ಅನ್ತರ್ಭಾವ "paths.h"
#ಅನ್ತರ್ಭಾವ "utf8.h"
#ಅನ್ತರ್ಭಾವ "parser.h"
#ಅನ್ತರ್ಭಾವ "keysym.h"

/*
 * Grammar adapted from libX11/modules/im/ximcp/imLcPrs.c.
 * See also the XCompose(5) manpage.
 *
 * FILE          ::= { [PRODUCTION] [COMMENT] "\n" | INCLUDE }
 * INCLUDE       ::= "include" '"' INCLUDE_STRING '"'
 * PRODUCTION    ::= LHS ":" RHS [ COMMENT ]
 * COMMENT       ::= "#" {<any character except null or newline>}
 * LHS           ::= EVENT { EVENT }
 * EVENT         ::= [MODIFIER_LIST] "<" keysym ">"
 * MODIFIER_LIST ::= (["!"] {MODIFIER} ) | "None"
 * MODIFIER      ::= ["~"] MODIFIER_NAME
 * MODIFIER_NAME ::= ("Ctrl"|"Lock"|"Caps"|"Shift"|"Alt"|"Meta")
 * RHS           ::= ( STRING | keysym | STRING keysym )
 * STRING        ::= '"' { CHAR } '"'
 * CHAR          ::= GRAPHIC_CHAR | ESCAPED_CHAR
 * GRAPHIC_CHAR  ::= locale (codeset) dependent code
 * ESCAPED_CHAR  ::= ('\\' | '\"' | OCTAL | HEX )
 * OCTAL         ::= '\' OCTAL_CHAR [OCTAL_CHAR [OCTAL_CHAR]]
 * OCTAL_CHAR    ::= (0|1|2|3|4|5|6|7)
 * HEX           ::= '\' (x|X) HEX_CHAR [HEX_CHAR]]
 * HEX_CHAR      ::= (0|1|2|3|4|5|6|7|8|9|A|B|C|D|E|F|a|b|c|d|e|f)
 *
 * INCLUDE_STRING is a filesystem path, with the following %-expansions:
 *     %% - '%'.
 *     %H - The user's home directory (the $HOME environment variable).
 *     %L - The name of the locale specific Compose file (e.g.,
 *          "/usr/share/X11/locale/<localename>/Compose").
 *     %S - The name of the system directory for Compose files (e.g.,
 *          "/usr/share/X11/locale").
 */

ಎಣಿಕೆ ನಿಯಮಗಳು_token {
    TOK_ಕೊನೆ_ದ_FILE = 0,
    TOK_ಕೊನೆ_ದ_ಸಾಲು,
    TOK_INCLUDE,
    TOK_INCLUDE_STRING,
    TOK_LHS_KEYSYM,
    TOK_COLON,
    TOK_BANG,
    TOK_TILDE,
    TOK_STRING,
    TOK_IDENT,
    TOK_ERROR
};

/* Values returned with some tokens, like yylval. */
ಒಕ್ಕೂಟ lvalue {
    ರಚನೆ {
        /* Still \0-terminated. */
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str;
        ಗಾತ್ರ_t len;
    } string;
};

ಸ್ಥಿರ ಎಣಿಕೆ ನಿಯಮಗಳು_token
lex(ರಚನೆ scanner *s, ಒಕ್ಕೂಟ lvalue *val)
{
skip_ಇನ್ನಷ್ಟು_whitespace_and_comments:
    /* Skip spaces. */
    ಆಗ (is_space(scanner_peek(s)))
        ಆದರೆ (scanner_ಮುಂದಿನ(s) == '\n')
            ಹಿಂತಿರುಗಿ TOK_ಕೊನೆ_ದ_ಸಾಲು;

    /* Skip comments. */
    ಆದರೆ (scanner_chr(s, '#')) {
        scanner_skip_ಗಮ್ಯ_eol(s);
        ಹೋಗು skip_ಇನ್ನಷ್ಟು_whitespace_and_comments;
    }

    /* See if we're done. */
    ಆದರೆ (scanner_eof(s)) ಹಿಂತಿರುಗಿ TOK_ಕೊನೆ_ದ_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    ಆದರೆ (scanner_chr(s, '<')) {
        ಆಗ (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_ಮುಂದಿನ(s));
        ಆದರೆ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            ಹಿಂತಿರುಗಿ TOK_ERROR;
        }
        ಆದರೆ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            ಹಿಂತಿರುಗಿ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ಹಿಂತಿರುಗಿ TOK_LHS_KEYSYM;
    }

    /* Colon. */
    ಆದರೆ (scanner_chr(s, ':'))
        ಹಿಂತಿರುಗಿ TOK_COLON;
    ಆದರೆ (scanner_chr(s, '!'))
        ಹಿಂತಿರುಗಿ TOK_BANG;
    ಆದರೆ (scanner_chr(s, '~'))
        ಹಿಂತಿರುಗಿ TOK_TILDE;

    /* String literal. */
    ಆದರೆ (scanner_chr(s, '\"')) {
        ಆಗ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ಆದರೆ (scanner_chr(s, '\\')) {
                uint8_t o;
                ಗಾತ್ರ_t ಆರಂಭ_pos = s->pos;
                ಆದರೆ (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    ಆದರೆ (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (ಅಕ್ಷರ) o);
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (ಪೂರ್ಣಾಂಕ) (s->pos - ಆರಂಭ_pos + 1),
                                     &s->s[ಆರಂಭ_pos - 1]);
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (ಅಕ್ಷರ) o);
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (s->pos > ಆರಂಭ_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (ಪೂರ್ಣಾಂಕ) (s->pos - ಆರಂಭ_pos + 1),
                                 &s->s[ಆರಂಭ_pos - 1]);
                    /* Ignore. */
                }
                ಇಲ್ಲದಿದ್ದರೆ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                scanner_buf_append(s, scanner_ಮುಂದಿನ(s));
            }
        }
        ಆದರೆ (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            ಹಿಂತಿರುಗಿ TOK_ERROR;
        }
        ಆದರೆ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            ಹಿಂತಿರುಗಿ TOK_ERROR;
        }
        ಆದರೆ (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            ಹಿಂತಿರುಗಿ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ಹಿಂತಿರುಗಿ TOK_STRING;
    }

    /* Identifier or include. */
    ಆದರೆ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        ಆಗ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_ಮುಂದಿನ(s));
        ಆದರೆ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            ಹಿಂತಿರುಗಿ TOK_ERROR;
        }

        ಆದರೆ (streq(s->buf, "include"))
            ಹಿಂತಿರುಗಿ TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ಹಿಂತಿರುಗಿ TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_ಗಮ್ಯ_eol(s);
    ಹಿಂತಿರುಗಿ TOK_ERROR;
}

ಸ್ಥಿರ ಎಣಿಕೆ ನಿಯಮಗಳು_token
lex_include_string(ರಚನೆ scanner *s, ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
                   ಒಕ್ಕೂಟ lvalue *val_ಹೊರಹೋಗು)
{
    ಆಗ (is_space(scanner_peek(s)))
        ಆದರೆ (scanner_ಮುಂದಿನ(s) == '\n')
            ಹಿಂತಿರುಗಿ TOK_ಕೊನೆ_ದ_ಸಾಲು;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    ಆದರೆ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        ಹಿಂತಿರುಗಿ TOK_ERROR;
    }

    ಆಗ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        ಆದರೆ (scanner_chr(s, '%')) {
            ಆದರೆ (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scanner_chr(s, 'H')) {
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *home = xkb_context_getenv(ಕೋಷ್ಟಕ->ctx, "HOME");
                ಆದರೆ (!home) {
                    scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    ಹಿಂತಿರುಗಿ TOK_ERROR;
                }
                ಆದರೆ (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                "include path after expanding %%H is too long");
                    ಹಿಂತಿರುಗಿ TOK_ERROR;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scanner_chr(s, 'L')) {
                ಅಕ್ಷರ *ಮಾರ್ಗ = get_locale_compose_file_ಮಾರ್ಗ(ಕೋಷ್ಟಕ->ctx, ಕೋಷ್ಟಕ->locale);
                ಆದರೆ (!ಮಾರ್ಗ) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    ಹಿಂತಿರುಗಿ TOK_ERROR;
                }
                ಆದರೆ (!scanner_buf_appends(s, ಮಾರ್ಗ)) {
                    ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
                    scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                "include path after expanding %%L is too long");
                    ಹಿಂತಿರುಗಿ TOK_ERROR;
                }
                ಬಿಡುಗಡೆ(ಮಾರ್ಗ);
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (scanner_chr(s, 'S')) {
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *xlocaledir = get_xlocaledir_ಮಾರ್ಗ(ಕೋಷ್ಟಕ->ctx);
                ಆದರೆ (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                "include path after expanding %%S is too long");
                    ಹಿಂತಿರುಗಿ TOK_ERROR;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                ಹಿಂತಿರುಗಿ TOK_ERROR;
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            scanner_buf_append(s, scanner_ಮುಂದಿನ(s));
        }
    }
    ಆದರೆ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        ಹಿಂತಿರುಗಿ TOK_ERROR;
    }
    ಆದರೆ (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "include path is too long");
        ಹಿಂತಿರುಗಿ TOK_ERROR;
    }
    val_ಹೊರಹೋಗು->string.str = s->buf;
    val_ಹೊರಹೋಗು->string.len = s->buf_pos;
    ಹಿಂತಿರುಗಿ TOK_INCLUDE_STRING;
}

ರಚನೆ production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len;
    xkb_keysym_t keysym;
    ಅಕ್ಷರ string[XKB_COMPOSE_MAX_STRING_ಗಾತ್ರ];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_ಮುಖವಾಡ_t modmask;
    xkb_mod_ಮುಖವಾಡ_t mods;
};

ಸ್ಥಿರ ಶೂನ್ಯ
ಕೂಡಿಸು_production(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ರಚನೆ scanner *s,
               ಸ್ಥಿರಾಂಕ ರಚನೆ production *production)
{
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ lhs_pos = 0;
    uint32_t curr = darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = ಖಾಲಿ;
    ರಚನೆ compose_ಗಂಟು *ಗಂಟು = ಖಾಲಿ;

    /* Warn before potentially going over the limit, discard silently after. */
    ಆದರೆ (darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    ಆದರೆ (darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->nodes) + production->len >= MAX_COMPOSE_NODES)
        ಹಿಂತಿರುಗಿ;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    ಆಗ (true) {
        ಸ್ಥಿರಾಂಕ xkb_keysym_t keysym = production->lhs[lhs_pos];
        ಸ್ಥಿರಾಂಕ bool ಕೊನೆಯ = lhs_pos + 1 == production->len;

        ಆದರೆ (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            ರಚನೆ compose_ಗಂಟು new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .ಆಂತರಿಕ = {
                    .eqkid = 0,
                    .is_ಎಲೆ = false,
                },
            };
            curr = darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->nodes);
            ಆದರೆ (pptr != ಖಾಲಿ) {
                *pptr = curr;
                pptr = ಖಾಲಿ;
            }
            darray_append(ಕೋಷ್ಟಕ->nodes, new);
        }

        ಗಂಟು = &darray_ಅಂಶ(ಕೋಷ್ಟಕ->nodes, curr);

        ಆದರೆ (keysym < ಗಂಟು->keysym) {
            pptr = &ಗಂಟು->lokid;
            curr = ಗಂಟು->lokid;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (keysym > ಗಂಟು->keysym) {
            pptr = &ಗಂಟು->hikid;
            curr = ಗಂಟು->hikid;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಕೊನೆಯ) {
            ಆದರೆ (ಗಂಟು->is_ಎಲೆ) {
                scanner_warn(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                ಗಂಟು->ಆಂತರಿಕ.eqkid = 0;
                ಗಂಟು->ಆಂತರಿಕ.is_ಎಲೆ = false;
            }
            lhs_pos++;
            pptr = &ಗಂಟು->ಆಂತರಿಕ.eqkid;
            curr = ಗಂಟು->ಆಂತರಿಕ.eqkid;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (ಗಂಟು->is_ಎಲೆ) {
                bool same_string =
                    (ಗಂಟು->ಎಲೆ.utf8 == 0 && !production->has_string) ||
                    (
                        ಗಂಟು->ಎಲೆ.utf8 != 0 && production->has_string &&
                        streq(&darray_ಅಂಶ(ಕೋಷ್ಟಕ->utf8, ಗಂಟು->ಎಲೆ.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (ಗಂಟು->ಎಲೆ.keysym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ && !production->has_keysym) ||
                    (
                        ಗಂಟು->ಎಲೆ.keysym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ && production->has_keysym &&
                        ಗಂಟು->ಎಲೆ.keysym == production->keysym
                    );
                ಆದರೆ (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    ಹಿಂತಿರುಗಿ;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    scanner_warn(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗಂಟು->ಆಂತರಿಕ.eqkid != 0) {
                scanner_warn(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                ಗಂಟು->ಆಂತರಿಕ.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            ಆದರೆ (production->has_string) {
                ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(production->string);
                ಆದರೆ (ಗಂಟು->is_ಎಲೆ && ಗಂಟು->ಎಲೆ.utf8 &&
                    len <= ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(&darray_ಅಂಶ(ಕೋಷ್ಟಕ->utf8, ಗಂಟು->ಎಲೆ.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    ಸ್ಮರಣೆ_ನಕಲು(&darray_ಅಂಶ(ಕೋಷ್ಟಕ->utf8, ಗಂಟು->ಎಲೆ.utf8),
                           production->string, len + 1);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    /* Cannot reuse string: allocate new one */
                    ಗಂಟು->ಎಲೆ.utf8 = darray_ಗಾತ್ರ(ಕೋಷ್ಟಕ->utf8);
                    darray_append_ವಸ್ತುಗಳು(ಕೋಷ್ಟಕ->utf8, production->string,
                                        (darray_ಗಾತ್ರ_t)ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(production->string) + 1);
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Ensure we reset possible previous entry */
                ಗಂಟು->ಎಲೆ.utf8 = 0;
            }

            ಆದರೆ (production->has_keysym) {
                ಗಂಟು->ಎಲೆ.keysym = production->keysym;
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Ensure we reset possible previous entry */
                ಗಂಟು->ಎಲೆ.keysym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
            }

            ಗಂಟು->is_ಎಲೆ = true;
            ಹಿಂತಿರುಗಿ;
        }
    }
}

/* Should match resolve_modifier(). */
#ವ್ಯಾಖ್ಯಾನಿಸು ALL_MODS_ಮುಖವಾಡ ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

ಸ್ಥಿರ xkb_mod_ಸೂಚ್ಯಂಕ_t
resolve_modifier(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
        xkb_mod_ಸೂಚ್ಯಂಕ_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; i < ARRAY_ಗಾತ್ರ(mods); i++)
        ಆದರೆ (streq(name, mods[i].name))
            ಹಿಂತಿರುಗಿ mods[i].mod;

    ಹಿಂತಿರುಗಿ XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
ಅಕ್ಷರ *
parse_string_literal(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string)
{
    ರಚನೆ scanner s;
    ಒಕ್ಕೂಟ lvalue val;
    scanner_init(&s, ctx, string, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(string), "(unnamed)", ಖಾಲಿ);
    ಆದರೆ (lex(&s, &val) == TOK_STRING) {
        ಹಿಂತಿರುಗಿ strdup(val.string.str);
    } ಇಲ್ಲದಿದ್ದರೆ {
        fprintf(stderr, "ERROR: %s\n", s.s);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
}

ಸ್ಥಿರ bool
parse(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ರಚನೆ scanner *s,
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ);

ಸ್ಥಿರ bool
do_include(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ರಚನೆ scanner *s,
           ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ)
{
    FILE *file;
    bool ok;
    ಅಕ್ಷರ *string;
    ಗಾತ್ರ_t ಗಾತ್ರ_2;
    ರಚನೆ scanner new_s;

    ಆದರೆ (include_ಆಳ >= COMPOSE_MAX_INCLUDE_ಆಳ) {
        scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_ಆಳ);
        ಹಿಂತಿರುಗಿ false;
    }

    file = fopen(ಮಾರ್ಗ, "rb");
    ಆದರೆ (!file) {
        scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    ಮಾರ್ಗ, strerror(errno));
        ಹಿಂತಿರುಗಿ false;
    }

    ok = map_file(file, &string, &ಗಾತ್ರ_2);
    ಆದರೆ (!ok) {
        scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    ಮಾರ್ಗ, strerror(errno));
        ಹೋಗು err_file;
    }

    scanner_init(&new_s, ಕೋಷ್ಟಕ->ctx, string, ಗಾತ್ರ_2, ಮಾರ್ಗ, s->priv);

    ok = parse(ಕೋಷ್ಟಕ, &new_s, include_ಆಳ + 1);
    ಆದರೆ (!ok)
        ಹೋಗು err_unmap;

err_unmap:
    unmap_file(string, ಗಾತ್ರ_2);
err_file:
    fclose(file);
    ಹಿಂತಿರುಗಿ ok;
}

// NOLINTBEGIN(readability-trivial-switch)
ಸ್ಥಿರ bool
parse(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ರಚನೆ scanner *s,
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ)
{
    ಎಣಿಕೆ ನಿಯಮಗಳು_token tok;
    ಒಕ್ಕೂಟ lvalue val;
    xkb_keysym_t keysym;
    ರಚನೆ production production;
    ಎಣಿಕೆ { MAX_ERRORS = 10 };
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    ಆದರೆ (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        ಹೋಗು fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    ಆಯ್ಕೆ (tok = lex(s, &val)) {
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_ಸಾಲು:
        ಹೋಗು initial_eol;
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_FILE:
        ಹೋಗು ಪೂರ್ಣಗೊಂಡ;
    ಸ್ಥಿತಿ TOK_INCLUDE:
        ಹೋಗು include;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು lhs_tok;
    }

include:
    ಆಯ್ಕೆ (tok = lex_include_string(s, ಕೋಷ್ಟಕ, &val)) {
    ಸ್ಥಿತಿ TOK_INCLUDE_STRING:
        ಹೋಗು include_eol;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

include_eol:
    ಆಯ್ಕೆ (tok = lex(s, &val)) {
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_ಸಾಲು:
        ಆದರೆ (!do_include(ಕೋಷ್ಟಕ, s, val.string.str, include_ಆಳ))
            ಹೋಗು fail;
        ಹೋಗು initial;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    ಆಯ್ಕೆ (tok) {
    ಸ್ಥಿತಿ TOK_COLON:
        ಆದರೆ (production.len <= 0) {
            scanner_warn(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            ಹೋಗು skip;
        }
        ಹೋಗು rhs;
    ಸ್ಥಿತಿ TOK_IDENT:
        ಆದರೆ (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_ಮುಖವಾಡ;
            ಹೋಗು lhs_keysym;
        }
        ಹೋಗು lhs_mod_list_tok;
    ಸ್ಥಿತಿ TOK_TILDE:
        ಹೋಗು lhs_mod_list_tok;
    ಸ್ಥಿತಿ TOK_BANG:
        production.modmask = ALL_MODS_ಮುಖವಾಡ;
        ಹೋಗು lhs_mod_list;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    ಆಯ್ಕೆ (tok) {
    ಸ್ಥಿತಿ TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ಆದರೆ (keysym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
            scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            ಹೋಗು error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ಆದರೆ (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            ಹೋಗು skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        ಹೋಗು lhs;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_ಸೂಚ್ಯಂಕ_t mod;

        ಆದರೆ (tok != TOK_TILDE && tok != TOK_IDENT)
            ಹೋಗು lhs_keysym_tok;

        ಆದರೆ (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        ಆದರೆ (tok != TOK_IDENT)
            ಹೋಗು unexpected;

        mod = resolve_modifier(val.string.str);
        ಆದರೆ (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            ಹೋಗು error;
        }

        production.modmask |= UINT32_C(1) << mod;
        ಆದರೆ (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        ಇಲ್ಲದಿದ್ದರೆ
            production.mods |= UINT32_C(1) << mod;

        ಹೋಗು lhs_mod_list;
    }

rhs:
    ಆಯ್ಕೆ (tok = lex(s, &val)) {
    ಸ್ಥಿತಿ TOK_STRING:
        ಆದರೆ (production.has_string) {
            scanner_warn(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            ಹೋಗು skip;
        }
        ಆದರೆ (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            ಹೋಗು skip;
        }
        ಆದರೆ (val.string.len > ಗಾತ್ರ(production.string)) {
            scanner_warn(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         ಗಾತ್ರ(production.string) - 1,
                         val.string.len - 1);
            ಹೋಗು skip;
        }
        ಸ್ಮರಣೆ_ನಕಲು(production.string, val.string.str, val.string.len);
        production.has_string = true;
        ಹೋಗು rhs;
    ಸ್ಥಿತಿ TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ಆದರೆ (keysym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
            scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            ಹೋಗು error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ಆದರೆ (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            ಹೋಗು skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    ಸ್ಥಿತಿ TOK_ಕೊನೆ_ದ_ಸಾಲು:
        ಆದರೆ (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            ಹೋಗು skip;
        }
        ಕೂಡಿಸು_production(ಕೋಷ್ಟಕ, s, &production);
        ಹೋಗು initial;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ಹೋಗು unexpected;
    }

unexpected:
    ಆದರೆ (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    ಅಂಕೆ_errors++;
    ಆದರೆ (ಅಂಕೆ_errors <= MAX_ERRORS)
        ಹೋಗು skip;

    scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "too many errors");
    ಹೋಗು fail;

fail:
    scanner_err(s, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "failed to parse file");
    ಹಿಂತಿರುಗಿ false;

skip:
    ಆಗ (tok != TOK_ಕೊನೆ_ದ_ಸಾಲು && tok != TOK_ಕೊನೆ_ದ_FILE)
        tok = lex(s, &val);
    ಹೋಗು initial;

ಪೂರ್ಣಗೊಂಡ:
    ಹಿಂತಿರುಗಿ true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string, ಗಾತ್ರ_t len,
             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name)
{
    ರಚನೆ scanner s;
    scanner_init(&s, ಕೋಷ್ಟಕ->ctx, string, len, file_name, ಖಾಲಿ);
    ಆದರೆ (!parse(ಕೋಷ್ಟಕ, &s, 0))
        ಹಿಂತಿರುಗಿ false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(ಕೋಷ್ಟಕ->nodes);
    darray_shrink(ಕೋಷ್ಟಕ->utf8);
    ಹಿಂತಿರುಗಿ true;
}

bool
parse_file(ರಚನೆ xkb_compose_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, FILE *file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file_name)
{
    bool ok;
    ಅಕ್ಷರ *string;
    ಗಾತ್ರ_t ಗಾತ್ರ_2;

    ok = map_file(file, &string, &ಗಾತ್ರ_2);
    ಆದರೆ (!ok) {
        log_err(ಕೋಷ್ಟಕ->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        ಹಿಂತಿರುಗಿ false;
    }

    ok = parse_string(ಕೋಷ್ಟಕ, string, ಗಾತ್ರ_2, file_name);
    unmap_file(string, ಗಾತ್ರ_2);
    ಹಿಂತಿರುಗಿ ok;
}
