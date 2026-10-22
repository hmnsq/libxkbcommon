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

प्रकारनिर्देश bool (*IdentLookupFunc)(संरचना xkb_context *ctx, अचर शून्य *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
अभिव्यक्ति_Resolve_Lhs(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति,
               अचर वर्ण **elem_rtrn, अचर वर्ण **field_rtrn,
               अभिव्यक्ति_Def **index_rtrn)
{
    विकल्प (अभिव्यक्ति->common.type) {
    पक्ष STMT_अभिव्यक्ति_IDENT:
        *elem_rtrn = रिक्त;
        *field_rtrn = xkb_atom_text(ctx, अभिव्यक्ति->ident.ident);
        *index_rtrn = रिक्त;
        फर्काऊ (*field_rtrn != रिक्त);
    पक्ष STMT_अभिव्यक्ति_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, अभिव्यक्ति->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, अभिव्यक्ति->field_ref.field);
        *index_rtrn = रिक्त;
        फर्काऊ (*elem_rtrn != रिक्त && *field_rtrn != रिक्त);
    पक्ष STMT_अभिव्यक्ति_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, अभिव्यक्ति->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, अभिव्यक्ति->array_ref.field);
        *index_rtrn = अभिव्यक्ति->array_ref.entry;
        यदि (अभिव्यक्ति->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == रिक्त)
            फर्काऊ false;
        यदि (*field_rtrn == रिक्त)
            फर्काऊ false;
        फर्काऊ true;
    स्वभाव:
        रोक;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", अभिव्यक्ति->common.type);
    फर्काऊ false;
}

स्थिर bool
SimpleLookup(संरचना xkb_context *ctx, अचर शून्य *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    यदि (!priv || field == XKB_ATOM_NONE)
        फर्काऊ false;

    अचर वर्ण *str = xkb_atom_text(ctx, field);
    लागि (अचर LookupEntry *entry = priv; entry && entry->name; entry++) {
        यदि (istreq(str, entry->name)) {
            *val_rtrn = entry->मान;
            फर्काऊ true;
        }
    }

    फर्काऊ false;
}

