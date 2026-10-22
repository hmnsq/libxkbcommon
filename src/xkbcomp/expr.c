/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdint.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "expr.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "keysym.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/ast.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "util-numbers.h"
#ଅନ୍ତର୍ଭାଵ "util-checked-arithmetic.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା bool (*IdentLookupFunc)(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଶୂନ୍ୟ *priv,
                                xkb_atom_t field, uint32_t *val_rtrn,
                                bool *pending);

bool
ପଦସମଷ୍ଟି_Resolve_Lhs(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
               ଅଚଳ ଅକ୍ଷର **elem_rtrn, ଅଚଳ ଅକ୍ଷର **field_rtrn,
               ପଦସମଷ୍ଟି_Def **index_rtrn)
{
    ବାଛ (ପଦସମଷ୍ଟି->common.type) {
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_IDENT:
        *elem_rtrn = ଖାଲି;
        *field_rtrn = xkb_atom_text(ctx, ପଦସମଷ୍ଟି->ident.ident);
        *index_rtrn = ଖାଲି;
        ଫେରନ୍ତୁ (*field_rtrn != ଖାଲି);
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_FIELD_REF:
        *elem_rtrn = xkb_atom_text(ctx, ପଦସମଷ୍ଟି->field_ref.element);
        *field_rtrn = xkb_atom_text(ctx, ପଦସମଷ୍ଟି->field_ref.field);
        *index_rtrn = ଖାଲି;
        ଫେରନ୍ତୁ (*elem_rtrn != ଖାଲି && *field_rtrn != ଖାଲି);
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ARRAY_REF:
        *elem_rtrn = xkb_atom_text(ctx, ପଦସମଷ୍ଟି->array_ref.element);
        *field_rtrn = xkb_atom_text(ctx, ପଦସମଷ୍ଟି->array_ref.field);
        *index_rtrn = ପଦସମଷ୍ଟି->array_ref.entry;
        ଯଦି (ପଦସମଷ୍ଟି->array_ref.element != XKB_ATOM_NONE && *elem_rtrn == ଖାଲି)
            ଫେରନ୍ତୁ false;
        ଯଦି (*field_rtrn == ଖାଲି)
            ଫେରନ୍ତୁ false;
        ଫେରନ୍ତୁ true;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଭାଙ୍ଗ;
    }
    log_wsgo(ctx,
             XKB_ERROR_INVALID_XKB_SYNTAX,
             "Unexpected operator %d in ResolveLhs\n", ପଦସମଷ୍ଟି->common.type);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
SimpleLookup(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଶୂନ୍ୟ *priv, xkb_atom_t field,
             uint32_t *val_rtrn, bool *pending_rtrn)
{
    ଯଦି (!priv || field == XKB_ATOM_NONE)
        ଫେରନ୍ତୁ false;

    ଅଚଳ ଅକ୍ଷର *str = xkb_atom_text(ctx, field);
    ପାଇଁ (ଅଚଳ LookupEntry *entry = priv; entry && entry->name; entry++) {
        ଯଦି (istreq(str, entry->name)) {
            *val_rtrn = entry->ମୂଲ୍ୟ;
            ଫେରନ୍ତୁ true;
        }
    }

    ଫେରନ୍ତୁ false;
}

ସଂରଚନା named_integer_pattern {
    ଅଚଳ ଅକ୍ଷର *prefix;
    size_t prefix_length;
    uint32_t min;
    uint32_t max;
    ଅଚଳ LookupEntry *entries;
    ଅଚଳ LookupEntry *pending_entries;
    bool is_mask;
    ଗଣନା xkb_message_code error_id;
};

/* Parse a number expressed with the pattern `<prefix><decimal number>` */
ସ୍ଥିର bool
NamedIntegerPatternLookup(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଶୂନ୍ୟ *priv,
                          xkb_atom_t field, uint32_t *val_rtrn,
                          bool *pending_rtrn)
{
    ଯଦି (!priv || field == XKB_ATOM_NONE)
        ଫେରନ୍ତୁ false;

    ଅଚଳ ଅକ୍ଷର * ଅଚଳ str = xkb_atom_text(ctx, field);
    ଅଚଳ ସଂରଚନା named_integer_pattern * ଅଚଳ pattern = priv;

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = (istrneq(str, pattern->prefix, pattern->prefix_length))
        ? parse_dec_to_uint32_t(str + pattern->prefix_length, SIZE_MAX, val_rtrn)
        : 0;

    ଯଦି (ଗଣନା_2 > 0 && *(str + pattern->prefix_length + ଗଣନା_2) == '\0') {
        ଯଦି (*val_rtrn < pattern->min || *val_rtrn > pattern->max) {
            log_err(ctx, pattern->error_id,
                    "%s index %"PRIu32" is out of range (%"PRIu32"..%"PRIu32")\n",
                    pattern->prefix, *val_rtrn, pattern->min, pattern->max);
            ଫେରନ୍ତୁ false;
        }
        ଯଦି (pattern->is_mask) {
            /* Compute mask (bit 0 = min index) */
            ପ୍ରତିଜ୍ଞା(*val_rtrn - pattern->min < 32);
            *val_rtrn = UINT32_C(1) << (*val_rtrn - pattern->min);
        }
        ଫେରନ୍ତୁ true;
    } ଅନ୍ୟଥା {
        ଯଦି (pattern->entries &&
            SimpleLookup(ctx, pattern->entries, field, val_rtrn, ଖାଲି)) {
            ଫେରନ୍ତୁ true;
        }
        ଯଦି (pattern->pending_entries && pending_rtrn &&
            SimpleLookup(ctx, pattern->pending_entries, field, val_rtrn, ଖାଲି)) {
            *pending_rtrn = true;
            ଫେରନ୍ତୁ true;
        }
        ଫେରନ୍ତୁ false;
    }
}

/* Data passed in the *priv argument for LookupModMask. */
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଅଚଳ ସଂରଚନା xkb_mod_set *mods;
    ଗଣନା mod_type mod_type;
} LookupModMaskPriv;

