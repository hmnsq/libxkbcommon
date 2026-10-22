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

enum rules_token {
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
union lvalue {
    struct {
        /* Still \0-terminated. */
        const char *str;
        size_t len;
    } string;
};

static enum rules_token
lex(struct scanner *s, union lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    trong_khi (is_space(scanner_peek(s)))
        nếu (scanner_next(s) == '\n')
            trả_về TOK_END_OF_LINE;

    /* Skip comments. */
    nếu (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        goto skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    nếu (scanner_eof(s)) trả_về TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    nếu (scanner_chr(s, '<')) {
        trong_khi (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_next(s));
        nếu (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            trả_về TOK_ERROR;
        }
        nếu (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            trả_về TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        trả_về TOK_LHS_KEYSYM;
    }

    /* Colon. */
    nếu (scanner_chr(s, ':'))
        trả_về TOK_COLON;
    nếu (scanner_chr(s, '!'))
        trả_về TOK_BANG;
    nếu (scanner_chr(s, '~'))
        trả_về TOK_TILDE;

    /* String literal. */
    nếu (scanner_chr(s, '\"')) {
        trong_khi (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            nếu (scanner_chr(s, '\\')) {
                uint8_t o;
                size_t start_pos = s->pos;
                nếu (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                không_thì nếu (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                không_thì nếu (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    nếu (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (char) o);
                    } không_thì {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (số_nguyên) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                không_thì nếu (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (char) o);
                }
                không_thì nếu (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (số_nguyên) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                không_thì {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } không_thì {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        nếu (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            trả_về TOK_ERROR;
        }
        nếu (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            trả_về TOK_ERROR;
        }
        nếu (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            trả_về TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        trả_về TOK_STRING;
    }

    /* Identifier or include. */
    nếu (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        trong_khi (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_next(s));
        nếu (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            trả_về TOK_ERROR;
        }

        nếu (streq(s->buf, "include"))
            trả_về TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        trả_về TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    trả_về TOK_ERROR;
}

static enum rules_token
lex_include_string(struct scanner *s, struct xkb_compose_table *table,
                   union lvalue *val_out)
{
    trong_khi (is_space(scanner_peek(s)))
        nếu (scanner_next(s) == '\n')
            trả_về TOK_END_OF_LINE;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    nếu (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        trả_về TOK_ERROR;
    }

    trong_khi (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        nếu (scanner_chr(s, '%')) {
            nếu (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            không_thì nếu (scanner_chr(s, 'H')) {
                const char *home = xkb_context_getenv(table->ctx, "HOME");
                nếu (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    trả_về TOK_ERROR;
                }
                nếu (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    trả_về TOK_ERROR;
                }
            }
            không_thì nếu (scanner_chr(s, 'L')) {
                char *path = get_locale_compose_file_path(table->ctx, table->locale);
                nếu (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    trả_về TOK_ERROR;
                }
                nếu (!scanner_buf_appends(s, path)) {
                    free(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    trả_về TOK_ERROR;
                }
                free(path);
            }
            không_thì nếu (scanner_chr(s, 'S')) {
                const char *xlocaledir = get_xlocaledir_path(table->ctx);
                nếu (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    trả_về TOK_ERROR;
                }
            }
            không_thì {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                trả_về TOK_ERROR;
            }
        } không_thì {
            scanner_buf_append(s, scanner_next(s));
        }
    }
    nếu (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        trả_về TOK_ERROR;
    }
    nếu (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        trả_về TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    trả_về TOK_INCLUDE_STRING;
}

struct production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    unsigned số_nguyên len;
    xkb_keysym_t keysym;
    char string[XKB_COMPOSE_MAX_STRING_SIZE];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

static void
add_production(struct xkb_compose_table *table, struct scanner *s,
               const struct production *production)
{
    unsigned số_nguyên lhs_pos = 0;
    uint32_t curr = darray_size(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = NULL;
    struct compose_node *node = NULL;

    /* Warn before potentially going over the limit, discard silently after. */
    nếu (darray_size(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    nếu (darray_size(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        trả_về;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    trong_khi (true) {
        const xkb_keysym_t keysym = production->lhs[lhs_pos];
        const bool last = lhs_pos + 1 == production->len;

        nếu (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            struct compose_node new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_size(table->nodes);
            nếu (pptr != NULL) {
                *pptr = curr;
                pptr = NULL;
            }
            darray_append(table->nodes, new);
        }

        node = &darray_item(table->nodes, curr);

        nếu (keysym < node->keysym) {
            pptr = &node->lokid;
            curr = node->lokid;
        } không_thì nếu (keysym > node->keysym) {
            pptr = &node->hikid;
            curr = node->hikid;
        } không_thì nếu (!last) {
            nếu (node->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                node->internal.eqkid = 0;
                node->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &node->internal.eqkid;
            curr = node->internal.eqkid;
        } không_thì {
            nếu (node->is_leaf) {
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
                nếu (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    trả_về;
                } không_thì {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } không_thì nếu (node->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                node->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            nếu (production->has_string) {
                const size_t len = strlen(production->string);
                nếu (node->is_leaf && node->leaf.utf8 &&
                    len <= strlen(&darray_item(table->utf8, node->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    memcpy(&darray_item(table->utf8, node->leaf.utf8),
                           production->string, len + 1);
                } không_thì {
                    /* Cannot reuse string: allocate new one */
                    node->leaf.utf8 = darray_size(table->utf8);
                    darray_append_items(table->utf8, production->string,
                                        (darray_size_t)strlen(production->string) + 1);
                }
            } không_thì {
                /* Ensure we reset possible previous entry */
                node->leaf.utf8 = 0;
            }

            nếu (production->has_keysym) {
                node->leaf.keysym = production->keysym;
            } không_thì {
                /* Ensure we reset possible previous entry */
                node->leaf.keysym = XKB_KEY_NoSymbol;
            }

            node->is_leaf = true;
            trả_về;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

static xkb_mod_index_t
resolve_modifier(const char *name)
{
    static const struct {
        const char *name;
        xkb_mod_index_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    cho (unsigned số_nguyên i = 0; i < ARRAY_SIZE(mods); i++)
        nếu (streq(name, mods[i].name))
            trả_về mods[i].mod;

    trả_về XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
char *
parse_string_literal(struct xkb_context *ctx, const char *string)
{
    struct scanner s;
    union lvalue val;
    scanner_init(&s, ctx, string, strlen(string), "(unnamed)", NULL);
    nếu (lex(&s, &val) == TOK_STRING) {
        trả_về strdup(val.string.str);
    } không_thì {
        fprintf(stderr, "ERROR: %s\n", s.s);
        trả_về NULL;
    }
}

static bool
parse(struct xkb_compose_table *table, struct scanner *s,
      unsigned số_nguyên include_depth);

static bool
do_include(struct xkb_compose_table *table, struct scanner *s,
           const char *path, unsigned số_nguyên include_depth)
{
    FILE *file;
    bool ok;
    char *string;
    size_t size;
    struct scanner new_s;

    nếu (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        trả_về false;
    }

    file = fopen(path, "rb");
    nếu (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        trả_về false;
    }

    ok = map_file(file, &string, &size);
    nếu (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        goto err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    nếu (!ok)
        goto err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    fclose(file);
    trả_về ok;
}

// NOLINTBEGIN(readability-trivial-switch)
static bool
parse(struct xkb_compose_table *table, struct scanner *s,
      unsigned số_nguyên include_depth)
{
    enum rules_token tok;
    union lvalue val;
    xkb_keysym_t keysym;
    struct production production;
    enum { MAX_ERRORS = 10 };
    số_nguyên num_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    nếu (!scanner_check_supported_char_encoding(s)) {
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
    case TOK_END_OF_LINE:
        goto initial_eol;
    case TOK_END_OF_FILE:
        goto finished;
    case TOK_INCLUDE:
        goto include;
    default:
        goto lhs_tok;
    }

include:
    switch (tok = lex_include_string(s, table, &val)) {
    case TOK_INCLUDE_STRING:
        goto include_eol;
    default:
        goto unexpected;
    }

include_eol:
    switch (tok = lex(s, &val)) {
    case TOK_END_OF_LINE:
        nếu (!do_include(table, s, val.string.str, include_depth))
            goto fail;
        goto initial;
    default:
        goto unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    switch (tok) {
    case TOK_COLON:
        nếu (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            goto skip;
        }
        goto rhs;
    case TOK_IDENT:
        nếu (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            goto lhs_keysym;
        }
        goto lhs_mod_list_tok;
    case TOK_TILDE:
        goto lhs_mod_list_tok;
    case TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        goto lhs_mod_list;
    default:
        goto lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    switch (tok) {
    case TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        nếu (keysym == XKB_KEY_NoSymbol) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            goto error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        nếu (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
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

        nếu (tok != TOK_TILDE && tok != TOK_IDENT)
            goto lhs_keysym_tok;

        nếu (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        nếu (tok != TOK_IDENT)
            goto unexpected;

        mod = resolve_modifier(val.string.str);
        nếu (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            goto error;
        }

        production.modmask |= UINT32_C(1) << mod;
        nếu (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        không_thì
            production.mods |= UINT32_C(1) << mod;

        goto lhs_mod_list;
    }

rhs:
    switch (tok = lex(s, &val)) {
    case TOK_STRING:
        nếu (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            goto skip;
        }
        nếu (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            goto skip;
        }
        nếu (val.string.len > sizeof(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         sizeof(production.string) - 1,
                         val.string.len - 1);
            goto skip;
        }
        memcpy(production.string, val.string.str, val.string.len);
        production.has_string = true;
        goto rhs;
    case TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        nếu (keysym == XKB_KEY_NoSymbol) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            goto error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        nếu (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            goto skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    case TOK_END_OF_LINE:
        nếu (!production.has_string && !production.has_keysym) {
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
    nếu (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    num_errors++;
    nếu (num_errors <= MAX_ERRORS)
        goto skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    goto fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    trả_về false;

skip:
    trong_khi (tok != TOK_END_OF_LINE && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    goto initial;

finished:
    trả_về true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(struct xkb_compose_table *table, const char *string, size_t len,
             const char *file_name)
{
    struct scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, NULL);
    nếu (!parse(table, &s, 0))
        trả_về false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    trả_về true;
}

bool
parse_file(struct xkb_compose_table *table, FILE *file, const char *file_name)
{
    bool ok;
    char *string;
    size_t size;

    ok = map_file(file, &string, &size);
    nếu (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        trả_về false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    trả_về ok;
}
