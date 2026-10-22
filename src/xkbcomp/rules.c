/*
 * For HPND:
 * Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "rules.h"
#ଅନ୍ତର୍ଭାଵ "rmlvo.h"
#ଅନ୍ତର୍ଭାଵ "include.h"
#ଅନ୍ତର୍ଭାଵ "scanner-utils.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "util-numbers.h"
#ଅନ୍ତର୍ଭାଵ "util-paths.h"

#define MAX_INCLUDE_DEPTH 5

/* Scanner / Lexer */

/* Values returned with some tokens, like yylval. */
ସଂଘ lvalue {
    ସଂରଚନା sval string;
};

ଗଣନା rules_token {
    TOK_END_OF_FILE = 0,
    TOK_END_OF_ଧାଡ଼ି,
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

ସ୍ଥିର ଗଣନା rules_token
lex(ସଂରଚନା scanner *s, ସଂଘ lvalue *val)
{
skip_more_whitespace_and_comments:
    /* Skip spaces */
    ଯେତେବେଳେ (scanner_chr(s, ' ') || scanner_chr(s, '\t') || scanner_chr(s, '\r'));

    /* Skip comments */
    ଯଦି (scanner_lit(s, "//")) {
        scanner_skip_to_eol(s);
    }

    /* New line */
    ଯଦି (scanner_eol(s)) {
        ଯେତେବେଳେ (scanner_eol(s)) scanner_next(s);
        ଫେରନ୍ତୁ TOK_END_OF_ଧାଡ଼ି;
    }

    /* Escaped line continuation */
    ଯଦି (scanner_chr(s, '\\')) {
        /* Optional \r. */
        scanner_chr(s, '\r');
        ଯଦି (!scanner_eol(s)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "illegal new line escape; must appear at end of line");
            ଫେରନ୍ତୁ TOK_ERROR;
        }
        scanner_next(s);
        ଯାଅ skip_more_whitespace_and_comments;
    }

    /* See if we're done */
    ଯଦି (scanner_eof(s)) ଫେରନ୍ତୁ TOK_END_OF_FILE;

    /* New token */
    s->token_pos = s->pos;
    ଗଣନା rules_token token = TOK_IDENTIFIER;

    ଅଚଳ ଅକ୍ଷର next = scanner_next(s);
    ବାଛ (next) {
    ସ୍ଥିତି '!':
        ଫେରନ୍ତୁ TOK_BANG;
    ସ୍ଥିତି '$':
        /* Group */
        token = TOK_GROUP_NAME;
        val->string.len = 0;
        ଭାଙ୍ଗ;
    ସ୍ଥିତି '*':
        ଯଦି (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            ଭାଙ୍ଗ;
        } ଅନ୍ୟଥା {
            /* Legacy wild card */
            ଫେରନ୍ତୁ TOK_WILD_CARD_STAR;
        }
    ସ୍ଥିତି '<':
        /* Extended wild cards */
        ଯଦି (scanner_rules_lit_token(s, "none>")) ଫେରନ୍ତୁ TOK_WILD_CARD_NONE;
        ଯଦି (scanner_rules_lit_token(s, "some>")) ଫେରନ୍ତୁ TOK_WILD_CARD_SOME;
        ଯଦି (scanner_rules_lit_token(s, "any>")) ଫେରନ୍ତୁ TOK_WILD_CARD_ANY;
        /* No match: continue lax parsing */
        ପ୍ରତିଜ୍ଞା(scanner_rules_is_ident('<'));
        val->string.len = 1;
        ଭାଙ୍ଗ;
    ସ୍ଥିତି '=':
        ଯଦି (scanner_rules_is_ident(scanner_peek(s))) {
            /* Backtrack */
            s->pos--;
            ଭାଙ୍ଗ;
        } ଅନ୍ୟଥା {
            ଫେରନ୍ତୁ TOK_EQUALS;
        }
    ସ୍ଥିତି 'i':
        ଯଦି (scanner_rules_lit_token(s, "nclude")) {
            /* Include statement */
            ଫେରନ୍ତୁ TOK_INCLUDE;
        }
        /* Identifier */
        ପ୍ରତିଜ୍ଞା(scanner_rules_is_ident('i'));
        val->string.len = 1;
        ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (likely(scanner_rules_is_ident(next))) {
            /* Identifier */
            val->string.len = 1;
        } ଅନ୍ୟଥା {
            /* Invalid */
            s->pos--;
            val->string.len = 0;
        }
    }

    /* Identifier */
    val->string.start = s->s + s->pos - val->string.len;
    /* Ensure that we can parse KcCGST values with merge modes */
    ପ୍ରତିଜ୍ଞା(scanner_rules_is_ident(MERGE_OVERRIDE_PREFIX));
    ପ୍ରତିଜ୍ଞା(scanner_rules_is_ident(MERGE_AUGMENT_PREFIX));
    ପ୍ରତିଜ୍ଞା(scanner_rules_is_ident(MERGE_REPLACE_PREFIX));
    ପ୍ରତିଜ୍ଞା(scanner_rules_is_ident('('));
    ପ୍ରତିଜ୍ଞା(scanner_rules_is_ident(')'));
    val->string.len += scanner_rules_ident_token(s);

    ଯଦି (unlikely(val->string.len == 0)) {
        ଯଦି (token == TOK_GROUP_NAME) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unexpected character after \'$\'; expected name");
        } ଅନ୍ୟଥା {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "unrecognized token");
        }
        ଫେରନ୍ତୁ TOK_ERROR;
    }

    ଫେରନ୍ତୁ token;
}

/***====================================================================***/

ଗଣନା rules_mlvo {
    MLVO_MODEL,
    MLVO_LAYOUT,
    MLVO_VARIANT,
    MLVO_OPTION,
    MLVO_ଅଙ୍କ_ENTRIES
};

