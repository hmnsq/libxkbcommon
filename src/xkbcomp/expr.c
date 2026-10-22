/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <stdint.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "xkbcomp-priv.h"
#অন্তর্ভাৱ "expr.h"
#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "keysym.h"
#অন্তর্ভাৱ "messages-codes.h"
#অন্তর্ভাৱ "text.h"
#অন্তর্ভাৱ "xkbcomp/ast.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "util-numbers.h"
#অন্তর্ভাৱ "util-checked-arithmetic.h"

প্রকারনির্দেশ bool (*IdentLookupFunc)(সংরচনা xkb_context *ctx, ধ্রুৱ শূন্য *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
ৰাশিমালা_Resolve_Lhs(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
               ধ্রুৱ ৱর্ণ **elem_rtrn, ধ্রুৱ ৱর্ণ **field_rtrn,
               ৰাশিমালা_Def **index_rtrn)
{
    ৱিকল্প (ৰাশিমালা->common.type) {
    পক্ষ STMT_ৰাশিমালা_IDENT:
        *elem_rtrn = রিক্ত;
        *field_rtrn = xkb_atom_text(ctx, ৰাশিমালা->ident.ident);
        *index_rtrn = রিক্ত;
        ঘূৰাই (*field_rtrn != রিক্ত);
    পক্ষ STMT_ৰাশিমালা_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, ৰাশিমালা->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, ৰাশিমালা->field_ref.field);
        *index_rtrn = রিক্ত;
        ঘূৰাই (*elem_rtrn != রিক্ত && *field_rtrn != রিক্ত);
    পক্ষ STMT_ৰাশিমালা_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, ৰাশিমালা->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, ৰাশিমালা->array_ref.field);
        *index_rtrn = ৰাশিমালা->array_ref.entry;
        যদি (ৰাশিমালা->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == রিক্ত)
            ঘূৰাই false;
        যদি (*field_rtrn == রিক্ত)
            ঘূৰাই false;
        ঘূৰাই true;
    স্ৱভাৱ:
        ভঙ্গ;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", ৰাশিমালা->common.type);
    ঘূৰাই false;
}

স্থির bool
SimpleLookup(সংরচনা xkb_context *ctx, ধ্রুৱ শূন্য *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    যদি (!priv || field == XKB_ATOM_NONE)
        ঘূৰাই false;

    ধ্রুৱ ৱর্ণ *str = xkb_atom_text(ctx, field);
    বাবে (ধ্রুৱ LookupEntry *entry = priv; entry && entry->name; entry++) {
        যদি (istreq(str, entry->name)) {
            *val_rtrn = entry->মান;
            ঘূৰাই true;
        }
    }

    ঘূৰাই false;
}

