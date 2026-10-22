/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#include "config.h"

#include <stdint.h>

#include "xkbcommon/xkbcommon.h"
#include "xkbcomp-priv.h"
#include "expr.h"
#include "keymap.h"
#include "keysym.h"
#include "messages-codes.h"
#include "text.h"
#include "xkbcomp/ast.h"
#include "utils.h"
#include "util-numbers.h"
#include "util-checked-arithmetic.h"

typedef bool (*IdentLookupFunc)(sa_nracanaa xkb_context *ctx, const void *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
ExprResolveLhs(sa_nracanaa xkb_context *ctx, const ExprDef *expr,
               const var_na **elem_rtrn, const var_na **field_rtrn,
               ExprDef **index_rtrn)
{
    switch (expr->common.type) {
    pak_sa STMT_EXPR_IDENT:
        *elem_rtrn = NULL;
        *field_rtrn = xkb_atom_text(ctx, expr->ident.ident);
        *index_rtrn = NULL;
        ཕྱིར (*field_rtrn != NULL);
    pak_sa STMT_EXPR_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, expr->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, expr->field_ref.field);
        *index_rtrn = NULL;
        ཕྱིར (*elem_rtrn != NULL && *field_rtrn != NULL);
    pak_sa STMT_EXPR_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, expr->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, expr->array_ref.field);
        *index_rtrn = expr->array_ref.entry;
        གལ་ཏེ (expr->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == NULL)
            ཕྱིར false;
        གལ་ཏེ (*field_rtrn == NULL)
            ཕྱིར false;
        ཕྱིར true;
    default:
        bha_nga;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", expr->common.type);
    ཕྱིར false;
}

static bool
SimpleLookup(sa_nracanaa xkb_context *ctx, const void *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    གལ་ཏེ (!priv || field == XKB_ATOM_NONE)
        ཕྱིར false;

    const var_na *str = xkb_atom_text(ctx, field);
    ཆེད་དུ (const LookupEntry *entry = priv; entry && entry->name; entry++) {
        གལ་ཏེ (istreq(str, entry->name)) {
            *val_rtrn = entry->value;
            ཕྱིར true;
        }
    }

    ཕྱིར false;
}

sa_nracanaa named_integer_pattern {
    const var_na *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    const LookupEntry *entries;
    const LookupEntry *pending_entries;
    bool is_mask;
    ga_nanaa xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
static bool
NamedIntegerPatternLookup(sa_nracanaa xkb_context *ctx, const void *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    གལ་ཏེ (!priv || field == XKB_ATOM_NONE)
        ཕྱིར false;

    const var_na * const str = xkb_atom_text(ctx, field);
    const sa_nracanaa named_integer_pattern * const pattern = priv;

    const ཧྲིལ count = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    གལ་ཏེ (count > 0 && *(str + pattern->prefix_length + count) == '\0') {
        གལ་ཏེ (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            ཕྱིར false;
        }
        གལ་ཏེ (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            assert(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        ཕྱིར true;
    } དེ་མིན {
        གལ་ཏེ (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, NULL)) {
            ཕྱིར true;
        }
        གལ་ཏེ (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, NULL)) {
            *pending_rtrn = true;
            ཕྱིར true;
        }
        ཕྱིར false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
typedef sa_nracanaa {
    const sa_nracanaa xkb_mod_set *mods;
    ga_nanaa mod_type mod_type;
} LookupModMaskPriv;

static bool
LookupModMask(sa_nracanaa xkb_context *ctx, const void *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    const var_na *str = xkb_atom_text(ctx, field);
    གལ་ཏེ (!str)
        ཕྱིར false;

    གལ་ཏེ (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        ཕྱིར true;
    }

    གལ་ཏེ (istreq(str, "none")) {
        *val_rtrn = 0;
        ཕྱིར true;
    }

    const LookupModMaskPriv *arg = priv;
    const sa_nracanaa xkb_mod_set *mods = arg->mods;
    const ga_nanaa mod_type mod_type = arg->mod_type;
    const xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    གལ་ཏེ (ndx == XKB_MOD_INVALID)
        ཕྱིར false;

    *val_rtrn = (UINT32_C(1) << ndx);
    ཕྱིར true;
}

bool
ExprResolveBoolean(sa_nracanaa xkb_context *ctx, const ExprDef *expr,
                   bool *set_rtrn)
{
    bool ok = false;
    const var_na *ident;

    switch (expr->common.type) {
    pak_sa STMT_EXPR_BOOLEAN_LITERAL:
        *set_rtrn = expr->boolean.set;
        ཕྱིར true;

    pak_sa STMT_EXPR_STRING_LITERAL:
    pak_sa STMT_EXPR_INTEGER_LITERAL:
    pak_sa STMT_EXPR_FLOAT_LITERAL:
    pak_sa STMT_EXPR_KEYNAME_LITERAL:
    pak_sa STMT_EXPR_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(expr->common.type));
        ཕྱིར false;

    pak_sa STMT_EXPR_IDENT:
        ident = xkb_atom_text(ctx, expr->ident.ident);
        གལ་ཏེ (ident) {
            གལ་ཏེ (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                ཕྱིར true;
            }
            དེ་མིན གལ་ཏེ (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                ཕྱིར true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        ཕྱིར false;

    pak_sa STMT_EXPR_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, expr->field_ref.element),
                xkb_atom_text(ctx, expr->field_ref.field));
        ཕྱིར false;

    pak_sa STMT_EXPR_INVERT:
    pak_sa STMT_EXPR_NOT:
        ok = ExprResolveBoolean(ctx, expr->unary.child, set_rtrn);
        གལ་ཏེ (ok)
            *set_rtrn = !*set_rtrn;
        ཕྱིར ok;
    pak_sa STMT_EXPR_ADD:
    pak_sa STMT_EXPR_SUBTRACT:
    pak_sa STMT_EXPR_MULTIPLY:
    pak_sa STMT_EXPR_DIVIDE:
    pak_sa STMT_EXPR_ASSIGN:
    pak_sa STMT_EXPR_NEGATE:
    pak_sa STMT_EXPR_UNARY_PLUS:
    pak_sa STMT_EXPR_EMPTY_LIST:
    pak_sa STMT_EXPR_ACTION_DECL:
    pak_sa STMT_EXPR_ACTION_LIST:
    pak_sa STMT_EXPR_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(expr->common.type));
        bha_nga;

    default:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 expr->common.type);
        bha_nga;
    }

    ཕྱིར false;
}

