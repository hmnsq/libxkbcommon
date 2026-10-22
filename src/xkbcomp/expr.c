/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ضمن "config.h"

#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcomp-priv.h"
#ضمن "expr.h"
#ضمن "keymap.h"
#ضمن "keysym.h"
#ضمن "messages-codes.h"
#ضمن "text.h"
#ضمن "xkbcomp/ast.h"
#ضمن "utils.h"
#ضمن "util-numbers.h"
#ضمن "util-checked-arithmetic.h"

تعريفنوع bool (*IdentLookupFunc)(بنية xkb_context *ctx, ثابت فراغ *priv,
                                xkb_atom_t میدان_2, uint32_t *val_rtrn,
                                bool *pending);

bool
اظہاریہ_Resolve_Lhs(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
               ثابت حرف **elem_rtrn, ثابت حرف **میدان_rtrn,
               اظہاریہ_Def **اشاریہ_rtrn)
{
    بدل (اظہاریہ->common.type) {
    حالة STMT_اظہاریہ_IDENT:
        *elem_rtrn = عدم;
        *میدان_rtrn = xkb_atom_متن(ctx, اظہاریہ->ident.ident);
        *اشاریہ_rtrn = عدم;
        أرجع (*میدان_rtrn != عدم);
    حالة STMT_اظہاریہ_میدان_REF:
        *elem_rtrn = xkb_atom_متن(ctx, اظہاریہ->میدان_ref.element);
        *میدان_rtrn = xkb_atom_متن(ctx, اظہاریہ->میدان_ref.میدان_2);
        *اشاریہ_rtrn = عدم;
        أرجع (*elem_rtrn != عدم && *میدان_rtrn != عدم);
    حالة STMT_اظہاریہ_ARRAY_REF:
        *elem_rtrn = xkb_atom_متن(ctx, اظہاریہ->array_ref.element);
        *میدان_rtrn = xkb_atom_متن(ctx, اظہاریہ->array_ref.میدان_2);
        *اشاریہ_rtrn = اظہاریہ->array_ref.اندراج;
        إذا (اظہاریہ->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == عدم)
            أرجع false;
        إذا (*میدان_rtrn == عدم)
            أرجع false;
        أرجع true;
    افتراضي:
        اكسر;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", اظہاریہ->common.type);
    أرجع false;
}

ساكن bool
SimpleLookup(بنية xkb_context *ctx, ثابت فراغ *priv, xkb_atom_t میدان_2,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    إذا (!priv || میدان_2 == XKB_ATOM_NONE)
        أرجع false;

    ثابت حرف *str = xkb_atom_متن(ctx, میدان_2);
    لكل (ثابت Lookup_اندراج *اندراج = priv; اندراج && اندراج->name; اندراج++) {
        إذا (istreq(str, اندراج->name)) {
            *val_rtrn = اندراج->قدر;
            أرجع true;
        }
    }

    أرجع false;
}

بنية named_integer_نمونہ {
    ثابت حرف *اگیتر;
    ناپ_t اگیتر_لمائی;
    uint32_t min;
    uint32_t max;
    ثابت Lookup_اندراج *entries;
    ثابت Lookup_اندراج *pending_entries;
    bool is_نقاب;
    تعداد xkb_سنیہا_کوڈ error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ساكن bool
Named_Integer_نمونہ_Lookup(بنية xkb_context *ctx, ثابت فراغ *priv,
                          xkb_atom_t میدان_2, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    إذا (!priv || میدان_2 == XKB_ATOM_NONE)
        أرجع false;

    ثابت حرف * ثابت str = xkb_atom_متن(ctx, میدان_2);
    ثابت بنية named_integer_نمونہ * ثابت نمونہ = priv;

    ثابت صحيح گنتی = (istrneq(str, نمونہ->اگیتر, نمونہ->اگیتر_لمائی))
        ? parse_dec_منزل_uint32_t(str + نمونہ->اگیتر_لمائی, SIZE_MAX, val_rtrn)
        : 0;

    إذا (گنتی > 0 && *(str + نمونہ->اگیتر_لمائی + گنتی) == '\0') {
        إذا (*val_rtrn < نمونہ->min || *val_rtrn > نمونہ->max) {
            log_err(ctx, نمونہ->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    نمونہ->اگیتر, *val_rtrn, نمونہ->min, نمونہ->max);
            أرجع false;
        }
        إذا (نمونہ->is_نقاب) {
            /* Compute mask (bit 0 = min index) */
            أكد(*val_rtrn - نمونہ->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - نمونہ->min);
        }
        أرجع true;
    } وإلا {
        إذا (نمونہ->entries &&
            SimpleLookup(ctx, نمونہ->entries, میدان_2, val_rtrn, عدم)) {
            أرجع true;
        }
        إذا (نمونہ->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, نمونہ->pending_entries, میدان_2, val_rtrn, عدم)) {
            *pending_rtrn = true;
            أرجع true;
        }
        أرجع false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
تعريفنوع بنية {
    ثابت بنية xkb_mod_سیٹ *mods;
    تعداد mod_type mod_type;
} Lookup_Mod_نقاب_Priv;

ساكن bool
Lookup_Mod_نقاب(بنية xkb_context *ctx, ثابت فراغ *priv, xkb_atom_t میدان_2,
              xkb_mod_نقاب_t *val_rtrn, bool *pending_rtrn)
{
    ثابت حرف *str = xkb_atom_متن(ctx, میدان_2);
    إذا (!str)
        أرجع false;

    إذا (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_نقاب_ALL;
        أرجع true;
    }

    إذا (istreq(str, "none")) {
        *val_rtrn = 0;
        أرجع true;
    }

    ثابت Lookup_Mod_نقاب_Priv *arg = priv;
    ثابت بنية xkb_mod_سیٹ *mods = arg->mods;
    ثابت تعداد mod_type mod_type = arg->mod_type;
    ثابت xkb_mod_اشاریہ_t ndx = Xkb_Mod_Name_منزل_اشاریہ(mods, میدان_2, mod_type);
    إذا (ndx == XKB_MOD_INVALID)
        أرجع false;

    *val_rtrn = (UINT32_C(1) << ndx);
    أرجع true;
}

bool
اظہاریہ_Resolve_Boolean(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   bool *سیٹ_rtrn)
{
    bool ok = false;
    ثابت حرف *ident;

    بدل (اظہاریہ->common.type) {
    حالة STMT_اظہاریہ_BOOLEAN_LITERAL:
        *سیٹ_rtrn = اظہاریہ->boolean.سیٹ;
        أرجع true;

    حالة STMT_اظہاریہ_STRING_LITERAL:
    حالة STMT_اظہاریہ_INTEGER_LITERAL:
    حالة STMT_اظہاریہ_FLOAT_LITERAL:
    حالة STMT_اظہاریہ_KEYNAME_LITERAL:
    حالة STMT_اظہاریہ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_منزل_string(اظہاریہ->common.type));
        أرجع false;

    حالة STMT_اظہاریہ_IDENT:
        ident = xkb_atom_متن(ctx, اظہاریہ->ident.ident);
        إذا (ident) {
            إذا (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *سیٹ_rtrn = true;
                أرجع true;
            }
            وإلا إذا (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *سیٹ_rtrn = false;
                أرجع true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        أرجع false;

    حالة STMT_اظہاریہ_میدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.element),
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.میدان_2));
        أرجع false;

    حالة STMT_اظہاریہ_INVERT:
    حالة STMT_اظہاریہ_NOT:
        ok = اظہاریہ_Resolve_Boolean(ctx, اظہاریہ->unary.بچہ, سیٹ_rtrn);
        إذا (ok)
            *سیٹ_rtrn = !*سیٹ_rtrn;
        أرجع ok;
    حالة STMT_اظہاریہ_جوڑو:
    حالة STMT_اظہاریہ_گھٹاؤ:
    حالة STMT_اظہاریہ_MULTIPLY:
    حالة STMT_اظہاریہ_DIVIDE:
    حالة STMT_اظہاریہ_سونپو:
    حالة STMT_اظہاریہ_NEGATE:
    حالة STMT_اظہاریہ_UNARY_PLUS:
    حالة STMT_اظہاریہ_خالی_تھاواں_LIST:
    حالة STMT_اظہاریہ_ACTION_DECL:
    حالة STMT_اظہاریہ_ACTION_LIST:
    حالة STMT_اظہاریہ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_منزل_string(اظہاریہ->common.type));
        اكسر;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 اظہاریہ->common.type);
        اكسر;
    }

    أرجع false;
}

