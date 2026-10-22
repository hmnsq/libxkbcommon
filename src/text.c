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
LookupString(स्थिरांक LookupEntry tab[], स्थिरांक अक्षर *string,
              अचिन्हित पूर्णांक *मूल्य_rtrn)
{
    जर (!string)
        परत false;

    साठी (स्थिरांक LookupEntry *entry = tab; entry->name; entry++) {
        जर (istreq(entry->name, string)) {
            *मूल्य_rtrn = entry->मूल्य;
            परत true;
        }
    }

    परत false;
}

स्थिरांक अक्षर *
Lookup_मूल्य(स्थिरांक LookupEntry tab[], अचिन्हित पूर्णांक मूल्य)
{
    साठी (स्थिरांक LookupEntry *entry = tab; entry->name; entry++)
        जर (entry->मूल्य == मूल्य)
            परत entry->name;

    परत रिक्त;
}

स्थिरांक LookupEntry ctrlMaskNames[25] = {
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
    { "StickyKeys", CONTROL_STICKY_किल्ल्या },
    { "MouseKeys", CONTROL_MOUSE_किल्ल्या },
    { "MouseKeysAccel", CONTROL_MOUSE_किल्ल्या_ACCEL },
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

स्थिरांक LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { रिक्त, 0 }
};

स्थिरांक LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { रिक्त, 0 }
};

स्थिरांक LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { रिक्त, 0 }
};

स्थिरांक LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { रिक्त, 0 }
};

स्थिरांक LookupEntry use_Mod_Map_मूल्य_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { रिक्त, 0 }
};

स्थिरांक LookupEntry actionTypeNames[] = {
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_किल्ली },
    { "Redirect", ACTION_TYPE_REDIRECT_किल्ली },
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

स्थिरांक LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { रिक्त, 0 },
};

स्थिरांक अक्षर *
ModIndexText(संरचना xkb_context *ctx, स्थिरांक संरचना xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    जर (ndx == XKB_MOD_INVALID)
        परत "none";

    जर (ndx == XKB_MOD_NONE)
        परत "None";

    जर (ndx >= mods->अंक_mods)
        परत रिक्त;

    परत xkb_atom_text(ctx, mods->mods[ndx].name);
}

स्थिरांक अक्षर *
ActionTypeText(गणना xkb_action_type type)
{
    स्थिरांक अक्षर *name = Lookup_मूल्य(actionTypeNames, type);
    परत name ? name : "Private";
}

स्थिरांक अक्षर *
KeysymText(संरचना xkb_context *ctx, xkb_keysym_t sym)
{
    अक्षर *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    परत buffer;
}

स्थिरांक अक्षर *
किल्ली_Name_Text(संरचना xkb_context *ctx, xkb_atom_t name)
{
    स्थिरांक अक्षर *sname = xkb_atom_text(ctx, name);
    माप_प्रकार len = strlen_safe(sname) + 3;
    अक्षर *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    परत buf;
}

स्थिरांक अक्षर *
SIMatchText(गणना xkb_match_operation type)
{
    परत Lookup_मूल्य(symInterpretMatchMaskNames, type);
}

स्थिरांक अक्षर *
ModMaskText(संरचना xkb_context *ctx, गणना mod_type type,
            स्थिरांक संरचना xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    अक्षर buf[1024] = {0};
    माप_प्रकार pos = 0;
    स्थिरांक संरचना xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    प्रतिज्ञा(type == MOD_REAL || type == MOD_BOTH);

    जर (mask == 0)
        परत "none";

    जर (mask == MOD_REAL_MASK_ALL)
        परत "all";

    जर ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->अंक_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        स्थिरांक पूर्णांक ret = snprintf(buf, आकार(buf), "0x%"PRIx32, mask);
        static_assert(आकार(mask) == 4 && आकार(buf) >= आकार("0xffffffff"),
                      "Buffer too small");
        प्रतिज्ञा(ret >= 0 && (माप_प्रकार) ret < आकार(buf));
        pos = (माप_प्रकार) ret;
    } अन्यथा {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            पूर्णांक ret = snprintf(buf + pos, आकार(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            जर (ret <= 0 || pos + (माप_प्रकार)ret >= आकार(buf))
                थांबा;
            अन्यथा
                pos += (माप_प्रकार)ret;
        }
    }
    pos++;

    परत स_नकल(xkb_context_get_buffer(ctx, pos), buf, pos);
}

स्थिरांक अक्षर *
LedStateMaskText(संरचना xkb_context *ctx, स्थिरांक LookupEntry *lookup,
                 गणना xkb_state_component mask)
{
    अक्षर buf[1024];
    माप_प्रकार pos = 0;

    जर (mask == 0)
        परत "0";

    साठी (अचिन्हित पूर्णांक i = 0; mask; i++) {
        पूर्णांक ret;

        जर (!(mask & (1u << i)))
            सुरूठेवा;

        mask &= ~(1u << i);

        स्थिरांक अक्षर* स्थिरांक maskText = Lookup_मूल्य(lookup, 1u << i);
        प्रतिज्ञा(maskText != रिक्त);
        ret = snprintf(buf + pos, आकार(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        जर (ret <= 0 || pos + (माप_प्रकार)ret >= आकार(buf))
            थांबा;
        अन्यथा
            pos += (माप_प्रकार)ret;
    }
    pos++;

    परत स_नकल(xkb_context_get_buffer(ctx, pos), buf, pos);
}

स्थिरांक अक्षर *
ControlMaskText(संरचना xkb_context *ctx, गणना xkb_keymap_format format,
                गणना xkb_action_controls mask)
{
    अक्षर buf[1024];
    माप_प्रकार pos = 0;

    स्थिरांक गणना xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    जर (mask == 0)
        परत "none";

    जर (mask == all_ctrls)
        परत "all";

    स्थिरांक uint8_t control_names_offset = format_control_names_offset(format);
    साठी (अचिन्हित पूर्णांक i = 0; mask; i++) {
        पूर्णांक ret;

        जर (!(mask & (1u << i)))
            सुरूठेवा;

        mask &= ~(1u << i);

        स्थिरांक अक्षर* स्थिरांक maskText =
            Lookup_मूल्य(ctrlMaskNames + control_names_offset, 1u << i);
        प्रतिज्ञा(maskText);
        ret = snprintf(buf + pos, आकार(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        जर (ret <= 0 || pos + (माप_प्रकार)ret >= आकार(buf))
            थांबा;
        अन्यथा
            pos += (माप_प्रकार)ret;
    }
    pos++;

    परत स_नकल(xkb_context_get_buffer(ctx, pos), buf, pos);
}
