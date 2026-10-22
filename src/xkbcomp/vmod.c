/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <stdint.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp/ast.h"
#ಅನ್ತರ್ಭಾವ "vmod.h"

ಶೂನ್ಯ
InitVMods(ಸಂರಚನಾ xkb_mod_set *info, ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    ಯದಿ (!reset)
        ಪ್ರತ್ಯಾಗಮ;

    ಸಂರಚನಾ xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

ಶೂನ್ಯ
MergeModSets(ಸಂರಚನಾ xkb_context *ctx, ಸಂರಚನಾ xkb_mod_set *into,
             ಧ್ರುವ ಸಂರಚನಾ xkb_mod_set *from, ಗಣನಾ merge_mode merge) {
    ಧ್ರುವ bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    ಧ್ರುವ ಸಂರಚನಾ xkb_mod *mod;
    ಪ್ರತಿಜ್ಞಾ(into->ಅಂಕೆ_mods <= from->ಅಂಕೆ_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        ಧ್ರುವ xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        ಯದಿ ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            ಪ್ರತಿಜ್ಞಾ((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            ಅನುವರ್ತ;
        } ಅನ್ಯಥಾ ಯದಿ (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            ಪ್ರತಿಜ್ಞಾ(into->mods[vmod].name == XKB_ATOM_NONE);
            ಪ್ರತಿಜ್ಞಾ(mod->type == MOD_VIRT);
            ಪ್ರತಿಜ್ಞಾ(mod->name != XKB_ATOM_NONE);
            ಪ್ರತಿಜ್ಞಾ(vmod >= into->ಅಂಕೆ_mods);
            into->mods[vmod] = *mod;
            ಯದಿ (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } ಅನ್ಯಥಾ {
            /* Modifier exists in both */
            ಪ್ರತಿಜ್ಞಾ(mod->type == MOD_VIRT);
            ಪ್ರತಿಜ್ಞಾ(mod->name != XKB_ATOM_NONE);
            ಪ್ರತಿಜ್ಞಾ(into->mods[vmod].type == mod->type);
            ಪ್ರತಿಜ್ಞಾ(into->mods[vmod].name == mod->name);

            ಯದಿ (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                ಪ್ರತಿಜ್ಞಾ(mod->mapping == 0);
            } ಅನ್ಯಥಾ ಯದಿ (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                ಪ್ರತಿಜ್ಞಾ(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } ಅನ್ಯಥಾ ಯದಿ (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                ಧ್ರುವ xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                ಧ್ರುವ xkb_mod_mask_t ignore = (clobber)
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
    into->ಅಂಕೆ_mods = from->ಅಂಕೆ_mods;
}

bool
HandleVModDef(ಸಂರಚನಾ xkb_context *ctx, ಸಂರಚನಾ xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    ಯದಿ (stmt->ಮೌಲ್ಯ &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_Mask(ctx, stmt->ಮೌಲ್ಯ, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        ಪ್ರತ್ಯಾಗಮ false;
    }

    xkb_mod_index_t vmod;
    ಸಂರಚನಾ xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        ಯದಿ (mod->name == stmt->name) {
            ಯದಿ (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                ಪ್ರತ್ಯಾಗಮ false;
            }

            ಧ್ರುವ xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            ಯದಿ (!stmt->ಮೌಲ್ಯ) {
                /* No new explicit mapping: do nothing */
                ಪ್ರತ್ಯಾಗಮ true;
            } ಅನ್ಯಥಾ ಯದಿ (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } ಅನ್ಯಥಾ ಯದಿ (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                ಧ್ರುವ bool clobber = (stmt->merge != MERGE_AUGMENT);
                ಧ್ರುವ xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                ಧ್ರುವ xkb_mod_mask_t ignore =
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

            ಪ್ರತ್ಯಾಗಮ true;
        }
    }

    ಯದಿ (mods->ಅಂಕೆ_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    mods->mods[mods->ಅಂಕೆ_mods].name = stmt->name;
    mods->mods[mods->ಅಂಕೆ_mods].type = MOD_VIRT;
    mods->mods[mods->ಅಂಕೆ_mods].mapping = mapping;
    ಯದಿ (stmt->ಮೌಲ್ಯ) {
        ಧ್ರುವ xkb_mod_mask_t mask = UINT32_C(1) << mods->ಅಂಕೆ_mods;
        mods->explicit_vmods |= mask;
    }
    mods->ಅಂಕೆ_mods++;
    ಪ್ರತ್ಯಾಗಮ true;
}