ساكن bool
اظہاریہ_Resolve_Integer_Lookup(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ثابت فراغ *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    اظہاریہ_Def *بایاں, *دایاں;

    بدل (اظہاریہ->common.type) {
    حالة STMT_اظہاریہ_INTEGER_LITERAL:
        *val_rtrn = اظہاریہ->integer.ival;
        أرجع true;

    حالة STMT_اظہاریہ_STRING_LITERAL:
    حالة STMT_اظہاریہ_FLOAT_LITERAL:
    حالة STMT_اظہاریہ_BOOLEAN_LITERAL:
    حالة STMT_اظہاریہ_KEYNAME_LITERAL:
    حالة STMT_اظہاریہ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_منزل_string(اظہاریہ->common.type));
        أرجع false;

    حالة STMT_اظہاریہ_IDENT:
        إذا (lookup)
            ok = lookup(ctx, lookupPriv, اظہاریہ->ident.ident, &u, pending);

        إذا (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_متن(ctx, اظہاریہ->ident.ident));
        وإلا
            *val_rtrn = (int64_t) u;

        إذا (pending && *pending) {
            /* Postpone computation */
            أرجع false;
        }

        أرجع ok;

    حالة STMT_اظہاریہ_میدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.element),
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.میدان_2));
        أرجع false;

    حالة STMT_اظہاریہ_جوڑو:
    حالة STMT_اظہاریہ_گھٹاؤ:
    حالة STMT_اظہاریہ_MULTIPLY:
    حالة STMT_اظہاریہ_DIVIDE:
        بایاں = اظہاریہ->ثنائی.بایاں;
        دایاں = اظہاریہ->ثنائی.دایاں;
        إذا (!اظہاریہ_Resolve_Integer_Lookup(ctx, بایاں, &l, pending, lookup, lookupPriv) ||
            !اظہاریہ_Resolve_Integer_Lookup(ctx, دایاں, &r, pending, lookup, lookupPriv))
            أرجع false;

        بدل (اظہاریہ->common.type) {
        حالة STMT_اظہاریہ_جوڑو:
            إذا (ckd_جوڑو(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_حد_توں_ودھ,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_اظہاریہ_گھٹاؤ:
            إذا (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_حد_توں_ودھ,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_اظہاریہ_MULTIPLY:
            إذا (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_حد_توں_ودھ,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_اظہاریہ_DIVIDE:
            إذا (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                أرجع false;
            }
            *val_rtrn = l / r;
            اكسر;
        افتراضي:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_منزل_string(اظہاریہ->common.type));
            أرجع false;
        }

        أرجع true;

    حالة STMT_اظہاریہ_سونپو:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        اكسر;

    حالة STMT_اظہاریہ_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        أرجع false;

    حالة STMT_اظہاریہ_INVERT:
    حالة STMT_اظہاریہ_NEGATE:
        بایاں = اظہاریہ->unary.بچہ;
        إذا (!اظہاریہ_Resolve_Integer_Lookup(ctx, بایاں, &l, pending, lookup, lookupPriv))
            أرجع false;

        *val_rtrn = (اظہاریہ->common.type == STMT_اظہاریہ_NEGATE ? -l : ~l);
        أرجع true;

    حالة STMT_اظہاریہ_UNARY_PLUS:
        بایاں = اظہاریہ->unary.بچہ;
        أرجع اظہاریہ_Resolve_Integer_Lookup(ctx, بایاں, val_rtrn, pending, lookup,
                                        lookupPriv);

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 اظہاریہ->common.type);
        اكسر;
    }

    أرجع false;
}

