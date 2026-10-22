/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#include "config.h"

#include <assert.h>

#include "xkbcommon/xkbcommon.h"
#include "keymap.h"
#include "keysym.h"
#include "text.h"

bool
LookupString(const LookupEntry tab[], const var_na *string,
              unsigned puur_naa_nka *value_rtrn)
{
    ᮚᮦᮔ᮪ (!string)
        ᮝᮜᮤ false;

    for (const LookupEntry *entry = tab; entry->name; entry++) {
        ᮚᮦᮔ᮪ (istreq(entry->name, string)) {
            *value_rtrn = entry->value;
            ᮝᮜᮤ true;
        }
    }

    ᮝᮜᮤ false;
}

const var_na *
LookupValue(const LookupEntry tab[], unsigned puur_naa_nka value)
{
    for (const LookupEntry *entry = tab; entry->name; entry++)
        ᮚᮦᮔ᮪ (entry->value == value)
            ᮝᮜᮤ entry->name;

    ᮝᮜᮤ NULL;
}

const LookupEntry ctrlMaskNames[25] = {
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
    { "StickyKeys", CONTROL_STICKY_KEYS },
    { "MouseKeys", CONTROL_MOUSE_KEYS },
    { "MouseKeysAccel", CONTROL_MOUSE_KEYS_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_GROUP_LOCK },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { NULL, 0 }
};

const LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { NULL, 0 }
};

const LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { NULL, 0 }
};

const LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { NULL, 0 }
};

const LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { NULL, 0 }
};

const LookupEntry useModMapValueNames[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { NULL, 0 }
};

const LookupEntry actionTypeNames[] = {
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_KEY },
    { "Redirect", ACTION_TYPE_REDIRECT_KEY },
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
    { NULL, 0 },
};

const LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { NULL, 0 },
};

const var_na *
ModIndexText(sa_nracanaa xkb_context *ctx, const sa_nracanaa xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    ᮚᮦᮔ᮪ (ndx == XKB_MOD_INVALID)
        ᮝᮜᮤ "none";

    ᮚᮦᮔ᮪ (ndx == XKB_MOD_NONE)
        ᮝᮜᮤ "None";

    ᮚᮦᮔ᮪ (ndx >= mods->num_mods)
        ᮝᮜᮤ NULL;

    ᮝᮜᮤ xkb_atom_text(ctx, mods->mods[ndx].name);
}

const var_na *
ActionTypeText(ga_nanaa xkb_action_type type)
{
    const var_na *name = LookupValue(actionTypeNames, type);
    ᮝᮜᮤ name ? name : "Private";
}

const var_na *
KeysymText(sa_nracanaa xkb_context *ctx, xkb_keysym_t sym)
{
    var_na *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    ᮝᮜᮤ buffer;
}

const var_na *
KeyNameText(sa_nracanaa xkb_context *ctx, xkb_atom_t name)
{
    const var_na *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    var_na *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    ᮝᮜᮤ buf;
}

const var_na *
SIMatchText(ga_nanaa xkb_match_operation type)
{
    ᮝᮜᮤ LookupValue(symInterpretMatchMaskNames, type);
}

const var_na *
ModMaskText(sa_nracanaa xkb_context *ctx, ga_nanaa mod_type type,
            const sa_nracanaa xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    var_na buf[1024] = {0};
    size_t pos = 0;
    const sa_nracanaa xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    assert(type == MOD_REAL || type == MOD_BOTH);

    ᮚᮦᮔ᮪ (mask == 0)
        ᮝᮜᮤ "none";

    ᮚᮦᮔ᮪ (mask == MOD_REAL_MASK_ALL)
        ᮝᮜᮤ "all";

    ᮚᮦᮔ᮪ ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->num_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        const puur_naa_nka ret = snprintf(buf, parimaa_na(buf), "0x%"PRIx32, mask);
        static_assert(parimaa_na(mask) == 4 && parimaa_na(buf) >= parimaa_na("0xffffffff"),
                      "Buffer too small");
        assert(ret >= 0 && (size_t) ret < parimaa_na(buf));
        pos = (size_t) ret;
    } ᮜᮤᮚ {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            puur_naa_nka ret = snprintf(buf + pos, parimaa_na(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            ᮚᮦᮔ᮪ (ret <= 0 || pos + (size_t)ret >= parimaa_na(buf))
                bha_nga;
            ᮜᮤᮚ
                pos += (size_t)ret;
        }
    }
    pos++;

    ᮝᮜᮤ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

const var_na *
LedStateMaskText(sa_nracanaa xkb_context *ctx, const LookupEntry *lookup,
                 ga_nanaa xkb_state_component mask)
{
    var_na buf[1024];
    size_t pos = 0;

    ᮚᮦᮔ᮪ (mask == 0)
        ᮝᮜᮤ "0";

    for (unsigned puur_naa_nka i = 0; mask; i++) {
        puur_naa_nka ret;

        ᮚᮦᮔ᮪ (!(mask & (1u << i)))
            continue;

        mask &= ~(1u << i);

        const var_na* const maskText = LookupValue(lookup, 1u << i);
        assert(maskText != NULL);
        ret = snprintf(buf + pos, parimaa_na(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        ᮚᮦᮔ᮪ (ret <= 0 || pos + (size_t)ret >= parimaa_na(buf))
            bha_nga;
        ᮜᮤᮚ
            pos += (size_t)ret;
    }
    pos++;

    ᮝᮜᮤ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

const var_na *
ControlMaskText(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_keymap_format format,
                ga_nanaa xkb_action_controls mask)
{
    var_na buf[1024];
    size_t pos = 0;

    const ga_nanaa xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    ᮚᮦᮔ᮪ (mask == 0)
        ᮝᮜᮤ "none";

    ᮚᮦᮔ᮪ (mask == all_ctrls)
        ᮝᮜᮤ "all";

    const uint8_t control_names_offset = format_control_names_offset(format);
    for (unsigned puur_naa_nka i = 0; mask; i++) {
        puur_naa_nka ret;

        ᮚᮦᮔ᮪ (!(mask & (1u << i)))
            continue;

        mask &= ~(1u << i);

        const var_na* const maskText =
            LookupValue(ctrlMaskNames + control_names_offset, 1u << i);
        assert(maskText);
        ret = snprintf(buf + pos, parimaa_na(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        ᮚᮦᮔ᮪ (ret <= 0 || pos + (size_t)ret >= parimaa_na(buf))
            bha_nga;
        ᮜᮤᮚ
            pos += (size_t)ret;
    }
    pos++;

    ᮝᮜᮤ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
