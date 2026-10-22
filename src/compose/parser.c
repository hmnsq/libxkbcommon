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

#समावेश "config.h"

#समावेश <errno.h>
#समावेश <string.h>

#समावेश "xkbcommon/xkbcommon-keysyms.h"
#समावेश "darray.h"
#समावेश "messages-codes.h"
#समावेश "utils.h"
#समावेश "constants.h"
#समावेश "table.h"
#समावेश "scanner-utils.h"
#समावेश "paths.h"
#समावेश "utf8.h"
#समावेश "parser.h"
#समावेश "keysym.h"

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

क्रमागत rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_पंक्ति,
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
जोड़ lvalue {
    काष्ठा {
        /* Still \0-terminated. */
        स्थिर अक्षर *str;
        माप_प्रकार len;
    } string;
};

अटल क्रमागत rules_token
lex(काष्ठा scanner *s, जोड़ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    जबतक (is_space(scanner_peek(s)))
        अगर (scanner_अग्र(s) == '\n')
            वापस TOK_END_OF_पंक्ति;

    /* Skip comments. */
    अगर (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        जाओ skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    अगर (scanner_eof(s)) वापस TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    अगर (scanner_chr(s, '<')) {
        जबतक (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_अग्र(s));
        अगर (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            वापस TOK_ERROR;
        }
        अगर (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            वापस TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        वापस TOK_LHS_KEYSYM;
    }

    /* Colon. */
    अगर (scanner_chr(s, ':'))
        वापस TOK_COLON;
    अगर (scanner_chr(s, '!'))
        वापस TOK_BANG;
    अगर (scanner_chr(s, '~'))
        वापस TOK_TILDE;

    /* String literal. */
    अगर (scanner_chr(s, '\"')) {
        जबतक (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            अगर (scanner_chr(s, '\\')) {
                uint8_t o;
                माप_प्रकार start_pos = s->pos;
                अगर (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                अन्यथा अगर (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                अन्यथा अगर (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    अगर (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (अक्षर) o);
                    } अन्यथा {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (पूर्णांक) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                अन्यथा अगर (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (अक्षर) o);
                }
                अन्यथा अगर (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (पूर्णांक) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                अन्यथा {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } अन्यथा {
                scanner_buf_append(s, scanner_अग्र(s));
            }
        }
        अगर (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            वापस TOK_ERROR;
        }
        अगर (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            वापस TOK_ERROR;
        }
        अगर (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            वापस TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        वापस TOK_STRING;
    }

    /* Identifier or include. */
    अगर (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        जबतक (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_अग्र(s));
        अगर (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            वापस TOK_ERROR;
        }

        अगर (streq(s->buf, "include"))
            वापस TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        वापस TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    वापस TOK_ERROR;
}

अटल क्रमागत rules_token
lex_include_string(काष्ठा scanner *s, काष्ठा xkb_compose_table *table,
                   जोड़ lvalue *val_out)
{
    जबतक (is_space(scanner_peek(s)))
        अगर (scanner_अग्र(s) == '\n')
            वापस TOK_END_OF_पंक्ति;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    अगर (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        वापस TOK_ERROR;
    }

    जबतक (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        अगर (scanner_chr(s, '%')) {
            अगर (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            अन्यथा अगर (scanner_chr(s, 'H')) {
                स्थिर अक्षर *home = xkb_context_getenv(table->ctx, "HOME");
                अगर (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    वापस TOK_ERROR;
                }
                अगर (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    वापस TOK_ERROR;
                }
            }
            अन्यथा अगर (scanner_chr(s, 'L')) {
                अक्षर *path = get_locale_compose_file_path(table->ctx, table->locale);
                अगर (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    वापस TOK_ERROR;
                }
                अगर (!scanner_buf_appends(s, path)) {
                    मुक्त(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    वापस TOK_ERROR;
                }
                मुक्त(path);
            }
            अन्यथा अगर (scanner_chr(s, 'S')) {
                स्थिर अक्षर *xlocaledir = get_xlocaledir_path(table->ctx);
                अगर (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    वापस TOK_ERROR;
                }
            }
            अन्यथा {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                वापस TOK_ERROR;
            }
        } अन्यथा {
            scanner_buf_append(s, scanner_अग्र(s));
        }
    }
    अगर (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        वापस TOK_ERROR;
    }
    अगर (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        वापस TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    वापस TOK_INCLUDE_STRING;
}

काष्ठा production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    अचिन्हित पूर्णांक len;
    xkb_keysym_t keysym;
    अक्षर string[XKB_COMPOSE_MAX_STRING_आकार];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

अटल व्योम
add_production(काष्ठा xkb_compose_table *table, काष्ठा scanner *s,
               स्थिर काष्ठा production *production)
{
    अचिन्हित पूर्णांक lhs_pos = 0;
    uint32_t curr = darray_आकार(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = शून्य;
    काष्ठा compose_गाँठ *गाँठ = शून्य;

    /* Warn before potentially going over the limit, discard silently after. */
    अगर (darray_आकार(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    अगर (darray_आकार(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        वापस;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    जबतक (true) {
        स्थिर xkb_keysym_t keysym = production->lhs[lhs_pos];
        स्थिर bool last = lhs_pos + 1 == production->len;

        अगर (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            काष्ठा compose_गाँठ new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_आकार(table->nodes);
            अगर (pptr != शून्य) {
                *pptr = curr;
                pptr = शून्य;
            }
            darray_append(table->nodes, new);
        }

        गाँठ = &darray_item(table->nodes, curr);

        अगर (keysym < गाँठ->keysym) {
            pptr = &गाँठ->lokid;
            curr = गाँठ->lokid;
        } अन्यथा अगर (keysym > गाँठ->keysym) {
            pptr = &गाँठ->hikid;
            curr = गाँठ->hikid;
        } अन्यथा अगर (!last) {
            अगर (गाँठ->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                गाँठ->internal.eqkid = 0;
                गाँठ->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &गाँठ->internal.eqkid;
            curr = गाँठ->internal.eqkid;
        } अन्यथा {
            अगर (गाँठ->is_leaf) {
                bool same_string =
                    (गाँठ->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        गाँठ->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, गाँठ->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (गाँठ->leaf.keysym == XKB_कुंजी_No_प्रतीक && !production->has_keysym) ||
                    (
                        गाँठ->leaf.keysym != XKB_कुंजी_No_प्रतीक && production->has_keysym &&
                        गाँठ->leaf.keysym == production->keysym
                    );
                अगर (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    वापस;
                } अन्यथा {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } अन्यथा अगर (गाँठ->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                गाँठ->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            अगर (production->has_string) {
                स्थिर माप_प्रकार len = म_माप(production->string);
                अगर (गाँठ->is_leaf && गाँठ->leaf.utf8 &&
                    len <= म_माप(&darray_item(table->utf8, गाँठ->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    स_नकल(&darray_item(table->utf8, गाँठ->leaf.utf8),
                           production->string, len + 1);
                } अन्यथा {
                    /* Cannot reuse string: allocate new one */
                    गाँठ->leaf.utf8 = darray_आकार(table->utf8);
                    darray_append_वस्तुएँ(table->utf8, production->string,
                                        (darray_आकार_t)म_माप(production->string) + 1);
                }
            } अन्यथा {
                /* Ensure we reset possible previous entry */
                गाँठ->leaf.utf8 = 0;
            }

            अगर (production->has_keysym) {
                गाँठ->leaf.keysym = production->keysym;
            } अन्यथा {
                /* Ensure we reset possible previous entry */
                गाँठ->leaf.keysym = XKB_कुंजी_No_प्रतीक;
            }

            गाँठ->is_leaf = true;
            वापस;
        }
    }
}

/* Should match resolve_modifier(). */
#घोषणा ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

अटल xkb_mod_सूचकांक_t
resolve_modifier(स्थिर अक्षर *name)
{
    अटल स्थिर काष्ठा {
        स्थिर अक्षर *name;
        xkb_mod_सूचकांक_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    क्रम (अचिन्हित पूर्णांक i = 0; i < ARRAY_आकार(mods); i++)
        अगर (streq(name, mods[i].name))
            वापस mods[i].mod;

    वापस XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
अक्षर *
parse_string_literal(काष्ठा xkb_context *ctx, स्थिर अक्षर *string)
{
    काष्ठा scanner s;
    जोड़ lvalue val;
    scanner_init(&s, ctx, string, म_माप(string), "(unnamed)", शून्य);
    अगर (lex(&s, &val) == TOK_STRING) {
        वापस strdup(val.string.str);
    } अन्यथा {
        ख_लिखो(मानक_त्रुटि, "ERROR: %s\n", s.s);
        वापस शून्य;
    }
}

अटल bool
parse(काष्ठा xkb_compose_table *table, काष्ठा scanner *s,
      अचिन्हित पूर्णांक include_depth);

अटल bool
do_include(काष्ठा xkb_compose_table *table, काष्ठा scanner *s,
           स्थिर अक्षर *path, अचिन्हित पूर्णांक include_depth)
{
    खाता *file;
    bool ok;
    अक्षर *string;
    माप_प्रकार आकार;
    काष्ठा scanner new_s;

    अगर (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        वापस false;
    }

    file = ख_खोलो(path, "rb");
    अगर (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, म_त्रुटि(त्रुटि_सं));
        वापस false;
    }

    ok = map_file(file, &string, &आकार);
    अगर (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, म_त्रुटि(त्रुटि_सं));
        जाओ err_file;
    }

    scanner_init(&new_s, table->ctx, string, आकार, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    अगर (!ok)
        जाओ err_unmap;

err_unmap:
    unmap_file(string, आकार);
err_file:
    ख_बंद(file);
    वापस ok;
}

// NOLINTBEGIN(readability-trivial-switch)
अटल bool
parse(काष्ठा xkb_compose_table *table, काष्ठा scanner *s,
      अचिन्हित पूर्णांक include_depth)
{
    क्रमागत rules_token tok;
    जोड़ lvalue val;
    xkb_keysym_t keysym;
    काष्ठा production production;
    क्रमागत { MAX_ERRORS = 10 };
    पूर्णांक अंक_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    अगर (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        जाओ fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    चयन (tok = lex(s, &val)) {
    हाल TOK_END_OF_पंक्ति:
        जाओ initial_eol;
    हाल TOK_END_OF_FILE:
        जाओ finished;
    हाल TOK_INCLUDE:
        जाओ include;
    शेष:
        जाओ lhs_tok;
    }

include:
    चयन (tok = lex_include_string(s, table, &val)) {
    हाल TOK_INCLUDE_STRING:
        जाओ include_eol;
    शेष:
        जाओ unexpected;
    }

include_eol:
    चयन (tok = lex(s, &val)) {
    हाल TOK_END_OF_पंक्ति:
        अगर (!do_include(table, s, val.string.str, include_depth))
            जाओ fail;
        जाओ initial;
    शेष:
        जाओ unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    चयन (tok) {
    हाल TOK_COLON:
        अगर (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            जाओ skip;
        }
        जाओ rhs;
    हाल TOK_IDENT:
        अगर (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            जाओ lhs_keysym;
        }
        जाओ lhs_mod_list_tok;
    हाल TOK_TILDE:
        जाओ lhs_mod_list_tok;
    हाल TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        जाओ lhs_mod_list;
    शेष:
        जाओ lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    चयन (tok) {
    हाल TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        अगर (keysym == XKB_कुंजी_No_प्रतीक) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            जाओ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        अगर (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            जाओ skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        जाओ lhs;
    शेष:
        जाओ unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_सूचकांक_t mod;

        अगर (tok != TOK_TILDE && tok != TOK_IDENT)
            जाओ lhs_keysym_tok;

        अगर (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        अगर (tok != TOK_IDENT)
            जाओ unexpected;

        mod = resolve_modifier(val.string.str);
        अगर (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            जाओ error;
        }

        production.modmask |= UINT32_C(1) << mod;
        अगर (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        अन्यथा
            production.mods |= UINT32_C(1) << mod;

        जाओ lhs_mod_list;
    }

rhs:
    चयन (tok = lex(s, &val)) {
    हाल TOK_STRING:
        अगर (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            जाओ skip;
        }
        अगर (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            जाओ skip;
        }
        अगर (val.string.len > माप(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         माप(production.string) - 1,
                         val.string.len - 1);
            जाओ skip;
        }
        स_नकल(production.string, val.string.str, val.string.len);
        production.has_string = true;
        जाओ rhs;
    हाल TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        अगर (keysym == XKB_कुंजी_No_प्रतीक) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            जाओ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        अगर (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            जाओ skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    हाल TOK_END_OF_पंक्ति:
        अगर (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            जाओ skip;
        }
        add_production(table, s, &production);
        जाओ initial;
    शेष:
        जाओ unexpected;
    }

unexpected:
    अगर (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    अंक_errors++;
    अगर (अंक_errors <= MAX_ERRORS)
        जाओ skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    जाओ fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    वापस false;

skip:
    जबतक (tok != TOK_END_OF_पंक्ति && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    जाओ initial;

finished:
    वापस true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(काष्ठा xkb_compose_table *table, स्थिर अक्षर *string, माप_प्रकार len,
             स्थिर अक्षर *file_name)
{
    काष्ठा scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, शून्य);
    अगर (!parse(table, &s, 0))
        वापस false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    वापस true;
}

bool
parse_file(काष्ठा xkb_compose_table *table, खाता *file, स्थिर अक्षर *file_name)
{
    bool ok;
    अक्षर *string;
    माप_प्रकार आकार;

    ok = map_file(file, &string, &आकार);
    अगर (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, म_त्रुटि(त्रुटि_सं));
        वापस false;
    }

    ok = parse_string(table, string, आकार, file_name);
    unmap_file(string, आकार);
    वापस ok;
}
