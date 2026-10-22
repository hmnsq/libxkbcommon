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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "expr.h"
#അന്തര്ഭാവ "xkbcomp/ast.h"
#അന്തര്ഭാവ "action.h"

സ്ഥിതം സ്ഥിരം വ്യഞ്ജകം_Boolean constTrue = {
    .common = { .type = STMT_വ്യഞ്ജകം_BOOLEAN_LITERAL, .next = ശൂന്യ },
    .set = true,
};

സ്ഥിതം സ്ഥിരം വ്യഞ്ജകം_Boolean constFalse = {
    .common = { .type = STMT_വ്യഞ്ജകം_BOOLEAN_LITERAL, .next = ശൂന്യ },
    .set = false,
};

എണ്ണൽ action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_താക്കോൽ_EVENT,
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
    ACTION_FIELD_മൂല്യം,
    ACTION_FIELD_CONTROLS,
    ACTION_FIELD_TYPE,
    ACTION_FIELD_എണ്ണം,
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

ശൂന്യം
InitActionsInfo(സ്ഥിരം ഘടന xkb_keymap *keymap, ActionsInfo *info)
{
    വേണ്ടി (എണ്ണൽ xkb_action_type type = 0; type < ACTION_TYPE_അക്കം_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.മൂല്യം = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_താക്കോൽ].redirect.keycode =
        keymap->redirect_താക്കോൽ_auto;
}

സ്ഥിതം സ്ഥിരം LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_താക്കോൽ_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_താക്കോൽ_EVENT   },
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
    { "value",            ACTION_FIELD_മൂല്യം           },
    { "controls",         ACTION_FIELD_CONTROLS        },
    { "ctrls",            ACTION_FIELD_CONTROLS        },
    { "type",             ACTION_FIELD_TYPE            },
    { "count",            ACTION_FIELD_എണ്ണം           },
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
    { ശൂന്യ,               0                            }
};

സ്ഥിതം bool
stringToActionType(സ്ഥിരം അക്ഷരം *str, എണ്ണൽ xkb_action_type *type_rtrn)
{
    unsigned പൂർണ്ണസംഖ്യ type = 0;
    സ്ഥിരം bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (എണ്ണൽ xkb_action_type) type;
    തിരികെ ret;
}

