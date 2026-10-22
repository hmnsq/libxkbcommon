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

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "context.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "expr.h"
#అన్తర్భావ "xkbcomp/ast.h"
#అన్తర్భావ "action.h"

స్థిర స్థిరరాశి సమాసం_Boolean constTrue = {
    .common = { .type = STMT_సమాసం_BOOLEAN_LITERAL, .తదుపరి = ఖాళీ },
    .సమితి = true,
};

స్థిర స్థిరరాశి సమాసం_Boolean constFalse = {
    .common = { .type = STMT_సమాసం_BOOLEAN_LITERAL, .తదుపరి = ఖాళీ },
    .సమితి = false,
};

గణన action_క్షేత్రం {
    ACTION_క్షేత్రం_CLEAR_LOCKS,
    ACTION_క్షేత్రం_LATCH_గమ్యం_తాళం,
    ACTION_క్షేత్రం_GEN_కీలకం_EVENT,
    ACTION_క్షేత్రం_REPORT,
    ACTION_క్షేత్రం_DEFAULT,
    ACTION_క్షేత్రం_AFFECT,
    ACTION_క్షేత్రం_INCREMENT,
    ACTION_క్షేత్రం_MODIFIERS,
    ACTION_క్షేత్రం_సమూహం,
    ACTION_క్షేత్రం_X,
    ACTION_క్షేత్రం_Y,
    ACTION_క్షేత్రం_REPEAT,
    ACTION_క్షేత్రం_BUTTON,
    ACTION_క్షేత్రం_విలువ,
    ACTION_క్షేత్రం_CONTROLS,
    ACTION_క్షేత్రం_TYPE,
    ACTION_క్షేత్రం_లెక్క,
    ACTION_క్షేత్రం_SCREEN,
    ACTION_క్షేత్రం_SAME,
    ACTION_క్షేత్రం_దత్తాంశం,
    ACTION_క్షేత్రం_DEVICE,
    ACTION_క్షేత్రం_KEYCODE,
    ACTION_క్షేత్రం_MODS_గమ్యం_CLEAR,
    ACTION_క్షేత్రం_తాళం_ON_RELEASE,
    ACTION_క్షేత్రం_UNLOCK_ON_PRESS,
    ACTION_క్షేత్రం_LATCH_ON_PRESS,
};

శూన్య
InitActionsInfo(స్థిరరాశి నిర్మాణం xkb_keymap *keymap, ActionsInfo *info)
{
    కొరకు (గణన xkb_action_type type = 0; type < ACTION_TYPE_అంకె_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.విలువ = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_కీలకం].redirect.keycode =
        keymap->redirect_కీలకం_auto;
}

స్థిర స్థిరరాశి Lookup_నమోదు క్షేత్రం_అక్షరశ్రేణులు[] = {
    { "clearLocks",       ACTION_క్షేత్రం_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_క్షేత్రం_LATCH_గమ్యం_తాళం   },
    { "genKeyEvent",      ACTION_క్షేత్రం_GEN_కీలకం_EVENT   },
    { "generateKeyEvent", ACTION_క్షేత్రం_GEN_కీలకం_EVENT   },
    { "report",           ACTION_క్షేత్రం_REPORT          },
    { "default",          ACTION_క్షేత్రం_DEFAULT         },
    { "affect",           ACTION_క్షేత్రం_AFFECT          },
    { "increment",        ACTION_క్షేత్రం_INCREMENT       },
    { "modifiers",        ACTION_క్షేత్రం_MODIFIERS       },
    { "mods",             ACTION_క్షేత్రం_MODIFIERS       },
    { "group",            ACTION_క్షేత్రం_సమూహం           },
    { "x",                ACTION_క్షేత్రం_X               },
    { "y",                ACTION_క్షేత్రం_Y               },
    { "repeat",           ACTION_క్షేత్రం_REPEAT          },
    { "accel",            ACTION_క్షేత్రం_REPEAT          },
    { "accelerate",       ACTION_క్షేత్రం_REPEAT          },
    { "button",           ACTION_క్షేత్రం_BUTTON          },
    { "value",            ACTION_క్షేత్రం_విలువ           },
    { "controls",         ACTION_క్షేత్రం_CONTROLS        },
    { "ctrls",            ACTION_క్షేత్రం_CONTROLS        },
    { "type",             ACTION_క్షేత్రం_TYPE            },
    { "count",            ACTION_క్షేత్రం_లెక్క           },
    { "screen",           ACTION_క్షేత్రం_SCREEN          },
    { "same",             ACTION_క్షేత్రం_SAME            },
    { "sameServer",       ACTION_క్షేత్రం_SAME            },
    { "data",             ACTION_క్షేత్రం_దత్తాంశం            },
    { "device",           ACTION_క్షేత్రం_DEVICE          },
    { "dev",              ACTION_క్షేత్రం_DEVICE          },
    { "key",              ACTION_క్షేత్రం_KEYCODE         },
    { "keycode",          ACTION_క్షేత్రం_KEYCODE         },
    { "kc",               ACTION_క్షేత్రం_KEYCODE         },
    { "clearmods",        ACTION_క్షేత్రం_MODS_గమ్యం_CLEAR   },
    { "clearmodifiers",   ACTION_క్షేత్రం_MODS_గమ్యం_CLEAR   },
    { "lockOnRelease",    ACTION_క్షేత్రం_తాళం_ON_RELEASE },
    { "unlockOnPress",    ACTION_క్షేత్రం_UNLOCK_ON_PRESS },
    { "latchOnPress",     ACTION_క్షేత్రం_LATCH_ON_PRESS  },
    { ఖాళీ,               0                            }
};

