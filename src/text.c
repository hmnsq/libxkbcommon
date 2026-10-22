/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "keysym.h"
#ಅನ್ತರ್ಭಾವ "text.h"

bool
LookupString(ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು tab[], ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *string,
              ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *ಮೌಲ್ಯ_rtrn)
{
    ಆದರೆ (!string)
        ಹಿಂತಿರುಗಿ false;

    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *ನಮೂದು = tab; ನಮೂದು->name; ನಮೂದು++) {
        ಆದರೆ (istreq(ನಮೂದು->name, string)) {
            *ಮೌಲ್ಯ_rtrn = ನಮೂದು->ಮೌಲ್ಯ;
            ಹಿಂತಿರುಗಿ true;
        }
    }

    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Lookup_ಮೌಲ್ಯ(ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು tab[], ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ)
{
    ಗಾಗಿ (ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *ನಮೂದು = tab; ನಮೂದು->name; ನಮೂದು++)
        ಆದರೆ (ನಮೂದು->ಮೌಲ್ಯ == ಮೌಲ್ಯ)
            ಹಿಂತಿರುಗಿ ನಮೂದು->name;

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು ctrl_ಮುಖವಾಡ_ಹೆಸರುಗಳು[25] = {
    /* v2 keymap format only */
    { "Overlay3", CONTROL_OVERLAY3 },
    { "Overlay4", CONTROL_OVERLAY4 },
    { "Overlay5", CONTROL_OVERLAY5 },
    { "Overlay6", CONTROL_OVERLAY6 },
    { "Overlay7", CONTROL_OVERLAY7 },
    { "Overlay8", CONTROL_OVERLAY8 },
    { "all", CONTROL_ALL_BOOLEAN },
    /* v1 + v2 keymap formats */
    [CONTROL_ಹೆಸರುಗಳು_MIN_V1_ಸೂಚ್ಯಂಕ] = { "RepeatKeys", CONTROL_REPEAT },
    { "Repeat", CONTROL_REPEAT },
    { "AutoRepeat", CONTROL_REPEAT },
    { "SlowKeys", CONTROL_SLOW },
    { "BounceKeys", CONTROL_DEBOUNCE },
    { "StickyKeys", CONTROL_STICKY_ಕೀಲಿಗಳು },
    { "MouseKeys", CONTROL_MOUSE_ಕೀಲಿಗಳು },
    { "MouseKeysAccel", CONTROL_MOUSE_ಕೀಲಿಗಳು_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_ಗುಂಪು_ಬೀಗ },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { ಖಾಲಿ, 0 }
};

ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು mod_Component_ಮುಖವಾಡ_ಹೆಸರುಗಳು[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { ಖಾಲಿ, 0 }
};

ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು ಗುಂಪು_Component_ಮುಖವಾಡ_ಹೆಸರುಗಳು[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { ಖಾಲಿ, 0 }
};

ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು ಗುಂಪು_ಮುಖವಾಡ_ಹೆಸರುಗಳು[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { ಖಾಲಿ, 0 }
};

ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು button_ಹೆಸರುಗಳು[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { ಖಾಲಿ, 0 }
};

ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು use_Mod_Map_ಮೌಲ್ಯ_ಹೆಸರುಗಳು[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { ಖಾಲಿ, 0 }
};

ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು action_Type_ಹೆಸರುಗಳು[] = {
    { "NoAction", ACTION_TYPE_NONE },
    { "VoidAction", ACTION_TYPE_VOID },
    { "SetMods", ACTION_TYPE_MOD_ಗಣ },
    { "LatchMods", ACTION_TYPE_MOD_LATCH },
    { "LockMods", ACTION_TYPE_MOD_ಬೀಗ },
    { "SetGroup", ACTION_TYPE_ಗುಂಪು_ಗಣ },
    { "LatchGroup", ACTION_TYPE_ಗುಂಪು_LATCH },
    { "LockGroup", ACTION_TYPE_ಗುಂಪು_ಬೀಗ },
    { "MovePtr", ACTION_TYPE_PTR_MOVE },
    { "MovePointer", ACTION_TYPE_PTR_MOVE },
    { "PtrBtn", ACTION_TYPE_PTR_BUTTON },
    { "PointerButton", ACTION_TYPE_PTR_BUTTON },
    { "LockPtrBtn", ACTION_TYPE_PTR_ಬೀಗ },
    { "LockPtrButton", ACTION_TYPE_PTR_ಬೀಗ },
    { "LockPointerButton", ACTION_TYPE_PTR_ಬೀಗ },
    { "LockPointerBtn", ACTION_TYPE_PTR_ಬೀಗ },
    { "SetPtrDflt", ACTION_TYPE_PTR_DEFAULT },
    { "SetPointerDefault", ACTION_TYPE_PTR_DEFAULT },
    { "Terminate", ACTION_TYPE_TERMINATE },
    { "TerminateServer", ACTION_TYPE_TERMINATE },
    { "SwitchScreen", ACTION_TYPE_SWITCH_VT },
    { "SetControls", ACTION_TYPE_CTRL_ಗಣ },
    { "LockControls", ACTION_TYPE_CTRL_ಬೀಗ },
    { "RedirectKey", ACTION_TYPE_REDIRECT_ಕೀಲಿ },
    { "Redirect", ACTION_TYPE_REDIRECT_ಕೀಲಿ },
    { "Private", ACTION_TYPE_ಖಾಸಗಿ },
    /* deprecated actions below here - unused */
    { "ISOLock", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "ActionMessage", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "MessageAction", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "Message", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DeviceBtn", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DevBtn", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DevButton", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DeviceButton", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "LockDeviceBtn", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "LockDevBtn", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "LockDevButton", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "LockDeviceButton", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DeviceValuator", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DevVal", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DeviceVal", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { "DevValuator", ACTION_TYPE_UNSUPPORTED_LEGACY },
    { ಖಾಲಿ, 0 },
};

ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು sym_Interpret_ಹೊಂದಿಕೆ_ಮುಖವಾಡ_ಹೆಸರುಗಳು[] = {
    { "NoneOf", ಹೊಂದಿಕೆ_NONE },
    { "AnyOfOrNone", ಹೊಂದಿಕೆ_ANY_OR_NONE },
    { "AnyOf", ಹೊಂದಿಕೆ_ANY },
    { "AllOf", ಹೊಂದಿಕೆ_ALL },
    { "Exactly", ಹೊಂದಿಕೆ_EXACTLY },
    { ಖಾಲಿ, 0 },
};

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Mod_ಸೂಚ್ಯಂಕ_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods,
             xkb_mod_ಸೂಚ್ಯಂಕ_t ndx)
{
    ಆದರೆ (ndx == XKB_MOD_INVALID)
        ಹಿಂತಿರುಗಿ "none";

    ಆದರೆ (ndx == XKB_MOD_NONE)
        ಹಿಂತಿರುಗಿ "None";

    ಆದರೆ (ndx >= mods->ಅಂಕೆ_mods)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    ಹಿಂತಿರುಗಿ xkb_atom_ಪಠ್ಯ(ctx, mods->mods[ndx].name);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Action_Type_ಪಠ್ಯ(ಎಣಿಕೆ xkb_action_type type)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = Lookup_ಮೌಲ್ಯ(action_Type_ಹೆಸರುಗಳು, type);
    ಹಿಂತಿರುಗಿ name ? name : "Private";
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Keysym_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, xkb_keysym_t sym)
{
    ಅಕ್ಷರ *ಬಫರ್ = xkb_context_get_ಬಫರ್(ctx, XKB_KEYSYM_NAME_MAX_ಗಾತ್ರ);
    xkb_keysym_get_name(sym, ಬಫರ್, XKB_KEYSYM_NAME_MAX_ಗಾತ್ರ);
    ಹಿಂತಿರುಗಿ ಬಫರ್;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಕೀಲಿ_Name_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, xkb_atom_t name)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *sname = xkb_atom_ಪಠ್ಯ(ctx, name);
    ಗಾತ್ರ_t len = strlen_ಸುರಕ್ಷಿತ(sname) + 3;
    ಅಕ್ಷರ *buf = xkb_context_get_ಬಫರ್(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    ಹಿಂತಿರುಗಿ buf;
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
SIMatch_ಪಠ್ಯ(ಎಣಿಕೆ xkb_ಹೊಂದಿಕೆ_operation type)
{
    ಹಿಂತಿರುಗಿ Lookup_ಮೌಲ್ಯ(sym_Interpret_ಹೊಂದಿಕೆ_ಮುಖವಾಡ_ಹೆಸರುಗಳು, type);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Mod_ಮುಖವಾಡ_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ mod_type type,
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods, xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ)
{
    ಅಕ್ಷರ buf[1024] = {0};
    ಗಾತ್ರ_t pos = 0;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    ಪ್ರತಿಜ್ಞಾ(type == MOD_REAL || type == MOD_BOTH);

    ಆದರೆ (ಮುಖವಾಡ == 0)
        ಹಿಂತಿರುಗಿ "none";

    ಆದರೆ (ಮುಖವಾಡ == MOD_REAL_ಮುಖವಾಡ_ALL)
        ಹಿಂತಿರುಗಿ "all";

    ಆದರೆ ((type == MOD_REAL && (ಮುಖವಾಡ & ~MOD_REAL_ಮುಖವಾಡ_ALL)) ||
        unlikely(ಮುಖವಾಡ & ~((UINT64_C(1) << mods->ಅಂಕೆ_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ret = snprintf(buf, ಗಾತ್ರ(buf), "0x%"PRIx32, ಮುಖವಾಡ);
        static_assert(ಗಾತ್ರ(ಮುಖವಾಡ) == 4 && ಗಾತ್ರ(buf) >= ಗಾತ್ರ("0xffffffff"),
                      "Buffer too small");
        ಪ್ರತಿಜ್ಞಾ(ret >= 0 && (ಗಾತ್ರ_t) ret < ಗಾತ್ರ(buf));
        pos = (ಗಾತ್ರ_t) ret;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Print known mods */
        xkb_mods_ಮುಖವಾಡ_foreach(ಮುಖವಾಡ, mod, mods) {
            ಪೂರ್ಣಾಂಕ ret = snprintf(buf + pos, ಗಾತ್ರ(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_ಪಠ್ಯ(ctx, mod->name));
            ಆದರೆ (ret <= 0 || pos + (ಗಾತ್ರ_t)ret >= ಗಾತ್ರ(buf))
                ನಿಲ್ಲಿಸು;
            ಇಲ್ಲದಿದ್ದರೆ
                pos += (ಗಾತ್ರ_t)ret;
        }
    }
    pos++;

    ಹಿಂತಿರುಗಿ ಸ್ಮರಣೆ_ನಕಲು(xkb_context_get_ಬಫರ್(ctx, pos), buf, pos);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Led_State_ಮುಖವಾಡ_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು *lookup,
                 ಎಣಿಕೆ xkb_state_component ಮುಖವಾಡ)
{
    ಅಕ್ಷರ buf[1024];
    ಗಾತ್ರ_t pos = 0;

    ಆದರೆ (ಮುಖವಾಡ == 0)
        ಹಿಂತಿರುಗಿ "0";

    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; ಮುಖವಾಡ; i++) {
        ಪೂರ್ಣಾಂಕ ret;

        ಆದರೆ (!(ಮುಖವಾಡ & (1u << i)))
            ಮುಂದುವರಿ;

        ಮುಖವಾಡ &= ~(1u << i);

        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ ಮುಖವಾಡ_ಪಠ್ಯ = Lookup_ಮೌಲ್ಯ(lookup, 1u << i);
        ಪ್ರತಿಜ್ಞಾ(ಮುಖವಾಡ_ಪಠ್ಯ != ಖಾಲಿ);
        ret = snprintf(buf + pos, ಗಾತ್ರ(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", ಮುಖವಾಡ_ಪಠ್ಯ);
        ಆದರೆ (ret <= 0 || pos + (ಗಾತ್ರ_t)ret >= ಗಾತ್ರ(buf))
            ನಿಲ್ಲಿಸು;
        ಇಲ್ಲದಿದ್ದರೆ
            pos += (ಗಾತ್ರ_t)ret;
    }
    pos++;

    ಹಿಂತಿರುಗಿ ಸ್ಮರಣೆ_ನಕಲು(xkb_context_get_ಬಫರ್(ctx, pos), buf, pos);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
Control_ಮುಖವಾಡ_ಪಠ್ಯ(ರಚನೆ xkb_context *ctx, ಎಣಿಕೆ xkb_keymap_format format,
                ಎಣಿಕೆ xkb_action_controls ಮುಖವಾಡ)
{
    ಅಕ್ಷರ buf[1024];
    ಗಾತ್ರ_t pos = 0;

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_action_controls all_ctrls = format_boolean_controls(format);
    ಮುಖವಾಡ &= all_ctrls;

    ಆದರೆ (ಮುಖವಾಡ == 0)
        ಹಿಂತಿರುಗಿ "none";

    ಆದರೆ (ಮುಖವಾಡ == all_ctrls)
        ಹಿಂತಿರುಗಿ "all";

    ಸ್ಥಿರಾಂಕ uint8_t control_ಹೆಸರುಗಳು_ಸ್ಥಾನಾಂತರ = format_control_ಹೆಸರುಗಳು_ಸ್ಥಾನಾಂತರ(format);
    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i = 0; ಮುಖವಾಡ; i++) {
        ಪೂರ್ಣಾಂಕ ret;

        ಆದರೆ (!(ಮುಖವಾಡ & (1u << i)))
            ಮುಂದುವರಿ;

        ಮುಖವಾಡ &= ~(1u << i);

        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ ಮುಖವಾಡ_ಪಠ್ಯ =
            Lookup_ಮೌಲ್ಯ(ctrl_ಮುಖವಾಡ_ಹೆಸರುಗಳು + control_ಹೆಸರುಗಳು_ಸ್ಥಾನಾಂತರ, 1u << i);
        ಪ್ರತಿಜ್ಞಾ(ಮುಖವಾಡ_ಪಠ್ಯ);
        ret = snprintf(buf + pos, ಗಾತ್ರ(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", ಮುಖವಾಡ_ಪಠ್ಯ);
        ಆದರೆ (ret <= 0 || pos + (ಗಾತ್ರ_t)ret >= ಗಾತ್ರ(buf))
            ನಿಲ್ಲಿಸು;
        ಇಲ್ಲದಿದ್ದರೆ
            pos += (ಗಾತ್ರ_t)ret;
    }
    pos++;

    ಹಿಂತಿರುಗಿ ಸ್ಮರಣೆ_ನಕಲು(xkb_context_get_ಬಫರ್(ctx, pos), buf, pos);
}