bool
اظہاریہ_Resolve_Integer(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   int64_t *val_rtrn)
{
    أرجع اظہاریہ_Resolve_Integer_Lookup(ctx, اظہاریہ, val_rtrn, عدم, عدم, عدم);
}

تعداد xkb_parser_error
اظہاریہ_Resolve_گروہ(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت اظہاریہ_Def *اظہاریہ, bool absolute,
                 xkb_layout_اشاریہ_t *گروہ_rtrn, bool *pending)
{
    ساكن ثابت Lookup_اندراج pending_گروہ_اشاریہ_ناں[] = {
        { گروہ_آخری_اشاریہ_NAME, 0 },
        { عدم, 0 }
    };

    ثابت بنية named_integer_نمونہ گروہ_name_نمونہ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .اگیتر = "Group",
        .اگیتر_لمائی = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_نقاب = false,
        .entries = keymap_info->lookup.گروہ_اشاریہ_ناں,
        .pending_entries = pending_گروہ_اشاریہ_ناں,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
    };

    int64_t نتیجہ = 0;
    إذا (!اظہاریہ_Resolve_Integer_Lookup(keymap_info->keymap.ctx, اظہاریہ, &نتیجہ, pending,
                                  Named_Integer_نمونہ_Lookup, &گروہ_name_نمونہ))
        أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    إذا (نتیجہ < (int64_t)absolute || نتیجہ > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                نتیجہ, absolute, keymap_info->features.max_groups);
        أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *گروہ_rtrn = (xkb_layout_اشاریہ_t) نتیجہ;
    أرجع PARSER_SUCCESS;
}

