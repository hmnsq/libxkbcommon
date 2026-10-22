/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"
#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "expr.h"
#ಅನ್ತರ್ಭಾವ "include.h"
#ಅನ್ತರ್ಭಾವ "util-mem.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} ಮೇಲಿನ_Keycode_ನಮೂದು;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) ಕೆಳಗಿನ;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(ಮೇಲಿನ_Keycode_ನಮೂದು) ಮೇಲಿನ;
    /**
     * name -> keycode mapping
     */
    darray(Keycode_ಹೊಂದಿಕೆ) ಹೆಸರುಗಳು;
} KeycodeStore;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->ಕೆಳಗಿನ);
    darray_init(store->ಮೇಲಿನ);
    darray_init(store->ಹೆಸರುಗಳು);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->ಕೆಳಗಿನ);
    darray_free(store->ಮೇಲಿನ);
    darray_free(store->ಹೆಸರುಗಳು);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
keycode_store_update_ಕೀಲಿ(KeycodeStore *store, Keycode_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ, xkb_atom_t name)
{
    ಆದರೆ (unlikely(!ಹೊಂದಿಕೆ.ಸಿಕ್ಕಿತು || ಹೊಂದಿಕೆ.is_alias)) {
        ಹಿಂತಿರುಗಿ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹೊಂದಿಕೆ.ಕೀಲಿ.ಕೆಳಗಿನ) {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ < darray_ಗಾತ್ರ(store->ಕೆಳಗಿನ));
        darray_ಅಂಶ(store->ಕೆಳಗಿನ, ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ) = name;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ < darray_ಗಾತ್ರ(store->ಮೇಲಿನ));
        darray_ಅಂಶ(store->ಮೇಲಿನ, ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ).name = name;
    }

    /* Names lookup */
    ಆದರೆ (name >= darray_ಗಾತ್ರ(store->ಹೆಸರುಗಳು)) {
        darray_resize0(store->ಹೆಸರುಗಳು, name + 1);
    }
    darray_ಅಂಶ(store->ಹೆಸರುಗಳು, name) = ಹೊಂದಿಕೆ;
}

