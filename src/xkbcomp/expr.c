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

प्रकारव्याख्या bool (*IdentLookupFunc)(संरचना xkb_context *ctx, स्थिरांक शून्य *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
अभिव्यक्ती_Resolve_Lhs(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती,
               स्थिरांक अक्षर **elem_rtrn, स्थिरांक अक्षर **field_rtrn,
               अभिव्यक्ती_Def **index_rtrn)
{
    निवड (अभिव्यक्ती->common.type) {
    स्थिती STMT_अभिव्यक्ती_IDENT:
        *elem_rtrn = रिक्त;
        *field_rtrn = xkb_atom_text(ctx, अभिव्यक्ती->ident.ident);
        *index_rtrn = रिक्त;
        परत (*field_rtrn != रिक्त);
    स्थिती STMT_अभिव्यक्ती_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, अभिव्यक्ती->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, अभिव्यक्ती->field_ref.field);
        *index_rtrn = रिक्त;
        परत (*elem_rtrn != रिक्त && *field_rtrn != रिक्त);
    स्थिती STMT_अभिव्यक्ती_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, अभिव्यक्ती->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, अभिव्यक्ती->array_ref.field);
        *index_rtrn = अभिव्यक्ती->array_ref.entry;
        जर (अभिव्यक्ती->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == रिक्त)
            परत false;
        जर (*field_rtrn == रिक्त)
            परत false;
        परत true;
    पूर्वनिर्धारित:
        थांबा;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", अभिव्यक्ती->common.type);
    परत false;
}

स्थिर bool
SimpleLookup(संरचना xkb_context *ctx, स्थिरांक शून्य *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    जर (!priv || field == XKB_ATOM_NONE)
        परत false;

    स्थिरांक अक्षर *str = xkb_atom_text(ctx, field);
    साठी (स्थिरांक LookupEntry *entry = priv; entry && entry->name; entry++) {
        जर (istreq(str, entry->name)) {
            *val_rtrn = entry->मूल्य;
            परत true;
        }
    }

    परत false;
}

