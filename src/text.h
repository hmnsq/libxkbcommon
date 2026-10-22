/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdbool.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "keymap.h"

/** LUT entry */
തരനിർവചനം ഘടന {
    സ്ഥിരം അക്ഷരം *name;
    uint32_t മൂല്യം;
} LookupEntry;

bool
LookupString(സ്ഥിരം LookupEntry tab[], സ്ഥിരം അക്ഷരം *string,
              unsigned പൂർണ്ണസംഖ്യ *മൂല്യം_rtrn);

സ്ഥിരം അക്ഷരം *
Lookup_മൂല്യം(സ്ഥിരം LookupEntry tab[], unsigned പൂർണ്ണസംഖ്യ മൂല്യം);

#define GROUP_LAST_INDEX_NAME "last"
എണ്ണൽ {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

സ്ഥിതം വരിയിൽ uint8_t
format_control_names_offset(എണ്ണൽ xkb_keymap_format format)
{
    തിരികെ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

ബാഹ്യ സ്ഥിരം LookupEntry ctrlMaskNames[];
ബാഹ്യ സ്ഥിരം LookupEntry modComponentMaskNames[];
ബാഹ്യ സ്ഥിരം LookupEntry groupComponentMaskNames[];
ബാഹ്യ സ്ഥിരം LookupEntry groupMaskNames[];
ബാഹ്യ സ്ഥിരം LookupEntry buttonNames[];
ബാഹ്യ സ്ഥിരം LookupEntry use_Mod_Map_മൂല്യം_Names[];
ബാഹ്യ സ്ഥിരം LookupEntry actionTypeNames[];
ബാഹ്യ സ്ഥിരം LookupEntry symInterpretMatchMaskNames[];

സ്ഥിരം അക്ഷരം *
ModMaskText(ഘടന xkb_context *ctx, എണ്ണൽ mod_type type,
            സ്ഥിരം ഘടന xkb_mod_set *mods, xkb_mod_mask_t mask);

സ്ഥിരം അക്ഷരം *
ModIndexText(ഘടന xkb_context *ctx, സ്ഥിരം ഘടന xkb_mod_set *mods,
             xkb_mod_index_t ndx);

സ്ഥിരം അക്ഷരം *
ActionTypeText(എണ്ണൽ xkb_action_type type);

സ്ഥിരം അക്ഷരം *
KeysymText(ഘടന xkb_context *ctx, xkb_keysym_t sym);

സ്ഥിരം അക്ഷരം *
താക്കോൽ_Name_Text(ഘടന xkb_context *ctx, xkb_atom_t name);

സ്ഥിരം അക്ഷരം *
SIMatchText(എണ്ണൽ xkb_match_operation type);

സ്ഥിരം അക്ഷരം *
LedStateMaskText(ഘടന xkb_context *ctx, സ്ഥിരം LookupEntry *lookup,
                 എണ്ണൽ xkb_state_component mask);

സ്ഥിരം അക്ഷരം *
ControlMaskText(ഘടന xkb_context *ctx, എണ്ണൽ xkb_keymap_format format,
                എണ്ണൽ xkb_action_controls mask);
