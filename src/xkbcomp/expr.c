/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdint.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "expr.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "keysym.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "text.h"
#અન્તર્ભાવ "xkbcomp/ast.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "util-numbers.h"
#અન્તર્ભાવ "util-checked-arithmetic.h"

પ્રકારવ્યાખ્યા bool (*IdentLookupFunc)(સંરચના xkb_context *ctx, અચલ શૂન્ય *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
પદાવલિ_Resolve_Lhs(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
               અચલ અક્ષર **elem_rtrn, અચલ અક્ષર **field_rtrn,
               પદાવલિ_Def **index_rtrn)
{
    પસંદગી (પદાવલિ->common.type) {
    સ્થિતિ STMT_પદાવલિ_IDENT:
        *elem_rtrn = ખાલી;
        *field_rtrn = xkb_atom_text(ctx, પદાવલિ->ident.ident);
        *index_rtrn = ખાલી;
        પાછા (*field_rtrn != ખાલી);
    સ્થિતિ STMT_પદાવલિ_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, પદાવલિ->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, પદાવલિ->field_ref.field);
        *index_rtrn = ખાલી;
        પાછા (*elem_rtrn != ખાલી && *field_rtrn != ખાલી);
    સ્થિતિ STMT_પદાવલિ_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, પદાવલિ->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, પદાવલિ->array_ref.field);
        *index_rtrn = પદાવલિ->array_ref.entry;
        જો (પદાવલિ->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == ખાલી)
            પાછા false;
        જો (*field_rtrn == ખાલી)
            પાછા false;
        પાછા true;
    મૂળભૂત:
        તોડો;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", પદાવલિ->common.type);
    પાછા false;
}

સ્થિર bool
SimpleLookup(સંરચના xkb_context *ctx, અચલ શૂન્ય *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    જો (!priv || field == XKB_ATOM_NONE)
        પાછા false;

    અચલ અક્ષર *str = xkb_atom_text(ctx, field);
    માટે (અચલ LookupEntry *entry = priv; entry && entry->name; entry++) {
        જો (istreq(str, entry->name)) {
            *val_rtrn = entry->કિંમત;
            પાછા true;
        }
    }

    પાછા false;
}

