/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdint.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "expr.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "keysym.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "xkbcomp/ast.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "util-numbers.h"
#అన్తర్భావ "util-checked-arithmetic.h"

రకనిర్వచనం bool (*IdentLookupFunc)(నిర్మాణం xkb_context *ctx, స్థిరరాశి శూన్య *priv,
                                xkb_atom_t క్షేత్రం_2, uint32_t *val_rtrn,
                                bool *pending);

bool
సమాసం_Resolve_Lhs(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
               స్థిరరాశి అక్షరం **elem_rtrn, స్థిరరాశి అక్షరం **క్షేత్రం_rtrn,
               సమాసం_Def **సూచీ_rtrn)
{
    ఎంపిక (సమాసం->common.type) {
    స్థితి STMT_సమాసం_IDENT:
        *elem_rtrn = ఖాళీ;
        *క్షేత్రం_rtrn = xkb_atom_పాఠ్యం(ctx, సమాసం->ident.ident);
        *సూచీ_rtrn = ఖాళీ;
        తిరిగి (*క్షేత్రం_rtrn != ఖాళీ);
    స్థితి STMT_సమాసం_క్షేత్రం_REF:
        *elem_rtrn = xkb_atom_పాఠ్యం(ctx, సమాసం->క్షేత్రం_ref.element);
        *క్షేత్రం_rtrn = xkb_atom_పాఠ్యం(ctx, సమాసం->క్షేత్రం_ref.క్షేత్రం_2);
        *సూచీ_rtrn = ఖాళీ;
        తిరిగి (*elem_rtrn != ఖాళీ && *క్షేత్రం_rtrn != ఖాళీ);
    స్థితి STMT_సమాసం_ARRAY_REF:
        *elem_rtrn = xkb_atom_పాఠ్యం(ctx, సమాసం->array_ref.element);
        *క్షేత్రం_rtrn = xkb_atom_పాఠ్యం(ctx, సమాసం->array_ref.క్షేత్రం_2);
        *సూచీ_rtrn = సమాసం->array_ref.నమోదు_2;
        ఒకవేళ (సమాసం->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == ఖాళీ)
            తిరిగి false;
        ఒకవేళ (*క్షేత్రం_rtrn == ఖాళీ)
            తిరిగి false;
        తిరిగి true;
    అప్రమేయ:
        ఆపు;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", సమాసం->common.type);
    తిరిగి false;
}

స్థిర bool
SimpleLookup(నిర్మాణం xkb_context *ctx, స్థిరరాశి శూన్య *priv, xkb_atom_t క్షేత్రం_2,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    ఒకవేళ (!priv || క్షేత్రం_2 == XKB_ATOM_NONE)
        తిరిగి false;

    స్థిరరాశి అక్షరం *str = xkb_atom_పాఠ్యం(ctx, క్షేత్రం_2);
    కొరకు (స్థిరరాశి Lookup_నమోదు *నమోదు_2 = priv; నమోదు_2 && నమోదు_2->name; నమోదు_2++) {
        ఒకవేళ (istreq(str, నమోదు_2->name)) {
            *val_rtrn = నమోదు_2->విలువ;
            తిరిగి true;
        }
    }

    తిరిగి false;
}

నిర్మాణం named_integer_నమూనా {
    స్థిరరాశి అక్షరం *ఉపసర్గ;
    కొలత_t ఉపసర్గ_నిడివి;
    uint32_t min;
    uint32_t max;
    స్థిరరాశి Lookup_నమోదు *entries;
    స్థిరరాశి Lookup_నమోదు *pending_entries;
    bool is_ముసుగు;
    గణన xkb_సందేశం_సంకేతం error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
స్థిర bool
Named_Integer_నమూనా_Lookup(నిర్మాణం xkb_context *ctx, స్థిరరాశి శూన్య *priv,
                          xkb_atom_t క్షేత్రం_2, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    ఒకవేళ (!priv || క్షేత్రం_2 == XKB_ATOM_NONE)
        తిరిగి false;

    స్థిరరాశి అక్షరం * స్థిరరాశి str = xkb_atom_పాఠ్యం(ctx, క్షేత్రం_2);
    స్థిరరాశి నిర్మాణం named_integer_నమూనా * స్థిరరాశి నమూనా = priv;

    స్థిరరాశి పూర్ణాంకం లెక్క = (istrneq(str, నమూనా->ఉపసర్గ, నమూనా->ఉపసర్గ_నిడివి))
        ? parse_dec_గమ్యం_uint32_t(str + నమూనా->ఉపసర్గ_నిడివి, SIZE_MAX, val_rtrn)
        : 0;

    ఒకవేళ (లెక్క > 0 && *(str + నమూనా->ఉపసర్గ_నిడివి + లెక్క) == '\0') {
        ఒకవేళ (*val_rtrn < నమూనా->min || *val_rtrn > నమూనా->max) {
            log_err(ctx, నమూనా->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    నమూనా->ఉపసర్గ, *val_rtrn, నమూనా->min, నమూనా->max);
            తిరిగి false;
        }
        ఒకవేళ (నమూనా->is_ముసుగు) {
            /* Compute mask (bit 0 = min index) */
            ప్రతిజ్ఞా(*val_rtrn - నమూనా->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - నమూనా->min);
        }
        తిరిగి true;
    } లేకపోతే {
        ఒకవేళ (నమూనా->entries &&
            SimpleLookup(ctx, నమూనా->entries, క్షేత్రం_2, val_rtrn, ఖాళీ)) {
            తిరిగి true;
        }
        ఒకవేళ (నమూనా->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, నమూనా->pending_entries, క్షేత్రం_2, val_rtrn, ఖాళీ)) {
            *pending_rtrn = true;
            తిరిగి true;
        }
        తిరిగి false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
రకనిర్వచనం నిర్మాణం {
    స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods;
    గణన mod_type mod_type;
} Lookup_Mod_ముసుగు_Priv;

స్థిర bool
Lookup_Mod_ముసుగు(నిర్మాణం xkb_context *ctx, స్థిరరాశి శూన్య *priv, xkb_atom_t క్షేత్రం_2,
              xkb_mod_ముసుగు_t *val_rtrn, bool *pending_rtrn)
{
    స్థిరరాశి అక్షరం *str = xkb_atom_పాఠ్యం(ctx, క్షేత్రం_2);
    ఒకవేళ (!str)
        తిరిగి false;

    ఒకవేళ (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_ముసుగు_ALL;
        తిరిగి true;
    }

    ఒకవేళ (istreq(str, "none")) {
        *val_rtrn = 0;
        తిరిగి true;
    }

    స్థిరరాశి Lookup_Mod_ముసుగు_Priv *arg = priv;
    స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods = arg->mods;
    స్థిరరాశి గణన mod_type mod_type = arg->mod_type;
    స్థిరరాశి xkb_mod_సూచీ_t ndx = Xkb_Mod_Name_గమ్యం_సూచీ(mods, క్షేత్రం_2, mod_type);
    ఒకవేళ (ndx == XKB_MOD_INVALID)
        తిరిగి false;

    *val_rtrn = (UINT32_C(1) << ndx);
    తిరిగి true;
}

bool
సమాసం_Resolve_Boolean(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                   bool *సమితి_rtrn)
{
    bool ok = false;
    స్థిరరాశి అక్షరం *ident;

    ఎంపిక (సమాసం->common.type) {
    స్థితి STMT_సమాసం_BOOLEAN_LITERAL:
        *సమితి_rtrn = సమాసం->boolean.సమితి;
        తిరిగి true;

    స్థితి STMT_సమాసం_STRING_LITERAL:
    స్థితి STMT_సమాసం_INTEGER_LITERAL:
    స్థితి STMT_సమాసం_FLOAT_LITERAL:
    స్థితి STMT_సమాసం_KEYNAME_LITERAL:
    స్థితి STMT_సమాసం_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_గమ్యం_string(సమాసం->common.type));
        తిరిగి false;

    స్థితి STMT_సమాసం_IDENT:
        ident = xkb_atom_పాఠ్యం(ctx, సమాసం->ident.ident);
        ఒకవేళ (ident) {
            ఒకవేళ (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *సమితి_rtrn = true;
                తిరిగి true;
            }
            లేకపోతే ఒకవేళ (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *సమితి_rtrn = false;
                తిరిగి true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        తిరిగి false;

    స్థితి STMT_సమాసం_క్షేత్రం_REF:
        log_err(ctx, XKB_ERROR_INVALID_వ్యక్తీకరణ_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_పాఠ్యం(ctx, సమాసం->క్షేత్రం_ref.element),
                xkb_atom_పాఠ్యం(ctx, సమాసం->క్షేత్రం_ref.క్షేత్రం_2));
        తిరిగి false;

    స్థితి STMT_సమాసం_INVERT:
    స్థితి STMT_సమాసం_NOT:
        ok = సమాసం_Resolve_Boolean(ctx, సమాసం->unary.బిడ్డ, సమితి_rtrn);
        ఒకవేళ (ok)
            *సమితి_rtrn = !*సమితి_rtrn;
        తిరిగి ok;
    స్థితి STMT_సమాసం_కూడు:
    స్థితి STMT_సమాసం_వ్యవకలనం:
    స్థితి STMT_సమాసం_MULTIPLY:
    స్థితి STMT_సమాసం_DIVIDE:
    స్థితి STMT_సమాసం_కేటాయించు:
    స్థితి STMT_సమాసం_NEGATE:
    స్థితి STMT_సమాసం_UNARY_PLUS:
    స్థితి STMT_సమాసం_ఖాళీలు_LIST:
    స్థితి STMT_సమాసం_ACTION_DECL:
    స్థితి STMT_సమాసం_ACTION_LIST:
    స్థితి STMT_సమాసం_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_గమ్యం_string(సమాసం->common.type));
        ఆపు;

    అప్రమేయ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 సమాసం->common.type);
        ఆపు;
    }

    తిరిగి false;
}

స్థిర bool
సమాసం_Resolve_Integer_Lookup(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, స్థిరరాశి శూన్య *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    సమాసం_Def *ఎడమ, *కుడి;

    ఎంపిక (సమాసం->common.type) {
    స్థితి STMT_సమాసం_INTEGER_LITERAL:
        *val_rtrn = సమాసం->integer.ival;
        తిరిగి true;

    స్థితి STMT_సమాసం_STRING_LITERAL:
    స్థితి STMT_సమాసం_FLOAT_LITERAL:
    స్థితి STMT_సమాసం_BOOLEAN_LITERAL:
    స్థితి STMT_సమాసం_KEYNAME_LITERAL:
    స్థితి STMT_సమాసం_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_గమ్యం_string(సమాసం->common.type));
        తిరిగి false;

    స్థితి STMT_సమాసం_IDENT:
        ఒకవేళ (lookup)
            ok = lookup(ctx, lookupPriv, సమాసం->ident.ident, &u, pending);

        ఒకవేళ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_పాఠ్యం(ctx, సమాసం->ident.ident));
        లేకపోతే
            *val_rtrn = (int64_t) u;

        ఒకవేళ (pending && *pending) {
            /* Postpone computation */
            తిరిగి false;
        }

        తిరిగి ok;

    స్థితి STMT_సమాసం_క్షేత్రం_REF:
        log_err(ctx, XKB_ERROR_INVALID_వ్యక్తీకరణ_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_పాఠ్యం(ctx, సమాసం->క్షేత్రం_ref.element),
                xkb_atom_పాఠ్యం(ctx, సమాసం->క్షేత్రం_ref.క్షేత్రం_2));
        తిరిగి false;

    స్థితి STMT_సమాసం_కూడు:
    స్థితి STMT_సమాసం_వ్యవకలనం:
    స్థితి STMT_సమాసం_MULTIPLY:
    స్థితి STMT_సమాసం_DIVIDE:
        ఎడమ = సమాసం->ద్విమాన.ఎడమ;
        కుడి = సమాసం->ద్విమాన.కుడి;
        ఒకవేళ (!సమాసం_Resolve_Integer_Lookup(ctx, ఎడమ, &l, pending, lookup, lookupPriv) ||
            !సమాసం_Resolve_Integer_Lookup(ctx, కుడి, &r, pending, lookup, lookupPriv))
            తిరిగి false;

        ఎంపిక (సమాసం->common.type) {
        స్థితి STMT_సమాసం_కూడు:
            ఒకవేళ (ckd_కూడు(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_అధికప్రవాహం,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                తిరిగి false;
            }
            ఆపు;
        స్థితి STMT_సమాసం_వ్యవకలనం:
            ఒకవేళ (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_అధికప్రవాహం,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                తిరిగి false;
            }
            ఆపు;
        స్థితి STMT_సమాసం_MULTIPLY:
            ఒకవేళ (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_అధికప్రవాహం,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                తిరిగి false;
            }
            ఆపు;
        స్థితి STMT_సమాసం_DIVIDE:
            ఒకవేళ (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                తిరిగి false;
            }
            *val_rtrn = l / r;
            ఆపు;
        అప్రమేయ:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_గమ్యం_string(సమాసం->common.type));
            తిరిగి false;
        }

        తిరిగి true;

    స్థితి STMT_సమాసం_కేటాయించు:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ఆపు;

    స్థితి STMT_సమాసం_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        తిరిగి false;

    స్థితి STMT_సమాసం_INVERT:
    స్థితి STMT_సమాసం_NEGATE:
        ఎడమ = సమాసం->unary.బిడ్డ;
        ఒకవేళ (!సమాసం_Resolve_Integer_Lookup(ctx, ఎడమ, &l, pending, lookup, lookupPriv))
            తిరిగి false;

        *val_rtrn = (సమాసం->common.type == STMT_సమాసం_NEGATE ? -l : ~l);
        తిరిగి true;

    స్థితి STMT_సమాసం_UNARY_PLUS:
        ఎడమ = సమాసం->unary.బిడ్డ;
        తిరిగి సమాసం_Resolve_Integer_Lookup(ctx, ఎడమ, val_rtrn, pending, lookup,
                                        lookupPriv);

    అప్రమేయ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 సమాసం->common.type);
        ఆపు;
    }

    తిరిగి false;
}

