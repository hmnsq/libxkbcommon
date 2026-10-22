/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <stdint.h>

#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "atom.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "expr.h"
#अन्तर्भाव "xkbcomp/ast.h"
#अन्तर्भाव "vmod.h"

शून्य
InitVMods(संरचना xkb_mod_set *info, ध्रुव संरचना xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    जइ (!reset)
        प्रत्यागम;

    संरचना xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

शून्य
MergeModSets(संरचना xkb_context *ctx, संरचना xkb_mod_set *into,
             ध्रुव संरचना xkb_mod_set *from, गणना merge_mode merge) {
    ध्रुव bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    ध्रुव संरचना xkb_mod *mod;
    प्रतिज्ञा(into->अङ्कः_mods <= from->अङ्कः_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        ध्रुव xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        जइ ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            प्रतिज्ञा((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            अनुवर्त;
        } अण्णहा जइ (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            प्रतिज्ञा(into->mods[vmod].name == XKB_ATOM_NONE);
            प्रतिज्ञा(mod->type == MOD_VIRT);
            प्रतिज्ञा(mod->name != XKB_ATOM_NONE);
            प्रतिज्ञा(vmod >= into->अङ्कः_mods);
            into->mods[vmod] = *mod;
            जइ (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } अण्णहा {
            /* Modifier exists in both */
            प्रतिज्ञा(mod->type == MOD_VIRT);
            प्रतिज्ञा(mod->name != XKB_ATOM_NONE);
            प्रतिज्ञा(into->mods[vmod].type == mod->type);
            प्रतिज्ञा(into->mods[vmod].name == mod->name);

            जइ (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                प्रतिज्ञा(mod->mapping == 0);
            } अण्णहा जइ (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                प्रतिज्ञा(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } अण्णहा जइ (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                ध्रुव xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                ध्रुव xkb_mod_mask_t ignore = (clobber)
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
    into->अङ्कः_mods = from->अङ्कः_mods;
}

bool
HandleVModDef(संरचना xkb_context *ctx, संरचना xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    जइ (stmt->मानम् &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !व्यञ्जकम्_Resolve_Mod_Mask(ctx, stmt->मानम्, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        प्रत्यागम false;
    }

    xkb_mod_index_t vmod;
    संरचना xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        जइ (mod->name == stmt->name) {
            जइ (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                प्रत्यागम false;
            }

            ध्रुव xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            जइ (!stmt->मानम्) {
                /* No new explicit mapping: do nothing */
                प्रत्यागम true;
            } अण्णहा जइ (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } अण्णहा जइ (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                ध्रुव bool clobber = (stmt->merge != MERGE_AUGMENT);
                ध्रुव xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                ध्रुव xkb_mod_mask_t ignore =
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

            प्रत्यागम true;
        }
    }

    जइ (mods->अङ्कः_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        प्रत्यागम false;
    }

    mods->mods[mods->अङ्कः_mods].name = stmt->name;
    mods->mods[mods->अङ्कः_mods].type = MOD_VIRT;
    mods->mods[mods->अङ्कः_mods].mapping = mapping;
    जइ (stmt->मानम्) {
        ध्रुव xkb_mod_mask_t mask = UINT32_C(1) << mods->अङ्कः_mods;
        mods->explicit_vmods |= mask;
    }
    mods->अङ्कः_mods++;
    प्रत्यागम true;
}
