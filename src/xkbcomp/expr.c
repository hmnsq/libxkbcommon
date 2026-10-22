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
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
ئىپادە_Resolve_Lhs(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە,
               ثابت حرف **elem_rtrn, ثابت حرف **field_rtrn,
               ئىپادە_Def **index_rtrn)
{
    بدل (ئىپادە->common.type) {
    حالة STMT_ئىپادە_IDENT:
        *elem_rtrn = عدم;
        *field_rtrn = xkb_atom_text(ctx, ئىپادە->ident.ident);
        *index_rtrn = عدم;
        أرجع (*field_rtrn != عدم);
    حالة STMT_ئىپادە_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, ئىپادە->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, ئىپادە->field_ref.field);
        *index_rtrn = عدم;
        أرجع (*elem_rtrn != عدم && *field_rtrn != عدم);
    حالة STMT_ئىپادە_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, ئىپادە->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, ئىپادە->array_ref.field);
        *index_rtrn = ئىپادە->array_ref.entry;
        إذا (ئىپادە->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == عدم)
            أرجع false;
        إذا (*field_rtrn == عدم)
            أرجع false;
        أرجع true;
    افتراضي:
        اكسر;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", ئىپادە->common.type);
    أرجع false;
}

ساكن bool
SimpleLookup(بنية xkb_context *ctx, ثابت فراغ *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    إذا (!priv || field == XKB_ATOM_NONE)
        أرجع false;

    ثابت حرف *str = xkb_atom_text(ctx, field);
    لكل (ثابت LookupEntry *entry = priv; entry && entry->name; entry++) {
        إذا (istreq(str, entry->name)) {
            *val_rtrn = entry->قىممەت;
            أرجع true;
        }
    }

    أرجع false;
}

بنية named_integer_pattern {
    ثابت حرف *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    ثابت LookupEntry *entries;
    ثابت LookupEntry *pending_entries;
    bool is_mask;
    تعداد xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ساكن bool
NamedIntegerPatternLookup(بنية xkb_context *ctx, ثابت فراغ *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    إذا (!priv || field == XKB_ATOM_NONE)
        أرجع false;

    ثابت حرف * ثابت str = xkb_atom_text(ctx, field);
    ثابت بنية named_integer_pattern * ثابت pattern = priv;

    ثابت صحيح سانى = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    إذا (سانى > 0 && *(str + pattern->prefix_length + سانى) == '\0') {
        إذا (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            أرجع false;
        }
        إذا (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            أكد(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        أرجع true;
    } وإلا {
        إذا (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, عدم)) {
            أرجع true;
        }
        إذا (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, عدم)) {
            *pending_rtrn = true;
            أرجع true;
        }
        أرجع false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
تعريفنوع بنية {
    ثابت بنية xkb_mod_set *mods;
    تعداد mod_type mod_type;
} LookupModMaskPriv;

ساكن bool
LookupModMask(بنية xkb_context *ctx, ثابت فراغ *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    ثابت حرف *str = xkb_atom_text(ctx, field);
    إذا (!str)
        أرجع false;

    إذا (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        أرجع true;
    }

    إذا (istreq(str, "none")) {
        *val_rtrn = 0;
        أرجع true;
    }

    ثابت LookupModMaskPriv *arg = priv;
    ثابت بنية xkb_mod_set *mods = arg->mods;
    ثابت تعداد mod_type mod_type = arg->mod_type;
    ثابت xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    إذا (ndx == XKB_MOD_INVALID)
        أرجع false;

    *val_rtrn = (UINT32_C(1) << ndx);
    أرجع true;
}

bool
ئىپادە_Resolve_Boolean(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە,
                   bool *set_rtrn)
{
    bool ok = false;
    ثابت حرف *ident;

    بدل (ئىپادە->common.type) {
    حالة STMT_ئىپادە_BOOLEAN_LITERAL:
        *set_rtrn = ئىپادە->boolean.set;
        أرجع true;

    حالة STMT_ئىپادە_STRING_LITERAL:
    حالة STMT_ئىپادە_INTEGER_LITERAL:
    حالة STMT_ئىپادە_FLOAT_LITERAL:
    حالة STMT_ئىپادە_KEYNAME_LITERAL:
    حالة STMT_ئىپادە_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(ئىپادە->common.type));
        أرجع false;

    حالة STMT_ئىپادە_IDENT:
        ident = xkb_atom_text(ctx, ئىپادە->ident.ident);
        إذا (ident) {
            إذا (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                أرجع true;
            }
            وإلا إذا (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                أرجع true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        أرجع false;

    حالة STMT_ئىپادە_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, ئىپادە->field_ref.element),
                xkb_atom_text(ctx, ئىپادە->field_ref.field));
        أرجع false;

    حالة STMT_ئىپادە_INVERT:
    حالة STMT_ئىپادە_NOT:
        ok = ئىپادە_Resolve_Boolean(ctx, ئىپادە->unary.child, set_rtrn);
        إذا (ok)
            *set_rtrn = !*set_rtrn;
        أرجع ok;
    حالة STMT_ئىپادە_ADD:
    حالة STMT_ئىپادە_SUBTRACT:
    حالة STMT_ئىپادە_MULTIPLY:
    حالة STMT_ئىپادە_DIVIDE:
    حالة STMT_ئىپادە_ASSIGN:
    حالة STMT_ئىپادە_NEGATE:
    حالة STMT_ئىپادە_UNARY_PLUS:
    حالة STMT_ئىپادە_EMPTY_LIST:
    حالة STMT_ئىپادە_ACTION_DECL:
    حالة STMT_ئىپادە_ACTION_LIST:
    حالة STMT_ئىپادە_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(ئىپادە->common.type));
        اكسر;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 ئىپادە->common.type);
        اكسر;
    }

    أرجع false;
}

