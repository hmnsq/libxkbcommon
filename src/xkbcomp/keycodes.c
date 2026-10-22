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

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} ಮೇಲಿನ_Keycode_Entry;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
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
    darray(ಮೇಲಿನ_Keycode_Entry) ಮೇಲಿನ;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->ಕೆಳಗಿನ);
    darray_init(store->ಮೇಲಿನ);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->ಕೆಳಗಿನ);
    darray_free(store->ಮೇಲಿನ);
    darray_free(store->names);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
keycode_store_update_ಕೀಲಿ(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    ಯದಿ (unlikely(!match.found || match.is_alias)) {
        ಪ್ರತ್ಯಾಗಮ;
    } ಅನ್ಯಥಾ ಯದಿ (match.ಕೀಲಿ.ಕೆಳಗಿನ) {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(match.ಕೀಲಿ.index < darray_size(store->ಕೆಳಗಿನ));
        darray_item(store->ಕೆಳಗಿನ, match.ಕೀಲಿ.index) = name;
    } ಅನ್ಯಥಾ {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(match.ಕೀಲಿ.index < darray_size(store->ಮೇಲಿನ));
        darray_item(store->ಮೇಲಿನ, match.ಕೀಲಿ.index).name = name;
    }

    /* Names lookup */
    ಯದಿ (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

ಸ್ಥಿರ bool
keycode_store_ಸೇರಿಸು_ಕೀಲಿ(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    ಯದಿ (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    ಯದಿ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        ಯದಿ (kc >= (xkb_keycode_t) darray_size(store->ಕೆಳಗಿನ))
            darray_resize0(store->ಕೆಳಗಿನ, kc + 1);
        darray_item(store->ಕೆಳಗಿನ, kc) = name;
        ಯದಿ (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .ಕೀಲಿ = {
                .found = true,
                .ಕೆಳಗಿನ = true,
                .is_alias = false,
                .index = kc
            }
        };
    } ಅನ್ಯಥಾ {
        /* High keycode: insert into a sorted list */
        ಧ್ರುವ darray_size_t idx = darray_size(store->ಮೇಲಿನ);
        ಯದಿ (idx && darray_item(store->ಮೇಲಿನ, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            ಯಾವತ್ (lower < upper) {
                ಧ್ರುವ darray_size_t mid = lower + (upper - 1 - lower) / 2;
                ಧ್ರುವ ಮೇಲಿನ_Keycode_Entry * ಧ್ರುವ entry =
                    &darray_item(store->ಮೇಲಿನ, mid);
                ಯದಿ (entry->keycode < kc) {
                    lower = mid + 1;
                } ಅನ್ಯಥಾ ಯದಿ (entry->keycode > kc) {
                    upper = mid;
                } ಅನ್ಯಥಾ {
                    /* Unreachable: there is no repetition in the list */
                    ಪ್ರತಿಜ್ಞಾ(entry->keycode != kc);
                }
            }
            ಪ್ರತಿಜ್ಞಾ(lower < idx);
            ಪ್ರತಿಜ್ಞಾ(darray_item(store->ಮೇಲಿನ, lower).keycode > kc);

            /* Update references to entries that will be moved */
            ಮೇಲಿನ_Keycode_Entry *entry;
            darray_foreach_from(entry, store->ಮೇಲಿನ, lower) {
                darray_item(store->names, entry->name).ಕೀಲಿ.index++;
            }

            darray_ಸೇರಿಸು(store->ಮೇಲಿನ, lower,
                          (ಮೇಲಿನ_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .ಕೀಲಿ = {
                    .found = true,
                    .ಕೆಳಗಿನ = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } ಅನ್ಯಥಾ {
            /* Fast path: no need to sort */
            darray_append(store->ಮೇಲಿನ,
                          (ಮೇಲಿನ_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .ಕೀಲಿ = {
                    .found = true,
                    .ಕೆಳಗಿನ = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        ಯದಿ (darray_empty(store->ಕೆಳಗಿನ))
            store->min = darray_item(store->ಮೇಲಿನ, 0).keycode;
    }
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
keycode_store_ಸೇರಿಸು_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    ಯದಿ (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
keycode_store_delete_name(ಧ್ರುವ KeycodeStore *store, xkb_atom_t name)
{
    ಯದಿ (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

ಸ್ಥಿರ ಶೂನ್ಯ
keycode_store_delete_ಕೀಲಿ(KeycodeStore *store, ಧ್ರುವ KeycodeMatch match)
{
    ಯದಿ (unlikely(!match.found || match.is_alias)) {
        ಪ್ರತ್ಯಾಗಮ;
    } ಅನ್ಯಥಾ ಯದಿ (match.ಕೀಲಿ.ಕೆಳಗಿನ) {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(match.ಕೀಲಿ.index < darray_size(store->ಕೆಳಗಿನ));
        darray_item(store->names,
                    darray_item(store->ಕೆಳಗಿನ, match.ಕೀಲಿ.index)).found = false;
        ಯದಿ (match.ಕೀಲಿ.index + 1u == darray_size(store->ಕೆಳಗಿನ)) {
            /* Highest low keycode: shrink */
            ಯದಿ (store->min == match.ಕೀಲಿ.index) {
                /* No low keycode left */
                darray_size(store->ಕೆಳಗಿನ) = 0;
            } ಅನ್ಯಥಾ {
                /* Look for previous defined low keycode */
                ಪ್ರತಿ (darray_size_t idx = match.ಕೀಲಿ.index; idx > 0; idx--) {
                    ಯದಿ (darray_item(store->ಕೆಳಗಿನ, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->ಕೆಳಗಿನ) = idx;
                        ಭಙ್ಗ;
                    }
                }
            }
        } ಅನ್ಯಥಾ {
            /* Lower keycode: reset */
            darray_item(store->ಕೆಳಗಿನ, match.ಕೀಲಿ.index) = XKB_ATOM_NONE;
        }
    } ಅನ್ಯಥಾ {
        ಪ್ರತಿಜ್ಞಾ(match.ಕೀಲಿ.index < darray_size(store->ಮೇಲಿನ)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->ಮೇಲಿನ, match.ಕೀಲಿ.index).name).found = false;
        darray_remove(store->ಮೇಲಿನ, match.ಕೀಲಿ.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            ಯದಿ (entry->found && !entry->is_alias && !entry->ಕೀಲಿ.ಕೆಳಗಿನ &&
                entry->ಕೀಲಿ.index > match.ಕೀಲಿ.index) {
                entry->ಕೀಲಿ.index--;
            }
        }
    }

    /* Update bounds */
    ಯದಿ (darray_empty(store->ಕೆಳಗಿನ)) {
        store->min = (darray_empty(store->ಮೇಲಿನ))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->ಮೇಲಿನ, 0).keycode;
    } ಅನ್ಯಥಾ {
        ಪ್ರತಿ (xkb_keycode_t kc = store->min; kc < darray_size(store->ಕೆಳಗಿನ); kc++) {
            ಯದಿ (darray_item(store->ಕೆಳಗಿನ, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                ಭಙ್ಗ;
            }
        }
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_keycode_t
keycode_store_get_keycode(ಧ್ರುವ KeycodeStore *store, KeycodeMatch match)
{
    ಯದಿ (!match.found || match.is_alias) {
        ಪ್ರತ್ಯಾಗಮ XKB_KEYCODE_INVALID;
    } ಅನ್ಯಥಾ ಯದಿ (match.ಕೀಲಿ.ಕೆಳಗಿನ) {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(match.ಕೀಲಿ.index < darray_size(store->ಕೆಳಗಿನ));
        ಪ್ರತ್ಯಾಗಮ (xkb_keycode_t) match.ಕೀಲಿ.index;
    } ಅನ್ಯಥಾ {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(match.ಕೀಲಿ.index < darray_size(store->ಮೇಲಿನ));
        ಪ್ರತ್ಯಾಗಮ darray_item(store->ಮೇಲಿನ, match.ಕೀಲಿ.index).keycode;
    }
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ xkb_atom_t
keycode_store_get_ಕೀಲಿ_name(ಧ್ರುವ KeycodeStore *store, KeycodeMatch match)
{
    ಯದಿ (!match.found || match.is_alias) {
        ಪ್ರತ್ಯಾಗಮ XKB_ATOM_NONE;
    } ಅನ್ಯಥಾ ಯದಿ (match.ಕೀಲಿ.ಕೆಳಗಿನ) {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(match.ಕೀಲಿ.index < darray_size(store->ಕೆಳಗಿನ));
        ಪ್ರತ್ಯಾಗಮ darray_item(store->ಕೆಳಗಿನ, match.ಕೀಲಿ.index);
    } ಅನ್ಯಥಾ {
        /* assume valid index */
        ಪ್ರತಿಜ್ಞಾ(match.ಕೀಲಿ.index < darray_size(store->ಮೇಲಿನ));
        ಪ್ರತ್ಯಾಗಮ darray_item(store->ಮೇಲಿನ, match.ಕೀಲಿ.index).name;
    }
}

ಸ್ಥಿರ KeycodeMatch
keycode_store_lookup_keycode(ಧ್ರುವ KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    ಯದಿ (kc < (xkb_keycode_t) darray_size(store->ಕೆಳಗಿನ)) {
        ಪ್ರತ್ಯಾಗಮ (KeycodeMatch) {
            .ಕೀಲಿ = {
                .found = true,
                .ಕೆಳಗಿನ = true,
                .is_alias = false,
                .index = kc
            }
        };
    } ಅನ್ಯಥಾ ಯದಿ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        ಪ್ರತ್ಯಾಗಮ (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->ಮೇಲಿನ);
    ಯಾವತ್ (lower < upper) {
        ಧ್ರುವ darray_size_t mid = lower + (upper - 1 - lower) / 2;
        ಮೇಲಿನ_Keycode_Entry * ಧ್ರುವ entry = &darray_item(store->ಮೇಲಿನ, mid);
        ಯದಿ (entry->keycode < kc) {
            lower = mid + 1;
        } ಅನ್ಯಥಾ ಯದಿ (entry->keycode > kc) {
            upper = mid;
        } ಅನ್ಯಥಾ {
            ಪ್ರತ್ಯಾಗಮ (KeycodeMatch) {
                .ಕೀಲಿ = {
                    .found = true,
                    .ಕೆಳಗಿನ = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    ಪ್ರತ್ಯಾಗಮ (KeycodeMatch) { .found = false };
}

ಸ್ಥಿರ KeycodeMatch
keycode_store_lookup_name(ಧ್ರುವ KeycodeStore *store, xkb_atom_t name)
{
    ಯದಿ (name >= darray_size(store->names)) {
        ಪ್ರತ್ಯಾಗಮ (KeycodeMatch) { .found = false, .is_alias = false };
    } ಅನ್ಯಥಾ {
        ಪ್ರತ್ಯಾಗಮ darray_item(store->names, name);
    }
}

/***====================================================================***/

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಗಣನಾ merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ವರ್ಣ *name;
    ಪೂರ್ಣಾಙ್ಕ error_ಎಣಿಕೆ;
    unsigned ಪೂರ್ಣಾಙ್ಕ include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t ಅಂಕೆ_led_names;

    ಸಂರಚನಾ xkb_context *ctx;
    ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info;
} ಕೀಲಿ_Names_Info;

/***====================================================================***/

ಸ್ಥಿರ LedNameInfo *
FindLedByName(ಕೀಲಿ_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    ಪ್ರತಿ (xkb_led_index_t idx = 0; idx < info->ಅಂಕೆ_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        ಯದಿ (ledi->name == name) {
            *idx_out = idx;
            ಪ್ರತ್ಯಾಗಮ ledi;
        }
    }

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ bool
AddLedName(ಕೀಲಿ_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    ಧ್ರುವ bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    ಯದಿ (old) {
        ಯದಿ (old_idx == new_idx) {
            ಯದಿ (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            ಪ್ರತ್ಯಾಗಮ true;
        }

        ಯದಿ (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        ಯದಿ (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } ಅನ್ಯಥಾ {
            ಪ್ರತ್ಯಾಗಮ true;
        }
    }

    ಯದಿ (new_idx >= info->ಅಂಕೆ_led_names)
        info->ಅಂಕೆ_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    ಯದಿ (old->name != XKB_ATOM_NONE) {
        ಯದಿ (report) {
            ಧ್ರುವ xkb_atom_t use    = (replace ? new->name : old->name);
            ಧ್ರುವ xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        ಯದಿ (replace)
            *old = *new;

        ಪ್ರತ್ಯಾಗಮ true;
    }

    *old = *new;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ
Clear_ಕೀಲಿ_Names_Info(ಕೀಲಿ_Names_Info *info)
{
    ಮುಕ್ತ(info->name);
    keycode_store_free(&info->keycodes);
}

ಸ್ಥಿರ ಶೂನ್ಯ
Init_ಕೀಲಿ_Names_Info(ಕೀಲಿ_Names_Info *info, ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_info *keymap_info,
                 unsigned ಪೂರ್ಣಾಙ್ಕ include_depth)
{
    memset(info, 0, ಪರಿಮಾಣ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

ಸ್ಥಿರ bool
Add_ಕೀಲಿ_Name(ಕೀಲಿ_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           ಗಣನಾ merge_mode merge, bool report)
{
#ifdef NDEBUG
    ಧ್ರುವ
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    ಯದಿ (match_name.found) {
        ಧ್ರುವ bool clobber = (merge != MERGE_AUGMENT);

        ಯದಿ (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            ಯದಿ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         ಕೀಲಿ_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            ಯದಿ (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } ಅನ್ಯಥಾ {
                ಪ್ರತ್ಯಾಗಮ true;
            }
        } ಅನ್ಯಥಾ {
            ಧ್ರುವ xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            ಪ್ರತಿಜ್ಞಾ(old_kc != XKB_KEYCODE_INVALID);
            ಯದಿ (old_kc != kc) {
                /* There is already a different key with this name. */

                ಯದಿ (report) {
                    ಧ್ರುವ xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ಧ್ರುವ xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            ಕೀಲಿ_Name_Text(info->ctx, name), use, ignore);
                }

                ಯದಿ (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_ಕೀಲಿ(&info->keycodes, match_name);
                } ಅನ್ಯಥಾ {
                    ಪ್ರತ್ಯಾಗಮ true;
                }
            }
        }
    }

    ಧ್ರುವ KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ಧ್ರುವ xkb_atom_t old_name = keycode_store_get_ಕೀಲಿ_name(&info->keycodes, match_kc);
    ಯದಿ (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        ಯದಿ (old_name == name) {
            ಪ್ರತಿಜ್ಞಾ(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            ಯದಿ (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         ಕೀಲಿ_Name_Text(info->ctx, old_name), kc);
            ಪ್ರತ್ಯಾಗಮ true;
        }

        ಧ್ರುವ bool clobber = (merge != MERGE_AUGMENT);
        ಯದಿ (report) {
            ಧ್ರುವ ವರ್ಣ* ಧ್ರುವ kname     = ಕೀಲಿ_Name_Text(info->ctx, name);
            ಧ್ರುವ ವರ್ಣ* ಧ್ರುವ old_kname = ಕೀಲಿ_Name_Text(info->ctx, old_name);
            ಧ್ರುವ ವರ್ಣ* ಧ್ರುವ use    = (clobber) ? kname : old_kname;
            ಧ್ರುವ ವರ್ಣ* ಧ್ರುವ ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        ಯದಿ (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_ಕೀಲಿ(&info->keycodes, match_kc, name);
        }
    } ಅನ್ಯಥಾ {
        /* No previous keycode */
        ಯದಿ (!keycode_store_ಸೇರಿಸು_ಕೀಲಿ(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            ಪ್ರತ್ಯಾಗಮ false;
        }
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

/***====================================================================***/

ಸ್ಥಿರ bool
HandleAliasDef(ಕೀಲಿ_Names_Info *info, ಧ್ರುವ ಕೀಲಿ_Alias_Def *def, bool report);

ಸ್ಥಿರ ಶೂನ್ಯ
MergeKeycodeStores(ಕೀಲಿ_Names_Info *into, ಕೀಲಿ_Names_Info *from,
                   ಗಣನಾ merge_mode merge, bool report)
{
    ಯದಿ (darray_empty(into->keycodes.ಕೆಳಗಿನ) && darray_empty(into->keycodes.ಮೇಲಿನ) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.ಕೆಳಗಿನ);
        darray_init(from->keycodes.ಮೇಲಿನ);
        darray_init(from->keycodes.names);
    } ಅನ್ಯಥಾ {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        ಪ್ರತಿ (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.ಕೆಳಗಿನ);
             kc++) {
            /* Skip undefined keycodes */
            ಧ್ರುವ xkb_atom_t name = darray_item(from->keycodes.ಕೆಳಗಿನ, kc);
            ಯದಿ (name == XKB_ATOM_NONE)
                ಅನುವರ್ತ;

            ಯದಿ (!Add_ಕೀಲಿ_Name(into, kc, name, merge, report))
                into->error_ಎಣಿಕೆ++;
        }

        /* High keycodes */
        ಧ್ರುವ ಮೇಲಿನ_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.ಮೇಲಿನ) {
            ಪ್ರತಿಜ್ಞಾ(new->name != XKB_ATOM_NONE);
            ಯದಿ (!Add_ಕೀಲಿ_Name(into, new->keycode, new->name, merge, report))
                into->error_ಎಣಿಕೆ++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            ಯದಿ (!match->found || !match->is_alias)
                ಅನುವರ್ತ;

            ಧ್ರುವ ಕೀಲಿ_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            ಯದಿ (!HandleAliasDef(into, &def, report))
                into->error_ಎಣಿಕೆ++;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
MergeIncludedKeycodes(ಕೀಲಿ_Names_Info *into, ಕೀಲಿ_Names_Info *from,
                      ಗಣನಾ merge_mode merge, bool report)
{
    ಯದಿ (from->error_ಎಣಿಕೆ > 0) {
        into->error_ಎಣಿಕೆ += from->error_ಎಣಿಕೆ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (into->name == ರಿಕ್ತ) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    ಯದಿ (into->ಅಂಕೆ_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               ಪರಿಮಾಣ(*from->led_names) * from->ಅಂಕೆ_led_names);
        into->ಅಂಕೆ_led_names = from->ಅಂಕೆ_led_names;
        from->ಅಂಕೆ_led_names = 0;
    }
    ಅನ್ಯಥಾ {
        ಪ್ರತಿ (xkb_led_index_t idx = 0; idx < from->ಅಂಕೆ_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            ಯದಿ (ledi->name == XKB_ATOM_NONE)
                ಅನುವರ್ತ;

            ledi->merge = merge;
            ಯದಿ (!AddLedName(into, false, ledi, idx, report))
                into->error_ಎಣಿಕೆ++;
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleKeycodesFile(ಕೀಲಿ_Names_Info *info, XkbFile *file);

ಸ್ಥಿರ bool
HandleIncludeKeycodes(ಕೀಲಿ_Names_Info *info, IncludeStmt *include, bool report)
{
    ಕೀಲಿ_Names_Info included;

    ಯದಿ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ಎಣಿಕೆ += 10;
        ಪ್ರತ್ಯಾಗಮ false;
    }

    Init_ಕೀಲಿ_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ಧ್ರುವ ಸಂರಚನಾ parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ಪ್ರತಿ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        ಕೀಲಿ_Names_Info next_incl;
        XkbFile *file;

        ವರ್ಣ path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, ಪರಿಮಾಣ(path));
        ಯದಿ (!file) {
            info->error_ಎಣಿಕೆ += 10;
            Clear_ಕೀಲಿ_Names_Info(&included);
            ಪ್ರತ್ಯಾಗಮ false;
        }

        Init_ಕೀಲಿ_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_ಕೀಲಿ_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_ಕೀಲಿ_Names_Info(&included);

    ಪ್ರತ್ಯಾಗಮ (info->error_ಎಣಿಕೆ == 0);
}

ಸ್ಥಿರ bool
HandleKeycodeDef(ಕೀಲಿ_Names_Info *info, KeycodeDef *stmt, bool report)
{
    ಯದಿ (stmt->ಮೌಲ್ಯ < 0 || stmt->ಮೌಲ್ಯ > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->ಮೌಲ್ಯ, XKB_KEYCODE_MAX);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಪ್ರತ್ಯಾಗಮ Add_ಕೀಲಿ_Name(info, (xkb_keycode_t) stmt->ಮೌಲ್ಯ,
                      stmt->name, stmt->merge, report);
}

ಸ್ಥಿರ bool
HandleAliasDef(ಕೀಲಿ_Names_Info *info, ಧ್ರುವ ಕೀಲಿ_Alias_Def *def, bool report)
{
    ಧ್ರುವ KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    ಯದಿ (match_name.found) {
        ಧ್ರುವ bool clobber = (def->merge != MERGE_AUGMENT);
        ಯದಿ (match_name.is_alias) {
            ಯದಿ (def->real == match_name.alias.real) {
                ಯದಿ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             ಕೀಲಿ_Name_Text(info->ctx, def->alias),
                             ಕೀಲಿ_Name_Text(info->ctx, def->real));
            } ಅನ್ಯಥಾ {
                ಧ್ರುವ xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                ಧ್ರುವ xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                ಯದಿ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             ಕೀಲಿ_Name_Text(info->ctx, def->alias),
                             ಕೀಲಿ_Name_Text(info->ctx, use),
                             ಕೀಲಿ_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            ಪ್ರತ್ಯಾಗಮ true;
        } ಅನ್ಯಥಾ {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            ಯದಿ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ಕೀಲಿ_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         ಕೀಲಿ_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            ಯದಿ (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_ಕೀಲಿ(&info->keycodes, match_name);
            } ಅನ್ಯಥಾ {
                ಪ್ರತ್ಯಾಗಮ true;
            }
        }
    }

    ಪ್ರತ್ಯಾಗಮ keycode_store_ಸೇರಿಸು_alias(&info->keycodes, def->alias, def->real);
}

ಸ್ಥಿರ bool
Handle_ಕೀಲಿ_Name_Var(ಕೀಲಿ_Names_Info *info, VarDef *stmt)
{
    ಧ್ರುವ ವರ್ಣ *elem, *field;
    ಅಭಿವ್ಯಕ್ತಿ_Def *arrayNdx;

    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        ಪ್ರತ್ಯಾಗಮ false;

    ಯದಿ (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ಯದಿ (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ಯದಿ (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_Integer(info->ctx, stmt->ಮೌಲ್ಯ, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        ಪ್ರತ್ಯಾಗಮ !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
HandleLedNameDef(ಕೀಲಿ_Names_Info *info, LedNameDef *def, bool report)
{
    ಯದಿ (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_ಎಣಿಕೆ++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        ಪ್ರತ್ಯಾಗಮ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ಯದಿ (!ಅಭಿವ್ಯಕ್ತಿ_Resolve_String(info->ctx, def->name, &name)) {
        ವರ್ಣ buf[20];
        snprintf(buf, ಪರಿಮಾಣ(buf), "%"PRId64, def->ndx);
        info->error_ಎಣಿಕೆ++;
        ಪ್ರತ್ಯಾಗಮ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    ಪ್ರತ್ಯಾಗಮ AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

ಸ್ಥಿರ ಶೂನ್ಯ
HandleKeycodesFile(ಕೀಲಿ_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ verbosity = xkb_context_get_log_verbosity(info->ctx);
    ಧ್ರುವ bool report_same_file = verbosity > 0;
    ಧ್ರುವ bool report_include   = verbosity > 7;

    ಮುಕ್ತ(info->name);
    info->name = strdup_safe(file->name);

    ಪ್ರತಿ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        ವಿಕಲ್ಪ (stmt->type) {
        ಪಕ್ಷ STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_ALIAS:
            ok = HandleAliasDef(info, (ಕೀಲಿ_Alias_Def *) stmt, report_same_file);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_VAR:
            ok = Handle_ಕೀಲಿ_Name_Var(info, (VarDef *) stmt);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            ಭಙ್ಗ;
        ಪಕ್ಷ STMT_UNKNOWN_DECLARATION:
        ಪಕ್ಷ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ಭಙ್ಗ;
        }

        ಯದಿ (!ok)
            info->error_ಎಣಿಕೆ++;

        ಯದಿ (info->error_ಎಣಿಕೆ > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            ಭಙ್ಗ;
        }
    }
}

/***====================================================================***/

ಸ್ಥಿರ bool
Copy_ಕೀಲಿ_Names_To_Keymap(ಸಂರಚನಾ xkb_keymap *keymap, ಕೀಲಿ_Names_Info *info)
{
    ಯದಿ (darray_empty(info->keycodes.ಕೆಳಗಿನ) && darray_empty(info->keycodes.ಮೇಲಿನ)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        ಪ್ರತಿಜ್ಞಾ(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_ಕೀಲಿ_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_ಕೀಲಿ_code = 255;
        keymap->ಅಂಕೆ_ಕೀಲಿಗಳು = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ = keymap->max_ಕೀಲಿ_code + 1;
    } ಅನ್ಯಥಾ {
        ಪ್ರತಿಜ್ಞಾ(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_ಕೀಲಿ_code = info->keycodes.min;
        keymap->max_ಕೀಲಿ_code = (darray_empty(info->keycodes.ಮೇಲಿನ))
            ? darray_size(info->keycodes.ಕೆಳಗಿನ) - 1
            : darray_item(
                info->keycodes.ಮೇಲಿನ,
                darray_size(info->keycodes.ಮೇಲಿನ) - 1
            ).keycode;
        keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ = darray_size(info->keycodes.ಕೆಳಗಿನ);
        keymap->ಅಂಕೆ_ಕೀಲಿಗಳು = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ
                         + darray_size(info->keycodes.ಮೇಲಿನ);
    }

    ಸಂರಚನಾ xkb_ಕೀಲಿ * ಧ್ರುವ ಕೀಲಿಗಳು = calloc(keymap->ಅಂಕೆ_ಕೀಲಿಗಳು, ಪರಿಮಾಣ(*ಕೀಲಿಗಳು));
    ಯದಿ (!ಕೀಲಿಗಳು) {
        keymap->ಅಂಕೆ_ಕೀಲಿಗಳು = 0;
        keymap->min_ಕೀಲಿ_code = keymap->max_ಕೀಲಿ_code = XKB_KEYCODE_INVALID;
        ಪ್ರತ್ಯಾಗಮ false;
    }

    /* Low keycodes */
    ಪ್ರತಿ (xkb_keycode_t kc = keymap->min_ಕೀಲಿ_code; kc < keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ; kc++)
        ಕೀಲಿಗಳು[kc].keycode = kc;

    ಪ್ರತಿ (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.ಕೆಳಗಿನ);
         kc++)
        ಕೀಲಿಗಳು[kc].name = darray_item(info->keycodes.ಕೆಳಗಿನ, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ; /* First high keycode index */
    ಧ್ರುವ ಮೇಲಿನ_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.ಮೇಲಿನ) {
        ಪ್ರತಿಜ್ಞಾ(entry->name != XKB_ATOM_NONE);
        ಕೀಲಿಗಳು[idx].keycode = entry->keycode;
        ಕೀಲಿಗಳು[idx].name = entry->name;
        idx++;
    }

    keymap->ಕೀಲಿಗಳು = ಕೀಲಿಗಳು;
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
CopyKeycodeNameLUT(ಸಂರಚನಾ xkb_keymap *keymap, ಕೀಲಿ_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        ಯದಿ (!match->found)
            ಅನುವರ್ತ;

        ಯದಿ (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ಧ್ರುವ KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            ಯದಿ (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        ಕೀಲಿ_Name_Text(info->ctx, name),
                        ಕೀಲಿ_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                ಅನುವರ್ತ;
            }
            ಪ್ರತಿಜ್ಞಾ(!match_real.is_alias);
        } ಅನ್ಯಥಾ ಯದಿ (!match->ಕೀಲಿ.ಕೆಳಗಿನ) {
            /* Update to final index in keymap::keys */
            match->ಕೀಲಿ.index += keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->ಅಂಕೆ_ಕೀಲಿ_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->ಕೀಲಿ_names, ರಿಕ್ತ);
    darray_init(info->keycodes.names);
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
CopyLedNamesToKeymap(ಸಂರಚನಾ xkb_keymap *keymap, ಕೀಲಿ_Names_Info *info)
{
    keymap->ಅಂಕೆ_leds = info->ಅಂಕೆ_led_names;
    ಪ್ರತಿ (xkb_led_index_t idx = 0; idx < info->ಅಂಕೆ_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        ಯದಿ (ledi->name == XKB_ATOM_NONE)
            ಅನುವರ್ತ;

        keymap->leds[idx].name = ledi->name;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
Copy_ಕೀಲಿ_Names_Info_To_Keymap(ಸಂರಚನಾ xkb_keymap *keymap, ಕೀಲಿ_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    ಯದಿ (!Copy_ಕೀಲಿ_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        ಪ್ರತ್ಯಾಗಮ false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    ಯದಿ (!keymap->ಅಂಕೆ_ಕೀಲಿಗಳು ||
        keymap->min_ಕೀಲಿ_code > 0) {
        keymap->redirect_ಕೀಲಿ_auto = 0;
    } ಅನ್ಯಥಾ {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        ಪ್ರತಿ (xkb_keycode_t k = keymap->ಅಂಕೆ_ಕೀಲಿಗಳು; k-- > keymap->ಅಂಕೆ_ಕೀಲಿಗಳು_ಕೆಳಗಿನ;) {
            ಯದಿ (keycode > keymap->ಕೀಲಿಗಳು[k].keycode)
                ಭಙ್ಗ;
            /* Try next keycode downwards */
            keycode = keymap->ಕೀಲಿಗಳು[k].keycode - 1;
        }
        keymap->redirect_ಕೀಲಿ_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    ಪ್ರತ್ಯಾಗಮ true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, ಸಂರಚನಾ xkb_keymap_info *keymap_info)
{
    ಕೀಲಿ_Names_Info info;

    Init_ಕೀಲಿ_Names_Info(&info, keymap_info, 0);

    ಯದಿ (file != ರಿಕ್ತ)
        HandleKeycodesFile(&info, file);

    ಯದಿ (info.error_ಎಣಿಕೆ != 0)
        ಗಚ್ಛ err_info;

    ಯದಿ (!Copy_ಕೀಲಿ_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        ಗಚ್ಛ err_info;

    Clear_ಕೀಲಿ_Names_Info(&info);
    ಪ್ರತ್ಯಾಗಮ true;

err_info:
    Clear_ಕೀಲಿ_Names_Info(&info);
    ಪ್ರತ್ಯಾಗಮ false;
}
