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

#شامل "config.h"

#شامل <assert.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "context.h"
#شامل "darray.h"
#شامل "keymap.h"
#شامل "messages-codes.h"
#شامل "xkbcomp-priv.h"
#شامل "text.h"
#شامل "expr.h"
#شامل "xkbcomp/ast.h"
#شامل "action.h"

ساکن ثابت اظہاریہ_Boolean constTrue = {
    .common = { .type = STMT_اظہاریہ_BOOLEAN_LITERAL, .اگلا = تہی },
    .سیٹ = true,
};

ساکن ثابت اظہاریہ_Boolean constFalse = {
    .common = { .type = STMT_اظہاریہ_BOOLEAN_LITERAL, .اگلا = تہی },
    .سیٹ = false,
};

شمار action_میدان {
    ACTION_میدان_CLEAR_LOCKS,
    ACTION_میدان_LATCH_مقصد_تالا,
    ACTION_میدان_GEN_کلید_EVENT,
    ACTION_میدان_REPORT,
    ACTION_میدان_DEFAULT,
    ACTION_میدان_AFFECT,
    ACTION_میدان_INCREMENT,
    ACTION_میدان_MODIFIERS,
    ACTION_میدان_گروہ,
    ACTION_میدان_X,
    ACTION_میدان_Y,
    ACTION_میدان_REPEAT,
    ACTION_میدان_BUTTON,
    ACTION_میدان_قدر,
    ACTION_میدان_CONTROLS,
    ACTION_میدان_TYPE,
    ACTION_میدان_گنتی,
    ACTION_میدان_SCREEN,
    ACTION_میدان_SAME,
    ACTION_میدان_ڈیٹا,
    ACTION_میدان_DEVICE,
    ACTION_میدان_KEYCODE,
    ACTION_میدان_MODS_مقصد_CLEAR,
    ACTION_میدان_تالا_ON_RELEASE,
    ACTION_میدان_UNLOCK_ON_PRESS,
    ACTION_میدان_LATCH_ON_PRESS,
};

خالی
InitActionsInfo(ثابت ساخت xkb_keymap *keymap, ActionsInfo *info)
{
    برائے (شمار xkb_action_type type = 0; type < ACTION_TYPE_عدد_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.قدر = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_کلید].redirect.keycode =
        keymap->redirect_کلید_auto;
}

ساکن ثابت Lookup_اندراج میدان_لڑیاں[] = {
    { "clearLocks",       ACTION_میدان_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_میدان_LATCH_مقصد_تالا   },
    { "genKeyEvent",      ACTION_میدان_GEN_کلید_EVENT   },
    { "generateKeyEvent", ACTION_میدان_GEN_کلید_EVENT   },
    { "report",           ACTION_میدان_REPORT          },
    { "default",          ACTION_میدان_DEFAULT         },
    { "affect",           ACTION_میدان_AFFECT          },
    { "increment",        ACTION_میدان_INCREMENT       },
    { "modifiers",        ACTION_میدان_MODIFIERS       },
    { "mods",             ACTION_میدان_MODIFIERS       },
    { "group",            ACTION_میدان_گروہ           },
    { "x",                ACTION_میدان_X               },
    { "y",                ACTION_میدان_Y               },
    { "repeat",           ACTION_میدان_REPEAT          },
    { "accel",            ACTION_میدان_REPEAT          },
    { "accelerate",       ACTION_میدان_REPEAT          },
    { "button",           ACTION_میدان_BUTTON          },
    { "value",            ACTION_میدان_قدر           },
    { "controls",         ACTION_میدان_CONTROLS        },
    { "ctrls",            ACTION_میدان_CONTROLS        },
    { "type",             ACTION_میدان_TYPE            },
    { "count",            ACTION_میدان_گنتی           },
    { "screen",           ACTION_میدان_SCREEN          },
    { "same",             ACTION_میدان_SAME            },
    { "sameServer",       ACTION_میدان_SAME            },
    { "data",             ACTION_میدان_ڈیٹا            },
    { "device",           ACTION_میدان_DEVICE          },
    { "dev",              ACTION_میدان_DEVICE          },
    { "key",              ACTION_میدان_KEYCODE         },
    { "keycode",          ACTION_میدان_KEYCODE         },
    { "kc",               ACTION_میدان_KEYCODE         },
    { "clearmods",        ACTION_میدان_MODS_مقصد_CLEAR   },
    { "clearmodifiers",   ACTION_میدان_MODS_مقصد_CLEAR   },
    { "lockOnRelease",    ACTION_میدان_تالا_ON_RELEASE },
    { "unlockOnPress",    ACTION_میدان_UNLOCK_ON_PRESS },
    { "latchOnPress",     ACTION_میدان_LATCH_ON_PRESS  },
    { تہی,               0                            }
};

