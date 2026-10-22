/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#شامل "config.h"

#شامل <stdint.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcomp-priv.h"
#شامل "expr.h"
#شامل "keymap.h"
#شامل "keysym.h"
#شامل "messages-codes.h"
#شامل "text.h"
#شامل "xkbcomp/ast.h"
#شامل "utils.h"
#شامل "util-numbers.h"
#شامل "util-checked-arithmetic.h"

قسمتعریف bool (*IdentLookupFunc)(ساخت xkb_context *ctx, ثابت خالی *priv,
                                xkb_atom_t میدان_2, uint32_t *val_rtrn,
                                bool *pending);

bool
اظہاریہ_Resolve_Lhs(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
               ثابت حرف **elem_rtrn, ثابت حرف **میدان_rtrn,
               اظہاریہ_Def **اشاریہ_rtrn)
{
    انتخاب (اظہاریہ->common.type) {
    حالت STMT_اظہاریہ_IDENT:
        *elem_rtrn = تہی;
        *میدان_rtrn = xkb_atom_متن(ctx, اظہاریہ->ident.ident);
        *اشاریہ_rtrn = تہی;
        واپس (*میدان_rtrn != تہی);
    حالت STMT_اظہاریہ_میدان_REF:
        *elem_rtrn = xkb_atom_متن(ctx, اظہاریہ->میدان_ref.element);
        *میدان_rtrn = xkb_atom_متن(ctx, اظہاریہ->میدان_ref.میدان_2);
        *اشاریہ_rtrn = تہی;
        واپس (*elem_rtrn != تہی && *میدان_rtrn != تہی);
    حالت STMT_اظہاریہ_ARRAY_REF:
        *elem_rtrn = xkb_atom_متن(ctx, اظہاریہ->array_ref.element);
        *میدان_rtrn = xkb_atom_متن(ctx, اظہاریہ->array_ref.میدان_2);
        *اشاریہ_rtrn = اظہاریہ->array_ref.اندراج_2;
        اگر (اظہاریہ->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == تہی)
            واپس false;
        اگر (*میدان_rtrn == تہی)
            واپس false;
        واپس true;
    طےشدہ:
        توڑو;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", اظہاریہ->common.type);
    واپس false;
}

ساکن bool
SimpleLookup(ساخت xkb_context *ctx, ثابت خالی *priv, xkb_atom_t میدان_2,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    اگر (!priv || میدان_2 == XKB_ATOM_NONE)
        واپس false;

    ثابت حرف *str = xkb_atom_متن(ctx, میدان_2);
    برائے (ثابت Lookup_اندراج *اندراج_2 = priv; اندراج_2 && اندراج_2->name; اندراج_2++) {
        اگر (istreq(str, اندراج_2->name)) {
            *val_rtrn = اندراج_2->قدر;
            واپس true;
        }
    }

    واپس false;
}

ساخت named_integer_نمونہ {
    ثابت حرف *سابقہ;
    سائز_t سابقہ_لمبائی;
    uint32_t min;
    uint32_t max;
    ثابت Lookup_اندراج *entries;
    ثابت Lookup_اندراج *pending_entries;
    bool is_نقاب;
    شمار xkb_پیغام_رمز error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ساکن bool
Named_Integer_نمونہ_Lookup(ساخت xkb_context *ctx, ثابت خالی *priv,
                          xkb_atom_t میدان_2, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    اگر (!priv || میدان_2 == XKB_ATOM_NONE)
        واپس false;

    ثابت حرف * ثابت str = xkb_atom_متن(ctx, میدان_2);
    ثابت ساخت named_integer_نمونہ * ثابت نمونہ = priv;

    ثابت صحیح گنتی = (istrneq(str, نمونہ->سابقہ, نمونہ->سابقہ_لمبائی))
        ? parse_dec_مقصد_uint32_t(str + نمونہ->سابقہ_لمبائی, SIZE_MAX, val_rtrn)
        : 0;

    اگر (گنتی > 0 && *(str + نمونہ->سابقہ_لمبائی + گنتی) == '\0') {
        اگر (*val_rtrn < نمونہ->min || *val_rtrn > نمونہ->max) {
            log_err(ctx, نمونہ->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    نمونہ->سابقہ, *val_rtrn, نمونہ->min, نمونہ->max);
            واپس false;
        }
        اگر (نمونہ->is_نقاب) {
            /* Compute mask (bit 0 = min index) */
            أكد(*val_rtrn - نمونہ->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - نمونہ->min);
        }
        واپس true;
    } ورنہ {
        اگر (نمونہ->entries &&
            SimpleLookup(ctx, نمونہ->entries, میدان_2, val_rtrn, تہی)) {
            واپس true;
        }
        اگر (نمونہ->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, نمونہ->pending_entries, میدان_2, val_rtrn, تہی)) {
            *pending_rtrn = true;
            واپس true;
        }
        واپس false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
قسمتعریف ساخت {
    ثابت ساخت xkb_mod_سیٹ *mods;
    شمار mod_type mod_type;
} Lookup_Mod_نقاب_Priv;

ساکن bool
Lookup_Mod_نقاب(ساخت xkb_context *ctx, ثابت خالی *priv, xkb_atom_t میدان_2,
              xkb_mod_نقاب_t *val_rtrn, bool *pending_rtrn)
{
    ثابت حرف *str = xkb_atom_متن(ctx, میدان_2);
    اگر (!str)
        واپس false;

    اگر (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_نقاب_ALL;
        واپس true;
    }

    اگر (istreq(str, "none")) {
        *val_rtrn = 0;
        واپس true;
    }

    ثابت Lookup_Mod_نقاب_Priv *arg = priv;
    ثابت ساخت xkb_mod_سیٹ *mods = arg->mods;
    ثابت شمار mod_type mod_type = arg->mod_type;
    ثابت xkb_mod_اشاریہ_t ndx = Xkb_Mod_Name_مقصد_اشاریہ(mods, میدان_2, mod_type);
    اگر (ndx == XKB_MOD_INVALID)
        واپس false;

    *val_rtrn = (UINT32_C(1) << ndx);
    واپس true;
}

bool
اظہاریہ_Resolve_Boolean(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   bool *سیٹ_rtrn)
{
    bool ok = false;
    ثابت حرف *ident;

    انتخاب (اظہاریہ->common.type) {
    حالت STMT_اظہاریہ_BOOLEAN_LITERAL:
        *سیٹ_rtrn = اظہاریہ->boolean.سیٹ;
        واپس true;

    حالت STMT_اظہاریہ_STRING_LITERAL:
    حالت STMT_اظہاریہ_INTEGER_LITERAL:
    حالت STMT_اظہاریہ_FLOAT_LITERAL:
    حالت STMT_اظہاریہ_KEYNAME_LITERAL:
    حالت STMT_اظہاریہ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_مقصد_string(اظہاریہ->common.type));
        واپس false;

    حالت STMT_اظہاریہ_IDENT:
        ident = xkb_atom_متن(ctx, اظہاریہ->ident.ident);
        اگر (ident) {
            اگر (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *سیٹ_rtrn = true;
                واپس true;
            }
            ورنہ اگر (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *سیٹ_rtrn = false;
                واپس true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        واپس false;

    حالت STMT_اظہاریہ_میدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.element),
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.میدان_2));
        واپس false;

    حالت STMT_اظہاریہ_INVERT:
    حالت STMT_اظہاریہ_NOT:
        ok = اظہاریہ_Resolve_Boolean(ctx, اظہاریہ->unary.بچہ, سیٹ_rtrn);
        اگر (ok)
            *سیٹ_rtrn = !*سیٹ_rtrn;
        واپس ok;
    حالت STMT_اظہاریہ_جوڑو:
    حالت STMT_اظہاریہ_تفریق:
    حالت STMT_اظہاریہ_MULTIPLY:
    حالت STMT_اظہاریہ_DIVIDE:
    حالت STMT_اظہاریہ_تفویض:
    حالت STMT_اظہاریہ_NEGATE:
    حالت STMT_اظہاریہ_UNARY_PLUS:
    حالت STMT_اظہاریہ_خالی_خانے_LIST:
    حالت STMT_اظہاریہ_ACTION_DECL:
    حالت STMT_اظہاریہ_ACTION_LIST:
    حالت STMT_اظہاریہ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_مقصد_string(اظہاریہ->common.type));
        توڑو;

    طےشدہ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 اظہاریہ->common.type);
        توڑو;
    }

    واپس false;
}

