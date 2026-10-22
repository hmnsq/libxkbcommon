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

#అన్తర్భావ "config.h"

#అన్తర్భావ <errno.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "xkbcommon/xkbcommon-keysyms.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "constants.h"
#అన్తర్భావ "table.h"
#అన్తర్భావ "scanner-utils.h"
#అన్తర్భావ "paths.h"
#అన్తర్భావ "utf8.h"
#అన్తర్భావ "parser.h"
#అన్తర్భావ "keysym.h"

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

గణన నియమాలు_token {
    TOK_చివర_యొక్క_FILE = 0,
    TOK_చివర_యొక్క_పంక్తి,
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
సంఘం lvalue {
    నిర్మాణం {
        /* Still \0-terminated. */
        స్థిరరాశి అక్షరం *str;
        కొలత_t len;
    } string;
};

స్థిర గణన నియమాలు_token
lex(నిర్మాణం scanner *s, సంఘం lvalue *val)
{
skip_మరిన్ని_whitespace_and_comments:
    /* Skip spaces. */
    అయితే (is_space(scanner_peek(s)))
        ఒకవేళ (scanner_తదుపరి(s) == '\n')
            తిరిగి TOK_చివర_యొక్క_పంక్తి;

    /* Skip comments. */
    ఒకవేళ (scanner_chr(s, '#')) {
        scanner_skip_గమ్యం_eol(s);
        వెళ్ళు skip_మరిన్ని_whitespace_and_comments;
    }

    /* See if we're done. */
    ఒకవేళ (scanner_eof(s)) తిరిగి TOK_చివర_యొక్క_FILE;

    /* New token. */
    s->token_pos = s->pos;
    s->buf_pos = 0;

    /* LHS Keysym. */
    ఒకవేళ (scanner_chr(s, '<')) {
        అయితే (scanner_peek(s) != '>' && !scanner_eol(s) && !scanner_eof(s))
            scanner_buf_append(s, scanner_తదుపరి(s));
        ఒకవేళ (!scanner_chr(s, '>')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated keysym literal");
            తిరిగి TOK_ERROR;
        }
        ఒకవేళ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "keysym literal is too long");
            తిరిగి TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        తిరిగి TOK_LHS_KEYSYM;
    }

    /* Colon. */
    ఒకవేళ (scanner_chr(s, ':'))
        తిరిగి TOK_COLON;
    ఒకవేళ (scanner_chr(s, '!'))
        తిరిగి TOK_BANG;
    ఒకవేళ (scanner_chr(s, '~'))
        తిరిగి TOK_TILDE;

    /* String literal. */
    ఒకవేళ (scanner_chr(s, '\"')) {
        అయితే (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
            ఒకవేళ (scanner_chr(s, '\\')) {
                uint8_t o;
                కొలత_t ప్రారంభం_pos = s->pos;
                ఒకవేళ (scanner_chr(s, '\\')) {
                    scanner_buf_append(s, '\\');
                }
                లేకపోతే ఒకవేళ (scanner_chr(s, '"')) {
                    scanner_buf_append(s, '"');
                }
                లేకపోతే ఒకవేళ (scanner_chr(s, 'x') || scanner_chr(s, 'X')) {
                    ఒకవేళ (scanner_hex(s, &o) && is_valid_char((uint32_t) o)) {
                        scanner_buf_append(s, (అక్షరం) o);
                    } లేకపోతే {
                        scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                     "illegal hexadecimal escape sequence "
                                     "\"%.*s\" in string literal",
                                     (పూర్ణాంకం) (s->pos - ప్రారంభం_pos + 1),
                                     &s->s[ప్రారంభం_pos - 1]);
                    }
                }
                లేకపోతే ఒకవేళ (scanner_oct(s, &o) && is_valid_char((uint32_t) o)) {
                    scanner_buf_append(s, (అక్షరం) o);
                }
                లేకపోతే ఒకవేళ (s->pos > ప్రారంభం_pos) {
                    scanner_warn(s, XKB_WARNING_INVALID_ESCAPE_SEQUENCE,
                                 "illegal octal escape sequence \"%.*s\" "
                                 "in string literal",
                                 (పూర్ణాంకం) (s->pos - ప్రారంభం_pos + 1),
                                 &s->s[ప్రారంభం_pos - 1]);
                    /* Ignore. */
                }
                లేకపోతే {
                    scanner_warn(s, XKB_WARNING_UNKNOWN_CHAR_ESCAPE_SEQUENCE,
                                 "unknown escape sequence \"\\%c\" "
                                 "in string literal",
                                 scanner_peek(s));
                    /* Ignore. */
                }
            } లేకపోతే {
                scanner_buf_append(s, scanner_తదుపరి(s));
            }
        }
        ఒకవేళ (!scanner_chr(s, '\"')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unterminated string literal");
            తిరిగి TOK_ERROR;
        }
        ఒకవేళ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "string literal is too long");
            తిరిగి TOK_ERROR;
        }
        ఒకవేళ (!is_valid_utf8(s->buf, s->buf_pos - 1)) {
            scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                        "string literal is not a valid UTF-8 string");
            తిరిగి TOK_ERROR;
        }
        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        తిరిగి TOK_STRING;
    }

    /* Identifier or include. */
    ఒకవేళ (is_alpha(scanner_peek(s)) || scanner_peek(s) == '_') {
        s->buf_pos = 0;
        అయితే (is_alnum(scanner_peek(s)) || scanner_peek(s) == '_')
            scanner_buf_append(s, scanner_తదుపరి(s));
        ఒకవేళ (!scanner_buf_append(s, '\0')) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "identifier is too long");
            తిరిగి TOK_ERROR;
        }

        ఒకవేళ (streq(s->buf, "include"))
            తిరిగి TOK_INCLUDE;

        val->string.str = s->buf;
        val->string.len = s->buf_pos;
        తిరిగి TOK_IDENT;
    }

    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                "unrecognized token");
    /* Discard rest of line. */
    scanner_skip_గమ్యం_eol(s);
    తిరిగి TOK_ERROR;
}