స్థిర bool
string_గమ్యం_Action_Type(స్థిరరాశి అక్షరం *str, గణన xkb_action_type *type_rtrn)
{
    గుర్తులేని పూర్ణాంకం type = 0;
    స్థిరరాశి bool ret = LookupString(action_Type_పేర్లు, str, &type);
    *type_rtrn = (గణన xkb_action_type) type;
    తిరిగి ret;
}

స్థిర bool
string_గమ్యం_క్షేత్రం(స్థిరరాశి అక్షరం *str, గణన action_క్షేత్రం *క్షేత్రం_rtrn)
{
    గుర్తులేని పూర్ణాంకం క్షేత్రం_2 = 0;
    స్థిరరాశి bool ret = LookupString(క్షేత్రం_అక్షరశ్రేణులు, str, &క్షేత్రం_2);
    *క్షేత్రం_rtrn = (గణన action_క్షేత్రం) క్షేత్రం_2;
    తిరిగి ret;
}

స్థిర స్థిరరాశి అక్షరం *
క్షేత్రం_పాఠ్యం(గణన action_క్షేత్రం క్షేత్రం_2)
{
    తిరిగి Lookup_విలువ(క్షేత్రం_అక్షరశ్రేణులు, క్షేత్రం_2);
}

/***====================================================================***/

