/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#رهنمود once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "atom.h"
#ضمن "keymap.h"

/** LUT entry */
تعريفنوع بنية {
    ثابت حرف *name;
    uint32_t مقدار;
} Lookup_مدخل;

bool
LookupString(ثابت Lookup_مدخل tab[], ثابت حرف *string,
              بیعلامت صحيح *مقدار_rtrn);

ثابت حرف *
Lookup_مقدار(ثابت Lookup_مدخل tab[], بیعلامت صحيح مقدار);

#تعریف_کن گروه_آخرین_شاخص_NAME "last"
تعداد {
    CONTROL_نامها_MIN_V1_شاخص = 7,
    CONTROL_نامها_MIN_V2_شاخص = 0,
};

ساكن مضمن uint8_t
format_control_نامها_جابه_جایی(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? CONTROL_نامها_MIN_V1_شاخص
        : CONTROL_نامها_MIN_V2_شاخص;
}

خارجي ثابت Lookup_مدخل ctrl_ماسک_نامها[];
خارجي ثابت Lookup_مدخل mod_Component_ماسک_نامها[];
خارجي ثابت Lookup_مدخل گروه_Component_ماسک_نامها[];
خارجي ثابت Lookup_مدخل گروه_ماسک_نامها[];
خارجي ثابت Lookup_مدخل button_نامها[];
خارجي ثابت Lookup_مدخل use_Mod_Map_مقدار_نامها[];
خارجي ثابت Lookup_مدخل action_Type_نامها[];
خارجي ثابت Lookup_مدخل sym_Interpret_تطابق_ماسک_نامها[];

ثابت حرف *
Mod_ماسک_متن(بنية xkb_context *ctx, تعداد mod_type type,
            ثابت بنية xkb_mod_مجموعه *mods, xkb_mod_ماسک_t ماسک);

ثابت حرف *
Mod_شاخص_متن(بنية xkb_context *ctx, ثابت بنية xkb_mod_مجموعه *mods,
             xkb_mod_شاخص_t ndx);

ثابت حرف *
Action_Type_متن(تعداد xkb_action_type type);

ثابت حرف *
Keysym_متن(بنية xkb_context *ctx, xkb_keysym_t sym);

ثابت حرف *
کلید_Name_متن(بنية xkb_context *ctx, xkb_atom_t name);

ثابت حرف *
SIMatch_متن(تعداد xkb_تطابق_operation type);

ثابت حرف *
Led_State_ماسک_متن(بنية xkb_context *ctx, ثابت Lookup_مدخل *lookup,
                 تعداد xkb_state_component ماسک);

ثابت حرف *
Control_ماسک_متن(بنية xkb_context *ctx, تعداد xkb_keymap_format format,
                تعداد xkb_action_controls ماسک);