ساکن bool
اظہاریہ_Resolve_Integer_Lookup(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ثابت خالی *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    اظہاریہ_Def *بایاں, *دایاں;

    انتخاب (اظہاریہ->common.type) {
    حالت STMT_اظہاریہ_INTEGER_LITERAL:
        *val_rtrn = اظہاریہ->integer.ival;
        واپس true;

    حالت STMT_اظہاریہ_STRING_LITERAL:
    حالت STMT_اظہاریہ_FLOAT_LITERAL:
    حالت STMT_اظہاریہ_BOOLEAN_LITERAL:
    حالت STMT_اظہاریہ_KEYNAME_LITERAL:
    حالت STMT_اظہاریہ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_مقصد_string(اظہاریہ->common.type));
        واپس false;

    حالت STMT_اظہاریہ_IDENT:
        اگر (lookup)
            ok = lookup(ctx, lookupPriv, اظہاریہ->ident.ident, &u, pending);

        اگر (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_متن(ctx, اظہاریہ->ident.ident));
        ورنہ
            *val_rtrn = (int64_t) u;

        اگر (pending && *pending) {
            /* Postpone computation */
            واپس false;
        }

        واپس ok;

    حالت STMT_اظہاریہ_میدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.element),
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.میدان_2));
        واپس false;

    حالت STMT_اظہاریہ_جوڑو:
    حالت STMT_اظہاریہ_تفریق:
    حالت STMT_اظہاریہ_MULTIPLY:
    حالت STMT_اظہاریہ_DIVIDE:
        بایاں = اظہاریہ->ثنائی.بایاں;
        دایاں = اظہاریہ->ثنائی.دایاں;
        اگر (!اظہاریہ_Resolve_Integer_Lookup(ctx, بایاں, &l, pending, lookup, lookupPriv) ||
            !اظہاریہ_Resolve_Integer_Lookup(ctx, دایاں, &r, pending, lookup, lookupPriv))
            واپس false;

        انتخاب (اظہاریہ->common.type) {
        حالت STMT_اظہاریہ_جوڑو:
            اگر (ckd_جوڑو(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_حد_سے_تجاوز,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                واپس false;
            }
            توڑو;
        حالت STMT_اظہاریہ_تفریق:
            اگر (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_حد_سے_تجاوز,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                واپس false;
            }
            توڑو;
        حالت STMT_اظہاریہ_MULTIPLY:
            اگر (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_حد_سے_تجاوز,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                واپس false;
            }
            توڑو;
        حالت STMT_اظہاریہ_DIVIDE:
            اگر (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                واپس false;
            }
            *val_rtrn = l / r;
            توڑو;
        طےشدہ:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_مقصد_string(اظہاریہ->common.type));
            واپس false;
        }

        واپس true;

    حالت STMT_اظہاریہ_تفویض:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        توڑو;

    حالت STMT_اظہاریہ_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        واپس false;

    حالت STMT_اظہاریہ_INVERT:
    حالت STMT_اظہاریہ_NEGATE:
        بایاں = اظہاریہ->unary.بچہ;
        اگر (!اظہاریہ_Resolve_Integer_Lookup(ctx, بایاں, &l, pending, lookup, lookupPriv))
            واپس false;

        *val_rtrn = (اظہاریہ->common.type == STMT_اظہاریہ_NEGATE ? -l : ~l);
        واپس true;

    حالت STMT_اظہاریہ_UNARY_PLUS:
        بایاں = اظہاریہ->unary.بچہ;
        واپس اظہاریہ_Resolve_Integer_Lookup(ctx, بایاں, val_rtrn, pending, lookup,
                                        lookupPriv);

    طےشدہ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 اظہاریہ->common.type);
        توڑو;
    }

    واپس false;
}