స్థిర గణన నియమాలు_token
lex_include_string(నిర్మాణం scanner *s, నిర్మాణం xkb_compose_పట్టిక *పట్టిక,
                   సంఘం lvalue *val_బహిర్గతం)
{
    అయితే (is_space(scanner_peek(s)))
        ఒకవేళ (scanner_తదుపరి(s) == '\n')
            తిరిగి TOK_చివర_యొక్క_పంక్తి;

    s->token_pos = s->pos;
    s->buf_pos = 0;

    ఒకవేళ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "include statement must be followed by a path");
        తిరిగి TOK_ERROR;
    }

    అయితే (!scanner_eof(s) && !scanner_eol(s) && scanner_peek(s) != '\"') {
        ఒకవేళ (scanner_chr(s, '%')) {
            ఒకవేళ (scanner_chr(s, '%')) {
                scanner_buf_append(s, '%');
            }
            లేకపోతే ఒకవేళ (scanner_chr(s, 'H')) {
                స్థిరరాశి అక్షరం *home = xkb_context_getenv(పట్టిక->ctx, "HOME");
                ఒకవేళ (!home) {
                    scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                                "%%H was used in an include statement, "
                                "but the HOME environment variable is not set");
                    తిరిగి TOK_ERROR;
                }
                ఒకవేళ (!scanner_buf_appends(s, home)) {
                    scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                                "include path after expanding %%H is too long");
                    తిరిగి TOK_ERROR;
                }
            }
            లేకపోతే ఒకవేళ (scanner_chr(s, 'L')) {
                అక్షరం *మార్గం = get_locale_compose_file_మార్గం(పట్టిక->ctx, పట్టిక->locale);
                ఒకవేళ (!మార్గం) {
                    scanner_err(s, XKB_ERROR_INVALID_COMPOSE_LOCALE,
                                "failed to expand %%L to the locale Compose file");
                    తిరిగి TOK_ERROR;
                }
                ఒకవేళ (!scanner_buf_appends(s, మార్గం)) {
                    విడుదల(మార్గం);
                    scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                                "include path after expanding %%L is too long");
                    తిరిగి TOK_ERROR;
                }
                విడుదల(మార్గం);
            }
            లేకపోతే ఒకవేళ (scanner_chr(s, 'S')) {
                స్థిరరాశి అక్షరం *xlocaledir = get_xlocaledir_మార్గం(పట్టిక->ctx);
                ఒకవేళ (!scanner_buf_appends(s, xlocaledir)) {
                    scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                                "include path after expanding %%S is too long");
                    తిరిగి TOK_ERROR;
                }
            }
            లేకపోతే {
                scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                            "unknown %% format (%c) in include statement", scanner_peek(s));
                తిరిగి TOK_ERROR;
            }
        } లేకపోతే {
            scanner_buf_append(s, scanner_తదుపరి(s));
        }
    }
    ఒకవేళ (!scanner_chr(s, '\"')) {
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unterminated include statement");
        తిరిగి TOK_ERROR;
    }
    ఒకవేళ (!scanner_buf_append(s, '\0')) {
        scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                    "include path is too long");
        తిరిగి TOK_ERROR;
    }
    val_బహిర్గతం->string.str = s->buf;
    val_బహిర్గతం->string.len = s->buf_pos;
    తిరిగి TOK_INCLUDE_STRING;
}

