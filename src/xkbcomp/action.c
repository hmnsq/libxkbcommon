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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "expr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "action.h"

ਸਥਿਰ ਅਚਲ ਵਿਅੰਜਕ_Boolean constTrue = {
    .common = { .type = STMT_ਵਿਅੰਜਕ_BOOLEAN_LITERAL, .ਅਗਲਾ = ਨਿਰਾ },
    .ਸੈੱਟ = true,
};

ਸਥਿਰ ਅਚਲ ਵਿਅੰਜਕ_Boolean constFalse = {
    .common = { .type = STMT_ਵਿਅੰਜਕ_BOOLEAN_LITERAL, .ਅਗਲਾ = ਨਿਰਾ },
    .ਸੈੱਟ = false,
};

ਗਿਣਤੀ action_ਖੇਤਰ {
    ACTION_ਖੇਤਰ_CLEAR_LOCKS,
    ACTION_ਖੇਤਰ_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ,
    ACTION_ਖੇਤਰ_GEN_ਕੁੰਜੀ_EVENT,
    ACTION_ਖੇਤਰ_REPORT,
    ACTION_ਖੇਤਰ_DEFAULT,
    ACTION_ਖੇਤਰ_AFFECT,
    ACTION_ਖੇਤਰ_INCREMENT,
    ACTION_ਖੇਤਰ_MODIFIERS,
    ACTION_ਖੇਤਰ_ਸਮੂਹ,
    ACTION_ਖੇਤਰ_X,
    ACTION_ਖੇਤਰ_Y,
    ACTION_ਖੇਤਰ_REPEAT,
    ACTION_ਖੇਤਰ_BUTTON,
    ACTION_ਖੇਤਰ_ਮੁੱਲ,
    ACTION_ਖੇਤਰ_CONTROLS,
    ACTION_ਖੇਤਰ_TYPE,
    ACTION_ਖੇਤਰ_ਗਿਣਤੀ,
    ACTION_ਖੇਤਰ_SCREEN,
    ACTION_ਖੇਤਰ_SAME,
    ACTION_ਖੇਤਰ_ਅੰਕੜੇ,
    ACTION_ਖੇਤਰ_DEVICE,
    ACTION_ਖੇਤਰ_KEYCODE,
    ACTION_ਖੇਤਰ_MODS_ਮੰਜ਼ਿਲ_CLEAR,
    ACTION_ਖੇਤਰ_ਤਾਲਾ_ON_RELEASE,
    ACTION_ਖੇਤਰ_UNLOCK_ON_PRESS,
    ACTION_ਖੇਤਰ_LATCH_ON_PRESS,
};

ਖਾਲੀ
InitActionsInfo(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, ActionsInfo *info)
{
    ਲਈ (ਗਿਣਤੀ xkb_action_type type = 0; type < ACTION_TYPE_ਅੰਕ_ENTRIES; type++)
        info->actions[type].type = type;

    /* Apply some "factory defaults". */

    /* Increment default button. */
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.flags = 0;
    info->actions[ACTION_TYPE_PTR_DEFAULT].dflt.ਮੁੱਲ = 1;
    info->actions[ACTION_TYPE_PTR_MOVE].ptr.flags = ACTION_REPEAT;
    info->actions[ACTION_TYPE_SWITCH_VT].screen.flags = ACTION_SAME_SCREEN;
    info->actions[ACTION_TYPE_REDIRECT_ਕੁੰਜੀ].redirect.keycode =
        keymap->redirect_ਕੁੰਜੀ_auto;
}

ਸਥਿਰ ਅਚਲ Lookup_ਇੰਦਰਾਜ ਖੇਤਰ_ਲੜੀਆਂ[] = {
    { "clearLocks",       ACTION_ਖੇਤਰ_CLEAR_LOCKS     },
    { "latchToLock",      ACTION_ਖੇਤਰ_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ   },
    { "genKeyEvent",      ACTION_ਖੇਤਰ_GEN_ਕੁੰਜੀ_EVENT   },
    { "generateKeyEvent", ACTION_ਖੇਤਰ_GEN_ਕੁੰਜੀ_EVENT   },
    { "report",           ACTION_ਖੇਤਰ_REPORT          },
    { "default",          ACTION_ਖੇਤਰ_DEFAULT         },
    { "affect",           ACTION_ਖੇਤਰ_AFFECT          },
    { "increment",        ACTION_ਖੇਤਰ_INCREMENT       },
    { "modifiers",        ACTION_ਖੇਤਰ_MODIFIERS       },
    { "mods",             ACTION_ਖੇਤਰ_MODIFIERS       },
    { "group",            ACTION_ਖੇਤਰ_ਸਮੂਹ           },
    { "x",                ACTION_ਖੇਤਰ_X               },
    { "y",                ACTION_ਖੇਤਰ_Y               },
    { "repeat",           ACTION_ਖੇਤਰ_REPEAT          },
    { "accel",            ACTION_ਖੇਤਰ_REPEAT          },
    { "accelerate",       ACTION_ਖੇਤਰ_REPEAT          },
    { "button",           ACTION_ਖੇਤਰ_BUTTON          },
    { "value",            ACTION_ਖੇਤਰ_ਮੁੱਲ           },
    { "controls",         ACTION_ਖੇਤਰ_CONTROLS        },
    { "ctrls",            ACTION_ਖੇਤਰ_CONTROLS        },
    { "type",             ACTION_ਖੇਤਰ_TYPE            },
    { "count",            ACTION_ਖੇਤਰ_ਗਿਣਤੀ           },
    { "screen",           ACTION_ਖੇਤਰ_SCREEN          },
    { "same",             ACTION_ਖੇਤਰ_SAME            },
    { "sameServer",       ACTION_ਖੇਤਰ_SAME            },
    { "data",             ACTION_ਖੇਤਰ_ਅੰਕੜੇ            },
    { "device",           ACTION_ਖੇਤਰ_DEVICE          },
    { "dev",              ACTION_ਖੇਤਰ_DEVICE          },
    { "key",              ACTION_ਖੇਤਰ_KEYCODE         },
    { "keycode",          ACTION_ਖੇਤਰ_KEYCODE         },
    { "kc",               ACTION_ਖੇਤਰ_KEYCODE         },
    { "clearmods",        ACTION_ਖੇਤਰ_MODS_ਮੰਜ਼ਿਲ_CLEAR   },
    { "clearmodifiers",   ACTION_ਖੇਤਰ_MODS_ਮੰਜ਼ਿਲ_CLEAR   },
    { "lockOnRelease",    ACTION_ਖੇਤਰ_ਤਾਲਾ_ON_RELEASE },
    { "unlockOnPress",    ACTION_ਖੇਤਰ_UNLOCK_ON_PRESS },
    { "latchOnPress",     ACTION_ਖੇਤਰ_LATCH_ON_PRESS  },
    { ਨਿਰਾ,               0                            }
};

