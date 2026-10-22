/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "expr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "include.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-mem.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} ਉੱਚਾ_Keycode_ਇੰਦਰਾਜ;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) ਨੀਵਾਂ;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(ਉੱਚਾ_Keycode_ਇੰਦਰਾਜ) ਉੱਚਾ;
    /**
     * name -> keycode mapping
     */
    darray(Keycode_ਮੇਲ) ਨਾਂ;
} KeycodeStore;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->ਨੀਵਾਂ);
    darray_init(store->ਉੱਚਾ);
    darray_init(store->ਨਾਂ);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->ਨੀਵਾਂ);
    darray_free(store->ਉੱਚਾ);
    darray_free(store->ਨਾਂ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
keycode_store_update_ਕੁੰਜੀ(KeycodeStore *store, Keycode_ਮੇਲ ਮੇਲ, xkb_atom_t name)
{
    ਜੇ (unlikely(!ਮੇਲ.ਮਿਲਿਆ || ਮੇਲ.is_alias)) {
        ਵਾਪਸ;
    } ਨਹੀਂਤਾਂ ਜੇ (ਮੇਲ.ਕੁੰਜੀ.ਨੀਵਾਂ) {
        /* assume valid index */
        ਪ੍ਰਤਿਜ੍ਞਾ(ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ < darray_ਮਾਪ(store->ਨੀਵਾਂ));
        darray_ਵਸਤੂ(store->ਨੀਵਾਂ, ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ) = name;
    } ਨਹੀਂਤਾਂ {
        /* assume valid index */
        ਪ੍ਰਤਿਜ੍ਞਾ(ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ < darray_ਮਾਪ(store->ਉੱਚਾ));
        darray_ਵਸਤੂ(store->ਉੱਚਾ, ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ).name = name;
    }

    /* Names lookup */
    ਜੇ (name >= darray_ਮਾਪ(store->ਨਾਂ)) {
        darray_resize0(store->ਨਾਂ, name + 1);
    }
    darray_ਵਸਤੂ(store->ਨਾਂ, name) = ਮੇਲ;
}

ਸਥਿਰ bool
keycode_store_ਪਾਓ_ਕੁੰਜੀ(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    ਜੇ (name >= darray_ਮਾਪ(store->ਨਾਂ)) {
        darray_resize0(store->ਨਾਂ, name + 1);
    }
    ਜੇ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        ਜੇ (kc >= (xkb_keycode_t) darray_ਮਾਪ(store->ਨੀਵਾਂ))
            darray_resize0(store->ਨੀਵਾਂ, kc + 1);
        darray_ਵਸਤੂ(store->ਨੀਵਾਂ, kc) = name;
        ਜੇ (kc < store->min)
            store->min = kc;
        darray_ਵਸਤੂ(store->ਨਾਂ, name) = (Keycode_ਮੇਲ) {
            .ਕੁੰਜੀ = {
                .ਮਿਲਿਆ = true,
                .ਨੀਵਾਂ = true,
                .is_alias = false,
                .ਸੂਚਕਾਂਕ = kc
            }
        };
    } ਨਹੀਂਤਾਂ {
        /* High keycode: insert into a sorted list */
        ਅਚਲ darray_ਮਾਪ_t idx = darray_ਮਾਪ(store->ਉੱਚਾ);
        ਜੇ (idx && darray_ਵਸਤੂ(store->ਉੱਚਾ, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_ਮਾਪ_t ਹੇਠਲਾ = 0;
            darray_ਮਾਪ_t ਉੱਤਲਾ = idx;
            ਜਦੋਂ (ਹੇਠਲਾ < ਉੱਤਲਾ) {
                ਅਚਲ darray_ਮਾਪ_t mid = ਹੇਠਲਾ + (ਉੱਤਲਾ - 1 - ਹੇਠਲਾ) / 2;
                ਅਚਲ ਉੱਚਾ_Keycode_ਇੰਦਰਾਜ * ਅਚਲ ਇੰਦਰਾਜ =
                    &darray_ਵਸਤੂ(store->ਉੱਚਾ, mid);
                ਜੇ (ਇੰਦਰਾਜ->keycode < kc) {
                    ਹੇਠਲਾ = mid + 1;
                } ਨਹੀਂਤਾਂ ਜੇ (ਇੰਦਰਾਜ->keycode > kc) {
                    ਉੱਤਲਾ = mid;
                } ਨਹੀਂਤਾਂ {
                    /* Unreachable: there is no repetition in the list */
                    ਪ੍ਰਤਿਜ੍ਞਾ(ਇੰਦਰਾਜ->keycode != kc);
                }
            }
            ਪ੍ਰਤਿਜ੍ਞਾ(ਹੇਠਲਾ < idx);
            ਪ੍ਰਤਿਜ੍ਞਾ(darray_ਵਸਤੂ(store->ਉੱਚਾ, ਹੇਠਲਾ).keycode > kc);

            /* Update references to entries that will be moved */
            ਉੱਚਾ_Keycode_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
            darray_foreach_from(ਇੰਦਰਾਜ, store->ਉੱਚਾ, ਹੇਠਲਾ) {
                darray_ਵਸਤੂ(store->ਨਾਂ, ਇੰਦਰਾਜ->name).ਕੁੰਜੀ.ਸੂਚਕਾਂਕ++;
            }

            darray_ਪਾਓ(store->ਉੱਚਾ, ਹੇਠਲਾ,
                          (ਉੱਚਾ_Keycode_ਇੰਦਰਾਜ){.keycode = kc, .name = name});
            darray_ਵਸਤੂ(store->ਨਾਂ, name) = (Keycode_ਮੇਲ) {
                .ਕੁੰਜੀ = {
                    .ਮਿਲਿਆ = true,
                    .ਨੀਵਾਂ = false,
                    .is_alias = false,
                    .ਸੂਚਕਾਂਕ = ਹੇਠਲਾ
                }
            };
        } ਨਹੀਂਤਾਂ {
            /* Fast path: no need to sort */
            darray_append(store->ਉੱਚਾ,
                          (ਉੱਚਾ_Keycode_ਇੰਦਰਾਜ){.keycode = kc, .name = name});
            darray_ਵਸਤੂ(store->ਨਾਂ, name) = (Keycode_ਮੇਲ) {
                .ਕੁੰਜੀ = {
                    .ਮਿਲਿਆ = true,
                    .ਨੀਵਾਂ = false,
                    .is_alias = false,
                    .ਸੂਚਕਾਂਕ = idx
                }
            };
        }
        ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(store->ਨੀਵਾਂ))
            store->min = darray_ਵਸਤੂ(store->ਉੱਚਾ, 0).keycode;
    }
    ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
keycode_store_ਪਾਓ_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    ਜੇ (alias >= darray_ਮਾਪ(store->ਨਾਂ)) {
        darray_resize0(store->ਨਾਂ, alias + 1);
    }
    darray_ਵਸਤੂ(store->ਨਾਂ, alias) = (Keycode_ਮੇਲ) {
        .alias = {
            .ਮਿਲਿਆ = true,
            .is_alias = true,
            .real = real
        }
    };
    ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_ਵਸਤੂ(store->ਨਾਂ, alias).alias.real = real;
    ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
keycode_store_delete_name(ਅਚਲ KeycodeStore *store, xkb_atom_t name)
{
    ਜੇ (name < darray_ਮਾਪ(store->ਨਾਂ))
        darray_ਵਸਤੂ(store->ਨਾਂ, name).ਮਿਲਿਆ = false;
}

ਸਥਿਰ ਖਾਲੀ
keycode_store_delete_ਕੁੰਜੀ(KeycodeStore *store, ਅਚਲ Keycode_ਮੇਲ ਮੇਲ)
{
    ਜੇ (unlikely(!ਮੇਲ.ਮਿਲਿਆ || ਮੇਲ.is_alias)) {
        ਵਾਪਸ;
    } ਨਹੀਂਤਾਂ ਜੇ (ਮੇਲ.ਕੁੰਜੀ.ਨੀਵਾਂ) {
        /* assume valid index */
        ਪ੍ਰਤਿਜ੍ਞਾ(ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ < darray_ਮਾਪ(store->ਨੀਵਾਂ));
        darray_ਵਸਤੂ(store->ਨਾਂ,
                    darray_ਵਸਤੂ(store->ਨੀਵਾਂ, ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ)).ਮਿਲਿਆ = false;
        ਜੇ (ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ + 1u == darray_ਮਾਪ(store->ਨੀਵਾਂ)) {
            /* Highest low keycode: shrink */
            ਜੇ (store->min == ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ) {
                /* No low keycode left */
                darray_ਮਾਪ(store->ਨੀਵਾਂ) = 0;
            } ਨਹੀਂਤਾਂ {
                /* Look for previous defined low keycode */
                ਲਈ (darray_ਮਾਪ_t idx = ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ; idx > 0; idx--) {
                    ਜੇ (darray_ਵਸਤੂ(store->ਨੀਵਾਂ, idx - 1) != XKB_ATOM_NONE) {
                        darray_ਮਾਪ(store->ਨੀਵਾਂ) = idx;
                        ਰੋਕੋ;
                    }
                }
            }
        } ਨਹੀਂਤਾਂ {
            /* Lower keycode: reset */
            darray_ਵਸਤੂ(store->ਨੀਵਾਂ, ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ) = XKB_ATOM_NONE;
        }
    } ਨਹੀਂਤਾਂ {
        ਪ੍ਰਤਿਜ੍ਞਾ(ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ < darray_ਮਾਪ(store->ਉੱਚਾ)); /* assume valid index */
        darray_ਵਸਤੂ(store->ਨਾਂ,
                    darray_ਵਸਤੂ(store->ਉੱਚਾ, ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ).name).ਮਿਲਿਆ = false;
        darray_remove(store->ਉੱਚਾ, ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ);
        /* Update LUT indexes of high codes after the deleted one, if any */
        Keycode_ਮੇਲ *ਇੰਦਰਾਜ;
        darray_foreach(ਇੰਦਰਾਜ, store->ਨਾਂ) {
            ਜੇ (ਇੰਦਰਾਜ->ਮਿਲਿਆ && !ਇੰਦਰਾਜ->is_alias && !ਇੰਦਰਾਜ->ਕੁੰਜੀ.ਨੀਵਾਂ &&
                ਇੰਦਰਾਜ->ਕੁੰਜੀ.ਸੂਚਕਾਂਕ > ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ) {
                ਇੰਦਰਾਜ->ਕੁੰਜੀ.ਸੂਚਕਾਂਕ--;
            }
        }
    }

    /* Update bounds */
    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(store->ਨੀਵਾਂ)) {
        store->min = (darray_ਖਾਲੀ_ਥਾਵਾਂ(store->ਉੱਚਾ))
            ? XKB_KEYCODE_INVALID
            : darray_ਵਸਤੂ(store->ਉੱਚਾ, 0).keycode;
    } ਨਹੀਂਤਾਂ {
        ਲਈ (xkb_keycode_t kc = store->min; kc < darray_ਮਾਪ(store->ਨੀਵਾਂ); kc++) {
            ਜੇ (darray_ਵਸਤੂ(store->ਨੀਵਾਂ, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                ਰੋਕੋ;
            }
        }
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_keycode_t
keycode_store_get_keycode(ਅਚਲ KeycodeStore *store, Keycode_ਮੇਲ ਮੇਲ)
{
    ਜੇ (!ਮੇਲ.ਮਿਲਿਆ || ਮੇਲ.is_alias) {
        ਵਾਪਸ XKB_KEYCODE_INVALID;
    } ਨਹੀਂਤਾਂ ਜੇ (ਮੇਲ.ਕੁੰਜੀ.ਨੀਵਾਂ) {
        /* assume valid index */
        ਪ੍ਰਤਿਜ੍ਞਾ(ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ < darray_ਮਾਪ(store->ਨੀਵਾਂ));
        ਵਾਪਸ (xkb_keycode_t) ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ;
    } ਨਹੀਂਤਾਂ {
        /* assume valid index */
        ਪ੍ਰਤਿਜ੍ਞਾ(ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ < darray_ਮਾਪ(store->ਉੱਚਾ));
        ਵਾਪਸ darray_ਵਸਤੂ(store->ਉੱਚਾ, ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ).keycode;
    }
}

ਸਥਿਰ ਸਤਰਅੰਦਰ xkb_atom_t
keycode_store_get_ਕੁੰਜੀ_name(ਅਚਲ KeycodeStore *store, Keycode_ਮੇਲ ਮੇਲ)
{
    ਜੇ (!ਮੇਲ.ਮਿਲਿਆ || ਮੇਲ.is_alias) {
        ਵਾਪਸ XKB_ATOM_NONE;
    } ਨਹੀਂਤਾਂ ਜੇ (ਮੇਲ.ਕੁੰਜੀ.ਨੀਵਾਂ) {
        /* assume valid index */
        ਪ੍ਰਤਿਜ੍ਞਾ(ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ < darray_ਮਾਪ(store->ਨੀਵਾਂ));
        ਵਾਪਸ darray_ਵਸਤੂ(store->ਨੀਵਾਂ, ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ);
    } ਨਹੀਂਤਾਂ {
        /* assume valid index */
        ਪ੍ਰਤਿਜ੍ਞਾ(ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ < darray_ਮਾਪ(store->ਉੱਚਾ));
        ਵਾਪਸ darray_ਵਸਤੂ(store->ਉੱਚਾ, ਮੇਲ.ਕੁੰਜੀ.ਸੂਚਕਾਂਕ).name;
    }
}

ਸਥਿਰ Keycode_ਮੇਲ
keycode_store_lookup_keycode(ਅਚਲ KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    ਜੇ (kc < (xkb_keycode_t) darray_ਮਾਪ(store->ਨੀਵਾਂ)) {
        ਵਾਪਸ (Keycode_ਮੇਲ) {
            .ਕੁੰਜੀ = {
                .ਮਿਲਿਆ = true,
                .ਨੀਵਾਂ = true,
                .is_alias = false,
                .ਸੂਚਕਾਂਕ = kc
            }
        };
    } ਨਹੀਂਤਾਂ ਜੇ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        ਵਾਪਸ (Keycode_ਮੇਲ) { .ਮਿਲਿਆ = false };
    }

    /* High keycodes: use binary search */
    darray_ਮਾਪ_t ਹੇਠਲਾ = 0;
    darray_ਮਾਪ_t ਉੱਤਲਾ = darray_ਮਾਪ(store->ਉੱਚਾ);
    ਜਦੋਂ (ਹੇਠਲਾ < ਉੱਤਲਾ) {
        ਅਚਲ darray_ਮਾਪ_t mid = ਹੇਠਲਾ + (ਉੱਤਲਾ - 1 - ਹੇਠਲਾ) / 2;
        ਉੱਚਾ_Keycode_ਇੰਦਰਾਜ * ਅਚਲ ਇੰਦਰਾਜ = &darray_ਵਸਤੂ(store->ਉੱਚਾ, mid);
        ਜੇ (ਇੰਦਰਾਜ->keycode < kc) {
            ਹੇਠਲਾ = mid + 1;
        } ਨਹੀਂਤਾਂ ਜੇ (ਇੰਦਰਾਜ->keycode > kc) {
            ਉੱਤਲਾ = mid;
        } ਨਹੀਂਤਾਂ {
            ਵਾਪਸ (Keycode_ਮੇਲ) {
                .ਕੁੰਜੀ = {
                    .ਮਿਲਿਆ = true,
                    .ਨੀਵਾਂ = false,
                    .is_alias = false,
                    .ਸੂਚਕਾਂਕ = mid
                }
            };
        }
    }

    ਵਾਪਸ (Keycode_ਮੇਲ) { .ਮਿਲਿਆ = false };
}

ਸਥਿਰ Keycode_ਮੇਲ
keycode_store_lookup_name(ਅਚਲ KeycodeStore *store, xkb_atom_t name)
{
    ਜੇ (name >= darray_ਮਾਪ(store->ਨਾਂ)) {
        ਵਾਪਸ (Keycode_ਮੇਲ) { .ਮਿਲਿਆ = false, .is_alias = false };
    } ਨਹੀਂਤਾਂ {
        ਵਾਪਸ darray_ਵਸਤੂ(store->ਨਾਂ, name);
    }
}

/***====================================================================***/

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ;

    xkb_atom_t name;
} LedNameInfo;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਅੱਖਰ *name;
    ਪੂਰਨ error_ਗਿਣਤੀ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ;

    KeycodeStore keycodes;
    LedNameInfo led_ਨਾਂ[XKB_MAX_LEDS];
    xkb_led_ਸੂਚਕਾਂਕ_t ਅੰਕ_led_ਨਾਂ;

    ਬਣਤਰ xkb_context *ctx;
    ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info;
} ਕੁੰਜੀ_ਨਾਂ_Info;

