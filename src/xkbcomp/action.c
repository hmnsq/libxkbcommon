/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 * Author: Ran Benita <ran234@gmail.com>
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "expr.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/ast.h"
#ଅନ୍ତର୍ଭାଵ "action.h"

ସ୍ଥିର ଅଚଳ ପଦସମଷ୍ଟି_Boolean constTrue = {
    .common = { .type = STMT_ପଦସମଷ୍ଟି_BOOLEAN_LITERAL, .next = ଖାଲି },
    .set = true,
};

ସ୍ଥିର ଅଚଳ ପଦସମଷ୍ଟି_Boolean constFalse = {
    .common = { .type = STMT_ପଦସମଷ୍ଟି_BOOLEAN_LITERAL, .next = ଖାଲି },
    .set = false,
};

ଗଣନା action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_ଚାବି_EVENT,
    ACTION_FIELD_REPORT,
    ACTION_FIELD_DEFAULT,
    ACTION_FIELD_AFFECT,
    ACTION_FIELD_INCREMENT,
    ACTION_FIELD_MODIFIERS,
    ACTION_FIELD_GROUP,
    ACTION_FIELD_X,
    ACTION_FIELD_Y,
    ACTION_FIELD_REPEAT,
    ACTION_FIELD_BUTTON,
    ACTION_FIELD_ମୂଲ୍ୟ,
    ACTION_FIELD_CONTROLS,
    ACTION_FIELD_TYPE,
    ACTION_FIELD_ଗଣନା,
    ACTION_FIELD_SCREEN,
    ACTION_FIELD_SAME,
    ACTION_FIELD_DATA,
    ACTION_FIELD_DEVICE,
    ACTION_FIELD_KEYCODE,
    ACTION_FIELD_MODS_TO_CLEAR,
    ACTION_FIELD_LOCK_ON_RELEASE,
    ACTION_FIELD_UNLOCK_ON_PRESS,
    ACTION_FIELD_LATCH_ON_PRESS,
};

ଶୂନ୍ୟ
InitActionsInfo(ଅଚଳ ସଂରଚନା xkb_keymap *keymap, ActionsInfo *info)
{
    ପାଇଁ (ଗଣନା xkb_action_type type = 0; type < ACTION_TYPE_ଅଙ୍କ_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.ମୂଲ୍ୟ = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_ଚାବି].redirect.keycode =
        keymap->redirect_ଚାବି_auto;
}

ସ୍ଥିର ଅଚଳ LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_ଚାବି_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_ଚାବି_EVENT   },
    { "report",           ACTION_FIELD_REPORT          },
    { "default",          ACTION_FIELD_DEFAULT         },
    { "affect",           ACTION_FIELD_AFFECT          },
    { "increment",        ACTION_FIELD_INCREMENT       },
    { "modifiers",        ACTION_FIELD_MODIFIERS       },
    { "mods",             ACTION_FIELD_MODIFIERS       },
    { "group",            ACTION_FIELD_GROUP           },
    { "x",                ACTION_FIELD_X               },
    { "y",                ACTION_FIELD_Y               },
    { "repeat",           ACTION_FIELD_REPEAT          },
    { "accel",            ACTION_FIELD_REPEAT          },
    { "accelerate",       ACTION_FIELD_REPEAT          },
    { "button",           ACTION_FIELD_BUTTON          },
    { "value",            ACTION_FIELD_ମୂଲ୍ୟ           },
    { "controls",         ACTION_FIELD_CONTROLS        },
    { "ctrls",            ACTION_FIELD_CONTROLS        },
    { "type",             ACTION_FIELD_TYPE            },
    { "count",            ACTION_FIELD_ଗଣନା           },
    { "screen",           ACTION_FIELD_SCREEN          },
    { "same",             ACTION_FIELD_SAME            },
    { "sameServer",       ACTION_FIELD_SAME            },
    { "data",             ACTION_FIELD_DATA            },
    { "device",           ACTION_FIELD_DEVICE          },
    { "dev",              ACTION_FIELD_DEVICE          },
    { "key",              ACTION_FIELD_KEYCODE         },
    { "keycode",          ACTION_FIELD_KEYCODE         },
    { "kc",               ACTION_FIELD_KEYCODE         },
    { "clearmods",        ACTION_FIELD_MODS_TO_CLEAR   },
    { "clearmodifiers",   ACTION_FIELD_MODS_TO_CLEAR   },
    { "lockOnRelease",    ACTION_FIELD_LOCK_ON_RELEASE },
    { "unlockOnPress",    ACTION_FIELD_UNLOCK_ON_PRESS },
    { "latchOnPress",     ACTION_FIELD_LATCH_ON_PRESS  },
    { ଖାଲି,               0                            }
};

