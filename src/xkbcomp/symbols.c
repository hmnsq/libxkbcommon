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

#अन्तर्भाव "config.h"

#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "xkbcommon/xkbcommon-keysyms.h"

#अन्तर्भाव "action.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "expr.h"
#अन्तर्भाव "include.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "keysym.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "util-numbers.h"
#अन्तर्भाव "util-mem.h"
#अन्तर्भाव "vmod.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "xkbcomp/ast.h"

गणना कुञ्चिका_repeat {
    कुञ्चिका_REPEAT_UNDEFINED = 0,
    कुञ्चिका_REPEAT_YES,
    कुञ्चिका_REPEAT_NO,
    कुञ्चिका_REPEAT_अङ्कः_ENTRIES,
};
गणना { कुञ्चिका_REPEAT_MIN_विस्तारः = 3 /* 2 bits + sign */ };
static_assert(कुञ्चिका_REPEAT_अङ्कः_ENTRIES - 1 <=
              (1 << (कुञ्चिका_REPEAT_MIN_विस्तारः - 1)) - 1,
              "Cannot encode key repeat");

गणना गणः_क्षेत्रम् {
    गणः_क्षेत्रम्_SYMS = (1 << 0),
    गणः_क्षेत्रम्_ACTS = (1 << 1),
    गणः_क्षेत्रम्_TYPE = (1 << 2),
};

गणना कुञ्चिका_क्षेत्रम् {
    कुञ्चिका_क्षेत्रम्_NONE = 0,
    कुञ्चिका_क्षेत्रम्_REPEAT = (1 << 0),
    कुञ्चिका_क्षेत्रम्_DEFAULT_TYPE = (1 << 1),
    कुञ्चिका_क्षेत्रम्_GROUPINFO = (1 << 2),
    कुञ्चिका_क्षेत्रम्_VMODMAP = (1 << 3),
    कुञ्चिका_क्षेत्रम्_OVERLAY = (1 << 4),
    कुञ्चिका_क्षेत्रम्_ALL = कुञ्चिका_क्षेत्रम्_REPEAT | कुञ्चिका_क्षेत्रम्_DEFAULT_TYPE
                  | कुञ्चिका_क्षेत्रम्_GROUPINFO | कुञ्चिका_क्षेत्रम्_VMODMAP | कुञ्चिका_क्षेत्रम्_OVERLAY,
};
गणना { कुञ्चिका_क्षेत्रम्_MIN_विस्तारः = 6 /* 5 bits + sign */ };
static_assert(कुञ्चिका_क्षेत्रम्_ALL <= (1 << (कुञ्चिका_क्षेत्रम्_MIN_विस्तारः - 1)) - 1,
              "Cannot encode key field");

प्रकारनिर्देश संरचना {
    darray(संरचना xkb_level) स्तराः;
    गणना गणः_क्षेत्रम् defined;
    xkb_atom_t type;
} गणः_Info_2;

गणना {
    XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY_MIN_विस्तारः = 4 /* 3 bits + sign */,
    KEYINFO_बहिर्गमनम्_सम्बन्धि_RANGE_गणः_POLICY_BIT_क्षेत्रम् = 8,
    KEYINFO_DEFINED_BIT_क्षेत्रम् = 16,
    KEYINFO_MERGE_BIT_क्षेत्रम् = 8,
    KEYINFO_REPEAT_BIT_क्षेत्रम् = 8,
};
static_assert(
    XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP < XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_CLAMP &&
    XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_CLAMP < XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT,
    "Enum values were changed!"
);
static_assert(XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT <=
              (1 << XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY_MIN_विस्तारः) - 1,
              "Cannot encode out of range redirect");
static_assert(KEYINFO_बहिर्गमनम्_सम्बन्धि_RANGE_गणः_POLICY_BIT_क्षेत्रम् >=
              XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_POLICY_MIN_विस्तारः, "");
static_assert((अचिह्नित)KEYINFO_DEFINED_BIT_क्षेत्रम् >=
              (अचिह्नित)कुञ्चिका_क्षेत्रम्_MIN_विस्तारः, "Cannot encode KeyInfo field");
static_assert((अचिह्नित)KEYINFO_MERGE_BIT_क्षेत्रम् >=
              (अचिह्नित)MERGE_बहुलकम्_MIN_विस्तारः, "Cannot encode KeyInfo field");
static_assert((अचिह्नित)KEYINFO_REPEAT_BIT_क्षेत्रम् >=
              (अचिह्नित)कुञ्चिका_REPEAT_MIN_विस्तारः, "Cannot encode KeyInfo field");

प्रकारनिर्देश संरचना {
    xkb_atom_t name;

    xkb_mod_आवरणम्_t vmodmap;
    xkb_atom_t default_type;

    xkb_layout_सूचकांक_t बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या;
    darray(गणः_Info_2) groups;
    गणना xkb_layout_बहिर्गमनम्_सम्बन्धि_range_policy
        बहिर्गमनम्_सम्बन्धि_range_गणः_policy:KEYINFO_बहिर्गमनम्_सम्बन्धि_RANGE_गणः_POLICY_BIT_क्षेत्रम्;

    /* NOTE: this bitfield could be reduce to 1 byte, should we need it */
    गणना कुञ्चिका_क्षेत्रम् defined:KEYINFO_DEFINED_BIT_क्षेत्रम्;
    गणना merge_बहुलकम् merge:KEYINFO_MERGE_BIT_क्षेत्रम्;
    गणना कुञ्चिका_repeat repeat:KEYINFO_REPEAT_BIT_क्षेत्रम्;

    bool बहिर्गमनम्_सम्बन्धि_range_pending_गणः:1;

    /** If true, overlays in the mask are explicitly cleared */
    bool overlays_clear:1;
    /** Track the count of allocated slots of overlays_keys */
    xkb_overlay_सूचकांक_t overlays_alloc;
    /** Overlays indices */
    xkb_overlay_आवरणम्_t overlays;
    /**
     * Target overlays keys, if any.
     *
     * For 0 or 1 elements storage is inline in the struct; for 2 or more a
     * heap sparse array is used.
     */
    सङ्घ {
        /** Inlined if at most one overlay is defined (overlays_alloc = 0) */
        ध्रुव संरचना xkb_कुञ्चिका *overlay_कुञ्चिका;
        /**
         * Allocated if 2 or more overlays are defined (overlays_alloc > 0).
         *
         * A sparse array maps the set of sparse overlay indices in field
         * `overlays` to contiguously stored values. Values are stored in
         * ascending overlay order, and a value’s position in storage equals the
         * popcount of all mask bits strictly below its overlay — its rank.
         */
        ध्रुव संरचना xkb_कुञ्चिका **overlays_कुञ्चिकाः;
    };
} कुञ्चिका_Info;

स्थिर शून्य
StealLevelInfo(संरचना xkb_level *into, संरचना xkb_level *from)
{
    clear_level(into);
    यदि (from->अङ्कः_syms > 1) {
        /* Steal */
        into->s.syms = steal(&from->s.syms);
    } अन्यथा {
        into->s.sym = from->s.sym;
    }
    into->अङ्कः_syms = from->अङ्कः_syms;
    from->अङ्कः_syms = 0;

    यदि (from->अङ्कः_actions > 1) {
        into->a.actions = steal(&from->a.actions);
    } अन्यथा {
        into->a.action = from->a.action;
    }
    into->अङ्कः_actions = from->अङ्कः_actions;
    from->अङ्कः_actions = 0;
}

स्थिर शून्य
Init_गणः_Info(गणः_Info_2 *groupi)
{
    स्मृतिं_पूरय(groupi, 0, परिमाण(*groupi));
}

स्थिर शून्य
Clear_गणः_Info(गणः_Info_2 *groupi)
{
    संरचना xkb_level *leveli;
    darray_foreach(leveli, groupi->स्तराः)
        clear_level(leveli);
    darray_free(groupi->स्तराः);
}

स्थिर शून्य
Copy_गणः_Info(गणः_Info_2 *गन्तव्यम्, ध्रुव गणः_Info_2 *from)
{
    गन्तव्यम्->defined = from->defined;
    गन्तव्यम्->type = from->type;
    darray_init(गन्तव्यम्->स्तराः);
    darray_copy(गन्तव्यम्->स्तराः, from->स्तराः);
    प्रति (xkb_level_सूचकांक_t j = 0; j < darray_आकार(गन्तव्यम्->स्तराः); j++) {
        यदि (darray_item(from->स्तराः, j).अङ्कः_syms > 1) {
            darray_item(गन्तव्यम्->स्तराः, j).s.syms =
                memdup(darray_item(from->स्तराः, j).s.syms,
                       darray_item(from->स्तराः, j).अङ्कः_syms,
                       परिमाण(xkb_keysym_t));
        }
        यदि (darray_item(from->स्तराः, j).अङ्कः_actions > 1) {
            darray_item(गन्तव्यम्->स्तराः, j).a.actions =
                memdup(darray_item(from->स्तराः, j).a.actions,
                       darray_item(from->स्तराः, j).अङ्कः_actions,
                       परिमाण(सङ्घ xkb_action));
        }
    }
}

स्थिर शून्य
Init_कुञ्चिका_Info(संरचना xkb_context *ctx, कुञ्चिका_Info *keyi)
{
    स्मृतिं_पूरय(keyi, 0, परिमाण(*keyi));
    static_assert(!DEFAULT_कुञ्चिका_REPEAT, "key repeat not initialized properly");
    static_assert(!DEFAULT_कुञ्चिका_VMODMAP, "key vmodmap not initialized properly");
    keyi->name = xkb_atom_intern_literal(ctx, "*");
    keyi->बहिर्गमनम्_सम्बन्धि_range_गणः_policy = XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP;
    darray_init(keyi->groups);
    keyi->overlay_कुञ्चिका = रिक्त;
}

स्थिर शून्य
Clear_कुञ्चिका_Info(कुञ्चिका_Info *keyi)
{
    गणः_Info_2 *groupi;
    darray_foreach(groupi, keyi->groups)
        Clear_गणः_Info(groupi);
    darray_free(keyi->groups);
    यदि (keyi->overlays_alloc)
        मुक्त(keyi->overlays_कुञ्चिकाः);
}

/***====================================================================***/

प्रकारनिर्देश संरचना {
    वर्ण *name;         /* e.g. pc+us+inet(evdev) */
    पूर्णाङ्क error_गणनम्;
    अचिह्नित पूर्णाङ्क include_गभीरता;
    xkb_layout_सूचकांक_t explicit_गणः;
    xkb_layout_सूचकांक_t max_groups;
    darray(कुञ्चिका_Info) कुञ्चिकाः;
    कुञ्चिका_Info default_कुञ्चिका;
    ActionsInfo default_actions;
    darray(xkb_atom_t) गणः_नामानि;
    darray(Mod_Map_प्रविष्टिः) modmaps;
    संरचना xkb_mod_समुच्चयः mods;

    संरचना xkb_context *ctx;
    /* Needed for AddKeySymbols and parsing actions */
    ध्रुव संरचना xkb_keymap_info *keymap_info;
} SymbolsInfo;

