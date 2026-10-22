/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <limits.h>
#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>

#অন্তর্ভাৱ "context.h"
#অন্তর্ভাৱ "messages-codes.h"
#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "xkbcomp-priv.h"
#অন্তর্ভাৱ "atom.h"
#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "text.h"
#অন্তর্ভাৱ "expr.h"
#অন্তর্ভাৱ "include.h"
#অন্তর্ভাৱ "util-mem.h"

প্রকারনির্দেশ সংরচনা {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} উপৰেৰ_Keycode_Entry;

প্রকারনির্দেশ সংরচনা {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) নিচেৰ;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(উপৰেৰ_Keycode_Entry) উপৰেৰ;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

স্থির অন্তঃস্থ শূন্য
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->নিচেৰ);
    darray_init(store->উপৰেৰ);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

স্থির অন্তঃস্থ শূন্য
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->নিচেৰ);
    darray_free(store->উপৰেৰ);
    darray_free(store->names);
}

স্থির অন্তঃস্থ শূন্য
keycode_store_update_চাবি(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    যদি (unlikely(!match.found || match.is_alias)) {
        ঘূৰাই;
    } অন্যথা যদি (match.চাবি.নিচেৰ) {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->নিচেৰ));
        darray_item(store->নিচেৰ, match.চাবি.index) = name;
    } অন্যথা {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->উপৰেৰ));
        darray_item(store->উপৰেৰ, match.চাবি.index).name = name;
    }

    /* Names lookup */
    যদি (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

স্থির bool
keycode_store_ঢোকাও_চাবি(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    যদি (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    যদি (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        যদি (kc >= (xkb_keycode_t) darray_size(store->নিচেৰ))
            darray_resize0(store->নিচেৰ, kc + 1);
        darray_item(store->নিচেৰ, kc) = name;
        যদি (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .চাবি = {
                .found = true,
                .নিচেৰ = true,
                .is_alias = false,
                .index = kc
            }
        };
    } অন্যথা {
        /* High keycode: insert into a sorted list */
        ধ্রুৱ darray_size_t idx = darray_size(store->উপৰেৰ);
        যদি (idx && darray_item(store->উপৰেৰ, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            যেতিয়ালৈ (lower < upper) {
                ধ্রুৱ darray_size_t mid = lower + (upper - 1 - lower) / 2;
                ধ্রুৱ উপৰেৰ_Keycode_Entry * ধ্রুৱ entry =
                    &darray_item(store->উপৰেৰ, mid);
                যদি (entry->keycode < kc) {
                    lower = mid + 1;
                } অন্যথা যদি (entry->keycode > kc) {
                    upper = mid;
                } অন্যথা {
                    /* Unreachable: there is no repetition in the list */
                    প্রতিজ্ঞা(entry->keycode != kc);
                }
            }
            প্রতিজ্ঞা(lower < idx);
            প্রতিজ্ঞা(darray_item(store->উপৰেৰ, lower).keycode > kc);

            /* Update references to entries that will be moved */
            উপৰেৰ_Keycode_Entry *entry;
            darray_foreach_from(entry, store->উপৰেৰ, lower) {
                darray_item(store->names, entry->name).চাবি.index++;
            }

            darray_ঢোকাও(store->উপৰেৰ, lower,
                          (উপৰেৰ_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .চাবি = {
                    .found = true,
                    .নিচেৰ = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } অন্যথা {
            /* Fast path: no need to sort */
            darray_append(store->উপৰেৰ,
                          (উপৰেৰ_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .চাবি = {
                    .found = true,
                    .নিচেৰ = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        যদি (darray_empty(store->নিচেৰ))
            store->min = darray_item(store->উপৰেৰ, 0).keycode;
    }
    ঘূৰাই true;
}

স্থির অন্তঃস্থ bool
keycode_store_ঢোকাও_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    যদি (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    ঘূৰাই true;
}

স্থির অন্তঃস্থ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    ঘূৰাই true;
}

স্থির অন্তঃস্থ শূন্য
keycode_store_delete_name(ধ্রুৱ KeycodeStore *store, xkb_atom_t name)
{
    যদি (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

স্থির শূন্য
keycode_store_delete_চাবি(KeycodeStore *store, ধ্রুৱ KeycodeMatch match)
{
    যদি (unlikely(!match.found || match.is_alias)) {
        ঘূৰাই;
    } অন্যথা যদি (match.চাবি.নিচেৰ) {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->নিচেৰ));
        darray_item(store->names,
                    darray_item(store->নিচেৰ, match.চাবি.index)).found = false;
        যদি (match.চাবি.index + 1u == darray_size(store->নিচেৰ)) {
            /* Highest low keycode: shrink */
            যদি (store->min == match.চাবি.index) {
                /* No low keycode left */
                darray_size(store->নিচেৰ) = 0;
            } অন্যথা {
                /* Look for previous defined low keycode */
                বাবে (darray_size_t idx = match.চাবি.index; idx > 0; idx--) {
                    যদি (darray_item(store->নিচেৰ, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->নিচেৰ) = idx;
                        ভঙ্গ;
                    }
                }
            }
        } অন্যথা {
            /* Lower keycode: reset */
            darray_item(store->নিচেৰ, match.চাবি.index) = XKB_ATOM_NONE;
        }
    } অন্যথা {
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->উপৰেৰ)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->উপৰেৰ, match.চাবি.index).name).found = false;
        darray_remove(store->উপৰেৰ, match.চাবি.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            যদি (entry->found && !entry->is_alias && !entry->চাবি.নিচেৰ &&
                entry->চাবি.index > match.চাবি.index) {
                entry->চাবি.index--;
            }
        }
    }

    /* Update bounds */
    যদি (darray_empty(store->নিচেৰ)) {
        store->min = (darray_empty(store->উপৰেৰ))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->উপৰেৰ, 0).keycode;
    } অন্যথা {
        বাবে (xkb_keycode_t kc = store->min; kc < darray_size(store->নিচেৰ); kc++) {
            যদি (darray_item(store->নিচেৰ, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                ভঙ্গ;
            }
        }
    }
}

স্থির অন্তঃস্থ xkb_keycode_t
keycode_store_get_keycode(ধ্রুৱ KeycodeStore *store, KeycodeMatch match)
{
    যদি (!match.found || match.is_alias) {
        ঘূৰাই XKB_KEYCODE_INVALID;
    } অন্যথা যদি (match.চাবি.নিচেৰ) {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->নিচেৰ));
        ঘূৰাই (xkb_keycode_t) match.চাবি.index;
    } অন্যথা {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->উপৰেৰ));
        ঘূৰাই darray_item(store->উপৰেৰ, match.চাবি.index).keycode;
    }
}

স্থির অন্তঃস্থ xkb_atom_t
keycode_store_get_চাবি_name(ধ্রুৱ KeycodeStore *store, KeycodeMatch match)
{
    যদি (!match.found || match.is_alias) {
        ঘূৰাই XKB_ATOM_NONE;
    } অন্যথা যদি (match.চাবি.নিচেৰ) {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->নিচেৰ));
        ঘূৰাই darray_item(store->নিচেৰ, match.চাবি.index);
    } অন্যথা {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->উপৰেৰ));
        ঘূৰাই darray_item(store->উপৰেৰ, match.চাবি.index).name;
    }
}

