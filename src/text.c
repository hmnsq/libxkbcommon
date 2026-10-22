/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "keysym.h"
#അന്തര്ഭാവ "text.h"

bool
LookupString(സ്ഥിരം LookupEntry tab[], സ്ഥിരം അക്ഷരം *string,
              unsigned പൂർണ്ണസംഖ്യ *മൂല്യം_rtrn)
{
    എങ്കിൽ (!string)
        തിരികെ false;

    വേണ്ടി (സ്ഥിരം LookupEntry *entry = tab; entry->name; entry++) {
        എങ്കിൽ (istreq(entry->name, string)) {
            *മൂല്യം_rtrn = entry->മൂല്യം;
            തിരികെ true;
        }
    }

    തിരികെ false;
}

സ്ഥിരം അക്ഷരം *
Lookup_മൂല്യം(സ്ഥിരം LookupEntry tab[], unsigned പൂർണ്ണസംഖ്യ മൂല്യം)
{
    വേണ്ടി (സ്ഥിരം LookupEntry *entry = tab; entry->name; entry++)
        എങ്കിൽ (entry->മൂല്യം == മൂല്യം)
            തിരികെ entry->name;

    തിരികെ ശൂന്യ;
}

സ്ഥിരം LookupEntry ctrlMaskNames[25] = {
    /* v2 keymap format only */
    { "Overlay3", CONTROL_OVERLAY3 },
    { "Overlay4", CONTROL_OVERLAY4 },
    { "Overlay5", CONTROL_OVERLAY5 },
    { "Overlay6", CONTROL_OVERLAY6 },
    { "Overlay7", CONTROL_OVERLAY7 },
    { "Overlay8", CONTROL_OVERLAY8 },
    { "all", CONTROL_ALL_BOOLEAN },
    /* v1 + v2 keymap formats */
    [CONTROL_NAMES_MIN_V1_INDEX] = { "RepeatKeys", CONTROL_REPEAT },
    { "Repeat", CONTROL_REPEAT },
    { "AutoRepeat", CONTROL_REPEAT },
    { "SlowKeys", CONTROL_SLOW },
    { "BounceKeys", CONTROL_DEBOUNCE },
    { "StickyKeys", CONTROL_STICKY_താക്കോലുകൾ },
    { "MouseKeys", CONTROL_MOUSE_താക്കോലുകൾ },
    { "MouseKeysAccel", CONTROL_MOUSE_താക്കോലുകൾ_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_GROUP_LOCK },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { ശൂന്യ, 0 }
};

സ്ഥിരം LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { ശൂന്യ, 0 }
};

സ്ഥിരം LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { ശൂന്യ, 0 }
};

സ്ഥിരം LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { ശൂന്യ, 0 }
};

സ്ഥിരം LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { ശൂന്യ, 0 }
};

സ്ഥിരം LookupEntry use_Mod_Map_മൂല്യം_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { ശൂന്യ, 0 }
};

സ്ഥിരം LookupEntry actionTypeNames[] = {
    { "NoAction", ACTION_TYPE_NONE },
    { "VoidAction", ACTION_TYPE_VOID },
    { "SetMods", ACTION_TYPE_MOD_SET },
    { "LatchMods", ACTION_TYPE_MOD_LATCH },
    { "LockMods", ACTION_TYPE_MOD_LOCK },
    { "SetGroup", ACTION_TYPE_GROUP_SET },
    { "LatchGroup", ACTION_TYPE_GROUP_LATCH },
    { "LockGroup", ACTION_TYPE_GROUP_LOCK },
    { "MovePtr", ACTION_TYPE_PTR_MOVE },
    { "MovePointer", ACTION_TYPE_PTR_MOVE },
    { "PtrBtn", ACTION_TYPE_PTR_BUTTON },
    { "PointerButton", ACTION_TYPE_PTR_BUTTON },
    { "LockPtrBtn", ACTION_TYPE_PTR_LOCK },
    { "LockPtrButton", ACTION_TYPE_PTR_LOCK },
    { "LockPointerButton", ACTION_TYPE_PTR_LOCK },
    { "LockPointerBtn", ACTION_TYPE_PTR_LOCK },
    { "SetPtrDflt", ACTION_TYPE_PTR_DEFAULT },
    { "SetPointerDefault", ACTION_TYPE_PTR_DEFAULT },
    { "Terminate", ACTION_TYPE_TERMINATE },
    { "TerminateServer", ACTION_TYPE_TERMINATE },
    { "SwitchScreen", ACTION_TYPE_SWITCH_VT },
    { "SetControls", ACTION_TYPE_CTRL_SET },
    { "LockControls", ACTION_TYPE_CTRL_LOCK },
    { "RedirectKey", ACTION_TYPE_REDIRECT_താക്കോൽ },
    { "Redirect", ACTION_TYPE_REDIRECT_താക്കോൽ },
    { "Private", ACTION_TYPE_PRIVATE },
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
    { ശൂന്യ, 0 },
};

സ്ഥിരം LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { ശൂന്യ, 0 },
};

