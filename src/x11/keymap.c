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
#define अङ्क_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define अङ्क_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define अङ्क_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(अभिव्यक्ति) गर {                              \
    यदि (!(अभिव्यक्ति)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(अभिव्यक्ति));                 \
        गच्छ fail;                                          \
    }                                                       \
} जबसम्म (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) गर {         \
    यदि (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        गच्छ fail;                                          \
    }                                                       \
} जबसम्म (0)

#define ALLOC_OR_FAIL(arr, nmemb) गर {                      \
    यदि ((nmemb) > 0) {                                      \
        (arr) = सुस्मृति((nmemb), परिमाण(*(arr)));            \
        यदि (!(arr))                                         \
            गच्छ fail;                                      \
    }                                                       \
} जबसम्म (0)

स्थिर अचर xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_साँचो_TYPES |
     XCB_XKB_MAP_PART_साँचो_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_साँचो_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

स्थिर अचर xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_साँचो_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_साँचो_NAMES |
     XCB_XKB_NAME_DETAIL_साँचो_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
स्थिर अचर xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_साँचो_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_साँचो_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


स्थिर xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_तल्लो, uint16_t vmods_माथिल्लो)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    फर्काऊ
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_तल्लो << 8) |
        ((xkb_mod_mask_t) vmods_माथिल्लो << 16);
}

