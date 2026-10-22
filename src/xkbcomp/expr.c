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
                                xkb_atom_t ميدان_2, uint32_t *val_rtrn,
                                bool *pending);

bool
اظهار_Resolve_Lhs(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
               ثابت حرف **elem_rtrn, ثابت حرف **ميدان_rtrn,
               اظهار_Def **اشاريو_rtrn)
{
    بدل (اظهار->common.type) {
    حالة STMT_اظهار_IDENT:
        *elem_rtrn = عدم;
        *ميدان_rtrn = xkb_atom_متن(ctx, اظهار->ident.ident);
        *اشاريو_rtrn = عدم;
        أرجع (*ميدان_rtrn != عدم);
    حالة STMT_اظهار_ميدان_REF:
        *elem_rtrn = xkb_atom_متن(ctx, اظهار->ميدان_ref.element);
        *ميدان_rtrn = xkb_atom_متن(ctx, اظهار->ميدان_ref.ميدان_2);
        *اشاريو_rtrn = عدم;
        أرجع (*elem_rtrn != عدم && *ميدان_rtrn != عدم);
    حالة STMT_اظهار_ARRAY_REF:
        *elem_rtrn = xkb_atom_متن(ctx, اظهار->array_ref.element);
        *ميدان_rtrn = xkb_atom_متن(ctx, اظهار->array_ref.ميدان_2);
        *اشاريو_rtrn = اظهار->array_ref.داخلا;
        إذا (اظهار->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == عدم)
            أرجع false;
        إذا (*ميدان_rtrn == عدم)
            أرجع false;
        أرجع true;
    افتراضي:
        اكسر;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", اظهار->common.type);
    أرجع false;
}

ساكن bool
SimpleLookup(بنية xkb_context *ctx, ثابت فراغ *priv, xkb_atom_t ميدان_2,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    إذا (!priv || ميدان_2 == XKB_ATOM_NONE)
        أرجع false;

    ثابت حرف *str = xkb_atom_متن(ctx, ميدان_2);
    لكل (ثابت Lookup_داخلا *داخلا = priv; داخلا && داخلا->name; داخلا++) {
        إذا (istreq(str, داخلا->name)) {
            *val_rtrn = داخلا->قدر;
            أرجع true;
        }
    }

    أرجع false;
}

بنية named_integer_نمونو {
    ثابت حرف *اڳياڙي;
    سائيز_t اڳياڙي_ڊيگهائي;
    uint32_t min;
    uint32_t max;
    ثابت Lookup_داخلا *entries;
    ثابت Lookup_داخلا *pending_entries;
    bool is_نقاب;
    تعداد xkb_پيغام_ڪوڊ error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ساكن bool
Named_Integer_نمونو_Lookup(بنية xkb_context *ctx, ثابت فراغ *priv,
                          xkb_atom_t ميدان_2, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    إذا (!priv || ميدان_2 == XKB_ATOM_NONE)
        أرجع false;

    ثابت حرف * ثابت str = xkb_atom_متن(ctx, ميدان_2);
    ثابت بنية named_integer_نمونو * ثابت نمونو = priv;

    ثابت صحيح ڳڻپ = (istrneq(str, نمونو->اڳياڙي, نمونو->اڳياڙي_ڊيگهائي))
        ? parse_dec_منزل_uint32_t(str + نمونو->اڳياڙي_ڊيگهائي, SIZE_MAX, val_rtrn)
        : 0;

    إذا (ڳڻپ > 0 && *(str + نمونو->اڳياڙي_ڊيگهائي + ڳڻپ) == '\0') {
        إذا (*val_rtrn < نمونو->min || *val_rtrn > نمونو->max) {
            log_err(ctx, نمونو->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    نمونو->اڳياڙي, *val_rtrn, نمونو->min, نمونو->max);
            أرجع false;
        }
        إذا (نمونو->is_نقاب) {
            /* Compute mask (bit 0 = min index) */
            أكد(*val_rtrn - نمونو->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - نمونو->min);
        }
        أرجع true;
    } وإلا {
        إذا (نمونو->entries &&
            SimpleLookup(ctx, نمونو->entries, ميدان_2, val_rtrn, عدم)) {
            أرجع true;
        }
        إذا (نمونو->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, نمونو->pending_entries, ميدان_2, val_rtrn, عدم)) {
            *pending_rtrn = true;
            أرجع true;
        }
        أرجع false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
تعريفنوع بنية {
    ثابت بنية xkb_mod_سيٽ *mods;
    تعداد mod_type mod_type;
} Lookup_Mod_نقاب_Priv;

ساكن bool
Lookup_Mod_نقاب(بنية xkb_context *ctx, ثابت فراغ *priv, xkb_atom_t ميدان_2,
              xkb_mod_نقاب_t *val_rtrn, bool *pending_rtrn)
{
    ثابت حرف *str = xkb_atom_متن(ctx, ميدان_2);
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
    ثابت بنية xkb_mod_سيٽ *mods = arg->mods;
    ثابت تعداد mod_type mod_type = arg->mod_type;
    ثابت xkb_mod_اشاريو_t ndx = Xkb_Mod_Name_منزل_اشاريو(mods, ميدان_2, mod_type);
    إذا (ndx == XKB_MOD_INVALID)
        أرجع false;

    *val_rtrn = (UINT32_C(1) << ndx);
    أرجع true;
}

bool
اظهار_Resolve_Boolean(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                   bool *سيٽ_rtrn)
{
    bool ok = false;
    ثابت حرف *ident;

    بدل (اظهار->common.type) {
    حالة STMT_اظهار_BOOLEAN_LITERAL:
        *سيٽ_rtrn = اظهار->boolean.سيٽ;
        أرجع true;

    حالة STMT_اظهار_STRING_LITERAL:
    حالة STMT_اظهار_INTEGER_LITERAL:
    حالة STMT_اظهار_FLOAT_LITERAL:
    حالة STMT_اظهار_KEYNAME_LITERAL:
    حالة STMT_اظهار_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_منزل_string(اظهار->common.type));
        أرجع false;

    حالة STMT_اظهار_IDENT:
        ident = xkb_atom_متن(ctx, اظهار->ident.ident);
        إذا (ident) {
            إذا (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *سيٽ_rtrn = true;
                أرجع true;
            }
            وإلا إذا (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *سيٽ_rtrn = false;
                أرجع true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        أرجع false;

    حالة STMT_اظهار_ميدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_اظهار_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_متن(ctx, اظهار->ميدان_ref.element),
                xkb_atom_متن(ctx, اظهار->ميدان_ref.ميدان_2));
        أرجع false;

    حالة STMT_اظهار_INVERT:
    حالة STMT_اظهار_NOT:
        ok = اظهار_Resolve_Boolean(ctx, اظهار->unary.ٻار, سيٽ_rtrn);
        إذا (ok)
            *سيٽ_rtrn = !*سيٽ_rtrn;
        أرجع ok;
    حالة STMT_اظهار_جوڙيو:
    حالة STMT_اظهار_ڪٽيو:
    حالة STMT_اظهار_MULTIPLY:
    حالة STMT_اظهار_DIVIDE:
    حالة STMT_اظهار_سونپيو:
    حالة STMT_اظهار_NEGATE:
    حالة STMT_اظهار_UNARY_PLUS:
    حالة STMT_اظهار_خالي_جايون_LIST:
    حالة STMT_اظهار_ACTION_DECL:
    حالة STMT_اظهار_ACTION_LIST:
    حالة STMT_اظهار_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_منزل_string(اظهار->common.type));
        اكسر;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 اظهار->common.type);
        اكسر;
    }

    أرجع false;
}

ساكن bool
اظهار_Resolve_Integer_Lookup(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ثابت فراغ *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    اظهار_Def *کاٻو, *ساڄو;

    بدل (اظهار->common.type) {
    حالة STMT_اظهار_INTEGER_LITERAL:
        *val_rtrn = اظهار->integer.ival;
        أرجع true;

    حالة STMT_اظهار_STRING_LITERAL:
    حالة STMT_اظهار_FLOAT_LITERAL:
    حالة STMT_اظهار_BOOLEAN_LITERAL:
    حالة STMT_اظهار_KEYNAME_LITERAL:
    حالة STMT_اظهار_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_منزل_string(اظهار->common.type));
        أرجع false;

    حالة STMT_اظهار_IDENT:
        إذا (lookup)
            ok = lookup(ctx, lookupPriv, اظهار->ident.ident, &u, pending);

        إذا (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_متن(ctx, اظهار->ident.ident));
        وإلا
            *val_rtrn = (int64_t) u;

        إذا (pending && *pending) {
            /* Postpone computation */
            أرجع false;
        }

        أرجع ok;

    حالة STMT_اظهار_ميدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_اظهار_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_متن(ctx, اظهار->ميدان_ref.element),
                xkb_atom_متن(ctx, اظهار->ميدان_ref.ميدان_2));
        أرجع false;

    حالة STMT_اظهار_جوڙيو:
    حالة STMT_اظهار_ڪٽيو:
    حالة STMT_اظهار_MULTIPLY:
    حالة STMT_اظهار_DIVIDE:
        کاٻو = اظهار->ٻٽو.کاٻو;
        ساڄو = اظهار->ٻٽو.ساڄو;
        إذا (!اظهار_Resolve_Integer_Lookup(ctx, کاٻو, &l, pending, lookup, lookupPriv) ||
            !اظهار_Resolve_Integer_Lookup(ctx, ساڄو, &r, pending, lookup, lookupPriv))
            أرجع false;

        بدل (اظهار->common.type) {
        حالة STMT_اظهار_جوڙيو:
            إذا (ckd_جوڙيو(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_حد_کان_مٿي,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_اظهار_ڪٽيو:
            إذا (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_حد_کان_مٿي,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_اظهار_MULTIPLY:
            إذا (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_حد_کان_مٿي,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_اظهار_DIVIDE:
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
                    stmt_type_منزل_string(اظهار->common.type));
            أرجع false;
        }

        أرجع true;

    حالة STMT_اظهار_سونپيو:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        اكسر;

    حالة STMT_اظهار_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        أرجع false;

    حالة STMT_اظهار_INVERT:
    حالة STMT_اظهار_NEGATE:
        کاٻو = اظهار->unary.ٻار;
        إذا (!اظهار_Resolve_Integer_Lookup(ctx, کاٻو, &l, pending, lookup, lookupPriv))
            أرجع false;

        *val_rtrn = (اظهار->common.type == STMT_اظهار_NEGATE ? -l : ~l);
        أرجع true;

    حالة STMT_اظهار_UNARY_PLUS:
        کاٻو = اظهار->unary.ٻار;
        أرجع اظهار_Resolve_Integer_Lookup(ctx, کاٻو, val_rtrn, pending, lookup,
                                        lookupPriv);

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 اظهار->common.type);
        اكسر;
    }

    أرجع false;
}

bool
اظهار_Resolve_Integer(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                   int64_t *val_rtrn)
{
    أرجع اظهار_Resolve_Integer_Lookup(ctx, اظهار, val_rtrn, عدم, عدم, عدم);
}

تعداد xkb_parser_error
اظهار_Resolve_گروهه(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت اظهار_Def *اظهار, bool absolute,
                 xkb_layout_اشاريو_t *گروهه_rtrn, bool *pending)
{
    ساكن ثابت Lookup_داخلا pending_گروهه_اشاريو_نالا[] = {
        { گروهه_آخري_اشاريو_NAME, 0 },
        { عدم, 0 }
    };

    ثابت بنية named_integer_نمونو گروهه_name_نمونو = {
        /* Prefix is title-cased, because it is also used in error messages */
        .اڳياڙي = "Group",
        .اڳياڙي_ڊيگهائي = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_نقاب = false,
        .entries = keymap_info->lookup.گروهه_اشاريو_نالا,
        .pending_entries = pending_گروهه_اشاريو_نالا,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
    };

    int64_t نتيجو = 0;
    إذا (!اظهار_Resolve_Integer_Lookup(keymap_info->keymap.ctx, اظهار, &نتيجو, pending,
                                  Named_Integer_نمونو_Lookup, &گروهه_name_نمونو))
        أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    إذا (نتيجو < (int64_t)absolute || نتيجو > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                نتيجو, absolute, keymap_info->features.max_groups);
        أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *گروهه_rtrn = (xkb_layout_اشاريو_t) نتيجو;
    أرجع PARSER_SUCCESS;
}

bool
اظهار_Resolve_Level(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                 xkb_level_اشاريو_t *level_rtrn)
{
    ساكن ثابت بنية named_integer_نمونو level_name_نمونو = {
        /* Prefix is title-cased, because it is also used in error messages */
        .اڳياڙي = "Level",
        .اڳياڙي_ڊيگهائي = حجم("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_نقاب = false,
        .entries = عدم,
        .error_id = XKB_ERROR_UNSUPPORTED_سرڪايو_LEVEL,
    };
    int64_t نتيجو = 0;
    إذا (!اظهار_Resolve_Integer_Lookup(ctx, اظهار, &نتيجو, عدم,
                                  Named_Integer_نمونو_Lookup,
                                  &level_name_نمونو))
        أرجع false;

    إذا (نتيجو < 1 || نتيجو > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_سرڪايو_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                نتيجو, XKB_LEVEL_MAX_IMPL);
        أرجع false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_اشاريو_t) (نتيجو - 1);
    أرجع true;
}

bool
اظهار_Resolve_Button(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار, int64_t *btn_rtrn)
{
    أرجع اظهار_Resolve_Integer_Lookup(ctx, اظهار, btn_rtrn, عدم, SimpleLookup,
                                    button_نالا);
}

bool
اظهار_Resolve_String(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                  xkb_atom_t *val_rtrn)
{
    بدل (اظهار->common.type) {
    حالة STMT_اظهار_STRING_LITERAL:
        *val_rtrn = اظهار->string.str;
        أرجع true;

    حالة STMT_اظهار_INTEGER_LITERAL:
    حالة STMT_اظهار_FLOAT_LITERAL:
    حالة STMT_اظهار_BOOLEAN_LITERAL:
    حالة STMT_اظهار_KEYNAME_LITERAL:
    حالة STMT_اظهار_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Found %s, expected a string\n",
                stmt_type_منزل_string(اظهار->common.type));
        أرجع false;

    حالة STMT_اظهار_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_متن(ctx, اظهار->ident.ident));
        أرجع false;

    حالة STMT_اظهار_ميدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_اظهار_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_متن(ctx, اظهار->ميدان_ref.element),
                xkb_atom_متن(ctx, اظهار->ميدان_ref.ميدان_2));
        أرجع false;

    حالة STMT_اظهار_جوڙيو:
    حالة STMT_اظهار_ڪٽيو:
    حالة STMT_اظهار_MULTIPLY:
    حالة STMT_اظهار_DIVIDE:
    حالة STMT_اظهار_سونپيو:
    حالة STMT_اظهار_NEGATE:
    حالة STMT_اظهار_INVERT:
    حالة STMT_اظهار_NOT:
    حالة STMT_اظهار_UNARY_PLUS:
    حالة STMT_اظهار_خالي_جايون_LIST:
    حالة STMT_اظهار_ACTION_DECL:
    حالة STMT_اظهار_ACTION_LIST:
    حالة STMT_اظهار_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_منزل_string(اظهار->common.type));
        أرجع false;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 اظهار->common.type);
        اكسر;
    }
    أرجع false;
}