bool
اظہاریہ_Resolve_Level(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                 xkb_level_اشاریہ_t *level_rtrn)
{
    ساكن ثابت بنية named_integer_نمونہ level_name_نمونہ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .اگیتر = "Level",
        .اگیتر_لمائی = حجم("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_نقاب = false,
        .entries = عدم,
        .error_id = XKB_ERROR_UNSUPPORTED_کھسکاؤ_LEVEL,
    };
    int64_t نتیجہ = 0;
    إذا (!اظہاریہ_Resolve_Integer_Lookup(ctx, اظہاریہ, &نتیجہ, عدم,
                                  Named_Integer_نمونہ_Lookup,
                                  &level_name_نمونہ))
        أرجع false;

    إذا (نتیجہ < 1 || نتیجہ > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_کھسکاؤ_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                نتیجہ, XKB_LEVEL_MAX_IMPL);
        أرجع false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_اشاریہ_t) (نتیجہ - 1);
    أرجع true;
}

bool
اظہاریہ_Resolve_Button(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ, int64_t *btn_rtrn)
{
    أرجع اظہاریہ_Resolve_Integer_Lookup(ctx, اظہاریہ, btn_rtrn, عدم, SimpleLookup,
                                    button_ناں);
}

bool
اظہاریہ_Resolve_String(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                  xkb_atom_t *val_rtrn)
{
    بدل (اظہاریہ->common.type) {
    حالة STMT_اظہاریہ_STRING_LITERAL:
        *val_rtrn = اظہاریہ->string.str;
        أرجع true;

    حالة STMT_اظہاریہ_INTEGER_LITERAL:
    حالة STMT_اظہاریہ_FLOAT_LITERAL:
    حالة STMT_اظہاریہ_BOOLEAN_LITERAL:
    حالة STMT_اظہاریہ_KEYNAME_LITERAL:
    حالة STMT_اظہاریہ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Found %s, expected a string\n",
                stmt_type_منزل_string(اظہاریہ->common.type));
        أرجع false;

    حالة STMT_اظہاریہ_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_متن(ctx, اظہاریہ->ident.ident));
        أرجع false;

    حالة STMT_اظہاریہ_میدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.element),
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.میدان_2));
        أرجع false;

    حالة STMT_اظہاریہ_جوڑو:
    حالة STMT_اظہاریہ_گھٹاؤ:
    حالة STMT_اظہاریہ_MULTIPLY:
    حالة STMT_اظہاریہ_DIVIDE:
    حالة STMT_اظہاریہ_سونپو:
    حالة STMT_اظہاریہ_NEGATE:
    حالة STMT_اظہاریہ_INVERT:
    حالة STMT_اظہاریہ_NOT:
    حالة STMT_اظہاریہ_UNARY_PLUS:
    حالة STMT_اظہاریہ_خالی_تھاواں_LIST:
    حالة STMT_اظہاریہ_ACTION_DECL:
    حالة STMT_اظہاریہ_ACTION_LIST:
    حالة STMT_اظہاریہ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_منزل_string(اظہاریہ->common.type));
        أرجع false;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 اظہاریہ->common.type);
        اكسر;
    }
    أرجع false;
}

