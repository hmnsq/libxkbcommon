/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keysym.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"

bool
LookupString(ਅਚਲ Lookup_ਇੰਦਰਾਜ tab[], ਅਚਲ ਅੱਖਰ *string,
              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਮੁੱਲ_rtrn)
{
    ਜੇ (!string)
        ਵਾਪਸ false;

    ਲਈ (ਅਚਲ Lookup_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = tab; ਇੰਦਰਾਜ->name; ਇੰਦਰਾਜ++) {
        ਜੇ (istreq(ਇੰਦਰਾਜ->name, string)) {
            *ਮੁੱਲ_rtrn = ਇੰਦਰਾਜ->ਮੁੱਲ;
            ਵਾਪਸ true;
        }
    }

    ਵਾਪਸ false;
}

ਅਚਲ ਅੱਖਰ *
Lookup_ਮੁੱਲ(ਅਚਲ Lookup_ਇੰਦਰਾਜ tab[], ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ)
{
    ਲਈ (ਅਚਲ Lookup_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = tab; ਇੰਦਰਾਜ->name; ਇੰਦਰਾਜ++)
        ਜੇ (ਇੰਦਰਾਜ->ਮੁੱਲ == ਮੁੱਲ)
            ਵਾਪਸ ਇੰਦਰਾਜ->name;

    ਵਾਪਸ ਨਿਰਾ;
}

ਅਚਲ Lookup_ਇੰਦਰਾਜ ctrl_ਨਕਾਬ_ਨਾਂ[25] = {
    /* v2 keymap format only */
    { "Overlay3", CONTROL_OVERLAY3 },
    { "Overlay4", CONTROL_OVERLAY4 },
    { "Overlay5", CONTROL_OVERLAY5 },
    { "Overlay6", CONTROL_OVERLAY6 },
    { "Overlay7", CONTROL_OVERLAY7 },
    { "Overlay8", CONTROL_OVERLAY8 },
    { "all", CONTROL_ALL_BOOLEAN },
    /* v1 + v2 keymap formats */
    [CONTROL_ਨਾਂ_MIN_V1_ਸੂਚਕਾਂਕ] = { "RepeatKeys", CONTROL_REPEAT },
    { "Repeat", CONTROL_REPEAT },
    { "AutoRepeat", CONTROL_REPEAT },
    { "SlowKeys", CONTROL_SLOW },
    { "BounceKeys", CONTROL_DEBOUNCE },
    { "StickyKeys", CONTROL_STICKY_ਕੁੰਜੀਆਂ },
    { "MouseKeys", CONTROL_MOUSE_ਕੁੰਜੀਆਂ },
    { "MouseKeysAccel", CONTROL_MOUSE_ਕੁੰਜੀਆਂ_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_ਸਮੂਹ_ਤਾਲਾ },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { ਨਿਰਾ, 0 }
};

ਅਚਲ Lookup_ਇੰਦਰਾਜ mod_Component_ਨਕਾਬ_ਨਾਂ[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { ਨਿਰਾ, 0 }
};

ਅਚਲ Lookup_ਇੰਦਰਾਜ ਸਮੂਹ_Component_ਨਕਾਬ_ਨਾਂ[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { ਨਿਰਾ, 0 }
};

ਅਚਲ Lookup_ਇੰਦਰਾਜ ਸਮੂਹ_ਨਕਾਬ_ਨਾਂ[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { ਨਿਰਾ, 0 }
};

ਅਚਲ Lookup_ਇੰਦਰਾਜ button_ਨਾਂ[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { ਨਿਰਾ, 0 }
};

ਅਚਲ Lookup_ਇੰਦਰਾਜ use_Mod_Map_ਮੁੱਲ_ਨਾਂ[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { ਨਿਰਾ, 0 }
};

ਅਚਲ Lookup_ਇੰਦਰਾਜ action_Type_ਨਾਂ[] = {
    { "NoAction", ACTION_TYPE_NONE },
    { "VoidAction", ACTION_TYPE_VOID },
    { "SetMods", ACTION_TYPE_MOD_ਸੈੱਟ },
    { "LatchMods", ACTION_TYPE_MOD_LATCH },
    { "LockMods", ACTION_TYPE_MOD_ਤਾਲਾ },
    { "SetGroup", ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ },
    { "LatchGroup", ACTION_TYPE_ਸਮੂਹ_LATCH },
    { "LockGroup", ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ },
    { "MovePtr", ACTION_TYPE_PTR_MOVE },
    { "MovePointer", ACTION_TYPE_PTR_MOVE },
    { "PtrBtn", ACTION_TYPE_PTR_BUTTON },
    { "PointerButton", ACTION_TYPE_PTR_BUTTON },
    { "LockPtrBtn", ACTION_TYPE_PTR_ਤਾਲਾ },
    { "LockPtrButton", ACTION_TYPE_PTR_ਤਾਲਾ },
    { "LockPointerButton", ACTION_TYPE_PTR_ਤਾਲਾ },
    { "LockPointerBtn", ACTION_TYPE_PTR_ਤਾਲਾ },
    { "SetPtrDflt", ACTION_TYPE_PTR_DEFAULT },
    { "SetPointerDefault", ACTION_TYPE_PTR_DEFAULT },
    { "Terminate", ACTION_TYPE_TERMINATE },
    { "TerminateServer", ACTION_TYPE_TERMINATE },
    { "SwitchScreen", ACTION_TYPE_SWITCH_VT },
    { "SetControls", ACTION_TYPE_CTRL_ਸੈੱਟ },
    { "LockControls", ACTION_TYPE_CTRL_ਤਾਲਾ },
    { "RedirectKey", ACTION_TYPE_REDIRECT_ਕੁੰਜੀ },
    { "Redirect", ACTION_TYPE_REDIRECT_ਕੁੰਜੀ },
    { "Private", ACTION_TYPE_ਨਿੱਜੀ },
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
    { ਨਿਰਾ, 0 },
};