संरचना named_integer_pattern {
    स्थिरांक अक्षर *prefix;
    माप_प्रकार prefix_length;
    uint32_t min;
    uint32_t max;
    स्थिरांक LookupEntry *entries;
    स्थिरांक LookupEntry *pending_entries;
    bool is_mask;
    गणना xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
स्थिर bool
NamedIntegerPatternLookup(संरचना xkb_context *ctx, स्थिरांक शून्य *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    जर (!priv || field == XKB_ATOM_NONE)
        परत false;

    स्थिरांक अक्षर * स्थिरांक str = xkb_atom_text(ctx, field);
    स्थिरांक संरचना named_integer_pattern * स्थिरांक pattern = priv;

    स्थिरांक पूर्णांक मोजणी = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    जर (मोजणी > 0 && *(str + pattern->prefix_length + मोजणी) == '\0') {
        जर (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            परत false;
        }
        जर (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            प्रतिज्ञा(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        परत true;
    } अन्यथा {
        जर (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, रिक्त)) {
            परत true;
        }
        जर (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, रिक्त)) {
            *pending_rtrn = true;
            परत true;
        }
        परत false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
प्रकारव्याख्या संरचना {
    स्थिरांक संरचना xkb_mod_set *mods;
    गणना mod_type mod_type;
} LookupModMaskPriv;

स्थिर bool
LookupModMask(संरचना xkb_context *ctx, स्थिरांक शून्य *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    स्थिरांक अक्षर *str = xkb_atom_text(ctx, field);
    जर (!str)
        परत false;

    जर (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        परत true;
    }

    जर (istreq(str, "none")) {
        *val_rtrn = 0;
        परत true;
    }

    स्थिरांक LookupModMaskPriv *arg = priv;
    स्थिरांक संरचना xkb_mod_set *mods = arg->mods;
    स्थिरांक गणना mod_type mod_type = arg->mod_type;
    स्थिरांक xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    जर (ndx == XKB_MOD_INVALID)
        परत false;

    *val_rtrn = (UINT32_C(1) << ndx);
    परत true;
}

bool
अभिव्यक्ती_Resolve_Boolean(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती,
                   bool *set_rtrn)
{
    bool ok = false;
    स्थिरांक अक्षर *ident;

    निवड (अभिव्यक्ती->common.type) {
    स्थिती STMT_अभिव्यक्ती_BOOLEAN_LITERAL:
        *set_rtrn = अभिव्यक्ती->boolean.set;
        परत true;

    स्थिती STMT_अभिव्यक्ती_STRING_LITERAL:
    स्थिती STMT_अभिव्यक्ती_INTEGER_LITERAL:
    स्थिती STMT_अभिव्यक्ती_FLOAT_LITERAL:
    स्थिती STMT_अभिव्यक्ती_KEYNAME_LITERAL:
    स्थिती STMT_अभिव्यक्ती_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(अभिव्यक्ती->common.type));
        परत false;

    स्थिती STMT_अभिव्यक्ती_IDENT:
        ident = xkb_atom_text(ctx, अभिव्यक्ती->ident.ident);
        जर (ident) {
            जर (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                परत true;
            }
            अन्यथा जर (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                परत true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        परत false;

    स्थिती STMT_अभिव्यक्ती_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, अभिव्यक्ती->field_ref.element),
                xkb_atom_text(ctx, अभिव्यक्ती->field_ref.field));
        परत false;

    स्थिती STMT_अभिव्यक्ती_INVERT:
    स्थिती STMT_अभिव्यक्ती_NOT:
        ok = अभिव्यक्ती_Resolve_Boolean(ctx, अभिव्यक्ती->unary.child, set_rtrn);
        जर (ok)
            *set_rtrn = !*set_rtrn;
        परत ok;
    स्थिती STMT_अभिव्यक्ती_ADD:
    स्थिती STMT_अभिव्यक्ती_SUBTRACT:
    स्थिती STMT_अभिव्यक्ती_MULTIPLY:
    स्थिती STMT_अभिव्यक्ती_DIVIDE:
    स्थिती STMT_अभिव्यक्ती_ASSIGN:
    स्थिती STMT_अभिव्यक्ती_NEGATE:
    स्थिती STMT_अभिव्यक्ती_UNARY_PLUS:
    स्थिती STMT_अभिव्यक्ती_EMPTY_LIST:
    स्थिती STMT_अभिव्यक्ती_ACTION_DECL:
    स्थिती STMT_अभिव्यक्ती_ACTION_LIST:
    स्थिती STMT_अभिव्यक्ती_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(अभिव्यक्ती->common.type));
        थांबा;

    पूर्वनिर्धारित:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 अभिव्यक्ती->common.type);
        थांबा;
    }

    परत false;
}

