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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "text.h"
#அந்தர்பாவ "expr.h"
#அந்தர்பாவ "xkbcomp/ast.h"
#அந்தர்பாவ "action.h"

நிலையான மாறிலி கோவை_Boolean constTrue = {
    .common = { .type = STMT_கோவை_BOOLEAN_LITERAL, .next = வெற்று },
    .set = true,
};

நிலையான மாறிலி கோவை_Boolean constFalse = {
    .common = { .type = STMT_கோவை_BOOLEAN_LITERAL, .next = வெற்று },
    .set = false,
};

எண்ணிக்கை action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_சாவி_EVENT,
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
    ACTION_FIELD_மதிப்பு,
    ACTION_FIELD_CONTROLS,
    ACTION_FIELD_TYPE,
    ACTION_FIELD_மொத்தம்,
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

வெறுமை
InitActionsInfo(மாறிலி கட்டமைப்பு xkb_keymap *keymap, ActionsInfo *info)
{
    க்கு (எண்ணிக்கை xkb_action_type type = 0; type < ACTION_TYPE_இலக்கம்_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.மதிப்பு = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_சாவி].redirect.keycode =
        keymap->redirect_சாவி_auto;
}

நிலையான மாறிலி LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_சாவி_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_சாவி_EVENT   },
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
    { "value",            ACTION_FIELD_மதிப்பு           },
    { "controls",         ACTION_FIELD_CONTROLS        },
    { "ctrls",            ACTION_FIELD_CONTROLS        },
    { "type",             ACTION_FIELD_TYPE            },
    { "count",            ACTION_FIELD_மொத்தம்           },
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
    { வெற்று,               0                            }
};

நிலையான bool
stringToActionType(மாறிலி எழுத்து *str, எண்ணிக்கை xkb_action_type *type_rtrn)
{
    unsigned முழு type = 0;
    மாறிலி bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (எண்ணிக்கை xkb_action_type) type;
    திரும்பு ret;
}

நிலையான bool
stringToField(மாறிலி எழுத்து *str, எண்ணிக்கை action_field *field_rtrn)
{
    unsigned முழு field = 0;
    மாறிலி bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (எண்ணிக்கை action_field) field;
    திரும்பு ret;
}

நிலையான மாறிலி எழுத்து *
fieldText(எண்ணிக்கை action_field field)
{
    திரும்பு Lookup_மதிப்பு(fieldStrings, field);
}

/***====================================================================***/