ସ୍ଥିର bool
LookupModMask(ସଂରଚନା xkb_context *ctx, ଅଚଳ ଶୂନ୍ୟ *priv, xkb_atom_t field,
              xkb_mod_mask_t *val_rtrn, bool *pending_rtrn)
{
    ଅଚଳ ଅକ୍ଷର *str = xkb_atom_text(ctx, field);
    ଯଦି (!str)
        ଫେରନ୍ତୁ false;

    ଯଦି (istreq(str, "all")) {
        *val_rtrn  = MOD_REAL_MASK_ALL;
        ଫେରନ୍ତୁ true;
    }

    ଯଦି (istreq(str, "none")) {
        *val_rtrn = 0;
        ଫେରନ୍ତୁ true;
    }

    ଅଚଳ LookupModMaskPriv *arg = priv;
    ଅଚଳ ସଂରଚନା xkb_mod_set *mods = arg->mods;
    ଅଚଳ ଗଣନା mod_type mod_type = arg->mod_type;
    ଅଚଳ xkb_mod_index_t ndx = XkbModNameToIndex(mods, field, mod_type);
    ଯଦି (ndx == XKB_MOD_INVALID)
        ଫେରନ୍ତୁ false;

    *val_rtrn = (UINT32_C(1) << ndx);
    ଫେରନ୍ତୁ true;
}

