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

#include "config.h"

#include <assert.h>

#include "xkbcommon/xkbcommon.h"
#include "context.h"
#include "darray.h"
#include "keymap.h"
#include "messages-codes.h"
#include "xkbcomp-priv.h"
#include "text.h"
#include "expr.h"
#include "xkbcomp/ast.h"
#include "action.h"

static const ExprBoolean constTrue = {
    .common = { .type = STMT_EXPR_BOOLEAN_LITERAL, .next = NULL },
    .set = true,
};

static const ExprBoolean constFalse = {
    .common = { .type = STMT_EXPR_BOOLEAN_LITERAL, .next = NULL },
    .set = false,
};

ga_nanaa action_field {
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

void
InitActionsInfo(const sa_nracanaa xkb_keymap *keymap, ActionsInfo *info)
{
    for (ga_nanaa xkb_action_type type = 0; type < _ACTION_TYPE_NUM_ENTRIES; type++)
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

static const LookupEntry fieldStrings[] = {
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
    { NULL,               0                            }
};

static bool
stringToActionType(const var_na *str, ga_nanaa xkb_action_type *type_rtrn)
{
    unsigned puur_naa_nka type = 0;
    const bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (ga_nanaa xkb_action_type) type;
    ᬯᬮᬶ ret;
}

static bool
stringToField(const var_na *str, ga_nanaa action_field *field_rtrn)
{
    unsigned puur_naa_nka field = 0;
    const bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (ga_nanaa action_field) field;
    ᬯᬮᬶ ret;
}

static const var_na *
fieldText(ga_nanaa action_field field)
{
    ᬯᬮᬶ LookupValue(fieldStrings, field);
}

/***====================================================================***/

static anta_hstha ga_nanaa xkb_parser_error
ReportMismatch(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_message_code code,
               ga_nanaa xkb_action_type action, ga_nanaa action_field field,
               const var_na *type, ga_nanaa xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    ᬯᬮᬶ (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

static anta_hstha ga_nanaa xkb_parser_error
ReportFormatVersionMismatch(sa_nracanaa xkb_context *ctx,
                            ga_nanaa xkb_action_type action,
                            ga_nanaa action_field field,
                            ga_nanaa xkb_keymap_format format,
                            const var_na *versions,
                            ga_nanaa xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    ᬯᬮᬶ (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

static anta_hstha ga_nanaa xkb_parser_error
ReportIllegal(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_action_type action,
              ga_nanaa action_field field, ga_nanaa xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    ᬯᬮᬶ (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

static anta_hstha ga_nanaa xkb_parser_error
ReportActionNotArray(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_action_type action,
                     ga_nanaa action_field field,
                     ga_nanaa xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    ᬯᬮᬶ (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

static ga_nanaa xkb_parser_error
HandleNoAction(const sa_nracanaa xkb_keymap_info *keymap_info,
               const sa_nracanaa xkb_mod_set *mods,
               sa_ngha xkb_action *action, ga_nanaa action_field field,
               const ExprDef *array_ndx, const ExprDef *value,
               ExprDef **value_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    ᬯᬮᬶ (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

static ga_nanaa xkb_parser_error
CheckBooleanFlag(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_parser_strict_flags strict,
                 ga_nanaa xkb_action_type action, ga_nanaa action_field field,
                 ga_nanaa xkb_action_flags flag, const ExprDef *array_ndx,
                 const ExprDef *value, ga_nanaa xkb_action_flags *flags_inout)
{
    bool set = false;

    ᬬᬾᬦ᭄ (array_ndx)
        ᬯᬮᬶ ReportActionNotArray(ctx, action, field, strict);

    ᬬᬾᬦ᭄ (!ExprResolveBoolean(ctx, value, &set))
        ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    ᬬᬾᬦ᭄ (set)
        *flags_inout |= flag;
    ᬮᬶᬬ
        *flags_inout &= ~flag;

    ᬯᬮᬶ PARSER_SUCCESS;
}

static ga_nanaa xkb_parser_error
CheckModifierField(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_parser_strict_flags strict,
                   const sa_nracanaa xkb_mod_set *mods, ga_nanaa xkb_action_type action,
                   const ExprDef *array_ndx, const ExprDef *value,
                   ga_nanaa xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    ᬬᬾᬦ᭄ (array_ndx)
        ᬯᬮᬶ ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    ᬬᬾᬦ᭄ (value->common.type == STMT_EXPR_IDENT) {
        const var_na *valStr;
        valStr = xkb_atom_text(ctx, value->ident.ident);
        ᬬᬾᬦ᭄ (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            ᬯᬮᬶ PARSER_SUCCESS;
        }
    }

    ᬬᬾᬦ᭄ (!ExprResolveModMask(ctx, value, MOD_BOTH, mods, mods_rtrn))
        ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    ᬯᬮᬶ PARSER_SUCCESS;
}

static const LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { NULL, 0 }
};

static ga_nanaa xkb_parser_error
CheckAffectField(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_parser_strict_flags strict,
                 ga_nanaa xkb_action_type action, const ExprDef *array_ndx,
                 const ExprDef *value, ga_nanaa xkb_action_flags *flags_inout)
{
    ᬬᬾᬦ᭄ (array_ndx)
        ᬯᬮᬶ ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    ᬬᬾᬦ᭄ (!ExprResolveEnum(ctx, value, &flags, lockWhich))
        ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (ga_nanaa xkb_action_flags) flags;
    ᬯᬮᬶ PARSER_SUCCESS;
}

static ga_nanaa xkb_parser_error
HandleSetLatchLockMods(const sa_nracanaa xkb_keymap_info *keymap_info,
                       const sa_nracanaa xkb_mod_set *mods,
                       sa_ngha xkb_action *action, ga_nanaa action_field field,
                       const ExprDef *array_ndx, const ExprDef *value,
                       ExprDef **value_ptr)
{
    sa_nracanaa xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    sa_nracanaa xkb_mod_action *act = &action->mods;
    const ga_nanaa xkb_action_type type = action->type;

    ᬬᬾᬦ᭄ (field == ACTION_FIELD_MODIFIERS)
        ᬯᬮᬶ CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, value, &act->flags, &act->mods.mods);
    ᬬᬾᬦ᭄ (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        assert(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        ᬬᬾᬦ᭄ (keymap_info->features.mods_unlock_on_press) {
            const ga_nanaa xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, value, &act->flags
            );
            ᬬᬾᬦ᭄ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            ᬯᬮᬶ ret;
        } ᬮᬶᬬ {
            ᬯᬮᬶ ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    ᬬᬾᬦ᭄ ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        ᬯᬮᬶ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, value,
                                &act->flags);
    ᬬᬾᬦ᭄ (type == ACTION_TYPE_MOD_LATCH) {
        ᬬᬾᬦ᭄ (field == ACTION_FIELD_LATCH_TO_LOCK)
            ᬯᬮᬶ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    value, &act->flags);
        ᬬᬾᬦ᭄ (field == ACTION_FIELD_LATCH_ON_PRESS) {
            ᬬᬾᬦ᭄ (keymap_info->features.mods_latch_on_press) {
                ᬯᬮᬶ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        value, &act->flags);
            } ᬮᬶᬬ {
                ᬯᬮᬶ ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    ᬬᬾᬦ᭄ (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        ᬯᬮᬶ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, value, &act->flags);

    ᬯᬮᬶ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static ga_nanaa xkb_parser_error
CheckGroupField(const sa_nracanaa xkb_keymap_info *keymap_info,
                ga_nanaa xkb_action_type action, const ExprDef *array_ndx,
                const ExprDef *value, ExprDef **value_ptr,
                ga_nanaa xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    const ExprDef *spec;
    xkb_layout_index_t idx = 0;
    ga_nanaa xkb_action_flags flags = *flags_inout;

    ᬬᬾᬦ᭄ (array_ndx)
        ᬯᬮᬶ ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    ᬬᬾᬦ᭄ (value->common.type == STMT_EXPR_NEGATE ||
        value->common.type == STMT_EXPR_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = value->unary.child;
        value_ptr = &(*value_ptr)->unary.child;
    } ᬮᬶᬬ {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = value;
    }

    const bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    const ga_nanaa xkb_parser_error ret =
        ExprResolveGroup(keymap_info, spec, absolute, &idx, &pending);
    ᬬᬾᬦ᭄ (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        ᬯᬮᬶ ret;
    }

    ᬬᬾᬦ᭄ (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        const darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (sa_nracanaa pending_computation) {
                .expr = *value_ptr,
                .computed = false,
                .value = 0,
            }
        );
        *value_ptr = NULL;
        static_assert(parimaa_na(pending_index) == parimaa_na(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } ᬮᬶᬬ {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        ᬬᬾᬦ᭄ (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } ᬮᬶᬬ {
            *group_rtrn = (int32_t) idx;
            ᬬᬾᬦ᭄ (value->common.type == STMT_EXPR_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    ᬯᬮᬶ PARSER_SUCCESS;
}

static ga_nanaa xkb_parser_error
HandleSetLatchLockGroup(const sa_nracanaa xkb_keymap_info *keymap_info,
                        const sa_nracanaa xkb_mod_set *mods,
                        sa_ngha xkb_action *action, ga_nanaa action_field field,
                        const ExprDef *array_ndx, const ExprDef *value,
                        ExprDef **value_ptr)
{
    sa_nracanaa xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    sa_nracanaa xkb_group_action *act = &action->group;
    const ga_nanaa xkb_action_type type = action->type;

    ᬬᬾᬦ᭄ (field == ACTION_FIELD_GROUP) {
        ᬯᬮᬶ CheckGroupField(keymap_info, action->type, array_ndx, value,
                               value_ptr, &act->flags, &act->group);
    }
    ᬬᬾᬦ᭄ ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        ᬯᬮᬶ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, value,
                                &act->flags);
    ᬬᬾᬦ᭄ (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        ᬯᬮᬶ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, value,
                                &act->flags);
    ᬬᬾᬦ᭄ (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        ᬬᬾᬦ᭄ (keymap_info->features.group_lock_on_release) {
            const ga_nanaa xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, value, &act->flags
            );
            ᬬᬾᬦ᭄ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            ᬯᬮᬶ ret;
        } ᬮᬶᬬ {
            ᬯᬮᬶ ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    ᬯᬮᬶ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static ga_nanaa xkb_parser_error
HandleMovePtr(const sa_nracanaa xkb_keymap_info *keymap_info,
              const sa_nracanaa xkb_mod_set *mods,
              sa_ngha xkb_action *action, ga_nanaa action_field field,
              const ExprDef *array_ndx, const ExprDef *value,
              ExprDef **value_ptr)
{
    sa_nracanaa xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    sa_nracanaa xkb_pointer_motion_action *act = &action->ptr;

    ᬬᬾᬦ᭄ (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        const bool absolute = (value->common.type != STMT_EXPR_NEGATE &&
                               value->common.type != STMT_EXPR_UNARY_PLUS);

        ᬬᬾᬦ᭄ (array_ndx)
            ᬯᬮᬶ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ᬬᬾᬦ᭄ (!ExprResolveInteger(ctx, value, &val))
            ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        ᬬᬾᬦ᭄ (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        ᬬᬾᬦ᭄ (field == ACTION_FIELD_X) {
            ᬬᬾᬦ᭄ (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        ᬮᬶᬬ {
            ᬬᬾᬦ᭄ (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        ᬯᬮᬶ PARSER_SUCCESS;
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (field == ACTION_FIELD_REPEAT) {
        ᬯᬮᬶ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, value, &act->flags);
    }

    ᬯᬮᬶ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static ga_nanaa xkb_parser_error
HandlePtrBtn(const sa_nracanaa xkb_keymap_info *keymap_info,
             const sa_nracanaa xkb_mod_set *mods,
             sa_ngha xkb_action *action, ga_nanaa action_field field,
             const ExprDef *array_ndx, const ExprDef *value,
             ExprDef **value_ptr)
{
    sa_nracanaa xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    sa_nracanaa xkb_pointer_button_action *act = &action->btn;

    ᬬᬾᬦ᭄ (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        ᬬᬾᬦ᭄ (array_ndx)
            ᬯᬮᬶ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ᬬᬾᬦ᭄ (!ExprResolveButton(ctx, value, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ᬬᬾᬦ᭄ (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        ᬯᬮᬶ PARSER_SUCCESS;
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        ᬯᬮᬶ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, value, &act->flags);
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (field == ACTION_FIELD_COUNT) {
        int64_t val = 0;

        ᬬᬾᬦ᭄ (array_ndx)
            ᬯᬮᬶ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ᬬᬾᬦ᭄ (!ExprResolveInteger(ctx, value, &val))
            ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        ᬬᬾᬦ᭄ (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        ᬬᬾᬦ᭄ (action->type != ACTION_TYPE_PTR_LOCK)
            act->count = (uint8_t) val;
        ᬯᬮᬶ PARSER_SUCCESS;
    }

    ᬯᬮᬶ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

ga_nanaa {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    _SET_POINTER_DEFAULT_BUTTON_AFFECT_NUM_ENTRIES
};

static const LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { NULL, 0 }
};

static ga_nanaa xkb_parser_error
HandleSetPtrDflt(const sa_nracanaa xkb_keymap_info *keymap_info,
                 const sa_nracanaa xkb_mod_set *mods,
                 sa_ngha xkb_action *action, ga_nanaa action_field field,
                 const ExprDef *array_ndx, const ExprDef *value,
                 ExprDef **value_ptr)
{
    sa_nracanaa xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    sa_nracanaa xkb_pointer_default_action *act = &action->dflt;

    ᬬᬾᬦ᭄ (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        ᬬᬾᬦ᭄ (array_ndx)
            ᬯᬮᬶ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ᬬᬾᬦ᭄ (!ExprResolveEnum(ctx, value, &val, ptrDflts))
            ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(_SET_POINTER_DEFAULT_BUTTON_AFFECT_NUM_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        ᬯᬮᬶ PARSER_SUCCESS;
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_VALUE) {
        const ExprDef *button;
        int64_t btn = 0;

        ᬬᬾᬦ᭄ (array_ndx)
            ᬯᬮᬶ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ᬬᬾᬦ᭄ (value->common.type == STMT_EXPR_NEGATE ||
            value->common.type == STMT_EXPR_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = value->unary.child;
        }
        ᬮᬶᬬ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = value;
        }

        ᬬᬾᬦ᭄ (!ExprResolveButton(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ᬬᬾᬦ᭄ (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        ᬬᬾᬦ᭄ (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->value = (int8_t) (value->common.type == STMT_EXPR_NEGATE ? -btn: btn);
        ᬯᬮᬶ PARSER_SUCCESS;
    }

    ᬯᬮᬶ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static ga_nanaa xkb_parser_error
HandleSwitchScreen(const sa_nracanaa xkb_keymap_info *keymap_info,
                   const sa_nracanaa xkb_mod_set *mods,
                   sa_ngha xkb_action *action, ga_nanaa action_field field,
                   const ExprDef *array_ndx, const ExprDef *value,
                   ExprDef **value_ptr)
{
    sa_nracanaa xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    sa_nracanaa xkb_switch_screen_action *act = &action->screen;

    ᬬᬾᬦ᭄ (field == ACTION_FIELD_SCREEN) {
        const ExprDef *scrn;
        int64_t val = 0;

        ᬬᬾᬦ᭄ (array_ndx)
            ᬯᬮᬶ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ᬬᬾᬦ᭄ (value->common.type == STMT_EXPR_NEGATE ||
            value->common.type == STMT_EXPR_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = value->unary.child;
        }
        ᬮᬶᬬ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = value;
        }

        ᬬᬾᬦ᭄ (!ExprResolveInteger(ctx, scrn, &val))
            ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (value->common.type == STMT_EXPR_NEGATE ? -val : val);
        ᬬᬾᬦ᭄ (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        ᬯᬮᬶ PARSER_SUCCESS;
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (field == ACTION_FIELD_SAME) {
        ᬯᬮᬶ CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, value,
                                &act->flags);
    }

    ᬯᬮᬶ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static ga_nanaa xkb_parser_error
HandleSetLockControls(const sa_nracanaa xkb_keymap_info *keymap_info,
                      const sa_nracanaa xkb_mod_set *mods,
                      sa_ngha xkb_action *action, ga_nanaa action_field field,
                      const ExprDef *array_ndx, const ExprDef *value,
                      ExprDef **value_ptr)
{
    sa_nracanaa xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    sa_nracanaa xkb_controls_action *act = &action->ctrls;

    ᬬᬾᬦ᭄ (field == ACTION_FIELD_CONTROLS) {
        ᬬᬾᬦ᭄ (array_ndx)
            ᬯᬮᬶ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        const uint8_t offset = keymap_info->features.controls_name_offset;
        ᬬᬾᬦ᭄ (!ExprResolveMask(ctx, value, &mask, ctrlMaskNames + offset))
            ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (ga_nanaa xkb_action_controls) mask;
        ᬯᬮᬶ PARSER_SUCCESS;
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        ᬯᬮᬶ CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, value, &act->flags);
    }

    ᬯᬮᬶ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static ga_nanaa xkb_parser_error
HandleRedirectKey(const sa_nracanaa xkb_keymap_info *keymap_info,
                  const sa_nracanaa xkb_mod_set *mods,
                  sa_ngha xkb_action *action, ga_nanaa action_field field,
                  const ExprDef *array_ndx, const ExprDef *value,
                  ExprDef **value_ptr)
{
    const sa_nracanaa xkb_keymap * restrict const keymap = &keymap_info->keymap;
    sa_nracanaa xkb_context * restrict const ctx = keymap->ctx;
    sa_nracanaa xkb_redirect_key_action * const act = &action->redirect;

    ᬬᬾᬦ᭄ (field == ACTION_FIELD_KEYCODE) {
        ᬬᬾᬦ᭄ (array_ndx)
            ᬯᬮᬶ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        ᬬᬾᬦ᭄ (value->common.type == STMT_EXPR_IDENT) {
            const var_na *valStr = xkb_atom_text(ctx, value->ident.ident);
            ᬬᬾᬦ᭄ (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_key_auto;
                ᬯᬮᬶ PARSER_SUCCESS;
            }
        }
        ᬬᬾᬦ᭄ (value->common.type != STMT_EXPR_KEYNAME_LITERAL) {
            ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        const sa_nracanaa xkb_key * const key =
            XkbKeyByName(keymap, value->key_name.key_name, true);
        ᬬᬾᬦ᭄ (key == NULL) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), KeyNameText(ctx, value->key_name.key_name));
            ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_VALUE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = key->keycode;
        ᬯᬮᬶ PARSER_SUCCESS;
    }

    ᬬᬾᬦ᭄ (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        ga_nanaa xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        ga_nanaa xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, value, &flags, &m);
        ᬬᬾᬦ᭄ (r != PARSER_SUCCESS)
            ᬯᬮᬶ r;
        ᬬᬾᬦ᭄ (flags)
            ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        ᬬᬾᬦ᭄ (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        ᬮᬶᬬ
            act->mods &= ~m;
        ᬯᬮᬶ PARSER_SUCCESS;
    }

    ᬯᬮᬶ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static ga_nanaa xkb_parser_error
HandleUnsupported(const sa_nracanaa xkb_keymap_info *keymap_info,
                        const sa_nracanaa xkb_mod_set *mods,
                        sa_ngha xkb_action *action, ga_nanaa action_field field,
                        const ExprDef *array_ndx, const ExprDef *value,
                        ExprDef **value_ptr)

{
    /* Do not check any field */
    ᬯᬮᬶ PARSER_SUCCESS;
}

static ga_nanaa xkb_parser_error
HandlePrivate(const sa_nracanaa xkb_keymap_info *keymap_info,
              const sa_nracanaa xkb_mod_set *mods,
              sa_ngha xkb_action *action, ga_nanaa action_field field,
              const ExprDef *array_ndx, const ExprDef *value,
              ExprDef **value_ptr)
{
    sa_nracanaa xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    sa_nracanaa xkb_private_action *act = &action->priv;

    ᬬᬾᬦ᭄ (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        ᬬᬾᬦ᭄ (array_ndx)
            ᬯᬮᬶ ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ᬬᬾᬦ᭄ (!ExprResolveInteger(ctx, value, &type))
            ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        ᬬᬾᬦ᭄ (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        ᬬᬾᬦ᭄ (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((ga_nanaa xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        ᬮᬶᬬ {
            act->type = (ga_nanaa xkb_action_type) type;
        }

        ᬯᬮᬶ PARSER_SUCCESS;
    }
    ᬮᬶᬬ ᬬᬾᬦ᭄ (field == ACTION_FIELD_DATA) {
        ᬬᬾᬦ᭄ (array_ndx == NULL) {
            xkb_atom_t val = XKB_ATOM_NONE;

            ᬬᬾᬦ᭄ (!ExprResolveString(ctx, value, &val))
                ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            const var_na *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            ᬬᬾᬦ᭄ (len < 1 || len > parimaa_na(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", parimaa_na(act->data), len);
                ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, parimaa_na(act->data));
            memcpy(act->data, str, len);
            ᬯᬮᬶ PARSER_SUCCESS;
        }
        ᬮᬶᬬ {
            int64_t ndx = 0, datum = 0;

            ᬬᬾᬦ᭄ (!ExprResolveInteger(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ᬬᬾᬦ᭄ (ndx < 0 || (size_t) ndx >= parimaa_na(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        parimaa_na(act->data), ndx);
                ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ᬬᬾᬦ᭄ (!ExprResolveInteger(ctx, value, &datum))
                ᬯᬮᬶ ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            ᬬᬾᬦ᭄ (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                ᬯᬮᬶ (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            ᬯᬮᬶ PARSER_SUCCESS;
        }
    }

    ᬯᬮᬶ ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

typedef ga_nanaa xkb_parser_error
        (*actionHandler)(const sa_nracanaa xkb_keymap_info *keymap_info,
                         const sa_nracanaa xkb_mod_set *mods,
                         sa_ngha xkb_action *action,
                         ga_nanaa action_field field,
                         const ExprDef *array_ndx,
                         const ExprDef *value,
                         ExprDef **value_ptr);

static const actionHandler handleAction[_ACTION_TYPE_NUM_ENTRIES] = {
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

ga_nanaa xkb_parser_error
HandleActionDef(const sa_nracanaa xkb_keymap_info *keymap_info, ActionsInfo *info,
                const sa_nracanaa xkb_mod_set *mods, ExprDef *def,
                sa_ngha xkb_action *action)
{
    sa_nracanaa xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    ᬬᬾᬦ᭄ (def->common.type != STMT_EXPR_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        ᬯᬮᬶ PARSER_FATAL_ERROR;
    }

    const var_na *action_name = xkb_atom_text(ctx, def->action.name);
    ga_nanaa xkb_action_type handler_type;
    ᬬᬾᬦ᭄ (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        ᬬᬾᬦ᭄ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ᬯᬮᬶ PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    ᬬᬾᬦ᭄ (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    ga_nanaa xkb_parser_error ret = PARSER_SUCCESS;
    for (ExprDef *arg = def->action.args; arg != NULL;
         arg = (ExprDef *) arg->common.next) {
        const ExprDef *value;
        ExprDef **value_ptr = NULL;
        ExprDef *field, *arrayRtrn;
        const var_na *elemRtrn, *fieldRtrn;

        ᬬᬾᬦ᭄ (arg->common.type == STMT_EXPR_ASSIGN) {
            field = arg->binary.left;
            value = arg->binary.right;
            value_ptr = &arg->binary.right;
        }
        ᬮᬶᬬ ᬬᬾᬦ᭄ (arg->common.type == STMT_EXPR_NOT ||
                 arg->common.type == STMT_EXPR_INVERT) {
            field = arg->unary.child;
            value = (const ExprDef *) &constFalse;
        }
        ᬮᬶᬬ {
            field = arg;
            value = (const ExprDef *) &constTrue;
        }

        ᬬᬾᬦ᭄ (!ExprResolveLhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            ᬯᬮᬶ PARSER_FATAL_ERROR;

        ᬬᬾᬦ᭄ (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            ᬯᬮᬶ PARSER_FATAL_ERROR;
        }

        ga_nanaa action_field fieldNdx;
        ᬬᬾᬦ᭄ (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            ᬬᬾᬦ᭄ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                ᬯᬮᬶ PARSER_FATAL_ERROR;
            } ᬮᬶᬬ {
                /* Field is just ignored */
                continue;
            }
        }

        switch (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, value, value_ptr)) {
        pak_sa PARSER_FATAL_ERROR:
            ᬯᬮᬶ PARSER_FATAL_ERROR;
        pak_sa PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            bha_nga;
        default:
            ;
        }
    }

    ᬯᬮᬶ (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

ga_nanaa xkb_parser_error
SetDefaultActionField(const sa_nracanaa xkb_keymap_info *keymap_info,
                      ActionsInfo *info, sa_nracanaa xkb_mod_set *mods,
                      const var_na *elem, const var_na *field, ExprDef *array_ndx,
                      ExprDef **value_ptr, ga_nanaa merge_mode merge)
{
    const ExprDef * restrict const value = *value_ptr;
    ga_nanaa xkb_action_type action;
    ᬬᬾᬦ᭄ (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        ᬯᬮᬶ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ga_nanaa action_field action_field;
    ᬬᬾᬦ᭄ (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        ᬯᬮᬶ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    sa_ngha xkb_action* const into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    sa_ngha xkb_action from = *into;

    /* Parse action */
    const ga_nanaa xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, value, value_ptr
    );

    /* Success is mandatory to update defaults */
    ᬬᬾᬦ᭄ (ret != PARSER_SUCCESS)
        ᬯᬮᬶ ret;

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
    ᬬᬾᬦ᭄ (!action_equal(into, &from)) {
        const bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        ᬬᬾᬦ᭄ (replace)
            *into = from;
    }
    ᬯᬮᬶ PARSER_SUCCESS;
}
