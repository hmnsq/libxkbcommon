/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <xcb/xkb.h>

#ضمن "darray.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcommon/xkbcommon-keysyms.h"
#ضمن "atom.h"
#ضمن "keymap.h"
#ضمن "x11-priv.h"

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
#وصف_ڪريو انگ_REAL_MODS 8u
/* XkbNumVirtualMods. */
#وصف_ڪريو انگ_VMODS 16u
/* XkbNoModifier. */
#وصف_ڪريو NO_MODIFIER 0xff
/* XkbNumIndicators. */
#وصف_ڪريو انگ_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#وصف_ڪريو ALL_INDICATORS_نقاب 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#وصف_ڪريو FAIL_UNLESS(اظهار) افعل {                              \
    إذا (!(اظهار)) {                                          \
        log_err(keymap->ctx, XKB_LOG_پيغام_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(اظهار));                 \
        اذهب fail;                                          \
    }                                                       \
} طالما (0)

#وصف_ڪريو FAIL_IF_BAD_جواب(جواب, request_name) افعل {         \
    إذا (!(جواب)) {                                           \
        log_err(keymap->ctx, XKB_LOG_پيغام_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        اذهب fail;                                          \
    }                                                       \
} طالما (0)

#وصف_ڪريو ALLOC_OR_FAIL(arr, nmemb) افعل {                      \
    إذا ((nmemb) > 0) {                                      \
        (arr) = صفر_يادگيري_ڏيو((nmemb), حجم(*(arr)));            \
        إذا (!(arr))                                         \
            اذهب fail;                                      \
    }                                                       \
} طالما (0)

ساكن ثابت xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_ڪنجي_TYPES |
     XCB_XKB_MAP_PART_ڪنجي_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_ڪنجي_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

ساكن ثابت xcb_xkb_name_detail_t get_نالا_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_ڪنجي_TYPE_نالا |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_نالا |
     XCB_XKB_NAME_DETAIL_INDICATOR_نالا |
     XCB_XKB_NAME_DETAIL_ڪنجي_نالا |
     XCB_XKB_NAME_DETAIL_ڪنجي_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_نالا |
     XCB_XKB_NAME_DETAIL_گروهه_نالا);
ساكن ثابت xcb_xkb_name_detail_t get_نالا_required =
    (XCB_XKB_NAME_DETAIL_ڪنجي_TYPE_نالا |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_نالا |
     XCB_XKB_NAME_DETAIL_ڪنجي_نالا |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_نالا);


ساكن xkb_mod_نقاب_t
translate_mods(uint8_t rmods, uint16_t vmods_هيٺيون, uint16_t vmods_مٿاهون)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    أرجع
        ((xkb_mod_نقاب_t) rmods) |
        ((xkb_mod_نقاب_t) vmods_هيٺيون << 8) |
        ((xkb_mod_نقاب_t) vmods_مٿاهون << 16);
}

تعداد xkb_action_controls
translate_controls_نقاب(uint32_t wire)
{
    تعداد xkb_action_controls ret = 0;
    إذا (wire & XCB_XKB_BOOL_CTRL_REPEAT_ڪنجيون)
        ret |= CONTROL_REPEAT;
    إذا (wire & XCB_XKB_BOOL_CTRL_SLOW_ڪنجيون)
        ret |= CONTROL_SLOW;
    إذا (wire & XCB_XKB_BOOL_CTRL_BOUNCE_ڪنجيون)
        ret |= CONTROL_DEBOUNCE;
    إذا (wire & XCB_XKB_BOOL_CTRL_STICKY_ڪنجيون)
        ret |= CONTROL_STICKY_ڪنجيون;
    إذا (wire & XCB_XKB_BOOL_CTRL_MOUSE_ڪنجيون)
        ret |= CONTROL_MOUSE_ڪنجيون;
    إذا (wire & XCB_XKB_BOOL_CTRL_MOUSE_ڪنجيون_ACCEL)
        ret |= CONTROL_MOUSE_ڪنجيون_ACCEL;
    إذا (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_ڪنجيون)
        ret |= CONTROL_AX;
    إذا (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_نقاب)
        ret |= CONTROL_AX_TIMEOUT;
    إذا (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_نقاب)
        ret |= CONTROL_AX_FEEDBACK;
    إذا (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_نقاب)
        ret |= CONTROL_BELL;
    إذا (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_نقاب)
        ret |= CONTROL_OVERLAY1;
    إذا (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_نقاب)
        ret |= CONTROL_OVERLAY2;
    إذا (wire & XCB_XKB_BOOL_CTRL_IGNORE_گروهه_تالو_نقاب)
        ret |= CONTROL_IGNORE_گروهه_تالو;
    /* Some controls are not supported and don't appear here. */
    أرجع ret;
}