bool
ପଦସମଷ୍ଟି_Resolve_Boolean(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                   bool *set_rtrn)
{
    bool ok = false;
    ଅଚଳ ଅକ୍ଷର *ident;

    ବାଛ (ପଦସମଷ୍ଟି->common.type) {
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_BOOLEAN_LITERAL:
        *set_rtrn = ପଦସମଷ୍ଟି->boolean.set;
        ଫେରନ୍ତୁ true;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_STRING_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_INTEGER_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_FLOAT_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where boolean was expected\n",
                stmt_type_to_string(ପଦସମଷ୍ଟି->common.type));
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_IDENT:
        ident = xkb_atom_text(ctx, ପଦସମଷ୍ଟି->ident.ident);
        ଯଦି (ident) {
            ଯଦି (istreq(ident, "true") ||
                istreq(ident, "yes") ||
                istreq(ident, "on")) {
                *set_rtrn = true;
                ଫେରନ୍ତୁ true;
            }
            ଅନ୍ୟଥା ଯଦି (istreq(ident, "false") ||
                     istreq(ident, "no") ||
                     istreq(ident, "off")) {
                *set_rtrn = false;
                ଫେରନ୍ତୁ true;
            }
        }
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type boolean is unknown\n", ident);
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type boolean is unknown\n",
                xkb_atom_text(ctx, ପଦସମଷ୍ଟି->field_ref.element),
                xkb_atom_text(ctx, ପଦସମଷ୍ଟି->field_ref.field));
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_INVERT:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NOT:
        ok = ପଦସମଷ୍ଟି_Resolve_Boolean(ctx, ପଦସମଷ୍ଟି->unary.child, set_rtrn);
        ଯଦି (ok)
            *set_rtrn = !*set_rtrn;
        ଫେରନ୍ତୁ ok;
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ADD:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_SUBTRACT:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_MULTIPLY:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_DIVIDE:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ASSIGN:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NEGATE:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_UNARY_PLUS:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_EMPTY_LIST:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ACTION_DECL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ACTION_LIST:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "%s of boolean values not permitted\n",
                stmt_type_to_string(ପଦସମଷ୍ଟି->common.type));
        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveBoolean\n",
                 ପଦସମଷ୍ଟି->common.type);
        ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                         int64_t *val_rtrn, bool *pending,
                         IdentLookupFunc lookup, ଅଚଳ ଶୂନ୍ୟ *lookupPriv)
{
    bool ok = false;
    int64_t l = 0, r = 0;
    uint32_t u = 0;
    ପଦସମଷ୍ଟି_Def *ବାମ, *ଡାହାଣ;

    ବାଛ (ପଦସମଷ୍ଟି->common.type) {
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_INTEGER_LITERAL:
        *val_rtrn = ପଦସମଷ୍ଟି->integer.ival;
        ଫେରନ୍ତୁ true;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_STRING_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_FLOAT_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_BOOLEAN_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where an int was expected\n",
                stmt_type_to_string(ପଦସମଷ୍ଟି->common.type));
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_IDENT:
        ଯଦି (lookup)
            ok = lookup(ctx, lookupPriv, ପଦସମଷ୍ଟି->ident.ident, &u, pending);

        ଯଦି (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, ପଦସମଷ୍ଟି->ident.ident));
        ଅନ୍ୟଥା
            *val_rtrn = (int64_t) u;

        ଯଦି (pending && *pending) {
            /* Postpone computation */
            ଫେରନ୍ତୁ false;
        }

        ଫେରନ୍ତୁ ok;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, ପଦସମଷ୍ଟି->field_ref.element),
                xkb_atom_text(ctx, ପଦସମଷ୍ଟି->field_ref.field));
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ADD:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_SUBTRACT:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_MULTIPLY:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_DIVIDE:
        ବାମ = ପଦସମଷ୍ଟି->ଦ୍ୱିମିକ.ବାମ;
        ଡାହାଣ = ପଦସମଷ୍ଟି->ଦ୍ୱିମିକ.ଡାହାଣ;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(ctx, ବାମ, &l, pending, lookup, lookupPriv) ||
            !ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(ctx, ଡାହାଣ, &r, pending, lookup, lookupPriv))
            ଫେରନ୍ତୁ false;

        ବାଛ (ପଦସମଷ୍ଟି->common.type) {
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ADD:
            ଯଦି (ckd_add(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Addition %"PRId64" + %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ଫେରନ୍ତୁ false;
            }
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_SUBTRACT:
            ଯଦି (ckd_sub(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Substraction %"PRId64" - %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ଫେରନ୍ତୁ false;
            }
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_MULTIPLY:
            ଯଦି (ckd_mul(val_rtrn, l, r)) {
                log_err(ctx, XKB_ERROR_INTEGER_OVERFLOW,
                        "Multiplication %"PRId64" * %"PRId64" has an invalid "
                        "mathematical result: %"PRId64"\n",
                        l, r, *val_rtrn);
                ଫେରନ୍ତୁ false;
            }
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_DIVIDE:
            ଯଦି (r == 0) {
                log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                        "Cannot divide by zero: %"PRId64" / %"PRId64"\n", l, r);
                ଫେରନ୍ତୁ false;
            }
            *val_rtrn = l / r;
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "%s of integers not permitted\n",
                    stmt_type_to_string(ପଦସମଷ୍ଟି->common.type));
            ଫେରନ୍ତୁ false;
        }

        ଫେରନ୍ତୁ true;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ASSIGN:
        log_wsgo(ctx,
                 XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ଭାଙ୍ଗ;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NOT:
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The ! operator cannot be applied to an integer\n");
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_INVERT:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NEGATE:
        ବାମ = ପଦସମଷ୍ଟି->unary.child;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(ctx, ବାମ, &l, pending, lookup, lookupPriv))
            ଫେରନ୍ତୁ false;

        *val_rtrn = (ପଦସମଷ୍ଟି->common.type == STMT_ପଦସମଷ୍ଟି_NEGATE ? -l : ~l);
        ଫେରନ୍ତୁ true;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_UNARY_PLUS:
        ବାମ = ପଦସମଷ୍ଟି->unary.child;
        ଫେରନ୍ତୁ ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(ctx, ବାମ, val_rtrn, pending, lookup,
                                        lookupPriv);

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveInteger\n",
                 ପଦସମଷ୍ଟି->common.type);
        ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ false;
}

