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

#अन्तर्भाव "config.h"

#अन्तर्भाव <errno.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon-keysyms.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "constants.h"
#अन्तर्भाव "table.h"
#अन्तर्भाव "scanner-utils.h"
#अन्तर्भाव "paths.h"
#अन्तर्भाव "utf8.h"
#अन्तर्भाव "parser.h"
#अन्तर्भाव "keysym.h"

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

गणना rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_ओळ,
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
संघ lvalue {
    संरचना {
        /* Still \0-terminated. */
        स्थिरांक अक्षर *str;
        माप_प्रकार len;
    } string;
};

स्थिर गणना rules_token
lex(संरचना scanner *s, संघ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    पर्यंत (is_space(scanner_peek(s)))
        जर (scanner_next(s) == '\n')
            परत TOK_END_OF_ओळ;

    /* Skip comments. */
    जर (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        जा skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    जर (scanner_eof(s)) परत TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    जर (scanner_chr(s, '<')) {
        पर्यंत (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_next(s));
        जर (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            परत TOK_ERROR;
        }
        जर (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            परत TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        परत TOK_LHS_KEYSYM;
    }

    /* Colon. */
    जर (scanner_chr(s, ':'))
        परत TOK_COLON;
    जर (scanner_chr(s, '!'))
        परत TOK_BANG;
    जर (scanner_chr(s, '~'))
        परत TOK_TILDE;

    /* String literal. */
    जर (scanner_chr(s, '\"')) {
        पर्यंत (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            जर (scanner_chr(s, '\\')) {
                uint8_t o;
                माप_प्रकार start_pos = s->pos;
                जर (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                अन्यथा जर (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                अन्यथा जर (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    जर (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (अक्षर) o);
                    } अन्यथा {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (पूर्णांक) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                अन्यथा जर (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (अक्षर) o);
                }
                अन्यथा जर (s->pos > start_pos) {
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
                scanner_buf_append(s, scanner_next(s));
            }
        }
        जर (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            परत TOK_ERROR;
        }
        जर (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            परत TOK_ERROR;
        }
        जर (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            परत TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        परत TOK_STRING;
    }

    /* Identifier or include. */
    जर (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        पर्यंत (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_next(s));
        जर (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            परत TOK_ERROR;
        }

        जर (streq(s->buf, "include"))
            परत TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        परत TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    परत TOK_ERROR;
}

स्थिर गणना rules_token
lex_include_string(संरचना scanner *s, संरचना xkb_compose_table *table,
                   संघ lvalue *val_out)
{
    पर्यंत (is_space(scanner_peek(s)))
        जर (scanner_next(s) == '\n')
            परत TOK_END_OF_ओळ;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    जर (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        परत TOK_ERROR;
    }

    पर्यंत (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        जर (scanner_chr(s, '%')) {
            जर (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            अन्यथा जर (scanner_chr(s, 'H')) {
                स्थिरांक अक्षर *home = xkb_context_getenv(table->ctx, "HOME");
                जर (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    परत TOK_ERROR;
                }
                जर (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    परत TOK_ERROR;
                }
            }
            अन्यथा जर (scanner_chr(s, 'L')) {
                अक्षर *path = get_locale_compose_file_path(table->ctx, table->locale);
                जर (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    परत TOK_ERROR;
                }
                जर (!scanner_buf_appends(s, path)) {
                    मुक्त(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    परत TOK_ERROR;
                }
                मुक्त(path);
            }
            अन्यथा जर (scanner_chr(s, 'S')) {
                स्थिरांक अक्षर *xlocaledir = get_xlocaledir_path(table->ctx);
                जर (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    परत TOK_ERROR;
                }
            }
            अन्यथा {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                परत TOK_ERROR;
            }
        } अन्यथा {
            scanner_buf_append(s, scanner_next(s));
        }
    }
    जर (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        परत TOK_ERROR;
    }
    जर (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        परत TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    परत TOK_INCLUDE_STRING;
}

संरचना production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    अचिन्हित पूर्णांक len;
    xkb_keysym_t keysym;
    अक्षर string[XKB_COMPOSE_MAX_STRING_SIZE];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

स्थिर शून्य
add_production(संरचना xkb_compose_table *table, संरचना scanner *s,
               स्थिरांक संरचना production *production)
{
    अचिन्हित पूर्णांक lhs_pos = 0;
    uint32_t curr = darray_size(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = रिक्त;
    संरचना compose_गाठ *गाठ = रिक्त;

    /* Warn before potentially going over the limit, discard silently after. */
    जर (darray_size(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    जर (darray_size(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        परत;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    पर्यंत (true) {
        स्थिरांक xkb_keysym_t keysym = production->lhs[lhs_pos];
        स्थिरांक bool last = lhs_pos + 1 == production->len;

        जर (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            संरचना compose_गाठ new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_size(table->nodes);
            जर (pptr != रिक्त) {
                *pptr = curr;
                pptr = रिक्त;
            }
            darray_append(table->nodes, new);
        }

        गाठ = &darray_item(table->nodes, curr);

        जर (keysym < गाठ->keysym) {
            pptr = &गाठ->lokid;
            curr = गाठ->lokid;
        } अन्यथा जर (keysym > गाठ->keysym) {
            pptr = &गाठ->hikid;
            curr = गाठ->hikid;
        } अन्यथा जर (!last) {
            जर (गाठ->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                गाठ->internal.eqkid = 0;
                गाठ->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &गाठ->internal.eqkid;
            curr = गाठ->internal.eqkid;
        } अन्यथा {
            जर (गाठ->is_leaf) {
                bool same_string =
                    (गाठ->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        गाठ->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, गाठ->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (गाठ->leaf.keysym == XKB_किल्ली_No_चिन्ह && !production->has_keysym) ||
                    (
                        गाठ->leaf.keysym != XKB_किल्ली_No_चिन्ह && production->has_keysym &&
                        गाठ->leaf.keysym == production->keysym
                    );
                जर (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    परत;
                } अन्यथा {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } अन्यथा जर (गाठ->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                गाठ->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            जर (production->has_string) {
                स्थिरांक माप_प्रकार len = म_माप(production->string);
                जर (गाठ->is_leaf && गाठ->leaf.utf8 &&
                    len <= म_माप(&darray_item(table->utf8, गाठ->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    स_नकल(&darray_item(table->utf8, गाठ->leaf.utf8),
                           production->string, len + 1);
                } अन्यथा {
                    /* Cannot reuse string: allocate new one */
                    गाठ->leaf.utf8 = darray_size(table->utf8);
                    darray_append_वस्तुसंच(table->utf8, production->string,
                                        (darray_size_t)म_माप(production->string) + 1);
                }
            } अन्यथा {
                /* Ensure we reset possible previous entry */
                गाठ->leaf.utf8 = 0;
            }

            जर (production->has_keysym) {
                गाठ->leaf.keysym = production->keysym;
            } अन्यथा {
                /* Ensure we reset possible previous entry */
                गाठ->leaf.keysym = XKB_किल्ली_No_चिन्ह;
            }

            गाठ->is_leaf = true;
            परत;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

स्थिर xkb_mod_index_t
resolve_modifier(स्थिरांक अक्षर *name)
{
    स्थिर स्थिरांक संरचना {
        स्थिरांक अक्षर *name;
        xkb_mod_index_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    साठी (अचिन्हित पूर्णांक i = 0; i < ARRAY_SIZE(mods); i++)
        जर (streq(name, mods[i].name))
            परत mods[i].mod;

    परत XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
अक्षर *
parse_string_literal(संरचना xkb_context *ctx, स्थिरांक अक्षर *string)
{
    संरचना scanner s;
    संघ lvalue val;
    scanner_init(&s, ctx, string, म_माप(string), "(unnamed)", रिक्त);
    जर (lex(&s, &val) == TOK_STRING) {
        परत strdup(val.string.str);
    } अन्यथा {
        ख_लिखो(मानक_त्रुटि, "ERROR: %s\n", s.s);
        परत रिक्त;
    }
}

स्थिर bool
parse(संरचना xkb_compose_table *table, संरचना scanner *s,
      अचिन्हित पूर्णांक include_depth);

स्थिर bool
do_include(संरचना xkb_compose_table *table, संरचना scanner *s,
           स्थिरांक अक्षर *path, अचिन्हित पूर्णांक include_depth)
{
    खाता *file;
    bool ok;
    अक्षर *string;
    माप_प्रकार size;
    संरचना scanner new_s;

    जर (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        परत false;
    }

    file = ख_खोलो(path, "rb");
    जर (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, म_त्रुटि(त्रुटि_सं));
        परत false;
    }

    ok = map_file(file, &string, &size);
    जर (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, म_त्रुटि(त्रुटि_सं));
        जा err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    जर (!ok)
        जा err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    ख_बंद(file);
    परत ok;
}

// NOLINTBEGIN(readability-trivial-switch)
स्थिर bool
parse(संरचना xkb_compose_table *table, संरचना scanner *s,
      अचिन्हित पूर्णांक include_depth)
{
    गणना rules_token tok;
    संघ lvalue val;
    xkb_keysym_t keysym;
    संरचना production production;
    गणना { MAX_ERRORS = 10 };
    पूर्णांक अंक_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    जर (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        जा fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    निवड (tok = lex(s, &val)) {
    स्थिती TOK_END_OF_ओळ:
        जा initial_eol;
    स्थिती TOK_END_OF_FILE:
        जा finished;
    स्थिती TOK_INCLUDE:
        जा include;
    पूर्वनिर्धारित:
        जा lhs_tok;
    }

include:
    निवड (tok = lex_include_string(s, table, &val)) {
    स्थिती TOK_INCLUDE_STRING:
        जा include_eol;
    पूर्वनिर्धारित:
        जा unexpected;
    }

include_eol:
    निवड (tok = lex(s, &val)) {
    स्थिती TOK_END_OF_ओळ:
        जर (!do_include(table, s, val.string.str, include_depth))
            जा fail;
        जा initial;
    पूर्वनिर्धारित:
        जा unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    निवड (tok) {
    स्थिती TOK_COLON:
        जर (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            जा skip;
        }
        जा rhs;
    स्थिती TOK_IDENT:
        जर (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            जा lhs_keysym;
        }
        जा lhs_mod_list_tok;
    स्थिती TOK_TILDE:
        जा lhs_mod_list_tok;
    स्थिती TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        जा lhs_mod_list;
    पूर्वनिर्धारित:
        जा lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    निवड (tok) {
    स्थिती TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        जर (keysym == XKB_किल्ली_No_चिन्ह) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            जा error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        जर (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            जा skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        जा lhs;
    पूर्वनिर्धारित:
        जा unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_index_t mod;

        जर (tok != TOK_TILDE && tok != TOK_IDENT)
            जा lhs_keysym_tok;

        जर (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        जर (tok != TOK_IDENT)
            जा unexpected;

        mod = resolve_modifier(val.string.str);
        जर (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            जा error;
        }

        production.modmask |= UINT32_C(1) << mod;
        जर (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        अन्यथा
            production.mods |= UINT32_C(1) << mod;

        जा lhs_mod_list;
    }

rhs:
    निवड (tok = lex(s, &val)) {
    स्थिती TOK_STRING:
        जर (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            जा skip;
        }
        जर (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            जा skip;
        }
        जर (val.string.len > आकार(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         आकार(production.string) - 1,
                         val.string.len - 1);
            जा skip;
        }
        स_नकल(production.string, val.string.str, val.string.len);
        production.has_string = true;
        जा rhs;
    स्थिती TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        जर (keysym == XKB_किल्ली_No_चिन्ह) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            जा error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        जर (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            जा skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    स्थिती TOK_END_OF_ओळ:
        जर (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            जा skip;
        }
        add_production(table, s, &production);
        जा initial;
    पूर्वनिर्धारित:
        जा unexpected;
    }

unexpected:
    जर (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    अंक_errors++;
    जर (अंक_errors <= MAX_ERRORS)
        जा skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    जा fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    परत false;

skip:
    पर्यंत (tok != TOK_END_OF_ओळ && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    जा initial;

finished:
    परत true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(संरचना xkb_compose_table *table, स्थिरांक अक्षर *string, माप_प्रकार len,
             स्थिरांक अक्षर *file_name)
{
    संरचना scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, रिक्त);
    जर (!parse(table, &s, 0))
        परत false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    परत true;
}

bool
parse_file(संरचना xkb_compose_table *table, खाता *file, स्थिरांक अक्षर *file_name)
{
    bool ok;
    अक्षर *string;
    माप_प्रकार size;

    ok = map_file(file, &string, &size);
    जर (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, म_त्रुटि(त्रुटि_सं));
        परत false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    परत ok;
}
