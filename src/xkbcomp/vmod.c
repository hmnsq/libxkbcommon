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
InitVMods(ರಚನೆ xkb_mod_ಗಣ *info, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *mods, bool reset)
{
    *info = *mods;

    ಆದರೆ (!reset)
        ಹಿಂತಿರುಗಿ;

    ರಚನೆ xkb_mod *mod;
    xkb_mod_ಸೂಚ್ಯಂಕ_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

ಶೂನ್ಯ
ವಿಲೀನ_Mod_Sets(ರಚನೆ xkb_context *ctx, ರಚನೆ xkb_mod_ಗಣ *into,
             ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod_ಗಣ *from, ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ) {
    ಸ್ಥಿರಾಂಕ bool clobber = (ವಿಲೀನ != ವಿಲೀನ_AUGMENT);
    xkb_mod_ಸೂಚ್ಯಂಕ_t vmod;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_mod *mod;
    ಪ್ರತಿಜ್ಞಾ(into->ಅಂಕೆ_mods <= from->ಅಂಕೆ_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ = UINT32_C(1) << vmod;

        ಆದರೆ ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            ಪ್ರತಿಜ್ಞಾ((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            ಮುಂದುವರಿ;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            ಪ್ರತಿಜ್ಞಾ(into->mods[vmod].name == XKB_ATOM_NONE);
            ಪ್ರತಿಜ್ಞಾ(mod->type == MOD_VIRT);
            ಪ್ರತಿಜ್ಞಾ(mod->name != XKB_ATOM_NONE);
            ಪ್ರತಿಜ್ಞಾ(vmod >= into->ಅಂಕೆ_mods);
            into->mods[vmod] = *mod;
            ಆದರೆ (from->explicit_vmods & ಮುಖವಾಡ)
                into->explicit_vmods |= ಮುಖವಾಡ;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Modifier exists in both */
            ಪ್ರತಿಜ್ಞಾ(mod->type == MOD_VIRT);
            ಪ್ರತಿಜ್ಞಾ(mod->name != XKB_ATOM_NONE);
            ಪ್ರತಿಜ್ಞಾ(into->mods[vmod].type == mod->type);
            ಪ್ರತಿಜ್ಞಾ(into->mods[vmod].name == mod->name);

            ಆದರೆ (!(from->explicit_vmods & ಮುಖವಾಡ)) {
                /* Implicit mapping in `from`: do nothing */
                ಪ್ರತಿಜ್ಞಾ(mod->mapping == 0);
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(into->explicit_vmods & ಮುಖವಾಡ)) {
                /* Implicit mapping in `into`: replace */
                ಪ್ರತಿಜ್ಞಾ(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= ಮುಖವಾಡ;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t ignore = (clobber)
                    ? into->mods[vmod].mapping
                    : mod->mapping;

                log_warn(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_ಪಠ್ಯ(ctx, mod->name),
                         Mod_ಮುಖವಾಡ_ಪಠ್ಯ(ctx, MOD_REAL, from, use),
                         Mod_ಮುಖವಾಡ_ಪಠ್ಯ(ctx, MOD_REAL, from, ignore));

                into->mods[vmod].mapping = use;
            }
        }
    }
    into->ಅಂಕೆ_mods = from->ಅಂಕೆ_mods;
}

bool
HandleVModDef(ರಚನೆ xkb_context *ctx, ರಚನೆ xkb_mod_ಗಣ *mods, VModDef *stmt)
{
    xkb_mod_ಮುಖವಾಡ_t mapping = 0;
    ಆದರೆ (stmt->ಮೌಲ್ಯ &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !ಅಭಿವ್ಯಕ್ತಿ_Resolve_Mod_ಮುಖವಾಡ(ctx, stmt->ಮೌಲ್ಯ, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_ಪಠ್ಯ(ctx, stmt->name));
        ಹಿಂತಿರುಗಿ false;
    }

    xkb_mod_ಸೂಚ್ಯಂಕ_t vmod;
    ರಚನೆ xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        ಆದರೆ (mod->name == stmt->name) {
            ಆದರೆ (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_ಪಠ್ಯ(ctx, mod->name));
                ಹಿಂತಿರುಗಿ false;
            }

            ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ = UINT32_C(1) << vmod;

            ಆದರೆ (!stmt->ಮೌಲ್ಯ) {
                /* No new explicit mapping: do nothing */
                ಹಿಂತಿರುಗಿ true;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!(mods->explicit_vmods & ಮುಖವಾಡ)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                ಸ್ಥಿರಾಂಕ bool clobber = (stmt->ವಿಲೀನ != ವಿಲೀನ_AUGMENT);
                ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t use =
                    (clobber ? mapping : mod->mapping);
                ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t ignore =
                    (clobber ? mod->mapping : mapping);

                log_warn(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_ಪಠ್ಯ(ctx, stmt->name),
                         Mod_ಮುಖವಾಡ_ಪಠ್ಯ(ctx, MOD_REAL, mods, use),
                         Mod_ಮುಖವಾಡ_ಪಠ್ಯ(ctx, MOD_REAL, mods, ignore));

                mod->mapping = use;
            }

            mods->explicit_vmods |= ಮುಖವಾಡ;

            ಹಿಂತಿರುಗಿ true;
        }
    }

    ಆದರೆ (mods->ಅಂಕೆ_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_ಪಠ್ಯ(ctx, stmt->name), XKB_MAX_MODS);
        ಹಿಂತಿರುಗಿ false;
    }

    mods->mods[mods->ಅಂಕೆ_mods].name = stmt->name;
    mods->mods[mods->ಅಂಕೆ_mods].type = MOD_VIRT;
    mods->mods[mods->ಅಂಕೆ_mods].mapping = mapping;
    ಆದರೆ (stmt->ಮೌಲ್ಯ) {
        ಸ್ಥಿರಾಂಕ xkb_mod_ಮುಖವಾಡ_t ಮುಖವಾಡ = UINT32_C(1) << mods->ಅಂಕೆ_mods;
        mods->explicit_vmods |= ಮುಖವಾಡ;
    }
    mods->ಅಂಕೆ_mods++;
    ಹಿಂತಿರುಗಿ true;
}
