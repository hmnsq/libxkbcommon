/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#అన్తర్భావ "config.h"

#అన్తర్భావ <assert.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

#అన్తర్భావ "context.h"
#అన్తర్భావ "messages-codes.h"
#అన్తర్భావ "xkbcommon/xkbcommon.h"
#అన్తర్భావ "xkbcomp-priv.h"
#అన్తర్భావ "atom.h"
#అన్తర్భావ "darray.h"
#అన్తర్భావ "keymap.h"
#అన్తర్భావ "text.h"
#అన్తర్భావ "expr.h"
#అన్తర్భావ "include.h"
#అన్తర్భావ "util-mem.h"

రకనిర్వచనం నిర్మాణం {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} ఎగువ_Keycode_నమోదు;

రకనిర్వచనం నిర్మాణం {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) దిగువ;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(ఎగువ_Keycode_నమోదు) ఎగువ;
    /**
     * name -> keycode mapping
     */
    darray(Keycode_సరిపోలిక) పేర్లు;
} KeycodeStore;

స్థిర అంతఃవరుస శూన్య
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->దిగువ);
    darray_init(store->ఎగువ);
    darray_init(store->పేర్లు);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

స్థిర అంతఃవరుస శూన్య
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->దిగువ);
    darray_free(store->ఎగువ);
    darray_free(store->పేర్లు);
}

స్థిర అంతఃవరుస శూన్య
keycode_store_update_కీలకం(KeycodeStore *store, Keycode_సరిపోలిక సరిపోలిక, xkb_atom_t name)
{
    ఒకవేళ (unlikely(!సరిపోలిక.దొరికింది || సరిపోలిక.is_alias)) {
        తిరిగి;
    } లేకపోతే ఒకవేళ (సరిపోలిక.కీలకం.దిగువ) {
        /* assume valid index */
        ప్రతిజ్ఞా(సరిపోలిక.కీలకం.సూచీ < darray_కొలత(store->దిగువ));
        darray_అంశం(store->దిగువ, సరిపోలిక.కీలకం.సూచీ) = name;
    } లేకపోతే {
        /* assume valid index */
        ప్రతిజ్ఞా(సరిపోలిక.కీలకం.సూచీ < darray_కొలత(store->ఎగువ));
        darray_అంశం(store->ఎగువ, సరిపోలిక.కీలకం.సూచీ).name = name;
    }

    /* Names lookup */
    ఒకవేళ (name >= darray_కొలత(store->పేర్లు)) {
        darray_resize0(store->పేర్లు, name + 1);
    }
    darray_అంశం(store->పేర్లు, name) = సరిపోలిక;
}

స్థిర bool
keycode_store_చొప్పించు_కీలకం(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    ఒకవేళ (name >= darray_కొలత(store->పేర్లు)) {
        darray_resize0(store->పేర్లు, name + 1);
    }
    ఒకవేళ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        ఒకవేళ (kc >= (xkb_keycode_t) darray_కొలత(store->దిగువ))
            darray_resize0(store->దిగువ, kc + 1);
        darray_అంశం(store->దిగువ, kc) = name;
        ఒకవేళ (kc < store->min)
            store->min = kc;
        darray_అంశం(store->పేర్లు, name) = (Keycode_సరిపోలిక) {
            .కీలకం = {
                .దొరికింది = true,
                .దిగువ = true,
                .is_alias = false,
                .సూచీ = kc
            }
        };
    } లేకపోతే {
        /* High keycode: insert into a sorted list */
        స్థిరరాశి darray_కొలత_t idx = darray_కొలత(store->ఎగువ);
        ఒకవేళ (idx && darray_అంశం(store->ఎగువ, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_కొలత_t క్రింది = 0;
            darray_కొలత_t పై = idx;
            అయితే (క్రింది < పై) {
                స్థిరరాశి darray_కొలత_t mid = క్రింది + (పై - 1 - క్రింది) / 2;
                స్థిరరాశి ఎగువ_Keycode_నమోదు * స్థిరరాశి నమోదు_2 =
                    &darray_అంశం(store->ఎగువ, mid);
                ఒకవేళ (నమోదు_2->keycode < kc) {
                    క్రింది = mid + 1;
                } లేకపోతే ఒకవేళ (నమోదు_2->keycode > kc) {
                    పై = mid;
                } లేకపోతే {
                    /* Unreachable: there is no repetition in the list */
                    ప్రతిజ్ఞా(నమోదు_2->keycode != kc);
                }
            }
            ప్రతిజ్ఞా(క్రింది < idx);
            ప్రతిజ్ఞా(darray_అంశం(store->ఎగువ, క్రింది).keycode > kc);

            /* Update references to entries that will be moved */
            ఎగువ_Keycode_నమోదు *నమోదు_2;
            darray_foreach_from(నమోదు_2, store->ఎగువ, క్రింది) {
                darray_అంశం(store->పేర్లు, నమోదు_2->name).కీలకం.సూచీ++;
            }

            darray_చొప్పించు(store->ఎగువ, క్రింది,
                          (ఎగువ_Keycode_నమోదు){.keycode = kc, .name = name});
            darray_అంశం(store->పేర్లు, name) = (Keycode_సరిపోలిక) {
                .కీలకం = {
                    .దొరికింది = true,
                    .దిగువ = false,
                    .is_alias = false,
                    .సూచీ = క్రింది
                }
            };
        } లేకపోతే {
            /* Fast path: no need to sort */
            darray_append(store->ఎగువ,
                          (ఎగువ_Keycode_నమోదు){.keycode = kc, .name = name});
            darray_అంశం(store->పేర్లు, name) = (Keycode_సరిపోలిక) {
                .కీలకం = {
                    .దొరికింది = true,
                    .దిగువ = false,
                    .is_alias = false,
                    .సూచీ = idx
                }
            };
        }
        ఒకవేళ (darray_ఖాళీలు(store->దిగువ))
            store->min = darray_అంశం(store->ఎగువ, 0).keycode;
    }
    తిరిగి true;
}

