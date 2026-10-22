/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#समावेश "config.h"

#समावेश <stdint.h>

#समावेश "xkbcommon/xkbcommon.h"
#समावेश "atom.h"
#समावेश "keymap.h"
#समावेश "text.h"
#समावेश "expr.h"
#समावेश "xkbcomp/ast.h"
#समावेश "vmod.h"

व्योम
InitVMods(काष्ठा xkb_mod_set *info, स्थिर काष्ठा xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    अगर (!reset)
        वापस;

    काष्ठा xkb_mod *mod;
    xkb_mod_सूचकांक_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

व्योम
MergeModSets(काष्ठा xkb_context *ctx, काष्ठा xkb_mod_set *into,
             स्थिर काष्ठा xkb_mod_set *from, क्रमागत merge_mode merge) {
    स्थिर bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_सूचकांक_t vmod;
    स्थिर काष्ठा xkb_mod *mod;
    निश्चित(into->अंक_mods <= from->अंक_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        स्थिर xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        अगर ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            निश्चित((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            जारी;
        } अन्यथा अगर (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            निश्चित(into->mods[vmod].name == XKB_ATOM_NONE);
            निश्चित(mod->type == MOD_VIRT);
            निश्चित(mod->name != XKB_ATOM_NONE);
            निश्चित(vmod >= into->अंक_mods);
            into->mods[vmod] = *mod;
            अगर (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } अन्यथा {
            /* Modifier exists in both */
            निश्चित(mod->type == MOD_VIRT);
            निश्चित(mod->name != XKB_ATOM_NONE);
            निश्चित(into->mods[vmod].type == mod->type);
            निश्चित(into->mods[vmod].name == mod->name);

            अगर (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                निश्चित(mod->mapping == 0);
            } अन्यथा अगर (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                निश्चित(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } अन्यथा अगर (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                स्थिर xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                स्थिर xkb_mod_mask_t ignore = (clobber)
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
    into->अंक_mods = from->अंक_mods;
}

bool
HandleVModDef(काष्ठा xkb_context *ctx, काष्ठा xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    अगर (stmt->मूल्य &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !व्यंजक_Resolve_Mod_Mask(ctx, stmt->मूल्य, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        वापस false;
    }

    xkb_mod_सूचकांक_t vmod;
    काष्ठा xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        अगर (mod->name == stmt->name) {
            अगर (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                वापस false;
            }

            स्थिर xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            अगर (!stmt->मूल्य) {
                /* No new explicit mapping: do nothing */
                वापस true;
            } अन्यथा अगर (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } अन्यथा अगर (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                स्थिर bool clobber = (stmt->merge != MERGE_AUGMENT);
                स्थिर xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                स्थिर xkb_mod_mask_t ignore =
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

            वापस true;
        }
    }

    अगर (mods->अंक_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        वापस false;
    }

    mods->mods[mods->अंक_mods].name = stmt->name;
    mods->mods[mods->अंक_mods].type = MOD_VIRT;
    mods->mods[mods->अंक_mods].mapping = mapping;
    अगर (stmt->मूल्य) {
        स्थिर xkb_mod_mask_t mask = UINT32_C(1) << mods->अंक_mods;
        mods->explicit_vmods |= mask;
    }
    mods->अंक_mods++;
    वापस true;
}
