/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#समावेश "config.h"

#समावेश <stdint.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcomp-priv.h"
#समावेश "expr.h"
#समावेश "keymap.h"
#समावेश "keysym.h"
#समावेश "messages-codes.h"
#समावेश "text.h"
#समावेश "xkbcomp/ast.h"
#समावेश "utils.h"
#समावेश "util-numbers.h"
#समावेश "util-checked-arithmetic.h"

प्रकार bool (*IdentLookupFunc)(काष्ठा xkb_context *ctx, स्थिर व्योम *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
व्यंजक_Resolve_Lhs(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
               स्थिर अक्षर **elem_rtrn, स्थिर अक्षर **field_rtrn,
               व्यंजक_Def **सूचकांक_rtrn)
{
    चयन (व्यंजक->common.type) {
    हाल STMT_व्यंजक_IDENT:
        *elem_rtrn = शून्य;
        *field_rtrn = xkb_atom_text(ctx, व्यंजक->ident.ident);
        *सूचकांक_rtrn = शून्य;
        वापस (*field_rtrn != शून्य);
    हाल STMT_व्यंजक_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, व्यंजक->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, व्यंजक->field_ref.field);
        *सूचकांक_rtrn = शून्य;
        वापस (*elem_rtrn != शून्य && *field_rtrn != शून्य);
    हाल STMT_व्यंजक_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, व्यंजक->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, व्यंजक->array_ref.field);
        *सूचकांक_rtrn = व्यंजक->array_ref.entry;
        अगर (व्यंजक->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == शून्य)
            वापस false;
        अगर (*field_rtrn == शून्य)
            वापस false;
        वापस true;
    शेष:
        अवरोध;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", व्यंजक->common.type);
    वापस false;
}

अटल bool
SimpleLookup(काष्ठा xkb_context *ctx, स्थिर व्योम *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    अगर (!priv || field == XKB_ATOM_NONE)
        वापस false;

    स्थिर अक्षर *str = xkb_atom_text(ctx, field);
    क्रम (स्थिर LookupEntry *entry = priv; entry && entry->name; entry++) {
        अगर (istreq(str, entry->name)) {
            *val_rtrn = entry->मूल्य;
            वापस true;
        }
    }

    वापस false;
}

