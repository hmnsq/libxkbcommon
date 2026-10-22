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

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "context.h"
#ضمن "darray.h"
#ضمن "keymap.h"
#ضمن "messages-codes.h"
#ضمن "xkbcomp-priv.h"
#ضمن "text.h"
#ضمن "expr.h"
#ضمن "xkbcomp/ast.h"
#ضمن "action.h"

ساكن ثابت ExprBoolean constTrue = {
    .common = { .type = STMT_EXPR_BOOLEAN_LITERAL, .next = عدم },
    .set = true,
};

ساكن ثابت ExprBoolean constFalse = {
    .common = { .type = STMT_EXPR_BOOLEAN_LITERAL, .next = عدم },
    .set = false,
};

تعداد action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_KEY_EVENT,
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
    ACTION_FIELD_VALUE,
    ACTION_FIELD_CONTROLS,
    ACTION_FIELD_TYPE,
    ACTION_FIELD_COUNT,
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

فراغ
InitActionsInfo(ثابت بنية xkb_keymap *keymap, ActionsInfo *info)
{
    لكل (تعداد xkb_action_type type = 0; type < _ACTION_TYPE_NUM_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.value = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_KEY].redirect.keycode =
        keymap->redirect_key_auto;
}

ساكن ثابت LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_KEY_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_KEY_EVENT   },
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
    { "value",            ACTION_FIELD_VALUE           },
    { "controls",         ACTION_FIELD_CONTROLS        },
    { "ctrls",            ACTION_FIELD_CONTROLS        },
    { "type",             ACTION_FIELD_TYPE            },
    { "count",            ACTION_FIELD_COUNT           },
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
    { عدم,               0                            }
};

ساكن bool
stringToActionType(ثابت حرف *str, تعداد xkb_action_type *type_rtrn)
{
    unsigned صحيح type = 0;
    ثابت bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (تعداد xkb_action_type) type;
    ܦܢܐ ret;
}

ساكن bool
stringToField(ثابت حرف *str, تعداد action_field *field_rtrn)
{
    unsigned صحيح field = 0;
    ثابت bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (تعداد action_field) field;
    ܦܢܐ ret;
}

ساكن ثابت حرف *
fieldText(تعداد action_field field)
{
    ܦܢܐ LookupValue(fieldStrings, field);
}

/***====================================================================***/