/***====================================================================***/

ਸਥਿਰ LedNameInfo *
ਲੱਭੋ_Led_By_Name(ਕੁੰਜੀ_ਨਾਂ_Info *info, xkb_atom_t name,
              xkb_led_ਸੂਚਕਾਂਕ_t *idx_ਬਾਹਰਲਾ)
{
    ਲਈ (xkb_led_ਸੂਚਕਾਂਕ_t idx = 0; idx < info->ਅੰਕ_led_ਨਾਂ; idx++) {
        LedNameInfo *ledi = &info->led_ਨਾਂ[idx];
        ਜੇ (ledi->name == name) {
            *idx_ਬਾਹਰਲਾ = idx;
            ਵਾਪਸ ledi;
        }
    }

    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ bool
ਜੋੜੋ_Led_Name(ਕੁੰਜੀ_ਨਾਂ_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_ਸੂਚਕਾਂਕ_t new_idx, bool report)
{
    xkb_led_ਸੂਚਕਾਂਕ_t old_idx;
    LedNameInfo *old;
    ਅਚਲ bool replace = (new->ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = ਲੱਭੋ_Led_By_Name(info, new->name, &old_idx);
    ਜੇ (old) {
        ਜੇ (old_idx == new_idx) {
            ਜੇ (report)
                log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_ਪਾਠ(info->ctx, new->name));
            ਵਾਪਸ true;
        }

        ਜੇ (report) {
            xkb_led_ਸੂਚਕਾਂਕ_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_ਸੂਚਕਾਂਕ_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_ਪਾਠ(info->ctx, new->name), use, ignore);
        }

        ਜੇ (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } ਨਹੀਂਤਾਂ {
            ਵਾਪਸ true;
        }
    }

    ਜੇ (new_idx >= info->ਅੰਕ_led_ਨਾਂ)
        info->ਅੰਕ_led_ਨਾਂ = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_ਨਾਂ[new_idx];
    ਜੇ (old->name != XKB_ATOM_NONE) {
        ਜੇ (report) {
            ਅਚਲ xkb_atom_t use    = (replace ? new->name : old->name);
            ਅਚਲ xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_ਪਾਠ(info->ctx, use),
                     xkb_atom_ਪਾਠ(info->ctx, ignore));
        }

        ਜੇ (replace)
            *old = *new;

        ਵਾਪਸ true;
    }

    *old = *new;
    ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ
Clear_ਕੁੰਜੀ_ਨਾਂ_Info(ਕੁੰਜੀ_ਨਾਂ_Info *info)
{
    ਮੁਕਤ(info->name);
    keycode_store_free(&info->keycodes);
}

ਸਥਿਰ ਖਾਲੀ
Init_ਕੁੰਜੀ_ਨਾਂ_Info(ਕੁੰਜੀ_ਨਾਂ_Info *info, ਅਚਲ ਬਣਤਰ xkb_keymap_info *keymap_info,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ include_ਡੂੰਘਾਈ)
{
    memset(info, 0, ਆਕਾਰ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_ਡੂੰਘਾਈ = include_ਡੂੰਘਾਈ;
    keycode_store_init(&info->keycodes);
}

ਸਥਿਰ bool
ਜੋੜੋ_ਕੁੰਜੀ_Name(ਕੁੰਜੀ_ਨਾਂ_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ, bool report)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NDEBUG
    ਅਚਲ
#ਜੇ_ਅੰਤ
    Keycode_ਮੇਲ ਮੇਲ_name = keycode_store_lookup_name(&info->keycodes, name);
    ਜੇ (ਮੇਲ_name.ਮਿਲਿਆ) {
        ਅਚਲ bool clobber = (ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);

        ਜੇ (ਮੇਲ_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            ਜੇ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            ਜੇ (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NDEBUG
                ਮੇਲ_name.ਮਿਲਿਆ = false;
#ਜੇ_ਅੰਤ
            } ਨਹੀਂਤਾਂ {
                ਵਾਪਸ true;
            }
        } ਨਹੀਂਤਾਂ {
            ਅਚਲ xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, ਮੇਲ_name);
            ਪ੍ਰਤਿਜ੍ਞਾ(old_kc != XKB_KEYCODE_INVALID);
            ਜੇ (old_kc != kc) {
                /* There is already a different key with this name. */

                ਜੇ (report) {
                    ਅਚਲ xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ਅਚਲ xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, name), use, ignore);
                }

                ਜੇ (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_ਕੁੰਜੀ(&info->keycodes, ਮੇਲ_name);
                } ਨਹੀਂਤਾਂ {
                    ਵਾਪਸ true;
                }
            }
        }
    }

    ਅਚਲ Keycode_ਮੇਲ ਮੇਲ_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ਅਚਲ xkb_atom_t old_name = keycode_store_get_ਕੁੰਜੀ_name(&info->keycodes, ਮੇਲ_kc);
    ਜੇ (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        ਜੇ (old_name == name) {
            ਪ੍ਰਤਿਜ੍ਞਾ(keycode_store_get_keycode(&info->keycodes, ਮੇਲ_name) == kc);
            ਜੇ (report)
                log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, old_name), kc);
            ਵਾਪਸ true;
        }

        ਅਚਲ bool clobber = (ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);
        ਜੇ (report) {
            ਅਚਲ ਅੱਖਰ* ਅਚਲ kname     = ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, name);
            ਅਚਲ ਅੱਖਰ* ਅਚਲ old_kname = ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, old_name);
            ਅਚਲ ਅੱਖਰ* ਅਚਲ use    = (clobber) ? kname : old_kname;
            ਅਚਲ ਅੱਖਰ* ਅਚਲ ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        ਜੇ (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_ਕੁੰਜੀ(&info->keycodes, ਮੇਲ_kc, name);
        }
    } ਨਹੀਂਤਾਂ {
        /* No previous keycode */
        ਜੇ (!keycode_store_ਪਾਓ_ਕੁੰਜੀ(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
                    "Cannot add keycode\n");
            ਵਾਪਸ false;
        }
    }

    ਵਾਪਸ true;
}

