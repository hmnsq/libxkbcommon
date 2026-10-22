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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon-keysyms.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "constants.h"
#അന്തര്ഭാവ "table.h"
#അന്തര്ഭാവ "scanner-utils.h"
#അന്തര്ഭാവ "paths.h"
#അന്തര്ഭാവ "utf8.h"
#അന്തര്ഭാവ "parser.h"
#അന്തര്ഭാവ "keysym.h"

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

എണ്ണൽ rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_വരി,
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
സംഘം lvalue {
    ഘടന {
        /* Still \0-terminated. */
        സ്ഥിരം അക്ഷരം *str;
        size_t len;
    } string;
};

സ്ഥിതം എണ്ണൽ rules_token
lex(ഘടന scanner *s, സംഘം lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    ആകുമ്പോൾ (is_space(scanner_peek(s)))
        എങ്കിൽ (scanner_next(s) == '\n')
            തിരികെ TOK_END_OF_വരി;

    /* Skip comments. */
    എങ്കിൽ (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        പോകുക skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    എങ്കിൽ (scanner_eof(s)) തിരികെ TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    എങ്കിൽ (scanner_chr(s, '<')) {
        ആകുമ്പോൾ (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_next(s));
        എങ്കിൽ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            തിരികെ TOK_ERROR;
        }
        എങ്കിൽ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            തിരികെ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        തിരികെ TOK_LHS_KEYSYM;
    }

    /* Colon. */
    എങ്കിൽ (scanner_chr(s, ':'))
        തിരികെ TOK_COLON;
    എങ്കിൽ (scanner_chr(s, '!'))
        തിരികെ TOK_BANG;
    എങ്കിൽ (scanner_chr(s, '~'))
        തിരികെ TOK_TILDE;

    /* String literal. */
    എങ്കിൽ (scanner_chr(s, '\"')) {
        ആകുമ്പോൾ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            എങ്കിൽ (scanner_chr(s, '\\')) {
                uint8_t o;
                size_t start_pos = s->pos;
                എങ്കിൽ (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                അല്ലെങ്കിൽ എങ്കിൽ (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                അല്ലെങ്കിൽ എങ്കിൽ (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    എങ്കിൽ (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (അക്ഷരം) o);
                    } അല്ലെങ്കിൽ {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (പൂർണ്ണസംഖ്യ) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                അല്ലെങ്കിൽ എങ്കിൽ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (അക്ഷരം) o);
                }
                അല്ലെങ്കിൽ എങ്കിൽ (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (പൂർണ്ണസംഖ്യ) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                അല്ലെങ്കിൽ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } അല്ലെങ്കിൽ {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        എങ്കിൽ (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            തിരികെ TOK_ERROR;
        }
        എങ്കിൽ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            തിരികെ TOK_ERROR;
        }
        എങ്കിൽ (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            തിരികെ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        തിരികെ TOK_STRING;
    }

    /* Identifier or include. */
    എങ്കിൽ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        ആകുമ്പോൾ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_next(s));
        എങ്കിൽ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            തിരികെ TOK_ERROR;
        }

        എങ്കിൽ (streq(s->buf, "include"))
            തിരികെ TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        തിരികെ TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    തിരികെ TOK_ERROR;
}

സ്ഥിതം എണ്ണൽ rules_token
lex_include_string(ഘടന scanner *s, ഘടന xkb_compose_table *table,
                   സംഘം lvalue *val_out)
{
    ആകുമ്പോൾ (is_space(scanner_peek(s)))
        എങ്കിൽ (scanner_next(s) == '\n')
            തിരികെ TOK_END_OF_വരി;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    എങ്കിൽ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        തിരികെ TOK_ERROR;
    }

    ആകുമ്പോൾ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        എങ്കിൽ (scanner_chr(s, '%')) {
            എങ്കിൽ (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            അല്ലെങ്കിൽ എങ്കിൽ (scanner_chr(s, 'H')) {
                സ്ഥിരം അക്ഷരം *home = xkb_context_getenv(table->ctx, "HOME");
                എങ്കിൽ (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    തിരികെ TOK_ERROR;
                }
                എങ്കിൽ (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    തിരികെ TOK_ERROR;
                }
            }
            അല്ലെങ്കിൽ എങ്കിൽ (scanner_chr(s, 'L')) {
                അക്ഷരം *path = get_locale_compose_file_path(table->ctx, table->locale);
                എങ്കിൽ (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    തിരികെ TOK_ERROR;
                }
                എങ്കിൽ (!scanner_buf_appends(s, path)) {
                    സ്വതന്ത്രം(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    തിരികെ TOK_ERROR;
                }
                സ്വതന്ത്രം(path);
            }
            അല്ലെങ്കിൽ എങ്കിൽ (scanner_chr(s, 'S')) {
                സ്ഥിരം അക്ഷരം *xlocaledir = get_xlocaledir_path(table->ctx);
                എങ്കിൽ (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    തിരികെ TOK_ERROR;
                }
            }
            അല്ലെങ്കിൽ {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                തിരികെ TOK_ERROR;
            }
        } അല്ലെങ്കിൽ {
            scanner_buf_append(s, scanner_next(s));
        }
    }
    എങ്കിൽ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        തിരികെ TOK_ERROR;
    }
    എങ്കിൽ (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        തിരികെ TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    തിരികെ TOK_INCLUDE_STRING;
}

ഘടന production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    unsigned പൂർണ്ണസംഖ്യ len;
    xkb_keysym_t keysym;
    അക്ഷരം string[XKB_COMPOSE_MAX_STRING_SIZE];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

സ്ഥിതം ശൂന്യം
add_production(ഘടന xkb_compose_table *table, ഘടന scanner *s,
               സ്ഥിരം ഘടന production *production)
{
    unsigned പൂർണ്ണസംഖ്യ lhs_pos = 0;
    uint32_t curr = darray_size(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = ശൂന്യ;
    ഘടന compose_കണ്ണി *കണ്ണി = ശൂന്യ;

    /* Warn before potentially going over the limit, discard silently after. */
    എങ്കിൽ (darray_size(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    എങ്കിൽ (darray_size(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        തിരികെ;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    ആകുമ്പോൾ (true) {
        സ്ഥിരം xkb_keysym_t keysym = production->lhs[lhs_pos];
        സ്ഥിരം bool last = lhs_pos + 1 == production->len;

        എങ്കിൽ (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            ഘടന compose_കണ്ണി new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_size(table->nodes);
            എങ്കിൽ (pptr != ശൂന്യ) {
                *pptr = curr;
                pptr = ശൂന്യ;
            }
            darray_append(table->nodes, new);
        }

        കണ്ണി = &darray_item(table->nodes, curr);

        എങ്കിൽ (keysym < കണ്ണി->keysym) {
            pptr = &കണ്ണി->lokid;
            curr = കണ്ണി->lokid;
        } അല്ലെങ്കിൽ എങ്കിൽ (keysym > കണ്ണി->keysym) {
            pptr = &കണ്ണി->hikid;
            curr = കണ്ണി->hikid;
        } അല്ലെങ്കിൽ എങ്കിൽ (!last) {
            എങ്കിൽ (കണ്ണി->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                കണ്ണി->internal.eqkid = 0;
                കണ്ണി->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &കണ്ണി->internal.eqkid;
            curr = കണ്ണി->internal.eqkid;
        } അല്ലെങ്കിൽ {
            എങ്കിൽ (കണ്ണി->is_leaf) {
                bool same_string =
                    (കണ്ണി->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        കണ്ണി->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, കണ്ണി->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (കണ്ണി->leaf.keysym == XKB_താക്കോൽ_No_ചിഹ്നം && !production->has_keysym) ||
                    (
                        കണ്ണി->leaf.keysym != XKB_താക്കോൽ_No_ചിഹ്നം && production->has_keysym &&
                        കണ്ണി->leaf.keysym == production->keysym
                    );
                എങ്കിൽ (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    തിരികെ;
                } അല്ലെങ്കിൽ {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } അല്ലെങ്കിൽ എങ്കിൽ (കണ്ണി->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                കണ്ണി->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            എങ്കിൽ (production->has_string) {
                സ്ഥിരം size_t len = strlen(production->string);
                എങ്കിൽ (കണ്ണി->is_leaf && കണ്ണി->leaf.utf8 &&
                    len <= strlen(&darray_item(table->utf8, കണ്ണി->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    memcpy(&darray_item(table->utf8, കണ്ണി->leaf.utf8),
                           production->string, len + 1);
                } അല്ലെങ്കിൽ {
                    /* Cannot reuse string: allocate new one */
                    കണ്ണി->leaf.utf8 = darray_size(table->utf8);
                    darray_append_ഇനങ്ങൾ(table->utf8, production->string,
                                        (darray_size_t)strlen(production->string) + 1);
                }
            } അല്ലെങ്കിൽ {
                /* Ensure we reset possible previous entry */
                കണ്ണി->leaf.utf8 = 0;
            }

            എങ്കിൽ (production->has_keysym) {
                കണ്ണി->leaf.keysym = production->keysym;
            } അല്ലെങ്കിൽ {
                /* Ensure we reset possible previous entry */
                കണ്ണി->leaf.keysym = XKB_താക്കോൽ_No_ചിഹ്നം;
            }

            കണ്ണി->is_leaf = true;
            തിരികെ;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

സ്ഥിതം xkb_mod_index_t
resolve_modifier(സ്ഥിരം അക്ഷരം *name)
{
    സ്ഥിതം സ്ഥിരം ഘടന {
        സ്ഥിരം അക്ഷരം *name;
        xkb_mod_index_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ARRAY_SIZE(mods); i++)
        എങ്കിൽ (streq(name, mods[i].name))
            തിരികെ mods[i].mod;

    തിരികെ XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
അക്ഷരം *
parse_string_literal(ഘടന xkb_context *ctx, സ്ഥിരം അക്ഷരം *string)
{
    ഘടന scanner s;
    സംഘം lvalue val;
    scanner_init(&s, ctx, string, strlen(string), "(unnamed)", ശൂന്യ);
    എങ്കിൽ (lex(&s, &val) == TOK_STRING) {
        തിരികെ strdup(val.string.str);
    } അല്ലെങ്കിൽ {
        fprintf(stderr, "ERROR: %s\n", s.s);
        തിരികെ ശൂന്യ;
    }
}

സ്ഥിതം bool
parse(ഘടന xkb_compose_table *table, ഘടന scanner *s,
      unsigned പൂർണ്ണസംഖ്യ include_depth);

സ്ഥിതം bool
do_include(ഘടന xkb_compose_table *table, ഘടന scanner *s,
           സ്ഥിരം അക്ഷരം *path, unsigned പൂർണ്ണസംഖ്യ include_depth)
{
    FILE *file;
    bool ok;
    അക്ഷരം *string;
    size_t size;
    ഘടന scanner new_s;

    എങ്കിൽ (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        തിരികെ false;
    }

    file = fopen(path, "rb");
    എങ്കിൽ (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        തിരികെ false;
    }

    ok = map_file(file, &string, &size);
    എങ്കിൽ (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        പോകുക err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    എങ്കിൽ (!ok)
        പോകുക err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    fclose(file);
    തിരികെ ok;
}

// NOLINTBEGIN(readability-trivial-switch)
സ്ഥിതം bool
parse(ഘടന xkb_compose_table *table, ഘടന scanner *s,
      unsigned പൂർണ്ണസംഖ്യ include_depth)
{
    എണ്ണൽ rules_token tok;
    സംഘം lvalue val;
    xkb_keysym_t keysym;
    ഘടന production production;
    എണ്ണൽ { MAX_ERRORS = 10 };
    പൂർണ്ണസംഖ്യ അക്കം_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    എങ്കിൽ (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        പോകുക fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    തിരഞ്ഞെടുപ്പ് (tok = lex(s, &val)) {
    അവസ്ഥ TOK_END_OF_വരി:
        പോകുക initial_eol;
    അവസ്ഥ TOK_END_OF_FILE:
        പോകുക finished;
    അവസ്ഥ TOK_INCLUDE:
        പോകുക include;
    സ്ഥിരസ്ഥിതി:
        പോകുക lhs_tok;
    }

include:
    തിരഞ്ഞെടുപ്പ് (tok = lex_include_string(s, table, &val)) {
    അവസ്ഥ TOK_INCLUDE_STRING:
        പോകുക include_eol;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

include_eol:
    തിരഞ്ഞെടുപ്പ് (tok = lex(s, &val)) {
    അവസ്ഥ TOK_END_OF_വരി:
        എങ്കിൽ (!do_include(table, s, val.string.str, include_depth))
            പോകുക fail;
        പോകുക initial;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    തിരഞ്ഞെടുപ്പ് (tok) {
    അവസ്ഥ TOK_COLON:
        എങ്കിൽ (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            പോകുക skip;
        }
        പോകുക rhs;
    അവസ്ഥ TOK_IDENT:
        എങ്കിൽ (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            പോകുക lhs_keysym;
        }
        പോകുക lhs_mod_list_tok;
    അവസ്ഥ TOK_TILDE:
        പോകുക lhs_mod_list_tok;
    അവസ്ഥ TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        പോകുക lhs_mod_list;
    സ്ഥിരസ്ഥിതി:
        പോകുക lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    തിരഞ്ഞെടുപ്പ് (tok) {
    അവസ്ഥ TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        എങ്കിൽ (keysym == XKB_താക്കോൽ_No_ചിഹ്നം) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            പോകുക error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        എങ്കിൽ (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            പോകുക skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        പോകുക lhs;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_index_t mod;

        എങ്കിൽ (tok != TOK_TILDE && tok != TOK_IDENT)
            പോകുക lhs_keysym_tok;

        എങ്കിൽ (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        എങ്കിൽ (tok != TOK_IDENT)
            പോകുക unexpected;

        mod = resolve_modifier(val.string.str);
        എങ്കിൽ (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            പോകുക error;
        }

        production.modmask |= UINT32_C(1) << mod;
        എങ്കിൽ (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        അല്ലെങ്കിൽ
            production.mods |= UINT32_C(1) << mod;

        പോകുക lhs_mod_list;
    }

rhs:
    തിരഞ്ഞെടുപ്പ് (tok = lex(s, &val)) {
    അവസ്ഥ TOK_STRING:
        എങ്കിൽ (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            പോകുക skip;
        }
        എങ്കിൽ (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            പോകുക skip;
        }
        എങ്കിൽ (val.string.len > വലുപ്പം(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         വലുപ്പം(production.string) - 1,
                         val.string.len - 1);
            പോകുക skip;
        }
        memcpy(production.string, val.string.str, val.string.len);
        production.has_string = true;
        പോകുക rhs;
    അവസ്ഥ TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        എങ്കിൽ (keysym == XKB_താക്കോൽ_No_ചിഹ്നം) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            പോകുക error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        എങ്കിൽ (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            പോകുക skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    അവസ്ഥ TOK_END_OF_വരി:
        എങ്കിൽ (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            പോകുക skip;
        }
        add_production(table, s, &production);
        പോകുക initial;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

unexpected:
    എങ്കിൽ (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    അക്കം_errors++;
    എങ്കിൽ (അക്കം_errors <= MAX_ERRORS)
        പോകുക skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    പോകുക fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    തിരികെ false;

skip:
    ആകുമ്പോൾ (tok != TOK_END_OF_വരി && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    പോകുക initial;

finished:
    തിരികെ true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(ഘടന xkb_compose_table *table, സ്ഥിരം അക്ഷരം *string, size_t len,
             സ്ഥിരം അക്ഷരം *file_name)
{
    ഘടന scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, ശൂന്യ);
    എങ്കിൽ (!parse(table, &s, 0))
        തിരികെ false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    തിരികെ true;
}

bool
parse_file(ഘടന xkb_compose_table *table, FILE *file, സ്ഥിരം അക്ഷരം *file_name)
{
    bool ok;
    അക്ഷരം *string;
    size_t size;

    ok = map_file(file, &string, &size);
    എങ്കിൽ (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        തിരികെ false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    തിരികെ ok;
}
