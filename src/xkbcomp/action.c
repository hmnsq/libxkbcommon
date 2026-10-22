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

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "context.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "text.h"
#অন্তর্ভাব "expr.h"
#অন্তর্ভাব "xkbcomp/ast.h"
#অন্তর্ভাব "action.h"

স্থির ধ্রুব রাশিমালা_Boolean constTrue = {
    .common = { .type = STMT_রাশিমালা_BOOLEAN_LITERAL, .next = রিক্ত },
    .set = true,
};

স্থির ধ্রুব রাশিমালা_Boolean constFalse = {
    .common = { .type = STMT_রাশিমালা_BOOLEAN_LITERAL, .next = রিক্ত },
    .set = false,
};

গণনা action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_চাবি_EVENT,
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
    ACTION_FIELD_মান,
    ACTION_FIELD_CONTROLS,
    ACTION_FIELD_TYPE,
    ACTION_FIELD_গুনতি,
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

শূন্য
InitActionsInfo(ধ্রুব সংরচনা xkb_keymap *keymap, ActionsInfo *info)
{
    প্রতি (গণনা xkb_action_type type = 0; type < ACTION_TYPE_অঙ্ক_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.মান = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_চাবি].redirect.keycode =
        keymap->redirect_চাবি_auto;
}

স্থির ধ্রুব LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_চাবি_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_চাবি_EVENT   },
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
    { "value",            ACTION_FIELD_মান           },
    { "controls",         ACTION_FIELD_CONTROLS        },
    { "ctrls",            ACTION_FIELD_CONTROLS        },
    { "type",             ACTION_FIELD_TYPE            },
    { "count",            ACTION_FIELD_গুনতি           },
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
    { রিক্ত,               0                            }
};

স্থির bool
stringToActionType(ধ্রুব বর্ণ *str, গণনা xkb_action_type *type_rtrn)
{
    unsigned পূর্ণাঙ্ক type = 0;
    ধ্রুব bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (গণনা xkb_action_type) type;
    প্রত্যাগম ret;
}

স্থির bool
stringToField(ধ্রুব বর্ণ *str, গণনা action_field *field_rtrn)
{
    unsigned পূর্ণাঙ্ক field = 0;
    ধ্রুব bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (গণনা action_field) field;
    প্রত্যাগম ret;
}

স্থির ধ্রুব বর্ণ *
fieldText(গণনা action_field field)
{
    প্রত্যাগম Lookup_মান(fieldStrings, field);
}

/***====================================================================***/

