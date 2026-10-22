/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <xcb/xkb.h>

#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon-keysyms.h"
#અન્તર્ભાવ "atom.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "x11-priv.h"

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
#define અંક_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define અંક_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define અંક_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(પદાવલિ) કરો {                              \
    જો (!(પદાવલિ)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(પદાવલિ));                 \
        જાઓ fail;                                          \
    }                                                       \
} જ્યારે (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) કરો {         \
    જો (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        જાઓ fail;                                          \
    }                                                       \
} જ્યારે (0)

#define ALLOC_OR_FAIL(arr, nmemb) કરો {                      \
    જો ((nmemb) > 0) {                                      \
        (arr) = calloc((nmemb), કદ(*(arr)));            \
        જો (!(arr))                                         \
            જાઓ fail;                                      \
    }                                                       \
} જ્યારે (0)

સ્થિર અચલ xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_ચાવી_TYPES |
     XCB_XKB_MAP_PART_ચાવી_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_ચાવી_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

સ્થિર અચલ xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_ચાવી_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_ચાવી_NAMES |
     XCB_XKB_NAME_DETAIL_ચાવી_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
સ્થિર અચલ xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_ચાવી_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_ચાવી_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


સ્થિર xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_નીચલું, uint16_t vmods_ઉપલું)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    પાછા
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_નીચલું << 8) |
        ((xkb_mod_mask_t) vmods_ઉપલું << 16);
}

