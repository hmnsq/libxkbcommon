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
InitVMods(بنية xkb_mod_سېټ *info, ثابت بنية xkb_mod_سېټ *mods, bool reset)
{
    *info = *mods;

    إذا (!reset)
        أرجع;

    بنية xkb_mod *mod;
    xkb_mod_شاخص_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

فراغ
یوځای_Mod_Sets(بنية xkb_context *ctx, بنية xkb_mod_سېټ *into,
             ثابت بنية xkb_mod_سېټ *from, تعداد یوځای_ډېرپېښ یوځای) {
    ثابت bool clobber = (یوځای != یوځای_AUGMENT);
    xkb_mod_شاخص_t vmod;
    ثابت بنية xkb_mod *mod;
    أكد(into->عدد_mods <= from->عدد_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        ثابت xkb_mod_ماسک_t ماسک = UINT32_C(1) << vmod;

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
            أكد(vmod >= into->عدد_mods);
            into->mods[vmod] = *mod;
            إذا (from->explicit_vmods & ماسک)
                into->explicit_vmods |= ماسک;
        } وإلا {
            /* Modifier exists in both */
            أكد(mod->type == MOD_VIRT);
            أكد(mod->name != XKB_ATOM_NONE);
            أكد(into->mods[vmod].type == mod->type);
            أكد(into->mods[vmod].name == mod->name);

            إذا (!(from->explicit_vmods & ماسک)) {
                /* Implicit mapping in `from`: do nothing */
                أكد(mod->mapping == 0);
            } وإلا إذا (!(into->explicit_vmods & ماسک)) {
                /* Implicit mapping in `into`: replace */
                أكد(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= ماسک;
            } وإلا إذا (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                ثابت xkb_mod_ماسک_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                ثابت xkb_mod_ماسک_t ignore = (clobber)
                    ? into->mods[vmod].mapping
                    : mod->mapping;

                log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_متن(ctx, mod->name),
                         Mod_ماسک_متن(ctx, MOD_REAL, from, use),
                         Mod_ماسک_متن(ctx, MOD_REAL, from, ignore));

                into->mods[vmod].mapping = use;
            }
        }
    }
    into->عدد_mods = from->عدد_mods;
}

bool
HandleVModDef(بنية xkb_context *ctx, بنية xkb_mod_سېټ *mods, VModDef *stmt)
{
    xkb_mod_ماسک_t mapping = 0;
    إذا (stmt->ارزښت &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !بیان_Resolve_Mod_ماسک(ctx, stmt->ارزښت, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_متن(ctx, stmt->name));
        أرجع false;
    }

    xkb_mod_شاخص_t vmod;
    بنية xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        إذا (mod->name == stmt->name) {
            إذا (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_متن(ctx, mod->name));
                أرجع false;
            }

            ثابت xkb_mod_ماسک_t ماسک = UINT32_C(1) << vmod;

            إذا (!stmt->ارزښت) {
                /* No new explicit mapping: do nothing */
                أرجع true;
            } وإلا إذا (!(mods->explicit_vmods & ماسک)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } وإلا إذا (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                ثابت bool clobber = (stmt->یوځای != یوځای_AUGMENT);
                ثابت xkb_mod_ماسک_t use =
                    (clobber ? mapping : mod->mapping);
                ثابت xkb_mod_ماسک_t ignore =
                    (clobber ? mod->mapping : mapping);

                log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_متن(ctx, stmt->name),
                         Mod_ماسک_متن(ctx, MOD_REAL, mods, use),
                         Mod_ماسک_متن(ctx, MOD_REAL, mods, ignore));

                mod->mapping = use;
            }

            mods->explicit_vmods |= ماسک;

            أرجع true;
        }
    }

    إذا (mods->عدد_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_متن(ctx, stmt->name), XKB_MAX_MODS);
        أرجع false;
    }

    mods->mods[mods->عدد_mods].name = stmt->name;
    mods->mods[mods->عدد_mods].type = MOD_VIRT;
    mods->mods[mods->عدد_mods].mapping = mapping;
    إذا (stmt->ارزښت) {
        ثابت xkb_mod_ماسک_t ماسک = UINT32_C(1) << mods->عدد_mods;
        mods->explicit_vmods |= ماسک;
    }
    mods->عدد_mods++;
    أرجع true;
}