স্থির অন্তঃস্থ গণনা xkb_parser_error
ReportMismatch(সংরচনা xkb_context *ctx, গণনা xkb_message_code code,
               গণনা xkb_action_type action, গণনা action_field field,
               ধ্রুব বর্ণ *type, গণনা xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    প্রত্যাগম (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

স্থির অন্তঃস্থ গণনা xkb_parser_error
ReportFormatVersionMismatch(সংরচনা xkb_context *ctx,
                            গণনা xkb_action_type action,
                            গণনা action_field field,
                            গণনা xkb_keymap_format format,
                            ধ্রুব বর্ণ *versions,
                            গণনা xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    প্রত্যাগম (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

স্থির অন্তঃস্থ গণনা xkb_parser_error
ReportIllegal(সংরচনা xkb_context *ctx, গণনা xkb_action_type action,
              গণনা action_field field, গণনা xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    প্রত্যাগম (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

স্থির অন্তঃস্থ গণনা xkb_parser_error
ReportActionNotArray(সংরচনা xkb_context *ctx, গণনা xkb_action_type action,
                     গণনা action_field field,
                     গণনা xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    প্রত্যাগম (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

স্থির গণনা xkb_parser_error
HandleNoAction(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
               ধ্রুব সংরচনা xkb_mod_set *mods,
               সঙ্ঘ xkb_action *action, গণনা action_field field,
               ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
               রাশিমালা_Def **মান_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    প্রত্যাগম (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

স্থির গণনা xkb_parser_error
CheckBooleanFlag(সংরচনা xkb_context *ctx, গণনা xkb_parser_strict_flags strict,
                 গণনা xkb_action_type action, গণনা action_field field,
                 গণনা xkb_action_flags flag, ধ্রুব রাশিমালা_Def *array_ndx,
                 ধ্রুব রাশিমালা_Def *মান, গণনা xkb_action_flags *flags_inout)
{
    bool set = false;

    যদি (array_ndx)
        প্রত্যাগম ReportActionNotArray(ctx, action, field, strict);

    যদি (!রাশিমালা_Resolve_Boolean(ctx, মান, &set))
        প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    যদি (set)
        *flags_inout |= flag;
    অন্যথা
        *flags_inout &= ~flag;

    প্রত্যাগম PARSER_SUCCESS;
}

স্থির গণনা xkb_parser_error
CheckModifierField(সংরচনা xkb_context *ctx, গণনা xkb_parser_strict_flags strict,
                   ধ্রুব সংরচনা xkb_mod_set *mods, গণনা xkb_action_type action,
                   ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
                   গণনা xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    যদি (array_ndx)
        প্রত্যাগম ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    যদি (মান->common.type == STMT_রাশিমালা_IDENT) {
        ধ্রুব বর্ণ *valStr;
        valStr = xkb_atom_text(ctx, মান->ident.ident);
        যদি (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            প্রত্যাগম PARSER_SUCCESS;
        }
    }

    যদি (!রাশিমালা_Resolve_Mod_Mask(ctx, মান, MOD_BOTH, mods, mods_rtrn))
        প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    প্রত্যাগম PARSER_SUCCESS;
}

স্থির ধ্রুব LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { রিক্ত, 0 }
};

স্থির গণনা xkb_parser_error
CheckAffectField(সংরচনা xkb_context *ctx, গণনা xkb_parser_strict_flags strict,
                 গণনা xkb_action_type action, ধ্রুব রাশিমালা_Def *array_ndx,
                 ধ্রুব রাশিমালা_Def *মান, গণনা xkb_action_flags *flags_inout)
{
    যদি (array_ndx)
        প্রত্যাগম ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    যদি (!রাশিমালা_Resolve_Enum(ctx, মান, &flags, lockWhich))
        প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (গণনা xkb_action_flags) flags;
    প্রত্যাগম PARSER_SUCCESS;
}

স্থির গণনা xkb_parser_error
HandleSetLatchLockMods(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                       ধ্রুব সংরচনা xkb_mod_set *mods,
                       সঙ্ঘ xkb_action *action, গণনা action_field field,
                       ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
                       রাশিমালা_Def **মান_ptr)
{
    সংরচনা xkb_context * restrict ধ্রুব ctx = keymap_info->keymap.ctx;
    সংরচনা xkb_mod_action *act = &action->mods;
    ধ্রুব গণনা xkb_action_type type = action->type;

    যদি (field == ACTION_FIELD_MODIFIERS)
        প্রত্যাগম CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, মান, &act->flags, &act->mods.mods);
    যদি (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        প্রতিজ্ঞা(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        যদি (keymap_info->features.mods_unlock_on_press) {
            ধ্রুব গণনা xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, মান, &act->flags
            );
            যদি (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            প্রত্যাগম ret;
        } অন্যথা {
            প্রত্যাগম ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    যদি ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        প্রত্যাগম CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, মান,
                                &act->flags);
    যদি (type == ACTION_TYPE_MOD_LATCH) {
        যদি (field == ACTION_FIELD_LATCH_TO_LOCK)
            প্রত্যাগম CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    মান, &act->flags);
        যদি (field == ACTION_FIELD_LATCH_ON_PRESS) {
            যদি (keymap_info->features.mods_latch_on_press) {
                প্রত্যাগম CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        মান, &act->flags);
            } অন্যথা {
                প্রত্যাগম ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    যদি (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        প্রত্যাগম CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, মান, &act->flags);

    প্রত্যাগম ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

স্থির গণনা xkb_parser_error
CheckGroupField(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                গণনা xkb_action_type action, ধ্রুব রাশিমালা_Def *array_ndx,
                ধ্রুব রাশিমালা_Def *মান, রাশিমালা_Def **মান_ptr,
                গণনা xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    ধ্রুব রাশিমালা_Def *spec;
    xkb_layout_index_t idx = 0;
    গণনা xkb_action_flags flags = *flags_inout;

    যদি (array_ndx)
        প্রত্যাগম ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    যদি (মান->common.type == STMT_রাশিমালা_NEGATE ||
        মান->common.type == STMT_রাশিমালা_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = মান->unary.child;
        মান_ptr = &(*মান_ptr)->unary.child;
    } অন্যথা {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = মান;
    }

    ধ্রুব bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ধ্রুব গণনা xkb_parser_error ret =
        রাশিমালা_Resolve_Group(keymap_info, spec, absolute, &idx, &pending);
    যদি (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        প্রত্যাগম ret;
    }

    যদি (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ধ্রুব darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (সংরচনা pending_computation) {
                .রাশিমালা = *মান_ptr,
                .computed = false,
                .মান = 0,
            }
        );
        *মান_ptr = রিক্ত;
        static_assert(পরিমাণ(pending_index) == পরিমাণ(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } অন্যথা {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        যদি (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } অন্যথা {
            *group_rtrn = (int32_t) idx;
            যদি (মান->common.type == STMT_রাশিমালা_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    প্রত্যাগম PARSER_SUCCESS;
}

স্থির গণনা xkb_parser_error
HandleSetLatchLockGroup(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                        ধ্রুব সংরচনা xkb_mod_set *mods,
                        সঙ্ঘ xkb_action *action, গণনা action_field field,
                        ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
                        রাশিমালা_Def **মান_ptr)
{
    সংরচনা xkb_context * restrict ধ্রুব ctx = keymap_info->keymap.ctx;
    সংরচনা xkb_group_action *act = &action->group;
    ধ্রুব গণনা xkb_action_type type = action->type;

    যদি (field == ACTION_FIELD_GROUP) {
        প্রত্যাগম CheckGroupField(keymap_info, action->type, array_ndx, মান,
                               মান_ptr, &act->flags, &act->group);
    }
    যদি ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        প্রত্যাগম CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, মান,
                                &act->flags);
    যদি (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        প্রত্যাগম CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, মান,
                                &act->flags);
    যদি (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        যদি (keymap_info->features.group_lock_on_release) {
            ধ্রুব গণনা xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, মান, &act->flags
            );
            যদি (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            প্রত্যাগম ret;
        } অন্যথা {
            প্রত্যাগম ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    প্রত্যাগম ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

স্থির গণনা xkb_parser_error
HandleMovePtr(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
              ধ্রুব সংরচনা xkb_mod_set *mods,
              সঙ্ঘ xkb_action *action, গণনা action_field field,
              ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
              রাশিমালা_Def **মান_ptr)
{
    সংরচনা xkb_context * restrict ধ্রুব ctx = keymap_info->keymap.ctx;
    সংরচনা xkb_pointer_motion_action *act = &action->ptr;

    যদি (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        ধ্রুব bool absolute = (মান->common.type != STMT_রাশিমালা_NEGATE &&
                               মান->common.type != STMT_রাশিমালা_UNARY_PLUS);

        যদি (array_ndx)
            প্রত্যাগম ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        যদি (!রাশিমালা_Resolve_Integer(ctx, মান, &val))
            প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        যদি (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        যদি (field == ACTION_FIELD_X) {
            যদি (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        অন্যথা {
            যদি (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        প্রত্যাগম PARSER_SUCCESS;
    }
    অন্যথা যদি (field == ACTION_FIELD_REPEAT) {
        প্রত্যাগম CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, মান, &act->flags);
    }

    প্রত্যাগম ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

স্থির গণনা xkb_parser_error
HandlePtrBtn(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
             ধ্রুব সংরচনা xkb_mod_set *mods,
             সঙ্ঘ xkb_action *action, গণনা action_field field,
             ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
             রাশিমালা_Def **মান_ptr)
{
    সংরচনা xkb_context * restrict ধ্রুব ctx = keymap_info->keymap.ctx;
    সংরচনা xkb_pointer_button_action *act = &action->btn;

    যদি (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        যদি (array_ndx)
            প্রত্যাগম ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        যদি (!রাশিমালা_Resolve_Button(ctx, মান, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        যদি (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        প্রত্যাগম PARSER_SUCCESS;
    }
    অন্যথা যদি (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        প্রত্যাগম CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, মান, &act->flags);
    }
    অন্যথা যদি (field == ACTION_FIELD_গুনতি) {
        int64_t val = 0;

        যদি (array_ndx)
            প্রত্যাগম ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        যদি (!রাশিমালা_Resolve_Integer(ctx, মান, &val))
            প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        যদি (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        যদি (action->type != ACTION_TYPE_PTR_LOCK)
            act->গুনতি = (uint8_t) val;
        প্রত্যাগম PARSER_SUCCESS;
    }

    প্রত্যাগম ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

গণনা {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    SET_POINTER_DEFAULT_BUTTON_AFFECT_অঙ্ক_ENTRIES
};

স্থির ধ্রুব LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { রিক্ত, 0 }
};

স্থির গণনা xkb_parser_error
HandleSetPtrDflt(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                 ধ্রুব সংরচনা xkb_mod_set *mods,
                 সঙ্ঘ xkb_action *action, গণনা action_field field,
                 ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
                 রাশিমালা_Def **মান_ptr)
{
    সংরচনা xkb_context * restrict ধ্রুব ctx = keymap_info->keymap.ctx;
    সংরচনা xkb_pointer_default_action *act = &action->dflt;

    যদি (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        যদি (array_ndx)
            প্রত্যাগম ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        যদি (!রাশিমালা_Resolve_Enum(ctx, মান, &val, ptrDflts))
            প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(SET_POINTER_DEFAULT_BUTTON_AFFECT_অঙ্ক_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        প্রত্যাগম PARSER_SUCCESS;
    }
    অন্যথা যদি (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_মান) {
        ধ্রুব রাশিমালা_Def *button;
        int64_t btn = 0;

        যদি (array_ndx)
            প্রত্যাগম ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        যদি (মান->common.type == STMT_রাশিমালা_NEGATE ||
            মান->common.type == STMT_রাশিমালা_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = মান->unary.child;
        }
        অন্যথা {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = মান;
        }

        যদি (!রাশিমালা_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        যদি (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        যদি (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->মান = (int8_t) (মান->common.type == STMT_রাশিমালা_NEGATE ? -btn: btn);
        প্রত্যাগম PARSER_SUCCESS;
    }

    প্রত্যাগম ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

স্থির গণনা xkb_parser_error
HandleSwitchScreen(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                   ধ্রুব সংরচনা xkb_mod_set *mods,
                   সঙ্ঘ xkb_action *action, গণনা action_field field,
                   ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
                   রাশিমালা_Def **মান_ptr)
{
    সংরচনা xkb_context * restrict ধ্রুব ctx = keymap_info->keymap.ctx;
    সংরচনা xkb_switch_screen_action *act = &action->screen;

    যদি (field == ACTION_FIELD_SCREEN) {
        ধ্রুব রাশিমালা_Def *scrn;
        int64_t val = 0;

        যদি (array_ndx)
            প্রত্যাগম ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        যদি (মান->common.type == STMT_রাশিমালা_NEGATE ||
            মান->common.type == STMT_রাশিমালা_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = মান->unary.child;
        }
        অন্যথা {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = মান;
        }

        যদি (!রাশিমালা_Resolve_Integer(ctx, scrn, &val))
            প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (মান->common.type == STMT_রাশিমালা_NEGATE ? -val : val);
        যদি (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        প্রত্যাগম PARSER_SUCCESS;
    }
    অন্যথা যদি (field == ACTION_FIELD_SAME) {
        প্রত্যাগম CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, মান,
                                &act->flags);
    }

    প্রত্যাগম ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

স্থির গণনা xkb_parser_error
HandleSetLockControls(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                      ধ্রুব সংরচনা xkb_mod_set *mods,
                      সঙ্ঘ xkb_action *action, গণনা action_field field,
                      ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
                      রাশিমালা_Def **মান_ptr)
{
    সংরচনা xkb_context * restrict ধ্রুব ctx = keymap_info->keymap.ctx;
    সংরচনা xkb_controls_action *act = &action->ctrls;

    যদি (field == ACTION_FIELD_CONTROLS) {
        যদি (array_ndx)
            প্রত্যাগম ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        ধ্রুব uint8_t offset = keymap_info->features.controls_name_offset;
        যদি (!রাশিমালা_Resolve_Mask(ctx, মান, &mask, ctrlMaskNames + offset))
            প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (গণনা xkb_action_controls) mask;
        প্রত্যাগম PARSER_SUCCESS;
    }
    অন্যথা যদি (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        প্রত্যাগম CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, মান, &act->flags);
    }

    প্রত্যাগম ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

স্থির গণনা xkb_parser_error
Handle_Redirect_চাবি(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                  ধ্রুব সংরচনা xkb_mod_set *mods,
                  সঙ্ঘ xkb_action *action, গণনা action_field field,
                  ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
                  রাশিমালা_Def **মান_ptr)
{
    ধ্রুব সংরচনা xkb_keymap * restrict ধ্রুব keymap = &keymap_info->keymap;
    সংরচনা xkb_context * restrict ধ্রুব ctx = keymap->ctx;
    সংরচনা xkb_redirect_চাবি_action * ধ্রুব act = &action->redirect;

    যদি (field == ACTION_FIELD_KEYCODE) {
        যদি (array_ndx)
            প্রত্যাগম ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        যদি (মান->common.type == STMT_রাশিমালা_IDENT) {
            ধ্রুব বর্ণ *valStr = xkb_atom_text(ctx, মান->ident.ident);
            যদি (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_চাবি_auto;
                প্রত্যাগম PARSER_SUCCESS;
            }
        }
        যদি (মান->common.type != STMT_রাশিমালা_KEYNAME_LITERAL) {
            প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        ধ্রুব সংরচনা xkb_চাবি * ধ্রুব চাবি =
            Xkb_চাবি_By_Name(keymap, মান->চাবি_name.চাবি_name, true);
        যদি (চাবি == রিক্ত) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), চাবি_Name_Text(ctx, মান->চাবি_name.চাবি_name));
            প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_মান_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = চাবি->keycode;
        প্রত্যাগম PARSER_SUCCESS;
    }

    যদি (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        গণনা xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        গণনা xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, মান, &flags, &m);
        যদি (r != PARSER_SUCCESS)
            প্রত্যাগম r;
        যদি (flags)
            প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        যদি (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        অন্যথা
            act->mods &= ~m;
        প্রত্যাগম PARSER_SUCCESS;
    }

    প্রত্যাগম ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

স্থির গণনা xkb_parser_error
HandleUnsupported(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                        ধ্রুব সংরচনা xkb_mod_set *mods,
                        সঙ্ঘ xkb_action *action, গণনা action_field field,
                        ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
                        রাশিমালা_Def **মান_ptr)

{
    /* Do not check any field */
    প্রত্যাগম PARSER_SUCCESS;
}

স্থির গণনা xkb_parser_error
HandlePrivate(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
              ধ্রুব সংরচনা xkb_mod_set *mods,
              সঙ্ঘ xkb_action *action, গণনা action_field field,
              ধ্রুব রাশিমালা_Def *array_ndx, ধ্রুব রাশিমালা_Def *মান,
              রাশিমালা_Def **মান_ptr)
{
    সংরচনা xkb_context * restrict ধ্রুব ctx = keymap_info->keymap.ctx;
    সংরচনা xkb_private_action *act = &action->priv;

    যদি (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        যদি (array_ndx)
            প্রত্যাগম ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        যদি (!রাশিমালা_Resolve_Integer(ctx, মান, &type))
            প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        যদি (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        যদি (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((গণনা xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        অন্যথা {
            act->type = (গণনা xkb_action_type) type;
        }

        প্রত্যাগম PARSER_SUCCESS;
    }
    অন্যথা যদি (field == ACTION_FIELD_DATA) {
        যদি (array_ndx == রিক্ত) {
            xkb_atom_t val = XKB_ATOM_NONE;

            যদি (!রাশিমালা_Resolve_String(ctx, মান, &val))
                প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            ধ্রুব বর্ণ *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            যদি (len < 1 || len > পরিমাণ(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", পরিমাণ(act->data), len);
                প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, পরিমাণ(act->data));
            memcpy(act->data, str, len);
            প্রত্যাগম PARSER_SUCCESS;
        }
        অন্যথা {
            int64_t ndx = 0, datum = 0;

            যদি (!রাশিমালা_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            যদি (ndx < 0 || (size_t) ndx >= পরিমাণ(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        পরিমাণ(act->data), ndx);
                প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            যদি (!রাশিমালা_Resolve_Integer(ctx, মান, &datum))
                প্রত্যাগম ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            যদি (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                প্রত্যাগম (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            প্রত্যাগম PARSER_SUCCESS;
        }
    }

    প্রত্যাগম ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

প্রকারনির্দেশ গণনা xkb_parser_error
        (*actionHandler)(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                         ধ্রুব সংরচনা xkb_mod_set *mods,
                         সঙ্ঘ xkb_action *action,
                         গণনা action_field field,
                         ধ্রুব রাশিমালা_Def *array_ndx,
                         ধ্রুব রাশিমালা_Def *মান,
                         রাশিমালা_Def **মান_ptr);

স্থির ধ্রুব actionHandler handleAction[ACTION_TYPE_অঙ্ক_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_চাবি] = Handle_Redirect_চাবি,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_অঙ্ক_ENTRIES,
              "Missing action type");

/***====================================================================***/

গণনা xkb_parser_error
HandleActionDef(ধ্রুব সংরচনা xkb_keymap_info *keymap_info, ActionsInfo *info,
                ধ্রুব সংরচনা xkb_mod_set *mods, রাশিমালা_Def *def,
                সঙ্ঘ xkb_action *action)
{
    সংরচনা xkb_context * restrict ধ্রুব ctx = keymap_info->keymap.ctx;
    যদি (def->common.type != STMT_রাশিমালা_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        প্রত্যাগম PARSER_FATAL_ERROR;
    }

    ধ্রুব বর্ণ *action_name = xkb_atom_text(ctx, def->action.name);
    গণনা xkb_action_type handler_type;
    যদি (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        যদি (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            প্রত্যাগম PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    যদি (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    গণনা xkb_parser_error ret = PARSER_SUCCESS;
    প্রতি (রাশিমালা_Def *arg = def->action.args; arg != রিক্ত;
         arg = (রাশিমালা_Def *) arg->common.next) {
        ধ্রুব রাশিমালা_Def *মান;
        রাশিমালা_Def **মান_ptr = রিক্ত;
        রাশিমালা_Def *field, *arrayRtrn;
        ধ্রুব বর্ণ *elemRtrn, *fieldRtrn;

        যদি (arg->common.type == STMT_রাশিমালা_ASSIGN) {
            field = arg->দ্বিমিক.বাম;
            মান = arg->দ্বিমিক.ডান;
            মান_ptr = &arg->দ্বিমিক.ডান;
        }
        অন্যথা যদি (arg->common.type == STMT_রাশিমালা_NOT ||
                 arg->common.type == STMT_রাশিমালা_INVERT) {
            field = arg->unary.child;
            মান = (ধ্রুব রাশিমালা_Def *) &constFalse;
        }
        অন্যথা {
            field = arg;
            মান = (ধ্রুব রাশিমালা_Def *) &constTrue;
        }

        যদি (!রাশিমালা_Resolve_Lhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            প্রত্যাগম PARSER_FATAL_ERROR;

        যদি (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            প্রত্যাগম PARSER_FATAL_ERROR;
        }

        গণনা action_field fieldNdx;
        যদি (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            যদি (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                প্রত্যাগম PARSER_FATAL_ERROR;
            } অন্যথা {
                /* Field is just ignored */
                অনুবর্ত;
            }
        }

        বিকল্প (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, মান, মান_ptr)) {
        পক্ষ PARSER_FATAL_ERROR:
            প্রত্যাগম PARSER_FATAL_ERROR;
        পক্ষ PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            ভঙ্গ;
        স্বভাব:
            ;
        }
    }

    প্রত্যাগম (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

গণনা xkb_parser_error
SetDefaultActionField(ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
                      ActionsInfo *info, সংরচনা xkb_mod_set *mods,
                      ধ্রুব বর্ণ *elem, ধ্রুব বর্ণ *field, রাশিমালা_Def *array_ndx,
                      রাশিমালা_Def **মান_ptr, গণনা merge_mode merge)
{
    ধ্রুব রাশিমালা_Def * restrict ধ্রুব মান = *মান_ptr;
    গণনা xkb_action_type action;
    যদি (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        প্রত্যাগম (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    গণনা action_field action_field;
    যদি (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        প্রত্যাগম (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    সঙ্ঘ xkb_action* ধ্রুব into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    সঙ্ঘ xkb_action from = *into;

    /* Parse action */
    ধ্রুব গণনা xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, মান, মান_ptr
    );

    /* Success is mandatory to update defaults */
    যদি (ret != PARSER_SUCCESS)
        প্রত্যাগম ret;

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
    যদি (!action_equal(into, &from)) {
        ধ্রুব bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        যদি (replace)
            *into = from;
    }
    প্রত্যাগম PARSER_SUCCESS;
}
