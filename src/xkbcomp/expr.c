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

প্রকারলেখ bool (*IdentLookupFunc)(কাঠামো xkb_context *ctx, ধ্রুবক শূন্য *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
রাশিমালা_Resolve_Lhs(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
               ধ্রুবক অক্ষর **elem_rtrn, ধ্রুবক অক্ষর **field_rtrn,
               রাশিমালা_Def **সূচক_rtrn)
{
    নির্বাচন (রাশিমালা->common.type) {
    ক্ষেত্র STMT_রাশিমালা_IDENT:
        *elem_rtrn = ফাঁকা;
        *field_rtrn = xkb_atom_text(ctx, রাশিমালা->ident.ident);
        *সূচক_rtrn = ফাঁকা;
        ফেরত (*field_rtrn != ফাঁকা);
    ক্ষেত্র STMT_রাশিমালা_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, রাশিমালা->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, রাশিমালা->field_ref.field);
        *সূচক_rtrn = ফাঁকা;
        ফেরত (*elem_rtrn != ফাঁকা && *field_rtrn != ফাঁকা);
    ক্ষেত্র STMT_রাশিমালা_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, রাশিমালা->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, রাশিমালা->array_ref.field);
        *সূচক_rtrn = রাশিমালা->array_ref.entry;
        যদি (রাশিমালা->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == ফাঁকা)
            ফেরত false;
        যদি (*field_rtrn == ফাঁকা)
            ফেরত false;
        ফেরত true;
    পূর্বনির্ধারিত:
        বিরতি;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", রাশিমালা->common.type);
    ফেরত false;
}

স্থির bool
SimpleLookup(কাঠামো xkb_context *ctx, ধ্রুবক শূন্য *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    যদি (!priv || field == XKB_ATOM_NONE)
        ফেরত false;

    ধ্রুবক অক্ষর *str = xkb_atom_text(ctx, field);
    জন্য (ধ্রুবক LookupEntry *entry = priv; entry && entry->name; entry++) {
        যদি (istreq(str, entry->name)) {
            *val_rtrn = entry->মান;
            ফেরত true;
        }
    }

    ফেরত false;
}