गणना xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    गणना xkb_action_controls ret = 0;
    यदि (wire & XCB_XKB_BOOL_CTRL_REPEAT_साँचोहरू)
        ret |= CONTROL_REPEAT;
    यदि (wire & XCB_XKB_BOOL_CTRL_SLOW_साँचोहरू)
        ret |= CONTROL_SLOW;
    यदि (wire & XCB_XKB_BOOL_CTRL_BOUNCE_साँचोहरू)
        ret |= CONTROL_DEBOUNCE;
    यदि (wire & XCB_XKB_BOOL_CTRL_STICKY_साँचोहरू)
        ret |= CONTROL_STICKY_साँचोहरू;
    यदि (wire & XCB_XKB_BOOL_CTRL_MOUSE_साँचोहरू)
        ret |= CONTROL_MOUSE_साँचोहरू;
    यदि (wire & XCB_XKB_BOOL_CTRL_MOUSE_साँचोहरू_ACCEL)
        ret |= CONTROL_MOUSE_साँचोहरू_ACCEL;
    यदि (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_साँचोहरू)
        ret |= CONTROL_AX;
    यदि (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    यदि (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    यदि (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    यदि (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    यदि (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    यदि (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    फर्काऊ ret;
}

स्थिर bool
translate_action(सङ्घ xkb_action *action, अचर xcb_xkb_action_t *wire,
                 xkb_keycode_t min_साँचो_code, xkb_keycode_t max_साँचो_code)
{
    विकल्प (wire->type) {
    पक्ष XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_तल्लो_2,
                                                wire->setmods.vmods_माथिल्लो_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        यदि (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        यदि (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        यदि (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_तल्लो_2,
                                                wire->latchmods.vmods_माथिल्लो_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        यदि (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        यदि (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        यदि (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_तल्लो_2,
                                                wire->lockmods.vmods_माथिल्लो_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        यदि (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        यदि (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        यदि (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        यदि (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        यदि (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        यदि (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        यदि (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        यदि (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        यदि (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        यदि (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_तल्लो | ((uint16_t) wire->moveptr.x_माथिल्लो << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_तल्लो | ((uint16_t) wire->moveptr.y_माथिल्लो << 8));

        यदि (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        यदि (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        यदि (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.गन्ती = wire->ptrbtn.गन्ती;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.गन्ती = 0;

        यदि (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        यदि (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.मान = wire->setptrdflt.मान;

        यदि (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        यदि (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        यदि (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        रोक;
    पक्ष XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            अचर uint16_t mask = (wire->setcontrols.bool_Ctrls_तल्लो |
                                   (wire->setcontrols.bool_Ctrls_माथिल्लो << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        रोक;
    पक्ष XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            अचर uint16_t mask = (wire->lockcontrols.bool_Ctrls_तल्लो |
                                   (wire->lockcontrols.bool_Ctrls_माथिल्लो << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        रोक;

    पक्ष XCB_XKB_SA_TYPE_REDIRECT_साँचो:
        action->type = ACTION_TYPE_REDIRECT_साँचो;
        यदि (wire->redirect.newkey < min_साँचो_code ||
            wire->redirect.newkey > max_साँचो_code)
                फर्काऊ false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_तल्लो,
            wire->redirect.vmods_Mask_माथिल्लो
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_तल्लो_2,
                                               wire->redirect.vmods_माथिल्लो_2);
        रोक;

    पक्ष XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        रोक;

    /* We don't support these. */
    पक्ष XCB_XKB_SA_TYPE_ISO_LOCK:
    पक्ष XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    पक्ष XCB_XKB_SA_TYPE_DEVICE_BTN:
    पक्ष XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    पक्ष XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        रोक;

    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अङ्क_ENTRIES,
                      "Missing action type");

        यदि (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            रोक;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(परिमाण(action->priv.data) == 7 &&
                      परिमाण(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        स_नकल(action->priv.data, wire->noaction.pad0, 7);
        रोक;
    }
    फर्काऊ true;
}

स्थिर bool
get_types(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_साँचो_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->अङ्क_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->अङ्क_types);
    प्रतिज्ञा(types_length == (पूर्णाङ्क) keymap->अङ्क_types);

    लागि (पूर्णाङ्क i = 0; i < types_length; i++) {
        xcb_xkb_साँचो_type_t *wire_type = types_iter.data;
        संरचना xkb_साँचो_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->अङ्क_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->अङ्क_levels = wire_type->अङ्क_Levels;

        {
            पूर्णाङ्क entries_length = xcb_xkb_साँचो_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_साँचो_type_map_iterator(wire_type);

            type->अङ्क_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->अङ्क_entries);
            प्रतिज्ञा(entries_length == (पूर्णाङ्क) type->अङ्क_entries);

            लागि (पूर्णाङ्क j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                संरचना xkb_साँचो_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->अङ्क_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            पूर्णाङ्क preserves_length = xcb_xkb_साँचो_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_साँचो_type_preserve_iterator(wire_type);

            FAIL_UNLESS((अचिन्हित) preserves_length <= type->अङ्क_entries);

            लागि (पूर्णाङ्क j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                संरचना xkb_साँचो_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_साँचो_type_next(&types_iter);
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_sym_maps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_साँचो_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_साँचो_Code <= reply->max_साँचो_Code);
    FAIL_UNLESS(reply->first_साँचो_Sym >= reply->min_साँचो_Code);
    FAIL_UNLESS(reply->first_साँचो_Sym + reply->n_साँचो_Syms <= reply->max_साँचो_Code + 1);

    keymap->min_साँचो_code = reply->min_साँचो_Code;
    keymap->max_साँचो_code = reply->max_साँचो_Code;
    keymap->अङ्क_साँचोहरू = keymap->अङ्क_साँचोहरू_तल्लो = reply->max_साँचो_Code + 1;

    ALLOC_OR_FAIL(keymap->साँचोहरू, keymap->max_साँचो_code + 1);

    लागि (xkb_keycode_t kc = keymap->min_साँचो_code; kc <= keymap->max_साँचो_code; kc++)
        keymap->साँचोहरू[kc].keycode = kc;

    लागि (पूर्णाङ्क i = 0; i < sym_maps_length; i++) {
        xcb_xkb_साँचो_sym_map_t *wire_sym_map = sym_maps_iter.data;
        संरचना xkb_साँचो *साँचो = &keymap->साँचोहरू[reply->first_साँचो_Sym + i];

        साँचो->अङ्क_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(साँचो->अङ्क_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(साँचो->groups, साँचो->अङ्क_groups);

        लागि (xkb_layout_index_t j = 0; j < साँचो->अङ्क_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->अङ्क_types);
            साँचो->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(साँचो->groups[j].levels, साँचो->groups[j].type->अङ्क_levels);
        }

        साँचो->out_of_range_group_सङ्ख्या = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(साँचो->out_of_range_group_सङ्ख्या <= साँचो->अङ्क_groups);

        यदि (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            साँचो->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        अन्यथा यदि (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            साँचो->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        अन्यथा
            साँचो->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            पूर्णाङ्क syms_length = xcb_xkb_साँचो_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_साँचो_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((अचिन्हित) syms_length == wire_sym_map->width * साँचो->अङ्क_groups);

            यदि (syms_length > 0)
                साँचो->explicit |= EXPLICIT_SYMBOLS;

            लागि (xkb_layout_index_t group = 0; group < साँचो->अङ्क_groups; group++) {
                लागि (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    प्रतिज्ञा(साँचो->groups[group].type != रिक्त);
                    यदि (level < साँचो->groups[group].type->अङ्क_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        साँचो->groups[group].levels[level].अङ्क_syms = 1;
                        साँचो->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        साँचो->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        यदि (wire_keysym != XKB_साँचो_No_चिह्न)
                            साँचो->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_साँचो_sym_map_next(&sym_maps_iter);
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_actions(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क acts_गन्ती_length =
        xcb_xkb_get_map_map_acts_rtrn_गन्ती_length(reply, map);
    uint8_t *acts_गन्ती_iter = xcb_xkb_get_map_map_acts_rtrn_गन्ती(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_साँचो_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_साँचो_Action == keymap->min_साँचो_code);
    FAIL_UNLESS(reply->first_साँचो_Action + reply->n_साँचो_Actions ==
                keymap->max_साँचो_code + 1);

    लागि (पूर्णाङ्क i = 0; i < acts_गन्ती_length; i++) {
        xcb_xkb_साँचो_sym_map_t *wire_sym_map = sym_maps_iter.data;
        पूर्णाङ्क syms_length = xcb_xkb_साँचो_sym_map_syms_length(wire_sym_map);
        uint8_t wire_गन्ती = *acts_गन्ती_iter;
        संरचना xkb_साँचो *साँचो = &keymap->साँचोहरू[reply->first_साँचो_Action + i];

        FAIL_UNLESS((अचिन्हित) syms_length == wire_sym_map->width * साँचो->अङ्क_groups);
        FAIL_UNLESS(wire_गन्ती == 0 || wire_गन्ती == syms_length);

        अचर xkb_keycode_t min_साँचो_code = keymap->min_साँचो_code;
        अचर xkb_keycode_t max_साँचो_code = keymap->max_साँचो_code;

        यदि (wire_गन्ती != 0) {
            लागि (xkb_layout_index_t group = 0; group < साँचो->अङ्क_groups; group++) {
                लागि (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    यदि (level < साँचो->groups[group].type->अङ्क_levels) {
                        साँचो->groups[group].levels[level].अङ्क_actions = 1;
                        सङ्घ xkb_action *action =
                            &साँचो->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_साँचो_code, max_साँचो_code));

                        यदि (action->type != ACTION_TYPE_NONE) {
                            साँचो->groups[group].implicit_actions =
                                !(साँचो->explicit & EXPLICIT_INTERP);
                        } अन्यथा यदि (साँचो->groups[group].levels[level].s.sym ==
                                   XKB_साँचो_No_चिह्न) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            साँचो->groups[group].levels[level].अङ्क_syms = 0;
                            साँचो->groups[group].levels[level].अङ्क_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                यदि (साँचो->groups[group].implicit_actions)
                    साँचो->implicit_actions = true;
            }
        }

        acts_गन्ती_iter++;
        xcb_xkb_साँचो_sym_map_next(&sym_maps_iter);
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_vmods(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.अङ्क_mods =
        अङ्क_REAL_MODS + MIN(msb_pos(reply->virtualMods), अङ्क_VMODS);
    FAIL_UNLESS(keymap->mods.अङ्क_mods <= XKB_MAX_MODS);

    static_assert(अङ्क_REAL_MODS + अङ्क_VMODS <= XKB_MAX_MODS, "");
    लागि (अचिन्हित i = 0; i < अङ्क_VMODS; i++) {
        यदि (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            संरचना xkb_mod *mod = &keymap->mods.mods[अङ्क_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_explicits(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    लागि (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        संरचना xkb_साँचो *साँचो;

        FAIL_UNLESS(wire->keycode >= keymap->min_साँचो_code &&
                    wire->keycode <= keymap->max_साँचो_code);

        साँचो = &keymap->साँचोहरू[wire->keycode];

        यदि ((wire->explicit & XCB_XKB_EXPLICIT_साँचो_TYPE_1) &&
            साँचो->अङ्क_groups > 0) {
            साँचो->groups[0].explicit_type = true;
            साँचो->explicit |= EXPLICIT_TYPES;
        }
        यदि ((wire->explicit & XCB_XKB_EXPLICIT_साँचो_TYPE_2) &&
            साँचो->अङ्क_groups > 1) {
            साँचो->groups[1].explicit_type = true;
            साँचो->explicit |= EXPLICIT_TYPES;
        }
        यदि ((wire->explicit & XCB_XKB_EXPLICIT_साँचो_TYPE_3) &&
            साँचो->अङ्क_groups > 2) {
            साँचो->groups[2].explicit_type = true;
            साँचो->explicit |= EXPLICIT_TYPES;
        }
        यदि ((wire->explicit & XCB_XKB_EXPLICIT_साँचो_TYPE_4) &&
            साँचो->अङ्क_groups > 3) {
            साँचो->groups[3].explicit_type = true;
            साँचो->explicit |= EXPLICIT_TYPES;
        }
        यदि (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            साँचो->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            लागि (xkb_layout_index_t l = 0; l < साँचो->अङ्क_groups; l++) {
                साँचो->groups[l].explicit_actions = true;
            }
        }
        यदि (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            साँचो->explicit |= EXPLICIT_REPEAT;
        यदि (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            साँचो->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_modmaps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_साँचो_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    लागि (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_साँचो_mod_map_t *wire = iter.data;
        संरचना xkb_साँचो *साँचो;

        FAIL_UNLESS(wire->keycode >= keymap->min_साँचो_code &&
                    wire->keycode <= keymap->max_साँचो_code);

        साँचो = &keymap->साँचोहरू[wire->keycode];
        साँचो->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .साँचो_Code = साँचो->keycode,
            .mods = साँचो->modmap,
            .have_चिह्न = false,
            .u.साँचो_Name = साँचो->name
        });

        xcb_xkb_साँचो_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->अङ्क_modmaps);
    फर्काऊ true;

fail:
    darray_free(modmaps);
    फर्काऊ false;
}

स्थिर bool
get_vmodmaps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_साँचो_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    लागि (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_साँचो_v_mod_map_t *wire = iter.data;
        संरचना xkb_साँचो *साँचो;

        FAIL_UNLESS(wire->keycode >= keymap->min_साँचो_code &&
                    wire->keycode <= keymap->max_साँचो_code);

        साँचो = &keymap->साँचोहरू[wire->keycode];
        साँचो->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_साँचो_v_mod_map_next(&iter);
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, रिक्त);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    यदि ((reply->present & get_map_required_components) != get_map_required_components)
        गच्छ fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_साँचो_Syms,
                               reply->n_साँचो_Actions,
                               reply->totalActions,
                               reply->total_साँचो_Behaviors,
                               reply->virtualMods,
                               reply->total_साँचो_Explicit,
                               reply->total_Mod_Map_साँचोहरू,
                               reply->total_VMod_Map_साँचोहरू,
                               reply->present,
                               &map);

    यदि (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        गच्छ fail;

    मुक्त(reply);
    फर्काऊ true;

fail:
    मुक्त(reply);
    फर्काऊ false;
}

स्थिर bool
get_indicators(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->अङ्क_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->अङ्क_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == अङ्क_INDICATORS, "");
    लागि (अचिन्हित i = 0; i < अङ्क_INDICATORS; i++) {
        यदि (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            संरचना xkb_led *led = &keymap->leds[i];

            यदि (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            यदि (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            यदि (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            यदि (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            यदि (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            यदि (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            यदि (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            यदि (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            यदि (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            यदि (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_indicator_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, रिक्त);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    यदि (!get_indicators(keymap, conn, reply))
        गच्छ fail;

    मुक्त(reply);
    फर्काऊ true;

fail:
    मुक्त(reply);
    फर्काऊ false;
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

    keymap->अङ्क_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->अङ्क_sym_interprets);

    अचर xkb_keycode_t min_साँचो_code = keymap->min_साँचो_code;
    अचर xkb_keycode_t max_साँचो_code = keymap->max_साँचो_code;

    लागि (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        संरचना xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        विकल्प (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        पक्ष XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            रोक;
        पक्ष XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            रोक;
        पक्ष XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            रोक;
        पक्ष XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            रोक;
        पक्ष XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            रोक;
        स्वभाव:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            गच्छ fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        यदि (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        अन्यथा
            sym_interpret->virtual_mod = अङ्क_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_साँचो_code, max_साँचो_code));
        sym_interpret->अङ्क_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_compat_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, रिक्त);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    यदि (!get_sym_interprets(keymap, conn, reply))
        गच्छ fail;

    मुक्त(reply);
    फर्काऊ true;

fail:
    मुक्त(reply);
    फर्काऊ false;
}

स्थिर bool
get_type_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_मान_list_t *list)
{
    पूर्णाङ्क साँचो_type_names_length =
        xcb_xkb_get_names_मान_list_type_names_length(reply, list);
    xcb_atom_t *साँचो_type_names_iter =
        xcb_xkb_get_names_मान_list_type_names(list);
    पूर्णाङ्क n_levels_per_type_length =
        xcb_xkb_get_names_मान_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_मान_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_मान_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->अङ्क_types);
    FAIL_UNLESS(साँचो_type_names_length == n_levels_per_type_length);

    लागि (पूर्णाङ्क i = 0; i < साँचो_type_names_length; i++) {
        xcb_atom_t wire_type_name = *साँचो_type_names_iter;
        uint8_t wire_अङ्क_levels = *n_levels_per_type_iter;
        संरचना xkb_साँचो_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->अङ्क_levels >= wire_अङ्क_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->अङ्क_levels);
        type->अङ्क_level_names = type->अङ्क_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        लागि (माप_प्रकार j = 0; j < wire_अङ्क_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        लागि (माप_प्रकार j = wire_अङ्क_levels; j < type->अङ्क_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_अङ्क_levels;
        साँचो_type_names_iter++;
        n_levels_per_type_iter++;
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_indicator_names(संरचना xkb_keymap *keymap,
                    संरचना x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_मान_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_मान_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->अङ्क_leds);

    लागि (अचिन्हित i = 0; i < अङ्क_INDICATORS; i++) {
        यदि (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            संरचना xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_vmod_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_मान_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_मान_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.अङ्क_mods =
        अङ्क_REAL_MODS + MIN(msb_pos(reply->virtualMods), अङ्क_VMODS);
    FAIL_UNLESS(keymap->mods.अङ्क_mods <= XKB_MAX_MODS);

    static_assert(अङ्क_REAL_MODS + अङ्क_VMODS <= XKB_MAX_MODS, "");
    लागि (अचिन्हित i = 0; i < अङ्क_VMODS; i++) {
        यदि (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            संरचना xkb_mod *mod = &keymap->mods.mods[अङ्क_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_group_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_मान_list_t *list)
{
    पूर्णाङ्क length = xcb_xkb_get_names_मान_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_मान_list_groups(list);

    keymap->अङ्क_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->अङ्क_group_names);

    लागि (पूर्णाङ्क i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_साँचो_names(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_मान_list_t *list)
{
    पूर्णाङ्क length = xcb_xkb_get_names_मान_list_साँचो_names_length(reply, list);
    xcb_xkb_साँचो_name_iterator_t iter =
        xcb_xkb_get_names_मान_list_साँचो_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_साँचो_Code == keymap->min_साँचो_code);
    FAIL_UNLESS(reply->max_साँचो_Code == keymap->max_साँचो_code);
    FAIL_UNLESS(reply->first_साँचो == keymap->min_साँचो_code);
    FAIL_UNLESS(reply->first_साँचो + reply->n_साँचोहरू - 1U == keymap->max_साँचो_code);

    लागि (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_साँचो_name_t *wire = iter.data;
        xkb_atom_t *साँचो_name = &keymap->साँचोहरू[reply->first_साँचो + i].name;

        यदि (wire->name[0] == '\0') {
            *साँचो_name = XKB_ATOM_NONE;
        }
        अन्यथा {
            *साँचो_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_साँचो_NAME_LENGTH));
            यदि (!*साँचो_name)
                फर्काऊ false;
        }

        xcb_xkb_साँचो_name_next(&iter);
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_aliases(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_मान_list_t *list)
{
    पूर्णाङ्क length = xcb_xkb_get_names_मान_list_साँचो_aliases_length(reply, list);
    xcb_xkb_साँचो_alias_iterator_t iter =
        xcb_xkb_get_names_मान_list_साँचो_aliases_iterator(reply, list);

    keymap->अङ्क_साँचो_aliases = reply->n_साँचो_Aliases;
    ALLOC_OR_FAIL(keymap->साँचो_aliases, keymap->अङ्क_साँचो_aliases);

    लागि (पूर्णाङ्क i = 0; i < length; i++) {
        xcb_xkb_साँचो_alias_t *wire = iter.data;
        संरचना xkb_साँचो_alias *alias = &keymap->साँचो_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_साँचो_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_साँचो_NAME_LENGTH));
        यदि (!alias->real || !alias->alias)
            गच्छ fail;

        xcb_xkb_साँचो_alias_next(&iter);
    }

    फर्काऊ true;

fail:
    फर्काऊ false;
}

स्थिर bool
get_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, रिक्त);
    xcb_xkb_get_names_मान_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_मान_list_unpack(xcb_xkb_get_names_मान_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_साँचोहरू,
                                        reply->n_साँचो_Aliases,
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
    यदि (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_साँचो_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        गच्छ fail;

    मुक्त(reply);
    फर्काऊ true;

fail:
    मुक्त(reply);
    फर्काऊ false;
}

स्थिर bool
get_controls(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, रिक्त);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->अङ्क_Groups > 0 && reply->अङ्क_Groups <= 4);

    keymap->अङ्क_groups = reply->अङ्क_Groups;

    FAIL_UNLESS(keymap->max_साँचो_code < XCB_XKB_CONST_PER_साँचो_BIT_ARRAY_SIZE * 8);

    लागि (xkb_keycode_t i = keymap->min_साँचो_code; i <= keymap->max_साँचो_code; i++)
        keymap->साँचोहरू[i].repeats = (reply->per_साँचो_Repeat[i / 8] & (1u << (i % 8)));

    मुक्त(reply);
    फर्काऊ true;

fail:
    मुक्त(reply);
    फर्काऊ false;
}

संरचना xkb_keymap *
xkb_x11_keymap_new_from_device(संरचना xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               गणना xkb_keymap_compile_flags flags)
{
    यदि (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        फर्काऊ रिक्त;
    }

    अचर गणना xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    संरचना xkb_keymap * अचर keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    यदि (!keymap)
        फर्काऊ रिक्त;

    keymap->redirect_साँचो_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

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

    यदि (!get_map(keymap, conn, map_cookie))
        गच्छ err_map;
    यदि (!get_indicator_map(keymap, conn, indicator_map_cookie))
        गच्छ err_indicator_map;
    यदि (!get_compat_map(keymap, conn, compat_map_cookie))
        गच्छ err_compat_map;
    यदि (!get_names(keymap, &interner, names_cookie))
        गच्छ err_names;
    यदि (!get_controls(keymap, conn, controls_cookie))
        गच्छ err_controls;
    x11_atom_interner_round_trip(&interner);
    यदि (interner.had_error)
        गच्छ err_interner;

    फर्काऊ keymap;

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
    फर्काऊ रिक्त;
}