స్థిర అంతఃవరుస గణన xkb_parser_error
ReportMismatch(నిర్మాణం xkb_context *ctx, గణన xkb_సందేశం_సంకేతం సంకేతం_2,
               గణన xkb_action_type action, గణన action_క్షేత్రం క్షేత్రం_2,
               స్థిరరాశి అక్షరం *type, గణన xkb_parser_strict_flags strict)
{
    log_err(ctx, సంకేతం_2,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            క్షేత్రం_పాఠ్యం(క్షేత్రం_2), type, Action_Type_పాఠ్యం(action));
    తిరిగి (strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

స్థిర అంతఃవరుస గణన xkb_parser_error
Report_Format_సంస్కరణ_Mismatch(నిర్మాణం xkb_context *ctx,
                            గణన xkb_action_type action,
                            గణన action_క్షేత్రం క్షేత్రం_2,
                            గణన xkb_keymap_format format,
                            స్థిరరాశి అక్షరం *versions,
                            గణన xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_పాఠ్యం_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            క్షేత్రం_పాఠ్యం(క్షేత్రం_2), Action_Type_పాఠ్యం(action), versions, format);
    తిరిగి (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

స్థిర అంతఃవరుస గణన xkb_parser_error
ReportIllegal(నిర్మాణం xkb_context *ctx, గణన xkb_action_type action,
              గణన action_క్షేత్రం క్షేత్రం_2, గణన xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_క్షేత్రం,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            క్షేత్రం_పాఠ్యం(క్షేత్రం_2), Action_Type_పాఠ్యం(action));
    తిరిగి (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

స్థిర అంతఃవరుస గణన xkb_parser_error
ReportActionNotArray(నిర్మాణం xkb_context *ctx, గణన xkb_action_type action,
                     గణన action_క్షేత్రం క్షేత్రం_2,
                     గణన xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            క్షేత్రం_పాఠ్యం(క్షేత్రం_2), Action_Type_పాఠ్యం(action));
    తిరిగి (strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

స్థిర గణన xkb_parser_error
HandleNoAction(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
               స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
               సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
               స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
               సమాసం_Def **విలువ_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_క్షేత్రం,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            Action_Type_పాఠ్యం(action->type), క్షేత్రం_పాఠ్యం(క్షేత్రం_2));
    తిరిగి (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

స్థిర గణన xkb_parser_error
CheckBooleanFlag(నిర్మాణం xkb_context *ctx, గణన xkb_parser_strict_flags strict,
                 గణన xkb_action_type action, గణన action_క్షేత్రం క్షేత్రం_2,
                 గణన xkb_action_flags flag, స్థిరరాశి సమాసం_Def *array_ndx,
                 స్థిరరాశి సమాసం_Def *విలువ, గణన xkb_action_flags *flags_inout)
{
    bool సమితి = false;

    ఒకవేళ (array_ndx)
        తిరిగి ReportActionNotArray(ctx, action, క్షేత్రం_2, strict);

    ఒకవేళ (!సమాసం_Resolve_Boolean(ctx, విలువ, &సమితి))
        తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                              action, క్షేత్రం_2, "boolean", strict);

    ఒకవేళ (సమితి)
        *flags_inout |= flag;
    లేకపోతే
        *flags_inout &= ~flag;

    తిరిగి PARSER_SUCCESS;
}

స్థిర గణన xkb_parser_error
Check_Modifier_క్షేత్రం(నిర్మాణం xkb_context *ctx, గణన xkb_parser_strict_flags strict,
                   స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods, గణన xkb_action_type action,
                   స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
                   గణన xkb_action_flags *flags_inout, xkb_mod_ముసుగు_t *mods_rtrn)
{
    ఒకవేళ (array_ndx)
        తిరిగి ReportActionNotArray(ctx, action, ACTION_క్షేత్రం_MODIFIERS, strict);

    ఒకవేళ (విలువ->common.type == STMT_సమాసం_IDENT) {
        స్థిరరాశి అక్షరం *valStr;
        valStr = xkb_atom_పాఠ్యం(ctx, విలువ->ident.ident);
        ఒకవేళ (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            తిరిగి PARSER_SUCCESS;
        }
    }

    ఒకవేళ (!సమాసం_Resolve_Mod_ముసుగు(ctx, విలువ, MOD_BOTH, mods, mods_rtrn))
        తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, action,
                              ACTION_క్షేత్రం_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    తిరిగి PARSER_SUCCESS;
}

స్థిర స్థిరరాశి Lookup_నమోదు తాళం_Which[] = {
    { "both", 0 },
    { "lock", ACTION_తాళం_NO_UNLOCK },
    { "neither", (ACTION_తాళం_NO_తాళం | ACTION_తాళం_NO_UNLOCK) },
    { "unlock", ACTION_తాళం_NO_తాళం },
    { ఖాళీ, 0 }
};

స్థిర గణన xkb_parser_error
Check_Affect_క్షేత్రం(నిర్మాణం xkb_context *ctx, గణన xkb_parser_strict_flags strict,
                 గణన xkb_action_type action, స్థిరరాశి సమాసం_Def *array_ndx,
                 స్థిరరాశి సమాసం_Def *విలువ, గణన xkb_action_flags *flags_inout)
{
    ఒకవేళ (array_ndx)
        తిరిగి ReportActionNotArray(ctx, action, ACTION_క్షేత్రం_AFFECT, strict);

    uint32_t flags = 0;
    ఒకవేళ (!సమాసం_Resolve_Enum(ctx, విలువ, &flags, తాళం_Which))
        తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                              action, ACTION_క్షేత్రం_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_తాళం_NO_తాళం | ACTION_తాళం_NO_UNLOCK);
    *flags_inout |= (గణన xkb_action_flags) flags;
    తిరిగి PARSER_SUCCESS;
}

స్థిర గణన xkb_parser_error
Handle_సమితి_Latch_తాళం_Mods(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                       స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
                       సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
                       స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
                       సమాసం_Def **విలువ_ptr)
{
    నిర్మాణం xkb_context * పరిమిత స్థిరరాశి ctx = keymap_info->keymap.ctx;
    నిర్మాణం xkb_mod_action *act = &action->mods;
    స్థిరరాశి గణన xkb_action_type type = action->type;

    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_MODIFIERS)
        తిరిగి Check_Modifier_క్షేత్రం(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, విలువ, &act->flags, &act->mods.mods);
    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        ప్రతిజ్ఞా(type == ACTION_TYPE_MOD_సమితి || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_తాళం);
        ఒకవేళ (keymap_info->features.mods_unlock_on_press) {
            స్థిరరాశి గణన xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, క్షేత్రం_2,
                ACTION_UNLOCK_ON_PRESS, array_ndx, విలువ, &act->flags
            );
            ఒకవేళ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            తిరిగి ret;
        } లేకపోతే {
            తిరిగి Report_Format_సంస్కరణ_Mismatch(ctx, action->type, క్షేత్రం_2,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    ఒకవేళ ((type == ACTION_TYPE_MOD_సమితి || type == ACTION_TYPE_MOD_LATCH) &&
        క్షేత్రం_2 == ACTION_క్షేత్రం_CLEAR_LOCKS)
        తిరిగి CheckBooleanFlag(ctx, keymap_info->strict, action->type, క్షేత్రం_2,
                                ACTION_తాళం_CLEAR, array_ndx, విలువ,
                                &act->flags);
    ఒకవేళ (type == ACTION_TYPE_MOD_LATCH) {
        ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_LATCH_గమ్యం_తాళం)
            తిరిగి CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    క్షేత్రం_2, ACTION_LATCH_గమ్యం_తాళం, array_ndx,
                                    విలువ, &act->flags);
        ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_LATCH_ON_PRESS) {
            ఒకవేళ (keymap_info->features.mods_latch_on_press) {
                తిరిగి CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        క్షేత్రం_2, ACTION_LATCH_ON_PRESS, array_ndx,
                                        విలువ, &act->flags);
            } లేకపోతే {
                తిరిగి Report_Format_సంస్కరణ_Mismatch(ctx, action->type, క్షేత్రం_2,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    ఒకవేళ (type == ACTION_TYPE_MOD_తాళం && క్షేత్రం_2 == ACTION_క్షేత్రం_AFFECT)
        తిరిగి Check_Affect_క్షేత్రం(ctx, keymap_info->strict, action->type,
                                array_ndx, విలువ, &act->flags);

    తిరిగి ReportIllegal(ctx, action->type, క్షేత్రం_2, keymap_info->strict);
}

స్థిర గణన xkb_parser_error
Check_సమూహం_క్షేత్రం(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                గణన xkb_action_type action, స్థిరరాశి సమాసం_Def *array_ndx,
                స్థిరరాశి సమాసం_Def *విలువ, సమాసం_Def **విలువ_ptr,
                గణన xkb_action_flags *flags_inout, int32_t *సమూహం_rtrn)
{
    స్థిరరాశి సమాసం_Def *spec;
    xkb_layout_సూచీ_t idx = 0;
    గణన xkb_action_flags flags = *flags_inout;

    ఒకవేళ (array_ndx)
        తిరిగి ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_క్షేత్రం_సమూహం, keymap_info->strict);

    ఒకవేళ (విలువ->common.type == STMT_సమాసం_NEGATE ||
        విలువ->common.type == STMT_సమాసం_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = విలువ->unary.బిడ్డ;
        విలువ_ptr = &(*విలువ_ptr)->unary.బిడ్డ;
    } లేకపోతే {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = విలువ;
    }

    స్థిరరాశి bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    స్థిరరాశి గణన xkb_parser_error ret =
        సమాసం_Resolve_సమూహం(keymap_info, spec, absolute, &idx, &pending);
    ఒకవేళ (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_సూచీ_2, action,
                       ACTION_క్షేత్రం_సమూహం, "integer", keymap_info->strict);
        తిరిగి ret;
    }

    ఒకవేళ (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        స్థిరరాశి darray_కొలత_t pending_సూచీ =
            darray_కొలత(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (నిర్మాణం pending_computation) {
                .సమాసం = *విలువ_ptr,
                .computed = false,
                .విలువ = 0,
            }
        );
        *విలువ_ptr = ఖాళీ;
        static_assert(పరిమాణం(pending_సూచీ) == పరిమాణం(*సమూహం_rtrn),
                      "Cannot save pending computation");
        *సమూహం_rtrn = (int32_t) pending_సూచీ;
    } లేకపోతే {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        ఒకవేళ (absolute) {
            *సమూహం_rtrn = (int32_t) (idx - 1);
        } లేకపోతే {
            *సమూహం_rtrn = (int32_t) idx;
            ఒకవేళ (విలువ->common.type == STMT_సమాసం_NEGATE)
                *సమూహం_rtrn = -*సమూహం_rtrn;
        }
    }

    *flags_inout = flags;
    తిరిగి PARSER_SUCCESS;
}

స్థిర గణన xkb_parser_error
Handle_సమితి_Latch_తాళం_సమూహం(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                        స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
                        సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
                        స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
                        సమాసం_Def **విలువ_ptr)
{
    నిర్మాణం xkb_context * పరిమిత స్థిరరాశి ctx = keymap_info->keymap.ctx;
    నిర్మాణం xkb_సమూహం_action *act = &action->సమూహం;
    స్థిరరాశి గణన xkb_action_type type = action->type;

    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_సమూహం) {
        తిరిగి Check_సమూహం_క్షేత్రం(keymap_info, action->type, array_ndx, విలువ,
                               విలువ_ptr, &act->flags, &act->సమూహం);
    }
    ఒకవేళ ((type == ACTION_TYPE_సమూహం_సమితి || type == ACTION_TYPE_సమూహం_LATCH) &&
        క్షేత్రం_2 == ACTION_క్షేత్రం_CLEAR_LOCKS)
        తిరిగి CheckBooleanFlag(ctx, keymap_info->strict, action->type, క్షేత్రం_2,
                                ACTION_తాళం_CLEAR, array_ndx, విలువ,
                                &act->flags);
    ఒకవేళ (type == ACTION_TYPE_సమూహం_LATCH &&
        క్షేత్రం_2 == ACTION_క్షేత్రం_LATCH_గమ్యం_తాళం)
        తిరిగి CheckBooleanFlag(ctx, keymap_info->strict, action->type, క్షేత్రం_2,
                                ACTION_LATCH_గమ్యం_తాళం, array_ndx, విలువ,
                                &act->flags);
    ఒకవేళ (type == ACTION_TYPE_సమూహం_తాళం &&
        క్షేత్రం_2 == ACTION_క్షేత్రం_తాళం_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        ఒకవేళ (keymap_info->features.సమూహం_తాళం_on_release) {
            స్థిరరాశి గణన xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, క్షేత్రం_2,
                ACTION_తాళం_ON_RELEASE, array_ndx, విలువ, &act->flags
            );
            ఒకవేళ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_తాళం_ON_RELEASE;
            తిరిగి ret;
        } లేకపోతే {
            తిరిగి Report_Format_సంస్కరణ_Mismatch(ctx, action->type, క్షేత్రం_2,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    తిరిగి ReportIllegal(ctx, action->type, క్షేత్రం_2, keymap_info->strict);
}

స్థిర గణన xkb_parser_error
HandleMovePtr(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
              స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
              సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
              స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
              సమాసం_Def **విలువ_ptr)
{
    నిర్మాణం xkb_context * పరిమిత స్థిరరాశి ctx = keymap_info->keymap.ctx;
    నిర్మాణం xkb_సూచిక_motion_action *act = &action->ptr;

    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_X || క్షేత్రం_2 == ACTION_క్షేత్రం_Y) {
        int64_t val = 0;
        స్థిరరాశి bool absolute = (విలువ->common.type != STMT_సమాసం_NEGATE &&
                               విలువ->common.type != STMT_సమాసం_UNARY_PLUS);

        ఒకవేళ (array_ndx)
            తిరిగి ReportActionNotArray(ctx, action->type, క్షేత్రం_2,
                                        keymap_info->strict);

        ఒకవేళ (!సమాసం_Resolve_Integer(ctx, విలువ, &val))
            తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, action->type,
                                  క్షేత్రం_2, "integer", keymap_info->strict);

        ఒకవేళ (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    క్షేత్రం_పాఠ్యం(క్షేత్రం_2), Action_Type_పాఠ్యం(action->type),
                    INT16_MIN, INT16_MAX, val);
            తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_X) {
            ఒకవేళ (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        లేకపోతే {
            ఒకవేళ (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        తిరిగి PARSER_SUCCESS;
    }
    లేకపోతే ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_REPEAT) {
        తిరిగి CheckBooleanFlag(ctx, keymap_info->strict, action->type, క్షేత్రం_2,
                                ACTION_REPEAT, array_ndx, విలువ, &act->flags);
    }

    తిరిగి ReportIllegal(ctx, action->type, క్షేత్రం_2, keymap_info->strict);
}

స్థిర గణన xkb_parser_error
HandlePtrBtn(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
             స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
             సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
             స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
             సమాసం_Def **విలువ_ptr)
{
    నిర్మాణం xkb_context * పరిమిత స్థిరరాశి ctx = keymap_info->keymap.ctx;
    నిర్మాణం xkb_సూచిక_button_action *act = &action->btn;

    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_BUTTON) {
        int64_t btn = 0;

        ఒకవేళ (array_ndx)
            తిరిగి ReportActionNotArray(ctx, action->type, క్షేత్రం_2,
                                        keymap_info->strict);

        ఒకవేళ (!సమాసం_Resolve_Button(ctx, విలువ, &btn)) {
            static_assert(XKB_సూచిక_BUTTON_MIN == 1 &&
                          XKB_సూచిక_BUTTON_MAX == 5, "invalid message");
            తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, action->type,
                                  క్షేత్రం_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_సూచిక_BUTTON_DEFAULT < XKB_సూచిక_BUTTON_MIN, "");
        ఒకవేళ (btn < XKB_సూచిక_BUTTON_DEFAULT || btn > XKB_సూచిక_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_సూచిక_BUTTON_MIN, XKB_సూచిక_BUTTON_MAX, btn);
            తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        తిరిగి PARSER_SUCCESS;
    }
    లేకపోతే ఒకవేళ (action->type == ACTION_TYPE_PTR_తాళం &&
             క్షేత్రం_2 == ACTION_క్షేత్రం_AFFECT) {
        తిరిగి Check_Affect_క్షేత్రం(ctx, keymap_info->strict, action->type,
                                array_ndx, విలువ, &act->flags);
    }
    లేకపోతే ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_లెక్క) {
        int64_t val = 0;

        ఒకవేళ (array_ndx)
            తిరిగి ReportActionNotArray(ctx, action->type, క్షేత్రం_2,
                                        keymap_info->strict);

        ఒకవేళ (!సమాసం_Resolve_Integer(ctx, విలువ, &val))
            తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, action->type,
                                  క్షేత్రం_2, "integer", keymap_info->strict);

        ఒకవేళ (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        ఒకవేళ (action->type != ACTION_TYPE_PTR_తాళం)
            act->లెక్క = (uint8_t) val;
        తిరిగి PARSER_SUCCESS;
    }

    తిరిగి ReportIllegal(ctx, action->type, క్షేత్రం_2, keymap_info->strict);
}

గణన {
    సమితి_సూచిక_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    సమితి_సూచిక_DEFAULT_BUTTON_AFFECT_అంకె_ENTRIES
};

స్థిర స్థిరరాశి Lookup_నమోదు ptrDflts[] = {
    { "dfltbtn", సమితి_సూచిక_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", సమితి_సూచిక_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", సమితి_సూచిక_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { ఖాళీ, 0 }
};

స్థిర గణన xkb_parser_error
Handle_సమితి_Ptr_Dflt(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                 స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
                 సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
                 స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
                 సమాసం_Def **విలువ_ptr)
{
    నిర్మాణం xkb_context * పరిమిత స్థిరరాశి ctx = keymap_info->keymap.ctx;
    నిర్మాణం xkb_సూచిక_default_action *act = &action->dflt;

    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_AFFECT) {
        uint32_t val = 0;

        ఒకవేళ (array_ndx)
            తిరిగి ReportActionNotArray(ctx, action->type, క్షేత్రం_2,
                                        keymap_info->strict);

        ఒకవేళ (!సమాసం_Resolve_Enum(ctx, విలువ, &val, ptrDflts))
            తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, action->type,
                                  క్షేత్రం_2, "pointer component", keymap_info->strict);
        static_assert(సమితి_సూచిక_DEFAULT_BUTTON_AFFECT_అంకె_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        తిరిగి PARSER_SUCCESS;
    }
    లేకపోతే ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_BUTTON || క్షేత్రం_2 == ACTION_క్షేత్రం_విలువ) {
        స్థిరరాశి సమాసం_Def *button;
        int64_t btn = 0;

        ఒకవేళ (array_ndx)
            తిరిగి ReportActionNotArray(ctx, action->type, క్షేత్రం_2,
                                        keymap_info->strict);

        ఒకవేళ (విలువ->common.type == STMT_సమాసం_NEGATE ||
            విలువ->common.type == STMT_సమాసం_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = విలువ->unary.బిడ్డ;
        }
        లేకపోతే {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = విలువ;
        }

        ఒకవేళ (!సమాసం_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_సూచిక_BUTTON_MIN == 1 &&
                          XKB_సూచిక_BUTTON_MAX == 5, "invalid message");
            తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, action->type,
                                  క్షేత్రం_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_సూచిక_BUTTON_DEFAULT < XKB_సూచిక_BUTTON_MIN, "");
        ఒకవేళ (btn < XKB_సూచిక_BUTTON_DEFAULT || btn > XKB_సూచిక_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_సూచిక_BUTTON_MIN, XKB_సూచిక_BUTTON_MAX, btn);
            తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        ఒకవేళ (btn == XKB_సూచిక_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->విలువ = (int8_t) (విలువ->common.type == STMT_సమాసం_NEGATE ? -btn: btn);
        తిరిగి PARSER_SUCCESS;
    }

    తిరిగి ReportIllegal(ctx, action->type, క్షేత్రం_2, keymap_info->strict);
}