स्थिर bool
अभिव्यक्ती_Resolve_Integer_Lookup(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, स्थिरांक शून्य *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    अभिव्यक्ती_Def *डावा, *उजवा;

    निवड (अभिव्यक्ती->common.type) {
    स्थिती STMT_अभिव्यक्ती_INTEGER_LITERAL:
        *val_rtrn = अभिव्यक्ती->integer.ival;
        परत true;

    स्थिती STMT_अभिव्यक्ती_STRING_LITERAL:
    स्थिती STMT_अभिव्यक्ती_FLOAT_LITERAL:
    स्थिती STMT_अभिव्यक्ती_BOOLEAN_LITERAL:
    स्थिती STMT_अभिव्यक्ती_KEYNAME_LITERAL:
    स्थिती STMT_अभिव्यक्ती_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(अभिव्यक्ती->common.type));
        परत false;

    स्थिती STMT_अभिव्यक्ती_IDENT:
        जर (lookup)
            ok = lookup(ctx, lookupPriv, अभिव्यक्ती->ident.ident, &u, pending);

        जर (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, अभिव्यक्ती->ident.ident));
        अन्यथा
            *val_rtrn = (int64_t) u;

        जर (pending && *pending) {
            /* Postpone computation */
            परत false;
        }

        परत ok;

    स्थिती STMT_अभिव्यक्ती_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, अभिव्यक्ती->field_ref.element),
                xkb_atom_text(ctx, अभिव्यक्ती->field_ref.field));
        परत false;

    स्थिती STMT_अभिव्यक्ती_ADD:
    स्थिती STMT_अभिव्यक्ती_SUBTRACT:
    स्थिती STMT_अभिव्यक्ती_MULTIPLY:
    स्थिती STMT_अभिव्यक्ती_DIVIDE:
        डावा = अभिव्यक्ती->द्विमान.डावा;
        उजवा = अभिव्यक्ती->द्विमान.उजवा;
        जर (!अभिव्यक्ती_Resolve_Integer_Lookup(ctx, डावा, &l, pending, lookup, lookupPriv) ||
            !अभिव्यक्ती_Resolve_Integer_Lookup(ctx, उजवा, &r, pending, lookup, lookupPriv))
            परत false;

        निवड (अभिव्यक्ती->common.type) {
        स्थिती STMT_अभिव्यक्ती_ADD:
            जर (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                परत false;
            }
            थांबा;
        स्थिती STMT_अभिव्यक्ती_SUBTRACT:
            जर (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                परत false;
            }
            थांबा;
        स्थिती STMT_अभिव्यक्ती_MULTIPLY:
            जर (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                परत false;
            }
            थांबा;
        स्थिती STMT_अभिव्यक्ती_DIVIDE:
            जर (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                परत false;
            }
            *val_rtrn = l / r;
            थांबा;
        पूर्वनिर्धारित:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(अभिव्यक्ती->common.type));
            परत false;
        }

        परत true;

    स्थिती STMT_अभिव्यक्ती_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        थांबा;

    स्थिती STMT_अभिव्यक्ती_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        परत false;

    स्थिती STMT_अभिव्यक्ती_INVERT:
    स्थिती STMT_अभिव्यक्ती_NEGATE:
        डावा = अभिव्यक्ती->unary.child;
        जर (!अभिव्यक्ती_Resolve_Integer_Lookup(ctx, डावा, &l, pending, lookup, lookupPriv))
            परत false;

        *val_rtrn = (अभिव्यक्ती->common.type == STMT_अभिव्यक्ती_NEGATE ? -l : ~l);
        परत true;

    स्थिती STMT_अभिव्यक्ती_UNARY_PLUS:
        डावा = अभिव्यक्ती->unary.child;
        परत अभिव्यक्ती_Resolve_Integer_Lookup(ctx, डावा, val_rtrn, pending, lookup,
                                        lookupPriv);

    पूर्वनिर्धारित:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 अभिव्यक्ती->common.type);
        थांबा;
    }

    परत false;
}

bool
अभिव्यक्ती_Resolve_Integer(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती,
                   int64_t *val_rtrn)
{
    परत अभिव्यक्ती_Resolve_Integer_Lookup(ctx, अभिव्यक्ती, val_rtrn, रिक्त, रिक्त, रिक्त);
}

गणना xkb_parser_error
अभिव्यक्ती_Resolve_Group(स्थिरांक संरचना xkb_keymap_info *keymap_info,
                 स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    स्थिर स्थिरांक LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { रिक्त, 0 }
    };

    स्थिरांक संरचना named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = आकार("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    जर (!अभिव्यक्ती_Resolve_Integer_Lookup(keymap_info->keymap.ctx, अभिव्यक्ती, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        परत (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    जर (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        परत (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    परत PARSER_SUCCESS;
}

