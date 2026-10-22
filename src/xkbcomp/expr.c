/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "keysym.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-numbers.h"
#ಅನ್ತರ್ಭಾವ "util-checked-arithmetic.h"

ಪ್ರಕಾರನಿರ್ದೇಶ bool (*IdentLookupFunc)(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಶೂನ್ಯ *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
               ಧ್ರುವ ವರ್ಣ **elem_rtrn, ಧ್ರುವ ವರ್ಣ **field_rtrn,
               ಅಭಿವ್ಯಕ್ತಿ_Def **index_rtrn)
{
    ವಿಕಲ್ಪ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT:
        *elem_rtrn = ರಿಕ್ತ;
        *field_rtrn = xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident);
        *index_rtrn = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ (*field_rtrn != ರಿಕ್ತ);
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->field_ref.field);
        *index_rtrn = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ (*elem_rtrn != ರಿಕ್ತ && *field_rtrn != ರಿಕ್ತ);
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->array_ref.field);
        *index_rtrn = ಅಭಿವ್ಯಕ್ತಿ->array_ref.entry;
        ಯದಿ (ಅಭಿವ್ಯಕ್ತಿ->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ false;
        ಯದಿ (*field_rtrn == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ false;
        ಪ್ರತ್ಯಾಗಮ true;
    ಸ್ವಭಾವ:
        ಭಙ್ಗ;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", ಅಭಿವ್ಯಕ್ತಿ->common.type);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
SimpleLookup(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಶೂನ್ಯ *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    ಯದಿ (!priv || field == XKB_ATOM_NONE)
        ಪ್ರತ್ಯಾಗಮ false;

    ಧ್ರುವ ವರ್ಣ *str = xkb_atom_text(ctx, field);
    ಪ್ರತಿ (ಧ್ರುವ LookupEntry *entry = priv; entry && entry->name; entry++) {
        ಯದಿ (istreq(str, entry->name)) {
            *val_rtrn = entry->ಮೌಲ್ಯ;
            ಪ್ರತ್ಯಾಗಮ true;
        }
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

ಸಂರಚನಾ named_integer_pattern {
    ಧ್ರುವ ವರ್ಣ *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    ಧ್ರುವ LookupEntry *entries;
    ಧ್ರುವ LookupEntry *pending_entries;
    bool is_mask;
    ಗಣನಾ xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ಸ್ಥಿರ bool
NamedIntegerPatternLookup(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಶೂನ್ಯ *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    ಯದಿ (!priv || field == XKB_ATOM_NONE)
        ಪ್ರತ್ಯಾಗಮ false;

    ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ str = xkb_atom_text(ctx, field);
    ಧ್ರುವ ಸಂರಚನಾ named_integer_pattern * ಧ್ರುವ pattern = priv;

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    ಯದಿ (ಎಣಿಕೆ > 0 && *(str + pattern->prefix_length + ಎಣಿಕೆ) == '\0') {
        ಯದಿ (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            ಪ್ರತ್ಯಾಗಮ false;
        }
        ಯದಿ (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            ಪ್ರತಿಜ್ಞಾ(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        ಪ್ರತ್ಯಾಗಮ true;
    } ಅನ್ಯಥಾ {
        ಯದಿ (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, ರಿಕ್ತ)) {
            ಪ್ರತ್ಯಾಗಮ true;
        }
        ಯದಿ (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, ರಿಕ್ತ)) {
            *pending_rtrn = true;
            ಪ್ರತ್ಯಾಗಮ true;
        }
        ಪ್ರತ್ಯಾಗಮ false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods;
    ಗಣನಾ mod_type mod_type;
} LookupModMaskPriv;

ಸ್ಥಿರ bool
LookupModMask(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಶೂನ್ಯ *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    ಧ್ರುವ ವರ್ಣ *str = xkb_atom_text(ctx, field);
    ಯದಿ (!str)
        ಪ್ರತ್ಯಾಗಮ false;

    ಯದಿ (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಯದಿ (istreq(str, "none")) {
        *val_rtrn = 0;
        ಪ್ರತ್ಯಾಗಮ true;
    }

    ಧ್ರುವ LookupModMaskPriv *arg = priv;
    ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods = arg->mods;
    ಧ್ರುವ ಗಣನಾ mod_type mod_type = arg->mod_type;
    ಧ್ರುವ xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    ಯದಿ (ndx == XKB_MOD_INVALID)
        ಪ್ರತ್ಯಾಗಮ false;

    *val_rtrn = (UINT32_C(1) << ndx);
    ಪ್ರತ್ಯಾಗಮ true;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   bool *set_rtrn)
{
    bool ok = false;
    ಧ್ರುವ ವರ್ಣ *ident;

    ವಿಕಲ್ಪ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL:
        *set_rtrn = ಅಭಿವ್ಯಕ್ತಿ->boolean.set;
        ಪ್ರತ್ಯಾಗಮ true;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT:
        ident = xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident);
        ಯದಿ (ident) {
            ಯದಿ (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                ಪ್ರತ್ಯಾಗಮ true;
            }
            ಅನ್ಯಥಾ ಯದಿ (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                ಪ್ರತ್ಯಾಗಮ true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->field_ref.element),
                xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->field_ref.field));
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
        ok = ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(ctx, ಅಭಿವ್ಯಕ್ತಿ->unary.child, set_rtrn);
        ಯದಿ (ok)
            *set_rtrn = !*set_rtrn;
        ಪ್ರತ್ಯಾಗಮ ok;
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ADD:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_SUBTRACT:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ASSIGN:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_EMPTY_LIST:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 ಅಭಿವ್ಯಕ್ತಿ->common.type);
        ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ಧ್ರುವ ಶೂನ್ಯ *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಎಡ, *ಬಲ;

    ವಿಕಲ್ಪ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL:
        *val_rtrn = ಅಭಿವ್ಯಕ್ತಿ->integer.ival;
        ಪ್ರತ್ಯಾಗಮ true;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT:
        ಯದಿ (lookup)
            ok = lookup(ctx, lookupPriv, ಅಭಿವ್ಯಕ್ತಿ->ident.ident, &u, pending);

        ಯದಿ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident));
        ಅನ್ಯಥಾ
            *val_rtrn = (int64_t) u;

        ಯದಿ (pending && *pending) {
            /* Postpone computation */
            ಪ್ರತ್ಯಾಗಮ false;
        }

        ಪ್ರತ್ಯಾಗಮ ok;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->field_ref.element),
                xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->field_ref.field));
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ADD:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_SUBTRACT:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಎಡ;
        ಬಲ = ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಬಲ;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಎಡ, &l, pending, lookup, lookupPriv) ||
            !ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಬಲ, &r, pending, lookup, lookupPriv))
            ಪ್ರತ್ಯಾಗಮ false;

        ವಿಕಲ್ಪ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ADD:
            ಯದಿ (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ಪ್ರತ್ಯಾಗಮ false;
            }
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_SUBTRACT:
            ಯದಿ (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ಪ್ರತ್ಯಾಗಮ false;
            }
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
            ಯದಿ (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ಪ್ರತ್ಯಾಗಮ false;
            }
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
            ಯದಿ (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                ಪ್ರತ್ಯಾಗಮ false;
            }
            *val_rtrn = l / r;
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
            ಪ್ರತ್ಯಾಗಮ false;
        }

        ಪ್ರತ್ಯಾಗಮ true;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ಭಙ್ಗ;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->unary.child;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಎಡ, &l, pending, lookup, lookupPriv))
            ಪ್ರತ್ಯಾಗಮ false;

        *val_rtrn = (ಅಭಿವ್ಯಕ್ತಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ? -l : ~l);
        ಪ್ರತ್ಯಾಗಮ true;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->unary.child;
        ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಎಡ, val_rtrn, pending, lookup,
                                        lookupPriv);

    ಸ್ವಭಾವ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 ಅಭಿವ್ಯಕ್ತಿ->common.type);
        ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   int64_t *val_rtrn)
{
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಅಭಿವ್ಯಕ್ತಿ, val_rtrn, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ);
}

