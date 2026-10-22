/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <xcb/xkb.h>

#अन्तर्भाव "darray.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-keysyms.h"
#अन्तर्भाव "atom.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "x11-priv.h"

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
#define अङ्कः_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define अङ्कः_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define अङ्कः_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(व्यञ्जकम्) करेहि {                              \
    जइ (!(व्यञ्जकम्)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(व्यञ्जकम्));                 \
        गच्छ fail;                                          \
    }                                                       \
} जाव (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) करेहि {         \
    जइ (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        गच्छ fail;                                          \
    }                                                       \
} जाव (0)

#define ALLOC_OR_FAIL(arr, nmemb) करेहि {                      \
    जइ ((nmemb) > 0) {                                      \
        (arr) = calloc((nmemb), परिमाण(*(arr)));            \
        जइ (!(arr))                                         \
            गच्छ fail;                                      \
    }                                                       \
} जाव (0)

स्थिर ध्रुव xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_कुञ्चिका_TYPES |
     XCB_XKB_MAP_PART_कुञ्चिका_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_कुञ्चिका_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

स्थिर ध्रुव xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_कुञ्चिका_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_कुञ्चिका_NAMES |
     XCB_XKB_NAME_DETAIL_कुञ्चिका_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
स्थिर ध्रुव xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_कुञ्चिका_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_कुञ्चिका_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


स्थिर xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_नीचः, uint16_t vmods_उच्चः)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    प्रत्यागम
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_नीचः << 8) |
        ((xkb_mod_mask_t) vmods_उच्चः << 16);
}

