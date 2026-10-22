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

ساكن ثابت عبارت_Boolean constTrue = {
    .common = { .type = STMT_عبارت_BOOLEAN_LITERAL, .بعدی = عدم },
    .مجموعه = true,
};

ساكن ثابت عبارت_Boolean constFalse = {
    .common = { .type = STMT_عبارت_BOOLEAN_LITERAL, .بعدی = عدم },
    .مجموعه = false,
};

تعداد action_فیلد {
    ACTION_فیلد_CLEAR_LOCKS,
    ACTION_فیلد_LATCH_مقصد_قفل,
    ACTION_فیلد_GEN_کلید_EVENT,
    ACTION_فیلد_REPORT,
    ACTION_فیلد_DEFAULT,
    ACTION_فیلد_AFFECT,
    ACTION_فیلد_INCREMENT,
    ACTION_فیلد_MODIFIERS,
    ACTION_فیلد_گروه,
    ACTION_فیلد_X,
    ACTION_فیلد_Y,
    ACTION_فیلد_REPEAT,
    ACTION_فیلد_BUTTON,
    ACTION_فیلد_مقدار,
    ACTION_فیلد_CONTROLS,
    ACTION_فیلد_TYPE,
    ACTION_فیلد_شمار,
    ACTION_فیلد_SCREEN,
    ACTION_فیلد_SAME,
    ACTION_فیلد_داده,
    ACTION_فیلد_DEVICE,
    ACTION_فیلد_KEYCODE,
    ACTION_فیلد_MODS_مقصد_CLEAR,
    ACTION_فیلد_قفل_ON_RELEASE,
    ACTION_فیلد_UNLOCK_ON_PRESS,
    ACTION_فیلد_LATCH_ON_PRESS,
};

فراغ
InitActionsInfo(ثابت بنية xkb_keymap *keymap, ActionsInfo *info)
{
    لكل (تعداد xkb_action_type type = 0; type < ACTION_TYPE_رقم_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.مقدار = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_کلید].redirect.keycode =
        keymap->redirect_کلید_auto;
}

ساكن ثابت Lookup_مدخل فیلد_رشتهها[] = {
    { "clearLocks",       ACTION_فیلد_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_فیلد_LATCH_مقصد_قفل   },
    { "genKeyEvent",      ACTION_فیلد_GEN_کلید_EVENT   },
    { "generateKeyEvent", ACTION_فیلد_GEN_کلید_EVENT   },
    { "report",           ACTION_فیلد_REPORT          },
    { "default",          ACTION_فیلد_DEFAULT         },
    { "affect",           ACTION_فیلد_AFFECT          },
    { "increment",        ACTION_فیلد_INCREMENT       },
    { "modifiers",        ACTION_فیلد_MODIFIERS       },
    { "mods",             ACTION_فیلد_MODIFIERS       },
    { "group",            ACTION_فیلد_گروه           },
    { "x",                ACTION_فیلد_X               },
    { "y",                ACTION_فیلد_Y               },
    { "repeat",           ACTION_فیلد_REPEAT          },
    { "accel",            ACTION_فیلد_REPEAT          },
    { "accelerate",       ACTION_فیلد_REPEAT          },
    { "button",           ACTION_فیلد_BUTTON          },
    { "value",            ACTION_فیلد_مقدار           },
    { "controls",         ACTION_فیلد_CONTROLS        },
    { "ctrls",            ACTION_فیلد_CONTROLS        },
    { "type",             ACTION_فیلد_TYPE            },
    { "count",            ACTION_فیلد_شمار           },
    { "screen",           ACTION_فیلد_SCREEN          },
    { "same",             ACTION_فیلد_SAME            },
    { "sameServer",       ACTION_فیلد_SAME            },
    { "data",             ACTION_فیلد_داده            },
    { "device",           ACTION_فیلد_DEVICE          },
    { "dev",              ACTION_فیلد_DEVICE          },
    { "key",              ACTION_فیلد_KEYCODE         },
    { "keycode",          ACTION_فیلد_KEYCODE         },
    { "kc",               ACTION_فیلد_KEYCODE         },
    { "clearmods",        ACTION_فیلد_MODS_مقصد_CLEAR   },
    { "clearmodifiers",   ACTION_فیلد_MODS_مقصد_CLEAR   },
    { "lockOnRelease",    ACTION_فیلد_قفل_ON_RELEASE },
    { "unlockOnPress",    ACTION_فیلد_UNLOCK_ON_PRESS },
    { "latchOnPress",     ACTION_فیلد_LATCH_ON_PRESS  },
    { عدم,               0                            }
};

