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
LookupString(ಧ್ರುವ LookupEntry tab[], ಧ್ರುವ ವರ್ಣ *string,
              unsigned ಪೂರ್ಣಾಙ್ಕ *ಮೌಲ್ಯ_rtrn)
{
    ಯದಿ (!string)
        ಪ್ರತ್ಯಾಗಮ false;

    ಪ್ರತಿ (ಧ್ರುವ LookupEntry *entry = tab; entry->name; entry++) {
        ಯದಿ (istreq(entry->name, string)) {
            *ಮೌಲ್ಯ_rtrn = entry->ಮೌಲ್ಯ;
            ಪ್ರತ್ಯಾಗಮ true;
        }
    }

    ಪ್ರತ್ಯಾಗಮ false;
}

ಧ್ರುವ ವರ್ಣ *
Lookup_ಮೌಲ್ಯ(ಧ್ರುವ LookupEntry tab[], unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ)
{
    ಪ್ರತಿ (ಧ್ರುವ LookupEntry *entry = tab; entry->name; entry++)
        ಯದಿ (entry->ಮೌಲ್ಯ == ಮೌಲ್ಯ)
            ಪ್ರತ್ಯಾಗಮ entry->name;

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಧ್ರುವ LookupEntry ctrlMaskNames[25] = {
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
    { "StickyKeys", CONTROL_STICKY_ಕೀಲಿಗಳು },
    { "MouseKeys", CONTROL_MOUSE_ಕೀಲಿಗಳು },
    { "MouseKeysAccel", CONTROL_MOUSE_ಕೀಲಿಗಳು_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_GROUP_LOCK },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { ರಿಕ್ತ, 0 }
};

ಧ್ರುವ LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { ರಿಕ್ತ, 0 }
};

ಧ್ರುವ LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { ರಿಕ್ತ, 0 }
};

ಧ್ರುವ LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { ರಿಕ್ತ, 0 }
};

ಧ್ರುವ LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { ರಿಕ್ತ, 0 }
};

ಧ್ರುವ LookupEntry use_Mod_Map_ಮೌಲ್ಯ_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { ರಿಕ್ತ, 0 }
};

ಧ್ರುವ LookupEntry actionTypeNames[] = {
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_ಕೀಲಿ },
    { "Redirect", ACTION_TYPE_REDIRECT_ಕೀಲಿ },
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
    { ರಿಕ್ತ, 0 },
};

ಧ್ರುವ LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { ರಿಕ್ತ, 0 },
};

ಧ್ರುವ ವರ್ಣ *
ModIndexText(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    ಯದಿ (ndx == XKB_MOD_INVALID)
        ಪ್ರತ್ಯಾಗಮ "none";

    ಯದಿ (ndx == XKB_MOD_NONE)
        ಪ್ರತ್ಯಾಗಮ "None";

    ಯದಿ (ndx >= mods->ಅಂಕೆ_mods)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ಪ್ರತ್ಯಾಗಮ xkb_atom_text(ctx, mods->mods[ndx].name);
}

ಧ್ರುವ ವರ್ಣ *
ActionTypeText(ಗಣನಾ xkb_action_type type)
{
    ಧ್ರುವ ವರ್ಣ *name = Lookup_ಮೌಲ್ಯ(actionTypeNames, type);
    ಪ್ರತ್ಯಾಗಮ name ? name : "Private";
}

ಧ್ರುವ ವರ್ಣ *
KeysymText(ಸಂರಚನಾ xkb_context *ctx, xkb_keysym_t sym)
{
    ವರ್ಣ *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    ಪ್ರತ್ಯಾಗಮ buffer;
}

ಧ್ರುವ ವರ್ಣ *
ಕೀಲಿ_Name_Text(ಸಂರಚನಾ xkb_context *ctx, xkb_atom_t name)
{
    ಧ್ರುವ ವರ್ಣ *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    ವರ್ಣ *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    ಪ್ರತ್ಯಾಗಮ buf;
}

ಧ್ರುವ ವರ್ಣ *
SIMatchText(ಗಣನಾ xkb_match_operation type)
{
    ಪ್ರತ್ಯಾಗಮ Lookup_ಮೌಲ್ಯ(symInterpretMatchMaskNames, type);
}