bool
اظہاریہ_Resolve_Integer(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   int64_t *val_rtrn)
{
    واپس اظہاریہ_Resolve_Integer_Lookup(ctx, اظہاریہ, val_rtrn, تہی, تہی, تہی);
}

شمار xkb_parser_error
اظہاریہ_Resolve_گروہ(ثابت ساخت xkb_keymap_info *keymap_info,
                 ثابت اظہاریہ_Def *اظہاریہ, bool absolute,
                 xkb_layout_اشاریہ_t *گروہ_rtrn, bool *pending)
{
    ساکن ثابت Lookup_اندراج pending_گروہ_اشاریہ_نام[] = {
        { گروہ_آخری_اشاریہ_NAME, 0 },
        { تہی, 0 }
    };

    ثابت ساخت named_integer_نمونہ گروہ_name_نمونہ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .سابقہ = "Group",
        .سابقہ_لمبائی = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_نقاب = false,
        .entries = keymap_info->lookup.گروہ_اشاریہ_نام,
        .pending_entries = pending_گروہ_اشاریہ_نام,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
    };

    int64_t نتیجہ = 0;
    اگر (!اظہاریہ_Resolve_Integer_Lookup(keymap_info->keymap.ctx, اظہاریہ, &نتیجہ, pending,
                                  Named_Integer_نمونہ_Lookup, &گروہ_name_نمونہ))
        واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    اگر (نتیجہ < (int64_t)absolute || نتیجہ > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                نتیجہ, absolute, keymap_info->features.max_groups);
        واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *گروہ_rtrn = (xkb_layout_اشاریہ_t) نتیجہ;
    واپس PARSER_SUCCESS;
}