నిర్మాణం production {
    xkb_keysym_t lhs[COMPOSE_MAX_LHS_LEN];
    గుర్తులేని పూర్ణాంకం len;
    xkb_keysym_t keysym;
    అక్షరం string[XKB_COMPOSE_MAX_STRING_కొలత];
    /* At least one of these is true. */
    bool has_keysym;
    bool has_string;

    /* The matching is as follows: (active_mods & modmask) == mods. */
    xkb_mod_ముసుగు_t modmask;
    xkb_mod_ముసుగు_t mods;
};

స్థిర శూన్య
కూడు_production(నిర్మాణం xkb_compose_పట్టిక *పట్టిక, నిర్మాణం scanner *s,
               స్థిరరాశి నిర్మాణం production *production)
{
    గుర్తులేని పూర్ణాంకం lhs_pos = 0;
    uint32_t curr = darray_కొలత(పట్టిక->nodes) == 1 ? 0 : 1;
    uint32_t *pptr = ఖాళీ;
    నిర్మాణం compose_కణుపు *కణుపు = ఖాళీ;

    /* Warn before potentially going over the limit, discard silently after. */
    ఒకవేళ (darray_కొలత(పట్టిక->nodes) + production->len + COMPOSE_MAX_LHS_LEN >
        MAX_COMPOSE_NODES)
        scanner_warn(s, XKB_LOG_సందేశం_NO_ID,
                     "too many sequences for one Compose file; "
                     "will ignore further lines");
    ఒకవేళ (darray_కొలత(పట్టిక->nodes) + production->len >= MAX_COMPOSE_NODES)
        తిరిగి;

    /*
     * Insert the sequence to the ternary search tree, creating new nodes as
     * needed.
     *
     * TODO: We insert in the order given, this means some inputs can create
     * long O(n) chains, which results in total O(n^2) parsing time. We should
     * ensure the tree is reasonably balanced somehow.
     */
    అయితే (true) {
        స్థిరరాశి xkb_keysym_t keysym = production->lhs[lhs_pos];
        స్థిరరాశి bool చివరి = lhs_pos + 1 == production->len;

        ఒకవేళ (curr == 0) {
            /*
             * Create a new node and update the parent pointer to it.
             * Update the pointer first because the append invalidates it.
             */
            నిర్మాణం compose_కణుపు new = {
                .keysym = keysym,
                .lokid = 0,
                .hikid = 0,
                .అంతర్గత = {
                    .eqkid = 0,
                    .is_ఆకు = false,
                },
            };
            curr = darray_కొలత(పట్టిక->nodes);
            ఒకవేళ (pptr != ఖాళీ) {
                *pptr = curr;
                pptr = ఖాళీ;
            }
            darray_append(పట్టిక->nodes, new);
        }

        కణుపు = &darray_అంశం(పట్టిక->nodes, curr);

        ఒకవేళ (keysym < కణుపు->keysym) {
            pptr = &కణుపు->lokid;
            curr = కణుపు->lokid;
        } లేకపోతే ఒకవేళ (keysym > కణుపు->keysym) {
            pptr = &కణుపు->hikid;
            curr = కణుపు->hikid;
        } లేకపోతే ఒకవేళ (!చివరి) {
            ఒకవేళ (కణుపు->is_ఆకు) {
                scanner_warn(s, XKB_LOG_సందేశం_NO_ID,
                             "a sequence already exists which is a prefix of "
                             "this sequence; overriding");
                కణుపు->అంతర్గత.eqkid = 0;
                కణుపు->అంతర్గత.is_ఆకు = false;
            }
            lhs_pos++;
            pptr = &కణుపు->అంతర్గత.eqkid;
            curr = కణుపు->అంతర్గత.eqkid;
        } లేకపోతే {
            ఒకవేళ (కణుపు->is_ఆకు) {
                bool same_string =
                    (కణుపు->ఆకు.utf8 == 0 && !production->has_string) ||
                    (
                        కణుపు->ఆకు.utf8 != 0 && production->has_string &&
                        streq(&darray_అంశం(పట్టిక->utf8, కణుపు->ఆకు.utf8),
                              production->string)
                    );
                bool same_keysym =
                    (కణుపు->ఆకు.keysym == XKB_కీలకం_No_చిహ్నం && !production->has_keysym) ||
                    (
                        కణుపు->ఆకు.keysym != XKB_కీలకం_No_చిహ్నం && production->has_keysym &&
                        కణుపు->ఆకు.keysym == production->keysym
                    );
                ఒకవేళ (same_string && same_keysym) {
                    scanner_warn(s, XKB_LOG_సందేశం_NO_ID,
                                 "this compose sequence is a duplicate of another; "
                                 "skipping line");
                    తిరిగి;
                } లేకపోతే {
                    scanner_warn(s, XKB_LOG_సందేశం_NO_ID,
                                 "this compose sequence already exists; overriding");
                }
            } లేకపోతే ఒకవేళ (కణుపు->అంతర్గత.eqkid != 0) {
                scanner_warn(s, XKB_LOG_సందేశం_NO_ID,
                             "this compose sequence is a prefix of another; "
                             "overriding");
                కణుపు->అంతర్గత.eqkid = 0;
            }

            /* NOTE: If there was a previous entry, its string may *not* be
             * reused in the UTF8 table and the corresponding memory is then
             * wasted! */
            ఒకవేళ (production->has_string) {
                స్థిరరాశి కొలత_t len = అక్షరశ్రేణి_పొడవు(production->string);
                ఒకవేళ (కణుపు->is_ఆకు && కణుపు->ఆకు.utf8 &&
                    len <= అక్షరశ్రేణి_పొడవు(&darray_అంశం(పట్టిక->utf8, కణుపు->ఆకు.utf8))) {
                    /* There is a previous entry with an overwritable string */
                    జ్ఞాపకం_నకలు(&darray_అంశం(పట్టిక->utf8, కణుపు->ఆకు.utf8),
                           production->string, len + 1);
                } లేకపోతే {
                    /* Cannot reuse string: allocate new one */
                    కణుపు->ఆకు.utf8 = darray_కొలత(పట్టిక->utf8);
                    darray_append_వస్తువులు(పట్టిక->utf8, production->string,
                                        (darray_కొలత_t)అక్షరశ్రేణి_పొడవు(production->string) + 1);
                }
            } లేకపోతే {
                /* Ensure we reset possible previous entry */
                కణుపు->ఆకు.utf8 = 0;
            }

            ఒకవేళ (production->has_keysym) {
                కణుపు->ఆకు.keysym = production->keysym;
            } లేకపోతే {
                /* Ensure we reset possible previous entry */
                కణుపు->ఆకు.keysym = XKB_కీలకం_No_చిహ్నం;
            }

            కణుపు->is_ఆకు = true;
            తిరిగి;
        }
    }
}

