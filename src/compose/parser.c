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

#ضمن "config.h"

#ضمن <errno.h>
#ضمن <string.h>

#ضمن "xkbcommon/xkbcommon-keysyms.h"
#ضمن "darray.h"
#ضمن "messages-codes.h"
#ضمن "utils.h"
#ضمن "constants.h"
#ضمن "table.h"
#ضمن "scanner-utils.h"
#ضمن "paths.h"
#ضمن "utf8.h"
#ضمن "parser.h"
#ضمن "keysym.h"

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

تعداد قواعد_token {
    TOK_انتها_از_FILE = 0,
    TOK_انتها_از_سطر,
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
    بنية {
        /* Still \0-terminated. */
        ثابت حرف *str;
        اندازه_t len;
    } string;
};

ساكن تعداد قواعد_token
lex(بنية scanner *s, اتحاد lvalue *val)
{
skip_بیشتر_whitespace_and_comments:
    /* Skip spaces. */
    طالما (is_space(scanner_peek(s)))
        إذا (scanner_بعدی(s) == '\n')
            أرجع TOK_انتها_از_سطر;

    /* Skip comments. */
    إذا (scanner_chr(s, '#')) {
        scanner_skip_مقصد_eol(s);
        اذهب skip_بیشتر_whitespace_and_comments;
    }

    /* See if we're done. */
    إذا (scanner_eof(s)) أرجع TOK_انتها_از_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    إذا (scanner_chr(s, '<')) {
        طالما (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_بعدی(s));
        إذا (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            أرجع TOK_ERROR;
        }
        إذا (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            أرجع TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        أرجع TOK_LHS_KEYSYM;
    }

    /* Colon. */
    إذا (scanner_chr(s, ':'))
        أرجع TOK_COLON;
    إذا (scanner_chr(s, '!'))
        أرجع TOK_BANG;
    إذا (scanner_chr(s, '~'))
        أرجع TOK_TILDE;

    /* String literal. */
    إذا (scanner_chr(s, '\"')) {
        طالما (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            إذا (scanner_chr(s, '\\')) {
                uint8_t o;
                اندازه_t شروع_pos = s->pos;
                إذا (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                وإلا إذا (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                وإلا إذا (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    إذا (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (حرف) o);
                    } وإلا {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (صحيح) (s->pos - شروع_pos + 1),
                                     &s->s[شروع_pos - 1]);
                    }
                }
                وإلا إذا (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (حرف) o);
                }
                وإلا إذا (s->pos > شروع_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (صحيح) (s->pos - شروع_pos + 1),
                                 &s->s[شروع_pos - 1]);
                    /* Ignore. */
                }
                وإلا {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } وإلا {
                scanner_buf_append(s, scanner_بعدی(s));
            }
        }
        إذا (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            أرجع TOK_ERROR;
        }
        إذا (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            أرجع TOK_ERROR;
        }
        إذا (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            أرجع TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        أرجع TOK_STRING;
    }

    /* Identifier or include. */
    إذا (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        طالما (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_بعدی(s));
        إذا (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            أرجع TOK_ERROR;
        }

        إذا (streq(s->buf, "include"))
            أرجع TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        أرجع TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_مقصد_eol(s);
    أرجع TOK_ERROR;
}

ساكن تعداد قواعد_token
lex_include_string(بنية scanner *s, بنية xkb_compose_جدول *جدول,
                   اتحاد lvalue *val_بیرون)
{
    طالما (is_space(scanner_peek(s)))
        إذا (scanner_بعدی(s) == '\n')
            أرجع TOK_انتها_از_سطر;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    إذا (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        أرجع TOK_ERROR;
    }

    طالما (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        إذا (scanner_chr(s, '%')) {
            إذا (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            وإلا إذا (scanner_chr(s, 'H')) {
                ثابت حرف *home = xkb_context_getenv(جدول->ctx, "HOME");
                إذا (!home) {
                    scanner_err(s, XKB_LOG_پیام_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    أرجع TOK_ERROR;
                }
                إذا (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_پیام_NO_ID,
                                "include path after expanding %%H is too long");
                    أرجع TOK_ERROR;
                }
            }
            وإلا إذا (scanner_chr(s, 'L')) {
                حرف *مسیر = get_locale_compose_file_مسیر(جدول->ctx, جدول->locale);
                إذا (!مسیر) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    أرجع TOK_ERROR;
                }
                إذا (!scanner_buf_appends(s, مسیر)) {
                    حرر(مسیر);
                    scanner_err(s, XKB_LOG_پیام_NO_ID,
                                "include path after expanding %%L is too long");
                    أرجع TOK_ERROR;
                }
                حرر(مسیر);
            }
            وإلا إذا (scanner_chr(s, 'S')) {
                ثابت حرف *xlocaledir = get_xlocaledir_مسیر(جدول->ctx);
                إذا (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_پیام_NO_ID,
                                "include path after expanding %%S is too long");
                    أرجع TOK_ERROR;
                }
            }
            وإلا {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                أرجع TOK_ERROR;
            }
        } وإلا {
            scanner_buf_append(s, scanner_بعدی(s));
        }
    }
    إذا (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        أرجع TOK_ERROR;
    }
    إذا (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_پیام_NO_ID,
                    "include path is too long");
        أرجع TOK_ERROR;
    }
    val_بیرون->string.str = s->buf;
    val_بیرون->string.len = s->buf_pos;
    أرجع TOK_INCLUDE_STRING;
}