ସ୍ଥିର bool
stringToActionType(ଅଚଳ ଅକ୍ଷର *str, ଗଣନା xkb_action_type *type_rtrn)
{
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = 0;
    ଅଚଳ bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (ଗଣନା xkb_action_type) type;
    ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର bool
stringToField(ଅଚଳ ଅକ୍ଷର *str, ଗଣନା action_field *field_rtrn)
{
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା field = 0;
    ଅଚଳ bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (ଗଣନା action_field) field;
    ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
fieldText(ଗଣନା action_field field)
{
    ଫେରନ୍ତୁ Lookup_ମୂଲ୍ୟ(fieldStrings, field);
}

/***====================================================================***/

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା xkb_parser_error
ReportMismatch(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_message_code code,
               ଗଣନା xkb_action_type action, ଗଣନା action_field field,
               ଅଚଳ ଅକ୍ଷର *type, ଗଣନା xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    ଫେରନ୍ତୁ (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା xkb_parser_error
ReportFormatVersionMismatch(ସଂରଚନା xkb_context *ctx,
                            ଗଣନା xkb_action_type action,
                            ଗଣନା action_field field,
                            ଗଣନା xkb_keymap_format format,
                            ଅଚଳ ଅକ୍ଷର *versions,
                            ଗଣନା xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    ଫେରନ୍ତୁ (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା xkb_parser_error
ReportIllegal(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_action_type action,
              ଗଣନା action_field field, ଗଣନା xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    ଫେରନ୍ତୁ (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଗଣନା xkb_parser_error
ReportActionNotArray(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_action_type action,
                     ଗଣନା action_field field,
                     ଗଣନା xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    ଫେରନ୍ତୁ (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ସ୍ଥିର ଗଣନା xkb_parser_error
HandleNoAction(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
               ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
               ସଂଘ xkb_action *action, ଗଣନା action_field field,
               ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
               ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ସ୍ଥିର ଗଣନା xkb_parser_error
CheckBooleanFlag(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_parser_strict_flags strict,
                 ଗଣନା xkb_action_type action, ଗଣନା action_field field,
                 ଗଣନା xkb_action_flags flag, ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx,
                 ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ, ଗଣନା xkb_action_flags *flags_inout)
{
    bool set = false;

    ଯଦି (array_ndx)
        ଫେରନ୍ତୁ ReportActionNotArray(ctx, action, field, strict);

    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Boolean(ctx, ମୂଲ୍ୟ, &set))
        ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    ଯଦି (set)
        *flags_inout |= flag;
    ଅନ୍ୟଥା
        *flags_inout &= ~flag;

    ଫେରନ୍ତୁ PARSER_SUCCESS;
}

ସ୍ଥିର ଗଣନା xkb_parser_error
CheckModifierField(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_parser_strict_flags strict,
                   ଅଚଳ ସଂରଚନା xkb_mod_set *mods, ଗଣନା xkb_action_type action,
                   ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
                   ଗଣନା xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    ଯଦି (array_ndx)
        ଫେରନ୍ତୁ ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    ଯଦି (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_IDENT) {
        ଅଚଳ ଅକ୍ଷର *valStr;
        valStr = xkb_atom_text(ctx, ମୂଲ୍ୟ->ident.ident);
        ଯଦି (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            ଫେରନ୍ତୁ PARSER_SUCCESS;
        }
    }

    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mod_Mask(ctx, ମୂଲ୍ୟ, MOD_BOTH, mods, mods_rtrn))
        ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    ଫେରନ୍ତୁ PARSER_SUCCESS;
}

ସ୍ଥିର ଅଚଳ LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { ଖାଲି, 0 }
};

ସ୍ଥିର ଗଣନା xkb_parser_error
CheckAffectField(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_parser_strict_flags strict,
                 ଗଣନା xkb_action_type action, ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx,
                 ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ, ଗଣନା xkb_action_flags *flags_inout)
{
    ଯଦି (array_ndx)
        ଫେରନ୍ତୁ ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Enum(ctx, ମୂଲ୍ୟ, &flags, lockWhich))
        ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (ଗଣନା xkb_action_flags) flags;
    ଫେରନ୍ତୁ PARSER_SUCCESS;
}

ସ୍ଥିର ଗଣନା xkb_parser_error
HandleSetLatchLockMods(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                       ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
                       ସଂଘ xkb_action *action, ଗଣନା action_field field,
                       ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
                       ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ସଂରଚନା xkb_context * restrict ଅଚଳ ctx = keymap_info->keymap.ctx;
    ସଂରଚନା xkb_mod_action *act = &action->mods;
    ଅଚଳ ଗଣନା xkb_action_type type = action->type;

    ଯଦି (field == ACTION_FIELD_MODIFIERS)
        ଫେରନ୍ତୁ CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, ମୂଲ୍ୟ, &act->flags, &act->mods.mods);
    ଯଦି (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        ପ୍ରତିଜ୍ଞା(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        ଯଦି (keymap_info->features.mods_unlock_on_press) {
            ଅଚଳ ଗଣନା xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, ମୂଲ୍ୟ, &act->flags
            );
            ଯଦି (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            ଫେରନ୍ତୁ ret;
        } ଅନ୍ୟଥା {
            ଫେରନ୍ତୁ ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    ଯଦି ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        ଫେରନ୍ତୁ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, ମୂଲ୍ୟ,
                                &act->flags);
    ଯଦି (type == ACTION_TYPE_MOD_LATCH) {
        ଯଦି (field == ACTION_FIELD_LATCH_TO_LOCK)
            ଫେରନ୍ତୁ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    ମୂଲ୍ୟ, &act->flags);
        ଯଦି (field == ACTION_FIELD_LATCH_ON_PRESS) {
            ଯଦି (keymap_info->features.mods_latch_on_press) {
                ଫେରନ୍ତୁ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        ମୂଲ୍ୟ, &act->flags);
            } ଅନ୍ୟଥା {
                ଫେରନ୍ତୁ ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    ଯଦି (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        ଫେରନ୍ତୁ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, ମୂଲ୍ୟ, &act->flags);

    ଫେରନ୍ତୁ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ସ୍ଥିର ଗଣନା xkb_parser_error
CheckGroupField(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                ଗଣନା xkb_action_type action, ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx,
                ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ, ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr,
                ଗଣନା xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    ଅଚଳ ପଦସମଷ୍ଟି_Def *spec;
    xkb_layout_index_t idx = 0;
    ଗଣନା xkb_action_flags flags = *flags_inout;

    ଯଦି (array_ndx)
        ଫେରନ୍ତୁ ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    ଯଦି (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_NEGATE ||
        ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = ମୂଲ୍ୟ->unary.child;
        ମୂଲ୍ୟ_ptr = &(*ମୂଲ୍ୟ_ptr)->unary.child;
    } ଅନ୍ୟଥା {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = ମୂଲ୍ୟ;
    }

    ଅଚଳ bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ଅଚଳ ଗଣନା xkb_parser_error ret =
        ପଦସମଷ୍ଟି_Resolve_Group(keymap_info, spec, absolute, &idx, &pending);
    ଯଦି (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        ଫେରନ୍ତୁ ret;
    }

    ଯଦି (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ଅଚଳ darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (ସଂରଚନା pending_computation) {
                .ପଦସମଷ୍ଟି = *ମୂଲ୍ୟ_ptr,
                .computed = false,
                .ମୂଲ୍ୟ = 0,
            }
        );
        *ମୂଲ୍ୟ_ptr = ଖାଲି;
        static_assert(ଆକାର(pending_index) == ଆକାର(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } ଅନ୍ୟଥା {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        ଯଦି (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } ଅନ୍ୟଥା {
            *group_rtrn = (int32_t) idx;
            ଯଦି (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    ଫେରନ୍ତୁ PARSER_SUCCESS;
}

ସ୍ଥିର ଗଣନା xkb_parser_error
HandleSetLatchLockGroup(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                        ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
                        ସଂଘ xkb_action *action, ଗଣନା action_field field,
                        ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
                        ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ସଂରଚନା xkb_context * restrict ଅଚଳ ctx = keymap_info->keymap.ctx;
    ସଂରଚନା xkb_group_action *act = &action->group;
    ଅଚଳ ଗଣନା xkb_action_type type = action->type;

    ଯଦି (field == ACTION_FIELD_GROUP) {
        ଫେରନ୍ତୁ CheckGroupField(keymap_info, action->type, array_ndx, ମୂଲ୍ୟ,
                               ମୂଲ୍ୟ_ptr, &act->flags, &act->group);
    }
    ଯଦି ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        ଫେରନ୍ତୁ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, ମୂଲ୍ୟ,
                                &act->flags);
    ଯଦି (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        ଫେରନ୍ତୁ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, ମୂଲ୍ୟ,
                                &act->flags);
    ଯଦି (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        ଯଦି (keymap_info->features.group_lock_on_release) {
            ଅଚଳ ଗଣନା xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, ମୂଲ୍ୟ, &act->flags
            );
            ଯଦି (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            ଫେରନ୍ତୁ ret;
        } ଅନ୍ୟଥା {
            ଫେରନ୍ତୁ ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    ଫେରନ୍ତୁ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ସ୍ଥିର ଗଣନା xkb_parser_error
HandleMovePtr(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
              ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
              ସଂଘ xkb_action *action, ଗଣନା action_field field,
              ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
              ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ସଂରଚନା xkb_context * restrict ଅଚଳ ctx = keymap_info->keymap.ctx;
    ସଂରଚନା xkb_pointer_motion_action *act = &action->ptr;

    ଯଦି (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        ଅଚଳ bool absolute = (ମୂଲ୍ୟ->common.type != STMT_ପଦସମଷ୍ଟି_NEGATE &&
                               ମୂଲ୍ୟ->common.type != STMT_ପଦସମଷ୍ଟି_UNARY_PLUS);

        ଯଦି (array_ndx)
            ଫେରନ୍ତୁ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer(ctx, ମୂଲ୍ୟ, &val))
            ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        ଯଦି (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        ଯଦି (field == ACTION_FIELD_X) {
            ଯଦି (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        ଅନ୍ୟଥା {
            ଯଦି (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        ଫେରନ୍ତୁ PARSER_SUCCESS;
    }
    ଅନ୍ୟଥା ଯଦି (field == ACTION_FIELD_REPEAT) {
        ଫେରନ୍ତୁ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, ମୂଲ୍ୟ, &act->flags);
    }

    ଫେରନ୍ତୁ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ସ୍ଥିର ଗଣନା xkb_parser_error
HandlePtrBtn(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
             ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
             ସଂଘ xkb_action *action, ଗଣନା action_field field,
             ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
             ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ସଂରଚନା xkb_context * restrict ଅଚଳ ctx = keymap_info->keymap.ctx;
    ସଂରଚନା xkb_pointer_button_action *act = &action->btn;

    ଯଦି (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        ଯଦି (array_ndx)
            ଫେରନ୍ତୁ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Button(ctx, ମୂଲ୍ୟ, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ଯଦି (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        ଫେରନ୍ତୁ PARSER_SUCCESS;
    }
    ଅନ୍ୟଥା ଯଦି (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        ଫେରନ୍ତୁ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, ମୂଲ୍ୟ, &act->flags);
    }
    ଅନ୍ୟଥା ଯଦି (field == ACTION_FIELD_ଗଣନା) {
        int64_t val = 0;

        ଯଦି (array_ndx)
            ଫେରନ୍ତୁ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer(ctx, ମୂଲ୍ୟ, &val))
            ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        ଯଦି (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        ଯଦି (action->type != ACTION_TYPE_PTR_LOCK)
            act->ଗଣନା_2 = (uint8_t) val;
        ଫେରନ୍ତୁ PARSER_SUCCESS;
    }

    ଫେରନ୍ତୁ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ଗଣନା {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    SET_POINTER_DEFAULT_BUTTON_AFFECT_ଅଙ୍କ_ENTRIES
};

ସ୍ଥିର ଅଚଳ LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { ଖାଲି, 0 }
};

ସ୍ଥିର ଗଣନା xkb_parser_error
HandleSetPtrDflt(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                 ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
                 ସଂଘ xkb_action *action, ଗଣନା action_field field,
                 ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
                 ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ସଂରଚନା xkb_context * restrict ଅଚଳ ctx = keymap_info->keymap.ctx;
    ସଂରଚନା xkb_pointer_default_action *act = &action->dflt;

    ଯଦି (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        ଯଦି (array_ndx)
            ଫେରନ୍ତୁ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Enum(ctx, ମୂଲ୍ୟ, &val, ptrDflts))
            ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(SET_POINTER_DEFAULT_BUTTON_AFFECT_ଅଙ୍କ_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        ଫେରନ୍ତୁ PARSER_SUCCESS;
    }
    ଅନ୍ୟଥା ଯଦି (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_ମୂଲ୍ୟ) {
        ଅଚଳ ପଦସମଷ୍ଟି_Def *button;
        int64_t btn = 0;

        ଯଦି (array_ndx)
            ଫେରନ୍ତୁ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ଯଦି (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_NEGATE ||
            ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = ମୂଲ୍ୟ->unary.child;
        }
        ଅନ୍ୟଥା {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = ମୂଲ୍ୟ;
        }

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ଯଦି (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        ଯଦି (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->ମୂଲ୍ୟ = (int8_t) (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_NEGATE ? -btn: btn);
        ଫେରନ୍ତୁ PARSER_SUCCESS;
    }

    ଫେରନ୍ତୁ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ସ୍ଥିର ଗଣନା xkb_parser_error
HandleSwitchScreen(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                   ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
                   ସଂଘ xkb_action *action, ଗଣନା action_field field,
                   ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
                   ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ସଂରଚନା xkb_context * restrict ଅଚଳ ctx = keymap_info->keymap.ctx;
    ସଂରଚନା xkb_switch_screen_action *act = &action->screen;

    ଯଦି (field == ACTION_FIELD_SCREEN) {
        ଅଚଳ ପଦସମଷ୍ଟି_Def *scrn;
        int64_t val = 0;

        ଯଦି (array_ndx)
            ଫେରନ୍ତୁ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ଯଦି (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_NEGATE ||
            ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = ମୂଲ୍ୟ->unary.child;
        }
        ଅନ୍ୟଥା {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = ମୂଲ୍ୟ;
        }

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer(ctx, scrn, &val))
            ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_NEGATE ? -val : val);
        ଯଦି (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        ଫେରନ୍ତୁ PARSER_SUCCESS;
    }
    ଅନ୍ୟଥା ଯଦି (field == ACTION_FIELD_SAME) {
        ଫେରନ୍ତୁ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, ମୂଲ୍ୟ,
                                &act->flags);
    }

    ଫେରନ୍ତୁ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ସ୍ଥିର ଗଣନା xkb_parser_error
HandleSetLockControls(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                      ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
                      ସଂଘ xkb_action *action, ଗଣନା action_field field,
                      ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
                      ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ସଂରଚନା xkb_context * restrict ଅଚଳ ctx = keymap_info->keymap.ctx;
    ସଂରଚନା xkb_controls_action *act = &action->ctrls;

    ଯଦି (field == ACTION_FIELD_CONTROLS) {
        ଯଦି (array_ndx)
            ଫେରନ୍ତୁ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        ଅଚଳ uint8_t offset = keymap_info->features.controls_name_offset;
        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Mask(ctx, ମୂଲ୍ୟ, &mask, ctrlMaskNames + offset))
            ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (ଗଣନା xkb_action_controls) mask;
        ଫେରନ୍ତୁ PARSER_SUCCESS;
    }
    ଅନ୍ୟଥା ଯଦି (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        ଫେରନ୍ତୁ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, ମୂଲ୍ୟ, &act->flags);
    }

    ଫେରନ୍ତୁ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ସ୍ଥିର ଗଣନା xkb_parser_error
Handle_Redirect_ଚାବି(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                  ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
                  ସଂଘ xkb_action *action, ଗଣନା action_field field,
                  ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
                  ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ଅଚଳ ସଂରଚନା xkb_keymap * restrict ଅଚଳ keymap = &keymap_info->keymap;
    ସଂରଚନା xkb_context * restrict ଅଚଳ ctx = keymap->ctx;
    ସଂରଚନା xkb_redirect_ଚାବି_action * ଅଚଳ act = &action->redirect;

    ଯଦି (field == ACTION_FIELD_KEYCODE) {
        ଯଦି (array_ndx)
            ଫେରନ୍ତୁ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        ଯଦି (ମୂଲ୍ୟ->common.type == STMT_ପଦସମଷ୍ଟି_IDENT) {
            ଅଚଳ ଅକ୍ଷର *valStr = xkb_atom_text(ctx, ମୂଲ୍ୟ->ident.ident);
            ଯଦି (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_ଚାବି_auto;
                ଫେରନ୍ତୁ PARSER_SUCCESS;
            }
        }
        ଯଦି (ମୂଲ୍ୟ->common.type != STMT_ପଦସମଷ୍ଟି_KEYNAME_LITERAL) {
            ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        ଅଚଳ ସଂରଚନା xkb_ଚାବି * ଅଚଳ ଚାବି =
            Xkb_ଚାବି_By_Name(keymap, ମୂଲ୍ୟ->ଚାବି_name.ଚାବି_name, true);
        ଯଦି (ଚାବି == ଖାଲି) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), ଚାବି_Name_Text(ctx, ମୂଲ୍ୟ->ଚାବି_name.ଚାବି_name));
            ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_ମୂଲ୍ୟ_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = ଚାବି->keycode;
        ଫେରନ୍ତୁ PARSER_SUCCESS;
    }

    ଯଦି (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        ଗଣନା xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        ଗଣନା xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, ମୂଲ୍ୟ, &flags, &m);
        ଯଦି (r != PARSER_SUCCESS)
            ଫେରନ୍ତୁ r;
        ଯଦି (flags)
            ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        ଯଦି (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        ଅନ୍ୟଥା
            act->mods &= ~m;
        ଫେରନ୍ତୁ PARSER_SUCCESS;
    }

    ଫେରନ୍ତୁ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ସ୍ଥିର ଗଣନା xkb_parser_error
HandleUnsupported(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                        ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
                        ସଂଘ xkb_action *action, ଗଣନା action_field field,
                        ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
                        ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)

{
    /* Do not check any field */
    ଫେରନ୍ତୁ PARSER_SUCCESS;
}

ସ୍ଥିର ଗଣନା xkb_parser_error
HandlePrivate(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
              ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
              ସଂଘ xkb_action *action, ଗଣନା action_field field,
              ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx, ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
              ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr)
{
    ସଂରଚନା xkb_context * restrict ଅଚଳ ctx = keymap_info->keymap.ctx;
    ସଂରଚନା xkb_private_action *act = &action->priv;

    ଯଦି (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        ଯଦି (array_ndx)
            ଫେରନ୍ତୁ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer(ctx, ମୂଲ୍ୟ, &type))
            ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        ଯଦି (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * It's possible for someone to write something like this:
         *      actions = [ Private(type=3,data[0]=1,data[1]=3,data[2]=3) ]
         * where the type refers to some existing action type, e.g. LockMods.
         * This assumes that this action's struct is laid out in memory
         * exactly as described in the XKB specification and libraries.
         * We, however, have changed these structs in various ways, so this
         * assumption is no longer true. Since this is a lousy "feature", we
         * make actions like these no-ops for now.
         */
        ଯଦି (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((ଗଣନା xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        ଅନ୍ୟଥା {
            act->type = (ଗଣନା xkb_action_type) type;
        }

        ଫେରନ୍ତୁ PARSER_SUCCESS;
    }
    ଅନ୍ୟଥା ଯଦି (field == ACTION_FIELD_DATA) {
        ଯଦି (array_ndx == ଖାଲି) {
            xkb_atom_t val = XKB_ATOM_NONE;

            ଯଦି (!ପଦସମଷ୍ଟି_Resolve_String(ctx, ମୂଲ୍ୟ, &val))
                ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            ଅଚଳ ଅକ୍ଷର *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            ଯଦି (len < 1 || len > ଆକାର(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", ଆକାର(act->data), len);
                ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, ଆକାର(act->data));
            memcpy(act->data, str, len);
            ଫେରନ୍ତୁ PARSER_SUCCESS;
        }
        ଅନ୍ୟଥା {
            int64_t ndx = 0, datum = 0;

            ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ଯଦି (ndx < 0 || (size_t) ndx >= ଆକାର(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        ଆକାର(act->data), ndx);
                ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer(ctx, ମୂଲ୍ୟ, &datum))
                ଫେରନ୍ତୁ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            ଯଦି (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            ଫେରନ୍ତୁ PARSER_SUCCESS;
        }
    }

    ଫେରନ୍ତୁ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଗଣନା xkb_parser_error
        (*actionHandler)(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                         ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
                         ସଂଘ xkb_action *action,
                         ଗଣନା action_field field,
                         ଅଚଳ ପଦସମଷ୍ଟି_Def *array_ndx,
                         ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ,
                         ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr);

ସ୍ଥିର ଅଚଳ actionHandler handleAction[ACTION_TYPE_ଅଙ୍କ_ENTRIES] = {
    [ACTION_TYPE_NONE] = HandleNoAction,
    [ACTION_TYPE_VOID] = HandleNoAction,
    [ACTION_TYPE_MOD_SET] = HandleSetLatchLockMods,
    [ACTION_TYPE_MOD_LATCH] = HandleSetLatchLockMods,
    [ACTION_TYPE_MOD_LOCK] = HandleSetLatchLockMods,
    [ACTION_TYPE_GROUP_SET] = HandleSetLatchLockGroup,
    [ACTION_TYPE_GROUP_LATCH] = HandleSetLatchLockGroup,
    [ACTION_TYPE_GROUP_LOCK] = HandleSetLatchLockGroup,
    [ACTION_TYPE_PTR_MOVE] = HandleMovePtr,
    [ACTION_TYPE_PTR_BUTTON] = HandlePtrBtn,
    [ACTION_TYPE_PTR_LOCK] = HandlePtrBtn,
    [ACTION_TYPE_PTR_DEFAULT] = HandleSetPtrDflt,
    [ACTION_TYPE_TERMINATE] = HandleNoAction,
    [ACTION_TYPE_SWITCH_VT] = HandleSwitchScreen,
    [ACTION_TYPE_CTRL_SET] = HandleSetLockControls,
    [ACTION_TYPE_CTRL_LOCK] = HandleSetLockControls,
    [ACTION_TYPE_REDIRECT_ଚାବି] = Handle_Redirect_ଚାବି,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ଅଙ୍କ_ENTRIES,
              "Missing action type");

/***====================================================================***/

ଗଣନା xkb_parser_error
HandleActionDef(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info, ActionsInfo *info,
                ଅଚଳ ସଂରଚନା xkb_mod_set *mods, ପଦସମଷ୍ଟି_Def *def,
                ସଂଘ xkb_action *action)
{
    ସଂରଚନା xkb_context * restrict ଅଚଳ ctx = keymap_info->keymap.ctx;
    ଯଦି (def->common.type != STMT_ପଦସମଷ୍ଟି_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        ଫେରନ୍ତୁ PARSER_FATAL_ERROR;
    }

    ଅଚଳ ଅକ୍ଷର *action_name = xkb_atom_text(ctx, def->action.name);
    ଗଣନା xkb_action_type handler_type;
    ଯଦି (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        ଯଦି (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ଫେରନ୍ତୁ PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    ଯଦି (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
        log_warn(ctx, XKB_WARNING_UNSUPPORTED_LEGACY_ACTION,
                 "Unsupported legacy action type \"%s\".\n", action_name);
        /*
         * Degrade to an uneffective supported action type.
         * Fields will still be processed with the original action type.
         */
        action->type = ACTION_TYPE_NONE;
        /* FIXME: Degrading to NoAction() has the following drawbacks:
         * - It disables overwriting a previous action.
         * - It enables interpretations. */
    }

    /*
     * Now change the action properties as specified for this
     * particular instance, e.g. "modifiers" and "clearLocks" in:
     *     SetMods(modifiers=Alt,clearLocks);
     */
    ଗଣନା xkb_parser_error ret = PARSER_SUCCESS;
    ପାଇଁ (ପଦସମଷ୍ଟି_Def *arg = def->action.args; arg != ଖାଲି;
         arg = (ପଦସମଷ୍ଟି_Def *) arg->common.next) {
        ଅଚଳ ପଦସମଷ୍ଟି_Def *ମୂଲ୍ୟ;
        ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr = ଖାଲି;
        ପଦସମଷ୍ଟି_Def *field, *arrayRtrn;
        ଅଚଳ ଅକ୍ଷର *elemRtrn, *fieldRtrn;

        ଯଦି (arg->common.type == STMT_ପଦସମଷ୍ଟି_ASSIGN) {
            field = arg->ଦ୍ୱିମିକ.ବାମ;
            ମୂଲ୍ୟ = arg->ଦ୍ୱିମିକ.ଡାହାଣ;
            ମୂଲ୍ୟ_ptr = &arg->ଦ୍ୱିମିକ.ଡାହାଣ;
        }
        ଅନ୍ୟଥା ଯଦି (arg->common.type == STMT_ପଦସମଷ୍ଟି_NOT ||
                 arg->common.type == STMT_ପଦସମଷ୍ଟି_INVERT) {
            field = arg->unary.child;
            ମୂଲ୍ୟ = (ଅଚଳ ପଦସମଷ୍ଟି_Def *) &constFalse;
        }
        ଅନ୍ୟଥା {
            field = arg;
            ମୂଲ୍ୟ = (ଅଚଳ ପଦସମଷ୍ଟି_Def *) &constTrue;
        }

        ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Lhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            ଫେରନ୍ତୁ PARSER_FATAL_ERROR;

        ଯଦି (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            ଫେରନ୍ତୁ PARSER_FATAL_ERROR;
        }

        ଗଣନା action_field fieldNdx;
        ଯଦି (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            ଯଦି (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                ଫେରନ୍ତୁ PARSER_FATAL_ERROR;
            } ଅନ୍ୟଥା {
                /* Field is just ignored */
                ଜାରି;
            }
        }

        ବାଛ (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, ମୂଲ୍ୟ, ମୂଲ୍ୟ_ptr)) {
        ସ୍ଥିତି PARSER_FATAL_ERROR:
            ଫେରନ୍ତୁ PARSER_FATAL_ERROR;
        ସ୍ଥିତି PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            ;
        }
    }

    ଫେରନ୍ତୁ (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

ଗଣନା xkb_parser_error
SetDefaultActionField(ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ସଂରଚନା xkb_mod_set *mods,
                      ଅଚଳ ଅକ୍ଷର *elem, ଅଚଳ ଅକ୍ଷର *field, ପଦସମଷ୍ଟି_Def *array_ndx,
                      ପଦସମଷ୍ଟି_Def **ମୂଲ୍ୟ_ptr, ଗଣନା merge_mode merge)
{
    ଅଚଳ ପଦସମଷ୍ଟି_Def * restrict ଅଚଳ ମୂଲ୍ୟ = *ମୂଲ୍ୟ_ptr;
    ଗଣନା xkb_action_type action;
    ଯଦି (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ଗଣନା action_field action_field;
    ଯଦି (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        ଫେରନ୍ତୁ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ସଂଘ xkb_action* ଅଚଳ into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    ସଂଘ xkb_action from = *into;

    /* Parse action */
    ଅଚଳ ଗଣନା xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, ମୂଲ୍ୟ, ମୂଲ୍ୟ_ptr
    );

    /* Success is mandatory to update defaults */
    ଯଦି (ret != PARSER_SUCCESS)
        ଫେରନ୍ତୁ ret;

    /*
     * Merge action with its corresponding default
     *
     * NOTE: Contrary to other items, actions do not have a “defined” field, so
     * we fallback to compare all the actions fields. The drawback is that it
     * overmatches, as even setting an *explicit* default value for the first
     * time (and different from the “factory” default) would *always* display a
     * warning. So we guard the logging with a high verbosity, as best effort
     * mitigation.
     */
    ଯଦି (!action_equal(into, &from)) {
        ଅଚଳ bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        ଯଦି (replace)
            *into = from;
    }
    ଫେରନ୍ତୁ PARSER_SUCCESS;
}