স্থির KeycodeMatch
keycode_store_lookup_keycode(ধ্রুৱ KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    যদি (kc < (xkb_keycode_t) darray_size(store->নিচেৰ)) {
        ঘূৰাই (KeycodeMatch) {
            .চাবি = {
                .found = true,
                .নিচেৰ = true,
                .is_alias = false,
                .index = kc
            }
        };
    } অন্যথা যদি (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        ঘূৰাই (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->উপৰেৰ);
    যেতিয়ালৈ (lower < upper) {
        ধ্রুৱ darray_size_t mid = lower + (upper - 1 - lower) / 2;
        উপৰেৰ_Keycode_Entry * ধ্রুৱ entry = &darray_item(store->উপৰেৰ, mid);
        যদি (entry->keycode < kc) {
            lower = mid + 1;
        } অন্যথা যদি (entry->keycode > kc) {
            upper = mid;
        } অন্যথা {
            ঘূৰাই (KeycodeMatch) {
                .চাবি = {
                    .found = true,
                    .নিচেৰ = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    ঘূৰাই (KeycodeMatch) { .found = false };
}

স্থির KeycodeMatch
keycode_store_lookup_name(ধ্রুৱ KeycodeStore *store, xkb_atom_t name)
{
    যদি (name >= darray_size(store->names)) {
        ঘূৰাই (KeycodeMatch) { .found = false, .is_alias = false };
    } অন্যথা {
        ঘূৰাই darray_item(store->names, name);
    }
}

/***====================================================================***/

প্রকারনির্দেশ সংরচনা {
    গণনা merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

প্রকারনির্দেশ সংরচনা {
    ৱর্ণ *name;
    পূর্ণাঙ্ক error_গুনতি;
    unsigned পূর্ণাঙ্ক include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t অঙ্ক_led_names;

    সংরচনা xkb_context *ctx;
    ধ্রুৱ সংরচনা xkb_keymap_info *keymap_info;
} চাবি_Names_Info;

/***====================================================================***/

স্থির LedNameInfo *
FindLedByName(চাবি_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    বাবে (xkb_led_index_t idx = 0; idx < info->অঙ্ক_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        যদি (ledi->name == name) {
            *idx_out = idx;
            ঘূৰাই ledi;
        }
    }

    ঘূৰাই রিক্ত;
}

স্থির bool
AddLedName(চাবি_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    ধ্রুৱ bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    যদি (old) {
        যদি (old_idx == new_idx) {
            যদি (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            ঘূৰাই true;
        }

        যদি (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        যদি (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } অন্যথা {
            ঘূৰাই true;
        }
    }

    যদি (new_idx >= info->অঙ্ক_led_names)
        info->অঙ্ক_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    যদি (old->name != XKB_ATOM_NONE) {
        যদি (report) {
            ধ্রুৱ xkb_atom_t use    = (replace ? new->name : old->name);
            ধ্রুৱ xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        যদি (replace)
            *old = *new;

        ঘূৰাই true;
    }

    *old = *new;
    ঘূৰাই true;
}

স্থির শূন্য
Clear_চাবি_Names_Info(চাবি_Names_Info *info)
{
    মুক্ত(info->name);
    keycode_store_free(&info->keycodes);
}

স্থির শূন্য
Init_চাবি_Names_Info(চাবি_Names_Info *info, ধ্রুৱ সংরচনা xkb_keymap_info *keymap_info,
                 unsigned পূর্ণাঙ্ক include_depth)
{
    memset(info, 0, পরিমাণ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

স্থির bool
Add_চাবি_Name(চাবি_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           গণনা merge_mode merge, bool report)
{
#ifdef NDEBUG
    ধ্রুৱ
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    যদি (match_name.found) {
        ধ্রুৱ bool clobber = (merge != MERGE_AUGMENT);

        যদি (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            যদি (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         চাবি_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            যদি (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } অন্যথা {
                ঘূৰাই true;
            }
        } অন্যথা {
            ধ্রুৱ xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            প্রতিজ্ঞা(old_kc != XKB_KEYCODE_INVALID);
            যদি (old_kc != kc) {
                /* There is already a different key with this name. */

                যদি (report) {
                    ধ্রুৱ xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ধ্রুৱ xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            চাবি_Name_Text(info->ctx, name), use, ignore);
                }

                যদি (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_চাবি(&info->keycodes, match_name);
                } অন্যথা {
                    ঘূৰাই true;
                }
            }
        }
    }

    ধ্রুৱ KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ধ্রুৱ xkb_atom_t old_name = keycode_store_get_চাবি_name(&info->keycodes, match_kc);
    যদি (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        যদি (old_name == name) {
            প্রতিজ্ঞা(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            যদি (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         চাবি_Name_Text(info->ctx, old_name), kc);
            ঘূৰাই true;
        }

        ধ্রুৱ bool clobber = (merge != MERGE_AUGMENT);
        যদি (report) {
            ধ্রুৱ ৱর্ণ* ধ্রুৱ kname     = চাবি_Name_Text(info->ctx, name);
            ধ্রুৱ ৱর্ণ* ধ্রুৱ old_kname = চাবি_Name_Text(info->ctx, old_name);
            ধ্রুৱ ৱর্ণ* ধ্রুৱ use    = (clobber) ? kname : old_kname;
            ধ্রুৱ ৱর্ণ* ধ্রুৱ ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        যদি (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_চাবি(&info->keycodes, match_kc, name);
        }
    } অন্যথা {
        /* No previous keycode */
        যদি (!keycode_store_ঢোকাও_চাবি(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            ঘূৰাই false;
        }
    }

    ঘূৰাই true;
}

/***====================================================================***/

স্থির bool
HandleAliasDef(চাবি_Names_Info *info, ধ্রুৱ চাবি_Alias_Def *def, bool report);

স্থির শূন্য
MergeKeycodeStores(চাবি_Names_Info *into, চাবি_Names_Info *from,
                   গণনা merge_mode merge, bool report)
{
    যদি (darray_empty(into->keycodes.নিচেৰ) && darray_empty(into->keycodes.উপৰেৰ) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.নিচেৰ);
        darray_init(from->keycodes.উপৰেৰ);
        darray_init(from->keycodes.names);
    } অন্যথা {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        বাবে (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.নিচেৰ);
             kc++) {
            /* Skip undefined keycodes */
            ধ্রুৱ xkb_atom_t name = darray_item(from->keycodes.নিচেৰ, kc);
            যদি (name == XKB_ATOM_NONE)
                অনুৱর্ত;

            যদি (!Add_চাবি_Name(into, kc, name, merge, report))
                into->error_গুনতি++;
        }

        /* High keycodes */
        ধ্রুৱ উপৰেৰ_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.উপৰেৰ) {
            প্রতিজ্ঞা(new->name != XKB_ATOM_NONE);
            যদি (!Add_চাবি_Name(into, new->keycode, new->name, merge, report))
                into->error_গুনতি++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            যদি (!match->found || !match->is_alias)
                অনুৱর্ত;

            ধ্রুৱ চাবি_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            যদি (!HandleAliasDef(into, &def, report))
                into->error_গুনতি++;
        }
    }
}

স্থির শূন্য
MergeIncludedKeycodes(চাবি_Names_Info *into, চাবি_Names_Info *from,
                      গণনা merge_mode merge, bool report)
{
    যদি (from->error_গুনতি > 0) {
        into->error_গুনতি += from->error_গুনতি;
        ঘূৰাই;
    }

    যদি (into->name == রিক্ত) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    যদি (into->অঙ্ক_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               পরিমাণ(*from->led_names) * from->অঙ্ক_led_names);
        into->অঙ্ক_led_names = from->অঙ্ক_led_names;
        from->অঙ্ক_led_names = 0;
    }
    অন্যথা {
        বাবে (xkb_led_index_t idx = 0; idx < from->অঙ্ক_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            যদি (ledi->name == XKB_ATOM_NONE)
                অনুৱর্ত;

            ledi->merge = merge;
            যদি (!AddLedName(into, false, ledi, idx, report))
                into->error_গুনতি++;
        }
    }
}

স্থির শূন্য
HandleKeycodesFile(চাবি_Names_Info *info, XkbFile *file);

স্থির bool
HandleIncludeKeycodes(চাবি_Names_Info *info, IncludeStmt *include, bool report)
{
    চাবি_Names_Info included;

    যদি (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_গুনতি += 10;
        ঘূৰাই false;
    }

    Init_চাবি_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ধ্রুৱ সংরচনা parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    বাবে (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        চাবি_Names_Info next_incl;
        XkbFile *file;

        ৱর্ণ path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, পরিমাণ(path));
        যদি (!file) {
            info->error_গুনতি += 10;
            Clear_চাবি_Names_Info(&included);
            ঘূৰাই false;
        }

        Init_চাবি_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_চাবি_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_চাবি_Names_Info(&included);

    ঘূৰাই (info->error_গুনতি == 0);
}

স্থির bool
HandleKeycodeDef(চাবি_Names_Info *info, KeycodeDef *stmt, bool report)
{
    যদি (stmt->মান < 0 || stmt->মান > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->মান, XKB_KEYCODE_MAX);
        ঘূৰাই false;
    }

    ঘূৰাই Add_চাবি_Name(info, (xkb_keycode_t) stmt->মান,
                      stmt->name, stmt->merge, report);
}

স্থির bool
HandleAliasDef(চাবি_Names_Info *info, ধ্রুৱ চাবি_Alias_Def *def, bool report)
{
    ধ্রুৱ KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    যদি (match_name.found) {
        ধ্রুৱ bool clobber = (def->merge != MERGE_AUGMENT);
        যদি (match_name.is_alias) {
            যদি (def->real == match_name.alias.real) {
                যদি (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             চাবি_Name_Text(info->ctx, def->alias),
                             চাবি_Name_Text(info->ctx, def->real));
            } অন্যথা {
                ধ্রুৱ xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                ধ্রুৱ xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                যদি (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             চাবি_Name_Text(info->ctx, def->alias),
                             চাবি_Name_Text(info->ctx, use),
                             চাবি_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            ঘূৰাই true;
        } অন্যথা {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            যদি (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         চাবি_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            যদি (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_চাবি(&info->keycodes, match_name);
            } অন্যথা {
                ঘূৰাই true;
            }
        }
    }

    ঘূৰাই keycode_store_ঢোকাও_alias(&info->keycodes, def->alias, def->real);
}

স্থির bool
Handle_চাবি_Name_Var(চাবি_Names_Info *info, VarDef *stmt)
{
    ধ্রুৱ ৱর্ণ *elem, *field;
    ৰাশিমালা_Def *arrayNdx;

    যদি (!ৰাশিমালা_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        ঘূৰাই false;

    যদি (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        ঘূৰাই !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    যদি (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ঘূৰাই !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    যদি (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        ঘূৰাই !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    যদি (!ৰাশিমালা_Resolve_Integer(info->ctx, stmt->মান, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        ঘূৰাই !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    ঘূৰাই true;
}

স্থির bool
HandleLedNameDef(চাবি_Names_Info *info, LedNameDef *def, bool report)
{
    যদি (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_গুনতি++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        ঘূৰাই false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    যদি (!ৰাশিমালা_Resolve_String(info->ctx, def->name, &name)) {
        ৱর্ণ buf[20];
        snprintf(buf, পরিমাণ(buf), "%"PRId64, def->ndx);
        info->error_গুনতি++;
        ঘূৰাই ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    ঘূৰাই AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

স্থির শূন্য
HandleKeycodesFile(চাবি_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ধ্রুৱ পূর্ণাঙ্ক verbosity = xkb_context_get_log_verbosity(info->ctx);
    ধ্রুৱ bool report_same_file = verbosity > 0;
    ধ্রুৱ bool report_include   = verbosity > 7;

    মুক্ত(info->name);
    info->name = strdup_safe(file->name);

    বাবে (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        ৱিকল্প (stmt->type) {
        পক্ষ STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            ভঙ্গ;
        পক্ষ STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            ভঙ্গ;
        পক্ষ STMT_ALIAS:
            ok = HandleAliasDef(info, (চাবি_Alias_Def *) stmt, report_same_file);
            ভঙ্গ;
        পক্ষ STMT_VAR:
            ok = Handle_চাবি_Name_Var(info, (VarDef *) stmt);
            ভঙ্গ;
        পক্ষ STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            ভঙ্গ;
        পক্ষ STMT_UNKNOWN_DECLARATION:
        পক্ষ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            ভঙ্গ;
        স্ৱভাৱ:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            ভঙ্গ;
        }

        যদি (!ok)
            info->error_গুনতি++;

        যদি (info->error_গুনতি > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            ভঙ্গ;
        }
    }
}

/***====================================================================***/

স্থির bool
Copy_চাবি_Names_To_Keymap(সংরচনা xkb_keymap *keymap, চাবি_Names_Info *info)
{
    যদি (darray_empty(info->keycodes.নিচেৰ) && darray_empty(info->keycodes.উপৰেৰ)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        প্রতিজ্ঞা(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_চাবি_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_চাবি_code = 255;
        keymap->অঙ্ক_চাবিগুলি = keymap->অঙ্ক_চাবিগুলি_নিচেৰ = keymap->max_চাবি_code + 1;
    } অন্যথা {
        প্রতিজ্ঞা(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_চাবি_code = info->keycodes.min;
        keymap->max_চাবি_code = (darray_empty(info->keycodes.উপৰেৰ))
            ? darray_size(info->keycodes.নিচেৰ) - 1
            : darray_item(
                info->keycodes.উপৰেৰ,
                darray_size(info->keycodes.উপৰেৰ) - 1
            ).keycode;
        keymap->অঙ্ক_চাবিগুলি_নিচেৰ = darray_size(info->keycodes.নিচেৰ);
        keymap->অঙ্ক_চাবিগুলি = keymap->অঙ্ক_চাবিগুলি_নিচেৰ
                         + darray_size(info->keycodes.উপৰেৰ);
    }

    সংরচনা xkb_চাবি * ধ্রুৱ চাবিগুলি = calloc(keymap->অঙ্ক_চাবিগুলি, পরিমাণ(*চাবিগুলি));
    যদি (!চাবিগুলি) {
        keymap->অঙ্ক_চাবিগুলি = 0;
        keymap->min_চাবি_code = keymap->max_চাবি_code = XKB_KEYCODE_INVALID;
        ঘূৰাই false;
    }

    /* Low keycodes */
    বাবে (xkb_keycode_t kc = keymap->min_চাবি_code; kc < keymap->অঙ্ক_চাবিগুলি_নিচেৰ; kc++)
        চাবিগুলি[kc].keycode = kc;

    বাবে (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.নিচেৰ);
         kc++)
        চাবিগুলি[kc].name = darray_item(info->keycodes.নিচেৰ, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->অঙ্ক_চাবিগুলি_নিচেৰ; /* First high keycode index */
    ধ্রুৱ উপৰেৰ_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.উপৰেৰ) {
        প্রতিজ্ঞা(entry->name != XKB_ATOM_NONE);
        চাবিগুলি[idx].keycode = entry->keycode;
        চাবিগুলি[idx].name = entry->name;
        idx++;
    }

    keymap->চাবিগুলি = চাবিগুলি;
    ঘূৰাই true;
}

স্থির bool
CopyKeycodeNameLUT(সংরচনা xkb_keymap *keymap, চাবি_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        যদি (!match->found)
            অনুৱর্ত;

        যদি (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ধ্রুৱ KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            যদি (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        চাবি_Name_Text(info->ctx, name),
                        চাবি_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                অনুৱর্ত;
            }
            প্রতিজ্ঞা(!match_real.is_alias);
        } অন্যথা যদি (!match->চাবি.নিচেৰ) {
            /* Update to final index in keymap::keys */
            match->চাবি.index += keymap->অঙ্ক_চাবিগুলি_নিচেৰ;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->অঙ্ক_চাবি_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->চাবি_names, রিক্ত);
    darray_init(info->keycodes.names);
    ঘূৰাই true;
}

স্থির bool
CopyLedNamesToKeymap(সংরচনা xkb_keymap *keymap, চাবি_Names_Info *info)
{
    keymap->অঙ্ক_leds = info->অঙ্ক_led_names;
    বাবে (xkb_led_index_t idx = 0; idx < info->অঙ্ক_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        যদি (ledi->name == XKB_ATOM_NONE)
            অনুৱর্ত;

        keymap->leds[idx].name = ledi->name;
    }

    ঘূৰাই true;
}

স্থির bool
Copy_চাবি_Names_Info_To_Keymap(সংরচনা xkb_keymap *keymap, চাবি_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    যদি (!Copy_চাবি_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        ঘূৰাই false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    যদি (!keymap->অঙ্ক_চাবিগুলি ||
        keymap->min_চাবি_code > 0) {
        keymap->redirect_চাবি_auto = 0;
    } অন্যথা {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        বাবে (xkb_keycode_t k = keymap->অঙ্ক_চাবিগুলি; k-- > keymap->অঙ্ক_চাবিগুলি_নিচেৰ;) {
            যদি (keycode > keymap->চাবিগুলি[k].keycode)
                ভঙ্গ;
            /* Try next keycode downwards */
            keycode = keymap->চাবিগুলি[k].keycode - 1;
        }
        keymap->redirect_চাবি_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    ঘূৰাই true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, সংরচনা xkb_keymap_info *keymap_info)
{
    চাবি_Names_Info info;

    Init_চাবি_Names_Info(&info, keymap_info, 0);

    যদি (file != রিক্ত)
        HandleKeycodesFile(&info, file);

    যদি (info.error_গুনতি != 0)
        গচ্ছ err_info;

    যদি (!Copy_চাবি_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        গচ্ছ err_info;

    Clear_চাবি_Names_Info(&info);
    ঘূৰাই true;

err_info:
    Clear_চাবি_Names_Info(&info);
    ঘূৰাই false;
}
