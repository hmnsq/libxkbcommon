/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#অন্তর্ভাব "config.h"

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব "context.h"
#অন্তর্ভাব "messages-codes.h"
#অন্তর্ভাব "xkbcommon/xkbcommon.h"
#অন্তর্ভাব "xkbcomp-priv.h"
#অন্তর্ভাব "atom.h"
#অন্তর্ভাব "darray.h"
#অন্তর্ভাব "keymap.h"
#অন্তর্ভাব "text.h"
#অন্তর্ভাব "expr.h"
#অন্তর্ভাব "include.h"
#অন্তর্ভাব "util-mem.h"

প্রকারনির্দেশ সংরচনা {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} উপরের_Keycode_Entry;

প্রকারনির্দেশ সংরচনা {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) নিচের;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(উপরের_Keycode_Entry) উপরের;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

স্থির অন্তঃস্থ শূন্য
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->নিচের);
    darray_init(store->উপরের);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

স্থির অন্তঃস্থ শূন্য
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->নিচের);
    darray_free(store->উপরের);
    darray_free(store->names);
}

স্থির অন্তঃস্থ শূন্য
keycode_store_update_চাবি(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    যদি (unlikely(!match.found || match.is_alias)) {
        প্রত্যাগম;
    } অন্যথা যদি (match.চাবি.নিচের) {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->নিচের));
        darray_item(store->নিচের, match.চাবি.index) = name;
    } অন্যথা {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->উপরের));
        darray_item(store->উপরের, match.চাবি.index).name = name;
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
        যদি (kc >= (xkb_keycode_t) darray_size(store->নিচের))
            darray_resize0(store->নিচের, kc + 1);
        darray_item(store->নিচের, kc) = name;
        যদি (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .চাবি = {
                .found = true,
                .নিচের = true,
                .is_alias = false,
                .index = kc
            }
        };
    } অন্যথা {
        /* High keycode: insert into a sorted list */
        ধ্রুব darray_size_t idx = darray_size(store->উপরের);
        যদি (idx && darray_item(store->উপরের, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            যাবত্ (lower < upper) {
                ধ্রুব darray_size_t mid = lower + (upper - 1 - lower) / 2;
                ধ্রুব উপরের_Keycode_Entry * ধ্রুব entry =
                    &darray_item(store->উপরের, mid);
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
            প্রতিজ্ঞা(darray_item(store->উপরের, lower).keycode > kc);

            /* Update references to entries that will be moved */
            উপরের_Keycode_Entry *entry;
            darray_foreach_from(entry, store->উপরের, lower) {
                darray_item(store->names, entry->name).চাবি.index++;
            }

            darray_ঢোকাও(store->উপরের, lower,
                          (উপরের_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .চাবি = {
                    .found = true,
                    .নিচের = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } অন্যথা {
            /* Fast path: no need to sort */
            darray_append(store->উপরের,
                          (উপরের_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .চাবি = {
                    .found = true,
                    .নিচের = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        যদি (darray_empty(store->নিচের))
            store->min = darray_item(store->উপরের, 0).keycode;
    }
    প্রত্যাগম true;
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
    প্রত্যাগম true;
}

স্থির অন্তঃস্থ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    প্রত্যাগম true;
}

স্থির অন্তঃস্থ শূন্য
keycode_store_delete_name(ধ্রুব KeycodeStore *store, xkb_atom_t name)
{
    যদি (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

স্থির শূন্য
keycode_store_delete_চাবি(KeycodeStore *store, ধ্রুব KeycodeMatch match)
{
    যদি (unlikely(!match.found || match.is_alias)) {
        প্রত্যাগম;
    } অন্যথা যদি (match.চাবি.নিচের) {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->নিচের));
        darray_item(store->names,
                    darray_item(store->নিচের, match.চাবি.index)).found = false;
        যদি (match.চাবি.index + 1u == darray_size(store->নিচের)) {
            /* Highest low keycode: shrink */
            যদি (store->min == match.চাবি.index) {
                /* No low keycode left */
                darray_size(store->নিচের) = 0;
            } অন্যথা {
                /* Look for previous defined low keycode */
                প্রতি (darray_size_t idx = match.চাবি.index; idx > 0; idx--) {
                    যদি (darray_item(store->নিচের, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->নিচের) = idx;
                        ভঙ্গ;
                    }
                }
            }
        } অন্যথা {
            /* Lower keycode: reset */
            darray_item(store->নিচের, match.চাবি.index) = XKB_ATOM_NONE;
        }
    } অন্যথা {
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->উপরের)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->উপরের, match.চাবি.index).name).found = false;
        darray_remove(store->উপরের, match.চাবি.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            যদি (entry->found && !entry->is_alias && !entry->চাবি.নিচের &&
                entry->চাবি.index > match.চাবি.index) {
                entry->চাবি.index--;
            }
        }
    }

    /* Update bounds */
    যদি (darray_empty(store->নিচের)) {
        store->min = (darray_empty(store->উপরের))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->উপরের, 0).keycode;
    } অন্যথা {
        প্রতি (xkb_keycode_t kc = store->min; kc < darray_size(store->নিচের); kc++) {
            যদি (darray_item(store->নিচের, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                ভঙ্গ;
            }
        }
    }
}

স্থির অন্তঃস্থ xkb_keycode_t
keycode_store_get_keycode(ধ্রুব KeycodeStore *store, KeycodeMatch match)
{
    যদি (!match.found || match.is_alias) {
        প্রত্যাগম XKB_KEYCODE_INVALID;
    } অন্যথা যদি (match.চাবি.নিচের) {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->নিচের));
        প্রত্যাগম (xkb_keycode_t) match.চাবি.index;
    } অন্যথা {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->উপরের));
        প্রত্যাগম darray_item(store->উপরের, match.চাবি.index).keycode;
    }
}

