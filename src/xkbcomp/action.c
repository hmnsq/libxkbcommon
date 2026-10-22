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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast.h"
#ಅನ್ತರ್ಭಾವ "action.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Boolean constTrue = {
    .common = { .type = STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL, .ಮುಂದಿನ = ಖಾಲಿ },
    .ಗಣ = true,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Boolean constFalse = {
    .common = { .type = STMT_ಅಭಿವ್ಯಕ್ತಿ_BOOLEAN_LITERAL, .ಮುಂದಿನ = ಖಾಲಿ },
    .ಗಣ = false,
};

ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ {
    ACTION_ಕ್ಷೇತ್ರ_CLEAR_LOCKS,
    ACTION_ಕ್ಷೇತ್ರ_LATCH_ಗಮ್ಯ_ಬೀಗ,
    ACTION_ಕ್ಷೇತ್ರ_GEN_ಕೀಲಿ_EVENT,
    ACTION_ಕ್ಷೇತ್ರ_REPORT,
    ACTION_ಕ್ಷೇತ್ರ_DEFAULT,
    ACTION_ಕ್ಷೇತ್ರ_AFFECT,
    ACTION_ಕ್ಷೇತ್ರ_INCREMENT,
    ACTION_ಕ್ಷೇತ್ರ_MODIFIERS,
    ACTION_ಕ್ಷೇತ್ರ_ಗುಂಪು,
    ACTION_ಕ್ಷೇತ್ರ_X,
    ACTION_ಕ್ಷೇತ್ರ_Y,
    ACTION_ಕ್ಷೇತ್ರ_REPEAT,
    ACTION_ಕ್ಷೇತ್ರ_BUTTON,
    ACTION_ಕ್ಷೇತ್ರ_ಮೌಲ್ಯ,
    ACTION_ಕ್ಷೇತ್ರ_CONTROLS,
    ACTION_ಕ್ಷೇತ್ರ_TYPE,
    ACTION_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ,
    ACTION_ಕ್ಷೇತ್ರ_SCREEN,
    ACTION_ಕ್ಷೇತ್ರ_SAME,
    ACTION_ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ,
    ACTION_ಕ್ಷೇತ್ರ_DEVICE,
    ACTION_ಕ್ಷೇತ್ರ_KEYCODE,
    ACTION_ಕ್ಷೇತ್ರ_MODS_ಗಮ್ಯ_CLEAR,
    ACTION_ಕ್ಷೇತ್ರ_ಬೀಗ_ON_RELEASE,
    ACTION_ಕ್ಷೇತ್ರ_UNLOCK_ON_PRESS,
    ACTION_ಕ್ಷೇತ್ರ_LATCH_ON_PRESS,
};

