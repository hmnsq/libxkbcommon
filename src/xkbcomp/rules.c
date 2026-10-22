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
संघ lvalue {
    संरचना sval string;
};

गणना rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_ओळ,
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
lex(संरचना scanner *s, संघ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    पर्यंत (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    जर (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    जर (scanner_eol(s)) {
        पर्यंत (scanner_eol(s)) scanner_next(s);
        परत TOK_END_OF_ओळ;
    }

    /* Escaped line continuation */
    जर (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        जर (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            परत TOK_ERROR;
        }
        scanner_next(s);
        जा skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    जर (scanner_eof(s)) परत TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    गणना rules_token token = TOK_IDENTIFIER;

    स्थिरांक अक्षर next = scanner_next(s);
    निवड (next) {
    स्थिती '!':
        परत TOK_BANG;
    स्थिती '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        थांबा;
    स्थिती '*':
        जर (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            थांबा;
        } अन्यथा {
            /* Legacy wild card */
            परत TOK_WILD_CARD_STAR;
        }
    स्थिती '<':
        /* Extended wild cards */
        जर (scanner_rules_lit_token(s, "none>")) परत TOK_WILD_CARD_NONE;
        जर (scanner_rules_lit_token(s, "some>")) परत TOK_WILD_CARD_SOME;
        जर (scanner_rules_lit_token(s, "any>")) परत TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        प्रतिज्ञा(scanner_rules_is_ident('<'));
        val->string.len = 1;
        थांबा;
    स्थिती '=':
        जर (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            थांबा;
        } अन्यथा {
            परत TOK_EQUALS;
        }
    स्थिती 'i':
        जर (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            परत TOK_INCLUDE;
        }
        /* Identifier */
        प्रतिज्ञा(scanner_rules_is_ident('i'));
        val->string.len = 1;
        थांबा;
    पूर्वनिर्धारित:
        जर (likely(scanner_rules_is_ident(next))) {
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
    प्रतिज्ञा(scanner_rules_is_ident(MERGE_OVERRIDE_PREFIX));
    प्रतिज्ञा(scanner_rules_is_ident(MERGE_AUGMENT_PREFIX));
    प्रतिज्ञा(scanner_rules_is_ident(MERGE_REPLACE_PREFIX));
    प्रतिज्ञा(scanner_rules_is_ident('('));
    प्रतिज्ञा(scanner_rules_is_ident(')'));
    val->string.len += scanner_rules_ident_token(s);

    जर (unlikely(val->string.len == 0)) {
        जर (token == TOK_GROUP_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } अन्यथा {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unrecognized token");
        }
        परत TOK_ERROR;
    }

    परत token;
}

/***====================================================================***/

गणना rules_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_अंक_ENTRIES
};

गणना {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

प्रकारव्याख्या uint8_t mlvo_index_t;
प्रकारव्याख्या uint8_t mlvo_mask_t;


स्थिर स्थिरांक संरचना sval rules_mlvo_svals[MLVO_अंक_ENTRIES] = {
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
    KCCGST_अंक_ENTRIES
};

प्रकारव्याख्या uint8_t kccgst_index_t;
प्रकारव्याख्या uint8_t kccgst_mask_t;

स्थिर स्थिरांक संरचना sval rules_kccgst_svals[KCCGST_अंक_ENTRIES] = {
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
प्रकारव्याख्या darray(संरचना matched_sval) darray_matched_sval;

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
    गणना rules_mlvo mlvo_at_pos[MLVO_अंक_ENTRIES];
    mlvo_index_t अंक_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    संघ {
        संरचना { xkb_layout_index_t layout_idx, variant_idx; };
        संरचना { xkb_layout_index_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    संघ {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    गणना rules_kccgst kccgst_at_pos[KCCGST_अंक_ENTRIES];
    kccgst_index_t अंक_kccgst;
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
    संरचना sval mlvo_मूल्य_at_pos[MLVO_अंक_ENTRIES];
    गणना mlvo_match_type match_type_at_pos[MLVO_अंक_ENTRIES];
    संरचना sval kccgst_मूल्य_at_pos[KCCGST_अंक_ENTRIES];
    mlvo_index_t अंक_mlvo_मूल्ये;
    kccgst_index_t अंक_kccgst_मूल्ये;
    bool skip;
};

#define SLICE_KCCGST_BIT_FIELD_SIZE 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_SIZE - 1)) &
                (KCCGST_अंक_ENTRIES - 1)),
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
    संघ lvalue val;
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
    darray_char kccgst[KCCGST_अंक_ENTRIES];
};

स्थिर संरचना sval
strip_spaces(संरचना sval v)
{
    पर्यंत (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    पर्यंत (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    परत v;
}

स्थिर darray_matched_sval
split_comma_separated_mlvo(संरचना xkb_context *ctx,
                           गणना rules_mlvo mlvo, स्थिरांक अक्षर *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    जर (!s) {
        ensure_at_least_one_मूल्य:
        /* Label followed by a declaration is a C23 extension */ ;

        संरचना matched_sval val = {
            .sval = SVAL(रिक्त, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        परत arr;
    }

    पर्यंत (true) {
        संरचना matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        पर्यंत (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        जर (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            स्थिरांक अक्षर* स्थिरांक layout_start = s;
            xkb_layout_index_t layout = XKB_LAYOUT_INVALID;

            पूर्णांक मोजणी = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            जर (मोजणी > 0) {
                /* Note: 1-indexed layout */
                s += मोजणी;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                जर (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (अचिन्हित पूर्णांक) val.sval.len, val.sval.start);
                } अन्यथा जर (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (अचिन्हित पूर्णांक) val.sval.len, val.sval.start);
                } अन्यथा {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            स्थिरांक अक्षर* स्थिरांक layout_index_end = s;
            पर्यंत (*s != '\0' && *s != ',') { s++; }
            जर (मोजणी <= 0 || layout_index_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (अचिन्हित पूर्णांक) (s - layout_start), layout_start,
                        (अचिन्हित पूर्णांक) val.sval.len, val.sval.start);
            }
        }

        जर (val.sval.len || mlvo != MLVO_OPTION) {
            जर (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                संरचना matched_sval *prev;
                darray_foreach(prev, arr) {
                    जर (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        जर (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } अन्यथा जर (prev->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        जा next_मूल्य;
                    }
                }
            }
            darray_append(arr, val);
        }

next_मूल्य:
        जर (*s == '\0') थांबा;
        जर (*s == ',') s++;
    }

    जर (darray_empty(arr))
        जा ensure_at_least_one_मूल्य;

    परत arr;
}

स्थिर संरचना matcher *
matcher_new_from_rmlvo(स्थिरांक संरचना xkb_rmlvo_builder *rmlvo, स्थिरांक अक्षर **rules)
{
    संरचना matcher *m = सुस्मृति(1, आकार(*m));
    जर (!m)
        परत रिक्त;

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
    स्थिरांक गणना RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
                                                               &names);

    /* Initialize matcher with sanitized names, if relevant */
    जर (changed & RMLVO_RULES) {
        *rules = names.rules;
    } अन्यथा {
        *rules = rmlvo->rules;
    }

    जर (changed & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = names.model;
    } अन्यथा {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;

    प्रतिज्ञा((changed & RMLVO_LAYOUT) || !(changed & RMLVO_VARIANT));
    जर (changed & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = split_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      names.layout);
        m->rmlvo.variants = split_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       names.variant);
        जर (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            जर (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        } अन्यथा जर (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } अन्यथा {
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

    जर (changed & RMLVO_OPTIONS) {
        m->rmlvo.options = split_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      names.options);
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

    परत m;
}

स्थिर संरचना matcher *
matcher_new_from_names(संरचना xkb_context *ctx,
            स्थिरांक संरचना xkb_rule_names *rmlvo)
{
    संरचना matcher *m = सुस्मृति(1, आकार(*m));
    जर (!m)
        परत रिक्त;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    जर (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        जर (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
    } अन्यथा जर (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    परत m;
}

स्थिर शून्य
matcher_free(संरचना matcher *m)
{
    जर (!m)
        परत;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    संरचना group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.buffer);
    darray_free(m->pending_kccgst.slices);
    साठी (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_अंक_ENTRIES; i++)
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
                अचिन्हित पूर्णांक include_depth,
                खाता *file,
                स्थिरांक अक्षर *path);

स्थिर शून्य
matcher_include(संरचना matcher *m, संरचना scanner *parent_scanner,
                अचिन्हित पूर्णांक include_depth,
                संरचना sval inc)
{
    जर (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        परत;
    }

    स्थिरांक अक्षर *stmt_file = inc.start;
    माप_प्रकार stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    अक्षर buf[PATH_MAX] = {0};
    स्थिरांक ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, आकार(buf));
    जर (expanded < 0) {
        /* Error */
        परत;
    } अन्यथा जर (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (माप_प्रकार) expanded;
        प्रतिज्ञा(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    खाता* file;
    अचिन्हित पूर्णांक offset = 0;
    स्थिरांक bool absolute_path = is_absolute_path(stmt_file);
    जर (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        जर (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            जर (stmt_file_len < आकार(buf)) {
                स_नकल(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } अन्यथा {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, आकार(buf),
                        (अचिन्हित पूर्णांक) stmt_file_len, stmt_file);
                परत;
            }
        } अन्यथा {
            /* %-expansion is always NULL-terminated */
            प्रतिज्ञा(stmt_file[stmt_file_len] == '\0');
        }
        file = ख_खोलो(stmt_file, "rb");
    } अन्यथा {
        /* Relative path: lookup the first XKB path */
        जर (unlikely(expanded)) {
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
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, आकार(buf), &offset, true);
        }
    }

    पर्यंत (file) {
        प्रतिज्ञा(strlen_safe(buf) < आकार(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        ख_बंद(file);
        जर (ret)
            परत;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        जर (absolute_path) {
            /* There is no point to search further if the path is absolute */
            थांबा;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, आकार(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (अचिन्हित पूर्णांक) stmt_file_len, stmt_file);
}

स्थिर शून्य
matcher_mapping_start_new(संरचना matcher *m)
{
    साठी (mlvo_index_t i = 0; i < (mlvo_index_t) MLVO_अंक_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_अंक_ENTRIES;
    साठी (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_अंक_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_अंक_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.अंक_mlvo = m->mapping.अंक_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
स्थिर पूर्णांक
parse_layout_int_index_tail(स्थिरांक अक्षर *s, माप_प्रकार max_len,
                            xkb_layout_index_t *out)
{
    जर (max_len < 3)
        परत -1;
    uint32_t val = 0;
    स्थिरांक पूर्णांक मोजणी = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    जर (मोजणी <= 0 || s[1 + मोजणी] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        परत -1;
    /* To zero-based index. */
    *out = val - 1;
    परत मोजणी + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
स्थिर पूर्णांक
extract_layout_index(स्थिरांक अक्षर *s, माप_प्रकार max_len, xkb_layout_index_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    जर (max_len < 3 || s[0] != '[')
        परत -1;
    जर (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_index_t)(s[1] - '1');
        परत 3;
    }
    जर (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        परत 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    परत parse_layout_int_index_tail(s, max_len, out);
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
extract_mapping_layout_index(स्थिरांक अक्षर *s, माप_प्रकार max_len,
                             xkb_layout_index_t *out)
{
    /* Check for minimal `[` + index + `]` */
    जर (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        परत false;
    }

    /* Small numeric index */
    जर (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_index_t)(s[1] - '1');
        परत true;
    }

    /* Named indices ranges */
    स्थिर स्थिरांक संरचना {
        स्थिरांक अक्षर* name;
        माप_प्रकार size;
        xkb_layout_index_t मूल्य;
    } names[] = {
        #define INDEX_RANGE(tail, मूल्य) \
            { (tail), आकार(tail), (xkb_layout_index_t)(मूल्य) }
        INDEX_RANGE("single]",   LAYOUT_INDEX_SINGLE),
        INDEX_RANGE("first]",    LAYOUT_INDEX_FIRST),
        INDEX_RANGE("later]",    LAYOUT_INDEX_LATER),
        INDEX_RANGE("multiple]", LAYOUT_INDEX_MULTIPLE),
        INDEX_RANGE("any]",      LAYOUT_INDEX_ANY),
        #undef INDEX_RANGE
    };

    साठी (माप_प्रकार k = 0; k < ARRAY_SIZE(names); k++) {
        जर (max_len == names[k].size &&
            स_भेद(&s[1], names[k].name, names[k].size - 1) == 0) {
            *out = names[k].मूल्य;
            परत true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    स्थिरांक पूर्णांक मोजणी = parse_layout_int_index_tail(s, max_len, out);
    परत मोजणी > 0 && (माप_प्रकार)मोजणी == max_len;
}

स्थिर ओळीत bool
is_mlvo_mask_defined(संरचना matcher *m, गणना rules_mlvo mlvo)
{
    परत m->mapping.defined_mlvo_mask & (1u << mlvo);
}

स्थिर शून्य
matcher_mapping_set_mlvo(संरचना matcher *m, संरचना scanner *s,
                         संरचना sval ident)
{
    गणना rules_mlvo mlvo;
    संरचना sval mlvo_sval;

    साठी (mlvo = 0; mlvo < MLVO_अंक_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        जर (svaleq_prefix(mlvo_sval, ident))
            थांबा;
    }

    /* Not found. */
    जर (mlvo >= MLVO_अंक_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (अचिन्हित पूर्णांक) ident.len, ident.start);
        m->mapping.active = false;
        परत;
    }

    जर (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (अचिन्हित पूर्णांक) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        परत;
    }

    /* If there are leftovers still, it must be an index. */
    जर (mlvo_sval.len < ident.len) {
        xkb_layout_index_t idx;
        जर (!extract_mapping_layout_index(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (अचिन्हित पूर्णांक) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            परत;
        }

        जर (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        अन्यथा जर (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        अन्यथा {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (अचिन्हित पूर्णांक) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            परत;
        }
    } अन्यथा जर (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    } अन्यथा जर (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    जर (((mlvo == MLVO_LAYOUT && is_mlvo_mask_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_mask_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.active = false;
        परत;
    }

    m->mapping.mlvo_at_pos[m->mapping.अंक_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.अंक_mlvo++;
}

स्थिर शून्य
matcher_mapping_set_layout_bounds(संरचना matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_index_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    निवड (idx) {
        स्थिती XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            प्रतिज्ञा(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            थांबा;
        स्थिती LAYOUT_INDEX_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            थांबा;
        स्थिती LAYOUT_INDEX_MULTIPLE:
        स्थिती LAYOUT_INDEX_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            थांबा;
        स्थिती LAYOUT_INDEX_SINGLE:
        स्थिती LAYOUT_INDEX_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        पूर्वनिर्धारित:
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

    साठी (kccgst = 0; kccgst < KCCGST_अंक_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        जर (svaleq(rules_kccgst_svals[kccgst], ident))
            थांबा;
    }

    /* Not found. */
    जर (kccgst >= KCCGST_अंक_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (अचिन्हित पूर्णांक) ident.len, ident.start);
        m->mapping.active = false;
        परत;
    }

    जर (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (अचिन्हित पूर्णांक) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        परत;
    }

    m->mapping.kccgst_at_pos[m->mapping.अंक_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.अंक_kccgst++;
}

स्थिर bool
matcher_mapping_verify(संरचना matcher *m, संरचना scanner *s)
{
    जर (m->mapping.अंक_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        जा skip;
    }

    जर (m->mapping.अंक_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        जा skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    जर (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        प्रतिज्ञा(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        निवड (m->mapping.layout_idx) {
            स्थिती LAYOUT_INDEX_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                जर (darray_size(m->rmlvo.layouts) > 1)
                    जा skip;
                थांबा;
            स्थिती LAYOUT_INDEX_MULTIPLE:
            स्थिती LAYOUT_INDEX_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                जर (darray_size(m->rmlvo.layouts) < 2)
                    जा skip;
                थांबा;
            स्थिती LAYOUT_INDEX_ANY:
            स्थिती LAYOUT_INDEX_FIRST:
                /* No restrictions */
                थांबा;
            पूर्वनिर्धारित:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                जर (darray_size(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_size(m->rmlvo.layouts))
                    जा skip;
        }
    }

    जर (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        प्रतिज्ञा(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        निवड (m->mapping.variant_idx) {
            स्थिती LAYOUT_INDEX_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                जर (darray_size(m->rmlvo.variants) > 1)
                    जा skip;
                थांबा;
            स्थिती LAYOUT_INDEX_ANY:
            स्थिती LAYOUT_INDEX_LATER:
            स्थिती LAYOUT_INDEX_FIRST:
                /* No restriction */
                थांबा;
            पूर्वनिर्धारित:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                जर (darray_size(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_size(m->rmlvo.variants))
                    जा skip;
        }
    }

    परत true;

skip:
    m->mapping.active = false;
    परत false;
}

स्थिर शून्य
matcher_rule_start_new(संरचना matcher *m)
{
    स_रखो(&m->rule, 0, आकार(m->rule));
    m->rule.skip = !m->mapping.active;
}

स्थिर शून्य
matcher_rule_set_mlvo_common(संरचना matcher *m, संरचना scanner *s,
                             संरचना sval ident,
                             गणना mlvo_match_type match_type)
{
    जर (m->rule.अंक_mlvo_मूल्ये >= m->mapping.अंक_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        परत;
    }
    m->rule.match_type_at_pos[m->rule.अंक_mlvo_मूल्ये] = match_type;
    m->rule.mlvo_मूल्य_at_pos[m->rule.अंक_mlvo_मूल्ये] = ident;
    m->rule.अंक_mlvo_मूल्ये++;
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
    जर (m->rule.अंक_kccgst_मूल्ये >= m->mapping.अंक_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        परत;
    }
    m->rule.kccgst_मूल्य_at_pos[m->rule.अंक_kccgst_मूल्ये] = ident;
    m->rule.अंक_kccgst_मूल्ये++;
}

स्थिर bool
match_group(संरचना matcher *m, संरचना sval group_name, संरचना sval to)
{
    संरचना group *group = रिक्त;
    संरचना sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        जर (svaleq(group->name, group_name)) {
            found = true;
            थांबा;
        }
    }

    जर (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        परत false;
    }

    darray_foreach(element, group->elements)
        जर (svaleq(to, *element))
            परत true;

    परत false;
}

स्थिर bool
match_मूल्य(संरचना matcher *m, स्थिरांक संरचना sval val, स्थिरांक संरचना sval to,
            गणना mlvo_match_type match_type,
            गणना wildcard_match_type wildcard_type)
{
    निवड (match_type) {
        स्थिती MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            परत wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        स्थिती MLVO_MATCH_WILDCARD_NONE:
            परत !to.len;
        स्थिती MLVO_MATCH_WILDCARD_SOME:
            परत !!to.len;
        स्थिती MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            परत true;
        स्थिती MLVO_MATCH_GROUP:
            परत match_group(m, val, to);
        पूर्वनिर्धारित:
            प्रतिज्ञा(match_type == MLVO_MATCH_NORMAL);
            परत svaleq(val, to);
    }
}

स्थिर bool
match_मूल्य_and_mark(संरचना matcher *m, स्थिरांक संरचना sval val,
                     संरचना matched_sval *to, गणना mlvo_match_type match_type,
                     गणना wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_मूल्य(m, val, to->sval, match_type, wildcard_type);
    जर (matched)
        to->matched |= layouts;
    परत matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
स्थिर bool
expand_rmlvo_in_kccgst_मूल्य(संरचना matcher *m, संरचना scanner *s,
                             संरचना sval मूल्य, xkb_layout_index_t layout_idx,
                             darray_char *expanded, माप_प्रकार *i)
{
    स्थिरांक अक्षर *str = मूल्य.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    गणना rules_mlvo mlv;
    xkb_layout_index_t idx;
    अक्षर pfx, sfx;
    संरचना matched_sval *expanded_मूल्य;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    जर (str[*i] == 'i' &&
        (*i + 1 == मूल्य.len || is_merge_mode_prefix(str[*i + 1])))
    {
        जर (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_INDEX_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            जा error;
        }
        (*i)++;
        अक्षर index_str[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
        स्थिरांक पूर्णांक मोजणी = snprintf(index_str, आकार(index_str), "%"PRIu32,
                                   layout_idx + 1);
        प्रतिज्ञा(मोजणी > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, index_str, (darray_size_t)मोजणी);
        परत true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    जर (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        जर (str[*i] == '(') sfx = ')';
        जर (++(*i) >= मूल्य.len) जा error;
    }

    /* Mandatory model/layout/variant specifier. */
    निवड (str[(*i)++]) {
    स्थिती 'm': mlv = MLVO_MODEL; थांबा;
    स्थिती 'l': mlv = MLVO_LAYOUT; थांबा;
    स्थिती 'v': mlv = MLVO_VARIANT; थांबा;
    पूर्वनिर्धारित: जा error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_index = false;
    जर (*i < मूल्य.len && str[*i] == '[') {
        जर (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            जा error;
        }

        पूर्णांक consumed = extract_layout_index(str + (*i), मूल्य.len - (*i), &idx);
        जर (consumed <= 0) जा error;
        जर (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            प्रतिज्ञा(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_index = true;
        }
        *i += (माप_प्रकार)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    जर (sfx != 0) {
        जर (*i >= मूल्य.len) जा error;
        जर (str[(*i)++] != sfx) जा error;
    }

    /* Get the expanded value. */
    expanded_मूल्य = रिक्त;

    जर (mlv == MLVO_LAYOUT) {
        जर (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            जर (darray_size(m->rmlvo.layouts) == 1)
                expanded_मूल्य = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } अन्यथा जर (idx < darray_size(m->rmlvo.layouts) &&
                   (variable_index || darray_size(m->rmlvo.layouts) > 1)) {
                expanded_मूल्य = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    अन्यथा जर (mlv == MLVO_VARIANT) {
        जर (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            जर (darray_size(m->rmlvo.variants) == 1)
                expanded_मूल्य = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } अन्यथा जर (idx < darray_size(m->rmlvo.variants) &&
                   (variable_index || darray_size(m->rmlvo.variants) > 1)) {
                expanded_मूल्य = &darray_item(m->rmlvo.variants, idx);
        }
    }
    अन्यथा जर (mlv == MLVO_MODEL) {
        expanded_मूल्य = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    जर (!expanded_मूल्य || expanded_मूल्य->sval.len == 0) {
        परत true;
    }

    जर (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_मूल्य->sval.start,
                                 (darray_size_t) expanded_मूल्य->sval.len);
    जर (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    जर (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        स्थिरांक xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_मूल्य->matched |= layouts;
    } अन्यथा {
        expanded_मूल्य->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    परत true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    परत false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
स्थिर शून्य
expand_qualifier_in_kccgst_मूल्य(
    संरचना matcher *m, संरचना scanner *s,
    संरचना sval मूल्य, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_size_t prefix_idx, माप_प्रकार *i)
{
    स्थिरांक अक्षर *str = मूल्य.start;

    /* “all” followed by nothing or by a layout separator */
    जर (*i + 2 < मूल्य.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == मूल्य.len || is_merge_mode_prefix(str[*i + 3]))) {
        जर (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_MESSAGE_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        जर (darray_size(m->rmlvo.layouts) > 1) {
            अक्षर layout_index[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
            स्थिरांक darray_size_t prefix_length =
                darray_size(*expanded) - prefix_idx - 1;
            साठी (xkb_layout_index_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_size(m->rmlvo.layouts));
                 l++)
            {
                जर (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_length);
                /* Append index */
                स्थिरांक पूर्णांक मोजणी = snprintf(layout_index, आकार(layout_index),
                                           "%"PRIu32, l + 1);
                प्रतिज्ञा(मोजणी > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_index,
                                             (darray_size_t)मोजणी);
            }
        }
        *i += 3;
    }
}

स्थिर ओळीत शून्य
#ifdef _MSC_VER
concat_kccgst(darray_char *into, darray_size_t size, _In_reads_(size) स्थिरांक अक्षर* from)
#अन्यथा
concat_kccgst(darray_char *into, darray_size_t size, स्थिरांक अक्षर from[स्थिर size])
#endif
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    स्थिरांक bool from_plus = is_merge_mode_prefix(from[0]);
    जर (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, size);
    } अन्यथा {
        स्थिरांक अक्षर ch =
            (अक्षर) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        स्थिरांक bool into_plus = is_merge_mode_prefix(ch);
        जर (into_plus)
            darray_prepends_nullterminate(*into, from, size);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
स्थिर bool
append_expanded_kccgst_मूल्य(संरचना matcher *m, संरचना scanner *s,
                             bool merge, darray_char *to, संरचना sval मूल्य,
                             xkb_layout_index_t layout_idx)
{
    स्थिरांक अक्षर *str = मूल्य.start;
    darray_char expanded = darray_new();
    darray_size_t last_item_idx = 0;
    bool has_separator = false;

    साठी (माप_प्रकार i = 0; i < मूल्य.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        निवड (str[i]) {
            /* Qualifier */
            स्थिती ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_मूल्य(m, s, मूल्य, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                थांबा;
            /* Expansion */
            स्थिती '%':
                i++;
                जर (i >= मूल्य.len ||
                    !expand_rmlvo_in_kccgst_मूल्य(m, s, मूल्य, layout_idx,
                                                  &expanded, &i))
                        जा error;
                थांबा;
            /* New item */
            स्थिती MERGE_OVERRIDE_PREFIX:
            स्थिती MERGE_AUGMENT_PREFIX:
            स्थिती MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_size(expanded) - 1;
                has_separator = true;
                थांबा;
            /* Just a normal character. */
            पूर्वनिर्धारित:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    जर (merge) {
        जर (!darray_empty(expanded))
            concat_kccgst(to, darray_size(expanded), darray_वस्तुसंच(expanded));
    } अन्यथा {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    परत true;

error:
    darray_free(expanded);
    परत false;
}

स्थिर bool
matcher_append_pending_kccgst(संरचना matcher *m)
{
    जर (!m->mapping.has_multiple_layouts)
        परत true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    साठी (kccgst_index_t i = 0; i < m->mapping.अंक_kccgst; i++) {
        स्थिरांक गणना rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        साठी (xkb_layout_index_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            नोंदणी स्थिरांक संरचना kccgst_buffer* स्थिरांक buf = &m->pending_kccgst;
            माप_प्रकार offset = 0;
            साठी (darray_size_t k = 0; k < darray_size(buf->slices); k++) {
                नोंदणी स्थिरांक संरचना kccgst_buffer_slice * स्थिरांक slice =
                    &darray_item(buf->slices, k);
                जर (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->length)
                    concat_kccgst(&m->kccgst[kccgst], slice->length,
                                  darray_वस्तुसंच(buf->buffer) + offset);
                offset += slice->length;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    परत true;
}

स्थिर शून्य
matcher_rule_verify(संरचना matcher *m, संरचना scanner *s)
{
    जर (m->rule.अंक_mlvo_मूल्ये != m->mapping.अंक_mlvo ||
        m->rule.अंक_kccgst_मूल्ये != m->mapping.अंक_kccgst) {
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
matcher_rule_match_model(संरचना matcher *m, स्थिरांक संरचना sval मूल्य,
                         गणना mlvo_match_type match_type)
{
    परत match_मूल्य_and_mark(
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
स्थिर bool
matcher_rule_match_option(संरचना matcher *m, स्थिरांक संरचना sval मूल्य,
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
        स्थिरांक xkb_layout_mask_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
        जर (matchable && match_मूल्य_and_mark(m, मूल्य, option, match_type,
                                              WILDCARD_MATCH_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            जर (!unmatched)
                थांबा;
        }
    }
    जर (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    परत matched;
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
                                     darray_matched_sval *आदान,
                                     स्थिरांक संरचना sval मूल्य,
                                     गणना mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    प्रतिज्ञा(!darray_empty(*आदान));

    bool matched = false;
    /* Loop over the layout index range */
    साठी (xkb_layout_index_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        स्थिरांक xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        जर (layout & *candidates) {
            संरचना matched_sval *to = &darray_item(*आदान, idx);
            जर (match_मूल्य_and_mark(m, मूल्य, to, match_type,
                                     WILDCARD_MATCH_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } अन्यथा {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    परत matched;
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
    साठी (mlvo_index_t i = 0; i < m->mapping.अंक_mlvo; i++) {
        स्थिरांक गणना rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        स्थिरांक संरचना sval मूल्य = m->rule.mlvo_मूल्य_at_pos[i];
        स्थिरांक गणना mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        निवड (mlvo) {
        स्थिती MLVO_MODEL:
            matched = matcher_rule_match_model(m, मूल्य, match_type);
            थांबा;
        स्थिती MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, मूल्य, match_type, &candidate_layouts
            );
            थांबा;
        स्थिती MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, मूल्य, match_type, &candidate_layouts
            );
            थांबा;
        स्थिती MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, मूल्य, match_type, &candidate_layouts
            );
            थांबा;
        पूर्वनिर्धारित: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_अंक_ENTRIES - 1,
                          "Unexpected MLVO field");
            प्रतिज्ञा(!"Unreachable");
        }
        }

        जर (!matched)
            परत;
    }

    जर (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        साठी (xkb_layout_index_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            जर (candidate_layouts & (UINT32_C(1) << idx)) {
                साठी (kccgst_index_t i = 0; i < m->mapping.अंक_kccgst; i++) {
                    स्थिरांक गणना rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    स्थिरांक संरचना sval मूल्य = m->rule.kccgst_मूल्य_at_pos[i];
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
                    संरचना kccgst_buffer * स्थिरांक buf = &m->pending_kccgst;
                    स्थिरांक darray_size_t prev_buffer_length =
                        darray_size(buf->buffer);
                    append_expanded_kccgst_मूल्य(m, s, false, &buf->buffer,
                                                 मूल्य, idx);
                    स्थिरांक uint32_t length = (uint32_t) (darray_size(buf->buffer)
                                          - prev_buffer_length);
                    स्थिरांक संरचना kccgst_buffer_slice slice = {
                        .length = length,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } अन्यथा {
        /* Numeric index or no index */
        साठी (kccgst_index_t i = 0; i < m->mapping.अंक_kccgst; i++) {
            गणना rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            संरचना sval मूल्य = m->rule.kccgst_मूल्य_at_pos[i];
            append_expanded_kccgst_मूल्य(m, s, true, &m->kccgst[kccgst], मूल्य,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    जर (!(is_mlvo_mask_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_mask &= ~candidate_layouts;
    }
}

स्थिर गणना rules_token
gettok(संरचना matcher *m, संरचना scanner *s)
{
    परत lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
स्थिर bool
matcher_match(संरचना matcher *m, संरचना scanner *s,
              अचिन्हित पूर्णांक include_depth,
              स्थिरांक अक्षर *string, माप_प्रकार len,
              स्थिरांक अक्षर *file_name)
{
    गणना rules_token tok;

    जर (!m)
        परत false;

initial:
    निवड (tok = gettok(m, s)) {
    स्थिती TOK_BANG:
        जा bang;
    स्थिती TOK_END_OF_ओळ:
        जा initial;
    स्थिती TOK_END_OF_FILE:
        जा finish;
    पूर्वनिर्धारित:
        जा unexpected;
    }

bang:
    निवड (tok = gettok(m, s)) {
    स्थिती TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        जा group_name;
    स्थिती TOK_INCLUDE:
        जा include_statement;
    स्थिती TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        जा mapping_mlvo;
    पूर्वनिर्धारित:
        जा unexpected;
    }

group_name:
    निवड (tok = gettok(m, s)) {
    स्थिती TOK_EQUALS:
        जा group_element;
    पूर्वनिर्धारित:
        जा unexpected;
    }

group_element:
    निवड (tok = gettok(m, s)) {
    स्थिती TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        जा group_element;
    स्थिती TOK_END_OF_ओळ:
        जा initial;
    पूर्वनिर्धारित:
        जा unexpected;
    }

include_statement:
    निवड (tok = gettok(m, s)) {
    स्थिती TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        जा include_statement_end;
    पूर्वनिर्धारित:
        जा unexpected;
    }

include_statement_end:
    निवड (tok = gettok(m, s)) {
    स्थिती TOK_END_OF_ओळ:
        जा initial;
    पूर्वनिर्धारित:
        जा unexpected;
    }

mapping_mlvo:
    निवड (tok = gettok(m, s)) {
    स्थिती TOK_IDENTIFIER:
        जर (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        जा mapping_mlvo;
    स्थिती TOK_EQUALS:
        जा mapping_kccgst;
    पूर्वनिर्धारित:
        जा unexpected;
    }

mapping_kccgst:
    निवड (tok = gettok(m, s)) {
    स्थिती TOK_IDENTIFIER:
        जर (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        जा mapping_kccgst;
    स्थिती TOK_END_OF_ओळ:
        जर (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            जर (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_size(m->pending_kccgst.buffer) = 0;
                darray_size(m->pending_kccgst.slices) = 0;
            }
        }
        जा rule_mlvo_first;
    पूर्वनिर्धारित:
        जा unexpected;
    }

rule_mlvo_first:
    निवड (tok = gettok(m, s)) {
    स्थिती TOK_BANG:
        matcher_append_pending_kccgst(m);
        जा bang;
    स्थिती TOK_END_OF_ओळ:
        जा rule_mlvo_first;
    स्थिती TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        जा finish;
    पूर्वनिर्धारित:
        matcher_rule_start_new(m);
        जा rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    निवड (tok) {
    स्थिती TOK_IDENTIFIER:
        जर (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        जा rule_mlvo;
    स्थिती TOK_WILD_CARD_STAR:
        जर (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        जा rule_mlvo;
    स्थिती TOK_WILD_CARD_NONE:
        जर (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        जा rule_mlvo;
    स्थिती TOK_WILD_CARD_SOME:
        जर (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        जा rule_mlvo;
    स्थिती TOK_WILD_CARD_ANY:
        जर (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        जा rule_mlvo;
    स्थिती TOK_GROUP_NAME:
        जर (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        जा rule_mlvo;
    स्थिती TOK_EQUALS:
        जा rule_kccgst;
    पूर्वनिर्धारित:
        जा unexpected;
    }

rule_kccgst:
    निवड (tok = gettok(m, s)) {
    स्थिती TOK_IDENTIFIER:
        जर (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        जा rule_kccgst;
    स्थिती TOK_END_OF_ओळ:
        जर (!m->rule.skip)
            matcher_rule_verify(m, s);
        जर (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        जा rule_mlvo_first;
    पूर्वनिर्धारित:
        जा unexpected;
    }

unexpected:
    निवड (tok) {
    स्थिती TOK_ERROR:
        जा error;
    पूर्वनिर्धारित:
        जा state_error;
    }

finish:
    परत true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    परत false;
}
// NOLINTEND(readability-trivial-switch)

स्थिर bool
read_rules_file(संरचना xkb_context *ctx,
                संरचना matcher *matcher,
                अचिन्हित पूर्णांक include_depth,
                खाता *file,
                स्थिरांक अक्षर *path)
{
    bool ret;
    अक्षर *string;
    माप_प्रकार size;
    संरचना scanner scanner;

    जर (!map_file(file, &string, &size)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, म_त्रुटि(त्रुटि_सं));
        परत false;
    }

    scanner_init(&scanner, matcher->ctx, string, size, path, रिक्त);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    जर (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, size);
        परत false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, size, path);
    unmap_file(string, size);
    परत ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
स्थिर bool
xkb_resolve_partial_rules(संरचना xkb_context *ctx, अक्षर *path, माप_प्रकार path_size,
                          स्थिरांक अक्षर* rules, स्थिरांक अक्षर* suffix,
                          संरचना matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    अक्षर partial_rules[60]; /* Arbitrary, but we do not expect long names */
    जर (unlikely(!snprintf_safe(partial_rules, आकार(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        परत false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    अचिन्हित पूर्णांक offset = 0;
    खाता *file = रिक्त;
    स्थिरांक माप_प्रकार len = म_माप(partial_rules);
    पर्यंत ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_size, &offset, false)) != रिक्त) {
        स्थिरांक bool ok = read_rules_file(ctx, matcher, 0, file, path);
        ख_बंद(file);
        जर (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            परत false;
        }
        offset++;
    }

    परत true;
}

स्थिर bool
xkb_resolve_rules(संरचना xkb_context *ctx,
                  स्थिरांक अक्षर* rules, संरचना matcher *matcher,
                  संरचना xkb_component_names *out,
                  xkb_layout_index_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    अचिन्हित पूर्णांक offset = 0;
    अक्षर path[PATH_MAX];
    खाता * स्थिरांक file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, म_माप(rules), FILE_TYPE_RULES,
                                          path, आकार(path), &offset, true);
    जर (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        जा err_out;
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
    ret = xkb_resolve_partial_rules(ctx, path, आकार(path),
                                    rules, ".pre", matcher);
    जर (!ret)
        जा err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    जर (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        जा err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, आकार(path),
                                    rules, ".post", matcher);
    जर (!ret)
        जा err_out;

    /* Check we got required components */
    जर (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        जा err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, रिक्त);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, रिक्त);

    संरचना matched_sval *mval = &matcher->rmlvo.model;
    जर (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मूल्य,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (अचिन्हित पूर्णांक) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        जर (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मूल्य,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (अचिन्हित पूर्णांक) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        जर (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मूल्य,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (अचिन्हित पूर्णांक) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        जर (!mval->sval.len)
            सुरूठेवा;

        जर (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मूल्य,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (अचिन्हित पूर्णांक) mval->sval.len, mval->sval.start, rules);
        } अन्यथा जर (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            स्थिरांक xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            प्रतिज्ञा(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_मूल्य,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (अचिन्हित पूर्णांक)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    जर (out->symbols != रिक्त && explicit_layouts != रिक्त) {
        *explicit_layouts = 1; /* at least one group */
        स्थिरांक अक्षर *symbols = out->symbols;
        /* Take the highest modifier */
        पर्यंत ((symbols = म_अक्षर(symbols, ':')) != रिक्त && symbols[1] != '\0') {
            xkb_layout_index_t group = 0;
            स्थिरांक पूर्णांक मोजणी = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            जर (मोजणी > 0 && (symbols[मोजणी] == '\0' ||
                is_merge_mode_prefix(symbols[मोजणी])) &&
                group > 0 && group <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, group);
                symbols += मोजणी;
            }
        }
    }

err_out:
    जर (file)
        ख_बंद(file);

    परत ret;
}

bool
xkb_components_from_rmlvo_builder(स्थिरांक संरचना xkb_rmlvo_builder *rmlvo,
                                  संरचना xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts)
{
    स्थिरांक अक्षर *rules = rmlvo->rules;
    संरचना matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    जर (!matcher)
        परत false;

    स्थिरांक bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    परत ret;
}


bool
xkb_components_from_rules_names(संरचना xkb_context *ctx,
                                स्थिरांक संरचना xkb_rule_names *rmlvo,
                                संरचना xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts)
{
    संरचना matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    जर (!matcher)
        परत false;

    स्थिरांक bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    परत ret;
}