स्थिर शून्य
InitSymbolsInfo(SymbolsInfo *info, ध्रुव संरचना xkb_keymap_info *keymap_info,
                अचिह्नित पूर्णाङ्क include_गभीरता, ध्रुव संरचना xkb_mod_समुच्चयः *mods)
{
    स्मृतिं_पूरय(info, 0, परिमाण(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->include_गभीरता = include_गभीरता;
    info->keymap_info = keymap_info;
    info->max_groups = keymap_info->features.max_groups;
    Init_कुञ्चिका_Info(keymap_info->keymap.ctx, &info->default_कुञ्चिका);
    InitActionsInfo(&keymap_info->keymap, &info->default_actions);
    InitVMods(&info->mods, mods, include_गभीरता > 0);
    info->explicit_गणः = XKB_LAYOUT_INVALID;
}

स्थिर शून्य
ClearSymbolsInfo(SymbolsInfo *info)
{
    कुञ्चिका_Info *keyi;
    मुक्त(info->name);
    darray_foreach(keyi, info->कुञ्चिकाः)
        Clear_कुञ्चिका_Info(keyi);
    darray_free(info->कुञ्चिकाः);
    darray_free(info->गणः_नामानि);
    darray_free(info->modmaps);
    Clear_कुञ्चिका_Info(&info->default_कुञ्चिका);
}

स्थिर ध्रुव वर्ण *
कुञ्चिका_Info_Text(SymbolsInfo *info, कुञ्चिका_Info *keyi)
{
    प्रत्यागम कुञ्चिका_Name_Text(info->ctx, keyi->name);
}

स्थिर bool
MergeGroups(SymbolsInfo *info, गणः_Info_2 *into, गणः_Info_2 *from, bool clobber,
            bool report, xkb_layout_सूचकांक_t गणः, xkb_atom_t कुञ्चिका_name)
{
    /* First find the type of the merged group. */
    यदि (into->type != from->type) {
        यदि (from->type == XKB_ATOM_NONE) {
            /* it's empty for consistency with other comparisons */
        }
        अन्यथा यदि (into->type == XKB_ATOM_NONE) {
            into->type = from->type;
        }
        अन्यथा {
            xkb_atom_t use = (clobber ? from->type : into->type);
            xkb_atom_t ignore = (clobber ? into->type : from->type);

            यदि (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_TYPE_MERGING_GROUPS,
                         "Multiple definitions for group %"PRIu32" type of key %s; "
                         "Using %s, ignoring %s\n",
                         गणः + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका_name),
                         xkb_atom_text(info->ctx, use),
                         xkb_atom_text(info->ctx, ignore));
            }

            into->type = use;
        }
    }
    into->defined |= (from->defined & गणः_क्षेत्रम्_TYPE);

    /* Now look at the levels. */

    यदि (darray_रिक्तानि(from->स्तराः)) {
        Init_गणः_Info(from);
        प्रत्यागम true;
    }

    यदि (darray_रिक्तानि(into->स्तराः)) {
        from->type = into->type;
        *into = *from;
        Init_गणः_Info(from);
        प्रत्यागम true;
    }

    /* Merge the actions and syms. */
    ध्रुव darray_आकार_t स्तराः_अन्तर्_both =
        MIN(darray_आकार(into->स्तराः), darray_आकार(from->स्तराः));
    darray_आकार_t from_Keysyms_गणनम् = 0;
    darray_आकार_t from_Actions_गणनम् = 0;
    प्रति (darray_आकार_t i = 0; i < स्तराः_अन्तर्_both; i++) {
        संरचना xkb_level* ध्रुव intoLevel = &darray_item(into->स्तराः, i);
        संरचना xkb_level* ध्रुव fromLevel = &darray_item(from->स्तराः, i);

        ध्रुव bool fromHasNoKeysym = fromLevel->अङ्कः_syms == 0;
        ध्रुव bool fromHasNoAction = fromLevel->अङ्कः_actions == 0;
        यदि (fromHasNoKeysym && fromHasNoAction) {
            /* Empty `from`: do nothing */
            अनुवर्त;
        }

        ध्रुव bool intoHasNoKeysym = intoLevel->अङ्कः_syms == 0;
        ध्रुव bool intoHasNoAction = intoLevel->अङ्कः_actions == 0;
        यदि (intoHasNoKeysym && intoHasNoAction) {
            /* Empty `into`: use `from` keysyms and actions */
            StealLevelInfo(intoLevel, fromLevel);
            from_Keysyms_गणनम्++;
            from_Actions_गणनम्++;
            अनुवर्त;
        }

        /* Possible level conflict */
        प्रतिज्ञा(intoLevel->अङ्कः_syms > 0 || intoLevel->अङ्कः_actions > 0);
        प्रतिज्ञा(fromLevel->अङ्कः_syms > 0 || fromLevel->अङ्कः_actions > 0);

        /* Handle keysyms */
        यदि (!Xkb_स्तराः_Same_Syms(fromLevel, intoLevel)) {
            /* Incompatible keysyms */
            यदि (report && !(intoHasNoKeysym || fromHasNoKeysym)) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_चिह्नम्,
                            "Multiple symbols for level %u/group "
                            "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                            i + 1, गणः + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका_name),
                            (clobber ? "from" : "to"),
                            (clobber ? "to" : "from"));
            }
            यदि (fromHasNoKeysym) {
                /* No keysym to copy */
            } अन्यथा यदि (clobber) {
                /* Override: copy any defined keysym from `from` */
                यदि (unlikely(fromLevel->अङ्कः_syms > 1)) {
                    /* Multiple keysyms: always replace, all syms are defined */
                    यदि (unlikely(intoLevel->अङ्कः_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    /* Steal */
                    intoLevel->s.syms = fromLevel->s.syms;
                    intoLevel->अङ्कः_syms = fromLevel->अङ्कः_syms;
                    fromLevel->अङ्कः_syms = 0;
                    from_Keysyms_गणनम्++;
                } अन्यथा यदि (fromLevel->s.sym != XKB_कुञ्चिका_No_चिह्नम्) {
                    /* Single defined keysym */
                    यदि (unlikely(intoLevel->अङ्कः_syms > 1))
                        मुक्त(intoLevel->s.syms);
                    intoLevel->s.sym = fromLevel->s.sym;
                    intoLevel->अङ्कः_syms = 1;
                    from_Keysyms_गणनम्++;
                }
            } अन्यथा {
                /* Augment: copy only the keysyms from `from` that are
                 * undefined in `into` */
                यदि (unlikely(intoLevel->अङ्कः_syms > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अन्यथा यदि (intoLevel->s.sym == XKB_कुञ्चिका_No_चिह्नम्) {
                    /* Single undefined keysym */
                    यदि (unlikely(fromLevel->अङ्कः_syms > 1)) {
                        /* Steal */
                        intoLevel->s.syms = fromLevel->s.syms;
                        intoLevel->अङ्कः_syms = fromLevel->अङ्कः_syms;
                        fromLevel->अङ्कः_syms = 0;
                    } अन्यथा {
                        intoLevel->s.sym = fromLevel->s.sym;
                        intoLevel->अङ्कः_syms = 1;
                    }
                    from_Keysyms_गणनम्++;
                }
            }
        }

        /* Handle actions */
        यदि (!Xkb_स्तराः_Same_Actions(intoLevel, fromLevel)) {
            /* Incompatible actions */
            यदि (report && !(intoHasNoAction || fromHasNoAction)) {
                यदि (unlikely(intoLevel->अङ्कः_actions > 1)) {
                    log_warn(
                        info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_ACTION,
                        "Multiple actions for level %u/group %"PRIu32" "
                        "on key %s; %s\n",
                        i + 1, गणः + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका_name),
                        clobber ? "Using from, ignoring to"
                                : "Using to, ignoring from");
                } अन्यथा {
                    सङ्घ xkb_action *use, *ignore;
                    use = clobber
                        ? &fromLevel->a.action
                        : &intoLevel->a.action;
                    ignore = clobber
                        ? &intoLevel->a.action
                        : &fromLevel->a.action;

                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_ACTION,
                                "Multiple actions for level %u/group "
                                "%"PRIu32" on key %s; Using %s, ignoring %s\n",
                                i + 1, गणः + 1,
                                कुञ्चिका_Name_Text(info->ctx, कुञ्चिका_name),
                                ActionTypeText(use->type),
                                ActionTypeText(ignore->type));
                }
            }
            यदि (fromHasNoAction) {
                /* No action to copy */
            } अन्यथा यदि (clobber) {
                /* Override: copy any defined action from `from` */
                यदि (unlikely(fromLevel->अङ्कः_actions > 1)) {
                    /* Multiple actions: always replace, all syms are defined */
                    यदि (unlikely(intoLevel->अङ्कः_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    /* Steal */
                    intoLevel->a.actions = fromLevel->a.actions;
                    intoLevel->अङ्कः_actions = fromLevel->अङ्कः_actions;
                    fromLevel->अङ्कः_actions = 0;
                    from_Actions_गणनम्++;
                } अन्यथा यदि (fromLevel->a.action.type != ACTION_TYPE_NONE) {
                    /* Single defined action */
                    यदि (unlikely(intoLevel->अङ्कः_actions > 1))
                        मुक्त(intoLevel->a.actions);
                    intoLevel->a.action = fromLevel->a.action;
                    intoLevel->अङ्कः_actions = 1;
                    from_Actions_गणनम्++;
                }
            } अन्यथा {
                /* Augment: copy only the actions from `from` that are
                 * undefined in `into` */
                यदि (unlikely(intoLevel->अङ्कः_actions > 1)) {
                    /* Multiple keysyms: always ignore, all syms are defined */
                } अन्यथा यदि (intoLevel->a.action.type == ACTION_TYPE_NONE) {
                    /* Single undefined keysym */
                    यदि (unlikely(fromLevel->अङ्कः_actions > 1)) {
                        /* Steal */
                        intoLevel->a.actions = fromLevel->a.actions;
                        intoLevel->अङ्कः_actions = fromLevel->अङ्कः_actions;
                        fromLevel->अङ्कः_actions = 0;
                    } अन्यथा {
                        intoLevel->a.action = fromLevel->a.action;
                        intoLevel->अङ्कः_actions = 1;
                    }
                    from_Actions_गणनम्++;
                }
            }
        }
    }
    /* If @from has extra levels, get them as well. */
    संरचना xkb_level *level;
    darray_foreach_from(level, from->स्तराः, स्तराः_अन्तर्_both) {
        darray_append(into->स्तराः, *level);
        /* We may have stolen keysyms or actions arrays:
         * do not free them when clearing `from` */
        level->अङ्कः_syms = 0;
        level->अङ्कः_actions = 0;
        from_Keysyms_गणनम्++;
        from_Actions_गणनम्++;
    }
    यदि (from_Keysyms_गणनम्) {
        /* Reset defined keysyms field if we used no keysym from `into` */
        यदि (from_Keysyms_गणनम् == darray_आकार(into->स्तराः))
            into->defined &= ~गणः_क्षेत्रम्_SYMS;
        into->defined |= (from->defined & गणः_क्षेत्रम्_SYMS);
    }
    यदि (from_Actions_गणनम्) {
        /* Reset defined actions field if we used no action from `into` */
        यदि (from_Actions_गणनम् == darray_आकार(into->स्तराः))
            into->defined &= ~गणः_क्षेत्रम्_ACTS;
        into->defined |= (from->defined & गणः_क्षेत्रम्_ACTS);
    }

    प्रत्यागम true;
}

स्थिर bool
Use_New_कुञ्चिका_क्षेत्रम्(गणना कुञ्चिका_क्षेत्रम् क्षेत्रम्_2, गणना कुञ्चिका_क्षेत्रम् old, गणना कुञ्चिका_क्षेत्रम् new,
               bool clobber, bool report, गणना कुञ्चिका_क्षेत्रम् *collide)
{
    यदि (!(old & क्षेत्रम्_2))
        प्रत्यागम (new & क्षेत्रम्_2);

    यदि (new & क्षेत्रम्_2) {
        यदि (report)
            *collide |= क्षेत्रम्_2;

        प्रत्यागम clobber;
    }

    प्रत्यागम false;
}

स्थिर bool
overlays_get(ध्रुव कुञ्चिका_Info *info, xkb_overlay_सूचकांक_t bit,
             ध्रुव संरचना xkb_कुञ्चिका **कुञ्चिका_बहिर्गमनम्)
{
    यदि (bit >= (xkb_overlay_सूचकांक_t)(परिमाण(xkb_overlay_आवरणम्_t) * CHAR_BIT))
        प्रत्यागम false;
    ध्रुव xkb_overlay_आवरणम्_t आवरणम् = (xkb_overlay_आवरणम्_t)(1u << bit);
    यदि (!(info->overlays & आवरणम्)) प्रत्यागम false;
    यदि (कुञ्चिका_बहिर्गमनम्) {
        यदि (!info->overlays_alloc) {
            /* Stored inline */
            *कुञ्चिका_बहिर्गमनम् = info->overlay_कुञ्चिका;
        } अन्यथा {
            /* Stored in sparse array */
            ध्रुव xkb_overlay_आवरणम्_t नीचः = (xkb_overlay_आवरणम्_t)(
                info->overlays & (आवरणम् - 1u)
            );
            ध्रुव xkb_overlay_सूचकांक_t सूचकांक = (xkb_overlay_सूचकांक_t)popcount32(नीचः);
            *कुञ्चिका_बहिर्गमनम् = info->overlays_कुञ्चिकाः[सूचकांक];
        }
    }
    प्रत्यागम true;
}

स्थिर bool
overlays_निवेशय(कुञ्चिका_Info *keyi, xkb_overlay_सूचकांक_t bit,
                ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका)
{
    यदि (bit >= (xkb_overlay_सूचकांक_t)(परिमाण(xkb_overlay_आवरणम्_t) * CHAR_BIT))
        प्रत्यागम false;

    ध्रुव xkb_overlay_आवरणम्_t आवरणम् = (xkb_overlay_आवरणम्_t)(1u << bit);

    यदि ((keyi->overlays & आवरणम्) && !keyi->overlays_clear) {
        /* Overwrite existing slot */
        यदि (!keyi->overlays_alloc) {
            /* Stored inline */
            keyi->overlay_कुञ्चिका = कुञ्चिका;
            यदि (!कुञ्चिका)
                keyi->overlays_clear = true;
        } अन्यथा {
            /* Stored in sparse array */
            ध्रुव xkb_overlay_आवरणम्_t नीचः = (xkb_overlay_आवरणम्_t)(
                keyi->overlays & (xkb_overlay_आवरणम्_t)(आवरणम् - 1u)
            );
            /* index = number of set bits strictly below bit */
            ध्रुव xkb_overlay_सूचकांक_t सूचकांक =
                (xkb_overlay_सूचकांक_t)popcount32(नीचः);
            keyi->overlays_कुञ्चिकाः[सूचकांक] = कुञ्चिका;
        }
        प्रत्यागम true;
    }

    /* Insert new element */
    यदि (!keyi->overlays || (keyi->overlays_clear && !कुञ्चिका)) {
        /* No previous overlay or only cleared overlays: use inline storage */
        keyi->overlay_कुञ्चिका = कुञ्चिका;
        keyi->overlays |= आवरणम्;
        keyi->overlays_clear = !कुञ्चिका;
    } अन्यथा यदि (!keyi->overlays_alloc) {
        /* Promote inline element to heap */
        ध्रुव xkb_overlay_आवरणम्_t overlays = keyi->overlays | आवरणम्;
        ध्रुव xkb_overlay_सूचकांक_t alloc =
            (xkb_overlay_सूचकांक_t)popcount32(overlays);

        /* Alloc sparse array */
        ध्रुव संरचना xkb_कुञ्चिका ** ध्रुव tmp = शून्यस्मृतिं_देहि(alloc, परिमाण(*tmp));
        यदि (!tmp) प्रत्यागम false;

        /* Copy previous entries to the buffer */
        यदि (!keyi->overlays_clear) {
            /* A previous single overlay was set */
            ध्रुव xkb_overlay_आवरणम्_t नीचः = (xkb_overlay_आवरणम्_t)(
                overlays & (xkb_overlay_आवरणम्_t)(keyi->overlays - 1u)
            );
            ध्रुव xkb_overlay_सूचकांक_t idx = (xkb_overlay_सूचकांक_t)popcount32(नीचः);
            tmp[idx] = keyi->overlay_कुञ्चिका;
        } अन्यथा {
            /* All previous overlays were set to none: set thanks to calloc */
        }

        /* Insert new entry into the buffer */
        ध्रुव xkb_overlay_आवरणम्_t नीचः = (xkb_overlay_आवरणम्_t)(
            overlays & (xkb_overlay_आवरणम्_t)(आवरणम् - 1u)
        );
        ध्रुव xkb_overlay_सूचकांक_t idx = (xkb_overlay_सूचकांक_t)popcount32(नीचः);
        tmp[idx] = कुञ्चिका;

        keyi->overlays_कुञ्चिकाः = tmp;
        keyi->overlays_alloc = alloc;
        keyi->overlays = overlays;
        keyi->overlays_clear = false;
    } अन्यथा {
        /* Allocated items: grow heap and shift if relevant */
        ध्रुव xkb_overlay_आवरणम्_t overlays =
            (xkb_overlay_आवरणम्_t)(keyi->overlays | आवरणम्);
        ध्रुव xkb_overlay_सूचकांक_t गणनम् =
            (xkb_overlay_सूचकांक_t)popcount32(overlays);
        यदि (गणनम् > keyi->overlays_alloc) {
            ध्रुव xkb_overlay_सूचकांक_t alloc =
                (xkb_overlay_सूचकांक_t)अग्र_pow2(गणनम्);
            प्रतिज्ञा(alloc >= गणनम्);
            ध्रुव संरचना xkb_कुञ्चिका ** ध्रुव tmp = realloc(
                keyi->overlays_कुञ्चिकाः,
                (आकार_t)alloc * परिमाण(*keyi->overlays_कुञ्चिकाः)
            );
            यदि (!tmp) प्रत्यागम false;
            keyi->overlays_कुञ्चिकाः = tmp;
            keyi->overlays_alloc = alloc;
        }
        /* Get new entry index in sparse array */
        ध्रुव xkb_overlay_आवरणम्_t नीचः = (xkb_overlay_आवरणम्_t)(
            overlays & (xkb_overlay_आवरणम्_t)(आवरणम् - 1u)
        );
        ध्रुव xkb_overlay_सूचकांक_t सूचकांक = (xkb_overlay_सूचकांक_t)popcount32(नीचः);
        यदि (सूचकांक >= keyi->overlays_alloc)
            PANIC_UNREACHABLE();
        यदि (सूचकांक < गणनम् - 1u /* previous count */) {
            /* insert between 2 previous items: move items from greater bits */
            प्रतिज्ञा(सूचकांक + 1 < keyi->overlays_alloc);
            memmove(keyi->overlays_कुञ्चिकाः + सूचकांक + 1, keyi->overlays_कुञ्चिकाः + सूचकांक,
                    (आकार_t)(गणनम् - 1u - सूचकांक) * परिमाण(ध्रुव संरचना xkb_कुञ्चिका *));
        }
        keyi->overlays_कुञ्चिकाः[सूचकांक] = कुञ्चिका;
        keyi->overlays = overlays;
    }

    प्रत्यागम true;
}

/** Merge keyboard overlays. At most one can be defined (X11 limitation) */
स्थिर bool
merge_overlays(SymbolsInfo *info, कुञ्चिका_Info *into, कुञ्चिका_Info *from,
               bool clobber, bool report, गणना कुञ्चिका_क्षेत्रम् *collide)
{
    यदि (!(from->defined & कुञ्चिका_क्षेत्रम्_OVERLAY)) {
        /* `from` is empty: do nothing */
    } अन्यथा यदि (!(into->defined & कुञ्चिका_क्षेत्रम्_OVERLAY)) {
        /* `into` is empty: steal `from` */
        into->overlays = from->overlays;
        into->overlays_कुञ्चिकाः = from->overlays_कुञ्चिकाः;
        from->overlays_कुञ्चिकाः = रिक्त;
        into->overlays_alloc = from->overlays_alloc;
        from->overlays_alloc = 0;
        into->defined |= कुञ्चिका_क्षेत्रम्_OVERLAY;
    } अन्यथा यदि (into->overlays_clear && from->overlays_clear) {
        /* Only cleared overlays: combine them */
        into->overlays |= from->overlays;
    } अन्यथा {
        यदि (info->keymap_info->features.overlapping_overlays) {
            /* Overlapping overlays */

            ध्रुव xkb_overlay_आवरणम्_t परिणाम_आवरणम् =
                (into->overlays | from->overlays);
            ध्रुव xkb_overlay_सूचकांक_t गणनम् =
                (xkb_overlay_सूचकांक_t)popcount32(परिणाम_आवरणम्);

            यदि (गणनम् == 0)
                PANIC_UNREACHABLE();

            /*
             * Pick the best buffer to write. Prefer the larger one to avoid
             * future realloc; only realloc if both are too small. The unchosen
             * buffer may be freed.
             */

            कुञ्चिका_Info *dest = into;
            कुञ्चिका_Info *src = from;
            यदि (into->overlays_alloc >= गणनम् &&
                into->overlays_alloc >= from->overlays_alloc) {
                /* Do nothing */
            } अन्यथा यदि (from->overlays_alloc >= गणनम्) {
                dest = from;
                src = into;
                clobber = !clobber;
            } अन्यथा यदि (गणनम् > 1) {
                /* Neither is sufficient: realloc the larger one if possible */
                यदि (into->overlays_alloc < from->overlays_alloc) {
                    dest = from;
                    src = into;
                    clobber = !clobber;
                }

                यदि (!dest->overlays_alloc) {
                    ध्रुव संरचना xkb_कुञ्चिका **tmp = शून्यस्मृतिं_देहि(गणनम्, परिमाण(*tmp));
                    यदि (!tmp)
                        प्रत्यागम false;
                    यदि (dest->overlay_कुञ्चिका)
                        tmp[0] = dest->overlay_कुञ्चिका;
                    dest->overlays_कुञ्चिकाः = tmp;
                    dest->overlays_alloc = गणनम्;
                } अन्यथा {
                    ध्रुव संरचना xkb_कुञ्चिका **tmp =
                        realloc(dest->overlays_कुञ्चिकाः,
                                गणनम् * परिमाण(*dest->overlays_कुञ्चिकाः));
                    यदि (!tmp)
                        प्रत्यागम false;
                    dest->overlays_कुञ्चिकाः = tmp;
                    dest->overlays_alloc = गणनम्;
                }
            }

            xkb_overlay_आवरणम्_t remaining = src->overlays;
            ध्रुव संरचना xkb_कुञ्चिका **src_कुञ्चिकाः = (src->overlays_clear)
                ? रिक्त
                : (src->overlays_alloc)
                    ? src->overlays_कुञ्चिकाः
                    : &src->overlay_कुञ्चिका;
            यावत् (remaining) {
                /* isolate lowest set bit */
                ध्रुव xkb_overlay_आवरणम्_t lsb = (xkb_overlay_आवरणम्_t)(
                    remaining &
                    (xkb_overlay_आवरणम्_t)((अचिह्नित)~remaining + 1u)
                );
                /* get its index */
                ध्रुव xkb_overlay_सूचकांक_t bit =
                    (xkb_overlay_सूचकांक_t)popcount32(lsb - 1u);
                remaining = (remaining & ~lsb);

                /* pop current value */
                यदि (!src->overlays_clear && !src->overlays_alloc && remaining)
                    PANIC_UNREACHABLE();
                ध्रुव संरचना xkb_कुञ्चिका *src_कुञ्चिका = (src_कुञ्चिकाः)
                    ? *(src_कुञ्चिकाः++)
                    : रिक्त;

                /* check dest */
                ध्रुव संरचना xkb_कुञ्चिका *dest_कुञ्चिका = रिक्त;
                ध्रुव bool conflict = overlays_get(dest, bit, &dest_कुञ्चिका);
                यदि (conflict) {
                    यदि (dest_कुञ्चिका == src_कुञ्चिका)
                       अनुवर्त;
                    अन्यथा यदि (report)
                        *collide |= कुञ्चिका_क्षेत्रम्_OVERLAY;
                }

                यदि ((!conflict || clobber) && !overlays_निवेशय(dest, bit, src_कुञ्चिका))
                    प्रत्यागम false;
            }

            यदि (dest == from) {
                /* `from` buffer was used; clone it into `into` then clear it */
                यदि (into->overlays_alloc)
                    मुक्त(into->overlays_कुञ्चिकाः);
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                यदि (from->overlays_alloc) {
                    /* Steal */
                    into->overlays_कुञ्चिकाः = from->overlays_कुञ्चिकाः;
                    from->overlays_कुञ्चिकाः = रिक्त;
                    from->overlays_alloc = 0;
                } अन्यथा {
                    into->overlay_कुञ्चिका = from->overlay_कुञ्चिका;
                }
            }
        } अन्यथा {
            /* Disjoint overlays */

            यदि (into->overlays == from->overlays &&
                into->overlays_clear == from->overlays_clear &&
                into->overlay_कुञ्चिका == from->overlay_कुञ्चिका) {
                /* Indentical: do nothing */
                प्रत्यागम true;
            }
            यदि (!(into->overlays & from->overlays)) {
                यदि (into->overlays_clear) {
                    /* `into` clears and `from` sets distinct overlays: use `from` */
                    into->overlays = from->overlays;
                    into->overlays_clear = from->overlays_clear;
                    into->overlays_alloc = from->overlays_alloc;
                    into->overlay_कुञ्चिका = from->overlay_कुञ्चिका;
                    प्रत्यागम true;
                } अन्यथा यदि (from->overlays_clear) {
                    /* `into` sets and `from` clears distinct overlays: use `into` */
                    प्रत्यागम true;
                }
            }
            /* Conflict */
            यदि (report)
                *collide |= कुञ्चिका_क्षेत्रम्_OVERLAY;
            यदि (clobber) {
                into->overlays = from->overlays;
                into->overlays_clear = from->overlays_clear;
                into->overlays_alloc = from->overlays_alloc;
                into->overlay_कुञ्चिका = from->overlay_कुञ्चिका;
            }
        }
    }
    प्रत्यागम true;
}

स्थिर bool
Merge_कुञ्चिकाः(SymbolsInfo *info, कुञ्चिका_Info *into, कुञ्चिका_Info *from, bool same_file)
{
    xkb_layout_सूचकांक_t i;
    xkb_layout_सूचकांक_t groups_अन्तर्_both;
    गणना कुञ्चिका_क्षेत्रम् collide = 0;
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    ध्रुव bool clobber = (from->merge != MERGE_AUGMENT);
    ध्रुव bool report = (same_file && verbosity > 0) || verbosity > 9;

    यदि (from->merge == MERGE_REPLACE) {
        Clear_कुञ्चिका_Info(into);
        *into = *from;
        Init_कुञ्चिका_Info(info->ctx, from);
        प्रत्यागम true;
    }

    groups_अन्तर्_both = MIN(darray_आकार(into->groups), darray_आकार(from->groups));
    प्रति (i = 0; i < groups_अन्तर्_both; i++)
        MergeGroups(info,
                    &darray_item(into->groups, i),
                    &darray_item(from->groups, i),
                    clobber, report, i, into->name);
    /* If @from has extra groups, just move them to @into. */
    प्रति (i = groups_अन्तर्_both; i < darray_आकार(from->groups); i++) {
        darray_append(into->groups, darray_item(from->groups, i));
        Init_गणः_Info(&darray_item(from->groups, i));
    }

    यदि (Use_New_कुञ्चिका_क्षेत्रम्(कुञ्चिका_क्षेत्रम्_VMODMAP, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->vmodmap = from->vmodmap;
        into->defined |= कुञ्चिका_क्षेत्रम्_VMODMAP;
    }
    यदि (Use_New_कुञ्चिका_क्षेत्रम्(कुञ्चिका_क्षेत्रम्_REPEAT, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->repeat = from->repeat;
        into->defined |= कुञ्चिका_क्षेत्रम्_REPEAT;
    }
    यदि (Use_New_कुञ्चिका_क्षेत्रम्(कुञ्चिका_क्षेत्रम्_DEFAULT_TYPE, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->default_type = from->default_type;
        into->defined |= कुञ्चिका_क्षेत्रम्_DEFAULT_TYPE;
    }
    यदि (Use_New_कुञ्चिका_क्षेत्रम्(कुञ्चिका_क्षेत्रम्_GROUPINFO, into->defined, from->defined,
                       clobber, report, &collide)) {
        into->बहिर्गमनम्_सम्बन्धि_range_pending_गणः = from->बहिर्गमनम्_सम्बन्धि_range_pending_गणः;
        into->बहिर्गमनम्_सम्बन्धि_range_गणः_policy = from->बहिर्गमनम्_सम्बन्धि_range_गणः_policy;
        into->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या = from->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या;
        into->defined |= कुञ्चिका_क्षेत्रम्_GROUPINFO;
    }

    यदि (!merge_overlays(info, into, from, clobber, report, &collide))
        प्रत्यागम false;

    यदि (collide) {
        log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_FIELDS,
                 "Symbol map for key %s redefined; "
                 "Using %s definition for conflicting fields\n",
                 कुञ्चिका_Name_Text(info->ctx, into->name),
                 (clobber ? "first" : "last"));
    }

    Clear_कुञ्चिका_Info(from);
    Init_कुञ्चिका_Info(info->ctx, from);
    प्रत्यागम true;
}

स्थिर xkb_atom_t
Xkb_Resolve_कुञ्चिका_Alias(ध्रुव संरचना xkb_keymap *keymap, xkb_atom_t name)
{
    यदि (name < keymap->अङ्कः_कुञ्चिका_नामानि) {
        ध्रुव Keycode_मेलनम् मेलनम् = keymap->कुञ्चिका_नामानि[name];
        यदि (मेलनम्.प्राप्तम् && मेलनम्.is_alias) {
            प्रत्यागम मेलनम्.alias.real;
        }
    }
    प्रत्यागम name;
}

/* TODO: Make it so this function doesn't need the entire keymap. */
स्थिर bool
सङ्कलय_कुञ्चिका_Symbols(SymbolsInfo *info, कुञ्चिका_Info *keyi, bool same_file)
{
    /*
     * Don't keep aliases in the keys array; this guarantees that
     * searching for keys to merge with by straight comparison (see the
     * following loop) is enough, and we won't get multiple KeyInfo's
     * for the same key because of aliases.
     */
    keyi->name = Xkb_Resolve_कुञ्चिका_Alias(&info->keymap_info->keymap, keyi->name);

    कुञ्चिका_Info *iter;
    darray_foreach(iter, info->कुञ्चिकाः)
        यदि (iter->name == keyi->name)
            प्रत्यागम Merge_कुञ्चिकाः(info, iter, keyi, same_file);

    darray_append(info->कुञ्चिकाः, *keyi);
    Init_कुञ्चिका_Info(info->ctx, keyi);
    प्रत्यागम true;
}

स्थिर bool
सङ्कलय_Mod_Map_प्रविष्टिः(SymbolsInfo *info, ध्रुव Mod_Map_प्रविष्टिः *new)
{
    ध्रुव bool clobber = (new->merge != MERGE_AUGMENT);

    Mod_Map_प्रविष्टिः *old;
    darray_foreach(old, info->modmaps) {
        यदि ((new->have_चिह्नम् != old->have_चिह्नम्) ||
            (new->have_चिह्नम् && new->u.कुञ्चिका_Sym != old->u.कुञ्चिका_Sym) ||
            (!new->have_चिह्नम् && new->u.कुञ्चिका_Name != old->u.कुञ्चिका_Name))
            अनुवर्त;

        यदि (new->mods == old->mods)
            प्रत्यागम true;

        ध्रुव xkb_mod_आवरणम्_t use = (clobber ? new->mods : old->mods);
        ध्रुव xkb_mod_आवरणम्_t ignore = (clobber ? old->mods : new->mods);

        यदि (new->have_चिह्नम्) {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Symbol \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    KeysymText(info->ctx, new->u.कुञ्चिका_Sym),
                    Mod_आवरणम्_Text(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_आवरणम्_Text(info->ctx, MOD_REAL, &info->mods, ignore));
        } अन्यथा {
            log_warn(info->ctx, XKB_WARNING_CONFLICTING_MODMAP,
                    "Key \"%s\" added to modifier map "
                    "for distinct modifiers mask; Using %s, ignoring %s\n",
                    कुञ्चिका_Name_Text(info->ctx, new->u.कुञ्चिका_Name),
                    Mod_आवरणम्_Text(info->ctx, MOD_REAL, &info->mods, use),
                    Mod_आवरणम्_Text(info->ctx, MOD_REAL, &info->mods, ignore));
        }
        old->mods = use;
        प्रत्यागम true;
    }

    darray_append(info->modmaps, *new);

    प्रत्यागम true;
}

/***====================================================================***/

स्थिर शून्य
MergeIncludedSymbols(SymbolsInfo *into, SymbolsInfo *from,
                     गणना merge_बहुलकम् merge)
{
    xkb_atom_t *गणः_name;
    xkb_layout_सूचकांक_t गणः_नामानि_अन्तर्_both;

    यदि (from->error_गणनम् > 0) {
        into->error_गणनम् += from->error_गणनम्;
        प्रत्यागम;
    }

    MergeModSets(into->ctx, &into->mods, &from->mods, merge);

    यदि (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    गणः_नामानि_अन्तर्_both = MIN(darray_आकार(into->गणः_नामानि),
                              darray_आकार(from->गणः_नामानि));
    प्रति (xkb_layout_सूचकांक_t i = 0; i < गणः_नामानि_अन्तर्_both; i++) {
        यदि (!darray_item(from->गणः_नामानि, i))
            अनुवर्त;

        यदि (merge == MERGE_AUGMENT && darray_item(into->गणः_नामानि, i))
            अनुवर्त;

        darray_item(into->गणः_नामानि, i) = darray_item(from->गणः_नामानि, i);
    }
    /* If @from has more, get them as well. */
    darray_foreach_from(गणः_name, from->गणः_नामानि, गणः_नामानि_अन्तर्_both)
        darray_append(into->गणः_नामानि, *गणः_name);

    यदि (darray_रिक्तानि(into->कुञ्चिकाः)) {
        into->कुञ्चिकाः = from->कुञ्चिकाः;
        darray_init(from->कुञ्चिकाः);
    }
    अन्यथा {
        कुञ्चिका_Info *keyi;
        darray_foreach(keyi, from->कुञ्चिकाः) {
            keyi->merge = merge;
            यदि (!सङ्कलय_कुञ्चिका_Symbols(into, keyi, false))
                into->error_गणनम्++;
        }
    }

    यदि (darray_रिक्तानि(into->modmaps)) {
        into->modmaps = from->modmaps;
        darray_init(from->modmaps);
    }
    अन्यथा {
        Mod_Map_प्रविष्टिः *mm;
        darray_foreach(mm, from->modmaps) {
            mm->merge = merge;
            यदि (!सङ्कलय_Mod_Map_प्रविष्टिः(into, mm))
                into->error_गणनम्++;
        }
    }
}

स्थिर शून्य
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file);

स्थिर bool
HandleIncludeSymbols(SymbolsInfo *info, IncludeStmt *include)
{
    SymbolsInfo included;

    यदि (Exceeds_Include_Max_गभीरता(info->ctx, info->include_गभीरता)) {
        info->error_गणनम् += 10;
        प्रत्यागम false;
    }

    InitSymbolsInfo(&included, info->keymap_info, info->include_गभीरता + 1,
                    &info->mods);
    included.name = steal(&include->stmt);

    ध्रुव संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    प्रति (IncludeStmt *stmt = include; stmt; stmt = stmt->अग्र_incl) {
        SymbolsInfo अग्र_incl;
        XkbFile *file;

        वर्ण मार्गः[PATH_MAX];
        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_SYMBOLS,
                                  मार्गः, परिमाण(मार्गः));
        यदि (!file) {
            info->error_गणनम् += 10;
            ClearSymbolsInfo(&included);
            प्रत्यागम false;
        }

        InitSymbolsInfo(&अग्र_incl, info->keymap_info, info->include_गभीरता + 1,
                        &included.mods);
        यदि (stmt->modifier) {
            ध्रुव पूर्णाङ्क गणनम् = parse_dec_गन्तव्यम्_uint32_t(stmt->modifier, SIZE_MAX,
                                                    &अग्र_incl.explicit_गणः);
            यदि (गणनम् <= 0 || *(stmt->modifier + गणनम्) != '\0' ||
                अग्र_incl.explicit_गणः == 0 ||
                अग्र_incl.explicit_गणः > info->max_groups) {
                log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                        "Cannot set explicit group to \"%s\": "
                        "must be between 1..%"PRIu32"; Ignoring group number\n",
                        stmt->modifier, info->max_groups);
                अग्र_incl.explicit_गणः = info->explicit_गणः;
            } अन्यथा {
                अग्र_incl.explicit_गणः--;
            }
        }
        अन्यथा यदि (info->keymap_info->keymap.अङ्कः_groups != 0 &&
                 अग्र_incl.include_गभीरता == 1) {
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
            अग्र_incl.explicit_गणः = 0;
        }
        अन्यथा {
            /* The keymap was not generated from rules or this is not the first
             * level of include: take the parent’s explicit group. */
            अग्र_incl.explicit_गणः = info->explicit_गणः;
        }

        HandleSymbolsFile(&अग्र_incl, file);

        MergeIncludedSymbols(&included, &अग्र_incl, stmt->merge);

        ClearSymbolsInfo(&अग्र_incl);
        FreeXkbFile(file);
    }

    MergeIncludedSymbols(info, &included, include->merge);
    ClearSymbolsInfo(&included);

    प्रत्यागम (info->error_गणनम् == 0);
}

स्थिर bool
Get_गणः_सूचकांक(SymbolsInfo *info, कुञ्चिका_Info *keyi, व्यञ्जकम्_Def *arrayNdx,
              गणना गणः_क्षेत्रम् क्षेत्रम्_2, xkb_layout_सूचकांक_t *ndx_rtrn)
{
    प्रतिज्ञा (क्षेत्रम्_2 == गणः_क्षेत्रम्_SYMS || क्षेत्रम्_2 == गणः_क्षेत्रम्_ACTS);
    ध्रुव वर्ण *name = (क्षेत्रम्_2 == गणः_क्षेत्रम्_SYMS ? "symbols" : "actions");

    यदि (arrayNdx == रिक्त) {
        xkb_layout_सूचकांक_t i = 0;
        गणः_Info_2 *groupi;

        darray_enumerate(i, groupi, keyi->groups) {
            यदि (!(groupi->defined & क्षेत्रम्_2)) {
                *ndx_rtrn = i;
                प्रत्यागम true;
            }
        }

        यदि (i >= info->max_groups) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                    "Too many groups of %s for key %s (max %"PRIu32"); "
                    "Ignoring %s defined for extra groups\n",
                    name, कुञ्चिका_Info_Text(info, keyi), info->max_groups, name);
            प्रत्यागम false;
        }

        darray_resize0(keyi->groups, darray_आकार(keyi->groups) + 1);
        *ndx_rtrn = darray_आकार(keyi->groups) - 1;
        प्रत्यागम true;
    }

    यदि (व्यञ्जकम्_Resolve_गणः(info->keymap_info, arrayNdx, false, ndx_rtrn, रिक्त) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "Illegal group index for %s of key %s\n"
                "Definition with non-integer array index ignored\n",
                name, कुञ्चिका_Info_Text(info, keyi));
        प्रत्यागम false;
    }

    (*ndx_rtrn)--;
    यदि (*ndx_rtrn >= darray_आकार(keyi->groups))
        darray_resize0(keyi->groups, *ndx_rtrn + 1);

    प्रत्यागम true;
}

