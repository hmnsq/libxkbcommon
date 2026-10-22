/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#include "config.h"

#include <assert.h>
#include <xcb/xkb.h>

#include "darray.h"
#include "xkbcommon/xkbcommon.h"
#include "xkbcommon/xkbcommon-keysyms.h"
#include "atom.h"
#include "keymap.h"
#include "x11-priv.h"

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
#define NUM_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define NUM_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define NUM_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(expr) do {                              \
    නම් (!(expr)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(expr));                 \
        goto fail;                                          \
    }                                                       \
} අතරතුර (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) do {         \
    නම් (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        goto fail;                                          \
    }                                                       \
} අතරතුර (0)

#define ALLOC_OR_FAIL(arr, nmemb) do {                      \
    නම් ((nmemb) > 0) {                                      \
        (arr) = calloc((nmemb), parimaa_na(*(arr)));            \
        නම් (!(arr))                                         \
            goto fail;                                      \
    }                                                       \
} අතරතුර (0)

static const xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_KEY_TYPES |
     XCB_XKB_MAP_PART_KEY_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_KEY_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

static const xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_KEY_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_KEY_NAMES |
     XCB_XKB_NAME_DETAIL_KEY_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
static const xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_KEY_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_KEY_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


static xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_low, uint16_t vmods_high)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    ආපසු
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_low << 8) |
        ((xkb_mod_mask_t) vmods_high << 16);
}