ಶೂನ್ಯ
InitActionsInfo(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, ActionsInfo *info)
{
    ಗಾಗಿ (ಎಣಿಕೆ xkb_action_type type = 0; type < ACTION_TYPE_ಅಂಕೆ_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.ಮೌಲ್ಯ = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_ಕೀಲಿ].redirect.keycode =
        keymap->redirect_ಕೀಲಿ_auto;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು ಕ್ಷೇತ್ರ_ಅಕ್ಷರಸರಣಿಗಳು[] = {
    { "clearLocks",       ACTION_ಕ್ಷೇತ್ರ_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_ಕ್ಷೇತ್ರ_LATCH_ಗಮ್ಯ_ಬೀಗ   },
    { "genKeyEvent",      ACTION_ಕ್ಷೇತ್ರ_GEN_ಕೀಲಿ_EVENT   },
    { "generateKeyEvent", ACTION_ಕ್ಷೇತ್ರ_GEN_ಕೀಲಿ_EVENT   },
    { "report",           ACTION_ಕ್ಷೇತ್ರ_REPORT          },
    { "default",          ACTION_ಕ್ಷೇತ್ರ_DEFAULT         },
    { "affect",           ACTION_ಕ್ಷೇತ್ರ_AFFECT          },
    { "increment",        ACTION_ಕ್ಷೇತ್ರ_INCREMENT       },
    { "modifiers",        ACTION_ಕ್ಷೇತ್ರ_MODIFIERS       },
    { "mods",             ACTION_ಕ್ಷೇತ್ರ_MODIFIERS       },
    { "group",            ACTION_ಕ್ಷೇತ್ರ_ಗುಂಪು           },
    { "x",                ACTION_ಕ್ಷೇತ್ರ_X               },
    { "y",                ACTION_ಕ್ಷೇತ್ರ_Y               },
    { "repeat",           ACTION_ಕ್ಷೇತ್ರ_REPEAT          },
    { "accel",            ACTION_ಕ್ಷೇತ್ರ_REPEAT          },
    { "accelerate",       ACTION_ಕ್ಷೇತ್ರ_REPEAT          },
    { "button",           ACTION_ಕ್ಷೇತ್ರ_BUTTON          },
    { "value",            ACTION_ಕ್ಷೇತ್ರ_ಮೌಲ್ಯ           },
    { "controls",         ACTION_ಕ್ಷೇತ್ರ_CONTROLS        },
    { "ctrls",            ACTION_ಕ್ಷೇತ್ರ_CONTROLS        },
    { "type",             ACTION_ಕ್ಷೇತ್ರ_TYPE            },
    { "count",            ACTION_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ           },
    { "screen",           ACTION_ಕ್ಷೇತ್ರ_SCREEN          },
    { "same",             ACTION_ಕ್ಷೇತ್ರ_SAME            },
    { "sameServer",       ACTION_ಕ್ಷೇತ್ರ_SAME            },
    { "data",             ACTION_ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ            },
    { "device",           ACTION_ಕ್ಷೇತ್ರ_DEVICE          },
    { "dev",              ACTION_ಕ್ಷೇತ್ರ_DEVICE          },
    { "key",              ACTION_ಕ್ಷೇತ್ರ_KEYCODE         },
    { "keycode",          ACTION_ಕ್ಷೇತ್ರ_KEYCODE         },
    { "kc",               ACTION_ಕ್ಷೇತ್ರ_KEYCODE         },
    { "clearmods",        ACTION_ಕ್ಷೇತ್ರ_MODS_ಗಮ್ಯ_CLEAR   },
    { "clearmodifiers",   ACTION_ಕ್ಷೇತ್ರ_MODS_ಗಮ್ಯ_CLEAR   },
    { "lockOnRelease",    ACTION_ಕ್ಷೇತ್ರ_ಬೀಗ_ON_RELEASE },
    { "unlockOnPress",    ACTION_ಕ್ಷೇತ್ರ_UNLOCK_ON_PRESS },
    { "latchOnPress",     ACTION_ಕ್ಷೇತ್ರ_LATCH_ON_PRESS  },
    { ಖಾಲಿ,               0                            }
};

ಸ್ಥಿರ bool
string_ಗಮ್ಯ_Action_Type(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಎಣಿಕೆ xkb_action_type *type_rtrn)
{
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ type = 0;
    ಸ್ಥಿರಾಂಕ bool ret = LookupString(action_Type_ಹೆಸರುಗಳು, str, &type);
    *type_rtrn = (ಎಣಿಕೆ xkb_action_type) type;
    ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ bool
string_ಗಮ್ಯ_ಕ್ಷೇತ್ರ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ *ಕ್ಷೇತ್ರ_rtrn)
{
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಕ್ಷೇತ್ರ_2 = 0;
    ಸ್ಥಿರಾಂಕ bool ret = LookupString(ಕ್ಷೇತ್ರ_ಅಕ್ಷರಸರಣಿಗಳು, str, &ಕ್ಷೇತ್ರ_2);
    *ಕ್ಷೇತ್ರ_rtrn = (ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ) ಕ್ಷೇತ್ರ_2;
    ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಕ್ಷೇತ್ರ_ಪಠ್ಯ(ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2)
{
    ಹಿಂತಿರುಗಿ Lookup_ಮೌಲ್ಯ(ಕ್ಷೇತ್ರ_ಅಕ್ಷರಸರಣಿಗಳು, ಕ್ಷೇತ್ರ_2);
}

/***====================================================================***/

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ xkb_parser_error
ReportMismatch(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_ಸಂದೇಶ_ಸಂಕೇತ ಸಂಕೇತ_2,
               ಎಣಿಕೆ xkb_action_type action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
               ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *type, ಎಣಿಕೆ xkb_parser_strict_flags strict)
{
    log_err(ctx, ಸಂಕೇತ_2,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            ಕ್ಷೇತ್ರ_ಪಠ್ಯ(ಕ್ಷೇತ್ರ_2), type, Action_Type_ಪಠ್ಯ(action));
    ಹಿಂತಿರುಗಿ (strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ xkb_parser_error
Report_Format_ಆವೃತ್ತಿ_Mismatch(ರಚನೆ xkb_context *ctx,
                            ಎಣಿಕೆ xkb_action_type action,
                            ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                            ಎಣಿಕೆ xkb_keymap_format format,
                            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *versions,
                            ಎಣಿಕೆ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ಪಠ್ಯ_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            ಕ್ಷೇತ್ರ_ಪಠ್ಯ(ಕ್ಷೇತ್ರ_2), Action_Type_ಪಠ್ಯ(action), versions, format);
    ಹಿಂತಿರುಗಿ (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ xkb_parser_error
ReportIllegal(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_action_type action,
              ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2, ಎಣಿಕೆ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_ಕ್ಷೇತ್ರ,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            ಕ್ಷೇತ್ರ_ಪಠ್ಯ(ಕ್ಷೇತ್ರ_2), Action_Type_ಪಠ್ಯ(action));
    ಹಿಂತಿರುಗಿ (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ xkb_parser_error
ReportActionNotArray(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_action_type action,
                     ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                     ಎಣಿಕೆ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            ಕ್ಷೇತ್ರ_ಪಠ್ಯ(ಕ್ಷೇತ್ರ_2), Action_Type_ಪಠ್ಯ(action));
    ಹಿಂತಿರುಗಿ (strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
HandleNoAction(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
               ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
               ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
               ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
               ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_ಕ್ಷೇತ್ರ,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            Action_Type_ಪಠ್ಯ(action->type), ಕ್ಷೇತ್ರ_ಪಠ್ಯ(ಕ್ಷೇತ್ರ_2));
    ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
CheckBooleanFlag(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_parser_strict_flags strict,
                 ಎಣಿಕೆ xkb_action_type action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                 ಎಣಿಕೆ xkb_action_flags flag, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                 ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ, ಎಣಿಕೆ xkb_action_flags *flags_inout)
{
    bool ಗಣ = false;

    ಆದರೆ (array_ndx)
        ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action, ಕ್ಷೇತ್ರ_2, strict);

    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(ctx, ಮೌಲ್ಯ, &ಗಣ))
        ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                              action, ಕ್ಷೇತ್ರ_2, "boolean", strict);

    ಆದರೆ (ಗಣ)
        *flags_inout |= flag;
    ಇಲ್ಲದಿದ್ದರೆ
        *flags_inout &= ~flag;

    ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
Check_Modifier_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_parser_strict_flags strict,
                   ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods, ಎಣಿಕೆ xkb_action_type action,
                   ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                   ಎಣಿಕೆ xkb_action_flags *flags_inout, xkb_mod_ಮುಖವಾಡ_t *mods_rtrn)
{
    ಆದರೆ (array_ndx)
        ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action, ACTION_ಕ್ಷೇತ್ರ_MODIFIERS, strict);

    ಆದರೆ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *valStr;
        valStr = xkb_atom_ಪಠ್ಯ(ctx, ಮೌಲ್ಯ->ident.ident);
        ಆದರೆ (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
        }
    }

    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(ctx, ಮೌಲ್ಯ, MOD_BOTH, mods, mods_rtrn))
        ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, action,
                              ACTION_ಕ್ಷೇತ್ರ_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು ಬೀಗ_Which[] = {
    { "both", 0 },
    { "lock", ACTION_ಬೀಗ_NO_UNLOCK },
    { "neither", (ACTION_ಬೀಗ_NO_ಬೀಗ | ACTION_ಬೀಗ_NO_UNLOCK) },
    { "unlock", ACTION_ಬೀಗ_NO_ಬೀಗ },
    { ಖಾಲಿ, 0 }
};

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
Check_Affect_ಕ್ಷೇತ್ರ(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_parser_strict_flags strict,
                 ಎಣಿಕೆ xkb_action_type action, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                 ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ, ಎಣಿಕೆ xkb_action_flags *flags_inout)
{
    ಆದರೆ (array_ndx)
        ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action, ACTION_ಕ್ಷೇತ್ರ_AFFECT, strict);

    uint32_t flags = 0;
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(ctx, ಮೌಲ್ಯ, &flags, ಬೀಗ_Which))
        ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                              action, ACTION_ಕ್ಷೇತ್ರ_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_ಬೀಗ_NO_ಬೀಗ | ACTION_ಬೀಗ_NO_UNLOCK);
    *flags_inout |= (ಎಣಿಕೆ xkb_action_flags) flags;
    ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
