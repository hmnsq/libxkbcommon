/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <stdint.h>

#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "atom.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "text.h"
#অন্তর্ভাব "expr.h"
#অন্তর্ভাব "xkbcomp/ast.h"
#অন্তর্ভাব "vmod.h"

শূন্য
InitVMods(সংরচনা xkb_mod_set *info, ধ্রুব সংরচনা xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    যদি (!reset)
        প্রত্যাগম;

    সংরচনা xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

শূন্য
MergeModSets(সংরচনা xkb_context *ctx, সংরচনা xkb_mod_set *into,
             ধ্রুব সংরচনা xkb_mod_set *from, গণনা merge_mode merge) {
    ধ্রুব bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    ধ্রুব সংরচনা xkb_mod *mod;
    প্রতিজ্ঞা(into->অঙ্ক_mods <= from->অঙ্ক_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        ধ্রুব xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        যদি ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            প্রতিজ্ঞা((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            অনুবর্ত;
        } অন্যথা যদি (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            প্রতিজ্ঞা(into->mods[vmod].name == XKB_ATOM_NONE);
            প্রতিজ্ঞা(mod->type == MOD_VIRT);
            প্রতিজ্ঞা(mod->name != XKB_ATOM_NONE);
            প্রতিজ্ঞা(vmod >= into->অঙ্ক_mods);
            into->mods[vmod] = *mod;
            যদি (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } অন্যথা {
            /* Modifier exists in both */
            প্রতিজ্ঞা(mod->type == MOD_VIRT);
            প্রতিজ্ঞা(mod->name != XKB_ATOM_NONE);
            প্রতিজ্ঞা(into->mods[vmod].type == mod->type);
            প্রতিজ্ঞা(into->mods[vmod].name == mod->name);

            যদি (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                প্রতিজ্ঞা(mod->mapping == 0);
            } অন্যথা যদি (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                প্রতিজ্ঞা(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } অন্যথা যদি (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                ধ্রুব xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                ধ্রুব xkb_mod_mask_t ignore = (clobber)
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
    into->অঙ্ক_mods = from->অঙ্ক_mods;
}

bool
HandleVModDef(সংরচনা xkb_context *ctx, সংরচনা xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    যদি (stmt->মান &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !রাশিমালা_Resolve_Mod_Mask(ctx, stmt->মান, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        প্রত্যাগম false;
    }

    xkb_mod_index_t vmod;
    সংরচনা xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        যদি (mod->name == stmt->name) {
            যদি (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                প্রত্যাগম false;
            }

            ধ্রুব xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            যদি (!stmt->মান) {
                /* No new explicit mapping: do nothing */
                প্রত্যাগম true;
            } অন্যথা যদি (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } অন্যথা যদি (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                ধ্রুব bool clobber = (stmt->merge != MERGE_AUGMENT);
                ধ্রুব xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                ধ্রুব xkb_mod_mask_t ignore =
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

            প্রত্যাগম true;
        }
    }

    যদি (mods->অঙ্ক_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        প্রত্যাগম false;
    }

    mods->mods[mods->অঙ্ক_mods].name = stmt->name;
    mods->mods[mods->অঙ্ক_mods].type = MOD_VIRT;
    mods->mods[mods->অঙ্ক_mods].mapping = mapping;
    যদি (stmt->মান) {
        ধ্রুব xkb_mod_mask_t mask = UINT32_C(1) << mods->অঙ্ক_mods;
        mods->explicit_vmods |= mask;
    }
    mods->অঙ্ক_mods++;
    প্রত্যাগম true;
}
