/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <xcb/xkb.h>

#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-keysyms.h"
#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "x11-priv.h"

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
#define അക്കം_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define അക്കം_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define അക്കം_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(വ്യഞ്ജകം) ചെയ്യുക {                              \
    എങ്കിൽ (!(വ്യഞ്ജകം)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(വ്യഞ്ജകം));                 \
        പോകുക fail;                                          \
    }                                                       \
} ആകുമ്പോൾ (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) ചെയ്യുക {         \
    എങ്കിൽ (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        പോകുക fail;                                          \
    }                                                       \
} ആകുമ്പോൾ (0)

#define ALLOC_OR_FAIL(arr, nmemb) ചെയ്യുക {                      \
    എങ്കിൽ ((nmemb) > 0) {                                      \
        (arr) = calloc((nmemb), വലുപ്പം(*(arr)));            \
        എങ്കിൽ (!(arr))                                         \
            പോകുക fail;                                      \
    }                                                       \
} ആകുമ്പോൾ (0)

സ്ഥിതം സ്ഥിരം xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_താക്കോൽ_TYPES |
     XCB_XKB_MAP_PART_താക്കോൽ_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_താക്കോൽ_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

സ്ഥിതം സ്ഥിരം xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_താക്കോൽ_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_താക്കോൽ_NAMES |
     XCB_XKB_NAME_DETAIL_താക്കോൽ_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
സ്ഥിതം സ്ഥിരം xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_താക്കോൽ_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_താക്കോൽ_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


സ്ഥിതം xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_താഴെയുള്ള, uint16_t vmods_മുകളിലെ)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    തിരികെ
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_താഴെയുള്ള << 8) |
        ((xkb_mod_mask_t) vmods_മുകളിലെ << 16);
}

എണ്ണൽ xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    എണ്ണൽ xkb_action_controls ret = 0;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_REPEAT_താക്കോലുകൾ)
        ret |= CONTROL_REPEAT;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_SLOW_താക്കോലുകൾ)
        ret |= CONTROL_SLOW;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_BOUNCE_താക്കോലുകൾ)
        ret |= CONTROL_DEBOUNCE;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_STICKY_താക്കോലുകൾ)
        ret |= CONTROL_STICKY_താക്കോലുകൾ;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_MOUSE_താക്കോലുകൾ)
        ret |= CONTROL_MOUSE_താക്കോലുകൾ;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_MOUSE_താക്കോലുകൾ_ACCEL)
        ret |= CONTROL_MOUSE_താക്കോലുകൾ_ACCEL;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_താക്കോലുകൾ)
        ret |= CONTROL_AX;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    എങ്കിൽ (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    തിരികെ ret;
}