ਅਚਲ Lookup_ਇੰਦਰਾਜ sym_Interpret_ਮੇਲ_ਨਕਾਬ_ਨਾਂ[] = {
    { "NoneOf", ਮੇਲ_NONE },
    { "AnyOfOrNone", ਮੇਲ_ANY_OR_NONE },
    { "AnyOf", ਮੇਲ_ANY },
    { "AllOf", ਮੇਲ_ALL },
    { "Exactly", ਮੇਲ_EXACTLY },
    { ਨਿਰਾ, 0 },
};

ਅਚਲ ਅੱਖਰ *
Mod_ਸੂਚਕਾਂਕ_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
             xkb_mod_ਸੂਚਕਾਂਕ_t ndx)
{
    ਜੇ (ndx == XKB_MOD_INVALID)
        ਵਾਪਸ "none";

    ਜੇ (ndx == XKB_MOD_NONE)
        ਵਾਪਸ "None";

    ਜੇ (ndx >= mods->ਅੰਕ_mods)
        ਵਾਪਸ ਨਿਰਾ;

    ਵਾਪਸ xkb_atom_ਪਾਠ(ctx, mods->mods[ndx].name);
}

ਅਚਲ ਅੱਖਰ *
Action_Type_ਪਾਠ(ਗਿਣਤੀ xkb_action_type type)
{
    ਅਚਲ ਅੱਖਰ *name = Lookup_ਮੁੱਲ(action_Type_ਨਾਂ, type);
    ਵਾਪਸ name ? name : "Private";
}

ਅਚਲ ਅੱਖਰ *
Keysym_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, xkb_keysym_t sym)
{
    ਅੱਖਰ *ਬਫਰ = xkb_context_get_ਬਫਰ(ctx, XKB_KEYSYM_NAME_MAX_ਮਾਪ);
    xkb_keysym_get_name(sym, ਬਫਰ, XKB_KEYSYM_NAME_MAX_ਮਾਪ);
    ਵਾਪਸ ਬਫਰ;
}

