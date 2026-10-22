/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#समावेश "config.h"

#समावेश <assert.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "keymap.h"
#समावेश "keysym.h"
#समावेश "text.h"

bool
LookupString(स्थिर LookupEntry tab[], स्थिर अक्षर *string,
              अचिन्हित पूर्णांक *मूल्य_rtrn)
{
    अगर (!string)
        वापस false;

    क्रम (स्थिर LookupEntry *entry = tab; entry->name; entry++) {
        अगर (istreq(entry->name, string)) {
            *मूल्य_rtrn = entry->मूल्य;
            वापस true;
        }
    }

    वापस false;
}

स्थिर अक्षर *
Lookup_मूल्य(स्थिर LookupEntry tab[], अचिन्हित पूर्णांक मूल्य)
{
    क्रम (स्थिर LookupEntry *entry = tab; entry->name; entry++)
        अगर (entry->मूल्य == मूल्य)
            वापस entry->name;

    वापस शून्य;
}

स्थिर LookupEntry ctrlMaskNames[25] = {
    /* v2 keymap format only */
    { "Overlay3", CONTROL_OVERLAY3 },
    { "Overlay4", CONTROL_OVERLAY4 },
    { "Overlay5", CONTROL_OVERLAY5 },
    { "Overlay6", CONTROL_OVERLAY6 },
    { "Overlay7", CONTROL_OVERLAY7 },
    { "Overlay8", CONTROL_OVERLAY8 },
    { "all", CONTROL_ALL_BOOLEAN },
    /* v1 + v2 keymap formats */
    [CONTROL_NAMES_MIN_V1_सूचकांक] = { "RepeatKeys", CONTROL_REPEAT },
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
    { शून्य, 0 }
};

स्थिर LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { शून्य, 0 }
};

स्थिर LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { शून्य, 0 }
};

स्थिर LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { शून्य, 0 }
};

स्थिर LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { शून्य, 0 }
};

स्थिर LookupEntry use_Mod_Map_मूल्य_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { शून्य, 0 }
};

स्थिर LookupEntry actionTypeNames[] = {
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
    { शून्य, 0 },
};

स्थिर LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { शून्य, 0 },
};

स्थिर अक्षर *
Mod_सूचकांक_Text(काष्ठा xkb_context *ctx, स्थिर काष्ठा xkb_mod_set *mods,
             xkb_mod_सूचकांक_t ndx)
{
    अगर (ndx == XKB_MOD_INVALID)
        वापस "none";

    अगर (ndx == XKB_MOD_NONE)
        वापस "None";

    अगर (ndx >= mods->अंक_mods)
        वापस शून्य;

    वापस xkb_atom_text(ctx, mods->mods[ndx].name);
}

स्थिर अक्षर *
ActionTypeText(क्रमागत xkb_action_type type)
{
    स्थिर अक्षर *name = Lookup_मूल्य(actionTypeNames, type);
    वापस name ? name : "Private";
}

स्थिर अक्षर *
KeysymText(काष्ठा xkb_context *ctx, xkb_keysym_t sym)
{
    अक्षर *बफर = xkb_context_get_बफर(ctx, XKB_KEYSYM_NAME_MAX_आकार);
    xkb_keysym_get_name(sym, बफर, XKB_KEYSYM_NAME_MAX_आकार);
    वापस बफर;
}

स्थिर अक्षर *
कुंजी_Name_Text(काष्ठा xkb_context *ctx, xkb_atom_t name)
{
    स्थिर अक्षर *sname = xkb_atom_text(ctx, name);
    माप_प्रकार len = strlen_safe(sname) + 3;
    अक्षर *buf = xkb_context_get_बफर(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    वापस buf;
}

स्थिर अक्षर *
SIMatchText(क्रमागत xkb_match_operation type)
{
    वापस Lookup_मूल्य(symInterpretMatchMaskNames, type);
}

स्थिर अक्षर *
ModMaskText(काष्ठा xkb_context *ctx, क्रमागत mod_type type,
            स्थिर काष्ठा xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    अक्षर buf[1024] = {0};
    माप_प्रकार pos = 0;
    स्थिर काष्ठा xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    निश्चित(type == MOD_REAL || type == MOD_BOTH);

    अगर (mask == 0)
        वापस "none";

    अगर (mask == MOD_REAL_MASK_ALL)
        वापस "all";

    अगर ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->अंक_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        स्थिर पूर्णांक ret = snprintf(buf, माप(buf), "0x%"PRIx32, mask);
        static_assert(माप(mask) == 4 && माप(buf) >= माप("0xffffffff"),
                      "Buffer too small");
        निश्चित(ret >= 0 && (माप_प्रकार) ret < माप(buf));
        pos = (माप_प्रकार) ret;
    } अन्यथा {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            पूर्णांक ret = snprintf(buf + pos, माप(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            अगर (ret <= 0 || pos + (माप_प्रकार)ret >= माप(buf))
                अवरोध;
            अन्यथा
                pos += (माप_प्रकार)ret;
        }
    }
    pos++;

    वापस स_नकल(xkb_context_get_बफर(ctx, pos), buf, pos);
}

स्थिर अक्षर *
LedStateMaskText(काष्ठा xkb_context *ctx, स्थिर LookupEntry *lookup,
                 क्रमागत xkb_state_component mask)
{
    अक्षर buf[1024];
    माप_प्रकार pos = 0;

    अगर (mask == 0)
        वापस "0";

    क्रम (अचिन्हित पूर्णांक i = 0; mask; i++) {
        पूर्णांक ret;

        अगर (!(mask & (1u << i)))
            जारी;

        mask &= ~(1u << i);

        स्थिर अक्षर* स्थिर maskText = Lookup_मूल्य(lookup, 1u << i);
        निश्चित(maskText != शून्य);
        ret = snprintf(buf + pos, माप(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        अगर (ret <= 0 || pos + (माप_प्रकार)ret >= माप(buf))
            अवरोध;
        अन्यथा
            pos += (माप_प्रकार)ret;
    }
    pos++;

    वापस स_नकल(xkb_context_get_बफर(ctx, pos), buf, pos);
}

स्थिर अक्षर *
ControlMaskText(काष्ठा xkb_context *ctx, क्रमागत xkb_keymap_format format,
                क्रमागत xkb_action_controls mask)
{
    अक्षर buf[1024];
    माप_प्रकार pos = 0;

    स्थिर क्रमागत xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    अगर (mask == 0)
        वापस "none";

    अगर (mask == all_ctrls)
        वापस "all";

    स्थिर uint8_t control_names_offset = format_control_names_offset(format);
    क्रम (अचिन्हित पूर्णांक i = 0; mask; i++) {
        पूर्णांक ret;

        अगर (!(mask & (1u << i)))
            जारी;

        mask &= ~(1u << i);

        स्थिर अक्षर* स्थिर maskText =
            Lookup_मूल्य(ctrlMaskNames + control_names_offset, 1u << i);
        निश्चित(maskText);
        ret = snprintf(buf + pos, माप(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        अगर (ret <= 0 || pos + (माप_प्रकार)ret >= माप(buf))
            अवरोध;
        अन्यथा
            pos += (माप_प्रकार)ret;
    }
    pos++;

    वापस स_नकल(xkb_context_get_बफर(ctx, pos), buf, pos);
}