সংরচনা named_integer_pattern {
    ধ্রুৱ ৱর্ণ *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    ধ্রুৱ LookupEntry *entries;
    ধ্রুৱ LookupEntry *pending_entries;
    bool is_mask;
    গণনা xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
স্থির bool
NamedIntegerPatternLookup(সংরচনা xkb_context *ctx, ধ্রুৱ শূন্য *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    যদি (!priv || field == XKB_ATOM_NONE)
        ঘূৰাই false;

    ধ্রুৱ ৱর্ণ * ধ্রুৱ str = xkb_atom_text(ctx, field);
    ধ্রুৱ সংরচনা named_integer_pattern * ধ্রুৱ pattern = priv;

    ধ্রুৱ পূর্ণাঙ্ক গুনতি = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    যদি (গুনতি > 0 && *(str + pattern->prefix_length + গুনতি) == '\0') {
        যদি (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            ঘূৰাই false;
        }
        যদি (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            প্রতিজ্ঞা(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        ঘূৰাই true;
    } অন্যথা {
        যদি (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, রিক্ত)) {
            ঘূৰাই true;
        }
        যদি (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, রিক্ত)) {
            *pending_rtrn = true;
            ঘূৰাই true;
        }
        ঘূৰাই false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
প্রকারনির্দেশ সংরচনা {
    ধ্রুৱ সংরচনা xkb_mod_set *mods;
    গণনা mod_type mod_type;
} LookupModMaskPriv;

স্থির bool
LookupModMask(সংরচনা xkb_context *ctx, ধ্রুৱ শূন্য *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    ধ্রুৱ ৱর্ণ *str = xkb_atom_text(ctx, field);
    যদি (!str)
        ঘূৰাই false;

    যদি (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        ঘূৰাই true;
    }

    যদি (istreq(str, "none")) {
        *val_rtrn = 0;
        ঘূৰাই true;
    }

    ধ্রুৱ LookupModMaskPriv *arg = priv;
    ধ্রুৱ সংরচনা xkb_mod_set *mods = arg->mods;
    ধ্রুৱ গণনা mod_type mod_type = arg->mod_type;
    ধ্রুৱ xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    যদি (ndx == XKB_MOD_INVALID)
        ঘূৰাই false;

    *val_rtrn = (UINT32_C(1) << ndx);
    ঘূৰাই true;
}

bool
ৰাশিমালা_Resolve_Boolean(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                   bool *set_rtrn)
{
    bool ok = false;
    ধ্রুৱ ৱর্ণ *ident;

    ৱিকল্প (ৰাশিমালা->common.type) {
    পক্ষ STMT_ৰাশিমালা_BOOLEAN_LITERAL:
        *set_rtrn = ৰাশিমালা->boolean.set;
        ঘূৰাই true;

    পক্ষ STMT_ৰাশিমালা_STRING_LITERAL:
    পক্ষ STMT_ৰাশিমালা_INTEGER_LITERAL:
    পক্ষ STMT_ৰাশিমালা_FLOAT_LITERAL:
    পক্ষ STMT_ৰাশিমালা_KEYNAME_LITERAL:
    পক্ষ STMT_ৰাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(ৰাশিমালা->common.type));
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_IDENT:
        ident = xkb_atom_text(ctx, ৰাশিমালা->ident.ident);
        যদি (ident) {
            যদি (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                ঘূৰাই true;
            }
            অন্যথা যদি (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                ঘূৰাই true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, ৰাশিমালা->field_ref.element),
                xkb_atom_text(ctx, ৰাশিমালা->field_ref.field));
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_INVERT:
    পক্ষ STMT_ৰাশিমালা_NOT:
        ok = ৰাশিমালা_Resolve_Boolean(ctx, ৰাশিমালা->unary.child, set_rtrn);
        যদি (ok)
            *set_rtrn = !*set_rtrn;
        ঘূৰাই ok;
    পক্ষ STMT_ৰাশিমালা_ADD:
    পক্ষ STMT_ৰাশিমালা_SUBTRACT:
    পক্ষ STMT_ৰাশিমালা_MULTIPLY:
    পক্ষ STMT_ৰাশিমালা_DIVIDE:
    পক্ষ STMT_ৰাশিমালা_ASSIGN:
    পক্ষ STMT_ৰাশিমালা_NEGATE:
    পক্ষ STMT_ৰাশিমালা_UNARY_PLUS:
    পক্ষ STMT_ৰাশিমালা_EMPTY_LIST:
    পক্ষ STMT_ৰাশিমালা_ACTION_DECL:
    পক্ষ STMT_ৰাশিমালা_ACTION_LIST:
    পক্ষ STMT_ৰাশিমালা_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(ৰাশিমালা->common.type));
        ভঙ্গ;

    স্ৱভাৱ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 ৰাশিমালা->common.type);
        ভঙ্গ;
    }

    ঘূৰাই false;
}

