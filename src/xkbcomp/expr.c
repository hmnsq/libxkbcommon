/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdint.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "expr.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "keysym.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "text.h"
#অন্তর্ভাব "xkbcomp/ast.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "util-numbers.h"
#অন্তর্ভাব "util-checked-arithmetic.h"

প্রকারনির্দেশ bool (*IdentLookupFunc)(সংরচনা xkb_context *ctx, ধ্রুব শূন্য *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
রাশিমালা_Resolve_Lhs(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
               ধ্রুব বর্ণ **elem_rtrn, ধ্রুব বর্ণ **field_rtrn,
               রাশিমালা_Def **index_rtrn)
{
    বিকল্প (রাশিমালা->common.type) {
    পক্ষ STMT_রাশিমালা_IDENT:
        *elem_rtrn = রিক্ত;
        *field_rtrn = xkb_atom_text(ctx, রাশিমালা->ident.ident);
        *index_rtrn = রিক্ত;
        প্রত্যাগম (*field_rtrn != রিক্ত);
    পক্ষ STMT_রাশিমালা_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, রাশিমালা->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, রাশিমালা->field_ref.field);
        *index_rtrn = রিক্ত;
        প্রত্যাগম (*elem_rtrn != রিক্ত && *field_rtrn != রিক্ত);
    পক্ষ STMT_রাশিমালা_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, রাশিমালা->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, রাশিমালা->array_ref.field);
        *index_rtrn = রাশিমালা->array_ref.entry;
        যদি (রাশিমালা->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == রিক্ত)
            প্রত্যাগম false;
        যদি (*field_rtrn == রিক্ত)
            প্রত্যাগম false;
        প্রত্যাগম true;
    স্বভাব:
        ভঙ্গ;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", রাশিমালা->common.type);
    প্রত্যাগম false;
}

স্থির bool
SimpleLookup(সংরচনা xkb_context *ctx, ধ্রুব শূন্য *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    যদি (!priv || field == XKB_ATOM_NONE)
        প্রত্যাগম false;

    ধ্রুব বর্ণ *str = xkb_atom_text(ctx, field);
    প্রতি (ধ্রুব LookupEntry *entry = priv; entry && entry->name; entry++) {
        যদি (istreq(str, entry->name)) {
            *val_rtrn = entry->মান;
            প্রত্যাগম true;
        }
    }

    প্রত্যাগম false;
}

