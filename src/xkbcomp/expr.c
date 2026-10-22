/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdint.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "expr.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "keysym.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "text.h"
#அந்தர்பாவ "xkbcomp/ast.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "util-numbers.h"
#அந்தர்பாவ "util-checked-arithmetic.h"

வகைவரை bool (*IdentLookupFunc)(கட்டமைப்பு xkb_context *ctx, மாறிலி வெறுமை *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
கோவை_Resolve_Lhs(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
               மாறிலி எழுத்து **elem_rtrn, மாறிலி எழுத்து **field_rtrn,
               கோவை_Def **index_rtrn)
{
    தேர்வு (கோவை->common.type) {
    நிலை STMT_கோவை_IDENT:
        *elem_rtrn = வெற்று;
        *field_rtrn = xkb_atom_text(ctx, கோவை->ident.ident);
        *index_rtrn = வெற்று;
        திரும்பு (*field_rtrn != வெற்று);
    நிலை STMT_கோவை_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, கோவை->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, கோவை->field_ref.field);
        *index_rtrn = வெற்று;
        திரும்பு (*elem_rtrn != வெற்று && *field_rtrn != வெற்று);
    நிலை STMT_கோவை_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, கோவை->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, கோவை->array_ref.field);
        *index_rtrn = கோவை->array_ref.entry;
        எனில் (கோவை->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == வெற்று)
            திரும்பு false;
        எனில் (*field_rtrn == வெற்று)
            திரும்பு false;
        திரும்பு true;
    இயல்பு:
        நிறுத்து;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", கோவை->common.type);
    திரும்பு false;
}

நிலையான bool
SimpleLookup(கட்டமைப்பு xkb_context *ctx, மாறிலி வெறுமை *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    எனில் (!priv || field == XKB_ATOM_NONE)
        திரும்பு false;

    மாறிலி எழுத்து *str = xkb_atom_text(ctx, field);
    க்கு (மாறிலி LookupEntry *entry = priv; entry && entry->name; entry++) {
        எனில் (istreq(str, entry->name)) {
            *val_rtrn = entry->மதிப்பு;
            திரும்பு true;
        }
    }

    திரும்பு false;
}

