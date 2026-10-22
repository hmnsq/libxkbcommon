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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-keysyms.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "constants.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "table.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "scanner-utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "paths.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parser.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keysym.h"

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

ਗਿਣਤੀ ਨਿਯਮਾਂ_token {
    TOK_ਸਿਰਾ_ਦਾ_FILE = 0,
    TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ,
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
ਸੰਘ lvalue {
    ਬਣਤਰ {
        /* Still \0-terminated. */
        ਅਚਲ ਅੱਖਰ *str;
        ਮਾਪ_t len;
    } string;
};

ਸਥਿਰ ਗਿਣਤੀ ਨਿਯਮਾਂ_token
lex(ਬਣਤਰ scanner *s, ਸੰਘ lvalue *val)
{
skip_ਹੋਰ_whitespace_and_comments:
    /* Skip spaces. */
    ਜਦੋਂ (is_space(scanner_peek(s)))
        ਜੇ (scanner_ਅਗਲਾ(s) == '\n')
            ਵਾਪਸ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ;

    /* Skip comments. */
    ਜੇ (scanner_chr(s, '#')) {
        scanner_skip_ਮੰਜ਼ਿਲ_eol(s);
        ਜਾਓ skip_ਹੋਰ_whitespace_and_comments;
    }

    /* See if we're done. */
    ਜੇ (scanner_eof(s)) ਵਾਪਸ TOK_ਸਿਰਾ_ਦਾ_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    ਜੇ (scanner_chr(s, '<')) {
        ਜਦੋਂ (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_ਅਗਲਾ(s));
        ਜੇ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            ਵਾਪਸ TOK_ERROR;
        }
        ਜੇ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            ਵਾਪਸ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ਵਾਪਸ TOK_LHS_KEYSYM;
    }

    /* Colon. */
    ਜੇ (scanner_chr(s, ':'))
        ਵਾਪਸ TOK_COLON;
    ਜੇ (scanner_chr(s, '!'))
        ਵਾਪਸ TOK_BANG;
    ਜੇ (scanner_chr(s, '~'))
        ਵਾਪਸ TOK_TILDE;

    /* String literal. */
    ਜੇ (scanner_chr(s, '\"')) {
        ਜਦੋਂ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ਜੇ (scanner_chr(s, '\\')) {
                uint8_t o;
                ਮਾਪ_t ਸ਼ੁਰੂ_pos = s->pos;
                ਜੇ (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                ਨਹੀਂਤਾਂ ਜੇ (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                ਨਹੀਂਤਾਂ ਜੇ (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    ਜੇ (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (ਅੱਖਰ) o);
                    } ਨਹੀਂਤਾਂ {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (ਪੂਰਨ) (s->pos - ਸ਼ੁਰੂ_pos + 1),
                                     &s->s[ਸ਼ੁਰੂ_pos - 1]);
                    }
                }
                ਨਹੀਂਤਾਂ ਜੇ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (ਅੱਖਰ) o);
                }
                ਨਹੀਂਤਾਂ ਜੇ (s->pos > ਸ਼ੁਰੂ_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (ਪੂਰਨ) (s->pos - ਸ਼ੁਰੂ_pos + 1),
                                 &s->s[ਸ਼ੁਰੂ_pos - 1]);
                    /* Ignore. */
                }
                ਨਹੀਂਤਾਂ {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } ਨਹੀਂਤਾਂ {
                scanner_buf_append(s, scanner_ਅਗਲਾ(s));
            }
        }
        ਜੇ (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            ਵਾਪਸ TOK_ERROR;
        }
        ਜੇ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            ਵਾਪਸ TOK_ERROR;
        }
        ਜੇ (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            ਵਾਪਸ TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ਵਾਪਸ TOK_STRING;
    }

    /* Identifier or include. */
    ਜੇ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        ਜਦੋਂ (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_ਅਗਲਾ(s));
        ਜੇ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            ਵਾਪਸ TOK_ERROR;
        }

        ਜੇ (streq(s->buf, "include"))
            ਵਾਪਸ TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        ਵਾਪਸ TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_ਮੰਜ਼ਿਲ_eol(s);
    ਵਾਪਸ TOK_ERROR;
}

