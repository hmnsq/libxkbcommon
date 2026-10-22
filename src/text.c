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
LookupString(ثابت Lookup_مدخل tab[], ثابت حرف *string,
              بیعلامت صحيح *مقدار_rtrn)
{
    إذا (!string)
        أرجع false;

    لكل (ثابت Lookup_مدخل *مدخل = tab; مدخل->name; مدخل++) {
        إذا (istreq(مدخل->name, string)) {
            *مقدار_rtrn = مدخل->مقدار;
            أرجع true;
        }
    }

    أرجع false;
}

ثابت حرف *
Lookup_مقدار(ثابت Lookup_مدخل tab[], بیعلامت صحيح مقدار)
{
    لكل (ثابت Lookup_مدخل *مدخل = tab; مدخل->name; مدخل++)
        إذا (مدخل->مقدار == مقدار)
            أرجع مدخل->name;

    أرجع عدم;
}

ثابت Lookup_مدخل ctrl_ماسک_نامها[25] = {
    /* v2 keymap format only */
    { "Overlay3", CONTROL_OVERLAY3 },
    { "Overlay4", CONTROL_OVERLAY4 },
    { "Overlay5", CONTROL_OVERLAY5 },
    { "Overlay6", CONTROL_OVERLAY6 },
    { "Overlay7", CONTROL_OVERLAY7 },
    { "Overlay8", CONTROL_OVERLAY8 },
    { "all", CONTROL_ALL_BOOLEAN },
    /* v1 + v2 keymap formats */
    [CONTROL_نامها_MIN_V1_شاخص] = { "RepeatKeys", CONTROL_REPEAT },
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
    { "IgnoreGroupLock", CONTROL_IGNORE_گروه_قفل },
    { "Overlay1", CONTROL_OVERLAY1 },
    { "Overlay2", CONTROL_OVERLAY2 },
    { "all", CONTROL_ALL_BOOLEAN_V1 }, /* duplicate, matches only v1 */
    { "none", 0 },
    { عدم, 0 }
};

ثابت Lookup_مدخل mod_Component_ماسک_نامها[] = {
    { "base", XKB_STATE_MODS_DEPRESSED },
    { "latched", XKB_STATE_MODS_LATCHED },
    { "locked", XKB_STATE_MODS_LOCKED },
    { "effective", XKB_STATE_MODS_EFFECTIVE },
    { "compat", XKB_STATE_MODS_EFFECTIVE },
    { "any", XKB_STATE_MODS_EFFECTIVE },
    { "none", 0 },
    { عدم, 0 }
};

ثابت Lookup_مدخل گروه_Component_ماسک_نامها[] = {
    { "base", XKB_STATE_LAYOUT_DEPRESSED },
    { "latched", XKB_STATE_LAYOUT_LATCHED },
    { "locked", XKB_STATE_LAYOUT_LOCKED },
    { "effective", XKB_STATE_LAYOUT_EFFECTIVE },
    { "any", XKB_STATE_LAYOUT_EFFECTIVE },
    { "none", 0 },
    { عدم, 0 }
};

ثابت Lookup_مدخل گروه_ماسک_نامها[] = {
    { "none", 0x00 },
    /* "first" is omitted for compatibility reasons */
    { "all", XKB_ALL_GROUPS },
    { عدم, 0 }
};

ثابت Lookup_مدخل button_نامها[] = {
    { "Button1", 1 },
    { "Button2", 2 },
    { "Button3", 3 },
    { "Button4", 4 },
    { "Button5", 5 },
    { "default", 0 },
    { عدم, 0 }
};

ثابت Lookup_مدخل use_Mod_Map_مقدار_نامها[] = {
    { "LevelOne", 1 },
    { "Level1", 1 },
    { "AnyLevel", 0 },
    { "any", 0 },
    { عدم, 0 }
};