गणना xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    गणना xkb_action_controls ret = 0;
    जइ (wire & XCB_XKB_BOOL_CTRL_REPEAT_कुञ्चिकाः)
        ret |= CONTROL_REPEAT;
    जइ (wire & XCB_XKB_BOOL_CTRL_SLOW_कुञ्चिकाः)
        ret |= CONTROL_SLOW;
    जइ (wire & XCB_XKB_BOOL_CTRL_BOUNCE_कुञ्चिकाः)
        ret |= CONTROL_DEBOUNCE;
    जइ (wire & XCB_XKB_BOOL_CTRL_STICKY_कुञ्चिकाः)
        ret |= CONTROL_STICKY_कुञ्चिकाः;
    जइ (wire & XCB_XKB_BOOL_CTRL_MOUSE_कुञ्चिकाः)
        ret |= CONTROL_MOUSE_कुञ्चिकाः;
    जइ (wire & XCB_XKB_BOOL_CTRL_MOUSE_कुञ्चिकाः_ACCEL)
        ret |= CONTROL_MOUSE_कुञ्चिकाः_ACCEL;
    जइ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_कुञ्चिकाः)
        ret |= CONTROL_AX;
    जइ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    जइ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    जइ (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    जइ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    जइ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    जइ (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    प्रत्यागम ret;
}

स्थिर bool
translate_action(सङ्घ xkb_action *action, ध्रुव xcb_xkb_action_t *wire,
                 xkb_keycode_t min_कुञ्चिका_code, xkb_keycode_t max_कुञ्चिका_code)
{
    विकल्प (wire->type) {
    पक्ष XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_नीचः_2,
                                                wire->setmods.vmods_उच्चः_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        जइ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        जइ (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        जइ (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_नीचः_2,
                                                wire->latchmods.vmods_उच्चः_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        जइ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        जइ (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        जइ (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_नीचः_2,
                                                wire->lockmods.vmods_उच्चः_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        जइ (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        जइ (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        जइ (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        जइ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        जइ (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        जइ (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        जइ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        जइ (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        जइ (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        जइ (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_नीचः | ((uint16_t) wire->moveptr.x_उच्चः << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_नीचः | ((uint16_t) wire->moveptr.y_उच्चः << 8));

        जइ (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        जइ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        जइ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.गणनम् = wire->ptrbtn.गणनम्;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.गणनम् = 0;

        जइ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        जइ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.मानम् = wire->setptrdflt.मानम्;

        जइ (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        जइ (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        जइ (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            ध्रुव uint16_t mask = (wire->setcontrols.bool_Ctrls_नीचः |
                                   (wire->setcontrols.bool_Ctrls_उच्चः << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            ध्रुव uint16_t mask = (wire->lockcontrols.bool_Ctrls_नीचः |
                                   (wire->lockcontrols.bool_Ctrls_उच्चः << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        भङ्ग;

    पक्ष XCB_XKB_SA_TYPE_REDIRECT_कुञ्चिका:
        action->type = ACTION_TYPE_REDIRECT_कुञ्चिका;
        जइ (wire->redirect.newkey < min_कुञ्चिका_code ||
            wire->redirect.newkey > max_कुञ्चिका_code)
                प्रत्यागम false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_नीचः,
            wire->redirect.vmods_Mask_उच्चः
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_नीचः_2,
                                               wire->redirect.vmods_उच्चः_2);
        भङ्ग;

    पक्ष XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        भङ्ग;

    /* We don't support these. */
    पक्ष XCB_XKB_SA_TYPE_ISO_LOCK:
    पक्ष XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    पक्ष XCB_XKB_SA_TYPE_DEVICE_BTN:
    पक्ष XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    पक्ष XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        भङ्ग;

    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अङ्कः_ENTRIES,
                      "Missing action type");

        जइ (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            भङ्ग;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(परिमाण(action->priv.data) == 7 &&
                      परिमाण(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        memcpy(action->priv.data, wire->noaction.pad0, 7);
        भङ्ग;
    }
    प्रत्यागम true;
}

स्थिर bool
get_types(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_कुञ्चिका_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->अङ्कः_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->अङ्कः_types);
    प्रतिज्ञा(types_length == (पूर्णाङ्क) keymap->अङ्कः_types);

    प्रति (पूर्णाङ्क i = 0; i < types_length; i++) {
        xcb_xkb_कुञ्चिका_type_t *wire_type = types_iter.data;
        संरचना xkb_कुञ्चिका_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->अङ्कः_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->अङ्कः_levels = wire_type->अङ्कः_Levels;

        {
            पूर्णाङ्क entries_length = xcb_xkb_कुञ्चिका_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_कुञ्चिका_type_map_iterator(wire_type);

            type->अङ्कः_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->अङ्कः_entries);
            प्रतिज्ञा(entries_length == (पूर्णाङ्क) type->अङ्कः_entries);

            प्रति (पूर्णाङ्क j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                संरचना xkb_कुञ्चिका_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->अङ्कः_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            पूर्णाङ्क preserves_length = xcb_xkb_कुञ्चिका_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_कुञ्चिका_type_preserve_iterator(wire_type);

            FAIL_UNLESS((unsigned) preserves_length <= type->अङ्कः_entries);

            प्रति (पूर्णाङ्क j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                संरचना xkb_कुञ्चिका_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_कुञ्चिका_type_next(&types_iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_sym_maps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_कुञ्चिका_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_कुञ्चिका_Code <= reply->max_कुञ्चिका_Code);
    FAIL_UNLESS(reply->first_कुञ्चिका_Sym >= reply->min_कुञ्चिका_Code);
    FAIL_UNLESS(reply->first_कुञ्चिका_Sym + reply->n_कुञ्चिका_Syms <= reply->max_कुञ्चिका_Code + 1);

    keymap->min_कुञ्चिका_code = reply->min_कुञ्चिका_Code;
    keymap->max_कुञ्चिका_code = reply->max_कुञ्चिका_Code;
    keymap->अङ्कः_कुञ्चिकाः = keymap->अङ्कः_कुञ्चिकाः_नीचः = reply->max_कुञ्चिका_Code + 1;

    ALLOC_OR_FAIL(keymap->कुञ्चिकाः, keymap->max_कुञ्चिका_code + 1);

    प्रति (xkb_keycode_t kc = keymap->min_कुञ्चिका_code; kc <= keymap->max_कुञ्चिका_code; kc++)
        keymap->कुञ्चिकाः[kc].keycode = kc;

    प्रति (पूर्णाङ्क i = 0; i < sym_maps_length; i++) {
        xcb_xkb_कुञ्चिका_sym_map_t *wire_sym_map = sym_maps_iter.data;
        संरचना xkb_कुञ्चिका *कुञ्चिका = &keymap->कुञ्चिकाः[reply->first_कुञ्चिका_Sym + i];

        कुञ्चिका->अङ्कः_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(कुञ्चिका->अङ्कः_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(कुञ्चिका->groups, कुञ्चिका->अङ्कः_groups);

        प्रति (xkb_layout_index_t j = 0; j < कुञ्चिका->अङ्कः_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->अङ्कः_types);
            कुञ्चिका->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(कुञ्चिका->groups[j].levels, कुञ्चिका->groups[j].type->अङ्कः_levels);
        }

        कुञ्चिका->out_of_range_group_सङ्ख्या = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(कुञ्चिका->out_of_range_group_सङ्ख्या <= कुञ्चिका->अङ्कः_groups);

        जइ (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            कुञ्चिका->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        अण्णहा जइ (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            कुञ्चिका->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        अण्णहा
            कुञ्चिका->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            पूर्णाङ्क syms_length = xcb_xkb_कुञ्चिका_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_कुञ्चिका_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * कुञ्चिका->अङ्कः_groups);

            जइ (syms_length > 0)
                कुञ्चिका->explicit |= EXPLICIT_SYMBOLS;

            प्रति (xkb_layout_index_t group = 0; group < कुञ्चिका->अङ्कः_groups; group++) {
                प्रति (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    प्रतिज्ञा(कुञ्चिका->groups[group].type != रिक्त);
                    जइ (level < कुञ्चिका->groups[group].type->अङ्कः_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        कुञ्चिका->groups[group].levels[level].अङ्कः_syms = 1;
                        कुञ्चिका->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        कुञ्चिका->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        जइ (wire_keysym != XKB_कुञ्चिका_No_चिह्नम्)
                            कुञ्चिका->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_कुञ्चिका_sym_map_next(&sym_maps_iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_actions(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क acts_गणनम्_length =
        xcb_xkb_get_map_map_acts_rtrn_गणनम्_length(reply, map);
    uint8_t *acts_गणनम्_iter = xcb_xkb_get_map_map_acts_rtrn_गणनम्(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_कुञ्चिका_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_कुञ्चिका_Action == keymap->min_कुञ्चिका_code);
    FAIL_UNLESS(reply->first_कुञ्चिका_Action + reply->n_कुञ्चिका_Actions ==
                keymap->max_कुञ्चिका_code + 1);

    प्रति (पूर्णाङ्क i = 0; i < acts_गणनम्_length; i++) {
        xcb_xkb_कुञ्चिका_sym_map_t *wire_sym_map = sym_maps_iter.data;
        पूर्णाङ्क syms_length = xcb_xkb_कुञ्चिका_sym_map_syms_length(wire_sym_map);
        uint8_t wire_गणनम् = *acts_गणनम्_iter;
        संरचना xkb_कुञ्चिका *कुञ्चिका = &keymap->कुञ्चिकाः[reply->first_कुञ्चिका_Action + i];

        FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * कुञ्चिका->अङ्कः_groups);
        FAIL_UNLESS(wire_गणनम् == 0 || wire_गणनम् == syms_length);

        ध्रुव xkb_keycode_t min_कुञ्चिका_code = keymap->min_कुञ्चिका_code;
        ध्रुव xkb_keycode_t max_कुञ्चिका_code = keymap->max_कुञ्चिका_code;

        जइ (wire_गणनम् != 0) {
            प्रति (xkb_layout_index_t group = 0; group < कुञ्चिका->अङ्कः_groups; group++) {
                प्रति (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    जइ (level < कुञ्चिका->groups[group].type->अङ्कः_levels) {
                        कुञ्चिका->groups[group].levels[level].अङ्कः_actions = 1;
                        सङ्घ xkb_action *action =
                            &कुञ्चिका->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_कुञ्चिका_code, max_कुञ्चिका_code));

                        जइ (action->type != ACTION_TYPE_NONE) {
                            कुञ्चिका->groups[group].implicit_actions =
                                !(कुञ्चिका->explicit & EXPLICIT_INTERP);
                        } अण्णहा जइ (कुञ्चिका->groups[group].levels[level].s.sym ==
                                   XKB_कुञ्चिका_No_चिह्नम्) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            कुञ्चिका->groups[group].levels[level].अङ्कः_syms = 0;
                            कुञ्चिका->groups[group].levels[level].अङ्कः_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                जइ (कुञ्चिका->groups[group].implicit_actions)
                    कुञ्चिका->implicit_actions = true;
            }
        }

        acts_गणनम्_iter++;
        xcb_xkb_कुञ्चिका_sym_map_next(&sym_maps_iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_vmods(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.अङ्कः_mods =
        अङ्कः_REAL_MODS + MIN(msb_pos(reply->virtualMods), अङ्कः_VMODS);
    FAIL_UNLESS(keymap->mods.अङ्कः_mods <= XKB_MAX_MODS);

    static_assert(अङ्कः_REAL_MODS + अङ्कः_VMODS <= XKB_MAX_MODS, "");
    प्रति (unsigned i = 0; i < अङ्कः_VMODS; i++) {
        जइ (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            संरचना xkb_mod *mod = &keymap->mods.mods[अङ्कः_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_explicits(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    प्रति (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        संरचना xkb_कुञ्चिका *कुञ्चिका;

        FAIL_UNLESS(wire->keycode >= keymap->min_कुञ्चिका_code &&
                    wire->keycode <= keymap->max_कुञ्चिका_code);

        कुञ्चिका = &keymap->कुञ्चिकाः[wire->keycode];

        जइ ((wire->explicit & XCB_XKB_EXPLICIT_कुञ्चिका_TYPE_1) &&
            कुञ्चिका->अङ्कः_groups > 0) {
            कुञ्चिका->groups[0].explicit_type = true;
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
        }
        जइ ((wire->explicit & XCB_XKB_EXPLICIT_कुञ्चिका_TYPE_2) &&
            कुञ्चिका->अङ्कः_groups > 1) {
            कुञ्चिका->groups[1].explicit_type = true;
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
        }
        जइ ((wire->explicit & XCB_XKB_EXPLICIT_कुञ्चिका_TYPE_3) &&
            कुञ्चिका->अङ्कः_groups > 2) {
            कुञ्चिका->groups[2].explicit_type = true;
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
        }
        जइ ((wire->explicit & XCB_XKB_EXPLICIT_कुञ्चिका_TYPE_4) &&
            कुञ्चिका->अङ्कः_groups > 3) {
            कुञ्चिका->groups[3].explicit_type = true;
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
        }
        जइ (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            कुञ्चिका->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            प्रति (xkb_layout_index_t l = 0; l < कुञ्चिका->अङ्कः_groups; l++) {
                कुञ्चिका->groups[l].explicit_actions = true;
            }
        }
        जइ (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            कुञ्चिका->explicit |= EXPLICIT_REPEAT;
        जइ (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            कुञ्चिका->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_modmaps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_कुञ्चिका_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    प्रति (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_कुञ्चिका_mod_map_t *wire = iter.data;
        संरचना xkb_कुञ्चिका *कुञ्चिका;

        FAIL_UNLESS(wire->keycode >= keymap->min_कुञ्चिका_code &&
                    wire->keycode <= keymap->max_कुञ्चिका_code);

        कुञ्चिका = &keymap->कुञ्चिकाः[wire->keycode];
        कुञ्चिका->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .कुञ्चिका_Code = कुञ्चिका->keycode,
            .mods = कुञ्चिका->modmap,
            .have_चिह्नम् = false,
            .u.कुञ्चिका_Name = कुञ्चिका->name
        });

        xcb_xkb_कुञ्चिका_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->अङ्कः_modmaps);
    प्रत्यागम true;

fail:
    darray_free(modmaps);
    प्रत्यागम false;
}

स्थिर bool
get_vmodmaps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_कुञ्चिका_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    प्रति (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_कुञ्चिका_v_mod_map_t *wire = iter.data;
        संरचना xkb_कुञ्चिका *कुञ्चिका;

        FAIL_UNLESS(wire->keycode >= keymap->min_कुञ्चिका_code &&
                    wire->keycode <= keymap->max_कुञ्चिका_code);

        कुञ्चिका = &keymap->कुञ्चिकाः[wire->keycode];
        कुञ्चिका->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_कुञ्चिका_v_mod_map_next(&iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, रिक्त);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    जइ ((reply->present & get_map_required_components) != get_map_required_components)
        गच्छ fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_कुञ्चिका_Syms,
                               reply->n_कुञ्चिका_Actions,
                               reply->totalActions,
                               reply->total_कुञ्चिका_Behaviors,
                               reply->virtualMods,
                               reply->total_कुञ्चिका_Explicit,
                               reply->total_Mod_Map_कुञ्चिकाः,
                               reply->total_VMod_Map_कुञ्चिकाः,
                               reply->present,
                               &map);

    जइ (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        गच्छ fail;

    मुक्त(reply);
    प्रत्यागम true;

fail:
    मुक्त(reply);
    प्रत्यागम false;
}

स्थिर bool
get_indicators(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->अङ्कः_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->अङ्कः_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == अङ्कः_INDICATORS, "");
    प्रति (unsigned i = 0; i < अङ्कः_INDICATORS; i++) {
        जइ (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            संरचना xkb_led *led = &keymap->leds[i];

            जइ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            जइ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            जइ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            जइ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            जइ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            जइ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            जइ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            जइ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            जइ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            जइ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_indicator_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, रिक्त);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    जइ (!get_indicators(keymap, conn, reply))
        गच्छ fail;

    मुक्त(reply);
    प्रत्यागम true;

fail:
    मुक्त(reply);
    प्रत्यागम false;
}

स्थिर bool
get_sym_interprets(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    पूर्णाङ्क length = xcb_xkb_get_compat_map_si_rtrn_length(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->nTotalSI);

    keymap->अङ्कः_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->अङ्कः_sym_interprets);

    ध्रुव xkb_keycode_t min_कुञ्चिका_code = keymap->min_कुञ्चिका_code;
    ध्रुव xkb_keycode_t max_कुञ्चिका_code = keymap->max_कुञ्चिका_code;

    प्रति (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        संरचना xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        विकल्प (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        पक्ष XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            भङ्ग;
        पक्ष XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            भङ्ग;
        पक्ष XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            भङ्ग;
        पक्ष XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            भङ्ग;
        पक्ष XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            भङ्ग;
        स्वभाव:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            गच्छ fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        जइ (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        अण्णहा
            sym_interpret->virtual_mod = अङ्कः_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_कुञ्चिका_code, max_कुञ्चिका_code));
        sym_interpret->अङ्कः_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_compat_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, रिक्त);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    जइ (!get_sym_interprets(keymap, conn, reply))
        गच्छ fail;

    मुक्त(reply);
    प्रत्यागम true;

fail:
    मुक्त(reply);
    प्रत्यागम false;
}

स्थिर bool
get_type_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_मानम्_list_t *list)
{
    पूर्णाङ्क कुञ्चिका_type_names_length =
        xcb_xkb_get_names_मानम्_list_type_names_length(reply, list);
    xcb_atom_t *कुञ्चिका_type_names_iter =
        xcb_xkb_get_names_मानम्_list_type_names(list);
    पूर्णाङ्क n_levels_per_type_length =
        xcb_xkb_get_names_मानम्_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_मानम्_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_मानम्_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->अङ्कः_types);
    FAIL_UNLESS(कुञ्चिका_type_names_length == n_levels_per_type_length);

    प्रति (पूर्णाङ्क i = 0; i < कुञ्चिका_type_names_length; i++) {
        xcb_atom_t wire_type_name = *कुञ्चिका_type_names_iter;
        uint8_t wire_अङ्कः_levels = *n_levels_per_type_iter;
        संरचना xkb_कुञ्चिका_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->अङ्कः_levels >= wire_अङ्कः_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->अङ्कः_levels);
        type->अङ्कः_level_names = type->अङ्कः_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        प्रति (size_t j = 0; j < wire_अङ्कः_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        प्रति (size_t j = wire_अङ्कः_levels; j < type->अङ्कः_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_अङ्कः_levels;
        कुञ्चिका_type_names_iter++;
        n_levels_per_type_iter++;
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_indicator_names(संरचना xkb_keymap *keymap,
                    संरचना x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_मानम्_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_मानम्_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->अङ्कः_leds);

    प्रति (unsigned i = 0; i < अङ्कः_INDICATORS; i++) {
        जइ (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            संरचना xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_vmod_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_मानम्_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_मानम्_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.अङ्कः_mods =
        अङ्कः_REAL_MODS + MIN(msb_pos(reply->virtualMods), अङ्कः_VMODS);
    FAIL_UNLESS(keymap->mods.अङ्कः_mods <= XKB_MAX_MODS);

    static_assert(अङ्कः_REAL_MODS + अङ्कः_VMODS <= XKB_MAX_MODS, "");
    प्रति (unsigned i = 0; i < अङ्कः_VMODS; i++) {
        जइ (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            संरचना xkb_mod *mod = &keymap->mods.mods[अङ्कः_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_group_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_मानम्_list_t *list)
{
    पूर्णाङ्क length = xcb_xkb_get_names_मानम्_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_मानम्_list_groups(list);

    keymap->अङ्कः_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->अङ्कः_group_names);

    प्रति (पूर्णाङ्क i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_कुञ्चिका_names(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_मानम्_list_t *list)
{
    पूर्णाङ्क length = xcb_xkb_get_names_मानम्_list_कुञ्चिका_names_length(reply, list);
    xcb_xkb_कुञ्चिका_name_iterator_t iter =
        xcb_xkb_get_names_मानम्_list_कुञ्चिका_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_कुञ्चिका_Code == keymap->min_कुञ्चिका_code);
    FAIL_UNLESS(reply->max_कुञ्चिका_Code == keymap->max_कुञ्चिका_code);
    FAIL_UNLESS(reply->first_कुञ्चिका == keymap->min_कुञ्चिका_code);
    FAIL_UNLESS(reply->first_कुञ्चिका + reply->n_कुञ्चिकाः - 1U == keymap->max_कुञ्चिका_code);

    प्रति (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_कुञ्चिका_name_t *wire = iter.data;
        xkb_atom_t *कुञ्चिका_name = &keymap->कुञ्चिकाः[reply->first_कुञ्चिका + i].name;

        जइ (wire->name[0] == '\0') {
            *कुञ्चिका_name = XKB_ATOM_NONE;
        }
        अण्णहा {
            *कुञ्चिका_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_कुञ्चिका_NAME_LENGTH));
            जइ (!*कुञ्चिका_name)
                प्रत्यागम false;
        }

        xcb_xkb_कुञ्चिका_name_next(&iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_aliases(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_मानम्_list_t *list)
{
    पूर्णाङ्क length = xcb_xkb_get_names_मानम्_list_कुञ्चिका_aliases_length(reply, list);
    xcb_xkb_कुञ्चिका_alias_iterator_t iter =
        xcb_xkb_get_names_मानम्_list_कुञ्चिका_aliases_iterator(reply, list);

    keymap->अङ्कः_कुञ्चिका_aliases = reply->n_कुञ्चिका_Aliases;
    ALLOC_OR_FAIL(keymap->कुञ्चिका_aliases, keymap->अङ्कः_कुञ्चिका_aliases);

    प्रति (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_कुञ्चिका_alias_t *wire = iter.data;
        संरचना xkb_कुञ्चिका_alias *alias = &keymap->कुञ्चिका_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_कुञ्चिका_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_कुञ्चिका_NAME_LENGTH));
        जइ (!alias->real || !alias->alias)
            गच्छ fail;

        xcb_xkb_कुञ्चिका_alias_next(&iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, रिक्त);
    xcb_xkb_get_names_मानम्_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_मानम्_list_unpack(xcb_xkb_get_names_मानम्_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_कुञ्चिकाः,
                                        reply->n_कुञ्चिका_Aliases,
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
    जइ (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_कुञ्चिका_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        गच्छ fail;

    मुक्त(reply);
    प्रत्यागम true;

fail:
    मुक्त(reply);
    प्रत्यागम false;
}

स्थिर bool
get_controls(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, रिक्त);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->अङ्कः_Groups > 0 && reply->अङ्कः_Groups <= 4);

    keymap->अङ्कः_groups = reply->अङ्कः_Groups;

    FAIL_UNLESS(keymap->max_कुञ्चिका_code < XCB_XKB_CONST_PER_कुञ्चिका_BIT_ARRAY_SIZE * 8);

    प्रति (xkb_keycode_t i = keymap->min_कुञ्चिका_code; i <= keymap->max_कुञ्चिका_code; i++)
        keymap->कुञ्चिकाः[i].repeats = (reply->per_कुञ्चिका_Repeat[i / 8] & (1u << (i % 8)));

    मुक्त(reply);
    प्रत्यागम true;

fail:
    मुक्त(reply);
    प्रत्यागम false;
}

संरचना xkb_keymap *
xkb_x11_keymap_new_from_device(संरचना xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               गणना xkb_keymap_compile_flags flags)
{
    जइ (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        प्रत्यागम रिक्त;
    }

    ध्रुव गणना xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    संरचना xkb_keymap * ध्रुव keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    जइ (!keymap)
        प्रत्यागम रिक्त;

    keymap->redirect_कुञ्चिका_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    संरचना x11_atom_interner interner;
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

    जइ (!get_map(keymap, conn, map_cookie))
        गच्छ err_map;
    जइ (!get_indicator_map(keymap, conn, indicator_map_cookie))
        गच्छ err_indicator_map;
    जइ (!get_compat_map(keymap, conn, compat_map_cookie))
        गच्छ err_compat_map;
    जइ (!get_names(keymap, &interner, names_cookie))
        गच्छ err_names;
    जइ (!get_controls(keymap, conn, controls_cookie))
        गच्छ err_controls;
    x11_atom_interner_round_trip(&interner);
    जइ (interner.had_error)
        गच्छ err_interner;

    प्रत्यागम keymap;

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
    प्रत्यागम रिक्त;
}