ਸਥਿਰ ਗਿਣਤੀ ਨਿਯਮਾਂ_token
lex_include_string(ਬਣਤਰ scanner *s, ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ,
                   ਸੰਘ lvalue *val_ਬਾਹਰਲਾ)
{
    ਜਦੋਂ (is_space(scanner_peek(s)))
        ਜੇ (scanner_ਅਗਲਾ(s) == '\n')
            ਵਾਪਸ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    ਜੇ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        ਵਾਪਸ TOK_ERROR;
    }

    ਜਦੋਂ (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        ਜੇ (scanner_chr(s, '%')) {
            ਜੇ (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            ਨਹੀਂਤਾਂ ਜੇ (scanner_chr(s, 'H')) {
                ਅਚਲ ਅੱਖਰ *home = xkb_context_getenv(ਸਾਰਣੀ->ctx, "HOME");
                ਜੇ (!home) {
                    scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    ਵਾਪਸ TOK_ERROR;
                }
                ਜੇ (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                "include path after expanding %%H is too long");
                    ਵਾਪਸ TOK_ERROR;
                }
            }
            ਨਹੀਂਤਾਂ ਜੇ (scanner_chr(s, 'L')) {
                ਅੱਖਰ *ਰਸਤਾ = get_locale_compose_file_ਰਸਤਾ(ਸਾਰਣੀ->ctx, ਸਾਰਣੀ->locale);
                ਜੇ (!ਰਸਤਾ) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    ਵਾਪਸ TOK_ERROR;
                }
                ਜੇ (!scanner_buf_appends(s, ਰਸਤਾ)) {
                    ਮੁਕਤ(ਰਸਤਾ);
                    scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                "include path after expanding %%L is too long");
                    ਵਾਪਸ TOK_ERROR;
                }
                ਮੁਕਤ(ਰਸਤਾ);
            }
            ਨਹੀਂਤਾਂ ਜੇ (scanner_chr(s, 'S')) {
                ਅਚਲ ਅੱਖਰ *xlocaledir = get_xlocaledir_ਰਸਤਾ(ਸਾਰਣੀ->ctx);
                ਜੇ (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                "include path after expanding %%S is too long");
                    ਵਾਪਸ TOK_ERROR;
                }
            }
            ਨਹੀਂਤਾਂ {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                ਵਾਪਸ TOK_ERROR;
            }
        } ਨਹੀਂਤਾਂ {
            scanner_buf_append(s, scanner_ਅਗਲਾ(s));
        }
    }
    ਜੇ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        ਵਾਪਸ TOK_ERROR;
    }
    ਜੇ (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "include path is too long");
        ਵਾਪਸ TOK_ERROR;
    }
    val_ਬਾਹਰਲਾ->string.str = s->buf;
    val_ਬਾਹਰਲਾ->string.len = s->buf_pos;
    ਵਾਪਸ TOK_INCLUDE_STRING;
}

ਬਣਤਰ production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len;
    xkb_keysym_t keysym;
    ਅੱਖਰ string[XKB_COMPOSE_MAX_STRING_ਮਾਪ];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_ਨਕਾਬ_t modmask;
    xkb_mod_ਨਕਾਬ_t mods;
};

