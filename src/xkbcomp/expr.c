/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdint.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "expr.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "keysym.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "xkbcomp/ast.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "util-numbers.h"
#അന്തര്ഭാവ "util-checked-arithmetic.h"

തരനിർവചനം bool (*IdentLookupFunc)(ഘടന xkb_context *ctx, സ്ഥിരം ശൂന്യം *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
വ്യഞ്ജകം_Resolve_Lhs(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
               സ്ഥിരം അക്ഷരം **elem_rtrn, സ്ഥിരം അക്ഷരം **field_rtrn,
               വ്യഞ്ജകം_Def **index_rtrn)
{
    തിരഞ്ഞെടുപ്പ് (വ്യഞ്ജകം->common.type) {
    അവസ്ഥ STMT_വ്യഞ്ജകം_IDENT:
        *elem_rtrn = ശൂന്യ;
        *field_rtrn = xkb_atom_text(ctx, വ്യഞ്ജകം->ident.ident);
        *index_rtrn = ശൂന്യ;
        തിരികെ (*field_rtrn != ശൂന്യ);
    അവസ്ഥ STMT_വ്യഞ്ജകം_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, വ്യഞ്ജകം->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, വ്യഞ്ജകം->field_ref.field);
        *index_rtrn = ശൂന്യ;
        തിരികെ (*elem_rtrn != ശൂന്യ && *field_rtrn != ശൂന്യ);
    അവസ്ഥ STMT_വ്യഞ്ജകം_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, വ്യഞ്ജകം->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, വ്യഞ്ജകം->array_ref.field);
        *index_rtrn = വ്യഞ്ജകം->array_ref.entry;
        എങ്കിൽ (വ്യഞ്ജകം->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == ശൂന്യ)
            തിരികെ false;
        എങ്കിൽ (*field_rtrn == ശൂന്യ)
            തിരികെ false;
        തിരികെ true;
    സ്ഥിരസ്ഥിതി:
        നിർത്തുക;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", വ്യഞ്ജകം->common.type);
    തിരികെ false;
}

സ്ഥിതം bool
SimpleLookup(ഘടന xkb_context *ctx, സ്ഥിരം ശൂന്യം *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    എങ്കിൽ (!priv || field == XKB_ATOM_NONE)
        തിരികെ false;

    സ്ഥിരം അക്ഷരം *str = xkb_atom_text(ctx, field);
    വേണ്ടി (സ്ഥിരം LookupEntry *entry = priv; entry && entry->name; entry++) {
        എങ്കിൽ (istreq(str, entry->name)) {
            *val_rtrn = entry->മൂല്യം;
            തിരികെ true;
        }
    }

    തിരികെ false;
}

