/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcommon/xkbcommon-names.h"
#అన్తర్భావ "features/enums.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "messages-codes.h"

స్థిర శూన్య
update_builtin_keymap_fields(నిర్మాణం xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    స్థిర స్థిరరాశి అక్షరం *స్థిరరాశి builtin_mods[] = {
        [XKB_MOD_సూచీ_జరుపు] = XKB_MOD_NAME_జరుపు,
        [XKB_MOD_సూచీ_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_సూచీ_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_సూచీ_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_సూచీ_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_సూచీ_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_సూచీ_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_సూచీ_MOD5]  = XKB_MOD_NAME_MOD5
    };

    కొరకు (xkb_mod_సూచీ_t i = 0; i < ARRAY_కొలత(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    అక్షరశ్రేణి_పొడవు(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.అంకె_mods = ARRAY_కొలత(builtin_mods);
    keymap->canonical_state_ముసుగు = MOD_REAL_ముసుగు_ALL;
}

నిర్మాణం xkb_keymap *
xkb_keymap_new(నిర్మాణం xkb_context *ctx, స్థిరరాశి అక్షరం *func,
               గణన xkb_keymap_format format,
               గణన xkb_keymap_compile_flags flags)
{
    స్థిరరాశి గణన xkb_keymap_compile_flags invalid_flags = (
        flags & ~(గణన xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_విలువలు
    );
    ఒకవేళ (invalid_flags) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        తిరిగి ఖాళీ;
    }

    నిర్మాణం xkb_keymap * స్థిరరాశి keymap = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(*keymap));
    ఒకవేళ (!keymap)
        తిరిగి ఖాళీ;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    తిరిగి keymap;
}

శూన్య
XkbEscapeMapName(అక్షరం *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    స్థిర స్థిరరాశి గుర్తులేని అక్షరం legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    ఒకవేళ (!name)
        తిరిగి;

    అయితే (*name) {
        గుర్తులేని అక్షరం c = (గుర్తులేని అక్షరం)*name;
        ఒకవేళ (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_సూచీ_t
Xkb_Mod_Name_గమ్యం_సూచీ(స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods, xkb_atom_t name,
                  గణన mod_type type)
{
    xkb_mod_సూచీ_t i;
    స్థిరరాశి నిర్మాణం xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        ఒకవేళ ((mod->type & type) && name == mod->name)
            తిరిగి i;

    తిరిగి XKB_MOD_INVALID;
}

bool
Xkb_స్థాయిలు_Same_Syms(స్థిరరాశి నిర్మాణం xkb_level *a, స్థిరరాశి నిర్మాణం xkb_level *b)
{
    ఒకవేళ (a->అంకె_syms != b->అంకె_syms)
        తిరిగి false;
    ఒకవేళ (a->అంకె_syms <= 1)
        తిరిగి a->s.sym == b->s.sym;
    తిరిగి memcmp(a->s.syms, b->s.syms, పరిమాణం(*a->s.syms) * a->అంకె_syms) == 0;
}

bool
action_equal(స్థిరరాశి సంఘం xkb_action *a, స్థిరరాశి సంఘం xkb_action *b)
{
    ఒకవేళ (a->type != b->type)
        తిరిగి false;

    ఎంపిక (a->type) {
    స్థితి ACTION_TYPE_NONE:
    స్థితి ACTION_TYPE_VOID:
        తిరిగి true;
    స్థితి ACTION_TYPE_MOD_సమితి:
    స్థితి ACTION_TYPE_MOD_LATCH:
    స్థితి ACTION_TYPE_MOD_తాళం:
        తిరిగి (a->mods.flags == b->mods.flags &&
                a->mods.mods.ముసుగు == b->mods.mods.ముసుగు &&
                a->mods.mods.mods == b->mods.mods.mods);
    స్థితి ACTION_TYPE_సమూహం_సమితి:
    స్థితి ACTION_TYPE_సమూహం_LATCH:
    స్థితి ACTION_TYPE_సమూహం_తాళం:
        తిరిగి (a->సమూహం.flags == b->సమూహం.flags &&
                a->సమూహం.సమూహం == b->సమూహం.సమూహం);
    స్థితి ACTION_TYPE_PTR_MOVE:
        తిరిగి (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    స్థితి ACTION_TYPE_PTR_BUTTON:
    స్థితి ACTION_TYPE_PTR_తాళం:
        తిరిగి (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.లెక్క == b->btn.లెక్క);
    స్థితి ACTION_TYPE_PTR_DEFAULT:
        తిరిగి (a->dflt.flags == b->dflt.flags &&
                a->dflt.విలువ == b->dflt.విలువ);
    స్థితి ACTION_TYPE_TERMINATE:
        తిరిగి true;
    స్థితి ACTION_TYPE_SWITCH_VT:
        తిరిగి (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    స్థితి ACTION_TYPE_CTRL_సమితి:
    స్థితి ACTION_TYPE_CTRL_తాళం:
        తిరిగి (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    స్థితి ACTION_TYPE_REDIRECT_కీలకం:
        తిరిగి (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    స్థితి ACTION_TYPE_UNSUPPORTED_LEGACY:
    స్థితి ACTION_TYPE_UNKNOWN:
        తిరిగి true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    స్థితి ACTION_TYPE_అంతర్గత:
        తిరిగి (a->అంతర్గత.flags == b->అంతర్గత.flags) &&
               (a->అంతర్గత.clear_latched_mods == b->అంతర్గత.clear_latched_mods);
    అప్రమేయ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_అంతర్గత == 20 &&
                      ACTION_TYPE_అంతర్గత + 1 == ACTION_TYPE_అంకె_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        తిరిగి memcmp(a->priv.దత్తాంశం, b->priv.దత్తాంశం, పరిమాణం(a->priv.దత్తాంశం)) == 0;
    }
}

bool
Xkb_స్థాయిలు_Same_Actions(స్థిరరాశి నిర్మాణం xkb_level *a, స్థిరరాశి నిర్మాణం xkb_level *b)
{
    ఒకవేళ (a->అంకె_actions != b->అంకె_actions)
        తిరిగి false;
    ఒకవేళ (a->అంకె_actions <= 1)
        తిరిగి action_equal(&a->a.action, &b->a.action);
    కొరకు (xkb_action_లెక్క_t k = 0; k < a->అంకె_actions; k++) {
        ఒకవేళ (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            తిరిగి false;
    }
    తిరిగి true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_సూచీ_t
Xkb_Wrap_సమూహం_Into_Range(int32_t సమూహం,
                      xkb_layout_సూచీ_t అంకె_groups,
                      గణన xkb_layout_బహిర్గతం_యొక్క_range_policy బహిర్గతం_యొక్క_range_సమూహం_policy,
                      xkb_layout_సూచీ_t బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య)
{
    ఒకవేళ (అంకె_groups == 0)
        తిరిగి XKB_LAYOUT_INVALID;

    ఒకవేళ (సమూహం >= 0 && (xkb_layout_సూచీ_t)సమూహం < అంకె_groups)
        తిరిగి (xkb_layout_సూచీ_t)సమూహం;

    ఎంపిక (బహిర్గతం_యొక్క_range_సమూహం_policy) {
    స్థితి XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_REDIRECT:
        ఒకవేళ (బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య >= అంకె_groups)
            తిరిగి 0;
        తిరిగి బహిర్గతం_యొక్క_range_సమూహం_సంఖ్య;

    స్థితి XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_CLAMP:
        ఒకవేళ (సమూహం < 0)
            తిరిగి 0;
        లేకపోతే
            తిరిగి అంకె_groups - 1;

    స్థితి XKB_LAYOUT_బహిర్గతం_యొక్క_RANGE_WRAP:
    అప్రమేయ: {
        /*
         * Ensure conversion xkb_layout_index_t → int32_t is lossless.
         *
         * NOTE: C11 requires a compound statement because it does not allow
         * a declaration after a label (C23 does allow it).
         */
        static_assert(XKB_MAX_GROUPS < INT32_MAX, "Max groups max don't fit");
        /*
         * % returns the *remainder* of the division, not the modulus (see C11
         * standard 6.5.5 “Multiplicative operators”: a % b = a - (a/b)*b. While
         * both operators return the same result for positive operands, they do
         * not for e.g. a negative dividend: remainder may be negative (in the
         * open interval ]-num_groups, num_groups[) while the modulus is always
         * positive. So if the remainder is negative, we must add `num_groups`
         * to get the modulus.
         */
        స్థిరరాశి int32_t rem = సమూహం % (int32_t) అంకె_groups;
        తిరిగి (rem >= 0)
            ? (xkb_layout_సూచీ_t)rem
            : (xkb_layout_సూచీ_t)(rem + (int32_t)అంకె_groups);
    }
    }
}

xkb_action_లెక్క_t
xkb_keymap_కీలకం_get_actions_by_level(స్థిరరాశి నిర్మాణం xkb_keymap *keymap,
                                    స్థిరరాశి నిర్మాణం xkb_కీలకం *కీలకం,
                                    xkb_layout_సూచీ_t layout,
                                    xkb_level_సూచీ_t level,
                                    స్థిరరాశి సంఘం xkb_action **actions)
{
    ఒకవేళ (!కీలకం)
        వెళ్ళు err;

    layout = xkb_keymap_కీలకం_effective_layout(కీలకం, layout);
    ఒకవేళ (layout == XKB_LAYOUT_INVALID)
        వెళ్ళు err;

    ఒకవేళ (level >= Xkb_కీలకం_అంకె_స్థాయిలు(కీలకం, layout))
        వెళ్ళు err;

    స్థిరరాశి xkb_action_లెక్క_t లెక్క =
        కీలకం->groups[layout].స్థాయిలు[level].అంకె_actions;
    ఎంపిక (లెక్క) {
        స్థితి 0:
            వెళ్ళు err;
        స్థితి 1:
            *actions = &కీలకం->groups[layout].స్థాయిలు[level].a.action;
            ఆపు;
        అప్రమేయ:
            *actions = కీలకం->groups[layout].స్థాయిలు[level].a.actions;
    }
    తిరిగి లెక్క;

err:
    *actions = ఖాళీ;
    తిరిగి 0;
}