संरचना named_integer_pattern {
    अचर वर्ण *prefix;
    माप_प्रकार prefix_length;
    uint32_t min;
    uint32_t max;
    अचर LookupEntry *entries;
    अचर LookupEntry *pending_entries;
    bool is_mask;
    गणना xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
स्थिर bool
NamedIntegerPatternLookup(संरचना xkb_context *ctx, अचर शून्य *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    यदि (!priv || field == XKB_ATOM_NONE)
        फर्काऊ false;

    अचर वर्ण * अचर str = xkb_atom_text(ctx, field);
    अचर संरचना named_integer_pattern * अचर pattern = priv;

    अचर पूर्णाङ्क गन्ती = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    यदि (गन्ती > 0 && *(str + pattern->prefix_length + गन्ती) == '\0') {
        यदि (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            फर्काऊ false;
        }
        यदि (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            प्रतिज्ञा(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        फर्काऊ true;
    } अन्यथा {
        यदि (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, रिक्त)) {
            फर्काऊ true;
        }
        यदि (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, रिक्त)) {
            *pending_rtrn = true;
            फर्काऊ true;
        }
        फर्काऊ false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
प्रकारनिर्देश संरचना {
    अचर संरचना xkb_mod_set *mods;
    गणना mod_type mod_type;
} LookupModMaskPriv;

स्थिर bool
LookupModMask(संरचना xkb_context *ctx, अचर शून्य *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    अचर वर्ण *str = xkb_atom_text(ctx, field);
    यदि (!str)
        फर्काऊ false;

    यदि (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        फर्काऊ true;
    }

    यदि (istreq(str, "none")) {
        *val_rtrn = 0;
        फर्काऊ true;
    }

    अचर LookupModMaskPriv *arg = priv;
    अचर संरचना xkb_mod_set *mods = arg->mods;
    अचर गणना mod_type mod_type = arg->mod_type;
    अचर xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    यदि (ndx == XKB_MOD_INVALID)
        फर्काऊ false;

    *val_rtrn = (UINT32_C(1) << ndx);
    फर्काऊ true;
}

bool
अभिव्यक्ति_Resolve_Boolean(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति,
                   bool *set_rtrn)
{
    bool ok = false;
    अचर वर्ण *ident;

    विकल्प (अभिव्यक्ति->common.type) {
    पक्ष STMT_अभिव्यक्ति_BOOLEAN_LITERAL:
        *set_rtrn = अभिव्यक्ति->boolean.set;
        फर्काऊ true;

    पक्ष STMT_अभिव्यक्ति_STRING_LITERAL:
    पक्ष STMT_अभिव्यक्ति_INTEGER_LITERAL:
    पक्ष STMT_अभिव्यक्ति_FLOAT_LITERAL:
    पक्ष STMT_अभिव्यक्ति_KEYNAME_LITERAL:
    पक्ष STMT_अभिव्यक्ति_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(अभिव्यक्ति->common.type));
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_IDENT:
        ident = xkb_atom_text(ctx, अभिव्यक्ति->ident.ident);
        यदि (ident) {
            यदि (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                फर्काऊ true;
            }
            अन्यथा यदि (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                फर्काऊ true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, अभिव्यक्ति->field_ref.element),
                xkb_atom_text(ctx, अभिव्यक्ति->field_ref.field));
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_INVERT:
    पक्ष STMT_अभिव्यक्ति_NOT:
        ok = अभिव्यक्ति_Resolve_Boolean(ctx, अभिव्यक्ति->unary.child, set_rtrn);
        यदि (ok)
            *set_rtrn = !*set_rtrn;
        फर्काऊ ok;
    पक्ष STMT_अभिव्यक्ति_ADD:
    पक्ष STMT_अभिव्यक्ति_SUBTRACT:
    पक्ष STMT_अभिव्यक्ति_MULTIPLY:
    पक्ष STMT_अभिव्यक्ति_DIVIDE:
    पक्ष STMT_अभिव्यक्ति_ASSIGN:
    पक्ष STMT_अभिव्यक्ति_NEGATE:
    पक्ष STMT_अभिव्यक्ति_UNARY_PLUS:
    पक्ष STMT_अभिव्यक्ति_EMPTY_LIST:
    पक्ष STMT_अभिव्यक्ति_ACTION_DECL:
    पक्ष STMT_अभिव्यक्ति_ACTION_LIST:
    पक्ष STMT_अभिव्यक्ति_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(अभिव्यक्ति->common.type));
        रोक;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 अभिव्यक्ति->common.type);
        रोक;
    }

    फर्काऊ false;
}