స్థిర గణన xkb_parser_error
HandleSwitchScreen(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                   స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
                   సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
                   స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
                   సమాసం_Def **విలువ_ptr)
{
    నిర్మాణం xkb_context * పరిమిత స్థిరరాశి ctx = keymap_info->keymap.ctx;
    నిర్మాణం xkb_switch_screen_action *act = &action->screen;

    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_SCREEN) {
        స్థిరరాశి సమాసం_Def *scrn;
        int64_t val = 0;

        ఒకవేళ (array_ndx)
            తిరిగి ReportActionNotArray(ctx, action->type, క్షేత్రం_2,
                                        keymap_info->strict);

        ఒకవేళ (విలువ->common.type == STMT_సమాసం_NEGATE ||
            విలువ->common.type == STMT_సమాసం_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = విలువ->unary.బిడ్డ;
        }
        లేకపోతే {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = విలువ;
        }

        ఒకవేళ (!సమాసం_Resolve_Integer(ctx, scrn, &val))
            తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, action->type,
                                  క్షేత్రం_2, "integer (-128..127)",
                                  keymap_info->strict);

        val = (విలువ->common.type == STMT_సమాసం_NEGATE ? -val : val);
        ఒకవేళ (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        తిరిగి PARSER_SUCCESS;
    }
    లేకపోతే ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_SAME) {
        తిరిగి CheckBooleanFlag(ctx, keymap_info->strict, action->type, క్షేత్రం_2,
                                ACTION_SAME_SCREEN, array_ndx, విలువ,
                                &act->flags);
    }

    తిరిగి ReportIllegal(ctx, action->type, క్షేత్రం_2, keymap_info->strict);
}

