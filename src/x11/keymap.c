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
#تعریف_کړه عدد_REAL_MODS 8u
/* XkbNumVirtualMods. */
#تعریف_کړه عدد_VMODS 16u
/* XkbNoModifier. */
#تعریف_کړه NO_MODIFIER 0xff
/* XkbNumIndicators. */
#تعریف_کړه عدد_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#تعریف_کړه ALL_INDICATORS_ماسک 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#تعریف_کړه FAIL_UNLESS(بیان) افعل {                              \
    إذا (!(بیان)) {                                          \
        log_err(keymap->ctx, XKB_LOG_پیغام_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(بیان));                 \
        اذهب fail;                                          \
    }                                                       \
} طالما (0)

#تعریف_کړه FAIL_IF_BAD_ځواب(ځواب, request_name) افعل {         \
    إذا (!(ځواب)) {                                           \
        log_err(keymap->ctx, XKB_LOG_پیغام_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        اذهب fail;                                          \
    }                                                       \
} طالما (0)

#تعریف_کړه ALLOC_OR_FAIL(arr, nmemb) افعل {                      \
    إذا ((nmemb) > 0) {                                      \
        (arr) = حافظه_صفر_ورکړه((nmemb), حجم(*(arr)));            \
        إذا (!(arr))                                         \
            اذهب fail;                                      \
    }                                                       \
} طالما (0)

ساكن ثابت xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_کلی_TYPES |
     XCB_XKB_MAP_PART_کلی_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_کلی_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

ساكن ثابت xcb_xkb_name_detail_t get_نومونه_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_کلی_TYPE_نومونه |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_نومونه |
     XCB_XKB_NAME_DETAIL_INDICATOR_نومونه |
     XCB_XKB_NAME_DETAIL_کلی_نومونه |
     XCB_XKB_NAME_DETAIL_کلی_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_نومونه |
     XCB_XKB_NAME_DETAIL_ډله_نومونه);
ساكن ثابت xcb_xkb_name_detail_t get_نومونه_required =
    (XCB_XKB_NAME_DETAIL_کلی_TYPE_نومونه |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_نومونه |
     XCB_XKB_NAME_DETAIL_کلی_نومونه |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_نومونه);


ساكن xkb_mod_ماسک_t
translate_mods(uint8_t rmods, uint16_t vmods_ټیټ, uint16_t vmods_لوړ)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    أرجع
        ((xkb_mod_ماسک_t) rmods) |
        ((xkb_mod_ماسک_t) vmods_ټیټ << 8) |
        ((xkb_mod_ماسک_t) vmods_لوړ << 16);
}

تعداد xkb_action_controls
translate_controls_ماسک(uint32_t wire)
{
    تعداد xkb_action_controls ret = 0;
    إذا (wire & XCB_XKB_BOOL_CTRL_REPEAT_کلیګانې)
        ret |= CONTROL_REPEAT;
    إذا (wire & XCB_XKB_BOOL_CTRL_SLOW_کلیګانې)
        ret |= CONTROL_SLOW;
    إذا (wire & XCB_XKB_BOOL_CTRL_BOUNCE_کلیګانې)
        ret |= CONTROL_DEBOUNCE;
    إذا (wire & XCB_XKB_BOOL_CTRL_STICKY_کلیګانې)
        ret |= CONTROL_STICKY_کلیګانې;
    إذا (wire & XCB_XKB_BOOL_CTRL_MOUSE_کلیګانې)
        ret |= CONTROL_MOUSE_کلیګانې;
    إذا (wire & XCB_XKB_BOOL_CTRL_MOUSE_کلیګانې_ACCEL)
        ret |= CONTROL_MOUSE_کلیګانې_ACCEL;
    إذا (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_کلیګانې)
        ret |= CONTROL_AX;
    إذا (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_ماسک)
        ret |= CONTROL_AX_TIMEOUT;
    إذا (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_ماسک)
        ret |= CONTROL_AX_FEEDBACK;
    إذا (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_ماسک)
        ret |= CONTROL_BELL;
    إذا (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_ماسک)
        ret |= CONTROL_OVERLAY1;
    إذا (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_ماسک)
        ret |= CONTROL_OVERLAY2;
    إذا (wire & XCB_XKB_BOOL_CTRL_IGNORE_ډله_قلف_ماسک)
        ret |= CONTROL_IGNORE_ډله_قلف;
    /* Some controls are not supported and don't appear here. */
    أرجع ret;
}

