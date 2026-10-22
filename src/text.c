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
LookupString(अचर LookupEntry tab[], अचर वर्ण *string,
              अचिन्हित पूर्णाङ्क *मान_rtrn)
{
    यदि (!string)
        फर्काऊ false;

    लागि (अचर LookupEntry *entry = tab; entry->name; entry++) {
        यदि (istreq(entry->name, string)) {
            *मान_rtrn = entry->मान;
            फर्काऊ true;
        }
    }

    फर्काऊ false;
}

अचर वर्ण *
Lookup_मान(अचर LookupEntry tab[], अचिन्हित पूर्णाङ्क मान)
{
    लागि (अचर LookupEntry *entry = tab; entry->name; entry++)
        यदि (entry->मान == मान)
            फर्काऊ entry->name;

    फर्काऊ रिक्त;
}

अचर LookupEntry ctrlMaskNames[25] = {
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
    { "StickyKeys", CONTROL_STICKY_साँचोहरू },
    { "MouseKeys", CONTROL_MOUSE_साँचोहरू },
    { "MouseKeysAccel", CONTROL_MOUSE_साँचोहरू_ACCEL },
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

अचर LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { रिक्त, 0 }
};

अचर LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { रिक्त, 0 }
};

अचर LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { रिक्त, 0 }
};

अचर LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { रिक्त, 0 }
};

अचर LookupEntry use_Mod_Map_मान_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { रिक्त, 0 }
};

अचर LookupEntry actionTypeNames[] = {
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_साँचो },
    { "Redirect", ACTION_TYPE_REDIRECT_साँचो },
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

अचर LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { रिक्त, 0 },
};

अचर वर्ण *
ModIndexText(संरचना xkb_context *ctx, अचर संरचना xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    यदि (ndx == XKB_MOD_INVALID)
        फर्काऊ "none";

    यदि (ndx == XKB_MOD_NONE)
        फर्काऊ "None";

    यदि (ndx >= mods->अङ्क_mods)
        फर्काऊ रिक्त;

    फर्काऊ xkb_atom_text(ctx, mods->mods[ndx].name);
}

अचर वर्ण *
ActionTypeText(गणना xkb_action_type type)
{
    अचर वर्ण *name = Lookup_मान(actionTypeNames, type);
    फर्काऊ name ? name : "Private";
}

अचर वर्ण *
KeysymText(संरचना xkb_context *ctx, xkb_keysym_t sym)
{
    वर्ण *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    फर्काऊ buffer;
}

अचर वर्ण *
साँचो_Name_Text(संरचना xkb_context *ctx, xkb_atom_t name)
{
    अचर वर्ण *sname = xkb_atom_text(ctx, name);
    माप_प्रकार len = strlen_safe(sname) + 3;
    वर्ण *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    फर्काऊ buf;
}

अचर वर्ण *
SIMatchText(गणना xkb_match_operation type)
{
    फर्काऊ Lookup_मान(symInterpretMatchMaskNames, type);
}

अचर वर्ण *
ModMaskText(संरचना xkb_context *ctx, गणना mod_type type,
            अचर संरचना xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    वर्ण buf[1024] = {0};
    माप_प्रकार pos = 0;
    अचर संरचना xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    प्रतिज्ञा(type == MOD_REAL || type == MOD_BOTH);

    यदि (mask == 0)
        फर्काऊ "none";

    यदि (mask == MOD_REAL_MASK_ALL)
        फर्काऊ "all";

    यदि ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->अङ्क_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        अचर पूर्णाङ्क ret = snprintf(buf, परिमाण(buf), "0x%"PRIx32, mask);
        static_assert(परिमाण(mask) == 4 && परिमाण(buf) >= परिमाण("0xffffffff"),
                      "Buffer too small");
        प्रतिज्ञा(ret >= 0 && (माप_प्रकार) ret < परिमाण(buf));
        pos = (माप_प्रकार) ret;
    } अन्यथा {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            पूर्णाङ्क ret = snprintf(buf + pos, परिमाण(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            यदि (ret <= 0 || pos + (माप_प्रकार)ret >= परिमाण(buf))
                रोक;
            अन्यथा
                pos += (माप_प्रकार)ret;
        }
    }
    pos++;

    फर्काऊ स_नकल(xkb_context_get_buffer(ctx, pos), buf, pos);
}

अचर वर्ण *
LedStateMaskText(संरचना xkb_context *ctx, अचर LookupEntry *lookup,
                 गणना xkb_state_component mask)
{
    वर्ण buf[1024];
    माप_प्रकार pos = 0;

    यदि (mask == 0)
        फर्काऊ "0";

    लागि (अचिन्हित पूर्णाङ्क i = 0; mask; i++) {
        पूर्णाङ्क ret;

        यदि (!(mask & (1u << i)))
            जारी;

        mask &= ~(1u << i);

        अचर वर्ण* अचर maskText = Lookup_मान(lookup, 1u << i);
        प्रतिज्ञा(maskText != रिक्त);
        ret = snprintf(buf + pos, परिमाण(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        यदि (ret <= 0 || pos + (माप_प्रकार)ret >= परिमाण(buf))
            रोक;
        अन्यथा
            pos += (माप_प्रकार)ret;
    }
    pos++;

    फर्काऊ स_नकल(xkb_context_get_buffer(ctx, pos), buf, pos);
}

अचर वर्ण *
ControlMaskText(संरचना xkb_context *ctx, गणना xkb_keymap_format format,
                गणना xkb_action_controls mask)
{
    वर्ण buf[1024];
    माप_प्रकार pos = 0;

    अचर गणना xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    यदि (mask == 0)
        फर्काऊ "none";

    यदि (mask == all_ctrls)
        फर्काऊ "all";

    अचर uint8_t control_names_offset = format_control_names_offset(format);
    लागि (अचिन्हित पूर्णाङ्क i = 0; mask; i++) {
        पूर्णाङ्क ret;

        यदि (!(mask & (1u << i)))
            जारी;

        mask &= ~(1u << i);

        अचर वर्ण* अचर maskText =
            Lookup_मान(ctrlMaskNames + control_names_offset, 1u << i);
        प्रतिज्ञा(maskText);
        ret = snprintf(buf + pos, परिमाण(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        यदि (ret <= 0 || pos + (माप_प्रकार)ret >= परिमाण(buf))
            रोक;
        अन्यथा
            pos += (माप_प्रकार)ret;
    }
    pos++;

    फर्काऊ स_नकल(xkb_context_get_buffer(ctx, pos), buf, pos);
}