ਸਥਿਰ ਖਾਲੀ
ਜੋੜੋ_production(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ, ਬਣਤਰ scanner *s,
               ਅਚਲ ਬਣਤਰ production *production)
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lhs_pos = 0;
    uint32_t curr = darray_ਮਾਪ(ਸਾਰਣੀ->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = ਨਿਰਾ;
    ਬਣਤਰ compose_ਗੰਢ *ਗੰਢ = ਨਿਰਾ;

    /* Warn before potentially going over the limit, discard silently after. */
    ਜੇ (darray_ਮਾਪ(ਸਾਰਣੀ->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    ਜੇ (darray_ਮਾਪ(ਸਾਰਣੀ->nodes) + production->len >= MAX_COMPOSE_NODES)
        ਵਾਪਸ;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    ਜਦੋਂ (true) {
        ਅਚਲ xkb_keysym_t keysym = production->lhs[lhs_pos];
        ਅਚਲ bool ਆਖਰੀ = lhs_pos + 1 == production->len;

        ਜੇ (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            ਬਣਤਰ compose_ਗੰਢ new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .ਅੰਦਰੂਨੀ = {
                    .eqkid = 0,
                    .is_ਪੱਤੀ = false,
                },
            };
            curr = darray_ਮਾਪ(ਸਾਰਣੀ->nodes);
            ਜੇ (pptr != ਨਿਰਾ) {
                *pptr = curr;
                pptr = ਨਿਰਾ;
            }
            darray_append(ਸਾਰਣੀ->nodes, new);
        }

        ਗੰਢ = &darray_ਵਸਤੂ(ਸਾਰਣੀ->nodes, curr);

        ਜੇ (keysym < ਗੰਢ->keysym) {
            pptr = &ਗੰਢ->lokid;
            curr = ਗੰਢ->lokid;
        } ਨਹੀਂਤਾਂ ਜੇ (keysym > ਗੰਢ->keysym) {
            pptr = &ਗੰਢ->hikid;
            curr = ਗੰਢ->hikid;
        } ਨਹੀਂਤਾਂ ਜੇ (!ਆਖਰੀ) {
            ਜੇ (ਗੰਢ->is_ਪੱਤੀ) {
                scanner_warn(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                ਗੰਢ->ਅੰਦਰੂਨੀ.eqkid = 0;
                ਗੰਢ->ਅੰਦਰੂਨੀ.is_ਪੱਤੀ = false;
            }
            lhs_pos++;
            pptr = &ਗੰਢ->ਅੰਦਰੂਨੀ.eqkid;
            curr = ਗੰਢ->ਅੰਦਰੂਨੀ.eqkid;
        } ਨਹੀਂਤਾਂ {
            ਜੇ (ਗੰਢ->is_ਪੱਤੀ) {
                bool same_string =
                    (ਗੰਢ->ਪੱਤੀ.utf8 == 0 && !production->has_string) ||
                    (
                        ਗੰਢ->ਪੱਤੀ.utf8 != 0 && production->has_string &&
                        streq(&darray_ਵਸਤੂ(ਸਾਰਣੀ->utf8, ਗੰਢ->ਪੱਤੀ.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (ਗੰਢ->ਪੱਤੀ.keysym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ && !production->has_keysym) ||
                    (
                        ਗੰਢ->ਪੱਤੀ.keysym != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ && production->has_keysym &&
                        ਗੰਢ->ਪੱਤੀ.keysym == production->keysym
                    );
                ਜੇ (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    ਵਾਪਸ;
                } ਨਹੀਂਤਾਂ {
                    scanner_warn(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } ਨਹੀਂਤਾਂ ਜੇ (ਗੰਢ->ਅੰਦਰੂਨੀ.eqkid != 0) {
                scanner_warn(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                ਗੰਢ->ਅੰਦਰੂਨੀ.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            ਜੇ (production->has_string) {
                ਅਚਲ ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(production->string);
                ਜੇ (ਗੰਢ->is_ਪੱਤੀ && ਗੰਢ->ਪੱਤੀ.utf8 &&
                    len <= ਲੜੀ_ਲੰਬਾਈ(&darray_ਵਸਤੂ(ਸਾਰਣੀ->utf8, ਗੰਢ->ਪੱਤੀ.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ(&darray_ਵਸਤੂ(ਸਾਰਣੀ->utf8, ਗੰਢ->ਪੱਤੀ.utf8),
                           production->string, len + 1);
                } ਨਹੀਂਤਾਂ {
                    /* Cannot reuse string: allocate new one */
                    ਗੰਢ->ਪੱਤੀ.utf8 = darray_ਮਾਪ(ਸਾਰਣੀ->utf8);
                    darray_append_ਚੀਜ਼ਾਂ(ਸਾਰਣੀ->utf8, production->string,
                                        (darray_ਮਾਪ_t)ਲੜੀ_ਲੰਬਾਈ(production->string) + 1);
                }
            } ਨਹੀਂਤਾਂ {
                /* Ensure we reset possible previous entry */
                ਗੰਢ->ਪੱਤੀ.utf8 = 0;
            }

            ਜੇ (production->has_keysym) {
                ਗੰਢ->ਪੱਤੀ.keysym = production->keysym;
            } ਨਹੀਂਤਾਂ {
                /* Ensure we reset possible previous entry */
                ਗੰਢ->ਪੱਤੀ.keysym = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
            }

            ਗੰਢ->is_ਪੱਤੀ = true;
            ਵਾਪਸ;
        }
    }
}

/* Should match resolve_modifier(). */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALL_MODS_ਨਕਾਬ ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

ਸਥਿਰ xkb_mod_ਸੂਚਕਾਂਕ_t
resolve_modifier(ਅਚਲ ਅੱਖਰ *name)
{
    ਸਥਿਰ ਅਚਲ ਬਣਤਰ {
        ਅਚਲ ਅੱਖਰ *name;
        xkb_mod_ਸੂਚਕਾਂਕ_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ARRAY_ਮਾਪ(mods); i++)
        ਜੇ (streq(name, mods[i].name))
            ਵਾਪਸ mods[i].mod;

    ਵਾਪਸ XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
ਅੱਖਰ *
parse_string_literal(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਅੱਖਰ *string)
{
    ਬਣਤਰ scanner s;
    ਸੰਘ lvalue val;
    scanner_init(&s, ctx, string, ਲੜੀ_ਲੰਬਾਈ(string), "(unnamed)", ਨਿਰਾ);
    ਜੇ (lex(&s, &val) == TOK_STRING) {
        ਵਾਪਸ strdup(val.string.str);
    } ਨਹੀਂਤਾਂ {
        fprintf(stderr, "ERROR: %s\n", s.s);
        ਵਾਪਸ ਨਿਰਾ;
    }
}

ਸਥਿਰ bool
parse(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ, ਬਣਤਰ scanner *s,
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ);

ਸਥਿਰ bool
do_include(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ, ਬਣਤਰ scanner *s,
           ਅਚਲ ਅੱਖਰ *ਰਸਤਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ)
{
    FILE *file;
    bool ok;
    ਅੱਖਰ *string;
    ਮਾਪ_t ਮਾਪ;
    ਬਣਤਰ scanner new_s;

    ਜੇ (include_ਡੂੰਘਾਈ >= COMPOSE_MAX_INCLUDE_ਡੂੰਘਾਈ) {
        scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_ਡੂੰਘਾਈ);
        ਵਾਪਸ false;
    }

    file = fopen(ਰਸਤਾ, "rb");
    ਜੇ (!file) {
        scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    ਰਸਤਾ, strerror(errno));
        ਵਾਪਸ false;
    }

    ok = map_file(file, &string, &ਮਾਪ);
    ਜੇ (!ok) {
        scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    ਰਸਤਾ, strerror(errno));
        ਜਾਓ err_file;
    }

    scanner_init(&new_s, ਸਾਰਣੀ->ctx, string, ਮਾਪ, ਰਸਤਾ, s->priv);

    ok = parse(ਸਾਰਣੀ, &new_s, include_ਡੂੰਘਾਈ + 1);
    ਜੇ (!ok)
        ਜਾਓ err_unmap;

err_unmap:
    unmap_file(string, ਮਾਪ);
err_file:
    fclose(file);
    ਵਾਪਸ ok;
}

// NOLINTBEGIN(readability-trivial-switch)
ਸਥਿਰ bool
parse(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ, ਬਣਤਰ scanner *s,
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ)
{
    ਗਿਣਤੀ ਨਿਯਮਾਂ_token tok;
    ਸੰਘ lvalue val;
    xkb_keysym_t keysym;
    ਬਣਤਰ production production;
    ਗਿਣਤੀ { MAX_ERRORS = 10 };
    ਪੂਰਨ ਅੰਕ_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    ਜੇ (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        ਜਾਓ fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    ਚੋਣ (tok = lex(s, &val)) {
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ:
        ਜਾਓ initial_eol;
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_FILE:
        ਜਾਓ ਮੁਕੰਮਲ;
    ਹਾਲਤ TOK_INCLUDE:
        ਜਾਓ include;
    ਮੂਲ:
        ਜਾਓ lhs_tok;
    }

include:
    ਚੋਣ (tok = lex_include_string(s, ਸਾਰਣੀ, &val)) {
    ਹਾਲਤ TOK_INCLUDE_STRING:
        ਜਾਓ include_eol;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

include_eol:
    ਚੋਣ (tok = lex(s, &val)) {
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ:
        ਜੇ (!do_include(ਸਾਰਣੀ, s, val.string.str, include_ਡੂੰਘਾਈ))
            ਜਾਓ fail;
        ਜਾਓ initial;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    ਚੋਣ (tok) {
    ਹਾਲਤ TOK_COLON:
        ਜੇ (production.len <= 0) {
            scanner_warn(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            ਜਾਓ skip;
        }
        ਜਾਓ rhs;
    ਹਾਲਤ TOK_IDENT:
        ਜੇ (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_ਨਕਾਬ;
            ਜਾਓ lhs_keysym;
        }
        ਜਾਓ lhs_mod_list_tok;
    ਹਾਲਤ TOK_TILDE:
        ਜਾਓ lhs_mod_list_tok;
    ਹਾਲਤ TOK_BANG:
        production.modmask = ALL_MODS_ਨਕਾਬ;
        ਜਾਓ lhs_mod_list;
    ਮੂਲ:
        ਜਾਓ lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    ਚੋਣ (tok) {
    ਹਾਲਤ TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ਜੇ (keysym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
            scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            ਜਾਓ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ਜੇ (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            ਜਾਓ skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        ਜਾਓ lhs;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_ਸੂਚਕਾਂਕ_t mod;

        ਜੇ (tok != TOK_TILDE && tok != TOK_IDENT)
            ਜਾਓ lhs_keysym_tok;

        ਜੇ (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        ਜੇ (tok != TOK_IDENT)
            ਜਾਓ unexpected;

        mod = resolve_modifier(val.string.str);
        ਜੇ (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            ਜਾਓ error;
        }

        production.modmask |= UINT32_C(1) << mod;
        ਜੇ (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        ਨਹੀਂਤਾਂ
            production.mods |= UINT32_C(1) << mod;

        ਜਾਓ lhs_mod_list;
    }

rhs:
    ਚੋਣ (tok = lex(s, &val)) {
    ਹਾਲਤ TOK_STRING:
        ਜੇ (production.has_string) {
            scanner_warn(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            ਜਾਓ skip;
        }
        ਜੇ (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            ਜਾਓ skip;
        }
        ਜੇ (val.string.len > ਆਕਾਰ(production.string)) {
            scanner_warn(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         ਆਕਾਰ(production.string) - 1,
                         val.string.len - 1);
            ਜਾਓ skip;
        }
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(production.string, val.string.str, val.string.len);
        production.has_string = true;
        ਜਾਓ rhs;
    ਹਾਲਤ TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ਜੇ (keysym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
            scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            ਜਾਓ error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ਜੇ (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            ਜਾਓ skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    ਹਾਲਤ TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ:
        ਜੇ (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            ਜਾਓ skip;
        }
        ਜੋੜੋ_production(ਸਾਰਣੀ, s, &production);
        ਜਾਓ initial;
    ਮੂਲ:
        ਜਾਓ unexpected;
    }

unexpected:
    ਜੇ (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    ਅੰਕ_errors++;
    ਜੇ (ਅੰਕ_errors <= MAX_ERRORS)
        ਜਾਓ skip;

    scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "too many errors");
    ਜਾਓ fail;

fail:
    scanner_err(s, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "failed to parse file");
    ਵਾਪਸ false;

skip:
    ਜਦੋਂ (tok != TOK_ਸਿਰਾ_ਦਾ_ਸਤਰ && tok != TOK_ਸਿਰਾ_ਦਾ_FILE)
        tok = lex(s, &val);
    ਜਾਓ initial;

ਮੁਕੰਮਲ:
    ਵਾਪਸ true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ, ਅਚਲ ਅੱਖਰ *string, ਮਾਪ_t len,
             ਅਚਲ ਅੱਖਰ *file_name)
{
    ਬਣਤਰ scanner s;
    scanner_init(&s, ਸਾਰਣੀ->ctx, string, len, file_name, ਨਿਰਾ);
    ਜੇ (!parse(ਸਾਰਣੀ, &s, 0))
        ਵਾਪਸ false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(ਸਾਰਣੀ->nodes);
    darray_shrink(ਸਾਰਣੀ->utf8);
    ਵਾਪਸ true;
}

bool
parse_file(ਬਣਤਰ xkb_compose_ਸਾਰਣੀ *ਸਾਰਣੀ, FILE *file, ਅਚਲ ਅੱਖਰ *file_name)
{
    bool ok;
    ਅੱਖਰ *string;
    ਮਾਪ_t ਮਾਪ;

    ok = map_file(file, &string, &ਮਾਪ);
    ਜੇ (!ok) {
        log_err(ਸਾਰਣੀ->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        ਵਾਪਸ false;
    }

    ok = parse_string(ਸਾਰਣੀ, string, ਮਾਪ, file_name);
    unmap_file(string, ਮਾਪ);
    ਵਾਪਸ ok;
}
