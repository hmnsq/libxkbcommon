/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <xcb/xkb.h>

#شامل "darray.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcommon/xkbcommon-keysyms.h"
#شامل "atom.h"
#شامل "keymap.h"
#شامل "x11-priv.h"

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
#define رقم_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define رقم_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define رقم_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(عبارت) انجامبده {                              \
    اگر (!(عبارت)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(عبارت));                 \
        برو fail;                                          \
    }                                                       \
} وقتی (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) انجامبده {         \
    اگر (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        برو fail;                                          \
    }                                                       \
} وقتی (0)

#define ALLOC_OR_FAIL(arr, nmemb) انجامبده {                      \
    اگر ((nmemb) > 0) {                                      \
        (arr) = calloc((nmemb), اندازه(*(arr)));            \
        اگر (!(arr))                                         \
            برو fail;                                      \
    }                                                       \
} وقتی (0)

ایستا ثابت xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_کلید_TYPES |
     XCB_XKB_MAP_PART_کلید_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_کلید_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

ایستا ثابت xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_کلید_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_کلید_NAMES |
     XCB_XKB_NAME_DETAIL_کلید_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
ایستا ثابت xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_کلید_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_کلید_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


ایستا xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_کم, uint16_t vmods_بالا)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    بازگشت
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_کم << 8) |
        ((xkb_mod_mask_t) vmods_بالا << 16);
}

