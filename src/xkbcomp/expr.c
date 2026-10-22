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
                                xkb_atom_t क्षेत्रम्_2, uint32_t *val_rtrn,
                                bool *pending);

bool
व्यञ्जकम्_Resolve_Lhs(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
               ध्रुव वर्ण **elem_rtrn, ध्रुव वर्ण **क्षेत्रम्_rtrn,
               व्यञ्जकम्_Def **सूचकांक_rtrn)
{
    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_IDENT:
        *elem_rtrn = रिक्त;
        *क्षेत्रम्_rtrn = xkb_atom_text(ctx, व्यञ्जकम्->ident.ident);
        *सूचकांक_rtrn = रिक्त;
        प्रत्यागम (*क्षेत्रम्_rtrn != रिक्त);
    पक्ष STMT_व्यञ्जकम्_क्षेत्रम्_REF:
        *elem_rtrn = xkb_atom_text(ctx, व्यञ्जकम्->क्षेत्रम्_ref.element);
        *क्षेत्रम्_rtrn = xkb_atom_text(ctx, व्यञ्जकम्->क्षेत्रम्_ref.क्षेत्रम्_2);
        *सूचकांक_rtrn = रिक्त;
        प्रत्यागम (*elem_rtrn != रिक्त && *क्षेत्रम्_rtrn != रिक्त);
    पक्ष STMT_व्यञ्जकम्_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, व्यञ्जकम्->array_ref.element);
        *क्षेत्रम्_rtrn = xkb_atom_text(ctx, व्यञ्जकम्->array_ref.क्षेत्रम्_2);
        *सूचकांक_rtrn = व्यञ्जकम्->array_ref.प्रविष्टिः;
        यदि (व्यञ्जकम्->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == रिक्त)
            प्रत्यागम false;
        यदि (*क्षेत्रम्_rtrn == रिक्त)
            प्रत्यागम false;
        प्रत्यागम true;
    स्वभाव:
        भङ्ग;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", व्यञ्जकम्->common.type);
    प्रत्यागम false;
}

स्थिर bool
SimpleLookup(संरचना xkb_context *ctx, ध्रुव शून्य *priv, xkb_atom_t क्षेत्रम्_2,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    यदि (!priv || क्षेत्रम्_2 == XKB_ATOM_NONE)
        प्रत्यागम false;

    ध्रुव वर्ण *str = xkb_atom_text(ctx, क्षेत्रम्_2);
    प्रति (ध्रुव Lookup_प्रविष्टिः *प्रविष्टिः = priv; प्रविष्टिः && प्रविष्टिः->name; प्रविष्टिः++) {
        यदि (istreq(str, प्रविष्टिः->name)) {
            *val_rtrn = प्रविष्टिः->मानम्;
            प्रत्यागम true;
        }
    }

    प्रत्यागम false;
}