ਸਥਿਰ bool
string_ਮੰਜ਼ਿਲ_Action_Type(ਅਚਲ ਅੱਖਰ *str, ਗਿਣਤੀ xkb_action_type *type_rtrn)
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ type = 0;
    ਅਚਲ bool ret = LookupString(action_Type_ਨਾਂ, str, &type);
    *type_rtrn = (ਗਿਣਤੀ xkb_action_type) type;
    ਵਾਪਸ ret;
}

ਸਥਿਰ bool
string_ਮੰਜ਼ਿਲ_ਖੇਤਰ(ਅਚਲ ਅੱਖਰ *str, ਗਿਣਤੀ action_ਖੇਤਰ *ਖੇਤਰ_rtrn)
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੇਤਰ_2 = 0;
    ਅਚਲ bool ret = LookupString(ਖੇਤਰ_ਲੜੀਆਂ, str, &ਖੇਤਰ_2);
    *ਖੇਤਰ_rtrn = (ਗਿਣਤੀ action_ਖੇਤਰ) ਖੇਤਰ_2;
    ਵਾਪਸ ret;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
ਖੇਤਰ_ਪਾਠ(ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2)
{
    ਵਾਪਸ Lookup_ਮੁੱਲ(ਖੇਤਰ_ਲੜੀਆਂ, ਖੇਤਰ_2);
}