bool
اظهار_Resolve_Enum(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                uint32_t *val_rtrn, ثابت Lookup_داخلا *قدرون)
{
    إذا (اظهار->common.type != STMT_اظهار_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_منزل_string(اظهار->common.type));
        أرجع false;
    }

    إذا (!SimpleLookup(ctx, قدرون, اظهار->ident.ident, val_rtrn, عدم)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_متن(ctx, اظهار->ident.ident));
        طالما (قدرون && قدرون->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", قدرون->name);
            قدرون++;
        }
        أرجع false;
    }

    أرجع true;
}

ساكن bool
اظهار_Resolve_نقاب_Lookup(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ثابت فراغ *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    اظهار_Def *کاٻو, *ساڄو;
    ثابت حرف *bogus = عدم;

    بدل (اظهار->common.type) {
    حالة STMT_اظهار_INTEGER_LITERAL:
        إذا (اظهار->integer.ival < 0 || اظهار->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    اظهار->integer.ival < 0 ? "-" : "",
                    imaxabs(اظهار->integer.ival),
                    UINT32_MAX);
            أرجع false;
        }
        *val_rtrn = (uint32_t) اظهار->integer.ival;
        أرجع true;

    حالة STMT_اظهار_STRING_LITERAL:
    حالة STMT_اظهار_FLOAT_LITERAL:
    حالة STMT_اظهار_BOOLEAN_LITERAL:
    حالة STMT_اظهار_KEYNAME_LITERAL:
    حالة STMT_اظهار_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_منزل_string(اظهار->common.type));
        أرجع false;

    حالة STMT_اظهار_IDENT:
        ok = lookup(ctx, lookupPriv, اظهار->ident.ident, val_rtrn, pending);
        إذا (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_متن(ctx, اظهار->ident.ident));
        إذا (pending && *pending) {
            /* Postpone computation */
            أرجع false;
        }
        أرجع ok;

    حالة STMT_اظهار_ميدان_REF:
        log_err(ctx, XKB_ERROR_INVALID_اظهار_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_متن(ctx, اظهار->ميدان_ref.element),
                xkb_atom_متن(ctx, اظهار->ميدان_ref.ميدان_2));
        أرجع false;

    حالة STMT_اظهار_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    حالة STMT_اظهار_ACTION_DECL:
        إذا (bogus == عدم)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        أرجع false;

    حالة STMT_اظهار_جوڙيو:
    حالة STMT_اظهار_ڪٽيو:
    حالة STMT_اظهار_MULTIPLY:
    حالة STMT_اظهار_DIVIDE:
        کاٻو = اظهار->ٻٽو.کاٻو;
        ساڄو = اظهار->ٻٽو.ساڄو;
        إذا (!اظهار_Resolve_نقاب_Lookup(ctx, کاٻو, &l, pending, lookup, lookupPriv) ||
            !اظهار_Resolve_نقاب_Lookup(ctx, ساڄو, &r, pending, lookup, lookupPriv))
            أرجع false;

        بدل (اظهار->common.type) {
        حالة STMT_اظهار_جوڙيو:
            *val_rtrn = l | r;
            اكسر;
        حالة STMT_اظهار_ڪٽيو:
            *val_rtrn = l & (~r);
            اكسر;
        حالة STMT_اظهار_MULTIPLY:
        حالة STMT_اظهار_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (اظهار->common.type == STMT_اظهار_DIVIDE ? "divide" : "multiply"));
            أرجع false;
        افتراضي:
            اكسر;
        }

        أرجع true;

    حالة STMT_اظهار_سونپيو:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        اكسر;

    حالة STMT_اظهار_INVERT:
        کاٻو = اظهار->unary.ٻار;
        إذا (!اظهار_Resolve_Integer_Lookup(ctx, کاٻو, &v, pending, lookup, lookupPriv))
            أرجع false;
        إذا (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            أرجع false;
        }
        *val_rtrn = ~(uint32_t) v;
        أرجع true;

    حالة STMT_اظهار_UNARY_PLUS:
    حالة STMT_اظهار_NEGATE:
    حالة STMT_اظهار_NOT:
        کاٻو = اظهار->unary.ٻار;
        إذا (!اظهار_Resolve_Integer_Lookup(ctx, کاٻو, &v, pending, lookup, lookupPriv))
            أرجع false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_منزل_operator_char(اظهار->common.type));
        أرجع false;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 اظهار->common.type);
        اكسر;
    }

    أرجع false;
}