স্থির অন্তঃস্থ xkb_atom_t
keycode_store_get_চাবি_name(ধ্রুব KeycodeStore *store, KeycodeMatch match)
{
    যদি (!match.found || match.is_alias) {
        প্রত্যাগম XKB_ATOM_NONE;
    } অন্যথা যদি (match.চাবি.নিচের) {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->নিচের));
        প্রত্যাগম darray_item(store->নিচের, match.চাবি.index);
    } অন্যথা {
        /* assume valid index */
        প্রতিজ্ঞা(match.চাবি.index < darray_size(store->উপরের));
        প্রত্যাগম darray_item(store->উপরের, match.চাবি.index).name;
    }
}

স্থির KeycodeMatch
keycode_store_lookup_keycode(ধ্রুব KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    যদি (kc < (xkb_keycode_t) darray_size(store->নিচের)) {
        প্রত্যাগম (KeycodeMatch) {
            .চাবি = {
                .found = true,
                .নিচের = true,
                .is_alias = false,
                .index = kc
            }
        };
    } অন্যথা যদি (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        প্রত্যাগম (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->উপরের);
    যাবত্ (lower < upper) {
        ধ্রুব darray_size_t mid = lower + (upper - 1 - lower) / 2;
        উপরের_Keycode_Entry * ধ্রুব entry = &darray_item(store->উপরের, mid);
        যদি (entry->keycode < kc) {
            lower = mid + 1;
        } অন্যথা যদি (entry->keycode > kc) {
            upper = mid;
        } অন্যথা {
            প্রত্যাগম (KeycodeMatch) {
                .চাবি = {
                    .found = true,
                    .নিচের = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    প্রত্যাগম (KeycodeMatch) { .found = false };
}

স্থির KeycodeMatch
keycode_store_lookup_name(ধ্রুব KeycodeStore *store, xkb_atom_t name)
{
    যদি (name >= darray_size(store->names)) {
        প্রত্যাগম (KeycodeMatch) { .found = false, .is_alias = false };
    } অন্যথা {
        প্রত্যাগম darray_item(store->names, name);
    }
}

/***====================================================================***/

প্রকারনির্দেশ সংরচনা {
    গণনা merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

প্রকারনির্দেশ সংরচনা {
    বর্ণ *name;
    পূর্ণাঙ্ক error_গুনতি;
    unsigned পূর্ণাঙ্ক include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t অঙ্ক_led_names;

    সংরচনা xkb_context *ctx;
    ধ্রুব সংরচনা xkb_keymap_info *keymap_info;
} চাবি_Names_Info;

/***====================================================================***/

স্থির LedNameInfo *
FindLedByName(চাবি_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    প্রতি (xkb_led_index_t idx = 0; idx < info->অঙ্ক_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        যদি (ledi->name == name) {
            *idx_out = idx;
            প্রত্যাগম ledi;
        }
    }

    প্রত্যাগম রিক্ত;
}

স্থির bool
AddLedName(চাবি_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    ধ্রুব bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    যদি (old) {
        যদি (old_idx == new_idx) {
            যদি (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            প্রত্যাগম true;
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
            প্রত্যাগম true;
        }
    }

    যদি (new_idx >= info->অঙ্ক_led_names)
        info->অঙ্ক_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    যদি (old->name != XKB_ATOM_NONE) {
        যদি (report) {
            ধ্রুব xkb_atom_t use    = (replace ? new->name : old->name);
            ধ্রুব xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        যদি (replace)
            *old = *new;

        প্রত্যাগম true;
    }

    *old = *new;
    প্রত্যাগম true;
}

স্থির শূন্য
Clear_চাবি_Names_Info(চাবি_Names_Info *info)
{
    মুক্ত(info->name);
    keycode_store_free(&info->keycodes);
}

স্থির শূন্য
Init_চাবি_Names_Info(চাবি_Names_Info *info, ধ্রুব সংরচনা xkb_keymap_info *keymap_info,
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
    ধ্রুব
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    যদি (match_name.found) {
        ধ্রুব bool clobber = (merge != MERGE_AUGMENT);

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
                প্রত্যাগম true;
            }
        } অন্যথা {
            ধ্রুব xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            প্রতিজ্ঞা(old_kc != XKB_KEYCODE_INVALID);
            যদি (old_kc != kc) {
                /* There is already a different key with this name. */

                যদি (report) {
                    ধ্রুব xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ধ্রুব xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            চাবি_Name_Text(info->ctx, name), use, ignore);
                }

                যদি (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_চাবি(&info->keycodes, match_name);
                } অন্যথা {
                    প্রত্যাগম true;
                }
            }
        }
    }

    ধ্রুব KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ধ্রুব xkb_atom_t old_name = keycode_store_get_চাবি_name(&info->keycodes, match_kc);
    যদি (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        যদি (old_name == name) {
            প্রতিজ্ঞা(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            যদি (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         চাবি_Name_Text(info->ctx, old_name), kc);
            প্রত্যাগম true;
        }

        ধ্রুব bool clobber = (merge != MERGE_AUGMENT);
        যদি (report) {
            ধ্রুব বর্ণ* ধ্রুব kname     = চাবি_Name_Text(info->ctx, name);
            ধ্রুব বর্ণ* ধ্রুব old_kname = চাবি_Name_Text(info->ctx, old_name);
            ধ্রুব বর্ণ* ধ্রুব use    = (clobber) ? kname : old_kname;
            ধ্রুব বর্ণ* ধ্রুব ignore = (clobber) ? old_kname : kname;
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
            প্রত্যাগম false;
        }
    }

    প্রত্যাগম true;
}

/***====================================================================***/

স্থির bool
HandleAliasDef(চাবি_Names_Info *info, ধ্রুব চাবি_Alias_Def *def, bool report);

স্থির শূন্য
MergeKeycodeStores(চাবি_Names_Info *into, চাবি_Names_Info *from,
                   গণনা merge_mode merge, bool report)
{
    যদি (darray_empty(into->keycodes.নিচের) && darray_empty(into->keycodes.উপরের) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.নিচের);
        darray_init(from->keycodes.উপরের);
        darray_init(from->keycodes.names);
    } অন্যথা {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        প্রতি (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.নিচের);
             kc++) {
            /* Skip undefined keycodes */
            ধ্রুব xkb_atom_t name = darray_item(from->keycodes.নিচের, kc);
            যদি (name == XKB_ATOM_NONE)
                অনুবর্ত;

            যদি (!Add_চাবি_Name(into, kc, name, merge, report))
                into->error_গুনতি++;
        }

        /* High keycodes */
        ধ্রুব উপরের_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.উপরের) {
            প্রতিজ্ঞা(new->name != XKB_ATOM_NONE);
            যদি (!Add_চাবি_Name(into, new->keycode, new->name, merge, report))
                into->error_গুনতি++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            যদি (!match->found || !match->is_alias)
                অনুবর্ত;

            ধ্রুব চাবি_Alias_Def def = {
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
        প্রত্যাগম;
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
        প্রতি (xkb_led_index_t idx = 0; idx < from->অঙ্ক_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            যদি (ledi->name == XKB_ATOM_NONE)
                অনুবর্ত;

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
        প্রত্যাগম false;
    }

    Init_চাবি_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ধ্রুব সংরচনা parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    প্রতি (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        চাবি_Names_Info next_incl;
        XkbFile *file;

        বর্ণ path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, পরিমাণ(path));
        যদি (!file) {
            info->error_গুনতি += 10;
            Clear_চাবি_Names_Info(&included);
            প্রত্যাগম false;
        }

        Init_চাবি_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_চাবি_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_চাবি_Names_Info(&included);

    প্রত্যাগম (info->error_গুনতি == 0);
}

স্থির bool
HandleKeycodeDef(চাবি_Names_Info *info, KeycodeDef *stmt, bool report)
{
    যদি (stmt->মান < 0 || stmt->মান > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->মান, XKB_KEYCODE_MAX);
        প্রত্যাগম false;
    }

    প্রত্যাগম Add_চাবি_Name(info, (xkb_keycode_t) stmt->মান,
                      stmt->name, stmt->merge, report);
}

স্থির bool
HandleAliasDef(চাবি_Names_Info *info, ধ্রুব চাবি_Alias_Def *def, bool report)
{
    ধ্রুব KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    যদি (match_name.found) {
        ধ্রুব bool clobber = (def->merge != MERGE_AUGMENT);
        যদি (match_name.is_alias) {
            যদি (def->real == match_name.alias.real) {
                যদি (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_চাবি_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             চাবি_Name_Text(info->ctx, def->alias),
                             চাবি_Name_Text(info->ctx, def->real));
            } অন্যথা {
                ধ্রুব xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                ধ্রুব xkb_atom_t ignore = (clobber)
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
            প্রত্যাগম true;
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
                প্রত্যাগম true;
            }
        }
    }

    প্রত্যাগম keycode_store_ঢোকাও_alias(&info->keycodes, def->alias, def->real);
}

স্থির bool
Handle_চাবি_Name_Var(চাবি_Names_Info *info, VarDef *stmt)
{
    ধ্রুব বর্ণ *elem, *field;
    রাশিমালা_Def *arrayNdx;

    যদি (!রাশিমালা_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        প্রত্যাগম false;

    যদি (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        প্রত্যাগম !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    যদি (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        প্রত্যাগম !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    যদি (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        প্রত্যাগম !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    যদি (!রাশিমালা_Resolve_Integer(info->ctx, stmt->মান, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        প্রত্যাগম !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    প্রত্যাগম true;
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
        প্রত্যাগম false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    যদি (!রাশিমালা_Resolve_String(info->ctx, def->name, &name)) {
        বর্ণ buf[20];
        snprintf(buf, পরিমাণ(buf), "%"PRId64, def->ndx);
        info->error_গুনতি++;
        প্রত্যাগম ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    প্রত্যাগম AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

স্থির শূন্য
HandleKeycodesFile(চাবি_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ধ্রুব পূর্ণাঙ্ক verbosity = xkb_context_get_log_verbosity(info->ctx);
    ধ্রুব bool report_same_file = verbosity > 0;
    ধ্রুব bool report_include   = verbosity > 7;

    মুক্ত(info->name);
    info->name = strdup_safe(file->name);

    প্রতি (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        বিকল্প (stmt->type) {
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
        স্বভাব:
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
    যদি (darray_empty(info->keycodes.নিচের) && darray_empty(info->keycodes.উপরের)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        প্রতিজ্ঞা(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_চাবি_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_চাবি_code = 255;
        keymap->অঙ্ক_চাবিগুলি = keymap->অঙ্ক_চাবিগুলি_নিচের = keymap->max_চাবি_code + 1;
    } অন্যথা {
        প্রতিজ্ঞা(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_চাবি_code = info->keycodes.min;
        keymap->max_চাবি_code = (darray_empty(info->keycodes.উপরের))
            ? darray_size(info->keycodes.নিচের) - 1
            : darray_item(
                info->keycodes.উপরের,
                darray_size(info->keycodes.উপরের) - 1
            ).keycode;
        keymap->অঙ্ক_চাবিগুলি_নিচের = darray_size(info->keycodes.নিচের);
        keymap->অঙ্ক_চাবিগুলি = keymap->অঙ্ক_চাবিগুলি_নিচের
                         + darray_size(info->keycodes.উপরের);
    }

    সংরচনা xkb_চাবি * ধ্রুব চাবিগুলি = calloc(keymap->অঙ্ক_চাবিগুলি, পরিমাণ(*চাবিগুলি));
    যদি (!চাবিগুলি) {
        keymap->অঙ্ক_চাবিগুলি = 0;
        keymap->min_চাবি_code = keymap->max_চাবি_code = XKB_KEYCODE_INVALID;
        প্রত্যাগম false;
    }

    /* Low keycodes */
    প্রতি (xkb_keycode_t kc = keymap->min_চাবি_code; kc < keymap->অঙ্ক_চাবিগুলি_নিচের; kc++)
        চাবিগুলি[kc].keycode = kc;

    প্রতি (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.নিচের);
         kc++)
        চাবিগুলি[kc].name = darray_item(info->keycodes.নিচের, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->অঙ্ক_চাবিগুলি_নিচের; /* First high keycode index */
    ধ্রুব উপরের_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.উপরের) {
        প্রতিজ্ঞা(entry->name != XKB_ATOM_NONE);
        চাবিগুলি[idx].keycode = entry->keycode;
        চাবিগুলি[idx].name = entry->name;
        idx++;
    }

    keymap->চাবিগুলি = চাবিগুলি;
    প্রত্যাগম true;
}

স্থির bool
CopyKeycodeNameLUT(সংরচনা xkb_keymap *keymap, চাবি_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        যদি (!match->found)
            অনুবর্ত;

        যদি (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ধ্রুব KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            যদি (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        চাবি_Name_Text(info->ctx, name),
                        চাবি_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                অনুবর্ত;
            }
            প্রতিজ্ঞা(!match_real.is_alias);
        } অন্যথা যদি (!match->চাবি.নিচের) {
            /* Update to final index in keymap::keys */
            match->চাবি.index += keymap->অঙ্ক_চাবিগুলি_নিচের;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->অঙ্ক_চাবি_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->চাবি_names, রিক্ত);
    darray_init(info->keycodes.names);
    প্রত্যাগম true;
}

স্থির bool
CopyLedNamesToKeymap(সংরচনা xkb_keymap *keymap, চাবি_Names_Info *info)
{
    keymap->অঙ্ক_leds = info->অঙ্ক_led_names;
    প্রতি (xkb_led_index_t idx = 0; idx < info->অঙ্ক_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        যদি (ledi->name == XKB_ATOM_NONE)
            অনুবর্ত;

        keymap->leds[idx].name = ledi->name;
    }

    প্রত্যাগম true;
}

স্থির bool
Copy_চাবি_Names_Info_To_Keymap(সংরচনা xkb_keymap *keymap, চাবি_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    যদি (!Copy_চাবি_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        প্রত্যাগম false;

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
        প্রতি (xkb_keycode_t k = keymap->অঙ্ক_চাবিগুলি; k-- > keymap->অঙ্ক_চাবিগুলি_নিচের;) {
            যদি (keycode > keymap->চাবিগুলি[k].keycode)
                ভঙ্গ;
            /* Try next keycode downwards */
            keycode = keymap->চাবিগুলি[k].keycode - 1;
        }
        keymap->redirect_চাবি_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    প্রত্যাগম true;
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
    প্রত্যাগম true;

err_info:
    Clear_চাবি_Names_Info(&info);
    প্রত্যাগম false;
}
