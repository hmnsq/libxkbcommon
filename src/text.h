/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ہدایت once

#ضمن "config.h"

#ضمن <stdbool.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "atom.h"
#ضمن "keymap.h"

/** LUT entry */
تعريفنوع بنية {
    ثابت حرف *name;
    uint32_t قدر;
} Lookup_اندراج;

bool
LookupString(ثابت Lookup_اندراج tab[], ثابت حرف *string,
              بنانشانی صحيح *قدر_rtrn);

ثابت حرف *
Lookup_قدر(ثابت Lookup_اندراج tab[], بنانشانی صحيح قدر);

#تعریف_کرو گروہ_آخری_اشاریہ_NAME "last"
تعداد {
    CONTROL_ناں_MIN_V1_اشاریہ = 7,
    CONTROL_ناں_MIN_V2_اشاریہ = 0,
};

ساكن مضمن uint8_t
format_control_ناں_وتھ(تعداد xkb_keymap_format format)
{
    أرجع (format == XKB_KEYMAP_FORMAT_متن_V1)
        ? CONTROL_ناں_MIN_V1_اشاریہ
        : CONTROL_ناں_MIN_V2_اشاریہ;
}

خارجي ثابت Lookup_اندراج ctrl_نقاب_ناں[];
خارجي ثابت Lookup_اندراج mod_Component_نقاب_ناں[];
خارجي ثابت Lookup_اندراج گروہ_Component_نقاب_ناں[];
خارجي ثابت Lookup_اندراج گروہ_نقاب_ناں[];
خارجي ثابت Lookup_اندراج button_ناں[];
خارجي ثابت Lookup_اندراج use_Mod_Map_قدر_ناں[];
خارجي ثابت Lookup_اندراج action_Type_ناں[];
خارجي ثابت Lookup_اندراج sym_Interpret_میل_نقاب_ناں[];

ثابت حرف *
Mod_نقاب_متن(بنية xkb_context *ctx, تعداد mod_type type,
            ثابت بنية xkb_mod_سیٹ *mods, xkb_mod_نقاب_t نقاب);

ثابت حرف *
Mod_اشاریہ_متن(بنية xkb_context *ctx, ثابت بنية xkb_mod_سیٹ *mods,
             xkb_mod_اشاریہ_t ndx);

ثابت حرف *
Action_Type_متن(تعداد xkb_action_type type);

ثابت حرف *
Keysym_متن(بنية xkb_context *ctx, xkb_keysym_t sym);

ثابت حرف *
کلید_Name_متن(بنية xkb_context *ctx, xkb_atom_t name);

ثابت حرف *
SIMatch_متن(تعداد xkb_میل_operation type);

ثابت حرف *
Led_State_نقاب_متن(بنية xkb_context *ctx, ثابت Lookup_اندراج *lookup,
                 تعداد xkb_state_component نقاب);

ثابت حرف *
Control_نقاب_متن(بنية xkb_context *ctx, تعداد xkb_keymap_format format,
                تعداد xkb_action_controls نقاب);