bool
اظہاریہ_Resolve_Level(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                 xkb_level_اشاریہ_t *level_rtrn)
{
    ساکن ثابت ساخت named_integer_نمونہ level_name_نمونہ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .سابقہ = "Level",
        .سابقہ_لمبائی = حجم("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_نقاب = false,
        .entries = تہی,
        .error_id = XKB_ERROR_UNSUPPORTED_سرکاؤ_LEVEL,
    };
    int64_t نتیجہ = 0;
    اگر (!اظہاریہ_Resolve_Integer_Lookup(ctx, اظہاریہ, &نتیجہ, تہی,
                                  Named_Integer_نمونہ_Lookup,
                                  &level_name_نمونہ))
        واپس false;

    اگر (نتیجہ < 1 || نتیجہ > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_سرکاؤ_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                نتیجہ, XKB_LEVEL_MAX_IMPL);
        واپس false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_اشاریہ_t) (نتیجہ - 1);
    واپس true;
}

bool
اظہاریہ_Resolve_Button(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ, int64_t *btn_rtrn)
{
    واپس اظہاریہ_Resolve_Integer_Lookup(ctx, اظہاریہ, btn_rtrn, تہی, SimpleLookup,
                                    button_نام);
}

bool
اظہاریہ_Resolve_String(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                  xkb_atom_t *val_rtrn)
{
    انتخاب (اظہاریہ->common.type) {
    حالت STMT_اظہاریہ_STRING_LITERAL:
        *val_rtrn = اظہاریہ->string.str;
        واپس true;

    حالت STMT_اظہاریہ_INTEGER_LITERAL:
    حالت STMT_اظہاریہ_FLOAT_LITERAL:
    حالت STMT_اظہاریہ_BOOLEAN_LITERAL:
    حالت STMT_اظہاریہ_KEYNAME_LITERAL:
    حالت STMT_اظہاریہ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Found %s, expected a string\n",
                stmt_type_مقصد_string(اظہاریہ->common.type));
        واپس false;

    حالت STMT_اظہاریہ_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_متن(ctx, اظہاریہ->ident.ident));
        واپس false;

    حالت STMT_اظہاریہ_میدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.element),
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.میدان_2));
        واپس false;

    حالت STMT_اظہاریہ_جوڑو:
    حالت STMT_اظہاریہ_تفریق:
    حالت STMT_اظہاریہ_MULTIPLY:
    حالت STMT_اظہاریہ_DIVIDE:
    حالت STMT_اظہاریہ_تفویض:
    حالت STMT_اظہاریہ_NEGATE:
    حالت STMT_اظہاریہ_INVERT:
    حالت STMT_اظہاریہ_NOT:
    حالت STMT_اظہاریہ_UNARY_PLUS:
    حالت STMT_اظہاریہ_خالی_خانے_LIST:
    حالت STMT_اظہاریہ_ACTION_DECL:
    حالت STMT_اظہاریہ_ACTION_LIST:
    حالت STMT_اظہاریہ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_مقصد_string(اظہاریہ->common.type));
        واپس false;

    طےشدہ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 اظہاریہ->common.type);
        توڑو;
    }
    واپس false;
}