/***====================================================================***/

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ xkb_parser_error
ReportMismatch(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_ਸੁਨੇਹਾ_ਕੋਡ ਕੋਡ,
               ਗਿਣਤੀ xkb_action_type action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
               ਅਚਲ ਅੱਖਰ *type, ਗਿਣਤੀ xkb_parser_strict_flags strict)
{
    log_err(ctx, ਕੋਡ,
            "Value of %s field must be of type %s; "
            "Action %s definition ignored\n",
            ਖੇਤਰ_ਪਾਠ(ਖੇਤਰ_2), type, Action_Type_ਪਾਠ(action));
    ਵਾਪਸ (strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ xkb_parser_error
Report_Format_ਸੰਸਕਰਣ_Mismatch(ਬਣਤਰ xkb_context *ctx,
                            ਗਿਣਤੀ xkb_action_type action,
                            ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                            ਗਿਣਤੀ xkb_keymap_format format,
                            ਅਚਲ ਅੱਖਰ *versions,
                            ਗਿਣਤੀ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ਪਾਠ_FORMAT,
            "Field %s for an action of type %s requires keymap text format %s, "
            " but got: %d; Action definition ignored\n",
            ਖੇਤਰ_ਪਾਠ(ਖੇਤਰ_2), Action_Type_ਪਾਠ(action), versions, format);
    ਵਾਪਸ (strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ xkb_parser_error
ReportIllegal(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_action_type action,
              ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2, ਗਿਣਤੀ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_INVALID_ACTION_ਖੇਤਰ,
            "Field %s is not defined for an action of type %s; "
            "Action definition ignored\n",
            ਖੇਤਰ_ਪਾਠ(ਖੇਤਰ_2), Action_Type_ਪਾਠ(action));
    ਵਾਪਸ (strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ xkb_parser_error
ReportActionNotArray(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_action_type action,
                     ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                     ਗਿਣਤੀ xkb_parser_strict_flags strict)
{
    log_err(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
            "The %s field in the %s action is not an array; "
            "Action definition ignored\n",
            ਖੇਤਰ_ਪਾਠ(ਖੇਤਰ_2), Action_Type_ਪਾਠ(action));
    ਵਾਪਸ (strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
        ? PARSER_FATAL_ERROR
        : PARSER_RECOVERABLE_ERROR;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
HandleNoAction(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
               ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
               ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
               ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
               ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_ਖੇਤਰ,
            "The \"%s\" action takes no argument, but got \"%s\" field; "
            "Action definition ignored\n",
            Action_Type_ਪਾਠ(action->type), ਖੇਤਰ_ਪਾਠ(ਖੇਤਰ_2));
    ਵਾਪਸ (keymap_info->strict & PARSER_NO_ILLEGAL_ACTION_FIELDS)
        ? PARSER_FATAL_ERROR
        /* No error reported: field is just ignored */
        : PARSER_SUCCESS;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
CheckBooleanFlag(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_parser_strict_flags strict,
                 ਗਿਣਤੀ xkb_action_type action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                 ਗਿਣਤੀ xkb_action_flags flag, ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx,
                 ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ, ਗਿਣਤੀ xkb_action_flags *flags_inout)
{
    bool ਸੈੱਟ = false;

    ਜੇ (array_ndx)
        ਵਾਪਸ ReportActionNotArray(ctx, action, ਖੇਤਰ_2, strict);

    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Boolean(ctx, ਮੁੱਲ, &ਸੈੱਟ))
        ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                              action, ਖੇਤਰ_2, "boolean", strict);

    ਜੇ (ਸੈੱਟ)
        *flags_inout |= flag;
    ਨਹੀਂਤਾਂ
        *flags_inout &= ~flag;

    ਵਾਪਸ PARSER_SUCCESS;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
Check_Modifier_ਖੇਤਰ(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_parser_strict_flags strict,
                   ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, ਗਿਣਤੀ xkb_action_type action,
                   ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
                   ਗਿਣਤੀ xkb_action_flags *flags_inout, xkb_mod_ਨਕਾਬ_t *mods_rtrn)
{
    ਜੇ (array_ndx)
        ਵਾਪਸ ReportActionNotArray(ctx, action, ACTION_ਖੇਤਰ_MODIFIERS, strict);

    ਜੇ (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_IDENT) {
        ਅਚਲ ਅੱਖਰ *valStr;
        valStr = xkb_atom_ਪਾਠ(ctx, ਮੁੱਲ->ident.ident);
        ਜੇ (valStr && (istreq(valStr, "usemodmapmods") ||
                       istreq(valStr, "modmapmods"))) {
            *mods_rtrn = 0;
            *flags_inout |= ACTION_MODS_LOOKUP_MODMAP;
            ਵਾਪਸ PARSER_SUCCESS;
        }
    }

    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(ctx, ਮੁੱਲ, MOD_BOTH, mods, mods_rtrn))
        ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, action,
                              ACTION_ਖੇਤਰ_MODIFIERS, "modifier mask", strict);

    *flags_inout &= ~ACTION_MODS_LOOKUP_MODMAP;
    ਵਾਪਸ PARSER_SUCCESS;
}

ਸਥਿਰ ਅਚਲ Lookup_ਇੰਦਰਾਜ ਤਾਲਾ_Which[] = {
    { "both", 0 },
    { "lock", ACTION_ਤਾਲਾ_NO_UNLOCK },
    { "neither", (ACTION_ਤਾਲਾ_NO_ਤਾਲਾ | ACTION_ਤਾਲਾ_NO_UNLOCK) },
    { "unlock", ACTION_ਤਾਲਾ_NO_ਤਾਲਾ },
    { ਨਿਰਾ, 0 }
};

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
Check_Affect_ਖੇਤਰ(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_parser_strict_flags strict,
                 ਗਿਣਤੀ xkb_action_type action, ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx,
                 ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ, ਗਿਣਤੀ xkb_action_flags *flags_inout)
{
    ਜੇ (array_ndx)
        ਵਾਪਸ ReportActionNotArray(ctx, action, ACTION_ਖੇਤਰ_AFFECT, strict);

    uint32_t flags = 0;
    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Enum(ctx, ਮੁੱਲ, &flags, ਤਾਲਾ_Which))
        ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                              action, ACTION_ਖੇਤਰ_AFFECT,
                              "lock, unlock, both, neither", strict);

    *flags_inout &= ~(ACTION_ਤਾਲਾ_NO_ਤਾਲਾ | ACTION_ਤਾਲਾ_NO_UNLOCK);
    *flags_inout |= (ਗਿਣਤੀ xkb_action_flags) flags;
    ਵਾਪਸ PARSER_SUCCESS;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
Handle_ਸੈੱਟ_Latch_ਤਾਲਾ_Mods(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                       ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                       ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                       ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
                       ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ਅਚਲ ctx = keymap_info->keymap.ctx;
    ਬਣਤਰ xkb_mod_action *act = &action->mods;
    ਅਚਲ ਗਿਣਤੀ xkb_action_type type = action->type;

    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_MODIFIERS)
        ਵਾਪਸ Check_Modifier_ਖੇਤਰ(ctx, keymap_info->strict, mods, action->type,
                                  array_ndx, ਮੁੱਲ, &act->flags, &act->mods.mods);
    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_UNLOCK_ON_PRESS) {
        /* Ensure to update if a new modifier action is introduced. */
        ਪ੍ਰਤਿਜ੍ਞਾ(type == ACTION_TYPE_MOD_ਸੈੱਟ || type == ACTION_TYPE_MOD_LATCH ||
               type == ACTION_TYPE_MOD_ਤਾਲਾ);
        ਜੇ (keymap_info->features.mods_unlock_on_press) {
            ਅਚਲ ਗਿਣਤੀ xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, ਖੇਤਰ_2,
                ACTION_UNLOCK_ON_PRESS, array_ndx, ਮੁੱਲ, &act->flags
            );
            ਜੇ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_UNLOCK_ON_PRESS;
            ਵਾਪਸ ret;
        } ਨਹੀਂਤਾਂ {
            ਵਾਪਸ Report_Format_ਸੰਸਕਰਣ_Mismatch(ctx, action->type, ਖੇਤਰ_2,
                                               keymap_info->keymap.format,
                                               ">= 2", keymap_info->strict);
        }
    }
    ਜੇ ((type == ACTION_TYPE_MOD_ਸੈੱਟ || type == ACTION_TYPE_MOD_LATCH) &&
        ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_CLEAR_LOCKS)
        ਵਾਪਸ CheckBooleanFlag(ctx, keymap_info->strict, action->type, ਖੇਤਰ_2,
                                ACTION_ਤਾਲਾ_CLEAR, array_ndx, ਮੁੱਲ,
                                &act->flags);
    ਜੇ (type == ACTION_TYPE_MOD_LATCH) {
        ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ)
            ਵਾਪਸ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                    ਖੇਤਰ_2, ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ, array_ndx,
                                    ਮੁੱਲ, &act->flags);
        ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_LATCH_ON_PRESS) {
            ਜੇ (keymap_info->features.mods_latch_on_press) {
                ਵਾਪਸ CheckBooleanFlag(ctx, keymap_info->strict, action->type,
                                        ਖੇਤਰ_2, ACTION_LATCH_ON_PRESS, array_ndx,
                                        ਮੁੱਲ, &act->flags);
            } ਨਹੀਂਤਾਂ {
                ਵਾਪਸ Report_Format_ਸੰਸਕਰਣ_Mismatch(ctx, action->type, ਖੇਤਰ_2,
                                                   keymap_info->keymap.format,
                                                   ">= 2", keymap_info->strict);
            }
        }
    }
    ਜੇ (type == ACTION_TYPE_MOD_ਤਾਲਾ && ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_AFFECT)
        ਵਾਪਸ Check_Affect_ਖੇਤਰ(ctx, keymap_info->strict, action->type,
                                array_ndx, ਮੁੱਲ, &act->flags);

    ਵਾਪਸ ReportIllegal(ctx, action->type, ਖੇਤਰ_2, keymap_info->strict);
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
Check_ਸਮੂਹ_ਖੇਤਰ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                ਗਿਣਤੀ xkb_action_type action, ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx,
                ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ, ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr,
                ਗਿਣਤੀ xkb_action_flags *flags_inout, int32_t *ਸਮੂਹ_rtrn)
{
    ਅਚਲ ਵਿਅੰਜਕ_Def *spec;
    xkb_layout_ਸੂਚਕਾਂਕ_t idx = 0;
    ਗਿਣਤੀ xkb_action_flags flags = *flags_inout;

    ਜੇ (array_ndx)
        ਵਾਪਸ ReportActionNotArray(keymap_info->keymap.ctx, action,
                                    ACTION_ਖੇਤਰ_ਸਮੂਹ, keymap_info->strict);

    ਜੇ (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_NEGATE ||
        ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_UNARY_PLUS) {
        flags &= ~ACTION_ABSOLUTE_SWITCH;
        spec = ਮੁੱਲ->unary.ਬੱਚਾ;
        ਮੁੱਲ_ptr = &(*ਮੁੱਲ_ptr)->unary.ਬੱਚਾ;
    } ਨਹੀਂਤਾਂ {
        flags |= ACTION_ABSOLUTE_SWITCH;
        spec = ਮੁੱਲ;
    }

    ਅਚਲ bool absolute = (flags & ACTION_ABSOLUTE_SWITCH);
    bool pending = false;
    ਅਚਲ ਗਿਣਤੀ xkb_parser_error ret =
        ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ(keymap_info, spec, absolute, &idx, &pending);
    ਜੇ (ret != PARSER_SUCCESS && !pending) {
        ReportMismatch(keymap_info->keymap.ctx,
                       XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2, action,
                       ACTION_ਖੇਤਰ_ਸਮੂਹ, "integer", keymap_info->strict);
        ਵਾਪਸ ret;
    }

    ਜੇ (pending) {
        flags |= ACTION_PENDING_COMPUTATION;
        ਅਚਲ darray_ਮਾਪ_t pending_ਸੂਚਕਾਂਕ =
            darray_ਮਾਪ(*keymap_info->pending_computations);
        darray_append(
            *keymap_info->pending_computations,
            (ਬਣਤਰ pending_computation) {
                .ਵਿਅੰਜਕ = *ਮੁੱਲ_ptr,
                .computed = false,
                .ਮੁੱਲ = 0,
            }
        );
        *ਮੁੱਲ_ptr = ਨਿਰਾ;
        static_assert(ਆਕਾਰ(pending_ਸੂਚਕਾਂਕ) == ਆਕਾਰ(*ਸਮੂਹ_rtrn),
                      "Cannot save pending computation");
        *ਸਮੂਹ_rtrn = (int32_t) pending_ਸੂਚਕਾਂਕ;
    } ਨਹੀਂਤਾਂ {
        flags &= ~ACTION_PENDING_COMPUTATION;
        /* `+n`, `-n` are relative, `n` is absolute. */
        ਜੇ (absolute) {
            *ਸਮੂਹ_rtrn = (int32_t) (idx - 1);
        } ਨਹੀਂਤਾਂ {
            *ਸਮੂਹ_rtrn = (int32_t) idx;
            ਜੇ (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_NEGATE)
                *ਸਮੂਹ_rtrn = -*ਸਮੂਹ_rtrn;
        }
    }

    *flags_inout = flags;
    ਵਾਪਸ PARSER_SUCCESS;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
Handle_ਸੈੱਟ_Latch_ਤਾਲਾ_ਸਮੂਹ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                        ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                        ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                        ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
                        ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ਅਚਲ ctx = keymap_info->keymap.ctx;
    ਬਣਤਰ xkb_ਸਮੂਹ_action *act = &action->ਸਮੂਹ;
    ਅਚਲ ਗਿਣਤੀ xkb_action_type type = action->type;

    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_ਸਮੂਹ) {
        ਵਾਪਸ Check_ਸਮੂਹ_ਖੇਤਰ(keymap_info, action->type, array_ndx, ਮੁੱਲ,
                               ਮੁੱਲ_ptr, &act->flags, &act->ਸਮੂਹ);
    }
    ਜੇ ((type == ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ || type == ACTION_TYPE_ਸਮੂਹ_LATCH) &&
        ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_CLEAR_LOCKS)
        ਵਾਪਸ CheckBooleanFlag(ctx, keymap_info->strict, action->type, ਖੇਤਰ_2,
                                ACTION_ਤਾਲਾ_CLEAR, array_ndx, ਮੁੱਲ,
                                &act->flags);
    ਜੇ (type == ACTION_TYPE_ਸਮੂਹ_LATCH &&
        ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ)
        ਵਾਪਸ CheckBooleanFlag(ctx, keymap_info->strict, action->type, ਖੇਤਰ_2,
                                ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ, array_ndx, ਮੁੱਲ,
                                &act->flags);
    ਜੇ (type == ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ &&
        ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_ਤਾਲਾ_ON_RELEASE) {
        /* TODO: support this for `ACTION_TYPE_MOD_LOCK` too? */
        ਜੇ (keymap_info->features.ਸਮੂਹ_ਤਾਲਾ_on_release) {
            ਅਚਲ ਗਿਣਤੀ xkb_parser_error ret = CheckBooleanFlag(
                ctx, keymap_info->strict, action->type, ਖੇਤਰ_2,
                ACTION_ਤਾਲਾ_ON_RELEASE, array_ndx, ਮੁੱਲ, &act->flags
            );
            ਜੇ (ret == PARSER_SUCCESS)
                act->flags |= ACTION_EXPLICIT_ਤਾਲਾ_ON_RELEASE;
            ਵਾਪਸ ret;
        } ਨਹੀਂਤਾਂ {
            ਵਾਪਸ Report_Format_ਸੰਸਕਰਣ_Mismatch(ctx, action->type, ਖੇਤਰ_2,
                                               keymap_info->keymap.format,
                                               ">= v2", keymap_info->strict);
        }
    }

    ਵਾਪਸ ReportIllegal(ctx, action->type, ਖੇਤਰ_2, keymap_info->strict);
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
HandleMovePtr(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
              ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
              ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
              ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
              ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ਅਚਲ ctx = keymap_info->keymap.ctx;
    ਬਣਤਰ xkb_ਸੂਚਕ_motion_action *act = &action->ptr;

    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_X || ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_Y) {
        int64_t val = 0;
        ਅਚਲ bool absolute = (ਮੁੱਲ->common.type != STMT_ਵਿਅੰਜਕ_NEGATE &&
                               ਮੁੱਲ->common.type != STMT_ਵਿਅੰਜਕ_UNARY_PLUS);

        ਜੇ (array_ndx)
            ਵਾਪਸ ReportActionNotArray(ctx, action->type, ਖੇਤਰ_2,
                                        keymap_info->strict);

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer(ctx, ਮੁੱਲ, &val))
            ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, action->type,
                                  ਖੇਤਰ_2, "integer", keymap_info->strict);

        ਜੇ (val < INT16_MIN || val > INT16_MAX) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "The %s field in the %s action must be in range %d..%d, "
                    "but got %"PRId64". Action definition ignored\n",
                    ਖੇਤਰ_ਪਾਠ(ਖੇਤਰ_2), Action_Type_ਪਾਠ(action->type),
                    INT16_MIN, INT16_MAX, val);
            ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_X) {
            ਜੇ (absolute)
                act->flags |= ACTION_ABSOLUTE_X;
            act->x = (int16_t) val;
        }
        ਨਹੀਂਤਾਂ {
            ਜੇ (absolute)
                act->flags |= ACTION_ABSOLUTE_Y;
            act->y = (int16_t) val;
        }

        ਵਾਪਸ PARSER_SUCCESS;
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_REPEAT) {
        ਵਾਪਸ CheckBooleanFlag(ctx, keymap_info->strict, action->type, ਖੇਤਰ_2,
                                ACTION_REPEAT, array_ndx, ਮੁੱਲ, &act->flags);
    }

    ਵਾਪਸ ReportIllegal(ctx, action->type, ਖੇਤਰ_2, keymap_info->strict);
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
HandlePtrBtn(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
             ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
             ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
             ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
             ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ਅਚਲ ctx = keymap_info->keymap.ctx;
    ਬਣਤਰ xkb_ਸੂਚਕ_button_action *act = &action->btn;

    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_BUTTON) {
        int64_t btn = 0;

        ਜੇ (array_ndx)
            ਵਾਪਸ ReportActionNotArray(ctx, action->type, ਖੇਤਰ_2,
                                        keymap_info->strict);

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Button(ctx, ਮੁੱਲ, &btn)) {
            static_assert(XKB_ਸੂਚਕ_BUTTON_MIN == 1 &&
                          XKB_ਸੂਚਕ_BUTTON_MAX == 5, "invalid message");
            ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, action->type,
                                  ਖੇਤਰ_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_ਸੂਚਕ_BUTTON_DEFAULT < XKB_ਸੂਚਕ_BUTTON_MIN, "");
        ਜੇ (btn < XKB_ਸੂਚਕ_BUTTON_DEFAULT || btn > XKB_ਸੂਚਕ_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Button must specify default or be in the range %d..%d; "
                    "Illegal button value %"PRId64" ignored\n",
                    XKB_ਸੂਚਕ_BUTTON_MIN, XKB_ਸੂਚਕ_BUTTON_MAX, btn);
            ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->button = (uint8_t) btn;
        ਵਾਪਸ PARSER_SUCCESS;
    }
    ਨਹੀਂਤਾਂ ਜੇ (action->type == ACTION_TYPE_PTR_ਤਾਲਾ &&
             ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_AFFECT) {
        ਵਾਪਸ Check_Affect_ਖੇਤਰ(ctx, keymap_info->strict, action->type,
                                array_ndx, ਮੁੱਲ, &act->flags);
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_ਗਿਣਤੀ) {
        int64_t val = 0;

        ਜੇ (array_ndx)
            ਵਾਪਸ ReportActionNotArray(ctx, action->type, ਖੇਤਰ_2,
                                        keymap_info->strict);

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer(ctx, ਮੁੱਲ, &val))
            ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, action->type,
                                  ਖੇਤਰ_2, "integer", keymap_info->strict);

        ਜੇ (val < 0 || val > 255) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "The count field must have a value in the range 0..255; "
                    "Illegal count %"PRId64" ignored\n", val);
            ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        /*
         * NOTE: `count` is parsed but ignored. It seems an oversight in
         * X.Org’s xkbcomp, since `SA_LockPtrBtn` has no `count` parameter.
         * See: https://xorg.freedesktop.org/archive/current/doc/kbproto/xkbproto.html#:~:text=SA_LockPtrBtn
         */
        ਜੇ (action->type != ACTION_TYPE_PTR_ਤਾਲਾ)
            act->ਗਿਣਤੀ_2 = (uint8_t) val;
        ਵਾਪਸ PARSER_SUCCESS;
    }

    ਵਾਪਸ ReportIllegal(ctx, action->type, ਖੇਤਰ_2, keymap_info->strict);
}

