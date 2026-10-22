/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#include "config.h"

#include <stdint.h>

#include "xkbcommon/xkbcommon.h"
#include "atom.h"
#include "keymap.h"
#include "text.h"
#include "expr.h"
#include "xkbcomp/ast.h"
#include "vmod.h"

void
InitVMods(sa_nracanaa xkb_mod_set *info, const sa_nracanaa xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    ᨀᨘ (!reset)
        ᨀᨘᨒ;

    sa_nracanaa xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

void
MergeModSets(sa_nracanaa xkb_context *ctx, sa_nracanaa xkb_mod_set *into,
             const sa_nracanaa xkb_mod_set *from, ga_nanaa merge_mode merge) {
    const bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    const sa_nracanaa xkb_mod *mod;
    assert(into->num_mods <= from->num_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        const xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        ᨀᨘ ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            assert((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            continue;
        } ᨀᨘᨁ ᨀᨘ (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            assert(into->mods[vmod].name == XKB_ATOM_NONE);
            assert(mod->type == MOD_VIRT);
            assert(mod->name != XKB_ATOM_NONE);
            assert(vmod >= into->num_mods);
            into->mods[vmod] = *mod;
            ᨀᨘ (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } ᨀᨘᨁ {
            /* Modifier exists in both */
            assert(mod->type == MOD_VIRT);
            assert(mod->name != XKB_ATOM_NONE);
            assert(into->mods[vmod].type == mod->type);
            assert(into->mods[vmod].name == mod->name);

            ᨀᨘ (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                assert(mod->mapping == 0);
            } ᨀᨘᨁ ᨀᨘ (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                assert(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } ᨀᨘᨁ ᨀᨘ (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                const xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                const xkb_mod_mask_t ignore = (clobber)
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
    into->num_mods = from->num_mods;
}

bool
HandleVModDef(sa_nracanaa xkb_context *ctx, sa_nracanaa xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    ᨀᨘ (stmt->value &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !ExprResolveModMask(ctx, stmt->value, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        ᨀᨘᨒ false;
    }

    xkb_mod_index_t vmod;
    sa_nracanaa xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        ᨀᨘ (mod->name == stmt->name) {
            ᨀᨘ (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                ᨀᨘᨒ false;
            }

            const xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            ᨀᨘ (!stmt->value) {
                /* No new explicit mapping: do nothing */
                ᨀᨘᨒ true;
            } ᨀᨘᨁ ᨀᨘ (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } ᨀᨘᨁ ᨀᨘ (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                const bool clobber = (stmt->merge != MERGE_AUGMENT);
                const xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                const xkb_mod_mask_t ignore =
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

            ᨀᨘᨒ true;
        }
    }

    ᨀᨘ (mods->num_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        ᨀᨘᨒ false;
    }

    mods->mods[mods->num_mods].name = stmt->name;
    mods->mods[mods->num_mods].type = MOD_VIRT;
    mods->mods[mods->num_mods].mapping = mapping;
    ᨀᨘ (stmt->value) {
        const xkb_mod_mask_t mask = UINT32_C(1) << mods->num_mods;
        mods->explicit_vmods |= mask;
    }
    mods->num_mods++;
    ᨀᨘᨒ true;
}