ساكن bool
translate_action(اتحاد xkb_action *action, ثابت xcb_xkb_action_t *wire,
                 xkb_keycode_t min_کلی_کوډ, xkb_keycode_t max_کلی_کوډ)
{
    بدل (wire->type) {
    حالة XCB_XKB_SA_TYPE_سېټ_MODS:
        action->type = ACTION_TYPE_MOD_سېټ;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_ټیټ_2,
                                                wire->setmods.vmods_لوړ_2);
        action->mods.mods.ماسک = translate_mods(wire->setmods.ماسک, 0, 0);

        إذا (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_قلف_CLEAR;
        إذا (wire->setmods.flags & XCB_XKB_SA_LATCH_منزل_قلف)
            action->mods.flags |= ACTION_LATCH_منزل_قلف;
        إذا (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_ټیټ_2,
                                                wire->latchmods.vmods_لوړ_2);
        action->mods.mods.ماسک = translate_mods(wire->latchmods.ماسک, 0, 0);

        إذا (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_قلف_CLEAR;
        إذا (wire->latchmods.flags & XCB_XKB_SA_LATCH_منزل_قلف)
            action->mods.flags |= ACTION_LATCH_منزل_قلف;
        إذا (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_قلف_MODS:
        action->type = ACTION_TYPE_MOD_قلف;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_ټیټ_2,
                                                wire->lockmods.vmods_لوړ_2);
        action->mods.mods.ماسک = translate_mods(wire->lockmods.ماسک, 0, 0);

        إذا (wire->lockmods.flags & XCB_XKB_SA_ISO_قلف_FLAG_NO_قلف)
            action->mods.flags |= ACTION_قلف_NO_قلف;
        إذا (wire->lockmods.flags & XCB_XKB_SA_ISO_قلف_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_قلف_NO_UNLOCK;
        إذا (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_سېټ_ډله:
        action->type = ACTION_TYPE_ډله_سېټ;

        action->ډله.ډله = (int32_t)wire->setgroup.ډله;

        إذا (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->ډله.flags |= ACTION_قلف_CLEAR;
        إذا (wire->setmods.flags & XCB_XKB_SA_LATCH_منزل_قلف)
            action->ډله.flags |= ACTION_LATCH_منزل_قلف;
        إذا (wire->setmods.flags & XCB_XKB_SA_ISO_قلف_FLAG_ډله_ABSOLUTE)
            action->ډله.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_LATCH_ډله:
        action->type = ACTION_TYPE_ډله_LATCH;

        action->ډله.ډله = (int32_t)wire->latchgroup.ډله;

        إذا (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->ډله.flags |= ACTION_قلف_CLEAR;
        إذا (wire->latchmods.flags & XCB_XKB_SA_LATCH_منزل_قلف)
            action->ډله.flags |= ACTION_LATCH_منزل_قلف;
        إذا (wire->latchmods.flags & XCB_XKB_SA_ISO_قلف_FLAG_ډله_ABSOLUTE)
            action->ډله.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_قلف_ډله:
        action->type = ACTION_TYPE_ډله_قلف;

        action->ډله.ډله = (int32_t)wire->lockgroup.ډله;

        إذا (wire->lockgroup.flags & XCB_XKB_SA_ISO_قلف_FLAG_ډله_ABSOLUTE)
            action->ډله.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_ټیټ | ((uint16_t) wire->moveptr.x_لوړ << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_ټیټ | ((uint16_t) wire->moveptr.y_لوړ << 8));

        إذا (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        إذا (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        إذا (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        اكسر;
    حالة XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.شمېر = wire->ptrbtn.شمېر;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        اكسر;
    حالة XCB_XKB_SA_TYPE_قلف_PTR_BTN:
        action->type = ACTION_TYPE_PTR_قلف;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.شمېر = 0;

        إذا (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_قلف_FLAG_NO_قلف)
            action->btn.flags |= ACTION_قلف_NO_قلف;
        إذا (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_قلف_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_قلف_NO_UNLOCK;

        اكسر;
    حالة XCB_XKB_SA_TYPE_سېټ_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.ارزښت = wire->setptrdflt.ارزښت;

        إذا (wire->setptrdflt.flags & XCB_XKB_SA_سېټ_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
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
    حالة XCB_XKB_SA_TYPE_سېټ_CONTROLS:
        action->type = ACTION_TYPE_CTRL_سېټ;
        {
            ثابت uint16_t ماسک = (wire->setcontrols.bool_Ctrls_ټیټ |
                                   (wire->setcontrols.bool_Ctrls_لوړ << 8));
            action->ctrls.ctrls = translate_controls_ماسک(ماسک);
        }
        اكسر;
    حالة XCB_XKB_SA_TYPE_قلف_CONTROLS:
        action->type = ACTION_TYPE_CTRL_قلف;
        {
            ثابت uint16_t ماسک = (wire->lockcontrols.bool_Ctrls_ټیټ |
                                   (wire->lockcontrols.bool_Ctrls_لوړ << 8));
            action->ctrls.ctrls = translate_controls_ماسک(ماسک);
        }
        اكسر;

    حالة XCB_XKB_SA_TYPE_REDIRECT_کلی:
        action->type = ACTION_TYPE_REDIRECT_کلی;
        إذا (wire->redirect.newkey < min_کلی_کوډ ||
            wire->redirect.newkey > max_کلی_کوډ)
                أرجع false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.ماسک,
            wire->redirect.vmods_ماسک_ټیټ,
            wire->redirect.vmods_ماسک_لوړ
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_ټیټ_2,
                                               wire->redirect.vmods_لوړ_2);
        اكسر;

    حالة XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        اكسر;

    /* We don't support these. */
    حالة XCB_XKB_SA_TYPE_ISO_قلف:
    حالة XCB_XKB_SA_TYPE_ACTION_پیغام:
    حالة XCB_XKB_SA_TYPE_DEVICE_BTN:
    حالة XCB_XKB_SA_TYPE_قلف_DEVICE_BTN:
    حالة XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_دننی == 20 &&
                      ACTION_TYPE_دننی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");

        إذا (wire->type < ACTION_TYPE_شخصي) {
            action->type = ACTION_TYPE_NONE;
            اكسر;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(حجم(action->priv.معلومات) == 7 &&
                      حجم(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        حافظه_نقل(action->priv.معلومات, wire->noaction.pad0, 7);
        اكسر;
    }
    أرجع true;
}

ساكن bool
get_types(بنية xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_ځواب_t *ځواب, xcb_xkb_get_map_map_t *map)
{
    صحيح types_اوږدتیا = xcb_xkb_get_map_map_types_rtrn_اوږدتیا(ځواب, map);
    xcb_xkb_کلی_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(ځواب, map);

    FAIL_UNLESS(ځواب->لومړی_Type == 0);

    keymap->عدد_types = ځواب->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->عدد_types);
    أكد(types_اوږدتیا == (صحيح) keymap->عدد_types);

    لكل (صحيح i = 0; i < types_اوږدتیا; i++) {
        xcb_xkb_کلی_type_t *wire_type = types_iter.معلومات;
        بنية xkb_کلی_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->عدد_پوړونه_2 > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.ماسک = translate_mods(wire_type->mods_ماسک, 0, 0);
        type->عدد_پوړونه = wire_type->عدد_پوړونه_2;

        {
            صحيح entries_اوږدتیا = xcb_xkb_کلی_type_map_اوږدتیا(wire_type);
            xcb_xkb_kt_map_ثبت_iterator_t entries_iter =
                xcb_xkb_کلی_type_map_iterator(wire_type);

            type->عدد_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->عدد_entries);
            أكد(entries_اوږدتیا == (صحيح) type->عدد_entries);

            لكل (صحيح j = 0; j < entries_اوږدتیا; j++) {
                xcb_xkb_kt_map_ثبت_t *wire_ثبت = entries_iter.معلومات;
                بنية xkb_کلی_type_ثبت *ثبت = &type->entries[j];

                FAIL_UNLESS(wire_ثبت->level < type->عدد_پوړونه);

                ثبت->level = wire_ثبت->level;
                ثبت->mods.mods = translate_mods(wire_ثبت->mods_mods,
                                                  wire_ثبت->mods_vmods, 0);
                ثبت->mods.ماسک = translate_mods(wire_ثبت->mods_ماسک, 0, 0);

                xcb_xkb_kt_map_ثبت_بل(&entries_iter);
            }
        }

        {
            صحيح preserves_اوږدتیا = xcb_xkb_کلی_type_preserve_اوږدتیا(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_کلی_type_preserve_iterator(wire_type);

            FAIL_UNLESS((بینښه) preserves_اوږدتیا <= type->عدد_entries);

            لكل (صحيح j = 0; j < preserves_اوږدتیا; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.معلومات;
                بنية xkb_کلی_type_ثبت *ثبت = &type->entries[j];

                ثبت->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                ثبت->preserve.ماسک = translate_mods(wire_preserve->ماسک, 0, 0);

                xcb_xkb_mod_def_بل(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_کلی_type_بل(&types_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_sym_maps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_ځواب_t *ځواب, xcb_xkb_get_map_map_t *map)
{
    صحيح sym_maps_اوږدتیا = xcb_xkb_get_map_map_syms_rtrn_اوږدتیا(ځواب, map);
    xcb_xkb_کلی_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(ځواب, map);

    FAIL_UNLESS(ځواب->min_کلی_کوډ_2 <= ځواب->max_کلی_کوډ_2);
    FAIL_UNLESS(ځواب->لومړی_کلی_Sym >= ځواب->min_کلی_کوډ_2);
    FAIL_UNLESS(ځواب->لومړی_کلی_Sym + ځواب->n_کلی_Syms <= ځواب->max_کلی_کوډ_2 + 1);

    keymap->min_کلی_کوډ = ځواب->min_کلی_کوډ_2;
    keymap->max_کلی_کوډ = ځواب->max_کلی_کوډ_2;
    keymap->عدد_کلیګانې = keymap->عدد_کلیګانې_ټیټ = ځواب->max_کلی_کوډ_2 + 1;

    ALLOC_OR_FAIL(keymap->کلیګانې, keymap->max_کلی_کوډ + 1);

    لكل (xkb_keycode_t kc = keymap->min_کلی_کوډ; kc <= keymap->max_کلی_کوډ; kc++)
        keymap->کلیګانې[kc].keycode = kc;

    لكل (صحيح i = 0; i < sym_maps_اوږدتیا; i++) {
        xcb_xkb_کلی_sym_map_t *wire_sym_map = sym_maps_iter.معلومات;
        بنية xkb_کلی *کلی = &keymap->کلیګانې[ځواب->لومړی_کلی_Sym + i];

        کلی->عدد_groups = wire_sym_map->ډله_Info & 0x0f;
        FAIL_UNLESS(کلی->عدد_groups <= ARRAY_اندازه(wire_sym_map->kt_شاخص));
        ALLOC_OR_FAIL(کلی->groups, کلی->عدد_groups);

        لكل (xkb_layout_شاخص_t j = 0; j < کلی->عدد_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_شاخص[j] < keymap->عدد_types);
            کلی->groups[j].type = &keymap->types[wire_sym_map->kt_شاخص[j]];

            ALLOC_OR_FAIL(کلی->groups[j].پوړونه, کلی->groups[j].type->عدد_پوړونه);
        }

        کلی->بهر_د_range_ډله_شمېره = (wire_sym_map->ډله_Info & 0x30) >> 4;

        FAIL_UNLESS(کلی->بهر_د_range_ډله_شمېره <= کلی->عدد_groups);

        إذا (wire_sym_map->ډله_Info & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            کلی->بهر_د_range_ډله_policy = XKB_LAYOUT_بهر_د_RANGE_CLAMP;
        وإلا إذا (wire_sym_map->ډله_Info & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            کلی->بهر_د_range_ډله_policy = XKB_LAYOUT_بهر_د_RANGE_REDIRECT;
        وإلا
            کلی->بهر_د_range_ډله_policy = XKB_LAYOUT_بهر_د_RANGE_WRAP;

        {
            صحيح syms_اوږدتیا = xcb_xkb_کلی_sym_map_syms_اوږدتیا(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_کلی_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((بینښه) syms_اوږدتیا == wire_sym_map->پلنوالی * کلی->عدد_groups);

            إذا (syms_اوږدتیا > 0)
                کلی->explicit |= EXPLICIT_SYMBOLS;

            لكل (xkb_layout_شاخص_t ډله = 0; ډله < کلی->عدد_groups; ډله++) {
                لكل (xkb_level_شاخص_t level = 0; level < wire_sym_map->پلنوالی; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    أكد(کلی->groups[ډله].type != عدم);
                    إذا (level < کلی->groups[ډله].type->عدد_پوړونه) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        کلی->groups[ډله].پوړونه[level].عدد_syms = 1;
                        کلی->groups[ډله].پوړونه[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        کلی->groups[ډله].پوړونه[level].پاسنی =
                            xkb_keysym_منزل_پاسنی(wire_keysym);
                        إذا (wire_keysym != XKB_کلی_No_نښه)
                            کلی->groups[ډله].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_کلی_sym_map_بل(&sym_maps_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_actions(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_ځواب_t *ځواب, xcb_xkb_get_map_map_t *map)
{
    صحيح acts_شمېر_اوږدتیا =
        xcb_xkb_get_map_map_acts_rtrn_شمېر_اوږدتیا(ځواب, map);
    uint8_t *acts_شمېر_iter = xcb_xkb_get_map_map_acts_rtrn_شمېر(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(ځواب, map);
    xcb_xkb_کلی_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(ځواب, map);

    FAIL_UNLESS(ځواب->لومړی_کلی_Action == keymap->min_کلی_کوډ);
    FAIL_UNLESS(ځواب->لومړی_کلی_Action + ځواب->n_کلی_Actions ==
                keymap->max_کلی_کوډ + 1);

    لكل (صحيح i = 0; i < acts_شمېر_اوږدتیا; i++) {
        xcb_xkb_کلی_sym_map_t *wire_sym_map = sym_maps_iter.معلومات;
        صحيح syms_اوږدتیا = xcb_xkb_کلی_sym_map_syms_اوږدتیا(wire_sym_map);
        uint8_t wire_شمېر = *acts_شمېر_iter;
        بنية xkb_کلی *کلی = &keymap->کلیګانې[ځواب->لومړی_کلی_Action + i];

        FAIL_UNLESS((بینښه) syms_اوږدتیا == wire_sym_map->پلنوالی * کلی->عدد_groups);
        FAIL_UNLESS(wire_شمېر == 0 || wire_شمېر == syms_اوږدتیا);

        ثابت xkb_keycode_t min_کلی_کوډ = keymap->min_کلی_کوډ;
        ثابت xkb_keycode_t max_کلی_کوډ = keymap->max_کلی_کوډ;

        إذا (wire_شمېر != 0) {
            لكل (xkb_layout_شاخص_t ډله = 0; ډله < کلی->عدد_groups; ډله++) {
                لكل (xkb_level_شاخص_t level = 0; level < wire_sym_map->پلنوالی; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.معلومات;

                    إذا (level < کلی->groups[ډله].type->عدد_پوړونه) {
                        کلی->groups[ډله].پوړونه[level].عدد_actions = 1;
                        اتحاد xkb_action *action =
                            &کلی->groups[ډله].پوړونه[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_کلی_کوډ, max_کلی_کوډ));

                        إذا (action->type != ACTION_TYPE_NONE) {
                            کلی->groups[ډله].implicit_actions =
                                !(کلی->explicit & EXPLICIT_INTERP);
                        } وإلا إذا (کلی->groups[ډله].پوړونه[level].s.sym ==
                                   XKB_کلی_No_نښه) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            کلی->groups[ډله].پوړونه[level].عدد_syms = 0;
                            کلی->groups[ډله].پوړونه[level].عدد_actions = 0;
                        }
                    }

                    xcb_xkb_action_بل(&acts_iter);
                }

                إذا (کلی->groups[ډله].implicit_actions)
                    کلی->implicit_actions = true;
            }
        }

        acts_شمېر_iter++;
        xcb_xkb_کلی_sym_map_بل(&sym_maps_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_vmods(بنية xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_ځواب_t *ځواب, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.عدد_mods =
        عدد_REAL_MODS + MIN(msb_pos(ځواب->virtualMods), عدد_VMODS);
    FAIL_UNLESS(keymap->mods.عدد_mods <= XKB_MAX_MODS);

    static_assert(عدد_REAL_MODS + عدد_VMODS <= XKB_MAX_MODS, "");
    لكل (بینښه i = 0; i < عدد_VMODS; i++) {
        إذا (ځواب->virtualMods & (1u << i)) {
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
              xcb_xkb_get_map_ځواب_t *ځواب, xcb_xkb_get_map_map_t *map)
{
    صحيح اوږدتیا = xcb_xkb_get_map_map_explicit_rtrn_اوږدتیا(ځواب, map);
    xcb_xkb_سېټ_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(ځواب, map);

    لكل (صحيح i = 0; i < اوږدتیا; i++) {
        xcb_xkb_سېټ_explicit_t *wire = iter.معلومات;
        بنية xkb_کلی *کلی;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلی_کوډ &&
                    wire->keycode <= keymap->max_کلی_کوډ);

        کلی = &keymap->کلیګانې[wire->keycode];

        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلی_TYPE_1) &&
            کلی->عدد_groups > 0) {
            کلی->groups[0].explicit_type = true;
            کلی->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلی_TYPE_2) &&
            کلی->عدد_groups > 1) {
            کلی->groups[1].explicit_type = true;
            کلی->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلی_TYPE_3) &&
            کلی->عدد_groups > 2) {
            کلی->groups[2].explicit_type = true;
            کلی->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلی_TYPE_4) &&
            کلی->عدد_groups > 3) {
            کلی->groups[3].explicit_type = true;
            کلی->explicit |= EXPLICIT_TYPES;
        }
        إذا (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            کلی->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            لكل (xkb_layout_شاخص_t l = 0; l < کلی->عدد_groups; l++) {
                کلی->groups[l].explicit_actions = true;
            }
        }
        إذا (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            کلی->explicit |= EXPLICIT_REPEAT;
        إذا (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            کلی->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_سېټ_explicit_بل(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_modmaps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_ځواب_t *ځواب, xcb_xkb_get_map_map_t *map)
{
    صحيح اوږدتیا = xcb_xkb_get_map_map_modmap_rtrn_اوږدتیا(ځواب, map);
    xcb_xkb_کلی_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(ځواب, map);

    darray(Mod_Map_ثبت) modmaps = darray_new();

    لكل (صحيح i = 0; i < اوږدتیا; i++) {
        xcb_xkb_کلی_mod_map_t *wire = iter.معلومات;
        بنية xkb_کلی *کلی;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلی_کوډ &&
                    wire->keycode <= keymap->max_کلی_کوډ);

        کلی = &keymap->کلیګانې[wire->keycode];
        کلی->modmap = wire->mods;

        darray_append(modmaps, (Mod_Map_ثبت) {
            .کلی_کوډ = کلی->keycode,
            .mods = کلی->modmap,
            .have_نښه = false,
            .u.کلی_Name = کلی->name
        });

        xcb_xkb_کلی_mod_map_بل(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->عدد_modmaps);
    أرجع true;

fail:
    darray_free(modmaps);
    أرجع false;
}

ساكن bool
get_vmodmaps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_ځواب_t *ځواب, xcb_xkb_get_map_map_t *map)
{
    صحيح اوږدتیا = xcb_xkb_get_map_map_vmodmap_rtrn_اوږدتیا(ځواب, map);
    xcb_xkb_کلی_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(ځواب, map);

    لكل (صحيح i = 0; i < اوږدتیا; i++) {
        xcb_xkb_کلی_v_mod_map_t *wire = iter.معلومات;
        بنية xkb_کلی *کلی;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلی_کوډ &&
                    wire->keycode <= keymap->max_کلی_کوډ);

        کلی = &keymap->کلیګانې[wire->keycode];
        کلی->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_کلی_v_mod_map_بل(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_map(بنية xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_ځواب_t *ځواب = xcb_xkb_get_map_ځواب(conn, cookie, عدم);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_ځواب(ځواب, "XkbGetMap");

    إذا ((ځواب->present & get_map_required_components) != get_map_required_components)
        اذهب fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(ځواب),
                               ځواب->nTypes,
                               ځواب->n_کلی_Syms,
                               ځواب->n_کلی_Actions,
                               ځواب->ټول_Actions,
                               ځواب->ټول_کلی_Behaviors,
                               ځواب->virtualMods,
                               ځواب->ټول_کلی_Explicit,
                               ځواب->ټول_Mod_Map_کلیګانې,
                               ځواب->ټول_VMod_Map_کلیګانې,
                               ځواب->present,
                               &map);

    إذا (!get_types(keymap, conn, ځواب, &map) ||
        !get_sym_maps(keymap, conn, ځواب, &map) ||
        !get_actions(keymap, conn, ځواب, &map) ||
        !get_vmods(keymap, conn, ځواب, &map) ||
        !get_explicits(keymap, conn, ځواب, &map) ||
        !get_modmaps(keymap, conn, ځواب, &map) ||
        !get_vmodmaps(keymap, conn, ځواب, &map))
        اذهب fail;

    حرر(ځواب);
    أرجع true;

fail:
    حرر(ځواب);
    أرجع false;
}

ساكن bool
get_indicators(بنية xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_ځواب_t *ځواب)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(ځواب);

    keymap->عدد_leds = msb_pos(ځواب->which);
    FAIL_UNLESS(keymap->عدد_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == عدد_INDICATORS, "");
    لكل (بینښه i = 0; i < عدد_INDICATORS; i++) {
        إذا (ځواب->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.معلومات;
            بنية xkb_led *led = &keymap->leds[i];

            إذا (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_بنسټ)
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

            إذا (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_بنسټ)
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
            led->mods.ماسک = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_ماسک(wire->ctrls);

            xcb_xkb_indicator_map_بل(&iter);
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
    xcb_xkb_get_indicator_map_ځواب_t *ځواب =
        xcb_xkb_get_indicator_map_ځواب(conn, cookie, عدم);

    FAIL_IF_BAD_ځواب(ځواب, "XkbGetIndicatorMap");

    إذا (!get_indicators(keymap, conn, ځواب))
        اذهب fail;

    حرر(ځواب);
    أرجع true;

fail:
    حرر(ځواب);
    أرجع false;
}

ساكن bool
get_sym_interprets(بنية xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_ځواب_t *ځواب)
{
    صحيح اوږدتیا = xcb_xkb_get_compat_map_si_rtrn_اوږدتیا(ځواب);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(ځواب);

    FAIL_UNLESS(ځواب->لومړی_SIRtrn == 0);
    FAIL_UNLESS(ځواب->nSIRtrn == ځواب->n_ټول_SI);

    keymap->عدد_sym_interprets = ځواب->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->عدد_sym_interprets);

    ثابت xkb_keycode_t min_کلی_کوډ = keymap->min_کلی_کوډ;
    ثابت xkb_keycode_t max_کلی_کوډ = keymap->max_کلی_کوډ;

    لكل (صحيح i = 0; i < اوږدتیا; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.معلومات;
        بنية xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        بدل (wire->سمون & XCB_XKB_SYM_INTERP_سمون_OP_ماسک) {
        حالة XCB_XKB_SYM_INTERPRET_سمون_NONE_د:
            sym_interpret->سمون = سمون_NONE;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_سمون_ANY_د_OR_NONE:
            sym_interpret->سمون = سمون_ANY_OR_NONE;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_سمون_ANY_د:
            sym_interpret->سمون = سمون_ANY;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_سمون_ALL_د:
            sym_interpret->سمون = سمون_ALL;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_سمون_EXACTLY:
            sym_interpret->سمون = سمون_EXACTLY;
            اكسر;
        افتراضي:
            log_err_func(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->سمون & XCB_XKB_SYM_INTERP_سمون_OP_ماسک);
            اذهب fail;
        }

        sym_interpret->level_one_only =
            (wire->سمون & XCB_XKB_SYM_INTERP_سمون_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        إذا (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        وإلا
            sym_interpret->virtual_mod = عدد_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_کلی_کوډ, max_کلی_کوډ));
        sym_interpret->عدد_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_بل(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_compat_map(بنية xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_ځواب_t *ځواب =
        xcb_xkb_get_compat_map_ځواب(conn, cookie, عدم);

    FAIL_IF_BAD_ځواب(ځواب, "XkbGetCompatMap");

    إذا (!get_sym_interprets(keymap, conn, ځواب))
        اذهب fail;

    حرر(ځواب);
    أرجع true;

fail:
    حرر(ځواب);
    أرجع false;
}

ساكن bool
get_type_نومونه(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
               xcb_xkb_get_نومونه_ځواب_t *ځواب,
               xcb_xkb_get_نومونه_ارزښت_list_t *list)
{
    صحيح کلی_type_نومونه_اوږدتیا =
        xcb_xkb_get_نومونه_ارزښت_list_type_نومونه_اوږدتیا(ځواب, list);
    xcb_atom_t *کلی_type_نومونه_iter =
        xcb_xkb_get_نومونه_ارزښت_list_type_نومونه(list);
    صحيح n_پوړونه_فی_type_اوږدتیا =
        xcb_xkb_get_نومونه_ارزښت_list_n_پوړونه_فی_type_اوږدتیا(ځواب, list);
    uint8_t *n_پوړونه_فی_type_iter =
        xcb_xkb_get_نومونه_ارزښت_list_n_پوړونه_فی_type(list);
    xcb_atom_t *kt_level_نومونه_iter =
        xcb_xkb_get_نومونه_ارزښت_list_kt_level_نومونه(list);

    FAIL_UNLESS(ځواب->nTypes == keymap->عدد_types);
    FAIL_UNLESS(کلی_type_نومونه_اوږدتیا == n_پوړونه_فی_type_اوږدتیا);

    لكل (صحيح i = 0; i < کلی_type_نومونه_اوږدتیا; i++) {
        xcb_atom_t wire_type_name = *کلی_type_نومونه_iter;
        uint8_t wire_عدد_پوړونه = *n_پوړونه_فی_type_iter;
        بنية xkb_کلی_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->عدد_پوړونه >= wire_عدد_پوړونه);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_نومونه, type->عدد_پوړونه);
        type->عدد_level_نومونه = type->عدد_پوړونه;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        لكل (اندازه_t j = 0; j < wire_عدد_پوړونه; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_نومونه_iter[j],
                                         &type->level_نومونه[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        لكل (اندازه_t j = wire_عدد_پوړونه; j < type->عدد_پوړونه; j++)
            type->level_نومونه[j] = XKB_ATOM_NONE;

        kt_level_نومونه_iter += wire_عدد_پوړونه;
        کلی_type_نومونه_iter++;
        n_پوړونه_فی_type_iter++;
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_indicator_نومونه(بنية xkb_keymap *keymap,
                    بنية x11_atom_interner *interner,
                    xcb_xkb_get_نومونه_ځواب_t *ځواب,
                    xcb_xkb_get_نومونه_ارزښت_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_نومونه_ارزښت_list_indicator_نومونه(list);

    FAIL_UNLESS(msb_pos(ځواب->indicators) <= keymap->عدد_leds);

    لكل (بینښه i = 0; i < عدد_INDICATORS; i++) {
        إذا (ځواب->indicators & (UINT32_C(1) << i)) {
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
get_vmod_نومونه(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
               xcb_xkb_get_نومونه_ځواب_t *ځواب,
               xcb_xkb_get_نومونه_ارزښت_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_نومونه_ارزښت_list_virtual_mod_نومونه(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.عدد_mods =
        عدد_REAL_MODS + MIN(msb_pos(ځواب->virtualMods), عدد_VMODS);
    FAIL_UNLESS(keymap->mods.عدد_mods <= XKB_MAX_MODS);

    static_assert(عدد_REAL_MODS + عدد_VMODS <= XKB_MAX_MODS, "");
    لكل (بینښه i = 0; i < عدد_VMODS; i++) {
        إذا (ځواب->virtualMods & (1u << i)) {
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
get_ډله_نومونه(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
                xcb_xkb_get_نومونه_ځواب_t *ځواب,
                xcb_xkb_get_نومونه_ارزښت_list_t *list)
{
    صحيح اوږدتیا = xcb_xkb_get_نومونه_ارزښت_list_groups_اوږدتیا(ځواب, list);
    xcb_atom_t *iter = xcb_xkb_get_نومونه_ارزښت_list_groups(list);

    keymap->عدد_ډله_نومونه = msb_pos(ځواب->ډله_نومونه_2);
    ALLOC_OR_FAIL(keymap->ډله_نومونه, keymap->عدد_ډله_نومونه);

    لكل (صحيح i = 0; i < اوږدتیا; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->ډله_نومونه[i]);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_کلی_نومونه(بنية xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_نومونه_ځواب_t *ځواب,
              xcb_xkb_get_نومونه_ارزښت_list_t *list)
{
    صحيح اوږدتیا = xcb_xkb_get_نومونه_ارزښت_list_کلی_نومونه_اوږدتیا(ځواب, list);
    xcb_xkb_کلی_name_iterator_t iter =
        xcb_xkb_get_نومونه_ارزښت_list_کلی_نومونه_iterator(ځواب, list);

    FAIL_UNLESS(ځواب->min_کلی_کوډ_2 == keymap->min_کلی_کوډ);
    FAIL_UNLESS(ځواب->max_کلی_کوډ_2 == keymap->max_کلی_کوډ);
    FAIL_UNLESS(ځواب->لومړی_کلی == keymap->min_کلی_کوډ);
    FAIL_UNLESS(ځواب->لومړی_کلی + ځواب->n_کلیګانې - 1U == keymap->max_کلی_کوډ);

    لكل (صحيح i = 0; i < اوږدتیا; i++) {
        xcb_xkb_کلی_name_t *wire = iter.معلومات;
        xkb_atom_t *کلی_name = &keymap->کلیګانې[ځواب->لومړی_کلی + i].name;

        إذا (wire->name[0] == '\0') {
            *کلی_name = XKB_ATOM_NONE;
        }
        وإلا {
            *کلی_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_کلی_NAME_اوږدتیا));
            إذا (!*کلی_name)
                أرجع false;
        }

        xcb_xkb_کلی_name_بل(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_aliases(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_نومونه_ځواب_t *ځواب,
            xcb_xkb_get_نومونه_ارزښت_list_t *list)
{
    صحيح اوږدتیا = xcb_xkb_get_نومونه_ارزښت_list_کلی_aliases_اوږدتیا(ځواب, list);
    xcb_xkb_کلی_alias_iterator_t iter =
        xcb_xkb_get_نومونه_ارزښت_list_کلی_aliases_iterator(ځواب, list);

    keymap->عدد_کلی_aliases = ځواب->n_کلی_Aliases;
    ALLOC_OR_FAIL(keymap->کلی_aliases, keymap->عدد_کلی_aliases);

    لكل (صحيح i = 0; i < اوږدتیا; i++) {
        xcb_xkb_کلی_alias_t *wire = iter.معلومات;
        بنية xkb_کلی_alias *alias = &keymap->کلی_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_کلی_NAME_اوږدتیا));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_کلی_NAME_اوږدتیا));
        إذا (!alias->real || !alias->alias)
            اذهب fail;

        xcb_xkb_کلی_alias_بل(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_نومونه(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
          xcb_xkb_get_نومونه_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_نومونه_ځواب_t *ځواب =
        xcb_xkb_get_نومونه_ځواب(conn, cookie, عدم);
    xcb_xkb_get_نومونه_ارزښت_list_t list;

    FAIL_IF_BAD_ځواب(ځواب, "XkbGetNames");

    FAIL_UNLESS((ځواب->which & get_نومونه_required) == get_نومونه_required);

    xcb_xkb_get_نومونه_ارزښت_list_unpack(xcb_xkb_get_نومونه_ارزښت_list(ځواب),
                                        ځواب->nTypes,
                                        ځواب->indicators,
                                        ځواب->virtualMods,
                                        ځواب->ډله_نومونه_2,
                                        ځواب->n_کلیګانې,
                                        ځواب->n_کلی_Aliases,
                                        ځواب->nRadioGroups,
                                        ځواب->which,
                                        &list);

    x11_atom_interner_get_escaped_atom_name(interner, list.keycodesName,
                                            &keymap->keycodes_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.symbolsName,
                                            &keymap->symbols_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.typesName,
                                            &keymap->types_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.compatName,
                                            &keymap->compat_section_name);
    إذا (!get_type_نومونه(keymap, interner, ځواب, &list) ||
        !get_indicator_نومونه(keymap, interner, ځواب, &list) ||
        !get_vmod_نومونه(keymap, interner, ځواب, &list) ||
        !get_ډله_نومونه(keymap, interner, ځواب, &list) ||
        !get_کلی_نومونه(keymap, conn, ځواب, &list) ||
        !get_aliases(keymap, conn, ځواب, &list))
        اذهب fail;

    حرر(ځواب);
    أرجع true;

fail:
    حرر(ځواب);
    أرجع false;
}

ساكن bool
get_controls(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_ځواب_t *ځواب =
        xcb_xkb_get_controls_ځواب(conn, cookie, عدم);

    FAIL_IF_BAD_ځواب(ځواب, "XkbGetControls");
    FAIL_UNLESS(ځواب->عدد_Groups > 0 && ځواب->عدد_Groups <= 4);

    keymap->عدد_groups = ځواب->عدد_Groups;

    FAIL_UNLESS(keymap->max_کلی_کوډ < XCB_XKB_CONST_فی_کلی_بټ_ARRAY_اندازه * 8);

    لكل (xkb_keycode_t i = keymap->min_کلی_کوډ; i <= keymap->max_کلی_کوډ; i++)
        keymap->کلیګانې[i].repeats = (ځواب->فی_کلی_Repeat[i / 8] & (1u << (i % 8)));

    حرر(ځواب);
    أرجع true;

fail:
    حرر(ځواب);
    أرجع false;
}

بنية xkb_keymap *
xkb_x11_keymap_new_from_device(بنية xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               تعداد xkb_keymap_compile_flags flags)
{
    إذا (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_پیغام_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        أرجع عدم;
    }

    ثابت تعداد xkb_keymap_format format = XKB_KEYMAP_FORMAT_متن_V1;
    بنية xkb_keymap * ثابت keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    إذا (!keymap)
        أرجع عدم;

    keymap->redirect_کلی_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

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
        xcb_xkb_get_indicator_map(conn, device_id, ALL_INDICATORS_ماسک);
    xcb_xkb_get_compat_map_cookie_t compat_map_cookie =
        xcb_xkb_get_compat_map(conn, device_id, 0, true, 0, 0);
    xcb_xkb_get_نومونه_cookie_t نومونه_cookie =
        xcb_xkb_get_نومونه(conn, device_id, get_نومونه_wanted);
    xcb_xkb_get_controls_cookie_t controls_cookie =
        xcb_xkb_get_controls(conn, device_id);

    إذا (!get_map(keymap, conn, map_cookie))
        اذهب err_map;
    إذا (!get_indicator_map(keymap, conn, indicator_map_cookie))
        اذهب err_indicator_map;
    إذا (!get_compat_map(keymap, conn, compat_map_cookie))
        اذهب err_compat_map;
    إذا (!get_نومونه(keymap, &interner, نومونه_cookie))
        اذهب err_نومونه;
    إذا (!get_controls(keymap, conn, controls_cookie))
        اذهب err_controls;
    x11_atom_interner_پړاو_trip(&interner);
    إذا (interner.had_error)
        اذهب err_interner;

    أرجع keymap;

err_map:
    xcb_discard_ځواب(conn, indicator_map_cookie.sequence);
err_indicator_map:
    xcb_discard_ځواب(conn, compat_map_cookie.sequence);
err_compat_map:
    xcb_discard_ځواب(conn, نومونه_cookie.sequence);
err_نومونه:
    xcb_discard_ځواب(conn, controls_cookie.sequence);
err_controls:
    x11_atom_interner_پړاو_trip(&interner);
err_interner:
    xkb_keymap_unref(keymap);
    أرجع عدم;
}
