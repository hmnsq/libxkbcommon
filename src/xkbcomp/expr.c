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

تعریفنوع bool (*IdentLookupFunc)(ساختار xkb_context *ctx, ثابت تهی *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
عبارت_Resolve_Lhs(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
               ثابت نویسه **elem_rtrn, ثابت نویسه **field_rtrn,
               عبارت_Def **index_rtrn)
{
    انتخاب (عبارت->common.type) {
    حالت STMT_عبارت_IDENT:
        *elem_rtrn = پوچ;
        *field_rtrn = xkb_atom_text(ctx, عبارت->ident.ident);
        *index_rtrn = پوچ;
        بازگشت (*field_rtrn != پوچ);
    حالت STMT_عبارت_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, عبارت->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, عبارت->field_ref.field);
        *index_rtrn = پوچ;
        بازگشت (*elem_rtrn != پوچ && *field_rtrn != پوچ);
    حالت STMT_عبارت_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, عبارت->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, عبارت->array_ref.field);
        *index_rtrn = عبارت->array_ref.entry;
        اگر (عبارت->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == پوچ)
            بازگشت false;
        اگر (*field_rtrn == پوچ)
            بازگشت false;
        بازگشت true;
    پیشفرض:
        بشکن;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", عبارت->common.type);
    بازگشت false;
}

ایستا bool
SimpleLookup(ساختار xkb_context *ctx, ثابت تهی *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    اگر (!priv || field == XKB_ATOM_NONE)
        بازگشت false;

    ثابت نویسه *str = xkb_atom_text(ctx, field);
    برای (ثابت LookupEntry *entry = priv; entry && entry->name; entry++) {
        اگر (istreq(str, entry->name)) {
            *val_rtrn = entry->مقدار;
            بازگشت true;
        }
    }

    بازگشت false;
}

ساختار named_integer_pattern {
    ثابت نویسه *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    ثابت LookupEntry *entries;
    ثابت LookupEntry *pending_entries;
    bool is_mask;
    شمارش xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ایستا bool
NamedIntegerPatternLookup(ساختار xkb_context *ctx, ثابت تهی *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    اگر (!priv || field == XKB_ATOM_NONE)
        بازگشت false;

    ثابت نویسه * ثابت str = xkb_atom_text(ctx, field);
    ثابت ساختار named_integer_pattern * ثابت pattern = priv;

    ثابت صحیح شمار = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    اگر (شمار > 0 && *(str + pattern->prefix_length + شمار) == '\0') {
        اگر (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            بازگشت false;
        }
        اگر (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            أكد(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        بازگشت true;
    } وگرنه {
        اگر (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, پوچ)) {
            بازگشت true;
        }
        اگر (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, پوچ)) {
            *pending_rtrn = true;
            بازگشت true;
        }
        بازگشت false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
تعریفنوع ساختار {
    ثابت ساختار xkb_mod_set *mods;
    شمارش mod_type mod_type;
} LookupModMaskPriv;

ایستا bool
LookupModMask(ساختار xkb_context *ctx, ثابت تهی *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    ثابت نویسه *str = xkb_atom_text(ctx, field);
    اگر (!str)
        بازگشت false;

    اگر (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        بازگشت true;
    }

    اگر (istreq(str, "none")) {
        *val_rtrn = 0;
        بازگشت true;
    }

    ثابت LookupModMaskPriv *arg = priv;
    ثابت ساختار xkb_mod_set *mods = arg->mods;
    ثابت شمارش mod_type mod_type = arg->mod_type;
    ثابت xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    اگر (ndx == XKB_MOD_INVALID)
        بازگشت false;

    *val_rtrn = (UINT32_C(1) << ndx);
    بازگشت true;
}

bool
عبارت_Resolve_Boolean(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   bool *set_rtrn)
{
    bool ok = false;
    ثابت نویسه *ident;

    انتخاب (عبارت->common.type) {
    حالت STMT_عبارت_BOOLEAN_LITERAL:
        *set_rtrn = عبارت->boolean.set;
        بازگشت true;

    حالت STMT_عبارت_STRING_LITERAL:
    حالت STMT_عبارت_INTEGER_LITERAL:
    حالت STMT_عبارت_FLOAT_LITERAL:
    حالت STMT_عبارت_KEYNAME_LITERAL:
    حالت STMT_عبارت_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(عبارت->common.type));
        بازگشت false;

    حالت STMT_عبارت_IDENT:
        ident = xkb_atom_text(ctx, عبارت->ident.ident);
        اگر (ident) {
            اگر (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                بازگشت true;
            }
            وگرنه اگر (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                بازگشت true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        بازگشت false;

    حالت STMT_عبارت_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, عبارت->field_ref.element),
                xkb_atom_text(ctx, عبارت->field_ref.field));
        بازگشت false;

    حالت STMT_عبارت_INVERT:
    حالت STMT_عبارت_NOT:
        ok = عبارت_Resolve_Boolean(ctx, عبارت->unary.child, set_rtrn);
        اگر (ok)
            *set_rtrn = !*set_rtrn;
        بازگشت ok;
    حالت STMT_عبارت_ADD:
    حالت STMT_عبارت_SUBTRACT:
    حالت STMT_عبارت_MULTIPLY:
    حالت STMT_عبارت_DIVIDE:
    حالت STMT_عبارت_ASSIGN:
    حالت STMT_عبارت_NEGATE:
    حالت STMT_عبارت_UNARY_PLUS:
    حالت STMT_عبارت_EMPTY_LIST:
    حالت STMT_عبارت_ACTION_DECL:
    حالت STMT_عبارت_ACTION_LIST:
    حالت STMT_عبارت_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(عبارت->common.type));
        بشکن;

    پیشفرض:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 عبارت->common.type);
        بشکن;
    }

    بازگشت false;
}

