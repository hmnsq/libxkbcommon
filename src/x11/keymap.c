/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <xcb/xkb.h>

#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon-keysyms.h"
#ଅନ୍ତର୍ଭାଵ "atom.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "x11-priv.h"

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
#define ଅଙ୍କ_REAL_MODS 8u
/* XkbNumVirtualMods. */
#define ଅଙ୍କ_VMODS 16u
/* XkbNoModifier. */
#define NO_MODIFIER 0xff
/* XkbNumIndicators. */
#define ଅଙ୍କ_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#define ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#define FAIL_UNLESS(ପଦସମଷ୍ଟି) କର {                              \
    ଯଦି (!(ପଦସମଷ୍ଟି)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(ପଦସମଷ୍ଟି));                 \
        ଯାଅ fail;                                          \
    }                                                       \
} ଯେତେବେଳେ (0)

#define FAIL_IF_BAD_REPLY(reply, request_name) କର {         \
    ଯଦି (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        ଯାଅ fail;                                          \
    }                                                       \
} ଯେତେବେଳେ (0)

#define ALLOC_OR_FAIL(arr, nmemb) କର {                      \
    ଯଦି ((nmemb) > 0) {                                      \
        (arr) = calloc((nmemb), ଆକାର(*(arr)));            \
        ଯଦି (!(arr))                                         \
            ଯାଅ fail;                                      \
    }                                                       \
} ଯେତେବେଳେ (0)

ସ୍ଥିର ଅଚଳ xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_ଚାବି_TYPES |
     XCB_XKB_MAP_PART_ଚାବି_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_ଚାବି_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

ସ୍ଥିର ଅଚଳ xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_ଚାବି_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_ଚାବି_NAMES |
     XCB_XKB_NAME_DETAIL_ଚାବି_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
ସ୍ଥିର ଅଚଳ xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_ଚାବି_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_ଚାବି_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


ସ୍ଥିର xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_ତଳ, uint16_t vmods_ଉପର)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    ଫେରନ୍ତୁ
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_ତଳ << 8) |
        ((xkb_mod_mask_t) vmods_ଉପର << 16);
}