స్థిర గణన xkb_parser_error
Handle_సమితి_తాళం_Controls(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                      స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
                      సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
                      స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
                      సమాసం_Def **విలువ_ptr)
{
    నిర్మాణం xkb_context * పరిమిత స్థిరరాశి ctx = keymap_info->keymap.ctx;
    నిర్మాణం xkb_controls_action *act = &action->ctrls;

    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_CONTROLS) {
        ఒకవేళ (array_ndx)
            తిరిగి ReportActionNotArray(ctx, action->type, క్షేత్రం_2,
                                        keymap_info->strict);

        uint32_t ముసుగు = 0;
        స్థిరరాశి uint8_t స్థానభ్రంశం = keymap_info->features.controls_name_స్థానభ్రంశం;
        ఒకవేళ (!సమాసం_Resolve_ముసుగు(ctx, విలువ, &ముసుగు, ctrl_ముసుగు_పేర్లు + స్థానభ్రంశం))
            తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, action->type,
                                  క్షేత్రం_2, "controls mask", keymap_info->strict);

        act->ctrls = (గణన xkb_action_controls) ముసుగు;
        తిరిగి PARSER_SUCCESS;
    }
    లేకపోతే ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_AFFECT &&
             action->type == ACTION_TYPE_CTRL_తాళం) {
        తిరిగి Check_Affect_క్షేత్రం(ctx, keymap_info->strict, action->type,
                                array_ndx, విలువ, &act->flags);
    }

    తిరిగి ReportIllegal(ctx, action->type, క్షేత్రం_2, keymap_info->strict);
}