Handle_ಗಣ_Latch_ಬೀಗ_Mods(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                       ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
                       ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                       ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                       ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ರಚನೆ xkb_context * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ctx = keymap_info->keymap.ctx;
    ರಚನೆ xkb_mod_action *act = &action->mods;
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_action_type type = action->type;

    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_MODIFIERS)
        ಹಿಂತಿರುಗಿ Check_Modifier_ಕ್ಷೇತ್ರ(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, ಮೌಲ್ಯ, &act->flags, &act->mods.mods);
    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        ಪ್ರತಿಜ್ಞಾ(type == ACTION_TYPE_MOD_ಗಣ || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_ಬೀಗ);
        ಆದರೆ (keymap_info->features.mods_unlock_on_press) {
            ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, ಕ್ಷೇತ್ರ_2,
                ACTION_UNLOCK_ON_PRESS, array_ndx, ಮೌಲ್ಯ, &act->flags
            );
            ಆದರೆ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            ಹಿಂತಿರುಗಿ ret;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಹಿಂತಿರುಗಿ Report_Format_ಆವೃತ್ತಿ_Mismatch(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    ಆದರೆ ((type == ACTION_TYPE_MOD_ಗಣ || type == ACTION_TYPE_MOD_LATCH) &&
        ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_CLEAR_LOCKS)
        ಹಿಂತಿರುಗಿ CheckBooleanFlag(ctx, keymap_info->strict, action->type, ಕ್ಷೇತ್ರ_2,
                                ACTION_ಬೀಗ_CLEAR, array_ndx, ಮೌಲ್ಯ,
                                &act->flags);
    ಆದರೆ (type == ACTION_TYPE_MOD_LATCH) {
        ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_LATCH_ಗಮ್ಯ_ಬೀಗ)
            ಹಿಂತಿರುಗಿ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    ಕ್ಷೇತ್ರ_2, ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ, array_ndx,
                                    ಮೌಲ್ಯ, &act->flags);
        ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_LATCH_ON_PRESS) {
            ಆದರೆ (keymap_info->features.mods_latch_on_press) {
                ಹಿಂತಿರುಗಿ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        ಕ್ಷೇತ್ರ_2, ACTION_LATCH_ON_PRESS, array_ndx,
                                        ಮೌಲ್ಯ, &act->flags);
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಹಿಂತಿರುಗಿ Report_Format_ಆವೃತ್ತಿ_Mismatch(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    ಆದರೆ (type == ACTION_TYPE_MOD_ಬೀಗ && ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_AFFECT)
        ಹಿಂತಿರುಗಿ Check_Affect_ಕ್ಷೇತ್ರ(ctx, keymap_info->strict, action->type,
                                array_ndx, ಮೌಲ್ಯ, &act->flags);

    ಹಿಂತಿರುಗಿ ReportIllegal(ctx, action->type, ಕ್ಷೇತ್ರ_2, keymap_info->strict);
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
Check_ಗುಂಪು_ಕ್ಷೇತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                ಎಣಿಕೆ xkb_action_type action, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ, ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr,
                ಎಣಿಕೆ xkb_action_flags *flags_inout, int32_t *ಗುಂಪು_rtrn)
{
    ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *spec;
    xkb_layout_ಸೂಚ್ಯಂಕ_t idx = 0;
    ಎಣಿಕೆ xkb_action_flags flags = *flags_inout;

    ಆದರೆ (array_ndx)
        ಹಿಂತಿರುಗಿ ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_ಕ್ಷೇತ್ರ_ಗುಂಪು, keymap_info->strict);

    ಆದರೆ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ||
        ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = ಮೌಲ್ಯ->unary.ಮಗು;
        ಮೌಲ್ಯ_ptr = &(*ಮೌಲ್ಯ_ptr)->unary.ಮಗು;
    } ಇಲ್ಲದಿದ್ದರೆ {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = ಮೌಲ್ಯ;
    }

    ಸ್ಥಿರಾಂಕ bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_parser_error ret =
        ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು(keymap_info, spec, absolute, &idx, &pending);
    ಆದರೆ (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2, action,
                       ACTION_ಕ್ಷೇತ್ರ_ಗುಂಪು, "integer", keymap_info->strict);
        ಹಿಂತಿರುಗಿ ret;
    }

    ಆದರೆ (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t pending_ಸೂಚ್ಯಂಕ =
            darray_ಗಾತ್ರ(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (ರಚನೆ pending_computation) {
                .ಅಭಿವ್ಯಕ್ತಿ = *ಮೌಲ್ಯ_ptr,
                .computed = false,
                .ಮೌಲ್ಯ = 0,
            }
        );
        *ಮೌಲ್ಯ_ptr = ಖಾಲಿ;
        static_assert(ಗಾತ್ರ(pending_ಸೂಚ್ಯಂಕ) == ಗಾತ್ರ(*ಗುಂಪು_rtrn),
                      "Cannot save pending computation");
        *ಗುಂಪು_rtrn = (int32_t) pending_ಸೂಚ್ಯಂಕ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        ಆದರೆ (absolute) {
            *ಗುಂಪು_rtrn = (int32_t) (idx - 1);
        } ಇಲ್ಲದಿದ್ದರೆ {
            *ಗುಂಪು_rtrn = (int32_t) idx;
            ಆದರೆ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE)
                *ಗುಂಪು_rtrn = -*ಗುಂಪು_rtrn;
        }
    }

    *flags_inout = flags;
    ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