ایستا bool
عبارت_Resolve_Integer_Lookup(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ثابت تهی *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    عبارت_Def *چپ, *راست;

    انتخاب (عبارت->common.type) {
    حالت STMT_عبارت_INTEGER_LITERAL:
        *val_rtrn = عبارت->integer.ival;
        بازگشت true;

    حالت STMT_عبارت_STRING_LITERAL:
    حالت STMT_عبارت_FLOAT_LITERAL:
    حالت STMT_عبارت_BOOLEAN_LITERAL:
    حالت STMT_عبارت_KEYNAME_LITERAL:
    حالت STMT_عبارت_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(عبارت->common.type));
        بازگشت false;

    حالت STMT_عبارت_IDENT:
        اگر (lookup)
            ok = lookup(ctx, lookupPriv, عبارت->ident.ident, &u, pending);

        اگر (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, عبارت->ident.ident));
        وگرنه
            *val_rtrn = (int64_t) u;

        اگر (pending && *pending) {
            /* Postpone computation */
            بازگشت false;
        }

        بازگشت ok;

    حالت STMT_عبارت_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, عبارت->field_ref.element),
                xkb_atom_text(ctx, عبارت->field_ref.field));
        بازگشت false;

    حالت STMT_عبارت_ADD:
    حالت STMT_عبارت_SUBTRACT:
    حالت STMT_عبارت_MULTIPLY:
    حالت STMT_عبارت_DIVIDE:
        چپ = عبارت->دودویی.چپ;
        راست = عبارت->دودویی.راست;
        اگر (!عبارت_Resolve_Integer_Lookup(ctx, چپ, &l, pending, lookup, lookupPriv) ||
            !عبارت_Resolve_Integer_Lookup(ctx, راست, &r, pending, lookup, lookupPriv))
            بازگشت false;

        انتخاب (عبارت->common.type) {
        حالت STMT_عبارت_ADD:
            اگر (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                بازگشت false;
            }
            بشکن;
        حالت STMT_عبارت_SUBTRACT:
            اگر (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                بازگشت false;
            }
            بشکن;
        حالت STMT_عبارت_MULTIPLY:
            اگر (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                بازگشت false;
            }
            بشکن;
        حالت STMT_عبارت_DIVIDE:
            اگر (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                بازگشت false;
            }
            *val_rtrn = l / r;
            بشکن;
        پیشفرض:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(عبارت->common.type));
            بازگشت false;
        }

        بازگشت true;

    حالت STMT_عبارت_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        بشکن;

    حالت STMT_عبارت_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        بازگشت false;

    حالت STMT_عبارت_INVERT:
    حالت STMT_عبارت_NEGATE:
        چپ = عبارت->unary.child;
        اگر (!عبارت_Resolve_Integer_Lookup(ctx, چپ, &l, pending, lookup, lookupPriv))
            بازگشت false;

        *val_rtrn = (عبارت->common.type == STMT_عبارت_NEGATE ? -l : ~l);
        بازگشت true;

    حالت STMT_عبارت_UNARY_PLUS:
        چپ = عبارت->unary.child;
        بازگشت عبارت_Resolve_Integer_Lookup(ctx, چپ, val_rtrn, pending, lookup,
                                        lookupPriv);

    پیشفرض:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 عبارت->common.type);
        بشکن;
    }

    بازگشت false;
}