स्थिर bool
सङ्कलय_Symbols_गन्तव्यम्_कुञ्चिका(SymbolsInfo *info, कुञ्चिका_Info *keyi, व्यञ्जकम्_Def *arrayNdx,
                व्यञ्जकम्_Def *मानम्)
{
    xkb_layout_सूचकांक_t ndx = 0;

    यदि (!Get_गणः_सूचकांक(info, keyi, arrayNdx, गणः_क्षेत्रम्_SYMS, &ndx))
        प्रत्यागम false;

    गणः_Info_2 *groupi = &darray_item(keyi->groups, ndx);

    यदि (मानम्->common.type == STMT_व्यञ्जकम्_रिक्तानि_LIST) {
        groupi->defined |= गणः_क्षेत्रम्_SYMS;
        प्रत्यागम true;
    }

    यदि (मानम्->common.type != STMT_व्यञ्जकम्_KEYSYM_LIST) {
        log_err(info->ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Expected a list of symbols, found %s; "
                "Ignoring symbols for group %"PRIu32" of %s\n",
                stmt_type_गन्तव्यम्_string(मानम्->common.type), ndx + 1,
                कुञ्चिका_Info_Text(info, keyi));
        प्रत्यागम false;
    }

    यदि (groupi->defined & गणः_क्षेत्रम्_SYMS) {
        log_err(info->ctx, XKB_ERROR_CONFLICTING_कुञ्चिका_SYMBOLS_प्रविष्टिः,
                "Symbols for key %s, group %"PRIu32" already defined; "
                "Ignoring duplicate definition\n",
                कुञ्चिका_Info_Text(info, keyi), ndx + 1);
        प्रत्यागम false;
    }

    xkb_level_सूचकांक_t n_स्तराः = 0;
    xkb_level_सूचकांक_t non_रिक्तानि_स्तराः = 0;
    /* Contrary to actions, keysyms are already parsed at this point so we drop
     * trailing symbols by not adding them in the first place. */
    प्रति (व्यञ्जकम्_Keysym_List *keysymList = (व्यञ्जकम्_Keysym_List *) मानम्;
         keysymList;
         keysymList = (व्यञ्जकम्_Keysym_List *) keysymList->common.अग्र) {
        n_स्तराः++;
        /* Drop trailing NoSymbol */
        यदि (darray_आकार(keysymList->syms) > 0)
            non_रिक्तानि_स्तराः = n_स्तराः;
    }
    यदि (non_रिक्तानि_स्तराः < n_स्तराः)
        n_स्तराः = non_रिक्तानि_स्तराः;
    यदि (darray_आकार(groupi->स्तराः) < n_स्तराः)
        darray_resize0(groupi->स्तराः, n_स्तराः);

    groupi->defined |= गणः_क्षेत्रम्_SYMS;

    xkb_level_सूचकांक_t level = 0;
    प्रति (व्यञ्जकम्_Keysym_List *keysymList = (व्यञ्जकम्_Keysym_List *) मानम्;
         keysymList && level < n_स्तराः;
         keysymList = (व्यञ्जकम्_Keysym_List *) keysymList->common.अग्र, level++) {
        संरचना xkb_level *leveli = &darray_item(groupi->स्तराः, level);
        प्रतिज्ञा(leveli->अङ्कः_syms == 0);

        यदि (unlikely(darray_आकार(keysymList->syms) > MAX_KEYSYMS_प्रत्येकम्_LEVEL)) {
            log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Key %s has too many keysyms for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    कुञ्चिका_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_KEYSYMS_प्रत्येकम्_LEVEL, darray_आकार(keysymList->syms));
            प्रत्यागम false;
        }

        leveli->अङ्कः_syms = (xkb_keysym_गणनम्_t) darray_आकार(keysymList->syms);
        विकल्प (leveli->अङ्कः_syms) {
        पक्ष 0:
            leveli->s.sym = XKB_कुञ्चिका_No_चिह्नम्;
            भङ्ग;
        पक्ष 1:
            leveli->s.sym = darray_item(keysymList->syms, 0);
            प्रतिज्ञा(leveli->s.sym != XKB_कुञ्चिका_No_चिह्नम्);
            भङ्ग;
        स्वभाव:
            darray_shrink(keysymList->syms);
            darray_steal(keysymList->syms, &leveli->s.syms, रिक्त);
#यदि_अपरिभाषितम् NDEBUG
            /* Canonical list: all NoSymbol were dropped */
            प्रति (xkb_keysym_गणनम्_t k = 0; k < leveli->अङ्कः_syms; k++)
                प्रतिज्ञा(leveli->s.syms[k] != XKB_कुञ्चिका_No_चिह्नम्);
#यदि_अन्तः
        }
    }

    प्रत्यागम true;
}

