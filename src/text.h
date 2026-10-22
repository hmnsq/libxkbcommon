/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#شامل "config.h"

#شامل <stdbool.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "atom.h"
#شامل "keymap.h"

/** LUT entry */
قسمتعریف ساخت {
    ثابت حرف *name;
    uint32_t قدر;
} Lookup_اندراج;

bool
LookupString(ثابت Lookup_اندراج tab[], ثابت حرف *string,
              غیرعلامتی صحیح *قدر_rtrn);

ثابت حرف *
Lookup_قدر(ثابت Lookup_اندراج tab[], غیرعلامتی صحیح قدر);

#تعریف_کرو گروہ_آخری_اشاریہ_NAME "last"
شمار {
    CONTROL_نام_MIN_V1_اشاریہ = 7,
    CONTROL_نام_MIN_V2_اشاریہ = 0,
};

ساکن اندرونی uint8_t
format_control_نام_انحراف(شمار xkb_keymap_format format)
{
    واپس (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? CONTROL_نام_MIN_V1_اشاریہ
        : CONTROL_نام_MIN_V2_اشاریہ;
}

خارجی ثابت Lookup_اندراج ctrl_نقاب_نام[];
خارجی ثابت Lookup_اندراج mod_Component_نقاب_نام[];
خارجی ثابت Lookup_اندراج گروہ_Component_نقاب_نام[];
خارجی ثابت Lookup_اندراج گروہ_نقاب_نام[];
خارجی ثابت Lookup_اندراج button_نام[];
خارجی ثابت Lookup_اندراج use_Mod_Map_قدر_نام[];
خارجی ثابت Lookup_اندراج action_Type_نام[];
خارجی ثابت Lookup_اندراج sym_Interpret_مطابقت_نقاب_نام[];

ثابت حرف *
Mod_نقاب_متن(ساخت xkb_context *ctx, شمار mod_type type,
            ثابت ساخت xkb_mod_سیٹ *mods, xkb_mod_نقاب_t نقاب);

ثابت حرف *
Mod_اشاریہ_متن(ساخت xkb_context *ctx, ثابت ساخت xkb_mod_سیٹ *mods,
             xkb_mod_اشاریہ_t ndx);

ثابت حرف *
Action_Type_متن(شمار xkb_action_type type);

ثابت حرف *
Keysym_متن(ساخت xkb_context *ctx, xkb_keysym_t sym);

ثابت حرف *
کلید_Name_متن(ساخت xkb_context *ctx, xkb_atom_t name);

ثابت حرف *
SIMatch_متن(شمار xkb_مطابقت_operation type);

ثابت حرف *
Led_State_نقاب_متن(ساخت xkb_context *ctx, ثابت Lookup_اندراج *lookup,
                 شمار xkb_state_component نقاب);

ثابت حرف *
Control_نقاب_متن(ساخت xkb_context *ctx, شمار xkb_keymap_format format,
                شمار xkb_action_controls نقاب);
