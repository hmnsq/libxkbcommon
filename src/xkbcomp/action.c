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

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "text.h"
#અન્તર્ભાવ "expr.h"
#અન્તર્ભાવ "xkbcomp/ast.h"
#અન્તર્ભાવ "action.h"

સ્થિર અચલ પદાવલિ_Boolean constTrue = {
    .common = { .type = STMT_પદાવલિ_BOOLEAN_LITERAL, .next = ખાલી },
    .set = true,
};

સ્થિર અચલ પદાવલિ_Boolean constFalse = {
    .common = { .type = STMT_પદાવલિ_BOOLEAN_LITERAL, .next = ખાલી },
    .set = false,
};

ગણના action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_ચાવી_EVENT,
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
    ACTION_FIELD_કિંમત,
    ACTION_FIELD_CONTROLS,
    ACTION_FIELD_TYPE,
    ACTION_FIELD_ગણતરી,
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

શૂન્ય
InitActionsInfo(અચલ સંરચના xkb_keymap *keymap, ActionsInfo *info)
{
    માટે (ગણના xkb_action_type type = 0; type < ACTION_TYPE_અંક_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.કિંમત = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_ચાવી].redirect.keycode =
        keymap->redirect_ચાવી_auto;
}

સ્થિર અચલ LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_ચાવી_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_ચાવી_EVENT   },
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
    { "value",            ACTION_FIELD_કિંમત           },
    { "controls",         ACTION_FIELD_CONTROLS        },
    { "ctrls",            ACTION_FIELD_CONTROLS        },
    { "type",             ACTION_FIELD_TYPE            },
    { "count",            ACTION_FIELD_ગણતરી           },
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
    { ખાલી,               0                            }
};

સ્થિર bool
stringToActionType(અચલ અક્ષર *str, ગણના xkb_action_type *type_rtrn)
{
    unsigned પૂર્ણાંક type = 0;
    અચલ bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (ગણના xkb_action_type) type;
    પાછા ret;
}

સ્થિર bool
stringToField(અચલ અક્ષર *str, ગણના action_field *field_rtrn)
{
    unsigned પૂર્ણાંક field = 0;
    અચલ bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (ગણના action_field) field;
    પાછા ret;
}

સ્થિર અચલ અક્ષર *
fieldText(ગણના action_field field)
{
    પાછા Lookup_કિંમત(fieldStrings, field);
}

/***====================================================================***/