काष्ठा named_integer_pattern {
    स्थिर अक्षर *prefix;
    माप_प्रकार prefix_लंबाई;
    uint32_t min;
    uint32_t max;
    स्थिर LookupEntry *entries;
    स्थिर LookupEntry *pending_entries;
    bool is_mask;
    क्रमागत xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
अटल bool
NamedIntegerPatternLookup(काष्ठा xkb_context *ctx, स्थिर व्योम *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    अगर (!priv || field == XKB_ATOM_NONE)
        वापस false;

    स्थिर अक्षर * स्थिर str = xkb_atom_text(ctx, field);
    स्थिर काष्ठा named_integer_pattern * स्थिर pattern = priv;

    स्थिर पूर्णांक गिनती = (istrneq(str, pattern->prefix, pattern->prefix_लंबाई))
        ? parse_dec_to_uint32_t(str + pattern->prefix_लंबाई, SIZE_MAX, val_rtrn)
        : 0;

    अगर (गिनती > 0 && *(str + pattern->prefix_लंबाई + गिनती) == '\0') {
        अगर (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            वापस false;
        }
        अगर (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            निश्चित(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        वापस true;
    } अन्यथा {
        अगर (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, शून्य)) {
            वापस true;
        }
        अगर (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, शून्य)) {
            *pending_rtrn = true;
            वापस true;
        }
        वापस false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
प्रकार काष्ठा {
    स्थिर काष्ठा xkb_mod_set *mods;
    क्रमागत mod_type mod_type;
} LookupModMaskPriv;

अटल bool
LookupModMask(काष्ठा xkb_context *ctx, स्थिर व्योम *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    स्थिर अक्षर *str = xkb_atom_text(ctx, field);
    अगर (!str)
        वापस false;

    अगर (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        वापस true;
    }

    अगर (istreq(str, "none")) {
        *val_rtrn = 0;
        वापस true;
    }

    स्थिर LookupModMaskPriv *arg = priv;
    स्थिर काष्ठा xkb_mod_set *mods = arg->mods;
    स्थिर क्रमागत mod_type mod_type = arg->mod_type;
    स्थिर xkb_mod_सूचकांक_t ndx = Xkb_Mod_Name_To_सूचकांक(mods, field, mod_type);
    अगर (ndx == XKB_MOD_INVALID)
        वापस false;

    *val_rtrn = (UINT32_C(1) << ndx);
    वापस true;
}

bool
व्यंजक_Resolve_Boolean(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                   bool *set_rtrn)
{
    bool ok = false;
    स्थिर अक्षर *ident;

    चयन (व्यंजक->common.type) {
    हाल STMT_व्यंजक_BOOLEAN_LITERAL:
        *set_rtrn = व्यंजक->boolean.set;
        वापस true;

    हाल STMT_व्यंजक_STRING_LITERAL:
    हाल STMT_व्यंजक_INTEGER_LITERAL:
    हाल STMT_व्यंजक_FLOAT_LITERAL:
    हाल STMT_व्यंजक_KEYNAME_LITERAL:
    हाल STMT_व्यंजक_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(व्यंजक->common.type));
        वापस false;

    हाल STMT_व्यंजक_IDENT:
        ident = xkb_atom_text(ctx, व्यंजक->ident.ident);
        अगर (ident) {
            अगर (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                वापस true;
            }
            अन्यथा अगर (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                वापस true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        वापस false;

    हाल STMT_व्यंजक_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, व्यंजक->field_ref.element),
                xkb_atom_text(ctx, व्यंजक->field_ref.field));
        वापस false;

    हाल STMT_व्यंजक_INVERT:
    हाल STMT_व्यंजक_NOT:
        ok = व्यंजक_Resolve_Boolean(ctx, व्यंजक->unary.child, set_rtrn);
        अगर (ok)
            *set_rtrn = !*set_rtrn;
        वापस ok;
    हाल STMT_व्यंजक_ADD:
    हाल STMT_व्यंजक_SUBTRACT:
    हाल STMT_व्यंजक_MULTIPLY:
    हाल STMT_व्यंजक_DIVIDE:
    हाल STMT_व्यंजक_ASSIGN:
    हाल STMT_व्यंजक_NEGATE:
    हाल STMT_व्यंजक_UNARY_PLUS:
    हाल STMT_व्यंजक_EMPTY_LIST:
    हाल STMT_व्यंजक_ACTION_DECL:
    हाल STMT_व्यंजक_ACTION_LIST:
    हाल STMT_व्यंजक_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(व्यंजक->common.type));
        अवरोध;

    शेष:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 व्यंजक->common.type);
        अवरोध;
    }

    वापस false;
}

