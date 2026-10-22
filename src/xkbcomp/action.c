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

ساكن ثابت تعبير_Boolean constTrue = {
    .common = { .type = STMT_تعبير_BOOLEAN_LITERAL, .next = عدم },
    .set = true,
};

ساكن ثابت تعبير_Boolean constFalse = {
    .common = { .type = STMT_تعبير_BOOLEAN_LITERAL, .next = عدم },
    .set = false,
};

تعداد action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_مفتاح_EVENT,
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
    ACTION_FIELD_قيمة,
    ACTION_FIELD_CONTROLS,
    ACTION_FIELD_TYPE,
    ACTION_FIELD_عدد,
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
    لكل (تعداد xkb_action_type type = 0; type < ACTION_TYPE_رقم_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.قيمة = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_مفتاح].redirect.keycode =
        keymap->redirect_مفتاح_auto;
}

ساكن ثابت LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_مفتاح_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_مفتاح_EVENT   },
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
    { "value",            ACTION_FIELD_قيمة           },
    { "controls",         ACTION_FIELD_CONTROLS        },
    { "ctrls",            ACTION_FIELD_CONTROLS        },
    { "type",             ACTION_FIELD_TYPE            },
    { "count",            ACTION_FIELD_عدد           },
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
    أرجع ret;
}

ساكن bool
stringToField(ثابت حرف *str, تعداد action_field *field_rtrn)
{
    unsigned صحيح field = 0;
    ثابت bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (تعداد action_field) field;
    أرجع ret;
}