స్థిర గణన xkb_parser_error
Handle_Redirect_కీలకం(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                  స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
                  సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
                  స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
                  సమాసం_Def **విలువ_ptr)
{
    స్థిరరాశి నిర్మాణం xkb_keymap * పరిమిత స్థిరరాశి keymap = &keymap_info->keymap;
    నిర్మాణం xkb_context * పరిమిత స్థిరరాశి ctx = keymap->ctx;
    నిర్మాణం xkb_redirect_కీలకం_action * స్థిరరాశి act = &action->redirect;

    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_KEYCODE) {
        ఒకవేళ (array_ndx)
            తిరిగి ReportActionNotArray(ctx, action->type, క్షేత్రం_2,
                                        keymap_info->strict);
        ఒకవేళ (విలువ->common.type == STMT_సమాసం_IDENT) {
            స్థిరరాశి అక్షరం *valStr = xkb_atom_పాఠ్యం(ctx, విలువ->ident.ident);
            ఒకవేళ (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_కీలకం_auto;
                తిరిగి PARSER_SUCCESS;
            }
        }
        ఒకవేళ (విలువ->common.type != STMT_సమాసం_KEYNAME_LITERAL) {
            తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, action->type,
                                  క్షేత్రం_2, "key name", keymap_info->strict);
        }
        స్థిరరాశి నిర్మాణం xkb_కీలకం * స్థిరరాశి కీలకం =
            Xkb_కీలకం_By_Name(keymap, విలువ->కీలకం_name.కీలకం_name, true);
        ఒకవేళ (కీలకం == ఖాళీ) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    క్షేత్రం_పాఠ్యం(క్షేత్రం_2), కీలకం_Name_పాఠ్యం(ctx, విలువ->కీలకం_name.కీలకం_name));
            తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_విలువ_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = కీలకం->keycode;
        తిరిగి PARSER_SUCCESS;
    }

    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_MODIFIERS || క్షేత్రం_2 == ACTION_క్షేత్రం_MODS_గమ్యం_CLEAR) {
        గణన xkb_action_flags flags = 0;
        xkb_mod_ముసుగు_t m = 0;
        గణన xkb_parser_error r =
            Check_Modifier_క్షేత్రం(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, విలువ, &flags, &m);
        ఒకవేళ (r != PARSER_SUCCESS)
            తిరిగి r;
        ఒకవేళ (flags)
            తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, action->type,
                                  క్షేత్రం_2, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_MODIFIERS)
            act->mods |= m;
        లేకపోతే
            act->mods &= ~m;
        తిరిగి PARSER_SUCCESS;
    }

    తిరిగి ReportIllegal(ctx, action->type, క్షేత్రం_2, keymap_info->strict);
}

స్థిర గణన xkb_parser_error
HandleUnsupported(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                        స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
                        సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
                        స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
                        సమాసం_Def **విలువ_ptr)