अटल bool
व्यंजक_Resolve_Integer_Lookup(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, स्थिर व्योम *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    व्यंजक_Def *बायाँ, *दायाँ;

    चयन (व्यंजक->common.type) {
    हाल STMT_व्यंजक_INTEGER_LITERAL:
        *val_rtrn = व्यंजक->integer.ival;
        वापस true;

    हाल STMT_व्यंजक_STRING_LITERAL:
    हाल STMT_व्यंजक_FLOAT_LITERAL:
    हाल STMT_व्यंजक_BOOLEAN_LITERAL:
    हाल STMT_व्यंजक_KEYNAME_LITERAL:
    हाल STMT_व्यंजक_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(व्यंजक->common.type));
        वापस false;

    हाल STMT_व्यंजक_IDENT:
        अगर (lookup)
            ok = lookup(ctx, lookupPriv, व्यंजक->ident.ident, &u, pending);

        अगर (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, व्यंजक->ident.ident));
        अन्यथा
            *val_rtrn = (int64_t) u;

        अगर (pending && *pending) {
            /* Postpone computation */
            वापस false;
        }

        वापस ok;

    हाल STMT_व्यंजक_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, व्यंजक->field_ref.element),
                xkb_atom_text(ctx, व्यंजक->field_ref.field));
        वापस false;

    हाल STMT_व्यंजक_ADD:
    हाल STMT_व्यंजक_SUBTRACT:
    हाल STMT_व्यंजक_MULTIPLY:
    हाल STMT_व्यंजक_DIVIDE:
        बायाँ = व्यंजक->द्विआधारी.बायाँ;
        दायाँ = व्यंजक->द्विआधारी.दायाँ;
        अगर (!व्यंजक_Resolve_Integer_Lookup(ctx, बायाँ, &l, pending, lookup, lookupPriv) ||
            !व्यंजक_Resolve_Integer_Lookup(ctx, दायाँ, &r, pending, lookup, lookupPriv))
            वापस false;

        चयन (व्यंजक->common.type) {
        हाल STMT_व्यंजक_ADD:
            अगर (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                वापस false;
            }
            अवरोध;
        हाल STMT_व्यंजक_SUBTRACT:
            अगर (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                वापस false;
            }
            अवरोध;
        हाल STMT_व्यंजक_MULTIPLY:
            अगर (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                वापस false;
            }
            अवरोध;
        हाल STMT_व्यंजक_DIVIDE:
            अगर (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                वापस false;
            }
            *val_rtrn = l / r;
            अवरोध;
        शेष:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(व्यंजक->common.type));
            वापस false;
        }

        वापस true;

    हाल STMT_व्यंजक_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        अवरोध;

    हाल STMT_व्यंजक_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        वापस false;

    हाल STMT_व्यंजक_INVERT:
    हाल STMT_व्यंजक_NEGATE:
        बायाँ = व्यंजक->unary.child;
        अगर (!व्यंजक_Resolve_Integer_Lookup(ctx, बायाँ, &l, pending, lookup, lookupPriv))
            वापस false;

        *val_rtrn = (व्यंजक->common.type == STMT_व्यंजक_NEGATE ? -l : ~l);
        वापस true;

    हाल STMT_व्यंजक_UNARY_PLUS:
        बायाँ = व्यंजक->unary.child;
        वापस व्यंजक_Resolve_Integer_Lookup(ctx, बायाँ, val_rtrn, pending, lookup,
                                        lookupPriv);

    शेष:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 व्यंजक->common.type);
        अवरोध;
    }

    वापस false;
}

bool
व्यंजक_Resolve_Integer(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                   int64_t *val_rtrn)
{
    वापस व्यंजक_Resolve_Integer_Lookup(ctx, व्यंजक, val_rtrn, शून्य, शून्य, शून्य);
}