কাঠামো named_integer_pattern {
    ধ্রুবক অক্ষর *prefix;
    আকার_t prefix_দৈর্ঘ্য;
    uint32_t min;
    uint32_t max;
    ধ্রুবক LookupEntry *entries;
    ধ্রুবক LookupEntry *pending_entries;
    bool is_mask;
    গণনা xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
স্থির bool
NamedIntegerPatternLookup(কাঠামো xkb_context *ctx, ধ্রুবক শূন্য *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    যদি (!priv || field == XKB_ATOM_NONE)
        ফেরত false;

    ধ্রুবক অক্ষর * ধ্রুবক str = xkb_atom_text(ctx, field);
    ধ্রুবক কাঠামো named_integer_pattern * ধ্রুবক pattern = priv;

    ধ্রুবক পূর্ণসংখ্যা গুনতি = (istrneq(str, pattern->prefix, pattern->prefix_দৈর্ঘ্য))
        ? parse_dec_to_uint32_t(str + pattern->prefix_দৈর্ঘ্য, SIZE_MAX, val_rtrn)
        : 0;

    যদি (গুনতি > 0 && *(str + pattern->prefix_দৈর্ঘ্য + গুনতি) == '\0') {
        যদি (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            ফেরত false;
        }
        যদি (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            প্রতিজ্ঞা(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        ফেরত true;
    } অন্যথা {
        যদি (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, ফাঁকা)) {
            ফেরত true;
        }
        যদি (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, ফাঁকা)) {
            *pending_rtrn = true;
            ফেরত true;
        }
        ফেরত false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
প্রকারলেখ কাঠামো {
    ধ্রুবক কাঠামো xkb_mod_set *mods;
    গণনা mod_type mod_type;
} LookupModMaskPriv;

স্থির bool
LookupModMask(কাঠামো xkb_context *ctx, ধ্রুবক শূন্য *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    ধ্রুবক অক্ষর *str = xkb_atom_text(ctx, field);
    যদি (!str)
        ফেরত false;

    যদি (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        ফেরত true;
    }

    যদি (istreq(str, "none")) {
        *val_rtrn = 0;
        ফেরত true;
    }

    ধ্রুবক LookupModMaskPriv *arg = priv;
    ধ্রুবক কাঠামো xkb_mod_set *mods = arg->mods;
    ধ্রুবক গণনা mod_type mod_type = arg->mod_type;
    ধ্রুবক xkb_mod_সূচক_t ndx = Xkb_Mod_Name_To_সূচক(mods, field, mod_type);
    যদি (ndx == XKB_MOD_INVALID)
        ফেরত false;

    *val_rtrn = (UINT32_C(1) << ndx);
    ফেরত true;
}

bool
রাশিমালা_Resolve_Boolean(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                   bool *set_rtrn)
{
    bool ok = false;
    ধ্রুবক অক্ষর *ident;

    নির্বাচন (রাশিমালা->common.type) {
    ক্ষেত্র STMT_রাশিমালা_BOOLEAN_LITERAL:
        *set_rtrn = রাশিমালা->boolean.set;
        ফেরত true;

    ক্ষেত্র STMT_রাশিমালা_STRING_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_INTEGER_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_FLOAT_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_KEYNAME_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(রাশিমালা->common.type));
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_IDENT:
        ident = xkb_atom_text(ctx, রাশিমালা->ident.ident);
        যদি (ident) {
            যদি (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                ফেরত true;
            }
            অন্যথা যদি (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                ফেরত true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, রাশিমালা->field_ref.element),
                xkb_atom_text(ctx, রাশিমালা->field_ref.field));
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_INVERT:
    ক্ষেত্র STMT_রাশিমালা_NOT:
        ok = রাশিমালা_Resolve_Boolean(ctx, রাশিমালা->unary.child, set_rtrn);
        যদি (ok)
            *set_rtrn = !*set_rtrn;
        ফেরত ok;
    ক্ষেত্র STMT_রাশিমালা_ADD:
    ক্ষেত্র STMT_রাশিমালা_SUBTRACT:
    ক্ষেত্র STMT_রাশিমালা_MULTIPLY:
    ক্ষেত্র STMT_রাশিমালা_DIVIDE:
    ক্ষেত্র STMT_রাশিমালা_ASSIGN:
    ক্ষেত্র STMT_রাশিমালা_NEGATE:
    ক্ষেত্র STMT_রাশিমালা_UNARY_PLUS:
    ক্ষেত্র STMT_রাশিমালা_EMPTY_LIST:
    ক্ষেত্র STMT_রাশিমালা_ACTION_DECL:
    ক্ষেত্র STMT_রাশিমালা_ACTION_LIST:
    ক্ষেত্র STMT_রাশিমালা_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(রাশিমালা->common.type));
        বিরতি;

    পূর্বনির্ধারিত:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 রাশিমালা->common.type);
        বিরতি;
    }

    ফেরত false;
}