bool
اظہاریہ_Resolve_Enum(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                uint32_t *val_rtrn, ثابت Lookup_اندراج *اقدار)
{
    اگر (اظہاریہ->common.type != STMT_اظہاریہ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_مقصد_string(اظہاریہ->common.type));
        واپس false;
    }

    اگر (!SimpleLookup(ctx, اقدار, اظہاریہ->ident.ident, val_rtrn, تہی)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_متن(ctx, اظہاریہ->ident.ident));
        جب (اقدار && اقدار->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", اقدار->name);
            اقدار++;
        }
        واپس false;
    }

    واپس true;
}

ساکن bool
اظہاریہ_Resolve_نقاب_Lookup(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ثابت خالی *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    اظہاریہ_Def *بایاں, *دایاں;
    ثابت حرف *bogus = تہی;

    انتخاب (اظہاریہ->common.type) {
    حالت STMT_اظہاریہ_INTEGER_LITERAL:
        اگر (اظہاریہ->integer.ival < 0 || اظہاریہ->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    اظہاریہ->integer.ival < 0 ? "-" : "",
                    imaxabs(اظہاریہ->integer.ival),
                    UINT32_MAX);
            واپس false;
        }
        *val_rtrn = (uint32_t) اظہاریہ->integer.ival;
        واپس true;

    حالت STMT_اظہاریہ_STRING_LITERAL:
    حالت STMT_اظہاریہ_FLOAT_LITERAL:
    حالت STMT_اظہاریہ_BOOLEAN_LITERAL:
    حالت STMT_اظہاریہ_KEYNAME_LITERAL:
    حالت STMT_اظہاریہ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_مقصد_string(اظہاریہ->common.type));
        واپس false;

    حالت STMT_اظہاریہ_IDENT:
        ok = lookup(ctx, lookupPriv, اظہاریہ->ident.ident, val_rtrn, pending);
        اگر (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_متن(ctx, اظہاریہ->ident.ident));
        اگر (pending && *pending) {
            /* Postpone computation */
            واپس false;
        }
        واپس ok;

    حالت STMT_اظہاریہ_میدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.element),
                xkb_atom_متن(ctx, اظہاریہ->میدان_ref.میدان_2));
        واپس false;

    حالت STMT_اظہاریہ_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    حالت STMT_اظہاریہ_ACTION_DECL:
        اگر (bogus == تہی)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        واپس false;

    حالت STMT_اظہاریہ_جوڑو:
    حالت STMT_اظہاریہ_تفریق:
    حالت STMT_اظہاریہ_MULTIPLY:
    حالت STMT_اظہاریہ_DIVIDE:
        بایاں = اظہاریہ->ثنائی.بایاں;
        دایاں = اظہاریہ->ثنائی.دایاں;
        اگر (!اظہاریہ_Resolve_نقاب_Lookup(ctx, بایاں, &l, pending, lookup, lookupPriv) ||
            !اظہاریہ_Resolve_نقاب_Lookup(ctx, دایاں, &r, pending, lookup, lookupPriv))
            واپس false;

        انتخاب (اظہاریہ->common.type) {
        حالت STMT_اظہاریہ_جوڑو:
            *val_rtrn = l | r;
            توڑو;
        حالت STMT_اظہاریہ_تفریق:
            *val_rtrn = l & (~r);
            توڑو;
        حالت STMT_اظہاریہ_MULTIPLY:
        حالت STMT_اظہاریہ_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (اظہاریہ->common.type == STMT_اظہاریہ_DIVIDE ? "divide" : "multiply"));
            واپس false;
        طےشدہ:
            توڑو;
        }

        واپس true;

    حالت STMT_اظہاریہ_تفویض:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        توڑو;

    حالت STMT_اظہاریہ_INVERT:
        بایاں = اظہاریہ->unary.بچہ;
        اگر (!اظہاریہ_Resolve_Integer_Lookup(ctx, بایاں, &v, pending, lookup, lookupPriv))
            واپس false;
        اگر (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            واپس false;
        }
        *val_rtrn = ~(uint32_t) v;
        واپس true;

    حالت STMT_اظہاریہ_UNARY_PLUS:
    حالت STMT_اظہاریہ_NEGATE:
    حالت STMT_اظہاریہ_NOT:
        بایاں = اظہاریہ->unary.بچہ;
        اگر (!اظہاریہ_Resolve_Integer_Lookup(ctx, بایاں, &v, pending, lookup, lookupPriv))
            واپس false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_مقصد_operator_char(اظہاریہ->common.type));
        واپس false;

    طےشدہ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 اظہاریہ->common.type);
        توڑو;
    }

    واپس false;
}

