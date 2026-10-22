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

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "expr.h"
#अन्तर्भाव "xkbcomp/ast.h"
#अन्तर्भाव "action.h"

स्थिर ध्रुव अभिव्यक्ती_Boolean constTrue = {
    .common = { .type = STMT_अभिव्यक्ती_BOOLEAN_LITERAL, .next = रिक्त },
    .set = true,
};

स्थिर ध्रुव अभिव्यक्ती_Boolean constFalse = {
    .common = { .type = STMT_अभिव्यक्ती_BOOLEAN_LITERAL, .next = रिक्त },
    .set = false,
};

गणना action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_किल्ली_EVENT,
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
    ACTION_FIELD_मूल्य,
    ACTION_FIELD_CONTROLS,
    ACTION_FIELD_TYPE,
    ACTION_FIELD_मोजणी,
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

शून्य
InitActionsInfo(ध्रुव संरचना xkb_keymap *keymap, ActionsInfo *info)
{
    प्रति (गणना xkb_action_type type = 0; type < ACTION_TYPE_अंक_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.मूल्य = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_किल्ली].redirect.keycode =
        keymap->redirect_किल्ली_auto;
}

स्थिर ध्रुव LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_किल्ली_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_किल्ली_EVENT   },
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
    { "value",            ACTION_FIELD_मूल्य           },
    { "controls",         ACTION_FIELD_CONTROLS        },
    { "ctrls",            ACTION_FIELD_CONTROLS        },
    { "type",             ACTION_FIELD_TYPE            },
    { "count",            ACTION_FIELD_मोजणी           },
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
    { रिक्त,               0                            }
};

स्थिर bool
stringToActionType(ध्रुव वर्ण *str, गणना xkb_action_type *type_rtrn)
{
    unsigned पूर्णाङ्क type = 0;
    ध्रुव bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (गणना xkb_action_type) type;
    प्रत्यागम ret;
}

स्थिर bool
stringToField(ध्रुव वर्ण *str, गणना action_field *field_rtrn)
{
    unsigned पूर्णाङ्क field = 0;
    ध्रुव bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (गणना action_field) field;
    प्रत्यागम ret;
}

स्थिर ध्रुव वर्ण *
fieldText(गणना action_field field)
{
    प्रत्यागम Lookup_मूल्य(fieldStrings, field);
}

/***====================================================================***/

