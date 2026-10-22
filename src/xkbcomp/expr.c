/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "expr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keysym.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-numbers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-checked-arithmetic.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ bool (*IdentLookupFunc)(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਖਾਲੀ *priv,
                                xkb_atom_t ਖੇਤਰ_2, uint32_t *val_rtrn,
                                bool *pending);

bool
ਵਿਅੰਜਕ_Resolve_Lhs(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
               ਅਚਲ ਅੱਖਰ **elem_rtrn, ਅਚਲ ਅੱਖਰ **ਖੇਤਰ_rtrn,
               ਵਿਅੰਜਕ_Def **ਸੂਚਕਾਂਕ_rtrn)
{
    ਚੋਣ (ਵਿਅੰਜਕ->common.type) {
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_IDENT:
        *elem_rtrn = ਨਿਰਾ;
        *ਖੇਤਰ_rtrn = xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ident.ident);
        *ਸੂਚਕਾਂਕ_rtrn = ਨਿਰਾ;
        ਵਾਪਸ (*ਖੇਤਰ_rtrn != ਨਿਰਾ);
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਖੇਤਰ_REF:
        *elem_rtrn = xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ਖੇਤਰ_ref.element);
        *ਖੇਤਰ_rtrn = xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ਖੇਤਰ_ref.ਖੇਤਰ_2);
        *ਸੂਚਕਾਂਕ_rtrn = ਨਿਰਾ;
        ਵਾਪਸ (*elem_rtrn != ਨਿਰਾ && *ਖੇਤਰ_rtrn != ਨਿਰਾ);
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ARRAY_REF:
        *elem_rtrn = xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->array_ref.element);
        *ਖੇਤਰ_rtrn = xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->array_ref.ਖੇਤਰ_2);
        *ਸੂਚਕਾਂਕ_rtrn = ਵਿਅੰਜਕ->array_ref.ਇੰਦਰਾਜ;
        ਜੇ (ਵਿਅੰਜਕ->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == ਨਿਰਾ)
            ਵਾਪਸ false;
        ਜੇ (*ਖੇਤਰ_rtrn == ਨਿਰਾ)
            ਵਾਪਸ false;
        ਵਾਪਸ true;
    ਮੂਲ:
        ਰੋਕੋ;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", ਵਿਅੰਜਕ->common.type);
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
SimpleLookup(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਖਾਲੀ *priv, xkb_atom_t ਖੇਤਰ_2,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    ਜੇ (!priv || ਖੇਤਰ_2 == XKB_ATOM_NONE)
        ਵਾਪਸ false;

    ਅਚਲ ਅੱਖਰ *str = xkb_atom_ਪਾਠ(ctx, ਖੇਤਰ_2);
    ਲਈ (ਅਚਲ Lookup_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = priv; ਇੰਦਰਾਜ && ਇੰਦਰਾਜ->name; ਇੰਦਰਾਜ++) {
        ਜੇ (istreq(str, ਇੰਦਰਾਜ->name)) {
            *val_rtrn = ਇੰਦਰਾਜ->ਮੁੱਲ;
            ਵਾਪਸ true;
        }
    }

    ਵਾਪਸ false;
}

