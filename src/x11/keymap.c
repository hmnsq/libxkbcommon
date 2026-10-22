/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <xcb/xkb.h>

#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-keysyms.h"
#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "x11-priv.h"

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
#define இலக்கம்_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define இலக்கம்_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define இலக்கம்_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(கோவை) செய் {                              \
    எனில் (!(கோவை)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(கோவை));                 \
        செல் fail;                                          \
    }                                                       \
} போது (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) செய் {         \
    எனில் (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        செல் fail;                                          \
    }                                                       \
} போது (0)

#define ALLOC_OR_FAIL(arr, nmemb) செய் {                      \
    எனில் ((nmemb) > 0) {                                      \
        (arr) = calloc((nmemb), அளவு(*(arr)));            \
        எனில் (!(arr))                                         \
            செல் fail;                                      \
    }                                                       \
} போது (0)

நிலையான மாறிலி xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_சாவி_TYPES |
     XCB_XKB_MAP_PART_சாவி_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_சாவி_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

நிலையான மாறிலி xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_சாவி_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_சாவி_NAMES |
     XCB_XKB_NAME_DETAIL_சாவி_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
நிலையான மாறிலி xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_சாவி_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_சாவி_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


நிலையான xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_கீழ், uint16_t vmods_உயர்வு)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    திரும்பு
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_கீழ் << 8) |
        ((xkb_mod_mask_t) vmods_உயர்வு << 16);
}