static bool
ExprResolveIntegerLookup(sa_nracanaa xkb_context *ctx, const ExprDef *expr,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, const void *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    ExprDef *left, *right;

    switch (expr->common.type) {
    pak_sa STMT_EXPR_INTEGER_LITERAL:
        *val_rtrn = expr->integer.ival;
        ཕྱིར true;

    pak_sa STMT_EXPR_STRING_LITERAL:
    pak_sa STMT_EXPR_FLOAT_LITERAL:
    pak_sa STMT_EXPR_BOOLEAN_LITERAL:
    pak_sa STMT_EXPR_KEYNAME_LITERAL:
    pak_sa STMT_EXPR_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(expr->common.type));
        ཕྱིར false;

    pak_sa STMT_EXPR_IDENT:
        གལ་ཏེ (lookup)
            ok = lookup(ctx, lookupPriv, expr->ident.ident, &u, pending);

        གལ་ཏེ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, expr->ident.ident));
        དེ་མིན
            *val_rtrn = (int64_t) u;

        གལ་ཏེ (pending && *pending) {
            /* Postpone computation */
            ཕྱིར false;
        }

        ཕྱིར ok;

    pak_sa STMT_EXPR_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, expr->field_ref.element),
                xkb_atom_text(ctx, expr->field_ref.field));
        ཕྱིར false;

    pak_sa STMT_EXPR_ADD:
    pak_sa STMT_EXPR_SUBTRACT:
    pak_sa STMT_EXPR_MULTIPLY:
    pak_sa STMT_EXPR_DIVIDE:
        left = expr->binary.left;
        right = expr->binary.right;
        གལ་ཏེ (!ExprResolveIntegerLookup(ctx, left, &l, pending, lookup, lookupPriv) ||
            !ExprResolveIntegerLookup(ctx, right, &r, pending, lookup, lookupPriv))
            ཕྱིར false;

        switch (expr->common.type) {
        pak_sa STMT_EXPR_ADD:
            གལ་ཏེ (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ཕྱིར false;
            }
            bha_nga;
        pak_sa STMT_EXPR_SUBTRACT:
            གལ་ཏེ (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ཕྱིར false;
            }
            bha_nga;
        pak_sa STMT_EXPR_MULTIPLY:
            གལ་ཏེ (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ཕྱིར false;
            }
            bha_nga;
        pak_sa STMT_EXPR_DIVIDE:
            གལ་ཏེ (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                ཕྱིར false;
            }
            *val_rtrn = l / r;
            bha_nga;
        default:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(expr->common.type));
            ཕྱིར false;
        }

        ཕྱིར true;

    pak_sa STMT_EXPR_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        bha_nga;

    pak_sa STMT_EXPR_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        ཕྱིར false;

    pak_sa STMT_EXPR_INVERT:
    pak_sa STMT_EXPR_NEGATE:
        left = expr->unary.child;
        གལ་ཏེ (!ExprResolveIntegerLookup(ctx, left, &l, pending, lookup, lookupPriv))
            ཕྱིར false;

        *val_rtrn = (expr->common.type == STMT_EXPR_NEGATE ? -l : ~l);
        ཕྱིར true;

    pak_sa STMT_EXPR_UNARY_PLUS:
        left = expr->unary.child;
        ཕྱིར ExprResolveIntegerLookup(ctx, left, val_rtrn, pending, lookup,
                                        lookupPriv);

    default:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 expr->common.type);
        bha_nga;
    }

    ཕྱིར false;
}

