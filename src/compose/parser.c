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

ಗಣನಾ rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_ಸಾಲು,
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
ಸಙ್ಘ lvalue {
    ಸಂರಚನಾ {
        /* Still \0-terminated. */
        ಧ್ರುವ ವರ್ಣ *str;
        size_t len;
    } string;
};

ಸ್ಥಿರ ಗಣನಾ rules_token
lex(ಸಂರಚನಾ scanner *s, ಸಙ್ಘ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    ಯಾವತ್ (is_space(scanner_peek(s)))
        ಯದಿ (scanner_next(s) == '\n')
            ಪ್ರತ್ಯಾಗಮ TOK_END_OF_ಸಾಲು;

    /* Skip comments. */
    ಯದಿ (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        ಗಚ್ಛ skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    ಯದಿ (scanner_eof(s)) ಪ್ರತ್ಯಾಗಮ TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    ಯದಿ (scanner_chr(s, '<')) {
        ಯಾವತ್ (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_next(s));
        ಯದಿ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
        }
        ಯದಿ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ಪ್ರತ್ಯಾಗಮ TOK_LHS_KEYSYM;
    }

    /* Colon. */
    ಯದಿ (scanner_chr(s, ':'))
        ಪ್ರತ್ಯಾಗಮ TOK_COLON;
    ಯದಿ (scanner_chr(s, '!'))
        ಪ್ರತ್ಯಾಗಮ TOK_BANG;
    ಯದಿ (scanner_chr(s, '~'))
        ಪ್ರತ್ಯಾಗಮ TOK_TILDE;

    /* String literal. */
    ಯದಿ (scanner_chr(s, '\"')) {
        ಯಾವತ್ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ಯದಿ (scanner_chr(s, '\\')) {
                uint8_t o;
                size_t start_pos = s->pos;
                ಯದಿ (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                ಅನ್ಯಥಾ ಯದಿ (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                ಅನ್ಯಥಾ ಯದಿ (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    ಯದಿ (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (ವರ್ಣ) o);
                    } ಅನ್ಯಥಾ {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (ಪೂರ್ಣಾಙ್ಕ) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                ಅನ್ಯಥಾ ಯದಿ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (ವರ್ಣ) o);
                }
                ಅನ್ಯಥಾ ಯದಿ (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (ಪೂರ್ಣಾಙ್ಕ) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                ಅನ್ಯಥಾ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ಅನ್ಯಥಾ {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        ಯದಿ (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
        }
        ಯದಿ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
        }
        ಯದಿ (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ಪ್ರತ್ಯಾಗಮ TOK_STRING;
    }

    /* Identifier or include. */
    ಯದಿ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        ಯಾವತ್ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_next(s));
        ಯದಿ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
        }

        ಯದಿ (streq(s->buf, "include"))
            ಪ್ರತ್ಯಾಗಮ TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ಪ್ರತ್ಯಾಗಮ TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
}

ಸ್ಥಿರ ಗಣನಾ rules_token
lex_include_string(ಸಂರಚನಾ scanner *s, ಸಂರಚನಾ xkb_compose_table *table,
                   ಸಙ್ಘ lvalue *val_out)
{
    ಯಾವತ್ (is_space(scanner_peek(s)))
        ಯದಿ (scanner_next(s) == '\n')
            ಪ್ರತ್ಯಾಗಮ TOK_END_OF_ಸಾಲು;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    ಯದಿ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
    }

    ಯಾವತ್ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        ಯದಿ (scanner_chr(s, '%')) {
            ಯದಿ (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            ಅನ್ಯಥಾ ಯದಿ (scanner_chr(s, 'H')) {
                ಧ್ರುವ ವರ್ಣ *home = xkb_context_getenv(table->ctx, "HOME");
                ಯದಿ (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
                }
                ಯದಿ (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
                }
            }
            ಅನ್ಯಥಾ ಯದಿ (scanner_chr(s, 'L')) {
                ವರ್ಣ *path = get_locale_compose_file_path(table->ctx, table->locale);
                ಯದಿ (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
                }
                ಯದಿ (!scanner_buf_appends(s, path)) {
                    ಮುಕ್ತ(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
                }
                ಮುಕ್ತ(path);
            }
            ಅನ್ಯಥಾ ಯದಿ (scanner_chr(s, 'S')) {
                ಧ್ರುವ ವರ್ಣ *xlocaledir = get_xlocaledir_path(table->ctx);
                ಯದಿ (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
                }
            }
            ಅನ್ಯಥಾ {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
            }
        } ಅನ್ಯಥಾ {
            scanner_buf_append(s, scanner_next(s));
        }
    }
    ಯದಿ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
    }
    ಯದಿ (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        ಪ್ರತ್ಯಾಗಮ TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    ಪ್ರತ್ಯಾಗಮ TOK_INCLUDE_STRING;
}

ಸಂರಚನಾ production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    unsigned ಪೂರ್ಣಾಙ್ಕ len;
    xkb_keysym_t keysym;
    ವರ್ಣ string[XKB_COMPOSE_MAX_STRING_SIZE];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

ಸ್ಥಿರ ಶೂನ್ಯ
add_production(ಸಂರಚನಾ xkb_compose_table *table, ಸಂರಚನಾ scanner *s,
               ಧ್ರುವ ಸಂರಚನಾ production *production)
{
    unsigned ಪೂರ್ಣಾಙ್ಕ lhs_pos = 0;
    uint32_t curr = darray_size(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = ರಿಕ್ತ;
    ಸಂರಚನಾ compose_ಗಂಟು *ಗಂಟು = ರಿಕ್ತ;

    /* Warn before potentially going over the limit, discard silently after. */
    ಯದಿ (darray_size(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    ಯದಿ (darray_size(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        ಪ್ರತ್ಯಾಗಮ;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    ಯಾವತ್ (true) {
        ಧ್ರುವ xkb_keysym_t keysym = production->lhs[lhs_pos];
        ಧ್ರುವ bool last = lhs_pos + 1 == production->len;

        ಯದಿ (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            ಸಂರಚನಾ compose_ಗಂಟು new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_size(table->nodes);
            ಯದಿ (pptr != ರಿಕ್ತ) {
                *pptr = curr;
                pptr = ರಿಕ್ತ;
            }
            darray_append(table->nodes, new);
        }

        ಗಂಟು = &darray_item(table->nodes, curr);

        ಯದಿ (keysym < ಗಂಟು->keysym) {
            pptr = &ಗಂಟು->lokid;
            curr = ಗಂಟು->lokid;
        } ಅನ್ಯಥಾ ಯದಿ (keysym > ಗಂಟು->keysym) {
            pptr = &ಗಂಟು->hikid;
            curr = ಗಂಟು->hikid;
        } ಅನ್ಯಥಾ ಯದಿ (!last) {
            ಯದಿ (ಗಂಟು->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                ಗಂಟು->internal.eqkid = 0;
                ಗಂಟು->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &ಗಂಟು->internal.eqkid;
            curr = ಗಂಟು->internal.eqkid;
        } ಅನ್ಯಥಾ {
            ಯದಿ (ಗಂಟು->is_leaf) {
                bool same_string =
                    (ಗಂಟು->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        ಗಂಟು->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, ಗಂಟು->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (ಗಂಟು->leaf.keysym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ && !production->has_keysym) ||
                    (
                        ಗಂಟು->leaf.keysym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ && production->has_keysym &&
                        ಗಂಟು->leaf.keysym == production->keysym
                    );
                ಯದಿ (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    ಪ್ರತ್ಯಾಗಮ;
                } ಅನ್ಯಥಾ {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } ಅನ್ಯಥಾ ಯದಿ (ಗಂಟು->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                ಗಂಟು->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            ಯದಿ (production->has_string) {
                ಧ್ರುವ size_t len = strlen(production->string);
                ಯದಿ (ಗಂಟು->is_leaf && ಗಂಟು->leaf.utf8 &&
                    len <= strlen(&darray_item(table->utf8, ಗಂಟು->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    memcpy(&darray_item(table->utf8, ಗಂಟು->leaf.utf8),
                           production->string, len + 1);
                } ಅನ್ಯಥಾ {
                    /* Cannot reuse string: allocate new one */
                    ಗಂಟು->leaf.utf8 = darray_size(table->utf8);
                    darray_append_ವಸ್ತುಗಳು(table->utf8, production->string,
                                        (darray_size_t)strlen(production->string) + 1);
                }
            } ಅನ್ಯಥಾ {
                /* Ensure we reset possible previous entry */
                ಗಂಟು->leaf.utf8 = 0;
            }

            ಯದಿ (production->has_keysym) {
                ಗಂಟು->leaf.keysym = production->keysym;
            } ಅನ್ಯಥಾ {
                /* Ensure we reset possible previous entry */
                ಗಂಟು->leaf.keysym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
            }

            ಗಂಟು->is_leaf = true;
            ಪ್ರತ್ಯಾಗಮ;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

ಸ್ಥಿರ xkb_mod_index_t
resolve_modifier(ಧ್ರುವ ವರ್ಣ *name)
{
    ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ {
        ಧ್ರುವ ವರ್ಣ *name;
        xkb_mod_index_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ARRAY_SIZE(mods); i++)
        ಯದಿ (streq(name, mods[i].name))
            ಪ್ರತ್ಯಾಗಮ mods[i].mod;

    ಪ್ರತ್ಯಾಗಮ XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
ವರ್ಣ *
parse_string_literal(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ವರ್ಣ *string)
{
    ಸಂರಚನಾ scanner s;
    ಸಙ್ಘ lvalue val;
    scanner_init(&s, ctx, string, strlen(string), "(unnamed)", ರಿಕ್ತ);
    ಯದಿ (lex(&s, &val) == TOK_STRING) {
        ಪ್ರತ್ಯಾಗಮ strdup(val.string.str);
    } ಅನ್ಯಥಾ {
        fprintf(stderr, "ERROR: %s\n", s.s);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
}

ಸ್ಥಿರ bool
parse(ಸಂರಚನಾ xkb_compose_table *table, ಸಂರಚನಾ scanner *s,
      unsigned ಪೂರ್ಣಾಙ್ಕ include_depth);

ಸ್ಥಿರ bool
do_include(ಸಂರಚನಾ xkb_compose_table *table, ಸಂರಚನಾ scanner *s,
           ಧ್ರುವ ವರ್ಣ *path, unsigned ಪೂರ್ಣಾಙ್ಕ include_depth)
{
    FILE *file;
    bool ok;
    ವರ್ಣ *string;
    size_t size;
    ಸಂರಚನಾ scanner new_s;

    ಯದಿ (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    file = fopen(path, "rb");
    ಯದಿ (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ok = map_file(file, &string, &size);
    ಯದಿ (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        ಗಚ್ಛ err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    ಯದಿ (!ok)
        ಗಚ್ಛ err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    fclose(file);
    ಪ್ರತ್ಯಾಗಮ ok;
}

// NOLINTBEGIN(readability-trivial-switch)
ಸ್ಥಿರ bool
parse(ಸಂರಚನಾ xkb_compose_table *table, ಸಂರಚನಾ scanner *s,
      unsigned ಪೂರ್ಣಾಙ್ಕ include_depth)
{
    ಗಣನಾ rules_token tok;
    ಸಙ್ಘ lvalue val;
    xkb_keysym_t keysym;
    ಸಂರಚನಾ production production;
    ಗಣನಾ { MAX_ERRORS = 10 };
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    ಯದಿ (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        ಗಚ್ಛ fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    ವಿಕಲ್ಪ (tok = lex(s, &val)) {
    ಪಕ್ಷ TOK_END_OF_ಸಾಲು:
        ಗಚ್ಛ initial_eol;
    ಪಕ್ಷ TOK_END_OF_FILE:
        ಗಚ್ಛ finished;
    ಪಕ್ಷ TOK_INCLUDE:
        ಗಚ್ಛ include;
    ಸ್ವಭಾವ:
        ಗಚ್ಛ lhs_tok;
    }

include:
    ವಿಕಲ್ಪ (tok = lex_include_string(s, table, &val)) {
    ಪಕ್ಷ TOK_INCLUDE_STRING:
        ಗಚ್ಛ include_eol;
    ಸ್ವಭಾವ:
        ಗಚ್ಛ unexpected;
    }

include_eol:
    ವಿಕಲ್ಪ (tok = lex(s, &val)) {
    ಪಕ್ಷ TOK_END_OF_ಸಾಲು:
        ಯದಿ (!do_include(table, s, val.string.str, include_depth))
            ಗಚ್ಛ fail;
        ಗಚ್ಛ initial;
    ಸ್ವಭಾವ:
        ಗಚ್ಛ unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    ವಿಕಲ್ಪ (tok) {
    ಪಕ್ಷ TOK_COLON:
        ಯದಿ (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            ಗಚ್ಛ skip;
        }
        ಗಚ್ಛ rhs;
    ಪಕ್ಷ TOK_IDENT:
        ಯದಿ (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            ಗಚ್ಛ lhs_keysym;
        }
        ಗಚ್ಛ lhs_mod_list_tok;
    ಪಕ್ಷ TOK_TILDE:
        ಗಚ್ಛ lhs_mod_list_tok;
    ಪಕ್ಷ TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        ಗಚ್ಛ lhs_mod_list;
    ಸ್ವಭಾವ:
        ಗಚ್ಛ lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    ವಿಕಲ್ಪ (tok) {
    ಪಕ್ಷ TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ಯದಿ (keysym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            ಗಚ್ಛ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ಯದಿ (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            ಗಚ್ಛ skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        ಗಚ್ಛ lhs;
    ಸ್ವಭಾವ:
        ಗಚ್ಛ unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_index_t mod;

        ಯದಿ (tok != TOK_TILDE && tok != TOK_IDENT)
            ಗಚ್ಛ lhs_keysym_tok;

        ಯದಿ (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        ಯದಿ (tok != TOK_IDENT)
            ಗಚ್ಛ unexpected;

        mod = resolve_modifier(val.string.str);
        ಯದಿ (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            ಗಚ್ಛ error;
        }

        production.modmask |= UINT32_C(1) << mod;
        ಯದಿ (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        ಅನ್ಯಥಾ
            production.mods |= UINT32_C(1) << mod;

        ಗಚ್ಛ lhs_mod_list;
    }

rhs:
    ವಿಕಲ್ಪ (tok = lex(s, &val)) {
    ಪಕ್ಷ TOK_STRING:
        ಯದಿ (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            ಗಚ್ಛ skip;
        }
        ಯದಿ (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            ಗಚ್ಛ skip;
        }
        ಯದಿ (val.string.len > ಪರಿಮಾಣ(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         ಪರಿಮಾಣ(production.string) - 1,
                         val.string.len - 1);
            ಗಚ್ಛ skip;
        }
        memcpy(production.string, val.string.str, val.string.len);
        production.has_string = true;
        ಗಚ್ಛ rhs;
    ಪಕ್ಷ TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ಯದಿ (keysym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            ಗಚ್ಛ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ಯದಿ (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            ಗಚ್ಛ skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    ಪಕ್ಷ TOK_END_OF_ಸಾಲು:
        ಯದಿ (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            ಗಚ್ಛ skip;
        }
        add_production(table, s, &production);
        ಗಚ್ಛ initial;
    ಸ್ವಭಾವ:
        ಗಚ್ಛ unexpected;
    }

unexpected:
    ಯದಿ (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    ಅಂಕೆ_errors++;
    ಯದಿ (ಅಂಕೆ_errors <= MAX_ERRORS)
        ಗಚ್ಛ skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    ಗಚ್ಛ fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    ಪ್ರತ್ಯಾಗಮ false;

skip:
    ಯಾವತ್ (tok != TOK_END_OF_ಸಾಲು && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    ಗಚ್ಛ initial;

finished:
    ಪ್ರತ್ಯಾಗಮ true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(ಸಂರಚನಾ xkb_compose_table *table, ಧ್ರುವ ವರ್ಣ *string, size_t len,
             ಧ್ರುವ ವರ್ಣ *file_name)
{
    ಸಂರಚನಾ scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, ರಿಕ್ತ);
    ಯದಿ (!parse(table, &s, 0))
        ಪ್ರತ್ಯಾಗಮ false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    ಪ್ರತ್ಯಾಗಮ true;
}

bool
parse_file(ಸಂರಚನಾ xkb_compose_table *table, FILE *file, ಧ್ರುವ ವರ್ಣ *file_name)
{
    bool ok;
    ವರ್ಣ *string;
    size_t size;

    ok = map_file(file, &string, &size);
    ಯದಿ (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    ಪ್ರತ್ಯಾಗಮ ok;
}