ಸ್ಥಿರ bool
keycode_store_ಸೇರಿಸು_ಕೀಲಿ(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    ಆದರೆ (name >= darray_ಗಾತ್ರ(store->ಹೆಸರುಗಳು)) {
        darray_resize0(store->ಹೆಸರುಗಳು, name + 1);
    }
    ಆದರೆ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        ಆದರೆ (kc >= (xkb_keycode_t) darray_ಗಾತ್ರ(store->ಕೆಳಗಿನ))
            darray_resize0(store->ಕೆಳಗಿನ, kc + 1);
        darray_ಅಂಶ(store->ಕೆಳಗಿನ, kc) = name;
        ಆದರೆ (kc < store->min)
            store->min = kc;
        darray_ಅಂಶ(store->ಹೆಸರುಗಳು, name) = (Keycode_ಹೊಂದಿಕೆ) {
            .ಕೀಲಿ = {
                .ಸಿಕ್ಕಿತು = true,
                .ಕೆಳಗಿನ = true,
                .is_alias = false,
                .ಸೂಚ್ಯಂಕ = kc
            }
        };
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* High keycode: insert into a sorted list */
        ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t idx = darray_ಗಾತ್ರ(store->ಮೇಲಿನ);
        ಆದರೆ (idx && darray_ಅಂಶ(store->ಮೇಲಿನ, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_ಗಾತ್ರ_t ಅಧೋ = 0;
            darray_ಗಾತ್ರ_t ಊರ್ಧ್ವ = idx;
            ಆಗ (ಅಧೋ < ಊರ್ಧ್ವ) {
                ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t mid = ಅಧೋ + (ಊರ್ಧ್ವ - 1 - ಅಧೋ) / 2;
                ಸ್ಥಿರಾಂಕ ಮೇಲಿನ_Keycode_ನಮೂದು * ಸ್ಥಿರಾಂಕ ನಮೂದು =
                    &darray_ಅಂಶ(store->ಮೇಲಿನ, mid);
                ಆದರೆ (ನಮೂದು->keycode < kc) {
                    ಅಧೋ = mid + 1;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ನಮೂದು->keycode > kc) {
                    ಊರ್ಧ್ವ = mid;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    /* Unreachable: there is no repetition in the list */
                    ಪ್ರತಿಜ್ಞಾ(ನಮೂದು->keycode != kc);
                }
            }
            ಪ್ರತಿಜ್ಞಾ(ಅಧೋ < idx);
            ಪ್ರತಿಜ್ಞಾ(darray_ಅಂಶ(store->ಮೇಲಿನ, ಅಧೋ).keycode > kc);

            /* Update references to entries that will be moved */
            ಮೇಲಿನ_Keycode_ನಮೂದು *ನಮೂದು;
            darray_foreach_from(ನಮೂದು, store->ಮೇಲಿನ, ಅಧೋ) {
                darray_ಅಂಶ(store->ಹೆಸರುಗಳು, ನಮೂದು->name).ಕೀಲಿ.ಸೂಚ್ಯಂಕ++;
            }

            darray_ಸೇರಿಸು(store->ಮೇಲಿನ, ಅಧೋ,
                          (ಮೇಲಿನ_Keycode_ನಮೂದು){.keycode = kc, .name = name});
            darray_ಅಂಶ(store->ಹೆಸರುಗಳು, name) = (Keycode_ಹೊಂದಿಕೆ) {
                .ಕೀಲಿ = {
                    .ಸಿಕ್ಕಿತು = true,
                    .ಕೆಳಗಿನ = false,
                    .is_alias = false,
                    .ಸೂಚ್ಯಂಕ = ಅಧೋ
                }
            };
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Fast path: no need to sort */
            darray_append(store->ಮೇಲಿನ,
                          (ಮೇಲಿನ_Keycode_ನಮೂದು){.keycode = kc, .name = name});
            darray_ಅಂಶ(store->ಹೆಸರುಗಳು, name) = (Keycode_ಹೊಂದಿಕೆ) {
                .ಕೀಲಿ = {
                    .ಸಿಕ್ಕಿತು = true,
                    .ಕೆಳಗಿನ = false,
                    .is_alias = false,
                    .ಸೂಚ್ಯಂಕ = idx
                }
            };
        }
        ಆದರೆ (darray_ಖಾಲಿಗಳು(store->ಕೆಳಗಿನ))
            store->min = darray_ಅಂಶ(store->ಮೇಲಿನ, 0).keycode;
    }
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
keycode_store_ಸೇರಿಸು_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    ಆದರೆ (alias >= darray_ಗಾತ್ರ(store->ಹೆಸರುಗಳು)) {
        darray_resize0(store->ಹೆಸರುಗಳು, alias + 1);
    }
    darray_ಅಂಶ(store->ಹೆಸರುಗಳು, alias) = (Keycode_ಹೊಂದಿಕೆ) {
        .alias = {
            .ಸಿಕ್ಕಿತು = true,
            .is_alias = true,
            .real = real
        }
    };
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_ಅಂಶ(store->ಹೆಸರುಗಳು, alias).alias.real = real;
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
keycode_store_delete_name(ಸ್ಥಿರಾಂಕ KeycodeStore *store, xkb_atom_t name)
{
    ಆದರೆ (name < darray_ಗಾತ್ರ(store->ಹೆಸರುಗಳು))
        darray_ಅಂಶ(store->ಹೆಸರುಗಳು, name).ಸಿಕ್ಕಿತು = false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
keycode_store_delete_ಕೀಲಿ(KeycodeStore *store, ಸ್ಥಿರಾಂಕ Keycode_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ)
{
    ಆದರೆ (unlikely(!ಹೊಂದಿಕೆ.ಸಿಕ್ಕಿತು || ಹೊಂದಿಕೆ.is_alias)) {
        ಹಿಂತಿರುಗಿ;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹೊಂದಿಕೆ.ಕೀಲಿ.ಕೆಳಗಿನ) {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ < darray_ಗಾತ್ರ(store->ಕೆಳಗಿನ));
        darray_ಅಂಶ(store->ಹೆಸರುಗಳು,
                    darray_ಅಂಶ(store->ಕೆಳಗಿನ, ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ)).ಸಿಕ್ಕಿತು = false;
        ಆದರೆ (ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ + 1u == darray_ಗಾತ್ರ(store->ಕೆಳಗಿನ)) {
            /* Highest low keycode: shrink */
            ಆದರೆ (store->min == ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ) {
                /* No low keycode left */
                darray_ಗಾತ್ರ(store->ಕೆಳಗಿನ) = 0;
            } ಇಲ್ಲದಿದ್ದರೆ {
                /* Look for previous defined low keycode */
                ಗಾಗಿ (darray_ಗಾತ್ರ_t idx = ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ; idx > 0; idx--) {
                    ಆದರೆ (darray_ಅಂಶ(store->ಕೆಳಗಿನ, idx - 1) != XKB_ATOM_NONE) {
                        darray_ಗಾತ್ರ(store->ಕೆಳಗಿನ) = idx;
                        ನಿಲ್ಲಿಸು;
                    }
                }
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* Lower keycode: reset */
            darray_ಅಂಶ(store->ಕೆಳಗಿನ, ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ) = XKB_ATOM_NONE;
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಪ್ರತಿಜ್ಞಾ(ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ < darray_ಗಾತ್ರ(store->ಮೇಲಿನ)); /* assume valid index */
        darray_ಅಂಶ(store->ಹೆಸರುಗಳು,
                    darray_ಅಂಶ(store->ಮೇಲಿನ, ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ).name).ಸಿಕ್ಕಿತು = false;
        darray_remove(store->ಮೇಲಿನ, ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ);
        /* Update LUT indexes of high codes after the deleted one, if any */
        Keycode_ಹೊಂದಿಕೆ *ನಮೂದು;
        darray_foreach(ನಮೂದು, store->ಹೆಸರುಗಳು) {
            ಆದರೆ (ನಮೂದು->ಸಿಕ್ಕಿತು && !ನಮೂದು->is_alias && !ನಮೂದು->ಕೀಲಿ.ಕೆಳಗಿನ &&
                ನಮೂದು->ಕೀಲಿ.ಸೂಚ್ಯಂಕ > ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ) {
                ನಮೂದು->ಕೀಲಿ.ಸೂಚ್ಯಂಕ--;
            }
        }
    }

    /* Update bounds */
    ಆದರೆ (darray_ಖಾಲಿಗಳು(store->ಕೆಳಗಿನ)) {
        store->min = (darray_ಖಾಲಿಗಳು(store->ಮೇಲಿನ))
            ? XKB_KEYCODE_INVALID
            : darray_ಅಂಶ(store->ಮೇಲಿನ, 0).keycode;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಗಾಗಿ (xkb_keycode_t kc = store->min; kc < darray_ಗಾತ್ರ(store->ಕೆಳಗಿನ); kc++) {
            ಆದರೆ (darray_ಅಂಶ(store->ಕೆಳಗಿನ, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                ನಿಲ್ಲಿಸು;
            }
        }
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_keycode_t
keycode_store_get_keycode(ಸ್ಥಿರಾಂಕ KeycodeStore *store, Keycode_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ)
{
    ಆದರೆ (!ಹೊಂದಿಕೆ.ಸಿಕ್ಕಿತು || ಹೊಂದಿಕೆ.is_alias) {
        ಹಿಂತಿರುಗಿ XKB_KEYCODE_INVALID;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹೊಂದಿಕೆ.ಕೀಲಿ.ಕೆಳಗಿನ) {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ < darray_ಗಾತ್ರ(store->ಕೆಳಗಿನ));
        ಹಿಂತಿರುಗಿ (xkb_keycode_t) ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ < darray_ಗಾತ್ರ(store->ಮೇಲಿನ));
        ಹಿಂತಿರುಗಿ darray_ಅಂಶ(store->ಮೇಲಿನ, ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ).keycode;
    }
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ xkb_atom_t
keycode_store_get_ಕೀಲಿ_name(ಸ್ಥಿರಾಂಕ KeycodeStore *store, Keycode_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ)
{
    ಆದರೆ (!ಹೊಂದಿಕೆ.ಸಿಕ್ಕಿತು || ಹೊಂದಿಕೆ.is_alias) {
        ಹಿಂತಿರುಗಿ XKB_ATOM_NONE;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹೊಂದಿಕೆ.ಕೀಲಿ.ಕೆಳಗಿನ) {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ < darray_ಗಾತ್ರ(store->ಕೆಳಗಿನ));
        ಹಿಂತಿರುಗಿ darray_ಅಂಶ(store->ಕೆಳಗಿನ, ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ);
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ < darray_ಗಾತ್ರ(store->ಮೇಲಿನ));
        ಹಿಂತಿರುಗಿ darray_ಅಂಶ(store->ಮೇಲಿನ, ಹೊಂದಿಕೆ.ಕೀಲಿ.ಸೂಚ್ಯಂಕ).name;
    }
}

ಸ್ಥಿರ Keycode_ಹೊಂದಿಕೆ
keycode_store_lookup_keycode(ಸ್ಥಿರಾಂಕ KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    ಆದರೆ (kc < (xkb_keycode_t) darray_ಗಾತ್ರ(store->ಕೆಳಗಿನ)) {
        ಹಿಂತಿರುಗಿ (Keycode_ಹೊಂದಿಕೆ) {
            .ಕೀಲಿ = {
                .ಸಿಕ್ಕಿತು = true,
                .ಕೆಳಗಿನ = true,
                .is_alias = false,
                .ಸೂಚ್ಯಂಕ = kc
            }
        };
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        ಹಿಂತಿರುಗಿ (Keycode_ಹೊಂದಿಕೆ) { .ಸಿಕ್ಕಿತು = false };
    }

    /* High keycodes: use binary search */
    darray_ಗಾತ್ರ_t ಅಧೋ = 0;
    darray_ಗಾತ್ರ_t ಊರ್ಧ್ವ = darray_ಗಾತ್ರ(store->ಮೇಲಿನ);
    ಆಗ (ಅಧೋ < ಊರ್ಧ್ವ) {
        ಸ್ಥಿರಾಂಕ darray_ಗಾತ್ರ_t mid = ಅಧೋ + (ಊರ್ಧ್ವ - 1 - ಅಧೋ) / 2;
        ಮೇಲಿನ_Keycode_ನಮೂದು * ಸ್ಥಿರಾಂಕ ನಮೂದು = &darray_ಅಂಶ(store->ಮೇಲಿನ, mid);
        ಆದರೆ (ನಮೂದು->keycode < kc) {
            ಅಧೋ = mid + 1;
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ನಮೂದು->keycode > kc) {
            ಊರ್ಧ್ವ = mid;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಹಿಂತಿರುಗಿ (Keycode_ಹೊಂದಿಕೆ) {
                .ಕೀಲಿ = {
                    .ಸಿಕ್ಕಿತು = true,
                    .ಕೆಳಗಿನ = false,
                    .is_alias = false,
                    .ಸೂಚ್ಯಂಕ = mid
                }
            };
        }
    }

    ಹಿಂತಿರುಗಿ (Keycode_ಹೊಂದಿಕೆ) { .ಸಿಕ್ಕಿತು = false };
}

ಸ್ಥಿರ Keycode_ಹೊಂದಿಕೆ
keycode_store_lookup_name(ಸ್ಥಿರಾಂಕ KeycodeStore *store, xkb_atom_t name)
{
    ಆದರೆ (name >= darray_ಗಾತ್ರ(store->ಹೆಸರುಗಳು)) {
        ಹಿಂತಿರುಗಿ (Keycode_ಹೊಂದಿಕೆ) { .ಸಿಕ್ಕಿತು = false, .is_alias = false };
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಹಿಂತಿರುಗಿ darray_ಅಂಶ(store->ಹೆಸರುಗಳು, name);
    }
}

/***====================================================================***/

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ;

    xkb_atom_t name;
} LedNameInfo;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಅಕ್ಷರ *name;
    ಪೂರ್ಣಾಂಕ error_ಎಣಿಕೆ;
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ;

    KeycodeStore keycodes;
    LedNameInfo led_ಹೆಸರುಗಳು[XKB_MAX_LEDS];
    xkb_led_ಸೂಚ್ಯಂಕ_t ಅಂಕೆ_led_ಹೆಸರುಗಳು;

    ರಚನೆ xkb_context *ctx;
    ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info;
} ಕೀಲಿ_ಹೆಸರುಗಳು_Info;

/***====================================================================***/

ಸ್ಥಿರ LedNameInfo *
ಕಂಡುಹಿಡಿ_Led_By_Name(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, xkb_atom_t name,
              xkb_led_ಸೂಚ್ಯಂಕ_t *idx_ಹೊರಹೋಗು)
{
    ಗಾಗಿ (xkb_led_ಸೂಚ್ಯಂಕ_t idx = 0; idx < info->ಅಂಕೆ_led_ಹೆಸರುಗಳು; idx++) {
        LedNameInfo *ledi = &info->led_ಹೆಸರುಗಳು[idx];
        ಆದರೆ (ledi->name == name) {
            *idx_ಹೊರಹೋಗು = idx;
            ಹಿಂತಿರುಗಿ ledi;
        }
    }

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ bool
ಕೂಡಿಸು_Led_Name(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_ಸೂಚ್ಯಂಕ_t new_idx, bool report)
{
    xkb_led_ಸೂಚ್ಯಂಕ_t old_idx;
    LedNameInfo *old;
    ಸ್ಥಿರಾಂಕ bool replace = (new->ವಿಲೀನ != ವಿಲೀನ_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = ಕಂಡುಹಿಡಿ_Led_By_Name(info, new->name, &old_idx);
    ಆದರೆ (old) {
        ಆದರೆ (old_idx == new_idx) {
            ಆದರೆ (report)
                log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_ಪಠ್ಯ(info->ctx, new->name));
            ಹಿಂತಿರುಗಿ true;
        }

        ಆದರೆ (report) {
            xkb_led_ಸೂಚ್ಯಂಕ_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_ಸೂಚ್ಯಂಕ_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_ಪಠ್ಯ(info->ctx, new->name), use, ignore);
        }

        ಆದರೆ (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಹಿಂತಿರುಗಿ true;
        }
    }

    ಆದರೆ (new_idx >= info->ಅಂಕೆ_led_ಹೆಸರುಗಳು)
        info->ಅಂಕೆ_led_ಹೆಸರುಗಳು = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_ಹೆಸರುಗಳು[new_idx];
    ಆದರೆ (old->name != XKB_ATOM_NONE) {
        ಆದರೆ (report) {
            ಸ್ಥಿರಾಂಕ xkb_atom_t use    = (replace ? new->name : old->name);
            ಸ್ಥಿರಾಂಕ xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_ಪಠ್ಯ(info->ctx, use),
                     xkb_atom_ಪಠ್ಯ(info->ctx, ignore));
        }

        ಆದರೆ (replace)
            *old = *new;

        ಹಿಂತಿರುಗಿ true;
    }

    *old = *new;
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
Clear_ಕೀಲಿ_ಹೆಸರುಗಳು_Info(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info)
{
    ಬಿಡುಗಡೆ(info->name);
    keycode_store_free(&info->keycodes);
}

ಸ್ಥಿರ ಶೂನ್ಯ
Init_ಕೀಲಿ_ಹೆಸರುಗಳು_Info(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, ಸ್ಥಿರಾಂಕ ರಚನೆ xkb_keymap_info *keymap_info,
                 ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ include_ಆಳ)
{
    memset(info, 0, ಗಾತ್ರ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_ಆಳ = include_ಆಳ;
    keycode_store_init(&info->keycodes);
}

ಸ್ಥಿರ bool
ಕೂಡಿಸು_ಕೀಲಿ_Name(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ, bool report)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NDEBUG
    ಸ್ಥಿರಾಂಕ
#ಆದರೆ_ಅಂತ್ಯ
    Keycode_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ_name = keycode_store_lookup_name(&info->keycodes, name);
    ಆದರೆ (ಹೊಂದಿಕೆ_name.ಸಿಕ್ಕಿತು) {
        ಸ್ಥಿರಾಂಕ bool clobber = (ವಿಲೀನ != ವಿಲೀನ_AUGMENT);

        ಆದರೆ (ಹೊಂದಿಕೆ_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            ಆದರೆ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            ಆದರೆ (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NDEBUG
                ಹೊಂದಿಕೆ_name.ಸಿಕ್ಕಿತು = false;
#ಆದರೆ_ಅಂತ್ಯ
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಹಿಂತಿರುಗಿ true;
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            ಸ್ಥಿರಾಂಕ xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, ಹೊಂದಿಕೆ_name);
            ಪ್ರತಿಜ್ಞಾ(old_kc != XKB_KEYCODE_INVALID);
            ಆದರೆ (old_kc != kc) {
                /* There is already a different key with this name. */

                ಆದರೆ (report) {
                    ಸ್ಥಿರಾಂಕ xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ಸ್ಥಿರಾಂಕ xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, name), use, ignore);
                }

                ಆದರೆ (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_ಕೀಲಿ(&info->keycodes, ಹೊಂದಿಕೆ_name);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ಹಿಂತಿರುಗಿ true;
                }
            }
        }
    }

    ಸ್ಥಿರಾಂಕ Keycode_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ಸ್ಥಿರಾಂಕ xkb_atom_t old_name = keycode_store_get_ಕೀಲಿ_name(&info->keycodes, ಹೊಂದಿಕೆ_kc);
    ಆದರೆ (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        ಆದರೆ (old_name == name) {
            ಪ್ರತಿಜ್ಞಾ(keycode_store_get_keycode(&info->keycodes, ಹೊಂದಿಕೆ_name) == kc);
            ಆದರೆ (report)
                log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, old_name), kc);
            ಹಿಂತಿರುಗಿ true;
        }

        ಸ್ಥಿರಾಂಕ bool clobber = (ವಿಲೀನ != ವಿಲೀನ_AUGMENT);
        ಆದರೆ (report) {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ kname     = ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, name);
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ old_kname = ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, old_name);
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ use    = (clobber) ? kname : old_kname;
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        ಆದರೆ (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_ಕೀಲಿ(&info->keycodes, ಹೊಂದಿಕೆ_kc, name);
        }
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* No previous keycode */
        ಆದರೆ (!keycode_store_ಸೇರಿಸು_ಕೀಲಿ(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ಹಂಚಿಕೆ_FAILURE_2,
                    "Cannot add keycode\n");
            ಹಿಂತಿರುಗಿ false;
        }
    }

    ಹಿಂತಿರುಗಿ true;
}