స్థిర అంతఃవరుస bool
keycode_store_చొప్పించు_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    ఒకవేళ (alias >= darray_కొలత(store->పేర్లు)) {
        darray_resize0(store->పేర్లు, alias + 1);
    }
    darray_అంశం(store->పేర్లు, alias) = (Keycode_సరిపోలిక) {
        .alias = {
            .దొరికింది = true,
            .is_alias = true,
            .real = real
        }
    };
    తిరిగి true;
}

స్థిర అంతఃవరుస bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_అంశం(store->పేర్లు, alias).alias.real = real;
    తిరిగి true;
}

స్థిర అంతఃవరుస శూన్య
keycode_store_delete_name(స్థిరరాశి KeycodeStore *store, xkb_atom_t name)
{
    ఒకవేళ (name < darray_కొలత(store->పేర్లు))
        darray_అంశం(store->పేర్లు, name).దొరికింది = false;
}

స్థిర శూన్య
keycode_store_delete_కీలకం(KeycodeStore *store, స్థిరరాశి Keycode_సరిపోలిక సరిపోలిక)
{
    ఒకవేళ (unlikely(!సరిపోలిక.దొరికింది || సరిపోలిక.is_alias)) {
        తిరిగి;
    } లేకపోతే ఒకవేళ (సరిపోలిక.కీలకం.దిగువ) {
        /* assume valid index */
        ప్రతిజ్ఞా(సరిపోలిక.కీలకం.సూచీ < darray_కొలత(store->దిగువ));
        darray_అంశం(store->పేర్లు,
                    darray_అంశం(store->దిగువ, సరిపోలిక.కీలకం.సూచీ)).దొరికింది = false;
        ఒకవేళ (సరిపోలిక.కీలకం.సూచీ + 1u == darray_కొలత(store->దిగువ)) {
            /* Highest low keycode: shrink */
            ఒకవేళ (store->min == సరిపోలిక.కీలకం.సూచీ) {
                /* No low keycode left */
                darray_కొలత(store->దిగువ) = 0;
            } లేకపోతే {
                /* Look for previous defined low keycode */
                కొరకు (darray_కొలత_t idx = సరిపోలిక.కీలకం.సూచీ; idx > 0; idx--) {
                    ఒకవేళ (darray_అంశం(store->దిగువ, idx - 1) != XKB_ATOM_NONE) {
                        darray_కొలత(store->దిగువ) = idx;
                        ఆపు;
                    }
                }
            }
        } లేకపోతే {
            /* Lower keycode: reset */
            darray_అంశం(store->దిగువ, సరిపోలిక.కీలకం.సూచీ) = XKB_ATOM_NONE;
        }
    } లేకపోతే {
        ప్రతిజ్ఞా(సరిపోలిక.కీలకం.సూచీ < darray_కొలత(store->ఎగువ)); /* assume valid index */
        darray_అంశం(store->పేర్లు,
                    darray_అంశం(store->ఎగువ, సరిపోలిక.కీలకం.సూచీ).name).దొరికింది = false;
        darray_remove(store->ఎగువ, సరిపోలిక.కీలకం.సూచీ);
        /* Update LUT indexes of high codes after the deleted one, if any */
        Keycode_సరిపోలిక *నమోదు_2;
        darray_foreach(నమోదు_2, store->పేర్లు) {
            ఒకవేళ (నమోదు_2->దొరికింది && !నమోదు_2->is_alias && !నమోదు_2->కీలకం.దిగువ &&
                నమోదు_2->కీలకం.సూచీ > సరిపోలిక.కీలకం.సూచీ) {
                నమోదు_2->కీలకం.సూచీ--;
            }
        }
    }

    /* Update bounds */
    ఒకవేళ (darray_ఖాళీలు(store->దిగువ)) {
        store->min = (darray_ఖాళీలు(store->ఎగువ))
            ? XKB_KEYCODE_INVALID
            : darray_అంశం(store->ఎగువ, 0).keycode;
    } లేకపోతే {
        కొరకు (xkb_keycode_t kc = store->min; kc < darray_కొలత(store->దిగువ); kc++) {
            ఒకవేళ (darray_అంశం(store->దిగువ, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                ఆపు;
            }
        }
    }
}

స్థిర అంతఃవరుస xkb_keycode_t
keycode_store_get_keycode(స్థిరరాశి KeycodeStore *store, Keycode_సరిపోలిక సరిపోలిక)
{
    ఒకవేళ (!సరిపోలిక.దొరికింది || సరిపోలిక.is_alias) {
        తిరిగి XKB_KEYCODE_INVALID;
    } లేకపోతే ఒకవేళ (సరిపోలిక.కీలకం.దిగువ) {
        /* assume valid index */
        ప్రతిజ్ఞా(సరిపోలిక.కీలకం.సూచీ < darray_కొలత(store->దిగువ));
        తిరిగి (xkb_keycode_t) సరిపోలిక.కీలకం.సూచీ;
    } లేకపోతే {
        /* assume valid index */
        ప్రతిజ్ఞా(సరిపోలిక.కీలకం.సూచీ < darray_కొలత(store->ఎగువ));
        తిరిగి darray_అంశం(store->ఎగువ, సరిపోలిక.కీలకం.సూచీ).keycode;
    }
}

స్థిర అంతఃవరుస xkb_atom_t
keycode_store_get_కీలకం_name(స్థిరరాశి KeycodeStore *store, Keycode_సరిపోలిక సరిపోలిక)
{
    ఒకవేళ (!సరిపోలిక.దొరికింది || సరిపోలిక.is_alias) {
        తిరిగి XKB_ATOM_NONE;
    } లేకపోతే ఒకవేళ (సరిపోలిక.కీలకం.దిగువ) {
        /* assume valid index */
        ప్రతిజ్ఞా(సరిపోలిక.కీలకం.సూచీ < darray_కొలత(store->దిగువ));
        తిరిగి darray_అంశం(store->దిగువ, సరిపోలిక.కీలకం.సూచీ);
    } లేకపోతే {
        /* assume valid index */
        ప్రతిజ్ఞా(సరిపోలిక.కీలకం.సూచీ < darray_కొలత(store->ఎగువ));
        తిరిగి darray_అంశం(store->ఎగువ, సరిపోలిక.కీలకం.సూచీ).name;
    }
}

స్థిర Keycode_సరిపోలిక
keycode_store_lookup_keycode(స్థిరరాశి KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    ఒకవేళ (kc < (xkb_keycode_t) darray_కొలత(store->దిగువ)) {
        తిరిగి (Keycode_సరిపోలిక) {
            .కీలకం = {
                .దొరికింది = true,
                .దిగువ = true,
                .is_alias = false,
                .సూచీ = kc
            }
        };
    } లేకపోతే ఒకవేళ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        తిరిగి (Keycode_సరిపోలిక) { .దొరికింది = false };
    }

    /* High keycodes: use binary search */
    darray_కొలత_t క్రింది = 0;
    darray_కొలత_t పై = darray_కొలత(store->ఎగువ);
    అయితే (క్రింది < పై) {
        స్థిరరాశి darray_కొలత_t mid = క్రింది + (పై - 1 - క్రింది) / 2;
        ఎగువ_Keycode_నమోదు * స్థిరరాశి నమోదు_2 = &darray_అంశం(store->ఎగువ, mid);
        ఒకవేళ (నమోదు_2->keycode < kc) {
            క్రింది = mid + 1;
        } లేకపోతే ఒకవేళ (నమోదు_2->keycode > kc) {
            పై = mid;
        } లేకపోతే {
            తిరిగి (Keycode_సరిపోలిక) {
                .కీలకం = {
                    .దొరికింది = true,
                    .దిగువ = false,
                    .is_alias = false,
                    .సూచీ = mid
                }
            };
        }
    }

    తిరిగి (Keycode_సరిపోలిక) { .దొరికింది = false };
}

