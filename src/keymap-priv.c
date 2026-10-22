/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2012 Ran Benita <ran234@gmail.com>
 * SPDX-License-Identifier: MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <stdint.h>
#समावेश <string.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcommon/xkbcommon-names.h"
#समावेश "features/enums.h"
#समावेश "keymap.h"
#समावेश "messages-codes.h"

अटल व्योम
update_builtin_keymap_fields(काष्ठा xkb_keymap *keymap)
{
    /* Predefined (AKA real, core, X11) modifiers. The order is important! */
    अटल स्थिर अक्षर *स्थिर builtin_mods[] = {
        [XKB_MOD_सूचकांक_SHIFT] = XKB_MOD_NAME_SHIFT,
        [XKB_MOD_सूचकांक_CAPS]  = XKB_MOD_NAME_CAPS,
        [XKB_MOD_सूचकांक_CTRL]  = XKB_MOD_NAME_CTRL,
        [XKB_MOD_सूचकांक_MOD1]  = XKB_MOD_NAME_MOD1,
        [XKB_MOD_सूचकांक_MOD2]  = XKB_MOD_NAME_MOD2,
        [XKB_MOD_सूचकांक_MOD3]  = XKB_MOD_NAME_MOD3,
        [XKB_MOD_सूचकांक_MOD4]  = XKB_MOD_NAME_MOD4,
        [XKB_MOD_सूचकांक_MOD5]  = XKB_MOD_NAME_MOD5
    };

    क्रम (xkb_mod_सूचकांक_t i = 0; i < ARRAY_आकार(builtin_mods); i++) {
        keymap->mods.mods[i].name = xkb_atom_intern(keymap->ctx,
                                                    builtin_mods[i],
                                                    म_माप(builtin_mods[i]));
        keymap->mods.mods[i].type = MOD_REAL;
        /* Real modifiers have a canonical mapping */
        keymap->mods.mods[i].mapping = UINT32_C(1) << i;
    }
    keymap->mods.अंक_mods = ARRAY_आकार(builtin_mods);
    keymap->canonical_state_mask = MOD_REAL_MASK_ALL;
}

काष्ठा xkb_keymap *
xkb_keymap_new(काष्ठा xkb_context *ctx, स्थिर अक्षर *func,
               क्रमागत xkb_keymap_format format,
               क्रमागत xkb_keymap_compile_flags flags)
{
    स्थिर क्रमागत xkb_keymap_compile_flags invalid_flags = (
        flags & ~(क्रमागत xkb_keymap_compile_flags)XKB_KEYMAP_COMPILE_FLAGS_मूल्यावली
    );
    अगर (invalid_flags) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "%s: unrecognized keymap compilation flags: 0x%x\n",
                func, invalid_flags);
        वापस शून्य;
    }

    काष्ठा xkb_keymap * स्थिर keymap = सुस्मृति(1, माप(*keymap));
    अगर (!keymap)
        वापस शून्य;

    keymap->refcnt = 1;
    keymap->ctx = xkb_context_ref(ctx);

    keymap->format = format;
    keymap->flags = flags;

    update_builtin_keymap_fields(keymap);

    वापस keymap;
}

व्योम
XkbEscapeMapName(अक्षर *name)
{
    /*
     * All latin-1 alphanumerics, plus parens, slash, minus, underscore and
     * wildcards.
     */
    अटल स्थिर अचिन्हित अक्षर legal[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa7, 0xff, 0x83,
        0xfe, 0xff, 0xff, 0x87, 0xfe, 0xff, 0xff, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff
    };

    अगर (!name)
        वापस;

    जबतक (*name) {
        अचिन्हित अक्षर c = (अचिन्हित अक्षर)*name;
        अगर (!(legal[c / 8] & (1u << (c % 8))))
            *name = '_';
        name++;
    }
}

xkb_mod_सूचकांक_t
Xkb_Mod_Name_To_सूचकांक(स्थिर काष्ठा xkb_mod_set *mods, xkb_atom_t name,
                  क्रमागत mod_type type)
{
    xkb_mod_सूचकांक_t i;
    स्थिर काष्ठा xkb_mod *mod;

    xkb_mods_enumerate(i, mod, mods)
        अगर ((mod->type & type) && name == mod->name)
            वापस i;

    वापस XKB_MOD_INVALID;
}

bool
XkbLevelsSameSyms(स्थिर काष्ठा xkb_level *a, स्थिर काष्ठा xkb_level *b)
{
    अगर (a->अंक_syms != b->अंक_syms)
        वापस false;
    अगर (a->अंक_syms <= 1)
        वापस a->s.sym == b->s.sym;
    वापस स_भेद(a->s.syms, b->s.syms, माप(*a->s.syms) * a->अंक_syms) == 0;
}