ਗਿਣਤੀ {
    ਸੈੱਟ_ਸੂਚਕ_DEFAULT_BUTTON_AFFECT_DEFAULT = 0,
    ਸੈੱਟ_ਸੂਚਕ_DEFAULT_BUTTON_AFFECT_ਅੰਕ_ENTRIES
};

ਸਥਿਰ ਅਚਲ Lookup_ਇੰਦਰਾਜ ptrDflts[] = {
    { "dfltbtn", ਸੈੱਟ_ਸੂਚਕ_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "defaultbutton", ਸੈੱਟ_ਸੂਚਕ_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { "button", ਸੈੱਟ_ਸੂਚਕ_DEFAULT_BUTTON_AFFECT_DEFAULT },
    { ਨਿਰਾ, 0 }
};

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
Handle_ਸੈੱਟ_Ptr_Dflt(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                 ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                 ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                 ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
                 ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ਅਚਲ ctx = keymap_info->keymap.ctx;
    ਬਣਤਰ xkb_ਸੂਚਕ_default_action *act = &action->dflt;

    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_AFFECT) {
        uint32_t val = 0;

        ਜੇ (array_ndx)
            ਵਾਪਸ ReportActionNotArray(ctx, action->type, ਖੇਤਰ_2,
                                        keymap_info->strict);

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Enum(ctx, ਮੁੱਲ, &val, ptrDflts))
            ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, action->type,
                                  ਖੇਤਰ_2, "pointer component", keymap_info->strict);
        static_assert(ਸੈੱਟ_ਸੂਚਕ_DEFAULT_BUTTON_AFFECT_ਅੰਕ_ENTRIES == 1,
                      "affect has a single valid value and thus it is not stored");
        ਵਾਪਸ PARSER_SUCCESS;
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_BUTTON || ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_ਮੁੱਲ) {
        ਅਚਲ ਵਿਅੰਜਕ_Def *button;
        int64_t btn = 0;

        ਜੇ (array_ndx)
            ਵਾਪਸ ReportActionNotArray(ctx, action->type, ਖੇਤਰ_2,
                                        keymap_info->strict);

        ਜੇ (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_NEGATE ||
            ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            button = ਮੁੱਲ->unary.ਬੱਚਾ;
        }
        ਨਹੀਂਤਾਂ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            button = ਮੁੱਲ;
        }

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Button(ctx, button, &btn)) {
            static_assert(XKB_ਸੂਚਕ_BUTTON_MIN == 1 &&
                          XKB_ਸੂਚਕ_BUTTON_MAX == 5, "invalid message");
            ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, action->type,
                                  ਖੇਤਰ_2, "integer (range 1..5)",
                                  keymap_info->strict);
        }

        static_assert(XKB_ਸੂਚਕ_BUTTON_DEFAULT < XKB_ਸੂਚਕ_BUTTON_MIN, "");
        ਜੇ (btn < XKB_ਸੂਚਕ_BUTTON_DEFAULT || btn > XKB_ਸੂਚਕ_BUTTON_MAX) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "New default button value must be in the range %d..%d; "
                    "Illegal default button value %"PRId64" ignored\n",
                    XKB_ਸੂਚਕ_BUTTON_MIN, XKB_ਸੂਚਕ_BUTTON_MAX, btn);
            ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        ਜੇ (btn == XKB_ਸੂਚਕ_BUTTON_DEFAULT) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Cannot set default pointer button to \"default\"; "
                    "Illegal default button setting ignored\n");
            ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->ਮੁੱਲ = (int8_t) (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_NEGATE ? -btn: btn);
        ਵਾਪਸ PARSER_SUCCESS;
    }

    ਵਾਪਸ ReportIllegal(ctx, action->type, ਖੇਤਰ_2, keymap_info->strict);
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
HandleSwitchScreen(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                   ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                   ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                   ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
                   ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ਅਚਲ ctx = keymap_info->keymap.ctx;
    ਬਣਤਰ xkb_switch_screen_action *act = &action->screen;

    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_SCREEN) {
        ਅਚਲ ਵਿਅੰਜਕ_Def *scrn;
        int64_t val = 0;

        ਜੇ (array_ndx)
            ਵਾਪਸ ReportActionNotArray(ctx, action->type, ਖੇਤਰ_2,
                                        keymap_info->strict);

        ਜੇ (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_NEGATE ||
            ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_UNARY_PLUS) {
            act->flags &= ~ACTION_ABSOLUTE_SWITCH;
            scrn = ਮੁੱਲ->unary.ਬੱਚਾ;
        }
        ਨਹੀਂਤਾਂ {
            act->flags |= ACTION_ABSOLUTE_SWITCH;
            scrn = ਮੁੱਲ;
        }

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer(ctx, scrn, &val))
            ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, action->type,
                                  ਖੇਤਰ_2, "integer (-128..127)",
                                  keymap_info->strict);

        val = (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_NEGATE ? -val : val);
        ਜੇ (val < INT8_MIN || val > INT8_MAX) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Screen index must be in the range %d..%d; "
                    "Illegal screen value %"PRId64" ignored\n",
                    INT8_MIN, INT8_MAX, val);
            ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }

        act->screen = (int8_t) val;
        ਵਾਪਸ PARSER_SUCCESS;
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_SAME) {
        ਵਾਪਸ CheckBooleanFlag(ctx, keymap_info->strict, action->type, ਖੇਤਰ_2,
                                ACTION_SAME_SCREEN, array_ndx, ਮੁੱਲ,
                                &act->flags);
    }

    ਵਾਪਸ ReportIllegal(ctx, action->type, ਖੇਤਰ_2, keymap_info->strict);
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
Handle_ਸੈੱਟ_ਤਾਲਾ_Controls(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                      ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                      ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                      ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
                      ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ਅਚਲ ctx = keymap_info->keymap.ctx;
    ਬਣਤਰ xkb_controls_action *act = &action->ctrls;

    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_CONTROLS) {
        ਜੇ (array_ndx)
            ਵਾਪਸ ReportActionNotArray(ctx, action->type, ਖੇਤਰ_2,
                                        keymap_info->strict);

        uint32_t ਨਕਾਬ = 0;
        ਅਚਲ uint8_t ਵਿੱਥ = keymap_info->features.controls_name_ਵਿੱਥ;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_ਨਕਾਬ(ctx, ਮੁੱਲ, &ਨਕਾਬ, ctrl_ਨਕਾਬ_ਨਾਂ + ਵਿੱਥ))
            ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, action->type,
                                  ਖੇਤਰ_2, "controls mask", keymap_info->strict);

        act->ctrls = (ਗਿਣਤੀ xkb_action_controls) ਨਕਾਬ;
        ਵਾਪਸ PARSER_SUCCESS;
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_AFFECT &&
             action->type == ACTION_TYPE_CTRL_ਤਾਲਾ) {
        ਵਾਪਸ Check_Affect_ਖੇਤਰ(ctx, keymap_info->strict, action->type,
                                array_ndx, ਮੁੱਲ, &act->flags);
    }

    ਵਾਪਸ ReportIllegal(ctx, action->type, ਖੇਤਰ_2, keymap_info->strict);
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
Handle_Redirect_ਕੁੰਜੀ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                  ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                  ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                  ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
                  ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਅਚਲ ਬਣਤਰ xkb_keymap * ਸੀਮਿਤ ਅਚਲ keymap = &keymap_info->keymap;
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ਅਚਲ ctx = keymap->ctx;
    ਬਣਤਰ xkb_redirect_ਕੁੰਜੀ_action * ਅਚਲ act = &action->redirect;

    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_KEYCODE) {
        ਜੇ (array_ndx)
            ਵਾਪਸ ReportActionNotArray(ctx, action->type, ਖੇਤਰ_2,
                                        keymap_info->strict);
        ਜੇ (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_IDENT) {
            ਅਚਲ ਅੱਖਰ *valStr = xkb_atom_ਪਾਠ(ctx, ਮੁੱਲ->ident.ident);
            ਜੇ (valStr && istreq(valStr, "auto")) {
                act->keycode = keymap_info->keymap.redirect_ਕੁੰਜੀ_auto;
                ਵਾਪਸ PARSER_SUCCESS;
            }
        }
        ਜੇ (ਮੁੱਲ->common.type != STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL) {
            ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, action->type,
                                  ਖੇਤਰ_2, "key name", keymap_info->strict);
        }
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਅਚਲ ਕੁੰਜੀ =
            Xkb_ਕੁੰਜੀ_By_Name(keymap, ਮੁੱਲ->ਕੁੰਜੀ_name.ਕੁੰਜੀ_name, true);
        ਜੇ (ਕੁੰਜੀ == ਨਿਰਾ) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "RedirectKey field %s cannot resolve %s to a valid key\n",
                    ਖੇਤਰ_ਪਾਠ(ਖੇਤਰ_2), ਕੁੰਜੀ_Name_ਪਾਠ(ctx, ਮੁੱਲ->ਕੁੰਜੀ_name.ਕੁੰਜੀ_name));
            ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_ਮੁੱਲ_MISMATCH)
                ? PARSER_FATAL_ERROR
                : PARSER_RECOVERABLE_ERROR;
        }
        act->keycode = ਕੁੰਜੀ->keycode;
        ਵਾਪਸ PARSER_SUCCESS;
    }

    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_MODIFIERS || ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_MODS_ਮੰਜ਼ਿਲ_CLEAR) {
        ਗਿਣਤੀ xkb_action_flags flags = 0;
        xkb_mod_ਨਕਾਬ_t m = 0;
        ਗਿਣਤੀ xkb_parser_error r =
            Check_Modifier_ਖੇਤਰ(ctx, keymap_info->strict, mods, action->type,
                               array_ndx, ਮੁੱਲ, &flags, &m);
        ਜੇ (r != PARSER_SUCCESS)
            ਵਾਪਸ r;
        ਜੇ (flags)
            ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, action->type,
                                  ਖੇਤਰ_2, "modifier mask", keymap_info->strict);
        act->affect_mods |= m;
        ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_MODIFIERS)
            act->mods |= m;
        ਨਹੀਂਤਾਂ
            act->mods &= ~m;
        ਵਾਪਸ PARSER_SUCCESS;
    }

    ਵਾਪਸ ReportIllegal(ctx, action->type, ਖੇਤਰ_2, keymap_info->strict);
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
HandleUnsupported(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                        ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                        ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                        ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
                        ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)