స్థిర Keycode_సరిపోలిక
keycode_store_lookup_name(స్థిరరాశి KeycodeStore *store, xkb_atom_t name)
{
    ఒకవేళ (name >= darray_కొలత(store->పేర్లు)) {
        తిరిగి (Keycode_సరిపోలిక) { .దొరికింది = false, .is_alias = false };
    } లేకపోతే {
        తిరిగి darray_అంశం(store->పేర్లు, name);
    }
}

/***====================================================================***/

రకనిర్వచనం నిర్మాణం {
    గణన విలీనం_బహుళకం విలీనం;

    xkb_atom_t name;
} LedNameInfo;

రకనిర్వచనం నిర్మాణం {
    అక్షరం *name;
    పూర్ణాంకం error_లెక్క;
    గుర్తులేని పూర్ణాంకం include_లోతు;

    KeycodeStore keycodes;
    LedNameInfo led_పేర్లు[XKB_MAX_LEDS];
    xkb_led_సూచీ_t అంకె_led_పేర్లు;

    నిర్మాణం xkb_context *ctx;
    స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info;
} కీలకం_పేర్లు_Info;

/***====================================================================***/

స్థిర LedNameInfo *
కనుగొను_Led_By_Name(కీలకం_పేర్లు_Info *info, xkb_atom_t name,
              xkb_led_సూచీ_t *idx_బహిర్గతం)
{
    కొరకు (xkb_led_సూచీ_t idx = 0; idx < info->అంకె_led_పేర్లు; idx++) {
        LedNameInfo *ledi = &info->led_పేర్లు[idx];
        ఒకవేళ (ledi->name == name) {
            *idx_బహిర్గతం = idx;
            తిరిగి ledi;
        }
    }

    తిరిగి ఖాళీ;
}

