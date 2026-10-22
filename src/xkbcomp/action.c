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

enum action_field {
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
InitActionsInfo(const struct xkb_keymap *keymap, ActionsInfo *info)
{
    за (enum xkb_action_type type = 0; type < _ACTION_TYPE_NUM_ENTRIES; type++)
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
stringToActionType(const char *str, enum xkb_action_type *type_rtrn)
{
    unsigned цяло type = 0;
    const bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (enum xkb_action_type) type;
    върни ret;
}

static bool
stringToField(const char *str, enum action_field *field_rtrn)
{
    unsigned цяло field = 0;
    const bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (enum action_field) field;
    върни ret;
}

static const char *
fieldText(enum action_field field)
{
    върни LookupValue(fieldStrings, field);
}

/***====================================================================***/

static inline enum xkb_parser_error
ReportMismatch(struct xkb_context *ctx, enum xkb_message_code code,
               enum xkb_action_type action, enum action_field field,
               const char *type, enum xkb_parser_strict_flags strict)
{
    log_err(ctx, code,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            fieldText(field), type, ActionTypeText(action));
    върни (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

static inline enum xkb_parser_error
ReportFormatVersionMismatch(struct xkb_context *ctx,
                            enum xkb_action_type action,
                            enum action_field field,
                            enum xkb_keymap_format format,
                            const char *versions,
                            enum xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            fieldText(field), ActionTypeText(action), versions, format);
    върни (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

static inline enum xkb_parser_error
ReportIllegal(struct xkb_context *ctx, enum xkb_action_type action,
              enum action_field field, enum xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    върни (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

static inline enum xkb_parser_error
ReportActionNotArray(struct xkb_context *ctx, enum xkb_action_type action,
                     enum action_field field,
                     enum xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            fieldText(field), ActionTypeText(action));
    върни (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

static enum xkb_parser_error
HandleNoAction(const struct xkb_keymap_info *keymap_info,
               const struct xkb_mod_set *mods,
               union xkb_action *action, enum action_field field,
               const ExprDef *array_ndx, const ExprDef *value,
               ExprDef **value_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    върни (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

static enum xkb_parser_error
CheckBooleanFlag(struct xkb_context *ctx, enum xkb_parser_strict_flags strict,
                 enum xkb_action_type action, enum action_field field,
                 enum xkb_action_flags flag, const ExprDef *array_ndx,
                 const ExprDef *value, enum xkb_action_flags *flags_inout)
{
    bool set = false;

    ако (array_ndx)
        върни ReportActionNotArray(ctx, action, field, strict);

    ако (!ExprResolveBoolean(ctx, value, &set))
        върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    ако (set)
        *flags_inout |= flag;
    иначе
        *flags_inout &= ~flag;

    върни PARSER_SUCCESS;
}

static enum xkb_parser_error
CheckModifierField(struct xkb_context *ctx, enum xkb_parser_strict_flags strict,
                   const struct xkb_mod_set *mods, enum xkb_action_type action,
                   const ExprDef *array_ndx, const ExprDef *value,
                   enum xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    ако (array_ndx)
        върни ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    ако (value->common.type == STMT_EXPR_IDENT) {
        const char *valStr;
        valStr = xkb_atom_text(ctx, value->ident.ident);
        ако (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            върни PARSER_SUCCESS;
        }
    }

    ако (!ExprResolveModMask(ctx, value, MOD_BOTH, mods, mods_rtrn))
        върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    върни PARSER_SUCCESS;
}

static const LookupEntry lockWhich[] = {
    { "both", 0 },
    { "lock", ACTION_LOCK_NO_UNLOCK },
    { "neither", (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK) },
    { "unlock", ACTION_LOCK_NO_LOCK },
    { NULL, 0 }
};

static enum xkb_parser_error
CheckAffectField(struct xkb_context *ctx, enum xkb_parser_strict_flags strict,
                 enum xkb_action_type action, const ExprDef *array_ndx,
                 const ExprDef *value, enum xkb_action_flags *flags_inout)
{
    ако (array_ndx)
        върни ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    ако (!ExprResolveEnum(ctx, value, &flags, lockWhich))
        върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (enum xkb_action_flags) flags;
    върни PARSER_SUCCESS;
}

static enum xkb_parser_error
HandleSetLatchLockMods(const struct xkb_keymap_info *keymap_info,
                       const struct xkb_mod_set *mods,
                       union xkb_action *action, enum action_field field,
                       const ExprDef *array_ndx, const ExprDef *value,
                       ExprDef **value_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_mod_action *act = &action->mods;
    const enum xkb_action_type type = action->type;

    ако (field == ACTION_FIELD_MODIFIERS)
        върни CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, value, &act->flags, &act->mods.mods);
    ако (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        assert(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        ако (keymap_info->features.mods_unlock_on_press) {
            const enum xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, value, &act->flags
            );
            ако (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            върни ret;
        } иначе {
            върни ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    ако ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        върни CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, value,
                                &act->flags);
    ако (type == ACTION_TYPE_MOD_LATCH) {
        ако (field == ACTION_FIELD_LATCH_TO_LOCK)
            върни CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    value, &act->flags);
        ако (field == ACTION_FIELD_LATCH_ON_PRESS) {
            ако (keymap_info->features.mods_latch_on_press) {
                върни CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        value, &act->flags);
            } иначе {
                върни ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    ако (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        върни CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, value, &act->flags);

    върни ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
CheckGroupField(const struct xkb_keymap_info *keymap_info,
                enum xkb_action_type action, const ExprDef *array_ndx,
                const ExprDef *value, ExprDef **value_ptr,
                enum xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    const ExprDef *spec;
    xkb_layout_index_t idx = 0;
    enum xkb_action_flags flags = *flags_inout;

    ако (array_ndx)
        върни ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    ако (value->common.type == STMT_EXPR_NEGATE ||
        value->common.type == STMT_EXPR_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = value->unary.child;
        value_ptr = &(*value_ptr)->unary.child;
    } иначе {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = value;
    }

    const bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    const enum xkb_parser_error ret =
        ExprResolveGroup(keymap_info, spec, absolute, &idx, &pending);
    ако (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        върни ret;
    }

    ако (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        const darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (struct pending_computation) {
                .expr = *value_ptr,
                .computed = false,
                .value = 0,
            }
        );
        *value_ptr = NULL;
        static_assert(sizeof(pending_index) == sizeof(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } иначе {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        ако (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } иначе {
            *group_rtrn = (int32_t) idx;
            ако (value->common.type == STMT_EXPR_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    върни PARSER_SUCCESS;
}

static enum xkb_parser_error
HandleSetLatchLockGroup(const struct xkb_keymap_info *keymap_info,
                        const struct xkb_mod_set *mods,
                        union xkb_action *action, enum action_field field,
                        const ExprDef *array_ndx, const ExprDef *value,
                        ExprDef **value_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_group_action *act = &action->group;
    const enum xkb_action_type type = action->type;

    ако (field == ACTION_FIELD_GROUP) {
        върни CheckGroupField(keymap_info, action->type, array_ndx, value,
                               value_ptr, &act->flags, &act->group);
    }
    ако ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        върни CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, value,
                                &act->flags);
    ако (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        върни CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, value,
                                &act->flags);
    ако (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        ако (keymap_info->features.group_lock_on_release) {
            const enum xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, value, &act->flags
            );
            ако (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            върни ret;
        } иначе {
            върни ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    върни ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandleMovePtr(const struct xkb_keymap_info *keymap_info,
              const struct xkb_mod_set *mods,
              union xkb_action *action, enum action_field field,
              const ExprDef *array_ndx, const ExprDef *value,
              ExprDef **value_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_pointer_motion_action *act = &action->ptr;

    ако (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        const bool absolute = (value->common.type != STMT_EXPR_NEGATE &&
                               value->common.type != STMT_EXPR_UNARY_PLUS);

        ако (array_ndx)
            върни ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ако (!ExprResolveInteger(ctx, value, &val))
            върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        ако (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        ако (field == ACTION_FIELD_X) {
            ако (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        иначе {
            ако (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        върни PARSER_SUCCESS;
    }
    иначе ако (field == ACTION_FIELD_REPEAT) {
        върни CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, value, &act->flags);
    }

    върни ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandlePtrBtn(const struct xkb_keymap_info *keymap_info,
             const struct xkb_mod_set *mods,
             union xkb_action *action, enum action_field field,
             const ExprDef *array_ndx, const ExprDef *value,
             ExprDef **value_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_pointer_button_action *act = &action->btn;

    ако (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        ако (array_ndx)
            върни ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ако (!ExprResolveButton(ctx, value, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ако (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        върни PARSER_SUCCESS;
    }
    иначе ако (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        върни CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, value, &act->flags);
    }
    иначе ако (field == ACTION_FIELD_COUNT) {
        int64_t val = 0;

        ако (array_ndx)
            върни ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ако (!ExprResolveInteger(ctx, value, &val))
            върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        ако (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        ако (action->type != ACTION_TYPE_PTR_LOCK)
            act->count = (uint8_t) val;
        върни PARSER_SUCCESS;
    }

    върни ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

enum {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    _SET_POINTER_DEFAULT_BUTTON_AFFECT_NUM_ENTRIES
};

static const LookupEntry ptrDflts[] = {
    { "dfltbtn", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { NULL, 0 }
};

static enum xkb_parser_error
HandleSetPtrDflt(const struct xkb_keymap_info *keymap_info,
                 const struct xkb_mod_set *mods,
                 union xkb_action *action, enum action_field field,
                 const ExprDef *array_ndx, const ExprDef *value,
                 ExprDef **value_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_pointer_default_action *act = &action->dflt;

    ако (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        ако (array_ndx)
            върни ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ако (!ExprResolveEnum(ctx, value, &val, ptrDflts))
            върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(_SET_POINTER_DEFAULT_BUTTON_AFFECT_NUM_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        върни PARSER_SUCCESS;
    }
    иначе ако (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_VALUE) {
        const ExprDef *button;
        int64_t btn = 0;

        ако (array_ndx)
            върни ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ако (value->common.type == STMT_EXPR_NEGATE ||
            value->common.type == STMT_EXPR_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = value->unary.child;
        }
        иначе {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = value;
        }

        ако (!ExprResolveButton(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ако (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        ако (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->value = (int8_t) (value->common.type == STMT_EXPR_NEGATE ? -btn: btn);
        върни PARSER_SUCCESS;
    }

    върни ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandleSwitchScreen(const struct xkb_keymap_info *keymap_info,
                   const struct xkb_mod_set *mods,
                   union xkb_action *action, enum action_field field,
                   const ExprDef *array_ndx, const ExprDef *value,
                   ExprDef **value_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_switch_screen_action *act = &action->screen;

    ако (field == ACTION_FIELD_SCREEN) {
        const ExprDef *scrn;
        int64_t val = 0;

        ако (array_ndx)
            върни ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ако (value->common.type == STMT_EXPR_NEGATE ||
            value->common.type == STMT_EXPR_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = value->unary.child;
        }
        иначе {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = value;
        }

        ако (!ExprResolveInteger(ctx, scrn, &val))
            върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (value->common.type == STMT_EXPR_NEGATE ? -val : val);
        ако (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        върни PARSER_SUCCESS;
    }
    иначе ако (field == ACTION_FIELD_SAME) {
        върни CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, value,
                                &act->flags);
    }

    върни ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandleSetLockControls(const struct xkb_keymap_info *keymap_info,
                      const struct xkb_mod_set *mods,
                      union xkb_action *action, enum action_field field,
                      const ExprDef *array_ndx, const ExprDef *value,
                      ExprDef **value_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_controls_action *act = &action->ctrls;

    ако (field == ACTION_FIELD_CONTROLS) {
        ако (array_ndx)
            върни ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        const uint8_t offset = keymap_info->features.controls_name_offset;
        ако (!ExprResolveMask(ctx, value, &mask, ctrlMaskNames + offset))
            върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (enum xkb_action_controls) mask;
        върни PARSER_SUCCESS;
    }
    иначе ако (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        върни CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, value, &act->flags);
    }

    върни ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandleRedirectKey(const struct xkb_keymap_info *keymap_info,
                  const struct xkb_mod_set *mods,
                  union xkb_action *action, enum action_field field,
                  const ExprDef *array_ndx, const ExprDef *value,
                  ExprDef **value_ptr)
{
    const struct xkb_keymap * restrict const keymap = &keymap_info->keymap;
    struct xkb_context * restrict const ctx = keymap->ctx;
    struct xkb_redirect_key_action * const act = &action->redirect;

    ако (field == ACTION_FIELD_KEYCODE) {
        ако (array_ndx)
            върни ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        ако (value->common.type == STMT_EXPR_IDENT) {
            const char *valStr = xkb_atom_text(ctx, value->ident.ident);
            ако (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_key_auto;
                върни PARSER_SUCCESS;
            }
        }
        ако (value->common.type != STMT_EXPR_KEYNAME_LITERAL) {
            върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        const struct xkb_key * const key =
            XkbKeyByName(keymap, value->key_name.key_name, true);
        ако (key == NULL) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), KeyNameText(ctx, value->key_name.key_name));
            върни (keymap_info->strict & PARSER_NO_FIELD_VALUE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = key->keycode;
        върни PARSER_SUCCESS;
    }

    ако (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        enum xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        enum xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, value, &flags, &m);
        ако (r != PARSER_SUCCESS)
            върни r;
        ако (flags)
            върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        ако (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        иначе
            act->mods &= ~m;
        върни PARSER_SUCCESS;
    }

    върни ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandleUnsupported(const struct xkb_keymap_info *keymap_info,
                        const struct xkb_mod_set *mods,
                        union xkb_action *action, enum action_field field,
                        const ExprDef *array_ndx, const ExprDef *value,
                        ExprDef **value_ptr)

{
    /* Do not check any field */
    върни PARSER_SUCCESS;
}

static enum xkb_parser_error
HandlePrivate(const struct xkb_keymap_info *keymap_info,
              const struct xkb_mod_set *mods,
              union xkb_action *action, enum action_field field,
              const ExprDef *array_ndx, const ExprDef *value,
              ExprDef **value_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_private_action *act = &action->priv;

    ако (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        ако (array_ndx)
            върни ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        ако (!ExprResolveInteger(ctx, value, &type))
            върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        ако (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        ако (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((enum xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        иначе {
            act->type = (enum xkb_action_type) type;
        }

        върни PARSER_SUCCESS;
    }
    иначе ако (field == ACTION_FIELD_DATA) {
        ако (array_ndx == NULL) {
            xkb_atom_t val = XKB_ATOM_NONE;

            ако (!ExprResolveString(ctx, value, &val))
                върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            const char *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            ако (len < 1 || len > sizeof(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", sizeof(act->data), len);
                върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, sizeof(act->data));
            memcpy(act->data, str, len);
            върни PARSER_SUCCESS;
        }
        иначе {
            int64_t ndx = 0, datum = 0;

            ако (!ExprResolveInteger(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ако (ndx < 0 || (size_t) ndx >= sizeof(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        sizeof(act->data), ndx);
                върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ако (!ExprResolveInteger(ctx, value, &datum))
                върни ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            ако (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                върни (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            върни PARSER_SUCCESS;
        }
    }

    върни ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

typedef enum xkb_parser_error
        (*actionHandler)(const struct xkb_keymap_info *keymap_info,
                         const struct xkb_mod_set *mods,
                         union xkb_action *action,
                         enum action_field field,
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

enum xkb_parser_error
HandleActionDef(const struct xkb_keymap_info *keymap_info, ActionsInfo *info,
                const struct xkb_mod_set *mods, ExprDef *def,
                union xkb_action *action)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    ако (def->common.type != STMT_EXPR_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        върни PARSER_FATAL_ERROR;
    }

    const char *action_name = xkb_atom_text(ctx, def->action.name);
    enum xkb_action_type handler_type;
    ако (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        ако (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            върни PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    ако (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    enum xkb_parser_error ret = PARSER_SUCCESS;
    за (ExprDef *arg = def->action.args; arg != NULL;
         arg = (ExprDef *) arg->common.next) {
        const ExprDef *value;
        ExprDef **value_ptr = NULL;
        ExprDef *field, *arrayRtrn;
        const char *elemRtrn, *fieldRtrn;

        ако (arg->common.type == STMT_EXPR_ASSIGN) {
            field = arg->binary.left;
            value = arg->binary.right;
            value_ptr = &arg->binary.right;
        }
        иначе ако (arg->common.type == STMT_EXPR_NOT ||
                 arg->common.type == STMT_EXPR_INVERT) {
            field = arg->unary.child;
            value = (const ExprDef *) &constFalse;
        }
        иначе {
            field = arg;
            value = (const ExprDef *) &constTrue;
        }

        ако (!ExprResolveLhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            върни PARSER_FATAL_ERROR;

        ако (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            върни PARSER_FATAL_ERROR;
        }

        enum action_field fieldNdx;
        ако (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            ако (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                върни PARSER_FATAL_ERROR;
            } иначе {
                /* Field is just ignored */
                continue;
            }
        }

        switch (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, value, value_ptr)) {
        case PARSER_FATAL_ERROR:
            върни PARSER_FATAL_ERROR;
        case PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            break;
        default:
            ;
        }
    }

    върни (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

enum xkb_parser_error
SetDefaultActionField(const struct xkb_keymap_info *keymap_info,
                      ActionsInfo *info, struct xkb_mod_set *mods,
                      const char *elem, const char *field, ExprDef *array_ndx,
                      ExprDef **value_ptr, enum merge_mode merge)
{
    const ExprDef * restrict const value = *value_ptr;
    enum xkb_action_type action;
    ако (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        върни (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    enum action_field action_field;
    ако (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        върни (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    union xkb_action* const into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    union xkb_action from = *into;

    /* Parse action */
    const enum xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, value, value_ptr
    );

    /* Success is mandatory to update defaults */
    ако (ret != PARSER_SUCCESS)
        върни ret;

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
    ако (!action_equal(into, &from)) {
        const bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        ако (replace)
            *into = from;
    }
    върни PARSER_SUCCESS;
}