bool
اظہاریہ_Resolve_نقاب(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                uint32_t *نقاب_rtrn, ثابت Lookup_اندراج *اقدار)
{
    واپس اظہاریہ_Resolve_نقاب_Lookup(ctx, اظہاریہ, نقاب_rtrn, تہی, SimpleLookup, اقدار);
}

bool
اظہاریہ_Resolve_Mod_نقاب(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *اظہاریہ,
                   شمار mod_type mod_type, ثابت ساخت xkb_mod_سیٹ *mods,
                   xkb_mod_نقاب_t *نقاب_rtrn)
{
    Lookup_Mod_نقاب_Priv priv = { .mods = mods, .mod_type = mod_type };
    واپس اظہاریہ_Resolve_نقاب_Lookup(ctx, اظہاریہ, نقاب_rtrn, تہی, Lookup_Mod_نقاب, &priv);
}

bool
اظہاریہ_Resolve_Mod(ساخت xkb_context *ctx, ثابت اظہاریہ_Def *def,
               شمار mod_type mod_type, ثابت ساخت xkb_mod_سیٹ *mods,
               xkb_mod_اشاریہ_t *ndx_rtrn)
{
    اگر (def->common.type != STMT_اظہاریہ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_مقصد_string(def->common.type));
        واپس false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_اشاریہ_t ndx = Xkb_Mod_Name_مقصد_اشاریہ(mods, name, mod_type);
    اگر (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_متن(ctx, name));
        واپس false;
    }

    *ndx_rtrn = ndx;
    واپس true;
}

bool
اظہاریہ_Resolve_گروہ_نقاب(ثابت ساخت xkb_keymap_info *keymap_info,
                     ثابت اظہاریہ_Def *اظہاریہ, xkb_layout_نقاب_t *گروہ_rtrn,
                     bool *pending_rtrn)
{
    ساکن ثابت Lookup_اندراج pending_گروہ_نقاب_نام[] = {
        { گروہ_آخری_اشاریہ_NAME, 0 },
        { تہی, 0 }
    };

    ثابت ساخت named_integer_نمونہ گروہ_name_نمونہ = {
        /* Prefix is title-cased, because it is also used in error messages */
        .سابقہ = "Group",
        .سابقہ_لمبائی = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_نقاب = true,
        .entries = keymap_info->lookup.گروہ_نقاب_نام,
        .pending_entries = pending_گروہ_نقاب_نام,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2
    };

    واپس اظہاریہ_Resolve_نقاب_Lookup(keymap_info->keymap.ctx, اظہاریہ, گروہ_rtrn,
                                 pending_rtrn, Named_Integer_نمونہ_Lookup,
                                 &گروہ_name_نمونہ);
}