स्थिर bool
सङ्कलय_Actions_गन्तव्यम्_कुञ्चिका(SymbolsInfo *info, कुञ्चिका_Info *keyi, व्यञ्जकम्_Def *arrayNdx,
                व्यञ्जकम्_Def *मानम्)
{
    xkb_layout_सूचकांक_t ndx = 0;

    यदि (!Get_गणः_सूचकांक(info, keyi, arrayNdx, गणः_क्षेत्रम्_ACTS, &ndx))
        प्रत्यागम false;

    गणः_Info_2 *groupi = &darray_item(keyi->groups, ndx);

    यदि (मानम्->common.type == STMT_व्यञ्जकम्_रिक्तानि_LIST) {
        groupi->defined |= गणः_क्षेत्रम्_ACTS;
        प्रत्यागम true;
    }

    यदि (मानम्->common.type != STMT_व्यञ्जकम्_ACTION_LIST) {
        log_wsgo(info->ctx, XKB_ERROR_INVALID_EXPRESSION_TYPE,
                 "Bad expression type (%d) for action list value; "
                 "Ignoring actions for group %"PRIu32" of %s\n",
                 मानम्->common.type, ndx, कुञ्चिका_Info_Text(info, keyi));
        प्रत्यागम false;
    }

    यदि (groupi->defined & गणः_क्षेत्रम्_ACTS) {
        log_wsgo(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_ACTION,
                 "Actions for key %s, group %"PRIu32" already defined\n",
                 कुञ्चिका_Info_Text(info, keyi), ndx);
        प्रत्यागम false;
    }

    xkb_level_सूचकांक_t n_स्तराः = 0;
    /* Contrary to keysyms with trailing `NoSymbol`, we cannot detect trailing
     * `NoAction()` now, because we need to parse the actions first. Jusr count
     * explicit actions for now. */
    प्रति (ParseCommon *p = &मानम्->common; p; p = p->अग्र)
        n_स्तराः++;
    यदि (darray_आकार(groupi->स्तराः) < n_स्तराः)
        darray_resize0(groupi->स्तराः, n_स्तराः);

    groupi->defined |= गणः_क्षेत्रम्_ACTS;

    xkb_level_सूचकांक_t level = 0;
    xkb_level_सूचकांक_t non_रिक्तानि_स्तराः = 0;
    प्रति (व्यञ्जकम्_Action_List *actionList = (व्यञ्जकम्_Action_List *) मानम्;
         actionList;
         actionList = (व्यञ्जकम्_Action_List *) actionList->common.अग्र, level++) {
        संरचना xkb_level *leveli = &darray_item(groupi->स्तराः, level);
        प्रतिज्ञा(leveli->अङ्कः_actions == 0);

        अचिह्नित पूर्णाङ्क अङ्कः_actions = 0;
        प्रति (व्यञ्जकम्_Def *act = actionList->actions;
             act; act = (व्यञ्जकम्_Def *) act->common.अग्र)
            अङ्कः_actions++;

        यदि (unlikely(अङ्कः_actions > MAX_ACTIONS_प्रत्येकम्_LEVEL)) {
            log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Key %s has too many actions for group %"PRIu32", "
                    "level %"PRIu32"; expected max %u, got: %u\n",
                    कुञ्चिका_Info_Text(info, keyi), ndx + 1, level + 1,
                    MAX_ACTIONS_प्रत्येकम्_LEVEL, अङ्कः_actions);
            प्रत्यागम false;
        }

        /* Parse actions and add only defined actions */
        darray(सङ्घ xkb_action) actions = darray_new();
        प्रति (व्यञ्जकम्_Def *act = actionList->actions;
             act; act = (व्यञ्जकम्_Def *) act->common.अग्र) {
            सङ्घ xkb_action गन्तव्यम्_Act = { 0 };
            ध्रुव गणना xkb_parser_error r =
                HandleActionDef(info->keymap_info, &info->default_actions,
                                &info->mods, act, &गन्तव्यम्_Act);
            यदि (r != PARSER_SUCCESS) {
                log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                        "Illegal action definition for %s; "
                        "Action for group %"PRIu32"/level %"PRIu32" ignored\n",
                        कुञ्चिका_Info_Text(info, keyi), ndx + 1, level + 1);
                यदि (r == PARSER_FATAL_ERROR) {
                    darray_free(actions);
                    प्रत्यागम false;
                } अन्यथा {
                    /* Ensure action type is reset */
                    गन्तव्यम्_Act.type = ACTION_TYPE_NONE;
                }
            }
            यदि (गन्तव्यम्_Act.type == ACTION_TYPE_NONE) {
                /* Drop action */
            } अन्यथा यदि (likely(अङ्कः_actions == 1)) {
                /* Only one action: do not allocate */
                leveli->अङ्कः_actions = 1;
                leveli->a.action = गन्तव्यम्_Act;
                गच्छ अग्र_level;
            } अन्यथा {
                darray_append(actions, गन्तव्यम्_Act);
            }
        }
        यदि (darray_रिक्तानि(actions)) {
            leveli->अङ्कः_actions = 0;
        } अन्यथा यदि (likely(darray_आकार(actions) > 1)) {
            leveli->अङ्कः_actions = (xkb_action_गणनम्_t) darray_आकार(actions);
            darray_shrink(actions);
            darray_steal(actions, &leveli->a.actions, रिक्त);
#यदि_अपरिभाषितम् NDEBUG
            /* Canonical list: all NoAction() were dropped */
            प्रति (xkb_action_गणनम्_t k = 0; k < leveli->अङ्कः_actions; k++)
                प्रतिज्ञा(leveli->a.actions[k].type != ACTION_TYPE_NONE);
#यदि_अन्तः
        } अन्यथा {
            /* Unlikely: some actions were dropped and only one remains */
            प्रतिज्ञा(अङ्कः_actions > darray_आकार(actions));
            leveli->अङ्कः_actions = 1;
            leveli->a.action = darray_item(actions, 0);
            प्रतिज्ञा(leveli->a.action.type != ACTION_TYPE_NONE);
            darray_free(actions);
        }