संरचना named_integer_आदर्शः {
    ध्रुव वर्ण *उपसर्गः;
    आकार_t उपसर्गः_दीर्घता;
    uint32_t min;
    uint32_t max;
    ध्रुव Lookup_प्रविष्टिः *entries;
    ध्रुव Lookup_प्रविष्टिः *pending_entries;
    bool is_आवरणम्;
    गणना xkb_सन्देशः_संहिता error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
स्थिर bool
Named_Integer_आदर्शः_Lookup(संरचना xkb_context *ctx, ध्रुव शून्य *priv,
                          xkb_atom_t क्षेत्रम्_2, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    यदि (!priv || क्षेत्रम्_2 == XKB_ATOM_NONE)
        प्रत्यागम false;

    ध्रुव वर्ण * ध्रुव str = xkb_atom_text(ctx, क्षेत्रम्_2);
    ध्रुव संरचना named_integer_आदर्शः * ध्रुव आदर्शः = priv;

    ध्रुव पूर्णाङ्क गणनम् = (istrneq(str, आदर्शः->उपसर्गः, आदर्शः->उपसर्गः_दीर्घता))
        ? parse_dec_गन्तव्यम्_uint32_t(str + आदर्शः->उपसर्गः_दीर्घता, SIZE_MAX, val_rtrn)
        : 0;

    यदि (गणनम् > 0 && *(str + आदर्शः->उपसर्गः_दीर्घता + गणनम्) == '\0') {
        यदि (*val_rtrn < आदर्शः->min || *val_rtrn > आदर्शः->max) {
            log_err(ctx, आदर्शः->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    आदर्शः->उपसर्गः, *val_rtrn, आदर्शः->min, आदर्शः->max);
            प्रत्यागम false;
        }
        यदि (आदर्शः->is_आवरणम्) {
            /* Compute mask (bit 0 = min index) */
            प्रतिज्ञा(*val_rtrn - आदर्शः->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - आदर्शः->min);
        }
        प्रत्यागम true;
    } अन्यथा {
        यदि (आदर्शः->entries &&
            SimpleLookup(ctx, आदर्शः->entries, क्षेत्रम्_2, val_rtrn, रिक्त)) {
            प्रत्यागम true;
        }
        यदि (आदर्शः->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, आदर्शः->pending_entries, क्षेत्रम्_2, val_rtrn, रिक्त)) {
            *pending_rtrn = true;
            प्रत्यागम true;
        }
        प्रत्यागम false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
प्रकारनिर्देश संरचना {
    ध्रुव संरचना xkb_mod_समुच्चयः *mods;
    गणना mod_type mod_type;
} Lookup_Mod_आवरणम्_Priv;

स्थिर bool
Lookup_Mod_आवरणम्(संरचना xkb_context *ctx, ध्रुव शून्य *priv, xkb_atom_t क्षेत्रम्_2,
              xkb_mod_आवरणम्_t *val_rtrn, bool *pending_rtrn)
{
    ध्रुव वर्ण *str = xkb_atom_text(ctx, क्षेत्रम्_2);
    यदि (!str)
        प्रत्यागम false;

    यदि (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_आवरणम्_ALL;
        प्रत्यागम true;
    }

    यदि (istreq(str, "none")) {
        *val_rtrn = 0;
        प्रत्यागम true;
    }

    ध्रुव Lookup_Mod_आवरणम्_Priv *arg = priv;
    ध्रुव संरचना xkb_mod_समुच्चयः *mods = arg->mods;
    ध्रुव गणना mod_type mod_type = arg->mod_type;
    ध्रुव xkb_mod_सूचकांक_t ndx = Xkb_Mod_Name_गन्तव्यम्_सूचकांक(mods, क्षेत्रम्_2, mod_type);
    यदि (ndx == XKB_MOD_INVALID)
        प्रत्यागम false;

    *val_rtrn = (UINT32_C(1) << ndx);
    प्रत्यागम true;
}

bool
व्यञ्जकम्_Resolve_Boolean(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   bool *समुच्चयः_rtrn)
{
    bool ok = false;
    ध्रुव वर्ण *ident;

    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_BOOLEAN_LITERAL:
        *समुच्चयः_rtrn = व्यञ्जकम्->boolean.समुच्चयः;
        प्रत्यागम true;

    पक्ष STMT_व्यञ्जकम्_STRING_LITERAL:
    पक्ष STMT_व्यञ्जकम्_INTEGER_LITERAL:
    पक्ष STMT_व्यञ्जकम्_FLOAT_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_गन्तव्यम्_string(व्यञ्जकम्->common.type));
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_IDENT:
        ident = xkb_atom_text(ctx, व्यञ्जकम्->ident.ident);
        यदि (ident) {
            यदि (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *समुच्चयः_rtrn = true;
                प्रत्यागम true;
            }
            अन्यथा यदि (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *समुच्चयः_rtrn = false;
                प्रत्यागम true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_क्षेत्रम्_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, व्यञ्जकम्->क्षेत्रम्_ref.element),
                xkb_atom_text(ctx, व्यञ्जकम्->क्षेत्रम्_ref.क्षेत्रम्_2));
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_INVERT:
    पक्ष STMT_व्यञ्जकम्_NOT:
        ok = व्यञ्जकम्_Resolve_Boolean(ctx, व्यञ्जकम्->unary.child, समुच्चयः_rtrn);
        यदि (ok)
            *समुच्चयः_rtrn = !*समुच्चयः_rtrn;
        प्रत्यागम ok;
    पक्ष STMT_व्यञ्जकम्_सङ्कलय:
    पक्ष STMT_व्यञ्जकम्_व्यवकलय:
    पक्ष STMT_व्यञ्जकम्_MULTIPLY:
    पक्ष STMT_व्यञ्जकम्_DIVIDE:
    पक्ष STMT_व्यञ्जकम्_नियोजय:
    पक्ष STMT_व्यञ्जकम्_NEGATE:
    पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
    पक्ष STMT_व्यञ्जकम्_रिक्तानि_LIST:
    पक्ष STMT_व्यञ्जकम्_ACTION_DECL:
    पक्ष STMT_व्यञ्जकम्_ACTION_LIST:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_गन्तव्यम्_string(व्यञ्जकम्->common.type));
        भङ्ग;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 व्यञ्जकम्->common.type);
        भङ्ग;
    }

    प्रत्यागम false;
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
        प्रत्यागम true;

    पक्ष STMT_व्यञ्जकम्_STRING_LITERAL:
    पक्ष STMT_व्यञ्जकम्_FLOAT_LITERAL:
    पक्ष STMT_व्यञ्जकम्_BOOLEAN_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_गन्तव्यम्_string(व्यञ्जकम्->common.type));
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_IDENT:
        यदि (lookup)
            ok = lookup(ctx, lookupPriv, व्यञ्जकम्->ident.ident, &u, pending);

        यदि (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, व्यञ्जकम्->ident.ident));
        अन्यथा
            *val_rtrn = (int64_t) u;

        यदि (pending && *pending) {
            /* Postpone computation */
            प्रत्यागम false;
        }

        प्रत्यागम ok;

    पक्ष STMT_व्यञ्जकम्_क्षेत्रम्_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, व्यञ्जकम्->क्षेत्रम्_ref.element),
                xkb_atom_text(ctx, व्यञ्जकम्->क्षेत्रम्_ref.क्षेत्रम्_2));
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_सङ्कलय:
    पक्ष STMT_व्यञ्जकम्_व्यवकलय:
    पक्ष STMT_व्यञ्जकम्_MULTIPLY:
    पक्ष STMT_व्यञ्जकम्_DIVIDE:
        वामः = व्यञ्जकम्->द्विमान.वामः;
        दक्षिणः = व्यञ्जकम्->द्विमान.दक्षिणः;
        यदि (!व्यञ्जकम्_Resolve_Integer_Lookup(ctx, वामः, &l, pending, lookup, lookupPriv) ||
            !व्यञ्जकम्_Resolve_Integer_Lookup(ctx, दक्षिणः, &r, pending, lookup, lookupPriv))
            प्रत्यागम false;

        विकल्प (व्यञ्जकम्->common.type) {
        पक्ष STMT_व्यञ्जकम्_सङ्कलय:
            यदि (ckd_सङ्कलय(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_अतिप्रवाहः,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                प्रत्यागम false;
            }
            भङ्ग;
        पक्ष STMT_व्यञ्जकम्_व्यवकलय:
            यदि (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_अतिप्रवाहः,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                प्रत्यागम false;
            }
            भङ्ग;
        पक्ष STMT_व्यञ्जकम्_MULTIPLY:
            यदि (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_अतिप्रवाहः,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                प्रत्यागम false;
            }
            भङ्ग;
        पक्ष STMT_व्यञ्जकम्_DIVIDE:
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
                    stmt_type_गन्तव्यम्_string(व्यञ्जकम्->common.type));
            प्रत्यागम false;
        }

        प्रत्यागम true;

    पक्ष STMT_व्यञ्जकम्_नियोजय:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        भङ्ग;

    पक्ष STMT_व्यञ्जकम्_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_INVERT:
    पक्ष STMT_व्यञ्जकम्_NEGATE:
        वामः = व्यञ्जकम्->unary.child;
        यदि (!व्यञ्जकम्_Resolve_Integer_Lookup(ctx, वामः, &l, pending, lookup, lookupPriv))
            प्रत्यागम false;

        *val_rtrn = (व्यञ्जकम्->common.type == STMT_व्यञ्जकम्_NEGATE ? -l : ~l);
        प्रत्यागम true;

    पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
        वामः = व्यञ्जकम्->unary.child;
        प्रत्यागम व्यञ्जकम्_Resolve_Integer_Lookup(ctx, वामः, val_rtrn, pending, lookup,
                                        lookupPriv);

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 व्यञ्जकम्->common.type);
        भङ्ग;
    }

    प्रत्यागम false;
}

