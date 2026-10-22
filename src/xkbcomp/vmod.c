/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <stdint.h>

#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "text.h"
#அந்தர்பாவ "expr.h"
#அந்தர்பாவ "xkbcomp/ast.h"
#அந்தர்பாவ "vmod.h"

வெறுமை
InitVMods(கட்டமைப்பு xkb_mod_set *info, மாறிலி கட்டமைப்பு xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    எனில் (!reset)
        திரும்பு;

    கட்டமைப்பு xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

வெறுமை
MergeModSets(கட்டமைப்பு xkb_context *ctx, கட்டமைப்பு xkb_mod_set *into,
             மாறிலி கட்டமைப்பு xkb_mod_set *from, எண்ணிக்கை merge_mode merge) {
    மாறிலி bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    மாறிலி கட்டமைப்பு xkb_mod *mod;
    ப்ரதிஜ்ஞா(into->இலக்கம்_mods <= from->இலக்கம்_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        மாறிலி xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        எனில் ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            ப்ரதிஜ்ஞா((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            தொடர்;
        } இல்லையெனில் எனில் (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            ப்ரதிஜ்ஞா(into->mods[vmod].name == XKB_ATOM_NONE);
            ப்ரதிஜ்ஞா(mod->type == MOD_VIRT);
            ப்ரதிஜ்ஞா(mod->name != XKB_ATOM_NONE);
            ப்ரதிஜ்ஞா(vmod >= into->இலக்கம்_mods);
            into->mods[vmod] = *mod;
            எனில் (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } இல்லையெனில் {
            /* Modifier exists in both */
            ப்ரதிஜ்ஞா(mod->type == MOD_VIRT);
            ப்ரதிஜ்ஞா(mod->name != XKB_ATOM_NONE);
            ப்ரதிஜ்ஞா(into->mods[vmod].type == mod->type);
            ப்ரதிஜ்ஞா(into->mods[vmod].name == mod->name);

            எனில் (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                ப்ரதிஜ்ஞா(mod->mapping == 0);
            } இல்லையெனில் எனில் (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                ப்ரதிஜ்ஞா(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } இல்லையெனில் எனில் (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                மாறிலி xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                மாறிலி xkb_mod_mask_t ignore = (clobber)
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
    into->இலக்கம்_mods = from->இலக்கம்_mods;
}

bool
HandleVModDef(கட்டமைப்பு xkb_context *ctx, கட்டமைப்பு xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    எனில் (stmt->மதிப்பு &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !கோவை_Resolve_Mod_Mask(ctx, stmt->மதிப்பு, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        திரும்பு false;
    }

    xkb_mod_index_t vmod;
    கட்டமைப்பு xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        எனில் (mod->name == stmt->name) {
            எனில் (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                திரும்பு false;
            }

            மாறிலி xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            எனில் (!stmt->மதிப்பு) {
                /* No new explicit mapping: do nothing */
                திரும்பு true;
            } இல்லையெனில் எனில் (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } இல்லையெனில் எனில் (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                மாறிலி bool clobber = (stmt->merge != MERGE_AUGMENT);
                மாறிலி xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                மாறிலி xkb_mod_mask_t ignore =
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

            திரும்பு true;
        }
    }

    எனில் (mods->இலக்கம்_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        திரும்பு false;
    }

    mods->mods[mods->இலக்கம்_mods].name = stmt->name;
    mods->mods[mods->இலக்கம்_mods].type = MOD_VIRT;
    mods->mods[mods->இலக்கம்_mods].mapping = mapping;
    எனில் (stmt->மதிப்பு) {
        மாறிலி xkb_mod_mask_t mask = UINT32_C(1) << mods->இலக்கம்_mods;
        mods->explicit_vmods |= mask;
    }
    mods->இலக்கம்_mods++;
    திரும்பு true;
}