கட்டமைப்பு named_integer_pattern {
    மாறிலி எழுத்து *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    மாறிலி LookupEntry *entries;
    மாறிலி LookupEntry *pending_entries;
    bool is_mask;
    எண்ணிக்கை xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
நிலையான bool
NamedIntegerPatternLookup(கட்டமைப்பு xkb_context *ctx, மாறிலி வெறுமை *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    எனில் (!priv || field == XKB_ATOM_NONE)
        திரும்பு false;

    மாறிலி எழுத்து * மாறிலி str = xkb_atom_text(ctx, field);
    மாறிலி கட்டமைப்பு named_integer_pattern * மாறிலி pattern = priv;

    மாறிலி முழு மொத்தம் = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    எனில் (மொத்தம் > 0 && *(str + pattern->prefix_length + மொத்தம்) == '\0') {
        எனில் (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            திரும்பு false;
        }
        எனில் (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            ப்ரதிஜ்ஞா(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        திரும்பு true;
    } இல்லையெனில் {
        எனில் (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, வெற்று)) {
            திரும்பு true;
        }
        எனில் (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, வெற்று)) {
            *pending_rtrn = true;
            திரும்பு true;
        }
        திரும்பு false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
வகைவரை கட்டமைப்பு {
    மாறிலி கட்டமைப்பு xkb_mod_set *mods;
    எண்ணிக்கை mod_type mod_type;
} LookupModMaskPriv;

நிலையான bool
LookupModMask(கட்டமைப்பு xkb_context *ctx, மாறிலி வெறுமை *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    மாறிலி எழுத்து *str = xkb_atom_text(ctx, field);
    எனில் (!str)
        திரும்பு false;

    எனில் (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        திரும்பு true;
    }

    எனில் (istreq(str, "none")) {
        *val_rtrn = 0;
        திரும்பு true;
    }

    மாறிலி LookupModMaskPriv *arg = priv;
    மாறிலி கட்டமைப்பு xkb_mod_set *mods = arg->mods;
    மாறிலி எண்ணிக்கை mod_type mod_type = arg->mod_type;
    மாறிலி xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    எனில் (ndx == XKB_MOD_INVALID)
        திரும்பு false;

    *val_rtrn = (UINT32_C(1) << ndx);
    திரும்பு true;
}

bool
கோவை_Resolve_Boolean(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                   bool *set_rtrn)
{
    bool ok = false;
    மாறிலி எழுத்து *ident;

    தேர்வு (கோவை->common.type) {
    நிலை STMT_கோவை_BOOLEAN_LITERAL:
        *set_rtrn = கோவை->boolean.set;
        திரும்பு true;

    நிலை STMT_கோவை_STRING_LITERAL:
    நிலை STMT_கோவை_INTEGER_LITERAL:
    நிலை STMT_கோவை_FLOAT_LITERAL:
    நிலை STMT_கோவை_KEYNAME_LITERAL:
    நிலை STMT_கோவை_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(கோவை->common.type));
        திரும்பு false;

    நிலை STMT_கோவை_IDENT:
        ident = xkb_atom_text(ctx, கோவை->ident.ident);
        எனில் (ident) {
            எனில் (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                திரும்பு true;
            }
            இல்லையெனில் எனில் (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                திரும்பு true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        திரும்பு false;

    நிலை STMT_கோவை_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, கோவை->field_ref.element),
                xkb_atom_text(ctx, கோவை->field_ref.field));
        திரும்பு false;

    நிலை STMT_கோவை_INVERT:
    நிலை STMT_கோவை_NOT:
        ok = கோவை_Resolve_Boolean(ctx, கோவை->unary.child, set_rtrn);
        எனில் (ok)
            *set_rtrn = !*set_rtrn;
        திரும்பு ok;
    நிலை STMT_கோவை_ADD:
    நிலை STMT_கோவை_SUBTRACT:
    நிலை STMT_கோவை_MULTIPLY:
    நிலை STMT_கோவை_DIVIDE:
    நிலை STMT_கோவை_ASSIGN:
    நிலை STMT_கோவை_NEGATE:
    நிலை STMT_கோவை_UNARY_PLUS:
    நிலை STMT_கோவை_EMPTY_LIST:
    நிலை STMT_கோவை_ACTION_DECL:
    நிலை STMT_கோவை_ACTION_LIST:
    நிலை STMT_கோவை_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(கோவை->common.type));
        நிறுத்து;

    இயல்பு:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 கோவை->common.type);
        நிறுத்து;
    }

    திரும்பு false;
}

