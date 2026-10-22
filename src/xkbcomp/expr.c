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
                                xkb_atom_t فیلد_2, uint32_t *val_rtrn,
                                bool *pending);

bool
عبارت_Resolve_Lhs(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
               ثابت حرف **elem_rtrn, ثابت حرف **فیلد_rtrn,
               عبارت_Def **شاخص_rtrn)
{
    بدل (عبارت->common.type) {
    حالة STMT_عبارت_IDENT:
        *elem_rtrn = عدم;
        *فیلد_rtrn = xkb_atom_متن(ctx, عبارت->ident.ident);
        *شاخص_rtrn = عدم;
        أرجع (*فیلد_rtrn != عدم);
    حالة STMT_عبارت_فیلد_REF:
        *elem_rtrn = xkb_atom_متن(ctx, عبارت->فیلد_ref.element);
        *فیلد_rtrn = xkb_atom_متن(ctx, عبارت->فیلد_ref.فیلد_2);
        *شاخص_rtrn = عدم;
        أرجع (*elem_rtrn != عدم && *فیلد_rtrn != عدم);
    حالة STMT_عبارت_ARRAY_REF:
        *elem_rtrn = xkb_atom_متن(ctx, عبارت->array_ref.element);
        *فیلد_rtrn = xkb_atom_متن(ctx, عبارت->array_ref.فیلد_2);
        *شاخص_rtrn = عبارت->array_ref.مدخل;
        إذا (عبارت->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == عدم)
            أرجع false;
        إذا (*فیلد_rtrn == عدم)
            أرجع false;
        أرجع true;
    افتراضي:
        اكسر;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", عبارت->common.type);
    أرجع false;
}

ساكن bool
SimpleLookup(بنية xkb_context *ctx, ثابت فراغ *priv, xkb_atom_t فیلد_2,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    إذا (!priv || فیلد_2 == XKB_ATOM_NONE)
        أرجع false;

    ثابت حرف *str = xkb_atom_متن(ctx, فیلد_2);
    لكل (ثابت Lookup_مدخل *مدخل = priv; مدخل && مدخل->name; مدخل++) {
        إذا (istreq(str, مدخل->name)) {
            *val_rtrn = مدخل->مقدار;
            أرجع true;
        }
    }

    أرجع false;
}

بنية named_integer_الگو {
    ثابت حرف *پیشوند;
    اندازه_t پیشوند_درازی;
    uint32_t min;
    uint32_t max;
    ثابت Lookup_مدخل *entries;
    ثابت Lookup_مدخل *pending_entries;
    bool is_ماسک;
    تعداد xkb_پیام_کد error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ساكن bool
Named_Integer_الگو_Lookup(بنية xkb_context *ctx, ثابت فراغ *priv,
                          xkb_atom_t فیلد_2, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    إذا (!priv || فیلد_2 == XKB_ATOM_NONE)
        أرجع false;

    ثابت حرف * ثابت str = xkb_atom_متن(ctx, فیلد_2);
    ثابت بنية named_integer_الگو * ثابت الگو = priv;

    ثابت صحيح شمار = (istrneq(str, الگو->پیشوند, الگو->پیشوند_درازی))
        ? parse_dec_مقصد_uint32_t(str + الگو->پیشوند_درازی, SIZE_MAX, val_rtrn)
        : 0;

    إذا (شمار > 0 && *(str + الگو->پیشوند_درازی + شمار) == '\0') {
        إذا (*val_rtrn < الگو->min || *val_rtrn > الگو->max) {
            log_err(ctx, الگو->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    الگو->پیشوند, *val_rtrn, الگو->min, الگو->max);
            أرجع false;
        }
        إذا (الگو->is_ماسک) {
            /* Compute mask (bit 0 = min index) */
            أكد(*val_rtrn - الگو->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - الگو->min);
        }
        أرجع true;
    } وإلا {
        إذا (الگو->entries &&
            SimpleLookup(ctx, الگو->entries, فیلد_2, val_rtrn, عدم)) {
            أرجع true;
        }
        إذا (الگو->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, الگو->pending_entries, فیلد_2, val_rtrn, عدم)) {
            *pending_rtrn = true;
            أرجع true;
        }
        أرجع false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
تعريفنوع بنية {
    ثابت بنية xkb_mod_مجموعه *mods;
    تعداد mod_type mod_type;
} Lookup_Mod_ماسک_Priv;

ساكن bool
Lookup_Mod_ماسک(بنية xkb_context *ctx, ثابت فراغ *priv, xkb_atom_t فیلد_2,
              xkb_mod_ماسک_t *val_rtrn, bool *pending_rtrn)
{
    ثابت حرف *str = xkb_atom_متن(ctx, فیلد_2);
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
    ثابت بنية xkb_mod_مجموعه *mods = arg->mods;
    ثابت تعداد mod_type mod_type = arg->mod_type;
    ثابت xkb_mod_شاخص_t ndx = Xkb_Mod_Name_مقصد_شاخص(mods, فیلد_2, mod_type);
    إذا (ndx == XKB_MOD_INVALID)
        أرجع false;

    *val_rtrn = (UINT32_C(1) << ndx);
    أرجع true;
}

bool
عبارت_Resolve_Boolean(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   bool *مجموعه_rtrn)
{
    bool ok = false;
    ثابت حرف *ident;

    بدل (عبارت->common.type) {
    حالة STMT_عبارت_BOOLEAN_LITERAL:
        *مجموعه_rtrn = عبارت->boolean.مجموعه;
        أرجع true;

    حالة STMT_عبارت_STRING_LITERAL:
    حالة STMT_عبارت_INTEGER_LITERAL:
    حالة STMT_عبارت_FLOAT_LITERAL:
    حالة STMT_عبارت_KEYNAME_LITERAL:
    حالة STMT_عبارت_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_مقصد_string(عبارت->common.type));
        أرجع false;

    حالة STMT_عبارت_IDENT:
        ident = xkb_atom_متن(ctx, عبارت->ident.ident);
        إذا (ident) {
            إذا (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *مجموعه_rtrn = true;
                أرجع true;
            }
            وإلا إذا (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *مجموعه_rtrn = false;
                أرجع true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        أرجع false;

    حالة STMT_عبارت_فیلد_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_متن(ctx, عبارت->فیلد_ref.element),
                xkb_atom_متن(ctx, عبارت->فیلد_ref.فیلد_2));
        أرجع false;

    حالة STMT_عبارت_INVERT:
    حالة STMT_عبارت_NOT:
        ok = عبارت_Resolve_Boolean(ctx, عبارت->unary.فرزند, مجموعه_rtrn);
        إذا (ok)
            *مجموعه_rtrn = !*مجموعه_rtrn;
        أرجع ok;
    حالة STMT_عبارت_جمع_کن:
    حالة STMT_عبارت_تفریق:
    حالة STMT_عبارت_MULTIPLY:
    حالة STMT_عبارت_DIVIDE:
    حالة STMT_عبارت_تخصیص_بده:
    حالة STMT_عبارت_NEGATE:
    حالة STMT_عبارت_UNARY_PLUS:
    حالة STMT_عبارت_خالی_LIST:
    حالة STMT_عبارت_ACTION_DECL:
    حالة STMT_عبارت_ACTION_LIST:
    حالة STMT_عبارت_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_مقصد_string(عبارت->common.type));
        اكسر;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 عبارت->common.type);
        اكسر;
    }

    أرجع false;
}

ساكن bool
عبارت_Resolve_Integer_Lookup(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ثابت فراغ *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    عبارت_Def *چپ, *راست;

    بدل (عبارت->common.type) {
    حالة STMT_عبارت_INTEGER_LITERAL:
        *val_rtrn = عبارت->integer.ival;
        أرجع true;

    حالة STMT_عبارت_STRING_LITERAL:
    حالة STMT_عبارت_FLOAT_LITERAL:
    حالة STMT_عبارت_BOOLEAN_LITERAL:
    حالة STMT_عبارت_KEYNAME_LITERAL:
    حالة STMT_عبارت_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_مقصد_string(عبارت->common.type));
        أرجع false;

    حالة STMT_عبارت_IDENT:
        إذا (lookup)
            ok = lookup(ctx, lookupPriv, عبارت->ident.ident, &u, pending);

        إذا (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_متن(ctx, عبارت->ident.ident));
        وإلا
            *val_rtrn = (int64_t) u;

        إذا (pending && *pending) {
            /* Postpone computation */
            أرجع false;
        }

        أرجع ok;

    حالة STMT_عبارت_فیلد_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_متن(ctx, عبارت->فیلد_ref.element),
                xkb_atom_متن(ctx, عبارت->فیلد_ref.فیلد_2));
        أرجع false;

    حالة STMT_عبارت_جمع_کن:
    حالة STMT_عبارت_تفریق:
    حالة STMT_عبارت_MULTIPLY:
    حالة STMT_عبارت_DIVIDE:
        چپ = عبارت->دودویی.چپ;
        راست = عبارت->دودویی.راست;
        إذا (!عبارت_Resolve_Integer_Lookup(ctx, چپ, &l, pending, lookup, lookupPriv) ||
            !عبارت_Resolve_Integer_Lookup(ctx, راست, &r, pending, lookup, lookupPriv))
            أرجع false;

        بدل (عبارت->common.type) {
        حالة STMT_عبارت_جمع_کن:
            إذا (ckd_جمع_کن(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_سرریز,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_عبارت_تفریق:
            إذا (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_سرریز,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_عبارت_MULTIPLY:
            إذا (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_سرریز,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_عبارت_DIVIDE:
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
                    stmt_type_مقصد_string(عبارت->common.type));
            أرجع false;
        }

        أرجع true;

    حالة STMT_عبارت_تخصیص_بده:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        اكسر;

    حالة STMT_عبارت_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        أرجع false;

    حالة STMT_عبارت_INVERT:
    حالة STMT_عبارت_NEGATE:
        چپ = عبارت->unary.فرزند;
        إذا (!عبارت_Resolve_Integer_Lookup(ctx, چپ, &l, pending, lookup, lookupPriv))
            أرجع false;

        *val_rtrn = (عبارت->common.type == STMT_عبارت_NEGATE ? -l : ~l);
        أرجع true;

    حالة STMT_عبارت_UNARY_PLUS:
        چپ = عبارت->unary.فرزند;
        أرجع عبارت_Resolve_Integer_Lookup(ctx, چپ, val_rtrn, pending, lookup,
                                        lookupPriv);

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 عبارت->common.type);
        اكسر;
    }

    أرجع false;
}

bool
عبارت_Resolve_Integer(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   int64_t *val_rtrn)
{
    أرجع عبارت_Resolve_Integer_Lookup(ctx, عبارت, val_rtrn, عدم, عدم, عدم);
}

تعداد xkb_parser_error
عبارت_Resolve_گروه(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت عبارت_Def *عبارت, bool absolute,
                 xkb_layout_شاخص_t *گروه_rtrn, bool *pending)
{
    ساكن ثابت Lookup_مدخل pending_گروه_شاخص_نامها[] = {
        { گروه_آخرین_شاخص_NAME, 0 },
        { عدم, 0 }
    };

    ثابت بنية named_integer_الگو گروه_name_الگو = {
        /* Prefix is title-cased, because it is also used in error messages */
        .پیشوند = "Group",
        .پیشوند_درازی = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_ماسک = false,
        .entries = keymap_info->lookup.گروه_شاخص_نامها,
        .pending_entries = pending_گروه_شاخص_نامها,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
    };

    int64_t نتیجه = 0;
    إذا (!عبارت_Resolve_Integer_Lookup(keymap_info->keymap.ctx, عبارت, &نتیجه, pending,
                                  Named_Integer_الگو_Lookup, &گروه_name_الگو))
        أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    إذا (نتیجه < (int64_t)absolute || نتیجه > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                نتیجه, absolute, keymap_info->features.max_groups);
        أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *گروه_rtrn = (xkb_layout_شاخص_t) نتیجه;
    أرجع PARSER_SUCCESS;
}

bool
عبارت_Resolve_Level(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                 xkb_level_شاخص_t *level_rtrn)
{
    ساكن ثابت بنية named_integer_الگو level_name_الگو = {
        /* Prefix is title-cased, because it is also used in error messages */
        .پیشوند = "Level",
        .پیشوند_درازی = حجم("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_ماسک = false,
        .entries = عدم,
        .error_id = XKB_ERROR_UNSUPPORTED_لغزش_LEVEL,
    };
    int64_t نتیجه = 0;
    إذا (!عبارت_Resolve_Integer_Lookup(ctx, عبارت, &نتیجه, عدم,
                                  Named_Integer_الگو_Lookup,
                                  &level_name_الگو))
        أرجع false;

    إذا (نتیجه < 1 || نتیجه > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_لغزش_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                نتیجه, XKB_LEVEL_MAX_IMPL);
        أرجع false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_شاخص_t) (نتیجه - 1);
    أرجع true;
}

bool
عبارت_Resolve_Button(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت, int64_t *btn_rtrn)
{
    أرجع عبارت_Resolve_Integer_Lookup(ctx, عبارت, btn_rtrn, عدم, SimpleLookup,
                                    button_نامها);
}

bool
عبارت_Resolve_String(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                  xkb_atom_t *val_rtrn)
{
    بدل (عبارت->common.type) {
    حالة STMT_عبارت_STRING_LITERAL:
        *val_rtrn = عبارت->string.str;
        أرجع true;

    حالة STMT_عبارت_INTEGER_LITERAL:
    حالة STMT_عبارت_FLOAT_LITERAL:
    حالة STMT_عبارت_BOOLEAN_LITERAL:
    حالة STMT_عبارت_KEYNAME_LITERAL:
    حالة STMT_عبارت_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Found %s, expected a string\n",
                stmt_type_مقصد_string(عبارت->common.type));
        أرجع false;

    حالة STMT_عبارت_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_متن(ctx, عبارت->ident.ident));
        أرجع false;

    حالة STMT_عبارت_فیلد_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_متن(ctx, عبارت->فیلد_ref.element),
                xkb_atom_متن(ctx, عبارت->فیلد_ref.فیلد_2));
        أرجع false;

    حالة STMT_عبارت_جمع_کن:
    حالة STMT_عبارت_تفریق:
    حالة STMT_عبارت_MULTIPLY:
    حالة STMT_عبارت_DIVIDE:
    حالة STMT_عبارت_تخصیص_بده:
    حالة STMT_عبارت_NEGATE:
    حالة STMT_عبارت_INVERT:
    حالة STMT_عبارت_NOT:
    حالة STMT_عبارت_UNARY_PLUS:
    حالة STMT_عبارت_خالی_LIST:
    حالة STMT_عبارت_ACTION_DECL:
    حالة STMT_عبارت_ACTION_LIST:
    حالة STMT_عبارت_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_مقصد_string(عبارت->common.type));
        أرجع false;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 عبارت->common.type);
        اكسر;
    }
    أرجع false;
}