{
    /* Do not check any field */
    తిరిగి PARSER_SUCCESS;
}

స్థిర గణన xkb_parser_error
Handle_వ్యక్తిగత(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
              స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
              సంఘం xkb_action *action, గణన action_క్షేత్రం క్షేత్రం_2,
              స్థిరరాశి సమాసం_Def *array_ndx, స్థిరరాశి సమాసం_Def *విలువ,
              సమాసం_Def **విలువ_ptr)
{
    నిర్మాణం xkb_context * పరిమిత స్థిరరాశి ctx = keymap_info->keymap.ctx;
    నిర్మాణం xkb_వ్యక్తిగత_action *act = &action->priv;

    ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_TYPE) {
        int64_t type = 0;

        ఒకవేళ (array_ndx)
            తిరిగి ReportActionNotArray(ctx, action->type, క్షేత్రం_2,
                                        keymap_info->strict);

        ఒకవేళ (!సమాసం_Resolve_Integer(ctx, విలువ, &type))
            తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                                  ACTION_TYPE_వ్యక్తిగత, క్షేత్రం_2, "integer",
                                  keymap_info->strict);

        ఒకవేళ (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
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
        ఒకవేళ (type < ACTION_TYPE_వ్యక్తిగత) {
            log_info(ctx, XKB_LOG_సందేశం_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     Action_Type_పాఠ్యం((గణన xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        లేకపోతే {
            act->type = (గణన xkb_action_type) type;
        }

        తిరిగి PARSER_SUCCESS;
    }
    లేకపోతే ఒకవేళ (క్షేత్రం_2 == ACTION_క్షేత్రం_దత్తాంశం) {
        ఒకవేళ (array_ndx == ఖాళీ) {
            xkb_atom_t val = XKB_ATOM_NONE;

            ఒకవేళ (!సమాసం_Resolve_String(ctx, విలువ, &val))
                తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                                      action->type, క్షేత్రం_2, "string",
                                      keymap_info->strict);

            స్థిరరాశి అక్షరం *str = xkb_atom_పాఠ్యం(ctx, val);
            కొలత_t len = అక్షరశ్రేణి_పొడవు(str);
            ఒకవేళ (len < 1 || len > పరిమాణం(act->దత్తాంశం)) {
                log_warn(ctx, XKB_LOG_సందేశం_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", పరిమాణం(act->దత్తాంశం), len);
                తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->దత్తాంశం, 0, పరిమాణం(act->దత్తాంశం));
            జ్ఞాపకం_నకలు(act->దత్తాంశం, str, len);
            తిరిగి PARSER_SUCCESS;
        }
        లేకపోతే {
            int64_t ndx = 0, datum = 0;

            ఒకవేళ (!సమాసం_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ఒకవేళ (ndx < 0 || (కొలత_t) ndx >= పరిమాణం(act->దత్తాంశం)) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        పరిమాణం(act->దత్తాంశం), ndx);
                తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ఒకవేళ (!సమాసం_Resolve_Integer(ctx, విలువ, &datum))
                తిరిగి ReportMismatch(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, act->type,
                                      క్షేత్రం_2, "integer", keymap_info->strict);

            ఒకవేళ (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                తిరిగి (keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->దత్తాంశం[ndx] = (uint8_t) datum;
            తిరిగి PARSER_SUCCESS;
        }
    }

    తిరిగి ReportIllegal(ctx, action->type, క్షేత్రం_2, keymap_info->strict);
}

రకనిర్వచనం గణన xkb_parser_error
        (*actionHandler)(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                         స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods,
                         సంఘం xkb_action *action,
                         గణన action_క్షేత్రం క్షేత్రం_2,
                         స్థిరరాశి సమాసం_Def *array_ndx,
                         స్థిరరాశి సమాసం_Def *విలువ,
                         సమాసం_Def **విలువ_ptr);

స్థిర స్థిరరాశి actionHandler handleAction[ACTION_TYPE_అంకె_ENTRIES] = {
    [ACTION_TYPE_NONE] = HandleNoAction,
    [ACTION_TYPE_VOID] = HandleNoAction,
    [ACTION_TYPE_MOD_సమితి] = Handle_సమితి_Latch_తాళం_Mods,
    [ACTION_TYPE_MOD_LATCH] = Handle_సమితి_Latch_తాళం_Mods,
    [ACTION_TYPE_MOD_తాళం] = Handle_సమితి_Latch_తాళం_Mods,
    [ACTION_TYPE_సమూహం_సమితి] = Handle_సమితి_Latch_తాళం_సమూహం,
    [ACTION_TYPE_సమూహం_LATCH] = Handle_సమితి_Latch_తాళం_సమూహం,
    [ACTION_TYPE_సమూహం_తాళం] = Handle_సమితి_Latch_తాళం_సమూహం,
    [ACTION_TYPE_PTR_MOVE] = HandleMovePtr,
    [ACTION_TYPE_PTR_BUTTON] = HandlePtrBtn,
    [ACTION_TYPE_PTR_తాళం] = HandlePtrBtn,
    [ACTION_TYPE_PTR_DEFAULT] = Handle_సమితి_Ptr_Dflt,
    [ACTION_TYPE_TERMINATE] = HandleNoAction,
    [ACTION_TYPE_SWITCH_VT] = HandleSwitchScreen,
    [ACTION_TYPE_CTRL_సమితి] = Handle_సమితి_తాళం_Controls,
    [ACTION_TYPE_CTRL_తాళం] = Handle_సమితి_తాళం_Controls,
    [ACTION_TYPE_REDIRECT_కీలకం] = Handle_Redirect_కీలకం,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_వ్యక్తిగత] = Handle_వ్యక్తిగత,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_అంతర్గత == 20 &&
              ACTION_TYPE_అంతర్గత + 1 == ACTION_TYPE_అంకె_ENTRIES,
              "Missing action type");

/***====================================================================***/

గణన xkb_parser_error
HandleActionDef(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info, ActionsInfo *info,
                స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods, సమాసం_Def *def,
                సంఘం xkb_action *action)
{
    నిర్మాణం xkb_context * పరిమిత స్థిరరాశి ctx = keymap_info->keymap.ctx;
    ఒకవేళ (def->common.type != STMT_సమాసం_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_గమ్యం_string(def->common.type));
        తిరిగి PARSER_FATAL_ERROR;
    }

    స్థిరరాశి అక్షరం *action_name = xkb_atom_పాఠ్యం(ctx, def->action.name);
    గణన xkb_action_type handler_type;
    ఒకవేళ (!string_గమ్యం_Action_Type(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        ఒకవేళ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            తిరిగి PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    ఒకవేళ (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    గణన xkb_parser_error ret = PARSER_SUCCESS;
    కొరకు (సమాసం_Def *arg = def->action.వాదనలు; arg != ఖాళీ;
         arg = (సమాసం_Def *) arg->common.తదుపరి) {
        స్థిరరాశి సమాసం_Def *విలువ;
        సమాసం_Def **విలువ_ptr = ఖాళీ;
        సమాసం_Def *క్షేత్రం_2, *arrayRtrn;
        స్థిరరాశి అక్షరం *elemRtrn, *క్షేత్రం_Rtrn;

        ఒకవేళ (arg->common.type == STMT_సమాసం_కేటాయించు) {
            క్షేత్రం_2 = arg->ద్విమాన.ఎడమ;
            విలువ = arg->ద్విమాన.కుడి;
            విలువ_ptr = &arg->ద్విమాన.కుడి;
        }
        లేకపోతే ఒకవేళ (arg->common.type == STMT_సమాసం_NOT ||
                 arg->common.type == STMT_సమాసం_INVERT) {
            క్షేత్రం_2 = arg->unary.బిడ్డ;
            విలువ = (స్థిరరాశి సమాసం_Def *) &constFalse;
        }
        లేకపోతే {
            క్షేత్రం_2 = arg;
            విలువ = (స్థిరరాశి సమాసం_Def *) &constTrue;
        }

        ఒకవేళ (!సమాసం_Resolve_Lhs(ctx, క్షేత్రం_2, &elemRtrn, &క్షేత్రం_Rtrn, &arrayRtrn))
            తిరిగి PARSER_FATAL_ERROR;

        ఒకవేళ (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, క్షేత్రం_Rtrn);
            తిరిగి PARSER_FATAL_ERROR;
        }

        గణన action_క్షేత్రం క్షేత్రం_Ndx;
        ఒకవేళ (!string_గమ్యం_క్షేత్రం(క్షేత్రం_Rtrn, &క్షేత్రం_Ndx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_క్షేత్రం,
                    "Unknown field name %s for action %s discarded\n",
                    క్షేత్రం_Rtrn, Action_Type_పాఠ్యం(action->type));
            ఒకవేళ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                తిరిగి PARSER_FATAL_ERROR;
            } లేకపోతే {
                /* Field is just ignored */
                కొనసాగు;
            }
        }

        ఎంపిక (handleAction[handler_type](keymap_info, mods, action, క్షేత్రం_Ndx,
                                           arrayRtrn, విలువ, విలువ_ptr)) {
        స్థితి PARSER_FATAL_ERROR:
            తిరిగి PARSER_FATAL_ERROR;
        స్థితి PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            ఆపు;
        అప్రమేయ:
            ;
        }
    }

    తిరిగి (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

గణన xkb_parser_error
సమితి_Default_Action_క్షేత్రం(స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                      ActionsInfo *info, నిర్మాణం xkb_mod_సమితి *mods,
                      స్థిరరాశి అక్షరం *elem, స్థిరరాశి అక్షరం *క్షేత్రం_2, సమాసం_Def *array_ndx,
                      సమాసం_Def **విలువ_ptr, గణన విలీనం_బహుళకం విలీనం)
{
    స్థిరరాశి సమాసం_Def * పరిమిత స్థిరరాశి విలువ = *విలువ_ptr;
    గణన xkb_action_type action;
    ఒకవేళ (!string_గమ్యం_Action_Type(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        తిరిగి (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    గణన action_క్షేత్రం action_క్షేత్రం;
    ఒకవేళ (!string_గమ్యం_క్షేత్రం(క్షేత్రం_2, &action_క్షేత్రం)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_క్షేత్రం,
                "Unknown action field \"%s\"\n", క్షేత్రం_2);
        తిరిగి (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    సంఘం xkb_action* స్థిరరాశి into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    సంఘం xkb_action from = *into;

    /* Parse action */
    స్థిరరాశి గణన xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_క్షేత్రం, array_ndx, విలువ, విలువ_ptr
    );

    /* Success is mandatory to update defaults */
    ఒకవేళ (ret != PARSER_SUCCESS)
        తిరిగి ret;

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
    ఒకవేళ (!action_equal(into, &from)) {
        స్థిరరాశి bool replace = (విలీనం != విలీనం_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_సందేశం_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                క్షేత్రం_పాఠ్యం(action_క్షేత్రం), Action_Type_పాఠ్యం(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        ఒకవేళ (replace)
            *into = from;
    }
    తిరిగి PARSER_SUCCESS;
}
