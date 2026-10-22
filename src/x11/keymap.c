/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <xcb/xkb.h>

#समावेश "darray.h"
#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcommon/xkbcommon-keysyms.h"
#समावेश "atom.h"
#समावेश "keymap.h"
#समावेश "x11-priv.h"

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
#घोषणा अंक_REAL_MODS 8u
/* XkbNumVirtualMods. */
#घोषणा अंक_VMODS 16u
/* XkbNoModifier. */
#घोषणा NO_MODIFIER 0xff
/* XkbNumIndicators. */
#घोषणा अंक_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#घोषणा ALL_INDICATORS_MASK 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#घोषणा FAIL_UNLESS(व्यंजक) करो {                              \
    अगर (!(व्यंजक)) {                                          \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(व्यंजक));                 \
        जाओ fail;                                          \
    }                                                       \
} जबतक (0)

#घोषणा FAIL_IF_BAD_REPLY(reply, request_name) करो {         \
    अगर (!(reply)) {                                           \
        log_err(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        जाओ fail;                                          \
    }                                                       \
} जबतक (0)

#घोषणा ALLOC_OR_FAIL(arr, nmemb) करो {                      \
    अगर ((nmemb) > 0) {                                      \
        (arr) = सुस्मृति((nmemb), माप(*(arr)));            \
        अगर (!(arr))                                         \
            जाओ fail;                                      \
    }                                                       \
} जबतक (0)

अटल स्थिर xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_कुंजी_TYPES |
     XCB_XKB_MAP_PART_कुंजी_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_कुंजी_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

अटल स्थिर xcb_xkb_name_detail_t get_names_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_कुंजी_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_INDICATOR_NAMES |
     XCB_XKB_NAME_DETAIL_कुंजी_NAMES |
     XCB_XKB_NAME_DETAIL_कुंजी_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES |
     XCB_XKB_NAME_DETAIL_GROUP_NAMES);
अटल स्थिर xcb_xkb_name_detail_t get_names_required =
    (XCB_XKB_NAME_DETAIL_कुंजी_TYPE_NAMES |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_NAMES |
     XCB_XKB_NAME_DETAIL_कुंजी_NAMES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_NAMES);


अटल xkb_mod_mask_t
translate_mods(uint8_t rmods, uint16_t vmods_निचला, uint16_t vmods_ऊपरी)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    वापस
        ((xkb_mod_mask_t) rmods) |
        ((xkb_mod_mask_t) vmods_निचला << 8) |
        ((xkb_mod_mask_t) vmods_ऊपरी << 16);
}

क्रमागत xkb_action_controls
translate_controls_mask(uint32_t wire)
{
    क्रमागत xkb_action_controls ret = 0;
    अगर (wire & XCB_XKB_BOOL_CTRL_REPEAT_कुंजियाँ)
        ret |= CONTROL_REPEAT;
    अगर (wire & XCB_XKB_BOOL_CTRL_SLOW_कुंजियाँ)
        ret |= CONTROL_SLOW;
    अगर (wire & XCB_XKB_BOOL_CTRL_BOUNCE_कुंजियाँ)
        ret |= CONTROL_DEBOUNCE;
    अगर (wire & XCB_XKB_BOOL_CTRL_STICKY_कुंजियाँ)
        ret |= CONTROL_STICKY_कुंजियाँ;
    अगर (wire & XCB_XKB_BOOL_CTRL_MOUSE_कुंजियाँ)
        ret |= CONTROL_MOUSE_कुंजियाँ;
    अगर (wire & XCB_XKB_BOOL_CTRL_MOUSE_कुंजियाँ_ACCEL)
        ret |= CONTROL_MOUSE_कुंजियाँ_ACCEL;
    अगर (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_कुंजियाँ)
        ret |= CONTROL_AX;
    अगर (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_MASK)
        ret |= CONTROL_AX_TIMEOUT;
    अगर (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_MASK)
        ret |= CONTROL_AX_FEEDBACK;
    अगर (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_MASK)
        ret |= CONTROL_BELL;
    अगर (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_MASK)
        ret |= CONTROL_OVERLAY1;
    अगर (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_MASK)
        ret |= CONTROL_OVERLAY2;
    अगर (wire & XCB_XKB_BOOL_CTRL_IGNORE_GROUP_LOCK_MASK)
        ret |= CONTROL_IGNORE_GROUP_LOCK;
    /* Some controls are not supported and don't appear here. */
    वापस ret;
}

