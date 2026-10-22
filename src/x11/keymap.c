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
#تعریف_کرو عدد_REAL_MODS 8u
/* XkbNumVirtualMods. */
#تعریف_کرو عدد_VMODS 16u
/* XkbNoModifier. */
#تعریف_کرو NO_MODIFIER 0xff
/* XkbNumIndicators. */
#تعریف_کرو عدد_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#تعریف_کرو ALL_INDICATORS_نقاب 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#تعریف_کرو FAIL_UNLESS(اظہاریہ) افعل {                              \
    إذا (!(اظہاریہ)) {                                          \
        log_err(keymap->ctx, XKB_LOG_سنیہا_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(اظہاریہ));                 \
        اذهب fail;                                          \
    }                                                       \
} طالما (0)

#تعریف_کرو FAIL_IF_BAD_جواب(جواب, request_name) افعل {         \
    إذا (!(جواب)) {                                           \
        log_err(keymap->ctx, XKB_LOG_سنیہا_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        اذهب fail;                                          \
    }                                                       \
} طالما (0)

#تعریف_کرو ALLOC_OR_FAIL(arr, nmemb) افعل {                      \
    إذا ((nmemb) > 0) {                                      \
        (arr) = صفر_حافظہ_دیو((nmemb), حجم(*(arr)));            \
        إذا (!(arr))                                         \
            اذهب fail;                                      \
    }                                                       \
} طالما (0)

ساكن ثابت xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_کلید_TYPES |
     XCB_XKB_MAP_PART_کلید_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_کلید_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

ساكن ثابت xcb_xkb_name_detail_t get_ناں_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_کلید_TYPE_ناں |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_ناں |
     XCB_XKB_NAME_DETAIL_INDICATOR_ناں |
     XCB_XKB_NAME_DETAIL_کلید_ناں |
     XCB_XKB_NAME_DETAIL_کلید_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_ناں |
     XCB_XKB_NAME_DETAIL_گروہ_ناں);
ساكن ثابت xcb_xkb_name_detail_t get_ناں_required =
    (XCB_XKB_NAME_DETAIL_کلید_TYPE_ناں |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_ناں |
     XCB_XKB_NAME_DETAIL_کلید_ناں |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_ناں);


ساكن xkb_mod_نقاب_t
translate_mods(uint8_t rmods, uint16_t vmods_کم, uint16_t vmods_بلند)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    أرجع
        ((xkb_mod_نقاب_t) rmods) |
        ((xkb_mod_نقاب_t) vmods_کم << 8) |
        ((xkb_mod_نقاب_t) vmods_بلند << 16);
}

تعداد xkb_action_controls
translate_controls_نقاب(uint32_t wire)
{
    تعداد xkb_action_controls ret = 0;
    إذا (wire & XCB_XKB_BOOL_CTRL_REPEAT_کلیدیں)
        ret |= CONTROL_REPEAT;
    إذا (wire & XCB_XKB_BOOL_CTRL_SLOW_کلیدیں)
        ret |= CONTROL_SLOW;
    إذا (wire & XCB_XKB_BOOL_CTRL_BOUNCE_کلیدیں)
        ret |= CONTROL_DEBOUNCE;
    إذا (wire & XCB_XKB_BOOL_CTRL_STICKY_کلیدیں)
        ret |= CONTROL_STICKY_کلیدیں;
    إذا (wire & XCB_XKB_BOOL_CTRL_MOUSE_کلیدیں)
        ret |= CONTROL_MOUSE_کلیدیں;
    إذا (wire & XCB_XKB_BOOL_CTRL_MOUSE_کلیدیں_ACCEL)
        ret |= CONTROL_MOUSE_کلیدیں_ACCEL;
    إذا (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_کلیدیں)
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
    إذا (wire & XCB_XKB_BOOL_CTRL_IGNORE_گروہ_جندرا_نقاب)
        ret |= CONTROL_IGNORE_گروہ_جندرا;
    /* Some controls are not supported and don't appear here. */
    أرجع ret;
}

