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

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "context.h"
#ضمن "darray.h"
#ضمن "keymap.h"
#ضمن "messages-codes.h"
#ضمن "xkbcomp-priv.h"
#ضمن "text.h"
#ضمن "expr.h"
#ضمن "xkbcomp/ast.h"
#ضمن "action.h"

ساكن ثابت اظہاریہ_Boolean constTrue = {
    .common = { .type = STMT_اظہاریہ_BOOLEAN_LITERAL, .اگلا = عدم },
    .سیٹ = true,
};

ساكن ثابت اظہاریہ_Boolean constFalse = {
    .common = { .type = STMT_اظہاریہ_BOOLEAN_LITERAL, .اگلا = عدم },
    .سیٹ = false,
};

تعداد action_میدان {
    ACTION_میدان_CLEAR_LOCKS,
    ACTION_میدان_LATCH_منزل_جندرا,
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
    ACTION_میدان_انکڑے,
    ACTION_میدان_DEVICE,
    ACTION_میدان_KEYCODE,
    ACTION_میدان_MODS_منزل_CLEAR,
    ACTION_میدان_جندرا_ON_RELEASE,
    ACTION_میدان_UNLOCK_ON_PRESS,
    ACTION_میدان_LATCH_ON_PRESS,
};

فراغ
InitActionsInfo(ثابت بنية xkb_keymap *keymap, ActionsInfo *info)
{
    لكل (تعداد xkb_action_type type = 0; type < ACTION_TYPE_عدد_ENTRIES; type++)
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

ساكن ثابت Lookup_اندراج میدان_لڑیاں[] = {
    { "clearLocks",       ACTION_میدان_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_میدان_LATCH_منزل_جندرا   },
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
    { "data",             ACTION_میدان_انکڑے            },
    { "device",           ACTION_میدان_DEVICE          },
    { "dev",              ACTION_میدان_DEVICE          },
    { "key",              ACTION_میدان_KEYCODE         },
    { "keycode",          ACTION_میدان_KEYCODE         },
    { "kc",               ACTION_میدان_KEYCODE         },
    { "clearmods",        ACTION_میدان_MODS_منزل_CLEAR   },
    { "clearmodifiers",   ACTION_میدان_MODS_منزل_CLEAR   },
    { "lockOnRelease",    ACTION_میدان_جندرا_ON_RELEASE },
    { "unlockOnPress",    ACTION_میدان_UNLOCK_ON_PRESS },
    { "latchOnPress",     ACTION_میدان_LATCH_ON_PRESS  },
    { عدم,               0                            }
};

ساكن bool
string_منزل_Action_Type(ثابت حرف *str, تعداد xkb_action_type *type_rtrn)
{
    بنانشانی صحيح type = 0;
    ثابت bool ret = LookupString(action_Type_ناں, str, &type);
    *type_rtrn = (تعداد xkb_action_type) type;
    أرجع ret;
}

ساكن bool
string_منزل_میدان(ثابت حرف *str, تعداد action_میدان *میدان_rtrn)
{
    بنانشانی صحيح میدان_2 = 0;
    ثابت bool ret = LookupString(میدان_لڑیاں, str, &میدان_2);
    *میدان_rtrn = (تعداد action_میدان) میدان_2;
    أرجع ret;
}

ساكن ثابت حرف *
میدان_متن(تعداد action_میدان میدان_2)
{
    أرجع Lookup_قدر(میدان_لڑیاں, میدان_2);
}

/***====================================================================***/

ساكن مضمن تعداد xkb_parser_error
ReportMismatch(بنية xkb_context *ctx, تعداد xkb_سنیہا_کوڈ کوڈ,
               تعداد xkb_action_type action, تعداد action_میدان میدان_2,
               ثابت حرف *type, تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, کوڈ,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            میدان_متن(میدان_2), type, Action_Type_متن(action));
    أرجع (strict & PARSER_NO_میدان_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن مضمن تعداد xkb_parser_error
Report_Format_نسخہ_Mismatch(بنية xkb_context *ctx,
                            تعداد xkb_action_type action,
                            تعداد action_میدان میدان_2,
                            تعداد xkb_keymap_format format,
                            ثابت حرف *versions,
                            تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            میدان_متن(میدان_2), Action_Type_متن(action), versions, format);
    أرجع (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن مضمن تعداد xkb_parser_error
ReportIllegal(بنية xkb_context *ctx, تعداد xkb_action_type action,
              تعداد action_میدان میدان_2, تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_میدان,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            میدان_متن(میدان_2), Action_Type_متن(action));
    أرجع (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن مضمن تعداد xkb_parser_error
ReportActionNotArray(بنية xkb_context *ctx, تعداد xkb_action_type action,
                     تعداد action_میدان میدان_2,
                     تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            میدان_متن(میدان_2), Action_Type_متن(action));
    أرجع (strict & PARSER_NO_میدان_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن تعداد xkb_parser_error
HandleNoAction(ثابت بنية xkb_keymap_info *keymap_info,
               ثابت بنية xkb_mod_سیٹ *mods,
               اتحاد xkb_action *action, تعداد action_میدان میدان_2,
               ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
               اظہاریہ_Def **قدر_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_میدان,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            Action_Type_متن(action->type), میدان_متن(میدان_2));
    أرجع (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
CheckBooleanFlag(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, تعداد action_میدان میدان_2,
                 تعداد xkb_action_flags flag, ثابت اظہاریہ_Def *array_ndx,
                 ثابت اظہاریہ_Def *قدر, تعداد xkb_action_flags *flags_inout)
{
    bool سیٹ = false;

    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, میدان_2, strict);

    إذا (!اظہاریہ_Resolve_Boolean(ctx, قدر, &سیٹ))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                              action, میدان_2, "boolean", strict);

    إذا (سیٹ)
        *flags_inout |= flag;
    وإلا
        *flags_inout &= ~flag;

    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Check_Modifier_میدان(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                   ثابت بنية xkb_mod_سیٹ *mods, تعداد xkb_action_type action,
                   ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                   تعداد xkb_action_flags *flags_inout, xkb_mod_نقاب_t *mods_rtrn)
{
    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ACTION_میدان_MODIFIERS, strict);

    إذا (قدر->common.type == STMT_اظہاریہ_IDENT) {
        ثابت حرف *valStr;
        valStr = xkb_atom_متن(ctx, قدر->ident.ident);
        إذا (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            أرجع PARSER_SUCCESS;
        }
    }

    إذا (!اظہاریہ_Resolve_Mod_نقاب(ctx, قدر, MOD_BOTH, mods, mods_rtrn))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action,
                              ACTION_میدان_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    أرجع PARSER_SUCCESS;
}

ساكن ثابت Lookup_اندراج جندرا_Which[] = {
    { "both", 0 },
    { "lock", ACTION_جندرا_NO_UNLOCK },
    { "neither", (ACTION_جندرا_NO_جندرا | ACTION_جندرا_NO_UNLOCK) },
    { "unlock", ACTION_جندرا_NO_جندرا },
    { عدم, 0 }
};

ساكن تعداد xkb_parser_error
Check_Affect_میدان(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, ثابت اظہاریہ_Def *array_ndx,
                 ثابت اظہاریہ_Def *قدر, تعداد xkb_action_flags *flags_inout)
{
    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ACTION_میدان_AFFECT, strict);

    uint32_t flags = 0;
    إذا (!اظہاریہ_Resolve_Enum(ctx, قدر, &flags, جندرا_Which))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                              action, ACTION_میدان_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_جندرا_NO_جندرا | ACTION_جندرا_NO_UNLOCK);
    *flags_inout |= (تعداد xkb_action_flags) flags;
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_سیٹ_Latch_جندرا_Mods(ثابت بنية xkb_keymap_info *keymap_info,
                       ثابت بنية xkb_mod_سیٹ *mods,
                       اتحاد xkb_action *action, تعداد action_میدان میدان_2,
                       ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                       اظہاریہ_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_mod_action *act = &action->mods;
    ثابت تعداد xkb_action_type type = action->type;

    إذا (میدان_2 == ACTION_میدان_MODIFIERS)
        أرجع Check_Modifier_میدان(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, قدر, &act->flags, &act->mods.mods);
    إذا (میدان_2 == ACTION_میدان_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        أكد(type == ACTION_TYPE_MOD_سیٹ || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_جندرا);
        إذا (keymap_info->features.mods_unlock_on_press) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, میدان_2,
                ACTION_UNLOCK_ON_PRESS, array_ndx, قدر, &act->flags
            );
            إذا (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            أرجع ret;
        } وإلا {
            أرجع Report_Format_نسخہ_Mismatch(ctx, action->type, میدان_2,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    إذا ((type == ACTION_TYPE_MOD_سیٹ || type == ACTION_TYPE_MOD_LATCH) &&
        میدان_2 == ACTION_میدان_CLEAR_LOCKS)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, میدان_2,
                                ACTION_جندرا_CLEAR, array_ndx, قدر,
                                &act->flags);
    إذا (type == ACTION_TYPE_MOD_LATCH) {
        إذا (میدان_2 == ACTION_میدان_LATCH_منزل_جندرا)
            أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    میدان_2, ACTION_LATCH_منزل_جندرا, array_ndx,
                                    قدر, &act->flags);
        إذا (میدان_2 == ACTION_میدان_LATCH_ON_PRESS) {
            إذا (keymap_info->features.mods_latch_on_press) {
                أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        میدان_2, ACTION_LATCH_ON_PRESS, array_ndx,
                                        قدر, &act->flags);
            } وإلا {
                أرجع Report_Format_نسخہ_Mismatch(ctx, action->type, میدان_2,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    إذا (type == ACTION_TYPE_MOD_جندرا && میدان_2 == ACTION_میدان_AFFECT)
        أرجع Check_Affect_میدان(ctx, keymap_info->strict, action->type,
                                array_ndx, قدر, &act->flags);

    أرجع ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Check_گروہ_میدان(ثابت بنية xkb_keymap_info *keymap_info,
                تعداد xkb_action_type action, ثابت اظہاریہ_Def *array_ndx,
                ثابت اظہاریہ_Def *قدر, اظہاریہ_Def **قدر_ptr,
                تعداد xkb_action_flags *flags_inout, int32_t *گروہ_rtrn)
{
    ثابت اظہاریہ_Def *spec;
    xkb_layout_اشاریہ_t idx = 0;
    تعداد xkb_action_flags flags = *flags_inout;

    إذا (array_ndx)
        أرجع ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_میدان_گروہ, keymap_info->strict);

    إذا (قدر->common.type == STMT_اظہاریہ_NEGATE ||
        قدر->common.type == STMT_اظہاریہ_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = قدر->unary.بچہ;
        قدر_ptr = &(*قدر_ptr)->unary.بچہ;
    } وإلا {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = قدر;
    }

    ثابت bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ثابت تعداد xkb_parser_error ret =
        اظہاریہ_Resolve_گروہ(keymap_info, spec, absolute, &idx, &pending);
    إذا (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_اشاریہ_2, action,
                       ACTION_میدان_گروہ, "integer", keymap_info->strict);
        أرجع ret;
    }

    إذا (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ثابت darray_ناپ_t pending_اشاریہ =
            darray_ناپ(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (بنية pending_computation) {
                .اظہاریہ = *قدر_ptr,
                .computed = false,
                .قدر = 0,
            }
        );
        *قدر_ptr = عدم;
        static_assert(حجم(pending_اشاریہ) == حجم(*گروہ_rtrn),
                      "Cannot save pending computation");
        *گروہ_rtrn = (int32_t) pending_اشاریہ;
    } وإلا {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        إذا (absolute) {
            *گروہ_rtrn = (int32_t) (idx - 1);
        } وإلا {
            *گروہ_rtrn = (int32_t) idx;
            إذا (قدر->common.type == STMT_اظہاریہ_NEGATE)
                *گروہ_rtrn = -*گروہ_rtrn;
        }
    }

    *flags_inout = flags;
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_سیٹ_Latch_جندرا_گروہ(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_سیٹ *mods,
                        اتحاد xkb_action *action, تعداد action_میدان میدان_2,
                        ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                        اظہاریہ_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_گروہ_action *act = &action->گروہ;
    ثابت تعداد xkb_action_type type = action->type;

    إذا (میدان_2 == ACTION_میدان_گروہ) {
        أرجع Check_گروہ_میدان(keymap_info, action->type, array_ndx, قدر,
                               قدر_ptr, &act->flags, &act->گروہ);
    }
    إذا ((type == ACTION_TYPE_گروہ_سیٹ || type == ACTION_TYPE_گروہ_LATCH) &&
        میدان_2 == ACTION_میدان_CLEAR_LOCKS)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, میدان_2,
                                ACTION_جندرا_CLEAR, array_ndx, قدر,
                                &act->flags);
    إذا (type == ACTION_TYPE_گروہ_LATCH &&
        میدان_2 == ACTION_میدان_LATCH_منزل_جندرا)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, میدان_2,
                                ACTION_LATCH_منزل_جندرا, array_ndx, قدر,
                                &act->flags);
    إذا (type == ACTION_TYPE_گروہ_جندرا &&
        میدان_2 == ACTION_میدان_جندرا_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        إذا (keymap_info->features.گروہ_جندرا_on_release) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, میدان_2,
                ACTION_جندرا_ON_RELEASE, array_ndx, قدر, &act->flags
            );
            إذا (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_جندرا_ON_RELEASE;
            أرجع ret;
        } وإلا {
            أرجع Report_Format_نسخہ_Mismatch(ctx, action->type, میدان_2,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    أرجع ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleMovePtr(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_سیٹ *mods,
              اتحاد xkb_action *action, تعداد action_میدان میدان_2,
              ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
              اظہاریہ_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشارہ_گر_motion_action *act = &action->ptr;

    إذا (میدان_2 == ACTION_میدان_X || میدان_2 == ACTION_میدان_Y) {
        int64_t val = 0;
        ثابت bool absolute = (قدر->common.type != STMT_اظہاریہ_NEGATE &&
                               قدر->common.type != STMT_اظہاریہ_UNARY_PLUS);

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        إذا (!اظہاریہ_Resolve_Integer(ctx, قدر, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "integer", keymap_info->strict);

        إذا (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    میدان_متن(میدان_2), Action_Type_متن(action->type),
                    INT16_MIN, INT16_MAX, val);
            أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        إذا (میدان_2 == ACTION_میدان_X) {
            إذا (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        وإلا {
            إذا (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (میدان_2 == ACTION_میدان_REPEAT) {
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, میدان_2,
                                ACTION_REPEAT, array_ndx, قدر, &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandlePtrBtn(ثابت بنية xkb_keymap_info *keymap_info,
             ثابت بنية xkb_mod_سیٹ *mods,
             اتحاد xkb_action *action, تعداد action_میدان میدان_2,
             ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
             اظہاریہ_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشارہ_گر_button_action *act = &action->btn;

    إذا (میدان_2 == ACTION_میدان_BUTTON) {
        int64_t btn = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        إذا (!اظہاریہ_Resolve_Button(ctx, قدر, &btn)) {
            static_assert(XKB_اشارہ_گر_BUTTON_MIN == 1 &&
                          XKB_اشارہ_گر_BUTTON_MAX == 5, "invalid message");
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_اشارہ_گر_BUTTON_DEFAULT < XKB_اشارہ_گر_BUTTON_MIN, "");
        إذا (btn < XKB_اشارہ_گر_BUTTON_DEFAULT || btn > XKB_اشارہ_گر_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_اشارہ_گر_BUTTON_MIN, XKB_اشارہ_گر_BUTTON_MAX, btn);
            أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (action->type == ACTION_TYPE_PTR_جندرا &&
             میدان_2 == ACTION_میدان_AFFECT) {
        أرجع Check_Affect_میدان(ctx, keymap_info->strict, action->type,
                                array_ndx, قدر, &act->flags);
    }
    وإلا إذا (میدان_2 == ACTION_میدان_گنتی) {
        int64_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        إذا (!اظہاریہ_Resolve_Integer(ctx, قدر, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "integer", keymap_info->strict);

        إذا (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        إذا (action->type != ACTION_TYPE_PTR_جندرا)
            act->گنتی = (uint8_t) val;
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

تعداد {
    سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_عدد_ENTRIES
};

ساكن ثابت Lookup_اندراج ptrDflts[] = {
    { "dfltbtn", سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { عدم, 0 }
};

ساكن تعداد xkb_parser_error
Handle_سیٹ_Ptr_Dflt(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت بنية xkb_mod_سیٹ *mods,
                 اتحاد xkb_action *action, تعداد action_میدان میدان_2,
                 ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                 اظہاریہ_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشارہ_گر_default_action *act = &action->dflt;

    إذا (میدان_2 == ACTION_میدان_AFFECT) {
        uint32_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        إذا (!اظہاریہ_Resolve_Enum(ctx, قدر, &val, ptrDflts))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "pointer component", keymap_info->strict);
        static_assert(سیٹ_اشارہ_گر_DEFAULT_BUTTON_AFFECT_عدد_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (میدان_2 == ACTION_میدان_BUTTON || میدان_2 == ACTION_میدان_قدر) {
        ثابت اظہاریہ_Def *button;
        int64_t btn = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        إذا (قدر->common.type == STMT_اظہاریہ_NEGATE ||
            قدر->common.type == STMT_اظہاریہ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = قدر->unary.بچہ;
        }
        وإلا {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = قدر;
        }

        إذا (!اظہاریہ_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_اشارہ_گر_BUTTON_MIN == 1 &&
                          XKB_اشارہ_گر_BUTTON_MAX == 5, "invalid message");
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_اشارہ_گر_BUTTON_DEFAULT < XKB_اشارہ_گر_BUTTON_MIN, "");
        إذا (btn < XKB_اشارہ_گر_BUTTON_DEFAULT || btn > XKB_اشارہ_گر_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_اشارہ_گر_BUTTON_MIN, XKB_اشارہ_گر_BUTTON_MAX, btn);
            أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        إذا (btn == XKB_اشارہ_گر_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->قدر = (int8_t) (قدر->common.type == STMT_اظہاریہ_NEGATE ? -btn: btn);
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleSwitchScreen(ثابت بنية xkb_keymap_info *keymap_info,
                   ثابت بنية xkb_mod_سیٹ *mods,
                   اتحاد xkb_action *action, تعداد action_میدان میدان_2,
                   ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                   اظہاریہ_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_switch_screen_action *act = &action->screen;

    إذا (میدان_2 == ACTION_میدان_SCREEN) {
        ثابت اظہاریہ_Def *scrn;
        int64_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        إذا (قدر->common.type == STMT_اظہاریہ_NEGATE ||
            قدر->common.type == STMT_اظہاریہ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = قدر->unary.بچہ;
        }
        وإلا {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = قدر;
        }

        إذا (!اظہاریہ_Resolve_Integer(ctx, scrn, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "integer (-128..127)",
                                  keymap_info->strict);

        val = (قدر->common.type == STMT_اظہاریہ_NEGATE ? -val : val);
        إذا (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (میدان_2 == ACTION_میدان_SAME) {
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, میدان_2,
                                ACTION_SAME_SCREEN, array_ndx, قدر,
                                &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Handle_سیٹ_جندرا_Controls(ثابت بنية xkb_keymap_info *keymap_info,
                      ثابت بنية xkb_mod_سیٹ *mods,
                      اتحاد xkb_action *action, تعداد action_میدان میدان_2,
                      ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                      اظہاریہ_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_controls_action *act = &action->ctrls;

    إذا (میدان_2 == ACTION_میدان_CONTROLS) {
        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        uint32_t نقاب = 0;
        ثابت uint8_t وتھ = keymap_info->features.controls_name_وتھ;
        إذا (!اظہاریہ_Resolve_نقاب(ctx, قدر, &نقاب, ctrl_نقاب_ناں + وتھ))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "controls mask", keymap_info->strict);

        act->ctrls = (تعداد xkb_action_controls) نقاب;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (میدان_2 == ACTION_میدان_AFFECT &&
             action->type == ACTION_TYPE_CTRL_جندرا) {
        أرجع Check_Affect_میدان(ctx, keymap_info->strict, action->type,
                                array_ndx, قدر, &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Handle_Redirect_کلید(ثابت بنية xkb_keymap_info *keymap_info,
                  ثابت بنية xkb_mod_سیٹ *mods,
                  اتحاد xkb_action *action, تعداد action_میدان میدان_2,
                  ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                  اظہاریہ_Def **قدر_ptr)
{
    ثابت بنية xkb_keymap * محدود ثابت keymap = &keymap_info->keymap;
    بنية xkb_context * محدود ثابت ctx = keymap->ctx;
    بنية xkb_redirect_کلید_action * ثابت act = &action->redirect;

    إذا (میدان_2 == ACTION_میدان_KEYCODE) {
        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);
        إذا (قدر->common.type == STMT_اظہاریہ_IDENT) {
            ثابت حرف *valStr = xkb_atom_متن(ctx, قدر->ident.ident);
            إذا (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_کلید_auto;
                أرجع PARSER_SUCCESS;
            }
        }
        إذا (قدر->common.type != STMT_اظہاریہ_KEYNAME_LITERAL) {
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "key name", keymap_info->strict);
        }
        ثابت بنية xkb_کلید * ثابت کلید =
            Xkb_کلید_By_Name(keymap, قدر->کلید_name.کلید_name, true);
        إذا (کلید == عدم) {
            log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    میدان_متن(میدان_2), کلید_Name_متن(ctx, قدر->کلید_name.کلید_name));
            أرجع (keymap_info->strict & PARSER_NO_میدان_قدر_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = کلید->keycode;
        أرجع PARSER_SUCCESS;
    }

    إذا (میدان_2 == ACTION_میدان_MODIFIERS || میدان_2 == ACTION_میدان_MODS_منزل_CLEAR) {
        تعداد xkb_action_flags flags = 0;
        xkb_mod_نقاب_t m = 0;
        تعداد xkb_parser_error r =
            Check_Modifier_میدان(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, قدر, &flags, &m);
        إذا (r != PARSER_SUCCESS)
            أرجع r;
        إذا (flags)
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, action->type,
                                  میدان_2, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        إذا (میدان_2 == ACTION_میدان_MODIFIERS)
            act->mods |= m;
        وإلا
            act->mods &= ~m;
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleUnsupported(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_سیٹ *mods,
                        اتحاد xkb_action *action, تعداد action_میدان میدان_2,
                        ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
                        اظہاریہ_Def **قدر_ptr)

{
    /* Do not check any field */
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_نجی(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_سیٹ *mods,
              اتحاد xkb_action *action, تعداد action_میدان میدان_2,
              ثابت اظہاریہ_Def *array_ndx, ثابت اظہاریہ_Def *قدر,
              اظہاریہ_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_نجی_action *act = &action->priv;

    إذا (میدان_2 == ACTION_میدان_TYPE) {
        int64_t type = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, میدان_2,
                                        keymap_info->strict);

        إذا (!اظہاریہ_Resolve_Integer(ctx, قدر, &type))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                                  ACTION_TYPE_نجی, میدان_2, "integer",
                                  keymap_info->strict);

        إذا (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
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
        إذا (type < ACTION_TYPE_نجی) {
            log_info(ctx, XKB_LOG_سنیہا_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     Action_Type_متن((تعداد xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        وإلا {
            act->type = (تعداد xkb_action_type) type;
        }

        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (میدان_2 == ACTION_میدان_انکڑے) {
        إذا (array_ndx == عدم) {
            xkb_atom_t val = XKB_ATOM_NONE;

            إذا (!اظہاریہ_Resolve_String(ctx, قدر, &val))
                أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                                      action->type, میدان_2, "string",
                                      keymap_info->strict);

            ثابت حرف *str = xkb_atom_متن(ctx, val);
            ناپ_t len = لڑی_لمبائی(str);
            إذا (len < 1 || len > حجم(act->انکڑے)) {
                log_warn(ctx, XKB_LOG_سنیہا_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", حجم(act->انکڑے), len);
                أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->انکڑے, 0, حجم(act->انکڑے));
            حافظہ_نقل(act->انکڑے, str, len);
            أرجع PARSER_SUCCESS;
        }
        وإلا {
            int64_t ndx = 0, datum = 0;

            إذا (!اظہاریہ_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            إذا (ndx < 0 || (ناپ_t) ndx >= حجم(act->انکڑے)) {
                log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        حجم(act->انکڑے), ndx);
                أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            إذا (!اظہاریہ_Resolve_Integer(ctx, قدر, &datum))
                أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_میدان_TYPE, act->type,
                                      میدان_2, "integer", keymap_info->strict);

            إذا (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_سنیہا_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                أرجع (keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->انکڑے[ndx] = (uint8_t) datum;
            أرجع PARSER_SUCCESS;
        }
    }

    أرجع ReportIllegal(ctx, action->type, میدان_2, keymap_info->strict);
}

تعريفنوع تعداد xkb_parser_error
        (*actionHandler)(ثابت بنية xkb_keymap_info *keymap_info,
                         ثابت بنية xkb_mod_سیٹ *mods,
                         اتحاد xkb_action *action,
                         تعداد action_میدان میدان_2,
                         ثابت اظہاریہ_Def *array_ndx,
                         ثابت اظہاریہ_Def *قدر,
                         اظہاریہ_Def **قدر_ptr);

ساكن ثابت actionHandler handleAction[ACTION_TYPE_عدد_ENTRIES] = {
    [ACTION_TYPE_NONE] = HandleNoAction,
    [ACTION_TYPE_VOID] = HandleNoAction,
    [ACTION_TYPE_MOD_سیٹ] = Handle_سیٹ_Latch_جندرا_Mods,
    [ACTION_TYPE_MOD_LATCH] = Handle_سیٹ_Latch_جندرا_Mods,
    [ACTION_TYPE_MOD_جندرا] = Handle_سیٹ_Latch_جندرا_Mods,
    [ACTION_TYPE_گروہ_سیٹ] = Handle_سیٹ_Latch_جندرا_گروہ,
    [ACTION_TYPE_گروہ_LATCH] = Handle_سیٹ_Latch_جندرا_گروہ,
    [ACTION_TYPE_گروہ_جندرا] = Handle_سیٹ_Latch_جندرا_گروہ,
    [ACTION_TYPE_PTR_MOVE] = HandleMovePtr,
    [ACTION_TYPE_PTR_BUTTON] = HandlePtrBtn,
    [ACTION_TYPE_PTR_جندرا] = HandlePtrBtn,
    [ACTION_TYPE_PTR_DEFAULT] = Handle_سیٹ_Ptr_Dflt,
    [ACTION_TYPE_TERMINATE] = HandleNoAction,
    [ACTION_TYPE_SWITCH_VT] = HandleSwitchScreen,
    [ACTION_TYPE_CTRL_سیٹ] = Handle_سیٹ_جندرا_Controls,
    [ACTION_TYPE_CTRL_جندرا] = Handle_سیٹ_جندرا_Controls,
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

تعداد xkb_parser_error
HandleActionDef(ثابت بنية xkb_keymap_info *keymap_info, ActionsInfo *info,
                ثابت بنية xkb_mod_سیٹ *mods, اظہاریہ_Def *def,
                اتحاد xkb_action *action)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    إذا (def->common.type != STMT_اظہاریہ_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_میدان_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_منزل_string(def->common.type));
        أرجع PARSER_FATAL_ERROR;
    }

    ثابت حرف *action_name = xkb_atom_متن(ctx, def->action.name);
    تعداد xkb_action_type handler_type;
    إذا (!string_منزل_Action_Type(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        إذا (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            أرجع PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    إذا (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    تعداد xkb_parser_error ret = PARSER_SUCCESS;
    لكل (اظہاریہ_Def *arg = def->action.دلیلاں; arg != عدم;
         arg = (اظہاریہ_Def *) arg->common.اگلا) {
        ثابت اظہاریہ_Def *قدر;
        اظہاریہ_Def **قدر_ptr = عدم;
        اظہاریہ_Def *میدان_2, *arrayRtrn;
        ثابت حرف *elemRtrn, *میدان_Rtrn;

        إذا (arg->common.type == STMT_اظہاریہ_سونپو) {
            میدان_2 = arg->ثنائی.بایاں;
            قدر = arg->ثنائی.دایاں;
            قدر_ptr = &arg->ثنائی.دایاں;
        }
        وإلا إذا (arg->common.type == STMT_اظہاریہ_NOT ||
                 arg->common.type == STMT_اظہاریہ_INVERT) {
            میدان_2 = arg->unary.بچہ;
            قدر = (ثابت اظہاریہ_Def *) &constFalse;
        }
        وإلا {
            میدان_2 = arg;
            قدر = (ثابت اظہاریہ_Def *) &constTrue;
        }

        إذا (!اظہاریہ_Resolve_Lhs(ctx, میدان_2, &elemRtrn, &میدان_Rtrn, &arrayRtrn))
            أرجع PARSER_FATAL_ERROR;

        إذا (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, میدان_Rtrn);
            أرجع PARSER_FATAL_ERROR;
        }

        تعداد action_میدان میدان_Ndx;
        إذا (!string_منزل_میدان(میدان_Rtrn, &میدان_Ndx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_میدان,
                    "Unknown field name %s for action %s discarded\n",
                    میدان_Rtrn, Action_Type_متن(action->type));
            إذا (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                أرجع PARSER_FATAL_ERROR;
            } وإلا {
                /* Field is just ignored */
                استمر;
            }
        }

        بدل (handleAction[handler_type](keymap_info, mods, action, میدان_Ndx,
                                           arrayRtrn, قدر, قدر_ptr)) {
        حالة PARSER_FATAL_ERROR:
            أرجع PARSER_FATAL_ERROR;
        حالة PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            اكسر;
        افتراضي:
            ;
        }
    }

    أرجع (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

تعداد xkb_parser_error
سیٹ_Default_Action_میدان(ثابت بنية xkb_keymap_info *keymap_info,
                      ActionsInfo *info, بنية xkb_mod_سیٹ *mods,
                      ثابت حرف *elem, ثابت حرف *میدان_2, اظہاریہ_Def *array_ndx,
                      اظہاریہ_Def **قدر_ptr, تعداد ملاؤ_منوال ملاؤ)
{
    ثابت اظہاریہ_Def * محدود ثابت قدر = *قدر_ptr;
    تعداد xkb_action_type action;
    إذا (!string_منزل_Action_Type(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        أرجع (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    تعداد action_میدان action_میدان;
    إذا (!string_منزل_میدان(میدان_2, &action_میدان)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_میدان,
                "Unknown action field \"%s\"\n", میدان_2);
        أرجع (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    اتحاد xkb_action* ثابت into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    اتحاد xkb_action from = *into;

    /* Parse action */
    ثابت تعداد xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_میدان, array_ndx, قدر, قدر_ptr
    );

    /* Success is mandatory to update defaults */
    إذا (ret != PARSER_SUCCESS)
        أرجع ret;

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
    إذا (!action_equal(into, &from)) {
        ثابت bool replace = (ملاؤ != ملاؤ_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_سنیہا_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                میدان_متن(action_میدان), Action_Type_متن(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        إذا (replace)
            *into = from;
    }
    أرجع PARSER_SUCCESS;
}
