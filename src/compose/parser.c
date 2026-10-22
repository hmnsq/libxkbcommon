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

#شامل "config.h"

#شامل <errno.h>
#شامل <string.h>

#شامل "xkbcommon/xkbcommon-keysyms.h"
#شامل "darray.h"
#شامل "messages-codes.h"
#شامل "utils.h"
#شامل "constants.h"
#شامل "table.h"
#شامل "scanner-utils.h"
#شامل "paths.h"
#شامل "utf8.h"
#شامل "parser.h"
#شامل "keysym.h"

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

شمارش rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_سطر,
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
اجتماع lvalue {
    ساختار {
        /* Still \0-terminated. */
        ثابت نویسه *str;
        size_t len;
    } string;
};

ایستا شمارش rules_token
lex(ساختار scanner *s, اجتماع lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces. */
    وقتی (is_space(scanner_peek(s)))
        اگر (scanner_next(s) == '\n')
            بازگشت TOK_END_OF_سطر;

    /* Skip comments. */
    اگر (scanner_chr(s, '#')) {
        scanner_skip_to_eol(s);
        برو skip_more_whitespace_and_comments;
    }

    /* See if we're done. */
    اگر (scanner_eof(s)) بازگشت TOK_END_OF_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    اگر (scanner_chr(s, '<')) {
        وقتی (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_next(s));
        اگر (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            بازگشت TOK_ERROR;
        }
        اگر (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            بازگشت TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        بازگشت TOK_LHS_KEYSYM;
    }

    /* Colon. */
    اگر (scanner_chr(s, ':'))
        بازگشت TOK_COLON;
    اگر (scanner_chr(s, '!'))
        بازگشت TOK_BANG;
    اگر (scanner_chr(s, '~'))
        بازگشت TOK_TILDE;

    /* String literal. */
    اگر (scanner_chr(s, '\"')) {
        وقتی (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            اگر (scanner_chr(s, '\\')) {
                uint8_t o;
                size_t start_pos = s->pos;
                اگر (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                وگرنه اگر (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                وگرنه اگر (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    اگر (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (نویسه) o);
                    } وگرنه {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (صحیح) (s->pos - start_pos + 1),
                                     &s->s[start_pos - 1]);
                    }
                }
                وگرنه اگر (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (نویسه) o);
                }
                وگرنه اگر (s->pos > start_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (صحیح) (s->pos - start_pos + 1),
                                 &s->s[start_pos - 1]);
                    /* Ignore. */
                }
                وگرنه {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } وگرنه {
                scanner_buf_append(s, scanner_next(s));
            }
        }
        اگر (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            بازگشت TOK_ERROR;
        }
        اگر (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            بازگشت TOK_ERROR;
        }
        اگر (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            بازگشت TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        بازگشت TOK_STRING;
    }

    /* Identifier or include. */
    اگر (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        وقتی (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_next(s));
        اگر (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            بازگشت TOK_ERROR;
        }

        اگر (streq(s->buf, "include"))
            بازگشت TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        بازگشت TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_to_eol(s);
    بازگشت TOK_ERROR;
}

ایستا شمارش rules_token
lex_include_string(ساختار scanner *s, ساختار xkb_compose_table *table,
                   اجتماع lvalue *val_out)
{
    وقتی (is_space(scanner_peek(s)))
        اگر (scanner_next(s) == '\n')
            بازگشت TOK_END_OF_سطر;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    اگر (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        بازگشت TOK_ERROR;
    }

    وقتی (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        اگر (scanner_chr(s, '%')) {
            اگر (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            وگرنه اگر (scanner_chr(s, 'H')) {
                ثابت نویسه *home = xkb_context_getenv(table->ctx, "HOME");
                اگر (!home) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    بازگشت TOK_ERROR;
                }
                اگر (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%H is too long");
                    بازگشت TOK_ERROR;
                }
            }
            وگرنه اگر (scanner_chr(s, 'L')) {
                نویسه *path = get_locale_compose_file_path(table->ctx, table->locale);
                اگر (!path) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    بازگشت TOK_ERROR;
                }
                اگر (!scanner_buf_appends(s, path)) {
                    آزاد(path);
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%L is too long");
                    بازگشت TOK_ERROR;
                }
                آزاد(path);
            }
            وگرنه اگر (scanner_chr(s, 'S')) {
                ثابت نویسه *xlocaledir = get_xlocaledir_path(table->ctx);
                اگر (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                                "include path after expanding %%S is too long");
                    بازگشت TOK_ERROR;
                }
            }
            وگرنه {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                بازگشت TOK_ERROR;
            }
        } وگرنه {
            scanner_buf_append(s, scanner_next(s));
        }
    }
    اگر (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        بازگشت TOK_ERROR;
    }
    اگر (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "include path is too long");
        بازگشت TOK_ERROR;
    }
    val_out->string.str = s->buf;
    val_out->string.len = s->buf_pos;
    بازگشت TOK_INCLUDE_STRING;
}