bool
व्यञ्जकम्_Resolve_Integer(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   int64_t *val_rtrn)
{
    प्रत्यागम व्यञ्जकम्_Resolve_Integer_Lookup(ctx, व्यञ्जकम्, val_rtrn, रिक्त, रिक्त, रिक्त);
}

गणना xkb_parser_error
व्यञ्जकम्_Resolve_गणः(ध्रुव संरचना xkb_keymap_info *keymap_info,
                 ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, bool absolute,
                 xkb_layout_सूचकांक_t *गणः_rtrn, bool *pending)
{
    स्थिर ध्रुव Lookup_प्रविष्टिः pending_गणः_सूचकांक_नामानि[] = {
        { गणः_चरमः_सूचकांक_NAME, 0 },
        { रिक्त, 0 }
    };

    ध्रुव संरचना named_integer_आदर्शः गणः_name_आदर्शः = {
        /* Prefix is title-cased, because it is also used in error messages */
        .उपसर्गः = "Group",
        .उपसर्गः_दीर्घता = परिमाण("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_आवरणम् = false,
        .entries = keymap_info->lookup.गणः_सूचकांक_नामानि,
        .pending_entries = pending_गणः_सूचकांक_नामानि,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
    };

    int64_t परिणाम = 0;
    यदि (!व्यञ्जकम्_Resolve_Integer_Lookup(keymap_info->keymap.ctx, व्यञ्जकम्, &परिणाम, pending,
                                  Named_Integer_आदर्शः_Lookup, &गणः_name_आदर्शः))
        प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    यदि (परिणाम < (int64_t)absolute || परिणाम > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                परिणाम, absolute, keymap_info->features.max_groups);
        प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *गणः_rtrn = (xkb_layout_सूचकांक_t) परिणाम;
    प्रत्यागम PARSER_SUCCESS;
}

bool
व्यञ्जकम्_Resolve_Level(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                 xkb_level_सूचकांक_t *level_rtrn)
{
    स्थिर ध्रुव संरचना named_integer_आदर्शः level_name_आदर्शः = {
        /* Prefix is title-cased, because it is also used in error messages */
        .उपसर्गः = "Level",
        .उपसर्गः_दीर्घता = परिमाण("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_आवरणम् = false,
        .entries = रिक्त,
        .error_id = XKB_ERROR_UNSUPPORTED_सरणम्_LEVEL,
    };
    int64_t परिणाम = 0;
    यदि (!व्यञ्जकम्_Resolve_Integer_Lookup(ctx, व्यञ्जकम्, &परिणाम, रिक्त,
                                  Named_Integer_आदर्शः_Lookup,
                                  &level_name_आदर्शः))
        प्रत्यागम false;

    यदि (परिणाम < 1 || परिणाम > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_सरणम्_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                परिणाम, XKB_LEVEL_MAX_IMPL);
        प्रत्यागम false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_सूचकांक_t) (परिणाम - 1);
    प्रत्यागम true;
}

bool
व्यञ्जकम्_Resolve_Button(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, int64_t *btn_rtrn)
{
    प्रत्यागम व्यञ्जकम्_Resolve_Integer_Lookup(ctx, व्यञ्जकम्, btn_rtrn, रिक्त, SimpleLookup,
                                    button_नामानि);
}

bool
व्यञ्जकम्_Resolve_String(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                  xkb_atom_t *val_rtrn)
{
    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_STRING_LITERAL:
        *val_rtrn = व्यञ्जकम्->string.str;
        प्रत्यागम true;

    पक्ष STMT_व्यञ्जकम्_INTEGER_LITERAL:
    पक्ष STMT_व्यञ्जकम्_FLOAT_LITERAL:
    पक्ष STMT_व्यञ्जकम्_BOOLEAN_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Found %s, expected a string\n",
                stmt_type_गन्तव्यम्_string(व्यञ्जकम्->common.type));
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, व्यञ्जकम्->ident.ident));
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_क्षेत्रम्_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, व्यञ्जकम्->क्षेत्रम्_ref.element),
                xkb_atom_text(ctx, व्यञ्जकम्->क्षेत्रम्_ref.क्षेत्रम्_2));
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_सङ्कलय:
    पक्ष STMT_व्यञ्जकम्_व्यवकलय:
    पक्ष STMT_व्यञ्जकम्_MULTIPLY:
    पक्ष STMT_व्यञ्जकम्_DIVIDE:
    पक्ष STMT_व्यञ्जकम्_नियोजय:
    पक्ष STMT_व्यञ्जकम्_NEGATE:
    पक्ष STMT_व्यञ्जकम्_INVERT:
    पक्ष STMT_व्यञ्जकम्_NOT:
    पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
    पक्ष STMT_व्यञ्जकम्_रिक्तानि_LIST:
    पक्ष STMT_व्यञ्जकम्_ACTION_DECL:
    पक्ष STMT_व्यञ्जकम्_ACTION_LIST:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_गन्तव्यम्_string(व्यञ्जकम्->common.type));
        प्रत्यागम false;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 व्यञ्जकम्->common.type);
        भङ्ग;
    }
    प्रत्यागम false;
}

