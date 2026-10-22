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

#include "config.h"

#include <errno.h>
#include <string.h>

#include "xkbcommon/xkbcommon-keysyms.h"
#include "darray.h"
#include "messages-codes.h"
#include "utils.h"
#include "constants.h"
#include "table.h"
#include "scanner-utils.h"
#include "paths.h"
#include "utf8.h"
#include "parser.h"
#include "keysym.h"

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

ga_nanaa rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_LINE,
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
sa_ngha lvalue {
    sa_nracanaa {
        /* Still \0-terminated. */
        const var_na *str;
        size_t len;
    } string;
};

static ga_nanaa rules_token
lex(sa_nracanaa scanner *s, sa_ngha lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    ᤀᤠ᤺ (is_space(scanner_peek(s)))
        ᤀᤡ᤺ (scanner_next(s) == '\n')
            ᤀᤡᤴ TOK_END_OF_LINE;

    /* Skip comments. */
    ᤀᤡ᤺ (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    ᤀᤡ᤺ (scanner_eof(s)) ᤀᤡᤴ TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    ᤀᤡ᤺ (scanner_chr(s, '<')) {
        ᤀᤠ᤺ (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_next(s));
        ᤀᤡ᤺ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            ᤀᤡᤴ TOK_ERROR;
        }
        ᤀᤡ᤺ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            ᤀᤡᤴ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ᤀᤡᤴ TOK_LHS_KEYSYM;
    }

    /* Colon. */
    ᤀᤡ᤺ (scanner_chr(s, ':'))
        ᤀᤡᤴ TOK_COLON;
    ᤀᤡ᤺ (scanner_chr(s, '!'))
        ᤀᤡᤴ TOK_BANG;
    ᤀᤡ᤺ (scanner_chr(s, '~'))
        ᤀᤡᤴ TOK_TILDE;

    /* String literal. */
    ᤀᤡ᤺ (scanner_chr(s, '\"')) {
        ᤀᤠ᤺ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ᤀᤡ᤺ (scanner_chr(s, '\\')) {
                uint8_t o;
                size_t start_pos = s->pos;
                ᤀᤡ᤺ (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    ᤀᤡ᤺ (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (var_na) o);
                    } ᤀᤡ᤺ᤆᤡ {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (puur_naa_nka) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (var_na) o);
                }
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (puur_naa_nka) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                ᤀᤡ᤺ᤆᤡ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ᤀᤡ᤺ᤆᤡ {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        ᤀᤡ᤺ (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            ᤀᤡᤴ TOK_ERROR;
        }
        ᤀᤡ᤺ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            ᤀᤡᤴ TOK_ERROR;
        }
        ᤀᤡ᤺ (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            ᤀᤡᤴ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ᤀᤡᤴ TOK_STRING;
    }

    /* Identifier or include. */
    ᤀᤡ᤺ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        ᤀᤠ᤺ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_next(s));
        ᤀᤡ᤺ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            ᤀᤡᤴ TOK_ERROR;
        }

        ᤀᤡ᤺ (streq(s->buf, "include"))
            ᤀᤡᤴ TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ᤀᤡᤴ TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    ᤀᤡᤴ TOK_ERROR;
}

