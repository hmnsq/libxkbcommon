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

typedef bool (*IdentLookupFunc)(struct xkb_context *ctx, const void *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
पाहायाग_Resolve_Lhs(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
               const char **elem_rtrn, const char **field_rtrn,
               पाहायाग_Def **index_rtrn)
{
    switch (पाहायाग->common.type) {
    case STMT_पाहायाग_IDENT:
        *elem_rtrn = NULL;
        *field_rtrn = xkb_atom_text(ctx, पाहायाग->ident.ident);
        *index_rtrn = NULL;
        return (*field_rtrn != NULL);
    case STMT_पाहायाग_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, पाहायाग->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, पाहायाग->field_ref.field);
        *index_rtrn = NULL;
        return (*elem_rtrn != NULL && *field_rtrn != NULL);
    case STMT_पाहायाग_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, पाहायाग->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, पाहायाग->array_ref.field);
        *index_rtrn = पाहायाग->array_ref.entry;
        if (पाहायाग->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == NULL)
            return false;
        if (*field_rtrn == NULL)
            return false;
        return true;
    default:
        break;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", पाहायाग->common.type);
    return false;
}

static bool
SimpleLookup(struct xkb_context *ctx, const void *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    if (!priv || field == XKB_ATOM_NONE)
        return false;

    const char *str = xkb_atom_text(ctx, field);
    for (const LookupEntry *entry = priv; entry && entry->name; entry++) {
        if (istreq(str, entry->name)) {
            *val_rtrn = entry->हालागा;
            return true;
        }
    }

    return false;
}

struct named_integer_pattern {
    const char *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    const LookupEntry *entries;
    const LookupEntry *pending_entries;
    bool is_mask;
    enum xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
static bool
NamedIntegerPatternLookup(struct xkb_context *ctx, const void *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    if (!priv || field == XKB_ATOM_NONE)
        return false;

    const char * const str = xkb_atom_text(ctx, field);
    const struct named_integer_pattern * const pattern = priv;

    const int बिलांग = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    if (बिलांग > 0 && *(str + pattern->prefix_length + बिलांग) == '\0') {
        if (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            return false;
        }
        if (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            assert(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        return true;
    } else {
        if (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, NULL)) {
            return true;
        }
        if (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, NULL)) {
            *pending_rtrn = true;
            return true;
        }
        return false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
typedef struct {
    const struct xkb_mod_set *mods;
    enum mod_type mod_type;
} LookupModMaskPriv;

static bool
LookupModMask(struct xkb_context *ctx, const void *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    const char *str = xkb_atom_text(ctx, field);
    if (!str)
        return false;

    if (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        return true;
    }

    if (istreq(str, "none")) {
        *val_rtrn = 0;
        return true;
    }

    const LookupModMaskPriv *arg = priv;
    const struct xkb_mod_set *mods = arg->mods;
    const enum mod_type mod_type = arg->mod_type;
    const xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    if (ndx == XKB_MOD_INVALID)
        return false;

    *val_rtrn = (UINT32_C(1) << ndx);
    return true;
}

bool
पाहायाग_Resolve_Boolean(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                   bool *set_rtrn)
{
    bool ok = false;
    const char *ident;

    switch (पाहायाग->common.type) {
    case STMT_पाहायाग_BOOLEAN_LITERAL:
        *set_rtrn = पाहायाग->boolean.set;
        return true;

    case STMT_पाहायाग_STRING_LITERAL:
    case STMT_पाहायाग_INTEGER_LITERAL:
    case STMT_पाहायाग_FLOAT_LITERAL:
    case STMT_पाहायाग_KEYNAME_LITERAL:
    case STMT_पाहायाग_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(पाहायाग->common.type));
        return false;

    case STMT_पाहायाग_IDENT:
        ident = xkb_atom_text(ctx, पाहायाग->ident.ident);
        if (ident) {
            if (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                return true;
            }
            else if (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                return true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        return false;

    case STMT_पाहायाग_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, पाहायाग->field_ref.element),
                xkb_atom_text(ctx, पाहायाग->field_ref.field));
        return false;

    case STMT_पाहायाग_INVERT:
    case STMT_पाहायाग_NOT:
        ok = पाहायाग_Resolve_Boolean(ctx, पाहायाग->unary.child, set_rtrn);
        if (ok)
            *set_rtrn = !*set_rtrn;
        return ok;
    case STMT_पाहायाग_ADD:
    case STMT_पाहायाग_SUBTRACT:
    case STMT_पाहायाग_MULTIPLY:
    case STMT_पाहायाग_DIVIDE:
    case STMT_पाहायाग_ASSIGN:
    case STMT_पाहायाग_NEGATE:
    case STMT_पाहायाग_UNARY_PLUS:
    case STMT_पाहायाग_EMPTY_LIST:
    case STMT_पाहायाग_ACTION_DECL:
    case STMT_पाहायाग_ACTION_LIST:
    case STMT_पाहायाग_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(पाहायाग->common.type));
        break;

    default:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 पाहायाग->common.type);
        break;
    }

