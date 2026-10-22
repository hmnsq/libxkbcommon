/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <limits.h>
#ضمن <stdint.h>
#ضمن <stdlib.h>
#ضمن <string.h>

#ضمن "context.h"
#ضمن "messages-codes.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "xkbcomp-priv.h"
#ضمن "atom.h"
#ضمن "darray.h"
#ضمن "keymap.h"
#ضمن "text.h"
#ضمن "expr.h"
#ضمن "include.h"
#ضمن "util-mem.h"

تعريفنوع بنية {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} HighKeycodeEntry;

تعريفنوع بنية {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) low;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(HighKeycodeEntry) high;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

ساكن مضمن فراغ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->low);
    darray_init(store->high);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ساكن مضمن فراغ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->low);
    darray_free(store->high);
    darray_free(store->names);
}

ساكن مضمن فراغ
keycode_store_update_key(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    ܐܢ (unlikely(!match.found || match.is_alias)) {
        ܦܢܐ;
    } ܐܚܪܢܐ ܐܢ (match.key.low) {
        /* assume valid index */
        أكد(match.key.index < darray_size(store->low));
        darray_item(store->low, match.key.index) = name;
    } ܐܚܪܢܐ {
        /* assume valid index */
        أكد(match.key.index < darray_size(store->high));
        darray_item(store->high, match.key.index).name = name;
    }

    /* Names lookup */
    ܐܢ (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

ساكن bool
keycode_store_insert_key(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    ܐܢ (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    ܐܢ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        ܐܢ (kc >= (xkb_keycode_t) darray_size(store->low))
            darray_resize0(store->low, kc + 1);
        darray_item(store->low, kc) = name;
        ܐܢ (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .key = {
                .found = true,
                .low = true,
                .is_alias = false,
                .index = kc
            }
        };
    } ܐܚܪܢܐ {
        /* High keycode: insert into a sorted list */
        ثابت darray_size_t idx = darray_size(store->high);
        ܐܢ (idx && darray_item(store->high, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            ܟܡܐ (lower < upper) {
                ثابت darray_size_t mid = lower + (upper - 1 - lower) / 2;
                ثابت HighKeycodeEntry * ثابت entry =
                    &darray_item(store->high, mid);
                ܐܢ (entry->keycode < kc) {
                    lower = mid + 1;
                } ܐܚܪܢܐ ܐܢ (entry->keycode > kc) {
                    upper = mid;
                } ܐܚܪܢܐ {
                    /* Unreachable: there is no repetition in the list */
                    أكد(entry->keycode != kc);
                }
            }
            أكد(lower < idx);
            أكد(darray_item(store->high, lower).keycode > kc);

            /* Update references to entries that will be moved */
            HighKeycodeEntry *entry;
            darray_foreach_from(entry, store->high, lower) {
                darray_item(store->names, entry->name).key.index++;
            }

            darray_insert(store->high, lower,
                          (HighKeycodeEntry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .key = {
                    .found = true,
                    .low = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } ܐܚܪܢܐ {
            /* Fast path: no need to sort */
            darray_append(store->high,
                          (HighKeycodeEntry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .key = {
                    .found = true,
                    .low = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        ܐܢ (darray_empty(store->low))
            store->min = darray_item(store->high, 0).keycode;
    }
    ܦܢܐ true;
}

ساكن مضمن bool
keycode_store_insert_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    ܐܢ (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    ܦܢܐ true;
}

ساكن مضمن bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    ܦܢܐ true;
}

ساكن مضمن فراغ
keycode_store_delete_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    ܐܢ (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

ساكن فراغ
keycode_store_delete_key(KeycodeStore *store, ثابت KeycodeMatch match)
{
    ܐܢ (unlikely(!match.found || match.is_alias)) {
        ܦܢܐ;
    } ܐܚܪܢܐ ܐܢ (match.key.low) {
        /* assume valid index */
        أكد(match.key.index < darray_size(store->low));
        darray_item(store->names,
                    darray_item(store->low, match.key.index)).found = false;
        ܐܢ (match.key.index + 1u == darray_size(store->low)) {
            /* Highest low keycode: shrink */
            ܐܢ (store->min == match.key.index) {
                /* No low keycode left */
                darray_size(store->low) = 0;
            } ܐܚܪܢܐ {
                /* Look for previous defined low keycode */
                لكل (darray_size_t idx = match.key.index; idx > 0; idx--) {
                    ܐܢ (darray_item(store->low, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->low) = idx;
                        اكسر;
                    }
                }
            }
        } ܐܚܪܢܐ {
            /* Lower keycode: reset */
            darray_item(store->low, match.key.index) = XKB_ATOM_NONE;
        }
    } ܐܚܪܢܐ {
        أكد(match.key.index < darray_size(store->high)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->high, match.key.index).name).found = false;
        darray_remove(store->high, match.key.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            ܐܢ (entry->found && !entry->is_alias && !entry->key.low &&
                entry->key.index > match.key.index) {
                entry->key.index--;
            }
        }
    }

    /* Update bounds */
    ܐܢ (darray_empty(store->low)) {
        store->min = (darray_empty(store->high))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->high, 0).keycode;
    } ܐܚܪܢܐ {
        لكل (xkb_keycode_t kc = store->min; kc < darray_size(store->low); kc++) {
            ܐܢ (darray_item(store->low, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                اكسر;
            }
        }
    }
}

ساكن مضمن xkb_keycode_t
keycode_store_get_keycode(ثابت KeycodeStore *store, KeycodeMatch match)
{
    ܐܢ (!match.found || match.is_alias) {
        ܦܢܐ XKB_KEYCODE_INVALID;
    } ܐܚܪܢܐ ܐܢ (match.key.low) {
        /* assume valid index */
        أكد(match.key.index < darray_size(store->low));
        ܦܢܐ (xkb_keycode_t) match.key.index;
    } ܐܚܪܢܐ {
        /* assume valid index */
        أكد(match.key.index < darray_size(store->high));
        ܦܢܐ darray_item(store->high, match.key.index).keycode;
    }
}

ساكن مضمن xkb_atom_t
keycode_store_get_key_name(ثابت KeycodeStore *store, KeycodeMatch match)
{
    ܐܢ (!match.found || match.is_alias) {
        ܦܢܐ XKB_ATOM_NONE;
    } ܐܚܪܢܐ ܐܢ (match.key.low) {
        /* assume valid index */
        أكد(match.key.index < darray_size(store->low));
        ܦܢܐ darray_item(store->low, match.key.index);
    } ܐܚܪܢܐ {
        /* assume valid index */
        أكد(match.key.index < darray_size(store->high));
        ܦܢܐ darray_item(store->high, match.key.index).name;
    }
}

ساكن KeycodeMatch
keycode_store_lookup_keycode(ثابت KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    ܐܢ (kc < (xkb_keycode_t) darray_size(store->low)) {
        ܦܢܐ (KeycodeMatch) {
            .key = {
                .found = true,
                .low = true,
                .is_alias = false,
                .index = kc
            }
        };
    } ܐܚܪܢܐ ܐܢ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        ܦܢܐ (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->high);
    ܟܡܐ (lower < upper) {
        ثابت darray_size_t mid = lower + (upper - 1 - lower) / 2;
        HighKeycodeEntry * ثابت entry = &darray_item(store->high, mid);
        ܐܢ (entry->keycode < kc) {
            lower = mid + 1;
        } ܐܚܪܢܐ ܐܢ (entry->keycode > kc) {
            upper = mid;
        } ܐܚܪܢܐ {
            ܦܢܐ (KeycodeMatch) {
                .key = {
                    .found = true,
                    .low = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    ܦܢܐ (KeycodeMatch) { .found = false };
}

ساكن KeycodeMatch
keycode_store_lookup_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    ܐܢ (name >= darray_size(store->names)) {
        ܦܢܐ (KeycodeMatch) { .found = false, .is_alias = false };
    } ܐܚܪܢܐ {
        ܦܢܐ darray_item(store->names, name);
    }
}

/***====================================================================***/

تعريفنوع بنية {
    تعداد merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

تعريفنوع بنية {
    حرف *name;
    صحيح errorCount;
    unsigned صحيح include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t num_led_names;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} KeyNamesInfo;

/***====================================================================***/

ساكن LedNameInfo *
FindLedByName(KeyNamesInfo *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    لكل (xkb_led_index_t idx = 0; idx < info->num_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        ܐܢ (ledi->name == name) {
            *idx_out = idx;
            ܦܢܐ ledi;
        }
    }

    ܦܢܐ عدم;
}

ساكن bool
AddLedName(KeyNamesInfo *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    ثابت bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    ܐܢ (old) {
        ܐܢ (old_idx == new_idx) {
            ܐܢ (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            ܦܢܐ true;
        }

        ܐܢ (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        ܐܢ (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } ܐܚܪܢܐ {
            ܦܢܐ true;
        }
    }

    ܐܢ (new_idx >= info->num_led_names)
        info->num_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    ܐܢ (old->name != XKB_ATOM_NONE) {
        ܐܢ (report) {
            ثابت xkb_atom_t use    = (replace ? new->name : old->name);
            ثابت xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        ܐܢ (replace)
            *old = *new;

        ܦܢܐ true;
    }

    *old = *new;
    ܦܢܐ true;
}

ساكن فراغ
ClearKeyNamesInfo(KeyNamesInfo *info)
{
    حرر(info->name);
    keycode_store_free(&info->keycodes);
}

ساكن فراغ
InitKeyNamesInfo(KeyNamesInfo *info, ثابت بنية xkb_keymap_info *keymap_info,
                 unsigned صحيح include_depth)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

ساكن bool
AddKeyName(KeyNamesInfo *info, xkb_keycode_t kc, xkb_atom_t name,
           تعداد merge_mode merge, bool report)
{
#ifdef NDEBUG
    ثابت
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    ܐܢ (match_name.found) {
        ثابت bool clobber = (merge != MERGE_AUGMENT);

        ܐܢ (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            ܐܢ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         KeyNameText(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            ܐܢ (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } ܐܚܪܢܐ {
                ܦܢܐ true;
            }
        } ܐܚܪܢܐ {
            ثابت xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            أكد(old_kc != XKB_KEYCODE_INVALID);
            ܐܢ (old_kc != kc) {
                /* There is already a different key with this name. */

                ܐܢ (report) {
                    ثابت xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ثابت xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            KeyNameText(info->ctx, name), use, ignore);
                }

                ܐܢ (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_key(&info->keycodes, match_name);
                } ܐܚܪܢܐ {
                    ܦܢܐ true;
                }
            }
        }
    }

    ثابت KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ثابت xkb_atom_t old_name = keycode_store_get_key_name(&info->keycodes, match_kc);
    ܐܢ (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        ܐܢ (old_name == name) {
            أكد(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            ܐܢ (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         KeyNameText(info->ctx, old_name), kc);
            ܦܢܐ true;
        }

        ثابت bool clobber = (merge != MERGE_AUGMENT);
        ܐܢ (report) {
            ثابت حرف* ثابت kname     = KeyNameText(info->ctx, name);
            ثابت حرف* ثابت old_kname = KeyNameText(info->ctx, old_name);
            ثابت حرف* ثابت use    = (clobber) ? kname : old_kname;
            ثابت حرف* ثابت ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        ܐܢ (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_key(&info->keycodes, match_kc, name);
        }
    } ܐܚܪܢܐ {
        /* No previous keycode */
        ܐܢ (!keycode_store_insert_key(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            ܦܢܐ false;
        }
    }

    ܦܢܐ true;
}

/***====================================================================***/

ساكن bool
HandleAliasDef(KeyNamesInfo *info, ثابت KeyAliasDef *def, bool report);

ساكن فراغ
MergeKeycodeStores(KeyNamesInfo *into, KeyNamesInfo *from,
                   تعداد merge_mode merge, bool report)
{
    ܐܢ (darray_empty(into->keycodes.low) && darray_empty(into->keycodes.high) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.low);
        darray_init(from->keycodes.high);
        darray_init(from->keycodes.names);
    } ܐܚܪܢܐ {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        لكل (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.low);
             kc++) {
            /* Skip undefined keycodes */
            ثابت xkb_atom_t name = darray_item(from->keycodes.low, kc);
            ܐܢ (name == XKB_ATOM_NONE)
                استمر;

            ܐܢ (!AddKeyName(into, kc, name, merge, report))
                into->errorCount++;
        }

        /* High keycodes */
        ثابت HighKeycodeEntry *new;
        darray_foreach(new, from->keycodes.high) {
            أكد(new->name != XKB_ATOM_NONE);
            ܐܢ (!AddKeyName(into, new->keycode, new->name, merge, report))
                into->errorCount++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            ܐܢ (!match->found || !match->is_alias)
                استمر;

            ثابت KeyAliasDef def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            ܐܢ (!HandleAliasDef(into, &def, report))
                into->errorCount++;
        }
    }
}

ساكن فراغ
MergeIncludedKeycodes(KeyNamesInfo *into, KeyNamesInfo *from,
                      تعداد merge_mode merge, bool report)
{
    ܐܢ (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        ܦܢܐ;
    }

    ܐܢ (into->name == عدم) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    ܐܢ (into->num_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               حجم(*from->led_names) * from->num_led_names);
        into->num_led_names = from->num_led_names;
        from->num_led_names = 0;
    }
    ܐܚܪܢܐ {
        لكل (xkb_led_index_t idx = 0; idx < from->num_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            ܐܢ (ledi->name == XKB_ATOM_NONE)
                استمر;

            ledi->merge = merge;
            ܐܢ (!AddLedName(into, false, ledi, idx, report))
                into->errorCount++;
        }
    }
}

ساكن فراغ
HandleKeycodesFile(KeyNamesInfo *info, XkbFile *file);

ساكن bool
HandleIncludeKeycodes(KeyNamesInfo *info, IncludeStmt *include, bool report)
{
    KeyNamesInfo included;

    ܐܢ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->errorCount += 10;
        ܦܢܐ false;
    }

    InitKeyNamesInfo(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        KeyNamesInfo next_incl;
        XkbFile *file;

        حرف path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, حجم(path));
        ܐܢ (!file) {
            info->errorCount += 10;
            ClearKeyNamesInfo(&included);
            ܦܢܐ false;
        }

        InitKeyNamesInfo(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        ClearKeyNamesInfo(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    ClearKeyNamesInfo(&included);

    ܦܢܐ (info->errorCount == 0);
}

ساكن bool
HandleKeycodeDef(KeyNamesInfo *info, KeycodeDef *stmt, bool report)
{
    ܐܢ (stmt->value < 0 || stmt->value > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->value, XKB_KEYCODE_MAX);
        ܦܢܐ false;
    }

    ܦܢܐ AddKeyName(info, (xkb_keycode_t) stmt->value,
                      stmt->name, stmt->merge, report);
}

ساكن bool
HandleAliasDef(KeyNamesInfo *info, ثابت KeyAliasDef *def, bool report)
{
    ثابت KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    ܐܢ (match_name.found) {
        ثابت bool clobber = (def->merge != MERGE_AUGMENT);
        ܐܢ (match_name.is_alias) {
            ܐܢ (def->real == match_name.alias.real) {
                ܐܢ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             KeyNameText(info->ctx, def->alias),
                             KeyNameText(info->ctx, def->real));
            } ܐܚܪܢܐ {
                ثابت xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                ثابت xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                ܐܢ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             KeyNameText(info->ctx, def->alias),
                             KeyNameText(info->ctx, use),
                             KeyNameText(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            ܦܢܐ true;
        } ܐܚܪܢܐ {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            ܐܢ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         KeyNameText(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            ܐܢ (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_key(&info->keycodes, match_name);
            } ܐܚܪܢܐ {
                ܦܢܐ true;
            }
        }
    }

    ܦܢܐ keycode_store_insert_alias(&info->keycodes, def->alias, def->real);
}

ساكن bool
HandleKeyNameVar(KeyNamesInfo *info, VarDef *stmt)
{
    ثابت حرف *elem, *field;
    ExprDef *arrayNdx;

    ܐܢ (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        ܦܢܐ false;

    ܐܢ (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        ܦܢܐ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ܐܢ (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        ܦܢܐ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    ܐܢ (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        ܦܢܐ !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    ܐܢ (!ExprResolveInteger(info->ctx, stmt->value, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        ܦܢܐ !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    ܦܢܐ true;
}

ساكن bool
HandleLedNameDef(KeyNamesInfo *info, LedNameDef *def, bool report)
{
    ܐܢ (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->errorCount++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        ܦܢܐ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    ܐܢ (!ExprResolveString(info->ctx, def->name, &name)) {
        حرف buf[20];
        snprintf(buf, حجم(buf), "%"PRId64, def->ndx);
        info->errorCount++;
        ܦܢܐ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    ܦܢܐ AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

ساكن فراغ
HandleKeycodesFile(KeyNamesInfo *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report_same_file = verbosity > 0;
    ثابت bool report_include   = verbosity > 7;

    حرر(info->name);
    info->name = strdup_safe(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            اكسر;
        حالة STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            اكسر;
        حالة STMT_ALIAS:
            ok = HandleAliasDef(info, (KeyAliasDef *) stmt, report_same_file);
            اكسر;
        حالة STMT_VAR:
            ok = HandleKeyNameVar(info, (VarDef *) stmt);
            اكسر;
        حالة STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            اكسر;
        حالة STMT_UNKNOWN_DECLARATION:
        حالة STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            اكسر;
        افتراضي:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            اكسر;
        }

        ܐܢ (!ok)
            info->errorCount++;

        ܐܢ (info->errorCount > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
CopyKeyNamesToKeymap(بنية xkb_keymap *keymap, KeyNamesInfo *info)
{
    ܐܢ (darray_empty(info->keycodes.low) && darray_empty(info->keycodes.high)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        أكد(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_key_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_key_code = 255;
        keymap->num_keys = keymap->num_keys_low = keymap->max_key_code + 1;
    } ܐܚܪܢܐ {
        أكد(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_key_code = info->keycodes.min;
        keymap->max_key_code = (darray_empty(info->keycodes.high))
            ? darray_size(info->keycodes.low) - 1
            : darray_item(
                info->keycodes.high,
                darray_size(info->keycodes.high) - 1
            ).keycode;
        keymap->num_keys_low = darray_size(info->keycodes.low);
        keymap->num_keys = keymap->num_keys_low
                         + darray_size(info->keycodes.high);
    }

    بنية xkb_key * ثابت keys = calloc(keymap->num_keys, حجم(*keys));
    ܐܢ (!keys) {
        keymap->num_keys = 0;
        keymap->min_key_code = keymap->max_key_code = XKB_KEYCODE_INVALID;
        ܦܢܐ false;
    }

    /* Low keycodes */
    لكل (xkb_keycode_t kc = keymap->min_key_code; kc < keymap->num_keys_low; kc++)
        keys[kc].keycode = kc;

    لكل (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.low);
         kc++)
        keys[kc].name = darray_item(info->keycodes.low, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->num_keys_low; /* First high keycode index */
    ثابت HighKeycodeEntry *entry;
    darray_foreach(entry, info->keycodes.high) {
        أكد(entry->name != XKB_ATOM_NONE);
        keys[idx].keycode = entry->keycode;
        keys[idx].name = entry->name;
        idx++;
    }

    keymap->keys = keys;
    ܦܢܐ true;
}

ساكن bool
CopyKeycodeNameLUT(بنية xkb_keymap *keymap, KeyNamesInfo *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        ܐܢ (!match->found)
            استمر;

        ܐܢ (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ثابت KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            ܐܢ (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        KeyNameText(info->ctx, name),
                        KeyNameText(info->ctx, match->alias.real));
                match->found = false;
                استمر;
            }
            أكد(!match_real.is_alias);
        } ܐܚܪܢܐ ܐܢ (!match->key.low) {
            /* Update to final index in keymap::keys */
            match->key.index += keymap->num_keys_low;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->num_key_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->key_names, عدم);
    darray_init(info->keycodes.names);
    ܦܢܐ true;
}

ساكن bool
CopyLedNamesToKeymap(بنية xkb_keymap *keymap, KeyNamesInfo *info)
{
    keymap->num_leds = info->num_led_names;
    لكل (xkb_led_index_t idx = 0; idx < info->num_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        ܐܢ (ledi->name == XKB_ATOM_NONE)
            استمر;

        keymap->leds[idx].name = ledi->name;
    }

    ܦܢܐ true;
}

ساكن bool
CopyKeyNamesInfoToKeymap(بنية xkb_keymap *keymap, KeyNamesInfo *info)
{
    /* This function trashes keymap on error, but that's OK. */
    ܐܢ (!CopyKeyNamesToKeymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        ܦܢܐ false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    ܐܢ (!keymap->num_keys ||
        keymap->min_key_code > 0) {
        keymap->redirect_key_auto = 0;
    } ܐܚܪܢܐ {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        لكل (xkb_keycode_t k = keymap->num_keys; k-- > keymap->num_keys_low;) {
            ܐܢ (keycode > keymap->keys[k].keycode)
                اكسر;
            /* Try next keycode downwards */
            keycode = keymap->keys[k].keycode - 1;
        }
        keymap->redirect_key_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    ܦܢܐ true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    KeyNamesInfo info;

    InitKeyNamesInfo(&info, keymap_info, 0);

    ܐܢ (file != عدم)
        HandleKeycodesFile(&info, file);

    ܐܢ (info.errorCount != 0)
        اذهب err_info;

    ܐܢ (!CopyKeyNamesInfoToKeymap(&keymap_info->keymap, &info))
        اذهب err_info;

    ClearKeyNamesInfo(&info);
    ܦܢܐ true;

err_info:
    ClearKeyNamesInfo(&info);
    ܦܢܐ false;
}
