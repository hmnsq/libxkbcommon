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
व्यंजक_Resolve_Lhs(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
               ध्रुव वर्ण **elem_rtrn, ध्रुव वर्ण **field_rtrn,
               व्यंजक_Def **index_rtrn)
{
    विकल्प (व्यंजक->common.type) {
    पक्ष STMT_व्यंजक_IDENT:
        *elem_rtrn = रिक्त;
        *field_rtrn = xkb_atom_text(ctx, व्यंजक->ident.ident);
        *index_rtrn = रिक्त;
        प्रत्यागम (*field_rtrn != रिक्त);
    पक्ष STMT_व्यंजक_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, व्यंजक->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, व्यंजक->field_ref.field);
        *index_rtrn = रिक्त;
        प्रत्यागम (*elem_rtrn != रिक्त && *field_rtrn != रिक्त);
    पक्ष STMT_व्यंजक_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, व्यंजक->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, व्यंजक->array_ref.field);
        *index_rtrn = व्यंजक->array_ref.entry;
        यदि (व्यंजक->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == रिक्त)
            प्रत्यागम false;
        यदि (*field_rtrn == रिक्त)
            प्रत्यागम false;
        प्रत्यागम true;
    स्वभाव:
        भङ्ग;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", व्यंजक->common.type);
    प्रत्यागम false;
}

स्थिर bool
SimpleLookup(संरचना xkb_context *ctx, ध्रुव शून्य *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    यदि (!priv || field == XKB_ATOM_NONE)
        प्रत्यागम false;

    ध्रुव वर्ण *str = xkb_atom_text(ctx, field);
    प्रति (ध्रुव LookupEntry *entry = priv; entry && entry->name; entry++) {
        यदि (istreq(str, entry->name)) {
            *val_rtrn = entry->मूल्य;
            प्रत्यागम true;
        }
    }

    प्रत्यागम false;
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
    यदि (!priv || field == XKB_ATOM_NONE)
        प्रत्यागम false;

    ध्रुव वर्ण * ध्रुव str = xkb_atom_text(ctx, field);
    ध्रुव संरचना named_integer_pattern * ध्रुव pattern = priv;

    ध्रुव पूर्णाङ्क गिनती = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    यदि (गिनती > 0 && *(str + pattern->prefix_length + गिनती) == '\0') {
        यदि (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            प्रत्यागम false;
        }
        यदि (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            प्रतिज्ञा(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        प्रत्यागम true;
    } अन्यथा {
        यदि (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, रिक्त)) {
            प्रत्यागम true;
        }
        यदि (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, रिक्त)) {
            *pending_rtrn = true;
            प्रत्यागम true;
        }
        प्रत्यागम false;
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
    यदि (!str)
        प्रत्यागम false;

    यदि (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        प्रत्यागम true;
    }

    यदि (istreq(str, "none")) {
        *val_rtrn = 0;
        प्रत्यागम true;
    }

    ध्रुव LookupModMaskPriv *arg = priv;
    ध्रुव संरचना xkb_mod_set *mods = arg->mods;
    ध्रुव गणना mod_type mod_type = arg->mod_type;
    ध्रुव xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    यदि (ndx == XKB_MOD_INVALID)
        प्रत्यागम false;

    *val_rtrn = (UINT32_C(1) << ndx);
    प्रत्यागम true;
}

bool
व्यंजक_Resolve_Boolean(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                   bool *set_rtrn)
{
    bool ok = false;
    ध्रुव वर्ण *ident;

    विकल्प (व्यंजक->common.type) {
    पक्ष STMT_व्यंजक_BOOLEAN_LITERAL:
        *set_rtrn = व्यंजक->boolean.set;
        प्रत्यागम true;

    पक्ष STMT_व्यंजक_STRING_LITERAL:
    पक्ष STMT_व्यंजक_INTEGER_LITERAL:
    पक्ष STMT_व्यंजक_FLOAT_LITERAL:
    पक्ष STMT_व्यंजक_KEYNAME_LITERAL:
    पक्ष STMT_व्यंजक_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(व्यंजक->common.type));
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_IDENT:
        ident = xkb_atom_text(ctx, व्यंजक->ident.ident);
        यदि (ident) {
            यदि (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                प्रत्यागम true;
            }
            अन्यथा यदि (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                प्रत्यागम true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, व्यंजक->field_ref.element),
                xkb_atom_text(ctx, व्यंजक->field_ref.field));
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_INVERT:
    पक्ष STMT_व्यंजक_NOT:
        ok = व्यंजक_Resolve_Boolean(ctx, व्यंजक->unary.child, set_rtrn);
        यदि (ok)
            *set_rtrn = !*set_rtrn;
        प्रत्यागम ok;
    पक्ष STMT_व्यंजक_ADD:
    पक्ष STMT_व्यंजक_SUBTRACT:
    पक्ष STMT_व्यंजक_MULTIPLY:
    पक्ष STMT_व्यंजक_DIVIDE:
    पक्ष STMT_व्यंजक_ASSIGN:
    पक्ष STMT_व्यंजक_NEGATE:
    पक्ष STMT_व्यंजक_UNARY_PLUS:
    पक्ष STMT_व्यंजक_EMPTY_LIST:
    पक्ष STMT_व्यंजक_ACTION_DECL:
    पक्ष STMT_व्यंजक_ACTION_LIST:
    पक्ष STMT_व्यंजक_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(व्यंजक->common.type));
        भङ्ग;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 व्यंजक->common.type);
        भङ्ग;
    }

    प्रत्यागम false;
}

