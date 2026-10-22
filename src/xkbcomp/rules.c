/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <stdint.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "rules.h"
#അന്തര്ഭാവ "rmlvo.h"
#അന്തര്ഭാവ "include.h"
#അന്തര്ഭാവ "scanner-utils.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "util-numbers.h"
#അന്തര്ഭാവ "util-paths.h"

#define MAX_INCLUDE_DEPTH 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
സംഘം lvalue {
    ഘടന sval string;
};

എണ്ണൽ rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_വരി,
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

സ്ഥിതം എണ്ണൽ rules_token
lex(ഘടന scanner *s, സംഘം lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    ആകുമ്പോൾ (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    എങ്കിൽ (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    എങ്കിൽ (scanner_eol(s)) {
        ആകുമ്പോൾ (scanner_eol(s)) scanner_next(s);
        തിരികെ TOK_END_OF_വരി;
    }

    /* Escaped line continuation */
    എങ്കിൽ (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        എങ്കിൽ (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            തിരികെ TOK_ERROR;
        }
        scanner_next(s);
        പോകുക skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    എങ്കിൽ (scanner_eof(s)) തിരികെ TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    എണ്ണൽ rules_token token = TOK_IDENTIFIER;

    സ്ഥിരം അക്ഷരം next = scanner_next(s);
    തിരഞ്ഞെടുപ്പ് (next) {
    അവസ്ഥ '!':
        തിരികെ TOK_BANG;
    അവസ്ഥ '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        നിർത്തുക;
    അവസ്ഥ '*':
        എങ്കിൽ (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            നിർത്തുക;
        } അല്ലെങ്കിൽ {
            /* Legacy wild card */
            തിരികെ TOK_WILD_CARD_STAR;
        }
    അവസ്ഥ '<':
        /* Extended wild cards */
        എങ്കിൽ (scanner_rules_lit_token(s, "none>")) തിരികെ TOK_WILD_CARD_NONE;
        എങ്കിൽ (scanner_rules_lit_token(s, "some>")) തിരികെ TOK_WILD_CARD_SOME;
        എങ്കിൽ (scanner_rules_lit_token(s, "any>")) തിരികെ TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        പ്രതിജ്ഞാ(scanner_rules_is_ident('<'));
        val->string.len = 1;
        നിർത്തുക;
    അവസ്ഥ '=':
        എങ്കിൽ (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            നിർത്തുക;
        } അല്ലെങ്കിൽ {
            തിരികെ TOK_EQUALS;
        }
    അവസ്ഥ 'i':
        എങ്കിൽ (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            തിരികെ TOK_INCLUDE;
        }
        /* Identifier */
        പ്രതിജ്ഞാ(scanner_rules_is_ident('i'));
        val->string.len = 1;
        നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (likely(scanner_rules_is_ident(next))) {
            /* Identifier */
            val->string.len = 1;
        } അല്ലെങ്കിൽ {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.start = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    പ്രതിജ്ഞാ(scanner_rules_is_ident(MERGE_OVERRIDE_PREFIX));
    പ്രതിജ്ഞാ(scanner_rules_is_ident(MERGE_AUGMENT_PREFIX));
    പ്രതിജ്ഞാ(scanner_rules_is_ident(MERGE_REPLACE_PREFIX));
    പ്രതിജ്ഞാ(scanner_rules_is_ident('('));
    പ്രതിജ്ഞാ(scanner_rules_is_ident(')'));
    val->string.len += scanner_rules_ident_token(s);

    എങ്കിൽ (unlikely(val->string.len == 0)) {
        എങ്കിൽ (token == TOK_GROUP_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } അല്ലെങ്കിൽ {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unrecognized token");
        }
        തിരികെ TOK_ERROR;
    }

    തിരികെ token;
}

/***====================================================================***/

എണ്ണൽ rules_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_അക്കം_ENTRIES
};

എണ്ണൽ {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

തരനിർവചനം uint8_t mlvo_index_t;
തരനിർവചനം uint8_t mlvo_mask_t;


സ്ഥിതം സ്ഥിരം ഘടന sval rules_mlvo_svals[MLVO_അക്കം_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

എണ്ണൽ rules_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_അക്കം_ENTRIES
};

തരനിർവചനം uint8_t kccgst_index_t;
തരനിർവചനം uint8_t kccgst_mask_t;

സ്ഥിതം സ്ഥിരം ഘടന sval rules_kccgst_svals[KCCGST_അക്കം_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

എണ്ണൽ {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL ==
              (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
ഘടന matched_sval {
    ഘടന sval sval;
    /** Used for layout-specific options */
    xkb_layout_mask_t layouts;
    xkb_layout_mask_t matched;
};
തരനിർവചനം darray(ഘടന matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
ഘടന rule_names {
    ഘടന matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

ഘടന group {
    ഘടന sval name;
    darray_sval elements;
};

ഘടന mapping {
    എണ്ണൽ rules_mlvo mlvo_at_pos[MLVO_അക്കം_ENTRIES];
    mlvo_index_t അക്കം_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    സംഘം {
        ഘടന { xkb_layout_index_t layout_idx, variant_idx; };
        ഘടന { xkb_layout_index_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    സംഘം {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    എണ്ണൽ rules_kccgst kccgst_at_pos[KCCGST_അക്കം_ENTRIES];
    kccgst_index_t അക്കം_kccgst;
    kccgst_mask_t defined_kccgst_mask;
};

എണ്ണൽ mlvo_match_type {
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

എണ്ണൽ wildcard_match_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_MATCH_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_MATCH_ALL,
};

ഘടന rule {
    ഘടന sval mlvo_മൂല്യം_at_pos[MLVO_അക്കം_ENTRIES];
    എണ്ണൽ mlvo_match_type match_type_at_pos[MLVO_അക്കം_ENTRIES];
    ഘടന sval kccgst_മൂല്യം_at_pos[KCCGST_അക്കം_ENTRIES];
    mlvo_index_t അക്കം_mlvo_മൂല്യങ്ങൾ;
    kccgst_index_t അക്കം_kccgst_മൂല്യങ്ങൾ;
    bool skip;
};

#define SLICE_KCCGST_BIT_FIELD_SIZE 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_SIZE - 1)) &
                (KCCGST_അക്കം_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
ഘടന kccgst_buffer_slice {
    uint32_t length:(32 - SLICE_KCCGST_BIT_FIELD_SIZE);
    എണ്ണൽ rules_kccgst kccgst:SLICE_KCCGST_BIT_FIELD_SIZE;
    xkb_layout_index_t layout;
};

/* Buffer for pending KcCGST values */
ഘടന kccgst_buffer {
    darray_char buffer;
    /* Slice corresponding to each value in the buffer */
    darray(ഘടന kccgst_buffer_slice) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
ഘടന matcher {
    ഘടന xkb_context *ctx;
    /* Input.*/
    ഘടന rule_names rmlvo;
    സംഘം lvalue val;
    darray(ഘടന group) groups;
    /* Current mapping. */
    ഘടന mapping mapping;
    /* Current rule. */
    ഘടന rule rule;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    ഘടന kccgst_buffer pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_അക്കം_ENTRIES];
};

സ്ഥിതം ഘടന sval
strip_spaces(ഘടന sval v)
{
    ആകുമ്പോൾ (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    ആകുമ്പോൾ (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    തിരികെ v;
}

സ്ഥിതം darray_matched_sval
split_comma_separated_mlvo(ഘടന xkb_context *ctx,
                           എണ്ണൽ rules_mlvo mlvo, സ്ഥിരം അക്ഷരം *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    എങ്കിൽ (!s) {
        ensure_at_least_one_മൂല്യം:
        /* Label followed by a declaration is a C23 extension */ ;

        ഘടന matched_sval val = {
            .sval = SVAL(ശൂന്യ, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        തിരികെ arr;
    }

    ആകുമ്പോൾ (true) {
        ഘടന matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        ആകുമ്പോൾ (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        എങ്കിൽ (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            സ്ഥിരം അക്ഷരം* സ്ഥിരം layout_start = s;
            xkb_layout_index_t layout = XKB_LAYOUT_INVALID;

            പൂർണ്ണസംഖ്യ എണ്ണം = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            എങ്കിൽ (എണ്ണം > 0) {
                /* Note: 1-indexed layout */
                s += എണ്ണം;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                എങ്കിൽ (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (unsigned പൂർണ്ണസംഖ്യ) val.sval.len, val.sval.start);
                } അല്ലെങ്കിൽ എങ്കിൽ (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (unsigned പൂർണ്ണസംഖ്യ) val.sval.len, val.sval.start);
                } അല്ലെങ്കിൽ {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            സ്ഥിരം അക്ഷരം* സ്ഥിരം layout_index_end = s;
            ആകുമ്പോൾ (*s != '\0' && *s != ',') { s++; }
            എങ്കിൽ (എണ്ണം <= 0 || layout_index_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (unsigned പൂർണ്ണസംഖ്യ) (s - layout_start), layout_start,
                        (unsigned പൂർണ്ണസംഖ്യ) val.sval.len, val.sval.start);
            }
        }

        എങ്കിൽ (val.sval.len || mlvo != MLVO_OPTION) {
            എങ്കിൽ (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                ഘടന matched_sval *prev;
                darray_foreach(prev, arr) {
                    എങ്കിൽ (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        എങ്കിൽ (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } അല്ലെങ്കിൽ എങ്കിൽ (prev->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        പോകുക next_മൂല്യം;
                    }
                }
            }
            darray_append(arr, val);
        }

next_മൂല്യം:
        എങ്കിൽ (*s == '\0') നിർത്തുക;
        എങ്കിൽ (*s == ',') s++;
    }

    എങ്കിൽ (darray_empty(arr))
        പോകുക ensure_at_least_one_മൂല്യം;

    തിരികെ arr;
}

സ്ഥിതം ഘടന matcher *
matcher_new_from_rmlvo(സ്ഥിരം ഘടന xkb_rmlvo_builder *rmlvo, സ്ഥിരം അക്ഷരം **rules)
{
    ഘടന matcher *m = calloc(1, വലുപ്പം(*m));
    എങ്കിൽ (!m)
        തിരികെ ശൂന്യ;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    ഘടന xkb_rule_names names = {
        .rules = rmlvo->rules,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_empty(rmlvo->layouts)) ? ശൂന്യ : "x",
        .variant = (darray_empty(rmlvo->layouts)) ? ശൂന്യ : "x",
        .options = (darray_empty(rmlvo->options)) ? ശൂന്യ : "x",
    };
    സ്ഥിരം എണ്ണൽ RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
                                                               &names);

    /* Initialize matcher with sanitized names, if relevant */
    എങ്കിൽ (changed & RMLVO_RULES) {
        *rules = names.rules;
    } അല്ലെങ്കിൽ {
        *rules = rmlvo->rules;
    }

    എങ്കിൽ (changed & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = names.model;
    } അല്ലെങ്കിൽ {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;

    പ്രതിജ്ഞാ((changed & RMLVO_LAYOUT) || !(changed & RMLVO_VARIANT));
    എങ്കിൽ (changed & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = split_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      names.layout);
        m->rmlvo.variants = split_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       names.variant);
        എങ്കിൽ (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            എങ്കിൽ (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        } അല്ലെങ്കിൽ എങ്കിൽ (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } അല്ലെങ്കിൽ {
        ഘടന xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            ഘടന matched_sval val = {
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

    എങ്കിൽ (changed & RMLVO_OPTIONS) {
        m->rmlvo.options = split_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      names.options);
    } അല്ലെങ്കിൽ {
        ഘടന xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            ഘടന matched_sval val = {
                .sval = SVAL(option->option, strlen_safe(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    തിരികെ m;
}

സ്ഥിതം ഘടന matcher *
matcher_new_from_names(ഘടന xkb_context *ctx,
            സ്ഥിരം ഘടന xkb_rule_names *rmlvo)
{
    ഘടന matcher *m = calloc(1, വലുപ്പം(*m));
    എങ്കിൽ (!m)
        തിരികെ ശൂന്യ;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    എങ്കിൽ (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        എങ്കിൽ (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
    } അല്ലെങ്കിൽ എങ്കിൽ (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    തിരികെ m;
}

സ്ഥിതം ശൂന്യം
matcher_free(ഘടന matcher *m)
{
    എങ്കിൽ (!m)
        തിരികെ;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    ഘടന group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.buffer);
    darray_free(m->pending_kccgst.slices);
    വേണ്ടി (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_അക്കം_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    സ്വതന്ത്രം(m);
}

സ്ഥിതം ശൂന്യം
matcher_group_start_new(ഘടന matcher *m, ഘടന sval name)
{
    ഘടന group group = { .name = name, .elements = darray_new() };
    darray_append(m->groups, group);
}

സ്ഥിതം ശൂന്യം
matcher_group_add_element(ഘടന matcher *m, ഘടന scanner *s,
                          ഘടന sval element)
{
    darray_append(darray_item(m->groups, darray_size(m->groups) - 1).elements,
                  element);
}

സ്ഥിതം bool
read_rules_file(ഘടന xkb_context *ctx,
                ഘടന matcher *matcher,
                unsigned പൂർണ്ണസംഖ്യ include_depth,
                FILE *file,
                സ്ഥിരം അക്ഷരം *path);

സ്ഥിതം ശൂന്യം
matcher_include(ഘടന matcher *m, ഘടന scanner *parent_scanner,
                unsigned പൂർണ്ണസംഖ്യ include_depth,
                ഘടന sval inc)
{
    എങ്കിൽ (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        തിരികെ;
    }

    സ്ഥിരം അക്ഷരം *stmt_file = inc.start;
    size_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    അക്ഷരം buf[PATH_MAX] = {0};
    സ്ഥിരം ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, വലുപ്പം(buf));
    എങ്കിൽ (expanded < 0) {
        /* Error */
        തിരികെ;
    } അല്ലെങ്കിൽ എങ്കിൽ (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (size_t) expanded;
        പ്രതിജ്ഞാ(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned പൂർണ്ണസംഖ്യ offset = 0;
    സ്ഥിരം bool absolute_path = is_absolute_path(stmt_file);
    എങ്കിൽ (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        എങ്കിൽ (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            എങ്കിൽ (stmt_file_len < വലുപ്പം(buf)) {
                memcpy(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } അല്ലെങ്കിൽ {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, വലുപ്പം(buf),
                        (unsigned പൂർണ്ണസംഖ്യ) stmt_file_len, stmt_file);
                തിരികെ;
            }
        } അല്ലെങ്കിൽ {
            /* %-expansion is always NULL-terminated */
            പ്രതിജ്ഞാ(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } അല്ലെങ്കിൽ {
        /* Relative path: lookup the first XKB path */
        എങ്കിൽ (unlikely(expanded)) {
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
            file = ശൂന്യ;
        } അല്ലെങ്കിൽ {
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, വലുപ്പം(buf), &offset, true);
        }
    }

    ആകുമ്പോൾ (file) {
        പ്രതിജ്ഞാ(strlen_safe(buf) < വലുപ്പം(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        fclose(file);
        എങ്കിൽ (ret)
            തിരികെ;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        എങ്കിൽ (absolute_path) {
            /* There is no point to search further if the path is absolute */
            നിർത്തുക;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, വലുപ്പം(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (unsigned പൂർണ്ണസംഖ്യ) stmt_file_len, stmt_file);
}

സ്ഥിതം ശൂന്യം
matcher_mapping_start_new(ഘടന matcher *m)
{
    വേണ്ടി (mlvo_index_t i = 0; i < (mlvo_index_t) MLVO_അക്കം_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_അക്കം_ENTRIES;
    വേണ്ടി (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_അക്കം_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_അക്കം_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.അക്കം_mlvo = m->mapping.അക്കം_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
സ്ഥിതം പൂർണ്ണസംഖ്യ
parse_layout_int_index_tail(സ്ഥിരം അക്ഷരം *s, size_t max_len,
                            xkb_layout_index_t *out)
{
    എങ്കിൽ (max_len < 3)
        തിരികെ -1;
    uint32_t val = 0;
    സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    എങ്കിൽ (എണ്ണം <= 0 || s[1 + എണ്ണം] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        തിരികെ -1;
    /* To zero-based index. */
    *out = val - 1;
    തിരികെ എണ്ണം + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
സ്ഥിതം പൂർണ്ണസംഖ്യ
extract_layout_index(സ്ഥിരം അക്ഷരം *s, size_t max_len, xkb_layout_index_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    എങ്കിൽ (max_len < 3 || s[0] != '[')
        തിരികെ -1;
    എങ്കിൽ (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_index_t)(s[1] - '1');
        തിരികെ 3;
    }
    എങ്കിൽ (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        തിരികെ 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    തിരികെ parse_layout_int_index_tail(s, max_len, out);
}

/* Special layout indices */
എണ്ണൽ layout_index_ranges {
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
സ്ഥിതം bool
extract_mapping_layout_index(സ്ഥിരം അക്ഷരം *s, size_t max_len,
                             xkb_layout_index_t *out)
{
    /* Check for minimal `[` + index + `]` */
    എങ്കിൽ (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        തിരികെ false;
    }

    /* Small numeric index */
    എങ്കിൽ (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_index_t)(s[1] - '1');
        തിരികെ true;
    }

    /* Named indices ranges */
    സ്ഥിതം സ്ഥിരം ഘടന {
        സ്ഥിരം അക്ഷരം* name;
        size_t size;
        xkb_layout_index_t മൂല്യം;
    } names[] = {
        #define INDEX_RANGE(tail, മൂല്യം) \
            { (tail), വലുപ്പം(tail), (xkb_layout_index_t)(മൂല്യം) }
        INDEX_RANGE("single]",   LAYOUT_INDEX_SINGLE),
        INDEX_RANGE("first]",    LAYOUT_INDEX_FIRST),
        INDEX_RANGE("later]",    LAYOUT_INDEX_LATER),
        INDEX_RANGE("multiple]", LAYOUT_INDEX_MULTIPLE),
        INDEX_RANGE("any]",      LAYOUT_INDEX_ANY),
        #undef INDEX_RANGE
    };

    വേണ്ടി (size_t k = 0; k < ARRAY_SIZE(names); k++) {
        എങ്കിൽ (max_len == names[k].size &&
            memcmp(&s[1], names[k].name, names[k].size - 1) == 0) {
            *out = names[k].മൂല്യം;
            തിരികെ true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം = parse_layout_int_index_tail(s, max_len, out);
    തിരികെ എണ്ണം > 0 && (size_t)എണ്ണം == max_len;
}

സ്ഥിതം വരിയിൽ bool
is_mlvo_mask_defined(ഘടന matcher *m, എണ്ണൽ rules_mlvo mlvo)
{
    തിരികെ m->mapping.defined_mlvo_mask & (1u << mlvo);
}

സ്ഥിതം ശൂന്യം
matcher_mapping_set_mlvo(ഘടന matcher *m, ഘടന scanner *s,
                         ഘടന sval ident)
{
    എണ്ണൽ rules_mlvo mlvo;
    ഘടന sval mlvo_sval;

    വേണ്ടി (mlvo = 0; mlvo < MLVO_അക്കം_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        എങ്കിൽ (svaleq_prefix(mlvo_sval, ident))
            നിർത്തുക;
    }

    /* Not found. */
    എങ്കിൽ (mlvo >= MLVO_അക്കം_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned പൂർണ്ണസംഖ്യ) ident.len, ident.start);
        m->mapping.active = false;
        തിരികെ;
    }

    എങ്കിൽ (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned പൂർണ്ണസംഖ്യ) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        തിരികെ;
    }

    /* If there are leftovers still, it must be an index. */
    എങ്കിൽ (mlvo_sval.len < ident.len) {
        xkb_layout_index_t idx;
        എങ്കിൽ (!extract_mapping_layout_index(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (unsigned പൂർണ്ണസംഖ്യ) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            തിരികെ;
        }

        എങ്കിൽ (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        അല്ലെങ്കിൽ {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (unsigned പൂർണ്ണസംഖ്യ) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            തിരികെ;
        }
    } അല്ലെങ്കിൽ എങ്കിൽ (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    } അല്ലെങ്കിൽ എങ്കിൽ (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    എങ്കിൽ (((mlvo == MLVO_LAYOUT && is_mlvo_mask_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_mask_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.active = false;
        തിരികെ;
    }

    m->mapping.mlvo_at_pos[m->mapping.അക്കം_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.അക്കം_mlvo++;
}

സ്ഥിതം ശൂന്യം
matcher_mapping_set_layout_bounds(ഘടന matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_index_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    തിരഞ്ഞെടുപ്പ് (idx) {
        അവസ്ഥ XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            പ്രതിജ്ഞാ(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            നിർത്തുക;
        അവസ്ഥ LAYOUT_INDEX_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            നിർത്തുക;
        അവസ്ഥ LAYOUT_INDEX_MULTIPLE:
        അവസ്ഥ LAYOUT_INDEX_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            നിർത്തുക;
        അവസ്ഥ LAYOUT_INDEX_SINGLE:
        അവസ്ഥ LAYOUT_INDEX_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        സ്ഥിരസ്ഥിതി:
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

സ്ഥിതം ശൂന്യം
matcher_mapping_set_kccgst(ഘടന matcher *m, ഘടന scanner *s, ഘടന sval ident)
{
    എണ്ണൽ rules_kccgst kccgst;
    ഘടന sval kccgst_sval;

    വേണ്ടി (kccgst = 0; kccgst < KCCGST_അക്കം_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        എങ്കിൽ (svaleq(rules_kccgst_svals[kccgst], ident))
            നിർത്തുക;
    }

    /* Not found. */
    എങ്കിൽ (kccgst >= KCCGST_അക്കം_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned പൂർണ്ണസംഖ്യ) ident.len, ident.start);
        m->mapping.active = false;
        തിരികെ;
    }

    എങ്കിൽ (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned പൂർണ്ണസംഖ്യ) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        തിരികെ;
    }

    m->mapping.kccgst_at_pos[m->mapping.അക്കം_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.അക്കം_kccgst++;
}

സ്ഥിതം bool
matcher_mapping_verify(ഘടന matcher *m, ഘടന scanner *s)
{
    എങ്കിൽ (m->mapping.അക്കം_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        പോകുക skip;
    }

    എങ്കിൽ (m->mapping.അക്കം_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        പോകുക skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    എങ്കിൽ (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        പ്രതിജ്ഞാ(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        തിരഞ്ഞെടുപ്പ് (m->mapping.layout_idx) {
            അവസ്ഥ LAYOUT_INDEX_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                എങ്കിൽ (darray_size(m->rmlvo.layouts) > 1)
                    പോകുക skip;
                നിർത്തുക;
            അവസ്ഥ LAYOUT_INDEX_MULTIPLE:
            അവസ്ഥ LAYOUT_INDEX_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                എങ്കിൽ (darray_size(m->rmlvo.layouts) < 2)
                    പോകുക skip;
                നിർത്തുക;
            അവസ്ഥ LAYOUT_INDEX_ANY:
            അവസ്ഥ LAYOUT_INDEX_FIRST:
                /* No restrictions */
                നിർത്തുക;
            സ്ഥിരസ്ഥിതി:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                എങ്കിൽ (darray_size(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_size(m->rmlvo.layouts))
                    പോകുക skip;
        }
    }

    എങ്കിൽ (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        പ്രതിജ്ഞാ(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        തിരഞ്ഞെടുപ്പ് (m->mapping.variant_idx) {
            അവസ്ഥ LAYOUT_INDEX_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                എങ്കിൽ (darray_size(m->rmlvo.variants) > 1)
                    പോകുക skip;
                നിർത്തുക;
            അവസ്ഥ LAYOUT_INDEX_ANY:
            അവസ്ഥ LAYOUT_INDEX_LATER:
            അവസ്ഥ LAYOUT_INDEX_FIRST:
                /* No restriction */
                നിർത്തുക;
            സ്ഥിരസ്ഥിതി:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                എങ്കിൽ (darray_size(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_size(m->rmlvo.variants))
                    പോകുക skip;
        }
    }

    തിരികെ true;

skip:
    m->mapping.active = false;
    തിരികെ false;
}

സ്ഥിതം ശൂന്യം
matcher_rule_start_new(ഘടന matcher *m)
{
    memset(&m->rule, 0, വലുപ്പം(m->rule));
    m->rule.skip = !m->mapping.active;
}

സ്ഥിതം ശൂന്യം
matcher_rule_set_mlvo_common(ഘടന matcher *m, ഘടന scanner *s,
                             ഘടന sval ident,
                             എണ്ണൽ mlvo_match_type match_type)
{
    എങ്കിൽ (m->rule.അക്കം_mlvo_മൂല്യങ്ങൾ >= m->mapping.അക്കം_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        തിരികെ;
    }
    m->rule.match_type_at_pos[m->rule.അക്കം_mlvo_മൂല്യങ്ങൾ] = match_type;
    m->rule.mlvo_മൂല്യം_at_pos[m->rule.അക്കം_mlvo_മൂല്യങ്ങൾ] = ident;
    m->rule.അക്കം_mlvo_മൂല്യങ്ങൾ++;
}

സ്ഥിതം ശൂന്യം
matcher_rule_set_mlvo_wildcard(ഘടന matcher *m, ഘടന scanner *s,
                               എണ്ണൽ mlvo_match_type match_type)
{
    ഘടന sval dummy = SVAL(ശൂന്യ, 0);
    matcher_rule_set_mlvo_common(m, s, dummy, match_type);
}

സ്ഥിതം ശൂന്യം
matcher_rule_set_mlvo_group(ഘടന matcher *m, ഘടന scanner *s,
                            ഘടന sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_GROUP);
}

സ്ഥിതം ശൂന്യം
matcher_rule_set_mlvo(ഘടന matcher *m, ഘടന scanner *s,
                      ഘടന sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_NORMAL);
}

സ്ഥിതം ശൂന്യം
matcher_rule_set_kccgst(ഘടന matcher *m, ഘടന scanner *s,
                        ഘടന sval ident)
{
    എങ്കിൽ (m->rule.അക്കം_kccgst_മൂല്യങ്ങൾ >= m->mapping.അക്കം_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        തിരികെ;
    }
    m->rule.kccgst_മൂല്യം_at_pos[m->rule.അക്കം_kccgst_മൂല്യങ്ങൾ] = ident;
    m->rule.അക്കം_kccgst_മൂല്യങ്ങൾ++;
}

സ്ഥിതം bool
match_group(ഘടന matcher *m, ഘടന sval group_name, ഘടന sval to)
{
    ഘടന group *group = ശൂന്യ;
    ഘടന sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        എങ്കിൽ (svaleq(group->name, group_name)) {
            found = true;
            നിർത്തുക;
        }
    }

    എങ്കിൽ (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        തിരികെ false;
    }

    darray_foreach(element, group->elements)
        എങ്കിൽ (svaleq(to, *element))
            തിരികെ true;

    തിരികെ false;
}

സ്ഥിതം bool
match_മൂല്യം(ഘടന matcher *m, സ്ഥിരം ഘടന sval val, സ്ഥിരം ഘടന sval to,
            എണ്ണൽ mlvo_match_type match_type,
            എണ്ണൽ wildcard_match_type wildcard_type)
{
    തിരഞ്ഞെടുപ്പ് (match_type) {
        അവസ്ഥ MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            തിരികെ wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        അവസ്ഥ MLVO_MATCH_WILDCARD_NONE:
            തിരികെ !to.len;
        അവസ്ഥ MLVO_MATCH_WILDCARD_SOME:
            തിരികെ !!to.len;
        അവസ്ഥ MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            തിരികെ true;
        അവസ്ഥ MLVO_MATCH_GROUP:
            തിരികെ match_group(m, val, to);
        സ്ഥിരസ്ഥിതി:
            പ്രതിജ്ഞാ(match_type == MLVO_MATCH_NORMAL);
            തിരികെ svaleq(val, to);
    }
}

സ്ഥിതം bool
match_മൂല്യം_and_mark(ഘടന matcher *m, സ്ഥിരം ഘടന sval val,
                     ഘടന matched_sval *to, എണ്ണൽ mlvo_match_type match_type,
                     എണ്ണൽ wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_മൂല്യം(m, val, to->sval, match_type, wildcard_type);
    എങ്കിൽ (matched)
        to->matched |= layouts;
    തിരികെ matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
സ്ഥിതം bool
expand_rmlvo_in_kccgst_മൂല്യം(ഘടന matcher *m, ഘടന scanner *s,
                             ഘടന sval മൂല്യം, xkb_layout_index_t layout_idx,
                             darray_char *expanded, size_t *i)
{
    സ്ഥിരം അക്ഷരം *str = മൂല്യം.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    എണ്ണൽ rules_mlvo mlv;
    xkb_layout_index_t idx;
    അക്ഷരം pfx, sfx;
    ഘടന matched_sval *expanded_മൂല്യം;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    എങ്കിൽ (str[*i] == 'i' &&
        (*i + 1 == മൂല്യം.len || is_merge_mode_prefix(str[*i + 1])))
    {
        എങ്കിൽ (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_INDEX_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            പോകുക error;
        }
        (*i)++;
        അക്ഷരം index_str[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
        സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം = snprintf(index_str, വലുപ്പം(index_str), "%"PRIu32,
                                   layout_idx + 1);
        പ്രതിജ്ഞാ(എണ്ണം > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, index_str, (darray_size_t)എണ്ണം);
        തിരികെ true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    എങ്കിൽ (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        എങ്കിൽ (str[*i] == '(') sfx = ')';
        എങ്കിൽ (++(*i) >= മൂല്യം.len) പോകുക error;
    }

    /* Mandatory model/layout/variant specifier. */
    തിരഞ്ഞെടുപ്പ് (str[(*i)++]) {
    അവസ്ഥ 'm': mlv = MLVO_MODEL; നിർത്തുക;
    അവസ്ഥ 'l': mlv = MLVO_LAYOUT; നിർത്തുക;
    അവസ്ഥ 'v': mlv = MLVO_VARIANT; നിർത്തുക;
    സ്ഥിരസ്ഥിതി: പോകുക error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_index = false;
    എങ്കിൽ (*i < മൂല്യം.len && str[*i] == '[') {
        എങ്കിൽ (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            പോകുക error;
        }

        പൂർണ്ണസംഖ്യ consumed = extract_layout_index(str + (*i), മൂല്യം.len - (*i), &idx);
        എങ്കിൽ (consumed <= 0) പോകുക error;
        എങ്കിൽ (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            പ്രതിജ്ഞാ(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_index = true;
        }
        *i += (size_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    എങ്കിൽ (sfx != 0) {
        എങ്കിൽ (*i >= മൂല്യം.len) പോകുക error;
        എങ്കിൽ (str[(*i)++] != sfx) പോകുക error;
    }

    /* Get the expanded value. */
    expanded_മൂല്യം = ശൂന്യ;

    എങ്കിൽ (mlv == MLVO_LAYOUT) {
        എങ്കിൽ (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            എങ്കിൽ (darray_size(m->rmlvo.layouts) == 1)
                expanded_മൂല്യം = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } അല്ലെങ്കിൽ എങ്കിൽ (idx < darray_size(m->rmlvo.layouts) &&
                   (variable_index || darray_size(m->rmlvo.layouts) > 1)) {
                expanded_മൂല്യം = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ (mlv == MLVO_VARIANT) {
        എങ്കിൽ (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            എങ്കിൽ (darray_size(m->rmlvo.variants) == 1)
                expanded_മൂല്യം = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } അല്ലെങ്കിൽ എങ്കിൽ (idx < darray_size(m->rmlvo.variants) &&
                   (variable_index || darray_size(m->rmlvo.variants) > 1)) {
                expanded_മൂല്യം = &darray_item(m->rmlvo.variants, idx);
        }
    }
    അല്ലെങ്കിൽ എങ്കിൽ (mlv == MLVO_MODEL) {
        expanded_മൂല്യം = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    എങ്കിൽ (!expanded_മൂല്യം || expanded_മൂല്യം->sval.len == 0) {
        തിരികെ true;
    }

    എങ്കിൽ (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_മൂല്യം->sval.start,
                                 (darray_size_t) expanded_മൂല്യം->sval.len);
    എങ്കിൽ (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    എങ്കിൽ (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        സ്ഥിരം xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_മൂല്യം->matched |= layouts;
    } അല്ലെങ്കിൽ {
        expanded_മൂല്യം->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    തിരികെ true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    തിരികെ false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
സ്ഥിതം ശൂന്യം
expand_qualifier_in_kccgst_മൂല്യം(
    ഘടന matcher *m, ഘടന scanner *s,
    ഘടന sval മൂല്യം, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_size_t prefix_idx, size_t *i)
{
    സ്ഥിരം അക്ഷരം *str = മൂല്യം.start;

    /* “all” followed by nothing or by a layout separator */
    എങ്കിൽ (*i + 2 < മൂല്യം.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == മൂല്യം.len || is_merge_mode_prefix(str[*i + 3]))) {
        എങ്കിൽ (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_MESSAGE_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        എങ്കിൽ (darray_size(m->rmlvo.layouts) > 1) {
            അക്ഷരം layout_index[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
            സ്ഥിരം darray_size_t prefix_length =
                darray_size(*expanded) - prefix_idx - 1;
            വേണ്ടി (xkb_layout_index_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_size(m->rmlvo.layouts));
                 l++)
            {
                എങ്കിൽ (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_length);
                /* Append index */
                സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം = snprintf(layout_index, വലുപ്പം(layout_index),
                                           "%"PRIu32, l + 1);
                പ്രതിജ്ഞാ(എണ്ണം > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_index,
                                             (darray_size_t)എണ്ണം);
            }
        }
        *i += 3;
    }
}

സ്ഥിതം വരിയിൽ ശൂന്യം
#ifdef _MSC_VER
concat_kccgst(darray_char *into, darray_size_t size, _In_reads_(size) സ്ഥിരം അക്ഷരം* from)
#അല്ലെങ്കിൽ
concat_kccgst(darray_char *into, darray_size_t size, സ്ഥിരം അക്ഷരം from[സ്ഥിതം size])
#endif
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    സ്ഥിരം bool from_plus = is_merge_mode_prefix(from[0]);
    എങ്കിൽ (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, size);
    } അല്ലെങ്കിൽ {
        സ്ഥിരം അക്ഷരം ch =
            (അക്ഷരം) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        സ്ഥിരം bool into_plus = is_merge_mode_prefix(ch);
        എങ്കിൽ (into_plus)
            darray_prepends_nullterminate(*into, from, size);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
സ്ഥിതം bool
append_expanded_kccgst_മൂല്യം(ഘടന matcher *m, ഘടന scanner *s,
                             bool merge, darray_char *to, ഘടന sval മൂല്യം,
                             xkb_layout_index_t layout_idx)
{
    സ്ഥിരം അക്ഷരം *str = മൂല്യം.start;
    darray_char expanded = darray_new();
    darray_size_t last_item_idx = 0;
    bool has_separator = false;

    വേണ്ടി (size_t i = 0; i < മൂല്യം.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        തിരഞ്ഞെടുപ്പ് (str[i]) {
            /* Qualifier */
            അവസ്ഥ ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_മൂല്യം(m, s, മൂല്യം, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                നിർത്തുക;
            /* Expansion */
            അവസ്ഥ '%':
                i++;
                എങ്കിൽ (i >= മൂല്യം.len ||
                    !expand_rmlvo_in_kccgst_മൂല്യം(m, s, മൂല്യം, layout_idx,
                                                  &expanded, &i))
                        പോകുക error;
                നിർത്തുക;
            /* New item */
            അവസ്ഥ MERGE_OVERRIDE_PREFIX:
            അവസ്ഥ MERGE_AUGMENT_PREFIX:
            അവസ്ഥ MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_size(expanded) - 1;
                has_separator = true;
                നിർത്തുക;
            /* Just a normal character. */
            സ്ഥിരസ്ഥിതി:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    എങ്കിൽ (merge) {
        എങ്കിൽ (!darray_empty(expanded))
            concat_kccgst(to, darray_size(expanded), darray_ഇനങ്ങൾ(expanded));
    } അല്ലെങ്കിൽ {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    തിരികെ true;

error:
    darray_free(expanded);
    തിരികെ false;
}

സ്ഥിതം bool
matcher_append_pending_kccgst(ഘടന matcher *m)
{
    എങ്കിൽ (!m->mapping.has_multiple_layouts)
        തിരികെ true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    വേണ്ടി (kccgst_index_t i = 0; i < m->mapping.അക്കം_kccgst; i++) {
        സ്ഥിരം എണ്ണൽ rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        വേണ്ടി (xkb_layout_index_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            രജിസ്റ്റർ സ്ഥിരം ഘടന kccgst_buffer* സ്ഥിരം buf = &m->pending_kccgst;
            size_t offset = 0;
            വേണ്ടി (darray_size_t k = 0; k < darray_size(buf->slices); k++) {
                രജിസ്റ്റർ സ്ഥിരം ഘടന kccgst_buffer_slice * സ്ഥിരം slice =
                    &darray_item(buf->slices, k);
                എങ്കിൽ (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->length)
                    concat_kccgst(&m->kccgst[kccgst], slice->length,
                                  darray_ഇനങ്ങൾ(buf->buffer) + offset);
                offset += slice->length;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    തിരികെ true;
}

സ്ഥിതം ശൂന്യം
matcher_rule_verify(ഘടന matcher *m, ഘടന scanner *s)
{
    എങ്കിൽ (m->rule.അക്കം_mlvo_മൂല്യങ്ങൾ != m->mapping.അക്കം_mlvo ||
        m->rule.അക്കം_kccgst_മൂല്യങ്ങൾ != m->mapping.അക്കം_kccgst) {
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
സ്ഥിതം bool
matcher_rule_match_model(ഘടന matcher *m, സ്ഥിരം ഘടന sval മൂല്യം,
                         എണ്ണൽ mlvo_match_type match_type)
{
    തിരികെ match_മൂല്യം_and_mark(
        m, മൂല്യം, &m->rmlvo.model, match_type, WILDCARD_MATCH_ALL,
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
സ്ഥിതം bool
matcher_rule_match_option(ഘടന matcher *m, സ്ഥിരം ഘടന sval മൂല്യം,
                          എണ്ണൽ mlvo_match_type match_type,
                          xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    പ്രതിജ്ഞാ(!darray_empty(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_mask_t unmatched =
        (m->mapping.defined_mlvo_mask & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    ഘടന matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        സ്ഥിരം xkb_layout_mask_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
        എങ്കിൽ (matchable && match_മൂല്യം_and_mark(m, മൂല്യം, option, match_type,
                                              WILDCARD_MATCH_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            എങ്കിൽ (!unmatched)
                നിർത്തുക;
        }
    }
    എങ്കിൽ (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    തിരികെ matched;
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
സ്ഥിതം bool
matcher_rule_match_layout_or_variant(ഘടന matcher *m,
                                     darray_matched_sval *നിവേശം,
                                     സ്ഥിരം ഘടന sval മൂല്യം,
                                     എണ്ണൽ mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    പ്രതിജ്ഞാ(!darray_empty(*നിവേശം));

    bool matched = false;
    /* Loop over the layout index range */
    വേണ്ടി (xkb_layout_index_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        സ്ഥിരം xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        എങ്കിൽ (layout & *candidates) {
            ഘടന matched_sval *to = &darray_item(*നിവേശം, idx);
            എങ്കിൽ (match_മൂല്യം_and_mark(m, മൂല്യം, to, match_type,
                                     WILDCARD_MATCH_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } അല്ലെങ്കിൽ {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    തിരികെ matched;
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
സ്ഥിതം ശൂന്യം
matcher_rule_apply_if_matches(ഘടന matcher *m, ഘടന scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_mask_t candidate_layouts = m->mapping.layouts_candidates_mask;

    /* Loop over MLVO pattern components */
    വേണ്ടി (mlvo_index_t i = 0; i < m->mapping.അക്കം_mlvo; i++) {
        സ്ഥിരം എണ്ണൽ rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        സ്ഥിരം ഘടന sval മൂല്യം = m->rule.mlvo_മൂല്യം_at_pos[i];
        സ്ഥിരം എണ്ണൽ mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        തിരഞ്ഞെടുപ്പ് (mlvo) {
        അവസ്ഥ MLVO_MODEL:
            matched = matcher_rule_match_model(m, മൂല്യം, match_type);
            നിർത്തുക;
        അവസ്ഥ MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, മൂല്യം, match_type, &candidate_layouts
            );
            നിർത്തുക;
        അവസ്ഥ MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, മൂല്യം, match_type, &candidate_layouts
            );
            നിർത്തുക;
        അവസ്ഥ MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, മൂല്യം, match_type, &candidate_layouts
            );
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_അക്കം_ENTRIES - 1,
                          "Unexpected MLVO field");
            പ്രതിജ്ഞാ(!"Unreachable");
        }
        }

        എങ്കിൽ (!matched)
            തിരികെ;
    }

    എങ്കിൽ (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        വേണ്ടി (xkb_layout_index_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            എങ്കിൽ (candidate_layouts & (UINT32_C(1) << idx)) {
                വേണ്ടി (kccgst_index_t i = 0; i < m->mapping.അക്കം_kccgst; i++) {
                    സ്ഥിരം എണ്ണൽ rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    സ്ഥിരം ഘടന sval മൂല്യം = m->rule.kccgst_മൂല്യം_at_pos[i];
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
                    ഘടന kccgst_buffer * സ്ഥിരം buf = &m->pending_kccgst;
                    സ്ഥിരം darray_size_t prev_buffer_length =
                        darray_size(buf->buffer);
                    append_expanded_kccgst_മൂല്യം(m, s, false, &buf->buffer,
                                                 മൂല്യം, idx);
                    സ്ഥിരം uint32_t length = (uint32_t) (darray_size(buf->buffer)
                                          - prev_buffer_length);
                    സ്ഥിരം ഘടന kccgst_buffer_slice slice = {
                        .length = length,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } അല്ലെങ്കിൽ {
        /* Numeric index or no index */
        വേണ്ടി (kccgst_index_t i = 0; i < m->mapping.അക്കം_kccgst; i++) {
            എണ്ണൽ rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            ഘടന sval മൂല്യം = m->rule.kccgst_മൂല്യം_at_pos[i];
            append_expanded_kccgst_മൂല്യം(m, s, true, &m->kccgst[kccgst], മൂല്യം,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    എങ്കിൽ (!(is_mlvo_mask_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_mask &= ~candidate_layouts;
    }
}

സ്ഥിതം എണ്ണൽ rules_token
gettok(ഘടന matcher *m, ഘടന scanner *s)
{
    തിരികെ lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
സ്ഥിതം bool
matcher_match(ഘടന matcher *m, ഘടന scanner *s,
              unsigned പൂർണ്ണസംഖ്യ include_depth,
              സ്ഥിരം അക്ഷരം *string, size_t len,
              സ്ഥിരം അക്ഷരം *file_name)
{
    എണ്ണൽ rules_token tok;

    എങ്കിൽ (!m)
        തിരികെ false;

initial:
    തിരഞ്ഞെടുപ്പ് (tok = gettok(m, s)) {
    അവസ്ഥ TOK_BANG:
        പോകുക bang;
    അവസ്ഥ TOK_END_OF_വരി:
        പോകുക initial;
    അവസ്ഥ TOK_END_OF_FILE:
        പോകുക finish;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

bang:
    തിരഞ്ഞെടുപ്പ് (tok = gettok(m, s)) {
    അവസ്ഥ TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        പോകുക group_name;
    അവസ്ഥ TOK_INCLUDE:
        പോകുക include_statement;
    അവസ്ഥ TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        പോകുക mapping_mlvo;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

group_name:
    തിരഞ്ഞെടുപ്പ് (tok = gettok(m, s)) {
    അവസ്ഥ TOK_EQUALS:
        പോകുക group_element;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

group_element:
    തിരഞ്ഞെടുപ്പ് (tok = gettok(m, s)) {
    അവസ്ഥ TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        പോകുക group_element;
    അവസ്ഥ TOK_END_OF_വരി:
        പോകുക initial;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

include_statement:
    തിരഞ്ഞെടുപ്പ് (tok = gettok(m, s)) {
    അവസ്ഥ TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        പോകുക include_statement_end;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

include_statement_end:
    തിരഞ്ഞെടുപ്പ് (tok = gettok(m, s)) {
    അവസ്ഥ TOK_END_OF_വരി:
        പോകുക initial;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

mapping_mlvo:
    തിരഞ്ഞെടുപ്പ് (tok = gettok(m, s)) {
    അവസ്ഥ TOK_IDENTIFIER:
        എങ്കിൽ (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        പോകുക mapping_mlvo;
    അവസ്ഥ TOK_EQUALS:
        പോകുക mapping_kccgst;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

mapping_kccgst:
    തിരഞ്ഞെടുപ്പ് (tok = gettok(m, s)) {
    അവസ്ഥ TOK_IDENTIFIER:
        എങ്കിൽ (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        പോകുക mapping_kccgst;
    അവസ്ഥ TOK_END_OF_വരി:
        എങ്കിൽ (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            എങ്കിൽ (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_size(m->pending_kccgst.buffer) = 0;
                darray_size(m->pending_kccgst.slices) = 0;
            }
        }
        പോകുക rule_mlvo_first;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

rule_mlvo_first:
    തിരഞ്ഞെടുപ്പ് (tok = gettok(m, s)) {
    അവസ്ഥ TOK_BANG:
        matcher_append_pending_kccgst(m);
        പോകുക bang;
    അവസ്ഥ TOK_END_OF_വരി:
        പോകുക rule_mlvo_first;
    അവസ്ഥ TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        പോകുക finish;
    സ്ഥിരസ്ഥിതി:
        matcher_rule_start_new(m);
        പോകുക rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    തിരഞ്ഞെടുപ്പ് (tok) {
    അവസ്ഥ TOK_IDENTIFIER:
        എങ്കിൽ (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        പോകുക rule_mlvo;
    അവസ്ഥ TOK_WILD_CARD_STAR:
        എങ്കിൽ (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        പോകുക rule_mlvo;
    അവസ്ഥ TOK_WILD_CARD_NONE:
        എങ്കിൽ (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        പോകുക rule_mlvo;
    അവസ്ഥ TOK_WILD_CARD_SOME:
        എങ്കിൽ (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        പോകുക rule_mlvo;
    അവസ്ഥ TOK_WILD_CARD_ANY:
        എങ്കിൽ (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        പോകുക rule_mlvo;
    അവസ്ഥ TOK_GROUP_NAME:
        എങ്കിൽ (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        പോകുക rule_mlvo;
    അവസ്ഥ TOK_EQUALS:
        പോകുക rule_kccgst;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

rule_kccgst:
    തിരഞ്ഞെടുപ്പ് (tok = gettok(m, s)) {
    അവസ്ഥ TOK_IDENTIFIER:
        എങ്കിൽ (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        പോകുക rule_kccgst;
    അവസ്ഥ TOK_END_OF_വരി:
        എങ്കിൽ (!m->rule.skip)
            matcher_rule_verify(m, s);
        എങ്കിൽ (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        പോകുക rule_mlvo_first;
    സ്ഥിരസ്ഥിതി:
        പോകുക unexpected;
    }

unexpected:
    തിരഞ്ഞെടുപ്പ് (tok) {
    അവസ്ഥ TOK_ERROR:
        പോകുക error;
    സ്ഥിരസ്ഥിതി:
        പോകുക state_error;
    }

finish:
    തിരികെ true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    തിരികെ false;
}
// NOLINTEND(readability-trivial-switch)

സ്ഥിതം bool
read_rules_file(ഘടന xkb_context *ctx,
                ഘടന matcher *matcher,
                unsigned പൂർണ്ണസംഖ്യ include_depth,
                FILE *file,
                സ്ഥിരം അക്ഷരം *path)
{
    bool ret;
    അക്ഷരം *string;
    size_t size;
    ഘടന scanner scanner;

    എങ്കിൽ (!map_file(file, &string, &size)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, strerror(errno));
        തിരികെ false;
    }

    scanner_init(&scanner, matcher->ctx, string, size, path, ശൂന്യ);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    എങ്കിൽ (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, size);
        തിരികെ false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, size, path);
    unmap_file(string, size);
    തിരികെ ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
സ്ഥിതം bool
xkb_resolve_partial_rules(ഘടന xkb_context *ctx, അക്ഷരം *path, size_t path_size,
                          സ്ഥിരം അക്ഷരം* rules, സ്ഥിരം അക്ഷരം* suffix,
                          ഘടന matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    അക്ഷരം partial_rules[60]; /* Arbitrary, but we do not expect long names */
    എങ്കിൽ (unlikely(!snprintf_safe(partial_rules, വലുപ്പം(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        തിരികെ false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    unsigned പൂർണ്ണസംഖ്യ offset = 0;
    FILE *file = ശൂന്യ;
    സ്ഥിരം size_t len = strlen(partial_rules);
    ആകുമ്പോൾ ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_size, &offset, false)) != ശൂന്യ) {
        സ്ഥിരം bool ok = read_rules_file(ctx, matcher, 0, file, path);
        fclose(file);
        എങ്കിൽ (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            തിരികെ false;
        }
        offset++;
    }

    തിരികെ true;
}

സ്ഥിതം bool
xkb_resolve_rules(ഘടന xkb_context *ctx,
                  സ്ഥിരം അക്ഷരം* rules, ഘടന matcher *matcher,
                  ഘടന xkb_component_names *out,
                  xkb_layout_index_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    unsigned പൂർണ്ണസംഖ്യ offset = 0;
    അക്ഷരം path[PATH_MAX];
    FILE * സ്ഥിരം file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, strlen(rules), FILE_TYPE_RULES,
                                          path, വലുപ്പം(path), &offset, true);
    എങ്കിൽ (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        പോകുക err_out;
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
    ret = xkb_resolve_partial_rules(ctx, path, വലുപ്പം(path),
                                    rules, ".pre", matcher);
    എങ്കിൽ (!ret)
        പോകുക err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    എങ്കിൽ (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        പോകുക err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, വലുപ്പം(path),
                                    rules, ".post", matcher);
    എങ്കിൽ (!ret)
        പോകുക err_out;

    /* Check we got required components */
    എങ്കിൽ (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        പോകുക err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, ശൂന്യ);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, ശൂന്യ);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, ശൂന്യ);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, ശൂന്യ);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, ശൂന്യ);

    ഘടന matched_sval *mval = &matcher->rmlvo.model;
    എങ്കിൽ (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_മൂല്യം,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (unsigned പൂർണ്ണസംഖ്യ) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        എങ്കിൽ (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_മൂല്യം,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned പൂർണ്ണസംഖ്യ) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        എങ്കിൽ (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_മൂല്യം,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned പൂർണ്ണസംഖ്യ) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        എങ്കിൽ (!mval->sval.len)
            തുടരുക;

        എങ്കിൽ (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_മൂല്യം,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned പൂർണ്ണസംഖ്യ) mval->sval.len, mval->sval.start, rules);
        } അല്ലെങ്കിൽ എങ്കിൽ (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            സ്ഥിരം xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            പ്രതിജ്ഞാ(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_മൂല്യം,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (unsigned പൂർണ്ണസംഖ്യ)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    എങ്കിൽ (out->symbols != ശൂന്യ && explicit_layouts != ശൂന്യ) {
        *explicit_layouts = 1; /* at least one group */
        സ്ഥിരം അക്ഷരം *symbols = out->symbols;
        /* Take the highest modifier */
        ആകുമ്പോൾ ((symbols = strchr(symbols, ':')) != ശൂന്യ && symbols[1] != '\0') {
            xkb_layout_index_t group = 0;
            സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            എങ്കിൽ (എണ്ണം > 0 && (symbols[എണ്ണം] == '\0' ||
                is_merge_mode_prefix(symbols[എണ്ണം])) &&
                group > 0 && group <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, group);
                symbols += എണ്ണം;
            }
        }
    }

err_out:
    എങ്കിൽ (file)
        fclose(file);

    തിരികെ ret;
}

bool
xkb_components_from_rmlvo_builder(സ്ഥിരം ഘടന xkb_rmlvo_builder *rmlvo,
                                  ഘടന xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts)
{
    സ്ഥിരം അക്ഷരം *rules = rmlvo->rules;
    ഘടന matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    എങ്കിൽ (!matcher)
        തിരികെ false;

    സ്ഥിരം bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    തിരികെ ret;
}


bool
xkb_components_from_rules_names(ഘടന xkb_context *ctx,
                                സ്ഥിരം ഘടന xkb_rule_names *rmlvo,
                                ഘടന xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts)
{
    ഘടന matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    എങ്കിൽ (!matcher)
        തിരികെ false;

    സ്ഥിരം bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    തിരികെ ret;
}