स्थिर bool
अभिव्यक्ति_Resolve_Integer_Lookup(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, अचर शून्य *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    अभिव्यक्ति_Def *बायाँ, *दायाँ;

    विकल्प (अभिव्यक्ति->common.type) {
    पक्ष STMT_अभिव्यक्ति_INTEGER_LITERAL:
        *val_rtrn = अभिव्यक्ति->integer.ival;
        फर्काऊ true;

    पक्ष STMT_अभिव्यक्ति_STRING_LITERAL:
    पक्ष STMT_अभिव्यक्ति_FLOAT_LITERAL:
    पक्ष STMT_अभिव्यक्ति_BOOLEAN_LITERAL:
    पक्ष STMT_अभिव्यक्ति_KEYNAME_LITERAL:
    पक्ष STMT_अभिव्यक्ति_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(अभिव्यक्ति->common.type));
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_IDENT:
        यदि (lookup)
            ok = lookup(ctx, lookupPriv, अभिव्यक्ति->ident.ident, &u, pending);

        यदि (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, अभिव्यक्ति->ident.ident));
        अन्यथा
            *val_rtrn = (int64_t) u;

        यदि (pending && *pending) {
            /* Postpone computation */
            फर्काऊ false;
        }

        फर्काऊ ok;

    पक्ष STMT_अभिव्यक्ति_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, अभिव्यक्ति->field_ref.element),
                xkb_atom_text(ctx, अभिव्यक्ति->field_ref.field));
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_ADD:
    पक्ष STMT_अभिव्यक्ति_SUBTRACT:
    पक्ष STMT_अभिव्यक्ति_MULTIPLY:
    पक्ष STMT_अभिव्यक्ति_DIVIDE:
        बायाँ = अभिव्यक्ति->द्विआधारी.बायाँ;
        दायाँ = अभिव्यक्ति->द्विआधारी.दायाँ;
        यदि (!अभिव्यक्ति_Resolve_Integer_Lookup(ctx, बायाँ, &l, pending, lookup, lookupPriv) ||
            !अभिव्यक्ति_Resolve_Integer_Lookup(ctx, दायाँ, &r, pending, lookup, lookupPriv))
            फर्काऊ false;

        विकल्प (अभिव्यक्ति->common.type) {
        पक्ष STMT_अभिव्यक्ति_ADD:
            यदि (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                फर्काऊ false;
            }
            रोक;
        पक्ष STMT_अभिव्यक्ति_SUBTRACT:
            यदि (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                फर्काऊ false;
            }
            रोक;
        पक्ष STMT_अभिव्यक्ति_MULTIPLY:
            यदि (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                फर्काऊ false;
            }
            रोक;
        पक्ष STMT_अभिव्यक्ति_DIVIDE:
            यदि (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                फर्काऊ false;
            }
            *val_rtrn = l / r;
            रोक;
        स्वभाव:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(अभिव्यक्ति->common.type));
            फर्काऊ false;
        }

        फर्काऊ true;

    पक्ष STMT_अभिव्यक्ति_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        रोक;

    पक्ष STMT_अभिव्यक्ति_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_INVERT:
    पक्ष STMT_अभिव्यक्ति_NEGATE:
        बायाँ = अभिव्यक्ति->unary.child;
        यदि (!अभिव्यक्ति_Resolve_Integer_Lookup(ctx, बायाँ, &l, pending, lookup, lookupPriv))
            फर्काऊ false;

        *val_rtrn = (अभिव्यक्ति->common.type == STMT_अभिव्यक्ति_NEGATE ? -l : ~l);
        फर्काऊ true;

    पक्ष STMT_अभिव्यक्ति_UNARY_PLUS:
        बायाँ = अभिव्यक्ति->unary.child;
        फर्काऊ अभिव्यक्ति_Resolve_Integer_Lookup(ctx, बायाँ, val_rtrn, pending, lookup,
                                        lookupPriv);

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 अभिव्यक्ति->common.type);
        रोक;
    }

    फर्काऊ false;
}

bool
अभिव्यक्ति_Resolve_Integer(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति,
                   int64_t *val_rtrn)
{
    फर्काऊ अभिव्यक्ति_Resolve_Integer_Lookup(ctx, अभिव्यक्ति, val_rtrn, रिक्त, रिक्त, रिक्त);
}

गणना xkb_parser_error
अभिव्यक्ति_Resolve_Group(अचर संरचना xkb_keymap_info *keymap_info,
                 अचर अभिव्यक्ति_Def *अभिव्यक्ति, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    स्थिर अचर LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { रिक्त, 0 }
    };

    अचर संरचना named_integer_pattern group_name_pattern = {
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
    यदि (!अभिव्यक्ति_Resolve_Integer_Lookup(keymap_info->keymap.ctx, अभिव्यक्ति, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        फर्काऊ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    यदि (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        फर्काऊ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    फर्काऊ PARSER_SUCCESS;
}

bool
अभिव्यक्ति_Resolve_Level(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति,
                 xkb_level_index_t *level_rtrn)
{
    स्थिर अचर संरचना named_integer_pattern level_name_pattern = {
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
    यदि (!अभिव्यक्ति_Resolve_Integer_Lookup(ctx, अभिव्यक्ति, &result, रिक्त,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        फर्काऊ false;

    यदि (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        फर्काऊ false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    फर्काऊ true;
}

bool
अभिव्यक्ति_Resolve_Button(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति, int64_t *btn_rtrn)
{
    फर्काऊ अभिव्यक्ति_Resolve_Integer_Lookup(ctx, अभिव्यक्ति, btn_rtrn, रिक्त, SimpleLookup,
                                    buttonNames);
}

bool
अभिव्यक्ति_Resolve_String(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति,
                  xkb_atom_t *val_rtrn)
{
    विकल्प (अभिव्यक्ति->common.type) {
    पक्ष STMT_अभिव्यक्ति_STRING_LITERAL:
        *val_rtrn = अभिव्यक्ति->string.str;
        फर्काऊ true;

    पक्ष STMT_अभिव्यक्ति_INTEGER_LITERAL:
    पक्ष STMT_अभिव्यक्ति_FLOAT_LITERAL:
    पक्ष STMT_अभिव्यक्ति_BOOLEAN_LITERAL:
    पक्ष STMT_अभिव्यक्ति_KEYNAME_LITERAL:
    पक्ष STMT_अभिव्यक्ति_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(अभिव्यक्ति->common.type));
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, अभिव्यक्ति->ident.ident));
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, अभिव्यक्ति->field_ref.element),
                xkb_atom_text(ctx, अभिव्यक्ति->field_ref.field));
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_ADD:
    पक्ष STMT_अभिव्यक्ति_SUBTRACT:
    पक्ष STMT_अभिव्यक्ति_MULTIPLY:
    पक्ष STMT_अभिव्यक्ति_DIVIDE:
    पक्ष STMT_अभिव्यक्ति_ASSIGN:
    पक्ष STMT_अभिव्यक्ति_NEGATE:
    पक्ष STMT_अभिव्यक्ति_INVERT:
    पक्ष STMT_अभिव्यक्ति_NOT:
    पक्ष STMT_अभिव्यक्ति_UNARY_PLUS:
    पक्ष STMT_अभिव्यक्ति_EMPTY_LIST:
    पक्ष STMT_अभिव्यक्ति_ACTION_DECL:
    पक्ष STMT_अभिव्यक्ति_ACTION_LIST:
    पक्ष STMT_अभिव्यक्ति_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(अभिव्यक्ति->common.type));
        फर्काऊ false;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 अभिव्यक्ति->common.type);
        रोक;
    }
    फर्काऊ false;
}