{
    /* Do not check any field */
    ਵਾਪਸ PARSER_SUCCESS;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_parser_error
Handle_ਨਿੱਜੀ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
              ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
              ਸੰਘ xkb_action *action, ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
              ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx, ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
              ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ਅਚਲ ctx = keymap_info->keymap.ctx;
    ਬਣਤਰ xkb_ਨਿੱਜੀ_action *act = &action->priv;

    ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_TYPE) {
        int64_t type = 0;

        ਜੇ (array_ndx)
            ਵਾਪਸ ReportActionNotArray(ctx, action->type, ਖੇਤਰ_2,
                                        keymap_info->strict);

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer(ctx, ਮੁੱਲ, &type))
            ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                                  ACTION_TYPE_ਨਿੱਜੀ, ਖੇਤਰ_2, "integer",
                                  keymap_info->strict);

        ਜੇ (type < 0 || type > 255) {
            log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Private action type must be in the range 0..255; "
                    "Illegal type %"PRId64" ignored\n", type);
            ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
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
        ਜੇ (type < ACTION_TYPE_ਨਿੱਜੀ) {
            log_info(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Private actions of type %s are not supported; Ignored\n",
                     Action_Type_ਪਾਠ((ਗਿਣਤੀ xkb_action_type) type));
            act->type = ACTION_TYPE_NONE;
        }
        ਨਹੀਂਤਾਂ {
            act->type = (ਗਿਣਤੀ xkb_action_type) type;
        }

        ਵਾਪਸ PARSER_SUCCESS;
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਖੇਤਰ_2 == ACTION_ਖੇਤਰ_ਅੰਕੜੇ) {
        ਜੇ (array_ndx == ਨਿਰਾ) {
            xkb_atom_t val = XKB_ATOM_NONE;

            ਜੇ (!ਵਿਅੰਜਕ_Resolve_String(ctx, ਮੁੱਲ, &val))
                ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                                      action->type, ਖੇਤਰ_2, "string",
                                      keymap_info->strict);

            ਅਚਲ ਅੱਖਰ *str = xkb_atom_ਪਾਠ(ctx, val);
            ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(str);
            ਜੇ (len < 1 || len > ਆਕਾਰ(act->ਅੰਕੜੇ)) {
                log_warn(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "A private action has %zu data bytes, but got: %zu; "
                         "Illegal data ignored\n", ਆਕਾਰ(act->ਅੰਕੜੇ), len);
                ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            /* act->data may not be null-terminated, this is intentional */
            memset(act->ਅੰਕੜੇ, 0, ਆਕਾਰ(act->ਅੰਕੜੇ));
            ਸਮ੍ਰਿਤੀ_ਨਕਲ(act->ਅੰਕੜੇ, str, len);
            ਵਾਪਸ PARSER_SUCCESS;
        }
        ਨਹੀਂਤਾਂ {
            int64_t ndx = 0, datum = 0;

            ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer(ctx, array_ndx, &ndx)) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Array subscript must be integer; "
                        "Illegal subscript ignored\n");
                ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ਜੇ (ndx < 0 || (ਮਾਪ_t) ndx >= ਆਕਾਰ(act->ਅੰਕੜੇ)) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "The data for a private action is %zu bytes long; "
                        "Attempt to use data[%"PRId64"] ignored\n",
                        ਆਕਾਰ(act->ਅੰਕੜੇ), ndx);
                ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer(ctx, ਮੁੱਲ, &datum))
                ਵਾਪਸ ReportMismatch(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, act->type,
                                      ਖੇਤਰ_2, "integer", keymap_info->strict);

            ਜੇ (datum < 0 || datum > 255) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "All data for a private action must be 0..255; "
                        "Illegal datum %"PRId64" ignored\n", datum);
                ਵਾਪਸ (keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH)
                    ? PARSER_FATAL_ERROR
                    : PARSER_RECOVERABLE_ERROR;
            }

            act->ਅੰਕੜੇ[ndx] = (uint8_t) datum;
            ਵਾਪਸ PARSER_SUCCESS;
        }
    }

    ਵਾਪਸ ReportIllegal(ctx, action->type, ਖੇਤਰ_2, keymap_info->strict);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਗਿਣਤੀ xkb_parser_error
        (*actionHandler)(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                         ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                         ਸੰਘ xkb_action *action,
                         ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_2,
                         ਅਚਲ ਵਿਅੰਜਕ_Def *array_ndx,
                         ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
                         ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr);

