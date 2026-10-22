/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 *
 * SPDX-License-Identifier: HPND AND MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 * Author: Ran Benita <ran234@gmail.com>
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-status.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-keysyms.h"

#ಅನ್ತರ್ಭಾವ "action.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "include.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "keysym.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "util-numbers.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"
#ಅನ್ತರ್ಭಾವ "vmod.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast.h"

ಎಣಿಕೆ ಕೀಲಿ_repeat {
    ಕೀಲಿ_REPEAT_UNDEFINED = 0,
    ಕೀಲಿ_REPEAT_YES,
    ಕೀಲಿ_REPEAT_NO,
    ಕೀಲಿ_REPEAT_ಅಂಕೆ_ENTRIES,
};
ಎಣಿಕೆ { ಕೀಲಿ_REPEAT_MIN_ಅಗಲ = 3 /* 2 bits + sign */ };
static_assert(ಕೀಲಿ_REPEAT_ಅಂಕೆ_ENTRIES - 1 <=
              (1 << (ಕೀಲಿ_REPEAT_MIN_ಅಗಲ - 1)) - 1,
              "Cannot encode key repeat");

ಎಣಿಕೆ ಗುಂಪು_ಕ್ಷೇತ್ರ {
    ಗುಂಪು_ಕ್ಷೇತ್ರ_SYMS = (1 << 0),
    ಗುಂಪು_ಕ್ಷೇತ್ರ_ACTS = (1 << 1),
    ಗುಂಪು_ಕ್ಷೇತ್ರ_TYPE = (1 << 2),
};

ಎಣಿಕೆ ಕೀಲಿ_ಕ್ಷೇತ್ರ {
    ಕೀಲಿ_ಕ್ಷೇತ್ರ_NONE = 0,
    ಕೀಲಿ_ಕ್ಷೇತ್ರ_REPEAT = (1 << 0),
    ಕೀಲಿ_ಕ್ಷೇತ್ರ_DEFAULT_TYPE = (1 << 1),
    ಕೀಲಿ_ಕ್ಷೇತ್ರ_GROUPINFO = (1 << 2),
    ಕೀಲಿ_ಕ್ಷೇತ್ರ_VMODMAP = (1 << 3),
    ಕೀಲಿ_ಕ್ಷೇತ್ರ_OVERLAY = (1 << 4),
    ಕೀಲಿ_ಕ್ಷೇತ್ರ_ALL = ಕೀಲಿ_ಕ್ಷೇತ್ರ_REPEAT | ಕೀಲಿ_ಕ್ಷೇತ್ರ_DEFAULT_TYPE
                  | ಕೀಲಿ_ಕ್ಷೇತ್ರ_GROUPINFO | ಕೀಲಿ_ಕ್ಷೇತ್ರ_VMODMAP | ಕೀಲಿ_ಕ್ಷೇತ್ರ_OVERLAY,
};
ಎಣಿಕೆ { ಕೀಲಿ_ಕ್ಷೇತ್ರ_MIN_ಅಗಲ = 6 /* 5 bits + sign */ };
static_assert(ಕೀಲಿ_ಕ್ಷೇತ್ರ_ALL <= (1 << (ಕೀಲಿ_ಕ್ಷೇತ್ರ_MIN_ಅಗಲ - 1)) - 1,
              "Cannot encode key field");

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    darray(ರಚನೆ xkb_level) ಮಟ್ಟಗಳು;
    ಎಣಿಕೆ ಗುಂಪು_ಕ್ಷೇತ್ರ defined;
    xkb_atom_t type;
} ಗುಂಪು_Info_2;

ಎಣಿಕೆ {
    XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY_MIN_ಅಗಲ = 4 /* 3 bits + sign */,
    KEYINFO_ಹೊರಹೋಗು_ದ_RANGE_ಗುಂಪು_POLICY_ಬಿಟ್_ಕ್ಷೇತ್ರ = 8,
    KEYINFO_DEFINED_ಬಿಟ್_ಕ್ಷೇತ್ರ = 16,
    KEYINFO_ವಿಲೀನ_ಬಿಟ್_ಕ್ಷೇತ್ರ = 8,
    KEYINFO_REPEAT_ಬಿಟ್_ಕ್ಷೇತ್ರ = 8,
};
static_assert(
    XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP < XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP &&
    XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP < XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY_MIN_ಅಗಲ) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_ಹೊರಹೋಗು_ದ_RANGE_ಗುಂಪು_POLICY_ಬಿಟ್_ಕ್ಷೇತ್ರ >=
              XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_POLICY_MIN_ಅಗಲ, "");
static_assert((ಚಿಹ್ನೆರಹಿತ)KEYINFO_DEFINED_ಬಿಟ್_ಕ್ಷೇತ್ರ >=
              (ಚಿಹ್ನೆರಹಿತ)ಕೀಲಿ_ಕ್ಷೇತ್ರ_MIN_ಅಗಲ, "Cannot encode KeyInfo field");
static_assert((ಚಿಹ್ನೆರಹಿತ)KEYINFO_ವಿಲೀನ_ಬಿಟ್_ಕ್ಷೇತ್ರ >=
              (ಚಿಹ್ನೆರಹಿತ)ವಿಲೀನ_ಬಹುಲಕ_MIN_ಅಗಲ, "Cannot encode KeyInfo field");
static_assert((ಚಿಹ್ನೆರಹಿತ)KEYINFO_REPEAT_ಬಿಟ್_ಕ್ಷೇತ್ರ >=
              (ಚಿಹ್ನೆರಹಿತ)ಕೀಲಿ_REPEAT_MIN_ಅಗಲ, "Cannot encode KeyInfo field");

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    xkb_atom_t name;

    xkb_mod_ಮುಖವಾಡ_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_ಸೂಚ್ಯಂಕ_t ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ;
    darray(ಗುಂಪು_Info_2) groups;
    ಎಣಿಕೆ xkb_layout_ಹೊರಹೋಗು_ದ_range_policy
        ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy:KEYINFO_ಹೊರಹೋಗು_ದ_RANGE_ಗುಂಪು_POLICY_ಬಿಟ್_ಕ್ಷೇತ್ರ;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    ಎಣಿಕೆ ಕೀಲಿ_ಕ್ಷೇತ್ರ defined:KEYINFO_DEFINED_ಬಿಟ್_ಕ್ಷೇತ್ರ;
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ:KEYINFO_ವಿಲೀನ_ಬಿಟ್_ಕ್ಷೇತ್ರ;
    ಎಣಿಕೆ ಕೀಲಿ_repeat repeat:KEYINFO_REPEAT_ಬಿಟ್_ಕ್ಷೇತ್ರ;

    bool ಹೊರಹೋಗು_ದ_range_pending_ಗುಂಪು:1;

    /** If true, overlays in the mask are explicitly cleared */
    bool overlays_clear:1;
    /** Track the count of allocated slots of overlays_keys */
    xkb_overlay_ಸೂಚ್ಯಂಕ_t overlays_alloc;
    /** Overlays indices */
    xkb_overlay_ಮುಖವಾಡ_t overlays;
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    ಒಕ್ಕೂಟ {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *overlay_ಕೀಲಿ;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ **overlays_ಕೀಲಿಗಳು;
    };
} ಕೀಲಿ_Info;