अटल bool
translate_action(जोड़ xkb_action *action, स्थिर xcb_xkb_action_t *wire,
                 xkb_keycode_t min_कुंजी_code, xkb_keycode_t max_कुंजी_code)
{
    चयन (wire->type) {
    हाल XCB_XKB_SA_TYPE_SET_MODS:
        action->type = ACTION_TYPE_MOD_SET;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_निचला_2,
                                                wire->setmods.vmods_ऊपरी_2);
        action->mods.mods.mask = translate_mods(wire->setmods.mask, 0, 0);

        अगर (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        अगर (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        अगर (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_निचला_2,
                                                wire->latchmods.vmods_ऊपरी_2);
        action->mods.mods.mask = translate_mods(wire->latchmods.mask, 0, 0);

        अगर (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_LOCK_CLEAR;
        अगर (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->mods.flags |= ACTION_LATCH_TO_LOCK;
        अगर (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_LOCK_MODS:
        action->type = ACTION_TYPE_MOD_LOCK;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_निचला_2,
                                                wire->lockmods.vmods_ऊपरी_2);
        action->mods.mods.mask = translate_mods(wire->lockmods.mask, 0, 0);

        अगर (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->mods.flags |= ACTION_LOCK_NO_LOCK;
        अगर (wire->lockmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_LOCK_NO_UNLOCK;
        अगर (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_SET_GROUP:
        action->type = ACTION_TYPE_GROUP_SET;

        action->group.group = (int32_t)wire->setgroup.group;

        अगर (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        अगर (wire->setmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        अगर (wire->setmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_LATCH_GROUP:
        action->type = ACTION_TYPE_GROUP_LATCH;

        action->group.group = (int32_t)wire->latchgroup.group;

        अगर (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->group.flags |= ACTION_LOCK_CLEAR;
        अगर (wire->latchmods.flags & XCB_XKB_SA_LATCH_TO_LOCK)
            action->group.flags |= ACTION_LATCH_TO_LOCK;
        अगर (wire->latchmods.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_LOCK_GROUP:
        action->type = ACTION_TYPE_GROUP_LOCK;

        action->group.group = (int32_t)wire->lockgroup.group;

        अगर (wire->lockgroup.flags & XCB_XKB_SA_ISO_LOCK_FLAG_GROUP_ABSOLUTE)
            action->group.flags |= ACTION_ABSOLUTE_SWITCH;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_निचला | ((uint16_t) wire->moveptr.x_ऊपरी << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_निचला | ((uint16_t) wire->moveptr.y_ऊपरी << 8));

        अगर (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        अगर (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        अगर (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.गिनती = wire->ptrbtn.गिनती;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_LOCK_PTR_BTN:
        action->type = ACTION_TYPE_PTR_LOCK;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.गिनती = 0;

        अगर (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_LOCK)
            action->btn.flags |= ACTION_LOCK_NO_LOCK;
        अगर (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_LOCK_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_LOCK_NO_UNLOCK;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_SET_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.मूल्य = wire->setptrdflt.मूल्य;

        अगर (wire->setptrdflt.flags & XCB_XKB_SA_SET_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        अगर (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        अगर (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        अवरोध;
    हाल XCB_XKB_SA_TYPE_SET_CONTROLS:
        action->type = ACTION_TYPE_CTRL_SET;
        {
            स्थिर uint16_t mask = (wire->setcontrols.bool_Ctrls_निचला |
                                   (wire->setcontrols.bool_Ctrls_ऊपरी << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        अवरोध;
    हाल XCB_XKB_SA_TYPE_LOCK_CONTROLS:
        action->type = ACTION_TYPE_CTRL_LOCK;
        {
            स्थिर uint16_t mask = (wire->lockcontrols.bool_Ctrls_निचला |
                                   (wire->lockcontrols.bool_Ctrls_ऊपरी << 8));
            action->ctrls.ctrls = translate_controls_mask(mask);
        }
        अवरोध;

    हाल XCB_XKB_SA_TYPE_REDIRECT_कुंजी:
        action->type = ACTION_TYPE_REDIRECT_कुंजी;
        अगर (wire->redirect.newkey < min_कुंजी_code ||
            wire->redirect.newkey > max_कुंजी_code)
                वापस false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.mask,
            wire->redirect.vmods_Mask_निचला,
            wire->redirect.vmods_Mask_ऊपरी
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_निचला_2,
                                               wire->redirect.vmods_ऊपरी_2);
        अवरोध;

    हाल XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        अवरोध;

    /* We don't support these. */
    हाल XCB_XKB_SA_TYPE_ISO_LOCK:
    हाल XCB_XKB_SA_TYPE_ACTION_MESSAGE:
    हाल XCB_XKB_SA_TYPE_DEVICE_BTN:
    हाल XCB_XKB_SA_TYPE_LOCK_DEVICE_BTN:
    हाल XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        अवरोध;

    शेष:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
                      "Missing action type");

        अगर (wire->type < ACTION_TYPE_PRIVATE) {
            action->type = ACTION_TYPE_NONE;
            अवरोध;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(माप(action->priv.तथ्य) == 7 &&
                      माप(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        स_नकल(action->priv.तथ्य, wire->noaction.pad0, 7);
        अवरोध;
    }
    वापस true;
}

अटल bool
get_types(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक types_लंबाई = xcb_xkb_get_map_map_types_rtrn_लंबाई(reply, map);
    xcb_xkb_कुंजी_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->firstType == 0);

    keymap->अंक_types = reply->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->अंक_types);
    निश्चित(types_लंबाई == (पूर्णांक) keymap->अंक_types);

    क्रम (पूर्णांक i = 0; i < types_लंबाई; i++) {
        xcb_xkb_कुंजी_type_t *wire_type = types_iter.तथ्य;
        काष्ठा xkb_कुंजी_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->अंक_Levels > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.mask = translate_mods(wire_type->mods_mask, 0, 0);
        type->अंक_levels = wire_type->अंक_Levels;

        {
            पूर्णांक entries_लंबाई = xcb_xkb_कुंजी_type_map_लंबाई(wire_type);
            xcb_xkb_kt_map_entry_iterator_t entries_iter =
                xcb_xkb_कुंजी_type_map_iterator(wire_type);

            type->अंक_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->अंक_entries);
            निश्चित(entries_लंबाई == (पूर्णांक) type->अंक_entries);

            क्रम (पूर्णांक j = 0; j < entries_लंबाई; j++) {
                xcb_xkb_kt_map_entry_t *wire_entry = entries_iter.तथ्य;
                काष्ठा xkb_कुंजी_type_entry *entry = &type->entries[j];

                FAIL_UNLESS(wire_entry->level < type->अंक_levels);

                entry->level = wire_entry->level;
                entry->mods.mods = translate_mods(wire_entry->mods_mods,
                                                  wire_entry->mods_vmods, 0);
                entry->mods.mask = translate_mods(wire_entry->mods_mask, 0, 0);

                xcb_xkb_kt_map_entry_अग्र(&entries_iter);
            }
        }

        {
            पूर्णांक preserves_लंबाई = xcb_xkb_कुंजी_type_preserve_लंबाई(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_कुंजी_type_preserve_iterator(wire_type);

            FAIL_UNLESS((अचिन्हित) preserves_लंबाई <= type->अंक_entries);

            क्रम (पूर्णांक j = 0; j < preserves_लंबाई; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.तथ्य;
                काष्ठा xkb_कुंजी_type_entry *entry = &type->entries[j];

                entry->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                entry->preserve.mask = translate_mods(wire_preserve->mask, 0, 0);

                xcb_xkb_mod_def_अग्र(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_कुंजी_type_अग्र(&types_iter);
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_sym_maps(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक sym_maps_लंबाई = xcb_xkb_get_map_map_syms_rtrn_लंबाई(reply, map);
    xcb_xkb_कुंजी_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->min_कुंजी_Code <= reply->max_कुंजी_Code);
    FAIL_UNLESS(reply->first_कुंजी_Sym >= reply->min_कुंजी_Code);
    FAIL_UNLESS(reply->first_कुंजी_Sym + reply->n_कुंजी_Syms <= reply->max_कुंजी_Code + 1);

    keymap->min_कुंजी_code = reply->min_कुंजी_Code;
    keymap->max_कुंजी_code = reply->max_कुंजी_Code;
    keymap->अंक_कुंजियाँ = keymap->अंक_कुंजियाँ_निचला = reply->max_कुंजी_Code + 1;

    ALLOC_OR_FAIL(keymap->कुंजियाँ, keymap->max_कुंजी_code + 1);

    क्रम (xkb_keycode_t kc = keymap->min_कुंजी_code; kc <= keymap->max_कुंजी_code; kc++)
        keymap->कुंजियाँ[kc].keycode = kc;

    क्रम (पूर्णांक i = 0; i < sym_maps_लंबाई; i++) {
        xcb_xkb_कुंजी_sym_map_t *wire_sym_map = sym_maps_iter.तथ्य;
        काष्ठा xkb_कुंजी *कुंजी = &keymap->कुंजियाँ[reply->first_कुंजी_Sym + i];

        कुंजी->अंक_groups = wire_sym_map->groupInfo & 0x0f;
        FAIL_UNLESS(कुंजी->अंक_groups <= ARRAY_आकार(wire_sym_map->kt_सूचकांक));
        ALLOC_OR_FAIL(कुंजी->groups, कुंजी->अंक_groups);

        क्रम (xkb_layout_सूचकांक_t j = 0; j < कुंजी->अंक_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_सूचकांक[j] < keymap->अंक_types);
            कुंजी->groups[j].type = &keymap->types[wire_sym_map->kt_सूचकांक[j]];

            ALLOC_OR_FAIL(कुंजी->groups[j].levels, कुंजी->groups[j].type->अंक_levels);
        }

        कुंजी->out_of_range_group_संख्या = (wire_sym_map->groupInfo & 0x30) >> 4;

        FAIL_UNLESS(कुंजी->out_of_range_group_संख्या <= कुंजी->अंक_groups);

        अगर (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            कुंजी->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_CLAMP;
        अन्यथा अगर (wire_sym_map->groupInfo & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            कुंजी->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_REDIRECT;
        अन्यथा
            कुंजी->out_of_range_group_policy = XKB_LAYOUT_OUT_OF_RANGE_WRAP;

        {
            पूर्णांक syms_लंबाई = xcb_xkb_कुंजी_sym_map_syms_लंबाई(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_कुंजी_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((अचिन्हित) syms_लंबाई == wire_sym_map->width * कुंजी->अंक_groups);

            अगर (syms_लंबाई > 0)
                कुंजी->explicit |= EXPLICIT_SYMBOLS;

            क्रम (xkb_layout_सूचकांक_t group = 0; group < कुंजी->अंक_groups; group++) {
                क्रम (xkb_level_सूचकांक_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    निश्चित(कुंजी->groups[group].type != शून्य);
                    अगर (level < कुंजी->groups[group].type->अंक_levels) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        कुंजी->groups[group].levels[level].अंक_syms = 1;
                        कुंजी->groups[group].levels[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        कुंजी->groups[group].levels[level].upper =
                            xkb_keysym_to_upper(wire_keysym);
                        अगर (wire_keysym != XKB_कुंजी_No_प्रतीक)
                            कुंजी->groups[group].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_कुंजी_sym_map_अग्र(&sym_maps_iter);
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_actions(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक acts_गिनती_लंबाई =
        xcb_xkb_get_map_map_acts_rtrn_गिनती_लंबाई(reply, map);
    uint8_t *acts_गिनती_iter = xcb_xkb_get_map_map_acts_rtrn_गिनती(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(reply, map);
    xcb_xkb_कुंजी_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(reply, map);

    FAIL_UNLESS(reply->first_कुंजी_Action == keymap->min_कुंजी_code);
    FAIL_UNLESS(reply->first_कुंजी_Action + reply->n_कुंजी_Actions ==
                keymap->max_कुंजी_code + 1);

    क्रम (पूर्णांक i = 0; i < acts_गिनती_लंबाई; i++) {
        xcb_xkb_कुंजी_sym_map_t *wire_sym_map = sym_maps_iter.तथ्य;
        पूर्णांक syms_लंबाई = xcb_xkb_कुंजी_sym_map_syms_लंबाई(wire_sym_map);
        uint8_t wire_गिनती = *acts_गिनती_iter;
        काष्ठा xkb_कुंजी *कुंजी = &keymap->कुंजियाँ[reply->first_कुंजी_Action + i];

        FAIL_UNLESS((अचिन्हित) syms_लंबाई == wire_sym_map->width * कुंजी->अंक_groups);
        FAIL_UNLESS(wire_गिनती == 0 || wire_गिनती == syms_लंबाई);

        स्थिर xkb_keycode_t min_कुंजी_code = keymap->min_कुंजी_code;
        स्थिर xkb_keycode_t max_कुंजी_code = keymap->max_कुंजी_code;

        अगर (wire_गिनती != 0) {
            क्रम (xkb_layout_सूचकांक_t group = 0; group < कुंजी->अंक_groups; group++) {
                क्रम (xkb_level_सूचकांक_t level = 0; level < wire_sym_map->width; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.तथ्य;

                    अगर (level < कुंजी->groups[group].type->अंक_levels) {
                        कुंजी->groups[group].levels[level].अंक_actions = 1;
                        जोड़ xkb_action *action =
                            &कुंजी->groups[group].levels[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_कुंजी_code, max_कुंजी_code));

                        अगर (action->type != ACTION_TYPE_NONE) {
                            कुंजी->groups[group].implicit_actions =
                                !(कुंजी->explicit & EXPLICIT_INTERP);
                        } अन्यथा अगर (कुंजी->groups[group].levels[level].s.sym ==
                                   XKB_कुंजी_No_प्रतीक) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            कुंजी->groups[group].levels[level].अंक_syms = 0;
                            कुंजी->groups[group].levels[level].अंक_actions = 0;
                        }
                    }

                    xcb_xkb_action_अग्र(&acts_iter);
                }

                अगर (कुंजी->groups[group].implicit_actions)
                    कुंजी->implicit_actions = true;
            }
        }

        acts_गिनती_iter++;
        xcb_xkb_कुंजी_sym_map_अग्र(&sym_maps_iter);
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_vmods(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.अंक_mods =
        अंक_REAL_MODS + MIN(msb_pos(reply->virtualMods), अंक_VMODS);
    FAIL_UNLESS(keymap->mods.अंक_mods <= XKB_MAX_MODS);

    static_assert(अंक_REAL_MODS + अंक_VMODS <= XKB_MAX_MODS, "");
    क्रम (अचिन्हित i = 0; i < अंक_VMODS; i++) {
        अगर (reply->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            काष्ठा xkb_mod *mod = &keymap->mods.mods[अंक_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_explicits(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक लंबाई = xcb_xkb_get_map_map_explicit_rtrn_लंबाई(reply, map);
    xcb_xkb_set_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(reply, map);

    क्रम (पूर्णांक i = 0; i < लंबाई; i++) {
        xcb_xkb_set_explicit_t *wire = iter.तथ्य;
        काष्ठा xkb_कुंजी *कुंजी;

        FAIL_UNLESS(wire->keycode >= keymap->min_कुंजी_code &&
                    wire->keycode <= keymap->max_कुंजी_code);

        कुंजी = &keymap->कुंजियाँ[wire->keycode];

        अगर ((wire->explicit & XCB_XKB_EXPLICIT_कुंजी_TYPE_1) &&
            कुंजी->अंक_groups > 0) {
            कुंजी->groups[0].explicit_type = true;
            कुंजी->explicit |= EXPLICIT_TYPES;
        }
        अगर ((wire->explicit & XCB_XKB_EXPLICIT_कुंजी_TYPE_2) &&
            कुंजी->अंक_groups > 1) {
            कुंजी->groups[1].explicit_type = true;
            कुंजी->explicit |= EXPLICIT_TYPES;
        }
        अगर ((wire->explicit & XCB_XKB_EXPLICIT_कुंजी_TYPE_3) &&
            कुंजी->अंक_groups > 2) {
            कुंजी->groups[2].explicit_type = true;
            कुंजी->explicit |= EXPLICIT_TYPES;
        }
        अगर ((wire->explicit & XCB_XKB_EXPLICIT_कुंजी_TYPE_4) &&
            कुंजी->अंक_groups > 3) {
            कुंजी->groups[3].explicit_type = true;
            कुंजी->explicit |= EXPLICIT_TYPES;
        }
        अगर (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            कुंजी->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            क्रम (xkb_layout_सूचकांक_t l = 0; l < कुंजी->अंक_groups; l++) {
                कुंजी->groups[l].explicit_actions = true;
            }
        }
        अगर (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            कुंजी->explicit |= EXPLICIT_REPEAT;
        अगर (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            कुंजी->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_set_explicit_अग्र(&iter);
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_modmaps(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक लंबाई = xcb_xkb_get_map_map_modmap_rtrn_लंबाई(reply, map);
    xcb_xkb_कुंजी_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(reply, map);

    darray(ModMapEntry) modmaps = darray_new();

    क्रम (पूर्णांक i = 0; i < लंबाई; i++) {
        xcb_xkb_कुंजी_mod_map_t *wire = iter.तथ्य;
        काष्ठा xkb_कुंजी *कुंजी;

        FAIL_UNLESS(wire->keycode >= keymap->min_कुंजी_code &&
                    wire->keycode <= keymap->max_कुंजी_code);

        कुंजी = &keymap->कुंजियाँ[wire->keycode];
        कुंजी->modmap = wire->mods;

        darray_append(modmaps, (ModMapEntry) {
            .कुंजी_Code = कुंजी->keycode,
            .mods = कुंजी->modmap,
            .have_प्रतीक = false,
            .u.कुंजी_Name = कुंजी->name
        });

        xcb_xkb_कुंजी_mod_map_अग्र(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->अंक_modmaps);
    वापस true;

fail:
    darray_free(modmaps);
    वापस false;
}

अटल bool
get_vmodmaps(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_reply_t *reply, xcb_xkb_get_map_map_t *map)
{
    पूर्णांक लंबाई = xcb_xkb_get_map_map_vmodmap_rtrn_लंबाई(reply, map);
    xcb_xkb_कुंजी_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(reply, map);

    क्रम (पूर्णांक i = 0; i < लंबाई; i++) {
        xcb_xkb_कुंजी_v_mod_map_t *wire = iter.तथ्य;
        काष्ठा xkb_कुंजी *कुंजी;

        FAIL_UNLESS(wire->keycode >= keymap->min_कुंजी_code &&
                    wire->keycode <= keymap->max_कुंजी_code);

        कुंजी = &keymap->कुंजियाँ[wire->keycode];
        कुंजी->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_कुंजी_v_mod_map_अग्र(&iter);
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_map(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_reply_t *reply = xcb_xkb_get_map_reply(conn, cookie, शून्य);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_REPLY(reply, "XkbGetMap");

    अगर ((reply->present & get_map_required_components) != get_map_required_components)
        जाओ fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(reply),
                               reply->nTypes,
                               reply->n_कुंजी_Syms,
                               reply->n_कुंजी_Actions,
                               reply->योग_Actions,
                               reply->योग_कुंजी_Behaviors,
                               reply->virtualMods,
                               reply->योग_कुंजी_Explicit,
                               reply->योग_Mod_Map_कुंजियाँ,
                               reply->योग_VMod_Map_कुंजियाँ,
                               reply->present,
                               &map);

    अगर (!get_types(keymap, conn, reply, &map) ||
        !get_sym_maps(keymap, conn, reply, &map) ||
        !get_actions(keymap, conn, reply, &map) ||
        !get_vmods(keymap, conn, reply, &map) ||
        !get_explicits(keymap, conn, reply, &map) ||
        !get_modmaps(keymap, conn, reply, &map) ||
        !get_vmodmaps(keymap, conn, reply, &map))
        जाओ fail;

    मुक्त(reply);
    वापस true;

fail:
    मुक्त(reply);
    वापस false;
}

अटल bool
get_indicators(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_reply_t *reply)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(reply);

    keymap->अंक_leds = msb_pos(reply->which);
    FAIL_UNLESS(keymap->अंक_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == अंक_INDICATORS, "");
    क्रम (अचिन्हित i = 0; i < अंक_INDICATORS; i++) {
        अगर (reply->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.तथ्य;
            काष्ठा xkb_led *led = &keymap->leds[i];

            अगर (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_BASE)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            अगर (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            अगर (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            अगर (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            अगर (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            अगर (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_BASE)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            अगर (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            अगर (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            अगर (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            अगर (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.mask = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_mask(wire->ctrls);

            xcb_xkb_indicator_map_अग्र(&iter);
        }
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_indicator_map(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_reply_t *reply =
        xcb_xkb_get_indicator_map_reply(conn, cookie, शून्य);

    FAIL_IF_BAD_REPLY(reply, "XkbGetIndicatorMap");

    अगर (!get_indicators(keymap, conn, reply))
        जाओ fail;

    मुक्त(reply);
    वापस true;

fail:
    मुक्त(reply);
    वापस false;
}

अटल bool
get_sym_interprets(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_reply_t *reply)
{
    पूर्णांक लंबाई = xcb_xkb_get_compat_map_si_rtrn_लंबाई(reply);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(reply);

    FAIL_UNLESS(reply->firstSIRtrn == 0);
    FAIL_UNLESS(reply->nSIRtrn == reply->n_योग_SI);

    keymap->अंक_sym_interprets = reply->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->अंक_sym_interprets);

    स्थिर xkb_keycode_t min_कुंजी_code = keymap->min_कुंजी_code;
    स्थिर xkb_keycode_t max_कुंजी_code = keymap->max_कुंजी_code;

    क्रम (पूर्णांक i = 0; i < लंबाई; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.तथ्य;
        काष्ठा xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        चयन (wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK) {
        हाल XCB_XKB_SYM_INTERPRET_MATCH_NONE_OF:
            sym_interpret->match = MATCH_NONE;
            अवरोध;
        हाल XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF_OR_NONE:
            sym_interpret->match = MATCH_ANY_OR_NONE;
            अवरोध;
        हाल XCB_XKB_SYM_INTERPRET_MATCH_ANY_OF:
            sym_interpret->match = MATCH_ANY;
            अवरोध;
        हाल XCB_XKB_SYM_INTERPRET_MATCH_ALL_OF:
            sym_interpret->match = MATCH_ALL;
            अवरोध;
        हाल XCB_XKB_SYM_INTERPRET_MATCH_EXACTLY:
            sym_interpret->match = MATCH_EXACTLY;
            अवरोध;
        शेष:
            log_err_func(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->match & XCB_XKB_SYM_INTERP_MATCH_OP_MASK);
            जाओ fail;
        }

        sym_interpret->level_one_only =
            (wire->match & XCB_XKB_SYM_INTERP_MATCH_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        अगर (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        अन्यथा
            sym_interpret->virtual_mod = अंक_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_कुंजी_code, max_कुंजी_code));
        sym_interpret->अंक_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_अग्र(&iter);
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_compat_map(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_reply_t *reply =
        xcb_xkb_get_compat_map_reply(conn, cookie, शून्य);

    FAIL_IF_BAD_REPLY(reply, "XkbGetCompatMap");

    अगर (!get_sym_interprets(keymap, conn, reply))
        जाओ fail;

    मुक्त(reply);
    वापस true;

fail:
    मुक्त(reply);
    वापस false;
}

अटल bool
get_type_names(काष्ठा xkb_keymap *keymap, काष्ठा x11_atom_interner *interner,
               xcb_xkb_get_names_reply_t *reply,
               xcb_xkb_get_names_मूल्य_list_t *list)
{
    पूर्णांक कुंजी_type_names_लंबाई =
        xcb_xkb_get_names_मूल्य_list_type_names_लंबाई(reply, list);
    xcb_atom_t *कुंजी_type_names_iter =
        xcb_xkb_get_names_मूल्य_list_type_names(list);
    पूर्णांक n_levels_per_type_लंबाई =
        xcb_xkb_get_names_मूल्य_list_n_levels_per_type_लंबाई(reply, list);
    uint8_t *n_levels_per_type_iter =
        xcb_xkb_get_names_मूल्य_list_n_levels_per_type(list);
    xcb_atom_t *kt_level_names_iter =
        xcb_xkb_get_names_मूल्य_list_kt_level_names(list);

    FAIL_UNLESS(reply->nTypes == keymap->अंक_types);
    FAIL_UNLESS(कुंजी_type_names_लंबाई == n_levels_per_type_लंबाई);

    क्रम (पूर्णांक i = 0; i < कुंजी_type_names_लंबाई; i++) {
        xcb_atom_t wire_type_name = *कुंजी_type_names_iter;
        uint8_t wire_अंक_levels = *n_levels_per_type_iter;
        काष्ठा xkb_कुंजी_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->अंक_levels >= wire_अंक_levels);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_names, type->अंक_levels);
        type->अंक_level_names = type->अंक_levels;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        क्रम (माप_प्रकार j = 0; j < wire_अंक_levels; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_names_iter[j],
                                         &type->level_names[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        क्रम (माप_प्रकार j = wire_अंक_levels; j < type->अंक_levels; j++)
            type->level_names[j] = XKB_ATOM_NONE;

        kt_level_names_iter += wire_अंक_levels;
        कुंजी_type_names_iter++;
        n_levels_per_type_iter++;
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_indicator_names(काष्ठा xkb_keymap *keymap,
                    काष्ठा x11_atom_interner *interner,
                    xcb_xkb_get_names_reply_t *reply,
                    xcb_xkb_get_names_मूल्य_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_names_मूल्य_list_indicator_names(list);

    FAIL_UNLESS(msb_pos(reply->indicators) <= keymap->अंक_leds);

    क्रम (अचिन्हित i = 0; i < अंक_INDICATORS; i++) {
        अगर (reply->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            काष्ठा xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_vmod_names(काष्ठा xkb_keymap *keymap, काष्ठा x11_atom_interner *interner,
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
    क्रम (अचिन्हित i = 0; i < अंक_VMODS; i++) {
        अगर (reply->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            काष्ठा xkb_mod *mod = &keymap->mods.mods[अंक_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_group_names(काष्ठा xkb_keymap *keymap, काष्ठा x11_atom_interner *interner,
                xcb_xkb_get_names_reply_t *reply,
                xcb_xkb_get_names_मूल्य_list_t *list)
{
    पूर्णांक लंबाई = xcb_xkb_get_names_मूल्य_list_groups_लंबाई(reply, list);
    xcb_atom_t *iter = xcb_xkb_get_names_मूल्य_list_groups(list);

    keymap->अंक_group_names = msb_pos(reply->groupNames);
    ALLOC_OR_FAIL(keymap->group_names, keymap->अंक_group_names);

    क्रम (पूर्णांक i = 0; i < लंबाई; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->group_names[i]);
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_कुंजी_names(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_names_reply_t *reply,
              xcb_xkb_get_names_मूल्य_list_t *list)
{
    पूर्णांक लंबाई = xcb_xkb_get_names_मूल्य_list_कुंजी_names_लंबाई(reply, list);
    xcb_xkb_कुंजी_name_iterator_t iter =
        xcb_xkb_get_names_मूल्य_list_कुंजी_names_iterator(reply, list);

    FAIL_UNLESS(reply->min_कुंजी_Code == keymap->min_कुंजी_code);
    FAIL_UNLESS(reply->max_कुंजी_Code == keymap->max_कुंजी_code);
    FAIL_UNLESS(reply->first_कुंजी == keymap->min_कुंजी_code);
    FAIL_UNLESS(reply->first_कुंजी + reply->n_कुंजियाँ - 1U == keymap->max_कुंजी_code);

    क्रम (पूर्णांक i = 0; i < लंबाई; i++) {
        xcb_xkb_कुंजी_name_t *wire = iter.तथ्य;
        xkb_atom_t *कुंजी_name = &keymap->कुंजियाँ[reply->first_कुंजी + i].name;

        अगर (wire->name[0] == '\0') {
            *कुंजी_name = XKB_ATOM_NONE;
        }
        अन्यथा {
            *कुंजी_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_कुंजी_NAME_लंबाई));
            अगर (!*कुंजी_name)
                वापस false;
        }

        xcb_xkb_कुंजी_name_अग्र(&iter);
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_aliases(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_names_reply_t *reply,
            xcb_xkb_get_names_मूल्य_list_t *list)
{
    पूर्णांक लंबाई = xcb_xkb_get_names_मूल्य_list_कुंजी_aliases_लंबाई(reply, list);
    xcb_xkb_कुंजी_alias_iterator_t iter =
        xcb_xkb_get_names_मूल्य_list_कुंजी_aliases_iterator(reply, list);

    keymap->अंक_कुंजी_aliases = reply->n_कुंजी_Aliases;
    ALLOC_OR_FAIL(keymap->कुंजी_aliases, keymap->अंक_कुंजी_aliases);

    क्रम (पूर्णांक i = 0; i < लंबाई; i++) {
        xcb_xkb_कुंजी_alias_t *wire = iter.तथ्य;
        काष्ठा xkb_कुंजी_alias *alias = &keymap->कुंजी_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_कुंजी_NAME_लंबाई));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_कुंजी_NAME_लंबाई));
        अगर (!alias->real || !alias->alias)
            जाओ fail;

        xcb_xkb_कुंजी_alias_अग्र(&iter);
    }

    वापस true;

fail:
    वापस false;
}

अटल bool
get_names(काष्ठा xkb_keymap *keymap, काष्ठा x11_atom_interner *interner,
          xcb_xkb_get_names_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_names_reply_t *reply =
        xcb_xkb_get_names_reply(conn, cookie, शून्य);
    xcb_xkb_get_names_मूल्य_list_t list;

    FAIL_IF_BAD_REPLY(reply, "XkbGetNames");

    FAIL_UNLESS((reply->which & get_names_required) == get_names_required);

    xcb_xkb_get_names_मूल्य_list_unpack(xcb_xkb_get_names_मूल्य_list(reply),
                                        reply->nTypes,
                                        reply->indicators,
                                        reply->virtualMods,
                                        reply->groupNames,
                                        reply->n_कुंजियाँ,
                                        reply->n_कुंजी_Aliases,
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
    अगर (!get_type_names(keymap, interner, reply, &list) ||
        !get_indicator_names(keymap, interner, reply, &list) ||
        !get_vmod_names(keymap, interner, reply, &list) ||
        !get_group_names(keymap, interner, reply, &list) ||
        !get_कुंजी_names(keymap, conn, reply, &list) ||
        !get_aliases(keymap, conn, reply, &list))
        जाओ fail;

    मुक्त(reply);
    वापस true;

fail:
    मुक्त(reply);
    वापस false;
}

अटल bool
get_controls(काष्ठा xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_reply_t *reply =
        xcb_xkb_get_controls_reply(conn, cookie, शून्य);

    FAIL_IF_BAD_REPLY(reply, "XkbGetControls");
    FAIL_UNLESS(reply->अंक_Groups > 0 && reply->अंक_Groups <= 4);

    keymap->अंक_groups = reply->अंक_Groups;

    FAIL_UNLESS(keymap->max_कुंजी_code < XCB_XKB_CONST_PER_कुंजी_BIT_ARRAY_आकार * 8);

    क्रम (xkb_keycode_t i = keymap->min_कुंजी_code; i <= keymap->max_कुंजी_code; i++)
        keymap->कुंजियाँ[i].repeats = (reply->per_कुंजी_Repeat[i / 8] & (1u << (i % 8)));

    मुक्त(reply);
    वापस true;

fail:
    मुक्त(reply);
    वापस false;
}

काष्ठा xkb_keymap *
xkb_x11_keymap_new_from_device(काष्ठा xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               क्रमागत xkb_keymap_compile_flags flags)
{
    अगर (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_MESSAGE_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        वापस शून्य;
    }

    स्थिर क्रमागत xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    काष्ठा xkb_keymap * स्थिर keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    अगर (!keymap)
        वापस शून्य;

    keymap->redirect_कुंजी_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    काष्ठा x11_atom_interner interner;
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

    अगर (!get_map(keymap, conn, map_cookie))
        जाओ err_map;
    अगर (!get_indicator_map(keymap, conn, indicator_map_cookie))
        जाओ err_indicator_map;
    अगर (!get_compat_map(keymap, conn, compat_map_cookie))
        जाओ err_compat_map;
    अगर (!get_names(keymap, &interner, names_cookie))
        जाओ err_names;
    अगर (!get_controls(keymap, conn, controls_cookie))
        जाओ err_controls;
    x11_atom_interner_round_trip(&interner);
    अगर (interner.had_error)
        जाओ err_interner;

    वापस keymap;

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
    वापस शून्य;
}
