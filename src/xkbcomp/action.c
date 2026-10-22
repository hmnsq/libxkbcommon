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

ساكن ثابت اظهار_Boolean constTrue = {
    .common = { .type = STMT_اظهار_BOOLEAN_LITERAL, .ايندڙ = عدم },
    .سيٽ = true,
};

ساكن ثابت اظهار_Boolean constFalse = {
    .common = { .type = STMT_اظهار_BOOLEAN_LITERAL, .ايندڙ = عدم },
    .سيٽ = false,
};

تعداد action_ميدان {
    ACTION_ميدان_CLEAR_LOCKS,
    ACTION_ميدان_LATCH_منزل_تالو,
    ACTION_ميدان_GEN_ڪنجي_EVENT,
    ACTION_ميدان_REPORT,
    ACTION_ميدان_DEFAULT,
    ACTION_ميدان_AFFECT,
    ACTION_ميدان_INCREMENT,
    ACTION_ميدان_MODIFIERS,
    ACTION_ميدان_گروهه,
    ACTION_ميدان_X,
    ACTION_ميدان_Y,
    ACTION_ميدان_REPEAT,
    ACTION_ميدان_BUTTON,
    ACTION_ميدان_قدر,
    ACTION_ميدان_CONTROLS,
    ACTION_ميدان_TYPE,
    ACTION_ميدان_ڳڻپ,
    ACTION_ميدان_SCREEN,
    ACTION_ميدان_SAME,
    ACTION_ميدان_معلومات,
    ACTION_ميدان_DEVICE,
    ACTION_ميدان_KEYCODE,
    ACTION_ميدان_MODS_منزل_CLEAR,
    ACTION_ميدان_تالو_ON_RELEASE,
    ACTION_ميدان_UNLOCK_ON_PRESS,
    ACTION_ميدان_LATCH_ON_PRESS,
};

فراغ
InitActionsInfo(ثابت بنية xkb_keymap *keymap, ActionsInfo *info)
{
    لكل (تعداد xkb_action_type type = 0; type < ACTION_TYPE_انگ_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.قدر = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_ڪنجي].redirect.keycode =
        keymap->redirect_ڪنجي_auto;
}

ساكن ثابت Lookup_داخلا ميدان_لڙيون[] = {
    { "clearLocks",       ACTION_ميدان_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_ميدان_LATCH_منزل_تالو   },
    { "genKeyEvent",      ACTION_ميدان_GEN_ڪنجي_EVENT   },
    { "generateKeyEvent", ACTION_ميدان_GEN_ڪنجي_EVENT   },
    { "report",           ACTION_ميدان_REPORT          },
    { "default",          ACTION_ميدان_DEFAULT         },
    { "affect",           ACTION_ميدان_AFFECT          },
    { "increment",        ACTION_ميدان_INCREMENT       },
    { "modifiers",        ACTION_ميدان_MODIFIERS       },
    { "mods",             ACTION_ميدان_MODIFIERS       },
    { "group",            ACTION_ميدان_گروهه           },
    { "x",                ACTION_ميدان_X               },
    { "y",                ACTION_ميدان_Y               },
    { "repeat",           ACTION_ميدان_REPEAT          },
    { "accel",            ACTION_ميدان_REPEAT          },
    { "accelerate",       ACTION_ميدان_REPEAT          },
    { "button",           ACTION_ميدان_BUTTON          },
    { "value",            ACTION_ميدان_قدر           },
    { "controls",         ACTION_ميدان_CONTROLS        },
    { "ctrls",            ACTION_ميدان_CONTROLS        },
    { "type",             ACTION_ميدان_TYPE            },
    { "count",            ACTION_ميدان_ڳڻپ           },
    { "screen",           ACTION_ميدان_SCREEN          },
    { "same",             ACTION_ميدان_SAME            },
    { "sameServer",       ACTION_ميدان_SAME            },
    { "data",             ACTION_ميدان_معلومات            },
    { "device",           ACTION_ميدان_DEVICE          },
    { "dev",              ACTION_ميدان_DEVICE          },
    { "key",              ACTION_ميدان_KEYCODE         },
    { "keycode",          ACTION_ميدان_KEYCODE         },
    { "kc",               ACTION_ميدان_KEYCODE         },
    { "clearmods",        ACTION_ميدان_MODS_منزل_CLEAR   },
    { "clearmodifiers",   ACTION_ميدان_MODS_منزل_CLEAR   },
    { "lockOnRelease",    ACTION_ميدان_تالو_ON_RELEASE },
    { "unlockOnPress",    ACTION_ميدان_UNLOCK_ON_PRESS },
    { "latchOnPress",     ACTION_ميدان_LATCH_ON_PRESS  },
    { عدم,               0                            }
};