bool
अभिव्यक्ती_Resolve_Level(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती,
                 xkb_level_index_t *level_rtrn)
{
    स्थिर स्थिरांक संरचना named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = आकार("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = रिक्त,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    जर (!अभिव्यक्ती_Resolve_Integer_Lookup(ctx, अभिव्यक्ती, &result, रिक्त,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        परत false;

    जर (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        परत false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    परत true;
}

bool
अभिव्यक्ती_Resolve_Button(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती, int64_t *btn_rtrn)
{
    परत अभिव्यक्ती_Resolve_Integer_Lookup(ctx, अभिव्यक्ती, btn_rtrn, रिक्त, SimpleLookup,
                                    buttonNames);
}

bool
अभिव्यक्ती_Resolve_String(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती,
                  xkb_atom_t *val_rtrn)
{
    निवड (अभिव्यक्ती->common.type) {
    स्थिती STMT_अभिव्यक्ती_STRING_LITERAL:
        *val_rtrn = अभिव्यक्ती->string.str;
        परत true;

    स्थिती STMT_अभिव्यक्ती_INTEGER_LITERAL:
    स्थिती STMT_अभिव्यक्ती_FLOAT_LITERAL:
    स्थिती STMT_अभिव्यक्ती_BOOLEAN_LITERAL:
    स्थिती STMT_अभिव्यक्ती_KEYNAME_LITERAL:
    स्थिती STMT_अभिव्यक्ती_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(अभिव्यक्ती->common.type));
        परत false;

    स्थिती STMT_अभिव्यक्ती_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, अभिव्यक्ती->ident.ident));
        परत false;

    स्थिती STMT_अभिव्यक्ती_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, अभिव्यक्ती->field_ref.element),
                xkb_atom_text(ctx, अभिव्यक्ती->field_ref.field));
        परत false;

    स्थिती STMT_अभिव्यक्ती_ADD:
    स्थिती STMT_अभिव्यक्ती_SUBTRACT:
    स्थिती STMT_अभिव्यक्ती_MULTIPLY:
    स्थिती STMT_अभिव्यक्ती_DIVIDE:
    स्थिती STMT_अभिव्यक्ती_ASSIGN:
    स्थिती STMT_अभिव्यक्ती_NEGATE:
    स्थिती STMT_अभिव्यक्ती_INVERT:
    स्थिती STMT_अभिव्यक्ती_NOT:
    स्थिती STMT_अभिव्यक्ती_UNARY_PLUS:
    स्थिती STMT_अभिव्यक्ती_EMPTY_LIST:
    स्थिती STMT_अभिव्यक्ती_ACTION_DECL:
    स्थिती STMT_अभिव्यक्ती_ACTION_LIST:
    स्थिती STMT_अभिव्यक्ती_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(अभिव्यक्ती->common.type));
        परत false;

    पूर्वनिर्धारित:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 अभिव्यक्ती->common.type);
        थांबा;
    }
    परत false;
}

bool
अभिव्यक्ती_Resolve_Enum(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती,
                uint32_t *val_rtrn, स्थिरांक LookupEntry *मूल्ये)
{
    जर (अभिव्यक्ती->common.type != STMT_अभिव्यक्ती_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(अभिव्यक्ती->common.type));
        परत false;
    }

    जर (!SimpleLookup(ctx, मूल्ये, अभिव्यक्ती->ident.ident, val_rtrn, रिक्त)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, अभिव्यक्ती->ident.ident));
        पर्यंत (मूल्ये && मूल्ये->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", मूल्ये->name);
            मूल्ये++;
        }
        परत false;
    }

    परत true;
}