ساکن bool
string_مقصد_Action_Type(ثابت حرف *str, شمار xkb_action_type *type_rtrn)
{
    غیرعلامتی صحیح type = 0;
    ثابت bool ret = LookupString(action_Type_نام, str, &type);
    *type_rtrn = (شمار xkb_action_type) type;
    واپس ret;
}

ساکن bool
string_مقصد_میدان(ثابت حرف *str, شمار action_میدان *میدان_rtrn)
{
    غیرعلامتی صحیح میدان_2 = 0;
    ثابت bool ret = LookupString(میدان_لڑیاں, str, &میدان_2);
    *میدان_rtrn = (شمار action_میدان) میدان_2;
    واپس ret;
}

ساکن ثابت حرف *
میدان_متن(شمار action_میدان میدان_2)
{
    واپس Lookup_قدر(میدان_لڑیاں, میدان_2);
}

/***====================================================================***/

ساکن اندرونی شمار xkb_parser_error
ReportMismatch(ساخت xkb_context *ctx, شمار xkb_پیغام_رمز رمز,
               شمار xkb_action_type action, شمار action_میدان میدان_2,
               ثابت حرف *type, شمار xkb_parser_strict_flags strict)
{
    log_err(ctx, رمز,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            میدان_متن(میدان_2), type, Action_Type_متن(action));
    واپس (strict & PARSER_NO_میدان_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساکن اندرونی شمار xkb_parser_error
Report_Format_نسخہ_Mismatch(ساخت xkb_context *ctx,
                            شمار xkb_action_type action,
                            شمار action_میدان میدان_2,
                            شمار xkb_keymap_format format,
                            ثابت حرف *versions,
                            شمار xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            میدان_متن(میدان_2), Action_Type_متن(action), versions, format);
    واپس (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساکن اندرونی شمار xkb_parser_error
ReportIllegal(ساخت xkb_context *ctx, شمار xkb_action_type action,
              شمار action_میدان میدان_2, شمار xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_میدان,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            میدان_متن(میدان_2), Action_Type_متن(action));
    واپس (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساکن اندرونی شمار xkb_parser_error
ReportActionNotArray(ساخت xkb_context *ctx, شمار xkb_action_type action,
                     شمار action_میدان میدان_2,
                     شمار xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            میدان_متن(میدان_2), Action_Type_متن(action));
    واپس (strict & PARSER_NO_میدان_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساکن شمار xkb_parser_error
HandleNoAction(ثابت ساخت xkb_keymap_info *keymap_info,
               ثابت ساخت xkb_mod_سیٹ *mods,
               اتحاد xkb_action *action, شمار action_میدان میدان_2,
               ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
               اظہاریہ_Def **قدر_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_میدان,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            Action_Type_متن(action->type), میدان_متن(میدان_2));
    واپس (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساکن شمار xkb_parser_error
CheckBooleanFlag(ساخت xkb_context *ctx, شمار xkb_parser_strict_flags strict,
                 شمار xkb_action_type action, شمار action_میدان میدان_2,
                 شمار xkb_action_flags flag, ثابت اظہاریہ_Def *array_ndx,
                 ثابت اظہاریہ_Def *قدر, شمار xkb_action_flags *flags_inout)
{
    bool سیٹ = false;

    اگر (array_ndx)
        واپس ReportActionNotArray(ctx, action, میدان_2, strict);

    اگر (!اظہاریہ_Resolve_Boolean(ctx, قدر, &سیٹ))
        واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                              action, میدان_2, "boolean", strict);

    اگر (سیٹ)
        *flags_inout |= flag;
    ورنہ
        *flags_inout &= ~flag;

    واپس PARSER_SUCCESS;
}

ساکن شمار xkb_parser_error
Check_Modifier_میدان(ساخت xkb_context *ctx, شمار xkb_parser_strict_flags strict,
                   ثابت ساخت xkb_mod_سیٹ *mods, شمار xkb_action_type action,
                   ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                   شمار xkb_action_flags *flags_inout, xkb_mod_نقاب_t *mods_rtrn)
{
    اگر (array_ndx)
        واپس ReportActionNotArray(ctx, action, ACTION_میدان_MODIFIERS, strict);

    اگر (قدر->common.type == STMT_اظہاریہ_IDENT) {
        ثابت حرف *valStr;
        valStr = xkb_atom_متن(ctx, قدر->ident.ident);
        اگر (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            واپس PARSER_SUCCESS;
        }
    }

    اگر (!اظہاریہ_Resolve_Mod_نقاب(ctx, قدر, MOD_BOTH, mods, mods_rtrn))
        واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action,
                              ACTION_میدان_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    واپس PARSER_SUCCESS;
}

ساکن ثابت Lookup_اندراج تالا_Which[] = {
    { "both", 0 },
    { "lock", ACTION_تالا_NO_UNLOCK },
    { "neither", (ACTION_تالا_NO_تالا | ACTION_تالا_NO_UNLOCK) },
    { "unlock", ACTION_تالا_NO_تالا },
    { تہی, 0 }
};

ساکن شمار xkb_parser_error
Check_Affect_میدان(ساخت xkb_context *ctx, شمار xkb_parser_strict_flags strict,
                 شمار xkb_action_type action, ثابت اظہاریہ_Def *array_ndx,
                 ثابت اظہاریہ_Def *قدر, شمار xkb_action_flags *flags_inout)
{
    اگر (array_ndx)
        واپس ReportActionNotArray(ctx, action, ACTION_میدان_AFFECT, strict);

    uint32_t flags = 0;
    اگر (!اظہاریہ_Resolve_Enum(ctx, قدر, &flags, تالا_Which))
        واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                              action, ACTION_میدان_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_تالا_NO_تالا | ACTION_تالا_NO_UNLOCK);
    *flags_inout |= (شمار xkb_action_flags) flags;
    واپس PARSER_SUCCESS;
}

ساکن شمار xkb_parser_error
Handle_سیٹ_Latch_تالا_Mods(ثابت ساخت xkb_keymap_info *keymap_info,
                       ثابت ساخت xkb_mod_سیٹ *mods,
                       اتحاد xkb_action *action, شمار action_میدان میدان_2,
                       ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                       اظہاریہ_Def **قدر_ptr)
{
    ساخت xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    ساخت xkb_mod_action *act = &action->mods;
    ثابت شمار xkb_action_type type = action->type;

    اگر (میدان_2 == ACTION_میدان_MODIFIERS)
        واپس Check_Modifier_میدان(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, قدر, &act->flags, &act->mods.mods);
    اگر (میدان_2 == ACTION_میدان_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        أكد(type == ACTION_TYPE_MOD_سیٹ || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_تالا);
        اگر (keymap_info->features.mods_unlock_on_press) {
            ثابت شمار xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, میدان_2,
                ACTION_UNLOCK_ON_PRESS, array_ndx, قدر, &act->flags
            );
            اگر (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            واپس ret;
        } ورنہ {
            واپس Report_Format_نسخہ_Mismatch(ctx, action->type, میدان_2,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    اگر ((type == ACTION_TYPE_MOD_سیٹ || type == ACTION_TYPE_MOD_LATCH) &&
        میدان_2 == ACTION_میدان_CLEAR_LOCKS)
        واپس CheckBooleanFlag(ctx, keymap_info->strict, action->type, میدان_2,
                                ACTION_تالا_CLEAR, array_ndx, قدر,
                                &act->flags);
    اگر (type == ACTION_TYPE_MOD_LATCH) {
        اگر (میدان_2 == ACTION_میدان_LATCH_مقصد_تالا)
            واپس CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    میدان_2, ACTION_LATCH_مقصد_تالا, array_ndx,
                                    قدر, &act->flags);
        اگر (میدان_2 == ACTION_میدان_LATCH_ON_PRESS) {
            اگر (keymap_info->features.mods_latch_on_press) {
                واپس CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        میدان_2, ACTION_LATCH_ON_PRESS, array_ndx,
                                        قدر, &act->flags);
            } ورنہ {
                واپس Report_Format_نسخہ_Mismatch(ctx, action->type, میدان_2,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    اگر (type == ACTION_TYPE_MOD_تالا && میدان_2 == ACTION_میدان_AFFECT)
        واپس Check_Affect_میدان(ctx, keymap_info->strict, action->type,
                                array_ndx, قدر, &act->flags);

    واپس ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساکن شمار xkb_parser_error
Check_گروہ_میدان(ثابت ساخت xkb_keymap_info *keymap_info,
                شمار xkb_action_type action, ثابت اظہاریہ_Def *array_ndx,
                ثابت اظہاریہ_Def *قدر, اظہاریہ_Def **قدر_ptr,
                شمار xkb_action_flags *flags_inout, int32_t *گروہ_rtrn)
{
    ثابت اظہاریہ_Def *spec;
    xkb_layout_اشاریہ_t idx = 0;
    شمار xkb_action_flags flags = *flags_inout;

    اگر (array_ndx)
        واپس ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_میدان_گروہ, keymap_info->strict);

    اگر (قدر->common.type == STMT_اظہاریہ_NEGATE ||
        قدر->common.type == STMT_اظہاریہ_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = قدر->unary.بچہ;
        قدر_ptr = &(*قدر_ptr)->unary.بچہ;
    } ورنہ {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = قدر;
    }

    ثابت bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ثابت شمار xkb_parser_error ret =
        اظہاریہ_Resolve_گروہ(keymap_info, spec, absolute, &idx, &pending);
    اگر (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2, action,
                       ACTION_میدان_گروہ, "integer", keymap_info->strict);
        واپس ret;
    }

    اگر (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ثابت darray_سائز_t pending_اشاریہ =
            darray_سائز(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (ساخت pending_computation) {
                .اظہاریہ = *قدر_ptr,
                .computed = false,
                .قدر = 0,
            }
        );
        *قدر_ptr = تہی;
        static_assert(حجم(pending_اشاریہ) == حجم(*گروہ_rtrn),
                      "Cannot save pending computation");
        *گروہ_rtrn = (int32_t) pending_اشاریہ;
    } ورنہ {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        اگر (absolute) {
            *گروہ_rtrn = (int32_t) (idx - 1);
        } ورنہ {
            *گروہ_rtrn = (int32_t) idx;
            اگر (قدر->common.type == STMT_اظہاریہ_NEGATE)
                *گروہ_rtrn = -*گروہ_rtrn;
        }
    }

    *flags_inout = flags;
    واپس PARSER_SUCCESS;
}

ساکن شمار xkb_parser_error
Handle_سیٹ_Latch_تالا_گروہ(ثابت ساخت xkb_keymap_info *keymap_info,
                        ثابت ساخت xkb_mod_سیٹ *mods,
                        اتحاد xkb_action *action, شمار action_میدان میدان_2,
                        ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                        اظہاریہ_Def **قدر_ptr)
{
    ساخت xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    ساخت xkb_گروہ_action *act = &action->گروہ;
    ثابت شمار xkb_action_type type = action->type;

    اگر (میدان_2 == ACTION_میدان_گروہ) {
        واپس Check_گروہ_میدان(keymap_info, action->type, array_ndx, قدر,
                               قدر_ptr, &act->flags, &act->گروہ);
    }
    اگر ((type == ACTION_TYPE_گروہ_سیٹ || type == ACTION_TYPE_گروہ_LATCH) &&
        میدان_2 == ACTION_میدان_CLEAR_LOCKS)
        واپس CheckBooleanFlag(ctx, keymap_info->strict, action->type, میدان_2,
                                ACTION_تالا_CLEAR, array_ndx, قدر,
                                &act->flags);
    اگر (type == ACTION_TYPE_گروہ_LATCH &&
        میدان_2 == ACTION_میدان_LATCH_مقصد_تالا)
        واپس CheckBooleanFlag(ctx, keymap_info->strict, action->type, میدان_2,
                                ACTION_LATCH_مقصد_تالا, array_ndx, قدر,
                                &act->flags);
    اگر (type == ACTION_TYPE_گروہ_تالا &&
        میدان_2 == ACTION_میدان_تالا_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        اگر (keymap_info->features.گروہ_تالا_on_release) {
            ثابت شمار xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, میدان_2,
                ACTION_تالا_ON_RELEASE, array_ndx, قدر, &act->flags
            );
            اگر (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_تالا_ON_RELEASE;
            واپس ret;
        } ورنہ {
            واپس Report_Format_نسخہ_Mismatch(ctx, action->type, میدان_2,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    واپس ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساکن شمار xkb_parser_error
HandleMovePtr(ثابت ساخت xkb_keymap_info *keymap_info,
              ثابت ساخت xkb_mod_سیٹ *mods,
              اتحاد xkb_action *action, شمار action_میدان میدان_2,
              ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
              اظہاریہ_Def **قدر_ptr)
{
    ساخت xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    ساخت xkb_اشارہ_گر_motion_action *act = &action->ptr;

    اگر (میدان_2 == ACTION_میدان_X || میدان_2 == ACTION_میدان_Y) {
        int64_t val = 0;
        ثابت bool absolute = (قدر->common.type != STMT_اظہاریہ_NEGATE &&
                               قدر->common.type != STMT_اظہاریہ_UNARY_PLUS);

        اگر (array_ndx)
            واپس ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        اگر (!اظہاریہ_Resolve_Integer(ctx, قدر, &val))
            واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "integer", keymap_info->strict);

        اگر (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    میدان_متن(میدان_2), Action_Type_متن(action->type),
                    INT16_MIN, INT16_MAX, val);
            واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        اگر (میدان_2 == ACTION_میدان_X) {
            اگر (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        ورنہ {
            اگر (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        واپس PARSER_SUCCESS;
    }
    ورنہ اگر (میدان_2 == ACTION_میدان_REPEAT) {
        واپس CheckBooleanFlag(ctx, keymap_info->strict, action->type, میدان_2,
                                ACTION_REPEAT, array_ndx, قدر, &act->flags);
    }

    واپس ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساکن شمار xkb_parser_error
HandlePtrBtn(ثابت ساخت xkb_keymap_info *keymap_info,
             ثابت ساخت xkb_mod_سیٹ *mods,
             اتحاد xkb_action *action, شمار action_میدان میدان_2,
             ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
             اظہاریہ_Def **قدر_ptr)
{
    ساخت xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    ساخت xkb_اشارہ_گر_button_action *act = &action->btn;

    اگر (میدان_2 == ACTION_میدان_BUTTON) {
        int64_t btn = 0;

        اگر (array_ndx)
            واپس ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        اگر (!اظہاریہ_Resolve_Button(ctx, قدر, &btn)) {
            static_assert(XKB_اشارہ_گر_BUTTON_MIN == 1 &&
                          XKB_اشارہ_گر_BUTTON_MAX == 5, "invalid message");
            واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_اشارہ_گر_BUTTON_DEFAULT < XKB_اشارہ_گر_BUTTON_MIN, "");
        اگر (btn < XKB_اشارہ_گر_BUTTON_DEFAULT || btn > XKB_اشارہ_گر_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_اشارہ_گر_BUTTON_MIN, XKB_اشارہ_گر_BUTTON_MAX, btn);
            واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        واپس PARSER_SUCCESS;
    }
    ورنہ اگر (action->type == ACTION_TYPE_PTR_تالا &&
             میدان_2 == ACTION_میدان_AFFECT) {
        واپس Check_Affect_میدان(ctx, keymap_info->strict, action->type,
                                array_ndx, قدر, &act->flags);
    }
    ورنہ اگر (میدان_2 == ACTION_میدان_گنتی) {
        int64_t val = 0;

        اگر (array_ndx)
            واپس ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        اگر (!اظہاریہ_Resolve_Integer(ctx, قدر, &val))
            واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "integer", keymap_info->strict);

        اگر (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        اگر (action->type != ACTION_TYPE_PTR_تالا)
            act->گنتی = (uint8_t) val;
        واپس PARSER_SUCCESS;
    }

    واپس ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

شمار {
    سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_عدد_ENTRIES
};

ساکن ثابت Lookup_اندراج ptrDflts[] = {
    { "dfltbtn", سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { تہی, 0 }
};

ساکن شمار xkb_parser_error
Handle_سیٹ_Ptr_Dflt(ثابت ساخت xkb_keymap_info *keymap_info,
                 ثابت ساخت xkb_mod_سیٹ *mods,
                 اتحاد xkb_action *action, شمار action_میدان میدان_2,
                 ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                 اظہاریہ_Def **قدر_ptr)
{
    ساخت xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    ساخت xkb_اشارہ_گر_default_action *act = &action->dflt;

    اگر (میدان_2 == ACTION_میدان_AFFECT) {
        uint32_t val = 0;

        اگر (array_ndx)
            واپس ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        اگر (!اظہاریہ_Resolve_Enum(ctx, قدر, &val, ptrDflts))
            واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "pointer component", keymap_info->strict);
        static_assert(سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_عدد_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        واپس PARSER_SUCCESS;
    }
    ورنہ اگر (میدان_2 == ACTION_میدان_BUTTON || میدان_2 == ACTION_میدان_قدر) {
        ثابت اظہاریہ_Def *button;
        int64_t btn = 0;

        اگر (array_ndx)
            واپس ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        اگر (قدر->common.type == STMT_اظہاریہ_NEGATE ||
            قدر->common.type == STMT_اظہاریہ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = قدر->unary.بچہ;
        }
        ورنہ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = قدر;
        }

        اگر (!اظہاریہ_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_اشارہ_گر_BUTTON_MIN == 1 &&
                          XKB_اشارہ_گر_BUTTON_MAX == 5, "invalid message");
            واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_اشارہ_گر_BUTTON_DEFAULT < XKB_اشارہ_گر_BUTTON_MIN, "");
        اگر (btn < XKB_اشارہ_گر_BUTTON_DEFAULT || btn > XKB_اشارہ_گر_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_اشارہ_گر_BUTTON_MIN, XKB_اشارہ_گر_BUTTON_MAX, btn);
            واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        اگر (btn == XKB_اشارہ_گر_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->قدر = (int8_t) (قدر->common.type == STMT_اظہاریہ_NEGATE ? -btn: btn);
        واپس PARSER_SUCCESS;
    }

    واپس ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساکن شمار xkb_parser_error
HandleSwitchScreen(ثابت ساخت xkb_keymap_info *keymap_info,
                   ثابت ساخت xkb_mod_سیٹ *mods,
                   اتحاد xkb_action *action, شمار action_میدان میدان_2,
                   ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                   اظہاریہ_Def **قدر_ptr)
{
    ساخت xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    ساخت xkb_switch_screen_action *act = &action->screen;

    اگر (میدان_2 == ACTION_میدان_SCREEN) {
        ثابت اظہاریہ_Def *scrn;
        int64_t val = 0;

        اگر (array_ndx)
            واپس ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        اگر (قدر->common.type == STMT_اظہاریہ_NEGATE ||
            قدر->common.type == STMT_اظہاریہ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = قدر->unary.بچہ;
        }
        ورنہ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = قدر;
        }

        اگر (!اظہاریہ_Resolve_Integer(ctx, scrn, &val))
            واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "integer (-128..127)",
                                  keymap_info->strict);

        val = (قدر->common.type == STMT_اظہاریہ_NEGATE ? -val : val);
        اگر (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        واپس PARSER_SUCCESS;
    }
    ورنہ اگر (میدان_2 == ACTION_میدان_SAME) {
        واپس CheckBooleanFlag(ctx, keymap_info->strict, action->type, میدان_2,
                                ACTION_SAME_SCREEN, array_ndx, قدر,
                                &act->flags);
    }

    واپس ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساکن شمار xkb_parser_error
Handle_سیٹ_تالا_Controls(ثابت ساخت xkb_keymap_info *keymap_info,
                      ثابت ساخت xkb_mod_سیٹ *mods,
                      اتحاد xkb_action *action, شمار action_میدان میدان_2,
                      ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                      اظہاریہ_Def **قدر_ptr)
{
    ساخت xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    ساخت xkb_controls_action *act = &action->ctrls;

    اگر (میدان_2 == ACTION_میدان_CONTROLS) {
        اگر (array_ndx)
            واپس ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        uint32_t نقاب = 0;
        ثابت uint8_t انحراف = keymap_info->features.controls_name_انحراف;
        اگر (!اظہاریہ_Resolve_نقاب(ctx, قدر, &نقاب, ctrl_نقاب_نام + انحراف))
            واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "controls mask", keymap_info->strict);

        act->ctrls = (شمار xkb_action_controls) نقاب;
        واپس PARSER_SUCCESS;
    }
    ورنہ اگر (میدان_2 == ACTION_میدان_AFFECT &&
             action->type == ACTION_TYPE_CTRL_تالا) {
        واپس Check_Affect_میدان(ctx, keymap_info->strict, action->type,
                                array_ndx, قدر, &act->flags);
    }

    واپس ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساکن شمار xkb_parser_error
Handle_Redirect_کلید(ثابت ساخت xkb_keymap_info *keymap_info,
                  ثابت ساخت xkb_mod_سیٹ *mods,
                  اتحاد xkb_action *action, شمار action_میدان میدان_2,
                  ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                  اظہاریہ_Def **قدر_ptr)
{
    ثابت ساخت xkb_keymap * محدود ثابت keymap = &keymap_info->keymap;
    ساخت xkb_context * محدود ثابت ctx = keymap->ctx;
    ساخت xkb_redirect_کلید_action * ثابت act = &action->redirect;

    اگر (میدان_2 == ACTION_میدان_KEYCODE) {
        اگر (array_ndx)
            واپس ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);
        اگر (قدر->common.type == STMT_اظہاریہ_IDENT) {
            ثابت حرف *valStr = xkb_atom_متن(ctx, قدر->ident.ident);
            اگر (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_کلید_auto;
                واپس PARSER_SUCCESS;
            }
        }
        اگر (قدر->common.type != STMT_اظہاریہ_KEYNAME_LITERAL) {
            واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "key name", keymap_info->strict);
        }
        ثابت ساخت xkb_کلید * ثابت کلید =
            Xkb_کلید_By_Name(keymap, قدر->کلید_name.کلید_name, true);
        اگر (کلید == تہی) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    میدان_متن(میدان_2), کلید_Name_متن(ctx, قدر->کلید_name.کلید_name));
            واپس (keymap_info->strict & PARSER_NO_میدان_قدر_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = کلید->keycode;
        واپس PARSER_SUCCESS;
    }

    اگر (میدان_2 == ACTION_میدان_MODIFIERS || میدان_2 == ACTION_میدان_MODS_مقصد_CLEAR) {
        شمار xkb_action_flags flags = 0;
        xkb_mod_نقاب_t m = 0;
        شمار xkb_parser_error r =
            Check_Modifier_میدان(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, قدر, &flags, &m);
        اگر (r != PARSER_SUCCESS)
            واپس r;
        اگر (flags)
            واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        اگر (میدان_2 == ACTION_میدان_MODIFIERS)
            act->mods |= m;
        ورنہ
            act->mods &= ~m;
        واپس PARSER_SUCCESS;
    }

    واپس ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساکن شمار xkb_parser_error
HandleUnsupported(ثابت ساخت xkb_keymap_info *keymap_info,
                        ثابت ساخت xkb_mod_سیٹ *mods,
                        اتحاد xkb_action *action, شمار action_میدان میدان_2,
                        ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                        اظہاریہ_Def **قدر_ptr)

{
    /* Do not check any field */
    واپس PARSER_SUCCESS;
}

ساکن شمار xkb_parser_error
Handle_نجی(ثابت ساخت xkb_keymap_info *keymap_info,
              ثابت ساخت xkb_mod_سیٹ *mods,
              اتحاد xkb_action *action, شمار action_میدان میدان_2,
              ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
              اظہاریہ_Def **قدر_ptr)
{
    ساخت xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    ساخت xkb_نجی_action *act = &action->priv;

    اگر (میدان_2 == ACTION_میدان_TYPE) {
        int64_t type = 0;

        اگر (array_ndx)
            واپس ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        اگر (!اظہاریہ_Resolve_Integer(ctx, قدر, &type))
            واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                                  ACTION_TYPE_نجی, میدان_2, "integer",
                                  keymap_info->strict);

        اگر (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
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
        اگر (type < ACTION_TYPE_نجی) {
            log_info(ctx, XKB_LOG_پیغام_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     Action_Type_متن((شمار xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        ورنہ {
            act->type = (شمار xkb_action_type) type;
        }

        واپس PARSER_SUCCESS;
    }
    ورنہ اگر (میدان_2 == ACTION_میدان_ڈیٹا) {
        اگر (array_ndx == تہی) {
            xkb_atom_t val = XKB_ATOM_NONE;

            اگر (!اظہاریہ_Resolve_String(ctx, قدر, &val))
                واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                                      action->type, میدان_2, "string",
                                      keymap_info->strict);

            ثابت حرف *str = xkb_atom_متن(ctx, val);
            سائز_t len = لڑی_لمبائی(str);
            اگر (len < 1 || len > حجم(act->ڈیٹا)) {
                log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", حجم(act->ڈیٹا), len);
                واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->ڈیٹا, 0, حجم(act->ڈیٹا));
            حافظہ_نقل(act->ڈیٹا, str, len);
            واپس PARSER_SUCCESS;
        }
        ورنہ {
            int64_t ndx = 0, datum = 0;

            اگر (!اظہاریہ_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            اگر (ndx < 0 || (سائز_t) ndx >= حجم(act->ڈیٹا)) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        حجم(act->ڈیٹا), ndx);
                واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            اگر (!اظہاریہ_Resolve_Integer(ctx, قدر, &datum))
                واپس ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, act->type,
                                      میدان_2, "integer", keymap_info->strict);

            اگر (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                واپس (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->ڈیٹا[ndx] = (uint8_t) datum;
            واپس PARSER_SUCCESS;
        }
    }

    واپس ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

قسمتعریف شمار xkb_parser_error
        (*actionHandler)(ثابت ساخت xkb_keymap_info *keymap_info,
                         ثابت ساخت xkb_mod_سیٹ *mods,
                         اتحاد xkb_action *action,
                         شمار action_میدان میدان_2,
                         ثابت اظہاریہ_Def *array_ndx,
                         ثابت اظہاریہ_Def *قدر,
                         اظہاریہ_Def **قدر_ptr);

ساکن ثابت actionHandler handleAction[ACTION_TYPE_عدد_ENTRIES] = {
    [ACTION_TYPE_NONE] = HandleNoAction,
    [ACTION_TYPE_VOID] = HandleNoAction,
    [ACTION_TYPE_MOD_سیٹ] = Handle_سیٹ_Latch_تالا_Mods,
    [ACTION_TYPE_MOD_LATCH] = Handle_سیٹ_Latch_تالا_Mods,
    [ACTION_TYPE_MOD_تالا] = Handle_سیٹ_Latch_تالا_Mods,
    [ACTION_TYPE_گروہ_سیٹ] = Handle_سیٹ_Latch_تالا_گروہ,
    [ACTION_TYPE_گروہ_LATCH] = Handle_سیٹ_Latch_تالا_گروہ,
    [ACTION_TYPE_گروہ_تالا] = Handle_سیٹ_Latch_تالا_گروہ,
    [ACTION_TYPE_PTR_MOVE] = HandleMovePtr,
    [ACTION_TYPE_PTR_BUTTON] = HandlePtrBtn,
    [ACTION_TYPE_PTR_تالا] = HandlePtrBtn,
    [ACTION_TYPE_PTR_DEFAULT] = Handle_سیٹ_Ptr_Dflt,
    [ACTION_TYPE_TERMINATE] = HandleNoAction,
    [ACTION_TYPE_SWITCH_VT] = HandleSwitchScreen,
    [ACTION_TYPE_CTRL_سیٹ] = Handle_سیٹ_تالا_Controls,
    [ACTION_TYPE_CTRL_تالا] = Handle_سیٹ_تالا_Controls,
    [ACTION_TYPE_REDIRECT_کلید] = Handle_Redirect_کلید,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_نجی] = Handle_نجی,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_اندرونی == 20 &&
              ACTION_TYPE_اندرونی + 1 == ACTION_TYPE_عدد_ENTRIES,
              "Missing action type");

/***====================================================================***/

شمار xkb_parser_error
HandleActionDef(ثابت ساخت xkb_keymap_info *keymap_info, ActionsInfo *info,
                ثابت ساخت xkb_mod_سیٹ *mods, اظہاریہ_Def *def,
                اتحاد xkb_action *action)
{
    ساخت xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    اگر (def->common.type != STMT_اظہاریہ_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_مقصد_string(def->common.type));
        واپس PARSER_FATAL_ERROR;
    }

    ثابت حرف *action_name = xkb_atom_متن(ctx, def->action.name);
    شمار xkb_action_type handler_type;
    اگر (!string_مقصد_Action_Type(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        اگر (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            واپس PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    اگر (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    شمار xkb_parser_error ret = PARSER_SUCCESS;
    برائے (اظہاریہ_Def *arg = def->action.دلائل; arg != تہی;
         arg = (اظہاریہ_Def *) arg->common.اگلا) {
        ثابت اظہاریہ_Def *قدر;
        اظہاریہ_Def **قدر_ptr = تہی;
        اظہاریہ_Def *میدان_2, *arrayRtrn;
        ثابت حرف *elemRtrn, *میدان_Rtrn;

        اگر (arg->common.type == STMT_اظہاریہ_تفویض) {
            میدان_2 = arg->ثنائی.بایاں;
            قدر = arg->ثنائی.دایاں;
            قدر_ptr = &arg->ثنائی.دایاں;
        }
        ورنہ اگر (arg->common.type == STMT_اظہاریہ_NOT ||
                 arg->common.type == STMT_اظہاریہ_INVERT) {
            میدان_2 = arg->unary.بچہ;
            قدر = (ثابت اظہاریہ_Def *) &constFalse;
        }
        ورنہ {
            میدان_2 = arg;
            قدر = (ثابت اظہاریہ_Def *) &constTrue;
        }

        اگر (!اظہاریہ_Resolve_Lhs(ctx, میدان_2, &elemRtrn, &میدان_Rtrn, &arrayRtrn))
            واپس PARSER_FATAL_ERROR;

        اگر (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, میدان_Rtrn);
            واپس PARSER_FATAL_ERROR;
        }

        شمار action_میدان میدان_Ndx;
        اگر (!string_مقصد_میدان(میدان_Rtrn, &میدان_Ndx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_میدان,
                    "Unknown field name %s for action %s discarded\n",
                    میدان_Rtrn, Action_Type_متن(action->type));
            اگر (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                واپس PARSER_FATAL_ERROR;
            } ورنہ {
                /* Field is just ignored */
                جاری;
            }
        }

        انتخاب (handleAction[handler_type](keymap_info, mods, action, میدان_Ndx,
                                           arrayRtrn, قدر, قدر_ptr)) {
        حالت PARSER_FATAL_ERROR:
            واپس PARSER_FATAL_ERROR;
        حالت PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            توڑو;
        طےشدہ:
            ;
        }
    }

    واپس (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

شمار xkb_parser_error
سیٹ_Default_Action_میدان(ثابت ساخت xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ساخت xkb_mod_سیٹ *mods,
                      ثابت حرف *elem, ثابت حرف *میدان_2, اظہاریہ_Def *array_ndx,
                      اظہاریہ_Def **قدر_ptr, شمار ملاؤ_منوال ملاؤ)
{
    ثابت اظہاریہ_Def * محدود ثابت قدر = *قدر_ptr;
    شمار xkb_action_type action;
    اگر (!string_مقصد_Action_Type(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        واپس (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    شمار action_میدان action_میدان;
    اگر (!string_مقصد_میدان(میدان_2, &action_میدان)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_میدان,
                "Unknown action field \"%s\"\n", میدان_2);
        واپس (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    اتحاد xkb_action* ثابت into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    اتحاد xkb_action from = *into;

    /* Parse action */
    ثابت شمار xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_میدان, array_ndx, قدر, قدر_ptr
    );

    /* Success is mandatory to update defaults */
    اگر (ret != PARSER_SUCCESS)
        واپس ret;

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
    اگر (!action_equal(into, &from)) {
        ثابت bool replace = (ملاؤ != ملاؤ_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_پیغام_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                میدان_متن(action_میدان), Action_Type_متن(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        اگر (replace)
            *into = from;
    }
    واپس PARSER_SUCCESS;
}
