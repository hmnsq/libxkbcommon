/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <stdint.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "expr.h"
#അന്തര്ഭാവ "xkbcomp/ast.h"
#അന്തര്ഭാവ "vmod.h"

ശൂന്യം
InitVMods(ഘടന xkb_mod_set *info, സ്ഥിരം ഘടന xkb_mod_set *mods, bool reset)
{
    *info = *mods;

    എങ്കിൽ (!reset)
        തിരികെ;

    ഘടന xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

ശൂന്യം
MergeModSets(ഘടന xkb_context *ctx, ഘടന xkb_mod_set *into,
             സ്ഥിരം ഘടന xkb_mod_set *from, എണ്ണൽ merge_mode merge) {
    സ്ഥിരം bool clobber = (merge != MERGE_AUGMENT);
    xkb_mod_index_t vmod;
    സ്ഥിരം ഘടന xkb_mod *mod;
    പ്രതിജ്ഞാ(into->അക്കം_mods <= from->അക്കം_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        സ്ഥിരം xkb_mod_mask_t mask = UINT32_C(1) << vmod;

        എങ്കിൽ ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            പ്രതിജ്ഞാ((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            തുടരുക;
        } അല്ലെങ്കിൽ എങ്കിൽ (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            പ്രതിജ്ഞാ(into->mods[vmod].name == XKB_ATOM_NONE);
            പ്രതിജ്ഞാ(mod->type == MOD_VIRT);
            പ്രതിജ്ഞാ(mod->name != XKB_ATOM_NONE);
            പ്രതിജ്ഞാ(vmod >= into->അക്കം_mods);
            into->mods[vmod] = *mod;
            എങ്കിൽ (from->explicit_vmods & mask)
                into->explicit_vmods |= mask;
        } അല്ലെങ്കിൽ {
            /* Modifier exists in both */
            പ്രതിജ്ഞാ(mod->type == MOD_VIRT);
            പ്രതിജ്ഞാ(mod->name != XKB_ATOM_NONE);
            പ്രതിജ്ഞാ(into->mods[vmod].type == mod->type);
            പ്രതിജ്ഞാ(into->mods[vmod].name == mod->name);

            എങ്കിൽ (!(from->explicit_vmods & mask)) {
                /* Implicit mapping in `from`: do nothing */
                പ്രതിജ്ഞാ(mod->mapping == 0);
            } അല്ലെങ്കിൽ എങ്കിൽ (!(into->explicit_vmods & mask)) {
                /* Implicit mapping in `into`: replace */
                പ്രതിജ്ഞാ(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= mask;
            } അല്ലെങ്കിൽ എങ്കിൽ (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                സ്ഥിരം xkb_mod_mask_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                സ്ഥിരം xkb_mod_mask_t ignore = (clobber)
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
    into->അക്കം_mods = from->അക്കം_mods;
}

bool
HandleVModDef(ഘടന xkb_context *ctx, ഘടന xkb_mod_set *mods, VModDef *stmt)
{
    xkb_mod_mask_t mapping = 0;
    എങ്കിൽ (stmt->മൂല്യം &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !വ്യഞ്ജകം_Resolve_Mod_Mask(ctx, stmt->മൂല്യം, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_text(ctx, stmt->name));
        തിരികെ false;
    }

    xkb_mod_index_t vmod;
    ഘടന xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        എങ്കിൽ (mod->name == stmt->name) {
            എങ്കിൽ (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_text(ctx, mod->name));
                തിരികെ false;
            }

            സ്ഥിരം xkb_mod_mask_t mask = UINT32_C(1) << vmod;

            എങ്കിൽ (!stmt->മൂല്യം) {
                /* No new explicit mapping: do nothing */
                തിരികെ true;
            } അല്ലെങ്കിൽ എങ്കിൽ (!(mods->explicit_vmods & mask)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } അല്ലെങ്കിൽ എങ്കിൽ (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                സ്ഥിരം bool clobber = (stmt->merge != MERGE_AUGMENT);
                സ്ഥിരം xkb_mod_mask_t use =
                    (clobber ? mapping : mod->mapping);
                സ്ഥിരം xkb_mod_mask_t ignore =
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

            തിരികെ true;
        }
    }

    എങ്കിൽ (mods->അക്കം_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_MESSAGE_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_text(ctx, stmt->name), XKB_MAX_MODS);
        തിരികെ false;
    }

    mods->mods[mods->അക്കം_mods].name = stmt->name;
    mods->mods[mods->അക്കം_mods].type = MOD_VIRT;
    mods->mods[mods->അക്കം_mods].mapping = mapping;
    എങ്കിൽ (stmt->മൂല്യം) {
        സ്ഥിരം xkb_mod_mask_t mask = UINT32_C(1) << mods->അക്കം_mods;
        mods->explicit_vmods |= mask;
    }
    mods->അക്കം_mods++;
    തിരികെ true;
}
