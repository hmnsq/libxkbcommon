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

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "expr.h"
#अन्तर्भाव "xkbcomp/ast.h"
#अन्तर्भाव "action.h"

स्थिर ध्रुव व्यञ्जकम्_Boolean constTrue = {
    .common = { .type = STMT_व्यञ्जकम्_BOOLEAN_LITERAL, .अग्र = रिक्त },
    .समुच्चयः = true,
};

स्थिर ध्रुव व्यञ्जकम्_Boolean constFalse = {
    .common = { .type = STMT_व्यञ्जकम्_BOOLEAN_LITERAL, .अग्र = रिक्त },
    .समुच्चयः = false,
};

गणना action_क्षेत्रम् {
    ACTION_क्षेत्रम्_CLEAR_LOCKS,
    ACTION_क्षेत्रम्_LATCH_गन्तव्यम्_तालकम्,
    ACTION_क्षेत्रम्_GEN_कुञ्चिका_EVENT,
    ACTION_क्षेत्रम्_REPORT,
    ACTION_क्षेत्रम्_DEFAULT,
    ACTION_क्षेत्रम्_AFFECT,
    ACTION_क्षेत्रम्_INCREMENT,
    ACTION_क्षेत्रम्_MODIFIERS,
    ACTION_क्षेत्रम्_गणः,
    ACTION_क्षेत्रम्_X,
    ACTION_क्षेत्रम्_Y,
    ACTION_क्षेत्रम्_REPEAT,
    ACTION_क्षेत्रम्_BUTTON,
    ACTION_क्षेत्रम्_मानम्,
    ACTION_क्षेत्रम्_CONTROLS,
    ACTION_क्षेत्रम्_TYPE,
    ACTION_क्षेत्रम्_गणनम्,
    ACTION_क्षेत्रम्_SCREEN,
    ACTION_क्षेत्रम्_SAME,
    ACTION_क्षेत्रम्_दत्त,
    ACTION_क्षेत्रम्_DEVICE,
    ACTION_क्षेत्रम्_KEYCODE,
    ACTION_क्षेत्रम्_MODS_गन्तव्यम्_CLEAR,
    ACTION_क्षेत्रम्_तालकम्_ON_RELEASE,
    ACTION_क्षेत्रम्_UNLOCK_ON_PRESS,
    ACTION_क्षेत्रम्_LATCH_ON_PRESS,
};

शून्य
InitActionsInfo(ध्रुव संरचना xkb_keymap *keymap, ActionsInfo *info)
{
    प्रति (गणना xkb_action_type type = 0; type < ACTION_TYPE_अङ्कः_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.मानम् = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_कुञ्चिका].redirect.keycode =
        keymap->redirect_कुञ्चिका_auto;
}

स्थिर ध्रुव Lookup_प्रविष्टिः क्षेत्रम्_वाक्यानि[] = {
    { "clearLocks",       ACTION_क्षेत्रम्_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_क्षेत्रम्_LATCH_गन्तव्यम्_तालकम्   },
    { "genKeyEvent",      ACTION_क्षेत्रम्_GEN_कुञ्चिका_EVENT   },
    { "generateKeyEvent", ACTION_क्षेत्रम्_GEN_कुञ्चिका_EVENT   },
    { "report",           ACTION_क्षेत्रम्_REPORT          },
    { "default",          ACTION_क्षेत्रम्_DEFAULT         },
    { "affect",           ACTION_क्षेत्रम्_AFFECT          },
    { "increment",        ACTION_क्षेत्रम्_INCREMENT       },
    { "modifiers",        ACTION_क्षेत्रम्_MODIFIERS       },
    { "mods",             ACTION_क्षेत्रम्_MODIFIERS       },
    { "group",            ACTION_क्षेत्रम्_गणः           },
    { "x",                ACTION_क्षेत्रम्_X               },
    { "y",                ACTION_क्षेत्रम्_Y               },
    { "repeat",           ACTION_क्षेत्रम्_REPEAT          },
    { "accel",            ACTION_क्षेत्रम्_REPEAT          },
    { "accelerate",       ACTION_क्षेत्रम्_REPEAT          },
    { "button",           ACTION_क्षेत्रम्_BUTTON          },
    { "value",            ACTION_क्षेत्रम्_मानम्           },
    { "controls",         ACTION_क्षेत्रम्_CONTROLS        },
    { "ctrls",            ACTION_क्षेत्रम्_CONTROLS        },
    { "type",             ACTION_क्षेत्रम्_TYPE            },
    { "count",            ACTION_क्षेत्रम्_गणनम्           },
    { "screen",           ACTION_क्षेत्रम्_SCREEN          },
    { "same",             ACTION_क्षेत्रम्_SAME            },
    { "sameServer",       ACTION_क्षेत्रम्_SAME            },
    { "data",             ACTION_क्षेत्रम्_दत्त            },
    { "device",           ACTION_क्षेत्रम्_DEVICE          },
    { "dev",              ACTION_क्षेत्रम्_DEVICE          },
    { "key",              ACTION_क्षेत्रम्_KEYCODE         },
    { "keycode",          ACTION_क्षेत्रम्_KEYCODE         },
    { "kc",               ACTION_क्षेत्रम्_KEYCODE         },
    { "clearmods",        ACTION_क्षेत्रम्_MODS_गन्तव्यम्_CLEAR   },
    { "clearmodifiers",   ACTION_क्षेत्रम्_MODS_गन्तव्यम्_CLEAR   },
    { "lockOnRelease",    ACTION_क्षेत्रम्_तालकम्_ON_RELEASE },
    { "unlockOnPress",    ACTION_क्षेत्रम्_UNLOCK_ON_PRESS },
    { "latchOnPress",     ACTION_क्षेत्रम्_LATCH_ON_PRESS  },
    { रिक्त,               0                            }
};

