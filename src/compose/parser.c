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

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon-keysyms.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "constants.h"
#અન્તર્ભાવ "table.h"
#અન્તર્ભાવ "scanner-utils.h"
#અન્તર્ભાવ "paths.h"
#અન્તર્ભાવ "utf8.h"
#અન્તર્ભાવ "parser.h"
#અન્તર્ભાવ "keysym.h"

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

ગણના rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_લીટી,
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
સંઘ lvalue {
    સંરચના {
        /* Still \0-terminated. */
        અચલ અક્ષર *str;
        size_t len;
    } string;
};

સ્થિર ગણના rules_token
lex(સંરચના scanner *s, સંઘ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    જ્યારે (is_space(scanner_peek(s)))
        જો (scanner_next(s) == '\n')
            પાછા TOK_END_OF_લીટી;

    /* Skip comments. */
    જો (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        જાઓ skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    જો (scanner_eof(s)) પાછા TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    જો (scanner_chr(s, '<')) {
        જ્યારે (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_next(s));
        જો (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            પાછા TOK_ERROR;
        }
        જો (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            પાછા TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        પાછા TOK_LHS_KEYSYM;
    }

    /* Colon. */
    જો (scanner_chr(s, ':'))
        પાછા TOK_COLON;
    જો (scanner_chr(s, '!'))
        પાછા TOK_BANG;
    જો (scanner_chr(s, '~'))
        પાછા TOK_TILDE;

    /* String literal. */
    જો (scanner_chr(s, '\"')) {
        જ્યારે (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            જો (scanner_chr(s, '\\')) {
                uint8_t o;
                size_t start_pos = s->pos;
                જો (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                નહીંતર જો (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                નહીંતર જો (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    જો (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (અક્ષર) o);
                    } નહીંતર {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (પૂર્ણાંક) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                નહીંતર જો (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (અક્ષર) o);
                }
                નહીંતર જો (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (પૂર્ણાંક) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                નહીંતર {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } નહીંતર {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        જો (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            પાછા TOK_ERROR;
        }
        જો (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            પાછા TOK_ERROR;
        }
        જો (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            પાછા TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        પાછા TOK_STRING;
    }

    /* Identifier or include. */
    જો (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        જ્યારે (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_next(s));
        જો (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            પાછા TOK_ERROR;
        }

        જો (streq(s->buf, "include"))
            પાછા TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        પાછા TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    પાછા TOK_ERROR;
}

સ્થિર ગણના rules_token
lex_include_string(સંરચના scanner *s, સંરચના xkb_compose_table *table,
                   સંઘ lvalue *val_out)
{
    જ્યારે (is_space(scanner_peek(s)))
        જો (scanner_next(s) == '\n')
            પાછા TOK_END_OF_લીટી;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    જો (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        પાછા TOK_ERROR;
    }

    જ્યારે (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        જો (scanner_chr(s, '%')) {
            જો (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            નહીંતર જો (scanner_chr(s, 'H')) {
                અચલ અક્ષર *home = xkb_context_getenv(table->ctx, "HOME");
                જો (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    પાછા TOK_ERROR;
                }
                જો (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    પાછા TOK_ERROR;
                }
            }
            નહીંતર જો (scanner_chr(s, 'L')) {
                અક્ષર *path = get_locale_compose_file_path(table->ctx, table->locale);
                જો (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    પાછા TOK_ERROR;
                }
                જો (!scanner_buf_appends(s, path)) {
                    મુક્ત(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    પાછા TOK_ERROR;
                }
                મુક્ત(path);
            }
            નહીંતર જો (scanner_chr(s, 'S')) {
                અચલ અક્ષર *xlocaledir = get_xlocaledir_path(table->ctx);
                જો (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    પાછા TOK_ERROR;
                }
            }
            નહીંતર {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                પાછા TOK_ERROR;
            }
        } નહીંતર {
            scanner_buf_append(s, scanner_next(s));
        }
    }
    જો (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        પાછા TOK_ERROR;
    }
    જો (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        પાછા TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    પાછા TOK_INCLUDE_STRING;
}

સંરચના production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    unsigned પૂર્ણાંક len;
    xkb_keysym_t keysym;
    અક્ષર string[XKB_COMPOSE_MAX_STRING_SIZE];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

સ્થિર શૂન્ય
add_production(સંરચના xkb_compose_table *table, સંરચના scanner *s,
               અચલ સંરચના production *production)
{
    unsigned પૂર્ણાંક lhs_pos = 0;
    uint32_t curr = darray_size(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = ખાલી;
    સંરચના compose_ગાંઠ *ગાંઠ = ખાલી;

    /* Warn before potentially going over the limit, discard silently after. */
    જો (darray_size(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    જો (darray_size(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        પાછા;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    જ્યારે (true) {
        અચલ xkb_keysym_t keysym = production->lhs[lhs_pos];
        અચલ bool last = lhs_pos + 1 == production->len;

        જો (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            સંરચના compose_ગાંઠ new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_size(table->nodes);
            જો (pptr != ખાલી) {
                *pptr = curr;
                pptr = ખાલી;
            }
            darray_append(table->nodes, new);
        }

        ગાંઠ = &darray_item(table->nodes, curr);

        જો (keysym < ગાંઠ->keysym) {
            pptr = &ગાંઠ->lokid;
            curr = ગાંઠ->lokid;
        } નહીંતર જો (keysym > ગાંઠ->keysym) {
            pptr = &ગાંઠ->hikid;
            curr = ગાંઠ->hikid;
        } નહીંતર જો (!last) {
            જો (ગાંઠ->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                ગાંઠ->internal.eqkid = 0;
                ગાંઠ->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &ગાંઠ->internal.eqkid;
            curr = ગાંઠ->internal.eqkid;
        } નહીંતર {
            જો (ગાંઠ->is_leaf) {
                bool same_string =
                    (ગાંઠ->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        ગાંઠ->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, ગાંઠ->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (ગાંઠ->leaf.keysym == XKB_ચાવી_No_પ્રતીક && !production->has_keysym) ||
                    (
                        ગાંઠ->leaf.keysym != XKB_ચાવી_No_પ્રતીક && production->has_keysym &&
                        ગાંઠ->leaf.keysym == production->keysym
                    );
                જો (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    પાછા;
                } નહીંતર {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } નહીંતર જો (ગાંઠ->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                ગાંઠ->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            જો (production->has_string) {
                અચલ size_t len = strlen(production->string);
                જો (ગાંઠ->is_leaf && ગાંઠ->leaf.utf8 &&
                    len <= strlen(&darray_item(table->utf8, ગાંઠ->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    memcpy(&darray_item(table->utf8, ગાંઠ->leaf.utf8),
                           production->string, len + 1);
                } નહીંતર {
                    /* Cannot reuse string: allocate new one */
                    ગાંઠ->leaf.utf8 = darray_size(table->utf8);
                    darray_append_વસ્તુઓ(table->utf8, production->string,
                                        (darray_size_t)strlen(production->string) + 1);
                }
            } નહીંતર {
                /* Ensure we reset possible previous entry */
                ગાંઠ->leaf.utf8 = 0;
            }

            જો (production->has_keysym) {
                ગાંઠ->leaf.keysym = production->keysym;
            } નહીંતર {
                /* Ensure we reset possible previous entry */
                ગાંઠ->leaf.keysym = XKB_ચાવી_No_પ્રતીક;
            }

            ગાંઠ->is_leaf = true;
            પાછા;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

સ્થિર xkb_mod_index_t
resolve_modifier(અચલ અક્ષર *name)
{
    સ્થિર અચલ સંરચના {
        અચલ અક્ષર *name;
        xkb_mod_index_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    માટે (unsigned પૂર્ણાંક i = 0; i < ARRAY_SIZE(mods); i++)
        જો (streq(name, mods[i].name))
            પાછા mods[i].mod;

    પાછા XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
અક્ષર *
parse_string_literal(સંરચના xkb_context *ctx, અચલ અક્ષર *string)
{
    સંરચના scanner s;
    સંઘ lvalue val;
    scanner_init(&s, ctx, string, strlen(string), "(unnamed)", ખાલી);
    જો (lex(&s, &val) == TOK_STRING) {
        પાછા strdup(val.string.str);
    } નહીંતર {
        fprintf(stderr, "ERROR: %s\n", s.s);
        પાછા ખાલી;
    }
}

સ્થિર bool
parse(સંરચના xkb_compose_table *table, સંરચના scanner *s,
      unsigned પૂર્ણાંક include_depth);

સ્થિર bool
do_include(સંરચના xkb_compose_table *table, સંરચના scanner *s,
           અચલ અક્ષર *path, unsigned પૂર્ણાંક include_depth)
{
    FILE *file;
    bool ok;
    અક્ષર *string;
    size_t size;
    સંરચના scanner new_s;

    જો (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        પાછા false;
    }

    file = fopen(path, "rb");
    જો (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        પાછા false;
    }

    ok = map_file(file, &string, &size);
    જો (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        જાઓ err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    જો (!ok)
        જાઓ err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    fclose(file);
    પાછા ok;
}

// NOLINTBEGIN(readability-trivial-switch)
સ્થિર bool
parse(સંરચના xkb_compose_table *table, સંરચના scanner *s,
      unsigned પૂર્ણાંક include_depth)
{
    ગણના rules_token tok;
    સંઘ lvalue val;
    xkb_keysym_t keysym;
    સંરચના production production;
    ગણના { MAX_ERRORS = 10 };
    પૂર્ણાંક અંક_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    જો (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        જાઓ fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    પસંદગી (tok = lex(s, &val)) {
    સ્થિતિ TOK_END_OF_લીટી:
        જાઓ initial_eol;
    સ્થિતિ TOK_END_OF_FILE:
        જાઓ finished;
    સ્થિતિ TOK_INCLUDE:
        જાઓ include;
    મૂળભૂત:
        જાઓ lhs_tok;
    }

include:
    પસંદગી (tok = lex_include_string(s, table, &val)) {
    સ્થિતિ TOK_INCLUDE_STRING:
        જાઓ include_eol;
    મૂળભૂત:
        જાઓ unexpected;
    }

include_eol:
    પસંદગી (tok = lex(s, &val)) {
    સ્થિતિ TOK_END_OF_લીટી:
        જો (!do_include(table, s, val.string.str, include_depth))
            જાઓ fail;
        જાઓ initial;
    મૂળભૂત:
        જાઓ unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    પસંદગી (tok) {
    સ્થિતિ TOK_COLON:
        જો (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            જાઓ skip;
        }
        જાઓ rhs;
    સ્થિતિ TOK_IDENT:
        જો (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            જાઓ lhs_keysym;
        }
        જાઓ lhs_mod_list_tok;
    સ્થિતિ TOK_TILDE:
        જાઓ lhs_mod_list_tok;
    સ્થિતિ TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        જાઓ lhs_mod_list;
    મૂળભૂત:
        જાઓ lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    પસંદગી (tok) {
    સ્થિતિ TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        જો (keysym == XKB_ચાવી_No_પ્રતીક) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            જાઓ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        જો (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            જાઓ skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        જાઓ lhs;
    મૂળભૂત:
        જાઓ unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_index_t mod;

        જો (tok != TOK_TILDE && tok != TOK_IDENT)
            જાઓ lhs_keysym_tok;

        જો (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        જો (tok != TOK_IDENT)
            જાઓ unexpected;

        mod = resolve_modifier(val.string.str);
        જો (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            જાઓ error;
        }

        production.modmask |= UINT32_C(1) << mod;
        જો (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        નહીંતર
            production.mods |= UINT32_C(1) << mod;

        જાઓ lhs_mod_list;
    }

rhs:
    પસંદગી (tok = lex(s, &val)) {
    સ્થિતિ TOK_STRING:
        જો (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            જાઓ skip;
        }
        જો (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            જાઓ skip;
        }
        જો (val.string.len > કદ(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         કદ(production.string) - 1,
                         val.string.len - 1);
            જાઓ skip;
        }
        memcpy(production.string, val.string.str, val.string.len);
        production.has_string = true;
        જાઓ rhs;
    સ્થિતિ TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        જો (keysym == XKB_ચાવી_No_પ્રતીક) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            જાઓ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        જો (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            જાઓ skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    સ્થિતિ TOK_END_OF_લીટી:
        જો (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            જાઓ skip;
        }
        add_production(table, s, &production);
        જાઓ initial;
    મૂળભૂત:
        જાઓ unexpected;
    }

unexpected:
    જો (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    અંક_errors++;
    જો (અંક_errors <= MAX_ERRORS)
        જાઓ skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    જાઓ fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    પાછા false;

skip:
    જ્યારે (tok != TOK_END_OF_લીટી && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    જાઓ initial;

finished:
    પાછા true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(સંરચના xkb_compose_table *table, અચલ અક્ષર *string, size_t len,
             અચલ અક્ષર *file_name)
{
    સંરચના scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, ખાલી);
    જો (!parse(table, &s, 0))
        પાછા false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    પાછા true;
}

bool
parse_file(સંરચના xkb_compose_table *table, FILE *file, અચલ અક્ષર *file_name)
{
    bool ok;
    અક્ષર *string;
    size_t size;

    ok = map_file(file, &string, &size);
    જો (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        પાછા false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    પાછા ok;
}