ਬਣਤਰ named_integer_ਨਮੂਨਾ {
    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
    ਮਾਪ_t ਅਗੇਤਰ_ਲੰਮਾਈ;
    uint32_t min;
    uint32_t max;
    ਅਚਲ Lookup_ਇੰਦਰਾਜ *entries;
    ਅਚਲ Lookup_ਇੰਦਰਾਜ *pending_entries;
    bool is_ਨਕਾਬ;
    ਗਿਣਤੀ xkb_ਸੁਨੇਹਾ_ਕੋਡ error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ਸਥਿਰ bool
Named_Integer_ਨਮੂਨਾ_Lookup(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਖਾਲੀ *priv,
                          xkb_atom_t ਖੇਤਰ_2, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    ਜੇ (!priv || ਖੇਤਰ_2 == XKB_ATOM_NONE)
        ਵਾਪਸ false;

    ਅਚਲ ਅੱਖਰ * ਅਚਲ str = xkb_atom_ਪਾਠ(ctx, ਖੇਤਰ_2);
    ਅਚਲ ਬਣਤਰ named_integer_ਨਮੂਨਾ * ਅਚਲ ਨਮੂਨਾ = priv;

    ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 = (istrneq(str, ਨਮੂਨਾ->ਅਗੇਤਰ, ਨਮੂਨਾ->ਅਗੇਤਰ_ਲੰਮਾਈ))
        ? parse_dec_ਮੰਜ਼ਿਲ_uint32_t(str + ਨਮੂਨਾ->ਅਗੇਤਰ_ਲੰਮਾਈ, SIZE_MAX, val_rtrn)
        : 0;

    ਜੇ (ਗਿਣਤੀ_2 > 0 && *(str + ਨਮੂਨਾ->ਅਗੇਤਰ_ਲੰਮਾਈ + ਗਿਣਤੀ_2) == '\0') {
        ਜੇ (*val_rtrn < ਨਮੂਨਾ->min || *val_rtrn > ਨਮੂਨਾ->max) {
            log_err(ctx, ਨਮੂਨਾ->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    ਨਮੂਨਾ->ਅਗੇਤਰ, *val_rtrn, ਨਮੂਨਾ->min, ਨਮੂਨਾ->max);
            ਵਾਪਸ false;
        }
        ਜੇ (ਨਮੂਨਾ->is_ਨਕਾਬ) {
            /* Compute mask (bit 0 = min index) */
            ਪ੍ਰਤਿਜ੍ਞਾ(*val_rtrn - ਨਮੂਨਾ->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - ਨਮੂਨਾ->min);
        }
        ਵਾਪਸ true;
    } ਨਹੀਂਤਾਂ {
        ਜੇ (ਨਮੂਨਾ->entries &&
            SimpleLookup(ctx, ਨਮੂਨਾ->entries, ਖੇਤਰ_2, val_rtrn, ਨਿਰਾ)) {
            ਵਾਪਸ true;
        }
        ਜੇ (ਨਮੂਨਾ->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, ਨਮੂਨਾ->pending_entries, ਖੇਤਰ_2, val_rtrn, ਨਿਰਾ)) {
            *pending_rtrn = true;
            ਵਾਪਸ true;
        }
        ਵਾਪਸ false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods;
    ਗਿਣਤੀ mod_type mod_type;
} Lookup_Mod_ਨਕਾਬ_Priv;

ਸਥਿਰ bool
Lookup_Mod_ਨਕਾਬ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਖਾਲੀ *priv, xkb_atom_t ਖੇਤਰ_2,
              xkb_mod_ਨਕਾਬ_t *val_rtrn, bool *pending_rtrn)
{
    ਅਚਲ ਅੱਖਰ *str = xkb_atom_ਪਾਠ(ctx, ਖੇਤਰ_2);
    ਜੇ (!str)
        ਵਾਪਸ false;

    ਜੇ (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_ਨਕਾਬ_ALL;
        ਵਾਪਸ true;
    }

    ਜੇ (istreq(str, "none")) {
        *val_rtrn = 0;
        ਵਾਪਸ true;
    }

    ਅਚਲ Lookup_Mod_ਨਕਾਬ_Priv *arg = priv;
    ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods = arg->mods;
    ਅਚਲ ਗਿਣਤੀ mod_type mod_type = arg->mod_type;
    ਅਚਲ xkb_mod_ਸੂਚਕਾਂਕ_t ndx = Xkb_Mod_Name_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(mods, ਖੇਤਰ_2, mod_type);
    ਜੇ (ndx == XKB_MOD_INVALID)
        ਵਾਪਸ false;

    *val_rtrn = (UINT32_C(1) << ndx);
    ਵਾਪਸ true;
}