bool
اظہاریہ_Resolve_Enum(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                uint32_t *val_rtrn, ثابت Lookup_اندراج *اقدار)
{
    إذا (اظہاریہ->common.type != STMT_اظہاریہ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_منزل_string(اظہاریہ->common.type));
        أرجع false;
    }

    إذا (!SimpleLookup(ctx, اقدار, اظہاریہ->ident.ident, val_rtrn, عدم)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_متن(ctx, اظہاریہ->ident.ident));
        طالما (اقدار && اقدار->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", اقدار->name);
            اقدار++;
        }
        أرجع false;
    }

    أرجع true;
}

ساكن bool
اظہاریہ_Resolve_نقاب_Lookup(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ثابت فراغ *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    اظہاریہ_Def *بایاں, *دایاں;
    ثابت حرف *bogus = عدم;

    بدل (اظہاریہ->common.type) {
    حالة STMT_اظہاریہ_INTEGER_LITERAL:
        إذا (اظہاریہ->integer.ival < 0 || اظہاریہ->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    اظہاریہ->integer.ival < 0 ? "-" : "",
                    imaxabs(اظہاریہ->integer.ival),
                    UINT32_MAX);
            أرجع false;
        }
        *val_rtrn = (uint32_t) اظہاریہ->integer.ival;
        أرجع true;

    حالة STMT_اظہاریہ_STRING_LITERAL:
    حالة STMT_اظہاریہ_FLOAT_LITERAL:
    حالة STMT_اظہاریہ_BOOLEAN_LITERAL:
    حالة STMT_اظہاریہ_KEYNAME_LITERAL:
    حالة STMT_اظہاریہ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_منزل_string(اظہاریہ->common.type));
        أرجع false;

    حالة STMT_اظہاریہ_IDENT:
        ok = lookup(ctx, lookupPriv, اظہاریہ->ident.ident, val_rtrn, pending);
        إذا (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_متن(ctx, اظہاریہ->ident.ident));
        إذا (pending && *pending) {
            /* Postpone computation */
            أرجع false;
        }
        أرجع ok;

    حالة STMT_اظہاریہ_میدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.element),
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.میدان_2));
        أرجع false;

    حالة STMT_اظہاریہ_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    حالة STMT_اظہاریہ_ACTION_DECL:
        إذا (bogus == عدم)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        أرجع false;

    حالة STMT_اظہاریہ_جوڑو:
    حالة STMT_اظہاریہ_گھٹاؤ:
    حالة STMT_اظہاریہ_MULTIPLY:
    حالة STMT_اظہاریہ_DIVIDE:
        بایاں = اظہاریہ->ثنائی.بایاں;
        دایاں = اظہاریہ->ثنائی.دایاں;
        إذا (!اظہاریہ_Resolve_نقاب_Lookup(ctx, بایاں, &l, pending, lookup, lookupPriv) ||
            !اظہاریہ_Resolve_نقاب_Lookup(ctx, دایاں, &r, pending, lookup, lookupPriv))
            أرجع false;

        بدل (اظہاریہ->common.type) {
        حالة STMT_اظہاریہ_جوڑو:
            *val_rtrn = l | r;
            اكسر;
        حالة STMT_اظہاریہ_گھٹاؤ:
            *val_rtrn = l & (~r);
            اكسر;
        حالة STMT_اظہاریہ_MULTIPLY:
        حالة STMT_اظہاریہ_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (اظہاریہ->common.type == STMT_اظہاریہ_DIVIDE ? "divide" : "multiply"));
            أرجع false;
        افتراضي:
            اكسر;
        }

        أرجع true;

    حالة STMT_اظہاریہ_سونپو:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        اكسر;

    حالة STMT_اظہاریہ_INVERT:
        بایاں = اظہاریہ->unary.بچہ;
        إذا (!اظہاریہ_Resolve_Integer_Lookup(ctx, بایاں, &v, pending, lookup, lookupPriv))
            أرجع false;
        إذا (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            أرجع false;
        }
        *val_rtrn = ~(uint32_t) v;
        أرجع true;

    حالة STMT_اظہاریہ_UNARY_PLUS:
    حالة STMT_اظہاریہ_NEGATE:
    حالة STMT_اظہاریہ_NOT:
        بایاں = اظہاریہ->unary.بچہ;
        إذا (!اظہاریہ_Resolve_Integer_Lookup(ctx, بایاں, &v, pending, lookup, lookupPriv))
            أرجع false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_منزل_operator_char(اظہاریہ->common.type));
        أرجع false;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 اظہاریہ->common.type);
        اكسر;
    }

    أرجع false;
}

