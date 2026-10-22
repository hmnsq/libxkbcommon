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

गणना नियमाः_token {
    TOK_अन्तः_सम्बन्धि_FILE = 0,
    TOK_अन्तः_सम्बन्धि_पङ्क्तिः,
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
सङ्घ lvalue {
    संरचना {
        /* Still \0-terminated. */
        ध्रुव वर्ण *str;
        आकार_t len;
    } string;
};

स्थिर गणना नियमाः_token
lex(संरचना scanner *s, सङ्घ lvalue *val)
{
skip_अधिकम्_whitespace_and_comments:
    /* Skip spaces. */
    यावत् (is_space(scanner_peek(s)))
        यदि (scanner_अग्र(s) == '\n')
            प्रत्यागम TOK_अन्तः_सम्बन्धि_पङ्क्तिः;

    /* Skip comments. */
    यदि (scanner_chr(s, '#')) {
        scanner_skip_गन्तव्यम्_eol(s);
        गच्छ skip_अधिकम्_whitespace_and_comments;
    }

    /* See if we're done. */
    यदि (scanner_eof(s)) प्रत्यागम TOK_अन्तः_सम्बन्धि_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    यदि (scanner_chr(s, '<')) {
        यावत् (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_अग्र(s));
        यदि (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            प्रत्यागम TOK_ERROR;
        }
        यदि (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            प्रत्यागम TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        प्रत्यागम TOK_LHS_KEYSYM;
    }

    /* Colon. */
    यदि (scanner_chr(s, ':'))
        प्रत्यागम TOK_COLON;
    यदि (scanner_chr(s, '!'))
        प्रत्यागम TOK_BANG;
    यदि (scanner_chr(s, '~'))
        प्रत्यागम TOK_TILDE;

    /* String literal. */
    यदि (scanner_chr(s, '\"')) {
        यावत् (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            यदि (scanner_chr(s, '\\')) {
                uint8_t o;
                आकार_t start_pos = s->pos;
                यदि (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                अन्यथा यदि (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                अन्यथा यदि (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    यदि (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (वर्ण) o);
                    } अन्यथा {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (पूर्णाङ्क) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                अन्यथा यदि (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (वर्ण) o);
                }
                अन्यथा यदि (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (पूर्णाङ्क) (s->pos - start_pos + 1),
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
        यदि (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            प्रत्यागम TOK_ERROR;
        }
        यदि (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            प्रत्यागम TOK_ERROR;
        }
        यदि (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            प्रत्यागम TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        प्रत्यागम TOK_STRING;
    }

    /* Identifier or include. */
    यदि (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        यावत् (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_अग्र(s));
        यदि (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            प्रत्यागम TOK_ERROR;
        }

        यदि (streq(s->buf, "include"))
            प्रत्यागम TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        प्रत्यागम TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_गन्तव्यम्_eol(s);
    प्रत्यागम TOK_ERROR;
}

स्थिर गणना नियमाः_token
lex_include_string(संरचना scanner *s, संरचना xkb_compose_सारणी *सारणी,
                   सङ्घ lvalue *val_बहिर्गमनम्)
{
    यावत् (is_space(scanner_peek(s)))
        यदि (scanner_अग्र(s) == '\n')
            प्रत्यागम TOK_अन्तः_सम्बन्धि_पङ्क्तिः;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    यदि (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        प्रत्यागम TOK_ERROR;
    }

    यावत् (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        यदि (scanner_chr(s, '%')) {
            यदि (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            अन्यथा यदि (scanner_chr(s, 'H')) {
                ध्रुव वर्ण *home = xkb_context_getenv(सारणी->ctx, "HOME");
                यदि (!home) {
                    scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    प्रत्यागम TOK_ERROR;
                }
                यदि (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                                "include path after expanding %%H is too long");
                    प्रत्यागम TOK_ERROR;
                }
            }
            अन्यथा यदि (scanner_chr(s, 'L')) {
                वर्ण *मार्गः = get_locale_compose_file_मार्गः(सारणी->ctx, सारणी->locale);
                यदि (!मार्गः) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    प्रत्यागम TOK_ERROR;
                }
                यदि (!scanner_buf_appends(s, मार्गः)) {
                    मुक्त(मार्गः);
                    scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                                "include path after expanding %%L is too long");
                    प्रत्यागम TOK_ERROR;
                }
                मुक्त(मार्गः);
            }
            अन्यथा यदि (scanner_chr(s, 'S')) {
                ध्रुव वर्ण *xlocaledir = get_xlocaledir_मार्गः(सारणी->ctx);
                यदि (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                                "include path after expanding %%S is too long");
                    प्रत्यागम TOK_ERROR;
                }
            }
            अन्यथा {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                प्रत्यागम TOK_ERROR;
            }
        } अन्यथा {
            scanner_buf_append(s, scanner_अग्र(s));
        }
    }
    यदि (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        प्रत्यागम TOK_ERROR;
    }
    यदि (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                    "include path is too long");
        प्रत्यागम TOK_ERROR;
    }
    val_बहिर्गमनम्->string.str = s->buf;
    val_बहिर्गमनम्->string.len = s->buf_pos;
    प्रत्यागम TOK_INCLUDE_STRING;
}

संरचना production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    अचिह्नित पूर्णाङ्क len;
    xkb_keysym_t keysym;
    वर्ण string[XKB_COMPOSE_MAX_STRING_आकार];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_आवरणम्_t modmask;
    xkb_mod_आवरणम्_t mods;
};

स्थिर शून्य
सङ्कलय_production(संरचना xkb_compose_सारणी *सारणी, संरचना scanner *s,
               ध्रुव संरचना production *production)
{
    अचिह्नित पूर्णाङ्क lhs_pos = 0;
    uint32_t curr = darray_आकार(सारणी->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = रिक्त;
    संरचना compose_ग्रन्थिः *ग्रन्थिः = रिक्त;

    /* Warn before potentially going over the limit, discard silently after. */
    यदि (darray_आकार(सारणी->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_सन्देशः_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    यदि (darray_आकार(सारणी->nodes) + production->len >= MAX_COMPOSE_NODES)
        प्रत्यागम;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    यावत् (true) {
        ध्रुव xkb_keysym_t keysym = production->lhs[lhs_pos];
        ध्रुव bool चरमः = lhs_pos + 1 == production->len;

        यदि (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            संरचना compose_ग्रन्थिः new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .आन्तरिकम् = {
                    .eqkid = 0,
                    .is_पर्णम् = false,
                },
            };
            curr = darray_आकार(सारणी->nodes);
            यदि (pptr != रिक्त) {
                *pptr = curr;
                pptr = रिक्त;
            }
            darray_append(सारणी->nodes, new);
        }

        ग्रन्थिः = &darray_item(सारणी->nodes, curr);

        यदि (keysym < ग्रन्थिः->keysym) {
            pptr = &ग्रन्थिः->lokid;
            curr = ग्रन्थिः->lokid;
        } अन्यथा यदि (keysym > ग्रन्थिः->keysym) {
            pptr = &ग्रन्थिः->hikid;
            curr = ग्रन्थिः->hikid;
        } अन्यथा यदि (!चरमः) {
            यदि (ग्रन्थिः->is_पर्णम्) {
                scanner_warn(s, XKB_LOG_सन्देशः_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                ग्रन्थिः->आन्तरिकम्.eqkid = 0;
                ग्रन्थिः->आन्तरिकम्.is_पर्णम् = false;
            }
            lhs_pos++;
            pptr = &ग्रन्थिः->आन्तरिकम्.eqkid;
            curr = ग्रन्थिः->आन्तरिकम्.eqkid;
        } अन्यथा {
            यदि (ग्रन्थिः->is_पर्णम्) {
                bool same_string =
                    (ग्रन्थिः->पर्णम्.utf8 == 0 && !production->has_string) ||
                    (
                        ग्रन्थिः->पर्णम्.utf8 != 0 && production->has_string &&
                        streq(&darray_item(सारणी->utf8, ग्रन्थिः->पर्णम्.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (ग्रन्थिः->पर्णम्.keysym == XKB_कुञ्चिका_No_चिह्नम् && !production->has_keysym) ||
                    (
                        ग्रन्थिः->पर्णम्.keysym != XKB_कुञ्चिका_No_चिह्नम् && production->has_keysym &&
                        ग्रन्थिः->पर्णम्.keysym == production->keysym
                    );
                यदि (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_सन्देशः_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    प्रत्यागम;
                } अन्यथा {
                    scanner_warn(s, XKB_LOG_सन्देशः_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } अन्यथा यदि (ग्रन्थिः->आन्तरिकम्.eqkid != 0) {
                scanner_warn(s, XKB_LOG_सन्देशः_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                ग्रन्थिः->आन्तरिकम्.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            यदि (production->has_string) {
                ध्रुव आकार_t len = वाक्यदैर्घ्यम्(production->string);
                यदि (ग्रन्थिः->is_पर्णम् && ग्रन्थिः->पर्णम्.utf8 &&
                    len <= वाक्यदैर्घ्यम्(&darray_item(सारणी->utf8, ग्रन्थिः->पर्णम्.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    स्मृतिं_प्रतिलिख(&darray_item(सारणी->utf8, ग्रन्थिः->पर्णम्.utf8),
                           production->string, len + 1);
                } अन्यथा {
                    /* Cannot reuse string: allocate new one */
                    ग्रन्थिः->पर्णम्.utf8 = darray_आकार(सारणी->utf8);
                    darray_append_वस्तूनि(सारणी->utf8, production->string,
                                        (darray_आकार_t)वाक्यदैर्घ्यम्(production->string) + 1);
                }
            } अन्यथा {
                /* Ensure we reset possible previous entry */
                ग्रन्थिः->पर्णम्.utf8 = 0;
            }

            यदि (production->has_keysym) {
                ग्रन्थिः->पर्णम्.keysym = production->keysym;
            } अन्यथा {
                /* Ensure we reset possible previous entry */
                ग्रन्थिः->पर्णम्.keysym = XKB_कुञ्चिका_No_चिह्नम्;
            }

            ग्रन्थिः->is_पर्णम् = true;
            प्रत्यागम;
        }
    }
}

/* Should match resolve_modifier(). */
#परिभाषय ALL_MODS_आवरणम् ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

स्थिर xkb_mod_सूचकांक_t
resolve_modifier(ध्रुव वर्ण *name)
{
    स्थिर ध्रुव संरचना {
        ध्रुव वर्ण *name;
        xkb_mod_सूचकांक_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    प्रति (अचिह्नित पूर्णाङ्क i = 0; i < ARRAY_आकार(mods); i++)
        यदि (streq(name, mods[i].name))
            प्रत्यागम mods[i].mod;

    प्रत्यागम XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
वर्ण *
parse_string_literal(संरचना xkb_context *ctx, ध्रुव वर्ण *string)
{
    संरचना scanner s;
    सङ्घ lvalue val;
    scanner_init(&s, ctx, string, वाक्यदैर्घ्यम्(string), "(unnamed)", रिक्त);
    यदि (lex(&s, &val) == TOK_STRING) {
        प्रत्यागम strdup(val.string.str);
    } अन्यथा {
        fprintf(stderr, "ERROR: %s\n", s.s);
        प्रत्यागम रिक्त;
    }
}

स्थिर bool
parse(संरचना xkb_compose_सारणी *सारणी, संरचना scanner *s,
      अचिह्नित पूर्णाङ्क include_गभीरता);

स्थिर bool
do_include(संरचना xkb_compose_सारणी *सारणी, संरचना scanner *s,
           ध्रुव वर्ण *मार्गः, अचिह्नित पूर्णाङ्क include_गभीरता)
{
    FILE *file;
    bool ok;
    वर्ण *string;
    आकार_t आकार;
    संरचना scanner new_s;

    यदि (include_गभीरता >= COMPOSE_MAX_INCLUDE_गभीरता) {
        scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_गभीरता);
        प्रत्यागम false;
    }

    file = fopen(मार्गः, "rb");
    यदि (!file) {
        scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    मार्गः, strerror(errno));
        प्रत्यागम false;
    }

    ok = map_file(file, &string, &आकार);
    यदि (!ok) {
        scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    मार्गः, strerror(errno));
        गच्छ err_file;
    }

    scanner_init(&new_s, सारणी->ctx, string, आकार, मार्गः, s->priv);

    ok = parse(सारणी, &new_s, include_गभीरता + 1);
    यदि (!ok)
        गच्छ err_unmap;

err_unmap:
    unmap_file(string, आकार);
err_file:
    fclose(file);
    प्रत्यागम ok;
}

// NOLINTBEGIN(readability-trivial-switch)
स्थिर bool
parse(संरचना xkb_compose_सारणी *सारणी, संरचना scanner *s,
      अचिह्नित पूर्णाङ्क include_गभीरता)
{
    गणना नियमाः_token tok;
    सङ्घ lvalue val;
    xkb_keysym_t keysym;
    संरचना production production;
    गणना { MAX_ERRORS = 10 };
    पूर्णाङ्क अङ्कः_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    यदि (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        गच्छ fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    विकल्प (tok = lex(s, &val)) {
    पक्ष TOK_अन्तः_सम्बन्धि_पङ्क्तिः:
        गच्छ initial_eol;
    पक्ष TOK_अन्तः_सम्बन्धि_FILE:
        गच्छ परिसमाप्तम्;
    पक्ष TOK_INCLUDE:
        गच्छ include;
    स्वभाव:
        गच्छ lhs_tok;
    }

include:
    विकल्प (tok = lex_include_string(s, सारणी, &val)) {
    पक्ष TOK_INCLUDE_STRING:
        गच्छ include_eol;
    स्वभाव:
        गच्छ unexpected;
    }

include_eol:
    विकल्प (tok = lex(s, &val)) {
    पक्ष TOK_अन्तः_सम्बन्धि_पङ्क्तिः:
        यदि (!do_include(सारणी, s, val.string.str, include_गभीरता))
            गच्छ fail;
        गच्छ initial;
    स्वभाव:
        गच्छ unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    विकल्प (tok) {
    पक्ष TOK_COLON:
        यदि (production.len <= 0) {
            scanner_warn(s, XKB_LOG_सन्देशः_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            गच्छ skip;
        }
        गच्छ rhs;
    पक्ष TOK_IDENT:
        यदि (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_आवरणम्;
            गच्छ lhs_keysym;
        }
        गच्छ lhs_mod_list_tok;
    पक्ष TOK_TILDE:
        गच्छ lhs_mod_list_tok;
    पक्ष TOK_BANG:
        production.modmask = ALL_MODS_आवरणम्;
        गच्छ lhs_mod_list;
    स्वभाव:
        गच्छ lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    विकल्प (tok) {
    पक्ष TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        यदि (keysym == XKB_कुञ्चिका_No_चिह्नम्) {
            scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            गच्छ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        यदि (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            गच्छ skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        गच्छ lhs;
    स्वभाव:
        गच्छ unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_सूचकांक_t mod;

        यदि (tok != TOK_TILDE && tok != TOK_IDENT)
            गच्छ lhs_keysym_tok;

        यदि (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        यदि (tok != TOK_IDENT)
            गच्छ unexpected;

        mod = resolve_modifier(val.string.str);
        यदि (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            गच्छ error;
        }

        production.modmask |= UINT32_C(1) << mod;
        यदि (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        अन्यथा
            production.mods |= UINT32_C(1) << mod;

        गच्छ lhs_mod_list;
    }

rhs:
    विकल्प (tok = lex(s, &val)) {
    पक्ष TOK_STRING:
        यदि (production.has_string) {
            scanner_warn(s, XKB_LOG_सन्देशः_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            गच्छ skip;
        }
        यदि (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_सन्देशः_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            गच्छ skip;
        }
        यदि (val.string.len > परिमाण(production.string)) {
            scanner_warn(s, XKB_LOG_सन्देशः_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         परिमाण(production.string) - 1,
                         val.string.len - 1);
            गच्छ skip;
        }
        स्मृतिं_प्रतिलिख(production.string, val.string.str, val.string.len);
        production.has_string = true;
        गच्छ rhs;
    पक्ष TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        यदि (keysym == XKB_कुञ्चिका_No_चिह्नम्) {
            scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            गच्छ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        यदि (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            गच्छ skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    पक्ष TOK_अन्तः_सम्बन्धि_पङ्क्तिः:
        यदि (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            गच्छ skip;
        }
        सङ्कलय_production(सारणी, s, &production);
        गच्छ initial;
    स्वभाव:
        गच्छ unexpected;
    }

unexpected:
    यदि (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    अङ्कः_errors++;
    यदि (अङ्कः_errors <= MAX_ERRORS)
        गच्छ skip;

    scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                "too many errors");
    गच्छ fail;

fail:
    scanner_err(s, XKB_LOG_सन्देशः_NO_ID,
                "failed to parse file");
    प्रत्यागम false;

skip:
    यावत् (tok != TOK_अन्तः_सम्बन्धि_पङ्क्तिः && tok != TOK_अन्तः_सम्बन्धि_FILE)
        tok = lex(s, &val);
    गच्छ initial;

परिसमाप्तम्:
    प्रत्यागम true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(संरचना xkb_compose_सारणी *सारणी, ध्रुव वर्ण *string, आकार_t len,
             ध्रुव वर्ण *file_name)
{
    संरचना scanner s;
    scanner_init(&s, सारणी->ctx, string, len, file_name, रिक्त);
    यदि (!parse(सारणी, &s, 0))
        प्रत्यागम false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(सारणी->nodes);
    darray_shrink(सारणी->utf8);
    प्रत्यागम true;
}

bool
parse_file(संरचना xkb_compose_सारणी *सारणी, FILE *file, ध्रुव वर्ण *file_name)
{
    bool ok;
    वर्ण *string;
    आकार_t आकार;

    ok = map_file(file, &string, &आकार);
    यदि (!ok) {
        log_err(सारणी->ctx, XKB_LOG_सन्देशः_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        प्रत्यागम false;
    }

    ok = parse_string(सारणी, string, आकार, file_name);
    unmap_file(string, आकार);
    प्रत्यागम ok;
}
