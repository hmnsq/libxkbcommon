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

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ bool (*IdentLookupFunc)(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *priv,
                                xkb_atom_t ಕ್ಷೇತ್ರ_2, uint32_t *val_rtrn,
                                bool *pending);

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **elem_rtrn, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **ಕ್ಷೇತ್ರ_rtrn,
               ಅಭಿವ್ಯಕ್ತಿ_Def **ಸೂಚ್ಯಂಕ_rtrn)
{
    ಆಯ್ಕೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT:
        *elem_rtrn = ಖಾಲಿ;
        *ಕ್ಷೇತ್ರ_rtrn = xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident);
        *ಸೂಚ್ಯಂಕ_rtrn = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ (*ಕ್ಷೇತ್ರ_rtrn != ಖಾಲಿ);
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕ್ಷೇತ್ರ_REF:
        *elem_rtrn = xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.element);
        *ಕ್ಷೇತ್ರ_rtrn = xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.ಕ್ಷೇತ್ರ_2);
        *ಸೂಚ್ಯಂಕ_rtrn = ಖಾಲಿ;
        ಹಿಂತಿರುಗಿ (*elem_rtrn != ಖಾಲಿ && *ಕ್ಷೇತ್ರ_rtrn != ಖಾಲಿ);
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF:
        *elem_rtrn = xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->array_ref.element);
        *ಕ್ಷೇತ್ರ_rtrn = xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->array_ref.ಕ್ಷೇತ್ರ_2);
        *ಸೂಚ್ಯಂಕ_rtrn = ಅಭಿವ್ಯಕ್ತಿ->array_ref.ನಮೂದು;
        ಆದರೆ (ಅಭಿವ್ಯಕ್ತಿ->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ false;
        ಆದರೆ (*ಕ್ಷೇತ್ರ_rtrn == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ false;
        ಹಿಂತಿರುಗಿ true;
    ಪೂರ್ವನಿಯೋಜಿತ:
        ನಿಲ್ಲಿಸು;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", ಅಭಿವ್ಯಕ್ತಿ->common.type);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
SimpleLookup(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *priv, xkb_atom_t ಕ್ಷೇತ್ರ_2,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    ಆದರೆ (!priv || ಕ್ಷೇತ್ರ_2 == XKB_ATOM_NONE)
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = xkb_atom_ಪಠ್ಯ(ctx, ಕ್ಷೇತ್ರ_2);
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *ನಮೂದು = priv; ನಮೂದು && ನಮೂದು->name; ನಮೂದು++) {
        ಆದರೆ (istreq(str, ನಮೂದು->name)) {
            *val_rtrn = ನಮೂದು->ಮೌಲ್ಯ;
            ಹಿಂತಿರುಗಿ true;
        }
    }

    ಹಿಂತಿರುಗಿ false;
}

ರಚನೆ named_integer_ನಮೂನೆ {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
    ಗಾತ್ರ_t ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ;
    uint32_t min;
    uint32_t max;
    ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *entries;
    ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *pending_entries;
    bool is_ಮುಖವಾಡ;
    ಎಣಿಕೆ xkb_ಸಂದೇಶ_ಸಂಕೇತ error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ಸ್ಥಿರ bool
Named_Integer_ನಮೂನೆ_Lookup(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *priv,
                          xkb_atom_t ಕ್ಷೇತ್ರ_2, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    ಆದರೆ (!priv || ಕ್ಷೇತ್ರ_2 == XKB_ATOM_NONE)
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ str = xkb_atom_ಪಠ್ಯ(ctx, ಕ್ಷೇತ್ರ_2);
    ಸ್ಥಿರಾಂಕ ರಚನೆ named_integer_ನಮೂನೆ * ಸ್ಥಿರಾಂಕ ನಮೂನೆ = priv;

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = (istrneq(str, ನಮೂನೆ->ಪೂರ್ವಪ್ರತ್ಯಯ, ನಮೂನೆ->ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ))
        ? parse_dec_ಗಮ್ಯ_uint32_t(str + ನಮೂನೆ->ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ, SIZE_MAX, val_rtrn)
        : 0;

    ಆದರೆ (ಎಣಿಕೆ_2 > 0 && *(str + ನಮೂನೆ->ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ + ಎಣಿಕೆ_2) == '\0') {
        ಆದರೆ (*val_rtrn < ನಮೂನೆ->min || *val_rtrn > ನಮೂನೆ->max) {
            log_err(ctx, ನಮೂನೆ->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    ನಮೂನೆ->ಪೂರ್ವಪ್ರತ್ಯಯ, *val_rtrn, ನಮೂನೆ->min, ನಮೂನೆ->max);
            ಹಿಂತಿರುಗಿ false;
        }
        ಆದರೆ (ನಮೂನೆ->is_ಮುಖವಾಡ) {
            /* Compute mask (bit 0 = min index) */
            ಪ್ರತಿಜ್ಞಾ(*val_rtrn - ನಮೂನೆ->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - ನಮೂನೆ->min);
        }
        ಹಿಂತಿರುಗಿ true;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (ನಮೂನೆ->entries &&
            SimpleLookup(ctx, ನಮೂನೆ->entries, ಕ್ಷೇತ್ರ_2, val_rtrn, ಖಾಲಿ)) {
            ಹಿಂತಿರುಗಿ true;
        }
        ಆದರೆ (ನಮೂನೆ->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, ನಮೂನೆ->pending_entries, ಕ್ಷೇತ್ರ_2, val_rtrn, ಖಾಲಿ)) {
            *pending_rtrn = true;
            ಹಿಂತಿರುಗಿ true;
        }
        ಹಿಂತಿರುಗಿ false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods;
    ಎಣಿಕೆ mod_type mod_type;
} Lookup_Mod_ಮುಖವಾಡ_Priv;

ಸ್ಥಿರ bool
Lookup_Mod_ಮುಖವಾಡ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *priv, xkb_atom_t ಕ್ಷೇತ್ರ_2,
              xkb_mod_ಮುಖವಾಡ_t *val_rtrn, bool *pending_rtrn)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = xkb_atom_ಪಠ್ಯ(ctx, ಕ್ಷೇತ್ರ_2);
    ಆದರೆ (!str)
        ಹಿಂತಿರುಗಿ false;

    ಆದರೆ (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_ಮುಖವಾಡ_ALL;
        ಹಿಂತಿರುಗಿ true;
    }

    ಆದರೆ (istreq(str, "none")) {
        *val_rtrn = 0;
        ಹಿಂತಿರುಗಿ true;
    }

    ಸ್ಥಿರಾಂಕ Lookup_Mod_ಮುಖವಾಡ_Priv *arg = priv;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods = arg->mods;
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ mod_type mod_type = arg->mod_type;
    ಸ್ಥಿರಾಂಕ xkb_mod_ಸೂಚ್ಯಂಕ_t ndx = Xkb_Mod_Name_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(mods, ಕ್ಷೇತ್ರ_2, mod_type);
    ಆದರೆ (ndx == XKB_MOD_INVALID)
        ಹಿಂತಿರುಗಿ false;

    *val_rtrn = (UINT32_C(1) << ndx);
    ಹಿಂತಿರುಗಿ true;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   bool *ಗಣ_rtrn)
{
    bool ok = false;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ident;

    ಆಯ್ಕೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL:
        *ಗಣ_rtrn = ಅಭಿವ್ಯಕ್ತಿ->boolean.ಗಣ;
        ಹಿಂತಿರುಗಿ true;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_ಗಮ್ಯ_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT:
        ident = xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident);
        ಆದರೆ (ident) {
            ಆದರೆ (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *ಗಣ_rtrn = true;
                ಹಿಂತಿರುಗಿ true;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *ಗಣ_rtrn = false;
                ಹಿಂತಿರುಗಿ true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕ್ಷೇತ್ರ_REF:
        log_err(ctx, XKB_ERROR_INVALID_ಅಭಿವ್ಯಕ್ತಿ_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.element),
                xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.ಕ್ಷೇತ್ರ_2));
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
        ok = ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(ctx, ಅಭಿವ್ಯಕ್ತಿ->unary.ಮಗು, ಗಣ_rtrn);
        ಆದರೆ (ok)
            *ಗಣ_rtrn = !*ಗಣ_rtrn;
        ಹಿಂತಿರುಗಿ ok;
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ನಿಯೋಜಿಸು:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_LIST:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_ಗಮ್ಯ_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 ಅಭಿವ್ಯಕ್ತಿ->common.type);
        ನಿಲ್ಲಿಸು;
    }

    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಎಡ, *ಬಲ;

    ಆಯ್ಕೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL:
        *val_rtrn = ಅಭಿವ್ಯಕ್ತಿ->integer.ival;
        ಹಿಂತಿರುಗಿ true;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_ಗಮ್ಯ_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT:
        ಆದರೆ (lookup)
            ok = lookup(ctx, lookupPriv, ಅಭಿವ್ಯಕ್ತಿ->ident.ident, &u, pending);

        ಆದರೆ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident));
        ಇಲ್ಲದಿದ್ದರೆ
            *val_rtrn = (int64_t) u;

        ಆದರೆ (pending && *pending) {
            /* Postpone computation */
            ಹಿಂತಿರುಗಿ false;
        }

        ಹಿಂತಿರುಗಿ ok;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕ್ಷೇತ್ರ_REF:
        log_err(ctx, XKB_ERROR_INVALID_ಅಭಿವ್ಯಕ್ತಿ_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.element),
                xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.ಕ್ಷೇತ್ರ_2));
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಎಡ;
        ಬಲ = ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಬಲ;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಎಡ, &l, pending, lookup, lookupPriv) ||
            !ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಬಲ, &r, pending, lookup, lookupPriv))
            ಹಿಂತಿರುಗಿ false;

        ಆಯ್ಕೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು:
            ಆದರೆ (ckd_ಕೂಡಿಸು(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_ಅತಿಪ್ರವಾಹ,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ಹಿಂತಿರುಗಿ false;
            }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ:
            ಆದರೆ (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_ಅತಿಪ್ರವಾಹ,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ಹಿಂತಿರುಗಿ false;
            }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
            ಆದರೆ (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_ಅತಿಪ್ರವಾಹ,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ಹಿಂತಿರುಗಿ false;
            }
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
            ಆದರೆ (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                ಹಿಂತಿರುಗಿ false;
            }
            *val_rtrn = l / r;
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_ಗಮ್ಯ_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
            ಹಿಂತಿರುಗಿ false;
        }

        ಹಿಂತಿರುಗಿ true;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ನಿಯೋಜಿಸು:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->unary.ಮಗು;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಎಡ, &l, pending, lookup, lookupPriv))
            ಹಿಂತಿರುಗಿ false;

        *val_rtrn = (ಅಭಿವ್ಯಕ್ತಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ? -l : ~l);
        ಹಿಂತಿರುಗಿ true;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->unary.ಮಗು;
        ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಎಡ, val_rtrn, pending, lookup,
                                        lookupPriv);

    ಪೂರ್ವನಿಯೋಜಿತ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 ಅಭಿವ್ಯಕ್ತಿ->common.type);
        ನಿಲ್ಲಿಸು;
    }

    ಹಿಂತಿರುಗಿ false;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   int64_t *val_rtrn)
{
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಅಭಿವ್ಯಕ್ತಿ, val_rtrn, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ);
}

