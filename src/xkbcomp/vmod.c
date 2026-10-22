/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <stdint.h>

#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "atom.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "expr.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp/ast.h"
#ଅନ୍ତର୍ଭାଵ "vmod.h"

ଶୂନ୍ୟ
InitVMods(ସଂରଚନା xkb_mod_set *info, ଅଚଳ ସଂରଚନା xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    ଯଦି (!reset)
        ଫେରନ୍ତୁ;

    ସଂରଚନା xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

ଶୂନ୍ୟ
MergeModSets(ସଂରଚନା xkb_context *ctx, ସଂରଚନା xkb_mod_set *into,
             ଅଚଳ ସଂରଚନା xkb_mod_set *from, ଗଣନା merge_mode merge) {
    ଅଚଳ bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    ଅଚଳ ସଂରଚନା xkb_mod *mod;
    ପ୍ରତିଜ୍ଞା(into->ଅଙ୍କ_mods <= from->ଅଙ୍କ_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        ଅଚଳ xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        ଯଦି ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            ପ୍ରତିଜ୍ଞା((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            ଜାରି;
        } ଅନ୍ୟଥା ଯଦି (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            ପ୍ରତିଜ୍ଞା(into->mods[vmod].name == XKB_ATOM_NONE);
            ପ୍ରତିଜ୍ଞା(mod->type == MOD_VIRT);
            ପ୍ରତିଜ୍ଞା(mod->name != XKB_ATOM_NONE);
            ପ୍ରତିଜ୍ଞା(vmod >= into->ଅଙ୍କ_mods);
            into->mods[vmod] = *mod;
            ଯଦି (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } ଅନ୍ୟଥା {
            /* Modifier exists in both */
            ପ୍ରତିଜ୍ଞା(mod->type == MOD_VIRT);
            ପ୍ରତିଜ୍ଞା(mod->name != XKB_ATOM_NONE);
            ପ୍ରତିଜ୍ଞା(into->mods[vmod].type == mod->type);
            ପ୍ରତିଜ୍ଞା(into->mods[vmod].name == mod->name);

            ଯଦି (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                ପ୍ରତିଜ୍ଞା(mod->mapping == 0);
            } ଅନ୍ୟଥା ଯଦି (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                ପ୍ରତିଜ୍ଞା(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } ଅନ୍ୟଥା ଯଦି (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                ଅଚଳ xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                ଅଚଳ xkb_mod_mask_t ignore = (clobber)
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
    into->ଅଙ୍କ_mods = from->ଅଙ୍କ_mods;
}

bool
HandleVModDef(ସଂରଚନା xkb_context *ctx, ସଂରଚନା xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    ଯଦି (stmt->ମୂଲ୍ୟ &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !ପଦସମଷ୍ଟି_Resolve_Mod_Mask(ctx, stmt->ମୂଲ୍ୟ, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        ଫେରନ୍ତୁ false;
    }

    xkb_mod_index_t vmod;
    ସଂରଚନା xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        ଯଦି (mod->name == stmt->name) {
            ଯଦି (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                ଫେରନ୍ତୁ false;
            }

            ଅଚଳ xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            ଯଦି (!stmt->ମୂଲ୍ୟ) {
                /* No new explicit mapping: do nothing */
                ଫେରନ୍ତୁ true;
            } ଅନ୍ୟଥା ଯଦି (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } ଅନ୍ୟଥା ଯଦି (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                ଅଚଳ bool clobber = (stmt->merge != MERGE_AUGMENT);
                ଅଚଳ xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                ଅଚଳ xkb_mod_mask_t ignore =
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

            ଫେରନ୍ତୁ true;
        }
    }

    ଯଦି (mods->ଅଙ୍କ_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        ଫେରନ୍ତୁ false;
    }

    mods->mods[mods->ଅଙ୍କ_mods].name = stmt->name;
    mods->mods[mods->ଅଙ୍କ_mods].type = MOD_VIRT;
    mods->mods[mods->ଅଙ୍କ_mods].mapping = mapping;
    ଯଦି (stmt->ମୂଲ୍ୟ) {
        ଅଚଳ xkb_mod_mask_t mask = UINT32_C(1) << mods->ଅଙ୍କ_mods;
        mods->explicit_vmods |= mask;
    }
    mods->ଅଙ୍କ_mods++;
    ଫେରନ୍ତୁ true;
}