স্থির bool
রাশিমালা_Resolve_Integer_Lookup(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ধ্রুবক শূন্য *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    রাশিমালা_Def *বাম, *ডান;

    নির্বাচন (রাশিমালা->common.type) {
    ক্ষেত্র STMT_রাশিমালা_INTEGER_LITERAL:
        *val_rtrn = রাশিমালা->integer.ival;
        ফেরত true;

    ক্ষেত্র STMT_রাশিমালা_STRING_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_FLOAT_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_BOOLEAN_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_KEYNAME_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(রাশিমালা->common.type));
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_IDENT:
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
            ফেরত false;
        }

        ফেরত ok;

    ক্ষেত্র STMT_রাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, রাশিমালা->field_ref.element),
                xkb_atom_text(ctx, রাশিমালা->field_ref.field));
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_ADD:
    ক্ষেত্র STMT_রাশিমালা_SUBTRACT:
    ক্ষেত্র STMT_রাশিমালা_MULTIPLY:
    ক্ষেত্র STMT_রাশিমালা_DIVIDE:
        বাম = রাশিমালা->দ্বিমিক.বাম;
        ডান = রাশিমালা->দ্বিমিক.ডান;
        যদি (!রাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &l, pending, lookup, lookupPriv) ||
            !রাশিমালা_Resolve_Integer_Lookup(ctx, ডান, &r, pending, lookup, lookupPriv))
            ফেরত false;

        নির্বাচন (রাশিমালা->common.type) {
        ক্ষেত্র STMT_রাশিমালা_ADD:
            যদি (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ফেরত false;
            }
            বিরতি;
        ক্ষেত্র STMT_রাশিমালা_SUBTRACT:
            যদি (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ফেরত false;
            }
            বিরতি;
        ক্ষেত্র STMT_রাশিমালা_MULTIPLY:
            যদি (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ফেরত false;
            }
            বিরতি;
        ক্ষেত্র STMT_রাশিমালা_DIVIDE:
            যদি (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                ফেরত false;
            }
            *val_rtrn = l / r;
            বিরতি;
        পূর্বনির্ধারিত:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(রাশিমালা->common.type));
            ফেরত false;
        }

        ফেরত true;

    ক্ষেত্র STMT_রাশিমালা_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        বিরতি;

    ক্ষেত্র STMT_রাশিমালা_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_INVERT:
    ক্ষেত্র STMT_রাশিমালা_NEGATE:
        বাম = রাশিমালা->unary.child;
        যদি (!রাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &l, pending, lookup, lookupPriv))
            ফেরত false;

        *val_rtrn = (রাশিমালা->common.type == STMT_রাশিমালা_NEGATE ? -l : ~l);
        ফেরত true;

    ক্ষেত্র STMT_রাশিমালা_UNARY_PLUS:
        বাম = রাশিমালা->unary.child;
        ফেরত রাশিমালা_Resolve_Integer_Lookup(ctx, বাম, val_rtrn, pending, lookup,
                                        lookupPriv);

    পূর্বনির্ধারিত:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 রাশিমালা->common.type);
        বিরতি;
    }

    ফেরত false;
}

bool
রাশিমালা_Resolve_Integer(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                   int64_t *val_rtrn)
{
    ফেরত রাশিমালা_Resolve_Integer_Lookup(ctx, রাশিমালা, val_rtrn, ফাঁকা, ফাঁকা, ফাঁকা);
}

গণনা xkb_parser_error
রাশিমালা_Resolve_Group(ধ্রুবক কাঠামো xkb_keymap_info *keymap_info,
                 ধ্রুবক রাশিমালা_Def *রাশিমালা, bool absolute,
                 xkb_layout_সূচক_t *group_rtrn, bool *pending)
{
    স্থির ধ্রুবক LookupEntry pending_Group_সূচক_Names[] = {
        { GROUP_LAST_সূচক_NAME, 0 },
        { ফাঁকা, 0 }
    };

    ধ্রুবক কাঠামো named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_দৈর্ঘ্য = আকার("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.group_সূচক_Names,
        .pending_entries = pending_Group_সূচক_Names,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক_2,
    };

    int64_t ফলাফল = 0;
    যদি (!রাশিমালা_Resolve_Integer_Lookup(keymap_info->keymap.ctx, রাশিমালা, &ফলাফল, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        ফেরত (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    যদি (ফলাফল < (int64_t)absolute || ফলাফল > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                ফলাফল, absolute, keymap_info->features.max_groups);
        ফেরত (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_সূচক_t) ফলাফল;
    ফেরত PARSER_SUCCESS;
}

bool
রাশিমালা_Resolve_Level(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                 xkb_level_সূচক_t *level_rtrn)
{
    স্থির ধ্রুবক কাঠামো named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_দৈর্ঘ্য = আকার("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = ফাঁকা,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t ফলাফল = 0;
    যদি (!রাশিমালা_Resolve_Integer_Lookup(ctx, রাশিমালা, &ফলাফল, ফাঁকা,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        ফেরত false;

    যদি (ফলাফল < 1 || ফলাফল > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                ফলাফল, XKB_LEVEL_MAX_IMPL);
        ফেরত false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_সূচক_t) (ফলাফল - 1);
    ফেরত true;
}

bool
রাশিমালা_Resolve_Button(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা, int64_t *btn_rtrn)
{
    ফেরত রাশিমালা_Resolve_Integer_Lookup(ctx, রাশিমালা, btn_rtrn, ফাঁকা, SimpleLookup,
                                    buttonNames);
}

bool
রাশিমালা_Resolve_String(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                  xkb_atom_t *val_rtrn)
{
    নির্বাচন (রাশিমালা->common.type) {
    ক্ষেত্র STMT_রাশিমালা_STRING_LITERAL:
        *val_rtrn = রাশিমালা->string.str;
        ফেরত true;

    ক্ষেত্র STMT_রাশিমালা_INTEGER_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_FLOAT_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_BOOLEAN_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_KEYNAME_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(রাশিমালা->common.type));
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, রাশিমালা->ident.ident));
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, রাশিমালা->field_ref.element),
                xkb_atom_text(ctx, রাশিমালা->field_ref.field));
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_ADD:
    ক্ষেত্র STMT_রাশিমালা_SUBTRACT:
    ক্ষেত্র STMT_রাশিমালা_MULTIPLY:
    ক্ষেত্র STMT_রাশিমালা_DIVIDE:
    ক্ষেত্র STMT_রাশিমালা_ASSIGN:
    ক্ষেত্র STMT_রাশিমালা_NEGATE:
    ক্ষেত্র STMT_রাশিমালা_INVERT:
    ক্ষেত্র STMT_রাশিমালা_NOT:
    ক্ষেত্র STMT_রাশিমালা_UNARY_PLUS:
    ক্ষেত্র STMT_রাশিমালা_EMPTY_LIST:
    ক্ষেত্র STMT_রাশিমালা_ACTION_DECL:
    ক্ষেত্র STMT_রাশিমালা_ACTION_LIST:
    ক্ষেত্র STMT_রাশিমালা_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(রাশিমালা->common.type));
        ফেরত false;

    পূর্বনির্ধারিত:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 রাশিমালা->common.type);
        বিরতি;
    }
    ফেরত false;
}