સંરચના named_integer_pattern {
    અચલ અક્ષર *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    અચલ LookupEntry *entries;
    અચલ LookupEntry *pending_entries;
    bool is_mask;
    ગણના xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
સ્થિર bool
NamedIntegerPatternLookup(સંરચના xkb_context *ctx, અચલ શૂન્ય *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    જો (!priv || field == XKB_ATOM_NONE)
        પાછા false;

    અચલ અક્ષર * અચલ str = xkb_atom_text(ctx, field);
    અચલ સંરચના named_integer_pattern * અચલ pattern = priv;

    અચલ પૂર્ણાંક ગણતરી = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    જો (ગણતરી > 0 && *(str + pattern->prefix_length + ગણતરી) == '\0') {
        જો (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            પાછા false;
        }
        જો (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            પ્રતિજ્ઞા(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        પાછા true;
    } નહીંતર {
        જો (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, ખાલી)) {
            પાછા true;
        }
        જો (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, ખાલી)) {
            *pending_rtrn = true;
            પાછા true;
        }
        પાછા false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
પ્રકારવ્યાખ્યા સંરચના {
    અચલ સંરચના xkb_mod_set *mods;
    ગણના mod_type mod_type;
} LookupModMaskPriv;

સ્થિર bool
LookupModMask(સંરચના xkb_context *ctx, અચલ શૂન્ય *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    અચલ અક્ષર *str = xkb_atom_text(ctx, field);
    જો (!str)
        પાછા false;

    જો (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        પાછા true;
    }

    જો (istreq(str, "none")) {
        *val_rtrn = 0;
        પાછા true;
    }

    અચલ LookupModMaskPriv *arg = priv;
    અચલ સંરચના xkb_mod_set *mods = arg->mods;
    અચલ ગણના mod_type mod_type = arg->mod_type;
    અચલ xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    જો (ndx == XKB_MOD_INVALID)
        પાછા false;

    *val_rtrn = (UINT32_C(1) << ndx);
    પાછા true;
}

bool
પદાવલિ_Resolve_Boolean(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                   bool *set_rtrn)
{
    bool ok = false;
    અચલ અક્ષર *ident;

    પસંદગી (પદાવલિ->common.type) {
    સ્થિતિ STMT_પદાવલિ_BOOLEAN_LITERAL:
        *set_rtrn = પદાવલિ->boolean.set;
        પાછા true;

    સ્થિતિ STMT_પદાવલિ_STRING_LITERAL:
    સ્થિતિ STMT_પદાવલિ_INTEGER_LITERAL:
    સ્થિતિ STMT_પદાવલિ_FLOAT_LITERAL:
    સ્થિતિ STMT_પદાવલિ_KEYNAME_LITERAL:
    સ્થિતિ STMT_પદાવલિ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(પદાવલિ->common.type));
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_IDENT:
        ident = xkb_atom_text(ctx, પદાવલિ->ident.ident);
        જો (ident) {
            જો (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                પાછા true;
            }
            નહીંતર જો (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                પાછા true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, પદાવલિ->field_ref.element),
                xkb_atom_text(ctx, પદાવલિ->field_ref.field));
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_INVERT:
    સ્થિતિ STMT_પદાવલિ_NOT:
        ok = પદાવલિ_Resolve_Boolean(ctx, પદાવલિ->unary.child, set_rtrn);
        જો (ok)
            *set_rtrn = !*set_rtrn;
        પાછા ok;
    સ્થિતિ STMT_પદાવલિ_ADD:
    સ્થિતિ STMT_પદાવલિ_SUBTRACT:
    સ્થિતિ STMT_પદાવલિ_MULTIPLY:
    સ્થિતિ STMT_પદાવલિ_DIVIDE:
    સ્થિતિ STMT_પદાવલિ_ASSIGN:
    સ્થિતિ STMT_પદાવલિ_NEGATE:
    સ્થિતિ STMT_પદાવલિ_UNARY_PLUS:
    સ્થિતિ STMT_પદાવલિ_EMPTY_LIST:
    સ્થિતિ STMT_પદાવલિ_ACTION_DECL:
    સ્થિતિ STMT_પદાવલિ_ACTION_LIST:
    સ્થિતિ STMT_પદાવલિ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(પદાવલિ->common.type));
        તોડો;

    મૂળભૂત:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 પદાવલિ->common.type);
        તોડો;
    }

    પાછા false;
}