bool
व्यञ्जकम्_Resolve_Enum(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                uint32_t *val_rtrn, ध्रुव Lookup_प्रविष्टिः *मानानि)
{
    यदि (व्यञ्जकम्->common.type != STMT_व्यञ्जकम्_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_गन्तव्यम्_string(व्यञ्जकम्->common.type));
        प्रत्यागम false;
    }

    यदि (!SimpleLookup(ctx, मानानि, व्यञ्जकम्->ident.ident, val_rtrn, रिक्त)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, व्यञ्जकम्->ident.ident));
        यावत् (मानानि && मानानि->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", मानानि->name);
            मानानि++;
        }
        प्रत्यागम false;
    }

    प्रत्यागम true;
}

स्थिर bool
व्यञ्जकम्_Resolve_आवरणम्_Lookup(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
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
        यदि (व्यञ्जकम्->integer.ival < 0 || व्यञ्जकम्->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    व्यञ्जकम्->integer.ival < 0 ? "-" : "",
                    imaxabs(व्यञ्जकम्->integer.ival),
                    UINT32_MAX);
            प्रत्यागम false;
        }
        *val_rtrn = (uint32_t) व्यञ्जकम्->integer.ival;
        प्रत्यागम true;

    पक्ष STMT_व्यञ्जकम्_STRING_LITERAL:
    पक्ष STMT_व्यञ्जकम्_FLOAT_LITERAL:
    पक्ष STMT_व्यञ्जकम्_BOOLEAN_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
    पक्ष STMT_व्यञ्जकम्_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_गन्तव्यम्_string(व्यञ्जकम्->common.type));
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_IDENT:
        ok = lookup(ctx, lookupPriv, व्यञ्जकम्->ident.ident, val_rtrn, pending);
        यदि (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, व्यञ्जकम्->ident.ident));
        यदि (pending && *pending) {
            /* Postpone computation */
            प्रत्यागम false;
        }
        प्रत्यागम ok;

    पक्ष STMT_व्यञ्जकम्_क्षेत्रम्_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, व्यञ्जकम्->क्षेत्रम्_ref.element),
                xkb_atom_text(ctx, व्यञ्जकम्->क्षेत्रम्_ref.क्षेत्रम्_2));
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    पक्ष STMT_व्यञ्जकम्_ACTION_DECL:
        यदि (bogus == रिक्त)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        प्रत्यागम false;

    पक्ष STMT_व्यञ्जकम्_सङ्कलय:
    पक्ष STMT_व्यञ्जकम्_व्यवकलय:
    पक्ष STMT_व्यञ्जकम्_MULTIPLY:
    पक्ष STMT_व्यञ्जकम्_DIVIDE:
        वामः = व्यञ्जकम्->द्विमान.वामः;
        दक्षिणः = व्यञ्जकम्->द्विमान.दक्षिणः;
        यदि (!व्यञ्जकम्_Resolve_आवरणम्_Lookup(ctx, वामः, &l, pending, lookup, lookupPriv) ||
            !व्यञ्जकम्_Resolve_आवरणम्_Lookup(ctx, दक्षिणः, &r, pending, lookup, lookupPriv))
            प्रत्यागम false;

        विकल्प (व्यञ्जकम्->common.type) {
        पक्ष STMT_व्यञ्जकम्_सङ्कलय:
            *val_rtrn = l | r;
            भङ्ग;
        पक्ष STMT_व्यञ्जकम्_व्यवकलय:
            *val_rtrn = l & (~r);
            भङ्ग;
        पक्ष STMT_व्यञ्जकम्_MULTIPLY:
        पक्ष STMT_व्यञ्जकम्_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (व्यञ्जकम्->common.type == STMT_व्यञ्जकम्_DIVIDE ? "divide" : "multiply"));
            प्रत्यागम false;
        स्वभाव:
            भङ्ग;
        }

        प्रत्यागम true;

    पक्ष STMT_व्यञ्जकम्_नियोजय:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        भङ्ग;

    पक्ष STMT_व्यञ्जकम्_INVERT:
        वामः = व्यञ्जकम्->unary.child;
        यदि (!व्यञ्जकम्_Resolve_Integer_Lookup(ctx, वामः, &v, pending, lookup, lookupPriv))
            प्रत्यागम false;
        यदि (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            प्रत्यागम false;
        }
        *val_rtrn = ~(uint32_t) v;
        प्रत्यागम true;

    पक्ष STMT_व्यञ्जकम्_UNARY_PLUS:
    पक्ष STMT_व्यञ्जकम्_NEGATE:
    पक्ष STMT_व्यञ्जकम्_NOT:
        वामः = व्यञ्जकम्->unary.child;
        यदि (!व्यञ्जकम्_Resolve_Integer_Lookup(ctx, वामः, &v, pending, lookup, lookupPriv))
            प्रत्यागम false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_गन्तव्यम्_operator_char(व्यञ्जकम्->common.type));
        प्रत्यागम false;

    स्वभाव:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 व्यञ्जकम्->common.type);
        भङ्ग;
    }

    प्रत्यागम false;
}