ਸਥਿਰ ਅਚਲ actionHandler handleAction[ACTION_TYPE_ਅੰਕ_ENTRIES] = {
    [ACTION_TYPE_NONE] = HandleNoAction,
    [ACTION_TYPE_VOID] = HandleNoAction,
    [ACTION_TYPE_MOD_ਸੈੱਟ] = Handle_ਸੈੱਟ_Latch_ਤਾਲਾ_Mods,
    [ACTION_TYPE_MOD_LATCH] = Handle_ਸੈੱਟ_Latch_ਤਾਲਾ_Mods,
    [ACTION_TYPE_MOD_ਤਾਲਾ] = Handle_ਸੈੱਟ_Latch_ਤਾਲਾ_Mods,
    [ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ] = Handle_ਸੈੱਟ_Latch_ਤਾਲਾ_ਸਮੂਹ,
    [ACTION_TYPE_ਸਮੂਹ_LATCH] = Handle_ਸੈੱਟ_Latch_ਤਾਲਾ_ਸਮੂਹ,
    [ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ] = Handle_ਸੈੱਟ_Latch_ਤਾਲਾ_ਸਮੂਹ,
    [ACTION_TYPE_PTR_MOVE] = HandleMovePtr,
    [ACTION_TYPE_PTR_BUTTON] = HandlePtrBtn,
    [ACTION_TYPE_PTR_ਤਾਲਾ] = HandlePtrBtn,
    [ACTION_TYPE_PTR_DEFAULT] = Handle_ਸੈੱਟ_Ptr_Dflt,
    [ACTION_TYPE_TERMINATE] = HandleNoAction,
    [ACTION_TYPE_SWITCH_VT] = HandleSwitchScreen,
    [ACTION_TYPE_CTRL_ਸੈੱਟ] = Handle_ਸੈੱਟ_ਤਾਲਾ_Controls,
    [ACTION_TYPE_CTRL_ਤਾਲਾ] = Handle_ਸੈੱਟ_ਤਾਲਾ_Controls,
    [ACTION_TYPE_REDIRECT_ਕੁੰਜੀ] = Handle_Redirect_ਕੁੰਜੀ,
    [ACTION_TYPE_UNSUPPORTED_LEGACY] = HandleUnsupported,
    [ACTION_TYPE_UNKNOWN] = HandleUnsupported,
    [ACTION_TYPE_ਨਿੱਜੀ] = Handle_ਨਿੱਜੀ,
};

