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

static const पाहायाग_Boolean constTrue = {
    .common = { .type = STMT_पाहायाग_BOOLEAN_LITERAL, .next = NULL },
    .set = true,
};

static const पाहायाग_Boolean constFalse = {
    .common = { .type = STMT_पाहायाग_BOOLEAN_LITERAL, .next = NULL },
    .set = false,
};

enum action_field {
    ACTION_FIELD_CLEAR_LOCKS,
    ACTION_FIELD_LATCH_TO_LOCK,
    ACTION_FIELD_GEN_सुसि_EVENT,
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
    ACTION_FIELD_हालागा,
    ACTION_FIELD_CONTROLS,
    ACTION_FIELD_TYPE,
    ACTION_FIELD_बिलांग,
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
    for (enum xkb_action_type type = 0; type < ACTION_TYPE_नुमेरो_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.हालागा = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_सुसि].redirect.keycode =
        keymap->redirect_सुसि_auto;
}

static const LookupEntry fieldStrings[] = {
    { "clearLocks",       ACTION_FIELD_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_FIELD_LATCH_TO_LOCK   },
    { "genKeyEvent",      ACTION_FIELD_GEN_सुसि_EVENT   },
    { "generateKeyEvent", ACTION_FIELD_GEN_सुसि_EVENT   },
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
    { "value",            ACTION_FIELD_हालागा           },
    { "controls",         ACTION_FIELD_CONTROLS        },
    { "ctrls",            ACTION_FIELD_CONTROLS        },
    { "type",             ACTION_FIELD_TYPE            },
    { "count",            ACTION_FIELD_बिलांग           },
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
    unsigned int type = 0;
    const bool ret = LookupString(actionTypeNames, str, &type);
    *type_rtrn = (enum xkb_action_type) type;
    return ret;
}

static bool
stringToField(const char *str, enum action_field *field_rtrn)
{
    unsigned int field = 0;
    const bool ret = LookupString(fieldStrings, str, &field);
    *field_rtrn = (enum action_field) field;
    return ret;
}

