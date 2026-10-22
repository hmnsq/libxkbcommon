/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ "context.h"
#ଅନ୍ତର୍ଭାଵ "messages-codes.h"
#ଅନ୍ତର୍ଭାଵ "xkbcommon/xkbcommon.h"
#ଅନ୍ତର୍ଭାଵ "xkbcomp-priv.h"
#ଅନ୍ତର୍ଭାଵ "atom.h"
#ଅନ୍ତର୍ଭାଵ "darray.h"
#ଅନ୍ତର୍ଭାଵ "keymap.h"
#ଅନ୍ତର୍ଭାଵ "text.h"
#ଅନ୍ତର୍ଭାଵ "expr.h"
#ଅନ୍ତର୍ଭାଵ "include.h"
#ଅନ୍ତର୍ଭାଵ "util-mem.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} ଉପର_Keycode_Entry;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) ତଳ;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(ଉପର_Keycode_Entry) ଉପର;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->ତଳ);
    darray_init(store->ଉପର);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->ତଳ);
    darray_free(store->ଉପର);
    darray_free(store->names);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
keycode_store_update_ଚାବି(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    ଯଦି (unlikely(!match.found || match.is_alias)) {
        ଫେରନ୍ତୁ;
    } ଅନ୍ୟଥା ଯଦି (match.ଚାବି.ତଳ) {
        /* assume valid index */
        ପ୍ରତିଜ୍ଞା(match.ଚାବି.index < darray_size(store->ତଳ));
        darray_item(store->ତଳ, match.ଚାବି.index) = name;
    } ଅନ୍ୟଥା {
        /* assume valid index */
        ପ୍ରତିଜ୍ଞା(match.ଚାବି.index < darray_size(store->ଉପର));
        darray_item(store->ଉପର, match.ଚାବି.index).name = name;
    }

    /* Names lookup */
    ଯଦି (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

ସ୍ଥିର bool
keycode_store_ଭର୍ତ୍ତି_ଚାବି(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    ଯଦି (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    ଯଦି (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        ଯଦି (kc >= (xkb_keycode_t) darray_size(store->ତଳ))
            darray_resize0(store->ତଳ, kc + 1);
        darray_item(store->ତଳ, kc) = name;
        ଯଦି (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .ଚାବି = {
                .found = true,
                .ତଳ = true,
                .is_alias = false,
                .index = kc
            }
        };
    } ଅନ୍ୟଥା {
        /* High keycode: insert into a sorted list */
        ଅଚଳ darray_size_t idx = darray_size(store->ଉପର);
        ଯଦି (idx && darray_item(store->ଉପର, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            ଯେତେବେଳେ (lower < upper) {
                ଅଚଳ darray_size_t mid = lower + (upper - 1 - lower) / 2;
                ଅଚଳ ଉପର_Keycode_Entry * ଅଚଳ entry =
                    &darray_item(store->ଉପର, mid);
                ଯଦି (entry->keycode < kc) {
                    lower = mid + 1;
                } ଅନ୍ୟଥା ଯଦି (entry->keycode > kc) {
                    upper = mid;
                } ଅନ୍ୟଥା {
                    /* Unreachable: there is no repetition in the list */
                    ପ୍ରତିଜ୍ଞା(entry->keycode != kc);
                }
            }
            ପ୍ରତିଜ୍ଞା(lower < idx);
            ପ୍ରତିଜ୍ଞା(darray_item(store->ଉପର, lower).keycode > kc);

            /* Update references to entries that will be moved */
            ଉପର_Keycode_Entry *entry;
            darray_foreach_from(entry, store->ଉପର, lower) {
                darray_item(store->names, entry->name).ଚାବି.index++;
            }

            darray_ଭର୍ତ୍ତି(store->ଉପର, lower,
                          (ଉପର_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .ଚାବି = {
                    .found = true,
                    .ତଳ = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } ଅନ୍ୟଥା {
            /* Fast path: no need to sort */
            darray_append(store->ଉପର,
                          (ଉପର_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .ଚାବି = {
                    .found = true,
                    .ତଳ = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        ଯଦି (darray_empty(store->ତଳ))
            store->min = darray_item(store->ଉପର, 0).keycode;
    }
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
keycode_store_ଭର୍ତ୍ତି_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    ଯଦି (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
keycode_store_delete_name(ଅଚଳ KeycodeStore *store, xkb_atom_t name)
{
    ଯଦି (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

ସ୍ଥିର ଶୂନ୍ୟ
keycode_store_delete_ଚାବି(KeycodeStore *store, ଅଚଳ KeycodeMatch match)
{
    ଯଦି (unlikely(!match.found || match.is_alias)) {
        ଫେରନ୍ତୁ;
    } ଅନ୍ୟଥା ଯଦି (match.ଚାବି.ତଳ) {
        /* assume valid index */
        ପ୍ରତିଜ୍ଞା(match.ଚାବି.index < darray_size(store->ତଳ));
        darray_item(store->names,
                    darray_item(store->ତଳ, match.ଚାବି.index)).found = false;
        ଯଦି (match.ଚାବି.index + 1u == darray_size(store->ତଳ)) {
            /* Highest low keycode: shrink */
            ଯଦି (store->min == match.ଚାବି.index) {
                /* No low keycode left */
                darray_size(store->ତଳ) = 0;
            } ଅନ୍ୟଥା {
                /* Look for previous defined low keycode */
                ପାଇଁ (darray_size_t idx = match.ଚାବି.index; idx > 0; idx--) {
                    ଯଦି (darray_item(store->ତଳ, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->ତଳ) = idx;
                        ଭାଙ୍ଗ;
                    }
                }
            }
        } ଅନ୍ୟଥା {
            /* Lower keycode: reset */
            darray_item(store->ତଳ, match.ଚାବି.index) = XKB_ATOM_NONE;
        }
    } ଅନ୍ୟଥା {
        ପ୍ରତିଜ୍ଞା(match.ଚାବି.index < darray_size(store->ଉପର)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->ଉପର, match.ଚାବି.index).name).found = false;
        darray_remove(store->ଉପର, match.ଚାବି.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            ଯଦି (entry->found && !entry->is_alias && !entry->ଚାବି.ତଳ &&
                entry->ଚାବି.index > match.ଚାବି.index) {
                entry->ଚାବି.index--;
            }
        }
    }

    /* Update bounds */
    ଯଦି (darray_empty(store->ତଳ)) {
        store->min = (darray_empty(store->ଉପର))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->ଉପର, 0).keycode;
    } ଅନ୍ୟଥା {
        ପାଇଁ (xkb_keycode_t kc = store->min; kc < darray_size(store->ତଳ); kc++) {
            ଯଦି (darray_item(store->ତଳ, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                ଭାଙ୍ଗ;
            }
        }
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ xkb_keycode_t
keycode_store_get_keycode(ଅଚଳ KeycodeStore *store, KeycodeMatch match)
{
    ଯଦି (!match.found || match.is_alias) {
        ଫେରନ୍ତୁ XKB_KEYCODE_INVALID;
    } ଅନ୍ୟଥା ଯଦି (match.ଚାବି.ତଳ) {
        /* assume valid index */
        ପ୍ରତିଜ୍ଞା(match.ଚାବି.index < darray_size(store->ତଳ));
        ଫେରନ୍ତୁ (xkb_keycode_t) match.ଚାବି.index;
    } ଅନ୍ୟଥା {
        /* assume valid index */
        ପ୍ରତିଜ୍ଞା(match.ଚାବି.index < darray_size(store->ଉପର));
        ଫେରନ୍ତୁ darray_item(store->ଉପର, match.ଚାବି.index).keycode;
    }
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ xkb_atom_t
keycode_store_get_ଚାବି_name(ଅଚଳ KeycodeStore *store, KeycodeMatch match)
{
    ଯଦି (!match.found || match.is_alias) {
        ଫେରନ୍ତୁ XKB_ATOM_NONE;
    } ଅନ୍ୟଥା ଯଦି (match.ଚାବି.ତଳ) {
        /* assume valid index */
        ପ୍ରତିଜ୍ଞା(match.ଚାବି.index < darray_size(store->ତଳ));
        ଫେରନ୍ତୁ darray_item(store->ତଳ, match.ଚାବି.index);
    } ଅନ୍ୟଥା {
        /* assume valid index */
        ପ୍ରତିଜ୍ଞା(match.ଚାବି.index < darray_size(store->ଉପର));
        ଫେରନ୍ତୁ darray_item(store->ଉପର, match.ଚାବି.index).name;
    }
}

ସ୍ଥିର KeycodeMatch
keycode_store_lookup_keycode(ଅଚଳ KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    ଯଦି (kc < (xkb_keycode_t) darray_size(store->ତଳ)) {
        ଫେରନ୍ତୁ (KeycodeMatch) {
            .ଚାବି = {
                .found = true,
                .ତଳ = true,
                .is_alias = false,
                .index = kc
            }
        };
    } ଅନ୍ୟଥା ଯଦି (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        ଫେରନ୍ତୁ (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->ଉପର);
    ଯେତେବେଳେ (lower < upper) {
        ଅଚଳ darray_size_t mid = lower + (upper - 1 - lower) / 2;
        ଉପର_Keycode_Entry * ଅଚଳ entry = &darray_item(store->ଉପର, mid);
        ଯଦି (entry->keycode < kc) {
            lower = mid + 1;
        } ଅନ୍ୟଥା ଯଦି (entry->keycode > kc) {
            upper = mid;
        } ଅନ୍ୟଥା {
            ଫେରନ୍ତୁ (KeycodeMatch) {
                .ଚାବି = {
                    .found = true,
                    .ତଳ = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    ଫେରନ୍ତୁ (KeycodeMatch) { .found = false };
}

ସ୍ଥିର KeycodeMatch
keycode_store_lookup_name(ଅଚଳ KeycodeStore *store, xkb_atom_t name)
{
    ଯଦି (name >= darray_size(store->names)) {
        ଫେରନ୍ତୁ (KeycodeMatch) { .found = false, .is_alias = false };
    } ଅନ୍ୟଥା {
        ଫେରନ୍ତୁ darray_item(store->names, name);
    }
}

/***====================================================================***/

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଗଣନା merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଅକ୍ଷର *name;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା error_ଗଣନା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t ଅଙ୍କ_led_names;

    ସଂରଚନା xkb_context *ctx;
    ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info;
} ଚାବି_Names_Info;

/***====================================================================***/

ସ୍ଥିର LedNameInfo *
FindLedByName(ଚାବି_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    ପାଇଁ (xkb_led_index_t idx = 0; idx < info->ଅଙ୍କ_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        ଯଦି (ledi->name == name) {
            *idx_out = idx;
            ଫେରନ୍ତୁ ledi;
        }
    }

    ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର bool
AddLedName(ଚାବି_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    ଅଚଳ bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    ଯଦି (old) {
        ଯଦି (old_idx == new_idx) {
            ଯଦି (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            ଫେରନ୍ତୁ true;
        }

        ଯଦି (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        ଯଦି (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } ଅନ୍ୟଥା {
            ଫେରନ୍ତୁ true;
        }
    }

    ଯଦି (new_idx >= info->ଅଙ୍କ_led_names)
        info->ଅଙ୍କ_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    ଯଦି (old->name != XKB_ATOM_NONE) {
        ଯଦି (report) {
            ଅଚଳ xkb_atom_t use    = (replace ? new->name : old->name);
            ଅଚଳ xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        ଯଦି (replace)
            *old = *new;

        ଫେରନ୍ତୁ true;
    }

    *old = *new;
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ
Clear_ଚାବି_Names_Info(ଚାବି_Names_Info *info)
{
    ମୁକ୍ତ(info->name);
    keycode_store_free(&info->keycodes);
}

ସ୍ଥିର ଶୂନ୍ୟ
Init_ଚାବି_Names_Info(ଚାବି_Names_Info *info, ଅଚଳ ସଂରଚନା xkb_keymap_info *keymap_info,
                 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା include_depth)
{
    memset(info, 0, ଆକାର(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

ସ୍ଥିର bool
Add_ଚାବି_Name(ଚାବି_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           ଗଣନା merge_mode merge, bool report)
{
#ifdef NDEBUG
    ଅଚଳ
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    ଯଦି (match_name.found) {
        ଅଚଳ bool clobber = (merge != MERGE_AUGMENT);

        ଯଦି (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            ଯଦି (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         ଚାବି_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            ଯଦି (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } ଅନ୍ୟଥା {
                ଫେରନ୍ତୁ true;
            }
        } ଅନ୍ୟଥା {
            ଅଚଳ xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            ପ୍ରତିଜ୍ଞା(old_kc != XKB_KEYCODE_INVALID);
            ଯଦି (old_kc != kc) {
                /* There is already a different key with this name. */

                ଯଦି (report) {
                    ଅଚଳ xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ଅଚଳ xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            ଚାବି_Name_Text(info->ctx, name), use, ignore);
                }

                ଯଦି (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_ଚାବି(&info->keycodes, match_name);
                } ଅନ୍ୟଥା {
                    ଫେରନ୍ତୁ true;
                }
            }
        }
    }

    ଅଚଳ KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ଅଚଳ xkb_atom_t old_name = keycode_store_get_ଚାବି_name(&info->keycodes, match_kc);
    ଯଦି (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        ଯଦି (old_name == name) {
            ପ୍ରତିଜ୍ଞା(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            ଯଦି (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         ଚାବି_Name_Text(info->ctx, old_name), kc);
            ଫେରନ୍ତୁ true;
        }

        ଅଚଳ bool clobber = (merge != MERGE_AUGMENT);
        ଯଦି (report) {
            ଅଚଳ ଅକ୍ଷର* ଅଚଳ kname     = ଚାବି_Name_Text(info->ctx, name);
            ଅଚଳ ଅକ୍ଷର* ଅଚଳ old_kname = ଚାବି_Name_Text(info->ctx, old_name);
            ଅଚଳ ଅକ୍ଷର* ଅଚଳ use    = (clobber) ? kname : old_kname;
            ଅଚଳ ଅକ୍ଷର* ଅଚଳ ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        ଯଦି (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_ଚାବି(&info->keycodes, match_kc, name);
        }
    } ଅନ୍ୟଥା {
        /* No previous keycode */
        ଯଦି (!keycode_store_ଭର୍ତ୍ତି_ଚାବି(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            ଫେରନ୍ତୁ false;
        }
    }

    ଫେରନ୍ତୁ true;
}

/***====================================================================***/

ସ୍ଥିର bool
HandleAliasDef(ଚାବି_Names_Info *info, ଅଚଳ ଚାବି_Alias_Def *def, bool report);

ସ୍ଥିର ଶୂନ୍ୟ
MergeKeycodeStores(ଚାବି_Names_Info *into, ଚାବି_Names_Info *from,
                   ଗଣନା merge_mode merge, bool report)
{
    ଯଦି (darray_empty(into->keycodes.ତଳ) && darray_empty(into->keycodes.ଉପର) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.ତଳ);
        darray_init(from->keycodes.ଉପର);
        darray_init(from->keycodes.names);
    } ଅନ୍ୟଥା {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        ପାଇଁ (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.ତଳ);
             kc++) {
            /* Skip undefined keycodes */
            ଅଚଳ xkb_atom_t name = darray_item(from->keycodes.ତଳ, kc);
            ଯଦି (name == XKB_ATOM_NONE)
                ଜାରି;

            ଯଦି (!Add_ଚାବି_Name(into, kc, name, merge, report))
                into->error_ଗଣନା++;
        }

        /* High keycodes */
        ଅଚଳ ଉପର_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.ଉପର) {
            ପ୍ରତିଜ୍ଞା(new->name != XKB_ATOM_NONE);
            ଯଦି (!Add_ଚାବି_Name(into, new->keycode, new->name, merge, report))
                into->error_ଗଣନା++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            ଯଦି (!match->found || !match->is_alias)
                ଜାରି;

            ଅଚଳ ଚାବି_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            ଯଦି (!HandleAliasDef(into, &def, report))
                into->error_ଗଣନା++;
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
MergeIncludedKeycodes(ଚାବି_Names_Info *into, ଚାବି_Names_Info *from,
                      ଗଣନା merge_mode merge, bool report)
{
    ଯଦି (from->error_ଗଣନା > 0) {
        into->error_ଗଣନା += from->error_ଗଣନା;
        ଫେରନ୍ତୁ;
    }

    ଯଦି (into->name == ଖାଲି) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    ଯଦି (into->ଅଙ୍କ_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               ଆକାର(*from->led_names) * from->ଅଙ୍କ_led_names);
        into->ଅଙ୍କ_led_names = from->ଅଙ୍କ_led_names;
        from->ଅଙ୍କ_led_names = 0;
    }
    ଅନ୍ୟଥା {
        ପାଇଁ (xkb_led_index_t idx = 0; idx < from->ଅଙ୍କ_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            ଯଦି (ledi->name == XKB_ATOM_NONE)
                ଜାରି;

            ledi->merge = merge;
            ଯଦି (!AddLedName(into, false, ledi, idx, report))
                into->error_ଗଣନା++;
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
HandleKeycodesFile(ଚାବି_Names_Info *info, XkbFile *file);

ସ୍ଥିର bool
HandleIncludeKeycodes(ଚାବି_Names_Info *info, IncludeStmt *include, bool report)
{
    ଚାବି_Names_Info included;

    ଯଦି (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ଗଣନା += 10;
        ଫେରନ୍ତୁ false;
    }

    Init_ଚାବି_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ଅଚଳ ସଂରଚନା parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    ପାଇଁ (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        ଚାବି_Names_Info next_incl;
        XkbFile *file;

        ଅକ୍ଷର path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, ଆକାର(path));
        ଯଦି (!file) {
            info->error_ଗଣନା += 10;
            Clear_ଚାବି_Names_Info(&included);
            ଫେରନ୍ତୁ false;
        }

        Init_ଚାବି_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_ଚାବି_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_ଚାବି_Names_Info(&included);

    ଫେରନ୍ତୁ (info->error_ଗଣନା == 0);
}

ସ୍ଥିର bool
HandleKeycodeDef(ଚାବି_Names_Info *info, KeycodeDef *stmt, bool report)
{
    ଯଦି (stmt->ମୂଲ୍ୟ < 0 || stmt->ମୂଲ୍ୟ > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->ମୂଲ୍ୟ, XKB_KEYCODE_MAX);
        ଫେରନ୍ତୁ false;
    }

    ଫେରନ୍ତୁ Add_ଚାବି_Name(info, (xkb_keycode_t) stmt->ମୂଲ୍ୟ,
                      stmt->name, stmt->merge, report);
}

ସ୍ଥିର bool
HandleAliasDef(ଚାବି_Names_Info *info, ଅଚଳ ଚାବି_Alias_Def *def, bool report)
{
    ଅଚଳ KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    ଯଦି (match_name.found) {
        ଅଚଳ bool clobber = (def->merge != MERGE_AUGMENT);
        ଯଦି (match_name.is_alias) {
            ଯଦି (def->real == match_name.alias.real) {
                ଯଦି (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             ଚାବି_Name_Text(info->ctx, def->alias),
                             ଚାବି_Name_Text(info->ctx, def->real));
            } ଅନ୍ୟଥା {
                ଅଚଳ xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                ଅଚଳ xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                ଯଦି (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             ଚାବି_Name_Text(info->ctx, def->alias),
                             ଚାବି_Name_Text(info->ctx, use),
                             ଚାବି_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            ଫେରନ୍ତୁ true;
        } ଅନ୍ୟଥା {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            ଯଦି (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ଚାବି_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         ଚାବି_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            ଯଦି (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_ଚାବି(&info->keycodes, match_name);
            } ଅନ୍ୟଥା {
                ଫେରନ୍ତୁ true;
            }
        }
    }

    ଫେରନ୍ତୁ keycode_store_ଭର୍ତ୍ତି_alias(&info->keycodes, def->alias, def->real);
}

ସ୍ଥିର bool
Handle_ଚାବି_Name_Var(ଚାବି_Names_Info *info, VarDef *stmt)
{
    ଅଚଳ ଅକ୍ଷର *elem, *field;
    ପଦସମଷ୍ଟି_Def *arrayNdx;

    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        ଫେରନ୍ତୁ false;

    ଯଦି (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        ଫେରନ୍ତୁ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ଯଦି (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ଫେରନ୍ତୁ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ଯଦି (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        ଫେରନ୍ତୁ !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_Integer(info->ctx, stmt->ମୂଲ୍ୟ, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        ଫେରନ୍ତୁ !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
HandleLedNameDef(ଚାବି_Names_Info *info, LedNameDef *def, bool report)
{
    ଯଦି (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_ଗଣନା++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        ଫେରନ୍ତୁ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ଯଦି (!ପଦସମଷ୍ଟି_Resolve_String(info->ctx, def->name, &name)) {
        ଅକ୍ଷର buf[20];
        snprintf(buf, ଆକାର(buf), "%"PRId64, def->ndx);
        info->error_ଗଣନା++;
        ଫେରନ୍ତୁ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    ଫେରନ୍ତୁ AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

ସ୍ଥିର ଶୂନ୍ୟ
HandleKeycodesFile(ଚାବି_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା verbosity = xkb_context_get_log_verbosity(info->ctx);
    ଅଚଳ bool report_same_file = verbosity > 0;
    ଅଚଳ bool report_include   = verbosity > 7;

    ମୁକ୍ତ(info->name);
    info->name = strdup_safe(file->name);

    ପାଇଁ (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        ବାଛ (stmt->type) {
        ସ୍ଥିତି STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_ALIAS:
            ok = HandleAliasDef(info, (ଚାବି_Alias_Def *) stmt, report_same_file);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_VAR:
            ok = Handle_ଚାବି_Name_Var(info, (VarDef *) stmt);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            ଭାଙ୍ଗ;
        ସ୍ଥିତି STMT_UNKNOWN_DECLARATION:
        ସ୍ଥିତି STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ଭାଙ୍ଗ;
        }

        ଯଦି (!ok)
            info->error_ଗଣନା++;

        ଯଦି (info->error_ଗଣନା > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            ଭାଙ୍ଗ;
        }
    }
}

/***====================================================================***/

ସ୍ଥିର bool
Copy_ଚାବି_Names_To_Keymap(ସଂରଚନା xkb_keymap *keymap, ଚାବି_Names_Info *info)
{
    ଯଦି (darray_empty(info->keycodes.ତଳ) && darray_empty(info->keycodes.ଉପର)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        ପ୍ରତିଜ୍ଞା(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_ଚାବି_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_ଚାବି_code = 255;
        keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ = keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ = keymap->max_ଚାବି_code + 1;
    } ଅନ୍ୟଥା {
        ପ୍ରତିଜ୍ଞା(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_ଚାବି_code = info->keycodes.min;
        keymap->max_ଚାବି_code = (darray_empty(info->keycodes.ଉପର))
            ? darray_size(info->keycodes.ତଳ) - 1
            : darray_item(
                info->keycodes.ଉପର,
                darray_size(info->keycodes.ଉପର) - 1
            ).keycode;
        keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ = darray_size(info->keycodes.ତଳ);
        keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ = keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ
                         + darray_size(info->keycodes.ଉପର);
    }

    ସଂରଚନା xkb_ଚାବି * ଅଚଳ ଚାବିଗୁଡ଼ିକ = calloc(keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ, ଆକାର(*ଚାବିଗୁଡ଼ିକ));
    ଯଦି (!ଚାବିଗୁଡ଼ିକ) {
        keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ = 0;
        keymap->min_ଚାବି_code = keymap->max_ଚାବି_code = XKB_KEYCODE_INVALID;
        ଫେରନ୍ତୁ false;
    }

    /* Low keycodes */
    ପାଇଁ (xkb_keycode_t kc = keymap->min_ଚାବି_code; kc < keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ; kc++)
        ଚାବିଗୁଡ଼ିକ[kc].keycode = kc;

    ପାଇଁ (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.ତଳ);
         kc++)
        ଚାବିଗୁଡ଼ିକ[kc].name = darray_item(info->keycodes.ତଳ, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ; /* First high keycode index */
    ଅଚଳ ଉପର_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.ଉପର) {
        ପ୍ରତିଜ୍ଞା(entry->name != XKB_ATOM_NONE);
        ଚାବିଗୁଡ଼ିକ[idx].keycode = entry->keycode;
        ଚାବିଗୁଡ଼ିକ[idx].name = entry->name;
        idx++;
    }

    keymap->ଚାବିଗୁଡ଼ିକ = ଚାବିଗୁଡ଼ିକ;
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
CopyKeycodeNameLUT(ସଂରଚନା xkb_keymap *keymap, ଚାବି_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        ଯଦି (!match->found)
            ଜାରି;

        ଯଦି (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ଅଚଳ KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            ଯଦି (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        ଚାବି_Name_Text(info->ctx, name),
                        ଚାବି_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                ଜାରି;
            }
            ପ୍ରତିଜ୍ଞା(!match_real.is_alias);
        } ଅନ୍ୟଥା ଯଦି (!match->ଚାବି.ତଳ) {
            /* Update to final index in keymap::keys */
            match->ଚାବି.index += keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->ଅଙ୍କ_ଚାବି_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->ଚାବି_names, ଖାଲି);
    darray_init(info->keycodes.names);
    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
CopyLedNamesToKeymap(ସଂରଚନା xkb_keymap *keymap, ଚାବି_Names_Info *info)
{
    keymap->ଅଙ୍କ_leds = info->ଅଙ୍କ_led_names;
    ପାଇଁ (xkb_led_index_t idx = 0; idx < info->ଅଙ୍କ_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        ଯଦି (ledi->name == XKB_ATOM_NONE)
            ଜାରି;

        keymap->leds[idx].name = ledi->name;
    }

    ଫେରନ୍ତୁ true;
}

ସ୍ଥିର bool
Copy_ଚାବି_Names_Info_To_Keymap(ସଂରଚନା xkb_keymap *keymap, ଚାବି_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    ଯଦି (!Copy_ଚାବି_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        ଫେରନ୍ତୁ false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    ଯଦି (!keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ ||
        keymap->min_ଚାବି_code > 0) {
        keymap->redirect_ଚାବି_auto = 0;
    } ଅନ୍ୟଥା {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        ପାଇଁ (xkb_keycode_t k = keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ; k-- > keymap->ଅଙ୍କ_ଚାବିଗୁଡ଼ିକ_ତଳ;) {
            ଯଦି (keycode > keymap->ଚାବିଗୁଡ଼ିକ[k].keycode)
                ଭାଙ୍ଗ;
            /* Try next keycode downwards */
            keycode = keymap->ଚାବିଗୁଡ଼ିକ[k].keycode - 1;
        }
        keymap->redirect_ଚାବି_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    ଫେରନ୍ତୁ true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, ସଂରଚନା xkb_keymap_info *keymap_info)
{
    ଚାବି_Names_Info info;

    Init_ଚାବି_Names_Info(&info, keymap_info, 0);

    ଯଦି (file != ଖାଲି)
        HandleKeycodesFile(&info, file);

    ଯଦି (info.error_ଗଣନା != 0)
        ଯାଅ err_info;

    ଯଦି (!Copy_ଚାବି_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        ଯାଅ err_info;

    Clear_ଚାବି_Names_Info(&info);
    ଫେରନ୍ତୁ true;

err_info:
    Clear_ଚାବି_Names_Info(&info);
    ଫେରନ୍ତୁ false;
}