ଗଣନା {
    MLVO_LAYOUT_AND_VARIANT = ((1 << MLVO_LAYOUT) | (1 << MLVO_VARIANT))
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା uint8_t mlvo_index_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା uint8_t mlvo_mask_t;


ସ୍ଥିର ଅଚଳ ସଂରଚନା sval rules_mlvo_svals[MLVO_ଅଙ୍କ_ENTRIES] = {
    [MLVO_MODEL] = SVAL_INIT("model"),
    [MLVO_LAYOUT] = SVAL_INIT("layout"),
    [MLVO_VARIANT] = SVAL_INIT("variant"),
    [MLVO_OPTION] = SVAL_INIT("option"),
};

ଗଣନା rules_kccgst {
    KCCGST_KEYCODES,
    KCCGST_TYPES,
    KCCGST_COMPAT,
    KCCGST_SYMBOLS,
    KCCGST_GEOMETRY,
    KCCGST_ଅଙ୍କ_ENTRIES
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା uint8_t kccgst_index_t;
ପ୍ରକାରବ୍ୟାଖ୍ୟା uint8_t kccgst_mask_t;

ସ୍ଥିର ଅଚଳ ସଂରଚନା sval rules_kccgst_svals[KCCGST_ଅଙ୍କ_ENTRIES] = {
    [KCCGST_KEYCODES] = SVAL_INIT("keycodes"),
    [KCCGST_TYPES] = SVAL_INIT("types"),
    [KCCGST_COMPAT] = SVAL_INIT("compat"),
    [KCCGST_SYMBOLS] = SVAL_INIT("symbols"),
    [KCCGST_GEOMETRY] = SVAL_INIT("geometry"),
};

ଗଣନା {
    /** Value of the matched layout mask for layout-independent MLVO fields */
    GLOBAL_MATCHED_LAYOUTS = XKB_ALL_GROUPS,
};

static_assert(~(xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL ==
              (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS,
              "Global layout mask consistency");

/* We use this to keep score whether an mlvo was matched or not; if not,
 * we warn the user that their preference was ignored. */
ସଂରଚନା matched_sval {
    ସଂରଚନା sval sval;
    /** Used for layout-specific options */
    xkb_layout_mask_t layouts;
    xkb_layout_mask_t matched;
};
ପ୍ରକାରବ୍ୟାଖ୍ୟା darray(ସଂରଚନା matched_sval) darray_matched_sval;

/*
 * A broken-down version of xkb_rule_names (without the rules,
 * obviously).
 */
ସଂରଚନା rule_names {
    ସଂରଚନା matched_sval model;
    darray_matched_sval layouts;
    darray_matched_sval variants;
    darray_matched_sval options;
};

ସଂରଚନା group {
    ସଂରଚନା sval name;
    darray_sval elements;
};

ସଂରଚନା mapping {
    ଗଣନା rules_mlvo mlvo_at_pos[MLVO_ଅଙ୍କ_ENTRIES];
    mlvo_index_t ଅଙ୍କ_mlvo;
    mlvo_mask_t defined_mlvo_mask;
    bool has_layout_idx_range;
    bool has_multiple_layouts;
    /* This member has 2 uses:
     * • Keep track of layout and variant indices while parsing MLVO headers.
     * • Store layout/variant range afterwards.
     * Thus provide 2 structs to reflect these semantics in the code. */
    ସଂଘ {
        ସଂରଚନା { xkb_layout_index_t layout_idx, variant_idx; };
        ସଂରଚନା { xkb_layout_index_t layout_idx_min, layout_idx_max; };
    };
    /* This member has 2 uses:
     * • Check if the mapping is active by interpreting the value as a boolean.
     * • Keep track of the remaining layout indices to match.
     * Thus provide 2 names to reflect these semantics in the code. */
    ସଂଘ {
        xkb_layout_mask_t active;
        xkb_layout_mask_t layouts_candidates_mask;
    };
    ଗଣନା rules_kccgst kccgst_at_pos[KCCGST_ଅଙ୍କ_ENTRIES];
    kccgst_index_t ଅଙ୍କ_kccgst;
    kccgst_mask_t defined_kccgst_mask;
};

ଗଣନା mlvo_match_type {
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

ଗଣନା wildcard_match_type {
    /** ‘*’ matches only non-empty strings */
    WILDCARD_MATCH_NONEMPTY = 0,
    /** ‘*’ matches all strings */
    WILDCARD_MATCH_ALL,
};

ସଂରଚନା rule {
    ସଂରଚନା sval mlvo_ମୂଲ୍ୟ_at_pos[MLVO_ଅଙ୍କ_ENTRIES];
    ଗଣନା mlvo_match_type match_type_at_pos[MLVO_ଅଙ୍କ_ENTRIES];
    ସଂରଚନା sval kccgst_ମୂଲ୍ୟ_at_pos[KCCGST_ଅଙ୍କ_ENTRIES];
    mlvo_index_t ଅଙ୍କ_mlvo_ମୂଲ୍ୟଗୁଡ଼ିକ;
    kccgst_index_t ଅଙ୍କ_kccgst_ମୂଲ୍ୟଗୁଡ଼ିକ;
    bool skip;
};

#define SLICE_KCCGST_BIT_FIELD_SIZE 4
static_assert(!((1u << (SLICE_KCCGST_BIT_FIELD_SIZE - 1)) &
                (KCCGST_ଅଙ୍କ_ENTRIES - 1)),
              "Cannot encode KcCGST enum safely (need space for the sign)");
ସଂରଚନା kccgst_buffer_slice {
    uint32_t length:(32 - SLICE_KCCGST_BIT_FIELD_SIZE);
    ଗଣନା rules_kccgst kccgst:SLICE_KCCGST_BIT_FIELD_SIZE;
    xkb_layout_index_t layout;
};

/* Buffer for pending KcCGST values */
ସଂରଚନା kccgst_buffer {
    darray_char buffer;
    /* Slice corresponding to each value in the buffer */
    darray(ସଂରଚନା kccgst_buffer_slice) slices;
};

/*
 * This is the main object used to match a given RMLVO against a rules
 * file and aggregate the results in a KcCGST. It goes through a simple
 * matching state machine, with tokens as transitions (see
 * matcher_match()).
 */
ସଂରଚନା matcher {
    ସଂରଚନା xkb_context *ctx;
    /* Input.*/
    ସଂରଚନା rule_names rmlvo;
    ସଂଘ lvalue val;
    darray(ସଂରଚନା group) groups;
    /* Current mapping. */
    ସଂରଚନା mapping mapping;
    /* Current rule. */
    ସଂରଚନା rule rule;
    /*
     * Buffers for pending KcCGST values. Required in case of using layout
     * index ranges, to ensure that the values are merged in the expected order.
     * See the note: “Layout index ranges and merging KcCGST values”.
     */
    ସଂରଚନା kccgst_buffer pending_kccgst;
    /* Output. */
    darray_char kccgst[KCCGST_ଅଙ୍କ_ENTRIES];
};

ସ୍ଥିର ସଂରଚନା sval
strip_spaces(ସଂରଚନା sval v)
{
    ଯେତେବେଳେ (v.len > 0 && is_space(v.start[0])) { v.len--; v.start++; }
    ଯେତେବେଳେ (v.len > 0 && is_space(v.start[v.len - 1])) v.len--;
    ଫେରନ୍ତୁ v;
}

ସ୍ଥିର darray_matched_sval
split_comma_separated_mlvo(ସଂରଚନା xkb_context *ctx,
                           ଗଣନା rules_mlvo mlvo, ଅଚଳ ଅକ୍ଷର *s)
{
    darray_matched_sval arr = darray_new();

    /*
     * Make sure the array returned by this function always includes at
     * least one value, e.g. "" -> { "" } and "," -> { "", "" }.
     */

    ଯଦି (!s) {
        ensure_at_least_one_ମୂଲ୍ୟ:
        /* Label followed by a declaration is a C23 extension */ ;

        ସଂରଚନା matched_sval val = {
            .sval = SVAL(ଖାଲି, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0
        };
        darray_append(arr, val);
        ଫେରନ୍ତୁ arr;
    }

    ଯେତେବେଳେ (true) {
        ସଂରଚନା matched_sval val = {
            .sval = SVAL(s, 0),
            .layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL,
            .matched = 0,
        };
        ଯେତେବେଳେ (*s != '\0' && *s != ',' && *s != OPTIONS_GROUP_SPECIFIER_PREFIX) {
            s++;
            val.sval.len++;
        }
        val.sval = strip_spaces(val.sval);

        ଯଦି (*s == OPTIONS_GROUP_SPECIFIER_PREFIX) {
            /* Handle numeric layout index */
            s++;
            ଅଚଳ ଅକ୍ଷର* ଅଚଳ layout_start = s;
            xkb_layout_index_t layout = XKB_LAYOUT_INVALID;

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = parse_dec_to_uint32_t(s, SIZE_MAX, &layout);
            ଯଦି (ଗଣନା_2 > 0) {
                /* Note: 1-indexed layout */
                s += ଗଣନା_2;
                static_assert(XKB_MAX_GROUPS == 32, "Invalid shift");
                ଯଦି (layout == 0 || layout > XKB_MAX_GROUPS) {
                    log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                            "Invalid layout index %"PRIu32" "
                            "for the RMVLO component: \"%.*s\"\n", layout,
                            (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) val.sval.len, val.sval.start);
                } ଅନ୍ୟଥା ଯଦି (mlvo != MLVO_OPTION) {
                    log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                             "Layout index %"PRIu32" is not supported for "
                             "the RMLVO component: \"%.*s\"\n", layout,
                             (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) val.sval.len, val.sval.start);
                } ଅନ୍ୟଥା {
                    val.layouts = (UINT32_C(1) << (layout - 1));
                }
            }

            /* Consume until reaching next item or end of string */
            ଅଚଳ ଅକ୍ଷର* ଅଚଳ layout_index_end = s;
            ଯେତେବେଳେ (*s != '\0' && *s != ',') { s++; }
            ଯଦି (ଗଣନା_2 <= 0 || layout_index_end != s) {
                log_err(ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Invalid layout index \"%.*s\" for the RMLVO "
                        "component \"%.*s\"; discarding specifier.\n",
                        (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (s - layout_start), layout_start,
                        (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) val.sval.len, val.sval.start);
            }
        }

        ଯଦି (val.sval.len || mlvo != MLVO_OPTION) {
            ଯଦି (mlvo == MLVO_OPTION) {
                /* Check for previous entry */
                ସଂରଚନା matched_sval *prev;
                darray_foreach(prev, arr) {
                    ଯଦି (svaleq(prev->sval, val.sval)) {
                        /* Merge options */
                        ଯଦି (val.layouts ==
                            (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Global option overrides layout-specific ones */
                            prev->layouts = val.layouts;
                        } ଅନ୍ୟଥା ଯଦି (prev->layouts != (xkb_layout_mask_t)
                                   XKB_OPTION_LAYOUT_MASK_GLOBAL) {
                            /* Layout-specific option cannot override global */
                            prev->layouts |= val.layouts;
                        }
                        ଯାଅ next_ମୂଲ୍ୟ;
                    }
                }
            }
            darray_append(arr, val);
        }

next_ମୂଲ୍ୟ:
        ଯଦି (*s == '\0') ଭାଙ୍ଗ;
        ଯଦି (*s == ',') s++;
    }

    ଯଦି (darray_empty(arr))
        ଯାଅ ensure_at_least_one_ମୂଲ୍ୟ;

    ଫେରନ୍ତୁ arr;
}

ସ୍ଥିର ସଂରଚନା matcher *
matcher_new_from_rmlvo(ଅଚଳ ସଂରଚନା xkb_rmlvo_builder *rmlvo, ଅଚଳ ଅକ୍ଷର **rules)
{
    ସଂରଚନା matcher *m = calloc(1, ଆକାର(*m));
    ଯଦି (!m)
        ଫେରନ୍ତୁ ଖାଲି;

    m->ctx = rmlvo->ctx;

    /* Sanitize */
    ସଂରଚନା xkb_rule_names names = {
        .rules = rmlvo->rules,
        .model = rmlvo->model,
        /* Initial values only used to detect missing entries */
        .layout = (darray_empty(rmlvo->layouts)) ? ଖାଲି : "x",
        .variant = (darray_empty(rmlvo->layouts)) ? ଖାଲି : "x",
        .options = (darray_empty(rmlvo->options)) ? ଖାଲି : "x",
    };
    ଅଚଳ ଗଣନା RMLVO changed = xkb_context_sanitize_rule_names(rmlvo->ctx,
                                                               &names);

    /* Initialize matcher with sanitized names, if relevant */
    ଯଦି (changed & RMLVO_RULES) {
        *rules = names.rules;
    } ଅନ୍ୟଥା {
        *rules = rmlvo->rules;
    }

    ଯଦି (changed & RMLVO_MODEL) {
        m->rmlvo.model.sval.start = names.model;
    } ଅନ୍ୟଥା {
        m->rmlvo.model.sval.start = rmlvo->model;
    }
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;

    ପ୍ରତିଜ୍ଞା((changed & RMLVO_LAYOUT) || !(changed & RMLVO_VARIANT));
    ଯଦି (changed & RMLVO_LAYOUT) {
        /* Layout and variant are tied together */
        m->rmlvo.layouts = split_comma_separated_mlvo(rmlvo->ctx, MLVO_LAYOUT,
                                                      names.layout);
        m->rmlvo.variants = split_comma_separated_mlvo(rmlvo->ctx, MLVO_VARIANT,
                                                       names.variant);
        ଯଦି (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
            /* Do not warn if no variants was provided */
            ଯଦି (!isempty(names.variant))
                log_warn(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "More layouts than variants: \"%s\" vs. \"%s\".\n",
                         names.layout ? names.layout : "(none)",
                         names.variant ? names.variant : "(none)");
            darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        } ଅନ୍ୟଥା ଯଦି (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
            log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                    names.layout ? names.layout : "(none)",
                    names.variant ? names.variant : "(none)");
            darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
            darray_shrink(m->rmlvo.variants);
        }
    } ଅନ୍ୟଥା {
        ସଂରଚନା xkb_rmlvo_builder_layout *layout;
        darray_foreach(layout, rmlvo->layouts) {
            ସଂରଚନା matched_sval val = {
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

    ଯଦି (changed & RMLVO_OPTIONS) {
        m->rmlvo.options = split_comma_separated_mlvo(rmlvo->ctx, MLVO_OPTION,
                                                      names.options);
    } ଅନ୍ୟଥା {
        ସଂରଚନା xkb_rmlvo_builder_option *option;
        darray_foreach(option, rmlvo->options) {
            ସଂରଚନା matched_sval val = {
                .sval = SVAL(option->option, strlen_safe(option->option)),
                .layouts = option->layouts,
                .matched = 0
            };
            darray_append(m->rmlvo.options, val);
        }
    }

    ଫେରନ୍ତୁ m;
}

ସ୍ଥିର ସଂରଚନା matcher *
matcher_new_from_names(ସଂରଚନା xkb_context *ctx,
            ଅଚଳ ସଂରଚନା xkb_rule_names *rmlvo)
{
    ସଂରଚନା matcher *m = calloc(1, ଆକାର(*m));
    ଯଦି (!m)
        ଫେରନ୍ତୁ ଖାଲି;

    m->ctx = ctx;
    m->rmlvo.model.sval.start = rmlvo->model;
    m->rmlvo.model.sval.len = strlen_safe(rmlvo->model);
    m->rmlvo.model.layouts = (xkb_layout_mask_t)XKB_OPTION_LAYOUT_MASK_GLOBAL;
    m->rmlvo.layouts = split_comma_separated_mlvo(ctx, MLVO_LAYOUT, rmlvo->layout);
    m->rmlvo.variants = split_comma_separated_mlvo(ctx, MLVO_VARIANT, rmlvo->variant);
    m->rmlvo.options = split_comma_separated_mlvo(ctx, MLVO_OPTION, rmlvo->options);

    ଯଦି (darray_size(m->rmlvo.layouts) > darray_size(m->rmlvo.variants)) {
        /* Do not warn if no variants was provided */
        ଯଦି (!isempty(rmlvo->variant))
            log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "More layouts than variants: \"%s\" vs. \"%s\".\n",
                     rmlvo->layout ? rmlvo->layout : "(none)",
                     rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize0(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
    } ଅନ୍ୟଥା ଯଦି (darray_size(m->rmlvo.layouts) < darray_size(m->rmlvo.variants)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Less layouts than variants: \"%s\" vs. \"%s\".\n",
                rmlvo->layout ? rmlvo->layout : "(none)",
                rmlvo->variant ? rmlvo->variant : "(none)");
        darray_resize(m->rmlvo.variants, darray_size(m->rmlvo.layouts));
        darray_shrink(m->rmlvo.variants);
    }

    ଫେରନ୍ତୁ m;
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_free(ସଂରଚନା matcher *m)
{
    ଯଦି (!m)
        ଫେରନ୍ତୁ;
    darray_free(m->rmlvo.layouts);
    darray_free(m->rmlvo.variants);
    darray_free(m->rmlvo.options);
    ସଂରଚନା group *group;
    darray_foreach(group, m->groups)
        darray_free(group->elements);
    darray_free(m->pending_kccgst.buffer);
    darray_free(m->pending_kccgst.slices);
    ପାଇଁ (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_ଅଙ୍କ_ENTRIES; i++)
        darray_free(m->kccgst[i]);
    darray_free(m->groups);
    ମୁକ୍ତ(m);
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_group_start_new(ସଂରଚନା matcher *m, ସଂରଚନା sval name)
{
    ସଂରଚନା group group = { .name = name, .elements = darray_new() };
    darray_append(m->groups, group);
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_group_add_element(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
                          ସଂରଚନା sval element)
{
    darray_append(darray_item(m->groups, darray_size(m->groups) - 1).elements,
                  element);
}

ସ୍ଥିର bool
read_rules_file(ସଂରଚନା xkb_context *ctx,
                ସଂରଚନା matcher *matcher,
                unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth,
                FILE *file,
                ଅଚଳ ଅକ୍ଷର *path);

ସ୍ଥିର ଶୂନ୍ୟ
matcher_include(ସଂରଚନା matcher *m, ସଂରଚନା scanner *parent_scanner,
                unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth,
                ସଂରଚନା sval inc)
{
    ଯଦି (include_depth >= MAX_INCLUDE_DEPTH) {
        scanner_err(parent_scanner, XKB_LOG_MESSAGE_NO_ID,
                    "maximum include depth (%u) exceeded; "
                    "maybe there is an include loop?",
                    MAX_INCLUDE_DEPTH);
        ଫେରନ୍ତୁ;
    }

    ଅଚଳ ଅକ୍ଷର *stmt_file = inc.start;
    size_t stmt_file_len = inc.len;

    /* Process %-expansion, if any */
    ଅକ୍ଷର buf[PATH_MAX] = {0};
    ଅଚଳ ssize_t expanded = expand_path(m->ctx, parent_scanner->file_name,
                                         stmt_file, stmt_file_len,
                                         FILE_TYPE_RULES,
                                         buf, ଆକାର(buf));
    ଯଦି (expanded < 0) {
        /* Error */
        ଫେରନ୍ତୁ;
    } ଅନ୍ୟଥା ଯଦି (expanded > 0) {
        /* %-expanded */
        stmt_file = buf;
        stmt_file_len = (size_t) expanded;
        ପ୍ରତିଜ୍ଞା(stmt_file[stmt_file_len] == '\0');
    }

    /* Lookup the first candidate */
    FILE* file;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    ଅଚଳ bool absolute_path = is_absolute_path(stmt_file);
    ଯଦି (absolute_path) {
        /* Absolute path: no need for lookup in XKB paths */
        ଯଦି (!expanded) {
            /* No %expansion: ensure it’s NULL-terminated */
            ଯଦି (stmt_file_len < ଆକାର(buf)) {
                memcpy(buf, stmt_file, stmt_file_len);
                buf[stmt_file_len] = '\0';
                stmt_file = buf;
            } ଅନ୍ୟଥା {
                log_err(m->ctx, XKB_ERROR_INVALID_PATH,
                        "Path is too long: %zu > %zu, got raw path: %.*s\n",
                        stmt_file_len, ଆକାର(buf),
                        (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) stmt_file_len, stmt_file);
                ଫେରନ୍ତୁ;
            }
        } ଅନ୍ୟଥା {
            /* %-expansion is always NULL-terminated */
            ପ୍ରତିଜ୍ଞା(stmt_file[stmt_file_len] == '\0');
        }
        file = fopen(stmt_file, "rb");
    } ଅନ୍ୟଥା {
        /* Relative path: lookup the first XKB path */
        ଯଦି (unlikely(expanded)) {
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
            file = ଖାଲି;
        } ଅନ୍ୟଥା {
            file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                     stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                     buf, ଆକାର(buf), &offset, true);
        }
    }

    ଯେତେବେଳେ (file) {
        ପ୍ରତିଜ୍ଞା(strlen_safe(buf) < ଆକାର(buf));
        bool ret = read_rules_file(m->ctx, m, include_depth + 1, file, buf);
        fclose(file);
        ଯଦି (ret)
            ଫେରନ୍ତୁ;
        /* Failed to parse rules or get all the components */
        log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
                "No components returned from included XKB rules \"%s\"\n",
                buf);

        ଯଦି (absolute_path) {
            /* There is no point to search further if the path is absolute */
            ଭାଙ୍ଗ;
        }

        /* Try next XKB path */
        offset++;
        file = FindFileInXkbPath(m->ctx, parent_scanner->file_name,
                                 stmt_file, stmt_file_len, FILE_TYPE_RULES,
                                 buf, ଆକାର(buf), &offset, true);
    }

    log_err(m->ctx, XKB_LOG_MESSAGE_NO_ID,
            "Failed to open included XKB rules \"%.*s\"\n",
            (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) stmt_file_len, stmt_file);
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_mapping_start_new(ସଂରଚନା matcher *m)
{
    ପାଇଁ (mlvo_index_t i = 0; i < (mlvo_index_t) MLVO_ଅଙ୍କ_ENTRIES; i++)
        m->mapping.mlvo_at_pos[i] = MLVO_ଅଙ୍କ_ENTRIES;
    ପାଇଁ (kccgst_index_t i = 0; i < (kccgst_index_t) KCCGST_ଅଙ୍କ_ENTRIES; i++)
        m->mapping.kccgst_at_pos[i] = KCCGST_ଅଙ୍କ_ENTRIES;
    m->mapping.has_multiple_layouts = false;
    m->mapping.layout_idx = m->mapping.variant_idx = XKB_LAYOUT_INVALID;
    m->mapping.ଅଙ୍କ_mlvo = m->mapping.ଅଙ୍କ_kccgst = 0;
    m->mapping.defined_mlvo_mask = 0;
    m->mapping.defined_kccgst_mask = 0;
    m->mapping.active = true;
}

/** Caller must check prefix `[` and initialize `out` to XKB_LAYOUT_INVALID */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
parse_layout_int_index_tail(ଅଚଳ ଅକ୍ଷର *s, size_t max_len,
                            xkb_layout_index_t *out)
{
    ଯଦି (max_len < 3)
        ଫେରନ୍ତୁ -1;
    uint32_t val = 0;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = parse_dec_to_uint32_t(&s[1], max_len - 2, &val);
    ଯଦି (ଗଣନା_2 <= 0 || s[1 + ଗଣନା_2] != ']' || val == 0 || val > XKB_MAX_GROUPS)
        ଫେରନ୍ତୁ -1;
    /* To zero-based index. */
    *out = val - 1;
    ଫେରନ୍ତୁ ଗଣନା_2 + 2; /* == length "[index]" */
}

/* Parse Kccgst layout index:
 * "[%i]" or "[n]", where "n" is a decimal number */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
extract_layout_index(ଅଚଳ ଅକ୍ଷର *s, size_t max_len, xkb_layout_index_t *out)
{
    /* This function is pretty stupid, but works for now. */
    *out = XKB_LAYOUT_INVALID;
    ଯଦି (max_len < 3 || s[0] != '[')
        ଫେରନ୍ତୁ -1;
    ଯଦି (s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        /* Small numeric index */
        *out = (xkb_layout_index_t)(s[1] - '1');
        ଫେରନ୍ତୁ 3;
    }
    ଯଦି (max_len > 3 && s[1] == '%' && s[2] == 'i' && s[3] == ']') {
        /* Special index: %i */
        ଫେରନ୍ତୁ 4; /* == length "[%i]" */
    }
    /* Fallback: any numeric index (unlikely) */
    ଫେରନ୍ତୁ parse_layout_int_index_tail(s, max_len, out);
}

/* Special layout indices */
ଗଣନା layout_index_ranges {
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
ସ୍ଥିର bool
extract_mapping_layout_index(ଅଚଳ ଅକ୍ଷର *s, size_t max_len,
                             xkb_layout_index_t *out)
{
    /* Check for minimal `[` + index + `]` */
    ଯଦି (max_len < 3 || s[0] != '[') {
        *out = XKB_LAYOUT_INVALID;
        ଫେରନ୍ତୁ false;
    }

    /* Small numeric index */
    ଯଦି (max_len == 3 && s[2] == ']' && s[1] >= '1' && s[1] <= '9') {
        *out = (xkb_layout_index_t)(s[1] - '1');
        ଫେରନ୍ତୁ true;
    }

    /* Named indices ranges */
    ସ୍ଥିର ଅଚଳ ସଂରଚନା {
        ଅଚଳ ଅକ୍ଷର* name;
        size_t size;
        xkb_layout_index_t ମୂଲ୍ୟ;
    } names[] = {
        #define INDEX_RANGE(tail, ମୂଲ୍ୟ) \
            { (tail), ଆକାର(tail), (xkb_layout_index_t)(ମୂଲ୍ୟ) }
        INDEX_RANGE("single]",   LAYOUT_INDEX_SINGLE),
        INDEX_RANGE("first]",    LAYOUT_INDEX_FIRST),
        INDEX_RANGE("later]",    LAYOUT_INDEX_LATER),
        INDEX_RANGE("multiple]", LAYOUT_INDEX_MULTIPLE),
        INDEX_RANGE("any]",      LAYOUT_INDEX_ANY),
        #undef INDEX_RANGE
    };

    ପାଇଁ (size_t k = 0; k < ARRAY_SIZE(names); k++) {
        ଯଦି (max_len == names[k].size &&
            memcmp(&s[1], names[k].name, names[k].size - 1) == 0) {
            *out = names[k].ମୂଲ୍ୟ;
            ଫେରନ୍ତୁ true;
        }
    }

    /* Fallback: any numeric index (unlikely) */
    *out = XKB_LAYOUT_INVALID;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = parse_layout_int_index_tail(s, max_len, out);
    ଫେରନ୍ତୁ ଗଣନା_2 > 0 && (size_t)ଗଣନା_2 == max_len;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
is_mlvo_mask_defined(ସଂରଚନା matcher *m, ଗଣନା rules_mlvo mlvo)
{
    ଫେରନ୍ତୁ m->mapping.defined_mlvo_mask & (1u << mlvo);
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_mapping_set_mlvo(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
                         ସଂରଚନା sval ident)
{
    ଗଣନା rules_mlvo mlvo;
    ସଂରଚନା sval mlvo_sval;

    ପାଇଁ (mlvo = 0; mlvo < MLVO_ଅଙ୍କ_ENTRIES; mlvo++) {
        mlvo_sval = rules_mlvo_svals[mlvo];

        ଯଦି (svaleq_prefix(mlvo_sval, ident))
            ଭାଙ୍ଗ;
    }

    /* Not found. */
    ଯଦି (mlvo >= MLVO_ଅଙ୍କ_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ident.len, ident.start);
        m->mapping.active = false;
        ଫେରନ୍ତୁ;
    }

    ଯଦି (is_mlvo_mask_defined(m, mlvo)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mlvo_sval.len, mlvo_sval.start);
        m->mapping.active = false;
        ଫେରନ୍ତୁ;
    }

    /* If there are leftovers still, it must be an index. */
    ଯଦି (mlvo_sval.len < ident.len) {
        xkb_layout_index_t idx;
        ଯଦି (!extract_mapping_layout_index(ident.start + mlvo_sval.len,
                                          ident.len - mlvo_sval.len, &idx)) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" may only be followed by a "
                        "valid group index; ignoring rule set",
                        (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            ଫେରନ୍ତୁ;
        }

        ଯଦି (mlvo == MLVO_LAYOUT) {
            m->mapping.layout_idx = idx;
        }
        ଅନ୍ୟଥା ଯଦି (mlvo == MLVO_VARIANT) {
            m->mapping.variant_idx = idx;
        }
        ଅନ୍ୟଥା {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid mapping: \"%.*s\" cannot be followed by a group "
                        "index; ignoring rule set",
                        (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mlvo_sval.len, mlvo_sval.start);
            m->mapping.active = false;
            ଫେରନ୍ତୁ;
        }
    } ଅନ୍ୟଥା ଯଦି (mlvo == MLVO_LAYOUT) {
        m->mapping.layout_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    } ଅନ୍ୟଥା ଯଦି (mlvo == MLVO_VARIANT) {
        m->mapping.variant_idx = (xkb_layout_index_t) LAYOUT_INDEX_SINGLE;
    }

    /* Check that if both layout and variant are defined, then they must have
     * the same index */
    ଯଦି (((mlvo == MLVO_LAYOUT && is_mlvo_mask_defined(m, MLVO_VARIANT)) ||
         (mlvo == MLVO_VARIANT && is_mlvo_mask_defined(m, MLVO_LAYOUT))) &&
        m->mapping.layout_idx != m->mapping.variant_idx) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"layout\" index must be the same as the "
                    "\"variant\" index");
        m->mapping.active = false;
        ଫେରନ୍ତୁ;
    }

    m->mapping.mlvo_at_pos[m->mapping.ଅଙ୍କ_mlvo] = mlvo;
    m->mapping.defined_mlvo_mask |= (mlvo_mask_t) 1u << mlvo;
    m->mapping.ଅଙ୍କ_mlvo++;
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_mapping_set_layout_bounds(ସଂରଚନା matcher *m)
{
    /* Handle case where one of the index is XKB_LAYOUT_INVALID */
    xkb_layout_index_t idx = MIN(m->mapping.layout_idx, m->mapping.variant_idx);
    ବାଛ (idx) {
        ସ୍ଥିତି XKB_LAYOUT_INVALID:
            /* No layout nor variant */
            ପ୍ରତିଜ୍ଞା(!is_mlvo_mask_defined(m, MLVO_LAYOUT) &&
                   !is_mlvo_mask_defined(m, MLVO_VARIANT));
            m->mapping.has_layout_idx_range = false;
            m->mapping.layout_idx_min = XKB_LAYOUT_INVALID;
            m->mapping.layout_idx_max = m->mapping.layout_idx_min;
            m->mapping.layouts_candidates_mask = 0x1; /* active = true */
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LAYOUT_INDEX_LATER:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 1;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All but the first layout */
                ((UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1)) &
                ~UINT64_C(1);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LAYOUT_INDEX_MULTIPLE:
        ସ୍ଥିତି LAYOUT_INDEX_ANY:
            m->mapping.has_layout_idx_range = true;
            m->mapping.layout_idx_min = 0;
            m->mapping.layout_idx_max = MIN(XKB_MAX_GROUPS,
                                            darray_size(m->rmlvo.layouts));
            m->mapping.layouts_candidates_mask = (xkb_layout_mask_t)
                /* All layouts */
                (UINT64_C(1) << m->mapping.layout_idx_max) - UINT64_C(1);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି LAYOUT_INDEX_SINGLE:
        ସ୍ଥିତି LAYOUT_INDEX_FIRST:
            /* No index or first index */
            idx = 0;
            /* fallthrough */
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
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

ସ୍ଥିର ଶୂନ୍ୟ
matcher_mapping_set_kccgst(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s, ସଂରଚନା sval ident)
{
    ଗଣନା rules_kccgst kccgst;
    ସଂରଚନା sval kccgst_sval;

    ପାଇଁ (kccgst = 0; kccgst < KCCGST_ଅଙ୍କ_ENTRIES; kccgst++) {
        kccgst_sval = rules_kccgst_svals[kccgst];

        ଯଦି (svaleq(rules_kccgst_svals[kccgst], ident))
            ଭାଙ୍ଗ;
    }

    /* Not found. */
    ଯଦି (kccgst >= KCCGST_ଅଙ୍କ_ENTRIES) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" is not a valid value here; "
                    "ignoring rule set",
                    (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ident.len, ident.start);
        m->mapping.active = false;
        ଫେରନ୍ତୁ;
    }

    ଯଦି (m->mapping.defined_kccgst_mask & (1u << kccgst)) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: \"%.*s\" appears twice on the same line; "
                    "ignoring rule set",
                    (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) kccgst_sval.len, kccgst_sval.start);
        m->mapping.active = false;
        ଫେରନ୍ତୁ;
    }

    m->mapping.kccgst_at_pos[m->mapping.ଅଙ୍କ_kccgst] = kccgst;
    m->mapping.defined_kccgst_mask |= (kccgst_mask_t) 1u << kccgst;
    m->mapping.ଅଙ୍କ_kccgst++;
}

ସ୍ଥିର bool
matcher_mapping_verify(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s)
{
    ଯଦି (m->mapping.ଅଙ୍କ_mlvo == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the left "
                    "hand side; ignoring rule set");
        ଯାଅ skip;
    }

    ଯଦି (m->mapping.ଅଙ୍କ_kccgst == 0) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid mapping: must have at least one value on the right "
                    "hand side; ignoring rule set");
        ଯାଅ skip;
    }

    /*
     * This following is very stupid, but this is how it works.
     * See the "Notes" section in the overview above.
     */

    ଯଦି (is_mlvo_mask_defined(m, MLVO_LAYOUT)) {
        ପ୍ରତିଜ୍ଞା(m->mapping.layout_idx != XKB_LAYOUT_INVALID);
        ବାଛ (m->mapping.layout_idx) {
            ସ୍ଥିତି LAYOUT_INDEX_SINGLE:
                /* Layout rule without index matches when
                 * exactly 1 layout is specified */
                ଯଦି (darray_size(m->rmlvo.layouts) > 1)
                    ଯାଅ skip;
                ଭାଙ୍ଗ;
            ସ୍ଥିତି LAYOUT_INDEX_MULTIPLE:
            ସ୍ଥିତି LAYOUT_INDEX_LATER:
                /* Layout rule matches when at least 2 layouts are specified */
                ଯଦି (darray_size(m->rmlvo.layouts) < 2)
                    ଯାଅ skip;
                ଭାଙ୍ଗ;
            ସ୍ଥିତି LAYOUT_INDEX_ANY:
            ସ୍ଥିତି LAYOUT_INDEX_FIRST:
                /* No restrictions */
                ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                /* Layout rule with index matches when at least 2 layouts are
                 * specified. Index must be in valid range. */
                ଯଦି (darray_size(m->rmlvo.layouts) < 2 ||
                    m->mapping.layout_idx >= darray_size(m->rmlvo.layouts))
                    ଯାଅ skip;
        }
    }

    ଯଦି (is_mlvo_mask_defined(m, MLVO_VARIANT)) {
        ପ୍ରତିଜ୍ଞା(m->mapping.variant_idx != XKB_LAYOUT_INVALID);
        ବାଛ (m->mapping.variant_idx) {
            ସ୍ଥିତି LAYOUT_INDEX_SINGLE:
                /* Variant rule without index matches
                 * when exactly 1 variant is specified */
                ଯଦି (darray_size(m->rmlvo.variants) > 1)
                    ଯାଅ skip;
                ଭାଙ୍ଗ;
            ସ୍ଥିତି LAYOUT_INDEX_ANY:
            ସ୍ଥିତି LAYOUT_INDEX_LATER:
            ସ୍ଥିତି LAYOUT_INDEX_FIRST:
                /* No restriction */
                ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                /* Variant rule with index matches when at least 2 variants are
                 * specified. Index must be in valid range. */
                ଯଦି (darray_size(m->rmlvo.variants) < 2 ||
                    m->mapping.variant_idx >= darray_size(m->rmlvo.variants))
                    ଯାଅ skip;
        }
    }

    ଫେରନ୍ତୁ true;

skip:
    m->mapping.active = false;
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_rule_start_new(ସଂରଚନା matcher *m)
{
    memset(&m->rule, 0, ଆକାର(m->rule));
    m->rule.skip = !m->mapping.active;
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_rule_set_mlvo_common(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
                             ସଂରଚନା sval ident,
                             ଗଣନା mlvo_match_type match_type)
{
    ଯଦି (m->rule.ଅଙ୍କ_mlvo_ମୂଲ୍ୟଗୁଡ଼ିକ >= m->mapping.ଅଙ୍କ_mlvo) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        ଫେରନ୍ତୁ;
    }
    m->rule.match_type_at_pos[m->rule.ଅଙ୍କ_mlvo_ମୂଲ୍ୟଗୁଡ଼ିକ] = match_type;
    m->rule.mlvo_ମୂଲ୍ୟ_at_pos[m->rule.ଅଙ୍କ_mlvo_ମୂଲ୍ୟଗୁଡ଼ିକ] = ident;
    m->rule.ଅଙ୍କ_mlvo_ମୂଲ୍ୟଗୁଡ଼ିକ++;
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_rule_set_mlvo_wildcard(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
                               ଗଣନା mlvo_match_type match_type)
{
    ସଂରଚନା sval dummy = SVAL(ଖାଲି, 0);
    matcher_rule_set_mlvo_common(m, s, dummy, match_type);
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_rule_set_mlvo_group(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
                            ସଂରଚନା sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_GROUP);
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_rule_set_mlvo(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
                      ସଂରଚନା sval ident)
{
    matcher_rule_set_mlvo_common(m, s, ident, MLVO_MATCH_NORMAL);
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_rule_set_kccgst(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
                        ସଂରଚନା sval ident)
{
    ଯଦି (m->rule.ଅଙ୍କ_kccgst_ମୂଲ୍ୟଗୁଡ଼ିକ >= m->mapping.ଅଙ୍କ_kccgst) {
        scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                    "invalid rule: has more values than the mapping line; "
                    "ignoring rule");
        m->rule.skip = true;
        ଫେରନ୍ତୁ;
    }
    m->rule.kccgst_ମୂଲ୍ୟ_at_pos[m->rule.ଅଙ୍କ_kccgst_ମୂଲ୍ୟଗୁଡ଼ିକ] = ident;
    m->rule.ଅଙ୍କ_kccgst_ମୂଲ୍ୟଗୁଡ଼ିକ++;
}

ସ୍ଥିର bool
match_group(ସଂରଚନା matcher *m, ସଂରଚନା sval group_name, ସଂରଚନା sval to)
{
    ସଂରଚନା group *group = ଖାଲି;
    ସଂରଚନା sval *element;
    bool found = false;

    darray_foreach(group, m->groups) {
        ଯଦି (svaleq(group->name, group_name)) {
            found = true;
            ଭାଙ୍ଗ;
        }
    }

    ଯଦି (!found) {
        /*
         * rules/evdev intentionally uses some undeclared group names
         * in rules (e.g. commented group definitions which may be
         * uncommented if needed). So we continue silently.
         */
        ଫେରନ୍ତୁ false;
    }

    darray_foreach(element, group->elements)
        ଯଦି (svaleq(to, *element))
            ଫେରନ୍ତୁ true;

    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
match_ମୂଲ୍ୟ(ସଂରଚନା matcher *m, ଅଚଳ ସଂରଚନା sval val, ଅଚଳ ସଂରଚନା sval to,
            ଗଣନା mlvo_match_type match_type,
            ଗଣନା wildcard_match_type wildcard_type)
{
    ବାଛ (match_type) {
        ସ୍ଥିତି MLVO_MATCH_WILDCARD_LEGACY:
            /* Match empty values only if explicitly required */
            ଫେରନ୍ତୁ wildcard_type == WILDCARD_MATCH_ALL || !!to.len;
        ସ୍ଥିତି MLVO_MATCH_WILDCARD_NONE:
            ଫେରନ୍ତୁ !to.len;
        ସ୍ଥିତି MLVO_MATCH_WILDCARD_SOME:
            ଫେରନ୍ତୁ !!to.len;
        ସ୍ଥିତି MLVO_MATCH_WILDCARD_ANY:
            /* Contrary to the legacy ‘*’, this wild card *always* matches */
            ଫେରନ୍ତୁ true;
        ସ୍ଥିତି MLVO_MATCH_GROUP:
            ଫେରନ୍ତୁ match_group(m, val, to);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ପ୍ରତିଜ୍ଞା(match_type == MLVO_MATCH_NORMAL);
            ଫେରନ୍ତୁ svaleq(val, to);
    }
}

ସ୍ଥିର bool
match_ମୂଲ୍ୟ_and_mark(ସଂରଚନା matcher *m, ଅଚଳ ସଂରଚନା sval val,
                     ସଂରଚନା matched_sval *to, ଗଣନା mlvo_match_type match_type,
                     ଗଣନା wildcard_match_type wildcard_type,
                     xkb_layout_mask_t layouts)
{
    bool matched = match_ମୂଲ୍ୟ(m, val, to->sval, match_type, wildcard_type);
    ଯଦି (matched)
        to->matched |= layouts;
    ଫେରନ୍ତୁ matched;
}

/*
 * This function performs %-expansion on @value (see overview above),
 * and appends the result to @expanded.
 */
ସ୍ଥିର bool
expand_rmlvo_in_kccgst_ମୂଲ୍ୟ(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
                             ସଂରଚନା sval ମୂଲ୍ୟ, xkb_layout_index_t layout_idx,
                             darray_char *expanded, size_t *i)
{
    ଅଚଳ ଅକ୍ଷର *str = ମୂଲ୍ୟ.start;

    /*
     * Some ugly hand-lexing here, but going through the scanner is more
     * trouble than it's worth, and the format is ugly on its own merit.
     */
    ଗଣନା rules_mlvo mlv;
    xkb_layout_index_t idx;
    ଅକ୍ଷର pfx, sfx;
    ସଂରଚନା matched_sval *expanded_ମୂଲ୍ୟ;

    /* %i not as layout/variant index "%l[%i]" but as qualifier ":%i" */
    ଯଦି (str[*i] == 'i' &&
        (*i + 1 == ମୂଲ୍ୟ.len || is_merge_mode_prefix(str[*i + 1])))
    {
        ଯଦି (layout_idx == XKB_LAYOUT_INVALID) {
            scanner_err(
                s, XKB_ERROR_RULES_INVALID_LAYOUT_INDEX_PERCENT_EXPANSION,
                "Invalid %%i in %%-expansion: there is no corresponding "
                "layout nor variant in the MLVO fields of the rules header."
            );
            ଯାଅ error;
        }
        (*i)++;
        ଅକ୍ଷର index_str[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = snprintf(index_str, ଆକାର(index_str), "%"PRIu32,
                                   layout_idx + 1);
        ପ୍ରତିଜ୍ଞା(ଗଣନା_2 > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
        darray_appends_nullterminate(*expanded, index_str, (darray_size_t)ଗଣନା_2);
        ଫେରନ୍ତୁ true;
    }

    pfx = sfx = 0;

    /* Check for prefix. */
    ଯଦି (str[*i] == '(' ||
        is_merge_mode_prefix(str[*i]) ||
        str[*i] == '_' || str[*i] == '-') {
        pfx = str[*i];
        ଯଦି (str[*i] == '(') sfx = ')';
        ଯଦି (++(*i) >= ମୂଲ୍ୟ.len) ଯାଅ error;
    }

    /* Mandatory model/layout/variant specifier. */
    ବାଛ (str[(*i)++]) {
    ସ୍ଥିତି 'm': mlv = MLVO_MODEL; ଭାଙ୍ଗ;
    ସ୍ଥିତି 'l': mlv = MLVO_LAYOUT; ଭାଙ୍ଗ;
    ସ୍ଥିତି 'v': mlv = MLVO_VARIANT; ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଯାଅ error;
    }

    /* Check for index. */
    idx = XKB_LAYOUT_INVALID;
    bool variable_index = false;
    ଯଦି (*i < ମୂଲ୍ୟ.len && str[*i] == '[') {
        ଯଦି (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
            scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                        "invalid index in %%-expansion; "
                        "may only index layout or variant");
            ଯାଅ error;
        }

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା consumed = extract_layout_index(str + (*i), ମୂଲ୍ୟ.len - (*i), &idx);
        ଯଦି (consumed <= 0) ଯାଅ error;
        ଯଦି (idx == XKB_LAYOUT_INVALID) {
            /* %i encountered */
            ପ୍ରତିଜ୍ଞା(layout_idx != XKB_LAYOUT_INVALID);
            idx = layout_idx;
            variable_index = true;
        }
        *i += (size_t)consumed;
    }

    /* Check for suffix, if there supposed to be one. */
    ଯଦି (sfx != 0) {
        ଯଦି (*i >= ମୂଲ୍ୟ.len) ଯାଅ error;
        ଯଦି (str[(*i)++] != sfx) ଯାଅ error;
    }

    /* Get the expanded value. */
    expanded_ମୂଲ୍ୟ = ଖାଲି;

    ଯଦି (mlv == MLVO_LAYOUT) {
        ଯଦି (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single layout */
            ଯଦି (darray_size(m->rmlvo.layouts) == 1)
                expanded_ମୂଲ୍ୟ = &darray_item(m->rmlvo.layouts, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple layouts */
        } ଅନ୍ୟଥା ଯଦି (idx < darray_size(m->rmlvo.layouts) &&
                   (variable_index || darray_size(m->rmlvo.layouts) > 1)) {
                expanded_ମୂଲ୍ୟ = &darray_item(m->rmlvo.layouts, idx);
        }
    }
    ଅନ୍ୟଥା ଯଦି (mlv == MLVO_VARIANT) {
        ଯଦି (idx == XKB_LAYOUT_INVALID) {
            /* No index provided: match only if single variant */
            ଯଦି (darray_size(m->rmlvo.variants) == 1)
                expanded_ମୂଲ୍ୟ = &darray_item(m->rmlvo.variants, 0);
        /* Some index provided: expand only if it is %i or
         * if there are multiple variants */
        } ଅନ୍ୟଥା ଯଦି (idx < darray_size(m->rmlvo.variants) &&
                   (variable_index || darray_size(m->rmlvo.variants) > 1)) {
                expanded_ମୂଲ୍ୟ = &darray_item(m->rmlvo.variants, idx);
        }
    }
    ଅନ୍ୟଥା ଯଦି (mlv == MLVO_MODEL) {
        expanded_ମୂଲ୍ୟ = &m->rmlvo.model;
    }

    /* If we didn't get one, skip silently. */
    ଯଦି (!expanded_ମୂଲ୍ୟ || expanded_ମୂଲ୍ୟ->sval.len == 0) {
        ଫେରନ୍ତୁ true;
    }

    ଯଦି (pfx != 0)
        darray_appends_nullterminate(*expanded, &pfx, 1);
    darray_appends_nullterminate(*expanded,
                                 expanded_ମୂଲ୍ୟ->sval.start,
                                 (darray_size_t) expanded_ମୂଲ୍ୟ->sval.len);
    ଯଦି (sfx != 0)
        darray_appends_nullterminate(*expanded, &sfx, 1);

    ଯଦି (mlv != MLVO_LAYOUT && mlv != MLVO_VARIANT) {
        static_assert(XKB_MAX_GROUPS == 32 &&
                      XKB_MAX_GROUPS < XKB_LAYOUT_INVALID,
                      "Invalid shift");
        ଅଚଳ xkb_layout_mask_t layouts = (idx == XKB_LAYOUT_INVALID)
            ? 0x1 /* default to first layout */
            : (UINT32_C(1) << idx);
        expanded_ମୂଲ୍ୟ->matched |= layouts;
    } ଅନ୍ୟଥା {
        expanded_ମୂଲ୍ୟ->matched |= (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
    }

    ଫେରନ୍ତୁ true;

error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "invalid %%-expansion in value; not used");
    ଫେରନ୍ତୁ false;
}

/*
 * This function performs :all replacement on @value (see overview above),
 * and appends the result to @expanded.
 */
ସ୍ଥିର ଶୂନ୍ୟ
expand_qualifier_in_kccgst_ମୂଲ୍ୟ(
    ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
    ସଂରଚନା sval ମୂଲ୍ୟ, darray_char *expanded,
    bool has_layout_idx_range, bool has_separator,
    darray_size_t prefix_idx, size_t *i)
{
    ଅଚଳ ଅକ୍ଷର *str = ମୂଲ୍ୟ.start;

    /* “all” followed by nothing or by a layout separator */
    ଯଦି (*i + 2 < ମୂଲ୍ୟ.len &&
        str[*i] == 'a' && str[*i + 1] == 'l' && str[*i + 2] == 'l' &&
        (*i + 3 == ମୂଲ୍ୟ.len || is_merge_mode_prefix(str[*i + 3]))) {
        ଯଦି (has_layout_idx_range)
            scanner_vrb(s, XKB_LOG_VERBOSITY_DETAILED, XKB_LOG_MESSAGE_NO_ID,
                        "Using :all qualifier with indices range "
                        "is not recommended.");
        /* Add at least one layout */
        darray_appends_nullterminate(*expanded, "1", 1);
        /* Check for more layouts (slow path) */
        ଯଦି (darray_size(m->rmlvo.layouts) > 1) {
            ଅକ୍ଷର layout_index[MAX_LAYOUT_INDEX_STR_LENGTH + 1];
            ଅଚଳ darray_size_t prefix_length =
                darray_size(*expanded) - prefix_idx - 1;
            ପାଇଁ (xkb_layout_index_t l = 1;
                 l < MIN(XKB_MAX_GROUPS, darray_size(m->rmlvo.layouts));
                 l++)
            {
                ଯଦି (!has_separator)
                    darray_append(*expanded, MERGE_DEFAULT_PREFIX);
                /* Append prefix */
                darray_appends_nullterminate(*expanded,
                                             &darray_item(*expanded, prefix_idx),
                                             prefix_length);
                /* Append index */
                ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = snprintf(layout_index, ଆକାର(layout_index),
                                           "%"PRIu32, l + 1);
                ପ୍ରତିଜ୍ଞା(ଗଣନା_2 > 0); /* See MAX_LAYOUT_INDEX_STR_LENGTH */
                darray_appends_nullterminate(*expanded, layout_index,
                                             (darray_size_t)ଗଣନା_2);
            }
        }
        *i += 3;
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
#ifdef _MSC_VER
concat_kccgst(darray_char *into, darray_size_t size, _In_reads_(size) ଅଚଳ ଅକ୍ଷର* from)
#ଅନ୍ୟଥା
concat_kccgst(darray_char *into, darray_size_t size, ଅଚଳ ଅକ୍ଷର from[ସ୍ଥିର size])
#endif
{
    /*
     * Appending  bar to  foo ->  foo (not an error if this happens)
     * Appending +bar to  foo ->  foo+bar
     * Appending  bar to +foo ->  bar+foo
     * Appending +bar to +foo -> +foo+bar
     */
    ଅଚଳ bool from_plus = is_merge_mode_prefix(from[0]);
    ଯଦି (from_plus || darray_empty(*into)) {
        darray_appends_nullterminate(*into, from, size);
    } ଅନ୍ୟଥା {
        ଅଚଳ ଅକ୍ଷର ch =
            (ଅକ୍ଷର) (darray_empty(*into) ? '\0' : darray_item(*into, 0));
        ଅଚଳ bool into_plus = is_merge_mode_prefix(ch);
        ଯଦି (into_plus)
            darray_prepends_nullterminate(*into, from, size);
    }
}

/*
 * This function performs %-expansion and :all-expansion on @value
 * (see overview above), and appends the result to @to.
 */
ସ୍ଥିର bool
append_expanded_kccgst_ମୂଲ୍ୟ(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
                             bool merge, darray_char *to, ସଂରଚନା sval ମୂଲ୍ୟ,
                             xkb_layout_index_t layout_idx)
{
    ଅଚଳ ଅକ୍ଷର *str = ମୂଲ୍ୟ.start;
    darray_char expanded = darray_new();
    darray_size_t last_item_idx = 0;
    bool has_separator = false;

    ପାଇଁ (size_t i = 0; i < ମୂଲ୍ୟ.len; ) {
        /* Check if that's a start of an expansion or qualifier */
        ବାଛ (str[i]) {
            /* Qualifier */
            ସ୍ଥିତି ':':
                darray_appends_nullterminate(expanded, &str[i++], 1);
                expand_qualifier_in_kccgst_ମୂଲ୍ୟ(m, s, ମୂଲ୍ୟ, &expanded,
                                                 m->mapping.has_layout_idx_range,
                                                 has_separator,
                                                 last_item_idx, &i);
                ଭାଙ୍ଗ;
            /* Expansion */
            ସ୍ଥିତି '%':
                i++;
                ଯଦି (i >= ମୂଲ୍ୟ.len ||
                    !expand_rmlvo_in_kccgst_ମୂଲ୍ୟ(m, s, ମୂଲ୍ୟ, layout_idx,
                                                  &expanded, &i))
                        ଯାଅ error;
                ଭାଙ୍ଗ;
            /* New item */
            ସ୍ଥିତି MERGE_OVERRIDE_PREFIX:
            ସ୍ଥିତି MERGE_AUGMENT_PREFIX:
            ସ୍ଥିତି MERGE_REPLACE_PREFIX:
                darray_appends_nullterminate(expanded, &str[i++], 1);
                last_item_idx = darray_size(expanded) - 1;
                has_separator = true;
                ଭାଙ୍ଗ;
            /* Just a normal character. */
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                darray_appends_nullterminate(expanded, &str[i++], 1);
        }
    }

    /* See note: “Layout index ranges and merging KcCGST values” */
    ଯଦି (merge) {
        ଯଦି (!darray_empty(expanded))
            concat_kccgst(to, darray_size(expanded), darray_ବସ୍ତୁଗୁଡ଼ିକ(expanded));
    } ଅନ୍ୟଥା {
        darray_concat(*to, expanded);
    }
    darray_free(expanded);
    ଫେରନ୍ତୁ true;

error:
    darray_free(expanded);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
matcher_append_pending_kccgst(ସଂରଚନା matcher *m)
{
    ଯଦି (!m->mapping.has_multiple_layouts)
        ଫେରନ୍ତୁ true;
    /*
     * Handle pending KcCGST values
     * See note: “Layout index ranges and merging KcCGST values”
     */
    ପାଇଁ (kccgst_index_t i = 0; i < m->mapping.ଅଙ୍କ_kccgst; i++) {
        ଅଚଳ ଗଣନା rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
        /* For each relevant layout, append the relevant KcCGST values to
         * the output. */
        ପାଇଁ (xkb_layout_index_t layout = m->mapping.layout_idx_min;
             layout < m->mapping.layout_idx_max;
             layout++) {
            /* There may be multiple values to add if the rule set involved
             * options. Process them sequentially. */
            ପଞ୍ଜିକରଣ ଅଚଳ ସଂରଚନା kccgst_buffer* ଅଚଳ buf = &m->pending_kccgst;
            size_t offset = 0;
            ପାଇଁ (darray_size_t k = 0; k < darray_size(buf->slices); k++) {
                ପଞ୍ଜିକରଣ ଅଚଳ ସଂରଚନା kccgst_buffer_slice * ଅଚଳ slice =
                    &darray_item(buf->slices, k);
                ଯଦି (slice->kccgst == kccgst && slice->layout == layout &&
                    slice->length)
                    concat_kccgst(&m->kccgst[kccgst], slice->length,
                                  darray_ବସ୍ତୁଗୁଡ଼ିକ(buf->buffer) + offset);
                offset += slice->length;
            }
        }
    }
    /* Ensure we won’t come here before the next relevant rule set */
    m->mapping.has_multiple_layouts = false;
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ
matcher_rule_verify(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s)
{
    ଯଦି (m->rule.ଅଙ୍କ_mlvo_ମୂଲ୍ୟଗୁଡ଼ିକ != m->mapping.ଅଙ୍କ_mlvo ||
        m->rule.ଅଙ୍କ_kccgst_ମୂଲ୍ୟଗୁଡ଼ିକ != m->mapping.ଅଙ୍କ_kccgst) {
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
ସ୍ଥିର bool
matcher_rule_match_model(ସଂରଚନା matcher *m, ଅଚଳ ସଂରଚନା sval ମୂଲ୍ୟ,
                         ଗଣନା mlvo_match_type match_type)
{
    ଫେରନ୍ତୁ match_ମୂଲ୍ୟ_and_mark(
        m, ମୂଲ୍ୟ, &m->rmlvo.model, match_type, WILDCARD_MATCH_ALL,
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
ସ୍ଥିର bool
matcher_rule_match_option(ସଂରଚନା matcher *m, ଅଚଳ ସଂରଚନା sval ମୂଲ୍ୟ,
                          ଗଣନା mlvo_match_type match_type,
                          xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    ପ୍ରତିଜ୍ଞା(!darray_empty(m->rmlvo.options));

    /* Remaining layout indices to match if MLVO has a layout/variant field */
    xkb_layout_mask_t unmatched =
        (m->mapping.defined_mlvo_mask & MLVO_LAYOUT_AND_VARIANT)
            ? *candidates
            : 0;

    bool matched = false;
    ସଂରଚନା matched_sval *option;
    darray_foreach(option, m->rmlvo.options) {
        ଅଚଳ xkb_layout_mask_t matchable = option->layouts
            /*
             * Layout-specific option; may match only if:
             * - there is a layout or variant field, and
             * - the option layout matches the remaining candidates.
             */
            ? (unmatched & option->layouts)
            /* Layout-generic option: no restriction */
            : (xkb_layout_mask_t)GLOBAL_MATCHED_LAYOUTS;
        ଯଦି (matchable && match_ମୂଲ୍ୟ_and_mark(m, ମୂଲ୍ୟ, option, match_type,
                                              WILDCARD_MATCH_ALL, matchable)) {
            matched = true;
            unmatched &= ~matchable;
            ଯଦି (!unmatched)
                ଭାଙ୍ଗ;
        }
    }
    ଯଦି (unmatched) {
        /* Remove unmatched layout indices */
        *candidates &= ~unmatched;
    }
    ଫେରନ୍ତୁ matched;
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
ସ୍ଥିର bool
matcher_rule_match_layout_or_variant(ସଂରଚନା matcher *m,
                                     darray_matched_sval *ନିବେଶ,
                                     ଅଚଳ ସଂରଚନା sval ମୂଲ୍ୟ,
                                     ଗଣନା mlvo_match_type match_type,
                                     xkb_layout_mask_t *candidates)
{
    /* There is always at least one value */
    ପ୍ରତିଜ୍ଞା(!darray_empty(*ନିବେଶ));

    bool matched = false;
    /* Loop over the layout index range */
    ପାଇଁ (xkb_layout_index_t idx = m->mapping.layout_idx_min;
         idx < m->mapping.layout_idx_max && *candidates;
         idx++)
    {
        /* Process only if layout index is enabled */
        ଅଚଳ xkb_layout_mask_t layout = (UINT32_C(1) << idx);
        ଯଦି (layout & *candidates) {
            ସଂରଚନା matched_sval *to = &darray_item(*ନିବେଶ, idx);
            ଯଦି (match_ମୂଲ୍ୟ_and_mark(m, ମୂଲ୍ୟ, to, match_type,
                                     WILDCARD_MATCH_NONEMPTY, layout)) {
                /* Matched, keep index */
                matched = true;
            } ଅନ୍ୟଥା {
                /* Not matched, remove index */
                *candidates &= ~layout;
            }
        }
    }
    ଫେରନ୍ତୁ matched;
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
ସ୍ଥିର ଶୂନ୍ୟ
matcher_rule_apply_if_matches(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s)
{
    /* Initial candidates (used if MLVO has a layout or variant field) */
    xkb_layout_mask_t candidate_layouts = m->mapping.layouts_candidates_mask;

    /* Loop over MLVO pattern components */
    ପାଇଁ (mlvo_index_t i = 0; i < m->mapping.ଅଙ୍କ_mlvo; i++) {
        ଅଚଳ ଗଣନା rules_mlvo mlvo = m->mapping.mlvo_at_pos[i];
        ଅଚଳ ସଂରଚନା sval ମୂଲ୍ୟ = m->rule.mlvo_ମୂଲ୍ୟ_at_pos[i];
        ଅଚଳ ଗଣନା mlvo_match_type match_type = m->rule.match_type_at_pos[i];
        bool matched = false;

        ବାଛ (mlvo) {
        ସ୍ଥିତି MLVO_MODEL:
            matched = matcher_rule_match_model(m, ମୂଲ୍ୟ, match_type);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି MLVO_LAYOUT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.layouts, ମୂଲ୍ୟ, match_type, &candidate_layouts
            );
            ଭାଙ୍ଗ;
        ସ୍ଥିତି MLVO_VARIANT:
            matched = matcher_rule_match_layout_or_variant(
                m, &m->rmlvo.variants, ମୂଲ୍ୟ, match_type, &candidate_layouts
            );
            ଭାଙ୍ଗ;
        ସ୍ଥିତି MLVO_OPTION:
            matched = matcher_rule_match_option(
                m, ମୂଲ୍ୟ, match_type, &candidate_layouts
            );
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ: {
            static_assert(MLVO_OPTION == 3 &&
                          MLVO_OPTION == MLVO_ଅଙ୍କ_ENTRIES - 1,
                          "Unexpected MLVO field");
            ପ୍ରତିଜ୍ଞା(!"Unreachable");
        }
        }

        ଯଦି (!matched)
            ଫେରନ୍ତୁ;
    }

    ଯଦି (m->mapping.has_multiple_layouts) {
        /* Special index: loop over the index range */
        ପାଇଁ (xkb_layout_index_t idx = m->mapping.layout_idx_min;
             idx < m->mapping.layout_idx_max;
             idx++)
        {
            ଯଦି (candidate_layouts & (UINT32_C(1) << idx)) {
                ପାଇଁ (kccgst_index_t i = 0; i < m->mapping.ଅଙ୍କ_kccgst; i++) {
                    ଅଚଳ ଗଣନା rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
                    ଅଚଳ ସଂରଚନା sval ମୂଲ୍ୟ = m->rule.kccgst_ମୂଲ୍ୟ_at_pos[i];
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
                    ସଂରଚନା kccgst_buffer * ଅଚଳ buf = &m->pending_kccgst;
                    ଅଚଳ darray_size_t prev_buffer_length =
                        darray_size(buf->buffer);
                    append_expanded_kccgst_ମୂଲ୍ୟ(m, s, false, &buf->buffer,
                                                 ମୂଲ୍ୟ, idx);
                    ଅଚଳ uint32_t length = (uint32_t) (darray_size(buf->buffer)
                                          - prev_buffer_length);
                    ଅଚଳ ସଂରଚନା kccgst_buffer_slice slice = {
                        .length = length,
                        .kccgst = kccgst,
                        .layout = idx
                    };
                    darray_append(buf->slices, slice);
                }
            }
        }
    } ଅନ୍ୟଥା {
        /* Numeric index or no index */
        ପାଇଁ (kccgst_index_t i = 0; i < m->mapping.ଅଙ୍କ_kccgst; i++) {
            ଗଣନା rules_kccgst kccgst = m->mapping.kccgst_at_pos[i];
            ସଂରଚନା sval ମୂଲ୍ୟ = m->rule.kccgst_ମୂଲ୍ୟ_at_pos[i];
            append_expanded_kccgst_ମୂଲ୍ୟ(m, s, true, &m->kccgst[kccgst], ମୂଲ୍ୟ,
                                         m->mapping.layout_idx_min);
        }
    }

    /*
     * If a rule matches in a rule set, the rest of the set should be
     * skipped. However, rule sets matching against options may contain
     * several legitimate rules, so they are processed entirely.
     */
    ଯଦି (!(is_mlvo_mask_defined(m, MLVO_OPTION))) {
        m->mapping.layouts_candidates_mask &= ~candidate_layouts;
    }
}

ସ୍ଥିର ଗଣନା rules_token
gettok(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s)
{
    ଫେରନ୍ତୁ lex(s, &m->val);
}

// NOLINTBEGIN(readability-trivial-switch)
ସ୍ଥିର bool
matcher_match(ସଂରଚନା matcher *m, ସଂରଚନା scanner *s,
              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth,
              ଅଚଳ ଅକ୍ଷର *string, size_t len,
              ଅଚଳ ଅକ୍ଷର *file_name)
{
    ଗଣନା rules_token tok;

    ଯଦି (!m)
        ଫେରନ୍ତୁ false;

initial:
    ବାଛ (tok = gettok(m, s)) {
    ସ୍ଥିତି TOK_BANG:
        ଯାଅ bang;
    ସ୍ଥିତି TOK_END_OF_ଧାଡ଼ି:
        ଯାଅ initial;
    ସ୍ଥିତି TOK_END_OF_FILE:
        ଯାଅ finish;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

bang:
    ବାଛ (tok = gettok(m, s)) {
    ସ୍ଥିତି TOK_GROUP_NAME:
        matcher_group_start_new(m, m->val.string);
        ଯାଅ group_name;
    ସ୍ଥିତି TOK_INCLUDE:
        ଯାଅ include_statement;
    ସ୍ଥିତି TOK_IDENTIFIER:
        matcher_mapping_start_new(m);
        matcher_mapping_set_mlvo(m, s, m->val.string);
        ଯାଅ mapping_mlvo;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

group_name:
    ବାଛ (tok = gettok(m, s)) {
    ସ୍ଥିତି TOK_EQUALS:
        ଯାଅ group_element;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

group_element:
    ବାଛ (tok = gettok(m, s)) {
    ସ୍ଥିତି TOK_IDENTIFIER:
        matcher_group_add_element(m, s, m->val.string);
        ଯାଅ group_element;
    ସ୍ଥିତି TOK_END_OF_ଧାଡ଼ି:
        ଯାଅ initial;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

include_statement:
    ବାଛ (tok = gettok(m, s)) {
    ସ୍ଥିତି TOK_IDENTIFIER:
        matcher_include(m, s, include_depth, m->val.string);
        ଯାଅ include_statement_end;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

include_statement_end:
    ବାଛ (tok = gettok(m, s)) {
    ସ୍ଥିତି TOK_END_OF_ଧାଡ଼ି:
        ଯାଅ initial;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

mapping_mlvo:
    ବାଛ (tok = gettok(m, s)) {
    ସ୍ଥିତି TOK_IDENTIFIER:
        ଯଦି (m->mapping.active)
            matcher_mapping_set_mlvo(m, s, m->val.string);
        ଯାଅ mapping_mlvo;
    ସ୍ଥିତି TOK_EQUALS:
        ଯାଅ mapping_kccgst;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

mapping_kccgst:
    ବାଛ (tok = gettok(m, s)) {
    ସ୍ଥିତି TOK_IDENTIFIER:
        ଯଦି (m->mapping.active)
            matcher_mapping_set_kccgst(m, s, m->val.string);
        ଯାଅ mapping_kccgst;
    ସ୍ଥିତି TOK_END_OF_ଧାଡ଼ି:
        ଯଦି (m->mapping.active && matcher_mapping_verify(m, s)) {
            matcher_mapping_set_layout_bounds(m);
            ଯଦି (m->mapping.has_multiple_layouts) {
                /* Lazily reset buffers for layout index ranges.
                 * We’ll reuse the allocations. */
                darray_size(m->pending_kccgst.buffer) = 0;
                darray_size(m->pending_kccgst.slices) = 0;
            }
        }
        ଯାଅ rule_mlvo_first;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

rule_mlvo_first:
    ବାଛ (tok = gettok(m, s)) {
    ସ୍ଥିତି TOK_BANG:
        matcher_append_pending_kccgst(m);
        ଯାଅ bang;
    ସ୍ଥିତି TOK_END_OF_ଧାଡ଼ି:
        ଯାଅ rule_mlvo_first;
    ସ୍ଥିତି TOK_END_OF_FILE:
        matcher_append_pending_kccgst(m);
        ଯାଅ finish;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        matcher_rule_start_new(m);
        ଯାଅ rule_mlvo_no_tok;
    }

rule_mlvo:
    tok = gettok(m, s);
rule_mlvo_no_tok:
    ବାଛ (tok) {
    ସ୍ଥିତି TOK_IDENTIFIER:
        ଯଦି (!m->rule.skip)
            matcher_rule_set_mlvo(m, s, m->val.string);
        ଯାଅ rule_mlvo;
    ସ୍ଥିତି TOK_WILD_CARD_STAR:
        ଯଦି (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_LEGACY);
        ଯାଅ rule_mlvo;
    ସ୍ଥିତି TOK_WILD_CARD_NONE:
        ଯଦି (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_NONE);
        ଯାଅ rule_mlvo;
    ସ୍ଥିତି TOK_WILD_CARD_SOME:
        ଯଦି (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_SOME);
        ଯାଅ rule_mlvo;
    ସ୍ଥିତି TOK_WILD_CARD_ANY:
        ଯଦି (!m->rule.skip)
            matcher_rule_set_mlvo_wildcard(m, s, MLVO_MATCH_WILDCARD_ANY);
        ଯାଅ rule_mlvo;
    ସ୍ଥିତି TOK_GROUP_NAME:
        ଯଦି (!m->rule.skip)
            matcher_rule_set_mlvo_group(m, s, m->val.string);
        ଯାଅ rule_mlvo;
    ସ୍ଥିତି TOK_EQUALS:
        ଯାଅ rule_kccgst;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

rule_kccgst:
    ବାଛ (tok = gettok(m, s)) {
    ସ୍ଥିତି TOK_IDENTIFIER:
        ଯଦି (!m->rule.skip)
            matcher_rule_set_kccgst(m, s, m->val.string);
        ଯାଅ rule_kccgst;
    ସ୍ଥିତି TOK_END_OF_ଧାଡ଼ି:
        ଯଦି (!m->rule.skip)
            matcher_rule_verify(m, s);
        ଯଦି (!m->rule.skip)
            matcher_rule_apply_if_matches(m, s);
        ଯାଅ rule_mlvo_first;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ unexpected;
    }

unexpected:
    ବାଛ (tok) {
    ସ୍ଥିତି TOK_ERROR:
        ଯାଅ error;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯାଅ state_error;
    }

finish:
    ଫେରନ୍ତୁ true;

state_error:
    scanner_err(s, XKB_ERROR_INVALID_RULES_SYNTAX,
                "unexpected token (0x%x)", tok);
error:
    ଫେରନ୍ତୁ false;
}
// NOLINTEND(readability-trivial-switch)

ସ୍ଥିର bool
read_rules_file(ସଂରଚନା xkb_context *ctx,
                ସଂରଚନା matcher *matcher,
                unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth,
                FILE *file,
                ଅଚଳ ଅକ୍ଷର *path)
{
    bool ret;
    ଅକ୍ଷର *string;
    size_t size;
    ସଂରଚନା scanner scanner;

    ଯଦି (!map_file(file, &string, &size)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Couldn't read rules file \"%s\": %s\n",
                path, strerror(errno));
        ଫେରନ୍ତୁ false;
    }

    scanner_init(&scanner, matcher->ctx, string, size, path, ଖାଲି);

    /* Basic detection of wrong character encoding.
       The first character relevant to the grammar must be ASCII:
       whitespace, !, / (for comment) */
    ଯଦି (!scanner_check_supported_char_encoding(&scanner)) {
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "This could be a file encoding issue. "
                    "Supported encodings must be backward compatible with ASCII.");
        scanner_err(&scanner, XKB_ERROR_INVALID_FILE_ENCODING,
                    "E.g. ISO/CEI 8859 and UTF-8 are supported "
                    "but UTF-16, UTF-32 and CP1026 are not.");
        unmap_file(string, size);
        ଫେରନ୍ତୁ false;
    }

    ret = matcher_match(matcher, &scanner, include_depth, string, size, path);
    unmap_file(string, size);
    ଫେରନ୍ତୁ ret;
}

/**
 * XKB extension: append all *.pre or *.post optional partial rules files.
 * This is a lightweight extension mechanism to enable *simple* rules files
 * composition: rules files are just processed sequentially using the matcher
 * resulting from the previous file.
 */
ସ୍ଥିର bool
xkb_resolve_partial_rules(ସଂରଚନା xkb_context *ctx, ଅକ୍ଷର *path, size_t path_size,
                          ଅଚଳ ଅକ୍ଷର* rules, ଅଚଳ ଅକ୍ଷର* suffix,
                          ସଂରଚନା matcher *matcher)
{
    /* Set partial rules filename: canonical rules filename + suffix */
    ଅକ୍ଷର partial_rules[60]; /* Arbitrary, but we do not expect long names */
    ଯଦି (unlikely(!snprintf_safe(partial_rules, ଆକାର(partial_rules),
                                "%s%s", rules, suffix))) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s%s\"\n", rules, suffix);
        ଫେରନ୍ତୁ false;
    }

    /*
     * Traverse all include paths and resolve all corresponding partial rules.
     * These files are optional, but required to be valid if present.
     */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    FILE *file = ଖାଲି;
    ଅଚଳ size_t len = strlen(partial_rules);
    ଯେତେବେଳେ ((file = FindFileInXkbPath(ctx, "(unknown)",
                                     partial_rules, len, FILE_TYPE_RULES,
                                     path, path_size, &offset, false)) != ଖାଲି) {
        ଅଚଳ bool ok = read_rules_file(ctx, matcher, 0, file, path);
        fclose(file);
        ଯଦି (!ok) {
            log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                    "Error while parsing XKB rules \"%s\"\n", path);
            ଫେରନ୍ତୁ false;
        }
        offset++;
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
xkb_resolve_rules(ସଂରଚନା xkb_context *ctx,
                  ଅଚଳ ଅକ୍ଷର* rules, ସଂରଚନା matcher *matcher,
                  ସଂରଚନା xkb_component_names *out,
                  xkb_layout_index_t *explicit_layouts)
{
    bool ret = false;

    /* First check if the main rules file exists or error early */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = 0;
    ଅକ୍ଷର path[PATH_MAX];
    FILE * ଅଚଳ file = FindFileInXkbPath(ctx, "(unknown)",
                                          rules, strlen(rules), FILE_TYPE_RULES,
                                          path, ଆକାର(path), &offset, true);
    ଯଦି (!file) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Cannot load XKB rules \"%s\"\n", rules);
        ଯାଅ err_out;
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
    ret = xkb_resolve_partial_rules(ctx, path, ଆକାର(path),
                                    rules, ".pre", matcher);
    ଯଦି (!ret)
        ଯାଅ err_out;

    /* Resolve main <rules> file */
    ret = read_rules_file(ctx, matcher, 0, file, path);
    ଯଦି (!ret) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "Error while parsing XKB rules \"%s\"\n", path);
        ଯାଅ err_out;
    }

    /* XKB extension: resolve optional <rules>.post files */
    ret = xkb_resolve_partial_rules(ctx, path, ଆକାର(path),
                                    rules, ".post", matcher);
    ଯଦି (!ret)
        ଯାଅ err_out;

    /* Check we got required components */
    ଯଦି (darray_empty(matcher->kccgst[KCCGST_KEYCODES]) ||
        darray_empty(matcher->kccgst[KCCGST_TYPES]) ||
        darray_empty(matcher->kccgst[KCCGST_COMPAT]) ||
        /* darray_empty(matcher->kccgst[KCCGST_GEOMETRY]) || */
        darray_empty(matcher->kccgst[KCCGST_SYMBOLS])) {
        log_err(ctx, XKB_ERROR_CANNOT_RESOLVE_RMLVO,
                "No components returned from XKB rules \"%s\"\n", rules);
        ret = false;
        ଯାଅ err_out;
    }

    darray_steal(matcher->kccgst[KCCGST_KEYCODES], &out->keycodes, ଖାଲି);
    darray_steal(matcher->kccgst[KCCGST_TYPES], &out->types, ଖାଲି);
    darray_steal(matcher->kccgst[KCCGST_COMPAT], &out->compatibility, ଖାଲି);
    darray_steal(matcher->kccgst[KCCGST_SYMBOLS], &out->symbols, ଖାଲି);
    darray_steal(matcher->kccgst[KCCGST_GEOMETRY], &out->geometry, ଖାଲି);

    ସଂରଚନା matched_sval *mval = &matcher->rmlvo.model;
    ଯଦି (!mval->matched && mval->sval.len > 0)
        log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ମୂଲ୍ୟ,
                "RMLVO mismatch: unrecognized model \"%.*s\" "
                "was ignored in rules \"%s\"\n",
                (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.layouts)
        ଯଦି (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ମୂଲ୍ୟ,
                    "RMLVO mismatch: unrecognized layout \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.variants)
        ଯଦି (!mval->matched && mval->sval.len > 0)
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ମୂଲ୍ୟ,
                    "RMLVO mismatch: unrecognized variant \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mval->sval.len, mval->sval.start, rules);
    darray_foreach(mval, matcher->rmlvo.options) {
        ଯଦି (!mval->sval.len)
            ଜାରି;

        ଯଦି (!mval->matched) {
            /* No match (either layout-specific or layout-independent) */
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ମୂଲ୍ୟ,
                    "RMLVO mismatch: unrecognized option \"%.*s\" "
                    "was ignored in rules \"%s\"\n",
                    (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mval->sval.len, mval->sval.start, rules);
        } ଅନ୍ୟଥା ଯଦି (mval->layouts && mval->layouts != mval->matched) {
            /* Partial match (layout-specific) */
            ଅଚଳ xkb_layout_mask_t unmatched = (mval->layouts & ~mval->matched);
            ପ୍ରତିଜ୍ଞା(unmatched);
            log_err(matcher->ctx, XKB_ERROR_UNRECOGNIZED_RMLVO_ମୂଲ୍ୟ,
                    "RMLVO mismatch: option \"%.*s\" with layout mask "
                    "0x%"PRIx32" was ignored in rules \"%s\"\n",
                    (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)mval->sval.len, mval->sval.start,
                    unmatched, rules);
        }
    }

    /* Set the number of explicit layouts */
    ଯଦି (out->symbols != ଖାଲି && explicit_layouts != ଖାଲି) {
        *explicit_layouts = 1; /* at least one group */
        ଅଚଳ ଅକ୍ଷର *symbols = out->symbols;
        /* Take the highest modifier */
        ଯେତେବେଳେ ((symbols = strchr(symbols, ':')) != ଖାଲି && symbols[1] != '\0') {
            xkb_layout_index_t group = 0;
            ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = parse_dec_to_uint32_t(++symbols, SIZE_MAX, &group);
            /* Update only when valid group index, but continue parsing
             * even on invalid ones, as we do not handle them here. */
            ଯଦି (ଗଣନା_2 > 0 && (symbols[ଗଣନା_2] == '\0' ||
                is_merge_mode_prefix(symbols[ଗଣନା_2])) &&
                group > 0 && group <= XKB_MAX_GROUPS) {
                *explicit_layouts = MAX(*explicit_layouts, group);
                symbols += ଗଣନା_2;
            }
        }
    }

err_out:
    ଯଦି (file)
        fclose(file);

    ଫେରନ୍ତୁ ret;
}

bool
xkb_components_from_rmlvo_builder(ଅଚଳ ସଂରଚନା xkb_rmlvo_builder *rmlvo,
                                  ସଂରଚନା xkb_component_names *out,
                                  xkb_layout_index_t *explicit_layouts)
{
    ଅଚଳ ଅକ୍ଷର *rules = rmlvo->rules;
    ସଂରଚନା matcher *matcher = matcher_new_from_rmlvo(rmlvo, &rules);
    ଯଦି (!matcher)
        ଫେରନ୍ତୁ false;

    ଅଚଳ bool ret = xkb_resolve_rules(rmlvo->ctx, rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    ଫେରନ୍ତୁ ret;
}


bool
xkb_components_from_rules_names(ସଂରଚନା xkb_context *ctx,
                                ଅଚଳ ସଂରଚନା xkb_rule_names *rmlvo,
                                ସଂରଚନା xkb_component_names *out,
                                xkb_layout_index_t *explicit_layouts)
{
    ସଂରଚନା matcher *matcher = matcher_new_from_names(ctx, rmlvo);
    ଯଦି (!matcher)
        ଫେରନ୍ତୁ false;

    ଅଚଳ bool ret = xkb_resolve_rules(ctx, rmlvo->rules, matcher,
                                       out, explicit_layouts);

    matcher_free(matcher);
    ଫେରନ୍ତୁ ret;
}