bool
اظہاریہ_Resolve_نقاب(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                uint32_t *نقاب_rtrn, ثابت Lookup_اندراج *اقدار)
{
    أرجع اظہاریہ_Resolve_نقاب_Lookup(ctx, اظہاریہ, نقاب_rtrn, عدم, SimpleLookup, اقدار);
}

bool
اظہاریہ_Resolve_Mod_نقاب(بنية xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_سیٹ *mods,
                   xkb_mod_نقاب_t *نقاب_rtrn)
{
    Lookup_Mod_نقاب_Priv priv = { .mods = mods, .mod_type = mod_type };
    أرجع اظہاریہ_Resolve_نقاب_Lookup(ctx, اظہاریہ, نقاب_rtrn, عدم, Lookup_Mod_نقاب, &priv);
}

bool
اظہاریہ_Resolve_Mod(بنية xkb_context *ctx, ثابت اظہاریہ_Def *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_سیٹ *mods,
               xkb_mod_اشاریہ_t *ndx_rtrn)
{
    إذا (def->common.type != STMT_اظہاریہ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_منزل_string(def->common.type));
        أرجع false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_اشاریہ_t ndx = Xkb_Mod_Name_منزل_اشاریہ(mods, name, mod_type);
    إذا (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_متن(ctx, name));
        أرجع false;
    }

    *ndx_rtrn = ndx;
    أرجع true;
}

bool
اظہاریہ_Resolve_گروہ_نقاب(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت اظہاریہ_Def *اظہاریہ, xkb_layout_نقاب_t *گروہ_rtrn,
                     bool *pending_rtrn)
{
    ساكن ثابت Lookup_اندراج pending_گروہ_نقاب_ناں[] = {
        { گروہ_آخری_اشاریہ_NAME, 0 },
        { عدم, 0 }
    };

    ثابت بنية named_integer_نمونہ گروہ_name_نمونہ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .اگیتر = "Group",
        .اگیتر_لمائی = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_نقاب = true,
        .entries = keymap_info->lookup.گروہ_نقاب_ناں,
        .pending_entries = pending_گروہ_نقاب_ناں,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2
    };

    أرجع اظہاریہ_Resolve_نقاب_Lookup(keymap_info->keymap.ctx, اظہاریہ, گروہ_rtrn,
                                 pending_rtrn, Named_Integer_نمونہ_Lookup,
                                 &گروہ_name_نمونہ);
}
