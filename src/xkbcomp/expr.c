/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdint.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "expr.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "keysym.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "xkbcomp/ast.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-numbers.h"
#अन्तर्भाव "util-checked-arithmetic.h"

प्रकारनिर्देश bool (*IdentLookupFunc)(संरचना xkb_context *ctx, ध्रुव शून्य *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
व्यञ्जकम्_Resolve_Lhs(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
               ध्रुव वर्ण **elem_rtrn, ध्रुव वर्ण **field_rtrn,
               व्यञ्जकम्_Def **index_rtrn)
{
    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_IDENT:
        *elem_rtrn = रिक्त;
        *field_rtrn = xkb_atom_text(ctx, व्यञ्जकम्->ident.ident);
        *index_rtrn = रिक्त;
        पटिनिवत्त (*field_rtrn != रिक्त);
    पक्ष STMT_व्यञ्जकम्_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, व्यञ्जकम्->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, व्यञ्जकम्->field_ref.field);
        *index_rtrn = रिक्त;
        पटिनिवत्त (*elem_rtrn != रिक्त && *field_rtrn != रिक्त);
    पक्ष STMT_व्यञ्जकम्_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, व्यञ्जकम्->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, व्यञ्जकम्->array_ref.field);
        *index_rtrn = व्यञ्जकम्->array_ref.entry;
        सचे (व्यञ्जकम्->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == रिक्त)
            पटिनिवत्त false;
        सचे (*field_rtrn == रिक्त)
            पटिनिवत्त false;
        पटिनिवत्त true;
    स्वभाव:
        भङ्ग;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", व्यञ्जकम्->common.type);
    पटिनिवत्त false;
}

स्थिर bool
SimpleLookup(संरचना xkb_context *ctx, ध्रुव शून्य *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    सचे (!priv || field == XKB_ATOM_NONE)
        पटिनिवत्त false;

    ध्रुव वर्ण *str = xkb_atom_text(ctx, field);
    अत्थाय (ध्रुव LookupEntry *entry = priv; entry && entry->name; entry++) {
        सचे (istreq(str, entry->name)) {
            *val_rtrn = entry->मानम्;
            पटिनिवत्त true;
        }
    }

    पटिनिवत्त false;
}