bool
సమాసం_Resolve_Integer(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                   int64_t *val_rtrn)
{
    తిరిగి సమాసం_Resolve_Integer_Lookup(ctx, సమాసం, val_rtrn, ఖాళీ, ఖాళీ, ఖాళీ);
}

గణన xkb_parser_error
సమాసం_Resolve_సమూహం(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                 స్థిరరాశి సమాసం_Def *సమాసం, bool absolute,
                 xkb_layout_సూచీ_t *సమూహం_rtrn, bool *pending)
{
    స్థిర స్థిరరాశి Lookup_నమోదు pending_సమూహం_సూచీ_పేర్లు[] = {
        { సమూహం_చివరి_సూచీ_NAME, 0 },
        { ఖాళీ, 0 }
    };

    స్థిరరాశి నిర్మాణం named_integer_నమూనా సమూహం_name_నమూనా = {
        /* Prefix is title-cased, because it is also used in error messages */
        .ఉపసర్గ = "Group",
        .ఉపసర్గ_నిడివి = పరిమాణం("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_ముసుగు = false,
        .entries = keymap_info->lookup.సమూహం_సూచీ_పేర్లు,
        .pending_entries = pending_సమూహం_సూచీ_పేర్లు,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
    };

    int64_t ఫలితం = 0;
    ఒకవేళ (!సమాసం_Resolve_Integer_Lookup(keymap_info->keymap.ctx, సమాసం, &ఫలితం, pending,
                                  Named_Integer_నమూనా_Lookup, &సమూహం_name_నమూనా))
        తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    ఒకవేళ (ఫలితం < (int64_t)absolute || ఫలితం > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                ఫలితం, absolute, keymap_info->features.max_groups);
        తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *సమూహం_rtrn = (xkb_layout_సూచీ_t) ఫలితం;
    తిరిగి PARSER_SUCCESS;
}

bool
సమాసం_Resolve_Level(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                 xkb_level_సూచీ_t *level_rtrn)
{
    స్థిర స్థిరరాశి నిర్మాణం named_integer_నమూనా level_name_నమూనా = {
        /* Prefix is title-cased, because it is also used in error messages */
        .ఉపసర్గ = "Level",
        .ఉపసర్గ_నిడివి = పరిమాణం("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_ముసుగు = false,
        .entries = ఖాళీ,
        .error_id = XKB_ERROR_UNSUPPORTED_జరుపు_LEVEL,
    };
    int64_t ఫలితం = 0;
    ఒకవేళ (!సమాసం_Resolve_Integer_Lookup(ctx, సమాసం, &ఫలితం, ఖాళీ,
                                  Named_Integer_నమూనా_Lookup,
                                  &level_name_నమూనా))
        తిరిగి false;

    ఒకవేళ (ఫలితం < 1 || ఫలితం > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_జరుపు_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                ఫలితం, XKB_LEVEL_MAX_IMPL);
        తిరిగి false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_సూచీ_t) (ఫలితం - 1);
    తిరిగి true;
}

bool
సమాసం_Resolve_Button(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం, int64_t *btn_rtrn)
{
    తిరిగి సమాసం_Resolve_Integer_Lookup(ctx, సమాసం, btn_rtrn, ఖాళీ, SimpleLookup,
                                    button_పేర్లు);
}

bool
సమాసం_Resolve_String(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                  xkb_atom_t *val_rtrn)
{
    ఎంపిక (సమాసం->common.type) {
    స్థితి STMT_సమాసం_STRING_LITERAL:
        *val_rtrn = సమాసం->string.str;
        తిరిగి true;

    స్థితి STMT_సమాసం_INTEGER_LITERAL:
    స్థితి STMT_సమాసం_FLOAT_LITERAL:
    స్థితి STMT_సమాసం_BOOLEAN_LITERAL:
    స్థితి STMT_సమాసం_KEYNAME_LITERAL:
    స్థితి STMT_సమాసం_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Found %s, expected a string\n",
                stmt_type_గమ్యం_string(సమాసం->common.type));
        తిరిగి false;

    స్థితి STMT_సమాసం_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_పాఠ్యం(ctx, సమాసం->ident.ident));
        తిరిగి false;

    స్థితి STMT_సమాసం_క్షేత్రం_REF:
        log_err(ctx, XKB_ERROR_INVALID_వ్యక్తీకరణ_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_పాఠ్యం(ctx, సమాసం->క్షేత్రం_ref.element),
                xkb_atom_పాఠ్యం(ctx, సమాసం->క్షేత్రం_ref.క్షేత్రం_2));
        తిరిగి false;

    స్థితి STMT_సమాసం_కూడు:
    స్థితి STMT_సమాసం_వ్యవకలనం:
    స్థితి STMT_సమాసం_MULTIPLY:
    స్థితి STMT_సమాసం_DIVIDE:
    స్థితి STMT_సమాసం_కేటాయించు:
    స్థితి STMT_సమాసం_NEGATE:
    స్థితి STMT_సమాసం_INVERT:
    స్థితి STMT_సమాసం_NOT:
    స్థితి STMT_సమాసం_UNARY_PLUS:
    స్థితి STMT_సమాసం_ఖాళీలు_LIST:
    స్థితి STMT_సమాసం_ACTION_DECL:
    స్థితి STMT_సమాసం_ACTION_LIST:
    స్థితి STMT_సమాసం_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_గమ్యం_string(సమాసం->common.type));
        తిరిగి false;

    అప్రమేయ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 సమాసం->common.type);
        ఆపు;
    }
    తిరిగి false;
}