    return false;
}

static bool
पाहायाग_Resolve_Integer_Lookup(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, const void *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    पाहायाग_Def *कालिवा, *कानान;

    switch (पाहायाग->common.type) {
    case STMT_पाहायाग_INTEGER_LITERAL:
        *val_rtrn = पाहायाग->integer.ival;
        return true;

    case STMT_पाहायाग_STRING_LITERAL:
    case STMT_पाहायाग_FLOAT_LITERAL:
    case STMT_पाहायाग_BOOLEAN_LITERAL:
    case STMT_पाहायाग_KEYNAME_LITERAL:
    case STMT_पाहायाग_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(पाहायाग->common.type));
        return false;

    case STMT_पाहायाग_IDENT:
        if (lookup)
            ok = lookup(ctx, lookupPriv, पाहायाग->ident.ident, &u, pending);

        if (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, पाहायाग->ident.ident));
        else
            *val_rtrn = (int64_t) u;

        if (pending && *pending) {
            /* Postpone computation */
            return false;
        }

        return ok;

    case STMT_पाहायाग_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, पाहायाग->field_ref.element),
                xkb_atom_text(ctx, पाहायाग->field_ref.field));
        return false;

    case STMT_पाहायाग_ADD:
    case STMT_पाहायाग_SUBTRACT:
    case STMT_पाहायाग_MULTIPLY:
    case STMT_पाहायाग_DIVIDE:
        कालिवा = पाहायाग->दालावाहान.कालिवा;
        कानान = पाहायाग->दालावाहान.कानान;
        if (!पाहायाग_Resolve_Integer_Lookup(ctx, कालिवा, &l, pending, lookup, lookupPriv) ||
            !पाहायाग_Resolve_Integer_Lookup(ctx, कानान, &r, pending, lookup, lookupPriv))
            return false;

        switch (पाहायाग->common.type) {
        case STMT_पाहायाग_ADD:
            if (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                return false;
            }
            break;
        case STMT_पाहायाग_SUBTRACT:
            if (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                return false;
            }
            break;
        case STMT_पाहायाग_MULTIPLY:
            if (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                return false;
            }
            break;
        case STMT_पाहायाग_DIVIDE:
            if (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                return false;
            }
            *val_rtrn = l / r;
            break;
        default:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(पाहायाग->common.type));
            return false;
        }

        return true;

    case STMT_पाहायाग_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        break;

    case STMT_पाहायाग_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        return false;

    case STMT_पाहायाग_INVERT:
    case STMT_पाहायाग_NEGATE:
        कालिवा = पाहायाग->unary.child;
        if (!पाहायाग_Resolve_Integer_Lookup(ctx, कालिवा, &l, pending, lookup, lookupPriv))
            return false;

        *val_rtrn = (पाहायाग->common.type == STMT_पाहायाग_NEGATE ? -l : ~l);
        return true;

    case STMT_पाहायाग_UNARY_PLUS:
        कालिवा = पाहायाग->unary.child;
        return पाहायाग_Resolve_Integer_Lookup(ctx, कालिवा, val_rtrn, pending, lookup,
                                        lookupPriv);

    default:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 पाहायाग->common.type);
        break;
    }

    return false;
}