/***====================================================================***/

ਸਥਿਰ bool
HandleAliasDef(ਕੁੰਜੀ_ਨਾਂ_Info *info, ਅਚਲ ਕੁੰਜੀ_Alias_Def *def, bool report);

ਸਥਿਰ ਖਾਲੀ
ਮਿਲਾਓ_Keycode_Stores(ਕੁੰਜੀ_ਨਾਂ_Info *into, ਕੁੰਜੀ_ਨਾਂ_Info *from,
                   ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ, bool report)
{
    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(into->keycodes.ਨੀਵਾਂ) && darray_ਖਾਲੀ_ਥਾਵਾਂ(into->keycodes.ਉੱਚਾ) &&
        darray_ਖਾਲੀ_ਥਾਵਾਂ(into->keycodes.ਨਾਂ)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.ਨੀਵਾਂ);
        darray_init(from->keycodes.ਉੱਚਾ);
        darray_init(from->keycodes.ਨਾਂ);
    } ਨਹੀਂਤਾਂ {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        ਲਈ (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_ਮਾਪ(from->keycodes.ਨੀਵਾਂ);
             kc++) {
            /* Skip undefined keycodes */
            ਅਚਲ xkb_atom_t name = darray_ਵਸਤੂ(from->keycodes.ਨੀਵਾਂ, kc);
            ਜੇ (name == XKB_ATOM_NONE)
                ਜਾਰੀ;

            ਜੇ (!ਜੋੜੋ_ਕੁੰਜੀ_Name(into, kc, name, ਮਿਲਾਓ, report))
                into->error_ਗਿਣਤੀ++;
        }

        /* High keycodes */
        ਅਚਲ ਉੱਚਾ_Keycode_ਇੰਦਰਾਜ *new;
        darray_foreach(new, from->keycodes.ਉੱਚਾ) {
            ਪ੍ਰਤਿਜ੍ਞਾ(new->name != XKB_ATOM_NONE);
            ਜੇ (!ਜੋੜੋ_ਕੁੰਜੀ_Name(into, new->keycode, new->name, ਮਿਲਾਓ, report))
                into->error_ਗਿਣਤੀ++;
        }

        /* Aliases. */
        Keycode_ਮੇਲ *ਮੇਲ;
        xkb_atom_t alias;
        darray_enumerate(alias, ਮੇਲ, from->keycodes.ਨਾਂ) {
            ਜੇ (!ਮੇਲ->ਮਿਲਿਆ || !ਮੇਲ->is_alias)
                ਜਾਰੀ;

            ਅਚਲ ਕੁੰਜੀ_Alias_Def def = {
                .ਮਿਲਾਓ = ਮਿਲਾਓ,
                .alias = alias,
                .real = ਮੇਲ->alias.real
            };
            ਜੇ (!HandleAliasDef(into, &def, report))
                into->error_ਗਿਣਤੀ++;
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
ਮਿਲਾਓ_Included_Keycodes(ਕੁੰਜੀ_ਨਾਂ_Info *into, ਕੁੰਜੀ_ਨਾਂ_Info *from,
                      ਗਿਣਤੀ ਮਿਲਾਓ_ਬਹੁਲਕ ਮਿਲਾਓ, bool report)
{
    ਜੇ (from->error_ਗਿਣਤੀ > 0) {
        into->error_ਗਿਣਤੀ += from->error_ਗਿਣਤੀ;
        ਵਾਪਸ;
    }

    ਜੇ (into->name == ਨਿਰਾ) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    ਮਿਲਾਓ_Keycode_Stores(into, from, ਮਿਲਾਓ, report);

    /* Merge LED names. */
    ਜੇ (into->ਅੰਕ_led_ਨਾਂ == 0) {
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(into->led_ਨਾਂ, from->led_ਨਾਂ,
               ਆਕਾਰ(*from->led_ਨਾਂ) * from->ਅੰਕ_led_ਨਾਂ);
        into->ਅੰਕ_led_ਨਾਂ = from->ਅੰਕ_led_ਨਾਂ;
        from->ਅੰਕ_led_ਨਾਂ = 0;
    }
    ਨਹੀਂਤਾਂ {
        ਲਈ (xkb_led_ਸੂਚਕਾਂਕ_t idx = 0; idx < from->ਅੰਕ_led_ਨਾਂ; idx++) {
            LedNameInfo *ledi = &from->led_ਨਾਂ[idx];

            ਜੇ (ledi->name == XKB_ATOM_NONE)
                ਜਾਰੀ;

            ledi->ਮਿਲਾਓ = ਮਿਲਾਓ;
            ਜੇ (!ਜੋੜੋ_Led_Name(into, false, ledi, idx, report))
                into->error_ਗਿਣਤੀ++;
        }
    }
}

ਸਥਿਰ ਖਾਲੀ
HandleKeycodesFile(ਕੁੰਜੀ_ਨਾਂ_Info *info, XkbFile *file);

ਸਥਿਰ bool
HandleIncludeKeycodes(ਕੁੰਜੀ_ਨਾਂ_Info *info, IncludeStmt *include, bool report)
{
    ਕੁੰਜੀ_ਨਾਂ_Info included;

    ਜੇ (Exceeds_Include_Max_ਡੂੰਘਾਈ(info->ctx, info->include_ਡੂੰਘਾਈ)) {
        info->error_ਗਿਣਤੀ += 10;
        ਵਾਪਸ false;
    }

    Init_ਕੁੰਜੀ_ਨਾਂ_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ਅਚਲ ਬਣਤਰ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ਲਈ (IncludeStmt *stmt = include; stmt; stmt = stmt->ਅਗਲਾ_incl) {
        ਕੁੰਜੀ_ਨਾਂ_Info ਅਗਲਾ_incl;
        XkbFile *file;

        ਅੱਖਰ ਰਸਤਾ[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  ਰਸਤਾ, ਆਕਾਰ(ਰਸਤਾ));
        ਜੇ (!file) {
            info->error_ਗਿਣਤੀ += 10;
            Clear_ਕੁੰਜੀ_ਨਾਂ_Info(&included);
            ਵਾਪਸ false;
        }

        Init_ਕੁੰਜੀ_ਨਾਂ_Info(&ਅਗਲਾ_incl, info->keymap_info, info->include_ਡੂੰਘਾਈ + 1);

        HandleKeycodesFile(&ਅਗਲਾ_incl, file);

        ਮਿਲਾਓ_Included_Keycodes(&included, &ਅਗਲਾ_incl, stmt->ਮਿਲਾਓ, report);

        Clear_ਕੁੰਜੀ_ਨਾਂ_Info(&ਅਗਲਾ_incl);
        FreeXkbFile(file);
    }

    ਮਿਲਾਓ_Included_Keycodes(info, &included, include->ਮਿਲਾਓ, report);
    Clear_ਕੁੰਜੀ_ਨਾਂ_Info(&included);

    ਵਾਪਸ (info->error_ਗਿਣਤੀ == 0);
}

ਸਥਿਰ bool
HandleKeycodeDef(ਕੁੰਜੀ_ਨਾਂ_Info *info, KeycodeDef *stmt, bool report)
{
    ਜੇ (stmt->ਮੁੱਲ < 0 || stmt->ਮੁੱਲ > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->ਮੁੱਲ, XKB_KEYCODE_MAX);
        ਵਾਪਸ false;
    }

    ਵਾਪਸ ਜੋੜੋ_ਕੁੰਜੀ_Name(info, (xkb_keycode_t) stmt->ਮੁੱਲ,
                      stmt->name, stmt->ਮਿਲਾਓ, report);
}

ਸਥਿਰ bool
HandleAliasDef(ਕੁੰਜੀ_ਨਾਂ_Info *info, ਅਚਲ ਕੁੰਜੀ_Alias_Def *def, bool report)
{
    ਅਚਲ Keycode_ਮੇਲ ਮੇਲ_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    ਜੇ (ਮੇਲ_name.ਮਿਲਿਆ) {
        ਅਚਲ bool clobber = (def->ਮਿਲਾਓ != ਮਿਲਾਓ_AUGMENT);
        ਜੇ (ਮੇਲ_name.is_alias) {
            ਜੇ (def->real == ਮੇਲ_name.alias.real) {
                ਜੇ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, def->alias),
                             ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, def->real));
            } ਨਹੀਂਤਾਂ {
                ਅਚਲ xkb_atom_t use = (clobber)
                    ? def->real
                    : ਮੇਲ_name.alias.real;
                ਅਚਲ xkb_atom_t ignore = (clobber)
                    ? ਮੇਲ_name.alias.real
                    : def->real;

                ਜੇ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, def->alias),
                             ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, use),
                             ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            ਵਾਪਸ true;
        } ਨਹੀਂਤਾਂ {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            ਜੇ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ਕੁੰਜੀ_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            ਜੇ (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_ਕੁੰਜੀ(&info->keycodes, ਮੇਲ_name);
            } ਨਹੀਂਤਾਂ {
                ਵਾਪਸ true;
            }
        }
    }

    ਵਾਪਸ keycode_store_ਪਾਓ_alias(&info->keycodes, def->alias, def->real);
}