ഘടന named_integer_pattern {
    സ്ഥിരം അക്ഷരം *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    സ്ഥിരം LookupEntry *entries;
    സ്ഥിരം LookupEntry *pending_entries;
    bool is_mask;
    എണ്ണൽ xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
സ്ഥിതം bool
NamedIntegerPatternLookup(ഘടന xkb_context *ctx, സ്ഥിരം ശൂന്യം *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    എങ്കിൽ (!priv || field == XKB_ATOM_NONE)
        തിരികെ false;

    സ്ഥിരം അക്ഷരം * സ്ഥിരം str = xkb_atom_text(ctx, field);
    സ്ഥിരം ഘടന named_integer_pattern * സ്ഥിരം pattern = priv;

    സ്ഥിരം പൂർണ്ണസംഖ്യ എണ്ണം = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    എങ്കിൽ (എണ്ണം > 0 && *(str + pattern->prefix_length + എണ്ണം) == '\0') {
        എങ്കിൽ (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            തിരികെ false;
        }
        എങ്കിൽ (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            പ്രതിജ്ഞാ(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        തിരികെ true;
    } അല്ലെങ്കിൽ {
        എങ്കിൽ (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, ശൂന്യ)) {
            തിരികെ true;
        }
        എങ്കിൽ (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, ശൂന്യ)) {
            *pending_rtrn = true;
            തിരികെ true;
        }
        തിരികെ false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
തരനിർവചനം ഘടന {
    സ്ഥിരം ഘടന xkb_mod_set *mods;
    എണ്ണൽ mod_type mod_type;
} LookupModMaskPriv;

സ്ഥിതം bool
LookupModMask(ഘടന xkb_context *ctx, സ്ഥിരം ശൂന്യം *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    സ്ഥിരം അക്ഷരം *str = xkb_atom_text(ctx, field);
    എങ്കിൽ (!str)
        തിരികെ false;

    എങ്കിൽ (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        തിരികെ true;
    }

    എങ്കിൽ (istreq(str, "none")) {
        *val_rtrn = 0;
        തിരികെ true;
    }

    സ്ഥിരം LookupModMaskPriv *arg = priv;
    സ്ഥിരം ഘടന xkb_mod_set *mods = arg->mods;
    സ്ഥിരം എണ്ണൽ mod_type mod_type = arg->mod_type;
    സ്ഥിരം xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    എങ്കിൽ (ndx == XKB_MOD_INVALID)
        തിരികെ false;

    *val_rtrn = (UINT32_C(1) << ndx);
    തിരികെ true;
}

bool
വ്യഞ്ജകം_Resolve_Boolean(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                   bool *set_rtrn)
{
    bool ok = false;
    സ്ഥിരം അക്ഷരം *ident;

    തിരഞ്ഞെടുപ്പ് (വ്യഞ്ജകം->common.type) {
    അവസ്ഥ STMT_വ്യഞ്ജകം_BOOLEAN_LITERAL:
        *set_rtrn = വ്യഞ്ജകം->boolean.set;
        തിരികെ true;

    അവസ്ഥ STMT_വ്യഞ്ജകം_STRING_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_INTEGER_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_FLOAT_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYNAME_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(വ്യഞ്ജകം->common.type));
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_IDENT:
        ident = xkb_atom_text(ctx, വ്യഞ്ജകം->ident.ident);
        എങ്കിൽ (ident) {
            എങ്കിൽ (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                തിരികെ true;
            }
            അല്ലെങ്കിൽ എങ്കിൽ (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                തിരികെ true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, വ്യഞ്ജകം->field_ref.element),
                xkb_atom_text(ctx, വ്യഞ്ജകം->field_ref.field));
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_INVERT:
    അവസ്ഥ STMT_വ്യഞ്ജകം_NOT:
        ok = വ്യഞ്ജകം_Resolve_Boolean(ctx, വ്യഞ്ജകം->unary.child, set_rtrn);
        എങ്കിൽ (ok)
            *set_rtrn = !*set_rtrn;
        തിരികെ ok;
    അവസ്ഥ STMT_വ്യഞ്ജകം_ADD:
    അവസ്ഥ STMT_വ്യഞ്ജകം_SUBTRACT:
    അവസ്ഥ STMT_വ്യഞ്ജകം_MULTIPLY:
    അവസ്ഥ STMT_വ്യഞ്ജകം_DIVIDE:
    അവസ്ഥ STMT_വ്യഞ്ജകം_ASSIGN:
    അവസ്ഥ STMT_വ്യഞ്ജകം_NEGATE:
    അവസ്ഥ STMT_വ്യഞ്ജകം_UNARY_PLUS:
    അവസ്ഥ STMT_വ്യഞ്ജകം_EMPTY_LIST:
    അവസ്ഥ STMT_വ്യഞ്ജകം_ACTION_DECL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_ACTION_LIST:
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(വ്യഞ്ജകം->common.type));
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 വ്യഞ്ജകം->common.type);
        നിർത്തുക;
    }

    തിരികെ false;
}