ಸ್ಥಿರ ಶೂನ್ಯ
StealLevelInfo(ರಚನೆ xkb_level *into, ರಚನೆ xkb_level *from)
{
    clear_level(into);
    ಆದರೆ (from->ಅಂಕೆ_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } ಇಲ್ಲದಿದ್ದರೆ {
        into->s.sym = from->s.sym;
    }
    into->ಅಂಕೆ_syms = from->ಅಂಕೆ_syms;
    from->ಅಂಕೆ_syms = 0;

    ಆದರೆ (from->ಅಂಕೆ_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } ಇಲ್ಲದಿದ್ದರೆ {
        into->a.action = from->a.action;
    }
    into->ಅಂಕೆ_actions = from->ಅಂಕೆ_actions;
    from->ಅಂಕೆ_actions = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ
Init_ಗುಂಪು_Info(ಗುಂಪು_Info_2 *groupi)
{
    memset(groupi, 0, ಗಾತ್ರ(*groupi));
}

ಸ್ಥಿರ ಶೂನ್ಯ
Clear_ಗುಂಪು_Info(ಗುಂಪು_Info_2 *groupi)
{
    ರಚನೆ xkb_level *leveli;
    darray_foreach(leveli, groupi->ಮಟ್ಟಗಳು)
        clear_level(leveli);
    darray_free(groupi->ಮಟ್ಟಗಳು);
}

ಸ್ಥಿರ ಶೂನ್ಯ
Copy_ಗುಂಪು_Info(ಗುಂಪು_Info_2 *ಗಮ್ಯ, ಸ್ಥಿರಾಂಕ ಗುಂಪು_Info_2 *from)
{
    ಗಮ್ಯ->defined = from->defined;
    ಗಮ್ಯ->type = from->type;
    darray_init(ಗಮ್ಯ->ಮಟ್ಟಗಳು);
    darray_copy(ಗಮ್ಯ->ಮಟ್ಟಗಳು, from->ಮಟ್ಟಗಳು);
    ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t j = 0; j < darray_ಗಾತ್ರ(ಗಮ್ಯ->ಮಟ್ಟಗಳು); j++) {
        ಆದರೆ (darray_ಅಂಶ(from->ಮಟ್ಟಗಳು, j).ಅಂಕೆ_syms > 1) {
            darray_ಅಂಶ(ಗಮ್ಯ->ಮಟ್ಟಗಳು, j).s.syms =
                memdup(darray_ಅಂಶ(from->ಮಟ್ಟಗಳು, j).s.syms,
                       darray_ಅಂಶ(from->ಮಟ್ಟಗಳು, j).ಅಂಕೆ_syms,
                       ಗಾತ್ರ(xkb_keysym_t));
        }
        ಆದರೆ (darray_ಅಂಶ(from->ಮಟ್ಟಗಳು, j).ಅಂಕೆ_actions > 1) {
            darray_ಅಂಶ(ಗಮ್ಯ->ಮಟ್ಟಗಳು, j).a.actions =
                memdup(darray_ಅಂಶ(from->ಮಟ್ಟಗಳು, j).a.actions,
                       darray_ಅಂಶ(from->ಮಟ್ಟಗಳು, j).ಅಂಕೆ_actions,
                       ಗಾತ್ರ(ಒಕ್ಕೂಟ xkb_action));
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
Init_ಕೀಲಿ_Info(ರಚನೆ xkb_context *ctx, ಕೀಲಿ_Info *keyi)
{
    memset(keyi, 0, ಗಾತ್ರ(*keyi));
    static_assert(!DEFAULT_ಕೀಲಿ_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_ಕೀಲಿ_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy = XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_ಕೀಲಿ = ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
Clear_ಕೀಲಿ_Info(ಕೀಲಿ_Info *keyi)
{
    ಗುಂಪು_Info_2 *groupi;
    darray_foreach(groupi, keyi->groups)
        Clear_ಗುಂಪು_Info(groupi);
    darray_free(keyi->groups);
    ಆದರೆ (keyi->overlays_alloc)
        ಬಿಡುಗಡೆ(keyi->overlays_ಕೀಲಿಗಳು);
}

/***====================================================================***/

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಅಕ್ಷರ *name;         /* e.g. pc+us+inet(evdev) */
    ಪೂರ್ಣಾಂಕ error_ಎಣಿಕೆ;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ;
    xkb_layout_ಸೂಚ್ಯಂಕ_t explicit_ಗುಂಪು;
    xkb_layout_ಸೂಚ್ಯಂಕ_t max_groups;
    darray(ಕೀಲಿ_Info) ಕೀಲಿಗಳು;
    ಕೀಲಿ_Info default_ಕೀಲಿ;
    ActionsInfo default_actions;
    darray(xkb_atom_t) ಗುಂಪು_ಹೆಸರುಗಳು;
    darray(Mod_Map_ನಮೂದು) modmaps;
    ರಚನೆ xkb_mod_ಗಣ mods;

    ರಚನೆ xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info;
} SymbolsInfo;

ಸ್ಥಿರ ಶೂನ್ಯ
InitSymbolsInfo(SymbolsInfo *info, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods)
{
    memset(info, 0, ಗಾತ್ರ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_ಆಳ = include_ಆಳ;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_ಕೀಲಿ_Info(keymap_info->keymap.ctx, &info->default_ಕೀಲಿ);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_ಆಳ > 0);
    info->explicit_ಗುಂಪು = XKB_LAYOUT_INVALID;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ClearSymbolsInfo(SymbolsInfo *info)
{
    ಕೀಲಿ_Info *keyi;
    ಬಿಡುಗಡೆ(info->name);
    darray_foreach(keyi, info->ಕೀಲಿಗಳು)
        Clear_ಕೀಲಿ_Info(keyi);
    darray_free(info->ಕೀಲಿಗಳು);
    darray_free(info->ಗುಂಪು_ಹೆಸರುಗಳು);
    darray_free(info->modmaps);
    Clear_ಕೀಲಿ_Info(&info->default_ಕೀಲಿ);
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
ಕೀಲಿ_Info_ಪಠ್ಯ(SymbolsInfo *info, ಕೀಲಿ_Info *keyi)
{
    ಹಿಂತಿರುಗಿ ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, keyi->name);
}

ಸ್ಥಿರ bool
ವಿಲೀನ_Groups(SymbolsInfo *info, ಗುಂಪು_Info_2 *into, ಗುಂಪು_Info_2 *from, bool clobber,
            bool report, xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು, xkb_atom_t ಕೀಲಿ_name)
{
    /* First find the type of the merged group. */
    ಆದರೆ (into->type != from->type) {
        ಆದರೆ (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            ಆದರೆ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         ಗುಂಪು + 1, ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, ಕೀಲಿ_name),
                         xkb_atom_ಪಠ್ಯ(info->ctx, use),
                         xkb_atom_ಪಠ್ಯ(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & ಗುಂಪು_ಕ್ಷೇತ್ರ_TYPE);

    /* Now look at the levels. */

    ಆದರೆ (darray_ಖಾಲಿಗಳು(from->ಮಟ್ಟಗಳು)) {
        Init_ಗುಂಪು_Info(from);
        ಹಿಂತಿರುಗಿ true;
    }

    ಆದರೆ (darray_ಖಾಲಿಗಳು(into->ಮಟ್ಟಗಳು)) {
        from->type = into->type;
        *into = *from;
        Init_ಗುಂಪು_Info(from);
        ಹಿಂತಿರುಗಿ true;
    }

    /* Merge the actions and syms. */
    ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t ಮಟ್ಟಗಳು_ಒಳ_both =
        MIN(darray_ಗಾತ್ರ(into->ಮಟ್ಟಗಳು), darray_ಗಾತ್ರ(from->ಮಟ್ಟಗಳು));
    darray_ಗಾತ್ರ_t from_Keysyms_ಎಣಿಕೆ = 0;
    darray_ಗಾತ್ರ_t from_Actions_ಎಣಿಕೆ = 0;
    ಗಾಗಿ (darray_ಗಾತ್ರ_t i = 0; i < ಮಟ್ಟಗಳು_ಒಳ_both; i++) {
        ರಚನೆ xkb_level* ಸ್ಥಿರಾಂಕ intoLevel = &darray_ಅಂಶ(into->ಮಟ್ಟಗಳು, i);
        ರಚನೆ xkb_level* ಸ್ಥಿರಾಂಕ fromLevel = &darray_ಅಂಶ(from->ಮಟ್ಟಗಳು, i);

        ಸ್ಥಿರಾಂಕ bool fromHasNoKeysym = fromLevel->ಅಂಕೆ_syms == 0;
        ಸ್ಥಿರಾಂಕ bool fromHasNoAction = fromLevel->ಅಂಕೆ_actions == 0;
        ಆದರೆ (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            ಮುಂದುವರಿ;
        }

        ಸ್ಥಿರಾಂಕ bool intoHasNoKeysym = intoLevel->ಅಂಕೆ_syms == 0;
        ಸ್ಥಿರಾಂಕ bool intoHasNoAction = intoLevel->ಅಂಕೆ_actions == 0;
        ಆದರೆ (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_ಎಣಿಕೆ++;
            from_Actions_ಎಣಿಕೆ++;
            ಮುಂದುವರಿ;
        }

        /* Possible level conflict */
        ಪ್ರತಿಜ್ಞಾ(intoLevel->ಅಂಕೆ_syms > 0 || intoLevel->ಅಂಕೆ_actions > 0);
        ಪ್ರತಿಜ್ಞಾ(fromLevel->ಅಂಕೆ_syms > 0 || fromLevel->ಅಂಕೆ_actions > 0);

        /* Handle keysyms */
        ಆದರೆ (!Xkb_ಮಟ್ಟಗಳು_Same_Syms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            ಆದರೆ (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_ಚಿಹ್ನೆ,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, ಗುಂಪು + 1, ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, ಕೀಲಿ_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            ಆದರೆ (fromHasNoKeysym) {
                /* No keysym to copy */
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (clobber) {
                /* Override: copy any defined keysym from `from` */
                ಆದರೆ (unlikely(fromLevel->ಅಂಕೆ_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    ಆದರೆ (unlikely(intoLevel->ಅಂಕೆ_syms > 1))
                        ಬಿಡುಗಡೆ(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->ಅಂಕೆ_syms = fromLevel->ಅಂಕೆ_syms;
                    fromLevel->ಅಂಕೆ_syms = 0;
                    from_Keysyms_ಎಣಿಕೆ++;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fromLevel->s.sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
                    /* Single defined keysym */
                    ಆದರೆ (unlikely(intoLevel->ಅಂಕೆ_syms > 1))
                        ಬಿಡುಗಡೆ(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->ಅಂಕೆ_syms = 1;
                    from_Keysyms_ಎಣಿಕೆ++;
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                ಆದರೆ (unlikely(intoLevel->ಅಂಕೆ_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (intoLevel->s.sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
                    /* Single undefined keysym */
                    ಆದರೆ (unlikely(fromLevel->ಅಂಕೆ_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->ಅಂಕೆ_syms = fromLevel->ಅಂಕೆ_syms;
                        fromLevel->ಅಂಕೆ_syms = 0;
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->ಅಂಕೆ_syms = 1;
                    }
                    from_Keysyms_ಎಣಿಕೆ++;
                }
            }
        }

        /* Handle actions */
        ಆದರೆ (!Xkb_ಮಟ್ಟಗಳು_Same_Actions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            ಆದರೆ (report && !(intoHasNoAction || fromHasNoAction)) {
                ಆದರೆ (unlikely(intoLevel->ಅಂಕೆ_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, ಗುಂಪು + 1, ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, ಕೀಲಿ_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ಒಕ್ಕೂಟ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, ಗುಂಪು + 1,
                                ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, ಕೀಲಿ_name),
                                Action_Type_ಪಠ್ಯ(use->type),
                                Action_Type_ಪಠ್ಯ(ignore->type));
                }
            }
            ಆದರೆ (fromHasNoAction) {
                /* No action to copy */
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (clobber) {
                /* Override: copy any defined action from `from` */
                ಆದರೆ (unlikely(fromLevel->ಅಂಕೆ_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    ಆದರೆ (unlikely(intoLevel->ಅಂಕೆ_actions > 1))
                        ಬಿಡುಗಡೆ(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->ಅಂಕೆ_actions = fromLevel->ಅಂಕೆ_actions;
                    fromLevel->ಅಂಕೆ_actions = 0;
                    from_Actions_ಎಣಿಕೆ++;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    ಆದರೆ (unlikely(intoLevel->ಅಂಕೆ_actions > 1))
                        ಬಿಡುಗಡೆ(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->ಅಂಕೆ_actions = 1;
                    from_Actions_ಎಣಿಕೆ++;
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                ಆದರೆ (unlikely(intoLevel->ಅಂಕೆ_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    ಆದರೆ (unlikely(fromLevel->ಅಂಕೆ_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->ಅಂಕೆ_actions = fromLevel->ಅಂಕೆ_actions;
                        fromLevel->ಅಂಕೆ_actions = 0;
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->ಅಂಕೆ_actions = 1;
                    }
                    from_Actions_ಎಣಿಕೆ++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    ರಚನೆ xkb_level *level;
    darray_foreach_from(level, from->ಮಟ್ಟಗಳು, ಮಟ್ಟಗಳು_ಒಳ_both) {
        darray_append(into->ಮಟ್ಟಗಳು, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->ಅಂಕೆ_syms = 0;
        level->ಅಂಕೆ_actions = 0;
        from_Keysyms_ಎಣಿಕೆ++;
        from_Actions_ಎಣಿಕೆ++;
    }
    ಆದರೆ (from_Keysyms_ಎಣಿಕೆ) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        ಆದರೆ (from_Keysyms_ಎಣಿಕೆ == darray_ಗಾತ್ರ(into->ಮಟ್ಟಗಳು))
            into->defined &= ~ಗುಂಪು_ಕ್ಷೇತ್ರ_SYMS;
        into->defined |= (from->defined & ಗುಂಪು_ಕ್ಷೇತ್ರ_SYMS);
    }
    ಆದರೆ (from_Actions_ಎಣಿಕೆ) {
        /* Reset defined actions field if we used no action from `into` */
        ಆದರೆ (from_Actions_ಎಣಿಕೆ == darray_ಗಾತ್ರ(into->ಮಟ್ಟಗಳು))
            into->defined &= ~ಗುಂಪು_ಕ್ಷೇತ್ರ_ACTS;
        into->defined |= (from->defined & ಗುಂಪು_ಕ್ಷೇತ್ರ_ACTS);
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
Use_New_ಕೀಲಿ_ಕ್ಷೇತ್ರ(ಎಣಿಕೆ ಕೀಲಿ_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2, ಎಣಿಕೆ ಕೀಲಿ_ಕ್ಷೇತ್ರ old, ಎಣಿಕೆ ಕೀಲಿ_ಕ್ಷೇತ್ರ new,
               bool clobber, bool report, ಎಣಿಕೆ ಕೀಲಿ_ಕ್ಷೇತ್ರ *collide)
{
    ಆದರೆ (!(old & ಕ್ಷೇತ್ರ_2))
        ಹಿಂತಿರುಗಿ (new & ಕ್ಷೇತ್ರ_2);

    ಆದರೆ (new & ಕ್ಷೇತ್ರ_2) {
        ಆದರೆ (report)
            *collide |= ಕ್ಷೇತ್ರ_2;

        ಹಿಂತಿರುಗಿ clobber;
    }

    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
overlays_get(ಸ್ಥಿರಾಂಕ ಕೀಲಿ_Info *info, xkb_overlay_ಸೂಚ್ಯಂಕ_t ಬಿಟ್,
             ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ **ಕೀಲಿ_ಹೊರಹೋಗು)
{
    ಆದರೆ (ಬಿಟ್ >= (xkb_overlay_ಸೂಚ್ಯಂಕ_t)(ಗಾತ್ರ(xkb_overlay_ಮುಖವಾಡ_t) * CHAR_BIT))
        ಹಿಂತಿರುಗಿ false;
    ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಮುಖವಾಡ = (xkb_overlay_ಮುಖವಾಡ_t)(1u << ಬಿಟ್);
    ಆದರೆ (!(info->overlays & ಮುಖವಾಡ)) ಹಿಂತಿರುಗಿ false;
    ಆದರೆ (ಕೀಲಿ_ಹೊರಹೋಗು) {
        ಆದರೆ (!info->overlays_alloc) {
            /* Stored inline */
            *ಕೀಲಿ_ಹೊರಹೋಗು = info->overlay_ಕೀಲಿ;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Stored in sparse array */
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಕೆಳಗಿನ = (xkb_overlay_ಮುಖವಾಡ_t)(
                info->overlays & (ಮುಖವಾಡ - 1u)
            );
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t ಸೂಚ್ಯಂಕ = (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(ಕೆಳಗಿನ);
            *ಕೀಲಿ_ಹೊರಹೋಗು = info->overlays_ಕೀಲಿಗಳು[ಸೂಚ್ಯಂಕ];
        }
    }
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
overlays_ಸೇರಿಸು(ಕೀಲಿ_Info *keyi, xkb_overlay_ಸೂಚ್ಯಂಕ_t ಬಿಟ್,
                ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    ಆದರೆ (ಬಿಟ್ >= (xkb_overlay_ಸೂಚ್ಯಂಕ_t)(ಗಾತ್ರ(xkb_overlay_ಮುಖವಾಡ_t) * CHAR_BIT))
        ಹಿಂತಿರುಗಿ false;

    ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಮುಖವಾಡ = (xkb_overlay_ಮುಖವಾಡ_t)(1u << ಬಿಟ್);

    ಆದರೆ ((keyi->overlays & ಮುಖವಾಡ) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        ಆದರೆ (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_ಕೀಲಿ = ಕೀಲಿ;
            ಆದರೆ (!ಕೀಲಿ)
                keyi->overlays_clear = true;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Stored in sparse array */
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಕೆಳಗಿನ = (xkb_overlay_ಮುಖವಾಡ_t)(
                keyi->overlays & (xkb_overlay_ಮುಖವಾಡ_t)(ಮುಖವಾಡ - 1u)
            );
            /* index = number of set bits strictly below bit */
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t ಸೂಚ್ಯಂಕ =
                (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(ಕೆಳಗಿನ);
            keyi->overlays_ಕೀಲಿಗಳು[ಸೂಚ್ಯಂಕ] = ಕೀಲಿ;
        }
        ಹಿಂತಿರುಗಿ true;
    }

    /* Insert new element */
    ಆದರೆ (!keyi->overlays || (keyi->overlays_clear && !ಕೀಲಿ)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_ಕೀಲಿ = ಕೀಲಿ;
        keyi->overlays |= ಮುಖವಾಡ;
        keyi->overlays_clear = !ಕೀಲಿ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t overlays = keyi->overlays | ಮುಖವಾಡ;
        ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t alloc =
            (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(overlays);

        /* Alloc sparse array */
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ ** ಸ್ಥಿರಾಂಕ tmp = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(alloc, ಗಾತ್ರ(*tmp));
        ಆದರೆ (!tmp) ಹಿಂತಿರುಗಿ false;

        /* Copy previous entries to the buffer */
        ಆದರೆ (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಕೆಳಗಿನ = (xkb_overlay_ಮುಖವಾಡ_t)(
                overlays & (xkb_overlay_ಮುಖವಾಡ_t)(keyi->overlays - 1u)
            );
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t idx = (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(ಕೆಳಗಿನ);
            tmp[idx] = keyi->overlay_ಕೀಲಿ;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಕೆಳಗಿನ = (xkb_overlay_ಮುಖವಾಡ_t)(
            overlays & (xkb_overlay_ಮುಖವಾಡ_t)(ಮುಖವಾಡ - 1u)
        );
        ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t idx = (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(ಕೆಳಗಿನ);
        tmp[idx] = ಕೀಲಿ;

        keyi->overlays_ಕೀಲಿಗಳು = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Allocated items: grow heap and shift if relevant */
        ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t overlays =
            (xkb_overlay_ಮುಖವಾಡ_t)(keyi->overlays | ಮುಖವಾಡ);
        ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t ಎಣಿಕೆ_2 =
            (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(overlays);
        ಆದರೆ (ಎಣಿಕೆ_2 > keyi->overlays_alloc) {
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t alloc =
                (xkb_overlay_ಸೂಚ್ಯಂಕ_t)ಮುಂದಿನ_pow2(ಎಣಿಕೆ_2);
            ಪ್ರತಿಜ್ಞಾ(alloc >= ಎಣಿಕೆ_2);
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ ** ಸ್ಥಿರಾಂಕ tmp = realloc(
                keyi->overlays_ಕೀಲಿಗಳು,
                (ಗಾತ್ರ_t)alloc * ಗಾತ್ರ(*keyi->overlays_ಕೀಲಿಗಳು)
            );
            ಆದರೆ (!tmp) ಹಿಂತಿರುಗಿ false;
            keyi->overlays_ಕೀಲಿಗಳು = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಕೆಳಗಿನ = (xkb_overlay_ಮುಖವಾಡ_t)(
            overlays & (xkb_overlay_ಮುಖವಾಡ_t)(ಮುಖವಾಡ - 1u)
        );
        ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t ಸೂಚ್ಯಂಕ = (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(ಕೆಳಗಿನ);
        ಆದರೆ (ಸೂಚ್ಯಂಕ >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        ಆದರೆ (ಸೂಚ್ಯಂಕ < ಎಣಿಕೆ_2 - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            ಪ್ರತಿಜ್ಞಾ(ಸೂಚ್ಯಂಕ + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_ಕೀಲಿಗಳು + ಸೂಚ್ಯಂಕ + 1, keyi->overlays_ಕೀಲಿಗಳು + ಸೂಚ್ಯಂಕ,
                    (ಗಾತ್ರ_t)(ಎಣಿಕೆ_2 - 1u - ಸೂಚ್ಯಂಕ) * ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *));
        }
        keyi->overlays_ಕೀಲಿಗಳು[ಸೂಚ್ಯಂಕ] = ಕೀಲಿ;
        keyi->overlays = overlays;
    }

    ಹಿಂತಿರುಗಿ true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ಸ್ಥಿರ bool
ವಿಲೀನ_overlays(SymbolsInfo *info, ಕೀಲಿ_Info *into, ಕೀಲಿ_Info *from,
               bool clobber, bool report, ಎಣಿಕೆ ಕೀಲಿ_ಕ್ಷೇತ್ರ *collide)
{
    ಆದರೆ (!(from->defined & ಕೀಲಿ_ಕ್ಷೇತ್ರ_OVERLAY)) {
        /* `from` is empty: do nothing */
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(into->defined & ಕೀಲಿ_ಕ್ಷೇತ್ರ_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_ಕೀಲಿಗಳು = from->overlays_ಕೀಲಿಗಳು;
        from->overlays_ಕೀಲಿಗಳು = ಖಾಲಿ;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_OVERLAY;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಆದರೆ (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಫಲಿತಾಂಶ_ಮುಖವಾಡ =
                (into->overlays | from->overlays);
            ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t ಎಣಿಕೆ_2 =
                (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(ಫಲಿತಾಂಶ_ಮುಖವಾಡ);

            ಆದರೆ (ಎಣಿಕೆ_2 == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            ಕೀಲಿ_Info *dest = into;
            ಕೀಲಿ_Info *src = from;
            ಆದರೆ (into->overlays_alloc >= ಎಣಿಕೆ_2 &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (from->overlays_alloc >= ಎಣಿಕೆ_2) {
                dest = from;
                src = into;
                clobber = !clobber;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಎಣಿಕೆ_2 > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                ಆದರೆ (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                ಆದರೆ (!dest->overlays_alloc) {
                    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ **tmp = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಎಣಿಕೆ_2, ಗಾತ್ರ(*tmp));
                    ಆದರೆ (!tmp)
                        ಹಿಂತಿರುಗಿ false;
                    ಆದರೆ (dest->overlay_ಕೀಲಿ)
                        tmp[0] = dest->overlay_ಕೀಲಿ;
                    dest->overlays_ಕೀಲಿಗಳು = tmp;
                    dest->overlays_alloc = ಎಣಿಕೆ_2;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ **tmp =
                        realloc(dest->overlays_ಕೀಲಿಗಳು,
                                ಎಣಿಕೆ_2 * ಗಾತ್ರ(*dest->overlays_ಕೀಲಿಗಳು));
                    ಆದರೆ (!tmp)
                        ಹಿಂತಿರುಗಿ false;
                    dest->overlays_ಕೀಲಿಗಳು = tmp;
                    dest->overlays_alloc = ಎಣಿಕೆ_2;
                }
            }

            xkb_overlay_ಮುಖವಾಡ_t ಉಳಿದ = src->overlays;
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ **src_ಕೀಲಿಗಳು = (src->overlays_clear)
                ? ಖಾಲಿ
                : (src->overlays_alloc)
                    ? src->overlays_ಕೀಲಿಗಳು
                    : &src->overlay_ಕೀಲಿ;
            ಆಗ (ಉಳಿದ) {
                /* isolate lowest set bit */
                ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t lsb = (xkb_overlay_ಮುಖವಾಡ_t)(
                    ಉಳಿದ &
                    (xkb_overlay_ಮುಖವಾಡ_t)((ಚಿಹ್ನೆರಹಿತ)~ಉಳಿದ + 1u)
                );
                /* get its index */
                ಸ್ಥಿರಾಂಕ xkb_overlay_ಸೂಚ್ಯಂಕ_t ಬಿಟ್ =
                    (xkb_overlay_ಸೂಚ್ಯಂಕ_t)popcount32(lsb - 1u);
                ಉಳಿದ = (ಉಳಿದ & ~lsb);

                /* pop current value */
                ಆದರೆ (!src->overlays_clear && !src->overlays_alloc && ಉಳಿದ)
                    PANIC_UNREACHABLE();
                ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *src_ಕೀಲಿ = (src_ಕೀಲಿಗಳು)
                    ? *(src_ಕೀಲಿಗಳು++)
                    : ಖಾಲಿ;

                /* check dest */
                ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *dest_ಕೀಲಿ = ಖಾಲಿ;
                ಸ್ಥಿರಾಂಕ bool conflict = overlays_get(dest, ಬಿಟ್, &dest_ಕೀಲಿ);
                ಆದರೆ (conflict) {
                    ಆದರೆ (dest_ಕೀಲಿ == src_ಕೀಲಿ)
                       ಮುಂದುವರಿ;
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (report)
                        *collide |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_OVERLAY;
                }

                ಆದರೆ ((!conflict || clobber) && !overlays_ಸೇರಿಸು(dest, ಬಿಟ್, src_ಕೀಲಿ))
                    ಹಿಂತಿರುಗಿ false;
            }

            ಆದರೆ (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                ಆದರೆ (into->overlays_alloc)
                    ಬಿಡುಗಡೆ(into->overlays_ಕೀಲಿಗಳು);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                ಆದರೆ (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_ಕೀಲಿಗಳು = from->overlays_ಕೀಲಿಗಳು;
                    from->overlays_ಕೀಲಿಗಳು = ಖಾಲಿ;
                    from->overlays_alloc = 0;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    into->overlay_ಕೀಲಿ = from->overlay_ಕೀಲಿ;
                }
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Disjoint overlays */

            ಆದರೆ (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_ಕೀಲಿ == from->overlay_ಕೀಲಿ) {
                /* Indentical: do nothing */
                ಹಿಂತಿರುಗಿ true;
            }
            ಆದರೆ (!(into->overlays & from->overlays)) {
                ಆದರೆ (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_ಕೀಲಿ = from->overlay_ಕೀಲಿ;
                    ಹಿಂತಿರುಗಿ true;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    ಹಿಂತಿರುಗಿ true;
                }
            }
            /* Conflict */
            ಆದರೆ (report)
                *collide |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_OVERLAY;
            ಆದರೆ (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_ಕೀಲಿ = from->overlay_ಕೀಲಿ;
            }
        }
    }
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ವಿಲೀನ_ಕೀಲಿಗಳು(SymbolsInfo *info, ಕೀಲಿ_Info *into, ಕೀಲಿ_Info *from, bool same_file)
{
    xkb_layout_ಸೂಚ್ಯಂಕ_t i;
    xkb_layout_ಸೂಚ್ಯಂಕ_t groups_ಒಳ_both;
    ಎಣಿಕೆ ಕೀಲಿ_ಕ್ಷೇತ್ರ collide = 0;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ಸ್ಥಿರಾಂಕ bool clobber = (from->ವಿಲೀನ != ವಿಲೀನ_AUGMENT);
    ಸ್ಥಿರಾಂಕ bool report = (same_file && verbosity > 0) || verbosity > 9;

    ಆದರೆ (from->ವಿಲೀನ == ವಿಲೀನ_REPLACE) {
        Clear_ಕೀಲಿ_Info(into);
        *into = *from;
        Init_ಕೀಲಿ_Info(info->ctx, from);
        ಹಿಂತಿರುಗಿ true;
    }

    groups_ಒಳ_both = MIN(darray_ಗಾತ್ರ(into->groups), darray_ಗಾತ್ರ(from->groups));
    ಗಾಗಿ (i = 0; i < groups_ಒಳ_both; i++)
        ವಿಲೀನ_Groups(info,
                    &darray_ಅಂಶ(into->groups, i),
                    &darray_ಅಂಶ(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    ಗಾಗಿ (i = groups_ಒಳ_both; i < darray_ಗಾತ್ರ(from->groups); i++) {
        darray_append(into->groups, darray_ಅಂಶ(from->groups, i));
        Init_ಗುಂಪು_Info(&darray_ಅಂಶ(from->groups, i));
    }

    ಆದರೆ (Use_New_ಕೀಲಿ_ಕ್ಷೇತ್ರ(ಕೀಲಿ_ಕ್ಷೇತ್ರ_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_VMODMAP;
    }
    ಆದರೆ (Use_New_ಕೀಲಿ_ಕ್ಷೇತ್ರ(ಕೀಲಿ_ಕ್ಷೇತ್ರ_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_REPEAT;
    }
    ಆದರೆ (Use_New_ಕೀಲಿ_ಕ್ಷೇತ್ರ(ಕೀಲಿ_ಕ್ಷೇತ್ರ_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_DEFAULT_TYPE;
    }
    ಆದರೆ (Use_New_ಕೀಲಿ_ಕ್ಷೇತ್ರ(ಕೀಲಿ_ಕ್ಷೇತ್ರ_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->ಹೊರಹೋಗು_ದ_range_pending_ಗುಂಪು = from->ಹೊರಹೋಗು_ದ_range_pending_ಗುಂಪು;
        into->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy = from->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy;
        into->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ = from->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ;
        into->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_GROUPINFO;
    }

    ಆದರೆ (!ವಿಲೀನ_overlays(info, into, from, clobber, report, &collide))
        ಹಿಂತಿರುಗಿ false;

    ಆದರೆ (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_ಕೀಲಿ_Info(from);
    Init_ಕೀಲಿ_Info(info->ctx, from);
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ xkb_atom_t
Xkb_Resolve_ಕೀಲಿ_Alias(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap *keymap, xkb_atom_t name)
{
    ಆದರೆ (name < keymap->ಅಂಕೆ_ಕೀಲಿ_ಹೆಸರುಗಳು) {
        ಸ್ಥಿರಾಂಕ Keycode_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ = keymap->ಕೀಲಿ_ಹೆಸರುಗಳು[name];
        ಆದರೆ (ಹೊಂದಿಕೆ.ಸಿಕ್ಕಿತು && ಹೊಂದಿಕೆ.is_alias) {
            ಹಿಂತಿರುಗಿ ಹೊಂದಿಕೆ.alias.real;
        }
    }
    ಹಿಂತಿರುಗಿ name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ಸ್ಥಿರ bool
ಕೂಡಿಸು_ಕೀಲಿ_Symbols(SymbolsInfo *info, ಕೀಲಿ_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_ಕೀಲಿ_Alias(&info->keymap_info->keymap, keyi->name);

    ಕೀಲಿ_Info *iter;
    darray_foreach(iter, info->ಕೀಲಿಗಳು)
        ಆದರೆ (iter->name == keyi->name)
            ಹಿಂತಿರುಗಿ ವಿಲೀನ_ಕೀಲಿಗಳು(info, iter, keyi, same_file);

    darray_append(info->ಕೀಲಿಗಳು, *keyi);
    Init_ಕೀಲಿ_Info(info->ctx, keyi);
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಕೂಡಿಸು_Mod_Map_ನಮೂದು(SymbolsInfo *info, ಸ್ಥಿರಾಂಕ Mod_Map_ನಮೂದು *new)
{
    ಸ್ಥಿರಾಂಕ bool clobber = (new->ವಿಲೀನ != ವಿಲೀನ_AUGMENT);

    Mod_Map_ನಮೂದು *old;
    darray_foreach(old, info->modmaps) {
        ಆದರೆ ((new->have_ಚಿಹ್ನೆ != old->have_ಚಿಹ್ನೆ) ||
            (new->have_ಚಿಹ್ನೆ && new->u.ಕೀಲಿ_Sym != old->u.ಕೀಲಿ_Sym) ||
            (!new->have_ಚಿಹ್ನೆ && new->u.ಕೀಲಿ_Name != old->u.ಕೀಲಿ_Name))
            ಮುಂದುವರಿ;

        ಆದರೆ (new->mods == old->mods)
            ಹಿಂತಿರುಗಿ true;

        ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t use = (clobber ? new->mods : old->mods);
        ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t ignore = (clobber ? old->mods : new->mods);

        ಆದರೆ (new->have_ಚಿಹ್ನೆ) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    Keysym_ಪಠ್ಯ(info->ctx, new->u.ಕೀಲಿ_Sym),
                    Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_REAL, &info->mods, ignore));
        } ಇಲ್ಲದಿದ್ದರೆ {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, new->u.ಕೀಲಿ_Name),
                    Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        ಹಿಂತಿರುಗಿ true;
    }

    darray_append(info->modmaps, *new);

    ಹಿಂತಿರುಗಿ true;
}

/***====================================================================***/

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಲೀನ_Included_Symbols(SymbolsInfo *into, SymbolsInfo *from,
                     ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ)
{
    xkb_atom_t *ಗುಂಪು_name;
    xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು_ಹೆಸರುಗಳು_ಒಳ_both;

    ಆದರೆ (from->error_ಎಣಿಕೆ > 0) {
        into->error_ಎಣಿಕೆ += from->error_ಎಣಿಕೆ;
        ಹಿಂತಿರುಗಿ;
    }

    ವಿಲೀನ_Mod_Sets(into->ctx, &into->mods, &from->mods, ವಿಲೀನ);

    ಆದರೆ (into->name == ಖಾಲಿ) {
        into->name = steal(&from->name);
    }

    ಗುಂಪು_ಹೆಸರುಗಳು_ಒಳ_both = MIN(darray_ಗಾತ್ರ(into->ಗುಂಪು_ಹೆಸರುಗಳು),
                              darray_ಗಾತ್ರ(from->ಗುಂಪು_ಹೆಸರುಗಳು));
    ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t i = 0; i < ಗುಂಪು_ಹೆಸರುಗಳು_ಒಳ_both; i++) {
        ಆದರೆ (!darray_ಅಂಶ(from->ಗುಂಪು_ಹೆಸರುಗಳು, i))
            ಮುಂದುವರಿ;

        ಆದರೆ (ವಿಲೀನ == ವಿಲೀನ_AUGMENT && darray_ಅಂಶ(into->ಗುಂಪು_ಹೆಸರುಗಳು, i))
            ಮುಂದುವರಿ;

        darray_ಅಂಶ(into->ಗುಂಪು_ಹೆಸರುಗಳು, i) = darray_ಅಂಶ(from->ಗುಂಪು_ಹೆಸರುಗಳು, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(ಗುಂಪು_name, from->ಗುಂಪು_ಹೆಸರುಗಳು, ಗುಂಪು_ಹೆಸರುಗಳು_ಒಳ_both)
        darray_append(into->ಗುಂಪು_ಹೆಸರುಗಳು, *ಗುಂಪು_name);

    ಆದರೆ (darray_ಖಾಲಿಗಳು(into->ಕೀಲಿಗಳು)) {
        into->ಕೀಲಿಗಳು = from->ಕೀಲಿಗಳು;
        darray_init(from->ಕೀಲಿಗಳು);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಕೀಲಿ_Info *keyi;
        darray_foreach(keyi, from->ಕೀಲಿಗಳು) {
            keyi->ವಿಲೀನ = ವಿಲೀನ;
            ಆದರೆ (!ಕೂಡಿಸು_ಕೀಲಿ_Symbols(into, keyi, false))
                into->error_ಎಣಿಕೆ++;
        }
    }

    ಆದರೆ (darray_ಖಾಲಿಗಳು(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        Mod_Map_ನಮೂದು *mm;
        darray_foreach(mm, from->modmaps) {
            mm->ವಿಲೀನ = ವಿಲೀನ;
            ಆದರೆ (!ಕೂಡಿಸು_Mod_Map_ನಮೂದು(into, mm))
                into->error_ಎಣಿಕೆ++;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ಸ್ಥಿರ bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    ಆದರೆ (Exceeds_Include_Max_ಆಳ(info->ctx, info->include_ಆಳ)) {
        info->error_ಎಣಿಕೆ += 10;
        ಹಿಂತಿರುಗಿ false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_ಆಳ + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ಗಾಗಿ (IncludeStmt *stmt = include; stmt; stmt = stmt->ಮುಂದಿನ_incl) {
        SymbolsInfo ಮುಂದಿನ_incl;
        XkbFile *file;

        ಅಕ್ಷರ ಮಾರ್ಗ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  ಮಾರ್ಗ, ಗಾತ್ರ(ಮಾರ್ಗ));
        ಆದರೆ (!file) {
            info->error_ಎಣಿಕೆ += 10;
            ClearSymbolsInfo(&included);
            ಹಿಂತಿರುಗಿ false;
        }

        InitSymbolsInfo(&ಮುಂದಿನ_incl, info->keymap_info, info->include_ಆಳ + 1,
                        &included.mods);
        ಆದರೆ (stmt->modifier) {
            ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = parse_dec_ಗಮ್ಯ_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &ಮುಂದಿನ_incl.explicit_ಗುಂಪು);
            ಆದರೆ (ಎಣಿಕೆ_2 <= 0 || *(stmt->modifier + ಎಣಿಕೆ_2) != '\0' ||
                ಮುಂದಿನ_incl.explicit_ಗುಂಪು == 0 ||
                ಮುಂದಿನ_incl.explicit_ಗುಂಪು > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                ಮುಂದಿನ_incl.explicit_ಗುಂಪು = info->explicit_ಗುಂಪು;
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಮುಂದಿನ_incl.explicit_ಗುಂಪು--;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (info->keymap_info->keymap.ಅಂಕೆ_groups != 0 &&
                 ಮುಂದಿನ_incl.include_ಆಳ == 1) {
            /*
             * If keymap is the result of RMLVO resolution and we are at the
             * first include depth, transform e.g. `pc` into `pc:1` in order to
             * force only one group per key using the explicit group.
             *
             * NOTE: X11’s xkbcomp does not apply this rule.
             *
             * WARNING: If this feature is removed, then the entries `Last` in
             * the `groupIndexNames` and `groupMaskNames` LUT should initially
             * be inactive.
             */
            ಮುಂದಿನ_incl.explicit_ಗುಂಪು = 0;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            ಮುಂದಿನ_incl.explicit_ಗುಂಪು = info->explicit_ಗುಂಪು;
        }

        HandleSymbolsFile(&ಮುಂದಿನ_incl, file);

        ವಿಲೀನ_Included_Symbols(&included, &ಮುಂದಿನ_incl, stmt->ವಿಲೀನ);

        ClearSymbolsInfo(&ಮುಂದಿನ_incl);
        FreeXkbFile(file);
    }

    ವಿಲೀನ_Included_Symbols(info, &included, include->ವಿಲೀನ);
    ClearSymbolsInfo(&included);

    ಹಿಂತಿರುಗಿ (info->error_ಎಣಿಕೆ == 0);
}

ಸ್ಥಿರ bool
Get_ಗುಂಪು_ಸೂಚ್ಯಂಕ(SymbolsInfo *info, ಕೀಲಿ_Info *keyi, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
              ಎಣಿಕೆ ಗುಂಪು_ಕ್ಷೇತ್ರ ಕ್ಷೇತ್ರ_2, xkb_layout_ಸೂಚ್ಯಂಕ_t *ndx_rtrn)
{
    ಪ್ರತಿಜ್ಞಾ (ಕ್ಷೇತ್ರ_2 == ಗುಂಪು_ಕ್ಷೇತ್ರ_SYMS || ಕ್ಷೇತ್ರ_2 == ಗುಂಪು_ಕ್ಷೇತ್ರ_ACTS);
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name = (ಕ್ಷೇತ್ರ_2 == ಗುಂಪು_ಕ್ಷೇತ್ರ_SYMS ? "symbols" : "actions");

    ಆದರೆ (arrayNdx == ಖಾಲಿ) {
        xkb_layout_ಸೂಚ್ಯಂಕ_t i = 0;
        ಗುಂಪು_Info_2 *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            ಆದರೆ (!(groupi->defined & ಕ್ಷೇತ್ರ_2)) {
                *ndx_rtrn = i;
                ಹಿಂತಿರುಗಿ true;
            }
        }

        ಆದರೆ (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi), info->max_groups, name);
            ಹಿಂತಿರುಗಿ false;
        }

        darray_resize0(keyi->groups, darray_ಗಾತ್ರ(keyi->groups) + 1);
        *ndx_rtrn = darray_ಗಾತ್ರ(keyi->groups) - 1;
        ಹಿಂತಿರುಗಿ true;
    }

    ಆದರೆ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು(info->keymap_info, arrayNdx, false, ndx_rtrn, ಖಾಲಿ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
        ಹಿಂತಿರುಗಿ false;
    }

    (*ndx_rtrn)--;
    ಆದರೆ (*ndx_rtrn >= darray_ಗಾತ್ರ(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಕೂಡಿಸು_Symbols_ಗಮ್ಯ_ಕೀಲಿ(SymbolsInfo *info, ಕೀಲಿ_Info *keyi, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
                ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    xkb_layout_ಸೂಚ್ಯಂಕ_t ndx = 0;

    ಆದರೆ (!Get_ಗುಂಪು_ಸೂಚ್ಯಂಕ(info, keyi, arrayNdx, ಗುಂಪು_ಕ್ಷೇತ್ರ_SYMS, &ndx))
        ಹಿಂತಿರುಗಿ false;

    ಗುಂಪು_Info_2 *groupi = &darray_ಅಂಶ(keyi->groups, ndx);

    ಆದರೆ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_LIST) {
        groupi->defined |= ಗುಂಪು_ಕ್ಷೇತ್ರ_SYMS;
        ಹಿಂತಿರುಗಿ true;
    }

    ಆದರೆ (ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_ಗಮ್ಯ_string(ಮೌಲ್ಯ->common.type), ndx + 1,
                ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (groupi->defined & ಗುಂಪು_ಕ್ಷೇತ್ರ_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_ಕೀಲಿ_SYMBOLS_ನಮೂದು,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi), ndx + 1);
        ಹಿಂತಿರುಗಿ false;
    }

    xkb_level_ಸೂಚ್ಯಂಕ_t n_ಮಟ್ಟಗಳು = 0;
    xkb_level_ಸೂಚ್ಯಂಕ_t non_ಖಾಲಿಗಳು_ಮಟ್ಟಗಳು = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    ಗಾಗಿ (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *keysymList = (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *) ಮೌಲ್ಯ;
         keysymList;
         keysymList = (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *) keysymList->common.ಮುಂದಿನ) {
        n_ಮಟ್ಟಗಳು++;
        /* Drop trailing NoSymbol */
        ಆದರೆ (darray_ಗಾತ್ರ(keysymList->syms) > 0)
            non_ಖಾಲಿಗಳು_ಮಟ್ಟಗಳು = n_ಮಟ್ಟಗಳು;
    }
    ಆದರೆ (non_ಖಾಲಿಗಳು_ಮಟ್ಟಗಳು < n_ಮಟ್ಟಗಳು)
        n_ಮಟ್ಟಗಳು = non_ಖಾಲಿಗಳು_ಮಟ್ಟಗಳು;
    ಆದರೆ (darray_ಗಾತ್ರ(groupi->ಮಟ್ಟಗಳು) < n_ಮಟ್ಟಗಳು)
        darray_resize0(groupi->ಮಟ್ಟಗಳು, n_ಮಟ್ಟಗಳು);

    groupi->defined |= ಗುಂಪು_ಕ್ಷೇತ್ರ_SYMS;

    xkb_level_ಸೂಚ್ಯಂಕ_t level = 0;
    ಗಾಗಿ (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *keysymList = (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *) ಮೌಲ್ಯ;
         keysymList && level < n_ಮಟ್ಟಗಳು;
         keysymList = (ಅಭಿವ್ಯಕ್ತಿ_Keysym_List *) keysymList->common.ಮುಂದಿನ, level++) {
        ರಚನೆ xkb_level *leveli = &darray_ಅಂಶ(groupi->ಮಟ್ಟಗಳು, level);
        ಪ್ರತಿಜ್ಞಾ(leveli->ಅಂಕೆ_syms == 0);

        ಆದರೆ (unlikely(darray_ಗಾತ್ರ(keysymList->syms) > MAX_KEYSYMS_ಪ್ರತಿ_LEVEL)) {
            log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_ಪ್ರತಿ_LEVEL, darray_ಗಾತ್ರ(keysymList->syms));
            ಹಿಂತಿರುಗಿ false;
        }

        leveli->ಅಂಕೆ_syms = (xkb_keysym_ಎಣಿಕೆ_t) darray_ಗಾತ್ರ(keysymList->syms);
        ಆಯ್ಕೆ (leveli->ಅಂಕೆ_syms) {
        ಸ್ಥಿತಿ 0:
            leveli->s.sym = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 1:
            leveli->s.sym = darray_ಅಂಶ(keysymList->syms, 0);
            ಪ್ರತಿಜ್ಞಾ(leveli->s.sym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, ಖಾಲಿ);
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            ಗಾಗಿ (xkb_keysym_ಎಣಿಕೆ_t k = 0; k < leveli->ಅಂಕೆ_syms; k++)
                ಪ್ರತಿಜ್ಞಾ(leveli->s.syms[k] != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);
#ಆದರೆ_ಅಂತ್ಯ
        }
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಕೂಡಿಸು_Actions_ಗಮ್ಯ_ಕೀಲಿ(SymbolsInfo *info, ಕೀಲಿ_Info *keyi, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
                ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ)
{
    xkb_layout_ಸೂಚ್ಯಂಕ_t ndx = 0;

    ಆದರೆ (!Get_ಗುಂಪು_ಸೂಚ್ಯಂಕ(info, keyi, arrayNdx, ಗುಂಪು_ಕ್ಷೇತ್ರ_ACTS, &ndx))
        ಹಿಂತಿರುಗಿ false;

    ಗುಂಪು_Info_2 *groupi = &darray_ಅಂಶ(keyi->groups, ndx);

    ಆದರೆ (ಮೌಲ್ಯ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_ಖಾಲಿಗಳು_LIST) {
        groupi->defined |= ಗುಂಪು_ಕ್ಷೇತ್ರ_ACTS;
        ಹಿಂತಿರುಗಿ true;
    }

    ಆದರೆ (ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_ಅಭಿವ್ಯಕ್ತಿ_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 ಮೌಲ್ಯ->common.type, ndx, ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (groupi->defined & ಗುಂಪು_ಕ್ಷೇತ್ರ_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi), ndx);
        ಹಿಂತಿರುಗಿ false;
    }

    xkb_level_ಸೂಚ್ಯಂಕ_t n_ಮಟ್ಟಗಳು = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    ಗಾಗಿ (ParseCommon *p = &ಮೌಲ್ಯ->common; p; p = p->ಮುಂದಿನ)
        n_ಮಟ್ಟಗಳು++;
    ಆದರೆ (darray_ಗಾತ್ರ(groupi->ಮಟ್ಟಗಳು) < n_ಮಟ್ಟಗಳು)
        darray_resize0(groupi->ಮಟ್ಟಗಳು, n_ಮಟ್ಟಗಳು);

    groupi->defined |= ಗುಂಪು_ಕ್ಷೇತ್ರ_ACTS;

    xkb_level_ಸೂಚ್ಯಂಕ_t level = 0;
    xkb_level_ಸೂಚ್ಯಂಕ_t non_ಖಾಲಿಗಳು_ಮಟ್ಟಗಳು = 0;
    ಗಾಗಿ (ಅಭಿವ್ಯಕ್ತಿ_Action_List *actionList = (ಅಭಿವ್ಯಕ್ತಿ_Action_List *) ಮೌಲ್ಯ;
         actionList;
         actionList = (ಅಭಿವ್ಯಕ್ತಿ_Action_List *) actionList->common.ಮುಂದಿನ, level++) {
        ರಚನೆ xkb_level *leveli = &darray_ಅಂಶ(groupi->ಮಟ್ಟಗಳು, level);
        ಪ್ರತಿಜ್ಞಾ(leveli->ಅಂಕೆ_actions == 0);

        ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ_actions = 0;
        ಗಾಗಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *act = actionList->actions;
             act; act = (ಅಭಿವ್ಯಕ್ತಿ_Def *) act->common.ಮುಂದಿನ)
            ಅಂಕೆ_actions++;

        ಆದರೆ (unlikely(ಅಂಕೆ_actions > MAX_ACTIONS_ಪ್ರತಿ_LEVEL)) {
            log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_ಪ್ರತಿ_LEVEL, ಅಂಕೆ_actions);
            ಹಿಂತಿರುಗಿ false;
        }

        /* Parse actions and add only defined actions */
        darray(ಒಕ್ಕೂಟ xkb_action) actions = darray_new();
        ಗಾಗಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *act = actionList->actions;
             act; act = (ಅಭಿವ್ಯಕ್ತಿ_Def *) act->common.ಮುಂದಿನ) {
            ಒಕ್ಕೂಟ xkb_action ಗಮ್ಯ_Act = { 0 };
            ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &ಗಮ್ಯ_Act);
            ಆದರೆ (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi), ndx + 1, level + 1);
                ಆದರೆ (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    ಹಿಂತಿರುಗಿ false;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    /* Ensure action type is reset */
                    ಗಮ್ಯ_Act.type = ACTION_TYPE_NONE;
                }
            }
            ಆದರೆ (ಗಮ್ಯ_Act.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (likely(ಅಂಕೆ_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->ಅಂಕೆ_actions = 1;
                leveli->a.action = ಗಮ್ಯ_Act;
                ಹೋಗು ಮುಂದಿನ_level;
            } ಇಲ್ಲದಿದ್ದರೆ {
                darray_append(actions, ಗಮ್ಯ_Act);
            }
        }
        ಆದರೆ (darray_ಖಾಲಿಗಳು(actions)) {
            leveli->ಅಂಕೆ_actions = 0;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (likely(darray_ಗಾತ್ರ(actions) > 1)) {
            leveli->ಅಂಕೆ_actions = (xkb_action_ಎಣಿಕೆ_t) darray_ಗಾತ್ರ(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, ಖಾಲಿ);
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NDEBUG
            /* Canonical list: all NoAction() were dropped */
            ಗಾಗಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < leveli->ಅಂಕೆ_actions; k++)
                ಪ್ರತಿಜ್ಞಾ(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#ಆದರೆ_ಅಂತ್ಯ
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Unlikely: some actions were dropped and only one remains */
            ಪ್ರತಿಜ್ಞಾ(ಅಂಕೆ_actions > darray_ಗಾತ್ರ(actions));
            leveli->ಅಂಕೆ_actions = 1;
            leveli->a.action = darray_ಅಂಶ(actions, 0);
            ಪ್ರತಿಜ್ಞಾ(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
ಮುಂದಿನ_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        ಆದರೆ (leveli->ಅಂಕೆ_actions > 0 || leveli->ಅಂಕೆ_syms > 0)
            non_ಖಾಲಿಗಳು_ಮಟ್ಟಗಳು = level + 1;
    }

    ಆದರೆ (non_ಖಾಲಿಗಳು_ಮಟ್ಟಗಳು < n_ಮಟ್ಟಗಳು) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_ಗಾತ್ರ(groupi->ಮಟ್ಟಗಳು) = non_ಖಾಲಿಗಳು_ಮಟ್ಟಗಳು;
        ಆದರೆ (non_ಖಾಲಿಗಳು_ಮಟ್ಟಗಳು > 0)
            darray_shrink(groupi->ಮಟ್ಟಗಳು);
        ಇಲ್ಲದಿದ್ದರೆ
            darray_free(groupi->ಮಟ್ಟಗಳು);
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ Lookup_ನಮೂದು repeatEntries[] = {
    { "true", ಕೀಲಿ_REPEAT_YES },
    { "yes", ಕೀಲಿ_REPEAT_YES },
    { "on", ಕೀಲಿ_REPEAT_YES },
    { "false", ಕೀಲಿ_REPEAT_NO },
    { "no", ಕೀಲಿ_REPEAT_NO },
    { "off", ಕೀಲಿ_REPEAT_NO },
    { "default", ಕೀಲಿ_REPEAT_UNDEFINED },
    { ಖಾಲಿ, 0 }
};

/** Handle overlay<N> = <key> entry */
ಸ್ಥಿರ bool
ಅಭಿವ್ಯಕ್ತಿ_Resolve_Overlay_ನಮೂದು(ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2, ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx,
                        ಸ್ಥಿರಾಂಕ ಅಭಿವ್ಯಕ್ತಿ_Def *ಅಭಿವ್ಯಕ್ತಿ, ಕೀಲಿ_Info *keyi,
                        xkb_overlay_ಸೂಚ್ಯಂಕ_t *overlay_rtrn,
                        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ **ಕೀಲಿ_rtrn)
{
    /* Overlay index */
    ಆದರೆ (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                ಕ್ಷೇತ್ರ_2, ಕೀಲಿ_Name_ಪಠ್ಯ(keymap_info->keymap.ctx, keyi->name));
        ಹಿಂತಿರುಗಿ false;
    }
    static_assert(ಗಾತ್ರ(xkb_overlay_ಸೂಚ್ಯಂಕ_t) == ಗಾತ್ರ(uint8_t), "");
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t ಪೂರ್ವಪ್ರತ್ಯಯ = ಗಾತ್ರ("overlay") - 1;
    ಸ್ಥಿರಾಂಕ ಗಾತ್ರ_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಕ್ಷೇತ್ರ_2 + ಪೂರ್ವಪ್ರತ್ಯಯ);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    ಆದರೆ (parse_dec_ಗಮ್ಯ_uint64_t(ಕ್ಷೇತ್ರ_2 + ಪೂರ್ವಪ್ರತ್ಯಯ, len, (uint64_t *)&raw_overlay)
        != (ಪೂರ್ಣಾಂಕ)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_ಸೂಚ್ಯಂಕ,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                ಕ್ಷೇತ್ರ_2, ಕೀಲಿ_Name_ಪಠ್ಯ(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        ಹಿಂತಿರುಗಿ false;
    }
    *overlay_rtrn = (xkb_overlay_ಸೂಚ್ಯಂಕ_t)raw_overlay - 1;

    /* Overlay key */
    ಆಯ್ಕೆ (ಅಭಿವ್ಯಕ್ತಿ->common.type) {
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL:
        *ಕೀಲಿ_rtrn = Xkb_ಕೀಲಿ_By_Name(&keymap_info->keymap, ಅಭಿವ್ಯಕ್ತಿ->ಕೀಲಿ_name.ಕೀಲಿ_name,
                                 false);
        ಆದರೆ (!*ಕೀಲಿ_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_ಪಠ್ಯ(keymap_info->keymap.ctx, ಅಭಿವ್ಯಕ್ತಿ->ಕೀಲಿ_name.ಕೀಲಿ_name),
                    ಕ್ಷೇತ್ರ_2, ಕೀಲಿ_Name_ಪಠ್ಯ(keymap_info->keymap.ctx, keyi->name));
            ಹಿಂತಿರುಗಿ false;
        }
        ಹಿಂತಿರುಗಿ true;
    ಸ್ಥಿತಿ STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT: {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ id =
            xkb_atom_ಪಠ್ಯ(keymap_info->keymap.ctx, ಅಭಿವ್ಯಕ್ತಿ->ident.ident);
        ಆದರೆ (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *ಕೀಲಿ_rtrn = ಖಾಲಿ;
            ಹಿಂತಿರುಗಿ true;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *ಕೀಲಿ_rtrn = ಖಾಲಿ;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            ಹಿಂತಿರುಗಿ true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, ಕ್ಷೇತ್ರ_2, ಕೀಲಿ_Name_ಪಠ್ಯ(keymap_info->keymap.ctx, keyi->name));
        ಹಿಂತಿರುಗಿ false;
    }
    ಪೂರ್ವನಿಯೋಜಿತ:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_ಗಮ್ಯ_string(STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL),
                ಕ್ಷೇತ್ರ_2, ಕೀಲಿ_Name_ಪಠ್ಯ(keymap_info->keymap.ctx, keyi->name),
                stmt_type_ಗಮ್ಯ_string(ಅಭಿವ್ಯಕ್ತಿ->common.type));
        ಹಿಂತಿರುಗಿ false;
    }
}

ಸ್ಥಿರ bool
ಗಣ_Symbols_ಕ್ಷೇತ್ರ(SymbolsInfo *info, ಕೀಲಿ_Info *keyi, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2,
                ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx, ಅಭಿವ್ಯಕ್ತಿ_Def **ಮೌಲ್ಯ_ptr)
{
    ಅಭಿವ್ಯಕ್ತಿ_Def * ಸೀಮಿತ ಸ್ಥಿರಾಂಕ ಮೌಲ್ಯ = *ಮೌಲ್ಯ_ptr;
    ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "type")) {
        xkb_layout_ಸೂಚ್ಯಂಕ_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(info->ctx, ಮೌಲ್ಯ, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            ಹಿಂತಿರುಗಿ false;
        }

        ಆದರೆ (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_DEFAULT_TYPE;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು(info->keymap_info, arrayNdx, false,
                                  &ndx, ಖಾಲಿ) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
            ಹಿಂತಿರುಗಿ false;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ndx--;
            ಆದರೆ (ndx >= darray_ಗಾತ್ರ(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                ಪ್ರತಿಜ್ಞಾ(darray_ಗಾತ್ರ(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_ಅಂಶ(keyi->groups, ndx).type = val;
            darray_ಅಂಶ(keyi->groups, ndx).defined |= ಗುಂಪು_ಕ್ಷೇತ್ರ_TYPE;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "symbols")) {
        ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_Symbols_ಗಮ್ಯ_ಕೀಲಿ(info, keyi, arrayNdx, ಮೌಲ್ಯ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "actions")) {
        ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_Actions_ಗಮ್ಯ_ಕೀಲಿ(info, keyi, arrayNdx, ಮೌಲ್ಯ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "vmods") ||
             istreq(ಕ್ಷೇತ್ರ_2, "virtualmods") ||
             istreq(ಕ್ಷೇತ್ರ_2, "virtualmodifiers")) {
        xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ = 0;

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(info->ctx, ಮೌಲ್ಯ, MOD_VIRT, &info->mods,
                                &ಮುಖವಾಡ)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ಮುಖವಾಡ,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_ಗಮ್ಯ_string(ಮೌಲ್ಯ->common.type),
                    ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
            ಹಿಂತಿರುಗಿ false;
        }

        keyi->vmodmap = ಮುಖವಾಡ;
        keyi->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_VMODMAP;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "locking") ||
             istreq(ಕ್ಷೇತ್ರ_2, "lock") ||
             istreq(ಕ್ಷೇತ್ರ_2, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ಕ್ಷೇತ್ರ,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "radiogroup") ||
             istreq(ಕ್ಷೇತ್ರ_2, "permanentradiogroup") ||
             istreq(ಕ್ಷೇತ್ರ_2, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ಕ್ಷೇತ್ರ,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("permanentoverlay", ಕ್ಷೇತ್ರ_2)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ಕ್ಷೇತ್ರ,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq_ಪೂರ್ವಪ್ರತ್ಯಯ("overlay", ಕ್ಷೇತ್ರ_2)) {
        xkb_overlay_ಸೂಚ್ಯಂಕ_t overlay = XKB_OVERLAY_INVALID;
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = ಖಾಲಿ;
        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Overlay_ನಮೂದು(info->keymap_info, ಕ್ಷೇತ್ರ_2, arrayNdx,
                                     *ಮೌಲ್ಯ_ptr, keyi, &overlay, &ಕೀಲಿ))
            ಹಿಂತಿರುಗಿ false;
        ಆದರೆ (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            ಹಿಂತಿರುಗಿ true;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ && ಕೀಲಿ->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ *prev;
            ಆದರೆ (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                ಆದರೆ (ಕೀಲಿ != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi), overlay + 1,
                             (prev
                                 ? ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (ಕೀಲಿ
                                 ? ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, ಕೀಲಿ->name)
                                 : "none"));
                }
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                ಆದರೆ (!overlays_ಸೇರಿಸು(keyi, overlay, ಕೀಲಿ))
                    ಹಿಂತಿರುಗಿ false;
                keyi->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_OVERLAY;
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Handle disjoint overlays mode */
                ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t ಮುಖವಾಡ = (xkb_overlay_ಮುಖವಾಡ_t)(1u << overlay);
                ಆದರೆ (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    ಆದರೆ (ಕೀಲಿ) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = ಮುಖವಾಡ;
                        keyi->overlays_clear = false;
                        keyi->overlay_ಕೀಲಿ = ಕೀಲಿ;
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= ಮುಖವಾಡ;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_OVERLAY;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (keyi->overlays) {
                    ಆದರೆ (ಕೀಲಿ) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi), keyi->overlays,
                                ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, keyi->overlay_ಕೀಲಿ->name),
                                overlay + 1, ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, ಕೀಲಿ->name));
                        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict &
                                 PARSER_NO_ಕ್ಷೇತ್ರ_ಮೌಲ್ಯ_MISMATCH);
                    } ಇಲ್ಲದಿದ್ದರೆ {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "repeating") ||
             istreq(ಕ್ಷೇತ್ರ_2, "repeats") ||
             istreq(ಕ್ಷೇತ್ರ_2, "repeat")) {
        uint32_t val = 0;

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Enum(info->ctx, ಮೌಲ್ಯ, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
            ಹಿಂತಿರುಗಿ false;
        }

        keyi->repeat = (ಎಣಿಕೆ ಕೀಲಿ_repeat) val;
        keyi->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_REPEAT;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "groupswrap") ||
             istreq(ಕ್ಷೇತ್ರ_2, "wrapgroups")) {
        bool ಗಣ = false;

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(info->ctx, ಮೌಲ್ಯ, &ಗಣ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
            ಹಿಂತಿರುಗಿ false;
        }

        keyi->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy = (ಗಣ)
            ? XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP
            : XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP;
        keyi->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_GROUPINFO;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "groupsclamp") ||
             istreq(ಕ್ಷೇತ್ರ_2, "clampgroups")) {
        bool ಗಣ = false;

        ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Boolean(info->ctx, ಮೌಲ್ಯ, &ಗಣ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ಮೌಲ್ಯ,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
            ಹಿಂತಿರುಗಿ false;
        }

        keyi->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy = (ಗಣ)
            ? XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP
            : XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP;
        keyi->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_GROUPINFO;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (istreq(ಕ್ಷೇತ್ರ_2, "groupsredirect") ||
             istreq(ಕ್ಷೇತ್ರ_2, "redirectgroups")) {
        xkb_layout_ಸೂಚ್ಯಂಕ_t grp = 0;
        bool pending = false;

        // TODO: recover?
        ಆದರೆ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು(info->keymap_info, ಮೌಲ್ಯ, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
            ಹಿಂತಿರುಗಿ false;
        }

        ಆದರೆ (pending) {
            keyi->ಹೊರಹೋಗು_ದ_range_pending_ಗುಂಪು = true;
            ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t pending_ಸೂಚ್ಯಂಕ =
                darray_ಗಾತ್ರ(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (ರಚನೆ pending_computation) {
                    .ಅಭಿವ್ಯಕ್ತಿ = *ಮೌಲ್ಯ_ptr,
                    .computed = false,
                    .ಮೌಲ್ಯ = 0,
                }
            );
            *ಮೌಲ್ಯ_ptr = ಖಾಲಿ;
            static_assert(ಗಾತ್ರ(keyi->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ) ==
                          ಗಾತ್ರ(pending_ಸೂಚ್ಯಂಕ),
                          "Cannot save pending computation");
            keyi->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ = pending_ಸೂಚ್ಯಂಕ;
        } ಇಲ್ಲದಿದ್ದರೆ {
            keyi->ಹೊರಹೋಗು_ದ_range_pending_ಗುಂಪು = false;
            keyi->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ = grp - 1;
        }

        keyi->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy = XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT;
        keyi->defined |= ಕೀಲಿ_ಕ್ಷೇತ್ರ_GROUPINFO;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_ಕ್ಷೇತ್ರ,
                "Unknown field \"%s\" in a key; definition ignored\n",
                ಕ್ಷೇತ್ರ_2);
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_ಕೀಲಿ_FIELDS);
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
ಗಣ_ಗುಂಪು_Name(SymbolsInfo *info, ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx, ಅಭಿವ್ಯಕ್ತಿ_Def *ಮೌಲ್ಯ,
             ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ)
{
    ಆದರೆ (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_ಗುಂಪು_NAME_ಸೂಚ್ಯಂಕ,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        ಹಿಂತಿರುಗಿ false;
    }

    xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0;
    ಆದರೆ (ಅಭಿವ್ಯಕ್ತಿ_Resolve_ಗುಂಪು(info->keymap_info, arrayNdx, false, &ಗುಂಪು, ಖಾಲಿ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ಸೂಚ್ಯಂಕ_2,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        ಹಿಂತಿರುಗಿ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(info->ctx, ಮೌಲ್ಯ, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", ಗುಂಪು);
        ಹಿಂತಿರುಗಿ false;
    }

    xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು_ಗಮ್ಯ_use;
    ಆದರೆ (info->explicit_ಗುಂಪು == XKB_LAYOUT_INVALID) {
        ಗುಂಪು_ಗಮ್ಯ_use = ಗುಂಪು - 1;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗುಂಪು - 1 == 0) {
        ಗುಂಪು_ಗಮ್ಯ_use = info->explicit_ಗುಂಪು;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        log_warn(info->ctx, XKB_WARNING_NON_ತಳ_ಗುಂಪು_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, ಗುಂಪು,
                 xkb_atom_ಪಠ್ಯ(info->ctx, name));
        ಹಿಂತಿರುಗಿ false;
    }

    ಆದರೆ (ಗುಂಪು_ಗಮ್ಯ_use >= darray_ಗಾತ್ರ(info->ಗುಂಪು_ಹೆಸರುಗಳು)) {
        /* Avoid clang-tidy false positive */
        ಪ್ರತಿಜ್ಞಾ(darray_ಗಾತ್ರ(info->ಗುಂಪು_ಹೆಸರುಗಳು) < ಗುಂಪು_ಗಮ್ಯ_use + 1);
        darray_resize0(info->ಗುಂಪು_ಹೆಸರುಗಳು, ಗುಂಪು_ಗಮ್ಯ_use + 1);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಸ್ಥಿರಾಂಕ xkb_atom_t old_name = darray_ಅಂಶ(info->ಗುಂಪು_ಹೆಸರುಗಳು, ಗುಂಪು_ಗಮ್ಯ_use);
        ಆದರೆ (old_name != XKB_ATOM_NONE && old_name != name) {
            ಸ್ಥಿರಾಂಕ bool replace = (ವಿಲೀನ != ವಿಲೀನ_AUGMENT);
            ಸ್ಥಿರಾಂಕ xkb_atom_t use    = (replace ? name : old_name);
            ಸ್ಥಿರಾಂಕ xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     ಗುಂಪು_ಗಮ್ಯ_use, info->name,
                     xkb_atom_ಪಠ್ಯ(info->ctx, use),
                     xkb_atom_ಪಠ್ಯ(info->ctx, ignore));
            name = use;
        }
    }
    darray_ಅಂಶ(info->ಗುಂಪು_ಹೆಸರುಗಳು, ಗುಂಪು_ಗಮ್ಯ_use) = name;

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elem, *ಕ್ಷೇತ್ರ_2;
    ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;
    bool ret;

    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, stmt->name, &elem, &ಕ್ಷೇತ್ರ_2, &arrayNdx))
        ಹಿಂತಿರುಗಿ false;

    ಆದರೆ (elem && istreq(elem, "key")) {
        ಕೀಲಿ_Info ತಾತ್ಕಾಲಿಕ = {0};
        Init_ಕೀಲಿ_Info(info->ctx, &ತಾತ್ಕಾಲಿಕ);
        /* Do not replace the whole key, only the current field */
        ತಾತ್ಕಾಲಿಕ.ವಿಲೀನ = (ತಾತ್ಕಾಲಿಕ.ವಿಲೀನ == ವಿಲೀನ_REPLACE)
            ? ವಿಲೀನ_OVERRIDE
            : stmt->ವಿಲೀನ;
        ret = ಗಣ_Symbols_ಕ್ಷೇತ್ರ(info, &ತಾತ್ಕಾಲಿಕ, ಕ್ಷೇತ್ರ_2, arrayNdx, &stmt->ಮೌಲ್ಯ) &&
              ವಿಲೀನ_ಕೀಲಿಗಳು(info, &info->default_ಕೀಲಿ, &ತಾತ್ಕಾಲಿಕ, true);
        Clear_ಕೀಲಿ_Info(&ತಾತ್ಕಾಲಿಕ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!elem && (istreq(ಕ್ಷೇತ್ರ_2, "name") ||
                       istreq(ಕ್ಷೇತ್ರ_2, "groupname"))) {
        ret = ಗಣ_ಗುಂಪು_Name(info, arrayNdx, stmt->ಮೌಲ್ಯ, stmt->ವಿಲೀನ);
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!elem && (istreq(ಕ್ಷೇತ್ರ_2, "groupswrap") ||
                       istreq(ಕ್ಷೇತ್ರ_2, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ಕ್ಷೇತ್ರ,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!elem && (istreq(ಕ್ಷೇತ್ರ_2, "groupsclamp") ||
                       istreq(ಕ್ಷೇತ್ರ_2, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ಕ್ಷೇತ್ರ,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!elem && (istreq(ಕ್ಷೇತ್ರ_2, "groupsredirect") ||
                       istreq(ಕ್ಷೇತ್ರ_2, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ಕ್ಷೇತ್ರ,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!elem && istreq(ಕ್ಷೇತ್ರ_2, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ಕ್ಷೇತ್ರ,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (elem) {
        ret = (ಗಣ_Default_Action_ಕ್ಷೇತ್ರ(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, ಕ್ಷೇತ್ರ_2, arrayNdx,
                                     &stmt->ಮೌಲ್ಯ, stmt->ವಿಲೀನ) !=
               PARSER_FATAL_ERROR);
    } ಇಲ್ಲದಿದ್ದರೆ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ಕ್ಷೇತ್ರ,
                "Default defined for unknown field \"%s\"; Ignored\n", ಕ್ಷೇತ್ರ_2);
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, ಕೀಲಿ_Info *keyi)
{
    bool all_valid_entries = true;

    ಗಾಗಿ (; def; def = (VarDef *) def->common.ಮುಂದಿನ) {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕ್ಷೇತ್ರ_2;
        ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;
        bool ok = true;

        ಆದರೆ (!def->name) {
            ಆದರೆ (unlikely(!def->ಮೌಲ್ಯ) || def->ಮೌಲ್ಯ->common.type != STMT_ಅಭಿವ್ಯಕ್ತಿ_ACTION_LIST)
                ಕ್ಷೇತ್ರ_2 = "symbols"; /* Default to symbols field */
            ಇಲ್ಲದಿದ್ದರೆ
                ಕ್ಷೇತ್ರ_2 = "actions";
            arrayNdx = ಖಾಲಿ;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elem;
            ok = ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, def->name, &elem, &ಕ್ಷೇತ್ರ_2,
                                &arrayNdx);
            ಆದರೆ (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, ಕ್ಷೇತ್ರ_2);
                ok = false;
            }
        }

        ಆದರೆ (unlikely(!def->ಮೌಲ್ಯ)) {
            log_err(info->ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", ಕ್ಷೇತ್ರ_2);
            ok = false;
        }

        ಆದರೆ (!ok || !ಗಣ_Symbols_ಕ್ಷೇತ್ರ(info, keyi, ಕ್ಷೇತ್ರ_2, arrayNdx, &def->ಮೌಲ್ಯ))
            all_valid_entries = false;
    }

    ಹಿಂತಿರುಗಿ all_valid_entries;
}

ಸ್ಥಿರ bool
ಗಣ_Explicit_ಗುಂಪು(SymbolsInfo *info, ಕೀಲಿ_Info *keyi)
{
    xkb_layout_ಸೂಚ್ಯಂಕ_t i;
    ಗುಂಪು_Info_2 *groupi;
    bool warn = false;

    ಆದರೆ (info->explicit_ಗುಂಪು == XKB_LAYOUT_INVALID)
        ಹಿಂತಿರುಗಿ true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        ಆದರೆ (groupi->defined) {
            warn = true;
            Clear_ಗುಂಪು_Info(groupi);
            Init_ಗುಂಪು_Info(groupi);
        }
    }

    ಆದರೆ (warn) {
        log_warn(info->ctx, XKB_WARNING_ಅಪವರ್ತ್ಯ_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_ಗುಂಪು + 1, ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_ಗುಂಪು + 1);
    ಆದರೆ (info->explicit_ಗುಂಪು > 0) {
        darray_ಅಂಶ(keyi->groups, info->explicit_ಗುಂಪು) =
            darray_ಅಂಶ(keyi->groups, 0);
        Init_ಗುಂಪು_Info(&darray_ಅಂಶ(keyi->groups, 0));
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    ಕೀಲಿ_Info keyi;

    keyi = info->default_ಕೀಲಿ;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_ಕೀಲಿ.groups);
    ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t i = 0; i < darray_ಗಾತ್ರ(keyi.groups); i++)
        Copy_ಗುಂಪು_Info(&darray_ಅಂಶ(keyi.groups, i),
                      &darray_ಅಂಶ(info->default_ಕೀಲಿ.groups, i));
    keyi.ವಿಲೀನ = stmt->ವಿಲೀನ;
    keyi.name = stmt->ಕೀಲಿ_Name;

    ಆದರೆ (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        ಗಣ_Explicit_ಗುಂಪು(info, &keyi) &&
        ಕೂಡಿಸು_ಕೀಲಿ_Symbols(info, &keyi, true)) {
        ಹಿಂತಿರುಗಿ true;
    }

    Clear_ಕೀಲಿ_Info(&keyi);
    info->error_ಎಣಿಕೆ++;
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    Mod_Map_ನಮೂದು tmp;
    ರಚನೆ xkb_context *ctx = info->ctx;

    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        ಹಿಂತಿರುಗಿ false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    ಪ್ರತಿಜ್ಞಾ(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1 ||
           def->modifiers->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_IDENT);

    bool ok = true;
    tmp.ವಿಲೀನ = def->ವಿಲೀನ;

    ಗಾಗಿ (ಅಭಿವ್ಯಕ್ತಿ_Def *ಕೀಲಿ = def->ಕೀಲಿಗಳು; ಕೀಲಿ; ಕೀಲಿ = (ಅಭಿವ್ಯಕ್ತಿ_Def *) ಕೀಲಿ->common.ಮುಂದಿನ) {
        ಆದರೆ (ಕೀಲಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYNAME_LITERAL) {
            tmp.have_ಚಿಹ್ನೆ = false;
            tmp.u.ಕೀಲಿ_Name = ಕೀಲಿ->ಕೀಲಿ_name.ಕೀಲಿ_name;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->common.type == STMT_ಅಭಿವ್ಯಕ್ತಿ_KEYSYM_LITERAL) {
            ಆದರೆ (ಕೀಲಿ->keysym.keysym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
                /* Invalid keysym: ignore. Error message already printed */
                ಮುಂದುವರಿ;
            } ಇಲ್ಲದಿದ್ದರೆ {
                tmp.have_ಚಿಹ್ನೆ = true;
                tmp.u.ಕೀಲಿ_Sym = ಕೀಲಿ->keysym.keysym;
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ನಮೂದು,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            ಮುಂದುವರಿ;
        }

        ok = ಕೂಡಿಸು_Mod_Map_ನಮೂದು(info, &tmp) && ok;
    }
    ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    ಬಿಡುಗಡೆ(info->name);
    info->name = strdup_ಸುರಕ್ಷಿತ(file->name);

    ಗಾಗಿ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ಮುಂದಿನ) {
        ಆಯ್ಕೆ (stmt->type) {
        ಸ್ಥಿತಿ STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_UNKNOWN_DECLARATION:
        ಸ್ಥಿತಿ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_ಗಮ್ಯ_string(stmt->type));
            ok = false;
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (!ok)
            info->error_ಎಣಿಕೆ++;

        ಆದರೆ (info->error_ಎಣಿಕೆ > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    ಸುರಕ್ಷಿತ_map_name(file->name));
            ನಿಲ್ಲಿಸು;
        }
    }
}

/**
 * Given a keysym @sym, return a key which generates it, or NULL.
 * This is used for example in a modifier map definition, such as:
 *      modifier_map Lock           { Caps_Lock };
 * where we want to add the Lock modifier to the modmap of the key
 * which matches the keysym Caps_Lock.
 * Since there can be many keys which generates the keysym, the key
 * is chosen first by lowest group in which the keysym appears, than
 * by lowest level and than by lowest key code.
 */
ಸ್ಥಿರ ರಚನೆ xkb_ಕೀಲಿ *
ಕಂಡುಹಿಡಿ_ಕೀಲಿ_For_ಚಿಹ್ನೆ(ರಚನೆ xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_ಗುಂಪು;
    xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0;
    ಮಾಡು {
        xkb_level_ಸೂಚ್ಯಂಕ_t level = 0;
        got_one_ಗುಂಪು = false;
        bool got_one_level;
        ಮಾಡು {
            got_one_level = false;
            ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;
            xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
                ಆದರೆ (ಗುಂಪು < ಕೀಲಿ->ಅಂಕೆ_groups &&
                    level < Xkb_ಕೀಲಿ_ಅಂಕೆ_ಮಟ್ಟಗಳು(ಕೀಲಿ, ಗುಂಪು)) {
                    got_one_ಗುಂಪು = got_one_level = true;
                    ಸ್ಥಿರಾಂಕ xkb_keysym_ಎಣಿಕೆ_t ಅಂಕೆ_syms =
                        ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].ಅಂಕೆ_syms;
                    ಆದರೆ (ಅಂಕೆ_syms > 1) {
                        ಗಾಗಿ (xkb_keysym_ಎಣಿಕೆ_t k = 0; k < ಅಂಕೆ_syms; k++) {
                            ಆದರೆ (ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].s.syms[k] == sym)
                                ಹಿಂತಿರುಗಿ ಕೀಲಿ;
                        }
                    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಂಕೆ_syms &&
                               ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].s.sym == sym) {
                        ಹಿಂತಿರುಗಿ ಕೀಲಿ;
                    }
                }
            }
            level++;
        } ಆಗ (got_one_level);
        ಗುಂಪು++;
    } ಆಗ (got_one_ಗುಂಪು);

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/*
 * Find an appropriate type for a group and return its name.
 *
 * Simple recipe:
 * - ONE_LEVEL for width 0/1
 * - ALPHABETIC for 2 shift levels, with lower/upercase keysyms
 * - KEYPAD for keypad keys.
 * - TWO_LEVEL for other 2 shift level keys.
 * and the same for four level keys.
 *
 * FIXME: Decide how to handle multiple-syms-per-level, and do it.
 */
ಸ್ಥಿರ xkb_atom_t
ಕಂಡುಹಿಡಿ_Automatic_Type(ರಚನೆ xkb_context *ctx, ಗುಂಪು_Info_2 *groupi)
{
    xkb_keysym_t sym0, sym1;
    ಸ್ಥಿರಾಂಕ xkb_level_ಸೂಚ್ಯಂಕ_t ಅಗಲ = darray_ಗಾತ್ರ(groupi->ಮಟ್ಟಗಳು);

#ವ್ಯಾಖ್ಯಾನಿಸು GET_SYM(level) \
    (darray_ಅಂಶ(groupi->ಮಟ್ಟಗಳು, level).ಅಂಕೆ_syms == 0 ? \
        XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ : \
     darray_ಅಂಶ(groupi->ಮಟ್ಟಗಳು, level).ಅಂಕೆ_syms == 1 ? \
        darray_ಅಂಶ(groupi->ಮಟ್ಟಗಳು, level).s.sym : \
     /* num_syms > 1 */ \
        darray_ಅಂಶ(groupi->ಮಟ್ಟಗಳು, level).s.syms[0])

    ಆದರೆ (ಅಗಲ == 1 || ಅಗಲ <= 0)
        ಹಿಂತಿರುಗಿ xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    ಆದರೆ (ಅಗಲ == 2) {
        ಆದರೆ (xkb_keysym_is_ಅಧೋ(sym0) && xkb_keysym_is_ಊರ್ಧ್ವ_or_title(sym1))
            ಹಿಂತಿರುಗಿ xkb_atom_intern_literal(ctx, "ALPHABETIC");

        ಆದರೆ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            ಹಿಂತಿರುಗಿ xkb_atom_intern_literal(ctx, "KEYPAD");

        ಹಿಂತಿರುಗಿ xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    ಆದರೆ (ಅಗಲ <= 4) {
        ಆದರೆ (xkb_keysym_is_ಅಧೋ(sym0) && xkb_keysym_is_ಊರ್ಧ್ವ_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (ಅಗಲ == 4 ? GET_SYM(3) : XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ);

            ಆದರೆ (xkb_keysym_is_ಅಧೋ(sym2) && xkb_keysym_is_ಊರ್ಧ್ವ_or_title(sym3))
                ಹಿಂತಿರುಗಿ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            ಹಿಂತಿರುಗಿ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        ಆದರೆ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            ಹಿಂತಿರುಗಿ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        ಹಿಂತಿರುಗಿ xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    ಹಿಂತಿರುಗಿ XKB_ATOM_NONE;

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು GET_SYM
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type *
ಕಂಡುಹಿಡಿ_Type_For_ಗುಂಪು(ರಚನೆ xkb_keymap *keymap, ಕೀಲಿ_Info *keyi,
                 xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು, bool *explicit_type)
{
    ಗುಂಪು_Info_2 *groupi = &darray_ಅಂಶ(keyi->groups, ಗುಂಪು);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    ಆದರೆ (type_name == XKB_ATOM_NONE) {
        ಆದರೆ (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            type_name = ಕಂಡುಹಿಡಿ_Automatic_Type(keymap->ctx, groupi);
            ಆದರೆ (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    ಆದರೆ (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_ಕೀಲಿ_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, keyi->name), ಗುಂಪು + 1,
                 darray_ಗಾತ್ರ(groupi->ಮಟ್ಟಗಳು));
        ಹೋಗು use_default;
    }

    darray_ಗಾತ್ರ_t i;
    ಗಾಗಿ (i = 0; i < keymap->ಅಂಕೆ_types; i++)
        ಆದರೆ (keymap->types[i].name == type_name)
            ನಿಲ್ಲಿಸು;

    ಆದರೆ (i >= keymap->ಅಂಕೆ_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_ಕೀಲಿ_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_ಪಠ್ಯ(keymap->ctx, type_name),
                 ಕೀಲಿ_Name_ಪಠ್ಯ(keymap->ctx, keyi->name), ಗುಂಪು + 1);
        ಹೋಗು use_default;
    }

    keymap->types[i].required = true;
    ಹಿಂತಿರುಗಿ &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    ಹಿಂತಿರುಗಿ &keymap->types[0];
}

ಸ್ಥಿರ bool
Copy_Symbols_Def_ಗಮ್ಯ_Keymap(ರಚನೆ xkb_keymap *keymap, SymbolsInfo *info,
                       ಕೀಲಿ_Info *keyi)
{
    ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;
    ಗುಂಪು_Info_2 *groupi;
    ಸ್ಥಿರಾಂಕ ಗುಂಪು_Info_2 *group0;
    xkb_layout_ಸೂಚ್ಯಂಕ_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    ಕೀಲಿ = Xkb_ಕೀಲಿ_By_Name(keymap, keyi->name, false);
    ಆದರೆ (!ಕೀಲಿ) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi));
        ಹಿಂತಿರುಗಿ false;
    }

    /* Find the range of groups we need. */
    ಕೀಲಿ->ಅಂಕೆ_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ಸ್ಥಿರಾಂಕ bool has_explicit_type = (keyi->defined & ಕೀಲಿ_ಕ್ಷೇತ್ರ_DEFAULT_TYPE)
                                    || (groupi->defined & ಗುಂಪು_ಕ್ಷೇತ್ರ_TYPE);
        ಆದರೆ (darray_ಗಾತ್ರ(groupi->ಮಟ್ಟಗಳು) > 0 || has_explicit_type)
            ಕೀಲಿ->ಅಂಕೆ_groups = i + 1;
        ಆದರೆ (has_explicit_type)
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
    }

    ಆದರೆ (ಕೀಲಿ->ಅಂಕೆ_groups <= 0) {
        /* A key with no group may still have other fields defined */
        ಆದರೆ (keyi->defined)
            ಹೋಗು ಕೀಲಿ_fields;
        ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ false;
    }

    darray_resize(keyi->groups, ಕೀಲಿ->ಅಂಕೆ_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_ಅಂಶ(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        ಆದರೆ (groupi->defined)
            ಮುಂದುವರಿ;

        Copy_ಗುಂಪು_Info(groupi, group0);
    }

    ಕೀಲಿ->groups = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಕೀಲಿ->ಅಂಕೆ_groups, ಗಾತ್ರ(*ಕೀಲಿ->groups));
    ಪ್ರತಿಜ್ಞಾ(darray_ಗಾತ್ರ(keyi->groups) == ಕೀಲಿ->ಅಂಕೆ_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < ಕೀಲಿ->ಅಂಕೆ_groups);
        #ಆದರೆ_ಅಂತ್ಯ
        bool explicit_type = false;
        ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ_type * ಸ್ಥಿರಾಂಕ type =
            ಕಂಡುಹಿಡಿ_Type_For_ಗುಂಪು(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        ಆದರೆ (type->ಅಂಕೆ_ಮಟ್ಟಗಳು < darray_ಗಾತ್ರ(groupi->ಮಟ್ಟಗಳು)) {
            ರಚನೆ xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_ಪಠ್ಯ(keymap->ctx, type->name), type->ಅಂಕೆ_ಮಟ್ಟಗಳು,
                    ಕೀಲಿ_Info_ಪಠ್ಯ(info, keyi),
                    darray_ಗಾತ್ರ(groupi->ಮಟ್ಟಗಳು));

            darray_foreach_from(leveli, groupi->ಮಟ್ಟಗಳು, type->ಅಂಕೆ_ಮಟ್ಟಗಳು)
                clear_level(leveli);
        }
        darray_resize0(groupi->ಮಟ್ಟಗಳು, type->ಅಂಕೆ_ಮಟ್ಟಗಳು);

        ಕೀಲಿ->groups[i].explicit_type = explicit_type;
        ಕೀಲಿ->groups[i].type = type;
    }

    /* Copy levels. */
    darray_enumerate(i, groupi, keyi->groups) {
        /*
         * Compute the capitalization transformation of the keysyms.
         * This is necessary because `xkb_state_key_get_syms()` returns an
         * immutable array and does not use a buffer, so we must store them.
         * We apply only simple capitalization rules, so the keysyms count is
         * unchanged.
         */
        ರಚನೆ xkb_level *leveli;
        darray_foreach(leveli, groupi->ಮಟ್ಟಗಳು) {
            ಆಯ್ಕೆ (leveli->ಅಂಕೆ_syms) {
            ಸ್ಥಿತಿ 0:
                leveli->ಊರ್ಧ್ವ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
                ನಿಲ್ಲಿಸು;
            ಸ್ಥಿತಿ 1:
                leveli->ಊರ್ಧ್ವ = xkb_keysym_ಗಮ್ಯ_ಊರ್ಧ್ವ(leveli->s.sym);
                ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_ಊರ್ಧ್ವ = false;
                ಗಾಗಿ (xkb_keysym_ಎಣಿಕೆ_t k = 0; k < leveli->ಅಂಕೆ_syms; k++) {
                    ಸ್ಥಿರಾಂಕ xkb_keysym_t ಊರ್ಧ್ವ =
                        xkb_keysym_ಗಮ್ಯ_ಊರ್ಧ್ವ(leveli->s.syms[k]);
                    ಆದರೆ (ಊರ್ಧ್ವ != leveli->s.syms[k]) {
                        leveli->has_ಊರ್ಧ್ವ = true;
                        ನಿಲ್ಲಿಸು;
                    }
                }
                ಆದರೆ (leveli->has_ಊರ್ಧ್ವ) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (ಗಾತ್ರ_t) 2 * leveli->ಅಂಕೆ_syms *
                                             ಗಾತ್ರ(*leveli->s.syms));
                    ಆದರೆ (!leveli->s.syms)
                        ಹಿಂತಿರುಗಿ false; /* FIXME: better handling? */
                    ಗಾಗಿ (xkb_keysym_ಎಣಿಕೆ_t k = 0; k < leveli->ಅಂಕೆ_syms; k++) {
                        leveli->s.syms[leveli->ಅಂಕೆ_syms + k] =
                            xkb_keysym_ಗಮ್ಯ_ಊರ್ಧ್ವ(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->ಮಟ್ಟಗಳು, &ಕೀಲಿ->groups[i].ಮಟ್ಟಗಳು, ಖಾಲಿ);
        ಆದರೆ (ಕೀಲಿ->groups[i].type->ಅಂಕೆ_ಮಟ್ಟಗಳು > 1 ||
            ಕೀಲಿ->groups[i].ಮಟ್ಟಗಳು[0].ಅಂಕೆ_syms > 0) {
            ಕೀಲಿ->groups[i].explicit_symbols = true;
            ಕೀಲಿ->explicit |= EXPLICIT_SYMBOLS;
        }
        ಆದರೆ (groupi->defined & ಗುಂಪು_ಕ್ಷೇತ್ರ_ACTS) { // FIXME
            ಕೀಲಿ->groups[i].explicit_actions = true;
            ಕೀಲಿ->explicit |= EXPLICIT_INTERP;
        }
        ಆದರೆ (ಕೀಲಿ->groups[i].explicit_type)
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
    }

    ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_pending_ಗುಂಪು = keyi->ಹೊರಹೋಗು_ದ_range_pending_ಗುಂಪು;
    ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ = keyi->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ;
    ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy = keyi->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy;

ಕೀಲಿ_fields:
    ಆದರೆ (keyi->defined & ಕೀಲಿ_ಕ್ಷೇತ್ರ_VMODMAP) {
        ಕೀಲಿ->vmodmap = keyi->vmodmap;
        ಕೀಲಿ->explicit |= EXPLICIT_VMODMAP;
    }

    ಆದರೆ (keyi->repeat != ಕೀಲಿ_REPEAT_UNDEFINED) {
        ಕೀಲಿ->repeats = (keyi->repeat == ಕೀಲಿ_REPEAT_YES);
        ಕೀಲಿ->explicit |= EXPLICIT_REPEAT;
    }

    ಆದರೆ ((keyi->defined & ಕೀಲಿ_ಕ್ಷೇತ್ರ_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        ಕೀಲಿ->overlays = keyi->overlays;
        ಆದರೆ (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_ಮುಖವಾಡ_t ಉಳಿದ = ಕೀಲಿ->overlays;
            ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_ಕೀಲಿ **overlays_ಕೀಲಿಗಳು = keyi->overlays_ಕೀಲಿಗಳು;
            ಆಗ (ಉಳಿದ) {
                /* isolate lowest set bit */
                ಸ್ಥಿರಾಂಕ xkb_overlay_ಮುಖವಾಡ_t lsb = (xkb_overlay_ಮುಖವಾಡ_t)(
                    ಉಳಿದ &
                    (xkb_overlay_ಮುಖವಾಡ_t)((ಚಿಹ್ನೆರಹಿತ)~ಉಳಿದ + 1u)
                );
                ಉಳಿದ = (ಉಳಿದ & ~lsb);
                ಆದರೆ (*overlays_ಕೀಲಿಗಳು) {
                    overlays_ಕೀಲಿಗಳು++;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    /* drop current null value (overlayN=none) */
                    ಕೀಲಿ->overlays &= ~lsb;
                }
            }

            ಆದರೆ (ಕೀಲಿ->overlays) {
                /* Steal */
                ಕೀಲಿ->overlays_ಕೀಲಿಗಳು = keyi->overlays_ಕೀಲಿಗಳು;
                keyi->overlays_ಕೀಲಿಗಳು = ಖಾಲಿ;
                keyi->overlays_alloc = 0;

                ಕೀಲಿ->overlays_inline = false;
                ಕೀಲಿ->explicit |= EXPLICIT_OVERLAY;
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಕೀಲಿ->overlay_ಕೀಲಿ = keyi->overlay_ಕೀಲಿ;
            ಕೀಲಿ->overlays_inline = true;
            ಕೀಲಿ->explicit |= EXPLICIT_OVERLAY;
        }
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
Copy_Mod_Map_Def_ಗಮ್ಯ_Keymap(ರಚನೆ xkb_keymap *keymap, SymbolsInfo *info,
                      Mod_Map_ನಮೂದು *ನಮೂದು)
{
    ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;

    ಆದರೆ (!ನಮೂದು->have_ಚಿಹ್ನೆ) {
        ಕೀಲಿ = Xkb_ಕೀಲಿ_By_Name(keymap, ನಮೂದು->u.ಕೀಲಿ_Name, true);
        ಆದರೆ (!ಕೀಲಿ) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, ನಮೂದು->u.ಕೀಲಿ_Name),
                    Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_REAL, &info->mods, ನಮೂದು->mods));
            ನಮೂದು->mods = 0; /* disable */
            ಹಿಂತಿರುಗಿ false;
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಕೀಲಿ = ಕಂಡುಹಿಡಿ_ಕೀಲಿ_For_ಚಿಹ್ನೆ(keymap, ನಮೂದು->u.ಕೀಲಿ_Sym);
        ಆದರೆ (!ಕೀಲಿ) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_ಚಿಹ್ನೆ,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    Keysym_ಪಠ್ಯ(info->ctx, ನಮೂದು->u.ಕೀಲಿ_Sym),
                    Mod_ಮುಖವಾಡ_ಪಠ್ಯ(info->ctx, MOD_REAL, &info->mods, ನಮೂದು->mods));
            ನಮೂದು->mods = 0; /* disable */
            ಹಿಂತಿರುಗಿ false;
        }
    }

    /* Skip modMap None */
    ಆದರೆ (ನಮೂದು->mods) {
        ಕೀಲಿ->modmap |= ನಮೂದು->mods;
        ನಮೂದು->ಕೀಲಿ_ಸಂಕೇತ = ಕೀಲಿ->keycode;
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
Copy_Symbols_ಗಮ್ಯ_Keymap(ರಚನೆ xkb_keymap *keymap, SymbolsInfo *info)
{
    ಕೀಲಿ_Info *keyi;

    keymap->symbols_section_name = strdup_ಸುರಕ್ಷಿತ(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು = (xkb_layout_ಸೂಚ್ಯಂಕ_t)darray_ಗಾತ್ರ(info->ಗುಂಪು_ಹೆಸರುಗಳು);
    darray_steal(info->ಗುಂಪು_ಹೆಸರುಗಳು, &keymap->ಗುಂಪು_ಹೆಸರುಗಳು, ಖಾಲಿ);

    darray_foreach(keyi, info->ಕೀಲಿಗಳು)
        ಆದರೆ (!Copy_Symbols_Def_ಗಮ್ಯ_Keymap(keymap, info, keyi))
            info->error_ಎಣಿಕೆ++;

    ಆದರೆ (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;

        xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
            ಆದರೆ (ಕೀಲಿ->name == XKB_ATOM_NONE)
                ಮುಂದುವರಿ;

            ಆದರೆ (ಕೀಲಿ->ಅಂಕೆ_groups < 1)
                log_info(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "No symbols defined for %s\n",
                         ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, ಕೀಲಿ->name));
        }
    }

    /* Resolve modmaps */
    Mod_Map_ನಮೂದು *mm;
    darray_foreach(mm, info->modmaps)
        ಆದರೆ (!Copy_Mod_Map_Def_ಗಮ್ಯ_Keymap(keymap, info, mm))
            info->error_ಎಣಿಕೆ++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->ಅಂಕೆ_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    ಹಿಂತಿರುಗಿ true;
}

bool
CompileSymbols(XkbFile *file, ರಚನೆ xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ಆದರೆ (file !=ಖಾಲಿ)
        HandleSymbolsFile(&info, file);

    ಆದರೆ (info.error_ಎಣಿಕೆ != 0)
        ಹೋಗು err_info;

    ಆದರೆ (!Copy_Symbols_ಗಮ್ಯ_Keymap(&keymap_info->keymap, &info))
        ಹೋಗು err_info;

    ClearSymbolsInfo(&info);
    ಹಿಂತಿರುಗಿ true;

err_info:
    ClearSymbolsInfo(&info);
    ಹಿಂತಿರುಗಿ false;
}