நிலையான bool
கோவை_Resolve_Integer_Lookup(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, மாறிலி வெறுமை *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    கோவை_Def *இடது, *வலது;

    தேர்வு (கோவை->common.type) {
    நிலை STMT_கோவை_INTEGER_LITERAL:
        *val_rtrn = கோவை->integer.ival;
        திரும்பு true;

    நிலை STMT_கோவை_STRING_LITERAL:
    நிலை STMT_கோவை_FLOAT_LITERAL:
    நிலை STMT_கோவை_BOOLEAN_LITERAL:
    நிலை STMT_கோவை_KEYNAME_LITERAL:
    நிலை STMT_கோவை_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(கோவை->common.type));
        திரும்பு false;

    நிலை STMT_கோவை_IDENT:
        எனில் (lookup)
            ok = lookup(ctx, lookupPriv, கோவை->ident.ident, &u, pending);

        எனில் (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, கோவை->ident.ident));
        இல்லையெனில்
            *val_rtrn = (int64_t) u;

        எனில் (pending && *pending) {
            /* Postpone computation */
            திரும்பு false;
        }

        திரும்பு ok;

    நிலை STMT_கோவை_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, கோவை->field_ref.element),
                xkb_atom_text(ctx, கோவை->field_ref.field));
        திரும்பு false;

    நிலை STMT_கோவை_ADD:
    நிலை STMT_கோவை_SUBTRACT:
    நிலை STMT_கோவை_MULTIPLY:
    நிலை STMT_கோவை_DIVIDE:
        இடது = கோவை->இருமம்.இடது;
        வலது = கோவை->இருமம்.வலது;
        எனில் (!கோவை_Resolve_Integer_Lookup(ctx, இடது, &l, pending, lookup, lookupPriv) ||
            !கோவை_Resolve_Integer_Lookup(ctx, வலது, &r, pending, lookup, lookupPriv))
            திரும்பு false;

        தேர்வு (கோவை->common.type) {
        நிலை STMT_கோவை_ADD:
            எனில் (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                திரும்பு false;
            }
            நிறுத்து;
        நிலை STMT_கோவை_SUBTRACT:
            எனில் (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                திரும்பு false;
            }
            நிறுத்து;
        நிலை STMT_கோவை_MULTIPLY:
            எனில் (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                திரும்பு false;
            }
            நிறுத்து;
        நிலை STMT_கோவை_DIVIDE:
            எனில் (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                திரும்பு false;
            }
            *val_rtrn = l / r;
            நிறுத்து;
        இயல்பு:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(கோவை->common.type));
            திரும்பு false;
        }

        திரும்பு true;

    நிலை STMT_கோவை_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        நிறுத்து;

    நிலை STMT_கோவை_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        திரும்பு false;

    நிலை STMT_கோவை_INVERT:
    நிலை STMT_கோவை_NEGATE:
        இடது = கோவை->unary.child;
        எனில் (!கோவை_Resolve_Integer_Lookup(ctx, இடது, &l, pending, lookup, lookupPriv))
            திரும்பு false;

        *val_rtrn = (கோவை->common.type == STMT_கோவை_NEGATE ? -l : ~l);
        திரும்பு true;

    நிலை STMT_கோவை_UNARY_PLUS:
        இடது = கோவை->unary.child;
        திரும்பு கோவை_Resolve_Integer_Lookup(ctx, இடது, val_rtrn, pending, lookup,
                                        lookupPriv);

    இயல்பு:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 கோவை->common.type);
        நிறுத்து;
    }

    திரும்பு false;
}

bool
கோவை_Resolve_Integer(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                   int64_t *val_rtrn)
{
    திரும்பு கோவை_Resolve_Integer_Lookup(ctx, கோவை, val_rtrn, வெற்று, வெற்று, வெற்று);
}

எண்ணிக்கை xkb_parser_error
கோவை_Resolve_Group(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                 மாறிலி கோவை_Def *கோவை, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    நிலையான மாறிலி LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { வெற்று, 0 }
    };

    மாறிலி கட்டமைப்பு named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = அளவு("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    எனில் (!கோவை_Resolve_Integer_Lookup(keymap_info->keymap.ctx, கோவை, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    எனில் (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    திரும்பு PARSER_SUCCESS;
}

