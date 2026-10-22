/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <xcb/xkb.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-keysyms.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "x11-priv.h"

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕ_REAL_MODS 8u
/* XkbNumVirtualMods. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕ_VMODS 16u
/* XkbNoModifier. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_MODIFIER 0xff
/* XkbNumIndicators. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕ_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALL_INDICATORS_ਨਕਾਬ 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FAIL_UNLESS(ਵਿਅੰਜਕ) ਕਰੋ {                              \
    ਜੇ (!(ਵਿਅੰਜਕ)) {                                          \
        log_err(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(ਵਿਅੰਜਕ));                 \
        ਜਾਓ fail;                                          \
    }                                                       \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FAIL_IF_BAD_ਜਵਾਬ(ਜਵਾਬ, request_name) ਕਰੋ {         \
    ਜੇ (!(ਜਵਾਬ)) {                                           \
        log_err(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        ਜਾਓ fail;                                          \
    }                                                       \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALLOC_OR_FAIL(arr, nmemb) ਕਰੋ {                      \
    ਜੇ ((nmemb) > 0) {                                      \
        (arr) = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ((nmemb), ਆਕਾਰ(*(arr)));            \
        ਜੇ (!(arr))                                         \
            ਜਾਓ fail;                                      \
    }                                                       \
} ਜਦੋਂ (0)

ਸਥਿਰ ਅਚਲ xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_ਕੁੰਜੀ_TYPES |
     XCB_XKB_MAP_PART_ਕੁੰਜੀ_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_ਕੁੰਜੀ_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

ਸਥਿਰ ਅਚਲ xcb_xkb_name_detail_t get_ਨਾਂ_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_ਕੁੰਜੀ_TYPE_ਨਾਂ |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_ਨਾਂ |
     XCB_XKB_NAME_DETAIL_INDICATOR_ਨਾਂ |
     XCB_XKB_NAME_DETAIL_ਕੁੰਜੀ_ਨਾਂ |
     XCB_XKB_NAME_DETAIL_ਕੁੰਜੀ_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_ਨਾਂ |
     XCB_XKB_NAME_DETAIL_ਸਮੂਹ_ਨਾਂ);
ਸਥਿਰ ਅਚਲ xcb_xkb_name_detail_t get_ਨਾਂ_required =
    (XCB_XKB_NAME_DETAIL_ਕੁੰਜੀ_TYPE_ਨਾਂ |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_ਨਾਂ |
     XCB_XKB_NAME_DETAIL_ਕੁੰਜੀ_ਨਾਂ |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_ਨਾਂ);


ਸਥਿਰ xkb_mod_ਨਕਾਬ_t
translate_mods(uint8_t rmods, uint16_t vmods_ਨੀਵਾਂ, uint16_t vmods_ਉੱਚਾ)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    ਵਾਪਸ
        ((xkb_mod_ਨਕਾਬ_t) rmods) |
        ((xkb_mod_ਨਕਾਬ_t) vmods_ਨੀਵਾਂ << 8) |
        ((xkb_mod_ਨਕਾਬ_t) vmods_ਉੱਚਾ << 16);
}

ਗਿਣਤੀ xkb_action_controls
translate_controls_ਨਕਾਬ(uint32_t wire)
{
    ਗਿਣਤੀ xkb_action_controls ret = 0;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_REPEAT_ਕੁੰਜੀਆਂ)
        ret |= CONTROL_REPEAT;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_SLOW_ਕੁੰਜੀਆਂ)
        ret |= CONTROL_SLOW;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_BOUNCE_ਕੁੰਜੀਆਂ)
        ret |= CONTROL_DEBOUNCE;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_STICKY_ਕੁੰਜੀਆਂ)
        ret |= CONTROL_STICKY_ਕੁੰਜੀਆਂ;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_MOUSE_ਕੁੰਜੀਆਂ)
        ret |= CONTROL_MOUSE_ਕੁੰਜੀਆਂ;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_MOUSE_ਕੁੰਜੀਆਂ_ACCEL)
        ret |= CONTROL_MOUSE_ਕੁੰਜੀਆਂ_ACCEL;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_ਕੁੰਜੀਆਂ)
        ret |= CONTROL_AX;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_ਨਕਾਬ)
        ret |= CONTROL_AX_TIMEOUT;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_ਨਕਾਬ)
        ret |= CONTROL_AX_FEEDBACK;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_ਨਕਾਬ)
        ret |= CONTROL_BELL;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_ਨਕਾਬ)
        ret |= CONTROL_OVERLAY1;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_ਨਕਾਬ)
        ret |= CONTROL_OVERLAY2;
    ਜੇ (wire & XCB_XKB_BOOL_CTRL_IGNORE_ਸਮੂਹ_ਤਾਲਾ_ਨਕਾਬ)
        ret |= CONTROL_IGNORE_ਸਮੂਹ_ਤਾਲਾ;
    /* Some controls are not supported and don't appear here. */
    ਵਾਪਸ ret;
}