స్థిర bool
కూడు_Led_Name(కీలకం_పేర్లు_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_సూచీ_t new_idx, bool report)
{
    xkb_led_సూచీ_t old_idx;
    LedNameInfo *old;
    స్థిరరాశి bool replace = (new->విలీనం != విలీనం_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = కనుగొను_Led_By_Name(info, new->name, &old_idx);
    ఒకవేళ (old) {
        ఒకవేళ (old_idx == new_idx) {
            ఒకవేళ (report)
                log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_పాఠ్యం(info->ctx, new->name));
            తిరిగి true;
        }

        ఒకవేళ (report) {
            xkb_led_సూచీ_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_సూచీ_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_పాఠ్యం(info->ctx, new->name), use, ignore);
        }

        ఒకవేళ (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } లేకపోతే {
            తిరిగి true;
        }
    }

    ఒకవేళ (new_idx >= info->అంకె_led_పేర్లు)
        info->అంకె_led_పేర్లు = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_పేర్లు[new_idx];
    ఒకవేళ (old->name != XKB_ATOM_NONE) {
        ఒకవేళ (report) {
            స్థిరరాశి xkb_atom_t use    = (replace ? new->name : old->name);
            స్థిరరాశి xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_పాఠ్యం(info->ctx, use),
                     xkb_atom_పాఠ్యం(info->ctx, ignore));
        }

        ఒకవేళ (replace)
            *old = *new;

        తిరిగి true;
    }

    *old = *new;
    తిరిగి true;
}

స్థిర శూన్య
Clear_కీలకం_పేర్లు_Info(కీలకం_పేర్లు_Info *info)
{
    విడుదల(info->name);
    keycode_store_free(&info->keycodes);
}

