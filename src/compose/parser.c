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

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "xkbcommon/xkbcommon-keysyms.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "constants.h"
#অন্তর্ভাব "table.h"
#অন্তর্ভাব "scanner-utils.h"
#অন্তর্ভাব "paths.h"
#অন্তর্ভাব "utf8.h"
#অন্তর্ভাব "parser.h"
#অন্তর্ভাব "keysym.h"

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
    TOK_END_OF_সারি,
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
সংঘ lvalue {
    কাঠামো {
        /* Still \0-terminated. */
        ধ্রুবক অক্ষর *str;
        আকার_t len;
    } string;
};

স্থির গণনা rules_token
lex(কাঠামো scanner *s, সংঘ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    যখন (is_space(scanner_peek(s)))
        যদি (scanner_পরবর্তী(s) == '\n')
            ফেরত TOK_END_OF_সারি;

    /* Skip comments. */
    যদি (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        যাও skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    যদি (scanner_eof(s)) ফেরত TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    যদি (scanner_chr(s, '<')) {
        যখন (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_পরবর্তী(s));
        যদি (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            ফেরত TOK_ERROR;
        }
        যদি (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            ফেরত TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ফেরত TOK_LHS_KEYSYM;
    }

    /* Colon. */
    যদি (scanner_chr(s, ':'))
        ফেরত TOK_COLON;
    যদি (scanner_chr(s, '!'))
        ফেরত TOK_BANG;
    যদি (scanner_chr(s, '~'))
        ফেরত TOK_TILDE;

    /* String literal. */
    যদি (scanner_chr(s, '\"')) {
        যখন (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            যদি (scanner_chr(s, '\\')) {
                uint8_t o;
                আকার_t start_pos = s->pos;
                যদি (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                অন্যথা যদি (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                অন্যথা যদি (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    যদি (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (অক্ষর) o);
                    } অন্যথা {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (পূর্ণসংখ্যা) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                অন্যথা যদি (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (অক্ষর) o);
                }
                অন্যথা যদি (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (পূর্ণসংখ্যা) (s->pos - start_pos + 1),
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
                scanner_buf_append(s, scanner_পরবর্তী(s));
            }
        }
        যদি (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            ফেরত TOK_ERROR;
        }
        যদি (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            ফেরত TOK_ERROR;
        }
        যদি (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            ফেরত TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ফেরত TOK_STRING;
    }

    /* Identifier or include. */
    যদি (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        যখন (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_পরবর্তী(s));
        যদি (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            ফেরত TOK_ERROR;
        }

        যদি (streq(s->buf, "include"))
            ফেরত TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ফেরত TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    ফেরত TOK_ERROR;
}

স্থির গণনা rules_token
lex_include_string(কাঠামো scanner *s, কাঠামো xkb_compose_table *table,
                   সংঘ lvalue *val_out)
{
    যখন (is_space(scanner_peek(s)))
        যদি (scanner_পরবর্তী(s) == '\n')
            ফেরত TOK_END_OF_সারি;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    যদি (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        ফেরত TOK_ERROR;
    }

    যখন (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        যদি (scanner_chr(s, '%')) {
            যদি (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            অন্যথা যদি (scanner_chr(s, 'H')) {
                ধ্রুবক অক্ষর *home = xkb_context_getenv(table->ctx, "HOME");
                যদি (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    ফেরত TOK_ERROR;
                }
                যদি (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    ফেরত TOK_ERROR;
                }
            }
            অন্যথা যদি (scanner_chr(s, 'L')) {
                অক্ষর *path = get_locale_compose_file_path(table->ctx, table->locale);
                যদি (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    ফেরত TOK_ERROR;
                }
                যদি (!scanner_buf_appends(s, path)) {
                    মুক্ত(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    ফেরত TOK_ERROR;
                }
                মুক্ত(path);
            }
            অন্যথা যদি (scanner_chr(s, 'S')) {
                ধ্রুবক অক্ষর *xlocaledir = get_xlocaledir_path(table->ctx);
                যদি (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    ফেরত TOK_ERROR;
                }
            }
            অন্যথা {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                ফেরত TOK_ERROR;
            }
        } অন্যথা {
            scanner_buf_append(s, scanner_পরবর্তী(s));
        }
    }
    যদি (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        ফেরত TOK_ERROR;
    }
    যদি (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        ফেরত TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    ফেরত TOK_INCLUDE_STRING;
}

কাঠামো production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    unsigned পূর্ণসংখ্যা len;
    xkb_keysym_t keysym;
    অক্ষর string[XKB_COMPOSE_MAX_STRING_আকার];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

স্থির শূন্য
add_production(কাঠামো xkb_compose_table *table, কাঠামো scanner *s,
               ধ্রুবক কাঠামো production *production)
{
    unsigned পূর্ণসংখ্যা lhs_pos = 0;
    uint32_t curr = darray_আকার(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = ফাঁকা;
    কাঠামো compose_গ্রন্থি *গ্রন্থি = ফাঁকা;

    /* Warn before potentially going over the limit, discard silently after. */
    যদি (darray_আকার(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    যদি (darray_আকার(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        ফেরত;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    যখন (true) {
        ধ্রুবক xkb_keysym_t keysym = production->lhs[lhs_pos];
        ধ্রুবক bool last = lhs_pos + 1 == production->len;

        যদি (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            কাঠামো compose_গ্রন্থি new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_আকার(table->nodes);
            যদি (pptr != ফাঁকা) {
                *pptr = curr;
                pptr = ফাঁকা;
            }
            darray_append(table->nodes, new);
        }

        গ্রন্থি = &darray_item(table->nodes, curr);

        যদি (keysym < গ্রন্থি->keysym) {
            pptr = &গ্রন্থি->lokid;
            curr = গ্রন্থি->lokid;
        } অন্যথা যদি (keysym > গ্রন্থি->keysym) {
            pptr = &গ্রন্থি->hikid;
            curr = গ্রন্থি->hikid;
        } অন্যথা যদি (!last) {
            যদি (গ্রন্থি->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                গ্রন্থি->internal.eqkid = 0;
                গ্রন্থি->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &গ্রন্থি->internal.eqkid;
            curr = গ্রন্থি->internal.eqkid;
        } অন্যথা {
            যদি (গ্রন্থি->is_leaf) {
                bool same_string =
                    (গ্রন্থি->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        গ্রন্থি->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, গ্রন্থি->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (গ্রন্থি->leaf.keysym == XKB_চাবি_No_প্রতীক && !production->has_keysym) ||
                    (
                        গ্রন্থি->leaf.keysym != XKB_চাবি_No_প্রতীক && production->has_keysym &&
                        গ্রন্থি->leaf.keysym == production->keysym
                    );
                যদি (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    ফেরত;
                } অন্যথা {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } অন্যথা যদি (গ্রন্থি->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                গ্রন্থি->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            যদি (production->has_string) {
                ধ্রুবক আকার_t len = strlen(production->string);
                যদি (গ্রন্থি->is_leaf && গ্রন্থি->leaf.utf8 &&
                    len <= strlen(&darray_item(table->utf8, গ্রন্থি->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    memcpy(&darray_item(table->utf8, গ্রন্থি->leaf.utf8),
                           production->string, len + 1);
                } অন্যথা {
                    /* Cannot reuse string: allocate new one */
                    গ্রন্থি->leaf.utf8 = darray_আকার(table->utf8);
                    darray_append_বস্তুসমূহ(table->utf8, production->string,
                                        (darray_আকার_t)strlen(production->string) + 1);
                }
            } অন্যথা {
                /* Ensure we reset possible previous entry */
                গ্রন্থি->leaf.utf8 = 0;
            }

            যদি (production->has_keysym) {
                গ্রন্থি->leaf.keysym = production->keysym;
            } অন্যথা {
                /* Ensure we reset possible previous entry */
                গ্রন্থি->leaf.keysym = XKB_চাবি_No_প্রতীক;
            }

            গ্রন্থি->is_leaf = true;
            ফেরত;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

স্থির xkb_mod_সূচক_t
resolve_modifier(ধ্রুবক অক্ষর *name)
{
    স্থির ধ্রুবক কাঠামো {
        ধ্রুবক অক্ষর *name;
        xkb_mod_সূচক_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < ARRAY_আকার(mods); i++)
        যদি (streq(name, mods[i].name))
            ফেরত mods[i].mod;

    ফেরত XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
অক্ষর *
parse_string_literal(কাঠামো xkb_context *ctx, ধ্রুবক অক্ষর *string)
{
    কাঠামো scanner s;
    সংঘ lvalue val;
    scanner_init(&s, ctx, string, strlen(string), "(unnamed)", ফাঁকা);
    যদি (lex(&s, &val) == TOK_STRING) {
        ফেরত strdup(val.string.str);
    } অন্যথা {
        fprintf(stderr, "ERROR: %s\n", s.s);
        ফেরত ফাঁকা;
    }
}

স্থির bool
parse(কাঠামো xkb_compose_table *table, কাঠামো scanner *s,
      unsigned পূর্ণসংখ্যা include_depth);

স্থির bool
do_include(কাঠামো xkb_compose_table *table, কাঠামো scanner *s,
           ধ্রুবক অক্ষর *path, unsigned পূর্ণসংখ্যা include_depth)
{
    FILE *file;
    bool ok;
    অক্ষর *string;
    আকার_t আকার_2;
    কাঠামো scanner new_s;

    যদি (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        ফেরত false;
    }

    file = fopen(path, "rb");
    যদি (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        ফেরত false;
    }

    ok = map_file(file, &string, &আকার_2);
    যদি (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        যাও err_file;
    }

    scanner_init(&new_s, table->ctx, string, আকার_2, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    যদি (!ok)
        যাও err_unmap;

err_unmap:
    unmap_file(string, আকার_2);
err_file:
    fclose(file);
    ফেরত ok;
}

// NOLINTBEGIN(readability-trivial-switch)
স্থির bool
parse(কাঠামো xkb_compose_table *table, কাঠামো scanner *s,
      unsigned পূর্ণসংখ্যা include_depth)
{
    গণনা rules_token tok;
    সংঘ lvalue val;
    xkb_keysym_t keysym;
    কাঠামো production production;
    গণনা { MAX_ERRORS = 10 };
    পূর্ণসংখ্যা অঙ্ক_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    যদি (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        যাও fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    নির্বাচন (tok = lex(s, &val)) {
    ক্ষেত্র TOK_END_OF_সারি:
        যাও initial_eol;
    ক্ষেত্র TOK_END_OF_FILE:
        যাও finished;
    ক্ষেত্র TOK_INCLUDE:
        যাও include;
    পূর্বনির্ধারিত:
        যাও lhs_tok;
    }

include:
    নির্বাচন (tok = lex_include_string(s, table, &val)) {
    ক্ষেত্র TOK_INCLUDE_STRING:
        যাও include_eol;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

include_eol:
    নির্বাচন (tok = lex(s, &val)) {
    ক্ষেত্র TOK_END_OF_সারি:
        যদি (!do_include(table, s, val.string.str, include_depth))
            যাও fail;
        যাও initial;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    নির্বাচন (tok) {
    ক্ষেত্র TOK_COLON:
        যদি (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            যাও skip;
        }
        যাও rhs;
    ক্ষেত্র TOK_IDENT:
        যদি (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            যাও lhs_keysym;
        }
        যাও lhs_mod_list_tok;
    ক্ষেত্র TOK_TILDE:
        যাও lhs_mod_list_tok;
    ক্ষেত্র TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        যাও lhs_mod_list;
    পূর্বনির্ধারিত:
        যাও lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    নির্বাচন (tok) {
    ক্ষেত্র TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        যদি (keysym == XKB_চাবি_No_প্রতীক) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            যাও error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        যদি (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            যাও skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        যাও lhs;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_সূচক_t mod;

        যদি (tok != TOK_TILDE && tok != TOK_IDENT)
            যাও lhs_keysym_tok;

        যদি (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        যদি (tok != TOK_IDENT)
            যাও unexpected;

        mod = resolve_modifier(val.string.str);
        যদি (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            যাও error;
        }

        production.modmask |= UINT32_C(1) << mod;
        যদি (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        অন্যথা
            production.mods |= UINT32_C(1) << mod;

        যাও lhs_mod_list;
    }

rhs:
    নির্বাচন (tok = lex(s, &val)) {
    ক্ষেত্র TOK_STRING:
        যদি (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            যাও skip;
        }
        যদি (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            যাও skip;
        }
        যদি (val.string.len > আকার(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         আকার(production.string) - 1,
                         val.string.len - 1);
            যাও skip;
        }
        memcpy(production.string, val.string.str, val.string.len);
        production.has_string = true;
        যাও rhs;
    ক্ষেত্র TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        যদি (keysym == XKB_চাবি_No_প্রতীক) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            যাও error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        যদি (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            যাও skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    ক্ষেত্র TOK_END_OF_সারি:
        যদি (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            যাও skip;
        }
        add_production(table, s, &production);
        যাও initial;
    পূর্বনির্ধারিত:
        যাও unexpected;
    }

unexpected:
    যদি (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    অঙ্ক_errors++;
    যদি (অঙ্ক_errors <= MAX_ERRORS)
        যাও skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    যাও fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    ফেরত false;

skip:
    যখন (tok != TOK_END_OF_সারি && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    যাও initial;

finished:
    ফেরত true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(কাঠামো xkb_compose_table *table, ধ্রুবক অক্ষর *string, আকার_t len,
             ধ্রুবক অক্ষর *file_name)
{
    কাঠামো scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, ফাঁকা);
    যদি (!parse(table, &s, 0))
        ফেরত false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    ফেরত true;
}

bool
parse_file(কাঠামো xkb_compose_table *table, FILE *file, ধ্রুবক অক্ষর *file_name)
{
    bool ok;
    অক্ষর *string;
    আকার_t আকার_2;

    ok = map_file(file, &string, &আকার_2);
    যদি (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        ফেরত false;
    }

    ok = parse_string(table, string, আকার_2, file_name);
    unmap_file(string, আকার_2);
    ফেরত ok;
}