स्थिर bool
अभिव्यक्ती_Resolve_Mask_Lookup(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, स्थिरांक शून्य *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    अभिव्यक्ती_Def *डावा, *उजवा;
    स्थिरांक अक्षर *bogus = रिक्त;

    निवड (अभिव्यक्ती->common.type) {
    स्थिती STMT_अभिव्यक्ती_INTEGER_LITERAL:
        जर (अभिव्यक्ती->integer.ival < 0 || अभिव्यक्ती->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    अभिव्यक्ती->integer.ival < 0 ? "-" : "",
                    imaxabs(अभिव्यक्ती->integer.ival),
                    UINT32_MAX);
            परत false;
        }
        *val_rtrn = (uint32_t) अभिव्यक्ती->integer.ival;
        परत true;

    स्थिती STMT_अभिव्यक्ती_STRING_LITERAL:
    स्थिती STMT_अभिव्यक्ती_FLOAT_LITERAL:
    स्थिती STMT_अभिव्यक्ती_BOOLEAN_LITERAL:
    स्थिती STMT_अभिव्यक्ती_KEYNAME_LITERAL:
    स्थिती STMT_अभिव्यक्ती_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(अभिव्यक्ती->common.type));
        परत false;

    स्थिती STMT_अभिव्यक्ती_IDENT:
        ok = lookup(ctx, lookupPriv, अभिव्यक्ती->ident.ident, val_rtrn, pending);
        जर (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, अभिव्यक्ती->ident.ident));
        जर (pending && *pending) {
            /* Postpone computation */
            परत false;
        }
        परत ok;

    स्थिती STMT_अभिव्यक्ती_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, अभिव्यक्ती->field_ref.element),
                xkb_atom_text(ctx, अभिव्यक्ती->field_ref.field));
        परत false;

    स्थिती STMT_अभिव्यक्ती_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    स्थिती STMT_अभिव्यक्ती_ACTION_DECL:
        जर (bogus == रिक्त)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        परत false;

    स्थिती STMT_अभिव्यक्ती_ADD:
    स्थिती STMT_अभिव्यक्ती_SUBTRACT:
    स्थिती STMT_अभिव्यक्ती_MULTIPLY:
    स्थिती STMT_अभिव्यक्ती_DIVIDE:
        डावा = अभिव्यक्ती->द्विमान.डावा;
        उजवा = अभिव्यक्ती->द्विमान.उजवा;
        जर (!अभिव्यक्ती_Resolve_Mask_Lookup(ctx, डावा, &l, pending, lookup, lookupPriv) ||
            !अभिव्यक्ती_Resolve_Mask_Lookup(ctx, उजवा, &r, pending, lookup, lookupPriv))
            परत false;

        निवड (अभिव्यक्ती->common.type) {
        स्थिती STMT_अभिव्यक्ती_ADD:
            *val_rtrn = l | r;
            थांबा;
        स्थिती STMT_अभिव्यक्ती_SUBTRACT:
            *val_rtrn = l & (~r);
            थांबा;
        स्थिती STMT_अभिव्यक्ती_MULTIPLY:
        स्थिती STMT_अभिव्यक्ती_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (अभिव्यक्ती->common.type == STMT_अभिव्यक्ती_DIVIDE ? "divide" : "multiply"));
            परत false;
        पूर्वनिर्धारित:
            थांबा;
        }

        परत true;

    स्थिती STMT_अभिव्यक्ती_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        थांबा;

    स्थिती STMT_अभिव्यक्ती_INVERT:
        डावा = अभिव्यक्ती->unary.child;
        जर (!अभिव्यक्ती_Resolve_Integer_Lookup(ctx, डावा, &v, pending, lookup, lookupPriv))
            परत false;
        जर (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            परत false;
        }
        *val_rtrn = ~(uint32_t) v;
        परत true;

    स्थिती STMT_अभिव्यक्ती_UNARY_PLUS:
    स्थिती STMT_अभिव्यक्ती_NEGATE:
    स्थिती STMT_अभिव्यक्ती_NOT:
        डावा = अभिव्यक्ती->unary.child;
        जर (!अभिव्यक्ती_Resolve_Integer_Lookup(ctx, डावा, &v, pending, lookup, lookupPriv))
            परत false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(अभिव्यक्ती->common.type));
        परत false;

    पूर्वनिर्धारित:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 अभिव्यक्ती->common.type);
        थांबा;
    }

    परत false;
}

bool
अभिव्यक्ती_Resolve_Mask(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती,
                uint32_t *mask_rtrn, स्थिरांक LookupEntry *मूल्ये)
{
    परत अभिव्यक्ती_Resolve_Mask_Lookup(ctx, अभिव्यक्ती, mask_rtrn, रिक्त, SimpleLookup, मूल्ये);
}

bool
अभिव्यक्ती_Resolve_Mod_Mask(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती,
                   गणना mod_type mod_type, स्थिरांक संरचना xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    परत अभिव्यक्ती_Resolve_Mask_Lookup(ctx, अभिव्यक्ती, mask_rtrn, रिक्त, LookupModMask, &priv);
}

bool
अभिव्यक्ती_Resolve_Mod(संरचना xkb_context *ctx, स्थिरांक अभिव्यक्ती_Def *def,
               गणना mod_type mod_type, स्थिरांक संरचना xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    जर (def->common.type != STMT_अभिव्यक्ती_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        परत false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    जर (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        परत false;
    }

    *ndx_rtrn = ndx;
    परत true;
}

bool
अभिव्यक्ती_Resolve_Group_Mask(स्थिरांक संरचना xkb_keymap_info *keymap_info,
                     स्थिरांक अभिव्यक्ती_Def *अभिव्यक्ती, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    स्थिर स्थिरांक LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { रिक्त, 0 }
    };

    स्थिरांक संरचना named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = आकार("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    परत अभिव्यक्ती_Resolve_Mask_Lookup(keymap_info->keymap.ctx, अभिव्यक्ती, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