സ്ഥിതം bool
translate_action(സംഘം xkb_action *action, സ്ഥിരം xcb_xkb_action_t *wire,
                 xkb_keycode_t min_താക്കോൽ_code, xkb_keycode_t max_താക്കോൽ_code)
{
    തിരഞ്ഞെടുപ്പ് (wire->type) {
    അവസ്ഥ XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_താഴെയുള്ള_2,
                                                wire->setmods.vmods_മുകളിലെ_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        എങ്കിൽ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        എങ്കിൽ (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        എങ്കിൽ (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_താഴെയുള്ള_2,
                                                wire->latchmods.vmods_മുകളിലെ_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        എങ്കിൽ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        എങ്കിൽ (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        എങ്കിൽ (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_താഴെയുള്ള_2,
                                                wire->lockmods.vmods_മുകളിലെ_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        എങ്കിൽ (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        എങ്കിൽ (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        എങ്കിൽ (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        എങ്കിൽ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        എങ്കിൽ (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        എങ്കിൽ (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        എങ്കിൽ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        എങ്കിൽ (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        എങ്കിൽ (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        എങ്കിൽ (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_താഴെയുള്ള | ((uint16_t) wire->moveptr.x_മുകളിലെ << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_താഴെയുള്ള | ((uint16_t) wire->moveptr.y_മുകളിലെ << 8));

        എങ്കിൽ (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        എങ്കിൽ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        എങ്കിൽ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.എണ്ണം = wire->ptrbtn.എണ്ണം;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.എണ്ണം = 0;

        എങ്കിൽ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        എങ്കിൽ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.മൂല്യം = wire->setptrdflt.മൂല്യം;

        എങ്കിൽ (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        എങ്കിൽ (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        എങ്കിൽ (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            സ്ഥിരം uint16_t mask = (wire->setcontrols.bool_Ctrls_താഴെയുള്ള |
                                   (wire->setcontrols.bool_Ctrls_മുകളിലെ << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        നിർത്തുക;
    അവസ്ഥ XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            സ്ഥിരം uint16_t mask = (wire->lockcontrols.bool_Ctrls_താഴെയുള്ള |
                                   (wire->lockcontrols.bool_Ctrls_മുകളിലെ << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        നിർത്തുക;

    അവസ്ഥ XCB_XKB_SA_TYPE_REDIRECT_താക്കോൽ:
        action->type = ACTION_TYPE_REDIRECT_താക്കോൽ;
        എങ്കിൽ (wire->redirect.newkey < min_താക്കോൽ_code ||
            wire->redirect.newkey > max_താക്കോൽ_code)
                തിരികെ false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_താഴെയുള്ള,
            wire->redirect.vmods_Mask_മുകളിലെ
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_താഴെയുള്ള_2,
                                               wire->redirect.vmods_മുകളിലെ_2);
        നിർത്തുക;

    അവസ്ഥ XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        നിർത്തുക;

    /* We don't support these. */
    അവസ്ഥ XCB_XKB_SA_TYPE_ISO_LOCK:
    അവസ്ഥ XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    അവസ്ഥ XCB_XKB_SA_TYPE_DEVICE_BTN:
    അവസ്ഥ XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    അവസ്ഥ XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_അക്കം_ENTRIES,
                      "Missing action type");

        എങ്കിൽ (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            നിർത്തുക;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(വലുപ്പം(action->priv.data) == 7 &&
                      വലുപ്പം(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        memcpy(action->priv.data, wire->noaction.pad0, 7);
        നിർത്തുക;
    }
    തിരികെ true;
}

സ്ഥിതം bool
get_types(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    പൂർണ്ണസംഖ്യ types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_താക്കോൽ_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->അക്കം_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->അക്കം_types);
    പ്രതിജ്ഞാ(types_length == (പൂർണ്ണസംഖ്യ) keymap->അക്കം_types);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < types_length; i++) {
        xcb_xkb_താക്കോൽ_type_t *wire_type = types_iter.data;
        ഘടന xkb_താക്കോൽ_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->അക്കം_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->അക്കം_levels = wire_type->അക്കം_Levels;

        {
            പൂർണ്ണസംഖ്യ entries_length = xcb_xkb_താക്കോൽ_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_താക്കോൽ_type_map_iterator(wire_type);

            type->അക്കം_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->അക്കം_entries);
            പ്രതിജ്ഞാ(entries_length == (പൂർണ്ണസംഖ്യ) type->അക്കം_entries);

            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                ഘടന xkb_താക്കോൽ_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->അക്കം_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            പൂർണ്ണസംഖ്യ preserves_length = xcb_xkb_താക്കോൽ_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_താക്കോൽ_type_preserve_iterator(wire_type);

            FAIL_UNLESS((unsigned) preserves_length <= type->അക്കം_entries);

            വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                ഘടന xkb_താക്കോൽ_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_താക്കോൽ_type_next(&types_iter);
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_sym_maps(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    പൂർണ്ണസംഖ്യ sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_താക്കോൽ_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_താക്കോൽ_Code <= reply->max_താക്കോൽ_Code);
    FAIL_UNLESS(reply->first_താക്കോൽ_Sym >= reply->min_താക്കോൽ_Code);
    FAIL_UNLESS(reply->first_താക്കോൽ_Sym + reply->n_താക്കോൽ_Syms <= reply->max_താക്കോൽ_Code + 1);

    keymap->min_താക്കോൽ_code = reply->min_താക്കോൽ_Code;
    keymap->max_താക്കോൽ_code = reply->max_താക്കോൽ_Code;
    keymap->അക്കം_താക്കോലുകൾ = keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള = reply->max_താക്കോൽ_Code + 1;

    ALLOC_OR_FAIL(keymap->താക്കോലുകൾ, keymap->max_താക്കോൽ_code + 1);

    വേണ്ടി (xkb_keycode_t kc = keymap->min_താക്കോൽ_code; kc <= keymap->max_താക്കോൽ_code; kc++)
        keymap->താക്കോലുകൾ[kc].keycode = kc;

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < sym_maps_length; i++) {
        xcb_xkb_താക്കോൽ_sym_map_t *wire_sym_map = sym_maps_iter.data;
        ഘടന xkb_താക്കോൽ *താക്കോൽ = &keymap->താക്കോലുകൾ[reply->first_താക്കോൽ_Sym + i];

        താക്കോൽ->അക്കം_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(താക്കോൽ->അക്കം_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(താക്കോൽ->groups, താക്കോൽ->അക്കം_groups);

        വേണ്ടി (xkb_layout_index_t j = 0; j < താക്കോൽ->അക്കം_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->അക്കം_types);
            താക്കോൽ->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(താക്കോൽ->groups[j].levels, താക്കോൽ->groups[j].type->അക്കം_levels);
        }

        താക്കോൽ->out_of_range_group_സംഖ്യ = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(താക്കോൽ->out_of_range_group_സംഖ്യ <= താക്കോൽ->അക്കം_groups);

        എങ്കിൽ (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            താക്കോൽ->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        അല്ലെങ്കിൽ എങ്കിൽ (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            താക്കോൽ->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        അല്ലെങ്കിൽ
            താക്കോൽ->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            പൂർണ്ണസംഖ്യ syms_length = xcb_xkb_താക്കോൽ_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_താക്കോൽ_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * താക്കോൽ->അക്കം_groups);

            എങ്കിൽ (syms_length > 0)
                താക്കോൽ->explicit |= EXPLICIT_SYMBOLS;

            വേണ്ടി (xkb_layout_index_t group = 0; group < താക്കോൽ->അക്കം_groups; group++) {
                വേണ്ടി (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    പ്രതിജ്ഞാ(താക്കോൽ->groups[group].type != ശൂന്യ);
                    എങ്കിൽ (level < താക്കോൽ->groups[group].type->അക്കം_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        താക്കോൽ->groups[group].levels[level].അക്കം_syms = 1;
                        താക്കോൽ->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        താക്കോൽ->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        എങ്കിൽ (wire_keysym != XKB_താക്കോൽ_No_ചിഹ്നം)
                            താക്കോൽ->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_താക്കോൽ_sym_map_next(&sym_maps_iter);
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_actions(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    പൂർണ്ണസംഖ്യ acts_എണ്ണം_length =
        xcb_xkb_get_map_map_acts_rtrn_എണ്ണം_length(reply, map);
    uint8_t *acts_എണ്ണം_iter = xcb_xkb_get_map_map_acts_rtrn_എണ്ണം(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_താക്കോൽ_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_താക്കോൽ_Action == keymap->min_താക്കോൽ_code);
    FAIL_UNLESS(reply->first_താക്കോൽ_Action + reply->n_താക്കോൽ_Actions ==
                keymap->max_താക്കോൽ_code + 1);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < acts_എണ്ണം_length; i++) {
        xcb_xkb_താക്കോൽ_sym_map_t *wire_sym_map = sym_maps_iter.data;
        പൂർണ്ണസംഖ്യ syms_length = xcb_xkb_താക്കോൽ_sym_map_syms_length(wire_sym_map);
        uint8_t wire_എണ്ണം = *acts_എണ്ണം_iter;
        ഘടന xkb_താക്കോൽ *താക്കോൽ = &keymap->താക്കോലുകൾ[reply->first_താക്കോൽ_Action + i];

        FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * താക്കോൽ->അക്കം_groups);
        FAIL_UNLESS(wire_എണ്ണം == 0 || wire_എണ്ണം == syms_length);

        സ്ഥിരം xkb_keycode_t min_താക്കോൽ_code = keymap->min_താക്കോൽ_code;
        സ്ഥിരം xkb_keycode_t max_താക്കോൽ_code = keymap->max_താക്കോൽ_code;

        എങ്കിൽ (wire_എണ്ണം != 0) {
            വേണ്ടി (xkb_layout_index_t group = 0; group < താക്കോൽ->അക്കം_groups; group++) {
                വേണ്ടി (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    എങ്കിൽ (level < താക്കോൽ->groups[group].type->അക്കം_levels) {
                        താക്കോൽ->groups[group].levels[level].അക്കം_actions = 1;
                        സംഘം xkb_action *action =
                            &താക്കോൽ->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_താക്കോൽ_code, max_താക്കോൽ_code));

                        എങ്കിൽ (action->type != ACTION_TYPE_NONE) {
                            താക്കോൽ->groups[group].implicit_actions =
                                !(താക്കോൽ->explicit & EXPLICIT_INTERP);
                        } അല്ലെങ്കിൽ എങ്കിൽ (താക്കോൽ->groups[group].levels[level].s.sym ==
                                   XKB_താക്കോൽ_No_ചിഹ്നം) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            താക്കോൽ->groups[group].levels[level].അക്കം_syms = 0;
                            താക്കോൽ->groups[group].levels[level].അക്കം_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                എങ്കിൽ (താക്കോൽ->groups[group].implicit_actions)
                    താക്കോൽ->implicit_actions = true;
            }
        }

        acts_എണ്ണം_iter++;
        xcb_xkb_താക്കോൽ_sym_map_next(&sym_maps_iter);
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_vmods(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.അക്കം_mods =
        അക്കം_REAL_MODS + MIN(msb_pos(reply->virtualMods), അക്കം_VMODS);
    FAIL_UNLESS(keymap->mods.അക്കം_mods <= XKB_MAX_MODS);

    static_assert(അക്കം_REAL_MODS + അക്കം_VMODS <= XKB_MAX_MODS, "");
    വേണ്ടി (unsigned i = 0; i < അക്കം_VMODS; i++) {
        എങ്കിൽ (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            ഘടന xkb_mod *mod = &keymap->mods.mods[അക്കം_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_explicits(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    പൂർണ്ണസംഖ്യ length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        ഘടന xkb_താക്കോൽ *താക്കോൽ;

        FAIL_UNLESS(wire->keycode >= keymap->min_താക്കോൽ_code &&
                    wire->keycode <= keymap->max_താക്കോൽ_code);

        താക്കോൽ = &keymap->താക്കോലുകൾ[wire->keycode];

        എങ്കിൽ ((wire->explicit & XCB_XKB_EXPLICIT_താക്കോൽ_TYPE_1) &&
            താക്കോൽ->അക്കം_groups > 0) {
            താക്കോൽ->groups[0].explicit_type = true;
            താക്കോൽ->explicit |= EXPLICIT_TYPES;
        }
        എങ്കിൽ ((wire->explicit & XCB_XKB_EXPLICIT_താക്കോൽ_TYPE_2) &&
            താക്കോൽ->അക്കം_groups > 1) {
            താക്കോൽ->groups[1].explicit_type = true;
            താക്കോൽ->explicit |= EXPLICIT_TYPES;
        }
        എങ്കിൽ ((wire->explicit & XCB_XKB_EXPLICIT_താക്കോൽ_TYPE_3) &&
            താക്കോൽ->അക്കം_groups > 2) {
            താക്കോൽ->groups[2].explicit_type = true;
            താക്കോൽ->explicit |= EXPLICIT_TYPES;
        }
        എങ്കിൽ ((wire->explicit & XCB_XKB_EXPLICIT_താക്കോൽ_TYPE_4) &&
            താക്കോൽ->അക്കം_groups > 3) {
            താക്കോൽ->groups[3].explicit_type = true;
            താക്കോൽ->explicit |= EXPLICIT_TYPES;
        }
        എങ്കിൽ (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            താക്കോൽ->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            വേണ്ടി (xkb_layout_index_t l = 0; l < താക്കോൽ->അക്കം_groups; l++) {
                താക്കോൽ->groups[l].explicit_actions = true;
            }
        }
        എങ്കിൽ (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            താക്കോൽ->explicit |= EXPLICIT_REPEAT;
        എങ്കിൽ (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            താക്കോൽ->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_modmaps(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    പൂർണ്ണസംഖ്യ length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_താക്കോൽ_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < length; i++) {
        xcb_xkb_താക്കോൽ_mod_map_t *wire = iter.data;
        ഘടന xkb_താക്കോൽ *താക്കോൽ;

        FAIL_UNLESS(wire->keycode >= keymap->min_താക്കോൽ_code &&
                    wire->keycode <= keymap->max_താക്കോൽ_code);

        താക്കോൽ = &keymap->താക്കോലുകൾ[wire->keycode];
        താക്കോൽ->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .താക്കോൽ_Code = താക്കോൽ->keycode,
            .mods = താക്കോൽ->modmap,
            .have_ചിഹ്നം = false,
            .u.താക്കോൽ_Name = താക്കോൽ->name
        });

        xcb_xkb_താക്കോൽ_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->അക്കം_modmaps);
    തിരികെ true;

fail:
    darray_free(modmaps);
    തിരികെ false;
}

സ്ഥിതം bool
get_vmodmaps(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    പൂർണ്ണസംഖ്യ length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_താക്കോൽ_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < length; i++) {
        xcb_xkb_താക്കോൽ_v_mod_map_t *wire = iter.data;
        ഘടന xkb_താക്കോൽ *താക്കോൽ;

        FAIL_UNLESS(wire->keycode >= keymap->min_താക്കോൽ_code &&
                    wire->keycode <= keymap->max_താക്കോൽ_code);

        താക്കോൽ = &keymap->താക്കോലുകൾ[wire->keycode];
        താക്കോൽ->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_താക്കോൽ_v_mod_map_next(&iter);
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_map(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, ശൂന്യ);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    എങ്കിൽ ((reply->present & get_map_required_components) != get_map_required_components)
        പോകുക fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_താക്കോൽ_Syms,
                               reply->n_താക്കോൽ_Actions,
                               reply->totalActions,
                               reply->total_താക്കോൽ_Behaviors,
                               reply->virtualMods,
                               reply->total_താക്കോൽ_Explicit,
                               reply->total_Mod_Map_താക്കോലുകൾ,
                               reply->total_VMod_Map_താക്കോലുകൾ,
                               reply->present,
                               &map);

    എങ്കിൽ (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        പോകുക fail;

    സ്വതന്ത്രം(reply);
    തിരികെ true;

fail:
    സ്വതന്ത്രം(reply);
    തിരികെ false;
}

സ്ഥിതം bool
get_indicators(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->അക്കം_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->അക്കം_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == അക്കം_INDICATORS, "");
    വേണ്ടി (unsigned i = 0; i < അക്കം_INDICATORS; i++) {
        എങ്കിൽ (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            ഘടന xkb_led *led = &keymap->leds[i];

            എങ്കിൽ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            എങ്കിൽ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            എങ്കിൽ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            എങ്കിൽ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            എങ്കിൽ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            എങ്കിൽ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            എങ്കിൽ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            എങ്കിൽ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            എങ്കിൽ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            എങ്കിൽ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_indicator_map(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, ശൂന്യ);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    എങ്കിൽ (!get_indicators(keymap, conn, reply))
        പോകുക fail;

    സ്വതന്ത്രം(reply);
    തിരികെ true;

fail:
    സ്വതന്ത്രം(reply);
    തിരികെ false;
}

സ്ഥിതം bool
get_sym_interprets(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    പൂർണ്ണസംഖ്യ length = xcb_xkb_get_compat_map_si_rtrn_length(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->nTotalSI);

    keymap->അക്കം_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->അക്കം_sym_interprets);

    സ്ഥിരം xkb_keycode_t min_താക്കോൽ_code = keymap->min_താക്കോൽ_code;
    സ്ഥിരം xkb_keycode_t max_താക്കോൽ_code = keymap->max_താക്കോൽ_code;

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        ഘടന xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        തിരഞ്ഞെടുപ്പ് (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        അവസ്ഥ XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            നിർത്തുക;
        അവസ്ഥ XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            നിർത്തുക;
        അവസ്ഥ XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            നിർത്തുക;
        അവസ്ഥ XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            നിർത്തുക;
        അവസ്ഥ XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            പോകുക fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        എങ്കിൽ (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        അല്ലെങ്കിൽ
            sym_interpret->virtual_mod = അക്കം_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_താക്കോൽ_code, max_താക്കോൽ_code));
        sym_interpret->അക്കം_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_compat_map(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, ശൂന്യ);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    എങ്കിൽ (!get_sym_interprets(keymap, conn, reply))
        പോകുക fail;

    സ്വതന്ത്രം(reply);
    തിരികെ true;

fail:
    സ്വതന്ത്രം(reply);
    തിരികെ false;
}

സ്ഥിതം bool
get_type_names(ഘടന xkb_keymap *keymap, ഘടന x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_മൂല്യം_list_t *list)
{
    പൂർണ്ണസംഖ്യ താക്കോൽ_type_names_length =
        xcb_xkb_get_names_മൂല്യം_list_type_names_length(reply, list);
    xcb_atom_t *താക്കോൽ_type_names_iter =
        xcb_xkb_get_names_മൂല്യം_list_type_names(list);
    പൂർണ്ണസംഖ്യ n_levels_per_type_length =
        xcb_xkb_get_names_മൂല്യം_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_മൂല്യം_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_മൂല്യം_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->അക്കം_types);
    FAIL_UNLESS(താക്കോൽ_type_names_length == n_levels_per_type_length);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < താക്കോൽ_type_names_length; i++) {
        xcb_atom_t wire_type_name = *താക്കോൽ_type_names_iter;
        uint8_t wire_അക്കം_levels = *n_levels_per_type_iter;
        ഘടന xkb_താക്കോൽ_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->അക്കം_levels >= wire_അക്കം_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->അക്കം_levels);
        type->അക്കം_level_names = type->അക്കം_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        വേണ്ടി (size_t j = 0; j < wire_അക്കം_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        വേണ്ടി (size_t j = wire_അക്കം_levels; j < type->അക്കം_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_അക്കം_levels;
        താക്കോൽ_type_names_iter++;
        n_levels_per_type_iter++;
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_indicator_names(ഘടന xkb_keymap *keymap,
                    ഘടന x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_മൂല്യം_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_മൂല്യം_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->അക്കം_leds);

    വേണ്ടി (unsigned i = 0; i < അക്കം_INDICATORS; i++) {
        എങ്കിൽ (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            ഘടന xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_vmod_names(ഘടന xkb_keymap *keymap, ഘടന x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_മൂല്യം_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_മൂല്യം_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.അക്കം_mods =
        അക്കം_REAL_MODS + MIN(msb_pos(reply->virtualMods), അക്കം_VMODS);
    FAIL_UNLESS(keymap->mods.അക്കം_mods <= XKB_MAX_MODS);

    static_assert(അക്കം_REAL_MODS + അക്കം_VMODS <= XKB_MAX_MODS, "");
    വേണ്ടി (unsigned i = 0; i < അക്കം_VMODS; i++) {
        എങ്കിൽ (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            ഘടന xkb_mod *mod = &keymap->mods.mods[അക്കം_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_group_names(ഘടന xkb_keymap *keymap, ഘടന x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_മൂല്യം_list_t *list)
{
    പൂർണ്ണസംഖ്യ length = xcb_xkb_get_names_മൂല്യം_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_മൂല്യം_list_groups(list);

    keymap->അക്കം_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->അക്കം_group_names);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_താക്കോൽ_names(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_മൂല്യം_list_t *list)
{
    പൂർണ്ണസംഖ്യ length = xcb_xkb_get_names_മൂല്യം_list_താക്കോൽ_names_length(reply, list);
    xcb_xkb_താക്കോൽ_name_iterator_t iter =
        xcb_xkb_get_names_മൂല്യം_list_താക്കോൽ_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_താക്കോൽ_Code == keymap->min_താക്കോൽ_code);
    FAIL_UNLESS(reply->max_താക്കോൽ_Code == keymap->max_താക്കോൽ_code);
    FAIL_UNLESS(reply->first_താക്കോൽ == keymap->min_താക്കോൽ_code);
    FAIL_UNLESS(reply->first_താക്കോൽ + reply->n_താക്കോലുകൾ - 1U == keymap->max_താക്കോൽ_code);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < length; i++) {
        xcb_xkb_താക്കോൽ_name_t *wire = iter.data;
        xkb_atom_t *താക്കോൽ_name = &keymap->താക്കോലുകൾ[reply->first_താക്കോൽ + i].name;

        എങ്കിൽ (wire->name[0] == '\0') {
            *താക്കോൽ_name = XKB_ATOM_NONE;
        }
        അല്ലെങ്കിൽ {
            *താക്കോൽ_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_താക്കോൽ_NAME_LENGTH));
            എങ്കിൽ (!*താക്കോൽ_name)
                തിരികെ false;
        }

        xcb_xkb_താക്കോൽ_name_next(&iter);
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_aliases(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_മൂല്യം_list_t *list)
{
    പൂർണ്ണസംഖ്യ length = xcb_xkb_get_names_മൂല്യം_list_താക്കോൽ_aliases_length(reply, list);
    xcb_xkb_താക്കോൽ_alias_iterator_t iter =
        xcb_xkb_get_names_മൂല്യം_list_താക്കോൽ_aliases_iterator(reply, list);

    keymap->അക്കം_താക്കോൽ_aliases = reply->n_താക്കോൽ_Aliases;
    ALLOC_OR_FAIL(keymap->താക്കോൽ_aliases, keymap->അക്കം_താക്കോൽ_aliases);

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < length; i++) {
        xcb_xkb_താക്കോൽ_alias_t *wire = iter.data;
        ഘടന xkb_താക്കോൽ_alias *alias = &keymap->താക്കോൽ_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_താക്കോൽ_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_താക്കോൽ_NAME_LENGTH));
        എങ്കിൽ (!alias->real || !alias->alias)
            പോകുക fail;

        xcb_xkb_താക്കോൽ_alias_next(&iter);
    }

    തിരികെ true;

fail:
    തിരികെ false;
}

സ്ഥിതം bool
get_names(ഘടന xkb_keymap *keymap, ഘടന x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, ശൂന്യ);
    xcb_xkb_get_names_മൂല്യം_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_മൂല്യം_list_unpack(xcb_xkb_get_names_മൂല്യം_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_താക്കോലുകൾ,
                                        reply->n_താക്കോൽ_Aliases,
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
    എങ്കിൽ (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_താക്കോൽ_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        പോകുക fail;

    സ്വതന്ത്രം(reply);
    തിരികെ true;

fail:
    സ്വതന്ത്രം(reply);
    തിരികെ false;
}

സ്ഥിതം bool
get_controls(ഘടന xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, ശൂന്യ);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->അക്കം_Groups > 0 && reply->അക്കം_Groups <= 4);

    keymap->അക്കം_groups = reply->അക്കം_Groups;

    FAIL_UNLESS(keymap->max_താക്കോൽ_code < XCB_XKB_CONST_PER_താക്കോൽ_BIT_ARRAY_SIZE * 8);

    വേണ്ടി (xkb_keycode_t i = keymap->min_താക്കോൽ_code; i <= keymap->max_താക്കോൽ_code; i++)
        keymap->താക്കോലുകൾ[i].repeats = (reply->per_താക്കോൽ_Repeat[i / 8] & (1u << (i % 8)));

    സ്വതന്ത്രം(reply);
    തിരികെ true;

fail:
    സ്വതന്ത്രം(reply);
    തിരികെ false;
}

ഘടന xkb_keymap *
xkb_x11_keymap_new_from_device(ഘടന xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               എണ്ണൽ xkb_keymap_compile_flags flags)
{
    എങ്കിൽ (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        തിരികെ ശൂന്യ;
    }

    സ്ഥിരം എണ്ണൽ xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    ഘടന xkb_keymap * സ്ഥിരം keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    എങ്കിൽ (!keymap)
        തിരികെ ശൂന്യ;

    keymap->redirect_താക്കോൽ_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    ഘടന x11_atom_interner interner;
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

    എങ്കിൽ (!get_map(keymap, conn, map_cookie))
        പോകുക err_map;
    എങ്കിൽ (!get_indicator_map(keymap, conn, indicator_map_cookie))
        പോകുക err_indicator_map;
    എങ്കിൽ (!get_compat_map(keymap, conn, compat_map_cookie))
        പോകുക err_compat_map;
    എങ്കിൽ (!get_names(keymap, &interner, names_cookie))
        പോകുക err_names;
    എങ്കിൽ (!get_controls(keymap, conn, controls_cookie))
        പോകുക err_controls;
    x11_atom_interner_round_trip(&interner);
    എങ്കിൽ (interner.had_error)
        പോകുക err_interner;

    തിരികെ keymap;

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
    തിരികെ ശൂന്യ;
}
