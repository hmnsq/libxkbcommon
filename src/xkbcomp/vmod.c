/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <stdint.h>

#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "atom.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "expr.h"
#అన్తర్భావ "xkbcomp/ast.h"
#అన్తర్భావ "vmod.h"

శూన్య
InitVMods(నిర్మాణం xkb_mod_సమితి *info, స్థిరరాశి నిర్మాణం xkb_mod_సమితి *mods, bool reset)
{
    *info = *mods;

    ఒకవేళ (!reset)
        తిరిగి;

    నిర్మాణం xkb_mod *mod;
    xkb_mod_సూచీ_t vmod = 0;
    xkb_mods_enumerate(vmod, mod, info) {
        mod->mapping = 0;
    }
    info->explicit_vmods = 0;
}

శూన్య
విలీనం_Mod_Sets(నిర్మాణం xkb_context *ctx, నిర్మాణం xkb_mod_సమితి *into,
             స్థిరరాశి నిర్మాణం xkb_mod_సమితి *from, గణన విలీనం_బహుళకం విలీనం) {
    స్థిరరాశి bool clobber = (విలీనం != విలీనం_AUGMENT);
    xkb_mod_సూచీ_t vmod;
    స్థిరరాశి నిర్మాణం xkb_mod *mod;
    ప్రతిజ్ఞా(into->అంకె_mods <= from->అంకె_mods);
    xkb_mods_enumerate(vmod, mod, from) {
        స్థిరరాశి xkb_mod_ముసుగు_t ముసుగు = UINT32_C(1) << vmod;

        ఒకవేళ ((mod->type != MOD_VIRT)) {
            /* No modifier in `from` or real modifier: nothing to do */
            ప్రతిజ్ఞా((mod->type == 0 &&
                    mod->name == XKB_ATOM_NONE) ||
                   ((mod->type & MOD_REAL) &&
                    into->mods[vmod].type == mod->type &&
                    mod->name != XKB_ATOM_NONE &&
                    into->mods[vmod].name == mod->name));
            కొనసాగు;
        } లేకపోతే ఒకవేళ (into->mods[vmod].type == 0) {
            /* No modifier in `into`: copy the whole modifier definition */
            ప్రతిజ్ఞా(into->mods[vmod].name == XKB_ATOM_NONE);
            ప్రతిజ్ఞా(mod->type == MOD_VIRT);
            ప్రతిజ్ఞా(mod->name != XKB_ATOM_NONE);
            ప్రతిజ్ఞా(vmod >= into->అంకె_mods);
            into->mods[vmod] = *mod;
            ఒకవేళ (from->explicit_vmods & ముసుగు)
                into->explicit_vmods |= ముసుగు;
        } లేకపోతే {
            /* Modifier exists in both */
            ప్రతిజ్ఞా(mod->type == MOD_VIRT);
            ప్రతిజ్ఞా(mod->name != XKB_ATOM_NONE);
            ప్రతిజ్ఞా(into->mods[vmod].type == mod->type);
            ప్రతిజ్ఞా(into->mods[vmod].name == mod->name);

            ఒకవేళ (!(from->explicit_vmods & ముసుగు)) {
                /* Implicit mapping in `from`: do nothing */
                ప్రతిజ్ఞా(mod->mapping == 0);
            } లేకపోతే ఒకవేళ (!(into->explicit_vmods & ముసుగు)) {
                /* Implicit mapping in `into`: replace */
                ప్రతిజ్ఞా(into->mods[vmod].mapping == 0);
                into->mods[vmod].mapping = mod->mapping;
                into->explicit_vmods |= ముసుగు;
            } లేకపోతే ఒకవేళ (mod->mapping != into->mods[vmod].mapping) {
                /* Handle conflicting mappings */
                స్థిరరాశి xkb_mod_ముసుగు_t use = (clobber)
                    ? mod->mapping
                    : into->mods[vmod].mapping;
                స్థిరరాశి xkb_mod_ముసుగు_t ignore = (clobber)
                    ? into->mods[vmod].mapping
                    : mod->mapping;

                log_warn(ctx, XKB_LOG_సందేశం_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_పాఠ్యం(ctx, mod->name),
                         Mod_ముసుగు_పాఠ్యం(ctx, MOD_REAL, from, use),
                         Mod_ముసుగు_పాఠ్యం(ctx, MOD_REAL, from, ignore));

                into->mods[vmod].mapping = use;
            }
        }
    }
    into->అంకె_mods = from->అంకె_mods;
}