ساكن bool
ئىپادە_Resolve_Integer_Lookup(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ثابت فراغ *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    ئىپادە_Def *سول, *ئوڭ;

    بدل (ئىپادە->common.type) {
    حالة STMT_ئىپادە_INTEGER_LITERAL:
        *val_rtrn = ئىپادە->integer.ival;
        أرجع true;

    حالة STMT_ئىپادە_STRING_LITERAL:
    حالة STMT_ئىپادە_FLOAT_LITERAL:
    حالة STMT_ئىپادە_BOOLEAN_LITERAL:
    حالة STMT_ئىپادە_KEYNAME_LITERAL:
    حالة STMT_ئىپادە_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(ئىپادە->common.type));
        أرجع false;

    حالة STMT_ئىپادە_IDENT:
        إذا (lookup)
            ok = lookup(ctx, lookupPriv, ئىپادە->ident.ident, &u, pending);

        إذا (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, ئىپادە->ident.ident));
        وإلا
            *val_rtrn = (int64_t) u;

        إذا (pending && *pending) {
            /* Postpone computation */
            أرجع false;
        }

        أرجع ok;

    حالة STMT_ئىپادە_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, ئىپادە->field_ref.element),
                xkb_atom_text(ctx, ئىپادە->field_ref.field));
        أرجع false;

    حالة STMT_ئىپادە_ADD:
    حالة STMT_ئىپادە_SUBTRACT:
    حالة STMT_ئىپادە_MULTIPLY:
    حالة STMT_ئىپادە_DIVIDE:
        سول = ئىپادە->ئىككىلىك.سول;
        ئوڭ = ئىپادە->ئىككىلىك.ئوڭ;
        إذا (!ئىپادە_Resolve_Integer_Lookup(ctx, سول, &l, pending, lookup, lookupPriv) ||
            !ئىپادە_Resolve_Integer_Lookup(ctx, ئوڭ, &r, pending, lookup, lookupPriv))
            أرجع false;

        بدل (ئىپادە->common.type) {
        حالة STMT_ئىپادە_ADD:
            إذا (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_ئىپادە_SUBTRACT:
            إذا (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_ئىپادە_MULTIPLY:
            إذا (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                أرجع false;
            }
            اكسر;
        حالة STMT_ئىپادە_DIVIDE:
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
                    stmt_type_to_string(ئىپادە->common.type));
            أرجع false;
        }

        أرجع true;

    حالة STMT_ئىپادە_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        اكسر;

    حالة STMT_ئىپادە_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        أرجع false;

    حالة STMT_ئىپادە_INVERT:
    حالة STMT_ئىپادە_NEGATE:
        سول = ئىپادە->unary.child;
        إذا (!ئىپادە_Resolve_Integer_Lookup(ctx, سول, &l, pending, lookup, lookupPriv))
            أرجع false;

        *val_rtrn = (ئىپادە->common.type == STMT_ئىپادە_NEGATE ? -l : ~l);
        أرجع true;

    حالة STMT_ئىپادە_UNARY_PLUS:
        سول = ئىپادە->unary.child;
        أرجع ئىپادە_Resolve_Integer_Lookup(ctx, سول, val_rtrn, pending, lookup,
                                        lookupPriv);

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 ئىپادە->common.type);
        اكسر;
    }

    أرجع false;
}

