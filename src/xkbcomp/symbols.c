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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-status.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-keysyms.h"

#அந்தர்பாவ "action.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "expr.h"
#அந்தர்பாவ "include.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "keysym.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "text.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "util-numbers.h"
#அந்தர்பாவ "util-mem.h"
#அந்தர்பாவ "vmod.h"
#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "xkbcomp/ast.h"

எண்ணிக்கை சாவி_repeat {
    சாவி_REPEAT_UNDEFINED = 0,
    சாவி_REPEAT_YES,
    சாவி_REPEAT_NO,
    சாவி_REPEAT_இலக்கம்_ENTRIES,
};
எண்ணிக்கை { சாவி_REPEAT_MIN_WIDTH = 3 /* 2 bits + sign */ };
static_assert(சாவி_REPEAT_இலக்கம்_ENTRIES - 1 <=
              (1 << (சாவி_REPEAT_MIN_WIDTH - 1)) - 1,
              "Cannot encode key repeat");

எண்ணிக்கை group_field {
    GROUP_FIELD_SYMS = (1 << 0),
    GROUP_FIELD_ACTS = (1 << 1),
    GROUP_FIELD_TYPE = (1 << 2),
};

எண்ணிக்கை சாவி_field {
    சாவி_FIELD_NONE = 0,
    சாவி_FIELD_REPEAT = (1 << 0),
    சாவி_FIELD_DEFAULT_TYPE = (1 << 1),
    சாவி_FIELD_GROUPINFO = (1 << 2),
    சாவி_FIELD_VMODMAP = (1 << 3),
    சாவி_FIELD_OVERLAY = (1 << 4),
    சாவி_FIELD_ALL = சாவி_FIELD_REPEAT | சாவி_FIELD_DEFAULT_TYPE
                  | சாவி_FIELD_GROUPINFO | சாவி_FIELD_VMODMAP | சாவி_FIELD_OVERLAY,
};
எண்ணிக்கை { சாவி_FIELD_MIN_WIDTH = 6 /* 5 bits + sign */ };
static_assert(சாவி_FIELD_ALL <= (1 << (சாவி_FIELD_MIN_WIDTH - 1)) - 1,
              "Cannot encode key field");

வகைவரை கட்டமைப்பு {
    darray(கட்டமைப்பு xkb_level) levels;
    எண்ணிக்கை group_field defined;
    xkb_atom_t type;
} GroupInfo;

எண்ணிக்கை {
    XKB_LAYOUT_OUT_OF_RANGE_POLICY_MIN_WIDTH = 4 /* 3 bits + sign */,
    KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD = 8,
    KEYINFO_DEFINED_BIT_FIELD = 16,
    KEYINFO_MERGE_BIT_FIELD = 8,
    KEYINFO_REPEAT_BIT_FIELD = 8,
};
static_assert(
    XKB_LAYOUT_OUT_OF_RANGE_WRAP < XKB_LAYOUT_OUT_OF_RANGE_CLAMP &&
    XKB_LAYOUT_OUT_OF_RANGE_CLAMP < XKB_LAYOUT_OUT_OF_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_OUT_OF_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_OUT_OF_RANGE_POLICY_MIN_WIDTH) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD >=
              XKB_LAYOUT_OUT_OF_RANGE_POLICY_MIN_WIDTH, "");
static_assert((unsigned)KEYINFO_DEFINED_BIT_FIELD >=
              (unsigned)சாவி_FIELD_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_MERGE_BIT_FIELD >=
              (unsigned)MERGE_MODE_MIN_WIDTH, "Cannot encode KeyInfo field");
static_assert((unsigned)KEYINFO_REPEAT_BIT_FIELD >=
              (unsigned)சாவி_REPEAT_MIN_WIDTH, "Cannot encode KeyInfo field");

வகைவரை கட்டமைப்பு {
    xkb_atom_t name;

    xkb_mod_mask_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_index_t out_of_range_group_எண்;
    darray(GroupInfo) groups;
    எண்ணிக்கை xkb_layout_out_of_range_policy
        out_of_range_group_policy:KEYINFO_OUT_OF_RANGE_GROUP_POLICY_BIT_FIELD;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    எண்ணிக்கை சாவி_field defined:KEYINFO_DEFINED_BIT_FIELD;
    எண்ணிக்கை merge_mode merge:KEYINFO_MERGE_BIT_FIELD;
    எண்ணிக்கை சாவி_repeat repeat:KEYINFO_REPEAT_BIT_FIELD;

    bool out_of_range_pending_group:1;

    /** If true, overlays in the mask are explicitly cleared */
    bool overlays_clear:1;
    /** Track the count of allocated slots of overlays_keys */
    xkb_overlay_index_t overlays_alloc;
    /** Overlays indices */
    xkb_overlay_mask_t overlays;
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    ஒன்றிப்பு {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        மாறிலி கட்டமைப்பு xkb_சாவி *overlay_சாவி;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        மாறிலி கட்டமைப்பு xkb_சாவி **overlays_சாவிகள்;
    };
} சாவி_Info;

நிலையான வெறுமை
StealLevelInfo(கட்டமைப்பு xkb_level *into, கட்டமைப்பு xkb_level *from)
{
    clear_level(into);
    எனில் (from->இலக்கம்_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } இல்லையெனில் {
        into->s.sym = from->s.sym;
    }
    into->இலக்கம்_syms = from->இலக்கம்_syms;
    from->இலக்கம்_syms = 0;

    எனில் (from->இலக்கம்_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } இல்லையெனில் {
        into->a.action = from->a.action;
    }
    into->இலக்கம்_actions = from->இலக்கம்_actions;
    from->இலக்கம்_actions = 0;
}

நிலையான வெறுமை
InitGroupInfo(GroupInfo *groupi)
{
    memset(groupi, 0, அளவு(*groupi));
}

நிலையான வெறுமை
ClearGroupInfo(GroupInfo *groupi)
{
    கட்டமைப்பு xkb_level *leveli;
    darray_foreach(leveli, groupi->levels)
        clear_level(leveli);
    darray_free(groupi->levels);
}

நிலையான வெறுமை
CopyGroupInfo(GroupInfo *to, மாறிலி GroupInfo *from)
{
    to->defined = from->defined;
    to->type = from->type;
    darray_init(to->levels);
    darray_copy(to->levels, from->levels);
    க்கு (xkb_level_index_t j = 0; j < darray_size(to->levels); j++) {
        எனில் (darray_item(from->levels, j).இலக்கம்_syms > 1) {
            darray_item(to->levels, j).s.syms =
                memdup(darray_item(from->levels, j).s.syms,
                       darray_item(from->levels, j).இலக்கம்_syms,
                       அளவு(xkb_keysym_t));
        }
        எனில் (darray_item(from->levels, j).இலக்கம்_actions > 1) {
            darray_item(to->levels, j).a.actions =
                memdup(darray_item(from->levels, j).a.actions,
                       darray_item(from->levels, j).இலக்கம்_actions,
                       அளவு(ஒன்றிப்பு xkb_action));
        }
    }
}