ساكن bool
string_منزل_Action_Type(ثابت حرف *str, تعداد xkb_action_type *type_rtrn)
{
    بنانشان صحيح type = 0;
    ثابت bool ret = LookupString(action_Type_نالا, str, &type);
    *type_rtrn = (تعداد xkb_action_type) type;
    أرجع ret;
}

ساكن bool
string_منزل_ميدان(ثابت حرف *str, تعداد action_ميدان *ميدان_rtrn)
{
    بنانشان صحيح ميدان_2 = 0;
    ثابت bool ret = LookupString(ميدان_لڙيون, str, &ميدان_2);
    *ميدان_rtrn = (تعداد action_ميدان) ميدان_2;
    أرجع ret;
}

ساكن ثابت حرف *
ميدان_متن(تعداد action_ميدان ميدان_2)
{
    أرجع Lookup_قدر(ميدان_لڙيون, ميدان_2);
}

/***====================================================================***/

ساكن مضمن تعداد xkb_parser_error
ReportMismatch(بنية xkb_context *ctx, تعداد xkb_پيغام_ڪوڊ ڪوڊ,
               تعداد xkb_action_type action, تعداد action_ميدان ميدان_2,
               ثابت حرف *type, تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, ڪوڊ,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            ميدان_متن(ميدان_2), type, Action_Type_متن(action));
    أرجع (strict & PARSER_NO_ميدان_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن مضمن تعداد xkb_parser_error
Report_Format_نسخو_Mismatch(بنية xkb_context *ctx,
                            تعداد xkb_action_type action,
                            تعداد action_ميدان ميدان_2,
                            تعداد xkb_keymap_format format,
                            ثابت حرف *versions,
                            تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            ميدان_متن(ميدان_2), Action_Type_متن(action), versions, format);
    أرجع (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن مضمن تعداد xkb_parser_error
ReportIllegal(بنية xkb_context *ctx, تعداد xkb_action_type action,
              تعداد action_ميدان ميدان_2, تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_ميدان,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            ميدان_متن(ميدان_2), Action_Type_متن(action));
    أرجع (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن مضمن تعداد xkb_parser_error
ReportActionNotArray(بنية xkb_context *ctx, تعداد xkb_action_type action,
                     تعداد action_ميدان ميدان_2,
                     تعداد xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            ميدان_متن(ميدان_2), Action_Type_متن(action));
    أرجع (strict & PARSER_NO_ميدان_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ساكن تعداد xkb_parser_error
HandleNoAction(ثابت بنية xkb_keymap_info *keymap_info,
               ثابت بنية xkb_mod_سيٽ *mods,
               اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
               ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
               اظهار_Def **قدر_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_ميدان,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            Action_Type_متن(action->type), ميدان_متن(ميدان_2));
    أرجع (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
CheckBooleanFlag(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, تعداد action_ميدان ميدان_2,
                 تعداد xkb_action_flags flag, ثابت اظهار_Def *array_ndx,
                 ثابت اظهار_Def *قدر, تعداد xkb_action_flags *flags_inout)
{
    bool سيٽ = false;

    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ميدان_2, strict);

    إذا (!اظهار_Resolve_Boolean(ctx, قدر, &سيٽ))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                              action, ميدان_2, "boolean", strict);

    إذا (سيٽ)
        *flags_inout |= flag;
    وإلا
        *flags_inout &= ~flag;

    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Check_Modifier_ميدان(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                   ثابت بنية xkb_mod_سيٽ *mods, تعداد xkb_action_type action,
                   ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
                   تعداد xkb_action_flags *flags_inout, xkb_mod_نقاب_t *mods_rtrn)
{
    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ACTION_ميدان_MODIFIERS, strict);

    إذا (قدر->common.type == STMT_اظهار_IDENT) {
        ثابت حرف *valStr;
        valStr = xkb_atom_متن(ctx, قدر->ident.ident);
        إذا (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            أرجع PARSER_SUCCESS;
        }
    }

    إذا (!اظهار_Resolve_Mod_نقاب(ctx, قدر, MOD_BOTH, mods, mods_rtrn))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, action,
                              ACTION_ميدان_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    أرجع PARSER_SUCCESS;
}

ساكن ثابت Lookup_داخلا تالو_Which[] = {
    { "both", 0 },
    { "lock", ACTION_تالو_NO_UNLOCK },
    { "neither", (ACTION_تالو_NO_تالو | ACTION_تالو_NO_UNLOCK) },
    { "unlock", ACTION_تالو_NO_تالو },
    { عدم, 0 }
};

ساكن تعداد xkb_parser_error
Check_Affect_ميدان(بنية xkb_context *ctx, تعداد xkb_parser_strict_flags strict,
                 تعداد xkb_action_type action, ثابت اظهار_Def *array_ndx,
                 ثابت اظهار_Def *قدر, تعداد xkb_action_flags *flags_inout)
{
    إذا (array_ndx)
        أرجع ReportActionNotArray(ctx, action, ACTION_ميدان_AFFECT, strict);

    uint32_t flags = 0;
    إذا (!اظهار_Resolve_Enum(ctx, قدر, &flags, تالو_Which))
        أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                              action, ACTION_ميدان_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_تالو_NO_تالو | ACTION_تالو_NO_UNLOCK);
    *flags_inout |= (تعداد xkb_action_flags) flags;
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_سيٽ_Latch_تالو_Mods(ثابت بنية xkb_keymap_info *keymap_info,
                       ثابت بنية xkb_mod_سيٽ *mods,
                       اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
                       ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
                       اظهار_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_mod_action *act = &action->mods;
    ثابت تعداد xkb_action_type type = action->type;

    إذا (ميدان_2 == ACTION_ميدان_MODIFIERS)
        أرجع Check_Modifier_ميدان(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, قدر, &act->flags, &act->mods.mods);
    إذا (ميدان_2 == ACTION_ميدان_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        أكد(type == ACTION_TYPE_MOD_سيٽ || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_تالو);
        إذا (keymap_info->features.mods_unlock_on_press) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, ميدان_2,
                ACTION_UNLOCK_ON_PRESS, array_ndx, قدر, &act->flags
            );
            إذا (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            أرجع ret;
        } وإلا {
            أرجع Report_Format_نسخو_Mismatch(ctx, action->type, ميدان_2,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    إذا ((type == ACTION_TYPE_MOD_سيٽ || type == ACTION_TYPE_MOD_LATCH) &&
        ميدان_2 == ACTION_ميدان_CLEAR_LOCKS)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, ميدان_2,
                                ACTION_تالو_CLEAR, array_ndx, قدر,
                                &act->flags);
    إذا (type == ACTION_TYPE_MOD_LATCH) {
        إذا (ميدان_2 == ACTION_ميدان_LATCH_منزل_تالو)
            أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    ميدان_2, ACTION_LATCH_منزل_تالو, array_ndx,
                                    قدر, &act->flags);
        إذا (ميدان_2 == ACTION_ميدان_LATCH_ON_PRESS) {
            إذا (keymap_info->features.mods_latch_on_press) {
                أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        ميدان_2, ACTION_LATCH_ON_PRESS, array_ndx,
                                        قدر, &act->flags);
            } وإلا {
                أرجع Report_Format_نسخو_Mismatch(ctx, action->type, ميدان_2,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    إذا (type == ACTION_TYPE_MOD_تالو && ميدان_2 == ACTION_ميدان_AFFECT)
        أرجع Check_Affect_ميدان(ctx, keymap_info->strict, action->type,
                                array_ndx, قدر, &act->flags);

    أرجع ReportIllegal(ctx, action->type, ميدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Check_گروهه_ميدان(ثابت بنية xkb_keymap_info *keymap_info,
                تعداد xkb_action_type action, ثابت اظهار_Def *array_ndx,
                ثابت اظهار_Def *قدر, اظهار_Def **قدر_ptr,
                تعداد xkb_action_flags *flags_inout, int32_t *گروهه_rtrn)
{
    ثابت اظهار_Def *spec;
    xkb_layout_اشاريو_t idx = 0;
    تعداد xkb_action_flags flags = *flags_inout;

    إذا (array_ndx)
        أرجع ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_ميدان_گروهه, keymap_info->strict);

    إذا (قدر->common.type == STMT_اظهار_NEGATE ||
        قدر->common.type == STMT_اظهار_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = قدر->unary.ٻار;
        قدر_ptr = &(*قدر_ptr)->unary.ٻار;
    } وإلا {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = قدر;
    }

    ثابت bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ثابت تعداد xkb_parser_error ret =
        اظهار_Resolve_گروهه(keymap_info, spec, absolute, &idx, &pending);
    إذا (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_اشاريو_2, action,
                       ACTION_ميدان_گروهه, "integer", keymap_info->strict);
        أرجع ret;
    }

    إذا (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ثابت darray_سائيز_t pending_اشاريو =
            darray_سائيز(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (بنية pending_computation) {
                .اظهار = *قدر_ptr,
                .computed = false,
                .قدر = 0,
            }
        );
        *قدر_ptr = عدم;
        static_assert(حجم(pending_اشاريو) == حجم(*گروهه_rtrn),
                      "Cannot save pending computation");
        *گروهه_rtrn = (int32_t) pending_اشاريو;
    } وإلا {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        إذا (absolute) {
            *گروهه_rtrn = (int32_t) (idx - 1);
        } وإلا {
            *گروهه_rtrn = (int32_t) idx;
            إذا (قدر->common.type == STMT_اظهار_NEGATE)
                *گروهه_rtrn = -*گروهه_rtrn;
        }
    }

    *flags_inout = flags;
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_سيٽ_Latch_تالو_گروهه(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_سيٽ *mods,
                        اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
                        ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
                        اظهار_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_گروهه_action *act = &action->گروهه;
    ثابت تعداد xkb_action_type type = action->type;

    إذا (ميدان_2 == ACTION_ميدان_گروهه) {
        أرجع Check_گروهه_ميدان(keymap_info, action->type, array_ndx, قدر,
                               قدر_ptr, &act->flags, &act->گروهه);
    }
    إذا ((type == ACTION_TYPE_گروهه_سيٽ || type == ACTION_TYPE_گروهه_LATCH) &&
        ميدان_2 == ACTION_ميدان_CLEAR_LOCKS)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, ميدان_2,
                                ACTION_تالو_CLEAR, array_ndx, قدر,
                                &act->flags);
    إذا (type == ACTION_TYPE_گروهه_LATCH &&
        ميدان_2 == ACTION_ميدان_LATCH_منزل_تالو)
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, ميدان_2,
                                ACTION_LATCH_منزل_تالو, array_ndx, قدر,
                                &act->flags);
    إذا (type == ACTION_TYPE_گروهه_تالو &&
        ميدان_2 == ACTION_ميدان_تالو_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        إذا (keymap_info->features.گروهه_تالو_on_release) {
            ثابت تعداد xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, ميدان_2,
                ACTION_تالو_ON_RELEASE, array_ndx, قدر, &act->flags
            );
            إذا (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_تالو_ON_RELEASE;
            أرجع ret;
        } وإلا {
            أرجع Report_Format_نسخو_Mismatch(ctx, action->type, ميدان_2,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    أرجع ReportIllegal(ctx, action->type, ميدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleMovePtr(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_سيٽ *mods,
              اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
              ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
              اظهار_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشارو_motion_action *act = &action->ptr;

    إذا (ميدان_2 == ACTION_ميدان_X || ميدان_2 == ACTION_ميدان_Y) {
        int64_t val = 0;
        ثابت bool absolute = (قدر->common.type != STMT_اظهار_NEGATE &&
                               قدر->common.type != STMT_اظهار_UNARY_PLUS);

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ميدان_2,
                                        keymap_info->strict);

        إذا (!اظهار_Resolve_Integer(ctx, قدر, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, action->type,
                                  ميدان_2, "integer", keymap_info->strict);

        إذا (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    ميدان_متن(ميدان_2), Action_Type_متن(action->type),
                    INT16_MIN, INT16_MAX, val);
            أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        إذا (ميدان_2 == ACTION_ميدان_X) {
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
    وإلا إذا (ميدان_2 == ACTION_ميدان_REPEAT) {
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, ميدان_2,
                                ACTION_REPEAT, array_ndx, قدر, &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, ميدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandlePtrBtn(ثابت بنية xkb_keymap_info *keymap_info,
             ثابت بنية xkb_mod_سيٽ *mods,
             اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
             ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
             اظهار_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشارو_button_action *act = &action->btn;

    إذا (ميدان_2 == ACTION_ميدان_BUTTON) {
        int64_t btn = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ميدان_2,
                                        keymap_info->strict);

        إذا (!اظهار_Resolve_Button(ctx, قدر, &btn)) {
            static_assert(XKB_اشارو_BUTTON_MIN == 1 &&
                          XKB_اشارو_BUTTON_MAX == 5, "invalid message");
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, action->type,
                                  ميدان_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_اشارو_BUTTON_DEFAULT < XKB_اشارو_BUTTON_MIN, "");
        إذا (btn < XKB_اشارو_BUTTON_DEFAULT || btn > XKB_اشارو_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_اشارو_BUTTON_MIN, XKB_اشارو_BUTTON_MAX, btn);
            أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (action->type == ACTION_TYPE_PTR_تالو &&
             ميدان_2 == ACTION_ميدان_AFFECT) {
        أرجع Check_Affect_ميدان(ctx, keymap_info->strict, action->type,
                                array_ndx, قدر, &act->flags);
    }
    وإلا إذا (ميدان_2 == ACTION_ميدان_ڳڻپ) {
        int64_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ميدان_2,
                                        keymap_info->strict);

        إذا (!اظهار_Resolve_Integer(ctx, قدر, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, action->type,
                                  ميدان_2, "integer", keymap_info->strict);

        إذا (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        إذا (action->type != ACTION_TYPE_PTR_تالو)
            act->ڳڻپ = (uint8_t) val;
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, ميدان_2, keymap_info->strict);
}

تعداد {
    سيٽ_اشارو_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    سيٽ_اشارو_DEFAULT_BUTTON_AFFECT_انگ_ENTRIES
};

ساكن ثابت Lookup_داخلا ptrDflts[] = {
    { "dfltbtn", سيٽ_اشارو_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", سيٽ_اشارو_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", سيٽ_اشارو_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { عدم, 0 }
};

ساكن تعداد xkb_parser_error
Handle_سيٽ_Ptr_Dflt(ثابت بنية xkb_keymap_info *keymap_info,
                 ثابت بنية xkb_mod_سيٽ *mods,
                 اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
                 ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
                 اظهار_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_اشارو_default_action *act = &action->dflt;

    إذا (ميدان_2 == ACTION_ميدان_AFFECT) {
        uint32_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ميدان_2,
                                        keymap_info->strict);

        إذا (!اظهار_Resolve_Enum(ctx, قدر, &val, ptrDflts))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, action->type,
                                  ميدان_2, "pointer component", keymap_info->strict);
        static_assert(سيٽ_اشارو_DEFAULT_BUTTON_AFFECT_انگ_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (ميدان_2 == ACTION_ميدان_BUTTON || ميدان_2 == ACTION_ميدان_قدر) {
        ثابت اظهار_Def *button;
        int64_t btn = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ميدان_2,
                                        keymap_info->strict);

        إذا (قدر->common.type == STMT_اظهار_NEGATE ||
            قدر->common.type == STMT_اظهار_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = قدر->unary.ٻار;
        }
        وإلا {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = قدر;
        }

        إذا (!اظهار_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_اشارو_BUTTON_MIN == 1 &&
                          XKB_اشارو_BUTTON_MAX == 5, "invalid message");
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, action->type,
                                  ميدان_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_اشارو_BUTTON_DEFAULT < XKB_اشارو_BUTTON_MIN, "");
        إذا (btn < XKB_اشارو_BUTTON_DEFAULT || btn > XKB_اشارو_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_اشارو_BUTTON_MIN, XKB_اشارو_BUTTON_MAX, btn);
            أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        إذا (btn == XKB_اشارو_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->قدر = (int8_t) (قدر->common.type == STMT_اظهار_NEGATE ? -btn: btn);
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, ميدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleSwitchScreen(ثابت بنية xkb_keymap_info *keymap_info,
                   ثابت بنية xkb_mod_سيٽ *mods,
                   اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
                   ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
                   اظهار_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_switch_screen_action *act = &action->screen;

    إذا (ميدان_2 == ACTION_ميدان_SCREEN) {
        ثابت اظهار_Def *scrn;
        int64_t val = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ميدان_2,
                                        keymap_info->strict);

        إذا (قدر->common.type == STMT_اظهار_NEGATE ||
            قدر->common.type == STMT_اظهار_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = قدر->unary.ٻار;
        }
        وإلا {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = قدر;
        }

        إذا (!اظهار_Resolve_Integer(ctx, scrn, &val))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, action->type,
                                  ميدان_2, "integer (-128..127)",
                                  keymap_info->strict);

        val = (قدر->common.type == STMT_اظهار_NEGATE ? -val : val);
        إذا (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (ميدان_2 == ACTION_ميدان_SAME) {
        أرجع CheckBooleanFlag(ctx, keymap_info->strict, action->type, ميدان_2,
                                ACTION_SAME_SCREEN, array_ndx, قدر,
                                &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, ميدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Handle_سيٽ_تالو_Controls(ثابت بنية xkb_keymap_info *keymap_info,
                      ثابت بنية xkb_mod_سيٽ *mods,
                      اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
                      ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
                      اظهار_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_controls_action *act = &action->ctrls;

    إذا (ميدان_2 == ACTION_ميدان_CONTROLS) {
        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ميدان_2,
                                        keymap_info->strict);

        uint32_t نقاب = 0;
        ثابت uint8_t وٿي = keymap_info->features.controls_name_وٿي;
        إذا (!اظهار_Resolve_نقاب(ctx, قدر, &نقاب, ctrl_نقاب_نالا + وٿي))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, action->type,
                                  ميدان_2, "controls mask", keymap_info->strict);

        act->ctrls = (تعداد xkb_action_controls) نقاب;
        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (ميدان_2 == ACTION_ميدان_AFFECT &&
             action->type == ACTION_TYPE_CTRL_تالو) {
        أرجع Check_Affect_ميدان(ctx, keymap_info->strict, action->type,
                                array_ndx, قدر, &act->flags);
    }

    أرجع ReportIllegal(ctx, action->type, ميدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
Handle_Redirect_ڪنجي(ثابت بنية xkb_keymap_info *keymap_info,
                  ثابت بنية xkb_mod_سيٽ *mods,
                  اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
                  ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
                  اظهار_Def **قدر_ptr)
{
    ثابت بنية xkb_keymap * محدود ثابت keymap = &keymap_info->keymap;
    بنية xkb_context * محدود ثابت ctx = keymap->ctx;
    بنية xkb_redirect_ڪنجي_action * ثابت act = &action->redirect;

    إذا (ميدان_2 == ACTION_ميدان_KEYCODE) {
        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ميدان_2,
                                        keymap_info->strict);
        إذا (قدر->common.type == STMT_اظهار_IDENT) {
            ثابت حرف *valStr = xkb_atom_متن(ctx, قدر->ident.ident);
            إذا (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_ڪنجي_auto;
                أرجع PARSER_SUCCESS;
            }
        }
        إذا (قدر->common.type != STMT_اظهار_KEYNAME_LITERAL) {
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, action->type,
                                  ميدان_2, "key name", keymap_info->strict);
        }
        ثابت بنية xkb_ڪنجي * ثابت ڪنجي =
            Xkb_ڪنجي_By_Name(keymap, قدر->ڪنجي_name.ڪنجي_name, true);
        إذا (ڪنجي == عدم) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    ميدان_متن(ميدان_2), ڪنجي_Name_متن(ctx, قدر->ڪنجي_name.ڪنجي_name));
            أرجع (keymap_info->strict & PARSER_NO_ميدان_قدر_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = ڪنجي->keycode;
        أرجع PARSER_SUCCESS;
    }

    إذا (ميدان_2 == ACTION_ميدان_MODIFIERS || ميدان_2 == ACTION_ميدان_MODS_منزل_CLEAR) {
        تعداد xkb_action_flags flags = 0;
        xkb_mod_نقاب_t m = 0;
        تعداد xkb_parser_error r =
            Check_Modifier_ميدان(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, قدر, &flags, &m);
        إذا (r != PARSER_SUCCESS)
            أرجع r;
        إذا (flags)
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, action->type,
                                  ميدان_2, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        إذا (ميدان_2 == ACTION_ميدان_MODIFIERS)
            act->mods |= m;
        وإلا
            act->mods &= ~m;
        أرجع PARSER_SUCCESS;
    }

    أرجع ReportIllegal(ctx, action->type, ميدان_2, keymap_info->strict);
}

ساكن تعداد xkb_parser_error
HandleUnsupported(ثابت بنية xkb_keymap_info *keymap_info,
                        ثابت بنية xkb_mod_سيٽ *mods,
                        اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
                        ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
                        اظهار_Def **قدر_ptr)

{
    /* Do not check any field */
    أرجع PARSER_SUCCESS;
}

ساكن تعداد xkb_parser_error
Handle_ذاتي(ثابت بنية xkb_keymap_info *keymap_info,
              ثابت بنية xkb_mod_سيٽ *mods,
              اتحاد xkb_action *action, تعداد action_ميدان ميدان_2,
              ثابت اظهار_Def *array_ndx, ثابت اظهار_Def *قدر,
              اظهار_Def **قدر_ptr)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    بنية xkb_ذاتي_action *act = &action->priv;

    إذا (ميدان_2 == ACTION_ميدان_TYPE) {
        int64_t type = 0;

        إذا (array_ndx)
            أرجع ReportActionNotArray(ctx, action->type, ميدان_2,
                                        keymap_info->strict);

        إذا (!اظهار_Resolve_Integer(ctx, قدر, &type))
            أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                                  ACTION_TYPE_ذاتي, ميدان_2, "integer",
                                  keymap_info->strict);

        إذا (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_پيغام_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
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
        إذا (type < ACTION_TYPE_ذاتي) {
            log_info(ctx, XKB_LOG_پيغام_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     Action_Type_متن((تعداد xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        وإلا {
            act->type = (تعداد xkb_action_type) type;
        }

        أرجع PARSER_SUCCESS;
    }
    وإلا إذا (ميدان_2 == ACTION_ميدان_معلومات) {
        إذا (array_ndx == عدم) {
            xkb_atom_t val = XKB_ATOM_NONE;

            إذا (!اظهار_Resolve_String(ctx, قدر, &val))
                أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                                      action->type, ميدان_2, "string",
                                      keymap_info->strict);

            ثابت حرف *str = xkb_atom_متن(ctx, val);
            سائيز_t len = لڙي_ڊيگهه(str);
            إذا (len < 1 || len > حجم(act->معلومات)) {
                log_warn(ctx, XKB_LOG_پيغام_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", حجم(act->معلومات), len);
                أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->معلومات, 0, حجم(act->معلومات));
            يادگيري_نقل(act->معلومات, str, len);
            أرجع PARSER_SUCCESS;
        }
        وإلا {
            int64_t ndx = 0, datum = 0;

            إذا (!اظهار_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_پيغام_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            إذا (ndx < 0 || (سائيز_t) ndx >= حجم(act->معلومات)) {
                log_err(ctx, XKB_LOG_پيغام_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        حجم(act->معلومات), ndx);
                أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            إذا (!اظهار_Resolve_Integer(ctx, قدر, &datum))
                أرجع ReportMismatch(ctx, XKB_ERROR_WRONG_ميدان_TYPE, act->type,
                                      ميدان_2, "integer", keymap_info->strict);

            إذا (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_پيغام_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                أرجع (keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->معلومات[ndx] = (uint8_t) datum;
            أرجع PARSER_SUCCESS;
        }
    }

    أرجع ReportIllegal(ctx, action->type, ميدان_2, keymap_info->strict);
}

تعريفنوع تعداد xkb_parser_error
        (*actionHandler)(ثابت بنية xkb_keymap_info *keymap_info,
                         ثابت بنية xkb_mod_سيٽ *mods,
                         اتحاد xkb_action *action,
                         تعداد action_ميدان ميدان_2,
                         ثابت اظهار_Def *array_ndx,
                         ثابت اظهار_Def *قدر,
                         اظهار_Def **قدر_ptr);

ساكن ثابت actionHandler handleAction[ACTION_TYPE_انگ_ENTRIES] = {
    [ACTION_TYPE_NONE] = HandleNoAction,
    [ACTION_TYPE_VOID] = HandleNoAction,
    [ACTION_TYPE_MOD_سيٽ] = Handle_سيٽ_Latch_تالو_Mods,
    [ACTION_TYPE_MOD_LATCH] = Handle_سيٽ_Latch_تالو_Mods,
    [ACTION_TYPE_MOD_تالو] = Handle_سيٽ_Latch_تالو_Mods,
    [ACTION_TYPE_گروهه_سيٽ] = Handle_سيٽ_Latch_تالو_گروهه,
    [ACTION_TYPE_گروهه_LATCH] = Handle_سيٽ_Latch_تالو_گروهه,
    [ACTION_TYPE_گروهه_تالو] = Handle_سيٽ_Latch_تالو_گروهه,
    [ACTION_TYPE_PTR_MOVE] = HandleMovePtr,
    [ACTION_TYPE_PTR_BUTTON] = HandlePtrBtn,
    [ACTION_TYPE_PTR_تالو] = HandlePtrBtn,
    [ACTION_TYPE_PTR_DEFAULT] = Handle_سيٽ_Ptr_Dflt,
    [ACTION_TYPE_TERMINATE] = HandleNoAction,
    [ACTION_TYPE_SWITCH_VT] = HandleSwitchScreen,
    [ACTION_TYPE_CTRL_سيٽ] = Handle_سيٽ_تالو_Controls,
    [ACTION_TYPE_CTRL_تالو] = Handle_سيٽ_تالو_Controls,
    [ACTION_TYPE_REDIRECT_ڪنجي] = Handle_Redirect_ڪنجي,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_ذاتي] = Handle_ذاتي,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_اندروني == 20 &&
              ACTION_TYPE_اندروني + 1 == ACTION_TYPE_انگ_ENTRIES,
              "Missing action type");

/***====================================================================***/

تعداد xkb_parser_error
HandleActionDef(ثابت بنية xkb_keymap_info *keymap_info, ActionsInfo *info,
                ثابت بنية xkb_mod_سيٽ *mods, اظهار_Def *def,
                اتحاد xkb_action *action)
{
    بنية xkb_context * محدود ثابت ctx = keymap_info->keymap.ctx;
    إذا (def->common.type != STMT_اظهار_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_ميدان_TYPE,
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
    لكل (اظهار_Def *arg = def->action.دليلون; arg != عدم;
         arg = (اظهار_Def *) arg->common.ايندڙ) {
        ثابت اظهار_Def *قدر;
        اظهار_Def **قدر_ptr = عدم;
        اظهار_Def *ميدان_2, *arrayRtrn;
        ثابت حرف *elemRtrn, *ميدان_Rtrn;

        إذا (arg->common.type == STMT_اظهار_سونپيو) {
            ميدان_2 = arg->ٻٽو.کاٻو;
            قدر = arg->ٻٽو.ساڄو;
            قدر_ptr = &arg->ٻٽو.ساڄو;
        }
        وإلا إذا (arg->common.type == STMT_اظهار_NOT ||
                 arg->common.type == STMT_اظهار_INVERT) {
            ميدان_2 = arg->unary.ٻار;
            قدر = (ثابت اظهار_Def *) &constFalse;
        }
        وإلا {
            ميدان_2 = arg;
            قدر = (ثابت اظهار_Def *) &constTrue;
        }

        إذا (!اظهار_Resolve_Lhs(ctx, ميدان_2, &elemRtrn, &ميدان_Rtrn, &arrayRtrn))
            أرجع PARSER_FATAL_ERROR;

        إذا (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, ميدان_Rtrn);
            أرجع PARSER_FATAL_ERROR;
        }

        تعداد action_ميدان ميدان_Ndx;
        إذا (!string_منزل_ميدان(ميدان_Rtrn, &ميدان_Ndx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_ميدان,
                    "Unknown field name %s for action %s discarded\n",
                    ميدان_Rtrn, Action_Type_متن(action->type));
            إذا (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                أرجع PARSER_FATAL_ERROR;
            } وإلا {
                /* Field is just ignored */
                استمر;
            }
        }

        بدل (handleAction[handler_type](keymap_info, mods, action, ميدان_Ndx,
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
سيٽ_Default_Action_ميدان(ثابت بنية xkb_keymap_info *keymap_info,
                      ActionsInfo *info, بنية xkb_mod_سيٽ *mods,
                      ثابت حرف *elem, ثابت حرف *ميدان_2, اظهار_Def *array_ndx,
                      اظهار_Def **قدر_ptr, تعداد ملايو_گهڻائي ملايو)
{
    ثابت اظهار_Def * محدود ثابت قدر = *قدر_ptr;
    تعداد xkb_action_type action;
    إذا (!string_منزل_Action_Type(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        أرجع (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    تعداد action_ميدان action_ميدان;
    إذا (!string_منزل_ميدان(ميدان_2, &action_ميدان)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_ميدان,
                "Unknown action field \"%s\"\n", ميدان_2);
        أرجع (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    اتحاد xkb_action* ثابت into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    اتحاد xkb_action from = *into;

    /* Parse action */
    ثابت تعداد xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_ميدان, array_ndx, قدر, قدر_ptr
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
        ثابت bool replace = (ملايو != ملايو_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_پيغام_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                ميدان_متن(action_ميدان), Action_Type_متن(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        إذا (replace)
            *into = from;
    }
    أرجع PARSER_SUCCESS;
}
