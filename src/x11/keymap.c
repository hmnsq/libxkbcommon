/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <xcb/xkb.h>

#అన్తర్భావ "darray.h"
#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcommon/xkbcommon-keysyms.h"
#అన్తర్భావ "atom.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "x11-priv.h"

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
#నిర్వచించు అంకె_REAL_MODS 8u
/* XkbNumVirtualMods. */
#నిర్వచించు అంకె_VMODS 16u
/* XkbNoModifier. */
#నిర్వచించు NO_MODIFIER 0xff
/* XkbNumIndicators. */
#నిర్వచించు అంకె_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#నిర్వచించు ALL_INDICATORS_ముసుగు 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#నిర్వచించు FAIL_UNLESS(సమాసం) చేయి {                              \
    ఒకవేళ (!(సమాసం)) {                                          \
        log_err(keymap->ctx, XKB_LOG_సందేశం_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(సమాసం));                 \
        వెళ్ళు fail;                                          \
    }                                                       \
} అయితే (0)

#నిర్వచించు FAIL_IF_BAD_జవాబు(జవాబు, request_name) చేయి {         \
    ఒకవేళ (!(జవాబు)) {                                           \
        log_err(keymap->ctx, XKB_LOG_సందేశం_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        వెళ్ళు fail;                                          \
    }                                                       \
} అయితే (0)

#నిర్వచించు ALLOC_OR_FAIL(arr, nmemb) చేయి {                      \
    ఒకవేళ ((nmemb) > 0) {                                      \
        (arr) = సున్నా_జ్ఞాపకం_ఇవ్వు((nmemb), పరిమాణం(*(arr)));            \
        ఒకవేళ (!(arr))                                         \
            వెళ్ళు fail;                                      \
    }                                                       \
} అయితే (0)

స్థిర స్థిరరాశి xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_కీలకం_TYPES |
     XCB_XKB_MAP_PART_కీలకం_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_కీలకం_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

స్థిర స్థిరరాశి xcb_xkb_name_detail_t get_పేర్లు_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_కీలకం_TYPE_పేర్లు |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_పేర్లు |
     XCB_XKB_NAME_DETAIL_INDICATOR_పేర్లు |
     XCB_XKB_NAME_DETAIL_కీలకం_పేర్లు |
     XCB_XKB_NAME_DETAIL_కీలకం_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_పేర్లు |
     XCB_XKB_NAME_DETAIL_సమూహం_పేర్లు);
స్థిర స్థిరరాశి xcb_xkb_name_detail_t get_పేర్లు_required =
    (XCB_XKB_NAME_DETAIL_కీలకం_TYPE_పేర్లు |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_పేర్లు |
     XCB_XKB_NAME_DETAIL_కీలకం_పేర్లు |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_పేర్లు);


స్థిర xkb_mod_ముసుగు_t
translate_mods(uint8_t rmods, uint16_t vmods_దిగువ, uint16_t vmods_ఎగువ)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    తిరిగి
        ((xkb_mod_ముసుగు_t) rmods) |
        ((xkb_mod_ముసుగు_t) vmods_దిగువ << 8) |
        ((xkb_mod_ముసుగు_t) vmods_ఎగువ << 16);
}

గణన xkb_action_controls
translate_controls_ముసుగు(uint32_t wire)
{
    గణన xkb_action_controls ret = 0;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_REPEAT_కీలకాలు)
        ret |= CONTROL_REPEAT;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_SLOW_కీలకాలు)
        ret |= CONTROL_SLOW;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_BOUNCE_కీలకాలు)
        ret |= CONTROL_DEBOUNCE;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_STICKY_కీలకాలు)
        ret |= CONTROL_STICKY_కీలకాలు;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_MOUSE_కీలకాలు)
        ret |= CONTROL_MOUSE_కీలకాలు;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_MOUSE_కీలకాలు_ACCEL)
        ret |= CONTROL_MOUSE_కీలకాలు_ACCEL;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_కీలకాలు)
        ret |= CONTROL_AX;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_ముసుగు)
        ret |= CONTROL_AX_TIMEOUT;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_ముసుగు)
        ret |= CONTROL_AX_FEEDBACK;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_ముసుగు)
        ret |= CONTROL_BELL;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_ముసుగు)
        ret |= CONTROL_OVERLAY1;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_ముసుగు)
        ret |= CONTROL_OVERLAY2;
    ఒకవేళ (wire & XCB_XKB_BOOL_CTRL_IGNORE_సమూహం_తాళం_ముసుగు)
        ret |= CONTROL_IGNORE_సమూహం_తాళం;
    /* Some controls are not supported and don't appear here. */
    తిరిగి ret;
}