संरचना named_integer_pattern {
    ध्रुव वर्ण *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    ध्रुव LookupEntry *entries;
    ध्रुव LookupEntry *pending_entries;
    bool is_mask;
    गणना xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
स्थिर bool
NamedIntegerPatternLookup(संरचना xkb_context *ctx, ध्रुव शून्य *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    सचे (!priv || field == XKB_ATOM_NONE)
        पटिनिवत्त false;

    ध्रुव वर्ण * ध्रुव str = xkb_atom_text(ctx, field);
    ध्रुव संरचना named_integer_pattern * ध्रुव pattern = priv;

    ध्रुव पूर्णाङ्क गणनम् = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    सचे (गणनम् > 0 && *(str + pattern->prefix_length + गणनम्) == '\0') {
        सचे (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            पटिनिवत्त false;
        }
        सचे (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            प्रतिज्ञा(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        पटिनिवत्त true;
    } अञ्ञथा {
        सचे (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, रिक्त)) {
            पटिनिवत्त true;
        }
        सचे (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, रिक्त)) {
            *pending_rtrn = true;
            पटिनिवत्त true;
        }
        पटिनिवत्त false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
प्रकारनिर्देश संरचना {
    ध्रुव संरचना xkb_mod_set *mods;
    गणना mod_type mod_type;
} LookupModMaskPriv;

स्थिर bool
LookupModMask(संरचना xkb_context *ctx, ध्रुव शून्य *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    ध्रुव वर्ण *str = xkb_atom_text(ctx, field);
    सचे (!str)
        पटिनिवत्त false;

    सचे (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        पटिनिवत्त true;
    }

    सचे (istreq(str, "none")) {
        *val_rtrn = 0;
        पटिनिवत्त true;
    }

    ध्रुव LookupModMaskPriv *arg = priv;
    ध्रुव संरचना xkb_mod_set *mods = arg->mods;
    ध्रुव गणना mod_type mod_type = arg->mod_type;
    ध्रुव xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    सचे (ndx == XKB_MOD_INVALID)
        पटिनिवत्त false;

    *val_rtrn = (UINT32_C(1) << ndx);
    पटिनिवत्त true;
}

bool
व्यञ्जकम्_Resolve_Boolean(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   bool *set_rtrn)
{
    bool ok = false;
    ध्रुव वर्ण *ident;

    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_BOOLEAN_LITERAL:
        *set_rtrn = व्यञ्जकम्->boolean.set;
        पटिनिवत्त true;

    पक्ष STMT_व्यञ्जकम्_STRING_LITERAL:
    पक्ष STMT_व्यञ्जकम्_INTEGER_LITERAL:
    पक्ष STMT_व्यञ्जकम्_FLOAT_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(व्यञ्जकम्->common.type));
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_IDENT:
        ident = xkb_atom_text(ctx, व्यञ्जकम्->ident.ident);
        सचे (ident) {
            सचे (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                पटिनिवत्त true;
            }
            अञ्ञथा सचे (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                पटिनिवत्त true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, व्यञ्जकम्->field_ref.element),
                xkb_atom_text(ctx, व्यञ्जकम्->field_ref.field));
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_INVERT:
    पक्ष STMT_व्यञ्जकम्_NOT:
        ok = व्यञ्जकम्_Resolve_Boolean(ctx, व्यञ्जकम्->unary.child, set_rtrn);
        सचे (ok)
            *set_rtrn = !*set_rtrn;
        पटिनिवत्त ok;
    पक्ष STMT_व्यञ्जकम्_ADD:
    पक्ष STMT_व्यञ्जकम्_SUBTRACT:
    पक्ष STMT_व्यञ्जकम्_MULTIPLY:
    पक्ष STMT_व्यञ्जकम्_DIVIDE:
    पक्ष STMT_व्यञ्जकम्_ASSIGN:
    पक्ष STMT_व्यञ्जकम्_NEGATE:
    पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
    पक्ष STMT_व्यञ्जकम्_EMPTY_LIST:
    पक्ष STMT_व्यञ्जकम्_ACTION_DECL:
    पक्ष STMT_व्यञ्जकम्_ACTION_LIST:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(व्यञ्जकम्->common.type));
        भङ्ग;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 व्यञ्जकम्->common.type);
        भङ्ग;
    }

    पटिनिवत्त false;
}