bool
ئىپادە_Resolve_Integer(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە,
                   int64_t *val_rtrn)
{
    أرجع ئىپادە_Resolve_Integer_Lookup(ctx, ئىپادە, val_rtrn, عدم, عدم, عدم);
}

تعداد xkb_parser_error
ئىپادە_Resolve_Group(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت ئىپادە_Def *ئىپادە, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    ساكن ثابت LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { عدم, 0 }
    };

    ثابت بنية named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    إذا (!ئىپادە_Resolve_Integer_Lookup(keymap_info->keymap.ctx, ئىپادە, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    إذا (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    أرجع PARSER_SUCCESS;
}

bool
ئىپادە_Resolve_Level(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە,
                 xkb_level_index_t *level_rtrn)
{
    ساكن ثابت بنية named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = حجم("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = عدم,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    إذا (!ئىپادە_Resolve_Integer_Lookup(ctx, ئىپادە, &result, عدم,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        أرجع false;

    إذا (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        أرجع false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    أرجع true;
}

bool
ئىپادە_Resolve_Button(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە, int64_t *btn_rtrn)
{
    أرجع ئىپادە_Resolve_Integer_Lookup(ctx, ئىپادە, btn_rtrn, عدم, SimpleLookup,
                                    buttonNames);
}

bool
ئىپادە_Resolve_String(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە,
                  xkb_atom_t *val_rtrn)
{
    بدل (ئىپادە->common.type) {
    حالة STMT_ئىپادە_STRING_LITERAL:
        *val_rtrn = ئىپادە->string.str;
        أرجع true;

    حالة STMT_ئىپادە_INTEGER_LITERAL:
    حالة STMT_ئىپادە_FLOAT_LITERAL:
    حالة STMT_ئىپادە_BOOLEAN_LITERAL:
    حالة STMT_ئىپادە_KEYNAME_LITERAL:
    حالة STMT_ئىپادە_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(ئىپادە->common.type));
        أرجع false;

    حالة STMT_ئىپادە_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, ئىپادە->ident.ident));
        أرجع false;

    حالة STMT_ئىپادە_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, ئىپادە->field_ref.element),
                xkb_atom_text(ctx, ئىپادە->field_ref.field));
        أرجع false;

    حالة STMT_ئىپادە_ADD:
    حالة STMT_ئىپادە_SUBTRACT:
    حالة STMT_ئىپادە_MULTIPLY:
    حالة STMT_ئىپادە_DIVIDE:
    حالة STMT_ئىپادە_ASSIGN:
    حالة STMT_ئىپادە_NEGATE:
    حالة STMT_ئىپادە_INVERT:
    حالة STMT_ئىپادە_NOT:
    حالة STMT_ئىپادە_UNARY_PLUS:
    حالة STMT_ئىپادە_EMPTY_LIST:
    حالة STMT_ئىپادە_ACTION_DECL:
    حالة STMT_ئىپادە_ACTION_LIST:
    حالة STMT_ئىپادە_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(ئىپادە->common.type));
        أرجع false;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 ئىپادە->common.type);
        اكسر;
    }
    أرجع false;
}

bool
ئىپادە_Resolve_Enum(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە,
                uint32_t *val_rtrn, ثابت LookupEntry *قىممەتلەر)
{
    إذا (ئىپادە->common.type != STMT_ئىپادە_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(ئىپادە->common.type));
        أرجع false;
    }

    إذا (!SimpleLookup(ctx, قىممەتلەر, ئىپادە->ident.ident, val_rtrn, عدم)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, ئىپادە->ident.ident));
        طالما (قىممەتلەر && قىممەتلەر->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", قىممەتلەر->name);
            قىممەتلەر++;
        }
        أرجع false;
    }

    أرجع true;
}