ગણના xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    ગણના xkb_action_controls ret = 0;
    જો (wire & XCB_XKB_BOOL_CTRL_REPEAT_ચાવીઓ)
        ret |= CONTROL_REPEAT;
    જો (wire & XCB_XKB_BOOL_CTRL_SLOW_ચાવીઓ)
        ret |= CONTROL_SLOW;
    જો (wire & XCB_XKB_BOOL_CTRL_BOUNCE_ચાવીઓ)
        ret |= CONTROL_DEBOUNCE;
    જો (wire & XCB_XKB_BOOL_CTRL_STICKY_ચાવીઓ)
        ret |= CONTROL_STICKY_ચાવીઓ;
    જો (wire & XCB_XKB_BOOL_CTRL_MOUSE_ચાવીઓ)
        ret |= CONTROL_MOUSE_ચાવીઓ;
    જો (wire & XCB_XKB_BOOL_CTRL_MOUSE_ચાવીઓ_ACCEL)
        ret |= CONTROL_MOUSE_ચાવીઓ_ACCEL;
    જો (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_ચાવીઓ)
        ret |= CONTROL_AX;
    જો (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    જો (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    જો (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    જો (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    જો (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    જો (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    પાછા ret;
}

સ્થિર bool
translate_action(સંઘ xkb_action *action, અચલ xcb_xkb_action_t *wire,
                 xkb_keycode_t min_ચાવી_code, xkb_keycode_t max_ચાવી_code)
{
    પસંદગી (wire->type) {
    સ્થિતિ XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_નીચલું_2,
                                                wire->setmods.vmods_ઉપલું_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        જો (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        જો (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        જો (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_નીચલું_2,
                                                wire->latchmods.vmods_ઉપલું_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        જો (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        જો (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        જો (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_નીચલું_2,
                                                wire->lockmods.vmods_ઉપલું_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        જો (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        જો (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        જો (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        જો (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        જો (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        જો (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        જો (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        જો (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        જો (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        જો (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_નીચલું | ((uint16_t) wire->moveptr.x_ઉપલું << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_નીચલું | ((uint16_t) wire->moveptr.y_ઉપલું << 8));

        જો (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        જો (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        જો (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.ગણતરી = wire->ptrbtn.ગણતરી;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.ગણતરી = 0;

        જો (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        જો (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.કિંમત = wire->setptrdflt.કિંમત;

        જો (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        જો (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        જો (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            અચલ uint16_t mask = (wire->setcontrols.bool_Ctrls_નીચલું |
                                   (wire->setcontrols.bool_Ctrls_ઉપલું << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        તોડો;
    સ્થિતિ XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            અચલ uint16_t mask = (wire->lockcontrols.bool_Ctrls_નીચલું |
                                   (wire->lockcontrols.bool_Ctrls_ઉપલું << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        તોડો;

    સ્થિતિ XCB_XKB_SA_TYPE_REDIRECT_ચાવી:
        action->type = ACTION_TYPE_REDIRECT_ચાવી;
        જો (wire->redirect.newkey < min_ચાવી_code ||
            wire->redirect.newkey > max_ચાવી_code)
                પાછા false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_નીચલું,
            wire->redirect.vmods_Mask_ઉપલું
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_નીચલું_2,
                                               wire->redirect.vmods_ઉપલું_2);
        તોડો;

    સ્થિતિ XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        તોડો;

    /* We don't support these. */
    સ્થિતિ XCB_XKB_SA_TYPE_ISO_LOCK:
    સ્થિતિ XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    સ્થિતિ XCB_XKB_SA_TYPE_DEVICE_BTN:
    સ્થિતિ XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    સ્થિતિ XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        તોડો;

    મૂળભૂત:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_અંક_ENTRIES,
                      "Missing action type");

        જો (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            તોડો;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(કદ(action->priv.data) == 7 &&
                      કદ(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        memcpy(action->priv.data, wire->noaction.pad0, 7);
        તોડો;
    }
    પાછા true;
}

સ્થિર bool
get_types(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    પૂર્ણાંક types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_ચાવી_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->અંક_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->અંક_types);
    પ્રતિજ્ઞા(types_length == (પૂર્ણાંક) keymap->અંક_types);

    માટે (પૂર્ણાંક i = 0; i < types_length; i++) {
        xcb_xkb_ચાવી_type_t *wire_type = types_iter.data;
        સંરચના xkb_ચાવી_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->અંક_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->અંક_levels = wire_type->અંક_Levels;

        {
            પૂર્ણાંક entries_length = xcb_xkb_ચાવી_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_ચાવી_type_map_iterator(wire_type);

            type->અંક_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->અંક_entries);
            પ્રતિજ્ઞા(entries_length == (પૂર્ણાંક) type->અંક_entries);

            માટે (પૂર્ણાંક j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                સંરચના xkb_ચાવી_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->અંક_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            પૂર્ણાંક preserves_length = xcb_xkb_ચાવી_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_ચાવી_type_preserve_iterator(wire_type);

            FAIL_UNLESS((unsigned) preserves_length <= type->અંક_entries);

            માટે (પૂર્ણાંક j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                સંરચના xkb_ચાવી_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_ચાવી_type_next(&types_iter);
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_sym_maps(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    પૂર્ણાંક sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_ચાવી_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_ચાવી_Code <= reply->max_ચાવી_Code);
    FAIL_UNLESS(reply->first_ચાવી_Sym >= reply->min_ચાવી_Code);
    FAIL_UNLESS(reply->first_ચાવી_Sym + reply->n_ચાવી_Syms <= reply->max_ચાવી_Code + 1);

    keymap->min_ચાવી_code = reply->min_ચાવી_Code;
    keymap->max_ચાવી_code = reply->max_ચાવી_Code;
    keymap->અંક_ચાવીઓ = keymap->અંક_ચાવીઓ_નીચલું = reply->max_ચાવી_Code + 1;

    ALLOC_OR_FAIL(keymap->ચાવીઓ, keymap->max_ચાવી_code + 1);

    માટે (xkb_keycode_t kc = keymap->min_ચાવી_code; kc <= keymap->max_ચાવી_code; kc++)
        keymap->ચાવીઓ[kc].keycode = kc;

    માટે (પૂર્ણાંક i = 0; i < sym_maps_length; i++) {
        xcb_xkb_ચાવી_sym_map_t *wire_sym_map = sym_maps_iter.data;
        સંરચના xkb_ચાવી *ચાવી = &keymap->ચાવીઓ[reply->first_ચાવી_Sym + i];

        ચાવી->અંક_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(ચાવી->અંક_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(ચાવી->groups, ચાવી->અંક_groups);

        માટે (xkb_layout_index_t j = 0; j < ચાવી->અંક_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->અંક_types);
            ચાવી->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(ચાવી->groups[j].levels, ચાવી->groups[j].type->અંક_levels);
        }

        ચાવી->out_of_range_group_સંખ્યા = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(ચાવી->out_of_range_group_સંખ્યા <= ચાવી->અંક_groups);

        જો (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            ચાવી->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        નહીંતર જો (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            ચાવી->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        નહીંતર
            ચાવી->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            પૂર્ણાંક syms_length = xcb_xkb_ચાવી_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_ચાવી_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * ચાવી->અંક_groups);

            જો (syms_length > 0)
                ચાવી->explicit |= EXPLICIT_SYMBOLS;

            માટે (xkb_layout_index_t group = 0; group < ચાવી->અંક_groups; group++) {
                માટે (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    પ્રતિજ્ઞા(ચાવી->groups[group].type != ખાલી);
                    જો (level < ચાવી->groups[group].type->અંક_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        ચાવી->groups[group].levels[level].અંક_syms = 1;
                        ચાવી->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        ચાવી->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        જો (wire_keysym != XKB_ચાવી_No_પ્રતીક)
                            ચાવી->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_ચાવી_sym_map_next(&sym_maps_iter);
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_actions(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    પૂર્ણાંક acts_ગણતરી_length =
        xcb_xkb_get_map_map_acts_rtrn_ગણતરી_length(reply, map);
    uint8_t *acts_ગણતરી_iter = xcb_xkb_get_map_map_acts_rtrn_ગણતરી(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_ચાવી_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_ચાવી_Action == keymap->min_ચાવી_code);
    FAIL_UNLESS(reply->first_ચાવી_Action + reply->n_ચાવી_Actions ==
                keymap->max_ચાવી_code + 1);

    માટે (પૂર્ણાંક i = 0; i < acts_ગણતરી_length; i++) {
        xcb_xkb_ચાવી_sym_map_t *wire_sym_map = sym_maps_iter.data;
        પૂર્ણાંક syms_length = xcb_xkb_ચાવી_sym_map_syms_length(wire_sym_map);
        uint8_t wire_ગણતરી = *acts_ગણતરી_iter;
        સંરચના xkb_ચાવી *ચાવી = &keymap->ચાવીઓ[reply->first_ચાવી_Action + i];

        FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * ચાવી->અંક_groups);
        FAIL_UNLESS(wire_ગણતરી == 0 || wire_ગણતરી == syms_length);

        અચલ xkb_keycode_t min_ચાવી_code = keymap->min_ચાવી_code;
        અચલ xkb_keycode_t max_ચાવી_code = keymap->max_ચાવી_code;

        જો (wire_ગણતરી != 0) {
            માટે (xkb_layout_index_t group = 0; group < ચાવી->અંક_groups; group++) {
                માટે (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    જો (level < ચાવી->groups[group].type->અંક_levels) {
                        ચાવી->groups[group].levels[level].અંક_actions = 1;
                        સંઘ xkb_action *action =
                            &ચાવી->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_ચાવી_code, max_ચાવી_code));

                        જો (action->type != ACTION_TYPE_NONE) {
                            ચાવી->groups[group].implicit_actions =
                                !(ચાવી->explicit & EXPLICIT_INTERP);
                        } નહીંતર જો (ચાવી->groups[group].levels[level].s.sym ==
                                   XKB_ચાવી_No_પ્રતીક) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            ચાવી->groups[group].levels[level].અંક_syms = 0;
                            ચાવી->groups[group].levels[level].અંક_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                જો (ચાવી->groups[group].implicit_actions)
                    ચાવી->implicit_actions = true;
            }
        }

        acts_ગણતરી_iter++;
        xcb_xkb_ચાવી_sym_map_next(&sym_maps_iter);
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_vmods(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.અંક_mods =
        અંક_REAL_MODS + MIN(msb_pos(reply->virtualMods), અંક_VMODS);
    FAIL_UNLESS(keymap->mods.અંક_mods <= XKB_MAX_MODS);

    static_assert(અંક_REAL_MODS + અંક_VMODS <= XKB_MAX_MODS, "");
    માટે (unsigned i = 0; i < અંક_VMODS; i++) {
        જો (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            સંરચના xkb_mod *mod = &keymap->mods.mods[અંક_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_explicits(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    પૂર્ણાંક length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    માટે (પૂર્ણાંક i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        સંરચના xkb_ચાવી *ચાવી;

        FAIL_UNLESS(wire->keycode >= keymap->min_ચાવી_code &&
                    wire->keycode <= keymap->max_ચાવી_code);

        ચાવી = &keymap->ચાવીઓ[wire->keycode];

        જો ((wire->explicit & XCB_XKB_EXPLICIT_ચાવી_TYPE_1) &&
            ચાવી->અંક_groups > 0) {
            ચાવી->groups[0].explicit_type = true;
            ચાવી->explicit |= EXPLICIT_TYPES;
        }
        જો ((wire->explicit & XCB_XKB_EXPLICIT_ચાવી_TYPE_2) &&
            ચાવી->અંક_groups > 1) {
            ચાવી->groups[1].explicit_type = true;
            ચાવી->explicit |= EXPLICIT_TYPES;
        }
        જો ((wire->explicit & XCB_XKB_EXPLICIT_ચાવી_TYPE_3) &&
            ચાવી->અંક_groups > 2) {
            ચાવી->groups[2].explicit_type = true;
            ચાવી->explicit |= EXPLICIT_TYPES;
        }
        જો ((wire->explicit & XCB_XKB_EXPLICIT_ચાવી_TYPE_4) &&
            ચાવી->અંક_groups > 3) {
            ચાવી->groups[3].explicit_type = true;
            ચાવી->explicit |= EXPLICIT_TYPES;
        }
        જો (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            ચાવી->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            માટે (xkb_layout_index_t l = 0; l < ચાવી->અંક_groups; l++) {
                ચાવી->groups[l].explicit_actions = true;
            }
        }
        જો (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            ચાવી->explicit |= EXPLICIT_REPEAT;
        જો (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            ચાવી->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_modmaps(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    પૂર્ણાંક length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_ચાવી_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    માટે (પૂર્ણાંક i = 0; i < length; i++) {
        xcb_xkb_ચાવી_mod_map_t *wire = iter.data;
        સંરચના xkb_ચાવી *ચાવી;

        FAIL_UNLESS(wire->keycode >= keymap->min_ચાવી_code &&
                    wire->keycode <= keymap->max_ચાવી_code);

        ચાવી = &keymap->ચાવીઓ[wire->keycode];
        ચાવી->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .ચાવી_Code = ચાવી->keycode,
            .mods = ચાવી->modmap,
            .have_પ્રતીક = false,
            .u.ચાવી_Name = ચાવી->name
        });

        xcb_xkb_ચાવી_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->અંક_modmaps);
    પાછા true;

fail:
    darray_free(modmaps);
    પાછા false;
}

સ્થિર bool
get_vmodmaps(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    પૂર્ણાંક length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_ચાવી_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    માટે (પૂર્ણાંક i = 0; i < length; i++) {
        xcb_xkb_ચાવી_v_mod_map_t *wire = iter.data;
        સંરચના xkb_ચાવી *ચાવી;

        FAIL_UNLESS(wire->keycode >= keymap->min_ચાવી_code &&
                    wire->keycode <= keymap->max_ચાવી_code);

        ચાવી = &keymap->ચાવીઓ[wire->keycode];
        ચાવી->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_ચાવી_v_mod_map_next(&iter);
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_map(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, ખાલી);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    જો ((reply->present & get_map_required_components) != get_map_required_components)
        જાઓ fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_ચાવી_Syms,
                               reply->n_ચાવી_Actions,
                               reply->totalActions,
                               reply->total_ચાવી_Behaviors,
                               reply->virtualMods,
                               reply->total_ચાવી_Explicit,
                               reply->total_Mod_Map_ચાવીઓ,
                               reply->total_VMod_Map_ચાવીઓ,
                               reply->present,
                               &map);

    જો (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        જાઓ fail;

    મુક્ત(reply);
    પાછા true;

fail:
    મુક્ત(reply);
    પાછા false;
}

સ્થિર bool
get_indicators(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->અંક_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->અંક_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == અંક_INDICATORS, "");
    માટે (unsigned i = 0; i < અંક_INDICATORS; i++) {
        જો (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            સંરચના xkb_led *led = &keymap->leds[i];

            જો (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            જો (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            જો (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            જો (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            જો (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            જો (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            જો (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            જો (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            જો (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            જો (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_indicator_map(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, ખાલી);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    જો (!get_indicators(keymap, conn, reply))
        જાઓ fail;

    મુક્ત(reply);
    પાછા true;

fail:
    મુક્ત(reply);
    પાછા false;
}

સ્થિર bool
get_sym_interprets(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    પૂર્ણાંક length = xcb_xkb_get_compat_map_si_rtrn_length(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->nTotalSI);

    keymap->અંક_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->અંક_sym_interprets);

    અચલ xkb_keycode_t min_ચાવી_code = keymap->min_ચાવી_code;
    અચલ xkb_keycode_t max_ચાવી_code = keymap->max_ચાવી_code;

    માટે (પૂર્ણાંક i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        સંરચના xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        પસંદગી (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        સ્થિતિ XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            તોડો;
        સ્થિતિ XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            તોડો;
        સ્થિતિ XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            તોડો;
        સ્થિતિ XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            તોડો;
        સ્થિતિ XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            તોડો;
        મૂળભૂત:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            જાઓ fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        જો (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        નહીંતર
            sym_interpret->virtual_mod = અંક_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_ચાવી_code, max_ચાવી_code));
        sym_interpret->અંક_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_compat_map(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, ખાલી);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    જો (!get_sym_interprets(keymap, conn, reply))
        જાઓ fail;

    મુક્ત(reply);
    પાછા true;

fail:
    મુક્ત(reply);
    પાછા false;
}

સ્થિર bool
get_type_names(સંરચના xkb_keymap *keymap, સંરચના x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_કિંમત_list_t *list)
{
    પૂર્ણાંક ચાવી_type_names_length =
        xcb_xkb_get_names_કિંમત_list_type_names_length(reply, list);
    xcb_atom_t *ચાવી_type_names_iter =
        xcb_xkb_get_names_કિંમત_list_type_names(list);
    પૂર્ણાંક n_levels_per_type_length =
        xcb_xkb_get_names_કિંમત_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_કિંમત_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_કિંમત_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->અંક_types);
    FAIL_UNLESS(ચાવી_type_names_length == n_levels_per_type_length);

    માટે (પૂર્ણાંક i = 0; i < ચાવી_type_names_length; i++) {
        xcb_atom_t wire_type_name = *ચાવી_type_names_iter;
        uint8_t wire_અંક_levels = *n_levels_per_type_iter;
        સંરચના xkb_ચાવી_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->અંક_levels >= wire_અંક_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->અંક_levels);
        type->અંક_level_names = type->અંક_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        માટે (size_t j = 0; j < wire_અંક_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        માટે (size_t j = wire_અંક_levels; j < type->અંક_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_અંક_levels;
        ચાવી_type_names_iter++;
        n_levels_per_type_iter++;
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_indicator_names(સંરચના xkb_keymap *keymap,
                    સંરચના x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_કિંમત_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_કિંમત_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->અંક_leds);

    માટે (unsigned i = 0; i < અંક_INDICATORS; i++) {
        જો (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            સંરચના xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_vmod_names(સંરચના xkb_keymap *keymap, સંરચના x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_કિંમત_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_કિંમત_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.અંક_mods =
        અંક_REAL_MODS + MIN(msb_pos(reply->virtualMods), અંક_VMODS);
    FAIL_UNLESS(keymap->mods.અંક_mods <= XKB_MAX_MODS);

    static_assert(અંક_REAL_MODS + અંક_VMODS <= XKB_MAX_MODS, "");
    માટે (unsigned i = 0; i < અંક_VMODS; i++) {
        જો (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            સંરચના xkb_mod *mod = &keymap->mods.mods[અંક_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_group_names(સંરચના xkb_keymap *keymap, સંરચના x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_કિંમત_list_t *list)
{
    પૂર્ણાંક length = xcb_xkb_get_names_કિંમત_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_કિંમત_list_groups(list);

    keymap->અંક_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->અંક_group_names);

    માટે (પૂર્ણાંક i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_ચાવી_names(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_કિંમત_list_t *list)
{
    પૂર્ણાંક length = xcb_xkb_get_names_કિંમત_list_ચાવી_names_length(reply, list);
    xcb_xkb_ચાવી_name_iterator_t iter =
        xcb_xkb_get_names_કિંમત_list_ચાવી_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_ચાવી_Code == keymap->min_ચાવી_code);
    FAIL_UNLESS(reply->max_ચાવી_Code == keymap->max_ચાવી_code);
    FAIL_UNLESS(reply->first_ચાવી == keymap->min_ચાવી_code);
    FAIL_UNLESS(reply->first_ચાવી + reply->n_ચાવીઓ - 1U == keymap->max_ચાવી_code);

    માટે (પૂર્ણાંક i = 0; i < length; i++) {
        xcb_xkb_ચાવી_name_t *wire = iter.data;
        xkb_atom_t *ચાવી_name = &keymap->ચાવીઓ[reply->first_ચાવી + i].name;

        જો (wire->name[0] == '\0') {
            *ચાવી_name = XKB_ATOM_NONE;
        }
        નહીંતર {
            *ચાવી_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_ચાવી_NAME_LENGTH));
            જો (!*ચાવી_name)
                પાછા false;
        }

        xcb_xkb_ચાવી_name_next(&iter);
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_aliases(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_કિંમત_list_t *list)
{
    પૂર્ણાંક length = xcb_xkb_get_names_કિંમત_list_ચાવી_aliases_length(reply, list);
    xcb_xkb_ચાવી_alias_iterator_t iter =
        xcb_xkb_get_names_કિંમત_list_ચાવી_aliases_iterator(reply, list);

    keymap->અંક_ચાવી_aliases = reply->n_ચાવી_Aliases;
    ALLOC_OR_FAIL(keymap->ચાવી_aliases, keymap->અંક_ચાવી_aliases);

    માટે (પૂર્ણાંક i = 0; i < length; i++) {
        xcb_xkb_ચાવી_alias_t *wire = iter.data;
        સંરચના xkb_ચાવી_alias *alias = &keymap->ચાવી_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_ચાવી_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_ચાવી_NAME_LENGTH));
        જો (!alias->real || !alias->alias)
            જાઓ fail;

        xcb_xkb_ચાવી_alias_next(&iter);
    }

    પાછા true;

fail:
    પાછા false;
}

સ્થિર bool
get_names(સંરચના xkb_keymap *keymap, સંરચના x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, ખાલી);
    xcb_xkb_get_names_કિંમત_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_કિંમત_list_unpack(xcb_xkb_get_names_કિંમત_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_ચાવીઓ,
                                        reply->n_ચાવી_Aliases,
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
    જો (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_ચાવી_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        જાઓ fail;

    મુક્ત(reply);
    પાછા true;

fail:
    મુક્ત(reply);
    પાછા false;
}

સ્થિર bool
get_controls(સંરચના xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, ખાલી);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->અંક_Groups > 0 && reply->અંક_Groups <= 4);

    keymap->અંક_groups = reply->અંક_Groups;

    FAIL_UNLESS(keymap->max_ચાવી_code < XCB_XKB_CONST_PER_ચાવી_BIT_ARRAY_SIZE * 8);

    માટે (xkb_keycode_t i = keymap->min_ચાવી_code; i <= keymap->max_ચાવી_code; i++)
        keymap->ચાવીઓ[i].repeats = (reply->per_ચાવી_Repeat[i / 8] & (1u << (i % 8)));

    મુક્ત(reply);
    પાછા true;

fail:
    મુક્ત(reply);
    પાછા false;
}

સંરચના xkb_keymap *
xkb_x11_keymap_new_from_device(સંરચના xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               ગણના xkb_keymap_compile_flags flags)
{
    જો (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        પાછા ખાલી;
    }

    અચલ ગણના xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    સંરચના xkb_keymap * અચલ keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    જો (!keymap)
        પાછા ખાલી;

    keymap->redirect_ચાવી_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    સંરચના x11_atom_interner interner;
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

    જો (!get_map(keymap, conn, map_cookie))
        જાઓ err_map;
    જો (!get_indicator_map(keymap, conn, indicator_map_cookie))
        જાઓ err_indicator_map;
    જો (!get_compat_map(keymap, conn, compat_map_cookie))
        જાઓ err_compat_map;
    જો (!get_names(keymap, &interner, names_cookie))
        જાઓ err_names;
    જો (!get_controls(keymap, conn, controls_cookie))
        જાઓ err_controls;
    x11_atom_interner_round_trip(&interner);
    જો (interner.had_error)
        જાઓ err_interner;

    પાછા keymap;

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
    પાછા ખાલી;
}
