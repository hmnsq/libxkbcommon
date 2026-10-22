/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#شامل "config.h"

#شامل <stdint.h>

#شامل "xkbcommon/xkbcommon.h"
#شامل "atom.h"
#شامل "keymap.h"
#شامل "text.h"
#شامل "expr.h"
#شامل "xkbcomp/ast.h"
#شامل "vmod.h"

تهی
InitVMods(ساختار xkb_mod_set *info, ثابت ساختار xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    اگر (!reset)
        بازگشت;

    ساختار xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

تهی
MergeModSets(ساختار xkb_context *ctx, ساختار xkb_mod_set *into,
             ثابت ساختار xkb_mod_set *from, شمارش merge_mode merge) {
    ثابت bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    ثابت ساختار xkb_mod *mod;
    أكد(into->رقم_mods <= from->رقم_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        ثابت xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        اگر ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            أكد((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            ادامه;
        } وگرنه اگر (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            أكد(into->mods[vmod].name == XKB_ATOM_NONE);
            أكد(mod->type == MOD_VIRT);
            أكد(mod->name != XKB_ATOM_NONE);
            أكد(vmod >= into->رقم_mods);
            into->mods[vmod] = *mod;
            اگر (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } وگرنه {
            /* Modifier exists in both */
            أكد(mod->type == MOD_VIRT);
            أكد(mod->name != XKB_ATOM_NONE);
            أكد(into->mods[vmod].type == mod->type);
            أكد(into->mods[vmod].name == mod->name);

            اگر (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                أكد(mod->mapping == 0);
            } وگرنه اگر (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                أكد(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } وگرنه اگر (mod->mapping != into->mods[vmod].mapping) {
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
    into->رقم_mods = from->رقم_mods;
}

bool
HandleVModDef(ساختار xkb_context *ctx, ساختار xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    اگر (stmt->مقدار &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !عبارت_Resolve_Mod_Mask(ctx, stmt->مقدار, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        بازگشت false;
    }

    xkb_mod_index_t vmod;
    ساختار xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        اگر (mod->name == stmt->name) {
            اگر (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                بازگشت false;
            }

            ثابت xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            اگر (!stmt->مقدار) {
                /* No new explicit mapping: do nothing */
                بازگشت true;
            } وگرنه اگر (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } وگرنه اگر (mod->mapping != mapping) {
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

            بازگشت true;
        }
    }

    اگر (mods->رقم_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        بازگشت false;
    }

    mods->mods[mods->رقم_mods].name = stmt->name;
    mods->mods[mods->رقم_mods].type = MOD_VIRT;
    mods->mods[mods->رقم_mods].mapping = mapping;
    اگر (stmt->مقدار) {
        ثابت xkb_mod_mask_t mask = UINT32_C(1) << mods->رقم_mods;
        mods->explicit_vmods |= mask;
    }
    mods->رقم_mods++;
    بازگشت true;
}