bool
ਵਿਅੰਜਕ_Resolve_Boolean(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                   bool *ਸੈੱਟ_rtrn)
{
    bool ok = false;
    ਅਚਲ ਅੱਖਰ *ident;

    ਚੋਣ (ਵਿਅੰਜਕ->common.type) {
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_BOOLEAN_LITERAL:
        *ਸੈੱਟ_rtrn = ਵਿਅੰਜਕ->boolean.ਸੈੱਟ;
        ਵਾਪਸ true;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_STRING_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_INTEGER_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_FLOAT_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(ਵਿਅੰਜਕ->common.type));
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_IDENT:
        ident = xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ident.ident);
        ਜੇ (ident) {
            ਜੇ (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *ਸੈੱਟ_rtrn = true;
                ਵਾਪਸ true;
            }
            ਨਹੀਂਤਾਂ ਜੇ (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *ਸੈੱਟ_rtrn = false;
                ਵਾਪਸ true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਖੇਤਰ_REF:
        log_err(ctx, XKB_ERROR_INVALID_ਵਿਅੰਜਕ_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ਖੇਤਰ_ref.element),
                xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ਖੇਤਰ_ref.ਖੇਤਰ_2));
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_INVERT:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NOT:
        ok = ਵਿਅੰਜਕ_Resolve_Boolean(ctx, ਵਿਅੰਜਕ->unary.ਬੱਚਾ, ਸੈੱਟ_rtrn);
        ਜੇ (ok)
            *ਸੈੱਟ_rtrn = !*ਸੈੱਟ_rtrn;
        ਵਾਪਸ ok;
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਜੋੜੋ:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਘਟਾਓ:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_MULTIPLY:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_DIVIDE:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਸੌਂਪੋ:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NEGATE:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_UNARY_PLUS:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_LIST:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ACTION_DECL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ACTION_LIST:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(ਵਿਅੰਜਕ->common.type));
        ਰੋਕੋ;

    ਮੂਲ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 ਵਿਅੰਜਕ->common.type);
        ਰੋਕੋ;
    }

    ਵਾਪਸ false;
}

