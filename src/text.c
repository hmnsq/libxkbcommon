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
LookupString(ध्रुव Lookup_प्रविष्टिः tab[], ध्रुव वर्ण *string,
              अचिह्नित पूर्णाङ्क *मानम्_rtrn)
{
    यदि (!string)
        प्रत्यागम false;

    प्रति (ध्रुव Lookup_प्रविष्टिः *प्रविष्टिः = tab; प्रविष्टिः->name; प्रविष्टिः++) {
        यदि (istreq(प्रविष्टिः->name, string)) {
            *मानम्_rtrn = प्रविष्टिः->मानम्;
            प्रत्यागम true;
        }
    }

    प्रत्यागम false;
}

ध्रुव वर्ण *
Lookup_मानम्(ध्रुव Lookup_प्रविष्टिः tab[], अचिह्नित पूर्णाङ्क मानम्)
{
    प्रति (ध्रुव Lookup_प्रविष्टिः *प्रविष्टिः = tab; प्रविष्टिः->name; प्रविष्टिः++)
        यदि (प्रविष्टिः->मानम् == मानम्)
            प्रत्यागम प्रविष्टिः->name;

    प्रत्यागम रिक्त;
}

ध्रुव Lookup_प्रविष्टिः ctrl_आवरणम्_नामानि[25] = {
    /* v2 keymap format only */
    { "Overlay3", CONTROL_OVERLAY3 },
    { "Overlay4", CONTROL_OVERLAY4 },
    { "Overlay5", CONTROL_OVERLAY5 },
    { "Overlay6", CONTROL_OVERLAY6 },
    { "Overlay7", CONTROL_OVERLAY7 },
    { "Overlay8", CONTROL_OVERLAY8 },
    { "all", CONTROL_ALL_BOOLEAN },
    /* v1 + v2 keymap formats */
    [CONTROL_नामानि_MIN_V1_सूचकांक] = { "RepeatKeys", CONTROL_REPEAT },
    { "Repeat", CONTROL_REPEAT },
    { "AutoRepeat", CONTROL_REPEAT },
    { "SlowKeys", CONTROL_SLOW },
    { "BounceKeys", CONTROL_DEBOUNCE },
    { "StickyKeys", CONTROL_STICKY_कुञ्चिकाः },
    { "MouseKeys", CONTROL_MOUSE_कुञ्चिकाः },
    { "MouseKeysAccel", CONTROL_MOUSE_कुञ्चिकाः_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_गणः_तालकम् },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { रिक्त, 0 }
};

ध्रुव Lookup_प्रविष्टिः mod_Component_आवरणम्_नामानि[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { रिक्त, 0 }
};

ध्रुव Lookup_प्रविष्टिः गणः_Component_आवरणम्_नामानि[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { रिक्त, 0 }
};

ध्रुव Lookup_प्रविष्टिः गणः_आवरणम्_नामानि[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { रिक्त, 0 }
};

ध्रुव Lookup_प्रविष्टिः button_नामानि[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { रिक्त, 0 }
};

ध्रुव Lookup_प्रविष्टिः use_Mod_Map_मानम्_नामानि[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { रिक्त, 0 }
};

