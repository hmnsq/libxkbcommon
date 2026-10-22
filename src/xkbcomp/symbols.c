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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-keysyms.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "action.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "expr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "include.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keysym.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-numbers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-mem.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "vmod.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/ast.h"

ਗਿਣਤੀ ਕੁੰਜੀ_repeat {
    ਕੁੰਜੀ_REPEAT_UNDEFINED = 0,
    ਕੁੰਜੀ_REPEAT_YES,
    ਕੁੰਜੀ_REPEAT_NO,
    ਕੁੰਜੀ_REPEAT_ਅੰਕ_ENTRIES,
};
ਗਿਣਤੀ { ਕੁੰਜੀ_REPEAT_MIN_ਚੌੜਾਈ = 3 /* 2 bits + sign */ };
static_assert(ਕੁੰਜੀ_REPEAT_ਅੰਕ_ENTRIES - 1 <=
              (1 << (ਕੁੰਜੀ_REPEAT_MIN_ਚੌੜਾਈ - 1)) - 1,
              "Cannot encode key repeat");

ਗਿਣਤੀ ਸਮੂਹ_ਖੇਤਰ {
    ਸਮੂਹ_ਖੇਤਰ_SYMS = (1 << 0),
    ਸਮੂਹ_ਖੇਤਰ_ACTS = (1 << 1),
    ਸਮੂਹ_ਖੇਤਰ_TYPE = (1 << 2),
};

ਗਿਣਤੀ ਕੁੰਜੀ_ਖੇਤਰ {
    ਕੁੰਜੀ_ਖੇਤਰ_NONE = 0,
    ਕੁੰਜੀ_ਖੇਤਰ_REPEAT = (1 << 0),
    ਕੁੰਜੀ_ਖੇਤਰ_DEFAULT_TYPE = (1 << 1),
    ਕੁੰਜੀ_ਖੇਤਰ_GROUPINFO = (1 << 2),
    ਕੁੰਜੀ_ਖੇਤਰ_VMODMAP = (1 << 3),
    ਕੁੰਜੀ_ਖੇਤਰ_OVERLAY = (1 << 4),
    ਕੁੰਜੀ_ਖੇਤਰ_ALL = ਕੁੰਜੀ_ਖੇਤਰ_REPEAT | ਕੁੰਜੀ_ਖੇਤਰ_DEFAULT_TYPE
                  | ਕੁੰਜੀ_ਖੇਤਰ_GROUPINFO | ਕੁੰਜੀ_ਖੇਤਰ_VMODMAP | ਕੁੰਜੀ_ਖੇਤਰ_OVERLAY,
};
ਗਿਣਤੀ { ਕੁੰਜੀ_ਖੇਤਰ_MIN_ਚੌੜਾਈ = 6 /* 5 bits + sign */ };
static_assert(ਕੁੰਜੀ_ਖੇਤਰ_ALL <= (1 << (ਕੁੰਜੀ_ਖੇਤਰ_MIN_ਚੌੜਾਈ - 1)) - 1,
              "Cannot encode key field");

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    darray(ਬਣਤਰ xkb_level) ਪੱਧਰ;
    ਗਿਣਤੀ ਸਮੂਹ_ਖੇਤਰ defined;
    xkb_atom_t type;
} ਸਮੂਹ_Info_2;

ਗਿਣਤੀ {
    XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY_MIN_ਚੌੜਾਈ = 4 /* 3 bits + sign */,
    KEYINFO_ਬਾਹਰਲਾ_ਦਾ_RANGE_ਸਮੂਹ_POLICY_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ = 8,
    KEYINFO_DEFINED_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ = 16,
    KEYINFO_ਮਿਲਾਓ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ = 8,
    KEYINFO_REPEAT_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ = 8,
};
static_assert(
    XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP < XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP &&
    XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP < XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY_MIN_ਚੌੜਾਈ) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_ਬਾਹਰਲਾ_ਦਾ_RANGE_ਸਮੂਹ_POLICY_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ >=
              XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_POLICY_MIN_ਚੌੜਾਈ, "");
