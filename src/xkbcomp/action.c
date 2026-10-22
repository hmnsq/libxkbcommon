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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast.h"
#ಅನ್ತರ್ಭಾವ "action.h"

ಸ್ಥಿರ ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Boolean constTrue = {
    .common = { .type = STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL, .next = ರಿಕ್ತ },
    .set = true,
};

ಸ್ಥಿರ ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Boolean constFalse = {
    .common = { .type = STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL, .next = ರಿಕ್ತ },
    .set = false,
};

ಗಣನಾ action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_ಕೀಲಿ_EVENT,
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
    ACTION_FIELD_ಮೌಲ್ಯ,
    ACTION_FIELD_CONTROLS,
    ACTION_FIELD_TYPE,
    ACTION_FIELD_ಎಣಿಕೆ,
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

ಶೂನ್ಯ
InitActionsInfo(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, ActionsInfo *info)
{
    ಪ್ರತಿ (ಗಣನಾ xkb_action_type type = 0; type < ACTION_TYPE_ಅಂಕೆ_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.ಮೌಲ್ಯ = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_ಕೀಲಿ].redirect.keycode =
        keymap->redirect_ಕೀಲಿ_auto;
}

ಸ್ಥಿರ ಧ್ರುವ LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_ಕೀಲಿ_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_ಕೀಲಿ_EVENT   },
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
    { "value",            ACTION_FIELD_ಮೌಲ್ಯ           },
    { "controls",         ACTION_FIELD_CONTROLS        },
    { "ctrls",            ACTION_FIELD_CONTROLS        },
    { "type",             ACTION_FIELD_TYPE            },
    { "count",            ACTION_FIELD_ಎಣಿಕೆ           },
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
    { ರಿಕ್ತ,               0                            }
};

