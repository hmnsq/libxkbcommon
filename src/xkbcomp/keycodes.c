/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ "context.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "xkbcomp-priv.h"
#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "text.h"
#அந்தர்பாவ "expr.h"
#அந்தர்பாவ "include.h"
#அந்தர்பாவ "util-mem.h"

வகைவரை கட்டமைப்பு {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} உயர்வு_Keycode_Entry;

வகைவரை கட்டமைப்பு {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) கீழ்;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(உயர்வு_Keycode_Entry) உயர்வு;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

நிலையான உள்வரி வெறுமை
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->கீழ்);
    darray_init(store->உயர்வு);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

நிலையான உள்வரி வெறுமை
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->கீழ்);
    darray_free(store->உயர்வு);
    darray_free(store->names);
}

நிலையான உள்வரி வெறுமை
keycode_store_update_சாவி(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    எனில் (unlikely(!match.found || match.is_alias)) {
        திரும்பு;
    } இல்லையெனில் எனில் (match.சாவி.கீழ்) {
        /* assume valid index */
        ப்ரதிஜ்ஞா(match.சாவி.index < darray_size(store->கீழ்));
        darray_item(store->கீழ், match.சாவி.index) = name;
    } இல்லையெனில் {
        /* assume valid index */
        ப்ரதிஜ்ஞா(match.சாவி.index < darray_size(store->உயர்வு));
        darray_item(store->உயர்வு, match.சாவி.index).name = name;
    }

    /* Names lookup */
    எனில் (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

நிலையான bool
keycode_store_செருகு_சாவி(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    எனில் (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    எனில் (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        எனில் (kc >= (xkb_keycode_t) darray_size(store->கீழ்))
            darray_resize0(store->கீழ், kc + 1);
        darray_item(store->கீழ், kc) = name;
        எனில் (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .சாவி = {
                .found = true,
                .கீழ் = true,
                .is_alias = false,
                .index = kc
            }
        };
    } இல்லையெனில் {
        /* High keycode: insert into a sorted list */
        மாறிலி darray_size_t idx = darray_size(store->உயர்வு);
        எனில் (idx && darray_item(store->உயர்வு, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            போது (lower < upper) {
                மாறிலி darray_size_t mid = lower + (upper - 1 - lower) / 2;
                மாறிலி உயர்வு_Keycode_Entry * மாறிலி entry =
                    &darray_item(store->உயர்வு, mid);
                எனில் (entry->keycode < kc) {
                    lower = mid + 1;
                } இல்லையெனில் எனில் (entry->keycode > kc) {
                    upper = mid;
                } இல்லையெனில் {
                    /* Unreachable: there is no repetition in the list */
                    ப்ரதிஜ்ஞா(entry->keycode != kc);
                }
            }
            ப்ரதிஜ்ஞா(lower < idx);
            ப்ரதிஜ்ஞா(darray_item(store->உயர்வு, lower).keycode > kc);

            /* Update references to entries that will be moved */
            உயர்வு_Keycode_Entry *entry;
            darray_foreach_from(entry, store->உயர்வு, lower) {
                darray_item(store->names, entry->name).சாவி.index++;
            }

            darray_செருகு(store->உயர்வு, lower,
                          (உயர்வு_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .சாவி = {
                    .found = true,
                    .கீழ் = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } இல்லையெனில் {
            /* Fast path: no need to sort */
            darray_append(store->உயர்வு,
                          (உயர்வு_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .சாவி = {
                    .found = true,
                    .கீழ் = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        எனில் (darray_empty(store->கீழ்))
            store->min = darray_item(store->உயர்வு, 0).keycode;
    }
    திரும்பு true;
}

நிலையான உள்வரி bool
keycode_store_செருகு_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    எனில் (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    திரும்பு true;
}

நிலையான உள்வரி bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    திரும்பு true;
}

நிலையான உள்வரி வெறுமை
keycode_store_delete_name(மாறிலி KeycodeStore *store, xkb_atom_t name)
{
    எனில் (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

நிலையான வெறுமை
keycode_store_delete_சாவி(KeycodeStore *store, மாறிலி KeycodeMatch match)
{
    எனில் (unlikely(!match.found || match.is_alias)) {
        திரும்பு;
    } இல்லையெனில் எனில் (match.சாவி.கீழ்) {
        /* assume valid index */
        ப்ரதிஜ்ஞா(match.சாவி.index < darray_size(store->கீழ்));
        darray_item(store->names,
                    darray_item(store->கீழ், match.சாவி.index)).found = false;
        எனில் (match.சாவி.index + 1u == darray_size(store->கீழ்)) {
            /* Highest low keycode: shrink */
            எனில் (store->min == match.சாவி.index) {
                /* No low keycode left */
                darray_size(store->கீழ்) = 0;
            } இல்லையெனில் {
                /* Look for previous defined low keycode */
                க்கு (darray_size_t idx = match.சாவி.index; idx > 0; idx--) {
                    எனில் (darray_item(store->கீழ், idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->கீழ்) = idx;
                        நிறுத்து;
                    }
                }
            }
        } இல்லையெனில் {
            /* Lower keycode: reset */
            darray_item(store->கீழ், match.சாவி.index) = XKB_ATOM_NONE;
        }
    } இல்லையெனில் {
        ப்ரதிஜ்ஞா(match.சாவி.index < darray_size(store->உயர்வு)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->உயர்வு, match.சாவி.index).name).found = false;
        darray_remove(store->உயர்வு, match.சாவி.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            எனில் (entry->found && !entry->is_alias && !entry->சாவி.கீழ் &&
                entry->சாவி.index > match.சாவி.index) {
                entry->சாவி.index--;
            }
        }
    }

    /* Update bounds */
    எனில் (darray_empty(store->கீழ்)) {
        store->min = (darray_empty(store->உயர்வு))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->உயர்வு, 0).keycode;
    } இல்லையெனில் {
        க்கு (xkb_keycode_t kc = store->min; kc < darray_size(store->கீழ்); kc++) {
            எனில் (darray_item(store->கீழ், kc) != XKB_ATOM_NONE) {
                store->min = kc;
                நிறுத்து;
            }
        }
    }
}

நிலையான உள்வரி xkb_keycode_t
keycode_store_get_keycode(மாறிலி KeycodeStore *store, KeycodeMatch match)
{
    எனில் (!match.found || match.is_alias) {
        திரும்பு XKB_KEYCODE_INVALID;
    } இல்லையெனில் எனில் (match.சாவி.கீழ்) {
        /* assume valid index */
        ப்ரதிஜ்ஞா(match.சாவி.index < darray_size(store->கீழ்));
        திரும்பு (xkb_keycode_t) match.சாவி.index;
    } இல்லையெனில் {
        /* assume valid index */
        ப்ரதிஜ்ஞா(match.சாவி.index < darray_size(store->உயர்வு));
        திரும்பு darray_item(store->உயர்வு, match.சாவி.index).keycode;
    }
}

நிலையான உள்வரி xkb_atom_t
keycode_store_get_சாவி_name(மாறிலி KeycodeStore *store, KeycodeMatch match)
{
    எனில் (!match.found || match.is_alias) {
        திரும்பு XKB_ATOM_NONE;
    } இல்லையெனில் எனில் (match.சாவி.கீழ்) {
        /* assume valid index */
        ப்ரதிஜ்ஞா(match.சாவி.index < darray_size(store->கீழ்));
        திரும்பு darray_item(store->கீழ், match.சாவி.index);
    } இல்லையெனில் {
        /* assume valid index */
        ப்ரதிஜ்ஞா(match.சாவி.index < darray_size(store->உயர்வு));
        திரும்பு darray_item(store->உயர்வு, match.சாவி.index).name;
    }
}

நிலையான KeycodeMatch
keycode_store_lookup_keycode(மாறிலி KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    எனில் (kc < (xkb_keycode_t) darray_size(store->கீழ்)) {
        திரும்பு (KeycodeMatch) {
            .சாவி = {
                .found = true,
                .கீழ் = true,
                .is_alias = false,
                .index = kc
            }
        };
    } இல்லையெனில் எனில் (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        திரும்பு (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->உயர்வு);
    போது (lower < upper) {
        மாறிலி darray_size_t mid = lower + (upper - 1 - lower) / 2;
        உயர்வு_Keycode_Entry * மாறிலி entry = &darray_item(store->உயர்வு, mid);
        எனில் (entry->keycode < kc) {
            lower = mid + 1;
        } இல்லையெனில் எனில் (entry->keycode > kc) {
            upper = mid;
        } இல்லையெனில் {
            திரும்பு (KeycodeMatch) {
                .சாவி = {
                    .found = true,
                    .கீழ் = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    திரும்பு (KeycodeMatch) { .found = false };
}

நிலையான KeycodeMatch
keycode_store_lookup_name(மாறிலி KeycodeStore *store, xkb_atom_t name)
{
    எனில் (name >= darray_size(store->names)) {
        திரும்பு (KeycodeMatch) { .found = false, .is_alias = false };
    } இல்லையெனில் {
        திரும்பு darray_item(store->names, name);
    }
}

/***====================================================================***/

வகைவரை கட்டமைப்பு {
    எண்ணிக்கை merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

வகைவரை கட்டமைப்பு {
    எழுத்து *name;
    முழு error_மொத்தம்;
    unsigned முழு include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t இலக்கம்_led_names;

    கட்டமைப்பு xkb_context *ctx;
    மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info;
} சாவி_Names_Info;

/***====================================================================***/

நிலையான LedNameInfo *
FindLedByName(சாவி_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    க்கு (xkb_led_index_t idx = 0; idx < info->இலக்கம்_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        எனில் (ledi->name == name) {
            *idx_out = idx;
            திரும்பு ledi;
        }
    }

    திரும்பு வெற்று;
}

நிலையான bool
AddLedName(சாவி_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    மாறிலி bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    எனில் (old) {
        எனில் (old_idx == new_idx) {
            எனில் (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            திரும்பு true;
        }

        எனில் (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        எனில் (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } இல்லையெனில் {
            திரும்பு true;
        }
    }

    எனில் (new_idx >= info->இலக்கம்_led_names)
        info->இலக்கம்_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    எனில் (old->name != XKB_ATOM_NONE) {
        எனில் (report) {
            மாறிலி xkb_atom_t use    = (replace ? new->name : old->name);
            மாறிலி xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        எனில் (replace)
            *old = *new;

        திரும்பு true;
    }

    *old = *new;
    திரும்பு true;
}

நிலையான வெறுமை
Clear_சாவி_Names_Info(சாவி_Names_Info *info)
{
    விடுவி(info->name);
    keycode_store_free(&info->keycodes);
}

நிலையான வெறுமை
Init_சாவி_Names_Info(சாவி_Names_Info *info, மாறிலி கட்டமைப்பு xkb_keymap_info *keymap_info,
                 unsigned முழு include_depth)
{
    memset(info, 0, அளவு(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

நிலையான bool
Add_சாவி_Name(சாவி_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           எண்ணிக்கை merge_mode merge, bool report)
{
#ifdef NDEBUG
    மாறிலி
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    எனில் (match_name.found) {
        மாறிலி bool clobber = (merge != MERGE_AUGMENT);

        எனில் (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            எனில் (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_சாவி_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         சாவி_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            எனில் (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } இல்லையெனில் {
                திரும்பு true;
            }
        } இல்லையெனில் {
            மாறிலி xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            ப்ரதிஜ்ஞா(old_kc != XKB_KEYCODE_INVALID);
            எனில் (old_kc != kc) {
                /* There is already a different key with this name. */

                எனில் (report) {
                    மாறிலி xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    மாறிலி xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_சாவி_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            சாவி_Name_Text(info->ctx, name), use, ignore);
                }

                எனில் (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_சாவி(&info->keycodes, match_name);
                } இல்லையெனில் {
                    திரும்பு true;
                }
            }
        }
    }

    மாறிலி KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    மாறிலி xkb_atom_t old_name = keycode_store_get_சாவி_name(&info->keycodes, match_kc);
    எனில் (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        எனில் (old_name == name) {
            ப்ரதிஜ்ஞா(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            எனில் (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         சாவி_Name_Text(info->ctx, old_name), kc);
            திரும்பு true;
        }

        மாறிலி bool clobber = (merge != MERGE_AUGMENT);
        எனில் (report) {
            மாறிலி எழுத்து* மாறிலி kname     = சாவி_Name_Text(info->ctx, name);
            மாறிலி எழுத்து* மாறிலி old_kname = சாவி_Name_Text(info->ctx, old_name);
            மாறிலி எழுத்து* மாறிலி use    = (clobber) ? kname : old_kname;
            மாறிலி எழுத்து* மாறிலி ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        எனில் (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_சாவி(&info->keycodes, match_kc, name);
        }
    } இல்லையெனில் {
        /* No previous keycode */
        எனில் (!keycode_store_செருகு_சாவி(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            திரும்பு false;
        }
    }

    திரும்பு true;
}

/***====================================================================***/

நிலையான bool
HandleAliasDef(சாவி_Names_Info *info, மாறிலி சாவி_Alias_Def *def, bool report);

நிலையான வெறுமை
MergeKeycodeStores(சாவி_Names_Info *into, சாவி_Names_Info *from,
                   எண்ணிக்கை merge_mode merge, bool report)
{
    எனில் (darray_empty(into->keycodes.கீழ்) && darray_empty(into->keycodes.உயர்வு) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.கீழ்);
        darray_init(from->keycodes.உயர்வு);
        darray_init(from->keycodes.names);
    } இல்லையெனில் {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        க்கு (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.கீழ்);
             kc++) {
            /* Skip undefined keycodes */
            மாறிலி xkb_atom_t name = darray_item(from->keycodes.கீழ், kc);
            எனில் (name == XKB_ATOM_NONE)
                தொடர்;

            எனில் (!Add_சாவி_Name(into, kc, name, merge, report))
                into->error_மொத்தம்++;
        }

        /* High keycodes */
        மாறிலி உயர்வு_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.உயர்வு) {
            ப்ரதிஜ்ஞா(new->name != XKB_ATOM_NONE);
            எனில் (!Add_சாவி_Name(into, new->keycode, new->name, merge, report))
                into->error_மொத்தம்++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            எனில் (!match->found || !match->is_alias)
                தொடர்;

            மாறிலி சாவி_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            எனில் (!HandleAliasDef(into, &def, report))
                into->error_மொத்தம்++;
        }
    }
}

நிலையான வெறுமை
MergeIncludedKeycodes(சாவி_Names_Info *into, சாவி_Names_Info *from,
                      எண்ணிக்கை merge_mode merge, bool report)
{
    எனில் (from->error_மொத்தம் > 0) {
        into->error_மொத்தம் += from->error_மொத்தம்;
        திரும்பு;
    }

    எனில் (into->name == வெற்று) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    எனில் (into->இலக்கம்_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               அளவு(*from->led_names) * from->இலக்கம்_led_names);
        into->இலக்கம்_led_names = from->இலக்கம்_led_names;
        from->இலக்கம்_led_names = 0;
    }
    இல்லையெனில் {
        க்கு (xkb_led_index_t idx = 0; idx < from->இலக்கம்_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            எனில் (ledi->name == XKB_ATOM_NONE)
                தொடர்;

            ledi->merge = merge;
            எனில் (!AddLedName(into, false, ledi, idx, report))
                into->error_மொத்தம்++;
        }
    }
}

நிலையான வெறுமை
HandleKeycodesFile(சாவி_Names_Info *info, XkbFile *file);

நிலையான bool
HandleIncludeKeycodes(சாவி_Names_Info *info, IncludeStmt *include, bool report)
{
    சாவி_Names_Info included;

    எனில் (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_மொத்தம் += 10;
        திரும்பு false;
    }

    Init_சாவி_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    மாறிலி கட்டமைப்பு parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    க்கு (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        சாவி_Names_Info next_incl;
        XkbFile *file;

        எழுத்து path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, அளவு(path));
        எனில் (!file) {
            info->error_மொத்தம் += 10;
            Clear_சாவி_Names_Info(&included);
            திரும்பு false;
        }

        Init_சாவி_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_சாவி_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_சாவி_Names_Info(&included);

    திரும்பு (info->error_மொத்தம் == 0);
}

நிலையான bool
HandleKeycodeDef(சாவி_Names_Info *info, KeycodeDef *stmt, bool report)
{
    எனில் (stmt->மதிப்பு < 0 || stmt->மதிப்பு > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->மதிப்பு, XKB_KEYCODE_MAX);
        திரும்பு false;
    }

    திரும்பு Add_சாவி_Name(info, (xkb_keycode_t) stmt->மதிப்பு,
                      stmt->name, stmt->merge, report);
}

நிலையான bool
HandleAliasDef(சாவி_Names_Info *info, மாறிலி சாவி_Alias_Def *def, bool report)
{
    மாறிலி KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    எனில் (match_name.found) {
        மாறிலி bool clobber = (def->merge != MERGE_AUGMENT);
        எனில் (match_name.is_alias) {
            எனில் (def->real == match_name.alias.real) {
                எனில் (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_சாவி_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             சாவி_Name_Text(info->ctx, def->alias),
                             சாவி_Name_Text(info->ctx, def->real));
            } இல்லையெனில் {
                மாறிலி xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                மாறிலி xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                எனில் (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_சாவி_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             சாவி_Name_Text(info->ctx, def->alias),
                             சாவி_Name_Text(info->ctx, use),
                             சாவி_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            திரும்பு true;
        } இல்லையெனில் {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            எனில் (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_சாவி_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         சாவி_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            எனில் (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_சாவி(&info->keycodes, match_name);
            } இல்லையெனில் {
                திரும்பு true;
            }
        }
    }

    திரும்பு keycode_store_செருகு_alias(&info->keycodes, def->alias, def->real);
}

நிலையான bool
Handle_சாவி_Name_Var(சாவி_Names_Info *info, VarDef *stmt)
{
    மாறிலி எழுத்து *elem, *field;
    கோவை_Def *arrayNdx;

    எனில் (!கோவை_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        திரும்பு false;

    எனில் (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        திரும்பு !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    எனில் (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        திரும்பு !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    எனில் (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        திரும்பு !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    எனில் (!கோவை_Resolve_Integer(info->ctx, stmt->மதிப்பு, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        திரும்பு !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    திரும்பு true;
}

நிலையான bool
HandleLedNameDef(சாவி_Names_Info *info, LedNameDef *def, bool report)
{
    எனில் (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_மொத்தம்++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        திரும்பு false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    எனில் (!கோவை_Resolve_String(info->ctx, def->name, &name)) {
        எழுத்து buf[20];
        snprintf(buf, அளவு(buf), "%"PRId64, def->ndx);
        info->error_மொத்தம்++;
        திரும்பு ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    திரும்பு AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

நிலையான வெறுமை
HandleKeycodesFile(சாவி_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    மாறிலி முழு verbosity = xkb_context_get_log_verbosity(info->ctx);
    மாறிலி bool report_same_file = verbosity > 0;
    மாறிலி bool report_include   = verbosity > 7;

    விடுவி(info->name);
    info->name = strdup_safe(file->name);

    க்கு (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        தேர்வு (stmt->type) {
        நிலை STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            நிறுத்து;
        நிலை STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            நிறுத்து;
        நிலை STMT_ALIAS:
            ok = HandleAliasDef(info, (சாவி_Alias_Def *) stmt, report_same_file);
            நிறுத்து;
        நிலை STMT_VAR:
            ok = Handle_சாவி_Name_Var(info, (VarDef *) stmt);
            நிறுத்து;
        நிலை STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            நிறுத்து;
        நிலை STMT_UNKNOWN_DECLARATION:
        நிலை STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            நிறுத்து;
        இயல்பு:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            நிறுத்து;
        }

        எனில் (!ok)
            info->error_மொத்தம்++;

        எனில் (info->error_மொத்தம் > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            நிறுத்து;
        }
    }
}

/***====================================================================***/

நிலையான bool
Copy_சாவி_Names_To_Keymap(கட்டமைப்பு xkb_keymap *keymap, சாவி_Names_Info *info)
{
    எனில் (darray_empty(info->keycodes.கீழ்) && darray_empty(info->keycodes.உயர்வு)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        ப்ரதிஜ்ஞா(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_சாவி_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_சாவி_code = 255;
        keymap->இலக்கம்_சாவிகள் = keymap->இலக்கம்_சாவிகள்_கீழ் = keymap->max_சாவி_code + 1;
    } இல்லையெனில் {
        ப்ரதிஜ்ஞா(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_சாவி_code = info->keycodes.min;
        keymap->max_சாவி_code = (darray_empty(info->keycodes.உயர்வு))
            ? darray_size(info->keycodes.கீழ்) - 1
            : darray_item(
                info->keycodes.உயர்வு,
                darray_size(info->keycodes.உயர்வு) - 1
            ).keycode;
        keymap->இலக்கம்_சாவிகள்_கீழ் = darray_size(info->keycodes.கீழ்);
        keymap->இலக்கம்_சாவிகள் = keymap->இலக்கம்_சாவிகள்_கீழ்
                         + darray_size(info->keycodes.உயர்வு);
    }

    கட்டமைப்பு xkb_சாவி * மாறிலி சாவிகள் = calloc(keymap->இலக்கம்_சாவிகள், அளவு(*சாவிகள்));
    எனில் (!சாவிகள்) {
        keymap->இலக்கம்_சாவிகள் = 0;
        keymap->min_சாவி_code = keymap->max_சாவி_code = XKB_KEYCODE_INVALID;
        திரும்பு false;
    }

    /* Low keycodes */
    க்கு (xkb_keycode_t kc = keymap->min_சாவி_code; kc < keymap->இலக்கம்_சாவிகள்_கீழ்; kc++)
        சாவிகள்[kc].keycode = kc;

    க்கு (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.கீழ்);
         kc++)
        சாவிகள்[kc].name = darray_item(info->keycodes.கீழ், kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->இலக்கம்_சாவிகள்_கீழ்; /* First high keycode index */
    மாறிலி உயர்வு_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.உயர்வு) {
        ப்ரதிஜ்ஞா(entry->name != XKB_ATOM_NONE);
        சாவிகள்[idx].keycode = entry->keycode;
        சாவிகள்[idx].name = entry->name;
        idx++;
    }

    keymap->சாவிகள் = சாவிகள்;
    திரும்பு true;
}

நிலையான bool
CopyKeycodeNameLUT(கட்டமைப்பு xkb_keymap *keymap, சாவி_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        எனில் (!match->found)
            தொடர்;

        எனில் (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            மாறிலி KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            எனில் (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        சாவி_Name_Text(info->ctx, name),
                        சாவி_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                தொடர்;
            }
            ப்ரதிஜ்ஞா(!match_real.is_alias);
        } இல்லையெனில் எனில் (!match->சாவி.கீழ்) {
            /* Update to final index in keymap::keys */
            match->சாவி.index += keymap->இலக்கம்_சாவிகள்_கீழ்;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->இலக்கம்_சாவி_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->சாவி_names, வெற்று);
    darray_init(info->keycodes.names);
    திரும்பு true;
}

நிலையான bool
CopyLedNamesToKeymap(கட்டமைப்பு xkb_keymap *keymap, சாவி_Names_Info *info)
{
    keymap->இலக்கம்_leds = info->இலக்கம்_led_names;
    க்கு (xkb_led_index_t idx = 0; idx < info->இலக்கம்_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        எனில் (ledi->name == XKB_ATOM_NONE)
            தொடர்;

        keymap->leds[idx].name = ledi->name;
    }

    திரும்பு true;
}

நிலையான bool
Copy_சாவி_Names_Info_To_Keymap(கட்டமைப்பு xkb_keymap *keymap, சாவி_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    எனில் (!Copy_சாவி_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        திரும்பு false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    எனில் (!keymap->இலக்கம்_சாவிகள் ||
        keymap->min_சாவி_code > 0) {
        keymap->redirect_சாவி_auto = 0;
    } இல்லையெனில் {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        க்கு (xkb_keycode_t k = keymap->இலக்கம்_சாவிகள்; k-- > keymap->இலக்கம்_சாவிகள்_கீழ்;) {
            எனில் (keycode > keymap->சாவிகள்[k].keycode)
                நிறுத்து;
            /* Try next keycode downwards */
            keycode = keymap->சாவிகள்[k].keycode - 1;
        }
        keymap->redirect_சாவி_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    திரும்பு true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, கட்டமைப்பு xkb_keymap_info *keymap_info)
{
    சாவி_Names_Info info;

    Init_சாவி_Names_Info(&info, keymap_info, 0);

    எனில் (file != வெற்று)
        HandleKeycodesFile(&info, file);

    எனில் (info.error_மொத்தம் != 0)
        செல் err_info;

    எனில் (!Copy_சாவி_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        செல் err_info;

    Clear_சாவி_Names_Info(&info);
    திரும்பு true;

err_info:
    Clear_சாவி_Names_Info(&info);
    திரும்பு false;
}