સ્થિર bool
પદાવલિ_Resolve_Integer_Lookup(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, અચલ શૂન્ય *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    પદાવલિ_Def *ડાબું, *જમણું;

    પસંદગી (પદાવલિ->common.type) {
    સ્થિતિ STMT_પદાવલિ_INTEGER_LITERAL:
        *val_rtrn = પદાવલિ->integer.ival;
        પાછા true;

    સ્થિતિ STMT_પદાવલિ_STRING_LITERAL:
    સ્થિતિ STMT_પદાવલિ_FLOAT_LITERAL:
    સ્થિતિ STMT_પદાવલિ_BOOLEAN_LITERAL:
    સ્થિતિ STMT_પદાવલિ_KEYNAME_LITERAL:
    સ્થિતિ STMT_પદાવલિ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(પદાવલિ->common.type));
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_IDENT:
        જો (lookup)
            ok = lookup(ctx, lookupPriv, પદાવલિ->ident.ident, &u, pending);

        જો (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, પદાવલિ->ident.ident));
        નહીંતર
            *val_rtrn = (int64_t) u;

        જો (pending && *pending) {
            /* Postpone computation */
            પાછા false;
        }

        પાછા ok;

    સ્થિતિ STMT_પદાવલિ_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, પદાવલિ->field_ref.element),
                xkb_atom_text(ctx, પદાવલિ->field_ref.field));
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_ADD:
    સ્થિતિ STMT_પદાવલિ_SUBTRACT:
    સ્થિતિ STMT_પદાવલિ_MULTIPLY:
    સ્થિતિ STMT_પદાવલિ_DIVIDE:
        ડાબું = પદાવલિ->દ્વિઅંકી.ડાબું;
        જમણું = પદાવલિ->દ્વિઅંકી.જમણું;
        જો (!પદાવલિ_Resolve_Integer_Lookup(ctx, ડાબું, &l, pending, lookup, lookupPriv) ||
            !પદાવલિ_Resolve_Integer_Lookup(ctx, જમણું, &r, pending, lookup, lookupPriv))
            પાછા false;

        પસંદગી (પદાવલિ->common.type) {
        સ્થિતિ STMT_પદાવલિ_ADD:
            જો (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                પાછા false;
            }
            તોડો;
        સ્થિતિ STMT_પદાવલિ_SUBTRACT:
            જો (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                પાછા false;
            }
            તોડો;
        સ્થિતિ STMT_પદાવલિ_MULTIPLY:
            જો (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                પાછા false;
            }
            તોડો;
        સ્થિતિ STMT_પદાવલિ_DIVIDE:
            જો (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                પાછા false;
            }
            *val_rtrn = l / r;
            તોડો;
        મૂળભૂત:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(પદાવલિ->common.type));
            પાછા false;
        }

        પાછા true;

    સ્થિતિ STMT_પદાવલિ_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        તોડો;

    સ્થિતિ STMT_પદાવલિ_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_INVERT:
    સ્થિતિ STMT_પદાવલિ_NEGATE:
        ડાબું = પદાવલિ->unary.child;
        જો (!પદાવલિ_Resolve_Integer_Lookup(ctx, ડાબું, &l, pending, lookup, lookupPriv))
            પાછા false;

        *val_rtrn = (પદાવલિ->common.type == STMT_પદાવલિ_NEGATE ? -l : ~l);
        પાછા true;

    સ્થિતિ STMT_પદાવલિ_UNARY_PLUS:
        ડાબું = પદાવલિ->unary.child;
        પાછા પદાવલિ_Resolve_Integer_Lookup(ctx, ડાબું, val_rtrn, pending, lookup,
                                        lookupPriv);

    મૂળભૂત:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 પદાવલિ->common.type);
        તોડો;
    }

    પાછા false;
}

bool
પદાવલિ_Resolve_Integer(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                   int64_t *val_rtrn)
{
    પાછા પદાવલિ_Resolve_Integer_Lookup(ctx, પદાવલિ, val_rtrn, ખાલી, ખાલી, ખાલી);
}

ગણના xkb_parser_error
પદાવલિ_Resolve_Group(અચલ સંરચના xkb_keymap_info *keymap_info,
                 અચલ પદાવલિ_Def *પદાવલિ, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    સ્થિર અચલ LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { ખાલી, 0 }
    };

    અચલ સંરચના named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = કદ("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    જો (!પદાવલિ_Resolve_Integer_Lookup(keymap_info->keymap.ctx, પદાવલિ, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    જો (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    પાછા PARSER_SUCCESS;
}