স্থির bool
ৰাশিমালা_Resolve_Integer_Lookup(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ধ্রুৱ শূন্য *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    ৰাশিমালা_Def *বাম, *ডান;

    ৱিকল্প (ৰাশিমালা->common.type) {
    পক্ষ STMT_ৰাশিমালা_INTEGER_LITERAL:
        *val_rtrn = ৰাশিমালা->integer.ival;
        ঘূৰাই true;

    পক্ষ STMT_ৰাশিমালা_STRING_LITERAL:
    পক্ষ STMT_ৰাশিমালা_FLOAT_LITERAL:
    পক্ষ STMT_ৰাশিমালা_BOOLEAN_LITERAL:
    পক্ষ STMT_ৰাশিমালা_KEYNAME_LITERAL:
    পক্ষ STMT_ৰাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(ৰাশিমালা->common.type));
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_IDENT:
        যদি (lookup)
            ok = lookup(ctx, lookupPriv, ৰাশিমালা->ident.ident, &u, pending);

        যদি (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, ৰাশিমালা->ident.ident));
        অন্যথা
            *val_rtrn = (int64_t) u;

        যদি (pending && *pending) {
            /* Postpone computation */
            ঘূৰাই false;
        }

        ঘূৰাই ok;

    পক্ষ STMT_ৰাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, ৰাশিমালা->field_ref.element),
                xkb_atom_text(ctx, ৰাশিমালা->field_ref.field));
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_ADD:
    পক্ষ STMT_ৰাশিমালা_SUBTRACT:
    পক্ষ STMT_ৰাশিমালা_MULTIPLY:
    পক্ষ STMT_ৰাশিমালা_DIVIDE:
        বাম = ৰাশিমালা->দ্বিমিক.বাম;
        ডান = ৰাশিমালা->দ্বিমিক.ডান;
        যদি (!ৰাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &l, pending, lookup, lookupPriv) ||
            !ৰাশিমালা_Resolve_Integer_Lookup(ctx, ডান, &r, pending, lookup, lookupPriv))
            ঘূৰাই false;

        ৱিকল্প (ৰাশিমালা->common.type) {
        পক্ষ STMT_ৰাশিমালা_ADD:
            যদি (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ঘূৰাই false;
            }
            ভঙ্গ;
        পক্ষ STMT_ৰাশিমালা_SUBTRACT:
            যদি (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ঘূৰাই false;
            }
            ভঙ্গ;
        পক্ষ STMT_ৰাশিমালা_MULTIPLY:
            যদি (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ঘূৰাই false;
            }
            ভঙ্গ;
        পক্ষ STMT_ৰাশিমালা_DIVIDE:
            যদি (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                ঘূৰাই false;
            }
            *val_rtrn = l / r;
            ভঙ্গ;
        স্ৱভাৱ:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(ৰাশিমালা->common.type));
            ঘূৰাই false;
        }

        ঘূৰাই true;

    পক্ষ STMT_ৰাশিমালা_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ভঙ্গ;

    পক্ষ STMT_ৰাশিমালা_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_INVERT:
    পক্ষ STMT_ৰাশিমালা_NEGATE:
        বাম = ৰাশিমালা->unary.child;
        যদি (!ৰাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &l, pending, lookup, lookupPriv))
            ঘূৰাই false;

        *val_rtrn = (ৰাশিমালা->common.type == STMT_ৰাশিমালা_NEGATE ? -l : ~l);
        ঘূৰাই true;

    পক্ষ STMT_ৰাশিমালা_UNARY_PLUS:
        বাম = ৰাশিমালা->unary.child;
        ঘূৰাই ৰাশিমালা_Resolve_Integer_Lookup(ctx, বাম, val_rtrn, pending, lookup,
                                        lookupPriv);

    স্ৱভাৱ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 ৰাশিমালা->common.type);
        ভঙ্গ;
    }

    ঘূৰাই false;
}

bool
ৰাশিমালা_Resolve_Integer(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                   int64_t *val_rtrn)
{
    ঘূৰাই ৰাশিমালা_Resolve_Integer_Lookup(ctx, ৰাশিমালা, val_rtrn, রিক্ত, রিক্ত, রিক্ত);
}

গণনা xkb_parser_error
ৰাশিমালা_Resolve_Group(ধ্রুৱ সংরচনা xkb_keymap_info *keymap_info,
                 ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    স্থির ধ্রুৱ LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { রিক্ত, 0 }
    };

    ধ্রুৱ সংরচনা named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = পরিমাণ("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    যদি (!ৰাশিমালা_Resolve_Integer_Lookup(keymap_info->keymap.ctx, ৰাশিমালা, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        ঘূৰাই (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    যদি (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        ঘূৰাই (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    ঘূৰাই PARSER_SUCCESS;
}