bool
action_equal(स्थिर जोड़ xkb_action *a, स्थिर जोड़ xkb_action *b)
{
    अगर (a->type != b->type)
        वापस false;

    चयन (a->type) {
    हाल ACTION_TYPE_NONE:
    हाल ACTION_TYPE_VOID:
        वापस true;
    हाल ACTION_TYPE_MOD_SET:
    हाल ACTION_TYPE_MOD_LATCH:
    हाल ACTION_TYPE_MOD_LOCK:
        वापस (a->mods.flags == b->mods.flags &&
                a->mods.mods.mask == b->mods.mods.mask &&
                a->mods.mods.mods == b->mods.mods.mods);
    हाल ACTION_TYPE_GROUP_SET:
    हाल ACTION_TYPE_GROUP_LATCH:
    हाल ACTION_TYPE_GROUP_LOCK:
        वापस (a->group.flags == b->group.flags &&
                a->group.group == b->group.group);
    हाल ACTION_TYPE_PTR_MOVE:
        वापस (a->ptr.flags == b->ptr.flags &&
                a->ptr.x == b->ptr.x &&
                a->ptr.y == b->ptr.y);
    हाल ACTION_TYPE_PTR_BUTTON:
    हाल ACTION_TYPE_PTR_LOCK:
        वापस (a->btn.flags == b->btn.flags &&
                a->btn.button == b->btn.button &&
                a->btn.गिनती == b->btn.गिनती);
    हाल ACTION_TYPE_PTR_DEFAULT:
        वापस (a->dflt.flags == b->dflt.flags &&
                a->dflt.मूल्य == b->dflt.मूल्य);
    हाल ACTION_TYPE_TERMINATE:
        वापस true;
    हाल ACTION_TYPE_SWITCH_VT:
        वापस (a->screen.flags == b->screen.flags &&
                a->screen.screen == b->screen.screen);
    हाल ACTION_TYPE_CTRL_SET:
    हाल ACTION_TYPE_CTRL_LOCK:
        वापस (a->ctrls.flags == b->ctrls.flags &&
                a->ctrls.ctrls == b->ctrls.ctrls);
    हाल ACTION_TYPE_REDIRECT_कुंजी:
        वापस (a->redirect.keycode == b->redirect.keycode &&
                a->redirect.affect_mods == b->redirect.affect_mods &&
                a->redirect.mods == b->redirect.mods);
    हाल ACTION_TYPE_UNSUPPORTED_LEGACY:
    हाल ACTION_TYPE_UNKNOWN:
        वापस true;
    /* ACTION_TYPE_PRIVATE processed in the default case */
    हाल ACTION_TYPE_INTERNAL:
        वापस (a->internal.flags == b->internal.flags) &&
               (a->internal.clear_latched_mods == b->internal.clear_latched_mods);
    शेष:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
                      "Missing action type");
        /* Private/custom action */
        वापस स_भेद(a->priv.तथ्य, b->priv.तथ्य, माप(a->priv.तथ्य)) == 0;
    }
}

bool
XkbLevelsSameActions(स्थिर काष्ठा xkb_level *a, स्थिर काष्ठा xkb_level *b)
{
    अगर (a->अंक_actions != b->अंक_actions)
        वापस false;
    अगर (a->अंक_actions <= 1)
        वापस action_equal(&a->a.action, &b->a.action);
    क्रम (xkb_action_गिनती_t k = 0; k < a->अंक_actions; k++) {
        अगर (!action_equal(&a->a.actions[k], &b->a.actions[k]))
            वापस false;
    }
    वापस true;
}

/* See: XkbAdjustGroup in Xorg xserver */
xkb_layout_सूचकांक_t
XkbWrapGroupIntoRange(int32_t group,
                      xkb_layout_सूचकांक_t अंक_groups,
                      क्रमागत xkb_layout_out_of_range_policy out_of_range_group_policy,
                      xkb_layout_सूचकांक_t out_of_range_group_संख्या)
{
    अगर (अंक_groups == 0)
        वापस XKB_LAYOUT_INVALID;

    अगर (group >= 0 && (xkb_layout_सूचकांक_t)group < अंक_groups)
        वापस (xkb_layout_सूचकांक_t)group;

    चयन (out_of_range_group_policy) {
    हाल XKB_LAYOUT_OUT_OF_RANGE_REDIRECT:
        अगर (out_of_range_group_संख्या >= अंक_groups)
            वापस 0;
        वापस out_of_range_group_संख्या;

    हाल XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        अगर (group < 0)
            वापस 0;
        अन्यथा
            वापस अंक_groups - 1;

    हाल XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    शेष: {
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
        स्थिर int32_t rem = group % (int32_t) अंक_groups;
        वापस (rem >= 0)
            ? (xkb_layout_सूचकांक_t)rem
            : (xkb_layout_सूचकांक_t)(rem + (int32_t)अंक_groups);
    }
    }
}

xkb_action_गिनती_t
xkb_keymap_कुंजी_get_actions_by_level(स्थिर काष्ठा xkb_keymap *keymap,
                                    स्थिर काष्ठा xkb_कुंजी *कुंजी,
                                    xkb_layout_सूचकांक_t layout,
                                    xkb_level_सूचकांक_t level,
                                    स्थिर जोड़ xkb_action **actions)
{
    अगर (!कुंजी)
        जाओ err;

    layout = xkb_keymap_कुंजी_effective_layout(कुंजी, layout);
    अगर (layout == XKB_LAYOUT_INVALID)
        जाओ err;

    अगर (level >= Xkb_कुंजी_अंक_Levels(कुंजी, layout))
        जाओ err;

    स्थिर xkb_action_गिनती_t गिनती =
        कुंजी->groups[layout].levels[level].अंक_actions;
    चयन (गिनती) {
        हाल 0:
            जाओ err;
        हाल 1:
            *actions = &कुंजी->groups[layout].levels[level].a.action;
            अवरोध;
        शेष:
            *actions = कुंजी->groups[layout].levels[level].a.actions;
    }
    वापस गिनती;

err:
    *actions = शून्य;
    वापस 0;
}
