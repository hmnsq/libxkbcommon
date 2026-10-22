/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#هدايت once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "atom.h"
#ضمن "keymap.h"

/** LUT entry */
تعريفنوع بنية {
    ثابت حرف *name;
    uint32_t قدر;
} Lookup_داخلا;

bool
LookupString(ثابت Lookup_داخلا tab[], ثابت حرف *string,
              بنانشان صحيح *قدر_rtrn);

ثابت حرف *
Lookup_قدر(ثابت Lookup_داخلا tab[], بنانشان صحيح قدر);

#وصف_ڪريو گروهه_آخري_اشاريو_NAME "last"
تعداد {
    CONTROL_نالا_MIN_V1_اشاريو = 7,
    CONTROL_نالا_MIN_V2_اشاريو = 0,
};

ساكن مضمن uint8_t
format_control_نالا_وٿي(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? CONTROL_نالا_MIN_V1_اشاريو
        : CONTROL_نالا_MIN_V2_اشاريو;
}

خارجي ثابت Lookup_داخلا ctrl_نقاب_نالا[];
خارجي ثابت Lookup_داخلا mod_Component_نقاب_نالا[];
خارجي ثابت Lookup_داخلا گروهه_Component_نقاب_نالا[];
خارجي ثابت Lookup_داخلا گروهه_نقاب_نالا[];
خارجي ثابت Lookup_داخلا button_نالا[];
خارجي ثابت Lookup_داخلا use_Mod_Map_قدر_نالا[];
خارجي ثابت Lookup_داخلا action_Type_نالا[];
خارجي ثابت Lookup_داخلا sym_Interpret_ميلاپ_نقاب_نالا[];

ثابت حرف *
Mod_نقاب_متن(بنية xkb_context *ctx, تعداد mod_type type,
            ثابت بنية xkb_mod_سيٽ *mods, xkb_mod_نقاب_t نقاب);

ثابت حرف *
Mod_اشاريو_متن(بنية xkb_context *ctx, ثابت بنية xkb_mod_سيٽ *mods,
             xkb_mod_اشاريو_t ndx);

ثابت حرف *
Action_Type_متن(تعداد xkb_action_type type);

ثابت حرف *
Keysym_متن(بنية xkb_context *ctx, xkb_keysym_t sym);

ثابت حرف *
ڪنجي_Name_متن(بنية xkb_context *ctx, xkb_atom_t name);

ثابت حرف *
SIMatch_متن(تعداد xkb_ميلاپ_operation type);

ثابت حرف *
Led_State_نقاب_متن(بنية xkb_context *ctx, ثابت Lookup_داخلا *lookup,
                 تعداد xkb_state_component نقاب);

ثابت حرف *
Control_نقاب_متن(بنية xkb_context *ctx, تعداد xkb_keymap_format format,
                تعداد xkb_action_controls نقاب);