bool
ৰাশিমালা_Resolve_Level(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                 xkb_level_index_t *level_rtrn)
{
    স্থির ধ্রুৱ সংরচনা named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = পরিমাণ("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = রিক্ত,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    যদি (!ৰাশিমালা_Resolve_Integer_Lookup(ctx, ৰাশিমালা, &result, রিক্ত,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        ঘূৰাই false;

    যদি (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        ঘূৰাই false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    ঘূৰাই true;
}

bool
ৰাশিমালা_Resolve_Button(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা, int64_t *btn_rtrn)
{
    ঘূৰাই ৰাশিমালা_Resolve_Integer_Lookup(ctx, ৰাশিমালা, btn_rtrn, রিক্ত, SimpleLookup,
                                    buttonNames);
}

bool
ৰাশিমালা_Resolve_String(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                  xkb_atom_t *val_rtrn)
{
    ৱিকল্প (ৰাশিমালা->common.type) {
    পক্ষ STMT_ৰাশিমালা_STRING_LITERAL:
        *val_rtrn = ৰাশিমালা->string.str;
        ঘূৰাই true;

    পক্ষ STMT_ৰাশিমালা_INTEGER_LITERAL:
    পক্ষ STMT_ৰাশিমালা_FLOAT_LITERAL:
    পক্ষ STMT_ৰাশিমালা_BOOLEAN_LITERAL:
    পক্ষ STMT_ৰাশিমালা_KEYNAME_LITERAL:
    পক্ষ STMT_ৰাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(ৰাশিমালা->common.type));
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, ৰাশিমালা->ident.ident));
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, ৰাশিমালা->field_ref.element),
                xkb_atom_text(ctx, ৰাশিমালা->field_ref.field));
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_ADD:
    পক্ষ STMT_ৰাশিমালা_SUBTRACT:
    পক্ষ STMT_ৰাশিমালা_MULTIPLY:
    পক্ষ STMT_ৰাশিমালা_DIVIDE:
    পক্ষ STMT_ৰাশিমালা_ASSIGN:
    পক্ষ STMT_ৰাশিমালা_NEGATE:
    পক্ষ STMT_ৰাশিমালা_INVERT:
    পক্ষ STMT_ৰাশিমালা_NOT:
    পক্ষ STMT_ৰাশিমালা_UNARY_PLUS:
    পক্ষ STMT_ৰাশিমালা_EMPTY_LIST:
    পক্ষ STMT_ৰাশিমালা_ACTION_DECL:
    পক্ষ STMT_ৰাশিমালা_ACTION_LIST:
    পক্ষ STMT_ৰাশিমালা_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(ৰাশিমালা->common.type));
        ঘূৰাই false;

    স্ৱভাৱ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 ৰাশিমালা->common.type);
        ভঙ্গ;
    }
    ঘূৰাই false;
}

bool
ৰাশিমালা_Resolve_Enum(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                uint32_t *val_rtrn, ধ্রুৱ LookupEntry *মানসমূহ)
{
    যদি (ৰাশিমালা->common.type != STMT_ৰাশিমালা_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(ৰাশিমালা->common.type));
        ঘূৰাই false;
    }

    যদি (!SimpleLookup(ctx, মানসমূহ, ৰাশিমালা->ident.ident, val_rtrn, রিক্ত)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, ৰাশিমালা->ident.ident));
        যেতিয়ালৈ (মানসমূহ && মানসমূহ->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", মানসমূহ->name);
            মানসমূহ++;
        }
        ঘূৰাই false;
    }

    ঘূৰাই true;
}