bool
ପଦସମଷ୍ଟି_Resolve_Integer(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                   int64_t *val_rtrn)
{
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(ctx, ପଦସମଷ୍ଟି, val_rtrn, ଖାଲି, ଖାଲି, ଖାଲି);
}

ଗଣନା xkb_parser_error
ପଦସମଷ୍ଟି_Resolve_Group(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                 ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି, bool absolute,
                 xkb_layout_index_t *group_rtrn, bool *pending)
{
    ସ୍ଥିର ଅଚଳ LookupEntry pendingGroupIndexNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { ଖାଲି, 0 }
    };

    ଅଚଳ ସଂରଚନା named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = ଆକାର("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = false,
        .entries = keymap_info->lookup.groupIndexNames,
        .pending_entries = pendingGroupIndexNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
    };

    int64_t result = 0;
    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(keymap_info->keymap.ctx, ପଦସମଷ୍ଟି, &result, pending,
                                  NamedIntegerPatternLookup, &group_name_pattern))
        ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;

    ଯଦି (result < (int64_t)absolute || result > keymap_info->features.max_groups) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Group index %"PRId64" is out of range (%u..%"PRIu32")\n",
                result, absolute, keymap_info->features.max_groups);
        ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    *group_rtrn = (xkb_layout_index_t) result;
    ଫେରନ୍ତୁ PARSER_SUCCESS;
}

bool
ପଦସମଷ୍ଟି_Resolve_Level(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                 xkb_level_index_t *level_rtrn)
{
    ସ୍ଥିର ଅଚଳ ସଂରଚନା named_integer_pattern level_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Level",
        .prefix_length = ଆକାର("Level") - 1,
        .min = 1,
        .max = XKB_LEVEL_MAX_IMPL,
        .is_mask = false,
        .entries = ଖାଲି,
        .error_id = XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
    };
    int64_t result = 0;
    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(ctx, ପଦସମଷ୍ଟି, &result, ଖାଲି,
                                  NamedIntegerPatternLookup,
                                  &level_name_pattern))
        ଫେରନ୍ତୁ false;

    ଯଦି (result < 1 || result > XKB_LEVEL_MAX_IMPL) {
        log_err(ctx, XKB_ERROR_UNSUPPORTED_SHIFT_LEVEL,
                "Shift level %"PRId64" is out of range (1..%u)\n",
                result, XKB_LEVEL_MAX_IMPL);
        ଫେରନ୍ତୁ false;
    }

    /* Level is zero-indexed from now on. */
    *level_rtrn = (xkb_level_index_t) (result - 1);
    ଫେରନ୍ତୁ true;
}

