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

ساكن ثابت بیان_Boolean constTrue = {
    .common = { .type = STMT_بیان_BOOLEAN_LITERAL, .بل = عدم },
    .سېټ = true,
};

ساكن ثابت بیان_Boolean constFalse = {
    .common = { .type = STMT_بیان_BOOLEAN_LITERAL, .بل = عدم },
    .سېټ = false,
};

تعداد action_ډګر {
    ACTION_ډګر_CLEAR_LOCKS,
    ACTION_ډګر_LATCH_منزل_قلف,
    ACTION_ډګر_GEN_کلی_EVENT,
    ACTION_ډګر_REPORT,
    ACTION_ډګر_DEFAULT,
    ACTION_ډګر_AFFECT,
    ACTION_ډګر_INCREMENT,
    ACTION_ډګر_MODIFIERS,
    ACTION_ډګر_ډله,
    ACTION_ډګر_X,
    ACTION_ډګر_Y,
    ACTION_ډګر_REPEAT,
    ACTION_ډګر_BUTTON,
    ACTION_ډګر_ارزښت,
    ACTION_ډګر_CONTROLS,
    ACTION_ډګر_TYPE,
    ACTION_ډګر_شمېر,
    ACTION_ډګر_SCREEN,
    ACTION_ډګر_SAME,
    ACTION_ډګر_معلومات,
    ACTION_ډګر_DEVICE,
    ACTION_ډګر_KEYCODE,
    ACTION_ډګر_MODS_منزل_CLEAR,
    ACTION_ډګر_قلف_ON_RELEASE,
    ACTION_ډګر_UNLOCK_ON_PRESS,
    ACTION_ډګر_LATCH_ON_PRESS,
};

فراغ
InitActionsInfo(ثابت بنية xkb_keymap *keymap, ActionsInfo *info)
{
    لكل (تعداد xkb_action_type type = 0; type < ACTION_TYPE_عدد_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.ارزښت = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_کلی].redirect.keycode =
        keymap->redirect_کلی_auto;
}

ساكن ثابت Lookup_ثبت ډګر_مزي_ګانې[] = {
    { "clearLocks",       ACTION_ډګر_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_ډګر_LATCH_منزل_قلف   },
    { "genKeyEvent",      ACTION_ډګر_GEN_کلی_EVENT   },
    { "generateKeyEvent", ACTION_ډګر_GEN_کلی_EVENT   },
    { "report",           ACTION_ډګر_REPORT          },
    { "default",          ACTION_ډګر_DEFAULT         },
    { "affect",           ACTION_ډګر_AFFECT          },
    { "increment",        ACTION_ډګر_INCREMENT       },
    { "modifiers",        ACTION_ډګر_MODIFIERS       },
    { "mods",             ACTION_ډګر_MODIFIERS       },
    { "group",            ACTION_ډګر_ډله           },
    { "x",                ACTION_ډګر_X               },
    { "y",                ACTION_ډګر_Y               },
    { "repeat",           ACTION_ډګر_REPEAT          },
    { "accel",            ACTION_ډګر_REPEAT          },
    { "accelerate",       ACTION_ډګر_REPEAT          },
    { "button",           ACTION_ډګر_BUTTON          },
    { "value",            ACTION_ډګر_ارزښت           },
    { "controls",         ACTION_ډګر_CONTROLS        },
    { "ctrls",            ACTION_ډګر_CONTROLS        },
    { "type",             ACTION_ډګر_TYPE            },
    { "count",            ACTION_ډګر_شمېر           },
    { "screen",           ACTION_ډګر_SCREEN          },
    { "same",             ACTION_ډګر_SAME            },
    { "sameServer",       ACTION_ډګر_SAME            },
    { "data",             ACTION_ډګر_معلومات            },
    { "device",           ACTION_ډګر_DEVICE          },
    { "dev",              ACTION_ډګر_DEVICE          },
    { "key",              ACTION_ډګر_KEYCODE         },
    { "keycode",          ACTION_ډګر_KEYCODE         },
    { "kc",               ACTION_ډګر_KEYCODE         },
    { "clearmods",        ACTION_ډګر_MODS_منزل_CLEAR   },
    { "clearmodifiers",   ACTION_ډګر_MODS_منزل_CLEAR   },
    { "lockOnRelease",    ACTION_ډګر_قلف_ON_RELEASE },
    { "unlockOnPress",    ACTION_ډګر_UNLOCK_ON_PRESS },
    { "latchOnPress",     ACTION_ډګر_LATCH_ON_PRESS  },
    { عدم,               0                            }
};

