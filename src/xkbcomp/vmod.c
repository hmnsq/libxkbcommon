/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ضمن "config.h"

#ضمن <stdint.h>

#ضمن "xkbcommon/xkbcommon.h"
#ضمن "atom.h"
#ضمن "keymap.h"
#ضمن "text.h"
#ضمن "expr.h"
#ضمن "xkbcomp/ast.h"
#ضمن "vmod.h"

فراغ
InitVMods(بنية xkb_mod_set *info, ثابت بنية xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    إذا (!reset)
        أرجع;

    بنية xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

فراغ
MergeModSets(بنية xkb_context *ctx, بنية xkb_mod_set *into,
             ثابت بنية xkb_mod_set *from, تعداد merge_mode merge) {
    ثابت bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    ثابت بنية xkb_mod *mod;
    أكد(into->رەقەم_mods <= from->رەقەم_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        ثابت xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        إذا ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            أكد((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            استمر;
        } وإلا إذا (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            أكد(into->mods[vmod].name == XKB_ATOM_NONE);
            أكد(mod->type == MOD_VIRT);
            أكد(mod->name != XKB_ATOM_NONE);
            أكد(vmod >= into->رەقەم_mods);
            into->mods[vmod] = *mod;
            إذا (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } وإلا {
            /* Modifier exists in both */
            أكد(mod->type == MOD_VIRT);
            أكد(mod->name != XKB_ATOM_NONE);
            أكد(into->mods[vmod].type == mod->type);
            أكد(into->mods[vmod].name == mod->name);

            إذا (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                أكد(mod->mapping == 0);
            } وإلا إذا (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                أكد(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } وإلا إذا (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                ثابت xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                ثابت xkb_mod_mask_t ignore = (clobber)
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
    into->رەقەم_mods = from->رەقەم_mods;
}

bool
HandleVModDef(بنية xkb_context *ctx, بنية xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    إذا (stmt->قىممەت &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !ئىپادە_Resolve_Mod_Mask(ctx, stmt->قىممەت, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        أرجع false;
    }

    xkb_mod_index_t vmod;
    بنية xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        إذا (mod->name == stmt->name) {
            إذا (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                أرجع false;
            }

            ثابت xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            إذا (!stmt->قىممەت) {
                /* No new explicit mapping: do nothing */
                أرجع true;
            } وإلا إذا (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } وإلا إذا (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                ثابت bool clobber = (stmt->merge != MERGE_AUGMENT);
                ثابت xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                ثابت xkb_mod_mask_t ignore =
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

            أرجع true;
        }
    }

    إذا (mods->رەقەم_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        أرجع false;
    }

    mods->mods[mods->رەقەم_mods].name = stmt->name;
    mods->mods[mods->رەقەم_mods].type = MOD_VIRT;
    mods->mods[mods->رەقەم_mods].mapping = mapping;
    إذا (stmt->قىممەت) {
        ثابت xkb_mod_mask_t mask = UINT32_C(1) << mods->رەقەم_mods;
        mods->explicit_vmods |= mask;
    }
    mods->رەقەم_mods++;
    أرجع true;
}
