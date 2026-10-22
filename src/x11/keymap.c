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
#define अंक_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define अंक_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define अंक_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(अभिव्यक्ती) करा {                              \
    जर (!(अभिव्यक्ती)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(अभिव्यक्ती));                 \
        जा fail;                                          \
    }                                                       \
} पर्यंत (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) करा {         \
    जर (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        जा fail;                                          \
    }                                                       \
} पर्यंत (0)

#define ALLOC_OR_FAIL(arr, nmemb) करा {                      \
    जर ((nmemb) > 0) {                                      \
        (arr) = सुस्मृति((nmemb), आकार(*(arr)));            \
        जर (!(arr))                                         \
            जा fail;                                      \
    }                                                       \
} पर्यंत (0)

स्थिर स्थिरांक xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_किल्ली_TYPES |
     XCB_XKB_MAP_PART_किल्ली_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_किल्ली_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

स्थिर स्थिरांक xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_किल्ली_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_किल्ली_NAMES |
     XCB_XKB_NAME_DETAIL_किल्ली_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
स्थिर स्थिरांक xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_किल्ली_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_किल्ली_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


स्थिर xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_खालचा, uint16_t vmods_वरचा)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    परत
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_खालचा << 8) |
        ((xkb_mod_mask_t) vmods_वरचा << 16);
}