ਸਥਿਰ bool
ਵਿਅੰਜਕ_Resolve_Integer_Lookup(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ਅਚਲ ਖਾਲੀ *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    ਵਿਅੰਜਕ_Def *ਖੱਬਾ, *ਸੱਜਾ;

    ਚੋਣ (ਵਿਅੰਜਕ->common.type) {
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_INTEGER_LITERAL:
        *val_rtrn = ਵਿਅੰਜਕ->integer.ival;
        ਵਾਪਸ true;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_STRING_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_FLOAT_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_BOOLEAN_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(ਵਿਅੰਜਕ->common.type));
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_IDENT:
        ਜੇ (lookup)
            ok = lookup(ctx, lookupPriv, ਵਿਅੰਜਕ->ident.ident, &u, pending);

        ਜੇ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ident.ident));
        ਨਹੀਂਤਾਂ
            *val_rtrn = (int64_t) u;

        ਜੇ (pending && *pending) {
            /* Postpone computation */
            ਵਾਪਸ false;
        }

        ਵਾਪਸ ok;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਖੇਤਰ_REF:
        log_err(ctx, XKB_ERROR_INVALID_ਵਿਅੰਜਕ_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ਖੇਤਰ_ref.element),
                xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ਖੇਤਰ_ref.ਖੇਤਰ_2));
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਜੋੜੋ:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਘਟਾਓ:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_MULTIPLY:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_DIVIDE:
        ਖੱਬਾ = ਵਿਅੰਜਕ->ਦੁਆਧਾਰੀ.ਖੱਬਾ;
        ਸੱਜਾ = ਵਿਅੰਜਕ->ਦੁਆਧਾਰੀ.ਸੱਜਾ;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer_Lookup(ctx, ਖੱਬਾ, &l, pending, lookup, lookupPriv) ||
            !ਵਿਅੰਜਕ_Resolve_Integer_Lookup(ctx, ਸੱਜਾ, &r, pending, lookup, lookupPriv))
            ਵਾਪਸ false;

        ਚੋਣ (ਵਿਅੰਜਕ->common.type) {
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਜੋੜੋ:
            ਜੇ (ckd_ਜੋੜੋ(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_ਉਛਾਲ,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ਵਾਪਸ false;
            }
            ਰੋਕੋ;
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਘਟਾਓ:
            ਜੇ (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_ਉਛਾਲ,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ਵਾਪਸ false;
            }
            ਰੋਕੋ;
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_MULTIPLY:
            ਜੇ (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_ਉਛਾਲ,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ਵਾਪਸ false;
            }
            ਰੋਕੋ;
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_DIVIDE:
            ਜੇ (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                ਵਾਪਸ false;
            }
            *val_rtrn = l / r;
            ਰੋਕੋ;
        ਮੂਲ:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_ਮੰਜ਼ਿਲ_string(ਵਿਅੰਜਕ->common.type));
            ਵਾਪਸ false;
        }

        ਵਾਪਸ true;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਸੌਂਪੋ:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ਰੋਕੋ;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_INVERT:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NEGATE:
        ਖੱਬਾ = ਵਿਅੰਜਕ->unary.ਬੱਚਾ;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer_Lookup(ctx, ਖੱਬਾ, &l, pending, lookup, lookupPriv))
            ਵਾਪਸ false;

        *val_rtrn = (ਵਿਅੰਜਕ->common.type == STMT_ਵਿਅੰਜਕ_NEGATE ? -l : ~l);
        ਵਾਪਸ true;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_UNARY_PLUS:
        ਖੱਬਾ = ਵਿਅੰਜਕ->unary.ਬੱਚਾ;
        ਵਾਪਸ ਵਿਅੰਜਕ_Resolve_Integer_Lookup(ctx, ਖੱਬਾ, val_rtrn, pending, lookup,
                                        lookupPriv);

    ਮੂਲ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 ਵਿਅੰਜਕ->common.type);
        ਰੋਕੋ;
    }

    ਵਾਪਸ false;
}

bool
ਵਿਅੰਜਕ_Resolve_Integer(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                   int64_t *val_rtrn)
{
    ਵਾਪਸ ਵਿਅੰਜਕ_Resolve_Integer_Lookup(ctx, ਵਿਅੰਜਕ, val_rtrn, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ);
}

ਗਿਣਤੀ xkb_parser_error
ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                 ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ, bool absolute,
                 xkb_layout_ਸੂਚਕਾਂਕ_t *ਸਮੂਹ_rtrn, bool *pending)
{
    ਸਥਿਰ ਅਚਲ Lookup_ਇੰਦਰਾਜ pending_ਸਮੂਹ_ਸੂਚਕਾਂਕ_ਨਾਂ[] = {
        { ਸਮੂਹ_ਆਖਰੀ_ਸੂਚਕਾਂਕ_NAME, 0 },
        { ਨਿਰਾ, 0 }
    };

    ਅਚਲ ਬਣਤਰ named_integer_ਨਮੂਨਾ ਸਮੂਹ_name_ਨਮੂਨਾ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .ਅਗੇਤਰ = "Group",
        .ਅਗੇਤਰ_ਲੰਮਾਈ = ਆਕਾਰ("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_ਨਕਾਬ = false,
        .entries = keymap_info->lookup.ਸਮੂਹ_ਸੂਚਕਾਂਕ_ਨਾਂ,
        .pending_entries = pending_ਸਮੂਹ_ਸੂਚਕਾਂਕ_ਨਾਂ,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
    };

    int64_t ਨਤੀਜਾ = 0;
    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer_Lookup(keymap_info->keymap.ctx, ਵਿਅੰਜਕ, &ਨਤੀਜਾ, pending,
                                  Named_Integer_ਨਮੂਨਾ_Lookup, &ਸਮੂਹ_name_ਨਮੂਨਾ))
        ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    ਜੇ (ਨਤੀਜਾ < (int64_t)absolute || ਨਤੀਜਾ > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                ਨਤੀਜਾ, absolute, keymap_info->features.max_groups);
        ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *ਸਮੂਹ_rtrn = (xkb_layout_ਸੂਚਕਾਂਕ_t) ਨਤੀਜਾ;
    ਵਾਪਸ PARSER_SUCCESS;
}