ਸਥਿਰ bool
translate_action(ਸੰਘ xkb_action *action, ਅਚਲ xcb_xkb_action_t *wire,
                 xkb_keycode_t min_ਕੁੰਜੀ_ਕੋਡ, xkb_keycode_t max_ਕੁੰਜੀ_ਕੋਡ)
{
    ਚੋਣ (wire->type) {
    ਹਾਲਤ XCB_XKB_SA_TYPE_ਸੈੱਟ_MODS:
        action->type = ACTION_TYPE_MOD_ਸੈੱਟ;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_ਨੀਵਾਂ_2,
                                                wire->setmods.vmods_ਉੱਚਾ_2);
        action->mods.mods.ਨਕਾਬ = translate_mods(wire->setmods.ਨਕਾਬ, 0, 0);

        ਜੇ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_ਤਾਲਾ_CLEAR;
        ਜੇ (wire->setmods.flags & XCB_XKB_SA_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ)
            action->mods.flags |= ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ;
        ਜੇ (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_ਨੀਵਾਂ_2,
                                                wire->latchmods.vmods_ਉੱਚਾ_2);
        action->mods.mods.ਨਕਾਬ = translate_mods(wire->latchmods.ਨਕਾਬ, 0, 0);

        ਜੇ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_ਤਾਲਾ_CLEAR;
        ਜੇ (wire->latchmods.flags & XCB_XKB_SA_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ)
            action->mods.flags |= ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ;
        ਜੇ (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_ਤਾਲਾ_MODS:
        action->type = ACTION_TYPE_MOD_ਤਾਲਾ;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_ਨੀਵਾਂ_2,
                                                wire->lockmods.vmods_ਉੱਚਾ_2);
        action->mods.mods.ਨਕਾਬ = translate_mods(wire->lockmods.ਨਕਾਬ, 0, 0);

        ਜੇ (wire->lockmods.flags & XCB_XKB_SA_ISO_ਤਾਲਾ_FLAG_NO_ਤਾਲਾ)
            action->mods.flags |= ACTION_ਤਾਲਾ_NO_ਤਾਲਾ;
        ਜੇ (wire->lockmods.flags & XCB_XKB_SA_ISO_ਤਾਲਾ_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_ਤਾਲਾ_NO_UNLOCK;
        ਜੇ (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_ਸੈੱਟ_ਸਮੂਹ:
        action->type = ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ;

        action->ਸਮੂਹ.ਸਮੂਹ = (int32_t)wire->setgroup.ਸਮੂਹ;

        ਜੇ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->ਸਮੂਹ.flags |= ACTION_ਤਾਲਾ_CLEAR;
        ਜੇ (wire->setmods.flags & XCB_XKB_SA_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ)
            action->ਸਮੂਹ.flags |= ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ;
        ਜੇ (wire->setmods.flags & XCB_XKB_SA_ISO_ਤਾਲਾ_FLAG_ਸਮੂਹ_ABSOLUTE)
            action->ਸਮੂਹ.flags |= ACTION_ABSOLUTE_SWITCH;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_LATCH_ਸਮੂਹ:
        action->type = ACTION_TYPE_ਸਮੂਹ_LATCH;

        action->ਸਮੂਹ.ਸਮੂਹ = (int32_t)wire->latchgroup.ਸਮੂਹ;

        ਜੇ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->ਸਮੂਹ.flags |= ACTION_ਤਾਲਾ_CLEAR;
        ਜੇ (wire->latchmods.flags & XCB_XKB_SA_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ)
            action->ਸਮੂਹ.flags |= ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ;
        ਜੇ (wire->latchmods.flags & XCB_XKB_SA_ISO_ਤਾਲਾ_FLAG_ਸਮੂਹ_ABSOLUTE)
            action->ਸਮੂਹ.flags |= ACTION_ABSOLUTE_SWITCH;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_ਤਾਲਾ_ਸਮੂਹ:
        action->type = ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ;

        action->ਸਮੂਹ.ਸਮੂਹ = (int32_t)wire->lockgroup.ਸਮੂਹ;

        ਜੇ (wire->lockgroup.flags & XCB_XKB_SA_ISO_ਤਾਲਾ_FLAG_ਸਮੂਹ_ABSOLUTE)
            action->ਸਮੂਹ.flags |= ACTION_ABSOLUTE_SWITCH;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_ਨੀਵਾਂ | ((uint16_t) wire->moveptr.x_ਉੱਚਾ << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_ਨੀਵਾਂ | ((uint16_t) wire->moveptr.y_ਉੱਚਾ << 8));

        ਜੇ (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        ਜੇ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        ਜੇ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.ਗਿਣਤੀ_2 = wire->ptrbtn.ਗਿਣਤੀ_2;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_ਤਾਲਾ_PTR_BTN:
        action->type = ACTION_TYPE_PTR_ਤਾਲਾ;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.ਗਿਣਤੀ_2 = 0;

        ਜੇ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_ਤਾਲਾ_FLAG_NO_ਤਾਲਾ)
            action->btn.flags |= ACTION_ਤਾਲਾ_NO_ਤਾਲਾ;
        ਜੇ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_ਤਾਲਾ_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_ਤਾਲਾ_NO_UNLOCK;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_ਸੈੱਟ_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.ਮੁੱਲ = wire->setptrdflt.ਮੁੱਲ;

        ਜੇ (wire->setptrdflt.flags & XCB_XKB_SA_ਸੈੱਟ_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        ਜੇ (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        ਜੇ (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_ਸੈੱਟ_CONTROLS:
        action->type = ACTION_TYPE_CTRL_ਸੈੱਟ;
        {
            ਅਚਲ uint16_t ਨਕਾਬ = (wire->setcontrols.bool_Ctrls_ਨੀਵਾਂ |
                                   (wire->setcontrols.bool_Ctrls_ਉੱਚਾ << 8));
            action->ctrls.ctrls = translate_controls_ਨਕਾਬ(ਨਕਾਬ);
        }
        ਰੋਕੋ;
    ਹਾਲਤ XCB_XKB_SA_TYPE_ਤਾਲਾ_CONTROLS:
        action->type = ACTION_TYPE_CTRL_ਤਾਲਾ;
        {
            ਅਚਲ uint16_t ਨਕਾਬ = (wire->lockcontrols.bool_Ctrls_ਨੀਵਾਂ |
                                   (wire->lockcontrols.bool_Ctrls_ਉੱਚਾ << 8));
            action->ctrls.ctrls = translate_controls_ਨਕਾਬ(ਨਕਾਬ);
        }
        ਰੋਕੋ;

    ਹਾਲਤ XCB_XKB_SA_TYPE_REDIRECT_ਕੁੰਜੀ:
        action->type = ACTION_TYPE_REDIRECT_ਕੁੰਜੀ;
        ਜੇ (wire->redirect.newkey < min_ਕੁੰਜੀ_ਕੋਡ ||
            wire->redirect.newkey > max_ਕੁੰਜੀ_ਕੋਡ)
                ਵਾਪਸ false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.ਨਕਾਬ,
            wire->redirect.vmods_ਨਕਾਬ_ਨੀਵਾਂ,
            wire->redirect.vmods_ਨਕਾਬ_ਉੱਚਾ
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_ਨੀਵਾਂ_2,
                                               wire->redirect.vmods_ਉੱਚਾ_2);
        ਰੋਕੋ;

    ਹਾਲਤ XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        ਰੋਕੋ;

    /* We don't support these. */
    ਹਾਲਤ XCB_XKB_SA_TYPE_ISO_ਤਾਲਾ:
    ਹਾਲਤ XCB_XKB_SA_TYPE_ACTION_ਸੁਨੇਹਾ:
    ਹਾਲਤ XCB_XKB_SA_TYPE_DEVICE_BTN:
    ਹਾਲਤ XCB_XKB_SA_TYPE_ਤਾਲਾ_DEVICE_BTN:
    ਹਾਲਤ XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        ਰੋਕੋ;

    ਮੂਲ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_ਅੰਦਰੂਨੀ == 20 &&
                      ACTION_TYPE_ਅੰਦਰੂਨੀ + 1 == ACTION_TYPE_ਅੰਕ_ENTRIES,
                      "Missing action type");

        ਜੇ (wire->type < ACTION_TYPE_ਨਿੱਜੀ) {
            action->type = ACTION_TYPE_NONE;
            ਰੋਕੋ;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(ਆਕਾਰ(action->priv.ਅੰਕੜੇ) == 7 &&
                      ਆਕਾਰ(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(action->priv.ਅੰਕੜੇ, wire->noaction.pad0, 7);
        ਰੋਕੋ;
    }
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
get_types(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_ਜਵਾਬ_t *ਜਵਾਬ, xcb_xkb_get_map_map_t *map)
{
    ਪੂਰਨ types_ਲੰਮਾਈ = xcb_xkb_get_map_map_types_rtrn_ਲੰਮਾਈ(ਜਵਾਬ, map);
    xcb_xkb_ਕੁੰਜੀ_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(ਜਵਾਬ, map);

    FAIL_UNLESS(ਜਵਾਬ->ਪਹਿਲਾ_Type == 0);

    keymap->ਅੰਕ_types = ਜਵਾਬ->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->ਅੰਕ_types);
    ਪ੍ਰਤਿਜ੍ਞਾ(types_ਲੰਮਾਈ == (ਪੂਰਨ) keymap->ਅੰਕ_types);

    ਲਈ (ਪੂਰਨ i = 0; i < types_ਲੰਮਾਈ; i++) {
        xcb_xkb_ਕੁੰਜੀ_type_t *wire_type = types_iter.ਅੰਕੜੇ;
        ਬਣਤਰ xkb_ਕੁੰਜੀ_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->ਅੰਕ_ਪੱਧਰ_2 > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.ਨਕਾਬ = translate_mods(wire_type->mods_ਨਕਾਬ, 0, 0);
        type->ਅੰਕ_ਪੱਧਰ = wire_type->ਅੰਕ_ਪੱਧਰ_2;

        {
            ਪੂਰਨ entries_ਲੰਮਾਈ = xcb_xkb_ਕੁੰਜੀ_type_map_ਲੰਮਾਈ(wire_type);
            xcb_xkb_kt_map_ਇੰਦਰਾਜ_iterator_t entries_iter =
                xcb_xkb_ਕੁੰਜੀ_type_map_iterator(wire_type);

            type->ਅੰਕ_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->ਅੰਕ_entries);
            ਪ੍ਰਤਿਜ੍ਞਾ(entries_ਲੰਮਾਈ == (ਪੂਰਨ) type->ਅੰਕ_entries);

            ਲਈ (ਪੂਰਨ j = 0; j < entries_ਲੰਮਾਈ; j++) {
                xcb_xkb_kt_map_ਇੰਦਰਾਜ_t *wire_ਇੰਦਰਾਜ = entries_iter.ਅੰਕੜੇ;
                ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = &type->entries[j];

                FAIL_UNLESS(wire_ਇੰਦਰਾਜ->level < type->ਅੰਕ_ਪੱਧਰ);

                ਇੰਦਰਾਜ->level = wire_ਇੰਦਰਾਜ->level;
                ਇੰਦਰਾਜ->mods.mods = translate_mods(wire_ਇੰਦਰਾਜ->mods_mods,
                                                  wire_ਇੰਦਰਾਜ->mods_vmods, 0);
                ਇੰਦਰਾਜ->mods.ਨਕਾਬ = translate_mods(wire_ਇੰਦਰਾਜ->mods_ਨਕਾਬ, 0, 0);

                xcb_xkb_kt_map_ਇੰਦਰਾਜ_ਅਗਲਾ(&entries_iter);
            }
        }

        {
            ਪੂਰਨ preserves_ਲੰਮਾਈ = xcb_xkb_ਕੁੰਜੀ_type_preserve_ਲੰਮਾਈ(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_ਕੁੰਜੀ_type_preserve_iterator(wire_type);

            FAIL_UNLESS((ਬਿਨਾਨਿਸ਼ਾਨੀ) preserves_ਲੰਮਾਈ <= type->ਅੰਕ_entries);

            ਲਈ (ਪੂਰਨ j = 0; j < preserves_ਲੰਮਾਈ; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.ਅੰਕੜੇ;
                ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = &type->entries[j];

                ਇੰਦਰਾਜ->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                ਇੰਦਰਾਜ->preserve.ਨਕਾਬ = translate_mods(wire_preserve->ਨਕਾਬ, 0, 0);

                xcb_xkb_mod_def_ਅਗਲਾ(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_ਕੁੰਜੀ_type_ਅਗਲਾ(&types_iter);
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_sym_maps(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_ਜਵਾਬ_t *ਜਵਾਬ, xcb_xkb_get_map_map_t *map)
{
    ਪੂਰਨ sym_maps_ਲੰਮਾਈ = xcb_xkb_get_map_map_syms_rtrn_ਲੰਮਾਈ(ਜਵਾਬ, map);
    xcb_xkb_ਕੁੰਜੀ_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(ਜਵਾਬ, map);

    FAIL_UNLESS(ਜਵਾਬ->min_ਕੁੰਜੀ_ਕੋਡ_2 <= ਜਵਾਬ->max_ਕੁੰਜੀ_ਕੋਡ_2);
    FAIL_UNLESS(ਜਵਾਬ->ਪਹਿਲਾ_ਕੁੰਜੀ_Sym >= ਜਵਾਬ->min_ਕੁੰਜੀ_ਕੋਡ_2);
    FAIL_UNLESS(ਜਵਾਬ->ਪਹਿਲਾ_ਕੁੰਜੀ_Sym + ਜਵਾਬ->n_ਕੁੰਜੀ_Syms <= ਜਵਾਬ->max_ਕੁੰਜੀ_ਕੋਡ_2 + 1);

    keymap->min_ਕੁੰਜੀ_ਕੋਡ = ਜਵਾਬ->min_ਕੁੰਜੀ_ਕੋਡ_2;
    keymap->max_ਕੁੰਜੀ_ਕੋਡ = ਜਵਾਬ->max_ਕੁੰਜੀ_ਕੋਡ_2;
    keymap->ਅੰਕ_ਕੁੰਜੀਆਂ = keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ = ਜਵਾਬ->max_ਕੁੰਜੀ_ਕੋਡ_2 + 1;

    ALLOC_OR_FAIL(keymap->ਕੁੰਜੀਆਂ, keymap->max_ਕੁੰਜੀ_ਕੋਡ + 1);

    ਲਈ (xkb_keycode_t kc = keymap->min_ਕੁੰਜੀ_ਕੋਡ; kc <= keymap->max_ਕੁੰਜੀ_ਕੋਡ; kc++)
        keymap->ਕੁੰਜੀਆਂ[kc].keycode = kc;

    ਲਈ (ਪੂਰਨ i = 0; i < sym_maps_ਲੰਮਾਈ; i++) {
        xcb_xkb_ਕੁੰਜੀ_sym_map_t *wire_sym_map = sym_maps_iter.ਅੰਕੜੇ;
        ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = &keymap->ਕੁੰਜੀਆਂ[ਜਵਾਬ->ਪਹਿਲਾ_ਕੁੰਜੀ_Sym + i];

        ਕੁੰਜੀ->ਅੰਕ_groups = wire_sym_map->ਸਮੂਹ_Info & 0x0f;
        FAIL_UNLESS(ਕੁੰਜੀ->ਅੰਕ_groups <= ARRAY_ਮਾਪ(wire_sym_map->kt_ਸੂਚਕਾਂਕ));
        ALLOC_OR_FAIL(ਕੁੰਜੀ->groups, ਕੁੰਜੀ->ਅੰਕ_groups);

        ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t j = 0; j < ਕੁੰਜੀ->ਅੰਕ_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_ਸੂਚਕਾਂਕ[j] < keymap->ਅੰਕ_types);
            ਕੁੰਜੀ->groups[j].type = &keymap->types[wire_sym_map->kt_ਸੂਚਕਾਂਕ[j]];

            ALLOC_OR_FAIL(ਕੁੰਜੀ->groups[j].ਪੱਧਰ, ਕੁੰਜੀ->groups[j].type->ਅੰਕ_ਪੱਧਰ);
        }

        ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ = (wire_sym_map->ਸਮੂਹ_Info & 0x30) >> 4;

        FAIL_UNLESS(ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ <= ਕੁੰਜੀ->ਅੰਕ_groups);

        ਜੇ (wire_sym_map->ਸਮੂਹ_Info & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy = XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP;
        ਨਹੀਂਤਾਂ ਜੇ (wire_sym_map->ਸਮੂਹ_Info & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy = XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT;
        ਨਹੀਂਤਾਂ
            ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy = XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP;

        {
            ਪੂਰਨ syms_ਲੰਮਾਈ = xcb_xkb_ਕੁੰਜੀ_sym_map_syms_ਲੰਮਾਈ(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_ਕੁੰਜੀ_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((ਬਿਨਾਨਿਸ਼ਾਨੀ) syms_ਲੰਮਾਈ == wire_sym_map->ਚੌੜਾਈ * ਕੁੰਜੀ->ਅੰਕ_groups);

            ਜੇ (syms_ਲੰਮਾਈ > 0)
                ਕੁੰਜੀ->explicit |= EXPLICIT_SYMBOLS;

            ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0; ਸਮੂਹ < ਕੁੰਜੀ->ਅੰਕ_groups; ਸਮੂਹ++) {
                ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t level = 0; level < wire_sym_map->ਚੌੜਾਈ; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    ਪ੍ਰਤਿਜ੍ਞਾ(ਕੁੰਜੀ->groups[ਸਮੂਹ].type != ਨਿਰਾ);
                    ਜੇ (level < ਕੁੰਜੀ->groups[ਸਮੂਹ].type->ਅੰਕ_ਪੱਧਰ) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].ਅੰਕ_syms = 1;
                        ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].ਉੱਤਲਾ =
                            xkb_keysym_ਮੰਜ਼ਿਲ_ਉੱਤਲਾ(wire_keysym);
                        ਜੇ (wire_keysym != XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ)
                            ਕੁੰਜੀ->groups[ਸਮੂਹ].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_ਕੁੰਜੀ_sym_map_ਅਗਲਾ(&sym_maps_iter);
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_actions(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_ਜਵਾਬ_t *ਜਵਾਬ, xcb_xkb_get_map_map_t *map)
{
    ਪੂਰਨ acts_ਗਿਣਤੀ_ਲੰਮਾਈ =
        xcb_xkb_get_map_map_acts_rtrn_ਗਿਣਤੀ_ਲੰਮਾਈ(ਜਵਾਬ, map);
    uint8_t *acts_ਗਿਣਤੀ_iter = xcb_xkb_get_map_map_acts_rtrn_ਗਿਣਤੀ(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(ਜਵਾਬ, map);
    xcb_xkb_ਕੁੰਜੀ_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(ਜਵਾਬ, map);

    FAIL_UNLESS(ਜਵਾਬ->ਪਹਿਲਾ_ਕੁੰਜੀ_Action == keymap->min_ਕੁੰਜੀ_ਕੋਡ);
    FAIL_UNLESS(ਜਵਾਬ->ਪਹਿਲਾ_ਕੁੰਜੀ_Action + ਜਵਾਬ->n_ਕੁੰਜੀ_Actions ==
                keymap->max_ਕੁੰਜੀ_ਕੋਡ + 1);

    ਲਈ (ਪੂਰਨ i = 0; i < acts_ਗਿਣਤੀ_ਲੰਮਾਈ; i++) {
        xcb_xkb_ਕੁੰਜੀ_sym_map_t *wire_sym_map = sym_maps_iter.ਅੰਕੜੇ;
        ਪੂਰਨ syms_ਲੰਮਾਈ = xcb_xkb_ਕੁੰਜੀ_sym_map_syms_ਲੰਮਾਈ(wire_sym_map);
        uint8_t wire_ਗਿਣਤੀ = *acts_ਗਿਣਤੀ_iter;
        ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = &keymap->ਕੁੰਜੀਆਂ[ਜਵਾਬ->ਪਹਿਲਾ_ਕੁੰਜੀ_Action + i];

        FAIL_UNLESS((ਬਿਨਾਨਿਸ਼ਾਨੀ) syms_ਲੰਮਾਈ == wire_sym_map->ਚੌੜਾਈ * ਕੁੰਜੀ->ਅੰਕ_groups);
        FAIL_UNLESS(wire_ਗਿਣਤੀ == 0 || wire_ਗਿਣਤੀ == syms_ਲੰਮਾਈ);

        ਅਚਲ xkb_keycode_t min_ਕੁੰਜੀ_ਕੋਡ = keymap->min_ਕੁੰਜੀ_ਕੋਡ;
        ਅਚਲ xkb_keycode_t max_ਕੁੰਜੀ_ਕੋਡ = keymap->max_ਕੁੰਜੀ_ਕੋਡ;

        ਜੇ (wire_ਗਿਣਤੀ != 0) {
            ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0; ਸਮੂਹ < ਕੁੰਜੀ->ਅੰਕ_groups; ਸਮੂਹ++) {
                ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t level = 0; level < wire_sym_map->ਚੌੜਾਈ; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.ਅੰਕੜੇ;

                    ਜੇ (level < ਕੁੰਜੀ->groups[ਸਮੂਹ].type->ਅੰਕ_ਪੱਧਰ) {
                        ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].ਅੰਕ_actions = 1;
                        ਸੰਘ xkb_action *action =
                            &ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_ਕੁੰਜੀ_ਕੋਡ, max_ਕੁੰਜੀ_ਕੋਡ));

                        ਜੇ (action->type != ACTION_TYPE_NONE) {
                            ਕੁੰਜੀ->groups[ਸਮੂਹ].implicit_actions =
                                !(ਕੁੰਜੀ->explicit & EXPLICIT_INTERP);
                        } ਨਹੀਂਤਾਂ ਜੇ (ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].s.sym ==
                                   XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].ਅੰਕ_syms = 0;
                            ਕੁੰਜੀ->groups[ਸਮੂਹ].ਪੱਧਰ[level].ਅੰਕ_actions = 0;
                        }
                    }

                    xcb_xkb_action_ਅਗਲਾ(&acts_iter);
                }

                ਜੇ (ਕੁੰਜੀ->groups[ਸਮੂਹ].implicit_actions)
                    ਕੁੰਜੀ->implicit_actions = true;
            }
        }

        acts_ਗਿਣਤੀ_iter++;
        xcb_xkb_ਕੁੰਜੀ_sym_map_ਅਗਲਾ(&sym_maps_iter);
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_vmods(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_ਜਵਾਬ_t *ਜਵਾਬ, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.ਅੰਕ_mods =
        ਅੰਕ_REAL_MODS + MIN(msb_pos(ਜਵਾਬ->virtualMods), ਅੰਕ_VMODS);
    FAIL_UNLESS(keymap->mods.ਅੰਕ_mods <= XKB_MAX_MODS);

    static_assert(ਅੰਕ_REAL_MODS + ਅੰਕ_VMODS <= XKB_MAX_MODS, "");
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_VMODS; i++) {
        ਜੇ (ਜਵਾਬ->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            ਬਣਤਰ xkb_mod *mod = &keymap->mods.mods[ਅੰਕ_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_explicits(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_ਜਵਾਬ_t *ਜਵਾਬ, xcb_xkb_get_map_map_t *map)
{
    ਪੂਰਨ ਲੰਮਾਈ = xcb_xkb_get_map_map_explicit_rtrn_ਲੰਮਾਈ(ਜਵਾਬ, map);
    xcb_xkb_ਸੈੱਟ_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(ਜਵਾਬ, map);

    ਲਈ (ਪੂਰਨ i = 0; i < ਲੰਮਾਈ; i++) {
        xcb_xkb_ਸੈੱਟ_explicit_t *wire = iter.ਅੰਕੜੇ;
        ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;

        FAIL_UNLESS(wire->keycode >= keymap->min_ਕੁੰਜੀ_ਕੋਡ &&
                    wire->keycode <= keymap->max_ਕੁੰਜੀ_ਕੋਡ);

        ਕੁੰਜੀ = &keymap->ਕੁੰਜੀਆਂ[wire->keycode];

        ਜੇ ((wire->explicit & XCB_XKB_EXPLICIT_ਕੁੰਜੀ_TYPE_1) &&
            ਕੁੰਜੀ->ਅੰਕ_groups > 0) {
            ਕੁੰਜੀ->groups[0].explicit_type = true;
            ਕੁੰਜੀ->explicit |= EXPLICIT_TYPES;
        }
        ਜੇ ((wire->explicit & XCB_XKB_EXPLICIT_ਕੁੰਜੀ_TYPE_2) &&
            ਕੁੰਜੀ->ਅੰਕ_groups > 1) {
            ਕੁੰਜੀ->groups[1].explicit_type = true;
            ਕੁੰਜੀ->explicit |= EXPLICIT_TYPES;
        }
        ਜੇ ((wire->explicit & XCB_XKB_EXPLICIT_ਕੁੰਜੀ_TYPE_3) &&
            ਕੁੰਜੀ->ਅੰਕ_groups > 2) {
            ਕੁੰਜੀ->groups[2].explicit_type = true;
            ਕੁੰਜੀ->explicit |= EXPLICIT_TYPES;
        }
        ਜੇ ((wire->explicit & XCB_XKB_EXPLICIT_ਕੁੰਜੀ_TYPE_4) &&
            ਕੁੰਜੀ->ਅੰਕ_groups > 3) {
            ਕੁੰਜੀ->groups[3].explicit_type = true;
            ਕੁੰਜੀ->explicit |= EXPLICIT_TYPES;
        }
        ਜੇ (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            ਕੁੰਜੀ->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t l = 0; l < ਕੁੰਜੀ->ਅੰਕ_groups; l++) {
                ਕੁੰਜੀ->groups[l].explicit_actions = true;
            }
        }
        ਜੇ (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            ਕੁੰਜੀ->explicit |= EXPLICIT_REPEAT;
        ਜੇ (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            ਕੁੰਜੀ->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_ਸੈੱਟ_explicit_ਅਗਲਾ(&iter);
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_modmaps(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_ਜਵਾਬ_t *ਜਵਾਬ, xcb_xkb_get_map_map_t *map)
{
    ਪੂਰਨ ਲੰਮਾਈ = xcb_xkb_get_map_map_modmap_rtrn_ਲੰਮਾਈ(ਜਵਾਬ, map);
    xcb_xkb_ਕੁੰਜੀ_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(ਜਵਾਬ, map);

    darray(Mod_Map_ਇੰਦਰਾਜ) modmaps = darray_new();

    ਲਈ (ਪੂਰਨ i = 0; i < ਲੰਮਾਈ; i++) {
        xcb_xkb_ਕੁੰਜੀ_mod_map_t *wire = iter.ਅੰਕੜੇ;
        ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;

        FAIL_UNLESS(wire->keycode >= keymap->min_ਕੁੰਜੀ_ਕੋਡ &&
                    wire->keycode <= keymap->max_ਕੁੰਜੀ_ਕੋਡ);

        ਕੁੰਜੀ = &keymap->ਕੁੰਜੀਆਂ[wire->keycode];
        ਕੁੰਜੀ->modmap = wire->mods;

        darray_append(modmaps, (Mod_Map_ਇੰਦਰਾਜ) {
            .ਕੁੰਜੀ_ਕੋਡ = ਕੁੰਜੀ->keycode,
            .mods = ਕੁੰਜੀ->modmap,
            .have_ਚਿੰਨ੍ਹ = false,
            .u.ਕੁੰਜੀ_Name = ਕੁੰਜੀ->name
        });

        xcb_xkb_ਕੁੰਜੀ_mod_map_ਅਗਲਾ(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->ਅੰਕ_modmaps);
    ਵਾਪਸ true;

fail:
    darray_free(modmaps);
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_vmodmaps(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_ਜਵਾਬ_t *ਜਵਾਬ, xcb_xkb_get_map_map_t *map)
{
    ਪੂਰਨ ਲੰਮਾਈ = xcb_xkb_get_map_map_vmodmap_rtrn_ਲੰਮਾਈ(ਜਵਾਬ, map);
    xcb_xkb_ਕੁੰਜੀ_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(ਜਵਾਬ, map);

    ਲਈ (ਪੂਰਨ i = 0; i < ਲੰਮਾਈ; i++) {
        xcb_xkb_ਕੁੰਜੀ_v_mod_map_t *wire = iter.ਅੰਕੜੇ;
        ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;

        FAIL_UNLESS(wire->keycode >= keymap->min_ਕੁੰਜੀ_ਕੋਡ &&
                    wire->keycode <= keymap->max_ਕੁੰਜੀ_ਕੋਡ);

        ਕੁੰਜੀ = &keymap->ਕੁੰਜੀਆਂ[wire->keycode];
        ਕੁੰਜੀ->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_ਕੁੰਜੀ_v_mod_map_ਅਗਲਾ(&iter);
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_map(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_ਜਵਾਬ_t *ਜਵਾਬ = xcb_xkb_get_map_ਜਵਾਬ(conn, cookie, ਨਿਰਾ);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_ਜਵਾਬ(ਜਵਾਬ, "XkbGetMap");

    ਜੇ ((ਜਵਾਬ->present & get_map_required_components) != get_map_required_components)
        ਜਾਓ fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(ਜਵਾਬ),
                               ਜਵਾਬ->nTypes,
                               ਜਵਾਬ->n_ਕੁੰਜੀ_Syms,
                               ਜਵਾਬ->n_ਕੁੰਜੀ_Actions,
                               ਜਵਾਬ->ਕੁੱਲ_Actions,
                               ਜਵਾਬ->ਕੁੱਲ_ਕੁੰਜੀ_Behaviors,
                               ਜਵਾਬ->virtualMods,
                               ਜਵਾਬ->ਕੁੱਲ_ਕੁੰਜੀ_Explicit,
                               ਜਵਾਬ->ਕੁੱਲ_Mod_Map_ਕੁੰਜੀਆਂ,
                               ਜਵਾਬ->ਕੁੱਲ_VMod_Map_ਕੁੰਜੀਆਂ,
                               ਜਵਾਬ->present,
                               &map);

    ਜੇ (!get_types(keymap, conn, ਜਵਾਬ, &map) ||
        !get_sym_maps(keymap, conn, ਜਵਾਬ, &map) ||
        !get_actions(keymap, conn, ਜਵਾਬ, &map) ||
        !get_vmods(keymap, conn, ਜਵਾਬ, &map) ||
        !get_explicits(keymap, conn, ਜਵਾਬ, &map) ||
        !get_modmaps(keymap, conn, ਜਵਾਬ, &map) ||
        !get_vmodmaps(keymap, conn, ਜਵਾਬ, &map))
        ਜਾਓ fail;

    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ true;

fail:
    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_indicators(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_ਜਵਾਬ_t *ਜਵਾਬ)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(ਜਵਾਬ);

    keymap->ਅੰਕ_leds = msb_pos(ਜਵਾਬ->which);
    FAIL_UNLESS(keymap->ਅੰਕ_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == ਅੰਕ_INDICATORS, "");
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_INDICATORS; i++) {
        ਜੇ (ਜਵਾਬ->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.ਅੰਕੜੇ;
            ਬਣਤਰ xkb_led *led = &keymap->leds[i];

            ਜੇ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_ਅਧਾਰ)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            ਜੇ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            ਜੇ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            ਜੇ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            ਜੇ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            ਜੇ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_ਅਧਾਰ)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            ਜੇ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            ਜੇ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            ਜੇ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            ਜੇ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.ਨਕਾਬ = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_ਨਕਾਬ(wire->ctrls);

            xcb_xkb_indicator_map_ਅਗਲਾ(&iter);
        }
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_indicator_map(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_ਜਵਾਬ_t *ਜਵਾਬ =
        xcb_xkb_get_indicator_map_ਜਵਾਬ(conn, cookie, ਨਿਰਾ);

    FAIL_IF_BAD_ਜਵਾਬ(ਜਵਾਬ, "XkbGetIndicatorMap");

    ਜੇ (!get_indicators(keymap, conn, ਜਵਾਬ))
        ਜਾਓ fail;

    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ true;

fail:
    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_sym_interprets(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_ਜਵਾਬ_t *ਜਵਾਬ)
{
    ਪੂਰਨ ਲੰਮਾਈ = xcb_xkb_get_compat_map_si_rtrn_ਲੰਮਾਈ(ਜਵਾਬ);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(ਜਵਾਬ);

    FAIL_UNLESS(ਜਵਾਬ->ਪਹਿਲਾ_SIRtrn == 0);
    FAIL_UNLESS(ਜਵਾਬ->nSIRtrn == ਜਵਾਬ->n_ਕੁੱਲ_SI);

    keymap->ਅੰਕ_sym_interprets = ਜਵਾਬ->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->ਅੰਕ_sym_interprets);

    ਅਚਲ xkb_keycode_t min_ਕੁੰਜੀ_ਕੋਡ = keymap->min_ਕੁੰਜੀ_ਕੋਡ;
    ਅਚਲ xkb_keycode_t max_ਕੁੰਜੀ_ਕੋਡ = keymap->max_ਕੁੰਜੀ_ਕੋਡ;

    ਲਈ (ਪੂਰਨ i = 0; i < ਲੰਮਾਈ; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.ਅੰਕੜੇ;
        ਬਣਤਰ xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        ਚੋਣ (wire->ਮੇਲ & XCB_XKB_SYM_INTERP_ਮੇਲ_OP_ਨਕਾਬ) {
        ਹਾਲਤ XCB_XKB_SYM_INTERPRET_ਮੇਲ_NONE_ਦਾ:
            sym_interpret->ਮੇਲ = ਮੇਲ_NONE;
            ਰੋਕੋ;
        ਹਾਲਤ XCB_XKB_SYM_INTERPRET_ਮੇਲ_ANY_ਦਾ_OR_NONE:
            sym_interpret->ਮੇਲ = ਮੇਲ_ANY_OR_NONE;
            ਰੋਕੋ;
        ਹਾਲਤ XCB_XKB_SYM_INTERPRET_ਮੇਲ_ANY_ਦਾ:
            sym_interpret->ਮੇਲ = ਮੇਲ_ANY;
            ਰੋਕੋ;
        ਹਾਲਤ XCB_XKB_SYM_INTERPRET_ਮੇਲ_ALL_ਦਾ:
            sym_interpret->ਮੇਲ = ਮੇਲ_ALL;
            ਰੋਕੋ;
        ਹਾਲਤ XCB_XKB_SYM_INTERPRET_ਮੇਲ_EXACTLY:
            sym_interpret->ਮੇਲ = ਮੇਲ_EXACTLY;
            ਰੋਕੋ;
        ਮੂਲ:
            log_err_func(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->ਮੇਲ & XCB_XKB_SYM_INTERP_ਮੇਲ_OP_ਨਕਾਬ);
            ਜਾਓ fail;
        }

        sym_interpret->level_one_only =
            (wire->ਮੇਲ & XCB_XKB_SYM_INTERP_ਮੇਲ_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        ਜੇ (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        ਨਹੀਂਤਾਂ
            sym_interpret->virtual_mod = ਅੰਕ_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_ਕੁੰਜੀ_ਕੋਡ, max_ਕੁੰਜੀ_ਕੋਡ));
        sym_interpret->ਅੰਕ_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_ਅਗਲਾ(&iter);
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_compat_map(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_ਜਵਾਬ_t *ਜਵਾਬ =
        xcb_xkb_get_compat_map_ਜਵਾਬ(conn, cookie, ਨਿਰਾ);

    FAIL_IF_BAD_ਜਵਾਬ(ਜਵਾਬ, "XkbGetCompatMap");

    ਜੇ (!get_sym_interprets(keymap, conn, ਜਵਾਬ))
        ਜਾਓ fail;

    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ true;

fail:
    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_type_ਨਾਂ(ਬਣਤਰ xkb_keymap *keymap, ਬਣਤਰ x11_atom_interner *interner,
               xcb_xkb_get_ਨਾਂ_ਜਵਾਬ_t *ਜਵਾਬ,
               xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_t *list)
{
    ਪੂਰਨ ਕੁੰਜੀ_type_ਨਾਂ_ਲੰਮਾਈ =
        xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_type_ਨਾਂ_ਲੰਮਾਈ(ਜਵਾਬ, list);
    xcb_atom_t *ਕੁੰਜੀ_type_ਨਾਂ_iter =
        xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_type_ਨਾਂ(list);
    ਪੂਰਨ n_ਪੱਧਰ_ਪ੍ਰਤੀ_type_ਲੰਮਾਈ =
        xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_n_ਪੱਧਰ_ਪ੍ਰਤੀ_type_ਲੰਮਾਈ(ਜਵਾਬ, list);
    uint8_t *n_ਪੱਧਰ_ਪ੍ਰਤੀ_type_iter =
        xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_n_ਪੱਧਰ_ਪ੍ਰਤੀ_type(list);
    xcb_atom_t *kt_level_ਨਾਂ_iter =
        xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_kt_level_ਨਾਂ(list);

    FAIL_UNLESS(ਜਵਾਬ->nTypes == keymap->ਅੰਕ_types);
    FAIL_UNLESS(ਕੁੰਜੀ_type_ਨਾਂ_ਲੰਮਾਈ == n_ਪੱਧਰ_ਪ੍ਰਤੀ_type_ਲੰਮਾਈ);

    ਲਈ (ਪੂਰਨ i = 0; i < ਕੁੰਜੀ_type_ਨਾਂ_ਲੰਮਾਈ; i++) {
        xcb_atom_t wire_type_name = *ਕੁੰਜੀ_type_ਨਾਂ_iter;
        uint8_t wire_ਅੰਕ_ਪੱਧਰ = *n_ਪੱਧਰ_ਪ੍ਰਤੀ_type_iter;
        ਬਣਤਰ xkb_ਕੁੰਜੀ_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->ਅੰਕ_ਪੱਧਰ >= wire_ਅੰਕ_ਪੱਧਰ);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_ਨਾਂ, type->ਅੰਕ_ਪੱਧਰ);
        type->ਅੰਕ_level_ਨਾਂ = type->ਅੰਕ_ਪੱਧਰ;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        ਲਈ (ਮਾਪ_t j = 0; j < wire_ਅੰਕ_ਪੱਧਰ; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_ਨਾਂ_iter[j],
                                         &type->level_ਨਾਂ[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        ਲਈ (ਮਾਪ_t j = wire_ਅੰਕ_ਪੱਧਰ; j < type->ਅੰਕ_ਪੱਧਰ; j++)
            type->level_ਨਾਂ[j] = XKB_ATOM_NONE;

        kt_level_ਨਾਂ_iter += wire_ਅੰਕ_ਪੱਧਰ;
        ਕੁੰਜੀ_type_ਨਾਂ_iter++;
        n_ਪੱਧਰ_ਪ੍ਰਤੀ_type_iter++;
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_indicator_ਨਾਂ(ਬਣਤਰ xkb_keymap *keymap,
                    ਬਣਤਰ x11_atom_interner *interner,
                    xcb_xkb_get_ਨਾਂ_ਜਵਾਬ_t *ਜਵਾਬ,
                    xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_indicator_ਨਾਂ(list);

    FAIL_UNLESS(msb_pos(ਜਵਾਬ->indicators) <= keymap->ਅੰਕ_leds);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_INDICATORS; i++) {
        ਜੇ (ਜਵਾਬ->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            ਬਣਤਰ xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_vmod_ਨਾਂ(ਬਣਤਰ xkb_keymap *keymap, ਬਣਤਰ x11_atom_interner *interner,
               xcb_xkb_get_ਨਾਂ_ਜਵਾਬ_t *ਜਵਾਬ,
               xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_virtual_mod_ਨਾਂ(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.ਅੰਕ_mods =
        ਅੰਕ_REAL_MODS + MIN(msb_pos(ਜਵਾਬ->virtualMods), ਅੰਕ_VMODS);
    FAIL_UNLESS(keymap->mods.ਅੰਕ_mods <= XKB_MAX_MODS);

    static_assert(ਅੰਕ_REAL_MODS + ਅੰਕ_VMODS <= XKB_MAX_MODS, "");
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_VMODS; i++) {
        ਜੇ (ਜਵਾਬ->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            ਬਣਤਰ xkb_mod *mod = &keymap->mods.mods[ਅੰਕ_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_ਸਮੂਹ_ਨਾਂ(ਬਣਤਰ xkb_keymap *keymap, ਬਣਤਰ x11_atom_interner *interner,
                xcb_xkb_get_ਨਾਂ_ਜਵਾਬ_t *ਜਵਾਬ,
                xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_t *list)
{
    ਪੂਰਨ ਲੰਮਾਈ = xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_groups_ਲੰਮਾਈ(ਜਵਾਬ, list);
    xcb_atom_t *iter = xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_groups(list);

    keymap->ਅੰਕ_ਸਮੂਹ_ਨਾਂ = msb_pos(ਜਵਾਬ->ਸਮੂਹ_ਨਾਂ_2);
    ALLOC_OR_FAIL(keymap->ਸਮੂਹ_ਨਾਂ, keymap->ਅੰਕ_ਸਮੂਹ_ਨਾਂ);

    ਲਈ (ਪੂਰਨ i = 0; i < ਲੰਮਾਈ; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->ਸਮੂਹ_ਨਾਂ[i]);
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_ਕੁੰਜੀ_ਨਾਂ(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_ਨਾਂ_ਜਵਾਬ_t *ਜਵਾਬ,
              xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_t *list)
{
    ਪੂਰਨ ਲੰਮਾਈ = xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_ਕੁੰਜੀ_ਨਾਂ_ਲੰਮਾਈ(ਜਵਾਬ, list);
    xcb_xkb_ਕੁੰਜੀ_name_iterator_t iter =
        xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_ਕੁੰਜੀ_ਨਾਂ_iterator(ਜਵਾਬ, list);

    FAIL_UNLESS(ਜਵਾਬ->min_ਕੁੰਜੀ_ਕੋਡ_2 == keymap->min_ਕੁੰਜੀ_ਕੋਡ);
    FAIL_UNLESS(ਜਵਾਬ->max_ਕੁੰਜੀ_ਕੋਡ_2 == keymap->max_ਕੁੰਜੀ_ਕੋਡ);
    FAIL_UNLESS(ਜਵਾਬ->ਪਹਿਲਾ_ਕੁੰਜੀ == keymap->min_ਕੁੰਜੀ_ਕੋਡ);
    FAIL_UNLESS(ਜਵਾਬ->ਪਹਿਲਾ_ਕੁੰਜੀ + ਜਵਾਬ->n_ਕੁੰਜੀਆਂ - 1U == keymap->max_ਕੁੰਜੀ_ਕੋਡ);

    ਲਈ (ਪੂਰਨ i = 0; i < ਲੰਮਾਈ; i++) {
        xcb_xkb_ਕੁੰਜੀ_name_t *wire = iter.ਅੰਕੜੇ;
        xkb_atom_t *ਕੁੰਜੀ_name = &keymap->ਕੁੰਜੀਆਂ[ਜਵਾਬ->ਪਹਿਲਾ_ਕੁੰਜੀ + i].name;

        ਜੇ (wire->name[0] == '\0') {
            *ਕੁੰਜੀ_name = XKB_ATOM_NONE;
        }
        ਨਹੀਂਤਾਂ {
            *ਕੁੰਜੀ_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_ਕੁੰਜੀ_NAME_ਲੰਮਾਈ));
            ਜੇ (!*ਕੁੰਜੀ_name)
                ਵਾਪਸ false;
        }

        xcb_xkb_ਕੁੰਜੀ_name_ਅਗਲਾ(&iter);
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_aliases(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_ਨਾਂ_ਜਵਾਬ_t *ਜਵਾਬ,
            xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_t *list)
{
    ਪੂਰਨ ਲੰਮਾਈ = xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_ਕੁੰਜੀ_aliases_ਲੰਮਾਈ(ਜਵਾਬ, list);
    xcb_xkb_ਕੁੰਜੀ_alias_iterator_t iter =
        xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_ਕੁੰਜੀ_aliases_iterator(ਜਵਾਬ, list);

    keymap->ਅੰਕ_ਕੁੰਜੀ_aliases = ਜਵਾਬ->n_ਕੁੰਜੀ_Aliases;
    ALLOC_OR_FAIL(keymap->ਕੁੰਜੀ_aliases, keymap->ਅੰਕ_ਕੁੰਜੀ_aliases);

    ਲਈ (ਪੂਰਨ i = 0; i < ਲੰਮਾਈ; i++) {
        xcb_xkb_ਕੁੰਜੀ_alias_t *wire = iter.ਅੰਕੜੇ;
        ਬਣਤਰ xkb_ਕੁੰਜੀ_alias *alias = &keymap->ਕੁੰਜੀ_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_ਕੁੰਜੀ_NAME_ਲੰਮਾਈ));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_ਕੁੰਜੀ_NAME_ਲੰਮਾਈ));
        ਜੇ (!alias->real || !alias->alias)
            ਜਾਓ fail;

        xcb_xkb_ਕੁੰਜੀ_alias_ਅਗਲਾ(&iter);
    }

    ਵਾਪਸ true;

fail:
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_ਨਾਂ(ਬਣਤਰ xkb_keymap *keymap, ਬਣਤਰ x11_atom_interner *interner,
          xcb_xkb_get_ਨਾਂ_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_ਨਾਂ_ਜਵਾਬ_t *ਜਵਾਬ =
        xcb_xkb_get_ਨਾਂ_ਜਵਾਬ(conn, cookie, ਨਿਰਾ);
    xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_t list;

    FAIL_IF_BAD_ਜਵਾਬ(ਜਵਾਬ, "XkbGetNames");

    FAIL_UNLESS((ਜਵਾਬ->which & get_ਨਾਂ_required) == get_ਨਾਂ_required);

    xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list_unpack(xcb_xkb_get_ਨਾਂ_ਮੁੱਲ_list(ਜਵਾਬ),
                                        ਜਵਾਬ->nTypes,
                                        ਜਵਾਬ->indicators,
                                        ਜਵਾਬ->virtualMods,
                                        ਜਵਾਬ->ਸਮੂਹ_ਨਾਂ_2,
                                        ਜਵਾਬ->n_ਕੁੰਜੀਆਂ,
                                        ਜਵਾਬ->n_ਕੁੰਜੀ_Aliases,
                                        ਜਵਾਬ->nRadioGroups,
                                        ਜਵਾਬ->which,
                                        &list);

    x11_atom_interner_get_escaped_atom_name(interner, list.keycodesName,
                                            &keymap->keycodes_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.symbolsName,
                                            &keymap->symbols_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.typesName,
                                            &keymap->types_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.compatName,
                                            &keymap->compat_section_name);
    ਜੇ (!get_type_ਨਾਂ(keymap, interner, ਜਵਾਬ, &list) ||
        !get_indicator_ਨਾਂ(keymap, interner, ਜਵਾਬ, &list) ||
        !get_vmod_ਨਾਂ(keymap, interner, ਜਵਾਬ, &list) ||
        !get_ਸਮੂਹ_ਨਾਂ(keymap, interner, ਜਵਾਬ, &list) ||
        !get_ਕੁੰਜੀ_ਨਾਂ(keymap, conn, ਜਵਾਬ, &list) ||
        !get_aliases(keymap, conn, ਜਵਾਬ, &list))
        ਜਾਓ fail;

    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ true;

fail:
    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ false;
}

ਸਥਿਰ bool
get_controls(ਬਣਤਰ xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_ਜਵਾਬ_t *ਜਵਾਬ =
        xcb_xkb_get_controls_ਜਵਾਬ(conn, cookie, ਨਿਰਾ);

    FAIL_IF_BAD_ਜਵਾਬ(ਜਵਾਬ, "XkbGetControls");
    FAIL_UNLESS(ਜਵਾਬ->ਅੰਕ_Groups > 0 && ਜਵਾਬ->ਅੰਕ_Groups <= 4);

    keymap->ਅੰਕ_groups = ਜਵਾਬ->ਅੰਕ_Groups;

    FAIL_UNLESS(keymap->max_ਕੁੰਜੀ_ਕੋਡ < XCB_XKB_CONST_ਪ੍ਰਤੀ_ਕੁੰਜੀ_ਦੋਆਧਾਰੀ_ਅੰਕ_ARRAY_ਮਾਪ * 8);

    ਲਈ (xkb_keycode_t i = keymap->min_ਕੁੰਜੀ_ਕੋਡ; i <= keymap->max_ਕੁੰਜੀ_ਕੋਡ; i++)
        keymap->ਕੁੰਜੀਆਂ[i].repeats = (ਜਵਾਬ->ਪ੍ਰਤੀ_ਕੁੰਜੀ_Repeat[i / 8] & (1u << (i % 8)));

    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ true;

fail:
    ਮੁਕਤ(ਜਵਾਬ);
    ਵਾਪਸ false;
}

ਬਣਤਰ xkb_keymap *
xkb_x11_keymap_new_from_device(ਬਣਤਰ xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               ਗਿਣਤੀ xkb_keymap_compile_flags flags)
{
    ਜੇ (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਅਚਲ ਗਿਣਤੀ xkb_keymap_format format = XKB_KEYMAP_FORMAT_ਪਾਠ_V1;
    ਬਣਤਰ xkb_keymap * ਅਚਲ keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    ਜੇ (!keymap)
        ਵਾਪਸ ਨਿਰਾ;

    keymap->redirect_ਕੁੰਜੀ_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    ਬਣਤਰ x11_atom_interner interner;
    x11_atom_interner_init(&interner, ctx, conn);

    /*
     * Send all requests together so only one roundtrip is needed
     * to get the replies.
     */
    xcb_xkb_get_map_cookie_t map_cookie =
        xcb_xkb_get_map(conn, device_id, get_map_required_components,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    xcb_xkb_get_indicator_map_cookie_t indicator_map_cookie =
        xcb_xkb_get_indicator_map(conn, device_id, ALL_INDICATORS_ਨਕਾਬ);
    xcb_xkb_get_compat_map_cookie_t compat_map_cookie =
        xcb_xkb_get_compat_map(conn, device_id, 0, true, 0, 0);
    xcb_xkb_get_ਨਾਂ_cookie_t ਨਾਂ_cookie =
        xcb_xkb_get_ਨਾਂ(conn, device_id, get_ਨਾਂ_wanted);
    xcb_xkb_get_controls_cookie_t controls_cookie =
        xcb_xkb_get_controls(conn, device_id);

    ਜੇ (!get_map(keymap, conn, map_cookie))
        ਜਾਓ err_map;
    ਜੇ (!get_indicator_map(keymap, conn, indicator_map_cookie))
        ਜਾਓ err_indicator_map;
    ਜੇ (!get_compat_map(keymap, conn, compat_map_cookie))
        ਜਾਓ err_compat_map;
    ਜੇ (!get_ਨਾਂ(keymap, &interner, ਨਾਂ_cookie))
        ਜਾਓ err_ਨਾਂ;
    ਜੇ (!get_controls(keymap, conn, controls_cookie))
        ਜਾਓ err_controls;
    x11_atom_interner_ਗੇੜਾ_trip(&interner);
    ਜੇ (interner.had_error)
        ਜਾਓ err_interner;

    ਵਾਪਸ keymap;

err_map:
    xcb_discard_ਜਵਾਬ(conn, indicator_map_cookie.sequence);
err_indicator_map:
    xcb_discard_ਜਵਾਬ(conn, compat_map_cookie.sequence);
err_compat_map:
    xcb_discard_ਜਵਾਬ(conn, ਨਾਂ_cookie.sequence);
err_ਨਾਂ:
    xcb_discard_ਜਵਾਬ(conn, controls_cookie.sequence);
err_controls:
    x11_atom_interner_ਗੇੜਾ_trip(&interner);
err_interner:
    xkb_keymap_unref(keymap);
    ਵਾਪਸ ਨਿਰਾ;
}
