/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#لارښوونه once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "atom.h"
#ضمن "keymap.h"

/** LUT entry */
تعريفنوع بنية {
    ثابت حرف *name;
    uint32_t ارزښت;
} Lookup_ثبت;

bool
LookupString(ثابت Lookup_ثبت tab[], ثابت حرف *string,
              بینښه صحيح *ارزښت_rtrn);

ثابت حرف *
Lookup_ارزښت(ثابت Lookup_ثبت tab[], بینښه صحيح ارزښت);

#تعریف_کړه ډله_وروستی_شاخص_NAME "last"
تعداد {
    CONTROL_نومونه_MIN_V1_شاخص = 7,
    CONTROL_نومونه_MIN_V2_شاخص = 0,
};

ساكن مضمن uint8_t
format_control_نومونه_دننه_واټن(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? CONTROL_نومونه_MIN_V1_شاخص
        : CONTROL_نومونه_MIN_V2_شاخص;
}

خارجي ثابت Lookup_ثبت ctrl_ماسک_نومونه[];
خارجي ثابت Lookup_ثبت mod_Component_ماسک_نومونه[];
خارجي ثابت Lookup_ثبت ډله_Component_ماسک_نومونه[];
خارجي ثابت Lookup_ثبت ډله_ماسک_نومونه[];
خارجي ثابت Lookup_ثبت button_نومونه[];
خارجي ثابت Lookup_ثبت use_Mod_Map_ارزښت_نومونه[];
خارجي ثابت Lookup_ثبت action_Type_نومونه[];
خارجي ثابت Lookup_ثبت sym_Interpret_سمون_ماسک_نومونه[];

ثابت حرف *
Mod_ماسک_متن(بنية xkb_context *ctx, تعداد mod_type type,
            ثابت بنية xkb_mod_سېټ *mods, xkb_mod_ماسک_t ماسک);

ثابت حرف *
Mod_شاخص_متن(بنية xkb_context *ctx, ثابت بنية xkb_mod_سېټ *mods,
             xkb_mod_شاخص_t ndx);

ثابت حرف *
Action_Type_متن(تعداد xkb_action_type type);

ثابت حرف *
Keysym_متن(بنية xkb_context *ctx, xkb_keysym_t sym);

ثابت حرف *
کلی_Name_متن(بنية xkb_context *ctx, xkb_atom_t name);

ثابت حرف *
SIMatch_متن(تعداد xkb_سمون_operation type);

ثابت حرف *
Led_State_ماسک_متن(بنية xkb_context *ctx, ثابت Lookup_ثبت *lookup,
                 تعداد xkb_state_component ماسک);

ثابت حرف *
Control_ماسک_متن(بنية xkb_context *ctx, تعداد xkb_keymap_format format,
                تعداد xkb_action_controls ماسک);