ಎಣಿಕೆ xkb_parser_error
ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                 ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, bool absolute,
                 xkb_layout_ಸೂಚ್ಯಂಕ_t *ಗುಂಪು_rtrn, bool *pending)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು pending_ಗುಂಪು_ಸೂಚ್ಯಂಕ_ಹೆಸರುಗಳು[] = {
        { ಗುಂಪು_ಕೊನೆಯ_ಸೂಚ್ಯಂಕ_NAME, 0 },
        { ಖಾಲಿ, 0 }
    };

    ಸ್ಥಿರಾಂಕ ರಚನೆ named_integer_ನಮೂನೆ ಗುಂಪು_name_ನಮೂನೆ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .ಪೂರ್ವಪ್ರತ್ಯಯ = "Group",
        .ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ = ಗಾತ್ರ("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_ಮುಖವಾಡ = false,
        .entries = keymap_info->lookup.ಗುಂಪು_ಸೂಚ್ಯಂಕ_ಹೆಸರುಗಳು,
        .pending_entries = pending_ಗುಂಪು_ಸೂಚ್ಯಂಕ_ಹೆಸರುಗಳು,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
    };

    int64_t ಫಲಿತಾಂಶ = 0;
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(keymap_info->keymap.ctx, ಅಭಿವ್ಯಕ್ತಿ, &ಫಲಿತಾಂಶ, pending,
                                  Named_Integer_ನಮೂನೆ_Lookup, &ಗುಂಪು_name_ನಮೂನೆ))
        ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    ಆದರೆ (ಫಲಿತಾಂಶ < (int64_t)absolute || ಫಲಿತಾಂಶ > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                ಫಲಿತಾಂಶ, absolute, keymap_info->features.max_groups);
        ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *ಗುಂಪು_rtrn = (xkb_layout_ಸೂಚ್ಯಂಕ_t) ಫಲಿತಾಂಶ;
    ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Level(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                 xkb_level_ಸೂಚ್ಯಂಕ_t *level_rtrn)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ named_integer_ನಮೂನೆ level_name_ನಮೂನೆ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .ಪೂರ್ವಪ್ರತ್ಯಯ = "Level",
        .ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ = ಗಾತ್ರ("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_ಮುಖವಾಡ = false,
        .entries = ಖಾಲಿ,
        .error_id = XKB_ERROR_UNSUPPORTED_ಸರಿಸು_LEVEL,
    };
    int64_t ಫಲಿತಾಂಶ = 0;
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಅಭಿವ್ಯಕ್ತಿ, &ಫಲಿತಾಂಶ, ಖಾಲಿ,
                                  Named_Integer_ನಮೂನೆ_Lookup,
                                  &level_name_ನಮೂನೆ))
        ಹಿಂತಿರುಗಿ false;

    ಆದರೆ (ಫಲಿತಾಂಶ < 1 || ಫಲಿತಾಂಶ > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_ಸರಿಸು_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                ಫಲಿತಾಂಶ, XKB_LEVEL_MAX_IMPL);
        ಹಿಂತಿರುಗಿ false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_ಸೂಚ್ಯಂಕ_t) (ಫಲಿತಾಂಶ - 1);
    ಹಿಂತಿರುಗಿ true;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Button(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, int64_t *btn_rtrn)
{
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಅಭಿವ್ಯಕ್ತಿ, btn_rtrn, ಖಾಲಿ, SimpleLookup,
                                    button_ಹೆಸರುಗಳು);
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                  xkb_atom_t *val_rtrn)
{
    ಆಯ್ಕೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL:
        *val_rtrn = ಅಭಿವ್ಯಕ್ತಿ->string.str;
        ಹಿಂತಿರುಗಿ true;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Found %s, expected a string\n",
                stmt_type_ಗಮ್ಯ_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident));
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕ್ಷೇತ್ರ_REF:
        log_err(ctx, XKB_ERROR_INVALID_ಅಭಿವ್ಯಕ್ತಿ_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.element),
                xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.ಕ್ಷೇತ್ರ_2));
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ನಿಯೋಜಿಸು:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_LIST:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_ಗಮ್ಯ_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಹಿಂತಿರುಗಿ false;

    ಪೂರ್ವನಿಯೋಜಿತ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 ಅಭಿವ್ಯಕ್ತಿ->common.type);
        ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ false;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                uint32_t *val_rtrn, ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *ಮೌಲ್ಯಗಳು)
{
    ಆದರೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_ಗಮ್ಯ_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (!SimpleLookup(ctx, ಮೌಲ್ಯಗಳು, ಅಭಿವ್ಯಕ್ತಿ->ident.ident, val_rtrn, ಖಾಲಿ)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident));
        ಆಗ (ಮೌಲ್ಯಗಳು && ಮೌಲ್ಯಗಳು->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", ಮೌಲ್ಯಗಳು->name);
            ಮೌಲ್ಯಗಳು++;
        }
        ಹಿಂತಿರುಗಿ false;
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ_Lookup(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    ಅಭಿವ್ಯಕ್ತಿ_Def *ಎಡ, *ಬಲ;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bogus = ಖಾಲಿ;

    ಆಯ್ಕೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_INTEGER_LITERAL:
        ಆದರೆ (ಅಭಿವ್ಯಕ್ತಿ->integer.ival < 0 || ಅಭಿವ್ಯಕ್ತಿ->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    ಅಭಿವ್ಯಕ್ತಿ->integer.ival < 0 ? "-" : "",
                    imaxabs(ಅಭಿವ್ಯಕ್ತಿ->integer.ival),
                    UINT32_MAX);
            ಹಿಂತಿರುಗಿ false;
        }
        *val_rtrn = (uint32_t) ಅಭಿವ್ಯಕ್ತಿ->integer.ival;
        ಹಿಂತಿರುಗಿ true;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_STRING_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_FLOAT_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_ಗಮ್ಯ_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT:
        ok = lookup(ctx, lookupPriv, ಅಭಿವ್ಯಕ್ತಿ->ident.ident, val_rtrn, pending);
        ಆದರೆ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident));
        ಆದರೆ (pending && *pending) {
            /* Postpone computation */
            ಹಿಂತಿರುಗಿ false;
        }
        ಹಿಂತಿರುಗಿ ok;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕ್ಷೇತ್ರ_REF:
        log_err(ctx, XKB_ERROR_INVALID_ಅಭಿವ್ಯಕ್ತಿ_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.element),
                xkb_atom_ಪಠ್ಯ(ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕ್ಷೇತ್ರ_ref.ಕ್ಷೇತ್ರ_2));
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL:
        ಆದರೆ (bogus == ಖಾಲಿ)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಎಡ;
        ಬಲ = ಅಭಿವ್ಯಕ್ತಿ->ದ್ವಿಮಾನ.ಬಲ;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ_Lookup(ctx, ಎಡ, &l, pending, lookup, lookupPriv) ||
            !ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ_Lookup(ctx, ಬಲ, &r, pending, lookup, lookupPriv))
            ಹಿಂತಿರುಗಿ false;

        ಆಯ್ಕೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕೂಡಿಸು:
            *val_rtrn = l | r;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ಕಳೆ:
            *val_rtrn = l & (~r);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_MULTIPLY:
        ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (ಅಭಿವ್ಯಕ್ತಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_DIVIDE ? "divide" : "multiply"));
            ಹಿಂತಿರುಗಿ false;
        ಪೂರ್ವನಿಯೋಜಿತ:
            ನಿಲ್ಲಿಸು;
        }

        ಹಿಂತಿರುಗಿ true;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_ನಿಯೋಜಿಸು:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->unary.ಮಗು;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಎಡ, &v, pending, lookup, lookupPriv))
            ಹಿಂತಿರುಗಿ false;
        ಆದರೆ (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            ಹಿಂತಿರುಗಿ false;
        }
        *val_rtrn = ~(uint32_t) v;
        ಹಿಂತಿರುಗಿ true;

    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE:
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT:
        ಎಡ = ಅಭಿವ್ಯಕ್ತಿ->unary.ಮಗು;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer_Lookup(ctx, ಎಡ, &v, pending, lookup, lookupPriv))
            ಹಿಂತಿರುಗಿ false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_ಗಮ್ಯ_operator_char(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಹಿಂತಿರುಗಿ false;

    ಪೂರ್ವನಿಯೋಜಿತ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 ಅಭಿವ್ಯಕ್ತಿ->common.type);
        ನಿಲ್ಲಿಸು;
    }

    ಹಿಂತಿರುಗಿ false;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                uint32_t *ಮುಖವಾಡ_rtrn, ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *ಮೌಲ್ಯಗಳು)
{
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ_Lookup(ctx, ಅಭಿವ್ಯಕ್ತಿ, ಮುಖವಾಡ_rtrn, ಖಾಲಿ, SimpleLookup, ಮೌಲ್ಯಗಳು);
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ,
                   ಎಣಿಕೆ mod_type mod_type, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
                   xkb_mod_ಮುಖವಾಡ_t *ಮುಖವಾಡ_rtrn)
{
    Lookup_Mod_ಮುಖವಾಡ_Priv priv = { .mods = mods, .mod_type = mod_type };
    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ_Lookup(ctx, ಅಭಿವ್ಯಕ್ತಿ, ಮುಖವಾಡ_rtrn, ಖಾಲಿ, Lookup_Mod_ಮುಖವಾಡ, &priv);
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *def,
               ಎಣಿಕೆ mod_type mod_type, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
               xkb_mod_ಸೂಚ್ಯಂಕ_t *ndx_rtrn)
{
    ಆದರೆ (def->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_ಗಮ್ಯ_string(def->common.type));
        ಹಿಂತಿರುಗಿ false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_ಸೂಚ್ಯಂಕ_t ndx = Xkb_Mod_Name_ಗಮ್ಯ_ಸೂಚ್ಯಂಕ(mods, name, mod_type);
    ಆದರೆ (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_ಪಠ್ಯ(ctx, name));
        ಹಿಂತಿರುಗಿ false;
    }

    *ndx_rtrn = ndx;
    ಹಿಂತಿರುಗಿ true;
}

bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು_ಮುಖವಾಡ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                     ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, xkb_layout_ಮುಖವಾಡ_t *ಗುಂಪು_rtrn,
                     bool *pending_rtrn)
{
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು pending_ಗುಂಪು_ಮುಖವಾಡ_ಹೆಸರುಗಳು[] = {
        { ಗುಂಪು_ಕೊನೆಯ_ಸೂಚ್ಯಂಕ_NAME, 0 },
        { ಖಾಲಿ, 0 }
    };

    ಸ್ಥಿರಾಂಕ ರಚನೆ named_integer_ನಮೂನೆ ಗುಂಪು_name_ನಮೂನೆ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .ಪೂರ್ವಪ್ರತ್ಯಯ = "Group",
        .ಪೂರ್ವಪ್ರತ್ಯಯ_ಉದ್ದಳತೆ = ಗಾತ್ರ("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_ಮುಖವಾಡ = true,
        .entries = keymap_info->lookup.ಗುಂಪು_ಮುಖವಾಡ_ಹೆಸರುಗಳು,
        .pending_entries = pending_ಗುಂಪು_ಮುಖವಾಡ_ಹೆಸರುಗಳು,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2
    };

    ಹಿಂತಿರುಗಿ ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ_Lookup(keymap_info->keymap.ctx, ಅಭಿವ್ಯಕ್ತಿ, ಗುಂಪು_rtrn,
                                 pending_rtrn, Named_Integer_ನಮೂನೆ_Lookup,
                                 &ಗುಂಪು_name_ನಮೂನೆ);
}