ساكن bool
translate_action(اتحاد xkb_action *action, ثابت xcb_xkb_action_t *wire,
                 xkb_keycode_t min_کلید_کوڈ, xkb_keycode_t max_کلید_کوڈ)
{
    بدل (wire->type) {
    حالة XCB_XKB_SA_TYPE_سیٹ_MODS:
        action->type = ACTION_TYPE_MOD_سیٹ;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_کم_2,
                                                wire->setmods.vmods_بلند_2);
        action->mods.mods.نقاب = translate_mods(wire->setmods.نقاب, 0, 0);

        إذا (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_جندرا_CLEAR;
        إذا (wire->setmods.flags & XCB_XKB_SA_LATCH_منزل_جندرا)
            action->mods.flags |= ACTION_LATCH_منزل_جندرا;
        إذا (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_کم_2,
                                                wire->latchmods.vmods_بلند_2);
        action->mods.mods.نقاب = translate_mods(wire->latchmods.نقاب, 0, 0);

        إذا (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_جندرا_CLEAR;
        إذا (wire->latchmods.flags & XCB_XKB_SA_LATCH_منزل_جندرا)
            action->mods.flags |= ACTION_LATCH_منزل_جندرا;
        إذا (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_جندرا_MODS:
        action->type = ACTION_TYPE_MOD_جندرا;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_کم_2,
                                                wire->lockmods.vmods_بلند_2);
        action->mods.mods.نقاب = translate_mods(wire->lockmods.نقاب, 0, 0);

        إذا (wire->lockmods.flags & XCB_XKB_SA_ISO_جندرا_FLAG_NO_جندرا)
            action->mods.flags |= ACTION_جندرا_NO_جندرا;
        إذا (wire->lockmods.flags & XCB_XKB_SA_ISO_جندرا_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_جندرا_NO_UNLOCK;
        إذا (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_سیٹ_گروہ:
        action->type = ACTION_TYPE_گروہ_سیٹ;

        action->گروہ.گروہ = (int32_t)wire->setgroup.گروہ;

        إذا (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->گروہ.flags |= ACTION_جندرا_CLEAR;
        إذا (wire->setmods.flags & XCB_XKB_SA_LATCH_منزل_جندرا)
            action->گروہ.flags |= ACTION_LATCH_منزل_جندرا;
        إذا (wire->setmods.flags & XCB_XKB_SA_ISO_جندرا_FLAG_گروہ_ABSOLUTE)
            action->گروہ.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_LATCH_گروہ:
        action->type = ACTION_TYPE_گروہ_LATCH;

        action->گروہ.گروہ = (int32_t)wire->latchgroup.گروہ;

        إذا (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->گروہ.flags |= ACTION_جندرا_CLEAR;
        إذا (wire->latchmods.flags & XCB_XKB_SA_LATCH_منزل_جندرا)
            action->گروہ.flags |= ACTION_LATCH_منزل_جندرا;
        إذا (wire->latchmods.flags & XCB_XKB_SA_ISO_جندرا_FLAG_گروہ_ABSOLUTE)
            action->گروہ.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_جندرا_گروہ:
        action->type = ACTION_TYPE_گروہ_جندرا;

        action->گروہ.گروہ = (int32_t)wire->lockgroup.گروہ;

        إذا (wire->lockgroup.flags & XCB_XKB_SA_ISO_جندرا_FLAG_گروہ_ABSOLUTE)
            action->گروہ.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_کم | ((uint16_t) wire->moveptr.x_بلند << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_کم | ((uint16_t) wire->moveptr.y_بلند << 8));

        إذا (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        إذا (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        إذا (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        اكسر;
    حالة XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.گنتی = wire->ptrbtn.گنتی;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        اكسر;
    حالة XCB_XKB_SA_TYPE_جندرا_PTR_BTN:
        action->type = ACTION_TYPE_PTR_جندرا;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.گنتی = 0;

        إذا (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_جندرا_FLAG_NO_جندرا)
            action->btn.flags |= ACTION_جندرا_NO_جندرا;
        إذا (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_جندرا_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_جندرا_NO_UNLOCK;

        اكسر;
    حالة XCB_XKB_SA_TYPE_سیٹ_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.قدر = wire->setptrdflt.قدر;

        إذا (wire->setptrdflt.flags & XCB_XKB_SA_سیٹ_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
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
    حالة XCB_XKB_SA_TYPE_سیٹ_CONTROLS:
        action->type = ACTION_TYPE_CTRL_سیٹ;
        {
            ثابت uint16_t نقاب = (wire->setcontrols.bool_Ctrls_کم |
                                   (wire->setcontrols.bool_Ctrls_بلند << 8));
            action->ctrls.ctrls = translate_controls_نقاب(نقاب);
        }
        اكسر;
    حالة XCB_XKB_SA_TYPE_جندرا_CONTROLS:
        action->type = ACTION_TYPE_CTRL_جندرا;
        {
            ثابت uint16_t نقاب = (wire->lockcontrols.bool_Ctrls_کم |
                                   (wire->lockcontrols.bool_Ctrls_بلند << 8));
            action->ctrls.ctrls = translate_controls_نقاب(نقاب);
        }
        اكسر;

    حالة XCB_XKB_SA_TYPE_REDIRECT_کلید:
        action->type = ACTION_TYPE_REDIRECT_کلید;
        إذا (wire->redirect.newkey < min_کلید_کوڈ ||
            wire->redirect.newkey > max_کلید_کوڈ)
                أرجع false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.نقاب,
            wire->redirect.vmods_نقاب_کم,
            wire->redirect.vmods_نقاب_بلند
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_کم_2,
                                               wire->redirect.vmods_بلند_2);
        اكسر;

    حالة XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        اكسر;

    /* We don't support these. */
    حالة XCB_XKB_SA_TYPE_ISO_جندرا:
    حالة XCB_XKB_SA_TYPE_ACTION_سنیہا:
    حالة XCB_XKB_SA_TYPE_DEVICE_BTN:
    حالة XCB_XKB_SA_TYPE_جندرا_DEVICE_BTN:
    حالة XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندرونی == 20 &&
                      ACTION_TYPE_اندرونی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");

        إذا (wire->type < ACTION_TYPE_نجی) {
            action->type = ACTION_TYPE_NONE;
            اكسر;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(حجم(action->priv.انکڑے) == 7 &&
                      حجم(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        حافظہ_نقل(action->priv.انکڑے, wire->noaction.pad0, 7);
        اكسر;
    }
    أرجع true;
}

ساكن bool
get_types(بنية xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح types_لمائی = xcb_xkb_get_map_map_types_rtrn_لمائی(جواب, map);
    xcb_xkb_کلید_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(جواب, map);

    FAIL_UNLESS(جواب->پہلا_Type == 0);

    keymap->عدد_types = جواب->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->عدد_types);
    أكد(types_لمائی == (صحيح) keymap->عدد_types);

    لكل (صحيح i = 0; i < types_لمائی; i++) {
        xcb_xkb_کلید_type_t *wire_type = types_iter.انکڑے;
        بنية xkb_کلید_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->عدد_پدھراں_2 > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.نقاب = translate_mods(wire_type->mods_نقاب, 0, 0);
        type->عدد_پدھراں = wire_type->عدد_پدھراں_2;

        {
            صحيح entries_لمائی = xcb_xkb_کلید_type_map_لمائی(wire_type);
            xcb_xkb_kt_map_اندراج_iterator_t entries_iter =
                xcb_xkb_کلید_type_map_iterator(wire_type);

            type->عدد_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->عدد_entries);
            أكد(entries_لمائی == (صحيح) type->عدد_entries);

            لكل (صحيح j = 0; j < entries_لمائی; j++) {
                xcb_xkb_kt_map_اندراج_t *wire_اندراج = entries_iter.انکڑے;
                بنية xkb_کلید_type_اندراج *اندراج = &type->entries[j];

                FAIL_UNLESS(wire_اندراج->level < type->عدد_پدھراں);

                اندراج->level = wire_اندراج->level;
                اندراج->mods.mods = translate_mods(wire_اندراج->mods_mods,
                                                  wire_اندراج->mods_vmods, 0);
                اندراج->mods.نقاب = translate_mods(wire_اندراج->mods_نقاب, 0, 0);

                xcb_xkb_kt_map_اندراج_اگلا(&entries_iter);
            }
        }

        {
            صحيح preserves_لمائی = xcb_xkb_کلید_type_preserve_لمائی(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_کلید_type_preserve_iterator(wire_type);

            FAIL_UNLESS((بنانشانی) preserves_لمائی <= type->عدد_entries);

            لكل (صحيح j = 0; j < preserves_لمائی; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.انکڑے;
                بنية xkb_کلید_type_اندراج *اندراج = &type->entries[j];

                اندراج->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                اندراج->preserve.نقاب = translate_mods(wire_preserve->نقاب, 0, 0);

                xcb_xkb_mod_def_اگلا(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_کلید_type_اگلا(&types_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_sym_maps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح sym_maps_لمائی = xcb_xkb_get_map_map_syms_rtrn_لمائی(جواب, map);
    xcb_xkb_کلید_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(جواب, map);

    FAIL_UNLESS(جواب->min_کلید_کوڈ_2 <= جواب->max_کلید_کوڈ_2);
    FAIL_UNLESS(جواب->پہلا_کلید_Sym >= جواب->min_کلید_کوڈ_2);
    FAIL_UNLESS(جواب->پہلا_کلید_Sym + جواب->n_کلید_Syms <= جواب->max_کلید_کوڈ_2 + 1);

    keymap->min_کلید_کوڈ = جواب->min_کلید_کوڈ_2;
    keymap->max_کلید_کوڈ = جواب->max_کلید_کوڈ_2;
    keymap->عدد_کلیدیں = keymap->عدد_کلیدیں_کم = جواب->max_کلید_کوڈ_2 + 1;

    ALLOC_OR_FAIL(keymap->کلیدیں, keymap->max_کلید_کوڈ + 1);

    لكل (xkb_keycode_t kc = keymap->min_کلید_کوڈ; kc <= keymap->max_کلید_کوڈ; kc++)
        keymap->کلیدیں[kc].keycode = kc;

    لكل (صحيح i = 0; i < sym_maps_لمائی; i++) {
        xcb_xkb_کلید_sym_map_t *wire_sym_map = sym_maps_iter.انکڑے;
        بنية xkb_کلید *کلید = &keymap->کلیدیں[جواب->پہلا_کلید_Sym + i];

        کلید->عدد_groups = wire_sym_map->گروہ_Info & 0x0f;
        FAIL_UNLESS(کلید->عدد_groups <= ARRAY_ناپ(wire_sym_map->kt_اشاریہ));
        ALLOC_OR_FAIL(کلید->groups, کلید->عدد_groups);

        لكل (xkb_layout_اشاریہ_t j = 0; j < کلید->عدد_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_اشاریہ[j] < keymap->عدد_types);
            کلید->groups[j].type = &keymap->types[wire_sym_map->kt_اشاریہ[j]];

            ALLOC_OR_FAIL(کلید->groups[j].پدھراں, کلید->groups[j].type->عدد_پدھراں);
        }

        کلید->باہرلا_دا_range_گروہ_نمبر = (wire_sym_map->گروہ_Info & 0x30) >> 4;

        FAIL_UNLESS(کلید->باہرلا_دا_range_گروہ_نمبر <= کلید->عدد_groups);

        إذا (wire_sym_map->گروہ_Info & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            کلید->باہرلا_دا_range_گروہ_policy = XKB_LAYOUT_باہرلا_دا_RANGE_CLAMP;
        وإلا إذا (wire_sym_map->گروہ_Info & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            کلید->باہرلا_دا_range_گروہ_policy = XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT;
        وإلا
            کلید->باہرلا_دا_range_گروہ_policy = XKB_LAYOUT_باہرلا_دا_RANGE_WRAP;

        {
            صحيح syms_لمائی = xcb_xkb_کلید_sym_map_syms_لمائی(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_کلید_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((بنانشانی) syms_لمائی == wire_sym_map->چوڑائی * کلید->عدد_groups);

            إذا (syms_لمائی > 0)
                کلید->explicit |= EXPLICIT_SYMBOLS;

            لكل (xkb_layout_اشاریہ_t گروہ = 0; گروہ < کلید->عدد_groups; گروہ++) {
                لكل (xkb_level_اشاریہ_t level = 0; level < wire_sym_map->چوڑائی; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    أكد(کلید->groups[گروہ].type != عدم);
                    إذا (level < کلید->groups[گروہ].type->عدد_پدھراں) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        کلید->groups[گروہ].پدھراں[level].عدد_syms = 1;
                        کلید->groups[گروہ].پدھراں[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        کلید->groups[گروہ].پدھراں[level].اتلا =
                            xkb_keysym_منزل_اتلا(wire_keysym);
                        إذا (wire_keysym != XKB_کلید_No_علامت)
                            کلید->groups[گروہ].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_کلید_sym_map_اگلا(&sym_maps_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_actions(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح acts_گنتی_لمائی =
        xcb_xkb_get_map_map_acts_rtrn_گنتی_لمائی(جواب, map);
    uint8_t *acts_گنتی_iter = xcb_xkb_get_map_map_acts_rtrn_گنتی(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(جواب, map);
    xcb_xkb_کلید_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(جواب, map);

    FAIL_UNLESS(جواب->پہلا_کلید_Action == keymap->min_کلید_کوڈ);
    FAIL_UNLESS(جواب->پہلا_کلید_Action + جواب->n_کلید_Actions ==
                keymap->max_کلید_کوڈ + 1);

    لكل (صحيح i = 0; i < acts_گنتی_لمائی; i++) {
        xcb_xkb_کلید_sym_map_t *wire_sym_map = sym_maps_iter.انکڑے;
        صحيح syms_لمائی = xcb_xkb_کلید_sym_map_syms_لمائی(wire_sym_map);
        uint8_t wire_گنتی = *acts_گنتی_iter;
        بنية xkb_کلید *کلید = &keymap->کلیدیں[جواب->پہلا_کلید_Action + i];

        FAIL_UNLESS((بنانشانی) syms_لمائی == wire_sym_map->چوڑائی * کلید->عدد_groups);
        FAIL_UNLESS(wire_گنتی == 0 || wire_گنتی == syms_لمائی);

        ثابت xkb_keycode_t min_کلید_کوڈ = keymap->min_کلید_کوڈ;
        ثابت xkb_keycode_t max_کلید_کوڈ = keymap->max_کلید_کوڈ;

        إذا (wire_گنتی != 0) {
            لكل (xkb_layout_اشاریہ_t گروہ = 0; گروہ < کلید->عدد_groups; گروہ++) {
                لكل (xkb_level_اشاریہ_t level = 0; level < wire_sym_map->چوڑائی; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.انکڑے;

                    إذا (level < کلید->groups[گروہ].type->عدد_پدھراں) {
                        کلید->groups[گروہ].پدھراں[level].عدد_actions = 1;
                        اتحاد xkb_action *action =
                            &کلید->groups[گروہ].پدھراں[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_کلید_کوڈ, max_کلید_کوڈ));

                        إذا (action->type != ACTION_TYPE_NONE) {
                            کلید->groups[گروہ].implicit_actions =
                                !(کلید->explicit & EXPLICIT_INTERP);
                        } وإلا إذا (کلید->groups[گروہ].پدھراں[level].s.sym ==
                                   XKB_کلید_No_علامت) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            کلید->groups[گروہ].پدھراں[level].عدد_syms = 0;
                            کلید->groups[گروہ].پدھراں[level].عدد_actions = 0;
                        }
                    }

                    xcb_xkb_action_اگلا(&acts_iter);
                }

                إذا (کلید->groups[گروہ].implicit_actions)
                    کلید->implicit_actions = true;
            }
        }

        acts_گنتی_iter++;
        xcb_xkb_کلید_sym_map_اگلا(&sym_maps_iter);
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

    keymap->mods.عدد_mods =
        عدد_REAL_MODS + MIN(msb_pos(جواب->virtualMods), عدد_VMODS);
    FAIL_UNLESS(keymap->mods.عدد_mods <= XKB_MAX_MODS);

    static_assert(عدد_REAL_MODS + عدد_VMODS <= XKB_MAX_MODS, "");
    لكل (بنانشانی i = 0; i < عدد_VMODS; i++) {
        إذا (جواب->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            بنية xkb_mod *mod = &keymap->mods.mods[عدد_REAL_MODS + i];

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
    صحيح لمائی = xcb_xkb_get_map_map_explicit_rtrn_لمائی(جواب, map);
    xcb_xkb_سیٹ_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(جواب, map);

    لكل (صحيح i = 0; i < لمائی; i++) {
        xcb_xkb_سیٹ_explicit_t *wire = iter.انکڑے;
        بنية xkb_کلید *کلید;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلید_کوڈ &&
                    wire->keycode <= keymap->max_کلید_کوڈ);

        کلید = &keymap->کلیدیں[wire->keycode];

        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_1) &&
            کلید->عدد_groups > 0) {
            کلید->groups[0].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_2) &&
            کلید->عدد_groups > 1) {
            کلید->groups[1].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_3) &&
            کلید->عدد_groups > 2) {
            کلید->groups[2].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_4) &&
            کلید->عدد_groups > 3) {
            کلید->groups[3].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        إذا (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            کلید->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            لكل (xkb_layout_اشاریہ_t l = 0; l < کلید->عدد_groups; l++) {
                کلید->groups[l].explicit_actions = true;
            }
        }
        إذا (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            کلید->explicit |= EXPLICIT_REPEAT;
        إذا (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            کلید->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_سیٹ_explicit_اگلا(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_modmaps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح لمائی = xcb_xkb_get_map_map_modmap_rtrn_لمائی(جواب, map);
    xcb_xkb_کلید_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(جواب, map);

    darray(Mod_Map_اندراج) modmaps = darray_new();

    لكل (صحيح i = 0; i < لمائی; i++) {
        xcb_xkb_کلید_mod_map_t *wire = iter.انکڑے;
        بنية xkb_کلید *کلید;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلید_کوڈ &&
                    wire->keycode <= keymap->max_کلید_کوڈ);

        کلید = &keymap->کلیدیں[wire->keycode];
        کلید->modmap = wire->mods;

        darray_append(modmaps, (Mod_Map_اندراج) {
            .کلید_کوڈ = کلید->keycode,
            .mods = کلید->modmap,
            .have_علامت = false,
            .u.کلید_Name = کلید->name
        });

        xcb_xkb_کلید_mod_map_اگلا(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->عدد_modmaps);
    أرجع true;

fail:
    darray_free(modmaps);
    أرجع false;
}

ساكن bool
get_vmodmaps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_جواب_t *جواب, xcb_xkb_get_map_map_t *map)
{
    صحيح لمائی = xcb_xkb_get_map_map_vmodmap_rtrn_لمائی(جواب, map);
    xcb_xkb_کلید_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(جواب, map);

    لكل (صحيح i = 0; i < لمائی; i++) {
        xcb_xkb_کلید_v_mod_map_t *wire = iter.انکڑے;
        بنية xkb_کلید *کلید;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلید_کوڈ &&
                    wire->keycode <= keymap->max_کلید_کوڈ);

        کلید = &keymap->کلیدیں[wire->keycode];
        کلید->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_کلید_v_mod_map_اگلا(&iter);
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
                               جواب->n_کلید_Syms,
                               جواب->n_کلید_Actions,
                               جواب->کل_Actions,
                               جواب->کل_کلید_Behaviors,
                               جواب->virtualMods,
                               جواب->کل_کلید_Explicit,
                               جواب->کل_Mod_Map_کلیدیں,
                               جواب->کل_VMod_Map_کلیدیں,
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

    keymap->عدد_leds = msb_pos(جواب->which);
    FAIL_UNLESS(keymap->عدد_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == عدد_INDICATORS, "");
    لكل (بنانشانی i = 0; i < عدد_INDICATORS; i++) {
        إذا (جواب->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.انکڑے;
            بنية xkb_led *led = &keymap->leds[i];

            إذا (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_ادھار)
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

            إذا (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_ادھار)
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

            xcb_xkb_indicator_map_اگلا(&iter);
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
    صحيح لمائی = xcb_xkb_get_compat_map_si_rtrn_لمائی(جواب);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(جواب);

    FAIL_UNLESS(جواب->پہلا_SIRtrn == 0);
    FAIL_UNLESS(جواب->nSIRtrn == جواب->n_کل_SI);

    keymap->عدد_sym_interprets = جواب->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->عدد_sym_interprets);

    ثابت xkb_keycode_t min_کلید_کوڈ = keymap->min_کلید_کوڈ;
    ثابت xkb_keycode_t max_کلید_کوڈ = keymap->max_کلید_کوڈ;

    لكل (صحيح i = 0; i < لمائی; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.انکڑے;
        بنية xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        بدل (wire->میل & XCB_XKB_SYM_INTERP_میل_OP_نقاب) {
        حالة XCB_XKB_SYM_INTERPRET_میل_NONE_دا:
            sym_interpret->میل = میل_NONE;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_میل_ANY_دا_OR_NONE:
            sym_interpret->میل = میل_ANY_OR_NONE;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_میل_ANY_دا:
            sym_interpret->میل = میل_ANY;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_میل_ALL_دا:
            sym_interpret->میل = میل_ALL;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_میل_EXACTLY:
            sym_interpret->میل = میل_EXACTLY;
            اكسر;
        افتراضي:
            log_err_func(keymap->ctx, XKB_LOG_سنیہا_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->میل & XCB_XKB_SYM_INTERP_میل_OP_نقاب);
            اذهب fail;
        }

        sym_interpret->level_one_only =
            (wire->میل & XCB_XKB_SYM_INTERP_میل_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        إذا (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        وإلا
            sym_interpret->virtual_mod = عدد_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_کلید_کوڈ, max_کلید_کوڈ));
        sym_interpret->عدد_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_اگلا(&iter);
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
get_type_ناں(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
               xcb_xkb_get_ناں_جواب_t *جواب,
               xcb_xkb_get_ناں_قدر_list_t *list)
{
    صحيح کلید_type_ناں_لمائی =
        xcb_xkb_get_ناں_قدر_list_type_ناں_لمائی(جواب, list);
    xcb_atom_t *کلید_type_ناں_iter =
        xcb_xkb_get_ناں_قدر_list_type_ناں(list);
    صحيح n_پدھراں_فی_type_لمائی =
        xcb_xkb_get_ناں_قدر_list_n_پدھراں_فی_type_لمائی(جواب, list);
    uint8_t *n_پدھراں_فی_type_iter =
        xcb_xkb_get_ناں_قدر_list_n_پدھراں_فی_type(list);
    xcb_atom_t *kt_level_ناں_iter =
        xcb_xkb_get_ناں_قدر_list_kt_level_ناں(list);

    FAIL_UNLESS(جواب->nTypes == keymap->عدد_types);
    FAIL_UNLESS(کلید_type_ناں_لمائی == n_پدھراں_فی_type_لمائی);

    لكل (صحيح i = 0; i < کلید_type_ناں_لمائی; i++) {
        xcb_atom_t wire_type_name = *کلید_type_ناں_iter;
        uint8_t wire_عدد_پدھراں = *n_پدھراں_فی_type_iter;
        بنية xkb_کلید_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->عدد_پدھراں >= wire_عدد_پدھراں);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_ناں, type->عدد_پدھراں);
        type->عدد_level_ناں = type->عدد_پدھراں;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        لكل (ناپ_t j = 0; j < wire_عدد_پدھراں; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_ناں_iter[j],
                                         &type->level_ناں[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        لكل (ناپ_t j = wire_عدد_پدھراں; j < type->عدد_پدھراں; j++)
            type->level_ناں[j] = XKB_ATOM_NONE;

        kt_level_ناں_iter += wire_عدد_پدھراں;
        کلید_type_ناں_iter++;
        n_پدھراں_فی_type_iter++;
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_indicator_ناں(بنية xkb_keymap *keymap,
                    بنية x11_atom_interner *interner,
                    xcb_xkb_get_ناں_جواب_t *جواب,
                    xcb_xkb_get_ناں_قدر_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_ناں_قدر_list_indicator_ناں(list);

    FAIL_UNLESS(msb_pos(جواب->indicators) <= keymap->عدد_leds);

    لكل (بنانشانی i = 0; i < عدد_INDICATORS; i++) {
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
get_vmod_ناں(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
               xcb_xkb_get_ناں_جواب_t *جواب,
               xcb_xkb_get_ناں_قدر_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_ناں_قدر_list_virtual_mod_ناں(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.عدد_mods =
        عدد_REAL_MODS + MIN(msb_pos(جواب->virtualMods), عدد_VMODS);
    FAIL_UNLESS(keymap->mods.عدد_mods <= XKB_MAX_MODS);

    static_assert(عدد_REAL_MODS + عدد_VMODS <= XKB_MAX_MODS, "");
    لكل (بنانشانی i = 0; i < عدد_VMODS; i++) {
        إذا (جواب->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            بنية xkb_mod *mod = &keymap->mods.mods[عدد_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_گروہ_ناں(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
                xcb_xkb_get_ناں_جواب_t *جواب,
                xcb_xkb_get_ناں_قدر_list_t *list)
{
    صحيح لمائی = xcb_xkb_get_ناں_قدر_list_groups_لمائی(جواب, list);
    xcb_atom_t *iter = xcb_xkb_get_ناں_قدر_list_groups(list);

    keymap->عدد_گروہ_ناں = msb_pos(جواب->گروہ_ناں_2);
    ALLOC_OR_FAIL(keymap->گروہ_ناں, keymap->عدد_گروہ_ناں);

    لكل (صحيح i = 0; i < لمائی; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->گروہ_ناں[i]);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_کلید_ناں(بنية xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_ناں_جواب_t *جواب,
              xcb_xkb_get_ناں_قدر_list_t *list)
{
    صحيح لمائی = xcb_xkb_get_ناں_قدر_list_کلید_ناں_لمائی(جواب, list);
    xcb_xkb_کلید_name_iterator_t iter =
        xcb_xkb_get_ناں_قدر_list_کلید_ناں_iterator(جواب, list);

    FAIL_UNLESS(جواب->min_کلید_کوڈ_2 == keymap->min_کلید_کوڈ);
    FAIL_UNLESS(جواب->max_کلید_کوڈ_2 == keymap->max_کلید_کوڈ);
    FAIL_UNLESS(جواب->پہلا_کلید == keymap->min_کلید_کوڈ);
    FAIL_UNLESS(جواب->پہلا_کلید + جواب->n_کلیدیں - 1U == keymap->max_کلید_کوڈ);

    لكل (صحيح i = 0; i < لمائی; i++) {
        xcb_xkb_کلید_name_t *wire = iter.انکڑے;
        xkb_atom_t *کلید_name = &keymap->کلیدیں[جواب->پہلا_کلید + i].name;

        إذا (wire->name[0] == '\0') {
            *کلید_name = XKB_ATOM_NONE;
        }
        وإلا {
            *کلید_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_کلید_NAME_لمائی));
            إذا (!*کلید_name)
                أرجع false;
        }

        xcb_xkb_کلید_name_اگلا(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_aliases(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_ناں_جواب_t *جواب,
            xcb_xkb_get_ناں_قدر_list_t *list)
{
    صحيح لمائی = xcb_xkb_get_ناں_قدر_list_کلید_aliases_لمائی(جواب, list);
    xcb_xkb_کلید_alias_iterator_t iter =
        xcb_xkb_get_ناں_قدر_list_کلید_aliases_iterator(جواب, list);

    keymap->عدد_کلید_aliases = جواب->n_کلید_Aliases;
    ALLOC_OR_FAIL(keymap->کلید_aliases, keymap->عدد_کلید_aliases);

    لكل (صحيح i = 0; i < لمائی; i++) {
        xcb_xkb_کلید_alias_t *wire = iter.انکڑے;
        بنية xkb_کلید_alias *alias = &keymap->کلید_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_کلید_NAME_لمائی));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_کلید_NAME_لمائی));
        إذا (!alias->real || !alias->alias)
            اذهب fail;

        xcb_xkb_کلید_alias_اگلا(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_ناں(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
          xcb_xkb_get_ناں_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_ناں_جواب_t *جواب =
        xcb_xkb_get_ناں_جواب(conn, cookie, عدم);
    xcb_xkb_get_ناں_قدر_list_t list;

    FAIL_IF_BAD_جواب(جواب, "XkbGetNames");

    FAIL_UNLESS((جواب->which & get_ناں_required) == get_ناں_required);

    xcb_xkb_get_ناں_قدر_list_unpack(xcb_xkb_get_ناں_قدر_list(جواب),
                                        جواب->nTypes,
                                        جواب->indicators,
                                        جواب->virtualMods,
                                        جواب->گروہ_ناں_2,
                                        جواب->n_کلیدیں,
                                        جواب->n_کلید_Aliases,
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
    إذا (!get_type_ناں(keymap, interner, جواب, &list) ||
        !get_indicator_ناں(keymap, interner, جواب, &list) ||
        !get_vmod_ناں(keymap, interner, جواب, &list) ||
        !get_گروہ_ناں(keymap, interner, جواب, &list) ||
        !get_کلید_ناں(keymap, conn, جواب, &list) ||
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
    FAIL_UNLESS(جواب->عدد_Groups > 0 && جواب->عدد_Groups <= 4);

    keymap->عدد_groups = جواب->عدد_Groups;

    FAIL_UNLESS(keymap->max_کلید_کوڈ < XCB_XKB_CONST_فی_کلید_ثنائی_ہندسہ_ARRAY_ناپ * 8);

    لكل (xkb_keycode_t i = keymap->min_کلید_کوڈ; i <= keymap->max_کلید_کوڈ; i++)
        keymap->کلیدیں[i].repeats = (جواب->فی_کلید_Repeat[i / 8] & (1u << (i % 8)));

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
        log_err_func(ctx, XKB_LOG_سنیہا_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        أرجع عدم;
    }

    ثابت تعداد xkb_keymap_format format = XKB_KEYMAP_FORMAT_متن_V1;
    بنية xkb_keymap * ثابت keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    keymap->redirect_کلید_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

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
    xcb_xkb_get_ناں_cookie_t ناں_cookie =
        xcb_xkb_get_ناں(conn, device_id, get_ناں_wanted);
    xcb_xkb_get_controls_cookie_t controls_cookie =
        xcb_xkb_get_controls(conn, device_id);

    إذا (!get_map(keymap, conn, map_cookie))
        اذهب err_map;
    إذا (!get_indicator_map(keymap, conn, indicator_map_cookie))
        اذهب err_indicator_map;
    إذا (!get_compat_map(keymap, conn, compat_map_cookie))
        اذهب err_compat_map;
    إذا (!get_ناں(keymap, &interner, ناں_cookie))
        اذهب err_ناں;
    إذا (!get_controls(keymap, conn, controls_cookie))
        اذهب err_controls;
    x11_atom_interner_گیڑا_trip(&interner);
    إذا (interner.had_error)
        اذهب err_interner;

    أرجع keymap;

err_map:
    xcb_discard_جواب(conn, indicator_map_cookie.sequence);
err_indicator_map:
    xcb_discard_جواب(conn, compat_map_cookie.sequence);
err_compat_map:
    xcb_discard_جواب(conn, ناں_cookie.sequence);
err_ناں:
    xcb_discard_جواب(conn, controls_cookie.sequence);
err_controls:
    x11_atom_interner_گیڑا_trip(&interner);
err_interner:
    xkb_keymap_unref(keymap);
    أرجع عدم;
}