/* Should match resolve_modifier(). */
#నిర్వచించు ALL_MODS_ముసుగు ((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3))

స్థిర xkb_mod_సూచీ_t
resolve_modifier(స్థిరరాశి అక్షరం *name)
{
    స్థిర స్థిరరాశి నిర్మాణం {
        స్థిరరాశి అక్షరం *name;
        xkb_mod_సూచీ_t mod;
    } mods[] = {
        { "Shift", 0 },
        { "Ctrl", 2 },
        { "Alt", 3 },
        { "Meta", 3 },
        { "Lock", 1 },
        { "Caps", 1 },
    };

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_కొలత(mods); i++)
        ఒకవేళ (streq(name, mods[i].name))
            తిరిగి mods[i].mod;

    తిరిగి XKB_MOD_INVALID;
}

/* Parse a string literal ("...") and return the corresponding unescaped string,
 * or NULL if it fails.
 * This is aimed only for testing (un)escaping characters. */
అక్షరం *
parse_string_literal(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *string)
{
    నిర్మాణం scanner s;
    సంఘం lvalue val;
    scanner_init(&s, ctx, string, అక్షరశ్రేణి_పొడవు(string), "(unnamed)", ఖాళీ);
    ఒకవేళ (lex(&s, &val) == TOK_STRING) {
        తిరిగి strdup(val.string.str);
    } లేకపోతే {
        fprintf(stderr, "ERROR: %s\n", s.s);
        తిరిగి ఖాళీ;
    }
}

