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
#تعریف_کن رقم_REAL_MODS 8u
/* XkbNumVirtualMods. */
#تعریف_کن رقم_VMODS 16u
/* XkbNoModifier. */
#تعریف_کن NO_MODIFIER 0xff
/* XkbNumIndicators. */
#تعریف_کن رقم_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#تعریف_کن ALL_INDICATORS_ماسک 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#تعریف_کن FAIL_UNLESS(عبارت) افعل {                              \
    إذا (!(عبارت)) {                                          \
        log_err(keymap->ctx, XKB_LOG_پیام_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(عبارت));                 \
        اذهب fail;                                          \
    }                                                       \
} طالما (0)

#تعریف_کن FAIL_IF_BAD_پاسخ(پاسخ, request_name) افعل {         \
    إذا (!(پاسخ)) {                                           \
        log_err(keymap->ctx, XKB_LOG_پیام_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        اذهب fail;                                          \
    }                                                       \
} طالما (0)

#تعریف_کن ALLOC_OR_FAIL(arr, nmemb) افعل {                      \
    إذا ((nmemb) > 0) {                                      \
        (arr) = حافظه_صفر_بده((nmemb), حجم(*(arr)));            \
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

ساكن ثابت xcb_xkb_name_detail_t get_نامها_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_کلید_TYPE_نامها |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_نامها |
     XCB_XKB_NAME_DETAIL_INDICATOR_نامها |
     XCB_XKB_NAME_DETAIL_کلید_نامها |
     XCB_XKB_NAME_DETAIL_کلید_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_نامها |
     XCB_XKB_NAME_DETAIL_گروه_نامها);
ساكن ثابت xcb_xkb_name_detail_t get_نامها_required =
    (XCB_XKB_NAME_DETAIL_کلید_TYPE_نامها |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_نامها |
     XCB_XKB_NAME_DETAIL_کلید_نامها |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_نامها);


ساكن xkb_mod_ماسک_t
translate_mods(uint8_t rmods, uint16_t vmods_کم, uint16_t vmods_بالا)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    أرجع
        ((xkb_mod_ماسک_t) rmods) |
        ((xkb_mod_ماسک_t) vmods_کم << 8) |
        ((xkb_mod_ماسک_t) vmods_بالا << 16);
}

تعداد xkb_action_controls
translate_controls_ماسک(uint32_t wire)
{
    تعداد xkb_action_controls ret = 0;
    إذا (wire & XCB_XKB_BOOL_CTRL_REPEAT_کلیدها)
        ret |= CONTROL_REPEAT;
    إذا (wire & XCB_XKB_BOOL_CTRL_SLOW_کلیدها)
        ret |= CONTROL_SLOW;
    إذا (wire & XCB_XKB_BOOL_CTRL_BOUNCE_کلیدها)
        ret |= CONTROL_DEBOUNCE;
    إذا (wire & XCB_XKB_BOOL_CTRL_STICKY_کلیدها)
        ret |= CONTROL_STICKY_کلیدها;
    إذا (wire & XCB_XKB_BOOL_CTRL_MOUSE_کلیدها)
        ret |= CONTROL_MOUSE_کلیدها;
    إذا (wire & XCB_XKB_BOOL_CTRL_MOUSE_کلیدها_ACCEL)
        ret |= CONTROL_MOUSE_کلیدها_ACCEL;
    إذا (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_کلیدها)
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
    إذا (wire & XCB_XKB_BOOL_CTRL_IGNORE_گروه_قفل_ماسک)
        ret |= CONTROL_IGNORE_گروه_قفل;
    /* Some controls are not supported and don't appear here. */
    أرجع ret;
}