bool
ਵਿਅੰਜਕ_Resolve_Level(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                 xkb_level_ਸੂਚਕਾਂਕ_t *level_rtrn)
{
    ਸਥਿਰ ਅਚਲ ਬਣਤਰ named_integer_ਨਮੂਨਾ level_name_ਨਮੂਨਾ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .ਅਗੇਤਰ = "Level",
        .ਅਗੇਤਰ_ਲੰਮਾਈ = ਆਕਾਰ("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_ਨਕਾਬ = false,
        .entries = ਨਿਰਾ,
        .error_id = XKB_ERROR_UNSUPPORTED_ਖਿਸਕਾਓ_LEVEL,
    };
    int64_t ਨਤੀਜਾ = 0;
    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer_Lookup(ctx, ਵਿਅੰਜਕ, &ਨਤੀਜਾ, ਨਿਰਾ,
                                  Named_Integer_ਨਮੂਨਾ_Lookup,
                                  &level_name_ਨਮੂਨਾ))
        ਵਾਪਸ false;

    ਜੇ (ਨਤੀਜਾ < 1 || ਨਤੀਜਾ > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_ਖਿਸਕਾਓ_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                ਨਤੀਜਾ, XKB_LEVEL_MAX_IMPL);
        ਵਾਪਸ false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_ਸੂਚਕਾਂਕ_t) (ਨਤੀਜਾ - 1);
    ਵਾਪਸ true;
}

bool
ਵਿਅੰਜਕ_Resolve_Button(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ, int64_t *btn_rtrn)
{
    ਵਾਪਸ ਵਿਅੰਜਕ_Resolve_Integer_Lookup(ctx, ਵਿਅੰਜਕ, btn_rtrn, ਨਿਰਾ, SimpleLookup,
                                    button_ਨਾਂ);
}

bool
ਵਿਅੰਜਕ_Resolve_String(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                  xkb_atom_t *val_rtrn)
{
    ਚੋਣ (ਵਿਅੰਜਕ->common.type) {
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_STRING_LITERAL:
        *val_rtrn = ਵਿਅੰਜਕ->string.str;
        ਵਾਪਸ true;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_INTEGER_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_FLOAT_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_BOOLEAN_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Found %s, expected a string\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(ਵਿਅੰਜਕ->common.type));
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ident.ident));
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਖੇਤਰ_REF:
        log_err(ctx, XKB_ERROR_INVALID_ਵਿਅੰਜਕ_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ਖੇਤਰ_ref.element),
                xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ਖੇਤਰ_ref.ਖੇਤਰ_2));
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਜੋੜੋ:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਘਟਾਓ:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_MULTIPLY:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_DIVIDE:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਸੌਂਪੋ:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NEGATE:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_INVERT:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NOT:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_UNARY_PLUS:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_LIST:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ACTION_DECL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ACTION_LIST:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(ਵਿਅੰਜਕ->common.type));
        ਵਾਪਸ false;

    ਮੂਲ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 ਵਿਅੰਜਕ->common.type);
        ਰੋਕੋ;
    }
    ਵਾਪਸ false;
}