स्थिर bool
व्यञ्जकम्_Resolve_Integer_Lookup(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ध्रुव शून्य *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    व्यञ्जकम्_Def *वामः, *दक्षिणः;

    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_INTEGER_LITERAL:
        *val_rtrn = व्यञ्जकम्->integer.ival;
        पटिनिवत्त true;

    पक्ष STMT_व्यञ्जकम्_STRING_LITERAL:
    पक्ष STMT_व्यञ्जकम्_FLOAT_LITERAL:
    पक्ष STMT_व्यञ्जकम्_BOOLEAN_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(व्यञ्जकम्->common.type));
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_IDENT:
        सचे (lookup)
            ok = lookup(ctx, lookupPriv, व्यञ्जकम्->ident.ident, &u, pending);

        सचे (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, व्यञ्जकम्->ident.ident));
        अञ्ञथा
            *val_rtrn = (int64_t) u;

        सचे (pending && *pending) {
            /* Postpone computation */
            पटिनिवत्त false;
        }

        पटिनिवत्त ok;

    पक्ष STMT_व्यञ्जकम्_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, व्यञ्जकम्->field_ref.element),
                xkb_atom_text(ctx, व्यञ्जकम्->field_ref.field));
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_ADD:
    पक्ष STMT_व्यञ्जकम्_SUBTRACT:
    पक्ष STMT_व्यञ्जकम्_MULTIPLY:
    पक्ष STMT_व्यञ्जकम्_DIVIDE:
        वामः = व्यञ्जकम्->द्विमान.वामः;
        दक्षिणः = व्यञ्जकम्->द्विमान.दक्षिणः;
        सचे (!व्यञ्जकम्_Resolve_Integer_Lookup(ctx, वामः, &l, pending, lookup, lookupPriv) ||
            !व्यञ्जकम्_Resolve_Integer_Lookup(ctx, दक्षिणः, &r, pending, lookup, lookupPriv))
            पटिनिवत्त false;

        विकल्प (व्यञ्जकम्->common.type) {
        पक्ष STMT_व्यञ्जकम्_ADD:
            सचे (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                पटिनिवत्त false;
            }
            भङ्ग;
        पक्ष STMT_व्यञ्जकम्_SUBTRACT:
            सचे (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                पटिनिवत्त false;
            }
            भङ्ग;
        पक्ष STMT_व्यञ्जकम्_MULTIPLY:
            सचे (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                पटिनिवत्त false;
            }
            भङ्ग;
        पक्ष STMT_व्यञ्जकम्_DIVIDE:
            सचे (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                पटिनिवत्त false;
            }
            *val_rtrn = l / r;
            भङ्ग;
        स्वभाव:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(व्यञ्जकम्->common.type));
            पटिनिवत्त false;
        }

        पटिनिवत्त true;

    पक्ष STMT_व्यञ्जकम्_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        भङ्ग;

    पक्ष STMT_व्यञ्जकम्_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_INVERT:
    पक्ष STMT_व्यञ्जकम्_NEGATE:
        वामः = व्यञ्जकम्->unary.child;
        सचे (!व्यञ्जकम्_Resolve_Integer_Lookup(ctx, वामः, &l, pending, lookup, lookupPriv))
            पटिनिवत्त false;

        *val_rtrn = (व्यञ्जकम्->common.type == STMT_व्यञ्जकम्_NEGATE ? -l : ~l);
        पटिनिवत्त true;

    पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
        वामः = व्यञ्जकम्->unary.child;
        पटिनिवत्त व्यञ्जकम्_Resolve_Integer_Lookup(ctx, वामः, val_rtrn, pending, lookup,
                                        lookupPriv);

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 व्यञ्जकम्->common.type);
        भङ्ग;
    }

    पटिनिवत्त false;
}

bool
व्यञ्जकम्_Resolve_Integer(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   int64_t *val_rtrn)
{
    पटिनिवत्त व्यञ्जकम्_Resolve_Integer_Lookup(ctx, व्यञ्जकम्, val_rtrn, रिक्त, रिक्त, रिक्त);
}

गणना xkb_parser_error
व्यञ्जकम्_Resolve_Group(ध्रुव संरचना xkb_keymap_info *keymap_info,
                 ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    स्थिर ध्रुव LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { रिक्त, 0 }
    };

    ध्रुव संरचना named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = परिमाण("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    सचे (!व्यञ्जकम्_Resolve_Integer_Lookup(keymap_info->keymap.ctx, व्यञ्जकम्, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        पटिनिवत्त (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    सचे (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        पटिनिवत्त (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    पटिनिवत्त PARSER_SUCCESS;
}