সংরচনা named_integer_pattern {
    ধ্রুব বর্ণ *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    ধ্রুব LookupEntry *entries;
    ধ্রুব LookupEntry *pending_entries;
    bool is_mask;
    গণনা xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
স্থির bool
NamedIntegerPatternLookup(সংরচনা xkb_context *ctx, ধ্রুব শূন্য *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    যদি (!priv || field == XKB_ATOM_NONE)
        প্রত্যাগম false;

    ধ্রুব বর্ণ * ধ্রুব str = xkb_atom_text(ctx, field);
    ধ্রুব সংরচনা named_integer_pattern * ধ্রুব pattern = priv;

    ধ্রুব পূর্ণাঙ্ক গুনতি = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    যদি (গুনতি > 0 && *(str + pattern->prefix_length + গুনতি) == '\0') {
        যদি (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            প্রত্যাগম false;
        }
        যদি (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            প্রতিজ্ঞা(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        প্রত্যাগম true;
    } অন্যথা {
        যদি (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, রিক্ত)) {
            প্রত্যাগম true;
        }
        যদি (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, রিক্ত)) {
            *pending_rtrn = true;
            প্রত্যাগম true;
        }
        প্রত্যাগম false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
প্রকারনির্দেশ সংরচনা {
    ধ্রুব সংরচনা xkb_mod_set *mods;
    গণনা mod_type mod_type;
} LookupModMaskPriv;

স্থির bool
LookupModMask(সংরচনা xkb_context *ctx, ধ্রুব শূন্য *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    ধ্রুব বর্ণ *str = xkb_atom_text(ctx, field);
    যদি (!str)
        প্রত্যাগম false;

    যদি (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        প্রত্যাগম true;
    }

    যদি (istreq(str, "none")) {
        *val_rtrn = 0;
        প্রত্যাগম true;
    }

    ধ্রুব LookupModMaskPriv *arg = priv;
    ধ্রুব সংরচনা xkb_mod_set *mods = arg->mods;
    ধ্রুব গণনা mod_type mod_type = arg->mod_type;
    ধ্রুব xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    যদি (ndx == XKB_MOD_INVALID)
        প্রত্যাগম false;

    *val_rtrn = (UINT32_C(1) << ndx);
    প্রত্যাগম true;
}

bool
রাশিমালা_Resolve_Boolean(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                   bool *set_rtrn)
{
    bool ok = false;
    ধ্রুব বর্ণ *ident;

    বিকল্প (রাশিমালা->common.type) {
    পক্ষ STMT_রাশিমালা_BOOLEAN_LITERAL:
        *set_rtrn = রাশিমালা->boolean.set;
        প্রত্যাগম true;

    পক্ষ STMT_রাশিমালা_STRING_LITERAL:
    পক্ষ STMT_রাশিমালা_INTEGER_LITERAL:
    পক্ষ STMT_রাশিমালা_FLOAT_LITERAL:
    পক্ষ STMT_রাশিমালা_KEYNAME_LITERAL:
    পক্ষ STMT_রাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(রাশিমালা->common.type));
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_IDENT:
        ident = xkb_atom_text(ctx, রাশিমালা->ident.ident);
        যদি (ident) {
            যদি (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                প্রত্যাগম true;
            }
            অন্যথা যদি (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                প্রত্যাগম true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, রাশিমালা->field_ref.element),
                xkb_atom_text(ctx, রাশিমালা->field_ref.field));
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_INVERT:
    পক্ষ STMT_রাশিমালা_NOT:
        ok = রাশিমালা_Resolve_Boolean(ctx, রাশিমালা->unary.child, set_rtrn);
        যদি (ok)
            *set_rtrn = !*set_rtrn;
        প্রত্যাগম ok;
    পক্ষ STMT_রাশিমালা_ADD:
    পক্ষ STMT_রাশিমালা_SUBTRACT:
    পক্ষ STMT_রাশিমালা_MULTIPLY:
    পক্ষ STMT_রাশিমালা_DIVIDE:
    পক্ষ STMT_রাশিমালা_ASSIGN:
    পক্ষ STMT_রাশিমালা_NEGATE:
    পক্ষ STMT_রাশিমালা_UNARY_PLUS:
    পক্ষ STMT_রাশিমালা_EMPTY_LIST:
    পক্ষ STMT_রাশিমালা_ACTION_DECL:
    পক্ষ STMT_রাশিমালা_ACTION_LIST:
    পক্ষ STMT_রাশিমালা_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(রাশিমালা->common.type));
        ভঙ্গ;

    স্বভাব:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 রাশিমালা->common.type);
        ভঙ্গ;
    }

    প্রত্যাগম false;
}