स्थिर bool
व्यंजक_Resolve_Integer_Lookup(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ध्रुव शून्य *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    व्यंजक_Def *बायाँ, *दायाँ;

    विकल्प (व्यंजक->common.type) {
    पक्ष STMT_व्यंजक_INTEGER_LITERAL:
        *val_rtrn = व्यंजक->integer.ival;
        प्रत्यागम true;

    पक्ष STMT_व्यंजक_STRING_LITERAL:
    पक्ष STMT_व्यंजक_FLOAT_LITERAL:
    पक्ष STMT_व्यंजक_BOOLEAN_LITERAL:
    पक्ष STMT_व्यंजक_KEYNAME_LITERAL:
    पक्ष STMT_व्यंजक_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(व्यंजक->common.type));
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_IDENT:
        यदि (lookup)
            ok = lookup(ctx, lookupPriv, व्यंजक->ident.ident, &u, pending);

        यदि (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, व्यंजक->ident.ident));
        अन्यथा
            *val_rtrn = (int64_t) u;

        यदि (pending && *pending) {
            /* Postpone computation */
            प्रत्यागम false;
        }

        प्रत्यागम ok;

    पक्ष STMT_व्यंजक_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, व्यंजक->field_ref.element),
                xkb_atom_text(ctx, व्यंजक->field_ref.field));
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_ADD:
    पक्ष STMT_व्यंजक_SUBTRACT:
    पक्ष STMT_व्यंजक_MULTIPLY:
    पक्ष STMT_व्यंजक_DIVIDE:
        बायाँ = व्यंजक->द्विआधारी.बायाँ;
        दायाँ = व्यंजक->द्विआधारी.दायाँ;
        यदि (!व्यंजक_Resolve_Integer_Lookup(ctx, बायाँ, &l, pending, lookup, lookupPriv) ||
            !व्यंजक_Resolve_Integer_Lookup(ctx, दायाँ, &r, pending, lookup, lookupPriv))
            प्रत्यागम false;

        विकल्प (व्यंजक->common.type) {
        पक्ष STMT_व्यंजक_ADD:
            यदि (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                प्रत्यागम false;
            }
            भङ्ग;
        पक्ष STMT_व्यंजक_SUBTRACT:
            यदि (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                प्रत्यागम false;
            }
            भङ्ग;
        पक्ष STMT_व्यंजक_MULTIPLY:
            यदि (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                प्रत्यागम false;
            }
            भङ्ग;
        पक्ष STMT_व्यंजक_DIVIDE:
            यदि (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                प्रत्यागम false;
            }
            *val_rtrn = l / r;
            भङ्ग;
        स्वभाव:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(व्यंजक->common.type));
            प्रत्यागम false;
        }

        प्रत्यागम true;

    पक्ष STMT_व्यंजक_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        भङ्ग;

    पक्ष STMT_व्यंजक_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_INVERT:
    पक्ष STMT_व्यंजक_NEGATE:
        बायाँ = व्यंजक->unary.child;
        यदि (!व्यंजक_Resolve_Integer_Lookup(ctx, बायाँ, &l, pending, lookup, lookupPriv))
            प्रत्यागम false;

        *val_rtrn = (व्यंजक->common.type == STMT_व्यंजक_NEGATE ? -l : ~l);
        प्रत्यागम true;

    पक्ष STMT_व्यंजक_UNARY_PLUS:
        बायाँ = व्यंजक->unary.child;
        प्रत्यागम व्यंजक_Resolve_Integer_Lookup(ctx, बायाँ, val_rtrn, pending, lookup,
                                        lookupPriv);

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 व्यंजक->common.type);
        भङ्ग;
    }

    प्रत्यागम false;
}