स्थिर bool
string_गन्तव्यम्_Action_Type(ध्रुव वर्ण *str, गणना xkb_action_type *type_rtrn)
{
    अचिह्नित पूर्णाङ्क type = 0;
    ध्रुव bool ret = LookupString(action_Type_नामानि, str, &type);
    *type_rtrn = (गणना xkb_action_type) type;
    प्रत्यागम ret;
}

स्थिर bool
string_गन्तव्यम्_क्षेत्रम्(ध्रुव वर्ण *str, गणना action_क्षेत्रम् *क्षेत्रम्_rtrn)
{
    अचिह्नित पूर्णाङ्क क्षेत्रम्_2 = 0;
    ध्रुव bool ret = LookupString(क्षेत्रम्_वाक्यानि, str, &क्षेत्रम्_2);
    *क्षेत्रम्_rtrn = (गणना action_क्षेत्रम्) क्षेत्रम्_2;
    प्रत्यागम ret;
}

स्थिर ध्रुव वर्ण *
क्षेत्रम्_Text(गणना action_क्षेत्रम् क्षेत्रम्_2)
{
    प्रत्यागम Lookup_मानम्(क्षेत्रम्_वाक्यानि, क्षेत्रम्_2);
}

/***====================================================================***/

स्थिर अन्तःस्थ गणना xkb_parser_error
ReportMismatch(संरचना xkb_context *ctx, गणना xkb_सन्देशः_संहिता संहिता,
               गणना xkb_action_type action, गणना action_क्षेत्रम् क्षेत्रम्_2,
               ध्रुव वर्ण *type, गणना xkb_parser_strict_flags strict)
{
    log_err(ctx, संहिता,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            क्षेत्रम्_Text(क्षेत्रम्_2), type, ActionTypeText(action));
    प्रत्यागम (strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

स्थिर अन्तःस्थ गणना xkb_parser_error
Report_Format_संस्करणम्_Mismatch(संरचना xkb_context *ctx,
                            गणना xkb_action_type action,
                            गणना action_क्षेत्रम् क्षेत्रम्_2,
                            गणना xkb_keymap_format format,
                            ध्रुव वर्ण *versions,
                            गणना xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            क्षेत्रम्_Text(क्षेत्रम्_2), ActionTypeText(action), versions, format);
    प्रत्यागम (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

स्थिर अन्तःस्थ गणना xkb_parser_error
ReportIllegal(संरचना xkb_context *ctx, गणना xkb_action_type action,
              गणना action_क्षेत्रम् क्षेत्रम्_2, गणना xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_क्षेत्रम्,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            क्षेत्रम्_Text(क्षेत्रम्_2), ActionTypeText(action));
    प्रत्यागम (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

स्थिर अन्तःस्थ गणना xkb_parser_error
ReportActionNotArray(संरचना xkb_context *ctx, गणना xkb_action_type action,
                     गणना action_क्षेत्रम् क्षेत्रम्_2,
                     गणना xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            क्षेत्रम्_Text(क्षेत्रम्_2), ActionTypeText(action));
    प्रत्यागम (strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

स्थिर गणना xkb_parser_error
HandleNoAction(ध्रुव संरचना xkb_keymap_info *keymap_info,
               ध्रुव संरचना xkb_mod_समुच्चयः *mods,
               सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
               ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
               व्यञ्जकम्_Def **मानम्_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_क्षेत्रम्,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            ActionTypeText(action->type), क्षेत्रम्_Text(क्षेत्रम्_2));
    प्रत्यागम (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

स्थिर गणना xkb_parser_error
CheckBooleanFlag(संरचना xkb_context *ctx, गणना xkb_parser_strict_flags strict,
                 गणना xkb_action_type action, गणना action_क्षेत्रम् क्षेत्रम्_2,
                 गणना xkb_action_flags flag, ध्रुव व्यञ्जकम्_Def *array_ndx,
                 ध्रुव व्यञ्जकम्_Def *मानम्, गणना xkb_action_flags *flags_inout)
{
    bool समुच्चयः = false;

    यदि (array_ndx)
        प्रत्यागम ReportActionNotArray(ctx, action, क्षेत्रम्_2, strict);

    यदि (!व्यञ्जकम्_Resolve_Boolean(ctx, मानम्, &समुच्चयः))
        प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                              action, क्षेत्रम्_2, "boolean", strict);

    यदि (समुच्चयः)
        *flags_inout |= flag;
    अन्यथा
        *flags_inout &= ~flag;

    प्रत्यागम PARSER_SUCCESS;
}

स्थिर गणना xkb_parser_error
Check_Modifier_क्षेत्रम्(संरचना xkb_context *ctx, गणना xkb_parser_strict_flags strict,
                   ध्रुव संरचना xkb_mod_समुच्चयः *mods, गणना xkb_action_type action,
                   ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
                   गणना xkb_action_flags *flags_inout, xkb_mod_आवरणम्_t *mods_rtrn)
{
    यदि (array_ndx)
        प्रत्यागम ReportActionNotArray(ctx, action, ACTION_क्षेत्रम्_MODIFIERS, strict);

    यदि (मानम्->common.type == STMT_व्यञ्जकम्_IDENT) {
        ध्रुव वर्ण *valStr;
        valStr = xkb_atom_text(ctx, मानम्->ident.ident);
        यदि (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            प्रत्यागम PARSER_SUCCESS;
        }
    }

    यदि (!व्यञ्जकम्_Resolve_Mod_आवरणम्(ctx, मानम्, MOD_BOTH, mods, mods_rtrn))
        प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, action,
                              ACTION_क्षेत्रम्_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    प्रत्यागम PARSER_SUCCESS;
}

स्थिर ध्रुव Lookup_प्रविष्टिः तालकम्_Which[] = {
    { "both", 0 },
    { "lock", ACTION_तालकम्_NO_UNLOCK },
    { "neither", (ACTION_तालकम्_NO_तालकम् | ACTION_तालकम्_NO_UNLOCK) },
    { "unlock", ACTION_तालकम्_NO_तालकम् },
    { रिक्त, 0 }
};

स्थिर गणना xkb_parser_error
Check_Affect_क्षेत्रम्(संरचना xkb_context *ctx, गणना xkb_parser_strict_flags strict,
                 गणना xkb_action_type action, ध्रुव व्यञ्जकम्_Def *array_ndx,
                 ध्रुव व्यञ्जकम्_Def *मानम्, गणना xkb_action_flags *flags_inout)
{
    यदि (array_ndx)
        प्रत्यागम ReportActionNotArray(ctx, action, ACTION_क्षेत्रम्_AFFECT, strict);

    uint32_t flags = 0;
    यदि (!व्यञ्जकम्_Resolve_Enum(ctx, मानम्, &flags, तालकम्_Which))
        प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                              action, ACTION_क्षेत्रम्_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_तालकम्_NO_तालकम् | ACTION_तालकम्_NO_UNLOCK);
    *flags_inout |= (गणना xkb_action_flags) flags;
    प्रत्यागम PARSER_SUCCESS;
}

स्थिर गणना xkb_parser_error
Handle_समुच्चयः_Latch_तालकम्_Mods(ध्रुव संरचना xkb_keymap_info *keymap_info,
                       ध्रुव संरचना xkb_mod_समुच्चयः *mods,
                       सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
                       ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
                       व्यञ्जकम्_Def **मानम्_ptr)
{
    संरचना xkb_context * सीमित ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_mod_action *act = &action->mods;
    ध्रुव गणना xkb_action_type type = action->type;

    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_MODIFIERS)
        प्रत्यागम Check_Modifier_क्षेत्रम्(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, मानम्, &act->flags, &act->mods.mods);
    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        प्रतिज्ञा(type == ACTION_TYPE_MOD_समुच्चयः || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_तालकम्);
        यदि (keymap_info->features.mods_unlock_on_press) {
            ध्रुव गणना xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, क्षेत्रम्_2,
                ACTION_UNLOCK_ON_PRESS, array_ndx, मानम्, &act->flags
            );
            यदि (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            प्रत्यागम ret;
        } अन्यथा {
            प्रत्यागम Report_Format_संस्करणम्_Mismatch(ctx, action->type, क्षेत्रम्_2,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    यदि ((type == ACTION_TYPE_MOD_समुच्चयः || type == ACTION_TYPE_MOD_LATCH) &&
        क्षेत्रम्_2 == ACTION_क्षेत्रम्_CLEAR_LOCKS)
        प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type, क्षेत्रम्_2,
                                ACTION_तालकम्_CLEAR, array_ndx, मानम्,
                                &act->flags);
    यदि (type == ACTION_TYPE_MOD_LATCH) {
        यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_LATCH_गन्तव्यम्_तालकम्)
            प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    क्षेत्रम्_2, ACTION_LATCH_गन्तव्यम्_तालकम्, array_ndx,
                                    मानम्, &act->flags);
        यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_LATCH_ON_PRESS) {
            यदि (keymap_info->features.mods_latch_on_press) {
                प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        क्षेत्रम्_2, ACTION_LATCH_ON_PRESS, array_ndx,
                                        मानम्, &act->flags);
            } अन्यथा {
                प्रत्यागम Report_Format_संस्करणम्_Mismatch(ctx, action->type, क्षेत्रम्_2,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    यदि (type == ACTION_TYPE_MOD_तालकम् && क्षेत्रम्_2 == ACTION_क्षेत्रम्_AFFECT)
        प्रत्यागम Check_Affect_क्षेत्रम्(ctx, keymap_info->strict, action->type,
                                array_ndx, मानम्, &act->flags);

    प्रत्यागम ReportIllegal(ctx, action->type, क्षेत्रम्_2, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
Check_गणः_क्षेत्रम्(ध्रुव संरचना xkb_keymap_info *keymap_info,
                गणना xkb_action_type action, ध्रुव व्यञ्जकम्_Def *array_ndx,
                ध्रुव व्यञ्जकम्_Def *मानम्, व्यञ्जकम्_Def **मानम्_ptr,
                गणना xkb_action_flags *flags_inout, int32_t *गणः_rtrn)
{
    ध्रुव व्यञ्जकम्_Def *spec;
    xkb_layout_सूचकांक_t idx = 0;
    गणना xkb_action_flags flags = *flags_inout;

    यदि (array_ndx)
        प्रत्यागम ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_क्षेत्रम्_गणः, keymap_info->strict);

    यदि (मानम्->common.type == STMT_व्यञ्जकम्_NEGATE ||
        मानम्->common.type == STMT_व्यञ्जकम्_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = मानम्->unary.child;
        मानम्_ptr = &(*मानम्_ptr)->unary.child;
    } अन्यथा {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = मानम्;
    }

    ध्रुव bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ध्रुव गणना xkb_parser_error ret =
        व्यञ्जकम्_Resolve_गणः(keymap_info, spec, absolute, &idx, &pending);
    यदि (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2, action,
                       ACTION_क्षेत्रम्_गणः, "integer", keymap_info->strict);
        प्रत्यागम ret;
    }

    यदि (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ध्रुव darray_आकार_t pending_सूचकांक =
            darray_आकार(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (संरचना pending_computation) {
                .व्यञ्जकम् = *मानम्_ptr,
                .computed = false,
                .मानम् = 0,
            }
        );
        *मानम्_ptr = रिक्त;
        static_assert(परिमाण(pending_सूचकांक) == परिमाण(*गणः_rtrn),
                      "Cannot save pending computation");
        *गणः_rtrn = (int32_t) pending_सूचकांक;
    } अन्यथा {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        यदि (absolute) {
            *गणः_rtrn = (int32_t) (idx - 1);
        } अन्यथा {
            *गणः_rtrn = (int32_t) idx;
            यदि (मानम्->common.type == STMT_व्यञ्जकम्_NEGATE)
                *गणः_rtrn = -*गणः_rtrn;
        }
    }

    *flags_inout = flags;
    प्रत्यागम PARSER_SUCCESS;
}

स्थिर गणना xkb_parser_error
Handle_समुच्चयः_Latch_तालकम्_गणः(ध्रुव संरचना xkb_keymap_info *keymap_info,
                        ध्रुव संरचना xkb_mod_समुच्चयः *mods,
                        सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
                        ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
                        व्यञ्जकम्_Def **मानम्_ptr)
{
    संरचना xkb_context * सीमित ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_गणः_action *act = &action->गणः;
    ध्रुव गणना xkb_action_type type = action->type;

    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_गणः) {
        प्रत्यागम Check_गणः_क्षेत्रम्(keymap_info, action->type, array_ndx, मानम्,
                               मानम्_ptr, &act->flags, &act->गणः);
    }
    यदि ((type == ACTION_TYPE_गणः_समुच्चयः || type == ACTION_TYPE_गणः_LATCH) &&
        क्षेत्रम्_2 == ACTION_क्षेत्रम्_CLEAR_LOCKS)
        प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type, क्षेत्रम्_2,
                                ACTION_तालकम्_CLEAR, array_ndx, मानम्,
                                &act->flags);
    यदि (type == ACTION_TYPE_गणः_LATCH &&
        क्षेत्रम्_2 == ACTION_क्षेत्रम्_LATCH_गन्तव्यम्_तालकम्)
        प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type, क्षेत्रम्_2,
                                ACTION_LATCH_गन्तव्यम्_तालकम्, array_ndx, मानम्,
                                &act->flags);
    यदि (type == ACTION_TYPE_गणः_तालकम् &&
        क्षेत्रम्_2 == ACTION_क्षेत्रम्_तालकम्_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        यदि (keymap_info->features.गणः_तालकम्_on_release) {
            ध्रुव गणना xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, क्षेत्रम्_2,
                ACTION_तालकम्_ON_RELEASE, array_ndx, मानम्, &act->flags
            );
            यदि (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_तालकम्_ON_RELEASE;
            प्रत्यागम ret;
        } अन्यथा {
            प्रत्यागम Report_Format_संस्करणम्_Mismatch(ctx, action->type, क्षेत्रम्_2,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    प्रत्यागम ReportIllegal(ctx, action->type, क्षेत्रम्_2, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
HandleMovePtr(ध्रुव संरचना xkb_keymap_info *keymap_info,
              ध्रुव संरचना xkb_mod_समुच्चयः *mods,
              सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
              ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
              व्यञ्जकम्_Def **मानम्_ptr)
{
    संरचना xkb_context * सीमित ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_pointer_motion_action *act = &action->ptr;

    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_X || क्षेत्रम्_2 == ACTION_क्षेत्रम्_Y) {
        int64_t val = 0;
        ध्रुव bool absolute = (मानम्->common.type != STMT_व्यञ्जकम्_NEGATE &&
                               मानम्->common.type != STMT_व्यञ्जकम्_UNARY_PLUS);

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, क्षेत्रम्_2,
                                        keymap_info->strict);

        यदि (!व्यञ्जकम्_Resolve_Integer(ctx, मानम्, &val))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, action->type,
                                  क्षेत्रम्_2, "integer", keymap_info->strict);

        यदि (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    क्षेत्रम्_Text(क्षेत्रम्_2), ActionTypeText(action->type),
                    INT16_MIN, INT16_MAX, val);
            प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_X) {
            यदि (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        अन्यथा {
            यदि (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_REPEAT) {
        प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type, क्षेत्रम्_2,
                                ACTION_REPEAT, array_ndx, मानम्, &act->flags);
    }

    प्रत्यागम ReportIllegal(ctx, action->type, क्षेत्रम्_2, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
HandlePtrBtn(ध्रुव संरचना xkb_keymap_info *keymap_info,
             ध्रुव संरचना xkb_mod_समुच्चयः *mods,
             सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
             ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
             व्यञ्जकम्_Def **मानम्_ptr)
{
    संरचना xkb_context * सीमित ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_pointer_button_action *act = &action->btn;

    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_BUTTON) {
        int64_t btn = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, क्षेत्रम्_2,
                                        keymap_info->strict);

        यदि (!व्यञ्जकम्_Resolve_Button(ctx, मानम्, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, action->type,
                                  क्षेत्रम्_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        यदि (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (action->type == ACTION_TYPE_PTR_तालकम् &&
             क्षेत्रम्_2 == ACTION_क्षेत्रम्_AFFECT) {
        प्रत्यागम Check_Affect_क्षेत्रम्(ctx, keymap_info->strict, action->type,
                                array_ndx, मानम्, &act->flags);
    }
    अन्यथा यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_गणनम्) {
        int64_t val = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, क्षेत्रम्_2,
                                        keymap_info->strict);

        यदि (!व्यञ्जकम्_Resolve_Integer(ctx, मानम्, &val))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, action->type,
                                  क्षेत्रम्_2, "integer", keymap_info->strict);

        यदि (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        यदि (action->type != ACTION_TYPE_PTR_तालकम्)
            act->गणनम् = (uint8_t) val;
        प्रत्यागम PARSER_SUCCESS;
    }

    प्रत्यागम ReportIllegal(ctx, action->type, क्षेत्रम्_2, keymap_info->strict);
}

गणना {
    समुच्चयः_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    समुच्चयः_POINTER_DEFAULT_BUTTON_AFFECT_अङ्कः_ENTRIES
};

स्थिर ध्रुव Lookup_प्रविष्टिः ptrDflts[] = {
    { "dfltbtn", समुच्चयः_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", समुच्चयः_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", समुच्चयः_POINTER_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { रिक्त, 0 }
};

स्थिर गणना xkb_parser_error
Handle_समुच्चयः_Ptr_Dflt(ध्रुव संरचना xkb_keymap_info *keymap_info,
                 ध्रुव संरचना xkb_mod_समुच्चयः *mods,
                 सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
                 ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
                 व्यञ्जकम्_Def **मानम्_ptr)
{
    संरचना xkb_context * सीमित ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_pointer_default_action *act = &action->dflt;

    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_AFFECT) {
        uint32_t val = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, क्षेत्रम्_2,
                                        keymap_info->strict);

        यदि (!व्यञ्जकम्_Resolve_Enum(ctx, मानम्, &val, ptrDflts))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, action->type,
                                  क्षेत्रम्_2, "pointer component", keymap_info->strict);
        static_assert(समुच्चयः_POINTER_DEFAULT_BUTTON_AFFECT_अङ्कः_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_BUTTON || क्षेत्रम्_2 == ACTION_क्षेत्रम्_मानम्) {
        ध्रुव व्यञ्जकम्_Def *button;
        int64_t btn = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, क्षेत्रम्_2,
                                        keymap_info->strict);

        यदि (मानम्->common.type == STMT_व्यञ्जकम्_NEGATE ||
            मानम्->common.type == STMT_व्यञ्जकम्_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = मानम्->unary.child;
        }
        अन्यथा {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = मानम्;
        }

        यदि (!व्यञ्जकम्_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_POINTER_BUTTON_MIN == 1 &&
                          XKB_POINTER_BUTTON_MAX == 5, "invalid message");
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, action->type,
                                  क्षेत्रम्_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        यदि (btn < XKB_POINTER_BUTTON_DEFAULT || btn > XKB_POINTER_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_POINTER_BUTTON_MIN, XKB_POINTER_BUTTON_MAX, btn);
            प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        यदि (btn == XKB_POINTER_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->मानम् = (int8_t) (मानम्->common.type == STMT_व्यञ्जकम्_NEGATE ? -btn: btn);
        प्रत्यागम PARSER_SUCCESS;
    }

    प्रत्यागम ReportIllegal(ctx, action->type, क्षेत्रम्_2, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
HandleSwitchScreen(ध्रुव संरचना xkb_keymap_info *keymap_info,
                   ध्रुव संरचना xkb_mod_समुच्चयः *mods,
                   सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
                   ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
                   व्यञ्जकम्_Def **मानम्_ptr)
{
    संरचना xkb_context * सीमित ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_switch_screen_action *act = &action->screen;

    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_SCREEN) {
        ध्रुव व्यञ्जकम्_Def *scrn;
        int64_t val = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, क्षेत्रम्_2,
                                        keymap_info->strict);

        यदि (मानम्->common.type == STMT_व्यञ्जकम्_NEGATE ||
            मानम्->common.type == STMT_व्यञ्जकम्_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = मानम्->unary.child;
        }
        अन्यथा {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = मानम्;
        }

        यदि (!व्यञ्जकम्_Resolve_Integer(ctx, scrn, &val))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, action->type,
                                  क्षेत्रम्_2, "integer (-128..127)",
                                  keymap_info->strict);

        val = (मानम्->common.type == STMT_व्यञ्जकम्_NEGATE ? -val : val);
        यदि (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_SAME) {
        प्रत्यागम CheckBooleanFlag(ctx, keymap_info->strict, action->type, क्षेत्रम्_2,
                                ACTION_SAME_SCREEN, array_ndx, मानम्,
                                &act->flags);
    }

    प्रत्यागम ReportIllegal(ctx, action->type, क्षेत्रम्_2, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
Handle_समुच्चयः_तालकम्_Controls(ध्रुव संरचना xkb_keymap_info *keymap_info,
                      ध्रुव संरचना xkb_mod_समुच्चयः *mods,
                      सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
                      ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
                      व्यञ्जकम्_Def **मानम्_ptr)
{
    संरचना xkb_context * सीमित ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_controls_action *act = &action->ctrls;

    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_CONTROLS) {
        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, क्षेत्रम्_2,
                                        keymap_info->strict);

        uint32_t आवरणम् = 0;
        ध्रुव uint8_t विस्थापनम् = keymap_info->features.controls_name_विस्थापनम्;
        यदि (!व्यञ्जकम्_Resolve_आवरणम्(ctx, मानम्, &आवरणम्, ctrl_आवरणम्_नामानि + विस्थापनम्))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, action->type,
                                  क्षेत्रम्_2, "controls mask", keymap_info->strict);

        act->ctrls = (गणना xkb_action_controls) आवरणम्;
        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_AFFECT &&
             action->type == ACTION_TYPE_CTRL_तालकम्) {
        प्रत्यागम Check_Affect_क्षेत्रम्(ctx, keymap_info->strict, action->type,
                                array_ndx, मानम्, &act->flags);
    }

    प्रत्यागम ReportIllegal(ctx, action->type, क्षेत्रम्_2, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
Handle_Redirect_कुञ्चिका(ध्रुव संरचना xkb_keymap_info *keymap_info,
                  ध्रुव संरचना xkb_mod_समुच्चयः *mods,
                  सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
                  ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
                  व्यञ्जकम्_Def **मानम्_ptr)
{
    ध्रुव संरचना xkb_keymap * सीमित ध्रुव keymap = &keymap_info->keymap;
    संरचना xkb_context * सीमित ध्रुव ctx = keymap->ctx;
    संरचना xkb_redirect_कुञ्चिका_action * ध्रुव act = &action->redirect;

    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_KEYCODE) {
        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, क्षेत्रम्_2,
                                        keymap_info->strict);
        यदि (मानम्->common.type == STMT_व्यञ्जकम्_IDENT) {
            ध्रुव वर्ण *valStr = xkb_atom_text(ctx, मानम्->ident.ident);
            यदि (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_कुञ्चिका_auto;
                प्रत्यागम PARSER_SUCCESS;
            }
        }
        यदि (मानम्->common.type != STMT_व्यञ्जकम्_KEYNAME_LITERAL) {
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, action->type,
                                  क्षेत्रम्_2, "key name", keymap_info->strict);
        }
        ध्रुव संरचना xkb_कुञ्चिका * ध्रुव कुञ्चिका =
            Xkb_कुञ्चिका_By_Name(keymap, मानम्->कुञ्चिका_name.कुञ्चिका_name, true);
        यदि (कुञ्चिका == रिक्त) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    क्षेत्रम्_Text(क्षेत्रम्_2), कुञ्चिका_Name_Text(ctx, मानम्->कुञ्चिका_name.कुञ्चिका_name));
            प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_मानम्_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = कुञ्चिका->keycode;
        प्रत्यागम PARSER_SUCCESS;
    }

    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_MODIFIERS || क्षेत्रम्_2 == ACTION_क्षेत्रम्_MODS_गन्तव्यम्_CLEAR) {
        गणना xkb_action_flags flags = 0;
        xkb_mod_आवरणम्_t m = 0;
        गणना xkb_parser_error r =
            Check_Modifier_क्षेत्रम्(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, मानम्, &flags, &m);
        यदि (r != PARSER_SUCCESS)
            प्रत्यागम r;
        यदि (flags)
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, action->type,
                                  क्षेत्रम्_2, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_MODIFIERS)
            act->mods |= m;
        अन्यथा
            act->mods &= ~m;
        प्रत्यागम PARSER_SUCCESS;
    }

    प्रत्यागम ReportIllegal(ctx, action->type, क्षेत्रम्_2, keymap_info->strict);
}

स्थिर गणना xkb_parser_error
HandleUnsupported(ध्रुव संरचना xkb_keymap_info *keymap_info,
                        ध्रुव संरचना xkb_mod_समुच्चयः *mods,
                        सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
                        ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
                        व्यञ्जकम्_Def **मानम्_ptr)

{
    /* Do not check any field */
    प्रत्यागम PARSER_SUCCESS;
}

स्थिर गणना xkb_parser_error
Handle_वैयक्तिकम्(ध्रुव संरचना xkb_keymap_info *keymap_info,
              ध्रुव संरचना xkb_mod_समुच्चयः *mods,
              सङ्घ xkb_action *action, गणना action_क्षेत्रम् क्षेत्रम्_2,
              ध्रुव व्यञ्जकम्_Def *array_ndx, ध्रुव व्यञ्जकम्_Def *मानम्,
              व्यञ्जकम्_Def **मानम्_ptr)
{
    संरचना xkb_context * सीमित ध्रुव ctx = keymap_info->keymap.ctx;
    संरचना xkb_वैयक्तिकम्_action *act = &action->priv;

    यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_TYPE) {
        int64_t type = 0;

        यदि (array_ndx)
            प्रत्यागम ReportActionNotArray(ctx, action->type, क्षेत्रम्_2,
                                        keymap_info->strict);

        यदि (!व्यञ्जकम्_Resolve_Integer(ctx, मानम्, &type))
            प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                                  ACTION_TYPE_वैयक्तिकम्, क्षेत्रम्_2, "integer",
                                  keymap_info->strict);

        यदि (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
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
        यदि (type < ACTION_TYPE_वैयक्तिकम्) {
            log_info(ctx, XKB_LOG_सन्देशः_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     ActionTypeText((गणना xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        अन्यथा {
            act->type = (गणना xkb_action_type) type;
        }

        प्रत्यागम PARSER_SUCCESS;
    }
    अन्यथा यदि (क्षेत्रम्_2 == ACTION_क्षेत्रम्_दत्त) {
        यदि (array_ndx == रिक्त) {
            xkb_atom_t val = XKB_ATOM_NONE;

            यदि (!व्यञ्जकम्_Resolve_String(ctx, मानम्, &val))
                प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                                      action->type, क्षेत्रम्_2, "string",
                                      keymap_info->strict);

            ध्रुव वर्ण *str = xkb_atom_text(ctx, val);
            आकार_t len = वाक्यदैर्घ्यम्(str);
            यदि (len < 1 || len > परिमाण(act->दत्त)) {
                log_warn(ctx, XKB_LOG_सन्देशः_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", परिमाण(act->दत्त), len);
                प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            स्मृतिं_पूरय(act->दत्त, 0, परिमाण(act->दत्त));
            स्मृतिं_प्रतिलिख(act->दत्त, str, len);
            प्रत्यागम PARSER_SUCCESS;
        }
        अन्यथा {
            int64_t ndx = 0, datum = 0;

            यदि (!व्यञ्जकम्_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            यदि (ndx < 0 || (आकार_t) ndx >= परिमाण(act->दत्त)) {
                log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        परिमाण(act->दत्त), ndx);
                प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            यदि (!व्यञ्जकम्_Resolve_Integer(ctx, मानम्, &datum))
                प्रत्यागम ReportMismatch(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, act->type,
                                      क्षेत्रम्_2, "integer", keymap_info->strict);

            यदि (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                प्रत्यागम (keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->दत्त[ndx] = (uint8_t) datum;
            प्रत्यागम PARSER_SUCCESS;
        }
    }

    प्रत्यागम ReportIllegal(ctx, action->type, क्षेत्रम्_2, keymap_info->strict);
}

प्रकारनिर्देश गणना xkb_parser_error
        (*actionHandler)(ध्रुव संरचना xkb_keymap_info *keymap_info,
                         ध्रुव संरचना xkb_mod_समुच्चयः *mods,
                         सङ्घ xkb_action *action,
                         गणना action_क्षेत्रम् क्षेत्रम्_2,
                         ध्रुव व्यञ्जकम्_Def *array_ndx,
                         ध्रुव व्यञ्जकम्_Def *मानम्,
                         व्यञ्जकम्_Def **मानम्_ptr);

स्थिर ध्रुव actionHandler handleAction[ACTION_TYPE_अङ्कः_ENTRIES] = {
    [ACTION_TYPE_NONE] = HandleNoAction,
    [ACTION_TYPE_VOID] = HandleNoAction,
    [ACTION_TYPE_MOD_समुच्चयः] = Handle_समुच्चयः_Latch_तालकम्_Mods,
    [ACTION_TYPE_MOD_LATCH] = Handle_समुच्चयः_Latch_तालकम्_Mods,
    [ACTION_TYPE_MOD_तालकम्] = Handle_समुच्चयः_Latch_तालकम्_Mods,
    [ACTION_TYPE_गणः_समुच्चयः] = Handle_समुच्चयः_Latch_तालकम्_गणः,
    [ACTION_TYPE_गणः_LATCH] = Handle_समुच्चयः_Latch_तालकम्_गणः,
    [ACTION_TYPE_गणः_तालकम्] = Handle_समुच्चयः_Latch_तालकम्_गणः,
    [ACTION_TYPE_PTR_MOVE] = HandleMovePtr,
    [ACTION_TYPE_PTR_BUTTON] = HandlePtrBtn,
    [ACTION_TYPE_PTR_तालकम्] = HandlePtrBtn,
    [ACTION_TYPE_PTR_DEFAULT] = Handle_समुच्चयः_Ptr_Dflt,
    [ACTION_TYPE_TERMINATE] = HandleNoAction,
    [ACTION_TYPE_SWITCH_VT] = HandleSwitchScreen,
    [ACTION_TYPE_CTRL_समुच्चयः] = Handle_समुच्चयः_तालकम्_Controls,
    [ACTION_TYPE_CTRL_तालकम्] = Handle_समुच्चयः_तालकम्_Controls,
    [ACTION_TYPE_REDIRECT_कुञ्चिका] = Handle_Redirect_कुञ्चिका,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_वैयक्तिकम्] = Handle_वैयक्तिकम्,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_आन्तरिकम् == 20 &&
              ACTION_TYPE_आन्तरिकम् + 1 == ACTION_TYPE_अङ्कः_ENTRIES,
              "Missing action type");

/***====================================================================***/

गणना xkb_parser_error
HandleActionDef(ध्रुव संरचना xkb_keymap_info *keymap_info, ActionsInfo *info,
                ध्रुव संरचना xkb_mod_समुच्चयः *mods, व्यञ्जकम्_Def *def,
                सङ्घ xkb_action *action)
{
    संरचना xkb_context * सीमित ध्रुव ctx = keymap_info->keymap.ctx;
    यदि (def->common.type != STMT_व्यञ्जकम्_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_गन्तव्यम्_string(def->common.type));
        प्रत्यागम PARSER_FATAL_ERROR;
    }

    ध्रुव वर्ण *action_name = xkb_atom_text(ctx, def->action.name);
    गणना xkb_action_type handler_type;
    यदि (!string_गन्तव्यम्_Action_Type(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        यदि (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            प्रत्यागम PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    यदि (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    गणना xkb_parser_error ret = PARSER_SUCCESS;
    प्रति (व्यञ्जकम्_Def *arg = def->action.तर्काः; arg != रिक्त;
         arg = (व्यञ्जकम्_Def *) arg->common.अग्र) {
        ध्रुव व्यञ्जकम्_Def *मानम्;
        व्यञ्जकम्_Def **मानम्_ptr = रिक्त;
        व्यञ्जकम्_Def *क्षेत्रम्_2, *arrayRtrn;
        ध्रुव वर्ण *elemRtrn, *क्षेत्रम्_Rtrn;

        यदि (arg->common.type == STMT_व्यञ्जकम्_नियोजय) {
            क्षेत्रम्_2 = arg->द्विमान.वामः;
            मानम् = arg->द्विमान.दक्षिणः;
            मानम्_ptr = &arg->द्विमान.दक्षिणः;
        }
        अन्यथा यदि (arg->common.type == STMT_व्यञ्जकम्_NOT ||
                 arg->common.type == STMT_व्यञ्जकम्_INVERT) {
            क्षेत्रम्_2 = arg->unary.child;
            मानम् = (ध्रुव व्यञ्जकम्_Def *) &constFalse;
        }
        अन्यथा {
            क्षेत्रम्_2 = arg;
            मानम् = (ध्रुव व्यञ्जकम्_Def *) &constTrue;
        }

        यदि (!व्यञ्जकम्_Resolve_Lhs(ctx, क्षेत्रम्_2, &elemRtrn, &क्षेत्रम्_Rtrn, &arrayRtrn))
            प्रत्यागम PARSER_FATAL_ERROR;

        यदि (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, क्षेत्रम्_Rtrn);
            प्रत्यागम PARSER_FATAL_ERROR;
        }

        गणना action_क्षेत्रम् क्षेत्रम्_Ndx;
        यदि (!string_गन्तव्यम्_क्षेत्रम्(क्षेत्रम्_Rtrn, &क्षेत्रम्_Ndx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_क्षेत्रम्,
                    "Unknown field name %s for action %s discarded\n",
                    क्षेत्रम्_Rtrn, ActionTypeText(action->type));
            यदि (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                प्रत्यागम PARSER_FATAL_ERROR;
            } अन्यथा {
                /* Field is just ignored */
                अनुवर्त;
            }
        }

        विकल्प (handleAction[handler_type](keymap_info, mods, action, क्षेत्रम्_Ndx,
                                           arrayRtrn, मानम्, मानम्_ptr)) {
        पक्ष PARSER_FATAL_ERROR:
            प्रत्यागम PARSER_FATAL_ERROR;
        पक्ष PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            भङ्ग;
        स्वभाव:
            ;
        }
    }

    प्रत्यागम (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

गणना xkb_parser_error
समुच्चयः_Default_Action_क्षेत्रम्(ध्रुव संरचना xkb_keymap_info *keymap_info,
                      ActionsInfo *info, संरचना xkb_mod_समुच्चयः *mods,
                      ध्रुव वर्ण *elem, ध्रुव वर्ण *क्षेत्रम्_2, व्यञ्जकम्_Def *array_ndx,
                      व्यञ्जकम्_Def **मानम्_ptr, गणना merge_बहुलकम् merge)
{
    ध्रुव व्यञ्जकम्_Def * सीमित ध्रुव मानम् = *मानम्_ptr;
    गणना xkb_action_type action;
    यदि (!string_गन्तव्यम्_Action_Type(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        प्रत्यागम (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    गणना action_क्षेत्रम् action_क्षेत्रम्;
    यदि (!string_गन्तव्यम्_क्षेत्रम्(क्षेत्रम्_2, &action_क्षेत्रम्)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_क्षेत्रम्,
                "Unknown action field \"%s\"\n", क्षेत्रम्_2);
        प्रत्यागम (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    सङ्घ xkb_action* ध्रुव into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    सङ्घ xkb_action from = *into;

    /* Parse action */
    ध्रुव गणना xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_क्षेत्रम्, array_ndx, मानम्, मानम्_ptr
    );

    /* Success is mandatory to update defaults */
    यदि (ret != PARSER_SUCCESS)
        प्रत्यागम ret;

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
    यदि (!action_equal(into, &from)) {
        ध्रुव bool replace = (merge != MERGE_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_सन्देशः_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                क्षेत्रम्_Text(action_क्षेत्रम्), ActionTypeText(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        यदि (replace)
            *into = from;
    }
    प्रत्यागम PARSER_SUCCESS;
}
