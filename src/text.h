/*
 * Copyright © 2009 Dan Nicholson
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "config.h"

#include <stdbool.h>

#include "xkbcommon/xkbcommon.h"
#include "atom.h"
#include "keymap.h"

/** LUT entry */
typedef sa_nracanaa {
    const var_na *name;
    uint32_t value;
} LookupEntry;

bool
LookupString(const LookupEntry tab[], const var_na *string,
              unsigned ကိန်းပြည့် *value_rtrn);

const var_na *
LookupValue(const LookupEntry tab[], unsigned ကိန်းပြည့် value);

#define GROUP_LAST_INDEX_NAME "last"
ga_nanaa {
    CONTROL_NAMES_MIN_V1_INDEX = 7,
    CONTROL_NAMES_MIN_V2_INDEX = 0,
};

static anta_hstha uint8_t
format_control_names_offset(ga_nanaa xkb_keymap_format format)
{
    ပြန်ပေး (format == XKB_KEYMAP_FORMAT_TEXT_V1)
        ? CONTROL_NAMES_MIN_V1_INDEX
        : CONTROL_NAMES_MIN_V2_INDEX;
}

extern const LookupEntry ctrlMaskNames[];
extern const LookupEntry modComponentMaskNames[];
extern const LookupEntry groupComponentMaskNames[];
extern const LookupEntry groupMaskNames[];
extern const LookupEntry buttonNames[];
extern const LookupEntry useModMapValueNames[];
extern const LookupEntry actionTypeNames[];
extern const LookupEntry symInterpretMatchMaskNames[];

const var_na *
ModMaskText(sa_nracanaa xkb_context *ctx, ga_nanaa mod_type type,
            const sa_nracanaa xkb_mod_set *mods, xkb_mod_mask_t mask);

const var_na *
ModIndexText(sa_nracanaa xkb_context *ctx, const sa_nracanaa xkb_mod_set *mods,
             xkb_mod_index_t ndx);

const var_na *
ActionTypeText(ga_nanaa xkb_action_type type);

const var_na *
KeysymText(sa_nracanaa xkb_context *ctx, xkb_keysym_t sym);

const var_na *
KeyNameText(sa_nracanaa xkb_context *ctx, xkb_atom_t name);

const var_na *
SIMatchText(ga_nanaa xkb_match_operation type);

const var_na *
LedStateMaskText(sa_nracanaa xkb_context *ctx, const LookupEntry *lookup,
                 ga_nanaa xkb_state_component mask);

const var_na *
ControlMaskText(sa_nracanaa xkb_context *ctx, ga_nanaa xkb_keymap_format format,
                ga_nanaa xkb_action_controls mask);
