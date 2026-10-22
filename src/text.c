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
              unsigned صحيح *قيمة_rtrn)
{
    إذا (!string)
        إرجاع false;

    لكل (ثابت LookupEntry *entry = tab; entry->name; entry++) {
        إذا (istreq(entry->name, string)) {
            *قيمة_rtrn = entry->قيمة;
            إرجاع true;
        }
    }

    إرجاع false;
}

ثابت حرف *
Lookup_قيمة(ثابت LookupEntry tab[], unsigned صحيح قيمة)
{
    لكل (ثابت LookupEntry *entry = tab; entry->name; entry++)
        إذا (entry->قيمة == قيمة)
            إرجاع entry->name;

    إرجاع عدم;
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
    { "StickyKeys", CONTROL_STICKY_مفاتيح },
    { "MouseKeys", CONTROL_MOUSE_مفاتيح },
    { "MouseKeysAccel", CONTROL_MOUSE_مفاتيح_ACCEL },
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

ثابت LookupEntry use_Mod_Map_قيمة_Names[] = {
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_مفتاح },
    { "Redirect", ACTION_TYPE_REDIRECT_مفتاح },
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
    إذا (ndx == XKB_MOD_INVALID)
        إرجاع "none";

    إذا (ndx == XKB_MOD_NONE)
        إرجاع "None";

    إذا (ndx >= mods->رقم_mods)
        إرجاع عدم;

    إرجاع xkb_atom_text(ctx, mods->mods[ndx].name);
}

ثابت حرف *
ActionTypeText(تعداد xkb_action_type type)
{
    ثابت حرف *name = Lookup_قيمة(actionTypeNames, type);
    إرجاع name ? name : "Private";
}

ثابت حرف *
KeysymText(بنية xkb_context *ctx, xkb_keysym_t sym)
{
    حرف *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    إرجاع buffer;
}

ثابت حرف *
مفتاح_Name_Text(بنية xkb_context *ctx, xkb_atom_t name)
{
    ثابت حرف *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    حرف *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    إرجاع buf;
}

ثابت حرف *
SIMatchText(تعداد xkb_match_operation type)
{
    إرجاع Lookup_قيمة(symInterpretMatchMaskNames, type);
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

    إذا (mask == 0)
        إرجاع "none";

    إذا (mask == MOD_REAL_MASK_ALL)
        إرجاع "all";

    إذا ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->رقم_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ثابت صحيح ret = snprintf(buf, حجم(buf), "0x%"PRIx32, mask);
        static_assert(حجم(mask) == 4 && حجم(buf) >= حجم("0xffffffff"),
                      "Buffer too small");
        أكد(ret >= 0 && (size_t) ret < حجم(buf));
        pos = (size_t) ret;
    } وإلا {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            صحيح ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            إذا (ret <= 0 || pos + (size_t)ret >= حجم(buf))
                اكسر;
            وإلا
                pos += (size_t)ret;
        }
    }
    pos++;

    إرجاع memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ثابت حرف *
LedStateMaskText(بنية xkb_context *ctx, ثابت LookupEntry *lookup,
                 تعداد xkb_state_component mask)
{
    حرف buf[1024];
    size_t pos = 0;

    إذا (mask == 0)
        إرجاع "0";

    لكل (unsigned صحيح i = 0; mask; i++) {
        صحيح ret;

        إذا (!(mask & (1u << i)))
            استمر;

        mask &= ~(1u << i);

        ثابت حرف* ثابت maskText = Lookup_قيمة(lookup, 1u << i);
        أكد(maskText != عدم);
        ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        إذا (ret <= 0 || pos + (size_t)ret >= حجم(buf))
            اكسر;
        وإلا
            pos += (size_t)ret;
    }
    pos++;

    إرجاع memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ثابت حرف *
ControlMaskText(بنية xkb_context *ctx, تعداد xkb_keymap_format format,
                تعداد xkb_action_controls mask)
{
    حرف buf[1024];
    size_t pos = 0;

    ثابت تعداد xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    إذا (mask == 0)
        إرجاع "none";

    إذا (mask == all_ctrls)
        إرجاع "all";

    ثابت uint8_t control_names_offset = format_control_names_offset(format);
    لكل (unsigned صحيح i = 0; mask; i++) {
        صحيح ret;

        إذا (!(mask & (1u << i)))
            استمر;

        mask &= ~(1u << i);

        ثابت حرف* ثابت maskText =
            Lookup_قيمة(ctrlMaskNames + control_names_offset, 1u << i);
        أكد(maskText);
        ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        إذا (ret <= 0 || pos + (size_t)ret >= حجم(buf))
            اكسر;
        وإلا
            pos += (size_t)ret;
    }
    pos++;

    إرجاع memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