bool
ਵਿਅੰਜਕ_Resolve_Enum(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                uint32_t *val_rtrn, ਅਚਲ Lookup_ਇੰਦਰਾਜ *ਮੁੱਲਾਂ)
{
    ਜੇ (ਵਿਅੰਜਕ->common.type != STMT_ਵਿਅੰਜਕ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(ਵਿਅੰਜਕ->common.type));
        ਵਾਪਸ false;
    }

    ਜੇ (!SimpleLookup(ctx, ਮੁੱਲਾਂ, ਵਿਅੰਜਕ->ident.ident, val_rtrn, ਨਿਰਾ)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ident.ident));
        ਜਦੋਂ (ਮੁੱਲਾਂ && ਮੁੱਲਾਂ->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", ਮੁੱਲਾਂ->name);
            ਮੁੱਲਾਂ++;
        }
        ਵਾਪਸ false;
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ_Lookup(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ਅਚਲ ਖਾਲੀ *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    ਵਿਅੰਜਕ_Def *ਖੱਬਾ, *ਸੱਜਾ;
    ਅਚਲ ਅੱਖਰ *bogus = ਨਿਰਾ;

    ਚੋਣ (ਵਿਅੰਜਕ->common.type) {
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_INTEGER_LITERAL:
        ਜੇ (ਵਿਅੰਜਕ->integer.ival < 0 || ਵਿਅੰਜਕ->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    ਵਿਅੰਜਕ->integer.ival < 0 ? "-" : "",
                    imaxabs(ਵਿਅੰਜਕ->integer.ival),
                    UINT32_MAX);
            ਵਾਪਸ false;
        }
        *val_rtrn = (uint32_t) ਵਿਅੰਜਕ->integer.ival;
        ਵਾਪਸ true;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_STRING_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_FLOAT_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_BOOLEAN_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(ਵਿਅੰਜਕ->common.type));
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_IDENT:
        ok = lookup(ctx, lookupPriv, ਵਿਅੰਜਕ->ident.ident, val_rtrn, pending);
        ਜੇ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ident.ident));
        ਜੇ (pending && *pending) {
            /* Postpone computation */
            ਵਾਪਸ false;
        }
        ਵਾਪਸ ok;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਖੇਤਰ_REF:
        log_err(ctx, XKB_ERROR_INVALID_ਵਿਅੰਜਕ_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ਖੇਤਰ_ref.element),
                xkb_atom_ਪਾਠ(ctx, ਵਿਅੰਜਕ->ਖੇਤਰ_ref.ਖੇਤਰ_2));
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ACTION_DECL:
        ਜੇ (bogus == ਨਿਰਾ)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        ਵਾਪਸ false;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਜੋੜੋ:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਘਟਾਓ:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_MULTIPLY:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_DIVIDE:
        ਖੱਬਾ = ਵਿਅੰਜਕ->ਦੁਆਧਾਰੀ.ਖੱਬਾ;
        ਸੱਜਾ = ਵਿਅੰਜਕ->ਦੁਆਧਾਰੀ.ਸੱਜਾ;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ_Lookup(ctx, ਖੱਬਾ, &l, pending, lookup, lookupPriv) ||
            !ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ_Lookup(ctx, ਸੱਜਾ, &r, pending, lookup, lookupPriv))
            ਵਾਪਸ false;

        ਚੋਣ (ਵਿਅੰਜਕ->common.type) {
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਜੋੜੋ:
            *val_rtrn = l | r;
            ਰੋਕੋ;
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਘਟਾਓ:
            *val_rtrn = l & (~r);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_MULTIPLY:
        ਹਾਲਤ STMT_ਵਿਅੰਜਕ_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (ਵਿਅੰਜਕ->common.type == STMT_ਵਿਅੰਜਕ_DIVIDE ? "divide" : "multiply"));
            ਵਾਪਸ false;
        ਮੂਲ:
            ਰੋਕੋ;
        }

        ਵਾਪਸ true;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_ਸੌਂਪੋ:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ਰੋਕੋ;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_INVERT:
        ਖੱਬਾ = ਵਿਅੰਜਕ->unary.ਬੱਚਾ;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer_Lookup(ctx, ਖੱਬਾ, &v, pending, lookup, lookupPriv))
            ਵਾਪਸ false;
        ਜੇ (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            ਵਾਪਸ false;
        }
        *val_rtrn = ~(uint32_t) v;
        ਵਾਪਸ true;

    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_UNARY_PLUS:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NEGATE:
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_NOT:
        ਖੱਬਾ = ਵਿਅੰਜਕ->unary.ਬੱਚਾ;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer_Lookup(ctx, ਖੱਬਾ, &v, pending, lookup, lookupPriv))
            ਵਾਪਸ false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_ਮੰਜ਼ਿਲ_operator_char(ਵਿਅੰਜਕ->common.type));
        ਵਾਪਸ false;

    ਮੂਲ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 ਵਿਅੰਜਕ->common.type);
        ਰੋਕੋ;
    }

    ਵਾਪਸ false;
}