bool
عبارت_Resolve_Integer(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   int64_t *val_rtrn)
{
    بازگشت عبارت_Resolve_Integer_Lookup(ctx, عبارت, val_rtrn, پوچ, پوچ, پوچ);
}

شمارش xkb_parser_error
عبارت_Resolve_Group(ثابت ساختار xkb_keymap_info *keymap_info,
                 ثابت عبارت_Def *عبارت, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    ایستا ثابت LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { پوچ, 0 }
    };

    ثابت ساختار named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = اندازه("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    اگر (!عبارت_Resolve_Integer_Lookup(keymap_info->keymap.ctx, عبارت, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        بازگشت (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    اگر (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        بازگشت (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    بازگشت PARSER_SUCCESS;
}

bool
عبارت_Resolve_Level(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                 xkb_level_index_t *level_rtrn)
{
    ایستا ثابت ساختار named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = اندازه("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = پوچ,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    اگر (!عبارت_Resolve_Integer_Lookup(ctx, عبارت, &result, پوچ,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        بازگشت false;

    اگر (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        بازگشت false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    بازگشت true;
}

bool
عبارت_Resolve_Button(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت, int64_t *btn_rtrn)
{
    بازگشت عبارت_Resolve_Integer_Lookup(ctx, عبارت, btn_rtrn, پوچ, SimpleLookup,
                                    buttonNames);
}

bool
عبارت_Resolve_String(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                  xkb_atom_t *val_rtrn)
{
    انتخاب (عبارت->common.type) {
    حالت STMT_عبارت_STRING_LITERAL:
        *val_rtrn = عبارت->string.str;
        بازگشت true;

    حالت STMT_عبارت_INTEGER_LITERAL:
    حالت STMT_عبارت_FLOAT_LITERAL:
    حالت STMT_عبارت_BOOLEAN_LITERAL:
    حالت STMT_عبارت_KEYNAME_LITERAL:
    حالت STMT_عبارت_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(عبارت->common.type));
        بازگشت false;

    حالت STMT_عبارت_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, عبارت->ident.ident));
        بازگشت false;

    حالت STMT_عبارت_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, عبارت->field_ref.element),
                xkb_atom_text(ctx, عبارت->field_ref.field));
        بازگشت false;

    حالت STMT_عبارت_ADD:
    حالت STMT_عبارت_SUBTRACT:
    حالت STMT_عبارت_MULTIPLY:
    حالت STMT_عبارت_DIVIDE:
    حالت STMT_عبارت_ASSIGN:
    حالت STMT_عبارت_NEGATE:
    حالت STMT_عبارت_INVERT:
    حالت STMT_عبارت_NOT:
    حالت STMT_عبارت_UNARY_PLUS:
    حالت STMT_عبارت_EMPTY_LIST:
    حالت STMT_عبارت_ACTION_DECL:
    حالت STMT_عبارت_ACTION_LIST:
    حالت STMT_عبارت_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(عبارت->common.type));
        بازگشت false;

    پیشفرض:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 عبارت->common.type);
        بشکن;
    }
    بازگشت false;
}

bool
عبارت_Resolve_Enum(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                uint32_t *val_rtrn, ثابت LookupEntry *مقادیر)
{
    اگر (عبارت->common.type != STMT_عبارت_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(عبارت->common.type));
        بازگشت false;
    }

    اگر (!SimpleLookup(ctx, مقادیر, عبارت->ident.ident, val_rtrn, پوچ)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, عبارت->ident.ident));
        وقتی (مقادیر && مقادیر->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", مقادیر->name);
            مقادیر++;
        }
        بازگشت false;
    }

    بازگشت true;
}