bool
సమాసం_Resolve_Enum(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                uint32_t *val_rtrn, స్థిరరాశి Lookup_నమోదు *విలువలు)
{
    ఒకవేళ (సమాసం->common.type != STMT_సమాసం_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_గమ్యం_string(సమాసం->common.type));
        తిరిగి false;
    }

    ఒకవేళ (!SimpleLookup(ctx, విలువలు, సమాసం->ident.ident, val_rtrn, ఖాళీ)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_పాఠ్యం(ctx, సమాసం->ident.ident));
        అయితే (విలువలు && విలువలు->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", విలువలు->name);
            విలువలు++;
        }
        తిరిగి false;
    }

    తిరిగి true;
}

స్థిర bool
సమాసం_Resolve_ముసుగు_Lookup(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, స్థిరరాశి శూన్య *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    సమాసం_Def *ఎడమ, *కుడి;
    స్థిరరాశి అక్షరం *bogus = ఖాళీ;

    ఎంపిక (సమాసం->common.type) {
    స్థితి STMT_సమాసం_INTEGER_LITERAL:
        ఒకవేళ (సమాసం->integer.ival < 0 || సమాసం->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    సమాసం->integer.ival < 0 ? "-" : "",
                    imaxabs(సమాసం->integer.ival),
                    UINT32_MAX);
            తిరిగి false;
        }
        *val_rtrn = (uint32_t) సమాసం->integer.ival;
        తిరిగి true;

    స్థితి STMT_సమాసం_STRING_LITERAL:
    స్థితి STMT_సమాసం_FLOAT_LITERAL:
    స్థితి STMT_సమాసం_BOOLEAN_LITERAL:
    స్థితి STMT_సమాసం_KEYNAME_LITERAL:
    స్థితి STMT_సమాసం_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_గమ్యం_string(సమాసం->common.type));
        తిరిగి false;

    స్థితి STMT_సమాసం_IDENT:
        ok = lookup(ctx, lookupPriv, సమాసం->ident.ident, val_rtrn, pending);
        ఒకవేళ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_పాఠ్యం(ctx, సమాసం->ident.ident));
        ఒకవేళ (pending && *pending) {
            /* Postpone computation */
            తిరిగి false;
        }
        తిరిగి ok;

    స్థితి STMT_సమాసం_క్షేత్రం_REF:
        log_err(ctx, XKB_ERROR_INVALID_వ్యక్తీకరణ_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_పాఠ్యం(ctx, సమాసం->క్షేత్రం_ref.element),
                xkb_atom_పాఠ్యం(ctx, సమాసం->క్షేత్రం_ref.క్షేత్రం_2));
        తిరిగి false;

    స్థితి STMT_సమాసం_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    స్థితి STMT_సమాసం_ACTION_DECL:
        ఒకవేళ (bogus == ఖాళీ)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        తిరిగి false;

    స్థితి STMT_సమాసం_కూడు:
    స్థితి STMT_సమాసం_వ్యవకలనం:
    స్థితి STMT_సమాసం_MULTIPLY:
    స్థితి STMT_సమాసం_DIVIDE:
        ఎడమ = సమాసం->ద్విమాన.ఎడమ;
        కుడి = సమాసం->ద్విమాన.కుడి;
        ఒకవేళ (!సమాసం_Resolve_ముసుగు_Lookup(ctx, ఎడమ, &l, pending, lookup, lookupPriv) ||
            !సమాసం_Resolve_ముసుగు_Lookup(ctx, కుడి, &r, pending, lookup, lookupPriv))
            తిరిగి false;

        ఎంపిక (సమాసం->common.type) {
        స్థితి STMT_సమాసం_కూడు:
            *val_rtrn = l | r;
            ఆపు;
        స్థితి STMT_సమాసం_వ్యవకలనం:
            *val_rtrn = l & (~r);
            ఆపు;
        స్థితి STMT_సమాసం_MULTIPLY:
        స్థితి STMT_సమాసం_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (సమాసం->common.type == STMT_సమాసం_DIVIDE ? "divide" : "multiply"));
            తిరిగి false;
        అప్రమేయ:
            ఆపు;
        }

        తిరిగి true;

    స్థితి STMT_సమాసం_కేటాయించు:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ఆపు;

    స్థితి STMT_సమాసం_INVERT:
        ఎడమ = సమాసం->unary.బిడ్డ;
        ఒకవేళ (!సమాసం_Resolve_Integer_Lookup(ctx, ఎడమ, &v, pending, lookup, lookupPriv))
            తిరిగి false;
        ఒకవేళ (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            తిరిగి false;
        }
        *val_rtrn = ~(uint32_t) v;
        తిరిగి true;

    స్థితి STMT_సమాసం_UNARY_PLUS:
    స్థితి STMT_సమాసం_NEGATE:
    స్థితి STMT_సమాసం_NOT:
        ఎడమ = సమాసం->unary.బిడ్డ;
        ఒకవేళ (!సమాసం_Resolve_Integer_Lookup(ctx, ఎడమ, &v, pending, lookup, lookupPriv))
            తిరిగి false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_గమ్యం_operator_char(సమాసం->common.type));
        తిరిగి false;

    అప్రమేయ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 సమాసం->common.type);
        ఆపు;
    }

    తిరిగి false;
}