bool
ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                uint32_t *ਨਕਾਬ_rtrn, ਅਚਲ Lookup_ਇੰਦਰਾਜ *ਮੁੱਲਾਂ)
{
    ਵਾਪਸ ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ_Lookup(ctx, ਵਿਅੰਜਕ, ਨਕਾਬ_rtrn, ਨਿਰਾ, SimpleLookup, ਮੁੱਲਾਂ);
}

bool
ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ,
                   ਗਿਣਤੀ mod_type mod_type, ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                   xkb_mod_ਨਕਾਬ_t *ਨਕਾਬ_rtrn)
{
    Lookup_Mod_ਨਕਾਬ_Priv priv = { .mods = mods, .mod_type = mod_type };
    ਵਾਪਸ ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ_Lookup(ctx, ਵਿਅੰਜਕ, ਨਕਾਬ_rtrn, ਨਿਰਾ, Lookup_Mod_ਨਕਾਬ, &priv);
}

bool
ਵਿਅੰਜਕ_Resolve_Mod(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਵਿਅੰਜਕ_Def *def,
               ਗਿਣਤੀ mod_type mod_type, ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
               xkb_mod_ਸੂਚਕਾਂਕ_t *ndx_rtrn)
{
    ਜੇ (def->common.type != STMT_ਵਿਅੰਜਕ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(def->common.type));
        ਵਾਪਸ false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_ਸੂਚਕਾਂਕ_t ndx = Xkb_Mod_Name_ਮੰਜ਼ਿਲ_ਸੂਚਕਾਂਕ(mods, name, mod_type);
    ਜੇ (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_ਪਾਠ(ctx, name));
        ਵਾਪਸ false;
    }

    *ndx_rtrn = ndx;
    ਵਾਪਸ true;
}

bool
ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ_ਨਕਾਬ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                     ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ, xkb_layout_ਨਕਾਬ_t *ਸਮੂਹ_rtrn,
                     bool *pending_rtrn)
{
    ਸਥਿਰ ਅਚਲ Lookup_ਇੰਦਰਾਜ pending_ਸਮੂਹ_ਨਕਾਬ_ਨਾਂ[] = {
        { ਸਮੂਹ_ਆਖਰੀ_ਸੂਚਕਾਂਕ_NAME, 0 },
        { ਨਿਰਾ, 0 }
    };

    ਅਚਲ ਬਣਤਰ named_integer_ਨਮੂਨਾ ਸਮੂਹ_name_ਨਮੂਨਾ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .ਅਗੇਤਰ = "Group",
        .ਅਗੇਤਰ_ਲੰਮਾਈ = ਆਕਾਰ("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_ਨਕਾਬ = true,
        .entries = keymap_info->lookup.ਸਮੂਹ_ਨਕਾਬ_ਨਾਂ,
        .pending_entries = pending_ਸਮੂਹ_ਨਕਾਬ_ਨਾਂ,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2
    };

    ਵਾਪਸ ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ_Lookup(keymap_info->keymap.ctx, ਵਿਅੰਜਕ, ਸਮੂਹ_rtrn,
                                 pending_rtrn, Named_Integer_ਨਮੂਨਾ_Lookup,
                                 &ਸਮੂਹ_name_ਨਮੂਨਾ);
}