நிலையான உள்வரி எண்ணிக்கை xkb_parser_error
ReportMismatch(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_message_code code,
               எண்ணிக்கை xkb_action_type action, எண்ணிக்கை action_field field,
               மாறிலி எழுத்து *type, எண்ணிக்கை xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    திரும்பு (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

நிலையான உள்வரி எண்ணிக்கை xkb_parser_error
ReportFormatVersionMismatch(கட்டமைப்பு xkb_context *ctx,
                            எண்ணிக்கை xkb_action_type action,
                            எண்ணிக்கை action_field field,
                            எண்ணிக்கை xkb_keymap_format format,
                            மாறிலி எழுத்து *versions,
                            எண்ணிக்கை xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    திரும்பு (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

நிலையான உள்வரி எண்ணிக்கை xkb_parser_error
ReportIllegal(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_action_type action,
              எண்ணிக்கை action_field field, எண்ணிக்கை xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    திரும்பு (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

நிலையான உள்வரி எண்ணிக்கை xkb_parser_error
ReportActionNotArray(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_action_type action,
                     எண்ணிக்கை action_field field,
                     எண்ணிக்கை xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    திரும்பு (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

நிலையான எண்ணிக்கை xkb_parser_error
HandleNoAction(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
               மாறிலி கட்டமைப்பு xkb_mod_set *mods,
               ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
               மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
               கோவை_Def **மதிப்பு_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    திரும்பு (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

நிலையான எண்ணிக்கை xkb_parser_error
CheckBooleanFlag(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_parser_strict_flags strict,
                 எண்ணிக்கை xkb_action_type action, எண்ணிக்கை action_field field,
                 எண்ணிக்கை xkb_action_flags flag, மாறிலி கோவை_Def *array_ndx,
                 மாறிலி கோவை_Def *மதிப்பு, எண்ணிக்கை xkb_action_flags *flags_inout)
{
    bool set = false;

    எனில் (array_ndx)
        திரும்பு ReportActionNotArray(ctx, action, field, strict);

    எனில் (!கோவை_Resolve_Boolean(ctx, மதிப்பு, &set))
        திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    எனில் (set)
        *flags_inout |= flag;
    இல்லையெனில்
        *flags_inout &= ~flag;

    திரும்பு PARSER_SUCCESS;
}

நிலையான எண்ணிக்கை xkb_parser_error
CheckModifierField(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_parser_strict_flags strict,
                   மாறிலி கட்டமைப்பு xkb_mod_set *mods, எண்ணிக்கை xkb_action_type action,
                   மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
                   எண்ணிக்கை xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    எனில் (array_ndx)
        திரும்பு ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    எனில் (மதிப்பு->common.type == STMT_கோவை_IDENT) {
        மாறிலி எழுத்து *valStr;
        valStr = xkb_atom_text(ctx, மதிப்பு->ident.ident);
        எனில் (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            திரும்பு PARSER_SUCCESS;
        }
    }

    எனில் (!கோவை_Resolve_Mod_Mask(ctx, மதிப்பு, MOD_BOTH, mods, mods_rtrn))
        திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    திரும்பு PARSER_SUCCESS;
}

நிலையான மாறிலி LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { வெற்று, 0 }
};

நிலையான எண்ணிக்கை xkb_parser_error
CheckAffectField(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_parser_strict_flags strict,
                 எண்ணிக்கை xkb_action_type action, மாறிலி கோவை_Def *array_ndx,
                 மாறிலி கோவை_Def *மதிப்பு, எண்ணிக்கை xkb_action_flags *flags_inout)
{
    எனில் (array_ndx)
        திரும்பு ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    எனில் (!கோவை_Resolve_Enum(ctx, மதிப்பு, &flags, lockWhich))
        திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (எண்ணிக்கை xkb_action_flags) flags;
    திரும்பு PARSER_SUCCESS;
}

நிலையான எண்ணிக்கை xkb_parser_error
HandleSetLatchLockMods(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                       மாறிலி கட்டமைப்பு xkb_mod_set *mods,
                       ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
                       மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
                       கோவை_Def **மதிப்பு_ptr)
{
    கட்டமைப்பு xkb_context * restrict மாறிலி ctx = keymap_info->keymap.ctx;
    கட்டமைப்பு xkb_mod_action *act = &action->mods;
    மாறிலி எண்ணிக்கை xkb_action_type type = action->type;

    எனில் (field == ACTION_FIELD_MODIFIERS)
        திரும்பு CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, மதிப்பு, &act->flags, &act->mods.mods);
    எனில் (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        ப்ரதிஜ்ஞா(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        எனில் (keymap_info->features.mods_unlock_on_press) {
            மாறிலி எண்ணிக்கை xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, மதிப்பு, &act->flags
            );
            எனில் (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            திரும்பு ret;
        } இல்லையெனில் {
            திரும்பு ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    எனில் ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        திரும்பு CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, மதிப்பு,
                                &act->flags);
    எனில் (type == ACTION_TYPE_MOD_LATCH) {
        எனில் (field == ACTION_FIELD_LATCH_TO_LOCK)
            திரும்பு CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    மதிப்பு, &act->flags);
        எனில் (field == ACTION_FIELD_LATCH_ON_PRESS) {
            எனில் (keymap_info->features.mods_latch_on_press) {
                திரும்பு CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        மதிப்பு, &act->flags);
            } இல்லையெனில் {
                திரும்பு ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    எனில் (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        திரும்பு CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, மதிப்பு, &act->flags);

    திரும்பு ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

நிலையான எண்ணிக்கை xkb_parser_error
CheckGroupField(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                எண்ணிக்கை xkb_action_type action, மாறிலி கோவை_Def *array_ndx,
                மாறிலி கோவை_Def *மதிப்பு, கோவை_Def **மதிப்பு_ptr,
                எண்ணிக்கை xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    மாறிலி கோவை_Def *spec;
    xkb_layout_index_t idx = 0;
    எண்ணிக்கை xkb_action_flags flags = *flags_inout;

    எனில் (array_ndx)
        திரும்பு ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    எனில் (மதிப்பு->common.type == STMT_கோவை_NEGATE ||
        மதிப்பு->common.type == STMT_கோவை_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = மதிப்பு->unary.child;
        மதிப்பு_ptr = &(*மதிப்பு_ptr)->unary.child;
    } இல்லையெனில் {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = மதிப்பு;
    }

    மாறிலி bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    மாறிலி எண்ணிக்கை xkb_parser_error ret =
        கோவை_Resolve_Group(keymap_info, spec, absolute, &idx, &pending);
    எனில் (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        திரும்பு ret;
    }

    எனில் (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        மாறிலி darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (கட்டமைப்பு pending_computation) {
                .கோவை = *மதிப்பு_ptr,
                .computed = false,
                .மதிப்பு = 0,
            }
        );
        *மதிப்பு_ptr = வெற்று;
        static_assert(அளவு(pending_index) == அளவு(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } இல்லையெனில் {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        எனில் (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } இல்லையெனில் {
            *group_rtrn = (int32_t) idx;
            எனில் (மதிப்பு->common.type == STMT_கோவை_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    திரும்பு PARSER_SUCCESS;
}

நிலையான எண்ணிக்கை xkb_parser_error
HandleSetLatchLockGroup(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                        மாறிலி கட்டமைப்பு xkb_mod_set *mods,
                        ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
                        மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
                        கோவை_Def **மதிப்பு_ptr)
{
    கட்டமைப்பு xkb_context * restrict மாறிலி ctx = keymap_info->keymap.ctx;
    கட்டமைப்பு xkb_group_action *act = &action->group;
    மாறிலி எண்ணிக்கை xkb_action_type type = action->type;

    எனில் (field == ACTION_FIELD_GROUP) {
        திரும்பு CheckGroupField(keymap_info, action->type, array_ndx, மதிப்பு,
                               மதிப்பு_ptr, &act->flags, &act->group);
    }
    எனில் ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        திரும்பு CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, மதிப்பு,
                                &act->flags);
    எனில் (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        திரும்பு CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, மதிப்பு,
                                &act->flags);
    எனில் (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        எனில் (keymap_info->features.group_lock_on_release) {
            மாறிலி எண்ணிக்கை xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, மதிப்பு, &act->flags
            );
            எனில் (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            திரும்பு ret;
        } இல்லையெனில் {
            திரும்பு ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    திரும்பு ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

நிலையான எண்ணிக்கை xkb_parser_error
HandleMovePtr(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
              மாறிலி கட்டமைப்பு xkb_mod_set *mods,
              ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
              மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
              கோவை_Def **மதிப்பு_ptr)
{
    கட்டமைப்பு xkb_context * restrict மாறிலி ctx = keymap_info->keymap.ctx;
    கட்டமைப்பு xkb_pointer_motion_action *act = &action->ptr;

    எனில் (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        மாறிலி bool absolute = (மதிப்பு->common.type != STMT_கோவை_NEGATE &&
                               மதிப்பு->common.type != STMT_கோவை_UNARY_PLUS);

        எனில் (array_ndx)
            திரும்பு ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        எனில் (!கோவை_Resolve_Integer(ctx, மதிப்பு, &val))
            திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        எனில் (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        எனில் (field == ACTION_FIELD_X) {
            எனில் (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        இல்லையெனில் {
            எனில் (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        திரும்பு PARSER_SUCCESS;
    }
    இல்லையெனில் எனில் (field == ACTION_FIELD_REPEAT) {
        திரும்பு CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, மதிப்பு, &act->flags);
    }

    திரும்பு ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

நிலையான எண்ணிக்கை xkb_parser_error
HandlePtrBtn(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
             மாறிலி கட்டமைப்பு xkb_mod_set *mods,
             ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
             மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
             கோவை_Def **மதிப்பு_ptr)
{
    கட்டமைப்பு xkb_context * restrict மாறிலி ctx = keymap_info->keymap.ctx;
    கட்டமைப்பு xkb_pointer_button_action *act = &action->btn;

    எனில் (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        எனில் (array_ndx)
            திரும்பு ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        எனில் (!கோவை_Resolve_Button(ctx, மதிப்பு, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        எனில் (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        திரும்பு PARSER_SUCCESS;
    }
    இல்லையெனில் எனில் (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        திரும்பு CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, மதிப்பு, &act->flags);
    }
    இல்லையெனில் எனில் (field == ACTION_FIELD_மொத்தம்) {
        int64_t val = 0;

        எனில் (array_ndx)
            திரும்பு ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        எனில் (!கோவை_Resolve_Integer(ctx, மதிப்பு, &val))
            திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        எனில் (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        எனில் (action->type != ACTION_TYPE_PTR_LOCK)
            act->மொத்தம் = (uint8_t) val;
        திரும்பு PARSER_SUCCESS;
    }

    திரும்பு ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

எண்ணிக்கை {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    SET_POINTER_DEFAULT_BUTTON_AFFECT_இலக்கம்_ENTRIES
};

நிலையான மாறிலி LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { வெற்று, 0 }
};

நிலையான எண்ணிக்கை xkb_parser_error
HandleSetPtrDflt(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                 மாறிலி கட்டமைப்பு xkb_mod_set *mods,
                 ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
                 மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
                 கோவை_Def **மதிப்பு_ptr)
{
    கட்டமைப்பு xkb_context * restrict மாறிலி ctx = keymap_info->keymap.ctx;
    கட்டமைப்பு xkb_pointer_default_action *act = &action->dflt;

    எனில் (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        எனில் (array_ndx)
            திரும்பு ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        எனில் (!கோவை_Resolve_Enum(ctx, மதிப்பு, &val, ptrDflts))
            திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(SET_POINTER_DEFAULT_BUTTON_AFFECT_இலக்கம்_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        திரும்பு PARSER_SUCCESS;
    }
    இல்லையெனில் எனில் (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_மதிப்பு) {
        மாறிலி கோவை_Def *button;
        int64_t btn = 0;

        எனில் (array_ndx)
            திரும்பு ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        எனில் (மதிப்பு->common.type == STMT_கோவை_NEGATE ||
            மதிப்பு->common.type == STMT_கோவை_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = மதிப்பு->unary.child;
        }
        இல்லையெனில் {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = மதிப்பு;
        }

        எனில் (!கோவை_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        எனில் (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        எனில் (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->மதிப்பு = (int8_t) (மதிப்பு->common.type == STMT_கோவை_NEGATE ? -btn: btn);
        திரும்பு PARSER_SUCCESS;
    }

    திரும்பு ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

நிலையான எண்ணிக்கை xkb_parser_error
HandleSwitchScreen(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                   மாறிலி கட்டமைப்பு xkb_mod_set *mods,
                   ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
                   மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
                   கோவை_Def **மதிப்பு_ptr)
{
    கட்டமைப்பு xkb_context * restrict மாறிலி ctx = keymap_info->keymap.ctx;
    கட்டமைப்பு xkb_switch_screen_action *act = &action->screen;

    எனில் (field == ACTION_FIELD_SCREEN) {
        மாறிலி கோவை_Def *scrn;
        int64_t val = 0;

        எனில் (array_ndx)
            திரும்பு ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        எனில் (மதிப்பு->common.type == STMT_கோவை_NEGATE ||
            மதிப்பு->common.type == STMT_கோவை_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = மதிப்பு->unary.child;
        }
        இல்லையெனில் {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = மதிப்பு;
        }

        எனில் (!கோவை_Resolve_Integer(ctx, scrn, &val))
            திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (மதிப்பு->common.type == STMT_கோவை_NEGATE ? -val : val);
        எனில் (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        திரும்பு PARSER_SUCCESS;
    }
    இல்லையெனில் எனில் (field == ACTION_FIELD_SAME) {
        திரும்பு CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, மதிப்பு,
                                &act->flags);
    }

    திரும்பு ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

நிலையான எண்ணிக்கை xkb_parser_error
HandleSetLockControls(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                      மாறிலி கட்டமைப்பு xkb_mod_set *mods,
                      ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
                      மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
                      கோவை_Def **மதிப்பு_ptr)
{
    கட்டமைப்பு xkb_context * restrict மாறிலி ctx = keymap_info->keymap.ctx;
    கட்டமைப்பு xkb_controls_action *act = &action->ctrls;

    எனில் (field == ACTION_FIELD_CONTROLS) {
        எனில் (array_ndx)
            திரும்பு ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        மாறிலி uint8_t offset = keymap_info->features.controls_name_offset;
        எனில் (!கோவை_Resolve_Mask(ctx, மதிப்பு, &mask, ctrlMaskNames + offset))
            திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (எண்ணிக்கை xkb_action_controls) mask;
        திரும்பு PARSER_SUCCESS;
    }
    இல்லையெனில் எனில் (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        திரும்பு CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, மதிப்பு, &act->flags);
    }

    திரும்பு ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

நிலையான எண்ணிக்கை xkb_parser_error
Handle_Redirect_சாவி(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                  மாறிலி கட்டமைப்பு xkb_mod_set *mods,
                  ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
                  மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
                  கோவை_Def **மதிப்பு_ptr)
{
    மாறிலி கட்டமைப்பு xkb_keymap * restrict மாறிலி keymap = &keymap_info->keymap;
    கட்டமைப்பு xkb_context * restrict மாறிலி ctx = keymap->ctx;
    கட்டமைப்பு xkb_redirect_சாவி_action * மாறிலி act = &action->redirect;

    எனில் (field == ACTION_FIELD_KEYCODE) {
        எனில் (array_ndx)
            திரும்பு ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        எனில் (மதிப்பு->common.type == STMT_கோவை_IDENT) {
            மாறிலி எழுத்து *valStr = xkb_atom_text(ctx, மதிப்பு->ident.ident);
            எனில் (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_சாவி_auto;
                திரும்பு PARSER_SUCCESS;
            }
        }
        எனில் (மதிப்பு->common.type != STMT_கோவை_KEYNAME_LITERAL) {
            திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        மாறிலி கட்டமைப்பு xkb_சாவி * மாறிலி சாவி =
            Xkb_சாவி_By_Name(keymap, மதிப்பு->சாவி_name.சாவி_name, true);
        எனில் (சாவி == வெற்று) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), சாவி_Name_Text(ctx, மதிப்பு->சாவி_name.சாவி_name));
            திரும்பு (keymap_info->strict & PARSER_NO_FIELD_மதிப்பு_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = சாவி->keycode;
        திரும்பு PARSER_SUCCESS;
    }

    எனில் (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        எண்ணிக்கை xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        எண்ணிக்கை xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, மதிப்பு, &flags, &m);
        எனில் (r != PARSER_SUCCESS)
            திரும்பு r;
        எனில் (flags)
            திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        எனில் (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        இல்லையெனில்
            act->mods &= ~m;
        திரும்பு PARSER_SUCCESS;
    }

    திரும்பு ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

நிலையான எண்ணிக்கை xkb_parser_error
HandleUnsupported(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                        மாறிலி கட்டமைப்பு xkb_mod_set *mods,
                        ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
                        மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
                        கோவை_Def **மதிப்பு_ptr)

{
    /* Do not check any field */
    திரும்பு PARSER_SUCCESS;
}

நிலையான எண்ணிக்கை xkb_parser_error
HandlePrivate(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
              மாறிலி கட்டமைப்பு xkb_mod_set *mods,
              ஒன்றிப்பு xkb_action *action, எண்ணிக்கை action_field field,
              மாறிலி கோவை_Def *array_ndx, மாறிலி கோவை_Def *மதிப்பு,
              கோவை_Def **மதிப்பு_ptr)
{
    கட்டமைப்பு xkb_context * restrict மாறிலி ctx = keymap_info->keymap.ctx;
    கட்டமைப்பு xkb_private_action *act = &action->priv;

    எனில் (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        எனில் (array_ndx)
            திரும்பு ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        எனில் (!கோவை_Resolve_Integer(ctx, மதிப்பு, &type))
            திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        எனில் (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        எனில் (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((எண்ணிக்கை xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        இல்லையெனில் {
            act->type = (எண்ணிக்கை xkb_action_type) type;
        }

        திரும்பு PARSER_SUCCESS;
    }
    இல்லையெனில் எனில் (field == ACTION_FIELD_DATA) {
        எனில் (array_ndx == வெற்று) {
            xkb_atom_t val = XKB_ATOM_NONE;

            எனில் (!கோவை_Resolve_String(ctx, மதிப்பு, &val))
                திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            மாறிலி எழுத்து *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            எனில் (len < 1 || len > அளவு(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", அளவு(act->data), len);
                திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, அளவு(act->data));
            memcpy(act->data, str, len);
            திரும்பு PARSER_SUCCESS;
        }
        இல்லையெனில் {
            int64_t ndx = 0, datum = 0;

            எனில் (!கோவை_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            எனில் (ndx < 0 || (size_t) ndx >= அளவு(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        அளவு(act->data), ndx);
                திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            எனில் (!கோவை_Resolve_Integer(ctx, மதிப்பு, &datum))
                திரும்பு ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            எனில் (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                திரும்பு (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            திரும்பு PARSER_SUCCESS;
        }
    }

    திரும்பு ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

வகைவரை எண்ணிக்கை xkb_parser_error
        (*actionHandler)(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                         மாறிலி கட்டமைப்பு xkb_mod_set *mods,
                         ஒன்றிப்பு xkb_action *action,
                         எண்ணிக்கை action_field field,
                         மாறிலி கோவை_Def *array_ndx,
                         மாறிலி கோவை_Def *மதிப்பு,
                         கோவை_Def **மதிப்பு_ptr);

நிலையான மாறிலி actionHandler handleAction[ACTION_TYPE_இலக்கம்_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_சாவி] = Handle_Redirect_சாவி,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_இலக்கம்_ENTRIES,
              "Missing action type");

/***====================================================================***/

எண்ணிக்கை xkb_parser_error
HandleActionDef(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info, ActionsInfo *info,
                மாறிலி கட்டமைப்பு xkb_mod_set *mods, கோவை_Def *def,
                ஒன்றிப்பு xkb_action *action)
{
    கட்டமைப்பு xkb_context * restrict மாறிலி ctx = keymap_info->keymap.ctx;
    எனில் (def->common.type != STMT_கோவை_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        திரும்பு PARSER_FATAL_ERROR;
    }

    மாறிலி எழுத்து *action_name = xkb_atom_text(ctx, def->action.name);
    எண்ணிக்கை xkb_action_type handler_type;
    எனில் (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        எனில் (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            திரும்பு PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    எனில் (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    எண்ணிக்கை xkb_parser_error ret = PARSER_SUCCESS;
    க்கு (கோவை_Def *arg = def->action.args; arg != வெற்று;
         arg = (கோவை_Def *) arg->common.next) {
        மாறிலி கோவை_Def *மதிப்பு;
        கோவை_Def **மதிப்பு_ptr = வெற்று;
        கோவை_Def *field, *arrayRtrn;
        மாறிலி எழுத்து *elemRtrn, *fieldRtrn;

        எனில் (arg->common.type == STMT_கோவை_ASSIGN) {
            field = arg->இருமம்.இடது;
            மதிப்பு = arg->இருமம்.வலது;
            மதிப்பு_ptr = &arg->இருமம்.வலது;
        }
        இல்லையெனில் எனில் (arg->common.type == STMT_கோவை_NOT ||
                 arg->common.type == STMT_கோவை_INVERT) {
            field = arg->unary.child;
            மதிப்பு = (மாறிலி கோவை_Def *) &constFalse;
        }
        இல்லையெனில் {
            field = arg;
            மதிப்பு = (மாறிலி கோவை_Def *) &constTrue;
        }

        எனில் (!கோவை_Resolve_Lhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            திரும்பு PARSER_FATAL_ERROR;

        எனில் (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            திரும்பு PARSER_FATAL_ERROR;
        }

        எண்ணிக்கை action_field fieldNdx;
        எனில் (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            எனில் (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                திரும்பு PARSER_FATAL_ERROR;
            } இல்லையெனில் {
                /* Field is just ignored */
                தொடர்;
            }
        }

        தேர்வு (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, மதிப்பு, மதிப்பு_ptr)) {
        நிலை PARSER_FATAL_ERROR:
            திரும்பு PARSER_FATAL_ERROR;
        நிலை PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            நிறுத்து;
        இயல்பு:
            ;
        }
    }

    திரும்பு (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

எண்ணிக்கை xkb_parser_error
SetDefaultActionField(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                      ActionsInfo *info, கட்டமைப்பு xkb_mod_set *mods,
                      மாறிலி எழுத்து *elem, மாறிலி எழுத்து *field, கோவை_Def *array_ndx,
                      கோவை_Def **மதிப்பு_ptr, எண்ணிக்கை merge_mode merge)
{
    மாறிலி கோவை_Def * restrict மாறிலி மதிப்பு = *மதிப்பு_ptr;
    எண்ணிக்கை xkb_action_type action;
    எனில் (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        திரும்பு (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    எண்ணிக்கை action_field action_field;
    எனில் (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        திரும்பு (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ஒன்றிப்பு xkb_action* மாறிலி into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    ஒன்றிப்பு xkb_action from = *into;

    /* Parse action */
    மாறிலி எண்ணிக்கை xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, மதிப்பு, மதிப்பு_ptr
    );

    /* Success is mandatory to update defaults */
    எனில் (ret != PARSER_SUCCESS)
        திரும்பு ret;

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
    எனில் (!action_equal(into, &from)) {
        மாறிலி bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        எனில் (replace)
            *into = from;
    }
    திரும்பு PARSER_SUCCESS;
}