ಗಣನಾ xkb_parser_error
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                 ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    ಸ್ಥಿರ ಧ್ರುವ LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { ರಿಕ್ತ, 0 }
    };

    ಧ್ರುವ ಸಂರಚನಾ named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = ಪರಿಮಾಣ("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(keymap_info->keymap.ctx, ಅಭಿವ್ಯಕ್ತಿ, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    ಯದಿ (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Level(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                 xkb_level_index_t *level_rtrn)
{
    ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = ಪರಿಮಾಣ("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = ರಿಕ್ತ,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಅಭಿವ್ಯಕ್ತಿ, &result, ರಿಕ್ತ,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        ಪ್ರತ್ಯಾಗಮ false;

    ಯದಿ (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    ಪ್ರತ್ಯಾಗಮ true;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Button(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, int64_t *btn_rtrn)
{
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಅಭಿವ್ಯಕ್ತಿ, btn_rtrn, ರಿಕ್ತ, SimpleLookup,
                                    buttonNames);
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                  xkb_atom_t *val_rtrn)
{
    ವಿಕಲ್ಪ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL:
        *val_rtrn = ಅಭಿವ್ಯಕ್ತಿ->string.str;
        ಪ್ರತ್ಯಾಗಮ true;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident));
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->field_ref.element),
                xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->field_ref.field));
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ADD:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_SUBTRACT:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ASSIGN:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_EMPTY_LIST:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಪ್ರತ್ಯಾಗಮ false;

    ಸ್ವಭಾವ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 ಅಭಿವ್ಯಕ್ತಿ->common.type);
        ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ false;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                uint32_t *val_rtrn, ಧ್ರುವ LookupEntry *ಮೌಲ್ಯಗಳು)
{
    ಯದಿ (ಅಭಿವ್ಯಕ್ತಿ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಯದಿ (!SimpleLookup(ctx, ಮೌಲ್ಯಗಳು, ಅಭಿವ್ಯಕ್ತಿ->ident.ident, val_rtrn, ರಿಕ್ತ)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident));
        ಯಾವತ್ (ಮೌಲ್ಯಗಳು && ಮೌಲ್ಯಗಳು->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", ಮೌಲ್ಯಗಳು->name);
            ಮೌಲ್ಯಗಳು++;
        }
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask_Lookup(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ಧ್ರುವ ಶೂನ್ಯ *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಎಡ, *ಬಲ;
    ಧ್ರುವ ವರ್ಣ *bogus = ರಿಕ್ತ;

    ವಿಕಲ್ಪ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL:
        ಯದಿ (ಅಭಿವ್ಯಕ್ತಿ->integer.ival < 0 || ಅಭಿವ್ಯಕ್ತಿ->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    ಅಭಿವ್ಯಕ್ತಿ->integer.ival < 0 ? "-" : "",
                    imaxabs(ಅಭಿವ್ಯಕ್ತಿ->integer.ival),
                    UINT32_MAX);
            ಪ್ರತ್ಯಾಗಮ false;
        }
        *val_rtrn = (uint32_t) ಅಭಿವ್ಯಕ್ತಿ->integer.ival;
        ಪ್ರತ್ಯಾಗಮ true;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT:
        ok = lookup(ctx, lookupPriv, ಅಭಿವ್ಯಕ್ತಿ->ident.ident, val_rtrn, pending);
        ಯದಿ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident));
        ಯದಿ (pending && *pending) {
            /* Postpone computation */
            ಪ್ರತ್ಯಾಗಮ false;
        }
        ಪ್ರತ್ಯಾಗಮ ok;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->field_ref.element),
                xkb_atom_text(ctx, ಅಭಿವ್ಯಕ್ತಿ->field_ref.field));
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL:
        ಯದಿ (bogus == ರಿಕ್ತ)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        ಪ್ರತ್ಯಾಗಮ false;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ADD:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_SUBTRACT:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಎಡ;
        ಬಲ = ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಬಲ;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask_Lookup(ctx, ಎಡ, &l, pending, lookup, lookupPriv) ||
            !ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask_Lookup(ctx, ಬಲ, &r, pending, lookup, lookupPriv))
            ಪ್ರತ್ಯಾಗಮ false;

        ವಿಕಲ್ಪ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ADD:
            *val_rtrn = l | r;
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_SUBTRACT:
            *val_rtrn = l & (~r);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
        ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (ಅಭಿವ್ಯಕ್ತಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE ? "divide" : "multiply"));
            ಪ್ರತ್ಯಾಗಮ false;
        ಸ್ವಭಾವ:
            ಭಙ್ಗ;
        }

        ಪ್ರತ್ಯಾಗಮ true;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ಭಙ್ಗ;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->unary.child;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಎಡ, &v, pending, lookup, lookupPriv))
            ಪ್ರತ್ಯಾಗಮ false;
        ಯದಿ (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            ಪ್ರತ್ಯಾಗಮ false;
        }
        *val_rtrn = ~(uint32_t) v;
        ಪ್ರತ್ಯಾಗಮ true;

    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
    ಪಕ್ಷ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->unary.child;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಎಡ, &v, pending, lookup, lookupPriv))
            ಪ್ರತ್ಯಾಗಮ false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಪ್ರತ್ಯಾಗಮ false;

    ಸ್ವಭಾವ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 ಅಭಿವ್ಯಕ್ತಿ->common.type);
        ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                uint32_t *mask_rtrn, ಧ್ರುವ LookupEntry *ಮೌಲ್ಯಗಳು)
{
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask_Lookup(ctx, ಅಭಿವ್ಯಕ್ತಿ, mask_rtrn, ರಿಕ್ತ, SimpleLookup, ಮೌಲ್ಯಗಳು);
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   ಗಣನಾ mod_type mod_type, ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask_Lookup(ctx, ಅಭಿವ್ಯಕ್ತಿ, mask_rtrn, ರಿಕ್ತ, LookupModMask, &priv);
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *def,
               ಗಣನಾ mod_type mod_type, ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    ಯದಿ (def->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    ಯದಿ (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    *ndx_rtrn = ndx;
    ಪ್ರತ್ಯಾಗಮ true;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group_Mask(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                     ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    ಸ್ಥಿರ ಧ್ರುವ LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { ರಿಕ್ತ, 0 }
    };

    ಧ್ರುವ ಸಂರಚನಾ named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = ಪರಿಮಾಣ("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    ಪ್ರತ್ಯಾಗಮ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask_Lookup(keymap_info->keymap.ctx, ಅಭಿವ್ಯಕ್ತಿ, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