bool
సమాసం_Resolve_ముసుగు(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                uint32_t *ముసుగు_rtrn, స్థిరరాశి Lookup_నమోదు *విలువలు)
{
    తిరిగి సమాసం_Resolve_ముసుగు_Lookup(ctx, సమాసం, ముసుగు_rtrn, ఖాళీ, SimpleLookup, విలువలు);
}

bool
సమాసం_Resolve_Mod_ముసుగు(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *సమాసం,
                   గణన mod_type mod_type, స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
                   xkb_mod_ముసుగు_t *ముసుగు_rtrn)
{
    Lookup_Mod_ముసుగు_Priv priv = { .mods = mods, .mod_type = mod_type };
    తిరిగి సమాసం_Resolve_ముసుగు_Lookup(ctx, సమాసం, ముసుగు_rtrn, ఖాళీ, Lookup_Mod_ముసుగు, &priv);
}

bool
సమాసం_Resolve_Mod(నిర్మాణం xkb_context *ctx, స్థిరరాశి సమాసం_Def *def,
               గణన mod_type mod_type, స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
               xkb_mod_సూచీ_t *ndx_rtrn)
{
    ఒకవేళ (def->common.type != STMT_సమాసం_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_గమ్యం_string(def->common.type));
        తిరిగి false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_సూచీ_t ndx = Xkb_Mod_Name_గమ్యం_సూచీ(mods, name, mod_type);
    ఒకవేళ (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_పాఠ్యం(ctx, name));
        తిరిగి false;
    }

    *ndx_rtrn = ndx;
    తిరిగి true;
}

bool
సమాసం_Resolve_సమూహం_ముసుగు(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                     స్థిరరాశి సమాసం_Def *సమాసం, xkb_layout_ముసుగు_t *సమూహం_rtrn,
                     bool *pending_rtrn)
{
    స్థిర స్థిరరాశి Lookup_నమోదు pending_సమూహం_ముసుగు_పేర్లు[] = {
        { సమూహం_చివరి_సూచీ_NAME, 0 },
        { ఖాళీ, 0 }
    };

    స్థిరరాశి నిర్మాణం named_integer_నమూనా సమూహం_name_నమూనా = {
        /* Prefix is title-cased, because it is also used in error messages */
        .ఉపసర్గ = "Group",
        .ఉపసర్గ_నిడివి = పరిమాణం("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_ముసుగు = true,
        .entries = keymap_info->lookup.సమూహం_ముసుగు_పేర్లు,
        .pending_entries = pending_సమూహం_ముసుగు_పేర్లు,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2
    };

    తిరిగి సమాసం_Resolve_ముసుగు_Lookup(keymap_info->keymap.ctx, సమాసం, సమూహం_rtrn,
                                 pending_rtrn, Named_Integer_నమూనా_Lookup,
                                 &సమూహం_name_నమూనా);
}
