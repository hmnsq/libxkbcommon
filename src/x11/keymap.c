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
#परिभाषय अङ्कः_REAL_MODS 8u
/* XkbNumVirtualMods. */
#परिभाषय अङ्कः_VMODS 16u
/* XkbNoModifier. */
#परिभाषय NO_MODIFIER 0xff
/* XkbNumIndicators. */
#परिभाषय अङ्कः_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#परिभाषय ALL_INDICATORS_आवरणम् 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#परिभाषय FAIL_UNLESS(व्यञ्जकम्) कुरु {                              \
    यदि (!(व्यञ्जकम्)) {                                          \
        log_err(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(व्यञ्जकम्));                 \
        गच्छ fail;                                          \
    }                                                       \
} यावत् (0)

#परिभाषय FAIL_IF_BAD_प्रत्युत्तरम्(प्रत्युत्तरम्, request_name) कुरु {         \
    यदि (!(प्रत्युत्तरम्)) {                                           \
        log_err(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        गच्छ fail;                                          \
    }                                                       \
} यावत् (0)

#परिभाषय ALLOC_OR_FAIL(arr, nmemb) कुरु {                      \
    यदि ((nmemb) > 0) {                                      \
        (arr) = शून्यस्मृतिं_देहि((nmemb), परिमाण(*(arr)));            \
        यदि (!(arr))                                         \
            गच्छ fail;                                      \
    }                                                       \
} यावत् (0)

स्थिर ध्रुव xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_कुञ्चिका_TYPES |
     XCB_XKB_MAP_PART_कुञ्चिका_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_कुञ्चिका_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

स्थिर ध्रुव xcb_xkb_name_detail_t get_नामानि_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_कुञ्चिका_TYPE_नामानि |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_नामानि |
     XCB_XKB_NAME_DETAIL_INDICATOR_नामानि |
     XCB_XKB_NAME_DETAIL_कुञ्चिका_नामानि |
     XCB_XKB_NAME_DETAIL_कुञ्चिका_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_नामानि |
     XCB_XKB_NAME_DETAIL_गणः_नामानि);
स्थिर ध्रुव xcb_xkb_name_detail_t get_नामानि_required =
    (XCB_XKB_NAME_DETAIL_कुञ्चिका_TYPE_नामानि |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_नामानि |
     XCB_XKB_NAME_DETAIL_कुञ्चिका_नामानि |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_नामानि);


स्थिर xkb_mod_आवरणम्_t
translate_mods(uint8_t rmods, uint16_t vmods_नीचः, uint16_t vmods_उच्चः)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    प्रत्यागम
        ((xkb_mod_आवरणम्_t) rmods) |
        ((xkb_mod_आवरणम्_t) vmods_नीचः << 8) |
        ((xkb_mod_आवरणम्_t) vmods_उच्चः << 16);
}

गणना xkb_action_controls
translate_controls_आवरणम्(uint32_t wire)
{
    गणना xkb_action_controls ret = 0;
    यदि (wire & XCB_XKB_BOOL_CTRL_REPEAT_कुञ्चिकाः)
        ret |= CONTROL_REPEAT;
    यदि (wire & XCB_XKB_BOOL_CTRL_SLOW_कुञ्चिकाः)
        ret |= CONTROL_SLOW;
    यदि (wire & XCB_XKB_BOOL_CTRL_BOUNCE_कुञ्चिकाः)
        ret |= CONTROL_DEBOUNCE;
    यदि (wire & XCB_XKB_BOOL_CTRL_STICKY_कुञ्चिकाः)
        ret |= CONTROL_STICKY_कुञ्चिकाः;
    यदि (wire & XCB_XKB_BOOL_CTRL_MOUSE_कुञ्चिकाः)
        ret |= CONTROL_MOUSE_कुञ्चिकाः;
    यदि (wire & XCB_XKB_BOOL_CTRL_MOUSE_कुञ्चिकाः_ACCEL)
        ret |= CONTROL_MOUSE_कुञ्चिकाः_ACCEL;
    यदि (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_कुञ्चिकाः)
        ret |= CONTROL_AX;
    यदि (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_आवरणम्)
        ret |= CONTROL_AX_TIMEOUT;
    यदि (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_आवरणम्)
        ret |= CONTROL_AX_FEEDBACK;
    यदि (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_आवरणम्)
        ret |= CONTROL_BELL;
    यदि (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_आवरणम्)
        ret |= CONTROL_OVERLAY1;
    यदि (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_आवरणम्)
        ret |= CONTROL_OVERLAY2;
    यदि (wire & XCB_XKB_BOOL_CTRL_IGNORE_गणः_तालकम्_आवरणम्)
        ret |= CONTROL_IGNORE_गणः_तालकम्;
    /* Some controls are not supported and don't appear here. */
    प्रत्यागम ret;
}