స్థిర శూన్య
Init_కీలకం_పేర్లు_Info(కీలకం_పేర్లు_Info *info, స్థిరరాశి నిర్మాణం xkb_keymap_info *keymap_info,
                 గుర్తులేని పూర్ణాంకం include_లోతు)
{
    memset(info, 0, పరిమాణం(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_లోతు = include_లోతు;
    keycode_store_init(&info->keycodes);
}

స్థిర bool
కూడు_కీలకం_Name(కీలకం_పేర్లు_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           గణన విలీనం_బహుళకం విలీనం, bool report)
{
#నిర్వచితమైతే NDEBUG
    స్థిరరాశి
#అయితే_ముగింపు
    Keycode_సరిపోలిక సరిపోలిక_name = keycode_store_lookup_name(&info->keycodes, name);
    ఒకవేళ (సరిపోలిక_name.దొరికింది) {
        స్థిరరాశి bool clobber = (విలీనం != విలీనం_AUGMENT);

        ఒకవేళ (సరిపోలిక_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            ఒకవేళ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         కీలకం_Name_పాఠ్యం(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            ఒకవేళ (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#నిర్వచితం_కాకపోతే NDEBUG
                సరిపోలిక_name.దొరికింది = false;
#అయితే_ముగింపు
            } లేకపోతే {
                తిరిగి true;
            }
        } లేకపోతే {
            స్థిరరాశి xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, సరిపోలిక_name);
            ప్రతిజ్ఞా(old_kc != XKB_KEYCODE_INVALID);
            ఒకవేళ (old_kc != kc) {
                /* There is already a different key with this name. */

                ఒకవేళ (report) {
                    స్థిరరాశి xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    స్థిరరాశి xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            కీలకం_Name_పాఠ్యం(info->ctx, name), use, ignore);
                }

                ఒకవేళ (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_కీలకం(&info->keycodes, సరిపోలిక_name);
                } లేకపోతే {
                    తిరిగి true;
                }
            }
        }
    }

    స్థిరరాశి Keycode_సరిపోలిక సరిపోలిక_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    స్థిరరాశి xkb_atom_t old_name = keycode_store_get_కీలకం_name(&info->keycodes, సరిపోలిక_kc);
    ఒకవేళ (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        ఒకవేళ (old_name == name) {
            ప్రతిజ్ఞా(keycode_store_get_keycode(&info->keycodes, సరిపోలిక_name) == kc);
            ఒకవేళ (report)
                log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         కీలకం_Name_పాఠ్యం(info->ctx, old_name), kc);
            తిరిగి true;
        }

        స్థిరరాశి bool clobber = (విలీనం != విలీనం_AUGMENT);
        ఒకవేళ (report) {
            స్థిరరాశి అక్షరం* స్థిరరాశి kname     = కీలకం_Name_పాఠ్యం(info->ctx, name);
            స్థిరరాశి అక్షరం* స్థిరరాశి old_kname = కీలకం_Name_పాఠ్యం(info->ctx, old_name);
            స్థిరరాశి అక్షరం* స్థిరరాశి use    = (clobber) ? kname : old_kname;
            స్థిరరాశి అక్షరం* స్థిరరాశి ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_సందేశం_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        ఒకవేళ (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_కీలకం(&info->keycodes, సరిపోలిక_kc, name);
        }
    } లేకపోతే {
        /* No previous keycode */
        ఒకవేళ (!keycode_store_చొప్పించు_కీలకం(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_కేటాయింపు_FAILURE_2,
                    "Cannot add keycode\n");
            తిరిగి false;
        }
    }

    తిరిగి true;
}

/***====================================================================***/

స్థిర bool
HandleAliasDef(కీలకం_పేర్లు_Info *info, స్థిరరాశి కీలకం_Alias_Def *def, bool report);

స్థిర శూన్య
విలీనం_Keycode_Stores(కీలకం_పేర్లు_Info *into, కీలకం_పేర్లు_Info *from,
                   గణన విలీనం_బహుళకం విలీనం, bool report)
{
    ఒకవేళ (darray_ఖాళీలు(into->keycodes.దిగువ) && darray_ఖాళీలు(into->keycodes.ఎగువ) &&
        darray_ఖాళీలు(into->keycodes.పేర్లు)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.దిగువ);
        darray_init(from->keycodes.ఎగువ);
        darray_init(from->keycodes.పేర్లు);
    } లేకపోతే {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        కొరకు (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_కొలత(from->keycodes.దిగువ);
             kc++) {
            /* Skip undefined keycodes */
            స్థిరరాశి xkb_atom_t name = darray_అంశం(from->keycodes.దిగువ, kc);
            ఒకవేళ (name == XKB_ATOM_NONE)
                కొనసాగు;

            ఒకవేళ (!కూడు_కీలకం_Name(into, kc, name, విలీనం, report))
                into->error_లెక్క++;
        }

        /* High keycodes */
        స్థిరరాశి ఎగువ_Keycode_నమోదు *new;
        darray_foreach(new, from->keycodes.ఎగువ) {
            ప్రతిజ్ఞా(new->name != XKB_ATOM_NONE);
            ఒకవేళ (!కూడు_కీలకం_Name(into, new->keycode, new->name, విలీనం, report))
                into->error_లెక్క++;
        }

        /* Aliases. */
        Keycode_సరిపోలిక *సరిపోలిక;
        xkb_atom_t alias;
        darray_enumerate(alias, సరిపోలిక, from->keycodes.పేర్లు) {
            ఒకవేళ (!సరిపోలిక->దొరికింది || !సరిపోలిక->is_alias)
                కొనసాగు;

            స్థిరరాశి కీలకం_Alias_Def def = {
                .విలీనం = విలీనం,
                .alias = alias,
                .real = సరిపోలిక->alias.real
            };
            ఒకవేళ (!HandleAliasDef(into, &def, report))
                into->error_లెక్క++;
        }
    }
}

స్థిర శూన్య
విలీనం_Included_Keycodes(కీలకం_పేర్లు_Info *into, కీలకం_పేర్లు_Info *from,
                      గణన విలీనం_బహుళకం విలీనం, bool report)
{
    ఒకవేళ (from->error_లెక్క > 0) {
        into->error_లెక్క += from->error_లెక్క;
        తిరిగి;
    }

    ఒకవేళ (into->name == ఖాళీ) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    విలీనం_Keycode_Stores(into, from, విలీనం, report);

    /* Merge LED names. */
    ఒకవేళ (into->అంకె_led_పేర్లు == 0) {
        జ్ఞాపకం_నకలు(into->led_పేర్లు, from->led_పేర్లు,
               పరిమాణం(*from->led_పేర్లు) * from->అంకె_led_పేర్లు);
        into->అంకె_led_పేర్లు = from->అంకె_led_పేర్లు;
        from->అంకె_led_పేర్లు = 0;
    }
    లేకపోతే {
        కొరకు (xkb_led_సూచీ_t idx = 0; idx < from->అంకె_led_పేర్లు; idx++) {
            LedNameInfo *ledi = &from->led_పేర్లు[idx];

            ఒకవేళ (ledi->name == XKB_ATOM_NONE)
                కొనసాగు;

            ledi->విలీనం = విలీనం;
            ఒకవేళ (!కూడు_Led_Name(into, false, ledi, idx, report))
                into->error_లెక్క++;
        }
    }
}