شمارش xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    شمارش xkb_action_controls ret = 0;
    اگر (wire & XCB_XKB_BOOL_CTRL_REPEAT_کلیدها)
        ret |= CONTROL_REPEAT;
    اگر (wire & XCB_XKB_BOOL_CTRL_SLOW_کلیدها)
        ret |= CONTROL_SLOW;
    اگر (wire & XCB_XKB_BOOL_CTRL_BOUNCE_کلیدها)
        ret |= CONTROL_DEBOUNCE;
    اگر (wire & XCB_XKB_BOOL_CTRL_STICKY_کلیدها)
        ret |= CONTROL_STICKY_کلیدها;
    اگر (wire & XCB_XKB_BOOL_CTRL_MOUSE_کلیدها)
        ret |= CONTROL_MOUSE_کلیدها;
    اگر (wire & XCB_XKB_BOOL_CTRL_MOUSE_کلیدها_ACCEL)
        ret |= CONTROL_MOUSE_کلیدها_ACCEL;
    اگر (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_کلیدها)
        ret |= CONTROL_AX;
    اگر (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    اگر (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    اگر (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    اگر (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    اگر (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    اگر (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    بازگشت ret;
}

ایستا bool
translate_action(اجتماع xkb_action *action, ثابت xcb_xkb_action_t *wire,
                 xkb_keycode_t min_کلید_code, xkb_keycode_t max_کلید_code)
{
    انتخاب (wire->type) {
    حالت XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_کم_2,
                                                wire->setmods.vmods_بالا_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        اگر (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        اگر (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        اگر (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        بشکن;
    حالت XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_کم_2,
                                                wire->latchmods.vmods_بالا_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        اگر (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        اگر (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        اگر (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        بشکن;
    حالت XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_کم_2,
                                                wire->lockmods.vmods_بالا_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        اگر (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        اگر (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        اگر (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        بشکن;
    حالت XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        اگر (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        اگر (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        اگر (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        بشکن;
    حالت XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        اگر (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        اگر (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        اگر (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        بشکن;
    حالت XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        اگر (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        بشکن;
    حالت XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_کم | ((uint16_t) wire->moveptr.x_بالا << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_کم | ((uint16_t) wire->moveptr.y_بالا << 8));

        اگر (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        اگر (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        اگر (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        بشکن;
    حالت XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.شمار = wire->ptrbtn.شمار;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        بشکن;
    حالت XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.شمار = 0;

        اگر (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        اگر (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        بشکن;
    حالت XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.مقدار = wire->setptrdflt.مقدار;

        اگر (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        بشکن;
    حالت XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        بشکن;
    حالت XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        اگر (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        اگر (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        بشکن;
    حالت XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            ثابت uint16_t mask = (wire->setcontrols.bool_Ctrls_کم |
                                   (wire->setcontrols.bool_Ctrls_بالا << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        بشکن;
    حالت XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            ثابت uint16_t mask = (wire->lockcontrols.bool_Ctrls_کم |
                                   (wire->lockcontrols.bool_Ctrls_بالا << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        بشکن;

    حالت XCB_XKB_SA_TYPE_REDIRECT_کلید:
        action->type = ACTION_TYPE_REDIRECT_کلید;
        اگر (wire->redirect.newkey < min_کلید_code ||
            wire->redirect.newkey > max_کلید_code)
                بازگشت false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_کم,
            wire->redirect.vmods_Mask_بالا
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_کم_2,
                                               wire->redirect.vmods_بالا_2);
        بشکن;

    حالت XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        بشکن;

    /* We don't support these. */
    حالت XCB_XKB_SA_TYPE_ISO_LOCK:
    حالت XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    حالت XCB_XKB_SA_TYPE_DEVICE_BTN:
    حالت XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    حالت XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        بشکن;

    پیشفرض:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_رقم_ENTRIES,
                      "Missing action type");

        اگر (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            بشکن;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(اندازه(action->priv.data) == 7 &&
                      اندازه(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        memcpy(action->priv.data, wire->noaction.pad0, 7);
        بشکن;
    }
    بازگشت true;
}

ایستا bool
get_types(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    صحیح types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_کلید_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->رقم_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->رقم_types);
    أكد(types_length == (صحیح) keymap->رقم_types);

    برای (صحیح i = 0; i < types_length; i++) {
        xcb_xkb_کلید_type_t *wire_type = types_iter.data;
        ساختار xkb_کلید_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->رقم_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->رقم_levels = wire_type->رقم_Levels;

        {
            صحیح entries_length = xcb_xkb_کلید_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_کلید_type_map_iterator(wire_type);

            type->رقم_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->رقم_entries);
            أكد(entries_length == (صحیح) type->رقم_entries);

            برای (صحیح j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                ساختار xkb_کلید_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->رقم_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            صحیح preserves_length = xcb_xkb_کلید_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_کلید_type_preserve_iterator(wire_type);

            FAIL_UNLESS((unsigned) preserves_length <= type->رقم_entries);

            برای (صحیح j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                ساختار xkb_کلید_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_کلید_type_next(&types_iter);
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_sym_maps(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    صحیح sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_کلید_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_کلید_Code <= reply->max_کلید_Code);
    FAIL_UNLESS(reply->first_کلید_Sym >= reply->min_کلید_Code);
    FAIL_UNLESS(reply->first_کلید_Sym + reply->n_کلید_Syms <= reply->max_کلید_Code + 1);

    keymap->min_کلید_code = reply->min_کلید_Code;
    keymap->max_کلید_code = reply->max_کلید_Code;
    keymap->رقم_کلیدها = keymap->رقم_کلیدها_کم = reply->max_کلید_Code + 1;

    ALLOC_OR_FAIL(keymap->کلیدها, keymap->max_کلید_code + 1);

    برای (xkb_keycode_t kc = keymap->min_کلید_code; kc <= keymap->max_کلید_code; kc++)
        keymap->کلیدها[kc].keycode = kc;

    برای (صحیح i = 0; i < sym_maps_length; i++) {
        xcb_xkb_کلید_sym_map_t *wire_sym_map = sym_maps_iter.data;
        ساختار xkb_کلید *کلید = &keymap->کلیدها[reply->first_کلید_Sym + i];

        کلید->رقم_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(کلید->رقم_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(کلید->groups, کلید->رقم_groups);

        برای (xkb_layout_index_t j = 0; j < کلید->رقم_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->رقم_types);
            کلید->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(کلید->groups[j].levels, کلید->groups[j].type->رقم_levels);
        }

        کلید->out_of_range_group_عدد = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(کلید->out_of_range_group_عدد <= کلید->رقم_groups);

        اگر (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            کلید->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        وگرنه اگر (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            کلید->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        وگرنه
            کلید->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            صحیح syms_length = xcb_xkb_کلید_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_کلید_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * کلید->رقم_groups);

            اگر (syms_length > 0)
                کلید->explicit |= EXPLICIT_SYMBOLS;

            برای (xkb_layout_index_t group = 0; group < کلید->رقم_groups; group++) {
                برای (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    أكد(کلید->groups[group].type != پوچ);
                    اگر (level < کلید->groups[group].type->رقم_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        کلید->groups[group].levels[level].رقم_syms = 1;
                        کلید->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        کلید->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        اگر (wire_keysym != XKB_کلید_No_نماد)
                            کلید->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_کلید_sym_map_next(&sym_maps_iter);
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_actions(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    صحیح acts_شمار_length =
        xcb_xkb_get_map_map_acts_rtrn_شمار_length(reply, map);
    uint8_t *acts_شمار_iter = xcb_xkb_get_map_map_acts_rtrn_شمار(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_کلید_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_کلید_Action == keymap->min_کلید_code);
    FAIL_UNLESS(reply->first_کلید_Action + reply->n_کلید_Actions ==
                keymap->max_کلید_code + 1);

    برای (صحیح i = 0; i < acts_شمار_length; i++) {
        xcb_xkb_کلید_sym_map_t *wire_sym_map = sym_maps_iter.data;
        صحیح syms_length = xcb_xkb_کلید_sym_map_syms_length(wire_sym_map);
        uint8_t wire_شمار = *acts_شمار_iter;
        ساختار xkb_کلید *کلید = &keymap->کلیدها[reply->first_کلید_Action + i];

        FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * کلید->رقم_groups);
        FAIL_UNLESS(wire_شمار == 0 || wire_شمار == syms_length);

        ثابت xkb_keycode_t min_کلید_code = keymap->min_کلید_code;
        ثابت xkb_keycode_t max_کلید_code = keymap->max_کلید_code;

        اگر (wire_شمار != 0) {
            برای (xkb_layout_index_t group = 0; group < کلید->رقم_groups; group++) {
                برای (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    اگر (level < کلید->groups[group].type->رقم_levels) {
                        کلید->groups[group].levels[level].رقم_actions = 1;
                        اجتماع xkb_action *action =
                            &کلید->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_کلید_code, max_کلید_code));

                        اگر (action->type != ACTION_TYPE_NONE) {
                            کلید->groups[group].implicit_actions =
                                !(کلید->explicit & EXPLICIT_INTERP);
                        } وگرنه اگر (کلید->groups[group].levels[level].s.sym ==
                                   XKB_کلید_No_نماد) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            کلید->groups[group].levels[level].رقم_syms = 0;
                            کلید->groups[group].levels[level].رقم_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                اگر (کلید->groups[group].implicit_actions)
                    کلید->implicit_actions = true;
            }
        }

        acts_شمار_iter++;
        xcb_xkb_کلید_sym_map_next(&sym_maps_iter);
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_vmods(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.رقم_mods =
        رقم_REAL_MODS + MIN(msb_pos(reply->virtualMods), رقم_VMODS);
    FAIL_UNLESS(keymap->mods.رقم_mods <= XKB_MAX_MODS);

    static_assert(رقم_REAL_MODS + رقم_VMODS <= XKB_MAX_MODS, "");
    برای (unsigned i = 0; i < رقم_VMODS; i++) {
        اگر (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            ساختار xkb_mod *mod = &keymap->mods.mods[رقم_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_explicits(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    صحیح length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    برای (صحیح i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        ساختار xkb_کلید *کلید;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلید_code &&
                    wire->keycode <= keymap->max_کلید_code);

        کلید = &keymap->کلیدها[wire->keycode];

        اگر ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_1) &&
            کلید->رقم_groups > 0) {
            کلید->groups[0].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        اگر ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_2) &&
            کلید->رقم_groups > 1) {
            کلید->groups[1].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        اگر ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_3) &&
            کلید->رقم_groups > 2) {
            کلید->groups[2].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        اگر ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_4) &&
            کلید->رقم_groups > 3) {
            کلید->groups[3].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        اگر (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            کلید->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            برای (xkb_layout_index_t l = 0; l < کلید->رقم_groups; l++) {
                کلید->groups[l].explicit_actions = true;
            }
        }
        اگر (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            کلید->explicit |= EXPLICIT_REPEAT;
        اگر (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            کلید->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_modmaps(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    صحیح length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_کلید_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    برای (صحیح i = 0; i < length; i++) {
        xcb_xkb_کلید_mod_map_t *wire = iter.data;
        ساختار xkb_کلید *کلید;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلید_code &&
                    wire->keycode <= keymap->max_کلید_code);

        کلید = &keymap->کلیدها[wire->keycode];
        کلید->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .کلید_Code = کلید->keycode,
            .mods = کلید->modmap,
            .have_نماد = false,
            .u.کلید_Name = کلید->name
        });

        xcb_xkb_کلید_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->رقم_modmaps);
    بازگشت true;

fail:
    darray_free(modmaps);
    بازگشت false;
}

ایستا bool
get_vmodmaps(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    صحیح length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_کلید_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    برای (صحیح i = 0; i < length; i++) {
        xcb_xkb_کلید_v_mod_map_t *wire = iter.data;
        ساختار xkb_کلید *کلید;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلید_code &&
                    wire->keycode <= keymap->max_کلید_code);

        کلید = &keymap->کلیدها[wire->keycode];
        کلید->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_کلید_v_mod_map_next(&iter);
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_map(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, پوچ);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    اگر ((reply->present & get_map_required_components) != get_map_required_components)
        برو fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_کلید_Syms,
                               reply->n_کلید_Actions,
                               reply->totalActions,
                               reply->total_کلید_Behaviors,
                               reply->virtualMods,
                               reply->total_کلید_Explicit,
                               reply->total_Mod_Map_کلیدها,
                               reply->total_VMod_Map_کلیدها,
                               reply->present,
                               &map);

    اگر (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        برو fail;

    آزاد(reply);
    بازگشت true;

fail:
    آزاد(reply);
    بازگشت false;
}

ایستا bool
get_indicators(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->رقم_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->رقم_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == رقم_INDICATORS, "");
    برای (unsigned i = 0; i < رقم_INDICATORS; i++) {
        اگر (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            ساختار xkb_led *led = &keymap->leds[i];

            اگر (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            اگر (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            اگر (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            اگر (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            اگر (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            اگر (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            اگر (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            اگر (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            اگر (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            اگر (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_indicator_map(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, پوچ);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    اگر (!get_indicators(keymap, conn, reply))
        برو fail;

    آزاد(reply);
    بازگشت true;

fail:
    آزاد(reply);
    بازگشت false;
}

ایستا bool
get_sym_interprets(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    صحیح length = xcb_xkb_get_compat_map_si_rtrn_length(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->nTotalSI);

    keymap->رقم_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->رقم_sym_interprets);

    ثابت xkb_keycode_t min_کلید_code = keymap->min_کلید_code;
    ثابت xkb_keycode_t max_کلید_code = keymap->max_کلید_code;

    برای (صحیح i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        ساختار xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        انتخاب (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        حالت XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            بشکن;
        حالت XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            بشکن;
        حالت XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            بشکن;
        حالت XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            بشکن;
        حالت XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            بشکن;
        پیشفرض:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            برو fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        اگر (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        وگرنه
            sym_interpret->virtual_mod = رقم_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_کلید_code, max_کلید_code));
        sym_interpret->رقم_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_compat_map(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, پوچ);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    اگر (!get_sym_interprets(keymap, conn, reply))
        برو fail;

    آزاد(reply);
    بازگشت true;

fail:
    آزاد(reply);
    بازگشت false;
}

ایستا bool
get_type_names(ساختار xkb_keymap *keymap, ساختار x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_مقدار_list_t *list)
{
    صحیح کلید_type_names_length =
        xcb_xkb_get_names_مقدار_list_type_names_length(reply, list);
    xcb_atom_t *کلید_type_names_iter =
        xcb_xkb_get_names_مقدار_list_type_names(list);
    صحیح n_levels_per_type_length =
        xcb_xkb_get_names_مقدار_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_مقدار_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_مقدار_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->رقم_types);
    FAIL_UNLESS(کلید_type_names_length == n_levels_per_type_length);

    برای (صحیح i = 0; i < کلید_type_names_length; i++) {
        xcb_atom_t wire_type_name = *کلید_type_names_iter;
        uint8_t wire_رقم_levels = *n_levels_per_type_iter;
        ساختار xkb_کلید_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->رقم_levels >= wire_رقم_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->رقم_levels);
        type->رقم_level_names = type->رقم_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        برای (size_t j = 0; j < wire_رقم_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        برای (size_t j = wire_رقم_levels; j < type->رقم_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_رقم_levels;
        کلید_type_names_iter++;
        n_levels_per_type_iter++;
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_indicator_names(ساختار xkb_keymap *keymap,
                    ساختار x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_مقدار_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_مقدار_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->رقم_leds);

    برای (unsigned i = 0; i < رقم_INDICATORS; i++) {
        اگر (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            ساختار xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_vmod_names(ساختار xkb_keymap *keymap, ساختار x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_مقدار_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_مقدار_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.رقم_mods =
        رقم_REAL_MODS + MIN(msb_pos(reply->virtualMods), رقم_VMODS);
    FAIL_UNLESS(keymap->mods.رقم_mods <= XKB_MAX_MODS);

    static_assert(رقم_REAL_MODS + رقم_VMODS <= XKB_MAX_MODS, "");
    برای (unsigned i = 0; i < رقم_VMODS; i++) {
        اگر (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            ساختار xkb_mod *mod = &keymap->mods.mods[رقم_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_group_names(ساختار xkb_keymap *keymap, ساختار x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_مقدار_list_t *list)
{
    صحیح length = xcb_xkb_get_names_مقدار_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_مقدار_list_groups(list);

    keymap->رقم_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->رقم_group_names);

    برای (صحیح i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_کلید_names(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_مقدار_list_t *list)
{
    صحیح length = xcb_xkb_get_names_مقدار_list_کلید_names_length(reply, list);
    xcb_xkb_کلید_name_iterator_t iter =
        xcb_xkb_get_names_مقدار_list_کلید_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_کلید_Code == keymap->min_کلید_code);
    FAIL_UNLESS(reply->max_کلید_Code == keymap->max_کلید_code);
    FAIL_UNLESS(reply->first_کلید == keymap->min_کلید_code);
    FAIL_UNLESS(reply->first_کلید + reply->n_کلیدها - 1U == keymap->max_کلید_code);

    برای (صحیح i = 0; i < length; i++) {
        xcb_xkb_کلید_name_t *wire = iter.data;
        xkb_atom_t *کلید_name = &keymap->کلیدها[reply->first_کلید + i].name;

        اگر (wire->name[0] == '\0') {
            *کلید_name = XKB_ATOM_NONE;
        }
        وگرنه {
            *کلید_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_کلید_NAME_LENGTH));
            اگر (!*کلید_name)
                بازگشت false;
        }

        xcb_xkb_کلید_name_next(&iter);
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_aliases(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_مقدار_list_t *list)
{
    صحیح length = xcb_xkb_get_names_مقدار_list_کلید_aliases_length(reply, list);
    xcb_xkb_کلید_alias_iterator_t iter =
        xcb_xkb_get_names_مقدار_list_کلید_aliases_iterator(reply, list);

    keymap->رقم_کلید_aliases = reply->n_کلید_Aliases;
    ALLOC_OR_FAIL(keymap->کلید_aliases, keymap->رقم_کلید_aliases);

    برای (صحیح i = 0; i < length; i++) {
        xcb_xkb_کلید_alias_t *wire = iter.data;
        ساختار xkb_کلید_alias *alias = &keymap->کلید_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_کلید_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_کلید_NAME_LENGTH));
        اگر (!alias->real || !alias->alias)
            برو fail;

        xcb_xkb_کلید_alias_next(&iter);
    }

    بازگشت true;

fail:
    بازگشت false;
}

ایستا bool
get_names(ساختار xkb_keymap *keymap, ساختار x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, پوچ);
    xcb_xkb_get_names_مقدار_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_مقدار_list_unpack(xcb_xkb_get_names_مقدار_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_کلیدها,
                                        reply->n_کلید_Aliases,
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
    اگر (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_کلید_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        برو fail;

    آزاد(reply);
    بازگشت true;

fail:
    آزاد(reply);
    بازگشت false;
}

ایستا bool
get_controls(ساختار xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, پوچ);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->رقم_Groups > 0 && reply->رقم_Groups <= 4);

    keymap->رقم_groups = reply->رقم_Groups;

    FAIL_UNLESS(keymap->max_کلید_code < XCB_XKB_CONST_PER_کلید_BIT_ARRAY_SIZE * 8);

    برای (xkb_keycode_t i = keymap->min_کلید_code; i <= keymap->max_کلید_code; i++)
        keymap->کلیدها[i].repeats = (reply->per_کلید_Repeat[i / 8] & (1u << (i % 8)));

    آزاد(reply);
    بازگشت true;

fail:
    آزاد(reply);
    بازگشت false;
}

ساختار xkb_keymap *
xkb_x11_keymap_new_from_device(ساختار xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               شمارش xkb_keymap_compile_flags flags)
{
    اگر (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        بازگشت پوچ;
    }

    ثابت شمارش xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    ساختار xkb_keymap * ثابت keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    اگر (!keymap)
        بازگشت پوچ;

    keymap->redirect_کلید_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    ساختار x11_atom_interner interner;
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

    اگر (!get_map(keymap, conn, map_cookie))
        برو err_map;
    اگر (!get_indicator_map(keymap, conn, indicator_map_cookie))
        برو err_indicator_map;
    اگر (!get_compat_map(keymap, conn, compat_map_cookie))
        برو err_compat_map;
    اگر (!get_names(keymap, &interner, names_cookie))
        برو err_names;
    اگر (!get_controls(keymap, conn, controls_cookie))
        برو err_controls;
    x11_atom_interner_round_trip(&interner);
    اگر (interner.had_error)
        برو err_interner;

    بازگشت keymap;

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
    بازگشت پوچ;
}