സ്ഥിതം bool
വ്യഞ്ജകം_Resolve_Integer_Lookup(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, സ്ഥിരം ശൂന്യം *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    വ്യഞ്ജകം_Def *ഇടത്, *വലത്;

    തിരഞ്ഞെടുപ്പ് (വ്യഞ്ജകം->common.type) {
    അവസ്ഥ STMT_വ്യഞ്ജകം_INTEGER_LITERAL:
        *val_rtrn = വ്യഞ്ജകം->integer.ival;
        തിരികെ true;

    അവസ്ഥ STMT_വ്യഞ്ജകം_STRING_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_FLOAT_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_BOOLEAN_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYNAME_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(വ്യഞ്ജകം->common.type));
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_IDENT:
        എങ്കിൽ (lookup)
            ok = lookup(ctx, lookupPriv, വ്യഞ്ജകം->ident.ident, &u, pending);

        എങ്കിൽ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, വ്യഞ്ജകം->ident.ident));
        അല്ലെങ്കിൽ
            *val_rtrn = (int64_t) u;

        എങ്കിൽ (pending && *pending) {
            /* Postpone computation */
            തിരികെ false;
        }

        തിരികെ ok;

    അവസ്ഥ STMT_വ്യഞ്ജകം_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, വ്യഞ്ജകം->field_ref.element),
                xkb_atom_text(ctx, വ്യഞ്ജകം->field_ref.field));
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_ADD:
    അവസ്ഥ STMT_വ്യഞ്ജകം_SUBTRACT:
    അവസ്ഥ STMT_വ്യഞ്ജകം_MULTIPLY:
    അവസ്ഥ STMT_വ്യഞ്ജകം_DIVIDE:
        ഇടത് = വ്യഞ്ജകം->ദ്വിമാന.ഇടത്;
        വലത് = വ്യഞ്ജകം->ദ്വിമാന.വലത്;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer_Lookup(ctx, ഇടത്, &l, pending, lookup, lookupPriv) ||
            !വ്യഞ്ജകം_Resolve_Integer_Lookup(ctx, വലത്, &r, pending, lookup, lookupPriv))
            തിരികെ false;

        തിരഞ്ഞെടുപ്പ് (വ്യഞ്ജകം->common.type) {
        അവസ്ഥ STMT_വ്യഞ്ജകം_ADD:
            എങ്കിൽ (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                തിരികെ false;
            }
            നിർത്തുക;
        അവസ്ഥ STMT_വ്യഞ്ജകം_SUBTRACT:
            എങ്കിൽ (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                തിരികെ false;
            }
            നിർത്തുക;
        അവസ്ഥ STMT_വ്യഞ്ജകം_MULTIPLY:
            എങ്കിൽ (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                തിരികെ false;
            }
            നിർത്തുക;
        അവസ്ഥ STMT_വ്യഞ്ജകം_DIVIDE:
            എങ്കിൽ (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                തിരികെ false;
            }
            *val_rtrn = l / r;
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(വ്യഞ്ജകം->common.type));
            തിരികെ false;
        }

        തിരികെ true;

    അവസ്ഥ STMT_വ്യഞ്ജകം_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        നിർത്തുക;

    അവസ്ഥ STMT_വ്യഞ്ജകം_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_INVERT:
    അവസ്ഥ STMT_വ്യഞ്ജകം_NEGATE:
        ഇടത് = വ്യഞ്ജകം->unary.child;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer_Lookup(ctx, ഇടത്, &l, pending, lookup, lookupPriv))
            തിരികെ false;

        *val_rtrn = (വ്യഞ്ജകം->common.type == STMT_വ്യഞ്ജകം_NEGATE ? -l : ~l);
        തിരികെ true;

    അവസ്ഥ STMT_വ്യഞ്ജകം_UNARY_PLUS:
        ഇടത് = വ്യഞ്ജകം->unary.child;
        തിരികെ വ്യഞ്ജകം_Resolve_Integer_Lookup(ctx, ഇടത്, val_rtrn, pending, lookup,
                                        lookupPriv);

    സ്ഥിരസ്ഥിതി:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 വ്യഞ്ജകം->common.type);
        നിർത്തുക;
    }

    തിരികെ false;
}

bool
വ്യഞ്ജകം_Resolve_Integer(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                   int64_t *val_rtrn)
{
    തിരികെ വ്യഞ്ജകം_Resolve_Integer_Lookup(ctx, വ്യഞ്ജകം, val_rtrn, ശൂന്യ, ശൂന്യ, ശൂന്യ);
}

എണ്ണൽ xkb_parser_error
വ്യഞ്ജകം_Resolve_Group(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                 സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    സ്ഥിതം സ്ഥിരം LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { ശൂന്യ, 0 }
    };

    സ്ഥിരം ഘടന named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = വലുപ്പം("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer_Lookup(keymap_info->keymap.ctx, വ്യഞ്ജകം, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    എങ്കിൽ (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    തിരികെ PARSER_SUCCESS;
}