സ്ഥിതം bool
stringToField(സ്ഥിരം അക്ഷരം *str, എണ്ണൽ action_field *field_rtrn)
{
    unsigned പൂർണ്ണസംഖ്യ field = 0;
    സ്ഥിരം bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (എണ്ണൽ action_field) field;
    തിരികെ ret;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
fieldText(എണ്ണൽ action_field field)
{
    തിരികെ Lookup_മൂല്യം(fieldStrings, field);
}

/***====================================================================***/

സ്ഥിതം വരിയിൽ എണ്ണൽ xkb_parser_error
ReportMismatch(ഘടന xkb_context *ctx, എണ്ണൽ xkb_message_code code,
               എണ്ണൽ xkb_action_type action, എണ്ണൽ action_field field,
               സ്ഥിരം അക്ഷരം *type, എണ്ണൽ xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    തിരികെ (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

സ്ഥിതം വരിയിൽ എണ്ണൽ xkb_parser_error
ReportFormatVersionMismatch(ഘടന xkb_context *ctx,
                            എണ്ണൽ xkb_action_type action,
                            എണ്ണൽ action_field field,
                            എണ്ണൽ xkb_keymap_format format,
                            സ്ഥിരം അക്ഷരം *versions,
                            എണ്ണൽ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    തിരികെ (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

സ്ഥിതം വരിയിൽ എണ്ണൽ xkb_parser_error
ReportIllegal(ഘടന xkb_context *ctx, എണ്ണൽ xkb_action_type action,
              എണ്ണൽ action_field field, എണ്ണൽ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    തിരികെ (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

സ്ഥിതം വരിയിൽ എണ്ണൽ xkb_parser_error
ReportActionNotArray(ഘടന xkb_context *ctx, എണ്ണൽ xkb_action_type action,
                     എണ്ണൽ action_field field,
                     എണ്ണൽ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    തിരികെ (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
HandleNoAction(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
               സ്ഥിരം ഘടന xkb_mod_set *mods,
               സംഘം xkb_action *action, എണ്ണൽ action_field field,
               സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
               വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    തിരികെ (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
CheckBooleanFlag(ഘടന xkb_context *ctx, എണ്ണൽ xkb_parser_strict_flags strict,
                 എണ്ണൽ xkb_action_type action, എണ്ണൽ action_field field,
                 എണ്ണൽ xkb_action_flags flag, സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx,
                 സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം, എണ്ണൽ xkb_action_flags *flags_inout)
{
    bool set = false;

    എങ്കിൽ (array_ndx)
        തിരികെ ReportActionNotArray(ctx, action, field, strict);

    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Boolean(ctx, മൂല്യം, &set))
        തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    എങ്കിൽ (set)
        *flags_inout |= flag;
    അല്ലെങ്കിൽ
        *flags_inout &= ~flag;

    തിരികെ PARSER_SUCCESS;
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
CheckModifierField(ഘടന xkb_context *ctx, എണ്ണൽ xkb_parser_strict_flags strict,
                   സ്ഥിരം ഘടന xkb_mod_set *mods, എണ്ണൽ xkb_action_type action,
                   സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
                   എണ്ണൽ xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    എങ്കിൽ (array_ndx)
        തിരികെ ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    എങ്കിൽ (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_IDENT) {
        സ്ഥിരം അക്ഷരം *valStr;
        valStr = xkb_atom_text(ctx, മൂല്യം->ident.ident);
        എങ്കിൽ (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            തിരികെ PARSER_SUCCESS;
        }
    }

    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mod_Mask(ctx, മൂല്യം, MOD_BOTH, mods, mods_rtrn))
        തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    തിരികെ PARSER_SUCCESS;
}

സ്ഥിതം സ്ഥിരം LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { ശൂന്യ, 0 }
};

സ്ഥിതം എണ്ണൽ xkb_parser_error
CheckAffectField(ഘടന xkb_context *ctx, എണ്ണൽ xkb_parser_strict_flags strict,
                 എണ്ണൽ xkb_action_type action, സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx,
                 സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം, എണ്ണൽ xkb_action_flags *flags_inout)
{
    എങ്കിൽ (array_ndx)
        തിരികെ ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Enum(ctx, മൂല്യം, &flags, lockWhich))
        തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (എണ്ണൽ xkb_action_flags) flags;
    തിരികെ PARSER_SUCCESS;
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
HandleSetLatchLockMods(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                       സ്ഥിരം ഘടന xkb_mod_set *mods,
                       സംഘം xkb_action *action, എണ്ണൽ action_field field,
                       സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
                       വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    ഘടന xkb_context * restrict സ്ഥിരം ctx = keymap_info->keymap.ctx;
    ഘടന xkb_mod_action *act = &action->mods;
    സ്ഥിരം എണ്ണൽ xkb_action_type type = action->type;

    എങ്കിൽ (field == ACTION_FIELD_MODIFIERS)
        തിരികെ CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, മൂല്യം, &act->flags, &act->mods.mods);
    എങ്കിൽ (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        പ്രതിജ്ഞാ(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        എങ്കിൽ (keymap_info->features.mods_unlock_on_press) {
            സ്ഥിരം എണ്ണൽ xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, മൂല്യം, &act->flags
            );
            എങ്കിൽ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            തിരികെ ret;
        } അല്ലെങ്കിൽ {
            തിരികെ ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    എങ്കിൽ ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        തിരികെ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, മൂല്യം,
                                &act->flags);
    എങ്കിൽ (type == ACTION_TYPE_MOD_LATCH) {
        എങ്കിൽ (field == ACTION_FIELD_LATCH_TO_LOCK)
            തിരികെ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    മൂല്യം, &act->flags);
        എങ്കിൽ (field == ACTION_FIELD_LATCH_ON_PRESS) {
            എങ്കിൽ (keymap_info->features.mods_latch_on_press) {
                തിരികെ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        മൂല്യം, &act->flags);
            } അല്ലെങ്കിൽ {
                തിരികെ ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    എങ്കിൽ (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        തിരികെ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, മൂല്യം, &act->flags);

    തിരികെ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
CheckGroupField(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                എണ്ണൽ xkb_action_type action, സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx,
                സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം, വ്യഞ്ജകം_Def **മൂല്യം_ptr,
                എണ്ണൽ xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    സ്ഥിരം വ്യഞ്ജകം_Def *spec;
    xkb_layout_index_t idx = 0;
    എണ്ണൽ xkb_action_flags flags = *flags_inout;

    എങ്കിൽ (array_ndx)
        തിരികെ ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    എങ്കിൽ (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_NEGATE ||
        മൂല്യം->common.type == STMT_വ്യഞ്ജകം_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = മൂല്യം->unary.child;
        മൂല്യം_ptr = &(*മൂല്യം_ptr)->unary.child;
    } അല്ലെങ്കിൽ {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = മൂല്യം;
    }

    സ്ഥിരം bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    സ്ഥിരം എണ്ണൽ xkb_parser_error ret =
        വ്യഞ്ജകം_Resolve_Group(keymap_info, spec, absolute, &idx, &pending);
    എങ്കിൽ (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        തിരികെ ret;
    }

    എങ്കിൽ (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        സ്ഥിരം darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (ഘടന pending_computation) {
                .വ്യഞ്ജകം = *മൂല്യം_ptr,
                .computed = false,
                .മൂല്യം = 0,
            }
        );
        *മൂല്യം_ptr = ശൂന്യ;
        static_assert(വലുപ്പം(pending_index) == വലുപ്പം(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } അല്ലെങ്കിൽ {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        എങ്കിൽ (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } അല്ലെങ്കിൽ {
            *group_rtrn = (int32_t) idx;
            എങ്കിൽ (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    തിരികെ PARSER_SUCCESS;
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
HandleSetLatchLockGroup(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                        സ്ഥിരം ഘടന xkb_mod_set *mods,
                        സംഘം xkb_action *action, എണ്ണൽ action_field field,
                        സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
                        വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    ഘടന xkb_context * restrict സ്ഥിരം ctx = keymap_info->keymap.ctx;
    ഘടന xkb_group_action *act = &action->group;
    സ്ഥിരം എണ്ണൽ xkb_action_type type = action->type;

    എങ്കിൽ (field == ACTION_FIELD_GROUP) {
        തിരികെ CheckGroupField(keymap_info, action->type, array_ndx, മൂല്യം,
                               മൂല്യം_ptr, &act->flags, &act->group);
    }
    എങ്കിൽ ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        തിരികെ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, മൂല്യം,
                                &act->flags);
    എങ്കിൽ (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        തിരികെ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, മൂല്യം,
                                &act->flags);
    എങ്കിൽ (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        എങ്കിൽ (keymap_info->features.group_lock_on_release) {
            സ്ഥിരം എണ്ണൽ xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, മൂല്യം, &act->flags
            );
            എങ്കിൽ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            തിരികെ ret;
        } അല്ലെങ്കിൽ {
            തിരികെ ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    തിരികെ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
HandleMovePtr(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
              സ്ഥിരം ഘടന xkb_mod_set *mods,
              സംഘം xkb_action *action, എണ്ണൽ action_field field,
              സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
              വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    ഘടന xkb_context * restrict സ്ഥിരം ctx = keymap_info->keymap.ctx;
    ഘടന xkb_pointer_motion_action *act = &action->ptr;

    എങ്കിൽ (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        സ്ഥിരം bool absolute = (മൂല്യം->common.type != STMT_വ്യഞ്ജകം_NEGATE &&
                               മൂല്യം->common.type != STMT_വ്യഞ്ജകം_UNARY_PLUS);

        എങ്കിൽ (array_ndx)
            തിരികെ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer(ctx, മൂല്യം, &val))
            തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        എങ്കിൽ (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        എങ്കിൽ (field == ACTION_FIELD_X) {
            എങ്കിൽ (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        അല്ലെങ്കിൽ {
            എങ്കിൽ (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        തിരികെ PARSER_SUCCESS;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (field == ACTION_FIELD_REPEAT) {
        തിരികെ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, മൂല്യം, &act->flags);
    }

    തിരികെ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
HandlePtrBtn(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
             സ്ഥിരം ഘടന xkb_mod_set *mods,
             സംഘം xkb_action *action, എണ്ണൽ action_field field,
             സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
             വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    ഘടന xkb_context * restrict സ്ഥിരം ctx = keymap_info->keymap.ctx;
    ഘടന xkb_pointer_button_action *act = &action->btn;

    എങ്കിൽ (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        എങ്കിൽ (array_ndx)
            തിരികെ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Button(ctx, മൂല്യം, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        എങ്കിൽ (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        തിരികെ PARSER_SUCCESS;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        തിരികെ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, മൂല്യം, &act->flags);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (field == ACTION_FIELD_എണ്ണം) {
        int64_t val = 0;

        എങ്കിൽ (array_ndx)
            തിരികെ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer(ctx, മൂല്യം, &val))
            തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        എങ്കിൽ (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        എങ്കിൽ (action->type != ACTION_TYPE_PTR_LOCK)
            act->എണ്ണം = (uint8_t) val;
        തിരികെ PARSER_SUCCESS;
    }

    തിരികെ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

എണ്ണൽ {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    SET_POINTER_DEFAULT_BUTTON_AFFECT_അക്കം_ENTRIES
};

സ്ഥിതം സ്ഥിരം LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { ശൂന്യ, 0 }
};

സ്ഥിതം എണ്ണൽ xkb_parser_error
HandleSetPtrDflt(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                 സ്ഥിരം ഘടന xkb_mod_set *mods,
                 സംഘം xkb_action *action, എണ്ണൽ action_field field,
                 സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
                 വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    ഘടന xkb_context * restrict സ്ഥിരം ctx = keymap_info->keymap.ctx;
    ഘടന xkb_pointer_default_action *act = &action->dflt;

    എങ്കിൽ (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        എങ്കിൽ (array_ndx)
            തിരികെ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Enum(ctx, മൂല്യം, &val, ptrDflts))
            തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(SET_POINTER_DEFAULT_BUTTON_AFFECT_അക്കം_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        തിരികെ PARSER_SUCCESS;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_മൂല്യം) {
        സ്ഥിരം വ്യഞ്ജകം_Def *button;
        int64_t btn = 0;

        എങ്കിൽ (array_ndx)
            തിരികെ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        എങ്കിൽ (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_NEGATE ||
            മൂല്യം->common.type == STMT_വ്യഞ്ജകം_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = മൂല്യം->unary.child;
        }
        അല്ലെങ്കിൽ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = മൂല്യം;
        }

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        എങ്കിൽ (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        എങ്കിൽ (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->മൂല്യം = (int8_t) (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_NEGATE ? -btn: btn);
        തിരികെ PARSER_SUCCESS;
    }

    തിരികെ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
HandleSwitchScreen(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                   സ്ഥിരം ഘടന xkb_mod_set *mods,
                   സംഘം xkb_action *action, എണ്ണൽ action_field field,
                   സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
                   വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    ഘടന xkb_context * restrict സ്ഥിരം ctx = keymap_info->keymap.ctx;
    ഘടന xkb_switch_screen_action *act = &action->screen;

    എങ്കിൽ (field == ACTION_FIELD_SCREEN) {
        സ്ഥിരം വ്യഞ്ജകം_Def *scrn;
        int64_t val = 0;

        എങ്കിൽ (array_ndx)
            തിരികെ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        എങ്കിൽ (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_NEGATE ||
            മൂല്യം->common.type == STMT_വ്യഞ്ജകം_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = മൂല്യം->unary.child;
        }
        അല്ലെങ്കിൽ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = മൂല്യം;
        }

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer(ctx, scrn, &val))
            തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_NEGATE ? -val : val);
        എങ്കിൽ (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        തിരികെ PARSER_SUCCESS;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (field == ACTION_FIELD_SAME) {
        തിരികെ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, മൂല്യം,
                                &act->flags);
    }

    തിരികെ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
HandleSetLockControls(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                      സ്ഥിരം ഘടന xkb_mod_set *mods,
                      സംഘം xkb_action *action, എണ്ണൽ action_field field,
                      സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
                      വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    ഘടന xkb_context * restrict സ്ഥിരം ctx = keymap_info->keymap.ctx;
    ഘടന xkb_controls_action *act = &action->ctrls;

    എങ്കിൽ (field == ACTION_FIELD_CONTROLS) {
        എങ്കിൽ (array_ndx)
            തിരികെ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        സ്ഥിരം uint8_t offset = keymap_info->features.controls_name_offset;
        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Mask(ctx, മൂല്യം, &mask, ctrlMaskNames + offset))
            തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (എണ്ണൽ xkb_action_controls) mask;
        തിരികെ PARSER_SUCCESS;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        തിരികെ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, മൂല്യം, &act->flags);
    }

    തിരികെ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
Handle_Redirect_താക്കോൽ(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                  സ്ഥിരം ഘടന xkb_mod_set *mods,
                  സംഘം xkb_action *action, എണ്ണൽ action_field field,
                  സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
                  വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    സ്ഥിരം ഘടന xkb_keymap * restrict സ്ഥിരം keymap = &keymap_info->keymap;
    ഘടന xkb_context * restrict സ്ഥിരം ctx = keymap->ctx;
    ഘടന xkb_redirect_താക്കോൽ_action * സ്ഥിരം act = &action->redirect;

    എങ്കിൽ (field == ACTION_FIELD_KEYCODE) {
        എങ്കിൽ (array_ndx)
            തിരികെ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        എങ്കിൽ (മൂല്യം->common.type == STMT_വ്യഞ്ജകം_IDENT) {
            സ്ഥിരം അക്ഷരം *valStr = xkb_atom_text(ctx, മൂല്യം->ident.ident);
            എങ്കിൽ (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_താക്കോൽ_auto;
                തിരികെ PARSER_SUCCESS;
            }
        }
        എങ്കിൽ (മൂല്യം->common.type != STMT_വ്യഞ്ജകം_KEYNAME_LITERAL) {
            തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        സ്ഥിരം ഘടന xkb_താക്കോൽ * സ്ഥിരം താക്കോൽ =
            Xkb_താക്കോൽ_By_Name(keymap, മൂല്യം->താക്കോൽ_name.താക്കോൽ_name, true);
        എങ്കിൽ (താക്കോൽ == ശൂന്യ) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), താക്കോൽ_Name_Text(ctx, മൂല്യം->താക്കോൽ_name.താക്കോൽ_name));
            തിരികെ (keymap_info->strict & PARSER_NO_FIELD_മൂല്യം_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = താക്കോൽ->keycode;
        തിരികെ PARSER_SUCCESS;
    }

    എങ്കിൽ (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        എണ്ണൽ xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        എണ്ണൽ xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, മൂല്യം, &flags, &m);
        എങ്കിൽ (r != PARSER_SUCCESS)
            തിരികെ r;
        എങ്കിൽ (flags)
            തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        എങ്കിൽ (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        അല്ലെങ്കിൽ
            act->mods &= ~m;
        തിരികെ PARSER_SUCCESS;
    }

    തിരികെ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
HandleUnsupported(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                        സ്ഥിരം ഘടന xkb_mod_set *mods,
                        സംഘം xkb_action *action, എണ്ണൽ action_field field,
                        സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
                        വ്യഞ്ജകം_Def **മൂല്യം_ptr)

{
    /* Do not check any field */
    തിരികെ PARSER_SUCCESS;
}

സ്ഥിതം എണ്ണൽ xkb_parser_error
HandlePrivate(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
              സ്ഥിരം ഘടന xkb_mod_set *mods,
              സംഘം xkb_action *action, എണ്ണൽ action_field field,
              സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx, സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
              വ്യഞ്ജകം_Def **മൂല്യം_ptr)
{
    ഘടന xkb_context * restrict സ്ഥിരം ctx = keymap_info->keymap.ctx;
    ഘടന xkb_private_action *act = &action->priv;

    എങ്കിൽ (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        എങ്കിൽ (array_ndx)
            തിരികെ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer(ctx, മൂല്യം, &type))
            തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        എങ്കിൽ (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        എങ്കിൽ (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((എണ്ണൽ xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        അല്ലെങ്കിൽ {
            act->type = (എണ്ണൽ xkb_action_type) type;
        }

        തിരികെ PARSER_SUCCESS;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (field == ACTION_FIELD_DATA) {
        എങ്കിൽ (array_ndx == ശൂന്യ) {
            xkb_atom_t val = XKB_ATOM_NONE;

            എങ്കിൽ (!വ്യഞ്ജകം_Resolve_String(ctx, മൂല്യം, &val))
                തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            സ്ഥിരം അക്ഷരം *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            എങ്കിൽ (len < 1 || len > വലുപ്പം(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", വലുപ്പം(act->data), len);
                തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, വലുപ്പം(act->data));
            memcpy(act->data, str, len);
            തിരികെ PARSER_SUCCESS;
        }
        അല്ലെങ്കിൽ {
            int64_t ndx = 0, datum = 0;

            എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            എങ്കിൽ (ndx < 0 || (size_t) ndx >= വലുപ്പം(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        വലുപ്പം(act->data), ndx);
                തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer(ctx, മൂല്യം, &datum))
                തിരികെ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            എങ്കിൽ (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                തിരികെ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            തിരികെ PARSER_SUCCESS;
        }
    }

    തിരികെ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

തരനിർവചനം എണ്ണൽ xkb_parser_error
        (*actionHandler)(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                         സ്ഥിരം ഘടന xkb_mod_set *mods,
                         സംഘം xkb_action *action,
                         എണ്ണൽ action_field field,
                         സ്ഥിരം വ്യഞ്ജകം_Def *array_ndx,
                         സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം,
                         വ്യഞ്ജകം_Def **മൂല്യം_ptr);

സ്ഥിതം സ്ഥിരം actionHandler handleAction[ACTION_TYPE_അക്കം_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_താക്കോൽ] = Handle_Redirect_താക്കോൽ,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_അക്കം_ENTRIES,
              "Missing action type");

/***====================================================================***/

എണ്ണൽ xkb_parser_error
HandleActionDef(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info, ActionsInfo *info,
                സ്ഥിരം ഘടന xkb_mod_set *mods, വ്യഞ്ജകം_Def *def,
                സംഘം xkb_action *action)
{
    ഘടന xkb_context * restrict സ്ഥിരം ctx = keymap_info->keymap.ctx;
    എങ്കിൽ (def->common.type != STMT_വ്യഞ്ജകം_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        തിരികെ PARSER_FATAL_ERROR;
    }

    സ്ഥിരം അക്ഷരം *action_name = xkb_atom_text(ctx, def->action.name);
    എണ്ണൽ xkb_action_type handler_type;
    എങ്കിൽ (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        എങ്കിൽ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            തിരികെ PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    എങ്കിൽ (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    എണ്ണൽ xkb_parser_error ret = PARSER_SUCCESS;
    വേണ്ടി (വ്യഞ്ജകം_Def *arg = def->action.args; arg != ശൂന്യ;
         arg = (വ്യഞ്ജകം_Def *) arg->common.next) {
        സ്ഥിരം വ്യഞ്ജകം_Def *മൂല്യം;
        വ്യഞ്ജകം_Def **മൂല്യം_ptr = ശൂന്യ;
        വ്യഞ്ജകം_Def *field, *arrayRtrn;
        സ്ഥിരം അക്ഷരം *elemRtrn, *fieldRtrn;

        എങ്കിൽ (arg->common.type == STMT_വ്യഞ്ജകം_ASSIGN) {
            field = arg->ദ്വിമാന.ഇടത്;
            മൂല്യം = arg->ദ്വിമാന.വലത്;
            മൂല്യം_ptr = &arg->ദ്വിമാന.വലത്;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (arg->common.type == STMT_വ്യഞ്ജകം_NOT ||
                 arg->common.type == STMT_വ്യഞ്ജകം_INVERT) {
            field = arg->unary.child;
            മൂല്യം = (സ്ഥിരം വ്യഞ്ജകം_Def *) &constFalse;
        }
        അല്ലെങ്കിൽ {
            field = arg;
            മൂല്യം = (സ്ഥിരം വ്യഞ്ജകം_Def *) &constTrue;
        }

        എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Lhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            തിരികെ PARSER_FATAL_ERROR;

        എങ്കിൽ (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            തിരികെ PARSER_FATAL_ERROR;
        }

        എണ്ണൽ action_field fieldNdx;
        എങ്കിൽ (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            എങ്കിൽ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                തിരികെ PARSER_FATAL_ERROR;
            } അല്ലെങ്കിൽ {
                /* Field is just ignored */
                തുടരുക;
            }
        }

        തിരഞ്ഞെടുപ്പ് (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, മൂല്യം, മൂല്യം_ptr)) {
        അവസ്ഥ PARSER_FATAL_ERROR:
            തിരികെ PARSER_FATAL_ERROR;
        അവസ്ഥ PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            ;
        }
    }

    തിരികെ (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

എണ്ണൽ xkb_parser_error
SetDefaultActionField(സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ഘടന xkb_mod_set *mods,
                      സ്ഥിരം അക്ഷരം *elem, സ്ഥിരം അക്ഷരം *field, വ്യഞ്ജകം_Def *array_ndx,
                      വ്യഞ്ജകം_Def **മൂല്യം_ptr, എണ്ണൽ merge_mode merge)
{
    സ്ഥിരം വ്യഞ്ജകം_Def * restrict സ്ഥിരം മൂല്യം = *മൂല്യം_ptr;
    എണ്ണൽ xkb_action_type action;
    എങ്കിൽ (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        തിരികെ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    എണ്ണൽ action_field action_field;
    എങ്കിൽ (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        തിരികെ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    സംഘം xkb_action* സ്ഥിരം into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    സംഘം xkb_action from = *into;

    /* Parse action */
    സ്ഥിരം എണ്ണൽ xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, മൂല്യം, മൂല്യം_ptr
    );

    /* Success is mandatory to update defaults */
    എങ്കിൽ (ret != PARSER_SUCCESS)
        തിരികെ ret;

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
    എങ്കിൽ (!action_equal(into, &from)) {
        സ്ഥിരം bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        എങ്കിൽ (replace)
            *into = from;
    }
    തിരികെ PARSER_SUCCESS;
}