अग्र_level:
        /*
         * Check trailing `NoAction()`, but count as empty level only if there
         * is no corresponding keysym.
         */
        यदि (leveli->अङ्कः_actions > 0 || leveli->अङ्कः_syms > 0)
            non_रिक्तानि_स्तराः = level + 1;
    }

    यदि (non_रिक्तानि_स्तराः < n_स्तराः) {
        /* Drop trailing `NoAction()`.
         * No need to clear dropped levels: there are no keysyms nor actions */
        darray_आकार(groupi->स्तराः) = non_रिक्तानि_स्तराः;
        यदि (non_रिक्तानि_स्तराः > 0)
            darray_shrink(groupi->स्तराः);
        अन्यथा
            darray_free(groupi->स्तराः);
    }

    प्रत्यागम true;
}

स्थिर ध्रुव Lookup_प्रविष्टिः repeatEntries[] = {
    { "true", कुञ्चिका_REPEAT_YES },
    { "yes", कुञ्चिका_REPEAT_YES },
    { "on", कुञ्चिका_REPEAT_YES },
    { "false", कुञ्चिका_REPEAT_NO },
    { "no", कुञ्चिका_REPEAT_NO },
    { "off", कुञ्चिका_REPEAT_NO },
    { "default", कुञ्चिका_REPEAT_UNDEFINED },
    { रिक्त, 0 }
};

/** Handle overlay<N> = <key> entry */
स्थिर bool
व्यञ्जकम्_Resolve_Overlay_प्रविष्टिः(ध्रुव संरचना xkb_keymap_info *keymap_info,
                        ध्रुव वर्ण *क्षेत्रम्_2, ध्रुव व्यञ्जकम्_Def *arrayNdx,
                        ध्रुव व्यञ्जकम्_Def *व्यञ्जकम्, कुञ्चिका_Info *keyi,
                        xkb_overlay_सूचकांक_t *overlay_rtrn,
                        ध्रुव संरचना xkb_कुञ्चिका **कुञ्चिका_rtrn)
{
    /* Overlay index */
    यदि (arrayNdx) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Overlay field \"%s\" in %s does not support array index; "
                "ignored\n",
                क्षेत्रम्_2, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name));
        प्रत्यागम false;
    }
    static_assert(परिमाण(xkb_overlay_सूचकांक_t) == परिमाण(uint8_t), "");
    ध्रुव आकार_t उपसर्गः = परिमाण("overlay") - 1;
    ध्रुव आकार_t len = वाक्यदैर्घ्यम्(क्षेत्रम्_2 + उपसर्गः);
    int64_t raw_overlay = XKB_OVERLAY_INVALID;
    यदि (parse_dec_गन्तव्यम्_uint64_t(क्षेत्रम्_2 + उपसर्गः, len, (uint64_t *)&raw_overlay)
        != (पूर्णाङ्क)len || raw_overlay < 1 ||
        raw_overlay > (int64_t)keymap_info->features.max_overlays) {
        log_err(keymap_info->keymap.ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_सूचकांक,
                "Unsupported overlay index \"%s\" field for %s: "
                "expected 1..%u, got: %"PRId64"; ignored\n",
                क्षेत्रम्_2, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name),
                keymap_info->features.max_overlays, raw_overlay);
        प्रत्यागम false;
    }
    *overlay_rtrn = (xkb_overlay_सूचकांक_t)raw_overlay - 1;

    /* Overlay key */
    विकल्प (व्यञ्जकम्->common.type) {
    पक्ष STMT_व्यञ्जकम्_KEYNAME_LITERAL:
        *कुञ्चिका_rtrn = Xkb_कुञ्चिका_By_Name(&keymap_info->keymap, व्यञ्जकम्->कुञ्चिका_name.कुञ्चिका_name,
                                 false);
        यदि (!*कुञ्चिका_rtrn) {
            log_err(keymap_info->keymap.ctx, XKB_WARNING_UNDEFINED_KEYCODE,
                    "Unknown key \"%s\" for field %s in %s\n",
                    xkb_atom_text(keymap_info->keymap.ctx, व्यञ्जकम्->कुञ्चिका_name.कुञ्चिका_name),
                    क्षेत्रम्_2, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name));
            प्रत्यागम false;
        }
        प्रत्यागम true;
    पक्ष STMT_व्यञ्जकम्_IDENT: {
        ध्रुव वर्ण * ध्रुव id =
            xkb_atom_text(keymap_info->keymap.ctx, व्यञ्जकम्->ident.ident);
        यदि (id && istreq(id, "none")) {
            /* Explicitly no overlay (clear) */
            *कुञ्चिका_rtrn = रिक्त;
            प्रत्यागम true;
        } अन्यथा यदि (id && istreq(id, "any")) {
            /* Implicitly no overlay */
            *कुञ्चिका_rtrn = रिक्त;
            *overlay_rtrn = XKB_OVERLAY_INVALID;
            प्रत्यागम true;
        }
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मानम्,
                "Unsupported overlay value \"%s\" for field %s in %s\n",
                id, क्षेत्रम्_2, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name));
        प्रत्यागम false;
    }
    स्वभाव:
        log_err(keymap_info->keymap.ctx, XKB_ERROR_INVALID_मानम्,
                "Expected %s for field \"%s\" in %s, got: %s\n",
                stmt_type_गन्तव्यम्_string(STMT_व्यञ्जकम्_KEYNAME_LITERAL),
                क्षेत्रम्_2, कुञ्चिका_Name_Text(keymap_info->keymap.ctx, keyi->name),
                stmt_type_गन्तव्यम्_string(व्यञ्जकम्->common.type));
        प्रत्यागम false;
    }
}