/* Ensure to not miss `xkb_action_type` updates */
static_assert(ACTION_TYPE_ਅੰਦਰੂਨੀ == 20 &&
              ACTION_TYPE_ਅੰਦਰੂਨੀ + 1 == ACTION_TYPE_ਅੰਕ_ENTRIES,
              "Missing action type");

/***====================================================================***/

ਗਿਣਤੀ xkb_parser_error
HandleActionDef(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info, ActionsInfo *info,
                ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, ਵਿਅੰਜਕ_Def *def,
                ਸੰਘ xkb_action *action)
{
    ਬਣਤਰ xkb_context * ਸੀਮਿਤ ਅਚਲ ctx = keymap_info->keymap.ctx;
    ਜੇ (def->common.type != STMT_ਵਿਅੰਜਕ_ACTION_DECL) {
        log_err(ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Expected an action definition, found %s\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(def->common.type));
        ਵਾਪਸ PARSER_FATAL_ERROR;
    }

    ਅਚਲ ਅੱਖਰ *action_name = xkb_atom_ਪਾਠ(ctx, def->action.name);
    ਗਿਣਤੀ xkb_action_type handler_type;
    ਜੇ (!string_ਮੰਜ਼ਿਲ_Action_Type(action_name, &handler_type)) {
        log_err(ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", action_name);
        handler_type = ACTION_TYPE_UNKNOWN;
        ਜੇ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ਵਾਪਸ PARSER_FATAL_ERROR;
    }

    /*
     * Get the default values for this action type, as modified by
     * statements such as:
     *     latchMods.clearLocks = True;
     */
    *action = info->actions[handler_type];

    ਜੇ (handler_type == ACTION_TYPE_UNSUPPORTED_LEGACY) {
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
    ਗਿਣਤੀ xkb_parser_error ret = PARSER_SUCCESS;
    ਲਈ (ਵਿਅੰਜਕ_Def *arg = def->action.ਦਲੀਲਾਂ; arg != ਨਿਰਾ;
         arg = (ਵਿਅੰਜਕ_Def *) arg->common.ਅਗਲਾ) {
        ਅਚਲ ਵਿਅੰਜਕ_Def *ਮੁੱਲ;
        ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr = ਨਿਰਾ;
        ਵਿਅੰਜਕ_Def *ਖੇਤਰ_2, *arrayRtrn;
        ਅਚਲ ਅੱਖਰ *elemRtrn, *ਖੇਤਰ_Rtrn;

        ਜੇ (arg->common.type == STMT_ਵਿਅੰਜਕ_ਸੌਂਪੋ) {
            ਖੇਤਰ_2 = arg->ਦੁਆਧਾਰੀ.ਖੱਬਾ;
            ਮੁੱਲ = arg->ਦੁਆਧਾਰੀ.ਸੱਜਾ;
            ਮੁੱਲ_ptr = &arg->ਦੁਆਧਾਰੀ.ਸੱਜਾ;
        }
        ਨਹੀਂਤਾਂ ਜੇ (arg->common.type == STMT_ਵਿਅੰਜਕ_NOT ||
                 arg->common.type == STMT_ਵਿਅੰਜਕ_INVERT) {
            ਖੇਤਰ_2 = arg->unary.ਬੱਚਾ;
            ਮੁੱਲ = (ਅਚਲ ਵਿਅੰਜਕ_Def *) &constFalse;
        }
        ਨਹੀਂਤਾਂ {
            ਖੇਤਰ_2 = arg;
            ਮੁੱਲ = (ਅਚਲ ਵਿਅੰਜਕ_Def *) &constTrue;
        }

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Lhs(ctx, ਖੇਤਰ_2, &elemRtrn, &ਖੇਤਰ_Rtrn, &arrayRtrn))
            ਵਾਪਸ PARSER_FATAL_ERROR;

        ਜੇ (elemRtrn) {
            log_err(ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                    "Cannot change defaults in an action definition; "
                    "Ignoring attempt to change \"%s.%s\".\n",
                    elemRtrn, ਖੇਤਰ_Rtrn);
            ਵਾਪਸ PARSER_FATAL_ERROR;
        }

        ਗਿਣਤੀ action_ਖੇਤਰ ਖੇਤਰ_Ndx;
        ਜੇ (!string_ਮੰਜ਼ਿਲ_ਖੇਤਰ(ਖੇਤਰ_Rtrn, &ਖੇਤਰ_Ndx)) {
            log_err(ctx, XKB_ERROR_INVALID_ACTION_ਖੇਤਰ,
                    "Unknown field name %s for action %s discarded\n",
                    ਖੇਤਰ_Rtrn, Action_Type_ਪਾਠ(action->type));
            ਜੇ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS) {
                ਵਾਪਸ PARSER_FATAL_ERROR;
            } ਨਹੀਂਤਾਂ {
                /* Field is just ignored */
                ਜਾਰੀ;
            }
        }

        ਚੋਣ (handleAction[handler_type](keymap_info, mods, action, ਖੇਤਰ_Ndx,
                                           arrayRtrn, ਮੁੱਲ, ਮੁੱਲ_ptr)) {
        ਹਾਲਤ PARSER_FATAL_ERROR:
            ਵਾਪਸ PARSER_FATAL_ERROR;
        ਹਾਲਤ PARSER_RECOVERABLE_ERROR:
            ret = PARSER_RECOVERABLE_ERROR;
            /* Continue field parsing */
            ਰੋਕੋ;
        ਮੂਲ:
            ;
        }
    }

    ਵਾਪਸ (action->type == ACTION_TYPE_UNKNOWN)
        ? PARSER_RECOVERABLE_ERROR
        : ret;
}

