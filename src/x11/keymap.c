/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <xcb/xkb.h>

#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcommon/xkbcommon-keysyms.h"
#অন্তর্ভাব "atom.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "x11-priv.h"

/*
 * References for the lonesome traveler:
 * Xkb protocol specification:
 *      https://www.x.org/releases/current/doc/kbproto/xkbproto.html
 * The XCB xkb XML protocol file:
 *      /user/share/xcb/xkb.xml
 * The XCB xkb header file:
 *      /usr/include/xcb/xkb.h
 * The old kbproto header files:
 *      /usr/include/X11/extensions/XKB{,proto,str}.h
 * Xlib XKB source code:
 *      <libX11>/src/xkb/XKBGetMap.c (and friends)
 * X server XKB protocol handling:
 *      <xserver>/xkb/xkb.c
 * Man pages:
 *      XkbGetMap(3), XkbGetCompatMap(3), etc.
 */

/* Constants from /usr/include/X11/extensions/XKB.h */
/* XkbNumModifiers. */
#define অঙ্ক_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define অঙ্ক_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define অঙ্ক_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(রাশিমালা) কুরু {                              \
    যদি (!(রাশিমালা)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(রাশিমালা));                 \
        গচ্ছ fail;                                          \
    }                                                       \
} যাবত্ (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) কুরু {         \
    যদি (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        গচ্ছ fail;                                          \
    }                                                       \
} যাবত্ (0)

#define ALLOC_OR_FAIL(arr, nmemb) কুরু {                      \
    যদি ((nmemb) > 0) {                                      \
        (arr) = calloc((nmemb), পরিমাণ(*(arr)));            \
        যদি (!(arr))                                         \
            গচ্ছ fail;                                      \
    }                                                       \
} যাবত্ (0)

স্থির ধ্রুব xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_চাবি_TYPES |
     XCB_XKB_MAP_PART_চাবি_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_চাবি_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

স্থির ধ্রুব xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_চাবি_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_চাবি_NAMES |
     XCB_XKB_NAME_DETAIL_চাবি_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
স্থির ধ্রুব xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_চাবি_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_চাবি_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


স্থির xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_নিচের, uint16_t vmods_উপরের)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    প্রত্যাগম
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_নিচের << 8) |
        ((xkb_mod_mask_t) vmods_উপরের << 16);
}

