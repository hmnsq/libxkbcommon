/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcommon/xkbcommon-names.h"
#अन्तर्भाव "features/enums.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "messages-codes.h"

स्थिर शून्य
update_builtin_keymap_fields(संरचना xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    स्थिर स्थिरांक अक्षर *स्थिरांक builtin_mods[] = {
        [XKB_MOD_INDEX_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_INDEX_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_INDEX_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_INDEX_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_INDEX_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_INDEX_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_INDEX_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_INDEX_MOD5]  = XKB_MOD_NAME_MOD5
    };

    साठी (xkb_mod_index_t i = 0; i < ARRAY_SIZE(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    म_माप(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.अंक_mods = ARRAY_SIZE(builtin_mods);
    keymap->canonical_state_mask = MOD_REAL_MASK_ALL;
}

संरचना xkb_keymap *
xkb_keymap_new(संरचना xkb_context *ctx, स्थिरांक अक्षर *func,
               गणना xkb_keymap_format format,
               गणना xkb_keymap_compile_flags flags)
{
    स्थिरांक गणना xkb_keymap_compile_flags invalid_flags = (
        flags & ~(गणना xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_मूल्ये
    );
    जर (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        परत रिक्त;
    }

    संरचना xkb_keymap * स्थिरांक keymap = सुस्मृति(1, आकार(*keymap));
    जर (!keymap)
        परत रिक्त;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    परत keymap;
}

शून्य
XkbEscapeMapName(अक्षर *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    स्थिर स्थिरांक अचिन्हित अक्षर legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    जर (!name)
        परत;

    पर्यंत (*name) {
        अचिन्हित अक्षर c = (अचिन्हित अक्षर)*name;
        जर (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_index_t
XkbModNameToIndex(स्थिरांक संरचना xkb_mod_set *mods, xkb_atom_t name,
                  गणना mod_type type)
{
    xkb_mod_index_t i;
    स्थिरांक संरचना xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        जर ((mod->type & type) && name == mod->name)
            परत i;

    परत XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(स्थिरांक संरचना xkb_level *a, स्थिरांक संरचना xkb_level *b)
{
    जर (a->अंक_syms != b->अंक_syms)
        परत false;
    जर (a->अंक_syms <= 1)
        परत a->s.sym == b->s.sym;
    परत स_भेद(a->s.syms, b->s.syms, आकार(*a->s.syms) * a->अंक_syms) == 0;
}

bool
action_equal(स्थिरांक संघ xkb_action *a, स्थिरांक संघ xkb_action *b)
{
    जर (a->type != b->type)
        परत false;

    निवड (a->type) {
    स्थिती ACTION_TYPE_NONE:
    स्थिती ACTION_TYPE_VOID:
        परत true;
    स्थिती ACTION_TYPE_MOD_SET:
    स्थिती ACTION_TYPE_MOD_LATCH:
    स्थिती ACTION_TYPE_MOD_LOCK:
        परत (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    स्थिती ACTION_TYPE_GROUP_SET:
    स्थिती ACTION_TYPE_GROUP_LATCH:
    स्थिती ACTION_TYPE_GROUP_LOCK:
        परत (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    स्थिती ACTION_TYPE_PTR_MOVE:
        परत (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    स्थिती ACTION_TYPE_PTR_BUTTON:
    स्थिती ACTION_TYPE_PTR_LOCK:
        परत (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.मोजणी == b->btn.मोजणी);
    स्थिती ACTION_TYPE_PTR_DEFAULT:
        परत (a->dflt.flags == b->dflt.flags &&
                a->dflt.मूल्य == b->dflt.मूल्य);
    स्थिती ACTION_TYPE_TERMINATE:
        परत true;
    स्थिती ACTION_TYPE_SWITCH_VT:
        परत (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    स्थिती ACTION_TYPE_CTRL_SET:
    स्थिती ACTION_TYPE_CTRL_LOCK:
        परत (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    स्थिती ACTION_TYPE_REDIRECT_किल्ली:
        परत (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    स्थिती ACTION_TYPE_UNSUPPORTED_LEGACY:
    स्थिती ACTION_TYPE_UNKNOWN:
        परत true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    स्थिती ACTION_TYPE_INTERNAL:
        परत (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    पूर्वनिर्धारित:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        परत स_भेद(a->priv.data, b->priv.data, आकार(a->priv.data)) == 0;
    }
}

bool
XkbLevelsSameActions(स्थिरांक संरचना xkb_level *a, स्थिरांक संरचना xkb_level *b)
{
    जर (a->अंक_actions != b->अंक_actions)
        परत false;
    जर (a->अंक_actions <= 1)
        परत action_equal(&a->a.action, &b->a.action);
    साठी (xkb_action_मोजणी_t k = 0; k < a->अंक_actions; k++) {
        जर (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            परत false;
    }
    परत true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_index_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_index_t अंक_groups,
                      गणना xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_index_t out_of_range_group_संख्या)
{
    जर (अंक_groups == 0)
        परत XKB_LAYOUT_INVALID;

    जर (group >= 0 && (xkb_layout_index_t)group < अंक_groups)
        परत (xkb_layout_index_t)group;

    निवड (out_of_range_group_policy) {
    स्थिती XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        जर (out_of_range_group_संख्या >= अंक_groups)
            परत 0;
        परत out_of_range_group_संख्या;

    स्थिती XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        जर (group < 0)
            परत 0;
        अन्यथा
            परत अंक_groups - 1;

    स्थिती XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    पूर्वनिर्धारित: {
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
        स्थिरांक int32_t rem = group % (int32_t) अंक_groups;
        परत (rem >= 0)
            ? (xkb_layout_index_t)rem
            : (xkb_layout_index_t)(rem + (int32_t)अंक_groups);
    }
    }
}

xkb_action_मोजणी_t
xkb_keymap_किल्ली_get_actions_by_level(स्थिरांक संरचना xkb_keymap *keymap,
                                    स्थिरांक संरचना xkb_किल्ली *किल्ली,
                                    xkb_layout_index_t layout,
                                    xkb_level_index_t level,
                                    स्थिरांक संघ xkb_action **actions)
{
    जर (!किल्ली)
        जा err;

    layout = xkb_keymap_किल्ली_effective_layout(किल्ली, layout);
    जर (layout == XKB_LAYOUT_INVALID)
        जा err;

    जर (level >= Xkb_किल्ली_अंक_Levels(किल्ली, layout))
        जा err;

    स्थिरांक xkb_action_मोजणी_t मोजणी =
        किल्ली->groups[layout].levels[level].अंक_actions;
    निवड (मोजणी) {
        स्थिती 0:
            जा err;
        स्थिती 1:
            *actions = &किल्ली->groups[layout].levels[level].a.action;
            थांबा;
        पूर्वनिर्धारित:
            *actions = किल्ली->groups[layout].levels[level].a.actions;
    }
    परत मोजणी;

err:
    *actions = रिक्त;
    परत 0;
}