எண்ணிக்கை xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    எண்ணிக்கை xkb_action_controls ret = 0;
    எனில் (wire & XCB_XKB_BOOL_CTRL_REPEAT_சாவிகள்)
        ret |= CONTROL_REPEAT;
    எனில் (wire & XCB_XKB_BOOL_CTRL_SLOW_சாவிகள்)
        ret |= CONTROL_SLOW;
    எனில் (wire & XCB_XKB_BOOL_CTRL_BOUNCE_சாவிகள்)
        ret |= CONTROL_DEBOUNCE;
    எனில் (wire & XCB_XKB_BOOL_CTRL_STICKY_சாவிகள்)
        ret |= CONTROL_STICKY_சாவிகள்;
    எனில் (wire & XCB_XKB_BOOL_CTRL_MOUSE_சாவிகள்)
        ret |= CONTROL_MOUSE_சாவிகள்;
    எனில் (wire & XCB_XKB_BOOL_CTRL_MOUSE_சாவிகள்_ACCEL)
        ret |= CONTROL_MOUSE_சாவிகள்_ACCEL;
    எனில் (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_சாவிகள்)
        ret |= CONTROL_AX;
    எனில் (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    எனில் (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    எனில் (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    எனில் (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    எனில் (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    எனில் (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    திரும்பு ret;
}

நிலையான bool
translate_action(ஒன்றிப்பு xkb_action *action, மாறிலி xcb_xkb_action_t *wire,
                 xkb_keycode_t min_சாவி_code, xkb_keycode_t max_சாவி_code)
{
    தேர்வு (wire->type) {
    நிலை XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_கீழ்_2,
                                                wire->setmods.vmods_உயர்வு_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        எனில் (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        எனில் (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        எனில் (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_கீழ்_2,
                                                wire->latchmods.vmods_உயர்வு_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        எனில் (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        எனில் (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        எனில் (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_கீழ்_2,
                                                wire->lockmods.vmods_உயர்வு_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        எனில் (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        எனில் (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        எனில் (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        எனில் (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        எனில் (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        எனில் (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        எனில் (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        எனில் (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        எனில் (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        எனில் (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_கீழ் | ((uint16_t) wire->moveptr.x_உயர்வு << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_கீழ் | ((uint16_t) wire->moveptr.y_உயர்வு << 8));

        எனில் (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        எனில் (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        எனில் (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.மொத்தம் = wire->ptrbtn.மொத்தம்;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.மொத்தம் = 0;

        எனில் (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        எனில் (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.மதிப்பு = wire->setptrdflt.மதிப்பு;

        எனில் (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        எனில் (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        எனில் (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            மாறிலி uint16_t mask = (wire->setcontrols.bool_Ctrls_கீழ் |
                                   (wire->setcontrols.bool_Ctrls_உயர்வு << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        நிறுத்து;
    நிலை XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            மாறிலி uint16_t mask = (wire->lockcontrols.bool_Ctrls_கீழ் |
                                   (wire->lockcontrols.bool_Ctrls_உயர்வு << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        நிறுத்து;

    நிலை XCB_XKB_SA_TYPE_REDIRECT_சாவி:
        action->type = ACTION_TYPE_REDIRECT_சாவி;
        எனில் (wire->redirect.newkey < min_சாவி_code ||
            wire->redirect.newkey > max_சாவி_code)
                திரும்பு false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_கீழ்,
            wire->redirect.vmods_Mask_உயர்வு
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_கீழ்_2,
                                               wire->redirect.vmods_உயர்வு_2);
        நிறுத்து;

    நிலை XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        நிறுத்து;

    /* We don't support these. */
    நிலை XCB_XKB_SA_TYPE_ISO_LOCK:
    நிலை XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    நிலை XCB_XKB_SA_TYPE_DEVICE_BTN:
    நிலை XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    நிலை XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        நிறுத்து;

    இயல்பு:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_இலக்கம்_ENTRIES,
                      "Missing action type");

        எனில் (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            நிறுத்து;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(அளவு(action->priv.data) == 7 &&
                      அளவு(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        memcpy(action->priv.data, wire->noaction.pad0, 7);
        நிறுத்து;
    }
    திரும்பு true;
}

நிலையான bool
get_types(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    முழு types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_சாவி_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->இலக்கம்_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->இலக்கம்_types);
    ப்ரதிஜ்ஞா(types_length == (முழு) keymap->இலக்கம்_types);

    க்கு (முழு i = 0; i < types_length; i++) {
        xcb_xkb_சாவி_type_t *wire_type = types_iter.data;
        கட்டமைப்பு xkb_சாவி_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->இலக்கம்_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->இலக்கம்_levels = wire_type->இலக்கம்_Levels;

        {
            முழு entries_length = xcb_xkb_சாவி_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_சாவி_type_map_iterator(wire_type);

            type->இலக்கம்_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->இலக்கம்_entries);
            ப்ரதிஜ்ஞா(entries_length == (முழு) type->இலக்கம்_entries);

            க்கு (முழு j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                கட்டமைப்பு xkb_சாவி_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->இலக்கம்_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            முழு preserves_length = xcb_xkb_சாவி_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_சாவி_type_preserve_iterator(wire_type);

            FAIL_UNLESS((unsigned) preserves_length <= type->இலக்கம்_entries);

            க்கு (முழு j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                கட்டமைப்பு xkb_சாவி_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_சாவி_type_next(&types_iter);
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_sym_maps(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    முழு sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_சாவி_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_சாவி_Code <= reply->max_சாவி_Code);
    FAIL_UNLESS(reply->first_சாவி_Sym >= reply->min_சாவி_Code);
    FAIL_UNLESS(reply->first_சாவி_Sym + reply->n_சாவி_Syms <= reply->max_சாவி_Code + 1);

    keymap->min_சாவி_code = reply->min_சாவி_Code;
    keymap->max_சாவி_code = reply->max_சாவி_Code;
    keymap->இலக்கம்_சாவிகள் = keymap->இலக்கம்_சாவிகள்_கீழ் = reply->max_சாவி_Code + 1;

    ALLOC_OR_FAIL(keymap->சாவிகள், keymap->max_சாவி_code + 1);

    க்கு (xkb_keycode_t kc = keymap->min_சாவி_code; kc <= keymap->max_சாவி_code; kc++)
        keymap->சாவிகள்[kc].keycode = kc;

    க்கு (முழு i = 0; i < sym_maps_length; i++) {
        xcb_xkb_சாவி_sym_map_t *wire_sym_map = sym_maps_iter.data;
        கட்டமைப்பு xkb_சாவி *சாவி = &keymap->சாவிகள்[reply->first_சாவி_Sym + i];

        சாவி->இலக்கம்_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(சாவி->இலக்கம்_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(சாவி->groups, சாவி->இலக்கம்_groups);

        க்கு (xkb_layout_index_t j = 0; j < சாவி->இலக்கம்_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->இலக்கம்_types);
            சாவி->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(சாவி->groups[j].levels, சாவி->groups[j].type->இலக்கம்_levels);
        }

        சாவி->out_of_range_group_எண் = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(சாவி->out_of_range_group_எண் <= சாவி->இலக்கம்_groups);

        எனில் (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            சாவி->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        இல்லையெனில் எனில் (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            சாவி->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        இல்லையெனில்
            சாவி->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            முழு syms_length = xcb_xkb_சாவி_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_சாவி_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * சாவி->இலக்கம்_groups);

            எனில் (syms_length > 0)
                சாவி->explicit |= EXPLICIT_SYMBOLS;

            க்கு (xkb_layout_index_t group = 0; group < சாவி->இலக்கம்_groups; group++) {
                க்கு (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    ப்ரதிஜ்ஞா(சாவி->groups[group].type != வெற்று);
                    எனில் (level < சாவி->groups[group].type->இலக்கம்_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        சாவி->groups[group].levels[level].இலக்கம்_syms = 1;
                        சாவி->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        சாவி->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        எனில் (wire_keysym != XKB_சாவி_No_குறியீடு)
                            சாவி->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_சாவி_sym_map_next(&sym_maps_iter);
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_actions(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    முழு acts_மொத்தம்_length =
        xcb_xkb_get_map_map_acts_rtrn_மொத்தம்_length(reply, map);
    uint8_t *acts_மொத்தம்_iter = xcb_xkb_get_map_map_acts_rtrn_மொத்தம்(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_சாவி_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_சாவி_Action == keymap->min_சாவி_code);
    FAIL_UNLESS(reply->first_சாவி_Action + reply->n_சாவி_Actions ==
                keymap->max_சாவி_code + 1);

    க்கு (முழு i = 0; i < acts_மொத்தம்_length; i++) {
        xcb_xkb_சாவி_sym_map_t *wire_sym_map = sym_maps_iter.data;
        முழு syms_length = xcb_xkb_சாவி_sym_map_syms_length(wire_sym_map);
        uint8_t wire_மொத்தம் = *acts_மொத்தம்_iter;
        கட்டமைப்பு xkb_சாவி *சாவி = &keymap->சாவிகள்[reply->first_சாவி_Action + i];

        FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * சாவி->இலக்கம்_groups);
        FAIL_UNLESS(wire_மொத்தம் == 0 || wire_மொத்தம் == syms_length);

        மாறிலி xkb_keycode_t min_சாவி_code = keymap->min_சாவி_code;
        மாறிலி xkb_keycode_t max_சாவி_code = keymap->max_சாவி_code;

        எனில் (wire_மொத்தம் != 0) {
            க்கு (xkb_layout_index_t group = 0; group < சாவி->இலக்கம்_groups; group++) {
                க்கு (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    எனில் (level < சாவி->groups[group].type->இலக்கம்_levels) {
                        சாவி->groups[group].levels[level].இலக்கம்_actions = 1;
                        ஒன்றிப்பு xkb_action *action =
                            &சாவி->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_சாவி_code, max_சாவி_code));

                        எனில் (action->type != ACTION_TYPE_NONE) {
                            சாவி->groups[group].implicit_actions =
                                !(சாவி->explicit & EXPLICIT_INTERP);
                        } இல்லையெனில் எனில் (சாவி->groups[group].levels[level].s.sym ==
                                   XKB_சாவி_No_குறியீடு) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            சாவி->groups[group].levels[level].இலக்கம்_syms = 0;
                            சாவி->groups[group].levels[level].இலக்கம்_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                எனில் (சாவி->groups[group].implicit_actions)
                    சாவி->implicit_actions = true;
            }
        }

        acts_மொத்தம்_iter++;
        xcb_xkb_சாவி_sym_map_next(&sym_maps_iter);
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_vmods(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.இலக்கம்_mods =
        இலக்கம்_REAL_MODS + MIN(msb_pos(reply->virtualMods), இலக்கம்_VMODS);
    FAIL_UNLESS(keymap->mods.இலக்கம்_mods <= XKB_MAX_MODS);

    static_assert(இலக்கம்_REAL_MODS + இலக்கம்_VMODS <= XKB_MAX_MODS, "");
    க்கு (unsigned i = 0; i < இலக்கம்_VMODS; i++) {
        எனில் (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            கட்டமைப்பு xkb_mod *mod = &keymap->mods.mods[இலக்கம்_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_explicits(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    முழு length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    க்கு (முழு i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        கட்டமைப்பு xkb_சாவி *சாவி;

        FAIL_UNLESS(wire->keycode >= keymap->min_சாவி_code &&
                    wire->keycode <= keymap->max_சாவி_code);

        சாவி = &keymap->சாவிகள்[wire->keycode];

        எனில் ((wire->explicit & XCB_XKB_EXPLICIT_சாவி_TYPE_1) &&
            சாவி->இலக்கம்_groups > 0) {
            சாவி->groups[0].explicit_type = true;
            சாவி->explicit |= EXPLICIT_TYPES;
        }
        எனில் ((wire->explicit & XCB_XKB_EXPLICIT_சாவி_TYPE_2) &&
            சாவி->இலக்கம்_groups > 1) {
            சாவி->groups[1].explicit_type = true;
            சாவி->explicit |= EXPLICIT_TYPES;
        }
        எனில் ((wire->explicit & XCB_XKB_EXPLICIT_சாவி_TYPE_3) &&
            சாவி->இலக்கம்_groups > 2) {
            சாவி->groups[2].explicit_type = true;
            சாவி->explicit |= EXPLICIT_TYPES;
        }
        எனில் ((wire->explicit & XCB_XKB_EXPLICIT_சாவி_TYPE_4) &&
            சாவி->இலக்கம்_groups > 3) {
            சாவி->groups[3].explicit_type = true;
            சாவி->explicit |= EXPLICIT_TYPES;
        }
        எனில் (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            சாவி->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            க்கு (xkb_layout_index_t l = 0; l < சாவி->இலக்கம்_groups; l++) {
                சாவி->groups[l].explicit_actions = true;
            }
        }
        எனில் (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            சாவி->explicit |= EXPLICIT_REPEAT;
        எனில் (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            சாவி->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_modmaps(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    முழு length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_சாவி_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    க்கு (முழு i = 0; i < length; i++) {
        xcb_xkb_சாவி_mod_map_t *wire = iter.data;
        கட்டமைப்பு xkb_சாவி *சாவி;

        FAIL_UNLESS(wire->keycode >= keymap->min_சாவி_code &&
                    wire->keycode <= keymap->max_சாவி_code);

        சாவி = &keymap->சாவிகள்[wire->keycode];
        சாவி->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .சாவி_Code = சாவி->keycode,
            .mods = சாவி->modmap,
            .have_குறியீடு = false,
            .u.சாவி_Name = சாவி->name
        });

        xcb_xkb_சாவி_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->இலக்கம்_modmaps);
    திரும்பு true;

fail:
    darray_free(modmaps);
    திரும்பு false;
}

நிலையான bool
get_vmodmaps(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    முழு length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_சாவி_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    க்கு (முழு i = 0; i < length; i++) {
        xcb_xkb_சாவி_v_mod_map_t *wire = iter.data;
        கட்டமைப்பு xkb_சாவி *சாவி;

        FAIL_UNLESS(wire->keycode >= keymap->min_சாவி_code &&
                    wire->keycode <= keymap->max_சாவி_code);

        சாவி = &keymap->சாவிகள்[wire->keycode];
        சாவி->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_சாவி_v_mod_map_next(&iter);
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_map(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, வெற்று);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    எனில் ((reply->present & get_map_required_components) != get_map_required_components)
        செல் fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_சாவி_Syms,
                               reply->n_சாவி_Actions,
                               reply->totalActions,
                               reply->total_சாவி_Behaviors,
                               reply->virtualMods,
                               reply->total_சாவி_Explicit,
                               reply->total_Mod_Map_சாவிகள்,
                               reply->total_VMod_Map_சாவிகள்,
                               reply->present,
                               &map);

    எனில் (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        செல் fail;

    விடுவி(reply);
    திரும்பு true;

fail:
    விடுவி(reply);
    திரும்பு false;
}

நிலையான bool
get_indicators(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->இலக்கம்_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->இலக்கம்_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == இலக்கம்_INDICATORS, "");
    க்கு (unsigned i = 0; i < இலக்கம்_INDICATORS; i++) {
        எனில் (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            கட்டமைப்பு xkb_led *led = &keymap->leds[i];

            எனில் (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            எனில் (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            எனில் (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            எனில் (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            எனில் (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            எனில் (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            எனில் (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            எனில் (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            எனில் (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            எனில் (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_indicator_map(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, வெற்று);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    எனில் (!get_indicators(keymap, conn, reply))
        செல் fail;

    விடுவி(reply);
    திரும்பு true;

fail:
    விடுவி(reply);
    திரும்பு false;
}

நிலையான bool
get_sym_interprets(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    முழு length = xcb_xkb_get_compat_map_si_rtrn_length(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->nTotalSI);

    keymap->இலக்கம்_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->இலக்கம்_sym_interprets);

    மாறிலி xkb_keycode_t min_சாவி_code = keymap->min_சாவி_code;
    மாறிலி xkb_keycode_t max_சாவி_code = keymap->max_சாவி_code;

    க்கு (முழு i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        கட்டமைப்பு xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        தேர்வு (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        நிலை XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            நிறுத்து;
        நிலை XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            நிறுத்து;
        நிலை XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            நிறுத்து;
        நிலை XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            நிறுத்து;
        நிலை XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            நிறுத்து;
        இயல்பு:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            செல் fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        எனில் (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        இல்லையெனில்
            sym_interpret->virtual_mod = இலக்கம்_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_சாவி_code, max_சாவி_code));
        sym_interpret->இலக்கம்_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_compat_map(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, வெற்று);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    எனில் (!get_sym_interprets(keymap, conn, reply))
        செல் fail;

    விடுவி(reply);
    திரும்பு true;

fail:
    விடுவி(reply);
    திரும்பு false;
}

நிலையான bool
get_type_names(கட்டமைப்பு xkb_keymap *keymap, கட்டமைப்பு x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_மதிப்பு_list_t *list)
{
    முழு சாவி_type_names_length =
        xcb_xkb_get_names_மதிப்பு_list_type_names_length(reply, list);
    xcb_atom_t *சாவி_type_names_iter =
        xcb_xkb_get_names_மதிப்பு_list_type_names(list);
    முழு n_levels_per_type_length =
        xcb_xkb_get_names_மதிப்பு_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_மதிப்பு_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_மதிப்பு_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->இலக்கம்_types);
    FAIL_UNLESS(சாவி_type_names_length == n_levels_per_type_length);

    க்கு (முழு i = 0; i < சாவி_type_names_length; i++) {
        xcb_atom_t wire_type_name = *சாவி_type_names_iter;
        uint8_t wire_இலக்கம்_levels = *n_levels_per_type_iter;
        கட்டமைப்பு xkb_சாவி_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->இலக்கம்_levels >= wire_இலக்கம்_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->இலக்கம்_levels);
        type->இலக்கம்_level_names = type->இலக்கம்_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        க்கு (size_t j = 0; j < wire_இலக்கம்_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        க்கு (size_t j = wire_இலக்கம்_levels; j < type->இலக்கம்_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_இலக்கம்_levels;
        சாவி_type_names_iter++;
        n_levels_per_type_iter++;
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_indicator_names(கட்டமைப்பு xkb_keymap *keymap,
                    கட்டமைப்பு x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_மதிப்பு_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_மதிப்பு_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->இலக்கம்_leds);

    க்கு (unsigned i = 0; i < இலக்கம்_INDICATORS; i++) {
        எனில் (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            கட்டமைப்பு xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_vmod_names(கட்டமைப்பு xkb_keymap *keymap, கட்டமைப்பு x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_மதிப்பு_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_மதிப்பு_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.இலக்கம்_mods =
        இலக்கம்_REAL_MODS + MIN(msb_pos(reply->virtualMods), இலக்கம்_VMODS);
    FAIL_UNLESS(keymap->mods.இலக்கம்_mods <= XKB_MAX_MODS);

    static_assert(இலக்கம்_REAL_MODS + இலக்கம்_VMODS <= XKB_MAX_MODS, "");
    க்கு (unsigned i = 0; i < இலக்கம்_VMODS; i++) {
        எனில் (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            கட்டமைப்பு xkb_mod *mod = &keymap->mods.mods[இலக்கம்_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_group_names(கட்டமைப்பு xkb_keymap *keymap, கட்டமைப்பு x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_மதிப்பு_list_t *list)
{
    முழு length = xcb_xkb_get_names_மதிப்பு_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_மதிப்பு_list_groups(list);

    keymap->இலக்கம்_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->இலக்கம்_group_names);

    க்கு (முழு i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_சாவி_names(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_மதிப்பு_list_t *list)
{
    முழு length = xcb_xkb_get_names_மதிப்பு_list_சாவி_names_length(reply, list);
    xcb_xkb_சாவி_name_iterator_t iter =
        xcb_xkb_get_names_மதிப்பு_list_சாவி_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_சாவி_Code == keymap->min_சாவி_code);
    FAIL_UNLESS(reply->max_சாவி_Code == keymap->max_சாவி_code);
    FAIL_UNLESS(reply->first_சாவி == keymap->min_சாவி_code);
    FAIL_UNLESS(reply->first_சாவி + reply->n_சாவிகள் - 1U == keymap->max_சாவி_code);

    க்கு (முழு i = 0; i < length; i++) {
        xcb_xkb_சாவி_name_t *wire = iter.data;
        xkb_atom_t *சாவி_name = &keymap->சாவிகள்[reply->first_சாவி + i].name;

        எனில் (wire->name[0] == '\0') {
            *சாவி_name = XKB_ATOM_NONE;
        }
        இல்லையெனில் {
            *சாவி_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_சாவி_NAME_LENGTH));
            எனில் (!*சாவி_name)
                திரும்பு false;
        }

        xcb_xkb_சாவி_name_next(&iter);
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_aliases(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_மதிப்பு_list_t *list)
{
    முழு length = xcb_xkb_get_names_மதிப்பு_list_சாவி_aliases_length(reply, list);
    xcb_xkb_சாவி_alias_iterator_t iter =
        xcb_xkb_get_names_மதிப்பு_list_சாவி_aliases_iterator(reply, list);

    keymap->இலக்கம்_சாவி_aliases = reply->n_சாவி_Aliases;
    ALLOC_OR_FAIL(keymap->சாவி_aliases, keymap->இலக்கம்_சாவி_aliases);

    க்கு (முழு i = 0; i < length; i++) {
        xcb_xkb_சாவி_alias_t *wire = iter.data;
        கட்டமைப்பு xkb_சாவி_alias *alias = &keymap->சாவி_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_சாவி_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_சாவி_NAME_LENGTH));
        எனில் (!alias->real || !alias->alias)
            செல் fail;

        xcb_xkb_சாவி_alias_next(&iter);
    }

    திரும்பு true;

fail:
    திரும்பு false;
}

நிலையான bool
get_names(கட்டமைப்பு xkb_keymap *keymap, கட்டமைப்பு x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, வெற்று);
    xcb_xkb_get_names_மதிப்பு_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_மதிப்பு_list_unpack(xcb_xkb_get_names_மதிப்பு_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_சாவிகள்,
                                        reply->n_சாவி_Aliases,
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
    எனில் (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_சாவி_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        செல் fail;

    விடுவி(reply);
    திரும்பு true;

fail:
    விடுவி(reply);
    திரும்பு false;
}

நிலையான bool
get_controls(கட்டமைப்பு xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, வெற்று);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->இலக்கம்_Groups > 0 && reply->இலக்கம்_Groups <= 4);

    keymap->இலக்கம்_groups = reply->இலக்கம்_Groups;

    FAIL_UNLESS(keymap->max_சாவி_code < XCB_XKB_CONST_PER_சாவி_BIT_ARRAY_SIZE * 8);

    க்கு (xkb_keycode_t i = keymap->min_சாவி_code; i <= keymap->max_சாவி_code; i++)
        keymap->சாவிகள்[i].repeats = (reply->per_சாவி_Repeat[i / 8] & (1u << (i % 8)));

    விடுவி(reply);
    திரும்பு true;

fail:
    விடுவி(reply);
    திரும்பு false;
}

கட்டமைப்பு xkb_keymap *
xkb_x11_keymap_new_from_device(கட்டமைப்பு xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               எண்ணிக்கை xkb_keymap_compile_flags flags)
{
    எனில் (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        திரும்பு வெற்று;
    }

    மாறிலி எண்ணிக்கை xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    கட்டமைப்பு xkb_keymap * மாறிலி keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    எனில் (!keymap)
        திரும்பு வெற்று;

    keymap->redirect_சாவி_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    கட்டமைப்பு x11_atom_interner interner;
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

    எனில் (!get_map(keymap, conn, map_cookie))
        செல் err_map;
    எனில் (!get_indicator_map(keymap, conn, indicator_map_cookie))
        செல் err_indicator_map;
    எனில் (!get_compat_map(keymap, conn, compat_map_cookie))
        செல் err_compat_map;
    எனில் (!get_names(keymap, &interner, names_cookie))
        செல் err_names;
    எனில் (!get_controls(keymap, conn, controls_cookie))
        செல் err_controls;
    x11_atom_interner_round_trip(&interner);
    எனில் (interner.had_error)
        செல் err_interner;

    திரும்பு keymap;

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
    திரும்பு வெற்று;
}
