/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <xcb/xkb.h>

#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-keysyms.h"
#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "x11-priv.h"

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
#define ಅಂಕೆ_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define ಅಂಕೆ_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define ಅಂಕೆ_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(ಅಭಿವ್ಯಕ್ತಿ) ಕುರು {                              \
    ಯದಿ (!(ಅಭಿವ್ಯಕ್ತಿ)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(ಅಭಿವ್ಯಕ್ತಿ));                 \
        ಗಚ್ಛ fail;                                          \
    }                                                       \
} ಯಾವತ್ (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) ಕುರು {         \
    ಯದಿ (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        ಗಚ್ಛ fail;                                          \
    }                                                       \
} ಯಾವತ್ (0)

#define ALLOC_OR_FAIL(arr, nmemb) ಕುರು {                      \
    ಯದಿ ((nmemb) > 0) {                                      \
        (arr) = calloc((nmemb), ಪರಿಮಾಣ(*(arr)));            \
        ಯದಿ (!(arr))                                         \
            ಗಚ್ಛ fail;                                      \
    }                                                       \
} ಯಾವತ್ (0)

ಸ್ಥಿರ ಧ್ರುವ xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_ಕೀಲಿ_TYPES |
     XCB_XKB_MAP_PART_ಕೀಲಿ_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_ಕೀಲಿ_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

ಸ್ಥಿರ ಧ್ರುವ xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_ಕೀಲಿ_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_ಕೀಲಿ_NAMES |
     XCB_XKB_NAME_DETAIL_ಕೀಲಿ_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
ಸ್ಥಿರ ಧ್ರುವ xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_ಕೀಲಿ_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_ಕೀಲಿ_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


ಸ್ಥಿರ xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_ಕೆಳಗಿನ, uint16_t vmods_ಮೇಲಿನ)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    ಪ್ರತ್ಯಾಗಮ
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_ಕೆಳಗಿನ << 8) |
        ((xkb_mod_mask_t) vmods_ಮೇಲಿನ << 16);
}