स्थिर bool
समुच्चयः_Symbols_क्षेत्रम्(SymbolsInfo *info, कुञ्चिका_Info *keyi, ध्रुव वर्ण *क्षेत्रम्_2,
                व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def **मानम्_ptr)
{
    व्यञ्जकम्_Def * सीमित ध्रुव मानम् = *मानम्_ptr;
    यदि (istreq(क्षेत्रम्_2, "type")) {
        xkb_layout_सूचकांक_t ndx = 0;
        xkb_atom_t val = XKB_ATOM_NONE;

        यदि (!व्यञ्जकम्_Resolve_String(info->ctx, मानम्, &val)) {
            log_err(info->ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                    "The type field of a key symbol map must be a string; "
                    "Ignoring illegal type definition\n");
            प्रत्यागम false;
        }

        यदि (!arrayNdx) {
            keyi->default_type = val;
            keyi->defined |= कुञ्चिका_क्षेत्रम्_DEFAULT_TYPE;
        }
        अन्यथा यदि (व्यञ्जकम्_Resolve_गणः(info->keymap_info, arrayNdx, false,
                                  &ndx, रिक्त) != PARSER_SUCCESS) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                    "Illegal group index for type of key %s; "
                    "Definition with non-integer array index ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }
        अन्यथा {
            ndx--;
            यदि (ndx >= darray_आकार(keyi->groups)) {
                /* Avoid clang-tidy false positive */
                प्रतिज्ञा(darray_आकार(keyi->groups) < ndx + 1);
                darray_resize0(keyi->groups, ndx + 1);
            }
            darray_item(keyi->groups, ndx).type = val;
            darray_item(keyi->groups, ndx).defined |= गणः_क्षेत्रम्_TYPE;
        }
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "symbols")) {
        प्रत्यागम सङ्कलय_Symbols_गन्तव्यम्_कुञ्चिका(info, keyi, arrayNdx, मानम्);
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "actions")) {
        प्रत्यागम सङ्कलय_Actions_गन्तव्यम्_कुञ्चिका(info, keyi, arrayNdx, मानम्);
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "vmods") ||
             istreq(क्षेत्रम्_2, "virtualmods") ||
             istreq(क्षेत्रम्_2, "virtualmodifiers")) {
        xkb_mod_आवरणम्_t आवरणम् = 0;

        यदि (!व्यञ्जकम्_Resolve_Mod_आवरणम्(info->ctx, मानम्, MOD_VIRT, &info->mods,
                                &आवरणम्)) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_MODIFIER_आवरणम्,
                    "Expected a virtual modifier mask, found %s; "
                    "Ignoring virtual modifiers definition for key %s\n",
                    stmt_type_गन्तव्यम्_string(मानम्->common.type),
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }

        keyi->vmodmap = आवरणम्;
        keyi->defined |= कुञ्चिका_क्षेत्रम्_VMODMAP;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "locking") ||
             istreq(क्षेत्रम्_2, "lock") ||
             istreq(क्षेत्रम्_2, "locks")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_क्षेत्रम्,
                "Key behaviors not supported; "
                "Ignoring locking specification for key %s\n",
                कुञ्चिका_Info_Text(info, keyi));
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "radiogroup") ||
             istreq(क्षेत्रम्_2, "permanentradiogroup") ||
             istreq(क्षेत्रम्_2, "allownone")) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_क्षेत्रम्,
                "Radio groups not supported; "
                "Ignoring radio group specification for key %s\n",
                कुञ्चिका_Info_Text(info, keyi));
    }
    अन्यथा यदि (istreq_उपसर्गः("permanentoverlay", क्षेत्रम्_2)) {
        /*
         * Permanent overlay is ineffectual and serves as documentation of the
         * hardware, so skip it entirely.
         */
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_UNSUPPORTED_SYMBOLS_क्षेत्रम्,
                "Permanent overlays not supported; "
                "Ignoring overlay specification for key %s\n",
                कुञ्चिका_Info_Text(info, keyi));
    }
    अन्यथा यदि (istreq_उपसर्गः("overlay", क्षेत्रम्_2)) {
        xkb_overlay_सूचकांक_t overlay = XKB_OVERLAY_INVALID;
        ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = रिक्त;
        यदि (!व्यञ्जकम्_Resolve_Overlay_प्रविष्टिः(info->keymap_info, क्षेत्रम्_2, arrayNdx,
                                     *मानम्_ptr, keyi, &overlay, &कुञ्चिका))
            प्रत्यागम false;
        यदि (overlay == XKB_OVERLAY_INVALID) {
            /* Overlay has not explicit value: skip */
            प्रत्यागम true;
        } अन्यथा यदि (कुञ्चिका && कुञ्चिका->name == keyi->name) {
            /* Do not overlay to the same key! */
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_LOG_सन्देशः_NO_ID,
                    "Cannot overlay a key to itself; "
                    "Ignoring overlay %u specification for key %s\n",
                    overlay + 1, कुञ्चिका_Info_Text(info, keyi));
        } अन्यथा {
            ध्रुव संरचना xkb_कुञ्चिका *पूर्व;
            यदि (overlays_get(keyi, overlay, &पूर्व)) {
                /* Duplicate entry */
                यदि (कुञ्चिका != पूर्व) {
                    /* Conflicting fields */
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_FIELDS,
                             "Conflicting overlays defined in key %s; "
                             "use overlay%d=%s, ignore overlay%u=%s\n",
                             कुञ्चिका_Info_Text(info, keyi), overlay + 1,
                             (पूर्व
                                 ? कुञ्चिका_Name_Text(info->ctx, पूर्व->name)
                                 : "none"),
                             overlay + 1,
                             (कुञ्चिका
                                 ? कुञ्चिका_Name_Text(info->ctx, कुञ्चिका->name)
                                 : "none"));
                }
            } अन्यथा यदि (info->keymap_info->features.overlapping_overlays) {
                /* Handle overlapping overlays mode */
                यदि (!overlays_निवेशय(keyi, overlay, कुञ्चिका))
                    प्रत्यागम false;
                keyi->defined |= कुञ्चिका_क्षेत्रम्_OVERLAY;
            } अन्यथा {
                /* Handle disjoint overlays mode */
                ध्रुव xkb_overlay_आवरणम्_t आवरणम् = (xkb_overlay_आवरणम्_t)(1u << overlay);
                यदि (!keyi->overlays || keyi->overlays_clear) {
                    /* No overlay previously defined or different overlay cleared */
                    यदि (कुञ्चिका) {
                        /* Replace any explicitly cleared overlay */
                        keyi->overlays = आवरणम्;
                        keyi->overlays_clear = false;
                        keyi->overlay_कुञ्चिका = कुञ्चिका;
                    } अन्यथा {
                        /* Explicitly no overlay (clear) */
                        keyi->overlays |= आवरणम्;
                        keyi->overlays_clear = true;
                    }
                    keyi->defined |= कुञ्चिका_क्षेत्रम्_OVERLAY;
                } अन्यथा यदि (keyi->overlays) {
                    यदि (कुञ्चिका) {
                        log_err(info->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                                "Overlapping overlays are not allowed in %s; "
                                "use overlay%d=%s, ignore overlay%u=%s\n",
                                कुञ्चिका_Info_Text(info, keyi), keyi->overlays,
                                कुञ्चिका_Name_Text(info->ctx, keyi->overlay_कुञ्चिका->name),
                                overlay + 1, कुञ्चिका_Name_Text(info->ctx, कुञ्चिका->name));
                        प्रत्यागम !(info->keymap_info->strict &
                                 PARSER_NO_क्षेत्रम्_मानम्_MISMATCH);
                    } अन्यथा {
                        /* Ignore: the previous overlay is explicitly defined */
                    }
                }
            }
        }
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "repeating") ||
             istreq(क्षेत्रम्_2, "repeats") ||
             istreq(क्षेत्रम्_2, "repeat")) {
        uint32_t val = 0;

        यदि (!व्यञ्जकम्_Resolve_Enum(info->ctx, मानम्, &val, repeatEntries)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                    "Illegal repeat setting for %s; "
                    "Non-boolean repeat setting ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }

        keyi->repeat = (गणना कुञ्चिका_repeat) val;
        keyi->defined |= कुञ्चिका_क्षेत्रम्_REPEAT;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "groupswrap") ||
             istreq(क्षेत्रम्_2, "wrapgroups")) {
        bool समुच्चयः = false;

        यदि (!व्यञ्जकम्_Resolve_Boolean(info->ctx, मानम्, &समुच्चयः)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                    "Illegal groupsWrap setting for %s; "
                    "Non-boolean value ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }

        keyi->बहिर्गमनम्_सम्बन्धि_range_गणः_policy = (समुच्चयः)
            ? XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP
            : XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_CLAMP;
        keyi->defined |= कुञ्चिका_क्षेत्रम्_GROUPINFO;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "groupsclamp") ||
             istreq(क्षेत्रम्_2, "clampgroups")) {
        bool समुच्चयः = false;

        यदि (!व्यञ्जकम्_Resolve_Boolean(info->ctx, मानम्, &समुच्चयः)) {
            log_err(info->ctx, XKB_ERROR_INVALID_मानम्,
                    "Illegal groupsClamp setting for %s; "
                    "Non-boolean value ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }

        keyi->बहिर्गमनम्_सम्बन्धि_range_गणः_policy = (समुच्चयः)
            ? XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_CLAMP
            : XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP;
        keyi->defined |= कुञ्चिका_क्षेत्रम्_GROUPINFO;
    }
    अन्यथा यदि (istreq(क्षेत्रम्_2, "groupsredirect") ||
             istreq(क्षेत्रम्_2, "redirectgroups")) {
        xkb_layout_सूचकांक_t grp = 0;
        bool pending = false;

        // TODO: recover?
        यदि (व्यञ्जकम्_Resolve_गणः(info->keymap_info, मानम्, false, &grp, &pending) !=
            PARSER_SUCCESS && !pending) {
            log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                    "Illegal group index for redirect of key %s; "
                    "Definition with non-integer group ignored\n",
                    कुञ्चिका_Info_Text(info, keyi));
            प्रत्यागम false;
        }

        यदि (pending) {
            keyi->बहिर्गमनम्_सम्बन्धि_range_pending_गणः = true;
            ध्रुव darray_आकार_t pending_सूचकांक =
                darray_आकार(*info->keymap_info->pending_computations);
            darray_append(
                *info->keymap_info->pending_computations,
                (संरचना pending_computation) {
                    .व्यञ्जकम् = *मानम्_ptr,
                    .computed = false,
                    .मानम् = 0,
                }
            );
            *मानम्_ptr = रिक्त;
            static_assert(परिमाण(keyi->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या) ==
                          परिमाण(pending_सूचकांक),
                          "Cannot save pending computation");
            keyi->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या = pending_सूचकांक;
        } अन्यथा {
            keyi->बहिर्गमनम्_सम्बन्धि_range_pending_गणः = false;
            keyi->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या = grp - 1;
        }

        keyi->बहिर्गमनम्_सम्बन्धि_range_गणः_policy = XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT;
        keyi->defined |= कुञ्चिका_क्षेत्रम्_GROUPINFO;
    }
    अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_क्षेत्रम्,
                "Unknown field \"%s\" in a key; definition ignored\n",
                क्षेत्रम्_2);
        प्रत्यागम !(info->keymap_info->strict & PARSER_NO_UNKNOWN_कुञ्चिका_FIELDS);
    }

    प्रत्यागम true;
}

स्थिर bool
समुच्चयः_गणः_Name(SymbolsInfo *info, व्यञ्जकम्_Def *arrayNdx, व्यञ्जकम्_Def *मानम्,
             गणना merge_बहुलकम् merge)
{
    यदि (!arrayNdx) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                XKB_WARNING_MISSING_SYMBOLS_गणः_NAME_सूचकांक,
                "You must specify an index when specifying a group name; "
                "Group name definition without array subscript ignored\n");
        प्रत्यागम false;
    }

    xkb_layout_सूचकांक_t गणः = 0;
    यदि (व्यञ्जकम्_Resolve_गणः(info->keymap_info, arrayNdx, false, &गणः, रिक्त) !=
        PARSER_SUCCESS) {
        log_err(info->ctx, XKB_ERROR_UNSUPPORTED_LAYOUT_सूचकांक_2,
                "Illegal index in group name definition; "
                "Definition with non-integer array index ignored\n");
        प्रत्यागम false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    यदि (!व्यञ्जकम्_Resolve_String(info->ctx, मानम्, &name)) {
        log_err(info->ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                "Group name must be a string; "
                "Illegal name for group %"PRIu32" ignored\n", गणः);
        प्रत्यागम false;
    }

    xkb_layout_सूचकांक_t गणः_गन्तव्यम्_use;
    यदि (info->explicit_गणः == XKB_LAYOUT_INVALID) {
        गणः_गन्तव्यम्_use = गणः - 1;
    }
    अन्यथा यदि (गणः - 1 == 0) {
        गणः_गन्तव्यम्_use = info->explicit_गणः;
    }
    अन्यथा {
        log_warn(info->ctx, XKB_WARNING_NON_आधारः_गणः_NAME,
                 "An explicit group was specified for the '%s' map, "
                 "but it provides a name for a group other than Group1 "
                 "(%"PRIu32"); Ignoring group name '%s'\n",
                 info->name, गणः,
                 xkb_atom_text(info->ctx, name));
        प्रत्यागम false;
    }

    यदि (गणः_गन्तव्यम्_use >= darray_आकार(info->गणः_नामानि)) {
        /* Avoid clang-tidy false positive */
        प्रतिज्ञा(darray_आकार(info->गणः_नामानि) < गणः_गन्तव्यम्_use + 1);
        darray_resize0(info->गणः_नामानि, गणः_गन्तव्यम्_use + 1);
    } अन्यथा {
        ध्रुव xkb_atom_t old_name = darray_item(info->गणः_नामानि, गणः_गन्तव्यम्_use);
        यदि (old_name != XKB_ATOM_NONE && old_name != name) {
            ध्रुव bool replace = (merge != MERGE_AUGMENT);
            ध्रुव xkb_atom_t use    = (replace ? name : old_name);
            ध्रुव xkb_atom_t ignore = (replace ? old_name : name);
            log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                     "Multiple definitions of group %"PRIu32" name "
                     "in map '%s'; Using '%s', ignoring '%s'\n",
                     गणः_गन्तव्यम्_use, info->name,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
            name = use;
        }
    }
    darray_item(info->गणः_नामानि, गणः_गन्तव्यम्_use) = name;

    प्रत्यागम true;
}

स्थिर bool
HandleGlobalVar(SymbolsInfo *info, VarDef *stmt)
{
    ध्रुव वर्ण *elem, *क्षेत्रम्_2;
    व्यञ्जकम्_Def *arrayNdx;
    bool ret;

    यदि (!व्यञ्जकम्_Resolve_Lhs(info->ctx, stmt->name, &elem, &क्षेत्रम्_2, &arrayNdx))
        प्रत्यागम false;

    यदि (elem && istreq(elem, "key")) {
        कुञ्चिका_Info अस्थायी = {0};
        Init_कुञ्चिका_Info(info->ctx, &अस्थायी);
        /* Do not replace the whole key, only the current field */
        अस्थायी.merge = (अस्थायी.merge == MERGE_REPLACE)
            ? MERGE_OVERRIDE
            : stmt->merge;
        ret = समुच्चयः_Symbols_क्षेत्रम्(info, &अस्थायी, क्षेत्रम्_2, arrayNdx, &stmt->मानम्) &&
              Merge_कुञ्चिकाः(info, &info->default_कुञ्चिका, &अस्थायी, true);
        Clear_कुञ्चिका_Info(&अस्थायी);
    }
    अन्यथा यदि (!elem && (istreq(क्षेत्रम्_2, "name") ||
                       istreq(क्षेत्रम्_2, "groupname"))) {
        ret = समुच्चयः_गणः_Name(info, arrayNdx, stmt->मानम्, stmt->merge);
    }
    अन्यथा यदि (!elem && (istreq(क्षेत्रम्_2, "groupswrap") ||
                       istreq(क्षेत्रम्_2, "wrapgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_क्षेत्रम्,
                "Global \"groupswrap\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा यदि (!elem && (istreq(क्षेत्रम्_2, "groupsclamp") ||
                       istreq(क्षेत्रम्_2, "clampgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_क्षेत्रम्,
                "Global \"groupsclamp\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा यदि (!elem && (istreq(क्षेत्रम्_2, "groupsredirect") ||
                       istreq(क्षेत्रम्_2, "redirectgroups"))) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_क्षेत्रम्,
                "Global \"groupsredirect\" not supported; Ignored\n");
        ret = true;
    }
    अन्यथा यदि (!elem && istreq(क्षेत्रम्_2, "allownone")) {
        log_err(info->ctx, XKB_WARNING_UNSUPPORTED_SYMBOLS_क्षेत्रम्,
                "Radio groups not supported; "
                "Ignoring \"allownone\" specification\n");
        ret = true;
    }
    अन्यथा यदि (elem) {
        ret = (समुच्चयः_Default_Action_क्षेत्रम्(info->keymap_info, &info->default_actions,
                                     &info->mods, elem, क्षेत्रम्_2, arrayNdx,
                                     &stmt->मानम्, stmt->merge) !=
               PARSER_FATAL_ERROR);
    } अन्यथा {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_क्षेत्रम्,
                "Default defined for unknown field \"%s\"; Ignored\n", क्षेत्रम्_2);
        प्रत्यागम !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_SYMBOLS_GLOBAL_FIELDS);
    }

    प्रत्यागम ret;
}

