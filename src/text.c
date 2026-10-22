/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "keysym.h"
#அந்தர்பாவ "text.h"

bool
LookupString(மாறிலி LookupEntry tab[], மாறிலி எழுத்து *string,
              unsigned முழு *மதிப்பு_rtrn)
{
    எனில் (!string)
        திரும்பு false;

    க்கு (மாறிலி LookupEntry *entry = tab; entry->name; entry++) {
        எனில் (istreq(entry->name, string)) {
            *மதிப்பு_rtrn = entry->மதிப்பு;
            திரும்பு true;
        }
    }

    திரும்பு false;
}

மாறிலி எழுத்து *
Lookup_மதிப்பு(மாறிலி LookupEntry tab[], unsigned முழு மதிப்பு)
{
    க்கு (மாறிலி LookupEntry *entry = tab; entry->name; entry++)
        எனில் (entry->மதிப்பு == மதிப்பு)
            திரும்பு entry->name;

    திரும்பு வெற்று;
}

மாறிலி LookupEntry ctrlMaskNames[25] = {
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
    { "StickyKeys", CONTROL_STICKY_சாவிகள் },
    { "MouseKeys", CONTROL_MOUSE_சாவிகள் },
    { "MouseKeysAccel", CONTROL_MOUSE_சாவிகள்_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_GROUP_LOCK },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { வெற்று, 0 }
};

மாறிலி LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { வெற்று, 0 }
};

மாறிலி LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { வெற்று, 0 }
};

மாறிலி LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { வெற்று, 0 }
};

மாறிலி LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { வெற்று, 0 }
};

மாறிலி LookupEntry use_Mod_Map_மதிப்பு_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { வெற்று, 0 }
};

மாறிலி LookupEntry actionTypeNames[] = {
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_சாவி },
    { "Redirect", ACTION_TYPE_REDIRECT_சாவி },
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
    { வெற்று, 0 },
};

மாறிலி LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { வெற்று, 0 },
};

மாறிலி எழுத்து *
ModIndexText(கட்டமைப்பு xkb_context *ctx, மாறிலி கட்டமைப்பு xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    எனில் (ndx == XKB_MOD_INVALID)
        திரும்பு "none";

    எனில் (ndx == XKB_MOD_NONE)
        திரும்பு "None";

    எனில் (ndx >= mods->இலக்கம்_mods)
        திரும்பு வெற்று;

    திரும்பு xkb_atom_text(ctx, mods->mods[ndx].name);
}

மாறிலி எழுத்து *
ActionTypeText(எண்ணிக்கை xkb_action_type type)
{
    மாறிலி எழுத்து *name = Lookup_மதிப்பு(actionTypeNames, type);
    திரும்பு name ? name : "Private";
}

மாறிலி எழுத்து *
KeysymText(கட்டமைப்பு xkb_context *ctx, xkb_keysym_t sym)
{
    எழுத்து *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    திரும்பு buffer;
}

மாறிலி எழுத்து *
சாவி_Name_Text(கட்டமைப்பு xkb_context *ctx, xkb_atom_t name)
{
    மாறிலி எழுத்து *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    எழுத்து *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    திரும்பு buf;
}

மாறிலி எழுத்து *
SIMatchText(எண்ணிக்கை xkb_match_operation type)
{
    திரும்பு Lookup_மதிப்பு(symInterpretMatchMaskNames, type);
}

மாறிலி எழுத்து *
ModMaskText(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை mod_type type,
            மாறிலி கட்டமைப்பு xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    எழுத்து buf[1024] = {0};
    size_t pos = 0;
    மாறிலி கட்டமைப்பு xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    ப்ரதிஜ்ஞா(type == MOD_REAL || type == MOD_BOTH);

    எனில் (mask == 0)
        திரும்பு "none";

    எனில் (mask == MOD_REAL_MASK_ALL)
        திரும்பு "all";

    எனில் ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->இலக்கம்_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        மாறிலி முழு ret = snprintf(buf, அளவு(buf), "0x%"PRIx32, mask);
        static_assert(அளவு(mask) == 4 && அளவு(buf) >= அளவு("0xffffffff"),
                      "Buffer too small");
        ப்ரதிஜ்ஞா(ret >= 0 && (size_t) ret < அளவு(buf));
        pos = (size_t) ret;
    } இல்லையெனில் {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            முழு ret = snprintf(buf + pos, அளவு(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            எனில் (ret <= 0 || pos + (size_t)ret >= அளவு(buf))
                நிறுத்து;
            இல்லையெனில்
                pos += (size_t)ret;
        }
    }
    pos++;

    திரும்பு memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

மாறிலி எழுத்து *
LedStateMaskText(கட்டமைப்பு xkb_context *ctx, மாறிலி LookupEntry *lookup,
                 எண்ணிக்கை xkb_state_component mask)
{
    எழுத்து buf[1024];
    size_t pos = 0;

    எனில் (mask == 0)
        திரும்பு "0";

    க்கு (unsigned முழு i = 0; mask; i++) {
        முழு ret;

        எனில் (!(mask & (1u << i)))
            தொடர்;

        mask &= ~(1u << i);

        மாறிலி எழுத்து* மாறிலி maskText = Lookup_மதிப்பு(lookup, 1u << i);
        ப்ரதிஜ்ஞா(maskText != வெற்று);
        ret = snprintf(buf + pos, அளவு(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        எனில் (ret <= 0 || pos + (size_t)ret >= அளவு(buf))
            நிறுத்து;
        இல்லையெனில்
            pos += (size_t)ret;
    }
    pos++;

    திரும்பு memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

மாறிலி எழுத்து *
ControlMaskText(கட்டமைப்பு xkb_context *ctx, எண்ணிக்கை xkb_keymap_format format,
                எண்ணிக்கை xkb_action_controls mask)
{
    எழுத்து buf[1024];
    size_t pos = 0;

    மாறிலி எண்ணிக்கை xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    எனில் (mask == 0)
        திரும்பு "none";

    எனில் (mask == all_ctrls)
        திரும்பு "all";

    மாறிலி uint8_t control_names_offset = format_control_names_offset(format);
    க்கு (unsigned முழு i = 0; mask; i++) {
        முழு ret;

        எனில் (!(mask & (1u << i)))
            தொடர்;

        mask &= ~(1u << i);

        மாறிலி எழுத்து* மாறிலி maskText =
            Lookup_மதிப்பு(ctrlMaskNames + control_names_offset, 1u << i);
        ப்ரதிஜ்ஞா(maskText);
        ret = snprintf(buf + pos, அளவு(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        எனில் (ret <= 0 || pos + (size_t)ret >= அளவு(buf))
            நிறுத்து;
        இல்லையெனில்
            pos += (size_t)ret;
    }
    pos++;

    திரும்பு memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