ଗଣନା xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    ଗଣନା xkb_action_controls ret = 0;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_REPEAT_ଚାବିଗୁଡ଼ିକ)
        ret |= CONTROL_REPEAT;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_SLOW_ଚାବିଗୁଡ଼ିକ)
        ret |= CONTROL_SLOW;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_BOUNCE_ଚାବିଗୁଡ଼ିକ)
        ret |= CONTROL_DEBOUNCE;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_STICKY_ଚାବିଗୁଡ଼ିକ)
        ret |= CONTROL_STICKY_ଚାବିଗୁଡ଼ିକ;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_MOUSE_ଚାବିଗୁଡ଼ିକ)
        ret |= CONTROL_MOUSE_ଚାବିଗୁଡ଼ିକ;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_MOUSE_ଚାବିଗୁଡ଼ିକ_ACCEL)
        ret |= CONTROL_MOUSE_ଚାବିଗୁଡ଼ିକ_ACCEL;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_ଚାବିଗୁଡ଼ିକ)
        ret |= CONTROL_AX;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    ଯଦି (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର bool
translate_action(ସଂଘ xkb_action *action, ଅଚଳ xcb_xkb_action_t *wire,
                 xkb_keycode_t min_ଚାବି_code, xkb_keycode_t max_ଚାବି_code)
{
    ବାଛ (wire->type) {
    ସ୍ଥିତି XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_ତଳ_2,
                                                wire->setmods.vmods_ଉପର_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        ଯଦି (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        ଯଦି (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        ଯଦି (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_ତଳ_2,
                                                wire->latchmods.vmods_ଉପର_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        ଯଦି (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        ଯଦି (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        ଯଦି (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_ତଳ_2,
                                                wire->lockmods.vmods_ଉପର_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        ଯଦି (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        ଯଦି (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        ଯଦି (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        ଯଦି (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        ଯଦି (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        ଯଦି (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        ଯଦି (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        ଯଦି (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        ଯଦି (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        ଯଦି (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_ତଳ | ((uint16_t) wire->moveptr.x_ଉପର << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_ତଳ | ((uint16_t) wire->moveptr.y_ଉପର << 8));

        ଯଦି (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        ଯଦି (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        ଯଦି (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.ଗଣନା_2 = wire->ptrbtn.ଗଣନା_2;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.ଗଣନା_2 = 0;

        ଯଦି (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        ଯଦି (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.ମୂଲ୍ୟ = wire->setptrdflt.ମୂଲ୍ୟ;

        ଯଦି (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        ଯଦି (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        ଯଦି (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            ଅଚଳ uint16_t mask = (wire->setcontrols.bool_Ctrls_ତଳ |
                                   (wire->setcontrols.bool_Ctrls_ଉପର << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        ଭାଙ୍ଗ;
    ସ୍ଥିତି XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            ଅଚଳ uint16_t mask = (wire->lockcontrols.bool_Ctrls_ତଳ |
                                   (wire->lockcontrols.bool_Ctrls_ଉପର << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        ଭାଙ୍ଗ;

    ସ୍ଥିତି XCB_XKB_SA_TYPE_REDIRECT_ଚାବି:
        action->type = ACTION_TYPE_REDIRECT_ଚାବି;
        ଯଦି (wire->redirect.newkey < min_ଚାବି_code ||
            wire->redirect.newkey > max_ଚାବି_code)
                ଫେରନ୍ତୁ false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_ତଳ,
            wire->redirect.vmods_Mask_ଉପର
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_ତଳ_2,
                                               wire->redirect.vmods_ଉପର_2);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        ଭାଙ୍ଗ;

    /* We don't support these. */
    ସ୍ଥିତି XCB_XKB_SA_TYPE_ISO_LOCK:
    ସ୍ଥିତି XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    ସ୍ଥିତି XCB_XKB_SA_TYPE_DEVICE_BTN:
    ସ୍ଥିତି XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    ସ୍ଥିତି XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ଅଙ୍କ_ENTRIES,
                      "Missing action type");

        ଯଦି (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            ଭାଙ୍ଗ;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(ଆକାର(action->priv.data) == 7 &&
                      ଆକାର(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        memcpy(action->priv.data, wire->noaction.pad0, 7);
        ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
get_types(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା types_length = xcb_xkb_get_map_map_types_rtrn_length(reply, map);
    xcb_xkb_ଚାବି_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->ଅଙ୍କ_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->ଅଙ୍କ_types);
    ପ୍ରତିଜ୍ଞା(types_length == (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) keymap->ଅଙ୍କ_types);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < types_length; i++) {
        xcb_xkb_ଚାବି_type_t *wire_type = types_iter.data;
        ସଂରଚନା xkb_ଚାବି_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->ଅଙ୍କ_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->ଅଙ୍କ_levels = wire_type->ଅଙ୍କ_Levels;

        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା entries_length = xcb_xkb_ଚାବି_type_map_length(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_ଚାବି_type_map_iterator(wire_type);

            type->ଅଙ୍କ_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->ଅଙ୍କ_entries);
            ପ୍ରତିଜ୍ଞା(entries_length == (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) type->ଅଙ୍କ_entries);

            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < entries_length; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.data;
                ସଂରଚନା xkb_ଚାବି_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->ଅଙ୍କ_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_next(&entries_iter);
            }
        }

        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା preserves_length = xcb_xkb_ଚାବି_type_preserve_length(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_ଚାବି_type_preserve_iterator(wire_type);

            FAIL_UNLESS((unsigned) preserves_length <= type->ଅଙ୍କ_entries);

            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < preserves_length; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.data;
                ସଂରଚନା xkb_ଚାବି_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_next(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_ଚାବି_type_next(&types_iter);
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_sym_maps(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sym_maps_length = xcb_xkb_get_map_map_syms_rtrn_length(reply, map);
    xcb_xkb_ଚାବି_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_ଚାବି_Code <= reply->max_ଚାବି_Code);
    FAIL_UNLESS(reply->first_ଚାବି_Sym >= reply->min_ଚାବି_Code);
    FAIL_UNLESS(reply->first_ଚାବି_Sym + reply->n_ଚାବି_Syms <= reply->max_ଚାବି_Code + 1);

    keymap->min_ଚାବି_code = reply->min_ଚାବି_Code;
    keymap->max_ଚାବି_code = reply->max_ଚାବି_Code;
    keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ = keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ = reply->max_ଚାବି_Code + 1;

    ALLOC_OR_FAIL(keymap->ଚାବିଗୁଡ଼ିକ, keymap->max_ଚାବି_code + 1);

    ପାଇଁ (xkb_keycode_t kc = keymap->min_ଚାବି_code; kc <= keymap->max_ଚାବି_code; kc++)
        keymap->ଚାବିଗୁଡ଼ିକ[kc].keycode = kc;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < sym_maps_length; i++) {
        xcb_xkb_ଚାବି_sym_map_t *wire_sym_map = sym_maps_iter.data;
        ସଂରଚନା xkb_ଚାବି *ଚାବି = &keymap->ଚାବିଗୁଡ଼ିକ[reply->first_ଚାବି_Sym + i];

        ଚାବି->ଅଙ୍କ_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(ଚାବି->ଅଙ୍କ_groups <= ARRAY_SIZE(wire_sym_map->kt_index));
        ALLOC_OR_FAIL(ଚାବି->groups, ଚାବି->ଅଙ୍କ_groups);

        ପାଇଁ (xkb_layout_index_t j = 0; j < ଚାବି->ଅଙ୍କ_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_index[j] < keymap->ଅଙ୍କ_types);
            ଚାବି->groups[j].type = &keymap->types[wire_sym_map->kt_index[j]];

            ALLOC_OR_FAIL(ଚାବି->groups[j].levels, ଚାବି->groups[j].type->ଅଙ୍କ_levels);
        }

        ଚାବି->out_of_range_group_ସଂଖ୍ୟା = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(ଚାବି->out_of_range_group_ସଂଖ୍ୟା <= ଚାବି->ଅଙ୍କ_groups);

        ଯଦି (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            ଚାବି->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        ଅନ୍ୟଥା ଯଦି (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            ଚାବି->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        ଅନ୍ୟଥା
            ଚାବି->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା syms_length = xcb_xkb_ଚାବି_sym_map_syms_length(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_ଚାବି_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * ଚାବି->ଅଙ୍କ_groups);

            ଯଦି (syms_length > 0)
                ଚାବି->explicit |= EXPLICIT_SYMBOLS;

            ପାଇଁ (xkb_layout_index_t group = 0; group < ଚାବି->ଅଙ୍କ_groups; group++) {
                ପାଇଁ (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    ପ୍ରତିଜ୍ଞା(ଚାବି->groups[group].type != ଖାଲି);
                    ଯଦି (level < ଚାବି->groups[group].type->ଅଙ୍କ_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        ଚାବି->groups[group].levels[level].ଅଙ୍କ_syms = 1;
                        ଚାବି->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        ଚାବି->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        ଯଦି (wire_keysym != XKB_ଚାବି_No_ପ୍ରତୀକ)
                            ଚାବି->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_ଚାବି_sym_map_next(&sym_maps_iter);
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_actions(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା acts_ଗଣନା_length =
        xcb_xkb_get_map_map_acts_rtrn_ଗଣନା_length(reply, map);
    uint8_t *acts_ଗଣନା_iter = xcb_xkb_get_map_map_acts_rtrn_ଗଣନା(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_ଚାବି_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_ଚାବି_Action == keymap->min_ଚାବି_code);
    FAIL_UNLESS(reply->first_ଚାବି_Action + reply->n_ଚାବି_Actions ==
                keymap->max_ଚାବି_code + 1);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < acts_ଗଣନା_length; i++) {
        xcb_xkb_ଚାବି_sym_map_t *wire_sym_map = sym_maps_iter.data;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା syms_length = xcb_xkb_ଚାବି_sym_map_syms_length(wire_sym_map);
        uint8_t wire_ଗଣନା = *acts_ଗଣନା_iter;
        ସଂରଚନା xkb_ଚାବି *ଚାବି = &keymap->ଚାବିଗୁଡ଼ିକ[reply->first_ଚାବି_Action + i];

        FAIL_UNLESS((unsigned) syms_length == wire_sym_map->width * ଚାବି->ଅଙ୍କ_groups);
        FAIL_UNLESS(wire_ଗଣନା == 0 || wire_ଗଣନା == syms_length);

        ଅଚଳ xkb_keycode_t min_ଚାବି_code = keymap->min_ଚାବି_code;
        ଅଚଳ xkb_keycode_t max_ଚାବି_code = keymap->max_ଚାବି_code;

        ଯଦି (wire_ଗଣନା != 0) {
            ପାଇଁ (xkb_layout_index_t group = 0; group < ଚାବି->ଅଙ୍କ_groups; group++) {
                ପାଇଁ (xkb_level_index_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.data;

                    ଯଦି (level < ଚାବି->groups[group].type->ଅଙ୍କ_levels) {
                        ଚାବି->groups[group].levels[level].ଅଙ୍କ_actions = 1;
                        ସଂଘ xkb_action *action =
                            &ଚାବି->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_ଚାବି_code, max_ଚାବି_code));

                        ଯଦି (action->type != ACTION_TYPE_NONE) {
                            ଚାବି->groups[group].implicit_actions =
                                !(ଚାବି->explicit & EXPLICIT_INTERP);
                        } ଅନ୍ୟଥା ଯଦି (ଚାବି->groups[group].levels[level].s.sym ==
                                   XKB_ଚାବି_No_ପ୍ରତୀକ) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            ଚାବି->groups[group].levels[level].ଅଙ୍କ_syms = 0;
                            ଚାବି->groups[group].levels[level].ଅଙ୍କ_actions = 0;
                        }
                    }

                    xcb_xkb_action_next(&acts_iter);
                }

                ଯଦି (ଚାବି->groups[group].implicit_actions)
                    ଚାବି->implicit_actions = true;
            }
        }

        acts_ଗଣନା_iter++;
        xcb_xkb_ଚାବି_sym_map_next(&sym_maps_iter);
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_vmods(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.ଅଙ୍କ_mods =
        ଅଙ୍କ_REAL_MODS + MIN(msb_pos(reply->virtualMods), ଅଙ୍କ_VMODS);
    FAIL_UNLESS(keymap->mods.ଅଙ୍କ_mods <= XKB_MAX_MODS);

    static_assert(ଅଙ୍କ_REAL_MODS + ଅଙ୍କ_VMODS <= XKB_MAX_MODS, "");
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_VMODS; i++) {
        ଯଦି (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            ସଂରଚନା xkb_mod *mod = &keymap->mods.mods[ଅଙ୍କ_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_explicits(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = xcb_xkb_get_map_map_explicit_rtrn_length(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < length; i++) {
        xcb_xkb_set_explicit_t *wire = iter.data;
        ସଂରଚନା xkb_ଚାବି *ଚାବି;

        FAIL_UNLESS(wire->keycode >= keymap->min_ଚାବି_code &&
                    wire->keycode <= keymap->max_ଚାବି_code);

        ଚାବି = &keymap->ଚାବିଗୁଡ଼ିକ[wire->keycode];

        ଯଦି ((wire->explicit & XCB_XKB_EXPLICIT_ଚାବି_TYPE_1) &&
            ଚାବି->ଅଙ୍କ_groups > 0) {
            ଚାବି->groups[0].explicit_type = true;
            ଚାବି->explicit |= EXPLICIT_TYPES;
        }
        ଯଦି ((wire->explicit & XCB_XKB_EXPLICIT_ଚାବି_TYPE_2) &&
            ଚାବି->ଅଙ୍କ_groups > 1) {
            ଚାବି->groups[1].explicit_type = true;
            ଚାବି->explicit |= EXPLICIT_TYPES;
        }
        ଯଦି ((wire->explicit & XCB_XKB_EXPLICIT_ଚାବି_TYPE_3) &&
            ଚାବି->ଅଙ୍କ_groups > 2) {
            ଚାବି->groups[2].explicit_type = true;
            ଚାବି->explicit |= EXPLICIT_TYPES;
        }
        ଯଦି ((wire->explicit & XCB_XKB_EXPLICIT_ଚାବି_TYPE_4) &&
            ଚାବି->ଅଙ୍କ_groups > 3) {
            ଚାବି->groups[3].explicit_type = true;
            ଚାବି->explicit |= EXPLICIT_TYPES;
        }
        ଯଦି (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            ଚାବି->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            ପାଇଁ (xkb_layout_index_t l = 0; l < ଚାବି->ଅଙ୍କ_groups; l++) {
                ଚାବି->groups[l].explicit_actions = true;
            }
        }
        ଯଦି (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            ଚାବି->explicit |= EXPLICIT_REPEAT;
        ଯଦି (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            ଚାବି->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_next(&iter);
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_modmaps(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = xcb_xkb_get_map_map_modmap_rtrn_length(reply, map);
    xcb_xkb_ଚାବି_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < length; i++) {
        xcb_xkb_ଚାବି_mod_map_t *wire = iter.data;
        ସଂରଚନା xkb_ଚାବି *ଚାବି;

        FAIL_UNLESS(wire->keycode >= keymap->min_ଚାବି_code &&
                    wire->keycode <= keymap->max_ଚାବି_code);

        ଚାବି = &keymap->ଚାବିଗୁଡ଼ିକ[wire->keycode];
        ଚାବି->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .ଚାବି_Code = ଚାବି->keycode,
            .mods = ଚାବି->modmap,
            .have_ପ୍ରତୀକ = false,
            .u.ଚାବି_Name = ଚାବି->name
        });

        xcb_xkb_ଚାବି_mod_map_next(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->ଅଙ୍କ_modmaps);
    ଫେରନ୍ତୁ true;

fail:
    darray_free(modmaps);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_vmodmaps(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = xcb_xkb_get_map_map_vmodmap_rtrn_length(reply, map);
    xcb_xkb_ଚାବି_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < length; i++) {
        xcb_xkb_ଚାବି_v_mod_map_t *wire = iter.data;
        ସଂରଚନା xkb_ଚାବି *ଚାବି;

        FAIL_UNLESS(wire->keycode >= keymap->min_ଚାବି_code &&
                    wire->keycode <= keymap->max_ଚାବି_code);

        ଚାବି = &keymap->ଚାବିଗୁଡ଼ିକ[wire->keycode];
        ଚାବି->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_ଚାବି_v_mod_map_next(&iter);
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_map(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, ଖାଲି);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    ଯଦି ((reply->present & get_map_required_components) != get_map_required_components)
        ଯାଅ fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_ଚାବି_Syms,
                               reply->n_ଚାବି_Actions,
                               reply->totalActions,
                               reply->total_ଚାବି_Behaviors,
                               reply->virtualMods,
                               reply->total_ଚାବି_Explicit,
                               reply->total_Mod_Map_ଚାବିଗୁଡ଼ିକ,
                               reply->total_VMod_Map_ଚାବିଗୁଡ଼ିକ,
                               reply->present,
                               &map);

    ଯଦି (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        ଯାଅ fail;

    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ true;

fail:
    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_indicators(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->ଅଙ୍କ_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->ଅଙ୍କ_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == ଅଙ୍କ_INDICATORS, "");
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_INDICATORS; i++) {
        ଯଦି (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.data;
            ସଂରଚନା xkb_led *led = &keymap->leds[i];

            ଯଦି (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            ଯଦି (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            ଯଦି (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            ଯଦି (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            ଯଦି (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            ଯଦି (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            ଯଦି (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            ଯଦି (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            ଯଦି (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            ଯଦି (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_next(&iter);
        }
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_indicator_map(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, ଖାଲି);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    ଯଦି (!get_indicators(keymap, conn, reply))
        ଯାଅ fail;

    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ true;

fail:
    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_sym_interprets(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = xcb_xkb_get_compat_map_si_rtrn_length(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->nTotalSI);

    keymap->ଅଙ୍କ_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->ଅଙ୍କ_sym_interprets);

    ଅଚଳ xkb_keycode_t min_ଚାବି_code = keymap->min_ଚାବି_code;
    ଅଚଳ xkb_keycode_t max_ଚାବି_code = keymap->max_ଚାବି_code;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < length; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.data;
        ସଂରଚନା xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        ବାଛ (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        ସ୍ଥିତି XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            ଭାଙ୍ଗ;
        ସ୍ଥିତି XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            ଭାଙ୍ଗ;
        ସ୍ଥିତି XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            ଭାଙ୍ଗ;
        ସ୍ଥିତି XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            ଭାଙ୍ଗ;
        ସ୍ଥିତି XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            ଯାଅ fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        ଯଦି (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        ଅନ୍ୟଥା
            sym_interpret->virtual_mod = ଅଙ୍କ_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_ଚାବି_code, max_ଚାବି_code));
        sym_interpret->ଅଙ୍କ_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_next(&iter);
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_compat_map(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, ଖାଲି);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    ଯଦି (!get_sym_interprets(keymap, conn, reply))
        ଯାଅ fail;

    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ true;

fail:
    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_type_names(ସଂରଚନା xkb_keymap *keymap, ସଂରଚନା x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_ମୂଲ୍ୟ_list_t *list)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି_type_names_length =
        xcb_xkb_get_names_ମୂଲ୍ୟ_list_type_names_length(reply, list);
    xcb_atom_t *ଚାବି_type_names_iter =
        xcb_xkb_get_names_ମୂଲ୍ୟ_list_type_names(list);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_levels_per_type_length =
        xcb_xkb_get_names_ମୂଲ୍ୟ_list_n_levels_per_type_length(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_ମୂଲ୍ୟ_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_ମୂଲ୍ୟ_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->ଅଙ୍କ_types);
    FAIL_UNLESS(ଚାବି_type_names_length == n_levels_per_type_length);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଚାବି_type_names_length; i++) {
        xcb_atom_t wire_type_name = *ଚାବି_type_names_iter;
        uint8_t wire_ଅଙ୍କ_levels = *n_levels_per_type_iter;
        ସଂରଚନା xkb_ଚାବି_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->ଅଙ୍କ_levels >= wire_ଅଙ୍କ_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->ଅଙ୍କ_levels);
        type->ଅଙ୍କ_level_names = type->ଅଙ୍କ_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        ପାଇଁ (size_t j = 0; j < wire_ଅଙ୍କ_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        ପାଇଁ (size_t j = wire_ଅଙ୍କ_levels; j < type->ଅଙ୍କ_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_ଅଙ୍କ_levels;
        ଚାବି_type_names_iter++;
        n_levels_per_type_iter++;
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_indicator_names(ସଂରଚନା xkb_keymap *keymap,
                    ସଂରଚନା x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_ମୂଲ୍ୟ_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_ମୂଲ୍ୟ_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->ଅଙ୍କ_leds);

    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_INDICATORS; i++) {
        ଯଦି (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            ସଂରଚନା xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_vmod_names(ସଂରଚନା xkb_keymap *keymap, ସଂରଚନା x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_ମୂଲ୍ୟ_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_ମୂଲ୍ୟ_list_virtual_mod_names(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.ଅଙ୍କ_mods =
        ଅଙ୍କ_REAL_MODS + MIN(msb_pos(reply->virtualMods), ଅଙ୍କ_VMODS);
    FAIL_UNLESS(keymap->mods.ଅଙ୍କ_mods <= XKB_MAX_MODS);

    static_assert(ଅଙ୍କ_REAL_MODS + ଅଙ୍କ_VMODS <= XKB_MAX_MODS, "");
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_VMODS; i++) {
        ଯଦି (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            ସଂରଚନା xkb_mod *mod = &keymap->mods.mods[ଅଙ୍କ_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_group_names(ସଂରଚନା xkb_keymap *keymap, ସଂରଚନା x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_ମୂଲ୍ୟ_list_t *list)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = xcb_xkb_get_names_ମୂଲ୍ୟ_list_groups_length(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_ମୂଲ୍ୟ_list_groups(list);

    keymap->ଅଙ୍କ_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->ଅଙ୍କ_group_names);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < length; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_ଚାବି_names(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_ମୂଲ୍ୟ_list_t *list)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = xcb_xkb_get_names_ମୂଲ୍ୟ_list_ଚାବି_names_length(reply, list);
    xcb_xkb_ଚାବି_name_iterator_t iter =
        xcb_xkb_get_names_ମୂଲ୍ୟ_list_ଚାବି_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_ଚାବି_Code == keymap->min_ଚାବି_code);
    FAIL_UNLESS(reply->max_ଚାବି_Code == keymap->max_ଚାବି_code);
    FAIL_UNLESS(reply->first_ଚାବି == keymap->min_ଚାବି_code);
    FAIL_UNLESS(reply->first_ଚାବି + reply->n_ଚାବିଗୁଡ଼ିକ - 1U == keymap->max_ଚାବି_code);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < length; i++) {
        xcb_xkb_ଚାବି_name_t *wire = iter.data;
        xkb_atom_t *ଚାବି_name = &keymap->ଚାବିଗୁଡ଼ିକ[reply->first_ଚାବି + i].name;

        ଯଦି (wire->name[0] == '\0') {
            *ଚାବି_name = XKB_ATOM_NONE;
        }
        ଅନ୍ୟଥା {
            *ଚାବି_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_ଚାବି_NAME_LENGTH));
            ଯଦି (!*ଚାବି_name)
                ଫେରନ୍ତୁ false;
        }

        xcb_xkb_ଚାବି_name_next(&iter);
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_aliases(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_ମୂଲ୍ୟ_list_t *list)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା length = xcb_xkb_get_names_ମୂଲ୍ୟ_list_ଚାବି_aliases_length(reply, list);
    xcb_xkb_ଚାବି_alias_iterator_t iter =
        xcb_xkb_get_names_ମୂଲ୍ୟ_list_ଚାବି_aliases_iterator(reply, list);

    keymap->ଅଙ୍କ_ଚାବି_aliases = reply->n_ଚାବି_Aliases;
    ALLOC_OR_FAIL(keymap->ଚାବି_aliases, keymap->ଅଙ୍କ_ଚାବି_aliases);

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < length; i++) {
        xcb_xkb_ଚାବି_alias_t *wire = iter.data;
        ସଂରଚନା xkb_ଚାବି_alias *alias = &keymap->ଚାବି_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_ଚାବି_NAME_LENGTH));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_ଚାବି_NAME_LENGTH));
        ଯଦି (!alias->real || !alias->alias)
            ଯାଅ fail;

        xcb_xkb_ଚାବି_alias_next(&iter);
    }

    ଫେରନ୍ତୁ true;

fail:
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_names(ସଂରଚନା xkb_keymap *keymap, ସଂରଚନା x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, ଖାଲି);
    xcb_xkb_get_names_ମୂଲ୍ୟ_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_ମୂଲ୍ୟ_list_unpack(xcb_xkb_get_names_ମୂଲ୍ୟ_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_ଚାବିଗୁଡ଼ିକ,
                                        reply->n_ଚାବି_Aliases,
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
    ଯଦି (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_ଚାବି_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        ଯାଅ fail;

    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ true;

fail:
    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ false;
}

ସ୍ଥିର bool
get_controls(ସଂରଚନା xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, ଖାଲି);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->ଅଙ୍କ_Groups > 0 && reply->ଅଙ୍କ_Groups <= 4);

    keymap->ଅଙ୍କ_groups = reply->ଅଙ୍କ_Groups;

    FAIL_UNLESS(keymap->max_ଚାବି_code < XCB_XKB_CONST_PER_ଚାବି_BIT_ARRAY_SIZE * 8);

    ପାଇଁ (xkb_keycode_t i = keymap->min_ଚାବି_code; i <= keymap->max_ଚାବି_code; i++)
        keymap->ଚାବିଗୁଡ଼ିକ[i].repeats = (reply->per_ଚାବି_Repeat[i / 8] & (1u << (i % 8)));

    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ true;

fail:
    ମୁକ୍ତ(reply);
    ଫେରନ୍ତୁ false;
}

ସଂରଚନା xkb_keymap *
xkb_x11_keymap_new_from_device(ସଂରଚନା xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               ଗଣନା xkb_keymap_compile_flags flags)
{
    ଯଦି (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ଅଚଳ ଗଣନା xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    ସଂରଚନା xkb_keymap * ଅଚଳ keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    ଯଦି (!keymap)
        ଫେରନ୍ତୁ ଖାଲି;

    keymap->redirect_ଚାବି_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    ସଂରଚନା x11_atom_interner interner;
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

    ଯଦି (!get_map(keymap, conn, map_cookie))
        ଯାଅ err_map;
    ଯଦି (!get_indicator_map(keymap, conn, indicator_map_cookie))
        ଯାଅ err_indicator_map;
    ଯଦି (!get_compat_map(keymap, conn, compat_map_cookie))
        ଯାଅ err_compat_map;
    ଯଦି (!get_names(keymap, &interner, names_cookie))
        ଯାଅ err_names;
    ଯଦି (!get_controls(keymap, conn, controls_cookie))
        ଯାଅ err_controls;
    x11_atom_interner_round_trip(&interner);
    ଯଦି (interner.had_error)
        ଯାଅ err_interner;

    ଫେରନ୍ତୁ keymap;

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
    ଫେରନ୍ତୁ ଖାଲି;
}