ساكن bool
string_مقصد_Action_Type(ثابت حرف *str, تعداد xkb_action_type *type_rtrn)
{
    بیعلامت صحيح type = 0;
    ثابت bool ret = LookupString(action_Type_نامها, str, &type);
    *type_rtrn = (تعداد xkb_action_type) type;
    أرجع ret;
}

ساكن bool
string_مقصد_فیلد(ثابت حرف *str, تعداد action_فیلد *فیلد_rtrn)
{
    بیعلامت صحيح فیلد_2 = 0;
    ثابت bool ret = LookupString(فیلد_رشتهها, str, &فیلد_2);
    *فیلد_rtrn = (تعداد action_فیلد) فیلد_2;
    أرجع ret;
}

ساكن ثابت حرف *
فیلد_متن(تعداد action_فیلد فیلد_2)
{
    أرجع Lookup_مقدار(فیلد_رشتهها, فیلد_2);
}

/***====================================================================***/

ساكن مضمن تعداد xkb_parser_error
ReportMismatch(بنية xkb_context *ctx, تعداد xkb_پیام_کد کد,
               تعداد xkb_action_type action, تعداد action_فیلد فیلد_2,
               ثابت حرف *type, تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, کد,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            فیلد_متن(فیلد_2), type, Action_Type_متن(action));
    أرجع (strict & PARSER_NO_فیلد_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن مضمن تعداد xkb_parser_error
Report_Format_نسخه_Mismatch(بنية xkb_context *ctx,
                            تعداد xkb_action_type action,
                            تعداد action_فیلد فیلد_2,
                            تعداد xkb_keymap_format format,
                            ثابت حرف *versions,
                            تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            فیلد_متن(فیلد_2), Action_Type_متن(action), versions, format);
    أرجع (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن مضمن تعداد xkb_parser_error
ReportIllegal(بنية xkb_context *ctx, تعداد xkb_action_type action,
              تعداد action_فیلد فیلد_2, تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_فیلد,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            فیلد_متن(فیلد_2), Action_Type_متن(action));
    أرجع (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن مضمن تعداد xkb_parser_error
ReportActionNotArray(بنية xkb_context *ctx, تعداد xkb_action_type action,
                     تعداد action_فیلد فیلد_2,
                     تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            فیلد_متن(فیلد_2), Action_Type_متن(action));
    أرجع (strict & PARSER_NO_فیلد_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن تعداد xkb_parser_error
HandleNoAction(ثابت بنية xkb_keymap_info *keymap_info,
               ثابت بنية xkb_mod_مجموعه *mods,
               اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
               ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
               عبارت_Def **مقدار_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_فیلد,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            Action_Type_متن(action->type), فیلد_متن(فیلد_2));
    أرجع (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
CheckBooleanFlag(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, تعداد action_فیلد فیلد_2,
                 تعداد xkb_action_flags flag, ثابت عبارت_Def *array_ndx,
                 ثابت عبارت_Def *مقدار, تعداد xkb_action_flags *flags_inout)
{
    bool مجموعه = false;

    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, فیلد_2, strict);

    إذا (!عبارت_Resolve_Boolean(ctx, مقدار, &مجموعه))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                              action, فیلد_2, "boolean", strict);

    إذا (مجموعه)
        *flags_inout |= flag;
    وإلا
        *flags_inout &= ~flag;

    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Check_Modifier_فیلد(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                   ثابت بنية xkb_mod_مجموعه *mods, تعداد xkb_action_type action,
                   ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
                   تعداد xkb_action_flags *flags_inout, xkb_mod_ماسک_t *mods_rtrn)
{
    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ACTION_فیلد_MODIFIERS, strict);

    إذا (مقدار->common.type == STMT_عبارت_IDENT) {
        ثابت حرف *valStr;
        valStr = xkb_atom_متن(ctx, مقدار->ident.ident);
        إذا (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            أرجع PARSER_SUCCESS;
        }
    }

    إذا (!عبارت_Resolve_Mod_ماسک(ctx, مقدار, MOD_BOTH, mods, mods_rtrn))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, action,
                              ACTION_فیلد_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    أرجع PARSER_SUCCESS;
}

ساكن ثابت Lookup_مدخل قفل_Which[] = {
    { "both", 0 },
    { "lock", ACTION_قفل_NO_UNLOCK },
    { "neither", (ACTION_قفل_NO_قفل | ACTION_قفل_NO_UNLOCK) },
    { "unlock", ACTION_قفل_NO_قفل },
    { عدم, 0 }
};

ساكن تعداد xkb_parser_error
Check_Affect_فیلد(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, ثابت عبارت_Def *array_ndx,
                 ثابت عبارت_Def *مقدار, تعداد xkb_action_flags *flags_inout)
{
    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ACTION_فیلد_AFFECT, strict);

    uint32_t flags = 0;
    إذا (!عبارت_Resolve_Enum(ctx, مقدار, &flags, قفل_Which))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                              action, ACTION_فیلد_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_قفل_NO_قفل | ACTION_قفل_NO_UNLOCK);
    *flags_inout |= (تعداد xkb_action_flags) flags;
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_مجموعه_Latch_قفل_Mods(ثابت بنية xkb_keymap_info *keymap_info,
                       ثابت بنية xkb_mod_مجموعه *mods,
                       اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
                       ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
                       عبارت_Def **مقدار_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_mod_action *act = &action->mods;
    ثابت تعداد xkb_action_type type = action->type;

    إذا (فیلد_2 == ACTION_فیلد_MODIFIERS)
        أرجع Check_Modifier_فیلد(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, مقدار, &act->flags, &act->mods.mods);
    إذا (فیلد_2 == ACTION_فیلد_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        أكد(type == ACTION_TYPE_MOD_مجموعه || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_قفل);
        إذا (keymap_info->features.mods_unlock_on_press) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, فیلد_2,
                ACTION_UNLOCK_ON_PRESS, array_ndx, مقدار, &act->flags
            );
            إذا (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            أرجع ret;
        } وإلا {
            أرجع Report_Format_نسخه_Mismatch(ctx, action->type, فیلد_2,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    إذا ((type == ACTION_TYPE_MOD_مجموعه || type == ACTION_TYPE_MOD_LATCH) &&
        فیلد_2 == ACTION_فیلد_CLEAR_LOCKS)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, فیلد_2,
                                ACTION_قفل_CLEAR, array_ndx, مقدار,
                                &act->flags);
    إذا (type == ACTION_TYPE_MOD_LATCH) {
        إذا (فیلد_2 == ACTION_فیلد_LATCH_مقصد_قفل)
            أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    فیلد_2, ACTION_LATCH_مقصد_قفل, array_ndx,
                                    مقدار, &act->flags);
        إذا (فیلد_2 == ACTION_فیلد_LATCH_ON_PRESS) {
            إذا (keymap_info->features.mods_latch_on_press) {
                أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        فیلد_2, ACTION_LATCH_ON_PRESS, array_ndx,
                                        مقدار, &act->flags);
            } وإلا {
                أرجع Report_Format_نسخه_Mismatch(ctx, action->type, فیلد_2,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    إذا (type == ACTION_TYPE_MOD_قفل && فیلد_2 == ACTION_فیلد_AFFECT)
        أرجع Check_Affect_فیلد(ctx, keymap_info->strict, action->type,
                                array_ndx, مقدار, &act->flags);

    أرجع ReportIllegal(ctx, action->type, فیلد_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Check_گروه_فیلد(ثابت بنية xkb_keymap_info *keymap_info,
                تعداد xkb_action_type action, ثابت عبارت_Def *array_ndx,
                ثابت عبارت_Def *مقدار, عبارت_Def **مقدار_ptr,
                تعداد xkb_action_flags *flags_inout, int32_t *گروه_rtrn)
{
    ثابت عبارت_Def *spec;
    xkb_layout_شاخص_t idx = 0;
    تعداد xkb_action_flags flags = *flags_inout;

    إذا (array_ndx)
        أرجع ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_فیلد_گروه, keymap_info->strict);

    إذا (مقدار->common.type == STMT_عبارت_NEGATE ||
        مقدار->common.type == STMT_عبارت_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = مقدار->unary.فرزند;
        مقدار_ptr = &(*مقدار_ptr)->unary.فرزند;
    } وإلا {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = مقدار;
    }

    ثابت bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ثابت تعداد xkb_parser_error ret =
        عبارت_Resolve_گروه(keymap_info, spec, absolute, &idx, &pending);
    إذا (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2, action,
                       ACTION_فیلد_گروه, "integer", keymap_info->strict);
        أرجع ret;
    }

    إذا (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ثابت darray_اندازه_t pending_شاخص =
            darray_اندازه(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (بنية pending_computation) {
                .عبارت = *مقدار_ptr,
                .computed = false,
                .مقدار = 0,
            }
        );
        *مقدار_ptr = عدم;
        static_assert(حجم(pending_شاخص) == حجم(*گروه_rtrn),
                      "Cannot save pending computation");
        *گروه_rtrn = (int32_t) pending_شاخص;
    } وإلا {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        إذا (absolute) {
            *گروه_rtrn = (int32_t) (idx - 1);
        } وإلا {
            *گروه_rtrn = (int32_t) idx;
            إذا (مقدار->common.type == STMT_عبارت_NEGATE)
                *گروه_rtrn = -*گروه_rtrn;
        }
    }

    *flags_inout = flags;
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_مجموعه_Latch_قفل_گروه(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_مجموعه *mods,
                        اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
                        ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
                        عبارت_Def **مقدار_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_گروه_action *act = &action->گروه;
    ثابت تعداد xkb_action_type type = action->type;

    إذا (فیلد_2 == ACTION_فیلد_گروه) {
        أرجع Check_گروه_فیلد(keymap_info, action->type, array_ndx, مقدار,
                               مقدار_ptr, &act->flags, &act->گروه);
    }
    إذا ((type == ACTION_TYPE_گروه_مجموعه || type == ACTION_TYPE_گروه_LATCH) &&
        فیلد_2 == ACTION_فیلد_CLEAR_LOCKS)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, فیلد_2,
                                ACTION_قفل_CLEAR, array_ndx, مقدار,
                                &act->flags);
    إذا (type == ACTION_TYPE_گروه_LATCH &&
        فیلد_2 == ACTION_فیلد_LATCH_مقصد_قفل)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, فیلد_2,
                                ACTION_LATCH_مقصد_قفل, array_ndx, مقدار,
                                &act->flags);
    إذا (type == ACTION_TYPE_گروه_قفل &&
        فیلد_2 == ACTION_فیلد_قفل_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        إذا (keymap_info->features.گروه_قفل_on_release) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, فیلد_2,
                ACTION_قفل_ON_RELEASE, array_ndx, مقدار, &act->flags
            );
            إذا (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_قفل_ON_RELEASE;
            أرجع ret;
        } وإلا {
            أرجع Report_Format_نسخه_Mismatch(ctx, action->type, فیلد_2,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    أرجع ReportIllegal(ctx, action->type, فیلد_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleMovePtr(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_مجموعه *mods,
              اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
              ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
              عبارت_Def **مقدار_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشاره_گر_motion_action *act = &action->ptr;

    إذا (فیلد_2 == ACTION_فیلد_X || فیلد_2 == ACTION_فیلد_Y) {
        int64_t val = 0;
        ثابت bool absolute = (مقدار->common.type != STMT_عبارت_NEGATE &&
                               مقدار->common.type != STMT_عبارت_UNARY_PLUS);

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, فیلد_2,
                                        keymap_info->strict);

        إذا (!عبارت_Resolve_Integer(ctx, مقدار, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, action->type,
                                  فیلد_2, "integer", keymap_info->strict);

        إذا (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_پیام_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    فیلد_متن(فیلد_2), Action_Type_متن(action->type),
                    INT16_MIN, INT16_MAX, val);
            أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        إذا (فیلد_2 == ACTION_فیلد_X) {
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
    وإلا إذا (فیلد_2 == ACTION_فیلد_REPEAT) {
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, فیلد_2,
                                ACTION_REPEAT, array_ndx, مقدار, &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, فیلد_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandlePtrBtn(ثابت بنية xkb_keymap_info *keymap_info,
             ثابت بنية xkb_mod_مجموعه *mods,
             اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
             ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
             عبارت_Def **مقدار_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشاره_گر_button_action *act = &action->btn;

    إذا (فیلد_2 == ACTION_فیلد_BUTTON) {
        int64_t btn = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, فیلد_2,
                                        keymap_info->strict);

        إذا (!عبارت_Resolve_Button(ctx, مقدار, &btn)) {
            static_assert(XKB_اشاره_گر_BUTTON_MIN == 1 &&
                          XKB_اشاره_گر_BUTTON_MAX == 5, "invalid message");
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, action->type,
                                  فیلد_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_اشاره_گر_BUTTON_DEFAULT < XKB_اشاره_گر_BUTTON_MIN, "");
        إذا (btn < XKB_اشاره_گر_BUTTON_DEFAULT || btn > XKB_اشاره_گر_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_پیام_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_اشاره_گر_BUTTON_MIN, XKB_اشاره_گر_BUTTON_MAX, btn);
            أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (action->type == ACTION_TYPE_PTR_قفل &&
             فیلد_2 == ACTION_فیلد_AFFECT) {
        أرجع Check_Affect_فیلد(ctx, keymap_info->strict, action->type,
                                array_ndx, مقدار, &act->flags);
    }
    وإلا إذا (فیلد_2 == ACTION_فیلد_شمار) {
        int64_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, فیلد_2,
                                        keymap_info->strict);

        إذا (!عبارت_Resolve_Integer(ctx, مقدار, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, action->type,
                                  فیلد_2, "integer", keymap_info->strict);

        إذا (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_پیام_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        إذا (action->type != ACTION_TYPE_PTR_قفل)
            act->شمار = (uint8_t) val;
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, فیلد_2, keymap_info->strict);
}

تعداد {
    مجموعه_اشاره_گر_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    مجموعه_اشاره_گر_DEFAULT_BUTTON_AFFECT_رقم_ENTRIES
};

ساكن ثابت Lookup_مدخل ptrDflts[] = {
    { "dfltbtn", مجموعه_اشاره_گر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", مجموعه_اشاره_گر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", مجموعه_اشاره_گر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { عدم, 0 }
};

ساكن تعداد xkb_parser_error
Handle_مجموعه_Ptr_Dflt(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت بنية xkb_mod_مجموعه *mods,
                 اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
                 ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
                 عبارت_Def **مقدار_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشاره_گر_default_action *act = &action->dflt;

    إذا (فیلد_2 == ACTION_فیلد_AFFECT) {
        uint32_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, فیلد_2,
                                        keymap_info->strict);

        إذا (!عبارت_Resolve_Enum(ctx, مقدار, &val, ptrDflts))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, action->type,
                                  فیلد_2, "pointer component", keymap_info->strict);
        static_assert(مجموعه_اشاره_گر_DEFAULT_BUTTON_AFFECT_رقم_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (فیلد_2 == ACTION_فیلد_BUTTON || فیلد_2 == ACTION_فیلد_مقدار) {
        ثابت عبارت_Def *button;
        int64_t btn = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, فیلد_2,
                                        keymap_info->strict);

        إذا (مقدار->common.type == STMT_عبارت_NEGATE ||
            مقدار->common.type == STMT_عبارت_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = مقدار->unary.فرزند;
        }
        وإلا {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = مقدار;
        }

        إذا (!عبارت_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_اشاره_گر_BUTTON_MIN == 1 &&
                          XKB_اشاره_گر_BUTTON_MAX == 5, "invalid message");
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, action->type,
                                  فیلد_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_اشاره_گر_BUTTON_DEFAULT < XKB_اشاره_گر_BUTTON_MIN, "");
        إذا (btn < XKB_اشاره_گر_BUTTON_DEFAULT || btn > XKB_اشاره_گر_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_پیام_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_اشاره_گر_BUTTON_MIN, XKB_اشاره_گر_BUTTON_MAX, btn);
            أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        إذا (btn == XKB_اشاره_گر_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_پیام_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->مقدار = (int8_t) (مقدار->common.type == STMT_عبارت_NEGATE ? -btn: btn);
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, فیلد_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleSwitchScreen(ثابت بنية xkb_keymap_info *keymap_info,
                   ثابت بنية xkb_mod_مجموعه *mods,
                   اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
                   ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
                   عبارت_Def **مقدار_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_switch_screen_action *act = &action->screen;

    إذا (فیلد_2 == ACTION_فیلد_SCREEN) {
        ثابت عبارت_Def *scrn;
        int64_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, فیلد_2,
                                        keymap_info->strict);

        إذا (مقدار->common.type == STMT_عبارت_NEGATE ||
            مقدار->common.type == STMT_عبارت_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = مقدار->unary.فرزند;
        }
        وإلا {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = مقدار;
        }

        إذا (!عبارت_Resolve_Integer(ctx, scrn, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, action->type,
                                  فیلد_2, "integer (-128..127)",
                                  keymap_info->strict);

        val = (مقدار->common.type == STMT_عبارت_NEGATE ? -val : val);
        إذا (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_پیام_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (فیلد_2 == ACTION_فیلد_SAME) {
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, فیلد_2,
                                ACTION_SAME_SCREEN, array_ndx, مقدار,
                                &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, فیلد_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Handle_مجموعه_قفل_Controls(ثابت بنية xkb_keymap_info *keymap_info,
                      ثابت بنية xkb_mod_مجموعه *mods,
                      اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
                      ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
                      عبارت_Def **مقدار_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_controls_action *act = &action->ctrls;

    إذا (فیلد_2 == ACTION_فیلد_CONTROLS) {
        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, فیلد_2,
                                        keymap_info->strict);

        uint32_t ماسک = 0;
        ثابت uint8_t جابه_جایی = keymap_info->features.controls_name_جابه_جایی;
        إذا (!عبارت_Resolve_ماسک(ctx, مقدار, &ماسک, ctrl_ماسک_نامها + جابه_جایی))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, action->type,
                                  فیلد_2, "controls mask", keymap_info->strict);

        act->ctrls = (تعداد xkb_action_controls) ماسک;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (فیلد_2 == ACTION_فیلد_AFFECT &&
             action->type == ACTION_TYPE_CTRL_قفل) {
        أرجع Check_Affect_فیلد(ctx, keymap_info->strict, action->type,
                                array_ndx, مقدار, &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, فیلد_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Handle_Redirect_کلید(ثابت بنية xkb_keymap_info *keymap_info,
                  ثابت بنية xkb_mod_مجموعه *mods,
                  اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
                  ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
                  عبارت_Def **مقدار_ptr)
{
    ثابت بنية xkb_keymap * محدود ثابت keymap = &keymap_info->keymap;
    بنية xkb_context * محدود ثابت ctx = keymap->ctx;
    بنية xkb_redirect_کلید_action * ثابت act = &action->redirect;

    إذا (فیلد_2 == ACTION_فیلد_KEYCODE) {
        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, فیلد_2,
                                        keymap_info->strict);
        إذا (مقدار->common.type == STMT_عبارت_IDENT) {
            ثابت حرف *valStr = xkb_atom_متن(ctx, مقدار->ident.ident);
            إذا (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_کلید_auto;
                أرجع PARSER_SUCCESS;
            }
        }
        إذا (مقدار->common.type != STMT_عبارت_KEYNAME_LITERAL) {
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, action->type,
                                  فیلد_2, "key name", keymap_info->strict);
        }
        ثابت بنية xkb_کلید * ثابت کلید =
            Xkb_کلید_By_Name(keymap, مقدار->کلید_name.کلید_name, true);
        إذا (کلید == عدم) {
            log_err(ctx, XKB_LOG_پیام_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    فیلد_متن(فیلد_2), کلید_Name_متن(ctx, مقدار->کلید_name.کلید_name));
            أرجع (keymap_info->strict & PARSER_NO_فیلد_مقدار_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = کلید->keycode;
        أرجع PARSER_SUCCESS;
    }

    إذا (فیلد_2 == ACTION_فیلد_MODIFIERS || فیلد_2 == ACTION_فیلد_MODS_مقصد_CLEAR) {
        تعداد xkb_action_flags flags = 0;
        xkb_mod_ماسک_t m = 0;
        تعداد xkb_parser_error r =
            Check_Modifier_فیلد(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, مقدار, &flags, &m);
        إذا (r != PARSER_SUCCESS)
            أرجع r;
        إذا (flags)
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, action->type,
                                  فیلد_2, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        إذا (فیلد_2 == ACTION_فیلد_MODIFIERS)
            act->mods |= m;
        وإلا
            act->mods &= ~m;
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, فیلد_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleUnsupported(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_مجموعه *mods,
                        اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
                        ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
                        عبارت_Def **مقدار_ptr)

{
    /* Do not check any field */
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_خصوصی(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_مجموعه *mods,
              اتحاد xkb_action *action, تعداد action_فیلد فیلد_2,
              ثابت عبارت_Def *array_ndx, ثابت عبارت_Def *مقدار,
              عبارت_Def **مقدار_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_خصوصی_action *act = &action->priv;

    إذا (فیلد_2 == ACTION_فیلد_TYPE) {
        int64_t type = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, فیلد_2,
                                        keymap_info->strict);

        إذا (!عبارت_Resolve_Integer(ctx, مقدار, &type))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                                  ACTION_TYPE_خصوصی, فیلد_2, "integer",
                                  keymap_info->strict);

        إذا (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_پیام_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
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
        إذا (type < ACTION_TYPE_خصوصی) {
            log_info(ctx, XKB_LOG_پیام_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     Action_Type_متن((تعداد xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        وإلا {
            act->type = (تعداد xkb_action_type) type;
        }

        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (فیلد_2 == ACTION_فیلد_داده) {
        إذا (array_ndx == عدم) {
            xkb_atom_t val = XKB_ATOM_NONE;

            إذا (!عبارت_Resolve_String(ctx, مقدار, &val))
                أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                                      action->type, فیلد_2, "string",
                                      keymap_info->strict);

            ثابت حرف *str = xkb_atom_متن(ctx, val);
            اندازه_t len = طول_رشته(str);
            إذا (len < 1 || len > حجم(act->داده)) {
                log_warn(ctx, XKB_LOG_پیام_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", حجم(act->داده), len);
                أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->داده, 0, حجم(act->داده));
            کپی_حافظه(act->داده, str, len);
            أرجع PARSER_SUCCESS;
        }
        وإلا {
            int64_t ndx = 0, datum = 0;

            إذا (!عبارت_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_پیام_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            إذا (ndx < 0 || (اندازه_t) ndx >= حجم(act->داده)) {
                log_err(ctx, XKB_LOG_پیام_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        حجم(act->داده), ndx);
                أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            إذا (!عبارت_Resolve_Integer(ctx, مقدار, &datum))
                أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_فیلد_TYPE, act->type,
                                      فیلد_2, "integer", keymap_info->strict);

            إذا (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_پیام_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                أرجع (keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->داده[ndx] = (uint8_t) datum;
            أرجع PARSER_SUCCESS;
        }
    }

    أرجع ReportIllegal(ctx, action->type, فیلد_2, keymap_info->strict);
}

تعريفنوع تعداد xkb_parser_error
        (*actionHandler)(ثابت بنية xkb_keymap_info *keymap_info,
                         ثابت بنية xkb_mod_مجموعه *mods,
                         اتحاد xkb_action *action,
                         تعداد action_فیلد فیلد_2,
                         ثابت عبارت_Def *array_ndx,
                         ثابت عبارت_Def *مقدار,
                         عبارت_Def **مقدار_ptr);

ساكن ثابت actionHandler handleAction[ACTION_TYPE_رقم_ENTRIES] = {
    [ACTION_TYPE_NONE] = HandleNoAction,
    [ACTION_TYPE_VOID] = HandleNoAction,
    [ACTION_TYPE_MOD_مجموعه] = Handle_مجموعه_Latch_قفل_Mods,
    [ACTION_TYPE_MOD_LATCH] = Handle_مجموعه_Latch_قفل_Mods,
    [ACTION_TYPE_MOD_قفل] = Handle_مجموعه_Latch_قفل_Mods,
    [ACTION_TYPE_گروه_مجموعه] = Handle_مجموعه_Latch_قفل_گروه,
    [ACTION_TYPE_گروه_LATCH] = Handle_مجموعه_Latch_قفل_گروه,
    [ACTION_TYPE_گروه_قفل] = Handle_مجموعه_Latch_قفل_گروه,
    [ACTION_TYPE_PTR_MOVE] = HandleMovePtr,
    [ACTION_TYPE_PTR_BUTTON] = HandlePtrBtn,
    [ACTION_TYPE_PTR_قفل] = HandlePtrBtn,
    [ACTION_TYPE_PTR_DEFAULT] = Handle_مجموعه_Ptr_Dflt,
    [ACTION_TYPE_TERMINATE] = HandleNoAction,
    [ACTION_TYPE_SWITCH_VT] = HandleSwitchScreen,
    [ACTION_TYPE_CTRL_مجموعه] = Handle_مجموعه_قفل_Controls,
    [ACTION_TYPE_CTRL_قفل] = Handle_مجموعه_قفل_Controls,
    [ACTION_TYPE_REDIRECT_کلید] = Handle_Redirect_کلید,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_خصوصی] = Handle_خصوصی,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_داخلی == 20 &&
              ACTION_TYPE_داخلی + 1 == ACTION_TYPE_رقم_ENTRIES,
              "Missing action type");

/***====================================================================***/

تعداد xkb_parser_error
HandleActionDef(ثابت بنية xkb_keymap_info *keymap_info, ActionsInfo *info,
                ثابت بنية xkb_mod_مجموعه *mods, عبارت_Def *def,
                اتحاد xkb_action *action)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    إذا (def->common.type != STMT_عبارت_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_مقصد_string(def->common.type));
        أرجع PARSER_FATAL_ERROR;
    }

    ثابت حرف *action_name = xkb_atom_متن(ctx, def->action.name);
    تعداد xkb_action_type handler_type;
    إذا (!string_مقصد_Action_Type(action_name, &handler_type)) {
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
    لكل (عبارت_Def *arg = def->action.آرگومانها; arg != عدم;
         arg = (عبارت_Def *) arg->common.بعدی) {
        ثابت عبارت_Def *مقدار;
        عبارت_Def **مقدار_ptr = عدم;
        عبارت_Def *فیلد_2, *arrayRtrn;
        ثابت حرف *elemRtrn, *فیلد_Rtrn;

        إذا (arg->common.type == STMT_عبارت_تخصیص_بده) {
            فیلد_2 = arg->دودویی.چپ;
            مقدار = arg->دودویی.راست;
            مقدار_ptr = &arg->دودویی.راست;
        }
        وإلا إذا (arg->common.type == STMT_عبارت_NOT ||
                 arg->common.type == STMT_عبارت_INVERT) {
            فیلد_2 = arg->unary.فرزند;
            مقدار = (ثابت عبارت_Def *) &constFalse;
        }
        وإلا {
            فیلد_2 = arg;
            مقدار = (ثابت عبارت_Def *) &constTrue;
        }

        إذا (!عبارت_Resolve_Lhs(ctx, فیلد_2, &elemRtrn, &فیلد_Rtrn, &arrayRtrn))
            أرجع PARSER_FATAL_ERROR;

        إذا (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, فیلد_Rtrn);
            أرجع PARSER_FATAL_ERROR;
        }

        تعداد action_فیلد فیلد_Ndx;
        إذا (!string_مقصد_فیلد(فیلد_Rtrn, &فیلد_Ndx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_فیلد,
                    "Unknown field name %s for action %s discarded\n",
                    فیلد_Rtrn, Action_Type_متن(action->type));
            إذا (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                أرجع PARSER_FATAL_ERROR;
            } وإلا {
                /* Field is just ignored */
                استمر;
            }
        }

        بدل (handleAction[handler_type](keymap_info, mods, action, فیلد_Ndx,
                                           arrayRtrn, مقدار, مقدار_ptr)) {
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
مجموعه_Default_Action_فیلد(ثابت بنية xkb_keymap_info *keymap_info,
                      ActionsInfo *info, بنية xkb_mod_مجموعه *mods,
                      ثابت حرف *elem, ثابت حرف *فیلد_2, عبارت_Def *array_ndx,
                      عبارت_Def **مقدار_ptr, تعداد ادغام_نما ادغام)
{
    ثابت عبارت_Def * محدود ثابت مقدار = *مقدار_ptr;
    تعداد xkb_action_type action;
    إذا (!string_مقصد_Action_Type(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        أرجع (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    تعداد action_فیلد action_فیلد;
    إذا (!string_مقصد_فیلد(فیلد_2, &action_فیلد)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_فیلد,
                "Unknown action field \"%s\"\n", فیلد_2);
        أرجع (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    اتحاد xkb_action* ثابت into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    اتحاد xkb_action from = *into;

    /* Parse action */
    ثابت تعداد xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_فیلد, array_ndx, مقدار, مقدار_ptr
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
        ثابت bool replace = (ادغام != ادغام_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_پیام_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                فیلد_متن(action_فیلد), Action_Type_متن(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        إذا (replace)
            *into = from;
    }
    أرجع PARSER_SUCCESS;
}
