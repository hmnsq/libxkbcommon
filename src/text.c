/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "keysym.h"
#અન્તર્ભાવ "text.h"

bool
LookupString(અચલ LookupEntry tab[], અચલ અક્ષર *string,
              unsigned પૂર્ણાંક *કિંમત_rtrn)
{
    જો (!string)
        પાછા false;

    માટે (અચલ LookupEntry *entry = tab; entry->name; entry++) {
        જો (istreq(entry->name, string)) {
            *કિંમત_rtrn = entry->કિંમત;
            પાછા true;
        }
    }

    પાછા false;
}

અચલ અક્ષર *
Lookup_કિંમત(અચલ LookupEntry tab[], unsigned પૂર્ણાંક કિંમત)
{
    માટે (અચલ LookupEntry *entry = tab; entry->name; entry++)
        જો (entry->કિંમત == કિંમત)
            પાછા entry->name;

    પાછા ખાલી;
}

અચલ LookupEntry ctrlMaskNames[25] = {
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
    { "StickyKeys", CONTROL_STICKY_ચાવીઓ },
    { "MouseKeys", CONTROL_MOUSE_ચાવીઓ },
    { "MouseKeysAccel", CONTROL_MOUSE_ચાવીઓ_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_GROUP_LOCK },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { ખાલી, 0 }
};

અચલ LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { ખાલી, 0 }
};

અચલ LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { ખાલી, 0 }
};

અચલ LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { ખાલી, 0 }
};

અચલ LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { ખાલી, 0 }
};

અચલ LookupEntry use_Mod_Map_કિંમત_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { ખાલી, 0 }
};

અચલ LookupEntry actionTypeNames[] = {
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_ચાવી },
    { "Redirect", ACTION_TYPE_REDIRECT_ચાવી },
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
    { ખાલી, 0 },
};

અચલ LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { ખાલી, 0 },
};

અચલ અક્ષર *
ModIndexText(સંરચના xkb_context *ctx, અચલ સંરચના xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    જો (ndx == XKB_MOD_INVALID)
        પાછા "none";

    જો (ndx == XKB_MOD_NONE)
        પાછા "None";

    જો (ndx >= mods->અંક_mods)
        પાછા ખાલી;

    પાછા xkb_atom_text(ctx, mods->mods[ndx].name);
}

અચલ અક્ષર *
ActionTypeText(ગણના xkb_action_type type)
{
    અચલ અક્ષર *name = Lookup_કિંમત(actionTypeNames, type);
    પાછા name ? name : "Private";
}

અચલ અક્ષર *
KeysymText(સંરચના xkb_context *ctx, xkb_keysym_t sym)
{
    અક્ષર *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    પાછા buffer;
}

અચલ અક્ષર *
ચાવી_Name_Text(સંરચના xkb_context *ctx, xkb_atom_t name)
{
    અચલ અક્ષર *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    અક્ષર *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    પાછા buf;
}

અચલ અક્ષર *
SIMatchText(ગણના xkb_match_operation type)
{
    પાછા Lookup_કિંમત(symInterpretMatchMaskNames, type);
}

અચલ અક્ષર *
ModMaskText(સંરચના xkb_context *ctx, ગણના mod_type type,
            અચલ સંરચના xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    અક્ષર buf[1024] = {0};
    size_t pos = 0;
    અચલ સંરચના xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    પ્રતિજ્ઞા(type == MOD_REAL || type == MOD_BOTH);

    જો (mask == 0)
        પાછા "none";

    જો (mask == MOD_REAL_MASK_ALL)
        પાછા "all";

    જો ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->અંક_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        અચલ પૂર્ણાંક ret = snprintf(buf, કદ(buf), "0x%"PRIx32, mask);
        static_assert(કદ(mask) == 4 && કદ(buf) >= કદ("0xffffffff"),
                      "Buffer too small");
        પ્રતિજ્ઞા(ret >= 0 && (size_t) ret < કદ(buf));
        pos = (size_t) ret;
    } નહીંતર {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            પૂર્ણાંક ret = snprintf(buf + pos, કદ(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            જો (ret <= 0 || pos + (size_t)ret >= કદ(buf))
                તોડો;
            નહીંતર
                pos += (size_t)ret;
        }
    }
    pos++;

    પાછા memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

અચલ અક્ષર *
LedStateMaskText(સંરચના xkb_context *ctx, અચલ LookupEntry *lookup,
                 ગણના xkb_state_component mask)
{
    અક્ષર buf[1024];
    size_t pos = 0;

    જો (mask == 0)
        પાછા "0";

    માટે (unsigned પૂર્ણાંક i = 0; mask; i++) {
        પૂર્ણાંક ret;

        જો (!(mask & (1u << i)))
            ચાલુ;

        mask &= ~(1u << i);

        અચલ અક્ષર* અચલ maskText = Lookup_કિંમત(lookup, 1u << i);
        પ્રતિજ્ઞા(maskText != ખાલી);
        ret = snprintf(buf + pos, કદ(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        જો (ret <= 0 || pos + (size_t)ret >= કદ(buf))
            તોડો;
        નહીંતર
            pos += (size_t)ret;
    }
    pos++;

    પાછા memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

અચલ અક્ષર *
ControlMaskText(સંરચના xkb_context *ctx, ગણના xkb_keymap_format format,
                ગણના xkb_action_controls mask)
{
    અક્ષર buf[1024];
    size_t pos = 0;

    અચલ ગણના xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    જો (mask == 0)
        પાછા "none";

    જો (mask == all_ctrls)
        પાછા "all";

    અચલ uint8_t control_names_offset = format_control_names_offset(format);
    માટે (unsigned પૂર્ણાંક i = 0; mask; i++) {
        પૂર્ણાંક ret;

        જો (!(mask & (1u << i)))
            ચાલુ;

        mask &= ~(1u << i);

        અચલ અક્ષર* અચલ maskText =
            Lookup_કિંમત(ctrlMaskNames + control_names_offset, 1u << i);
        પ્રતિજ્ઞા(maskText);
        ret = snprintf(buf + pos, કદ(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        જો (ret <= 0 || pos + (size_t)ret >= કદ(buf))
            તોડો;
        નહીંતર
            pos += (size_t)ret;
    }
    pos++;

    પાછા memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