ساختار production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    unsigned صحیح len;
    xkb_keysym_t keysym;
    نویسه string[XKB_COMPOSE_MAX_STRING_SIZE];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_mask_t modmask;
    xkb_mod_mask_t mods;
};

ایستا تهی
add_production(ساختار xkb_compose_table *table, ساختار scanner *s,
               ثابت ساختار production *production)
{
    unsigned صحیح lhs_pos = 0;
    uint32_t curr = darray_size(table->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = پوچ;
    ساختار compose_گره *گره = پوچ;

    /* Warn before potentially going over the limit, discard silently after. */
    اگر (darray_size(table->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    اگر (darray_size(table->nodes) + production->len >= MAX_COMPOSE_NODES)
        بازگشت;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    وقتی (true) {
        ثابت xkb_keysym_t keysym = production->lhs[lhs_pos];
        ثابت bool last = lhs_pos + 1 == production->len;

        اگر (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            ساختار compose_گره new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .internal = {
                    .eqkid = 0,
                    .is_leaf = false,
                },
            };
            curr = darray_size(table->nodes);
            اگر (pptr != پوچ) {
                *pptr = curr;
                pptr = پوچ;
            }
            darray_append(table->nodes, new);
        }

        گره = &darray_item(table->nodes, curr);

        اگر (keysym < گره->keysym) {
            pptr = &گره->lokid;
            curr = گره->lokid;
        } وگرنه اگر (keysym > گره->keysym) {
            pptr = &گره->hikid;
            curr = گره->hikid;
        } وگرنه اگر (!last) {
            اگر (گره->is_leaf) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                گره->internal.eqkid = 0;
                گره->internal.is_leaf = false;
            }
            lhs_pos++;
            pptr = &گره->internal.eqkid;
            curr = گره->internal.eqkid;
        } وگرنه {
            اگر (گره->is_leaf) {
                bool same_string =
                    (گره->leaf.utf8 == 0 && !production->has_string) ||
                    (
                        گره->leaf.utf8 != 0 && production->has_string &&
                        streq(&darray_item(table->utf8, گره->leaf.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (گره->leaf.keysym == XKB_کلید_No_نماد && !production->has_keysym) ||
                    (
                        گره->leaf.keysym != XKB_کلید_No_نماد && production->has_keysym &&
                        گره->leaf.keysym == production->keysym
                    );
                اگر (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    بازگشت;
                } وگرنه {
                    scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } وگرنه اگر (گره->internal.eqkid != 0) {
                scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                گره->internal.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            اگر (production->has_string) {
                ثابت size_t len = strlen(production->string);
                اگر (گره->is_leaf && گره->leaf.utf8 &&
                    len <= strlen(&darray_item(table->utf8, گره->leaf.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    memcpy(&darray_item(table->utf8, گره->leaf.utf8),
                           production->string, len + 1);
                } وگرنه {
                    /* Cannot reuse string: allocate new one */
                    گره->leaf.utf8 = darray_size(table->utf8);
                    darray_append_اقلام(table->utf8, production->string,
                                        (darray_size_t)strlen(production->string) + 1);
                }
            } وگرنه {
                /* Ensure we reset possible previous entry */
                گره->leaf.utf8 = 0;
            }

            اگر (production->has_keysym) {
                گره->leaf.keysym = production->keysym;
            } وگرنه {
                /* Ensure we reset possible previous entry */
                گره->leaf.keysym = XKB_کلید_No_نماد;
            }

            گره->is_leaf = true;
            بازگشت;
        }
    }
}

/* Should match resolve_modifier(). */
#define ALL_MODS_MASK ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

ایستا xkb_mod_index_t
resolve_modifier(ثابت نویسه *name)
{
    ایستا ثابت ساختار {
        ثابت نویسه *name;
        xkb_mod_index_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    برای (unsigned صحیح i = 0; i < ARRAY_SIZE(mods); i++)
        اگر (streq(name, mods[i].name))
            بازگشت mods[i].mod;

    بازگشت XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
نویسه *
parse_string_literal(ساختار xkb_context *ctx, ثابت نویسه *string)
{
    ساختار scanner s;
    اجتماع lvalue val;
    scanner_init(&s, ctx, string, strlen(string), "(unnamed)", پوچ);
    اگر (lex(&s, &val) == TOK_STRING) {
        بازگشت strdup(val.string.str);
    } وگرنه {
        fprintf(stderr, "ERROR: %s\n", s.s);
        بازگشت پوچ;
    }
}

ایستا bool
parse(ساختار xkb_compose_table *table, ساختار scanner *s,
      unsigned صحیح include_depth);

ایستا bool
do_include(ساختار xkb_compose_table *table, ساختار scanner *s,
           ثابت نویسه *path, unsigned صحیح include_depth)
{
    FILE *file;
    bool ok;
    نویسه *string;
    size_t size;
    ساختار scanner new_s;

    اگر (include_depth >= COMPOSE_MAX_INCLUDE_DEPTH) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_DEPTH);
        بازگشت false;
    }

    file = fopen(path, "rb");
    اگر (!file) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    path, strerror(errno));
        بازگشت false;
    }

    ok = map_file(file, &string, &size);
    اگر (!ok) {
        scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    path, strerror(errno));
        برو err_file;
    }

    scanner_init(&new_s, table->ctx, string, size, path, s->priv);

    ok = parse(table, &new_s, include_depth + 1);
    اگر (!ok)
        برو err_unmap;

err_unmap:
    unmap_file(string, size);
err_file:
    fclose(file);
    بازگشت ok;
}

// NOLINTBEGIN(readability-trivial-switch)
ایستا bool
parse(ساختار xkb_compose_table *table, ساختار scanner *s,
      unsigned صحیح include_depth)
{
    شمارش rules_token tok;
    اجتماع lvalue val;
    xkb_keysym_t keysym;
    ساختار production production;
    شمارش { MAX_ERRORS = 10 };
    صحیح رقم_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    اگر (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        برو fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    انتخاب (tok = lex(s, &val)) {
    حالت TOK_END_OF_سطر:
        برو initial_eol;
    حالت TOK_END_OF_FILE:
        برو finished;
    حالت TOK_INCLUDE:
        برو include;
    پیشفرض:
        برو lhs_tok;
    }

include:
    انتخاب (tok = lex_include_string(s, table, &val)) {
    حالت TOK_INCLUDE_STRING:
        برو include_eol;
    پیشفرض:
        برو unexpected;
    }

include_eol:
    انتخاب (tok = lex(s, &val)) {
    حالت TOK_END_OF_سطر:
        اگر (!do_include(table, s, val.string.str, include_depth))
            برو fail;
        برو initial;
    پیشفرض:
        برو unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    انتخاب (tok) {
    حالت TOK_COLON:
        اگر (production.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            برو skip;
        }
        برو rhs;
    حالت TOK_IDENT:
        اگر (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_MASK;
            برو lhs_keysym;
        }
        برو lhs_mod_list_tok;
    حالت TOK_TILDE:
        برو lhs_mod_list_tok;
    حالت TOK_BANG:
        production.modmask = ALL_MODS_MASK;
        برو lhs_mod_list;
    پیشفرض:
        برو lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    انتخاب (tok) {
    حالت TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        اگر (keysym == XKB_کلید_No_نماد) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            برو error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        اگر (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            برو skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        برو lhs;
    پیشفرض:
        برو unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_index_t mod;

        اگر (tok != TOK_TILDE && tok != TOK_IDENT)
            برو lhs_keysym_tok;

        اگر (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        اگر (tok != TOK_IDENT)
            برو unexpected;

        mod = resolve_modifier(val.string.str);
        اگر (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            برو error;
        }

        production.modmask |= UINT32_C(1) << mod;
        اگر (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        وگرنه
            production.mods |= UINT32_C(1) << mod;

        برو lhs_mod_list;
    }

rhs:
    انتخاب (tok = lex(s, &val)) {
    حالت TOK_STRING:
        اگر (production.has_string) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            برو skip;
        }
        اگر (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            برو skip;
        }
        اگر (val.string.len > اندازه(production.string)) {
            scanner_warn(s, XKB_LOG_MESSAGE_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         اندازه(production.string) - 1,
                         val.string.len - 1);
            برو skip;
        }
        memcpy(production.string, val.string.str, val.string.len);
        production.has_string = true;
        برو rhs;
    حالت TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        اگر (keysym == XKB_کلید_No_نماد) {
            scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            برو error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        اگر (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            برو skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    حالت TOK_END_OF_سطر:
        اگر (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            برو skip;
        }
        add_production(table, s, &production);
        برو initial;
    پیشفرض:
        برو unexpected;
    }

unexpected:
    اگر (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    رقم_errors++;
    اگر (رقم_errors <= MAX_ERRORS)
        برو skip;

    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "too many errors");
    برو fail;

fail:
    scanner_err(s, XKB_LOG_MESSAGE_NO_ID,
                "failed to parse file");
    بازگشت false;

skip:
    وقتی (tok != TOK_END_OF_سطر && tok != TOK_END_OF_FILE)
        tok = lex(s, &val);
    برو initial;

finished:
    بازگشت true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(ساختار xkb_compose_table *table, ثابت نویسه *string, size_t len,
             ثابت نویسه *file_name)
{
    ساختار scanner s;
    scanner_init(&s, table->ctx, string, len, file_name, پوچ);
    اگر (!parse(table, &s, 0))
        بازگشت false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(table->nodes);
    darray_shrink(table->utf8);
    بازگشت true;
}

bool
parse_file(ساختار xkb_compose_table *table, FILE *file, ثابت نویسه *file_name)
{
    bool ok;
    نویسه *string;
    size_t size;

    ok = map_file(file, &string, &size);
    اگر (!ok) {
        log_err(table->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        بازگشت false;
    }

    ok = parse_string(table, string, size, file_name);
    unmap_file(string, size);
    بازگشت ok;
}
