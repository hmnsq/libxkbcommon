/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdint.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "rules.h"
#అన్తర్భావ "rmlvo.h"
#అన్తర్భావ "include.h"
#అన్తర్భావ "scanner-utils.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "util-numbers.h"
#అన్తర్భావ "util-paths.h"

#నిర్వచించు MAX_INCLUDE_లోతు 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
సంఘం lvalue {
    నిర్మాణం sval string;
};

గణన నియమాలు_token {
    TOK_చివర_యొక్క_FILE = 0,
    TOK_చివర_యొక్క_పంక్తి,
    TOK_IDENTIFIER,
    TOK_సమూహం_NAME,
    TOK_BANG,
    TOK_EQUALS,
    TOK_WILD_CARD_నక్షత్రం,
    TOK_WILD_CARD_NONE,
    TOK_WILD_CARD_SOME,
    TOK_WILD_CARD_ANY,
    TOK_INCLUDE,
    TOK_ERROR
};

స్థిర గణన నియమాలు_token
lex(నిర్మాణం scanner *s, సంఘం lvalue *val)
{
skip_మరిన్ని_whitespace_and_comments:
    /* Skip spaces */
    అయితే (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    ఒకవేళ (scanner_lit(s, "//")) {
        scanner_skip_గమ్యం_eol(s);
    }

    /* New line */
    ఒకవేళ (scanner_eol(s)) {
        అయితే (scanner_eol(s)) scanner_తదుపరి(s);
        తిరిగి TOK_చివర_యొక్క_పంక్తి;
    }

    /* Escaped line continuation */
    ఒకవేళ (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        ఒకవేళ (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            తిరిగి TOK_ERROR;
        }
        scanner_తదుపరి(s);
        వెళ్ళు skip_మరిన్ని_whitespace_and_comments;
    }

    /* See if we're done */
    ఒకవేళ (scanner_eof(s)) తిరిగి TOK_చివర_యొక్క_FILE;

    /* New token */
    s->token_pos = s->pos;
    గణన నియమాలు_token token = TOK_IDENTIFIER;

    స్థిరరాశి అక్షరం తదుపరి = scanner_తదుపరి(s);
    ఎంపిక (తదుపరి) {
    స్థితి '!':
        తిరిగి TOK_BANG;
    స్థితి '$':
        /*సమూహం*/
        token = TOK_సమూహం_NAME;
        val->string.len = 0;
        ఆపు;
    స్థితి '*':
        ఒకవేళ (scanner_నియమాలు_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            ఆపు;
        } లేకపోతే {
            /* Legacy wild card */
            తిరిగి TOK_WILD_CARD_నక్షత్రం;
        }
    స్థితి '<':
        /* Extended wild cards */
        ఒకవేళ (scanner_నియమాలు_lit_token(s, "none>")) తిరిగి TOK_WILD_CARD_NONE;
        ఒకవేళ (scanner_నియమాలు_lit_token(s, "some>")) తిరిగి TOK_WILD_CARD_SOME;
        ఒకవేళ (scanner_నియమాలు_lit_token(s, "any>")) తిరిగి TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        ప్రతిజ్ఞా(scanner_నియమాలు_is_ident('<'));
        val->string.len = 1;
        ఆపు;
    స్థితి '=':
        ఒకవేళ (scanner_నియమాలు_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            ఆపు;
        } లేకపోతే {
            తిరిగి TOK_EQUALS;
        }
    స్థితి 'i':
        ఒకవేళ (scanner_నియమాలు_lit_token(s, "nclude")) {
            /* Include statement */
            తిరిగి TOK_INCLUDE;
        }
        /* Identifier */
        ప్రతిజ్ఞా(scanner_నియమాలు_is_ident('i'));
        val->string.len = 1;
        ఆపు;
    అప్రమేయ:
        ఒకవేళ (likely(scanner_నియమాలు_is_ident(తదుపరి))) {
            /* Identifier */
            val->string.len = 1;
        } లేకపోతే {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.ప్రారంభం = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    ప్రతిజ్ఞా(scanner_నియమాలు_is_ident(విలీనం_OVERRIDE_ఉపసర్గ));
    ప్రతిజ్ఞా(scanner_నియమాలు_is_ident(విలీనం_AUGMENT_ఉపసర్గ));
    ప్రతిజ్ఞా(scanner_నియమాలు_is_ident(విలీనం_REPLACE_ఉపసర్గ));
    ప్రతిజ్ఞా(scanner_నియమాలు_is_ident('('));
    ప్రతిజ్ఞా(scanner_నియమాలు_is_ident(')'));
    val->string.len += scanner_నియమాలు_ident_token(s);

    ఒకవేళ (unlikely(val->string.len == 0)) {
        ఒకవేళ (token == TOK_సమూహం_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } లేకపోతే {
            scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                        "unrecognized token");
        }
        తిరిగి TOK_ERROR;
    }

    తిరిగి token;
}

/***====================================================================***/

గణన నియమాలు_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_అంకె_ENTRIES
};

గణన {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

రకనిర్వచనం uint8_t mlvo_సూచీ_t;
రకనిర్వచనం uint8_t mlvo_ముసుగు_t;


స్థిర స్థిరరాశి నిర్మాణం sval నియమాలు_mlvo_svals[MLVO_అంకె_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

గణన నియమాలు_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_అంకె_ENTRIES
};

రకనిర్వచనం uint8_t kccgst_సూచీ_t;
రకనిర్వచనం uint8_t kccgst_ముసుగు_t;

స్థిర స్థిరరాశి నిర్మాణం sval నియమాలు_kccgst_svals[KCCGST_అంకె_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

గణన {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_ముసుగు_t)XKB_OPTION_LAYOUT_ముసుగు_GLOBAL ==
              (xkb_layout_ముసుగు_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
నిర్మాణం matched_sval {
    నిర్మాణం sval sval;
    /** Used for layout-specific options */
    xkb_layout_ముసుగు_t layouts;
    xkb_layout_ముసుగు_t matched;
};
రకనిర్వచనం darray(నిర్మాణం matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
నిర్మాణం నియమం_పేర్లు {
    నిర్మాణం matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

నిర్మాణం సమూహం {
    నిర్మాణం sval name;
    darray_sval elements;
};

నిర్మాణం mapping {
    గణన నియమాలు_mlvo mlvo_at_pos[MLVO_అంకె_ENTRIES];
    mlvo_సూచీ_t అంకె_mlvo;
    mlvo_ముసుగు_t defined_mlvo_ముసుగు;
    bool has_layout_idx_range;
    bool has_గుణిజం_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    సంఘం {
        నిర్మాణం { xkb_layout_సూచీ_t layout_idx, variant_idx; };
        నిర్మాణం { xkb_layout_సూచీ_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    సంఘం {
        xkb_layout_ముసుగు_t చురుకైన;
        xkb_layout_ముసుగు_t layouts_candidates_ముసుగు;
    };
    గణన నియమాలు_kccgst kccgst_at_pos[KCCGST_అంకె_ENTRIES];
    kccgst_సూచీ_t అంకె_kccgst;
    kccgst_ముసుగు_t defined_kccgst_ముసుగు;
};

గణన mlvo_సరిపోలిక_type {
    /** Match the given plain value */
    MLVO_సరిపోలిక_సాధారణ = 0,
    /** Match depending on the value of `wildcard_match_type` */
    MLVO_సరిపోలిక_WILDCARD_LEGACY,
    /** Match empty value */
    MLVO_సరిపోలిక_WILDCARD_NONE,
    /** Match non-empty value */
    MLVO_సరిపోలిక_WILDCARD_SOME,
    /** Match any value, optionally empty */
    MLVO_సరిపోలిక_WILDCARD_ANY,
    /** Match any entry in a group */
    MLVO_సరిపోలిక_సమూహం,
};

గణన wildcard_సరిపోలిక_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_సరిపోలిక_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_సరిపోలిక_ALL,
};

నిర్మాణం నియమం_2 {
    నిర్మాణం sval mlvo_విలువ_at_pos[MLVO_అంకె_ENTRIES];
    గణన mlvo_సరిపోలిక_type సరిపోలిక_type_at_pos[MLVO_అంకె_ENTRIES];
    నిర్మాణం sval kccgst_విలువ_at_pos[KCCGST_అంకె_ENTRIES];
    mlvo_సూచీ_t అంకె_mlvo_విలువలు;
    kccgst_సూచీ_t అంకె_kccgst_విలువలు;
    bool skip;
};

#నిర్వచించు ముక్క_KCCGST_బిట్టు_క్షేత్రం_కొలత 4
static_assert(!((1u << (ముక్క_KCCGST_బిట్టు_క్షేత్రం_కొలత - 1)) &
                (KCCGST_అంకె_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
నిర్మాణం kccgst_బఫరు_ముక్క {
    uint32_t నిడివి:(32 - ముక్క_KCCGST_బిట్టు_క్షేత్రం_కొలత);
    గణన నియమాలు_kccgst kccgst:ముక్క_KCCGST_బిట్టు_క్షేత్రం_కొలత;
    xkb_layout_సూచీ_t layout;
};

/* Buffer for pending KcCGST values */
నిర్మాణం kccgst_బఫరు {
    darray_char బఫరు;
    /* Slice corresponding to each value in the buffer */
    darray(నిర్మాణం kccgst_బఫరు_ముక్క) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
నిర్మాణం matcher {
    నిర్మాణం xkb_context *ctx;
    /* Input.*/
    నిర్మాణం నియమం_పేర్లు rmlvo;
    సంఘం lvalue val;
    darray(నిర్మాణం సమూహం) groups;
    /* Current mapping. */
    నిర్మాణం mapping mapping;
    /* Current rule. */
    నిర్మాణం నియమం_2 నియమం_2;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    నిర్మాణం kccgst_బఫరు pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_అంకె_ENTRIES];
};

స్థిర నిర్మాణం sval
strip_spaces(నిర్మాణం sval v)
{
    అయితే (v.len > 0 && is_space(v.ప్రారంభం[0])) { v.len--; v.ప్రారంభం++; }
    అయితే (v.len > 0 && is_space(v.ప్రారంభం[v.len - 1])) v.len--;
    తిరిగి v;
}

స్థిర darray_matched_sval
విభజించు_comma_separated_mlvo(నిర్మాణం xkb_context *ctx,
                           గణన నియమాలు_mlvo mlvo, స్థిరరాశి అక్షరం *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    ఒకవేళ (!s) {
        ensure_at_కనిష్ఠ_one_విలువ:
        /* Label followed by a declaration is a C23 extension */ ;

        నిర్మాణం matched_sval val = {
            .sval = SVAL(ఖాళీ, 0),
            .layouts = (xkb_layout_ముసుగు_t)XKB_OPTION_LAYOUT_ముసుగు_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        తిరిగి arr;
    }

    అయితే (true) {
        నిర్మాణం matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_ముసుగు_t)XKB_OPTION_LAYOUT_ముసుగు_GLOBAL,
            .matched = 0,
        };
        అయితే (*s != '\0' && *s != ',' && *s != OPTIONS_సమూహం_SPECIFIER_ఉపసర్గ) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        ఒకవేళ (*s == OPTIONS_సమూహం_SPECIFIER_ఉపసర్గ) {
            /* Handle numeric layout index */
            s++;
            స్థిరరాశి అక్షరం* స్థిరరాశి layout_ప్రారంభం = s;
            xkb_layout_సూచీ_t layout = XKB_LAYOUT_INVALID;

            పూర్ణాంకం లెక్క = parse_dec_గమ్యం_uint32_t(s, SIZE_MAX, &layout);
            ఒకవేళ (లెక్క > 0) {
                /* Note: 1-indexed layout */
                s += లెక్క;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                ఒకవేళ (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (గుర్తులేని పూర్ణాంకం) val.sval.len, val.sval.ప్రారంభం);
                } లేకపోతే ఒకవేళ (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_సందేశం_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (గుర్తులేని పూర్ణాంకం) val.sval.len, val.sval.ప్రారంభం);
                } లేకపోతే {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            స్థిరరాశి అక్షరం* స్థిరరాశి layout_సూచీ_చివర = s;
            అయితే (*s != '\0' && *s != ',') { s++; }
            ఒకవేళ (లెక్క <= 0 || layout_సూచీ_చివర != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (గుర్తులేని పూర్ణాంకం) (s - layout_ప్రారంభం), layout_ప్రారంభం,
                        (గుర్తులేని పూర్ణాంకం) val.sval.len, val.sval.ప్రారంభం);
            }
        }

        ఒకవేళ (val.sval.len || mlvo != MLVO_OPTION) {
            ఒకవేళ (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                నిర్మాణం matched_sval *prev;
                darray_foreach(prev, arr) {
                    ఒకవేళ (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        ఒకవేళ (val.layouts ==
                            (xkb_layout_ముసుగు_t)XKB_OPTION_LAYOUT_ముసుగు_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } లేకపోతే ఒకవేళ (prev->layouts != (xkb_layout_ముసుగు_t)
                                   XKB_OPTION_LAYOUT_ముసుగు_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        వెళ్ళు తదుపరి_విలువ;
                    }
                }
            }
            darray_append(arr, val);
        }

తదుపరి_విలువ:
        ఒకవేళ (*s == '\0') ఆపు;
        ఒకవేళ (*s == ',') s++;
    }

    ఒకవేళ (darray_ఖాళీలు(arr))
        వెళ్ళు ensure_at_కనిష్ఠ_one_విలువ;

    తిరిగి arr;
}

స్థిర నిర్మాణం matcher *
matcher_new_from_rmlvo(స్థిరరాశి నిర్మాణం xkb_rmlvo_builder *rmlvo, స్థిరరాశి అక్షరం **నియమాలు)
{
    నిర్మాణం matcher *m = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*m));
    ఒకవేళ (!m)
        తిరిగి ఖాళీ;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    నిర్మాణం xkb_నియమం_పేర్లు పేర్లు = {
        .నియమాలు = rmlvo->నియమాలు,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_ఖాళీలు(rmlvo->layouts)) ? ఖాళీ : "x",
        .variant = (darray_ఖాళీలు(rmlvo->layouts)) ? ఖాళీ : "x",
        .options = (darray_ఖాళీలు(rmlvo->options)) ? ఖాళీ : "x",
    };
    స్థిరరాశి గణన RMLVO మారింది = xkb_context_sanitize_నియమం_పేర్లు(rmlvo->ctx,
                                                               &పేర్లు);

    /* Initialize matcher with sanitized names, if relevant */
    ఒకవేళ (మారింది & RMLVO_నియమాలు) {
        *నియమాలు = పేర్లు.నియమాలు;
    } లేకపోతే {
        *నియమాలు = rmlvo->నియమాలు;
    }

    ఒకవేళ (మారింది & RMLVO_MODEL) {
        m->rmlvo.model.sval.ప్రారంభం = పేర్లు.model;
    } లేకపోతే {
        m->rmlvo.model.sval.ప్రారంభం = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_సురక్షిత(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_ముసుగు_t)XKB_OPTION_LAYOUT_ముసుగు_GLOBAL;

    ప్రతిజ్ఞా((మారింది & RMLVO_LAYOUT) || !(మారింది & RMLVO_VARIANT));
    ఒకవేళ (మారింది & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = విభజించు_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      పేర్లు.layout);
        m->rmlvo.variants = విభజించు_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       పేర్లు.variant);
        ఒకవేళ (darray_కొలత(m->rmlvo.layouts) > darray_కొలత(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            ఒకవేళ (!isempty(పేర్లు.variant))
                log_warn(m->ctx, XKB_LOG_సందేశం_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         పేర్లు.layout ? పేర్లు.layout : "(none)",
                         పేర్లు.variant ? పేర్లు.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_కొలత(m->rmlvo.layouts));
        } లేకపోతే ఒకవేళ (darray_కొలత(m->rmlvo.layouts) < darray_కొలత(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_సందేశం_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    పేర్లు.layout ? పేర్లు.layout : "(none)",
                    పేర్లు.variant ? పేర్లు.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_కొలత(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } లేకపోతే {
        నిర్మాణం xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            నిర్మాణం matched_sval val = {
                .sval = SVAL(layout->layout, strlen_సురక్షిత(layout->layout)),
                .layouts = (xkb_layout_ముసుగు_t)XKB_OPTION_LAYOUT_ముసుగు_GLOBAL,
                .matched = 0
            };
            darray_append(m->rmlvo.layouts, val);
            val.sval.ప్రారంభం = layout->variant;
            val.sval.len = strlen_సురక్షిత(layout->variant);
            darray_append(m->rmlvo.variants, val);
        }
    }

    ఒకవేళ (మారింది & RMLVO_OPTIONS) {
        m->rmlvo.options = విభజించు_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      పేర్లు.options);
    } లేకపోతే {
        నిర్మాణం xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            నిర్మాణం matched_sval val = {
                .sval = SVAL(option->option, strlen_సురక్షిత(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    తిరిగి m;
}

స్థిర నిర్మాణం matcher *
matcher_new_from_పేర్లు(నిర్మాణం xkb_context *ctx,
            స్థిరరాశి నిర్మాణం xkb_నియమం_పేర్లు *rmlvo)
{
    నిర్మాణం matcher *m = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*m));
    ఒకవేళ (!m)
        తిరిగి ఖాళీ;

    m->ctx = ctx;
    m->rmlvo.model.sval.ప్రారంభం = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_సురక్షిత(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_ముసుగు_t)XKB_OPTION_LAYOUT_ముసుగు_GLOBAL;
    m->rmlvo.layouts = విభజించు_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = విభజించు_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = విభజించు_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    ఒకవేళ (darray_కొలత(m->rmlvo.layouts) > darray_కొలత(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        ఒకవేళ (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_సందేశం_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_కొలత(m->rmlvo.layouts));
    } లేకపోతే ఒకవేళ (darray_కొలత(m->rmlvo.layouts) < darray_కొలత(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_కొలత(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    తిరిగి m;
}

స్థిర శూన్య
matcher_free(నిర్మాణం matcher *m)
{
    ఒకవేళ (!m)
        తిరిగి;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    నిర్మాణం సమూహం *సమూహం;
    darray_foreach(సమూహం, m->groups)
        darray_free(సమూహం->elements);
    darray_free(m->pending_kccgst.బఫరు);
    darray_free(m->pending_kccgst.slices);
    కొరకు (kccgst_సూచీ_t i = 0; i < (kccgst_సూచీ_t) KCCGST_అంకె_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    విడుదల(m);
}

స్థిర శూన్య
matcher_సమూహం_ప్రారంభం_new(నిర్మాణం matcher *m, నిర్మాణం sval name)
{
    నిర్మాణం సమూహం సమూహం = { .name = name, .elements = darray_new() };
    darray_append(m->groups, సమూహం);
}

స్థిర శూన్య
matcher_సమూహం_కూడు_element(నిర్మాణం matcher *m, నిర్మాణం scanner *s,
                          నిర్మాణం sval element)
{
    darray_append(darray_అంశం(m->groups, darray_కొలత(m->groups) - 1).elements,
                  element);
}

స్థిర bool
read_నియమాలు_file(నిర్మాణం xkb_context *ctx,
                నిర్మాణం matcher *matcher,
                గుర్తులేని పూర్ణాంకం include_లోతు,
                FILE *file,
                స్థిరరాశి అక్షరం *మార్గం);

స్థిర శూన్య
matcher_include(నిర్మాణం matcher *m, నిర్మాణం scanner *మాతృ_scanner,
                గుర్తులేని పూర్ణాంకం include_లోతు,
                నిర్మాణం sval inc)
{
    ఒకవేళ (include_లోతు >= MAX_INCLUDE_లోతు) {
        scanner_err(మాతృ_scanner, XKB_LOG_సందేశం_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_లోతు);
        తిరిగి;
    }

    స్థిరరాశి అక్షరం *stmt_file = inc.ప్రారంభం;
    కొలత_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    అక్షరం buf[PATH_MAX] = {0};
    స్థిరరాశి ssize_t expanded = expand_మార్గం(m->ctx, మాతృ_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_నియమాలు,
                                         buf, పరిమాణం(buf));
    ఒకవేళ (expanded < 0) {
        /* Error */
        తిరిగి;
    } లేకపోతే ఒకవేళ (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (కొలత_t) expanded;
        ప్రతిజ్ఞా(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = 0;
    స్థిరరాశి bool absolute_మార్గం = is_absolute_మార్గం(stmt_file);
    ఒకవేళ (absolute_మార్గం) {
        /* Absolute path: no need for lookup in XKB paths */
        ఒకవేళ (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            ఒకవేళ (stmt_file_len < పరిమాణం(buf)) {
                జ్ఞాపకం_నకలు(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } లేకపోతే {
                log_err(m->ctx, XKB_ERROR_INVALID_మార్గం,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, పరిమాణం(buf),
                        (గుర్తులేని పూర్ణాంకం) stmt_file_len, stmt_file);
                తిరిగి;
            }
        } లేకపోతే {
            /* %-expansion is always NULL-terminated */
            ప్రతిజ్ఞా(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } లేకపోతే {
        /* Relative path: lookup the first XKB path */
        ఒకవేళ (unlikely(expanded)) {
            /*
             * Relative path after expansion
             *
             * Unlikely to happen, because %-expansion is meant to use absolute
             * paths. Considering that:
             * - we do not resolve paths before expansion, leading to unexpected
             *   result here;
             * - we need the buffer afterwards, but it currently contains the
             *   expanded path;
             * - this is an edge case;
             * we simply make the lookup fail.
             */
            file = ఖాళీ;
        } లేకపోతే {
            file = కనుగొను_File_లోపల_Xkb_మార్గం(m->ctx, మాతృ_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_నియమాలు,
                                     buf, పరిమాణం(buf), &స్థానభ్రంశం, true);
        }
    }

    అయితే (file) {
        ప్రతిజ్ఞా(strlen_సురక్షిత(buf) < పరిమాణం(buf));
        bool ret = read_నియమాలు_file(m->ctx, m, include_లోతు + 1, file, buf);
        fclose(file);
        ఒకవేళ (ret)
            తిరిగి;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_సందేశం_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        ఒకవేళ (absolute_మార్గం) {
            /* There is no point to search further if the path is absolute */
            ఆపు;
        }

        /* Try next XKB path */
        స్థానభ్రంశం++;
        file = కనుగొను_File_లోపల_Xkb_మార్గం(m->ctx, మాతృ_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_నియమాలు,
                                 buf, పరిమాణం(buf), &స్థానభ్రంశం, true);
    }

    log_err(m->ctx, XKB_LOG_సందేశం_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (గుర్తులేని పూర్ణాంకం) stmt_file_len, stmt_file);
}

స్థిర శూన్య
matcher_mapping_ప్రారంభం_new(నిర్మాణం matcher *m)
{
    కొరకు (mlvo_సూచీ_t i = 0; i < (mlvo_సూచీ_t) MLVO_అంకె_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_అంకె_ENTRIES;
    కొరకు (kccgst_సూచీ_t i = 0; i < (kccgst_సూచీ_t) KCCGST_అంకె_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_అంకె_ENTRIES;
    m->mapping.has_గుణిజం_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.అంకె_mlvo = m->mapping.అంకె_kccgst = 0;
    m->mapping.defined_mlvo_ముసుగు = 0;
    m->mapping.defined_kccgst_ముసుగు = 0;
    m->mapping.చురుకైన = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
స్థిర పూర్ణాంకం
parse_layout_int_సూచీ_తోక(స్థిరరాశి అక్షరం *s, కొలత_t max_len,
                            xkb_layout_సూచీ_t *బహిర్గతం)
{
    ఒకవేళ (max_len < 3)
        తిరిగి -1;
    uint32_t val = 0;
    స్థిరరాశి పూర్ణాంకం లెక్క = parse_dec_గమ్యం_uint32_t(&s[1], max_len - 2, &val);
    ఒకవేళ (లెక్క <= 0 || s[1 + లెక్క] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        తిరిగి -1;
    /* To zero-based index. */
    *బహిర్గతం = val - 1;
    తిరిగి లెక్క + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
స్థిర పూర్ణాంకం
వెలికితీయి_layout_సూచీ(స్థిరరాశి అక్షరం *s, కొలత_t max_len, xkb_layout_సూచీ_t *బహిర్గతం)
{
    /* This function is pretty stupid, but works for now. */
    *బహిర్గతం = XKB_LAYOUT_INVALID;
    ఒకవేళ (max_len < 3 || s[0] != '[')
        తిరిగి -1;
    ఒకవేళ (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *బహిర్గతం = (xkb_layout_సూచీ_t)(s[1] - '1');
        తిరిగి 3;
    }
    ఒకవేళ (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        తిరిగి 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    తిరిగి parse_layout_int_సూచీ_తోక(s, max_len, బహిర్గతం);
}

/* Special layout indices */
గణన layout_సూచీ_ranges {
    LAYOUT_సూచీ_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_సూచీ_మొదటి,
    LAYOUT_సూచీ_LATER,
    LAYOUT_సూచీ_గుణిజం,
    LAYOUT_సూచీ_ANY,
};

static_assert((xkb_layout_సూచీ_t) XKB_MAX_GROUPS <
              (xkb_layout_సూచీ_t) LAYOUT_సూచీ_SINGLE,
              "Cannot define special indices");
static_assert((xkb_layout_సూచీ_t) LAYOUT_సూచీ_SINGLE <
              (xkb_layout_సూచీ_t) LAYOUT_సూచీ_మొదటి &&
              (xkb_layout_సూచీ_t) LAYOUT_సూచీ_మొదటి <
              (xkb_layout_సూచీ_t) LAYOUT_సూచీ_LATER &&
              (xkb_layout_సూచీ_t) LAYOUT_సూచీ_LATER <
              (xkb_layout_సూచీ_t) LAYOUT_సూచీ_గుణిజం &&
              (xkb_layout_సూచీ_t) LAYOUT_సూచీ_గుణిజం <
              (xkb_layout_సూచీ_t) LAYOUT_సూచీ_ANY &&
              (xkb_layout_సూచీ_t) LAYOUT_సూచీ_ANY <
              (xkb_layout_సూచీ_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
స్థిర bool
వెలికితీయి_mapping_layout_సూచీ(స్థిరరాశి అక్షరం *s, కొలత_t max_len,
                             xkb_layout_సూచీ_t *బహిర్గతం)
{
    /* Check for minimal `[` + index + `]` */
    ఒకవేళ (max_len < 3 || s[0] != '[') {
        *బహిర్గతం = XKB_LAYOUT_INVALID;
        తిరిగి false;
    }

    /* Small numeric index */
    ఒకవేళ (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *బహిర్గతం = (xkb_layout_సూచీ_t)(s[1] - '1');
        తిరిగి true;
    }

    /* Named indices ranges */
    స్థిర స్థిరరాశి నిర్మాణం {
        స్థిరరాశి అక్షరం* name;
        కొలత_t కొలత;
        xkb_layout_సూచీ_t విలువ;
    } పేర్లు[] = {
        #నిర్వచించు సూచీ_RANGE(తోక, విలువ) \
            { (తోక), పరిమాణం(తోక), (xkb_layout_సూచీ_t)(విలువ) }
        సూచీ_RANGE("single]",   LAYOUT_సూచీ_SINGLE),
        సూచీ_RANGE("first]",    LAYOUT_సూచీ_మొదటి),
        సూచీ_RANGE("later]",    LAYOUT_సూచీ_LATER),
        సూచీ_RANGE("multiple]", LAYOUT_సూచీ_గుణిజం),
        సూచీ_RANGE("any]",      LAYOUT_సూచీ_ANY),
        #నిర్వచనం_రద్దు సూచీ_RANGE
    };

    కొరకు (కొలత_t k = 0; k < ARRAY_కొలత(పేర్లు); k++) {
        ఒకవేళ (max_len == పేర్లు[k].కొలత &&
            memcmp(&s[1], పేర్లు[k].name, పేర్లు[k].కొలత - 1) == 0) {
            *బహిర్గతం = పేర్లు[k].విలువ;
            తిరిగి true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *బహిర్గతం = XKB_LAYOUT_INVALID;
    స్థిరరాశి పూర్ణాంకం లెక్క = parse_layout_int_సూచీ_తోక(s, max_len, బహిర్గతం);
    తిరిగి లెక్క > 0 && (కొలత_t)లెక్క == max_len;
}

స్థిర అంతఃవరుస bool
is_mlvo_ముసుగు_defined(నిర్మాణం matcher *m, గణన నియమాలు_mlvo mlvo)
{
    తిరిగి m->mapping.defined_mlvo_ముసుగు & (1u << mlvo);
}

స్థిర శూన్య
matcher_mapping_సమితి_mlvo(నిర్మాణం matcher *m, నిర్మాణం scanner *s,
                         నిర్మాణం sval ident)
{
    గణన నియమాలు_mlvo mlvo;
    నిర్మాణం sval mlvo_sval;

    కొరకు (mlvo = 0; mlvo < MLVO_అంకె_ENTRIES; mlvo++) {
        mlvo_sval = నియమాలు_mlvo_svals[mlvo];

        ఒకవేళ (svaleq_ఉపసర్గ(mlvo_sval, ident))
            ఆపు;
    }

    /* Not found. */
    ఒకవేళ (mlvo >= MLVO_అంకె_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (గుర్తులేని పూర్ణాంకం) ident.len, ident.ప్రారంభం);
        m->mapping.చురుకైన = false;
        తిరిగి;
    }

    ఒకవేళ (is_mlvo_ముసుగు_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (గుర్తులేని పూర్ణాంకం) mlvo_sval.len, mlvo_sval.ప్రారంభం);
        m->mapping.చురుకైన = false;
        తిరిగి;
    }

    /* If there are leftovers still, it must be an index. */
    ఒకవేళ (mlvo_sval.len < ident.len) {
        xkb_layout_సూచీ_t idx;
        ఒకవేళ (!వెలికితీయి_mapping_layout_సూచీ(ident.ప్రారంభం + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (గుర్తులేని పూర్ణాంకం) mlvo_sval.len, mlvo_sval.ప్రారంభం);
            m->mapping.చురుకైన = false;
            తిరిగి;
        }

        ఒకవేళ (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        లేకపోతే ఒకవేళ (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        లేకపోతే {
            scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (గుర్తులేని పూర్ణాంకం) mlvo_sval.len, mlvo_sval.ప్రారంభం);
            m->mapping.చురుకైన = false;
            తిరిగి;
        }
    } లేకపోతే ఒకవేళ (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_సూచీ_t) LAYOUT_సూచీ_SINGLE;
    } లేకపోతే ఒకవేళ (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_సూచీ_t) LAYOUT_సూచీ_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    ఒకవేళ (((mlvo == MLVO_LAYOUT && is_mlvo_ముసుగు_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_ముసుగు_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.చురుకైన = false;
        తిరిగి;
    }

    m->mapping.mlvo_at_pos[m->mapping.అంకె_mlvo] = mlvo;
    m->mapping.defined_mlvo_ముసుగు |= (mlvo_ముసుగు_t) 1u << mlvo;
    m->mapping.అంకె_mlvo++;
}

స్థిర శూన్య
matcher_mapping_సమితి_layout_bounds(నిర్మాణం matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_సూచీ_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    ఎంపిక (idx) {
        స్థితి XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            ప్రతిజ్ఞా(!is_mlvo_ముసుగు_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_ముసుగు_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_ముసుగు = 0x1; /* active = true */
            ఆపు;
        స్థితి LAYOUT_సూచీ_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_కొలత(m->rmlvo.layouts));
            m->mapping.layouts_candidates_ముసుగు = (xkb_layout_ముసుగు_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            ఆపు;
        స్థితి LAYOUT_సూచీ_గుణిజం:
        స్థితి LAYOUT_సూచీ_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_కొలత(m->rmlvo.layouts));
            m->mapping.layouts_candidates_ముసుగు = (xkb_layout_ముసుగు_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            ఆపు;
        స్థితి LAYOUT_సూచీ_SINGLE:
        స్థితి LAYOUT_సూచీ_మొదటి:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        అప్రమేయ:
            /* Mere layout index */
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = idx;
            m->mapping.layout_idx_max = idx + 1;
            m->mapping.layouts_candidates_ముసుగు = UINT32_C(1) << idx;
    }
    m->mapping.has_గుణిజం_layouts = (
        m->mapping.layout_idx_max > m->mapping.layout_idx_min &&
        m->mapping.layout_idx_max - m->mapping.layout_idx_min > 1
    );
}

స్థిర శూన్య
matcher_mapping_సమితి_kccgst(నిర్మాణం matcher *m, నిర్మాణం scanner *s, నిర్మాణం sval ident)
{
    గణన నియమాలు_kccgst kccgst;
    నిర్మాణం sval kccgst_sval;

    కొరకు (kccgst = 0; kccgst < KCCGST_అంకె_ENTRIES; kccgst++) {
        kccgst_sval = నియమాలు_kccgst_svals[kccgst];

        ఒకవేళ (svaleq(నియమాలు_kccgst_svals[kccgst], ident))
            ఆపు;
    }

    /* Not found. */
    ఒకవేళ (kccgst >= KCCGST_అంకె_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (గుర్తులేని పూర్ణాంకం) ident.len, ident.ప్రారంభం);
        m->mapping.చురుకైన = false;
        తిరిగి;
    }

    ఒకవేళ (m->mapping.defined_kccgst_ముసుగు & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (గుర్తులేని పూర్ణాంకం) kccgst_sval.len, kccgst_sval.ప్రారంభం);
        m->mapping.చురుకైన = false;
        తిరిగి;
    }

    m->mapping.kccgst_at_pos[m->mapping.అంకె_kccgst] = kccgst;
    m->mapping.defined_kccgst_ముసుగు |= (kccgst_ముసుగు_t) 1u << kccgst;
    m->mapping.అంకె_kccgst++;
}

స్థిర bool
matcher_mapping_verify(నిర్మాణం matcher *m, నిర్మాణం scanner *s)
{
    ఒకవేళ (m->mapping.అంకె_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        వెళ్ళు skip;
    }

    ఒకవేళ (m->mapping.అంకె_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        వెళ్ళు skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    ఒకవేళ (is_mlvo_ముసుగు_defined(m, MLVO_LAYOUT)) {
        ప్రతిజ్ఞా(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        ఎంపిక (m->mapping.layout_idx) {
            స్థితి LAYOUT_సూచీ_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                ఒకవేళ (darray_కొలత(m->rmlvo.layouts) > 1)
                    వెళ్ళు skip;
                ఆపు;
            స్థితి LAYOUT_సూచీ_గుణిజం:
            స్థితి LAYOUT_సూచీ_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                ఒకవేళ (darray_కొలత(m->rmlvo.layouts) < 2)
                    వెళ్ళు skip;
                ఆపు;
            స్థితి LAYOUT_సూచీ_ANY:
            స్థితి LAYOUT_సూచీ_మొదటి:
                /* No restrictions */
                ఆపు;
            అప్రమేయ:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                ఒకవేళ (darray_కొలత(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_కొలత(m->rmlvo.layouts))
                    వెళ్ళు skip;
        }
    }

    ఒకవేళ (is_mlvo_ముసుగు_defined(m, MLVO_VARIANT)) {
        ప్రతిజ్ఞా(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        ఎంపిక (m->mapping.variant_idx) {
            స్థితి LAYOUT_సూచీ_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                ఒకవేళ (darray_కొలత(m->rmlvo.variants) > 1)
                    వెళ్ళు skip;
                ఆపు;
            స్థితి LAYOUT_సూచీ_ANY:
            స్థితి LAYOUT_సూచీ_LATER:
            స్థితి LAYOUT_సూచీ_మొదటి:
                /* No restriction */
                ఆపు;
            అప్రమేయ:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                ఒకవేళ (darray_కొలత(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_కొలత(m->rmlvo.variants))
                    వెళ్ళు skip;
        }
    }

    తిరిగి true;

skip:
    m->mapping.చురుకైన = false;
    తిరిగి false;
}

స్థిర శూన్య
matcher_నియమం_ప్రారంభం_new(నిర్మాణం matcher *m)
{
    memset(&m->నియమం_2, 0, పరిమాణం(m->నియమం_2));
    m->నియమం_2.skip = !m->mapping.చురుకైన;
}

స్థిర శూన్య
matcher_నియమం_సమితి_mlvo_common(నిర్మాణం matcher *m, నిర్మాణం scanner *s,
                             నిర్మాణం sval ident,
                             గణన mlvo_సరిపోలిక_type సరిపోలిక_type)
{
    ఒకవేళ (m->నియమం_2.అంకె_mlvo_విలువలు >= m->mapping.అంకె_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->నియమం_2.skip = true;
        తిరిగి;
    }
    m->నియమం_2.సరిపోలిక_type_at_pos[m->నియమం_2.అంకె_mlvo_విలువలు] = సరిపోలిక_type;
    m->నియమం_2.mlvo_విలువ_at_pos[m->నియమం_2.అంకె_mlvo_విలువలు] = ident;
    m->నియమం_2.అంకె_mlvo_విలువలు++;
}

స్థిర శూన్య
matcher_నియమం_సమితి_mlvo_wildcard(నిర్మాణం matcher *m, నిర్మాణం scanner *s,
                               గణన mlvo_సరిపోలిక_type సరిపోలిక_type)
{
    నిర్మాణం sval dummy = SVAL(ఖాళీ, 0);
    matcher_నియమం_సమితి_mlvo_common(m, s, dummy, సరిపోలిక_type);
}

స్థిర శూన్య
matcher_నియమం_సమితి_mlvo_సమూహం(నిర్మాణం matcher *m, నిర్మాణం scanner *s,
                            నిర్మాణం sval ident)
{
    matcher_నియమం_సమితి_mlvo_common(m, s, ident, MLVO_సరిపోలిక_సమూహం);
}

స్థిర శూన్య
matcher_నియమం_సమితి_mlvo(నిర్మాణం matcher *m, నిర్మాణం scanner *s,
                      నిర్మాణం sval ident)
{
    matcher_నియమం_సమితి_mlvo_common(m, s, ident, MLVO_సరిపోలిక_సాధారణ);
}

స్థిర శూన్య
matcher_నియమం_సమితి_kccgst(నిర్మాణం matcher *m, నిర్మాణం scanner *s,
                        నిర్మాణం sval ident)
{
    ఒకవేళ (m->నియమం_2.అంకె_kccgst_విలువలు >= m->mapping.అంకె_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->నియమం_2.skip = true;
        తిరిగి;
    }
    m->నియమం_2.kccgst_విలువ_at_pos[m->నియమం_2.అంకె_kccgst_విలువలు] = ident;
    m->నియమం_2.అంకె_kccgst_విలువలు++;
}

స్థిర bool
సరిపోలిక_సమూహం(నిర్మాణం matcher *m, నిర్మాణం sval సమూహం_name, నిర్మాణం sval గమ్యం)
{
    నిర్మాణం సమూహం *సమూహం = ఖాళీ;
    నిర్మాణం sval *element;
    bool దొరికింది = false;

    darray_foreach(సమూహం, m->groups) {
        ఒకవేళ (svaleq(సమూహం->name, సమూహం_name)) {
            దొరికింది = true;
            ఆపు;
        }
    }

    ఒకవేళ (!దొరికింది) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        తిరిగి false;
    }

    darray_foreach(element, సమూహం->elements)
        ఒకవేళ (svaleq(గమ్యం, *element))
            తిరిగి true;

    తిరిగి false;
}

స్థిర bool
సరిపోలిక_విలువ(నిర్మాణం matcher *m, స్థిరరాశి నిర్మాణం sval val, స్థిరరాశి నిర్మాణం sval గమ్యం,
            గణన mlvo_సరిపోలిక_type సరిపోలిక_type,
            గణన wildcard_సరిపోలిక_type wildcard_type)
{
    ఎంపిక (సరిపోలిక_type) {
        స్థితి MLVO_సరిపోలిక_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            తిరిగి wildcard_type == WILDCARD_సరిపోలిక_ALL || !!గమ్యం.len;
        స్థితి MLVO_సరిపోలిక_WILDCARD_NONE:
            తిరిగి !గమ్యం.len;
        స్థితి MLVO_సరిపోలిక_WILDCARD_SOME:
            తిరిగి !!గమ్యం.len;
        స్థితి MLVO_సరిపోలిక_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            తిరిగి true;
        స్థితి MLVO_సరిపోలిక_సమూహం:
            తిరిగి సరిపోలిక_సమూహం(m, val, గమ్యం);
        అప్రమేయ:
            ప్రతిజ్ఞా(సరిపోలిక_type == MLVO_సరిపోలిక_సాధారణ);
            తిరిగి svaleq(val, గమ్యం);
    }
}

స్థిర bool
సరిపోలిక_విలువ_and_mark(నిర్మాణం matcher *m, స్థిరరాశి నిర్మాణం sval val,
                     నిర్మాణం matched_sval *గమ్యం, గణన mlvo_సరిపోలిక_type సరిపోలిక_type,
                     గణన wildcard_సరిపోలిక_type wildcard_type,
                     xkb_layout_ముసుగు_t layouts)
{
    bool matched = సరిపోలిక_విలువ(m, val, గమ్యం->sval, సరిపోలిక_type, wildcard_type);
    ఒకవేళ (matched)
        గమ్యం->matched |= layouts;
    తిరిగి matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
స్థిర bool
expand_rmlvo_లోపల_kccgst_విలువ(నిర్మాణం matcher *m, నిర్మాణం scanner *s,
                             నిర్మాణం sval విలువ, xkb_layout_సూచీ_t layout_idx,
                             darray_char *expanded, కొలత_t *i)
{
    స్థిరరాశి అక్షరం *str = విలువ.ప్రారంభం;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    గణన నియమాలు_mlvo mlv;
    xkb_layout_సూచీ_t idx;
    అక్షరం pfx, sfx;
    నిర్మాణం matched_sval *expanded_విలువ;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    ఒకవేళ (str[*i] == 'i' &&
        (*i + 1 == విలువ.len || is_విలీనం_బహుళకం_ఉపసర్గ(str[*i + 1])))
    {
        ఒకవేళ (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_నియమాలు_INVALID_LAYOUT_సూచీ_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            వెళ్ళు error;
        }
        (*i)++;
        అక్షరం సూచీ_str[MAX_LAYOUT_సూచీ_STR_నిడివి + 1];
        స్థిరరాశి పూర్ణాంకం లెక్క = snprintf(సూచీ_str, పరిమాణం(సూచీ_str), "%"PRIu32,
                                   layout_idx + 1);
        ప్రతిజ్ఞా(లెక్క > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, సూచీ_str, (darray_కొలత_t)లెక్క);
        తిరిగి true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    ఒకవేళ (str[*i] == '(' ||
        is_విలీనం_బహుళకం_ఉపసర్గ(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        ఒకవేళ (str[*i] == '(') sfx = ')';
        ఒకవేళ (++(*i) >= విలువ.len) వెళ్ళు error;
    }

    /* Mandatory model/layout/variant specifier. */
    ఎంపిక (str[(*i)++]) {
    స్థితి 'm': mlv = MLVO_MODEL; ఆపు;
    స్థితి 'l': mlv = MLVO_LAYOUT; ఆపు;
    స్థితి 'v': mlv = MLVO_VARIANT; ఆపు;
    అప్రమేయ: వెళ్ళు error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_సూచీ = false;
    ఒకవేళ (*i < విలువ.len && str[*i] == '[') {
        ఒకవేళ (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            వెళ్ళు error;
        }

        పూర్ణాంకం consumed = వెలికితీయి_layout_సూచీ(str + (*i), విలువ.len - (*i), &idx);
        ఒకవేళ (consumed <= 0) వెళ్ళు error;
        ఒకవేళ (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            ప్రతిజ్ఞా(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_సూచీ = true;
        }
        *i += (కొలత_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    ఒకవేళ (sfx != 0) {
        ఒకవేళ (*i >= విలువ.len) వెళ్ళు error;
        ఒకవేళ (str[(*i)++] != sfx) వెళ్ళు error;
    }

    /* Get the expanded value. */
    expanded_విలువ = ఖాళీ;

    ఒకవేళ (mlv == MLVO_LAYOUT) {
        ఒకవేళ (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            ఒకవేళ (darray_కొలత(m->rmlvo.layouts) == 1)
                expanded_విలువ = &darray_అంశం(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } లేకపోతే ఒకవేళ (idx < darray_కొలత(m->rmlvo.layouts) &&
                   (variable_సూచీ || darray_కొలత(m->rmlvo.layouts) > 1)) {
                expanded_విలువ = &darray_అంశం(m->rmlvo.layouts, idx);
        }
    }
    లేకపోతే ఒకవేళ (mlv == MLVO_VARIANT) {
        ఒకవేళ (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            ఒకవేళ (darray_కొలత(m->rmlvo.variants) == 1)
                expanded_విలువ = &darray_అంశం(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } లేకపోతే ఒకవేళ (idx < darray_కొలత(m->rmlvo.variants) &&
                   (variable_సూచీ || darray_కొలత(m->rmlvo.variants) > 1)) {
                expanded_విలువ = &darray_అంశం(m->rmlvo.variants, idx);
        }
    }
    లేకపోతే ఒకవేళ (mlv == MLVO_MODEL) {
        expanded_విలువ = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    ఒకవేళ (!expanded_విలువ || expanded_విలువ->sval.len == 0) {
        తిరిగి true;
    }

    ఒకవేళ (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_విలువ->sval.ప్రారంభం,
                                 (darray_కొలత_t) expanded_విలువ->sval.len);
    ఒకవేళ (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    ఒకవేళ (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        స్థిరరాశి xkb_layout_ముసుగు_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_విలువ->matched |= layouts;
    } లేకపోతే {
        expanded_విలువ->matched |= (xkb_layout_ముసుగు_t)GLOBAL_MATCHED_LAYOUTS;
    }

    తిరిగి true;

error:
    scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                "invalid %%-expansion in value; not used");
    తిరిగి false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
స్థిర శూన్య
expand_qualifier_లోపల_kccgst_విలువ(
    నిర్మాణం matcher *m, నిర్మాణం scanner *s,
    నిర్మాణం sval విలువ, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_కొలత_t ఉపసర్గ_idx, కొలత_t *i)
{
    స్థిరరాశి అక్షరం *str = విలువ.ప్రారంభం;

    /* “all” followed by nothing or by a layout separator */
    ఒకవేళ (*i + 2 < విలువ.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == విలువ.len || is_విలీనం_బహుళకం_ఉపసర్గ(str[*i + 3]))) {
        ఒకవేళ (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_సందేశం_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        ఒకవేళ (darray_కొలత(m->rmlvo.layouts) > 1) {
            అక్షరం layout_సూచీ[MAX_LAYOUT_సూచీ_STR_నిడివి + 1];
            స్థిరరాశి darray_కొలత_t ఉపసర్గ_నిడివి =
                darray_కొలత(*expanded) - ఉపసర్గ_idx - 1;
            కొరకు (xkb_layout_సూచీ_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_కొలత(m->rmlvo.layouts));
                 l++)
            {
                ఒకవేళ (!has_separator)
                    darray_append(*expanded, విలీనం_DEFAULT_ఉపసర్గ);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_అంశం(*expanded, ఉపసర్గ_idx),
                                             ఉపసర్గ_నిడివి);
                /* Append index */
                స్థిరరాశి పూర్ణాంకం లెక్క = snprintf(layout_సూచీ, పరిమాణం(layout_సూచీ),
                                           "%"PRIu32, l + 1);
                ప్రతిజ్ఞా(లెక్క > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_సూచీ,
                                             (darray_కొలత_t)లెక్క);
            }
        }
        *i += 3;
    }
}

స్థిర అంతఃవరుస శూన్య
#నిర్వచితమైతే _MSC_VER
concat_kccgst(darray_char *into, darray_కొలత_t కొలత, లోపల_reads(కొలత) స్థిరరాశి అక్షరం* from)
#లేకపోతే
concat_kccgst(darray_char *into, darray_కొలత_t కొలత, స్థిరరాశి అక్షరం from[స్థిర కొలత])
#అయితే_ముగింపు
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    స్థిరరాశి bool from_plus = is_విలీనం_బహుళకం_ఉపసర్గ(from[0]);
    ఒకవేళ (from_plus || darray_ఖాళీలు(*into)) {
        darray_appends_nullterminate(*into, from, కొలత);
    } లేకపోతే {
        స్థిరరాశి అక్షరం ch =
            (అక్షరం) (darray_ఖాళీలు(*into) ? '\0' : darray_అంశం(*into, 0));
        స్థిరరాశి bool into_plus = is_విలీనం_బహుళకం_ఉపసర్గ(ch);
        ఒకవేళ (into_plus)
            darray_prepends_nullterminate(*into, from, కొలత);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
స్థిర bool
append_expanded_kccgst_విలువ(నిర్మాణం matcher *m, నిర్మాణం scanner *s,
                             bool విలీనం, darray_char *గమ్యం, నిర్మాణం sval విలువ,
                             xkb_layout_సూచీ_t layout_idx)
{
    స్థిరరాశి అక్షరం *str = విలువ.ప్రారంభం;
    darray_char expanded = darray_new();
    darray_కొలత_t చివరి_అంశం_idx = 0;
    bool has_separator = false;

    కొరకు (కొలత_t i = 0; i < విలువ.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        ఎంపిక (str[i]) {
            /* Qualifier */
            స్థితి ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_లోపల_kccgst_విలువ(m, s, విలువ, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 చివరి_అంశం_idx, &i);
                ఆపు;
            /* Expansion */
            స్థితి '%':
                i++;
                ఒకవేళ (i >= విలువ.len ||
                    !expand_rmlvo_లోపల_kccgst_విలువ(m, s, విలువ, layout_idx,
                                                  &expanded, &i))
                        వెళ్ళు error;
                ఆపు;
            /* New item */
            స్థితి విలీనం_OVERRIDE_ఉపసర్గ:
            స్థితి విలీనం_AUGMENT_ఉపసర్గ:
            స్థితి విలీనం_REPLACE_ఉపసర్గ:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                చివరి_అంశం_idx = darray_కొలత(expanded) - 1;
                has_separator = true;
                ఆపు;
            /* Just a normal character. */
            అప్రమేయ:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    ఒకవేళ (విలీనం) {
        ఒకవేళ (!darray_ఖాళీలు(expanded))
            concat_kccgst(గమ్యం, darray_కొలత(expanded), darray_వస్తువులు(expanded));
    } లేకపోతే {
        darray_concat(*గమ్యం, expanded);
    }
    darray_free(expanded);
    తిరిగి true;

error:
    darray_free(expanded);
    తిరిగి false;
}

స్థిర bool
matcher_append_pending_kccgst(నిర్మాణం matcher *m)
{
    ఒకవేళ (!m->mapping.has_గుణిజం_layouts)
        తిరిగి true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    కొరకు (kccgst_సూచీ_t i = 0; i < m->mapping.అంకె_kccgst; i++) {
        స్థిరరాశి గణన నియమాలు_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        కొరకు (xkb_layout_సూచీ_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            నమోదు స్థిరరాశి నిర్మాణం kccgst_బఫరు* స్థిరరాశి buf = &m->pending_kccgst;
            కొలత_t స్థానభ్రంశం = 0;
            కొరకు (darray_కొలత_t k = 0; k < darray_కొలత(buf->slices); k++) {
                నమోదు స్థిరరాశి నిర్మాణం kccgst_బఫరు_ముక్క * స్థిరరాశి ముక్క =
                    &darray_అంశం(buf->slices, k);
                ఒకవేళ (ముక్క->kccgst == kccgst && ముక్క->layout == layout &&
                    ముక్క->నిడివి)
                    concat_kccgst(&m->kccgst[kccgst], ముక్క->నిడివి,
                                  darray_వస్తువులు(buf->బఫరు) + స్థానభ్రంశం);
                స్థానభ్రంశం += ముక్క->నిడివి;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_గుణిజం_layouts = false;
    తిరిగి true;
}

స్థిర శూన్య
matcher_నియమం_verify(నిర్మాణం matcher *m, నిర్మాణం scanner *s)
{
    ఒకవేళ (m->నియమం_2.అంకె_mlvo_విలువలు != m->mapping.అంకె_mlvo ||
        m->నియమం_2.అంకె_kccgst_విలువలు != m->mapping.అంకె_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                    "invalid rule: must have same number of values "
                    "as mapping line; ignoring rule");
        m->నియమం_2.skip = true;
    }
}

/*
 * NOTE: The wildcard `*` behaves differently depending on the MLVO field:
 * - model and options: matches any value, including empty;
 * - layout and variant: matches any *non-empty* value.
 *
 * This aligns with the implementation in libxkbfile and xserver, with
 * the exception of options, where `*` is entirely ignored.
 *
 * The underlying rationale for this discrepancy across MLVO fields is
 * undocumented.
 *
 * In xkbcommon, `*` behaves identically for both model and options to
 * maintain consistency and simplicity. This divergence is unlikely
 * to have a practical impact: to the best of our knowledge, `*` has
 * no real-world use for the options field.
 *
 * | MLVO field | Wildcard `*` matches        |
 * | ---------- | --------------------------- |
 * | Model      | Any value (including empty) |
 * | Option     | Any value (including empty) |
 * | Layout     | Non-empty values only       |
 * | Variant    | Non-empty values only       |
 *
 */

/** Match the `model` MLVO field
 *
 * `model` input is a single value.
 */
స్థిర bool
matcher_నియమం_సరిపోలిక_model(నిర్మాణం matcher *m, స్థిరరాశి నిర్మాణం sval విలువ,
                         గణన mlvo_సరిపోలిక_type సరిపోలిక_type)
{
    తిరిగి సరిపోలిక_విలువ_and_mark(
        m, విలువ, &m->rmlvo.model, సరిపోలిక_type, WILDCARD_సరిపోలిక_ALL,
        /* layout-independent */
        (xkb_layout_ముసుగు_t)GLOBAL_MATCHED_LAYOUTS
    );
}

/**
 * Match the `option` MLVO field.
 *
 * `option` input is a list of values (*not* layout-indexed).
 *
 * Matching an option field requires considering the entire list of input options
 * (e.g. `grp:menu_toggle,caps:swapescape`), because any number of them may
 * legitimately satisfy a single rule at once.
 *
 * - If the rule has no `layout` nor `variant` field, evaluation stops early on
 *   the first match.
 * - Otherwise each option potentially covers a different subset of the
 *   candidate layout indices. Candidates are narrowed by accumulating which
 *   candidates get covered:
 *
 *   - A layout-specific option (e.g. `grp:menu_toggle!1`, encoded as a non-zero
 *     `option->layouts` mask) can only cover candidates it applies to, so it
 *     is tried against `unmatched & option->layouts`.
 *   - A layout-generic option (e.g. `grp:menu_toggle`) is not restricted to
 *     specific layout indices, so a match covers every remaining candidate at
 *     once.
 *
 *   Matching stops early once every candidate is covered; otherwise every
 *   input option is tried. Any candidate left uncovered at the end cannot
 *   match the next RMLVO field (if any), so it is dropped from the candidates
 *   list.
 *
 * NOTE: The matching logic here differs from `model`, `layout`, and `variant`.
 * While the rule still provides a single value to match against, how that value
 * relates to candidate layout indices varies:
 *
 * | MLVO field | Input type                          | Candidates constraint   |
 * | ---------- | ----------------------------------- | ----------------------- |
 * | `model`    | single value                        | none                    |
 * | `layout`   | list of layout-indexed values       | 1-to-1                  |
 * | `variant`  | list of layout-indexed values       | 1-to-1                  |
 * | `option`   | list of values (not layout-indexed) | many-to-many            |
 *
 */
స్థిర bool
matcher_నియమం_సరిపోలిక_option(నిర్మాణం matcher *m, స్థిరరాశి నిర్మాణం sval విలువ,
                          గణన mlvo_సరిపోలిక_type సరిపోలిక_type,
                          xkb_layout_ముసుగు_t *candidates)
{
    /* There is always at least one value */
    ప్రతిజ్ఞా(!darray_ఖాళీలు(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_ముసుగు_t unmatched =
        (m->mapping.defined_mlvo_ముసుగు & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    నిర్మాణం matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        స్థిరరాశి xkb_layout_ముసుగు_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_ముసుగు_t)GLOBAL_MATCHED_LAYOUTS;
        ఒకవేళ (matchable && సరిపోలిక_విలువ_and_mark(m, విలువ, option, సరిపోలిక_type,
                                              WILDCARD_సరిపోలిక_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            ఒకవేళ (!unmatched)
                ఆపు;
        }
    }
    ఒకవేళ (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    తిరిగి matched;
}

/**
 * Match the `layout` or `variant` field
 *
 * Input is a list of layout-indexed values.
 *
 * The value of the rule’s field is tested against each remaining candidate
 * index using the corresponding RMLVO input list.
 *
 * - An index that fails to match can never satisfy this rule, so it is
 *   cleared from the candidates set. This narrows what later MLVO fields in
 *   the same rule still need to consider.
 * - An index that matches remains a candidate. Evaluation stops as soon as no
 *   candidates remain, since nothing left can satisfy the rule.
 */
స్థిర bool
matcher_నియమం_సరిపోలిక_layout_or_variant(నిర్మాణం matcher *m,
                                     darray_matched_sval *ఆదానం,
                                     స్థిరరాశి నిర్మాణం sval విలువ,
                                     గణన mlvo_సరిపోలిక_type సరిపోలిక_type,
                                     xkb_layout_ముసుగు_t *candidates)
{
    /* There is always at least one value */
    ప్రతిజ్ఞా(!darray_ఖాళీలు(*ఆదానం));

    bool matched = false;
    /* Loop over the layout index range */
    కొరకు (xkb_layout_సూచీ_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        స్థిరరాశి xkb_layout_ముసుగు_t layout = (UINT32_C(1) << idx);
        ఒకవేళ (layout & *candidates) {
            నిర్మాణం matched_sval *గమ్యం = &darray_అంశం(*ఆదానం, idx);
            ఒకవేళ (సరిపోలిక_విలువ_and_mark(m, విలువ, గమ్యం, సరిపోలిక_type,
                                     WILDCARD_సరిపోలిక_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } లేకపోతే {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    తిరిగి matched;
}

/**
 * Try to match the current rule against the RMLVO input; if every MLVO
 * field in the mapping matches, apply the rule’s KcCGST value(s).
 *
 * A mapping’s header line lists which MLVO fields it tests (some subset
 * of `model`, `layout`, `variant`, `option`, in any order) and which
 * KcCGST fields it produces. When the RMLVO input has several layouts
 * (e.g. `us,de,fr`), each rule needs to be checked against each layout
 * index independently. A candidate layout mask tracks the layout indices
 * valid for the current rule. Each MLVO field affects the candidates
 * layout indices differently:
 *
 * - `model` is layout-independent: it matches globally, for every
 *   candidate at once, or not at all.
 * - `layout` and `variant` narrow the mask directly: a candidate index
 *   survives only if the RMLVO value at that index matches the rule.
 * - `option` is different because the RMLVO input may contain multiple
 *   option values, possibly covering different candidates (or all of them,
 *   for a layout-generic option). A layout candidate survives only if some
 *   option matches and covers it.
 *
 * If any MLVO field fails to match, the rule cannot apply and this function
 * returns immediately, without changing the KcCGST output. For fields that
 * operate on layout indices (`layout`, `variant`, and `option`), this occurs
 * once no candidate layout indices remain.
 *
 * If every field matches, the KcCGST value(s) are processed for `%`-expansions
 * (e.g., `%l` for matched layout, `%v` for variant, `%m` for model, `%i` for
 * layout index) and applied against the remaining candidate layouts. If the
 * ruleset spans multiple layout indices and includes an `option` field, the
 * `%`-expanded values are buffered and subsequently merged to guarantee they
 * follow layout order first, rule order second.
 *
 * Within a rule set, a successful match normally terminates evaluation.
 * The only exception is rules containing an `option` field, which do not
 * terminate the set so that additional matching options may contribute.
 */
స్థిర శూన్య
matcher_నియమం_apply_if_matches(నిర్మాణం matcher *m, నిర్మాణం scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_ముసుగు_t candidate_layouts = m->mapping.layouts_candidates_ముసుగు;

    /* Loop over MLVO pattern components */
    కొరకు (mlvo_సూచీ_t i = 0; i < m->mapping.అంకె_mlvo; i++) {
        స్థిరరాశి గణన నియమాలు_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        స్థిరరాశి నిర్మాణం sval విలువ = m->నియమం_2.mlvo_విలువ_at_pos[i];
        స్థిరరాశి గణన mlvo_సరిపోలిక_type సరిపోలిక_type = m->నియమం_2.సరిపోలిక_type_at_pos[i];
        bool matched = false;

        ఎంపిక (mlvo) {
        స్థితి MLVO_MODEL:
            matched = matcher_నియమం_సరిపోలిక_model(m, విలువ, సరిపోలిక_type);
            ఆపు;
        స్థితి MLVO_LAYOUT:
            matched = matcher_నియమం_సరిపోలిక_layout_or_variant(
                m, &m->rmlvo.layouts, విలువ, సరిపోలిక_type, &candidate_layouts
            );
            ఆపు;
        స్థితి MLVO_VARIANT:
            matched = matcher_నియమం_సరిపోలిక_layout_or_variant(
                m, &m->rmlvo.variants, విలువ, సరిపోలిక_type, &candidate_layouts
            );
            ఆపు;
        స్థితి MLVO_OPTION:
            matched = matcher_నియమం_సరిపోలిక_option(
                m, విలువ, సరిపోలిక_type, &candidate_layouts
            );
            ఆపు;
        అప్రమేయ: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_అంకె_ENTRIES - 1,
                          "Unexpected MLVO field");
            ప్రతిజ్ఞా(!"Unreachable");
        }
        }

        ఒకవేళ (!matched)
            తిరిగి;
    }

    ఒకవేళ (m->mapping.has_గుణిజం_layouts) {
        /* Special index: loop over the index range */
        కొరకు (xkb_layout_సూచీ_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            ఒకవేళ (candidate_layouts & (UINT32_C(1) << idx)) {
                కొరకు (kccgst_సూచీ_t i = 0; i < m->mapping.అంకె_kccgst; i++) {
                    స్థిరరాశి గణన నియమాలు_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    స్థిరరాశి నిర్మాణం sval విలువ = m->నియమం_2.kccgst_విలువ_at_pos[i];
                    /*
                     * [NOTE] Layout index ranges and merging KcCGST values
                     *
                     * Layout indices match following first the order of the
                     * rules in the file, then their natural order. So do not
                     * merge with the output for now but buffer the resulting
                     * KcCGST value and wait reaching the end of the rule set.
                     *
                     * Because the rule set may also involve options, it may
                     * match multiple times for the *same* layout index. So
                     * buffer the result of *each* match.
                     *
                     * When the end of the rule set is reached, merge buffered
                     * KcCGST sequentially, following first the layouts order,
                     * then the order of the rules in the file.
                     *
                     * Example:
                     *
                     * ! model = symbols
                     *   *     = pc
                     * ! layout[any] option = symbols
                     *   C           1      = +c1:%i
                     *   C           2      = +c2:%i
                     *   B           3      = skip
                     *   B           4      = +b:%i
                     *
                     * The result of {layout: "A,B,C", options: "4,3,2,1"} is:
                     * symbols = pc+b:2+c1:3+c2:3.
                     *
                     * - `skip` was dropped because it has no explicit merge
                     *   mode;
                     * - although every rule was matched in order, the resulting
                     *   order of the symbols follows the order of the layouts,
                     *   so `+b` appears before `+c1` and `+c2`.
                     * - the relative order of the options for layout C follows
                     *   the order within the rule set, not the order of RMLVO.
                     */
                    నిర్మాణం kccgst_బఫరు * స్థిరరాశి buf = &m->pending_kccgst;
                    స్థిరరాశి darray_కొలత_t prev_బఫరు_నిడివి =
                        darray_కొలత(buf->బఫరు);
                    append_expanded_kccgst_విలువ(m, s, false, &buf->బఫరు,
                                                 విలువ, idx);
                    స్థిరరాశి uint32_t నిడివి = (uint32_t) (darray_కొలత(buf->బఫరు)
                                          - prev_బఫరు_నిడివి);
                    స్థిరరాశి నిర్మాణం kccgst_బఫరు_ముక్క ముక్క = {
                        .నిడివి = నిడివి,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, ముక్క);
                }
            }
        }
    } లేకపోతే {
        /* Numeric index or no index */
        కొరకు (kccgst_సూచీ_t i = 0; i < m->mapping.అంకె_kccgst; i++) {
            గణన నియమాలు_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            నిర్మాణం sval విలువ = m->నియమం_2.kccgst_విలువ_at_pos[i];
            append_expanded_kccgst_విలువ(m, s, true, &m->kccgst[kccgst], విలువ,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    ఒకవేళ (!(is_mlvo_ముసుగు_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_ముసుగు &= ~candidate_layouts;
    }
}

స్థిర గణన నియమాలు_token
gettok(నిర్మాణం matcher *m, నిర్మాణం scanner *s)
{
    తిరిగి lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
స్థిర bool
matcher_సరిపోలిక(నిర్మాణం matcher *m, నిర్మాణం scanner *s,
              గుర్తులేని పూర్ణాంకం include_లోతు,
              స్థిరరాశి అక్షరం *string, కొలత_t len,
              స్థిరరాశి అక్షరం *file_name)
{
    గణన నియమాలు_token tok;

    ఒకవేళ (!m)
        తిరిగి false;

initial:
    ఎంపిక (tok = gettok(m, s)) {
    స్థితి TOK_BANG:
        వెళ్ళు bang;
    స్థితి TOK_చివర_యొక్క_పంక్తి:
        వెళ్ళు initial;
    స్థితి TOK_చివర_యొక్క_FILE:
        వెళ్ళు ముగింపు;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

bang:
    ఎంపిక (tok = gettok(m, s)) {
    స్థితి TOK_సమూహం_NAME:
        matcher_సమూహం_ప్రారంభం_new(m, m->val.string);
        వెళ్ళు సమూహం_name;
    స్థితి TOK_INCLUDE:
        వెళ్ళు include_statement;
    స్థితి TOK_IDENTIFIER:
        matcher_mapping_ప్రారంభం_new(m);
        matcher_mapping_సమితి_mlvo(m, s, m->val.string);
        వెళ్ళు mapping_mlvo;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

సమూహం_name:
    ఎంపిక (tok = gettok(m, s)) {
    స్థితి TOK_EQUALS:
        వెళ్ళు సమూహం_element;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

సమూహం_element:
    ఎంపిక (tok = gettok(m, s)) {
    స్థితి TOK_IDENTIFIER:
        matcher_సమూహం_కూడు_element(m, s, m->val.string);
        వెళ్ళు సమూహం_element;
    స్థితి TOK_చివర_యొక్క_పంక్తి:
        వెళ్ళు initial;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

include_statement:
    ఎంపిక (tok = gettok(m, s)) {
    స్థితి TOK_IDENTIFIER:
        matcher_include(m, s, include_లోతు, m->val.string);
        వెళ్ళు include_statement_చివర;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

include_statement_చివర:
    ఎంపిక (tok = gettok(m, s)) {
    స్థితి TOK_చివర_యొక్క_పంక్తి:
        వెళ్ళు initial;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

mapping_mlvo:
    ఎంపిక (tok = gettok(m, s)) {
    స్థితి TOK_IDENTIFIER:
        ఒకవేళ (m->mapping.చురుకైన)
            matcher_mapping_సమితి_mlvo(m, s, m->val.string);
        వెళ్ళు mapping_mlvo;
    స్థితి TOK_EQUALS:
        వెళ్ళు mapping_kccgst;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

mapping_kccgst:
    ఎంపిక (tok = gettok(m, s)) {
    స్థితి TOK_IDENTIFIER:
        ఒకవేళ (m->mapping.చురుకైన)
            matcher_mapping_సమితి_kccgst(m, s, m->val.string);
        వెళ్ళు mapping_kccgst;
    స్థితి TOK_చివర_యొక్క_పంక్తి:
        ఒకవేళ (m->mapping.చురుకైన && matcher_mapping_verify(m, s)) {
            matcher_mapping_సమితి_layout_bounds(m);
            ఒకవేళ (m->mapping.has_గుణిజం_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_కొలత(m->pending_kccgst.బఫరు) = 0;
                darray_కొలత(m->pending_kccgst.slices) = 0;
            }
        }
        వెళ్ళు నియమం_mlvo_మొదటి;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

నియమం_mlvo_మొదటి:
    ఎంపిక (tok = gettok(m, s)) {
    స్థితి TOK_BANG:
        matcher_append_pending_kccgst(m);
        వెళ్ళు bang;
    స్థితి TOK_చివర_యొక్క_పంక్తి:
        వెళ్ళు నియమం_mlvo_మొదటి;
    స్థితి TOK_చివర_యొక్క_FILE:
        matcher_append_pending_kccgst(m);
        వెళ్ళు ముగింపు;
    అప్రమేయ:
        matcher_నియమం_ప్రారంభం_new(m);
        వెళ్ళు నియమం_mlvo_no_tok;
    }

నియమం_mlvo:
    tok = gettok(m, s);
నియమం_mlvo_no_tok:
    ఎంపిక (tok) {
    స్థితి TOK_IDENTIFIER:
        ఒకవేళ (!m->నియమం_2.skip)
            matcher_నియమం_సమితి_mlvo(m, s, m->val.string);
        వెళ్ళు నియమం_mlvo;
    స్థితి TOK_WILD_CARD_నక్షత్రం:
        ఒకవేళ (!m->నియమం_2.skip)
            matcher_నియమం_సమితి_mlvo_wildcard(m, s, MLVO_సరిపోలిక_WILDCARD_LEGACY);
        వెళ్ళు నియమం_mlvo;
    స్థితి TOK_WILD_CARD_NONE:
        ఒకవేళ (!m->నియమం_2.skip)
            matcher_నియమం_సమితి_mlvo_wildcard(m, s, MLVO_సరిపోలిక_WILDCARD_NONE);
        వెళ్ళు నియమం_mlvo;
    స్థితి TOK_WILD_CARD_SOME:
        ఒకవేళ (!m->నియమం_2.skip)
            matcher_నియమం_సమితి_mlvo_wildcard(m, s, MLVO_సరిపోలిక_WILDCARD_SOME);
        వెళ్ళు నియమం_mlvo;
    స్థితి TOK_WILD_CARD_ANY:
        ఒకవేళ (!m->నియమం_2.skip)
            matcher_నియమం_సమితి_mlvo_wildcard(m, s, MLVO_సరిపోలిక_WILDCARD_ANY);
        వెళ్ళు నియమం_mlvo;
    స్థితి TOK_సమూహం_NAME:
        ఒకవేళ (!m->నియమం_2.skip)
            matcher_నియమం_సమితి_mlvo_సమూహం(m, s, m->val.string);
        వెళ్ళు నియమం_mlvo;
    స్థితి TOK_EQUALS:
        వెళ్ళు నియమం_kccgst;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

నియమం_kccgst:
    ఎంపిక (tok = gettok(m, s)) {
    స్థితి TOK_IDENTIFIER:
        ఒకవేళ (!m->నియమం_2.skip)
            matcher_నియమం_సమితి_kccgst(m, s, m->val.string);
        వెళ్ళు నియమం_kccgst;
    స్థితి TOK_చివర_యొక్క_పంక్తి:
        ఒకవేళ (!m->నియమం_2.skip)
            matcher_నియమం_verify(m, s);
        ఒకవేళ (!m->నియమం_2.skip)
            matcher_నియమం_apply_if_matches(m, s);
        వెళ్ళు నియమం_mlvo_మొదటి;
    అప్రమేయ:
        వెళ్ళు unexpected;
    }

unexpected:
    ఎంపిక (tok) {
    స్థితి TOK_ERROR:
        వెళ్ళు error;
    అప్రమేయ:
        వెళ్ళు state_error;
    }

ముగింపు:
    తిరిగి true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_నియమాలు_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    తిరిగి false;
}
// NOLINTEND(readability-trivial-switch)

స్థిర bool
read_నియమాలు_file(నిర్మాణం xkb_context *ctx,
                నిర్మాణం matcher *matcher,
                గుర్తులేని పూర్ణాంకం include_లోతు,
                FILE *file,
                స్థిరరాశి అక్షరం *మార్గం)
{
    bool ret;
    అక్షరం *string;
    కొలత_t కొలత;
    నిర్మాణం scanner scanner;

    ఒకవేళ (!map_file(file, &string, &కొలత)) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                మార్గం, strerror(errno));
        తిరిగి false;
    }

    scanner_init(&scanner, matcher->ctx, string, కొలత, మార్గం, ఖాళీ);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    ఒకవేళ (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, కొలత);
        తిరిగి false;
    }

    ret = matcher_సరిపోలిక(matcher, &scanner, include_లోతు, string, కొలత, మార్గం);
    unmap_file(string, కొలత);
    తిరిగి ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
స్థిర bool
xkb_resolve_partial_నియమాలు(నిర్మాణం xkb_context *ctx, అక్షరం *మార్గం, కొలత_t మార్గం_కొలత,
                          స్థిరరాశి అక్షరం* నియమాలు, స్థిరరాశి అక్షరం* suffix,
                          నిర్మాణం matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    అక్షరం partial_నియమాలు[60]; /* Arbitrary, but we do not expect long names */
    ఒకవేళ (unlikely(!snprintf_సురక్షిత(partial_నియమాలు, పరిమాణం(partial_నియమాలు),
                                "%s%s", నియమాలు, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", నియమాలు, suffix);
        తిరిగి false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = 0;
    FILE *file = ఖాళీ;
    స్థిరరాశి కొలత_t len = అక్షరశ్రేణి_పొడవు(partial_నియమాలు);
    అయితే ((file = కనుగొను_File_లోపల_Xkb_మార్గం(ctx, "(unknown)",
                                     partial_నియమాలు, len, FILE_TYPE_నియమాలు,
                                     మార్గం, మార్గం_కొలత, &స్థానభ్రంశం, false)) != ఖాళీ) {
        స్థిరరాశి bool ok = read_నియమాలు_file(ctx, matcher, 0, file, మార్గం);
        fclose(file);
        ఒకవేళ (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", మార్గం);
            తిరిగి false;
        }
        స్థానభ్రంశం++;
    }

    తిరిగి true;
}

స్థిర bool
xkb_resolve_నియమాలు(నిర్మాణం xkb_context *ctx,
                  స్థిరరాశి అక్షరం* నియమాలు, నిర్మాణం matcher *matcher,
                  నిర్మాణం xkb_component_పేర్లు *బహిర్గతం,
                  xkb_layout_సూచీ_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    గుర్తులేని పూర్ణాంకం స్థానభ్రంశం = 0;
    అక్షరం మార్గం[PATH_MAX];
    FILE * స్థిరరాశి file = కనుగొను_File_లోపల_Xkb_మార్గం(ctx, "(unknown)",
                                          నియమాలు, అక్షరశ్రేణి_పొడవు(నియమాలు), FILE_TYPE_నియమాలు,
                                          మార్గం, పరిమాణం(మార్గం), &స్థానభ్రంశం, true);
    ఒకవేళ (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", నియమాలు);
        వెళ్ళు err_బహిర్గతం;
    }

    /*
     * Final rule set is equivalent to:
     *
     * ! include <include path 1>/rules/<rules>.pre  // only if defined
     * …
     * ! include <include path n>/rules/<rules>.pre  // only if defined
     * ! include <rules>                             // main rules file
     * ! include <include path 1>/rules/<rules>.post // only if defined
     * …
     * ! include <include path n>/rules/<rules>.post // only if defined
     */

    /* XKB extension: resolve optional <rules>.pre files */
    ret = xkb_resolve_partial_నియమాలు(ctx, మార్గం, పరిమాణం(మార్గం),
                                    నియమాలు, ".pre", matcher);
    ఒకవేళ (!ret)
        వెళ్ళు err_బహిర్గతం;

    /* Resolve main <rules> file */
    ret = read_నియమాలు_file(ctx, matcher, 0, file, మార్గం);
    ఒకవేళ (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", మార్గం);
        వెళ్ళు err_బహిర్గతం;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_నియమాలు(ctx, మార్గం, పరిమాణం(మార్గం),
                                    నియమాలు, ".post", matcher);
    ఒకవేళ (!ret)
        వెళ్ళు err_బహిర్గతం;

    /* Check we got required components */
    ఒకవేళ (darray_ఖాళీలు(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_ఖాళీలు(matcher->kccgst[KCCGST_TYPES]) ||
        darray_ఖాళీలు(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_ఖాళీలు(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", నియమాలు);
        ret = false;
        వెళ్ళు err_బహిర్గతం;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &బహిర్గతం->keycodes, ఖాళీ);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &బహిర్గతం->types, ఖాళీ);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &బహిర్గతం->compatibility, ఖాళీ);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &బహిర్గతం->symbols, ఖాళీ);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &బహిర్గతం->geometry, ఖాళీ);

    నిర్మాణం matched_sval *mval = &matcher->rmlvo.model;
    ఒకవేళ (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_విలువ,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (గుర్తులేని పూర్ణాంకం) mval->sval.len, mval->sval.ప్రారంభం, నియమాలు);
    darray_foreach(mval, matcher->rmlvo.layouts)
        ఒకవేళ (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_విలువ,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (గుర్తులేని పూర్ణాంకం) mval->sval.len, mval->sval.ప్రారంభం, నియమాలు);
    darray_foreach(mval, matcher->rmlvo.variants)
        ఒకవేళ (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_విలువ,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (గుర్తులేని పూర్ణాంకం) mval->sval.len, mval->sval.ప్రారంభం, నియమాలు);
    darray_foreach(mval, matcher->rmlvo.options) {
        ఒకవేళ (!mval->sval.len)
            కొనసాగు;

        ఒకవేళ (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_విలువ,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (గుర్తులేని పూర్ణాంకం) mval->sval.len, mval->sval.ప్రారంభం, నియమాలు);
        } లేకపోతే ఒకవేళ (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            స్థిరరాశి xkb_layout_ముసుగు_t unmatched = (mval->layouts & ~mval->matched);
            ప్రతిజ్ఞా(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_విలువ,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (గుర్తులేని పూర్ణాంకం)mval->sval.len, mval->sval.ప్రారంభం,
                    unmatched, నియమాలు);
        }
    }

    /* Set the number of explicit layouts */
    ఒకవేళ (బహిర్గతం->symbols != ఖాళీ && explicit_layouts != ఖాళీ) {
        *explicit_layouts = 1; /* at least one group */
        స్థిరరాశి అక్షరం *symbols = బహిర్గతం->symbols;
        /* Take the highest modifier */
        అయితే ((symbols = strchr(symbols, ':')) != ఖాళీ && symbols[1] != '\0') {
            xkb_layout_సూచీ_t సమూహం = 0;
            స్థిరరాశి పూర్ణాంకం లెక్క = parse_dec_గమ్యం_uint32_t(++symbols, SIZE_MAX, &సమూహం);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            ఒకవేళ (లెక్క > 0 && (symbols[లెక్క] == '\0' ||
                is_విలీనం_బహుళకం_ఉపసర్గ(symbols[లెక్క])) &&
                సమూహం > 0 && సమూహం <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, సమూహం);
                symbols += లెక్క;
            }
        }
    }

err_బహిర్గతం:
    ఒకవేళ (file)
        fclose(file);

    తిరిగి ret;
}

bool
xkb_components_from_rmlvo_builder(స్థిరరాశి నిర్మాణం xkb_rmlvo_builder *rmlvo,
                                  నిర్మాణం xkb_component_పేర్లు *బహిర్గతం,
                                  xkb_layout_సూచీ_t *explicit_layouts)
{
    స్థిరరాశి అక్షరం *నియమాలు = rmlvo->నియమాలు;
    నిర్మాణం matcher *matcher = matcher_new_from_rmlvo(rmlvo, &నియమాలు);
    ఒకవేళ (!matcher)
        తిరిగి false;

    స్థిరరాశి bool ret = xkb_resolve_నియమాలు(rmlvo->ctx, నియమాలు, matcher,
                                       బహిర్గతం, explicit_layouts);

    matcher_free(matcher);
    తిరిగి ret;
}


bool
xkb_components_from_నియమాలు_పేర్లు(నిర్మాణం xkb_context *ctx,
                                స్థిరరాశి నిర్మాణం xkb_నియమం_పేర్లు *rmlvo,
                                నిర్మాణం xkb_component_పేర్లు *బహిర్గతం,
                                xkb_layout_సూచీ_t *explicit_layouts)
{
    నిర్మాణం matcher *matcher = matcher_new_from_పేర్లు(ctx, rmlvo);
    ఒకవేళ (!matcher)
        తిరిగి false;

    స్థిరరాశి bool ret = xkb_resolve_నియమాలు(ctx, rmlvo->నియమాలు, matcher,
                                       బహిర్గతం, explicit_layouts);

    matcher_free(matcher);
    తిరిగి ret;
}