Handle_ಗಣ_Latch_ಬೀಗ_ಗುಂಪು(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
                        ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                        ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                        ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ರಚನೆ xkb_context * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ctx = keymap_info->keymap.ctx;
    ರಚನೆ xkb_ಗುಂಪು_action *act = &action->ಗುಂಪು;
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_action_type type = action->type;

    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_ಗುಂಪು) {
        ಹಿಂತಿರುಗಿ Check_ಗುಂಪು_ಕ್ಷೇತ್ರ(keymap_info, action->type, array_ndx, ಮೌಲ್ಯ,
                               ಮೌಲ್ಯ_ptr, &act->flags, &act->ಗುಂಪು);
    }
    ಆದರೆ ((type == ACTION_TYPE_ಗುಂಪು_ಗಣ || type == ACTION_TYPE_ಗುಂಪು_LATCH) &&
        ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_CLEAR_LOCKS)
        ಹಿಂತಿರುಗಿ CheckBooleanFlag(ctx, keymap_info->strict, action->type, ಕ್ಷೇತ್ರ_2,
                                ACTION_ಬೀಗ_CLEAR, array_ndx, ಮೌಲ್ಯ,
                                &act->flags);
    ಆದರೆ (type == ACTION_TYPE_ಗುಂಪು_LATCH &&
        ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_LATCH_ಗಮ್ಯ_ಬೀಗ)
        ಹಿಂತಿರುಗಿ CheckBooleanFlag(ctx, keymap_info->strict, action->type, ಕ್ಷೇತ್ರ_2,
                                ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ, array_ndx, ಮೌಲ್ಯ,
                                &act->flags);
    ಆದರೆ (type == ACTION_TYPE_ಗುಂಪು_ಬೀಗ &&
        ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_ಬೀಗ_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        ಆದರೆ (keymap_info->features.ಗುಂಪು_ಬೀಗ_on_release) {
            ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, ಕ್ಷೇತ್ರ_2,
                ACTION_ಬೀಗ_ON_RELEASE, array_ndx, ಮೌಲ್ಯ, &act->flags
            );
            ಆದರೆ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_ಬೀಗ_ON_RELEASE;
            ಹಿಂತಿರುಗಿ ret;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಹಿಂತಿರುಗಿ Report_Format_ಆವೃತ್ತಿ_Mismatch(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    ಹಿಂತಿರುಗಿ ReportIllegal(ctx, action->type, ಕ್ಷೇತ್ರ_2, keymap_info->strict);
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
HandleMovePtr(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
              ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
              ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
              ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
              ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ರಚನೆ xkb_context * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ctx = keymap_info->keymap.ctx;
    ರಚನೆ xkb_ಸೂಚಕ_motion_action *act = &action->ptr;

    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_X || ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_Y) {
        int64_t val = 0;
        ಸ್ಥಿರಾಂಕ bool absolute = (ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE &&
                               ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS);

        ಆದರೆ (array_ndx)
            ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                        keymap_info->strict);

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, ಮೌಲ್ಯ, &val))
            ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, action->type,
                                  ಕ್ಷೇತ್ರ_2, "integer", keymap_info->strict);

        ಆದರೆ (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    ಕ್ಷೇತ್ರ_ಪಠ್ಯ(ಕ್ಷೇತ್ರ_2), Action_Type_ಪಠ್ಯ(action->type),
                    INT16_MIN, INT16_MAX, val);
            ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_X) {
            ಆದರೆ (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಆದರೆ (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_REPEAT) {
        ಹಿಂತಿರುಗಿ CheckBooleanFlag(ctx, keymap_info->strict, action->type, ಕ್ಷೇತ್ರ_2,
                                ACTION_REPEAT, array_ndx, ಮೌಲ್ಯ, &act->flags);
    }

    ಹಿಂತಿರುಗಿ ReportIllegal(ctx, action->type, ಕ್ಷೇತ್ರ_2, keymap_info->strict);
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
HandlePtrBtn(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
             ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
             ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
             ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
             ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ರಚನೆ xkb_context * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ctx = keymap_info->keymap.ctx;
    ರಚನೆ xkb_ಸೂಚಕ_button_action *act = &action->btn;

    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_BUTTON) {
        int64_t btn = 0;

        ಆದರೆ (array_ndx)
            ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                        keymap_info->strict);

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Button(ctx, ಮೌಲ್ಯ, &btn)) {
            static_assert(XKB_ಸೂಚಕ_BUTTON_MIN == 1 &&
                          XKB_ಸೂಚಕ_BUTTON_MAX == 5, "invalid message");
            ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, action->type,
                                  ಕ್ಷೇತ್ರ_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_ಸೂಚಕ_BUTTON_DEFAULT < XKB_ಸೂಚಕ_BUTTON_MIN, "");
        ಆದರೆ (btn < XKB_ಸೂಚಕ_BUTTON_DEFAULT || btn > XKB_ಸೂಚಕ_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_ಸೂಚಕ_BUTTON_MIN, XKB_ಸೂಚಕ_BUTTON_MAX, btn);
            ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (action->type == ACTION_TYPE_PTR_ಬೀಗ &&
             ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_AFFECT) {
        ಹಿಂತಿರುಗಿ Check_Affect_ಕ್ಷೇತ್ರ(ctx, keymap_info->strict, action->type,
                                array_ndx, ಮೌಲ್ಯ, &act->flags);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_ಎಣಿಕೆ) {
        int64_t val = 0;

        ಆದರೆ (array_ndx)
            ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                        keymap_info->strict);

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, ಮೌಲ್ಯ, &val))
            ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, action->type,
                                  ಕ್ಷೇತ್ರ_2, "integer", keymap_info->strict);

        ಆದರೆ (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        ಆದರೆ (action->type != ACTION_TYPE_PTR_ಬೀಗ)
            act->ಎಣಿಕೆ_2 = (uint8_t) val;
        ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
    }

    ಹಿಂತಿರುಗಿ ReportIllegal(ctx, action->type, ಕ್ಷೇತ್ರ_2, keymap_info->strict);
}