স্থির bool
রাশিমালা_Resolve_Integer_Lookup(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ধ্রুব শূন্য *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    রাশিমালা_Def *বাম, *ডান;

    বিকল্প (রাশিমালা->common.type) {
    পক্ষ STMT_রাশিমালা_INTEGER_LITERAL:
        *val_rtrn = রাশিমালা->integer.ival;
        প্রত্যাগম true;

    পক্ষ STMT_রাশিমালা_STRING_LITERAL:
    পক্ষ STMT_রাশিমালা_FLOAT_LITERAL:
    পক্ষ STMT_রাশিমালা_BOOLEAN_LITERAL:
    পক্ষ STMT_রাশিমালা_KEYNAME_LITERAL:
    পক্ষ STMT_রাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(রাশিমালা->common.type));
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_IDENT:
        যদি (lookup)
            ok = lookup(ctx, lookupPriv, রাশিমালা->ident.ident, &u, pending);

        যদি (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, রাশিমালা->ident.ident));
        অন্যথা
            *val_rtrn = (int64_t) u;

        যদি (pending && *pending) {
            /* Postpone computation */
            প্রত্যাগম false;
        }

        প্রত্যাগম ok;

    পক্ষ STMT_রাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, রাশিমালা->field_ref.element),
                xkb_atom_text(ctx, রাশিমালা->field_ref.field));
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_ADD:
    পক্ষ STMT_রাশিমালা_SUBTRACT:
    পক্ষ STMT_রাশিমালা_MULTIPLY:
    পক্ষ STMT_রাশিমালা_DIVIDE:
        বাম = রাশিমালা->দ্বিমিক.বাম;
        ডান = রাশিমালা->দ্বিমিক.ডান;
        যদি (!রাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &l, pending, lookup, lookupPriv) ||
            !রাশিমালা_Resolve_Integer_Lookup(ctx, ডান, &r, pending, lookup, lookupPriv))
            প্রত্যাগম false;

        বিকল্প (রাশিমালা->common.type) {
        পক্ষ STMT_রাশিমালা_ADD:
            যদি (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                প্রত্যাগম false;
            }
            ভঙ্গ;
        পক্ষ STMT_রাশিমালা_SUBTRACT:
            যদি (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                প্রত্যাগম false;
            }
            ভঙ্গ;
        পক্ষ STMT_রাশিমালা_MULTIPLY:
            যদি (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                প্রত্যাগম false;
            }
            ভঙ্গ;
        পক্ষ STMT_রাশিমালা_DIVIDE:
            যদি (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                প্রত্যাগম false;
            }
            *val_rtrn = l / r;
            ভঙ্গ;
        স্বভাব:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(রাশিমালা->common.type));
            প্রত্যাগম false;
        }

        প্রত্যাগম true;

    পক্ষ STMT_রাশিমালা_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ভঙ্গ;

    পক্ষ STMT_রাশিমালা_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_INVERT:
    পক্ষ STMT_রাশিমালা_NEGATE:
        বাম = রাশিমালা->unary.child;
        যদি (!রাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &l, pending, lookup, lookupPriv))
            প্রত্যাগম false;

        *val_rtrn = (রাশিমালা->common.type == STMT_রাশিমালা_NEGATE ? -l : ~l);
        প্রত্যাগম true;

    পক্ষ STMT_রাশিমালা_UNARY_PLUS:
        বাম = রাশিমালা->unary.child;
        প্রত্যাগম রাশিমালা_Resolve_Integer_Lookup(ctx, বাম, val_rtrn, pending, lookup,
                                        lookupPriv);

    স্বভাব:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 রাশিমালা->common.type);
        ভঙ্গ;
    }

    প্রত্যাগম false;
}

bool
রাশিমালা_Resolve_Integer(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                   int64_t *val_rtrn)
{
    প্রত্যাগম রাশিমালা_Resolve_Integer_Lookup(ctx, রাশিমালা, val_rtrn, রিক্ত, রিক্ত, রিক্ত);
}

