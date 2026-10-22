/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"

/** LUT entry */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਅਚਲ ਅੱਖਰ *name;
    uint32_t ਮੁੱਲ;
} Lookup_ਇੰਦਰਾਜ;

bool
LookupString(ਅਚਲ Lookup_ਇੰਦਰਾਜ tab[], ਅਚਲ ਅੱਖਰ *string,
              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਮੁੱਲ_rtrn);

ਅਚਲ ਅੱਖਰ *
Lookup_ਮੁੱਲ(ਅਚਲ Lookup_ਇੰਦਰਾਜ tab[], ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਮੂਹ_ਆਖਰੀ_ਸੂਚਕਾਂਕ_NAME "last"
ਗਿਣਤੀ {
    CONTROL_ਨਾਂ_MIN_V1_ਸੂਚਕਾਂਕ = 7,
    CONTROL_ਨਾਂ_MIN_V2_ਸੂਚਕਾਂਕ = 0,
};

ਸਥਿਰ ਸਤਰਅੰਦਰ uint8_t
format_control_ਨਾਂ_ਵਿੱਥ(ਗਿਣਤੀ xkb_keymap_format format)
{
    ਵਾਪਸ (format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1)
        ? CONTROL_ਨਾਂ_MIN_V1_ਸੂਚਕਾਂਕ
        : CONTROL_ਨਾਂ_MIN_V2_ਸੂਚਕਾਂਕ;
}

ਬਾਹਰੀ ਅਚਲ Lookup_ਇੰਦਰਾਜ ctrl_ਨਕਾਬ_ਨਾਂ[];
ਬਾਹਰੀ ਅਚਲ Lookup_ਇੰਦਰਾਜ mod_Component_ਨਕਾਬ_ਨਾਂ[];
ਬਾਹਰੀ ਅਚਲ Lookup_ਇੰਦਰਾਜ ਸਮੂਹ_Component_ਨਕਾਬ_ਨਾਂ[];
ਬਾਹਰੀ ਅਚਲ Lookup_ਇੰਦਰਾਜ ਸਮੂਹ_ਨਕਾਬ_ਨਾਂ[];
ਬਾਹਰੀ ਅਚਲ Lookup_ਇੰਦਰਾਜ button_ਨਾਂ[];
ਬਾਹਰੀ ਅਚਲ Lookup_ਇੰਦਰਾਜ use_Mod_Map_ਮੁੱਲ_ਨਾਂ[];
ਬਾਹਰੀ ਅਚਲ Lookup_ਇੰਦਰਾਜ action_Type_ਨਾਂ[];
ਬਾਹਰੀ ਅਚਲ Lookup_ਇੰਦਰਾਜ sym_Interpret_ਮੇਲ_ਨਕਾਬ_ਨਾਂ[];

ਅਚਲ ਅੱਖਰ *
Mod_ਨਕਾਬ_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ mod_type type,
            ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, xkb_mod_ਨਕਾਬ_t ਨਕਾਬ);

ਅਚਲ ਅੱਖਰ *
Mod_ਸੂਚਕਾਂਕ_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods,
             xkb_mod_ਸੂਚਕਾਂਕ_t ndx);

ਅਚਲ ਅੱਖਰ *
Action_Type_ਪਾਠ(ਗਿਣਤੀ xkb_action_type type);

ਅਚਲ ਅੱਖਰ *
Keysym_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, xkb_keysym_t sym);

ਅਚਲ ਅੱਖਰ *
ਕੁੰਜੀ_Name_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, xkb_atom_t name);

ਅਚਲ ਅੱਖਰ *
SIMatch_ਪਾਠ(ਗਿਣਤੀ xkb_ਮੇਲ_operation type);

ਅਚਲ ਅੱਖਰ *
Led_State_ਨਕਾਬ_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, ਅਚਲ Lookup_ਇੰਦਰਾਜ *lookup,
                 ਗਿਣਤੀ xkb_state_component ਨਕਾਬ);

ਅਚਲ ਅੱਖਰ *
Control_ਨਕਾਬ_ਪਾਠ(ਬਣਤਰ xkb_context *ctx, ਗਿਣਤੀ xkb_keymap_format format,
                ਗਿਣਤੀ xkb_action_controls ਨਕਾਬ);
