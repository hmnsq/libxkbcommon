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
                                xkb_atom_t ډګر_2, uint32_t *val_rtrn,
                                bool *pending);

bool
بیان_Resolve_Lhs(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
               ثابت حرف **elem_rtrn, ثابت حرف **ډګر_rtrn,
               بیان_Def **شاخص_rtrn)
{
    بدل (بیان->common.type) {
    حالة STMT_بیان_IDENT:
        *elem_rtrn = عدم;
        *ډګر_rtrn = xkb_atom_متن(ctx, بیان->ident.ident);
        *شاخص_rtrn = عدم;
        أرجع (*ډګر_rtrn != عدم);
    حالة STMT_بیان_ډګر_REF:
        *elem_rtrn = xkb_atom_متن(ctx, بیان->ډګر_ref.element);
        *ډګر_rtrn = xkb_atom_متن(ctx, بیان->ډګر_ref.ډګر_2);
        *شاخص_rtrn = عدم;
        أرجع (*elem_rtrn != عدم && *ډګر_rtrn != عدم);
    حالة STMT_بیان_ARRAY_REF:
        *elem_rtrn = xkb_atom_متن(ctx, بیان->array_ref.element);
        *ډګر_rtrn = xkb_atom_متن(ctx, بیان->array_ref.ډګر_2);
        *شاخص_rtrn = بیان->array_ref.ثبت;
        إذا (بیان->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == عدم)
            أرجع false;
        إذا (*ډګر_rtrn == عدم)
            أرجع false;
        أرجع true;
    افتراضي:
        اكسر;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", بیان->common.type);
    أرجع false;
}

ساكن bool
SimpleLookup(بنية xkb_context *ctx, ثابت فراغ *priv, xkb_atom_t ډګر_2,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    إذا (!priv || ډګر_2 == XKB_ATOM_NONE)
        أرجع false;

    ثابت حرف *str = xkb_atom_متن(ctx, ډګر_2);
    لكل (ثابت Lookup_ثبت *ثبت = priv; ثبت && ثبت->name; ثبت++) {
        إذا (istreq(str, ثبت->name)) {
            *val_rtrn = ثبت->ارزښت;
            أرجع true;
        }
    }

    أرجع false;
}

بنية named_integer_نمونه {
    ثابت حرف *مختاړی;
    اندازه_t مختاړی_اوږدتیا;
    uint32_t min;
    uint32_t max;
    ثابت Lookup_ثبت *entries;
    ثابت Lookup_ثبت *pending_entries;
    bool is_ماسک;
    تعداد xkb_پیغام_کوډ error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ساكن bool
Named_Integer_نمونه_Lookup(بنية xkb_context *ctx, ثابت فراغ *priv,
                          xkb_atom_t ډګر_2, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    إذا (!priv || ډګر_2 == XKB_ATOM_NONE)
        أرجع false;

    ثابت حرف * ثابت str = xkb_atom_متن(ctx, ډګر_2);
    ثابت بنية named_integer_نمونه * ثابت نمونه = priv;

    ثابت صحيح شمېر = (istrneq(str, نمونه->مختاړی, نمونه->مختاړی_اوږدتیا))
        ? parse_dec_منزل_uint32_t(str + نمونه->مختاړی_اوږدتیا, SIZE_MAX, val_rtrn)
        : 0;

    إذا (شمېر > 0 && *(str + نمونه->مختاړی_اوږدتیا + شمېر) == '\0') {
        إذا (*val_rtrn < نمونه->min || *val_rtrn > نمونه->max) {
            log_err(ctx, نمونه->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    نمونه->مختاړی, *val_rtrn, نمونه->min, نمونه->max);
            أرجع false;
        }
        إذا (نمونه->is_ماسک) {
            /* Compute mask (bit 0 = min index) */
            أكد(*val_rtrn - نمونه->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - نمونه->min);
        }
        أرجع true;
    } وإلا {
        إذا (نمونه->entries &&
            SimpleLookup(ctx, نمونه->entries, ډګر_2, val_rtrn, عدم)) {
            أرجع true;
        }
        إذا (نمونه->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, نمونه->pending_entries, ډګر_2, val_rtrn, عدم)) {
            *pending_rtrn = true;
            أرجع true;
        }
        أرجع false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
تعريفنوع بنية {
    ثابت بنية xkb_mod_سېټ *mods;
    تعداد mod_type mod_type;
} Lookup_Mod_ماسک_Priv;

ساكن bool
Lookup_Mod_ماسک(بنية xkb_context *ctx, ثابت فراغ *priv, xkb_atom_t ډګر_2,
              xkb_mod_ماسک_t *val_rtrn, bool *pending_rtrn)
{
    ثابت حرف *str = xkb_atom_متن(ctx, ډګر_2);
    إذا (!str)
        أرجع false;

    إذا (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_ماسک_ALL;
        أرجع true;
    }

    إذا (istreq(str, "none")) {
        *val_rtrn = 0;
        أرجع true;
    }

    ثابت Lookup_Mod_ماسک_Priv *arg = priv;
    ثابت بنية xkb_mod_سېټ *mods = arg->mods;
    ثابت تعداد mod_type mod_type = arg->mod_type;
    ثابت xkb_mod_شاخص_t ndx = Xkb_Mod_Name_منزل_شاخص(mods, ډګر_2, mod_type);
    إذا (ndx == XKB_MOD_INVALID)
        أرجع false;

    *val_rtrn = (UINT32_C(1) << ndx);
    أرجع true;
}

bool
بیان_Resolve_Boolean(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                   bool *سېټ_rtrn)
{
    bool ok = false;
    ثابت حرف *ident;

    بدل (بیان->common.type) {
    حالة STMT_بیان_BOOLEAN_LITERAL:
        *سېټ_rtrn = بیان->boolean.سېټ;
        أرجع true;

    حالة STMT_بیان_STRING_LITERAL:
    حالة STMT_بیان_INTEGER_LITERAL:
    حالة STMT_بیان_FLOAT_LITERAL:
    حالة STMT_بیان_KEYNAME_LITERAL:
    حالة STMT_بیان_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_منزل_string(بیان->common.type));
        أرجع false;

    حالة STMT_بیان_IDENT:
        ident = xkb_atom_متن(ctx, بیان->ident.ident);
        إذا (ident) {
            إذا (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *سېټ_rtrn = true;
                أرجع true;
            }
            وإلا إذا (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *سېټ_rtrn = false;
                أرجع true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        أرجع false;

    حالة STMT_بیان_ډګر_REF:
        log_err(ctx, XKB_ERROR_INVALID_افاده_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_متن(ctx, بیان->ډګر_ref.element),
                xkb_atom_متن(ctx, بیان->ډګر_ref.ډګر_2));
        أرجع false;

    حالة STMT_بیان_INVERT:
    حالة STMT_بیان_NOT:
        ok = بیان_Resolve_Boolean(ctx, بیان->unary.ماشوم, سېټ_rtrn);
        إذا (ok)
            *سېټ_rtrn = !*سېټ_rtrn;
        أرجع ok;
    حالة STMT_بیان_جمع_کړه:
    حالة STMT_بیان_کمول:
    حالة STMT_بیان_MULTIPLY:
    حالة STMT_بیان_DIVIDE:
    حالة STMT_بیان_ټاکل:
    حالة STMT_بیان_NEGATE:
    حالة STMT_بیان_UNARY_PLUS:
    حالة STMT_بیان_تشه_LIST:
    حالة STMT_بیان_ACTION_DECL:
    حالة STMT_بیان_ACTION_LIST:
    حالة STMT_بیان_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_منزل_string(بیان->common.type));
        اكسر;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 بیان->common.type);
        اكسر;
    }

    أرجع false;
}

ساكن bool
بیان_Resolve_Integer_Lookup(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ثابت فراغ *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    بیان_Def *کیڼ, *ښی;

    بدل (بیان->common.type) {
    حالة STMT_بیان_INTEGER_LITERAL:
        *val_rtrn = بیان->integer.ival;
        أرجع true;

    حالة STMT_بیان_STRING_LITERAL:
    حالة STMT_بیان_FLOAT_LITERAL:
    حالة STMT_بیان_BOOLEAN_LITERAL:
    حالة STMT_بیان_KEYNAME_LITERAL:
    حالة STMT_بیان_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_منزل_string(بیان->common.type));
        أرجع false;

    حالة STMT_بیان_IDENT:
        إذا (lookup)
            ok = lookup(ctx, lookupPriv, بیان->ident.ident, &u, pending);

        إذا (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_متن(ctx, بیان->ident.ident));
        وإلا
            *val_rtrn = (int64_t) u;

        إذا (pending && *pending) {
            /* Postpone computation */
            أرجع false;
        }

        أرجع ok;

    حالة STMT_بیان_ډګر_REF:
        log_err(ctx, XKB_ERROR_INVALID_افاده_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_متن(ctx, بیان->ډګر_ref.element),
                xkb_atom_متن(ctx, بیان->ډګر_ref.ډګر_2));
        أرجع false;

    حالة STMT_بیان_جمع_کړه:
    حالة STMT_بیان_کمول:
    حالة STMT_بیان_MULTIPLY:
    حالة STMT_بیان_DIVIDE:
        کیڼ = بیان->دوګونی.کیڼ;
        ښی = بیان->دوګونی.ښی;
        إذا (!بیان_Resolve_Integer_Lookup(ctx, کیڼ, &l, pending, lookup, lookupPriv) ||
            !بیان_Resolve_Integer_Lookup(ctx, ښی, &r, pending, lookup, lookupPriv))
            أرجع false;

        بدل (بیان->common.type) {
        حالة STMT_بیان_جمع_کړه:
            إذا (ckd_جمع_کړه(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_ډکېدنه,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_بیان_کمول:
            إذا (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_ډکېدنه,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_بیان_MULTIPLY:
            إذا (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_ډکېدنه,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_بیان_DIVIDE:
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
                    stmt_type_منزل_string(بیان->common.type));
            أرجع false;
        }

        أرجع true;

    حالة STMT_بیان_ټاکل:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        اكسر;

    حالة STMT_بیان_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        أرجع false;

    حالة STMT_بیان_INVERT:
    حالة STMT_بیان_NEGATE:
        کیڼ = بیان->unary.ماشوم;
        إذا (!بیان_Resolve_Integer_Lookup(ctx, کیڼ, &l, pending, lookup, lookupPriv))
            أرجع false;

        *val_rtrn = (بیان->common.type == STMT_بیان_NEGATE ? -l : ~l);
        أرجع true;

    حالة STMT_بیان_UNARY_PLUS:
        کیڼ = بیان->unary.ماشوم;
        أرجع بیان_Resolve_Integer_Lookup(ctx, کیڼ, val_rtrn, pending, lookup,
                                        lookupPriv);

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 بیان->common.type);
        اكسر;
    }

    أرجع false;
}

bool
بیان_Resolve_Integer(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                   int64_t *val_rtrn)
{
    أرجع بیان_Resolve_Integer_Lookup(ctx, بیان, val_rtrn, عدم, عدم, عدم);
}

تعداد xkb_parser_error
بیان_Resolve_ډله(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت بیان_Def *بیان, bool absolute,
                 xkb_layout_شاخص_t *ډله_rtrn, bool *pending)
{
    ساكن ثابت Lookup_ثبت pending_ډله_شاخص_نومونه[] = {
        { ډله_وروستی_شاخص_NAME, 0 },
        { عدم, 0 }
    };

    ثابت بنية named_integer_نمونه ډله_name_نمونه = {
        /* Prefix is title-cased, because it is also used in error messages */
        .مختاړی = "Group",
        .مختاړی_اوږدتیا = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_ماسک = false,
        .entries = keymap_info->lookup.ډله_شاخص_نومونه,
        .pending_entries = pending_ډله_شاخص_نومونه,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
    };

    int64_t پایله = 0;
    إذا (!بیان_Resolve_Integer_Lookup(keymap_info->keymap.ctx, بیان, &پایله, pending,
                                  Named_Integer_نمونه_Lookup, &ډله_name_نمونه))
        أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    إذا (پایله < (int64_t)absolute || پایله > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                پایله, absolute, keymap_info->features.max_groups);
        أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *ډله_rtrn = (xkb_layout_شاخص_t) پایله;
    أرجع PARSER_SUCCESS;
}

bool
بیان_Resolve_Level(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                 xkb_level_شاخص_t *level_rtrn)
{
    ساكن ثابت بنية named_integer_نمونه level_name_نمونه = {
        /* Prefix is title-cased, because it is also used in error messages */
        .مختاړی = "Level",
        .مختاړی_اوږدتیا = حجم("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_ماسک = false,
        .entries = عدم,
        .error_id = XKB_ERROR_UNSUPPORTED_ښويدنه_LEVEL,
    };
    int64_t پایله = 0;
    إذا (!بیان_Resolve_Integer_Lookup(ctx, بیان, &پایله, عدم,
                                  Named_Integer_نمونه_Lookup,
                                  &level_name_نمونه))
        أرجع false;

    إذا (پایله < 1 || پایله > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_ښويدنه_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                پایله, XKB_LEVEL_MAX_IMPL);
        أرجع false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_شاخص_t) (پایله - 1);
    أرجع true;
}

bool
بیان_Resolve_Button(بنية xkb_context *ctx, ثابت بیان_Def *بیان, int64_t *btn_rtrn)
{
    أرجع بیان_Resolve_Integer_Lookup(ctx, بیان, btn_rtrn, عدم, SimpleLookup,
                                    button_نومونه);
}

bool
بیان_Resolve_String(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                  xkb_atom_t *val_rtrn)
{
    بدل (بیان->common.type) {
    حالة STMT_بیان_STRING_LITERAL:
        *val_rtrn = بیان->string.str;
        أرجع true;

    حالة STMT_بیان_INTEGER_LITERAL:
    حالة STMT_بیان_FLOAT_LITERAL:
    حالة STMT_بیان_BOOLEAN_LITERAL:
    حالة STMT_بیان_KEYNAME_LITERAL:
    حالة STMT_بیان_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Found %s, expected a string\n",
                stmt_type_منزل_string(بیان->common.type));
        أرجع false;

    حالة STMT_بیان_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_متن(ctx, بیان->ident.ident));
        أرجع false;

    حالة STMT_بیان_ډګر_REF:
        log_err(ctx, XKB_ERROR_INVALID_افاده_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_متن(ctx, بیان->ډګر_ref.element),
                xkb_atom_متن(ctx, بیان->ډګر_ref.ډګر_2));
        أرجع false;

    حالة STMT_بیان_جمع_کړه:
    حالة STMT_بیان_کمول:
    حالة STMT_بیان_MULTIPLY:
    حالة STMT_بیان_DIVIDE:
    حالة STMT_بیان_ټاکل:
    حالة STMT_بیان_NEGATE:
    حالة STMT_بیان_INVERT:
    حالة STMT_بیان_NOT:
    حالة STMT_بیان_UNARY_PLUS:
    حالة STMT_بیان_تشه_LIST:
    حالة STMT_بیان_ACTION_DECL:
    حالة STMT_بیان_ACTION_LIST:
    حالة STMT_بیان_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_منزل_string(بیان->common.type));
        أرجع false;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 بیان->common.type);
        اكسر;
    }
    أرجع false;
}