ਗਿਣਤੀ xkb_parser_error
ਸੈੱਟ_Default_Action_ਖੇਤਰ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                      ActionsInfo *info, ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
                      ਅਚਲ ਅੱਖਰ *elem, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2, ਵਿਅੰਜਕ_Def *array_ndx,
                      ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr, ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ)
{
    ਅਚਲ ਵਿਅੰਜਕ_Def * ਸੀਮਿਤ ਅਚਲ ਮੁੱਲ = *ਮੁੱਲ_ptr;
    ਗਿਣਤੀ xkb_action_type action;
    ਜੇ (!string_ਮੰਜ਼ਿਲ_Action_Type(elem, &action)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNKNOWN_ACTION_TYPE,
                "Unknown action \"%s\"\n", elem);
        ਵਾਪਸ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ਗਿਣਤੀ action_ਖੇਤਰ action_ਖੇਤਰ;
    ਜੇ (!string_ਮੰਜ਼ਿਲ_ਖੇਤਰ(ਖੇਤਰ_2, &action_ਖੇਤਰ)) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ACTION_ਖੇਤਰ,
                "Unknown action field \"%s\"\n", ਖੇਤਰ_2);
        ਵਾਪਸ (keymap_info->strict & PARSER_NO_UNKNOWN_ACTION_FIELDS)
            ? PARSER_FATAL_ERROR
            : PARSER_RECOVERABLE_ERROR;
    }

    ਸੰਘ xkb_action* ਅਚਲ into = &info->actions[action];
    /* Initialize with current defaults to enable comparison, see afterwards */
    ਸੰਘ xkb_action from = *into;

    /* Parse action */
    ਅਚਲ ਗਿਣਤੀ xkb_parser_error ret = handleAction[action](
        keymap_info, mods, &from, action_ਖੇਤਰ, array_ndx, ਮੁੱਲ, ਮੁੱਲ_ptr
    );

    /* Success is mandatory to update defaults */
    ਜੇ (ret != PARSER_SUCCESS)
        ਵਾਪਸ ret;

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
    ਜੇ (!action_equal(into, &from)) {
        ਅਚਲ bool replace = (ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);
        log_vrb(keymap_info->keymap.ctx, XKB_LOG_VERBOSITY_VERBOSE,
                XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Conflicting field \"%s\" for default action \"%s\"; "
                "Using %s, ignore %s\n",
                ਖੇਤਰ_ਪਾਠ(action_ਖੇਤਰ), Action_Type_ਪਾਠ(action),
                (replace ? "from" : "into"), (replace ? "into" : "from"));
        ਜੇ (replace)
            *into = from;
    }
    ਵਾਪਸ PARSER_SUCCESS;
}