क्रमागत xkb_parser_error
व्यंजक_Resolve_Group(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                 स्थिर व्यंजक_Def *व्यंजक, bool absolute,
                 xkb_layout_सूचकांक_t *group_rtrn, bool *pending)
{
    अटल स्थिर LookupEntry pending_Group_सूचकांक_Names[] = {
        { GROUP_LAST_सूचकांक_NAME, 0 },
        { शून्य, 0 }
    };

    स्थिर काष्ठा named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_लंबाई = माप("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.group_सूचकांक_Names,
        .pending_entries = pending_Group_सूचकांक_Names,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
    };

    int64_t परिणाम = 0;
    अगर (!व्यंजक_Resolve_Integer_Lookup(keymap_info->keymap.ctx, व्यंजक, &परिणाम, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    अगर (परिणाम < (int64_t)absolute || परिणाम > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                परिणाम, absolute, keymap_info->features.max_groups);
        वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_सूचकांक_t) परिणाम;
    वापस PARSER_SUCCESS;
}

bool
व्यंजक_Resolve_Level(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                 xkb_level_सूचकांक_t *level_rtrn)
{
    अटल स्थिर काष्ठा named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_लंबाई = माप("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = शून्य,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t परिणाम = 0;
    अगर (!व्यंजक_Resolve_Integer_Lookup(ctx, व्यंजक, &परिणाम, शून्य,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        वापस false;

    अगर (परिणाम < 1 || परिणाम > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                परिणाम, XKB_LEVEL_MAX_IMPL);
        वापस false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_सूचकांक_t) (परिणाम - 1);
    वापस true;
}

bool
व्यंजक_Resolve_Button(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक, int64_t *btn_rtrn)
{
    वापस व्यंजक_Resolve_Integer_Lookup(ctx, व्यंजक, btn_rtrn, शून्य, SimpleLookup,
                                    buttonNames);
}

bool
व्यंजक_Resolve_String(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                  xkb_atom_t *val_rtrn)
{
    चयन (व्यंजक->common.type) {
    हाल STMT_व्यंजक_STRING_LITERAL:
        *val_rtrn = व्यंजक->string.str;
        वापस true;

    हाल STMT_व्यंजक_INTEGER_LITERAL:
    हाल STMT_व्यंजक_FLOAT_LITERAL:
    हाल STMT_व्यंजक_BOOLEAN_LITERAL:
    हाल STMT_व्यंजक_KEYNAME_LITERAL:
    हाल STMT_व्यंजक_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(व्यंजक->common.type));
        वापस false;

    हाल STMT_व्यंजक_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, व्यंजक->ident.ident));
        वापस false;

    हाल STMT_व्यंजक_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, व्यंजक->field_ref.element),
                xkb_atom_text(ctx, व्यंजक->field_ref.field));
        वापस false;

    हाल STMT_व्यंजक_ADD:
    हाल STMT_व्यंजक_SUBTRACT:
    हाल STMT_व्यंजक_MULTIPLY:
    हाल STMT_व्यंजक_DIVIDE:
    हाल STMT_व्यंजक_ASSIGN:
    हाल STMT_व्यंजक_NEGATE:
    हाल STMT_व्यंजक_INVERT:
    हाल STMT_व्यंजक_NOT:
    हाल STMT_व्यंजक_UNARY_PLUS:
    हाल STMT_व्यंजक_EMPTY_LIST:
    हाल STMT_व्यंजक_ACTION_DECL:
    हाल STMT_व्यंजक_ACTION_LIST:
    हाल STMT_व्यंजक_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(व्यंजक->common.type));
        वापस false;

    शेष:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 व्यंजक->common.type);
        अवरोध;
    }
    वापस false;
}

bool
व्यंजक_Resolve_Enum(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                uint32_t *val_rtrn, स्थिर LookupEntry *मूल्यावली)
{
    अगर (व्यंजक->common.type != STMT_व्यंजक_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(व्यंजक->common.type));
        वापस false;
    }

    अगर (!SimpleLookup(ctx, मूल्यावली, व्यंजक->ident.ident, val_rtrn, शून्य)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, व्यंजक->ident.ident));
        जबतक (मूल्यावली && मूल्यावली->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", मूल्यावली->name);
            मूल्यावली++;
        }
        वापस false;
    }

    वापस true;
}