bool
கோவை_Resolve_Level(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                 xkb_level_index_t *level_rtrn)
{
    நிலையான மாறிலி கட்டமைப்பு named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = அளவு("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = வெற்று,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    எனில் (!கோவை_Resolve_Integer_Lookup(ctx, கோவை, &result, வெற்று,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        திரும்பு false;

    எனில் (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        திரும்பு false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    திரும்பு true;
}

bool
கோவை_Resolve_Button(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை, int64_t *btn_rtrn)
{
    திரும்பு கோவை_Resolve_Integer_Lookup(ctx, கோவை, btn_rtrn, வெற்று, SimpleLookup,
                                    buttonNames);
}

bool
கோவை_Resolve_String(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                  xkb_atom_t *val_rtrn)
{
    தேர்வு (கோவை->common.type) {
    நிலை STMT_கோவை_STRING_LITERAL:
        *val_rtrn = கோவை->string.str;
        திரும்பு true;

    நிலை STMT_கோவை_INTEGER_LITERAL:
    நிலை STMT_கோவை_FLOAT_LITERAL:
    நிலை STMT_கோவை_BOOLEAN_LITERAL:
    நிலை STMT_கோவை_KEYNAME_LITERAL:
    நிலை STMT_கோவை_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(கோவை->common.type));
        திரும்பு false;

    நிலை STMT_கோவை_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, கோவை->ident.ident));
        திரும்பு false;

    நிலை STMT_கோவை_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, கோவை->field_ref.element),
                xkb_atom_text(ctx, கோவை->field_ref.field));
        திரும்பு false;

    நிலை STMT_கோவை_ADD:
    நிலை STMT_கோவை_SUBTRACT:
    நிலை STMT_கோவை_MULTIPLY:
    நிலை STMT_கோவை_DIVIDE:
    நிலை STMT_கோவை_ASSIGN:
    நிலை STMT_கோவை_NEGATE:
    நிலை STMT_கோவை_INVERT:
    நிலை STMT_கோவை_NOT:
    நிலை STMT_கோவை_UNARY_PLUS:
    நிலை STMT_கோவை_EMPTY_LIST:
    நிலை STMT_கோவை_ACTION_DECL:
    நிலை STMT_கோவை_ACTION_LIST:
    நிலை STMT_கோவை_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(கோவை->common.type));
        திரும்பு false;

    இயல்பு:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 கோவை->common.type);
        நிறுத்து;
    }
    திரும்பு false;
}

bool
கோவை_Resolve_Enum(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                uint32_t *val_rtrn, மாறிலி LookupEntry *மதிப்புகள்)
{
    எனில் (கோவை->common.type != STMT_கோவை_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(கோவை->common.type));
        திரும்பு false;
    }

    எனில் (!SimpleLookup(ctx, மதிப்புகள், கோவை->ident.ident, val_rtrn, வெற்று)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, கோவை->ident.ident));
        போது (மதிப்புகள் && மதிப்புகள்->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", மதிப்புகள்->name);
            மதிப்புகள்++;
        }
        திரும்பு false;
    }

    திரும்பு true;
}