static ga_nanaa rules_token
lex_include_string(sa_nracanaa scanner *s, sa_nracanaa xkb_compose_table *table,
                   sa_ngha lvalue *val_out)
{
    ᤀᤠ᤺ (is_space(scanner_peek(s)))
        ᤀᤡ᤺ (scanner_next(s) == '\n')
            ᤀᤡᤴ TOK_END_OF_LINE;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    ᤀᤡ᤺ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        ᤀᤡᤴ TOK_ERROR;
    }

    ᤀᤠ᤺ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        ᤀᤡ᤺ (scanner_chr(s, '%')) {
            ᤀᤡ᤺ (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (scanner_chr(s, 'H')) {
                const var_na *home = xkb_context_getenv(table->ctx, "HOME");
                ᤀᤡ᤺ (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    ᤀᤡᤴ TOK_ERROR;
                }
                ᤀᤡ᤺ (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    ᤀᤡᤴ TOK_ERROR;
                }
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (scanner_chr(s, 'L')) {
                var_na *path = get_locale_compose_file_path(table->ctx, table->locale);
                ᤀᤡ᤺ (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    ᤀᤡᤴ TOK_ERROR;
                }
                ᤀᤡ᤺ (!scanner_buf_appends(s, path)) {
                    free(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    ᤀᤡᤴ TOK_ERROR;
                }
                free(path);
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (scanner_chr(s, 'S')) {
                const var_na *xlocaledir = get_xlocaledir_path(table->ctx);
                ᤀᤡ᤺ (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    ᤀᤡᤴ TOK_ERROR;
                }
            }
            ᤀᤡ᤺ᤆᤡ {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                ᤀᤡᤴ TOK_ERROR;
            }
        } ᤀᤡ᤺ᤆᤡ {
            scanner_buf_append(s, scanner_next(s));
        }
    }
    ᤀᤡ᤺ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        ᤀᤡᤴ TOK_ERROR;
    }
    ᤀᤡ᤺ (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        ᤀᤡᤴ TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    ᤀᤡᤴ TOK_INCLUDE_STRING;
}

sa_nracanaa production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    unsigned puur_naa_nka len;
    xkb_keysym_t keysym;
    var_na string[XKB_COMPOSE_MAX_STRING_SIZE];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

static void
add_production(sa_nracanaa xkb_compose_table *table, sa_nracanaa scanner *s,
               const sa_nracanaa production *production)
{
    unsigned puur_naa_nka lhs_pos = 0;
    uint32_t curr = darray_size(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = NULL;
    sa_nracanaa compose_node *node = NULL;

    /* Warn before potentially going over the limit, discard silently after. */
    ᤀᤡ᤺ (darray_size(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    ᤀᤡ᤺ (darray_size(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        ᤀᤡᤴ;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    ᤀᤠ᤺ (true) {
        const xkb_keysym_t keysym = production->lhs[lhs_pos];
        const bool last = lhs_pos + 1 == production->len;

        ᤀᤡ᤺ (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            sa_nracanaa compose_node new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_size(table->nodes);
            ᤀᤡ᤺ (pptr != NULL) {
                *pptr = curr;
                pptr = NULL;
            }
            darray_append(table->nodes, new);
        }

        node = &darray_item(table->nodes, curr);

        ᤀᤡ᤺ (keysym < node->keysym) {
            pptr = &node->lokid;
            curr = node->lokid;
        } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (keysym > node->keysym) {
            pptr = &node->hikid;
            curr = node->hikid;
        } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (!last) {
            ᤀᤡ᤺ (node->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                node->internal.eqkid = 0;
                node->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &node->internal.eqkid;
            curr = node->internal.eqkid;
        } ᤀᤡ᤺ᤆᤡ {
            ᤀᤡ᤺ (node->is_leaf) {
                bool same_string =
                    (node->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        node->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, node->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (node->leaf.keysym == XKB_KEY_NoSymbol && !production->has_keysym) ||
                    (
                        node->leaf.keysym != XKB_KEY_NoSymbol && production->has_keysym &&
                        node->leaf.keysym == production->keysym
                    );
                ᤀᤡ᤺ (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    ᤀᤡᤴ;
                } ᤀᤡ᤺ᤆᤡ {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (node->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                node->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            ᤀᤡ᤺ (production->has_string) {
                const size_t len = strlen(production->string);
                ᤀᤡ᤺ (node->is_leaf && node->leaf.utf8 &&
                    len <= strlen(&darray_item(table->utf8, node->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    memcpy(&darray_item(table->utf8, node->leaf.utf8),
                           production->string, len + 1);
                } ᤀᤡ᤺ᤆᤡ {
                    /* Cannot reuse string: allocate new one */
                    node->leaf.utf8 = darray_size(table->utf8);
                    darray_append_items(table->utf8, production->string,
                                        (darray_size_t)strlen(production->string) + 1);
                }
            } ᤀᤡ᤺ᤆᤡ {
                /* Ensure we reset possible previous entry */
                node->leaf.utf8 = 0;
            }

            ᤀᤡ᤺ (production->has_keysym) {
                node->leaf.keysym = production->keysym;
            } ᤀᤡ᤺ᤆᤡ {
                /* Ensure we reset possible previous entry */
                node->leaf.keysym = XKB_KEY_NoSymbol;
            }

            node->is_leaf = true;
            ᤀᤡᤴ;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

static xkb_mod_index_t
resolve_modifier(const var_na *name)
{
    static const sa_nracanaa {
        const var_na *name;
        xkb_mod_index_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    for (unsigned puur_naa_nka i = 0; i < ARRAY_SIZE(mods); i++)
        ᤀᤡ᤺ (streq(name, mods[i].name))
            ᤀᤡᤴ mods[i].mod;

    ᤀᤡᤴ XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
var_na *
parse_string_literal(sa_nracanaa xkb_context *ctx, const var_na *string)
{
    sa_nracanaa scanner s;
    sa_ngha lvalue val;
    scanner_init(&s, ctx, string, strlen(string), "(unnamed)", NULL);
    ᤀᤡ᤺ (lex(&s, &val) == TOK_STRING) {
        ᤀᤡᤴ strdup(val.string.str);
    } ᤀᤡ᤺ᤆᤡ {
        fprintf(stderr, "ERROR: %s\n", s.s);
        ᤀᤡᤴ NULL;
    }
}

static bool
parse(sa_nracanaa xkb_compose_table *table, sa_nracanaa scanner *s,
      unsigned puur_naa_nka include_depth);

static bool
do_include(sa_nracanaa xkb_compose_table *table, sa_nracanaa scanner *s,
           const var_na *path, unsigned puur_naa_nka include_depth)
{
    FILE *file;
    bool ok;
    var_na *string;
    size_t size;
    sa_nracanaa scanner new_s;

    ᤀᤡ᤺ (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        ᤀᤡᤴ false;
    }

    file = fopen(path, "rb");
    ᤀᤡ᤺ (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        ᤀᤡᤴ false;
    }

    ok = map_file(file, &string, &size);
    ᤀᤡ᤺ (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        goto err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    ᤀᤡ᤺ (!ok)
        goto err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    fclose(file);
    ᤀᤡᤴ ok;
}

// NOLINTBEGIN(readability-trivial-switch)
static bool
parse(sa_nracanaa xkb_compose_table *table, sa_nracanaa scanner *s,
      unsigned puur_naa_nka include_depth)
{
    ga_nanaa rules_token tok;
    sa_ngha lvalue val;
    xkb_keysym_t keysym;
    sa_nracanaa production production;
    ga_nanaa { MAX_ERRORS = 10 };
    puur_naa_nka num_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    ᤀᤡ᤺ (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        goto fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    switch (tok = lex(s, &val)) {
    pak_sa TOK_END_OF_LINE:
        goto initial_eol;
    pak_sa TOK_END_OF_FILE:
        goto finished;
    pak_sa TOK_INCLUDE:
        goto include;
    default:
        goto lhs_tok;
    }

include:
    switch (tok = lex_include_string(s, table, &val)) {
    pak_sa TOK_INCLUDE_STRING:
        goto include_eol;
    default:
        goto unexpected;
    }

include_eol:
    switch (tok = lex(s, &val)) {
    pak_sa TOK_END_OF_LINE:
        ᤀᤡ᤺ (!do_include(table, s, val.string.str, include_depth))
            goto fail;
        goto initial;
    default:
        goto unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    switch (tok) {
    pak_sa TOK_COLON:
        ᤀᤡ᤺ (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            goto skip;
        }
        goto rhs;
    pak_sa TOK_IDENT:
        ᤀᤡ᤺ (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            goto lhs_keysym;
        }
        goto lhs_mod_list_tok;
    pak_sa TOK_TILDE:
        goto lhs_mod_list_tok;
    pak_sa TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        goto lhs_mod_list;
    default:
        goto lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    switch (tok) {
    pak_sa TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ᤀᤡ᤺ (keysym == XKB_KEY_NoSymbol) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            goto error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ᤀᤡ᤺ (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            goto skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        goto lhs;
    default:
        goto unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_index_t mod;

        ᤀᤡ᤺ (tok != TOK_TILDE && tok != TOK_IDENT)
            goto lhs_keysym_tok;

        ᤀᤡ᤺ (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        ᤀᤡ᤺ (tok != TOK_IDENT)
            goto unexpected;

        mod = resolve_modifier(val.string.str);
        ᤀᤡ᤺ (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            goto error;
        }

        production.modmask |= UINT32_C(1) << mod;
        ᤀᤡ᤺ (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        ᤀᤡ᤺ᤆᤡ
            production.mods |= UINT32_C(1) << mod;

        goto lhs_mod_list;
    }

rhs:
    switch (tok = lex(s, &val)) {
    pak_sa TOK_STRING:
        ᤀᤡ᤺ (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            goto skip;
        }
        ᤀᤡ᤺ (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            goto skip;
        }
        ᤀᤡ᤺ (val.string.len > parimaa_na(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         parimaa_na(production.string) - 1,
                         val.string.len - 1);
            goto skip;
        }
        memcpy(production.string, val.string.str, val.string.len);
        production.has_string = true;
        goto rhs;
    pak_sa TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ᤀᤡ᤺ (keysym == XKB_KEY_NoSymbol) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            goto error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ᤀᤡ᤺ (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            goto skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    pak_sa TOK_END_OF_LINE:
        ᤀᤡ᤺ (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            goto skip;
        }
        add_production(table, s, &production);
        goto initial;
    default:
        goto unexpected;
    }

unexpected:
    ᤀᤡ᤺ (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    num_errors++;
    ᤀᤡ᤺ (num_errors <= MAX_ERRORS)
        goto skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    goto fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    ᤀᤡᤴ false;

skip:
    ᤀᤠ᤺ (tok != TOK_END_OF_LINE && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    goto initial;

finished:
    ᤀᤡᤴ true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(sa_nracanaa xkb_compose_table *table, const var_na *string, size_t len,
             const var_na *file_name)
{
    sa_nracanaa scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, NULL);
    ᤀᤡ᤺ (!parse(table, &s, 0))
        ᤀᤡᤴ false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    ᤀᤡᤴ true;
}

bool
parse_file(sa_nracanaa xkb_compose_table *table, FILE *file, const var_na *file_name)
{
    bool ok;
    var_na *string;
    size_t size;

    ok = map_file(file, &string, &size);
    ᤀᤡ᤺ (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        ᤀᤡᤴ false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    ᤀᤡᤴ ok;
}