bool
عبارت_Resolve_Enum(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                uint32_t *val_rtrn, ثابت Lookup_مدخل *مقادیر)
{
    إذا (عبارت->common.type != STMT_عبارت_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_مقصد_string(عبارت->common.type));
        أرجع false;
    }

    إذا (!SimpleLookup(ctx, مقادیر, عبارت->ident.ident, val_rtrn, عدم)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_متن(ctx, عبارت->ident.ident));
        طالما (مقادیر && مقادیر->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", مقادیر->name);
            مقادیر++;
        }
        أرجع false;
    }

    أرجع true;
}

ساكن bool
عبارت_Resolve_ماسک_Lookup(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ثابت فراغ *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    عبارت_Def *چپ, *راست;
    ثابت حرف *bogus = عدم;

    بدل (عبارت->common.type) {
    حالة STMT_عبارت_INTEGER_LITERAL:
        إذا (عبارت->integer.ival < 0 || عبارت->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_پیام_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    عبارت->integer.ival < 0 ? "-" : "",
                    imaxabs(عبارت->integer.ival),
                    UINT32_MAX);
            أرجع false;
        }
        *val_rtrn = (uint32_t) عبارت->integer.ival;
        أرجع true;

    حالة STMT_عبارت_STRING_LITERAL:
    حالة STMT_عبارت_FLOAT_LITERAL:
    حالة STMT_عبارت_BOOLEAN_LITERAL:
    حالة STMT_عبارت_KEYNAME_LITERAL:
    حالة STMT_عبارت_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_مقصد_string(عبارت->common.type));
        أرجع false;

    حالة STMT_عبارت_IDENT:
        ok = lookup(ctx, lookupPriv, عبارت->ident.ident, val_rtrn, pending);
        إذا (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_متن(ctx, عبارت->ident.ident));
        إذا (pending && *pending) {
            /* Postpone computation */
            أرجع false;
        }
        أرجع ok;

    حالة STMT_عبارت_فیلد_REF:
        log_err(ctx, XKB_ERROR_INVALID_عبارت_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_متن(ctx, عبارت->فیلد_ref.element),
                xkb_atom_متن(ctx, عبارت->فیلد_ref.فیلد_2));
        أرجع false;

    حالة STMT_عبارت_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    حالة STMT_عبارت_ACTION_DECL:
        إذا (bogus == عدم)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        أرجع false;

    حالة STMT_عبارت_جمع_کن:
    حالة STMT_عبارت_تفریق:
    حالة STMT_عبارت_MULTIPLY:
    حالة STMT_عبارت_DIVIDE:
        چپ = عبارت->دودویی.چپ;
        راست = عبارت->دودویی.راست;
        إذا (!عبارت_Resolve_ماسک_Lookup(ctx, چپ, &l, pending, lookup, lookupPriv) ||
            !عبارت_Resolve_ماسک_Lookup(ctx, راست, &r, pending, lookup, lookupPriv))
            أرجع false;

        بدل (عبارت->common.type) {
        حالة STMT_عبارت_جمع_کن:
            *val_rtrn = l | r;
            اكسر;
        حالة STMT_عبارت_تفریق:
            *val_rtrn = l & (~r);
            اكسر;
        حالة STMT_عبارت_MULTIPLY:
        حالة STMT_عبارت_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (عبارت->common.type == STMT_عبارت_DIVIDE ? "divide" : "multiply"));
            أرجع false;
        افتراضي:
            اكسر;
        }

        أرجع true;

    حالة STMT_عبارت_تخصیص_بده:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        اكسر;

    حالة STMT_عبارت_INVERT:
        چپ = عبارت->unary.فرزند;
        إذا (!عبارت_Resolve_Integer_Lookup(ctx, چپ, &v, pending, lookup, lookupPriv))
            أرجع false;
        إذا (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_پیام_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            أرجع false;
        }
        *val_rtrn = ~(uint32_t) v;
        أرجع true;

    حالة STMT_عبارت_UNARY_PLUS:
    حالة STMT_عبارت_NEGATE:
    حالة STMT_عبارت_NOT:
        چپ = عبارت->unary.فرزند;
        إذا (!عبارت_Resolve_Integer_Lookup(ctx, چپ, &v, pending, lookup, lookupPriv))
            أرجع false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_مقصد_operator_char(عبارت->common.type));
        أرجع false;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 عبارت->common.type);
        اكسر;
    }

    أرجع false;
}