bool
വ്യഞ്ജകം_Resolve_Level(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                 xkb_level_index_t *level_rtrn)
{
    സ്ഥിതം സ്ഥിരം ഘടന named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = വലുപ്പം("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = ശൂന്യ,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer_Lookup(ctx, വ്യഞ്ജകം, &result, ശൂന്യ,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        തിരികെ false;

    എങ്കിൽ (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        തിരികെ false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    തിരികെ true;
}

bool
വ്യഞ്ജകം_Resolve_Button(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം, int64_t *btn_rtrn)
{
    തിരികെ വ്യഞ്ജകം_Resolve_Integer_Lookup(ctx, വ്യഞ്ജകം, btn_rtrn, ശൂന്യ, SimpleLookup,
                                    buttonNames);
}

bool
വ്യഞ്ജകം_Resolve_String(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                  xkb_atom_t *val_rtrn)
{
    തിരഞ്ഞെടുപ്പ് (വ്യഞ്ജകം->common.type) {
    അവസ്ഥ STMT_വ്യഞ്ജകം_STRING_LITERAL:
        *val_rtrn = വ്യഞ്ജകം->string.str;
        തിരികെ true;

    അവസ്ഥ STMT_വ്യഞ്ജകം_INTEGER_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_FLOAT_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_BOOLEAN_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYNAME_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(വ്യഞ്ജകം->common.type));
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, വ്യഞ്ജകം->ident.ident));
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, വ്യഞ്ജകം->field_ref.element),
                xkb_atom_text(ctx, വ്യഞ്ജകം->field_ref.field));
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_ADD:
    അവസ്ഥ STMT_വ്യഞ്ജകം_SUBTRACT:
    അവസ്ഥ STMT_വ്യഞ്ജകം_MULTIPLY:
    അവസ്ഥ STMT_വ്യഞ്ജകം_DIVIDE:
    അവസ്ഥ STMT_വ്യഞ്ജകം_ASSIGN:
    അവസ്ഥ STMT_വ്യഞ്ജകം_NEGATE:
    അവസ്ഥ STMT_വ്യഞ്ജകം_INVERT:
    അവസ്ഥ STMT_വ്യഞ്ജകം_NOT:
    അവസ്ഥ STMT_വ്യഞ്ജകം_UNARY_PLUS:
    അവസ്ഥ STMT_വ്യഞ്ജകം_EMPTY_LIST:
    അവസ്ഥ STMT_വ്യഞ്ജകം_ACTION_DECL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_ACTION_LIST:
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(വ്യഞ്ജകം->common.type));
        തിരികെ false;

    സ്ഥിരസ്ഥിതി:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 വ്യഞ്ജകം->common.type);
        നിർത്തുക;
    }
    തിരികെ false;
}

bool
വ്യഞ്ജകം_Resolve_Enum(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                uint32_t *val_rtrn, സ്ഥിരം LookupEntry *മൂല്യങ്ങൾ)
{
    എങ്കിൽ (വ്യഞ്ജകം->common.type != STMT_വ്യഞ്ജകം_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(വ്യഞ്ജകം->common.type));
        തിരികെ false;
    }

    എങ്കിൽ (!SimpleLookup(ctx, മൂല്യങ്ങൾ, വ്യഞ്ജകം->ident.ident, val_rtrn, ശൂന്യ)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, വ്യഞ്ജകം->ident.ident));
        ആകുമ്പോൾ (മൂല്യങ്ങൾ && മൂല്യങ്ങൾ->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", മൂല്യങ്ങൾ->name);
            മൂല്യങ്ങൾ++;
        }
        തിരികെ false;
    }

    തിരികെ true;
}