స్థిర శూన్య
HandleKeycodesFile(కీలకం_పేర్లు_Info *info, XkbFile *file);

స్థిర bool
HandleIncludeKeycodes(కీలకం_పేర్లు_Info *info, IncludeStmt *include, bool report)
{
    కీలకం_పేర్లు_Info included;

    ఒకవేళ (Exceeds_Include_Max_లోతు(info->ctx, info->include_లోతు)) {
        info->error_లెక్క += 10;
        తిరిగి false;
    }

    Init_కీలకం_పేర్లు_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    స్థిరరాశి నిర్మాణం parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    కొరకు (IncludeStmt *stmt = include; stmt; stmt = stmt->తదుపరి_incl) {
        కీలకం_పేర్లు_Info తదుపరి_incl;
        XkbFile *file;

        అక్షరం మార్గం[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  మార్గం, పరిమాణం(మార్గం));
        ఒకవేళ (!file) {
            info->error_లెక్క += 10;
            Clear_కీలకం_పేర్లు_Info(&included);
            తిరిగి false;
        }

        Init_కీలకం_పేర్లు_Info(&తదుపరి_incl, info->keymap_info, info->include_లోతు + 1);

        HandleKeycodesFile(&తదుపరి_incl, file);

        విలీనం_Included_Keycodes(&included, &తదుపరి_incl, stmt->విలీనం, report);

        Clear_కీలకం_పేర్లు_Info(&తదుపరి_incl);
        FreeXkbFile(file);
    }

    విలీనం_Included_Keycodes(info, &included, include->విలీనం, report);
    Clear_కీలకం_పేర్లు_Info(&included);

    తిరిగి (info->error_లెక్క == 0);
}

స్థిర bool
HandleKeycodeDef(కీలకం_పేర్లు_Info *info, KeycodeDef *stmt, bool report)
{
    ఒకవేళ (stmt->విలువ < 0 || stmt->విలువ > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->విలువ, XKB_KEYCODE_MAX);
        తిరిగి false;
    }

    తిరిగి కూడు_కీలకం_Name(info, (xkb_keycode_t) stmt->విలువ,
                      stmt->name, stmt->విలీనం, report);
}