bool
पाहायाग_Resolve_Integer(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                   int64_t *val_rtrn)
{
    return पाहायाग_Resolve_Integer_Lookup(ctx, पाहायाग, val_rtrn, NULL, NULL, NULL);
}

enum xkb_parser_error
पाहायाग_Resolve_Group(const struct xkb_keymap_info *keymap_info,
                 const पाहायाग_Def *पाहायाग, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    static const LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { NULL, 0 }
    };

    const struct named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = sizeof("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    if (!पाहायाग_Resolve_Integer_Lookup(keymap_info->keymap.ctx, पाहायाग, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    if (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    return PARSER_SUCCESS;
}

bool
पाहायाग_Resolve_Level(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                 xkb_level_index_t *level_rtrn)
{
    static const struct named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = sizeof("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = NULL,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    if (!पाहायाग_Resolve_Integer_Lookup(ctx, पाहायाग, &result, NULL,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        return false;

    if (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        return false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    return true;
}

bool
पाहायाग_Resolve_Button(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग, int64_t *btn_rtrn)
{
    return पाहायाग_Resolve_Integer_Lookup(ctx, पाहायाग, btn_rtrn, NULL, SimpleLookup,
                                    buttonNames);
}

bool
पाहायाग_Resolve_String(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                  xkb_atom_t *val_rtrn)
{
    switch (पाहायाग->common.type) {
    case STMT_पाहायाग_STRING_LITERAL:
        *val_rtrn = पाहायाग->string.str;
        return true;

    case STMT_पाहायाग_INTEGER_LITERAL:
    case STMT_पाहायाग_FLOAT_LITERAL:
    case STMT_पाहायाग_BOOLEAN_LITERAL:
    case STMT_पाहायाग_KEYNAME_LITERAL:
    case STMT_पाहायाग_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(पाहायाग->common.type));
        return false;

    case STMT_पाहायाग_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, पाहायाग->ident.ident));
        return false;

    case STMT_पाहायाग_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, पाहायाग->field_ref.element),
                xkb_atom_text(ctx, पाहायाग->field_ref.field));
        return false;

    case STMT_पाहायाग_ADD:
    case STMT_पाहायाग_SUBTRACT:
    case STMT_पाहायाग_MULTIPLY:
    case STMT_पाहायाग_DIVIDE:
    case STMT_पाहायाग_ASSIGN:
    case STMT_पाहायाग_NEGATE:
    case STMT_पाहायाग_INVERT:
    case STMT_पाहायाग_NOT:
    case STMT_पाहायाग_UNARY_PLUS:
    case STMT_पाहायाग_EMPTY_LIST:
    case STMT_पाहायाग_ACTION_DECL:
    case STMT_पाहायाग_ACTION_LIST:
    case STMT_पाहायाग_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(पाहायाग->common.type));
        return false;

    default:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 पाहायाग->common.type);
        break;
    }
    return false;
}

bool
पाहायाग_Resolve_Enum(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                uint32_t *val_rtrn, const LookupEntry *मगा_हालागा)
{
    if (पाहायाग->common.type != STMT_पाहायाग_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(पाहायाग->common.type));
        return false;
    }

    if (!SimpleLookup(ctx, मगा_हालागा, पाहायाग->ident.ident, val_rtrn, NULL)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, पाहायाग->ident.ident));
        while (मगा_हालागा && मगा_हालागा->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", मगा_हालागा->name);
            मगा_हालागा++;
        }
        return false;
    }

    return true;
}

