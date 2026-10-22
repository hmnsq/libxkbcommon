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
LookupString(ثابت Lookup_اندراج tab[], ثابت حرف *string,
              غیرعلامتی صحیح *قدر_rtrn)
{
    اگر (!string)
        واپس false;

    برائے (ثابت Lookup_اندراج *اندراج_2 = tab; اندراج_2->name; اندراج_2++) {
        اگر (istreq(اندراج_2->name, string)) {
            *قدر_rtrn = اندراج_2->قدر;
            واپس true;
        }
    }

    واپس false;
}

ثابت حرف *
Lookup_قدر(ثابت Lookup_اندراج tab[], غیرعلامتی صحیح قدر)
{
    برائے (ثابت Lookup_اندراج *اندراج_2 = tab; اندراج_2->name; اندراج_2++)
        اگر (اندراج_2->قدر == قدر)
            واپس اندراج_2->name;

    واپس تہی;
}

ثابت Lookup_اندراج ctrl_نقاب_نام[25] = {
    /* v2 keymap format only */
    { "Overlay3", CONTROL_OVERLAY3 },
    { "Overlay4", CONTROL_OVERLAY4 },
    { "Overlay5", CONTROL_OVERLAY5 },
    { "Overlay6", CONTROL_OVERLAY6 },
    { "Overlay7", CONTROL_OVERLAY7 },
    { "Overlay8", CONTROL_OVERLAY8 },
    { "all", CONTROL_ALL_BOOLEAN },
    /* v1 + v2 keymap formats */
    [CONTROL_نام_MIN_V1_اشاریہ] = { "RepeatKeys", CONTROL_REPEAT },
    { "Repeat", CONTROL_REPEAT },
    { "AutoRepeat", CONTROL_REPEAT },
    { "SlowKeys", CONTROL_SLOW },
    { "BounceKeys", CONTROL_DEBOUNCE },
    { "StickyKeys", CONTROL_STICKY_کلیدیں },
    { "MouseKeys", CONTROL_MOUSE_کلیدیں },
    { "MouseKeysAccel", CONTROL_MOUSE_کلیدیں_ACCEL },
    { "AccessXKeys", CONTROL_AX },
    { "AccessXTimeout", CONTROL_AX_TIMEOUT },
    { "AccessXFeedback", CONTROL_AX_FEEDBACK },
    { "AudibleBell", CONTROL_BELL },
    { "IgnoreGroupLock", CONTROL_IGNORE_گروہ_تالا },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { تہی, 0 }
};

ثابت Lookup_اندراج mod_Component_نقاب_نام[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { تہی, 0 }
};

ثابت Lookup_اندراج گروہ_Component_نقاب_نام[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { تہی, 0 }
};

ثابت Lookup_اندراج گروہ_نقاب_نام[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { تہی, 0 }
};

ثابت Lookup_اندراج button_نام[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { تہی, 0 }
};

ثابت Lookup_اندراج use_Mod_Map_قدر_نام[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { تہی, 0 }
};

ثابت Lookup_اندراج action_Type_نام[] = {
    { "NoAction", ACTION_TYPE_NONE },
    { "VoidAction", ACTION_TYPE_VOID },
    { "SetMods", ACTION_TYPE_MOD_سیٹ },
    { "LatchMods", ACTION_TYPE_MOD_LATCH },
    { "LockMods", ACTION_TYPE_MOD_تالا },
    { "SetGroup", ACTION_TYPE_گروہ_سیٹ },
    { "LatchGroup", ACTION_TYPE_گروہ_LATCH },
    { "LockGroup", ACTION_TYPE_گروہ_تالا },
    { "MovePtr", ACTION_TYPE_PTR_MOVE },
    { "MovePointer", ACTION_TYPE_PTR_MOVE },
    { "PtrBtn", ACTION_TYPE_PTR_BUTTON },
    { "PointerButton", ACTION_TYPE_PTR_BUTTON },
    { "LockPtrBtn", ACTION_TYPE_PTR_تالا },
    { "LockPtrButton", ACTION_TYPE_PTR_تالا },
    { "LockPointerButton", ACTION_TYPE_PTR_تالا },
    { "LockPointerBtn", ACTION_TYPE_PTR_تالا },
    { "SetPtrDflt", ACTION_TYPE_PTR_DEFAULT },
    { "SetPointerDefault", ACTION_TYPE_PTR_DEFAULT },
    { "Terminate", ACTION_TYPE_TERMINATE },
    { "TerminateServer", ACTION_TYPE_TERMINATE },
    { "SwitchScreen", ACTION_TYPE_SWITCH_VT },
    { "SetControls", ACTION_TYPE_CTRL_سیٹ },
    { "LockControls", ACTION_TYPE_CTRL_تالا },
    { "RedirectKey", ACTION_TYPE_REDIRECT_کلید },
    { "Redirect", ACTION_TYPE_REDIRECT_کلید },
    { "Private", ACTION_TYPE_نجی },
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
    { تہی, 0 },
};

ثابت Lookup_اندراج sym_Interpret_مطابقت_نقاب_نام[] = {
    { "NoneOf", مطابقت_NONE },
    { "AnyOfOrNone", مطابقت_ANY_OR_NONE },
    { "AnyOf", مطابقت_ANY },
    { "AllOf", مطابقت_ALL },
    { "Exactly", مطابقت_EXACTLY },
    { تہی, 0 },
};