ساكن bool
ئىپادە_Resolve_Mask_Lookup(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ثابت فراغ *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    ئىپادە_Def *سول, *ئوڭ;
    ثابت حرف *bogus = عدم;

    بدل (ئىپادە->common.type) {
    حالة STMT_ئىپادە_INTEGER_LITERAL:
        إذا (ئىپادە->integer.ival < 0 || ئىپادە->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    ئىپادە->integer.ival < 0 ? "-" : "",
                    imaxabs(ئىپادە->integer.ival),
                    UINT32_MAX);
            أرجع false;
        }
        *val_rtrn = (uint32_t) ئىپادە->integer.ival;
        أرجع true;

    حالة STMT_ئىپادە_STRING_LITERAL:
    حالة STMT_ئىپادە_FLOAT_LITERAL:
    حالة STMT_ئىپادە_BOOLEAN_LITERAL:
    حالة STMT_ئىپادە_KEYNAME_LITERAL:
    حالة STMT_ئىپادە_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(ئىپادە->common.type));
        أرجع false;

    حالة STMT_ئىپادە_IDENT:
        ok = lookup(ctx, lookupPriv, ئىپادە->ident.ident, val_rtrn, pending);
        إذا (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, ئىپادە->ident.ident));
        إذا (pending && *pending) {
            /* Postpone computation */
            أرجع false;
        }
        أرجع ok;

    حالة STMT_ئىپادە_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, ئىپادە->field_ref.element),
                xkb_atom_text(ctx, ئىپادە->field_ref.field));
        أرجع false;

    حالة STMT_ئىپادە_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    حالة STMT_ئىپادە_ACTION_DECL:
        إذا (bogus == عدم)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        أرجع false;

    حالة STMT_ئىپادە_ADD:
    حالة STMT_ئىپادە_SUBTRACT:
    حالة STMT_ئىپادە_MULTIPLY:
    حالة STMT_ئىپادە_DIVIDE:
        سول = ئىپادە->ئىككىلىك.سول;
        ئوڭ = ئىپادە->ئىككىلىك.ئوڭ;
        إذا (!ئىپادە_Resolve_Mask_Lookup(ctx, سول, &l, pending, lookup, lookupPriv) ||
            !ئىپادە_Resolve_Mask_Lookup(ctx, ئوڭ, &r, pending, lookup, lookupPriv))
            أرجع false;

        بدل (ئىپادە->common.type) {
        حالة STMT_ئىپادە_ADD:
            *val_rtrn = l | r;
            اكسر;
        حالة STMT_ئىپادە_SUBTRACT:
            *val_rtrn = l & (~r);
            اكسر;
        حالة STMT_ئىپادە_MULTIPLY:
        حالة STMT_ئىپادە_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (ئىپادە->common.type == STMT_ئىپادە_DIVIDE ? "divide" : "multiply"));
            أرجع false;
        افتراضي:
            اكسر;
        }

        أرجع true;

    حالة STMT_ئىپادە_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        اكسر;

    حالة STMT_ئىپادە_INVERT:
        سول = ئىپادە->unary.child;
        إذا (!ئىپادە_Resolve_Integer_Lookup(ctx, سول, &v, pending, lookup, lookupPriv))
            أرجع false;
        إذا (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            أرجع false;
        }
        *val_rtrn = ~(uint32_t) v;
        أرجع true;

    حالة STMT_ئىپادە_UNARY_PLUS:
    حالة STMT_ئىپادە_NEGATE:
    حالة STMT_ئىپادە_NOT:
        سول = ئىپادە->unary.child;
        إذا (!ئىپادە_Resolve_Integer_Lookup(ctx, سول, &v, pending, lookup, lookupPriv))
            أرجع false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(ئىپادە->common.type));
        أرجع false;

    افتراضي:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 ئىپادە->common.type);
        اكسر;
    }

    أرجع false;
}

bool
ئىپادە_Resolve_Mask(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە,
                uint32_t *mask_rtrn, ثابت LookupEntry *قىممەتلەر)
{
    أرجع ئىپادە_Resolve_Mask_Lookup(ctx, ئىپادە, mask_rtrn, عدم, SimpleLookup, قىممەتلەر);
}

bool
ئىپادە_Resolve_Mod_Mask(بنية xkb_context *ctx, ثابت ئىپادە_Def *ئىپادە,
                   تعداد mod_type mod_type, ثابت بنية xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    أرجع ئىپادە_Resolve_Mask_Lookup(ctx, ئىپادە, mask_rtrn, عدم, LookupModMask, &priv);
}

bool
ئىپادە_Resolve_Mod(بنية xkb_context *ctx, ثابت ئىپادە_Def *def,
               تعداد mod_type mod_type, ثابت بنية xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    إذا (def->common.type != STMT_ئىپادە_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        أرجع false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    إذا (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        أرجع false;
    }

    *ndx_rtrn = ndx;
    أرجع true;
}

bool
ئىپادە_Resolve_Group_Mask(ثابت بنية xkb_keymap_info *keymap_info,
                     ثابت ئىپادە_Def *ئىپادە, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    ساكن ثابت LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { عدم, 0 }
    };

    ثابت بنية named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = حجم("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    أرجع ئىپادە_Resolve_Mask_Lookup(keymap_info->keymap.ctx, ئىپادە, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