bool
व्यञ्जकम्_Resolve_Level(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                 xkb_level_index_t *level_rtrn)
{
    स्थिर ध्रुव संरचना named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = परिमाण("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = रिक्त,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    सचे (!व्यञ्जकम्_Resolve_Integer_Lookup(ctx, व्यञ्जकम्, &result, रिक्त,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        पटिनिवत्त false;

    सचे (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        पटिनिवत्त false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    पटिनिवत्त true;
}

bool
व्यञ्जकम्_Resolve_Button(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, int64_t *btn_rtrn)
{
    पटिनिवत्त व्यञ्जकम्_Resolve_Integer_Lookup(ctx, व्यञ्जकम्, btn_rtrn, रिक्त, SimpleLookup,
                                    buttonNames);
}

bool
व्यञ्जकम्_Resolve_String(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                  xkb_atom_t *val_rtrn)
{
    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_STRING_LITERAL:
        *val_rtrn = व्यञ्जकम्->string.str;
        पटिनिवत्त true;

    पक्ष STMT_व्यञ्जकम्_INTEGER_LITERAL:
    पक्ष STMT_व्यञ्जकम्_FLOAT_LITERAL:
    पक्ष STMT_व्यञ्जकम्_BOOLEAN_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(व्यञ्जकम्->common.type));
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, व्यञ्जकम्->ident.ident));
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, व्यञ्जकम्->field_ref.element),
                xkb_atom_text(ctx, व्यञ्जकम्->field_ref.field));
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_ADD:
    पक्ष STMT_व्यञ्जकम्_SUBTRACT:
    पक्ष STMT_व्यञ्जकम्_MULTIPLY:
    पक्ष STMT_व्यञ्जकम्_DIVIDE:
    पक्ष STMT_व्यञ्जकम्_ASSIGN:
    पक्ष STMT_व्यञ्जकम्_NEGATE:
    पक्ष STMT_व्यञ्जकम्_INVERT:
    पक्ष STMT_व्यञ्जकम्_NOT:
    पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
    पक्ष STMT_व्यञ्जकम्_EMPTY_LIST:
    पक्ष STMT_व्यञ्जकम्_ACTION_DECL:
    पक्ष STMT_व्यञ्जकम्_ACTION_LIST:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(व्यञ्जकम्->common.type));
        पटिनिवत्त false;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 व्यञ्जकम्->common.type);
        भङ्ग;
    }
    पटिनिवत्त false;
}

bool
व्यञ्जकम्_Resolve_Enum(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                uint32_t *val_rtrn, ध्रुव LookupEntry *मानानि)
{
    सचे (व्यञ्जकम्->common.type != STMT_व्यञ्जकम्_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(व्यञ्जकम्->common.type));
        पटिनिवत्त false;
    }

    सचे (!SimpleLookup(ctx, मानानि, व्यञ्जकम्->ident.ident, val_rtrn, रिक्त)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, व्यञ्जकम्->ident.ident));
        याव (मानानि && मानानि->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", मानानि->name);
            मानानि++;
        }
        पटिनिवत्त false;
    }

    पटिनिवत्त true;
}