static bool
पाहायाग_Resolve_Mask_Lookup(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, const void *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    पाहायाग_Def *कालिवा, *कानान;
    const char *bogus = NULL;

    switch (पाहायाग->common.type) {
    case STMT_पाहायाग_INTEGER_LITERAL:
        if (पाहायाग->integer.ival < 0 || पाहायाग->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    पाहायाग->integer.ival < 0 ? "-" : "",
                    imaxabs(पाहायाग->integer.ival),
                    UINT32_MAX);
            return false;
        }
        *val_rtrn = (uint32_t) पाहायाग->integer.ival;
        return true;

    case STMT_पाहायाग_STRING_LITERAL:
    case STMT_पाहायाग_FLOAT_LITERAL:
    case STMT_पाहायाग_BOOLEAN_LITERAL:
    case STMT_पाहायाग_KEYNAME_LITERAL:
    case STMT_पाहायाग_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(पाहायाग->common.type));
        return false;

    case STMT_पाहायाग_IDENT:
        ok = lookup(ctx, lookupPriv, पाहायाग->ident.ident, val_rtrn, pending);
        if (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, पाहायाग->ident.ident));
        if (pending && *pending) {
            /* Postpone computation */
            return false;
        }
        return ok;

    case STMT_पाहायाग_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, पाहायाग->field_ref.element),
                xkb_atom_text(ctx, पाहायाग->field_ref.field));
        return false;

    case STMT_पाहायाग_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    case STMT_पाहायाग_ACTION_DECL:
        if (bogus == NULL)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        return false;

    case STMT_पाहायाग_ADD:
    case STMT_पाहायाग_SUBTRACT:
    case STMT_पाहायाग_MULTIPLY:
    case STMT_पाहायाग_DIVIDE:
        कालिवा = पाहायाग->दालावाहान.कालिवा;
        कानान = पाहायाग->दालावाहान.कानान;
        if (!पाहायाग_Resolve_Mask_Lookup(ctx, कालिवा, &l, pending, lookup, lookupPriv) ||
            !पाहायाग_Resolve_Mask_Lookup(ctx, कानान, &r, pending, lookup, lookupPriv))
            return false;

        switch (पाहायाग->common.type) {
        case STMT_पाहायाग_ADD:
            *val_rtrn = l | r;
            break;
        case STMT_पाहायाग_SUBTRACT:
            *val_rtrn = l & (~r);
            break;
        case STMT_पाहायाग_MULTIPLY:
        case STMT_पाहायाग_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (पाहायाग->common.type == STMT_पाहायाग_DIVIDE ? "divide" : "multiply"));
            return false;
        default:
            break;
        }

        return true;

    case STMT_पाहायाग_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        break;

    case STMT_पाहायाग_INVERT:
        कालिवा = पाहायाग->unary.child;
        if (!पाहायाग_Resolve_Integer_Lookup(ctx, कालिवा, &v, pending, lookup, lookupPriv))
            return false;
        if (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            return false;
        }
        *val_rtrn = ~(uint32_t) v;
        return true;

    case STMT_पाहायाग_UNARY_PLUS:
    case STMT_पाहायाग_NEGATE:
    case STMT_पाहायाग_NOT:
        कालिवा = पाहायाग->unary.child;
        if (!पाहायाग_Resolve_Integer_Lookup(ctx, कालिवा, &v, pending, lookup, lookupPriv))
            return false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(पाहायाग->common.type));
        return false;

    default:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 पाहायाग->common.type);
        break;
    }

    return false;
}

bool
पाहायाग_Resolve_Mask(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                uint32_t *mask_rtrn, const LookupEntry *मगा_हालागा)
{
    return पाहायाग_Resolve_Mask_Lookup(ctx, पाहायाग, mask_rtrn, NULL, SimpleLookup, मगा_हालागा);
}

bool
पाहायाग_Resolve_Mod_Mask(struct xkb_context *ctx, const पाहायाग_Def *पाहायाग,
                   enum mod_type mod_type, const struct xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    return पाहायाग_Resolve_Mask_Lookup(ctx, पाहायाग, mask_rtrn, NULL, LookupModMask, &priv);
}

bool
पाहायाग_Resolve_Mod(struct xkb_context *ctx, const पाहायाग_Def *def,
               enum mod_type mod_type, const struct xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    if (def->common.type != STMT_पाहायाग_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        return false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    if (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        return false;
    }

    *ndx_rtrn = ndx;
    return true;
}

bool
पाहायाग_Resolve_Group_Mask(const struct xkb_keymap_info *keymap_info,
                     const पाहायाग_Def *पाहायाग, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    static const LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { NULL, 0 }
    };

    const struct named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = sizeof("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    return पाहायाग_Resolve_Mask_Lookup(keymap_info->keymap.ctx, पाहायाग, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