গণনা xkb_parser_error
রাশিমালা_Resolve_Group(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                 ধ্রুব রাশিমালা_Def *রাশিমালা, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    স্থির ধ্রুব LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { রিক্ত, 0 }
    };

    ধ্রুব সংরচনা named_integer_pattern group_name_pattern = {
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
    যদি (!রাশিমালা_Resolve_Integer_Lookup(keymap_info->keymap.ctx, রাশিমালা, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    যদি (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    প্রত্যাগম PARSER_SUCCESS;
}

bool
রাশিমালা_Resolve_Level(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                 xkb_level_index_t *level_rtrn)
{
    স্থির ধ্রুব সংরচনা named_integer_pattern level_name_pattern = {
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
    যদি (!রাশিমালা_Resolve_Integer_Lookup(ctx, রাশিমালা, &result, রিক্ত,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        প্রত্যাগম false;

    যদি (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        প্রত্যাগম false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    প্রত্যাগম true;
}

bool
রাশিমালা_Resolve_Button(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা, int64_t *btn_rtrn)
{
    প্রত্যাগম রাশিমালা_Resolve_Integer_Lookup(ctx, রাশিমালা, btn_rtrn, রিক্ত, SimpleLookup,
                                    buttonNames);
}

bool
রাশিমালা_Resolve_String(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                  xkb_atom_t *val_rtrn)
{
    বিকল্প (রাশিমালা->common.type) {
    পক্ষ STMT_রাশিমালা_STRING_LITERAL:
        *val_rtrn = রাশিমালা->string.str;
        প্রত্যাগম true;

    পক্ষ STMT_রাশিমালা_INTEGER_LITERAL:
    পক্ষ STMT_রাশিমালা_FLOAT_LITERAL:
    পক্ষ STMT_রাশিমালা_BOOLEAN_LITERAL:
    পক্ষ STMT_রাশিমালা_KEYNAME_LITERAL:
    পক্ষ STMT_রাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(রাশিমালা->common.type));
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, রাশিমালা->ident.ident));
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, রাশিমালা->field_ref.element),
                xkb_atom_text(ctx, রাশিমালা->field_ref.field));
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_ADD:
    পক্ষ STMT_রাশিমালা_SUBTRACT:
    পক্ষ STMT_রাশিমালা_MULTIPLY:
    পক্ষ STMT_রাশিমালা_DIVIDE:
    পক্ষ STMT_রাশিমালা_ASSIGN:
    পক্ষ STMT_রাশিমালা_NEGATE:
    পক্ষ STMT_রাশিমালা_INVERT:
    পক্ষ STMT_রাশিমালা_NOT:
    পক্ষ STMT_রাশিমালা_UNARY_PLUS:
    পক্ষ STMT_রাশিমালা_EMPTY_LIST:
    পক্ষ STMT_রাশিমালা_ACTION_DECL:
    পক্ষ STMT_রাশিমালা_ACTION_LIST:
    পক্ষ STMT_রাশিমালা_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(রাশিমালা->common.type));
        প্রত্যাগম false;

    স্বভাব:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 রাশিমালা->common.type);
        ভঙ্গ;
    }
    প্রত্যাগম false;
}

bool
রাশিমালা_Resolve_Enum(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                uint32_t *val_rtrn, ধ্রুব LookupEntry *মানসমূহ)
{
    যদি (রাশিমালা->common.type != STMT_রাশিমালা_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(রাশিমালা->common.type));
        প্রত্যাগম false;
    }

    যদি (!SimpleLookup(ctx, মানসমূহ, রাশিমালা->ident.ident, val_rtrn, রিক্ত)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, রাশিমালা->ident.ident));
        যাবত্ (মানসমূহ && মানসমূহ->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", মানসমূহ->name);
            মানসমূহ++;
        }
        প্রত্যাগম false;
    }

    প্রত্যাগম true;
}

