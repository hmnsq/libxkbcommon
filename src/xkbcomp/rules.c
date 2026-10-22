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

#define MAX_INCLUDE_DEPTH 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
सङ्घ lvalue {
    संरचना sval string;
};

गणना rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_पङ्क्तिः,
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

स्थिर गणना rules_token
lex(संरचना scanner *s, सङ्घ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    याव (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    सचे (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    सचे (scanner_eol(s)) {
        याव (scanner_eol(s)) scanner_next(s);
        पटिनिवत्त TOK_END_OF_पङ्क्तिः;
    }

    /* Escaped line continuation */
    सचे (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        सचे (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            पटिनिवत्त TOK_ERROR;
        }
        scanner_next(s);
        गच्छ skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    सचे (scanner_eof(s)) पटिनिवत्त TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    गणना rules_token token = TOK_IDENTIFIER;

    ध्रुव वर्ण next = scanner_next(s);
    विकल्प (next) {
    पक्ष '!':
        पटिनिवत्त TOK_BANG;
    पक्ष '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        भङ्ग;
    पक्ष '*':
        सचे (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            भङ्ग;
        } अञ्ञथा {
            /* Legacy wild card */
            पटिनिवत्त TOK_WILD_CARD_STAR;
        }
    पक्ष '<':
        /* Extended wild cards */
        सचे (scanner_rules_lit_token(s, "none>")) पटिनिवत्त TOK_WILD_CARD_NONE;
        सचे (scanner_rules_lit_token(s, "some>")) पटिनिवत्त TOK_WILD_CARD_SOME;
        सचे (scanner_rules_lit_token(s, "any>")) पटिनिवत्त TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        प्रतिज्ञा(scanner_rules_is_ident('<'));
        val->string.len = 1;
        भङ्ग;
    पक्ष '=':
        सचे (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            भङ्ग;
        } अञ्ञथा {
            पटिनिवत्त TOK_EQUALS;
        }
    पक्ष 'i':
        सचे (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            पटिनिवत्त TOK_INCLUDE;
        }
        /* Identifier */
        प्रतिज्ञा(scanner_rules_is_ident('i'));
        val->string.len = 1;
        भङ्ग;
    स्वभाव:
        सचे (likely(scanner_rules_is_ident(next))) {
            /* Identifier */
            val->string.len = 1;
        } अञ्ञथा {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.start = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    प्रतिज्ञा(scanner_rules_is_ident(MERGE_OVERRIDE_PREFIX));
    प्रतिज्ञा(scanner_rules_is_ident(MERGE_AUGMENT_PREFIX));
    प्रतिज्ञा(scanner_rules_is_ident(MERGE_REPLACE_PREFIX));
    प्रतिज्ञा(scanner_rules_is_ident('('));
    प्रतिज्ञा(scanner_rules_is_ident(')'));
    val->string.len += scanner_rules_ident_token(s);

    सचे (unlikely(val->string.len == 0)) {
        सचे (token == TOK_GROUP_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } अञ्ञथा {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unrecognized token");
        }
        पटिनिवत्त TOK_ERROR;
    }

    पटिनिवत्त token;
}

/***====================================================================***/

गणना rules_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_अङ्कः_ENTRIES
};

गणना {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

प्रकारनिर्देश uint8_t mlvo_index_t;
प्रकारनिर्देश uint8_t mlvo_mask_t;


स्थिर ध्रुव संरचना sval rules_mlvo_svals[MLVO_अङ्कः_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

गणना rules_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_अङ्कः_ENTRIES
};

प्रकारनिर्देश uint8_t kccgst_index_t;
प्रकारनिर्देश uint8_t kccgst_mask_t;

स्थिर ध्रुव संरचना sval rules_kccgst_svals[KCCGST_अङ्कः_ENTRIES] = {
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

static_assert(~(xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL ==
              (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
संरचना matched_sval {
    संरचना sval sval;
    /** Used for layout-specific options */
    xkb_layout_mask_t layouts;
    xkb_layout_mask_t matched;
};
प्रकारनिर्देश darray(संरचना matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
संरचना rule_names {
    संरचना matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

संरचना group {
    संरचना sval name;
    darray_sval elements;
};

संरचना mapping {
    गणना rules_mlvo mlvo_at_pos[MLVO_अङ्कः_ENTRIES];
    mlvo_index_t अङ्कः_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    सङ्घ {
        संरचना { xkb_layout_index_t layout_idx, variant_idx; };
        संरचना { xkb_layout_index_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    सङ्घ {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    गणना rules_kccgst kccgst_at_pos[KCCGST_अङ्कः_ENTRIES];
    kccgst_index_t अङ्कः_kccgst;
    kccgst_mask_t defined_kccgst_mask;
};

गणना mlvo_match_type {
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

गणना wildcard_match_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_MATCH_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_MATCH_ALL,
};

संरचना rule {
    संरचना sval mlvo_मानम्_at_pos[MLVO_अङ्कः_ENTRIES];
    गणना mlvo_match_type match_type_at_pos[MLVO_अङ्कः_ENTRIES];
    संरचना sval kccgst_मानम्_at_pos[KCCGST_अङ्कः_ENTRIES];
    mlvo_index_t अङ्कः_mlvo_मानानि;
    kccgst_index_t अङ्कः_kccgst_मानानि;
    bool skip;
};

#define SLICE_KCCGST_BIT_FIELD_SIZE 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_SIZE - 1)) &
                (KCCGST_अङ्कः_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
संरचना kccgst_buffer_slice {
    uint32_t length:(32 - SLICE_KCCGST_BIT_FIELD_SIZE);
    गणना rules_kccgst kccgst:SLICE_KCCGST_BIT_FIELD_SIZE;
    xkb_layout_index_t layout;
};

/* Buffer for pending KcCGST values */
संरचना kccgst_buffer {
    darray_char buffer;
    /* Slice corresponding to each value in the buffer */
    darray(संरचना kccgst_buffer_slice) slices;
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
    संरचना rule_names rmlvo;
    सङ्घ lvalue val;
    darray(संरचना group) groups;
    /* Current mapping. */
    संरचना mapping mapping;
    /* Current rule. */
    संरचना rule rule;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    संरचना kccgst_buffer pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_अङ्कः_ENTRIES];
};

स्थिर संरचना sval
strip_spaces(संरचना sval v)
{
    याव (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    याव (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    पटिनिवत्त v;
}

स्थिर darray_matched_sval
split_comma_separated_mlvo(संरचना xkb_context *ctx,
                           गणना rules_mlvo mlvo, ध्रुव वर्ण *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    सचे (!s) {
        ensure_at_least_one_मानम्:
        /* Label followed by a declaration is a C23 extension */ ;

        संरचना matched_sval val = {
            .sval = SVAL(रिक्त, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        पटिनिवत्त arr;
    }

    याव (true) {
        संरचना matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        याव (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        सचे (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            ध्रुव वर्ण* ध्रुव layout_start = s;
            xkb_layout_index_t layout = XKB_LAYOUT_INVALID;

            पूर्णाङ्क गणनम् = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            सचे (गणनम् > 0) {
                /* Note: 1-indexed layout */
                s += गणनम्;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                सचे (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (unsigned पूर्णाङ्क) val.sval.len, val.sval.start);
                } अञ्ञथा सचे (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (unsigned पूर्णाङ्क) val.sval.len, val.sval.start);
                } अञ्ञथा {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ध्रुव वर्ण* ध्रुव layout_index_end = s;
            याव (*s != '\0' && *s != ',') { s++; }
            सचे (गणनम् <= 0 || layout_index_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (unsigned पूर्णाङ्क) (s - layout_start), layout_start,
                        (unsigned पूर्णाङ्क) val.sval.len, val.sval.start);
            }
        }

        सचे (val.sval.len || mlvo != MLVO_OPTION) {
            सचे (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                संरचना matched_sval *prev;
                darray_foreach(prev, arr) {
                    सचे (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        सचे (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } अञ्ञथा सचे (prev->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        गच्छ next_मानम्;
                    }
                }
            }
            darray_append(arr, val);
        }

next_मानम्:
        सचे (*s == '\0') भङ्ग;
        सचे (*s == ',') s++;
    }

    सचे (darray_empty(arr))
        गच्छ ensure_at_least_one_मानम्;

    पटिनिवत्त arr;
}

स्थिर संरचना matcher *
matcher_new_from_rmlvo(ध्रुव संरचना xkb_rmlvo_builder *rmlvo, ध्रुव वर्ण **rules)
{
    संरचना matcher *m = calloc(1, परिमाण(*m));
    सचे (!m)
        पटिनिवत्त रिक्त;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    संरचना xkb_rule_names names = {
        .rules = rmlvo->rules,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_empty(rmlvo->layouts)) ? रिक्त : "x",
        .variant = (darray_empty(rmlvo->layouts)) ? रिक्त : "x",
        .options = (darray_empty(rmlvo->options)) ? रिक्त : "x",
    };
    ध्रुव गणना RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
                                                               &names);

    /* Initialize matcher with sanitized names, if relevant */
    सचे (changed & RMLVO_RULES) {
        *rules = names.rules;
    } अञ्ञथा {
        *rules = rmlvo->rules;
    }

    सचे (changed & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = names.model;
    } अञ्ञथा {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;

    प्रतिज्ञा((changed & RMLVO_LAYOUT) || !(changed & RMLVO_VARIANT));
    सचे (changed & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = split_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      names.layout);
        m->rmlvo.variants = split_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       names.variant);
        सचे (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            सचे (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        } अञ्ञथा सचे (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } अञ्ञथा {
        संरचना xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            संरचना matched_sval val = {
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

    सचे (changed & RMLVO_OPTIONS) {
        m->rmlvo.options = split_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      names.options);
    } अञ्ञथा {
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

    पटिनिवत्त m;
}

स्थिर संरचना matcher *
matcher_new_from_names(संरचना xkb_context *ctx,
            ध्रुव संरचना xkb_rule_names *rmlvo)
{
    संरचना matcher *m = calloc(1, परिमाण(*m));
    सचे (!m)
        पटिनिवत्त रिक्त;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    सचे (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        सचे (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
    } अञ्ञथा सचे (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    पटिनिवत्त m;
}

स्थिर शून्य
matcher_free(संरचना matcher *m)
{
    सचे (!m)
        पटिनिवत्त;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    संरचना group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.buffer);
    darray_free(m->pending_kccgst.slices);
    अत्थाय (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_अङ्कः_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    मुक्त(m);
}

स्थिर शून्य
matcher_group_start_new(संरचना matcher *m, संरचना sval name)
{
    संरचना group group = { .name = name, .elements = darray_new() };
    darray_append(m->groups, group);
}

स्थिर शून्य
matcher_group_add_element(संरचना matcher *m, संरचना scanner *s,
                          संरचना sval element)
{
    darray_append(darray_item(m->groups, darray_size(m->groups) - 1).elements,
                  element);
}

स्थिर bool
read_rules_file(संरचना xkb_context *ctx,
                संरचना matcher *matcher,
                unsigned पूर्णाङ्क include_depth,
                FILE *file,
                ध्रुव वर्ण *path);

स्थिर शून्य
matcher_include(संरचना matcher *m, संरचना scanner *parent_scanner,
                unsigned पूर्णाङ्क include_depth,
                संरचना sval inc)
{
    सचे (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        पटिनिवत्त;
    }

    ध्रुव वर्ण *stmt_file = inc.start;
    size_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    वर्ण buf[PATH_MAX] = {0};
    ध्रुव ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, परिमाण(buf));
    सचे (expanded < 0) {
        /* Error */
        पटिनिवत्त;
    } अञ्ञथा सचे (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (size_t) expanded;
        प्रतिज्ञा(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned पूर्णाङ्क offset = 0;
    ध्रुव bool absolute_path = is_absolute_path(stmt_file);
    सचे (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        सचे (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            सचे (stmt_file_len < परिमाण(buf)) {
                memcpy(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } अञ्ञथा {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, परिमाण(buf),
                        (unsigned पूर्णाङ्क) stmt_file_len, stmt_file);
                पटिनिवत्त;
            }
        } अञ्ञथा {
            /* %-expansion is always NULL-terminated */
            प्रतिज्ञा(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } अञ्ञथा {
        /* Relative path: lookup the first XKB path */
        सचे (unlikely(expanded)) {
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
        } अञ्ञथा {
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, परिमाण(buf), &offset, true);
        }
    }

    याव (file) {
        प्रतिज्ञा(strlen_safe(buf) < परिमाण(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        fclose(file);
        सचे (ret)
            पटिनिवत्त;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        सचे (absolute_path) {
            /* There is no point to search further if the path is absolute */
            भङ्ग;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, परिमाण(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (unsigned पूर्णाङ्क) stmt_file_len, stmt_file);
}

स्थिर शून्य
matcher_mapping_start_new(संरचना matcher *m)
{
    अत्थाय (mlvo_index_t i = 0; i < (mlvo_index_t) MLVO_अङ्कः_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_अङ्कः_ENTRIES;
    अत्थाय (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_अङ्कः_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_अङ्कः_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.अङ्कः_mlvo = m->mapping.अङ्कः_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
स्थिर पूर्णाङ्क
parse_layout_int_index_tail(ध्रुव वर्ण *s, size_t max_len,
                            xkb_layout_index_t *out)
{
    सचे (max_len < 3)
        पटिनिवत्त -1;
    uint32_t val = 0;
    ध्रुव पूर्णाङ्क गणनम् = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    सचे (गणनम् <= 0 || s[1 + गणनम्] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        पटिनिवत्त -1;
    /* To zero-based index. */
    *out = val - 1;
    पटिनिवत्त गणनम् + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
स्थिर पूर्णाङ्क
extract_layout_index(ध्रुव वर्ण *s, size_t max_len, xkb_layout_index_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    सचे (max_len < 3 || s[0] != '[')
        पटिनिवत्त -1;
    सचे (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_index_t)(s[1] - '1');
        पटिनिवत्त 3;
    }
    सचे (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        पटिनिवत्त 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    पटिनिवत्त parse_layout_int_index_tail(s, max_len, out);
}

/* Special layout indices */
गणना layout_index_ranges {
    LAYOUT_INDEX_SINGLE = XKB_LAYOUT_INVALID - 5,
    LAYOUT_INDEX_FIRST,
    LAYOUT_INDEX_LATER,
    LAYOUT_INDEX_MULTIPLE,
    LAYOUT_INDEX_ANY,
};

static_assert((xkb_layout_index_t) XKB_MAX_GROUPS <
              (xkb_layout_index_t) LAYOUT_INDEX_SINGLE,
              "Cannot define special indices");
static_assert((xkb_layout_index_t) LAYOUT_INDEX_SINGLE <
              (xkb_layout_index_t) LAYOUT_INDEX_FIRST &&
              (xkb_layout_index_t) LAYOUT_INDEX_FIRST <
              (xkb_layout_index_t) LAYOUT_INDEX_LATER &&
              (xkb_layout_index_t) LAYOUT_INDEX_LATER <
              (xkb_layout_index_t) LAYOUT_INDEX_MULTIPLE &&
              (xkb_layout_index_t) LAYOUT_INDEX_MULTIPLE <
              (xkb_layout_index_t) LAYOUT_INDEX_ANY &&
              (xkb_layout_index_t) LAYOUT_INDEX_ANY <
              (xkb_layout_index_t) XKB_LAYOUT_INVALID,
              "Special indices must respect certain order");

/* Parse index of layout/variant in MLVO mapping */
स्थिर bool
extract_mapping_layout_index(ध्रुव वर्ण *s, size_t max_len,
                             xkb_layout_index_t *out)
{
    /* Check for minimal `[` + index + `]` */
    सचे (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        पटिनिवत्त false;
    }

    /* Small numeric index */
    सचे (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_index_t)(s[1] - '1');
        पटिनिवत्त true;
    }

    /* Named indices ranges */
    स्थिर ध्रुव संरचना {
        ध्रुव वर्ण* name;
        size_t size;
        xkb_layout_index_t मानम्;
    } names[] = {
        #define INDEX_RANGE(tail, मानम्) \
            { (tail), परिमाण(tail), (xkb_layout_index_t)(मानम्) }
        INDEX_RANGE("single]",   LAYOUT_INDEX_SINGLE),
        INDEX_RANGE("first]",    LAYOUT_INDEX_FIRST),
        INDEX_RANGE("later]",    LAYOUT_INDEX_LATER),
        INDEX_RANGE("multiple]", LAYOUT_INDEX_MULTIPLE),
        INDEX_RANGE("any]",      LAYOUT_INDEX_ANY),
        #undef INDEX_RANGE
    };

    अत्थाय (size_t k = 0; k < ARRAY_SIZE(names); k++) {
        सचे (max_len == names[k].size &&
            memcmp(&s[1], names[k].name, names[k].size - 1) == 0) {
            *out = names[k].मानम्;
            पटिनिवत्त true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    ध्रुव पूर्णाङ्क गणनम् = parse_layout_int_index_tail(s, max_len, out);
    पटिनिवत्त गणनम् > 0 && (size_t)गणनम् == max_len;
}

स्थिर अन्तःस्थ bool
is_mlvo_mask_defined(संरचना matcher *m, गणना rules_mlvo mlvo)
{
    पटिनिवत्त m->mapping.defined_mlvo_mask & (1u << mlvo);
}

स्थिर शून्य
matcher_mapping_set_mlvo(संरचना matcher *m, संरचना scanner *s,
                         संरचना sval ident)
{
    गणना rules_mlvo mlvo;
    संरचना sval mlvo_sval;

    अत्थाय (mlvo = 0; mlvo < MLVO_अङ्कः_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        सचे (svaleq_prefix(mlvo_sval, ident))
            भङ्ग;
    }

    /* Not found. */
    सचे (mlvo >= MLVO_अङ्कः_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned पूर्णाङ्क) ident.len, ident.start);
        m->mapping.active = false;
        पटिनिवत्त;
    }

    सचे (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned पूर्णाङ्क) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        पटिनिवत्त;
    }

    /* If there are leftovers still, it must be an index. */
    सचे (mlvo_sval.len < ident.len) {
        xkb_layout_index_t idx;
        सचे (!extract_mapping_layout_index(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (unsigned पूर्णाङ्क) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            पटिनिवत्त;
        }

        सचे (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        अञ्ञथा सचे (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        अञ्ञथा {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (unsigned पूर्णाङ्क) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            पटिनिवत्त;
        }
    } अञ्ञथा सचे (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    } अञ्ञथा सचे (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    सचे (((mlvo == MLVO_LAYOUT && is_mlvo_mask_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_mask_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.active = false;
        पटिनिवत्त;
    }

    m->mapping.mlvo_at_pos[m->mapping.अङ्कः_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.अङ्कः_mlvo++;
}

स्थिर शून्य
matcher_mapping_set_layout_bounds(संरचना matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_index_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    विकल्प (idx) {
        पक्ष XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            प्रतिज्ञा(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            भङ्ग;
        पक्ष LAYOUT_INDEX_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            भङ्ग;
        पक्ष LAYOUT_INDEX_MULTIPLE:
        पक्ष LAYOUT_INDEX_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            भङ्ग;
        पक्ष LAYOUT_INDEX_SINGLE:
        पक्ष LAYOUT_INDEX_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        स्वभाव:
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

स्थिर शून्य
matcher_mapping_set_kccgst(संरचना matcher *m, संरचना scanner *s, संरचना sval ident)
{
    गणना rules_kccgst kccgst;
    संरचना sval kccgst_sval;

    अत्थाय (kccgst = 0; kccgst < KCCGST_अङ्कः_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        सचे (svaleq(rules_kccgst_svals[kccgst], ident))
            भङ्ग;
    }

    /* Not found. */
    सचे (kccgst >= KCCGST_अङ्कः_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned पूर्णाङ्क) ident.len, ident.start);
        m->mapping.active = false;
        पटिनिवत्त;
    }

    सचे (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned पूर्णाङ्क) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        पटिनिवत्त;
    }

    m->mapping.kccgst_at_pos[m->mapping.अङ्कः_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.अङ्कः_kccgst++;
}

स्थिर bool
matcher_mapping_verify(संरचना matcher *m, संरचना scanner *s)
{
    सचे (m->mapping.अङ्कः_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        गच्छ skip;
    }

    सचे (m->mapping.अङ्कः_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        गच्छ skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    सचे (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        प्रतिज्ञा(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        विकल्प (m->mapping.layout_idx) {
            पक्ष LAYOUT_INDEX_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                सचे (darray_size(m->rmlvo.layouts) > 1)
                    गच्छ skip;
                भङ्ग;
            पक्ष LAYOUT_INDEX_MULTIPLE:
            पक्ष LAYOUT_INDEX_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                सचे (darray_size(m->rmlvo.layouts) < 2)
                    गच्छ skip;
                भङ्ग;
            पक्ष LAYOUT_INDEX_ANY:
            पक्ष LAYOUT_INDEX_FIRST:
                /* No restrictions */
                भङ्ग;
            स्वभाव:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                सचे (darray_size(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_size(m->rmlvo.layouts))
                    गच्छ skip;
        }
    }

    सचे (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        प्रतिज्ञा(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        विकल्प (m->mapping.variant_idx) {
            पक्ष LAYOUT_INDEX_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                सचे (darray_size(m->rmlvo.variants) > 1)
                    गच्छ skip;
                भङ्ग;
            पक्ष LAYOUT_INDEX_ANY:
            पक्ष LAYOUT_INDEX_LATER:
            पक्ष LAYOUT_INDEX_FIRST:
                /* No restriction */
                भङ्ग;
            स्वभाव:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                सचे (darray_size(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_size(m->rmlvo.variants))
                    गच्छ skip;
        }
    }

    पटिनिवत्त true;

skip:
    m->mapping.active = false;
    पटिनिवत्त false;
}

स्थिर शून्य
matcher_rule_start_new(संरचना matcher *m)
{
    memset(&m->rule, 0, परिमाण(m->rule));
    m->rule.skip = !m->mapping.active;
}

स्थिर शून्य
matcher_rule_set_mlvo_common(संरचना matcher *m, संरचना scanner *s,
                             संरचना sval ident,
                             गणना mlvo_match_type match_type)
{
    सचे (m->rule.अङ्कः_mlvo_मानानि >= m->mapping.अङ्कः_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        पटिनिवत्त;
    }
    m->rule.match_type_at_pos[m->rule.अङ्कः_mlvo_मानानि] = match_type;
    m->rule.mlvo_मानम्_at_pos[m->rule.अङ्कः_mlvo_मानानि] = ident;
    m->rule.अङ्कः_mlvo_मानानि++;
}

स्थिर शून्य
matcher_rule_set_mlvo_wildcard(संरचना matcher *m, संरचना scanner *s,
                               गणना mlvo_match_type match_type)
{
    संरचना sval dummy = SVAL(रिक्त, 0);
    matcher_rule_set_mlvo_common(m, s, dummy, match_type);
}

स्थिर शून्य
matcher_rule_set_mlvo_group(संरचना matcher *m, संरचना scanner *s,
                            संरचना sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_GROUP);
}

स्थिर शून्य
matcher_rule_set_mlvo(संरचना matcher *m, संरचना scanner *s,
                      संरचना sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_NORMAL);
}

स्थिर शून्य
matcher_rule_set_kccgst(संरचना matcher *m, संरचना scanner *s,
                        संरचना sval ident)
{
    सचे (m->rule.अङ्कः_kccgst_मानानि >= m->mapping.अङ्कः_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        पटिनिवत्त;
    }
    m->rule.kccgst_मानम्_at_pos[m->rule.अङ्कः_kccgst_मानानि] = ident;
    m->rule.अङ्कः_kccgst_मानानि++;
}

स्थिर bool
match_group(संरचना matcher *m, संरचना sval group_name, संरचना sval to)
{
    संरचना group *group = रिक्त;
    संरचना sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        सचे (svaleq(group->name, group_name)) {
            found = true;
            भङ्ग;
        }
    }

    सचे (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        पटिनिवत्त false;
    }

    darray_foreach(element, group->elements)
        सचे (svaleq(to, *element))
            पटिनिवत्त true;

    पटिनिवत्त false;
}

स्थिर bool
match_मानम्(संरचना matcher *m, ध्रुव संरचना sval val, ध्रुव संरचना sval to,
            गणना mlvo_match_type match_type,
            गणना wildcard_match_type wildcard_type)
{
    विकल्प (match_type) {
        पक्ष MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            पटिनिवत्त wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        पक्ष MLVO_MATCH_WILDCARD_NONE:
            पटिनिवत्त !to.len;
        पक्ष MLVO_MATCH_WILDCARD_SOME:
            पटिनिवत्त !!to.len;
        पक्ष MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            पटिनिवत्त true;
        पक्ष MLVO_MATCH_GROUP:
            पटिनिवत्त match_group(m, val, to);
        स्वभाव:
            प्रतिज्ञा(match_type == MLVO_MATCH_NORMAL);
            पटिनिवत्त svaleq(val, to);
    }
}

स्थिर bool
match_मानम्_and_mark(संरचना matcher *m, ध्रुव संरचना sval val,
                     संरचना matched_sval *to, गणना mlvo_match_type match_type,
                     गणना wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_मानम्(m, val, to->sval, match_type, wildcard_type);
    सचे (matched)
        to->matched |= layouts;
    पटिनिवत्त matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
स्थिर bool
expand_rmlvo_in_kccgst_मानम्(संरचना matcher *m, संरचना scanner *s,
                             संरचना sval मानम्, xkb_layout_index_t layout_idx,
                             darray_char *expanded, size_t *i)
{
    ध्रुव वर्ण *str = मानम्.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    गणना rules_mlvo mlv;
    xkb_layout_index_t idx;
    वर्ण pfx, sfx;
    संरचना matched_sval *expanded_मानम्;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    सचे (str[*i] == 'i' &&
        (*i + 1 == मानम्.len || is_merge_mode_prefix(str[*i + 1])))
    {
        सचे (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_INDEX_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            गच्छ error;
        }
        (*i)++;
        वर्ण index_str[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
        ध्रुव पूर्णाङ्क गणनम् = snprintf(index_str, परिमाण(index_str), "%"PRIu32,
                                   layout_idx + 1);
        प्रतिज्ञा(गणनम् > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, index_str, (darray_size_t)गणनम्);
        पटिनिवत्त true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    सचे (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        सचे (str[*i] == '(') sfx = ')';
        सचे (++(*i) >= मानम्.len) गच्छ error;
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
    bool variable_index = false;
    सचे (*i < मानम्.len && str[*i] == '[') {
        सचे (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            गच्छ error;
        }

        पूर्णाङ्क consumed = extract_layout_index(str + (*i), मानम्.len - (*i), &idx);
        सचे (consumed <= 0) गच्छ error;
        सचे (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            प्रतिज्ञा(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_index = true;
        }
        *i += (size_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    सचे (sfx != 0) {
        सचे (*i >= मानम्.len) गच्छ error;
        सचे (str[(*i)++] != sfx) गच्छ error;
    }

    /* Get the expanded value. */
    expanded_मानम् = रिक्त;

    सचे (mlv == MLVO_LAYOUT) {
        सचे (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            सचे (darray_size(m->rmlvo.layouts) == 1)
                expanded_मानम् = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } अञ्ञथा सचे (idx < darray_size(m->rmlvo.layouts) &&
                   (variable_index || darray_size(m->rmlvo.layouts) > 1)) {
                expanded_मानम् = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    अञ्ञथा सचे (mlv == MLVO_VARIANT) {
        सचे (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            सचे (darray_size(m->rmlvo.variants) == 1)
                expanded_मानम् = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } अञ्ञथा सचे (idx < darray_size(m->rmlvo.variants) &&
                   (variable_index || darray_size(m->rmlvo.variants) > 1)) {
                expanded_मानम् = &darray_item(m->rmlvo.variants, idx);
        }
    }
    अञ्ञथा सचे (mlv == MLVO_MODEL) {
        expanded_मानम् = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    सचे (!expanded_मानम् || expanded_मानम्->sval.len == 0) {
        पटिनिवत्त true;
    }

    सचे (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_मानम्->sval.start,
                                 (darray_size_t) expanded_मानम्->sval.len);
    सचे (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    सचे (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ध्रुव xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_मानम्->matched |= layouts;
    } अञ्ञथा {
        expanded_मानम्->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    पटिनिवत्त true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    पटिनिवत्त false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
स्थिर शून्य
expand_qualifier_in_kccgst_मानम्(
    संरचना matcher *m, संरचना scanner *s,
    संरचना sval मानम्, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_size_t prefix_idx, size_t *i)
{
    ध्रुव वर्ण *str = मानम्.start;

    /* “all” followed by nothing or by a layout separator */
    सचे (*i + 2 < मानम्.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == मानम्.len || is_merge_mode_prefix(str[*i + 3]))) {
        सचे (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_MESSAGE_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        सचे (darray_size(m->rmlvo.layouts) > 1) {
            वर्ण layout_index[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
            ध्रुव darray_size_t prefix_length =
                darray_size(*expanded) - prefix_idx - 1;
            अत्थाय (xkb_layout_index_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_size(m->rmlvo.layouts));
                 l++)
            {
                सचे (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_length);
                /* Append index */
                ध्रुव पूर्णाङ्क गणनम् = snprintf(layout_index, परिमाण(layout_index),
                                           "%"PRIu32, l + 1);
                प्रतिज्ञा(गणनम् > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_index,
                                             (darray_size_t)गणनम्);
            }
        }
        *i += 3;
    }
}

स्थिर अन्तःस्थ शून्य
#ifdef _MSC_VER
concat_kccgst(darray_char *into, darray_size_t size, _In_reads_(size) ध्रुव वर्ण* from)
#अञ्ञथा
concat_kccgst(darray_char *into, darray_size_t size, ध्रुव वर्ण from[स्थिर size])
#endif
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ध्रुव bool from_plus = is_merge_mode_prefix(from[0]);
    सचे (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, size);
    } अञ्ञथा {
        ध्रुव वर्ण ch =
            (वर्ण) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        ध्रुव bool into_plus = is_merge_mode_prefix(ch);
        सचे (into_plus)
            darray_prepends_nullterminate(*into, from, size);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
स्थिर bool
append_expanded_kccgst_मानम्(संरचना matcher *m, संरचना scanner *s,
                             bool merge, darray_char *to, संरचना sval मानम्,
                             xkb_layout_index_t layout_idx)
{
    ध्रुव वर्ण *str = मानम्.start;
    darray_char expanded = darray_new();
    darray_size_t last_item_idx = 0;
    bool has_separator = false;

    अत्थाय (size_t i = 0; i < मानम्.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        विकल्प (str[i]) {
            /* Qualifier */
            पक्ष ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_मानम्(m, s, मानम्, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                भङ्ग;
            /* Expansion */
            पक्ष '%':
                i++;
                सचे (i >= मानम्.len ||
                    !expand_rmlvo_in_kccgst_मानम्(m, s, मानम्, layout_idx,
                                                  &expanded, &i))
                        गच्छ error;
                भङ्ग;
            /* New item */
            पक्ष MERGE_OVERRIDE_PREFIX:
            पक्ष MERGE_AUGMENT_PREFIX:
            पक्ष MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_size(expanded) - 1;
                has_separator = true;
                भङ्ग;
            /* Just a normal character. */
            स्वभाव:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    सचे (merge) {
        सचे (!darray_empty(expanded))
            concat_kccgst(to, darray_size(expanded), darray_वस्तूनि(expanded));
    } अञ्ञथा {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    पटिनिवत्त true;

error:
    darray_free(expanded);
    पटिनिवत्त false;
}

स्थिर bool
matcher_append_pending_kccgst(संरचना matcher *m)
{
    सचे (!m->mapping.has_multiple_layouts)
        पटिनिवत्त true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    अत्थाय (kccgst_index_t i = 0; i < m->mapping.अङ्कः_kccgst; i++) {
        ध्रुव गणना rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        अत्थाय (xkb_layout_index_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            पञ्जिका ध्रुव संरचना kccgst_buffer* ध्रुव buf = &m->pending_kccgst;
            size_t offset = 0;
            अत्थाय (darray_size_t k = 0; k < darray_size(buf->slices); k++) {
                पञ्जिका ध्रुव संरचना kccgst_buffer_slice * ध्रुव slice =
                    &darray_item(buf->slices, k);
                सचे (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->length)
                    concat_kccgst(&m->kccgst[kccgst], slice->length,
                                  darray_वस्तूनि(buf->buffer) + offset);
                offset += slice->length;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    पटिनिवत्त true;
}

स्थिर शून्य
matcher_rule_verify(संरचना matcher *m, संरचना scanner *s)
{
    सचे (m->rule.अङ्कः_mlvo_मानानि != m->mapping.अङ्कः_mlvo ||
        m->rule.अङ्कः_kccgst_मानानि != m->mapping.अङ्कः_kccgst) {
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
स्थिर bool
matcher_rule_match_model(संरचना matcher *m, ध्रुव संरचना sval मानम्,
                         गणना mlvo_match_type match_type)
{
    पटिनिवत्त match_मानम्_and_mark(
        m, मानम्, &m->rmlvo.model, match_type, WILDCARD_MATCH_ALL,
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
स्थिर bool
matcher_rule_match_option(संरचना matcher *m, ध्रुव संरचना sval मानम्,
                          गणना mlvo_match_type match_type,
                          xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    प्रतिज्ञा(!darray_empty(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_mask_t unmatched =
        (m->mapping.defined_mlvo_mask & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    संरचना matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        ध्रुव xkb_layout_mask_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
        सचे (matchable && match_मानम्_and_mark(m, मानम्, option, match_type,
                                              WILDCARD_MATCH_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            सचे (!unmatched)
                भङ्ग;
        }
    }
    सचे (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    पटिनिवत्त matched;
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
matcher_rule_match_layout_or_variant(संरचना matcher *m,
                                     darray_matched_sval *निवेशः,
                                     ध्रुव संरचना sval मानम्,
                                     गणना mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    प्रतिज्ञा(!darray_empty(*निवेशः));

    bool matched = false;
    /* Loop over the layout index range */
    अत्थाय (xkb_layout_index_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ध्रुव xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        सचे (layout & *candidates) {
            संरचना matched_sval *to = &darray_item(*निवेशः, idx);
            सचे (match_मानम्_and_mark(m, मानम्, to, match_type,
                                     WILDCARD_MATCH_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } अञ्ञथा {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    पटिनिवत्त matched;
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
matcher_rule_apply_if_matches(संरचना matcher *m, संरचना scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_mask_t candidate_layouts = m->mapping.layouts_candidates_mask;

    /* Loop over MLVO pattern components */
    अत्थाय (mlvo_index_t i = 0; i < m->mapping.अङ्कः_mlvo; i++) {
        ध्रुव गणना rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ध्रुव संरचना sval मानम् = m->rule.mlvo_मानम्_at_pos[i];
        ध्रुव गणना mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        विकल्प (mlvo) {
        पक्ष MLVO_MODEL:
            matched = matcher_rule_match_model(m, मानम्, match_type);
            भङ्ग;
        पक्ष MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, मानम्, match_type, &candidate_layouts
            );
            भङ्ग;
        पक्ष MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, मानम्, match_type, &candidate_layouts
            );
            भङ्ग;
        पक्ष MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, मानम्, match_type, &candidate_layouts
            );
            भङ्ग;
        स्वभाव: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_अङ्कः_ENTRIES - 1,
                          "Unexpected MLVO field");
            प्रतिज्ञा(!"Unreachable");
        }
        }

        सचे (!matched)
            पटिनिवत्त;
    }

    सचे (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        अत्थाय (xkb_layout_index_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            सचे (candidate_layouts & (UINT32_C(1) << idx)) {
                अत्थाय (kccgst_index_t i = 0; i < m->mapping.अङ्कः_kccgst; i++) {
                    ध्रुव गणना rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ध्रुव संरचना sval मानम् = m->rule.kccgst_मानम्_at_pos[i];
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
                    संरचना kccgst_buffer * ध्रुव buf = &m->pending_kccgst;
                    ध्रुव darray_size_t prev_buffer_length =
                        darray_size(buf->buffer);
                    append_expanded_kccgst_मानम्(m, s, false, &buf->buffer,
                                                 मानम्, idx);
                    ध्रुव uint32_t length = (uint32_t) (darray_size(buf->buffer)
                                          - prev_buffer_length);
                    ध्रुव संरचना kccgst_buffer_slice slice = {
                        .length = length,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } अञ्ञथा {
        /* Numeric index or no index */
        अत्थाय (kccgst_index_t i = 0; i < m->mapping.अङ्कः_kccgst; i++) {
            गणना rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            संरचना sval मानम् = m->rule.kccgst_मानम्_at_pos[i];
            append_expanded_kccgst_मानम्(m, s, true, &m->kccgst[kccgst], मानम्,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    सचे (!(is_mlvo_mask_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_mask &= ~candidate_layouts;
    }
}

स्थिर गणना rules_token
gettok(संरचना matcher *m, संरचना scanner *s)
{
    पटिनिवत्त lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
स्थिर bool
matcher_match(संरचना matcher *m, संरचना scanner *s,
              unsigned पूर्णाङ्क include_depth,
              ध्रुव वर्ण *string, size_t len,
              ध्रुव वर्ण *file_name)
{
    गणना rules_token tok;

    सचे (!m)
        पटिनिवत्त false;

initial:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_BANG:
        गच्छ bang;
    पक्ष TOK_END_OF_पङ्क्तिः:
        गच्छ initial;
    पक्ष TOK_END_OF_FILE:
        गच्छ finish;
    स्वभाव:
        गच्छ unexpected;
    }

bang:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        गच्छ group_name;
    पक्ष TOK_INCLUDE:
        गच्छ include_statement;
    पक्ष TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        गच्छ mapping_mlvo;
    स्वभाव:
        गच्छ unexpected;
    }

group_name:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_EQUALS:
        गच्छ group_element;
    स्वभाव:
        गच्छ unexpected;
    }

group_element:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        गच्छ group_element;
    पक्ष TOK_END_OF_पङ्क्तिः:
        गच्छ initial;
    स्वभाव:
        गच्छ unexpected;
    }

include_statement:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        गच्छ include_statement_end;
    स्वभाव:
        गच्छ unexpected;
    }

include_statement_end:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_END_OF_पङ्क्तिः:
        गच्छ initial;
    स्वभाव:
        गच्छ unexpected;
    }

mapping_mlvo:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_IDENTIFIER:
        सचे (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        गच्छ mapping_mlvo;
    पक्ष TOK_EQUALS:
        गच्छ mapping_kccgst;
    स्वभाव:
        गच्छ unexpected;
    }

mapping_kccgst:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_IDENTIFIER:
        सचे (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        गच्छ mapping_kccgst;
    पक्ष TOK_END_OF_पङ्क्तिः:
        सचे (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            सचे (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_size(m->pending_kccgst.buffer) = 0;
                darray_size(m->pending_kccgst.slices) = 0;
            }
        }
        गच्छ rule_mlvo_first;
    स्वभाव:
        गच्छ unexpected;
    }

rule_mlvo_first:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_BANG:
        matcher_append_pending_kccgst(m);
        गच्छ bang;
    पक्ष TOK_END_OF_पङ्क्तिः:
        गच्छ rule_mlvo_first;
    पक्ष TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        गच्छ finish;
    स्वभाव:
        matcher_rule_start_new(m);
        गच्छ rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    विकल्प (tok) {
    पक्ष TOK_IDENTIFIER:
        सचे (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        गच्छ rule_mlvo;
    पक्ष TOK_WILD_CARD_STAR:
        सचे (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        गच्छ rule_mlvo;
    पक्ष TOK_WILD_CARD_NONE:
        सचे (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        गच्छ rule_mlvo;
    पक्ष TOK_WILD_CARD_SOME:
        सचे (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        गच्छ rule_mlvo;
    पक्ष TOK_WILD_CARD_ANY:
        सचे (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        गच्छ rule_mlvo;
    पक्ष TOK_GROUP_NAME:
        सचे (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        गच्छ rule_mlvo;
    पक्ष TOK_EQUALS:
        गच्छ rule_kccgst;
    स्वभाव:
        गच्छ unexpected;
    }

rule_kccgst:
    विकल्प (tok = gettok(m, s)) {
    पक्ष TOK_IDENTIFIER:
        सचे (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        गच्छ rule_kccgst;
    पक्ष TOK_END_OF_पङ्क्तिः:
        सचे (!m->rule.skip)
            matcher_rule_verify(m, s);
        सचे (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        गच्छ rule_mlvo_first;
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
    पटिनिवत्त true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    पटिनिवत्त false;
}
// NOLINTEND(readability-trivial-switch)

स्थिर bool
read_rules_file(संरचना xkb_context *ctx,
                संरचना matcher *matcher,
                unsigned पूर्णाङ्क include_depth,
                FILE *file,
                ध्रुव वर्ण *path)
{
    bool ret;
    वर्ण *string;
    size_t size;
    संरचना scanner scanner;

    सचे (!map_file(file, &string, &size)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, strerror(errno));
        पटिनिवत्त false;
    }

    scanner_init(&scanner, matcher->ctx, string, size, path, रिक्त);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    सचे (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, size);
        पटिनिवत्त false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, size, path);
    unmap_file(string, size);
    पटिनिवत्त ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
स्थिर bool
xkb_resolve_partial_rules(संरचना xkb_context *ctx, वर्ण *path, size_t path_size,
                          ध्रुव वर्ण* rules, ध्रुव वर्ण* suffix,
                          संरचना matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    वर्ण partial_rules[60]; /* Arbitrary, but we do not expect long names */
    सचे (unlikely(!snprintf_safe(partial_rules, परिमाण(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        पटिनिवत्त false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    unsigned पूर्णाङ्क offset = 0;
    FILE *file = रिक्त;
    ध्रुव size_t len = strlen(partial_rules);
    याव ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_size, &offset, false)) != रिक्त) {
        ध्रुव bool ok = read_rules_file(ctx, matcher, 0, file, path);
        fclose(file);
        सचे (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            पटिनिवत्त false;
        }
        offset++;
    }

    पटिनिवत्त true;
}

स्थिर bool
xkb_resolve_rules(संरचना xkb_context *ctx,
                  ध्रुव वर्ण* rules, संरचना matcher *matcher,
                  संरचना xkb_component_names *out,
                  xkb_layout_index_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    unsigned पूर्णाङ्क offset = 0;
    वर्ण path[PATH_MAX];
    FILE * ध्रुव file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, strlen(rules), FILE_TYPE_RULES,
                                          path, परिमाण(path), &offset, true);
    सचे (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        गच्छ err_out;
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
    ret = xkb_resolve_partial_rules(ctx, path, परिमाण(path),
                                    rules, ".pre", matcher);
    सचे (!ret)
        गच्छ err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    सचे (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        गच्छ err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, परिमाण(path),
                                    rules, ".post", matcher);
    सचे (!ret)
        गच्छ err_out;

    /* Check we got required components */
    सचे (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        गच्छ err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, रिक्त);

    संरचना matched_sval *mval = &matcher->rmlvo.model;
    सचे (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मानम्,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (unsigned पूर्णाङ्क) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        सचे (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मानम्,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned पूर्णाङ्क) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        सचे (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मानम्,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned पूर्णाङ्क) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        सचे (!mval->sval.len)
            अनुवर्त;

        सचे (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मानम्,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned पूर्णाङ्क) mval->sval.len, mval->sval.start, rules);
        } अञ्ञथा सचे (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ध्रुव xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            प्रतिज्ञा(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मानम्,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (unsigned पूर्णाङ्क)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    सचे (out->symbols != रिक्त && explicit_layouts != रिक्त) {
        *explicit_layouts = 1; /* at least one group */
        ध्रुव वर्ण *symbols = out->symbols;
        /* Take the highest modifier */
        याव ((symbols = strchr(symbols, ':')) != रिक्त && symbols[1] != '\0') {
            xkb_layout_index_t group = 0;
            ध्रुव पूर्णाङ्क गणनम् = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            सचे (गणनम् > 0 && (symbols[गणनम्] == '\0' ||
                is_merge_mode_prefix(symbols[गणनम्])) &&
                group > 0 && group <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, group);
                symbols += गणनम्;
            }
        }
    }

err_out:
    सचे (file)
        fclose(file);

    पटिनिवत्त ret;
}

bool
xkb_components_from_rmlvo_builder(ध्रुव संरचना xkb_rmlvo_builder *rmlvo,
                                  संरचना xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts)
{
    ध्रुव वर्ण *rules = rmlvo->rules;
    संरचना matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    सचे (!matcher)
        पटिनिवत्त false;

    ध्रुव bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    पटिनिवत्त ret;
}


bool
xkb_components_from_rules_names(संरचना xkb_context *ctx,
                                ध्रुव संरचना xkb_rule_names *rmlvo,
                                संरचना xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts)
{
    संरचना matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    सचे (!matcher)
        पटिनिवत्त false;

    ध्रुव bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    पटिनिवत्त ret;
}