bool
ପଦସମଷ୍ଟି_Resolve_Button(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି, int64_t *btn_rtrn)
{
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(ctx, ପଦସମଷ୍ଟି, btn_rtrn, ଖାଲି, SimpleLookup,
                                    buttonNames);
}

bool
ପଦସମଷ୍ଟି_Resolve_String(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                  xkb_atom_t *val_rtrn)
{
    ବାଛ (ପଦସମଷ୍ଟି->common.type) {
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_STRING_LITERAL:
        *val_rtrn = ପଦସମଷ୍ଟି->string.str;
        ଫେରନ୍ତୁ true;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_INTEGER_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_FLOAT_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_BOOLEAN_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s, expected a string\n",
                stmt_type_to_string(ପଦସମଷ୍ଟି->common.type));
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_IDENT:
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Identifier \"%s\" of type string not found\n",
                xkb_atom_text(ctx, ପଦସମଷ୍ଟି->ident.ident));
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type string not found\n",
                xkb_atom_text(ctx, ପଦସମଷ୍ଟି->field_ref.element),
                xkb_atom_text(ctx, ପଦସମଷ୍ଟି->field_ref.field));
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ADD:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_SUBTRACT:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_MULTIPLY:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_DIVIDE:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ASSIGN:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NEGATE:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_INVERT:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NOT:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_UNARY_PLUS:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_EMPTY_LIST:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ACTION_DECL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ACTION_LIST:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYSYM_LIST:
        log_err(ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                "%s of strings not permitted\n",
                stmt_type_to_string(ପଦସମଷ୍ଟି->common.type));
        ଫେରନ୍ତୁ false;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator %d in ResolveString\n",
                 ପଦସମଷ୍ଟି->common.type);
        ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ false;
}