bool
व्यंजक_Resolve_Integer(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                   int64_t *val_rtrn)
{
    प्रत्यागम व्यंजक_Resolve_Integer_Lookup(ctx, व्यंजक, val_rtrn, रिक्त, रिक्त, रिक्त);
}

गणना xkb_parser_error
व्यंजक_Resolve_Group(ध्रुव संरचना xkb_keymap_info *keymap_info,
                 ध्रुव व्यंजक_Def *व्यंजक, bool absolute,
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
    यदि (!व्यंजक_Resolve_Integer_Lookup(keymap_info->keymap.ctx, व्यंजक, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    यदि (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    प्रत्यागम PARSER_SUCCESS;
}

bool
व्यंजक_Resolve_Level(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
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
    यदि (!व्यंजक_Resolve_Integer_Lookup(ctx, व्यंजक, &result, रिक्त,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        प्रत्यागम false;

    यदि (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        प्रत्यागम false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    प्रत्यागम true;
}

bool
व्यंजक_Resolve_Button(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक, int64_t *btn_rtrn)
{
    प्रत्यागम व्यंजक_Resolve_Integer_Lookup(ctx, व्यंजक, btn_rtrn, रिक्त, SimpleLookup,
                                    buttonNames);
}

bool
व्यंजक_Resolve_String(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                  xkb_atom_t *val_rtrn)
{
    विकल्प (व्यंजक->common.type) {
    पक्ष STMT_व्यंजक_STRING_LITERAL:
        *val_rtrn = व्यंजक->string.str;
        प्रत्यागम true;

    पक्ष STMT_व्यंजक_INTEGER_LITERAL:
    पक्ष STMT_व्यंजक_FLOAT_LITERAL:
    पक्ष STMT_व्यंजक_BOOLEAN_LITERAL:
    पक्ष STMT_व्यंजक_KEYNAME_LITERAL:
    पक्ष STMT_व्यंजक_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(व्यंजक->common.type));
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, व्यंजक->ident.ident));
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, व्यंजक->field_ref.element),
                xkb_atom_text(ctx, व्यंजक->field_ref.field));
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_ADD:
    पक्ष STMT_व्यंजक_SUBTRACT:
    पक्ष STMT_व्यंजक_MULTIPLY:
    पक्ष STMT_व्यंजक_DIVIDE:
    पक्ष STMT_व्यंजक_ASSIGN:
    पक्ष STMT_व्यंजक_NEGATE:
    पक्ष STMT_व्यंजक_INVERT:
    पक्ष STMT_व्यंजक_NOT:
    पक्ष STMT_व्यंजक_UNARY_PLUS:
    पक्ष STMT_व्यंजक_EMPTY_LIST:
    पक्ष STMT_व्यंजक_ACTION_DECL:
    पक्ष STMT_व्यंजक_ACTION_LIST:
    पक्ष STMT_व्यंजक_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(व्यंजक->common.type));
        प्रत्यागम false;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 व्यंजक->common.type);
        भङ्ग;
    }
    प्रत्यागम false;
}

bool
व्यंजक_Resolve_Enum(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                uint32_t *val_rtrn, ध्रुव LookupEntry *मूल्यावली)
{
    यदि (व्यंजक->common.type != STMT_व्यंजक_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(व्यंजक->common.type));
        प्रत्यागम false;
    }

    यदि (!SimpleLookup(ctx, मूल्यावली, व्यंजक->ident.ident, val_rtrn, रिक्त)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, व्यंजक->ident.ident));
        यावत् (मूल्यावली && मूल्यावली->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", मूल्यावली->name);
            मूल्यावली++;
        }
        प्रत्यागम false;
    }

    प्रत्यागम true;
}