bool
بیان_Resolve_Enum(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                uint32_t *val_rtrn, ثابت Lookup_ثبت *ارزښتونه)
{
    إذا (بیان->common.type != STMT_بیان_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_منزل_string(بیان->common.type));
        أرجع false;
    }

    إذا (!SimpleLookup(ctx, ارزښتونه, بیان->ident.ident, val_rtrn, عدم)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_متن(ctx, بیان->ident.ident));
        طالما (ارزښتونه && ارزښتونه->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", ارزښتونه->name);
            ارزښتونه++;
        }
        أرجع false;
    }

    أرجع true;
}

ساكن bool
بیان_Resolve_ماسک_Lookup(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ثابت فراغ *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    بیان_Def *کیڼ, *ښی;
    ثابت حرف *bogus = عدم;

    بدل (بیان->common.type) {
    حالة STMT_بیان_INTEGER_LITERAL:
        إذا (بیان->integer.ival < 0 || بیان->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    بیان->integer.ival < 0 ? "-" : "",
                    imaxabs(بیان->integer.ival),
                    UINT32_MAX);
            أرجع false;
        }
        *val_rtrn = (uint32_t) بیان->integer.ival;
        أرجع true;

    حالة STMT_بیان_STRING_LITERAL:
    حالة STMT_بیان_FLOAT_LITERAL:
    حالة STMT_بیان_BOOLEAN_LITERAL:
    حالة STMT_بیان_KEYNAME_LITERAL:
    حالة STMT_بیان_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_منزل_string(بیان->common.type));
        أرجع false;

    حالة STMT_بیان_IDENT:
        ok = lookup(ctx, lookupPriv, بیان->ident.ident, val_rtrn, pending);
        إذا (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_متن(ctx, بیان->ident.ident));
        إذا (pending && *pending) {
            /* Postpone computation */
            أرجع false;
        }
        أرجع ok;

    حالة STMT_بیان_ډګر_REF:
        log_err(ctx, XKB_ERROR_INVALID_افاده_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_متن(ctx, بیان->ډګر_ref.element),
                xkb_atom_متن(ctx, بیان->ډګر_ref.ډګر_2));
        أرجع false;

    حالة STMT_بیان_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    حالة STMT_بیان_ACTION_DECL:
        إذا (bogus == عدم)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        أرجع false;

    حالة STMT_بیان_جمع_کړه:
    حالة STMT_بیان_کمول:
    حالة STMT_بیان_MULTIPLY:
    حالة STMT_بیان_DIVIDE:
        کیڼ = بیان->دوګونی.کیڼ;
        ښی = بیان->دوګونی.ښی;
        إذا (!بیان_Resolve_ماسک_Lookup(ctx, کیڼ, &l, pending, lookup, lookupPriv) ||
            !بیان_Resolve_ماسک_Lookup(ctx, ښی, &r, pending, lookup, lookupPriv))
            أرجع false;

        بدل (بیان->common.type) {
        حالة STMT_بیان_جمع_کړه:
            *val_rtrn = l | r;
            اكسر;
        حالة STMT_بیان_کمول:
            *val_rtrn = l & (~r);
            اكسر;
        حالة STMT_بیان_MULTIPLY:
        حالة STMT_بیان_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (بیان->common.type == STMT_بیان_DIVIDE ? "divide" : "multiply"));
            أرجع false;
        افتراضي:
            اكسر;
        }

        أرجع true;

    حالة STMT_بیان_ټاکل:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        اكسر;

    حالة STMT_بیان_INVERT:
        کیڼ = بیان->unary.ماشوم;
        إذا (!بیان_Resolve_Integer_Lookup(ctx, کیڼ, &v, pending, lookup, lookupPriv))
            أرجع false;
        إذا (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            أرجع false;
        }
        *val_rtrn = ~(uint32_t) v;
        أرجع true;

    حالة STMT_بیان_UNARY_PLUS:
    حالة STMT_بیان_NEGATE:
    حالة STMT_بیان_NOT:
        کیڼ = بیان->unary.ماشوم;
        إذا (!بیان_Resolve_Integer_Lookup(ctx, کیڼ, &v, pending, lookup, lookupPriv))
            أرجع false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_منزل_operator_char(بیان->common.type));
        أرجع false;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 بیان->common.type);
        اكسر;
    }

    أرجع false;
}

