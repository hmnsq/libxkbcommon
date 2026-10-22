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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-keysyms.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "constants.h"
#ଅନ୍ତର୍ଭାଵ "table.h"
#ଅନ୍ତର୍ଭାଵ "scanner-utils.h"
#ଅନ୍ତର୍ଭାଵ "paths.h"
#ଅନ୍ତର୍ଭାଵ "utf8.h"
#ଅନ୍ତର୍ଭାଵ "parser.h"
#ଅନ୍ତର୍ଭାଵ "keysym.h"

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

ଗଣନା rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_ଧାଡ଼ି,
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
ସଂଘ lvalue {
    ସଂରଚନା {
        /* Still \0-terminated. */
        ଅଚଳ ଅକ୍ଷର *str;
        size_t len;
    } string;
};

ସ୍ଥିର ଗଣନା rules_token
lex(ସଂରଚନା scanner *s, ସଂଘ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    ଯେତେବେଳେ (is_space(scanner_peek(s)))
        ଯଦି (scanner_next(s) == '\n')
            ଫେରନ୍ତୁ TOK_END_OF_ଧାଡ଼ି;

    /* Skip comments. */
    ଯଦି (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        ଯାଅ skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    ଯଦି (scanner_eof(s)) ଫେରନ୍ତୁ TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    ଯଦି (scanner_chr(s, '<')) {
        ଯେତେବେଳେ (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_next(s));
        ଯଦି (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            ଫେରନ୍ତୁ TOK_ERROR;
        }
        ଯଦି (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            ଫେରନ୍ତୁ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ଫେରନ୍ତୁ TOK_LHS_KEYSYM;
    }

    /* Colon. */
    ଯଦି (scanner_chr(s, ':'))
        ଫେରନ୍ତୁ TOK_COLON;
    ଯଦି (scanner_chr(s, '!'))
        ଫେରନ୍ତୁ TOK_BANG;
    ଯଦି (scanner_chr(s, '~'))
        ଫେରନ୍ତୁ TOK_TILDE;

    /* String literal. */
    ଯଦି (scanner_chr(s, '\"')) {
        ଯେତେବେଳେ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ଯଦି (scanner_chr(s, '\\')) {
                uint8_t o;
                size_t start_pos = s->pos;
                ଯଦି (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                ଅନ୍ୟଥା ଯଦି (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                ଅନ୍ୟଥା ଯଦି (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    ଯଦି (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (ଅକ୍ଷର) o);
                    } ଅନ୍ୟଥା {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                ଅନ୍ୟଥା ଯଦି (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (ଅକ୍ଷର) o);
                }
                ଅନ୍ୟଥା ଯଦି (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                ଅନ୍ୟଥା {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ଅନ୍ୟଥା {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        ଯଦି (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            ଫେରନ୍ତୁ TOK_ERROR;
        }
        ଯଦି (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            ଫେରନ୍ତୁ TOK_ERROR;
        }
        ଯଦି (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            ଫେରନ୍ତୁ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ଫେରନ୍ତୁ TOK_STRING;
    }

    /* Identifier or include. */
    ଯଦି (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        ଯେତେବେଳେ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_next(s));
        ଯଦି (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            ଫେରନ୍ତୁ TOK_ERROR;
        }

        ଯଦି (streq(s->buf, "include"))
            ଫେରନ୍ତୁ TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ଫେରନ୍ତୁ TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    ଫେରନ୍ତୁ TOK_ERROR;
}

ସ୍ଥିର ଗଣନା rules_token
lex_include_string(ସଂରଚନା scanner *s, ସଂରଚନା xkb_compose_table *table,
                   ସଂଘ lvalue *val_out)
{
    ଯେତେବେଳେ (is_space(scanner_peek(s)))
        ଯଦି (scanner_next(s) == '\n')
            ଫେରନ୍ତୁ TOK_END_OF_ଧାଡ଼ି;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    ଯଦି (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        ଫେରନ୍ତୁ TOK_ERROR;
    }

    ଯେତେବେଳେ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        ଯଦି (scanner_chr(s, '%')) {
            ଯଦି (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            ଅନ୍ୟଥା ଯଦି (scanner_chr(s, 'H')) {
                ଅଚଳ ଅକ୍ଷର *home = xkb_context_getenv(table->ctx, "HOME");
                ଯଦି (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    ଫେରନ୍ତୁ TOK_ERROR;
                }
                ଯଦି (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    ଫେରନ୍ତୁ TOK_ERROR;
                }
            }
            ଅନ୍ୟଥା ଯଦି (scanner_chr(s, 'L')) {
                ଅକ୍ଷର *path = get_locale_compose_file_path(table->ctx, table->locale);
                ଯଦି (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    ଫେରନ୍ତୁ TOK_ERROR;
                }
                ଯଦି (!scanner_buf_appends(s, path)) {
                    ମୁକ୍ତ(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    ଫେରନ୍ତୁ TOK_ERROR;
                }
                ମୁକ୍ତ(path);
            }
            ଅନ୍ୟଥା ଯଦି (scanner_chr(s, 'S')) {
                ଅଚଳ ଅକ୍ଷର *xlocaledir = get_xlocaledir_path(table->ctx);
                ଯଦି (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    ଫେରନ୍ତୁ TOK_ERROR;
                }
            }
            ଅନ୍ୟଥା {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                ଫେରନ୍ତୁ TOK_ERROR;
            }
        } ଅନ୍ୟଥା {
            scanner_buf_append(s, scanner_next(s));
        }
    }
    ଯଦି (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        ଫେରନ୍ତୁ TOK_ERROR;
    }
    ଯଦି (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        ଫେରନ୍ତୁ TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    ଫେରନ୍ତୁ TOK_INCLUDE_STRING;
}

ସଂରଚନା production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
    xkb_keysym_t keysym;
    ଅକ୍ଷର string[XKB_COMPOSE_MAX_STRING_SIZE];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

ସ୍ଥିର ଶୂନ୍ୟ
add_production(ସଂରଚନା xkb_compose_table *table, ସଂରଚନା scanner *s,
               ଅଚଳ ସଂରଚନା production *production)
{
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା lhs_pos = 0;
    uint32_t curr = darray_size(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = ଖାଲି;
    ସଂରଚନା compose_ଗଣ୍ଠି *ଗଣ୍ଠି = ଖାଲି;

    /* Warn before potentially going over the limit, discard silently after. */
    ଯଦି (darray_size(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    ଯଦି (darray_size(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        ଫେରନ୍ତୁ;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    ଯେତେବେଳେ (true) {
        ଅଚଳ xkb_keysym_t keysym = production->lhs[lhs_pos];
        ଅଚଳ bool last = lhs_pos + 1 == production->len;

        ଯଦି (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            ସଂରଚନା compose_ଗଣ୍ଠି new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_size(table->nodes);
            ଯଦି (pptr != ଖାଲି) {
                *pptr = curr;
                pptr = ଖାଲି;
            }
            darray_append(table->nodes, new);
        }

        ଗଣ୍ଠି = &darray_item(table->nodes, curr);

        ଯଦି (keysym < ଗଣ୍ଠି->keysym) {
            pptr = &ଗଣ୍ଠି->lokid;
            curr = ଗଣ୍ଠି->lokid;
        } ଅନ୍ୟଥା ଯଦି (keysym > ଗଣ୍ଠି->keysym) {
            pptr = &ଗଣ୍ଠି->hikid;
            curr = ଗଣ୍ଠି->hikid;
        } ଅନ୍ୟଥା ଯଦି (!last) {
            ଯଦି (ଗଣ୍ଠି->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                ଗଣ୍ଠି->internal.eqkid = 0;
                ଗଣ୍ଠି->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &ଗଣ୍ଠି->internal.eqkid;
            curr = ଗଣ୍ଠି->internal.eqkid;
        } ଅନ୍ୟଥା {
            ଯଦି (ଗଣ୍ଠି->is_leaf) {
                bool same_string =
                    (ଗଣ୍ଠି->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        ଗଣ୍ଠି->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, ଗଣ୍ଠି->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (ଗଣ୍ଠି->leaf.keysym == XKB_ଚାବି_No_ପ୍ରତୀକ && !production->has_keysym) ||
                    (
                        ଗଣ୍ଠି->leaf.keysym != XKB_ଚାବି_No_ପ୍ରତୀକ && production->has_keysym &&
                        ଗଣ୍ଠି->leaf.keysym == production->keysym
                    );
                ଯଦି (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    ଫେରନ୍ତୁ;
                } ଅନ୍ୟଥା {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } ଅନ୍ୟଥା ଯଦି (ଗଣ୍ଠି->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                ଗଣ୍ଠି->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            ଯଦି (production->has_string) {
                ଅଚଳ size_t len = strlen(production->string);
                ଯଦି (ଗଣ୍ଠି->is_leaf && ଗଣ୍ଠି->leaf.utf8 &&
                    len <= strlen(&darray_item(table->utf8, ଗଣ୍ଠି->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    memcpy(&darray_item(table->utf8, ଗଣ୍ଠି->leaf.utf8),
                           production->string, len + 1);
                } ଅନ୍ୟଥା {
                    /* Cannot reuse string: allocate new one */
                    ଗଣ୍ଠି->leaf.utf8 = darray_size(table->utf8);
                    darray_append_ବସ୍ତୁଗୁଡ଼ିକ(table->utf8, production->string,
                                        (darray_size_t)strlen(production->string) + 1);
                }
            } ଅନ୍ୟଥା {
                /* Ensure we reset possible previous entry */
                ଗଣ୍ଠି->leaf.utf8 = 0;
            }

            ଯଦି (production->has_keysym) {
                ଗଣ୍ଠି->leaf.keysym = production->keysym;
            } ଅନ୍ୟଥା {
                /* Ensure we reset possible previous entry */
                ଗଣ୍ଠି->leaf.keysym = XKB_ଚାବି_No_ପ୍ରତୀକ;
            }

            ଗଣ୍ଠି->is_leaf = true;
            ଫେରନ୍ତୁ;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

ସ୍ଥିର xkb_mod_index_t
resolve_modifier(ଅଚଳ ଅକ୍ଷର *name)
{
    ସ୍ଥିର ଅଚଳ ସଂରଚନା {
        ଅଚଳ ଅକ୍ଷର *name;
        xkb_mod_index_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ARRAY_SIZE(mods); i++)
        ଯଦି (streq(name, mods[i].name))
            ଫେରନ୍ତୁ mods[i].mod;

    ଫେରନ୍ତୁ XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
ଅକ୍ଷର *
parse_string_literal(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଅକ୍ଷର *string)
{
    ସଂରଚନା scanner s;
    ସଂଘ lvalue val;
    scanner_init(&s, ctx, string, strlen(string), "(unnamed)", ଖାଲି);
    ଯଦି (lex(&s, &val) == TOK_STRING) {
        ଫେରନ୍ତୁ strdup(val.string.str);
    } ଅନ୍ୟଥା {
        fprintf(stderr, "ERROR: %s\n", s.s);
        ଫେରନ୍ତୁ ଖାଲି;
    }
}

ସ୍ଥିର bool
parse(ସଂରଚନା xkb_compose_table *table, ସଂରଚନା scanner *s,
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth);

ସ୍ଥିର bool
do_include(ସଂରଚନା xkb_compose_table *table, ସଂରଚନା scanner *s,
           ଅଚଳ ଅକ୍ଷର *path, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth)
{
    FILE *file;
    bool ok;
    ଅକ୍ଷର *string;
    size_t size;
    ସଂରଚନା scanner new_s;

    ଯଦି (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        ଫେରନ୍ତୁ false;
    }

    file = fopen(path, "rb");
    ଯଦି (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        ଫେରନ୍ତୁ false;
    }

    ok = map_file(file, &string, &size);
    ଯଦି (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        ଯାଅ err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    ଯଦି (!ok)
        ଯାଅ err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    fclose(file);
    ଫେରନ୍ତୁ ok;
}

// NOLINTBEGIN(readability-trivial-switch)
ସ୍ଥିର bool
parse(ସଂରଚନା xkb_compose_table *table, ସଂରଚନା scanner *s,
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth)
{
    ଗଣନା rules_token tok;
    ସଂଘ lvalue val;
    xkb_keysym_t keysym;
    ସଂରଚନା production production;
    ଗଣନା { MAX_ERRORS = 10 };
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    ଯଦି (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        ଯାଅ fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    ବାଛ (tok = lex(s, &val)) {
    ସ୍ଥିତି TOK_END_OF_ଧାଡ଼ି:
        ଯାଅ initial_eol;
    ସ୍ଥିତି TOK_END_OF_FILE:
        ଯାଅ finished;
    ସ୍ଥିତି TOK_INCLUDE:
        ଯାଅ include;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ lhs_tok;
    }

include:
    ବାଛ (tok = lex_include_string(s, table, &val)) {
    ସ୍ଥିତି TOK_INCLUDE_STRING:
        ଯାଅ include_eol;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

include_eol:
    ବାଛ (tok = lex(s, &val)) {
    ସ୍ଥିତି TOK_END_OF_ଧାଡ଼ି:
        ଯଦି (!do_include(table, s, val.string.str, include_depth))
            ଯାଅ fail;
        ଯାଅ initial;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    ବାଛ (tok) {
    ସ୍ଥିତି TOK_COLON:
        ଯଦି (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            ଯାଅ skip;
        }
        ଯାଅ rhs;
    ସ୍ଥିତି TOK_IDENT:
        ଯଦି (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            ଯାଅ lhs_keysym;
        }
        ଯାଅ lhs_mod_list_tok;
    ସ୍ଥିତି TOK_TILDE:
        ଯାଅ lhs_mod_list_tok;
    ସ୍ଥିତି TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        ଯାଅ lhs_mod_list;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    ବାଛ (tok) {
    ସ୍ଥିତି TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ଯଦି (keysym == XKB_ଚାବି_No_ପ୍ରତୀକ) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            ଯାଅ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ଯଦି (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            ଯାଅ skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        ଯାଅ lhs;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_index_t mod;

        ଯଦି (tok != TOK_TILDE && tok != TOK_IDENT)
            ଯାଅ lhs_keysym_tok;

        ଯଦି (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        ଯଦି (tok != TOK_IDENT)
            ଯାଅ unexpected;

        mod = resolve_modifier(val.string.str);
        ଯଦି (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            ଯାଅ error;
        }

        production.modmask |= UINT32_C(1) << mod;
        ଯଦି (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        ଅନ୍ୟଥା
            production.mods |= UINT32_C(1) << mod;

        ଯାଅ lhs_mod_list;
    }

rhs:
    ବାଛ (tok = lex(s, &val)) {
    ସ୍ଥିତି TOK_STRING:
        ଯଦି (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            ଯାଅ skip;
        }
        ଯଦି (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            ଯାଅ skip;
        }
        ଯଦି (val.string.len > ଆକାର(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         ଆକାର(production.string) - 1,
                         val.string.len - 1);
            ଯାଅ skip;
        }
        memcpy(production.string, val.string.str, val.string.len);
        production.has_string = true;
        ଯାଅ rhs;
    ସ୍ଥିତି TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ଯଦି (keysym == XKB_ଚାବି_No_ପ୍ରତୀକ) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            ଯାଅ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ଯଦି (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            ଯାଅ skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    ସ୍ଥିତି TOK_END_OF_ଧାଡ଼ି:
        ଯଦି (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            ଯାଅ skip;
        }
        add_production(table, s, &production);
        ଯାଅ initial;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

unexpected:
    ଯଦି (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    ଅଙ୍କ_errors++;
    ଯଦି (ଅଙ୍କ_errors <= MAX_ERRORS)
        ଯାଅ skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    ଯାଅ fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    ଫେରନ୍ତୁ false;

skip:
    ଯେତେବେଳେ (tok != TOK_END_OF_ଧାଡ଼ି && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    ଯାଅ initial;

finished:
    ଫେରନ୍ତୁ true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(ସଂରଚନା xkb_compose_table *table, ଅଚଳ ଅକ୍ଷର *string, size_t len,
             ଅଚଳ ଅକ୍ଷର *file_name)
{
    ସଂରଚନା scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, ଖାଲି);
    ଯଦି (!parse(table, &s, 0))
        ଫେରନ୍ତୁ false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    ଫେରନ୍ତୁ true;
}

bool
parse_file(ସଂରଚନା xkb_compose_table *table, FILE *file, ଅଚଳ ଅକ୍ଷର *file_name)
{
    bool ok;
    ଅକ୍ଷର *string;
    size_t size;

    ok = map_file(file, &string, &size);
    ଯଦି (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        ଫେରନ୍ତୁ false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    ଫେରନ୍ତୁ ok;
}