bool
عبارت_Resolve_ماسک(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                uint32_t *ماسک_rtrn, ثابت Lookup_مدخل *مقادیر)
{
    أرجع عبارت_Resolve_ماسک_Lookup(ctx, عبارت, ماسک_rtrn, عدم, SimpleLookup, مقادیر);
}

bool
عبارت_Resolve_Mod_ماسک(بنية xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_مجموعه *mods,
                   xkb_mod_ماسک_t *ماسک_rtrn)
{
    Lookup_Mod_ماسک_Priv priv = { .mods = mods, .mod_type = mod_type };
    أرجع عبارت_Resolve_ماسک_Lookup(ctx, عبارت, ماسک_rtrn, عدم, Lookup_Mod_ماسک, &priv);
}

bool
عبارت_Resolve_Mod(بنية xkb_context *ctx, ثابت عبارت_Def *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_مجموعه *mods,
               xkb_mod_شاخص_t *ndx_rtrn)
{
    إذا (def->common.type != STMT_عبارت_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_مقصد_string(def->common.type));
        أرجع false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_شاخص_t ndx = Xkb_Mod_Name_مقصد_شاخص(mods, name, mod_type);
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
عبارت_Resolve_گروه_ماسک(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت عبارت_Def *عبارت, xkb_layout_ماسک_t *گروه_rtrn,
                     bool *pending_rtrn)
{
    ساكن ثابت Lookup_مدخل pending_گروه_ماسک_نامها[] = {
        { گروه_آخرین_شاخص_NAME, 0 },
        { عدم, 0 }
    };

    ثابت بنية named_integer_الگو گروه_name_الگو = {
        /* Prefix is title-cased, because it is also used in error messages */
        .پیشوند = "Group",
        .پیشوند_درازی = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_ماسک = true,
        .entries = keymap_info->lookup.گروه_ماسک_نامها,
        .pending_entries = pending_گروه_ماسک_نامها,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2
    };

    أرجع عبارت_Resolve_ماسک_Lookup(keymap_info->keymap.ctx, عبارت, گروه_rtrn,
                                 pending_rtrn, Named_Integer_الگو_Lookup,
                                 &گروه_name_الگو);
}