ساكن bool
translate_action(اتحاد xkb_action *action, ثابت xcb_xkb_action_t *wire,
                 xkb_keycode_t min_ڪنجي_ڪوڊ, xkb_keycode_t max_ڪنجي_ڪوڊ)
{
    بدل (wire->type) {
    حالة XCB_XKB_SA_TYPE_سيٽ_MODS:
        action->type = ACTION_TYPE_MOD_سيٽ;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_هيٺيون_2,
                                                wire->setmods.vmods_مٿاهون_2);
        action->mods.mods.نقاب = translate_mods(wire->setmods.نقاب, 0, 0);

        إذا (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_تالو_CLEAR;
        إذا (wire->setmods.flags & XCB_XKB_SA_LATCH_منزل_تالو)
            action->mods.flags |= ACTION_LATCH_منزل_تالو;
        إذا (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_هيٺيون_2,
                                                wire->latchmods.vmods_مٿاهون_2);
        action->mods.mods.نقاب = translate_mods(wire->latchmods.نقاب, 0, 0);

        إذا (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_تالو_CLEAR;
        إذا (wire->latchmods.flags & XCB_XKB_SA_LATCH_منزل_تالو)
            action->mods.flags |= ACTION_LATCH_منزل_تالو;
        إذا (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_تالو_MODS:
        action->type = ACTION_TYPE_MOD_تالو;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_هيٺيون_2,
                                                wire->lockmods.vmods_مٿاهون_2);
        action->mods.mods.نقاب = translate_mods(wire->lockmods.نقاب, 0, 0);

        إذا (wire->lockmods.flags & XCB_XKB_SA_ISO_تالو_FLAG_NO_تالو)
            action->mods.flags |= ACTION_تالو_NO_تالو;
        إذا (wire->lockmods.flags & XCB_XKB_SA_ISO_تالو_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_تالو_NO_UNLOCK;
        إذا (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_سيٽ_گروهه:
        action->type = ACTION_TYPE_گروهه_سيٽ;

        action->گروهه.گروهه = (int32_t)wire->setgroup.گروهه;

        إذا (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->گروهه.flags |= ACTION_تالو_CLEAR;
        إذا (wire->setmods.flags & XCB_XKB_SA_LATCH_منزل_تالو)
            action->گروهه.flags |= ACTION_LATCH_منزل_تالو;
        إذا (wire->setmods.flags & XCB_XKB_SA_ISO_تالو_FLAG_گروهه_ABSOLUTE)
            action->گروهه.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_LATCH_گروهه:
        action->type = ACTION_TYPE_گروهه_LATCH;

        action->گروهه.گروهه = (int32_t)wire->latchgroup.گروهه;

        إذا (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->گروهه.flags |= ACTION_تالو_CLEAR;
        إذا (wire->latchmods.flags & XCB_XKB_SA_LATCH_منزل_تالو)
            action->گروهه.flags |= ACTION_LATCH_منزل_تالو;
        إذا (wire->latchmods.flags & XCB_XKB_SA_ISO_تالو_FLAG_گروهه_ABSOLUTE)
            action->گروهه.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_تالو_گروهه:
        action->type = ACTION_TYPE_گروهه_تالو;

        action->گروهه.گروهه = (int32_t)wire->lockgroup.گروهه;

        إذا (wire->lockgroup.flags & XCB_XKB_SA_ISO_تالو_FLAG_گروهه_ABSOLUTE)
            action->گروهه.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_هيٺيون | ((uint16_t) wire->moveptr.x_مٿاهون << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_هيٺيون | ((uint16_t) wire->moveptr.y_مٿاهون << 8));

        إذا (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        إذا (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        إذا (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        اكسر;
    حالة XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.ڳڻپ = wire->ptrbtn.ڳڻپ;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        اكسر;
    حالة XCB_XKB_SA_TYPE_تالو_PTR_BTN:
        action->type = ACTION_TYPE_PTR_تالو;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.ڳڻپ = 0;

        إذا (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_تالو_FLAG_NO_تالو)
            action->btn.flags |= ACTION_تالو_NO_تالو;
        إذا (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_تالو_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_تالو_NO_UNLOCK;

        اكسر;
    حالة XCB_XKB_SA_TYPE_سيٽ_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.قدر = wire->setptrdflt.قدر;

        إذا (wire->setptrdflt.flags & XCB_XKB_SA_سيٽ_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        اكسر;
    حالة XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        إذا (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        إذا (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_سيٽ_CONTROLS:
        action->type = ACTION_TYPE_CTRL_سيٽ;
        {
            ثابت uint16_t نقاب = (wire->setcontrols.bool_Ctrls_هيٺيون |
                                   (wire->setcontrols.bool_Ctrls_مٿاهون << 8));
            action->ctrls.ctrls = translate_controls_نقاب(نقاب);
        }
        اكسر;
    حالة XCB_XKB_SA_TYPE_تالو_CONTROLS:
        action->type = ACTION_TYPE_CTRL_تالو;
        {
            ثابت uint16_t نقاب = (wire->lockcontrols.bool_Ctrls_هيٺيون |
                                   (wire->lockcontrols.bool_Ctrls_مٿاهون << 8));
            action->ctrls.ctrls = translate_controls_نقاب(نقاب);
        }
        اكسر;

    حالة XCB_XKB_SA_TYPE_REDIRECT_ڪنجي:
        action->type = ACTION_TYPE_REDIRECT_ڪنجي;
        إذا (wire->redirect.newkey < min_ڪنجي_ڪوڊ ||
            wire->redirect.newkey > max_ڪنجي_ڪوڊ)
                أرجع false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.نقاب,
            wire->redirect.vmods_نقاب_هيٺيون,
            wire->redirect.vmods_نقاب_مٿاهون
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_هيٺيون_2,
                                               wire->redirect.vmods_مٿاهون_2);
        اكسر;

    حالة XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        اكسر;

    /* We don't support these. */
    حالة XCB_XKB_SA_TYPE_ISO_تالو:
    حالة XCB_XKB_SA_TYPE_ACTION_پيغام:
    حالة XCB_XKB_SA_TYPE_DEVICE_BTN:
    حالة XCB_XKB_SA_TYPE_تالو_DEVICE_BTN:
    حالة XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندروني == 20 &&
                      ACTION_TYPE_اندروني + 1 == ACTION_TYPE_انگ_ENTRIES,
                      "Missing action type");

        إذا (wire->type < ACTION_TYPE_ذاتي) {
            action->type = ACTION_TYPE_NONE;
            اكسر;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(حجم(action->priv.معلومات) == 7 &&
                      حجم(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        يادگيري_نقل(action->priv.معلومات, wire->noaction.pad0, 7);
        اكسر;
    }
    أرجع true;
}

ساكن bool
get_types(بنية xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح types_ڊيگهائي = xcb_xkb_get_map_map_types_rtrn_ڊيگهائي(جواب, map);
    xcb_xkb_ڪنجي_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(جواب, map);

    FAIL_UNLESS(جواب->پهريون_Type == 0);

    keymap->انگ_types = جواب->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->انگ_types);
    أكد(types_ڊيگهائي == (صحيح) keymap->انگ_types);

    لكل (صحيح i = 0; i < types_ڊيگهائي; i++) {
        xcb_xkb_ڪنجي_type_t *wire_type = types_iter.معلومات;
        بنية xkb_ڪنجي_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->انگ_سطحون_2 > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.نقاب = translate_mods(wire_type->mods_نقاب, 0, 0);
        type->انگ_سطحون = wire_type->انگ_سطحون_2;

        {
            صحيح entries_ڊيگهائي = xcb_xkb_ڪنجي_type_map_ڊيگهائي(wire_type);
            xcb_xkb_kt_map_داخلا_iterator_t entries_iter =
                xcb_xkb_ڪنجي_type_map_iterator(wire_type);

            type->انگ_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->انگ_entries);
            أكد(entries_ڊيگهائي == (صحيح) type->انگ_entries);

            لكل (صحيح j = 0; j < entries_ڊيگهائي; j++) {
                xcb_xkb_kt_map_داخلا_t *wire_داخلا = entries_iter.معلومات;
                بنية xkb_ڪنجي_type_داخلا *داخلا = &type->entries[j];

                FAIL_UNLESS(wire_داخلا->level < type->انگ_سطحون);

                داخلا->level = wire_داخلا->level;
                داخلا->mods.mods = translate_mods(wire_داخلا->mods_mods,
                                                  wire_داخلا->mods_vmods, 0);
                داخلا->mods.نقاب = translate_mods(wire_داخلا->mods_نقاب, 0, 0);

                xcb_xkb_kt_map_داخلا_ايندڙ(&entries_iter);
            }
        }

        {
            صحيح preserves_ڊيگهائي = xcb_xkb_ڪنجي_type_preserve_ڊيگهائي(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_ڪنجي_type_preserve_iterator(wire_type);

            FAIL_UNLESS((بنانشان) preserves_ڊيگهائي <= type->انگ_entries);

            لكل (صحيح j = 0; j < preserves_ڊيگهائي; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.معلومات;
                بنية xkb_ڪنجي_type_داخلا *داخلا = &type->entries[j];

                داخلا->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                داخلا->preserve.نقاب = translate_mods(wire_preserve->نقاب, 0, 0);

                xcb_xkb_mod_def_ايندڙ(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_ڪنجي_type_ايندڙ(&types_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_sym_maps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح sym_maps_ڊيگهائي = xcb_xkb_get_map_map_syms_rtrn_ڊيگهائي(جواب, map);
    xcb_xkb_ڪنجي_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(جواب, map);

    FAIL_UNLESS(جواب->min_ڪنجي_ڪوڊ_2 <= جواب->max_ڪنجي_ڪوڊ_2);
    FAIL_UNLESS(جواب->پهريون_ڪنجي_Sym >= جواب->min_ڪنجي_ڪوڊ_2);
    FAIL_UNLESS(جواب->پهريون_ڪنجي_Sym + جواب->n_ڪنجي_Syms <= جواب->max_ڪنجي_ڪوڊ_2 + 1);

    keymap->min_ڪنجي_ڪوڊ = جواب->min_ڪنجي_ڪوڊ_2;
    keymap->max_ڪنجي_ڪوڊ = جواب->max_ڪنجي_ڪوڊ_2;
    keymap->انگ_ڪنجيون = keymap->انگ_ڪنجيون_هيٺيون = جواب->max_ڪنجي_ڪوڊ_2 + 1;

    ALLOC_OR_FAIL(keymap->ڪنجيون, keymap->max_ڪنجي_ڪوڊ + 1);

    لكل (xkb_keycode_t kc = keymap->min_ڪنجي_ڪوڊ; kc <= keymap->max_ڪنجي_ڪوڊ; kc++)
        keymap->ڪنجيون[kc].keycode = kc;

    لكل (صحيح i = 0; i < sym_maps_ڊيگهائي; i++) {
        xcb_xkb_ڪنجي_sym_map_t *wire_sym_map = sym_maps_iter.معلومات;
        بنية xkb_ڪنجي *ڪنجي = &keymap->ڪنجيون[جواب->پهريون_ڪنجي_Sym + i];

        ڪنجي->انگ_groups = wire_sym_map->گروهه_Info & 0x0f;
        FAIL_UNLESS(ڪنجي->انگ_groups <= ARRAY_سائيز(wire_sym_map->kt_اشاريو));
        ALLOC_OR_FAIL(ڪنجي->groups, ڪنجي->انگ_groups);

        لكل (xkb_layout_اشاريو_t j = 0; j < ڪنجي->انگ_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_اشاريو[j] < keymap->انگ_types);
            ڪنجي->groups[j].type = &keymap->types[wire_sym_map->kt_اشاريو[j]];

            ALLOC_OR_FAIL(ڪنجي->groups[j].سطحون, ڪنجي->groups[j].type->انگ_سطحون);
        }

        ڪنجي->ٻاهريون_جو_range_گروهه_عدد = (wire_sym_map->گروهه_Info & 0x30) >> 4;

        FAIL_UNLESS(ڪنجي->ٻاهريون_جو_range_گروهه_عدد <= ڪنجي->انگ_groups);

        إذا (wire_sym_map->گروهه_Info & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            ڪنجي->ٻاهريون_جو_range_گروهه_policy = XKB_LAYOUT_ٻاهريون_جو_RANGE_CLAMP;
        وإلا إذا (wire_sym_map->گروهه_Info & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            ڪنجي->ٻاهريون_جو_range_گروهه_policy = XKB_LAYOUT_ٻاهريون_جو_RANGE_REDIRECT;
        وإلا
            ڪنجي->ٻاهريون_جو_range_گروهه_policy = XKB_LAYOUT_ٻاهريون_جو_RANGE_WRAP;

        {
            صحيح syms_ڊيگهائي = xcb_xkb_ڪنجي_sym_map_syms_ڊيگهائي(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_ڪنجي_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((بنانشان) syms_ڊيگهائي == wire_sym_map->ويڪر * ڪنجي->انگ_groups);

            إذا (syms_ڊيگهائي > 0)
                ڪنجي->explicit |= EXPLICIT_SYMBOLS;

            لكل (xkb_layout_اشاريو_t گروهه = 0; گروهه < ڪنجي->انگ_groups; گروهه++) {
                لكل (xkb_level_اشاريو_t level = 0; level < wire_sym_map->ويڪر; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    أكد(ڪنجي->groups[گروهه].type != عدم);
                    إذا (level < ڪنجي->groups[گروهه].type->انگ_سطحون) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        ڪنجي->groups[گروهه].سطحون[level].انگ_syms = 1;
                        ڪنجي->groups[گروهه].سطحون[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        ڪنجي->groups[گروهه].سطحون[level].مٿانهون =
                            xkb_keysym_منزل_مٿانهون(wire_keysym);
                        إذا (wire_keysym != XKB_ڪنجي_No_نشان)
                            ڪنجي->groups[گروهه].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_ڪنجي_sym_map_ايندڙ(&sym_maps_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_actions(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح acts_ڳڻپ_ڊيگهائي =
        xcb_xkb_get_map_map_acts_rtrn_ڳڻپ_ڊيگهائي(جواب, map);
    uint8_t *acts_ڳڻپ_iter = xcb_xkb_get_map_map_acts_rtrn_ڳڻپ(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(جواب, map);
    xcb_xkb_ڪنجي_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(جواب, map);

    FAIL_UNLESS(جواب->پهريون_ڪنجي_Action == keymap->min_ڪنجي_ڪوڊ);
    FAIL_UNLESS(جواب->پهريون_ڪنجي_Action + جواب->n_ڪنجي_Actions ==
                keymap->max_ڪنجي_ڪوڊ + 1);

    لكل (صحيح i = 0; i < acts_ڳڻپ_ڊيگهائي; i++) {
        xcb_xkb_ڪنجي_sym_map_t *wire_sym_map = sym_maps_iter.معلومات;
        صحيح syms_ڊيگهائي = xcb_xkb_ڪنجي_sym_map_syms_ڊيگهائي(wire_sym_map);
        uint8_t wire_ڳڻپ = *acts_ڳڻپ_iter;
        بنية xkb_ڪنجي *ڪنجي = &keymap->ڪنجيون[جواب->پهريون_ڪنجي_Action + i];

        FAIL_UNLESS((بنانشان) syms_ڊيگهائي == wire_sym_map->ويڪر * ڪنجي->انگ_groups);
        FAIL_UNLESS(wire_ڳڻپ == 0 || wire_ڳڻپ == syms_ڊيگهائي);

        ثابت xkb_keycode_t min_ڪنجي_ڪوڊ = keymap->min_ڪنجي_ڪوڊ;
        ثابت xkb_keycode_t max_ڪنجي_ڪوڊ = keymap->max_ڪنجي_ڪوڊ;

        إذا (wire_ڳڻپ != 0) {
            لكل (xkb_layout_اشاريو_t گروهه = 0; گروهه < ڪنجي->انگ_groups; گروهه++) {
                لكل (xkb_level_اشاريو_t level = 0; level < wire_sym_map->ويڪر; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.معلومات;

                    إذا (level < ڪنجي->groups[گروهه].type->انگ_سطحون) {
                        ڪنجي->groups[گروهه].سطحون[level].انگ_actions = 1;
                        اتحاد xkb_action *action =
                            &ڪنجي->groups[گروهه].سطحون[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_ڪنجي_ڪوڊ, max_ڪنجي_ڪوڊ));

                        إذا (action->type != ACTION_TYPE_NONE) {
                            ڪنجي->groups[گروهه].implicit_actions =
                                !(ڪنجي->explicit & EXPLICIT_INTERP);
                        } وإلا إذا (ڪنجي->groups[گروهه].سطحون[level].s.sym ==
                                   XKB_ڪنجي_No_نشان) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            ڪنجي->groups[گروهه].سطحون[level].انگ_syms = 0;
                            ڪنجي->groups[گروهه].سطحون[level].انگ_actions = 0;
                        }
                    }

                    xcb_xkb_action_ايندڙ(&acts_iter);
                }

                إذا (ڪنجي->groups[گروهه].implicit_actions)
                    ڪنجي->implicit_actions = true;
            }
        }

        acts_ڳڻپ_iter++;
        xcb_xkb_ڪنجي_sym_map_ايندڙ(&sym_maps_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_vmods(بنية xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.انگ_mods =
        انگ_REAL_MODS + MIN(msb_pos(جواب->virtualMods), انگ_VMODS);
    FAIL_UNLESS(keymap->mods.انگ_mods <= XKB_MAX_MODS);

    static_assert(انگ_REAL_MODS + انگ_VMODS <= XKB_MAX_MODS, "");
    لكل (بنانشان i = 0; i < انگ_VMODS; i++) {
        إذا (جواب->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            بنية xkb_mod *mod = &keymap->mods.mods[انگ_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_explicits(بنية xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح ڊيگهائي = xcb_xkb_get_map_map_explicit_rtrn_ڊيگهائي(جواب, map);
    xcb_xkb_سيٽ_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(جواب, map);

    لكل (صحيح i = 0; i < ڊيگهائي; i++) {
        xcb_xkb_سيٽ_explicit_t *wire = iter.معلومات;
        بنية xkb_ڪنجي *ڪنجي;

        FAIL_UNLESS(wire->keycode >= keymap->min_ڪنجي_ڪوڊ &&
                    wire->keycode <= keymap->max_ڪنجي_ڪوڊ);

        ڪنجي = &keymap->ڪنجيون[wire->keycode];

        إذا ((wire->explicit & XCB_XKB_EXPLICIT_ڪنجي_TYPE_1) &&
            ڪنجي->انگ_groups > 0) {
            ڪنجي->groups[0].explicit_type = true;
            ڪنجي->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_ڪنجي_TYPE_2) &&
            ڪنجي->انگ_groups > 1) {
            ڪنجي->groups[1].explicit_type = true;
            ڪنجي->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_ڪنجي_TYPE_3) &&
            ڪنجي->انگ_groups > 2) {
            ڪنجي->groups[2].explicit_type = true;
            ڪنجي->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_ڪنجي_TYPE_4) &&
            ڪنجي->انگ_groups > 3) {
            ڪنجي->groups[3].explicit_type = true;
            ڪنجي->explicit |= EXPLICIT_TYPES;
        }
        إذا (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            ڪنجي->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            لكل (xkb_layout_اشاريو_t l = 0; l < ڪنجي->انگ_groups; l++) {
                ڪنجي->groups[l].explicit_actions = true;
            }
        }
        إذا (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            ڪنجي->explicit |= EXPLICIT_REPEAT;
        إذا (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            ڪنجي->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_سيٽ_explicit_ايندڙ(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_modmaps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح ڊيگهائي = xcb_xkb_get_map_map_modmap_rtrn_ڊيگهائي(جواب, map);
    xcb_xkb_ڪنجي_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(جواب, map);

    darray(Mod_Map_داخلا) modmaps = darray_new();

    لكل (صحيح i = 0; i < ڊيگهائي; i++) {
        xcb_xkb_ڪنجي_mod_map_t *wire = iter.معلومات;
        بنية xkb_ڪنجي *ڪنجي;

        FAIL_UNLESS(wire->keycode >= keymap->min_ڪنجي_ڪوڊ &&
                    wire->keycode <= keymap->max_ڪنجي_ڪوڊ);

        ڪنجي = &keymap->ڪنجيون[wire->keycode];
        ڪنجي->modmap = wire->mods;

        darray_append(modmaps, (Mod_Map_داخلا) {
            .ڪنجي_ڪوڊ = ڪنجي->keycode,
            .mods = ڪنجي->modmap,
            .have_نشان = false,
            .u.ڪنجي_Name = ڪنجي->name
        });

        xcb_xkb_ڪنجي_mod_map_ايندڙ(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->انگ_modmaps);
    أرجع true;

fail:
    darray_free(modmaps);
    أرجع false;
}

ساكن bool
get_vmodmaps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح ڊيگهائي = xcb_xkb_get_map_map_vmodmap_rtrn_ڊيگهائي(جواب, map);
    xcb_xkb_ڪنجي_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(جواب, map);

    لكل (صحيح i = 0; i < ڊيگهائي; i++) {
        xcb_xkb_ڪنجي_v_mod_map_t *wire = iter.معلومات;
        بنية xkb_ڪنجي *ڪنجي;

        FAIL_UNLESS(wire->keycode >= keymap->min_ڪنجي_ڪوڊ &&
                    wire->keycode <= keymap->max_ڪنجي_ڪوڊ);

        ڪنجي = &keymap->ڪنجيون[wire->keycode];
        ڪنجي->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_ڪنجي_v_mod_map_ايندڙ(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_map(بنية xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_جواب_t *جواب = xcb_xkb_get_map_جواب(conn, cookie, عدم);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_جواب(جواب, "XkbGetMap");

    إذا ((جواب->present & get_map_required_components) != get_map_required_components)
        اذهب fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(جواب),
                               جواب->nTypes,
                               جواب->n_ڪنجي_Syms,
                               جواب->n_ڪنجي_Actions,
                               جواب->ڪل_Actions,
                               جواب->ڪل_ڪنجي_Behaviors,
                               جواب->virtualMods,
                               جواب->ڪل_ڪنجي_Explicit,
                               جواب->ڪل_Mod_Map_ڪنجيون,
                               جواب->ڪل_VMod_Map_ڪنجيون,
                               جواب->present,
                               &map);

    إذا (!get_types(keymap, conn, جواب, &map) ||
        !get_sym_maps(keymap, conn, جواب, &map) ||
        !get_actions(keymap, conn, جواب, &map) ||
        !get_vmods(keymap, conn, جواب, &map) ||
        !get_explicits(keymap, conn, جواب, &map) ||
        !get_modmaps(keymap, conn, جواب, &map) ||
        !get_vmodmaps(keymap, conn, جواب, &map))
        اذهب fail;

    حرر(جواب);
    أرجع true;

fail:
    حرر(جواب);
    أرجع false;
}

ساكن bool
get_indicators(بنية xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_جواب_t *جواب)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(جواب);

    keymap->انگ_leds = msb_pos(جواب->which);
    FAIL_UNLESS(keymap->انگ_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == انگ_INDICATORS, "");
    لكل (بنانشان i = 0; i < انگ_INDICATORS; i++) {
        إذا (جواب->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.معلومات;
            بنية xkb_led *led = &keymap->leds[i];

            إذا (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_بنياد)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            إذا (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            إذا (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            إذا (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            إذا (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            إذا (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_بنياد)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            إذا (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            إذا (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            إذا (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            إذا (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.نقاب = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_نقاب(wire->ctrls);

            xcb_xkb_indicator_map_ايندڙ(&iter);
        }
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_indicator_map(بنية xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_جواب_t *جواب =
        xcb_xkb_get_indicator_map_جواب(conn, cookie, عدم);

    FAIL_IF_BAD_جواب(جواب, "XkbGetIndicatorMap");

    إذا (!get_indicators(keymap, conn, جواب))
        اذهب fail;

    حرر(جواب);
    أرجع true;

fail:
    حرر(جواب);
    أرجع false;
}

ساكن bool
get_sym_interprets(بنية xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_جواب_t *جواب)
{
    صحيح ڊيگهائي = xcb_xkb_get_compat_map_si_rtrn_ڊيگهائي(جواب);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(جواب);

    FAIL_UNLESS(جواب->پهريون_SIRtrn == 0);
    FAIL_UNLESS(جواب->nSIRtrn == جواب->n_ڪل_SI);

    keymap->انگ_sym_interprets = جواب->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->انگ_sym_interprets);

    ثابت xkb_keycode_t min_ڪنجي_ڪوڊ = keymap->min_ڪنجي_ڪوڊ;
    ثابت xkb_keycode_t max_ڪنجي_ڪوڊ = keymap->max_ڪنجي_ڪوڊ;

    لكل (صحيح i = 0; i < ڊيگهائي; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.معلومات;
        بنية xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        بدل (wire->ميلاپ & XCB_XKB_SYM_INTERP_ميلاپ_OP_نقاب) {
        حالة XCB_XKB_SYM_INTERPRET_ميلاپ_NONE_جو:
            sym_interpret->ميلاپ = ميلاپ_NONE;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_ميلاپ_ANY_جو_OR_NONE:
            sym_interpret->ميلاپ = ميلاپ_ANY_OR_NONE;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_ميلاپ_ANY_جو:
            sym_interpret->ميلاپ = ميلاپ_ANY;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_ميلاپ_ALL_جو:
            sym_interpret->ميلاپ = ميلاپ_ALL;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_ميلاپ_EXACTLY:
            sym_interpret->ميلاپ = ميلاپ_EXACTLY;
            اكسر;
        افتراضي:
            log_err_func(keymap->ctx, XKB_LOG_پيغام_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->ميلاپ & XCB_XKB_SYM_INTERP_ميلاپ_OP_نقاب);
            اذهب fail;
        }

        sym_interpret->level_one_only =
            (wire->ميلاپ & XCB_XKB_SYM_INTERP_ميلاپ_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        إذا (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        وإلا
            sym_interpret->virtual_mod = انگ_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_ڪنجي_ڪوڊ, max_ڪنجي_ڪوڊ));
        sym_interpret->انگ_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_ايندڙ(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_compat_map(بنية xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_جواب_t *جواب =
        xcb_xkb_get_compat_map_جواب(conn, cookie, عدم);

    FAIL_IF_BAD_جواب(جواب, "XkbGetCompatMap");

    إذا (!get_sym_interprets(keymap, conn, جواب))
        اذهب fail;

    حرر(جواب);
    أرجع true;

fail:
    حرر(جواب);
    أرجع false;
}

ساكن bool
get_type_نالا(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
               xcb_xkb_get_نالا_جواب_t *جواب,
               xcb_xkb_get_نالا_قدر_list_t *list)
{
    صحيح ڪنجي_type_نالا_ڊيگهائي =
        xcb_xkb_get_نالا_قدر_list_type_نالا_ڊيگهائي(جواب, list);
    xcb_atom_t *ڪنجي_type_نالا_iter =
        xcb_xkb_get_نالا_قدر_list_type_نالا(list);
    صحيح n_سطحون_في_type_ڊيگهائي =
        xcb_xkb_get_نالا_قدر_list_n_سطحون_في_type_ڊيگهائي(جواب, list);
    uint8_t *n_سطحون_في_type_iter =
        xcb_xkb_get_نالا_قدر_list_n_سطحون_في_type(list);
    xcb_atom_t *kt_level_نالا_iter =
        xcb_xkb_get_نالا_قدر_list_kt_level_نالا(list);

    FAIL_UNLESS(جواب->nTypes == keymap->انگ_types);
    FAIL_UNLESS(ڪنجي_type_نالا_ڊيگهائي == n_سطحون_في_type_ڊيگهائي);

    لكل (صحيح i = 0; i < ڪنجي_type_نالا_ڊيگهائي; i++) {
        xcb_atom_t wire_type_name = *ڪنجي_type_نالا_iter;
        uint8_t wire_انگ_سطحون = *n_سطحون_في_type_iter;
        بنية xkb_ڪنجي_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->انگ_سطحون >= wire_انگ_سطحون);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_نالا, type->انگ_سطحون);
        type->انگ_level_نالا = type->انگ_سطحون;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        لكل (سائيز_t j = 0; j < wire_انگ_سطحون; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_نالا_iter[j],
                                         &type->level_نالا[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        لكل (سائيز_t j = wire_انگ_سطحون; j < type->انگ_سطحون; j++)
            type->level_نالا[j] = XKB_ATOM_NONE;

        kt_level_نالا_iter += wire_انگ_سطحون;
        ڪنجي_type_نالا_iter++;
        n_سطحون_في_type_iter++;
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_indicator_نالا(بنية xkb_keymap *keymap,
                    بنية x11_atom_interner *interner,
                    xcb_xkb_get_نالا_جواب_t *جواب,
                    xcb_xkb_get_نالا_قدر_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_نالا_قدر_list_indicator_نالا(list);

    FAIL_UNLESS(msb_pos(جواب->indicators) <= keymap->انگ_leds);

    لكل (بنانشان i = 0; i < انگ_INDICATORS; i++) {
        إذا (جواب->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            بنية xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_vmod_نالا(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
               xcb_xkb_get_نالا_جواب_t *جواب,
               xcb_xkb_get_نالا_قدر_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_نالا_قدر_list_virtual_mod_نالا(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.انگ_mods =
        انگ_REAL_MODS + MIN(msb_pos(جواب->virtualMods), انگ_VMODS);
    FAIL_UNLESS(keymap->mods.انگ_mods <= XKB_MAX_MODS);

    static_assert(انگ_REAL_MODS + انگ_VMODS <= XKB_MAX_MODS, "");
    لكل (بنانشان i = 0; i < انگ_VMODS; i++) {
        إذا (جواب->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            بنية xkb_mod *mod = &keymap->mods.mods[انگ_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_گروهه_نالا(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
                xcb_xkb_get_نالا_جواب_t *جواب,
                xcb_xkb_get_نالا_قدر_list_t *list)
{
    صحيح ڊيگهائي = xcb_xkb_get_نالا_قدر_list_groups_ڊيگهائي(جواب, list);
    xcb_atom_t *iter = xcb_xkb_get_نالا_قدر_list_groups(list);

    keymap->انگ_گروهه_نالا = msb_pos(جواب->گروهه_نالا_2);
    ALLOC_OR_FAIL(keymap->گروهه_نالا, keymap->انگ_گروهه_نالا);

    لكل (صحيح i = 0; i < ڊيگهائي; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->گروهه_نالا[i]);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_ڪنجي_نالا(بنية xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_نالا_جواب_t *جواب,
              xcb_xkb_get_نالا_قدر_list_t *list)
{
    صحيح ڊيگهائي = xcb_xkb_get_نالا_قدر_list_ڪنجي_نالا_ڊيگهائي(جواب, list);
    xcb_xkb_ڪنجي_name_iterator_t iter =
        xcb_xkb_get_نالا_قدر_list_ڪنجي_نالا_iterator(جواب, list);

    FAIL_UNLESS(جواب->min_ڪنجي_ڪوڊ_2 == keymap->min_ڪنجي_ڪوڊ);
    FAIL_UNLESS(جواب->max_ڪنجي_ڪوڊ_2 == keymap->max_ڪنجي_ڪوڊ);
    FAIL_UNLESS(جواب->پهريون_ڪنجي == keymap->min_ڪنجي_ڪوڊ);
    FAIL_UNLESS(جواب->پهريون_ڪنجي + جواب->n_ڪنجيون - 1U == keymap->max_ڪنجي_ڪوڊ);

    لكل (صحيح i = 0; i < ڊيگهائي; i++) {
        xcb_xkb_ڪنجي_name_t *wire = iter.معلومات;
        xkb_atom_t *ڪنجي_name = &keymap->ڪنجيون[جواب->پهريون_ڪنجي + i].name;

        إذا (wire->name[0] == '\0') {
            *ڪنجي_name = XKB_ATOM_NONE;
        }
        وإلا {
            *ڪنجي_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_ڪنجي_NAME_ڊيگهائي));
            إذا (!*ڪنجي_name)
                أرجع false;
        }

        xcb_xkb_ڪنجي_name_ايندڙ(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_aliases(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_نالا_جواب_t *جواب,
            xcb_xkb_get_نالا_قدر_list_t *list)
{
    صحيح ڊيگهائي = xcb_xkb_get_نالا_قدر_list_ڪنجي_aliases_ڊيگهائي(جواب, list);
    xcb_xkb_ڪنجي_alias_iterator_t iter =
        xcb_xkb_get_نالا_قدر_list_ڪنجي_aliases_iterator(جواب, list);

    keymap->انگ_ڪنجي_aliases = جواب->n_ڪنجي_Aliases;
    ALLOC_OR_FAIL(keymap->ڪنجي_aliases, keymap->انگ_ڪنجي_aliases);

    لكل (صحيح i = 0; i < ڊيگهائي; i++) {
        xcb_xkb_ڪنجي_alias_t *wire = iter.معلومات;
        بنية xkb_ڪنجي_alias *alias = &keymap->ڪنجي_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_ڪنجي_NAME_ڊيگهائي));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_ڪنجي_NAME_ڊيگهائي));
        إذا (!alias->real || !alias->alias)
            اذهب fail;

        xcb_xkb_ڪنجي_alias_ايندڙ(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_نالا(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
          xcb_xkb_get_نالا_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_نالا_جواب_t *جواب =
        xcb_xkb_get_نالا_جواب(conn, cookie, عدم);
    xcb_xkb_get_نالا_قدر_list_t list;

    FAIL_IF_BAD_جواب(جواب, "XkbGetNames");

    FAIL_UNLESS((جواب->which & get_نالا_required) == get_نالا_required);

    xcb_xkb_get_نالا_قدر_list_unpack(xcb_xkb_get_نالا_قدر_list(جواب),
                                        جواب->nTypes,
                                        جواب->indicators,
                                        جواب->virtualMods,
                                        جواب->گروهه_نالا_2,
                                        جواب->n_ڪنجيون,
                                        جواب->n_ڪنجي_Aliases,
                                        جواب->nRadioGroups,
                                        جواب->which,
                                        &list);

    x11_atom_interner_get_escaped_atom_name(interner, list.keycodesName,
                                            &keymap->keycodes_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.symbolsName,
                                            &keymap->symbols_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.typesName,
                                            &keymap->types_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.compatName,
                                            &keymap->compat_section_name);
    إذا (!get_type_نالا(keymap, interner, جواب, &list) ||
        !get_indicator_نالا(keymap, interner, جواب, &list) ||
        !get_vmod_نالا(keymap, interner, جواب, &list) ||
        !get_گروهه_نالا(keymap, interner, جواب, &list) ||
        !get_ڪنجي_نالا(keymap, conn, جواب, &list) ||
        !get_aliases(keymap, conn, جواب, &list))
        اذهب fail;

    حرر(جواب);
    أرجع true;

fail:
    حرر(جواب);
    أرجع false;
}

ساكن bool
get_controls(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_جواب_t *جواب =
        xcb_xkb_get_controls_جواب(conn, cookie, عدم);

    FAIL_IF_BAD_جواب(جواب, "XkbGetControls");
    FAIL_UNLESS(جواب->انگ_Groups > 0 && جواب->انگ_Groups <= 4);

    keymap->انگ_groups = جواب->انگ_Groups;

    FAIL_UNLESS(keymap->max_ڪنجي_ڪوڊ < XCB_XKB_CONST_في_ڪنجي_ٻٽو_انگ_ARRAY_سائيز * 8);

    لكل (xkb_keycode_t i = keymap->min_ڪنجي_ڪوڊ; i <= keymap->max_ڪنجي_ڪوڊ; i++)
        keymap->ڪنجيون[i].repeats = (جواب->في_ڪنجي_Repeat[i / 8] & (1u << (i % 8)));

    حرر(جواب);
    أرجع true;

fail:
    حرر(جواب);
    أرجع false;
}

بنية xkb_keymap *
xkb_x11_keymap_new_from_device(بنية xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               تعداد xkb_keymap_compile_flags flags)
{
    إذا (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_پيغام_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        أرجع عدم;
    }

    ثابت تعداد xkb_keymap_format format = XKB_KEYMAP_FORMAT_متن_V1;
    بنية xkb_keymap * ثابت keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    keymap->redirect_ڪنجي_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    بنية x11_atom_interner interner;
    x11_atom_interner_init(&interner, ctx, conn);

    /*
     * Send all requests together so only one roundtrip is needed
     * to get the replies.
     */
    xcb_xkb_get_map_cookie_t map_cookie =
        xcb_xkb_get_map(conn, device_id, get_map_required_components,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    xcb_xkb_get_indicator_map_cookie_t indicator_map_cookie =
        xcb_xkb_get_indicator_map(conn, device_id, ALL_INDICATORS_نقاب);
    xcb_xkb_get_compat_map_cookie_t compat_map_cookie =
        xcb_xkb_get_compat_map(conn, device_id, 0, true, 0, 0);
    xcb_xkb_get_نالا_cookie_t نالا_cookie =
        xcb_xkb_get_نالا(conn, device_id, get_نالا_wanted);
    xcb_xkb_get_controls_cookie_t controls_cookie =
        xcb_xkb_get_controls(conn, device_id);

    إذا (!get_map(keymap, conn, map_cookie))
        اذهب err_map;
    إذا (!get_indicator_map(keymap, conn, indicator_map_cookie))
        اذهب err_indicator_map;
    إذا (!get_compat_map(keymap, conn, compat_map_cookie))
        اذهب err_compat_map;
    إذا (!get_نالا(keymap, &interner, نالا_cookie))
        اذهب err_نالا;
    إذا (!get_controls(keymap, conn, controls_cookie))
        اذهب err_controls;
    x11_atom_interner_ڦيرو_trip(&interner);
    إذا (interner.had_error)
        اذهب err_interner;

    أرجع keymap;

err_map:
    xcb_discard_جواب(conn, indicator_map_cookie.sequence);
err_indicator_map:
    xcb_discard_جواب(conn, compat_map_cookie.sequence);
err_compat_map:
    xcb_discard_جواب(conn, نالا_cookie.sequence);
err_نالا:
    xcb_discard_جواب(conn, controls_cookie.sequence);
err_controls:
    x11_atom_interner_ڦيرو_trip(&interner);
err_interner:
    xkb_keymap_unref(keymap);
    أرجع عدم;
}
