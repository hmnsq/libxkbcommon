/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "keysym.h"
#ଅନ୍ତର୍ଭାଵ "text.h"

bool
LookupString(ଅଚଳ LookupEntry tab[], ଅଚଳ ଅକ୍ଷର *string,
              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ମୂଲ୍ୟ_rtrn)
{
    ଯଦି (!string)
        ଫେରନ୍ତୁ false;

    ପାଇଁ (ଅଚଳ LookupEntry *entry = tab; entry->name; entry++) {
        ଯଦି (istreq(entry->name, string)) {
            *ମୂଲ୍ୟ_rtrn = entry->ମୂଲ୍ୟ;
            ଫେରନ୍ତୁ true;
        }
    }

    ଫେରନ୍ତୁ false;
}

ଅଚଳ ଅକ୍ଷର *
Lookup_ମୂଲ୍ୟ(ଅଚଳ LookupEntry tab[], unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ)
{
    ପାଇଁ (ଅଚଳ LookupEntry *entry = tab; entry->name; entry++)
        ଯଦି (entry->ମୂଲ୍ୟ == ମୂଲ୍ୟ)
            ଫେରନ୍ତୁ entry->name;

    ଫେରନ୍ତୁ ଖାଲି;
}

ଅଚଳ LookupEntry ctrlMaskNames[25] = {
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
    { "StickyKeys", CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ },
    { "MouseKeys", CONTROL_MOUSE_ଚାବିଗୁଡ଼ିକ },
    { "MouseKeysAccel", CONTROL_MOUSE_ଚାବିଗୁଡ଼ିକ_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_GROUP_LOCK },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { ଖାଲି, 0 }
};

ଅଚଳ LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { ଖାଲି, 0 }
};

ଅଚଳ LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { ଖାଲି, 0 }
};

ଅଚଳ LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { ଖାଲି, 0 }
};

ଅଚଳ LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { ଖାଲି, 0 }
};

ଅଚଳ LookupEntry use_Mod_Map_ମୂଲ୍ୟ_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { ଖାଲି, 0 }
};

ଅଚଳ LookupEntry actionTypeNames[] = {
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_ଚାବି },
    { "Redirect", ACTION_TYPE_REDIRECT_ଚାବି },
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
    { ଖାଲି, 0 },
};

ଅଚଳ LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { ଖାଲି, 0 },
};

ଅଚଳ ଅକ୍ଷର *
ModIndexText(ସଂରଚନା xkb_context *ctx, ଅଚଳ ସଂରଚନା xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    ଯଦି (ndx == XKB_MOD_INVALID)
        ଫେରନ୍ତୁ "none";

    ଯଦି (ndx == XKB_MOD_NONE)
        ଫେରନ୍ତୁ "None";

    ଯଦି (ndx >= mods->ଅଙ୍କ_mods)
        ଫେରନ୍ତୁ ଖାଲି;

    ଫେରନ୍ତୁ xkb_atom_text(ctx, mods->mods[ndx].name);
}

ଅଚଳ ଅକ୍ଷର *
ActionTypeText(ଗଣନା xkb_action_type type)
{
    ଅଚଳ ଅକ୍ଷର *name = Lookup_ମୂଲ୍ୟ(actionTypeNames, type);
    ଫେରନ୍ତୁ name ? name : "Private";
}

ଅଚଳ ଅକ୍ଷର *
KeysymText(ସଂରଚନା xkb_context *ctx, xkb_keysym_t sym)
{
    ଅକ୍ଷର *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    ଫେରନ୍ତୁ buffer;
}

ଅଚଳ ଅକ୍ଷର *
ଚାବି_Name_Text(ସଂରଚନା xkb_context *ctx, xkb_atom_t name)
{
    ଅଚଳ ଅକ୍ଷର *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    ଅକ୍ଷର *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    ଫେରନ୍ତୁ buf;
}

ଅଚଳ ଅକ୍ଷର *
SIMatchText(ଗଣନା xkb_match_operation type)
{
    ଫେରନ୍ତୁ Lookup_ମୂଲ୍ୟ(symInterpretMatchMaskNames, type);
}

