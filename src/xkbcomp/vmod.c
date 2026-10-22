/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "expr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp/ast.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "vmod.h"

ਖਾਲੀ
InitVMods(ਬਣਤਰ xkb_mod_ਸੈੱਟ *info, ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, bool reset)
{
    *info = *mods;

    ਜੇ (!reset)
        ਵਾਪਸ;

    ਬਣਤਰ xkb_mod *mod;
    xkb_mod_ਸੂਚਕਾਂਕ_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

ਖਾਲੀ
ਮਿਲਾਓ_Mod_Sets(ਬਣਤਰ xkb_context *ctx, ਬਣਤਰ xkb_mod_ਸੈੱਟ *into,
             ਅਚਲ ਬਣਤਰ xkb_mod_ਸੈੱਟ *from, ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ) {
    ਅਚਲ bool clobber = (ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);
    xkb_mod_ਸੂਚਕਾਂਕ_t vmod;
    ਅਚਲ ਬਣਤਰ xkb_mod *mod;
    ਪ੍ਰਤਿਜ੍ਞਾ(into->ਅੰਕ_mods <= from->ਅੰਕ_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        ਅਚਲ xkb_mod_ਨਕਾਬ_t ਨਕਾਬ = UINT32_C(1) << vmod;

        ਜੇ ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            ਪ੍ਰਤਿਜ੍ਞਾ((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            ਜਾਰੀ;
        } ਨਹੀਂਤਾਂ ਜੇ (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            ਪ੍ਰਤਿਜ੍ਞਾ(into->mods[vmod].name == XKB_ATOM_NONE);
            ਪ੍ਰਤਿਜ੍ਞਾ(mod->type == MOD_VIRT);
            ਪ੍ਰਤਿਜ੍ਞਾ(mod->name != XKB_ATOM_NONE);
            ਪ੍ਰਤਿਜ੍ਞਾ(vmod >= into->ਅੰਕ_mods);
            into->mods[vmod] = *mod;
            ਜੇ (from->explicit_vmods & ਨਕਾਬ)
                into->explicit_vmods |= ਨਕਾਬ;
        } ਨਹੀਂਤਾਂ {
            /* Modifier exists in both */
            ਪ੍ਰਤਿਜ੍ਞਾ(mod->type == MOD_VIRT);
            ਪ੍ਰਤਿਜ੍ਞਾ(mod->name != XKB_ATOM_NONE);
            ਪ੍ਰਤਿਜ੍ਞਾ(into->mods[vmod].type == mod->type);
            ਪ੍ਰਤਿਜ੍ਞਾ(into->mods[vmod].name == mod->name);

            ਜੇ (!(from->explicit_vmods & ਨਕਾਬ)) {
                /* Implicit mapping in `from`: do nothing */
                ਪ੍ਰਤਿਜ੍ਞਾ(mod->mapping == 0);
            } ਨਹੀਂਤਾਂ ਜੇ (!(into->explicit_vmods & ਨਕਾਬ)) {
                /* Implicit mapping in `into`: replace */
                ਪ੍ਰਤਿਜ੍ਞਾ(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= ਨਕਾਬ;
            } ਨਹੀਂਤਾਂ ਜੇ (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                ਅਚਲ xkb_mod_ਨਕਾਬ_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                ਅਚਲ xkb_mod_ਨਕਾਬ_t ignore = (clobber)
                    ? into->mods[vmod].mapping
                    : mod->mapping;

                log_warn(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_ਪਾਠ(ctx, mod->name),
                         Mod_ਨਕਾਬ_ਪਾਠ(ctx, MOD_REAL, from, use),
                         Mod_ਨਕਾਬ_ਪਾਠ(ctx, MOD_REAL, from, ignore));

                into->mods[vmod].mapping = use;
            }
        }
    }
    into->ਅੰਕ_mods = from->ਅੰਕ_mods;
}

bool
HandleVModDef(ਬਣਤਰ xkb_context *ctx, ਬਣਤਰ xkb_mod_ਸੈੱਟ *mods, VModDef *stmt)
{
    xkb_mod_ਨਕਾਬ_t mapping = 0;
    ਜੇ (stmt->ਮੁੱਲ &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !ਵਿਅੰਜਕ_Resolve_Mod_ਨਕਾਬ(ctx, stmt->ਮੁੱਲ, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_ਪਾਠ(ctx, stmt->name));
        ਵਾਪਸ false;
    }

    xkb_mod_ਸੂਚਕਾਂਕ_t vmod;
    ਬਣਤਰ xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        ਜੇ (mod->name == stmt->name) {
            ਜੇ (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_ਪਾਠ(ctx, mod->name));
                ਵਾਪਸ false;
            }

            ਅਚਲ xkb_mod_ਨਕਾਬ_t ਨਕਾਬ = UINT32_C(1) << vmod;

            ਜੇ (!stmt->ਮੁੱਲ) {
                /* No new explicit mapping: do nothing */
                ਵਾਪਸ true;
            } ਨਹੀਂਤਾਂ ਜੇ (!(mods->explicit_vmods & ਨਕਾਬ)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } ਨਹੀਂਤਾਂ ਜੇ (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                ਅਚਲ bool clobber = (stmt->ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);
                ਅਚਲ xkb_mod_ਨਕਾਬ_t use =
                    (clobber ? mapping : mod->mapping);
                ਅਚਲ xkb_mod_ਨਕਾਬ_t ignore =
                    (clobber ? mod->mapping : mapping);

                log_warn(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_ਪਾਠ(ctx, stmt->name),
                         Mod_ਨਕਾਬ_ਪਾਠ(ctx, MOD_REAL, mods, use),
                         Mod_ਨਕਾਬ_ਪਾਠ(ctx, MOD_REAL, mods, ignore));

                mod->mapping = use;
            }

            mods->explicit_vmods |= ਨਕਾਬ;

            ਵਾਪਸ true;
        }
    }

    ਜੇ (mods->ਅੰਕ_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_ਪਾਠ(ctx, stmt->name), XKB_MAX_MODS);
        ਵਾਪਸ false;
    }

    mods->mods[mods->ਅੰਕ_mods].name = stmt->name;
    mods->mods[mods->ਅੰਕ_mods].type = MOD_VIRT;
    mods->mods[mods->ਅੰਕ_mods].mapping = mapping;
    ਜੇ (stmt->ਮੁੱਲ) {
        ਅਚਲ xkb_mod_ਨਕਾਬ_t ਨਕਾਬ = UINT32_C(1) << mods->ਅੰਕ_mods;
        mods->explicit_vmods |= ਨਕਾਬ;
    }
    mods->ਅੰਕ_mods++;
    ਵਾਪਸ true;
}
