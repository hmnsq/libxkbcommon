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

شمار قواعد_token {
    TOK_انتہا_کا_FILE = 0,
    TOK_انتہا_کا_سطر,
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
اتحاد lvalue {
    ساخت {
        /* Still \0-terminated. */
        ثابت حرف *str;
        سائز_t len;
    } string;
};

ساکن شمار قواعد_token
lex(ساخت scanner *s, اتحاد lvalue *val)
{
skip_مزید_whitespace_and_comments:
    /* Skip spaces. */
    جب (is_space(scanner_peek(s)))
        اگر (scanner_اگلا(s) == '\n')
            واپس TOK_انتہا_کا_سطر;

    /* Skip comments. */
    اگر (scanner_chr(s, '#')) {
        scanner_skip_مقصد_eol(s);
        جاؤ skip_مزید_whitespace_and_comments;
    }

    /* See if we're done. */
    اگر (scanner_eof(s)) واپس TOK_انتہا_کا_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    اگر (scanner_chr(s, '<')) {
        جب (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_اگلا(s));
        اگر (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            واپس TOK_ERROR;
        }
        اگر (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            واپس TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        واپس TOK_LHS_KEYSYM;
    }

    /* Colon. */
    اگر (scanner_chr(s, ':'))
        واپس TOK_COLON;
    اگر (scanner_chr(s, '!'))
        واپس TOK_BANG;
    اگر (scanner_chr(s, '~'))
        واپس TOK_TILDE;

    /* String literal. */
    اگر (scanner_chr(s, '\"')) {
        جب (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            اگر (scanner_chr(s, '\\')) {
                uint8_t o;
                سائز_t آغاز_pos = s->pos;
                اگر (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                ورنہ اگر (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                ورنہ اگر (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    اگر (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (حرف) o);
                    } ورنہ {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (صحیح) (s->pos - آغاز_pos + 1),
                                     &s->s[آغاز_pos - 1]);
                    }
                }
                ورنہ اگر (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (حرف) o);
                }
                ورنہ اگر (s->pos > آغاز_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (صحیح) (s->pos - آغاز_pos + 1),
                                 &s->s[آغاز_pos - 1]);
                    /* Ignore. */
                }
                ورنہ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ورنہ {
                scanner_buf_append(s, scanner_اگلا(s));
            }
        }
        اگر (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            واپس TOK_ERROR;
        }
        اگر (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            واپس TOK_ERROR;
        }
        اگر (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            واپس TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        واپس TOK_STRING;
    }

    /* Identifier or include. */
    اگر (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        جب (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_اگلا(s));
        اگر (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            واپس TOK_ERROR;
        }

        اگر (streq(s->buf, "include"))
            واپس TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        واپس TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_مقصد_eol(s);
    واپس TOK_ERROR;
}

ساکن شمار قواعد_token
lex_include_string(ساخت scanner *s, ساخت xkb_compose_جدول *جدول,
                   اتحاد lvalue *val_برآمد)
{
    جب (is_space(scanner_peek(s)))
        اگر (scanner_اگلا(s) == '\n')
            واپس TOK_انتہا_کا_سطر;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    اگر (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        واپس TOK_ERROR;
    }

    جب (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        اگر (scanner_chr(s, '%')) {
            اگر (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            ورنہ اگر (scanner_chr(s, 'H')) {
                ثابت حرف *home = xkb_context_getenv(جدول->ctx, "HOME");
                اگر (!home) {
                    scanner_err(s, XKB_LOG_پیغام_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    واپس TOK_ERROR;
                }
                اگر (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_پیغام_NO_ID,
                                "include path after expanding %%H is too long");
                    واپس TOK_ERROR;
                }
            }
            ورنہ اگر (scanner_chr(s, 'L')) {
                حرف *راستہ = get_locale_compose_file_راستہ(جدول->ctx, جدول->locale);
                اگر (!راستہ) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    واپس TOK_ERROR;
                }
                اگر (!scanner_buf_appends(s, راستہ)) {
                    آزاد(راستہ);
                    scanner_err(s, XKB_LOG_پیغام_NO_ID,
                                "include path after expanding %%L is too long");
                    واپس TOK_ERROR;
                }
                آزاد(راستہ);
            }
            ورنہ اگر (scanner_chr(s, 'S')) {
                ثابت حرف *xlocaledir = get_xlocaledir_راستہ(جدول->ctx);
                اگر (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_پیغام_NO_ID,
                                "include path after expanding %%S is too long");
                    واپس TOK_ERROR;
                }
            }
            ورنہ {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                واپس TOK_ERROR;
            }
        } ورنہ {
            scanner_buf_append(s, scanner_اگلا(s));
        }
    }
    اگر (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        واپس TOK_ERROR;
    }
    اگر (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_پیغام_NO_ID,
                    "include path is too long");
        واپس TOK_ERROR;
    }
    val_برآمد->string.str = s->buf;
    val_برآمد->string.len = s->buf_pos;
    واپس TOK_INCLUDE_STRING;
}

ساخت production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    غیرعلامتی صحیح len;
    xkb_keysym_t keysym;
    حرف string[XKB_COMPOSE_MAX_STRING_سائز];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_نقاب_t modmask;
    xkb_mod_نقاب_t mods;
};