स्थिर bool
translate_action(सङ्घ xkb_action *action, ध्रुव xcb_xkb_action_t *wire,
                 xkb_keycode_t min_कुञ्चिका_संहिता, xkb_keycode_t max_कुञ्चिका_संहिता)
{
    विकल्प (wire->type) {
    पक्ष XCB_XKB_SA_TYPE_समुच्चयः_MODS:
        action->type = ACTION_TYPE_MOD_समुच्चयः;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_नीचः_2,
                                                wire->setmods.vmods_उच्चः_2);
        action->mods.mods.आवरणम् = translate_mods(wire->setmods.आवरणम्, 0, 0);

        यदि (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_तालकम्_CLEAR;
        यदि (wire->setmods.flags & XCB_XKB_SA_LATCH_गन्तव्यम्_तालकम्)
            action->mods.flags |= ACTION_LATCH_गन्तव्यम्_तालकम्;
        यदि (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_नीचः_2,
                                                wire->latchmods.vmods_उच्चः_2);
        action->mods.mods.आवरणम् = translate_mods(wire->latchmods.आवरणम्, 0, 0);

        यदि (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_तालकम्_CLEAR;
        यदि (wire->latchmods.flags & XCB_XKB_SA_LATCH_गन्तव्यम्_तालकम्)
            action->mods.flags |= ACTION_LATCH_गन्तव्यम्_तालकम्;
        यदि (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_तालकम्_MODS:
        action->type = ACTION_TYPE_MOD_तालकम्;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_नीचः_2,
                                                wire->lockmods.vmods_उच्चः_2);
        action->mods.mods.आवरणम् = translate_mods(wire->lockmods.आवरणम्, 0, 0);

        यदि (wire->lockmods.flags & XCB_XKB_SA_ISO_तालकम्_FLAG_NO_तालकम्)
            action->mods.flags |= ACTION_तालकम्_NO_तालकम्;
        यदि (wire->lockmods.flags & XCB_XKB_SA_ISO_तालकम्_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_तालकम्_NO_UNLOCK;
        यदि (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_समुच्चयः_गणः:
        action->type = ACTION_TYPE_गणः_समुच्चयः;

        action->गणः.गणः = (int32_t)wire->setgroup.गणः;

        यदि (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->गणः.flags |= ACTION_तालकम्_CLEAR;
        यदि (wire->setmods.flags & XCB_XKB_SA_LATCH_गन्तव्यम्_तालकम्)
            action->गणः.flags |= ACTION_LATCH_गन्तव्यम्_तालकम्;
        यदि (wire->setmods.flags & XCB_XKB_SA_ISO_तालकम्_FLAG_गणः_ABSOLUTE)
            action->गणः.flags |= ACTION_ABSOLUTE_SWITCH;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_LATCH_गणः:
        action->type = ACTION_TYPE_गणः_LATCH;

        action->गणः.गणः = (int32_t)wire->latchgroup.गणः;

        यदि (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->गणः.flags |= ACTION_तालकम्_CLEAR;
        यदि (wire->latchmods.flags & XCB_XKB_SA_LATCH_गन्तव्यम्_तालकम्)
            action->गणः.flags |= ACTION_LATCH_गन्तव्यम्_तालकम्;
        यदि (wire->latchmods.flags & XCB_XKB_SA_ISO_तालकम्_FLAG_गणः_ABSOLUTE)
            action->गणः.flags |= ACTION_ABSOLUTE_SWITCH;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_तालकम्_गणः:
        action->type = ACTION_TYPE_गणः_तालकम्;

        action->गणः.गणः = (int32_t)wire->lockgroup.गणः;

        यदि (wire->lockgroup.flags & XCB_XKB_SA_ISO_तालकम्_FLAG_गणः_ABSOLUTE)
            action->गणः.flags |= ACTION_ABSOLUTE_SWITCH;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_नीचः | ((uint16_t) wire->moveptr.x_उच्चः << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_नीचः | ((uint16_t) wire->moveptr.y_उच्चः << 8));

        यदि (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        यदि (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        यदि (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.गणनम् = wire->ptrbtn.गणनम्;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_तालकम्_PTR_BTN:
        action->type = ACTION_TYPE_PTR_तालकम्;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.गणनम् = 0;

        यदि (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_तालकम्_FLAG_NO_तालकम्)
            action->btn.flags |= ACTION_तालकम्_NO_तालकम्;
        यदि (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_तालकम्_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_तालकम्_NO_UNLOCK;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_समुच्चयः_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.मानम् = wire->setptrdflt.मानम्;

        यदि (wire->setptrdflt.flags & XCB_XKB_SA_समुच्चयः_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        यदि (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        यदि (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_समुच्चयः_CONTROLS:
        action->type = ACTION_TYPE_CTRL_समुच्चयः;
        {
            ध्रुव uint16_t आवरणम् = (wire->setcontrols.bool_Ctrls_नीचः |
                                   (wire->setcontrols.bool_Ctrls_उच्चः << 8));
            action->ctrls.ctrls = translate_controls_आवरणम्(आवरणम्);
        }
        भङ्ग;
    पक्ष XCB_XKB_SA_TYPE_तालकम्_CONTROLS:
        action->type = ACTION_TYPE_CTRL_तालकम्;
        {
            ध्रुव uint16_t आवरणम् = (wire->lockcontrols.bool_Ctrls_नीचः |
                                   (wire->lockcontrols.bool_Ctrls_उच्चः << 8));
            action->ctrls.ctrls = translate_controls_आवरणम्(आवरणम्);
        }
        भङ्ग;

    पक्ष XCB_XKB_SA_TYPE_REDIRECT_कुञ्चिका:
        action->type = ACTION_TYPE_REDIRECT_कुञ्चिका;
        यदि (wire->redirect.newkey < min_कुञ्चिका_संहिता ||
            wire->redirect.newkey > max_कुञ्चिका_संहिता)
                प्रत्यागम false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.आवरणम्,
            wire->redirect.vmods_आवरणम्_नीचः,
            wire->redirect.vmods_आवरणम्_उच्चः
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_नीचः_2,
                                               wire->redirect.vmods_उच्चः_2);
        भङ्ग;

    पक्ष XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        भङ्ग;

    /* We don't support these. */
    पक्ष XCB_XKB_SA_TYPE_ISO_तालकम्:
    पक्ष XCB_XKB_SA_TYPE_ACTION_सन्देशः:
    पक्ष XCB_XKB_SA_TYPE_DEVICE_BTN:
    पक्ष XCB_XKB_SA_TYPE_तालकम्_DEVICE_BTN:
    पक्ष XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        भङ्ग;

    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_आन्तरिकम् == 20 &&
                      ACTION_TYPE_आन्तरिकम् + 1 == ACTION_TYPE_अङ्कः_ENTRIES,
                      "Missing action type");

        यदि (wire->type < ACTION_TYPE_वैयक्तिकम्) {
            action->type = ACTION_TYPE_NONE;
            भङ्ग;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(परिमाण(action->priv.दत्त) == 7 &&
                      परिमाण(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        स्मृतिं_प्रतिलिख(action->priv.दत्त, wire->noaction.pad0, 7);
        भङ्ग;
    }
    प्रत्यागम true;
}

स्थिर bool
get_types(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_प्रत्युत्तरम्_t *प्रत्युत्तरम्, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क types_दीर्घता = xcb_xkb_get_map_map_types_rtrn_दीर्घता(प्रत्युत्तरम्, map);
    xcb_xkb_कुञ्चिका_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(प्रत्युत्तरम्, map);

    FAIL_UNLESS(प्रत्युत्तरम्->firstType == 0);

    keymap->अङ्कः_types = प्रत्युत्तरम्->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->अङ्कः_types);
    प्रतिज्ञा(types_दीर्घता == (पूर्णाङ्क) keymap->अङ्कः_types);

    प्रति (पूर्णाङ्क i = 0; i < types_दीर्घता; i++) {
        xcb_xkb_कुञ्चिका_type_t *wire_type = types_iter.दत्त;
        संरचना xkb_कुञ्चिका_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->अङ्कः_स्तराः_2 > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.आवरणम् = translate_mods(wire_type->mods_आवरणम्, 0, 0);
        type->अङ्कः_स्तराः = wire_type->अङ्कः_स्तराः_2;

        {
            पूर्णाङ्क entries_दीर्घता = xcb_xkb_कुञ्चिका_type_map_दीर्घता(wire_type);
            xcb_xkb_kt_map_प्रविष्टिः_iterator_t entries_iter =
                xcb_xkb_कुञ्चिका_type_map_iterator(wire_type);

            type->अङ्कः_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->अङ्कः_entries);
            प्रतिज्ञा(entries_दीर्घता == (पूर्णाङ्क) type->अङ्कः_entries);

            प्रति (पूर्णाङ्क j = 0; j < entries_दीर्घता; j++) {
                xcb_xkb_kt_map_प्रविष्टिः_t *wire_प्रविष्टिः = entries_iter.दत्त;
                संरचना xkb_कुञ्चिका_type_प्रविष्टिः *प्रविष्टिः = &type->entries[j];

                FAIL_UNLESS(wire_प्रविष्टिः->level < type->अङ्कः_स्तराः);

                प्रविष्टिः->level = wire_प्रविष्टिः->level;
                प्रविष्टिः->mods.mods = translate_mods(wire_प्रविष्टिः->mods_mods,
                                                  wire_प्रविष्टिः->mods_vmods, 0);
                प्रविष्टिः->mods.आवरणम् = translate_mods(wire_प्रविष्टिः->mods_आवरणम्, 0, 0);

                xcb_xkb_kt_map_प्रविष्टिः_अग्र(&entries_iter);
            }
        }

        {
            पूर्णाङ्क preserves_दीर्घता = xcb_xkb_कुञ्चिका_type_preserve_दीर्घता(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_कुञ्चिका_type_preserve_iterator(wire_type);

            FAIL_UNLESS((अचिह्नित) preserves_दीर्घता <= type->अङ्कः_entries);

            प्रति (पूर्णाङ्क j = 0; j < preserves_दीर्घता; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.दत्त;
                संरचना xkb_कुञ्चिका_type_प्रविष्टिः *प्रविष्टिः = &type->entries[j];

                प्रविष्टिः->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                प्रविष्टिः->preserve.आवरणम् = translate_mods(wire_preserve->आवरणम्, 0, 0);

                xcb_xkb_mod_def_अग्र(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_कुञ्चिका_type_अग्र(&types_iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_sym_maps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_प्रत्युत्तरम्_t *प्रत्युत्तरम्, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क sym_maps_दीर्घता = xcb_xkb_get_map_map_syms_rtrn_दीर्घता(प्रत्युत्तरम्, map);
    xcb_xkb_कुञ्चिका_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(प्रत्युत्तरम्, map);

    FAIL_UNLESS(प्रत्युत्तरम्->min_कुञ्चिका_संहिता_2 <= प्रत्युत्तरम्->max_कुञ्चिका_संहिता_2);
    FAIL_UNLESS(प्रत्युत्तरम्->first_कुञ्चिका_Sym >= प्रत्युत्तरम्->min_कुञ्चिका_संहिता_2);
    FAIL_UNLESS(प्रत्युत्तरम्->first_कुञ्चिका_Sym + प्रत्युत्तरम्->n_कुञ्चिका_Syms <= प्रत्युत्तरम्->max_कुञ्चिका_संहिता_2 + 1);

    keymap->min_कुञ्चिका_संहिता = प्रत्युत्तरम्->min_कुञ्चिका_संहिता_2;
    keymap->max_कुञ्चिका_संहिता = प्रत्युत्तरम्->max_कुञ्चिका_संहिता_2;
    keymap->अङ्कः_कुञ्चिकाः = keymap->अङ्कः_कुञ्चिकाः_नीचः = प्रत्युत्तरम्->max_कुञ्चिका_संहिता_2 + 1;

    ALLOC_OR_FAIL(keymap->कुञ्चिकाः, keymap->max_कुञ्चिका_संहिता + 1);

    प्रति (xkb_keycode_t kc = keymap->min_कुञ्चिका_संहिता; kc <= keymap->max_कुञ्चिका_संहिता; kc++)
        keymap->कुञ्चिकाः[kc].keycode = kc;

    प्रति (पूर्णाङ्क i = 0; i < sym_maps_दीर्घता; i++) {
        xcb_xkb_कुञ्चिका_sym_map_t *wire_sym_map = sym_maps_iter.दत्त;
        संरचना xkb_कुञ्चिका *कुञ्चिका = &keymap->कुञ्चिकाः[प्रत्युत्तरम्->first_कुञ्चिका_Sym + i];

        कुञ्चिका->अङ्कः_groups = wire_sym_map->गणः_Info & 0x0f;
        FAIL_UNLESS(कुञ्चिका->अङ्कः_groups <= ARRAY_आकार(wire_sym_map->kt_सूचकांक));
        ALLOC_OR_FAIL(कुञ्चिका->groups, कुञ्चिका->अङ्कः_groups);

        प्रति (xkb_layout_सूचकांक_t j = 0; j < कुञ्चिका->अङ्कः_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_सूचकांक[j] < keymap->अङ्कः_types);
            कुञ्चिका->groups[j].type = &keymap->types[wire_sym_map->kt_सूचकांक[j]];

            ALLOC_OR_FAIL(कुञ्चिका->groups[j].स्तराः, कुञ्चिका->groups[j].type->अङ्कः_स्तराः);
        }

        कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या = (wire_sym_map->गणः_Info & 0x30) >> 4;

        FAIL_UNLESS(कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या <= कुञ्चिका->अङ्कः_groups);

        यदि (wire_sym_map->गणः_Info & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_policy = XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_CLAMP;
        अन्यथा यदि (wire_sym_map->गणः_Info & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_policy = XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT;
        अन्यथा
            कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_policy = XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP;

        {
            पूर्णाङ्क syms_दीर्घता = xcb_xkb_कुञ्चिका_sym_map_syms_दीर्घता(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_कुञ्चिका_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((अचिह्नित) syms_दीर्घता == wire_sym_map->विस्तारः * कुञ्चिका->अङ्कः_groups);

            यदि (syms_दीर्घता > 0)
                कुञ्चिका->explicit |= EXPLICIT_SYMBOLS;

            प्रति (xkb_layout_सूचकांक_t गणः = 0; गणः < कुञ्चिका->अङ्कः_groups; गणः++) {
                प्रति (xkb_level_सूचकांक_t level = 0; level < wire_sym_map->विस्तारः; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    प्रतिज्ञा(कुञ्चिका->groups[गणः].type != रिक्त);
                    यदि (level < कुञ्चिका->groups[गणः].type->अङ्कः_स्तराः) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        कुञ्चिका->groups[गणः].स्तराः[level].अङ्कः_syms = 1;
                        कुञ्चिका->groups[गणः].स्तराः[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        कुञ्चिका->groups[गणः].स्तराः[level].ऊर्ध्वस्थम् =
                            xkb_keysym_गन्तव्यम्_ऊर्ध्वस्थम्(wire_keysym);
                        यदि (wire_keysym != XKB_कुञ्चिका_No_चिह्नम्)
                            कुञ्चिका->groups[गणः].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_कुञ्चिका_sym_map_अग्र(&sym_maps_iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_actions(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_प्रत्युत्तरम्_t *प्रत्युत्तरम्, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क acts_गणनम्_दीर्घता =
        xcb_xkb_get_map_map_acts_rtrn_गणनम्_दीर्घता(प्रत्युत्तरम्, map);
    uint8_t *acts_गणनम्_iter = xcb_xkb_get_map_map_acts_rtrn_गणनम्(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(प्रत्युत्तरम्, map);
    xcb_xkb_कुञ्चिका_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(प्रत्युत्तरम्, map);

    FAIL_UNLESS(प्रत्युत्तरम्->first_कुञ्चिका_Action == keymap->min_कुञ्चिका_संहिता);
    FAIL_UNLESS(प्रत्युत्तरम्->first_कुञ्चिका_Action + प्रत्युत्तरम्->n_कुञ्चिका_Actions ==
                keymap->max_कुञ्चिका_संहिता + 1);

    प्रति (पूर्णाङ्क i = 0; i < acts_गणनम्_दीर्घता; i++) {
        xcb_xkb_कुञ्चिका_sym_map_t *wire_sym_map = sym_maps_iter.दत्त;
        पूर्णाङ्क syms_दीर्घता = xcb_xkb_कुञ्चिका_sym_map_syms_दीर्घता(wire_sym_map);
        uint8_t wire_गणनम् = *acts_गणनम्_iter;
        संरचना xkb_कुञ्चिका *कुञ्चिका = &keymap->कुञ्चिकाः[प्रत्युत्तरम्->first_कुञ्चिका_Action + i];

        FAIL_UNLESS((अचिह्नित) syms_दीर्घता == wire_sym_map->विस्तारः * कुञ्चिका->अङ्कः_groups);
        FAIL_UNLESS(wire_गणनम् == 0 || wire_गणनम् == syms_दीर्घता);

        ध्रुव xkb_keycode_t min_कुञ्चिका_संहिता = keymap->min_कुञ्चिका_संहिता;
        ध्रुव xkb_keycode_t max_कुञ्चिका_संहिता = keymap->max_कुञ्चिका_संहिता;

        यदि (wire_गणनम् != 0) {
            प्रति (xkb_layout_सूचकांक_t गणः = 0; गणः < कुञ्चिका->अङ्कः_groups; गणः++) {
                प्रति (xkb_level_सूचकांक_t level = 0; level < wire_sym_map->विस्तारः; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.दत्त;

                    यदि (level < कुञ्चिका->groups[गणः].type->अङ्कः_स्तराः) {
                        कुञ्चिका->groups[गणः].स्तराः[level].अङ्कः_actions = 1;
                        सङ्घ xkb_action *action =
                            &कुञ्चिका->groups[गणः].स्तराः[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_कुञ्चिका_संहिता, max_कुञ्चिका_संहिता));

                        यदि (action->type != ACTION_TYPE_NONE) {
                            कुञ्चिका->groups[गणः].implicit_actions =
                                !(कुञ्चिका->explicit & EXPLICIT_INTERP);
                        } अन्यथा यदि (कुञ्चिका->groups[गणः].स्तराः[level].s.sym ==
                                   XKB_कुञ्चिका_No_चिह्नम्) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            कुञ्चिका->groups[गणः].स्तराः[level].अङ्कः_syms = 0;
                            कुञ्चिका->groups[गणः].स्तराः[level].अङ्कः_actions = 0;
                        }
                    }

                    xcb_xkb_action_अग्र(&acts_iter);
                }

                यदि (कुञ्चिका->groups[गणः].implicit_actions)
                    कुञ्चिका->implicit_actions = true;
            }
        }

        acts_गणनम्_iter++;
        xcb_xkb_कुञ्चिका_sym_map_अग्र(&sym_maps_iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_vmods(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_प्रत्युत्तरम्_t *प्रत्युत्तरम्, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.अङ्कः_mods =
        अङ्कः_REAL_MODS + MIN(msb_pos(प्रत्युत्तरम्->virtualMods), अङ्कः_VMODS);
    FAIL_UNLESS(keymap->mods.अङ्कः_mods <= XKB_MAX_MODS);

    static_assert(अङ्कः_REAL_MODS + अङ्कः_VMODS <= XKB_MAX_MODS, "");
    प्रति (अचिह्नित i = 0; i < अङ्कः_VMODS; i++) {
        यदि (प्रत्युत्तरम्->virtualMods & (1u << i)) {
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
              xcb_xkb_get_map_प्रत्युत्तरम्_t *प्रत्युत्तरम्, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क दीर्घता = xcb_xkb_get_map_map_explicit_rtrn_दीर्घता(प्रत्युत्तरम्, map);
    xcb_xkb_समुच्चयः_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(प्रत्युत्तरम्, map);

    प्रति (पूर्णाङ्क i = 0; i < दीर्घता; i++) {
        xcb_xkb_समुच्चयः_explicit_t *wire = iter.दत्त;
        संरचना xkb_कुञ्चिका *कुञ्चिका;

        FAIL_UNLESS(wire->keycode >= keymap->min_कुञ्चिका_संहिता &&
                    wire->keycode <= keymap->max_कुञ्चिका_संहिता);

        कुञ्चिका = &keymap->कुञ्चिकाः[wire->keycode];

        यदि ((wire->explicit & XCB_XKB_EXPLICIT_कुञ्चिका_TYPE_1) &&
            कुञ्चिका->अङ्कः_groups > 0) {
            कुञ्चिका->groups[0].explicit_type = true;
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
        }
        यदि ((wire->explicit & XCB_XKB_EXPLICIT_कुञ्चिका_TYPE_2) &&
            कुञ्चिका->अङ्कः_groups > 1) {
            कुञ्चिका->groups[1].explicit_type = true;
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
        }
        यदि ((wire->explicit & XCB_XKB_EXPLICIT_कुञ्चिका_TYPE_3) &&
            कुञ्चिका->अङ्कः_groups > 2) {
            कुञ्चिका->groups[2].explicit_type = true;
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
        }
        यदि ((wire->explicit & XCB_XKB_EXPLICIT_कुञ्चिका_TYPE_4) &&
            कुञ्चिका->अङ्कः_groups > 3) {
            कुञ्चिका->groups[3].explicit_type = true;
            कुञ्चिका->explicit |= EXPLICIT_TYPES;
        }
        यदि (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            कुञ्चिका->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            प्रति (xkb_layout_सूचकांक_t l = 0; l < कुञ्चिका->अङ्कः_groups; l++) {
                कुञ्चिका->groups[l].explicit_actions = true;
            }
        }
        यदि (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            कुञ्चिका->explicit |= EXPLICIT_REPEAT;
        यदि (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            कुञ्चिका->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_समुच्चयः_explicit_अग्र(&iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_modmaps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_प्रत्युत्तरम्_t *प्रत्युत्तरम्, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क दीर्घता = xcb_xkb_get_map_map_modmap_rtrn_दीर्घता(प्रत्युत्तरम्, map);
    xcb_xkb_कुञ्चिका_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(प्रत्युत्तरम्, map);

    darray(Mod_Map_प्रविष्टिः) modmaps = darray_new();

    प्रति (पूर्णाङ्क i = 0; i < दीर्घता; i++) {
        xcb_xkb_कुञ्चिका_mod_map_t *wire = iter.दत्त;
        संरचना xkb_कुञ्चिका *कुञ्चिका;

        FAIL_UNLESS(wire->keycode >= keymap->min_कुञ्चिका_संहिता &&
                    wire->keycode <= keymap->max_कुञ्चिका_संहिता);

        कुञ्चिका = &keymap->कुञ्चिकाः[wire->keycode];
        कुञ्चिका->modmap = wire->mods;

        darray_append(modmaps, (Mod_Map_प्रविष्टिः) {
            .कुञ्चिका_संहिता = कुञ्चिका->keycode,
            .mods = कुञ्चिका->modmap,
            .have_चिह्नम् = false,
            .u.कुञ्चिका_Name = कुञ्चिका->name
        });

        xcb_xkb_कुञ्चिका_mod_map_अग्र(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->अङ्कः_modmaps);
    प्रत्यागम true;

fail:
    darray_free(modmaps);
    प्रत्यागम false;
}

स्थिर bool
get_vmodmaps(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_प्रत्युत्तरम्_t *प्रत्युत्तरम्, xcb_xkb_get_map_map_t *map)
{
    पूर्णाङ्क दीर्घता = xcb_xkb_get_map_map_vmodmap_rtrn_दीर्घता(प्रत्युत्तरम्, map);
    xcb_xkb_कुञ्चिका_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(प्रत्युत्तरम्, map);

    प्रति (पूर्णाङ्क i = 0; i < दीर्घता; i++) {
        xcb_xkb_कुञ्चिका_v_mod_map_t *wire = iter.दत्त;
        संरचना xkb_कुञ्चिका *कुञ्चिका;

        FAIL_UNLESS(wire->keycode >= keymap->min_कुञ्चिका_संहिता &&
                    wire->keycode <= keymap->max_कुञ्चिका_संहिता);

        कुञ्चिका = &keymap->कुञ्चिकाः[wire->keycode];
        कुञ्चिका->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_कुञ्चिका_v_mod_map_अग्र(&iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_प्रत्युत्तरम्_t *प्रत्युत्तरम् = xcb_xkb_get_map_प्रत्युत्तरम्(conn, cookie, रिक्त);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_प्रत्युत्तरम्(प्रत्युत्तरम्, "XkbGetMap");

    यदि ((प्रत्युत्तरम्->present & get_map_required_components) != get_map_required_components)
        गच्छ fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(प्रत्युत्तरम्),
                               प्रत्युत्तरम्->nTypes,
                               प्रत्युत्तरम्->n_कुञ्चिका_Syms,
                               प्रत्युत्तरम्->n_कुञ्चिका_Actions,
                               प्रत्युत्तरम्->योग_Actions,
                               प्रत्युत्तरम्->योग_कुञ्चिका_Behaviors,
                               प्रत्युत्तरम्->virtualMods,
                               प्रत्युत्तरम्->योग_कुञ्चिका_Explicit,
                               प्रत्युत्तरम्->योग_Mod_Map_कुञ्चिकाः,
                               प्रत्युत्तरम्->योग_VMod_Map_कुञ्चिकाः,
                               प्रत्युत्तरम्->present,
                               &map);

    यदि (!get_types(keymap, conn, प्रत्युत्तरम्, &map) ||
        !get_sym_maps(keymap, conn, प्रत्युत्तरम्, &map) ||
        !get_actions(keymap, conn, प्रत्युत्तरम्, &map) ||
        !get_vmods(keymap, conn, प्रत्युत्तरम्, &map) ||
        !get_explicits(keymap, conn, प्रत्युत्तरम्, &map) ||
        !get_modmaps(keymap, conn, प्रत्युत्तरम्, &map) ||
        !get_vmodmaps(keymap, conn, प्रत्युत्तरम्, &map))
        गच्छ fail;

    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम true;

fail:
    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम false;
}

स्थिर bool
get_indicators(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_प्रत्युत्तरम्_t *प्रत्युत्तरम्)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(प्रत्युत्तरम्);

    keymap->अङ्कः_leds = msb_pos(प्रत्युत्तरम्->which);
    FAIL_UNLESS(keymap->अङ्कः_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == अङ्कः_INDICATORS, "");
    प्रति (अचिह्नित i = 0; i < अङ्कः_INDICATORS; i++) {
        यदि (प्रत्युत्तरम्->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.दत्त;
            संरचना xkb_led *led = &keymap->leds[i];

            यदि (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_आधारः)
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

            यदि (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_आधारः)
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
            led->mods.आवरणम् = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_आवरणम्(wire->ctrls);

            xcb_xkb_indicator_map_अग्र(&iter);
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
    xcb_xkb_get_indicator_map_प्रत्युत्तरम्_t *प्रत्युत्तरम् =
        xcb_xkb_get_indicator_map_प्रत्युत्तरम्(conn, cookie, रिक्त);

    FAIL_IF_BAD_प्रत्युत्तरम्(प्रत्युत्तरम्, "XkbGetIndicatorMap");

    यदि (!get_indicators(keymap, conn, प्रत्युत्तरम्))
        गच्छ fail;

    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम true;

fail:
    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम false;
}

स्थिर bool
get_sym_interprets(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_प्रत्युत्तरम्_t *प्रत्युत्तरम्)
{
    पूर्णाङ्क दीर्घता = xcb_xkb_get_compat_map_si_rtrn_दीर्घता(प्रत्युत्तरम्);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(प्रत्युत्तरम्);

    FAIL_UNLESS(प्रत्युत्तरम्->firstSIRtrn == 0);
    FAIL_UNLESS(प्रत्युत्तरम्->nSIRtrn == प्रत्युत्तरम्->n_योग_SI);

    keymap->अङ्कः_sym_interprets = प्रत्युत्तरम्->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->अङ्कः_sym_interprets);

    ध्रुव xkb_keycode_t min_कुञ्चिका_संहिता = keymap->min_कुञ्चिका_संहिता;
    ध्रुव xkb_keycode_t max_कुञ्चिका_संहिता = keymap->max_कुञ्चिका_संहिता;

    प्रति (पूर्णाङ्क i = 0; i < दीर्घता; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.दत्त;
        संरचना xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        विकल्प (wire->मेलनम् & XCB_XKB_SYM_INTERP_मेलनम्_OP_आवरणम्) {
        पक्ष XCB_XKB_SYM_INTERPRET_मेलनम्_NONE_सम्बन्धि:
            sym_interpret->मेलनम् = मेलनम्_NONE;
            भङ्ग;
        पक्ष XCB_XKB_SYM_INTERPRET_मेलनम्_ANY_सम्बन्धि_OR_NONE:
            sym_interpret->मेलनम् = मेलनम्_ANY_OR_NONE;
            भङ्ग;
        पक्ष XCB_XKB_SYM_INTERPRET_मेलनम्_ANY_सम्बन्धि:
            sym_interpret->मेलनम् = मेलनम्_ANY;
            भङ्ग;
        पक्ष XCB_XKB_SYM_INTERPRET_मेलनम्_ALL_सम्बन्धि:
            sym_interpret->मेलनम् = मेलनम्_ALL;
            भङ्ग;
        पक्ष XCB_XKB_SYM_INTERPRET_मेलनम्_EXACTLY:
            sym_interpret->मेलनम् = मेलनम्_EXACTLY;
            भङ्ग;
        स्वभाव:
            log_err_func(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->मेलनम् & XCB_XKB_SYM_INTERP_मेलनम्_OP_आवरणम्);
            गच्छ fail;
        }

        sym_interpret->level_one_only =
            (wire->मेलनम् & XCB_XKB_SYM_INTERP_मेलनम्_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        यदि (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        अन्यथा
            sym_interpret->virtual_mod = अङ्कः_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_कुञ्चिका_संहिता, max_कुञ्चिका_संहिता));
        sym_interpret->अङ्कः_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_अग्र(&iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_compat_map(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_प्रत्युत्तरम्_t *प्रत्युत्तरम् =
        xcb_xkb_get_compat_map_प्रत्युत्तरम्(conn, cookie, रिक्त);

    FAIL_IF_BAD_प्रत्युत्तरम्(प्रत्युत्तरम्, "XkbGetCompatMap");

    यदि (!get_sym_interprets(keymap, conn, प्रत्युत्तरम्))
        गच्छ fail;

    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम true;

fail:
    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम false;
}

स्थिर bool
get_type_नामानि(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
               xcb_xkb_get_नामानि_प्रत्युत्तरम्_t *प्रत्युत्तरम्,
               xcb_xkb_get_नामानि_मानम्_list_t *list)
{
    पूर्णाङ्क कुञ्चिका_type_नामानि_दीर्घता =
        xcb_xkb_get_नामानि_मानम्_list_type_नामानि_दीर्घता(प्रत्युत्तरम्, list);
    xcb_atom_t *कुञ्चिका_type_नामानि_iter =
        xcb_xkb_get_नामानि_मानम्_list_type_नामानि(list);
    पूर्णाङ्क n_स्तराः_प्रत्येकम्_type_दीर्घता =
        xcb_xkb_get_नामानि_मानम्_list_n_स्तराः_प्रत्येकम्_type_दीर्घता(प्रत्युत्तरम्, list);
    uint8_t *n_स्तराः_प्रत्येकम्_type_iter =
        xcb_xkb_get_नामानि_मानम्_list_n_स्तराः_प्रत्येकम्_type(list);
    xcb_atom_t *kt_level_नामानि_iter =
        xcb_xkb_get_नामानि_मानम्_list_kt_level_नामानि(list);

    FAIL_UNLESS(प्रत्युत्तरम्->nTypes == keymap->अङ्कः_types);
    FAIL_UNLESS(कुञ्चिका_type_नामानि_दीर्घता == n_स्तराः_प्रत्येकम्_type_दीर्घता);

    प्रति (पूर्णाङ्क i = 0; i < कुञ्चिका_type_नामानि_दीर्घता; i++) {
        xcb_atom_t wire_type_name = *कुञ्चिका_type_नामानि_iter;
        uint8_t wire_अङ्कः_स्तराः = *n_स्तराः_प्रत्येकम्_type_iter;
        संरचना xkb_कुञ्चिका_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->अङ्कः_स्तराः >= wire_अङ्कः_स्तराः);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_नामानि, type->अङ्कः_स्तराः);
        type->अङ्कः_level_नामानि = type->अङ्कः_स्तराः;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        प्रति (आकार_t j = 0; j < wire_अङ्कः_स्तराः; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_नामानि_iter[j],
                                         &type->level_नामानि[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        प्रति (आकार_t j = wire_अङ्कः_स्तराः; j < type->अङ्कः_स्तराः; j++)
            type->level_नामानि[j] = XKB_ATOM_NONE;

        kt_level_नामानि_iter += wire_अङ्कः_स्तराः;
        कुञ्चिका_type_नामानि_iter++;
        n_स्तराः_प्रत्येकम्_type_iter++;
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_indicator_नामानि(संरचना xkb_keymap *keymap,
                    संरचना x11_atom_interner *interner,
                    xcb_xkb_get_नामानि_प्रत्युत्तरम्_t *प्रत्युत्तरम्,
                    xcb_xkb_get_नामानि_मानम्_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_नामानि_मानम्_list_indicator_नामानि(list);

    FAIL_UNLESS(msb_pos(प्रत्युत्तरम्->indicators) <= keymap->अङ्कः_leds);

    प्रति (अचिह्नित i = 0; i < अङ्कः_INDICATORS; i++) {
        यदि (प्रत्युत्तरम्->indicators & (UINT32_C(1) << i)) {
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
get_vmod_नामानि(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
               xcb_xkb_get_नामानि_प्रत्युत्तरम्_t *प्रत्युत्तरम्,
               xcb_xkb_get_नामानि_मानम्_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_नामानि_मानम्_list_virtual_mod_नामानि(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.अङ्कः_mods =
        अङ्कः_REAL_MODS + MIN(msb_pos(प्रत्युत्तरम्->virtualMods), अङ्कः_VMODS);
    FAIL_UNLESS(keymap->mods.अङ्कः_mods <= XKB_MAX_MODS);

    static_assert(अङ्कः_REAL_MODS + अङ्कः_VMODS <= XKB_MAX_MODS, "");
    प्रति (अचिह्नित i = 0; i < अङ्कः_VMODS; i++) {
        यदि (प्रत्युत्तरम्->virtualMods & (1u << i)) {
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
get_गणः_नामानि(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
                xcb_xkb_get_नामानि_प्रत्युत्तरम्_t *प्रत्युत्तरम्,
                xcb_xkb_get_नामानि_मानम्_list_t *list)
{
    पूर्णाङ्क दीर्घता = xcb_xkb_get_नामानि_मानम्_list_groups_दीर्घता(प्रत्युत्तरम्, list);
    xcb_atom_t *iter = xcb_xkb_get_नामानि_मानम्_list_groups(list);

    keymap->अङ्कः_गणः_नामानि = msb_pos(प्रत्युत्तरम्->गणः_नामानि_2);
    ALLOC_OR_FAIL(keymap->गणः_नामानि, keymap->अङ्कः_गणः_नामानि);

    प्रति (पूर्णाङ्क i = 0; i < दीर्घता; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->गणः_नामानि[i]);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_कुञ्चिका_नामानि(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_नामानि_प्रत्युत्तरम्_t *प्रत्युत्तरम्,
              xcb_xkb_get_नामानि_मानम्_list_t *list)
{
    पूर्णाङ्क दीर्घता = xcb_xkb_get_नामानि_मानम्_list_कुञ्चिका_नामानि_दीर्घता(प्रत्युत्तरम्, list);
    xcb_xkb_कुञ्चिका_name_iterator_t iter =
        xcb_xkb_get_नामानि_मानम्_list_कुञ्चिका_नामानि_iterator(प्रत्युत्तरम्, list);

    FAIL_UNLESS(प्रत्युत्तरम्->min_कुञ्चिका_संहिता_2 == keymap->min_कुञ्चिका_संहिता);
    FAIL_UNLESS(प्रत्युत्तरम्->max_कुञ्चिका_संहिता_2 == keymap->max_कुञ्चिका_संहिता);
    FAIL_UNLESS(प्रत्युत्तरम्->first_कुञ्चिका == keymap->min_कुञ्चिका_संहिता);
    FAIL_UNLESS(प्रत्युत्तरम्->first_कुञ्चिका + प्रत्युत्तरम्->n_कुञ्चिकाः - 1U == keymap->max_कुञ्चिका_संहिता);

    प्रति (पूर्णाङ्क i = 0; i < दीर्घता; i++) {
        xcb_xkb_कुञ्चिका_name_t *wire = iter.दत्त;
        xkb_atom_t *कुञ्चिका_name = &keymap->कुञ्चिकाः[प्रत्युत्तरम्->first_कुञ्चिका + i].name;

        यदि (wire->name[0] == '\0') {
            *कुञ्चिका_name = XKB_ATOM_NONE;
        }
        अन्यथा {
            *कुञ्चिका_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_कुञ्चिका_NAME_दीर्घता));
            यदि (!*कुञ्चिका_name)
                प्रत्यागम false;
        }

        xcb_xkb_कुञ्चिका_name_अग्र(&iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_aliases(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_नामानि_प्रत्युत्तरम्_t *प्रत्युत्तरम्,
            xcb_xkb_get_नामानि_मानम्_list_t *list)
{
    पूर्णाङ्क दीर्घता = xcb_xkb_get_नामानि_मानम्_list_कुञ्चिका_aliases_दीर्घता(प्रत्युत्तरम्, list);
    xcb_xkb_कुञ्चिका_alias_iterator_t iter =
        xcb_xkb_get_नामानि_मानम्_list_कुञ्चिका_aliases_iterator(प्रत्युत्तरम्, list);

    keymap->अङ्कः_कुञ्चिका_aliases = प्रत्युत्तरम्->n_कुञ्चिका_Aliases;
    ALLOC_OR_FAIL(keymap->कुञ्चिका_aliases, keymap->अङ्कः_कुञ्चिका_aliases);

    प्रति (पूर्णाङ्क i = 0; i < दीर्घता; i++) {
        xcb_xkb_कुञ्चिका_alias_t *wire = iter.दत्त;
        संरचना xkb_कुञ्चिका_alias *alias = &keymap->कुञ्चिका_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_कुञ्चिका_NAME_दीर्घता));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_कुञ्चिका_NAME_दीर्घता));
        यदि (!alias->real || !alias->alias)
            गच्छ fail;

        xcb_xkb_कुञ्चिका_alias_अग्र(&iter);
    }

    प्रत्यागम true;

fail:
    प्रत्यागम false;
}

स्थिर bool
get_नामानि(संरचना xkb_keymap *keymap, संरचना x11_atom_interner *interner,
          xcb_xkb_get_नामानि_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_नामानि_प्रत्युत्तरम्_t *प्रत्युत्तरम् =
        xcb_xkb_get_नामानि_प्रत्युत्तरम्(conn, cookie, रिक्त);
    xcb_xkb_get_नामानि_मानम्_list_t list;

    FAIL_IF_BAD_प्रत्युत्तरम्(प्रत्युत्तरम्, "XkbGetNames");

    FAIL_UNLESS((प्रत्युत्तरम्->which & get_नामानि_required) == get_नामानि_required);

    xcb_xkb_get_नामानि_मानम्_list_unpack(xcb_xkb_get_नामानि_मानम्_list(प्रत्युत्तरम्),
                                        प्रत्युत्तरम्->nTypes,
                                        प्रत्युत्तरम्->indicators,
                                        प्रत्युत्तरम्->virtualMods,
                                        प्रत्युत्तरम्->गणः_नामानि_2,
                                        प्रत्युत्तरम्->n_कुञ्चिकाः,
                                        प्रत्युत्तरम्->n_कुञ्चिका_Aliases,
                                        प्रत्युत्तरम्->nRadioGroups,
                                        प्रत्युत्तरम्->which,
                                        &list);

    x11_atom_interner_get_escaped_atom_name(interner, list.keycodesName,
                                            &keymap->keycodes_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.symbolsName,
                                            &keymap->symbols_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.typesName,
                                            &keymap->types_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.compatName,
                                            &keymap->compat_section_name);
    यदि (!get_type_नामानि(keymap, interner, प्रत्युत्तरम्, &list) ||
        !get_indicator_नामानि(keymap, interner, प्रत्युत्तरम्, &list) ||
        !get_vmod_नामानि(keymap, interner, प्रत्युत्तरम्, &list) ||
        !get_गणः_नामानि(keymap, interner, प्रत्युत्तरम्, &list) ||
        !get_कुञ्चिका_नामानि(keymap, conn, प्रत्युत्तरम्, &list) ||
        !get_aliases(keymap, conn, प्रत्युत्तरम्, &list))
        गच्छ fail;

    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम true;

fail:
    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम false;
}

स्थिर bool
get_controls(संरचना xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_प्रत्युत्तरम्_t *प्रत्युत्तरम् =
        xcb_xkb_get_controls_प्रत्युत्तरम्(conn, cookie, रिक्त);

    FAIL_IF_BAD_प्रत्युत्तरम्(प्रत्युत्तरम्, "XkbGetControls");
    FAIL_UNLESS(प्रत्युत्तरम्->अङ्कः_Groups > 0 && प्रत्युत्तरम्->अङ्कः_Groups <= 4);

    keymap->अङ्कः_groups = प्रत्युत्तरम्->अङ्कः_Groups;

    FAIL_UNLESS(keymap->max_कुञ्चिका_संहिता < XCB_XKB_CONST_प्रत्येकम्_कुञ्चिका_BIT_ARRAY_आकार * 8);

    प्रति (xkb_keycode_t i = keymap->min_कुञ्चिका_संहिता; i <= keymap->max_कुञ्चिका_संहिता; i++)
        keymap->कुञ्चिकाः[i].repeats = (प्रत्युत्तरम्->प्रत्येकम्_कुञ्चिका_Repeat[i / 8] & (1u << (i % 8)));

    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम true;

fail:
    मुक्त(प्रत्युत्तरम्);
    प्रत्यागम false;
}

संरचना xkb_keymap *
xkb_x11_keymap_new_from_device(संरचना xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               गणना xkb_keymap_compile_flags flags)
{
    यदि (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_सन्देशः_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        प्रत्यागम रिक्त;
    }

    ध्रुव गणना xkb_keymap_format format = XKB_KEYMAP_FORMAT_TEXT_V1;
    संरचना xkb_keymap * ध्रुव keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    यदि (!keymap)
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
        xcb_xkb_get_indicator_map(conn, device_id, ALL_INDICATORS_आवरणम्);
    xcb_xkb_get_compat_map_cookie_t compat_map_cookie =
        xcb_xkb_get_compat_map(conn, device_id, 0, true, 0, 0);
    xcb_xkb_get_नामानि_cookie_t नामानि_cookie =
        xcb_xkb_get_नामानि(conn, device_id, get_नामानि_wanted);
    xcb_xkb_get_controls_cookie_t controls_cookie =
        xcb_xkb_get_controls(conn, device_id);

    यदि (!get_map(keymap, conn, map_cookie))
        गच्छ err_map;
    यदि (!get_indicator_map(keymap, conn, indicator_map_cookie))
        गच्छ err_indicator_map;
    यदि (!get_compat_map(keymap, conn, compat_map_cookie))
        गच्छ err_compat_map;
    यदि (!get_नामानि(keymap, &interner, नामानि_cookie))
        गच्छ err_नामानि;
    यदि (!get_controls(keymap, conn, controls_cookie))
        गच्छ err_controls;
    x11_atom_interner_आवर्तनम्_trip(&interner);
    यदि (interner.had_error)
        गच्छ err_interner;

    प्रत्यागम keymap;

err_map:
    xcb_discard_प्रत्युत्तरम्(conn, indicator_map_cookie.sequence);
err_indicator_map:
    xcb_discard_प्रत्युत्तरम्(conn, compat_map_cookie.sequence);
err_compat_map:
    xcb_discard_प्रत्युत्तरम्(conn, नामानि_cookie.sequence);
err_नामानि:
    xcb_discard_प्रत्युत्तरम्(conn, controls_cookie.sequence);
err_controls:
    x11_atom_interner_आवर्तनम्_trip(&interner);
err_interner:
    xkb_keymap_unref(keymap);
    प्रत्यागम रिक्त;
}