static_assert((ਬਿਨਾਨਿਸ਼ਾਨੀ)KEYINFO_DEFINED_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ >=
              (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਕੁੰਜੀ_ਖੇਤਰ_MIN_ਚੌੜਾਈ, "Cannot encode KeyInfo field");
static_assert((ਬਿਨਾਨਿਸ਼ਾਨੀ)KEYINFO_ਮਿਲਾਓ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ >=
              (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਮਿਲਾਓ_ਬਹੁਲਕ_MIN_ਚੌੜਾਈ, "Cannot encode KeyInfo field");
static_assert((ਬਿਨਾਨਿਸ਼ਾਨੀ)KEYINFO_REPEAT_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ >=
              (ਬਿਨਾਨਿਸ਼ਾਨੀ)ਕੁੰਜੀ_REPEAT_MIN_ਚੌੜਾਈ, "Cannot encode KeyInfo field");

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    xkb_atom_t name;

    xkb_mod_ਨਕਾਬ_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_ਸੂਚਕਾਂਕ_t ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ;
    darray(ਸਮੂਹ_Info_2) groups;
    ਗਿਣਤੀ xkb_layout_ਬਾਹਰਲਾ_ਦਾ_range_policy
        ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy:KEYINFO_ਬਾਹਰਲਾ_ਦਾ_RANGE_ਸਮੂਹ_POLICY_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    ਗਿਣਤੀ ਕੁੰਜੀ_ਖੇਤਰ defined:KEYINFO_DEFINED_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ;
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ:KEYINFO_ਮਿਲਾਓ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ;
    ਗਿਣਤੀ ਕੁੰਜੀ_repeat repeat:KEYINFO_REPEAT_ਦੋਆਧਾਰੀ_ਅੰਕ_ਖੇਤਰ;

    bool ਬਾਹਰਲਾ_ਦਾ_range_pending_ਸਮੂਹ:1;

    /** If true, overlays in the mask are explicitly cleared */
    bool overlays_clear:1;
    /** Track the count of allocated slots of overlays_keys */
    xkb_overlay_ਸੂਚਕਾਂਕ_t overlays_alloc;
    /** Overlays indices */
    xkb_overlay_ਨਕਾਬ_t overlays;
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    ਸੰਘ {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *overlay_ਕੁੰਜੀ;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ **overlays_ਕੁੰਜੀਆਂ;
    };
} ਕੁੰਜੀ_Info;

ਸਥਿਰ ਖਾਲੀ
StealLevelInfo(ਬਣਤਰ xkb_level *into, ਬਣਤਰ xkb_level *from)
{
    clear_level(into);
    ਜੇ (from->ਅੰਕ_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } ਨਹੀਂਤਾਂ {
        into->s.sym = from->s.sym;
    }
    into->ਅੰਕ_syms = from->ਅੰਕ_syms;
    from->ਅੰਕ_syms = 0;

    ਜੇ (from->ਅੰਕ_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } ਨਹੀਂਤਾਂ {
        into->a.action = from->a.action;
    }
    into->ਅੰਕ_actions = from->ਅੰਕ_actions;
    from->ਅੰਕ_actions = 0;
}

ਸਥਿਰ ਖਾਲੀ
Init_ਸਮੂਹ_Info(ਸਮੂਹ_Info_2 *groupi)
{
    memset(groupi, 0, ਆਕਾਰ(*groupi));
}

ਸਥਿਰ ਖਾਲੀ
Clear_ਸਮੂਹ_Info(ਸਮੂਹ_Info_2 *groupi)
{
    ਬਣਤਰ xkb_level *leveli;
    darray_foreach(leveli, groupi->ਪੱਧਰ)
        clear_level(leveli);
    darray_free(groupi->ਪੱਧਰ);
}

ਸਥਿਰ ਖਾਲੀ
Copy_ਸਮੂਹ_Info(ਸਮੂਹ_Info_2 *ਮੰਜ਼ਿਲ, ਅਚਲ ਸਮੂਹ_Info_2 *from)
{
    ਮੰਜ਼ਿਲ->defined = from->defined;
    ਮੰਜ਼ਿਲ->type = from->type;
    darray_init(ਮੰਜ਼ਿਲ->ਪੱਧਰ);
    darray_copy(ਮੰਜ਼ਿਲ->ਪੱਧਰ, from->ਪੱਧਰ);
    ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t j = 0; j < darray_ਮਾਪ(ਮੰਜ਼ਿਲ->ਪੱਧਰ); j++) {
        ਜੇ (darray_ਵਸਤੂ(from->ਪੱਧਰ, j).ਅੰਕ_syms > 1) {
            darray_ਵਸਤੂ(ਮੰਜ਼ਿਲ->ਪੱਧਰ, j).s.syms =
                memdup(darray_ਵਸਤੂ(from->ਪੱਧਰ, j).s.syms,
                       darray_ਵਸਤੂ(from->ਪੱਧਰ, j).ਅੰਕ_syms,
                       ਆਕਾਰ(xkb_keysym_t));
        }
        ਜੇ (darray_ਵਸਤੂ(from->ਪੱਧਰ, j).ਅੰਕ_actions > 1) {
            darray_ਵਸਤੂ(ਮੰਜ਼ਿਲ->ਪੱਧਰ, j).a.actions =
                memdup(darray_ਵਸਤੂ(from->ਪੱਧਰ, j).a.actions,
                       darray_ਵਸਤੂ(from->ਪੱਧਰ, j).ਅੰਕ_actions,
                       ਆਕਾਰ(ਸੰਘ xkb_action));
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
Init_ਕੁੰਜੀ_Info(ਬਣਤਰ xkb_context *ctx, ਕੁੰਜੀ_Info *keyi)
{
    memset(keyi, 0, ਆਕਾਰ(*keyi));
    static_assert(!DEFAULT_ਕੁੰਜੀ_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_ਕੁੰਜੀ_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy = XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_ਕੁੰਜੀ = ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ
Clear_ਕੁੰਜੀ_Info(ਕੁੰਜੀ_Info *keyi)
{
    ਸਮੂਹ_Info_2 *groupi;
    darray_foreach(groupi, keyi->groups)
        Clear_ਸਮੂਹ_Info(groupi);
    darray_free(keyi->groups);
    ਜੇ (keyi->overlays_alloc)
        ਮੁਕਤ(keyi->overlays_ਕੁੰਜੀਆਂ);
}

/***====================================================================***/

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਅੱਖਰ *name;         /* e.g. pc+us+inet(evdev) */
    ਪੂਰਨ error_ਗਿਣਤੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ;
    xkb_layout_ਸੂਚਕਾਂਕ_t explicit_ਸਮੂਹ;
    xkb_layout_ਸੂਚਕਾਂਕ_t max_groups;
    darray(ਕੁੰਜੀ_Info) ਕੁੰਜੀਆਂ;
    ਕੁੰਜੀ_Info default_ਕੁੰਜੀ;
    ActionsInfo default_actions;
    darray(xkb_atom_t) ਸਮੂਹ_ਨਾਂ;
    darray(Mod_Map_ਇੰਦਰਾਜ) modmaps;
    ਬਣਤਰ xkb_mod_ਸੈੱਟ mods;

    ਬਣਤਰ xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info;
} SymbolsInfo;

ਸਥਿਰ ਖਾਲੀ
InitSymbolsInfo(SymbolsInfo *info, ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ, ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods)
{
    memset(info, 0, ਆਕਾਰ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_ਡੂੰਘਾਈ = include_ਡੂੰਘਾਈ;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_ਕੁੰਜੀ_Info(keymap_info->keymap.ctx, &info->default_ਕੁੰਜੀ);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_ਡੂੰਘਾਈ > 0);
    info->explicit_ਸਮੂਹ = XKB_LAYOUT_INVALID;
}

ਸਥਿਰ ਖਾਲੀ
ClearSymbolsInfo(SymbolsInfo *info)
{
    ਕੁੰਜੀ_Info *keyi;
    ਮੁਕਤ(info->name);
    darray_foreach(keyi, info->ਕੁੰਜੀਆਂ)
        Clear_ਕੁੰਜੀ_Info(keyi);
    darray_free(info->ਕੁੰਜੀਆਂ);
    darray_free(info->ਸਮੂਹ_ਨਾਂ);
    darray_free(info->modmaps);
    Clear_ਕੁੰਜੀ_Info(&info->default_ਕੁੰਜੀ);
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
ਕੁੰਜੀ_Info_ਪਾਠ(SymbolsInfo *info, ਕੁੰਜੀ_Info *keyi)
{
    ਵਾਪਸ ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, keyi->name);
}

ਸਥਿਰ bool
ਮਿਲਾਓ_Groups(SymbolsInfo *info, ਸਮੂਹ_Info_2 *into, ਸਮੂਹ_Info_2 *from, bool clobber,
            bool report, xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ, xkb_atom_t ਕੁੰਜੀ_name)
{
    /* First find the type of the merged group. */
    ਜੇ (into->type != from->type) {
        ਜੇ (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        ਨਹੀਂਤਾਂ ਜੇ (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        ਨਹੀਂਤਾਂ {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            ਜੇ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         ਸਮੂਹ + 1, ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, ਕੁੰਜੀ_name),
                         xkb_atom_ਪਾਠ(info->ctx, use),
                         xkb_atom_ਪਾਠ(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & ਸਮੂਹ_ਖੇਤਰ_TYPE);

    /* Now look at the levels. */

    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(from->ਪੱਧਰ)) {
        Init_ਸਮੂਹ_Info(from);
        ਵਾਪਸ true;
    }

    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(into->ਪੱਧਰ)) {
        from->type = into->type;
        *into = *from;
        Init_ਸਮੂਹ_Info(from);
        ਵਾਪਸ true;
    }

    /* Merge the actions and syms. */
    ਅਚਲ darray_ਮਾਪ_t ਪੱਧਰ_ਵਿੱਚ_both =
        MIN(darray_ਮਾਪ(into->ਪੱਧਰ), darray_ਮਾਪ(from->ਪੱਧਰ));
    darray_ਮਾਪ_t from_Keysyms_ਗਿਣਤੀ = 0;
    darray_ਮਾਪ_t from_Actions_ਗਿਣਤੀ = 0;
    ਲਈ (darray_ਮਾਪ_t i = 0; i < ਪੱਧਰ_ਵਿੱਚ_both; i++) {
        ਬਣਤਰ xkb_level* ਅਚਲ intoLevel = &darray_ਵਸਤੂ(into->ਪੱਧਰ, i);
        ਬਣਤਰ xkb_level* ਅਚਲ fromLevel = &darray_ਵਸਤੂ(from->ਪੱਧਰ, i);

        ਅਚਲ bool fromHasNoKeysym = fromLevel->ਅੰਕ_syms == 0;
        ਅਚਲ bool fromHasNoAction = fromLevel->ਅੰਕ_actions == 0;
        ਜੇ (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            ਜਾਰੀ;
        }

        ਅਚਲ bool intoHasNoKeysym = intoLevel->ਅੰਕ_syms == 0;
        ਅਚਲ bool intoHasNoAction = intoLevel->ਅੰਕ_actions == 0;
        ਜੇ (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_ਗਿਣਤੀ++;
            from_Actions_ਗਿਣਤੀ++;
            ਜਾਰੀ;
        }

        /* Possible level conflict */
        ਪ੍ਰਤਿਜ੍ਞਾ(intoLevel->ਅੰਕ_syms > 0 || intoLevel->ਅੰਕ_actions > 0);
        ਪ੍ਰਤਿਜ੍ਞਾ(fromLevel->ਅੰਕ_syms > 0 || fromLevel->ਅੰਕ_actions > 0);

        /* Handle keysyms */
        ਜੇ (!Xkb_ਪੱਧਰ_Same_Syms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            ਜੇ (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_ਚਿੰਨ੍ਹ,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, ਸਮੂਹ + 1, ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, ਕੁੰਜੀ_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            ਜੇ (fromHasNoKeysym) {
                /* No keysym to copy */
            } ਨਹੀਂਤਾਂ ਜੇ (clobber) {
                /* Override: copy any defined keysym from `from` */
                ਜੇ (unlikely(fromLevel->ਅੰਕ_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    ਜੇ (unlikely(intoLevel->ਅੰਕ_syms > 1))
                        ਮੁਕਤ(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->ਅੰਕ_syms = fromLevel->ਅੰਕ_syms;
                    fromLevel->ਅੰਕ_syms = 0;
                    from_Keysyms_ਗਿਣਤੀ++;
                } ਨਹੀਂਤਾਂ ਜੇ (fromLevel->s.sym != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
                    /* Single defined keysym */
                    ਜੇ (unlikely(intoLevel->ਅੰਕ_syms > 1))
                        ਮੁਕਤ(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->ਅੰਕ_syms = 1;
                    from_Keysyms_ਗਿਣਤੀ++;
                }
            } ਨਹੀਂਤਾਂ {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                ਜੇ (unlikely(intoLevel->ਅੰਕ_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ਨਹੀਂਤਾਂ ਜੇ (intoLevel->s.sym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
                    /* Single undefined keysym */
                    ਜੇ (unlikely(fromLevel->ਅੰਕ_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->ਅੰਕ_syms = fromLevel->ਅੰਕ_syms;
                        fromLevel->ਅੰਕ_syms = 0;
                    } ਨਹੀਂਤਾਂ {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->ਅੰਕ_syms = 1;
                    }
                    from_Keysyms_ਗਿਣਤੀ++;
                }
            }
        }

        /* Handle actions */
        ਜੇ (!Xkb_ਪੱਧਰ_Same_Actions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            ਜੇ (report && !(intoHasNoAction || fromHasNoAction)) {
                ਜੇ (unlikely(intoLevel->ਅੰਕ_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, ਸਮੂਹ + 1, ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, ਕੁੰਜੀ_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } ਨਹੀਂਤਾਂ {
                    ਸੰਘ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, ਸਮੂਹ + 1,
                                ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, ਕੁੰਜੀ_name),
                                Action_Type_ਪਾਠ(use->type),
                                Action_Type_ਪਾਠ(ignore->type));
                }
            }
            ਜੇ (fromHasNoAction) {
                /* No action to copy */
            } ਨਹੀਂਤਾਂ ਜੇ (clobber) {
                /* Override: copy any defined action from `from` */
                ਜੇ (unlikely(fromLevel->ਅੰਕ_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    ਜੇ (unlikely(intoLevel->ਅੰਕ_actions > 1))
                        ਮੁਕਤ(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->ਅੰਕ_actions = fromLevel->ਅੰਕ_actions;
                    fromLevel->ਅੰਕ_actions = 0;
                    from_Actions_ਗਿਣਤੀ++;
                } ਨਹੀਂਤਾਂ ਜੇ (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    ਜੇ (unlikely(intoLevel->ਅੰਕ_actions > 1))
                        ਮੁਕਤ(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->ਅੰਕ_actions = 1;
                    from_Actions_ਗਿਣਤੀ++;
                }
            } ਨਹੀਂਤਾਂ {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                ਜੇ (unlikely(intoLevel->ਅੰਕ_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } ਨਹੀਂਤਾਂ ਜੇ (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    ਜੇ (unlikely(fromLevel->ਅੰਕ_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->ਅੰਕ_actions = fromLevel->ਅੰਕ_actions;
                        fromLevel->ਅੰਕ_actions = 0;
                    } ਨਹੀਂਤਾਂ {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->ਅੰਕ_actions = 1;
                    }
                    from_Actions_ਗਿਣਤੀ++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    ਬਣਤਰ xkb_level *level;
    darray_foreach_from(level, from->ਪੱਧਰ, ਪੱਧਰ_ਵਿੱਚ_both) {
        darray_append(into->ਪੱਧਰ, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->ਅੰਕ_syms = 0;
        level->ਅੰਕ_actions = 0;
        from_Keysyms_ਗਿਣਤੀ++;
        from_Actions_ਗਿਣਤੀ++;
    }
    ਜੇ (from_Keysyms_ਗਿਣਤੀ) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        ਜੇ (from_Keysyms_ਗਿਣਤੀ == darray_ਮਾਪ(into->ਪੱਧਰ))
            into->defined &= ~ਸਮੂਹ_ਖੇਤਰ_SYMS;
        into->defined |= (from->defined & ਸਮੂਹ_ਖੇਤਰ_SYMS);
    }
    ਜੇ (from_Actions_ਗਿਣਤੀ) {
        /* Reset defined actions field if we used no action from `into` */
        ਜੇ (from_Actions_ਗਿਣਤੀ == darray_ਮਾਪ(into->ਪੱਧਰ))
            into->defined &= ~ਸਮੂਹ_ਖੇਤਰ_ACTS;
        into->defined |= (from->defined & ਸਮੂਹ_ਖੇਤਰ_ACTS);
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
Use_New_ਕੁੰਜੀ_ਖੇਤਰ(ਗਿਣਤੀ ਕੁੰਜੀ_ਖੇਤਰ ਖੇਤਰ_2, ਗਿਣਤੀ ਕੁੰਜੀ_ਖੇਤਰ old, ਗਿਣਤੀ ਕੁੰਜੀ_ਖੇਤਰ new,
               bool clobber, bool report, ਗਿਣਤੀ ਕੁੰਜੀ_ਖੇਤਰ *collide)
{
    ਜੇ (!(old & ਖੇਤਰ_2))
        ਵਾਪਸ (new & ਖੇਤਰ_2);

    ਜੇ (new & ਖੇਤਰ_2) {
        ਜੇ (report)
            *collide |= ਖੇਤਰ_2;

        ਵਾਪਸ clobber;
    }

    ਵਾਪਸ false;
}

ਸਥਿਰ bool
overlays_get(ਅਚਲ ਕੁੰਜੀ_Info *info, xkb_overlay_ਸੂਚਕਾਂਕ_t ਦੋਆਧਾਰੀ_ਅੰਕ,
             ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ **ਕੁੰਜੀ_ਬਾਹਰਲਾ)
{
    ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ >= (xkb_overlay_ਸੂਚਕਾਂਕ_t)(ਆਕਾਰ(xkb_overlay_ਨਕਾਬ_t) * CHAR_BIT))
        ਵਾਪਸ false;
    ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨਕਾਬ = (xkb_overlay_ਨਕਾਬ_t)(1u << ਦੋਆਧਾਰੀ_ਅੰਕ);
    ਜੇ (!(info->overlays & ਨਕਾਬ)) ਵਾਪਸ false;
    ਜੇ (ਕੁੰਜੀ_ਬਾਹਰਲਾ) {
        ਜੇ (!info->overlays_alloc) {
            /* Stored inline */
            *ਕੁੰਜੀ_ਬਾਹਰਲਾ = info->overlay_ਕੁੰਜੀ;
        } ਨਹੀਂਤਾਂ {
            /* Stored in sparse array */
            ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨੀਵਾਂ = (xkb_overlay_ਨਕਾਬ_t)(
                info->overlays & (ਨਕਾਬ - 1u)
            );
            ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t ਸੂਚਕਾਂਕ = (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(ਨੀਵਾਂ);
            *ਕੁੰਜੀ_ਬਾਹਰਲਾ = info->overlays_ਕੁੰਜੀਆਂ[ਸੂਚਕਾਂਕ];
        }
    }
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
overlays_ਪਾਓ(ਕੁੰਜੀ_Info *keyi, xkb_overlay_ਸੂਚਕਾਂਕ_t ਦੋਆਧਾਰੀ_ਅੰਕ,
                ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ)
{
    ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ >= (xkb_overlay_ਸੂਚਕਾਂਕ_t)(ਆਕਾਰ(xkb_overlay_ਨਕਾਬ_t) * CHAR_BIT))
        ਵਾਪਸ false;

    ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨਕਾਬ = (xkb_overlay_ਨਕਾਬ_t)(1u << ਦੋਆਧਾਰੀ_ਅੰਕ);

    ਜੇ ((keyi->overlays & ਨਕਾਬ) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        ਜੇ (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_ਕੁੰਜੀ = ਕੁੰਜੀ;
            ਜੇ (!ਕੁੰਜੀ)
                keyi->overlays_clear = true;
        } ਨਹੀਂਤਾਂ {
            /* Stored in sparse array */
            ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨੀਵਾਂ = (xkb_overlay_ਨਕਾਬ_t)(
                keyi->overlays & (xkb_overlay_ਨਕਾਬ_t)(ਨਕਾਬ - 1u)
            );
            /* index = number of set bits strictly below bit */
            ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t ਸੂਚਕਾਂਕ =
                (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(ਨੀਵਾਂ);
            keyi->overlays_ਕੁੰਜੀਆਂ[ਸੂਚਕਾਂਕ] = ਕੁੰਜੀ;
        }
        ਵਾਪਸ true;
    }

    /* Insert new element */
    ਜੇ (!keyi->overlays || (keyi->overlays_clear && !ਕੁੰਜੀ)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_ਕੁੰਜੀ = ਕੁੰਜੀ;
        keyi->overlays |= ਨਕਾਬ;
        keyi->overlays_clear = !ਕੁੰਜੀ;
    } ਨਹੀਂਤਾਂ ਜੇ (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ਅਚਲ xkb_overlay_ਨਕਾਬ_t overlays = keyi->overlays | ਨਕਾਬ;
        ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t alloc =
            (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(overlays);

        /* Alloc sparse array */
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ ** ਅਚਲ tmp = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(alloc, ਆਕਾਰ(*tmp));
        ਜੇ (!tmp) ਵਾਪਸ false;

        /* Copy previous entries to the buffer */
        ਜੇ (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨੀਵਾਂ = (xkb_overlay_ਨਕਾਬ_t)(
                overlays & (xkb_overlay_ਨਕਾਬ_t)(keyi->overlays - 1u)
            );
            ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t idx = (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(ਨੀਵਾਂ);
            tmp[idx] = keyi->overlay_ਕੁੰਜੀ;
        } ਨਹੀਂਤਾਂ {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨੀਵਾਂ = (xkb_overlay_ਨਕਾਬ_t)(
            overlays & (xkb_overlay_ਨਕਾਬ_t)(ਨਕਾਬ - 1u)
        );
        ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t idx = (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(ਨੀਵਾਂ);
        tmp[idx] = ਕੁੰਜੀ;

        keyi->overlays_ਕੁੰਜੀਆਂ = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } ਨਹੀਂਤਾਂ {
        /* Allocated items: grow heap and shift if relevant */
        ਅਚਲ xkb_overlay_ਨਕਾਬ_t overlays =
            (xkb_overlay_ਨਕਾਬ_t)(keyi->overlays | ਨਕਾਬ);
        ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t ਗਿਣਤੀ_2 =
            (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(overlays);
        ਜੇ (ਗਿਣਤੀ_2 > keyi->overlays_alloc) {
            ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t alloc =
                (xkb_overlay_ਸੂਚਕਾਂਕ_t)ਅਗਲਾ_pow2(ਗਿਣਤੀ_2);
            ਪ੍ਰਤਿਜ੍ਞਾ(alloc >= ਗਿਣਤੀ_2);
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ ** ਅਚਲ tmp = realloc(
                keyi->overlays_ਕੁੰਜੀਆਂ,
                (ਮਾਪ_t)alloc * ਆਕਾਰ(*keyi->overlays_ਕੁੰਜੀਆਂ)
            );
            ਜੇ (!tmp) ਵਾਪਸ false;
            keyi->overlays_ਕੁੰਜੀਆਂ = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨੀਵਾਂ = (xkb_overlay_ਨਕਾਬ_t)(
            overlays & (xkb_overlay_ਨਕਾਬ_t)(ਨਕਾਬ - 1u)
        );
        ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t ਸੂਚਕਾਂਕ = (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(ਨੀਵਾਂ);
        ਜੇ (ਸੂਚਕਾਂਕ >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        ਜੇ (ਸੂਚਕਾਂਕ < ਗਿਣਤੀ_2 - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            ਪ੍ਰਤਿਜ੍ਞਾ(ਸੂਚਕਾਂਕ + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_ਕੁੰਜੀਆਂ + ਸੂਚਕਾਂਕ + 1, keyi->overlays_ਕੁੰਜੀਆਂ + ਸੂਚਕਾਂਕ,
                    (ਮਾਪ_t)(ਗਿਣਤੀ_2 - 1u - ਸੂਚਕਾਂਕ) * ਆਕਾਰ(ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *));
        }
        keyi->overlays_ਕੁੰਜੀਆਂ[ਸੂਚਕਾਂਕ] = ਕੁੰਜੀ;
        keyi->overlays = overlays;
    }

    ਵਾਪਸ true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
ਸਥਿਰ bool
ਮਿਲਾਓ_overlays(SymbolsInfo *info, ਕੁੰਜੀ_Info *into, ਕੁੰਜੀ_Info *from,
               bool clobber, bool report, ਗਿਣਤੀ ਕੁੰਜੀ_ਖੇਤਰ *collide)
{
    ਜੇ (!(from->defined & ਕੁੰਜੀ_ਖੇਤਰ_OVERLAY)) {
        /* `from` is empty: do nothing */
    } ਨਹੀਂਤਾਂ ਜੇ (!(into->defined & ਕੁੰਜੀ_ਖੇਤਰ_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_ਕੁੰਜੀਆਂ = from->overlays_ਕੁੰਜੀਆਂ;
        from->overlays_ਕੁੰਜੀਆਂ = ਨਿਰਾ;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= ਕੁੰਜੀ_ਖੇਤਰ_OVERLAY;
    } ਨਹੀਂਤਾਂ ਜੇ (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } ਨਹੀਂਤਾਂ {
        ਜੇ (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨਤੀਜਾ_ਨਕਾਬ =
                (into->overlays | from->overlays);
            ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t ਗਿਣਤੀ_2 =
                (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(ਨਤੀਜਾ_ਨਕਾਬ);

            ਜੇ (ਗਿਣਤੀ_2 == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            ਕੁੰਜੀ_Info *dest = into;
            ਕੁੰਜੀ_Info *src = from;
            ਜੇ (into->overlays_alloc >= ਗਿਣਤੀ_2 &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } ਨਹੀਂਤਾਂ ਜੇ (from->overlays_alloc >= ਗਿਣਤੀ_2) {
                dest = from;
                src = into;
                clobber = !clobber;
            } ਨਹੀਂਤਾਂ ਜੇ (ਗਿਣਤੀ_2 > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                ਜੇ (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                ਜੇ (!dest->overlays_alloc) {
                    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ **tmp = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਗਿਣਤੀ_2, ਆਕਾਰ(*tmp));
                    ਜੇ (!tmp)
                        ਵਾਪਸ false;
                    ਜੇ (dest->overlay_ਕੁੰਜੀ)
                        tmp[0] = dest->overlay_ਕੁੰਜੀ;
                    dest->overlays_ਕੁੰਜੀਆਂ = tmp;
                    dest->overlays_alloc = ਗਿਣਤੀ_2;
                } ਨਹੀਂਤਾਂ {
                    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ **tmp =
                        realloc(dest->overlays_ਕੁੰਜੀਆਂ,
                                ਗਿਣਤੀ_2 * ਆਕਾਰ(*dest->overlays_ਕੁੰਜੀਆਂ));
                    ਜੇ (!tmp)
                        ਵਾਪਸ false;
                    dest->overlays_ਕੁੰਜੀਆਂ = tmp;
                    dest->overlays_alloc = ਗਿਣਤੀ_2;
                }
            }

            xkb_overlay_ਨਕਾਬ_t ਬਚਿਆ = src->overlays;
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ **src_ਕੁੰਜੀਆਂ = (src->overlays_clear)
                ? ਨਿਰਾ
                : (src->overlays_alloc)
                    ? src->overlays_ਕੁੰਜੀਆਂ
                    : &src->overlay_ਕੁੰਜੀ;
            ਜਦੋਂ (ਬਚਿਆ) {
                /* isolate lowest set bit */
                ਅਚਲ xkb_overlay_ਨਕਾਬ_t lsb = (xkb_overlay_ਨਕਾਬ_t)(
                    ਬਚਿਆ &
                    (xkb_overlay_ਨਕਾਬ_t)((ਬਿਨਾਨਿਸ਼ਾਨੀ)~ਬਚਿਆ + 1u)
                );
                /* get its index */
                ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t ਦੋਆਧਾਰੀ_ਅੰਕ =
                    (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(lsb - 1u);
                ਬਚਿਆ = (ਬਚਿਆ & ~lsb);

                /* pop current value */
                ਜੇ (!src->overlays_clear && !src->overlays_alloc && ਬਚਿਆ)
                    PANIC_UNREACHABLE();
                ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *src_ਕੁੰਜੀ = (src_ਕੁੰਜੀਆਂ)
                    ? *(src_ਕੁੰਜੀਆਂ++)
                    : ਨਿਰਾ;

                /* check dest */
                ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *dest_ਕੁੰਜੀ = ਨਿਰਾ;
                ਅਚਲ bool conflict = overlays_get(dest, ਦੋਆਧਾਰੀ_ਅੰਕ, &dest_ਕੁੰਜੀ);
                ਜੇ (conflict) {
                    ਜੇ (dest_ਕੁੰਜੀ == src_ਕੁੰਜੀ)
                       ਜਾਰੀ;
                    ਨਹੀਂਤਾਂ ਜੇ (report)
                        *collide |= ਕੁੰਜੀ_ਖੇਤਰ_OVERLAY;
                }

                ਜੇ ((!conflict || clobber) && !overlays_ਪਾਓ(dest, ਦੋਆਧਾਰੀ_ਅੰਕ, src_ਕੁੰਜੀ))
                    ਵਾਪਸ false;
            }

            ਜੇ (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                ਜੇ (into->overlays_alloc)
                    ਮੁਕਤ(into->overlays_ਕੁੰਜੀਆਂ);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                ਜੇ (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_ਕੁੰਜੀਆਂ = from->overlays_ਕੁੰਜੀਆਂ;
                    from->overlays_ਕੁੰਜੀਆਂ = ਨਿਰਾ;
                    from->overlays_alloc = 0;
                } ਨਹੀਂਤਾਂ {
                    into->overlay_ਕੁੰਜੀ = from->overlay_ਕੁੰਜੀ;
                }
            }
        } ਨਹੀਂਤਾਂ {
            /* Disjoint overlays */

            ਜੇ (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_ਕੁੰਜੀ == from->overlay_ਕੁੰਜੀ) {
                /* Indentical: do nothing */
                ਵਾਪਸ true;
            }
            ਜੇ (!(into->overlays & from->overlays)) {
                ਜੇ (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_ਕੁੰਜੀ = from->overlay_ਕੁੰਜੀ;
                    ਵਾਪਸ true;
                } ਨਹੀਂਤਾਂ ਜੇ (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    ਵਾਪਸ true;
                }
            }
            /* Conflict */
            ਜੇ (report)
                *collide |= ਕੁੰਜੀ_ਖੇਤਰ_OVERLAY;
            ਜੇ (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_ਕੁੰਜੀ = from->overlay_ਕੁੰਜੀ;
            }
        }
    }
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਮਿਲਾਓ_ਕੁੰਜੀਆਂ(SymbolsInfo *info, ਕੁੰਜੀ_Info *into, ਕੁੰਜੀ_Info *from, bool same_file)
{
    xkb_layout_ਸੂਚਕਾਂਕ_t i;
    xkb_layout_ਸੂਚਕਾਂਕ_t groups_ਵਿੱਚ_both;
    ਗਿਣਤੀ ਕੁੰਜੀ_ਖੇਤਰ collide = 0;
    ਅਚਲ ਪੂਰਨ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ਅਚਲ bool clobber = (from->ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);
    ਅਚਲ bool report = (same_file && verbosity > 0) || verbosity > 9;

    ਜੇ (from->ਮਿਲਾਓ == ਮਿਲਾਓ_REPLACE) {
        Clear_ਕੁੰਜੀ_Info(into);
        *into = *from;
        Init_ਕੁੰਜੀ_Info(info->ctx, from);
        ਵਾਪਸ true;
    }

    groups_ਵਿੱਚ_both = MIN(darray_ਮਾਪ(into->groups), darray_ਮਾਪ(from->groups));
    ਲਈ (i = 0; i < groups_ਵਿੱਚ_both; i++)
        ਮਿਲਾਓ_Groups(info,
                    &darray_ਵਸਤੂ(into->groups, i),
                    &darray_ਵਸਤੂ(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    ਲਈ (i = groups_ਵਿੱਚ_both; i < darray_ਮਾਪ(from->groups); i++) {
        darray_append(into->groups, darray_ਵਸਤੂ(from->groups, i));
        Init_ਸਮੂਹ_Info(&darray_ਵਸਤੂ(from->groups, i));
    }

    ਜੇ (Use_New_ਕੁੰਜੀ_ਖੇਤਰ(ਕੁੰਜੀ_ਖੇਤਰ_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= ਕੁੰਜੀ_ਖੇਤਰ_VMODMAP;
    }
    ਜੇ (Use_New_ਕੁੰਜੀ_ਖੇਤਰ(ਕੁੰਜੀ_ਖੇਤਰ_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= ਕੁੰਜੀ_ਖੇਤਰ_REPEAT;
    }
    ਜੇ (Use_New_ਕੁੰਜੀ_ਖੇਤਰ(ਕੁੰਜੀ_ਖੇਤਰ_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= ਕੁੰਜੀ_ਖੇਤਰ_DEFAULT_TYPE;
    }
    ਜੇ (Use_New_ਕੁੰਜੀ_ਖੇਤਰ(ਕੁੰਜੀ_ਖੇਤਰ_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->ਬਾਹਰਲਾ_ਦਾ_range_pending_ਸਮੂਹ = from->ਬਾਹਰਲਾ_ਦਾ_range_pending_ਸਮੂਹ;
        into->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy = from->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy;
        into->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ = from->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ;
        into->defined |= ਕੁੰਜੀ_ਖੇਤਰ_GROUPINFO;
    }

    ਜੇ (!ਮਿਲਾਓ_overlays(info, into, from, clobber, report, &collide))
        ਵਾਪਸ false;

    ਜੇ (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_ਕੁੰਜੀ_Info(from);
    Init_ਕੁੰਜੀ_Info(info->ctx, from);
    ਵਾਪਸ true;
}

ਸਥਿਰ xkb_atom_t
Xkb_Resolve_ਕੁੰਜੀ_Alias(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, xkb_atom_t name)
{
    ਜੇ (name < keymap->ਅੰਕ_ਕੁੰਜੀ_ਨਾਂ) {
        ਅਚਲ Keycode_ਮੇਲ ਮੇਲ = keymap->ਕੁੰਜੀ_ਨਾਂ[name];
        ਜੇ (ਮੇਲ.ਮਿਲਿਆ && ਮੇਲ.is_alias) {
            ਵਾਪਸ ਮੇਲ.alias.real;
        }
    }
    ਵਾਪਸ name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
ਸਥਿਰ bool
ਜੋੜੋ_ਕੁੰਜੀ_Symbols(SymbolsInfo *info, ਕੁੰਜੀ_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_ਕੁੰਜੀ_Alias(&info->keymap_info->keymap, keyi->name);

    ਕੁੰਜੀ_Info *iter;
    darray_foreach(iter, info->ਕੁੰਜੀਆਂ)
        ਜੇ (iter->name == keyi->name)
            ਵਾਪਸ ਮਿਲਾਓ_ਕੁੰਜੀਆਂ(info, iter, keyi, same_file);

    darray_append(info->ਕੁੰਜੀਆਂ, *keyi);
    Init_ਕੁੰਜੀ_Info(info->ctx, keyi);
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਜੋੜੋ_Mod_Map_ਇੰਦਰਾਜ(SymbolsInfo *info, ਅਚਲ Mod_Map_ਇੰਦਰਾਜ *new)
{
    ਅਚਲ bool clobber = (new->ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);

    Mod_Map_ਇੰਦਰਾਜ *old;
    darray_foreach(old, info->modmaps) {
        ਜੇ ((new->have_ਚਿੰਨ੍ਹ != old->have_ਚਿੰਨ੍ਹ) ||
            (new->have_ਚਿੰਨ੍ਹ && new->u.ਕੁੰਜੀ_Sym != old->u.ਕੁੰਜੀ_Sym) ||
            (!new->have_ਚਿੰਨ੍ਹ && new->u.ਕੁੰਜੀ_Name != old->u.ਕੁੰਜੀ_Name))
            ਜਾਰੀ;

        ਜੇ (new->mods == old->mods)
            ਵਾਪਸ true;

        ਅਚਲ xkb_mod_ਨਕਾਬ_t use = (clobber ? new->mods : old->mods);
        ਅਚਲ xkb_mod_ਨਕਾਬ_t ignore = (clobber ? old->mods : new->mods);

        ਜੇ (new->have_ਚਿੰਨ੍ਹ) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    Keysym_ਪਾਠ(info->ctx, new->u.ਕੁੰਜੀ_Sym),
                    Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_REAL, &info->mods, ignore));
        } ਨਹੀਂਤਾਂ {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, new->u.ਕੁੰਜੀ_Name),
                    Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        ਵਾਪਸ true;
    }

    darray_append(info->modmaps, *new);

    ਵਾਪਸ true;
}

/***====================================================================***/

ਸਥਿਰ ਖਾਲੀ
ਮਿਲਾਓ_Included_Symbols(SymbolsInfo *into, SymbolsInfo *from,
                     ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ)
{
    xkb_atom_t *ਸਮੂਹ_name;
    xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ_ਨਾਂ_ਵਿੱਚ_both;

    ਜੇ (from->error_ਗਿਣਤੀ > 0) {
        into->error_ਗਿਣਤੀ += from->error_ਗਿਣਤੀ;
        ਵਾਪਸ;
    }

    ਮਿਲਾਓ_Mod_Sets(into->ctx, &into->mods, &from->mods, ਮਿਲਾਓ);

    ਜੇ (into->name == ਨਿਰਾ) {
        into->name = steal(&from->name);
    }

    ਸਮੂਹ_ਨਾਂ_ਵਿੱਚ_both = MIN(darray_ਮਾਪ(into->ਸਮੂਹ_ਨਾਂ),
                              darray_ਮਾਪ(from->ਸਮੂਹ_ਨਾਂ));
    ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t i = 0; i < ਸਮੂਹ_ਨਾਂ_ਵਿੱਚ_both; i++) {
        ਜੇ (!darray_ਵਸਤੂ(from->ਸਮੂਹ_ਨਾਂ, i))
            ਜਾਰੀ;

        ਜੇ (ਮਿਲਾਓ == ਮਿਲਾਓ_AUGMENT && darray_ਵਸਤੂ(into->ਸਮੂਹ_ਨਾਂ, i))
            ਜਾਰੀ;

        darray_ਵਸਤੂ(into->ਸਮੂਹ_ਨਾਂ, i) = darray_ਵਸਤੂ(from->ਸਮੂਹ_ਨਾਂ, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(ਸਮੂਹ_name, from->ਸਮੂਹ_ਨਾਂ, ਸਮੂਹ_ਨਾਂ_ਵਿੱਚ_both)
        darray_append(into->ਸਮੂਹ_ਨਾਂ, *ਸਮੂਹ_name);

    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(into->ਕੁੰਜੀਆਂ)) {
        into->ਕੁੰਜੀਆਂ = from->ਕੁੰਜੀਆਂ;
        darray_init(from->ਕੁੰਜੀਆਂ);
    }
    ਨਹੀਂਤਾਂ {
        ਕੁੰਜੀ_Info *keyi;
        darray_foreach(keyi, from->ਕੁੰਜੀਆਂ) {
            keyi->ਮਿਲਾਓ = ਮਿਲਾਓ;
            ਜੇ (!ਜੋੜੋ_ਕੁੰਜੀ_Symbols(into, keyi, false))
                into->error_ਗਿਣਤੀ++;
        }
    }

    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    ਨਹੀਂਤਾਂ {
        Mod_Map_ਇੰਦਰਾਜ *mm;
        darray_foreach(mm, from->modmaps) {
            mm->ਮਿਲਾਓ = ਮਿਲਾਓ;
            ਜੇ (!ਜੋੜੋ_Mod_Map_ਇੰਦਰਾਜ(into, mm))
                into->error_ਗਿਣਤੀ++;
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

ਸਥਿਰ bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    ਜੇ (Exceeds_Include_Max_ਡੂੰਘਾਈ(info->ctx, info->include_ਡੂੰਘਾਈ)) {
        info->error_ਗਿਣਤੀ += 10;
        ਵਾਪਸ false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_ਡੂੰਘਾਈ + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ਅਚਲ ਬਣਤਰ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ਲਈ (IncludeStmt *stmt = include; stmt; stmt = stmt->ਅਗਲਾ_incl) {
        SymbolsInfo ਅਗਲਾ_incl;
        XkbFile *file;

        ਅੱਖਰ ਰਸਤਾ[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  ਰਸਤਾ, ਆਕਾਰ(ਰਸਤਾ));
        ਜੇ (!file) {
            info->error_ਗਿਣਤੀ += 10;
            ClearSymbolsInfo(&included);
            ਵਾਪਸ false;
        }

        InitSymbolsInfo(&ਅਗਲਾ_incl, info->keymap_info, info->include_ਡੂੰਘਾਈ + 1,
                        &included.mods);
        ਜੇ (stmt->modifier) {
            ਅਚਲ ਪੂਰਨ ਗਿਣਤੀ_2 = parse_dec_ਮੰਜ਼ਿਲ_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &ਅਗਲਾ_incl.explicit_ਸਮੂਹ);
            ਜੇ (ਗਿਣਤੀ_2 <= 0 || *(stmt->modifier + ਗਿਣਤੀ_2) != '\0' ||
                ਅਗਲਾ_incl.explicit_ਸਮੂਹ == 0 ||
                ਅਗਲਾ_incl.explicit_ਸਮੂਹ > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                ਅਗਲਾ_incl.explicit_ਸਮੂਹ = info->explicit_ਸਮੂਹ;
            } ਨਹੀਂਤਾਂ {
                ਅਗਲਾ_incl.explicit_ਸਮੂਹ--;
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ (info->keymap_info->keymap.ਅੰਕ_groups != 0 &&
                 ਅਗਲਾ_incl.include_ਡੂੰਘਾਈ == 1) {
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
            ਅਗਲਾ_incl.explicit_ਸਮੂਹ = 0;
        }
        ਨਹੀਂਤਾਂ {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            ਅਗਲਾ_incl.explicit_ਸਮੂਹ = info->explicit_ਸਮੂਹ;
        }

        HandleSymbolsFile(&ਅਗਲਾ_incl, file);

        ਮਿਲਾਓ_Included_Symbols(&included, &ਅਗਲਾ_incl, stmt->ਮਿਲਾਓ);

        ClearSymbolsInfo(&ਅਗਲਾ_incl);
        FreeXkbFile(file);
    }

    ਮਿਲਾਓ_Included_Symbols(info, &included, include->ਮਿਲਾਓ);
    ClearSymbolsInfo(&included);

    ਵਾਪਸ (info->error_ਗਿਣਤੀ == 0);
}

ਸਥਿਰ bool
Get_ਸਮੂਹ_ਸੂਚਕਾਂਕ(SymbolsInfo *info, ਕੁੰਜੀ_Info *keyi, ਵਿਅੰਜਕ_Def *arrayNdx,
              ਗਿਣਤੀ ਸਮੂਹ_ਖੇਤਰ ਖੇਤਰ_2, xkb_layout_ਸੂਚਕਾਂਕ_t *ndx_rtrn)
{
    ਪ੍ਰਤਿਜ੍ਞਾ (ਖੇਤਰ_2 == ਸਮੂਹ_ਖੇਤਰ_SYMS || ਖੇਤਰ_2 == ਸਮੂਹ_ਖੇਤਰ_ACTS);
    ਅਚਲ ਅੱਖਰ *name = (ਖੇਤਰ_2 == ਸਮੂਹ_ਖੇਤਰ_SYMS ? "symbols" : "actions");

    ਜੇ (arrayNdx == ਨਿਰਾ) {
        xkb_layout_ਸੂਚਕਾਂਕ_t i = 0;
        ਸਮੂਹ_Info_2 *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            ਜੇ (!(groupi->defined & ਖੇਤਰ_2)) {
                *ndx_rtrn = i;
                ਵਾਪਸ true;
            }
        }

        ਜੇ (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi), info->max_groups, name);
            ਵਾਪਸ false;
        }

        darray_resize0(keyi->groups, darray_ਮਾਪ(keyi->groups) + 1);
        *ndx_rtrn = darray_ਮਾਪ(keyi->groups) - 1;
        ਵਾਪਸ true;
    }

    ਜੇ (ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ(info->keymap_info, arrayNdx, false, ndx_rtrn, ਨਿਰਾ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
        ਵਾਪਸ false;
    }

    (*ndx_rtrn)--;
    ਜੇ (*ndx_rtrn >= darray_ਮਾਪ(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਜੋੜੋ_Symbols_ਮੰਜ਼ਿਲ_ਕੁੰਜੀ(SymbolsInfo *info, ਕੁੰਜੀ_Info *keyi, ਵਿਅੰਜਕ_Def *arrayNdx,
                ਵਿਅੰਜਕ_Def *ਮੁੱਲ)
{
    xkb_layout_ਸੂਚਕਾਂਕ_t ndx = 0;

    ਜੇ (!Get_ਸਮੂਹ_ਸੂਚਕਾਂਕ(info, keyi, arrayNdx, ਸਮੂਹ_ਖੇਤਰ_SYMS, &ndx))
        ਵਾਪਸ false;

    ਸਮੂਹ_Info_2 *groupi = &darray_ਵਸਤੂ(keyi->groups, ndx);

    ਜੇ (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_LIST) {
        groupi->defined |= ਸਮੂਹ_ਖੇਤਰ_SYMS;
        ਵਾਪਸ true;
    }

    ਜੇ (ਮੁੱਲ->common.type != STMT_ਵਿਅੰਜਕ_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(ਮੁੱਲ->common.type), ndx + 1,
                ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
        ਵਾਪਸ false;
    }

    ਜੇ (groupi->defined & ਸਮੂਹ_ਖੇਤਰ_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_ਕੁੰਜੀ_SYMBOLS_ਇੰਦਰਾਜ,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi), ndx + 1);
        ਵਾਪਸ false;
    }

    xkb_level_ਸੂਚਕਾਂਕ_t n_ਪੱਧਰ = 0;
    xkb_level_ਸੂਚਕਾਂਕ_t non_ਖਾਲੀ_ਥਾਵਾਂ_ਪੱਧਰ = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    ਲਈ (ਵਿਅੰਜਕ_Keysym_List *keysymList = (ਵਿਅੰਜਕ_Keysym_List *) ਮੁੱਲ;
         keysymList;
         keysymList = (ਵਿਅੰਜਕ_Keysym_List *) keysymList->common.ਅਗਲਾ) {
        n_ਪੱਧਰ++;
        /* Drop trailing NoSymbol */
        ਜੇ (darray_ਮਾਪ(keysymList->syms) > 0)
            non_ਖਾਲੀ_ਥਾਵਾਂ_ਪੱਧਰ = n_ਪੱਧਰ;
    }
    ਜੇ (non_ਖਾਲੀ_ਥਾਵਾਂ_ਪੱਧਰ < n_ਪੱਧਰ)
        n_ਪੱਧਰ = non_ਖਾਲੀ_ਥਾਵਾਂ_ਪੱਧਰ;
    ਜੇ (darray_ਮਾਪ(groupi->ਪੱਧਰ) < n_ਪੱਧਰ)
        darray_resize0(groupi->ਪੱਧਰ, n_ਪੱਧਰ);

    groupi->defined |= ਸਮੂਹ_ਖੇਤਰ_SYMS;

    xkb_level_ਸੂਚਕਾਂਕ_t level = 0;
    ਲਈ (ਵਿਅੰਜਕ_Keysym_List *keysymList = (ਵਿਅੰਜਕ_Keysym_List *) ਮੁੱਲ;
         keysymList && level < n_ਪੱਧਰ;
         keysymList = (ਵਿਅੰਜਕ_Keysym_List *) keysymList->common.ਅਗਲਾ, level++) {
        ਬਣਤਰ xkb_level *leveli = &darray_ਵਸਤੂ(groupi->ਪੱਧਰ, level);
        ਪ੍ਰਤਿਜ੍ਞਾ(leveli->ਅੰਕ_syms == 0);

        ਜੇ (unlikely(darray_ਮਾਪ(keysymList->syms) > MAX_KEYSYMS_ਪ੍ਰਤੀ_LEVEL)) {
            log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_ਪ੍ਰਤੀ_LEVEL, darray_ਮਾਪ(keysymList->syms));
            ਵਾਪਸ false;
        }

        leveli->ਅੰਕ_syms = (xkb_keysym_ਗਿਣਤੀ_t) darray_ਮਾਪ(keysymList->syms);
        ਚੋਣ (leveli->ਅੰਕ_syms) {
        ਹਾਲਤ 0:
            leveli->s.sym = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
            ਰੋਕੋ;
        ਹਾਲਤ 1:
            leveli->s.sym = darray_ਵਸਤੂ(keysymList->syms, 0);
            ਪ੍ਰਤਿਜ੍ਞਾ(leveli->s.sym != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ);
            ਰੋਕੋ;
        ਮੂਲ:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, ਨਿਰਾ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            ਲਈ (xkb_keysym_ਗਿਣਤੀ_t k = 0; k < leveli->ਅੰਕ_syms; k++)
                ਪ੍ਰਤਿਜ੍ਞਾ(leveli->s.syms[k] != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ);
#ਜੇ_ਅੰਤ
        }
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਜੋੜੋ_Actions_ਮੰਜ਼ਿਲ_ਕੁੰਜੀ(SymbolsInfo *info, ਕੁੰਜੀ_Info *keyi, ਵਿਅੰਜਕ_Def *arrayNdx,
                ਵਿਅੰਜਕ_Def *ਮੁੱਲ)
{
    xkb_layout_ਸੂਚਕਾਂਕ_t ndx = 0;

    ਜੇ (!Get_ਸਮੂਹ_ਸੂਚਕਾਂਕ(info, keyi, arrayNdx, ਸਮੂਹ_ਖੇਤਰ_ACTS, &ndx))
        ਵਾਪਸ false;

    ਸਮੂਹ_Info_2 *groupi = &darray_ਵਸਤੂ(keyi->groups, ndx);

    ਜੇ (ਮੁੱਲ->common.type == STMT_ਵਿਅੰਜਕ_ਖਾਲੀ_ਥਾਵਾਂ_LIST) {
        groupi->defined |= ਸਮੂਹ_ਖੇਤਰ_ACTS;
        ਵਾਪਸ true;
    }

    ਜੇ (ਮੁੱਲ->common.type != STMT_ਵਿਅੰਜਕ_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_ਵਿਅੰਜਕ_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 ਮੁੱਲ->common.type, ndx, ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
        ਵਾਪਸ false;
    }

    ਜੇ (groupi->defined & ਸਮੂਹ_ਖੇਤਰ_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi), ndx);
        ਵਾਪਸ false;
    }

    xkb_level_ਸੂਚਕਾਂਕ_t n_ਪੱਧਰ = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    ਲਈ (ParseCommon *p = &ਮੁੱਲ->common; p; p = p->ਅਗਲਾ)
        n_ਪੱਧਰ++;
    ਜੇ (darray_ਮਾਪ(groupi->ਪੱਧਰ) < n_ਪੱਧਰ)
        darray_resize0(groupi->ਪੱਧਰ, n_ਪੱਧਰ);

    groupi->defined |= ਸਮੂਹ_ਖੇਤਰ_ACTS;

    xkb_level_ਸੂਚਕਾਂਕ_t level = 0;
    xkb_level_ਸੂਚਕਾਂਕ_t non_ਖਾਲੀ_ਥਾਵਾਂ_ਪੱਧਰ = 0;
    ਲਈ (ਵਿਅੰਜਕ_Action_List *actionList = (ਵਿਅੰਜਕ_Action_List *) ਮੁੱਲ;
         actionList;
         actionList = (ਵਿਅੰਜਕ_Action_List *) actionList->common.ਅਗਲਾ, level++) {
        ਬਣਤਰ xkb_level *leveli = &darray_ਵਸਤੂ(groupi->ਪੱਧਰ, level);
        ਪ੍ਰਤਿਜ੍ਞਾ(leveli->ਅੰਕ_actions == 0);

        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_actions = 0;
        ਲਈ (ਵਿਅੰਜਕ_Def *act = actionList->actions;
             act; act = (ਵਿਅੰਜਕ_Def *) act->common.ਅਗਲਾ)
            ਅੰਕ_actions++;

        ਜੇ (unlikely(ਅੰਕ_actions > MAX_ACTIONS_ਪ੍ਰਤੀ_LEVEL)) {
            log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_ਪ੍ਰਤੀ_LEVEL, ਅੰਕ_actions);
            ਵਾਪਸ false;
        }

        /* Parse actions and add only defined actions */
        darray(ਸੰਘ xkb_action) actions = darray_new();
        ਲਈ (ਵਿਅੰਜਕ_Def *act = actionList->actions;
             act; act = (ਵਿਅੰਜਕ_Def *) act->common.ਅਗਲਾ) {
            ਸੰਘ xkb_action ਮੰਜ਼ਿਲ_Act = { 0 };
            ਅਚਲ ਗਿਣਤੀ xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &ਮੰਜ਼ਿਲ_Act);
            ਜੇ (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_ਮੁੱਲ,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi), ndx + 1, level + 1);
                ਜੇ (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    ਵਾਪਸ false;
                } ਨਹੀਂਤਾਂ {
                    /* Ensure action type is reset */
                    ਮੰਜ਼ਿਲ_Act.type = ACTION_TYPE_NONE;
                }
            }
            ਜੇ (ਮੰਜ਼ਿਲ_Act.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } ਨਹੀਂਤਾਂ ਜੇ (likely(ਅੰਕ_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->ਅੰਕ_actions = 1;
                leveli->a.action = ਮੰਜ਼ਿਲ_Act;
                ਜਾਓ ਅਗਲਾ_level;
            } ਨਹੀਂਤਾਂ {
                darray_append(actions, ਮੰਜ਼ਿਲ_Act);
            }
        }
        ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(actions)) {
            leveli->ਅੰਕ_actions = 0;
        } ਨਹੀਂਤਾਂ ਜੇ (likely(darray_ਮਾਪ(actions) > 1)) {
            leveli->ਅੰਕ_actions = (xkb_action_ਗਿਣਤੀ_t) darray_ਮਾਪ(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, ਨਿਰਾ);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NDEBUG
            /* Canonical list: all NoAction() were dropped */
            ਲਈ (xkb_action_ਗਿਣਤੀ_t k = 0; k < leveli->ਅੰਕ_actions; k++)
                ਪ੍ਰਤਿਜ੍ਞਾ(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#ਜੇ_ਅੰਤ
        } ਨਹੀਂਤਾਂ {
            /* Unlikely: some actions were dropped and only one remains */
            ਪ੍ਰਤਿਜ੍ਞਾ(ਅੰਕ_actions > darray_ਮਾਪ(actions));
            leveli->ਅੰਕ_actions = 1;
            leveli->a.action = darray_ਵਸਤੂ(actions, 0);
            ਪ੍ਰਤਿਜ੍ਞਾ(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
ਅਗਲਾ_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        ਜੇ (leveli->ਅੰਕ_actions > 0 || leveli->ਅੰਕ_syms > 0)
            non_ਖਾਲੀ_ਥਾਵਾਂ_ਪੱਧਰ = level + 1;
    }

    ਜੇ (non_ਖਾਲੀ_ਥਾਵਾਂ_ਪੱਧਰ < n_ਪੱਧਰ) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_ਮਾਪ(groupi->ਪੱਧਰ) = non_ਖਾਲੀ_ਥਾਵਾਂ_ਪੱਧਰ;
        ਜੇ (non_ਖਾਲੀ_ਥਾਵਾਂ_ਪੱਧਰ > 0)
            darray_shrink(groupi->ਪੱਧਰ);
        ਨਹੀਂਤਾਂ
            darray_free(groupi->ਪੱਧਰ);
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ ਅਚਲ Lookup_ਇੰਦਰਾਜ repeatEntries[] = {
    { "true", ਕੁੰਜੀ_REPEAT_YES },
    { "yes", ਕੁੰਜੀ_REPEAT_YES },
    { "on", ਕੁੰਜੀ_REPEAT_YES },
    { "false", ਕੁੰਜੀ_REPEAT_NO },
    { "no", ਕੁੰਜੀ_REPEAT_NO },
    { "off", ਕੁੰਜੀ_REPEAT_NO },
    { "default", ਕੁੰਜੀ_REPEAT_UNDEFINED },
    { ਨਿਰਾ, 0 }
};

/** Handle overlay<N> = <key> entry */
ਸਥਿਰ bool
ਵਿਅੰਜਕ_Resolve_Overlay_ਇੰਦਰਾਜ(ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                        ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2, ਅਚਲ ਵਿਅੰਜਕ_Def *arrayNdx,
                        ਅਚਲ ਵਿਅੰਜਕ_Def *ਵਿਅੰਜਕ, ਕੁੰਜੀ_Info *keyi,
                        xkb_overlay_ਸੂਚਕਾਂਕ_t *overlay_rtrn,
                        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ **ਕੁੰਜੀ_rtrn)
{
    /* Overlay index */
    ਜੇ (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                ਖੇਤਰ_2, ਕੁੰਜੀ_Name_ਪਾਠ(keymap_info->keymap.ctx, keyi->name));
        ਵਾਪਸ false;
    }
    static_assert(ਆਕਾਰ(xkb_overlay_ਸੂਚਕਾਂਕ_t) == ਆਕਾਰ(uint8_t), "");
    ਅਚਲ ਮਾਪ_t ਅਗੇਤਰ = ਆਕਾਰ("overlay") - 1;
    ਅਚਲ ਮਾਪ_t len = ਲੜੀ_ਲੰਬਾਈ(ਖੇਤਰ_2 + ਅਗੇਤਰ);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    ਜੇ (parse_dec_ਮੰਜ਼ਿਲ_uint64_t(ਖੇਤਰ_2 + ਅਗੇਤਰ, len, (uint64_t *)&raw_overlay)
        != (ਪੂਰਨ)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_ਸੂਚਕਾਂਕ,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                ਖੇਤਰ_2, ਕੁੰਜੀ_Name_ਪਾਠ(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        ਵਾਪਸ false;
    }
    *overlay_rtrn = (xkb_overlay_ਸੂਚਕਾਂਕ_t)raw_overlay - 1;

    /* Overlay key */
    ਚੋਣ (ਵਿਅੰਜਕ->common.type) {
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL:
        *ਕੁੰਜੀ_rtrn = Xkb_ਕੁੰਜੀ_By_Name(&keymap_info->keymap, ਵਿਅੰਜਕ->ਕੁੰਜੀ_name.ਕੁੰਜੀ_name,
                                 false);
        ਜੇ (!*ਕੁੰਜੀ_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_ਪਾਠ(keymap_info->keymap.ctx, ਵਿਅੰਜਕ->ਕੁੰਜੀ_name.ਕੁੰਜੀ_name),
                    ਖੇਤਰ_2, ਕੁੰਜੀ_Name_ਪਾਠ(keymap_info->keymap.ctx, keyi->name));
            ਵਾਪਸ false;
        }
        ਵਾਪਸ true;
    ਹਾਲਤ STMT_ਵਿਅੰਜਕ_IDENT: {
        ਅਚਲ ਅੱਖਰ * ਅਚਲ id =
            xkb_atom_ਪਾਠ(keymap_info->keymap.ctx, ਵਿਅੰਜਕ->ident.ident);
        ਜੇ (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *ਕੁੰਜੀ_rtrn = ਨਿਰਾ;
            ਵਾਪਸ true;
        } ਨਹੀਂਤਾਂ ਜੇ (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *ਕੁੰਜੀ_rtrn = ਨਿਰਾ;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            ਵਾਪਸ true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ਮੁੱਲ,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, ਖੇਤਰ_2, ਕੁੰਜੀ_Name_ਪਾਠ(keymap_info->keymap.ctx, keyi->name));
        ਵਾਪਸ false;
    }
    ਮੂਲ:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_ਮੁੱਲ,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_ਮੰਜ਼ਿਲ_string(STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL),
                ਖੇਤਰ_2, ਕੁੰਜੀ_Name_ਪਾਠ(keymap_info->keymap.ctx, keyi->name),
                stmt_type_ਮੰਜ਼ਿਲ_string(ਵਿਅੰਜਕ->common.type));
        ਵਾਪਸ false;
    }
}

ਸਥਿਰ bool
ਸੈੱਟ_Symbols_ਖੇਤਰ(SymbolsInfo *info, ਕੁੰਜੀ_Info *keyi, ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2,
                ਵਿਅੰਜਕ_Def *arrayNdx, ਵਿਅੰਜਕ_Def **ਮੁੱਲ_ptr)
{
    ਵਿਅੰਜਕ_Def * ਸੀਮਿਤ ਅਚਲ ਮੁੱਲ = *ਮੁੱਲ_ptr;
    ਜੇ (istreq(ਖੇਤਰ_2, "type")) {
        xkb_layout_ਸੂਚਕਾਂਕ_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_String(info->ctx, ਮੁੱਲ, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            ਵਾਪਸ false;
        }

        ਜੇ (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= ਕੁੰਜੀ_ਖੇਤਰ_DEFAULT_TYPE;
        }
        ਨਹੀਂਤਾਂ ਜੇ (ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ(info->keymap_info, arrayNdx, false,
                                  &ndx, ਨਿਰਾ) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
            ਵਾਪਸ false;
        }
        ਨਹੀਂਤਾਂ {
            ndx--;
            ਜੇ (ndx >= darray_ਮਾਪ(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                ਪ੍ਰਤਿਜ੍ਞਾ(darray_ਮਾਪ(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_ਵਸਤੂ(keyi->groups, ndx).type = val;
            darray_ਵਸਤੂ(keyi->groups, ndx).defined |= ਸਮੂਹ_ਖੇਤਰ_TYPE;
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "symbols")) {
        ਵਾਪਸ ਜੋੜੋ_Symbols_ਮੰਜ਼ਿਲ_ਕੁੰਜੀ(info, keyi, arrayNdx, ਮੁੱਲ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "actions")) {
        ਵਾਪਸ ਜੋੜੋ_Actions_ਮੰਜ਼ਿਲ_ਕੁੰਜੀ(info, keyi, arrayNdx, ਮੁੱਲ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "vmods") ||
             istreq(ਖੇਤਰ_2, "virtualmods") ||
             istreq(ਖੇਤਰ_2, "virtualmodifiers")) {
        xkb_mod_ਨਕਾਬ_t ਨਕਾਬ = 0;

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(info->ctx, ਮੁੱਲ, MOD_VIRT, &info->mods,
                                &ਨਕਾਬ)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_ਨਕਾਬ,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_ਮੰਜ਼ਿਲ_string(ਮੁੱਲ->common.type),
                    ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
            ਵਾਪਸ false;
        }

        keyi->vmodmap = ਨਕਾਬ;
        keyi->defined |= ਕੁੰਜੀ_ਖੇਤਰ_VMODMAP;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "locking") ||
             istreq(ਖੇਤਰ_2, "lock") ||
             istreq(ਖੇਤਰ_2, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ਖੇਤਰ,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "radiogroup") ||
             istreq(ਖੇਤਰ_2, "permanentradiogroup") ||
             istreq(ਖੇਤਰ_2, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ਖੇਤਰ,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq_ਅਗੇਤਰ("permanentoverlay", ਖੇਤਰ_2)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_ਖੇਤਰ,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq_ਅਗੇਤਰ("overlay", ਖੇਤਰ_2)) {
        xkb_overlay_ਸੂਚਕਾਂਕ_t overlay = XKB_OVERLAY_INVALID;
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = ਨਿਰਾ;
        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Overlay_ਇੰਦਰਾਜ(info->keymap_info, ਖੇਤਰ_2, arrayNdx,
                                     *ਮੁੱਲ_ptr, keyi, &overlay, &ਕੁੰਜੀ))
            ਵਾਪਸ false;
        ਜੇ (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            ਵਾਪਸ true;
        } ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ && ਕੁੰਜੀ->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
        } ਨਹੀਂਤਾਂ {
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *prev;
            ਜੇ (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                ਜੇ (ਕੁੰਜੀ != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi), overlay + 1,
                             (prev
                                 ? ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (ਕੁੰਜੀ
                                 ? ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, ਕੁੰਜੀ->name)
                                 : "none"));
                }
            } ਨਹੀਂਤਾਂ ਜੇ (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                ਜੇ (!overlays_ਪਾਓ(keyi, overlay, ਕੁੰਜੀ))
                    ਵਾਪਸ false;
                keyi->defined |= ਕੁੰਜੀ_ਖੇਤਰ_OVERLAY;
            } ਨਹੀਂਤਾਂ {
                /* Handle disjoint overlays mode */
                ਅਚਲ xkb_overlay_ਨਕਾਬ_t ਨਕਾਬ = (xkb_overlay_ਨਕਾਬ_t)(1u << overlay);
                ਜੇ (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    ਜੇ (ਕੁੰਜੀ) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = ਨਕਾਬ;
                        keyi->overlays_clear = false;
                        keyi->overlay_ਕੁੰਜੀ = ਕੁੰਜੀ;
                    } ਨਹੀਂਤਾਂ {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= ਨਕਾਬ;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= ਕੁੰਜੀ_ਖੇਤਰ_OVERLAY;
                } ਨਹੀਂਤਾਂ ਜੇ (keyi->overlays) {
                    ਜੇ (ਕੁੰਜੀ) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi), keyi->overlays,
                                ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, keyi->overlay_ਕੁੰਜੀ->name),
                                overlay + 1, ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, ਕੁੰਜੀ->name));
                        ਵਾਪਸ !(info->keymap_info->strict &
                                 PARSER_NO_ਖੇਤਰ_ਮੁੱਲ_MISMATCH);
                    } ਨਹੀਂਤਾਂ {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "repeating") ||
             istreq(ਖੇਤਰ_2, "repeats") ||
             istreq(ਖੇਤਰ_2, "repeat")) {
        uint32_t val = 0;

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Enum(info->ctx, ਮੁੱਲ, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ਮੁੱਲ,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
            ਵਾਪਸ false;
        }

        keyi->repeat = (ਗਿਣਤੀ ਕੁੰਜੀ_repeat) val;
        keyi->defined |= ਕੁੰਜੀ_ਖੇਤਰ_REPEAT;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "groupswrap") ||
             istreq(ਖੇਤਰ_2, "wrapgroups")) {
        bool ਸੈੱਟ = false;

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Boolean(info->ctx, ਮੁੱਲ, &ਸੈੱਟ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ਮੁੱਲ,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
            ਵਾਪਸ false;
        }

        keyi->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy = (ਸੈੱਟ)
            ? XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP
            : XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP;
        keyi->defined |= ਕੁੰਜੀ_ਖੇਤਰ_GROUPINFO;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "groupsclamp") ||
             istreq(ਖੇਤਰ_2, "clampgroups")) {
        bool ਸੈੱਟ = false;

        ਜੇ (!ਵਿਅੰਜਕ_Resolve_Boolean(info->ctx, ਮੁੱਲ, &ਸੈੱਟ)) {
            log_err(info->ctx, XKB_ERROR_INVALID_ਮੁੱਲ,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
            ਵਾਪਸ false;
        }

        keyi->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy = (ਸੈੱਟ)
            ? XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP
            : XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP;
        keyi->defined |= ਕੁੰਜੀ_ਖੇਤਰ_GROUPINFO;
    }
    ਨਹੀਂਤਾਂ ਜੇ (istreq(ਖੇਤਰ_2, "groupsredirect") ||
             istreq(ਖੇਤਰ_2, "redirectgroups")) {
        xkb_layout_ਸੂਚਕਾਂਕ_t grp = 0;
        bool pending = false;

        // TODO: recover?
        ਜੇ (ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ(info->keymap_info, ਮੁੱਲ, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
            ਵਾਪਸ false;
        }

        ਜੇ (pending) {
            keyi->ਬਾਹਰਲਾ_ਦਾ_range_pending_ਸਮੂਹ = true;
            ਅਚਲ darray_ਮਾਪ_t pending_ਸੂਚਕਾਂਕ =
                darray_ਮਾਪ(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (ਬਣਤਰ pending_computation) {
                    .ਵਿਅੰਜਕ = *ਮੁੱਲ_ptr,
                    .computed = false,
                    .ਮੁੱਲ = 0,
                }
            );
            *ਮੁੱਲ_ptr = ਨਿਰਾ;
            static_assert(ਆਕਾਰ(keyi->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ) ==
                          ਆਕਾਰ(pending_ਸੂਚਕਾਂਕ),
                          "Cannot save pending computation");
            keyi->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ = pending_ਸੂਚਕਾਂਕ;
        } ਨਹੀਂਤਾਂ {
            keyi->ਬਾਹਰਲਾ_ਦਾ_range_pending_ਸਮੂਹ = false;
            keyi->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ = grp - 1;
        }

        keyi->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy = XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT;
        keyi->defined |= ਕੁੰਜੀ_ਖੇਤਰ_GROUPINFO;
    }
    ਨਹੀਂਤਾਂ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_ਖੇਤਰ,
                "Unknown field \"%s\" in a key; definition ignored\n",
                ਖੇਤਰ_2);
        ਵਾਪਸ !(info->keymap_info->strict & PARSER_NO_UNKNOWN_ਕੁੰਜੀ_FIELDS);
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
ਸੈੱਟ_ਸਮੂਹ_Name(SymbolsInfo *info, ਵਿਅੰਜਕ_Def *arrayNdx, ਵਿਅੰਜਕ_Def *ਮੁੱਲ,
             ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ)
{
    ਜੇ (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_ਸਮੂਹ_NAME_ਸੂਚਕਾਂਕ,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        ਵਾਪਸ false;
    }

    xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0;
    ਜੇ (ਵਿਅੰਜਕ_Resolve_ਸਮੂਹ(info->keymap_info, arrayNdx, false, &ਸਮੂਹ, ਨਿਰਾ) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_ਸੂਚਕਾਂਕ_2,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        ਵਾਪਸ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ਜੇ (!ਵਿਅੰਜਕ_Resolve_String(info->ctx, ਮੁੱਲ, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", ਸਮੂਹ);
        ਵਾਪਸ false;
    }

    xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ_ਮੰਜ਼ਿਲ_use;
    ਜੇ (info->explicit_ਸਮੂਹ == XKB_LAYOUT_INVALID) {
        ਸਮੂਹ_ਮੰਜ਼ਿਲ_use = ਸਮੂਹ - 1;
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਸਮੂਹ - 1 == 0) {
        ਸਮੂਹ_ਮੰਜ਼ਿਲ_use = info->explicit_ਸਮੂਹ;
    }
    ਨਹੀਂਤਾਂ {
        log_warn(info->ctx, XKB_WARNING_NON_ਅਧਾਰ_ਸਮੂਹ_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, ਸਮੂਹ,
                 xkb_atom_ਪਾਠ(info->ctx, name));
        ਵਾਪਸ false;
    }

    ਜੇ (ਸਮੂਹ_ਮੰਜ਼ਿਲ_use >= darray_ਮਾਪ(info->ਸਮੂਹ_ਨਾਂ)) {
        /* Avoid clang-tidy false positive */
        ਪ੍ਰਤਿਜ੍ਞਾ(darray_ਮਾਪ(info->ਸਮੂਹ_ਨਾਂ) < ਸਮੂਹ_ਮੰਜ਼ਿਲ_use + 1);
        darray_resize0(info->ਸਮੂਹ_ਨਾਂ, ਸਮੂਹ_ਮੰਜ਼ਿਲ_use + 1);
    } ਨਹੀਂਤਾਂ {
        ਅਚਲ xkb_atom_t old_name = darray_ਵਸਤੂ(info->ਸਮੂਹ_ਨਾਂ, ਸਮੂਹ_ਮੰਜ਼ਿਲ_use);
        ਜੇ (old_name != XKB_ATOM_NONE && old_name != name) {
            ਅਚਲ bool replace = (ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);
            ਅਚਲ xkb_atom_t use    = (replace ? name : old_name);
            ਅਚਲ xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     ਸਮੂਹ_ਮੰਜ਼ਿਲ_use, info->name,
                     xkb_atom_ਪਾਠ(info->ctx, use),
                     xkb_atom_ਪਾਠ(info->ctx, ignore));
            name = use;
        }
    }
    darray_ਵਸਤੂ(info->ਸਮੂਹ_ਨਾਂ, ਸਮੂਹ_ਮੰਜ਼ਿਲ_use) = name;

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ਅਚਲ ਅੱਖਰ *elem, *ਖੇਤਰ_2;
    ਵਿਅੰਜਕ_Def *arrayNdx;
    bool ret;

    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Lhs(info->ctx, stmt->name, &elem, &ਖੇਤਰ_2, &arrayNdx))
        ਵਾਪਸ false;

    ਜੇ (elem && istreq(elem, "key")) {
        ਕੁੰਜੀ_Info ਆਰਜ਼ੀ = {0};
        Init_ਕੁੰਜੀ_Info(info->ctx, &ਆਰਜ਼ੀ);
        /* Do not replace the whole key, only the current field */
        ਆਰਜ਼ੀ.ਮਿਲਾਓ = (ਆਰਜ਼ੀ.ਮਿਲਾਓ == ਮਿਲਾਓ_REPLACE)
            ? ਮਿਲਾਓ_OVERRIDE
            : stmt->ਮਿਲਾਓ;
        ret = ਸੈੱਟ_Symbols_ਖੇਤਰ(info, &ਆਰਜ਼ੀ, ਖੇਤਰ_2, arrayNdx, &stmt->ਮੁੱਲ) &&
              ਮਿਲਾਓ_ਕੁੰਜੀਆਂ(info, &info->default_ਕੁੰਜੀ, &ਆਰਜ਼ੀ, true);
        Clear_ਕੁੰਜੀ_Info(&ਆਰਜ਼ੀ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (!elem && (istreq(ਖੇਤਰ_2, "name") ||
                       istreq(ਖੇਤਰ_2, "groupname"))) {
        ret = ਸੈੱਟ_ਸਮੂਹ_Name(info, arrayNdx, stmt->ਮੁੱਲ, stmt->ਮਿਲਾਓ);
    }
    ਨਹੀਂਤਾਂ ਜੇ (!elem && (istreq(ਖੇਤਰ_2, "groupswrap") ||
                       istreq(ਖੇਤਰ_2, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ਖੇਤਰ,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    ਨਹੀਂਤਾਂ ਜੇ (!elem && (istreq(ਖੇਤਰ_2, "groupsclamp") ||
                       istreq(ਖੇਤਰ_2, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ਖੇਤਰ,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    ਨਹੀਂਤਾਂ ਜੇ (!elem && (istreq(ਖੇਤਰ_2, "groupsredirect") ||
                       istreq(ਖੇਤਰ_2, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ਖੇਤਰ,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    ਨਹੀਂਤਾਂ ਜੇ (!elem && istreq(ਖੇਤਰ_2, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_ਖੇਤਰ,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    ਨਹੀਂਤਾਂ ਜੇ (elem) {
        ret = (ਸੈੱਟ_Default_Action_ਖੇਤਰ(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, ਖੇਤਰ_2, arrayNdx,
                                     &stmt->ਮੁੱਲ, stmt->ਮਿਲਾਓ) !=
               PARSER_FATAL_ERROR);
    } ਨਹੀਂਤਾਂ {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ਖੇਤਰ,
                "Default defined for unknown field \"%s\"; Ignored\n", ਖੇਤਰ_2);
        ਵਾਪਸ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    ਵਾਪਸ ret;
}

ਸਥਿਰ bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, ਕੁੰਜੀ_Info *keyi)
{
    bool all_valid_entries = true;

    ਲਈ (; def; def = (VarDef *) def->common.ਅਗਲਾ) {
        ਅਚਲ ਅੱਖਰ *ਖੇਤਰ_2;
        ਵਿਅੰਜਕ_Def *arrayNdx;
        bool ok = true;

        ਜੇ (!def->name) {
            ਜੇ (unlikely(!def->ਮੁੱਲ) || def->ਮੁੱਲ->common.type != STMT_ਵਿਅੰਜਕ_ACTION_LIST)
                ਖੇਤਰ_2 = "symbols"; /* Default to symbols field */
            ਨਹੀਂਤਾਂ
                ਖੇਤਰ_2 = "actions";
            arrayNdx = ਨਿਰਾ;
        }
        ਨਹੀਂਤਾਂ {
            ਅਚਲ ਅੱਖਰ *elem;
            ok = ਵਿਅੰਜਕ_Resolve_Lhs(info->ctx, def->name, &elem, &ਖੇਤਰ_2,
                                &arrayNdx);
            ਜੇ (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, ਖੇਤਰ_2);
                ok = false;
            }
        }

        ਜੇ (unlikely(!def->ਮੁੱਲ)) {
            log_err(info->ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", ਖੇਤਰ_2);
            ok = false;
        }

        ਜੇ (!ok || !ਸੈੱਟ_Symbols_ਖੇਤਰ(info, keyi, ਖੇਤਰ_2, arrayNdx, &def->ਮੁੱਲ))
            all_valid_entries = false;
    }

    ਵਾਪਸ all_valid_entries;
}

ਸਥਿਰ bool
ਸੈੱਟ_Explicit_ਸਮੂਹ(SymbolsInfo *info, ਕੁੰਜੀ_Info *keyi)
{
    xkb_layout_ਸੂਚਕਾਂਕ_t i;
    ਸਮੂਹ_Info_2 *groupi;
    bool warn = false;

    ਜੇ (info->explicit_ਸਮੂਹ == XKB_LAYOUT_INVALID)
        ਵਾਪਸ true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        ਜੇ (groupi->defined) {
            warn = true;
            Clear_ਸਮੂਹ_Info(groupi);
            Init_ਸਮੂਹ_Info(groupi);
        }
    }

    ਜੇ (warn) {
        log_warn(info->ctx, XKB_WARNING_ਗੁਣਜ_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_ਸਮੂਹ + 1, ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_ਸਮੂਹ + 1);
    ਜੇ (info->explicit_ਸਮੂਹ > 0) {
        darray_ਵਸਤੂ(keyi->groups, info->explicit_ਸਮੂਹ) =
            darray_ਵਸਤੂ(keyi->groups, 0);
        Init_ਸਮੂਹ_Info(&darray_ਵਸਤੂ(keyi->groups, 0));
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    ਕੁੰਜੀ_Info keyi;

    keyi = info->default_ਕੁੰਜੀ;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_ਕੁੰਜੀ.groups);
    ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t i = 0; i < darray_ਮਾਪ(keyi.groups); i++)
        Copy_ਸਮੂਹ_Info(&darray_ਵਸਤੂ(keyi.groups, i),
                      &darray_ਵਸਤੂ(info->default_ਕੁੰਜੀ.groups, i));
    keyi.ਮਿਲਾਓ = stmt->ਮਿਲਾਓ;
    keyi.name = stmt->ਕੁੰਜੀ_Name;

    ਜੇ (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        ਸੈੱਟ_Explicit_ਸਮੂਹ(info, &keyi) &&
        ਜੋੜੋ_ਕੁੰਜੀ_Symbols(info, &keyi, true)) {
        ਵਾਪਸ true;
    }

    Clear_ਕੁੰਜੀ_Info(&keyi);
    info->error_ਗਿਣਤੀ++;
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    Mod_Map_ਇੰਦਰਾਜ tmp;
    ਬਣਤਰ xkb_context *ctx = info->ctx;

    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        ਵਾਪਸ false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    ਪ੍ਰਤਿਜ੍ਞਾ(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_ਪਾਠ_V1 ||
           def->modifiers->common.type == STMT_ਵਿਅੰਜਕ_IDENT);

    bool ok = true;
    tmp.ਮਿਲਾਓ = def->ਮਿਲਾਓ;

    ਲਈ (ਵਿਅੰਜਕ_Def *ਕੁੰਜੀ = def->ਕੁੰਜੀਆਂ; ਕੁੰਜੀ; ਕੁੰਜੀ = (ਵਿਅੰਜਕ_Def *) ਕੁੰਜੀ->common.ਅਗਲਾ) {
        ਜੇ (ਕੁੰਜੀ->common.type == STMT_ਵਿਅੰਜਕ_KEYNAME_LITERAL) {
            tmp.have_ਚਿੰਨ੍ਹ = false;
            tmp.u.ਕੁੰਜੀ_Name = ਕੁੰਜੀ->ਕੁੰਜੀ_name.ਕੁੰਜੀ_name;
        }
        ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->common.type == STMT_ਵਿਅੰਜਕ_KEYSYM_LITERAL) {
            ਜੇ (ਕੁੰਜੀ->keysym.keysym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
                /* Invalid keysym: ignore. Error message already printed */
                ਜਾਰੀ;
            } ਨਹੀਂਤਾਂ {
                tmp.have_ਚਿੰਨ੍ਹ = true;
                tmp.u.ਕੁੰਜੀ_Sym = ਕੁੰਜੀ->keysym.keysym;
            }
        }
        ਨਹੀਂਤਾਂ {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ਇੰਦਰਾਜ,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            ਜਾਰੀ;
        }

        ok = ਜੋੜੋ_Mod_Map_ਇੰਦਰਾਜ(info, &tmp) && ok;
    }
    ਵਾਪਸ ok;
}

ਸਥਿਰ ਖਾਲੀ
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    ਮੁਕਤ(info->name);
    info->name = strdup_ਸੁਰੱਖਿਅਤ(file->name);

    ਲਈ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ਅਗਲਾ) {
        ਚੋਣ (stmt->type) {
        ਹਾਲਤ STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_UNKNOWN_DECLARATION:
        ਹਾਲਤ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ਰੋਕੋ;
        ਮੂਲ:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_ਮੰਜ਼ਿਲ_string(stmt->type));
            ok = false;
            ਰੋਕੋ;
        }

        ਜੇ (!ok)
            info->error_ਗਿਣਤੀ++;

        ਜੇ (info->error_ਗਿਣਤੀ > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    ਸੁਰੱਖਿਅਤ_map_name(file->name));
            ਰੋਕੋ;
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
ਸਥਿਰ ਬਣਤਰ xkb_ਕੁੰਜੀ *
ਲੱਭੋ_ਕੁੰਜੀ_For_ਚਿੰਨ੍ਹ(ਬਣਤਰ xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_ਸਮੂਹ;
    xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0;
    ਕਰੋ {
        xkb_level_ਸੂਚਕਾਂਕ_t level = 0;
        got_one_ਸਮੂਹ = false;
        bool got_one_level;
        ਕਰੋ {
            got_one_level = false;
            ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;
            xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap) {
                ਜੇ (ਸਮੂਹ < ਕੁੰਜੀ->ਅੰਕ_groups &&
                    level < Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, ਸਮੂਹ)) {
                    got_one_ਸਮੂਹ = got_one_level = true;
                    ਅਚਲ xkb_keysym_ਗਿਣਤੀ_t ਅੰਕ_syms =
                        ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].ਅੰਕ_syms;
                    ਜੇ (ਅੰਕ_syms > 1) {
                        ਲਈ (xkb_keysym_ਗਿਣਤੀ_t k = 0; k < ਅੰਕ_syms; k++) {
                            ਜੇ (ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].s.syms[k] == sym)
                                ਵਾਪਸ ਕੁੰਜੀ;
                        }
                    } ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕ_syms &&
                               ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].s.sym == sym) {
                        ਵਾਪਸ ਕੁੰਜੀ;
                    }
                }
            }
            level++;
        } ਜਦੋਂ (got_one_level);
        ਸਮੂਹ++;
    } ਜਦੋਂ (got_one_ਸਮੂਹ);

    ਵਾਪਸ ਨਿਰਾ;
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
ਸਥਿਰ xkb_atom_t
ਲੱਭੋ_Automatic_Type(ਬਣਤਰ xkb_context *ctx, ਸਮੂਹ_Info_2 *groupi)
{
    xkb_keysym_t sym0, sym1;
    ਅਚਲ xkb_level_ਸੂਚਕਾਂਕ_t ਚੌੜਾਈ = darray_ਮਾਪ(groupi->ਪੱਧਰ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_SYM(level) \
    (darray_ਵਸਤੂ(groupi->ਪੱਧਰ, level).ਅੰਕ_syms == 0 ? \
        XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ : \
     darray_ਵਸਤੂ(groupi->ਪੱਧਰ, level).ਅੰਕ_syms == 1 ? \
        darray_ਵਸਤੂ(groupi->ਪੱਧਰ, level).s.sym : \
     /* num_syms > 1 */ \
        darray_ਵਸਤੂ(groupi->ਪੱਧਰ, level).s.syms[0])

    ਜੇ (ਚੌੜਾਈ == 1 || ਚੌੜਾਈ <= 0)
        ਵਾਪਸ xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    ਜੇ (ਚੌੜਾਈ == 2) {
        ਜੇ (xkb_keysym_is_ਹੇਠਲਾ(sym0) && xkb_keysym_is_ਉੱਤਲਾ_or_title(sym1))
            ਵਾਪਸ xkb_atom_intern_literal(ctx, "ALPHABETIC");

        ਜੇ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            ਵਾਪਸ xkb_atom_intern_literal(ctx, "KEYPAD");

        ਵਾਪਸ xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    ਜੇ (ਚੌੜਾਈ <= 4) {
        ਜੇ (xkb_keysym_is_ਹੇਠਲਾ(sym0) && xkb_keysym_is_ਉੱਤਲਾ_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (ਚੌੜਾਈ == 4 ? GET_SYM(3) : XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ);

            ਜੇ (xkb_keysym_is_ਹੇਠਲਾ(sym2) && xkb_keysym_is_ਉੱਤਲਾ_or_title(sym3))
                ਵਾਪਸ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            ਵਾਪਸ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        ਜੇ (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            ਵਾਪਸ xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        ਵਾਪਸ xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    ਵਾਪਸ XKB_ATOM_NONE;

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ GET_SYM
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type *
ਲੱਭੋ_Type_For_ਸਮੂਹ(ਬਣਤਰ xkb_keymap *keymap, ਕੁੰਜੀ_Info *keyi,
                 xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ, bool *explicit_type)
{
    ਸਮੂਹ_Info_2 *groupi = &darray_ਵਸਤੂ(keyi->groups, ਸਮੂਹ);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    ਜੇ (type_name == XKB_ATOM_NONE) {
        ਜੇ (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        ਨਹੀਂਤਾਂ {
            type_name = ਲੱਭੋ_Automatic_Type(keymap->ctx, groupi);
            ਜੇ (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    ਜੇ (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_ਕੁੰਜੀ_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, keyi->name), ਸਮੂਹ + 1,
                 darray_ਮਾਪ(groupi->ਪੱਧਰ));
        ਜਾਓ use_default;
    }

    darray_ਮਾਪ_t i;
    ਲਈ (i = 0; i < keymap->ਅੰਕ_types; i++)
        ਜੇ (keymap->types[i].name == type_name)
            ਰੋਕੋ;

    ਜੇ (i >= keymap->ਅੰਕ_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_ਕੁੰਜੀ_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_ਪਾਠ(keymap->ctx, type_name),
                 ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, keyi->name), ਸਮੂਹ + 1);
        ਜਾਓ use_default;
    }

    keymap->types[i].required = true;
    ਵਾਪਸ &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    ਵਾਪਸ &keymap->types[0];
}

ਸਥਿਰ bool
Copy_Symbols_Def_ਮੰਜ਼ਿਲ_Keymap(ਬਣਤਰ xkb_keymap *keymap, SymbolsInfo *info,
                       ਕੁੰਜੀ_Info *keyi)
{
    ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;
    ਸਮੂਹ_Info_2 *groupi;
    ਅਚਲ ਸਮੂਹ_Info_2 *group0;
    xkb_layout_ਸੂਚਕਾਂਕ_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ_By_Name(keymap, keyi->name, false);
    ਜੇ (!ਕੁੰਜੀ) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi));
        ਵਾਪਸ false;
    }

    /* Find the range of groups we need. */
    ਕੁੰਜੀ->ਅੰਕ_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ਅਚਲ bool has_explicit_type = (keyi->defined & ਕੁੰਜੀ_ਖੇਤਰ_DEFAULT_TYPE)
                                    || (groupi->defined & ਸਮੂਹ_ਖੇਤਰ_TYPE);
        ਜੇ (darray_ਮਾਪ(groupi->ਪੱਧਰ) > 0 || has_explicit_type)
            ਕੁੰਜੀ->ਅੰਕ_groups = i + 1;
        ਜੇ (has_explicit_type)
            ਕੁੰਜੀ->explicit |= EXPLICIT_TYPES;
    }

    ਜੇ (ਕੁੰਜੀ->ਅੰਕ_groups <= 0) {
        /* A key with no group may still have other fields defined */
        ਜੇ (keyi->defined)
            ਜਾਓ ਕੁੰਜੀ_fields;
        ਨਹੀਂਤਾਂ
            ਵਾਪਸ false;
    }

    darray_resize(keyi->groups, ਕੁੰਜੀ->ਅੰਕ_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_ਵਸਤੂ(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        ਜੇ (groupi->defined)
            ਜਾਰੀ;

        Copy_ਸਮੂਹ_Info(groupi, group0);
    }

    ਕੁੰਜੀ->groups = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਕੁੰਜੀ->ਅੰਕ_groups, ਆਕਾਰ(*ਕੁੰਜੀ->groups));
    ਪ੍ਰਤਿਜ੍ਞਾ(darray_ਮਾਪ(keyi->groups) == ਕੁੰਜੀ->ਅੰਕ_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < ਕੁੰਜੀ->ਅੰਕ_groups);
        #ਜੇ_ਅੰਤ
        bool explicit_type = false;
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type * ਅਚਲ type =
            ਲੱਭੋ_Type_For_ਸਮੂਹ(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        ਜੇ (type->ਅੰਕ_ਪੱਧਰ < darray_ਮਾਪ(groupi->ਪੱਧਰ)) {
            ਬਣਤਰ xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_ਪਾਠ(keymap->ctx, type->name), type->ਅੰਕ_ਪੱਧਰ,
                    ਕੁੰਜੀ_Info_ਪਾਠ(info, keyi),
                    darray_ਮਾਪ(groupi->ਪੱਧਰ));

            darray_foreach_from(leveli, groupi->ਪੱਧਰ, type->ਅੰਕ_ਪੱਧਰ)
                clear_level(leveli);
        }
        darray_resize0(groupi->ਪੱਧਰ, type->ਅੰਕ_ਪੱਧਰ);

        ਕੁੰਜੀ->groups[i].explicit_type = explicit_type;
        ਕੁੰਜੀ->groups[i].type = type;
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
        ਬਣਤਰ xkb_level *leveli;
        darray_foreach(leveli, groupi->ਪੱਧਰ) {
            ਚੋਣ (leveli->ਅੰਕ_syms) {
            ਹਾਲਤ 0:
                leveli->ਉੱਤਲਾ = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
                ਰੋਕੋ;
            ਹਾਲਤ 1:
                leveli->ਉੱਤਲਾ = xkb_keysym_ਮੰਜ਼ਿਲ_ਉੱਤਲਾ(leveli->s.sym);
                ਰੋਕੋ;
            ਮੂਲ:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_ਉੱਤਲਾ = false;
                ਲਈ (xkb_keysym_ਗਿਣਤੀ_t k = 0; k < leveli->ਅੰਕ_syms; k++) {
                    ਅਚਲ xkb_keysym_t ਉੱਤਲਾ =
                        xkb_keysym_ਮੰਜ਼ਿਲ_ਉੱਤਲਾ(leveli->s.syms[k]);
                    ਜੇ (ਉੱਤਲਾ != leveli->s.syms[k]) {
                        leveli->has_ਉੱਤਲਾ = true;
                        ਰੋਕੋ;
                    }
                }
                ਜੇ (leveli->has_ਉੱਤਲਾ) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (ਮਾਪ_t) 2 * leveli->ਅੰਕ_syms *
                                             ਆਕਾਰ(*leveli->s.syms));
                    ਜੇ (!leveli->s.syms)
                        ਵਾਪਸ false; /* FIXME: better handling? */
                    ਲਈ (xkb_keysym_ਗਿਣਤੀ_t k = 0; k < leveli->ਅੰਕ_syms; k++) {
                        leveli->s.syms[leveli->ਅੰਕ_syms + k] =
                            xkb_keysym_ਮੰਜ਼ਿਲ_ਉੱਤਲਾ(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->ਪੱਧਰ, &ਕੁੰਜੀ->groups[i].ਪੱਧਰ, ਨਿਰਾ);
        ਜੇ (ਕੁੰਜੀ->groups[i].type->ਅੰਕ_ਪੱਧਰ > 1 ||
            ਕੁੰਜੀ->groups[i].ਪੱਧਰ[0].ਅੰਕ_syms > 0) {
            ਕੁੰਜੀ->groups[i].explicit_symbols = true;
            ਕੁੰਜੀ->explicit |= EXPLICIT_SYMBOLS;
        }
        ਜੇ (groupi->defined & ਸਮੂਹ_ਖੇਤਰ_ACTS) { // FIXME
            ਕੁੰਜੀ->groups[i].explicit_actions = true;
            ਕੁੰਜੀ->explicit |= EXPLICIT_INTERP;
        }
        ਜੇ (ਕੁੰਜੀ->groups[i].explicit_type)
            ਕੁੰਜੀ->explicit |= EXPLICIT_TYPES;
    }

    ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_pending_ਸਮੂਹ = keyi->ਬਾਹਰਲਾ_ਦਾ_range_pending_ਸਮੂਹ;
    ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ = keyi->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ;
    ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy = keyi->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy;

ਕੁੰਜੀ_fields:
    ਜੇ (keyi->defined & ਕੁੰਜੀ_ਖੇਤਰ_VMODMAP) {
        ਕੁੰਜੀ->vmodmap = keyi->vmodmap;
        ਕੁੰਜੀ->explicit |= EXPLICIT_VMODMAP;
    }

    ਜੇ (keyi->repeat != ਕੁੰਜੀ_REPEAT_UNDEFINED) {
        ਕੁੰਜੀ->repeats = (keyi->repeat == ਕੁੰਜੀ_REPEAT_YES);
        ਕੁੰਜੀ->explicit |= EXPLICIT_REPEAT;
    }

    ਜੇ ((keyi->defined & ਕੁੰਜੀ_ਖੇਤਰ_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        ਕੁੰਜੀ->overlays = keyi->overlays;
        ਜੇ (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_ਨਕਾਬ_t ਬਚਿਆ = ਕੁੰਜੀ->overlays;
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ **overlays_ਕੁੰਜੀਆਂ = keyi->overlays_ਕੁੰਜੀਆਂ;
            ਜਦੋਂ (ਬਚਿਆ) {
                /* isolate lowest set bit */
                ਅਚਲ xkb_overlay_ਨਕਾਬ_t lsb = (xkb_overlay_ਨਕਾਬ_t)(
                    ਬਚਿਆ &
                    (xkb_overlay_ਨਕਾਬ_t)((ਬਿਨਾਨਿਸ਼ਾਨੀ)~ਬਚਿਆ + 1u)
                );
                ਬਚਿਆ = (ਬਚਿਆ & ~lsb);
                ਜੇ (*overlays_ਕੁੰਜੀਆਂ) {
                    overlays_ਕੁੰਜੀਆਂ++;
                } ਨਹੀਂਤਾਂ {
                    /* drop current null value (overlayN=none) */
                    ਕੁੰਜੀ->overlays &= ~lsb;
                }
            }

            ਜੇ (ਕੁੰਜੀ->overlays) {
                /* Steal */
                ਕੁੰਜੀ->overlays_ਕੁੰਜੀਆਂ = keyi->overlays_ਕੁੰਜੀਆਂ;
                keyi->overlays_ਕੁੰਜੀਆਂ = ਨਿਰਾ;
                keyi->overlays_alloc = 0;

                ਕੁੰਜੀ->overlays_inline = false;
                ਕੁੰਜੀ->explicit |= EXPLICIT_OVERLAY;
            }
        } ਨਹੀਂਤਾਂ {
            ਕੁੰਜੀ->overlay_ਕੁੰਜੀ = keyi->overlay_ਕੁੰਜੀ;
            ਕੁੰਜੀ->overlays_inline = true;
            ਕੁੰਜੀ->explicit |= EXPLICIT_OVERLAY;
        }
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
Copy_Mod_Map_Def_ਮੰਜ਼ਿਲ_Keymap(ਬਣਤਰ xkb_keymap *keymap, SymbolsInfo *info,
                      Mod_Map_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ)
{
    ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;

    ਜੇ (!ਇੰਦਰਾਜ->have_ਚਿੰਨ੍ਹ) {
        ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ_By_Name(keymap, ਇੰਦਰਾਜ->u.ਕੁੰਜੀ_Name, true);
        ਜੇ (!ਕੁੰਜੀ) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, ਇੰਦਰਾਜ->u.ਕੁੰਜੀ_Name),
                    Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_REAL, &info->mods, ਇੰਦਰਾਜ->mods));
            ਇੰਦਰਾਜ->mods = 0; /* disable */
            ਵਾਪਸ false;
        }
    }
    ਨਹੀਂਤਾਂ {
        ਕੁੰਜੀ = ਲੱਭੋ_ਕੁੰਜੀ_For_ਚਿੰਨ੍ਹ(keymap, ਇੰਦਰਾਜ->u.ਕੁੰਜੀ_Sym);
        ਜੇ (!ਕੁੰਜੀ) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_ਚਿੰਨ੍ਹ,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    Keysym_ਪਾਠ(info->ctx, ਇੰਦਰਾਜ->u.ਕੁੰਜੀ_Sym),
                    Mod_ਨਕਾਬ_ਪਾਠ(info->ctx, MOD_REAL, &info->mods, ਇੰਦਰਾਜ->mods));
            ਇੰਦਰਾਜ->mods = 0; /* disable */
            ਵਾਪਸ false;
        }
    }

    /* Skip modMap None */
    ਜੇ (ਇੰਦਰਾਜ->mods) {
        ਕੁੰਜੀ->modmap |= ਇੰਦਰਾਜ->mods;
        ਇੰਦਰਾਜ->ਕੁੰਜੀ_ਕੋਡ = ਕੁੰਜੀ->keycode;
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
Copy_Symbols_ਮੰਜ਼ਿਲ_Keymap(ਬਣਤਰ xkb_keymap *keymap, SymbolsInfo *info)
{
    ਕੁੰਜੀ_Info *keyi;

    keymap->symbols_section_name = strdup_ਸੁਰੱਖਿਅਤ(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->ਅੰਕ_ਸਮੂਹ_ਨਾਂ = (xkb_layout_ਸੂਚਕਾਂਕ_t)darray_ਮਾਪ(info->ਸਮੂਹ_ਨਾਂ);
    darray_steal(info->ਸਮੂਹ_ਨਾਂ, &keymap->ਸਮੂਹ_ਨਾਂ, ਨਿਰਾ);

    darray_foreach(keyi, info->ਕੁੰਜੀਆਂ)
        ਜੇ (!Copy_Symbols_Def_ਮੰਜ਼ਿਲ_Keymap(keymap, info, keyi))
            info->error_ਗਿਣਤੀ++;

    ਜੇ (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;

        xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap) {
            ਜੇ (ਕੁੰਜੀ->name == XKB_ATOM_NONE)
                ਜਾਰੀ;

            ਜੇ (ਕੁੰਜੀ->ਅੰਕ_groups < 1)
                log_info(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "No symbols defined for %s\n",
                         ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, ਕੁੰਜੀ->name));
        }
    }

    /* Resolve modmaps */
    Mod_Map_ਇੰਦਰਾਜ *mm;
    darray_foreach(mm, info->modmaps)
        ਜੇ (!Copy_Mod_Map_Def_ਮੰਜ਼ਿਲ_Keymap(keymap, info, mm))
            info->error_ਗਿਣਤੀ++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->ਅੰਕ_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    ਵਾਪਸ true;
}

bool
CompileSymbols(XkbFile *file, ਬਣਤਰ xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    ਜੇ (file !=ਨਿਰਾ)
        HandleSymbolsFile(&info, file);

    ਜੇ (info.error_ਗਿਣਤੀ != 0)
        ਜਾਓ err_info;

    ਜੇ (!Copy_Symbols_ਮੰਜ਼ਿਲ_Keymap(&keymap_info->keymap, &info))
        ਜਾਓ err_info;

    ClearSymbolsInfo(&info);
    ਵਾਪਸ true;

err_info:
    ClearSymbolsInfo(&info);
    ਵਾਪਸ false;
}