ଅଚଳ ଅକ୍ଷର *
ModMaskText(ସଂରଚନା xkb_context *ctx, ଗଣନା mod_type type,
            ଅଚଳ ସଂରଚନା xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    ଅକ୍ଷର buf[1024] = {0};
    size_t pos = 0;
    ଅଚଳ ସଂରଚନା xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    ପ୍ରତିଜ୍ଞା(type == MOD_REAL || type == MOD_BOTH);

    ଯଦି (mask == 0)
        ଫେରନ୍ତୁ "none";

    ଯଦି (mask == MOD_REAL_MASK_ALL)
        ଫେରନ୍ତୁ "all";

    ଯଦି ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->ଅଙ୍କ_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = snprintf(buf, ଆକାର(buf), "0x%"PRIx32, mask);
        static_assert(ଆକାର(mask) == 4 && ଆକାର(buf) >= ଆକାର("0xffffffff"),
                      "Buffer too small");
        ପ୍ରତିଜ୍ଞା(ret >= 0 && (size_t) ret < ଆକାର(buf));
        pos = (size_t) ret;
    } ଅନ୍ୟଥା {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = snprintf(buf + pos, ଆକାର(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            ଯଦି (ret <= 0 || pos + (size_t)ret >= ଆକାର(buf))
                ଭାଙ୍ଗ;
            ଅନ୍ୟଥା
                pos += (size_t)ret;
        }
    }
    pos++;

    ଫେରନ୍ତୁ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ଅଚଳ ଅକ୍ଷର *
LedStateMaskText(ସଂରଚନା xkb_context *ctx, ଅଚଳ LookupEntry *lookup,
                 ଗଣନା xkb_state_component mask)
{
    ଅକ୍ଷର buf[1024];
    size_t pos = 0;

    ଯଦି (mask == 0)
        ଫେରନ୍ତୁ "0";

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; mask; i++) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

        ଯଦି (!(mask & (1u << i)))
            ଜାରି;

        mask &= ~(1u << i);

        ଅଚଳ ଅକ୍ଷର* ଅଚଳ maskText = Lookup_ମୂଲ୍ୟ(lookup, 1u << i);
        ପ୍ରତିଜ୍ଞା(maskText != ଖାଲି);
        ret = snprintf(buf + pos, ଆକାର(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        ଯଦି (ret <= 0 || pos + (size_t)ret >= ଆକାର(buf))
            ଭାଙ୍ଗ;
        ଅନ୍ୟଥା
            pos += (size_t)ret;
    }
    pos++;

    ଫେରନ୍ତୁ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ଅଚଳ ଅକ୍ଷର *
ControlMaskText(ସଂରଚନା xkb_context *ctx, ଗଣନା xkb_keymap_format format,
                ଗଣନା xkb_action_controls mask)
{
    ଅକ୍ଷର buf[1024];
    size_t pos = 0;

    ଅଚଳ ଗଣନା xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    ଯଦି (mask == 0)
        ଫେରନ୍ତୁ "none";

    ଯଦି (mask == all_ctrls)
        ଫେରନ୍ତୁ "all";

    ଅଚଳ uint8_t control_names_offset = format_control_names_offset(format);
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; mask; i++) {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

        ଯଦି (!(mask & (1u << i)))
            ଜାରି;

        mask &= ~(1u << i);

        ଅଚଳ ଅକ୍ଷର* ଅଚଳ maskText =
            Lookup_ମୂଲ୍ୟ(ctrlMaskNames + control_names_offset, 1u << i);
        ପ୍ରତିଜ୍ଞା(maskText);
        ret = snprintf(buf + pos, ଆକାର(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        ଯଦି (ret <= 0 || pos + (size_t)ret >= ଆକାର(buf))
            ଭାଙ୍ଗ;
        ଅନ୍ୟଥା
            pos += (size_t)ret;
    }
    pos++;

    ଫେରନ୍ତୁ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
