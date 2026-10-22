/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव "context.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "xkbcomp-priv.h"
#अन्तर्भाव "atom.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "expr.h"
#अन्तर्भाव "include.h"
#अन्तर्भाव "util-mem.h"

प्रकारनिर्देश संरचना {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} उच्चः_Keycode_Entry;

प्रकारनिर्देश संरचना {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) नीचः;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(उच्चः_Keycode_Entry) उच्चः;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

स्थिर अन्तःस्थ शून्य
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->नीचः);
    darray_init(store->उच्चः);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

स्थिर अन्तःस्थ शून्य
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->नीचः);
    darray_free(store->उच्चः);
    darray_free(store->names);
}

स्थिर अन्तःस्थ शून्य
keycode_store_update_कुञ्चिका(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    सचे (unlikely(!match.found || match.is_alias)) {
        पटिनिवत्त;
    } अञ्ञथा सचे (match.कुञ्चिका.नीचः) {
        /* assume valid index */
        प्रतिज्ञा(match.कुञ्चिका.index < darray_size(store->नीचः));
        darray_item(store->नीचः, match.कुञ्चिका.index) = name;
    } अञ्ञथा {
        /* assume valid index */
        प्रतिज्ञा(match.कुञ्चिका.index < darray_size(store->उच्चः));
        darray_item(store->उच्चः, match.कुञ्चिका.index).name = name;
    }

    /* Names lookup */
    सचे (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

स्थिर bool
keycode_store_निवेशय_कुञ्चिका(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    सचे (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    सचे (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        सचे (kc >= (xkb_keycode_t) darray_size(store->नीचः))
            darray_resize0(store->नीचः, kc + 1);
        darray_item(store->नीचः, kc) = name;
        सचे (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .कुञ्चिका = {
                .found = true,
                .नीचः = true,
                .is_alias = false,
                .index = kc
            }
        };
    } अञ्ञथा {
        /* High keycode: insert into a sorted list */
        ध्रुव darray_size_t idx = darray_size(store->उच्चः);
        सचे (idx && darray_item(store->उच्चः, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            याव (lower < upper) {
                ध्रुव darray_size_t mid = lower + (upper - 1 - lower) / 2;
                ध्रुव उच्चः_Keycode_Entry * ध्रुव entry =
                    &darray_item(store->उच्चः, mid);
                सचे (entry->keycode < kc) {
                    lower = mid + 1;
                } अञ्ञथा सचे (entry->keycode > kc) {
                    upper = mid;
                } अञ्ञथा {
                    /* Unreachable: there is no repetition in the list */
                    प्रतिज्ञा(entry->keycode != kc);
                }
            }
            प्रतिज्ञा(lower < idx);
            प्रतिज्ञा(darray_item(store->उच्चः, lower).keycode > kc);

            /* Update references to entries that will be moved */
            उच्चः_Keycode_Entry *entry;
            darray_foreach_from(entry, store->उच्चः, lower) {
                darray_item(store->names, entry->name).कुञ्चिका.index++;
            }

            darray_निवेशय(store->उच्चः, lower,
                          (उच्चः_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .कुञ्चिका = {
                    .found = true,
                    .नीचः = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } अञ्ञथा {
            /* Fast path: no need to sort */
            darray_append(store->उच्चः,
                          (उच्चः_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .कुञ्चिका = {
                    .found = true,
                    .नीचः = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        सचे (darray_empty(store->नीचः))
            store->min = darray_item(store->उच्चः, 0).keycode;
    }
    पटिनिवत्त true;
}

स्थिर अन्तःस्थ bool
keycode_store_निवेशय_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    सचे (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    पटिनिवत्त true;
}

स्थिर अन्तःस्थ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    पटिनिवत्त true;
}

स्थिर अन्तःस्थ शून्य
keycode_store_delete_name(ध्रुव KeycodeStore *store, xkb_atom_t name)
{
    सचे (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

स्थिर शून्य
keycode_store_delete_कुञ्चिका(KeycodeStore *store, ध्रुव KeycodeMatch match)
{
    सचे (unlikely(!match.found || match.is_alias)) {
        पटिनिवत्त;
    } अञ्ञथा सचे (match.कुञ्चिका.नीचः) {
        /* assume valid index */
        प्रतिज्ञा(match.कुञ्चिका.index < darray_size(store->नीचः));
        darray_item(store->names,
                    darray_item(store->नीचः, match.कुञ्चिका.index)).found = false;
        सचे (match.कुञ्चिका.index + 1u == darray_size(store->नीचः)) {
            /* Highest low keycode: shrink */
            सचे (store->min == match.कुञ्चिका.index) {
                /* No low keycode left */
                darray_size(store->नीचः) = 0;
            } अञ्ञथा {
                /* Look for previous defined low keycode */
                अत्थाय (darray_size_t idx = match.कुञ्चिका.index; idx > 0; idx--) {
                    सचे (darray_item(store->नीचः, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->नीचः) = idx;
                        भङ्ग;
                    }
                }
            }
        } अञ्ञथा {
            /* Lower keycode: reset */
            darray_item(store->नीचः, match.कुञ्चिका.index) = XKB_ATOM_NONE;
        }
    } अञ्ञथा {
        प्रतिज्ञा(match.कुञ्चिका.index < darray_size(store->उच्चः)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->उच्चः, match.कुञ्चिका.index).name).found = false;
        darray_remove(store->उच्चः, match.कुञ्चिका.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            सचे (entry->found && !entry->is_alias && !entry->कुञ्चिका.नीचः &&
                entry->कुञ्चिका.index > match.कुञ्चिका.index) {
                entry->कुञ्चिका.index--;
            }
        }
    }

    /* Update bounds */
    सचे (darray_empty(store->नीचः)) {
        store->min = (darray_empty(store->उच्चः))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->उच्चः, 0).keycode;
    } अञ्ञथा {
        अत्थाय (xkb_keycode_t kc = store->min; kc < darray_size(store->नीचः); kc++) {
            सचे (darray_item(store->नीचः, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                भङ्ग;
            }
        }
    }
}

स्थिर अन्तःस्थ xkb_keycode_t
keycode_store_get_keycode(ध्रुव KeycodeStore *store, KeycodeMatch match)
{
    सचे (!match.found || match.is_alias) {
        पटिनिवत्त XKB_KEYCODE_INVALID;
    } अञ्ञथा सचे (match.कुञ्चिका.नीचः) {
        /* assume valid index */
        प्रतिज्ञा(match.कुञ्चिका.index < darray_size(store->नीचः));
        पटिनिवत्त (xkb_keycode_t) match.कुञ्चिका.index;
    } अञ्ञथा {
        /* assume valid index */
        प्रतिज्ञा(match.कुञ्चिका.index < darray_size(store->उच्चः));
        पटिनिवत्त darray_item(store->उच्चः, match.कुञ्चिका.index).keycode;
    }
}

स्थिर अन्तःस्थ xkb_atom_t
keycode_store_get_कुञ्चिका_name(ध्रुव KeycodeStore *store, KeycodeMatch match)
{
    सचे (!match.found || match.is_alias) {
        पटिनिवत्त XKB_ATOM_NONE;
    } अञ्ञथा सचे (match.कुञ्चिका.नीचः) {
        /* assume valid index */
        प्रतिज्ञा(match.कुञ्चिका.index < darray_size(store->नीचः));
        पटिनिवत्त darray_item(store->नीचः, match.कुञ्चिका.index);
    } अञ्ञथा {
        /* assume valid index */
        प्रतिज्ञा(match.कुञ्चिका.index < darray_size(store->उच्चः));
        पटिनिवत्त darray_item(store->उच्चः, match.कुञ्चिका.index).name;
    }
}

स्थिर KeycodeMatch
keycode_store_lookup_keycode(ध्रुव KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    सचे (kc < (xkb_keycode_t) darray_size(store->नीचः)) {
        पटिनिवत्त (KeycodeMatch) {
            .कुञ्चिका = {
                .found = true,
                .नीचः = true,
                .is_alias = false,
                .index = kc
            }
        };
    } अञ्ञथा सचे (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        पटिनिवत्त (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->उच्चः);
    याव (lower < upper) {
        ध्रुव darray_size_t mid = lower + (upper - 1 - lower) / 2;
        उच्चः_Keycode_Entry * ध्रुव entry = &darray_item(store->उच्चः, mid);
        सचे (entry->keycode < kc) {
            lower = mid + 1;
        } अञ्ञथा सचे (entry->keycode > kc) {
            upper = mid;
        } अञ्ञथा {
            पटिनिवत्त (KeycodeMatch) {
                .कुञ्चिका = {
                    .found = true,
                    .नीचः = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    पटिनिवत्त (KeycodeMatch) { .found = false };
}

स्थिर KeycodeMatch
keycode_store_lookup_name(ध्रुव KeycodeStore *store, xkb_atom_t name)
{
    सचे (name >= darray_size(store->names)) {
        पटिनिवत्त (KeycodeMatch) { .found = false, .is_alias = false };
    } अञ्ञथा {
        पटिनिवत्त darray_item(store->names, name);
    }
}

/***====================================================================***/

प्रकारनिर्देश संरचना {
    गणना merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

प्रकारनिर्देश संरचना {
    वर्ण *name;
    पूर्णाङ्क error_गणनम्;
    unsigned पूर्णाङ्क include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t अङ्कः_led_names;

    संरचना xkb_context *ctx;
    ध्रुव संरचना xkb_keymap_info *keymap_info;
} कुञ्चिका_Names_Info;

/***====================================================================***/

स्थिर LedNameInfo *
FindLedByName(कुञ्चिका_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    अत्थाय (xkb_led_index_t idx = 0; idx < info->अङ्कः_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        सचे (ledi->name == name) {
            *idx_out = idx;
            पटिनिवत्त ledi;
        }
    }

    पटिनिवत्त रिक्त;
}

स्थिर bool
AddLedName(कुञ्चिका_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    ध्रुव bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    सचे (old) {
        सचे (old_idx == new_idx) {
            सचे (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            पटिनिवत्त true;
        }

        सचे (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        सचे (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } अञ्ञथा {
            पटिनिवत्त true;
        }
    }

    सचे (new_idx >= info->अङ्कः_led_names)
        info->अङ्कः_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    सचे (old->name != XKB_ATOM_NONE) {
        सचे (report) {
            ध्रुव xkb_atom_t use    = (replace ? new->name : old->name);
            ध्रुव xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        सचे (replace)
            *old = *new;

        पटिनिवत्त true;
    }

    *old = *new;
    पटिनिवत्त true;
}

स्थिर शून्य
Clear_कुञ्चिका_Names_Info(कुञ्चिका_Names_Info *info)
{
    मुक्त(info->name);
    keycode_store_free(&info->keycodes);
}

स्थिर शून्य
Init_कुञ्चिका_Names_Info(कुञ्चिका_Names_Info *info, ध्रुव संरचना xkb_keymap_info *keymap_info,
                 unsigned पूर्णाङ्क include_depth)
{
    memset(info, 0, परिमाण(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

स्थिर bool
Add_कुञ्चिका_Name(कुञ्चिका_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           गणना merge_mode merge, bool report)
{
#ifdef NDEBUG
    ध्रुव
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    सचे (match_name.found) {
        ध्रुव bool clobber = (merge != MERGE_AUGMENT);

        सचे (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            सचे (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         कुञ्चिका_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            सचे (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } अञ्ञथा {
                पटिनिवत्त true;
            }
        } अञ्ञथा {
            ध्रुव xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            प्रतिज्ञा(old_kc != XKB_KEYCODE_INVALID);
            सचे (old_kc != kc) {
                /* There is already a different key with this name. */

                सचे (report) {
                    ध्रुव xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ध्रुव xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            कुञ्चिका_Name_Text(info->ctx, name), use, ignore);
                }

                सचे (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_कुञ्चिका(&info->keycodes, match_name);
                } अञ्ञथा {
                    पटिनिवत्त true;
                }
            }
        }
    }

    ध्रुव KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ध्रुव xkb_atom_t old_name = keycode_store_get_कुञ्चिका_name(&info->keycodes, match_kc);
    सचे (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        सचे (old_name == name) {
            प्रतिज्ञा(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            सचे (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         कुञ्चिका_Name_Text(info->ctx, old_name), kc);
            पटिनिवत्त true;
        }

        ध्रुव bool clobber = (merge != MERGE_AUGMENT);
        सचे (report) {
            ध्रुव वर्ण* ध्रुव kname     = कुञ्चिका_Name_Text(info->ctx, name);
            ध्रुव वर्ण* ध्रुव old_kname = कुञ्चिका_Name_Text(info->ctx, old_name);
            ध्रुव वर्ण* ध्रुव use    = (clobber) ? kname : old_kname;
            ध्रुव वर्ण* ध्रुव ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        सचे (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_कुञ्चिका(&info->keycodes, match_kc, name);
        }
    } अञ्ञथा {
        /* No previous keycode */
        सचे (!keycode_store_निवेशय_कुञ्चिका(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            पटिनिवत्त false;
        }
    }

    पटिनिवत्त true;
}

/***====================================================================***/

स्थिर bool
HandleAliasDef(कुञ्चिका_Names_Info *info, ध्रुव कुञ्चिका_Alias_Def *def, bool report);

स्थिर शून्य
MergeKeycodeStores(कुञ्चिका_Names_Info *into, कुञ्चिका_Names_Info *from,
                   गणना merge_mode merge, bool report)
{
    सचे (darray_empty(into->keycodes.नीचः) && darray_empty(into->keycodes.उच्चः) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.नीचः);
        darray_init(from->keycodes.उच्चः);
        darray_init(from->keycodes.names);
    } अञ्ञथा {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        अत्थाय (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.नीचः);
             kc++) {
            /* Skip undefined keycodes */
            ध्रुव xkb_atom_t name = darray_item(from->keycodes.नीचः, kc);
            सचे (name == XKB_ATOM_NONE)
                अनुवर्त;

            सचे (!Add_कुञ्चिका_Name(into, kc, name, merge, report))
                into->error_गणनम्++;
        }

        /* High keycodes */
        ध्रुव उच्चः_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.उच्चः) {
            प्रतिज्ञा(new->name != XKB_ATOM_NONE);
            सचे (!Add_कुञ्चिका_Name(into, new->keycode, new->name, merge, report))
                into->error_गणनम्++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            सचे (!match->found || !match->is_alias)
                अनुवर्त;

            ध्रुव कुञ्चिका_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            सचे (!HandleAliasDef(into, &def, report))
                into->error_गणनम्++;
        }
    }
}

स्थिर शून्य
MergeIncludedKeycodes(कुञ्चिका_Names_Info *into, कुञ्चिका_Names_Info *from,
                      गणना merge_mode merge, bool report)
{
    सचे (from->error_गणनम् > 0) {
        into->error_गणनम् += from->error_गणनम्;
        पटिनिवत्त;
    }

    सचे (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    सचे (into->अङ्कः_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               परिमाण(*from->led_names) * from->अङ्कः_led_names);
        into->अङ्कः_led_names = from->अङ्कः_led_names;
        from->अङ्कः_led_names = 0;
    }
    अञ्ञथा {
        अत्थाय (xkb_led_index_t idx = 0; idx < from->अङ्कः_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            सचे (ledi->name == XKB_ATOM_NONE)
                अनुवर्त;

            ledi->merge = merge;
            सचे (!AddLedName(into, false, ledi, idx, report))
                into->error_गणनम्++;
        }
    }
}

स्थिर शून्य
HandleKeycodesFile(कुञ्चिका_Names_Info *info, XkbFile *file);

स्थिर bool
HandleIncludeKeycodes(कुञ्चिका_Names_Info *info, IncludeStmt *include, bool report)
{
    कुञ्चिका_Names_Info included;

    सचे (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गणनम् += 10;
        पटिनिवत्त false;
    }

    Init_कुञ्चिका_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ध्रुव संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    अत्थाय (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        कुञ्चिका_Names_Info next_incl;
        XkbFile *file;

        वर्ण path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, परिमाण(path));
        सचे (!file) {
            info->error_गणनम् += 10;
            Clear_कुञ्चिका_Names_Info(&included);
            पटिनिवत्त false;
        }

        Init_कुञ्चिका_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_कुञ्चिका_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_कुञ्चिका_Names_Info(&included);

    पटिनिवत्त (info->error_गणनम् == 0);
}

स्थिर bool
HandleKeycodeDef(कुञ्चिका_Names_Info *info, KeycodeDef *stmt, bool report)
{
    सचे (stmt->मानम् < 0 || stmt->मानम् > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->मानम्, XKB_KEYCODE_MAX);
        पटिनिवत्त false;
    }

    पटिनिवत्त Add_कुञ्चिका_Name(info, (xkb_keycode_t) stmt->मानम्,
                      stmt->name, stmt->merge, report);
}

स्थिर bool
HandleAliasDef(कुञ्चिका_Names_Info *info, ध्रुव कुञ्चिका_Alias_Def *def, bool report)
{
    ध्रुव KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    सचे (match_name.found) {
        ध्रुव bool clobber = (def->merge != MERGE_AUGMENT);
        सचे (match_name.is_alias) {
            सचे (def->real == match_name.alias.real) {
                सचे (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             कुञ्चिका_Name_Text(info->ctx, def->alias),
                             कुञ्चिका_Name_Text(info->ctx, def->real));
            } अञ्ञथा {
                ध्रुव xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                ध्रुव xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                सचे (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             कुञ्चिका_Name_Text(info->ctx, def->alias),
                             कुञ्चिका_Name_Text(info->ctx, use),
                             कुञ्चिका_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            पटिनिवत्त true;
        } अञ्ञथा {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            सचे (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         कुञ्चिका_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            सचे (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_कुञ्चिका(&info->keycodes, match_name);
            } अञ्ञथा {
                पटिनिवत्त true;
            }
        }
    }

    पटिनिवत्त keycode_store_निवेशय_alias(&info->keycodes, def->alias, def->real);
}

स्थिर bool
Handle_कुञ्चिका_Name_Var(कुञ्चिका_Names_Info *info, VarDef *stmt)
{
    ध्रुव वर्ण *elem, *field;
    व्यञ्जकम्_Def *arrayNdx;

    सचे (!व्यञ्जकम्_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        पटिनिवत्त false;

    सचे (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        पटिनिवत्त !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    सचे (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        पटिनिवत्त !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    सचे (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        पटिनिवत्त !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    सचे (!व्यञ्जकम्_Resolve_Integer(info->ctx, stmt->मानम्, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        पटिनिवत्त !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    पटिनिवत्त true;
}

स्थिर bool
HandleLedNameDef(कुञ्चिका_Names_Info *info, LedNameDef *def, bool report)
{
    सचे (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_गणनम्++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        पटिनिवत्त false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    सचे (!व्यञ्जकम्_Resolve_String(info->ctx, def->name, &name)) {
        वर्ण buf[20];
        snprintf(buf, परिमाण(buf), "%"PRId64, def->ndx);
        info->error_गणनम्++;
        पटिनिवत्त ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    पटिनिवत्त AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

स्थिर शून्य
HandleKeycodesFile(कुञ्चिका_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    ध्रुव bool report_same_file = verbosity > 0;
    ध्रुव bool report_include   = verbosity > 7;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    अत्थाय (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            भङ्ग;
        पक्ष STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            भङ्ग;
        पक्ष STMT_ALIAS:
            ok = HandleAliasDef(info, (कुञ्चिका_Alias_Def *) stmt, report_same_file);
            भङ्ग;
        पक्ष STMT_VAR:
            ok = Handle_कुञ्चिका_Name_Var(info, (VarDef *) stmt);
            भङ्ग;
        पक्ष STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            भङ्ग;
        पक्ष STMT_UNKNOWN_DECLARATION:
        पक्ष STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            भङ्ग;
        स्वभाव:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            भङ्ग;
        }

        सचे (!ok)
            info->error_गणनम्++;

        सचे (info->error_गणनम् > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            भङ्ग;
        }
    }
}

/***====================================================================***/

स्थिर bool
Copy_कुञ्चिका_Names_To_Keymap(संरचना xkb_keymap *keymap, कुञ्चिका_Names_Info *info)
{
    सचे (darray_empty(info->keycodes.नीचः) && darray_empty(info->keycodes.उच्चः)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        प्रतिज्ञा(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_कुञ्चिका_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_कुञ्चिका_code = 255;
        keymap->अङ्कः_कुञ्चिकाः = keymap->अङ्कः_कुञ्चिकाः_नीचः = keymap->max_कुञ्चिका_code + 1;
    } अञ्ञथा {
        प्रतिज्ञा(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_कुञ्चिका_code = info->keycodes.min;
        keymap->max_कुञ्चिका_code = (darray_empty(info->keycodes.उच्चः))
            ? darray_size(info->keycodes.नीचः) - 1
            : darray_item(
                info->keycodes.उच्चः,
                darray_size(info->keycodes.उच्चः) - 1
            ).keycode;
        keymap->अङ्कः_कुञ्चिकाः_नीचः = darray_size(info->keycodes.नीचः);
        keymap->अङ्कः_कुञ्चिकाः = keymap->अङ्कः_कुञ्चिकाः_नीचः
                         + darray_size(info->keycodes.उच्चः);
    }

    संरचना xkb_कुञ्चिका * ध्रुव कुञ्चिकाः = calloc(keymap->अङ्कः_कुञ्चिकाः, परिमाण(*कुञ्चिकाः));
    सचे (!कुञ्चिकाः) {
        keymap->अङ्कः_कुञ्चिकाः = 0;
        keymap->min_कुञ्चिका_code = keymap->max_कुञ्चिका_code = XKB_KEYCODE_INVALID;
        पटिनिवत्त false;
    }

    /* Low keycodes */
    अत्थाय (xkb_keycode_t kc = keymap->min_कुञ्चिका_code; kc < keymap->अङ्कः_कुञ्चिकाः_नीचः; kc++)
        कुञ्चिकाः[kc].keycode = kc;

    अत्थाय (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.नीचः);
         kc++)
        कुञ्चिकाः[kc].name = darray_item(info->keycodes.नीचः, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->अङ्कः_कुञ्चिकाः_नीचः; /* First high keycode index */
    ध्रुव उच्चः_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.उच्चः) {
        प्रतिज्ञा(entry->name != XKB_ATOM_NONE);
        कुञ्चिकाः[idx].keycode = entry->keycode;
        कुञ्चिकाः[idx].name = entry->name;
        idx++;
    }

    keymap->कुञ्चिकाः = कुञ्चिकाः;
    पटिनिवत्त true;
}

स्थिर bool
CopyKeycodeNameLUT(संरचना xkb_keymap *keymap, कुञ्चिका_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        सचे (!match->found)
            अनुवर्त;

        सचे (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ध्रुव KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            सचे (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        कुञ्चिका_Name_Text(info->ctx, name),
                        कुञ्चिका_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                अनुवर्त;
            }
            प्रतिज्ञा(!match_real.is_alias);
        } अञ्ञथा सचे (!match->कुञ्चिका.नीचः) {
            /* Update to final index in keymap::keys */
            match->कुञ्चिका.index += keymap->अङ्कः_कुञ्चिकाः_नीचः;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->अङ्कः_कुञ्चिका_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->कुञ्चिका_names, रिक्त);
    darray_init(info->keycodes.names);
    पटिनिवत्त true;
}

स्थिर bool
CopyLedNamesToKeymap(संरचना xkb_keymap *keymap, कुञ्चिका_Names_Info *info)
{
    keymap->अङ्कः_leds = info->अङ्कः_led_names;
    अत्थाय (xkb_led_index_t idx = 0; idx < info->अङ्कः_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        सचे (ledi->name == XKB_ATOM_NONE)
            अनुवर्त;

        keymap->leds[idx].name = ledi->name;
    }

    पटिनिवत्त true;
}

स्थिर bool
Copy_कुञ्चिका_Names_Info_To_Keymap(संरचना xkb_keymap *keymap, कुञ्चिका_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    सचे (!Copy_कुञ्चिका_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        पटिनिवत्त false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    सचे (!keymap->अङ्कः_कुञ्चिकाः ||
        keymap->min_कुञ्चिका_code > 0) {
        keymap->redirect_कुञ्चिका_auto = 0;
    } अञ्ञथा {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        अत्थाय (xkb_keycode_t k = keymap->अङ्कः_कुञ्चिकाः; k-- > keymap->अङ्कः_कुञ्चिकाः_नीचः;) {
            सचे (keycode > keymap->कुञ्चिकाः[k].keycode)
                भङ्ग;
            /* Try next keycode downwards */
            keycode = keymap->कुञ्चिकाः[k].keycode - 1;
        }
        keymap->redirect_कुञ्चिका_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    पटिनिवत्त true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    कुञ्चिका_Names_Info info;

    Init_कुञ्चिका_Names_Info(&info, keymap_info, 0);

    सचे (file != रिक्त)
        HandleKeycodesFile(&info, file);

    सचे (info.error_गणनम् != 0)
        गच्छ err_info;

    सचे (!Copy_कुञ्चिका_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    Clear_कुञ्चिका_Names_Info(&info);
    पटिनिवत्त true;

err_info:
    Clear_कुञ्चिका_Names_Info(&info);
    पटिनिवत्त false;
}
