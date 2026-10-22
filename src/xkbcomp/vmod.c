/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <stdint.h>

#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "atom.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "text.h"
#અન્તર્ભાવ "expr.h"
#અન્તર્ભાવ "xkbcomp/ast.h"
#અન્તર્ભાવ "vmod.h"

શૂન્ય
InitVMods(સંરચના xkb_mod_set *info, અચલ સંરચના xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    જો (!reset)
        પાછા;

    સંરચના xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

શૂન્ય
MergeModSets(સંરચના xkb_context *ctx, સંરચના xkb_mod_set *into,
             અચલ સંરચના xkb_mod_set *from, ગણના merge_mode merge) {
    અચલ bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    અચલ સંરચના xkb_mod *mod;
    પ્રતિજ્ઞા(into->અંક_mods <= from->અંક_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        અચલ xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        જો ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            પ્રતિજ્ઞા((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            ચાલુ;
        } નહીંતર જો (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            પ્રતિજ્ઞા(into->mods[vmod].name == XKB_ATOM_NONE);
            પ્રતિજ્ઞા(mod->type == MOD_VIRT);
            પ્રતિજ્ઞા(mod->name != XKB_ATOM_NONE);
            પ્રતિજ્ઞા(vmod >= into->અંક_mods);
            into->mods[vmod] = *mod;
            જો (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } નહીંતર {
            /* Modifier exists in both */
            પ્રતિજ્ઞા(mod->type == MOD_VIRT);
            પ્રતિજ્ઞા(mod->name != XKB_ATOM_NONE);
            પ્રતિજ્ઞા(into->mods[vmod].type == mod->type);
            પ્રતિજ્ઞા(into->mods[vmod].name == mod->name);

            જો (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                પ્રતિજ્ઞા(mod->mapping == 0);
            } નહીંતર જો (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                પ્રતિજ્ઞા(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } નહીંતર જો (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                અચલ xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                અચલ xkb_mod_mask_t ignore = (clobber)
                    ? into->mods[vmod].mapping
                    : mod->mapping;

                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_text(ctx, mod->name),
                         ModMaskText(ctx, MOD_REAL, from, use),
                         ModMaskText(ctx, MOD_REAL, from, ignore));

                into->mods[vmod].mapping = use;
            }
        }
    }
    into->અંક_mods = from->અંક_mods;
}

bool
HandleVModDef(સંરચના xkb_context *ctx, સંરચના xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    જો (stmt->કિંમત &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !પદાવલિ_Resolve_Mod_Mask(ctx, stmt->કિંમત, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        પાછા false;
    }

    xkb_mod_index_t vmod;
    સંરચના xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        જો (mod->name == stmt->name) {
            જો (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                પાછા false;
            }

            અચલ xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            જો (!stmt->કિંમત) {
                /* No new explicit mapping: do nothing */
                પાછા true;
            } નહીંતર જો (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } નહીંતર જો (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                અચલ bool clobber = (stmt->merge != MERGE_AUGMENT);
                અચલ xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                અચલ xkb_mod_mask_t ignore =
                    (clobber ? mod->mapping : mapping);

                log_warn(ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_text(ctx, stmt->name),
                         ModMaskText(ctx, MOD_REAL, mods, use),
                         ModMaskText(ctx, MOD_REAL, mods, ignore));

                mod->mapping = use;
            }

            mods->explicit_vmods |= mask;

            પાછા true;
        }
    }

    જો (mods->અંક_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        પાછા false;
    }

    mods->mods[mods->અંક_mods].name = stmt->name;
    mods->mods[mods->અંક_mods].type = MOD_VIRT;
    mods->mods[mods->અંક_mods].mapping = mapping;
    જો (stmt->કિંમત) {
        અચલ xkb_mod_mask_t mask = UINT32_C(1) << mods->અંક_mods;
        mods->explicit_vmods |= mask;
    }
    mods->અંક_mods++;
    પાછા true;
}