ಗಣನಾ xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    ಗಣನಾ xkb_action_controls ret = 0;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_REPEAT_ಕೀಲಿಗಳು)
        ret |= CONTROL_REPEAT;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_SLOW_ಕೀಲಿಗಳು)
        ret |= CONTROL_SLOW;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_BOUNCE_ಕೀಲಿಗಳು)
        ret |= CONTROL_DEBOUNCE;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_STICKY_ಕೀಲಿಗಳು)
        ret |= CONTROL_STICKY_ಕೀಲಿಗಳು;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_MOUSE_ಕೀಲಿಗಳು)
        ret |= CONTROL_MOUSE_ಕೀಲಿಗಳು;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_MOUSE_ಕೀಲಿಗಳು_ACCEL)
        ret |= CONTROL_MOUSE_ಕೀಲಿಗಳು_ACCEL;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_ಕೀಲಿಗಳು)
        ret |= CONTROL_AX;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    ಯದಿ (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ bool
translate_action(ಸಙ್ಘ xkb_action *action, ಧ್ರುವ xcb_xkb_action_t *wire,
                 xkb_keycode_t min_ಕೀಲಿ_code, xkb_keycode_t max_ಕೀಲಿ_code)
{
    ವಿಕಲ್ಪ (wire->type) {
    ಪಕ್ಷ XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_ಕೆಳಗಿನ_2,
                                                wire->setmods.vmods_ಮೇಲಿನ_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        ಯದಿ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        ಯದಿ (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        ಯದಿ (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_ಕೆಳಗಿನ_2,
                                                wire->latchmods.vmods_ಮೇಲಿನ_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        ಯದಿ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        ಯದಿ (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        ಯದಿ (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_ಕೆಳಗಿನ_2,
                                                wire->lockmods.vmods_ಮೇಲಿನ_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        ಯದಿ (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        ಯದಿ (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        ಯದಿ (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        ಯದಿ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        ಯದಿ (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        ಯದಿ (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        ಯದಿ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        ಯದಿ (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        ಯದಿ (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        ಯದಿ (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_ಕೆಳಗಿನ | ((uint16_t) wire->moveptr.x_ಮೇಲಿನ << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_ಕೆಳಗಿನ | ((uint16_t) wire->moveptr.y_ಮೇಲಿನ << 8));

        ಯದಿ (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        ಯದಿ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        ಯದಿ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.ಎಣಿಕೆ = wire->ptrbtn.ಎಣಿಕೆ;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.ಎಣಿಕೆ = 0;

        ಯದಿ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        ಯದಿ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.ಮೌಲ್ಯ = wire->setptrdflt.ಮೌಲ್ಯ;

        ಯದಿ (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        ಯದಿ (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        ಯದಿ (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            ಧ್ರುವ uint16_t mask = (wire->setcontrols.bool_Ctrls_ಕೆಳಗಿನ |
                                   (wire->setcontrols.bool_Ctrls_ಮೇಲಿನ << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        ಭಙ್ಗ;
    ಪಕ್ಷ XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            ಧ್ರುವ uint16_t mask = (wire->lockcontrols.bool_Ctrls_ಕೆಳಗಿನ |
                                   (wire->lockcontrols.bool_Ctrls_ಮೇಲಿನ << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        ಭಙ್ಗ;

    ಪಕ್ಷ XCB_XKB_SA_TYPE_REDIRECT_ಕೀಲಿ:
        action->type = ACTION_TYPE_REDIRECT_ಕೀಲಿ;
        ಯದಿ (wire->redirect.newkey < min_ಕೀಲಿ_code ||
            wire->redirect.newkey > max_ಕೀಲಿ_code)
                ಪ್ರತ್ಯಾಗಮ false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_ಕೆಳಗಿನ,
            wire->redirect.vmods_Mask_ಮೇಲಿನ
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_ಕೆಳಗಿನ_2,
                                               wire->redirect.vmods_ಮೇಲಿನ_2);
        ಭಙ್ಗ;

    ಪಕ್ಷ XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        ಭಙ್ಗ;

    /* We don't support these. */
    ಪಕ್ಷ XCB_XKB_SA_TYPE_ISO_LOCK:
    ಪಕ್ಷ XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    ಪಕ್ಷ XCB_XKB_SA_TYPE_DEVICE_BTN:
    ಪಕ್ಷ XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    ಪಕ್ಷ XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
                      "Missing action type");

        ಯದಿ (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            ಭಙ್ಗ;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(ಪರಿಮಾಣ(action->priv.data) == 7 &&
                      ಪರಿಮಾಣ(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        memcpy(action->priv.data, wire->noaction.pad0, 7);
        ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
get_types(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಙ್ಕ types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_ಕೀಲಿ_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->ಅಂಕೆ_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->ಅಂಕೆ_types);
    ಪ್ರತಿಜ್ಞಾ(types_length == (ಪೂರ್ಣಾಙ್ಕ) keymap->ಅಂಕೆ_types);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < types_length; i++) {
        xcb_xkb_ಕೀಲಿ_type_t *wire_type = types_iter.data;
        ಸಂರಚನಾ xkb_ಕೀಲಿ_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->ಅಂಕೆ_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->ಅಂಕೆ_levels = wire_type->ಅಂಕೆ_Levels;

        {
            ಪೂರ್ಣಾಙ್ಕ entries_length = xcb_xkb_ಕೀಲಿ_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_ಕೀಲಿ_type_map_iterator(wire_type);

            type->ಅಂಕೆ_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->ಅಂಕೆ_entries);
            ಪ್ರತಿಜ್ಞಾ(entries_length == (ಪೂರ್ಣಾಙ್ಕ) type->ಅಂಕೆ_entries);

            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->ಅಂಕೆ_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            ಪೂರ್ಣಾಙ್ಕ preserves_length = xcb_xkb_ಕೀಲಿ_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_ಕೀಲಿ_type_preserve_iterator(wire_type);

            FAIL_UNLESS((unsigned) preserves_length <= type->ಅಂಕೆ_entries);

            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_ಕೀಲಿ_type_next(&types_iter);
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_sym_maps(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಙ್ಕ sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_ಕೀಲಿ_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_ಕೀಲಿ_Code <= reply->max_ಕೀಲಿ_Code);
    FAIL_UNLESS(reply->first_ಕೀಲಿ_Sym >= reply->min_ಕೀಲಿ_Code);
    FAIL_UNLESS(reply->first_ಕೀಲಿ_Sym + reply->n_ಕೀಲಿ_Syms <= reply->max_ಕೀಲಿ_Code + 1);

    keymap->min_ಕೀಲಿ_code = reply->min_ಕೀಲಿ_Code;
    keymap->max_ಕೀಲಿ_code = reply->max_ಕೀಲಿ_Code;
    keymap->ಅಂಕೆ_ಕೀಲಿಗಳು = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ = reply->max_ಕೀಲಿ_Code + 1;

    ALLOC_OR_FAIL(keymap->ಕೀಲಿಗಳು, keymap->max_ಕೀಲಿ_code + 1);

    ಪ್ರತಿ (xkb_keycode_t kc = keymap->min_ಕೀಲಿ_code; kc <= keymap->max_ಕೀಲಿ_code; kc++)
        keymap->ಕೀಲಿಗಳು[kc].keycode = kc;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < sym_maps_length; i++) {
        xcb_xkb_ಕೀಲಿ_sym_map_t *wire_sym_map = sym_maps_iter.data;
        ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[reply->first_ಕೀಲಿ_Sym + i];

        ಕೀಲಿ->ಅಂಕೆ_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(ಕೀಲಿ->ಅಂಕೆ_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(ಕೀಲಿ->groups, ಕೀಲಿ->ಅಂಕೆ_groups);

        ಪ್ರತಿ (xkb_layout_index_t j = 0; j < ಕೀಲಿ->ಅಂಕೆ_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->ಅಂಕೆ_types);
            ಕೀಲಿ->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(ಕೀಲಿ->groups[j].levels, ಕೀಲಿ->groups[j].type->ಅಂಕೆ_levels);
        }

        ಕೀಲಿ->out_of_range_group_ಸಂಖ್ಯೆ = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(ಕೀಲಿ->out_of_range_group_ಸಂಖ್ಯೆ <= ಕೀಲಿ->ಅಂಕೆ_groups);

        ಯದಿ (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            ಕೀಲಿ->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        ಅನ್ಯಥಾ ಯದಿ (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            ಕೀಲಿ->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        ಅನ್ಯಥಾ
            ಕೀಲಿ->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            ಪೂರ್ಣಾಙ್ಕ syms_length = xcb_xkb_ಕೀಲಿ_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_ಕೀಲಿ_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * ಕೀಲಿ->ಅಂಕೆ_groups);

            ಯದಿ (syms_length > 0)
                ಕೀಲಿ->explicit |= EXPLICIT_SYMBOLS;

            ಪ್ರತಿ (xkb_layout_index_t group = 0; group < ಕೀಲಿ->ಅಂಕೆ_groups; group++) {
                ಪ್ರತಿ (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    ಪ್ರತಿಜ್ಞಾ(ಕೀಲಿ->groups[group].type != ರಿಕ್ತ);
                    ಯದಿ (level < ಕೀಲಿ->groups[group].type->ಅಂಕೆ_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        ಕೀಲಿ->groups[group].levels[level].ಅಂಕೆ_syms = 1;
                        ಕೀಲಿ->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        ಕೀಲಿ->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        ಯದಿ (wire_keysym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                            ಕೀಲಿ->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_ಕೀಲಿ_sym_map_next(&sym_maps_iter);
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_actions(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಙ್ಕ acts_ಎಣಿಕೆ_length =
        xcb_xkb_get_map_map_acts_rtrn_ಎಣಿಕೆ_length(reply, map);
    uint8_t *acts_ಎಣಿಕೆ_iter = xcb_xkb_get_map_map_acts_rtrn_ಎಣಿಕೆ(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_ಕೀಲಿ_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_ಕೀಲಿ_Action == keymap->min_ಕೀಲಿ_code);
    FAIL_UNLESS(reply->first_ಕೀಲಿ_Action + reply->n_ಕೀಲಿ_Actions ==
                keymap->max_ಕೀಲಿ_code + 1);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < acts_ಎಣಿಕೆ_length; i++) {
        xcb_xkb_ಕೀಲಿ_sym_map_t *wire_sym_map = sym_maps_iter.data;
        ಪೂರ್ಣಾಙ್ಕ syms_length = xcb_xkb_ಕೀಲಿ_sym_map_syms_length(wire_sym_map);
        uint8_t wire_ಎಣಿಕೆ = *acts_ಎಣಿಕೆ_iter;
        ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[reply->first_ಕೀಲಿ_Action + i];

        FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * ಕೀಲಿ->ಅಂಕೆ_groups);
        FAIL_UNLESS(wire_ಎಣಿಕೆ == 0 || wire_ಎಣಿಕೆ == syms_length);

        ಧ್ರುವ xkb_keycode_t min_ಕೀಲಿ_code = keymap->min_ಕೀಲಿ_code;
        ಧ್ರುವ xkb_keycode_t max_ಕೀಲಿ_code = keymap->max_ಕೀಲಿ_code;

        ಯದಿ (wire_ಎಣಿಕೆ != 0) {
            ಪ್ರತಿ (xkb_layout_index_t group = 0; group < ಕೀಲಿ->ಅಂಕೆ_groups; group++) {
                ಪ್ರತಿ (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    ಯದಿ (level < ಕೀಲಿ->groups[group].type->ಅಂಕೆ_levels) {
                        ಕೀಲಿ->groups[group].levels[level].ಅಂಕೆ_actions = 1;
                        ಸಙ್ಘ xkb_action *action =
                            &ಕೀಲಿ->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_ಕೀಲಿ_code, max_ಕೀಲಿ_code));

                        ಯದಿ (action->type != ACTION_TYPE_NONE) {
                            ಕೀಲಿ->groups[group].implicit_actions =
                                !(ಕೀಲಿ->explicit & EXPLICIT_INTERP);
                        } ಅನ್ಯಥಾ ಯದಿ (ಕೀಲಿ->groups[group].levels[level].s.sym ==
                                   XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            ಕೀಲಿ->groups[group].levels[level].ಅಂಕೆ_syms = 0;
                            ಕೀಲಿ->groups[group].levels[level].ಅಂಕೆ_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                ಯದಿ (ಕೀಲಿ->groups[group].implicit_actions)
                    ಕೀಲಿ->implicit_actions = true;
            }
        }

        acts_ಎಣಿಕೆ_iter++;
        xcb_xkb_ಕೀಲಿ_sym_map_next(&sym_maps_iter);
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_vmods(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.ಅಂಕೆ_mods =
        ಅಂಕೆ_REAL_MODS + MIN(msb_pos(reply->virtualMods), ಅಂಕೆ_VMODS);
    FAIL_UNLESS(keymap->mods.ಅಂಕೆ_mods <= XKB_MAX_MODS);

    static_assert(ಅಂಕೆ_REAL_MODS + ಅಂಕೆ_VMODS <= XKB_MAX_MODS, "");
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_VMODS; i++) {
        ಯದಿ (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            ಸಂರಚನಾ xkb_mod *mod = &keymap->mods.mods[ಅಂಕೆ_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_explicits(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಙ್ಕ length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;

        FAIL_UNLESS(wire->keycode >= keymap->min_ಕೀಲಿ_code &&
                    wire->keycode <= keymap->max_ಕೀಲಿ_code);

        ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[wire->keycode];

        ಯದಿ ((wire->explicit & XCB_XKB_EXPLICIT_ಕೀಲಿ_TYPE_1) &&
            ಕೀಲಿ->ಅಂಕೆ_groups > 0) {
            ಕೀಲಿ->groups[0].explicit_type = true;
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
        }
        ಯದಿ ((wire->explicit & XCB_XKB_EXPLICIT_ಕೀಲಿ_TYPE_2) &&
            ಕೀಲಿ->ಅಂಕೆ_groups > 1) {
            ಕೀಲಿ->groups[1].explicit_type = true;
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
        }
        ಯದಿ ((wire->explicit & XCB_XKB_EXPLICIT_ಕೀಲಿ_TYPE_3) &&
            ಕೀಲಿ->ಅಂಕೆ_groups > 2) {
            ಕೀಲಿ->groups[2].explicit_type = true;
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
        }
        ಯದಿ ((wire->explicit & XCB_XKB_EXPLICIT_ಕೀಲಿ_TYPE_4) &&
            ಕೀಲಿ->ಅಂಕೆ_groups > 3) {
            ಕೀಲಿ->groups[3].explicit_type = true;
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
        }
        ಯದಿ (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            ಕೀಲಿ->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            ಪ್ರತಿ (xkb_layout_index_t l = 0; l < ಕೀಲಿ->ಅಂಕೆ_groups; l++) {
                ಕೀಲಿ->groups[l].explicit_actions = true;
            }
        }
        ಯದಿ (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            ಕೀಲಿ->explicit |= EXPLICIT_REPEAT;
        ಯದಿ (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            ಕೀಲಿ->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_modmaps(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಙ್ಕ length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_ಕೀಲಿ_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < length; i++) {
        xcb_xkb_ಕೀಲಿ_mod_map_t *wire = iter.data;
        ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;

        FAIL_UNLESS(wire->keycode >= keymap->min_ಕೀಲಿ_code &&
                    wire->keycode <= keymap->max_ಕೀಲಿ_code);

        ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[wire->keycode];
        ಕೀಲಿ->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .ಕೀಲಿ_Code = ಕೀಲಿ->keycode,
            .mods = ಕೀಲಿ->modmap,
            .have_ಚಿಹ್ನೆ = false,
            .u.ಕೀಲಿ_Name = ಕೀಲಿ->name
        });

        xcb_xkb_ಕೀಲಿ_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->ಅಂಕೆ_modmaps);
    ಪ್ರತ್ಯಾಗಮ true;

fail:
    darray_free(modmaps);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_vmodmaps(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಙ್ಕ length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_ಕೀಲಿ_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < length; i++) {
        xcb_xkb_ಕೀಲಿ_v_mod_map_t *wire = iter.data;
        ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;

        FAIL_UNLESS(wire->keycode >= keymap->min_ಕೀಲಿ_code &&
                    wire->keycode <= keymap->max_ಕೀಲಿ_code);

        ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[wire->keycode];
        ಕೀಲಿ->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_ಕೀಲಿ_v_mod_map_next(&iter);
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_map(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, ರಿಕ್ತ);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    ಯದಿ ((reply->present & get_map_required_components) != get_map_required_components)
        ಗಚ್ಛ fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_ಕೀಲಿ_Syms,
                               reply->n_ಕೀಲಿ_Actions,
                               reply->totalActions,
                               reply->total_ಕೀಲಿ_Behaviors,
                               reply->virtualMods,
                               reply->total_ಕೀಲಿ_Explicit,
                               reply->total_Mod_Map_ಕೀಲಿಗಳು,
                               reply->total_VMod_Map_ಕೀಲಿಗಳು,
                               reply->present,
                               &map);

    ಯದಿ (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        ಗಚ್ಛ fail;

    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_indicators(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->ಅಂಕೆ_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->ಅಂಕೆ_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == ಅಂಕೆ_INDICATORS, "");
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_INDICATORS; i++) {
        ಯದಿ (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            ಸಂರಚನಾ xkb_led *led = &keymap->leds[i];

            ಯದಿ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            ಯದಿ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            ಯದಿ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            ಯದಿ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            ಯದಿ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            ಯದಿ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            ಯದಿ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            ಯದಿ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            ಯದಿ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            ಯದಿ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_indicator_map(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, ರಿಕ್ತ);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    ಯದಿ (!get_indicators(keymap, conn, reply))
        ಗಚ್ಛ fail;

    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_sym_interprets(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    ಪೂರ್ಣಾಙ್ಕ length = xcb_xkb_get_compat_map_si_rtrn_length(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->nTotalSI);

    keymap->ಅಂಕೆ_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->ಅಂಕೆ_sym_interprets);

    ಧ್ರುವ xkb_keycode_t min_ಕೀಲಿ_code = keymap->min_ಕೀಲಿ_code;
    ಧ್ರುವ xkb_keycode_t max_ಕೀಲಿ_code = keymap->max_ಕೀಲಿ_code;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        ಸಂರಚನಾ xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        ವಿಕಲ್ಪ (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        ಪಕ್ಷ XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            ಭಙ್ಗ;
        ಪಕ್ಷ XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            ಭಙ್ಗ;
        ಪಕ್ಷ XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            ಭಙ್ಗ;
        ಪಕ್ಷ XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            ಭಙ್ಗ;
        ಪಕ್ಷ XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            ಗಚ್ಛ fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        ಯದಿ (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        ಅನ್ಯಥಾ
            sym_interpret->virtual_mod = ಅಂಕೆ_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_ಕೀಲಿ_code, max_ಕೀಲಿ_code));
        sym_interpret->ಅಂಕೆ_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_compat_map(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, ರಿಕ್ತ);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    ಯದಿ (!get_sym_interprets(keymap, conn, reply))
        ಗಚ್ಛ fail;

    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_type_names(ಸಂರಚನಾ xkb_keymap *keymap, ಸಂರಚನಾ x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_ಮೌಲ್ಯ_list_t *list)
{
    ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ_type_names_length =
        xcb_xkb_get_names_ಮೌಲ್ಯ_list_type_names_length(reply, list);
    xcb_atom_t *ಕೀಲಿ_type_names_iter =
        xcb_xkb_get_names_ಮೌಲ್ಯ_list_type_names(list);
    ಪೂರ್ಣಾಙ್ಕ n_levels_per_type_length =
        xcb_xkb_get_names_ಮೌಲ್ಯ_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_ಮೌಲ್ಯ_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_ಮೌಲ್ಯ_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->ಅಂಕೆ_types);
    FAIL_UNLESS(ಕೀಲಿ_type_names_length == n_levels_per_type_length);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಕೀಲಿ_type_names_length; i++) {
        xcb_atom_t wire_type_name = *ಕೀಲಿ_type_names_iter;
        uint8_t wire_ಅಂಕೆ_levels = *n_levels_per_type_iter;
        ಸಂರಚನಾ xkb_ಕೀಲಿ_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->ಅಂಕೆ_levels >= wire_ಅಂಕೆ_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->ಅಂಕೆ_levels);
        type->ಅಂಕೆ_level_names = type->ಅಂಕೆ_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        ಪ್ರತಿ (size_t j = 0; j < wire_ಅಂಕೆ_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        ಪ್ರತಿ (size_t j = wire_ಅಂಕೆ_levels; j < type->ಅಂಕೆ_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_ಅಂಕೆ_levels;
        ಕೀಲಿ_type_names_iter++;
        n_levels_per_type_iter++;
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_indicator_names(ಸಂರಚನಾ xkb_keymap *keymap,
                    ಸಂರಚನಾ x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_ಮೌಲ್ಯ_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_ಮೌಲ್ಯ_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->ಅಂಕೆ_leds);

    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_INDICATORS; i++) {
        ಯದಿ (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            ಸಂರಚನಾ xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_vmod_names(ಸಂರಚನಾ xkb_keymap *keymap, ಸಂರಚನಾ x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_ಮೌಲ್ಯ_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_ಮೌಲ್ಯ_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.ಅಂಕೆ_mods =
        ಅಂಕೆ_REAL_MODS + MIN(msb_pos(reply->virtualMods), ಅಂಕೆ_VMODS);
    FAIL_UNLESS(keymap->mods.ಅಂಕೆ_mods <= XKB_MAX_MODS);

    static_assert(ಅಂಕೆ_REAL_MODS + ಅಂಕೆ_VMODS <= XKB_MAX_MODS, "");
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_VMODS; i++) {
        ಯದಿ (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            ಸಂರಚನಾ xkb_mod *mod = &keymap->mods.mods[ಅಂಕೆ_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_group_names(ಸಂರಚನಾ xkb_keymap *keymap, ಸಂರಚನಾ x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_ಮೌಲ್ಯ_list_t *list)
{
    ಪೂರ್ಣಾಙ್ಕ length = xcb_xkb_get_names_ಮೌಲ್ಯ_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_ಮೌಲ್ಯ_list_groups(list);

    keymap->ಅಂಕೆ_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->ಅಂಕೆ_group_names);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_ಕೀಲಿ_names(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_ಮೌಲ್ಯ_list_t *list)
{
    ಪೂರ್ಣಾಙ್ಕ length = xcb_xkb_get_names_ಮೌಲ್ಯ_list_ಕೀಲಿ_names_length(reply, list);
    xcb_xkb_ಕೀಲಿ_name_iterator_t iter =
        xcb_xkb_get_names_ಮೌಲ್ಯ_list_ಕೀಲಿ_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_ಕೀಲಿ_Code == keymap->min_ಕೀಲಿ_code);
    FAIL_UNLESS(reply->max_ಕೀಲಿ_Code == keymap->max_ಕೀಲಿ_code);
    FAIL_UNLESS(reply->first_ಕೀಲಿ == keymap->min_ಕೀಲಿ_code);
    FAIL_UNLESS(reply->first_ಕೀಲಿ + reply->n_ಕೀಲಿಗಳು - 1U == keymap->max_ಕೀಲಿ_code);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < length; i++) {
        xcb_xkb_ಕೀಲಿ_name_t *wire = iter.data;
        xkb_atom_t *ಕೀಲಿ_name = &keymap->ಕೀಲಿಗಳು[reply->first_ಕೀಲಿ + i].name;

        ಯದಿ (wire->name[0] == '\0') {
            *ಕೀಲಿ_name = XKB_ATOM_NONE;
        }
        ಅನ್ಯಥಾ {
            *ಕೀಲಿ_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_ಕೀಲಿ_NAME_LENGTH));
            ಯದಿ (!*ಕೀಲಿ_name)
                ಪ್ರತ್ಯಾಗಮ false;
        }

        xcb_xkb_ಕೀಲಿ_name_next(&iter);
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_aliases(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_ಮೌಲ್ಯ_list_t *list)
{
    ಪೂರ್ಣಾಙ್ಕ length = xcb_xkb_get_names_ಮೌಲ್ಯ_list_ಕೀಲಿ_aliases_length(reply, list);
    xcb_xkb_ಕೀಲಿ_alias_iterator_t iter =
        xcb_xkb_get_names_ಮೌಲ್ಯ_list_ಕೀಲಿ_aliases_iterator(reply, list);

    keymap->ಅಂಕೆ_ಕೀಲಿ_aliases = reply->n_ಕೀಲಿ_Aliases;
    ALLOC_OR_FAIL(keymap->ಕೀಲಿ_aliases, keymap->ಅಂಕೆ_ಕೀಲಿ_aliases);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < length; i++) {
        xcb_xkb_ಕೀಲಿ_alias_t *wire = iter.data;
        ಸಂರಚನಾ xkb_ಕೀಲಿ_alias *alias = &keymap->ಕೀಲಿ_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_ಕೀಲಿ_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_ಕೀಲಿ_NAME_LENGTH));
        ಯದಿ (!alias->real || !alias->alias)
            ಗಚ್ಛ fail;

        xcb_xkb_ಕೀಲಿ_alias_next(&iter);
    }

    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_names(ಸಂರಚನಾ xkb_keymap *keymap, ಸಂರಚನಾ x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, ರಿಕ್ತ);
    xcb_xkb_get_names_ಮೌಲ್ಯ_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_ಮೌಲ್ಯ_list_unpack(xcb_xkb_get_names_ಮೌಲ್ಯ_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_ಕೀಲಿಗಳು,
                                        reply->n_ಕೀಲಿ_Aliases,
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
    ಯದಿ (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_ಕೀಲಿ_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        ಗಚ್ಛ fail;

    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ bool
get_controls(ಸಂರಚನಾ xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, ರಿಕ್ತ);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->ಅಂಕೆ_Groups > 0 && reply->ಅಂಕೆ_Groups <= 4);

    keymap->ಅಂಕೆ_groups = reply->ಅಂಕೆ_Groups;

    FAIL_UNLESS(keymap->max_ಕೀಲಿ_code < XCB_XKB_CONST_PER_ಕೀಲಿ_BIT_ARRAY_SIZE * 8);

    ಪ್ರತಿ (xkb_keycode_t i = keymap->min_ಕೀಲಿ_code; i <= keymap->max_ಕೀಲಿ_code; i++)
        keymap->ಕೀಲಿಗಳು[i].repeats = (reply->per_ಕೀಲಿ_Repeat[i / 8] & (1u << (i % 8)));

    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ true;

fail:
    ಮುಕ್ತ(reply);
    ಪ್ರತ್ಯಾಗಮ false;
}

ಸಂರಚನಾ xkb_keymap *
xkb_x11_keymap_new_from_device(ಸಂರಚನಾ xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               ಗಣನಾ xkb_keymap_compile_flags flags)
{
    ಯದಿ (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಧ್ರುವ ಗಣನಾ xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    ಸಂರಚನಾ xkb_keymap * ಧ್ರುವ keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    ಯದಿ (!keymap)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    keymap->redirect_ಕೀಲಿ_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    ಸಂರಚನಾ x11_atom_interner interner;
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

    ಯದಿ (!get_map(keymap, conn, map_cookie))
        ಗಚ್ಛ err_map;
    ಯದಿ (!get_indicator_map(keymap, conn, indicator_map_cookie))
        ಗಚ್ಛ err_indicator_map;
    ಯದಿ (!get_compat_map(keymap, conn, compat_map_cookie))
        ಗಚ್ಛ err_compat_map;
    ಯದಿ (!get_names(keymap, &interner, names_cookie))
        ಗಚ್ಛ err_names;
    ಯದಿ (!get_controls(keymap, conn, controls_cookie))
        ಗಚ್ಛ err_controls;
    x11_atom_interner_round_trip(&interner);
    ಯದಿ (interner.had_error)
        ಗಚ್ಛ err_interner;

    ಪ್ರತ್ಯಾಗಮ keymap;

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
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}