ಧ್ರುವ ವರ್ಣ *
ModMaskText(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ mod_type type,
            ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    ವರ್ಣ buf[1024] = {0};
    size_t pos = 0;
    ಧ್ರುವ ಸಂರಚನಾ xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    ಪ್ರತಿಜ್ಞಾ(type == MOD_REAL || type == MOD_BOTH);

    ಯದಿ (mask == 0)
        ಪ್ರತ್ಯಾಗಮ "none";

    ಯದಿ (mask == MOD_REAL_MASK_ALL)
        ಪ್ರತ್ಯಾಗಮ "all";

    ಯದಿ ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->ಅಂಕೆ_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ret = snprintf(buf, ಪರಿಮಾಣ(buf), "0x%"PRIx32, mask);
        static_assert(ಪರಿಮಾಣ(mask) == 4 && ಪರಿಮಾಣ(buf) >= ಪರಿಮಾಣ("0xffffffff"),
                      "Buffer too small");
        ಪ್ರತಿಜ್ಞಾ(ret >= 0 && (size_t) ret < ಪರಿಮಾಣ(buf));
        pos = (size_t) ret;
    } ಅನ್ಯಥಾ {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            ಪೂರ್ಣಾಙ್ಕ ret = snprintf(buf + pos, ಪರಿಮಾಣ(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            ಯದಿ (ret <= 0 || pos + (size_t)ret >= ಪರಿಮಾಣ(buf))
                ಭಙ್ಗ;
            ಅನ್ಯಥಾ
                pos += (size_t)ret;
        }
    }
    pos++;

    ಪ್ರತ್ಯಾಗಮ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ಧ್ರುವ ವರ್ಣ *
LedStateMaskText(ಸಂರಚನಾ xkb_context *ctx, ಧ್ರುವ LookupEntry *lookup,
                 ಗಣನಾ xkb_state_component mask)
{
    ವರ್ಣ buf[1024];
    size_t pos = 0;

    ಯದಿ (mask == 0)
        ಪ್ರತ್ಯಾಗಮ "0";

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; mask; i++) {
        ಪೂರ್ಣಾಙ್ಕ ret;

        ಯದಿ (!(mask & (1u << i)))
            ಅನುವರ್ತ;

        mask &= ~(1u << i);

        ಧ್ರುವ ವರ್ಣ* ಧ್ರುವ maskText = Lookup_ಮೌಲ್ಯ(lookup, 1u << i);
        ಪ್ರತಿಜ್ಞಾ(maskText != ರಿಕ್ತ);
        ret = snprintf(buf + pos, ಪರಿಮಾಣ(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        ಯದಿ (ret <= 0 || pos + (size_t)ret >= ಪರಿಮಾಣ(buf))
            ಭಙ್ಗ;
        ಅನ್ಯಥಾ
            pos += (size_t)ret;
    }
    pos++;

    ಪ್ರತ್ಯಾಗಮ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ಧ್ರುವ ವರ್ಣ *
ControlMaskText(ಸಂರಚನಾ xkb_context *ctx, ಗಣನಾ xkb_keymap_format format,
                ಗಣನಾ xkb_action_controls mask)
{
    ವರ್ಣ buf[1024];
    size_t pos = 0;

    ಧ್ರುವ ಗಣನಾ xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    ಯದಿ (mask == 0)
        ಪ್ರತ್ಯಾಗಮ "none";

    ಯದಿ (mask == all_ctrls)
        ಪ್ರತ್ಯಾಗಮ "all";

    ಧ್ರುವ uint8_t control_names_offset = format_control_names_offset(format);
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; mask; i++) {
        ಪೂರ್ಣಾಙ್ಕ ret;

        ಯದಿ (!(mask & (1u << i)))
            ಅನುವರ್ತ;

        mask &= ~(1u << i);

        ಧ್ರುವ ವರ್ಣ* ಧ್ರುವ maskText =
            Lookup_ಮೌಲ್ಯ(ctrlMaskNames + control_names_offset, 1u << i);
        ಪ್ರತಿಜ್ಞಾ(maskText);
        ret = snprintf(buf + pos, ಪರಿಮಾಣ(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        ಯದಿ (ret <= 0 || pos + (size_t)ret >= ಪರಿಮಾಣ(buf))
            ಭಙ್ಗ;
        ಅನ್ಯಥಾ
            pos += (size_t)ret;
    }
    pos++;

    ಪ್ರತ್ಯಾಗಮ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