ثابت حرف *
Mod_اشاریہ_متن(ساخت xkb_context *ctx, ثابت ساخت xkb_mod_سیٹ *mods,
             xkb_mod_اشاریہ_t ndx)
{
    اگر (ndx == XKB_MOD_INVALID)
        واپس "none";

    اگر (ndx == XKB_MOD_NONE)
        واپس "None";

    اگر (ndx >= mods->عدد_mods)
        واپس تہی;

    واپس xkb_atom_متن(ctx, mods->mods[ndx].name);
}

ثابت حرف *
Action_Type_متن(شمار xkb_action_type type)
{
    ثابت حرف *name = Lookup_قدر(action_Type_نام, type);
    واپس name ? name : "Private";
}

ثابت حرف *
Keysym_متن(ساخت xkb_context *ctx, xkb_keysym_t sym)
{
    حرف *بفر = xkb_context_get_بفر(ctx, XKB_KEYSYM_NAME_MAX_سائز);
    xkb_keysym_get_name(sym, بفر, XKB_KEYSYM_NAME_MAX_سائز);
    واپس بفر;
}

ثابت حرف *
کلید_Name_متن(ساخت xkb_context *ctx, xkb_atom_t name)
{
    ثابت حرف *sname = xkb_atom_متن(ctx, name);
    سائز_t len = strlen_محفوظ(sname) + 3;
    حرف *buf = xkb_context_get_بفر(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    واپس buf;
}

ثابت حرف *
SIMatch_متن(شمار xkb_مطابقت_operation type)
{
    واپس Lookup_قدر(sym_Interpret_مطابقت_نقاب_نام, type);
}

ثابت حرف *
Mod_نقاب_متن(ساخت xkb_context *ctx, شمار mod_type type,
            ثابت ساخت xkb_mod_سیٹ *mods, xkb_mod_نقاب_t نقاب)
{
    حرف buf[1024] = {0};
    سائز_t pos = 0;
    ثابت ساخت xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    أكد(type == MOD_REAL || type == MOD_BOTH);

    اگر (نقاب == 0)
        واپس "none";

    اگر (نقاب == MOD_REAL_نقاب_ALL)
        واپس "all";

    اگر ((type == MOD_REAL && (نقاب & ~MOD_REAL_نقاب_ALL)) ||
        unlikely(نقاب & ~((UINT64_C(1) << mods->عدد_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ثابت صحیح ret = snprintf(buf, حجم(buf), "0x%"PRIx32, نقاب);
        static_assert(حجم(نقاب) == 4 && حجم(buf) >= حجم("0xffffffff"),
                      "Buffer too small");
        أكد(ret >= 0 && (سائز_t) ret < حجم(buf));
        pos = (سائز_t) ret;
    } ورنہ {
        /* Print known mods */
        xkb_mods_نقاب_foreach(نقاب, mod, mods) {
            صحیح ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_متن(ctx, mod->name));
            اگر (ret <= 0 || pos + (سائز_t)ret >= حجم(buf))
                توڑو;
            ورنہ
                pos += (سائز_t)ret;
        }
    }
    pos++;

    واپس حافظہ_نقل(xkb_context_get_بفر(ctx, pos), buf, pos);
}

ثابت حرف *
Led_State_نقاب_متن(ساخت xkb_context *ctx, ثابت Lookup_اندراج *lookup,
                 شمار xkb_state_component نقاب)
{
    حرف buf[1024];
    سائز_t pos = 0;

    اگر (نقاب == 0)
        واپس "0";

    برائے (غیرعلامتی صحیح i = 0; نقاب; i++) {
        صحیح ret;

        اگر (!(نقاب & (1u << i)))
            جاری;

        نقاب &= ~(1u << i);

        ثابت حرف* ثابت نقاب_متن = Lookup_قدر(lookup, 1u << i);
        أكد(نقاب_متن != تہی);
        ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", نقاب_متن);
        اگر (ret <= 0 || pos + (سائز_t)ret >= حجم(buf))
            توڑو;
        ورنہ
            pos += (سائز_t)ret;
    }
    pos++;

    واپس حافظہ_نقل(xkb_context_get_بفر(ctx, pos), buf, pos);
}

ثابت حرف *
Control_نقاب_متن(ساخت xkb_context *ctx, شمار xkb_keymap_format format,
                شمار xkb_action_controls نقاب)
{
    حرف buf[1024];
    سائز_t pos = 0;

    ثابت شمار xkb_action_controls all_ctrls = format_boolean_controls(format);
    نقاب &= all_ctrls;

    اگر (نقاب == 0)
        واپس "none";

    اگر (نقاب == all_ctrls)
        واپس "all";

    ثابت uint8_t control_نام_انحراف = format_control_نام_انحراف(format);
    برائے (غیرعلامتی صحیح i = 0; نقاب; i++) {
        صحیح ret;

        اگر (!(نقاب & (1u << i)))
            جاری;

        نقاب &= ~(1u << i);

        ثابت حرف* ثابت نقاب_متن =
            Lookup_قدر(ctrl_نقاب_نام + control_نام_انحراف, 1u << i);
        أكد(نقاب_متن);
        ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", نقاب_متن);
        اگر (ret <= 0 || pos + (سائز_t)ret >= حجم(buf))
            توڑو;
        ورنہ
            pos += (سائز_t)ret;
    }
    pos++;

    واپس حافظہ_نقل(xkb_context_get_بفر(ctx, pos), buf, pos);
}
