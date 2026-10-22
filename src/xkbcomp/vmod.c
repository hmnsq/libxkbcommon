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

خالی
InitVMods(ساخت xkb_mod_سیٹ *info, ثابت ساخت xkb_mod_سیٹ *mods, bool reset)
{
    *info = *mods;

    اگر (!reset)
        واپس;

    ساخت xkb_mod *mod;
    xkb_mod_اشاریہ_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

خالی
ملاؤ_Mod_Sets(ساخت xkb_context *ctx, ساخت xkb_mod_سیٹ *into,
             ثابت ساخت xkb_mod_سیٹ *from, شمار ملاؤ_منوال ملاؤ) {
    ثابت bool clobber = (ملاؤ != ملاؤ_AUGMENT);
    xkb_mod_اشاریہ_t vmod;
    ثابت ساخت xkb_mod *mod;
    أكد(into->عدد_mods <= from->عدد_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        ثابت xkb_mod_نقاب_t نقاب = UINT32_C(1) << vmod;

        اگر ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            أكد((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            جاری;
        } ورنہ اگر (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            أكد(into->mods[vmod].name == XKB_ATOM_NONE);
            أكد(mod->type == MOD_VIRT);
            أكد(mod->name != XKB_ATOM_NONE);
            أكد(vmod >= into->عدد_mods);
            into->mods[vmod] = *mod;
            اگر (from->explicit_vmods & نقاب)
                into->explicit_vmods |= نقاب;
        } ورنہ {
            /* Modifier exists in both */
            أكد(mod->type == MOD_VIRT);
            أكد(mod->name != XKB_ATOM_NONE);
            أكد(into->mods[vmod].type == mod->type);
            أكد(into->mods[vmod].name == mod->name);

            اگر (!(from->explicit_vmods & نقاب)) {
                /* Implicit mapping in `from`: do nothing */
                أكد(mod->mapping == 0);
            } ورنہ اگر (!(into->explicit_vmods & نقاب)) {
                /* Implicit mapping in `into`: replace */
                أكد(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= نقاب;
            } ورنہ اگر (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                ثابت xkb_mod_نقاب_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                ثابت xkb_mod_نقاب_t ignore = (clobber)
                    ? into->mods[vmod].mapping
                    : mod->mapping;

                log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_متن(ctx, mod->name),
                         Mod_نقاب_متن(ctx, MOD_REAL, from, use),
                         Mod_نقاب_متن(ctx, MOD_REAL, from, ignore));

                into->mods[vmod].mapping = use;
            }
        }
    }
    into->عدد_mods = from->عدد_mods;
}

bool
HandleVModDef(ساخت xkb_context *ctx, ساخت xkb_mod_سیٹ *mods, VModDef *stmt)
{
    xkb_mod_نقاب_t mapping = 0;
    اگر (stmt->قدر &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !اظہاریہ_Resolve_Mod_نقاب(ctx, stmt->قدر, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_متن(ctx, stmt->name));
        واپس false;
    }

    xkb_mod_اشاریہ_t vmod;
    ساخت xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        اگر (mod->name == stmt->name) {
            اگر (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_پیغام_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_متن(ctx, mod->name));
                واپس false;
            }

            ثابت xkb_mod_نقاب_t نقاب = UINT32_C(1) << vmod;

            اگر (!stmt->قدر) {
                /* No new explicit mapping: do nothing */
                واپس true;
            } ورنہ اگر (!(mods->explicit_vmods & نقاب)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } ورنہ اگر (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                ثابت bool clobber = (stmt->ملاؤ != ملاؤ_AUGMENT);
                ثابت xkb_mod_نقاب_t use =
                    (clobber ? mapping : mod->mapping);
                ثابت xkb_mod_نقاب_t ignore =
                    (clobber ? mod->mapping : mapping);

                log_warn(ctx, XKB_LOG_پیغام_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_متن(ctx, stmt->name),
                         Mod_نقاب_متن(ctx, MOD_REAL, mods, use),
                         Mod_نقاب_متن(ctx, MOD_REAL, mods, ignore));

                mod->mapping = use;
            }

            mods->explicit_vmods |= نقاب;

            واپس true;
        }
    }

    اگر (mods->عدد_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_پیغام_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_متن(ctx, stmt->name), XKB_MAX_MODS);
        واپس false;
    }

    mods->mods[mods->عدد_mods].name = stmt->name;
    mods->mods[mods->عدد_mods].type = MOD_VIRT;
    mods->mods[mods->عدد_mods].mapping = mapping;
    اگر (stmt->قدر) {
        ثابت xkb_mod_نقاب_t نقاب = UINT32_C(1) << mods->عدد_mods;
        mods->explicit_vmods |= نقاب;
    }
    mods->عدد_mods++;
    واپس true;
}