ਅਚਲ ਅੱਖਰ *
ਕੁੰਜੀ_Name_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, xkb_atom_t name)
{
    ਅਚਲ ਅੱਖਰ *sname = xkb_atom_ਪਾਠ(ctx, name);
    ਮਾਪ_t len = strlen_ਸੁਰੱਖਿਅਤ(sname) + 3;
    ਅੱਖਰ *buf = xkb_context_get_ਬਫਰ(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    ਵਾਪਸ buf;
}

ਅਚਲ ਅੱਖਰ *
SIMatch_ਪਾਠ(ਗਿਣਤੀ xkb_ਮੇਲ_operation type)
{
    ਵਾਪਸ Lookup_ਮੁੱਲ(sym_Interpret_ਮੇਲ_ਨਕਾਬ_ਨਾਂ, type);
}

ਅਚਲ ਅੱਖਰ *
Mod_ਨਕਾਬ_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ mod_type type,
            ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, xkb_mod_ਨਕਾਬ_t ਨਕਾਬ)
{
    ਅੱਖਰ buf[1024] = {0};
    ਮਾਪ_t pos = 0;
    ਅਚਲ ਬਣਤਰ xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    ਪ੍ਰਤਿਜ੍ਞਾ(type == MOD_REAL || type == MOD_BOTH);

    ਜੇ (ਨਕਾਬ == 0)
        ਵਾਪਸ "none";

    ਜੇ (ਨਕਾਬ == MOD_REAL_ਨਕਾਬ_ALL)
        ਵਾਪਸ "all";

    ਜੇ ((type == MOD_REAL && (ਨਕਾਬ & ~MOD_REAL_ਨਕਾਬ_ALL)) ||
        unlikely(ਨਕਾਬ & ~((UINT64_C(1) << mods->ਅੰਕ_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ਅਚਲ ਪੂਰਨ ret = snprintf(buf, ਆਕਾਰ(buf), "0x%"PRIx32, ਨਕਾਬ);
        static_assert(ਆਕਾਰ(ਨਕਾਬ) == 4 && ਆਕਾਰ(buf) >= ਆਕਾਰ("0xffffffff"),
                      "Buffer too small");
        ਪ੍ਰਤਿਜ੍ਞਾ(ret >= 0 && (ਮਾਪ_t) ret < ਆਕਾਰ(buf));
        pos = (ਮਾਪ_t) ret;
    } ਨਹੀਂਤਾਂ {
        /* Print known mods */
        xkb_mods_ਨਕਾਬ_foreach(ਨਕਾਬ, mod, mods) {
            ਪੂਰਨ ret = snprintf(buf + pos, ਆਕਾਰ(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_ਪਾਠ(ctx, mod->name));
            ਜੇ (ret <= 0 || pos + (ਮਾਪ_t)ret >= ਆਕਾਰ(buf))
                ਰੋਕੋ;
            ਨਹੀਂਤਾਂ
                pos += (ਮਾਪ_t)ret;
        }
    }
    pos++;

    ਵਾਪਸ ਸਮ੍ਰਿਤੀ_ਨਕਲ(xkb_context_get_ਬਫਰ(ctx, pos), buf, pos);
}

ਅਚਲ ਅੱਖਰ *
Led_State_ਨਕਾਬ_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ Lookup_ਇੰਦਰਾਜ *lookup,
                 ਗਿਣਤੀ xkb_state_component ਨਕਾਬ)
{
    ਅੱਖਰ buf[1024];
    ਮਾਪ_t pos = 0;

    ਜੇ (ਨਕਾਬ == 0)
        ਵਾਪਸ "0";

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; ਨਕਾਬ; i++) {
        ਪੂਰਨ ret;

        ਜੇ (!(ਨਕਾਬ & (1u << i)))
            ਜਾਰੀ;

        ਨਕਾਬ &= ~(1u << i);

        ਅਚਲ ਅੱਖਰ* ਅਚਲ ਨਕਾਬ_ਪਾਠ = Lookup_ਮੁੱਲ(lookup, 1u << i);
        ਪ੍ਰਤਿਜ੍ਞਾ(ਨਕਾਬ_ਪਾਠ != ਨਿਰਾ);
        ret = snprintf(buf + pos, ਆਕਾਰ(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", ਨਕਾਬ_ਪਾਠ);
        ਜੇ (ret <= 0 || pos + (ਮਾਪ_t)ret >= ਆਕਾਰ(buf))
            ਰੋਕੋ;
        ਨਹੀਂਤਾਂ
            pos += (ਮਾਪ_t)ret;
    }
    pos++;

    ਵਾਪਸ ਸਮ੍ਰਿਤੀ_ਨਕਲ(xkb_context_get_ਬਫਰ(ctx, pos), buf, pos);
}

ਅਚਲ ਅੱਖਰ *
Control_ਨਕਾਬ_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_keymap_format format,
                ਗਿਣਤੀ xkb_action_controls ਨਕਾਬ)
{
    ਅੱਖਰ buf[1024];
    ਮਾਪ_t pos = 0;

    ਅਚਲ ਗਿਣਤੀ xkb_action_controls all_ctrls = format_boolean_controls(format);
    ਨਕਾਬ &= all_ctrls;

    ਜੇ (ਨਕਾਬ == 0)
        ਵਾਪਸ "none";

    ਜੇ (ਨਕਾਬ == all_ctrls)
        ਵਾਪਸ "all";

    ਅਚਲ uint8_t control_ਨਾਂ_ਵਿੱਥ = format_control_ਨਾਂ_ਵਿੱਥ(format);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; ਨਕਾਬ; i++) {
        ਪੂਰਨ ret;

        ਜੇ (!(ਨਕਾਬ & (1u << i)))
            ਜਾਰੀ;

        ਨਕਾਬ &= ~(1u << i);

        ਅਚਲ ਅੱਖਰ* ਅਚਲ ਨਕਾਬ_ਪਾਠ =
            Lookup_ਮੁੱਲ(ctrl_ਨਕਾਬ_ਨਾਂ + control_ਨਾਂ_ਵਿੱਥ, 1u << i);
        ਪ੍ਰਤਿਜ੍ਞਾ(ਨਕਾਬ_ਪਾਠ);
        ret = snprintf(buf + pos, ਆਕਾਰ(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", ਨਕਾਬ_ਪਾਠ);
        ਜੇ (ret <= 0 || pos + (ਮਾਪ_t)ret >= ਆਕਾਰ(buf))
            ਰੋਕੋ;
        ਨਹੀਂਤਾਂ
            pos += (ਮਾਪ_t)ret;
    }
    pos++;

    ਵਾਪਸ ਸਮ੍ਰਿਤੀ_ਨਕਲ(xkb_context_get_ਬਫਰ(ctx, pos), buf, pos);
}
