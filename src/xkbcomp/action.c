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

#समावेश "config.h"

#समावेश <assert.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "context.h"
#समावेश "darray.h"
#समावेश "keymap.h"
#समावेश "messages-codes.h"
#समावेश "xkbcomp-priv.h"
#समावेश "text.h"
#समावेश "expr.h"
#समावेश "xkbcomp/ast.h"
#समावेश "action.h"

अटल स्थिर व्यंजक_Boolean constTrue = {
    .common = { .type = STMT_व्यंजक_BOOLEAN_LITERAL, .अग्र = शून्य },
    .set = true,
};

अटल स्थिर व्यंजक_Boolean constFalse = {
    .common = { .type = STMT_व्यंजक_BOOLEAN_LITERAL, .अग्र = शून्य },
    .set = false,
};

क्रमागत action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_कुंजी_EVENT,
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
    ACTION_FIELD_गिनती,
    ACTION_FIELD_SCREEN,
    ACTION_FIELD_SAME,
    ACTION_FIELD_तथ्य,
    ACTION_FIELD_DEVICE,
    ACTION_FIELD_KEYCODE,
    ACTION_FIELD_MODS_TO_CLEAR,
    ACTION_FIELD_LOCK_ON_RELEASE,
    ACTION_FIELD_UNLOCK_ON_PRESS,
    ACTION_FIELD_LATCH_ON_PRESS,
};

व्योम
InitActionsInfo(स्थिर काष्ठा xkb_keymap *keymap, ActionsInfo *info)
{
    क्रम (क्रमागत xkb_action_type type = 0; type < ACTION_TYPE_अंक_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.मूल्य = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_कुंजी].redirect.keycode =
        keymap->redirect_कुंजी_auto;
}

अटल स्थिर LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_कुंजी_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_कुंजी_EVENT   },
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
    { "count",            ACTION_FIELD_गिनती           },
    { "screen",           ACTION_FIELD_SCREEN          },
    { "same",             ACTION_FIELD_SAME            },
    { "sameServer",       ACTION_FIELD_SAME            },
    { "data",             ACTION_FIELD_तथ्य            },
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
    { शून्य,               0                            }
};

अटल bool
stringToActionType(स्थिर अक्षर *str, क्रमागत xkb_action_type *type_rtrn)
{
    अचिन्हित पूर्णांक type = 0;
    स्थिर bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (क्रमागत xkb_action_type) type;
    वापस ret;
}

अटल bool
stringToField(स्थिर अक्षर *str, क्रमागत action_field *field_rtrn)
{
    अचिन्हित पूर्णांक field = 0;
    स्थिर bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (क्रमागत action_field) field;
    वापस ret;
}

अटल स्थिर अक्षर *
fieldText(क्रमागत action_field field)
{
    वापस Lookup_मूल्य(fieldStrings, field);
}

/***====================================================================***/

