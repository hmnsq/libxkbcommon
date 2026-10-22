/*
 * Copyright © 2013 Ran Benita
 * SPDX-License-Identifier: MIT
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <xcb/xkb.h>

#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-keysyms.h"
#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "x11-priv.h"

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
#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಕೆ_REAL_MODS 8u
/* XkbNumVirtualMods. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಕೆ_VMODS 16u
/* XkbNoModifier. */
#ವ್ಯಾಖ್ಯಾನಿಸು NO_MODIFIER 0xff
/* XkbNumIndicators. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಕೆ_INDICATORS 32u
/* XkbAllIndicatorsMask. */
#ವ್ಯಾಖ್ಯಾನಿಸು ALL_INDICATORS_ಮುಖವಾಡ 0xffffffff

/* Some macros. Not very nice but it'd be worse without them. */

/*
 * We try not to trust the server too much and be paranoid. If we get
 * something which we definitely shouldn't, we fail.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು FAIL_UNLESS(ಅಭಿವ್ಯಕ್ತಿ) ಮಾಡು {                              \
    ಆದರೆ (!(ಅಭಿವ್ಯಕ್ತಿ)) {                                          \
        log_err(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "unmet condition in %s(): %s\n",            \
                __func__, STRINGIFY(ಅಭಿವ್ಯಕ್ತಿ));                 \
        ಹೋಗು fail;                                          \
    }                                                       \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು FAIL_IF_BAD_ಉತ್ತರ(ಉತ್ತರ, request_name) ಮಾಡು {         \
    ಆದರೆ (!(ಉತ್ತರ)) {                                           \
        log_err(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,         \
                "x11: failed to get keymap from X server: " \
                "%s request failed\n",                      \
                (request_name));                            \
        ಹೋಗು fail;                                          \
    }                                                       \
} ಆಗ (0)

#ವ್ಯಾಖ್ಯಾನಿಸು ALLOC_OR_FAIL(arr, nmemb) ಮಾಡು {                      \
    ಆದರೆ ((nmemb) > 0) {                                      \
        (arr) = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು((nmemb), ಗಾತ್ರ(*(arr)));            \
        ಆದರೆ (!(arr))                                         \
            ಹೋಗು fail;                                      \
    }                                                       \
} ಆಗ (0)

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ xcb_xkb_map_part_t get_map_required_components =
    (XCB_XKB_MAP_PART_ಕೀಲಿ_TYPES |
     XCB_XKB_MAP_PART_ಕೀಲಿ_SYMS |
     XCB_XKB_MAP_PART_MODIFIER_MAP |
     XCB_XKB_MAP_PART_EXPLICIT_COMPONENTS |
     XCB_XKB_MAP_PART_ಕೀಲಿ_ACTIONS |
     XCB_XKB_MAP_PART_VIRTUAL_MODS |
     XCB_XKB_MAP_PART_VIRTUAL_MOD_MAP);

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ xcb_xkb_name_detail_t get_ಹೆಸರುಗಳು_wanted =
    (XCB_XKB_NAME_DETAIL_KEYCODES |
     XCB_XKB_NAME_DETAIL_SYMBOLS |
     XCB_XKB_NAME_DETAIL_TYPES |
     XCB_XKB_NAME_DETAIL_COMPAT |
     XCB_XKB_NAME_DETAIL_ಕೀಲಿ_TYPE_ಹೆಸರುಗಳು |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_ಹೆಸರುಗಳು |
     XCB_XKB_NAME_DETAIL_INDICATOR_ಹೆಸರುಗಳು |
     XCB_XKB_NAME_DETAIL_ಕೀಲಿ_ಹೆಸರುಗಳು |
     XCB_XKB_NAME_DETAIL_ಕೀಲಿ_ALIASES |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_ಹೆಸರುಗಳು |
     XCB_XKB_NAME_DETAIL_ಗುಂಪು_ಹೆಸರುಗಳು);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ xcb_xkb_name_detail_t get_ಹೆಸರುಗಳು_required =
    (XCB_XKB_NAME_DETAIL_ಕೀಲಿ_TYPE_ಹೆಸರುಗಳು |
     XCB_XKB_NAME_DETAIL_KT_LEVEL_ಹೆಸರುಗಳು |
     XCB_XKB_NAME_DETAIL_ಕೀಲಿ_ಹೆಸರುಗಳು |
     XCB_XKB_NAME_DETAIL_VIRTUAL_MOD_ಹೆಸರುಗಳು);


ಸ್ಥಿರ xkb_mod_ಮುಖವಾಡ_t
translate_mods(uint8_t rmods, uint16_t vmods_ಕೆಳಗಿನ, uint16_t vmods_ಮೇಲಿನ)
{
    /* We represent mod masks in a single uint32_t value, with real mods
     * first and vmods after (though we don't make these distinctions). */
    ಹಿಂತಿರುಗಿ
        ((xkb_mod_ಮುಖವಾಡ_t) rmods) |
        ((xkb_mod_ಮುಖವಾಡ_t) vmods_ಕೆಳಗಿನ << 8) |
        ((xkb_mod_ಮುಖವಾಡ_t) vmods_ಮೇಲಿನ << 16);
}

ಎಣಿಕೆ xkb_action_controls
translate_controls_ಮುಖವಾಡ(uint32_t wire)
{
    ಎಣಿಕೆ xkb_action_controls ret = 0;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_REPEAT_ಕೀಲಿಗಳು)
        ret |= CONTROL_REPEAT;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_SLOW_ಕೀಲಿಗಳು)
        ret |= CONTROL_SLOW;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_BOUNCE_ಕೀಲಿಗಳು)
        ret |= CONTROL_DEBOUNCE;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_STICKY_ಕೀಲಿಗಳು)
        ret |= CONTROL_STICKY_ಕೀಲಿಗಳು;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_MOUSE_ಕೀಲಿಗಳು)
        ret |= CONTROL_MOUSE_ಕೀಲಿಗಳು;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_MOUSE_ಕೀಲಿಗಳು_ACCEL)
        ret |= CONTROL_MOUSE_ಕೀಲಿಗಳು_ACCEL;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_ಕೀಲಿಗಳು)
        ret |= CONTROL_AX;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_TIMEOUT_ಮುಖವಾಡ)
        ret |= CONTROL_AX_TIMEOUT;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_ACCESS_X_FEEDBACK_ಮುಖವಾಡ)
        ret |= CONTROL_AX_FEEDBACK;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_AUDIBLE_BELL_ಮುಖವಾಡ)
        ret |= CONTROL_BELL;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_1_ಮುಖವಾಡ)
        ret |= CONTROL_OVERLAY1;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_OVERLAY_2_ಮುಖವಾಡ)
        ret |= CONTROL_OVERLAY2;
    ಆದರೆ (wire & XCB_XKB_BOOL_CTRL_IGNORE_ಗುಂಪು_ಬೀಗ_ಮುಖವಾಡ)
        ret |= CONTROL_IGNORE_ಗುಂಪು_ಬೀಗ;
    /* Some controls are not supported and don't appear here. */
    ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ bool