ثابت Lookup_مدخل action_Type_نامها[] = {
    { "NoAction", ACTION_TYPE_NONE },
    { "VoidAction", ACTION_TYPE_VOID },
    { "SetMods", ACTION_TYPE_MOD_مجموعه },
    { "LatchMods", ACTION_TYPE_MOD_LATCH },
    { "LockMods", ACTION_TYPE_MOD_قفل },
    { "SetGroup", ACTION_TYPE_گروه_مجموعه },
    { "LatchGroup", ACTION_TYPE_گروه_LATCH },
    { "LockGroup", ACTION_TYPE_گروه_قفل },
    { "MovePtr", ACTION_TYPE_PTR_MOVE },
    { "MovePointer", ACTION_TYPE_PTR_MOVE },
    { "PtrBtn", ACTION_TYPE_PTR_BUTTON },
    { "PointerButton", ACTION_TYPE_PTR_BUTTON },
    { "LockPtrBtn", ACTION_TYPE_PTR_قفل },
    { "LockPtrButton", ACTION_TYPE_PTR_قفل },
    { "LockPointerButton", ACTION_TYPE_PTR_قفل },
    { "LockPointerBtn", ACTION_TYPE_PTR_قفل },
    { "SetPtrDflt", ACTION_TYPE_PTR_DEFAULT },
    { "SetPointerDefault", ACTION_TYPE_PTR_DEFAULT },
    { "Terminate", ACTION_TYPE_TERMINATE },
    { "TerminateServer", ACTION_TYPE_TERMINATE },
    { "SwitchScreen", ACTION_TYPE_SWITCH_VT },
    { "SetControls", ACTION_TYPE_CTRL_مجموعه },
    { "LockControls", ACTION_TYPE_CTRL_قفل },
    { "RedirectKey", ACTION_TYPE_REDIRECT_کلید },
    { "Redirect", ACTION_TYPE_REDIRECT_کلید },
    { "Private", ACTION_TYPE_خصوصی },
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

ثابت Lookup_مدخل sym_Interpret_تطابق_ماسک_نامها[] = {
    { "NoneOf", تطابق_NONE },
    { "AnyOfOrNone", تطابق_ANY_OR_NONE },
    { "AnyOf", تطابق_ANY },
    { "AllOf", تطابق_ALL },
    { "Exactly", تطابق_EXACTLY },
    { عدم, 0 },
};

ثابت حرف *
Mod_شاخص_متن(بنية xkb_context *ctx, ثابت بنية xkb_mod_مجموعه *mods,
             xkb_mod_شاخص_t ndx)
{
    إذا (ndx == XKB_MOD_INVALID)
        أرجع "none";

    إذا (ndx == XKB_MOD_NONE)
        أرجع "None";

    إذا (ndx >= mods->رقم_mods)
        أرجع عدم;

    أرجع xkb_atom_متن(ctx, mods->mods[ndx].name);
}

ثابت حرف *
Action_Type_متن(تعداد xkb_action_type type)
{
    ثابت حرف *name = Lookup_مقدار(action_Type_نامها, type);
    أرجع name ? name : "Private";
}

ثابت حرف *
Keysym_متن(بنية xkb_context *ctx, xkb_keysym_t sym)
{
    حرف *میانگیر = xkb_context_get_میانگیر(ctx, XKB_KEYSYM_NAME_MAX_اندازه);
    xkb_keysym_get_name(sym, میانگیر, XKB_KEYSYM_NAME_MAX_اندازه);
    أرجع میانگیر;
}

ثابت حرف *
کلید_Name_متن(بنية xkb_context *ctx, xkb_atom_t name)
{
    ثابت حرف *sname = xkb_atom_متن(ctx, name);
    اندازه_t len = strlen_امن(sname) + 3;
    حرف *buf = xkb_context_get_میانگیر(ctx, len);
    snprintf(buf, len, "<%s>", strempty(sname));
    أرجع buf;
}

ثابت حرف *
SIMatch_متن(تعداد xkb_تطابق_operation type)
{
    أرجع Lookup_مقدار(sym_Interpret_تطابق_ماسک_نامها, type);
}

ثابت حرف *
Mod_ماسک_متن(بنية xkb_context *ctx, تعداد mod_type type,
            ثابت بنية xkb_mod_مجموعه *mods, xkb_mod_ماسک_t ماسک)
{
    حرف buf[1024] = {0};
    اندازه_t pos = 0;
    ثابت بنية xkb_mod *mod;

    /* We want to avoid boolean blindness, but we expected only 2 values */
    أكد(type == MOD_REAL || type == MOD_BOTH);

    إذا (ماسک == 0)
        أرجع "none";

    إذا (ماسک == MOD_REAL_ماسک_ALL)
        أرجع "all";

    إذا ((type == MOD_REAL && (ماسک & ~MOD_REAL_ماسک_ALL)) ||
        unlikely(ماسک & ~((UINT64_C(1) << mods->رقم_mods) - 1))) {
        /* If we get a mask that cannot be expressed with the known modifiers
         * of the given type, print it as hexadecimal */
        ثابت صحيح ret = snprintf(buf, حجم(buf), "0x%"PRIx32, ماسک);
        static_assert(حجم(ماسک) == 4 && حجم(buf) >= حجم("0xffffffff"),
                      "Buffer too small");
        أكد(ret >= 0 && (اندازه_t) ret < حجم(buf));
        pos = (اندازه_t) ret;
    } وإلا {
        /* Print known mods */
        xkb_mods_ماسک_foreach(ماسک, mod, mods) {
            صحيح ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                               pos == 0 ? "" : "+",
                               xkb_atom_متن(ctx, mod->name));
            إذا (ret <= 0 || pos + (اندازه_t)ret >= حجم(buf))
                اكسر;
            وإلا
                pos += (اندازه_t)ret;
        }
    }
    pos++;

    أرجع کپی_حافظه(xkb_context_get_میانگیر(ctx, pos), buf, pos);
}