अटल bool
व्यंजक_Resolve_Mask_Lookup(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, स्थिर व्योम *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    व्यंजक_Def *बायाँ, *दायाँ;
    स्थिर अक्षर *bogus = शून्य;

    चयन (व्यंजक->common.type) {
    हाल STMT_व्यंजक_INTEGER_LITERAL:
        अगर (व्यंजक->integer.ival < 0 || व्यंजक->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    व्यंजक->integer.ival < 0 ? "-" : "",
                    imaxabs(व्यंजक->integer.ival),
                    UINT32_MAX);
            वापस false;
        }
        *val_rtrn = (uint32_t) व्यंजक->integer.ival;
        वापस true;

    हाल STMT_व्यंजक_STRING_LITERAL:
    हाल STMT_व्यंजक_FLOAT_LITERAL:
    हाल STMT_व्यंजक_BOOLEAN_LITERAL:
    हाल STMT_व्यंजक_KEYNAME_LITERAL:
    हाल STMT_व्यंजक_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(व्यंजक->common.type));
        वापस false;

    हाल STMT_व्यंजक_IDENT:
        ok = lookup(ctx, lookupPriv, व्यंजक->ident.ident, val_rtrn, pending);
        अगर (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, व्यंजक->ident.ident));
        अगर (pending && *pending) {
            /* Postpone computation */
            वापस false;
        }
        वापस ok;

    हाल STMT_व्यंजक_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, व्यंजक->field_ref.element),
                xkb_atom_text(ctx, व्यंजक->field_ref.field));
        वापस false;

    हाल STMT_व्यंजक_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    हाल STMT_व्यंजक_ACTION_DECL:
        अगर (bogus == शून्य)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        वापस false;

    हाल STMT_व्यंजक_ADD:
    हाल STMT_व्यंजक_SUBTRACT:
    हाल STMT_व्यंजक_MULTIPLY:
    हाल STMT_व्यंजक_DIVIDE:
        बायाँ = व्यंजक->द्विआधारी.बायाँ;
        दायाँ = व्यंजक->द्विआधारी.दायाँ;
        अगर (!व्यंजक_Resolve_Mask_Lookup(ctx, बायाँ, &l, pending, lookup, lookupPriv) ||
            !व्यंजक_Resolve_Mask_Lookup(ctx, दायाँ, &r, pending, lookup, lookupPriv))
            वापस false;

        चयन (व्यंजक->common.type) {
        हाल STMT_व्यंजक_ADD:
            *val_rtrn = l | r;
            अवरोध;
        हाल STMT_व्यंजक_SUBTRACT:
            *val_rtrn = l & (~r);
            अवरोध;
        हाल STMT_व्यंजक_MULTIPLY:
        हाल STMT_व्यंजक_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (व्यंजक->common.type == STMT_व्यंजक_DIVIDE ? "divide" : "multiply"));
            वापस false;
        शेष:
            अवरोध;
        }

        वापस true;

    हाल STMT_व्यंजक_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        अवरोध;

    हाल STMT_व्यंजक_INVERT:
        बायाँ = व्यंजक->unary.child;
        अगर (!व्यंजक_Resolve_Integer_Lookup(ctx, बायाँ, &v, pending, lookup, lookupPriv))
            वापस false;
        अगर (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            वापस false;
        }
        *val_rtrn = ~(uint32_t) v;
        वापस true;

    हाल STMT_व्यंजक_UNARY_PLUS:
    हाल STMT_व्यंजक_NEGATE:
    हाल STMT_व्यंजक_NOT:
        बायाँ = व्यंजक->unary.child;
        अगर (!व्यंजक_Resolve_Integer_Lookup(ctx, बायाँ, &v, pending, lookup, lookupPriv))
            वापस false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(व्यंजक->common.type));
        वापस false;

    शेष:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 व्यंजक->common.type);
        अवरोध;
    }

    वापस false;
}

bool
व्यंजक_Resolve_Mask(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                uint32_t *mask_rtrn, स्थिर LookupEntry *मूल्यावली)
{
    वापस व्यंजक_Resolve_Mask_Lookup(ctx, व्यंजक, mask_rtrn, शून्य, SimpleLookup, मूल्यावली);
}

bool
व्यंजक_Resolve_Mod_Mask(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *व्यंजक,
                   क्रमागत mod_type mod_type, स्थिर काष्ठा xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    वापस व्यंजक_Resolve_Mask_Lookup(ctx, व्यंजक, mask_rtrn, शून्य, LookupModMask, &priv);
}

bool
व्यंजक_Resolve_Mod(काष्ठा xkb_context *ctx, स्थिर व्यंजक_Def *def,
               क्रमागत mod_type mod_type, स्थिर काष्ठा xkb_mod_set *mods,
               xkb_mod_सूचकांक_t *ndx_rtrn)
{
    अगर (def->common.type != STMT_व्यंजक_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        वापस false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_सूचकांक_t ndx = Xkb_Mod_Name_To_सूचकांक(mods, name, mod_type);
    अगर (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        वापस false;
    }

    *ndx_rtrn = ndx;
    वापस true;
}

bool
व्यंजक_Resolve_Group_Mask(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                     स्थिर व्यंजक_Def *व्यंजक, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    अटल स्थिर LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_सूचकांक_NAME, 0 },
        { शून्य, 0 }
    };

    स्थिर काष्ठा named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_लंबाई = माप("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2
    };

    वापस व्यंजक_Resolve_Mask_Lookup(keymap_info->keymap.ctx, व्यंजक, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