अटल अंतरभूत क्रमागत xkb_parser_error
ReportMismatch(काष्ठा xkb_context *ctx, क्रमागत xkb_message_code code,
               क्रमागत xkb_action_type action, क्रमागत action_field field,
               स्थिर अक्षर *type, क्रमागत xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    वापस (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

अटल अंतरभूत क्रमागत xkb_parser_error
ReportFormatVersionMismatch(काष्ठा xkb_context *ctx,
                            क्रमागत xkb_action_type action,
                            क्रमागत action_field field,
                            क्रमागत xkb_keymap_format format,
                            स्थिर अक्षर *versions,
                            क्रमागत xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    वापस (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

अटल अंतरभूत क्रमागत xkb_parser_error
ReportIllegal(काष्ठा xkb_context *ctx, क्रमागत xkb_action_type action,
              क्रमागत action_field field, क्रमागत xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    वापस (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

अटल अंतरभूत क्रमागत xkb_parser_error
ReportActionNotArray(काष्ठा xkb_context *ctx, क्रमागत xkb_action_type action,
                     क्रमागत action_field field,
                     क्रमागत xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    वापस (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

अटल क्रमागत xkb_parser_error
HandleNoAction(स्थिर काष्ठा xkb_keymap_info *keymap_info,
               स्थिर काष्ठा xkb_mod_set *mods,
               जोड़ xkb_action *action, क्रमागत action_field field,
               स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
               व्यंजक_Def **मूल्य_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    वापस (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

अटल क्रमागत xkb_parser_error
CheckBooleanFlag(काष्ठा xkb_context *ctx, क्रमागत xkb_parser_strict_flags strict,
                 क्रमागत xkb_action_type action, क्रमागत action_field field,
                 क्रमागत xkb_action_flags flag, स्थिर व्यंजक_Def *array_ndx,
                 स्थिर व्यंजक_Def *मूल्य, क्रमागत xkb_action_flags *flags_inout)
{
    bool set = false;

    अगर (array_ndx)
        वापस ReportActionNotArray(ctx, action, field, strict);

    अगर (!व्यंजक_Resolve_Boolean(ctx, मूल्य, &set))
        वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    अगर (set)
        *flags_inout |= flag;
    अन्यथा
        *flags_inout &= ~flag;

    वापस PARSER_SUCCESS;
}

अटल क्रमागत xkb_parser_error
CheckModifierField(काष्ठा xkb_context *ctx, क्रमागत xkb_parser_strict_flags strict,
                   स्थिर काष्ठा xkb_mod_set *mods, क्रमागत xkb_action_type action,
                   स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
                   क्रमागत xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    अगर (array_ndx)
        वापस ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    अगर (मूल्य->common.type == STMT_व्यंजक_IDENT) {
        स्थिर अक्षर *valStr;
        valStr = xkb_atom_text(ctx, मूल्य->ident.ident);
        अगर (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            वापस PARSER_SUCCESS;
        }
    }

    अगर (!व्यंजक_Resolve_Mod_Mask(ctx, मूल्य, MOD_BOTH, mods, mods_rtrn))
        वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    वापस PARSER_SUCCESS;
}

अटल स्थिर LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { शून्य, 0 }
};

अटल क्रमागत xkb_parser_error
CheckAffectField(काष्ठा xkb_context *ctx, क्रमागत xkb_parser_strict_flags strict,
                 क्रमागत xkb_action_type action, स्थिर व्यंजक_Def *array_ndx,
                 स्थिर व्यंजक_Def *मूल्य, क्रमागत xkb_action_flags *flags_inout)
{
    अगर (array_ndx)
        वापस ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    अगर (!व्यंजक_Resolve_Enum(ctx, मूल्य, &flags, lockWhich))
        वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (क्रमागत xkb_action_flags) flags;
    वापस PARSER_SUCCESS;
}

अटल क्रमागत xkb_parser_error
HandleSetLatchLockMods(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                       स्थिर काष्ठा xkb_mod_set *mods,
                       जोड़ xkb_action *action, क्रमागत action_field field,
                       स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
                       व्यंजक_Def **मूल्य_ptr)
{
    काष्ठा xkb_context * restrict स्थिर ctx = keymap_info->keymap.ctx;
    काष्ठा xkb_mod_action *act = &action->mods;
    स्थिर क्रमागत xkb_action_type type = action->type;

    अगर (field == ACTION_FIELD_MODIFIERS)
        वापस CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, मूल्य, &act->flags, &act->mods.mods);
    अगर (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        निश्चित(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        अगर (keymap_info->features.mods_unlock_on_press) {
            स्थिर क्रमागत xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, मूल्य, &act->flags
            );
            अगर (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            वापस ret;
        } अन्यथा {
            वापस ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    अगर ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        वापस CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, मूल्य,
                                &act->flags);
    अगर (type == ACTION_TYPE_MOD_LATCH) {
        अगर (field == ACTION_FIELD_LATCH_TO_LOCK)
            वापस CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    मूल्य, &act->flags);
        अगर (field == ACTION_FIELD_LATCH_ON_PRESS) {
            अगर (keymap_info->features.mods_latch_on_press) {
                वापस CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        मूल्य, &act->flags);
            } अन्यथा {
                वापस ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    अगर (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        वापस CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, मूल्य, &act->flags);

    वापस ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

अटल क्रमागत xkb_parser_error
CheckGroupField(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                क्रमागत xkb_action_type action, स्थिर व्यंजक_Def *array_ndx,
                स्थिर व्यंजक_Def *मूल्य, व्यंजक_Def **मूल्य_ptr,
                क्रमागत xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    स्थिर व्यंजक_Def *spec;
    xkb_layout_सूचकांक_t idx = 0;
    क्रमागत xkb_action_flags flags = *flags_inout;

    अगर (array_ndx)
        वापस ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    अगर (मूल्य->common.type == STMT_व्यंजक_NEGATE ||
        मूल्य->common.type == STMT_व्यंजक_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = मूल्य->unary.child;
        मूल्य_ptr = &(*मूल्य_ptr)->unary.child;
    } अन्यथा {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = मूल्य;
    }

    स्थिर bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    स्थिर क्रमागत xkb_parser_error ret =
        व्यंजक_Resolve_Group(keymap_info, spec, absolute, &idx, &pending);
    अगर (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        वापस ret;
    }

    अगर (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        स्थिर darray_आकार_t pending_सूचकांक =
            darray_आकार(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (काष्ठा pending_computation) {
                .व्यंजक = *मूल्य_ptr,
                .computed = false,
                .मूल्य = 0,
            }
        );
        *मूल्य_ptr = शून्य;
        static_assert(माप(pending_सूचकांक) == माप(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_सूचकांक;
    } अन्यथा {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        अगर (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } अन्यथा {
            *group_rtrn = (int32_t) idx;
            अगर (मूल्य->common.type == STMT_व्यंजक_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    वापस PARSER_SUCCESS;
}

अटल क्रमागत xkb_parser_error
HandleSetLatchLockGroup(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                        स्थिर काष्ठा xkb_mod_set *mods,
                        जोड़ xkb_action *action, क्रमागत action_field field,
                        स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
                        व्यंजक_Def **मूल्य_ptr)
{
    काष्ठा xkb_context * restrict स्थिर ctx = keymap_info->keymap.ctx;
    काष्ठा xkb_group_action *act = &action->group;
    स्थिर क्रमागत xkb_action_type type = action->type;

    अगर (field == ACTION_FIELD_GROUP) {
        वापस CheckGroupField(keymap_info, action->type, array_ndx, मूल्य,
                               मूल्य_ptr, &act->flags, &act->group);
    }
    अगर ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        वापस CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, मूल्य,
                                &act->flags);
    अगर (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        वापस CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, मूल्य,
                                &act->flags);
    अगर (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        अगर (keymap_info->features.group_lock_on_release) {
            स्थिर क्रमागत xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, मूल्य, &act->flags
            );
            अगर (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            वापस ret;
        } अन्यथा {
            वापस ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    वापस ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

अटल क्रमागत xkb_parser_error
HandleMovePtr(स्थिर काष्ठा xkb_keymap_info *keymap_info,
              स्थिर काष्ठा xkb_mod_set *mods,
              जोड़ xkb_action *action, क्रमागत action_field field,
              स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
              व्यंजक_Def **मूल्य_ptr)
{
    काष्ठा xkb_context * restrict स्थिर ctx = keymap_info->keymap.ctx;
    काष्ठा xkb_pointer_motion_action *act = &action->ptr;

    अगर (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        स्थिर bool absolute = (मूल्य->common.type != STMT_व्यंजक_NEGATE &&
                               मूल्य->common.type != STMT_व्यंजक_UNARY_PLUS);

        अगर (array_ndx)
            वापस ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        अगर (!व्यंजक_Resolve_Integer(ctx, मूल्य, &val))
            वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        अगर (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        अगर (field == ACTION_FIELD_X) {
            अगर (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        अन्यथा {
            अगर (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        वापस PARSER_SUCCESS;
    }
    अन्यथा अगर (field == ACTION_FIELD_REPEAT) {
        वापस CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, मूल्य, &act->flags);
    }

    वापस ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

अटल क्रमागत xkb_parser_error
HandlePtrBtn(स्थिर काष्ठा xkb_keymap_info *keymap_info,
             स्थिर काष्ठा xkb_mod_set *mods,
             जोड़ xkb_action *action, क्रमागत action_field field,
             स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
             व्यंजक_Def **मूल्य_ptr)
{
    काष्ठा xkb_context * restrict स्थिर ctx = keymap_info->keymap.ctx;
    काष्ठा xkb_pointer_button_action *act = &action->btn;

    अगर (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        अगर (array_ndx)
            वापस ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        अगर (!व्यंजक_Resolve_Button(ctx, मूल्य, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        अगर (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        वापस PARSER_SUCCESS;
    }
    अन्यथा अगर (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        वापस CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, मूल्य, &act->flags);
    }
    अन्यथा अगर (field == ACTION_FIELD_गिनती) {
        int64_t val = 0;

        अगर (array_ndx)
            वापस ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        अगर (!व्यंजक_Resolve_Integer(ctx, मूल्य, &val))
            वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        अगर (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        अगर (action->type != ACTION_TYPE_PTR_LOCK)
            act->गिनती = (uint8_t) val;
        वापस PARSER_SUCCESS;
    }

    वापस ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

क्रमागत {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    SET_POINTER_DEFAULT_BUTTON_AFFECT_अंक_ENTRIES
};

अटल स्थिर LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { शून्य, 0 }
};

अटल क्रमागत xkb_parser_error
HandleSetPtrDflt(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                 स्थिर काष्ठा xkb_mod_set *mods,
                 जोड़ xkb_action *action, क्रमागत action_field field,
                 स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
                 व्यंजक_Def **मूल्य_ptr)
{
    काष्ठा xkb_context * restrict स्थिर ctx = keymap_info->keymap.ctx;
    काष्ठा xkb_pointer_default_action *act = &action->dflt;

    अगर (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        अगर (array_ndx)
            वापस ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        अगर (!व्यंजक_Resolve_Enum(ctx, मूल्य, &val, ptrDflts))
            वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(SET_POINTER_DEFAULT_BUTTON_AFFECT_अंक_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        वापस PARSER_SUCCESS;
    }
    अन्यथा अगर (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_मूल्य) {
        स्थिर व्यंजक_Def *button;
        int64_t btn = 0;

        अगर (array_ndx)
            वापस ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        अगर (मूल्य->common.type == STMT_व्यंजक_NEGATE ||
            मूल्य->common.type == STMT_व्यंजक_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = मूल्य->unary.child;
        }
        अन्यथा {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = मूल्य;
        }

        अगर (!व्यंजक_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        अगर (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        अगर (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->मूल्य = (int8_t) (मूल्य->common.type == STMT_व्यंजक_NEGATE ? -btn: btn);
        वापस PARSER_SUCCESS;
    }

    वापस ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

अटल क्रमागत xkb_parser_error
HandleSwitchScreen(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                   स्थिर काष्ठा xkb_mod_set *mods,
                   जोड़ xkb_action *action, क्रमागत action_field field,
                   स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
                   व्यंजक_Def **मूल्य_ptr)
{
    काष्ठा xkb_context * restrict स्थिर ctx = keymap_info->keymap.ctx;
    काष्ठा xkb_switch_screen_action *act = &action->screen;

    अगर (field == ACTION_FIELD_SCREEN) {
        स्थिर व्यंजक_Def *scrn;
        int64_t val = 0;

        अगर (array_ndx)
            वापस ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        अगर (मूल्य->common.type == STMT_व्यंजक_NEGATE ||
            मूल्य->common.type == STMT_व्यंजक_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = मूल्य->unary.child;
        }
        अन्यथा {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = मूल्य;
        }

        अगर (!व्यंजक_Resolve_Integer(ctx, scrn, &val))
            वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (मूल्य->common.type == STMT_व्यंजक_NEGATE ? -val : val);
        अगर (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        वापस PARSER_SUCCESS;
    }
    अन्यथा अगर (field == ACTION_FIELD_SAME) {
        वापस CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, मूल्य,
                                &act->flags);
    }

    वापस ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

अटल क्रमागत xkb_parser_error
HandleSetLockControls(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                      स्थिर काष्ठा xkb_mod_set *mods,
                      जोड़ xkb_action *action, क्रमागत action_field field,
                      स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
                      व्यंजक_Def **मूल्य_ptr)
{
    काष्ठा xkb_context * restrict स्थिर ctx = keymap_info->keymap.ctx;
    काष्ठा xkb_controls_action *act = &action->ctrls;

    अगर (field == ACTION_FIELD_CONTROLS) {
        अगर (array_ndx)
            वापस ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        स्थिर uint8_t offset = keymap_info->features.controls_name_offset;
        अगर (!व्यंजक_Resolve_Mask(ctx, मूल्य, &mask, ctrlMaskNames + offset))
            वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (क्रमागत xkb_action_controls) mask;
        वापस PARSER_SUCCESS;
    }
    अन्यथा अगर (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        वापस CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, मूल्य, &act->flags);
    }

    वापस ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

अटल क्रमागत xkb_parser_error
Handle_Redirect_कुंजी(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                  स्थिर काष्ठा xkb_mod_set *mods,
                  जोड़ xkb_action *action, क्रमागत action_field field,
                  स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
                  व्यंजक_Def **मूल्य_ptr)
{
    स्थिर काष्ठा xkb_keymap * restrict स्थिर keymap = &keymap_info->keymap;
    काष्ठा xkb_context * restrict स्थिर ctx = keymap->ctx;
    काष्ठा xkb_redirect_कुंजी_action * स्थिर act = &action->redirect;

    अगर (field == ACTION_FIELD_KEYCODE) {
        अगर (array_ndx)
            वापस ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        अगर (मूल्य->common.type == STMT_व्यंजक_IDENT) {
            स्थिर अक्षर *valStr = xkb_atom_text(ctx, मूल्य->ident.ident);
            अगर (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_कुंजी_auto;
                वापस PARSER_SUCCESS;
            }
        }
        अगर (मूल्य->common.type != STMT_व्यंजक_KEYNAME_LITERAL) {
            वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        स्थिर काष्ठा xkb_कुंजी * स्थिर कुंजी =
            Xkb_कुंजी_By_Name(keymap, मूल्य->कुंजी_name.कुंजी_name, true);
        अगर (कुंजी == शून्य) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), कुंजी_Name_Text(ctx, मूल्य->कुंजी_name.कुंजी_name));
            वापस (keymap_info->strict & PARSER_NO_FIELD_मूल्य_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = कुंजी->keycode;
        वापस PARSER_SUCCESS;
    }

    अगर (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        क्रमागत xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        क्रमागत xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, मूल्य, &flags, &m);
        अगर (r != PARSER_SUCCESS)
            वापस r;
        अगर (flags)
            वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        अगर (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        अन्यथा
            act->mods &= ~m;
        वापस PARSER_SUCCESS;
    }

    वापस ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

अटल क्रमागत xkb_parser_error
HandleUnsupported(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                        स्थिर काष्ठा xkb_mod_set *mods,
                        जोड़ xkb_action *action, क्रमागत action_field field,
                        स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
                        व्यंजक_Def **मूल्य_ptr)

{
    /* Do not check any field */
    वापस PARSER_SUCCESS;
}

अटल क्रमागत xkb_parser_error
HandlePrivate(स्थिर काष्ठा xkb_keymap_info *keymap_info,
              स्थिर काष्ठा xkb_mod_set *mods,
              जोड़ xkb_action *action, क्रमागत action_field field,
              स्थिर व्यंजक_Def *array_ndx, स्थिर व्यंजक_Def *मूल्य,
              व्यंजक_Def **मूल्य_ptr)
{
    काष्ठा xkb_context * restrict स्थिर ctx = keymap_info->keymap.ctx;
    काष्ठा xkb_private_action *act = &action->priv;

    अगर (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        अगर (array_ndx)
            वापस ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        अगर (!व्यंजक_Resolve_Integer(ctx, मूल्य, &type))
            वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        अगर (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        अगर (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((क्रमागत xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        अन्यथा {
            act->type = (क्रमागत xkb_action_type) type;
        }

        वापस PARSER_SUCCESS;
    }
    अन्यथा अगर (field == ACTION_FIELD_तथ्य) {
        अगर (array_ndx == शून्य) {
            xkb_atom_t val = XKB_ATOM_NONE;

            अगर (!व्यंजक_Resolve_String(ctx, मूल्य, &val))
                वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            स्थिर अक्षर *str = xkb_atom_text(ctx, val);
            माप_प्रकार len = म_माप(str);
            अगर (len < 1 || len > माप(act->तथ्य)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", माप(act->तथ्य), len);
                वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            स_रखो(act->तथ्य, 0, माप(act->तथ्य));
            स_नकल(act->तथ्य, str, len);
            वापस PARSER_SUCCESS;
        }
        अन्यथा {
            int64_t ndx = 0, datum = 0;

            अगर (!व्यंजक_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            अगर (ndx < 0 || (माप_प्रकार) ndx >= माप(act->तथ्य)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        माप(act->तथ्य), ndx);
                वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            अगर (!व्यंजक_Resolve_Integer(ctx, मूल्य, &datum))
                वापस ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            अगर (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                वापस (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->तथ्य[ndx] = (uint8_t) datum;
            वापस PARSER_SUCCESS;
        }
    }

    वापस ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

प्रकार क्रमागत xkb_parser_error
        (*actionHandler)(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                         स्थिर काष्ठा xkb_mod_set *mods,
                         जोड़ xkb_action *action,
                         क्रमागत action_field field,
                         स्थिर व्यंजक_Def *array_ndx,
                         स्थिर व्यंजक_Def *मूल्य,
                         व्यंजक_Def **मूल्य_ptr);

अटल स्थिर actionHandler handleAction[ACTION_TYPE_अंक_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_कुंजी] = Handle_Redirect_कुंजी,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
              "Missing action type");

/***====================================================================***/

क्रमागत xkb_parser_error
HandleActionDef(स्थिर काष्ठा xkb_keymap_info *keymap_info, ActionsInfo *info,
                स्थिर काष्ठा xkb_mod_set *mods, व्यंजक_Def *def,
                जोड़ xkb_action *action)
{
    काष्ठा xkb_context * restrict स्थिर ctx = keymap_info->keymap.ctx;
    अगर (def->common.type != STMT_व्यंजक_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        वापस PARSER_FATAL_ERROR;
    }

    स्थिर अक्षर *action_name = xkb_atom_text(ctx, def->action.name);
    क्रमागत xkb_action_type handler_type;
    अगर (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        अगर (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            वापस PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    अगर (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    क्रमागत xkb_parser_error ret = PARSER_SUCCESS;
    क्रम (व्यंजक_Def *arg = def->action.args; arg != शून्य;
         arg = (व्यंजक_Def *) arg->common.अग्र) {
        स्थिर व्यंजक_Def *मूल्य;
        व्यंजक_Def **मूल्य_ptr = शून्य;
        व्यंजक_Def *field, *arrayRtrn;
        स्थिर अक्षर *elemRtrn, *fieldRtrn;

        अगर (arg->common.type == STMT_व्यंजक_ASSIGN) {
            field = arg->द्विआधारी.बायाँ;
            मूल्य = arg->द्विआधारी.दायाँ;
            मूल्य_ptr = &arg->द्विआधारी.दायाँ;
        }
        अन्यथा अगर (arg->common.type == STMT_व्यंजक_NOT ||
                 arg->common.type == STMT_व्यंजक_INVERT) {
            field = arg->unary.child;
            मूल्य = (स्थिर व्यंजक_Def *) &constFalse;
        }
        अन्यथा {
            field = arg;
            मूल्य = (स्थिर व्यंजक_Def *) &constTrue;
        }

        अगर (!व्यंजक_Resolve_Lhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            वापस PARSER_FATAL_ERROR;

        अगर (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            वापस PARSER_FATAL_ERROR;
        }

        क्रमागत action_field fieldNdx;
        अगर (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            अगर (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                वापस PARSER_FATAL_ERROR;
            } अन्यथा {
                /* Field is just ignored */
                जारी;
            }
        }

        चयन (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, मूल्य, मूल्य_ptr)) {
        हाल PARSER_FATAL_ERROR:
            वापस PARSER_FATAL_ERROR;
        हाल PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            अवरोध;
        शेष:
            ;
        }
    }

    वापस (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

क्रमागत xkb_parser_error
SetDefaultActionField(स्थिर काष्ठा xkb_keymap_info *keymap_info,
                      ActionsInfo *info, काष्ठा xkb_mod_set *mods,
                      स्थिर अक्षर *elem, स्थिर अक्षर *field, व्यंजक_Def *array_ndx,
                      व्यंजक_Def **मूल्य_ptr, क्रमागत merge_mode merge)
{
    स्थिर व्यंजक_Def * restrict स्थिर मूल्य = *मूल्य_ptr;
    क्रमागत xkb_action_type action;
    अगर (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        वापस (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    क्रमागत action_field action_field;
    अगर (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        वापस (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    जोड़ xkb_action* स्थिर into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    जोड़ xkb_action from = *into;

    /* Parse action */
    स्थिर क्रमागत xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, मूल्य, मूल्य_ptr
    );

    /* Success is mandatory to update defaults */
    अगर (ret != PARSER_SUCCESS)
        वापस ret;

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
    अगर (!action_equal(into, &from)) {
        स्थिर bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        अगर (replace)
            *into = from;
    }
    वापस PARSER_SUCCESS;
}