स्थिर bool
व्यंजक_Resolve_Mask_Lookup(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ध्रुव शून्य *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    व्यंजक_Def *बायाँ, *दायाँ;
    ध्रुव वर्ण *bogus = रिक्त;

    विकल्प (व्यंजक->common.type) {
    पक्ष STMT_व्यंजक_INTEGER_LITERAL:
        यदि (व्यंजक->integer.ival < 0 || व्यंजक->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    व्यंजक->integer.ival < 0 ? "-" : "",
                    imaxabs(व्यंजक->integer.ival),
                    UINT32_MAX);
            प्रत्यागम false;
        }
        *val_rtrn = (uint32_t) व्यंजक->integer.ival;
        प्रत्यागम true;

    पक्ष STMT_व्यंजक_STRING_LITERAL:
    पक्ष STMT_व्यंजक_FLOAT_LITERAL:
    पक्ष STMT_व्यंजक_BOOLEAN_LITERAL:
    पक्ष STMT_व्यंजक_KEYNAME_LITERAL:
    पक्ष STMT_व्यंजक_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(व्यंजक->common.type));
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_IDENT:
        ok = lookup(ctx, lookupPriv, व्यंजक->ident.ident, val_rtrn, pending);
        यदि (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, व्यंजक->ident.ident));
        यदि (pending && *pending) {
            /* Postpone computation */
            प्रत्यागम false;
        }
        प्रत्यागम ok;

    पक्ष STMT_व्यंजक_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, व्यंजक->field_ref.element),
                xkb_atom_text(ctx, व्यंजक->field_ref.field));
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    पक्ष STMT_व्यंजक_ACTION_DECL:
        यदि (bogus == रिक्त)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        प्रत्यागम false;

    पक्ष STMT_व्यंजक_ADD:
    पक्ष STMT_व्यंजक_SUBTRACT:
    पक्ष STMT_व्यंजक_MULTIPLY:
    पक्ष STMT_व्यंजक_DIVIDE:
        बायाँ = व्यंजक->द्विआधारी.बायाँ;
        दायाँ = व्यंजक->द्विआधारी.दायाँ;
        यदि (!व्यंजक_Resolve_Mask_Lookup(ctx, बायाँ, &l, pending, lookup, lookupPriv) ||
            !व्यंजक_Resolve_Mask_Lookup(ctx, दायाँ, &r, pending, lookup, lookupPriv))
            प्रत्यागम false;

        विकल्प (व्यंजक->common.type) {
        पक्ष STMT_व्यंजक_ADD:
            *val_rtrn = l | r;
            भङ्ग;
        पक्ष STMT_व्यंजक_SUBTRACT:
            *val_rtrn = l & (~r);
            भङ्ग;
        पक्ष STMT_व्यंजक_MULTIPLY:
        पक्ष STMT_व्यंजक_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (व्यंजक->common.type == STMT_व्यंजक_DIVIDE ? "divide" : "multiply"));
            प्रत्यागम false;
        स्वभाव:
            भङ्ग;
        }

        प्रत्यागम true;

    पक्ष STMT_व्यंजक_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        भङ्ग;

    पक्ष STMT_व्यंजक_INVERT:
        बायाँ = व्यंजक->unary.child;
        यदि (!व्यंजक_Resolve_Integer_Lookup(ctx, बायाँ, &v, pending, lookup, lookupPriv))
            प्रत्यागम false;
        यदि (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            प्रत्यागम false;
        }
        *val_rtrn = ~(uint32_t) v;
        प्रत्यागम true;

    पक्ष STMT_व्यंजक_UNARY_PLUS:
    पक्ष STMT_व्यंजक_NEGATE:
    पक्ष STMT_व्यंजक_NOT:
        बायाँ = व्यंजक->unary.child;
        यदि (!व्यंजक_Resolve_Integer_Lookup(ctx, बायाँ, &v, pending, lookup, lookupPriv))
            प्रत्यागम false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(व्यंजक->common.type));
        प्रत्यागम false;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 व्यंजक->common.type);
        भङ्ग;
    }

    प्रत्यागम false;
}

bool
व्यंजक_Resolve_Mask(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                uint32_t *mask_rtrn, ध्रुव LookupEntry *मूल्यावली)
{
    प्रत्यागम व्यंजक_Resolve_Mask_Lookup(ctx, व्यंजक, mask_rtrn, रिक्त, SimpleLookup, मूल्यावली);
}

bool
व्यंजक_Resolve_Mod_Mask(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *व्यंजक,
                   गणना mod_type mod_type, ध्रुव संरचना xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    प्रत्यागम व्यंजक_Resolve_Mask_Lookup(ctx, व्यंजक, mask_rtrn, रिक्त, LookupModMask, &priv);
}

bool
व्यंजक_Resolve_Mod(संरचना xkb_context *ctx, ध्रुव व्यंजक_Def *def,
               गणना mod_type mod_type, ध्रुव संरचना xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    यदि (def->common.type != STMT_व्यंजक_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        प्रत्यागम false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    यदि (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        प्रत्यागम false;
    }

    *ndx_rtrn = ndx;
    प्रत्यागम true;
}

bool
व्यंजक_Resolve_Group_Mask(ध्रुव संरचना xkb_keymap_info *keymap_info,
                     ध्रुव व्यंजक_Def *व्यंजक, xkb_layout_mask_t *group_rtrn,
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

    प्रत्यागम व्यंजक_Resolve_Mask_Lookup(keymap_info->keymap.ctx, व्यंजक, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
