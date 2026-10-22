/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#شامل "config.h"

#شامل <assert.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "keymap.h"
#شامل "keysym.h"
#شامل "text.h"

bool
LookupString(ثابت LookupEntry tab[], ثابت نویسه *string,
              unsigned صحیح *مقدار_rtrn)
{
    اگر (!string)
        بازگشت false;

    برای (ثابت LookupEntry *entry = tab; entry->name; entry++) {
        اگر (istreq(entry->name, string)) {
            *مقدار_rtrn = entry->مقدار;
            بازگشت true;
        }
    }

    بازگشت false;
}

ثابت نویسه *
Lookup_مقدار(ثابت LookupEntry tab[], unsigned صحیح مقدار)
{
    برای (ثابت LookupEntry *entry = tab; entry->name; entry++)
        اگر (entry->مقدار == مقدار)
            بازگشت entry->name;

    بازگشت پوچ;
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
    { "StickyKeys", CONTROL_STICKY_کلیدها },
    { "MouseKeys", CONTROL_MOUSE_کلیدها },
    { "MouseKeysAccel", CONTROL_MOUSE_کلیدها_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_GROUP_LOCK },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { پوچ, 0 }
};

ثابت LookupEntry modComponentMaskNames[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { پوچ, 0 }
};

ثابت LookupEntry groupComponentMaskNames[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { پوچ, 0 }
};

ثابت LookupEntry groupMaskNames[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { پوچ, 0 }
};

ثابت LookupEntry buttonNames[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { پوچ, 0 }
};

ثابت LookupEntry use_Mod_Map_مقدار_Names[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { پوچ, 0 }
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
    { "RedirectKey", ACTION_TYPE_REDIRECT_کلید },
    { "Redirect", ACTION_TYPE_REDIRECT_کلید },
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
    { پوچ, 0 },
};

ثابت LookupEntry symInterpretMatchMaskNames[] = {
    { "NoneOf", MATCH_NONE },
    { "AnyOfOrNone", MATCH_ANY_OR_NONE },
    { "AnyOf", MATCH_ANY },
    { "AllOf", MATCH_ALL },
    { "Exactly", MATCH_EXACTLY },
    { پوچ, 0 },
};

ثابت نویسه *
ModIndexText(ساختار xkb_context *ctx, ثابت ساختار xkb_mod_set *mods,
             xkb_mod_index_t ndx)
{
    اگر (ndx == XKB_MOD_INVALID)
        بازگشت "none";

    اگر (ndx == XKB_MOD_NONE)
        بازگشت "None";

    اگر (ndx >= mods->رقم_mods)
        بازگشت پوچ;

    بازگشت xkb_atom_text(ctx, mods->mods[ndx].name);
}

ثابت نویسه *
ActionTypeText(شمارش xkb_action_type type)
{
    ثابت نویسه *name = Lookup_مقدار(actionTypeNames, type);
    بازگشت name ? name : "Private";
}

ثابت نویسه *
KeysymText(ساختار xkb_context *ctx, xkb_keysym_t sym)
{
    نویسه *buffer = xkb_context_get_buffer(ctx, XKB_KEYSYM_NAME_MAX_SIZE);
    xkb_keysym_get_name(sym, buffer, XKB_KEYSYM_NAME_MAX_SIZE);
    بازگشت buffer;
}

ثابت نویسه *
کلید_Name_Text(ساختار xkb_context *ctx, xkb_atom_t name)
{
    ثابت نویسه *sname = xkb_atom_text(ctx, name);
    size_t len = strlen_safe(sname) + 3;
    نویسه *buf = xkb_context_get_buffer(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    بازگشت buf;
}

ثابت نویسه *
SIMatchText(شمارش xkb_match_operation type)
{
    بازگشت Lookup_مقدار(symInterpretMatchMaskNames, type);
}

ثابت نویسه *
ModMaskText(ساختار xkb_context *ctx, شمارش mod_type type,
            ثابت ساختار xkb_mod_set *mods, xkb_mod_mask_t mask)
{
    نویسه buf[1024] = {0};
    size_t pos = 0;
    ثابت ساختار xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    أكد(type == MOD_REAL || type == MOD_BOTH);

    اگر (mask == 0)
        بازگشت "none";

    اگر (mask == MOD_REAL_MASK_ALL)
        بازگشت "all";

    اگر ((type == MOD_REAL && (mask & ~MOD_REAL_MASK_ALL)) ||
        unlikely(mask & ~((UINT64_C(1) << mods->رقم_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ثابت صحیح ret = snprintf(buf, اندازه(buf), "0x%"PRIx32, mask);
        static_assert(اندازه(mask) == 4 && اندازه(buf) >= اندازه("0xffffffff"),
                      "Buffer too small");
        أكد(ret >= 0 && (size_t) ret < اندازه(buf));
        pos = (size_t) ret;
    } وگرنه {
        /* Print known mods */
        xkb_mods_mask_foreach(mask, mod, mods) {
            صحیح ret = snprintf(buf + pos, اندازه(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_text(ctx, mod->name));
            اگر (ret <= 0 || pos + (size_t)ret >= اندازه(buf))
                بشکن;
            وگرنه
                pos += (size_t)ret;
        }
    }
    pos++;

    بازگشت memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ثابت نویسه *
LedStateMaskText(ساختار xkb_context *ctx, ثابت LookupEntry *lookup,
                 شمارش xkb_state_component mask)
{
    نویسه buf[1024];
    size_t pos = 0;

    اگر (mask == 0)
        بازگشت "0";

    برای (unsigned صحیح i = 0; mask; i++) {
        صحیح ret;

        اگر (!(mask & (1u << i)))
            ادامه;

        mask &= ~(1u << i);

        ثابت نویسه* ثابت maskText = Lookup_مقدار(lookup, 1u << i);
        أكد(maskText != پوچ);
        ret = snprintf(buf + pos, اندازه(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        اگر (ret <= 0 || pos + (size_t)ret >= اندازه(buf))
            بشکن;
        وگرنه
            pos += (size_t)ret;
    }
    pos++;

    بازگشت memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}

ثابت نویسه *
ControlMaskText(ساختار xkb_context *ctx, شمارش xkb_keymap_format format,
                شمارش xkb_action_controls mask)
{
    نویسه buf[1024];
    size_t pos = 0;

    ثابت شمارش xkb_action_controls all_ctrls = format_boolean_controls(format);
    mask &= all_ctrls;

    اگر (mask == 0)
        بازگشت "none";

    اگر (mask == all_ctrls)
        بازگشت "all";

    ثابت uint8_t control_names_offset = format_control_names_offset(format);
    برای (unsigned صحیح i = 0; mask; i++) {
        صحیح ret;

        اگر (!(mask & (1u << i)))
            ادامه;

        mask &= ~(1u << i);

        ثابت نویسه* ثابت maskText =
            Lookup_مقدار(ctrlMaskNames + control_names_offset, 1u << i);
        أكد(maskText);
        ret = snprintf(buf + pos, اندازه(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", maskText);
        اگر (ret <= 0 || pos + (size_t)ret >= اندازه(buf))
            بشکن;
        وگرنه
            pos += (size_t)ret;
    }
    pos++;

    بازگشت memcpy(xkb_context_get_buffer(ctx, pos), buf, pos);
}