ਸਥਿਰ bool
Handle_ਕੁੰਜੀ_Name_Var(ਕੁੰਜੀ_ਨਾਂ_Info *info, VarDef *stmt)
{
    ਅਚਲ ਅੱਖਰ *elem, *ਖੇਤਰ_2;
    ਵਿਅੰਜਕ_Def *arrayNdx;

    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Lhs(info->ctx, stmt->name, &elem, &ਖੇਤਰ_2, &arrayNdx))
        ਵਾਪਸ false;

    ਜੇ (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, ਖੇਤਰ_2);
        ਵਾਪਸ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ਜੇ (!istreq(ਖੇਤਰ_2, "minimum") && !istreq(ਖੇਤਰ_2, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ਖੇਤਰ,
                "Default defined for unknown field \"%s\"; Ignored\n", ਖੇਤਰ_2);
        ਵਾਪਸ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ਜੇ (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", ਖੇਤਰ_2, "defaults");
        ਵਾਪਸ !(info->keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH);
    }

    int64_t val = 0;
    ਜੇ (!ਵਿਅੰਜਕ_Resolve_Integer(info->ctx, stmt->ਮੁੱਲ, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE, "keycodes",
                      ਖੇਤਰ_2, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        ਵਾਪਸ !(info->keymap_info->strict & PARSER_NO_ਖੇਤਰ_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
HandleLedNameDef(ਕੁੰਜੀ_ਨਾਂ_Info *info, LedNameDef *def, bool report)
{
    ਜੇ (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_ਗਿਣਤੀ++;
        log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        ਵਾਪਸ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ਜੇ (!ਵਿਅੰਜਕ_Resolve_String(info->ctx, def->name, &name)) {
        ਅੱਖਰ buf[20];
        snprintf(buf, ਆਕਾਰ(buf), "%"PRId64, def->ndx);
        info->error_ਗਿਣਤੀ++;
        ਵਾਪਸ ReportBadType(info->ctx, XKB_ERROR_WRONG_ਖੇਤਰ_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.ਮਿਲਾਓ = def->ਮਿਲਾਓ, .name = name};
    ਵਾਪਸ ਜੋੜੋ_Led_Name(info, true, &ledi, (xkb_led_ਸੂਚਕਾਂਕ_t) def->ndx - 1, report);
}

ਸਥਿਰ ਖਾਲੀ
HandleKeycodesFile(ਕੁੰਜੀ_ਨਾਂ_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ਅਚਲ ਪੂਰਨ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ਅਚਲ bool report_same_file = verbosity > 0;
    ਅਚਲ bool report_include   = verbosity > 7;

    ਮੁਕਤ(info->name);
    info->name = strdup_ਸੁਰੱਖਿਅਤ(file->name);

    ਲਈ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ਅਗਲਾ) {
        ਚੋਣ (stmt->type) {
        ਹਾਲਤ STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_ALIAS:
            ok = HandleAliasDef(info, (ਕੁੰਜੀ_Alias_Def *) stmt, report_same_file);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_VAR:
            ok = Handle_ਕੁੰਜੀ_Name_Var(info, (VarDef *) stmt);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            ਰੋਕੋ;
        ਹਾਲਤ STMT_UNKNOWN_DECLARATION:
        ਹਾਲਤ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ਰੋਕੋ;
        ਮੂਲ:
            log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_ਮੰਜ਼ਿਲ_string(stmt->type));
            ok = false;
            ਰੋਕੋ;
        }

        ਜੇ (!ok)
            info->error_ਗਿਣਤੀ++;

        ਜੇ (info->error_ਗਿਣਤੀ > 10) {
            log_err(info->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    ਸੁਰੱਖਿਅਤ_map_name(file->name));
            ਰੋਕੋ;
        }
    }
}

/***====================================================================***/

ਸਥਿਰ bool
Copy_ਕੁੰਜੀ_ਨਾਂ_ਮੰਜ਼ਿਲ_Keymap(ਬਣਤਰ xkb_keymap *keymap, ਕੁੰਜੀ_ਨਾਂ_Info *info)
{
    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(info->keycodes.ਨੀਵਾਂ) && darray_ਖਾਲੀ_ਥਾਵਾਂ(info->keycodes.ਉੱਚਾ)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        ਪ੍ਰਤਿਜ੍ਞਾ(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_ਕੁੰਜੀ_ਕੋਡ = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_ਕੁੰਜੀ_ਕੋਡ = 255;
        keymap->ਅੰਕ_ਕੁੰਜੀਆਂ = keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ = keymap->max_ਕੁੰਜੀ_ਕੋਡ + 1;
    } ਨਹੀਂਤਾਂ {
        ਪ੍ਰਤਿਜ੍ਞਾ(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_ਕੁੰਜੀ_ਕੋਡ = info->keycodes.min;
        keymap->max_ਕੁੰਜੀ_ਕੋਡ = (darray_ਖਾਲੀ_ਥਾਵਾਂ(info->keycodes.ਉੱਚਾ))
            ? darray_ਮਾਪ(info->keycodes.ਨੀਵਾਂ) - 1
            : darray_ਵਸਤੂ(
                info->keycodes.ਉੱਚਾ,
                darray_ਮਾਪ(info->keycodes.ਉੱਚਾ) - 1
            ).keycode;
        keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ = darray_ਮਾਪ(info->keycodes.ਨੀਵਾਂ);
        keymap->ਅੰਕ_ਕੁੰਜੀਆਂ = keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ
                         + darray_ਮਾਪ(info->keycodes.ਉੱਚਾ);
    }

    ਬਣਤਰ xkb_ਕੁੰਜੀ * ਅਚਲ ਕੁੰਜੀਆਂ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(keymap->ਅੰਕ_ਕੁੰਜੀਆਂ, ਆਕਾਰ(*ਕੁੰਜੀਆਂ));
    ਜੇ (!ਕੁੰਜੀਆਂ) {
        keymap->ਅੰਕ_ਕੁੰਜੀਆਂ = 0;
        keymap->min_ਕੁੰਜੀ_ਕੋਡ = keymap->max_ਕੁੰਜੀ_ਕੋਡ = XKB_KEYCODE_INVALID;
        ਵਾਪਸ false;
    }

    /* Low keycodes */
    ਲਈ (xkb_keycode_t kc = keymap->min_ਕੁੰਜੀ_ਕੋਡ; kc < keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ; kc++)
        ਕੁੰਜੀਆਂ[kc].keycode = kc;

    ਲਈ (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_ਮਾਪ(info->keycodes.ਨੀਵਾਂ);
         kc++)
        ਕੁੰਜੀਆਂ[kc].name = darray_ਵਸਤੂ(info->keycodes.ਨੀਵਾਂ, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ; /* First high keycode index */
    ਅਚਲ ਉੱਚਾ_Keycode_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
    darray_foreach(ਇੰਦਰਾਜ, info->keycodes.ਉੱਚਾ) {
        ਪ੍ਰਤਿਜ੍ਞਾ(ਇੰਦਰਾਜ->name != XKB_ATOM_NONE);
        ਕੁੰਜੀਆਂ[idx].keycode = ਇੰਦਰਾਜ->keycode;
        ਕੁੰਜੀਆਂ[idx].name = ਇੰਦਰਾਜ->name;
        idx++;
    }

    keymap->ਕੁੰਜੀਆਂ = ਕੁੰਜੀਆਂ;
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
CopyKeycodeNameLUT(ਬਣਤਰ xkb_keymap *keymap, ਕੁੰਜੀ_ਨਾਂ_Info *info)
{
    Keycode_ਮੇਲ *ਮੇਲ;
    xkb_atom_t name;
    darray_enumerate(name, ਮੇਲ, info->keycodes.ਨਾਂ) {
        ਜੇ (!ਮੇਲ->ਮਿਲਿਆ)
            ਜਾਰੀ;

        ਜੇ (ਮੇਲ->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ਅਚਲ Keycode_ਮੇਲ ਮੇਲ_real =
                keycode_store_lookup_name(&info->keycodes, ਮੇਲ->alias.real);
            ਜੇ (!ਮੇਲ_real.ਮਿਲਿਆ) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, name),
                        ਕੁੰਜੀ_Name_ਪਾਠ(info->ctx, ਮੇਲ->alias.real));
                ਮੇਲ->ਮਿਲਿਆ = false;
                ਜਾਰੀ;
            }
            ਪ੍ਰਤਿਜ੍ਞਾ(!ਮੇਲ_real.is_alias);
        } ਨਹੀਂਤਾਂ ਜੇ (!ਮੇਲ->ਕੁੰਜੀ.ਨੀਵਾਂ) {
            /* Update to final index in keymap::keys */
            ਮੇਲ->ਕੁੰਜੀ.ਸੂਚਕਾਂਕ += keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ;
        }
    }

    darray_shrink(info->keycodes.ਨਾਂ);
    keymap->ਅੰਕ_ਕੁੰਜੀ_ਨਾਂ = darray_ਮਾਪ(info->keycodes.ਨਾਂ);
    darray_steal(info->keycodes.ਨਾਂ, &keymap->ਕੁੰਜੀ_ਨਾਂ, ਨਿਰਾ);
    darray_init(info->keycodes.ਨਾਂ);
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
Copy_Led_ਨਾਂ_ਮੰਜ਼ਿਲ_Keymap(ਬਣਤਰ xkb_keymap *keymap, ਕੁੰਜੀ_ਨਾਂ_Info *info)
{
    keymap->ਅੰਕ_leds = info->ਅੰਕ_led_ਨਾਂ;
    ਲਈ (xkb_led_ਸੂਚਕਾਂਕ_t idx = 0; idx < info->ਅੰਕ_led_ਨਾਂ; idx++) {
        LedNameInfo *ledi = &info->led_ਨਾਂ[idx];

        ਜੇ (ledi->name == XKB_ATOM_NONE)
            ਜਾਰੀ;

        keymap->leds[idx].name = ledi->name;
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
Copy_ਕੁੰਜੀ_ਨਾਂ_Info_ਮੰਜ਼ਿਲ_Keymap(ਬਣਤਰ xkb_keymap *keymap, ਕੁੰਜੀ_ਨਾਂ_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    ਜੇ (!Copy_ਕੁੰਜੀ_ਨਾਂ_ਮੰਜ਼ਿਲ_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !Copy_Led_ਨਾਂ_ਮੰਜ਼ਿਲ_Keymap(keymap, info))
        ਵਾਪਸ false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    ਜੇ (!keymap->ਅੰਕ_ਕੁੰਜੀਆਂ ||
        keymap->min_ਕੁੰਜੀ_ਕੋਡ > 0) {
        keymap->redirect_ਕੁੰਜੀ_auto = 0;
    } ਨਹੀਂਤਾਂ {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        ਲਈ (xkb_keycode_t k = keymap->ਅੰਕ_ਕੁੰਜੀਆਂ; k-- > keymap->ਅੰਕ_ਕੁੰਜੀਆਂ_ਨੀਵਾਂ;) {
            ਜੇ (keycode > keymap->ਕੁੰਜੀਆਂ[k].keycode)
                ਰੋਕੋ;
            /* Try next keycode downwards */
            keycode = keymap->ਕੁੰਜੀਆਂ[k].keycode - 1;
        }
        keymap->redirect_ਕੁੰਜੀ_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_ਸੁਰੱਖਿਅਤ(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    ਵਾਪਸ true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, ਬਣਤਰ xkb_keymap_info *keymap_info)
{
    ਕੁੰਜੀ_ਨਾਂ_Info info;

    Init_ਕੁੰਜੀ_ਨਾਂ_Info(&info, keymap_info, 0);

    ਜੇ (file != ਨਿਰਾ)
        HandleKeycodesFile(&info, file);

    ਜੇ (info.error_ਗਿਣਤੀ != 0)
        ਜਾਓ err_info;

    ਜੇ (!Copy_ਕੁੰਜੀ_ਨਾਂ_Info_ਮੰਜ਼ਿਲ_Keymap(&keymap_info->keymap, &info))
        ਜਾਓ err_info;

    Clear_ਕੁੰਜੀ_ਨਾਂ_Info(&info);
    ਵਾਪਸ true;

err_info:
    Clear_ਕੁੰਜੀ_ਨਾਂ_Info(&info);
    ਵਾਪਸ false;
}