ثابت حرف *
Led_State_ماسک_متن(بنية xkb_context *ctx, ثابت Lookup_مدخل *lookup,
                 تعداد xkb_state_component ماسک)
{
    حرف buf[1024];
    اندازه_t pos = 0;

    إذا (ماسک == 0)
        أرجع "0";

    لكل (بیعلامت صحيح i = 0; ماسک; i++) {
        صحيح ret;

        إذا (!(ماسک & (1u << i)))
            استمر;

        ماسک &= ~(1u << i);

        ثابت حرف* ثابت ماسک_متن = Lookup_مقدار(lookup, 1u << i);
        أكد(ماسک_متن != عدم);
        ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", ماسک_متن);
        إذا (ret <= 0 || pos + (اندازه_t)ret >= حجم(buf))
            اكسر;
        وإلا
            pos += (اندازه_t)ret;
    }
    pos++;

    أرجع کپی_حافظه(xkb_context_get_میانگیر(ctx, pos), buf, pos);
}

ثابت حرف *
Control_ماسک_متن(بنية xkb_context *ctx, تعداد xkb_keymap_format format,
                تعداد xkb_action_controls ماسک)
{
    حرف buf[1024];
    اندازه_t pos = 0;

    ثابت تعداد xkb_action_controls all_ctrls = format_boolean_controls(format);
    ماسک &= all_ctrls;

    إذا (ماسک == 0)
        أرجع "none";

    إذا (ماسک == all_ctrls)
        أرجع "all";

    ثابت uint8_t control_نامها_جابه_جایی = format_control_نامها_جابه_جایی(format);
    لكل (بیعلامت صحيح i = 0; ماسک; i++) {
        صحيح ret;

        إذا (!(ماسک & (1u << i)))
            استمر;

        ماسک &= ~(1u << i);

        ثابت حرف* ثابت ماسک_متن =
            Lookup_مقدار(ctrl_ماسک_نامها + control_نامها_جابه_جایی, 1u << i);
        أكد(ماسک_متن);
        ret = snprintf(buf + pos, حجم(buf) - pos, "%s%s",
                       pos == 0 ? "" : "+", ماسک_متن);
        إذا (ret <= 0 || pos + (اندازه_t)ret >= حجم(buf))
            اكسر;
        وإلا
            pos += (اندازه_t)ret;
    }
    pos++;

    أرجع کپی_حافظه(xkb_context_get_میانگیر(ctx, pos), buf, pos);
}