गणना xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    गणना xkb_action_controls ret = 0;
    जर (wire & XCB_XKB_BOOL_CTRL_REPEAT_किल्ल्या)
        ret |= CONTROL_REPEAT;
    जर (wire & XCB_XKB_BOOL_CTRL_SLOW_किल्ल्या)
        ret |= CONTROL_SLOW;
    जर (wire & XCB_XKB_BOOL_CTRL_BOUNCE_किल्ल्या)
        ret |= CONTROL_DEBOUNCE;
    जर (wire & XCB_XKB_BOOL_CTRL_STICKY_किल्ल्या)
        ret |= CONTROL_STICKY_किल्ल्या;
    जर (wire & XCB_XKB_BOOL_CTRL_MOUSE_किल्ल्या)
        ret |= CONTROL_MOUSE_किल्ल्या;
    जर (wire & XCB_XKB_BOOL_CTRL_MOUSE_किल्ल्या_ACCEL)
        ret |= CONTROL_MOUSE_किल्ल्या_ACCEL;
    जर (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_किल्ल्या)
        ret |= CONTROL_AX;
    जर (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    जर (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    जर (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    जर (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    जर (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    जर (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    परत ret;
}

स्थिर bool
translate_action(संघ xkb_action *action, स्थिरांक xcb_xkb_action_t *wire,
                 xkb_keycode_t min_किल्ली_code, xkb_keycode_t max_किल्ली_code)
{
    निवड (wire->type) {
    स्थिती XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_खालचा_2,
                                                wire->setmods.vmods_वरचा_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        जर (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        जर (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        जर (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_खालचा_2,
                                                wire->latchmods.vmods_वरचा_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        जर (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        जर (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        जर (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_खालचा_2,
                                                wire->lockmods.vmods_वरचा_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        जर (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        जर (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        जर (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        जर (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        जर (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        जर (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        जर (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        जर (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        जर (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        जर (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_खालचा | ((uint16_t) wire->moveptr.x_वरचा << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_खालचा | ((uint16_t) wire->moveptr.y_वरचा << 8));

        जर (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        जर (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        जर (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.मोजणी = wire->ptrbtn.मोजणी;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.मोजणी = 0;

        जर (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        जर (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.मूल्य = wire->setptrdflt.मूल्य;

        जर (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        जर (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        जर (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        थांबा;
    स्थिती XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            स्थिरांक uint16_t mask = (wire->setcontrols.bool_Ctrls_खालचा |
                                   (wire->setcontrols.bool_Ctrls_वरचा << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        थांबा;
    स्थिती XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            स्थिरांक uint16_t mask = (wire->lockcontrols.bool_Ctrls_खालचा |
                                   (wire->lockcontrols.bool_Ctrls_वरचा << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        थांबा;

    स्थिती XCB_XKB_SA_TYPE_REDIRECT_किल्ली:
        action->type = ACTION_TYPE_REDIRECT_किल्ली;
        जर (wire->redirect.newkey < min_किल्ली_code ||
            wire->redirect.newkey > max_किल्ली_code)
                परत false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_खालचा,
            wire->redirect.vmods_Mask_वरचा
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_खालचा_2,
                                               wire->redirect.vmods_वरचा_2);
        थांबा;

    स्थिती XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        थांबा;

    /* We don't support these. */
    स्थिती XCB_XKB_SA_TYPE_ISO_LOCK:
    स्थिती XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    स्थिती XCB_XKB_SA_TYPE_DEVICE_BTN:
    स्थिती XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    स्थिती XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        थांबा;

    पूर्वनिर्धारित:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
                      "Missing action type");

        जर (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            थांबा;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(आकार(action->priv.data) == 7 &&
                      आकार(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        स_नकल(action->priv.data, wire->noaction.pad0, 7);
        थांबा;
    }
    परत true;
}

स्थिर bool
get_types(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_किल्ली_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->अंक_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->अंक_types);
    प्रतिज्ञा(types_length == (पूर्णांक) keymap->अंक_types);

    साठी (पूर्णांक i = 0; i < types_length; i++) {
        xcb_xkb_किल्ली_type_t *wire_type = types_iter.data;
        संरचना xkb_किल्ली_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->अंक_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->अंक_levels = wire_type->अंक_Levels;

        {
            पूर्णांक entries_length = xcb_xkb_किल्ली_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_किल्ली_type_map_iterator(wire_type);

            type->अंक_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->अंक_entries);
            प्रतिज्ञा(entries_length == (पूर्णांक) type->अंक_entries);

            साठी (पूर्णांक j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                संरचना xkb_किल्ली_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->अंक_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            पूर्णांक preserves_length = xcb_xkb_किल्ली_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_किल्ली_type_preserve_iterator(wire_type);

            FAIL_UNLESS((अचिन्हित) preserves_length <= type->अंक_entries);

            साठी (पूर्णांक j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                संरचना xkb_किल्ली_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_किल्ली_type_next(&types_iter);
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_sym_maps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_किल्ली_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_किल्ली_Code <= reply->max_किल्ली_Code);
    FAIL_UNLESS(reply->first_किल्ली_Sym >= reply->min_किल्ली_Code);
    FAIL_UNLESS(reply->first_किल्ली_Sym + reply->n_किल्ली_Syms <= reply->max_किल्ली_Code + 1);

    keymap->min_किल्ली_code = reply->min_किल्ली_Code;
    keymap->max_किल्ली_code = reply->max_किल्ली_Code;
    keymap->अंक_किल्ल्या = keymap->अंक_किल्ल्या_खालचा = reply->max_किल्ली_Code + 1;

    ALLOC_OR_FAIL(keymap->किल्ल्या, keymap->max_किल्ली_code + 1);

    साठी (xkb_keycode_t kc = keymap->min_किल्ली_code; kc <= keymap->max_किल्ली_code; kc++)
        keymap->किल्ल्या[kc].keycode = kc;

    साठी (पूर्णांक i = 0; i < sym_maps_length; i++) {
        xcb_xkb_किल्ली_sym_map_t *wire_sym_map = sym_maps_iter.data;
        संरचना xkb_किल्ली *किल्ली = &keymap->किल्ल्या[reply->first_किल्ली_Sym + i];

        किल्ली->अंक_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(किल्ली->अंक_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(किल्ली->groups, किल्ली->अंक_groups);

        साठी (xkb_layout_index_t j = 0; j < किल्ली->अंक_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->अंक_types);
            किल्ली->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(किल्ली->groups[j].levels, किल्ली->groups[j].type->अंक_levels);
        }

        किल्ली->out_of_range_group_संख्या = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(किल्ली->out_of_range_group_संख्या <= किल्ली->अंक_groups);

        जर (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            किल्ली->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        अन्यथा जर (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            किल्ली->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        अन्यथा
            किल्ली->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            पूर्णांक syms_length = xcb_xkb_किल्ली_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_किल्ली_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((अचिन्हित) syms_length == wire_sym_map->width * किल्ली->अंक_groups);

            जर (syms_length > 0)
                किल्ली->explicit |= EXPLICIT_SYMBOLS;

            साठी (xkb_layout_index_t group = 0; group < किल्ली->अंक_groups; group++) {
                साठी (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    प्रतिज्ञा(किल्ली->groups[group].type != रिक्त);
                    जर (level < किल्ली->groups[group].type->अंक_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        किल्ली->groups[group].levels[level].अंक_syms = 1;
                        किल्ली->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        किल्ली->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        जर (wire_keysym != XKB_किल्ली_No_चिन्ह)
                            किल्ली->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_किल्ली_sym_map_next(&sym_maps_iter);
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_actions(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक acts_मोजणी_length =
        xcb_xkb_get_map_map_acts_rtrn_मोजणी_length(reply, map);
    uint8_t *acts_मोजणी_iter = xcb_xkb_get_map_map_acts_rtrn_मोजणी(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_किल्ली_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_किल्ली_Action == keymap->min_किल्ली_code);
    FAIL_UNLESS(reply->first_किल्ली_Action + reply->n_किल्ली_Actions ==
                keymap->max_किल्ली_code + 1);

    साठी (पूर्णांक i = 0; i < acts_मोजणी_length; i++) {
        xcb_xkb_किल्ली_sym_map_t *wire_sym_map = sym_maps_iter.data;
        पूर्णांक syms_length = xcb_xkb_किल्ली_sym_map_syms_length(wire_sym_map);
        uint8_t wire_मोजणी = *acts_मोजणी_iter;
        संरचना xkb_किल्ली *किल्ली = &keymap->किल्ल्या[reply->first_किल्ली_Action + i];

        FAIL_UNLESS((अचिन्हित) syms_length == wire_sym_map->width * किल्ली->अंक_groups);
        FAIL_UNLESS(wire_मोजणी == 0 || wire_मोजणी == syms_length);

        स्थिरांक xkb_keycode_t min_किल्ली_code = keymap->min_किल्ली_code;
        स्थिरांक xkb_keycode_t max_किल्ली_code = keymap->max_किल्ली_code;

        जर (wire_मोजणी != 0) {
            साठी (xkb_layout_index_t group = 0; group < किल्ली->अंक_groups; group++) {
                साठी (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    जर (level < किल्ली->groups[group].type->अंक_levels) {
                        किल्ली->groups[group].levels[level].अंक_actions = 1;
                        संघ xkb_action *action =
                            &किल्ली->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_किल्ली_code, max_किल्ली_code));

                        जर (action->type != ACTION_TYPE_NONE) {
                            किल्ली->groups[group].implicit_actions =
                                !(किल्ली->explicit & EXPLICIT_INTERP);
                        } अन्यथा जर (किल्ली->groups[group].levels[level].s.sym ==
                                   XKB_किल्ली_No_चिन्ह) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            किल्ली->groups[group].levels[level].अंक_syms = 0;
                            किल्ली->groups[group].levels[level].अंक_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                जर (किल्ली->groups[group].implicit_actions)
                    किल्ली->implicit_actions = true;
            }
        }

        acts_मोजणी_iter++;
        xcb_xkb_किल्ली_sym_map_next(&sym_maps_iter);
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_vmods(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.अंक_mods =
        अंक_REAL_MODS + MIN(msb_pos(reply->virtualMods), अंक_VMODS);
    FAIL_UNLESS(keymap->mods.अंक_mods <= XKB_MAX_MODS);

    static_assert(अंक_REAL_MODS + अंक_VMODS <= XKB_MAX_MODS, "");
    साठी (अचिन्हित i = 0; i < अंक_VMODS; i++) {
        जर (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            संरचना xkb_mod *mod = &keymap->mods.mods[अंक_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_explicits(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    साठी (पूर्णांक i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        संरचना xkb_किल्ली *किल्ली;

        FAIL_UNLESS(wire->keycode >= keymap->min_किल्ली_code &&
                    wire->keycode <= keymap->max_किल्ली_code);

        किल्ली = &keymap->किल्ल्या[wire->keycode];

        जर ((wire->explicit & XCB_XKB_EXPLICIT_किल्ली_TYPE_1) &&
            किल्ली->अंक_groups > 0) {
            किल्ली->groups[0].explicit_type = true;
            किल्ली->explicit |= EXPLICIT_TYPES;
        }
        जर ((wire->explicit & XCB_XKB_EXPLICIT_किल्ली_TYPE_2) &&
            किल्ली->अंक_groups > 1) {
            किल्ली->groups[1].explicit_type = true;
            किल्ली->explicit |= EXPLICIT_TYPES;
        }
        जर ((wire->explicit & XCB_XKB_EXPLICIT_किल्ली_TYPE_3) &&
            किल्ली->अंक_groups > 2) {
            किल्ली->groups[2].explicit_type = true;
            किल्ली->explicit |= EXPLICIT_TYPES;
        }
        जर ((wire->explicit & XCB_XKB_EXPLICIT_किल्ली_TYPE_4) &&
            किल्ली->अंक_groups > 3) {
            किल्ली->groups[3].explicit_type = true;
            किल्ली->explicit |= EXPLICIT_TYPES;
        }
        जर (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            किल्ली->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            साठी (xkb_layout_index_t l = 0; l < किल्ली->अंक_groups; l++) {
                किल्ली->groups[l].explicit_actions = true;
            }
        }
        जर (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            किल्ली->explicit |= EXPLICIT_REPEAT;
        जर (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            किल्ली->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_modmaps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_किल्ली_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    साठी (पूर्णांक i = 0; i < length; i++) {
        xcb_xkb_किल्ली_mod_map_t *wire = iter.data;
        संरचना xkb_किल्ली *किल्ली;

        FAIL_UNLESS(wire->keycode >= keymap->min_किल्ली_code &&
                    wire->keycode <= keymap->max_किल्ली_code);

        किल्ली = &keymap->किल्ल्या[wire->keycode];
        किल्ली->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .किल्ली_Code = किल्ली->keycode,
            .mods = किल्ली->modmap,
            .have_चिन्ह = false,
            .u.किल्ली_Name = किल्ली->name
        });

        xcb_xkb_किल्ली_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->अंक_modmaps);
    परत true;

fail:
    darray_free(modmaps);
    परत false;
}

स्थिर bool
get_vmodmaps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_किल्ली_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    साठी (पूर्णांक i = 0; i < length; i++) {
        xcb_xkb_किल्ली_v_mod_map_t *wire = iter.data;
        संरचना xkb_किल्ली *किल्ली;

        FAIL_UNLESS(wire->keycode >= keymap->min_किल्ली_code &&
                    wire->keycode <= keymap->max_किल्ली_code);

        किल्ली = &keymap->किल्ल्या[wire->keycode];
        किल्ली->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_किल्ली_v_mod_map_next(&iter);
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, रिक्त);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    जर ((reply->present & get_map_required_components) != get_map_required_components)
        जा fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_किल्ली_Syms,
                               reply->n_किल्ली_Actions,
                               reply->totalActions,
                               reply->total_किल्ली_Behaviors,
                               reply->virtualMods,
                               reply->total_किल्ली_Explicit,
                               reply->total_Mod_Map_किल्ल्या,
                               reply->total_VMod_Map_किल्ल्या,
                               reply->present,
                               &map);

    जर (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        जा fail;

    मुक्त(reply);
    परत true;

fail:
    मुक्त(reply);
    परत false;
}

स्थिर bool
get_indicators(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->अंक_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->अंक_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == अंक_INDICATORS, "");
    साठी (अचिन्हित i = 0; i < अंक_INDICATORS; i++) {
        जर (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            संरचना xkb_led *led = &keymap->leds[i];

            जर (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            जर (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            जर (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            जर (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            जर (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            जर (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            जर (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            जर (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            जर (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            जर (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_indicator_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, रिक्त);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    जर (!get_indicators(keymap, conn, reply))
        जा fail;

    मुक्त(reply);
    परत true;

fail:
    मुक्त(reply);
    परत false;
}

स्थिर bool
get_sym_interprets(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    पूर्णांक length = xcb_xkb_get_compat_map_si_rtrn_length(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->nTotalSI);

    keymap->अंक_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->अंक_sym_interprets);

    स्थिरांक xkb_keycode_t min_किल्ली_code = keymap->min_किल्ली_code;
    स्थिरांक xkb_keycode_t max_किल्ली_code = keymap->max_किल्ली_code;

    साठी (पूर्णांक i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        संरचना xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        निवड (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        स्थिती XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            थांबा;
        स्थिती XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            थांबा;
        स्थिती XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            थांबा;
        स्थिती XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            थांबा;
        स्थिती XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            थांबा;
        पूर्वनिर्धारित:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            जा fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        जर (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        अन्यथा
            sym_interpret->virtual_mod = अंक_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_किल्ली_code, max_किल्ली_code));
        sym_interpret->अंक_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_compat_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, रिक्त);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    जर (!get_sym_interprets(keymap, conn, reply))
        जा fail;

    मुक्त(reply);
    परत true;

fail:
    मुक्त(reply);
    परत false;
}

स्थिर bool
get_type_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_मूल्य_list_t *list)
{
    पूर्णांक किल्ली_type_names_length =
        xcb_xkb_get_names_मूल्य_list_type_names_length(reply, list);
    xcb_atom_t *किल्ली_type_names_iter =
        xcb_xkb_get_names_मूल्य_list_type_names(list);
    पूर्णांक n_levels_per_type_length =
        xcb_xkb_get_names_मूल्य_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_मूल्य_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_मूल्य_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->अंक_types);
    FAIL_UNLESS(किल्ली_type_names_length == n_levels_per_type_length);

    साठी (पूर्णांक i = 0; i < किल्ली_type_names_length; i++) {
        xcb_atom_t wire_type_name = *किल्ली_type_names_iter;
        uint8_t wire_अंक_levels = *n_levels_per_type_iter;
        संरचना xkb_किल्ली_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->अंक_levels >= wire_अंक_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->अंक_levels);
        type->अंक_level_names = type->अंक_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        साठी (माप_प्रकार j = 0; j < wire_अंक_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        साठी (माप_प्रकार j = wire_अंक_levels; j < type->अंक_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_अंक_levels;
        किल्ली_type_names_iter++;
        n_levels_per_type_iter++;
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_indicator_names(संरचना xkb_keymap *keymap,
                    संरचना x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_मूल्य_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_मूल्य_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->अंक_leds);

    साठी (अचिन्हित i = 0; i < अंक_INDICATORS; i++) {
        जर (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            संरचना xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_vmod_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_मूल्य_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_मूल्य_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.अंक_mods =
        अंक_REAL_MODS + MIN(msb_pos(reply->virtualMods), अंक_VMODS);
    FAIL_UNLESS(keymap->mods.अंक_mods <= XKB_MAX_MODS);

    static_assert(अंक_REAL_MODS + अंक_VMODS <= XKB_MAX_MODS, "");
    साठी (अचिन्हित i = 0; i < अंक_VMODS; i++) {
        जर (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            संरचना xkb_mod *mod = &keymap->mods.mods[अंक_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_group_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_मूल्य_list_t *list)
{
    पूर्णांक length = xcb_xkb_get_names_मूल्य_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_मूल्य_list_groups(list);

    keymap->अंक_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->अंक_group_names);

    साठी (पूर्णांक i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_किल्ली_names(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_मूल्य_list_t *list)
{
    पूर्णांक length = xcb_xkb_get_names_मूल्य_list_किल्ली_names_length(reply, list);
    xcb_xkb_किल्ली_name_iterator_t iter =
        xcb_xkb_get_names_मूल्य_list_किल्ली_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_किल्ली_Code == keymap->min_किल्ली_code);
    FAIL_UNLESS(reply->max_किल्ली_Code == keymap->max_किल्ली_code);
    FAIL_UNLESS(reply->first_किल्ली == keymap->min_किल्ली_code);
    FAIL_UNLESS(reply->first_किल्ली + reply->n_किल्ल्या - 1U == keymap->max_किल्ली_code);

    साठी (पूर्णांक i = 0; i < length; i++) {
        xcb_xkb_किल्ली_name_t *wire = iter.data;
        xkb_atom_t *किल्ली_name = &keymap->किल्ल्या[reply->first_किल्ली + i].name;

        जर (wire->name[0] == '\0') {
            *किल्ली_name = XKB_ATOM_NONE;
        }
        अन्यथा {
            *किल्ली_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_किल्ली_NAME_LENGTH));
            जर (!*किल्ली_name)
                परत false;
        }

        xcb_xkb_किल्ली_name_next(&iter);
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_aliases(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_मूल्य_list_t *list)
{
    पूर्णांक length = xcb_xkb_get_names_मूल्य_list_किल्ली_aliases_length(reply, list);
    xcb_xkb_किल्ली_alias_iterator_t iter =
        xcb_xkb_get_names_मूल्य_list_किल्ली_aliases_iterator(reply, list);

    keymap->अंक_किल्ली_aliases = reply->n_किल्ली_Aliases;
    ALLOC_OR_FAIL(keymap->किल्ली_aliases, keymap->अंक_किल्ली_aliases);

    साठी (पूर्णांक i = 0; i < length; i++) {
        xcb_xkb_किल्ली_alias_t *wire = iter.data;
        संरचना xkb_किल्ली_alias *alias = &keymap->किल्ली_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_किल्ली_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_किल्ली_NAME_LENGTH));
        जर (!alias->real || !alias->alias)
            जा fail;

        xcb_xkb_किल्ली_alias_next(&iter);
    }

    परत true;

fail:
    परत false;
}

स्थिर bool
get_names(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, रिक्त);
    xcb_xkb_get_names_मूल्य_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_मूल्य_list_unpack(xcb_xkb_get_names_मूल्य_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_किल्ल्या,
                                        reply->n_किल्ली_Aliases,
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
    जर (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_किल्ली_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        जा fail;

    मुक्त(reply);
    परत true;

fail:
    मुक्त(reply);
    परत false;
}

स्थिर bool
get_controls(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, रिक्त);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->अंक_Groups > 0 && reply->अंक_Groups <= 4);

    keymap->अंक_groups = reply->अंक_Groups;

    FAIL_UNLESS(keymap->max_किल्ली_code < XCB_XKB_CONST_PER_किल्ली_BIT_ARRAY_SIZE * 8);

    साठी (xkb_keycode_t i = keymap->min_किल्ली_code; i <= keymap->max_किल्ली_code; i++)
        keymap->किल्ल्या[i].repeats = (reply->per_किल्ली_Repeat[i / 8] & (1u << (i % 8)));

    मुक्त(reply);
    परत true;

fail:
    मुक्त(reply);
    परत false;
}

संरचना xkb_keymap *
xkb_x11_keymap_new_from_device(संरचना xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               गणना xkb_keymap_compile_flags flags)
{
    जर (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        परत रिक्त;
    }

    स्थिरांक गणना xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    संरचना xkb_keymap * स्थिरांक keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    जर (!keymap)
        परत रिक्त;

    keymap->redirect_किल्ली_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

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

    जर (!get_map(keymap, conn, map_cookie))
        जा err_map;
    जर (!get_indicator_map(keymap, conn, indicator_map_cookie))
        जा err_indicator_map;
    जर (!get_compat_map(keymap, conn, compat_map_cookie))
        जा err_compat_map;
    जर (!get_names(keymap, &interner, names_cookie))
        जा err_names;
    जर (!get_controls(keymap, conn, controls_cookie))
        जा err_controls;
    x11_atom_interner_round_trip(&interner);
    जर (interner.had_error)
        जा err_interner;

    परत keymap;

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
    परत रिक्त;
}