bool
ପଦସମଷ୍ଟି_Resolve_Enum(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                uint32_t *val_rtrn, ଅଚଳ LookupEntry *ମୂଲ୍ୟଗୁଡ଼ିକ)
{
    ଯଦି (ପଦସମଷ୍ଟି->common.type != STMT_ପଦସମଷ୍ଟି_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found a %s where an enumerated value was expected\n",
                stmt_type_to_string(ପଦସମଷ୍ଟି->common.type));
        ଫେରନ୍ତୁ false;
    }

    ଯଦି (!SimpleLookup(ctx, ମୂଲ୍ୟଗୁଡ଼ିକ, ପଦସମଷ୍ଟି->ident.ident, val_rtrn, ଖାଲି)) {
        log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                "Illegal identifier %s; expected one of:\n",
                xkb_atom_text(ctx, ପଦସମଷ୍ଟି->ident.ident));
        ଯେତେବେଳେ (ମୂଲ୍ୟଗୁଡ଼ିକ && ମୂଲ୍ୟଗୁଡ଼ିକ->name)
        {
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER, "\t%s\n", ମୂଲ୍ୟଗୁଡ଼ିକ->name);
            ମୂଲ୍ୟଗୁଡ଼ିକ++;
        }
        ଫେରନ୍ତୁ false;
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
ପଦସମଷ୍ଟି_Resolve_Mask_Lookup(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                      uint32_t *val_rtrn, bool *pending,
                      IdentLookupFunc lookup, ଅଚଳ ଶୂନ୍ୟ *lookupPriv)
{
    bool ok = false;
    uint32_t l = 0, r = 0;
    int64_t v = 0;
    ପଦସମଷ୍ଟି_Def *ବାମ, *ଡାହାଣ;
    ଅଚଳ ଅକ୍ଷର *bogus = ଖାଲି;

    ବାଛ (ପଦସମଷ୍ଟି->common.type) {
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_INTEGER_LITERAL:
        ଯଦି (ପଦସମଷ୍ଟି->integer.ival < 0 || ପଦସମଷ୍ଟି->integer.ival > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    ପଦସମଷ୍ଟି->integer.ival < 0 ? "-" : "",
                    imaxabs(ପଦସମଷ୍ଟି->integer.ival),
                    UINT32_MAX);
            ଫେରନ୍ତୁ false;
        }
        *val_rtrn = (uint32_t) ପଦସମଷ୍ଟି->integer.ival;
        ଫେରନ୍ତୁ true;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_STRING_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_FLOAT_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_BOOLEAN_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_KEYSYM_LITERAL:
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Found %s where a mask was expected\n",
                stmt_type_to_string(ପଦସମଷ୍ଟି->common.type));
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_IDENT:
        ok = lookup(ctx, lookupPriv, ପଦସମଷ୍ଟି->ident.ident, val_rtrn, pending);
        ଯଦି (!ok)
            log_err(ctx, XKB_ERROR_INVALID_IDENTIFIER,
                    "Identifier \"%s\" of type int is unknown\n",
                    xkb_atom_text(ctx, ପଦସମଷ୍ଟି->ident.ident));
        ଯଦି (pending && *pending) {
            /* Postpone computation */
            ଫେରନ୍ତୁ false;
        }
        ଫେରନ୍ତୁ ok;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_FIELD_REF:
        log_err(ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                "Default \"%s.%s\" of type int is unknown\n",
                xkb_atom_text(ctx, ପଦସମଷ୍ଟି->field_ref.element),
                xkb_atom_text(ctx, ପଦସମଷ୍ଟି->field_ref.field));
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ARRAY_REF:
        bogus = "array reference";
        /* fallthrough */
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ACTION_DECL:
        ଯଦି (bogus == ଖାଲି)
            bogus = "function use";
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Unexpected %s in mask expression; Expression Ignored\n",
                bogus);
        ଫେରନ୍ତୁ false;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ADD:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_SUBTRACT:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_MULTIPLY:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_DIVIDE:
        ବାମ = ପଦସମଷ୍ଟି->ଦ୍ୱିମିକ.ବାମ;
        ଡାହାଣ = ପଦସମଷ୍ଟି->ଦ୍ୱିମିକ.ଡାହାଣ;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mask_Lookup(ctx, ବାମ, &l, pending, lookup, lookupPriv) ||
            !ପଦସମଷ୍ଟି_Resolve_Mask_Lookup(ctx, ଡାହାଣ, &r, pending, lookup, lookupPriv))
            ଫେରନ୍ତୁ false;

        ବାଛ (ପଦସମଷ୍ଟି->common.type) {
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ADD:
            *val_rtrn = l | r;
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_SUBTRACT:
            *val_rtrn = l & (~r);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_MULTIPLY:
        ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_DIVIDE:
            log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                    "Cannot %s masks; Illegal operation ignored\n",
                    (ପଦସମଷ୍ଟି->common.type == STMT_ପଦସମଷ୍ଟି_DIVIDE ? "divide" : "multiply"));
            ଫେରନ୍ତୁ false;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ଭାଙ୍ଗ;
        }

        ଫେରନ୍ତୁ true;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_ASSIGN:
        log_wsgo(ctx, XKB_ERROR_INVALID_OPERATION,
                 "Assignment operator not implemented yet\n");
        ଭାଙ୍ଗ;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_INVERT:
        ବାମ = ପଦସମଷ୍ଟି->unary.child;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(ctx, ବାମ, &v, pending, lookup, lookupPriv))
            ଫେରନ୍ତୁ false;
        ଯଦି (v < 0 || v > UINT32_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Mask %s%#"PRIxMAX" is out of range (0..%#"PRIx32")\n",
                    v < 0 ? "-" : "", imaxabs(v), UINT32_MAX);
            ଫେରନ୍ତୁ false;
        }
        *val_rtrn = ~(uint32_t) v;
        ଫେରନ୍ତୁ true;

    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_UNARY_PLUS:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NEGATE:
    ସ୍ଥିତି STMT_ପଦସମଷ୍ଟି_NOT:
        ବାମ = ପଦସମଷ୍ଟି->unary.child;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer_Lookup(ctx, ବାମ, &v, pending, lookup, lookupPriv))
            ଫେରନ୍ତୁ false;
        log_err(ctx, XKB_ERROR_INVALID_OPERATION,
                "The '%c' unary operator cannot be used with a mask\n",
                stmt_type_to_operator_char(ପଦସମଷ୍ଟି->common.type));
        ଫେରନ୍ତୁ false;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        log_wsgo(ctx, XKB_ERROR_UNKNOWN_OPERATOR,
                 "Unknown operator type %d in ResolveMask\n",
                 ପଦସମଷ୍ଟି->common.type);
        ଭାଙ୍ଗ;
    }

    ଫେରନ୍ତୁ false;
}

