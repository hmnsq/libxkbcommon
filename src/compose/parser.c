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

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <string.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon-keysyms.h"
#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "messages-codes.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "constants.h"
#অন্তর্ভাৱ "table.h"
#অন্তর্ভাৱ "scanner-utils.h"
#অন্তর্ভাৱ "paths.h"
#অন্তর্ভাৱ "utf8.h"
#অন্তর্ভাৱ "parser.h"
#অন্তর্ভাৱ "keysym.h"

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

গণনা rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_সাৰি,
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
সঙ্ঘ lvalue {
    সংরচনা {
        /* Still \0-terminated. */
        ধ্রুৱ ৱর্ণ *str;
        size_t len;
    } string;
};

স্থির গণনা rules_token
lex(সংরচনা scanner *s, সঙ্ঘ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    যেতিয়ালৈ (is_space(scanner_peek(s)))
        যদি (scanner_next(s) == '\n')
            ঘূৰাই TOK_END_OF_সাৰি;

    /* Skip comments. */
    যদি (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        গচ্ছ skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    যদি (scanner_eof(s)) ঘূৰাই TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    যদি (scanner_chr(s, '<')) {
        যেতিয়ালৈ (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_next(s));
        যদি (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            ঘূৰাই TOK_ERROR;
        }
        যদি (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            ঘূৰাই TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ঘূৰাই TOK_LHS_KEYSYM;
    }

    /* Colon. */
    যদি (scanner_chr(s, ':'))
        ঘূৰাই TOK_COLON;
    যদি (scanner_chr(s, '!'))
        ঘূৰাই TOK_BANG;
    যদি (scanner_chr(s, '~'))
        ঘূৰাই TOK_TILDE;

    /* String literal. */
    যদি (scanner_chr(s, '\"')) {
        যেতিয়ালৈ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            যদি (scanner_chr(s, '\\')) {
                uint8_t o;
                size_t start_pos = s->pos;
                যদি (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                অন্যথা যদি (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                অন্যথা যদি (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    যদি (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (ৱর্ণ) o);
                    } অন্যথা {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (পূর্ণাঙ্ক) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                অন্যথা যদি (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (ৱর্ণ) o);
                }
                অন্যথা যদি (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (পূর্ণাঙ্ক) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                অন্যথা {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } অন্যথা {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        যদি (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            ঘূৰাই TOK_ERROR;
        }
        যদি (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            ঘূৰাই TOK_ERROR;
        }
        যদি (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            ঘূৰাই TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ঘূৰাই TOK_STRING;
    }

    /* Identifier or include. */
    যদি (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        যেতিয়ালৈ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_next(s));
        যদি (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            ঘূৰাই TOK_ERROR;
        }

        যদি (streq(s->buf, "include"))
            ঘূৰাই TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ঘূৰাই TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    ঘূৰাই TOK_ERROR;
}

স্থির গণনা rules_token
lex_include_string(সংরচনা scanner *s, সংরচনা xkb_compose_table *table,
                   সঙ্ঘ lvalue *val_out)
{
    যেতিয়ালৈ (is_space(scanner_peek(s)))
        যদি (scanner_next(s) == '\n')
            ঘূৰাই TOK_END_OF_সাৰি;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    যদি (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        ঘূৰাই TOK_ERROR;
    }

    যেতিয়ালৈ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        যদি (scanner_chr(s, '%')) {
            যদি (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            অন্যথা যদি (scanner_chr(s, 'H')) {
                ধ্রুৱ ৱর্ণ *home = xkb_context_getenv(table->ctx, "HOME");
                যদি (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    ঘূৰাই TOK_ERROR;
                }
                যদি (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    ঘূৰাই TOK_ERROR;
                }
            }
            অন্যথা যদি (scanner_chr(s, 'L')) {
                ৱর্ণ *path = get_locale_compose_file_path(table->ctx, table->locale);
                যদি (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    ঘূৰাই TOK_ERROR;
                }
                যদি (!scanner_buf_appends(s, path)) {
                    মুক্ত(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    ঘূৰাই TOK_ERROR;
                }
                মুক্ত(path);
            }
            অন্যথা যদি (scanner_chr(s, 'S')) {
                ধ্রুৱ ৱর্ণ *xlocaledir = get_xlocaledir_path(table->ctx);
                যদি (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    ঘূৰাই TOK_ERROR;
                }
            }
            অন্যথা {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                ঘূৰাই TOK_ERROR;
            }
        } অন্যথা {
            scanner_buf_append(s, scanner_next(s));
        }
    }
    যদি (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        ঘূৰাই TOK_ERROR;
    }
    যদি (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        ঘূৰাই TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    ঘূৰাই TOK_INCLUDE_STRING;
}

সংরচনা production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    unsigned পূর্ণাঙ্ক len;
    xkb_keysym_t keysym;
    ৱর্ণ string[XKB_COMPOSE_MAX_STRING_SIZE];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

স্থির শূন্য
add_production(সংরচনা xkb_compose_table *table, সংরচনা scanner *s,
               ধ্রুৱ সংরচনা production *production)
{
    unsigned পূর্ণাঙ্ক lhs_pos = 0;
    uint32_t curr = darray_size(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = রিক্ত;
    সংরচনা compose_গ্ৰন্থি *গ্ৰন্থি = রিক্ত;

    /* Warn before potentially going over the limit, discard silently after. */
    যদি (darray_size(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    যদি (darray_size(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        ঘূৰাই;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    যেতিয়ালৈ (true) {
        ধ্রুৱ xkb_keysym_t keysym = production->lhs[lhs_pos];
        ধ্রুৱ bool last = lhs_pos + 1 == production->len;

        যদি (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            সংরচনা compose_গ্ৰন্থি new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_size(table->nodes);
            যদি (pptr != রিক্ত) {
                *pptr = curr;
                pptr = রিক্ত;
            }
            darray_append(table->nodes, new);
        }

        গ্ৰন্থি = &darray_item(table->nodes, curr);

        যদি (keysym < গ্ৰন্থি->keysym) {
            pptr = &গ্ৰন্থি->lokid;
            curr = গ্ৰন্থি->lokid;
        } অন্যথা যদি (keysym > গ্ৰন্থি->keysym) {
            pptr = &গ্ৰন্থি->hikid;
            curr = গ্ৰন্থি->hikid;
        } অন্যথা যদি (!last) {
            যদি (গ্ৰন্থি->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                গ্ৰন্থি->internal.eqkid = 0;
                গ্ৰন্থি->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &গ্ৰন্থি->internal.eqkid;
            curr = গ্ৰন্থি->internal.eqkid;
        } অন্যথা {
            যদি (গ্ৰন্থি->is_leaf) {
                bool same_string =
                    (গ্ৰন্থি->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        গ্ৰন্থি->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, গ্ৰন্থি->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (গ্ৰন্থি->leaf.keysym == XKB_চাবি_No_প্ৰতীক && !production->has_keysym) ||
                    (
                        গ্ৰন্থি->leaf.keysym != XKB_চাবি_No_প্ৰতীক && production->has_keysym &&
                        গ্ৰন্থি->leaf.keysym == production->keysym
                    );
                যদি (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    ঘূৰাই;
                } অন্যথা {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } অন্যথা যদি (গ্ৰন্থি->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                গ্ৰন্থি->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            যদি (production->has_string) {
                ধ্রুৱ size_t len = strlen(production->string);
                যদি (গ্ৰন্থি->is_leaf && গ্ৰন্থি->leaf.utf8 &&
                    len <= strlen(&darray_item(table->utf8, গ্ৰন্থি->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    memcpy(&darray_item(table->utf8, গ্ৰন্থি->leaf.utf8),
                           production->string, len + 1);
                } অন্যথা {
                    /* Cannot reuse string: allocate new one */
                    গ্ৰন্থি->leaf.utf8 = darray_size(table->utf8);
                    darray_append_বস্তুসমূহ(table->utf8, production->string,
                                        (darray_size_t)strlen(production->string) + 1);
                }
            } অন্যথা {
                /* Ensure we reset possible previous entry */
                গ্ৰন্থি->leaf.utf8 = 0;
            }

            যদি (production->has_keysym) {
                গ্ৰন্থি->leaf.keysym = production->keysym;
            } অন্যথা {
                /* Ensure we reset possible previous entry */
                গ্ৰন্থি->leaf.keysym = XKB_চাবি_No_প্ৰতীক;
            }

            গ্ৰন্থি->is_leaf = true;
            ঘূৰাই;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

স্থির xkb_mod_index_t
resolve_modifier(ধ্রুৱ ৱর্ণ *name)
{
    স্থির ধ্রুৱ সংরচনা {
        ধ্রুৱ ৱর্ণ *name;
        xkb_mod_index_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < ARRAY_SIZE(mods); i++)
        যদি (streq(name, mods[i].name))
            ঘূৰাই mods[i].mod;

    ঘূৰাই XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
ৱর্ণ *
parse_string_literal(সংরচনা xkb_context *ctx, ধ্রুৱ ৱর্ণ *string)
{
    সংরচনা scanner s;
    সঙ্ঘ lvalue val;
    scanner_init(&s, ctx, string, strlen(string), "(unnamed)", রিক্ত);
    যদি (lex(&s, &val) == TOK_STRING) {
        ঘূৰাই strdup(val.string.str);
    } অন্যথা {
        fprintf(stderr, "ERROR: %s\n", s.s);
        ঘূৰাই রিক্ত;
    }
}

স্থির bool
parse(সংরচনা xkb_compose_table *table, সংরচনা scanner *s,
      unsigned পূর্ণাঙ্ক include_depth);

স্থির bool
do_include(সংরচনা xkb_compose_table *table, সংরচনা scanner *s,
           ধ্রুৱ ৱর্ণ *path, unsigned পূর্ণাঙ্ক include_depth)
{
    FILE *file;
    bool ok;
    ৱর্ণ *string;
    size_t size;
    সংরচনা scanner new_s;

    যদি (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        ঘূৰাই false;
    }

    file = fopen(path, "rb");
    যদি (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        ঘূৰাই false;
    }

    ok = map_file(file, &string, &size);
    যদি (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        গচ্ছ err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    যদি (!ok)
        গচ্ছ err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    fclose(file);
    ঘূৰাই ok;
}

// NOLINTBEGIN(readability-trivial-switch)
স্থির bool
parse(সংরচনা xkb_compose_table *table, সংরচনা scanner *s,
      unsigned পূর্ণাঙ্ক include_depth)
{
    গণনা rules_token tok;
    সঙ্ঘ lvalue val;
    xkb_keysym_t keysym;
    সংরচনা production production;
    গণনা { MAX_ERRORS = 10 };
    পূর্ণাঙ্ক অঙ্ক_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    যদি (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        গচ্ছ fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    ৱিকল্প (tok = lex(s, &val)) {
    পক্ষ TOK_END_OF_সাৰি:
        গচ্ছ initial_eol;
    পক্ষ TOK_END_OF_FILE:
        গচ্ছ finished;
    পক্ষ TOK_INCLUDE:
        গচ্ছ include;
    স্ৱভাৱ:
        গচ্ছ lhs_tok;
    }

include:
    ৱিকল্প (tok = lex_include_string(s, table, &val)) {
    পক্ষ TOK_INCLUDE_STRING:
        গচ্ছ include_eol;
    স্ৱভাৱ:
        গচ্ছ unexpected;
    }

include_eol:
    ৱিকল্প (tok = lex(s, &val)) {
    পক্ষ TOK_END_OF_সাৰি:
        যদি (!do_include(table, s, val.string.str, include_depth))
            গচ্ছ fail;
        গচ্ছ initial;
    স্ৱভাৱ:
        গচ্ছ unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    ৱিকল্প (tok) {
    পক্ষ TOK_COLON:
        যদি (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            গচ্ছ skip;
        }
        গচ্ছ rhs;
    পক্ষ TOK_IDENT:
        যদি (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            গচ্ছ lhs_keysym;
        }
        গচ্ছ lhs_mod_list_tok;
    পক্ষ TOK_TILDE:
        গচ্ছ lhs_mod_list_tok;
    পক্ষ TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        গচ্ছ lhs_mod_list;
    স্ৱভাৱ:
        গচ্ছ lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    ৱিকল্প (tok) {
    পক্ষ TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        যদি (keysym == XKB_চাবি_No_প্ৰতীক) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            গচ্ছ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        যদি (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            গচ্ছ skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        গচ্ছ lhs;
    স্ৱভাৱ:
        গচ্ছ unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_index_t mod;

        যদি (tok != TOK_TILDE && tok != TOK_IDENT)
            গচ্ছ lhs_keysym_tok;

        যদি (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        যদি (tok != TOK_IDENT)
            গচ্ছ unexpected;

        mod = resolve_modifier(val.string.str);
        যদি (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            গচ্ছ error;
        }

        production.modmask |= UINT32_C(1) << mod;
        যদি (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        অন্যথা
            production.mods |= UINT32_C(1) << mod;

        গচ্ছ lhs_mod_list;
    }

rhs:
    ৱিকল্প (tok = lex(s, &val)) {
    পক্ষ TOK_STRING:
        যদি (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            গচ্ছ skip;
        }
        যদি (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            গচ্ছ skip;
        }
        যদি (val.string.len > পরিমাণ(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         পরিমাণ(production.string) - 1,
                         val.string.len - 1);
            গচ্ছ skip;
        }
        memcpy(production.string, val.string.str, val.string.len);
        production.has_string = true;
        গচ্ছ rhs;
    পক্ষ TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        যদি (keysym == XKB_চাবি_No_প্ৰতীক) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            গচ্ছ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        যদি (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            গচ্ছ skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    পক্ষ TOK_END_OF_সাৰি:
        যদি (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            গচ্ছ skip;
        }
        add_production(table, s, &production);
        গচ্ছ initial;
    স্ৱভাৱ:
        গচ্ছ unexpected;
    }

unexpected:
    যদি (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    অঙ্ক_errors++;
    যদি (অঙ্ক_errors <= MAX_ERRORS)
        গচ্ছ skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    গচ্ছ fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    ঘূৰাই false;

skip:
    যেতিয়ালৈ (tok != TOK_END_OF_সাৰি && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    গচ্ছ initial;

finished:
    ঘূৰাই true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(সংরচনা xkb_compose_table *table, ধ্রুৱ ৱর্ণ *string, size_t len,
             ধ্রুৱ ৱর্ণ *file_name)
{
    সংরচনা scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, রিক্ত);
    যদি (!parse(table, &s, 0))
        ঘূৰাই false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    ঘূৰাই true;
}

bool
parse_file(সংরচনা xkb_compose_table *table, FILE *file, ধ্রুৱ ৱর্ণ *file_name)
{
    bool ok;
    ৱর্ণ *string;
    size_t size;

    ok = map_file(file, &string, &size);
    যদি (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        ঘূৰাই false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    ঘূৰাই ok;
}