സ്ഥിരം അക്ഷരം *
ModIndexText(ഘടന xkb_context *ctx, സ്ഥിരം ഘടന xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    എങ്കിൽ (ndx == XKB_MOD_INVALID)
        തിരികെ "none";

    എങ്കിൽ (ndx == XKB_MOD_NONE)
        തിരികെ "None";

    എങ്കിൽ (ndx >= mods->അക്കം_mods)
        തിരികെ ശൂന്യ;

    തിരികെ xkb_atom_text(ctx, mods->mods[ndx].name);
}

സ്ഥിരം അക്ഷരം *
ActionTypeText(എണ്ണൽ xkb_action_type type)
{
    സ്ഥിരം അക്ഷരം *name = Lookup_മൂല്യം(actionTypeNames, type);
    തിരികെ name ? name : "Private";
}

സ്ഥിരം അക്ഷരം *
KeysymText(ഘടന xkb_context *ctx, xkb_keysym_t sym)
{
    അക്ഷരം *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    തിരികെ buffer;
}

സ്ഥിരം അക്ഷരം *
താക്കോൽ_Name_Text(ഘടന xkb_context *ctx, xkb_atom_t name)
{
    സ്ഥിരം അക്ഷരം *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    അക്ഷരം *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    തിരികെ buf;
}

സ്ഥിരം അക്ഷരം *
SIMatchText(എണ്ണൽ xkb_match_operation type)
{
    തിരികെ Lookup_മൂല്യം(symInterpretMatchMaskNames, type);
}

സ്ഥിരം അക്ഷരം *
ModMaskText(ഘടന xkb_context *ctx, എണ്ണൽ mod_type type,
            സ്ഥിരം ഘടന xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    അക്ഷരം buf[1024] = {0};
    size_t pos = 0;
    സ്ഥിരം ഘടന xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    പ്രതിജ്ഞാ(type == MOD_REAL || type == MOD_BOTH);

    എങ്കിൽ (mask == 0)
        തിരികെ "none";

    എങ്കിൽ (mask == MOD_REAL_MASK_ALL)
        തിരികെ "all";

    എങ്കിൽ ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->അക്കം_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        സ്ഥിരം പൂർണ്ണസംഖ്യ ret = snprintf(buf, വലുപ്പം(buf), "0x%"PRIx32, mask);
        static_assert(വലുപ്പം(mask) == 4 && വലുപ്പം(buf) >= വലുപ്പം("0xffffffff"),
                      "Buffer too small");
        പ്രതിജ്ഞാ(ret >= 0 && (size_t) ret < വലുപ്പം(buf));
        pos = (size_t) ret;
    } അല്ലെങ്കിൽ {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            പൂർണ്ണസംഖ്യ ret = snprintf(buf + pos, വലുപ്പം(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            എങ്കിൽ (ret <= 0 || pos + (size_t)ret >= വലുപ്പം(buf))
                നിർത്തുക;
            അല്ലെങ്കിൽ
                pos += (size_t)ret;
        }
    }
    pos++;

    തിരികെ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

സ്ഥിരം അക്ഷരം *
LedStateMaskText(ഘടന xkb_context *ctx, സ്ഥിരം LookupEntry *lookup,
                 എണ്ണൽ xkb_state_component mask)
{
    അക്ഷരം buf[1024];
    size_t pos = 0;

    എങ്കിൽ (mask == 0)
        തിരികെ "0";

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; mask; i++) {
        പൂർണ്ണസംഖ്യ ret;

        എങ്കിൽ (!(mask & (1u << i)))
            തുടരുക;

        mask &= ~(1u << i);

        സ്ഥിരം അക്ഷരം* സ്ഥിരം maskText = Lookup_മൂല്യം(lookup, 1u << i);
        പ്രതിജ്ഞാ(maskText != ശൂന്യ);
        ret = snprintf(buf + pos, വലുപ്പം(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        എങ്കിൽ (ret <= 0 || pos + (size_t)ret >= വലുപ്പം(buf))
            നിർത്തുക;
        അല്ലെങ്കിൽ
            pos += (size_t)ret;
    }
    pos++;

    തിരികെ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

സ്ഥിരം അക്ഷരം *
ControlMaskText(ഘടന xkb_context *ctx, എണ്ണൽ xkb_keymap_format format,
                എണ്ണൽ xkb_action_controls mask)
{
    അക്ഷരം buf[1024];
    size_t pos = 0;

    സ്ഥിരം എണ്ണൽ xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    എങ്കിൽ (mask == 0)
        തിരികെ "none";

    എങ്കിൽ (mask == all_ctrls)
        തിരികെ "all";

    സ്ഥിരം uint8_t control_names_offset = format_control_names_offset(format);
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; mask; i++) {
        പൂർണ്ണസംഖ്യ ret;

        എങ്കിൽ (!(mask & (1u << i)))
            തുടരുക;

        mask &= ~(1u << i);

        സ്ഥിരം അക്ഷരം* സ്ഥിരം maskText =
            Lookup_മൂല്യം(ctrlMaskNames + control_names_offset, 1u << i);
        പ്രതിജ്ഞാ(maskText);
        ret = snprintf(buf + pos, വലുപ്പം(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        എങ്കിൽ (ret <= 0 || pos + (size_t)ret >= വലുപ്പം(buf))
            നിർത്തുക;
        അല്ലെങ്കിൽ
            pos += (size_t)ret;
    }
    pos++;

    തിരികെ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