bool
अभिव्यक्ति_Resolve_Enum(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति,
                uint32_t *val_rtrn, अचर LookupEntry *मानहरू)
{
    यदि (अभिव्यक्ति->common.type != STMT_अभिव्यक्ति_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(अभिव्यक्ति->common.type));
        फर्काऊ false;
    }

    यदि (!SimpleLookup(ctx, मानहरू, अभिव्यक्ति->ident.ident, val_rtrn, रिक्त)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, अभिव्यक्ति->ident.ident));
        जबसम्म (मानहरू && मानहरू->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", मानहरू->name);
            मानहरू++;
        }
        फर्काऊ false;
    }

    फर्काऊ true;
}

स्थिर bool
अभिव्यक्ति_Resolve_Mask_Lookup(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, अचर शून्य *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    अभिव्यक्ति_Def *बायाँ, *दायाँ;
    अचर वर्ण *bogus = रिक्त;

    विकल्प (अभिव्यक्ति->common.type) {
    पक्ष STMT_अभिव्यक्ति_INTEGER_LITERAL:
        यदि (अभिव्यक्ति->integer.ival < 0 || अभिव्यक्ति->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    अभिव्यक्ति->integer.ival < 0 ? "-" : "",
                    imaxabs(अभिव्यक्ति->integer.ival),
                    UINT32_MAX);
            फर्काऊ false;
        }
        *val_rtrn = (uint32_t) अभिव्यक्ति->integer.ival;
        फर्काऊ true;

    पक्ष STMT_अभिव्यक्ति_STRING_LITERAL:
    पक्ष STMT_अभिव्यक्ति_FLOAT_LITERAL:
    पक्ष STMT_अभिव्यक्ति_BOOLEAN_LITERAL:
    पक्ष STMT_अभिव्यक्ति_KEYNAME_LITERAL:
    पक्ष STMT_अभिव्यक्ति_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(अभिव्यक्ति->common.type));
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_IDENT:
        ok = lookup(ctx, lookupPriv, अभिव्यक्ति->ident.ident, val_rtrn, pending);
        यदि (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, अभिव्यक्ति->ident.ident));
        यदि (pending && *pending) {
            /* Postpone computation */
            फर्काऊ false;
        }
        फर्काऊ ok;

    पक्ष STMT_अभिव्यक्ति_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, अभिव्यक्ति->field_ref.element),
                xkb_atom_text(ctx, अभिव्यक्ति->field_ref.field));
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    पक्ष STMT_अभिव्यक्ति_ACTION_DECL:
        यदि (bogus == रिक्त)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        फर्काऊ false;

    पक्ष STMT_अभिव्यक्ति_ADD:
    पक्ष STMT_अभिव्यक्ति_SUBTRACT:
    पक्ष STMT_अभिव्यक्ति_MULTIPLY:
    पक्ष STMT_अभिव्यक्ति_DIVIDE:
        बायाँ = अभिव्यक्ति->द्विआधारी.बायाँ;
        दायाँ = अभिव्यक्ति->द्विआधारी.दायाँ;
        यदि (!अभिव्यक्ति_Resolve_Mask_Lookup(ctx, बायाँ, &l, pending, lookup, lookupPriv) ||
            !अभिव्यक्ति_Resolve_Mask_Lookup(ctx, दायाँ, &r, pending, lookup, lookupPriv))
            फर्काऊ false;

        विकल्प (अभिव्यक्ति->common.type) {
        पक्ष STMT_अभिव्यक्ति_ADD:
            *val_rtrn = l | r;
            रोक;
        पक्ष STMT_अभिव्यक्ति_SUBTRACT:
            *val_rtrn = l & (~r);
            रोक;
        पक्ष STMT_अभिव्यक्ति_MULTIPLY:
        पक्ष STMT_अभिव्यक्ति_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (अभिव्यक्ति->common.type == STMT_अभिव्यक्ति_DIVIDE ? "divide" : "multiply"));
            फर्काऊ false;
        स्वभाव:
            रोक;
        }

        फर्काऊ true;

    पक्ष STMT_अभिव्यक्ति_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        रोक;

    पक्ष STMT_अभिव्यक्ति_INVERT:
        बायाँ = अभिव्यक्ति->unary.child;
        यदि (!अभिव्यक्ति_Resolve_Integer_Lookup(ctx, बायाँ, &v, pending, lookup, lookupPriv))
            फर्काऊ false;
        यदि (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            फर्काऊ false;
        }
        *val_rtrn = ~(uint32_t) v;
        फर्काऊ true;

    पक्ष STMT_अभिव्यक्ति_UNARY_PLUS:
    पक्ष STMT_अभिव्यक्ति_NEGATE:
    पक्ष STMT_अभिव्यक्ति_NOT:
        बायाँ = अभिव्यक्ति->unary.child;
        यदि (!अभिव्यक्ति_Resolve_Integer_Lookup(ctx, बायाँ, &v, pending, lookup, lookupPriv))
            फर्काऊ false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(अभिव्यक्ति->common.type));
        फर्काऊ false;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 अभिव्यक्ति->common.type);
        रोक;
    }

    फर्काऊ false;
}