গণনা xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    গণনা xkb_action_controls ret = 0;
    যদি (wire & XCB_XKB_BOOL_CTRL_REPEAT_চাবিগুলি)
        ret |= CONTROL_REPEAT;
    যদি (wire & XCB_XKB_BOOL_CTRL_SLOW_চাবিগুলি)
        ret |= CONTROL_SLOW;
    যদি (wire & XCB_XKB_BOOL_CTRL_BOUNCE_চাবিগুলি)
        ret |= CONTROL_DEBOUNCE;
    যদি (wire & XCB_XKB_BOOL_CTRL_STICKY_চাবিগুলি)
        ret |= CONTROL_STICKY_চাবিগুলি;
    যদি (wire & XCB_XKB_BOOL_CTRL_MOUSE_চাবিগুলি)
        ret |= CONTROL_MOUSE_চাবিগুলি;
    যদি (wire & XCB_XKB_BOOL_CTRL_MOUSE_চাবিগুলি_ACCEL)
        ret |= CONTROL_MOUSE_চাবিগুলি_ACCEL;
    যদি (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_চাবিগুলি)
        ret |= CONTROL_AX;
    যদি (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    যদি (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    যদি (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    যদি (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    যদি (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    যদি (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    প্রত্যাগম ret;
}

স্থির bool
translate_action(সঙ্ঘ xkb_action *action, ধ্রুব xcb_xkb_action_t *wire,
                 xkb_keycode_t min_চাবি_code, xkb_keycode_t max_চাবি_code)
{
    বিকল্প (wire->type) {
    পক্ষ XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_নিচের_2,
                                                wire->setmods.vmods_উপরের_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        যদি (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        যদি (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        যদি (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_নিচের_2,
                                                wire->latchmods.vmods_উপরের_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        যদি (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        যদি (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        যদি (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_নিচের_2,
                                                wire->lockmods.vmods_উপরের_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        যদি (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        যদি (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        যদি (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        যদি (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        যদি (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        যদি (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        যদি (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        যদি (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        যদি (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        যদি (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_নিচের | ((uint16_t) wire->moveptr.x_উপরের << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_নিচের | ((uint16_t) wire->moveptr.y_উপরের << 8));

        যদি (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        যদি (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        যদি (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.গুনতি = wire->ptrbtn.গুনতি;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.গুনতি = 0;

        যদি (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        যদি (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.মান = wire->setptrdflt.মান;

        যদি (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        যদি (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        যদি (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            ধ্রুব uint16_t mask = (wire->setcontrols.bool_Ctrls_নিচের |
                                   (wire->setcontrols.bool_Ctrls_উপরের << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        ভঙ্গ;
    পক্ষ XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            ধ্রুব uint16_t mask = (wire->lockcontrols.bool_Ctrls_নিচের |
                                   (wire->lockcontrols.bool_Ctrls_উপরের << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        ভঙ্গ;

    পক্ষ XCB_XKB_SA_TYPE_REDIRECT_চাবি:
        action->type = ACTION_TYPE_REDIRECT_চাবি;
        যদি (wire->redirect.newkey < min_চাবি_code ||
            wire->redirect.newkey > max_চাবি_code)
                প্রত্যাগম false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_নিচের,
            wire->redirect.vmods_Mask_উপরের
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_নিচের_2,
                                               wire->redirect.vmods_উপরের_2);
        ভঙ্গ;

    পক্ষ XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        ভঙ্গ;

    /* We don't support these. */
    পক্ষ XCB_XKB_SA_TYPE_ISO_LOCK:
    পক্ষ XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    পক্ষ XCB_XKB_SA_TYPE_DEVICE_BTN:
    পক্ষ XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    পক্ষ XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        ভঙ্গ;

    স্বভাব:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_অঙ্ক_ENTRIES,
                      "Missing action type");

        যদি (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            ভঙ্গ;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(পরিমাণ(action->priv.data) == 7 &&
                      পরিমাণ(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        memcpy(action->priv.data, wire->noaction.pad0, 7);
        ভঙ্গ;
    }
    প্রত্যাগম true;
}

স্থির bool
get_types(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    পূর্ণাঙ্ক types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_চাবি_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->অঙ্ক_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->অঙ্ক_types);
    প্রতিজ্ঞা(types_length == (পূর্ণাঙ্ক) keymap->অঙ্ক_types);

    প্রতি (পূর্ণাঙ্ক i = 0; i < types_length; i++) {
        xcb_xkb_চাবি_type_t *wire_type = types_iter.data;
        সংরচনা xkb_চাবি_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->অঙ্ক_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->অঙ্ক_levels = wire_type->অঙ্ক_Levels;

        {
            পূর্ণাঙ্ক entries_length = xcb_xkb_চাবি_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_চাবি_type_map_iterator(wire_type);

            type->অঙ্ক_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->অঙ্ক_entries);
            প্রতিজ্ঞা(entries_length == (পূর্ণাঙ্ক) type->অঙ্ক_entries);

            প্রতি (পূর্ণাঙ্ক j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                সংরচনা xkb_চাবি_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->অঙ্ক_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            পূর্ণাঙ্ক preserves_length = xcb_xkb_চাবি_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_চাবি_type_preserve_iterator(wire_type);

            FAIL_UNLESS((unsigned) preserves_length <= type->অঙ্ক_entries);

            প্রতি (পূর্ণাঙ্ক j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                সংরচনা xkb_চাবি_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_চাবি_type_next(&types_iter);
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_sym_maps(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    পূর্ণাঙ্ক sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_চাবি_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_চাবি_Code <= reply->max_চাবি_Code);
    FAIL_UNLESS(reply->first_চাবি_Sym >= reply->min_চাবি_Code);
    FAIL_UNLESS(reply->first_চাবি_Sym + reply->n_চাবি_Syms <= reply->max_চাবি_Code + 1);

    keymap->min_চাবি_code = reply->min_চাবি_Code;
    keymap->max_চাবি_code = reply->max_চাবি_Code;
    keymap->অঙ্ক_চাবিগুলি = keymap->অঙ্ক_চাবিগুলি_নিচের = reply->max_চাবি_Code + 1;

    ALLOC_OR_FAIL(keymap->চাবিগুলি, keymap->max_চাবি_code + 1);

    প্রতি (xkb_keycode_t kc = keymap->min_চাবি_code; kc <= keymap->max_চাবি_code; kc++)
        keymap->চাবিগুলি[kc].keycode = kc;

    প্রতি (পূর্ণাঙ্ক i = 0; i < sym_maps_length; i++) {
        xcb_xkb_চাবি_sym_map_t *wire_sym_map = sym_maps_iter.data;
        সংরচনা xkb_চাবি *চাবি = &keymap->চাবিগুলি[reply->first_চাবি_Sym + i];

        চাবি->অঙ্ক_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(চাবি->অঙ্ক_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(চাবি->groups, চাবি->অঙ্ক_groups);

        প্রতি (xkb_layout_index_t j = 0; j < চাবি->অঙ্ক_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->অঙ্ক_types);
            চাবি->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(চাবি->groups[j].levels, চাবি->groups[j].type->অঙ্ক_levels);
        }

        চাবি->out_of_range_group_সংখ্যা = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(চাবি->out_of_range_group_সংখ্যা <= চাবি->অঙ্ক_groups);

        যদি (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            চাবি->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        অন্যথা যদি (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            চাবি->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        অন্যথা
            চাবি->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            পূর্ণাঙ্ক syms_length = xcb_xkb_চাবি_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_চাবি_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * চাবি->অঙ্ক_groups);

            যদি (syms_length > 0)
                চাবি->explicit |= EXPLICIT_SYMBOLS;

            প্রতি (xkb_layout_index_t group = 0; group < চাবি->অঙ্ক_groups; group++) {
                প্রতি (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    প্রতিজ্ঞা(চাবি->groups[group].type != রিক্ত);
                    যদি (level < চাবি->groups[group].type->অঙ্ক_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        চাবি->groups[group].levels[level].অঙ্ক_syms = 1;
                        চাবি->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        চাবি->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        যদি (wire_keysym != XKB_চাবি_No_প্রতীক)
                            চাবি->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_চাবি_sym_map_next(&sym_maps_iter);
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_actions(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    পূর্ণাঙ্ক acts_গুনতি_length =
        xcb_xkb_get_map_map_acts_rtrn_গুনতি_length(reply, map);
    uint8_t *acts_গুনতি_iter = xcb_xkb_get_map_map_acts_rtrn_গুনতি(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_চাবি_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_চাবি_Action == keymap->min_চাবি_code);
    FAIL_UNLESS(reply->first_চাবি_Action + reply->n_চাবি_Actions ==
                keymap->max_চাবি_code + 1);

    প্রতি (পূর্ণাঙ্ক i = 0; i < acts_গুনতি_length; i++) {
        xcb_xkb_চাবি_sym_map_t *wire_sym_map = sym_maps_iter.data;
        পূর্ণাঙ্ক syms_length = xcb_xkb_চাবি_sym_map_syms_length(wire_sym_map);
        uint8_t wire_গুনতি = *acts_গুনতি_iter;
        সংরচনা xkb_চাবি *চাবি = &keymap->চাবিগুলি[reply->first_চাবি_Action + i];

        FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * চাবি->অঙ্ক_groups);
        FAIL_UNLESS(wire_গুনতি == 0 || wire_গুনতি == syms_length);

        ধ্রুব xkb_keycode_t min_চাবি_code = keymap->min_চাবি_code;
        ধ্রুব xkb_keycode_t max_চাবি_code = keymap->max_চাবি_code;

        যদি (wire_গুনতি != 0) {
            প্রতি (xkb_layout_index_t group = 0; group < চাবি->অঙ্ক_groups; group++) {
                প্রতি (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    যদি (level < চাবি->groups[group].type->অঙ্ক_levels) {
                        চাবি->groups[group].levels[level].অঙ্ক_actions = 1;
                        সঙ্ঘ xkb_action *action =
                            &চাবি->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_চাবি_code, max_চাবি_code));

                        যদি (action->type != ACTION_TYPE_NONE) {
                            চাবি->groups[group].implicit_actions =
                                !(চাবি->explicit & EXPLICIT_INTERP);
                        } অন্যথা যদি (চাবি->groups[group].levels[level].s.sym ==
                                   XKB_চাবি_No_প্রতীক) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            চাবি->groups[group].levels[level].অঙ্ক_syms = 0;
                            চাবি->groups[group].levels[level].অঙ্ক_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                যদি (চাবি->groups[group].implicit_actions)
                    চাবি->implicit_actions = true;
            }
        }

        acts_গুনতি_iter++;
        xcb_xkb_চাবি_sym_map_next(&sym_maps_iter);
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_vmods(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.অঙ্ক_mods =
        অঙ্ক_REAL_MODS + MIN(msb_pos(reply->virtualMods), অঙ্ক_VMODS);
    FAIL_UNLESS(keymap->mods.অঙ্ক_mods <= XKB_MAX_MODS);

    static_assert(অঙ্ক_REAL_MODS + অঙ্ক_VMODS <= XKB_MAX_MODS, "");
    প্রতি (unsigned i = 0; i < অঙ্ক_VMODS; i++) {
        যদি (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            সংরচনা xkb_mod *mod = &keymap->mods.mods[অঙ্ক_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_explicits(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    পূর্ণাঙ্ক length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    প্রতি (পূর্ণাঙ্ক i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        সংরচনা xkb_চাবি *চাবি;

        FAIL_UNLESS(wire->keycode >= keymap->min_চাবি_code &&
                    wire->keycode <= keymap->max_চাবি_code);

        চাবি = &keymap->চাবিগুলি[wire->keycode];

        যদি ((wire->explicit & XCB_XKB_EXPLICIT_চাবি_TYPE_1) &&
            চাবি->অঙ্ক_groups > 0) {
            চাবি->groups[0].explicit_type = true;
            চাবি->explicit |= EXPLICIT_TYPES;
        }
        যদি ((wire->explicit & XCB_XKB_EXPLICIT_চাবি_TYPE_2) &&
            চাবি->অঙ্ক_groups > 1) {
            চাবি->groups[1].explicit_type = true;
            চাবি->explicit |= EXPLICIT_TYPES;
        }
        যদি ((wire->explicit & XCB_XKB_EXPLICIT_চাবি_TYPE_3) &&
            চাবি->অঙ্ক_groups > 2) {
            চাবি->groups[2].explicit_type = true;
            চাবি->explicit |= EXPLICIT_TYPES;
        }
        যদি ((wire->explicit & XCB_XKB_EXPLICIT_চাবি_TYPE_4) &&
            চাবি->অঙ্ক_groups > 3) {
            চাবি->groups[3].explicit_type = true;
            চাবি->explicit |= EXPLICIT_TYPES;
        }
        যদি (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            চাবি->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            প্রতি (xkb_layout_index_t l = 0; l < চাবি->অঙ্ক_groups; l++) {
                চাবি->groups[l].explicit_actions = true;
            }
        }
        যদি (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            চাবি->explicit |= EXPLICIT_REPEAT;
        যদি (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            চাবি->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_modmaps(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    পূর্ণাঙ্ক length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_চাবি_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    প্রতি (পূর্ণাঙ্ক i = 0; i < length; i++) {
        xcb_xkb_চাবি_mod_map_t *wire = iter.data;
        সংরচনা xkb_চাবি *চাবি;

        FAIL_UNLESS(wire->keycode >= keymap->min_চাবি_code &&
                    wire->keycode <= keymap->max_চাবি_code);

        চাবি = &keymap->চাবিগুলি[wire->keycode];
        চাবি->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .চাবি_Code = চাবি->keycode,
            .mods = চাবি->modmap,
            .have_প্রতীক = false,
            .u.চাবি_Name = চাবি->name
        });

        xcb_xkb_চাবি_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->অঙ্ক_modmaps);
    প্রত্যাগম true;

fail:
    darray_free(modmaps);
    প্রত্যাগম false;
}

স্থির bool
get_vmodmaps(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    পূর্ণাঙ্ক length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_চাবি_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    প্রতি (পূর্ণাঙ্ক i = 0; i < length; i++) {
        xcb_xkb_চাবি_v_mod_map_t *wire = iter.data;
        সংরচনা xkb_চাবি *চাবি;

        FAIL_UNLESS(wire->keycode >= keymap->min_চাবি_code &&
                    wire->keycode <= keymap->max_চাবি_code);

        চাবি = &keymap->চাবিগুলি[wire->keycode];
        চাবি->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_চাবি_v_mod_map_next(&iter);
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_map(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, রিক্ত);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    যদি ((reply->present & get_map_required_components) != get_map_required_components)
        গচ্ছ fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_চাবি_Syms,
                               reply->n_চাবি_Actions,
                               reply->totalActions,
                               reply->total_চাবি_Behaviors,
                               reply->virtualMods,
                               reply->total_চাবি_Explicit,
                               reply->total_Mod_Map_চাবিগুলি,
                               reply->total_VMod_Map_চাবিগুলি,
                               reply->present,
                               &map);

    যদি (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        গচ্ছ fail;

    মুক্ত(reply);
    প্রত্যাগম true;

fail:
    মুক্ত(reply);
    প্রত্যাগম false;
}

স্থির bool
get_indicators(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->অঙ্ক_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->অঙ্ক_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == অঙ্ক_INDICATORS, "");
    প্রতি (unsigned i = 0; i < অঙ্ক_INDICATORS; i++) {
        যদি (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            সংরচনা xkb_led *led = &keymap->leds[i];

            যদি (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            যদি (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            যদি (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            যদি (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            যদি (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            যদি (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            যদি (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            যদি (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            যদি (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            যদি (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_indicator_map(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, রিক্ত);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    যদি (!get_indicators(keymap, conn, reply))
        গচ্ছ fail;

    মুক্ত(reply);
    প্রত্যাগম true;

fail:
    মুক্ত(reply);
    প্রত্যাগম false;
}

স্থির bool
get_sym_interprets(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    পূর্ণাঙ্ক length = xcb_xkb_get_compat_map_si_rtrn_length(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->nTotalSI);

    keymap->অঙ্ক_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->অঙ্ক_sym_interprets);

    ধ্রুব xkb_keycode_t min_চাবি_code = keymap->min_চাবি_code;
    ধ্রুব xkb_keycode_t max_চাবি_code = keymap->max_চাবি_code;

    প্রতি (পূর্ণাঙ্ক i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        সংরচনা xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        বিকল্প (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        পক্ষ XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            ভঙ্গ;
        পক্ষ XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            ভঙ্গ;
        পক্ষ XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            ভঙ্গ;
        পক্ষ XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            ভঙ্গ;
        পক্ষ XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            ভঙ্গ;
        স্বভাব:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            গচ্ছ fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        যদি (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        অন্যথা
            sym_interpret->virtual_mod = অঙ্ক_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_চাবি_code, max_চাবি_code));
        sym_interpret->অঙ্ক_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_compat_map(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, রিক্ত);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    যদি (!get_sym_interprets(keymap, conn, reply))
        গচ্ছ fail;

    মুক্ত(reply);
    প্রত্যাগম true;

fail:
    মুক্ত(reply);
    প্রত্যাগম false;
}

স্থির bool
get_type_names(সংরচনা xkb_keymap *keymap, সংরচনা x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_মান_list_t *list)
{
    পূর্ণাঙ্ক চাবি_type_names_length =
        xcb_xkb_get_names_মান_list_type_names_length(reply, list);
    xcb_atom_t *চাবি_type_names_iter =
        xcb_xkb_get_names_মান_list_type_names(list);
    পূর্ণাঙ্ক n_levels_per_type_length =
        xcb_xkb_get_names_মান_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_মান_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_মান_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->অঙ্ক_types);
    FAIL_UNLESS(চাবি_type_names_length == n_levels_per_type_length);

    প্রতি (পূর্ণাঙ্ক i = 0; i < চাবি_type_names_length; i++) {
        xcb_atom_t wire_type_name = *চাবি_type_names_iter;
        uint8_t wire_অঙ্ক_levels = *n_levels_per_type_iter;
        সংরচনা xkb_চাবি_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->অঙ্ক_levels >= wire_অঙ্ক_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->অঙ্ক_levels);
        type->অঙ্ক_level_names = type->অঙ্ক_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        প্রতি (size_t j = 0; j < wire_অঙ্ক_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        প্রতি (size_t j = wire_অঙ্ক_levels; j < type->অঙ্ক_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_অঙ্ক_levels;
        চাবি_type_names_iter++;
        n_levels_per_type_iter++;
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_indicator_names(সংরচনা xkb_keymap *keymap,
                    সংরচনা x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_মান_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_মান_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->অঙ্ক_leds);

    প্রতি (unsigned i = 0; i < অঙ্ক_INDICATORS; i++) {
        যদি (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            সংরচনা xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_vmod_names(সংরচনা xkb_keymap *keymap, সংরচনা x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_মান_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_মান_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.অঙ্ক_mods =
        অঙ্ক_REAL_MODS + MIN(msb_pos(reply->virtualMods), অঙ্ক_VMODS);
    FAIL_UNLESS(keymap->mods.অঙ্ক_mods <= XKB_MAX_MODS);

    static_assert(অঙ্ক_REAL_MODS + অঙ্ক_VMODS <= XKB_MAX_MODS, "");
    প্রতি (unsigned i = 0; i < অঙ্ক_VMODS; i++) {
        যদি (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            সংরচনা xkb_mod *mod = &keymap->mods.mods[অঙ্ক_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_group_names(সংরচনা xkb_keymap *keymap, সংরচনা x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_মান_list_t *list)
{
    পূর্ণাঙ্ক length = xcb_xkb_get_names_মান_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_মান_list_groups(list);

    keymap->অঙ্ক_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->অঙ্ক_group_names);

    প্রতি (পূর্ণাঙ্ক i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_চাবি_names(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_মান_list_t *list)
{
    পূর্ণাঙ্ক length = xcb_xkb_get_names_মান_list_চাবি_names_length(reply, list);
    xcb_xkb_চাবি_name_iterator_t iter =
        xcb_xkb_get_names_মান_list_চাবি_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_চাবি_Code == keymap->min_চাবি_code);
    FAIL_UNLESS(reply->max_চাবি_Code == keymap->max_চাবি_code);
    FAIL_UNLESS(reply->first_চাবি == keymap->min_চাবি_code);
    FAIL_UNLESS(reply->first_চাবি + reply->n_চাবিগুলি - 1U == keymap->max_চাবি_code);

    প্রতি (পূর্ণাঙ্ক i = 0; i < length; i++) {
        xcb_xkb_চাবি_name_t *wire = iter.data;
        xkb_atom_t *চাবি_name = &keymap->চাবিগুলি[reply->first_চাবি + i].name;

        যদি (wire->name[0] == '\0') {
            *চাবি_name = XKB_ATOM_NONE;
        }
        অন্যথা {
            *চাবি_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_চাবি_NAME_LENGTH));
            যদি (!*চাবি_name)
                প্রত্যাগম false;
        }

        xcb_xkb_চাবি_name_next(&iter);
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_aliases(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_মান_list_t *list)
{
    পূর্ণাঙ্ক length = xcb_xkb_get_names_মান_list_চাবি_aliases_length(reply, list);
    xcb_xkb_চাবি_alias_iterator_t iter =
        xcb_xkb_get_names_মান_list_চাবি_aliases_iterator(reply, list);

    keymap->অঙ্ক_চাবি_aliases = reply->n_চাবি_Aliases;
    ALLOC_OR_FAIL(keymap->চাবি_aliases, keymap->অঙ্ক_চাবি_aliases);

    প্রতি (পূর্ণাঙ্ক i = 0; i < length; i++) {
        xcb_xkb_চাবি_alias_t *wire = iter.data;
        সংরচনা xkb_চাবি_alias *alias = &keymap->চাবি_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_চাবি_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_চাবি_NAME_LENGTH));
        যদি (!alias->real || !alias->alias)
            গচ্ছ fail;

        xcb_xkb_চাবি_alias_next(&iter);
    }

    প্রত্যাগম true;

fail:
    প্রত্যাগম false;
}

স্থির bool
get_names(সংরচনা xkb_keymap *keymap, সংরচনা x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, রিক্ত);
    xcb_xkb_get_names_মান_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_মান_list_unpack(xcb_xkb_get_names_মান_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_চাবিগুলি,
                                        reply->n_চাবি_Aliases,
                                        reply->nRadioGroups,
                                        reply->which,
                                        &list);

    x11_atom_interner_get_escaped_atom_name(interner, list.keycodesName,
                                            &keymap->keycodes_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.symbolsName,
                                            &keymap->symbols_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.typesName,
                                            &keymap->types_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.compatName,
                                            &keymap->compat_section_name);
    যদি (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_চাবি_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        গচ্ছ fail;

    মুক্ত(reply);
    প্রত্যাগম true;

fail:
    মুক্ত(reply);
    প্রত্যাগম false;
}

স্থির bool
get_controls(সংরচনা xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, রিক্ত);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->অঙ্ক_Groups > 0 && reply->অঙ্ক_Groups <= 4);

    keymap->অঙ্ক_groups = reply->অঙ্ক_Groups;

    FAIL_UNLESS(keymap->max_চাবি_code < XCB_XKB_CONST_PER_চাবি_BIT_ARRAY_SIZE * 8);

    প্রতি (xkb_keycode_t i = keymap->min_চাবি_code; i <= keymap->max_চাবি_code; i++)
        keymap->চাবিগুলি[i].repeats = (reply->per_চাবি_Repeat[i / 8] & (1u << (i % 8)));

    মুক্ত(reply);
    প্রত্যাগম true;

fail:
    মুক্ত(reply);
    প্রত্যাগম false;
}

সংরচনা xkb_keymap *
xkb_x11_keymap_new_from_device(সংরচনা xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               গণনা xkb_keymap_compile_flags flags)
{
    যদি (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        প্রত্যাগম রিক্ত;
    }

    ধ্রুব গণনা xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    সংরচনা xkb_keymap * ধ্রুব keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    যদি (!keymap)
        প্রত্যাগম রিক্ত;

    keymap->redirect_চাবি_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    সংরচনা x11_atom_interner interner;
    x11_atom_interner_init(&interner, ctx, conn);

    /*
     * Send all requests together so only one roundtrip is needed
     * to get the replies.
     */
    xcb_xkb_get_map_cookie_t map_cookie =
        xcb_xkb_get_map(conn, device_id, get_map_required_components,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    xcb_xkb_get_indicator_map_cookie_t indicator_map_cookie =
        xcb_xkb_get_indicator_map(conn, device_id, ALL_INDICATORS_MASK);
    xcb_xkb_get_compat_map_cookie_t compat_map_cookie =
        xcb_xkb_get_compat_map(conn, device_id, 0, true, 0, 0);
    xcb_xkb_get_names_cookie_t names_cookie =
        xcb_xkb_get_names(conn, device_id, get_names_wanted);
    xcb_xkb_get_controls_cookie_t controls_cookie =
        xcb_xkb_get_controls(conn, device_id);

    যদি (!get_map(keymap, conn, map_cookie))
        গচ্ছ err_map;
    যদি (!get_indicator_map(keymap, conn, indicator_map_cookie))
        গচ্ছ err_indicator_map;
    যদি (!get_compat_map(keymap, conn, compat_map_cookie))
        গচ্ছ err_compat_map;
    যদি (!get_names(keymap, &interner, names_cookie))
        গচ্ছ err_names;
    যদি (!get_controls(keymap, conn, controls_cookie))
        গচ্ছ err_controls;
    x11_atom_interner_round_trip(&interner);
    যদি (interner.had_error)
        গচ্ছ err_interner;

    প্রত্যাগম keymap;

err_map:
    xcb_discard_reply(conn, indicator_map_cookie.sequence);
err_indicator_map:
    xcb_discard_reply(conn, compat_map_cookie.sequence);
err_compat_map:
    xcb_discard_reply(conn, names_cookie.sequence);
err_names:
    xcb_discard_reply(conn, controls_cookie.sequence);
err_controls:
    x11_atom_interner_round_trip(&interner);
err_interner:
    xkb_keymap_unref(keymap);
    প্রত্যাগম রিক্ত;
}