ga_nanaa xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    ga_nanaa xkb_action_controls ret = 0;
    නම් (wire & XCB_XKB_BOOL_CTRL_REPEAT_KEYS)
        ret |= CONTROL_REPEAT;
    නම් (wire & XCB_XKB_BOOL_CTRL_SLOW_KEYS)
        ret |= CONTROL_SLOW;
    නම් (wire & XCB_XKB_BOOL_CTRL_BOUNCE_KEYS)
        ret |= CONTROL_DEBOUNCE;
    නම් (wire & XCB_XKB_BOOL_CTRL_STICKY_KEYS)
        ret |= CONTROL_STICKY_KEYS;
    නම් (wire & XCB_XKB_BOOL_CTRL_MOUSE_KEYS)
        ret |= CONTROL_MOUSE_KEYS;
    නම් (wire & XCB_XKB_BOOL_CTRL_MOUSE_KEYS_ACCEL)
        ret |= CONTROL_MOUSE_KEYS_ACCEL;
    නම් (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_KEYS)
        ret |= CONTROL_AX;
    නම් (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    නම් (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    නම් (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    නම් (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    නම් (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    නම් (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    ආපසු ret;
}

static bool
translate_action(sa_ngha xkb_action *action, const xcb_xkb_action_t *wire,
                 xkb_keycode_t min_key_code, xkb_keycode_t max_key_code)
{
    switch (wire->type) {
    pak_sa XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmodsLow,
                                                wire->setmods.vmodsHigh);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        නම් (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        නම් (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        නම් (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmodsLow,
                                                wire->latchmods.vmodsHigh);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        නම් (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        නම් (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        නම් (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmodsLow,
                                                wire->lockmods.vmodsHigh);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        නම් (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        නම් (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        නම් (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        නම් (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        නම් (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        නම් (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        නම් (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        නම් (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        නම් (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        නම් (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.xLow | ((uint16_t) wire->moveptr.xHigh << 8));
        action->ptr.y = (int16_t) (wire->moveptr.yLow | ((uint16_t) wire->moveptr.yHigh << 8));

        නම් (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        නම් (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        නම් (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.count = wire->ptrbtn.count;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.count = 0;

        නම් (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        නම් (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.value = wire->setptrdflt.value;

        නම් (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        නම් (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        නම් (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            const uint16_t mask = (wire->setcontrols.boolCtrlsLow |
                                   (wire->setcontrols.boolCtrlsHigh << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        bha_nga;
    pak_sa XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            const uint16_t mask = (wire->lockcontrols.boolCtrlsLow |
                                   (wire->lockcontrols.boolCtrlsHigh << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        bha_nga;

    pak_sa XCB_XKB_SA_TYPE_REDIRECT_KEY:
        action->type = ACTION_TYPE_REDIRECT_KEY;
        නම් (wire->redirect.newkey < min_key_code ||
            wire->redirect.newkey > max_key_code)
                ආපසු false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmodsMaskLow,
            wire->redirect.vmodsMaskHigh
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmodsLow,
                                               wire->redirect.vmodsHigh);
        bha_nga;

    pak_sa XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        bha_nga;

    /* We don't support these. */
    pak_sa XCB_XKB_SA_TYPE_ISO_LOCK:
    pak_sa XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    pak_sa XCB_XKB_SA_TYPE_DEVICE_BTN:
    pak_sa XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    pak_sa XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        bha_nga;

    default:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == _ACTION_TYPE_NUM_ENTRIES,
                      "Missing action type");

        නම් (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            bha_nga;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(parimaa_na(action->priv.data) == 7 &&
                      parimaa_na(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        memcpy(action->priv.data, wire->noaction.pad0, 7);
        bha_nga;
    }
    ආපසු true;
}

static bool
get_types(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    පූර්ණ_සංඛ්‍යා types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_key_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->num_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->num_types);
    assert(types_length == (පූර්ණ_සංඛ්‍යා) keymap->num_types);

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < types_length; i++) {
        xcb_xkb_key_type_t *wire_type = types_iter.data;
        sa_nracanaa xkb_key_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->numLevels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->num_levels = wire_type->numLevels;

        {
            පූර්ණ_සංඛ්‍යා entries_length = xcb_xkb_key_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_key_type_map_iterator(wire_type);

            type->num_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->num_entries);
            assert(entries_length == (පූර්ණ_සංඛ්‍යා) type->num_entries);

            සඳහා (පූර්ණ_සංඛ්‍යා j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                sa_nracanaa xkb_key_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->num_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            පූර්ණ_සංඛ්‍යා preserves_length = xcb_xkb_key_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_key_type_preserve_iterator(wire_type);

            FAIL_UNLESS((unsigned) preserves_length <= type->num_entries);

            සඳහා (පූර්ණ_සංඛ්‍යා j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                sa_nracanaa xkb_key_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_key_type_next(&types_iter);
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_sym_maps(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    පූර්ණ_සංඛ්‍යා sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_key_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->minKeyCode <= reply->maxKeyCode);
    FAIL_UNLESS(reply->firstKeySym >= reply->minKeyCode);
    FAIL_UNLESS(reply->firstKeySym + reply->nKeySyms <= reply->maxKeyCode + 1);

    keymap->min_key_code = reply->minKeyCode;
    keymap->max_key_code = reply->maxKeyCode;
    keymap->num_keys = keymap->num_keys_low = reply->maxKeyCode + 1;

    ALLOC_OR_FAIL(keymap->keys, keymap->max_key_code + 1);

    සඳහා (xkb_keycode_t kc = keymap->min_key_code; kc <= keymap->max_key_code; kc++)
        keymap->keys[kc].keycode = kc;

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < sym_maps_length; i++) {
        xcb_xkb_key_sym_map_t *wire_sym_map = sym_maps_iter.data;
        sa_nracanaa xkb_key *key = &keymap->keys[reply->firstKeySym + i];

        key->num_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(key->num_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(key->groups, key->num_groups);

        සඳහා (xkb_layout_index_t j = 0; j < key->num_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->num_types);
            key->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(key->groups[j].levels, key->groups[j].type->num_levels);
        }

        key->out_of_range_group_number = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(key->out_of_range_group_number <= key->num_groups);

        නම් (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            key->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        නැතහොත් නම් (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            key->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        නැතහොත්
            key->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            පූර්ණ_සංඛ්‍යා syms_length = xcb_xkb_key_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_key_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * key->num_groups);

            නම් (syms_length > 0)
                key->explicit |= EXPLICIT_SYMBOLS;

            සඳහා (xkb_layout_index_t group = 0; group < key->num_groups; group++) {
                සඳහා (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    assert(key->groups[group].type != NULL);
                    නම් (level < key->groups[group].type->num_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        key->groups[group].levels[level].num_syms = 1;
                        key->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        key->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        නම් (wire_keysym != XKB_KEY_NoSymbol)
                            key->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_key_sym_map_next(&sym_maps_iter);
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_actions(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    පූර්ණ_සංඛ්‍යා acts_count_length =
        xcb_xkb_get_map_map_acts_rtrn_count_length(reply, map);
    uint8_t *acts_count_iter = xcb_xkb_get_map_map_acts_rtrn_count(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_key_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstKeyAction == keymap->min_key_code);
    FAIL_UNLESS(reply->firstKeyAction + reply->nKeyActions ==
                keymap->max_key_code + 1);

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < acts_count_length; i++) {
        xcb_xkb_key_sym_map_t *wire_sym_map = sym_maps_iter.data;
        පූර්ණ_සංඛ්‍යා syms_length = xcb_xkb_key_sym_map_syms_length(wire_sym_map);
        uint8_t wire_count = *acts_count_iter;
        sa_nracanaa xkb_key *key = &keymap->keys[reply->firstKeyAction + i];

        FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * key->num_groups);
        FAIL_UNLESS(wire_count == 0 || wire_count == syms_length);

        const xkb_keycode_t min_key_code = keymap->min_key_code;
        const xkb_keycode_t max_key_code = keymap->max_key_code;

        නම් (wire_count != 0) {
            සඳහා (xkb_layout_index_t group = 0; group < key->num_groups; group++) {
                සඳහා (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    නම් (level < key->groups[group].type->num_levels) {
                        key->groups[group].levels[level].num_actions = 1;
                        sa_ngha xkb_action *action =
                            &key->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_key_code, max_key_code));

                        නම් (action->type != ACTION_TYPE_NONE) {
                            key->groups[group].implicit_actions =
                                !(key->explicit & EXPLICIT_INTERP);
                        } නැතහොත් නම් (key->groups[group].levels[level].s.sym ==
                                   XKB_KEY_NoSymbol) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            key->groups[group].levels[level].num_syms = 0;
                            key->groups[group].levels[level].num_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                නම් (key->groups[group].implicit_actions)
                    key->implicit_actions = true;
            }
        }

        acts_count_iter++;
        xcb_xkb_key_sym_map_next(&sym_maps_iter);
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_vmods(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.num_mods =
        NUM_REAL_MODS + MIN(msb_pos(reply->virtualMods), NUM_VMODS);
    FAIL_UNLESS(keymap->mods.num_mods <= XKB_MAX_MODS);

    static_assert(NUM_REAL_MODS + NUM_VMODS <= XKB_MAX_MODS, "");
    සඳහා (unsigned i = 0; i < NUM_VMODS; i++) {
        නම් (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            sa_nracanaa xkb_mod *mod = &keymap->mods.mods[NUM_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_explicits(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    පූර්ණ_සංඛ්‍යා length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        sa_nracanaa xkb_key *key;

        FAIL_UNLESS(wire->keycode >= keymap->min_key_code &&
                    wire->keycode <= keymap->max_key_code);

        key = &keymap->keys[wire->keycode];

        නම් ((wire->explicit & XCB_XKB_EXPLICIT_KEY_TYPE_1) &&
            key->num_groups > 0) {
            key->groups[0].explicit_type = true;
            key->explicit |= EXPLICIT_TYPES;
        }
        නම් ((wire->explicit & XCB_XKB_EXPLICIT_KEY_TYPE_2) &&
            key->num_groups > 1) {
            key->groups[1].explicit_type = true;
            key->explicit |= EXPLICIT_TYPES;
        }
        නම් ((wire->explicit & XCB_XKB_EXPLICIT_KEY_TYPE_3) &&
            key->num_groups > 2) {
            key->groups[2].explicit_type = true;
            key->explicit |= EXPLICIT_TYPES;
        }
        නම් ((wire->explicit & XCB_XKB_EXPLICIT_KEY_TYPE_4) &&
            key->num_groups > 3) {
            key->groups[3].explicit_type = true;
            key->explicit |= EXPLICIT_TYPES;
        }
        නම් (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            key->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            සඳහා (xkb_layout_index_t l = 0; l < key->num_groups; l++) {
                key->groups[l].explicit_actions = true;
            }
        }
        නම් (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            key->explicit |= EXPLICIT_REPEAT;
        නම් (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            key->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_modmaps(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    පූර්ණ_සංඛ්‍යා length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_key_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < length; i++) {
        xcb_xkb_key_mod_map_t *wire = iter.data;
        sa_nracanaa xkb_key *key;

        FAIL_UNLESS(wire->keycode >= keymap->min_key_code &&
                    wire->keycode <= keymap->max_key_code);

        key = &keymap->keys[wire->keycode];
        key->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .keyCode = key->keycode,
            .mods = key->modmap,
            .haveSymbol = false,
            .u.keyName = key->name
        });

        xcb_xkb_key_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->num_modmaps);
    ආපසු true;

fail:
    darray_free(modmaps);
    ආපසු false;
}

static bool
get_vmodmaps(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    පූර්ණ_සංඛ්‍යා length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_key_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < length; i++) {
        xcb_xkb_key_v_mod_map_t *wire = iter.data;
        sa_nracanaa xkb_key *key;

        FAIL_UNLESS(wire->keycode >= keymap->min_key_code &&
                    wire->keycode <= keymap->max_key_code);

        key = &keymap->keys[wire->keycode];
        key->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_key_v_mod_map_next(&iter);
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_map(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, NULL);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    නම් ((reply->present & get_map_required_components) != get_map_required_components)
        goto fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->nKeySyms,
                               reply->nKeyActions,
                               reply->totalActions,
                               reply->totalKeyBehaviors,
                               reply->virtualMods,
                               reply->totalKeyExplicit,
                               reply->totalModMapKeys,
                               reply->totalVModMapKeys,
                               reply->present,
                               &map);

    නම් (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        goto fail;

    free(reply);
    ආපසු true;

fail:
    free(reply);
    ආපසු false;
}

static bool
get_indicators(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->num_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->num_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == NUM_INDICATORS, "");
    සඳහා (unsigned i = 0; i < NUM_INDICATORS; i++) {
        නම් (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            sa_nracanaa xkb_led *led = &keymap->leds[i];

            නම් (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            නම් (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            නම් (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            නම් (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            නම් (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            නම් (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            නම් (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            නම් (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            නම් (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            නම් (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_indicator_map(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, NULL);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    නම් (!get_indicators(keymap, conn, reply))
        goto fail;

    free(reply);
    ආපසු true;

fail:
    free(reply);
    ආපසු false;
}

static bool
get_sym_interprets(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    පූර්ණ_සංඛ්‍යා length = xcb_xkb_get_compat_map_si_rtrn_length(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->nTotalSI);

    keymap->num_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->num_sym_interprets);

    const xkb_keycode_t min_key_code = keymap->min_key_code;
    const xkb_keycode_t max_key_code = keymap->max_key_code;

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        sa_nracanaa xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        switch (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        pak_sa XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            bha_nga;
        pak_sa XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            bha_nga;
        pak_sa XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            bha_nga;
        pak_sa XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            bha_nga;
        pak_sa XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            bha_nga;
        default:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            goto fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        නම් (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        නැතහොත්
            sym_interpret->virtual_mod = NUM_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_key_code, max_key_code));
        sym_interpret->num_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_compat_map(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, NULL);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    නම් (!get_sym_interprets(keymap, conn, reply))
        goto fail;

    free(reply);
    ආපසු true;

fail:
    free(reply);
    ආපසු false;
}

static bool
get_type_names(sa_nracanaa xkb_keymap *keymap, sa_nracanaa x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_value_list_t *list)
{
    පූර්ණ_සංඛ්‍යා key_type_names_length =
        xcb_xkb_get_names_value_list_type_names_length(reply, list);
    xcb_atom_t *key_type_names_iter =
        xcb_xkb_get_names_value_list_type_names(list);
    පූර්ණ_සංඛ්‍යා n_levels_per_type_length =
        xcb_xkb_get_names_value_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_value_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_value_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->num_types);
    FAIL_UNLESS(key_type_names_length == n_levels_per_type_length);

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < key_type_names_length; i++) {
        xcb_atom_t wire_type_name = *key_type_names_iter;
        uint8_t wire_num_levels = *n_levels_per_type_iter;
        sa_nracanaa xkb_key_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->num_levels >= wire_num_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->num_levels);
        type->num_level_names = type->num_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        සඳහා (size_t j = 0; j < wire_num_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        සඳහා (size_t j = wire_num_levels; j < type->num_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_num_levels;
        key_type_names_iter++;
        n_levels_per_type_iter++;
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_indicator_names(sa_nracanaa xkb_keymap *keymap,
                    sa_nracanaa x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_value_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_value_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->num_leds);

    සඳහා (unsigned i = 0; i < NUM_INDICATORS; i++) {
        නම් (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            sa_nracanaa xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_vmod_names(sa_nracanaa xkb_keymap *keymap, sa_nracanaa x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_value_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_value_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.num_mods =
        NUM_REAL_MODS + MIN(msb_pos(reply->virtualMods), NUM_VMODS);
    FAIL_UNLESS(keymap->mods.num_mods <= XKB_MAX_MODS);

    static_assert(NUM_REAL_MODS + NUM_VMODS <= XKB_MAX_MODS, "");
    සඳහා (unsigned i = 0; i < NUM_VMODS; i++) {
        නම් (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            sa_nracanaa xkb_mod *mod = &keymap->mods.mods[NUM_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_group_names(sa_nracanaa xkb_keymap *keymap, sa_nracanaa x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_value_list_t *list)
{
    පූර්ණ_සංඛ්‍යා length = xcb_xkb_get_names_value_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_value_list_groups(list);

    keymap->num_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->num_group_names);

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_key_names(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_value_list_t *list)
{
    පූර්ණ_සංඛ්‍යා length = xcb_xkb_get_names_value_list_key_names_length(reply, list);
    xcb_xkb_key_name_iterator_t iter =
        xcb_xkb_get_names_value_list_key_names_iterator(reply, list);

    FAIL_UNLESS(reply->minKeyCode == keymap->min_key_code);
    FAIL_UNLESS(reply->maxKeyCode == keymap->max_key_code);
    FAIL_UNLESS(reply->firstKey == keymap->min_key_code);
    FAIL_UNLESS(reply->firstKey + reply->nKeys - 1U == keymap->max_key_code);

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < length; i++) {
        xcb_xkb_key_name_t *wire = iter.data;
        xkb_atom_t *key_name = &keymap->keys[reply->firstKey + i].name;

        නම් (wire->name[0] == '\0') {
            *key_name = XKB_ATOM_NONE;
        }
        නැතහොත් {
            *key_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_KEY_NAME_LENGTH));
            නම් (!*key_name)
                ආපසු false;
        }

        xcb_xkb_key_name_next(&iter);
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_aliases(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_value_list_t *list)
{
    පූර්ණ_සංඛ්‍යා length = xcb_xkb_get_names_value_list_key_aliases_length(reply, list);
    xcb_xkb_key_alias_iterator_t iter =
        xcb_xkb_get_names_value_list_key_aliases_iterator(reply, list);

    keymap->num_key_aliases = reply->nKeyAliases;
    ALLOC_OR_FAIL(keymap->key_aliases, keymap->num_key_aliases);

    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < length; i++) {
        xcb_xkb_key_alias_t *wire = iter.data;
        sa_nracanaa xkb_key_alias *alias = &keymap->key_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_KEY_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_KEY_NAME_LENGTH));
        නම් (!alias->real || !alias->alias)
            goto fail;

        xcb_xkb_key_alias_next(&iter);
    }

    ආපසු true;

fail:
    ආපසු false;
}

static bool
get_names(sa_nracanaa xkb_keymap *keymap, sa_nracanaa x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, NULL);
    xcb_xkb_get_names_value_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_value_list_unpack(xcb_xkb_get_names_value_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->nKeys,
                                        reply->nKeyAliases,
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
    නම් (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_key_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        goto fail;

    free(reply);
    ආපසු true;

fail:
    free(reply);
    ආපසු false;
}

static bool
get_controls(sa_nracanaa xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, NULL);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->numGroups > 0 && reply->numGroups <= 4);

    keymap->num_groups = reply->numGroups;

    FAIL_UNLESS(keymap->max_key_code < XCB_XKB_CONST_PER_KEY_BIT_ARRAY_SIZE * 8);

    සඳහා (xkb_keycode_t i = keymap->min_key_code; i <= keymap->max_key_code; i++)
        keymap->keys[i].repeats = (reply->perKeyRepeat[i / 8] & (1u << (i % 8)));

    free(reply);
    ආපසු true;

fail:
    free(reply);
    ආපසු false;
}

sa_nracanaa xkb_keymap *
xkb_x11_keymap_new_from_device(sa_nracanaa xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               ga_nanaa xkb_keymap_compile_flags flags)
{
    නම් (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        ආපසු NULL;
    }

    const ga_nanaa xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    sa_nracanaa xkb_keymap * const keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    නම් (!keymap)
        ආපසු NULL;

    keymap->redirect_key_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    sa_nracanaa x11_atom_interner interner;
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

    නම් (!get_map(keymap, conn, map_cookie))
        goto err_map;
    නම් (!get_indicator_map(keymap, conn, indicator_map_cookie))
        goto err_indicator_map;
    නම් (!get_compat_map(keymap, conn, compat_map_cookie))
        goto err_compat_map;
    නම් (!get_names(keymap, &interner, names_cookie))
        goto err_names;
    නම් (!get_controls(keymap, conn, controls_cookie))
        goto err_controls;
    x11_atom_interner_round_trip(&interner);
    නම් (interner.had_error)
        goto err_interner;

    ආපසු keymap;

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
    ආපසු NULL;
}