ساكن bool
translate_action(اتحاد xkb_action *action, ثابت xcb_xkb_action_t *wire,
                 xkb_keycode_t min_کلید_کد, xkb_keycode_t max_کلید_کد)
{
    بدل (wire->type) {
    حالة XCB_XKB_SA_TYPE_مجموعه_MODS:
        action->type = ACTION_TYPE_MOD_مجموعه;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_کم_2,
                                                wire->setmods.vmods_بالا_2);
        action->mods.mods.ماسک = translate_mods(wire->setmods.ماسک, 0, 0);

        إذا (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_قفل_CLEAR;
        إذا (wire->setmods.flags & XCB_XKB_SA_LATCH_مقصد_قفل)
            action->mods.flags |= ACTION_LATCH_مقصد_قفل;
        إذا (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_کم_2,
                                                wire->latchmods.vmods_بالا_2);
        action->mods.mods.ماسک = translate_mods(wire->latchmods.ماسک, 0, 0);

        إذا (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_قفل_CLEAR;
        إذا (wire->latchmods.flags & XCB_XKB_SA_LATCH_مقصد_قفل)
            action->mods.flags |= ACTION_LATCH_مقصد_قفل;
        إذا (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_قفل_MODS:
        action->type = ACTION_TYPE_MOD_قفل;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_کم_2,
                                                wire->lockmods.vmods_بالا_2);
        action->mods.mods.ماسک = translate_mods(wire->lockmods.ماسک, 0, 0);

        إذا (wire->lockmods.flags & XCB_XKB_SA_ISO_قفل_FLAG_NO_قفل)
            action->mods.flags |= ACTION_قفل_NO_قفل;
        إذا (wire->lockmods.flags & XCB_XKB_SA_ISO_قفل_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_قفل_NO_UNLOCK;
        إذا (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        اكسر;
    حالة XCB_XKB_SA_TYPE_مجموعه_گروه:
        action->type = ACTION_TYPE_گروه_مجموعه;

        action->گروه.گروه = (int32_t)wire->setgroup.گروه;

        إذا (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->گروه.flags |= ACTION_قفل_CLEAR;
        إذا (wire->setmods.flags & XCB_XKB_SA_LATCH_مقصد_قفل)
            action->گروه.flags |= ACTION_LATCH_مقصد_قفل;
        إذا (wire->setmods.flags & XCB_XKB_SA_ISO_قفل_FLAG_گروه_ABSOLUTE)
            action->گروه.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_LATCH_گروه:
        action->type = ACTION_TYPE_گروه_LATCH;

        action->گروه.گروه = (int32_t)wire->latchgroup.گروه;

        إذا (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->گروه.flags |= ACTION_قفل_CLEAR;
        إذا (wire->latchmods.flags & XCB_XKB_SA_LATCH_مقصد_قفل)
            action->گروه.flags |= ACTION_LATCH_مقصد_قفل;
        إذا (wire->latchmods.flags & XCB_XKB_SA_ISO_قفل_FLAG_گروه_ABSOLUTE)
            action->گروه.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_قفل_گروه:
        action->type = ACTION_TYPE_گروه_قفل;

        action->گروه.گروه = (int32_t)wire->lockgroup.گروه;

        إذا (wire->lockgroup.flags & XCB_XKB_SA_ISO_قفل_FLAG_گروه_ABSOLUTE)
            action->گروه.flags |= ACTION_ABSOLUTE_SWITCH;

        اكسر;
    حالة XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_کم | ((uint16_t) wire->moveptr.x_بالا << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_کم | ((uint16_t) wire->moveptr.y_بالا << 8));

        إذا (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        إذا (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        إذا (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        اكسر;
    حالة XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.شمار = wire->ptrbtn.شمار;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        اكسر;
    حالة XCB_XKB_SA_TYPE_قفل_PTR_BTN:
        action->type = ACTION_TYPE_PTR_قفل;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.شمار = 0;

        إذا (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_قفل_FLAG_NO_قفل)
            action->btn.flags |= ACTION_قفل_NO_قفل;
        إذا (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_قفل_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_قفل_NO_UNLOCK;

        اكسر;
    حالة XCB_XKB_SA_TYPE_مجموعه_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.مقدار = wire->setptrdflt.مقدار;

        إذا (wire->setptrdflt.flags & XCB_XKB_SA_مجموعه_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
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
    حالة XCB_XKB_SA_TYPE_مجموعه_CONTROLS:
        action->type = ACTION_TYPE_CTRL_مجموعه;
        {
            ثابت uint16_t ماسک = (wire->setcontrols.bool_Ctrls_کم |
                                   (wire->setcontrols.bool_Ctrls_بالا << 8));
            action->ctrls.ctrls = translate_controls_ماسک(ماسک);
        }
        اكسر;
    حالة XCB_XKB_SA_TYPE_قفل_CONTROLS:
        action->type = ACTION_TYPE_CTRL_قفل;
        {
            ثابت uint16_t ماسک = (wire->lockcontrols.bool_Ctrls_کم |
                                   (wire->lockcontrols.bool_Ctrls_بالا << 8));
            action->ctrls.ctrls = translate_controls_ماسک(ماسک);
        }
        اكسر;

    حالة XCB_XKB_SA_TYPE_REDIRECT_کلید:
        action->type = ACTION_TYPE_REDIRECT_کلید;
        إذا (wire->redirect.newkey < min_کلید_کد ||
            wire->redirect.newkey > max_کلید_کد)
                أرجع false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.ماسک,
            wire->redirect.vmods_ماسک_کم,
            wire->redirect.vmods_ماسک_بالا
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_کم_2,
                                               wire->redirect.vmods_بالا_2);
        اكسر;

    حالة XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        اكسر;

    /* We don't support these. */
    حالة XCB_XKB_SA_TYPE_ISO_قفل:
    حالة XCB_XKB_SA_TYPE_ACTION_پیام:
    حالة XCB_XKB_SA_TYPE_DEVICE_BTN:
    حالة XCB_XKB_SA_TYPE_قفل_DEVICE_BTN:
    حالة XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_داخلی == 20 &&
                      ACTION_TYPE_داخلی + 1 == ACTION_TYPE_رقم_ENTRIES,
                      "Missing action type");

        إذا (wire->type < ACTION_TYPE_خصوصی) {
            action->type = ACTION_TYPE_NONE;
            اكسر;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(حجم(action->priv.داده) == 7 &&
                      حجم(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        کپی_حافظه(action->priv.داده, wire->noaction.pad0, 7);
        اكسر;
    }
    أرجع true;
}

ساكن bool
get_types(بنية xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_پاسخ_t *پاسخ, xcb_xkb_get_map_map_t *map)
{
    صحيح types_درازی = xcb_xkb_get_map_map_types_rtrn_درازی(پاسخ, map);
    xcb_xkb_کلید_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(پاسخ, map);

    FAIL_UNLESS(پاسخ->اولی_Type == 0);

    keymap->رقم_types = پاسخ->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->رقم_types);
    أكد(types_درازی == (صحيح) keymap->رقم_types);

    لكل (صحيح i = 0; i < types_درازی; i++) {
        xcb_xkb_کلید_type_t *wire_type = types_iter.داده;
        بنية xkb_کلید_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->رقم_سطوح_2 > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.ماسک = translate_mods(wire_type->mods_ماسک, 0, 0);
        type->رقم_سطوح = wire_type->رقم_سطوح_2;

        {
            صحيح entries_درازی = xcb_xkb_کلید_type_map_درازی(wire_type);
            xcb_xkb_kt_map_مدخل_iterator_t entries_iter =
                xcb_xkb_کلید_type_map_iterator(wire_type);

            type->رقم_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->رقم_entries);
            أكد(entries_درازی == (صحيح) type->رقم_entries);

            لكل (صحيح j = 0; j < entries_درازی; j++) {
                xcb_xkb_kt_map_مدخل_t *wire_مدخل = entries_iter.داده;
                بنية xkb_کلید_type_مدخل *مدخل = &type->entries[j];

                FAIL_UNLESS(wire_مدخل->level < type->رقم_سطوح);

                مدخل->level = wire_مدخل->level;
                مدخل->mods.mods = translate_mods(wire_مدخل->mods_mods,
                                                  wire_مدخل->mods_vmods, 0);
                مدخل->mods.ماسک = translate_mods(wire_مدخل->mods_ماسک, 0, 0);

                xcb_xkb_kt_map_مدخل_بعدی(&entries_iter);
            }
        }

        {
            صحيح preserves_درازی = xcb_xkb_کلید_type_preserve_درازی(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_کلید_type_preserve_iterator(wire_type);

            FAIL_UNLESS((بیعلامت) preserves_درازی <= type->رقم_entries);

            لكل (صحيح j = 0; j < preserves_درازی; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.داده;
                بنية xkb_کلید_type_مدخل *مدخل = &type->entries[j];

                مدخل->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                مدخل->preserve.ماسک = translate_mods(wire_preserve->ماسک, 0, 0);

                xcb_xkb_mod_def_بعدی(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_کلید_type_بعدی(&types_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_sym_maps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_پاسخ_t *پاسخ, xcb_xkb_get_map_map_t *map)
{
    صحيح sym_maps_درازی = xcb_xkb_get_map_map_syms_rtrn_درازی(پاسخ, map);
    xcb_xkb_کلید_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(پاسخ, map);

    FAIL_UNLESS(پاسخ->min_کلید_کد_2 <= پاسخ->max_کلید_کد_2);
    FAIL_UNLESS(پاسخ->اولی_کلید_Sym >= پاسخ->min_کلید_کد_2);
    FAIL_UNLESS(پاسخ->اولی_کلید_Sym + پاسخ->n_کلید_Syms <= پاسخ->max_کلید_کد_2 + 1);

    keymap->min_کلید_کد = پاسخ->min_کلید_کد_2;
    keymap->max_کلید_کد = پاسخ->max_کلید_کد_2;
    keymap->رقم_کلیدها = keymap->رقم_کلیدها_کم = پاسخ->max_کلید_کد_2 + 1;

    ALLOC_OR_FAIL(keymap->کلیدها, keymap->max_کلید_کد + 1);

    لكل (xkb_keycode_t kc = keymap->min_کلید_کد; kc <= keymap->max_کلید_کد; kc++)
        keymap->کلیدها[kc].keycode = kc;

    لكل (صحيح i = 0; i < sym_maps_درازی; i++) {
        xcb_xkb_کلید_sym_map_t *wire_sym_map = sym_maps_iter.داده;
        بنية xkb_کلید *کلید = &keymap->کلیدها[پاسخ->اولی_کلید_Sym + i];

        کلید->رقم_groups = wire_sym_map->گروه_Info & 0x0f;
        FAIL_UNLESS(کلید->رقم_groups <= ARRAY_اندازه(wire_sym_map->kt_شاخص));
        ALLOC_OR_FAIL(کلید->groups, کلید->رقم_groups);

        لكل (xkb_layout_شاخص_t j = 0; j < کلید->رقم_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_شاخص[j] < keymap->رقم_types);
            کلید->groups[j].type = &keymap->types[wire_sym_map->kt_شاخص[j]];

            ALLOC_OR_FAIL(کلید->groups[j].سطوح, کلید->groups[j].type->رقم_سطوح);
        }

        کلید->بیرون_از_range_گروه_عدد = (wire_sym_map->گروه_Info & 0x30) >> 4;

        FAIL_UNLESS(کلید->بیرون_از_range_گروه_عدد <= کلید->رقم_groups);

        إذا (wire_sym_map->گروه_Info & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            کلید->بیرون_از_range_گروه_policy = XKB_LAYOUT_بیرون_از_RANGE_CLAMP;
        وإلا إذا (wire_sym_map->گروه_Info & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            کلید->بیرون_از_range_گروه_policy = XKB_LAYOUT_بیرون_از_RANGE_REDIRECT;
        وإلا
            کلید->بیرون_از_range_گروه_policy = XKB_LAYOUT_بیرون_از_RANGE_WRAP;

        {
            صحيح syms_درازی = xcb_xkb_کلید_sym_map_syms_درازی(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_کلید_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((بیعلامت) syms_درازی == wire_sym_map->عرض * کلید->رقم_groups);

            إذا (syms_درازی > 0)
                کلید->explicit |= EXPLICIT_SYMBOLS;

            لكل (xkb_layout_شاخص_t گروه = 0; گروه < کلید->رقم_groups; گروه++) {
                لكل (xkb_level_شاخص_t level = 0; level < wire_sym_map->عرض; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    أكد(کلید->groups[گروه].type != عدم);
                    إذا (level < کلید->groups[گروه].type->رقم_سطوح) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        کلید->groups[گروه].سطوح[level].رقم_syms = 1;
                        کلید->groups[گروه].سطوح[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        کلید->groups[گروه].سطوح[level].فوقانی =
                            xkb_keysym_مقصد_فوقانی(wire_keysym);
                        إذا (wire_keysym != XKB_کلید_No_نماد)
                            کلید->groups[گروه].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_کلید_sym_map_بعدی(&sym_maps_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_actions(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_پاسخ_t *پاسخ, xcb_xkb_get_map_map_t *map)
{
    صحيح acts_شمار_درازی =
        xcb_xkb_get_map_map_acts_rtrn_شمار_درازی(پاسخ, map);
    uint8_t *acts_شمار_iter = xcb_xkb_get_map_map_acts_rtrn_شمار(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(پاسخ, map);
    xcb_xkb_کلید_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(پاسخ, map);

    FAIL_UNLESS(پاسخ->اولی_کلید_Action == keymap->min_کلید_کد);
    FAIL_UNLESS(پاسخ->اولی_کلید_Action + پاسخ->n_کلید_Actions ==
                keymap->max_کلید_کد + 1);

    لكل (صحيح i = 0; i < acts_شمار_درازی; i++) {
        xcb_xkb_کلید_sym_map_t *wire_sym_map = sym_maps_iter.داده;
        صحيح syms_درازی = xcb_xkb_کلید_sym_map_syms_درازی(wire_sym_map);
        uint8_t wire_شمار = *acts_شمار_iter;
        بنية xkb_کلید *کلید = &keymap->کلیدها[پاسخ->اولی_کلید_Action + i];

        FAIL_UNLESS((بیعلامت) syms_درازی == wire_sym_map->عرض * کلید->رقم_groups);
        FAIL_UNLESS(wire_شمار == 0 || wire_شمار == syms_درازی);

        ثابت xkb_keycode_t min_کلید_کد = keymap->min_کلید_کد;
        ثابت xkb_keycode_t max_کلید_کد = keymap->max_کلید_کد;

        إذا (wire_شمار != 0) {
            لكل (xkb_layout_شاخص_t گروه = 0; گروه < کلید->رقم_groups; گروه++) {
                لكل (xkb_level_شاخص_t level = 0; level < wire_sym_map->عرض; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.داده;

                    إذا (level < کلید->groups[گروه].type->رقم_سطوح) {
                        کلید->groups[گروه].سطوح[level].رقم_actions = 1;
                        اتحاد xkb_action *action =
                            &کلید->groups[گروه].سطوح[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_کلید_کد, max_کلید_کد));

                        إذا (action->type != ACTION_TYPE_NONE) {
                            کلید->groups[گروه].implicit_actions =
                                !(کلید->explicit & EXPLICIT_INTERP);
                        } وإلا إذا (کلید->groups[گروه].سطوح[level].s.sym ==
                                   XKB_کلید_No_نماد) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            کلید->groups[گروه].سطوح[level].رقم_syms = 0;
                            کلید->groups[گروه].سطوح[level].رقم_actions = 0;
                        }
                    }

                    xcb_xkb_action_بعدی(&acts_iter);
                }

                إذا (کلید->groups[گروه].implicit_actions)
                    کلید->implicit_actions = true;
            }
        }

        acts_شمار_iter++;
        xcb_xkb_کلید_sym_map_بعدی(&sym_maps_iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_vmods(بنية xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_پاسخ_t *پاسخ, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.رقم_mods =
        رقم_REAL_MODS + MIN(msb_pos(پاسخ->virtualMods), رقم_VMODS);
    FAIL_UNLESS(keymap->mods.رقم_mods <= XKB_MAX_MODS);

    static_assert(رقم_REAL_MODS + رقم_VMODS <= XKB_MAX_MODS, "");
    لكل (بیعلامت i = 0; i < رقم_VMODS; i++) {
        إذا (پاسخ->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            بنية xkb_mod *mod = &keymap->mods.mods[رقم_REAL_MODS + i];

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
              xcb_xkb_get_map_پاسخ_t *پاسخ, xcb_xkb_get_map_map_t *map)
{
    صحيح درازی = xcb_xkb_get_map_map_explicit_rtrn_درازی(پاسخ, map);
    xcb_xkb_مجموعه_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(پاسخ, map);

    لكل (صحيح i = 0; i < درازی; i++) {
        xcb_xkb_مجموعه_explicit_t *wire = iter.داده;
        بنية xkb_کلید *کلید;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلید_کد &&
                    wire->keycode <= keymap->max_کلید_کد);

        کلید = &keymap->کلیدها[wire->keycode];

        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_1) &&
            کلید->رقم_groups > 0) {
            کلید->groups[0].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_2) &&
            کلید->رقم_groups > 1) {
            کلید->groups[1].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_3) &&
            کلید->رقم_groups > 2) {
            کلید->groups[2].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        إذا ((wire->explicit & XCB_XKB_EXPLICIT_کلید_TYPE_4) &&
            کلید->رقم_groups > 3) {
            کلید->groups[3].explicit_type = true;
            کلید->explicit |= EXPLICIT_TYPES;
        }
        إذا (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            کلید->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            لكل (xkb_layout_شاخص_t l = 0; l < کلید->رقم_groups; l++) {
                کلید->groups[l].explicit_actions = true;
            }
        }
        إذا (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            کلید->explicit |= EXPLICIT_REPEAT;
        إذا (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            کلید->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_مجموعه_explicit_بعدی(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_modmaps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_پاسخ_t *پاسخ, xcb_xkb_get_map_map_t *map)
{
    صحيح درازی = xcb_xkb_get_map_map_modmap_rtrn_درازی(پاسخ, map);
    xcb_xkb_کلید_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(پاسخ, map);

    darray(Mod_Map_مدخل) modmaps = darray_new();

    لكل (صحيح i = 0; i < درازی; i++) {
        xcb_xkb_کلید_mod_map_t *wire = iter.داده;
        بنية xkb_کلید *کلید;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلید_کد &&
                    wire->keycode <= keymap->max_کلید_کد);

        کلید = &keymap->کلیدها[wire->keycode];
        کلید->modmap = wire->mods;

        darray_append(modmaps, (Mod_Map_مدخل) {
            .کلید_کد = کلید->keycode,
            .mods = کلید->modmap,
            .have_نماد = false,
            .u.کلید_Name = کلید->name
        });

        xcb_xkb_کلید_mod_map_بعدی(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->رقم_modmaps);
    أرجع true;

fail:
    darray_free(modmaps);
    أرجع false;
}

ساكن bool
get_vmodmaps(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_پاسخ_t *پاسخ, xcb_xkb_get_map_map_t *map)
{
    صحيح درازی = xcb_xkb_get_map_map_vmodmap_rtrn_درازی(پاسخ, map);
    xcb_xkb_کلید_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(پاسخ, map);

    لكل (صحيح i = 0; i < درازی; i++) {
        xcb_xkb_کلید_v_mod_map_t *wire = iter.داده;
        بنية xkb_کلید *کلید;

        FAIL_UNLESS(wire->keycode >= keymap->min_کلید_کد &&
                    wire->keycode <= keymap->max_کلید_کد);

        کلید = &keymap->کلیدها[wire->keycode];
        کلید->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_کلید_v_mod_map_بعدی(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_map(بنية xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_پاسخ_t *پاسخ = xcb_xkb_get_map_پاسخ(conn, cookie, عدم);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_پاسخ(پاسخ, "XkbGetMap");

    إذا ((پاسخ->present & get_map_required_components) != get_map_required_components)
        اذهب fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(پاسخ),
                               پاسخ->nTypes,
                               پاسخ->n_کلید_Syms,
                               پاسخ->n_کلید_Actions,
                               پاسخ->مجموع_Actions,
                               پاسخ->مجموع_کلید_Behaviors,
                               پاسخ->virtualMods,
                               پاسخ->مجموع_کلید_Explicit,
                               پاسخ->مجموع_Mod_Map_کلیدها,
                               پاسخ->مجموع_VMod_Map_کلیدها,
                               پاسخ->present,
                               &map);

    إذا (!get_types(keymap, conn, پاسخ, &map) ||
        !get_sym_maps(keymap, conn, پاسخ, &map) ||
        !get_actions(keymap, conn, پاسخ, &map) ||
        !get_vmods(keymap, conn, پاسخ, &map) ||
        !get_explicits(keymap, conn, پاسخ, &map) ||
        !get_modmaps(keymap, conn, پاسخ, &map) ||
        !get_vmodmaps(keymap, conn, پاسخ, &map))
        اذهب fail;

    حرر(پاسخ);
    أرجع true;

fail:
    حرر(پاسخ);
    أرجع false;
}

ساكن bool
get_indicators(بنية xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_پاسخ_t *پاسخ)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(پاسخ);

    keymap->رقم_leds = msb_pos(پاسخ->which);
    FAIL_UNLESS(keymap->رقم_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == رقم_INDICATORS, "");
    لكل (بیعلامت i = 0; i < رقم_INDICATORS; i++) {
        إذا (پاسخ->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.داده;
            بنية xkb_led *led = &keymap->leds[i];

            إذا (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_پایه)
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

            إذا (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_پایه)
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

            xcb_xkb_indicator_map_بعدی(&iter);
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
    xcb_xkb_get_indicator_map_پاسخ_t *پاسخ =
        xcb_xkb_get_indicator_map_پاسخ(conn, cookie, عدم);

    FAIL_IF_BAD_پاسخ(پاسخ, "XkbGetIndicatorMap");

    إذا (!get_indicators(keymap, conn, پاسخ))
        اذهب fail;

    حرر(پاسخ);
    أرجع true;

fail:
    حرر(پاسخ);
    أرجع false;
}

ساكن bool
get_sym_interprets(بنية xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_پاسخ_t *پاسخ)
{
    صحيح درازی = xcb_xkb_get_compat_map_si_rtrn_درازی(پاسخ);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(پاسخ);

    FAIL_UNLESS(پاسخ->اولی_SIRtrn == 0);
    FAIL_UNLESS(پاسخ->nSIRtrn == پاسخ->n_مجموع_SI);

    keymap->رقم_sym_interprets = پاسخ->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->رقم_sym_interprets);

    ثابت xkb_keycode_t min_کلید_کد = keymap->min_کلید_کد;
    ثابت xkb_keycode_t max_کلید_کد = keymap->max_کلید_کد;

    لكل (صحيح i = 0; i < درازی; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.داده;
        بنية xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        بدل (wire->تطابق & XCB_XKB_SYM_INTERP_تطابق_OP_ماسک) {
        حالة XCB_XKB_SYM_INTERPRET_تطابق_NONE_از:
            sym_interpret->تطابق = تطابق_NONE;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_تطابق_ANY_از_OR_NONE:
            sym_interpret->تطابق = تطابق_ANY_OR_NONE;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_تطابق_ANY_از:
            sym_interpret->تطابق = تطابق_ANY;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_تطابق_ALL_از:
            sym_interpret->تطابق = تطابق_ALL;
            اكسر;
        حالة XCB_XKB_SYM_INTERPRET_تطابق_EXACTLY:
            sym_interpret->تطابق = تطابق_EXACTLY;
            اكسر;
        افتراضي:
            log_err_func(keymap->ctx, XKB_LOG_پیام_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->تطابق & XCB_XKB_SYM_INTERP_تطابق_OP_ماسک);
            اذهب fail;
        }

        sym_interpret->level_one_only =
            (wire->تطابق & XCB_XKB_SYM_INTERP_تطابق_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        إذا (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        وإلا
            sym_interpret->virtual_mod = رقم_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_کلید_کد, max_کلید_کد));
        sym_interpret->رقم_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_بعدی(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_compat_map(بنية xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_پاسخ_t *پاسخ =
        xcb_xkb_get_compat_map_پاسخ(conn, cookie, عدم);

    FAIL_IF_BAD_پاسخ(پاسخ, "XkbGetCompatMap");

    إذا (!get_sym_interprets(keymap, conn, پاسخ))
        اذهب fail;

    حرر(پاسخ);
    أرجع true;

fail:
    حرر(پاسخ);
    أرجع false;
}

ساكن bool
get_type_نامها(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
               xcb_xkb_get_نامها_پاسخ_t *پاسخ,
               xcb_xkb_get_نامها_مقدار_list_t *list)
{
    صحيح کلید_type_نامها_درازی =
        xcb_xkb_get_نامها_مقدار_list_type_نامها_درازی(پاسخ, list);
    xcb_atom_t *کلید_type_نامها_iter =
        xcb_xkb_get_نامها_مقدار_list_type_نامها(list);
    صحيح n_سطوح_در_هر_type_درازی =
        xcb_xkb_get_نامها_مقدار_list_n_سطوح_در_هر_type_درازی(پاسخ, list);
    uint8_t *n_سطوح_در_هر_type_iter =
        xcb_xkb_get_نامها_مقدار_list_n_سطوح_در_هر_type(list);
    xcb_atom_t *kt_level_نامها_iter =
        xcb_xkb_get_نامها_مقدار_list_kt_level_نامها(list);

    FAIL_UNLESS(پاسخ->nTypes == keymap->رقم_types);
    FAIL_UNLESS(کلید_type_نامها_درازی == n_سطوح_در_هر_type_درازی);

    لكل (صحيح i = 0; i < کلید_type_نامها_درازی; i++) {
        xcb_atom_t wire_type_name = *کلید_type_نامها_iter;
        uint8_t wire_رقم_سطوح = *n_سطوح_در_هر_type_iter;
        بنية xkb_کلید_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->رقم_سطوح >= wire_رقم_سطوح);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_نامها, type->رقم_سطوح);
        type->رقم_level_نامها = type->رقم_سطوح;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        لكل (اندازه_t j = 0; j < wire_رقم_سطوح; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_نامها_iter[j],
                                         &type->level_نامها[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        لكل (اندازه_t j = wire_رقم_سطوح; j < type->رقم_سطوح; j++)
            type->level_نامها[j] = XKB_ATOM_NONE;

        kt_level_نامها_iter += wire_رقم_سطوح;
        کلید_type_نامها_iter++;
        n_سطوح_در_هر_type_iter++;
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_indicator_نامها(بنية xkb_keymap *keymap,
                    بنية x11_atom_interner *interner,
                    xcb_xkb_get_نامها_پاسخ_t *پاسخ,
                    xcb_xkb_get_نامها_مقدار_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_نامها_مقدار_list_indicator_نامها(list);

    FAIL_UNLESS(msb_pos(پاسخ->indicators) <= keymap->رقم_leds);

    لكل (بیعلامت i = 0; i < رقم_INDICATORS; i++) {
        إذا (پاسخ->indicators & (UINT32_C(1) << i)) {
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
get_vmod_نامها(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
               xcb_xkb_get_نامها_پاسخ_t *پاسخ,
               xcb_xkb_get_نامها_مقدار_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_نامها_مقدار_list_virtual_mod_نامها(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.رقم_mods =
        رقم_REAL_MODS + MIN(msb_pos(پاسخ->virtualMods), رقم_VMODS);
    FAIL_UNLESS(keymap->mods.رقم_mods <= XKB_MAX_MODS);

    static_assert(رقم_REAL_MODS + رقم_VMODS <= XKB_MAX_MODS, "");
    لكل (بیعلامت i = 0; i < رقم_VMODS; i++) {
        إذا (پاسخ->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            بنية xkb_mod *mod = &keymap->mods.mods[رقم_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_گروه_نامها(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
                xcb_xkb_get_نامها_پاسخ_t *پاسخ,
                xcb_xkb_get_نامها_مقدار_list_t *list)
{
    صحيح درازی = xcb_xkb_get_نامها_مقدار_list_groups_درازی(پاسخ, list);
    xcb_atom_t *iter = xcb_xkb_get_نامها_مقدار_list_groups(list);

    keymap->رقم_گروه_نامها = msb_pos(پاسخ->گروه_نامها_2);
    ALLOC_OR_FAIL(keymap->گروه_نامها, keymap->رقم_گروه_نامها);

    لكل (صحيح i = 0; i < درازی; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->گروه_نامها[i]);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_کلید_نامها(بنية xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_نامها_پاسخ_t *پاسخ,
              xcb_xkb_get_نامها_مقدار_list_t *list)
{
    صحيح درازی = xcb_xkb_get_نامها_مقدار_list_کلید_نامها_درازی(پاسخ, list);
    xcb_xkb_کلید_name_iterator_t iter =
        xcb_xkb_get_نامها_مقدار_list_کلید_نامها_iterator(پاسخ, list);

    FAIL_UNLESS(پاسخ->min_کلید_کد_2 == keymap->min_کلید_کد);
    FAIL_UNLESS(پاسخ->max_کلید_کد_2 == keymap->max_کلید_کد);
    FAIL_UNLESS(پاسخ->اولی_کلید == keymap->min_کلید_کد);
    FAIL_UNLESS(پاسخ->اولی_کلید + پاسخ->n_کلیدها - 1U == keymap->max_کلید_کد);

    لكل (صحيح i = 0; i < درازی; i++) {
        xcb_xkb_کلید_name_t *wire = iter.داده;
        xkb_atom_t *کلید_name = &keymap->کلیدها[پاسخ->اولی_کلید + i].name;

        إذا (wire->name[0] == '\0') {
            *کلید_name = XKB_ATOM_NONE;
        }
        وإلا {
            *کلید_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_کلید_NAME_درازی));
            إذا (!*کلید_name)
                أرجع false;
        }

        xcb_xkb_کلید_name_بعدی(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_aliases(بنية xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_نامها_پاسخ_t *پاسخ,
            xcb_xkb_get_نامها_مقدار_list_t *list)
{
    صحيح درازی = xcb_xkb_get_نامها_مقدار_list_کلید_aliases_درازی(پاسخ, list);
    xcb_xkb_کلید_alias_iterator_t iter =
        xcb_xkb_get_نامها_مقدار_list_کلید_aliases_iterator(پاسخ, list);

    keymap->رقم_کلید_aliases = پاسخ->n_کلید_Aliases;
    ALLOC_OR_FAIL(keymap->کلید_aliases, keymap->رقم_کلید_aliases);

    لكل (صحيح i = 0; i < درازی; i++) {
        xcb_xkb_کلید_alias_t *wire = iter.داده;
        بنية xkb_کلید_alias *alias = &keymap->کلید_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_کلید_NAME_درازی));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_کلید_NAME_درازی));
        إذا (!alias->real || !alias->alias)
            اذهب fail;

        xcb_xkb_کلید_alias_بعدی(&iter);
    }

    أرجع true;

fail:
    أرجع false;
}

ساكن bool
get_نامها(بنية xkb_keymap *keymap, بنية x11_atom_interner *interner,
          xcb_xkb_get_نامها_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_نامها_پاسخ_t *پاسخ =
        xcb_xkb_get_نامها_پاسخ(conn, cookie, عدم);
    xcb_xkb_get_نامها_مقدار_list_t list;

    FAIL_IF_BAD_پاسخ(پاسخ, "XkbGetNames");

    FAIL_UNLESS((پاسخ->which & get_نامها_required) == get_نامها_required);

    xcb_xkb_get_نامها_مقدار_list_unpack(xcb_xkb_get_نامها_مقدار_list(پاسخ),
                                        پاسخ->nTypes,
                                        پاسخ->indicators,
                                        پاسخ->virtualMods,
                                        پاسخ->گروه_نامها_2,
                                        پاسخ->n_کلیدها,
                                        پاسخ->n_کلید_Aliases,
                                        پاسخ->nRadioGroups,
                                        پاسخ->which,
                                        &list);

    x11_atom_interner_get_escaped_atom_name(interner, list.keycodesName,
                                            &keymap->keycodes_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.symbolsName,
                                            &keymap->symbols_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.typesName,
                                            &keymap->types_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.compatName,
                                            &keymap->compat_section_name);
    إذا (!get_type_نامها(keymap, interner, پاسخ, &list) ||
        !get_indicator_نامها(keymap, interner, پاسخ, &list) ||
        !get_vmod_نامها(keymap, interner, پاسخ, &list) ||
        !get_گروه_نامها(keymap, interner, پاسخ, &list) ||
        !get_کلید_نامها(keymap, conn, پاسخ, &list) ||
        !get_aliases(keymap, conn, پاسخ, &list))
        اذهب fail;

    حرر(پاسخ);
    أرجع true;

fail:
    حرر(پاسخ);
    أرجع false;
}

ساكن bool
get_controls(بنية xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_پاسخ_t *پاسخ =
        xcb_xkb_get_controls_پاسخ(conn, cookie, عدم);

    FAIL_IF_BAD_پاسخ(پاسخ, "XkbGetControls");
    FAIL_UNLESS(پاسخ->رقم_Groups > 0 && پاسخ->رقم_Groups <= 4);

    keymap->رقم_groups = پاسخ->رقم_Groups;

    FAIL_UNLESS(keymap->max_کلید_کد < XCB_XKB_CONST_در_هر_کلید_بیت_ARRAY_اندازه * 8);

    لكل (xkb_keycode_t i = keymap->min_کلید_کد; i <= keymap->max_کلید_کد; i++)
        keymap->کلیدها[i].repeats = (پاسخ->در_هر_کلید_Repeat[i / 8] & (1u << (i % 8)));

    حرر(پاسخ);
    أرجع true;

fail:
    حرر(پاسخ);
    أرجع false;
}

بنية xkb_keymap *
xkb_x11_keymap_new_from_device(بنية xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               تعداد xkb_keymap_compile_flags flags)
{
    إذا (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_پیام_NO_ID,
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
        xcb_xkb_get_indicator_map(conn, device_id, ALL_INDICATORS_ماسک);
    xcb_xkb_get_compat_map_cookie_t compat_map_cookie =
        xcb_xkb_get_compat_map(conn, device_id, 0, true, 0, 0);
    xcb_xkb_get_نامها_cookie_t نامها_cookie =
        xcb_xkb_get_نامها(conn, device_id, get_نامها_wanted);
    xcb_xkb_get_controls_cookie_t controls_cookie =
        xcb_xkb_get_controls(conn, device_id);

    إذا (!get_map(keymap, conn, map_cookie))
        اذهب err_map;
    إذا (!get_indicator_map(keymap, conn, indicator_map_cookie))
        اذهب err_indicator_map;
    إذا (!get_compat_map(keymap, conn, compat_map_cookie))
        اذهب err_compat_map;
    إذا (!get_نامها(keymap, &interner, نامها_cookie))
        اذهب err_نامها;
    إذا (!get_controls(keymap, conn, controls_cookie))
        اذهب err_controls;
    x11_atom_interner_دور_trip(&interner);
    إذا (interner.had_error)
        اذهب err_interner;

    أرجع keymap;

err_map:
    xcb_discard_پاسخ(conn, indicator_map_cookie.sequence);
err_indicator_map:
    xcb_discard_پاسخ(conn, compat_map_cookie.sequence);
err_compat_map:
    xcb_discard_پاسخ(conn, نامها_cookie.sequence);
err_نامها:
    xcb_discard_پاسخ(conn, controls_cookie.sequence);
err_controls:
    x11_atom_interner_دور_trip(&interner);
err_interner:
    xkb_keymap_unref(keymap);
    أرجع عدم;
}