ಎಣಿಕೆ {
    ಗಣ_ಸೂಚಕ_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    ಗಣ_ಸೂಚಕ_DEFAULT_BUTTON_AFFECT_ಅಂಕೆ_ENTRIES
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು ptrDflts[] = {
    { "dfltbtn", ಗಣ_ಸೂಚಕ_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", ಗಣ_ಸೂಚಕ_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", ಗಣ_ಸೂಚಕ_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { ಖಾಲಿ, 0 }
};

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
Handle_ಗಣ_Ptr_Dflt(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                 ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
                 ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                 ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                 ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ರಚನೆ xkb_context * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ctx = keymap_info->keymap.ctx;
    ರಚನೆ xkb_ಸೂಚಕ_default_action *act = &action->dflt;

    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_AFFECT) {
        uint32_t val = 0;

        ಆದರೆ (array_ndx)
            ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                        keymap_info->strict);

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(ctx, ಮೌಲ್ಯ, &val, ptrDflts))
            ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, action->type,
                                  ಕ್ಷೇತ್ರ_2, "pointer component", keymap_info->strict);
        static_assert(ಗಣ_ಸೂಚಕ_DEFAULT_BUTTON_AFFECT_ಅಂಕೆ_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_BUTTON || ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_ಮೌಲ್ಯ) {
        ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *button;
        int64_t btn = 0;

        ಆದರೆ (array_ndx)
            ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                        keymap_info->strict);

        ಆದರೆ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ||
            ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = ಮೌಲ್ಯ->unary.ಮಗು;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = ಮೌಲ್ಯ;
        }

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_ಸೂಚಕ_BUTTON_MIN == 1 &&
                          XKB_ಸೂಚಕ_BUTTON_MAX == 5, "invalid message");
            ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, action->type,
                                  ಕ್ಷೇತ್ರ_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_ಸೂಚಕ_BUTTON_DEFAULT < XKB_ಸೂಚಕ_BUTTON_MIN, "");
        ಆದರೆ (btn < XKB_ಸೂಚಕ_BUTTON_DEFAULT || btn > XKB_ಸೂಚಕ_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_ಸೂಚಕ_BUTTON_MIN, XKB_ಸೂಚಕ_BUTTON_MAX, btn);
            ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        ಆದರೆ (btn == XKB_ಸೂಚಕ_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->ಮೌಲ್ಯ = (int8_t) (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ? -btn: btn);
        ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
    }

    ಹಿಂತಿರುಗಿ ReportIllegal(ctx, action->type, ಕ್ಷೇತ್ರ_2, keymap_info->strict);
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
HandleSwitchScreen(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                   ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
                   ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                   ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                   ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ರಚನೆ xkb_context * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ctx = keymap_info->keymap.ctx;
    ರಚನೆ xkb_switch_screen_action *act = &action->screen;

    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_SCREEN) {
        ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *scrn;
        int64_t val = 0;

        ಆದರೆ (array_ndx)
            ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                        keymap_info->strict);

        ಆದರೆ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ||
            ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = ಮೌಲ್ಯ->unary.ಮಗು;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = ಮೌಲ್ಯ;
        }

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, scrn, &val))
            ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, action->type,
                                  ಕ್ಷೇತ್ರ_2, "integer (-128..127)",
                                  keymap_info->strict);

        val = (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NEGATE ? -val : val);
        ಆದರೆ (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_SAME) {
        ಹಿಂತಿರುಗಿ CheckBooleanFlag(ctx, keymap_info->strict, action->type, ಕ್ಷೇತ್ರ_2,
                                ACTION_SAME_SCREEN, array_ndx, ಮೌಲ್ಯ,
                                &act->flags);
    }

    ಹಿಂತಿರುಗಿ ReportIllegal(ctx, action->type, ಕ್ಷೇತ್ರ_2, keymap_info->strict);
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
Handle_ಗಣ_ಬೀಗ_Controls(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                      ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
                      ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                      ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                      ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ರಚನೆ xkb_context * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ctx = keymap_info->keymap.ctx;
    ರಚನೆ xkb_controls_action *act = &action->ctrls;

    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_CONTROLS) {
        ಆದರೆ (array_ndx)
            ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                        keymap_info->strict);

        uint32_t ಮುಖವಾಡ = 0;
        ಸ್ಥಿರಾಂಕ uint8_t ಸ್ಥಾನಾಂತರ = keymap_info->features.controls_name_ಸ್ಥಾನಾಂತರ;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಮುಖವಾಡ(ctx, ಮೌಲ್ಯ, &ಮುಖವಾಡ, ctrl_ಮುಖವಾಡ_ಹೆಸರುಗಳು + ಸ್ಥಾನಾಂತರ))
            ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, action->type,
                                  ಕ್ಷೇತ್ರ_2, "controls mask", keymap_info->strict);

        act->ctrls = (ಎಣಿಕೆ xkb_action_controls) ಮುಖವಾಡ;
        ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_AFFECT &&
             action->type == ACTION_TYPE_CTRL_ಬೀಗ) {
        ಹಿಂತಿರುಗಿ Check_Affect_ಕ್ಷೇತ್ರ(ctx, keymap_info->strict, action->type,
                                array_ndx, ಮೌಲ್ಯ, &act->flags);
    }

    ಹಿಂತಿರುಗಿ ReportIllegal(ctx, action->type, ಕ್ಷೇತ್ರ_2, keymap_info->strict);
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
Handle_Redirect_ಕೀಲಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                  ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
                  ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                  ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                  ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ keymap = &keymap_info->keymap;
    ರಚನೆ xkb_context * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ctx = keymap->ctx;
    ರಚನೆ xkb_redirect_ಕೀಲಿ_action * ಸ್ಥಿರಾಂಕ act = &action->redirect;

    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_KEYCODE) {
        ಆದರೆ (array_ndx)
            ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                        keymap_info->strict);
        ಆದರೆ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT) {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *valStr = xkb_atom_ಪಠ್ಯ(ctx, ಮೌಲ್ಯ->ident.ident);
            ಆದರೆ (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_ಕೀಲಿ_auto;
                ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
            }
        }
        ಆದರೆ (ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL) {
            ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, action->type,
                                  ಕ್ಷೇತ್ರ_2, "key name", keymap_info->strict);
        }
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ * ಸ್ಥಿರಾಂಕ ಕೀಲಿ =
            Xkb_ಕೀಲಿ_By_Name(keymap, ಮೌಲ್ಯ->ಕೀಲಿ_name.ಕೀಲಿ_name, true);
        ಆದರೆ (ಕೀಲಿ == ಖಾಲಿ) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    ಕ್ಷೇತ್ರ_ಪಠ್ಯ(ಕ್ಷೇತ್ರ_2), ಕೀಲಿ_Name_ಪಠ್ಯ(ctx, ಮೌಲ್ಯ->ಕೀಲಿ_name.ಕೀಲಿ_name));
            ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_ಮೌಲ್ಯ_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = ಕೀಲಿ->keycode;
        ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
    }

    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_MODIFIERS || ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_MODS_ಗಮ್ಯ_CLEAR) {
        ಎಣಿಕೆ xkb_action_flags flags = 0;
        xkb_mod_ಮುಖವಾಡ_t m = 0;
        ಎಣಿಕೆ xkb_parser_error r =
            Check_Modifier_ಕ್ಷೇತ್ರ(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, ಮೌಲ್ಯ, &flags, &m);
        ಆದರೆ (r != PARSER_SUCCESS)
            ಹಿಂತಿರುಗಿ r;
        ಆದರೆ (flags)
            ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, action->type,
                                  ಕ್ಷೇತ್ರ_2, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_MODIFIERS)
            act->mods |= m;
        ಇಲ್ಲದಿದ್ದರೆ
            act->mods &= ~m;
        ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
    }

    ಹಿಂತಿರುಗಿ ReportIllegal(ctx, action->type, ಕ್ಷೇತ್ರ_2, keymap_info->strict);
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
HandleUnsupported(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
                        ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                        ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                        ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)