static const char *
fieldText(enum action_field field)
{
    return Lookup_हालागा(fieldStrings, field);
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
    return (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
    return (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
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
    return (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
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
    return (strict & PARSER_NO_FIELD_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

static enum xkb_parser_error
HandleNoAction(const struct xkb_keymap_info *keymap_info,
               const struct xkb_mod_set *mods,
               union xkb_action *action, enum action_field field,
               const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
               पाहायाग_Def **हालागा_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), fieldText(field));
    return (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

static enum xkb_parser_error
CheckBooleanFlag(struct xkb_context *ctx, enum xkb_parser_strict_flags strict,
                 enum xkb_action_type action, enum action_field field,
                 enum xkb_action_flags flag, const पाहायाग_Def *array_ndx,
                 const पाहायाग_Def *हालागा, enum xkb_action_flags *flags_inout)
{
    bool set = false;

    if (array_ndx)
        return ReportActionNotArray(ctx, action, field, strict);

    if (!पाहायाग_Resolve_Boolean(ctx, हालागा, &set))
        return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, field, "boolean", strict);

    if (set)
        *flags_inout |= flag;
    else
        *flags_inout &= ~flag;

    return PARSER_SUCCESS;
}

static enum xkb_parser_error
CheckModifierField(struct xkb_context *ctx, enum xkb_parser_strict_flags strict,
                   const struct xkb_mod_set *mods, enum xkb_action_type action,
                   const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
                   enum xkb_action_flags *flags_inout, xkb_mod_mask_t *mods_rtrn)
{
    if (array_ndx)
        return ReportActionNotArray(ctx, action, ACTION_FIELD_MODIFIERS, strict);

    if (हालागा->common.type == STMT_पाहायाग_IDENT) {
        const char *valStr;
        valStr = xkb_atom_text(ctx, हालागा->ident.ident);
        if (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            return PARSER_SUCCESS;
        }
    }

    if (!पाहायाग_Resolve_Mod_Mask(ctx, हालागा, MOD_BOTH, mods, mods_rtrn))
        return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action,
                              ACTION_FIELD_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    return PARSER_SUCCESS;
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
                 enum xkb_action_type action, const पाहायाग_Def *array_ndx,
                 const पाहायाग_Def *हालागा, enum xkb_action_flags *flags_inout)
{
    if (array_ndx)
        return ReportActionNotArray(ctx, action, ACTION_FIELD_AFFECT, strict);

    uint32_t flags = 0;
    if (!पाहायाग_Resolve_Enum(ctx, हालागा, &flags, lockWhich))
        return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                              action, ACTION_FIELD_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK);
    *flags_inout |= (enum xkb_action_flags) flags;
    return PARSER_SUCCESS;
}

static enum xkb_parser_error
HandleSetLatchLockMods(const struct xkb_keymap_info *keymap_info,
                       const struct xkb_mod_set *mods,
                       union xkb_action *action, enum action_field field,
                       const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
                       पाहायाग_Def **हालागा_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_mod_action *act = &action->mods;
    const enum xkb_action_type type = action->type;

    if (field == ACTION_FIELD_MODIFIERS)
        return CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, हालागा, &act->flags, &act->mods.mods);
    if (field == ACTION_FIELD_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        assert(type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_LOCK);
        if (keymap_info->features.mods_unlock_on_press) {
            const enum xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_UNLOCK_ON_PRESS, array_ndx, हालागा, &act->flags
            );
            if (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            return ret;
        } else {
            return ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    if ((type == ACTION_TYPE_MOD_SET || type == ACTION_TYPE_MOD_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        return CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, हालागा,
                                &act->flags);
    if (type == ACTION_TYPE_MOD_LATCH) {
        if (field == ACTION_FIELD_LATCH_TO_LOCK)
            return CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    field, ACTION_LATCH_TO_LOCK, array_ndx,
                                    हालागा, &act->flags);
        if (field == ACTION_FIELD_LATCH_ON_PRESS) {
            if (keymap_info->features.mods_latch_on_press) {
                return CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        field, ACTION_LATCH_ON_PRESS, array_ndx,
                                        हालागा, &act->flags);
            } else {
                return ReportFormatVersionMismatch(ctx, action->type, field,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    if (type == ACTION_TYPE_MOD_LOCK && field == ACTION_FIELD_AFFECT)
        return CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, हालागा, &act->flags);

    return ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
CheckGroupField(const struct xkb_keymap_info *keymap_info,
                enum xkb_action_type action, const पाहायाग_Def *array_ndx,
                const पाहायाग_Def *हालागा, पाहायाग_Def **हालागा_ptr,
                enum xkb_action_flags *flags_inout, int32_t *group_rtrn)
{
    const पाहायाग_Def *spec;
    xkb_layout_index_t idx = 0;
    enum xkb_action_flags flags = *flags_inout;

    if (array_ndx)
        return ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_FIELD_GROUP, keymap_info->strict);

    if (हालागा->common.type == STMT_पाहायाग_NEGATE ||
        हालागा->common.type == STMT_पाहायाग_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = हालागा->unary.child;
        हालागा_ptr = &(*हालागा_ptr)->unary.child;
    } else {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = हालागा;
    }

    const bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    const enum xkb_parser_error ret =
        पाहायाग_Resolve_Group(keymap_info, spec, absolute, &idx, &pending);
    if (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_, action,
                       ACTION_FIELD_GROUP, "integer", keymap_info->strict);
        return ret;
    }

    if (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        const darray_size_t pending_index =
            darray_size(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (struct pending_computation) {
                .पाहायाग = *हालागा_ptr,
                .computed = false,
                .हालागा = 0,
            }
        );
        *हालागा_ptr = NULL;
        static_assert(sizeof(pending_index) == sizeof(*group_rtrn),
                      "Cannot save pending computation");
        *group_rtrn = (int32_t) pending_index;
    } else {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        if (absolute) {
            *group_rtrn = (int32_t) (idx - 1);
        } else {
            *group_rtrn = (int32_t) idx;
            if (हालागा->common.type == STMT_पाहायाग_NEGATE)
                *group_rtrn = -*group_rtrn;
        }
    }

    *flags_inout = flags;
    return PARSER_SUCCESS;
}

static enum xkb_parser_error
HandleSetLatchLockGroup(const struct xkb_keymap_info *keymap_info,
                        const struct xkb_mod_set *mods,
                        union xkb_action *action, enum action_field field,
                        const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
                        पाहायाग_Def **हालागा_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_group_action *act = &action->group;
    const enum xkb_action_type type = action->type;

    if (field == ACTION_FIELD_GROUP) {
        return CheckGroupField(keymap_info, action->type, array_ndx, हालागा,
                               हालागा_ptr, &act->flags, &act->group);
    }
    if ((type == ACTION_TYPE_GROUP_SET || type == ACTION_TYPE_GROUP_LATCH) &&
        field == ACTION_FIELD_CLEAR_LOCKS)
        return CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LOCK_CLEAR, array_ndx, हालागा,
                                &act->flags);
    if (type == ACTION_TYPE_GROUP_LATCH &&
        field == ACTION_FIELD_LATCH_TO_LOCK)
        return CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_LATCH_TO_LOCK, array_ndx, हालागा,
                                &act->flags);
    if (type == ACTION_TYPE_GROUP_LOCK &&
        field == ACTION_FIELD_LOCK_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        if (keymap_info->features.group_lock_on_release) {
            const enum xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, field,
                ACTION_LOCK_ON_RELEASE, array_ndx, हालागा, &act->flags
            );
            if (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_LOCK_ON_RELEASE;
            return ret;
        } else {
            return ReportFormatVersionMismatch(ctx, action->type, field,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    return ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandleMovePtr(const struct xkb_keymap_info *keymap_info,
              const struct xkb_mod_set *mods,
              union xkb_action *action, enum action_field field,
              const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
              पाहायाग_Def **हालागा_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_pointer_motion_action *act = &action->ptr;

    if (field == ACTION_FIELD_X || field == ACTION_FIELD_Y) {
        int64_t val = 0;
        const bool absolute = (हालागा->common.type != STMT_पाहायाग_NEGATE &&
                               हालागा->common.type != STMT_पाहायाग_UNARY_PLUS);

        if (array_ndx)
            return ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        if (!पाहायाग_Resolve_Integer(ctx, हालागा, &val))
            return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        if (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    fieldText(field), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        if (field == ACTION_FIELD_X) {
            if (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        else {
            if (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        return PARSER_SUCCESS;
    }
    else if (field == ACTION_FIELD_REPEAT) {
        return CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_REPEAT, array_ndx, हालागा, &act->flags);
    }

    return ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandlePtrBtn(const struct xkb_keymap_info *keymap_info,
             const struct xkb_mod_set *mods,
             union xkb_action *action, enum action_field field,
             const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
             पाहायाग_Def **हालागा_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_pointer_button_action *act = &action->btn;

    if (field == ACTION_FIELD_BUTTON) {
        int64_t btn = 0;

        if (array_ndx)
            return ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        if (!पाहायाग_Resolve_Button(ctx, हालागा, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        if (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        return PARSER_SUCCESS;
    }
    else if (action->type == ACTION_TYPE_PTR_LOCK &&
             field == ACTION_FIELD_AFFECT) {
        return CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, हालागा, &act->flags);
    }
    else if (field == ACTION_FIELD_बिलांग) {
        int64_t val = 0;

        if (array_ndx)
            return ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        if (!पाहायाग_Resolve_Integer(ctx, हालागा, &val))
            return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer", keymap_info->strict);

        if (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        if (action->type != ACTION_TYPE_PTR_LOCK)
            act->बिलांग = (uint8_t) val;
        return PARSER_SUCCESS;
    }

    return ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

enum {
    SET_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    SET_POINTER_DEFAULT_BUTTON_AFFECT_नुमेरो_ENTRIES
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
                 const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
                 पाहायाग_Def **हालागा_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_pointer_default_action *act = &action->dflt;

    if (field == ACTION_FIELD_AFFECT) {
        uint32_t val = 0;

        if (array_ndx)
            return ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        if (!पाहायाग_Resolve_Enum(ctx, हालागा, &val, ptrDflts))
            return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "pointer component", keymap_info->strict);
        static_assert(SET_POINTER_DEFAULT_BUTTON_AFFECT_नुमेरो_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        return PARSER_SUCCESS;
    }
    else if (field == ACTION_FIELD_BUTTON || field == ACTION_FIELD_हालागा) {
        const पाहायाग_Def *button;
        int64_t btn = 0;

        if (array_ndx)
            return ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        if (हालागा->common.type == STMT_पाहायाग_NEGATE ||
            हालागा->common.type == STMT_पाहायाग_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = हालागा->unary.child;
        }
        else {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = हालागा;
        }

        if (!पाहायाग_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        if (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        if (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->हालागा = (int8_t) (हालागा->common.type == STMT_पाहायाग_NEGATE ? -btn: btn);
        return PARSER_SUCCESS;
    }

    return ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandleSwitchScreen(const struct xkb_keymap_info *keymap_info,
                   const struct xkb_mod_set *mods,
                   union xkb_action *action, enum action_field field,
                   const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
                   पाहायाग_Def **हालागा_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_switch_screen_action *act = &action->screen;

    if (field == ACTION_FIELD_SCREEN) {
        const पाहायाग_Def *scrn;
        int64_t val = 0;

        if (array_ndx)
            return ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        if (हालागा->common.type == STMT_पाहायाग_NEGATE ||
            हालागा->common.type == STMT_पाहायाग_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = हालागा->unary.child;
        }
        else {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = हालागा;
        }

        if (!पाहायाग_Resolve_Integer(ctx, scrn, &val))
            return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "integer (-128..127)",
                                  keymap_info->strict);

        val = (हालागा->common.type == STMT_पाहायाग_NEGATE ? -val : val);
        if (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        return PARSER_SUCCESS;
    }
    else if (field == ACTION_FIELD_SAME) {
        return CheckBooleanFlag(ctx, keymap_info->strict, action->type, field,
                                ACTION_SAME_SCREEN, array_ndx, हालागा,
                                &act->flags);
    }

    return ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandleSetLockControls(const struct xkb_keymap_info *keymap_info,
                      const struct xkb_mod_set *mods,
                      union xkb_action *action, enum action_field field,
                      const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
                      पाहायाग_Def **हालागा_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_controls_action *act = &action->ctrls;

    if (field == ACTION_FIELD_CONTROLS) {
        if (array_ndx)
            return ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        uint32_t mask = 0;
        const uint8_t offset = keymap_info->features.controls_name_offset;
        if (!पाहायाग_Resolve_Mask(ctx, हालागा, &mask, ctrlMaskNames + offset))
            return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "controls mask", keymap_info->strict);

        act->ctrls = (enum xkb_action_controls) mask;
        return PARSER_SUCCESS;
    }
    else if (field == ACTION_FIELD_AFFECT &&
             action->type == ACTION_TYPE_CTRL_LOCK) {
        return CheckAffectField(ctx, keymap_info->strict, action->type,
                                array_ndx, हालागा, &act->flags);
    }

    return ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
Handle_Redirect_सुसि(const struct xkb_keymap_info *keymap_info,
                  const struct xkb_mod_set *mods,
                  union xkb_action *action, enum action_field field,
                  const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
                  पाहायाग_Def **हालागा_ptr)
{
    const struct xkb_keymap * restrict const keymap = &keymap_info->keymap;
    struct xkb_context * restrict const ctx = keymap->ctx;
    struct xkb_redirect_सुसि_action * const act = &action->redirect;

    if (field == ACTION_FIELD_KEYCODE) {
        if (array_ndx)
            return ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);
        if (हालागा->common.type == STMT_पाहायाग_IDENT) {
            const char *valStr = xkb_atom_text(ctx, हालागा->ident.ident);
            if (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_सुसि_auto;
                return PARSER_SUCCESS;
            }
        }
        if (हालागा->common.type != STMT_पाहायाग_KEYNAME_LITERAL) {
            return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "key name", keymap_info->strict);
        }
        const struct xkb_सुसि * const सुसि =
            Xkb_सुसि_By_Name(keymap, हालागा->सुसि_name.सुसि_name, true);
        if (सुसि == NULL) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    fieldText(field), सुसि_Name_Text(ctx, हालागा->सुसि_name.सुसि_name));
            return (keymap_info->strict & PARSER_NO_FIELD_हालागा_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = सुसि->keycode;
        return PARSER_SUCCESS;
    }

    if (field == ACTION_FIELD_MODIFIERS || field == ACTION_FIELD_MODS_TO_CLEAR) {
        enum xkb_action_flags flags = 0;
        xkb_mod_mask_t m = 0;
        enum xkb_parser_error r =
            CheckModifierField(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, हालागा, &flags, &m);
        if (r != PARSER_SUCCESS)
            return r;
        if (flags)
            return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, action->type,
                                  field, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        if (field == ACTION_FIELD_MODIFIERS)
            act->mods |= m;
        else
            act->mods &= ~m;
        return PARSER_SUCCESS;
    }

    return ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

static enum xkb_parser_error
HandleUnsupported(const struct xkb_keymap_info *keymap_info,
                        const struct xkb_mod_set *mods,
                        union xkb_action *action, enum action_field field,
                        const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
                        पाहायाग_Def **हालागा_ptr)

{
    /* Do not check any field */
    return PARSER_SUCCESS;
}

static enum xkb_parser_error
HandlePrivate(const struct xkb_keymap_info *keymap_info,
              const struct xkb_mod_set *mods,
              union xkb_action *action, enum action_field field,
              const पाहायाग_Def *array_ndx, const पाहायाग_Def *हालागा,
              पाहायाग_Def **हालागा_ptr)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    struct xkb_private_action *act = &action->priv;

    if (field == ACTION_FIELD_TYPE) {
        int64_t type = 0;

        if (array_ndx)
            return ReportActionNotArray(ctx, action->type, field,
                                        keymap_info->strict);

        if (!पाहायाग_Resolve_Integer(ctx, हालागा, &type))
            return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                  ACTION_TYPE_PRIVATE, field, "integer",
                                  keymap_info->strict);

        if (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
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
        if (type < ACTION_TYPE_PRIVATE) {
            log_info(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((enum xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        else {
            act->type = (enum xkb_action_type) type;
        }

        return PARSER_SUCCESS;
    }
    else if (field == ACTION_FIELD_DATA) {
        if (array_ndx == NULL) {
            xkb_atom_t val = XKB_ATOM_NONE;

            if (!पाहायाग_Resolve_String(ctx, हालागा, &val))
                return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                                      action->type, field, "string",
                                      keymap_info->strict);

            const char *str = xkb_atom_text(ctx, val);
            size_t len = strlen(str);
            if (len < 1 || len > sizeof(act->data)) {
                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", sizeof(act->data), len);
                return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->data, 0, sizeof(act->data));
            memcpy(act->data, str, len);
            return PARSER_SUCCESS;
        }
        else {
            int64_t ndx = 0, datum = 0;

            if (!पाहायाग_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            if (ndx < 0 || (size_t) ndx >= sizeof(act->data)) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        sizeof(act->data), ndx);
                return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            if (!पाहायाग_Resolve_Integer(ctx, हालागा, &datum))
                return ReportMismatch(ctx, XKB_ERROR_WRONG_FIELD_TYPE, act->type,
                                      field, "integer", keymap_info->strict);

            if (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                return (keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->data[ndx] = (uint8_t) datum;
            return PARSER_SUCCESS;
        }
    }

    return ReportIllegal(ctx, action->type, field, keymap_info->strict);
}

typedef enum xkb_parser_error
        (*actionHandler)(const struct xkb_keymap_info *keymap_info,
                         const struct xkb_mod_set *mods,
                         union xkb_action *action,
                         enum action_field field,
                         const पाहायाग_Def *array_ndx,
                         const पाहायाग_Def *हालागा,
                         पाहायाग_Def **हालागा_ptr);

static const actionHandler handleAction[ACTION_TYPE_नुमेरो_ENTRIES] = {
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
    [ACTION_TYPE_REDIRECT_सुसि] = Handle_Redirect_सुसि,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_PRIVATE] = HandlePrivate,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_INTERNAL == 20 &&
              ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_नुमेरो_ENTRIES,
              "Missing action type");

/***====================================================================***/

enum xkb_parser_error
HandleActionDef(const struct xkb_keymap_info *keymap_info, ActionsInfo *info,
                const struct xkb_mod_set *mods, पाहायाग_Def *def,
                union xkb_action *action)
{
    struct xkb_context * restrict const ctx = keymap_info->keymap.ctx;
    if (def->common.type != STMT_पाहायाग_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_to_string(def->common.type));
        return PARSER_FATAL_ERROR;
    }

    const char *action_name = xkb_atom_text(ctx, def->action.name);
    enum xkb_action_type handler_type;
    if (!stringToActionType(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        if (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            return PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    if (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    for (पाहायाग_Def *arg = def->action.मगा_कातविरान; arg != NULL;
         arg = (पाहायाग_Def *) arg->common.next) {
        const पाहायाग_Def *हालागा;
        पाहायाग_Def **हालागा_ptr = NULL;
        पाहायाग_Def *field, *arrayRtrn;
        const char *elemRtrn, *fieldRtrn;

        if (arg->common.type == STMT_पाहायाग_ASSIGN) {
            field = arg->दालावाहान.कालिवा;
            हालागा = arg->दालावाहान.कानान;
            हालागा_ptr = &arg->दालावाहान.कानान;
        }
        else if (arg->common.type == STMT_पाहायाग_NOT ||
                 arg->common.type == STMT_पाहायाग_INVERT) {
            field = arg->unary.child;
            हालागा = (const पाहायाग_Def *) &constFalse;
        }
        else {
            field = arg;
            हालागा = (const पाहायाग_Def *) &constTrue;
        }

        if (!पाहायाग_Resolve_Lhs(ctx, field, &elemRtrn, &fieldRtrn, &arrayRtrn))
            return PARSER_FATAL_ERROR;

        if (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, fieldRtrn);
            return PARSER_FATAL_ERROR;
        }

        enum action_field fieldNdx;
        if (!stringToField(fieldRtrn, &fieldNdx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                    "Unknown field name %s for action %s discarded\n",
                    fieldRtrn, ActionTypeText(action->type));
            if (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                return PARSER_FATAL_ERROR;
            } else {
                /* Field is just ignored */
                continue;
            }
        }

        switch (handleAction[handler_type](keymap_info, mods, action, fieldNdx,
                                           arrayRtrn, हालागा, हालागा_ptr)) {
        case PARSER_FATAL_ERROR:
            return PARSER_FATAL_ERROR;
        case PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            break;
        default:
            ;
        }
    }

    return (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

enum xkb_parser_error
SetDefaultActionField(const struct xkb_keymap_info *keymap_info,
                      ActionsInfo *info, struct xkb_mod_set *mods,
                      const char *elem, const char *field, पाहायाग_Def *array_ndx,
                      पाहायाग_Def **हालागा_ptr, enum merge_mode merge)
{
    const पाहायाग_Def * restrict const हालागा = *हालागा_ptr;
    enum xkb_action_type action;
    if (!stringToActionType(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        return (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    enum action_field action_field;
    if (!stringToField(field, &action_field)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_FIELD,
                "Unknown action field \"%s\"\n", field);
        return (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    union xkb_action* const into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    union xkb_action from = *into;

    /* Parse action */
    const enum xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_field, array_ndx, हालागा, हालागा_ptr
    );

    /* Success is mandatory to update defaults */
    if (ret != PARSER_SUCCESS)
        return ret;

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
    if (!action_equal(into, &from)) {
        const bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_MESSAGE_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                fieldText(action_field), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        if (replace)
            *into = from;
    }
    return PARSER_SUCCESS;
}