स्थिर अन्तःस्थ गणना xkb_parser_error
ReportMismatch(संरचना xkb_context *ctx, गणना xkb_message_code code,
               गणना xkb_action_type action, गणना action_field field,
               ध्रुव वर्ण *type, गणना xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    प्रत्यागम (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

स्थिर अन्तःस्थ गणना xkb_parser_error
ReportFormatVersionMismatch(संरचना xkb_context *ctx,
                            गणना xkb_action_type action,
                            गणना action_field field,
                            गणना xkb_keymap_format format,
                            ध्रुव वर्ण *versions,
                            गणना xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    प्रत्यागम (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

स्थिर अन्तःस्थ गणना xkb_parser_error
ReportIllegal(संरचना xkb_context *ctx, गणना xkb_action_type action,
              गणना action_field field, गणना xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    प्रत्यागम (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

स्थिर अन्तःस्थ गणना xkb_parser_error
ReportActionNotArray(संरचना xkb_context *ctx, गणना xkb_action_type action,
                     गणना action_field field,
                     गणना xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    प्रत्यागम (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

स्थिर गणना xkb_parser_error
HandleNoAction(ध्रुव संरचना xkb_keymap_info *keymap_info,
               ध्रुव संरचना xkb_mod_set *mods,
               सङ्घ xkb_action *action, गणना action_field field,
               ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
               अभिव्यक्ती_Def **मूल्य_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    प्रत्यागम (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

स्थिर गणना xkb_parser_error
CheckBooleanFlag(संरचना xkb_context *ctx, गणना xkb_parser_strict_flags strict,
                 गणना xkb_action_type action, गणना action_field field,
                 गणना xkb_action_flags flag, ध्रुव अभिव्यक्ती_Def *array_ndx,
                 ध्रुव अभिव्यक्ती_Def *मूल्य, गणना xkb_action_flags *flags_inout)
{
    bool set = false;

    यदि (array_ndx)
        प्रत्यागम ReportActionNotArray(ctx, action, field, strict);

    यदि (!अभिव्यक्ती_Resolve_Boolean(ctx, मूल्य, &set))
        प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    यदि (set)
        *flags_inout |= flag;
    अन्यथा
        *flags_inout &= ~flag;

    प्रत्यागम PARSER_SUCCESS;
}

स्थिर गणना xkb_parser_error
CheckModifierField(संरचना xkb_context *ctx, गणना xkb_parser_strict_flags strict,
                   ध्रुव संरचना xkb_mod_set *mods, गणना xkb_action_type action,
                   ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
                   गणना xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    यदि (array_ndx)
        प्रत्यागम ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    यदि (मूल्य->common.type == STMT_अभिव्यक्ती_IDENT) {
        ध्रुव वर्ण *valStr;
        valStr = xkb_atom_text(ctx, मूल्य->ident.ident);
        यदि (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            प्रत्यागम PARSER_SUCCESS;
        }
    }

    यदि (!अभिव्यक्ती_Resolve_Mod_Mask(ctx, मूल्य, MOD_BOTH, mods, mods_rtrn))
        प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    प्रत्यागम PARSER_SUCCESS;
}

स्थिर ध्रुव LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { रिक्त, 0 }
};

स्थिर गणना xkb_parser_error
CheckAffectField(संरचना xkb_context *ctx, गणना xkb_parser_strict_flags strict,
                 गणना xkb_action_type action, ध्रुव अभिव्यक्ती_Def *array_ndx,
                 ध्रुव अभिव्यक्ती_Def *मूल्य, गणना xkb_action_flags *flags_inout)
{
    यदि (array_ndx)
        प्रत्यागम ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    यदि (!अभिव्यक्ती_Resolve_Enum(ctx, मूल्य, &flags, lockWhich))
        प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (गणना xkb_action_flags) flags;
    प्रत्यागम PARSER_SUCCESS;
}

स्थिर गणना xkb_parser_error
HandleSetLatchLockMods(ध्रुव संरचना xkb_keymap_info *keymap_info,
                       ध्रुव संरचना xkb_mod_set *mods,
                       सङ्घ xkb_action *action, गणना action_field field,
                       ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
                       अभिव्यक्ती_Def **मूल्य_ptr)
{
    संरचना xkb_context * restrict ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_mod_action *act = &action->mods;
    ध्रुव गणना xkb_action_type type = action->type;

    यदि (field == ACTION_FIELD_MODIFIERS)
        प्रत्यागम CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, मूल्य, &act->flags, &act->mods.mods);
    यदि (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        प्रतिज्ञा(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        यदि (keymap_info->features.mods_unlock_on_press) {
            ध्रुव गणना xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, मूल्य, &act->flags
            );
            यदि (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            प्रत्यागम ret;
        } अन्यथा {
            प्रत्यागम ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    यदि ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, मूल्य,
                                &act->flags);
    यदि (type == ACTION_TYPE_MOD_LATCH) {
        यदि (field == ACTION_FIELD_LATCH_TO_LOCK)
            प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    मूल्य, &act->flags);
        यदि (field == ACTION_FIELD_LATCH_ON_PRESS) {
            यदि (keymap_info->features.mods_latch_on_press) {
                प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        मूल्य, &act->flags);
            } अन्यथा {
                प्रत्यागम ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    यदि (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        प्रत्यागम CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, मूल्य, &act->flags);

    प्रत्यागम ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
CheckGroupField(ध्रुव संरचना xkb_keymap_info *keymap_info,
                गणना xkb_action_type action, ध्रुव अभिव्यक्ती_Def *array_ndx,
                ध्रुव अभिव्यक्ती_Def *मूल्य, अभिव्यक्ती_Def **मूल्य_ptr,
                गणना xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    ध्रुव अभिव्यक्ती_Def *spec;
    xkb_layout_index_t idx = 0;
    गणना xkb_action_flags flags = *flags_inout;

    यदि (array_ndx)
        प्रत्यागम ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    यदि (मूल्य->common.type == STMT_अभिव्यक्ती_NEGATE ||
        मूल्य->common.type == STMT_अभिव्यक्ती_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = मूल्य->unary.child;
        मूल्य_ptr = &(*मूल्य_ptr)->unary.child;
    } अन्यथा {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = मूल्य;
    }

    ध्रुव bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ध्रुव गणना xkb_parser_error ret =
        अभिव्यक्ती_Resolve_Group(keymap_info, spec, absolute, &idx, &pending);
    यदि (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        प्रत्यागम ret;
    }

    यदि (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ध्रुव darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (संरचना pending_computation) {
                .अभिव्यक्ती = *मूल्य_ptr,
                .computed = false,
                .मूल्य = 0,
            }
        );
        *मूल्य_ptr = रिक्त;
        static_assert(परिमाण(pending_index) == परिमाण(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } अन्यथा {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        यदि (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } अन्यथा {
            *group_rtrn = (int32_t) idx;
            यदि (मूल्य->common.type == STMT_अभिव्यक्ती_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    प्रत्यागम PARSER_SUCCESS;
}

स्थिर गणना xkb_parser_error
HandleSetLatchLockGroup(ध्रुव संरचना xkb_keymap_info *keymap_info,
                        ध्रुव संरचना xkb_mod_set *mods,
                        सङ्घ xkb_action *action, गणना action_field field,
                        ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
                        अभिव्यक्ती_Def **मूल्य_ptr)
{
    संरचना xkb_context * restrict ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_group_action *act = &action->group;
    ध्रुव गणना xkb_action_type type = action->type;

    यदि (field == ACTION_FIELD_GROUP) {
        प्रत्यागम CheckGroupField(keymap_info, action->type, array_ndx, मूल्य,
                               मूल्य_ptr, &act->flags, &act->group);
    }
    यदि ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, मूल्य,
                                &act->flags);
    यदि (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, मूल्य,
                                &act->flags);
    यदि (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        यदि (keymap_info->features.group_lock_on_release) {
            ध्रुव गणना xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, मूल्य, &act->flags
            );
            यदि (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            प्रत्यागम ret;
        } अन्यथा {
            प्रत्यागम ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    प्रत्यागम ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
HandleMovePtr(ध्रुव संरचना xkb_keymap_info *keymap_info,
              ध्रुव संरचना xkb_mod_set *mods,
              सङ्घ xkb_action *action, गणना action_field field,
              ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
              अभिव्यक्ती_Def **मूल्य_ptr)
{
    संरचना xkb_context * restrict ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_pointer_motion_action *act = &action->ptr;

    यदि (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        ध्रुव bool absolute = (मूल्य->common.type != STMT_अभिव्यक्ती_NEGATE &&
                               मूल्य->common.type != STMT_अभिव्यक्ती_UNARY_PLUS);

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        यदि (!अभिव्यक्ती_Resolve_Integer(ctx, मूल्य, &val))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        यदि (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        यदि (field == ACTION_FIELD_X) {
            यदि (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        अन्यथा {
            यदि (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (field == ACTION_FIELD_REPEAT) {
        प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, मूल्य, &act->flags);
    }

    प्रत्यागम ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
HandlePtrBtn(ध्रुव संरचना xkb_keymap_info *keymap_info,
             ध्रुव संरचना xkb_mod_set *mods,
             सङ्घ xkb_action *action, गणना action_field field,
             ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
             अभिव्यक्ती_Def **मूल्य_ptr)
{
    संरचना xkb_context * restrict ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_pointer_button_action *act = &action->btn;

    यदि (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        यदि (!अभिव्यक्ती_Resolve_Button(ctx, मूल्य, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        यदि (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        प्रत्यागम CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, मूल्य, &act->flags);
    }
    अन्यथा यदि (field == ACTION_FIELD_मोजणी) {
        int64_t val = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        यदि (!अभिव्यक्ती_Resolve_Integer(ctx, मूल्य, &val))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        यदि (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        यदि (action->type != ACTION_TYPE_PTR_LOCK)
            act->मोजणी = (uint8_t) val;
        प्रत्यागम PARSER_SUCCESS;
    }

    प्रत्यागम ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

गणना {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    SET_POINTER_DEFAULT_BUTTON_AFFECT_अंक_ENTRIES
};

स्थिर ध्रुव LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { रिक्त, 0 }
};

स्थिर गणना xkb_parser_error
HandleSetPtrDflt(ध्रुव संरचना xkb_keymap_info *keymap_info,
                 ध्रुव संरचना xkb_mod_set *mods,
                 सङ्घ xkb_action *action, गणना action_field field,
                 ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
                 अभिव्यक्ती_Def **मूल्य_ptr)
{
    संरचना xkb_context * restrict ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_pointer_default_action *act = &action->dflt;

    यदि (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        यदि (!अभिव्यक्ती_Resolve_Enum(ctx, मूल्य, &val, ptrDflts))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(SET_POINTER_DEFAULT_BUTTON_AFFECT_अंक_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_मूल्य) {
        ध्रुव अभिव्यक्ती_Def *button;
        int64_t btn = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        यदि (मूल्य->common.type == STMT_अभिव्यक्ती_NEGATE ||
            मूल्य->common.type == STMT_अभिव्यक्ती_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = मूल्य->unary.child;
        }
        अन्यथा {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = मूल्य;
        }

        यदि (!अभिव्यक्ती_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        यदि (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        यदि (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->मूल्य = (int8_t) (मूल्य->common.type == STMT_अभिव्यक्ती_NEGATE ? -btn: btn);
        प्रत्यागम PARSER_SUCCESS;
    }

    प्रत्यागम ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
HandleSwitchScreen(ध्रुव संरचना xkb_keymap_info *keymap_info,
                   ध्रुव संरचना xkb_mod_set *mods,
                   सङ्घ xkb_action *action, गणना action_field field,
                   ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
                   अभिव्यक्ती_Def **मूल्य_ptr)
{
    संरचना xkb_context * restrict ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_switch_screen_action *act = &action->screen;

    यदि (field == ACTION_FIELD_SCREEN) {
        ध्रुव अभिव्यक्ती_Def *scrn;
        int64_t val = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        यदि (मूल्य->common.type == STMT_अभिव्यक्ती_NEGATE ||
            मूल्य->common.type == STMT_अभिव्यक्ती_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = मूल्य->unary.child;
        }
        अन्यथा {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = मूल्य;
        }

        यदि (!अभिव्यक्ती_Resolve_Integer(ctx, scrn, &val))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (मूल्य->common.type == STMT_अभिव्यक्ती_NEGATE ? -val : val);
        यदि (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (field == ACTION_FIELD_SAME) {
        प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, मूल्य,
                                &act->flags);
    }

    प्रत्यागम ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
HandleSetLockControls(ध्रुव संरचना xkb_keymap_info *keymap_info,
                      ध्रुव संरचना xkb_mod_set *mods,
                      सङ्घ xkb_action *action, गणना action_field field,
                      ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
                      अभिव्यक्ती_Def **मूल्य_ptr)
{
    संरचना xkb_context * restrict ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_controls_action *act = &action->ctrls;

    यदि (field == ACTION_FIELD_CONTROLS) {
        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        ध्रुव uint8_t offset = keymap_info->features.controls_name_offset;
        यदि (!अभिव्यक्ती_Resolve_Mask(ctx, मूल्य, &mask, ctrlMaskNames + offset))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (गणना xkb_action_controls) mask;
        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        प्रत्यागम CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, मूल्य, &act->flags);
    }

    प्रत्यागम ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
Handle_Redirect_किल्ली(ध्रुव संरचना xkb_keymap_info *keymap_info,
                  ध्रुव संरचना xkb_mod_set *mods,
                  सङ्घ xkb_action *action, गणना action_field field,
                  ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
                  अभिव्यक्ती_Def **मूल्य_ptr)
{
    ध्रुव संरचना xkb_keymap * restrict ध्रुव keymap = &keymap_info->keymap;
    संरचना xkb_context * restrict ध्रुव ctx = keymap->ctx;
    संरचना xkb_redirect_किल्ली_action * ध्रुव act = &action->redirect;

    यदि (field == ACTION_FIELD_KEYCODE) {
        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        यदि (मूल्य->common.type == STMT_अभिव्यक्ती_IDENT) {
            ध्रुव वर्ण *valStr = xkb_atom_text(ctx, मूल्य->ident.ident);
            यदि (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_किल्ली_auto;
                प्रत्यागम PARSER_SUCCESS;
            }
        }
        यदि (मूल्य->common.type != STMT_अभिव्यक्ती_KEYNAME_LITERAL) {
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        ध्रुव संरचना xkb_किल्ली * ध्रुव किल्ली =
            Xkb_किल्ली_By_Name(keymap, मूल्य->किल्ली_name.किल्ली_name, true);
        यदि (किल्ली == रिक्त) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), किल्ली_Name_Text(ctx, मूल्य->किल्ली_name.किल्ली_name));
            प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_मूल्य_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = किल्ली->keycode;
        प्रत्यागम PARSER_SUCCESS;
    }

    यदि (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        गणना xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        गणना xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, मूल्य, &flags, &m);
        यदि (r != PARSER_SUCCESS)
            प्रत्यागम r;
        यदि (flags)
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        यदि (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        अन्यथा
            act->mods &= ~m;
        प्रत्यागम PARSER_SUCCESS;
    }

    प्रत्यागम ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
HandleUnsupported(ध्रुव संरचना xkb_keymap_info *keymap_info,
                        ध्रुव संरचना xkb_mod_set *mods,
                        सङ्घ xkb_action *action, गणना action_field field,
                        ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
                        अभिव्यक्ती_Def **मूल्य_ptr)

{
    /* Do not check any field */
    प्रत्यागम PARSER_SUCCESS;
}

स्थिर गणना xkb_parser_error
HandlePrivate(ध्रुव संरचना xkb_keymap_info *keymap_info,
              ध्रुव संरचना xkb_mod_set *mods,
              सङ्घ xkb_action *action, गणना action_field field,
              ध्रुव अभिव्यक्ती_Def *array_ndx, ध्रुव अभिव्यक्ती_Def *मूल्य,
              अभिव्यक्ती_Def **मूल्य_ptr)
{
    संरचना xkb_context * restrict ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_private_action *act = &action->priv;

    यदि (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        यदि (!अभिव्यक्ती_Resolve_Integer(ctx, मूल्य, &type))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        यदि (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        यदि (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((गणना xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        अन्यथा {
            act->type = (गणना xkb_action_type) type;
        }

        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (field == ACTION_FIELD_DATA) {
        यदि (array_ndx == रिक्त) {
            xkb_atom_t val = XKB_ATOM_NONE;

            यदि (!अभिव्यक्ती_Resolve_String(ctx, मूल्य, &val))
                प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            ध्रुव वर्ण *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            यदि (len < 1 || len > परिमाण(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", परिमाण(act->data), len);
                प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, परिमाण(act->data));
            memcpy(act->data, str, len);
            प्रत्यागम PARSER_SUCCESS;
        }
        अन्यथा {
            int64_t ndx = 0, datum = 0;

            यदि (!अभिव्यक्ती_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            यदि (ndx < 0 || (size_t) ndx >= परिमाण(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        परिमाण(act->data), ndx);
                प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            यदि (!अभिव्यक्ती_Resolve_Integer(ctx, मूल्य, &datum))
                प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            यदि (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                प्रत्यागम (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            प्रत्यागम PARSER_SUCCESS;
        }
    }

    प्रत्यागम ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

प्रकारनिर्देश गणना xkb_parser_error
        (*actionHandler)(ध्रुव संरचना xkb_keymap_info *keymap_info,
                         ध्रुव संरचना xkb_mod_set *mods,
                         सङ्घ xkb_action *action,
                         गणना action_field field,
                         ध्रुव अभिव्यक्ती_Def *array_ndx,
                         ध्रुव अभिव्यक्ती_Def *मूल्य,
                         अभिव्यक्ती_Def **मूल्य_ptr);

स्थिर ध्रुव actionHandler handleAction[ACTION_TYPE_अंक_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_किल्ली] = Handle_Redirect_किल्ली,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
              "Missing action type");

/***====================================================================***/

गणना xkb_parser_error
HandleActionDef(ध्रुव संरचना xkb_keymap_info *keymap_info, ActionsInfo *info,
                ध्रुव संरचना xkb_mod_set *mods, अभिव्यक्ती_Def *def,
                सङ्घ xkb_action *action)
{
    संरचना xkb_context * restrict ध्रुव ctx = keymap_info->keymap.ctx;
    यदि (def->common.type != STMT_अभिव्यक्ती_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        प्रत्यागम PARSER_FATAL_ERROR;
    }

    ध्रुव वर्ण *action_name = xkb_atom_text(ctx, def->action.name);
    गणना xkb_action_type handler_type;
    यदि (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        यदि (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            प्रत्यागम PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    यदि (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    गणना xkb_parser_error ret = PARSER_SUCCESS;
    प्रति (अभिव्यक्ती_Def *arg = def->action.args; arg != रिक्त;
         arg = (अभिव्यक्ती_Def *) arg->common.next) {
        ध्रुव अभिव्यक्ती_Def *मूल्य;
        अभिव्यक्ती_Def **मूल्य_ptr = रिक्त;
        अभिव्यक्ती_Def *field, *arrayRtrn;
        ध्रुव वर्ण *elemRtrn, *fieldRtrn;

        यदि (arg->common.type == STMT_अभिव्यक्ती_ASSIGN) {
            field = arg->द्विमान.डावा;
            मूल्य = arg->द्विमान.उजवा;
            मूल्य_ptr = &arg->द्विमान.उजवा;
        }
        अन्यथा यदि (arg->common.type == STMT_अभिव्यक्ती_NOT ||
                 arg->common.type == STMT_अभिव्यक्ती_INVERT) {
            field = arg->unary.child;
            मूल्य = (ध्रुव अभिव्यक्ती_Def *) &constFalse;
        }
        अन्यथा {
            field = arg;
            मूल्य = (ध्रुव अभिव्यक्ती_Def *) &constTrue;
        }

        यदि (!अभिव्यक्ती_Resolve_Lhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            प्रत्यागम PARSER_FATAL_ERROR;

        यदि (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            प्रत्यागम PARSER_FATAL_ERROR;
        }

        गणना action_field fieldNdx;
        यदि (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            यदि (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                प्रत्यागम PARSER_FATAL_ERROR;
            } अन्यथा {
                /* Field is just ignored */
                अनुवर्त;
            }
        }

        विकल्प (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, मूल्य, मूल्य_ptr)) {
        पक्ष PARSER_FATAL_ERROR:
            प्रत्यागम PARSER_FATAL_ERROR;
        पक्ष PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            भङ्ग;
        स्वभाव:
            ;
        }
    }

    प्रत्यागम (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

गणना xkb_parser_error
SetDefaultActionField(ध्रुव संरचना xkb_keymap_info *keymap_info,
                      ActionsInfo *info, संरचना xkb_mod_set *mods,
                      ध्रुव वर्ण *elem, ध्रुव वर्ण *field, अभिव्यक्ती_Def *array_ndx,
                      अभिव्यक्ती_Def **मूल्य_ptr, गणना merge_mode merge)
{
    ध्रुव अभिव्यक्ती_Def * restrict ध्रुव मूल्य = *मूल्य_ptr;
    गणना xkb_action_type action;
    यदि (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        प्रत्यागम (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    गणना action_field action_field;
    यदि (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        प्रत्यागम (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    सङ्घ xkb_action* ध्रुव into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    सङ्घ xkb_action from = *into;

    /* Parse action */
    ध्रुव गणना xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, मूल्य, मूल्य_ptr
    );

    /* Success is mandatory to update defaults */
    यदि (ret != PARSER_SUCCESS)
        प्रत्यागम ret;

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
    यदि (!action_equal(into, &from)) {
        ध्रुव bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        यदि (replace)
            *into = from;
    }
    प्रत्यागम PARSER_SUCCESS;
}