স্থির bool
রাশিমালা_Resolve_Mask_Lookup(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ধ্রুব শূন্য *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    রাশিমালা_Def *বাম, *ডান;
    ধ্রুব বর্ণ *bogus = রিক্ত;

    বিকল্প (রাশিমালা->common.type) {
    পক্ষ STMT_রাশিমালা_INTEGER_LITERAL:
        যদি (রাশিমালা->integer.ival < 0 || রাশিমালা->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    রাশিমালা->integer.ival < 0 ? "-" : "",
                    imaxabs(রাশিমালা->integer.ival),
                    UINT32_MAX);
            প্রত্যাগম false;
        }
        *val_rtrn = (uint32_t) রাশিমালা->integer.ival;
        প্রত্যাগম true;

    পক্ষ STMT_রাশিমালা_STRING_LITERAL:
    পক্ষ STMT_রাশিমালা_FLOAT_LITERAL:
    পক্ষ STMT_রাশিমালা_BOOLEAN_LITERAL:
    পক্ষ STMT_রাশিমালা_KEYNAME_LITERAL:
    পক্ষ STMT_রাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(রাশিমালা->common.type));
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_IDENT:
        ok = lookup(ctx, lookupPriv, রাশিমালা->ident.ident, val_rtrn, pending);
        যদি (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, রাশিমালা->ident.ident));
        যদি (pending && *pending) {
            /* Postpone computation */
            প্রত্যাগম false;
        }
        প্রত্যাগম ok;

    পক্ষ STMT_রাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, রাশিমালা->field_ref.element),
                xkb_atom_text(ctx, রাশিমালা->field_ref.field));
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    পক্ষ STMT_রাশিমালা_ACTION_DECL:
        যদি (bogus == রিক্ত)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        প্রত্যাগম false;

    পক্ষ STMT_রাশিমালা_ADD:
    পক্ষ STMT_রাশিমালা_SUBTRACT:
    পক্ষ STMT_রাশিমালা_MULTIPLY:
    পক্ষ STMT_রাশিমালা_DIVIDE:
        বাম = রাশিমালা->দ্বিমিক.বাম;
        ডান = রাশিমালা->দ্বিমিক.ডান;
        যদি (!রাশিমালা_Resolve_Mask_Lookup(ctx, বাম, &l, pending, lookup, lookupPriv) ||
            !রাশিমালা_Resolve_Mask_Lookup(ctx, ডান, &r, pending, lookup, lookupPriv))
            প্রত্যাগম false;

        বিকল্প (রাশিমালা->common.type) {
        পক্ষ STMT_রাশিমালা_ADD:
            *val_rtrn = l | r;
            ভঙ্গ;
        পক্ষ STMT_রাশিমালা_SUBTRACT:
            *val_rtrn = l & (~r);
            ভঙ্গ;
        পক্ষ STMT_রাশিমালা_MULTIPLY:
        পক্ষ STMT_রাশিমালা_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (রাশিমালা->common.type == STMT_রাশিমালা_DIVIDE ? "divide" : "multiply"));
            প্রত্যাগম false;
        স্বভাব:
            ভঙ্গ;
        }

        প্রত্যাগম true;

    পক্ষ STMT_রাশিমালা_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ভঙ্গ;

    পক্ষ STMT_রাশিমালা_INVERT:
        বাম = রাশিমালা->unary.child;
        যদি (!রাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &v, pending, lookup, lookupPriv))
            প্রত্যাগম false;
        যদি (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            প্রত্যাগম false;
        }
        *val_rtrn = ~(uint32_t) v;
        প্রত্যাগম true;

    পক্ষ STMT_রাশিমালা_UNARY_PLUS:
    পক্ষ STMT_রাশিমালা_NEGATE:
    পক্ষ STMT_রাশিমালা_NOT:
        বাম = রাশিমালা->unary.child;
        যদি (!রাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &v, pending, lookup, lookupPriv))
            প্রত্যাগম false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(রাশিমালা->common.type));
        প্রত্যাগম false;

    স্বভাব:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 রাশিমালা->common.type);
        ভঙ্গ;
    }

    প্রত্যাগম false;
}

bool
রাশিমালা_Resolve_Mask(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                uint32_t *mask_rtrn, ধ্রুব LookupEntry *মানসমূহ)
{
    প্রত্যাগম রাশিমালা_Resolve_Mask_Lookup(ctx, রাশিমালা, mask_rtrn, রিক্ত, SimpleLookup, মানসমূহ);
}

bool
রাশিমালা_Resolve_Mod_Mask(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *রাশিমালা,
                   গণনা mod_type mod_type, ধ্রুব সংরচনা xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    প্রত্যাগম রাশিমালা_Resolve_Mask_Lookup(ctx, রাশিমালা, mask_rtrn, রিক্ত, LookupModMask, &priv);
}

bool
রাশিমালা_Resolve_Mod(সংরচনা xkb_context *ctx, ধ্রুব রাশিমালা_Def *def,
               গণনা mod_type mod_type, ধ্রুব সংরচনা xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    যদি (def->common.type != STMT_রাশিমালা_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        প্রত্যাগম false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    যদি (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        প্রত্যাগম false;
    }

    *ndx_rtrn = ndx;
    প্রত্যাগম true;
}

bool
রাশিমালা_Resolve_Group_Mask(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                     ধ্রুব রাশিমালা_Def *রাশিমালা, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    স্থির ধ্রুব LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { রিক্ত, 0 }
    };

    ধ্রুব সংরচনা named_integer_pattern group_name_pattern = {
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

    প্রত্যাগম রাশিমালা_Resolve_Mask_Lookup(keymap_info->keymap.ctx, রাশিমালা, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