స్థిర bool
parse(నిర్మాణం xkb_compose_పట్టిక *పట్టిక, నిర్మాణం scanner *s,
      గుర్తులేని పూర్ణాంకం include_లోతు);

స్థిర bool
do_include(నిర్మాణం xkb_compose_పట్టిక *పట్టిక, నిర్మాణం scanner *s,
           స్థిరరాశి అక్షరం *మార్గం, గుర్తులేని పూర్ణాంకం include_లోతు)
{
    FILE *file;
    bool ok;
    అక్షరం *string;
    కొలత_t కొలత;
    నిర్మాణం scanner new_s;

    ఒకవేళ (include_లోతు >= COMPOSE_MAX_INCLUDE_లోతు) {
        scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                    "maximum include depth (%u) exceeded; maybe there is an include loop?",
                    COMPOSE_MAX_INCLUDE_లోతు);
        తిరిగి false;
    }

    file = fopen(మార్గం, "rb");
    ఒకవేళ (!file) {
        scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                    "failed to open included Compose file \"%s\": %s",
                    మార్గం, strerror(errno));
        తిరిగి false;
    }

    ok = map_file(file, &string, &కొలత);
    ఒకవేళ (!ok) {
        scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                    "failed to read included Compose file \"%s\": %s",
                    మార్గం, strerror(errno));
        వెళ్ళు err_file;
    }

    scanner_init(&new_s, పట్టిక->ctx, string, కొలత, మార్గం, s->priv);

    ok = parse(పట్టిక, &new_s, include_లోతు + 1);
    ఒకవేళ (!ok)
        వెళ్ళు err_unmap;

err_unmap:
    unmap_file(string, కొలత);
err_file:
    fclose(file);
    తిరిగి ok;
}