நிலையான bool
கோவை_Resolve_Mask_Lookup(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, மாறிலி வெறுமை *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    கோவை_Def *இடது, *வலது;
    மாறிலி எழுத்து *bogus = வெற்று;

    தேர்வு (கோவை->common.type) {
    நிலை STMT_கோவை_INTEGER_LITERAL:
        எனில் (கோவை->integer.ival < 0 || கோவை->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    கோவை->integer.ival < 0 ? "-" : "",
                    imaxabs(கோவை->integer.ival),
                    UINT32_MAX);
            திரும்பு false;
        }
        *val_rtrn = (uint32_t) கோவை->integer.ival;
        திரும்பு true;

    நிலை STMT_கோவை_STRING_LITERAL:
    நிலை STMT_கோவை_FLOAT_LITERAL:
    நிலை STMT_கோவை_BOOLEAN_LITERAL:
    நிலை STMT_கோவை_KEYNAME_LITERAL:
    நிலை STMT_கோவை_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(கோவை->common.type));
        திரும்பு false;

    நிலை STMT_கோவை_IDENT:
        ok = lookup(ctx, lookupPriv, கோவை->ident.ident, val_rtrn, pending);
        எனில் (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, கோவை->ident.ident));
        எனில் (pending && *pending) {
            /* Postpone computation */
            திரும்பு false;
        }
        திரும்பு ok;

    நிலை STMT_கோவை_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, கோவை->field_ref.element),
                xkb_atom_text(ctx, கோவை->field_ref.field));
        திரும்பு false;

    நிலை STMT_கோவை_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    நிலை STMT_கோவை_ACTION_DECL:
        எனில் (bogus == வெற்று)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        திரும்பு false;

    நிலை STMT_கோவை_ADD:
    நிலை STMT_கோவை_SUBTRACT:
    நிலை STMT_கோவை_MULTIPLY:
    நிலை STMT_கோவை_DIVIDE:
        இடது = கோவை->இருமம்.இடது;
        வலது = கோவை->இருமம்.வலது;
        எனில் (!கோவை_Resolve_Mask_Lookup(ctx, இடது, &l, pending, lookup, lookupPriv) ||
            !கோவை_Resolve_Mask_Lookup(ctx, வலது, &r, pending, lookup, lookupPriv))
            திரும்பு false;

        தேர்வு (கோவை->common.type) {
        நிலை STMT_கோவை_ADD:
            *val_rtrn = l | r;
            நிறுத்து;
        நிலை STMT_கோவை_SUBTRACT:
            *val_rtrn = l & (~r);
            நிறுத்து;
        நிலை STMT_கோவை_MULTIPLY:
        நிலை STMT_கோவை_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (கோவை->common.type == STMT_கோவை_DIVIDE ? "divide" : "multiply"));
            திரும்பு false;
        இயல்பு:
            நிறுத்து;
        }

        திரும்பு true;

    நிலை STMT_கோவை_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        நிறுத்து;

    நிலை STMT_கோவை_INVERT:
        இடது = கோவை->unary.child;
        எனில் (!கோவை_Resolve_Integer_Lookup(ctx, இடது, &v, pending, lookup, lookupPriv))
            திரும்பு false;
        எனில் (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            திரும்பு false;
        }
        *val_rtrn = ~(uint32_t) v;
        திரும்பு true;

    நிலை STMT_கோவை_UNARY_PLUS:
    நிலை STMT_கோவை_NEGATE:
    நிலை STMT_கோவை_NOT:
        இடது = கோவை->unary.child;
        எனில் (!கோவை_Resolve_Integer_Lookup(ctx, இடது, &v, pending, lookup, lookupPriv))
            திரும்பு false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(கோவை->common.type));
        திரும்பு false;

    இயல்பு:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 கோவை->common.type);
        நிறுத்து;
    }

    திரும்பு false;
}

bool
கோவை_Resolve_Mask(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                uint32_t *mask_rtrn, மாறிலி LookupEntry *மதிப்புகள்)
{
    திரும்பு கோவை_Resolve_Mask_Lookup(ctx, கோவை, mask_rtrn, வெற்று, SimpleLookup, மதிப்புகள்);
}

bool
கோவை_Resolve_Mod_Mask(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *கோவை,
                   எண்ணிக்கை mod_type mod_type, மாறிலி கட்டமைப்பு xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    திரும்பு கோவை_Resolve_Mask_Lookup(ctx, கோவை, mask_rtrn, வெற்று, LookupModMask, &priv);
}

bool
கோவை_Resolve_Mod(கட்டமைப்பு xkb_context *ctx, மாறிலி கோவை_Def *def,
               எண்ணிக்கை mod_type mod_type, மாறிலி கட்டமைப்பு xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    எனில் (def->common.type != STMT_கோவை_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        திரும்பு false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    எனில் (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        திரும்பு false;
    }

    *ndx_rtrn = ndx;
    திரும்பு true;
}

bool
கோவை_Resolve_Group_Mask(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                     மாறிலி கோவை_Def *கோவை, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    நிலையான மாறிலி LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { வெற்று, 0 }
    };

    மாறிலி கட்டமைப்பு named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = அளவு("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    திரும்பு கோவை_Resolve_Mask_Lookup(keymap_info->keymap.ctx, கோவை, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