ساکن خالی
جوڑو_production(ساخت xkb_compose_جدول *جدول, ساخت scanner *s,
               ثابت ساخت production *production)
{
    غیرعلامتی صحیح lhs_pos = 0;
    uint32_t curr = darray_سائز(جدول->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = تہی;
    ساخت compose_گرہ *گرہ = تہی;

    /* Warn before potentially going over the limit, discard silently after. */
    اگر (darray_سائز(جدول->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_پیغام_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    اگر (darray_سائز(جدول->nodes) + production->len >= MAX_COMPOSE_NODES)
        واپس;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    جب (true) {
        ثابت xkb_keysym_t keysym = production->lhs[lhs_pos];
        ثابت bool آخری = lhs_pos + 1 == production->len;

        اگر (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            ساخت compose_گرہ new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .اندرونی_2 = {
                    .eqkid = 0,
                    .is_برگ = false,
                },
            };
            curr = darray_سائز(جدول->nodes);
            اگر (pptr != تہی) {
                *pptr = curr;
                pptr = تہی;
            }
            darray_append(جدول->nodes, new);
        }

        گرہ = &darray_شے(جدول->nodes, curr);

        اگر (keysym < گرہ->keysym) {
            pptr = &گرہ->lokid;
            curr = گرہ->lokid;
        } ورنہ اگر (keysym > گرہ->keysym) {
            pptr = &گرہ->hikid;
            curr = گرہ->hikid;
        } ورنہ اگر (!آخری) {
            اگر (گرہ->is_برگ) {
                scanner_warn(s, XKB_LOG_پیغام_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                گرہ->اندرونی_2.eqkid = 0;
                گرہ->اندرونی_2.is_برگ = false;
            }
            lhs_pos++;
            pptr = &گرہ->اندرونی_2.eqkid;
            curr = گرہ->اندرونی_2.eqkid;
        } ورنہ {
            اگر (گرہ->is_برگ) {
                bool same_string =
                    (گرہ->برگ.utf8 == 0 && !production->has_string) ||
                    (
                        گرہ->برگ.utf8 != 0 && production->has_string &&
                        streq(&darray_شے(جدول->utf8, گرہ->برگ.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (گرہ->برگ.keysym == XKB_کلید_No_علامت && !production->has_keysym) ||
                    (
                        گرہ->برگ.keysym != XKB_کلید_No_علامت && production->has_keysym &&
                        گرہ->برگ.keysym == production->keysym
                    );
                اگر (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_پیغام_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    واپس;
                } ورنہ {
                    scanner_warn(s, XKB_LOG_پیغام_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } ورنہ اگر (گرہ->اندرونی_2.eqkid != 0) {
                scanner_warn(s, XKB_LOG_پیغام_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                گرہ->اندرونی_2.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            اگر (production->has_string) {
                ثابت سائز_t len = لڑی_لمبائی(production->string);
                اگر (گرہ->is_برگ && گرہ->برگ.utf8 &&
                    len <= لڑی_لمبائی(&darray_شے(جدول->utf8, گرہ->برگ.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    حافظہ_نقل(&darray_شے(جدول->utf8, گرہ->برگ.utf8),
                           production->string, len + 1);
                } ورنہ {
                    /* Cannot reuse string: allocate new one */
                    گرہ->برگ.utf8 = darray_سائز(جدول->utf8);
                    darray_append_اشیاء(جدول->utf8, production->string,
                                        (darray_سائز_t)لڑی_لمبائی(production->string) + 1);
                }
            } ورنہ {
                /* Ensure we reset possible previous entry */
                گرہ->برگ.utf8 = 0;
            }

            اگر (production->has_keysym) {
                گرہ->برگ.keysym = production->keysym;
            } ورنہ {
                /* Ensure we reset possible previous entry */
                گرہ->برگ.keysym = XKB_کلید_No_علامت;
            }

            گرہ->is_برگ = true;
            واپس;
        }
    }
}

/* Should match resolve_modifier(). */
#تعریف_کرو ALL_MODS_نقاب ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

ساکن xkb_mod_اشاریہ_t
resolve_modifier(ثابت حرف *name)
{
    ساکن ثابت ساخت {
        ثابت حرف *name;
        xkb_mod_اشاریہ_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    برائے (غیرعلامتی صحیح i = 0; i < ARRAY_سائز(mods); i++)
        اگر (streq(name, mods[i].name))
            واپس mods[i].mod;

    واپس XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
حرف *
parse_string_literal(ساخت xkb_context *ctx, ثابت حرف *string)
{
    ساخت scanner s;
    اتحاد lvalue val;
    scanner_init(&s, ctx, string, لڑی_لمبائی(string), "(unnamed)", تہی);
    اگر (lex(&s, &val) == TOK_STRING) {
        واپس strdup(val.string.str);
    } ورنہ {
        fprintf(stderr, "ERROR: %s\n", s.s);
        واپس تہی;
    }
}

ساکن bool
parse(ساخت xkb_compose_جدول *جدول, ساخت scanner *s,
      غیرعلامتی صحیح include_گہرائی);

ساکن bool
do_include(ساخت xkb_compose_جدول *جدول, ساخت scanner *s,
           ثابت حرف *راستہ, غیرعلامتی صحیح include_گہرائی)
{
    FILE *file;
    bool ok;
    حرف *string;
    سائز_t سائز;
    ساخت scanner new_s;

    اگر (include_گہرائی >= COMPOSE_MAX_INCLUDE_گہرائی) {
        scanner_err(s, XKB_LOG_پیغام_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_گہرائی);
        واپس false;
    }

    file = fopen(راستہ, "rb");
    اگر (!file) {
        scanner_err(s, XKB_LOG_پیغام_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    راستہ, strerror(errno));
        واپس false;
    }

    ok = map_file(file, &string, &سائز);
    اگر (!ok) {
        scanner_err(s, XKB_LOG_پیغام_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    راستہ, strerror(errno));
        جاؤ err_file;
    }

    scanner_init(&new_s, جدول->ctx, string, سائز, راستہ, s->priv);

    ok = parse(جدول, &new_s, include_گہرائی + 1);
    اگر (!ok)
        جاؤ err_unmap;

err_unmap:
    unmap_file(string, سائز);
err_file:
    fclose(file);
    واپس ok;
}

// NOLINTBEGIN(readability-trivial-switch)
ساکن bool
parse(ساخت xkb_compose_جدول *جدول, ساخت scanner *s,
      غیرعلامتی صحیح include_گہرائی)
{
    شمار قواعد_token tok;
    اتحاد lvalue val;
    xkb_keysym_t keysym;
    ساخت production production;
    شمار { MAX_ERRORS = 10 };
    صحیح عدد_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    اگر (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        جاؤ fail;
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
    حالت TOK_انتہا_کا_سطر:
        جاؤ initial_eol;
    حالت TOK_انتہا_کا_FILE:
        جاؤ ختم_شدہ;
    حالت TOK_INCLUDE:
        جاؤ include;
    طےشدہ:
        جاؤ lhs_tok;
    }

include:
    انتخاب (tok = lex_include_string(s, جدول, &val)) {
    حالت TOK_INCLUDE_STRING:
        جاؤ include_eol;
    طےشدہ:
        جاؤ unexpected;
    }

include_eol:
    انتخاب (tok = lex(s, &val)) {
    حالت TOK_انتہا_کا_سطر:
        اگر (!do_include(جدول, s, val.string.str, include_گہرائی))
            جاؤ fail;
        جاؤ initial;
    طےشدہ:
        جاؤ unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    انتخاب (tok) {
    حالت TOK_COLON:
        اگر (production.len <= 0) {
            scanner_warn(s, XKB_LOG_پیغام_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            جاؤ skip;
        }
        جاؤ rhs;
    حالت TOK_IDENT:
        اگر (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_نقاب;
            جاؤ lhs_keysym;
        }
        جاؤ lhs_mod_list_tok;
    حالت TOK_TILDE:
        جاؤ lhs_mod_list_tok;
    حالت TOK_BANG:
        production.modmask = ALL_MODS_نقاب;
        جاؤ lhs_mod_list;
    طےشدہ:
        جاؤ lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    انتخاب (tok) {
    حالت TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        اگر (keysym == XKB_کلید_No_علامت) {
            scanner_err(s, XKB_LOG_پیغام_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            جاؤ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        اگر (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            جاؤ skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        جاؤ lhs;
    طےشدہ:
        جاؤ unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_اشاریہ_t mod;

        اگر (tok != TOK_TILDE && tok != TOK_IDENT)
            جاؤ lhs_keysym_tok;

        اگر (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        اگر (tok != TOK_IDENT)
            جاؤ unexpected;

        mod = resolve_modifier(val.string.str);
        اگر (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            جاؤ error;
        }

        production.modmask |= UINT32_C(1) << mod;
        اگر (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        ورنہ
            production.mods |= UINT32_C(1) << mod;

        جاؤ lhs_mod_list;
    }

rhs:
    انتخاب (tok = lex(s, &val)) {
    حالت TOK_STRING:
        اگر (production.has_string) {
            scanner_warn(s, XKB_LOG_پیغام_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            جاؤ skip;
        }
        اگر (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_پیغام_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            جاؤ skip;
        }
        اگر (val.string.len > حجم(production.string)) {
            scanner_warn(s, XKB_LOG_پیغام_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         حجم(production.string) - 1,
                         val.string.len - 1);
            جاؤ skip;
        }
        حافظہ_نقل(production.string, val.string.str, val.string.len);
        production.has_string = true;
        جاؤ rhs;
    حالت TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        اگر (keysym == XKB_کلید_No_علامت) {
            scanner_err(s, XKB_LOG_پیغام_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            جاؤ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        اگر (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            جاؤ skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    حالت TOK_انتہا_کا_سطر:
        اگر (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            جاؤ skip;
        }
        جوڑو_production(جدول, s, &production);
        جاؤ initial;
    طےشدہ:
        جاؤ unexpected;
    }

unexpected:
    اگر (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    عدد_errors++;
    اگر (عدد_errors <= MAX_ERRORS)
        جاؤ skip;

    scanner_err(s, XKB_LOG_پیغام_NO_ID,
                "too many errors");
    جاؤ fail;

fail:
    scanner_err(s, XKB_LOG_پیغام_NO_ID,
                "failed to parse file");
    واپس false;

skip:
    جب (tok != TOK_انتہا_کا_سطر && tok != TOK_انتہا_کا_FILE)
        tok = lex(s, &val);
    جاؤ initial;

ختم_شدہ:
    واپس true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(ساخت xkb_compose_جدول *جدول, ثابت حرف *string, سائز_t len,
             ثابت حرف *file_name)
{
    ساخت scanner s;
    scanner_init(&s, جدول->ctx, string, len, file_name, تہی);
    اگر (!parse(جدول, &s, 0))
        واپس false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(جدول->nodes);
    darray_shrink(جدول->utf8);
    واپس true;
}

bool
parse_file(ساخت xkb_compose_جدول *جدول, FILE *file, ثابت حرف *file_name)
{
    bool ok;
    حرف *string;
    سائز_t سائز;

    ok = map_file(file, &string, &سائز);
    اگر (!ok) {
        log_err(جدول->ctx, XKB_LOG_پیغام_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        واپس false;
    }

    ok = parse_string(جدول, string, سائز, file_name);
    unmap_file(string, سائز);
    واپس ok;
}