{
    /* Do not check any field */
    ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
}

ಸ್ಥಿರ ಎಣಿಕೆ xkb_parser_error
Handle_ಖಾಸಗಿ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
              ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
              ಒಕ್ಕೂಟ xkb_action *action, ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
              ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
              ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ರಚನೆ xkb_context * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ctx = keymap_info->keymap.ctx;
    ರಚನೆ xkb_ಖಾಸಗಿ_action *act = &action->priv;

    ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_TYPE) {
        int64_t type = 0;

        ಆದರೆ (array_ndx)
            ಹಿಂತಿರುಗಿ ReportActionNotArray(ctx, action->type, ಕ್ಷೇತ್ರ_2,
                                        keymap_info->strict);

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, ಮೌಲ್ಯ, &type))
            ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                                  ACTION_TYPE_ಖಾಸಗಿ, ಕ್ಷೇತ್ರ_2, "integer",
                                  keymap_info->strict);

        ಆದರೆ (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
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
        ಆದರೆ (type < ACTION_TYPE_ಖಾಸಗಿ) {
            log_info(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     Action_Type_ಪಠ್ಯ((ಎಣಿಕೆ xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            act->type = (ಎಣಿಕೆ xkb_action_type) type;
        }

        ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕ್ಷೇತ್ರ_2 == ACTION_ಕ್ಷೇತ್ರ_ದತ್ತಾಂಶ) {
        ಆದರೆ (array_ndx == ಖಾಲಿ) {
            xkb_atom_t val = XKB_ATOM_NONE;

            ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(ctx, ಮೌಲ್ಯ, &val))
                ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                                      action->type, ಕ್ಷೇತ್ರ_2, "string",
                                      keymap_info->strict);

            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str = xkb_atom_ಪಠ್ಯ(ctx, val);
            ಗಾತ್ರ_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(str);
            ಆದರೆ (len < 1 || len > ಗಾತ್ರ(act->ದತ್ತಾಂಶ)) {
                log_warn(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", ಗಾತ್ರ(act->ದತ್ತಾಂಶ), len);
                ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->ದತ್ತಾಂಶ, 0, ಗಾತ್ರ(act->ದತ್ತಾಂಶ));
            ಸ್ಮರಣೆ_ನಕಲು(act->ದತ್ತಾಂಶ, str, len);
            ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            int64_t ndx = 0, datum = 0;

            ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ಆದರೆ (ndx < 0 || (ಗಾತ್ರ_t) ndx >= ಗಾತ್ರ(act->ದತ್ತಾಂಶ)) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        ಗಾತ್ರ(act->ದತ್ತಾಂಶ), ndx);
                ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(ctx, ಮೌಲ್ಯ, &datum))
                ಹಿಂತಿರುಗಿ ReportMismatch(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, act->type,
                                      ಕ್ಷೇತ್ರ_2, "integer", keymap_info->strict);

            ಆದರೆ (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->ದತ್ತಾಂಶ[ndx] = (uint8_t) datum;
            ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
        }
    }

    ಹಿಂತಿರುಗಿ ReportIllegal(ctx, action->type, ಕ್ಷೇತ್ರ_2, keymap_info->strict);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಎಣಿಕೆ xkb_parser_error
        (*actionHandler)(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                         ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
                         ಒಕ್ಕೂಟ xkb_action *action,
                         ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2,
                         ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                         ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
                         ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr);

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ actionHandler handleAction[ACTION_TYPE_ಅಂಕೆ_ENTRIES] = {
    [ACTION_TYPE_NONE] = HandleNoAction,
    [ACTION_TYPE_VOID] = HandleNoAction,
    [ACTION_TYPE_MOD_ಗಣ] = Handle_ಗಣ_Latch_ಬೀಗ_Mods,
    [ACTION_TYPE_MOD_LATCH] = Handle_ಗಣ_Latch_ಬೀಗ_Mods,
    [ACTION_TYPE_MOD_ಬೀಗ] = Handle_ಗಣ_Latch_ಬೀಗ_Mods,
    [ACTION_TYPE_ಗುಂಪು_ಗಣ] = Handle_ಗಣ_Latch_ಬೀಗ_ಗುಂಪು,
    [ACTION_TYPE_ಗುಂಪು_LATCH] = Handle_ಗಣ_Latch_ಬೀಗ_ಗುಂಪು,
    [ACTION_TYPE_ಗುಂಪು_ಬೀಗ] = Handle_ಗಣ_Latch_ಬೀಗ_ಗುಂಪು,
    [ACTION_TYPE_PTR_MOVE] = HandleMovePtr,
    [ACTION_TYPE_PTR_BUTTON] = HandlePtrBtn,
    [ACTION_TYPE_PTR_ಬೀಗ] = HandlePtrBtn,
    [ACTION_TYPE_PTR_DEFAULT] = Handle_ಗಣ_Ptr_Dflt,
    [ACTION_TYPE_TERMINATE] = HandleNoAction,
    [ACTION_TYPE_SWITCH_VT] = HandleSwitchScreen,
    [ACTION_TYPE_CTRL_ಗಣ] = Handle_ಗಣ_ಬೀಗ_Controls,
    [ACTION_TYPE_CTRL_ಬೀಗ] = Handle_ಗಣ_ಬೀಗ_Controls,
    [ACTION_TYPE_REDIRECT_ಕೀಲಿ] = Handle_Redirect_ಕೀಲಿ,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_ಖಾಸಗಿ] = Handle_ಖಾಸಗಿ,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_ಆಂತರಿಕ == 20 &&
              ACTION_TYPE_ಆಂತರಿಕ + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
              "Missing action type");

