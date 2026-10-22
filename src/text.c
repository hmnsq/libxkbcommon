/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "keysym.h"
#अन्तर्भाव "text.h"

bool
LookupString(ध्रुव LookupEntry tab[], ध्रुव वर्ण *string,
              unsigned पूर्णाङ्क *मूल्य_rtrn)
{
    यदि (!string)
        प्रत्यागम false;

    प्रति (ध्रुव LookupEntry *entry = tab; entry->name; entry++) {
        यदि (istreq(entry->name, string)) {
            *मूल्य_rtrn = entry->मूल्य;
            प्रत्यागम true;
        }
    }

    प्रत्यागम false;
}

ध्रुव वर्ण *
Lookup_मूल्य(ध्रुव LookupEntry tab[], unsigned पूर्णाङ्क मूल्य)
{
    प्रति (ध्रुव LookupEntry *entry = tab; entry->name; entry++)
        यदि (entry->मूल्य == मूल्य)
            प्रत्यागम entry->name;

    प्रत्यागम रिक्त;
}

ध्रुव LookupEntry ctrlMaskNames[25] = {
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
    { "StickyKeys", CONTROL_STICKY_कुंजियाँ },
    { "MouseKeys", CONTROL_MOUSE_कुंजियाँ },
    { "MouseKeysAccel", CONTROL_MOUSE_कुंजियाँ_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_GROUP_LOCK },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { रिक्त, 0 }
};

ध्रुव LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { रिक्त, 0 }
};

ध्रुव LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { रिक्त, 0 }
};

ध्रुव LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { रिक्त, 0 }
};

ध्रुव LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { रिक्त, 0 }
};

ध्रुव LookupEntry use_Mod_Map_मूल्य_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { रिक्त, 0 }
};

ध्रुव LookupEntry actionTypeNames[] = {
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_कुंजी },
    { "Redirect", ACTION_TYPE_REDIRECT_कुंजी },
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
    { रिक्त, 0 },
};

ध्रुव LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { रिक्त, 0 },
};

ध्रुव वर्ण *
ModIndexText(संरचना xkb_context *ctx, ध्रुव संरचना xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    यदि (ndx == XKB_MOD_INVALID)
        प्रत्यागम "none";

    यदि (ndx == XKB_MOD_NONE)
        प्रत्यागम "None";

    यदि (ndx >= mods->अंक_mods)
        प्रत्यागम रिक्त;

    प्रत्यागम xkb_atom_text(ctx, mods->mods[ndx].name);
}

ध्रुव वर्ण *
ActionTypeText(गणना xkb_action_type type)
{
    ध्रुव वर्ण *name = Lookup_मूल्य(actionTypeNames, type);
    प्रत्यागम name ? name : "Private";
}

ध्रुव वर्ण *
KeysymText(संरचना xkb_context *ctx, xkb_keysym_t sym)
{
    वर्ण *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    प्रत्यागम buffer;
}

ध्रुव वर्ण *
कुंजी_Name_Text(संरचना xkb_context *ctx, xkb_atom_t name)
{
    ध्रुव वर्ण *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    वर्ण *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    प्रत्यागम buf;
}

ध्रुव वर्ण *
SIMatchText(गणना xkb_match_operation type)
{
    प्रत्यागम Lookup_मूल्य(symInterpretMatchMaskNames, type);
}

ध्रुव वर्ण *
ModMaskText(संरचना xkb_context *ctx, गणना mod_type type,
            ध्रुव संरचना xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    वर्ण buf[1024] = {0};
    size_t pos = 0;
    ध्रुव संरचना xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    प्रतिज्ञा(type == MOD_REAL || type == MOD_BOTH);

    यदि (mask == 0)
        प्रत्यागम "none";

    यदि (mask == MOD_REAL_MASK_ALL)
        प्रत्यागम "all";

    यदि ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->अंक_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ध्रुव पूर्णाङ्क ret = snprintf(buf, परिमाण(buf), "0x%"PRIx32, mask);
        static_assert(परिमाण(mask) == 4 && परिमाण(buf) >= परिमाण("0xffffffff"),
                      "Buffer too small");
        प्रतिज्ञा(ret >= 0 && (size_t) ret < परिमाण(buf));
        pos = (size_t) ret;
    } अन्यथा {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            पूर्णाङ्क ret = snprintf(buf + pos, परिमाण(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            यदि (ret <= 0 || pos + (size_t)ret >= परिमाण(buf))
                भङ्ग;
            अन्यथा
                pos += (size_t)ret;
        }
    }
    pos++;

    प्रत्यागम memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ध्रुव वर्ण *
LedStateMaskText(संरचना xkb_context *ctx, ध्रुव LookupEntry *lookup,
                 गणना xkb_state_component mask)
{
    वर्ण buf[1024];
    size_t pos = 0;

    यदि (mask == 0)
        प्रत्यागम "0";

    प्रति (unsigned पूर्णाङ्क i = 0; mask; i++) {
        पूर्णाङ्क ret;

        यदि (!(mask & (1u << i)))
            अनुवर्त;

        mask &= ~(1u << i);

        ध्रुव वर्ण* ध्रुव maskText = Lookup_मूल्य(lookup, 1u << i);
        प्रतिज्ञा(maskText != रिक्त);
        ret = snprintf(buf + pos, परिमाण(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        यदि (ret <= 0 || pos + (size_t)ret >= परिमाण(buf))
            भङ्ग;
        अन्यथा
            pos += (size_t)ret;
    }
    pos++;

    प्रत्यागम memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ध्रुव वर्ण *
ControlMaskText(संरचना xkb_context *ctx, गणना xkb_keymap_format format,
                गणना xkb_action_controls mask)
{
    वर्ण buf[1024];
    size_t pos = 0;

    ध्रुव गणना xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    यदि (mask == 0)
        प्रत्यागम "none";

    यदि (mask == all_ctrls)
        प्रत्यागम "all";

    ध्रुव uint8_t control_names_offset = format_control_names_offset(format);
    प्रति (unsigned पूर्णाङ्क i = 0; mask; i++) {
        पूर्णाङ्क ret;

        यदि (!(mask & (1u << i)))
            अनुवर्त;

        mask &= ~(1u << i);

        ध्रुव वर्ण* ध्रुव maskText =
            Lookup_मूल्य(ctrlMaskNames + control_names_offset, 1u << i);
        प्रतिज्ञा(maskText);
        ret = snprintf(buf + pos, परिमाण(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        यदि (ret <= 0 || pos + (size_t)ret >= परिमाण(buf))
            भङ्ग;
        अन्यथा
            pos += (size_t)ret;
    }
    pos++;

    प्रत्यागम memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