بنية production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    بیعلامت صحيح len;
    xkb_keysym_t keysym;
    حرف string[XKB_COMPOSE_MAX_STRING_اندازه];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_ماسک_t modmask;
    xkb_mod_ماسک_t mods;
};

ساكن فراغ
جمع_کن_production(بنية xkb_compose_جدول *جدول, بنية scanner *s,
               ثابت بنية production *production)
{
    بیعلامت صحيح lhs_pos = 0;
    uint32_t curr = darray_اندازه(جدول->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = عدم;
    بنية compose_گره *گره = عدم;

    /* Warn before potentially going over the limit, discard silently after. */
    إذا (darray_اندازه(جدول->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_پیام_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    إذا (darray_اندازه(جدول->nodes) + production->len >= MAX_COMPOSE_NODES)
        أرجع;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    طالما (true) {
        ثابت xkb_keysym_t keysym = production->lhs[lhs_pos];
        ثابت bool آخرین = lhs_pos + 1 == production->len;

        إذا (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            بنية compose_گره new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .داخلی = {
                    .eqkid = 0,
                    .is_برگ = false,
                },
            };
            curr = darray_اندازه(جدول->nodes);
            إذا (pptr != عدم) {
                *pptr = curr;
                pptr = عدم;
            }
            darray_append(جدول->nodes, new);
        }

        گره = &darray_قلم(جدول->nodes, curr);

        إذا (keysym < گره->keysym) {
            pptr = &گره->lokid;
            curr = گره->lokid;
        } وإلا إذا (keysym > گره->keysym) {
            pptr = &گره->hikid;
            curr = گره->hikid;
        } وإلا إذا (!آخرین) {
            إذا (گره->is_برگ) {
                scanner_warn(s, XKB_LOG_پیام_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                گره->داخلی.eqkid = 0;
                گره->داخلی.is_برگ = false;
            }
            lhs_pos++;
            pptr = &گره->داخلی.eqkid;
            curr = گره->داخلی.eqkid;
        } وإلا {
            إذا (گره->is_برگ) {
                bool same_string =
                    (گره->برگ.utf8 == 0 && !production->has_string) ||
                    (
                        گره->برگ.utf8 != 0 && production->has_string &&
                        streq(&darray_قلم(جدول->utf8, گره->برگ.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (گره->برگ.keysym == XKB_کلید_No_نماد && !production->has_keysym) ||
                    (
                        گره->برگ.keysym != XKB_کلید_No_نماد && production->has_keysym &&
                        گره->برگ.keysym == production->keysym
                    );
                إذا (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_پیام_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    أرجع;
                } وإلا {
                    scanner_warn(s, XKB_LOG_پیام_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } وإلا إذا (گره->داخلی.eqkid != 0) {
                scanner_warn(s, XKB_LOG_پیام_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                گره->داخلی.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            إذا (production->has_string) {
                ثابت اندازه_t len = طول_رشته(production->string);
                إذا (گره->is_برگ && گره->برگ.utf8 &&
                    len <= طول_رشته(&darray_قلم(جدول->utf8, گره->برگ.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    کپی_حافظه(&darray_قلم(جدول->utf8, گره->برگ.utf8),
                           production->string, len + 1);
                } وإلا {
                    /* Cannot reuse string: allocate new one */
                    گره->برگ.utf8 = darray_اندازه(جدول->utf8);
                    darray_append_اقلام(جدول->utf8, production->string,
                                        (darray_اندازه_t)طول_رشته(production->string) + 1);
                }
            } وإلا {
                /* Ensure we reset possible previous entry */
                گره->برگ.utf8 = 0;
            }

            إذا (production->has_keysym) {
                گره->برگ.keysym = production->keysym;
            } وإلا {
                /* Ensure we reset possible previous entry */
                گره->برگ.keysym = XKB_کلید_No_نماد;
            }

            گره->is_برگ = true;
            أرجع;
        }
    }
}

/* Should match resolve_modifier(). */
#تعریف_کن ALL_MODS_ماسک ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

ساكن xkb_mod_شاخص_t
resolve_modifier(ثابت حرف *name)
{
    ساكن ثابت بنية {
        ثابت حرف *name;
        xkb_mod_شاخص_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    لكل (بیعلامت صحيح i = 0; i < ARRAY_اندازه(mods); i++)
        إذا (streq(name, mods[i].name))
            أرجع mods[i].mod;

    أرجع XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
حرف *
parse_string_literal(بنية xkb_context *ctx, ثابت حرف *string)
{
    بنية scanner s;
    اتحاد lvalue val;
    scanner_init(&s, ctx, string, طول_رشته(string), "(unnamed)", عدم);
    إذا (lex(&s, &val) == TOK_STRING) {
        أرجع strdup(val.string.str);
    } وإلا {
        fprintf(stderr, "ERROR: %s\n", s.s);
        أرجع عدم;
    }
}

ساكن bool
parse(بنية xkb_compose_جدول *جدول, بنية scanner *s,
      بیعلامت صحيح include_عمق);

ساكن bool
do_include(بنية xkb_compose_جدول *جدول, بنية scanner *s,
           ثابت حرف *مسیر, بیعلامت صحيح include_عمق)
{
    FILE *file;
    bool ok;
    حرف *string;
    اندازه_t اندازه;
    بنية scanner new_s;

    إذا (include_عمق >= COMPOSE_MAX_INCLUDE_عمق) {
        scanner_err(s, XKB_LOG_پیام_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_عمق);
        أرجع false;
    }

    file = fopen(مسیر, "rb");
    إذا (!file) {
        scanner_err(s, XKB_LOG_پیام_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    مسیر, strerror(errno));
        أرجع false;
    }

    ok = map_file(file, &string, &اندازه);
    إذا (!ok) {
        scanner_err(s, XKB_LOG_پیام_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    مسیر, strerror(errno));
        اذهب err_file;
    }

    scanner_init(&new_s, جدول->ctx, string, اندازه, مسیر, s->priv);

    ok = parse(جدول, &new_s, include_عمق + 1);
    إذا (!ok)
        اذهب err_unmap;

err_unmap:
    unmap_file(string, اندازه);
err_file:
    fclose(file);
    أرجع ok;
}

// NOLINTBEGIN(readability-trivial-switch)
ساكن bool
parse(بنية xkb_compose_جدول *جدول, بنية scanner *s,
      بیعلامت صحيح include_عمق)
{
    تعداد قواعد_token tok;
    اتحاد lvalue val;
    xkb_keysym_t keysym;
    بنية production production;
    تعداد { MAX_ERRORS = 10 };
    صحيح رقم_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    إذا (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        اذهب fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    بدل (tok = lex(s, &val)) {
    حالة TOK_انتها_از_سطر:
        اذهب initial_eol;
    حالة TOK_انتها_از_FILE:
        اذهب پایانیافته;
    حالة TOK_INCLUDE:
        اذهب include;
    افتراضي:
        اذهب lhs_tok;
    }

include:
    بدل (tok = lex_include_string(s, جدول, &val)) {
    حالة TOK_INCLUDE_STRING:
        اذهب include_eol;
    افتراضي:
        اذهب unexpected;
    }

include_eol:
    بدل (tok = lex(s, &val)) {
    حالة TOK_انتها_از_سطر:
        إذا (!do_include(جدول, s, val.string.str, include_عمق))
            اذهب fail;
        اذهب initial;
    افتراضي:
        اذهب unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    بدل (tok) {
    حالة TOK_COLON:
        إذا (production.len <= 0) {
            scanner_warn(s, XKB_LOG_پیام_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            اذهب skip;
        }
        اذهب rhs;
    حالة TOK_IDENT:
        إذا (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_ماسک;
            اذهب lhs_keysym;
        }
        اذهب lhs_mod_list_tok;
    حالة TOK_TILDE:
        اذهب lhs_mod_list_tok;
    حالة TOK_BANG:
        production.modmask = ALL_MODS_ماسک;
        اذهب lhs_mod_list;
    افتراضي:
        اذهب lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    بدل (tok) {
    حالة TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        إذا (keysym == XKB_کلید_No_نماد) {
            scanner_err(s, XKB_LOG_پیام_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            اذهب error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        إذا (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            اذهب skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        اذهب lhs;
    افتراضي:
        اذهب unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_شاخص_t mod;

        إذا (tok != TOK_TILDE && tok != TOK_IDENT)
            اذهب lhs_keysym_tok;

        إذا (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        إذا (tok != TOK_IDENT)
            اذهب unexpected;

        mod = resolve_modifier(val.string.str);
        إذا (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            اذهب error;
        }

        production.modmask |= UINT32_C(1) << mod;
        إذا (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        وإلا
            production.mods |= UINT32_C(1) << mod;

        اذهب lhs_mod_list;
    }

rhs:
    بدل (tok = lex(s, &val)) {
    حالة TOK_STRING:
        إذا (production.has_string) {
            scanner_warn(s, XKB_LOG_پیام_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            اذهب skip;
        }
        إذا (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_پیام_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            اذهب skip;
        }
        إذا (val.string.len > حجم(production.string)) {
            scanner_warn(s, XKB_LOG_پیام_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         حجم(production.string) - 1,
                         val.string.len - 1);
            اذهب skip;
        }
        کپی_حافظه(production.string, val.string.str, val.string.len);
        production.has_string = true;
        اذهب rhs;
    حالة TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        إذا (keysym == XKB_کلید_No_نماد) {
            scanner_err(s, XKB_LOG_پیام_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            اذهب error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        إذا (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            اذهب skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    حالة TOK_انتها_از_سطر:
        إذا (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            اذهب skip;
        }
        جمع_کن_production(جدول, s, &production);
        اذهب initial;
    افتراضي:
        اذهب unexpected;
    }

unexpected:
    إذا (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    رقم_errors++;
    إذا (رقم_errors <= MAX_ERRORS)
        اذهب skip;

    scanner_err(s, XKB_LOG_پیام_NO_ID,
                "too many errors");
    اذهب fail;

fail:
    scanner_err(s, XKB_LOG_پیام_NO_ID,
                "failed to parse file");
    أرجع false;

skip:
    طالما (tok != TOK_انتها_از_سطر && tok != TOK_انتها_از_FILE)
        tok = lex(s, &val);
    اذهب initial;

پایانیافته:
    أرجع true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(بنية xkb_compose_جدول *جدول, ثابت حرف *string, اندازه_t len,
             ثابت حرف *file_name)
{
    بنية scanner s;
    scanner_init(&s, جدول->ctx, string, len, file_name, عدم);
    إذا (!parse(جدول, &s, 0))
        أرجع false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(جدول->nodes);
    darray_shrink(جدول->utf8);
    أرجع true;
}

bool
parse_file(بنية xkb_compose_جدول *جدول, FILE *file, ثابت حرف *file_name)
{
    bool ok;
    حرف *string;
    اندازه_t اندازه;

    ok = map_file(file, &string, &اندازه);
    إذا (!ok) {
        log_err(جدول->ctx, XKB_LOG_پیام_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        أرجع false;
    }

    ok = parse_string(جدول, string, اندازه, file_name);
    unmap_file(string, اندازه);
    أرجع ok;
}