স্থির bool
ৰাশিমালা_Resolve_Mask_Lookup(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ধ্রুৱ শূন্য *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    ৰাশিমালা_Def *বাম, *ডান;
    ধ্রুৱ ৱর্ণ *bogus = রিক্ত;

    ৱিকল্প (ৰাশিমালা->common.type) {
    পক্ষ STMT_ৰাশিমালা_INTEGER_LITERAL:
        যদি (ৰাশিমালা->integer.ival < 0 || ৰাশিমালা->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    ৰাশিমালা->integer.ival < 0 ? "-" : "",
                    imaxabs(ৰাশিমালা->integer.ival),
                    UINT32_MAX);
            ঘূৰাই false;
        }
        *val_rtrn = (uint32_t) ৰাশিমালা->integer.ival;
        ঘূৰাই true;

    পক্ষ STMT_ৰাশিমালা_STRING_LITERAL:
    পক্ষ STMT_ৰাশিমালা_FLOAT_LITERAL:
    পক্ষ STMT_ৰাশিমালা_BOOLEAN_LITERAL:
    পক্ষ STMT_ৰাশিমালা_KEYNAME_LITERAL:
    পক্ষ STMT_ৰাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(ৰাশিমালা->common.type));
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_IDENT:
        ok = lookup(ctx, lookupPriv, ৰাশিমালা->ident.ident, val_rtrn, pending);
        যদি (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, ৰাশিমালা->ident.ident));
        যদি (pending && *pending) {
            /* Postpone computation */
            ঘূৰাই false;
        }
        ঘূৰাই ok;

    পক্ষ STMT_ৰাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, ৰাশিমালা->field_ref.element),
                xkb_atom_text(ctx, ৰাশিমালা->field_ref.field));
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    পক্ষ STMT_ৰাশিমালা_ACTION_DECL:
        যদি (bogus == রিক্ত)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        ঘূৰাই false;

    পক্ষ STMT_ৰাশিমালা_ADD:
    পক্ষ STMT_ৰাশিমালা_SUBTRACT:
    পক্ষ STMT_ৰাশিমালা_MULTIPLY:
    পক্ষ STMT_ৰাশিমালা_DIVIDE:
        বাম = ৰাশিমালা->দ্বিমিক.বাম;
        ডান = ৰাশিমালা->দ্বিমিক.ডান;
        যদি (!ৰাশিমালা_Resolve_Mask_Lookup(ctx, বাম, &l, pending, lookup, lookupPriv) ||
            !ৰাশিমালা_Resolve_Mask_Lookup(ctx, ডান, &r, pending, lookup, lookupPriv))
            ঘূৰাই false;

        ৱিকল্প (ৰাশিমালা->common.type) {
        পক্ষ STMT_ৰাশিমালা_ADD:
            *val_rtrn = l | r;
            ভঙ্গ;
        পক্ষ STMT_ৰাশিমালা_SUBTRACT:
            *val_rtrn = l & (~r);
            ভঙ্গ;
        পক্ষ STMT_ৰাশিমালা_MULTIPLY:
        পক্ষ STMT_ৰাশিমালা_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (ৰাশিমালা->common.type == STMT_ৰাশিমালা_DIVIDE ? "divide" : "multiply"));
            ঘূৰাই false;
        স্ৱভাৱ:
            ভঙ্গ;
        }

        ঘূৰাই true;

    পক্ষ STMT_ৰাশিমালা_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ভঙ্গ;

    পক্ষ STMT_ৰাশিমালা_INVERT:
        বাম = ৰাশিমালা->unary.child;
        যদি (!ৰাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &v, pending, lookup, lookupPriv))
            ঘূৰাই false;
        যদি (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            ঘূৰাই false;
        }
        *val_rtrn = ~(uint32_t) v;
        ঘূৰাই true;

    পক্ষ STMT_ৰাশিমালা_UNARY_PLUS:
    পক্ষ STMT_ৰাশিমালা_NEGATE:
    পক্ষ STMT_ৰাশিমালা_NOT:
        বাম = ৰাশিমালা->unary.child;
        যদি (!ৰাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &v, pending, lookup, lookupPriv))
            ঘূৰাই false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(ৰাশিমালা->common.type));
        ঘূৰাই false;

    স্ৱভাৱ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 ৰাশিমালা->common.type);
        ভঙ্গ;
    }

    ঘূৰাই false;
}

bool
ৰাশিমালা_Resolve_Mask(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                uint32_t *mask_rtrn, ধ্রুৱ LookupEntry *মানসমূহ)
{
    ঘূৰাই ৰাশিমালা_Resolve_Mask_Lookup(ctx, ৰাশিমালা, mask_rtrn, রিক্ত, SimpleLookup, মানসমূহ);
}

bool
ৰাশিমালা_Resolve_Mod_Mask(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা,
                   গণনা mod_type mod_type, ধ্রুৱ সংরচনা xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    ঘূৰাই ৰাশিমালা_Resolve_Mask_Lookup(ctx, ৰাশিমালা, mask_rtrn, রিক্ত, LookupModMask, &priv);
}

bool
ৰাশিমালা_Resolve_Mod(সংরচনা xkb_context *ctx, ধ্রুৱ ৰাশিমালা_Def *def,
               গণনা mod_type mod_type, ধ্রুৱ সংরচনা xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    যদি (def->common.type != STMT_ৰাশিমালা_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        ঘূৰাই false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    যদি (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        ঘূৰাই false;
    }

    *ndx_rtrn = ndx;
    ঘূৰাই true;
}

bool
ৰাশিমালা_Resolve_Group_Mask(ধ্রুৱ সংরচনা xkb_keymap_info *keymap_info,
                     ধ্রুৱ ৰাশিমালা_Def *ৰাশিমালা, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    স্থির ধ্রুৱ LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { রিক্ত, 0 }
    };

    ধ্রুৱ সংরচনা named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = পরিমাণ("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    ঘূৰাই ৰাশিমালা_Resolve_Mask_Lookup(keymap_info->keymap.ctx, ৰাশিমালা, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