ساكن مضمن تعداد xkb_parser_error
ReportMismatch(بنية xkb_context *ctx, تعداد xkb_message_code code,
               تعداد xkb_action_type action, تعداد action_field field,
               ثابت حرف *type, تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    ܦܢܐ (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن مضمن تعداد xkb_parser_error
ReportFormatVersionMismatch(بنية xkb_context *ctx,
                            تعداد xkb_action_type action,
                            تعداد action_field field,
                            تعداد xkb_keymap_format format,
                            ثابت حرف *versions,
                            تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    ܦܢܐ (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن مضمن تعداد xkb_parser_error
ReportIllegal(بنية xkb_context *ctx, تعداد xkb_action_type action,
              تعداد action_field field, تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    ܦܢܐ (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن مضمن تعداد xkb_parser_error
ReportActionNotArray(بنية xkb_context *ctx, تعداد xkb_action_type action,
                     تعداد action_field field,
                     تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    ܦܢܐ (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن تعداد xkb_parser_error
HandleNoAction(ثابت بنية xkb_keymap_info *keymap_info,
               ثابت بنية xkb_mod_set *mods,
               اتحاد xkb_action *action, تعداد action_field field,
               ثابت ExprDef *array_ndx, ثابت ExprDef *value,
               ExprDef **value_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    ܦܢܐ (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
CheckBooleanFlag(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, تعداد action_field field,
                 تعداد xkb_action_flags flag, ثابت ExprDef *array_ndx,
                 ثابت ExprDef *value, تعداد xkb_action_flags *flags_inout)
{
    bool set = false;

    ܐܢ (array_ndx)
        ܦܢܐ ReportActionNotArray(ctx, action, field, strict);

    ܐܢ (!ExprResolveBoolean(ctx, value, &set))
        ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    ܐܢ (set)
        *flags_inout |= flag;
    ܐܚܪܢܐ
        *flags_inout &= ~flag;

    ܦܢܐ PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
CheckModifierField(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                   ثابت بنية xkb_mod_set *mods, تعداد xkb_action_type action,
                   ثابت ExprDef *array_ndx, ثابت ExprDef *value,
                   تعداد xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    ܐܢ (array_ndx)
        ܦܢܐ ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    ܐܢ (value->common.type == STMT_EXPR_IDENT) {
        ثابت حرف *valStr;
        valStr = xkb_atom_text(ctx, value->ident.ident);
        ܐܢ (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            ܦܢܐ PARSER_SUCCESS;
        }
    }

    ܐܢ (!ExprResolveModMask(ctx, value, MOD_BOTH, mods, mods_rtrn))
        ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    ܦܢܐ PARSER_SUCCESS;
}

ساكن ثابت LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { عدم, 0 }
};

ساكن تعداد xkb_parser_error
CheckAffectField(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, ثابت ExprDef *array_ndx,
                 ثابت ExprDef *value, تعداد xkb_action_flags *flags_inout)
{
    ܐܢ (array_ndx)
        ܦܢܐ ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    ܐܢ (!ExprResolveEnum(ctx, value, &flags, lockWhich))
        ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (تعداد xkb_action_flags) flags;
    ܦܢܐ PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
HandleSetLatchLockMods(ثابت بنية xkb_keymap_info *keymap_info,
                       ثابت بنية xkb_mod_set *mods,
                       اتحاد xkb_action *action, تعداد action_field field,
                       ثابت ExprDef *array_ndx, ثابت ExprDef *value,
                       ExprDef **value_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_mod_action *act = &action->mods;
    ثابت تعداد xkb_action_type type = action->type;

    ܐܢ (field == ACTION_FIELD_MODIFIERS)
        ܦܢܐ CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, value, &act->flags, &act->mods.mods);
    ܐܢ (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        أكد(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        ܐܢ (keymap_info->features.mods_unlock_on_press) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, value, &act->flags
            );
            ܐܢ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            ܦܢܐ ret;
        } ܐܚܪܢܐ {
            ܦܢܐ ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    ܐܢ ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        ܦܢܐ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, value,
                                &act->flags);
    ܐܢ (type == ACTION_TYPE_MOD_LATCH) {
        ܐܢ (field == ACTION_FIELD_LATCH_TO_LOCK)
            ܦܢܐ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    value, &act->flags);
        ܐܢ (field == ACTION_FIELD_LATCH_ON_PRESS) {
            ܐܢ (keymap_info->features.mods_latch_on_press) {
                ܦܢܐ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        value, &act->flags);
            } ܐܚܪܢܐ {
                ܦܢܐ ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    ܐܢ (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        ܦܢܐ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, value, &act->flags);

    ܦܢܐ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
CheckGroupField(ثابت بنية xkb_keymap_info *keymap_info,
                تعداد xkb_action_type action, ثابت ExprDef *array_ndx,
                ثابت ExprDef *value, ExprDef **value_ptr,
                تعداد xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    ثابت ExprDef *spec;
    xkb_layout_index_t idx = 0;
    تعداد xkb_action_flags flags = *flags_inout;

    ܐܢ (array_ndx)
        ܦܢܐ ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    ܐܢ (value->common.type == STMT_EXPR_NEGATE ||
        value->common.type == STMT_EXPR_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = value->unary.child;
        value_ptr = &(*value_ptr)->unary.child;
    } ܐܚܪܢܐ {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = value;
    }

    ثابت bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ثابت تعداد xkb_parser_error ret =
        ExprResolveGroup(keymap_info, spec, absolute, &idx, &pending);
    ܐܢ (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        ܦܢܐ ret;
    }

    ܐܢ (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ثابت darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (بنية pending_computation) {
                .expr = *value_ptr,
                .computed = false,
                .value = 0,
            }
        );
        *value_ptr = عدم;
        static_assert(حجم(pending_index) == حجم(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } ܐܚܪܢܐ {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        ܐܢ (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } ܐܚܪܢܐ {
            *group_rtrn = (int32_t) idx;
            ܐܢ (value->common.type == STMT_EXPR_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    ܦܢܐ PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
HandleSetLatchLockGroup(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_set *mods,
                        اتحاد xkb_action *action, تعداد action_field field,
                        ثابت ExprDef *array_ndx, ثابت ExprDef *value,
                        ExprDef **value_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_group_action *act = &action->group;
    ثابت تعداد xkb_action_type type = action->type;

    ܐܢ (field == ACTION_FIELD_GROUP) {
        ܦܢܐ CheckGroupField(keymap_info, action->type, array_ndx, value,
                               value_ptr, &act->flags, &act->group);
    }
    ܐܢ ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        ܦܢܐ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, value,
                                &act->flags);
    ܐܢ (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        ܦܢܐ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, value,
                                &act->flags);
    ܐܢ (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        ܐܢ (keymap_info->features.group_lock_on_release) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, value, &act->flags
            );
            ܐܢ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            ܦܢܐ ret;
        } ܐܚܪܢܐ {
            ܦܢܐ ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    ܦܢܐ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleMovePtr(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_set *mods,
              اتحاد xkb_action *action, تعداد action_field field,
              ثابت ExprDef *array_ndx, ثابت ExprDef *value,
              ExprDef **value_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_pointer_motion_action *act = &action->ptr;

    ܐܢ (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        ثابت bool absolute = (value->common.type != STMT_EXPR_NEGATE &&
                               value->common.type != STMT_EXPR_UNARY_PLUS);

        ܐܢ (array_ndx)
            ܦܢܐ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ܐܢ (!ExprResolveInteger(ctx, value, &val))
            ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        ܐܢ (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        ܐܢ (field == ACTION_FIELD_X) {
            ܐܢ (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        ܐܚܪܢܐ {
            ܐܢ (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        ܦܢܐ PARSER_SUCCESS;
    }
    ܐܚܪܢܐ ܐܢ (field == ACTION_FIELD_REPEAT) {
        ܦܢܐ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, value, &act->flags);
    }

    ܦܢܐ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandlePtrBtn(ثابت بنية xkb_keymap_info *keymap_info,
             ثابت بنية xkb_mod_set *mods,
             اتحاد xkb_action *action, تعداد action_field field,
             ثابت ExprDef *array_ndx, ثابت ExprDef *value,
             ExprDef **value_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_pointer_button_action *act = &action->btn;

    ܐܢ (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        ܐܢ (array_ndx)
            ܦܢܐ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ܐܢ (!ExprResolveButton(ctx, value, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ܐܢ (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        ܦܢܐ PARSER_SUCCESS;
    }
    ܐܚܪܢܐ ܐܢ (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        ܦܢܐ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, value, &act->flags);
    }
    ܐܚܪܢܐ ܐܢ (field == ACTION_FIELD_COUNT) {
        int64_t val = 0;

        ܐܢ (array_ndx)
            ܦܢܐ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ܐܢ (!ExprResolveInteger(ctx, value, &val))
            ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        ܐܢ (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        ܐܢ (action->type != ACTION_TYPE_PTR_LOCK)
            act->count = (uint8_t) val;
        ܦܢܐ PARSER_SUCCESS;
    }

    ܦܢܐ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

تعداد {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    _SET_POINTER_DEFAULT_BUTTON_AFFECT_NUM_ENTRIES
};

ساكن ثابت LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { عدم, 0 }
};

ساكن تعداد xkb_parser_error
HandleSetPtrDflt(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت بنية xkb_mod_set *mods,
                 اتحاد xkb_action *action, تعداد action_field field,
                 ثابت ExprDef *array_ndx, ثابت ExprDef *value,
                 ExprDef **value_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_pointer_default_action *act = &action->dflt;

    ܐܢ (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        ܐܢ (array_ndx)
            ܦܢܐ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ܐܢ (!ExprResolveEnum(ctx, value, &val, ptrDflts))
            ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(_SET_POINTER_DEFAULT_BUTTON_AFFECT_NUM_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        ܦܢܐ PARSER_SUCCESS;
    }
    ܐܚܪܢܐ ܐܢ (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_VALUE) {
        ثابت ExprDef *button;
        int64_t btn = 0;

        ܐܢ (array_ndx)
            ܦܢܐ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ܐܢ (value->common.type == STMT_EXPR_NEGATE ||
            value->common.type == STMT_EXPR_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = value->unary.child;
        }
        ܐܚܪܢܐ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = value;
        }

        ܐܢ (!ExprResolveButton(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ܐܢ (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        ܐܢ (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->value = (int8_t) (value->common.type == STMT_EXPR_NEGATE ? -btn: btn);
        ܦܢܐ PARSER_SUCCESS;
    }

    ܦܢܐ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleSwitchScreen(ثابت بنية xkb_keymap_info *keymap_info,
                   ثابت بنية xkb_mod_set *mods,
                   اتحاد xkb_action *action, تعداد action_field field,
                   ثابت ExprDef *array_ndx, ثابت ExprDef *value,
                   ExprDef **value_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_switch_screen_action *act = &action->screen;

    ܐܢ (field == ACTION_FIELD_SCREEN) {
        ثابت ExprDef *scrn;
        int64_t val = 0;

        ܐܢ (array_ndx)
            ܦܢܐ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ܐܢ (value->common.type == STMT_EXPR_NEGATE ||
            value->common.type == STMT_EXPR_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = value->unary.child;
        }
        ܐܚܪܢܐ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = value;
        }

        ܐܢ (!ExprResolveInteger(ctx, scrn, &val))
            ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (value->common.type == STMT_EXPR_NEGATE ? -val : val);
        ܐܢ (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        ܦܢܐ PARSER_SUCCESS;
    }
    ܐܚܪܢܐ ܐܢ (field == ACTION_FIELD_SAME) {
        ܦܢܐ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, value,
                                &act->flags);
    }

    ܦܢܐ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleSetLockControls(ثابت بنية xkb_keymap_info *keymap_info,
                      ثابت بنية xkb_mod_set *mods,
                      اتحاد xkb_action *action, تعداد action_field field,
                      ثابت ExprDef *array_ndx, ثابت ExprDef *value,
                      ExprDef **value_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_controls_action *act = &action->ctrls;

    ܐܢ (field == ACTION_FIELD_CONTROLS) {
        ܐܢ (array_ndx)
            ܦܢܐ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        ثابت uint8_t offset = keymap_info->features.controls_name_offset;
        ܐܢ (!ExprResolveMask(ctx, value, &mask, ctrlMaskNames + offset))
            ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (تعداد xkb_action_controls) mask;
        ܦܢܐ PARSER_SUCCESS;
    }
    ܐܚܪܢܐ ܐܢ (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        ܦܢܐ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, value, &act->flags);
    }

    ܦܢܐ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleRedirectKey(ثابت بنية xkb_keymap_info *keymap_info,
                  ثابت بنية xkb_mod_set *mods,
                  اتحاد xkb_action *action, تعداد action_field field,
                  ثابت ExprDef *array_ndx, ثابت ExprDef *value,
                  ExprDef **value_ptr)
{
    ثابت بنية xkb_keymap * restrict ثابت keymap = &keymap_info->keymap;
    بنية xkb_context * restrict ثابت ctx = keymap->ctx;
    بنية xkb_redirect_key_action * ثابت act = &action->redirect;

    ܐܢ (field == ACTION_FIELD_KEYCODE) {
        ܐܢ (array_ndx)
            ܦܢܐ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        ܐܢ (value->common.type == STMT_EXPR_IDENT) {
            ثابت حرف *valStr = xkb_atom_text(ctx, value->ident.ident);
            ܐܢ (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_key_auto;
                ܦܢܐ PARSER_SUCCESS;
            }
        }
        ܐܢ (value->common.type != STMT_EXPR_KEYNAME_LITERAL) {
            ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        ثابت بنية xkb_key * ثابت key =
            XkbKeyByName(keymap, value->key_name.key_name, true);
        ܐܢ (key == عدم) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), KeyNameText(ctx, value->key_name.key_name));
            ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_VALUE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = key->keycode;
        ܦܢܐ PARSER_SUCCESS;
    }

    ܐܢ (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        تعداد xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        تعداد xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, value, &flags, &m);
        ܐܢ (r != PARSER_SUCCESS)
            ܦܢܐ r;
        ܐܢ (flags)
            ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        ܐܢ (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        ܐܚܪܢܐ
            act->mods &= ~m;
        ܦܢܐ PARSER_SUCCESS;
    }

    ܦܢܐ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleUnsupported(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_set *mods,
                        اتحاد xkb_action *action, تعداد action_field field,
                        ثابت ExprDef *array_ndx, ثابت ExprDef *value,
                        ExprDef **value_ptr)

{
    /* Do not check any field */
    ܦܢܐ PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
HandlePrivate(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_set *mods,
              اتحاد xkb_action *action, تعداد action_field field,
              ثابت ExprDef *array_ndx, ثابت ExprDef *value,
              ExprDef **value_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_private_action *act = &action->priv;

    ܐܢ (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        ܐܢ (array_ndx)
            ܦܢܐ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ܐܢ (!ExprResolveInteger(ctx, value, &type))
            ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        ܐܢ (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        ܐܢ (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((تعداد xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        ܐܚܪܢܐ {
            act->type = (تعداد xkb_action_type) type;
        }

        ܦܢܐ PARSER_SUCCESS;
    }
    ܐܚܪܢܐ ܐܢ (field == ACTION_FIELD_DATA) {
        ܐܢ (array_ndx == عدم) {
            xkb_atom_t val = XKB_ATOM_NONE;

            ܐܢ (!ExprResolveString(ctx, value, &val))
                ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            ثابت حرف *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            ܐܢ (len < 1 || len > حجم(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", حجم(act->data), len);
                ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, حجم(act->data));
            memcpy(act->data, str, len);
            ܦܢܐ PARSER_SUCCESS;
        }
        ܐܚܪܢܐ {
            int64_t ndx = 0, datum = 0;

            ܐܢ (!ExprResolveInteger(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ܐܢ (ndx < 0 || (size_t) ndx >= حجم(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        حجم(act->data), ndx);
                ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ܐܢ (!ExprResolveInteger(ctx, value, &datum))
                ܦܢܐ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            ܐܢ (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                ܦܢܐ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            ܦܢܐ PARSER_SUCCESS;
        }
    }

    ܦܢܐ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

تعريفنوع تعداد xkb_parser_error
        (*actionHandler)(ثابت بنية xkb_keymap_info *keymap_info,
                         ثابت بنية xkb_mod_set *mods,
                         اتحاد xkb_action *action,
                         تعداد action_field field,
                         ثابت ExprDef *array_ndx,
                         ثابت ExprDef *value,
                         ExprDef **value_ptr);

ساكن ثابت actionHandler handleAction[_ACTION_TYPE_NUM_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_KEY] = HandleRedirectKey,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == _ACTION_TYPE_NUM_ENTRIES,
              "Missing action type");

/***====================================================================***/

تعداد xkb_parser_error
HandleActionDef(ثابت بنية xkb_keymap_info *keymap_info, ActionsInfo *info,
                ثابت بنية xkb_mod_set *mods, ExprDef *def,
                اتحاد xkb_action *action)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    ܐܢ (def->common.type != STMT_EXPR_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        ܦܢܐ PARSER_FATAL_ERROR;
    }

    ثابت حرف *action_name = xkb_atom_text(ctx, def->action.name);
    تعداد xkb_action_type handler_type;
    ܐܢ (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        ܐܢ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ܦܢܐ PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    ܐܢ (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    تعداد xkb_parser_error ret = PARSER_SUCCESS;
    لكل (ExprDef *arg = def->action.args; arg != عدم;
         arg = (ExprDef *) arg->common.next) {
        ثابت ExprDef *value;
        ExprDef **value_ptr = عدم;
        ExprDef *field, *arrayRtrn;
        ثابت حرف *elemRtrn, *fieldRtrn;

        ܐܢ (arg->common.type == STMT_EXPR_ASSIGN) {
            field = arg->binary.left;
            value = arg->binary.right;
            value_ptr = &arg->binary.right;
        }
        ܐܚܪܢܐ ܐܢ (arg->common.type == STMT_EXPR_NOT ||
                 arg->common.type == STMT_EXPR_INVERT) {
            field = arg->unary.child;
            value = (ثابت ExprDef *) &constFalse;
        }
        ܐܚܪܢܐ {
            field = arg;
            value = (ثابت ExprDef *) &constTrue;
        }

        ܐܢ (!ExprResolveLhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            ܦܢܐ PARSER_FATAL_ERROR;

        ܐܢ (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            ܦܢܐ PARSER_FATAL_ERROR;
        }

        تعداد action_field fieldNdx;
        ܐܢ (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            ܐܢ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                ܦܢܐ PARSER_FATAL_ERROR;
            } ܐܚܪܢܐ {
                /* Field is just ignored */
                استمر;
            }
        }

        بدل (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, value, value_ptr)) {
        حالة PARSER_FATAL_ERROR:
            ܦܢܐ PARSER_FATAL_ERROR;
        حالة PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            اكسر;
        افتراضي:
            ;
        }
    }

    ܦܢܐ (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

تعداد xkb_parser_error
SetDefaultActionField(ثابت بنية xkb_keymap_info *keymap_info,
                      ActionsInfo *info, بنية xkb_mod_set *mods,
                      ثابت حرف *elem, ثابت حرف *field, ExprDef *array_ndx,
                      ExprDef **value_ptr, تعداد merge_mode merge)
{
    ثابت ExprDef * restrict ثابت value = *value_ptr;
    تعداد xkb_action_type action;
    ܐܢ (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        ܦܢܐ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    تعداد action_field action_field;
    ܐܢ (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        ܦܢܐ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    اتحاد xkb_action* ثابت into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    اتحاد xkb_action from = *into;

    /* Parse action */
    ثابت تعداد xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, value, value_ptr
    );

    /* Success is mandatory to update defaults */
    ܐܢ (ret != PARSER_SUCCESS)
        ܦܢܐ ret;

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
    ܐܢ (!action_equal(into, &from)) {
        ثابت bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        ܐܢ (replace)
            *into = from;
    }
    ܦܢܐ PARSER_SUCCESS;
}
