/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdint.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "rules.h"
#अन्तर्भाव "rmlvo.h"
#अन्तर्भाव "include.h"
#अन्तर्भाव "scanner-utils.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-numbers.h"
#अन्तर्भाव "util-paths.h"

#परिभाषय MAX_INCLUDE_गभीरता 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
सङ्घ lvalue {
    संरचना sval string;
};

गणना नियमाः_token {
    TOK_अन्तः_सम्बन्धि_FILE = 0,
    TOK_अन्तः_सम्बन्धि_पङ्क्तिः,
    TOK_IDENTIFIER,
    TOK_गणः_NAME,
    TOK_BANG,
    TOK_EQUALS,
    TOK_WILD_CARD_तारकम्,
    TOK_WILD_CARD_NONE,
    TOK_WILD_CARD_SOME,
    TOK_WILD_CARD_ANY,
    TOK_INCLUDE,
    TOK_ERROR
};

स्थिर गणना नियमाः_token
lex(संरचना scanner *s, सङ्घ lvalue *val)
{
skip_अधिकम्_whitespace_and_comments:
    /* Skip spaces */
    यावत् (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    यदि (scanner_lit(s, "//")) {
        scanner_skip_गन्तव्यम्_eol(s);
    }

    /* New line */
    यदि (scanner_eol(s)) {
        यावत् (scanner_eol(s)) scanner_अग्र(s);
        प्रत्यागम TOK_अन्तः_सम्बन्धि_पङ्क्तिः;
    }

    /* Escaped line continuation */
    यदि (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        यदि (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            प्रत्यागम TOK_ERROR;
        }
        scanner_अग्र(s);
        गच्छ skip_अधिकम्_whitespace_and_comments;
    }

    /* See if we're done */
    यदि (scanner_eof(s)) प्रत्यागम TOK_अन्तः_सम्बन्धि_FILE;

    /* New token */
    s->token_pos = s->pos;
    गणना नियमाः_token token = TOK_IDENTIFIER;

    ध्रुव वर्ण अग्र = scanner_अग्र(s);
    विकल्प (अग्र) {
    पक्ष '!':
        प्रत्यागम TOK_BANG;
    पक्ष '$':
        /*गणः*/
        token = TOK_गणः_NAME;
        val->string.len = 0;
        भङ्ग;
    पक्ष '*':
        यदि (scanner_नियमाः_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            भङ्ग;
        } अन्यथा {
            /* Legacy wild card */
            प्रत्यागम TOK_WILD_CARD_तारकम्;
        }
    पक्ष '<':
        /* Extended wild cards */
        यदि (scanner_नियमाः_lit_token(s, "none>")) प्रत्यागम TOK_WILD_CARD_NONE;
        यदि (scanner_नियमाः_lit_token(s, "some>")) प्रत्यागम TOK_WILD_CARD_SOME;
        यदि (scanner_नियमाः_lit_token(s, "any>")) प्रत्यागम TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        प्रतिज्ञा(scanner_नियमाः_is_ident('<'));
        val->string.len = 1;
        भङ्ग;
    पक्ष '=':
        यदि (scanner_नियमाः_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            भङ्ग;
        } अन्यथा {
            प्रत्यागम TOK_EQUALS;
        }
    पक्ष 'i':
        यदि (scanner_नियमाः_lit_token(s, "nclude")) {
            /* Include statement */
            प्रत्यागम TOK_INCLUDE;
        }
        /* Identifier */
        प्रतिज्ञा(scanner_नियमाः_is_ident('i'));
        val->string.len = 1;
        भङ्ग;
    स्वभाव:
        यदि (likely(scanner_नियमाः_is_ident(अग्र))) {
            /* Identifier */
            val->string.len = 1;
        } अन्यथा {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.start = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    प्रतिज्ञा(scanner_नियमाः_is_ident(MERGE_OVERRIDE_उपसर्गः));
    प्रतिज्ञा(scanner_नियमाः_is_ident(MERGE_AUGMENT_उपसर्गः));
    प्रतिज्ञा(scanner_नियमाः_is_ident(MERGE_REPLACE_उपसर्गः));
    प्रतिज्ञा(scanner_नियमाः_is_ident('('));
    प्रतिज्ञा(scanner_नियमाः_is_ident(')'));
    val->string.len += scanner_नियमाः_ident_token(s);

    यदि (unlikely(val->string.len == 0)) {
        यदि (token == TOK_गणः_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } अन्यथा {
            scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                        "unrecognized token");
        }
        प्रत्यागम TOK_ERROR;
    }

    प्रत्यागम token;
}

/***====================================================================***/

गणना नियमाः_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_अङ्कः_ENTRIES
};

गणना {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

प्रकारनिर्देश uint8_t mlvo_सूचकांक_t;
प्रकारनिर्देश uint8_t mlvo_आवरणम्_t;


स्थिर ध्रुव संरचना sval नियमाः_mlvo_svals[MLVO_अङ्कः_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

गणना नियमाः_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_अङ्कः_ENTRIES
};

प्रकारनिर्देश uint8_t kccgst_सूचकांक_t;
प्रकारनिर्देश uint8_t kccgst_आवरणम्_t;

स्थिर ध्रुव संरचना sval नियमाः_kccgst_svals[KCCGST_अङ्कः_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

गणना {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_आवरणम्_t)XKB_OPTION_LAYOUT_आवरणम्_GLOBAL ==
              (xkb_layout_आवरणम्_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
संरचना matched_sval {
    संरचना sval sval;
    /** Used for layout-specific options */
    xkb_layout_आवरणम्_t layouts;
    xkb_layout_आवरणम्_t matched;
};
प्रकारनिर्देश darray(संरचना matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
संरचना नियमः_नामानि {
    संरचना matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

संरचना गणः {
    संरचना sval name;
    darray_sval elements;
};

संरचना mapping {
    गणना नियमाः_mlvo mlvo_at_pos[MLVO_अङ्कः_ENTRIES];
    mlvo_सूचकांक_t अङ्कः_mlvo;
    mlvo_आवरणम्_t defined_mlvo_आवरणम्;
    bool has_layout_idx_range;
    bool has_गुणजः_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    सङ्घ {
        संरचना { xkb_layout_सूचकांक_t layout_idx, variant_idx; };
        संरचना { xkb_layout_सूचकांक_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    सङ्घ {
        xkb_layout_आवरणम्_t सक्रिय;
        xkb_layout_आवरणम्_t layouts_candidates_आवरणम्;
    };
    गणना नियमाः_kccgst kccgst_at_pos[KCCGST_अङ्कः_ENTRIES];
    kccgst_सूचकांक_t अङ्कः_kccgst;
    kccgst_आवरणम्_t defined_kccgst_आवरणम्;
};

गणना mlvo_मेलनम्_type {
    /** Match the given plain value */
    MLVO_मेलनम्_प्रसामान्यम् = 0,
    /** Match depending on the value of `wildcard_match_type` */
    MLVO_मेलनम्_WILDCARD_LEGACY,
    /** Match empty value */
    MLVO_मेलनम्_WILDCARD_NONE,
    /** Match non-empty value */
    MLVO_मेलनम्_WILDCARD_SOME,
    /** Match any value, optionally empty */
    MLVO_मेलनम्_WILDCARD_ANY,
    /** Match any entry in a group */
    MLVO_मेलनम्_गणः,
};

गणना wildcard_मेलनम्_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_मेलनम्_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_मेलनम्_ALL,
};

संरचना नियमः_2 {
    संरचना sval mlvo_मानम्_at_pos[MLVO_अङ्कः_ENTRIES];
    गणना mlvo_मेलनम्_type मेलनम्_type_at_pos[MLVO_अङ्कः_ENTRIES];
    संरचना sval kccgst_मानम्_at_pos[KCCGST_अङ्कः_ENTRIES];
    mlvo_सूचकांक_t अङ्कः_mlvo_मानानि;
    kccgst_सूचकांक_t अङ्कः_kccgst_मानानि;
    bool skip;
};

#परिभाषय खण्डिका_KCCGST_BIT_क्षेत्रम्_आकार 4
static_assert(!((1u << (खण्डिका_KCCGST_BIT_क्षेत्रम्_आकार - 1)) &
                (KCCGST_अङ्कः_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
संरचना kccgst_बफर_खण्डिका {
    uint32_t दीर्घता:(32 - खण्डिका_KCCGST_BIT_क्षेत्रम्_आकार);
    गणना नियमाः_kccgst kccgst:खण्डिका_KCCGST_BIT_क्षेत्रम्_आकार;
    xkb_layout_सूचकांक_t layout;
};

/* Buffer for pending KcCGST values */
संरचना kccgst_बफर {
    darray_char बफर;
    /* Slice corresponding to each value in the buffer */
    darray(संरचना kccgst_बफर_खण्डिका) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
संरचना matcher {
    संरचना xkb_context *ctx;
    /* Input.*/
    संरचना नियमः_नामानि rmlvo;
    सङ्घ lvalue val;
    darray(संरचना गणः) groups;
    /* Current mapping. */
    संरचना mapping mapping;
    /* Current rule. */
    संरचना नियमः_2 नियमः_2;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    संरचना kccgst_बफर pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_अङ्कः_ENTRIES];
};

स्थिर संरचना sval
strip_spaces(संरचना sval v)
{
    यावत् (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    यावत् (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    प्रत्यागम v;
}

स्थिर darray_matched_sval
विभज_comma_separated_mlvo(संरचना xkb_context *ctx,
                           गणना नियमाः_mlvo mlvo, ध्रुव वर्ण *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    यदि (!s) {
        ensure_at_न्यूनतमम्_one_मानम्:
        /* Label followed by a declaration is a C23 extension */ ;

        संरचना matched_sval val = {
            .sval = SVAL(रिक्त, 0),
            .layouts = (xkb_layout_आवरणम्_t)XKB_OPTION_LAYOUT_आवरणम्_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        प्रत्यागम arr;
    }

    यावत् (true) {
        संरचना matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_आवरणम्_t)XKB_OPTION_LAYOUT_आवरणम्_GLOBAL,
            .matched = 0,
        };
        यावत् (*s != '\0' && *s != ',' && *s != OPTIONS_गणः_SPECIFIER_उपसर्गः) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        यदि (*s == OPTIONS_गणः_SPECIFIER_उपसर्गः) {
            /* Handle numeric layout index */
            s++;
            ध्रुव वर्ण* ध्रुव layout_start = s;
            xkb_layout_सूचकांक_t layout = XKB_LAYOUT_INVALID;

            पूर्णाङ्क गणनम् = parse_dec_गन्तव्यम्_uint32_t(s, SIZE_MAX, &layout);
            यदि (गणनम् > 0) {
                /* Note: 1-indexed layout */
                s += गणनम्;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                यदि (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (अचिह्नित पूर्णाङ्क) val.sval.len, val.sval.start);
                } अन्यथा यदि (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_सन्देशः_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (अचिह्नित पूर्णाङ्क) val.sval.len, val.sval.start);
                } अन्यथा {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ध्रुव वर्ण* ध्रुव layout_सूचकांक_अन्तः = s;
            यावत् (*s != '\0' && *s != ',') { s++; }
            यदि (गणनम् <= 0 || layout_सूचकांक_अन्तः != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (अचिह्नित पूर्णाङ्क) (s - layout_start), layout_start,
                        (अचिह्नित पूर्णाङ्क) val.sval.len, val.sval.start);
            }
        }

        यदि (val.sval.len || mlvo != MLVO_OPTION) {
            यदि (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                संरचना matched_sval *पूर्व;
                darray_foreach(पूर्व, arr) {
                    यदि (svaleq(पूर्व->sval, val.sval)) {
                        /* Merge options */
                        यदि (val.layouts ==
                            (xkb_layout_आवरणम्_t)XKB_OPTION_LAYOUT_आवरणम्_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            पूर्व->layouts = val.layouts;
                        } अन्यथा यदि (पूर्व->layouts != (xkb_layout_आवरणम्_t)
                                   XKB_OPTION_LAYOUT_आवरणम्_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            पूर्व->layouts |= val.layouts;
                        }
                        गच्छ अग्र_मानम्;
                    }
                }
            }
            darray_append(arr, val);
        }

अग्र_मानम्:
        यदि (*s == '\0') भङ्ग;
        यदि (*s == ',') s++;
    }

    यदि (darray_रिक्तानि(arr))
        गच्छ ensure_at_न्यूनतमम्_one_मानम्;

    प्रत्यागम arr;
}

स्थिर संरचना matcher *
matcher_new_from_rmlvo(ध्रुव संरचना xkb_rmlvo_builder *rmlvo, ध्रुव वर्ण **नियमाः)
{
    संरचना matcher *m = शून्यस्मृतिं_देहि(1, परिमाण(*m));
    यदि (!m)
        प्रत्यागम रिक्त;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    संरचना xkb_नियमः_नामानि नामानि = {
        .नियमाः = rmlvo->नियमाः,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_रिक्तानि(rmlvo->layouts)) ? रिक्त : "x",
        .variant = (darray_रिक्तानि(rmlvo->layouts)) ? रिक्त : "x",
        .options = (darray_रिक्तानि(rmlvo->options)) ? रिक्त : "x",
    };
    ध्रुव गणना RMLVO परिवर्तितम् = xkb_context_sanitize_नियमः_नामानि(rmlvo->ctx,
                                                               &नामानि);

    /* Initialize matcher with sanitized names, if relevant */
    यदि (परिवर्तितम् & RMLVO_नियमाः) {
        *नियमाः = नामानि.नियमाः;
    } अन्यथा {
        *नियमाः = rmlvo->नियमाः;
    }

    यदि (परिवर्तितम् & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = नामानि.model;
    } अन्यथा {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_आवरणम्_t)XKB_OPTION_LAYOUT_आवरणम्_GLOBAL;

    प्रतिज्ञा((परिवर्तितम् & RMLVO_LAYOUT) || !(परिवर्तितम् & RMLVO_VARIANT));
    यदि (परिवर्तितम् & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = विभज_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      नामानि.layout);
        m->rmlvo.variants = विभज_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       नामानि.variant);
        यदि (darray_आकार(m->rmlvo.layouts) > darray_आकार(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            यदि (!isempty(नामानि.variant))
                log_warn(m->ctx, XKB_LOG_सन्देशः_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         नामानि.layout ? नामानि.layout : "(none)",
                         नामानि.variant ? नामानि.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_आकार(m->rmlvo.layouts));
        } अन्यथा यदि (darray_आकार(m->rmlvo.layouts) < darray_आकार(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    नामानि.layout ? नामानि.layout : "(none)",
                    नामानि.variant ? नामानि.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_आकार(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } अन्यथा {
        संरचना xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            संरचना matched_sval val = {
                .sval = SVAL(layout->layout, strlen_safe(layout->layout)),
                .layouts = (xkb_layout_आवरणम्_t)XKB_OPTION_LAYOUT_आवरणम्_GLOBAL,
                .matched = 0
            };
            darray_append(m->rmlvo.layouts, val);
            val.sval.start = layout->variant;
            val.sval.len = strlen_safe(layout->variant);
            darray_append(m->rmlvo.variants, val);
        }
    }

    यदि (परिवर्तितम् & RMLVO_OPTIONS) {
        m->rmlvo.options = विभज_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      नामानि.options);
    } अन्यथा {
        संरचना xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            संरचना matched_sval val = {
                .sval = SVAL(option->option, strlen_safe(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    प्रत्यागम m;
}

स्थिर संरचना matcher *
matcher_new_from_नामानि(संरचना xkb_context *ctx,
            ध्रुव संरचना xkb_नियमः_नामानि *rmlvo)
{
    संरचना matcher *m = शून्यस्मृतिं_देहि(1, परिमाण(*m));
    यदि (!m)
        प्रत्यागम रिक्त;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_आवरणम्_t)XKB_OPTION_LAYOUT_आवरणम्_GLOBAL;
    m->rmlvo.layouts = विभज_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = विभज_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = विभज_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    यदि (darray_आकार(m->rmlvo.layouts) > darray_आकार(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        यदि (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_सन्देशः_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_आकार(m->rmlvo.layouts));
    } अन्यथा यदि (darray_आकार(m->rmlvo.layouts) < darray_आकार(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_आकार(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    प्रत्यागम m;
}

स्थिर शून्य
matcher_free(संरचना matcher *m)
{
    यदि (!m)
        प्रत्यागम;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    संरचना गणः *गणः;
    darray_foreach(गणः, m->groups)
        darray_free(गणः->elements);
    darray_free(m->pending_kccgst.बफर);
    darray_free(m->pending_kccgst.slices);
    प्रति (kccgst_सूचकांक_t i = 0; i < (kccgst_सूचकांक_t) KCCGST_अङ्कः_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    मुक्त(m);
}

स्थिर शून्य
matcher_गणः_start_new(संरचना matcher *m, संरचना sval name)
{
    संरचना गणः गणः = { .name = name, .elements = darray_new() };
    darray_append(m->groups, गणः);
}

स्थिर शून्य
matcher_गणः_सङ्कलय_element(संरचना matcher *m, संरचना scanner *s,
                          संरचना sval element)
{
    darray_append(darray_item(m->groups, darray_आकार(m->groups) - 1).elements,
                  element);
}

स्थिर bool
read_नियमाः_file(संरचना xkb_context *ctx,
                संरचना matcher *matcher,
                अचिह्नित पूर्णाङ्क include_गभीरता,
                FILE *file,
                ध्रुव वर्ण *मार्गः);

स्थिर शून्य
matcher_include(संरचना matcher *m, संरचना scanner *parent_scanner,
                अचिह्नित पूर्णाङ्क include_गभीरता,
                संरचना sval inc)
{
    यदि (include_गभीरता >= MAX_INCLUDE_गभीरता) {
        scanner_err(parent_scanner, XKB_LOG_सन्देशः_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_गभीरता);
        प्रत्यागम;
    }

    ध्रुव वर्ण *stmt_file = inc.start;
    आकार_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    वर्ण buf[PATH_MAX] = {0};
    ध्रुव ssize_t expanded = expand_मार्गः(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_नियमाः,
                                         buf, परिमाण(buf));
    यदि (expanded < 0) {
        /* Error */
        प्रत्यागम;
    } अन्यथा यदि (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (आकार_t) expanded;
        प्रतिज्ञा(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    अचिह्नित पूर्णाङ्क विस्थापनम् = 0;
    ध्रुव bool absolute_मार्गः = is_absolute_मार्गः(stmt_file);
    यदि (absolute_मार्गः) {
        /* Absolute path: no need for lookup in XKB paths */
        यदि (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            यदि (stmt_file_len < परिमाण(buf)) {
                स्मृतिं_प्रतिलिख(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } अन्यथा {
                log_err(m->ctx, XKB_ERROR_INVALID_मार्गः,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, परिमाण(buf),
                        (अचिह्नित पूर्णाङ्क) stmt_file_len, stmt_file);
                प्रत्यागम;
            }
        } अन्यथा {
            /* %-expansion is always NULL-terminated */
            प्रतिज्ञा(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } अन्यथा {
        /* Relative path: lookup the first XKB path */
        यदि (unlikely(expanded)) {
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
            file = रिक्त;
        } अन्यथा {
            file = Find_File_अन्तर्_Xkb_मार्गः(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_नियमाः,
                                     buf, परिमाण(buf), &विस्थापनम्, true);
        }
    }

    यावत् (file) {
        प्रतिज्ञा(strlen_safe(buf) < परिमाण(buf));
        bool ret = read_नियमाः_file(m->ctx, m, include_गभीरता + 1, file, buf);
        fclose(file);
        यदि (ret)
            प्रत्यागम;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_सन्देशः_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        यदि (absolute_मार्गः) {
            /* There is no point to search further if the path is absolute */
            भङ्ग;
        }

        /* Try next XKB path */
        विस्थापनम्++;
        file = Find_File_अन्तर्_Xkb_मार्गः(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_नियमाः,
                                 buf, परिमाण(buf), &विस्थापनम्, true);
    }

    log_err(m->ctx, XKB_LOG_सन्देशः_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (अचिह्नित पूर्णाङ्क) stmt_file_len, stmt_file);
}

स्थिर शून्य
matcher_mapping_start_new(संरचना matcher *m)
{
    प्रति (mlvo_सूचकांक_t i = 0; i < (mlvo_सूचकांक_t) MLVO_अङ्कः_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_अङ्कः_ENTRIES;
    प्रति (kccgst_सूचकांक_t i = 0; i < (kccgst_सूचकांक_t) KCCGST_अङ्कः_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_अङ्कः_ENTRIES;
    m->mapping.has_गुणजः_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.अङ्कः_mlvo = m->mapping.अङ्कः_kccgst = 0;
    m->mapping.defined_mlvo_आवरणम् = 0;
    m->mapping.defined_kccgst_आवरणम् = 0;
    m->mapping.सक्रिय = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
स्थिर पूर्णाङ्क
parse_layout_int_सूचकांक_पुच्छ(ध्रुव वर्ण *s, आकार_t max_len,
                            xkb_layout_सूचकांक_t *बहिर्गमनम्)
{
    यदि (max_len < 3)
        प्रत्यागम -1;
    uint32_t val = 0;
    ध्रुव पूर्णाङ्क गणनम् = parse_dec_गन्तव्यम्_uint32_t(&s[1], max_len - 2, &val);
    यदि (गणनम् <= 0 || s[1 + गणनम्] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        प्रत्यागम -1;
    /* To zero-based index. */
    *बहिर्गमनम् = val - 1;
    प्रत्यागम गणनम् + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
स्थिर पूर्णाङ्क
extract_layout_सूचकांक(ध्रुव वर्ण *s, आकार_t max_len, xkb_layout_सूचकांक_t *बहिर्गमनम्)
{
    /* This function is pretty stupid, but works for now. */
    *बहिर्गमनम् = XKB_LAYOUT_INVALID;
    यदि (max_len < 3 || s[0] != '[')
        प्रत्यागम -1;
    यदि (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *बहिर्गमनम् = (xkb_layout_सूचकांक_t)(s[1] - '1');
        प्रत्यागम 3;
    }
    यदि (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        प्रत्यागम 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    प्रत्यागम parse_layout_int_सूचकांक_पुच्छ(s, max_len, बहिर्गमनम्);
}

/* Special layout indices */
गणना layout_सूचकांक_ranges {
    LAYOUT_सूचकांक_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_सूचकांक_FIRST,
    LAYOUT_सूचकांक_LATER,
    LAYOUT_सूचकांक_गुणजः,
    LAYOUT_सूचकांक_ANY,
};

static_assert((xkb_layout_सूचकांक_t) XKB_MAX_GROUPS <
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_SINGLE,
              "Cannot define special indices");
static_assert((xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_SINGLE <
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_FIRST &&
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_FIRST <
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_LATER &&
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_LATER <
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_गुणजः &&
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_गुणजः <
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_ANY &&
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_ANY <
              (xkb_layout_सूचकांक_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
स्थिर bool
extract_mapping_layout_सूचकांक(ध्रुव वर्ण *s, आकार_t max_len,
                             xkb_layout_सूचकांक_t *बहिर्गमनम्)
{
    /* Check for minimal `[` + index + `]` */
    यदि (max_len < 3 || s[0] != '[') {
        *बहिर्गमनम् = XKB_LAYOUT_INVALID;
        प्रत्यागम false;
    }

    /* Small numeric index */
    यदि (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *बहिर्गमनम् = (xkb_layout_सूचकांक_t)(s[1] - '1');
        प्रत्यागम true;
    }

    /* Named indices ranges */
    स्थिर ध्रुव संरचना {
        ध्रुव वर्ण* name;
        आकार_t आकार;
        xkb_layout_सूचकांक_t मानम्;
    } नामानि[] = {
        #परिभाषय सूचकांक_RANGE(पुच्छ, मानम्) \
            { (पुच्छ), परिमाण(पुच्छ), (xkb_layout_सूचकांक_t)(मानम्) }
        सूचकांक_RANGE("single]",   LAYOUT_सूचकांक_SINGLE),
        सूचकांक_RANGE("first]",    LAYOUT_सूचकांक_FIRST),
        सूचकांक_RANGE("later]",    LAYOUT_सूचकांक_LATER),
        सूचकांक_RANGE("multiple]", LAYOUT_सूचकांक_गुणजः),
        सूचकांक_RANGE("any]",      LAYOUT_सूचकांक_ANY),
        #अपरिभाषय सूचकांक_RANGE
    };

    प्रति (आकार_t k = 0; k < ARRAY_आकार(नामानि); k++) {
        यदि (max_len == नामानि[k].आकार &&
            memcmp(&s[1], नामानि[k].name, नामानि[k].आकार - 1) == 0) {
            *बहिर्गमनम् = नामानि[k].मानम्;
            प्रत्यागम true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *बहिर्गमनम् = XKB_LAYOUT_INVALID;
    ध्रुव पूर्णाङ्क गणनम् = parse_layout_int_सूचकांक_पुच्छ(s, max_len, बहिर्गमनम्);
    प्रत्यागम गणनम् > 0 && (आकार_t)गणनम् == max_len;
}

स्थिर अन्तःस्थ bool
is_mlvo_आवरणम्_defined(संरचना matcher *m, गणना नियमाः_mlvo mlvo)
{
    प्रत्यागम m->mapping.defined_mlvo_आवरणम् & (1u << mlvo);
}

स्थिर शून्य
matcher_mapping_समुच्चयः_mlvo(संरचना matcher *m, संरचना scanner *s,
                         संरचना sval ident)
{
    गणना नियमाः_mlvo mlvo;
    संरचना sval mlvo_sval;

    प्रति (mlvo = 0; mlvo < MLVO_अङ्कः_ENTRIES; mlvo++) {
        mlvo_sval = नियमाः_mlvo_svals[mlvo];

        यदि (svaleq_उपसर्गः(mlvo_sval, ident))
            भङ्ग;
    }

    /* Not found. */
    यदि (mlvo >= MLVO_अङ्कः_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (अचिह्नित पूर्णाङ्क) ident.len, ident.start);
        m->mapping.सक्रिय = false;
        प्रत्यागम;
    }

    यदि (is_mlvo_आवरणम्_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (अचिह्नित पूर्णाङ्क) mlvo_sval.len, mlvo_sval.start);
        m->mapping.सक्रिय = false;
        प्रत्यागम;
    }

    /* If there are leftovers still, it must be an index. */
    यदि (mlvo_sval.len < ident.len) {
        xkb_layout_सूचकांक_t idx;
        यदि (!extract_mapping_layout_सूचकांक(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (अचिह्नित पूर्णाङ्क) mlvo_sval.len, mlvo_sval.start);
            m->mapping.सक्रिय = false;
            प्रत्यागम;
        }

        यदि (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        अन्यथा यदि (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        अन्यथा {
            scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (अचिह्नित पूर्णाङ्क) mlvo_sval.len, mlvo_sval.start);
            m->mapping.सक्रिय = false;
            प्रत्यागम;
        }
    } अन्यथा यदि (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_SINGLE;
    } अन्यथा यदि (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    यदि (((mlvo == MLVO_LAYOUT && is_mlvo_आवरणम्_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_आवरणम्_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.सक्रिय = false;
        प्रत्यागम;
    }

    m->mapping.mlvo_at_pos[m->mapping.अङ्कः_mlvo] = mlvo;
    m->mapping.defined_mlvo_आवरणम् |= (mlvo_आवरणम्_t) 1u << mlvo;
    m->mapping.अङ्कः_mlvo++;
}

स्थिर शून्य
matcher_mapping_समुच्चयः_layout_bounds(संरचना matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_सूचकांक_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    विकल्प (idx) {
        पक्ष XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            प्रतिज्ञा(!is_mlvo_आवरणम्_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_आवरणम्_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_आवरणम् = 0x1; /* active = true */
            भङ्ग;
        पक्ष LAYOUT_सूचकांक_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_आकार(m->rmlvo.layouts));
            m->mapping.layouts_candidates_आवरणम् = (xkb_layout_आवरणम्_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            भङ्ग;
        पक्ष LAYOUT_सूचकांक_गुणजः:
        पक्ष LAYOUT_सूचकांक_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_आकार(m->rmlvo.layouts));
            m->mapping.layouts_candidates_आवरणम् = (xkb_layout_आवरणम्_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            भङ्ग;
        पक्ष LAYOUT_सूचकांक_SINGLE:
        पक्ष LAYOUT_सूचकांक_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        स्वभाव:
            /* Mere layout index */
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = idx;
            m->mapping.layout_idx_max = idx + 1;
            m->mapping.layouts_candidates_आवरणम् = UINT32_C(1) << idx;
    }
    m->mapping.has_गुणजः_layouts = (
        m->mapping.layout_idx_max > m->mapping.layout_idx_min &&
        m->mapping.layout_idx_max - m->mapping.layout_idx_min > 1
    );
}

स्थिर शून्य
matcher_mapping_समुच्चयः_kccgst(संरचना matcher *m, संरचना scanner *s, संरचना sval ident)
{
    गणना नियमाः_kccgst kccgst;
    संरचना sval kccgst_sval;

    प्रति (kccgst = 0; kccgst < KCCGST_अङ्कः_ENTRIES; kccgst++) {
        kccgst_sval = नियमाः_kccgst_svals[kccgst];

        यदि (svaleq(नियमाः_kccgst_svals[kccgst], ident))
            भङ्ग;
    }

    /* Not found. */
    यदि (kccgst >= KCCGST_अङ्कः_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (अचिह्नित पूर्णाङ्क) ident.len, ident.start);
        m->mapping.सक्रिय = false;
        प्रत्यागम;
    }

    यदि (m->mapping.defined_kccgst_आवरणम् & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (अचिह्नित पूर्णाङ्क) kccgst_sval.len, kccgst_sval.start);
        m->mapping.सक्रिय = false;
        प्रत्यागम;
    }

    m->mapping.kccgst_at_pos[m->mapping.अङ्कः_kccgst] = kccgst;
    m->mapping.defined_kccgst_आवरणम् |= (kccgst_आवरणम्_t) 1u << kccgst;
    m->mapping.अङ्कः_kccgst++;
}

स्थिर bool
matcher_mapping_verify(संरचना matcher *m, संरचना scanner *s)
{
    यदि (m->mapping.अङ्कः_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        गच्छ skip;
    }

    यदि (m->mapping.अङ्कः_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        गच्छ skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    यदि (is_mlvo_आवरणम्_defined(m, MLVO_LAYOUT)) {
        प्रतिज्ञा(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        विकल्प (m->mapping.layout_idx) {
            पक्ष LAYOUT_सूचकांक_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                यदि (darray_आकार(m->rmlvo.layouts) > 1)
                    गच्छ skip;
                भङ्ग;
            पक्ष LAYOUT_सूचकांक_गुणजः:
            पक्ष LAYOUT_सूचकांक_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                यदि (darray_आकार(m->rmlvo.layouts) < 2)
                    गच्छ skip;
                भङ्ग;
            पक्ष LAYOUT_सूचकांक_ANY:
            पक्ष LAYOUT_सूचकांक_FIRST:
                /* No restrictions */
                भङ्ग;
            स्वभाव:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                यदि (darray_आकार(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_आकार(m->rmlvo.layouts))
                    गच्छ skip;
        }
    }

    यदि (is_mlvo_आवरणम्_defined(m, MLVO_VARIANT)) {
        प्रतिज्ञा(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        विकल्प (m->mapping.variant_idx) {
            पक्ष LAYOUT_सूचकांक_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                यदि (darray_आकार(m->rmlvo.variants) > 1)
                    गच्छ skip;
                भङ्ग;
            पक्ष LAYOUT_सूचकांक_ANY:
            पक्ष LAYOUT_सूचकांक_LATER:
            पक्ष LAYOUT_सूचकांक_FIRST:
                /* No restriction */
                भङ्ग;
            स्वभाव:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                यदि (darray_आकार(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_आकार(m->rmlvo.variants))
                    गच्छ skip;
        }
    }

    प्रत्यागम true;

skip:
    m->mapping.सक्रिय = false;
    प्रत्यागम false;
}

स्थिर शून्य
matcher_नियमः_start_new(संरचना matcher *m)
{
    स्मृतिं_पूरय(&m->नियमः_2, 0, परिमाण(m->नियमः_2));
    m->नियमः_2.skip = !m->mapping.सक्रिय;
}

स्थिर शून्य
matcher_नियमः_समुच्चयः_mlvo_common(संरचना matcher *m, संरचना scanner *s,
                             संरचना sval ident,
                             गणना mlvo_मेलनम्_type मेलनम्_type)
{
    यदि (m->नियमः_2.अङ्कः_mlvo_मानानि >= m->mapping.अङ्कः_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->नियमः_2.skip = true;
        प्रत्यागम;
    }
    m->नियमः_2.मेलनम्_type_at_pos[m->नियमः_2.अङ्कः_mlvo_मानानि] = मेलनम्_type;
    m->नियमः_2.mlvo_मानम्_at_pos[m->नियमः_2.अङ्कः_mlvo_मानानि] = ident;
    m->नियमः_2.अङ्कः_mlvo_मानानि++;
}

स्थिर शून्य
matcher_नियमः_समुच्चयः_mlvo_wildcard(संरचना matcher *m, संरचना scanner *s,
                               गणना mlvo_मेलनम्_type मेलनम्_type)
{
    संरचना sval dummy = SVAL(रिक्त, 0);
    matcher_नियमः_समुच्चयः_mlvo_common(m, s, dummy, मेलनम्_type);
}

स्थिर शून्य
matcher_नियमः_समुच्चयः_mlvo_गणः(संरचना matcher *m, संरचना scanner *s,
                            संरचना sval ident)
{
    matcher_नियमः_समुच्चयः_mlvo_common(m, s, ident, MLVO_मेलनम्_गणः);
}

स्थिर शून्य
matcher_नियमः_समुच्चयः_mlvo(संरचना matcher *m, संरचना scanner *s,
                      संरचना sval ident)
{
    matcher_नियमः_समुच्चयः_mlvo_common(m, s, ident, MLVO_मेलनम्_प्रसामान्यम्);
}

स्थिर शून्य
matcher_नियमः_समुच्चयः_kccgst(संरचना matcher *m, संरचना scanner *s,
                        संरचना sval ident)
{
    यदि (m->नियमः_2.अङ्कः_kccgst_मानानि >= m->mapping.अङ्कः_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->नियमः_2.skip = true;
        प्रत्यागम;
    }
    m->नियमः_2.kccgst_मानम्_at_pos[m->नियमः_2.अङ्कः_kccgst_मानानि] = ident;
    m->नियमः_2.अङ्कः_kccgst_मानानि++;
}

स्थिर bool
मेलनम्_गणः(संरचना matcher *m, संरचना sval गणः_name, संरचना sval गन्तव्यम्)
{
    संरचना गणः *गणः = रिक्त;
    संरचना sval *element;
    bool प्राप्तम् = false;

    darray_foreach(गणः, m->groups) {
        यदि (svaleq(गणः->name, गणः_name)) {
            प्राप्तम् = true;
            भङ्ग;
        }
    }

    यदि (!प्राप्तम्) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        प्रत्यागम false;
    }

    darray_foreach(element, गणः->elements)
        यदि (svaleq(गन्तव्यम्, *element))
            प्रत्यागम true;

    प्रत्यागम false;
}

स्थिर bool
मेलनम्_मानम्(संरचना matcher *m, ध्रुव संरचना sval val, ध्रुव संरचना sval गन्तव्यम्,
            गणना mlvo_मेलनम्_type मेलनम्_type,
            गणना wildcard_मेलनम्_type wildcard_type)
{
    विकल्प (मेलनम्_type) {
        पक्ष MLVO_मेलनम्_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            प्रत्यागम wildcard_type == WILDCARD_मेलनम्_ALL || !!गन्तव्यम्.len;
        पक्ष MLVO_मेलनम्_WILDCARD_NONE:
            प्रत्यागम !गन्तव्यम्.len;
        पक्ष MLVO_मेलनम्_WILDCARD_SOME:
            प्रत्यागम !!गन्तव्यम्.len;
        पक्ष MLVO_मेलनम्_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            प्रत्यागम true;
        पक्ष MLVO_मेलनम्_गणः:
            प्रत्यागम मेलनम्_गणः(m, val, गन्तव्यम्);
        स्वभाव:
            प्रतिज्ञा(मेलनम्_type == MLVO_मेलनम्_प्रसामान्यम्);
            प्रत्यागम svaleq(val, गन्तव्यम्);
    }
}

स्थिर bool
मेलनम्_मानम्_and_mark(संरचना matcher *m, ध्रुव संरचना sval val,
                     संरचना matched_sval *गन्तव्यम्, गणना mlvo_मेलनम्_type मेलनम्_type,
                     गणना wildcard_मेलनम्_type wildcard_type,
                     xkb_layout_आवरणम्_t layouts)
{
    bool matched = मेलनम्_मानम्(m, val, गन्तव्यम्->sval, मेलनम्_type, wildcard_type);
    यदि (matched)
        गन्तव्यम्->matched |= layouts;
    प्रत्यागम matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
स्थिर bool
expand_rmlvo_अन्तर्_kccgst_मानम्(संरचना matcher *m, संरचना scanner *s,
                             संरचना sval मानम्, xkb_layout_सूचकांक_t layout_idx,
                             darray_char *expanded, आकार_t *i)
{
    ध्रुव वर्ण *str = मानम्.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    गणना नियमाः_mlvo mlv;
    xkb_layout_सूचकांक_t idx;
    वर्ण pfx, sfx;
    संरचना matched_sval *expanded_मानम्;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    यदि (str[*i] == 'i' &&
        (*i + 1 == मानम्.len || is_merge_बहुलकम्_उपसर्गः(str[*i + 1])))
    {
        यदि (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_नियमाः_INVALID_LAYOUT_सूचकांक_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            गच्छ error;
        }
        (*i)++;
        वर्ण सूचकांक_str[MAX_LAYOUT_सूचकांक_STR_दीर्घता + 1];
        ध्रुव पूर्णाङ्क गणनम् = snprintf(सूचकांक_str, परिमाण(सूचकांक_str), "%"PRIu32,
                                   layout_idx + 1);
        प्रतिज्ञा(गणनम् > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, सूचकांक_str, (darray_आकार_t)गणनम्);
        प्रत्यागम true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    यदि (str[*i] == '(' ||
        is_merge_बहुलकम्_उपसर्गः(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        यदि (str[*i] == '(') sfx = ')';
        यदि (++(*i) >= मानम्.len) गच्छ error;
    }

    /* Mandatory model/layout/variant specifier. */
    विकल्प (str[(*i)++]) {
    पक्ष 'm': mlv = MLVO_MODEL; भङ्ग;
    पक्ष 'l': mlv = MLVO_LAYOUT; भङ्ग;
    पक्ष 'v': mlv = MLVO_VARIANT; भङ्ग;
    स्वभाव: गच्छ error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_सूचकांक = false;
    यदि (*i < मानम्.len && str[*i] == '[') {
        यदि (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            गच्छ error;
        }

        पूर्णाङ्क consumed = extract_layout_सूचकांक(str + (*i), मानम्.len - (*i), &idx);
        यदि (consumed <= 0) गच्छ error;
        यदि (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            प्रतिज्ञा(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_सूचकांक = true;
        }
        *i += (आकार_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    यदि (sfx != 0) {
        यदि (*i >= मानम्.len) गच्छ error;
        यदि (str[(*i)++] != sfx) गच्छ error;
    }

    /* Get the expanded value. */
    expanded_मानम् = रिक्त;

    यदि (mlv == MLVO_LAYOUT) {
        यदि (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            यदि (darray_आकार(m->rmlvo.layouts) == 1)
                expanded_मानम् = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } अन्यथा यदि (idx < darray_आकार(m->rmlvo.layouts) &&
                   (variable_सूचकांक || darray_आकार(m->rmlvo.layouts) > 1)) {
                expanded_मानम् = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    अन्यथा यदि (mlv == MLVO_VARIANT) {
        यदि (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            यदि (darray_आकार(m->rmlvo.variants) == 1)
                expanded_मानम् = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } अन्यथा यदि (idx < darray_आकार(m->rmlvo.variants) &&
                   (variable_सूचकांक || darray_आकार(m->rmlvo.variants) > 1)) {
                expanded_मानम् = &darray_item(m->rmlvo.variants, idx);
        }
    }
    अन्यथा यदि (mlv == MLVO_MODEL) {
        expanded_मानम् = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    यदि (!expanded_मानम् || expanded_मानम्->sval.len == 0) {
        प्रत्यागम true;
    }

    यदि (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_मानम्->sval.start,
                                 (darray_आकार_t) expanded_मानम्->sval.len);
    यदि (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    यदि (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ध्रुव xkb_layout_आवरणम्_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_मानम्->matched |= layouts;
    } अन्यथा {
        expanded_मानम्->matched |= (xkb_layout_आवरणम्_t)GLOBAL_MATCHED_LAYOUTS;
    }

    प्रत्यागम true;

error:
    scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                "invalid %%-expansion in value; not used");
    प्रत्यागम false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
स्थिर शून्य
expand_qualifier_अन्तर्_kccgst_मानम्(
    संरचना matcher *m, संरचना scanner *s,
    संरचना sval मानम्, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_आकार_t उपसर्गः_idx, आकार_t *i)
{
    ध्रुव वर्ण *str = मानम्.start;

    /* “all” followed by nothing or by a layout separator */
    यदि (*i + 2 < मानम्.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == मानम्.len || is_merge_बहुलकम्_उपसर्गः(str[*i + 3]))) {
        यदि (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_सन्देशः_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        यदि (darray_आकार(m->rmlvo.layouts) > 1) {
            वर्ण layout_सूचकांक[MAX_LAYOUT_सूचकांक_STR_दीर्घता + 1];
            ध्रुव darray_आकार_t उपसर्गः_दीर्घता =
                darray_आकार(*expanded) - उपसर्गः_idx - 1;
            प्रति (xkb_layout_सूचकांक_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_आकार(m->rmlvo.layouts));
                 l++)
            {
                यदि (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_उपसर्गः);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, उपसर्गः_idx),
                                             उपसर्गः_दीर्घता);
                /* Append index */
                ध्रुव पूर्णाङ्क गणनम् = snprintf(layout_सूचकांक, परिमाण(layout_सूचकांक),
                                           "%"PRIu32, l + 1);
                प्रतिज्ञा(गणनम् > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_सूचकांक,
                                             (darray_आकार_t)गणनम्);
            }
        }
        *i += 3;
    }
}

स्थिर अन्तःस्थ शून्य
#यदि_परिभाषितम् _MSC_VER
concat_kccgst(darray_char *into, darray_आकार_t आकार, अन्तर्_reads(आकार) ध्रुव वर्ण* from)
#अन्यथा
concat_kccgst(darray_char *into, darray_आकार_t आकार, ध्रुव वर्ण from[स्थिर आकार])
#यदि_अन्तः
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ध्रुव bool from_plus = is_merge_बहुलकम्_उपसर्गः(from[0]);
    यदि (from_plus || darray_रिक्तानि(*into)) {
        darray_appends_nullterminate(*into, from, आकार);
    } अन्यथा {
        ध्रुव वर्ण ch =
            (वर्ण) (darray_रिक्तानि(*into) ? '\0' : darray_item(*into, 0));
        ध्रुव bool into_plus = is_merge_बहुलकम्_उपसर्गः(ch);
        यदि (into_plus)
            darray_prepends_nullterminate(*into, from, आकार);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
स्थिर bool
append_expanded_kccgst_मानम्(संरचना matcher *m, संरचना scanner *s,
                             bool merge, darray_char *गन्तव्यम्, संरचना sval मानम्,
                             xkb_layout_सूचकांक_t layout_idx)
{
    ध्रुव वर्ण *str = मानम्.start;
    darray_char expanded = darray_new();
    darray_आकार_t चरमः_item_idx = 0;
    bool has_separator = false;

    प्रति (आकार_t i = 0; i < मानम्.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        विकल्प (str[i]) {
            /* Qualifier */
            पक्ष ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_अन्तर्_kccgst_मानम्(m, s, मानम्, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 चरमः_item_idx, &i);
                भङ्ग;
            /* Expansion */
            पक्ष '%':
                i++;
                यदि (i >= मानम्.len ||
                    !expand_rmlvo_अन्तर्_kccgst_मानम्(m, s, मानम्, layout_idx,
                                                  &expanded, &i))
                        गच्छ error;
                भङ्ग;
            /* New item */
            पक्ष MERGE_OVERRIDE_उपसर्गः:
            पक्ष MERGE_AUGMENT_उपसर्गः:
            पक्ष MERGE_REPLACE_उपसर्गः:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                चरमः_item_idx = darray_आकार(expanded) - 1;
                has_separator = true;
                भङ्ग;
            /* Just a normal character. */
            स्वभाव:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    यदि (merge) {
        यदि (!darray_रिक्तानि(expanded))
            concat_kccgst(गन्तव्यम्, darray_आकार(expanded), darray_वस्तूनि(expanded));
    } अन्यथा {
        darray_concat(*गन्तव्यम्, expanded);
    }
    darray_free(expanded);
    प्रत्यागम true;

error:
    darray_free(expanded);
    प्रत्यागम false;
}

स्थिर bool
matcher_append_pending_kccgst(संरचना matcher *m)
{
    यदि (!m->mapping.has_गुणजः_layouts)
        प्रत्यागम true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    प्रति (kccgst_सूचकांक_t i = 0; i < m->mapping.अङ्कः_kccgst; i++) {
        ध्रुव गणना नियमाः_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        प्रति (xkb_layout_सूचकांक_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            पञ्जिका ध्रुव संरचना kccgst_बफर* ध्रुव buf = &m->pending_kccgst;
            आकार_t विस्थापनम् = 0;
            प्रति (darray_आकार_t k = 0; k < darray_आकार(buf->slices); k++) {
                पञ्जिका ध्रुव संरचना kccgst_बफर_खण्डिका * ध्रुव खण्डिका =
                    &darray_item(buf->slices, k);
                यदि (खण्डिका->kccgst == kccgst && खण्डिका->layout == layout &&
                    खण्डिका->दीर्घता)
                    concat_kccgst(&m->kccgst[kccgst], खण्डिका->दीर्घता,
                                  darray_वस्तूनि(buf->बफर) + विस्थापनम्);
                विस्थापनम् += खण्डिका->दीर्घता;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_गुणजः_layouts = false;
    प्रत्यागम true;
}

स्थिर शून्य
matcher_नियमः_verify(संरचना matcher *m, संरचना scanner *s)
{
    यदि (m->नियमः_2.अङ्कः_mlvo_मानानि != m->mapping.अङ्कः_mlvo ||
        m->नियमः_2.अङ्कः_kccgst_मानानि != m->mapping.अङ्कः_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                    "invalid rule: must have same number of values "
                    "as mapping line; ignoring rule");
        m->नियमः_2.skip = true;
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
स्थिर bool
matcher_नियमः_मेलनम्_model(संरचना matcher *m, ध्रुव संरचना sval मानम्,
                         गणना mlvo_मेलनम्_type मेलनम्_type)
{
    प्रत्यागम मेलनम्_मानम्_and_mark(
        m, मानम्, &m->rmlvo.model, मेलनम्_type, WILDCARD_मेलनम्_ALL,
        /* layout-independent */
        (xkb_layout_आवरणम्_t)GLOBAL_MATCHED_LAYOUTS
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
स्थिर bool
matcher_नियमः_मेलनम्_option(संरचना matcher *m, ध्रुव संरचना sval मानम्,
                          गणना mlvo_मेलनम्_type मेलनम्_type,
                          xkb_layout_आवरणम्_t *candidates)
{
    /* There is always at least one value */
    प्रतिज्ञा(!darray_रिक्तानि(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_आवरणम्_t unmatched =
        (m->mapping.defined_mlvo_आवरणम् & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    संरचना matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        ध्रुव xkb_layout_आवरणम्_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_आवरणम्_t)GLOBAL_MATCHED_LAYOUTS;
        यदि (matchable && मेलनम्_मानम्_and_mark(m, मानम्, option, मेलनम्_type,
                                              WILDCARD_मेलनम्_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            यदि (!unmatched)
                भङ्ग;
        }
    }
    यदि (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    प्रत्यागम matched;
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
स्थिर bool
matcher_नियमः_मेलनम्_layout_or_variant(संरचना matcher *m,
                                     darray_matched_sval *निवेशः,
                                     ध्रुव संरचना sval मानम्,
                                     गणना mlvo_मेलनम्_type मेलनम्_type,
                                     xkb_layout_आवरणम्_t *candidates)
{
    /* There is always at least one value */
    प्रतिज्ञा(!darray_रिक्तानि(*निवेशः));

    bool matched = false;
    /* Loop over the layout index range */
    प्रति (xkb_layout_सूचकांक_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ध्रुव xkb_layout_आवरणम्_t layout = (UINT32_C(1) << idx);
        यदि (layout & *candidates) {
            संरचना matched_sval *गन्तव्यम् = &darray_item(*निवेशः, idx);
            यदि (मेलनम्_मानम्_and_mark(m, मानम्, गन्तव्यम्, मेलनम्_type,
                                     WILDCARD_मेलनम्_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } अन्यथा {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    प्रत्यागम matched;
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
स्थिर शून्य
matcher_नियमः_apply_if_matches(संरचना matcher *m, संरचना scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_आवरणम्_t candidate_layouts = m->mapping.layouts_candidates_आवरणम्;

    /* Loop over MLVO pattern components */
    प्रति (mlvo_सूचकांक_t i = 0; i < m->mapping.अङ्कः_mlvo; i++) {
        ध्रुव गणना नियमाः_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ध्रुव संरचना sval मानम् = m->नियमः_2.mlvo_मानम्_at_pos[i];
        ध्रुव गणना mlvo_मेलनम्_type मेलनम्_type = m->नियमः_2.मेलनम्_type_at_pos[i];
        bool matched = false;

        विकल्प (mlvo) {
        पक्ष MLVO_MODEL:
            matched = matcher_नियमः_मेलनम्_model(m, मानम्, मेलनम्_type);
            भङ्ग;
        पक्ष MLVO_LAYOUT:
            matched = matcher_नियमः_मेलनम्_layout_or_variant(
                m, &m->rmlvo.layouts, मानम्, मेलनम्_type, &candidate_layouts
            );
            भङ्ग;
        पक्ष MLVO_VARIANT:
            matched = matcher_नियमः_मेलनम्_layout_or_variant(
                m, &m->rmlvo.variants, मानम्, मेलनम्_type, &candidate_layouts
            );
            भङ्ग;
        पक्ष MLVO_OPTION:
            matched = matcher_नियमः_मेलनम्_option(
                m, मानम्, मेलनम्_type, &candidate_layouts
            );
            भङ्ग;
        स्वभाव: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_अङ्कः_ENTRIES - 1,
                          "Unexpected MLVO field");
            प्रतिज्ञा(!"Unreachable");
        }
        }

        यदि (!matched)
            प्रत्यागम;
    }

    यदि (m->mapping.has_गुणजः_layouts) {
        /* Special index: loop over the index range */
        प्रति (xkb_layout_सूचकांक_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            यदि (candidate_layouts & (UINT32_C(1) << idx)) {
                प्रति (kccgst_सूचकांक_t i = 0; i < m->mapping.अङ्कः_kccgst; i++) {
                    ध्रुव गणना नियमाः_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ध्रुव संरचना sval मानम् = m->नियमः_2.kccgst_मानम्_at_pos[i];
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
                    संरचना kccgst_बफर * ध्रुव buf = &m->pending_kccgst;
                    ध्रुव darray_आकार_t पूर्व_बफर_दीर्घता =
                        darray_आकार(buf->बफर);
                    append_expanded_kccgst_मानम्(m, s, false, &buf->बफर,
                                                 मानम्, idx);
                    ध्रुव uint32_t दीर्घता = (uint32_t) (darray_आकार(buf->बफर)
                                          - पूर्व_बफर_दीर्घता);
                    ध्रुव संरचना kccgst_बफर_खण्डिका खण्डिका = {
                        .दीर्घता = दीर्घता,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, खण्डिका);
                }
            }
        }
    } अन्यथा {
        /* Numeric index or no index */
        प्रति (kccgst_सूचकांक_t i = 0; i < m->mapping.अङ्कः_kccgst; i++) {
            गणना नियमाः_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            संरचना sval मानम् = m->नियमः_2.kccgst_मानम्_at_pos[i];
            append_expanded_kccgst_मानम्(m, s, true, &m->kccgst[kccgst], मानम्,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    यदि (!(is_mlvo_आवरणम्_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_आवरणम् &= ~candidate_layouts;
    }
}

स्थिर गणना नियमाः_token
gettok(संरचना matcher *m, संरचना scanner *s)
{
    प्रत्यागम lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
स्थिर bool
matcher_मेलनम्(संरचना matcher *m, संरचना scanner *s,
              अचिह्नित पूर्णाङ्क include_गभीरता,
              ध्रुव वर्ण *string, आकार_t len,
              ध्रुव वर्ण *file_name)
{
    गणना नियमाः_token tok;

    यदि (!m)
        प्रत्यागम false;

initial:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_BANG:
        गच्छ bang;
    पक्ष TOK_अन्तः_सम्बन्धि_पङ्क्तिः:
        गच्छ initial;
    पक्ष TOK_अन्तः_सम्बन्धि_FILE:
        गच्छ finish;
    स्वभाव:
        गच्छ unexpected;
    }

bang:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_गणः_NAME:
        matcher_गणः_start_new(m, m->val.string);
        गच्छ गणः_name;
    पक्ष TOK_INCLUDE:
        गच्छ include_statement;
    पक्ष TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_समुच्चयः_mlvo(m, s, m->val.string);
        गच्छ mapping_mlvo;
    स्वभाव:
        गच्छ unexpected;
    }

गणः_name:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_EQUALS:
        गच्छ गणः_element;
    स्वभाव:
        गच्छ unexpected;
    }

गणः_element:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_IDENTIFIER:
        matcher_गणः_सङ्कलय_element(m, s, m->val.string);
        गच्छ गणः_element;
    पक्ष TOK_अन्तः_सम्बन्धि_पङ्क्तिः:
        गच्छ initial;
    स्वभाव:
        गच्छ unexpected;
    }

include_statement:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_IDENTIFIER:
        matcher_include(m, s, include_गभीरता, m->val.string);
        गच्छ include_statement_अन्तः;
    स्वभाव:
        गच्छ unexpected;
    }

include_statement_अन्तः:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_अन्तः_सम्बन्धि_पङ्क्तिः:
        गच्छ initial;
    स्वभाव:
        गच्छ unexpected;
    }

mapping_mlvo:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_IDENTIFIER:
        यदि (m->mapping.सक्रिय)
            matcher_mapping_समुच्चयः_mlvo(m, s, m->val.string);
        गच्छ mapping_mlvo;
    पक्ष TOK_EQUALS:
        गच्छ mapping_kccgst;
    स्वभाव:
        गच्छ unexpected;
    }

mapping_kccgst:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_IDENTIFIER:
        यदि (m->mapping.सक्रिय)
            matcher_mapping_समुच्चयः_kccgst(m, s, m->val.string);
        गच्छ mapping_kccgst;
    पक्ष TOK_अन्तः_सम्बन्धि_पङ्क्तिः:
        यदि (m->mapping.सक्रिय && matcher_mapping_verify(m, s)) {
            matcher_mapping_समुच्चयः_layout_bounds(m);
            यदि (m->mapping.has_गुणजः_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_आकार(m->pending_kccgst.बफर) = 0;
                darray_आकार(m->pending_kccgst.slices) = 0;
            }
        }
        गच्छ नियमः_mlvo_first;
    स्वभाव:
        गच्छ unexpected;
    }

नियमः_mlvo_first:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_BANG:
        matcher_append_pending_kccgst(m);
        गच्छ bang;
    पक्ष TOK_अन्तः_सम्बन्धि_पङ्क्तिः:
        गच्छ नियमः_mlvo_first;
    पक्ष TOK_अन्तः_सम्बन्धि_FILE:
        matcher_append_pending_kccgst(m);
        गच्छ finish;
    स्वभाव:
        matcher_नियमः_start_new(m);
        गच्छ नियमः_mlvo_no_tok;
    }

नियमः_mlvo:
    tok = gettok(m, s);
नियमः_mlvo_no_tok:
    विकल्प (tok) {
    पक्ष TOK_IDENTIFIER:
        यदि (!m->नियमः_2.skip)
            matcher_नियमः_समुच्चयः_mlvo(m, s, m->val.string);
        गच्छ नियमः_mlvo;
    पक्ष TOK_WILD_CARD_तारकम्:
        यदि (!m->नियमः_2.skip)
            matcher_नियमः_समुच्चयः_mlvo_wildcard(m, s, MLVO_मेलनम्_WILDCARD_LEGACY);
        गच्छ नियमः_mlvo;
    पक्ष TOK_WILD_CARD_NONE:
        यदि (!m->नियमः_2.skip)
            matcher_नियमः_समुच्चयः_mlvo_wildcard(m, s, MLVO_मेलनम्_WILDCARD_NONE);
        गच्छ नियमः_mlvo;
    पक्ष TOK_WILD_CARD_SOME:
        यदि (!m->नियमः_2.skip)
            matcher_नियमः_समुच्चयः_mlvo_wildcard(m, s, MLVO_मेलनम्_WILDCARD_SOME);
        गच्छ नियमः_mlvo;
    पक्ष TOK_WILD_CARD_ANY:
        यदि (!m->नियमः_2.skip)
            matcher_नियमः_समुच्चयः_mlvo_wildcard(m, s, MLVO_मेलनम्_WILDCARD_ANY);
        गच्छ नियमः_mlvo;
    पक्ष TOK_गणः_NAME:
        यदि (!m->नियमः_2.skip)
            matcher_नियमः_समुच्चयः_mlvo_गणः(m, s, m->val.string);
        गच्छ नियमः_mlvo;
    पक्ष TOK_EQUALS:
        गच्छ नियमः_kccgst;
    स्वभाव:
        गच्छ unexpected;
    }

नियमः_kccgst:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_IDENTIFIER:
        यदि (!m->नियमः_2.skip)
            matcher_नियमः_समुच्चयः_kccgst(m, s, m->val.string);
        गच्छ नियमः_kccgst;
    पक्ष TOK_अन्तः_सम्बन्धि_पङ्क्तिः:
        यदि (!m->नियमः_2.skip)
            matcher_नियमः_verify(m, s);
        यदि (!m->नियमः_2.skip)
            matcher_नियमः_apply_if_matches(m, s);
        गच्छ नियमः_mlvo_first;
    स्वभाव:
        गच्छ unexpected;
    }

unexpected:
    विकल्प (tok) {
    पक्ष TOK_ERROR:
        गच्छ error;
    स्वभाव:
        गच्छ state_error;
    }

finish:
    प्रत्यागम true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_नियमाः_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    प्रत्यागम false;
}
// NOLINTEND(readability-trivial-switch)

स्थिर bool
read_नियमाः_file(संरचना xkb_context *ctx,
                संरचना matcher *matcher,
                अचिह्नित पूर्णाङ्क include_गभीरता,
                FILE *file,
                ध्रुव वर्ण *मार्गः)
{
    bool ret;
    वर्ण *string;
    आकार_t आकार;
    संरचना scanner scanner;

    यदि (!map_file(file, &string, &आकार)) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                मार्गः, strerror(errno));
        प्रत्यागम false;
    }

    scanner_init(&scanner, matcher->ctx, string, आकार, मार्गः, रिक्त);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    यदि (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, आकार);
        प्रत्यागम false;
    }

    ret = matcher_मेलनम्(matcher, &scanner, include_गभीरता, string, आकार, मार्गः);
    unmap_file(string, आकार);
    प्रत्यागम ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
स्थिर bool
xkb_resolve_partial_नियमाः(संरचना xkb_context *ctx, वर्ण *मार्गः, आकार_t मार्गः_आकार,
                          ध्रुव वर्ण* नियमाः, ध्रुव वर्ण* suffix,
                          संरचना matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    वर्ण partial_नियमाः[60]; /* Arbitrary, but we do not expect long names */
    यदि (unlikely(!snprintf_safe(partial_नियमाः, परिमाण(partial_नियमाः),
                                "%s%s", नियमाः, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", नियमाः, suffix);
        प्रत्यागम false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    अचिह्नित पूर्णाङ्क विस्थापनम् = 0;
    FILE *file = रिक्त;
    ध्रुव आकार_t len = वाक्यदैर्घ्यम्(partial_नियमाः);
    यावत् ((file = Find_File_अन्तर्_Xkb_मार्गः(ctx, "(unknown)",
                                     partial_नियमाः, len, FILE_TYPE_नियमाः,
                                     मार्गः, मार्गः_आकार, &विस्थापनम्, false)) != रिक्त) {
        ध्रुव bool ok = read_नियमाः_file(ctx, matcher, 0, file, मार्गः);
        fclose(file);
        यदि (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", मार्गः);
            प्रत्यागम false;
        }
        विस्थापनम्++;
    }

    प्रत्यागम true;
}

स्थिर bool
xkb_resolve_नियमाः(संरचना xkb_context *ctx,
                  ध्रुव वर्ण* नियमाः, संरचना matcher *matcher,
                  संरचना xkb_component_नामानि *बहिर्गमनम्,
                  xkb_layout_सूचकांक_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    अचिह्नित पूर्णाङ्क विस्थापनम् = 0;
    वर्ण मार्गः[PATH_MAX];
    FILE * ध्रुव file = Find_File_अन्तर्_Xkb_मार्गः(ctx, "(unknown)",
                                          नियमाः, वाक्यदैर्घ्यम्(नियमाः), FILE_TYPE_नियमाः,
                                          मार्गः, परिमाण(मार्गः), &विस्थापनम्, true);
    यदि (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", नियमाः);
        गच्छ err_बहिर्गमनम्;
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
    ret = xkb_resolve_partial_नियमाः(ctx, मार्गः, परिमाण(मार्गः),
                                    नियमाः, ".pre", matcher);
    यदि (!ret)
        गच्छ err_बहिर्गमनम्;

    /* Resolve main <rules> file */
    ret = read_नियमाः_file(ctx, matcher, 0, file, मार्गः);
    यदि (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", मार्गः);
        गच्छ err_बहिर्गमनम्;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_नियमाः(ctx, मार्गः, परिमाण(मार्गः),
                                    नियमाः, ".post", matcher);
    यदि (!ret)
        गच्छ err_बहिर्गमनम्;

    /* Check we got required components */
    यदि (darray_रिक्तानि(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_रिक्तानि(matcher->kccgst[KCCGST_TYPES]) ||
        darray_रिक्तानि(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_रिक्तानि(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", नियमाः);
        ret = false;
        गच्छ err_बहिर्गमनम्;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &बहिर्गमनम्->keycodes, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &बहिर्गमनम्->types, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &बहिर्गमनम्->compatibility, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &बहिर्गमनम्->symbols, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &बहिर्गमनम्->geometry, रिक्त);

    संरचना matched_sval *mval = &matcher->rmlvo.model;
    यदि (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मानम्,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (अचिह्नित पूर्णाङ्क) mval->sval.len, mval->sval.start, नियमाः);
    darray_foreach(mval, matcher->rmlvo.layouts)
        यदि (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मानम्,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (अचिह्नित पूर्णाङ्क) mval->sval.len, mval->sval.start, नियमाः);
    darray_foreach(mval, matcher->rmlvo.variants)
        यदि (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मानम्,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (अचिह्नित पूर्णाङ्क) mval->sval.len, mval->sval.start, नियमाः);
    darray_foreach(mval, matcher->rmlvo.options) {
        यदि (!mval->sval.len)
            अनुवर्त;

        यदि (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मानम्,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (अचिह्नित पूर्णाङ्क) mval->sval.len, mval->sval.start, नियमाः);
        } अन्यथा यदि (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ध्रुव xkb_layout_आवरणम्_t unmatched = (mval->layouts & ~mval->matched);
            प्रतिज्ञा(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मानम्,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (अचिह्नित पूर्णाङ्क)mval->sval.len, mval->sval.start,
                    unmatched, नियमाः);
        }
    }

    /* Set the number of explicit layouts */
    यदि (बहिर्गमनम्->symbols != रिक्त && explicit_layouts != रिक्त) {
        *explicit_layouts = 1; /* at least one group */
        ध्रुव वर्ण *symbols = बहिर्गमनम्->symbols;
        /* Take the highest modifier */
        यावत् ((symbols = strchr(symbols, ':')) != रिक्त && symbols[1] != '\0') {
            xkb_layout_सूचकांक_t गणः = 0;
            ध्रुव पूर्णाङ्क गणनम् = parse_dec_गन्तव्यम्_uint32_t(++symbols, SIZE_MAX, &गणः);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            यदि (गणनम् > 0 && (symbols[गणनम्] == '\0' ||
                is_merge_बहुलकम्_उपसर्गः(symbols[गणनम्])) &&
                गणः > 0 && गणः <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, गणः);
                symbols += गणनम्;
            }
        }
    }

err_बहिर्गमनम्:
    यदि (file)
        fclose(file);

    प्रत्यागम ret;
}

bool
xkb_components_from_rmlvo_builder(ध्रुव संरचना xkb_rmlvo_builder *rmlvo,
                                  संरचना xkb_component_नामानि *बहिर्गमनम्,
                                  xkb_layout_सूचकांक_t *explicit_layouts)
{
    ध्रुव वर्ण *नियमाः = rmlvo->नियमाः;
    संरचना matcher *matcher = matcher_new_from_rmlvo(rmlvo, &नियमाः);
    यदि (!matcher)
        प्रत्यागम false;

    ध्रुव bool ret = xkb_resolve_नियमाः(rmlvo->ctx, नियमाः, matcher,
                                       बहिर्गमनम्, explicit_layouts);

    matcher_free(matcher);
    प्रत्यागम ret;
}


bool
xkb_components_from_नियमाः_नामानि(संरचना xkb_context *ctx,
                                ध्रुव संरचना xkb_नियमः_नामानि *rmlvo,
                                संरचना xkb_component_नामानि *बहिर्गमनम्,
                                xkb_layout_सूचकांक_t *explicit_layouts)
{
    संरचना matcher *matcher = matcher_new_from_नामानि(ctx, rmlvo);
    यदि (!matcher)
        प्रत्यागम false;

    ध्रुव bool ret = xkb_resolve_नियमाः(ctx, rmlvo->नियमाः, matcher,
                                       बहिर्गमनम्, explicit_layouts);

    matcher_free(matcher);
    प्रत्यागम ret;
}