bool
রাশিমালা_Resolve_Enum(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                uint32_t *val_rtrn, ধ্রুবক LookupEntry *মানসমূহ)
{
    যদি (রাশিমালা->common.type != STMT_রাশিমালা_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(রাশিমালা->common.type));
        ফেরত false;
    }

    যদি (!SimpleLookup(ctx, মানসমূহ, রাশিমালা->ident.ident, val_rtrn, ফাঁকা)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, রাশিমালা->ident.ident));
        যখন (মানসমূহ && মানসমূহ->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", মানসমূহ->name);
            মানসমূহ++;
        }
        ফেরত false;
    }

    ফেরত true;
}

স্থির bool
রাশিমালা_Resolve_Mask_Lookup(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ধ্রুবক শূন্য *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    রাশিমালা_Def *বাম, *ডান;
    ধ্রুবক অক্ষর *bogus = ফাঁকা;

    নির্বাচন (রাশিমালা->common.type) {
    ক্ষেত্র STMT_রাশিমালা_INTEGER_LITERAL:
        যদি (রাশিমালা->integer.ival < 0 || রাশিমালা->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    রাশিমালা->integer.ival < 0 ? "-" : "",
                    imaxabs(রাশিমালা->integer.ival),
                    UINT32_MAX);
            ফেরত false;
        }
        *val_rtrn = (uint32_t) রাশিমালা->integer.ival;
        ফেরত true;

    ক্ষেত্র STMT_রাশিমালা_STRING_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_FLOAT_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_BOOLEAN_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_KEYNAME_LITERAL:
    ক্ষেত্র STMT_রাশিমালা_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(রাশিমালা->common.type));
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_IDENT:
        ok = lookup(ctx, lookupPriv, রাশিমালা->ident.ident, val_rtrn, pending);
        যদি (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, রাশিমালা->ident.ident));
        যদি (pending && *pending) {
            /* Postpone computation */
            ফেরত false;
        }
        ফেরত ok;

    ক্ষেত্র STMT_রাশিমালা_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, রাশিমালা->field_ref.element),
                xkb_atom_text(ctx, রাশিমালা->field_ref.field));
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    ক্ষেত্র STMT_রাশিমালা_ACTION_DECL:
        যদি (bogus == ফাঁকা)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        ফেরত false;

    ক্ষেত্র STMT_রাশিমালা_ADD:
    ক্ষেত্র STMT_রাশিমালা_SUBTRACT:
    ক্ষেত্র STMT_রাশিমালা_MULTIPLY:
    ক্ষেত্র STMT_রাশিমালা_DIVIDE:
        বাম = রাশিমালা->দ্বিমিক.বাম;
        ডান = রাশিমালা->দ্বিমিক.ডান;
        যদি (!রাশিমালা_Resolve_Mask_Lookup(ctx, বাম, &l, pending, lookup, lookupPriv) ||
            !রাশিমালা_Resolve_Mask_Lookup(ctx, ডান, &r, pending, lookup, lookupPriv))
            ফেরত false;

        নির্বাচন (রাশিমালা->common.type) {
        ক্ষেত্র STMT_রাশিমালা_ADD:
            *val_rtrn = l | r;
            বিরতি;
        ক্ষেত্র STMT_রাশিমালা_SUBTRACT:
            *val_rtrn = l & (~r);
            বিরতি;
        ক্ষেত্র STMT_রাশিমালা_MULTIPLY:
        ক্ষেত্র STMT_রাশিমালা_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (রাশিমালা->common.type == STMT_রাশিমালা_DIVIDE ? "divide" : "multiply"));
            ফেরত false;
        পূর্বনির্ধারিত:
            বিরতি;
        }

        ফেরত true;

    ক্ষেত্র STMT_রাশিমালা_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        বিরতি;

    ক্ষেত্র STMT_রাশিমালা_INVERT:
        বাম = রাশিমালা->unary.child;
        যদি (!রাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &v, pending, lookup, lookupPriv))
            ফেরত false;
        যদি (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            ফেরত false;
        }
        *val_rtrn = ~(uint32_t) v;
        ফেরত true;

    ক্ষেত্র STMT_রাশিমালা_UNARY_PLUS:
    ক্ষেত্র STMT_রাশিমালা_NEGATE:
    ক্ষেত্র STMT_রাশিমালা_NOT:
        বাম = রাশিমালা->unary.child;
        যদি (!রাশিমালা_Resolve_Integer_Lookup(ctx, বাম, &v, pending, lookup, lookupPriv))
            ফেরত false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(রাশিমালা->common.type));
        ফেরত false;

    পূর্বনির্ধারিত:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 রাশিমালা->common.type);
        বিরতি;
    }

    ফেরত false;
}

