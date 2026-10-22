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
LookupString(ثابت Lookup_اندراج tab[], ثابت حرف *string,
              بنانشانی صحيح *قدر_rtrn)
{
    إذا (!string)
        أرجع false;

    لكل (ثابت Lookup_اندراج *اندراج = tab; اندراج->name; اندراج++) {
        إذا (istreq(اندراج->name, string)) {
            *قدر_rtrn = اندراج->قدر;
            أرجع true;
        }
    }

    أرجع false;
}

ثابت حرف *
Lookup_قدر(ثابت Lookup_اندراج tab[], بنانشانی صحيح قدر)
{
    لكل (ثابت Lookup_اندراج *اندراج = tab; اندراج->name; اندراج++)
        إذا (اندراج->قدر == قدر)
            أرجع اندراج->name;

    أرجع عدم;
}

ثابت Lookup_اندراج ctrl_نقاب_ناں[25] = {
    /* v2 keymap format only */
    { "Overlay3", CONTROL_OVERLAY3 },
    { "Overlay4", CONTROL_OVERLAY4 },
    { "Overlay5", CONTROL_OVERLAY5 },
    { "Overlay6", CONTROL_OVERLAY6 },
    { "Overlay7", CONTROL_OVERLAY7 },
    { "Overlay8", CONTROL_OVERLAY8 },
    { "all", CONTROL_ALL_BOOLEAN },
    /* v1 + v2 keymap formats */
    [CONTROL_ناں_MIN_V1_اشاریہ] = { "RepeatKeys", CONTROL_REPEAT },
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
    { "IgnoreGroupLock", CONTROL_IGNORE_گروہ_جندرا },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { عدم, 0 }
};

ثابت Lookup_اندراج mod_Component_نقاب_ناں[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { عدم, 0 }
};

ثابت Lookup_اندراج گروہ_Component_نقاب_ناں[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { عدم, 0 }
};

ثابت Lookup_اندراج گروہ_نقاب_ناں[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { عدم, 0 }
};

ثابت Lookup_اندراج button_ناں[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { عدم, 0 }
};

ثابت Lookup_اندراج use_Mod_Map_قدر_ناں[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { عدم, 0 }
};

ثابت Lookup_اندراج action_Type_ناں[] = {
    { "NoAction", ACTION_TYPE_NONE },
    { "VoidAction", ACTION_TYPE_VOID },
    { "SetMods", ACTION_TYPE_MOD_سیٹ },
    { "LatchMods", ACTION_TYPE_MOD_LATCH },
    { "LockMods", ACTION_TYPE_MOD_جندرا },
    { "SetGroup", ACTION_TYPE_گروہ_سیٹ },
    { "LatchGroup", ACTION_TYPE_گروہ_LATCH },
    { "LockGroup", ACTION_TYPE_گروہ_جندرا },
    { "MovePtr", ACTION_TYPE_PTR_MOVE },
    { "MovePointer", ACTION_TYPE_PTR_MOVE },
    { "PtrBtn", ACTION_TYPE_PTR_BUTTON },
    { "PointerButton", ACTION_TYPE_PTR_BUTTON },
    { "LockPtrBtn", ACTION_TYPE_PTR_جندرا },
    { "LockPtrButton", ACTION_TYPE_PTR_جندرا },
    { "LockPointerButton", ACTION_TYPE_PTR_جندرا },
    { "LockPointerBtn", ACTION_TYPE_PTR_جندرا },
    { "SetPtrDflt", ACTION_TYPE_PTR_DEFAULT },
    { "SetPointerDefault", ACTION_TYPE_PTR_DEFAULT },
    { "Terminate", ACTION_TYPE_TERMINATE },
    { "TerminateServer", ACTION_TYPE_TERMINATE },
    { "SwitchScreen", ACTION_TYPE_SWITCH_VT },
    { "SetControls", ACTION_TYPE_CTRL_سیٹ },
    { "LockControls", ACTION_TYPE_CTRL_جندرا },
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
    { عدم, 0 },
};

ثابت Lookup_اندراج sym_Interpret_میل_نقاب_ناں[] = {
    { "NoneOf", میل_NONE },
    { "AnyOfOrNone", میل_ANY_OR_NONE },
    { "AnyOf", میل_ANY },
    { "AllOf", میل_ALL },
    { "Exactly", میل_EXACTLY },
    { عدم, 0 },
};

ثابت حرف *
Mod_اشاریہ_متن(بنية xkb_context *ctx, ثابت بنية xkb_mod_سیٹ *mods,
             xkb_mod_اشاریہ_t ndx)
{
    إذا (ndx == XKB_MOD_INVALID)
        أرجع "none";

    إذا (ndx == XKB_MOD_NONE)
        أرجع "None";

    إذا (ndx >= mods->عدد_mods)
        أرجع عدم;

    أرجع xkb_atom_متن(ctx, mods->mods[ndx].name);
}