स्थिर bool
HandleSymbolsBody(SymbolsInfo *info, VarDef *def, कुञ्चिका_Info *keyi)
{
    bool all_valid_entries = true;

    प्रति (; def; def = (VarDef *) def->common.अग्र) {
        ध्रुव वर्ण *क्षेत्रम्_2;
        व्यञ्जकम्_Def *arrayNdx;
        bool ok = true;

        यदि (!def->name) {
            यदि (unlikely(!def->मानम्) || def->मानम्->common.type != STMT_व्यञ्जकम्_ACTION_LIST)
                क्षेत्रम्_2 = "symbols"; /* Default to symbols field */
            अन्यथा
                क्षेत्रम्_2 = "actions";
            arrayNdx = रिक्त;
        }
        अन्यथा {
            ध्रुव वर्ण *elem;
            ok = व्यञ्जकम्_Resolve_Lhs(info->ctx, def->name, &elem, &क्षेत्रम्_2,
                                &arrayNdx);
            यदि (ok && elem) {
                log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                        "Cannot set global defaults for \"%s\" element within "
                        "a key statement: move statements to the global file "
                        "scope. Assignment to \"%s.%s\" ignored.\n",
                        elem, elem, क्षेत्रम्_2);
                ok = false;
            }
        }

        यदि (unlikely(!def->मानम्)) {
            log_err(info->ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
                    "Could not allocate the value of field \"%s\". "
                    "Statement ignored.\n", क्षेत्रम्_2);
            ok = false;
        }

        यदि (!ok || !समुच्चयः_Symbols_क्षेत्रम्(info, keyi, क्षेत्रम्_2, arrayNdx, &def->मानम्))
            all_valid_entries = false;
    }

    प्रत्यागम all_valid_entries;
}

स्थिर bool
समुच्चयः_Explicit_गणः(SymbolsInfo *info, कुञ्चिका_Info *keyi)
{
    xkb_layout_सूचकांक_t i;
    गणः_Info_2 *groupi;
    bool warn = false;

    यदि (info->explicit_गणः == XKB_LAYOUT_INVALID)
        प्रत्यागम true;

    darray_enumerate_from(i, groupi, keyi->groups, 1) {
        यदि (groupi->defined) {
            warn = true;
            Clear_गणः_Info(groupi);
            Init_गणः_Info(groupi);
        }
    }

    यदि (warn) {
        log_warn(info->ctx, XKB_WARNING_गुणजः_GROUPS_AT_ONCE,
                 "For the map %s the explicit group %"PRIu32" is specified, "
                 "but key %s has more than one group defined; "
                 "All groups except first one will be ignored\n",
                 info->name, info->explicit_गणः + 1, कुञ्चिका_Info_Text(info, keyi));
    }

    darray_resize0(keyi->groups, info->explicit_गणः + 1);
    यदि (info->explicit_गणः > 0) {
        darray_item(keyi->groups, info->explicit_गणः) =
            darray_item(keyi->groups, 0);
        Init_गणः_Info(&darray_item(keyi->groups, 0));
    }

    प्रत्यागम true;
}

स्थिर bool
HandleSymbolsDef(SymbolsInfo *info, SymbolsDef *stmt)
{
    कुञ्चिका_Info keyi;

    keyi = info->default_कुञ्चिका;
    darray_init(keyi.groups);
    darray_copy(keyi.groups, info->default_कुञ्चिका.groups);
    प्रति (xkb_layout_सूचकांक_t i = 0; i < darray_आकार(keyi.groups); i++)
        Copy_गणः_Info(&darray_item(keyi.groups, i),
                      &darray_item(info->default_कुञ्चिका.groups, i));
    keyi.merge = stmt->merge;
    keyi.name = stmt->कुञ्चिका_Name;

    यदि (HandleSymbolsBody(info, stmt->symbols, &keyi) &&
        समुच्चयः_Explicit_गणः(info, &keyi) &&
        सङ्कलय_कुञ्चिका_Symbols(info, &keyi, true)) {
        प्रत्यागम true;
    }

    Clear_कुञ्चिका_Info(&keyi);
    info->error_गणनम्++;
    प्रत्यागम false;
}

स्थिर bool
HandleModMapDef(SymbolsInfo *info, ModMapDef *def)
{
    Mod_Map_प्रविष्टिः tmp;
    संरचना xkb_context *ctx = info->ctx;

    यदि (!व्यञ्जकम्_Resolve_Mod_आवरणम्(ctx, def->modifiers, MOD_REAL, &info->mods, &tmp.mods)) {
        log_err(ctx, XKB_LOG_सन्देशः_NO_ID,
                "Illegal modifier map definition: invalid modifier mask\n");
        प्रत्यागम false;
    }

    /*
     * NOTE: parser.y accepts only identifiers for v1 keymap format,
     * so no further check is needed (e.g. no operation nor numeric values).
     */
    प्रतिज्ञा(info->keymap_info->keymap.format != XKB_KEYMAP_FORMAT_TEXT_V1 ||
           def->modifiers->common.type == STMT_व्यञ्जकम्_IDENT);

    bool ok = true;
    tmp.merge = def->merge;

    प्रति (व्यञ्जकम्_Def *कुञ्चिका = def->कुञ्चिकाः; कुञ्चिका; कुञ्चिका = (व्यञ्जकम्_Def *) कुञ्चिका->common.अग्र) {
        यदि (कुञ्चिका->common.type == STMT_व्यञ्जकम्_KEYNAME_LITERAL) {
            tmp.have_चिह्नम् = false;
            tmp.u.कुञ्चिका_Name = कुञ्चिका->कुञ्चिका_name.कुञ्चिका_name;
        }
        अन्यथा यदि (कुञ्चिका->common.type == STMT_व्यञ्जकम्_KEYSYM_LITERAL) {
            यदि (कुञ्चिका->keysym.keysym == XKB_कुञ्चिका_No_चिह्नम्) {
                /* Invalid keysym: ignore. Error message already printed */
                अनुवर्त;
            } अन्यथा {
                tmp.have_चिह्नम् = true;
                tmp.u.कुञ्चिका_Sym = कुञ्चिका->keysym.keysym;
            }
        }
        अन्यथा {
            log_err(info->ctx, XKB_ERROR_INVALID_MODMAP_प्रविष्टिः,
                    "Modmap entries may contain only key names or keysyms; "
                    "Illegal definition for %s modifier mask ignored\n",
                    Mod_आवरणम्_Text(info->ctx, MOD_REAL, &info->mods, tmp.mods));
            अनुवर्त;
        }

        ok = सङ्कलय_Mod_Map_प्रविष्टिः(info, &tmp) && ok;
    }
    प्रत्यागम ok;
}