bool
রাশিমালা_Resolve_Mask(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                uint32_t *mask_rtrn, ধ্রুবক LookupEntry *মানসমূহ)
{
    ফেরত রাশিমালা_Resolve_Mask_Lookup(ctx, রাশিমালা, mask_rtrn, ফাঁকা, SimpleLookup, মানসমূহ);
}

bool
রাশিমালা_Resolve_Mod_Mask(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *রাশিমালা,
                   গণনা mod_type mod_type, ধ্রুবক কাঠামো xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    ফেরত রাশিমালা_Resolve_Mask_Lookup(ctx, রাশিমালা, mask_rtrn, ফাঁকা, LookupModMask, &priv);
}

bool
রাশিমালা_Resolve_Mod(কাঠামো xkb_context *ctx, ধ্রুবক রাশিমালা_Def *def,
               গণনা mod_type mod_type, ধ্রুবক কাঠামো xkb_mod_set *mods,
               xkb_mod_সূচক_t *ndx_rtrn)
{
    যদি (def->common.type != STMT_রাশিমালা_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        ফেরত false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_সূচক_t ndx = Xkb_Mod_Name_To_সূচক(mods, name, mod_type);
    যদি (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        ফেরত false;
    }

    *ndx_rtrn = ndx;
    ফেরত true;
}

bool
রাশিমালা_Resolve_Group_Mask(ধ্রুবক কাঠামো xkb_keymap_info *keymap_info,
                     ধ্রুবক রাশিমালা_Def *রাশিমালা, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    স্থির ধ্রুবক LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_সূচক_NAME, 0 },
        { ফাঁকা, 0 }
    };

    ধ্রুবক কাঠামো named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_দৈর্ঘ্য = আকার("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_সূচক_2
    };

    ফেরত রাশিমালা_Resolve_Mask_Lookup(keymap_info->keymap.ctx, রাশিমালা, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