ಸ್ಥಿರ bool
stringToActionType(ಧ್ರುವ ವರ್ಣ *str, ಗಣನಾ xkb_action_type *type_rtrn)
{
    unsigned ಪೂರ್ಣಾಙ್ಕ type = 0;
    ಧ್ರುವ bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (ಗಣನಾ xkb_action_type) type;
    ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ bool
stringToField(ಧ್ರುವ ವರ್ಣ *str, ಗಣನಾ action_field *field_rtrn)
{
    unsigned ಪೂರ್ಣಾಙ್ಕ field = 0;
    ಧ್ರುವ bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (ಗಣನಾ action_field) field;
    ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
fieldText(ಗಣನಾ action_field field)
{
    ಪ್ರತ್ಯಾಗಮ Lookup_ಮೌಲ್ಯ(fieldStrings, field);
}

/***====================================================================***/

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ xkb_parser_error
ReportMismatch(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_message_code code,
               ಗಣನಾ xkb_action_type action, ಗಣನಾ action_field field,
               ಧ್ರುವ ವರ್ಣ *type, ಗಣನಾ xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    ಪ್ರತ್ಯಾಗಮ (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ xkb_parser_error
ReportFormatVersionMismatch(ಸಂರಚನಾ xkb_context *ctx,
                            ಗಣನಾ xkb_action_type action,
                            ಗಣನಾ action_field field,
                            ಗಣನಾ xkb_keymap_format format,
                            ಧ್ರುವ ವರ್ಣ *versions,
                            ಗಣನಾ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    ಪ್ರತ್ಯಾಗಮ (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ xkb_parser_error
ReportIllegal(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_action_type action,
              ಗಣನಾ action_field field, ಗಣನಾ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    ಪ್ರತ್ಯಾಗಮ (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಗಣನಾ xkb_parser_error
ReportActionNotArray(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_action_type action,
                     ಗಣನಾ action_field field,
                     ಗಣನಾ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    ಪ್ರತ್ಯಾಗಮ (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
HandleNoAction(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
               ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
               ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
               ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
               ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
CheckBooleanFlag(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_parser_strict_flags strict,
                 ಗಣನಾ xkb_action_type action, ಗಣನಾ action_field field,
                 ಗಣನಾ xkb_action_flags flag, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                 ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ, ಗಣನಾ xkb_action_flags *flags_inout)
{
    bool set = false;

    ಯದಿ (array_ndx)
        ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action, field, strict);

    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(ctx, ಮೌಲ್ಯ, &set))
        ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    ಯದಿ (set)
        *flags_inout |= flag;
    ಅನ್ಯಥಾ
        *flags_inout &= ~flag;

    ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
CheckModifierField(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_parser_strict_flags strict,
                   ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods, ಗಣನಾ xkb_action_type action,
                   ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                   ಗಣನಾ xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    ಯದಿ (array_ndx)
        ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    ಯದಿ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
        ಧ್ರುವ ವರ್ಣ *valStr;
        valStr = xkb_atom_text(ctx, ಮೌಲ್ಯ->ident.ident);
        ಯದಿ (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
        }
    }

    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(ctx, ಮೌಲ್ಯ, MOD_BOTH, mods, mods_rtrn))
        ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
}

ಸ್ಥಿರ ಧ್ರುವ LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { ರಿಕ್ತ, 0 }
};

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
CheckAffectField(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_parser_strict_flags strict,
                 ಗಣನಾ xkb_action_type action, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                 ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ, ಗಣನಾ xkb_action_flags *flags_inout)
{
    ಯದಿ (array_ndx)
        ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(ctx, ಮೌಲ್ಯ, &flags, lockWhich))
        ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (ಗಣನಾ xkb_action_flags) flags;
    ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
HandleSetLatchLockMods(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                       ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
                       ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
                       ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                       ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಸಂರಚನಾ xkb_context * restrict ಧ್ರುವ ctx = keymap_info->keymap.ctx;
    ಸಂರಚನಾ xkb_mod_action *act = &action->mods;
    ಧ್ರುವ ಗಣನಾ xkb_action_type type = action->type;

    ಯದಿ (field == ACTION_FIELD_MODIFIERS)
        ಪ್ರತ್ಯಾಗಮ CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, ಮೌಲ್ಯ, &act->flags, &act->mods.mods);
    ಯದಿ (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        ಪ್ರತಿಜ್ಞಾ(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        ಯದಿ (keymap_info->features.mods_unlock_on_press) {
            ಧ್ರುವ ಗಣನಾ xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, ಮೌಲ್ಯ, &act->flags
            );
            ಯದಿ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            ಪ್ರತ್ಯಾಗಮ ret;
        } ಅನ್ಯಥಾ {
            ಪ್ರತ್ಯಾಗಮ ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    ಯದಿ ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        ಪ್ರತ್ಯಾಗಮ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, ಮೌಲ್ಯ,
                                &act->flags);
    ಯದಿ (type == ACTION_TYPE_MOD_LATCH) {
        ಯದಿ (field == ACTION_FIELD_LATCH_TO_LOCK)
            ಪ್ರತ್ಯಾಗಮ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    ಮೌಲ್ಯ, &act->flags);
        ಯದಿ (field == ACTION_FIELD_LATCH_ON_PRESS) {
            ಯದಿ (keymap_info->features.mods_latch_on_press) {
                ಪ್ರತ್ಯಾಗಮ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        ಮೌಲ್ಯ, &act->flags);
            } ಅನ್ಯಥಾ {
                ಪ್ರತ್ಯಾಗಮ ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    ಯದಿ (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        ಪ್ರತ್ಯಾಗಮ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, ಮೌಲ್ಯ, &act->flags);

    ಪ್ರತ್ಯಾಗಮ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
CheckGroupField(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                ಗಣನಾ xkb_action_type action, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ, ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr,
                ಗಣನಾ xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *spec;
    xkb_layout_index_t idx = 0;
    ಗಣನಾ xkb_action_flags flags = *flags_inout;

    ಯದಿ (array_ndx)
        ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    ಯದಿ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ||
        ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = ಮೌಲ್ಯ->unary.child;
        ಮೌಲ್ಯ_ptr = &(*ಮೌಲ್ಯ_ptr)->unary.child;
    } ಅನ್ಯಥಾ {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = ಮೌಲ್ಯ;
    }

    ಧ್ರುವ bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ಧ್ರುವ ಗಣನಾ xkb_parser_error ret =
        ಅಭಿವ್ಯಕ್ತಿ_Resolve_Group(keymap_info, spec, absolute, &idx, &pending);
    ಯದಿ (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        ಪ್ರತ್ಯಾಗಮ ret;
    }

    ಯದಿ (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ಧ್ರುವ darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (ಸಂರಚನಾ pending_computation) {
                .ಅಭಿವ್ಯಕ್ತಿ = *ಮೌಲ್ಯ_ptr,
                .computed = false,
                .ಮೌಲ್ಯ = 0,
            }
        );
        *ಮೌಲ್ಯ_ptr = ರಿಕ್ತ;
        static_assert(ಪರಿಮಾಣ(pending_index) == ಪರಿಮಾಣ(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } ಅನ್ಯಥಾ {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        ಯದಿ (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } ಅನ್ಯಥಾ {
            *group_rtrn = (int32_t) idx;
            ಯದಿ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
HandleSetLatchLockGroup(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                        ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
                        ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
                        ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                        ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಸಂರಚನಾ xkb_context * restrict ಧ್ರುವ ctx = keymap_info->keymap.ctx;
    ಸಂರಚನಾ xkb_group_action *act = &action->group;
    ಧ್ರುವ ಗಣನಾ xkb_action_type type = action->type;

    ಯದಿ (field == ACTION_FIELD_GROUP) {
        ಪ್ರತ್ಯಾಗಮ CheckGroupField(keymap_info, action->type, array_ndx, ಮೌಲ್ಯ,
                               ಮೌಲ್ಯ_ptr, &act->flags, &act->group);
    }
    ಯದಿ ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        ಪ್ರತ್ಯಾಗಮ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, ಮೌಲ್ಯ,
                                &act->flags);
    ಯದಿ (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        ಪ್ರತ್ಯಾಗಮ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, ಮೌಲ್ಯ,
                                &act->flags);
    ಯದಿ (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        ಯದಿ (keymap_info->features.group_lock_on_release) {
            ಧ್ರುವ ಗಣನಾ xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, ಮೌಲ್ಯ, &act->flags
            );
            ಯದಿ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            ಪ್ರತ್ಯಾಗಮ ret;
        } ಅನ್ಯಥಾ {
            ಪ್ರತ್ಯಾಗಮ ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    ಪ್ರತ್ಯಾಗಮ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
HandleMovePtr(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
              ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
              ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
              ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
              ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಸಂರಚನಾ xkb_context * restrict ಧ್ರುವ ctx = keymap_info->keymap.ctx;
    ಸಂರಚನಾ xkb_pointer_motion_action *act = &action->ptr;

    ಯದಿ (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        ಧ್ರುವ bool absolute = (ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE &&
                               ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS);

        ಯದಿ (array_ndx)
            ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, ಮೌಲ್ಯ, &val))
            ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        ಯದಿ (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        ಯದಿ (field == ACTION_FIELD_X) {
            ಯದಿ (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        ಅನ್ಯಥಾ {
            ಯದಿ (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
    }
    ಅನ್ಯಥಾ ಯದಿ (field == ACTION_FIELD_REPEAT) {
        ಪ್ರತ್ಯಾಗಮ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, ಮೌಲ್ಯ, &act->flags);
    }

    ಪ್ರತ್ಯಾಗಮ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
HandlePtrBtn(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
             ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
             ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
             ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
             ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಸಂರಚನಾ xkb_context * restrict ಧ್ರುವ ctx = keymap_info->keymap.ctx;
    ಸಂರಚನಾ xkb_pointer_button_action *act = &action->btn;

    ಯದಿ (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        ಯದಿ (array_ndx)
            ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Button(ctx, ಮೌಲ್ಯ, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ಯದಿ (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
    }
    ಅನ್ಯಥಾ ಯದಿ (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        ಪ್ರತ್ಯಾಗಮ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, ಮೌಲ್ಯ, &act->flags);
    }
    ಅನ್ಯಥಾ ಯದಿ (field == ACTION_FIELD_ಎಣಿಕೆ) {
        int64_t val = 0;

        ಯದಿ (array_ndx)
            ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, ಮೌಲ್ಯ, &val))
            ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        ಯದಿ (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        ಯದಿ (action->type != ACTION_TYPE_PTR_LOCK)
            act->ಎಣಿಕೆ = (uint8_t) val;
        ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
    }

    ಪ್ರತ್ಯಾಗಮ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ಗಣನಾ {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    SET_POINTER_DEFAULT_BUTTON_AFFECT_ಅಂಕೆ_ENTRIES
};

ಸ್ಥಿರ ಧ್ರುವ LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { ರಿಕ್ತ, 0 }
};

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
HandleSetPtrDflt(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                 ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
                 ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
                 ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                 ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಸಂರಚನಾ xkb_context * restrict ಧ್ರುವ ctx = keymap_info->keymap.ctx;
    ಸಂರಚನಾ xkb_pointer_default_action *act = &action->dflt;

    ಯದಿ (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        ಯದಿ (array_ndx)
            ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(ctx, ಮೌಲ್ಯ, &val, ptrDflts))
            ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(SET_POINTER_DEFAULT_BUTTON_AFFECT_ಅಂಕೆ_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
    }
    ಅನ್ಯಥಾ ಯದಿ (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_ಮೌಲ್ಯ) {
        ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *button;
        int64_t btn = 0;

        ಯದಿ (array_ndx)
            ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ಯದಿ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ||
            ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = ಮೌಲ್ಯ->unary.child;
        }
        ಅನ್ಯಥಾ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = ಮೌಲ್ಯ;
        }

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ಯದಿ (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        ಯದಿ (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->ಮೌಲ್ಯ = (int8_t) (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ? -btn: btn);
        ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
    }

    ಪ್ರತ್ಯಾಗಮ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
HandleSwitchScreen(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                   ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
                   ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
                   ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                   ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಸಂರಚನಾ xkb_context * restrict ಧ್ರುವ ctx = keymap_info->keymap.ctx;
    ಸಂರಚನಾ xkb_switch_screen_action *act = &action->screen;

    ಯದಿ (field == ACTION_FIELD_SCREEN) {
        ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *scrn;
        int64_t val = 0;

        ಯದಿ (array_ndx)
            ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ಯದಿ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ||
            ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = ಮೌಲ್ಯ->unary.child;
        }
        ಅನ್ಯಥಾ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = ಮೌಲ್ಯ;
        }

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, scrn, &val))
            ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ? -val : val);
        ಯದಿ (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
    }
    ಅನ್ಯಥಾ ಯದಿ (field == ACTION_FIELD_SAME) {
        ಪ್ರತ್ಯಾಗಮ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, ಮೌಲ್ಯ,
                                &act->flags);
    }

    ಪ್ರತ್ಯಾಗಮ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
HandleSetLockControls(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                      ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
                      ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
                      ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                      ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಸಂರಚನಾ xkb_context * restrict ಧ್ರುವ ctx = keymap_info->keymap.ctx;
    ಸಂರಚನಾ xkb_controls_action *act = &action->ctrls;

    ಯದಿ (field == ACTION_FIELD_CONTROLS) {
        ಯದಿ (array_ndx)
            ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        ಧ್ರುವ uint8_t offset = keymap_info->features.controls_name_offset;
        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mask(ctx, ಮೌಲ್ಯ, &mask, ctrlMaskNames + offset))
            ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (ಗಣನಾ xkb_action_controls) mask;
        ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
    }
    ಅನ್ಯಥಾ ಯದಿ (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        ಪ್ರತ್ಯಾಗಮ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, ಮೌಲ್ಯ, &act->flags);
    }

    ಪ್ರತ್ಯಾಗಮ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
Handle_Redirect_ಕೀಲಿ(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                  ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
                  ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
                  ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                  ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap * restrict ಧ್ರುವ keymap = &keymap_info->keymap;
    ಸಂರಚನಾ xkb_context * restrict ಧ್ರುವ ctx = keymap->ctx;
    ಸಂರಚನಾ xkb_redirect_ಕೀಲಿ_action * ಧ್ರುವ act = &action->redirect;

    ಯದಿ (field == ACTION_FIELD_KEYCODE) {
        ಯದಿ (array_ndx)
            ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        ಯದಿ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
            ಧ್ರುವ ವರ್ಣ *valStr = xkb_atom_text(ctx, ಮೌಲ್ಯ->ident.ident);
            ಯದಿ (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_ಕೀಲಿ_auto;
                ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
            }
        }
        ಯದಿ (ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL) {
            ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಧ್ರುವ ಕೀಲಿ =
            Xkb_ಕೀಲಿ_By_Name(keymap, ಮೌಲ್ಯ->ಕೀಲಿ_name.ಕೀಲಿ_name, true);
        ಯದಿ (ಕೀಲಿ == ರಿಕ್ತ) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), ಕೀಲಿ_Name_Text(ctx, ಮೌಲ್ಯ->ಕೀಲಿ_name.ಕೀಲಿ_name));
            ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_ಮೌಲ್ಯ_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = ಕೀಲಿ->keycode;
        ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
    }

    ಯದಿ (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        ಗಣನಾ xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        ಗಣನಾ xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, ಮೌಲ್ಯ, &flags, &m);
        ಯದಿ (r != PARSER_SUCCESS)
            ಪ್ರತ್ಯಾಗಮ r;
        ಯದಿ (flags)
            ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        ಯದಿ (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        ಅನ್ಯಥಾ
            act->mods &= ~m;
        ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
    }

    ಪ್ರತ್ಯಾಗಮ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
HandleUnsupported(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                        ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
                        ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
                        ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                        ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)

{
    /* Do not check any field */
    ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
}

ಸ್ಥಿರ ಗಣನಾ xkb_parser_error
HandlePrivate(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
              ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
              ಸಙ್ಘ xkb_action *action, ಗಣನಾ action_field field,
              ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
              ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಸಂರಚನಾ xkb_context * restrict ಧ್ರುವ ctx = keymap_info->keymap.ctx;
    ಸಂರಚನಾ xkb_private_action *act = &action->priv;

    ಯದಿ (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        ಯದಿ (array_ndx)
            ಪ್ರತ್ಯಾಗಮ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, ಮೌಲ್ಯ, &type))
            ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        ಯದಿ (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        ಯದಿ (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((ಗಣನಾ xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        ಅನ್ಯಥಾ {
            act->type = (ಗಣನಾ xkb_action_type) type;
        }

        ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
    }
    ಅನ್ಯಥಾ ಯದಿ (field == ACTION_FIELD_DATA) {
        ಯದಿ (array_ndx == ರಿಕ್ತ) {
            xkb_atom_t val = XKB_ATOM_NONE;

            ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(ctx, ಮೌಲ್ಯ, &val))
                ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            ಧ್ರುವ ವರ್ಣ *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            ಯದಿ (len < 1 || len > ಪರಿಮಾಣ(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", ಪರಿಮಾಣ(act->data), len);
                ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, ಪರಿಮಾಣ(act->data));
            memcpy(act->data, str, len);
            ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
        }
        ಅನ್ಯಥಾ {
            int64_t ndx = 0, datum = 0;

            ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ಯದಿ (ndx < 0 || (size_t) ndx >= ಪರಿಮಾಣ(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        ಪರಿಮಾಣ(act->data), ndx);
                ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, ಮೌಲ್ಯ, &datum))
                ಪ್ರತ್ಯಾಗಮ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            ಯದಿ (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
        }
    }

    ಪ್ರತ್ಯಾಗಮ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಗಣನಾ xkb_parser_error
        (*actionHandler)(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                         ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
                         ಸಙ್ಘ xkb_action *action,
                         ಗಣನಾ action_field field,
                         ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                         ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                         ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr);

ಸ್ಥಿರ ಧ್ರುವ actionHandler handleAction[ACTION_TYPE_ಅಂಕೆ_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_ಕೀಲಿ] = Handle_Redirect_ಕೀಲಿ,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
              "Missing action type");

/***====================================================================***/

ಗಣನಾ xkb_parser_error
HandleActionDef(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info, ActionsInfo *info,
                ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods, ಅಭಿವ್ಯಕ್ತಿ_Def *def,
                ಸಙ್ಘ xkb_action *action)
{
    ಸಂರಚನಾ xkb_context * restrict ಧ್ರುವ ctx = keymap_info->keymap.ctx;
    ಯದಿ (def->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        ಪ್ರತ್ಯಾಗಮ PARSER_FATAL_ERROR;
    }

    ಧ್ರುವ ವರ್ಣ *action_name = xkb_atom_text(ctx, def->action.name);
    ಗಣನಾ xkb_action_type handler_type;
    ಯದಿ (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        ಯದಿ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ಪ್ರತ್ಯಾಗಮ PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    ಯದಿ (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    ಗಣನಾ xkb_parser_error ret = PARSER_SUCCESS;
    ಪ್ರತಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *arg = def->action.args; arg != ರಿಕ್ತ;
         arg = (ಅಭಿವ್ಯಕ್ತಿ_Def *) arg->common.next) {
        ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ;
        ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr = ರಿಕ್ತ;
        ಅಭಿವ್ಯಕ್ತಿ_Def *field, *arrayRtrn;
        ಧ್ರುವ ವರ್ಣ *elemRtrn, *fieldRtrn;

        ಯದಿ (arg->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_ASSIGN) {
            field = arg->ದ್ವಿಮಾನ.ಎಡ;
            ಮೌಲ್ಯ = arg->ದ್ವಿಮಾನ.ಬಲ;
            ಮೌಲ್ಯ_ptr = &arg->ದ್ವಿಮಾನ.ಬಲ;
        }
        ಅನ್ಯಥಾ ಯದಿ (arg->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT ||
                 arg->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT) {
            field = arg->unary.child;
            ಮೌಲ್ಯ = (ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *) &constFalse;
        }
        ಅನ್ಯಥಾ {
            field = arg;
            ಮೌಲ್ಯ = (ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def *) &constTrue;
        }

        ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            ಪ್ರತ್ಯಾಗಮ PARSER_FATAL_ERROR;

        ಯದಿ (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            ಪ್ರತ್ಯಾಗಮ PARSER_FATAL_ERROR;
        }

        ಗಣನಾ action_field fieldNdx;
        ಯದಿ (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            ಯದಿ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                ಪ್ರತ್ಯಾಗಮ PARSER_FATAL_ERROR;
            } ಅನ್ಯಥಾ {
                /* Field is just ignored */
                ಅನುವರ್ತ;
            }
        }

        ವಿಕಲ್ಪ (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, ಮೌಲ್ಯ, ಮೌಲ್ಯ_ptr)) {
        ಪಕ್ಷ PARSER_FATAL_ERROR:
            ಪ್ರತ್ಯಾಗಮ PARSER_FATAL_ERROR;
        ಪಕ್ಷ PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            ;
        }
    }

    ಪ್ರತ್ಯಾಗಮ (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

ಗಣನಾ xkb_parser_error
SetDefaultActionField(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ಸಂರಚನಾ xkb_mod_set *mods,
                      ಧ್ರುವ ವರ್ಣ *elem, ಧ್ರುವ ವರ್ಣ *field, ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                      ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr, ಗಣನಾ merge_mode merge)
{
    ಧ್ರುವ ಅಭಿವ್ಯಕ್ತಿ_Def * restrict ಧ್ರುವ ಮೌಲ್ಯ = *ಮೌಲ್ಯ_ptr;
    ಗಣನಾ xkb_action_type action;
    ಯದಿ (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ಗಣನಾ action_field action_field;
    ಯದಿ (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        ಪ್ರತ್ಯಾಗಮ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ಸಙ್ಘ xkb_action* ಧ್ರುವ into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    ಸಙ್ಘ xkb_action from = *into;

    /* Parse action */
    ಧ್ರುವ ಗಣನಾ xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, ಮೌಲ್ಯ, ಮೌಲ್ಯ_ptr
    );

    /* Success is mandatory to update defaults */
    ಯದಿ (ret != PARSER_SUCCESS)
        ಪ್ರತ್ಯಾಗಮ ret;

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
    ಯದಿ (!action_equal(into, &from)) {
        ಧ್ರುವ bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        ಯದಿ (replace)
            *into = from;
    }
    ಪ್ರತ್ಯಾಗಮ PARSER_SUCCESS;
}
