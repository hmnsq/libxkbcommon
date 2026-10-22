/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <limits.h>
#شامل <stdint.h>
#شامل <stdlib.h>
#شامل <string.h>

#شامل "context.h"
#شامل "messages-codes.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcomp-priv.h"
#شامل "atom.h"
#شامل "darray.h"
#شامل "keymap.h"
#شامل "text.h"
#شامل "expr.h"
#شامل "include.h"
#شامل "util-mem.h"

تعریفنوع ساختار {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} بالا_Keycode_Entry;

تعریفنوع ساختار {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) کم;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(بالا_Keycode_Entry) بالا;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

ایستا درونخطی تهی
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->کم);
    darray_init(store->بالا);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ایستا درونخطی تهی
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->کم);
    darray_free(store->بالا);
    darray_free(store->names);
}

ایستا درونخطی تهی
keycode_store_update_کلید(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    اگر (unlikely(!match.found || match.is_alias)) {
        بازگشت;
    } وگرنه اگر (match.کلید.کم) {
        /* assume valid index */
        أكد(match.کلید.index < darray_size(store->کم));
        darray_item(store->کم, match.کلید.index) = name;
    } وگرنه {
        /* assume valid index */
        أكد(match.کلید.index < darray_size(store->بالا));
        darray_item(store->بالا, match.کلید.index).name = name;
    }

    /* Names lookup */
    اگر (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

ایستا bool
keycode_store_درج_کلید(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    اگر (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    اگر (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        اگر (kc >= (xkb_keycode_t) darray_size(store->کم))
            darray_resize0(store->کم, kc + 1);
        darray_item(store->کم, kc) = name;
        اگر (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .کلید = {
                .found = true,
                .کم = true,
                .is_alias = false,
                .index = kc
            }
        };
    } وگرنه {
        /* High keycode: insert into a sorted list */
        ثابت darray_size_t idx = darray_size(store->بالا);
        اگر (idx && darray_item(store->بالا, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            وقتی (lower < upper) {
                ثابت darray_size_t mid = lower + (upper - 1 - lower) / 2;
                ثابت بالا_Keycode_Entry * ثابت entry =
                    &darray_item(store->بالا, mid);
                اگر (entry->keycode < kc) {
                    lower = mid + 1;
                } وگرنه اگر (entry->keycode > kc) {
                    upper = mid;
                } وگرنه {
                    /* Unreachable: there is no repetition in the list */
                    أكد(entry->keycode != kc);
                }
            }
            أكد(lower < idx);
            أكد(darray_item(store->بالا, lower).keycode > kc);

            /* Update references to entries that will be moved */
            بالا_Keycode_Entry *entry;
            darray_foreach_from(entry, store->بالا, lower) {
                darray_item(store->names, entry->name).کلید.index++;
            }

            darray_درج(store->بالا, lower,
                          (بالا_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .کلید = {
                    .found = true,
                    .کم = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } وگرنه {
            /* Fast path: no need to sort */
            darray_append(store->بالا,
                          (بالا_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .کلید = {
                    .found = true,
                    .کم = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        اگر (darray_empty(store->کم))
            store->min = darray_item(store->بالا, 0).keycode;
    }
    بازگشت true;
}

ایستا درونخطی bool
keycode_store_درج_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    اگر (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    بازگشت true;
}

ایستا درونخطی bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    بازگشت true;
}

ایستا درونخطی تهی
keycode_store_delete_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    اگر (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

ایستا تهی
keycode_store_delete_کلید(KeycodeStore *store, ثابت KeycodeMatch match)
{
    اگر (unlikely(!match.found || match.is_alias)) {
        بازگشت;
    } وگرنه اگر (match.کلید.کم) {
        /* assume valid index */
        أكد(match.کلید.index < darray_size(store->کم));
        darray_item(store->names,
                    darray_item(store->کم, match.کلید.index)).found = false;
        اگر (match.کلید.index + 1u == darray_size(store->کم)) {
            /* Highest low keycode: shrink */
            اگر (store->min == match.کلید.index) {
                /* No low keycode left */
                darray_size(store->کم) = 0;
            } وگرنه {
                /* Look for previous defined low keycode */
                برای (darray_size_t idx = match.کلید.index; idx > 0; idx--) {
                    اگر (darray_item(store->کم, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->کم) = idx;
                        بشکن;
                    }
                }
            }
        } وگرنه {
            /* Lower keycode: reset */
            darray_item(store->کم, match.کلید.index) = XKB_ATOM_NONE;
        }
    } وگرنه {
        أكد(match.کلید.index < darray_size(store->بالا)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->بالا, match.کلید.index).name).found = false;
        darray_remove(store->بالا, match.کلید.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            اگر (entry->found && !entry->is_alias && !entry->کلید.کم &&
                entry->کلید.index > match.کلید.index) {
                entry->کلید.index--;
            }
        }
    }

    /* Update bounds */
    اگر (darray_empty(store->کم)) {
        store->min = (darray_empty(store->بالا))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->بالا, 0).keycode;
    } وگرنه {
        برای (xkb_keycode_t kc = store->min; kc < darray_size(store->کم); kc++) {
            اگر (darray_item(store->کم, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                بشکن;
            }
        }
    }
}

ایستا درونخطی xkb_keycode_t
keycode_store_get_keycode(ثابت KeycodeStore *store, KeycodeMatch match)
{
    اگر (!match.found || match.is_alias) {
        بازگشت XKB_KEYCODE_INVALID;
    } وگرنه اگر (match.کلید.کم) {
        /* assume valid index */
        أكد(match.کلید.index < darray_size(store->کم));
        بازگشت (xkb_keycode_t) match.کلید.index;
    } وگرنه {
        /* assume valid index */
        أكد(match.کلید.index < darray_size(store->بالا));
        بازگشت darray_item(store->بالا, match.کلید.index).keycode;
    }
}

ایستا درونخطی xkb_atom_t
keycode_store_get_کلید_name(ثابت KeycodeStore *store, KeycodeMatch match)
{
    اگر (!match.found || match.is_alias) {
        بازگشت XKB_ATOM_NONE;
    } وگرنه اگر (match.کلید.کم) {
        /* assume valid index */
        أكد(match.کلید.index < darray_size(store->کم));
        بازگشت darray_item(store->کم, match.کلید.index);
    } وگرنه {
        /* assume valid index */
        أكد(match.کلید.index < darray_size(store->بالا));
        بازگشت darray_item(store->بالا, match.کلید.index).name;
    }
}

ایستا KeycodeMatch
keycode_store_lookup_keycode(ثابت KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    اگر (kc < (xkb_keycode_t) darray_size(store->کم)) {
        بازگشت (KeycodeMatch) {
            .کلید = {
                .found = true,
                .کم = true,
                .is_alias = false,
                .index = kc
            }
        };
    } وگرنه اگر (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        بازگشت (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->بالا);
    وقتی (lower < upper) {
        ثابت darray_size_t mid = lower + (upper - 1 - lower) / 2;
        بالا_Keycode_Entry * ثابت entry = &darray_item(store->بالا, mid);
        اگر (entry->keycode < kc) {
            lower = mid + 1;
        } وگرنه اگر (entry->keycode > kc) {
            upper = mid;
        } وگرنه {
            بازگشت (KeycodeMatch) {
                .کلید = {
                    .found = true,
                    .کم = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    بازگشت (KeycodeMatch) { .found = false };
}

ایستا KeycodeMatch
keycode_store_lookup_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    اگر (name >= darray_size(store->names)) {
        بازگشت (KeycodeMatch) { .found = false, .is_alias = false };
    } وگرنه {
        بازگشت darray_item(store->names, name);
    }
}

/***====================================================================***/

تعریفنوع ساختار {
    شمارش merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

تعریفنوع ساختار {
    نویسه *name;
    صحیح error_شمار;
    unsigned صحیح include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t رقم_led_names;

    ساختار xkb_context *ctx;
    ثابت ساختار xkb_keymap_info *keymap_info;
} کلید_Names_Info;

/***====================================================================***/

ایستا LedNameInfo *
FindLedByName(کلید_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    برای (xkb_led_index_t idx = 0; idx < info->رقم_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        اگر (ledi->name == name) {
            *idx_out = idx;
            بازگشت ledi;
        }
    }

    بازگشت پوچ;
}

ایستا bool
AddLedName(کلید_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    ثابت bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    اگر (old) {
        اگر (old_idx == new_idx) {
            اگر (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            بازگشت true;
        }

        اگر (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        اگر (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } وگرنه {
            بازگشت true;
        }
    }

    اگر (new_idx >= info->رقم_led_names)
        info->رقم_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    اگر (old->name != XKB_ATOM_NONE) {
        اگر (report) {
            ثابت xkb_atom_t use    = (replace ? new->name : old->name);
            ثابت xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        اگر (replace)
            *old = *new;

        بازگشت true;
    }

    *old = *new;
    بازگشت true;
}

ایستا تهی
Clear_کلید_Names_Info(کلید_Names_Info *info)
{
    آزاد(info->name);
    keycode_store_free(&info->keycodes);
}

ایستا تهی
Init_کلید_Names_Info(کلید_Names_Info *info, ثابت ساختار xkb_keymap_info *keymap_info,
                 unsigned صحیح include_depth)
{
    memset(info, 0, اندازه(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

ایستا bool
Add_کلید_Name(کلید_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           شمارش merge_mode merge, bool report)
{
#ifdef NDEBUG
    ثابت
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    اگر (match_name.found) {
        ثابت bool clobber = (merge != MERGE_AUGMENT);

        اگر (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            اگر (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         کلید_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            اگر (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } وگرنه {
                بازگشت true;
            }
        } وگرنه {
            ثابت xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            أكد(old_kc != XKB_KEYCODE_INVALID);
            اگر (old_kc != kc) {
                /* There is already a different key with this name. */

                اگر (report) {
                    ثابت xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ثابت xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            کلید_Name_Text(info->ctx, name), use, ignore);
                }

                اگر (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_کلید(&info->keycodes, match_name);
                } وگرنه {
                    بازگشت true;
                }
            }
        }
    }

    ثابت KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ثابت xkb_atom_t old_name = keycode_store_get_کلید_name(&info->keycodes, match_kc);
    اگر (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        اگر (old_name == name) {
            أكد(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            اگر (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         کلید_Name_Text(info->ctx, old_name), kc);
            بازگشت true;
        }

        ثابت bool clobber = (merge != MERGE_AUGMENT);
        اگر (report) {
            ثابت نویسه* ثابت kname     = کلید_Name_Text(info->ctx, name);
            ثابت نویسه* ثابت old_kname = کلید_Name_Text(info->ctx, old_name);
            ثابت نویسه* ثابت use    = (clobber) ? kname : old_kname;
            ثابت نویسه* ثابت ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        اگر (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_کلید(&info->keycodes, match_kc, name);
        }
    } وگرنه {
        /* No previous keycode */
        اگر (!keycode_store_درج_کلید(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            بازگشت false;
        }
    }

    بازگشت true;
}

/***====================================================================***/

ایستا bool
HandleAliasDef(کلید_Names_Info *info, ثابت کلید_Alias_Def *def, bool report);

ایستا تهی
MergeKeycodeStores(کلید_Names_Info *into, کلید_Names_Info *from,
                   شمارش merge_mode merge, bool report)
{
    اگر (darray_empty(into->keycodes.کم) && darray_empty(into->keycodes.بالا) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.کم);
        darray_init(from->keycodes.بالا);
        darray_init(from->keycodes.names);
    } وگرنه {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        برای (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.کم);
             kc++) {
            /* Skip undefined keycodes */
            ثابت xkb_atom_t name = darray_item(from->keycodes.کم, kc);
            اگر (name == XKB_ATOM_NONE)
                ادامه;

            اگر (!Add_کلید_Name(into, kc, name, merge, report))
                into->error_شمار++;
        }

        /* High keycodes */
        ثابت بالا_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.بالا) {
            أكد(new->name != XKB_ATOM_NONE);
            اگر (!Add_کلید_Name(into, new->keycode, new->name, merge, report))
                into->error_شمار++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            اگر (!match->found || !match->is_alias)
                ادامه;

            ثابت کلید_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            اگر (!HandleAliasDef(into, &def, report))
                into->error_شمار++;
        }
    }
}

ایستا تهی
MergeIncludedKeycodes(کلید_Names_Info *into, کلید_Names_Info *from,
                      شمارش merge_mode merge, bool report)
{
    اگر (from->error_شمار > 0) {
        into->error_شمار += from->error_شمار;
        بازگشت;
    }

    اگر (into->name == پوچ) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    اگر (into->رقم_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               اندازه(*from->led_names) * from->رقم_led_names);
        into->رقم_led_names = from->رقم_led_names;
        from->رقم_led_names = 0;
    }
    وگرنه {
        برای (xkb_led_index_t idx = 0; idx < from->رقم_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            اگر (ledi->name == XKB_ATOM_NONE)
                ادامه;

            ledi->merge = merge;
            اگر (!AddLedName(into, false, ledi, idx, report))
                into->error_شمار++;
        }
    }
}

ایستا تهی
HandleKeycodesFile(کلید_Names_Info *info, XkbFile *file);

ایستا bool
HandleIncludeKeycodes(کلید_Names_Info *info, IncludeStmt *include, bool report)
{
    کلید_Names_Info included;

    اگر (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_شمار += 10;
        بازگشت false;
    }

    Init_کلید_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ثابت ساختار parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    برای (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        کلید_Names_Info next_incl;
        XkbFile *file;

        نویسه path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, اندازه(path));
        اگر (!file) {
            info->error_شمار += 10;
            Clear_کلید_Names_Info(&included);
            بازگشت false;
        }

        Init_کلید_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_کلید_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_کلید_Names_Info(&included);

    بازگشت (info->error_شمار == 0);
}

ایستا bool
HandleKeycodeDef(کلید_Names_Info *info, KeycodeDef *stmt, bool report)
{
    اگر (stmt->مقدار < 0 || stmt->مقدار > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->مقدار, XKB_KEYCODE_MAX);
        بازگشت false;
    }

    بازگشت Add_کلید_Name(info, (xkb_keycode_t) stmt->مقدار,
                      stmt->name, stmt->merge, report);
}

ایستا bool
HandleAliasDef(کلید_Names_Info *info, ثابت کلید_Alias_Def *def, bool report)
{
    ثابت KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    اگر (match_name.found) {
        ثابت bool clobber = (def->merge != MERGE_AUGMENT);
        اگر (match_name.is_alias) {
            اگر (def->real == match_name.alias.real) {
                اگر (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             کلید_Name_Text(info->ctx, def->alias),
                             کلید_Name_Text(info->ctx, def->real));
            } وگرنه {
                ثابت xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                ثابت xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                اگر (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             کلید_Name_Text(info->ctx, def->alias),
                             کلید_Name_Text(info->ctx, use),
                             کلید_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            بازگشت true;
        } وگرنه {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            اگر (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         کلید_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            اگر (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_کلید(&info->keycodes, match_name);
            } وگرنه {
                بازگشت true;
            }
        }
    }

    بازگشت keycode_store_درج_alias(&info->keycodes, def->alias, def->real);
}

ایستا bool
Handle_کلید_Name_Var(کلید_Names_Info *info, VarDef *stmt)
{
    ثابت نویسه *elem, *field;
    عبارت_Def *arrayNdx;

    اگر (!عبارت_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        بازگشت false;

    اگر (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        بازگشت !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    اگر (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        بازگشت !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    اگر (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        بازگشت !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    اگر (!عبارت_Resolve_Integer(info->ctx, stmt->مقدار, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        بازگشت !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    بازگشت true;
}

ایستا bool
HandleLedNameDef(کلید_Names_Info *info, LedNameDef *def, bool report)
{
    اگر (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_شمار++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        بازگشت false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    اگر (!عبارت_Resolve_String(info->ctx, def->name, &name)) {
        نویسه buf[20];
        snprintf(buf, اندازه(buf), "%"PRId64, def->ndx);
        info->error_شمار++;
        بازگشت ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    بازگشت AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

ایستا تهی
HandleKeycodesFile(کلید_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ثابت صحیح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report_same_file = verbosity > 0;
    ثابت bool report_include   = verbosity > 7;

    آزاد(info->name);
    info->name = strdup_safe(file->name);

    برای (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        انتخاب (stmt->type) {
        حالت STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            بشکن;
        حالت STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            بشکن;
        حالت STMT_ALIAS:
            ok = HandleAliasDef(info, (کلید_Alias_Def *) stmt, report_same_file);
            بشکن;
        حالت STMT_VAR:
            ok = Handle_کلید_Name_Var(info, (VarDef *) stmt);
            بشکن;
        حالت STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            بشکن;
        حالت STMT_UNKNOWN_DECLARATION:
        حالت STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            بشکن;
        پیشفرض:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            بشکن;
        }

        اگر (!ok)
            info->error_شمار++;

        اگر (info->error_شمار > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            بشکن;
        }
    }
}

/***====================================================================***/

ایستا bool
Copy_کلید_Names_To_Keymap(ساختار xkb_keymap *keymap, کلید_Names_Info *info)
{
    اگر (darray_empty(info->keycodes.کم) && darray_empty(info->keycodes.بالا)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        أكد(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_کلید_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_کلید_code = 255;
        keymap->رقم_کلیدها = keymap->رقم_کلیدها_کم = keymap->max_کلید_code + 1;
    } وگرنه {
        أكد(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_کلید_code = info->keycodes.min;
        keymap->max_کلید_code = (darray_empty(info->keycodes.بالا))
            ? darray_size(info->keycodes.کم) - 1
            : darray_item(
                info->keycodes.بالا,
                darray_size(info->keycodes.بالا) - 1
            ).keycode;
        keymap->رقم_کلیدها_کم = darray_size(info->keycodes.کم);
        keymap->رقم_کلیدها = keymap->رقم_کلیدها_کم
                         + darray_size(info->keycodes.بالا);
    }

    ساختار xkb_کلید * ثابت کلیدها = calloc(keymap->رقم_کلیدها, اندازه(*کلیدها));
    اگر (!کلیدها) {
        keymap->رقم_کلیدها = 0;
        keymap->min_کلید_code = keymap->max_کلید_code = XKB_KEYCODE_INVALID;
        بازگشت false;
    }

    /* Low keycodes */
    برای (xkb_keycode_t kc = keymap->min_کلید_code; kc < keymap->رقم_کلیدها_کم; kc++)
        کلیدها[kc].keycode = kc;

    برای (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.کم);
         kc++)
        کلیدها[kc].name = darray_item(info->keycodes.کم, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->رقم_کلیدها_کم; /* First high keycode index */
    ثابت بالا_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.بالا) {
        أكد(entry->name != XKB_ATOM_NONE);
        کلیدها[idx].keycode = entry->keycode;
        کلیدها[idx].name = entry->name;
        idx++;
    }

    keymap->کلیدها = کلیدها;
    بازگشت true;
}

ایستا bool
CopyKeycodeNameLUT(ساختار xkb_keymap *keymap, کلید_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        اگر (!match->found)
            ادامه;

        اگر (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ثابت KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            اگر (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        کلید_Name_Text(info->ctx, name),
                        کلید_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                ادامه;
            }
            أكد(!match_real.is_alias);
        } وگرنه اگر (!match->کلید.کم) {
            /* Update to final index in keymap::keys */
            match->کلید.index += keymap->رقم_کلیدها_کم;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->رقم_کلید_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->کلید_names, پوچ);
    darray_init(info->keycodes.names);
    بازگشت true;
}

ایستا bool
CopyLedNamesToKeymap(ساختار xkb_keymap *keymap, کلید_Names_Info *info)
{
    keymap->رقم_leds = info->رقم_led_names;
    برای (xkb_led_index_t idx = 0; idx < info->رقم_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        اگر (ledi->name == XKB_ATOM_NONE)
            ادامه;

        keymap->leds[idx].name = ledi->name;
    }

    بازگشت true;
}

ایستا bool
Copy_کلید_Names_Info_To_Keymap(ساختار xkb_keymap *keymap, کلید_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    اگر (!Copy_کلید_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        بازگشت false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    اگر (!keymap->رقم_کلیدها ||
        keymap->min_کلید_code > 0) {
        keymap->redirect_کلید_auto = 0;
    } وگرنه {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        برای (xkb_keycode_t k = keymap->رقم_کلیدها; k-- > keymap->رقم_کلیدها_کم;) {
            اگر (keycode > keymap->کلیدها[k].keycode)
                بشکن;
            /* Try next keycode downwards */
            keycode = keymap->کلیدها[k].keycode - 1;
        }
        keymap->redirect_کلید_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    بازگشت true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, ساختار xkb_keymap_info *keymap_info)
{
    کلید_Names_Info info;

    Init_کلید_Names_Info(&info, keymap_info, 0);

    اگر (file != پوچ)
        HandleKeycodesFile(&info, file);

    اگر (info.error_شمار != 0)
        برو err_info;

    اگر (!Copy_کلید_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        برو err_info;

    Clear_کلید_Names_Info(&info);
    بازگشت true;

err_info:
    Clear_کلید_Names_Info(&info);
    بازگشت false;
}