bool
HandleVModDef(నిర్మాణం xkb_context *ctx, నిర్మాణం xkb_mod_సమితి *mods, VModDef *stmt)
{
    xkb_mod_ముసుగు_t mapping = 0;
    ఒకవేళ (stmt->విలువ &&
        /*
         * This is a statement such as 'virtualModifiers NumLock = Mod1';
         * it initialize the vmod-to-real-mod[s] mapping before going
         * through modifier_map.
         */
        !సమాసం_Resolve_Mod_ముసుగు(ctx, stmt->విలువ, MOD_REAL, mods, &mapping))
    {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Declaration of %s ignored\n",
                xkb_atom_పాఠ్యం(ctx, stmt->name));
        తిరిగి false;
    }

    xkb_mod_సూచీ_t vmod;
    నిర్మాణం xkb_mod *mod;
    xkb_mods_enumerate(vmod, mod, mods) {
        ఒకవేళ (mod->name == stmt->name) {
            ఒకవేళ (mod->type != MOD_VIRT) {
                log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                        "Can't add a virtual modifier named \"%s\"; there is "
                        "already a non-virtual modifier with this name! "
                        "Ignored\n",
                        xkb_atom_పాఠ్యం(ctx, mod->name));
                తిరిగి false;
            }

            స్థిరరాశి xkb_mod_ముసుగు_t ముసుగు = UINT32_C(1) << vmod;

            ఒకవేళ (!stmt->విలువ) {
                /* No new explicit mapping: do nothing */
                తిరిగి true;
            } లేకపోతే ఒకవేళ (!(mods->explicit_vmods & ముసుగు)) {
                /* No previous explicit mapping: add mapping */
                mod->mapping = mapping;
            } లేకపోతే ఒకవేళ (mod->mapping != mapping) {
                /* Handle conflicting mappings */
                స్థిరరాశి bool clobber = (stmt->విలీనం != విలీనం_AUGMENT);
                స్థిరరాశి xkb_mod_ముసుగు_t use =
                    (clobber ? mapping : mod->mapping);
                స్థిరరాశి xkb_mod_ముసుగు_t ignore =
                    (clobber ? mod->mapping : mapping);

                log_warn(ctx, XKB_LOG_సందేశం_NO_ID,
                         "Virtual modifier %s mapping defined multiple times; "
                         "Using %s, ignoring %s\n",
                         xkb_atom_పాఠ్యం(ctx, stmt->name),
                         Mod_ముసుగు_పాఠ్యం(ctx, MOD_REAL, mods, use),
                         Mod_ముసుగు_పాఠ్యం(ctx, MOD_REAL, mods, ignore));

                mod->mapping = use;
            }

            mods->explicit_vmods |= ముసుగు;

            తిరిగి true;
        }
    }

    ఒకవేళ (mods->అంకె_mods >= XKB_MAX_MODS) {
        log_err(ctx, XKB_LOG_సందేశం_NO_ID,
                "Cannot define virtual modifier %s: "
                "too many modifiers defined (maximum %"PRIu32")\n",
                xkb_atom_పాఠ్యం(ctx, stmt->name), XKB_MAX_MODS);
        తిరిగి false;
    }

    mods->mods[mods->అంకె_mods].name = stmt->name;
    mods->mods[mods->అంకె_mods].type = MOD_VIRT;
    mods->mods[mods->అంకె_mods].mapping = mapping;
    ఒకవేళ (stmt->విలువ) {
        స్థిరరాశి xkb_mod_ముసుగు_t ముసుగు = UINT32_C(1) << mods->అంకె_mods;
        mods->explicit_vmods |= ముసుగు;
    }
    mods->అంకె_mods++;
    తిరిగి true;
}