ध्रुव Lookup_प्रविष्टिः action_Type_नामानि[] = {
    { "NoAction", ACTION_TYPE_NONE },
    { "VoidAction", ACTION_TYPE_VOID },
    { "SetMods", ACTION_TYPE_MOD_समुच्चयः },
    { "LatchMods", ACTION_TYPE_MOD_LATCH },
    { "LockMods", ACTION_TYPE_MOD_तालकम् },
    { "SetGroup", ACTION_TYPE_गणः_समुच्चयः },
    { "LatchGroup", ACTION_TYPE_गणः_LATCH },
    { "LockGroup", ACTION_TYPE_गणः_तालकम् },
    { "MovePtr", ACTION_TYPE_PTR_MOVE },
    { "MovePointer", ACTION_TYPE_PTR_MOVE },
    { "PtrBtn", ACTION_TYPE_PTR_BUTTON },
    { "PointerButton", ACTION_TYPE_PTR_BUTTON },
    { "LockPtrBtn", ACTION_TYPE_PTR_तालकम् },
    { "LockPtrButton", ACTION_TYPE_PTR_तालकम् },
    { "LockPointerButton", ACTION_TYPE_PTR_तालकम् },
    { "LockPointerBtn", ACTION_TYPE_PTR_तालकम् },
    { "SetPtrDflt", ACTION_TYPE_PTR_DEFAULT },
    { "SetPointerDefault", ACTION_TYPE_PTR_DEFAULT },
    { "Terminate", ACTION_TYPE_TERMINATE },
    { "TerminateServer", ACTION_TYPE_TERMINATE },
    { "SwitchScreen", ACTION_TYPE_SWITCH_VT },
    { "SetControls", ACTION_TYPE_CTRL_समुच्चयः },
    { "LockControls", ACTION_TYPE_CTRL_तालकम् },
    { "RedirectKey", ACTION_TYPE_REDIRECT_कुञ्चिका },
    { "Redirect", ACTION_TYPE_REDIRECT_कुञ्चिका },
    { "Private", ACTION_TYPE_वैयक्तिकम् },
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

ध्रुव Lookup_प्रविष्टिः sym_Interpret_मेलनम्_आवरणम्_नामानि[] = {
    { "NoneOf", मेलनम्_NONE },
    { "AnyOfOrNone", मेलनम्_ANY_OR_NONE },
    { "AnyOf", मेलनम्_ANY },
    { "AllOf", मेलनम्_ALL },
    { "Exactly", मेलनम्_EXACTLY },
    { रिक्त, 0 },
};

ध्रुव वर्ण *
Mod_सूचकांक_Text(संरचना xkb_context *ctx, ध्रुव संरचना xkb_mod_समुच्चयः *mods,
             xkb_mod_सूचकांक_t ndx)
{
    यदि (ndx == XKB_MOD_INVALID)
        प्रत्यागम "none";

    यदि (ndx == XKB_MOD_NONE)
        प्रत्यागम "None";

    यदि (ndx >= mods->अङ्कः_mods)
        प्रत्यागम रिक्त;

    प्रत्यागम xkb_atom_text(ctx, mods->mods[ndx].name);
}

ध्रुव वर्ण *
ActionTypeText(गणना xkb_action_type type)
{
    ध्रुव वर्ण *name = Lookup_मानम्(action_Type_नामानि, type);
    प्रत्यागम name ? name : "Private";
}

ध्रुव वर्ण *
KeysymText(संरचना xkb_context *ctx, xkb_keysym_t sym)
{
    वर्ण *बफर = xkb_context_get_बफर(ctx, XKB_KEYSYM_NAME_MAX_आकार);
    xkb_keysym_get_name(sym, बफर, XKB_KEYSYM_NAME_MAX_आकार);
    प्रत्यागम बफर;
}

ध्रुव वर्ण *
कुञ्चिका_Name_Text(संरचना xkb_context *ctx, xkb_atom_t name)
{
    ध्रुव वर्ण *sname = xkb_atom_text(ctx, name);
    आकार_t len = strlen_safe(sname) + 3;
    वर्ण *buf = xkb_context_get_बफर(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    प्रत्यागम buf;
}

ध्रुव वर्ण *
SIMatchText(गणना xkb_मेलनम्_operation type)
{
    प्रत्यागम Lookup_मानम्(sym_Interpret_मेलनम्_आवरणम्_नामानि, type);
}

ध्रुव वर्ण *
Mod_आवरणम्_Text(संरचना xkb_context *ctx, गणना mod_type type,
            ध्रुव संरचना xkb_mod_समुच्चयः *mods, xkb_mod_आवरणम्_t आवरणम्)
{
    वर्ण buf[1024] = {0};
    आकार_t pos = 0;
    ध्रुव संरचना xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    प्रतिज्ञा(type == MOD_REAL || type == MOD_BOTH);

    यदि (आवरणम् == 0)
        प्रत्यागम "none";

    यदि (आवरणम् == MOD_REAL_आवरणम्_ALL)
        प्रत्यागम "all";

    यदि ((type == MOD_REAL && (आवरणम् & ~MOD_REAL_आवरणम्_ALL)) ||
        unlikely(आवरणम् & ~((UINT64_C(1) << mods->अङ्कः_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ध्रुव पूर्णाङ्क ret = snprintf(buf, परिमाण(buf), "0x%"PRIx32, आवरणम्);
        static_assert(परिमाण(आवरणम्) == 4 && परिमाण(buf) >= परिमाण("0xffffffff"),
                      "Buffer too small");
        प्रतिज्ञा(ret >= 0 && (आकार_t) ret < परिमाण(buf));
        pos = (आकार_t) ret;
    } अन्यथा {
        /* Print known mods */
        xkb_mods_आवरणम्_foreach(आवरणम्, mod, mods) {
            पूर्णाङ्क ret = snprintf(buf + pos, परिमाण(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            यदि (ret <= 0 || pos + (आकार_t)ret >= परिमाण(buf))
                भङ्ग;
            अन्यथा
                pos += (आकार_t)ret;
        }
    }
    pos++;

    प्रत्यागम स्मृतिं_प्रतिलिख(xkb_context_get_बफर(ctx, pos), buf, pos);
}

ध्रुव वर्ण *
Led_State_आवरणम्_Text(संरचना xkb_context *ctx, ध्रुव Lookup_प्रविष्टिः *lookup,
                 गणना xkb_state_component आवरणम्)
{
    वर्ण buf[1024];
    आकार_t pos = 0;

    यदि (आवरणम् == 0)
        प्रत्यागम "0";

    प्रति (अचिह्नित पूर्णाङ्क i = 0; आवरणम्; i++) {
        पूर्णाङ्क ret;

        यदि (!(आवरणम् & (1u << i)))
            अनुवर्त;

        आवरणम् &= ~(1u << i);

        ध्रुव वर्ण* ध्रुव आवरणम्_Text = Lookup_मानम्(lookup, 1u << i);
        प्रतिज्ञा(आवरणम्_Text != रिक्त);
        ret = snprintf(buf + pos, परिमाण(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", आवरणम्_Text);
        यदि (ret <= 0 || pos + (आकार_t)ret >= परिमाण(buf))
            भङ्ग;
        अन्यथा
            pos += (आकार_t)ret;
    }
    pos++;

    प्रत्यागम स्मृतिं_प्रतिलिख(xkb_context_get_बफर(ctx, pos), buf, pos);
}

ध्रुव वर्ण *
Control_आवरणम्_Text(संरचना xkb_context *ctx, गणना xkb_keymap_format format,
                गणना xkb_action_controls आवरणम्)
{
    वर्ण buf[1024];
    आकार_t pos = 0;

    ध्रुव गणना xkb_action_controls all_ctrls = format_boolean_controls(format);
    आवरणम् &= all_ctrls;

    यदि (आवरणम् == 0)
        प्रत्यागम "none";

    यदि (आवरणम् == all_ctrls)
        प्रत्यागम "all";

    ध्रुव uint8_t control_नामानि_विस्थापनम् = format_control_नामानि_विस्थापनम्(format);
    प्रति (अचिह्नित पूर्णाङ्क i = 0; आवरणम्; i++) {
        पूर्णाङ्क ret;

        यदि (!(आवरणम् & (1u << i)))
            अनुवर्त;

        आवरणम् &= ~(1u << i);

        ध्रुव वर्ण* ध्रुव आवरणम्_Text =
            Lookup_मानम्(ctrl_आवरणम्_नामानि + control_नामानि_विस्थापनम्, 1u << i);
        प्रतिज्ञा(आवरणम्_Text);
        ret = snprintf(buf + pos, परिमाण(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", आवरणम्_Text);
        यदि (ret <= 0 || pos + (आकार_t)ret >= परिमाण(buf))
            भङ्ग;
        अन्यथा
            pos += (आकार_t)ret;
    }
    pos++;

    प्रत्यागम स्मृतिं_प्रतिलिख(xkb_context_get_बफर(ctx, pos), buf, pos);
}