ساكن ثابت حرف *
fieldText(تعداد action_field field)
{
    أرجع Lookup_قيمة(fieldStrings, field);
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
    أرجع (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
    أرجع (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
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
    أرجع (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
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
    أرجع (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن تعداد xkb_parser_error
HandleNoAction(ثابت بنية xkb_keymap_info *keymap_info,
               ثابت بنية xkb_mod_set *mods,
               اتحاد xkb_action *action, تعداد action_field field,
               ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
               تعبير_Def **قيمة_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    أرجع (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
CheckBooleanFlag(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, تعداد action_field field,
                 تعداد xkb_action_flags flag, ثابت تعبير_Def *array_ndx,
                 ثابت تعبير_Def *قيمة, تعداد xkb_action_flags *flags_inout)
{
    bool set = false;

    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, field, strict);

    إذا (!تعبير_Resolve_Boolean(ctx, قيمة, &set))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    إذا (set)
        *flags_inout |= flag;
    وإلا
        *flags_inout &= ~flag;

    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
CheckModifierField(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                   ثابت بنية xkb_mod_set *mods, تعداد xkb_action_type action,
                   ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
                   تعداد xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    إذا (قيمة->common.type == STMT_تعبير_IDENT) {
        ثابت حرف *valStr;
        valStr = xkb_atom_text(ctx, قيمة->ident.ident);
        إذا (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            أرجع PARSER_SUCCESS;
        }
    }

    إذا (!تعبير_Resolve_Mod_Mask(ctx, قيمة, MOD_BOTH, mods, mods_rtrn))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    أرجع PARSER_SUCCESS;
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
                 تعداد xkb_action_type action, ثابت تعبير_Def *array_ndx,
                 ثابت تعبير_Def *قيمة, تعداد xkb_action_flags *flags_inout)
{
    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    إذا (!تعبير_Resolve_Enum(ctx, قيمة, &flags, lockWhich))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (تعداد xkb_action_flags) flags;
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
HandleSetLatchLockMods(ثابت بنية xkb_keymap_info *keymap_info,
                       ثابت بنية xkb_mod_set *mods,
                       اتحاد xkb_action *action, تعداد action_field field,
                       ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
                       تعبير_Def **قيمة_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_mod_action *act = &action->mods;
    ثابت تعداد xkb_action_type type = action->type;

    إذا (field == ACTION_FIELD_MODIFIERS)
        أرجع CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, قيمة, &act->flags, &act->mods.mods);
    إذا (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        أكد(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        إذا (keymap_info->features.mods_unlock_on_press) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, قيمة, &act->flags
            );
            إذا (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            أرجع ret;
        } وإلا {
            أرجع ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    إذا ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, قيمة,
                                &act->flags);
    إذا (type == ACTION_TYPE_MOD_LATCH) {
        إذا (field == ACTION_FIELD_LATCH_TO_LOCK)
            أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    قيمة, &act->flags);
        إذا (field == ACTION_FIELD_LATCH_ON_PRESS) {
            إذا (keymap_info->features.mods_latch_on_press) {
                أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        قيمة, &act->flags);
            } وإلا {
                أرجع ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    إذا (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        أرجع CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, قيمة, &act->flags);

    أرجع ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
CheckGroupField(ثابت بنية xkb_keymap_info *keymap_info,
                تعداد xkb_action_type action, ثابت تعبير_Def *array_ndx,
                ثابت تعبير_Def *قيمة, تعبير_Def **قيمة_ptr,
                تعداد xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    ثابت تعبير_Def *spec;
    xkb_layout_index_t idx = 0;
    تعداد xkb_action_flags flags = *flags_inout;

    إذا (array_ndx)
        أرجع ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    إذا (قيمة->common.type == STMT_تعبير_NEGATE ||
        قيمة->common.type == STMT_تعبير_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = قيمة->unary.child;
        قيمة_ptr = &(*قيمة_ptr)->unary.child;
    } وإلا {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = قيمة;
    }

    ثابت bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ثابت تعداد xkb_parser_error ret =
        تعبير_Resolve_Group(keymap_info, spec, absolute, &idx, &pending);
    إذا (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        أرجع ret;
    }

    إذا (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ثابت darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (بنية pending_computation) {
                .تعبير = *قيمة_ptr,
                .computed = false,
                .قيمة = 0,
            }
        );
        *قيمة_ptr = عدم;
        static_assert(حجم(pending_index) == حجم(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } وإلا {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        إذا (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } وإلا {
            *group_rtrn = (int32_t) idx;
            إذا (قيمة->common.type == STMT_تعبير_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
HandleSetLatchLockGroup(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_set *mods,
                        اتحاد xkb_action *action, تعداد action_field field,
                        ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
                        تعبير_Def **قيمة_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_group_action *act = &action->group;
    ثابت تعداد xkb_action_type type = action->type;

    إذا (field == ACTION_FIELD_GROUP) {
        أرجع CheckGroupField(keymap_info, action->type, array_ndx, قيمة,
                               قيمة_ptr, &act->flags, &act->group);
    }
    إذا ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, قيمة,
                                &act->flags);
    إذا (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, قيمة,
                                &act->flags);
    إذا (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        إذا (keymap_info->features.group_lock_on_release) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, قيمة, &act->flags
            );
            إذا (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            أرجع ret;
        } وإلا {
            أرجع ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    أرجع ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleMovePtr(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_set *mods,
              اتحاد xkb_action *action, تعداد action_field field,
              ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
              تعبير_Def **قيمة_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_pointer_motion_action *act = &action->ptr;

    إذا (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        ثابت bool absolute = (قيمة->common.type != STMT_تعبير_NEGATE &&
                               قيمة->common.type != STMT_تعبير_UNARY_PLUS);

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        إذا (!تعبير_Resolve_Integer(ctx, قيمة, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        إذا (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        إذا (field == ACTION_FIELD_X) {
            إذا (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        وإلا {
            إذا (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (field == ACTION_FIELD_REPEAT) {
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, قيمة, &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandlePtrBtn(ثابت بنية xkb_keymap_info *keymap_info,
             ثابت بنية xkb_mod_set *mods,
             اتحاد xkb_action *action, تعداد action_field field,
             ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
             تعبير_Def **قيمة_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_pointer_button_action *act = &action->btn;

    إذا (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        إذا (!تعبير_Resolve_Button(ctx, قيمة, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        إذا (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        أرجع CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, قيمة, &act->flags);
    }
    وإلا إذا (field == ACTION_FIELD_عدد) {
        int64_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        إذا (!تعبير_Resolve_Integer(ctx, قيمة, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        إذا (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        إذا (action->type != ACTION_TYPE_PTR_LOCK)
            act->عدد = (uint8_t) val;
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

تعداد {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    SET_POINTER_DEFAULT_BUTTON_AFFECT_رقم_ENTRIES
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
                 ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
                 تعبير_Def **قيمة_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_pointer_default_action *act = &action->dflt;

    إذا (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        إذا (!تعبير_Resolve_Enum(ctx, قيمة, &val, ptrDflts))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(SET_POINTER_DEFAULT_BUTTON_AFFECT_رقم_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_قيمة) {
        ثابت تعبير_Def *button;
        int64_t btn = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        إذا (قيمة->common.type == STMT_تعبير_NEGATE ||
            قيمة->common.type == STMT_تعبير_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = قيمة->unary.child;
        }
        وإلا {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = قيمة;
        }

        إذا (!تعبير_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        إذا (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        إذا (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->قيمة = (int8_t) (قيمة->common.type == STMT_تعبير_NEGATE ? -btn: btn);
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleSwitchScreen(ثابت بنية xkb_keymap_info *keymap_info,
                   ثابت بنية xkb_mod_set *mods,
                   اتحاد xkb_action *action, تعداد action_field field,
                   ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
                   تعبير_Def **قيمة_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_switch_screen_action *act = &action->screen;

    إذا (field == ACTION_FIELD_SCREEN) {
        ثابت تعبير_Def *scrn;
        int64_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        إذا (قيمة->common.type == STMT_تعبير_NEGATE ||
            قيمة->common.type == STMT_تعبير_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = قيمة->unary.child;
        }
        وإلا {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = قيمة;
        }

        إذا (!تعبير_Resolve_Integer(ctx, scrn, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (قيمة->common.type == STMT_تعبير_NEGATE ? -val : val);
        إذا (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (field == ACTION_FIELD_SAME) {
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, قيمة,
                                &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleSetLockControls(ثابت بنية xkb_keymap_info *keymap_info,
                      ثابت بنية xkb_mod_set *mods,
                      اتحاد xkb_action *action, تعداد action_field field,
                      ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
                      تعبير_Def **قيمة_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_controls_action *act = &action->ctrls;

    إذا (field == ACTION_FIELD_CONTROLS) {
        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        ثابت uint8_t offset = keymap_info->features.controls_name_offset;
        إذا (!تعبير_Resolve_Mask(ctx, قيمة, &mask, ctrlMaskNames + offset))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (تعداد xkb_action_controls) mask;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        أرجع CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, قيمة, &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Handle_Redirect_مفتاح(ثابت بنية xkb_keymap_info *keymap_info,
                  ثابت بنية xkb_mod_set *mods,
                  اتحاد xkb_action *action, تعداد action_field field,
                  ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
                  تعبير_Def **قيمة_ptr)
{
    ثابت بنية xkb_keymap * restrict ثابت keymap = &keymap_info->keymap;
    بنية xkb_context * restrict ثابت ctx = keymap->ctx;
    بنية xkb_redirect_مفتاح_action * ثابت act = &action->redirect;

    إذا (field == ACTION_FIELD_KEYCODE) {
        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        إذا (قيمة->common.type == STMT_تعبير_IDENT) {
            ثابت حرف *valStr = xkb_atom_text(ctx, قيمة->ident.ident);
            إذا (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_مفتاح_auto;
                أرجع PARSER_SUCCESS;
            }
        }
        إذا (قيمة->common.type != STMT_تعبير_KEYNAME_LITERAL) {
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        ثابت بنية xkb_مفتاح * ثابت مفتاح =
            Xkb_مفتاح_By_Name(keymap, قيمة->مفتاح_name.مفتاح_name, true);
        إذا (مفتاح == عدم) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), مفتاح_Name_Text(ctx, قيمة->مفتاح_name.مفتاح_name));
            أرجع (keymap_info->strict & PARSER_NO_FIELD_قيمة_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = مفتاح->keycode;
        أرجع PARSER_SUCCESS;
    }

    إذا (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        تعداد xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        تعداد xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, قيمة, &flags, &m);
        إذا (r != PARSER_SUCCESS)
            أرجع r;
        إذا (flags)
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        إذا (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        وإلا
            act->mods &= ~m;
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleUnsupported(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_set *mods,
                        اتحاد xkb_action *action, تعداد action_field field,
                        ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
                        تعبير_Def **قيمة_ptr)

{
    /* Do not check any field */
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
HandlePrivate(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_set *mods,
              اتحاد xkb_action *action, تعداد action_field field,
              ثابت تعبير_Def *array_ndx, ثابت تعبير_Def *قيمة,
              تعبير_Def **قيمة_ptr)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_private_action *act = &action->priv;

    إذا (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        إذا (!تعبير_Resolve_Integer(ctx, قيمة, &type))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        إذا (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        إذا (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((تعداد xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        وإلا {
            act->type = (تعداد xkb_action_type) type;
        }

        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (field == ACTION_FIELD_DATA) {
        إذا (array_ndx == عدم) {
            xkb_atom_t val = XKB_ATOM_NONE;

            إذا (!تعبير_Resolve_String(ctx, قيمة, &val))
                أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            ثابت حرف *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            إذا (len < 1 || len > حجم(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", حجم(act->data), len);
                أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, حجم(act->data));
            memcpy(act->data, str, len);
            أرجع PARSER_SUCCESS;
        }
        وإلا {
            int64_t ndx = 0, datum = 0;

            إذا (!تعبير_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            إذا (ndx < 0 || (size_t) ndx >= حجم(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        حجم(act->data), ndx);
                أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            إذا (!تعبير_Resolve_Integer(ctx, قيمة, &datum))
                أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            إذا (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                أرجع (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            أرجع PARSER_SUCCESS;
        }
    }

    أرجع ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

تعريفنوع تعداد xkb_parser_error
        (*actionHandler)(ثابت بنية xkb_keymap_info *keymap_info,
                         ثابت بنية xkb_mod_set *mods,
                         اتحاد xkb_action *action,
                         تعداد action_field field,
                         ثابت تعبير_Def *array_ndx,
                         ثابت تعبير_Def *قيمة,
                         تعبير_Def **قيمة_ptr);

ساكن ثابت actionHandler handleAction[ACTION_TYPE_رقم_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_مفتاح] = Handle_Redirect_مفتاح,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_رقم_ENTRIES,
              "Missing action type");

/***====================================================================***/

تعداد xkb_parser_error
HandleActionDef(ثابت بنية xkb_keymap_info *keymap_info, ActionsInfo *info,
                ثابت بنية xkb_mod_set *mods, تعبير_Def *def,
                اتحاد xkb_action *action)
{
    بنية xkb_context * restrict ثابت ctx = keymap_info->keymap.ctx;
    إذا (def->common.type != STMT_تعبير_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        أرجع PARSER_FATAL_ERROR;
    }

    ثابت حرف *action_name = xkb_atom_text(ctx, def->action.name);
    تعداد xkb_action_type handler_type;
    إذا (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        إذا (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            أرجع PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    إذا (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    لكل (تعبير_Def *arg = def->action.وسائط; arg != عدم;
         arg = (تعبير_Def *) arg->common.next) {
        ثابت تعبير_Def *قيمة;
        تعبير_Def **قيمة_ptr = عدم;
        تعبير_Def *field, *arrayRtrn;
        ثابت حرف *elemRtrn, *fieldRtrn;

        إذا (arg->common.type == STMT_تعبير_ASSIGN) {
            field = arg->ثنائي.يسار;
            قيمة = arg->ثنائي.يمين;
            قيمة_ptr = &arg->ثنائي.يمين;
        }
        وإلا إذا (arg->common.type == STMT_تعبير_NOT ||
                 arg->common.type == STMT_تعبير_INVERT) {
            field = arg->unary.child;
            قيمة = (ثابت تعبير_Def *) &constFalse;
        }
        وإلا {
            field = arg;
            قيمة = (ثابت تعبير_Def *) &constTrue;
        }

        إذا (!تعبير_Resolve_Lhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            أرجع PARSER_FATAL_ERROR;

        إذا (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            أرجع PARSER_FATAL_ERROR;
        }

        تعداد action_field fieldNdx;
        إذا (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            إذا (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                أرجع PARSER_FATAL_ERROR;
            } وإلا {
                /* Field is just ignored */
                استمر;
            }
        }

        بدل (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, قيمة, قيمة_ptr)) {
        حالة PARSER_FATAL_ERROR:
            أرجع PARSER_FATAL_ERROR;
        حالة PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            اكسر;
        افتراضي:
            ;
        }
    }

    أرجع (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

تعداد xkb_parser_error
SetDefaultActionField(ثابت بنية xkb_keymap_info *keymap_info,
                      ActionsInfo *info, بنية xkb_mod_set *mods,
                      ثابت حرف *elem, ثابت حرف *field, تعبير_Def *array_ndx,
                      تعبير_Def **قيمة_ptr, تعداد merge_mode merge)
{
    ثابت تعبير_Def * restrict ثابت قيمة = *قيمة_ptr;
    تعداد xkb_action_type action;
    إذا (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        أرجع (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    تعداد action_field action_field;
    إذا (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        أرجع (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    اتحاد xkb_action* ثابت into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    اتحاد xkb_action from = *into;

    /* Parse action */
    ثابت تعداد xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, قيمة, قيمة_ptr
    );

    /* Success is mandatory to update defaults */
    إذا (ret != PARSER_SUCCESS)
        أرجع ret;

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
    إذا (!action_equal(into, &from)) {
        ثابت bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        إذا (replace)
            *into = from;
    }
    أرجع PARSER_SUCCESS;
}