స్థిర bool
HandleAliasDef(కీలకం_పేర్లు_Info *info, స్థిరరాశి కీలకం_Alias_Def *def, bool report)
{
    స్థిరరాశి Keycode_సరిపోలిక సరిపోలిక_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    ఒకవేళ (సరిపోలిక_name.దొరికింది) {
        స్థిరరాశి bool clobber = (def->విలీనం != విలీనం_AUGMENT);
        ఒకవేళ (సరిపోలిక_name.is_alias) {
            ఒకవేళ (def->real == సరిపోలిక_name.alias.real) {
                ఒకవేళ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             కీలకం_Name_పాఠ్యం(info->ctx, def->alias),
                             కీలకం_Name_పాఠ్యం(info->ctx, def->real));
            } లేకపోతే {
                స్థిరరాశి xkb_atom_t use = (clobber)
                    ? def->real
                    : సరిపోలిక_name.alias.real;
                స్థిరరాశి xkb_atom_t ignore = (clobber)
                    ? సరిపోలిక_name.alias.real
                    : def->real;

                ఒకవేళ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             కీలకం_Name_పాఠ్యం(info->ctx, def->alias),
                             కీలకం_Name_పాఠ్యం(info->ctx, use),
                             కీలకం_Name_పాఠ్యం(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            తిరిగి true;
        } లేకపోతే {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            ఒకవేళ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_కీలకం_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         కీలకం_Name_పాఠ్యం(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            ఒకవేళ (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_కీలకం(&info->keycodes, సరిపోలిక_name);
            } లేకపోతే {
                తిరిగి true;
            }
        }
    }

    తిరిగి keycode_store_చొప్పించు_alias(&info->keycodes, def->alias, def->real);
}

స్థిర bool
Handle_కీలకం_Name_Var(కీలకం_పేర్లు_Info *info, VarDef *stmt)
{
    స్థిరరాశి అక్షరం *elem, *క్షేత్రం_2;
    సమాసం_Def *arrayNdx;

    ఒకవేళ (!సమాసం_Resolve_Lhs(info->ctx, stmt->name, &elem, &క్షేత్రం_2, &arrayNdx))
        తిరిగి false;

    ఒకవేళ (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, క్షేత్రం_2);
        తిరిగి !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ఒకవేళ (!istreq(క్షేత్రం_2, "minimum") && !istreq(క్షేత్రం_2, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_క్షేత్రం,
                "Default defined for unknown field \"%s\"; Ignored\n", క్షేత్రం_2);
        తిరిగి !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ఒకవేళ (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", క్షేత్రం_2, "defaults");
        తిరిగి !(info->keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH);
    }

    int64_t val = 0;
    ఒకవేళ (!సమాసం_Resolve_Integer(info->ctx, stmt->విలువ, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE, "keycodes",
                      క్షేత్రం_2, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        తిరిగి !(info->keymap_info->strict & PARSER_NO_క్షేత్రం_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    తిరిగి true;
}

స్థిర bool
HandleLedNameDef(కీలకం_పేర్లు_Info *info, LedNameDef *def, bool report)
{
    ఒకవేళ (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_లెక్క++;
        log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        తిరిగి false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ఒకవేళ (!సమాసం_Resolve_String(info->ctx, def->name, &name)) {
        అక్షరం buf[20];
        snprintf(buf, పరిమాణం(buf), "%"PRId64, def->ndx);
        info->error_లెక్క++;
        తిరిగి ReportBadType(info->ctx, XKB_ERROR_WRONG_క్షేత్రం_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.విలీనం = def->విలీనం, .name = name};
    తిరిగి కూడు_Led_Name(info, true, &ledi, (xkb_led_సూచీ_t) def->ndx - 1, report);
}

స్థిర శూన్య
HandleKeycodesFile(కీలకం_పేర్లు_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    స్థిరరాశి పూర్ణాంకం verbosity = xkb_context_get_log_verbosity(info->ctx);
    స్థిరరాశి bool report_same_file = verbosity > 0;
    స్థిరరాశి bool report_include   = verbosity > 7;

    విడుదల(info->name);
    info->name = strdup_సురక్షిత(file->name);

    కొరకు (ParseCommon *stmt = file->defs; stmt; stmt = stmt->తదుపరి) {
        ఎంపిక (stmt->type) {
        స్థితి STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            ఆపు;
        స్థితి STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            ఆపు;
        స్థితి STMT_ALIAS:
            ok = HandleAliasDef(info, (కీలకం_Alias_Def *) stmt, report_same_file);
            ఆపు;
        స్థితి STMT_VAR:
            ok = Handle_కీలకం_Name_Var(info, (VarDef *) stmt);
            ఆపు;
        స్థితి STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            ఆపు;
        స్థితి STMT_UNKNOWN_DECLARATION:
        స్థితి STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ఆపు;
        అప్రమేయ:
            log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_గమ్యం_string(stmt->type));
            ok = false;
            ఆపు;
        }

        ఒకవేళ (!ok)
            info->error_లెక్క++;

        ఒకవేళ (info->error_లెక్క > 10) {
            log_err(info->ctx, XKB_LOG_సందేశం_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    సురక్షిత_map_name(file->name));
            ఆపు;
        }
    }
}

/***====================================================================***/

స్థిర bool
Copy_కీలకం_పేర్లు_గమ్యం_Keymap(నిర్మాణం xkb_keymap *keymap, కీలకం_పేర్లు_Info *info)
{
    ఒకవేళ (darray_ఖాళీలు(info->keycodes.దిగువ) && darray_ఖాళీలు(info->keycodes.ఎగువ)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        ప్రతిజ్ఞా(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_కీలకం_సంకేతం = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_కీలకం_సంకేతం = 255;
        keymap->అంకె_కీలకాలు = keymap->అంకె_కీలకాలు_దిగువ = keymap->max_కీలకం_సంకేతం + 1;
    } లేకపోతే {
        ప్రతిజ్ఞా(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_కీలకం_సంకేతం = info->keycodes.min;
        keymap->max_కీలకం_సంకేతం = (darray_ఖాళీలు(info->keycodes.ఎగువ))
            ? darray_కొలత(info->keycodes.దిగువ) - 1
            : darray_అంశం(
                info->keycodes.ఎగువ,
                darray_కొలత(info->keycodes.ఎగువ) - 1
            ).keycode;
        keymap->అంకె_కీలకాలు_దిగువ = darray_కొలత(info->keycodes.దిగువ);
        keymap->అంకె_కీలకాలు = keymap->అంకె_కీలకాలు_దిగువ
                         + darray_కొలత(info->keycodes.ఎగువ);
    }

    నిర్మాణం xkb_కీలకం * స్థిరరాశి కీలకాలు = సున్నా_జ్ఞాపకం_ఇవ్వు(keymap->అంకె_కీలకాలు, పరిమాణం(*కీలకాలు));
    ఒకవేళ (!కీలకాలు) {
        keymap->అంకె_కీలకాలు = 0;
        keymap->min_కీలకం_సంకేతం = keymap->max_కీలకం_సంకేతం = XKB_KEYCODE_INVALID;
        తిరిగి false;
    }

    /* Low keycodes */
    కొరకు (xkb_keycode_t kc = keymap->min_కీలకం_సంకేతం; kc < keymap->అంకె_కీలకాలు_దిగువ; kc++)
        కీలకాలు[kc].keycode = kc;

    కొరకు (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_కొలత(info->keycodes.దిగువ);
         kc++)
        కీలకాలు[kc].name = darray_అంశం(info->keycodes.దిగువ, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->అంకె_కీలకాలు_దిగువ; /* First high keycode index */
    స్థిరరాశి ఎగువ_Keycode_నమోదు *నమోదు_2;
    darray_foreach(నమోదు_2, info->keycodes.ఎగువ) {
        ప్రతిజ్ఞా(నమోదు_2->name != XKB_ATOM_NONE);
        కీలకాలు[idx].keycode = నమోదు_2->keycode;
        కీలకాలు[idx].name = నమోదు_2->name;
        idx++;
    }

    keymap->కీలకాలు = కీలకాలు;
    తిరిగి true;
}

స్థిర bool
CopyKeycodeNameLUT(నిర్మాణం xkb_keymap *keymap, కీలకం_పేర్లు_Info *info)
{
    Keycode_సరిపోలిక *సరిపోలిక;
    xkb_atom_t name;
    darray_enumerate(name, సరిపోలిక, info->keycodes.పేర్లు) {
        ఒకవేళ (!సరిపోలిక->దొరికింది)
            కొనసాగు;

        ఒకవేళ (సరిపోలిక->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            స్థిరరాశి Keycode_సరిపోలిక సరిపోలిక_real =
                keycode_store_lookup_name(&info->keycodes, సరిపోలిక->alias.real);
            ఒకవేళ (!సరిపోలిక_real.దొరికింది) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        కీలకం_Name_పాఠ్యం(info->ctx, name),
                        కీలకం_Name_పాఠ్యం(info->ctx, సరిపోలిక->alias.real));
                సరిపోలిక->దొరికింది = false;
                కొనసాగు;
            }
            ప్రతిజ్ఞా(!సరిపోలిక_real.is_alias);
        } లేకపోతే ఒకవేళ (!సరిపోలిక->కీలకం.దిగువ) {
            /* Update to final index in keymap::keys */
            సరిపోలిక->కీలకం.సూచీ += keymap->అంకె_కీలకాలు_దిగువ;
        }
    }

    darray_shrink(info->keycodes.పేర్లు);
    keymap->అంకె_కీలకం_పేర్లు = darray_కొలత(info->keycodes.పేర్లు);
    darray_steal(info->keycodes.పేర్లు, &keymap->కీలకం_పేర్లు, ఖాళీ);
    darray_init(info->keycodes.పేర్లు);
    తిరిగి true;
}

స్థిర bool
Copy_Led_పేర్లు_గమ్యం_Keymap(నిర్మాణం xkb_keymap *keymap, కీలకం_పేర్లు_Info *info)
{
    keymap->అంకె_leds = info->అంకె_led_పేర్లు;
    కొరకు (xkb_led_సూచీ_t idx = 0; idx < info->అంకె_led_పేర్లు; idx++) {
        LedNameInfo *ledi = &info->led_పేర్లు[idx];

        ఒకవేళ (ledi->name == XKB_ATOM_NONE)
            కొనసాగు;

        keymap->leds[idx].name = ledi->name;
    }

    తిరిగి true;
}

స్థిర bool
Copy_కీలకం_పేర్లు_Info_గమ్యం_Keymap(నిర్మాణం xkb_keymap *keymap, కీలకం_పేర్లు_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    ఒకవేళ (!Copy_కీలకం_పేర్లు_గమ్యం_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !Copy_Led_పేర్లు_గమ్యం_Keymap(keymap, info))
        తిరిగి false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    ఒకవేళ (!keymap->అంకె_కీలకాలు ||
        keymap->min_కీలకం_సంకేతం > 0) {
        keymap->redirect_కీలకం_auto = 0;
    } లేకపోతే {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        కొరకు (xkb_keycode_t k = keymap->అంకె_కీలకాలు; k-- > keymap->అంకె_కీలకాలు_దిగువ;) {
            ఒకవేళ (keycode > keymap->కీలకాలు[k].keycode)
                ఆపు;
            /* Try next keycode downwards */
            keycode = keymap->కీలకాలు[k].keycode - 1;
        }
        keymap->redirect_కీలకం_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_సురక్షిత(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    తిరిగి true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, నిర్మాణం xkb_keymap_info *keymap_info)
{
    కీలకం_పేర్లు_Info info;

    Init_కీలకం_పేర్లు_Info(&info, keymap_info, 0);

    ఒకవేళ (file != ఖాళీ)
        HandleKeycodesFile(&info, file);

    ఒకవేళ (info.error_లెక్క != 0)
        వెళ్ళు err_info;

    ఒకవేళ (!Copy_కీలకం_పేర్లు_Info_గమ్యం_Keymap(&keymap_info->keymap, &info))
        వెళ్ళు err_info;

    Clear_కీలకం_పేర్లు_Info(&info);
    తిరిగి true;

err_info:
    Clear_కీలకం_పేర్లు_Info(&info);
    తిరిగి false;
}