स्थिर शून्य
HandleSymbolsFile(SymbolsInfo *info, XkbFile *file)
{
    bool ok;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    प्रति (ParseCommon *stmt = file->defs; stmt; stmt = stmt->अग्र) {
        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            ok = HandleIncludeSymbols(info, (IncludeStmt *) stmt);
            भङ्ग;
        पक्ष STMT_SYMBOLS:
            ok = HandleSymbolsDef(info, (SymbolsDef *) stmt);
            भङ्ग;
        पक्ष STMT_VAR:
            ok = HandleGlobalVar(info, (VarDef *) stmt);
            भङ्ग;
        पक्ष STMT_VMOD:
            ok = HandleVModDef(info->ctx, &info->mods, (VModDef *) stmt);
            भङ्ग;
        पक्ष STMT_MODMAP:
            ok = HandleModMapDef(info, (ModMapDef *) stmt);
            भङ्ग;
        पक्ष STMT_UNKNOWN_DECLARATION:
        पक्ष STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported symbols %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            भङ्ग;
        स्वभाव:
            log_err(info->ctx, XKB_ERROR_WRONG_STATEMENT_TYPE,
                    "Symbols files may not include other types; "
                    "Ignoring %s\n", stmt_type_गन्तव्यम्_string(stmt->type));
            ok = false;
            भङ्ग;
        }

        यदि (!ok)
            info->error_गणनम्++;

        यदि (info->error_गणनम् > 10) {
            log_err(info->ctx, XKB_ERROR_INVALID_XKB_SYNTAX,
                    "Abandoning symbols file \"%s\"\n",
                    safe_map_name(file->name));
            भङ्ग;
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
स्थिर संरचना xkb_कुञ्चिका *
Find_कुञ्चिका_For_चिह्नम्(संरचना xkb_keymap *keymap, xkb_keysym_t sym)
{
    bool got_one_गणः;
    xkb_layout_सूचकांक_t गणः = 0;
    कुरु {
        xkb_level_सूचकांक_t level = 0;
        got_one_गणः = false;
        bool got_one_level;
        कुरु {
            got_one_level = false;
            संरचना xkb_कुञ्चिका *कुञ्चिका;
            xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
                यदि (गणः < कुञ्चिका->अङ्कः_groups &&
                    level < Xkb_कुञ्चिका_अङ्कः_स्तराः(कुञ्चिका, गणः)) {
                    got_one_गणः = got_one_level = true;
                    ध्रुव xkb_keysym_गणनम्_t अङ्कः_syms =
                        कुञ्चिका->groups[गणः].स्तराः[level].अङ्कः_syms;
                    यदि (अङ्कः_syms > 1) {
                        प्रति (xkb_keysym_गणनम्_t k = 0; k < अङ्कः_syms; k++) {
                            यदि (कुञ्चिका->groups[गणः].स्तराः[level].s.syms[k] == sym)
                                प्रत्यागम कुञ्चिका;
                        }
                    } अन्यथा यदि (अङ्कः_syms &&
                               कुञ्चिका->groups[गणः].स्तराः[level].s.sym == sym) {
                        प्रत्यागम कुञ्चिका;
                    }
                }
            }
            level++;
        } यावत् (got_one_level);
        गणः++;
    } यावत् (got_one_गणः);

    प्रत्यागम रिक्त;
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
स्थिर xkb_atom_t
FindAutomaticType(संरचना xkb_context *ctx, गणः_Info_2 *groupi)
{
    xkb_keysym_t sym0, sym1;
    ध्रुव xkb_level_सूचकांक_t विस्तारः = darray_आकार(groupi->स्तराः);

#परिभाषय GET_SYM(level) \
    (darray_item(groupi->स्तराः, level).अङ्कः_syms == 0 ? \
        XKB_कुञ्चिका_No_चिह्नम् : \
     darray_item(groupi->स्तराः, level).अङ्कः_syms == 1 ? \
        darray_item(groupi->स्तराः, level).s.sym : \
     /* num_syms > 1 */ \
        darray_item(groupi->स्तराः, level).s.syms[0])

    यदि (विस्तारः == 1 || विस्तारः <= 0)
        प्रत्यागम xkb_atom_intern_literal(ctx, "ONE_LEVEL");

    sym0 = GET_SYM(0);
    sym1 = GET_SYM(1);

    यदि (विस्तारः == 2) {
        यदि (xkb_keysym_is_अधस्तनम्(sym0) && xkb_keysym_is_ऊर्ध्वस्थम्_or_title(sym1))
            प्रत्यागम xkb_atom_intern_literal(ctx, "ALPHABETIC");

        यदि (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            प्रत्यागम xkb_atom_intern_literal(ctx, "KEYPAD");

        प्रत्यागम xkb_atom_intern_literal(ctx, "TWO_LEVEL");
    }

    यदि (विस्तारः <= 4) {
        यदि (xkb_keysym_is_अधस्तनम्(sym0) && xkb_keysym_is_ऊर्ध्वस्थम्_or_title(sym1)) {
            xkb_keysym_t sym2, sym3;
            sym2 = GET_SYM(2);
            sym3 = (विस्तारः == 4 ? GET_SYM(3) : XKB_कुञ्चिका_No_चिह्नम्);

            यदि (xkb_keysym_is_अधस्तनम्(sym2) && xkb_keysym_is_ऊर्ध्वस्थम्_or_title(sym3))
                प्रत्यागम xkb_atom_intern_literal(ctx, "FOUR_LEVEL_ALPHABETIC");

            प्रत्यागम xkb_atom_intern_literal(ctx, "FOUR_LEVEL_SEMIALPHABETIC");
        }

        यदि (xkb_keysym_is_keypad(sym0) || xkb_keysym_is_keypad(sym1))
            प्रत्यागम xkb_atom_intern_literal(ctx, "FOUR_LEVEL_KEYPAD");

        प्रत्यागम xkb_atom_intern_literal(ctx, "FOUR_LEVEL");
    }

    प्रत्यागम XKB_ATOM_NONE;

#अपरिभाषय GET_SYM
}

स्थिर ध्रुव संरचना xkb_कुञ्चिका_type *
Find_Type_For_गणः(संरचना xkb_keymap *keymap, कुञ्चिका_Info *keyi,
                 xkb_layout_सूचकांक_t गणः, bool *explicit_type)
{
    गणः_Info_2 *groupi = &darray_item(keyi->groups, गणः);
    xkb_atom_t type_name = groupi->type;

    *explicit_type = true;

    यदि (type_name == XKB_ATOM_NONE) {
        यदि (keyi->default_type != XKB_ATOM_NONE) {
            type_name = keyi->default_type;
        }
        अन्यथा {
            type_name = FindAutomaticType(keymap->ctx, groupi);
            यदि (type_name != XKB_ATOM_NONE)
                *explicit_type = false;
        }
    }

    यदि (type_name == XKB_ATOM_NONE) {
        log_warn(keymap->ctx, XKB_WARNING_CANNOT_INFER_कुञ्चिका_TYPE,
                 "Couldn't find an automatic type for key '%s' "
                 "group %"PRIu32" with %u levels; Using the default type\n",
                 कुञ्चिका_Name_Text(keymap->ctx, keyi->name), गणः + 1,
                 darray_आकार(groupi->स्तराः));
        गच्छ use_default;
    }

    darray_आकार_t i;
    प्रति (i = 0; i < keymap->अङ्कः_types; i++)
        यदि (keymap->types[i].name == type_name)
            भङ्ग;

    यदि (i >= keymap->अङ्कः_types) {
        log_warn(keymap->ctx, XKB_WARNING_UNDEFINED_कुञ्चिका_TYPE,
                 "The type \"%s\" for key '%s' group %"PRIu32" "
                 "was not previously defined; Using the default type\n",
                 xkb_atom_text(keymap->ctx, type_name),
                 कुञ्चिका_Name_Text(keymap->ctx, keyi->name), गणः + 1);
        गच्छ use_default;
    }

    keymap->types[i].required = true;
    प्रत्यागम &keymap->types[i];

use_default:
    /*
     * Index 0 is guaranteed to contain something, usually
     * ONE_LEVEL or at least some default one-level type.
     */
    keymap->types[0].required = true;
    प्रत्यागम &keymap->types[0];
}

स्थिर bool
Copy_Symbols_Def_गन्तव्यम्_Keymap(संरचना xkb_keymap *keymap, SymbolsInfo *info,
                       कुञ्चिका_Info *keyi)
{
    संरचना xkb_कुञ्चिका *कुञ्चिका;
    गणः_Info_2 *groupi;
    ध्रुव गणः_Info_2 *group0;
    xkb_layout_सूचकांक_t i;

    /*
     * The name is guaranteed to be real and not an alias (see
     * AddKeySymbols), so 'false' is safe here.
     */
    कुञ्चिका = Xkb_कुञ्चिका_By_Name(keymap, keyi->name, false);
    यदि (!कुञ्चिका) {
        log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                XKB_WARNING_UNDEFINED_KEYCODE,
                "Key %s not found in keycodes; Symbols ignored\n",
                कुञ्चिका_Info_Text(info, keyi));
        प्रत्यागम false;
    }

    /* Find the range of groups we need. */
    कुञ्चिका->अङ्कः_groups = 0;
    darray_enumerate(i, groupi, keyi->groups) {
        /* Skip groups that have no levels and no explicit type.
         * Such group would be filled with `NoSymbol` anyway. */
        ध्रुव bool has_explicit_type = (keyi->defined & कुञ्चिका_क्षेत्रम्_DEFAULT_TYPE)
                                    || (groupi->defined & गणः_क्षेत्रम्_TYPE);
        यदि (darray_आकार(groupi->स्तराः) > 0 || has_explicit_type)
            कुञ्चिका->अङ्कः_groups = i + 1;
        यदि (has_explicit_type)
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
    }

    यदि (कुञ्चिका->अङ्कः_groups <= 0) {
        /* A key with no group may still have other fields defined */
        यदि (keyi->defined)
            गच्छ कुञ्चिका_fields;
        अन्यथा
            प्रत्यागम false;
    }

    darray_resize(keyi->groups, कुञ्चिका->अङ्कः_groups);

    /*
     * If there are empty groups between non-empty ones, fill them with data
     * from the first group.
     * We can make a wrong assumption here. But leaving gaps is worse.
     */
    group0 = &darray_item(keyi->groups, 0);
    darray_foreach_from(groupi, keyi->groups, 1) {
        यदि (groupi->defined)
            अनुवर्त;

        Copy_गणः_Info(groupi, group0);
    }

    कुञ्चिका->groups = शून्यस्मृतिं_देहि(कुञ्चिका->अङ्कः_groups, परिमाण(*कुञ्चिका->groups));
    प्रतिज्ञा(darray_आकार(keyi->groups) == कुञ्चिका->अङ्कः_groups);

    /* Find and assign the groups' types in the keymap. */
    darray_enumerate(i, groupi, keyi->groups) {
        #यदि_परिभाषितम् __clang__
        /* Make clang-tidy happy */
        __builtin_assume(i < कुञ्चिका->अङ्कः_groups);
        #यदि_अन्तः
        bool explicit_type = false;
        ध्रुव संरचना xkb_कुञ्चिका_type * ध्रुव type =
            Find_Type_For_गणः(keymap, keyi, i, &explicit_type);

        /* Always have as many levels as the type specifies. */
        यदि (type->अङ्कः_स्तराः < darray_आकार(groupi->स्तराः)) {
            संरचना xkb_level *leveli;

            log_vrb(info->ctx, XKB_LOG_VERBOSITY_BRIEF,
                    XKB_WARNING_EXTRA_SYMBOLS_IGNORED,
                    "Type \"%s\" has %"PRIu32" levels, but %s has %u levels; "
                    "Ignoring extra symbols\n",
                    xkb_atom_text(keymap->ctx, type->name), type->अङ्कः_स्तराः,
                    कुञ्चिका_Info_Text(info, keyi),
                    darray_आकार(groupi->स्तराः));

            darray_foreach_from(leveli, groupi->स्तराः, type->अङ्कः_स्तराः)
                clear_level(leveli);
        }
        darray_resize0(groupi->स्तराः, type->अङ्कः_स्तराः);

        कुञ्चिका->groups[i].explicit_type = explicit_type;
        कुञ्चिका->groups[i].type = type;
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
        संरचना xkb_level *leveli;
        darray_foreach(leveli, groupi->स्तराः) {
            विकल्प (leveli->अङ्कः_syms) {
            पक्ष 0:
                leveli->ऊर्ध्वस्थम् = XKB_कुञ्चिका_No_चिह्नम्;
                भङ्ग;
            पक्ष 1:
                leveli->ऊर्ध्वस्थम् = xkb_keysym_गन्तव्यम्_ऊर्ध्वस्थम्(leveli->s.sym);
                भङ्ग;
            स्वभाव:
                /* Multiple keysyms: check if there is any cased keysym */
                leveli->has_ऊर्ध्वस्थम् = false;
                प्रति (xkb_keysym_गणनम्_t k = 0; k < leveli->अङ्कः_syms; k++) {
                    ध्रुव xkb_keysym_t ऊर्ध्वस्थम् =
                        xkb_keysym_गन्तव्यम्_ऊर्ध्वस्थम्(leveli->s.syms[k]);
                    यदि (ऊर्ध्वस्थम् != leveli->s.syms[k]) {
                        leveli->has_ऊर्ध्वस्थम् = true;
                        भङ्ग;
                    }
                }
                यदि (leveli->has_ऊर्ध्वस्थम्) {
                    /*
                     * Some cased keysyms: store the transformation result in
                     * the same array, right after the original keysyms.
                     */
                    leveli->s.syms = realloc(leveli->s.syms,
                                             (आकार_t) 2 * leveli->अङ्कः_syms *
                                             परिमाण(*leveli->s.syms));
                    यदि (!leveli->s.syms)
                        प्रत्यागम false; /* FIXME: better handling? */
                    प्रति (xkb_keysym_गणनम्_t k = 0; k < leveli->अङ्कः_syms; k++) {
                        leveli->s.syms[leveli->अङ्कः_syms + k] =
                            xkb_keysym_गन्तव्यम्_ऊर्ध्वस्थम्(leveli->s.syms[k]);
                    }
                }
            }
        }

        /* Copy the level */
        darray_steal(groupi->स्तराः, &कुञ्चिका->groups[i].स्तराः, रिक्त);
        यदि (कुञ्चिका->groups[i].type->अङ्कः_स्तराः > 1 ||
            कुञ्चिका->groups[i].स्तराः[0].अङ्कः_syms > 0) {
            कुञ्चिका->groups[i].explicit_symbols = true;
            कुञ्चिका->explicit |= EXPLICIT_SYMBOLS;
        }
        यदि (groupi->defined & गणः_क्षेत्रम्_ACTS) { // FIXME
            कुञ्चिका->groups[i].explicit_actions = true;
            कुञ्चिका->explicit |= EXPLICIT_INTERP;
        }
        यदि (कुञ्चिका->groups[i].explicit_type)
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
    }

    कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_pending_गणः = keyi->बहिर्गमनम्_सम्बन्धि_range_pending_गणः;
    कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या = keyi->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या;
    कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_policy = keyi->बहिर्गमनम्_सम्बन्धि_range_गणः_policy;

कुञ्चिका_fields:
    यदि (keyi->defined & कुञ्चिका_क्षेत्रम्_VMODMAP) {
        कुञ्चिका->vmodmap = keyi->vmodmap;
        कुञ्चिका->explicit |= EXPLICIT_VMODMAP;
    }

    यदि (keyi->repeat != कुञ्चिका_REPEAT_UNDEFINED) {
        कुञ्चिका->repeats = (keyi->repeat == कुञ्चिका_REPEAT_YES);
        कुञ्चिका->explicit |= EXPLICIT_REPEAT;
    }

    यदि ((keyi->defined & कुञ्चिका_क्षेत्रम्_OVERLAY) &&
        keyi->overlays && !keyi->overlays_clear) {
        कुञ्चिका->overlays = keyi->overlays;
        यदि (keyi->overlays_alloc) {
            /* Remove empty entries */

            xkb_overlay_आवरणम्_t remaining = कुञ्चिका->overlays;
            ध्रुव संरचना xkb_कुञ्चिका **overlays_कुञ्चिकाः = keyi->overlays_कुञ्चिकाः;
            यावत् (remaining) {
                /* isolate lowest set bit */
                ध्रुव xkb_overlay_आवरणम्_t lsb = (xkb_overlay_आवरणम्_t)(
                    remaining &
                    (xkb_overlay_आवरणम्_t)((अचिह्नित)~remaining + 1u)
                );
                remaining = (remaining & ~lsb);
                यदि (*overlays_कुञ्चिकाः) {
                    overlays_कुञ्चिकाः++;
                } अन्यथा {
                    /* drop current null value (overlayN=none) */
                    कुञ्चिका->overlays &= ~lsb;
                }
            }

            यदि (कुञ्चिका->overlays) {
                /* Steal */
                कुञ्चिका->overlays_कुञ्चिकाः = keyi->overlays_कुञ्चिकाः;
                keyi->overlays_कुञ्चिकाः = रिक्त;
                keyi->overlays_alloc = 0;

                कुञ्चिका->overlays_inline = false;
                कुञ्चिका->explicit |= EXPLICIT_OVERLAY;
            }
        } अन्यथा {
            कुञ्चिका->overlay_कुञ्चिका = keyi->overlay_कुञ्चिका;
            कुञ्चिका->overlays_inline = true;
            कुञ्चिका->explicit |= EXPLICIT_OVERLAY;
        }
    }

    प्रत्यागम true;
}

स्थिर bool
Copy_Mod_Map_Def_गन्तव्यम्_Keymap(संरचना xkb_keymap *keymap, SymbolsInfo *info,
                      Mod_Map_प्रविष्टिः *प्रविष्टिः)
{
    संरचना xkb_कुञ्चिका *कुञ्चिका;

    यदि (!प्रविष्टिः->have_चिह्नम्) {
        कुञ्चिका = Xkb_कुञ्चिका_By_Name(keymap, प्रविष्टिः->u.कुञ्चिका_Name, true);
        यदि (!कुञ्चिका) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNDEFINED_KEYCODE,
                    "Key %s not found in keycodes; "
                    "Modifier map entry for %s not updated\n",
                    कुञ्चिका_Name_Text(info->ctx, प्रविष्टिः->u.कुञ्चिका_Name),
                    Mod_आवरणम्_Text(info->ctx, MOD_REAL, &info->mods, प्रविष्टिः->mods));
            प्रविष्टिः->mods = 0; /* disable */
            प्रत्यागम false;
        }
    }
    अन्यथा {
        कुञ्चिका = Find_कुञ्चिका_For_चिह्नम्(keymap, प्रविष्टिः->u.कुञ्चिका_Sym);
        यदि (!कुञ्चिका) {
            log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                    XKB_WARNING_UNRESOLVED_KEYMAP_चिह्नम्,
                    "Key \"%s\" not found in symbol map; "
                    "Modifier map entry for %s not updated\n",
                    KeysymText(info->ctx, प्रविष्टिः->u.कुञ्चिका_Sym),
                    Mod_आवरणम्_Text(info->ctx, MOD_REAL, &info->mods, प्रविष्टिः->mods));
            प्रविष्टिः->mods = 0; /* disable */
            प्रत्यागम false;
        }
    }

    /* Skip modMap None */
    यदि (प्रविष्टिः->mods) {
        कुञ्चिका->modmap |= प्रविष्टिः->mods;
        प्रविष्टिः->कुञ्चिका_संहिता = कुञ्चिका->keycode;
    }

    प्रत्यागम true;
}

स्थिर bool
Copy_Symbols_गन्तव्यम्_Keymap(संरचना xkb_keymap *keymap, SymbolsInfo *info)
{
    कुञ्चिका_Info *keyi;

    keymap->symbols_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->symbols_section_name);

    keymap->mods = info->mods;

    keymap->अङ्कः_गणः_नामानि = (xkb_layout_सूचकांक_t)darray_आकार(info->गणः_नामानि);
    darray_steal(info->गणः_नामानि, &keymap->गणः_नामानि, रिक्त);

    darray_foreach(keyi, info->कुञ्चिकाः)
        यदि (!Copy_Symbols_Def_गन्तव्यम्_Keymap(keymap, info, keyi))
            info->error_गणनम्++;

    यदि (xkb_context_get_log_verbosity(keymap->ctx) > 3) {
        संरचना xkb_कुञ्चिका *कुञ्चिका;

        xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
            यदि (कुञ्चिका->name == XKB_ATOM_NONE)
                अनुवर्त;

            यदि (कुञ्चिका->अङ्कः_groups < 1)
                log_info(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                         "No symbols defined for %s\n",
                         कुञ्चिका_Name_Text(info->ctx, कुञ्चिका->name));
        }
    }

    /* Resolve modmaps */
    Mod_Map_प्रविष्टिः *mm;
    darray_foreach(mm, info->modmaps)
        यदि (!Copy_Mod_Map_Def_गन्तव्यम्_Keymap(keymap, info, mm))
            info->error_गणनम्++;

    /* Copy detailed modmap */
    darray_steal(info->modmaps, &keymap->modmaps, &keymap->अङ्कः_modmaps);

    /* XXX: If we don't ignore errorCount, things break. */
    प्रत्यागम true;
}

bool
CompileSymbols(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    SymbolsInfo info;

    InitSymbolsInfo(&info, keymap_info, 0, &keymap_info->keymap.mods);

    यदि (file !=रिक्त)
        HandleSymbolsFile(&info, file);

    यदि (info.error_गणनम् != 0)
        गच्छ err_info;

    यदि (!Copy_Symbols_गन्तव्यम्_Keymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    ClearSymbolsInfo(&info);
    प्रत्यागम true;

err_info:
    ClearSymbolsInfo(&info);
    प्रत्यागम false;
}