સ્થિર અંતઃરેખ ગણના xkb_parser_error
ReportMismatch(સંરચના xkb_context *ctx, ગણના xkb_message_code code,
               ગણના xkb_action_type action, ગણના action_field field,
               અચલ અક્ષર *type, ગણના xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    પાછા (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

સ્થિર અંતઃરેખ ગણના xkb_parser_error
ReportFormatVersionMismatch(સંરચના xkb_context *ctx,
                            ગણના xkb_action_type action,
                            ગણના action_field field,
                            ગણના xkb_keymap_format format,
                            અચલ અક્ષર *versions,
                            ગણના xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    પાછા (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

સ્થિર અંતઃરેખ ગણના xkb_parser_error
ReportIllegal(સંરચના xkb_context *ctx, ગણના xkb_action_type action,
              ગણના action_field field, ગણના xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    પાછા (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

સ્થિર અંતઃરેખ ગણના xkb_parser_error
ReportActionNotArray(સંરચના xkb_context *ctx, ગણના xkb_action_type action,
                     ગણના action_field field,
                     ગણના xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    પાછા (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

સ્થિર ગણના xkb_parser_error
HandleNoAction(અચલ સંરચના xkb_keymap_info *keymap_info,
               અચલ સંરચના xkb_mod_set *mods,
               સંઘ xkb_action *action, ગણના action_field field,
               અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
               પદાવલિ_Def **કિંમત_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    પાછા (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

સ્થિર ગણના xkb_parser_error
CheckBooleanFlag(સંરચના xkb_context *ctx, ગણના xkb_parser_strict_flags strict,
                 ગણના xkb_action_type action, ગણના action_field field,
                 ગણના xkb_action_flags flag, અચલ પદાવલિ_Def *array_ndx,
                 અચલ પદાવલિ_Def *કિંમત, ગણના xkb_action_flags *flags_inout)
{
    bool set = false;

    જો (array_ndx)
        પાછા ReportActionNotArray(ctx, action, field, strict);

    જો (!પદાવલિ_Resolve_Boolean(ctx, કિંમત, &set))
        પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    જો (set)
        *flags_inout |= flag;
    નહીંતર
        *flags_inout &= ~flag;

    પાછા PARSER_SUCCESS;
}

સ્થિર ગણના xkb_parser_error
CheckModifierField(સંરચના xkb_context *ctx, ગણના xkb_parser_strict_flags strict,
                   અચલ સંરચના xkb_mod_set *mods, ગણના xkb_action_type action,
                   અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
                   ગણના xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    જો (array_ndx)
        પાછા ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    જો (કિંમત->common.type == STMT_પદાવલિ_IDENT) {
        અચલ અક્ષર *valStr;
        valStr = xkb_atom_text(ctx, કિંમત->ident.ident);
        જો (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            પાછા PARSER_SUCCESS;
        }
    }

    જો (!પદાવલિ_Resolve_Mod_Mask(ctx, કિંમત, MOD_BOTH, mods, mods_rtrn))
        પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    પાછા PARSER_SUCCESS;
}

સ્થિર અચલ LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { ખાલી, 0 }
};

સ્થિર ગણના xkb_parser_error
CheckAffectField(સંરચના xkb_context *ctx, ગણના xkb_parser_strict_flags strict,
                 ગણના xkb_action_type action, અચલ પદાવલિ_Def *array_ndx,
                 અચલ પદાવલિ_Def *કિંમત, ગણના xkb_action_flags *flags_inout)
{
    જો (array_ndx)
        પાછા ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    જો (!પદાવલિ_Resolve_Enum(ctx, કિંમત, &flags, lockWhich))
        પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (ગણના xkb_action_flags) flags;
    પાછા PARSER_SUCCESS;
}

સ્થિર ગણના xkb_parser_error
HandleSetLatchLockMods(અચલ સંરચના xkb_keymap_info *keymap_info,
                       અચલ સંરચના xkb_mod_set *mods,
                       સંઘ xkb_action *action, ગણના action_field field,
                       અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
                       પદાવલિ_Def **કિંમત_ptr)
{
    સંરચના xkb_context * restrict અચલ ctx = keymap_info->keymap.ctx;
    સંરચના xkb_mod_action *act = &action->mods;
    અચલ ગણના xkb_action_type type = action->type;

    જો (field == ACTION_FIELD_MODIFIERS)
        પાછા CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, કિંમત, &act->flags, &act->mods.mods);
    જો (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        પ્રતિજ્ઞા(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        જો (keymap_info->features.mods_unlock_on_press) {
            અચલ ગણના xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, કિંમત, &act->flags
            );
            જો (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            પાછા ret;
        } નહીંતર {
            પાછા ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    જો ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        પાછા CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, કિંમત,
                                &act->flags);
    જો (type == ACTION_TYPE_MOD_LATCH) {
        જો (field == ACTION_FIELD_LATCH_TO_LOCK)
            પાછા CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    કિંમત, &act->flags);
        જો (field == ACTION_FIELD_LATCH_ON_PRESS) {
            જો (keymap_info->features.mods_latch_on_press) {
                પાછા CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        કિંમત, &act->flags);
            } નહીંતર {
                પાછા ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    જો (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        પાછા CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, કિંમત, &act->flags);

    પાછા ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

સ્થિર ગણના xkb_parser_error
CheckGroupField(અચલ સંરચના xkb_keymap_info *keymap_info,
                ગણના xkb_action_type action, અચલ પદાવલિ_Def *array_ndx,
                અચલ પદાવલિ_Def *કિંમત, પદાવલિ_Def **કિંમત_ptr,
                ગણના xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    અચલ પદાવલિ_Def *spec;
    xkb_layout_index_t idx = 0;
    ગણના xkb_action_flags flags = *flags_inout;

    જો (array_ndx)
        પાછા ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    જો (કિંમત->common.type == STMT_પદાવલિ_NEGATE ||
        કિંમત->common.type == STMT_પદાવલિ_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = કિંમત->unary.child;
        કિંમત_ptr = &(*કિંમત_ptr)->unary.child;
    } નહીંતર {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = કિંમત;
    }

    અચલ bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    અચલ ગણના xkb_parser_error ret =
        પદાવલિ_Resolve_Group(keymap_info, spec, absolute, &idx, &pending);
    જો (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        પાછા ret;
    }

    જો (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        અચલ darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (સંરચના pending_computation) {
                .પદાવલિ = *કિંમત_ptr,
                .computed = false,
                .કિંમત = 0,
            }
        );
        *કિંમત_ptr = ખાલી;
        static_assert(કદ(pending_index) == કદ(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } નહીંતર {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        જો (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } નહીંતર {
            *group_rtrn = (int32_t) idx;
            જો (કિંમત->common.type == STMT_પદાવલિ_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    પાછા PARSER_SUCCESS;
}

સ્થિર ગણના xkb_parser_error
HandleSetLatchLockGroup(અચલ સંરચના xkb_keymap_info *keymap_info,
                        અચલ સંરચના xkb_mod_set *mods,
                        સંઘ xkb_action *action, ગણના action_field field,
                        અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
                        પદાવલિ_Def **કિંમત_ptr)
{
    સંરચના xkb_context * restrict અચલ ctx = keymap_info->keymap.ctx;
    સંરચના xkb_group_action *act = &action->group;
    અચલ ગણના xkb_action_type type = action->type;

    જો (field == ACTION_FIELD_GROUP) {
        પાછા CheckGroupField(keymap_info, action->type, array_ndx, કિંમત,
                               કિંમત_ptr, &act->flags, &act->group);
    }
    જો ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        પાછા CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, કિંમત,
                                &act->flags);
    જો (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        પાછા CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, કિંમત,
                                &act->flags);
    જો (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        જો (keymap_info->features.group_lock_on_release) {
            અચલ ગણના xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, કિંમત, &act->flags
            );
            જો (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            પાછા ret;
        } નહીંતર {
            પાછા ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    પાછા ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

સ્થિર ગણના xkb_parser_error
HandleMovePtr(અચલ સંરચના xkb_keymap_info *keymap_info,
              અચલ સંરચના xkb_mod_set *mods,
              સંઘ xkb_action *action, ગણના action_field field,
              અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
              પદાવલિ_Def **કિંમત_ptr)
{
    સંરચના xkb_context * restrict અચલ ctx = keymap_info->keymap.ctx;
    સંરચના xkb_pointer_motion_action *act = &action->ptr;

    જો (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        અચલ bool absolute = (કિંમત->common.type != STMT_પદાવલિ_NEGATE &&
                               કિંમત->common.type != STMT_પદાવલિ_UNARY_PLUS);

        જો (array_ndx)
            પાછા ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        જો (!પદાવલિ_Resolve_Integer(ctx, કિંમત, &val))
            પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        જો (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        જો (field == ACTION_FIELD_X) {
            જો (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        નહીંતર {
            જો (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        પાછા PARSER_SUCCESS;
    }
    નહીંતર જો (field == ACTION_FIELD_REPEAT) {
        પાછા CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, કિંમત, &act->flags);
    }

    પાછા ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

સ્થિર ગણના xkb_parser_error
HandlePtrBtn(અચલ સંરચના xkb_keymap_info *keymap_info,
             અચલ સંરચના xkb_mod_set *mods,
             સંઘ xkb_action *action, ગણના action_field field,
             અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
             પદાવલિ_Def **કિંમત_ptr)
{
    સંરચના xkb_context * restrict અચલ ctx = keymap_info->keymap.ctx;
    સંરચના xkb_pointer_button_action *act = &action->btn;

    જો (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        જો (array_ndx)
            પાછા ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        જો (!પદાવલિ_Resolve_Button(ctx, કિંમત, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        જો (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        પાછા PARSER_SUCCESS;
    }
    નહીંતર જો (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        પાછા CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, કિંમત, &act->flags);
    }
    નહીંતર જો (field == ACTION_FIELD_ગણતરી) {
        int64_t val = 0;

        જો (array_ndx)
            પાછા ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        જો (!પદાવલિ_Resolve_Integer(ctx, કિંમત, &val))
            પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        જો (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        જો (action->type != ACTION_TYPE_PTR_LOCK)
            act->ગણતરી = (uint8_t) val;
        પાછા PARSER_SUCCESS;
    }

    પાછા ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ગણના {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    SET_POINTER_DEFAULT_BUTTON_AFFECT_અંક_ENTRIES
};

સ્થિર અચલ LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { ખાલી, 0 }
};

સ્થિર ગણના xkb_parser_error
HandleSetPtrDflt(અચલ સંરચના xkb_keymap_info *keymap_info,
                 અચલ સંરચના xkb_mod_set *mods,
                 સંઘ xkb_action *action, ગણના action_field field,
                 અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
                 પદાવલિ_Def **કિંમત_ptr)
{
    સંરચના xkb_context * restrict અચલ ctx = keymap_info->keymap.ctx;
    સંરચના xkb_pointer_default_action *act = &action->dflt;

    જો (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        જો (array_ndx)
            પાછા ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        જો (!પદાવલિ_Resolve_Enum(ctx, કિંમત, &val, ptrDflts))
            પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(SET_POINTER_DEFAULT_BUTTON_AFFECT_અંક_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        પાછા PARSER_SUCCESS;
    }
    નહીંતર જો (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_કિંમત) {
        અચલ પદાવલિ_Def *button;
        int64_t btn = 0;

        જો (array_ndx)
            પાછા ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        જો (કિંમત->common.type == STMT_પદાવલિ_NEGATE ||
            કિંમત->common.type == STMT_પદાવલિ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = કિંમત->unary.child;
        }
        નહીંતર {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = કિંમત;
        }

        જો (!પદાવલિ_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        જો (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        જો (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->કિંમત = (int8_t) (કિંમત->common.type == STMT_પદાવલિ_NEGATE ? -btn: btn);
        પાછા PARSER_SUCCESS;
    }

    પાછા ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

સ્થિર ગણના xkb_parser_error
HandleSwitchScreen(અચલ સંરચના xkb_keymap_info *keymap_info,
                   અચલ સંરચના xkb_mod_set *mods,
                   સંઘ xkb_action *action, ગણના action_field field,
                   અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
                   પદાવલિ_Def **કિંમત_ptr)
{
    સંરચના xkb_context * restrict અચલ ctx = keymap_info->keymap.ctx;
    સંરચના xkb_switch_screen_action *act = &action->screen;

    જો (field == ACTION_FIELD_SCREEN) {
        અચલ પદાવલિ_Def *scrn;
        int64_t val = 0;

        જો (array_ndx)
            પાછા ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        જો (કિંમત->common.type == STMT_પદાવલિ_NEGATE ||
            કિંમત->common.type == STMT_પદાવલિ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = કિંમત->unary.child;
        }
        નહીંતર {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = કિંમત;
        }

        જો (!પદાવલિ_Resolve_Integer(ctx, scrn, &val))
            પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (કિંમત->common.type == STMT_પદાવલિ_NEGATE ? -val : val);
        જો (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        પાછા PARSER_SUCCESS;
    }
    નહીંતર જો (field == ACTION_FIELD_SAME) {
        પાછા CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, કિંમત,
                                &act->flags);
    }

    પાછા ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

સ્થિર ગણના xkb_parser_error
HandleSetLockControls(અચલ સંરચના xkb_keymap_info *keymap_info,
                      અચલ સંરચના xkb_mod_set *mods,
                      સંઘ xkb_action *action, ગણના action_field field,
                      અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
                      પદાવલિ_Def **કિંમત_ptr)
{
    સંરચના xkb_context * restrict અચલ ctx = keymap_info->keymap.ctx;
    સંરચના xkb_controls_action *act = &action->ctrls;

    જો (field == ACTION_FIELD_CONTROLS) {
        જો (array_ndx)
            પાછા ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        અચલ uint8_t offset = keymap_info->features.controls_name_offset;
        જો (!પદાવલિ_Resolve_Mask(ctx, કિંમત, &mask, ctrlMaskNames + offset))
            પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (ગણના xkb_action_controls) mask;
        પાછા PARSER_SUCCESS;
    }
    નહીંતર જો (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        પાછા CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, કિંમત, &act->flags);
    }

    પાછા ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

સ્થિર ગણના xkb_parser_error
Handle_Redirect_ચાવી(અચલ સંરચના xkb_keymap_info *keymap_info,
                  અચલ સંરચના xkb_mod_set *mods,
                  સંઘ xkb_action *action, ગણના action_field field,
                  અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
                  પદાવલિ_Def **કિંમત_ptr)
{
    અચલ સંરચના xkb_keymap * restrict અચલ keymap = &keymap_info->keymap;
    સંરચના xkb_context * restrict અચલ ctx = keymap->ctx;
    સંરચના xkb_redirect_ચાવી_action * અચલ act = &action->redirect;

    જો (field == ACTION_FIELD_KEYCODE) {
        જો (array_ndx)
            પાછા ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        જો (કિંમત->common.type == STMT_પદાવલિ_IDENT) {
            અચલ અક્ષર *valStr = xkb_atom_text(ctx, કિંમત->ident.ident);
            જો (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_ચાવી_auto;
                પાછા PARSER_SUCCESS;
            }
        }
        જો (કિંમત->common.type != STMT_પદાવલિ_KEYNAME_LITERAL) {
            પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        અચલ સંરચના xkb_ચાવી * અચલ ચાવી =
            Xkb_ચાવી_By_Name(keymap, કિંમત->ચાવી_name.ચાવી_name, true);
        જો (ચાવી == ખાલી) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), ચાવી_Name_Text(ctx, કિંમત->ચાવી_name.ચાવી_name));
            પાછા (keymap_info->strict & PARSER_NO_FIELD_કિંમત_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = ચાવી->keycode;
        પાછા PARSER_SUCCESS;
    }

    જો (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        ગણના xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        ગણના xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, કિંમત, &flags, &m);
        જો (r != PARSER_SUCCESS)
            પાછા r;
        જો (flags)
            પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        જો (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        નહીંતર
            act->mods &= ~m;
        પાછા PARSER_SUCCESS;
    }

    પાછા ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

સ્થિર ગણના xkb_parser_error
HandleUnsupported(અચલ સંરચના xkb_keymap_info *keymap_info,
                        અચલ સંરચના xkb_mod_set *mods,
                        સંઘ xkb_action *action, ગણના action_field field,
                        અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
                        પદાવલિ_Def **કિંમત_ptr)

{
    /* Do not check any field */
    પાછા PARSER_SUCCESS;
}

સ્થિર ગણના xkb_parser_error
HandlePrivate(અચલ સંરચના xkb_keymap_info *keymap_info,
              અચલ સંરચના xkb_mod_set *mods,
              સંઘ xkb_action *action, ગણના action_field field,
              અચલ પદાવલિ_Def *array_ndx, અચલ પદાવલિ_Def *કિંમત,
              પદાવલિ_Def **કિંમત_ptr)
{
    સંરચના xkb_context * restrict અચલ ctx = keymap_info->keymap.ctx;
    સંરચના xkb_private_action *act = &action->priv;

    જો (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        જો (array_ndx)
            પાછા ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        જો (!પદાવલિ_Resolve_Integer(ctx, કિંમત, &type))
            પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        જો (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        જો (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((ગણના xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        નહીંતર {
            act->type = (ગણના xkb_action_type) type;
        }

        પાછા PARSER_SUCCESS;
    }
    નહીંતર જો (field == ACTION_FIELD_DATA) {
        જો (array_ndx == ખાલી) {
            xkb_atom_t val = XKB_ATOM_NONE;

            જો (!પદાવલિ_Resolve_String(ctx, કિંમત, &val))
                પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            અચલ અક્ષર *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            જો (len < 1 || len > કદ(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", કદ(act->data), len);
                પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, કદ(act->data));
            memcpy(act->data, str, len);
            પાછા PARSER_SUCCESS;
        }
        નહીંતર {
            int64_t ndx = 0, datum = 0;

            જો (!પદાવલિ_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            જો (ndx < 0 || (size_t) ndx >= કદ(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        કદ(act->data), ndx);
                પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            જો (!પદાવલિ_Resolve_Integer(ctx, કિંમત, &datum))
                પાછા ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            જો (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                પાછા (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            પાછા PARSER_SUCCESS;
        }
    }

    પાછા ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

પ્રકારવ્યાખ્યા ગણના xkb_parser_error
        (*actionHandler)(અચલ સંરચના xkb_keymap_info *keymap_info,
                         અચલ સંરચના xkb_mod_set *mods,
                         સંઘ xkb_action *action,
                         ગણના action_field field,
                         અચલ પદાવલિ_Def *array_ndx,
                         અચલ પદાવલિ_Def *કિંમત,
                         પદાવલિ_Def **કિંમત_ptr);

સ્થિર અચલ actionHandler handleAction[ACTION_TYPE_અંક_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_ચાવી] = Handle_Redirect_ચાવી,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_અંક_ENTRIES,
              "Missing action type");

/***====================================================================***/

ગણના xkb_parser_error
HandleActionDef(અચલ સંરચના xkb_keymap_info *keymap_info, ActionsInfo *info,
                અચલ સંરચના xkb_mod_set *mods, પદાવલિ_Def *def,
                સંઘ xkb_action *action)
{
    સંરચના xkb_context * restrict અચલ ctx = keymap_info->keymap.ctx;
    જો (def->common.type != STMT_પદાવલિ_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        પાછા PARSER_FATAL_ERROR;
    }

    અચલ અક્ષર *action_name = xkb_atom_text(ctx, def->action.name);
    ગણના xkb_action_type handler_type;
    જો (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        જો (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            પાછા PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    જો (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    ગણના xkb_parser_error ret = PARSER_SUCCESS;
    માટે (પદાવલિ_Def *arg = def->action.args; arg != ખાલી;
         arg = (પદાવલિ_Def *) arg->common.next) {
        અચલ પદાવલિ_Def *કિંમત;
        પદાવલિ_Def **કિંમત_ptr = ખાલી;
        પદાવલિ_Def *field, *arrayRtrn;
        અચલ અક્ષર *elemRtrn, *fieldRtrn;

        જો (arg->common.type == STMT_પદાવલિ_ASSIGN) {
            field = arg->દ્વિઅંકી.ડાબું;
            કિંમત = arg->દ્વિઅંકી.જમણું;
            કિંમત_ptr = &arg->દ્વિઅંકી.જમણું;
        }
        નહીંતર જો (arg->common.type == STMT_પદાવલિ_NOT ||
                 arg->common.type == STMT_પદાવલિ_INVERT) {
            field = arg->unary.child;
            કિંમત = (અચલ પદાવલિ_Def *) &constFalse;
        }
        નહીંતર {
            field = arg;
            કિંમત = (અચલ પદાવલિ_Def *) &constTrue;
        }

        જો (!પદાવલિ_Resolve_Lhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            પાછા PARSER_FATAL_ERROR;

        જો (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            પાછા PARSER_FATAL_ERROR;
        }

        ગણના action_field fieldNdx;
        જો (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            જો (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                પાછા PARSER_FATAL_ERROR;
            } નહીંતર {
                /* Field is just ignored */
                ચાલુ;
            }
        }

        પસંદગી (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, કિંમત, કિંમત_ptr)) {
        સ્થિતિ PARSER_FATAL_ERROR:
            પાછા PARSER_FATAL_ERROR;
        સ્થિતિ PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            તોડો;
        મૂળભૂત:
            ;
        }
    }

    પાછા (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

ગણના xkb_parser_error
SetDefaultActionField(અચલ સંરચના xkb_keymap_info *keymap_info,
                      ActionsInfo *info, સંરચના xkb_mod_set *mods,
                      અચલ અક્ષર *elem, અચલ અક્ષર *field, પદાવલિ_Def *array_ndx,
                      પદાવલિ_Def **કિંમત_ptr, ગણના merge_mode merge)
{
    અચલ પદાવલિ_Def * restrict અચલ કિંમત = *કિંમત_ptr;
    ગણના xkb_action_type action;
    જો (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        પાછા (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ગણના action_field action_field;
    જો (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        પાછા (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    સંઘ xkb_action* અચલ into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    સંઘ xkb_action from = *into;

    /* Parse action */
    અચલ ગણના xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, કિંમત, કિંમત_ptr
    );

    /* Success is mandatory to update defaults */
    જો (ret != PARSER_SUCCESS)
        પાછા ret;

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
    જો (!action_equal(into, &from)) {
        અચલ bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        જો (replace)
            *into = from;
    }
    પાછા PARSER_SUCCESS;
}
