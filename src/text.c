/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ضمن "config.h"

#ضمن <assert.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "keymap.h"
#ضمن "keysym.h"
#ضمن "text.h"

bool
LookupString(ثابت LookupEntry tab[], ثابت حرف *string,
              unsigned صحيح *value_rtrn)
{
    ܐܢ (!string)
        ܦܢܐ false;

    لكل (ثابت LookupEntry *entry = tab; entry->name; entry++) {
        ܐܢ (istreq(entry->name, string)) {
            *value_rtrn = entry->value;
            ܦܢܐ true;
        }
    }

    ܦܢܐ false;
}

ثابت حرف *
LookupValue(ثابت LookupEntry tab[], unsigned صحيح value)
{
    لكل (ثابت LookupEntry *entry = tab; entry->name; entry++)
        ܐܢ (entry->value == value)
            ܦܢܐ entry->name;

    ܦܢܐ عدم;
}

ثابت LookupEntry ctrlMaskNames[25] = {
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
    { عدم, 0 }
};

ثابت LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { عدم, 0 }
};

ثابت LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { عدم, 0 }
};

ثابت LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { عدم, 0 }
};

ثابت LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { عدم, 0 }
};

ثابت LookupEntry useModMapValueNames[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { عدم, 0 }
};

ثابت LookupEntry actionTypeNames[] = {
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
    { عدم, 0 },
};

ثابت LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { عدم, 0 },
};

ثابت حرف *
ModIndexText(بنية xkb_context *ctx, ثابت بنية xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    ܐܢ (ndx == XKB_MOD_INVALID)
        ܦܢܐ "none";

    ܐܢ (ndx == XKB_MOD_NONE)
        ܦܢܐ "None";

    ܐܢ (ndx >= mods->num_mods)
        ܦܢܐ عدم;

    ܦܢܐ xkb_atom_text(ctx, mods->mods[ndx].name);
}

ثابت حرف *
ActionTypeText(تعداد xkb_action_type type)
{
    ثابت حرف *name = LookupValue(actionTypeNames, type);
    ܦܢܐ name ? name : "Private";
}

ثابت حرف *
KeysymText(بنية xkb_context *ctx, xkb_keysym_t sym)
{
    حرف *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    ܦܢܐ buffer;
}

ثابت حرف *
KeyNameText(بنية xkb_context *ctx, xkb_atom_t name)
{
    ثابت حرف *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    حرف *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    ܦܢܐ buf;
}

ثابت حرف *
SIMatchText(تعداد xkb_match_operation type)
{
    ܦܢܐ LookupValue(symInterpretMatchMaskNames, type);
}

ثابت حرف *
ModMaskText(بنية xkb_context *ctx, تعداد mod_type type,
            ثابت بنية xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    حرف buf[1024] = {0};
    size_t pos = 0;
    ثابت بنية xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    أكد(type == MOD_REAL || type == MOD_BOTH);

    ܐܢ (mask == 0)
        ܦܢܐ "none";

    ܐܢ (mask == MOD_REAL_MASK_ALL)
        ܦܢܐ "all";

    ܐܢ ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->num_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ثابت صحيح ret = snprintf(buf, حجم(buf), "0x%"PRIx32, mask);
        static_assert(حجم(mask) == 4 && حجم(buf) >= حجم("0xffffffff"),
                      "Buffer too small");
        أكد(ret >= 0 && (size_t) ret < حجم(buf));
        pos = (size_t) ret;
    } ܐܚܪܢܐ {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            صحيح ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            ܐܢ (ret <= 0 || pos + (size_t)ret >= حجم(buf))
                اكسر;
            ܐܚܪܢܐ
                pos += (size_t)ret;
        }
    }
    pos++;

    ܦܢܐ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ثابت حرف *
LedStateMaskText(بنية xkb_context *ctx, ثابت LookupEntry *lookup,
                 تعداد xkb_state_component mask)
{
    حرف buf[1024];
    size_t pos = 0;

    ܐܢ (mask == 0)
        ܦܢܐ "0";

    لكل (unsigned صحيح i = 0; mask; i++) {
        صحيح ret;

        ܐܢ (!(mask & (1u << i)))
            استمر;

        mask &= ~(1u << i);

        ثابت حرف* ثابت maskText = LookupValue(lookup, 1u << i);
        أكد(maskText != عدم);
        ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        ܐܢ (ret <= 0 || pos + (size_t)ret >= حجم(buf))
            اكسر;
        ܐܚܪܢܐ
            pos += (size_t)ret;
    }
    pos++;

    ܦܢܐ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ثابت حرف *
ControlMaskText(بنية xkb_context *ctx, تعداد xkb_keymap_format format,
                تعداد xkb_action_controls mask)
{
    حرف buf[1024];
    size_t pos = 0;

    ثابت تعداد xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    ܐܢ (mask == 0)
        ܦܢܐ "none";

    ܐܢ (mask == all_ctrls)
        ܦܢܐ "all";

    ثابت uint8_t control_names_offset = format_control_names_offset(format);
    لكل (unsigned صحيح i = 0; mask; i++) {
        صحيح ret;

        ܐܢ (!(mask & (1u << i)))
            استمر;

        mask &= ~(1u << i);

        ثابت حرف* ثابت maskText =
            LookupValue(ctrlMaskNames + control_names_offset, 1u << i);
        أكد(maskText);
        ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        ܐܢ (ret <= 0 || pos + (size_t)ret >= حجم(buf))
            اكسر;
        ܐܚܪܢܐ
            pos += (size_t)ret;
    }
    pos++;

    ܦܢܐ memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