bool
પદાવલિ_Resolve_Level(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                 xkb_level_index_t *level_rtrn)
{
    સ્થિર અચલ સંરચના named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = કદ("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = ખાલી,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    જો (!પદાવલિ_Resolve_Integer_Lookup(ctx, પદાવલિ, &result, ખાલી,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        પાછા false;

    જો (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        પાછા false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    પાછા true;
}

bool
પદાવલિ_Resolve_Button(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ, int64_t *btn_rtrn)
{
    પાછા પદાવલિ_Resolve_Integer_Lookup(ctx, પદાવલિ, btn_rtrn, ખાલી, SimpleLookup,
                                    buttonNames);
}

bool
પદાવલિ_Resolve_String(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                  xkb_atom_t *val_rtrn)
{
    પસંદગી (પદાવલિ->common.type) {
    સ્થિતિ STMT_પદાવલિ_STRING_LITERAL:
        *val_rtrn = પદાવલિ->string.str;
        પાછા true;

    સ્થિતિ STMT_પદાવલિ_INTEGER_LITERAL:
    સ્થિતિ STMT_પદાવલિ_FLOAT_LITERAL:
    સ્થિતિ STMT_પદાવલિ_BOOLEAN_LITERAL:
    સ્થિતિ STMT_પદાવલિ_KEYNAME_LITERAL:
    સ્થિતિ STMT_પદાવલિ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(પદાવલિ->common.type));
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, પદાવલિ->ident.ident));
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, પદાવલિ->field_ref.element),
                xkb_atom_text(ctx, પદાવલિ->field_ref.field));
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_ADD:
    સ્થિતિ STMT_પદાવલિ_SUBTRACT:
    સ્થિતિ STMT_પદાવલિ_MULTIPLY:
    સ્થિતિ STMT_પદાવલિ_DIVIDE:
    સ્થિતિ STMT_પદાવલિ_ASSIGN:
    સ્થિતિ STMT_પદાવલિ_NEGATE:
    સ્થિતિ STMT_પદાવલિ_INVERT:
    સ્થિતિ STMT_પદાવલિ_NOT:
    સ્થિતિ STMT_પદાવલિ_UNARY_PLUS:
    સ્થિતિ STMT_પદાવલિ_EMPTY_LIST:
    સ્થિતિ STMT_પદાવલિ_ACTION_DECL:
    સ્થિતિ STMT_પદાવલિ_ACTION_LIST:
    સ્થિતિ STMT_પદાવલિ_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(પદાવલિ->common.type));
        પાછા false;

    મૂળભૂત:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 પદાવલિ->common.type);
        તોડો;
    }
    પાછા false;
}

bool
પદાવલિ_Resolve_Enum(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                uint32_t *val_rtrn, અચલ LookupEntry *કિંમતો)
{
    જો (પદાવલિ->common.type != STMT_પદાવલિ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(પદાવલિ->common.type));
        પાછા false;
    }

    જો (!SimpleLookup(ctx, કિંમતો, પદાવલિ->ident.ident, val_rtrn, ખાલી)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, પદાવલિ->ident.ident));
        જ્યારે (કિંમતો && કિંમતો->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", કિંમતો->name);
            કિંમતો++;
        }
        પાછા false;
    }

    પાછા true;
}

