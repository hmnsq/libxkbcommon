/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "keysym.h"
#অন্তর্ভাৱ "text.h"

bool
LookupString(ধ্রুৱ LookupEntry tab[], ধ্রুৱ ৱর্ণ *string,
              unsigned পূর্ণাঙ্ক *মান_rtrn)
{
    যদি (!string)
        ঘূৰাই false;

    বাবে (ধ্রুৱ LookupEntry *entry = tab; entry->name; entry++) {
        যদি (istreq(entry->name, string)) {
            *মান_rtrn = entry->মান;
            ঘূৰাই true;
        }
    }

    ঘূৰাই false;
}

ধ্রুৱ ৱর্ণ *
Lookup_মান(ধ্রুৱ LookupEntry tab[], unsigned পূর্ণাঙ্ক মান)
{
    বাবে (ধ্রুৱ LookupEntry *entry = tab; entry->name; entry++)
        যদি (entry->মান == মান)
            ঘূৰাই entry->name;

    ঘূৰাই রিক্ত;
}

ধ্রুৱ LookupEntry ctrlMaskNames[25] = {
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
    { "StickyKeys", CONTROL_STICKY_চাবিগুলি },
    { "MouseKeys", CONTROL_MOUSE_চাবিগুলি },
    { "MouseKeysAccel", CONTROL_MOUSE_চাবিগুলি_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_GROUP_LOCK },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { রিক্ত, 0 }
};

ধ্রুৱ LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { রিক্ত, 0 }
};

ধ্রুৱ LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { রিক্ত, 0 }
};

ধ্রুৱ LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { রিক্ত, 0 }
};

ধ্রুৱ LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { রিক্ত, 0 }
};

ধ্রুৱ LookupEntry use_Mod_Map_মান_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { রিক্ত, 0 }
};

ধ্রুৱ LookupEntry actionTypeNames[] = {
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_চাবি },
    { "Redirect", ACTION_TYPE_REDIRECT_চাবি },
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
    { রিক্ত, 0 },
};

ধ্রুৱ LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { রিক্ত, 0 },
};

ধ্রুৱ ৱর্ণ *
ModIndexText(সংরচনা xkb_context *ctx, ধ্রুৱ সংরচনা xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    যদি (ndx == XKB_MOD_INVALID)
        ঘূৰাই "none";

    যদি (ndx == XKB_MOD_NONE)
        ঘূৰাই "None";

    যদি (ndx >= mods->অঙ্ক_mods)
        ঘূৰাই রিক্ত;

    ঘূৰাই xkb_atom_text(ctx, mods->mods[ndx].name);
}

ধ্রুৱ ৱর্ণ *
ActionTypeText(গণনা xkb_action_type type)
{
    ধ্রুৱ ৱর্ণ *name = Lookup_মান(actionTypeNames, type);
    ঘূৰাই name ? name : "Private";
}

ধ্রুৱ ৱর্ণ *
KeysymText(সংরচনা xkb_context *ctx, xkb_keysym_t sym)
{
    ৱর্ণ *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    ঘূৰাই buffer;
}

ধ্রুৱ ৱর্ণ *
চাবি_Name_Text(সংরচনা xkb_context *ctx, xkb_atom_t name)
{
    ধ্রুৱ ৱর্ণ *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    ৱর্ণ *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    ঘূৰাই buf;
}

ধ্রুৱ ৱর্ণ *
SIMatchText(গণনা xkb_match_operation type)
{
    ঘূৰাই Lookup_মান(symInterpretMatchMaskNames, type);
}

ধ্রুৱ ৱর্ণ *
ModMaskText(সংরচনা xkb_context *ctx, গণনা mod_type type,
            ধ্রুৱ সংরচনা xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    ৱর্ণ buf[1024] = {0};
    size_t pos = 0;
    ধ্রুৱ সংরচনা xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    প্রতিজ্ঞা(type == MOD_REAL || type == MOD_BOTH);

    যদি (mask == 0)
        ঘূৰাই "none";

    যদি (mask == MOD_REAL_MASK_ALL)
        ঘূৰাই "all";

    যদি ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->অঙ্ক_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ধ্রুৱ পূর্ণাঙ্ক ret = snprintf(buf, পরিমাণ(buf), "0x%"PRIx32, mask);
        static_assert(পরিমাণ(mask) == 4 && পরিমাণ(buf) >= পরিমাণ("0xffffffff"),
                      "Buffer too small");
        প্রতিজ্ঞা(ret >= 0 && (size_t) ret < পরিমাণ(buf));
        pos = (size_t) ret;
    } অন্যথা {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            পূর্ণাঙ্ক ret = snprintf(buf + pos, পরিমাণ(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            যদি (ret <= 0 || pos + (size_t)ret >= পরিমাণ(buf))
                ভঙ্গ;
            অন্যথা
                pos += (size_t)ret;
        }
    }
    pos++;

    ঘূৰাই memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ধ্রুৱ ৱর্ণ *
LedStateMaskText(সংরচনা xkb_context *ctx, ধ্রুৱ LookupEntry *lookup,
                 গণনা xkb_state_component mask)
{
    ৱর্ণ buf[1024];
    size_t pos = 0;

    যদি (mask == 0)
        ঘূৰাই "0";

    বাবে (unsigned পূর্ণাঙ্ক i = 0; mask; i++) {
        পূর্ণাঙ্ক ret;

        যদি (!(mask & (1u << i)))
            অনুৱর্ত;

        mask &= ~(1u << i);

        ধ্রুৱ ৱর্ণ* ধ্রুৱ maskText = Lookup_মান(lookup, 1u << i);
        প্রতিজ্ঞা(maskText != রিক্ত);
        ret = snprintf(buf + pos, পরিমাণ(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        যদি (ret <= 0 || pos + (size_t)ret >= পরিমাণ(buf))
            ভঙ্গ;
        অন্যথা
            pos += (size_t)ret;
    }
    pos++;

    ঘূৰাই memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ধ্রুৱ ৱর্ণ *
ControlMaskText(সংরচনা xkb_context *ctx, গণনা xkb_keymap_format format,
                গণনা xkb_action_controls mask)
{
    ৱর্ণ buf[1024];
    size_t pos = 0;

    ধ্রুৱ গণনা xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    যদি (mask == 0)
        ঘূৰাই "none";

    যদি (mask == all_ctrls)
        ঘূৰাই "all";

    ধ্রুৱ uint8_t control_names_offset = format_control_names_offset(format);
    বাবে (unsigned পূর্ণাঙ্ক i = 0; mask; i++) {
        পূর্ণাঙ্ক ret;

        যদি (!(mask & (1u << i)))
            অনুৱর্ত;

        mask &= ~(1u << i);

        ধ্রুৱ ৱর্ণ* ধ্রুৱ maskText =
            Lookup_মান(ctrlMaskNames + control_names_offset, 1u << i);
        প্রতিজ্ঞা(maskText);
        ret = snprintf(buf + pos, পরিমাণ(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        যদি (ret <= 0 || pos + (size_t)ret >= পরিমাণ(buf))
            ভঙ্গ;
        অন্যথা
            pos += (size_t)ret;
    }
    pos++;

    ঘূৰাই memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