സ്ഥിതം bool
വ്യഞ്ജകം_Resolve_Mask_Lookup(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, സ്ഥിരം ശൂന്യം *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    വ്യഞ്ജകം_Def *ഇടത്, *വലത്;
    സ്ഥിരം അക്ഷരം *bogus = ശൂന്യ;

    തിരഞ്ഞെടുപ്പ് (വ്യഞ്ജകം->common.type) {
    അവസ്ഥ STMT_വ്യഞ്ജകം_INTEGER_LITERAL:
        എങ്കിൽ (വ്യഞ്ജകം->integer.ival < 0 || വ്യഞ്ജകം->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    വ്യഞ്ജകം->integer.ival < 0 ? "-" : "",
                    imaxabs(വ്യഞ്ജകം->integer.ival),
                    UINT32_MAX);
            തിരികെ false;
        }
        *val_rtrn = (uint32_t) വ്യഞ്ജകം->integer.ival;
        തിരികെ true;

    അവസ്ഥ STMT_വ്യഞ്ജകം_STRING_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_FLOAT_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_BOOLEAN_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYNAME_LITERAL:
    അവസ്ഥ STMT_വ്യഞ്ജകം_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(വ്യഞ്ജകം->common.type));
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_IDENT:
        ok = lookup(ctx, lookupPriv, വ്യഞ്ജകം->ident.ident, val_rtrn, pending);
        എങ്കിൽ (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, വ്യഞ്ജകം->ident.ident));
        എങ്കിൽ (pending && *pending) {
            /* Postpone computation */
            തിരികെ false;
        }
        തിരികെ ok;

    അവസ്ഥ STMT_വ്യഞ്ജകം_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, വ്യഞ്ജകം->field_ref.element),
                xkb_atom_text(ctx, വ്യഞ്ജകം->field_ref.field));
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    അവസ്ഥ STMT_വ്യഞ്ജകം_ACTION_DECL:
        എങ്കിൽ (bogus == ശൂന്യ)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        തിരികെ false;

    അവസ്ഥ STMT_വ്യഞ്ജകം_ADD:
    അവസ്ഥ STMT_വ്യഞ്ജകം_SUBTRACT:
    അവസ്ഥ STMT_വ്യഞ്ജകം_MULTIPLY:
    അവസ്ഥ STMT_വ്യഞ്ജകം_DIVIDE:
        ഇടത് = വ്യഞ്ജകം->ദ്വിമാന.ഇടത്;
        വലത് = വ്യഞ്ജകം->ദ്വിമാന.വലത്;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mask_Lookup(ctx, ഇടത്, &l, pending, lookup, lookupPriv) ||
            !വ്യഞ്ജകം_Resolve_Mask_Lookup(ctx, വലത്, &r, pending, lookup, lookupPriv))
            തിരികെ false;

        തിരഞ്ഞെടുപ്പ് (വ്യഞ്ജകം->common.type) {
        അവസ്ഥ STMT_വ്യഞ്ജകം_ADD:
            *val_rtrn = l | r;
            നിർത്തുക;
        അവസ്ഥ STMT_വ്യഞ്ജകം_SUBTRACT:
            *val_rtrn = l & (~r);
            നിർത്തുക;
        അവസ്ഥ STMT_വ്യഞ്ജകം_MULTIPLY:
        അവസ്ഥ STMT_വ്യഞ്ജകം_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (വ്യഞ്ജകം->common.type == STMT_വ്യഞ്ജകം_DIVIDE ? "divide" : "multiply"));
            തിരികെ false;
        സ്ഥിരസ്ഥിതി:
            നിർത്തുക;
        }

        തിരികെ true;

    അവസ്ഥ STMT_വ്യഞ്ജകം_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        നിർത്തുക;

    അവസ്ഥ STMT_വ്യഞ്ജകം_INVERT:
        ഇടത് = വ്യഞ്ജകം->unary.child;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer_Lookup(ctx, ഇടത്, &v, pending, lookup, lookupPriv))
            തിരികെ false;
        എങ്കിൽ (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            തിരികെ false;
        }
        *val_rtrn = ~(uint32_t) v;
        തിരികെ true;

    അവസ്ഥ STMT_വ്യഞ്ജകം_UNARY_PLUS:
    അവസ്ഥ STMT_വ്യഞ്ജകം_NEGATE:
    അവസ്ഥ STMT_വ്യഞ്ജകം_NOT:
        ഇടത് = വ്യഞ്ജകം->unary.child;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer_Lookup(ctx, ഇടത്, &v, pending, lookup, lookupPriv))
            തിരികെ false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(വ്യഞ്ജകം->common.type));
        തിരികെ false;

    സ്ഥിരസ്ഥിതി:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 വ്യഞ്ജകം->common.type);
        നിർത്തുക;
    }

    തിരികെ false;
}

bool
വ്യഞ്ജകം_Resolve_Mask(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                uint32_t *mask_rtrn, സ്ഥിരം LookupEntry *മൂല്യങ്ങൾ)
{
    തിരികെ വ്യഞ്ജകം_Resolve_Mask_Lookup(ctx, വ്യഞ്ജകം, mask_rtrn, ശൂന്യ, SimpleLookup, മൂല്യങ്ങൾ);
}

bool
വ്യഞ്ജകം_Resolve_Mod_Mask(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം,
                   എണ്ണൽ mod_type mod_type, സ്ഥിരം ഘടന xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    തിരികെ വ്യഞ്ജകം_Resolve_Mask_Lookup(ctx, വ്യഞ്ജകം, mask_rtrn, ശൂന്യ, LookupModMask, &priv);
}

bool
വ്യഞ്ജകം_Resolve_Mod(ഘടന xkb_context *ctx, സ്ഥിരം വ്യഞ്ജകം_Def *def,
               എണ്ണൽ mod_type mod_type, സ്ഥിരം ഘടന xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    എങ്കിൽ (def->common.type != STMT_വ്യഞ്ജകം_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        തിരികെ false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    എങ്കിൽ (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        തിരികെ false;
    }

    *ndx_rtrn = ndx;
    തിരികെ true;
}

bool
വ്യഞ്ജകം_Resolve_Group_Mask(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                     സ്ഥിരം വ്യഞ്ജകം_Def *വ്യഞ്ജകം, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    സ്ഥിതം സ്ഥിരം LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { ശൂന്യ, 0 }
    };

    സ്ഥിരം ഘടന named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = വലുപ്പം("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    തിരികെ വ്യഞ്ജകം_Resolve_Mask_Lookup(keymap_info->keymap.ctx, വ്യഞ്ജകം, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