// NOLINTBEGIN(readability-trivial-switch)
స్థిర bool
parse(నిర్మాణం xkb_compose_పట్టిక *పట్టిక, నిర్మాణం scanner *s,
      గుర్తులేని పూర్ణాంకం include_లోతు)
{
    గణన నియమాలు_token tok;
    సంఘం lvalue val;
    xkb_keysym_t keysym;
    నిర్మాణం production production;
    గణన { MAX_ERRORS = 10 };
    పూర్ణాంకం అంకె_errors = 0;

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, include, modifier list, keysym, comment */
    ఒకవేళ (!scanner_check_supported_char_encoding(s)) {
        scanner_err(s, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported file encodings are ASCII and UTF-8.");
        వెళ్ళు fail;
    }

initial:
    production.len = 0;
    production.has_keysym = false;
    production.has_string = false;
    production.mods = 0;
    production.modmask = 0;

    /* fallthrough */

initial_eol:
    ఎంపిక (tok = lex(s, &val)) {
    స్థితి TOK_చివర_యొక్క_పంక్తి:
        వెళ్ళు initial_eol;
    స్థితి TOK_చివర_యొక్క_FILE:
        వెళ్ళు ముగిసిన;
    స్థితి TOK_INCLUDE:
        వెళ్ళు include;
    అప్రమేయ:
        వెళ్ళు lhs_tok;
    }

include:
    ఎంపిక (tok = lex_include_string(s, పట్టిక, &val)) {
    స్థితి TOK_INCLUDE_STRING:
        వెళ్ళు include_eol;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

include_eol:
    ఎంపిక (tok = lex(s, &val)) {
    స్థితి TOK_చివర_యొక్క_పంక్తి:
        ఒకవేళ (!do_include(పట్టిక, s, val.string.str, include_లోతు))
            వెళ్ళు fail;
        వెళ్ళు initial;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

lhs:
    tok = lex(s, &val);
lhs_tok:
    ఎంపిక (tok) {
    స్థితి TOK_COLON:
        ఒకవేళ (production.len <= 0) {
            scanner_warn(s, XKB_LOG_సందేశం_NO_ID,
                         "expected at least one keysym on left-hand side; "
                         "skipping line");
            వెళ్ళు skip;
        }
        వెళ్ళు rhs;
    స్థితి TOK_IDENT:
        ఒకవేళ (streq(val.string.str, "None")) {
            production.mods = 0;
            production.modmask = ALL_MODS_ముసుగు;
            వెళ్ళు lhs_keysym;
        }
        వెళ్ళు lhs_mod_list_tok;
    స్థితి TOK_TILDE:
        వెళ్ళు lhs_mod_list_tok;
    స్థితి TOK_BANG:
        production.modmask = ALL_MODS_ముసుగు;
        వెళ్ళు lhs_mod_list;
    అప్రమేయ:
        వెళ్ళు lhs_keysym_tok;
    }

lhs_keysym:
    tok = lex(s, &val);
lhs_keysym_tok:
    ఎంపిక (tok) {
    స్థితి TOK_LHS_KEYSYM:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ఒకవేళ (keysym == XKB_కీలకం_No_చిహ్నం) {
            scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                        "unrecognized keysym \"%s\" on left-hand side",
                        val.string.str);
            వెళ్ళు error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ఒకవేళ (production.len + 1 > COMPOSE_MAX_LHS_LEN) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "too many keysyms (%u) on left-hand side; skipping line",
                         COMPOSE_MAX_LHS_LEN + 1);
            వెళ్ళు skip;
        }
        production.lhs[production.len++] = keysym;
        production.mods = 0;
        production.modmask = 0;
        వెళ్ళు lhs;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

lhs_mod_list:
    tok = lex(s, &val);
lhs_mod_list_tok: {
        bool tilde = false;
        xkb_mod_సూచీ_t mod;

        ఒకవేళ (tok != TOK_TILDE && tok != TOK_IDENT)
            వెళ్ళు lhs_keysym_tok;

        ఒకవేళ (tok == TOK_TILDE) {
            tilde = true;
            tok = lex(s, &val);
        }

        ఒకవేళ (tok != TOK_IDENT)
            వెళ్ళు unexpected;

        mod = resolve_modifier(val.string.str);
        ఒకవేళ (mod == XKB_MOD_INVALID) {
            scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                        "unrecognized modifier \"%s\"",
                        val.string.str);
            వెళ్ళు error;
        }

        production.modmask |= UINT32_C(1) << mod;
        ఒకవేళ (tilde)
            production.mods &= ~(UINT32_C(1) << mod);
        లేకపోతే
            production.mods |= UINT32_C(1) << mod;

        వెళ్ళు lhs_mod_list;
    }

rhs:
    ఎంపిక (tok = lex(s, &val)) {
    స్థితి TOK_STRING:
        ఒకవేళ (production.has_string) {
            scanner_warn(s, XKB_LOG_సందేశం_NO_ID,
                         "right-hand side can have at most one string; "
                         "skipping line");
            వెళ్ళు skip;
        }
        ఒకవేళ (val.string.len <= 0) {
            scanner_warn(s, XKB_LOG_సందేశం_NO_ID,
                         "right-hand side string must not be empty; "
                         "skipping line");
            వెళ్ళు skip;
        }
        ఒకవేళ (val.string.len > పరిమాణం(production.string)) {
            scanner_warn(s, XKB_LOG_సందేశం_NO_ID,
                         "right-hand side string is too long: "
                         "expected max: %zu, got: %zu; skipping line",
                         పరిమాణం(production.string) - 1,
                         val.string.len - 1);
            వెళ్ళు skip;
        }
        జ్ఞాపకం_నకలు(production.string, val.string.str, val.string.len);
        production.has_string = true;
        వెళ్ళు rhs;
    స్థితి TOK_IDENT:
        keysym = xkb_keysym_from_name(val.string.str, XKB_KEYSYM_NO_FLAGS);
        ఒకవేళ (keysym == XKB_కీలకం_No_చిహ్నం) {
            scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                        "unrecognized keysym \"%s\" on right-hand side",
                        val.string.str);
            వెళ్ళు error;
        }
        check_deprecated_keysyms(scanner_warn, s, s->ctx,
                                 keysym, val.string.str, val.string.str, "%s", "");
        ఒకవేళ (production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side can have at most one keysym; "
                         "skipping line");
            వెళ్ళు skip;
        }
        production.keysym = keysym;
        production.has_keysym = true;
        /* fallthrough */
    స్థితి TOK_చివర_యొక్క_పంక్తి:
        ఒకవేళ (!production.has_string && !production.has_keysym) {
            scanner_warn(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                         "right-hand side must have at least one of string "
                         "or keysym; skipping line");
            వెళ్ళు skip;
        }
        కూడు_production(పట్టిక, s, &production);
        వెళ్ళు initial;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

unexpected:
    ఒకవేళ (tok != TOK_ERROR)
        scanner_err(s, XKB_ERROR_INVALID_COMPOSE_SYNTAX,
                    "unexpected token");
error:
    అంకె_errors++;
    ఒకవేళ (అంకె_errors <= MAX_ERRORS)
        వెళ్ళు skip;

    scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                "too many errors");
    వెళ్ళు fail;