bool
ExprResolveInteger(sa_nracanaa xkb_context *ctx, const ExprDef *expr,
                   int64_t *val_rtrn)
{
    ཕྱིར ExprResolveIntegerLookup(ctx, expr, val_rtrn, NULL, NULL, NULL);
}

ga_nanaa xkb_parser_error
ExprResolveGroup(const sa_nracanaa xkb_keymap_info *keymap_info,
                 const ExprDef *expr, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    static const LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { NULL, 0 }
    };

    const sa_nracanaa named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = parimaa_na("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    གལ་ཏེ (!ExprResolveIntegerLookup(keymap_info->keymap.ctx, expr, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        ཕྱིར (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    གལ་ཏེ (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        ཕྱིར (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    ཕྱིར PARSER_SUCCESS;
}

bool
ExprResolveLevel(sa_nracanaa xkb_context *ctx, const ExprDef *expr,
                 xkb_level_index_t *level_rtrn)
{
    static const sa_nracanaa named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = parimaa_na("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = NULL,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    གལ་ཏེ (!ExprResolveIntegerLookup(ctx, expr, &result, NULL,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        ཕྱིར false;

    གལ་ཏེ (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        ཕྱིར false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    ཕྱིར true;
}

bool
ExprResolveButton(sa_nracanaa xkb_context *ctx, const ExprDef *expr, int64_t *btn_rtrn)
{
    ཕྱིར ExprResolveIntegerLookup(ctx, expr, btn_rtrn, NULL, SimpleLookup,
                                    buttonNames);
}

bool
ExprResolveString(sa_nracanaa xkb_context *ctx, const ExprDef *expr,
                  xkb_atom_t *val_rtrn)
{
    switch (expr->common.type) {
    pak_sa STMT_EXPR_STRING_LITERAL:
        *val_rtrn = expr->string.str;
        ཕྱིར true;

    pak_sa STMT_EXPR_INTEGER_LITERAL:
    pak_sa STMT_EXPR_FLOAT_LITERAL:
    pak_sa STMT_EXPR_BOOLEAN_LITERAL:
    pak_sa STMT_EXPR_KEYNAME_LITERAL:
    pak_sa STMT_EXPR_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(expr->common.type));
        ཕྱིར false;

    pak_sa STMT_EXPR_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, expr->ident.ident));
        ཕྱིར false;

    pak_sa STMT_EXPR_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, expr->field_ref.element),
                xkb_atom_text(ctx, expr->field_ref.field));
        ཕྱིར false;

    pak_sa STMT_EXPR_ADD:
    pak_sa STMT_EXPR_SUBTRACT:
    pak_sa STMT_EXPR_MULTIPLY:
    pak_sa STMT_EXPR_DIVIDE:
    pak_sa STMT_EXPR_ASSIGN:
    pak_sa STMT_EXPR_NEGATE:
    pak_sa STMT_EXPR_INVERT:
    pak_sa STMT_EXPR_NOT:
    pak_sa STMT_EXPR_UNARY_PLUS:
    pak_sa STMT_EXPR_EMPTY_LIST:
    pak_sa STMT_EXPR_ACTION_DECL:
    pak_sa STMT_EXPR_ACTION_LIST:
    pak_sa STMT_EXPR_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(expr->common.type));
        ཕྱིར false;

    default:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 expr->common.type);
        bha_nga;
    }
    ཕྱིར false;
}

bool
ExprResolveEnum(sa_nracanaa xkb_context *ctx, const ExprDef *expr,
                uint32_t *val_rtrn, const LookupEntry *values)
{
    གལ་ཏེ (expr->common.type != STMT_EXPR_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(expr->common.type));
        ཕྱིར false;
    }

    གལ་ཏེ (!SimpleLookup(ctx, values, expr->ident.ident, val_rtrn, NULL)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, expr->ident.ident));
        སྐབས (values && values->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", values->name);
            values++;
        }
        ཕྱིར false;
    }

    ཕྱིར true;
}