bool
ପଦସମଷ୍ଟି_Resolve_Mask(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                uint32_t *mask_rtrn, ଅଚଳ LookupEntry *ମୂଲ୍ୟଗୁଡ଼ିକ)
{
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି_Resolve_Mask_Lookup(ctx, ପଦସମଷ୍ଟି, mask_rtrn, ଖାଲି, SimpleLookup, ମୂଲ୍ୟଗୁଡ଼ିକ);
}

bool
ପଦସମଷ୍ଟି_Resolve_Mod_Mask(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି,
                   ଗଣନା mod_type mod_type, ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
                   xkb_mod_mask_t *mask_rtrn)
{
    LookupModMaskPriv priv = { .mods = mods, .mod_type = mod_type };
    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି_Resolve_Mask_Lookup(ctx, ପଦସମଷ୍ଟି, mask_rtrn, ଖାଲି, LookupModMask, &priv);
}

bool
ପଦସମଷ୍ଟି_Resolve_Mod(ସଂରଚନା xkb_context *ctx, ଅଚଳ ପଦସମଷ୍ଟି_Def *def,
               ଗଣନା mod_type mod_type, ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
               xkb_mod_index_t *ndx_rtrn)
{
    ଯଦି (def->common.type != STMT_ପଦସମଷ୍ଟି_IDENT) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Cannot resolve virtual modifier: "
                "found %s where a virtual modifier name was expected\n",
                stmt_type_to_string(def->common.type));
        ଫେରନ୍ତୁ false;
    }

    xkb_atom_t name = def->ident.ident;
    xkb_mod_index_t ndx = XkbModNameToIndex(mods, name, mod_type);
    ଯଦି (ndx == XKB_MOD_INVALID) {
        log_err(ctx, XKB_ERROR_UNDECLARED_VIRTUAL_MODIFIER,
                "Cannot resolve virtual modifier: "
                "\"%s\" was not previously declared\n",
                xkb_atom_text(ctx, name));
        ଫେରନ୍ତୁ false;
    }

    *ndx_rtrn = ndx;
    ଫେରନ୍ତୁ true;
}

bool
ପଦସମଷ୍ଟି_Resolve_Group_Mask(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                     ଅଚଳ ପଦସମଷ୍ଟି_Def *ପଦସମଷ୍ଟି, xkb_layout_mask_t *group_rtrn,
                     bool *pending_rtrn)
{
    ସ୍ଥିର ଅଚଳ LookupEntry pendingGroupMaskNames[] = {
        { GROUP_LAST_INDEX_NAME, 0 },
        { ଖାଲି, 0 }
    };

    ଅଚଳ ସଂରଚନା named_integer_pattern group_name_pattern = {
        /* Prefix is title-cased, because it is also used in error messages */
        .prefix = "Group",
        .prefix_length = ଆକାର("Group") - 1,
        .min = 1,
        .max = keymap_info->features.max_groups,
        .is_mask = true,
        .entries = keymap_info->lookup.groupMaskNames,
        .pending_entries = pendingGroupMaskNames,
        .error_id = XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_
    };

    ଫେରନ୍ତୁ ପଦସମଷ୍ଟି_Resolve_Mask_Lookup(keymap_info->keymap.ctx, ପଦସମଷ୍ଟି, group_rtrn,
                                 pending_rtrn, NamedIntegerPatternLookup,
                                 &group_name_pattern);
}