ایستا bool
عبارت_Resolve_Mask_Lookup(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ثابت تهی *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    عبارت_Def *چپ, *راست;
    ثابت نویسه *bogus = پوچ;

    انتخاب (عبارت->common.type) {
    حالت STMT_عبارت_INTEGER_LITERAL:
        اگر (عبارت->integer.ival < 0 || عبارت->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    عبارت->integer.ival < 0 ? "-" : "",
                    imaxabs(عبارت->integer.ival),
                    UINT32_MAX);
            بازگشت false;
        }
        *val_rtrn = (uint32_t) عبارت->integer.ival;
        بازگشت true;

    حالت STMT_عبارت_STRING_LITERAL:
    حالت STMT_عبارت_FLOAT_LITERAL:
    حالت STMT_عبارت_BOOLEAN_LITERAL:
    حالت STMT_عبارت_KEYNAME_LITERAL:
    حالت STMT_عبارت_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(عبارت->common.type));
        بازگشت false;

    حالت STMT_عبارت_IDENT:
        ok = lookup(ctx, lookupPriv, عبارت->ident.ident, val_rtrn, pending);
        اگر (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, عبارت->ident.ident));
        اگر (pending && *pending) {
            /* Postpone computation */
            بازگشت false;
        }
        بازگشت ok;

    حالت STMT_عبارت_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, عبارت->field_ref.element),
                xkb_atom_text(ctx, عبارت->field_ref.field));
        بازگشت false;

    حالت STMT_عبارت_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    حالت STMT_عبارت_ACTION_DECL:
        اگر (bogus == پوچ)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        بازگشت false;

    حالت STMT_عبارت_ADD:
    حالت STMT_عبارت_SUBTRACT:
    حالت STMT_عبارت_MULTIPLY:
    حالت STMT_عبارت_DIVIDE:
        چپ = عبارت->دودویی.چپ;
        راست = عبارت->دودویی.راست;
        اگر (!عبارت_Resolve_Mask_Lookup(ctx, چپ, &l, pending, lookup, lookupPriv) ||
            !عبارت_Resolve_Mask_Lookup(ctx, راست, &r, pending, lookup, lookupPriv))
            بازگشت false;

        انتخاب (عبارت->common.type) {
        حالت STMT_عبارت_ADD:
            *val_rtrn = l | r;
            بشکن;
        حالت STMT_عبارت_SUBTRACT:
            *val_rtrn = l & (~r);
            بشکن;
        حالت STMT_عبارت_MULTIPLY:
        حالت STMT_عبارت_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (عبارت->common.type == STMT_عبارت_DIVIDE ? "divide" : "multiply"));
            بازگشت false;
        پیشفرض:
            بشکن;
        }

        بازگشت true;

    حالت STMT_عبارت_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        بشکن;

    حالت STMT_عبارت_INVERT:
        چپ = عبارت->unary.child;
        اگر (!عبارت_Resolve_Integer_Lookup(ctx, چپ, &v, pending, lookup, lookupPriv))
            بازگشت false;
        اگر (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            بازگشت false;
        }
        *val_rtrn = ~(uint32_t) v;
        بازگشت true;

    حالت STMT_عبارت_UNARY_PLUS:
    حالت STMT_عبارت_NEGATE:
    حالت STMT_عبارت_NOT:
        چپ = عبارت->unary.child;
        اگر (!عبارت_Resolve_Integer_Lookup(ctx, چپ, &v, pending, lookup, lookupPriv))
            بازگشت false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(عبارت->common.type));
        بازگشت false;

    پیشفرض:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 عبارت->common.type);
        بشکن;
    }

    بازگشت false;
}

bool
عبارت_Resolve_Mask(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                uint32_t *mask_rtrn, ثابت LookupEntry *مقادیر)
{
    بازگشت عبارت_Resolve_Mask_Lookup(ctx, عبارت, mask_rtrn, پوچ, SimpleLookup, مقادیر);
}

bool
عبارت_Resolve_Mod_Mask(ساختار xkb_context *ctx, ثابت عبارت_Def *عبارت,
                   شمارش mod_type mod_type, ثابت ساختار xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    بازگشت عبارت_Resolve_Mask_Lookup(ctx, عبارت, mask_rtrn, پوچ, LookupModMask, &priv);
}

bool
عبارت_Resolve_Mod(ساختار xkb_context *ctx, ثابت عبارت_Def *def,
               شمارش mod_type mod_type, ثابت ساختار xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    اگر (def->common.type != STMT_عبارت_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        بازگشت false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    اگر (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        بازگشت false;
    }

    *ndx_rtrn = ndx;
    بازگشت true;
}

bool
عبارت_Resolve_Group_Mask(ثابت ساختار xkb_keymap_info *keymap_info,
                     ثابت عبارت_Def *عبارت, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    ایستا ثابت LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { پوچ, 0 }
    };

    ثابت ساختار named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = اندازه("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    بازگشت عبارت_Resolve_Mask_Lookup(keymap_info->keymap.ctx, عبارت, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