bool
اظهار_Resolve_نقاب(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                uint32_t *نقاب_rtrn, ثابت Lookup_داخلا *قدرون)
{
    أرجع اظهار_Resolve_نقاب_Lookup(ctx, اظهار, نقاب_rtrn, عدم, SimpleLookup, قدرون);
}

bool
اظهار_Resolve_Mod_نقاب(بنية xkb_context *ctx, ثابت اظهار_Def *اظهار,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_سيٽ *mods,
                   xkb_mod_نقاب_t *نقاب_rtrn)
{
    Lookup_Mod_نقاب_Priv priv = { .mods = mods, .mod_type = mod_type };
    أرجع اظهار_Resolve_نقاب_Lookup(ctx, اظهار, نقاب_rtrn, عدم, Lookup_Mod_نقاب, &priv);
}

bool
اظهار_Resolve_Mod(بنية xkb_context *ctx, ثابت اظهار_Def *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_سيٽ *mods,
               xkb_mod_اشاريو_t *ndx_rtrn)
{
    إذا (def->common.type != STMT_اظهار_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_منزل_string(def->common.type));
        أرجع false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_اشاريو_t ndx = Xkb_Mod_Name_منزل_اشاريو(mods, name, mod_type);
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
اظهار_Resolve_گروهه_نقاب(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت اظهار_Def *اظهار, xkb_layout_نقاب_t *گروهه_rtrn,
                     bool *pending_rtrn)
{
    ساكن ثابت Lookup_داخلا pending_گروهه_نقاب_نالا[] = {
        { گروهه_آخري_اشاريو_NAME, 0 },
        { عدم, 0 }
    };

    ثابت بنية named_integer_نمونو گروهه_name_نمونو = {
        /* Prefix is title-cased, because it is also used in error messages */
        .اڳياڙي = "Group",
        .اڳياڙي_ڊيگهائي = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_نقاب = true,
        .entries = keymap_info->lookup.گروهه_نقاب_نالا,
        .pending_entries = pending_گروهه_نقاب_نالا,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2
    };

    أرجع اظهار_Resolve_نقاب_Lookup(keymap_info->keymap.ctx, اظهار, گروهه_rtrn,
                                 pending_rtrn, Named_Integer_نمونو_Lookup,
                                 &گروهه_name_نمونو);
}