bool
अभिव्यक्ति_Resolve_Mask(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति,
                uint32_t *mask_rtrn, अचर LookupEntry *मानहरू)
{
    फर्काऊ अभिव्यक्ति_Resolve_Mask_Lookup(ctx, अभिव्यक्ति, mask_rtrn, रिक्त, SimpleLookup, मानहरू);
}

bool
अभिव्यक्ति_Resolve_Mod_Mask(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *अभिव्यक्ति,
                   गणना mod_type mod_type, अचर संरचना xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    फर्काऊ अभिव्यक्ति_Resolve_Mask_Lookup(ctx, अभिव्यक्ति, mask_rtrn, रिक्त, LookupModMask, &priv);
}

bool
अभिव्यक्ति_Resolve_Mod(संरचना xkb_context *ctx, अचर अभिव्यक्ति_Def *def,
               गणना mod_type mod_type, अचर संरचना xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    यदि (def->common.type != STMT_अभिव्यक्ति_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        फर्काऊ false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    यदि (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        फर्काऊ false;
    }

    *ndx_rtrn = ndx;
    फर्काऊ true;
}

bool
अभिव्यक्ति_Resolve_Group_Mask(अचर संरचना xkb_keymap_info *keymap_info,
                     अचर अभिव्यक्ति_Def *अभिव्यक्ति, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    स्थिर अचर LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { रिक्त, 0 }
    };

    अचर संरचना named_integer_pattern group_name_pattern = {
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

    फर्काऊ अभिव्यक्ति_Resolve_Mask_Lookup(keymap_info->keymap.ctx, अभिव्यक्ति, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