स्थिर bool
व्यञ्जकम्_Resolve_Mask_Lookup(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ध्रुव शून्य *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    व्यञ्जकम्_Def *वामः, *दक्षिणः;
    ध्रुव वर्ण *bogus = रिक्त;

    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_INTEGER_LITERAL:
        सचे (व्यञ्जकम्->integer.ival < 0 || व्यञ्जकम्->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    व्यञ्जकम्->integer.ival < 0 ? "-" : "",
                    imaxabs(व्यञ्जकम्->integer.ival),
                    UINT32_MAX);
            पटिनिवत्त false;
        }
        *val_rtrn = (uint32_t) व्यञ्जकम्->integer.ival;
        पटिनिवत्त true;

    पक्ष STMT_व्यञ्जकम्_STRING_LITERAL:
    पक्ष STMT_व्यञ्जकम्_FLOAT_LITERAL:
    पक्ष STMT_व्यञ्जकम्_BOOLEAN_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(व्यञ्जकम्->common.type));
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_IDENT:
        ok = lookup(ctx, lookupPriv, व्यञ्जकम्->ident.ident, val_rtrn, pending);
        सचे (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, व्यञ्जकम्->ident.ident));
        सचे (pending && *pending) {
            /* Postpone computation */
            पटिनिवत्त false;
        }
        पटिनिवत्त ok;

    पक्ष STMT_व्यञ्जकम्_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, व्यञ्जकम्->field_ref.element),
                xkb_atom_text(ctx, व्यञ्जकम्->field_ref.field));
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    पक्ष STMT_व्यञ्जकम्_ACTION_DECL:
        सचे (bogus == रिक्त)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        पटिनिवत्त false;

    पक्ष STMT_व्यञ्जकम्_ADD:
    पक्ष STMT_व्यञ्जकम्_SUBTRACT:
    पक्ष STMT_व्यञ्जकम्_MULTIPLY:
    पक्ष STMT_व्यञ्जकम्_DIVIDE:
        वामः = व्यञ्जकम्->द्विमान.वामः;
        दक्षिणः = व्यञ्जकम्->द्विमान.दक्षिणः;
        सचे (!व्यञ्जकम्_Resolve_Mask_Lookup(ctx, वामः, &l, pending, lookup, lookupPriv) ||
            !व्यञ्जकम्_Resolve_Mask_Lookup(ctx, दक्षिणः, &r, pending, lookup, lookupPriv))
            पटिनिवत्त false;

        विकल्प (व्यञ्जकम्->common.type) {
        पक्ष STMT_व्यञ्जकम्_ADD:
            *val_rtrn = l | r;
            भङ्ग;
        पक्ष STMT_व्यञ्जकम्_SUBTRACT:
            *val_rtrn = l & (~r);
            भङ्ग;
        पक्ष STMT_व्यञ्जकम्_MULTIPLY:
        पक्ष STMT_व्यञ्जकम्_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (व्यञ्जकम्->common.type == STMT_व्यञ्जकम्_DIVIDE ? "divide" : "multiply"));
            पटिनिवत्त false;
        स्वभाव:
            भङ्ग;
        }

        पटिनिवत्त true;

    पक्ष STMT_व्यञ्जकम्_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        भङ्ग;

    पक्ष STMT_व्यञ्जकम्_INVERT:
        वामः = व्यञ्जकम्->unary.child;
        सचे (!व्यञ्जकम्_Resolve_Integer_Lookup(ctx, वामः, &v, pending, lookup, lookupPriv))
            पटिनिवत्त false;
        सचे (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            पटिनिवत्त false;
        }
        *val_rtrn = ~(uint32_t) v;
        पटिनिवत्त true;

    पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
    पक्ष STMT_व्यञ्जकम्_NEGATE:
    पक्ष STMT_व्यञ्जकम्_NOT:
        वामः = व्यञ्जकम्->unary.child;
        सचे (!व्यञ्जकम्_Resolve_Integer_Lookup(ctx, वामः, &v, pending, lookup, lookupPriv))
            पटिनिवत्त false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(व्यञ्जकम्->common.type));
        पटिनिवत्त false;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 व्यञ्जकम्->common.type);
        भङ्ग;
    }

    पटिनिवत्त false;
}

bool
व्यञ्जकम्_Resolve_Mask(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                uint32_t *mask_rtrn, ध्रुव LookupEntry *मानानि)
{
    पटिनिवत्त व्यञ्जकम्_Resolve_Mask_Lookup(ctx, व्यञ्जकम्, mask_rtrn, रिक्त, SimpleLookup, मानानि);
}

bool
व्यञ्जकम्_Resolve_Mod_Mask(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   गणना mod_type mod_type, ध्रुव संरचना xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    पटिनिवत्त व्यञ्जकम्_Resolve_Mask_Lookup(ctx, व्यञ्जकम्, mask_rtrn, रिक्त, LookupModMask, &priv);
}

bool
व्यञ्जकम्_Resolve_Mod(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *def,
               गणना mod_type mod_type, ध्रुव संरचना xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    सचे (def->common.type != STMT_व्यञ्जकम्_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        पटिनिवत्त false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    सचे (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        पटिनिवत्त false;
    }

    *ndx_rtrn = ndx;
    पटिनिवत्त true;
}

bool
व्यञ्जकम्_Resolve_Group_Mask(ध्रुव संरचना xkb_keymap_info *keymap_info,
                     ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    स्थिर ध्रुव LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { रिक्त, 0 }
    };

    ध्रुव संरचना named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = परिमाण("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    पटिनिवत्त व्यञ्जकम्_Resolve_Mask_Lookup(keymap_info->keymap.ctx, व्यञ्जकम्, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
