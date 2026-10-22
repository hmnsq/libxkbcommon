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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "xkbcommon/xkbcommon-keysyms.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "constants.h"
#அந்தர்பாவ "table.h"
#அந்தர்பாவ "scanner-utils.h"
#அந்தர்பாவ "paths.h"
#அந்தர்பாவ "utf8.h"
#அந்தர்பாவ "parser.h"
#அந்தர்பாவ "keysym.h"

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

எண்ணிக்கை rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_வரி,
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
ஒன்றிப்பு lvalue {
    கட்டமைப்பு {
        /* Still \0-terminated. */
        மாறிலி எழுத்து *str;
        size_t len;
    } string;
};

நிலையான எண்ணிக்கை rules_token
lex(கட்டமைப்பு scanner *s, ஒன்றிப்பு lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    போது (is_space(scanner_peek(s)))
        எனில் (scanner_next(s) == '\n')
            திரும்பு TOK_END_OF_வரி;

    /* Skip comments. */
    எனில் (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        செல் skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    எனில் (scanner_eof(s)) திரும்பு TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    எனில் (scanner_chr(s, '<')) {
        போது (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_next(s));
        எனில் (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            திரும்பு TOK_ERROR;
        }
        எனில் (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            திரும்பு TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        திரும்பு TOK_LHS_KEYSYM;
    }

    /* Colon. */
    எனில் (scanner_chr(s, ':'))
        திரும்பு TOK_COLON;
    எனில் (scanner_chr(s, '!'))
        திரும்பு TOK_BANG;
    எனில் (scanner_chr(s, '~'))
        திரும்பு TOK_TILDE;

    /* String literal. */
    எனில் (scanner_chr(s, '\"')) {
        போது (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            எனில் (scanner_chr(s, '\\')) {
                uint8_t o;
                size_t start_pos = s->pos;
                எனில் (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                இல்லையெனில் எனில் (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                இல்லையெனில் எனில் (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    எனில் (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (எழுத்து) o);
                    } இல்லையெனில் {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (முழு) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                இல்லையெனில் எனில் (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (எழுத்து) o);
                }
                இல்லையெனில் எனில் (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (முழு) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                இல்லையெனில் {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } இல்லையெனில் {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        எனில் (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            திரும்பு TOK_ERROR;
        }
        எனில் (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            திரும்பு TOK_ERROR;
        }
        எனில் (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            திரும்பு TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        திரும்பு TOK_STRING;
    }

    /* Identifier or include. */
    எனில் (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        போது (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_next(s));
        எனில் (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            திரும்பு TOK_ERROR;
        }

        எனில் (streq(s->buf, "include"))
            திரும்பு TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        திரும்பு TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    திரும்பு TOK_ERROR;
}

நிலையான எண்ணிக்கை rules_token
lex_include_string(கட்டமைப்பு scanner *s, கட்டமைப்பு xkb_compose_table *table,
                   ஒன்றிப்பு lvalue *val_out)
{
    போது (is_space(scanner_peek(s)))
        எனில் (scanner_next(s) == '\n')
            திரும்பு TOK_END_OF_வரி;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    எனில் (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        திரும்பு TOK_ERROR;
    }

    போது (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        எனில் (scanner_chr(s, '%')) {
            எனில் (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            இல்லையெனில் எனில் (scanner_chr(s, 'H')) {
                மாறிலி எழுத்து *home = xkb_context_getenv(table->ctx, "HOME");
                எனில் (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    திரும்பு TOK_ERROR;
                }
                எனில் (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    திரும்பு TOK_ERROR;
                }
            }
            இல்லையெனில் எனில் (scanner_chr(s, 'L')) {
                எழுத்து *path = get_locale_compose_file_path(table->ctx, table->locale);
                எனில் (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    திரும்பு TOK_ERROR;
                }
                எனில் (!scanner_buf_appends(s, path)) {
                    விடுவி(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    திரும்பு TOK_ERROR;
                }
                விடுவி(path);
            }
            இல்லையெனில் எனில் (scanner_chr(s, 'S')) {
                மாறிலி எழுத்து *xlocaledir = get_xlocaledir_path(table->ctx);
                எனில் (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    திரும்பு TOK_ERROR;
                }
            }
            இல்லையெனில் {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                திரும்பு TOK_ERROR;
            }
        } இல்லையெனில் {
            scanner_buf_append(s, scanner_next(s));
        }
    }
    எனில் (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        திரும்பு TOK_ERROR;
    }
    எனில் (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        திரும்பு TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    திரும்பு TOK_INCLUDE_STRING;
}

கட்டமைப்பு production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    unsigned முழு len;
    xkb_keysym_t keysym;
    எழுத்து string[XKB_COMPOSE_MAX_STRING_SIZE];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

நிலையான வெறுமை
add_production(கட்டமைப்பு xkb_compose_table *table, கட்டமைப்பு scanner *s,
               மாறிலி கட்டமைப்பு production *production)
{
    unsigned முழு lhs_pos = 0;
    uint32_t curr = darray_size(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = வெற்று;
    கட்டமைப்பு compose_முனை *முனை = வெற்று;

    /* Warn before potentially going over the limit, discard silently after. */
    எனில் (darray_size(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    எனில் (darray_size(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        திரும்பு;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    போது (true) {
        மாறிலி xkb_keysym_t keysym = production->lhs[lhs_pos];
        மாறிலி bool last = lhs_pos + 1 == production->len;

        எனில் (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            கட்டமைப்பு compose_முனை new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_size(table->nodes);
            எனில் (pptr != வெற்று) {
                *pptr = curr;
                pptr = வெற்று;
            }
            darray_append(table->nodes, new);
        }

        முனை = &darray_item(table->nodes, curr);

        எனில் (keysym < முனை->keysym) {
            pptr = &முனை->lokid;
            curr = முனை->lokid;
        } இல்லையெனில் எனில் (keysym > முனை->keysym) {
            pptr = &முனை->hikid;
            curr = முனை->hikid;
        } இல்லையெனில் எனில் (!last) {
            எனில் (முனை->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                முனை->internal.eqkid = 0;
                முனை->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &முனை->internal.eqkid;
            curr = முனை->internal.eqkid;
        } இல்லையெனில் {
            எனில் (முனை->is_leaf) {
                bool same_string =
                    (முனை->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        முனை->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, முனை->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (முனை->leaf.keysym == XKB_சாவி_No_குறியீடு && !production->has_keysym) ||
                    (
                        முனை->leaf.keysym != XKB_சாவி_No_குறியீடு && production->has_keysym &&
                        முனை->leaf.keysym == production->keysym
                    );
                எனில் (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    திரும்பு;
                } இல்லையெனில் {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } இல்லையெனில் எனில் (முனை->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                முனை->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            எனில் (production->has_string) {
                மாறிலி size_t len = strlen(production->string);
                எனில் (முனை->is_leaf && முனை->leaf.utf8 &&
                    len <= strlen(&darray_item(table->utf8, முனை->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    memcpy(&darray_item(table->utf8, முனை->leaf.utf8),
                           production->string, len + 1);
                } இல்லையெனில் {
                    /* Cannot reuse string: allocate new one */
                    முனை->leaf.utf8 = darray_size(table->utf8);
                    darray_append_பொருட்கள்(table->utf8, production->string,
                                        (darray_size_t)strlen(production->string) + 1);
                }
            } இல்லையெனில் {
                /* Ensure we reset possible previous entry */
                முனை->leaf.utf8 = 0;
            }

            எனில் (production->has_keysym) {
                முனை->leaf.keysym = production->keysym;
            } இல்லையெனில் {
                /* Ensure we reset possible previous entry */
                முனை->leaf.keysym = XKB_சாவி_No_குறியீடு;
            }

            முனை->is_leaf = true;
            திரும்பு;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

நிலையான xkb_mod_index_t
resolve_modifier(மாறிலி எழுத்து *name)
{
    நிலையான மாறிலி கட்டமைப்பு {
        மாறிலி எழுத்து *name;
        xkb_mod_index_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    க்கு (unsigned முழு i = 0; i < ARRAY_SIZE(mods); i++)
        எனில் (streq(name, mods[i].name))
            திரும்பு mods[i].mod;

    திரும்பு XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
எழுத்து *
parse_string_literal(கட்டமைப்பு xkb_context *ctx, மாறிலி எழுத்து *string)
{
    கட்டமைப்பு scanner s;
    ஒன்றிப்பு lvalue val;
    scanner_init(&s, ctx, string, strlen(string), "(unnamed)", வெற்று);
    எனில் (lex(&s, &val) == TOK_STRING) {
        திரும்பு strdup(val.string.str);
    } இல்லையெனில் {
        fprintf(stderr, "ERROR: %s\n", s.s);
        திரும்பு வெற்று;
    }
}

நிலையான bool
parse(கட்டமைப்பு xkb_compose_table *table, கட்டமைப்பு scanner *s,
      unsigned முழு include_depth);

நிலையான bool
do_include(கட்டமைப்பு xkb_compose_table *table, கட்டமைப்பு scanner *s,
           மாறிலி எழுத்து *path, unsigned முழு include_depth)
{
    FILE *file;
    bool ok;
    எழுத்து *string;
    size_t size;
    கட்டமைப்பு scanner new_s;

    எனில் (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        திரும்பு false;
    }

    file = fopen(path, "rb");
    எனில் (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        திரும்பு false;
    }

    ok = map_file(file, &string, &size);
    எனில் (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        செல் err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    எனில் (!ok)
        செல் err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    fclose(file);
    திரும்பு ok;
}

// NOLINTBEGIN(readability-trivial-switch)
நிலையான bool
parse(கட்டமைப்பு xkb_compose_table *table, கட்டமைப்பு scanner *s,
      unsigned முழு include_depth)
{
    எண்ணிக்கை rules_token tok;
    ஒன்றிப்பு lvalue val;
    xkb_keysym_t keysym;
    கட்டமைப்பு production production;
    எண்ணிக்கை { MAX_ERRORS = 10 };
    முழு இலக்கம்_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    எனில் (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        செல் fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    தேர்வு (tok = lex(s, &val)) {
    நிலை TOK_END_OF_வரி:
        செல் initial_eol;
    நிலை TOK_END_OF_FILE:
        செல் finished;
    நிலை TOK_INCLUDE:
        செல் include;
    இயல்பு:
        செல் lhs_tok;
    }

include:
    தேர்வு (tok = lex_include_string(s, table, &val)) {
    நிலை TOK_INCLUDE_STRING:
        செல் include_eol;
    இயல்பு:
        செல் unexpected;
    }

include_eol:
    தேர்வு (tok = lex(s, &val)) {
    நிலை TOK_END_OF_வரி:
        எனில் (!do_include(table, s, val.string.str, include_depth))
            செல் fail;
        செல் initial;
    இயல்பு:
        செல் unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    தேர்வு (tok) {
    நிலை TOK_COLON:
        எனில் (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            செல் skip;
        }
        செல் rhs;
    நிலை TOK_IDENT:
        எனில் (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            செல் lhs_keysym;
        }
        செல் lhs_mod_list_tok;
    நிலை TOK_TILDE:
        செல் lhs_mod_list_tok;
    நிலை TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        செல் lhs_mod_list;
    இயல்பு:
        செல் lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    தேர்வு (tok) {
    நிலை TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        எனில் (keysym == XKB_சாவி_No_குறியீடு) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            செல் error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        எனில் (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            செல் skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        செல் lhs;
    இயல்பு:
        செல் unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_index_t mod;

        எனில் (tok != TOK_TILDE && tok != TOK_IDENT)
            செல் lhs_keysym_tok;

        எனில் (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        எனில் (tok != TOK_IDENT)
            செல் unexpected;

        mod = resolve_modifier(val.string.str);
        எனில் (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            செல் error;
        }

        production.modmask |= UINT32_C(1) << mod;
        எனில் (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        இல்லையெனில்
            production.mods |= UINT32_C(1) << mod;

        செல் lhs_mod_list;
    }

rhs:
    தேர்வு (tok = lex(s, &val)) {
    நிலை TOK_STRING:
        எனில் (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            செல் skip;
        }
        எனில் (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            செல் skip;
        }
        எனில் (val.string.len > அளவு(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         அளவு(production.string) - 1,
                         val.string.len - 1);
            செல் skip;
        }
        memcpy(production.string, val.string.str, val.string.len);
        production.has_string = true;
        செல் rhs;
    நிலை TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        எனில் (keysym == XKB_சாவி_No_குறியீடு) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            செல் error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        எனில் (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            செல் skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    நிலை TOK_END_OF_வரி:
        எனில் (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            செல் skip;
        }
        add_production(table, s, &production);
        செல் initial;
    இயல்பு:
        செல் unexpected;
    }

unexpected:
    எனில் (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    இலக்கம்_errors++;
    எனில் (இலக்கம்_errors <= MAX_ERRORS)
        செல் skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    செல் fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    திரும்பு false;

skip:
    போது (tok != TOK_END_OF_வரி && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    செல் initial;

finished:
    திரும்பு true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(கட்டமைப்பு xkb_compose_table *table, மாறிலி எழுத்து *string, size_t len,
             மாறிலி எழுத்து *file_name)
{
    கட்டமைப்பு scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, வெற்று);
    எனில் (!parse(table, &s, 0))
        திரும்பு false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    திரும்பு true;
}

bool
parse_file(கட்டமைப்பு xkb_compose_table *table, FILE *file, மாறிலி எழுத்து *file_name)
{
    bool ok;
    எழுத்து *string;
    size_t size;

    ok = map_file(file, &string, &size);
    எனில் (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        திரும்பு false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    திரும்பு ok;
}