bool
व्यञ्जकम्_Resolve_आवरणम्(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                uint32_t *आवरणम्_rtrn, ध्रुव Lookup_प्रविष्टिः *मानानि)
{
    प्रत्यागम व्यञ्जकम्_Resolve_आवरणम्_Lookup(ctx, व्यञ्जकम्, आवरणम्_rtrn, रिक्त, SimpleLookup, मानानि);
}

bool
व्यञ्जकम्_Resolve_Mod_आवरणम्(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्,
                   गणना mod_type mod_type, ध्रुव संरचना xkb_mod_समुच्चयः *mods,
                   xkb_mod_आवरणम्_t *आवरणम्_rtrn)
{
    Lookup_Mod_आवरणम्_Priv priv = { .mods = mods, .mod_type = mod_type };
    प्रत्यागम व्यञ्जकम्_Resolve_आवरणम्_Lookup(ctx, व्यञ्जकम्, आवरणम्_rtrn, रिक्त, Lookup_Mod_आवरणम्, &priv);
}

bool
व्यञ्जकम्_Resolve_Mod(संरचना xkb_context *ctx, ध्रुव व्यञ्जकम्_Def *def,
               गणना mod_type mod_type, ध्रुव संरचना xkb_mod_समुच्चयः *mods,
               xkb_mod_सूचकांक_t *ndx_rtrn)
{
    यदि (def->common.type != STMT_व्यञ्जकम्_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_गन्तव्यम्_string(def->common.type));
        प्रत्यागम false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_सूचकांक_t ndx = Xkb_Mod_Name_गन्तव्यम्_सूचकांक(mods, name, mod_type);
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
व्यञ्जकम्_Resolve_गणः_आवरणम्(ध्रुव संरचना xkb_keymap_info *keymap_info,
                     ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, xkb_layout_आवरणम्_t *गणः_rtrn,
                     bool *pending_rtrn)
{
    स्थिर ध्रुव Lookup_प्रविष्टिः pending_गणः_आवरणम्_नामानि[] = {
        { गणः_चरमः_सूचकांक_NAME, 0 },
        { रिक्त, 0 }
    };

    ध्रुव संरचना named_integer_आदर्शः गणः_name_आदर्शः = {
        /* Prefix is title-cased, because it is also used in error messages */
        .उपसर्गः = "Group",
        .उपसर्गः_दीर्घता = परिमाण("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_आवरणम् = true,
        .entries = keymap_info->lookup.गणः_आवरणम्_नामानि,
        .pending_entries = pending_गणः_आवरणम्_नामानि,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2
    };

    प्रत्यागम व्यञ्जकम्_Resolve_आवरणम्_Lookup(keymap_info->keymap.ctx, व्यञ्जकम्, गणः_rtrn,
                                 pending_rtrn, Named_Integer_आदर्शः_Lookup,
                                 &गणः_name_आदर्शः);
}
