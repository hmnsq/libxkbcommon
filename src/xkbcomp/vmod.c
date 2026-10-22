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
InitVMods(संरचना xkb_mod_set *info, अचर संरचना xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    यदि (!reset)
        फर्काऊ;

    संरचना xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

शून्य
MergeModSets(संरचना xkb_context *ctx, संरचना xkb_mod_set *into,
             अचर संरचना xkb_mod_set *from, गणना merge_mode merge) {
    अचर bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    अचर संरचना xkb_mod *mod;
    प्रतिज्ञा(into->अङ्क_mods <= from->अङ्क_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        अचर xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        यदि ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            प्रतिज्ञा((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            जारी;
        } अन्यथा यदि (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            प्रतिज्ञा(into->mods[vmod].name == XKB_ATOM_NONE);
            प्रतिज्ञा(mod->type == MOD_VIRT);
            प्रतिज्ञा(mod->name != XKB_ATOM_NONE);
            प्रतिज्ञा(vmod >= into->अङ्क_mods);
            into->mods[vmod] = *mod;
            यदि (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } अन्यथा {
            /* Modifier exists in both */
            प्रतिज्ञा(mod->type == MOD_VIRT);
            प्रतिज्ञा(mod->name != XKB_ATOM_NONE);
            प्रतिज्ञा(into->mods[vmod].type == mod->type);
            प्रतिज्ञा(into->mods[vmod].name == mod->name);

            यदि (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                प्रतिज्ञा(mod->mapping == 0);
            } अन्यथा यदि (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                प्रतिज्ञा(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } अन्यथा यदि (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                अचर xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                अचर xkb_mod_mask_t ignore = (clobber)
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
    into->अङ्क_mods = from->अङ्क_mods;
}

bool
HandleVModDef(संरचना xkb_context *ctx, संरचना xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    यदि (stmt->मान &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !अभिव्यक्ति_Resolve_Mod_Mask(ctx, stmt->मान, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        फर्काऊ false;
    }

    xkb_mod_index_t vmod;
    संरचना xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        यदि (mod->name == stmt->name) {
            यदि (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                फर्काऊ false;
            }

            अचर xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            यदि (!stmt->मान) {
                /* No new explicit mapping: do nothing */
                फर्काऊ true;
            } अन्यथा यदि (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } अन्यथा यदि (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                अचर bool clobber = (stmt->merge != MERGE_AUGMENT);
                अचर xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                अचर xkb_mod_mask_t ignore =
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

            फर्काऊ true;
        }
    }

    यदि (mods->अङ्क_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        फर्काऊ false;
    }

    mods->mods[mods->अङ्क_mods].name = stmt->name;
    mods->mods[mods->अङ्क_mods].type = MOD_VIRT;
    mods->mods[mods->अङ्क_mods].mapping = mapping;
    यदि (stmt->मान) {
        अचर xkb_mod_mask_t mask = UINT32_C(1) << mods->अङ्क_mods;
        mods->explicit_vmods |= mask;
    }
    mods->अङ्क_mods++;
    फर्काऊ true;
}