bool
بیان_Resolve_ماسک(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                uint32_t *ماسک_rtrn, ثابت Lookup_ثبت *ارزښتونه)
{
    أرجع بیان_Resolve_ماسک_Lookup(ctx, بیان, ماسک_rtrn, عدم, SimpleLookup, ارزښتونه);
}

bool
بیان_Resolve_Mod_ماسک(بنية xkb_context *ctx, ثابت بیان_Def *بیان,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_سېټ *mods,
                   xkb_mod_ماسک_t *ماسک_rtrn)
{
    Lookup_Mod_ماسک_Priv priv = { .mods = mods, .mod_type = mod_type };
    أرجع بیان_Resolve_ماسک_Lookup(ctx, بیان, ماسک_rtrn, عدم, Lookup_Mod_ماسک, &priv);
}

bool
بیان_Resolve_Mod(بنية xkb_context *ctx, ثابت بیان_Def *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_سېټ *mods,
               xkb_mod_شاخص_t *ndx_rtrn)
{
    إذا (def->common.type != STMT_بیان_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_منزل_string(def->common.type));
        أرجع false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_شاخص_t ndx = Xkb_Mod_Name_منزل_شاخص(mods, name, mod_type);
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
بیان_Resolve_ډله_ماسک(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت بیان_Def *بیان, xkb_layout_ماسک_t *ډله_rtrn,
                     bool *pending_rtrn)
{
    ساكن ثابت Lookup_ثبت pending_ډله_ماسک_نومونه[] = {
        { ډله_وروستی_شاخص_NAME, 0 },
        { عدم, 0 }
    };

    ثابت بنية named_integer_نمونه ډله_name_نمونه = {
        /* Prefix is title-cased, because it is also used in error messages */
        .مختاړی = "Group",
        .مختاړی_اوږدتیا = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_ماسک = true,
        .entries = keymap_info->lookup.ډله_ماسک_نومونه,
        .pending_entries = pending_ډله_ماسک_نومونه,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2
    };

    أرجع بیان_Resolve_ماسک_Lookup(keymap_info->keymap.ctx, بیان, ډله_rtrn,
                                 pending_rtrn, Named_Integer_نمونه_Lookup,
                                 &ډله_name_نمونه);
}