static bool
ExprResolveMaskLookup(sa_nracanaa xkb_context *ctx, const ExprDef *expr,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, const void *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    ExprDef *left, *right;
    const var_na *bogus = NULL;

    switch (expr->common.type) {
    pak_sa STMT_EXPR_INTEGER_LITERAL:
        གལ་ཏེ (expr->integer.ival < 0 || expr->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    expr->integer.ival < 0 ? "-" : "",
                    imaxabs(expr->integer.ival),
                    UINT32_MAX);
            ཕྱིར false;
        }
        *val_rtrn = (uint32_t) expr->integer.ival;
        ཕྱིར true;

    pak_sa STMT_EXPR_STRING_LITERAL:
    pak_sa STMT_EXPR_FLOAT_LITERAL:
    pak_sa STMT_EXPR_BOOLEAN_LITERAL:
    pak_sa STMT_EXPR_KEYNAME_LITERAL:
    pak_sa STMT_EXPR_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(expr->common.type));
        ཕྱིར false;

    pak_sa STMT_EXPR_IDENT:
        ok = lookup(ctx, lookupPriv, expr->ident.ident, val_rtrn, pending);
        གལ་ཏེ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, expr->ident.ident));
        གལ་ཏེ (pending && *pending) {
            /* Postpone computation */
            ཕྱིར false;
        }
        ཕྱིར ok;

    pak_sa STMT_EXPR_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, expr->field_ref.element),
                xkb_atom_text(ctx, expr->field_ref.field));
        ཕྱིར false;

    pak_sa STMT_EXPR_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    pak_sa STMT_EXPR_ACTION_DECL:
        གལ་ཏེ (bogus == NULL)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        ཕྱིར false;

    pak_sa STMT_EXPR_ADD:
    pak_sa STMT_EXPR_SUBTRACT:
    pak_sa STMT_EXPR_MULTIPLY:
    pak_sa STMT_EXPR_DIVIDE:
        left = expr->binary.left;
        right = expr->binary.right;
        གལ་ཏེ (!ExprResolveMaskLookup(ctx, left, &l, pending, lookup, lookupPriv) ||
            !ExprResolveMaskLookup(ctx, right, &r, pending, lookup, lookupPriv))
            ཕྱིར false;

        switch (expr->common.type) {
        pak_sa STMT_EXPR_ADD:
            *val_rtrn = l | r;
            bha_nga;
        pak_sa STMT_EXPR_SUBTRACT:
            *val_rtrn = l & (~r);
            bha_nga;
        pak_sa STMT_EXPR_MULTIPLY:
        pak_sa STMT_EXPR_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (expr->common.type == STMT_EXPR_DIVIDE ? "divide" : "multiply"));
            ཕྱིར false;
        default:
            bha_nga;
        }

        ཕྱིར true;

    pak_sa STMT_EXPR_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        bha_nga;

    pak_sa STMT_EXPR_INVERT:
        left = expr->unary.child;
        གལ་ཏེ (!ExprResolveIntegerLookup(ctx, left, &v, pending, lookup, lookupPriv))
            ཕྱིར false;
        གལ་ཏེ (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            ཕྱིར false;
        }
        *val_rtrn = ~(uint32_t) v;
        ཕྱིར true;

    pak_sa STMT_EXPR_UNARY_PLUS:
    pak_sa STMT_EXPR_NEGATE:
    pak_sa STMT_EXPR_NOT:
        left = expr->unary.child;
        གལ་ཏེ (!ExprResolveIntegerLookup(ctx, left, &v, pending, lookup, lookupPriv))
            ཕྱིར false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(expr->common.type));
        ཕྱིར false;

    default:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 expr->common.type);
        bha_nga;
    }

    ཕྱིར false;
}

bool
ExprResolveMask(sa_nracanaa xkb_context *ctx, const ExprDef *expr,
                uint32_t *mask_rtrn, const LookupEntry *values)
{
    ཕྱིར ExprResolveMaskLookup(ctx, expr, mask_rtrn, NULL, SimpleLookup, values);
}

bool
ExprResolveModMask(sa_nracanaa xkb_context *ctx, const ExprDef *expr,
                   ga_nanaa mod_type mod_type, const sa_nracanaa xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    ཕྱིར ExprResolveMaskLookup(ctx, expr, mask_rtrn, NULL, LookupModMask, &priv);
}

bool
ExprResolveMod(sa_nracanaa xkb_context *ctx, const ExprDef *def,
               ga_nanaa mod_type mod_type, const sa_nracanaa xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    གལ་ཏེ (def->common.type != STMT_EXPR_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        ཕྱིར false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    གལ་ཏེ (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        ཕྱིར false;
    }

    *ndx_rtrn = ndx;
    ཕྱིར true;
}

bool
ExprResolveGroupMask(const sa_nracanaa xkb_keymap_info *keymap_info,
                     const ExprDef *expr, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    static const LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { NULL, 0 }
    };

    const sa_nracanaa named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = parimaa_na("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    ཕྱིར ExprResolveMaskLookup(keymap_info->keymap.ctx, expr, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
