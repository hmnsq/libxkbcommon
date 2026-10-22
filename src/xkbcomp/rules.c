/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <limits.h>
#समावेश <stdint.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcomp-priv.h"
#समावेश "context.h"
#समावेश "keymap.h"
#समावेश "messages-codes.h"
#समावेश "rules.h"
#समावेश "rmlvo.h"
#समावेश "include.h"
#समावेश "scanner-utils.h"
#समावेश "darray.h"
#समावेश "utils.h"
#समावेश "util-numbers.h"
#समावेश "util-paths.h"

#घोषणा MAX_INCLUDE_DEPTH 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
जोड़ lvalue {
    काष्ठा sval string;
};

क्रमागत rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_पंक्ति,
    TOK_IDENTIFIER,
    TOK_GROUP_NAME,
    TOK_BANG,
    TOK_EQUALS,
    TOK_WILD_CARD_STAR,
    TOK_WILD_CARD_NONE,
    TOK_WILD_CARD_SOME,
    TOK_WILD_CARD_ANY,
    TOK_INCLUDE,
    TOK_ERROR
};

अटल क्रमागत rules_token
lex(काष्ठा scanner *s, जोड़ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    जबतक (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    अगर (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    अगर (scanner_eol(s)) {
        जबतक (scanner_eol(s)) scanner_अग्र(s);
        वापस TOK_END_OF_पंक्ति;
    }

    /* Escaped line continuation */
    अगर (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        अगर (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            वापस TOK_ERROR;
        }
        scanner_अग्र(s);
        जाओ skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    अगर (scanner_eof(s)) वापस TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    क्रमागत rules_token token = TOK_IDENTIFIER;

    स्थिर अक्षर अग्र = scanner_अग्र(s);
    चयन (अग्र) {
    हाल '!':
        वापस TOK_BANG;
    हाल '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        अवरोध;
    हाल '*':
        अगर (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            अवरोध;
        } अन्यथा {
            /* Legacy wild card */
            वापस TOK_WILD_CARD_STAR;
        }
    हाल '<':
        /* Extended wild cards */
        अगर (scanner_rules_lit_token(s, "none>")) वापस TOK_WILD_CARD_NONE;
        अगर (scanner_rules_lit_token(s, "some>")) वापस TOK_WILD_CARD_SOME;
        अगर (scanner_rules_lit_token(s, "any>")) वापस TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        निश्चित(scanner_rules_is_ident('<'));
        val->string.len = 1;
        अवरोध;
    हाल '=':
        अगर (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            अवरोध;
        } अन्यथा {
            वापस TOK_EQUALS;
        }
    हाल 'i':
        अगर (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            वापस TOK_INCLUDE;
        }
        /* Identifier */
        निश्चित(scanner_rules_is_ident('i'));
        val->string.len = 1;
        अवरोध;
    शेष:
        अगर (likely(scanner_rules_is_ident(अग्र))) {
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
    निश्चित(scanner_rules_is_ident(MERGE_OVERRIDE_PREFIX));
    निश्चित(scanner_rules_is_ident(MERGE_AUGMENT_PREFIX));
    निश्चित(scanner_rules_is_ident(MERGE_REPLACE_PREFIX));
    निश्चित(scanner_rules_is_ident('('));
    निश्चित(scanner_rules_is_ident(')'));
    val->string.len += scanner_rules_ident_token(s);

    अगर (unlikely(val->string.len == 0)) {
        अगर (token == TOK_GROUP_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } अन्यथा {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unrecognized token");
        }
        वापस TOK_ERROR;
    }

    वापस token;
}

/***====================================================================***/

क्रमागत rules_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_अंक_ENTRIES
};

क्रमागत {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

प्रकार uint8_t mlvo_सूचकांक_t;
प्रकार uint8_t mlvo_mask_t;


अटल स्थिर काष्ठा sval rules_mlvo_svals[MLVO_अंक_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

क्रमागत rules_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_अंक_ENTRIES
};

प्रकार uint8_t kccgst_सूचकांक_t;
प्रकार uint8_t kccgst_mask_t;

अटल स्थिर काष्ठा sval rules_kccgst_svals[KCCGST_अंक_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

क्रमागत {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL ==
              (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
काष्ठा matched_sval {
    काष्ठा sval sval;
    /** Used for layout-specific options */
    xkb_layout_mask_t layouts;
    xkb_layout_mask_t matched;
};
प्रकार darray(काष्ठा matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
काष्ठा rule_names {
    काष्ठा matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

काष्ठा group {
    काष्ठा sval name;
    darray_sval elements;
};

काष्ठा mapping {
    क्रमागत rules_mlvo mlvo_at_pos[MLVO_अंक_ENTRIES];
    mlvo_सूचकांक_t अंक_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    जोड़ {
        काष्ठा { xkb_layout_सूचकांक_t layout_idx, variant_idx; };
        काष्ठा { xkb_layout_सूचकांक_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    जोड़ {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    क्रमागत rules_kccgst kccgst_at_pos[KCCGST_अंक_ENTRIES];
    kccgst_सूचकांक_t अंक_kccgst;
    kccgst_mask_t defined_kccgst_mask;
};

क्रमागत mlvo_match_type {
    /** Match the given plain value */
    MLVO_MATCH_NORMAL = 0,
    /** Match depending on the value of `wildcard_match_type` */
    MLVO_MATCH_WILDCARD_LEGACY,
    /** Match empty value */
    MLVO_MATCH_WILDCARD_NONE,
    /** Match non-empty value */
    MLVO_MATCH_WILDCARD_SOME,
    /** Match any value, optionally empty */
    MLVO_MATCH_WILDCARD_ANY,
    /** Match any entry in a group */
    MLVO_MATCH_GROUP,
};

क्रमागत wildcard_match_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_MATCH_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_MATCH_ALL,
};

काष्ठा rule {
    काष्ठा sval mlvo_मूल्य_at_pos[MLVO_अंक_ENTRIES];
    क्रमागत mlvo_match_type match_type_at_pos[MLVO_अंक_ENTRIES];
    काष्ठा sval kccgst_मूल्य_at_pos[KCCGST_अंक_ENTRIES];
    mlvo_सूचकांक_t अंक_mlvo_मूल्यावली;
    kccgst_सूचकांक_t अंक_kccgst_मूल्यावली;
    bool skip;
};

#घोषणा SLICE_KCCGST_BIT_FIELD_आकार 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_आकार - 1)) &
                (KCCGST_अंक_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
काष्ठा kccgst_बफर_slice {
    uint32_t लंबाई:(32 - SLICE_KCCGST_BIT_FIELD_आकार);
    क्रमागत rules_kccgst kccgst:SLICE_KCCGST_BIT_FIELD_आकार;
    xkb_layout_सूचकांक_t layout;
};

/* Buffer for pending KcCGST values */
काष्ठा kccgst_बफर {
    darray_char बफर;
    /* Slice corresponding to each value in the buffer */
    darray(काष्ठा kccgst_बफर_slice) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
काष्ठा matcher {
    काष्ठा xkb_context *ctx;
    /* Input.*/
    काष्ठा rule_names rmlvo;
    जोड़ lvalue val;
    darray(काष्ठा group) groups;
    /* Current mapping. */
    काष्ठा mapping mapping;
    /* Current rule. */
    काष्ठा rule rule;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    काष्ठा kccgst_बफर pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_अंक_ENTRIES];
};

अटल काष्ठा sval
strip_spaces(काष्ठा sval v)
{
    जबतक (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    जबतक (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    वापस v;
}

अटल darray_matched_sval
split_comma_separated_mlvo(काष्ठा xkb_context *ctx,
                           क्रमागत rules_mlvo mlvo, स्थिर अक्षर *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    अगर (!s) {
        ensure_at_least_one_मूल्य:
        /* Label followed by a declaration is a C23 extension */ ;

        काष्ठा matched_sval val = {
            .sval = SVAL(शून्य, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        वापस arr;
    }

    जबतक (true) {
        काष्ठा matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        जबतक (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        अगर (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            स्थिर अक्षर* स्थिर layout_start = s;
            xkb_layout_सूचकांक_t layout = XKB_LAYOUT_INVALID;

            पूर्णांक गिनती = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            अगर (गिनती > 0) {
                /* Note: 1-indexed layout */
                s += गिनती;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                अगर (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (अचिन्हित पूर्णांक) val.sval.len, val.sval.start);
                } अन्यथा अगर (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (अचिन्हित पूर्णांक) val.sval.len, val.sval.start);
                } अन्यथा {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            स्थिर अक्षर* स्थिर layout_सूचकांक_end = s;
            जबतक (*s != '\0' && *s != ',') { s++; }
            अगर (गिनती <= 0 || layout_सूचकांक_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (अचिन्हित पूर्णांक) (s - layout_start), layout_start,
                        (अचिन्हित पूर्णांक) val.sval.len, val.sval.start);
            }
        }

        अगर (val.sval.len || mlvo != MLVO_OPTION) {
            अगर (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                काष्ठा matched_sval *पूर्व;
                darray_foreach(पूर्व, arr) {
                    अगर (svaleq(पूर्व->sval, val.sval)) {
                        /* Merge options */
                        अगर (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            पूर्व->layouts = val.layouts;
                        } अन्यथा अगर (पूर्व->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            पूर्व->layouts |= val.layouts;
                        }
                        जाओ अग्र_मूल्य;
                    }
                }
            }
            darray_append(arr, val);
        }

अग्र_मूल्य:
        अगर (*s == '\0') अवरोध;
        अगर (*s == ',') s++;
    }

    अगर (darray_empty(arr))
        जाओ ensure_at_least_one_मूल्य;

    वापस arr;
}

अटल काष्ठा matcher *
matcher_new_from_rmlvo(स्थिर काष्ठा xkb_rmlvo_builder *rmlvo, स्थिर अक्षर **rules)
{
    काष्ठा matcher *m = सुस्मृति(1, माप(*m));
    अगर (!m)
        वापस शून्य;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    काष्ठा xkb_rule_names names = {
        .rules = rmlvo->rules,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_empty(rmlvo->layouts)) ? शून्य : "x",
        .variant = (darray_empty(rmlvo->layouts)) ? शून्य : "x",
        .options = (darray_empty(rmlvo->options)) ? शून्य : "x",
    };
    स्थिर क्रमागत RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
                                                               &names);

    /* Initialize matcher with sanitized names, if relevant */
    अगर (changed & RMLVO_RULES) {
        *rules = names.rules;
    } अन्यथा {
        *rules = rmlvo->rules;
    }

    अगर (changed & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = names.model;
    } अन्यथा {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;

    निश्चित((changed & RMLVO_LAYOUT) || !(changed & RMLVO_VARIANT));
    अगर (changed & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = split_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      names.layout);
        m->rmlvo.variants = split_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       names.variant);
        अगर (darray_आकार(m->rmlvo.layouts) > darray_आकार(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            अगर (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_आकार(m->rmlvo.layouts));
        } अन्यथा अगर (darray_आकार(m->rmlvo.layouts) < darray_आकार(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_आकार(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } अन्यथा {
        काष्ठा xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            काष्ठा matched_sval val = {
                .sval = SVAL(layout->layout, strlen_safe(layout->layout)),
                .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
                .matched = 0
            };
            darray_append(m->rmlvo.layouts, val);
            val.sval.start = layout->variant;
            val.sval.len = strlen_safe(layout->variant);
            darray_append(m->rmlvo.variants, val);
        }
    }

    अगर (changed & RMLVO_OPTIONS) {
        m->rmlvo.options = split_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      names.options);
    } अन्यथा {
        काष्ठा xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            काष्ठा matched_sval val = {
                .sval = SVAL(option->option, strlen_safe(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    वापस m;
}

अटल काष्ठा matcher *
matcher_new_from_names(काष्ठा xkb_context *ctx,
            स्थिर काष्ठा xkb_rule_names *rmlvo)
{
    काष्ठा matcher *m = सुस्मृति(1, माप(*m));
    अगर (!m)
        वापस शून्य;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    अगर (darray_आकार(m->rmlvo.layouts) > darray_आकार(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        अगर (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_आकार(m->rmlvo.layouts));
    } अन्यथा अगर (darray_आकार(m->rmlvo.layouts) < darray_आकार(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_आकार(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    वापस m;
}

अटल व्योम
matcher_free(काष्ठा matcher *m)
{
    अगर (!m)
        वापस;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    काष्ठा group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.बफर);
    darray_free(m->pending_kccgst.slices);
    क्रम (kccgst_सूचकांक_t i = 0; i < (kccgst_सूचकांक_t) KCCGST_अंक_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    मुक्त(m);
}

अटल व्योम
matcher_group_start_new(काष्ठा matcher *m, काष्ठा sval name)
{
    काष्ठा group group = { .name = name, .elements = darray_new() };
    darray_append(m->groups, group);
}

अटल व्योम
matcher_group_add_element(काष्ठा matcher *m, काष्ठा scanner *s,
                          काष्ठा sval element)
{
    darray_append(darray_item(m->groups, darray_आकार(m->groups) - 1).elements,
                  element);
}

अटल bool
read_rules_file(काष्ठा xkb_context *ctx,
                काष्ठा matcher *matcher,
                अचिन्हित पूर्णांक include_depth,
                खाता *file,
                स्थिर अक्षर *path);

अटल व्योम
matcher_include(काष्ठा matcher *m, काष्ठा scanner *parent_scanner,
                अचिन्हित पूर्णांक include_depth,
                काष्ठा sval inc)
{
    अगर (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        वापस;
    }

    स्थिर अक्षर *stmt_file = inc.start;
    माप_प्रकार stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    अक्षर buf[PATH_MAX] = {0};
    स्थिर ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, माप(buf));
    अगर (expanded < 0) {
        /* Error */
        वापस;
    } अन्यथा अगर (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (माप_प्रकार) expanded;
        निश्चित(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    खाता* file;
    अचिन्हित पूर्णांक offset = 0;
    स्थिर bool absolute_path = is_absolute_path(stmt_file);
    अगर (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        अगर (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            अगर (stmt_file_len < माप(buf)) {
                स_नकल(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } अन्यथा {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, माप(buf),
                        (अचिन्हित पूर्णांक) stmt_file_len, stmt_file);
                वापस;
            }
        } अन्यथा {
            /* %-expansion is always NULL-terminated */
            निश्चित(stmt_file[stmt_file_len] == '\0');
        }
        file = ख_खोलो(stmt_file, "rb");
    } अन्यथा {
        /* Relative path: lookup the first XKB path */
        अगर (unlikely(expanded)) {
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
            file = शून्य;
        } अन्यथा {
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, माप(buf), &offset, true);
        }
    }

    जबतक (file) {
        निश्चित(strlen_safe(buf) < माप(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        ख_बंद(file);
        अगर (ret)
            वापस;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        अगर (absolute_path) {
            /* There is no point to search further if the path is absolute */
            अवरोध;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, माप(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (अचिन्हित पूर्णांक) stmt_file_len, stmt_file);
}

अटल व्योम
matcher_mapping_start_new(काष्ठा matcher *m)
{
    क्रम (mlvo_सूचकांक_t i = 0; i < (mlvo_सूचकांक_t) MLVO_अंक_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_अंक_ENTRIES;
    क्रम (kccgst_सूचकांक_t i = 0; i < (kccgst_सूचकांक_t) KCCGST_अंक_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_अंक_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.अंक_mlvo = m->mapping.अंक_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
अटल पूर्णांक
parse_layout_int_सूचकांक_पुच्छ(स्थिर अक्षर *s, माप_प्रकार max_len,
                            xkb_layout_सूचकांक_t *out)
{
    अगर (max_len < 3)
        वापस -1;
    uint32_t val = 0;
    स्थिर पूर्णांक गिनती = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    अगर (गिनती <= 0 || s[1 + गिनती] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        वापस -1;
    /* To zero-based index. */
    *out = val - 1;
    वापस गिनती + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
अटल पूर्णांक
extract_layout_सूचकांक(स्थिर अक्षर *s, माप_प्रकार max_len, xkb_layout_सूचकांक_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    अगर (max_len < 3 || s[0] != '[')
        वापस -1;
    अगर (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_सूचकांक_t)(s[1] - '1');
        वापस 3;
    }
    अगर (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        वापस 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    वापस parse_layout_int_सूचकांक_पुच्छ(s, max_len, out);
}

/* Special layout indices */
क्रमागत layout_सूचकांक_ranges {
    LAYOUT_सूचकांक_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_सूचकांक_FIRST,
    LAYOUT_सूचकांक_LATER,
    LAYOUT_सूचकांक_MULTIPLE,
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
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_MULTIPLE &&
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_MULTIPLE <
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_ANY &&
              (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_ANY <
              (xkb_layout_सूचकांक_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
अटल bool
extract_mapping_layout_सूचकांक(स्थिर अक्षर *s, माप_प्रकार max_len,
                             xkb_layout_सूचकांक_t *out)
{
    /* Check for minimal `[` + index + `]` */
    अगर (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        वापस false;
    }

    /* Small numeric index */
    अगर (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_सूचकांक_t)(s[1] - '1');
        वापस true;
    }

    /* Named indices ranges */
    अटल स्थिर काष्ठा {
        स्थिर अक्षर* name;
        माप_प्रकार आकार;
        xkb_layout_सूचकांक_t मूल्य;
    } names[] = {
        #घोषणा सूचकांक_RANGE(पुच्छ, मूल्य) \
            { (पुच्छ), माप(पुच्छ), (xkb_layout_सूचकांक_t)(मूल्य) }
        सूचकांक_RANGE("single]",   LAYOUT_सूचकांक_SINGLE),
        सूचकांक_RANGE("first]",    LAYOUT_सूचकांक_FIRST),
        सूचकांक_RANGE("later]",    LAYOUT_सूचकांक_LATER),
        सूचकांक_RANGE("multiple]", LAYOUT_सूचकांक_MULTIPLE),
        सूचकांक_RANGE("any]",      LAYOUT_सूचकांक_ANY),
        #अघोषित सूचकांक_RANGE
    };

    क्रम (माप_प्रकार k = 0; k < ARRAY_आकार(names); k++) {
        अगर (max_len == names[k].आकार &&
            स_भेद(&s[1], names[k].name, names[k].आकार - 1) == 0) {
            *out = names[k].मूल्य;
            वापस true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    स्थिर पूर्णांक गिनती = parse_layout_int_सूचकांक_पुच्छ(s, max_len, out);
    वापस गिनती > 0 && (माप_प्रकार)गिनती == max_len;
}

अटल अंतरभूत bool
is_mlvo_mask_defined(काष्ठा matcher *m, क्रमागत rules_mlvo mlvo)
{
    वापस m->mapping.defined_mlvo_mask & (1u << mlvo);
}

अटल व्योम
matcher_mapping_set_mlvo(काष्ठा matcher *m, काष्ठा scanner *s,
                         काष्ठा sval ident)
{
    क्रमागत rules_mlvo mlvo;
    काष्ठा sval mlvo_sval;

    क्रम (mlvo = 0; mlvo < MLVO_अंक_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        अगर (svaleq_prefix(mlvo_sval, ident))
            अवरोध;
    }

    /* Not found. */
    अगर (mlvo >= MLVO_अंक_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (अचिन्हित पूर्णांक) ident.len, ident.start);
        m->mapping.active = false;
        वापस;
    }

    अगर (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (अचिन्हित पूर्णांक) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        वापस;
    }

    /* If there are leftovers still, it must be an index. */
    अगर (mlvo_sval.len < ident.len) {
        xkb_layout_सूचकांक_t idx;
        अगर (!extract_mapping_layout_सूचकांक(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (अचिन्हित पूर्णांक) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            वापस;
        }

        अगर (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        अन्यथा अगर (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        अन्यथा {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (अचिन्हित पूर्णांक) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            वापस;
        }
    } अन्यथा अगर (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_SINGLE;
    } अन्यथा अगर (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_सूचकांक_t) LAYOUT_सूचकांक_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    अगर (((mlvo == MLVO_LAYOUT && is_mlvo_mask_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_mask_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.active = false;
        वापस;
    }

    m->mapping.mlvo_at_pos[m->mapping.अंक_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.अंक_mlvo++;
}

अटल व्योम
matcher_mapping_set_layout_bounds(काष्ठा matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_सूचकांक_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    चयन (idx) {
        हाल XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            निश्चित(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            अवरोध;
        हाल LAYOUT_सूचकांक_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_आकार(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            अवरोध;
        हाल LAYOUT_सूचकांक_MULTIPLE:
        हाल LAYOUT_सूचकांक_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_आकार(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            अवरोध;
        हाल LAYOUT_सूचकांक_SINGLE:
        हाल LAYOUT_सूचकांक_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        शेष:
            /* Mere layout index */
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = idx;
            m->mapping.layout_idx_max = idx + 1;
            m->mapping.layouts_candidates_mask = UINT32_C(1) << idx;
    }
    m->mapping.has_multiple_layouts = (
        m->mapping.layout_idx_max > m->mapping.layout_idx_min &&
        m->mapping.layout_idx_max - m->mapping.layout_idx_min > 1
    );
}

अटल व्योम
matcher_mapping_set_kccgst(काष्ठा matcher *m, काष्ठा scanner *s, काष्ठा sval ident)
{
    क्रमागत rules_kccgst kccgst;
    काष्ठा sval kccgst_sval;

    क्रम (kccgst = 0; kccgst < KCCGST_अंक_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        अगर (svaleq(rules_kccgst_svals[kccgst], ident))
            अवरोध;
    }

    /* Not found. */
    अगर (kccgst >= KCCGST_अंक_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (अचिन्हित पूर्णांक) ident.len, ident.start);
        m->mapping.active = false;
        वापस;
    }

    अगर (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (अचिन्हित पूर्णांक) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        वापस;
    }

    m->mapping.kccgst_at_pos[m->mapping.अंक_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.अंक_kccgst++;
}

अटल bool
matcher_mapping_verify(काष्ठा matcher *m, काष्ठा scanner *s)
{
    अगर (m->mapping.अंक_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        जाओ skip;
    }

    अगर (m->mapping.अंक_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        जाओ skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    अगर (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        निश्चित(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        चयन (m->mapping.layout_idx) {
            हाल LAYOUT_सूचकांक_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                अगर (darray_आकार(m->rmlvo.layouts) > 1)
                    जाओ skip;
                अवरोध;
            हाल LAYOUT_सूचकांक_MULTIPLE:
            हाल LAYOUT_सूचकांक_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                अगर (darray_आकार(m->rmlvo.layouts) < 2)
                    जाओ skip;
                अवरोध;
            हाल LAYOUT_सूचकांक_ANY:
            हाल LAYOUT_सूचकांक_FIRST:
                /* No restrictions */
                अवरोध;
            शेष:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                अगर (darray_आकार(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_आकार(m->rmlvo.layouts))
                    जाओ skip;
        }
    }

    अगर (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        निश्चित(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        चयन (m->mapping.variant_idx) {
            हाल LAYOUT_सूचकांक_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                अगर (darray_आकार(m->rmlvo.variants) > 1)
                    जाओ skip;
                अवरोध;
            हाल LAYOUT_सूचकांक_ANY:
            हाल LAYOUT_सूचकांक_LATER:
            हाल LAYOUT_सूचकांक_FIRST:
                /* No restriction */
                अवरोध;
            शेष:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                अगर (darray_आकार(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_आकार(m->rmlvo.variants))
                    जाओ skip;
        }
    }

    वापस true;

skip:
    m->mapping.active = false;
    वापस false;
}

अटल व्योम
matcher_rule_start_new(काष्ठा matcher *m)
{
    स_रखो(&m->rule, 0, माप(m->rule));
    m->rule.skip = !m->mapping.active;
}

अटल व्योम
matcher_rule_set_mlvo_common(काष्ठा matcher *m, काष्ठा scanner *s,
                             काष्ठा sval ident,
                             क्रमागत mlvo_match_type match_type)
{
    अगर (m->rule.अंक_mlvo_मूल्यावली >= m->mapping.अंक_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        वापस;
    }
    m->rule.match_type_at_pos[m->rule.अंक_mlvo_मूल्यावली] = match_type;
    m->rule.mlvo_मूल्य_at_pos[m->rule.अंक_mlvo_मूल्यावली] = ident;
    m->rule.अंक_mlvo_मूल्यावली++;
}

अटल व्योम
matcher_rule_set_mlvo_wildcard(काष्ठा matcher *m, काष्ठा scanner *s,
                               क्रमागत mlvo_match_type match_type)
{
    काष्ठा sval dummy = SVAL(शून्य, 0);
    matcher_rule_set_mlvo_common(m, s, dummy, match_type);
}

अटल व्योम
matcher_rule_set_mlvo_group(काष्ठा matcher *m, काष्ठा scanner *s,
                            काष्ठा sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_GROUP);
}

अटल व्योम
matcher_rule_set_mlvo(काष्ठा matcher *m, काष्ठा scanner *s,
                      काष्ठा sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_NORMAL);
}

अटल व्योम
matcher_rule_set_kccgst(काष्ठा matcher *m, काष्ठा scanner *s,
                        काष्ठा sval ident)
{
    अगर (m->rule.अंक_kccgst_मूल्यावली >= m->mapping.अंक_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        वापस;
    }
    m->rule.kccgst_मूल्य_at_pos[m->rule.अंक_kccgst_मूल्यावली] = ident;
    m->rule.अंक_kccgst_मूल्यावली++;
}

अटल bool
match_group(काष्ठा matcher *m, काष्ठा sval group_name, काष्ठा sval to)
{
    काष्ठा group *group = शून्य;
    काष्ठा sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        अगर (svaleq(group->name, group_name)) {
            found = true;
            अवरोध;
        }
    }

    अगर (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        वापस false;
    }

    darray_foreach(element, group->elements)
        अगर (svaleq(to, *element))
            वापस true;

    वापस false;
}

अटल bool
match_मूल्य(काष्ठा matcher *m, स्थिर काष्ठा sval val, स्थिर काष्ठा sval to,
            क्रमागत mlvo_match_type match_type,
            क्रमागत wildcard_match_type wildcard_type)
{
    चयन (match_type) {
        हाल MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            वापस wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        हाल MLVO_MATCH_WILDCARD_NONE:
            वापस !to.len;
        हाल MLVO_MATCH_WILDCARD_SOME:
            वापस !!to.len;
        हाल MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            वापस true;
        हाल MLVO_MATCH_GROUP:
            वापस match_group(m, val, to);
        शेष:
            निश्चित(match_type == MLVO_MATCH_NORMAL);
            वापस svaleq(val, to);
    }
}

अटल bool
match_मूल्य_and_mark(काष्ठा matcher *m, स्थिर काष्ठा sval val,
                     काष्ठा matched_sval *to, क्रमागत mlvo_match_type match_type,
                     क्रमागत wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_मूल्य(m, val, to->sval, match_type, wildcard_type);
    अगर (matched)
        to->matched |= layouts;
    वापस matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
अटल bool
expand_rmlvo_in_kccgst_मूल्य(काष्ठा matcher *m, काष्ठा scanner *s,
                             काष्ठा sval मूल्य, xkb_layout_सूचकांक_t layout_idx,
                             darray_char *expanded, माप_प्रकार *i)
{
    स्थिर अक्षर *str = मूल्य.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    क्रमागत rules_mlvo mlv;
    xkb_layout_सूचकांक_t idx;
    अक्षर pfx, sfx;
    काष्ठा matched_sval *expanded_मूल्य;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    अगर (str[*i] == 'i' &&
        (*i + 1 == मूल्य.len || is_merge_mode_prefix(str[*i + 1])))
    {
        अगर (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_सूचकांक_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            जाओ error;
        }
        (*i)++;
        अक्षर सूचकांक_str[MAX_LAYOUT_सूचकांक_STR_लंबाई + 1];
        स्थिर पूर्णांक गिनती = snprintf(सूचकांक_str, माप(सूचकांक_str), "%"PRIu32,
                                   layout_idx + 1);
        निश्चित(गिनती > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, सूचकांक_str, (darray_आकार_t)गिनती);
        वापस true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    अगर (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        अगर (str[*i] == '(') sfx = ')';
        अगर (++(*i) >= मूल्य.len) जाओ error;
    }

    /* Mandatory model/layout/variant specifier. */
    चयन (str[(*i)++]) {
    हाल 'm': mlv = MLVO_MODEL; अवरोध;
    हाल 'l': mlv = MLVO_LAYOUT; अवरोध;
    हाल 'v': mlv = MLVO_VARIANT; अवरोध;
    शेष: जाओ error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_सूचकांक = false;
    अगर (*i < मूल्य.len && str[*i] == '[') {
        अगर (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            जाओ error;
        }

        पूर्णांक consumed = extract_layout_सूचकांक(str + (*i), मूल्य.len - (*i), &idx);
        अगर (consumed <= 0) जाओ error;
        अगर (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            निश्चित(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_सूचकांक = true;
        }
        *i += (माप_प्रकार)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    अगर (sfx != 0) {
        अगर (*i >= मूल्य.len) जाओ error;
        अगर (str[(*i)++] != sfx) जाओ error;
    }

    /* Get the expanded value. */
    expanded_मूल्य = शून्य;

    अगर (mlv == MLVO_LAYOUT) {
        अगर (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            अगर (darray_आकार(m->rmlvo.layouts) == 1)
                expanded_मूल्य = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } अन्यथा अगर (idx < darray_आकार(m->rmlvo.layouts) &&
                   (variable_सूचकांक || darray_आकार(m->rmlvo.layouts) > 1)) {
                expanded_मूल्य = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    अन्यथा अगर (mlv == MLVO_VARIANT) {
        अगर (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            अगर (darray_आकार(m->rmlvo.variants) == 1)
                expanded_मूल्य = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } अन्यथा अगर (idx < darray_आकार(m->rmlvo.variants) &&
                   (variable_सूचकांक || darray_आकार(m->rmlvo.variants) > 1)) {
                expanded_मूल्य = &darray_item(m->rmlvo.variants, idx);
        }
    }
    अन्यथा अगर (mlv == MLVO_MODEL) {
        expanded_मूल्य = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    अगर (!expanded_मूल्य || expanded_मूल्य->sval.len == 0) {
        वापस true;
    }

    अगर (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_मूल्य->sval.start,
                                 (darray_आकार_t) expanded_मूल्य->sval.len);
    अगर (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    अगर (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        स्थिर xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_मूल्य->matched |= layouts;
    } अन्यथा {
        expanded_मूल्य->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    वापस true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    वापस false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
अटल व्योम
expand_qualifier_in_kccgst_मूल्य(
    काष्ठा matcher *m, काष्ठा scanner *s,
    काष्ठा sval मूल्य, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_आकार_t prefix_idx, माप_प्रकार *i)
{
    स्थिर अक्षर *str = मूल्य.start;

    /* “all” followed by nothing or by a layout separator */
    अगर (*i + 2 < मूल्य.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == मूल्य.len || is_merge_mode_prefix(str[*i + 3]))) {
        अगर (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_MESSAGE_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        अगर (darray_आकार(m->rmlvo.layouts) > 1) {
            अक्षर layout_सूचकांक[MAX_LAYOUT_सूचकांक_STR_लंबाई + 1];
            स्थिर darray_आकार_t prefix_लंबाई =
                darray_आकार(*expanded) - prefix_idx - 1;
            क्रम (xkb_layout_सूचकांक_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_आकार(m->rmlvo.layouts));
                 l++)
            {
                अगर (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_लंबाई);
                /* Append index */
                स्थिर पूर्णांक गिनती = snprintf(layout_सूचकांक, माप(layout_सूचकांक),
                                           "%"PRIu32, l + 1);
                निश्चित(गिनती > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_सूचकांक,
                                             (darray_आकार_t)गिनती);
            }
        }
        *i += 3;
    }
}

अटल अंतरभूत व्योम
#अगर_घोषित _MSC_VER
concat_kccgst(darray_char *into, darray_आकार_t आकार, _In_reads_(आकार) स्थिर अक्षर* from)
#अन्यथा
concat_kccgst(darray_char *into, darray_आकार_t आकार, स्थिर अक्षर from[अटल आकार])
#पूर्ण_अगर
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    स्थिर bool from_plus = is_merge_mode_prefix(from[0]);
    अगर (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, आकार);
    } अन्यथा {
        स्थिर अक्षर ch =
            (अक्षर) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        स्थिर bool into_plus = is_merge_mode_prefix(ch);
        अगर (into_plus)
            darray_prepends_nullterminate(*into, from, आकार);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
अटल bool
append_expanded_kccgst_मूल्य(काष्ठा matcher *m, काष्ठा scanner *s,
                             bool merge, darray_char *to, काष्ठा sval मूल्य,
                             xkb_layout_सूचकांक_t layout_idx)
{
    स्थिर अक्षर *str = मूल्य.start;
    darray_char expanded = darray_new();
    darray_आकार_t last_item_idx = 0;
    bool has_separator = false;

    क्रम (माप_प्रकार i = 0; i < मूल्य.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        चयन (str[i]) {
            /* Qualifier */
            हाल ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_मूल्य(m, s, मूल्य, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                अवरोध;
            /* Expansion */
            हाल '%':
                i++;
                अगर (i >= मूल्य.len ||
                    !expand_rmlvo_in_kccgst_मूल्य(m, s, मूल्य, layout_idx,
                                                  &expanded, &i))
                        जाओ error;
                अवरोध;
            /* New item */
            हाल MERGE_OVERRIDE_PREFIX:
            हाल MERGE_AUGMENT_PREFIX:
            हाल MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_आकार(expanded) - 1;
                has_separator = true;
                अवरोध;
            /* Just a normal character. */
            शेष:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    अगर (merge) {
        अगर (!darray_empty(expanded))
            concat_kccgst(to, darray_आकार(expanded), darray_वस्तुएँ(expanded));
    } अन्यथा {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    वापस true;

error:
    darray_free(expanded);
    वापस false;
}

अटल bool
matcher_append_pending_kccgst(काष्ठा matcher *m)
{
    अगर (!m->mapping.has_multiple_layouts)
        वापस true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    क्रम (kccgst_सूचकांक_t i = 0; i < m->mapping.अंक_kccgst; i++) {
        स्थिर क्रमागत rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        क्रम (xkb_layout_सूचकांक_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            रेजिस्टर स्थिर काष्ठा kccgst_बफर* स्थिर buf = &m->pending_kccgst;
            माप_प्रकार offset = 0;
            क्रम (darray_आकार_t k = 0; k < darray_आकार(buf->slices); k++) {
                रेजिस्टर स्थिर काष्ठा kccgst_बफर_slice * स्थिर slice =
                    &darray_item(buf->slices, k);
                अगर (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->लंबाई)
                    concat_kccgst(&m->kccgst[kccgst], slice->लंबाई,
                                  darray_वस्तुएँ(buf->बफर) + offset);
                offset += slice->लंबाई;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    वापस true;
}

अटल व्योम
matcher_rule_verify(काष्ठा matcher *m, काष्ठा scanner *s)
{
    अगर (m->rule.अंक_mlvo_मूल्यावली != m->mapping.अंक_mlvo ||
        m->rule.अंक_kccgst_मूल्यावली != m->mapping.अंक_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: must have same number of values "
                    "as mapping line; ignoring rule");
        m->rule.skip = true;
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
अटल bool
matcher_rule_match_model(काष्ठा matcher *m, स्थिर काष्ठा sval मूल्य,
                         क्रमागत mlvo_match_type match_type)
{
    वापस match_मूल्य_and_mark(
        m, मूल्य, &m->rmlvo.model, match_type, WILDCARD_MATCH_ALL,
        /* layout-independent */
        (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS
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
अटल bool
matcher_rule_match_option(काष्ठा matcher *m, स्थिर काष्ठा sval मूल्य,
                          क्रमागत mlvo_match_type match_type,
                          xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    निश्चित(!darray_empty(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_mask_t unmatched =
        (m->mapping.defined_mlvo_mask & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    काष्ठा matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        स्थिर xkb_layout_mask_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
        अगर (matchable && match_मूल्य_and_mark(m, मूल्य, option, match_type,
                                              WILDCARD_MATCH_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            अगर (!unmatched)
                अवरोध;
        }
    }
    अगर (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    वापस matched;
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
अटल bool
matcher_rule_match_layout_or_variant(काष्ठा matcher *m,
                                     darray_matched_sval *निवेश,
                                     स्थिर काष्ठा sval मूल्य,
                                     क्रमागत mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    निश्चित(!darray_empty(*निवेश));

    bool matched = false;
    /* Loop over the layout index range */
    क्रम (xkb_layout_सूचकांक_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        स्थिर xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        अगर (layout & *candidates) {
            काष्ठा matched_sval *to = &darray_item(*निवेश, idx);
            अगर (match_मूल्य_and_mark(m, मूल्य, to, match_type,
                                     WILDCARD_MATCH_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } अन्यथा {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    वापस matched;
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
अटल व्योम
matcher_rule_apply_if_matches(काष्ठा matcher *m, काष्ठा scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_mask_t candidate_layouts = m->mapping.layouts_candidates_mask;

    /* Loop over MLVO pattern components */
    क्रम (mlvo_सूचकांक_t i = 0; i < m->mapping.अंक_mlvo; i++) {
        स्थिर क्रमागत rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        स्थिर काष्ठा sval मूल्य = m->rule.mlvo_मूल्य_at_pos[i];
        स्थिर क्रमागत mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        चयन (mlvo) {
        हाल MLVO_MODEL:
            matched = matcher_rule_match_model(m, मूल्य, match_type);
            अवरोध;
        हाल MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, मूल्य, match_type, &candidate_layouts
            );
            अवरोध;
        हाल MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, मूल्य, match_type, &candidate_layouts
            );
            अवरोध;
        हाल MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, मूल्य, match_type, &candidate_layouts
            );
            अवरोध;
        शेष: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_अंक_ENTRIES - 1,
                          "Unexpected MLVO field");
            निश्चित(!"Unreachable");
        }
        }

        अगर (!matched)
            वापस;
    }

    अगर (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        क्रम (xkb_layout_सूचकांक_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            अगर (candidate_layouts & (UINT32_C(1) << idx)) {
                क्रम (kccgst_सूचकांक_t i = 0; i < m->mapping.अंक_kccgst; i++) {
                    स्थिर क्रमागत rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    स्थिर काष्ठा sval मूल्य = m->rule.kccgst_मूल्य_at_pos[i];
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
                    काष्ठा kccgst_बफर * स्थिर buf = &m->pending_kccgst;
                    स्थिर darray_आकार_t पूर्व_बफर_लंबाई =
                        darray_आकार(buf->बफर);
                    append_expanded_kccgst_मूल्य(m, s, false, &buf->बफर,
                                                 मूल्य, idx);
                    स्थिर uint32_t लंबाई = (uint32_t) (darray_आकार(buf->बफर)
                                          - पूर्व_बफर_लंबाई);
                    स्थिर काष्ठा kccgst_बफर_slice slice = {
                        .लंबाई = लंबाई,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } अन्यथा {
        /* Numeric index or no index */
        क्रम (kccgst_सूचकांक_t i = 0; i < m->mapping.अंक_kccgst; i++) {
            क्रमागत rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            काष्ठा sval मूल्य = m->rule.kccgst_मूल्य_at_pos[i];
            append_expanded_kccgst_मूल्य(m, s, true, &m->kccgst[kccgst], मूल्य,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    अगर (!(is_mlvo_mask_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_mask &= ~candidate_layouts;
    }
}

अटल क्रमागत rules_token
gettok(काष्ठा matcher *m, काष्ठा scanner *s)
{
    वापस lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
अटल bool
matcher_match(काष्ठा matcher *m, काष्ठा scanner *s,
              अचिन्हित पूर्णांक include_depth,
              स्थिर अक्षर *string, माप_प्रकार len,
              स्थिर अक्षर *file_name)
{
    क्रमागत rules_token tok;

    अगर (!m)
        वापस false;

initial:
    चयन (tok = gettok(m, s)) {
    हाल TOK_BANG:
        जाओ bang;
    हाल TOK_END_OF_पंक्ति:
        जाओ initial;
    हाल TOK_END_OF_FILE:
        जाओ finish;
    शेष:
        जाओ unexpected;
    }

bang:
    चयन (tok = gettok(m, s)) {
    हाल TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        जाओ group_name;
    हाल TOK_INCLUDE:
        जाओ include_statement;
    हाल TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        जाओ mapping_mlvo;
    शेष:
        जाओ unexpected;
    }

group_name:
    चयन (tok = gettok(m, s)) {
    हाल TOK_EQUALS:
        जाओ group_element;
    शेष:
        जाओ unexpected;
    }

group_element:
    चयन (tok = gettok(m, s)) {
    हाल TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        जाओ group_element;
    हाल TOK_END_OF_पंक्ति:
        जाओ initial;
    शेष:
        जाओ unexpected;
    }

include_statement:
    चयन (tok = gettok(m, s)) {
    हाल TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        जाओ include_statement_end;
    शेष:
        जाओ unexpected;
    }

include_statement_end:
    चयन (tok = gettok(m, s)) {
    हाल TOK_END_OF_पंक्ति:
        जाओ initial;
    शेष:
        जाओ unexpected;
    }

mapping_mlvo:
    चयन (tok = gettok(m, s)) {
    हाल TOK_IDENTIFIER:
        अगर (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        जाओ mapping_mlvo;
    हाल TOK_EQUALS:
        जाओ mapping_kccgst;
    शेष:
        जाओ unexpected;
    }

mapping_kccgst:
    चयन (tok = gettok(m, s)) {
    हाल TOK_IDENTIFIER:
        अगर (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        जाओ mapping_kccgst;
    हाल TOK_END_OF_पंक्ति:
        अगर (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            अगर (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_आकार(m->pending_kccgst.बफर) = 0;
                darray_आकार(m->pending_kccgst.slices) = 0;
            }
        }
        जाओ rule_mlvo_first;
    शेष:
        जाओ unexpected;
    }

rule_mlvo_first:
    चयन (tok = gettok(m, s)) {
    हाल TOK_BANG:
        matcher_append_pending_kccgst(m);
        जाओ bang;
    हाल TOK_END_OF_पंक्ति:
        जाओ rule_mlvo_first;
    हाल TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        जाओ finish;
    शेष:
        matcher_rule_start_new(m);
        जाओ rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    चयन (tok) {
    हाल TOK_IDENTIFIER:
        अगर (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        जाओ rule_mlvo;
    हाल TOK_WILD_CARD_STAR:
        अगर (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        जाओ rule_mlvo;
    हाल TOK_WILD_CARD_NONE:
        अगर (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        जाओ rule_mlvo;
    हाल TOK_WILD_CARD_SOME:
        अगर (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        जाओ rule_mlvo;
    हाल TOK_WILD_CARD_ANY:
        अगर (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        जाओ rule_mlvo;
    हाल TOK_GROUP_NAME:
        अगर (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        जाओ rule_mlvo;
    हाल TOK_EQUALS:
        जाओ rule_kccgst;
    शेष:
        जाओ unexpected;
    }

rule_kccgst:
    चयन (tok = gettok(m, s)) {
    हाल TOK_IDENTIFIER:
        अगर (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        जाओ rule_kccgst;
    हाल TOK_END_OF_पंक्ति:
        अगर (!m->rule.skip)
            matcher_rule_verify(m, s);
        अगर (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        जाओ rule_mlvo_first;
    शेष:
        जाओ unexpected;
    }

unexpected:
    चयन (tok) {
    हाल TOK_ERROR:
        जाओ error;
    शेष:
        जाओ state_error;
    }

finish:
    वापस true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    वापस false;
}
// NOLINTEND(readability-trivial-switch)

अटल bool
read_rules_file(काष्ठा xkb_context *ctx,
                काष्ठा matcher *matcher,
                अचिन्हित पूर्णांक include_depth,
                खाता *file,
                स्थिर अक्षर *path)
{
    bool ret;
    अक्षर *string;
    माप_प्रकार आकार;
    काष्ठा scanner scanner;

    अगर (!map_file(file, &string, &आकार)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, म_त्रुटि(त्रुटि_सं));
        वापस false;
    }

    scanner_init(&scanner, matcher->ctx, string, आकार, path, शून्य);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    अगर (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, आकार);
        वापस false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, आकार, path);
    unmap_file(string, आकार);
    वापस ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
अटल bool
xkb_resolve_partial_rules(काष्ठा xkb_context *ctx, अक्षर *path, माप_प्रकार path_आकार,
                          स्थिर अक्षर* rules, स्थिर अक्षर* suffix,
                          काष्ठा matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    अक्षर partial_rules[60]; /* Arbitrary, but we do not expect long names */
    अगर (unlikely(!snprintf_safe(partial_rules, माप(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        वापस false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    अचिन्हित पूर्णांक offset = 0;
    खाता *file = शून्य;
    स्थिर माप_प्रकार len = म_माप(partial_rules);
    जबतक ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_आकार, &offset, false)) != शून्य) {
        स्थिर bool ok = read_rules_file(ctx, matcher, 0, file, path);
        ख_बंद(file);
        अगर (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            वापस false;
        }
        offset++;
    }

    वापस true;
}

अटल bool
xkb_resolve_rules(काष्ठा xkb_context *ctx,
                  स्थिर अक्षर* rules, काष्ठा matcher *matcher,
                  काष्ठा xkb_component_names *out,
                  xkb_layout_सूचकांक_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    अचिन्हित पूर्णांक offset = 0;
    अक्षर path[PATH_MAX];
    खाता * स्थिर file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, म_माप(rules), FILE_TYPE_RULES,
                                          path, माप(path), &offset, true);
    अगर (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        जाओ err_out;
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
    ret = xkb_resolve_partial_rules(ctx, path, माप(path),
                                    rules, ".pre", matcher);
    अगर (!ret)
        जाओ err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    अगर (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        जाओ err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, माप(path),
                                    rules, ".post", matcher);
    अगर (!ret)
        जाओ err_out;

    /* Check we got required components */
    अगर (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        जाओ err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, शून्य);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, शून्य);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, शून्य);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, शून्य);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, शून्य);

    काष्ठा matched_sval *mval = &matcher->rmlvo.model;
    अगर (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मूल्य,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (अचिन्हित पूर्णांक) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        अगर (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मूल्य,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (अचिन्हित पूर्णांक) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        अगर (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मूल्य,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (अचिन्हित पूर्णांक) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        अगर (!mval->sval.len)
            जारी;

        अगर (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मूल्य,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (अचिन्हित पूर्णांक) mval->sval.len, mval->sval.start, rules);
        } अन्यथा अगर (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            स्थिर xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            निश्चित(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मूल्य,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (अचिन्हित पूर्णांक)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    अगर (out->symbols != शून्य && explicit_layouts != शून्य) {
        *explicit_layouts = 1; /* at least one group */
        स्थिर अक्षर *symbols = out->symbols;
        /* Take the highest modifier */
        जबतक ((symbols = म_अक्षर(symbols, ':')) != शून्य && symbols[1] != '\0') {
            xkb_layout_सूचकांक_t group = 0;
            स्थिर पूर्णांक गिनती = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            अगर (गिनती > 0 && (symbols[गिनती] == '\0' ||
                is_merge_mode_prefix(symbols[गिनती])) &&
                group > 0 && group <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, group);
                symbols += गिनती;
            }
        }
    }

err_out:
    अगर (file)
        ख_बंद(file);

    वापस ret;
}

bool
xkb_components_from_rmlvo_builder(स्थिर काष्ठा xkb_rmlvo_builder *rmlvo,
                                  काष्ठा xkb_component_names *out,
                                  xkb_layout_सूचकांक_t *explicit_layouts)
{
    स्थिर अक्षर *rules = rmlvo->rules;
    काष्ठा matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    अगर (!matcher)
        वापस false;

    स्थिर bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    वापस ret;
}


bool
xkb_components_from_rules_names(काष्ठा xkb_context *ctx,
                                स्थिर काष्ठा xkb_rule_names *rmlvo,
                                काष्ठा xkb_component_names *out,
                                xkb_layout_सूचकांक_t *explicit_layouts)
{
    काष्ठा matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    अगर (!matcher)
        वापस false;

    स्थिर bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    वापस ret;
}