/***====================================================================***/

ಎಣಿಕೆ xkb_parser_error
HandleActionDef(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info, ActionsInfo *info,
                ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods, ಅಭಿವ್ಯಕ್ತಿ_Def *def,
                ಒಕ್ಕೂಟ xkb_action *action)
{
    ರಚನೆ xkb_context * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ctx = keymap_info->keymap.ctx;
    ಆದರೆ (def->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_ಗಮ್ಯ_string(def->common.type));
        ಹಿಂತಿರುಗಿ PARSER_FATAL_ERROR;
    }

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *action_name = xkb_atom_ಪಠ್ಯ(ctx, def->action.name);
    ಎಣಿಕೆ xkb_action_type handler_type;
    ಆದರೆ (!string_ಗಮ್ಯ_Action_Type(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        ಆದರೆ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ಹಿಂತಿರುಗಿ PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    ಆದರೆ (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    ಎಣಿಕೆ xkb_parser_error ret = PARSER_SUCCESS;
    ಗಾಗಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *arg = def->action.ವಾದಗಳು; arg != ಖಾಲಿ;
         arg = (ಅಭಿವ್ಯಕ್ತಿ_Def *) arg->common.ಮುಂದಿನ) {
        ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ;
        ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr = ಖಾಲಿ;
        ಅಭಿವ್ಯಕ್ತಿ_Def *ಕ್ಷೇತ್ರ_2, *arrayRtrn;
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elemRtrn, *ಕ್ಷೇತ್ರ_Rtrn;

        ಆದರೆ (arg->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_ನಿಯೋಜಿಸು) {
            ಕ್ಷೇತ್ರ_2 = arg->ದ್ವಿಮಾನ.ಎಡ;
            ಮೌಲ್ಯ = arg->ದ್ವಿಮಾನ.ಬಲ;
            ಮೌಲ್ಯ_ptr = &arg->ದ್ವಿಮಾನ.ಬಲ;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (arg->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_NOT ||
                 arg->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_INVERT) {
            ಕ್ಷೇತ್ರ_2 = arg->unary.ಮಗು;
            ಮೌಲ್ಯ = (ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *) &constFalse;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಕ್ಷೇತ್ರ_2 = arg;
            ಮೌಲ್ಯ = (ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *) &constTrue;
        }

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(ctx, ಕ್ಷೇತ್ರ_2, &elemRtrn, &ಕ್ಷೇತ್ರ_Rtrn, &arrayRtrn))
            ಹಿಂತಿರುಗಿ PARSER_FATAL_ERROR;

        ಆದರೆ (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, ಕ್ಷೇತ್ರ_Rtrn);
            ಹಿಂತಿರುಗಿ PARSER_FATAL_ERROR;
        }

        ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_Ndx;
        ಆದರೆ (!string_ಗಮ್ಯ_ಕ್ಷೇತ್ರ(ಕ್ಷೇತ್ರ_Rtrn, &ಕ್ಷೇತ್ರ_Ndx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_ಕ್ಷೇತ್ರ,
                    "Unknown field name %s for action %s discarded\n",
                    ಕ್ಷೇತ್ರ_Rtrn, Action_Type_ಪಠ್ಯ(action->type));
            ಆದರೆ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                ಹಿಂತಿರುಗಿ PARSER_FATAL_ERROR;
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Field is just ignored */
                ಮುಂದುವರಿ;
            }
        }

        ಆಯ್ಕೆ (handleAction[handler_type](keymap_info, mods, action, ಕ್ಷೇತ್ರ_Ndx,
                                           arrayRtrn, ಮೌಲ್ಯ, ಮೌಲ್ಯ_ptr)) {
        ಸ್ಥಿತಿ PARSER_FATAL_ERROR:
            ಹಿಂತಿರುಗಿ PARSER_FATAL_ERROR;
        ಸ್ಥಿತಿ PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            ;
        }
    }

    ಹಿಂತಿರುಗಿ (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

ಎಣಿಕೆ xkb_parser_error
ಗಣ_Default_Action_ಕ್ಷೇತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ರಚನೆ xkb_mod_ಗಣ *mods,
                      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elem, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2, ಅಭಿವ್ಯಕ್ತಿ_Def *array_ndx,
                      ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr, ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ)
{
    ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ಮೌಲ್ಯ = *ಮೌಲ್ಯ_ptr;
    ಎಣಿಕೆ xkb_action_type action;
    ಆದರೆ (!string_ಗಮ್ಯ_Action_Type(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ಎಣಿಕೆ action_ಕ್ಷೇತ್ರ action_ಕ್ಷೇತ್ರ;
    ಆದರೆ (!string_ಗಮ್ಯ_ಕ್ಷೇತ್ರ(ಕ್ಷೇತ್ರ_2, &action_ಕ್ಷೇತ್ರ)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_ಕ್ಷೇತ್ರ,
                "Unknown action field \"%s\"\n", ಕ್ಷೇತ್ರ_2);
        ಹಿಂತಿರುಗಿ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ಒಕ್ಕೂಟ xkb_action* ಸ್ಥಿರಾಂಕ into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    ಒಕ್ಕೂಟ xkb_action from = *into;

    /* Parse action */
    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_ಕ್ಷೇತ್ರ, array_ndx, ಮೌಲ್ಯ, ಮೌಲ್ಯ_ptr
    );

    /* Success is mandatory to update defaults */
    ಆದರೆ (ret != PARSER_SUCCESS)
        ಹಿಂತಿರುಗಿ ret;

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
    ಆದರೆ (!action_equal(into, &from)) {
        ಸ್ಥಿರಾಂಕ bool replace = (ವಿಲೀನ != ವಿಲೀನ_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                ಕ್ಷೇತ್ರ_ಪಠ್ಯ(action_ಕ್ಷೇತ್ರ), Action_Type_ಪಠ್ಯ(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        ಆದರೆ (replace)
            *into = from;
    }
    ಹಿಂತಿರುಗಿ PARSER_SUCCESS;
}