స్థిర bool
translate_action(సంఘం xkb_action *action, స్థిరరాశి xcb_xkb_action_t *wire,
                 xkb_keycode_t min_కీలకం_సంకేతం, xkb_keycode_t max_కీలకం_సంకేతం)
{
    ఎంపిక (wire->type) {
    స్థితి XCB_XKB_SA_TYPE_సమితి_MODS:
        action->type = ACTION_TYPE_MOD_సమితి;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_దిగువ_2,
                                                wire->setmods.vmods_ఎగువ_2);
        action->mods.mods.ముసుగు = translate_mods(wire->setmods.ముసుగు, 0, 0);

        ఒకవేళ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_తాళం_CLEAR;
        ఒకవేళ (wire->setmods.flags & XCB_XKB_SA_LATCH_గమ్యం_తాళం)
            action->mods.flags |= ACTION_LATCH_గమ్యం_తాళం;
        ఒకవేళ (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_దిగువ_2,
                                                wire->latchmods.vmods_ఎగువ_2);
        action->mods.mods.ముసుగు = translate_mods(wire->latchmods.ముసుగు, 0, 0);

        ఒకవేళ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_తాళం_CLEAR;
        ఒకవేళ (wire->latchmods.flags & XCB_XKB_SA_LATCH_గమ్యం_తాళం)
            action->mods.flags |= ACTION_LATCH_గమ్యం_తాళం;
        ఒకవేళ (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_తాళం_MODS:
        action->type = ACTION_TYPE_MOD_తాళం;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_దిగువ_2,
                                                wire->lockmods.vmods_ఎగువ_2);
        action->mods.mods.ముసుగు = translate_mods(wire->lockmods.ముసుగు, 0, 0);

        ఒకవేళ (wire->lockmods.flags & XCB_XKB_SA_ISO_తాళం_FLAG_NO_తాళం)
            action->mods.flags |= ACTION_తాళం_NO_తాళం;
        ఒకవేళ (wire->lockmods.flags & XCB_XKB_SA_ISO_తాళం_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_తాళం_NO_UNLOCK;
        ఒకవేళ (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_సమితి_సమూహం:
        action->type = ACTION_TYPE_సమూహం_సమితి;

        action->సమూహం.సమూహం = (int32_t)wire->setgroup.సమూహం;

        ఒకవేళ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->సమూహం.flags |= ACTION_తాళం_CLEAR;
        ఒకవేళ (wire->setmods.flags & XCB_XKB_SA_LATCH_గమ్యం_తాళం)
            action->సమూహం.flags |= ACTION_LATCH_గమ్యం_తాళం;
        ఒకవేళ (wire->setmods.flags & XCB_XKB_SA_ISO_తాళం_FLAG_సమూహం_ABSOLUTE)
            action->సమూహం.flags |= ACTION_ABSOLUTE_SWITCH;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_LATCH_సమూహం:
        action->type = ACTION_TYPE_సమూహం_LATCH;

        action->సమూహం.సమూహం = (int32_t)wire->latchgroup.సమూహం;

        ఒకవేళ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->సమూహం.flags |= ACTION_తాళం_CLEAR;
        ఒకవేళ (wire->latchmods.flags & XCB_XKB_SA_LATCH_గమ్యం_తాళం)
            action->సమూహం.flags |= ACTION_LATCH_గమ్యం_తాళం;
        ఒకవేళ (wire->latchmods.flags & XCB_XKB_SA_ISO_తాళం_FLAG_సమూహం_ABSOLUTE)
            action->సమూహం.flags |= ACTION_ABSOLUTE_SWITCH;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_తాళం_సమూహం:
        action->type = ACTION_TYPE_సమూహం_తాళం;

        action->సమూహం.సమూహం = (int32_t)wire->lockgroup.సమూహం;

        ఒకవేళ (wire->lockgroup.flags & XCB_XKB_SA_ISO_తాళం_FLAG_సమూహం_ABSOLUTE)
            action->సమూహం.flags |= ACTION_ABSOLUTE_SWITCH;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_దిగువ | ((uint16_t) wire->moveptr.x_ఎగువ << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_దిగువ | ((uint16_t) wire->moveptr.y_ఎగువ << 8));

        ఒకవేళ (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        ఒకవేళ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        ఒకవేళ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.లెక్క = wire->ptrbtn.లెక్క;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_తాళం_PTR_BTN:
        action->type = ACTION_TYPE_PTR_తాళం;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.లెక్క = 0;

        ఒకవేళ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_తాళం_FLAG_NO_తాళం)
            action->btn.flags |= ACTION_తాళం_NO_తాళం;
        ఒకవేళ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_తాళం_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_తాళం_NO_UNLOCK;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_సమితి_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.విలువ = wire->setptrdflt.విలువ;

        ఒకవేళ (wire->setptrdflt.flags & XCB_XKB_SA_సమితి_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        ఒకవేళ (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        ఒకవేళ (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_సమితి_CONTROLS:
        action->type = ACTION_TYPE_CTRL_సమితి;
        {
            స్థిరరాశి uint16_t ముసుగు = (wire->setcontrols.bool_Ctrls_దిగువ |
                                   (wire->setcontrols.bool_Ctrls_ఎగువ << 8));
            action->ctrls.ctrls = translate_controls_ముసుగు(ముసుగు);
        }
        ఆపు;
    స్థితి XCB_XKB_SA_TYPE_తాళం_CONTROLS:
        action->type = ACTION_TYPE_CTRL_తాళం;
        {
            స్థిరరాశి uint16_t ముసుగు = (wire->lockcontrols.bool_Ctrls_దిగువ |
                                   (wire->lockcontrols.bool_Ctrls_ఎగువ << 8));
            action->ctrls.ctrls = translate_controls_ముసుగు(ముసుగు);
        }
        ఆపు;

    స్థితి XCB_XKB_SA_TYPE_REDIRECT_కీలకం:
        action->type = ACTION_TYPE_REDIRECT_కీలకం;
        ఒకవేళ (wire->redirect.newkey < min_కీలకం_సంకేతం ||
            wire->redirect.newkey > max_కీలకం_సంకేతం)
                తిరిగి false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.ముసుగు,
            wire->redirect.vmods_ముసుగు_దిగువ,
            wire->redirect.vmods_ముసుగు_ఎగువ
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_దిగువ_2,
                                               wire->redirect.vmods_ఎగువ_2);
        ఆపు;

    స్థితి XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        ఆపు;

    /* We don't support these. */
    స్థితి XCB_XKB_SA_TYPE_ISO_తాళం:
    స్థితి XCB_XKB_SA_TYPE_ACTION_సందేశం:
    స్థితి XCB_XKB_SA_TYPE_DEVICE_BTN:
    స్థితి XCB_XKB_SA_TYPE_తాళం_DEVICE_BTN:
    స్థితి XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        ఆపు;

    అప్రమేయ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_అంతర్గత == 20 &&
                      ACTION_TYPE_అంతర్గత + 1 == ACTION_TYPE_అంకె_ENTRIES,
                      "Missing action type");

        ఒకవేళ (wire->type < ACTION_TYPE_వ్యక్తిగత) {
            action->type = ACTION_TYPE_NONE;
            ఆపు;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(పరిమాణం(action->priv.దత్తాంశం) == 7 &&
                      పరిమాణం(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        జ్ఞాపకం_నకలు(action->priv.దత్తాంశం, wire->noaction.pad0, 7);
        ఆపు;
    }
    తిరిగి true;
}

స్థిర bool
get_types(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_జవాబు_t *జవాబు, xcb_xkb_get_map_map_t *map)
{
    పూర్ణాంకం types_నిడివి = xcb_xkb_get_map_map_types_rtrn_నిడివి(జవాబు, map);
    xcb_xkb_కీలకం_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(జవాబు, map);

    FAIL_UNLESS(జవాబు->మొదటి_Type == 0);

    keymap->అంకె_types = జవాబు->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->అంకె_types);
    ప్రతిజ్ఞా(types_నిడివి == (పూర్ణాంకం) keymap->అంకె_types);

    కొరకు (పూర్ణాంకం i = 0; i < types_నిడివి; i++) {
        xcb_xkb_కీలకం_type_t *wire_type = types_iter.దత్తాంశం;
        నిర్మాణం xkb_కీలకం_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->అంకె_స్థాయిలు_2 > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.ముసుగు = translate_mods(wire_type->mods_ముసుగు, 0, 0);
        type->అంకె_స్థాయిలు = wire_type->అంకె_స్థాయిలు_2;

        {
            పూర్ణాంకం entries_నిడివి = xcb_xkb_కీలకం_type_map_నిడివి(wire_type);
            xcb_xkb_kt_map_నమోదు_iterator_t entries_iter =
                xcb_xkb_కీలకం_type_map_iterator(wire_type);

            type->అంకె_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->అంకె_entries);
            ప్రతిజ్ఞా(entries_నిడివి == (పూర్ణాంకం) type->అంకె_entries);

            కొరకు (పూర్ణాంకం j = 0; j < entries_నిడివి; j++) {
                xcb_xkb_kt_map_నమోదు_t *wire_నమోదు = entries_iter.దత్తాంశం;
                నిర్మాణం xkb_కీలకం_type_నమోదు *నమోదు_2 = &type->entries[j];

                FAIL_UNLESS(wire_నమోదు->level < type->అంకె_స్థాయిలు);

                నమోదు_2->level = wire_నమోదు->level;
                నమోదు_2->mods.mods = translate_mods(wire_నమోదు->mods_mods,
                                                  wire_నమోదు->mods_vmods, 0);
                నమోదు_2->mods.ముసుగు = translate_mods(wire_నమోదు->mods_ముసుగు, 0, 0);

                xcb_xkb_kt_map_నమోదు_తదుపరి(&entries_iter);
            }
        }

        {
            పూర్ణాంకం preserves_నిడివి = xcb_xkb_కీలకం_type_preserve_నిడివి(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_కీలకం_type_preserve_iterator(wire_type);

            FAIL_UNLESS((గుర్తులేని) preserves_నిడివి <= type->అంకె_entries);

            కొరకు (పూర్ణాంకం j = 0; j < preserves_నిడివి; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.దత్తాంశం;
                నిర్మాణం xkb_కీలకం_type_నమోదు *నమోదు_2 = &type->entries[j];

                నమోదు_2->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                నమోదు_2->preserve.ముసుగు = translate_mods(wire_preserve->ముసుగు, 0, 0);

                xcb_xkb_mod_def_తదుపరి(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_కీలకం_type_తదుపరి(&types_iter);
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_sym_maps(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_జవాబు_t *జవాబు, xcb_xkb_get_map_map_t *map)
{
    పూర్ణాంకం sym_maps_నిడివి = xcb_xkb_get_map_map_syms_rtrn_నిడివి(జవాబు, map);
    xcb_xkb_కీలకం_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(జవాబు, map);

    FAIL_UNLESS(జవాబు->min_కీలకం_సంకేతం_2 <= జవాబు->max_కీలకం_సంకేతం_2);
    FAIL_UNLESS(జవాబు->మొదటి_కీలకం_Sym >= జవాబు->min_కీలకం_సంకేతం_2);
    FAIL_UNLESS(జవాబు->మొదటి_కీలకం_Sym + జవాబు->n_కీలకం_Syms <= జవాబు->max_కీలకం_సంకేతం_2 + 1);

    keymap->min_కీలకం_సంకేతం = జవాబు->min_కీలకం_సంకేతం_2;
    keymap->max_కీలకం_సంకేతం = జవాబు->max_కీలకం_సంకేతం_2;
    keymap->అంకె_కీలకాలు = keymap->అంకె_కీలకాలు_దిగువ = జవాబు->max_కీలకం_సంకేతం_2 + 1;

    ALLOC_OR_FAIL(keymap->కీలకాలు, keymap->max_కీలకం_సంకేతం + 1);

    కొరకు (xkb_keycode_t kc = keymap->min_కీలకం_సంకేతం; kc <= keymap->max_కీలకం_సంకేతం; kc++)
        keymap->కీలకాలు[kc].keycode = kc;

    కొరకు (పూర్ణాంకం i = 0; i < sym_maps_నిడివి; i++) {
        xcb_xkb_కీలకం_sym_map_t *wire_sym_map = sym_maps_iter.దత్తాంశం;
        నిర్మాణం xkb_కీలకం *కీలకం = &keymap->కీలకాలు[జవాబు->మొదటి_కీలకం_Sym + i];

        కీలకం->అంకె_groups = wire_sym_map->సమూహం_Info & 0x0f;
        FAIL_UNLESS(కీలకం->అంకె_groups <= ARRAY_కొలత(wire_sym_map->kt_సూచీ));
        ALLOC_OR_FAIL(కీలకం->groups, కీలకం->అంకె_groups);

        కొరకు (xkb_layout_సూచీ_t j = 0; j < కీలకం->అంకె_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_సూచీ[j] < keymap->అంకె_types);
            కీలకం->groups[j].type = &keymap->types[wire_sym_map->kt_సూచీ[j]];

            ALLOC_OR_FAIL(కీలకం->groups[j].స్థాయిలు, కీలకం->groups[j].type->అంకె_స్థాయిలు);
        }

        కీలకం->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య = (wire_sym_map->సమూహం_Info & 0x30) >> 4;

        FAIL_UNLESS(కీలకం->బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య <= కీలకం->అంకె_groups);

        ఒకవేళ (wire_sym_map->సమూహం_Info & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            కీలకం->బహిర్గతం_యొక్క_range_సమూహం_policy = XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP;
        లేకపోతే ఒకవేళ (wire_sym_map->సమూహం_Info & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            కీలకం->బహిర్గతం_యొక్క_range_సమూహం_policy = XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT;
        లేకపోతే
            కీలకం->బహిర్గతం_యొక్క_range_సమూహం_policy = XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP;

        {
            పూర్ణాంకం syms_నిడివి = xcb_xkb_కీలకం_sym_map_syms_నిడివి(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_కీలకం_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((గుర్తులేని) syms_నిడివి == wire_sym_map->వెడల్పు * కీలకం->అంకె_groups);

            ఒకవేళ (syms_నిడివి > 0)
                కీలకం->explicit |= EXPLICIT_SYMBOLS;

            కొరకు (xkb_layout_సూచీ_t సమూహం = 0; సమూహం < కీలకం->అంకె_groups; సమూహం++) {
                కొరకు (xkb_level_సూచీ_t level = 0; level < wire_sym_map->వెడల్పు; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    ప్రతిజ్ఞా(కీలకం->groups[సమూహం].type != ఖాళీ);
                    ఒకవేళ (level < కీలకం->groups[సమూహం].type->అంకె_స్థాయిలు) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        కీలకం->groups[సమూహం].స్థాయిలు[level].అంకె_syms = 1;
                        కీలకం->groups[సమూహం].స్థాయిలు[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        కీలకం->groups[సమూహం].స్థాయిలు[level].పై =
                            xkb_keysym_గమ్యం_పై(wire_keysym);
                        ఒకవేళ (wire_keysym != XKB_కీలకం_No_చిహ్నం)
                            కీలకం->groups[సమూహం].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_కీలకం_sym_map_తదుపరి(&sym_maps_iter);
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_actions(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_జవాబు_t *జవాబు, xcb_xkb_get_map_map_t *map)
{
    పూర్ణాంకం acts_లెక్క_నిడివి =
        xcb_xkb_get_map_map_acts_rtrn_లెక్క_నిడివి(జవాబు, map);
    uint8_t *acts_లెక్క_iter = xcb_xkb_get_map_map_acts_rtrn_లెక్క(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(జవాబు, map);
    xcb_xkb_కీలకం_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(జవాబు, map);

    FAIL_UNLESS(జవాబు->మొదటి_కీలకం_Action == keymap->min_కీలకం_సంకేతం);
    FAIL_UNLESS(జవాబు->మొదటి_కీలకం_Action + జవాబు->n_కీలకం_Actions ==
                keymap->max_కీలకం_సంకేతం + 1);

    కొరకు (పూర్ణాంకం i = 0; i < acts_లెక్క_నిడివి; i++) {
        xcb_xkb_కీలకం_sym_map_t *wire_sym_map = sym_maps_iter.దత్తాంశం;
        పూర్ణాంకం syms_నిడివి = xcb_xkb_కీలకం_sym_map_syms_నిడివి(wire_sym_map);
        uint8_t wire_లెక్క = *acts_లెక్క_iter;
        నిర్మాణం xkb_కీలకం *కీలకం = &keymap->కీలకాలు[జవాబు->మొదటి_కీలకం_Action + i];

        FAIL_UNLESS((గుర్తులేని) syms_నిడివి == wire_sym_map->వెడల్పు * కీలకం->అంకె_groups);
        FAIL_UNLESS(wire_లెక్క == 0 || wire_లెక్క == syms_నిడివి);

        స్థిరరాశి xkb_keycode_t min_కీలకం_సంకేతం = keymap->min_కీలకం_సంకేతం;
        స్థిరరాశి xkb_keycode_t max_కీలకం_సంకేతం = keymap->max_కీలకం_సంకేతం;

        ఒకవేళ (wire_లెక్క != 0) {
            కొరకు (xkb_layout_సూచీ_t సమూహం = 0; సమూహం < కీలకం->అంకె_groups; సమూహం++) {
                కొరకు (xkb_level_సూచీ_t level = 0; level < wire_sym_map->వెడల్పు; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.దత్తాంశం;

                    ఒకవేళ (level < కీలకం->groups[సమూహం].type->అంకె_స్థాయిలు) {
                        కీలకం->groups[సమూహం].స్థాయిలు[level].అంకె_actions = 1;
                        సంఘం xkb_action *action =
                            &కీలకం->groups[సమూహం].స్థాయిలు[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_కీలకం_సంకేతం, max_కీలకం_సంకేతం));

                        ఒకవేళ (action->type != ACTION_TYPE_NONE) {
                            కీలకం->groups[సమూహం].implicit_actions =
                                !(కీలకం->explicit & EXPLICIT_INTERP);
                        } లేకపోతే ఒకవేళ (కీలకం->groups[సమూహం].స్థాయిలు[level].s.sym ==
                                   XKB_కీలకం_No_చిహ్నం) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            కీలకం->groups[సమూహం].స్థాయిలు[level].అంకె_syms = 0;
                            కీలకం->groups[సమూహం].స్థాయిలు[level].అంకె_actions = 0;
                        }
                    }

                    xcb_xkb_action_తదుపరి(&acts_iter);
                }

                ఒకవేళ (కీలకం->groups[సమూహం].implicit_actions)
                    కీలకం->implicit_actions = true;
            }
        }

        acts_లెక్క_iter++;
        xcb_xkb_కీలకం_sym_map_తదుపరి(&sym_maps_iter);
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_vmods(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_జవాబు_t *జవాబు, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.అంకె_mods =
        అంకె_REAL_MODS + MIN(msb_pos(జవాబు->virtualMods), అంకె_VMODS);
    FAIL_UNLESS(keymap->mods.అంకె_mods <= XKB_MAX_MODS);

    static_assert(అంకె_REAL_MODS + అంకె_VMODS <= XKB_MAX_MODS, "");
    కొరకు (గుర్తులేని i = 0; i < అంకె_VMODS; i++) {
        ఒకవేళ (జవాబు->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            నిర్మాణం xkb_mod *mod = &keymap->mods.mods[అంకె_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_explicits(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_జవాబు_t *జవాబు, xcb_xkb_get_map_map_t *map)
{
    పూర్ణాంకం నిడివి = xcb_xkb_get_map_map_explicit_rtrn_నిడివి(జవాబు, map);
    xcb_xkb_సమితి_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(జవాబు, map);

    కొరకు (పూర్ణాంకం i = 0; i < నిడివి; i++) {
        xcb_xkb_సమితి_explicit_t *wire = iter.దత్తాంశం;
        నిర్మాణం xkb_కీలకం *కీలకం;

        FAIL_UNLESS(wire->keycode >= keymap->min_కీలకం_సంకేతం &&
                    wire->keycode <= keymap->max_కీలకం_సంకేతం);

        కీలకం = &keymap->కీలకాలు[wire->keycode];

        ఒకవేళ ((wire->explicit & XCB_XKB_EXPLICIT_కీలకం_TYPE_1) &&
            కీలకం->అంకె_groups > 0) {
            కీలకం->groups[0].explicit_type = true;
            కీలకం->explicit |= EXPLICIT_TYPES;
        }
        ఒకవేళ ((wire->explicit & XCB_XKB_EXPLICIT_కీలకం_TYPE_2) &&
            కీలకం->అంకె_groups > 1) {
            కీలకం->groups[1].explicit_type = true;
            కీలకం->explicit |= EXPLICIT_TYPES;
        }
        ఒకవేళ ((wire->explicit & XCB_XKB_EXPLICIT_కీలకం_TYPE_3) &&
            కీలకం->అంకె_groups > 2) {
            కీలకం->groups[2].explicit_type = true;
            కీలకం->explicit |= EXPLICIT_TYPES;
        }
        ఒకవేళ ((wire->explicit & XCB_XKB_EXPLICIT_కీలకం_TYPE_4) &&
            కీలకం->అంకె_groups > 3) {
            కీలకం->groups[3].explicit_type = true;
            కీలకం->explicit |= EXPLICIT_TYPES;
        }
        ఒకవేళ (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            కీలకం->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            కొరకు (xkb_layout_సూచీ_t l = 0; l < కీలకం->అంకె_groups; l++) {
                కీలకం->groups[l].explicit_actions = true;
            }
        }
        ఒకవేళ (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            కీలకం->explicit |= EXPLICIT_REPEAT;
        ఒకవేళ (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            కీలకం->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_సమితి_explicit_తదుపరి(&iter);
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_modmaps(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_జవాబు_t *జవాబు, xcb_xkb_get_map_map_t *map)
{
    పూర్ణాంకం నిడివి = xcb_xkb_get_map_map_modmap_rtrn_నిడివి(జవాబు, map);
    xcb_xkb_కీలకం_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(జవాబు, map);

    darray(Mod_Map_నమోదు) modmaps = darray_new();

    కొరకు (పూర్ణాంకం i = 0; i < నిడివి; i++) {
        xcb_xkb_కీలకం_mod_map_t *wire = iter.దత్తాంశం;
        నిర్మాణం xkb_కీలకం *కీలకం;

        FAIL_UNLESS(wire->keycode >= keymap->min_కీలకం_సంకేతం &&
                    wire->keycode <= keymap->max_కీలకం_సంకేతం);

        కీలకం = &keymap->కీలకాలు[wire->keycode];
        కీలకం->modmap = wire->mods;

        darray_append(modmaps, (Mod_Map_నమోదు) {
            .కీలకం_సంకేతం = కీలకం->keycode,
            .mods = కీలకం->modmap,
            .have_చిహ్నం = false,
            .u.కీలకం_Name = కీలకం->name
        });

        xcb_xkb_కీలకం_mod_map_తదుపరి(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->అంకె_modmaps);
    తిరిగి true;

fail:
    darray_free(modmaps);
    తిరిగి false;
}

స్థిర bool
get_vmodmaps(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_జవాబు_t *జవాబు, xcb_xkb_get_map_map_t *map)
{
    పూర్ణాంకం నిడివి = xcb_xkb_get_map_map_vmodmap_rtrn_నిడివి(జవాబు, map);
    xcb_xkb_కీలకం_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(జవాబు, map);

    కొరకు (పూర్ణాంకం i = 0; i < నిడివి; i++) {
        xcb_xkb_కీలకం_v_mod_map_t *wire = iter.దత్తాంశం;
        నిర్మాణం xkb_కీలకం *కీలకం;

        FAIL_UNLESS(wire->keycode >= keymap->min_కీలకం_సంకేతం &&
                    wire->keycode <= keymap->max_కీలకం_సంకేతం);

        కీలకం = &keymap->కీలకాలు[wire->keycode];
        కీలకం->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_కీలకం_v_mod_map_తదుపరి(&iter);
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_map(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_జవాబు_t *జవాబు = xcb_xkb_get_map_జవాబు(conn, cookie, ఖాళీ);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_జవాబు(జవాబు, "XkbGetMap");

    ఒకవేళ ((జవాబు->present & get_map_required_components) != get_map_required_components)
        వెళ్ళు fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(జవాబు),
                               జవాబు->nTypes,
                               జవాబు->n_కీలకం_Syms,
                               జవాబు->n_కీలకం_Actions,
                               జవాబు->మొత్తం_Actions,
                               జవాబు->మొత్తం_కీలకం_Behaviors,
                               జవాబు->virtualMods,
                               జవాబు->మొత్తం_కీలకం_Explicit,
                               జవాబు->మొత్తం_Mod_Map_కీలకాలు,
                               జవాబు->మొత్తం_VMod_Map_కీలకాలు,
                               జవాబు->present,
                               &map);

    ఒకవేళ (!get_types(keymap, conn, జవాబు, &map) ||
        !get_sym_maps(keymap, conn, జవాబు, &map) ||
        !get_actions(keymap, conn, జవాబు, &map) ||
        !get_vmods(keymap, conn, జవాబు, &map) ||
        !get_explicits(keymap, conn, జవాబు, &map) ||
        !get_modmaps(keymap, conn, జవాబు, &map) ||
        !get_vmodmaps(keymap, conn, జవాబు, &map))
        వెళ్ళు fail;

    విడుదల(జవాబు);
    తిరిగి true;

fail:
    విడుదల(జవాబు);
    తిరిగి false;
}

స్థిర bool
get_indicators(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_జవాబు_t *జవాబు)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(జవాబు);

    keymap->అంకె_leds = msb_pos(జవాబు->which);
    FAIL_UNLESS(keymap->అంకె_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == అంకె_INDICATORS, "");
    కొరకు (గుర్తులేని i = 0; i < అంకె_INDICATORS; i++) {
        ఒకవేళ (జవాబు->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.దత్తాంశం;
            నిర్మాణం xkb_led *led = &keymap->leds[i];

            ఒకవేళ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_పునాది)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            ఒకవేళ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            ఒకవేళ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            ఒకవేళ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            ఒకవేళ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            ఒకవేళ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_పునాది)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            ఒకవేళ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            ఒకవేళ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            ఒకవేళ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            ఒకవేళ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.ముసుగు = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_ముసుగు(wire->ctrls);

            xcb_xkb_indicator_map_తదుపరి(&iter);
        }
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_indicator_map(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_జవాబు_t *జవాబు =
        xcb_xkb_get_indicator_map_జవాబు(conn, cookie, ఖాళీ);

    FAIL_IF_BAD_జవాబు(జవాబు, "XkbGetIndicatorMap");

    ఒకవేళ (!get_indicators(keymap, conn, జవాబు))
        వెళ్ళు fail;

    విడుదల(జవాబు);
    తిరిగి true;

fail:
    విడుదల(జవాబు);
    తిరిగి false;
}

స్థిర bool
get_sym_interprets(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_జవాబు_t *జవాబు)
{
    పూర్ణాంకం నిడివి = xcb_xkb_get_compat_map_si_rtrn_నిడివి(జవాబు);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(జవాబు);

    FAIL_UNLESS(జవాబు->మొదటి_SIRtrn == 0);
    FAIL_UNLESS(జవాబు->nSIRtrn == జవాబు->n_మొత్తం_SI);

    keymap->అంకె_sym_interprets = జవాబు->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->అంకె_sym_interprets);

    స్థిరరాశి xkb_keycode_t min_కీలకం_సంకేతం = keymap->min_కీలకం_సంకేతం;
    స్థిరరాశి xkb_keycode_t max_కీలకం_సంకేతం = keymap->max_కీలకం_సంకేతం;

    కొరకు (పూర్ణాంకం i = 0; i < నిడివి; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.దత్తాంశం;
        నిర్మాణం xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        ఎంపిక (wire->సరిపోలిక & XCB_XKB_SYM_INTERP_సరిపోలిక_OP_ముసుగు) {
        స్థితి XCB_XKB_SYM_INTERPRET_సరిపోలిక_NONE_యొక్క:
            sym_interpret->సరిపోలిక = సరిపోలిక_NONE;
            ఆపు;
        స్థితి XCB_XKB_SYM_INTERPRET_సరిపోలిక_ANY_యొక్క_OR_NONE:
            sym_interpret->సరిపోలిక = సరిపోలిక_ANY_OR_NONE;
            ఆపు;
        స్థితి XCB_XKB_SYM_INTERPRET_సరిపోలిక_ANY_యొక్క:
            sym_interpret->సరిపోలిక = సరిపోలిక_ANY;
            ఆపు;
        స్థితి XCB_XKB_SYM_INTERPRET_సరిపోలిక_ALL_యొక్క:
            sym_interpret->సరిపోలిక = సరిపోలిక_ALL;
            ఆపు;
        స్థితి XCB_XKB_SYM_INTERPRET_సరిపోలిక_EXACTLY:
            sym_interpret->సరిపోలిక = సరిపోలిక_EXACTLY;
            ఆపు;
        అప్రమేయ:
            log_err_func(keymap->ctx, XKB_LOG_సందేశం_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->సరిపోలిక & XCB_XKB_SYM_INTERP_సరిపోలిక_OP_ముసుగు);
            వెళ్ళు fail;
        }

        sym_interpret->level_one_only =
            (wire->సరిపోలిక & XCB_XKB_SYM_INTERP_సరిపోలిక_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        ఒకవేళ (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        లేకపోతే
            sym_interpret->virtual_mod = అంకె_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_కీలకం_సంకేతం, max_కీలకం_సంకేతం));
        sym_interpret->అంకె_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_తదుపరి(&iter);
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_compat_map(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_జవాబు_t *జవాబు =
        xcb_xkb_get_compat_map_జవాబు(conn, cookie, ఖాళీ);

    FAIL_IF_BAD_జవాబు(జవాబు, "XkbGetCompatMap");

    ఒకవేళ (!get_sym_interprets(keymap, conn, జవాబు))
        వెళ్ళు fail;

    విడుదల(జవాబు);
    తిరిగి true;

fail:
    విడుదల(జవాబు);
    తిరిగి false;
}

స్థిర bool
get_type_పేర్లు(నిర్మాణం xkb_keymap *keymap, నిర్మాణం x11_atom_interner *interner,
               xcb_xkb_get_పేర్లు_జవాబు_t *జవాబు,
               xcb_xkb_get_పేర్లు_విలువ_list_t *list)
{
    పూర్ణాంకం కీలకం_type_పేర్లు_నిడివి =
        xcb_xkb_get_పేర్లు_విలువ_list_type_పేర్లు_నిడివి(జవాబు, list);
    xcb_atom_t *కీలకం_type_పేర్లు_iter =
        xcb_xkb_get_పేర్లు_విలువ_list_type_పేర్లు(list);
    పూర్ణాంకం n_స్థాయిలు_చొప్పున_type_నిడివి =
        xcb_xkb_get_పేర్లు_విలువ_list_n_స్థాయిలు_చొప్పున_type_నిడివి(జవాబు, list);
    uint8_t *n_స్థాయిలు_చొప్పున_type_iter =
        xcb_xkb_get_పేర్లు_విలువ_list_n_స్థాయిలు_చొప్పున_type(list);
    xcb_atom_t *kt_level_పేర్లు_iter =
        xcb_xkb_get_పేర్లు_విలువ_list_kt_level_పేర్లు(list);

    FAIL_UNLESS(జవాబు->nTypes == keymap->అంకె_types);
    FAIL_UNLESS(కీలకం_type_పేర్లు_నిడివి == n_స్థాయిలు_చొప్పున_type_నిడివి);

    కొరకు (పూర్ణాంకం i = 0; i < కీలకం_type_పేర్లు_నిడివి; i++) {
        xcb_atom_t wire_type_name = *కీలకం_type_పేర్లు_iter;
        uint8_t wire_అంకె_స్థాయిలు = *n_స్థాయిలు_చొప్పున_type_iter;
        నిర్మాణం xkb_కీలకం_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->అంకె_స్థాయిలు >= wire_అంకె_స్థాయిలు);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_పేర్లు, type->అంకె_స్థాయిలు);
        type->అంకె_level_పేర్లు = type->అంకె_స్థాయిలు;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        కొరకు (కొలత_t j = 0; j < wire_అంకె_స్థాయిలు; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_పేర్లు_iter[j],
                                         &type->level_పేర్లు[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        కొరకు (కొలత_t j = wire_అంకె_స్థాయిలు; j < type->అంకె_స్థాయిలు; j++)
            type->level_పేర్లు[j] = XKB_ATOM_NONE;

        kt_level_పేర్లు_iter += wire_అంకె_స్థాయిలు;
        కీలకం_type_పేర్లు_iter++;
        n_స్థాయిలు_చొప్పున_type_iter++;
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_indicator_పేర్లు(నిర్మాణం xkb_keymap *keymap,
                    నిర్మాణం x11_atom_interner *interner,
                    xcb_xkb_get_పేర్లు_జవాబు_t *జవాబు,
                    xcb_xkb_get_పేర్లు_విలువ_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_పేర్లు_విలువ_list_indicator_పేర్లు(list);

    FAIL_UNLESS(msb_pos(జవాబు->indicators) <= keymap->అంకె_leds);

    కొరకు (గుర్తులేని i = 0; i < అంకె_INDICATORS; i++) {
        ఒకవేళ (జవాబు->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            నిర్మాణం xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_vmod_పేర్లు(నిర్మాణం xkb_keymap *keymap, నిర్మాణం x11_atom_interner *interner,
               xcb_xkb_get_పేర్లు_జవాబు_t *జవాబు,
               xcb_xkb_get_పేర్లు_విలువ_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_పేర్లు_విలువ_list_virtual_mod_పేర్లు(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.అంకె_mods =
        అంకె_REAL_MODS + MIN(msb_pos(జవాబు->virtualMods), అంకె_VMODS);
    FAIL_UNLESS(keymap->mods.అంకె_mods <= XKB_MAX_MODS);

    static_assert(అంకె_REAL_MODS + అంకె_VMODS <= XKB_MAX_MODS, "");
    కొరకు (గుర్తులేని i = 0; i < అంకె_VMODS; i++) {
        ఒకవేళ (జవాబు->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            నిర్మాణం xkb_mod *mod = &keymap->mods.mods[అంకె_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_సమూహం_పేర్లు(నిర్మాణం xkb_keymap *keymap, నిర్మాణం x11_atom_interner *interner,
                xcb_xkb_get_పేర్లు_జవాబు_t *జవాబు,
                xcb_xkb_get_పేర్లు_విలువ_list_t *list)
{
    పూర్ణాంకం నిడివి = xcb_xkb_get_పేర్లు_విలువ_list_groups_నిడివి(జవాబు, list);
    xcb_atom_t *iter = xcb_xkb_get_పేర్లు_విలువ_list_groups(list);

    keymap->అంకె_సమూహం_పేర్లు = msb_pos(జవాబు->సమూహం_పేర్లు_2);
    ALLOC_OR_FAIL(keymap->సమూహం_పేర్లు, keymap->అంకె_సమూహం_పేర్లు);

    కొరకు (పూర్ణాంకం i = 0; i < నిడివి; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->సమూహం_పేర్లు[i]);
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_కీలకం_పేర్లు(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_పేర్లు_జవాబు_t *జవాబు,
              xcb_xkb_get_పేర్లు_విలువ_list_t *list)
{
    పూర్ణాంకం నిడివి = xcb_xkb_get_పేర్లు_విలువ_list_కీలకం_పేర్లు_నిడివి(జవాబు, list);
    xcb_xkb_కీలకం_name_iterator_t iter =
        xcb_xkb_get_పేర్లు_విలువ_list_కీలకం_పేర్లు_iterator(జవాబు, list);

    FAIL_UNLESS(జవాబు->min_కీలకం_సంకేతం_2 == keymap->min_కీలకం_సంకేతం);
    FAIL_UNLESS(జవాబు->max_కీలకం_సంకేతం_2 == keymap->max_కీలకం_సంకేతం);
    FAIL_UNLESS(జవాబు->మొదటి_కీలకం == keymap->min_కీలకం_సంకేతం);
    FAIL_UNLESS(జవాబు->మొదటి_కీలకం + జవాబు->n_కీలకాలు - 1U == keymap->max_కీలకం_సంకేతం);

    కొరకు (పూర్ణాంకం i = 0; i < నిడివి; i++) {
        xcb_xkb_కీలకం_name_t *wire = iter.దత్తాంశం;
        xkb_atom_t *కీలకం_name = &keymap->కీలకాలు[జవాబు->మొదటి_కీలకం + i].name;

        ఒకవేళ (wire->name[0] == '\0') {
            *కీలకం_name = XKB_ATOM_NONE;
        }
        లేకపోతే {
            *కీలకం_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_కీలకం_NAME_నిడివి));
            ఒకవేళ (!*కీలకం_name)
                తిరిగి false;
        }

        xcb_xkb_కీలకం_name_తదుపరి(&iter);
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_aliases(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_పేర్లు_జవాబు_t *జవాబు,
            xcb_xkb_get_పేర్లు_విలువ_list_t *list)
{
    పూర్ణాంకం నిడివి = xcb_xkb_get_పేర్లు_విలువ_list_కీలకం_aliases_నిడివి(జవాబు, list);
    xcb_xkb_కీలకం_alias_iterator_t iter =
        xcb_xkb_get_పేర్లు_విలువ_list_కీలకం_aliases_iterator(జవాబు, list);

    keymap->అంకె_కీలకం_aliases = జవాబు->n_కీలకం_Aliases;
    ALLOC_OR_FAIL(keymap->కీలకం_aliases, keymap->అంకె_కీలకం_aliases);

    కొరకు (పూర్ణాంకం i = 0; i < నిడివి; i++) {
        xcb_xkb_కీలకం_alias_t *wire = iter.దత్తాంశం;
        నిర్మాణం xkb_కీలకం_alias *alias = &keymap->కీలకం_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_కీలకం_NAME_నిడివి));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_కీలకం_NAME_నిడివి));
        ఒకవేళ (!alias->real || !alias->alias)
            వెళ్ళు fail;

        xcb_xkb_కీలకం_alias_తదుపరి(&iter);
    }

    తిరిగి true;

fail:
    తిరిగి false;
}

స్థిర bool
get_పేర్లు(నిర్మాణం xkb_keymap *keymap, నిర్మాణం x11_atom_interner *interner,
          xcb_xkb_get_పేర్లు_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_పేర్లు_జవాబు_t *జవాబు =
        xcb_xkb_get_పేర్లు_జవాబు(conn, cookie, ఖాళీ);
    xcb_xkb_get_పేర్లు_విలువ_list_t list;

    FAIL_IF_BAD_జవాబు(జవాబు, "XkbGetNames");

    FAIL_UNLESS((జవాబు->which & get_పేర్లు_required) == get_పేర్లు_required);

    xcb_xkb_get_పేర్లు_విలువ_list_unpack(xcb_xkb_get_పేర్లు_విలువ_list(జవాబు),
                                        జవాబు->nTypes,
                                        జవాబు->indicators,
                                        జవాబు->virtualMods,
                                        జవాబు->సమూహం_పేర్లు_2,
                                        జవాబు->n_కీలకాలు,
                                        జవాబు->n_కీలకం_Aliases,
                                        జవాబు->nRadioGroups,
                                        జవాబు->which,
                                        &list);

    x11_atom_interner_get_escaped_atom_name(interner, list.keycodesName,
                                            &keymap->keycodes_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.symbolsName,
                                            &keymap->symbols_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.typesName,
                                            &keymap->types_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.compatName,
                                            &keymap->compat_section_name);
    ఒకవేళ (!get_type_పేర్లు(keymap, interner, జవాబు, &list) ||
        !get_indicator_పేర్లు(keymap, interner, జవాబు, &list) ||
        !get_vmod_పేర్లు(keymap, interner, జవాబు, &list) ||
        !get_సమూహం_పేర్లు(keymap, interner, జవాబు, &list) ||
        !get_కీలకం_పేర్లు(keymap, conn, జవాబు, &list) ||
        !get_aliases(keymap, conn, జవాబు, &list))
        వెళ్ళు fail;

    విడుదల(జవాబు);
    తిరిగి true;

fail:
    విడుదల(జవాబు);
    తిరిగి false;
}

స్థిర bool
get_controls(నిర్మాణం xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_జవాబు_t *జవాబు =
        xcb_xkb_get_controls_జవాబు(conn, cookie, ఖాళీ);

    FAIL_IF_BAD_జవాబు(జవాబు, "XkbGetControls");
    FAIL_UNLESS(జవాబు->అంకె_Groups > 0 && జవాబు->అంకె_Groups <= 4);

    keymap->అంకె_groups = జవాబు->అంకె_Groups;

    FAIL_UNLESS(keymap->max_కీలకం_సంకేతం < XCB_XKB_CONST_చొప్పున_కీలకం_బిట్టు_ARRAY_కొలత * 8);

    కొరకు (xkb_keycode_t i = keymap->min_కీలకం_సంకేతం; i <= keymap->max_కీలకం_సంకేతం; i++)
        keymap->కీలకాలు[i].repeats = (జవాబు->చొప్పున_కీలకం_Repeat[i / 8] & (1u << (i % 8)));

    విడుదల(జవాబు);
    తిరిగి true;

fail:
    విడుదల(జవాబు);
    తిరిగి false;
}

నిర్మాణం xkb_keymap *
xkb_x11_keymap_new_from_device(నిర్మాణం xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               గణన xkb_keymap_compile_flags flags)
{
    ఒకవేళ (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_సందేశం_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        తిరిగి ఖాళీ;
    }

    స్థిరరాశి గణన xkb_keymap_format format = XKB_KEYMAP_FORMAT_పాఠ్యం_V1;
    నిర్మాణం xkb_keymap * స్థిరరాశి keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    ఒకవేళ (!keymap)
        తిరిగి ఖాళీ;

    keymap->redirect_కీలకం_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    నిర్మాణం x11_atom_interner interner;
    x11_atom_interner_init(&interner, ctx, conn);

    /*
     * Send all requests together so only one roundtrip is needed
     * to get the replies.
     */
    xcb_xkb_get_map_cookie_t map_cookie =
        xcb_xkb_get_map(conn, device_id, get_map_required_components,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    xcb_xkb_get_indicator_map_cookie_t indicator_map_cookie =
        xcb_xkb_get_indicator_map(conn, device_id, ALL_INDICATORS_ముసుగు);
    xcb_xkb_get_compat_map_cookie_t compat_map_cookie =
        xcb_xkb_get_compat_map(conn, device_id, 0, true, 0, 0);
    xcb_xkb_get_పేర్లు_cookie_t పేర్లు_cookie =
        xcb_xkb_get_పేర్లు(conn, device_id, get_పేర్లు_wanted);
    xcb_xkb_get_controls_cookie_t controls_cookie =
        xcb_xkb_get_controls(conn, device_id);

    ఒకవేళ (!get_map(keymap, conn, map_cookie))
        వెళ్ళు err_map;
    ఒకవేళ (!get_indicator_map(keymap, conn, indicator_map_cookie))
        వెళ్ళు err_indicator_map;
    ఒకవేళ (!get_compat_map(keymap, conn, compat_map_cookie))
        వెళ్ళు err_compat_map;
    ఒకవేళ (!get_పేర్లు(keymap, &interner, పేర్లు_cookie))
        వెళ్ళు err_పేర్లు;
    ఒకవేళ (!get_controls(keymap, conn, controls_cookie))
        వెళ్ళు err_controls;
    x11_atom_interner_విడత_trip(&interner);
    ఒకవేళ (interner.had_error)
        వెళ్ళు err_interner;

    తిరిగి keymap;

err_map:
    xcb_discard_జవాబు(conn, indicator_map_cookie.sequence);
err_indicator_map:
    xcb_discard_జవాబు(conn, compat_map_cookie.sequence);
err_compat_map:
    xcb_discard_జవాబు(conn, పేర్లు_cookie.sequence);
err_పేర్లు:
    xcb_discard_జవాబు(conn, controls_cookie.sequence);
err_controls:
    x11_atom_interner_విడత_trip(&interner);
err_interner:
    xkb_keymap_unref(keymap);
    తిరిగి ఖాళీ;
}