ساكن bool
string_منزل_Action_Type(ثابت حرف *str, تعداد xkb_action_type *type_rtrn)
{
    بینښه صحيح type = 0;
    ثابت bool ret = LookupString(action_Type_نومونه, str, &type);
    *type_rtrn = (تعداد xkb_action_type) type;
    أرجع ret;
}

ساكن bool
string_منزل_ډګر(ثابت حرف *str, تعداد action_ډګر *ډګر_rtrn)
{
    بینښه صحيح ډګر_2 = 0;
    ثابت bool ret = LookupString(ډګر_مزي_ګانې, str, &ډګر_2);
    *ډګر_rtrn = (تعداد action_ډګر) ډګر_2;
    أرجع ret;
}

ساكن ثابت حرف *
ډګر_متن(تعداد action_ډګر ډګر_2)
{
    أرجع Lookup_ارزښت(ډګر_مزي_ګانې, ډګر_2);
}

/***====================================================================***/

ساكن مضمن تعداد xkb_parser_error
ReportMismatch(بنية xkb_context *ctx, تعداد xkb_پیغام_کوډ کوډ,
               تعداد xkb_action_type action, تعداد action_ډګر ډګر_2,
               ثابت حرف *type, تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, کوډ,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            ډګر_متن(ډګر_2), type, Action_Type_متن(action));
    أرجع (strict & PARSER_NO_ډګر_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن مضمن تعداد xkb_parser_error
Report_Format_نسخه_Mismatch(بنية xkb_context *ctx,
                            تعداد xkb_action_type action,
                            تعداد action_ډګر ډګر_2,
                            تعداد xkb_keymap_format format,
                            ثابت حرف *versions,
                            تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            ډګر_متن(ډګر_2), Action_Type_متن(action), versions, format);
    أرجع (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن مضمن تعداد xkb_parser_error
ReportIllegal(بنية xkb_context *ctx, تعداد xkb_action_type action,
              تعداد action_ډګر ډګر_2, تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_ډګر,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            ډګر_متن(ډګر_2), Action_Type_متن(action));
    أرجع (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن مضمن تعداد xkb_parser_error
ReportActionNotArray(بنية xkb_context *ctx, تعداد xkb_action_type action,
                     تعداد action_ډګر ډګر_2,
                     تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            ډګر_متن(ډګر_2), Action_Type_متن(action));
    أرجع (strict & PARSER_NO_ډګر_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن تعداد xkb_parser_error
HandleNoAction(ثابت بنية xkb_keymap_info *keymap_info,
               ثابت بنية xkb_mod_سېټ *mods,
               اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
               ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
               بیان_Def **ارزښت_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_ډګر,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            Action_Type_متن(action->type), ډګر_متن(ډګر_2));
    أرجع (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
CheckBooleanFlag(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, تعداد action_ډګر ډګر_2,
                 تعداد xkb_action_flags flag, ثابت بیان_Def *array_ndx,
                 ثابت بیان_Def *ارزښت, تعداد xkb_action_flags *flags_inout)
{
    bool سېټ = false;

    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ډګر_2, strict);

    إذا (!بیان_Resolve_Boolean(ctx, ارزښت, &سېټ))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                              action, ډګر_2, "boolean", strict);

    إذا (سېټ)
        *flags_inout |= flag;
    وإلا
        *flags_inout &= ~flag;

    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Check_Modifier_ډګر(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                   ثابت بنية xkb_mod_سېټ *mods, تعداد xkb_action_type action,
                   ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
                   تعداد xkb_action_flags *flags_inout, xkb_mod_ماسک_t *mods_rtrn)
{
    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ACTION_ډګر_MODIFIERS, strict);

    إذا (ارزښت->common.type == STMT_بیان_IDENT) {
        ثابت حرف *valStr;
        valStr = xkb_atom_متن(ctx, ارزښت->ident.ident);
        إذا (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            أرجع PARSER_SUCCESS;
        }
    }

    إذا (!بیان_Resolve_Mod_ماسک(ctx, ارزښت, MOD_BOTH, mods, mods_rtrn))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, action,
                              ACTION_ډګر_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    أرجع PARSER_SUCCESS;
}

ساكن ثابت Lookup_ثبت قلف_Which[] = {
    { "both", 0 },
    { "lock", ACTION_قلف_NO_UNLOCK },
    { "neither", (ACTION_قلف_NO_قلف | ACTION_قلف_NO_UNLOCK) },
    { "unlock", ACTION_قلف_NO_قلف },
    { عدم, 0 }
};

ساكن تعداد xkb_parser_error
Check_Affect_ډګر(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, ثابت بیان_Def *array_ndx,
                 ثابت بیان_Def *ارزښت, تعداد xkb_action_flags *flags_inout)
{
    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ACTION_ډګر_AFFECT, strict);

    uint32_t flags = 0;
    إذا (!بیان_Resolve_Enum(ctx, ارزښت, &flags, قلف_Which))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                              action, ACTION_ډګر_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_قلف_NO_قلف | ACTION_قلف_NO_UNLOCK);
    *flags_inout |= (تعداد xkb_action_flags) flags;
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_سېټ_Latch_قلف_Mods(ثابت بنية xkb_keymap_info *keymap_info,
                       ثابت بنية xkb_mod_سېټ *mods,
                       اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
                       ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
                       بیان_Def **ارزښت_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_mod_action *act = &action->mods;
    ثابت تعداد xkb_action_type type = action->type;

    إذا (ډګر_2 == ACTION_ډګر_MODIFIERS)
        أرجع Check_Modifier_ډګر(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, ارزښت, &act->flags, &act->mods.mods);
    إذا (ډګر_2 == ACTION_ډګر_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        أكد(type == ACTION_TYPE_MOD_سېټ || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_قلف);
        إذا (keymap_info->features.mods_unlock_on_press) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, ډګر_2,
                ACTION_UNLOCK_ON_PRESS, array_ndx, ارزښت, &act->flags
            );
            إذا (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            أرجع ret;
        } وإلا {
            أرجع Report_Format_نسخه_Mismatch(ctx, action->type, ډګر_2,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    إذا ((type == ACTION_TYPE_MOD_سېټ || type == ACTION_TYPE_MOD_LATCH) &&
        ډګر_2 == ACTION_ډګر_CLEAR_LOCKS)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, ډګر_2,
                                ACTION_قلف_CLEAR, array_ndx, ارزښت,
                                &act->flags);
    إذا (type == ACTION_TYPE_MOD_LATCH) {
        إذا (ډګر_2 == ACTION_ډګر_LATCH_منزل_قلف)
            أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    ډګر_2, ACTION_LATCH_منزل_قلف, array_ndx,
                                    ارزښت, &act->flags);
        إذا (ډګر_2 == ACTION_ډګر_LATCH_ON_PRESS) {
            إذا (keymap_info->features.mods_latch_on_press) {
                أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        ډګر_2, ACTION_LATCH_ON_PRESS, array_ndx,
                                        ارزښت, &act->flags);
            } وإلا {
                أرجع Report_Format_نسخه_Mismatch(ctx, action->type, ډګر_2,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    إذا (type == ACTION_TYPE_MOD_قلف && ډګر_2 == ACTION_ډګر_AFFECT)
        أرجع Check_Affect_ډګر(ctx, keymap_info->strict, action->type,
                                array_ndx, ارزښت, &act->flags);

    أرجع ReportIllegal(ctx, action->type, ډګر_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Check_ډله_ډګر(ثابت بنية xkb_keymap_info *keymap_info,
                تعداد xkb_action_type action, ثابت بیان_Def *array_ndx,
                ثابت بیان_Def *ارزښت, بیان_Def **ارزښت_ptr,
                تعداد xkb_action_flags *flags_inout, int32_t *ډله_rtrn)
{
    ثابت بیان_Def *spec;
    xkb_layout_شاخص_t idx = 0;
    تعداد xkb_action_flags flags = *flags_inout;

    إذا (array_ndx)
        أرجع ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_ډګر_ډله, keymap_info->strict);

    إذا (ارزښت->common.type == STMT_بیان_NEGATE ||
        ارزښت->common.type == STMT_بیان_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = ارزښت->unary.ماشوم;
        ارزښت_ptr = &(*ارزښت_ptr)->unary.ماشوم;
    } وإلا {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = ارزښت;
    }

    ثابت bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ثابت تعداد xkb_parser_error ret =
        بیان_Resolve_ډله(keymap_info, spec, absolute, &idx, &pending);
    إذا (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_شاخص_2, action,
                       ACTION_ډګر_ډله, "integer", keymap_info->strict);
        أرجع ret;
    }

    إذا (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ثابت darray_اندازه_t pending_شاخص =
            darray_اندازه(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (بنية pending_computation) {
                .بیان = *ارزښت_ptr,
                .computed = false,
                .ارزښت = 0,
            }
        );
        *ارزښت_ptr = عدم;
        static_assert(حجم(pending_شاخص) == حجم(*ډله_rtrn),
                      "Cannot save pending computation");
        *ډله_rtrn = (int32_t) pending_شاخص;
    } وإلا {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        إذا (absolute) {
            *ډله_rtrn = (int32_t) (idx - 1);
        } وإلا {
            *ډله_rtrn = (int32_t) idx;
            إذا (ارزښت->common.type == STMT_بیان_NEGATE)
                *ډله_rtrn = -*ډله_rtrn;
        }
    }

    *flags_inout = flags;
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_سېټ_Latch_قلف_ډله(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_سېټ *mods,
                        اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
                        ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
                        بیان_Def **ارزښت_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_ډله_action *act = &action->ډله;
    ثابت تعداد xkb_action_type type = action->type;

    إذا (ډګر_2 == ACTION_ډګر_ډله) {
        أرجع Check_ډله_ډګر(keymap_info, action->type, array_ndx, ارزښت,
                               ارزښت_ptr, &act->flags, &act->ډله);
    }
    إذا ((type == ACTION_TYPE_ډله_سېټ || type == ACTION_TYPE_ډله_LATCH) &&
        ډګر_2 == ACTION_ډګر_CLEAR_LOCKS)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, ډګر_2,
                                ACTION_قلف_CLEAR, array_ndx, ارزښت,
                                &act->flags);
    إذا (type == ACTION_TYPE_ډله_LATCH &&
        ډګر_2 == ACTION_ډګر_LATCH_منزل_قلف)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, ډګر_2,
                                ACTION_LATCH_منزل_قلف, array_ndx, ارزښت,
                                &act->flags);
    إذا (type == ACTION_TYPE_ډله_قلف &&
        ډګر_2 == ACTION_ډګر_قلف_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        إذا (keymap_info->features.ډله_قلف_on_release) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, ډګر_2,
                ACTION_قلف_ON_RELEASE, array_ndx, ارزښت, &act->flags
            );
            إذا (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_قلف_ON_RELEASE;
            أرجع ret;
        } وإلا {
            أرجع Report_Format_نسخه_Mismatch(ctx, action->type, ډګر_2,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    أرجع ReportIllegal(ctx, action->type, ډګر_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleMovePtr(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_سېټ *mods,
              اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
              ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
              بیان_Def **ارزښت_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشاره_ګر_motion_action *act = &action->ptr;

    إذا (ډګر_2 == ACTION_ډګر_X || ډګر_2 == ACTION_ډګر_Y) {
        int64_t val = 0;
        ثابت bool absolute = (ارزښت->common.type != STMT_بیان_NEGATE &&
                               ارزښت->common.type != STMT_بیان_UNARY_PLUS);

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ډګر_2,
                                        keymap_info->strict);

        إذا (!بیان_Resolve_Integer(ctx, ارزښت, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, action->type,
                                  ډګر_2, "integer", keymap_info->strict);

        إذا (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    ډګر_متن(ډګر_2), Action_Type_متن(action->type),
                    INT16_MIN, INT16_MAX, val);
            أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        إذا (ډګر_2 == ACTION_ډګر_X) {
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
    وإلا إذا (ډګر_2 == ACTION_ډګر_REPEAT) {
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, ډګر_2,
                                ACTION_REPEAT, array_ndx, ارزښت, &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, ډګر_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandlePtrBtn(ثابت بنية xkb_keymap_info *keymap_info,
             ثابت بنية xkb_mod_سېټ *mods,
             اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
             ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
             بیان_Def **ارزښت_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشاره_ګر_button_action *act = &action->btn;

    إذا (ډګر_2 == ACTION_ډګر_BUTTON) {
        int64_t btn = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ډګر_2,
                                        keymap_info->strict);

        إذا (!بیان_Resolve_Button(ctx, ارزښت, &btn)) {
            static_assert(XKB_اشاره_ګر_BUTTON_MIN == 1 &&
                          XKB_اشاره_ګر_BUTTON_MAX == 5, "invalid message");
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, action->type,
                                  ډګر_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_اشاره_ګر_BUTTON_DEFAULT < XKB_اشاره_ګر_BUTTON_MIN, "");
        إذا (btn < XKB_اشاره_ګر_BUTTON_DEFAULT || btn > XKB_اشاره_ګر_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_اشاره_ګر_BUTTON_MIN, XKB_اشاره_ګر_BUTTON_MAX, btn);
            أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (action->type == ACTION_TYPE_PTR_قلف &&
             ډګر_2 == ACTION_ډګر_AFFECT) {
        أرجع Check_Affect_ډګر(ctx, keymap_info->strict, action->type,
                                array_ndx, ارزښت, &act->flags);
    }
    وإلا إذا (ډګر_2 == ACTION_ډګر_شمېر) {
        int64_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ډګر_2,
                                        keymap_info->strict);

        إذا (!بیان_Resolve_Integer(ctx, ارزښت, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, action->type,
                                  ډګر_2, "integer", keymap_info->strict);

        إذا (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        إذا (action->type != ACTION_TYPE_PTR_قلف)
            act->شمېر = (uint8_t) val;
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, ډګر_2, keymap_info->strict);
}

تعداد {
    سېټ_اشاره_ګر_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    سېټ_اشاره_ګر_DEFAULT_BUTTON_AFFECT_عدد_ENTRIES
};

ساكن ثابت Lookup_ثبت ptrDflts[] = {
    { "dfltbtn", سېټ_اشاره_ګر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", سېټ_اشاره_ګر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", سېټ_اشاره_ګر_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { عدم, 0 }
};

ساكن تعداد xkb_parser_error
Handle_سېټ_Ptr_Dflt(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت بنية xkb_mod_سېټ *mods,
                 اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
                 ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
                 بیان_Def **ارزښت_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشاره_ګر_default_action *act = &action->dflt;

    إذا (ډګر_2 == ACTION_ډګر_AFFECT) {
        uint32_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ډګر_2,
                                        keymap_info->strict);

        إذا (!بیان_Resolve_Enum(ctx, ارزښت, &val, ptrDflts))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, action->type,
                                  ډګر_2, "pointer component", keymap_info->strict);
        static_assert(سېټ_اشاره_ګر_DEFAULT_BUTTON_AFFECT_عدد_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (ډګر_2 == ACTION_ډګر_BUTTON || ډګر_2 == ACTION_ډګر_ارزښت) {
        ثابت بیان_Def *button;
        int64_t btn = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ډګر_2,
                                        keymap_info->strict);

        إذا (ارزښت->common.type == STMT_بیان_NEGATE ||
            ارزښت->common.type == STMT_بیان_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = ارزښت->unary.ماشوم;
        }
        وإلا {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = ارزښت;
        }

        إذا (!بیان_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_اشاره_ګر_BUTTON_MIN == 1 &&
                          XKB_اشاره_ګر_BUTTON_MAX == 5, "invalid message");
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, action->type,
                                  ډګر_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_اشاره_ګر_BUTTON_DEFAULT < XKB_اشاره_ګر_BUTTON_MIN, "");
        إذا (btn < XKB_اشاره_ګر_BUTTON_DEFAULT || btn > XKB_اشاره_ګر_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_اشاره_ګر_BUTTON_MIN, XKB_اشاره_ګر_BUTTON_MAX, btn);
            أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        إذا (btn == XKB_اشاره_ګر_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->ارزښت = (int8_t) (ارزښت->common.type == STMT_بیان_NEGATE ? -btn: btn);
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, ډګر_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleSwitchScreen(ثابت بنية xkb_keymap_info *keymap_info,
                   ثابت بنية xkb_mod_سېټ *mods,
                   اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
                   ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
                   بیان_Def **ارزښت_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_switch_screen_action *act = &action->screen;

    إذا (ډګر_2 == ACTION_ډګر_SCREEN) {
        ثابت بیان_Def *scrn;
        int64_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ډګر_2,
                                        keymap_info->strict);

        إذا (ارزښت->common.type == STMT_بیان_NEGATE ||
            ارزښت->common.type == STMT_بیان_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = ارزښت->unary.ماشوم;
        }
        وإلا {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = ارزښت;
        }

        إذا (!بیان_Resolve_Integer(ctx, scrn, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, action->type,
                                  ډګر_2, "integer (-128..127)",
                                  keymap_info->strict);

        val = (ارزښت->common.type == STMT_بیان_NEGATE ? -val : val);
        إذا (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (ډګر_2 == ACTION_ډګر_SAME) {
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, ډګر_2,
                                ACTION_SAME_SCREEN, array_ndx, ارزښت,
                                &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, ډګر_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Handle_سېټ_قلف_Controls(ثابت بنية xkb_keymap_info *keymap_info,
                      ثابت بنية xkb_mod_سېټ *mods,
                      اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
                      ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
                      بیان_Def **ارزښت_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_controls_action *act = &action->ctrls;

    إذا (ډګر_2 == ACTION_ډګر_CONTROLS) {
        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ډګر_2,
                                        keymap_info->strict);

        uint32_t ماسک = 0;
        ثابت uint8_t دننه_واټن = keymap_info->features.controls_name_دننه_واټن;
        إذا (!بیان_Resolve_ماسک(ctx, ارزښت, &ماسک, ctrl_ماسک_نومونه + دننه_واټن))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, action->type,
                                  ډګر_2, "controls mask", keymap_info->strict);

        act->ctrls = (تعداد xkb_action_controls) ماسک;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (ډګر_2 == ACTION_ډګر_AFFECT &&
             action->type == ACTION_TYPE_CTRL_قلف) {
        أرجع Check_Affect_ډګر(ctx, keymap_info->strict, action->type,
                                array_ndx, ارزښت, &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, ډګر_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Handle_Redirect_کلی(ثابت بنية xkb_keymap_info *keymap_info,
                  ثابت بنية xkb_mod_سېټ *mods,
                  اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
                  ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
                  بیان_Def **ارزښت_ptr)
{
    ثابت بنية xkb_keymap * محدود ثابت keymap = &keymap_info->keymap;
    بنية xkb_context * محدود ثابت ctx = keymap->ctx;
    بنية xkb_redirect_کلی_action * ثابت act = &action->redirect;

    إذا (ډګر_2 == ACTION_ډګر_KEYCODE) {
        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ډګر_2,
                                        keymap_info->strict);
        إذا (ارزښت->common.type == STMT_بیان_IDENT) {
            ثابت حرف *valStr = xkb_atom_متن(ctx, ارزښت->ident.ident);
            إذا (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_کلی_auto;
                أرجع PARSER_SUCCESS;
            }
        }
        إذا (ارزښت->common.type != STMT_بیان_KEYNAME_LITERAL) {
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, action->type,
                                  ډګر_2, "key name", keymap_info->strict);
        }
        ثابت بنية xkb_کلی * ثابت کلی =
            Xkb_کلی_By_Name(keymap, ارزښت->کلی_name.کلی_name, true);
        إذا (کلی == عدم) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    ډګر_متن(ډګر_2), کلی_Name_متن(ctx, ارزښت->کلی_name.کلی_name));
            أرجع (keymap_info->strict & PARSER_NO_ډګر_ارزښت_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = کلی->keycode;
        أرجع PARSER_SUCCESS;
    }

    إذا (ډګر_2 == ACTION_ډګر_MODIFIERS || ډګر_2 == ACTION_ډګر_MODS_منزل_CLEAR) {
        تعداد xkb_action_flags flags = 0;
        xkb_mod_ماسک_t m = 0;
        تعداد xkb_parser_error r =
            Check_Modifier_ډګر(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, ارزښت, &flags, &m);
        إذا (r != PARSER_SUCCESS)
            أرجع r;
        إذا (flags)
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, action->type,
                                  ډګر_2, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        إذا (ډګر_2 == ACTION_ډګر_MODIFIERS)
            act->mods |= m;
        وإلا
            act->mods &= ~m;
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, ډګر_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleUnsupported(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_سېټ *mods,
                        اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
                        ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
                        بیان_Def **ارزښت_ptr)

{
    /* Do not check any field */
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_شخصي(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_سېټ *mods,
              اتحاد xkb_action *action, تعداد action_ډګر ډګر_2,
              ثابت بیان_Def *array_ndx, ثابت بیان_Def *ارزښت,
              بیان_Def **ارزښت_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_شخصي_action *act = &action->priv;

    إذا (ډګر_2 == ACTION_ډګر_TYPE) {
        int64_t type = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ډګر_2,
                                        keymap_info->strict);

        إذا (!بیان_Resolve_Integer(ctx, ارزښت, &type))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                                  ACTION_TYPE_شخصي, ډګر_2, "integer",
                                  keymap_info->strict);

        إذا (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_پیغام_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
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
        إذا (type < ACTION_TYPE_شخصي) {
            log_info(ctx, XKB_LOG_پیغام_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     Action_Type_متن((تعداد xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        وإلا {
            act->type = (تعداد xkb_action_type) type;
        }

        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (ډګر_2 == ACTION_ډګر_معلومات) {
        إذا (array_ndx == عدم) {
            xkb_atom_t val = XKB_ATOM_NONE;

            إذا (!بیان_Resolve_String(ctx, ارزښت, &val))
                أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                                      action->type, ډګر_2, "string",
                                      keymap_info->strict);

            ثابت حرف *str = xkb_atom_متن(ctx, val);
            اندازه_t len = مزي_اوږدوالی(str);
            إذا (len < 1 || len > حجم(act->معلومات)) {
                log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", حجم(act->معلومات), len);
                أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->معلومات, 0, حجم(act->معلومات));
            حافظه_نقل(act->معلومات, str, len);
            أرجع PARSER_SUCCESS;
        }
        وإلا {
            int64_t ndx = 0, datum = 0;

            إذا (!بیان_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            إذا (ndx < 0 || (اندازه_t) ndx >= حجم(act->معلومات)) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        حجم(act->معلومات), ndx);
                أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            إذا (!بیان_Resolve_Integer(ctx, ارزښت, &datum))
                أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ډګر_TYPE, act->type,
                                      ډګر_2, "integer", keymap_info->strict);

            إذا (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                أرجع (keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->معلومات[ndx] = (uint8_t) datum;
            أرجع PARSER_SUCCESS;
        }
    }

    أرجع ReportIllegal(ctx, action->type, ډګر_2, keymap_info->strict);
}

تعريفنوع تعداد xkb_parser_error
        (*actionHandler)(ثابت بنية xkb_keymap_info *keymap_info,
                         ثابت بنية xkb_mod_سېټ *mods,
                         اتحاد xkb_action *action,
                         تعداد action_ډګر ډګر_2,
                         ثابت بیان_Def *array_ndx,
                         ثابت بیان_Def *ارزښت,
                         بیان_Def **ارزښت_ptr);

ساكن ثابت actionHandler handleAction[ACTION_TYPE_عدد_ENTRIES] = {
    [ACTION_TYPE_NONE] = HandleNoAction,
    [ACTION_TYPE_VOID] = HandleNoAction,
    [ACTION_TYPE_MOD_سېټ] = Handle_سېټ_Latch_قلف_Mods,
    [ACTION_TYPE_MOD_LATCH] = Handle_سېټ_Latch_قلف_Mods,
    [ACTION_TYPE_MOD_قلف] = Handle_سېټ_Latch_قلف_Mods,
    [ACTION_TYPE_ډله_سېټ] = Handle_سېټ_Latch_قلف_ډله,
    [ACTION_TYPE_ډله_LATCH] = Handle_سېټ_Latch_قلف_ډله,
    [ACTION_TYPE_ډله_قلف] = Handle_سېټ_Latch_قلف_ډله,
    [ACTION_TYPE_PTR_MOVE] = HandleMovePtr,
    [ACTION_TYPE_PTR_BUTTON] = HandlePtrBtn,
    [ACTION_TYPE_PTR_قلف] = HandlePtrBtn,
    [ACTION_TYPE_PTR_DEFAULT] = Handle_سېټ_Ptr_Dflt,
    [ACTION_TYPE_TERMINATE] = HandleNoAction,
    [ACTION_TYPE_SWITCH_VT] = HandleSwitchScreen,
    [ACTION_TYPE_CTRL_سېټ] = Handle_سېټ_قلف_Controls,
    [ACTION_TYPE_CTRL_قلف] = Handle_سېټ_قلف_Controls,
    [ACTION_TYPE_REDIRECT_کلی] = Handle_Redirect_کلی,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_شخصي] = Handle_شخصي,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_دننی == 20 &&
              ACTION_TYPE_دننی + 1 == ACTION_TYPE_عدد_ENTRIES,
              "Missing action type");

/***====================================================================***/

تعداد xkb_parser_error
HandleActionDef(ثابت بنية xkb_keymap_info *keymap_info, ActionsInfo *info,
                ثابت بنية xkb_mod_سېټ *mods, بیان_Def *def,
                اتحاد xkb_action *action)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    إذا (def->common.type != STMT_بیان_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_ډګر_TYPE,
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
    لكل (بیان_Def *arg = def->action.دلیلونه; arg != عدم;
         arg = (بیان_Def *) arg->common.بل) {
        ثابت بیان_Def *ارزښت;
        بیان_Def **ارزښت_ptr = عدم;
        بیان_Def *ډګر_2, *arrayRtrn;
        ثابت حرف *elemRtrn, *ډګر_Rtrn;

        إذا (arg->common.type == STMT_بیان_ټاکل) {
            ډګر_2 = arg->دوګونی.کیڼ;
            ارزښت = arg->دوګونی.ښی;
            ارزښت_ptr = &arg->دوګونی.ښی;
        }
        وإلا إذا (arg->common.type == STMT_بیان_NOT ||
                 arg->common.type == STMT_بیان_INVERT) {
            ډګر_2 = arg->unary.ماشوم;
            ارزښت = (ثابت بیان_Def *) &constFalse;
        }
        وإلا {
            ډګر_2 = arg;
            ارزښت = (ثابت بیان_Def *) &constTrue;
        }

        إذا (!بیان_Resolve_Lhs(ctx, ډګر_2, &elemRtrn, &ډګر_Rtrn, &arrayRtrn))
            أرجع PARSER_FATAL_ERROR;

        إذا (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, ډګر_Rtrn);
            أرجع PARSER_FATAL_ERROR;
        }

        تعداد action_ډګر ډګر_Ndx;
        إذا (!string_منزل_ډګر(ډګر_Rtrn, &ډګر_Ndx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_ډګر,
                    "Unknown field name %s for action %s discarded\n",
                    ډګر_Rtrn, Action_Type_متن(action->type));
            إذا (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                أرجع PARSER_FATAL_ERROR;
            } وإلا {
                /* Field is just ignored */
                استمر;
            }
        }

        بدل (handleAction[handler_type](keymap_info, mods, action, ډګر_Ndx,
                                           arrayRtrn, ارزښت, ارزښت_ptr)) {
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
سېټ_Default_Action_ډګر(ثابت بنية xkb_keymap_info *keymap_info,
                      ActionsInfo *info, بنية xkb_mod_سېټ *mods,
                      ثابت حرف *elem, ثابت حرف *ډګر_2, بیان_Def *array_ndx,
                      بیان_Def **ارزښت_ptr, تعداد یوځای_ډېرپېښ یوځای)
{
    ثابت بیان_Def * محدود ثابت ارزښت = *ارزښت_ptr;
    تعداد xkb_action_type action;
    إذا (!string_منزل_Action_Type(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        أرجع (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    تعداد action_ډګر action_ډګر;
    إذا (!string_منزل_ډګر(ډګر_2, &action_ډګر)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_ډګر,
                "Unknown action field \"%s\"\n", ډګر_2);
        أرجع (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    اتحاد xkb_action* ثابت into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    اتحاد xkb_action from = *into;

    /* Parse action */
    ثابت تعداد xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_ډګر, array_ndx, ارزښت, ارزښت_ptr
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
        ثابت bool replace = (یوځای != یوځای_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_پیغام_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                ډګر_متن(action_ډګر), Action_Type_متن(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        إذا (replace)
            *into = from;
    }
    أرجع PARSER_SUCCESS;
}