நிலையான வெறுமை
Init_சாவி_Info(கட்டமைப்பு xkb_context *ctx, சாவி_Info *keyi)
{
    memset(keyi, 0, அளவு(*keyi));
    static_assert(!DEFAULT_சாவி_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_சாவி_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_சாவி = வெற்று;
}

நிலையான வெறுமை
Clear_சாவி_Info(சாவி_Info *keyi)
{
    GroupInfo *groupi;
    darray_foreach(groupi, keyi->groups)
        ClearGroupInfo(groupi);
    darray_free(keyi->groups);
    எனில் (keyi->overlays_alloc)
        விடுவி(keyi->overlays_சாவிகள்);
}

/***====================================================================***/

வகைவரை கட்டமைப்பு {
    எழுத்து *name;         /* e.g. pc+us+inet(evdev) */
    முழு error_மொத்தம்;
    unsigned முழு include_depth;
    xkb_layout_index_t explicit_group;
    xkb_layout_index_t max_groups;
    darray(சாவி_Info) சாவிகள்;
    சாவி_Info default_சாவி;
    ActionsInfo default_actions;
    darray(xkb_atom_t) group_names;
    darray(ModMapEntry) modmaps;
    கட்டமைப்பு xkb_mod_set mods;

    கட்டமைப்பு xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info;
} SymbolsInfo;

நிலையான வெறுமை
InitSymbolsInfo(SymbolsInfo *info, மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                unsigned முழு include_depth, மாறிலி கட்டமைப்பு xkb_mod_set *mods)
{
    memset(info, 0, அளவு(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_depth = include_depth;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_சாவி_Info(keymap_info->keymap.ctx, &info->default_சாவி);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_depth > 0);
    info->explicit_group = XKB_LAYOUT_INVALID;
}

நிலையான வெறுமை
ClearSymbolsInfo(SymbolsInfo *info)
{
    சாவி_Info *keyi;
    விடுவி(info->name);
    darray_foreach(keyi, info->சாவிகள்)
        Clear_சாவி_Info(keyi);
    darray_free(info->சாவிகள்);
    darray_free(info->group_names);
    darray_free(info->modmaps);
    Clear_சாவி_Info(&info->default_சாவி);
}

நிலையான மாறிலி எழுத்து *
சாவி_Info_Text(SymbolsInfo *info, சாவி_Info *keyi)
{
    திரும்பு சாவி_Name_Text(info->ctx, keyi->name);
}

நிலையான bool
MergeGroups(SymbolsInfo *info, GroupInfo *into, GroupInfo *from, bool clobber,
            bool report, xkb_layout_index_t group, xkb_atom_t சாவி_name)
{
    /* First find the type of the merged group. */
    எனில் (into->type != from->type) {
        எனில் (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        இல்லையெனில் எனில் (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        இல்லையெனில் {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            எனில் (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_சாவி_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         group + 1, சாவி_Name_Text(info->ctx, சாவி_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & GROUP_FIELD_TYPE);

    /* Now look at the levels. */

    எனில் (darray_empty(from->levels)) {
        InitGroupInfo(from);
        திரும்பு true;
    }

    எனில் (darray_empty(into->levels)) {
        from->type = into->type;
        *into = *from;
        InitGroupInfo(from);
        திரும்பு true;
    }

    /* Merge the actions and syms. */
    மாறிலி darray_size_t levels_in_both =
        MIN(darray_size(into->levels), darray_size(from->levels));
    darray_size_t from_Keysyms_மொத்தம் = 0;
    darray_size_t from_Actions_மொத்தம் = 0;
    க்கு (darray_size_t i = 0; i < levels_in_both; i++) {
        கட்டமைப்பு xkb_level* மாறிலி intoLevel = &darray_item(into->levels, i);
        கட்டமைப்பு xkb_level* மாறிலி fromLevel = &darray_item(from->levels, i);

        மாறிலி bool fromHasNoKeysym = fromLevel->இலக்கம்_syms == 0;
        மாறிலி bool fromHasNoAction = fromLevel->இலக்கம்_actions == 0;
        எனில் (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            தொடர்;
        }

        மாறிலி bool intoHasNoKeysym = intoLevel->இலக்கம்_syms == 0;
        மாறிலி bool intoHasNoAction = intoLevel->இலக்கம்_actions == 0;
        எனில் (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_மொத்தம்++;
            from_Actions_மொத்தம்++;
            தொடர்;
        }

        /* Possible level conflict */
        ப்ரதிஜ்ஞா(intoLevel->இலக்கம்_syms > 0 || intoLevel->இலக்கம்_actions > 0);
        ப்ரதிஜ்ஞா(fromLevel->இலக்கம்_syms > 0 || fromLevel->இலக்கம்_actions > 0);

        /* Handle keysyms */
        எனில் (!XkbLevelsSameSyms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            எனில் (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_சாவி_குறியீடு,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, group + 1, சாவி_Name_Text(info->ctx, சாவி_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            எனில் (fromHasNoKeysym) {
                /* No keysym to copy */
            } இல்லையெனில் எனில் (clobber) {
                /* Override: copy any defined keysym from `from` */
                எனில் (unlikely(fromLevel->இலக்கம்_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    எனில் (unlikely(intoLevel->இலக்கம்_syms > 1))
                        விடுவி(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->இலக்கம்_syms = fromLevel->இலக்கம்_syms;
                    fromLevel->இலக்கம்_syms = 0;
                    from_Keysyms_மொத்தம்++;
                } இல்லையெனில் எனில் (fromLevel->s.sym != XKB_சாவி_No_குறியீடு) {
                    /* Single defined keysym */
                    எனில் (unlikely(intoLevel->இலக்கம்_syms > 1))
                        விடுவி(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->இலக்கம்_syms = 1;
                    from_Keysyms_மொத்தம்++;
                }
            } இல்லையெனில் {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                எனில் (unlikely(intoLevel->இலக்கம்_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } இல்லையெனில் எனில் (intoLevel->s.sym == XKB_சாவி_No_குறியீடு) {
                    /* Single undefined keysym */
                    எனில் (unlikely(fromLevel->இலக்கம்_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->இலக்கம்_syms = fromLevel->இலக்கம்_syms;
                        fromLevel->இலக்கம்_syms = 0;
                    } இல்லையெனில் {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->இலக்கம்_syms = 1;
                    }
                    from_Keysyms_மொத்தம்++;
                }
            }
        }

        /* Handle actions */
        எனில் (!XkbLevelsSameActions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            எனில் (report && !(intoHasNoAction || fromHasNoAction)) {
                எனில் (unlikely(intoLevel->இலக்கம்_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_சாவி_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, group + 1, சாவி_Name_Text(info->ctx, சாவி_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } இல்லையெனில் {
                    ஒன்றிப்பு xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_சாவி_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, group + 1,
                                சாவி_Name_Text(info->ctx, சாவி_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            எனில் (fromHasNoAction) {
                /* No action to copy */
            } இல்லையெனில் எனில் (clobber) {
                /* Override: copy any defined action from `from` */
                எனில் (unlikely(fromLevel->இலக்கம்_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    எனில் (unlikely(intoLevel->இலக்கம்_actions > 1))
                        விடுவி(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->இலக்கம்_actions = fromLevel->இலக்கம்_actions;
                    fromLevel->இலக்கம்_actions = 0;
                    from_Actions_மொத்தம்++;
                } இல்லையெனில் எனில் (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    எனில் (unlikely(intoLevel->இலக்கம்_actions > 1))
                        விடுவி(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->இலக்கம்_actions = 1;
                    from_Actions_மொத்தம்++;
                }
            } இல்லையெனில் {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                எனில் (unlikely(intoLevel->இலக்கம்_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } இல்லையெனில் எனில் (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    எனில் (unlikely(fromLevel->இலக்கம்_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->இலக்கம்_actions = fromLevel->இலக்கம்_actions;
                        fromLevel->இலக்கம்_actions = 0;
                    } இல்லையெனில் {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->இலக்கம்_actions = 1;
                    }
                    from_Actions_மொத்தம்++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    கட்டமைப்பு xkb_level *level;
    darray_foreach_from(level, from->levels, levels_in_both) {
        darray_append(into->levels, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->இலக்கம்_syms = 0;
        level->இலக்கம்_actions = 0;
        from_Keysyms_மொத்தம்++;
        from_Actions_மொத்தம்++;
    }
    எனில் (from_Keysyms_மொத்தம்) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        எனில் (from_Keysyms_மொத்தம் == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_SYMS;
        into->defined |= (from->defined & GROUP_FIELD_SYMS);
    }
    எனில் (from_Actions_மொத்தம்) {
        /* Reset defined actions field if we used no action from `into` */
        எனில் (from_Actions_மொத்தம் == darray_size(into->levels))
            into->defined &= ~GROUP_FIELD_ACTS;
        into->defined |= (from->defined & GROUP_FIELD_ACTS);
    }

    திரும்பு true;
}

நிலையான bool
Use_New_சாவி_Field(எண்ணிக்கை சாவி_field field, எண்ணிக்கை சாவி_field old, எண்ணிக்கை சாவி_field new,
               bool clobber, bool report, எண்ணிக்கை சாவி_field *collide)
{
    எனில் (!(old & field))
        திரும்பு (new & field);

    எனில் (new & field) {
        எனில் (report)
            *collide |= field;

        திரும்பு clobber;
    }

    திரும்பு false;
}

நிலையான bool
overlays_get(மாறிலி சாவி_Info *info, xkb_overlay_index_t bit,
             மாறிலி கட்டமைப்பு xkb_சாவி **சாவி_out)
{
    எனில் (bit >= (xkb_overlay_index_t)(அளவு(xkb_overlay_mask_t) * CHAR_BIT))
        திரும்பு false;
    மாறிலி xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);
    எனில் (!(info->overlays & mask)) திரும்பு false;
    எனில் (சாவி_out) {
        எனில் (!info->overlays_alloc) {
            /* Stored inline */
            *சாவி_out = info->overlay_சாவி;
        } இல்லையெனில் {
            /* Stored in sparse array */
            மாறிலி xkb_overlay_mask_t கீழ் = (xkb_overlay_mask_t)(
                info->overlays & (mask - 1u)
            );
            மாறிலி xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(கீழ்);
            *சாவி_out = info->overlays_சாவிகள்[index];
        }
    }
    திரும்பு true;
}

நிலையான bool
overlays_செருகு(சாவி_Info *keyi, xkb_overlay_index_t bit,
                மாறிலி கட்டமைப்பு xkb_சாவி *சாவி)
{
    எனில் (bit >= (xkb_overlay_index_t)(அளவு(xkb_overlay_mask_t) * CHAR_BIT))
        திரும்பு false;

    மாறிலி xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << bit);

    எனில் ((keyi->overlays & mask) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        எனில் (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_சாவி = சாவி;
            எனில் (!சாவி)
                keyi->overlays_clear = true;
        } இல்லையெனில் {
            /* Stored in sparse array */
            மாறிலி xkb_overlay_mask_t கீழ் = (xkb_overlay_mask_t)(
                keyi->overlays & (xkb_overlay_mask_t)(mask - 1u)
            );
            /* index = number of set bits strictly below bit */
            மாறிலி xkb_overlay_index_t index =
                (xkb_overlay_index_t)popcount32(கீழ்);
            keyi->overlays_சாவிகள்[index] = சாவி;
        }
        திரும்பு true;
    }

    /* Insert new element */
    எனில் (!keyi->overlays || (keyi->overlays_clear && !சாவி)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_சாவி = சாவி;
        keyi->overlays |= mask;
        keyi->overlays_clear = !சாவி;
    } இல்லையெனில் எனில் (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        மாறிலி xkb_overlay_mask_t overlays = keyi->overlays | mask;
        மாறிலி xkb_overlay_index_t alloc =
            (xkb_overlay_index_t)popcount32(overlays);

        /* Alloc sparse array */
        மாறிலி கட்டமைப்பு xkb_சாவி ** மாறிலி tmp = calloc(alloc, அளவு(*tmp));
        எனில் (!tmp) திரும்பு false;

        /* Copy previous entries to the buffer */
        எனில் (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            மாறிலி xkb_overlay_mask_t கீழ் = (xkb_overlay_mask_t)(
                overlays & (xkb_overlay_mask_t)(keyi->overlays - 1u)
            );
            மாறிலி xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(கீழ்);
            tmp[idx] = keyi->overlay_சாவி;
        } இல்லையெனில் {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        மாறிலி xkb_overlay_mask_t கீழ் = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        மாறிலி xkb_overlay_index_t idx = (xkb_overlay_index_t)popcount32(கீழ்);
        tmp[idx] = சாவி;

        keyi->overlays_சாவிகள் = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } இல்லையெனில் {
        /* Allocated items: grow heap and shift if relevant */
        மாறிலி xkb_overlay_mask_t overlays =
            (xkb_overlay_mask_t)(keyi->overlays | mask);
        மாறிலி xkb_overlay_index_t மொத்தம் =
            (xkb_overlay_index_t)popcount32(overlays);
        எனில் (மொத்தம் > keyi->overlays_alloc) {
            மாறிலி xkb_overlay_index_t alloc =
                (xkb_overlay_index_t)next_pow2(மொத்தம்);
            ப்ரதிஜ்ஞா(alloc >= மொத்தம்);
            மாறிலி கட்டமைப்பு xkb_சாவி ** மாறிலி tmp = realloc(
                keyi->overlays_சாவிகள்,
                (size_t)alloc * அளவு(*keyi->overlays_சாவிகள்)
            );
            எனில் (!tmp) திரும்பு false;
            keyi->overlays_சாவிகள் = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        மாறிலி xkb_overlay_mask_t கீழ் = (xkb_overlay_mask_t)(
            overlays & (xkb_overlay_mask_t)(mask - 1u)
        );
        மாறிலி xkb_overlay_index_t index = (xkb_overlay_index_t)popcount32(கீழ்);
        எனில் (index >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        எனில் (index < மொத்தம் - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            ப்ரதிஜ்ஞா(index + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_சாவிகள் + index + 1, keyi->overlays_சாவிகள் + index,
                    (size_t)(மொத்தம் - 1u - index) * அளவு(மாறிலி கட்டமைப்பு xkb_சாவி *));
        }
        keyi->overlays_சாவிகள்[index] = சாவி;
        keyi->overlays = overlays;
    }

    திரும்பு true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
நிலையான bool
merge_overlays(SymbolsInfo *info, சாவி_Info *into, சாவி_Info *from,
               bool clobber, bool report, எண்ணிக்கை சாவி_field *collide)
{
    எனில் (!(from->defined & சாவி_FIELD_OVERLAY)) {
        /* `from` is empty: do nothing */
    } இல்லையெனில் எனில் (!(into->defined & சாவி_FIELD_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_சாவிகள் = from->overlays_சாவிகள்;
        from->overlays_சாவிகள் = வெற்று;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= சாவி_FIELD_OVERLAY;
    } இல்லையெனில் எனில் (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } இல்லையெனில் {
        எனில் (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            மாறிலி xkb_overlay_mask_t result_mask =
                (into->overlays | from->overlays);
            மாறிலி xkb_overlay_index_t மொத்தம் =
                (xkb_overlay_index_t)popcount32(result_mask);

            எனில் (மொத்தம் == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            சாவி_Info *dest = into;
            சாவி_Info *src = from;
            எனில் (into->overlays_alloc >= மொத்தம் &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } இல்லையெனில் எனில் (from->overlays_alloc >= மொத்தம்) {
                dest = from;
                src = into;
                clobber = !clobber;
            } இல்லையெனில் எனில் (மொத்தம் > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                எனில் (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                எனில் (!dest->overlays_alloc) {
                    மாறிலி கட்டமைப்பு xkb_சாவி **tmp = calloc(மொத்தம், அளவு(*tmp));
                    எனில் (!tmp)
                        திரும்பு false;
                    எனில் (dest->overlay_சாவி)
                        tmp[0] = dest->overlay_சாவி;
                    dest->overlays_சாவிகள் = tmp;
                    dest->overlays_alloc = மொத்தம்;
                } இல்லையெனில் {
                    மாறிலி கட்டமைப்பு xkb_சாவி **tmp =
                        realloc(dest->overlays_சாவிகள்,
                                மொத்தம் * அளவு(*dest->overlays_சாவிகள்));
                    எனில் (!tmp)
                        திரும்பு false;
                    dest->overlays_சாவிகள் = tmp;
                    dest->overlays_alloc = மொத்தம்;
                }
            }

            xkb_overlay_mask_t remaining = src->overlays;
            மாறிலி கட்டமைப்பு xkb_சாவி **src_சாவிகள் = (src->overlays_clear)
                ? வெற்று
                : (src->overlays_alloc)
                    ? src->overlays_சாவிகள்
                    : &src->overlay_சாவி;
            போது (remaining) {
                /* isolate lowest set bit */
                மாறிலி xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                /* get its index */
                மாறிலி xkb_overlay_index_t bit =
                    (xkb_overlay_index_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                எனில் (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                மாறிலி கட்டமைப்பு xkb_சாவி *src_சாவி = (src_சாவிகள்)
                    ? *(src_சாவிகள்++)
                    : வெற்று;

                /* check dest */
                மாறிலி கட்டமைப்பு xkb_சாவி *dest_சாவி = வெற்று;
                மாறிலி bool conflict = overlays_get(dest, bit, &dest_சாவி);
                எனில் (conflict) {
                    எனில் (dest_சாவி == src_சாவி)
                       தொடர்;
                    இல்லையெனில் எனில் (report)
                        *collide |= சாவி_FIELD_OVERLAY;
                }

                எனில் ((!conflict || clobber) && !overlays_செருகு(dest, bit, src_சாவி))
                    திரும்பு false;
            }

            எனில் (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                எனில் (into->overlays_alloc)
                    விடுவி(into->overlays_சாவிகள்);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                எனில் (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_சாவிகள் = from->overlays_சாவிகள்;
                    from->overlays_சாவிகள் = வெற்று;
                    from->overlays_alloc = 0;
                } இல்லையெனில் {
                    into->overlay_சாவி = from->overlay_சாவி;
                }
            }
        } இல்லையெனில் {
            /* Disjoint overlays */

            எனில் (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_சாவி == from->overlay_சாவி) {
                /* Indentical: do nothing */
                திரும்பு true;
            }
            எனில் (!(into->overlays & from->overlays)) {
                எனில் (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_சாவி = from->overlay_சாவி;
                    திரும்பு true;
                } இல்லையெனில் எனில் (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    திரும்பு true;
                }
            }
            /* Conflict */
            எனில் (report)
                *collide |= சாவி_FIELD_OVERLAY;
            எனில் (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_சாவி = from->overlay_சாவி;
            }
        }
    }
    திரும்பு true;
}

நிலையான bool
Merge_சாவிகள்(SymbolsInfo *info, சாவி_Info *into, சாவி_Info *from, bool same_file)
{
    xkb_layout_index_t i;
    xkb_layout_index_t groups_in_both;
    எண்ணிக்கை சாவி_field collide = 0;
    மாறிலி முழு verbosity = xkb_context_get_log_verbosity(info->ctx);
    மாறிலி bool clobber = (from->merge != MERGE_AUGMENT);
    மாறிலி bool report = (same_file && verbosity > 0) || verbosity > 9;

    எனில் (from->merge == MERGE_REPLACE) {
        Clear_சாவி_Info(into);
        *into = *from;
        Init_சாவி_Info(info->ctx, from);
        திரும்பு true;
    }

    groups_in_both = MIN(darray_size(into->groups), darray_size(from->groups));
    க்கு (i = 0; i < groups_in_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    க்கு (i = groups_in_both; i < darray_size(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        InitGroupInfo(&darray_item(from->groups, i));
    }

    எனில் (Use_New_சாவி_Field(சாவி_FIELD_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= சாவி_FIELD_VMODMAP;
    }
    எனில் (Use_New_சாவி_Field(சாவி_FIELD_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= சாவி_FIELD_REPEAT;
    }
    எனில் (Use_New_சாவி_Field(சாவி_FIELD_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= சாவி_FIELD_DEFAULT_TYPE;
    }
    எனில் (Use_New_சாவி_Field(சாவி_FIELD_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->out_of_range_pending_group = from->out_of_range_pending_group;
        into->out_of_range_group_policy = from->out_of_range_group_policy;
        into->out_of_range_group_எண் = from->out_of_range_group_எண்;
        into->defined |= சாவி_FIELD_GROUPINFO;
    }

    எனில் (!merge_overlays(info, into, from, clobber, report, &collide))
        திரும்பு false;

    எனில் (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_சாவி_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 சாவி_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_சாவி_Info(from);
    Init_சாவி_Info(info->ctx, from);
    திரும்பு true;
}

நிலையான xkb_atom_t
Xkb_Resolve_சாவி_Alias(மாறிலி கட்டமைப்பு xkb_keymap *keymap, xkb_atom_t name)
{
    எனில் (name < keymap->இலக்கம்_சாவி_names) {
        மாறிலி KeycodeMatch match = keymap->சாவி_names[name];
        எனில் (match.found && match.is_alias) {
            திரும்பு match.alias.real;
        }
    }
    திரும்பு name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
நிலையான bool
Add_சாவி_Symbols(SymbolsInfo *info, சாவி_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_சாவி_Alias(&info->keymap_info->keymap, keyi->name);

    சாவி_Info *iter;
    darray_foreach(iter, info->சாவிகள்)
        எனில் (iter->name == keyi->name)
            திரும்பு Merge_சாவிகள்(info, iter, keyi, same_file);

    darray_append(info->சாவிகள், *keyi);
    Init_சாவி_Info(info->ctx, keyi);
    திரும்பு true;
}

நிலையான bool
AddModMapEntry(SymbolsInfo *info, மாறிலி ModMapEntry *new)
{
    மாறிலி bool clobber = (new->merge != MERGE_AUGMENT);

    ModMapEntry *old;
    darray_foreach(old, info->modmaps) {
        எனில் ((new->have_குறியீடு != old->have_குறியீடு) ||
            (new->have_குறியீடு && new->u.சாவி_Sym != old->u.சாவி_Sym) ||
            (!new->have_குறியீடு && new->u.சாவி_Name != old->u.சாவி_Name))
            தொடர்;

        எனில் (new->mods == old->mods)
            திரும்பு true;

        மாறிலி xkb_mod_mask_t use = (clobber ? new->mods : old->mods);
        மாறிலி xkb_mod_mask_t ignore = (clobber ? old->mods : new->mods);

        எனில் (new->have_குறியீடு) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.சாவி_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        } இல்லையெனில் {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    சாவி_Name_Text(info->ctx, new->u.சாவி_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, use),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        திரும்பு true;
    }

    darray_append(info->modmaps, *new);

    திரும்பு true;
}

/***====================================================================***/

நிலையான வெறுமை
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     எண்ணிக்கை merge_mode merge)
{
    xkb_atom_t *group_name;
    xkb_layout_index_t group_names_in_both;

    எனில் (from->error_மொத்தம் > 0) {
        into->error_மொத்தம் += from->error_மொத்தம்;
        திரும்பு;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    எனில் (into->name == வெற்று) {
        into->name = steal(&from->name);
    }

    group_names_in_both = MIN(darray_size(into->group_names),
                              darray_size(from->group_names));
    க்கு (xkb_layout_index_t i = 0; i < group_names_in_both; i++) {
        எனில் (!darray_item(from->group_names, i))
            தொடர்;

        எனில் (merge == MERGE_AUGMENT && darray_item(into->group_names, i))
            தொடர்;

        darray_item(into->group_names, i) = darray_item(from->group_names, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(group_name, from->group_names, group_names_in_both)
        darray_append(into->group_names, *group_name);

    எனில் (darray_empty(into->சாவிகள்)) {
        into->சாவிகள் = from->சாவிகள்;
        darray_init(from->சாவிகள்);
    }
    இல்லையெனில் {
        சாவி_Info *keyi;
        darray_foreach(keyi, from->சாவிகள்) {
            keyi->merge = merge;
            எனில் (!Add_சாவி_Symbols(into, keyi, false))
                into->error_மொத்தம்++;
        }
    }

    எனில் (darray_empty(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    இல்லையெனில் {
        ModMapEntry *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            எனில் (!AddModMapEntry(into, mm))
                into->error_மொத்தம்++;
        }
    }
}

நிலையான வெறுமை
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

நிலையான bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    எனில் (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_மொத்தம் += 10;
        திரும்பு false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_depth + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    மாறிலி கட்டமைப்பு parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    க்கு (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        SymbolsInfo next_incl;
        XkbFile *file;

        எழுத்து path[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  path, அளவு(path));
        எனில் (!file) {
            info->error_மொத்தம் += 10;
            ClearSymbolsInfo(&included);
            திரும்பு false;
        }

        InitSymbolsInfo(&next_incl, info->keymap_info, info->include_depth + 1,
                        &included.mods);
        எனில் (stmt->modifier) {
            மாறிலி முழு மொத்தம் = parse_dec_to_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &next_incl.explicit_group);
            எனில் (மொத்தம் <= 0 || *(stmt->modifier + மொத்தம்) != '\0' ||
                next_incl.explicit_group == 0 ||
                next_incl.explicit_group > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                next_incl.explicit_group = info->explicit_group;
            } இல்லையெனில் {
                next_incl.explicit_group--;
            }
        }
        இல்லையெனில் எனில் (info->keymap_info->keymap.இலக்கம்_groups != 0 &&
                 next_incl.include_depth == 1) {
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
            next_incl.explicit_group = 0;
        }
        இல்லையெனில் {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            next_incl.explicit_group = info->explicit_group;
        }

        HandleSymbolsFile(&next_incl, file);

        MergeIncludedSymbols(&included, &next_incl, stmt->merge);

        ClearSymbolsInfo(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedSymbols(info, &included, include->merge);
    ClearSymbolsInfo(&included);

    திரும்பு (info->error_மொத்தம் == 0);
}

நிலையான bool
GetGroupIndex(SymbolsInfo *info, சாவி_Info *keyi, கோவை_Def *arrayNdx,
              எண்ணிக்கை group_field field, xkb_layout_index_t *ndx_rtrn)
{
    ப்ரதிஜ்ஞா (field == GROUP_FIELD_SYMS || field == GROUP_FIELD_ACTS);
    மாறிலி எழுத்து *name = (field == GROUP_FIELD_SYMS ? "symbols" : "actions");

    எனில் (arrayNdx == வெற்று) {
        xkb_layout_index_t i = 0;
        GroupInfo *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            எனில் (!(groupi->defined & field)) {
                *ndx_rtrn = i;
                திரும்பு true;
            }
        }

        எனில் (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, சாவி_Info_Text(info, keyi), info->max_groups, name);
            திரும்பு false;
        }

        darray_resize0(keyi->groups, darray_size(keyi->groups) + 1);
        *ndx_rtrn = darray_size(keyi->groups) - 1;
        திரும்பு true;
    }

    எனில் (கோவை_Resolve_Group(info->keymap_info, arrayNdx, false, ndx_rtrn, வெற்று) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, சாவி_Info_Text(info, keyi));
        திரும்பு false;
    }

    (*ndx_rtrn)--;
    எனில் (*ndx_rtrn >= darray_size(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    திரும்பு true;
}

நிலையான bool
Add_Symbols_To_சாவி(SymbolsInfo *info, சாவி_Info *keyi, கோவை_Def *arrayNdx,
                கோவை_Def *மதிப்பு)
{
    xkb_layout_index_t ndx = 0;

    எனில் (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_SYMS, &ndx))
        திரும்பு false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    எனில் (மதிப்பு->common.type == STMT_கோவை_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_SYMS;
        திரும்பு true;
    }

    எனில் (மதிப்பு->common.type != STMT_கோவை_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_to_string(மதிப்பு->common.type), ndx + 1,
                சாவி_Info_Text(info, keyi));
        திரும்பு false;
    }

    எனில் (groupi->defined & GROUP_FIELD_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_சாவி_SYMBOLS_ENTRY,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                சாவி_Info_Text(info, keyi), ndx + 1);
        திரும்பு false;
    }

    xkb_level_index_t nLevels = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    க்கு (கோவை_Keysym_List *keysymList = (கோவை_Keysym_List *) மதிப்பு;
         keysymList;
         keysymList = (கோவை_Keysym_List *) keysymList->common.next) {
        nLevels++;
        /* Drop trailing NoSymbol */
        எனில் (darray_size(keysymList->syms) > 0)
            nonEmptyLevels = nLevels;
    }
    எனில் (nonEmptyLevels < nLevels)
        nLevels = nonEmptyLevels;
    எனில் (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_SYMS;

    xkb_level_index_t level = 0;
    க்கு (கோவை_Keysym_List *keysymList = (கோவை_Keysym_List *) மதிப்பு;
         keysymList && level < nLevels;
         keysymList = (கோவை_Keysym_List *) keysymList->common.next, level++) {
        கட்டமைப்பு xkb_level *leveli = &darray_item(groupi->levels, level);
        ப்ரதிஜ்ஞா(leveli->இலக்கம்_syms == 0);

        எனில் (unlikely(darray_size(keysymList->syms) > MAX_KEYSYMS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    சாவி_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_PER_LEVEL, darray_size(keysymList->syms));
            திரும்பு false;
        }

        leveli->இலக்கம்_syms = (xkb_keysym_மொத்தம்_t) darray_size(keysymList->syms);
        தேர்வு (leveli->இலக்கம்_syms) {
        நிலை 0:
            leveli->s.sym = XKB_சாவி_No_குறியீடு;
            நிறுத்து;
        நிலை 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            ப்ரதிஜ்ஞா(leveli->s.sym != XKB_சாவி_No_குறியீடு);
            நிறுத்து;
        இயல்பு:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, வெற்று);
#ifndef NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            க்கு (xkb_keysym_மொத்தம்_t k = 0; k < leveli->இலக்கம்_syms; k++)
                ப்ரதிஜ்ஞா(leveli->s.syms[k] != XKB_சாவி_No_குறியீடு);
#endif
        }
    }

    திரும்பு true;
}

நிலையான bool
Add_Actions_To_சாவி(SymbolsInfo *info, சாவி_Info *keyi, கோவை_Def *arrayNdx,
                கோவை_Def *மதிப்பு)
{
    xkb_layout_index_t ndx = 0;

    எனில் (!GetGroupIndex(info, keyi, arrayNdx, GROUP_FIELD_ACTS, &ndx))
        திரும்பு false;

    GroupInfo *groupi = &darray_item(keyi->groups, ndx);

    எனில் (மதிப்பு->common.type == STMT_கோவை_EMPTY_LIST) {
        groupi->defined |= GROUP_FIELD_ACTS;
        திரும்பு true;
    }

    எனில் (மதிப்பு->common.type != STMT_கோவை_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 மதிப்பு->common.type, ndx, சாவி_Info_Text(info, keyi));
        திரும்பு false;
    }

    எனில் (groupi->defined & GROUP_FIELD_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_சாவி_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 சாவி_Info_Text(info, keyi), ndx);
        திரும்பு false;
    }

    xkb_level_index_t nLevels = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    க்கு (ParseCommon *p = &மதிப்பு->common; p; p = p->next)
        nLevels++;
    எனில் (darray_size(groupi->levels) < nLevels)
        darray_resize0(groupi->levels, nLevels);

    groupi->defined |= GROUP_FIELD_ACTS;

    xkb_level_index_t level = 0;
    xkb_level_index_t nonEmptyLevels = 0;
    க்கு (கோவை_Action_List *actionList = (கோவை_Action_List *) மதிப்பு;
         actionList;
         actionList = (கோவை_Action_List *) actionList->common.next, level++) {
        கட்டமைப்பு xkb_level *leveli = &darray_item(groupi->levels, level);
        ப்ரதிஜ்ஞா(leveli->இலக்கம்_actions == 0);

        unsigned முழு இலக்கம்_actions = 0;
        க்கு (கோவை_Def *act = actionList->actions;
             act; act = (கோவை_Def *) act->common.next)
            இலக்கம்_actions++;

        எனில் (unlikely(இலக்கம்_actions > MAX_ACTIONS_PER_LEVEL)) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    சாவி_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_PER_LEVEL, இலக்கம்_actions);
            திரும்பு false;
        }

        /* Parse actions and add only defined actions */
        darray(ஒன்றிப்பு xkb_action) actions = darray_new();
        க்கு (கோவை_Def *act = actionList->actions;
             act; act = (கோவை_Def *) act->common.next) {
            ஒன்றிப்பு xkb_action toAct = { 0 };
            மாறிலி எண்ணிக்கை xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &toAct);
            எனில் (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_மதிப்பு,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        சாவி_Info_Text(info, keyi), ndx + 1, level + 1);
                எனில் (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    திரும்பு false;
                } இல்லையெனில் {
                    /* Ensure action type is reset */
                    toAct.type = ACTION_TYPE_NONE;
                }
            }
            எனில் (toAct.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } இல்லையெனில் எனில் (likely(இலக்கம்_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->இலக்கம்_actions = 1;
                leveli->a.action = toAct;
                செல் next_level;
            } இல்லையெனில் {
                darray_append(actions, toAct);
            }
        }
        எனில் (darray_empty(actions)) {
            leveli->இலக்கம்_actions = 0;
        } இல்லையெனில் எனில் (likely(darray_size(actions) > 1)) {
            leveli->இலக்கம்_actions = (xkb_action_மொத்தம்_t) darray_size(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, வெற்று);
#ifndef NDEBUG
            /* Canonical list: all NoAction() were dropped */
            க்கு (xkb_action_மொத்தம்_t k = 0; k < leveli->இலக்கம்_actions; k++)
                ப்ரதிஜ்ஞா(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#endif
        } இல்லையெனில் {
            /* Unlikely: some actions were dropped and only one remains */
            ப்ரதிஜ்ஞா(இலக்கம்_actions > darray_size(actions));
            leveli->இலக்கம்_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            ப்ரதிஜ்ஞா(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
next_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        எனில் (leveli->இலக்கம்_actions > 0 || leveli->இலக்கம்_syms > 0)
            nonEmptyLevels = level + 1;
    }

    எனில் (nonEmptyLevels < nLevels) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_size(groupi->levels) = nonEmptyLevels;
        எனில் (nonEmptyLevels > 0)
            darray_shrink(groupi->levels);
        இல்லையெனில்
            darray_free(groupi->levels);
    }

    திரும்பு true;
}

நிலையான மாறிலி LookupEntry repeatEntries[] = {
    { "true", சாவி_REPEAT_YES },
    { "yes", சாவி_REPEAT_YES },
    { "on", சாவி_REPEAT_YES },
    { "false", சாவி_REPEAT_NO },
    { "no", சாவி_REPEAT_NO },
    { "off", சாவி_REPEAT_NO },
    { "default", சாவி_REPEAT_UNDEFINED },
    { வெற்று, 0 }
};

/** Handle overlay<N> = <key> entry */
நிலையான bool
கோவை_Resolve_Overlay_Entry(மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                        மாறிலி எழுத்து *field, மாறிலி கோவை_Def *arrayNdx,
                        மாறிலி கோவை_Def *கோவை, சாவி_Info *keyi,
                        xkb_overlay_index_t *overlay_rtrn,
                        மாறிலி கட்டமைப்பு xkb_சாவி **சாவி_rtrn)
{
    /* Overlay index */
    எனில் (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                field, சாவி_Name_Text(keymap_info->keymap.ctx, keyi->name));
        திரும்பு false;
    }
    static_assert(அளவு(xkb_overlay_index_t) == அளவு(uint8_t), "");
    மாறிலி size_t prefix = அளவு("overlay") - 1;
    மாறிலி size_t len = strlen(field + prefix);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    எனில் (parse_dec_to_uint64_t(field + prefix, len, (uint64_t *)&raw_overlay)
        != (முழு)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                field, சாவி_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        திரும்பு false;
    }
    *overlay_rtrn = (xkb_overlay_index_t)raw_overlay - 1;

    /* Overlay key */
    தேர்வு (கோவை->common.type) {
    நிலை STMT_கோவை_KEYNAME_LITERAL:
        *சாவி_rtrn = Xkb_சாவி_By_Name(&keymap_info->keymap, கோவை->சாவி_name.சாவி_name,
                                 false);
        எனில் (!*சாவி_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, கோவை->சாவி_name.சாவி_name),
                    field, சாவி_Name_Text(keymap_info->keymap.ctx, keyi->name));
            திரும்பு false;
        }
        திரும்பு true;
    நிலை STMT_கோவை_IDENT: {
        மாறிலி எழுத்து * மாறிலி id =
            xkb_atom_text(keymap_info->keymap.ctx, கோவை->ident.ident);
        எனில் (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *சாவி_rtrn = வெற்று;
            திரும்பு true;
        } இல்லையெனில் எனில் (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *சாவி_rtrn = வெற்று;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            திரும்பு true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_மதிப்பு,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, field, சாவி_Name_Text(keymap_info->keymap.ctx, keyi->name));
        திரும்பு false;
    }
    இயல்பு:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_மதிப்பு,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_to_string(STMT_கோவை_KEYNAME_LITERAL),
                field, சாவி_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_to_string(கோவை->common.type));
        திரும்பு false;
    }
}

நிலையான bool
SetSymbolsField(SymbolsInfo *info, சாவி_Info *keyi, மாறிலி எழுத்து *field,
                கோவை_Def *arrayNdx, கோவை_Def **மதிப்பு_ptr)
{
    கோவை_Def * restrict மாறிலி மதிப்பு = *மதிப்பு_ptr;
    எனில் (istreq(field, "type")) {
        xkb_layout_index_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        எனில் (!கோவை_Resolve_String(info->ctx, மதிப்பு, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            திரும்பு false;
        }

        எனில் (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= சாவி_FIELD_DEFAULT_TYPE;
        }
        இல்லையெனில் எனில் (கோவை_Resolve_Group(info->keymap_info, arrayNdx, false,
                                  &ndx, வெற்று) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    சாவி_Info_Text(info, keyi));
            திரும்பு false;
        }
        இல்லையெனில் {
            ndx--;
            எனில் (ndx >= darray_size(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                ப்ரதிஜ்ஞா(darray_size(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= GROUP_FIELD_TYPE;
        }
    }
    இல்லையெனில் எனில் (istreq(field, "symbols")) {
        திரும்பு Add_Symbols_To_சாவி(info, keyi, arrayNdx, மதிப்பு);
    }
    இல்லையெனில் எனில் (istreq(field, "actions")) {
        திரும்பு Add_Actions_To_சாவி(info, keyi, arrayNdx, மதிப்பு);
    }
    இல்லையெனில் எனில் (istreq(field, "vmods") ||
             istreq(field, "virtualmods") ||
             istreq(field, "virtualmodifiers")) {
        xkb_mod_mask_t mask = 0;

        எனில் (!கோவை_Resolve_Mod_Mask(info->ctx, மதிப்பு, MOD_VIRT, &info->mods,
                                &mask)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_MASK,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_to_string(மதிப்பு->common.type),
                    சாவி_Info_Text(info, keyi));
            திரும்பு false;
        }

        keyi->vmodmap = mask;
        keyi->defined |= சாவி_FIELD_VMODMAP;
    }
    இல்லையெனில் எனில் (istreq(field, "locking") ||
             istreq(field, "lock") ||
             istreq(field, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                சாவி_Info_Text(info, keyi));
    }
    இல்லையெனில் எனில் (istreq(field, "radiogroup") ||
             istreq(field, "permanentradiogroup") ||
             istreq(field, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                சாவி_Info_Text(info, keyi));
    }
    இல்லையெனில் எனில் (istreq_prefix("permanentoverlay", field)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                சாவி_Info_Text(info, keyi));
    }
    இல்லையெனில் எனில் (istreq_prefix("overlay", field)) {
        xkb_overlay_index_t overlay = XKB_OVERLAY_INVALID;
        மாறிலி கட்டமைப்பு xkb_சாவி *சாவி = வெற்று;
        எனில் (!கோவை_Resolve_Overlay_Entry(info->keymap_info, field, arrayNdx,
                                     *மதிப்பு_ptr, keyi, &overlay, &சாவி))
            திரும்பு false;
        எனில் (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            திரும்பு true;
        } இல்லையெனில் எனில் (சாவி && சாவி->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_MESSAGE_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, சாவி_Info_Text(info, keyi));
        } இல்லையெனில் {
            மாறிலி கட்டமைப்பு xkb_சாவி *prev;
            எனில் (overlays_get(keyi, overlay, &prev)) {
                /* Duplicate entry */
                எனில் (சாவி != prev) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_சாவி_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             சாவி_Info_Text(info, keyi), overlay + 1,
                             (prev
                                 ? சாவி_Name_Text(info->ctx, prev->name)
                                 : "none"),
                             overlay + 1,
                             (சாவி
                                 ? சாவி_Name_Text(info->ctx, சாவி->name)
                                 : "none"));
                }
            } இல்லையெனில் எனில் (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                எனில் (!overlays_செருகு(keyi, overlay, சாவி))
                    திரும்பு false;
                keyi->defined |= சாவி_FIELD_OVERLAY;
            } இல்லையெனில் {
                /* Handle disjoint overlays mode */
                மாறிலி xkb_overlay_mask_t mask = (xkb_overlay_mask_t)(1u << overlay);
                எனில் (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    எனில் (சாவி) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = mask;
                        keyi->overlays_clear = false;
                        keyi->overlay_சாவி = சாவி;
                    } இல்லையெனில் {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= mask;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= சாவி_FIELD_OVERLAY;
                } இல்லையெனில் எனில் (keyi->overlays) {
                    எனில் (சாவி) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                சாவி_Info_Text(info, keyi), keyi->overlays,
                                சாவி_Name_Text(info->ctx, keyi->overlay_சாவி->name),
                                overlay + 1, சாவி_Name_Text(info->ctx, சாவி->name));
                        திரும்பு !(info->keymap_info->strict &
                                 PARSER_NO_FIELD_மதிப்பு_MISMATCH);
                    } இல்லையெனில் {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    இல்லையெனில் எனில் (istreq(field, "repeating") ||
             istreq(field, "repeats") ||
             istreq(field, "repeat")) {
        uint32_t val = 0;

        எனில் (!கோவை_Resolve_Enum(info->ctx, மதிப்பு, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_மதிப்பு,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    சாவி_Info_Text(info, keyi));
            திரும்பு false;
        }

        keyi->repeat = (எண்ணிக்கை சாவி_repeat) val;
        keyi->defined |= சாவி_FIELD_REPEAT;
    }
    இல்லையெனில் எனில் (istreq(field, "groupswrap") ||
             istreq(field, "wrapgroups")) {
        bool set = false;

        எனில் (!கோவை_Resolve_Boolean(info->ctx, மதிப்பு, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_மதிப்பு,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    சாவி_Info_Text(info, keyi));
            திரும்பு false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_WRAP
            : XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        keyi->defined |= சாவி_FIELD_GROUPINFO;
    }
    இல்லையெனில் எனில் (istreq(field, "groupsclamp") ||
             istreq(field, "clampgroups")) {
        bool set = false;

        எனில் (!கோவை_Resolve_Boolean(info->ctx, மதிப்பு, &set)) {
            log_err(info->ctx, XKB_ERROR_INVALID_மதிப்பு,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    சாவி_Info_Text(info, keyi));
            திரும்பு false;
        }

        keyi->out_of_range_group_policy = (set)
            ? XKB_LAYOUT_OUT_OF_RANGE_CLAMP
            : XKB_LAYOUT_OUT_OF_RANGE_WRAP;
        keyi->defined |= சாவி_FIELD_GROUPINFO;
    }
    இல்லையெனில் எனில் (istreq(field, "groupsredirect") ||
             istreq(field, "redirectgroups")) {
        xkb_layout_index_t grp = 0;
        bool pending = false;

        // TODO: recover?
        எனில் (கோவை_Resolve_Group(info->keymap_info, மதிப்பு, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    சாவி_Info_Text(info, keyi));
            திரும்பு false;
        }

        எனில் (pending) {
            keyi->out_of_range_pending_group = true;
            மாறிலி darray_size_t pending_index =
                darray_size(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (கட்டமைப்பு pending_computation) {
                    .கோவை = *மதிப்பு_ptr,
                    .computed = false,
                    .மதிப்பு = 0,
                }
            );
            *மதிப்பு_ptr = வெற்று;
            static_assert(அளவு(keyi->out_of_range_group_எண்) ==
                          அளவு(pending_index),
                          "Cannot save pending computation");
            keyi->out_of_range_group_எண் = pending_index;
        } இல்லையெனில் {
            keyi->out_of_range_pending_group = false;
            keyi->out_of_range_group_எண் = grp - 1;
        }

        keyi->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        keyi->defined |= சாவி_FIELD_GROUPINFO;
    }
    இல்லையெனில் {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_FIELD,
                "Unknown field \"%s\" in a key; definition ignored\n",
                field);
        திரும்பு !(info->keymap_info->strict & PARSER_NO_UNKNOWN_சாவி_FIELDS);
    }

    திரும்பு true;
}

நிலையான bool
SetGroupName(SymbolsInfo *info, கோவை_Def *arrayNdx, கோவை_Def *மதிப்பு,
             எண்ணிக்கை merge_mode merge)
{
    எனில் (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_GROUP_NAME_INDEX,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        திரும்பு false;
    }

    xkb_layout_index_t group = 0;
    எனில் (கோவை_Resolve_Group(info->keymap_info, arrayNdx, false, &group, வெற்று) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_INDEX_,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        திரும்பு false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    எனில் (!கோவை_Resolve_String(info->ctx, மதிப்பு, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", group);
        திரும்பு false;
    }

    xkb_layout_index_t group_to_use;
    எனில் (info->explicit_group == XKB_LAYOUT_INVALID) {
        group_to_use = group - 1;
    }
    இல்லையெனில் எனில் (group - 1 == 0) {
        group_to_use = info->explicit_group;
    }
    இல்லையெனில் {
        log_warn(info->ctx, XKB_WARNING_NON_BASE_GROUP_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, group,
                 xkb_atom_text(info->ctx, name));
        திரும்பு false;
    }

    எனில் (group_to_use >= darray_size(info->group_names)) {
        /* Avoid clang-tidy false positive */
        ப்ரதிஜ்ஞா(darray_size(info->group_names) < group_to_use + 1);
        darray_resize0(info->group_names, group_to_use + 1);
    } இல்லையெனில் {
        மாறிலி xkb_atom_t old_name = darray_item(info->group_names, group_to_use);
        எனில் (old_name != XKB_ATOM_NONE && old_name != name) {
            மாறிலி bool replace = (merge != MERGE_AUGMENT);
            மாறிலி xkb_atom_t use    = (replace ? name : old_name);
            மாறிலி xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     group_to_use, info->name,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
            name = use;
        }
    }
    darray_item(info->group_names, group_to_use) = name;

    திரும்பு true;
}

நிலையான bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    மாறிலி எழுத்து *elem, *field;
    கோவை_Def *arrayNdx;
    bool ret;

    எனில் (!கோவை_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        திரும்பு false;

    எனில் (elem && istreq(elem, "key")) {
        சாவி_Info temp = {0};
        Init_சாவி_Info(info->ctx, &temp);
        /* Do not replace the whole key, only the current field */
        temp.merge = (temp.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = SetSymbolsField(info, &temp, field, arrayNdx, &stmt->மதிப்பு) &&
              Merge_சாவிகள்(info, &info->default_சாவி, &temp, true);
        Clear_சாவி_Info(&temp);
    }
    இல்லையெனில் எனில் (!elem && (istreq(field, "name") ||
                       istreq(field, "groupname"))) {
        ret = SetGroupName(info, arrayNdx, stmt->மதிப்பு, stmt->merge);
    }
    இல்லையெனில் எனில் (!elem && (istreq(field, "groupswrap") ||
                       istreq(field, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    இல்லையெனில் எனில் (!elem && (istreq(field, "groupsclamp") ||
                       istreq(field, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    இல்லையெனில் எனில் (!elem && (istreq(field, "groupsredirect") ||
                       istreq(field, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    இல்லையெனில் எனில் (!elem && istreq(field, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_FIELD,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    இல்லையெனில் எனில் (elem) {
        ret = (SetDefaultActionField(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, field, arrayNdx,
                                     &stmt->மதிப்பு, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } இல்லையெனில் {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        திரும்பு !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    திரும்பு ret;
}

நிலையான bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, சாவி_Info *keyi)
{
    bool all_valid_entries = true;

    க்கு (; def; def = (VarDef *) def->common.next) {
        மாறிலி எழுத்து *field;
        கோவை_Def *arrayNdx;
        bool ok = true;

        எனில் (!def->name) {
            எனில் (unlikely(!def->மதிப்பு) || def->மதிப்பு->common.type != STMT_கோவை_ACTION_LIST)
                field = "symbols"; /* Default to symbols field */
            இல்லையெனில்
                field = "actions";
            arrayNdx = வெற்று;
        }
        இல்லையெனில் {
            மாறிலி எழுத்து *elem;
            ok = கோவை_Resolve_Lhs(info->ctx, def->name, &elem, &field,
                                &arrayNdx);
            எனில் (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, field);
                ok = false;
            }
        }

        எனில் (unlikely(!def->மதிப்பு)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", field);
            ok = false;
        }

        எனில் (!ok || !SetSymbolsField(info, keyi, field, arrayNdx, &def->மதிப்பு))
            all_valid_entries = false;
    }

    திரும்பு all_valid_entries;
}

நிலையான bool
SetExplicitGroup(SymbolsInfo *info, சாவி_Info *keyi)
{
    xkb_layout_index_t i;
    GroupInfo *groupi;
    bool warn = false;

    எனில் (info->explicit_group == XKB_LAYOUT_INVALID)
        திரும்பு true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        எனில் (groupi->defined) {
            warn = true;
            ClearGroupInfo(groupi);
            InitGroupInfo(groupi);
        }
    }

    எனில் (warn) {
        log_warn(info->ctx, XKB_WARNING_MULTIPLE_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_group + 1, சாவி_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_group + 1);
    எனில் (info->explicit_group > 0) {
        darray_item(keyi->groups, info->explicit_group) =
            darray_item(keyi->groups, 0);
        InitGroupInfo(&darray_item(keyi->groups, 0));
    }

    திரும்பு true;
}

நிலையான bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    சாவி_Info keyi;

    keyi = info->default_சாவி;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_சாவி.groups);
    க்கு (xkb_layout_index_t i = 0; i < darray_size(keyi.groups); i++)
        CopyGroupInfo(&darray_item(keyi.groups, i),
                      &darray_item(info->default_சாவி.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->சாவி_Name;

    எனில் (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        SetExplicitGroup(info, &keyi) &&
        Add_சாவி_Symbols(info, &keyi, true)) {
        திரும்பு true;
    }

    Clear_சாவி_Info(&keyi);
    info->error_மொத்தம்++;
    திரும்பு false;
}

நிலையான bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    ModMapEntry tmp;
    கட்டமைப்பு xkb_context *ctx = info->ctx;

    எனில் (!கோவை_Resolve_Mod_Mask(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        திரும்பு false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    ப்ரதிஜ்ஞா(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_கோவை_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    க்கு (கோவை_Def *சாவி = def->சாவிகள்; சாவி; சாவி = (கோவை_Def *) சாவி->common.next) {
        எனில் (சாவி->common.type == STMT_கோவை_KEYNAME_LITERAL) {
            tmp.have_குறியீடு = false;
            tmp.u.சாவி_Name = சாவி->சாவி_name.சாவி_name;
        }
        இல்லையெனில் எனில் (சாவி->common.type == STMT_கோவை_KEYSYM_LITERAL) {
            எனில் (சாவி->keysym.keysym == XKB_சாவி_No_குறியீடு) {
                /* Invalid keysym: ignore. Error message already printed */
                தொடர்;
            } இல்லையெனில் {
                tmp.have_குறியீடு = true;
                tmp.u.சாவி_Sym = சாவி->keysym.keysym;
            }
        }
        இல்லையெனில் {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_ENTRY,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            தொடர்;
        }

        ok = AddModMapEntry(info, &tmp) && ok;
    }
    திரும்பு ok;
}

நிலையான வெறுமை
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    விடுவி(info->name);
    info->name = strdup_safe(file->name);

    க்கு (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        தேர்வு (stmt->type) {
        நிலை STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            நிறுத்து;
        நிலை STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            நிறுத்து;
        நிலை STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            நிறுத்து;
        நிலை STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            நிறுத்து;
        நிலை STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            நிறுத்து;
        நிலை STMT_UNKNOWN_DECLARATION:
        நிலை STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            நிறுத்து;
        இயல்பு:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            நிறுத்து;
        }

        எனில் (!ok)
            info->error_மொத்தம்++;

        எனில் (info->error_மொத்தம் > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            நிறுத்து;
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
நிலையான கட்டமைப்பு xkb_சாவி *
Find_சாவி_For_குறியீடு(கட்டமைப்பு xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_group;
    xkb_layout_index_t group = 0;
    செய் {
        xkb_level_index_t level = 0;
        got_one_group = false;
        bool got_one_level;
        செய் {
            got_one_level = false;
            கட்டமைப்பு xkb_சாவி *சாவி;
            xkb_சாவிகள்_foreach(சாவி, keymap) {
                எனில் (group < சாவி->இலக்கம்_groups &&
                    level < Xkb_சாவி_இலக்கம்_Levels(சாவி, group)) {
                    got_one_group = got_one_level = true;
                    மாறிலி xkb_keysym_மொத்தம்_t இலக்கம்_syms =
                        சாவி->groups[group].levels[level].இலக்கம்_syms;
                    எனில் (இலக்கம்_syms > 1) {
                        க்கு (xkb_keysym_மொத்தம்_t k = 0; k < இலக்கம்_syms; k++) {
                            எனில் (சாவி->groups[group].levels[level].s.syms[k] == sym)
                                திரும்பு சாவி;
                        }
                    } இல்லையெனில் எனில் (இலக்கம்_syms &&
                               சாவி->groups[group].levels[level].s.sym == sym) {
                        திரும்பு சாவி;
                    }
                }
            }
            level++;
        } போது (got_one_level);
        group++;
    } போது (got_one_group);

    திரும்பு வெற்று;
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
நிலையான xkb_atom_t
FindAutomaticType(கட்டமைப்பு xkb_context *ctx, GroupInfo *groupi)
{
    xkb_keysym_t sym0, sym1;
    மாறிலி xkb_level_index_t width = darray_size(groupi->levels);

#define GET_SYM(level) \
    (darray_item(groupi->levels, level).இலக்கம்_syms == 0 ? \
        XKB_சாவி_No_குறியீடு : \
     darray_item(groupi->levels, level).இலக்கம்_syms == 1 ? \
        darray_item(groupi->levels, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->levels, level).s.syms[0])

    எனில் (width == 1 || width <= 0)
        திரும்பு xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    எனில் (width == 2) {
        எனில் (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1))
            திரும்பு xkb_atom_intern_literal(ctx, "ALPHABETIC");

        எனில் (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            திரும்பு xkb_atom_intern_literal(ctx, "KEYPAD");

        திரும்பு xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    எனில் (width <= 4) {
        எனில் (xkb_keysym_is_lower(sym0) && xkb_keysym_is_upper_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (width == 4 ? GET_SYM(3) : XKB_சாவி_No_குறியீடு);

            எனில் (xkb_keysym_is_lower(sym2) && xkb_keysym_is_upper_or_title(sym3))
                திரும்பு xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            திரும்பு xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        எனில் (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            திரும்பு xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        திரும்பு xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    திரும்பு XKB_ATOM_NONE;

#undef GET_SYM
}

நிலையான மாறிலி கட்டமைப்பு xkb_சாவி_type *
FindTypeForGroup(கட்டமைப்பு xkb_keymap *keymap, சாவி_Info *keyi,
                 xkb_layout_index_t group, bool *explicit_type)
{
    GroupInfo *groupi = &darray_item(keyi->groups, group);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    எனில் (type_name == XKB_ATOM_NONE) {
        எனில் (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        இல்லையெனில் {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            எனில் (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    எனில் (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_சாவி_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 சாவி_Name_Text(keymap->ctx, keyi->name), group + 1,
                 darray_size(groupi->levels));
        செல் use_default;
    }

    darray_size_t i;
    க்கு (i = 0; i < keymap->இலக்கம்_types; i++)
        எனில் (keymap->types[i].name == type_name)
            நிறுத்து;

    எனில் (i >= keymap->இலக்கம்_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_சாவி_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 சாவி_Name_Text(keymap->ctx, keyi->name), group + 1);
        செல் use_default;
    }

    keymap->types[i].required = true;
    திரும்பு &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    திரும்பு &keymap->types[0];
}

நிலையான bool
CopySymbolsDefToKeymap(கட்டமைப்பு xkb_keymap *keymap, SymbolsInfo *info,
                       சாவி_Info *keyi)
{
    கட்டமைப்பு xkb_சாவி *சாவி;
    GroupInfo *groupi;
    மாறிலி GroupInfo *group0;
    xkb_layout_index_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    சாவி = Xkb_சாவி_By_Name(keymap, keyi->name, false);
    எனில் (!சாவி) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                சாவி_Info_Text(info, keyi));
        திரும்பு false;
    }

    /* Find the range of groups we need. */
    சாவி->இலக்கம்_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        மாறிலி bool has_explicit_type = (keyi->defined & சாவி_FIELD_DEFAULT_TYPE)
                                    || (groupi->defined & GROUP_FIELD_TYPE);
        எனில் (darray_size(groupi->levels) > 0 || has_explicit_type)
            சாவி->இலக்கம்_groups = i + 1;
        எனில் (has_explicit_type)
            சாவி->explicit |= EXPLICIT_TYPES;
    }

    எனில் (சாவி->இலக்கம்_groups <= 0) {
        /* A key with no group may still have other fields defined */
        எனில் (keyi->defined)
            செல் சாவி_fields;
        இல்லையெனில்
            திரும்பு false;
    }

    darray_resize(keyi->groups, சாவி->இலக்கம்_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        எனில் (groupi->defined)
            தொடர்;

        CopyGroupInfo(groupi, group0);
    }

    சாவி->groups = calloc(சாவி->இலக்கம்_groups, அளவு(*சாவி->groups));
    ப்ரதிஜ்ஞா(darray_size(keyi->groups) == சாவி->இலக்கம்_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #ifdef __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < சாவி->இலக்கம்_groups);
        #endif
        bool explicit_type = false;
        மாறிலி கட்டமைப்பு xkb_சாவி_type * மாறிலி type =
            FindTypeForGroup(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        எனில் (type->இலக்கம்_levels < darray_size(groupi->levels)) {
            கட்டமைப்பு xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->இலக்கம்_levels,
                    சாவி_Info_Text(info, keyi),
                    darray_size(groupi->levels));

            darray_foreach_from(leveli, groupi->levels, type->இலக்கம்_levels)
                clear_level(leveli);
        }
        darray_resize0(groupi->levels, type->இலக்கம்_levels);

        சாவி->groups[i].explicit_type = explicit_type;
        சாவி->groups[i].type = type;
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
        கட்டமைப்பு xkb_level *leveli;
        darray_foreach(leveli, groupi->levels) {
            தேர்வு (leveli->இலக்கம்_syms) {
            நிலை 0:
                leveli->upper = XKB_சாவி_No_குறியீடு;
                நிறுத்து;
            நிலை 1:
                leveli->upper = xkb_keysym_to_upper(leveli->s.sym);
                நிறுத்து;
            இயல்பு:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_upper = false;
                க்கு (xkb_keysym_மொத்தம்_t k = 0; k < leveli->இலக்கம்_syms; k++) {
                    மாறிலி xkb_keysym_t upper =
                        xkb_keysym_to_upper(leveli->s.syms[k]);
                    எனில் (upper != leveli->s.syms[k]) {
                        leveli->has_upper = true;
                        நிறுத்து;
                    }
                }
                எனில் (leveli->has_upper) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (size_t) 2 * leveli->இலக்கம்_syms *
                                             அளவு(*leveli->s.syms));
                    எனில் (!leveli->s.syms)
                        திரும்பு false; /* FIXME: better handling? */
                    க்கு (xkb_keysym_மொத்தம்_t k = 0; k < leveli->இலக்கம்_syms; k++) {
                        leveli->s.syms[leveli->இலக்கம்_syms + k] =
                            xkb_keysym_to_upper(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->levels, &சாவி->groups[i].levels, வெற்று);
        எனில் (சாவி->groups[i].type->இலக்கம்_levels > 1 ||
            சாவி->groups[i].levels[0].இலக்கம்_syms > 0) {
            சாவி->groups[i].explicit_symbols = true;
            சாவி->explicit |= EXPLICIT_SYMBOLS;
        }
        எனில் (groupi->defined & GROUP_FIELD_ACTS) { // FIXME
            சாவி->groups[i].explicit_actions = true;
            சாவி->explicit |= EXPLICIT_INTERP;
        }
        எனில் (சாவி->groups[i].explicit_type)
            சாவி->explicit |= EXPLICIT_TYPES;
    }

    சாவி->out_of_range_pending_group = keyi->out_of_range_pending_group;
    சாவி->out_of_range_group_எண் = keyi->out_of_range_group_எண்;
    சாவி->out_of_range_group_policy = keyi->out_of_range_group_policy;

சாவி_fields:
    எனில் (keyi->defined & சாவி_FIELD_VMODMAP) {
        சாவி->vmodmap = keyi->vmodmap;
        சாவி->explicit |= EXPLICIT_VMODMAP;
    }

    எனில் (keyi->repeat != சாவி_REPEAT_UNDEFINED) {
        சாவி->repeats = (keyi->repeat == சாவி_REPEAT_YES);
        சாவி->explicit |= EXPLICIT_REPEAT;
    }

    எனில் ((keyi->defined & சாவி_FIELD_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        சாவி->overlays = keyi->overlays;
        எனில் (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_mask_t remaining = சாவி->overlays;
            மாறிலி கட்டமைப்பு xkb_சாவி **overlays_சாவிகள் = keyi->overlays_சாவிகள்;
            போது (remaining) {
                /* isolate lowest set bit */
                மாறிலி xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                    remaining &
                    (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                எனில் (*overlays_சாவிகள்) {
                    overlays_சாவிகள்++;
                } இல்லையெனில் {
                    /* drop current null value (overlayN=none) */
                    சாவி->overlays &= ~lsb;
                }
            }

            எனில் (சாவி->overlays) {
                /* Steal */
                சாவி->overlays_சாவிகள் = keyi->overlays_சாவிகள்;
                keyi->overlays_சாவிகள் = வெற்று;
                keyi->overlays_alloc = 0;

                சாவி->overlays_inline = false;
                சாவி->explicit |= EXPLICIT_OVERLAY;
            }
        } இல்லையெனில் {
            சாவி->overlay_சாவி = keyi->overlay_சாவி;
            சாவி->overlays_inline = true;
            சாவி->explicit |= EXPLICIT_OVERLAY;
        }
    }

    திரும்பு true;
}

நிலையான bool
CopyModMapDefToKeymap(கட்டமைப்பு xkb_keymap *keymap, SymbolsInfo *info,
                      ModMapEntry *entry)
{
    கட்டமைப்பு xkb_சாவி *சாவி;

    எனில் (!entry->have_குறியீடு) {
        சாவி = Xkb_சாவி_By_Name(keymap, entry->u.சாவி_Name, true);
        எனில் (!சாவி) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    சாவி_Name_Text(info->ctx, entry->u.சாவி_Name),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            திரும்பு false;
        }
    }
    இல்லையெனில் {
        சாவி = Find_சாவி_For_குறியீடு(keymap, entry->u.சாவி_Sym);
        எனில் (!சாவி) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_குறியீடு,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, entry->u.சாவி_Sym),
                    ModMaskText(info->ctx, MOD_REAL, &info->mods, entry->mods));
            entry->mods = 0; /* disable */
            திரும்பு false;
        }
    }

    /* Skip modMap None */
    எனில் (entry->mods) {
        சாவி->modmap |= entry->mods;
        entry->சாவி_Code = சாவி->keycode;
    }

    திரும்பு true;
}

நிலையான bool
CopySymbolsToKeymap(கட்டமைப்பு xkb_keymap *keymap, SymbolsInfo *info)
{
    சாவி_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->இலக்கம்_group_names = (xkb_layout_index_t)darray_size(info->group_names);
    darray_steal(info->group_names, &keymap->group_names, வெற்று);

    darray_foreach(keyi, info->சாவிகள்)
        எனில் (!CopySymbolsDefToKeymap(keymap, info, keyi))
            info->error_மொத்தம்++;

    எனில் (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        கட்டமைப்பு xkb_சாவி *சாவி;

        xkb_சாவிகள்_foreach(சாவி, keymap) {
            எனில் (சாவி->name == XKB_ATOM_NONE)
                தொடர்;

            எனில் (சாவி->இலக்கம்_groups < 1)
                log_info(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "No symbols defined for %s\n",
                         சாவி_Name_Text(info->ctx, சாவி->name));
        }
    }

    /* Resolve modmaps */
    ModMapEntry *mm;
    darray_foreach(mm, info->modmaps)
        எனில் (!CopyModMapDefToKeymap(keymap, info, mm))
            info->error_மொத்தம்++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->இலக்கம்_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    திரும்பு true;
}

bool
CompileSymbols(XkbFile *file, கட்டமைப்பு xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    எனில் (file !=வெற்று)
        HandleSymbolsFile(&info, file);

    எனில் (info.error_மொத்தம் != 0)
        செல் err_info;

    எனில் (!CopySymbolsToKeymap(&keymap_info->keymap, &info))
        செல் err_info;

    ClearSymbolsInfo(&info);
    திரும்பு true;

err_info:
    ClearSymbolsInfo(&info);
    திரும்பு false;
}