સ્થિર bool
પદાવલિ_Resolve_Mask_Lookup(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, અચલ શૂન્ય *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    પદાવલિ_Def *ડાબું, *જમણું;
    અચલ અક્ષર *bogus = ખાલી;

    પસંદગી (પદાવલિ->common.type) {
    સ્થિતિ STMT_પદાવલિ_INTEGER_LITERAL:
        જો (પદાવલિ->integer.ival < 0 || પદાવલિ->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    પદાવલિ->integer.ival < 0 ? "-" : "",
                    imaxabs(પદાવલિ->integer.ival),
                    UINT32_MAX);
            પાછા false;
        }
        *val_rtrn = (uint32_t) પદાવલિ->integer.ival;
        પાછા true;

    સ્થિતિ STMT_પદાવલિ_STRING_LITERAL:
    સ્થિતિ STMT_પદાવલિ_FLOAT_LITERAL:
    સ્થિતિ STMT_પદાવલિ_BOOLEAN_LITERAL:
    સ્થિતિ STMT_પદાવલિ_KEYNAME_LITERAL:
    સ્થિતિ STMT_પદાવલિ_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(પદાવલિ->common.type));
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_IDENT:
        ok = lookup(ctx, lookupPriv, પદાવલિ->ident.ident, val_rtrn, pending);
        જો (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, પદાવલિ->ident.ident));
        જો (pending && *pending) {
            /* Postpone computation */
            પાછા false;
        }
        પાછા ok;

    સ્થિતિ STMT_પદાવલિ_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, પદાવલિ->field_ref.element),
                xkb_atom_text(ctx, પદાવલિ->field_ref.field));
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    સ્થિતિ STMT_પદાવલિ_ACTION_DECL:
        જો (bogus == ખાલી)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        પાછા false;

    સ્થિતિ STMT_પદાવલિ_ADD:
    સ્થિતિ STMT_પદાવલિ_SUBTRACT:
    સ્થિતિ STMT_પદાવલિ_MULTIPLY:
    સ્થિતિ STMT_પદાવલિ_DIVIDE:
        ડાબું = પદાવલિ->દ્વિઅંકી.ડાબું;
        જમણું = પદાવલિ->દ્વિઅંકી.જમણું;
        જો (!પદાવલિ_Resolve_Mask_Lookup(ctx, ડાબું, &l, pending, lookup, lookupPriv) ||
            !પદાવલિ_Resolve_Mask_Lookup(ctx, જમણું, &r, pending, lookup, lookupPriv))
            પાછા false;

        પસંદગી (પદાવલિ->common.type) {
        સ્થિતિ STMT_પદાવલિ_ADD:
            *val_rtrn = l | r;
            તોડો;
        સ્થિતિ STMT_પદાવલિ_SUBTRACT:
            *val_rtrn = l & (~r);
            તોડો;
        સ્થિતિ STMT_પદાવલિ_MULTIPLY:
        સ્થિતિ STMT_પદાવલિ_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (પદાવલિ->common.type == STMT_પદાવલિ_DIVIDE ? "divide" : "multiply"));
            પાછા false;
        મૂળભૂત:
            તોડો;
        }

        પાછા true;

    સ્થિતિ STMT_પદાવલિ_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        તોડો;

    સ્થિતિ STMT_પદાવલિ_INVERT:
        ડાબું = પદાવલિ->unary.child;
        જો (!પદાવલિ_Resolve_Integer_Lookup(ctx, ડાબું, &v, pending, lookup, lookupPriv))
            પાછા false;
        જો (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            પાછા false;
        }
        *val_rtrn = ~(uint32_t) v;
        પાછા true;

    સ્થિતિ STMT_પદાવલિ_UNARY_PLUS:
    સ્થિતિ STMT_પદાવલિ_NEGATE:
    સ્થિતિ STMT_પદાવલિ_NOT:
        ડાબું = પદાવલિ->unary.child;
        જો (!પદાવલિ_Resolve_Integer_Lookup(ctx, ડાબું, &v, pending, lookup, lookupPriv))
            પાછા false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(પદાવલિ->common.type));
        પાછા false;

    મૂળભૂત:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 પદાવલિ->common.type);
        તોડો;
    }

    પાછા false;
}

bool
પદાવલિ_Resolve_Mask(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                uint32_t *mask_rtrn, અચલ LookupEntry *કિંમતો)
{
    પાછા પદાવલિ_Resolve_Mask_Lookup(ctx, પદાવલિ, mask_rtrn, ખાલી, SimpleLookup, કિંમતો);
}

bool
પદાવલિ_Resolve_Mod_Mask(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *પદાવલિ,
                   ગણના mod_type mod_type, અચલ સંરચના xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    પાછા પદાવલિ_Resolve_Mask_Lookup(ctx, પદાવલિ, mask_rtrn, ખાલી, LookupModMask, &priv);
}

bool
પદાવલિ_Resolve_Mod(સંરચના xkb_context *ctx, અચલ પદાવલિ_Def *def,
               ગણના mod_type mod_type, અચલ સંરચના xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    જો (def->common.type != STMT_પદાવલિ_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        પાછા false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    જો (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        પાછા false;
    }

    *ndx_rtrn = ndx;
    પાછા true;
}

bool
પદાવલિ_Resolve_Group_Mask(અચલ સંરચના xkb_keymap_info *keymap_info,
                     અચલ પદાવલિ_Def *પદાવલિ, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    સ્થિર અચલ LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { ખાલી, 0 }
    };

    અચલ સંરચના named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = કદ("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    પાછા પદાવલિ_Resolve_Mask_Lookup(keymap_info->keymap.ctx, પદાવલિ, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