/***====================================================================***/

ಸ್ಥಿರ bool
HandleAliasDef(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, ಸ್ಥಿರಾಂಕ ಕೀಲಿ_Alias_Def *def, bool report);

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಲೀನ_Keycode_Stores(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *into, ಕೀಲಿ_ಹೆಸರುಗಳು_Info *from,
                   ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ, bool report)
{
    ಆದರೆ (darray_ಖಾಲಿಗಳು(into->keycodes.ಕೆಳಗಿನ) && darray_ಖಾಲಿಗಳು(into->keycodes.ಮೇಲಿನ) &&
        darray_ಖಾಲಿಗಳು(into->keycodes.ಹೆಸರುಗಳು)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.ಕೆಳಗಿನ);
        darray_init(from->keycodes.ಮೇಲಿನ);
        darray_init(from->keycodes.ಹೆಸರುಗಳು);
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        ಗಾಗಿ (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_ಗಾತ್ರ(from->keycodes.ಕೆಳಗಿನ);
             kc++) {
            /* Skip undefined keycodes */
            ಸ್ಥಿರಾಂಕ xkb_atom_t name = darray_ಅಂಶ(from->keycodes.ಕೆಳಗಿನ, kc);
            ಆದರೆ (name == XKB_ATOM_NONE)
                ಮುಂದುವರಿ;

            ಆದರೆ (!ಕೂಡಿಸು_ಕೀಲಿ_Name(into, kc, name, ವಿಲೀನ, report))
                into->error_ಎಣಿಕೆ++;
        }

        /* High keycodes */
        ಸ್ಥಿರಾಂಕ ಮೇಲಿನ_Keycode_ನಮೂದು *new;
        darray_foreach(new, from->keycodes.ಮೇಲಿನ) {
            ಪ್ರತಿಜ್ಞಾ(new->name != XKB_ATOM_NONE);
            ಆದರೆ (!ಕೂಡಿಸು_ಕೀಲಿ_Name(into, new->keycode, new->name, ವಿಲೀನ, report))
                into->error_ಎಣಿಕೆ++;
        }

        /* Aliases. */
        Keycode_ಹೊಂದಿಕೆ *ಹೊಂದಿಕೆ;
        xkb_atom_t alias;
        darray_enumerate(alias, ಹೊಂದಿಕೆ, from->keycodes.ಹೆಸರುಗಳು) {
            ಆದರೆ (!ಹೊಂದಿಕೆ->ಸಿಕ್ಕಿತು || !ಹೊಂದಿಕೆ->is_alias)
                ಮುಂದುವರಿ;

            ಸ್ಥಿರಾಂಕ ಕೀಲಿ_Alias_Def def = {
                .ವಿಲೀನ = ವಿಲೀನ,
                .alias = alias,
                .real = ಹೊಂದಿಕೆ->alias.real
            };
            ಆದರೆ (!HandleAliasDef(into, &def, report))
                into->error_ಎಣಿಕೆ++;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
ವಿಲೀನ_Included_Keycodes(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *into, ಕೀಲಿ_ಹೆಸರುಗಳು_Info *from,
                      ಎಣಿಕೆ ವಿಲೀನ_ಬಹುಲಕ ವಿಲೀನ, bool report)
{
    ಆದರೆ (from->error_ಎಣಿಕೆ > 0) {
        into->error_ಎಣಿಕೆ += from->error_ಎಣಿಕೆ;
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (into->name == ಖಾಲಿ) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    ವಿಲೀನ_Keycode_Stores(into, from, ವಿಲೀನ, report);

    /* Merge LED names. */
    ಆದರೆ (into->ಅಂಕೆ_led_ಹೆಸರುಗಳು == 0) {
        ಸ್ಮರಣೆ_ನಕಲು(into->led_ಹೆಸರುಗಳು, from->led_ಹೆಸರುಗಳು,
               ಗಾತ್ರ(*from->led_ಹೆಸರುಗಳು) * from->ಅಂಕೆ_led_ಹೆಸರುಗಳು);
        into->ಅಂಕೆ_led_ಹೆಸರುಗಳು = from->ಅಂಕೆ_led_ಹೆಸರುಗಳು;
        from->ಅಂಕೆ_led_ಹೆಸರುಗಳು = 0;
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        ಗಾಗಿ (xkb_led_ಸೂಚ್ಯಂಕ_t idx = 0; idx < from->ಅಂಕೆ_led_ಹೆಸರುಗಳು; idx++) {
            LedNameInfo *ledi = &from->led_ಹೆಸರುಗಳು[idx];

            ಆದರೆ (ledi->name == XKB_ATOM_NONE)
                ಮುಂದುವರಿ;

            ledi->ವಿಲೀನ = ವಿಲೀನ;
            ಆದರೆ (!ಕೂಡಿಸು_Led_Name(into, false, ledi, idx, report))
                into->error_ಎಣಿಕೆ++;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleKeycodesFile(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, XkbFile *file);

ಸ್ಥಿರ bool
HandleIncludeKeycodes(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, IncludeStmt *include, bool report)
{
    ಕೀಲಿ_ಹೆಸರುಗಳು_Info included;

    ಆದರೆ (Exceeds_Include_Max_ಆಳ(info->ctx, info->include_ಆಳ)) {
        info->error_ಎಣಿಕೆ += 10;
        ಹಿಂತಿರುಗಿ false;
    }

    Init_ಕೀಲಿ_ಹೆಸರುಗಳು_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ಸ್ಥಿರಾಂಕ ರಚನೆ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ಗಾಗಿ (IncludeStmt *stmt = include; stmt; stmt = stmt->ಮುಂದಿನ_incl) {
        ಕೀಲಿ_ಹೆಸರುಗಳು_Info ಮುಂದಿನ_incl;
        XkbFile *file;

        ಅಕ್ಷರ ಮಾರ್ಗ[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  ಮಾರ್ಗ, ಗಾತ್ರ(ಮಾರ್ಗ));
        ಆದರೆ (!file) {
            info->error_ಎಣಿಕೆ += 10;
            Clear_ಕೀಲಿ_ಹೆಸರುಗಳು_Info(&included);
            ಹಿಂತಿರುಗಿ false;
        }

        Init_ಕೀಲಿ_ಹೆಸರುಗಳು_Info(&ಮುಂದಿನ_incl, info->keymap_info, info->include_ಆಳ + 1);

        HandleKeycodesFile(&ಮುಂದಿನ_incl, file);

        ವಿಲೀನ_Included_Keycodes(&included, &ಮುಂದಿನ_incl, stmt->ವಿಲೀನ, report);

        Clear_ಕೀಲಿ_ಹೆಸರುಗಳು_Info(&ಮುಂದಿನ_incl);
        FreeXkbFile(file);
    }

    ವಿಲೀನ_Included_Keycodes(info, &included, include->ವಿಲೀನ, report);
    Clear_ಕೀಲಿ_ಹೆಸರುಗಳು_Info(&included);

    ಹಿಂತಿರುಗಿ (info->error_ಎಣಿಕೆ == 0);
}

ಸ್ಥಿರ bool
HandleKeycodeDef(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, KeycodeDef *stmt, bool report)
{
    ಆದರೆ (stmt->ಮೌಲ್ಯ < 0 || stmt->ಮೌಲ್ಯ > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->ಮೌಲ್ಯ, XKB_KEYCODE_MAX);
        ಹಿಂತಿರುಗಿ false;
    }

    ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_ಕೀಲಿ_Name(info, (xkb_keycode_t) stmt->ಮೌಲ್ಯ,
                      stmt->name, stmt->ವಿಲೀನ, report);
}

ಸ್ಥಿರ bool
HandleAliasDef(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, ಸ್ಥಿರಾಂಕ ಕೀಲಿ_Alias_Def *def, bool report)
{
    ಸ್ಥಿರಾಂಕ Keycode_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    ಆದರೆ (ಹೊಂದಿಕೆ_name.ಸಿಕ್ಕಿತು) {
        ಸ್ಥಿರಾಂಕ bool clobber = (def->ವಿಲೀನ != ವಿಲೀನ_AUGMENT);
        ಆದರೆ (ಹೊಂದಿಕೆ_name.is_alias) {
            ಆದರೆ (def->real == ಹೊಂದಿಕೆ_name.alias.real) {
                ಆದರೆ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, def->alias),
                             ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, def->real));
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಸ್ಥಿರಾಂಕ xkb_atom_t use = (clobber)
                    ? def->real
                    : ಹೊಂದಿಕೆ_name.alias.real;
                ಸ್ಥಿರಾಂಕ xkb_atom_t ignore = (clobber)
                    ? ಹೊಂದಿಕೆ_name.alias.real
                    : def->real;

                ಆದರೆ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, def->alias),
                             ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, use),
                             ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            ಹಿಂತಿರುಗಿ true;
        } ಇಲ್ಲದಿದ್ದರೆ {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            ಆದರೆ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            ಆದರೆ (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_ಕೀಲಿ(&info->keycodes, ಹೊಂದಿಕೆ_name);
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಹಿಂತಿರುಗಿ true;
            }
        }
    }

    ಹಿಂತಿರುಗಿ keycode_store_ಸೇರಿಸು_alias(&info->keycodes, def->alias, def->real);
}

ಸ್ಥಿರ bool
Handle_ಕೀಲಿ_Name_Var(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, VarDef *stmt)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *elem, *ಕ್ಷೇತ್ರ_2;
    ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;

    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, stmt->name, &elem, &ಕ್ಷೇತ್ರ_2, &arrayNdx))
        ಹಿಂತಿರುಗಿ false;

    ಆದರೆ (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, ಕ್ಷೇತ್ರ_2);
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ಆದರೆ (!istreq(ಕ್ಷೇತ್ರ_2, "minimum") && !istreq(ಕ್ಷೇತ್ರ_2, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ಕ್ಷೇತ್ರ,
                "Default defined for unknown field \"%s\"; Ignored\n", ಕ್ಷೇತ್ರ_2);
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ಆದರೆ (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", ಕ್ಷೇತ್ರ_2, "defaults");
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH);
    }

    int64_t val = 0;
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(info->ctx, stmt->ಮೌಲ್ಯ, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE, "keycodes",
                      ಕ್ಷೇತ್ರ_2, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        ಹಿಂತಿರುಗಿ !(info->keymap_info->strict & PARSER_NO_ಕ್ಷೇತ್ರ_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
HandleLedNameDef(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, LedNameDef *def, bool report)
{
    ಆದರೆ (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_ಎಣಿಕೆ++;
        log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        ಹಿಂತಿರುಗಿ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ಆದರೆ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(info->ctx, def->name, &name)) {
        ಅಕ್ಷರ buf[20];
        snprintf(buf, ಗಾತ್ರ(buf), "%"PRId64, def->ndx);
        info->error_ಎಣಿಕೆ++;
        ಹಿಂತಿರುಗಿ ReportBadType(info->ctx, XKB_ERROR_WRONG_ಕ್ಷೇತ್ರ_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.ವಿಲೀನ = def->ವಿಲೀನ, .name = name};
    ಹಿಂತಿರುಗಿ ಕೂಡಿಸು_Led_Name(info, true, &ledi, (xkb_led_ಸೂಚ್ಯಂಕ_t) def->ndx - 1, report);
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleKeycodesFile(ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ಸ್ಥಿರಾಂಕ bool report_same_file = verbosity > 0;
    ಸ್ಥಿರಾಂಕ bool report_include   = verbosity > 7;

    ಬಿಡುಗಡೆ(info->name);
    info->name = strdup_ಸುರಕ್ಷಿತ(file->name);

    ಗಾಗಿ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ಮುಂದಿನ) {
        ಆಯ್ಕೆ (stmt->type) {
        ಸ್ಥಿತಿ STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_ALIAS:
            ok = HandleAliasDef(info, (ಕೀಲಿ_Alias_Def *) stmt, report_same_file);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_VAR:
            ok = Handle_ಕೀಲಿ_Name_Var(info, (VarDef *) stmt);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ STMT_UNKNOWN_DECLARATION:
        ಸ್ಥಿತಿ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
            log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_ಗಮ್ಯ_string(stmt->type));
            ok = false;
            ನಿಲ್ಲಿಸು;
        }

        ಆದರೆ (!ok)
            info->error_ಎಣಿಕೆ++;

        ಆದರೆ (info->error_ಎಣಿಕೆ > 10) {
            log_err(info->ctx, XKB_LOG_ಸಂದೇಶ_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    ಸುರಕ್ಷಿತ_map_name(file->name));
            ನಿಲ್ಲಿಸು;
        }
    }
}

/***====================================================================***/

ಸ್ಥಿರ bool
Copy_ಕೀಲಿ_ಹೆಸರುಗಳು_ಗಮ್ಯ_Keymap(ರಚನೆ xkb_keymap *keymap, ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info)
{
    ಆದರೆ (darray_ಖಾಲಿಗಳು(info->keycodes.ಕೆಳಗಿನ) && darray_ಖಾಲಿಗಳು(info->keycodes.ಮೇಲಿನ)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        ಪ್ರತಿಜ್ಞಾ(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_ಕೀಲಿ_ಸಂಕೇತ = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_ಕೀಲಿ_ಸಂಕೇತ = 255;
        keymap->ಅಂಕೆ_ಕೀಲಿಗಳು = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ = keymap->max_ಕೀಲಿ_ಸಂಕೇತ + 1;
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಪ್ರತಿಜ್ಞಾ(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_ಕೀಲಿ_ಸಂಕೇತ = info->keycodes.min;
        keymap->max_ಕೀಲಿ_ಸಂಕೇತ = (darray_ಖಾಲಿಗಳು(info->keycodes.ಮೇಲಿನ))
            ? darray_ಗಾತ್ರ(info->keycodes.ಕೆಳಗಿನ) - 1
            : darray_ಅಂಶ(
                info->keycodes.ಮೇಲಿನ,
                darray_ಗಾತ್ರ(info->keycodes.ಮೇಲಿನ) - 1
            ).keycode;
        keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ = darray_ಗಾತ್ರ(info->keycodes.ಕೆಳಗಿನ);
        keymap->ಅಂಕೆ_ಕೀಲಿಗಳು = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ
                         + darray_ಗಾತ್ರ(info->keycodes.ಮೇಲಿನ);
    }

    ರಚನೆ xkb_ಕೀಲಿ * ಸ್ಥಿರಾಂಕ ಕೀಲಿಗಳು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(keymap->ಅಂಕೆ_ಕೀಲಿಗಳು, ಗಾತ್ರ(*ಕೀಲಿಗಳು));
    ಆದರೆ (!ಕೀಲಿಗಳು) {
        keymap->ಅಂಕೆ_ಕೀಲಿಗಳು = 0;
        keymap->min_ಕೀಲಿ_ಸಂಕೇತ = keymap->max_ಕೀಲಿ_ಸಂಕೇತ = XKB_KEYCODE_INVALID;
        ಹಿಂತಿರುಗಿ false;
    }

    /* Low keycodes */
    ಗಾಗಿ (xkb_keycode_t kc = keymap->min_ಕೀಲಿ_ಸಂಕೇತ; kc < keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ; kc++)
        ಕೀಲಿಗಳು[kc].keycode = kc;

    ಗಾಗಿ (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_ಗಾತ್ರ(info->keycodes.ಕೆಳಗಿನ);
         kc++)
        ಕೀಲಿಗಳು[kc].name = darray_ಅಂಶ(info->keycodes.ಕೆಳಗಿನ, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ; /* First high keycode index */
    ಸ್ಥಿರಾಂಕ ಮೇಲಿನ_Keycode_ನಮೂದು *ನಮೂದು;
    darray_foreach(ನಮೂದು, info->keycodes.ಮೇಲಿನ) {
        ಪ್ರತಿಜ್ಞಾ(ನಮೂದು->name != XKB_ATOM_NONE);
        ಕೀಲಿಗಳು[idx].keycode = ನಮೂದು->keycode;
        ಕೀಲಿಗಳು[idx].name = ನಮೂದು->name;
        idx++;
    }

    keymap->ಕೀಲಿಗಳು = ಕೀಲಿಗಳು;
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
CopyKeycodeNameLUT(ರಚನೆ xkb_keymap *keymap, ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info)
{
    Keycode_ಹೊಂದಿಕೆ *ಹೊಂದಿಕೆ;
    xkb_atom_t name;
    darray_enumerate(name, ಹೊಂದಿಕೆ, info->keycodes.ಹೆಸರುಗಳು) {
        ಆದರೆ (!ಹೊಂದಿಕೆ->ಸಿಕ್ಕಿತು)
            ಮುಂದುವರಿ;

        ಆದರೆ (ಹೊಂದಿಕೆ->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ಸ್ಥಿರಾಂಕ Keycode_ಹೊಂದಿಕೆ ಹೊಂದಿಕೆ_real =
                keycode_store_lookup_name(&info->keycodes, ಹೊಂದಿಕೆ->alias.real);
            ಆದರೆ (!ಹೊಂದಿಕೆ_real.ಸಿಕ್ಕಿತು) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, name),
                        ಕೀಲಿ_Name_ಪಠ್ಯ(info->ctx, ಹೊಂದಿಕೆ->alias.real));
                ಹೊಂದಿಕೆ->ಸಿಕ್ಕಿತು = false;
                ಮುಂದುವರಿ;
            }
            ಪ್ರತಿಜ್ಞಾ(!ಹೊಂದಿಕೆ_real.is_alias);
        } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!ಹೊಂದಿಕೆ->ಕೀಲಿ.ಕೆಳಗಿನ) {
            /* Update to final index in keymap::keys */
            ಹೊಂದಿಕೆ->ಕೀಲಿ.ಸೂಚ್ಯಂಕ += keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ;
        }
    }

    darray_shrink(info->keycodes.ಹೆಸರುಗಳು);
    keymap->ಅಂಕೆ_ಕೀಲಿ_ಹೆಸರುಗಳು = darray_ಗಾತ್ರ(info->keycodes.ಹೆಸರುಗಳು);
    darray_steal(info->keycodes.ಹೆಸರುಗಳು, &keymap->ಕೀಲಿ_ಹೆಸರುಗಳು, ಖಾಲಿ);
    darray_init(info->keycodes.ಹೆಸರುಗಳು);
    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
Copy_Led_ಹೆಸರುಗಳು_ಗಮ್ಯ_Keymap(ರಚನೆ xkb_keymap *keymap, ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info)
{
    keymap->ಅಂಕೆ_leds = info->ಅಂಕೆ_led_ಹೆಸರುಗಳು;
    ಗಾಗಿ (xkb_led_ಸೂಚ್ಯಂಕ_t idx = 0; idx < info->ಅಂಕೆ_led_ಹೆಸರುಗಳು; idx++) {
        LedNameInfo *ledi = &info->led_ಹೆಸರುಗಳು[idx];

        ಆದರೆ (ledi->name == XKB_ATOM_NONE)
            ಮುಂದುವರಿ;

        keymap->leds[idx].name = ledi->name;
    }

    ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ bool
Copy_ಕೀಲಿ_ಹೆಸರುಗಳು_Info_ಗಮ್ಯ_Keymap(ರಚನೆ xkb_keymap *keymap, ಕೀಲಿ_ಹೆಸರುಗಳು_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    ಆದರೆ (!Copy_ಕೀಲಿ_ಹೆಸರುಗಳು_ಗಮ್ಯ_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !Copy_Led_ಹೆಸರುಗಳು_ಗಮ್ಯ_Keymap(keymap, info))
        ಹಿಂತಿರುಗಿ false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    ಆದರೆ (!keymap->ಅಂಕೆ_ಕೀಲಿಗಳು ||
        keymap->min_ಕೀಲಿ_ಸಂಕೇತ > 0) {
        keymap->redirect_ಕೀಲಿ_auto = 0;
    } ಇಲ್ಲದಿದ್ದರೆ {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        ಗಾಗಿ (xkb_keycode_t k = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು; k-- > keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ;) {
            ಆದರೆ (keycode > keymap->ಕೀಲಿಗಳು[k].keycode)
                ನಿಲ್ಲಿಸು;
            /* Try next keycode downwards */
            keycode = keymap->ಕೀಲಿಗಳು[k].keycode - 1;
        }
        keymap->redirect_ಕೀಲಿ_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_ಸುರಕ್ಷಿತ(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    ಹಿಂತಿರುಗಿ true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, ರಚನೆ xkb_keymap_info *keymap_info)
{
    ಕೀಲಿ_ಹೆಸರುಗಳು_Info info;

    Init_ಕೀಲಿ_ಹೆಸರುಗಳು_Info(&info, keymap_info, 0);

    ಆದರೆ (file != ಖಾಲಿ)
        HandleKeycodesFile(&info, file);

    ಆದರೆ (info.error_ಎಣಿಕೆ != 0)
        ಹೋಗು err_info;

    ಆದರೆ (!Copy_ಕೀಲಿ_ಹೆಸರುಗಳು_Info_ಗಮ್ಯ_Keymap(&keymap_info->keymap, &info))
        ಹೋಗು err_info;

    Clear_ಕೀಲಿ_ಹೆಸರುಗಳು_Info(&info);
    ಹಿಂತಿರುಗಿ true;

err_info:
    Clear_ಕೀಲಿ_ಹೆಸರುಗಳು_Info(&info);
    ಹಿಂತಿರುಗಿ false;
}