ثابت حرف *
Action_Type_متن(تعداد xkb_action_type type)
{
    ثابت حرف *name = Lookup_قدر(action_Type_ناں, type);
    أرجع name ? name : "Private";
}

ثابت حرف *
Keysym_متن(بنية xkb_context *ctx, xkb_keysym_t sym)
{
    حرف *بفر = xkb_context_get_بفر(ctx, XKB_KEYSYM_NAME_MAX_ناپ);
    xkb_keysym_get_name(sym, بفر, XKB_KEYSYM_NAME_MAX_ناپ);
    أرجع بفر;
}

ثابت حرف *
کلید_Name_متن(بنية xkb_context *ctx, xkb_atom_t name)
{
    ثابت حرف *sname = xkb_atom_متن(ctx, name);
    ناپ_t len = strlen_محفوظ(sname) + 3;
    حرف *buf = xkb_context_get_بفر(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    أرجع buf;
}

ثابت حرف *
SIMatch_متن(تعداد xkb_میل_operation type)
{
    أرجع Lookup_قدر(sym_Interpret_میل_نقاب_ناں, type);
}

ثابت حرف *
Mod_نقاب_متن(بنية xkb_context *ctx, تعداد mod_type type,
            ثابت بنية xkb_mod_سیٹ *mods, xkb_mod_نقاب_t نقاب)
{
    حرف buf[1024] = {0};
    ناپ_t pos = 0;
    ثابت بنية xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    أكد(type == MOD_REAL || type == MOD_BOTH);

    إذا (نقاب == 0)
        أرجع "none";

    إذا (نقاب == MOD_REAL_نقاب_ALL)
        أرجع "all";

    إذا ((type == MOD_REAL && (نقاب & ~MOD_REAL_نقاب_ALL)) ||
        unlikely(نقاب & ~((UINT64_C(1) << mods->عدد_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ثابت صحيح ret = snprintf(buf, حجم(buf), "0x%"PRIx32, نقاب);
        static_assert(حجم(نقاب) == 4 && حجم(buf) >= حجم("0xffffffff"),
                      "Buffer too small");
        أكد(ret >= 0 && (ناپ_t) ret < حجم(buf));
        pos = (ناپ_t) ret;
    } وإلا {
        /* Print known mods */
        xkb_mods_نقاب_foreach(نقاب, mod, mods) {
            صحيح ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_متن(ctx, mod->name));
            إذا (ret <= 0 || pos + (ناپ_t)ret >= حجم(buf))
                اكسر;
            وإلا
                pos += (ناپ_t)ret;
        }
    }
    pos++;

    أرجع حافظہ_نقل(xkb_context_get_بفر(ctx, pos), buf, pos);
}

ثابت حرف *
Led_State_نقاب_متن(بنية xkb_context *ctx, ثابت Lookup_اندراج *lookup,
                 تعداد xkb_state_component نقاب)
{
    حرف buf[1024];
    ناپ_t pos = 0;

    إذا (نقاب == 0)
        أرجع "0";

    لكل (بنانشانی صحيح i = 0; نقاب; i++) {
        صحيح ret;

        إذا (!(نقاب & (1u << i)))
            استمر;

        نقاب &= ~(1u << i);

        ثابت حرف* ثابت نقاب_متن = Lookup_قدر(lookup, 1u << i);
        أكد(نقاب_متن != عدم);
        ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", نقاب_متن);
        إذا (ret <= 0 || pos + (ناپ_t)ret >= حجم(buf))
            اكسر;
        وإلا
            pos += (ناپ_t)ret;
    }
    pos++;

    أرجع حافظہ_نقل(xkb_context_get_بفر(ctx, pos), buf, pos);
}

ثابت حرف *
Control_نقاب_متن(بنية xkb_context *ctx, تعداد xkb_keymap_format format,
                تعداد xkb_action_controls نقاب)
{
    حرف buf[1024];
    ناپ_t pos = 0;

    ثابت تعداد xkb_action_controls all_ctrls = format_boolean_controls(format);
    نقاب &= all_ctrls;

    إذا (نقاب == 0)
        أرجع "none";

    إذا (نقاب == all_ctrls)
        أرجع "all";

    ثابت uint8_t control_ناں_وتھ = format_control_ناں_وتھ(format);
    لكل (بنانشانی صحيح i = 0; نقاب; i++) {
        صحيح ret;

        إذا (!(نقاب & (1u << i)))
            استمر;

        نقاب &= ~(1u << i);

        ثابت حرف* ثابت نقاب_متن =
            Lookup_قدر(ctrl_نقاب_ناں + control_ناں_وتھ, 1u << i);
        أكد(نقاب_متن);
        ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", نقاب_متن);
        إذا (ret <= 0 || pos + (ناپ_t)ret >= حجم(buf))
            اكسر;
        وإلا
            pos += (ناپ_t)ret;
    }
    pos++;

    أرجع حافظہ_نقل(xkb_context_get_بفر(ctx, pos), buf, pos);
}