fail:
    scanner_err(s, XKB_LOG_సందేశం_NO_ID,
                "failed to parse file");
    తిరిగి false;

skip:
    అయితే (tok != TOK_చివర_యొక్క_పంక్తి && tok != TOK_చివర_యొక్క_FILE)
        tok = lex(s, &val);
    వెళ్ళు initial;

ముగిసిన:
    తిరిగి true;
}
// NOLINTEND(readability-trivial-switch)

bool
parse_string(నిర్మాణం xkb_compose_పట్టిక *పట్టిక, స్థిరరాశి అక్షరం *string, కొలత_t len,
             స్థిరరాశి అక్షరం *file_name)
{
    నిర్మాణం scanner s;
    scanner_init(&s, పట్టిక->ctx, string, len, file_name, ఖాళీ);
    ఒకవేళ (!parse(పట్టిక, &s, 0))
        తిరిగి false;
    /* Maybe the allocator can use the excess space. */
    darray_shrink(పట్టిక->nodes);
    darray_shrink(పట్టిక->utf8);
    తిరిగి true;
}

bool
parse_file(నిర్మాణం xkb_compose_పట్టిక *పట్టిక, FILE *file, స్థిరరాశి అక్షరం *file_name)
{
    bool ok;
    అక్షరం *string;
    కొలత_t కొలత;

    ok = map_file(file, &string, &కొలత);
    ఒకవేళ (!ok) {
        log_err(పట్టిక->ctx, XKB_LOG_సందేశం_NO_ID,
                "Couldn't read Compose file %s: %s\n",
                file_name, strerror(errno));
        తిరిగి false;
    }

    ok = parse_string(పట్టిక, string, కొలత, file_name);
    unmap_file(string, కొలత);
    తిరిగి ok;
}