translate_action(ಒಕ್ಕೂಟ xkb_action *action, ಸ್ಥಿರಾಂಕ xcb_xkb_action_t *wire,
                 xkb_keycode_t min_ಕೀಲಿ_ಸಂಕೇತ, xkb_keycode_t max_ಕೀಲಿ_ಸಂಕೇತ)
{
    ಆಯ್ಕೆ (wire->type) {
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ಗಣ_MODS:
        action->type = ACTION_TYPE_MOD_ಗಣ;
        action->mods.mods.mods = translate_mods(wire->setmods.realMods,
                                                wire->setmods.vmods_ಕೆಳಗಿನ_2,
                                                wire->setmods.vmods_ಮೇಲಿನ_2);
        action->mods.mods.ಮುಖವಾಡ = translate_mods(wire->setmods.ಮುಖವಾಡ, 0, 0);

        ಆದರೆ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_ಬೀಗ_CLEAR;
        ಆದರೆ (wire->setmods.flags & XCB_XKB_SA_LATCH_ಗಮ್ಯ_ಬೀಗ)
            action->mods.flags |= ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ;
        ಆದರೆ (wire->setmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_LATCH_MODS:
        action->type = ACTION_TYPE_MOD_LATCH;

        action->mods.mods.mods = translate_mods(wire->latchmods.realMods,
                                                wire->latchmods.vmods_ಕೆಳಗಿನ_2,
                                                wire->latchmods.vmods_ಮೇಲಿನ_2);
        action->mods.mods.ಮುಖವಾಡ = translate_mods(wire->latchmods.ಮುಖವಾಡ, 0, 0);

        ಆದರೆ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->mods.flags |= ACTION_ಬೀಗ_CLEAR;
        ಆದರೆ (wire->latchmods.flags & XCB_XKB_SA_LATCH_ಗಮ್ಯ_ಬೀಗ)
            action->mods.flags |= ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ;
        ಆದರೆ (wire->latchmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ಬೀಗ_MODS:
        action->type = ACTION_TYPE_MOD_ಬೀಗ;

        action->mods.mods.mods = translate_mods(wire->lockmods.realMods,
                                                wire->lockmods.vmods_ಕೆಳಗಿನ_2,
                                                wire->lockmods.vmods_ಮೇಲಿನ_2);
        action->mods.mods.ಮುಖವಾಡ = translate_mods(wire->lockmods.ಮುಖವಾಡ, 0, 0);

        ಆದರೆ (wire->lockmods.flags & XCB_XKB_SA_ISO_ಬೀಗ_FLAG_NO_ಬೀಗ)
            action->mods.flags |= ACTION_ಬೀಗ_NO_ಬೀಗ;
        ಆದರೆ (wire->lockmods.flags & XCB_XKB_SA_ISO_ಬೀಗ_FLAG_NO_UNLOCK)
            action->mods.flags |= ACTION_ಬೀಗ_NO_UNLOCK;
        ಆದರೆ (wire->lockmods.flags & XCB_XKB_SA_USE_MOD_MAP_MODS)
            action->mods.flags |= ACTION_MODS_LOOKUP_MODMAP;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ಗಣ_ಗುಂಪು:
        action->type = ACTION_TYPE_ಗುಂಪು_ಗಣ;

        action->ಗುಂಪು.ಗುಂಪು = (int32_t)wire->setgroup.ಗುಂಪು;

        ಆದರೆ (wire->setmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->ಗುಂಪು.flags |= ACTION_ಬೀಗ_CLEAR;
        ಆದರೆ (wire->setmods.flags & XCB_XKB_SA_LATCH_ಗಮ್ಯ_ಬೀಗ)
            action->ಗುಂಪು.flags |= ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ;
        ಆದರೆ (wire->setmods.flags & XCB_XKB_SA_ISO_ಬೀಗ_FLAG_ಗುಂಪು_ABSOLUTE)
            action->ಗುಂಪು.flags |= ACTION_ABSOLUTE_SWITCH;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_LATCH_ಗುಂಪು:
        action->type = ACTION_TYPE_ಗುಂಪು_LATCH;

        action->ಗುಂಪು.ಗುಂಪು = (int32_t)wire->latchgroup.ಗುಂಪು;

        ಆದರೆ (wire->latchmods.flags & XCB_XKB_SA_CLEAR_LOCKS)
            action->ಗುಂಪು.flags |= ACTION_ಬೀಗ_CLEAR;
        ಆದರೆ (wire->latchmods.flags & XCB_XKB_SA_LATCH_ಗಮ್ಯ_ಬೀಗ)
            action->ಗುಂಪು.flags |= ACTION_LATCH_ಗಮ್ಯ_ಬೀಗ;
        ಆದರೆ (wire->latchmods.flags & XCB_XKB_SA_ISO_ಬೀಗ_FLAG_ಗುಂಪು_ABSOLUTE)
            action->ಗುಂಪು.flags |= ACTION_ABSOLUTE_SWITCH;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ಬೀಗ_ಗುಂಪು:
        action->type = ACTION_TYPE_ಗುಂಪು_ಬೀಗ;

        action->ಗುಂಪು.ಗುಂಪು = (int32_t)wire->lockgroup.ಗುಂಪು;

        ಆದರೆ (wire->lockgroup.flags & XCB_XKB_SA_ISO_ಬೀಗ_FLAG_ಗುಂಪು_ABSOLUTE)
            action->ಗುಂಪು.flags |= ACTION_ABSOLUTE_SWITCH;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_MOVE_PTR:
        action->type = ACTION_TYPE_PTR_MOVE;

        action->ptr.x = (int16_t) (wire->moveptr.x_ಕೆಳಗಿನ | ((uint16_t) wire->moveptr.x_ಮೇಲಿನ << 8));
        action->ptr.y = (int16_t) (wire->moveptr.y_ಕೆಳಗಿನ | ((uint16_t) wire->moveptr.y_ಮೇಲಿನ << 8));

        ಆದರೆ (!(wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_NO_ACCELERATION))
            action->ptr.flags |= ACTION_REPEAT;
        ಆದರೆ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_X)
            action->ptr.flags |= ACTION_ABSOLUTE_X;
        ಆದರೆ (wire->moveptr.flags & XCB_XKB_SA_MOVE_PTR_FLAG_MOVE_ABSOLUTE_Y)
            action->ptr.flags |= ACTION_ABSOLUTE_Y;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_PTR_BTN:
        action->type = ACTION_TYPE_PTR_BUTTON;

        action->btn.ಎಣಿಕೆ_2 = wire->ptrbtn.ಎಣಿಕೆ_2;
        action->btn.button = wire->ptrbtn.button;
        action->btn.flags = 0;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ಬೀಗ_PTR_BTN:
        action->type = ACTION_TYPE_PTR_ಬೀಗ;

        action->btn.button = wire->lockptrbtn.button;
        action->btn.ಎಣಿಕೆ_2 = 0;

        ಆದರೆ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_ಬೀಗ_FLAG_NO_ಬೀಗ)
            action->btn.flags |= ACTION_ಬೀಗ_NO_ಬೀಗ;
        ಆದರೆ (wire->lockptrbtn.flags & XCB_XKB_SA_ISO_ಬೀಗ_FLAG_NO_UNLOCK)
            action->btn.flags |= ACTION_ಬೀಗ_NO_UNLOCK;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ಗಣ_PTR_DFLT:
        action->type = ACTION_TYPE_PTR_DEFAULT;

        action->dflt.ಮೌಲ್ಯ = wire->setptrdflt.ಮೌಲ್ಯ;

        ಆದರೆ (wire->setptrdflt.flags & XCB_XKB_SA_ಗಣ_PTR_DFLT_FLAG_DFLT_BTN_ABSOLUTE)
            action->dflt.flags |= ACTION_ABSOLUTE_SWITCH;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_TERMINATE:
        action->type = ACTION_TYPE_TERMINATE;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_SWITCH_SCREEN:
        action->type = ACTION_TYPE_SWITCH_VT;

        action->screen.screen = wire->switchscreen.newScreen;

        ಆದರೆ (!(wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_APPLICATION))
            action->screen.flags |= ACTION_SAME_SCREEN;
        ಆದರೆ (wire->switchscreen.flags & XCB_XKB_SWITCH_SCREEN_FLAG_ABSOLUTE)
            action->screen.flags |= ACTION_ABSOLUTE_SWITCH;

        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ಗಣ_CONTROLS:
        action->type = ACTION_TYPE_CTRL_ಗಣ;
        {
            ಸ್ಥಿರಾಂಕ uint16_t ಮುಖವಾಡ = (wire->setcontrols.bool_Ctrls_ಕೆಳಗಿನ |
                                   (wire->setcontrols.bool_Ctrls_ಮೇಲಿನ << 8));
            action->ctrls.ctrls = translate_controls_ಮುಖವಾಡ(ಮುಖವಾಡ);
        }
        ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ಬೀಗ_CONTROLS:
        action->type = ACTION_TYPE_CTRL_ಬೀಗ;
        {
            ಸ್ಥಿರಾಂಕ uint16_t ಮುಖವಾಡ = (wire->lockcontrols.bool_Ctrls_ಕೆಳಗಿನ |
                                   (wire->lockcontrols.bool_Ctrls_ಮೇಲಿನ << 8));
            action->ctrls.ctrls = translate_controls_ಮುಖವಾಡ(ಮುಖವಾಡ);
        }
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_REDIRECT_ಕೀಲಿ:
        action->type = ACTION_TYPE_REDIRECT_ಕೀಲಿ;
        ಆದರೆ (wire->redirect.newkey < min_ಕೀಲಿ_ಸಂಕೇತ ||
            wire->redirect.newkey > max_ಕೀಲಿ_ಸಂಕೇತ)
                ಹಿಂತಿರುಗಿ false;
        action->redirect.keycode = wire->redirect.newkey;
        /*
         * WARNING: there is a bug in Xorg that swap the low and high
         * vmod values. Real modifiers are fine though. See:
         * https://gitlab.freedesktop.org/xorg/proto/xorgproto/-/merge_requests/105
         */
        action->redirect.affect_mods = translate_mods(
            wire->redirect.ಮುಖವಾಡ,
            wire->redirect.vmods_ಮುಖವಾಡ_ಕೆಳಗಿನ,
            wire->redirect.vmods_ಮುಖವಾಡ_ಮೇಲಿನ
        );
        action->redirect.mods = translate_mods(wire->redirect.realModifiers,
                                               wire->redirect.vmods_ಕೆಳಗಿನ_2,
                                               wire->redirect.vmods_ಮೇಲಿನ_2);
        ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_NO_ACTION:
        action->type = ACTION_TYPE_NONE;
        ನಿಲ್ಲಿಸು;

    /* We don't support these. */
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ISO_ಬೀಗ:
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ACTION_ಸಂದೇಶ:
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_DEVICE_BTN:
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_ಬೀಗ_DEVICE_BTN:
    ಸ್ಥಿತಿ XCB_XKB_SA_TYPE_DEVICE_VALUATOR:
        action->type = ACTION_TYPE_UNSUPPORTED_LEGACY;
        ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_ಆಂತರಿಕ == 20 &&
                      ACTION_TYPE_ಆಂತರಿಕ + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
                      "Missing action type");

        ಆದರೆ (wire->type < ACTION_TYPE_ಖಾಸಗಿ) {
            action->type = ACTION_TYPE_NONE;
            ನಿಲ್ಲಿಸು;
        }

        /* Treat high unknown actions as Private actions. */
        action->priv.type = wire->noaction.type;
        static_assert(ಗಾತ್ರ(action->priv.ದತ್ತಾಂಶ) == 7 &&
                      ಗಾತ್ರ(wire->noaction.pad0) == 7,
                      "The private action data must be 7 bytes long!");
        ಸ್ಮರಣೆ_ನಕಲು(action->priv.ದತ್ತಾಂಶ, wire->noaction.pad0, 7);
        ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
get_types(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_ಉತ್ತರ_t *ಉತ್ತರ, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಂಕ types_ಉದ್ದಳತೆ = xcb_xkb_get_map_map_types_rtrn_ಉದ್ದಳತೆ(ಉತ್ತರ, map);
    xcb_xkb_ಕೀಲಿ_type_iterator_t types_iter =
        xcb_xkb_get_map_map_types_rtrn_iterator(ಉತ್ತರ, map);

    FAIL_UNLESS(ಉತ್ತರ->ಮೊದಲ_Type == 0);

    keymap->ಅಂಕೆ_types = ಉತ್ತರ->nTypes;
    ALLOC_OR_FAIL(keymap->types, keymap->ಅಂಕೆ_types);
    ಪ್ರತಿಜ್ಞಾ(types_ಉದ್ದಳತೆ == (ಪೂರ್ಣಾಂಕ) keymap->ಅಂಕೆ_types);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < types_ಉದ್ದಳತೆ; i++) {
        xcb_xkb_ಕೀಲಿ_type_t *wire_type = types_iter.ದತ್ತಾಂಶ;
        ರಚನೆ xkb_ಕೀಲಿ_type *type = &keymap->types[i];

        FAIL_UNLESS(wire_type->ಅಂಕೆ_ಮಟ್ಟಗಳು_2 > 0);

        type->mods.mods = translate_mods(wire_type->mods_mods,
                                         wire_type->mods_vmods, 0);
        type->mods.ಮುಖವಾಡ = translate_mods(wire_type->mods_ಮುಖವಾಡ, 0, 0);
        type->ಅಂಕೆ_ಮಟ್ಟಗಳು = wire_type->ಅಂಕೆ_ಮಟ್ಟಗಳು_2;

        {
            ಪೂರ್ಣಾಂಕ entries_ಉದ್ದಳತೆ = xcb_xkb_ಕೀಲಿ_type_map_ಉದ್ದಳತೆ(wire_type);
            xcb_xkb_kt_map_ನಮೂದು_iterator_t entries_iter =
                xcb_xkb_ಕೀಲಿ_type_map_iterator(wire_type);

            type->ಅಂಕೆ_entries = wire_type->nMapEntries;
            ALLOC_OR_FAIL(type->entries, type->ಅಂಕೆ_entries);
            ಪ್ರತಿಜ್ಞಾ(entries_ಉದ್ದಳತೆ == (ಪೂರ್ಣಾಂಕ) type->ಅಂಕೆ_entries);

            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < entries_ಉದ್ದಳತೆ; j++) {
                xcb_xkb_kt_map_ನಮೂದು_t *wire_ನಮೂದು = entries_iter.ದತ್ತಾಂಶ;
                ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *ನಮೂದು = &type->entries[j];

                FAIL_UNLESS(wire_ನಮೂದು->level < type->ಅಂಕೆ_ಮಟ್ಟಗಳು);

                ನಮೂದು->level = wire_ನಮೂದು->level;
                ನಮೂದು->mods.mods = translate_mods(wire_ನಮೂದು->mods_mods,
                                                  wire_ನಮೂದು->mods_vmods, 0);
                ನಮೂದು->mods.ಮುಖವಾಡ = translate_mods(wire_ನಮೂದು->mods_ಮುಖವಾಡ, 0, 0);

                xcb_xkb_kt_map_ನಮೂದು_ಮುಂದಿನ(&entries_iter);
            }
        }

        {
            ಪೂರ್ಣಾಂಕ preserves_ಉದ್ದಳತೆ = xcb_xkb_ಕೀಲಿ_type_preserve_ಉದ್ದಳತೆ(wire_type);
            xcb_xkb_mod_def_iterator_t preserves_iter =
                xcb_xkb_ಕೀಲಿ_type_preserve_iterator(wire_type);

            FAIL_UNLESS((ಚಿಹ್ನೆರಹಿತ) preserves_ಉದ್ದಳತೆ <= type->ಅಂಕೆ_entries);

            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < preserves_ಉದ್ದಳತೆ; j++) {
                xcb_xkb_mod_def_t *wire_preserve = preserves_iter.ದತ್ತಾಂಶ;
                ರಚನೆ xkb_ಕೀಲಿ_type_ನಮೂದು *ನಮೂದು = &type->entries[j];

                ನಮೂದು->preserve.mods = translate_mods(wire_preserve->realMods,
                                                      wire_preserve->vmods, 0);
                ನಮೂದು->preserve.ಮುಖವಾಡ = translate_mods(wire_preserve->ಮುಖವಾಡ, 0, 0);

                xcb_xkb_mod_def_ಮುಂದಿನ(&preserves_iter);
            }
        }

        /* Checked only when compiling a keymap from text */
        type->required = true;

        xcb_xkb_ಕೀಲಿ_type_ಮುಂದಿನ(&types_iter);
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_sym_maps(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_ಉತ್ತರ_t *ಉತ್ತರ, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಂಕ sym_maps_ಉದ್ದಳತೆ = xcb_xkb_get_map_map_syms_rtrn_ಉದ್ದಳತೆ(ಉತ್ತರ, map);
    xcb_xkb_ಕೀಲಿ_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(ಉತ್ತರ, map);

    FAIL_UNLESS(ಉತ್ತರ->min_ಕೀಲಿ_ಸಂಕೇತ_2 <= ಉತ್ತರ->max_ಕೀಲಿ_ಸಂಕೇತ_2);
    FAIL_UNLESS(ಉತ್ತರ->ಮೊದಲ_ಕೀಲಿ_Sym >= ಉತ್ತರ->min_ಕೀಲಿ_ಸಂಕೇತ_2);
    FAIL_UNLESS(ಉತ್ತರ->ಮೊದಲ_ಕೀಲಿ_Sym + ಉತ್ತರ->n_ಕೀಲಿ_Syms <= ಉತ್ತರ->max_ಕೀಲಿ_ಸಂಕೇತ_2 + 1);

    keymap->min_ಕೀಲಿ_ಸಂಕೇತ = ಉತ್ತರ->min_ಕೀಲಿ_ಸಂಕೇತ_2;
    keymap->max_ಕೀಲಿ_ಸಂಕೇತ = ಉತ್ತರ->max_ಕೀಲಿ_ಸಂಕೇತ_2;
    keymap->ಅಂಕೆ_ಕೀಲಿಗಳು = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ = ಉತ್ತರ->max_ಕೀಲಿ_ಸಂಕೇತ_2 + 1;

    ALLOC_OR_FAIL(keymap->ಕೀಲಿಗಳು, keymap->max_ಕೀಲಿ_ಸಂಕೇತ + 1);

    ಗಾಗಿ (xkb_keycode_t kc = keymap->min_ಕೀಲಿ_ಸಂಕೇತ; kc <= keymap->max_ಕೀಲಿ_ಸಂಕೇತ; kc++)
        keymap->ಕೀಲಿಗಳು[kc].keycode = kc;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < sym_maps_ಉದ್ದಳತೆ; i++) {
        xcb_xkb_ಕೀಲಿ_sym_map_t *wire_sym_map = sym_maps_iter.ದತ್ತಾಂಶ;
        ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[ಉತ್ತರ->ಮೊದಲ_ಕೀಲಿ_Sym + i];

        ಕೀಲಿ->ಅಂಕೆ_groups = wire_sym_map->ಗುಂಪು_Info & 0x0f;
        FAIL_UNLESS(ಕೀಲಿ->ಅಂಕೆ_groups <= ARRAY_ಗಾತ್ರ(wire_sym_map->kt_ಸೂಚ್ಯಂಕ));
        ALLOC_OR_FAIL(ಕೀಲಿ->groups, ಕೀಲಿ->ಅಂಕೆ_groups);

        ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t j = 0; j < ಕೀಲಿ->ಅಂಕೆ_groups; j++) {
            FAIL_UNLESS(wire_sym_map->kt_ಸೂಚ್ಯಂಕ[j] < keymap->ಅಂಕೆ_types);
            ಕೀಲಿ->groups[j].type = &keymap->types[wire_sym_map->kt_ಸೂಚ್ಯಂಕ[j]];

            ALLOC_OR_FAIL(ಕೀಲಿ->groups[j].ಮಟ್ಟಗಳು, ಕೀಲಿ->groups[j].type->ಅಂಕೆ_ಮಟ್ಟಗಳು);
        }

        ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ = (wire_sym_map->ಗುಂಪು_Info & 0x30) >> 4;

        FAIL_UNLESS(ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_ಸಂಖ್ಯೆ <= ಕೀಲಿ->ಅಂಕೆ_groups);

        ಆದರೆ (wire_sym_map->ಗುಂಪು_Info & XCB_XKB_GROUPS_WRAP_CLAMP_INTO_RANGE)
            ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy = XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_CLAMP;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (wire_sym_map->ಗುಂಪು_Info & XCB_XKB_GROUPS_WRAP_REDIRECT_INTO_RANGE)
            ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy = XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_REDIRECT;
        ಇಲ್ಲದಿದ್ದರೆ
            ಕೀಲಿ->ಹೊರಹೋಗು_ದ_range_ಗುಂಪು_policy = XKB_LAYOUT_ಹೊರಹೋಗು_ದ_RANGE_WRAP;

        {
            ಪೂರ್ಣಾಂಕ syms_ಉದ್ದಳತೆ = xcb_xkb_ಕೀಲಿ_sym_map_syms_ಉದ್ದಳತೆ(wire_sym_map);
            xcb_keysym_t *syms_iter = xcb_xkb_ಕೀಲಿ_sym_map_syms(wire_sym_map);

            FAIL_UNLESS((ಚಿಹ್ನೆರಹಿತ) syms_ಉದ್ದಳತೆ == wire_sym_map->ಅಗಲ * ಕೀಲಿ->ಅಂಕೆ_groups);

            ಆದರೆ (syms_ಉದ್ದಳತೆ > 0)
                ಕೀಲಿ->explicit |= EXPLICIT_SYMBOLS;

            ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0; ಗುಂಪು < ಕೀಲಿ->ಅಂಕೆ_groups; ಗುಂಪು++) {
                ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t level = 0; level < wire_sym_map->ಅಗಲ; level++) {
                    xcb_keysym_t wire_keysym = *syms_iter;

                    ಪ್ರತಿಜ್ಞಾ(ಕೀಲಿ->groups[ಗುಂಪು].type != ಖಾಲಿ);
                    ಆದರೆ (level < ಕೀಲಿ->groups[ಗುಂಪು].type->ಅಂಕೆ_ಮಟ್ಟಗಳು) {
                        /* Do not discard the keysym yet if it is NoSymbol, because
                         * there may be an action set */
                        ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].ಅಂಕೆ_syms = 1;
                        ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].s.sym = wire_keysym;
                        /* Set capitalization transformation */
                        ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].ಊರ್ಧ್ವ =
                            xkb_keysym_ಗಮ್ಯ_ಊರ್ಧ್ವ(wire_keysym);
                        ಆದರೆ (wire_keysym != XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                            ಕೀಲಿ->groups[ಗುಂಪು].explicit_symbols = true;
                    }

                    syms_iter++;
                }
            }
        }

        xcb_xkb_ಕೀಲಿ_sym_map_ಮುಂದಿನ(&sym_maps_iter);
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_actions(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_ಉತ್ತರ_t *ಉತ್ತರ, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಂಕ acts_ಎಣಿಕೆ_ಉದ್ದಳತೆ =
        xcb_xkb_get_map_map_acts_rtrn_ಎಣಿಕೆ_ಉದ್ದಳತೆ(ಉತ್ತರ, map);
    uint8_t *acts_ಎಣಿಕೆ_iter = xcb_xkb_get_map_map_acts_rtrn_ಎಣಿಕೆ(map);
    xcb_xkb_action_iterator_t acts_iter =
        xcb_xkb_get_map_map_acts_rtrn_acts_iterator(ಉತ್ತರ, map);
    xcb_xkb_ಕೀಲಿ_sym_map_iterator_t sym_maps_iter =
        xcb_xkb_get_map_map_syms_rtrn_iterator(ಉತ್ತರ, map);

    FAIL_UNLESS(ಉತ್ತರ->ಮೊದಲ_ಕೀಲಿ_Action == keymap->min_ಕೀಲಿ_ಸಂಕೇತ);
    FAIL_UNLESS(ಉತ್ತರ->ಮೊದಲ_ಕೀಲಿ_Action + ಉತ್ತರ->n_ಕೀಲಿ_Actions ==
                keymap->max_ಕೀಲಿ_ಸಂಕೇತ + 1);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < acts_ಎಣಿಕೆ_ಉದ್ದಳತೆ; i++) {
        xcb_xkb_ಕೀಲಿ_sym_map_t *wire_sym_map = sym_maps_iter.ದತ್ತಾಂಶ;
        ಪೂರ್ಣಾಂಕ syms_ಉದ್ದಳತೆ = xcb_xkb_ಕೀಲಿ_sym_map_syms_ಉದ್ದಳತೆ(wire_sym_map);
        uint8_t wire_ಎಣಿಕೆ = *acts_ಎಣಿಕೆ_iter;
        ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[ಉತ್ತರ->ಮೊದಲ_ಕೀಲಿ_Action + i];

        FAIL_UNLESS((ಚಿಹ್ನೆರಹಿತ) syms_ಉದ್ದಳತೆ == wire_sym_map->ಅಗಲ * ಕೀಲಿ->ಅಂಕೆ_groups);
        FAIL_UNLESS(wire_ಎಣಿಕೆ == 0 || wire_ಎಣಿಕೆ == syms_ಉದ್ದಳತೆ);

        ಸ್ಥಿರಾಂಕ xkb_keycode_t min_ಕೀಲಿ_ಸಂಕೇತ = keymap->min_ಕೀಲಿ_ಸಂಕೇತ;
        ಸ್ಥಿರಾಂಕ xkb_keycode_t max_ಕೀಲಿ_ಸಂಕೇತ = keymap->max_ಕೀಲಿ_ಸಂಕೇತ;

        ಆದರೆ (wire_ಎಣಿಕೆ != 0) {
            ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t ಗುಂಪು = 0; ಗುಂಪು < ಕೀಲಿ->ಅಂಕೆ_groups; ಗುಂಪು++) {
                ಗಾಗಿ (xkb_level_ಸೂಚ್ಯಂಕ_t level = 0; level < wire_sym_map->ಅಗಲ; level++) {
                    xcb_xkb_action_t *wire_action = acts_iter.ದತ್ತಾಂಶ;

                    ಆದರೆ (level < ಕೀಲಿ->groups[ಗುಂಪು].type->ಅಂಕೆ_ಮಟ್ಟಗಳು) {
                        ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].ಅಂಕೆ_actions = 1;
                        ಒಕ್ಕೂಟ xkb_action *action =
                            &ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].a.action;

                        FAIL_UNLESS(translate_action(action, wire_action,
                                                     min_ಕೀಲಿ_ಸಂಕೇತ, max_ಕೀಲಿ_ಸಂಕೇತ));

                        ಆದರೆ (action->type != ACTION_TYPE_NONE) {
                            ಕೀಲಿ->groups[ಗುಂಪು].implicit_actions =
                                !(ಕೀಲಿ->explicit & EXPLICIT_INTERP);
                        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].s.sym ==
                                   XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ) {
                            /* If the action and the keysym are both undefined,
                             * discard them */
                            ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].ಅಂಕೆ_syms = 0;
                            ಕೀಲಿ->groups[ಗುಂಪು].ಮಟ್ಟಗಳು[level].ಅಂಕೆ_actions = 0;
                        }
                    }

                    xcb_xkb_action_ಮುಂದಿನ(&acts_iter);
                }

                ಆದರೆ (ಕೀಲಿ->groups[ಗುಂಪು].implicit_actions)
                    ಕೀಲಿ->implicit_actions = true;
            }
        }

        acts_ಎಣಿಕೆ_iter++;
        xcb_xkb_ಕೀಲಿ_sym_map_ಮುಂದಿನ(&sym_maps_iter);
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_vmods(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
          xcb_xkb_get_map_ಉತ್ತರ_t *ಉತ್ತರ, xcb_xkb_get_map_map_t *map)
{
    uint8_t *iter = xcb_xkb_get_map_map_vmods_rtrn(map);

    keymap->mods.ಅಂಕೆ_mods =
        ಅಂಕೆ_REAL_MODS + MIN(msb_pos(ಉತ್ತರ->virtualMods), ಅಂಕೆ_VMODS);
    FAIL_UNLESS(keymap->mods.ಅಂಕೆ_mods <= XKB_MAX_MODS);

    static_assert(ಅಂಕೆ_REAL_MODS + ಅಂಕೆ_VMODS <= XKB_MAX_MODS, "");
    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ i = 0; i < ಅಂಕೆ_VMODS; i++) {
        ಆದರೆ (ಉತ್ತರ->virtualMods & (1u << i)) {
            uint8_t wire = *iter;
            ರಚನೆ xkb_mod *mod = &keymap->mods.mods[ಅಂಕೆ_REAL_MODS + i];

            mod->type = MOD_VIRT;
            mod->mapping = translate_mods(wire, 0, 0);

            iter++;
        }
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_explicits(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_map_ಉತ್ತರ_t *ಉತ್ತರ, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = xcb_xkb_get_map_map_explicit_rtrn_ಉದ್ದಳತೆ(ಉತ್ತರ, map);
    xcb_xkb_ಗಣ_explicit_iterator_t iter =
        xcb_xkb_get_map_map_explicit_rtrn_iterator(ಉತ್ತರ, map);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಉದ್ದಳತೆ; i++) {
        xcb_xkb_ಗಣ_explicit_t *wire = iter.ದತ್ತಾಂಶ;
        ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;

        FAIL_UNLESS(wire->keycode >= keymap->min_ಕೀಲಿ_ಸಂಕೇತ &&
                    wire->keycode <= keymap->max_ಕೀಲಿ_ಸಂಕೇತ);

        ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[wire->keycode];

        ಆದರೆ ((wire->explicit & XCB_XKB_EXPLICIT_ಕೀಲಿ_TYPE_1) &&
            ಕೀಲಿ->ಅಂಕೆ_groups > 0) {
            ಕೀಲಿ->groups[0].explicit_type = true;
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
        }
        ಆದರೆ ((wire->explicit & XCB_XKB_EXPLICIT_ಕೀಲಿ_TYPE_2) &&
            ಕೀಲಿ->ಅಂಕೆ_groups > 1) {
            ಕೀಲಿ->groups[1].explicit_type = true;
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
        }
        ಆದರೆ ((wire->explicit & XCB_XKB_EXPLICIT_ಕೀಲಿ_TYPE_3) &&
            ಕೀಲಿ->ಅಂಕೆ_groups > 2) {
            ಕೀಲಿ->groups[2].explicit_type = true;
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
        }
        ಆದರೆ ((wire->explicit & XCB_XKB_EXPLICIT_ಕೀಲಿ_TYPE_4) &&
            ಕೀಲಿ->ಅಂಕೆ_groups > 3) {
            ಕೀಲಿ->groups[3].explicit_type = true;
            ಕೀಲಿ->explicit |= EXPLICIT_TYPES;
        }
        ಆದರೆ (wire->explicit & XCB_XKB_EXPLICIT_INTERPRET) {
            ಕೀಲಿ->explicit |= EXPLICIT_INTERP;
            /*
             * Make all key groups have explicit actions too, because we have
             * no way to know which one is implicit.
             */
            ಗಾಗಿ (xkb_layout_ಸೂಚ್ಯಂಕ_t l = 0; l < ಕೀಲಿ->ಅಂಕೆ_groups; l++) {
                ಕೀಲಿ->groups[l].explicit_actions = true;
            }
        }
        ಆದರೆ (wire->explicit & XCB_XKB_EXPLICIT_AUTO_REPEAT)
            ಕೀಲಿ->explicit |= EXPLICIT_REPEAT;
        ಆದರೆ (wire->explicit & XCB_XKB_EXPLICIT_V_MOD_MAP)
            ಕೀಲಿ->explicit |= EXPLICIT_VMODMAP;

        xcb_xkb_ಗಣ_explicit_ಮುಂದಿನ(&iter);
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_modmaps(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_map_ಉತ್ತರ_t *ಉತ್ತರ, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = xcb_xkb_get_map_map_modmap_rtrn_ಉದ್ದಳತೆ(ಉತ್ತರ, map);
    xcb_xkb_ಕೀಲಿ_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_modmap_rtrn_iterator(ಉತ್ತರ, map);

    darray(Mod_Map_ನಮೂದು) modmaps = darray_new();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಉದ್ದಳತೆ; i++) {
        xcb_xkb_ಕೀಲಿ_mod_map_t *wire = iter.ದತ್ತಾಂಶ;
        ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;

        FAIL_UNLESS(wire->keycode >= keymap->min_ಕೀಲಿ_ಸಂಕೇತ &&
                    wire->keycode <= keymap->max_ಕೀಲಿ_ಸಂಕೇತ);

        ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[wire->keycode];
        ಕೀಲಿ->modmap = wire->mods;

        darray_append(modmaps, (Mod_Map_ನಮೂದು) {
            .ಕೀಲಿ_ಸಂಕೇತ = ಕೀಲಿ->keycode,
            .mods = ಕೀಲಿ->modmap,
            .have_ಚಿಹ್ನೆ = false,
            .u.ಕೀಲಿ_Name = ಕೀಲಿ->name
        });

        xcb_xkb_ಕೀಲಿ_mod_map_ಮುಂದಿನ(&iter);
    }

    darray_steal(modmaps, &keymap->modmaps, &keymap->ಅಂಕೆ_modmaps);
    ಹಿಂತಿರುಗಿ true;

fail:
    darray_free(modmaps);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_vmodmaps(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_map_ಉತ್ತರ_t *ಉತ್ತರ, xcb_xkb_get_map_map_t *map)
{
    ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = xcb_xkb_get_map_map_vmodmap_rtrn_ಉದ್ದಳತೆ(ಉತ್ತರ, map);
    xcb_xkb_ಕೀಲಿ_v_mod_map_iterator_t iter =
        xcb_xkb_get_map_map_vmodmap_rtrn_iterator(ಉತ್ತರ, map);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಉದ್ದಳತೆ; i++) {
        xcb_xkb_ಕೀಲಿ_v_mod_map_t *wire = iter.ದತ್ತಾಂಶ;
        ರಚನೆ xkb_ಕೀಲಿ *ಕೀಲಿ;

        FAIL_UNLESS(wire->keycode >= keymap->min_ಕೀಲಿ_ಸಂಕೇತ &&
                    wire->keycode <= keymap->max_ಕೀಲಿ_ಸಂಕೇತ);

        ಕೀಲಿ = &keymap->ಕೀಲಿಗಳು[wire->keycode];
        ಕೀಲಿ->vmodmap = translate_mods(0, wire->vmods, 0);

        xcb_xkb_ಕೀಲಿ_v_mod_map_ಮುಂದಿನ(&iter);
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_map(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
        xcb_xkb_get_map_cookie_t cookie)
{
    xcb_xkb_get_map_ಉತ್ತರ_t *ಉತ್ತರ = xcb_xkb_get_map_ಉತ್ತರ(conn, cookie, ಖಾಲಿ);
    xcb_xkb_get_map_map_t map;

    FAIL_IF_BAD_ಉತ್ತರ(ಉತ್ತರ, "XkbGetMap");

    ಆದರೆ ((ಉತ್ತರ->present & get_map_required_components) != get_map_required_components)
        ಹೋಗು fail;

    xcb_xkb_get_map_map_unpack(xcb_xkb_get_map_map(ಉತ್ತರ),
                               ಉತ್ತರ->nTypes,
                               ಉತ್ತರ->n_ಕೀಲಿ_Syms,
                               ಉತ್ತರ->n_ಕೀಲಿ_Actions,
                               ಉತ್ತರ->ಒಟ್ಟು_Actions,
                               ಉತ್ತರ->ಒಟ್ಟು_ಕೀಲಿ_Behaviors,
                               ಉತ್ತರ->virtualMods,
                               ಉತ್ತರ->ಒಟ್ಟು_ಕೀಲಿ_Explicit,
                               ಉತ್ತರ->ಒಟ್ಟು_Mod_Map_ಕೀಲಿಗಳು,
                               ಉತ್ತರ->ಒಟ್ಟು_VMod_Map_ಕೀಲಿಗಳು,
                               ಉತ್ತರ->present,
                               &map);

    ಆದರೆ (!get_types(keymap, conn, ಉತ್ತರ, &map) ||
        !get_sym_maps(keymap, conn, ಉತ್ತರ, &map) ||
        !get_actions(keymap, conn, ಉತ್ತರ, &map) ||
        !get_vmods(keymap, conn, ಉತ್ತರ, &map) ||
        !get_explicits(keymap, conn, ಉತ್ತರ, &map) ||
        !get_modmaps(keymap, conn, ಉತ್ತರ, &map) ||
        !get_vmodmaps(keymap, conn, ಉತ್ತರ, &map))
        ಹೋಗು fail;

    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ true;

fail:
    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_indicators(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_indicator_map_ಉತ್ತರ_t *ಉತ್ತರ)
{
    xcb_xkb_indicator_map_iterator_t iter =
        xcb_xkb_get_indicator_map_maps_iterator(ಉತ್ತರ);

    keymap->ಅಂಕೆ_leds = msb_pos(ಉತ್ತರ->which);
    FAIL_UNLESS(keymap->ಅಂಕೆ_leds <= XKB_MAX_LEDS);

    static_assert(XKB_MAX_LEDS == ಅಂಕೆ_INDICATORS, "");
    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ i = 0; i < ಅಂಕೆ_INDICATORS; i++) {
        ಆದರೆ (ಉತ್ತರ->which & (UINT32_C(1) << i)) {
            xcb_xkb_indicator_map_t *wire = iter.ದತ್ತಾಂಶ;
            ರಚನೆ xkb_led *led = &keymap->leds[i];

            ಆದರೆ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_ತಳ)
                led->which_groups |= XKB_STATE_LAYOUT_DEPRESSED;
            ಆದರೆ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LATCHED)
                led->which_groups |= XKB_STATE_LAYOUT_LATCHED;
            ಆದರೆ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_LOCKED)
                led->which_groups |= XKB_STATE_LAYOUT_LOCKED;
            ಆದರೆ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_EFFECTIVE)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;
            ಆದರೆ (wire->whichGroups & XCB_XKB_IM_GROUPS_WHICH_USE_COMPAT)
                led->which_groups |= XKB_STATE_LAYOUT_EFFECTIVE;

            led->groups = wire->groups;

            ಆದರೆ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_ತಳ)
                led->which_mods |= XKB_STATE_MODS_DEPRESSED;
            ಆದರೆ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LATCHED)
                led->which_mods |= XKB_STATE_MODS_LATCHED;
            ಆದರೆ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_LOCKED)
                led->which_mods |= XKB_STATE_MODS_LOCKED;
            ಆದರೆ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_EFFECTIVE)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;
            ಆದರೆ (wire->whichMods & XCB_XKB_IM_MODS_WHICH_USE_COMPAT)
                led->which_mods |= XKB_STATE_MODS_EFFECTIVE;

            led->mods.mods = translate_mods(wire->realMods, wire->vmods, 0);
            led->mods.ಮುಖವಾಡ = translate_mods(wire->mods, 0, 0);

            led->ctrls = translate_controls_ಮುಖವಾಡ(wire->ctrls);

            xcb_xkb_indicator_map_ಮುಂದಿನ(&iter);
        }
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_indicator_map(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
                  xcb_xkb_get_indicator_map_cookie_t cookie)
{
    xcb_xkb_get_indicator_map_ಉತ್ತರ_t *ಉತ್ತರ =
        xcb_xkb_get_indicator_map_ಉತ್ತರ(conn, cookie, ಖಾಲಿ);

    FAIL_IF_BAD_ಉತ್ತರ(ಉತ್ತರ, "XkbGetIndicatorMap");

    ಆದರೆ (!get_indicators(keymap, conn, ಉತ್ತರ))
        ಹೋಗು fail;

    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ true;

fail:
    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_sym_interprets(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
                   xcb_xkb_get_compat_map_ಉತ್ತರ_t *ಉತ್ತರ)
{
    ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = xcb_xkb_get_compat_map_si_rtrn_ಉದ್ದಳತೆ(ಉತ್ತರ);
    xcb_xkb_sym_interpret_iterator_t iter =
        xcb_xkb_get_compat_map_si_rtrn_iterator(ಉತ್ತರ);

    FAIL_UNLESS(ಉತ್ತರ->ಮೊದಲ_SIRtrn == 0);
    FAIL_UNLESS(ಉತ್ತರ->nSIRtrn == ಉತ್ತರ->n_ಒಟ್ಟು_SI);

    keymap->ಅಂಕೆ_sym_interprets = ಉತ್ತರ->nSIRtrn;
    ALLOC_OR_FAIL(keymap->sym_interprets, keymap->ಅಂಕೆ_sym_interprets);

    ಸ್ಥಿರಾಂಕ xkb_keycode_t min_ಕೀಲಿ_ಸಂಕೇತ = keymap->min_ಕೀಲಿ_ಸಂಕೇತ;
    ಸ್ಥಿರಾಂಕ xkb_keycode_t max_ಕೀಲಿ_ಸಂಕೇತ = keymap->max_ಕೀಲಿ_ಸಂಕೇತ;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಉದ್ದಳತೆ; i++) {
        xcb_xkb_sym_interpret_t *wire = iter.ದತ್ತಾಂಶ;
        ರಚನೆ xkb_sym_interpret *sym_interpret = &keymap->sym_interprets[i];

        sym_interpret->sym = wire->sym;

        ಆಯ್ಕೆ (wire->ಹೊಂದಿಕೆ & XCB_XKB_SYM_INTERP_ಹೊಂದಿಕೆ_OP_ಮುಖವಾಡ) {
        ಸ್ಥಿತಿ XCB_XKB_SYM_INTERPRET_ಹೊಂದಿಕೆ_NONE_ದ:
            sym_interpret->ಹೊಂದಿಕೆ = ಹೊಂದಿಕೆ_NONE;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ XCB_XKB_SYM_INTERPRET_ಹೊಂದಿಕೆ_ANY_ದ_OR_NONE:
            sym_interpret->ಹೊಂದಿಕೆ = ಹೊಂದಿಕೆ_ANY_OR_NONE;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ XCB_XKB_SYM_INTERPRET_ಹೊಂದಿಕೆ_ANY_ದ:
            sym_interpret->ಹೊಂದಿಕೆ = ಹೊಂದಿಕೆ_ANY;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ XCB_XKB_SYM_INTERPRET_ಹೊಂದಿಕೆ_ALL_ದ:
            sym_interpret->ಹೊಂದಿಕೆ = ಹೊಂದಿಕೆ_ALL;
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ XCB_XKB_SYM_INTERPRET_ಹೊಂದಿಕೆ_EXACTLY:
            sym_interpret->ಹೊಂದಿಕೆ = ಹೊಂದಿಕೆ_EXACTLY;
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            log_err_func(keymap->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "unrecognized interpret match value: %#x\n",
                         wire->ಹೊಂದಿಕೆ & XCB_XKB_SYM_INTERP_ಹೊಂದಿಕೆ_OP_ಮುಖವಾಡ);
            ಹೋಗು fail;
        }

        sym_interpret->level_one_only =
            (wire->ಹೊಂದಿಕೆ & XCB_XKB_SYM_INTERP_ಹೊಂದಿಕೆ_LEVEL_ONE_ONLY);
        sym_interpret->mods = wire->mods;

        ಆದರೆ (wire->virtualMod == NO_MODIFIER)
            sym_interpret->virtual_mod = XKB_MOD_INVALID;
        ಇಲ್ಲದಿದ್ದರೆ
            sym_interpret->virtual_mod = ಅಂಕೆ_REAL_MODS + wire->virtualMod;

        sym_interpret->repeat = (wire->flags & 0x01);
        FAIL_UNLESS(translate_action(&sym_interpret->a.action,
                                     (xcb_xkb_action_t *) &wire->action,
                                     min_ಕೀಲಿ_ಸಂಕೇತ, max_ಕೀಲಿ_ಸಂಕೇತ));
        sym_interpret->ಅಂಕೆ_actions =
            (sym_interpret->a.action.type != ACTION_TYPE_NONE);

        /* Checked only when compiling a keymap from text */
        sym_interpret->required = true;

        xcb_xkb_sym_interpret_ಮುಂದಿನ(&iter);
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_compat_map(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
               xcb_xkb_get_compat_map_cookie_t cookie)
{
    xcb_xkb_get_compat_map_ಉತ್ತರ_t *ಉತ್ತರ =
        xcb_xkb_get_compat_map_ಉತ್ತರ(conn, cookie, ಖಾಲಿ);

    FAIL_IF_BAD_ಉತ್ತರ(ಉತ್ತರ, "XkbGetCompatMap");

    ಆದರೆ (!get_sym_interprets(keymap, conn, ಉತ್ತರ))
        ಹೋಗು fail;

    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ true;

fail:
    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_type_ಹೆಸರುಗಳು(ರಚನೆ xkb_keymap *keymap, ರಚನೆ x11_atom_interner *interner,
               xcb_xkb_get_ಹೆಸರುಗಳು_ಉತ್ತರ_t *ಉತ್ತರ,
               xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_t *list)
{
    ಪೂರ್ಣಾಂಕ ಕೀಲಿ_type_ಹೆಸರುಗಳು_ಉದ್ದಳತೆ =
        xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_type_ಹೆಸರುಗಳು_ಉದ್ದಳತೆ(ಉತ್ತರ, list);
    xcb_atom_t *ಕೀಲಿ_type_ಹೆಸರುಗಳು_iter =
        xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_type_ಹೆಸರುಗಳು(list);
    ಪೂರ್ಣಾಂಕ n_ಮಟ್ಟಗಳು_ಪ್ರತಿ_type_ಉದ್ದಳತೆ =
        xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_n_ಮಟ್ಟಗಳು_ಪ್ರತಿ_type_ಉದ್ದಳತೆ(ಉತ್ತರ, list);
    uint8_t *n_ಮಟ್ಟಗಳು_ಪ್ರತಿ_type_iter =
        xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_n_ಮಟ್ಟಗಳು_ಪ್ರತಿ_type(list);
    xcb_atom_t *kt_level_ಹೆಸರುಗಳು_iter =
        xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_kt_level_ಹೆಸರುಗಳು(list);

    FAIL_UNLESS(ಉತ್ತರ->nTypes == keymap->ಅಂಕೆ_types);
    FAIL_UNLESS(ಕೀಲಿ_type_ಹೆಸರುಗಳು_ಉದ್ದಳತೆ == n_ಮಟ್ಟಗಳು_ಪ್ರತಿ_type_ಉದ್ದಳತೆ);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಕೀಲಿ_type_ಹೆಸರುಗಳು_ಉದ್ದಳತೆ; i++) {
        xcb_atom_t wire_type_name = *ಕೀಲಿ_type_ಹೆಸರುಗಳು_iter;
        uint8_t wire_ಅಂಕೆ_ಮಟ್ಟಗಳು = *n_ಮಟ್ಟಗಳು_ಪ್ರತಿ_type_iter;
        ರಚನೆ xkb_ಕೀಲಿ_type *type = &keymap->types[i];

        /* Levels names are optional, but the maximum is the level count */
        FAIL_UNLESS(type->ಅಂಕೆ_ಮಟ್ಟಗಳು >= wire_ಅಂಕೆ_ಮಟ್ಟಗಳು);

        /* Allocate names for all levels, even if some names are missing */
        ALLOC_OR_FAIL(type->level_ಹೆಸರುಗಳು, type->ಅಂಕೆ_ಮಟ್ಟಗಳು);
        type->ಅಂಕೆ_level_ಹೆಸರುಗಳು = type->ಅಂಕೆ_ಮಟ್ಟಗಳು;

        x11_atom_interner_adopt_atom(interner, wire_type_name, &type->name);
        ಗಾಗಿ (ಗಾತ್ರ_t j = 0; j < wire_ಅಂಕೆ_ಮಟ್ಟಗಳು; j++) {
            x11_atom_interner_adopt_atom(interner, kt_level_ಹೆಸರುಗಳು_iter[j],
                                         &type->level_ಹೆಸರುಗಳು[j]);
        }
        /* We may have received less names than there are levels: ensure
         * missing names have a fallback. */
        ಗಾಗಿ (ಗಾತ್ರ_t j = wire_ಅಂಕೆ_ಮಟ್ಟಗಳು; j < type->ಅಂಕೆ_ಮಟ್ಟಗಳು; j++)
            type->level_ಹೆಸರುಗಳು[j] = XKB_ATOM_NONE;

        kt_level_ಹೆಸರುಗಳು_iter += wire_ಅಂಕೆ_ಮಟ್ಟಗಳು;
        ಕೀಲಿ_type_ಹೆಸರುಗಳು_iter++;
        n_ಮಟ್ಟಗಳು_ಪ್ರತಿ_type_iter++;
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_indicator_ಹೆಸರುಗಳು(ರಚನೆ xkb_keymap *keymap,
                    ರಚನೆ x11_atom_interner *interner,
                    xcb_xkb_get_ಹೆಸರುಗಳು_ಉತ್ತರ_t *ಉತ್ತರ,
                    xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_indicator_ಹೆಸರುಗಳು(list);

    FAIL_UNLESS(msb_pos(ಉತ್ತರ->indicators) <= keymap->ಅಂಕೆ_leds);

    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ i = 0; i < ಅಂಕೆ_INDICATORS; i++) {
        ಆದರೆ (ಉತ್ತರ->indicators & (UINT32_C(1) << i)) {
            xcb_atom_t wire = *iter;
            ರಚನೆ xkb_led *led = &keymap->leds[i];

            x11_atom_interner_adopt_atom(interner, wire, &led->name);

            iter++;
        }
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_vmod_ಹೆಸರುಗಳು(ರಚನೆ xkb_keymap *keymap, ರಚನೆ x11_atom_interner *interner,
               xcb_xkb_get_ಹೆಸರುಗಳು_ಉತ್ತರ_t *ಉತ್ತರ,
               xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_t *list)
{
    xcb_atom_t *iter = xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_virtual_mod_ಹೆಸರುಗಳು(list);

    /*
     * GetMap's reply->virtualMods is always 0xffff. This one really
     * tells us which vmods exist (a vmod must have a name), so we fix
     * up the size here.
     */
    keymap->mods.ಅಂಕೆ_mods =
        ಅಂಕೆ_REAL_MODS + MIN(msb_pos(ಉತ್ತರ->virtualMods), ಅಂಕೆ_VMODS);
    FAIL_UNLESS(keymap->mods.ಅಂಕೆ_mods <= XKB_MAX_MODS);

    static_assert(ಅಂಕೆ_REAL_MODS + ಅಂಕೆ_VMODS <= XKB_MAX_MODS, "");
    ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ i = 0; i < ಅಂಕೆ_VMODS; i++) {
        ಆದರೆ (ಉತ್ತರ->virtualMods & (1u << i)) {
            xcb_atom_t wire = *iter;
            ರಚನೆ xkb_mod *mod = &keymap->mods.mods[ಅಂಕೆ_REAL_MODS + i];

            x11_atom_interner_adopt_atom(interner, wire, &mod->name);

            iter++;
        }
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_ಗುಂಪು_ಹೆಸರುಗಳು(ರಚನೆ xkb_keymap *keymap, ರಚನೆ x11_atom_interner *interner,
                xcb_xkb_get_ಹೆಸರುಗಳು_ಉತ್ತರ_t *ಉತ್ತರ,
                xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_t *list)
{
    ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_groups_ಉದ್ದಳತೆ(ಉತ್ತರ, list);
    xcb_atom_t *iter = xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_groups(list);

    keymap->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು = msb_pos(ಉತ್ತರ->ಗುಂಪು_ಹೆಸರುಗಳು_2);
    ALLOC_OR_FAIL(keymap->ಗುಂಪು_ಹೆಸರುಗಳು, keymap->ಅಂಕೆ_ಗುಂಪು_ಹೆಸರುಗಳು);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಉದ್ದಳತೆ; i++) {
        x11_atom_interner_adopt_atom(interner, iter[i],
                                     &keymap->ಗುಂಪು_ಹೆಸರುಗಳು[i]);
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_ಕೀಲಿ_ಹೆಸರುಗಳು(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
              xcb_xkb_get_ಹೆಸರುಗಳು_ಉತ್ತರ_t *ಉತ್ತರ,
              xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_t *list)
{
    ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_ಕೀಲಿ_ಹೆಸರುಗಳು_ಉದ್ದಳತೆ(ಉತ್ತರ, list);
    xcb_xkb_ಕೀಲಿ_name_iterator_t iter =
        xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_ಕೀಲಿ_ಹೆಸರುಗಳು_iterator(ಉತ್ತರ, list);

    FAIL_UNLESS(ಉತ್ತರ->min_ಕೀಲಿ_ಸಂಕೇತ_2 == keymap->min_ಕೀಲಿ_ಸಂಕೇತ);
    FAIL_UNLESS(ಉತ್ತರ->max_ಕೀಲಿ_ಸಂಕೇತ_2 == keymap->max_ಕೀಲಿ_ಸಂಕೇತ);
    FAIL_UNLESS(ಉತ್ತರ->ಮೊದಲ_ಕೀಲಿ == keymap->min_ಕೀಲಿ_ಸಂಕೇತ);
    FAIL_UNLESS(ಉತ್ತರ->ಮೊದಲ_ಕೀಲಿ + ಉತ್ತರ->n_ಕೀಲಿಗಳು - 1U == keymap->max_ಕೀಲಿ_ಸಂಕೇತ);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಉದ್ದಳತೆ; i++) {
        xcb_xkb_ಕೀಲಿ_name_t *wire = iter.ದತ್ತಾಂಶ;
        xkb_atom_t *ಕೀಲಿ_name = &keymap->ಕೀಲಿಗಳು[ಉತ್ತರ->ಮೊದಲ_ಕೀಲಿ + i].name;

        ಆದರೆ (wire->name[0] == '\0') {
            *ಕೀಲಿ_name = XKB_ATOM_NONE;
        }
        ಇಲ್ಲದಿದ್ದರೆ {
            *ಕೀಲಿ_name = xkb_atom_intern(keymap->ctx, wire->name,
                                        strnlen(wire->name,
                                                XCB_XKB_CONST_ಕೀಲಿ_NAME_ಉದ್ದಳತೆ));
            ಆದರೆ (!*ಕೀಲಿ_name)
                ಹಿಂತಿರುಗಿ false;
        }

        xcb_xkb_ಕೀಲಿ_name_ಮುಂದಿನ(&iter);
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_aliases(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
            xcb_xkb_get_ಹೆಸರುಗಳು_ಉತ್ತರ_t *ಉತ್ತರ,
            xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_t *list)
{
    ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_ಕೀಲಿ_aliases_ಉದ್ದಳತೆ(ಉತ್ತರ, list);
    xcb_xkb_ಕೀಲಿ_alias_iterator_t iter =
        xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_ಕೀಲಿ_aliases_iterator(ಉತ್ತರ, list);

    keymap->ಅಂಕೆ_ಕೀಲಿ_aliases = ಉತ್ತರ->n_ಕೀಲಿ_Aliases;
    ALLOC_OR_FAIL(keymap->ಕೀಲಿ_aliases, keymap->ಅಂಕೆ_ಕೀಲಿ_aliases);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಉದ್ದಳತೆ; i++) {
        xcb_xkb_ಕೀಲಿ_alias_t *wire = iter.ದತ್ತಾಂಶ;
        ರಚನೆ xkb_ಕೀಲಿ_alias *alias = &keymap->ಕೀಲಿ_aliases[i];

        alias->real =
            xkb_atom_intern(keymap->ctx, wire->real,
                            strnlen(wire->real, XCB_XKB_CONST_ಕೀಲಿ_NAME_ಉದ್ದಳತೆ));
        alias->alias =
            xkb_atom_intern(keymap->ctx, wire->alias,
                            strnlen(wire->alias, XCB_XKB_CONST_ಕೀಲಿ_NAME_ಉದ್ದಳತೆ));
        ಆದರೆ (!alias->real || !alias->alias)
            ಹೋಗು fail;

        xcb_xkb_ಕೀಲಿ_alias_ಮುಂದಿನ(&iter);
    }

    ಹಿಂತಿರುಗಿ true;

fail:
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_ಹೆಸರುಗಳು(ರಚನೆ xkb_keymap *keymap, ರಚನೆ x11_atom_interner *interner,
          xcb_xkb_get_ಹೆಸರುಗಳು_cookie_t cookie)
{
    xcb_connection_t *conn = interner->conn;
    xcb_xkb_get_ಹೆಸರುಗಳು_ಉತ್ತರ_t *ಉತ್ತರ =
        xcb_xkb_get_ಹೆಸರುಗಳು_ಉತ್ತರ(conn, cookie, ಖಾಲಿ);
    xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_t list;

    FAIL_IF_BAD_ಉತ್ತರ(ಉತ್ತರ, "XkbGetNames");

    FAIL_UNLESS((ಉತ್ತರ->which & get_ಹೆಸರುಗಳು_required) == get_ಹೆಸರುಗಳು_required);

    xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list_unpack(xcb_xkb_get_ಹೆಸರುಗಳು_ಮೌಲ್ಯ_list(ಉತ್ತರ),
                                        ಉತ್ತರ->nTypes,
                                        ಉತ್ತರ->indicators,
                                        ಉತ್ತರ->virtualMods,
                                        ಉತ್ತರ->ಗುಂಪು_ಹೆಸರುಗಳು_2,
                                        ಉತ್ತರ->n_ಕೀಲಿಗಳು,
                                        ಉತ್ತರ->n_ಕೀಲಿ_Aliases,
                                        ಉತ್ತರ->nRadioGroups,
                                        ಉತ್ತರ->which,
                                        &list);

    x11_atom_interner_get_escaped_atom_name(interner, list.keycodesName,
                                            &keymap->keycodes_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.symbolsName,
                                            &keymap->symbols_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.typesName,
                                            &keymap->types_section_name);
    x11_atom_interner_get_escaped_atom_name(interner, list.compatName,
                                            &keymap->compat_section_name);
    ಆದರೆ (!get_type_ಹೆಸರುಗಳು(keymap, interner, ಉತ್ತರ, &list) ||
        !get_indicator_ಹೆಸರುಗಳು(keymap, interner, ಉತ್ತರ, &list) ||
        !get_vmod_ಹೆಸರುಗಳು(keymap, interner, ಉತ್ತರ, &list) ||
        !get_ಗುಂಪು_ಹೆಸರುಗಳು(keymap, interner, ಉತ್ತರ, &list) ||
        !get_ಕೀಲಿ_ಹೆಸರುಗಳು(keymap, conn, ಉತ್ತರ, &list) ||
        !get_aliases(keymap, conn, ಉತ್ತರ, &list))
        ಹೋಗು fail;

    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ true;

fail:
    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool
get_controls(ರಚನೆ xkb_keymap *keymap, xcb_connection_t *conn,
             xcb_xkb_get_controls_cookie_t cookie)
{
    xcb_xkb_get_controls_ಉತ್ತರ_t *ಉತ್ತರ =
        xcb_xkb_get_controls_ಉತ್ತರ(conn, cookie, ಖಾಲಿ);

    FAIL_IF_BAD_ಉತ್ತರ(ಉತ್ತರ, "XkbGetControls");
    FAIL_UNLESS(ಉತ್ತರ->ಅಂಕೆ_Groups > 0 && ಉತ್ತರ->ಅಂಕೆ_Groups <= 4);

    keymap->ಅಂಕೆ_groups = ಉತ್ತರ->ಅಂಕೆ_Groups;

    FAIL_UNLESS(keymap->max_ಕೀಲಿ_ಸಂಕೇತ < XCB_XKB_CONST_ಪ್ರತಿ_ಕೀಲಿ_ಬಿಟ್_ARRAY_ಗಾತ್ರ * 8);

    ಗಾಗಿ (xkb_keycode_t i = keymap->min_ಕೀಲಿ_ಸಂಕೇತ; i <= keymap->max_ಕೀಲಿ_ಸಂಕೇತ; i++)
        keymap->ಕೀಲಿಗಳು[i].repeats = (ಉತ್ತರ->ಪ್ರತಿ_ಕೀಲಿ_Repeat[i / 8] & (1u << (i % 8)));

    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ true;

fail:
    ಬಿಡುಗಡೆ(ಉತ್ತರ);
    ಹಿಂತಿರುಗಿ false;
}

ರಚನೆ xkb_keymap *
xkb_x11_keymap_new_from_device(ರಚನೆ xkb_context *ctx,
                               xcb_connection_t *conn,
                               int32_t device_id,
                               ಎಣಿಕೆ xkb_keymap_compile_flags flags)
{
    ಆದರೆ (device_id < 0 || device_id > 127) {
        log_err_func(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "illegal device ID: %"PRId32"\n", device_id);
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    ಸ್ಥಿರಾಂಕ ಎಣಿಕೆ xkb_keymap_format format = XKB_KEYMAP_FORMAT_ಪಠ್ಯ_V1;
    ರಚನೆ xkb_keymap * ಸ್ಥಿರಾಂಕ keymap =
        xkb_keymap_new(ctx, __func__, format, flags);
    ಆದರೆ (!keymap)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;

    keymap->redirect_ಕೀಲಿ_auto = XKB_KEYCODE_MAX; /* Invalid X11 keycode */

    ರಚನೆ x11_atom_interner interner;
    x11_atom_interner_init(&interner, ctx, conn);

    /*
     * Send all requests together so only one roundtrip is needed
     * to get the replies.
     */
    xcb_xkb_get_map_cookie_t map_cookie =
        xcb_xkb_get_map(conn, device_id, get_map_required_components,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    xcb_xkb_get_indicator_map_cookie_t indicator_map_cookie =
        xcb_xkb_get_indicator_map(conn, device_id, ALL_INDICATORS_ಮುಖವಾಡ);
    xcb_xkb_get_compat_map_cookie_t compat_map_cookie =
        xcb_xkb_get_compat_map(conn, device_id, 0, true, 0, 0);
    xcb_xkb_get_ಹೆಸರುಗಳು_cookie_t ಹೆಸರುಗಳು_cookie =
        xcb_xkb_get_ಹೆಸರುಗಳು(conn, device_id, get_ಹೆಸರುಗಳು_wanted);
    xcb_xkb_get_controls_cookie_t controls_cookie =
        xcb_xkb_get_controls(conn, device_id);

    ಆದರೆ (!get_map(keymap, conn, map_cookie))
        ಹೋಗು err_map;
    ಆದರೆ (!get_indicator_map(keymap, conn, indicator_map_cookie))
        ಹೋಗು err_indicator_map;
    ಆದರೆ (!get_compat_map(keymap, conn, compat_map_cookie))
        ಹೋಗು err_compat_map;
    ಆದರೆ (!get_ಹೆಸರುಗಳು(keymap, &interner, ಹೆಸರುಗಳು_cookie))
        ಹೋಗು err_ಹೆಸರುಗಳು;
    ಆದರೆ (!get_controls(keymap, conn, controls_cookie))
        ಹೋಗು err_controls;
    x11_atom_interner_ಸುತ್ತು_trip(&interner);
    ಆದರೆ (interner.had_error)
        ಹೋಗು err_interner;

    ಹಿಂತಿರುಗಿ keymap;

err_map:
    xcb_discard_ಉತ್ತರ(conn, indicator_map_cookie.sequence);
err_indicator_map:
    xcb_discard_ಉತ್ತರ(conn, compat_map_cookie.sequence);
err_compat_map:
    xcb_discard_ಉತ್ತರ(conn, ಹೆಸರುಗಳು_cookie.sequence);
err_ಹೆಸರುಗಳು:
    xcb_discard_ಉತ್ತರ(conn, controls_cookie.sequence);
err_controls:
    x11_atom_interner_ಸುತ್ತು_trip(&interner);
err_interner:
    xkb_keymap_unref(keymap);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}
