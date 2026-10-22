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
} ऊपरी_Keycode_Entry;

प्रकारनिर्देश संरचना {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) निचला;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(ऊपरी_Keycode_Entry) ऊपरी;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

स्थिर अन्तःस्थ शून्य
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->निचला);
    darray_init(store->ऊपरी);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

स्थिर अन्तःस्थ शून्य
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->निचला);
    darray_free(store->ऊपरी);
    darray_free(store->names);
}

स्थिर अन्तःस्थ शून्य
keycode_store_update_कुंजी(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    यदि (unlikely(!match.found || match.is_alias)) {
        प्रत्यागम;
    } अन्यथा यदि (match.कुंजी.निचला) {
        /* assume valid index */
        प्रतिज्ञा(match.कुंजी.index < darray_size(store->निचला));
        darray_item(store->निचला, match.कुंजी.index) = name;
    } अन्यथा {
        /* assume valid index */
        प्रतिज्ञा(match.कुंजी.index < darray_size(store->ऊपरी));
        darray_item(store->ऊपरी, match.कुंजी.index).name = name;
    }

    /* Names lookup */
    यदि (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

स्थिर bool
keycode_store_जोड़ो_कुंजी(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    यदि (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    यदि (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        यदि (kc >= (xkb_keycode_t) darray_size(store->निचला))
            darray_resize0(store->निचला, kc + 1);
        darray_item(store->निचला, kc) = name;
        यदि (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .कुंजी = {
                .found = true,
                .निचला = true,
                .is_alias = false,
                .index = kc
            }
        };
    } अन्यथा {
        /* High keycode: insert into a sorted list */
        ध्रुव darray_size_t idx = darray_size(store->ऊपरी);
        यदि (idx && darray_item(store->ऊपरी, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            यावत् (lower < upper) {
                ध्रुव darray_size_t mid = lower + (upper - 1 - lower) / 2;
                ध्रुव ऊपरी_Keycode_Entry * ध्रुव entry =
                    &darray_item(store->ऊपरी, mid);
                यदि (entry->keycode < kc) {
                    lower = mid + 1;
                } अन्यथा यदि (entry->keycode > kc) {
                    upper = mid;
                } अन्यथा {
                    /* Unreachable: there is no repetition in the list */
                    प्रतिज्ञा(entry->keycode != kc);
                }
            }
            प्रतिज्ञा(lower < idx);
            प्रतिज्ञा(darray_item(store->ऊपरी, lower).keycode > kc);

            /* Update references to entries that will be moved */
            ऊपरी_Keycode_Entry *entry;
            darray_foreach_from(entry, store->ऊपरी, lower) {
                darray_item(store->names, entry->name).कुंजी.index++;
            }

            darray_जोड़ो(store->ऊपरी, lower,
                          (ऊपरी_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .कुंजी = {
                    .found = true,
                    .निचला = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } अन्यथा {
            /* Fast path: no need to sort */
            darray_append(store->ऊपरी,
                          (ऊपरी_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .कुंजी = {
                    .found = true,
                    .निचला = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        यदि (darray_empty(store->निचला))
            store->min = darray_item(store->ऊपरी, 0).keycode;
    }
    प्रत्यागम true;
}

स्थिर अन्तःस्थ bool
keycode_store_जोड़ो_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    यदि (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    प्रत्यागम true;
}

स्थिर अन्तःस्थ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    प्रत्यागम true;
}

स्थिर अन्तःस्थ शून्य
keycode_store_delete_name(ध्रुव KeycodeStore *store, xkb_atom_t name)
{
    यदि (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

स्थिर शून्य
keycode_store_delete_कुंजी(KeycodeStore *store, ध्रुव KeycodeMatch match)
{
    यदि (unlikely(!match.found || match.is_alias)) {
        प्रत्यागम;
    } अन्यथा यदि (match.कुंजी.निचला) {
        /* assume valid index */
        प्रतिज्ञा(match.कुंजी.index < darray_size(store->निचला));
        darray_item(store->names,
                    darray_item(store->निचला, match.कुंजी.index)).found = false;
        यदि (match.कुंजी.index + 1u == darray_size(store->निचला)) {
            /* Highest low keycode: shrink */
            यदि (store->min == match.कुंजी.index) {
                /* No low keycode left */
                darray_size(store->निचला) = 0;
            } अन्यथा {
                /* Look for previous defined low keycode */
                प्रति (darray_size_t idx = match.कुंजी.index; idx > 0; idx--) {
                    यदि (darray_item(store->निचला, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->निचला) = idx;
                        भङ्ग;
                    }
                }
            }
        } अन्यथा {
            /* Lower keycode: reset */
            darray_item(store->निचला, match.कुंजी.index) = XKB_ATOM_NONE;
        }
    } अन्यथा {
        प्रतिज्ञा(match.कुंजी.index < darray_size(store->ऊपरी)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->ऊपरी, match.कुंजी.index).name).found = false;
        darray_remove(store->ऊपरी, match.कुंजी.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            यदि (entry->found && !entry->is_alias && !entry->कुंजी.निचला &&
                entry->कुंजी.index > match.कुंजी.index) {
                entry->कुंजी.index--;
            }
        }
    }

    /* Update bounds */
    यदि (darray_empty(store->निचला)) {
        store->min = (darray_empty(store->ऊपरी))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->ऊपरी, 0).keycode;
    } अन्यथा {
        प्रति (xkb_keycode_t kc = store->min; kc < darray_size(store->निचला); kc++) {
            यदि (darray_item(store->निचला, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                भङ्ग;
            }
        }
    }
}

स्थिर अन्तःस्थ xkb_keycode_t
keycode_store_get_keycode(ध्रुव KeycodeStore *store, KeycodeMatch match)
{
    यदि (!match.found || match.is_alias) {
        प्रत्यागम XKB_KEYCODE_INVALID;
    } अन्यथा यदि (match.कुंजी.निचला) {
        /* assume valid index */
        प्रतिज्ञा(match.कुंजी.index < darray_size(store->निचला));
        प्रत्यागम (xkb_keycode_t) match.कुंजी.index;
    } अन्यथा {
        /* assume valid index */
        प्रतिज्ञा(match.कुंजी.index < darray_size(store->ऊपरी));
        प्रत्यागम darray_item(store->ऊपरी, match.कुंजी.index).keycode;
    }
}

स्थिर अन्तःस्थ xkb_atom_t
keycode_store_get_कुंजी_name(ध्रुव KeycodeStore *store, KeycodeMatch match)
{
    यदि (!match.found || match.is_alias) {
        प्रत्यागम XKB_ATOM_NONE;
    } अन्यथा यदि (match.कुंजी.निचला) {
        /* assume valid index */
        प्रतिज्ञा(match.कुंजी.index < darray_size(store->निचला));
        प्रत्यागम darray_item(store->निचला, match.कुंजी.index);
    } अन्यथा {
        /* assume valid index */
        प्रतिज्ञा(match.कुंजी.index < darray_size(store->ऊपरी));
        प्रत्यागम darray_item(store->ऊपरी, match.कुंजी.index).name;
    }
}

स्थिर KeycodeMatch
keycode_store_lookup_keycode(ध्रुव KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    यदि (kc < (xkb_keycode_t) darray_size(store->निचला)) {
        प्रत्यागम (KeycodeMatch) {
            .कुंजी = {
                .found = true,
                .निचला = true,
                .is_alias = false,
                .index = kc
            }
        };
    } अन्यथा यदि (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        प्रत्यागम (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->ऊपरी);
    यावत् (lower < upper) {
        ध्रुव darray_size_t mid = lower + (upper - 1 - lower) / 2;
        ऊपरी_Keycode_Entry * ध्रुव entry = &darray_item(store->ऊपरी, mid);
        यदि (entry->keycode < kc) {
            lower = mid + 1;
        } अन्यथा यदि (entry->keycode > kc) {
            upper = mid;
        } अन्यथा {
            प्रत्यागम (KeycodeMatch) {
                .कुंजी = {
                    .found = true,
                    .निचला = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    प्रत्यागम (KeycodeMatch) { .found = false };
}

स्थिर KeycodeMatch
keycode_store_lookup_name(ध्रुव KeycodeStore *store, xkb_atom_t name)
{
    यदि (name >= darray_size(store->names)) {
        प्रत्यागम (KeycodeMatch) { .found = false, .is_alias = false };
    } अन्यथा {
        प्रत्यागम darray_item(store->names, name);
    }
}

/***====================================================================***/

प्रकारनिर्देश संरचना {
    गणना merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

प्रकारनिर्देश संरचना {
    वर्ण *name;
    पूर्णाङ्क error_गिनती;
    unsigned पूर्णाङ्क include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t अंक_led_names;

    संरचना xkb_context *ctx;
    ध्रुव संरचना xkb_keymap_info *keymap_info;
} कुंजी_Names_Info;

/***====================================================================***/

स्थिर LedNameInfo *
FindLedByName(कुंजी_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    प्रति (xkb_led_index_t idx = 0; idx < info->अंक_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        यदि (ledi->name == name) {
            *idx_out = idx;
            प्रत्यागम ledi;
        }
    }

    प्रत्यागम रिक्त;
}

स्थिर bool
AddLedName(कुंजी_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    ध्रुव bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    यदि (old) {
        यदि (old_idx == new_idx) {
            यदि (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            प्रत्यागम true;
        }

        यदि (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        यदि (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } अन्यथा {
            प्रत्यागम true;
        }
    }

    यदि (new_idx >= info->अंक_led_names)
        info->अंक_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    यदि (old->name != XKB_ATOM_NONE) {
        यदि (report) {
            ध्रुव xkb_atom_t use    = (replace ? new->name : old->name);
            ध्रुव xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        यदि (replace)
            *old = *new;

        प्रत्यागम true;
    }

    *old = *new;
    प्रत्यागम true;
}

स्थिर शून्य
Clear_कुंजी_Names_Info(कुंजी_Names_Info *info)
{
    मुक्त(info->name);
    keycode_store_free(&info->keycodes);
}

स्थिर शून्य
Init_कुंजी_Names_Info(कुंजी_Names_Info *info, ध्रुव संरचना xkb_keymap_info *keymap_info,
                 unsigned पूर्णाङ्क include_depth)
{
    memset(info, 0, परिमाण(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

स्थिर bool
Add_कुंजी_Name(कुंजी_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           गणना merge_mode merge, bool report)
{
#ifdef NDEBUG
    ध्रुव
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    यदि (match_name.found) {
        ध्रुव bool clobber = (merge != MERGE_AUGMENT);

        यदि (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            यदि (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         कुंजी_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            यदि (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } अन्यथा {
                प्रत्यागम true;
            }
        } अन्यथा {
            ध्रुव xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            प्रतिज्ञा(old_kc != XKB_KEYCODE_INVALID);
            यदि (old_kc != kc) {
                /* There is already a different key with this name. */

                यदि (report) {
                    ध्रुव xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ध्रुव xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            कुंजी_Name_Text(info->ctx, name), use, ignore);
                }

                यदि (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_कुंजी(&info->keycodes, match_name);
                } अन्यथा {
                    प्रत्यागम true;
                }
            }
        }
    }

    ध्रुव KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ध्रुव xkb_atom_t old_name = keycode_store_get_कुंजी_name(&info->keycodes, match_kc);
    यदि (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        यदि (old_name == name) {
            प्रतिज्ञा(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            यदि (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         कुंजी_Name_Text(info->ctx, old_name), kc);
            प्रत्यागम true;
        }

        ध्रुव bool clobber = (merge != MERGE_AUGMENT);
        यदि (report) {
            ध्रुव वर्ण* ध्रुव kname     = कुंजी_Name_Text(info->ctx, name);
            ध्रुव वर्ण* ध्रुव old_kname = कुंजी_Name_Text(info->ctx, old_name);
            ध्रुव वर्ण* ध्रुव use    = (clobber) ? kname : old_kname;
            ध्रुव वर्ण* ध्रुव ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        यदि (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_कुंजी(&info->keycodes, match_kc, name);
        }
    } अन्यथा {
        /* No previous keycode */
        यदि (!keycode_store_जोड़ो_कुंजी(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            प्रत्यागम false;
        }
    }

    प्रत्यागम true;
}

/***====================================================================***/

स्थिर bool
HandleAliasDef(कुंजी_Names_Info *info, ध्रुव कुंजी_Alias_Def *def, bool report);

स्थिर शून्य
MergeKeycodeStores(कुंजी_Names_Info *into, कुंजी_Names_Info *from,
                   गणना merge_mode merge, bool report)
{
    यदि (darray_empty(into->keycodes.निचला) && darray_empty(into->keycodes.ऊपरी) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.निचला);
        darray_init(from->keycodes.ऊपरी);
        darray_init(from->keycodes.names);
    } अन्यथा {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        प्रति (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.निचला);
             kc++) {
            /* Skip undefined keycodes */
            ध्रुव xkb_atom_t name = darray_item(from->keycodes.निचला, kc);
            यदि (name == XKB_ATOM_NONE)
                अनुवर्त;

            यदि (!Add_कुंजी_Name(into, kc, name, merge, report))
                into->error_गिनती++;
        }

        /* High keycodes */
        ध्रुव ऊपरी_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.ऊपरी) {
            प्रतिज्ञा(new->name != XKB_ATOM_NONE);
            यदि (!Add_कुंजी_Name(into, new->keycode, new->name, merge, report))
                into->error_गिनती++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            यदि (!match->found || !match->is_alias)
                अनुवर्त;

            ध्रुव कुंजी_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            यदि (!HandleAliasDef(into, &def, report))
                into->error_गिनती++;
        }
    }
}

स्थिर शून्य
MergeIncludedKeycodes(कुंजी_Names_Info *into, कुंजी_Names_Info *from,
                      गणना merge_mode merge, bool report)
{
    यदि (from->error_गिनती > 0) {
        into->error_गिनती += from->error_गिनती;
        प्रत्यागम;
    }

    यदि (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    यदि (into->अंक_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               परिमाण(*from->led_names) * from->अंक_led_names);
        into->अंक_led_names = from->अंक_led_names;
        from->अंक_led_names = 0;
    }
    अन्यथा {
        प्रति (xkb_led_index_t idx = 0; idx < from->अंक_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            यदि (ledi->name == XKB_ATOM_NONE)
                अनुवर्त;

            ledi->merge = merge;
            यदि (!AddLedName(into, false, ledi, idx, report))
                into->error_गिनती++;
        }
    }
}

स्थिर शून्य
HandleKeycodesFile(कुंजी_Names_Info *info, XkbFile *file);

स्थिर bool
HandleIncludeKeycodes(कुंजी_Names_Info *info, IncludeStmt *include, bool report)
{
    कुंजी_Names_Info included;

    यदि (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गिनती += 10;
        प्रत्यागम false;
    }

    Init_कुंजी_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ध्रुव संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    प्रति (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        कुंजी_Names_Info next_incl;
        XkbFile *file;

        वर्ण path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, परिमाण(path));
        यदि (!file) {
            info->error_गिनती += 10;
            Clear_कुंजी_Names_Info(&included);
            प्रत्यागम false;
        }

        Init_कुंजी_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_कुंजी_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_कुंजी_Names_Info(&included);

    प्रत्यागम (info->error_गिनती == 0);
}

स्थिर bool
HandleKeycodeDef(कुंजी_Names_Info *info, KeycodeDef *stmt, bool report)
{
    यदि (stmt->मूल्य < 0 || stmt->मूल्य > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->मूल्य, XKB_KEYCODE_MAX);
        प्रत्यागम false;
    }

    प्रत्यागम Add_कुंजी_Name(info, (xkb_keycode_t) stmt->मूल्य,
                      stmt->name, stmt->merge, report);
}

स्थिर bool
HandleAliasDef(कुंजी_Names_Info *info, ध्रुव कुंजी_Alias_Def *def, bool report)
{
    ध्रुव KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    यदि (match_name.found) {
        ध्रुव bool clobber = (def->merge != MERGE_AUGMENT);
        यदि (match_name.is_alias) {
            यदि (def->real == match_name.alias.real) {
                यदि (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             कुंजी_Name_Text(info->ctx, def->alias),
                             कुंजी_Name_Text(info->ctx, def->real));
            } अन्यथा {
                ध्रुव xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                ध्रुव xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                यदि (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             कुंजी_Name_Text(info->ctx, def->alias),
                             कुंजी_Name_Text(info->ctx, use),
                             कुंजी_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            प्रत्यागम true;
        } अन्यथा {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            यदि (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         कुंजी_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            यदि (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_कुंजी(&info->keycodes, match_name);
            } अन्यथा {
                प्रत्यागम true;
            }
        }
    }

    प्रत्यागम keycode_store_जोड़ो_alias(&info->keycodes, def->alias, def->real);
}

स्थिर bool
Handle_कुंजी_Name_Var(कुंजी_Names_Info *info, VarDef *stmt)
{
    ध्रुव वर्ण *elem, *field;
    व्यंजक_Def *arrayNdx;

    यदि (!व्यंजक_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        प्रत्यागम false;

    यदि (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        प्रत्यागम !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    यदि (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        प्रत्यागम !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    यदि (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        प्रत्यागम !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    यदि (!व्यंजक_Resolve_Integer(info->ctx, stmt->मूल्य, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        प्रत्यागम !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    प्रत्यागम true;
}

स्थिर bool
HandleLedNameDef(कुंजी_Names_Info *info, LedNameDef *def, bool report)
{
    यदि (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_गिनती++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        प्रत्यागम false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    यदि (!व्यंजक_Resolve_String(info->ctx, def->name, &name)) {
        वर्ण buf[20];
        snprintf(buf, परिमाण(buf), "%"PRId64, def->ndx);
        info->error_गिनती++;
        प्रत्यागम ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    प्रत्यागम AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

स्थिर शून्य
HandleKeycodesFile(कुंजी_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    ध्रुव bool report_same_file = verbosity > 0;
    ध्रुव bool report_include   = verbosity > 7;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    प्रति (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            भङ्ग;
        पक्ष STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            भङ्ग;
        पक्ष STMT_ALIAS:
            ok = HandleAliasDef(info, (कुंजी_Alias_Def *) stmt, report_same_file);
            भङ्ग;
        पक्ष STMT_VAR:
            ok = Handle_कुंजी_Name_Var(info, (VarDef *) stmt);
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

        यदि (!ok)
            info->error_गिनती++;

        यदि (info->error_गिनती > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            भङ्ग;
        }
    }
}

/***====================================================================***/

स्थिर bool
Copy_कुंजी_Names_To_Keymap(संरचना xkb_keymap *keymap, कुंजी_Names_Info *info)
{
    यदि (darray_empty(info->keycodes.निचला) && darray_empty(info->keycodes.ऊपरी)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        प्रतिज्ञा(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_कुंजी_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_कुंजी_code = 255;
        keymap->अंक_कुंजियाँ = keymap->अंक_कुंजियाँ_निचला = keymap->max_कुंजी_code + 1;
    } अन्यथा {
        प्रतिज्ञा(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_कुंजी_code = info->keycodes.min;
        keymap->max_कुंजी_code = (darray_empty(info->keycodes.ऊपरी))
            ? darray_size(info->keycodes.निचला) - 1
            : darray_item(
                info->keycodes.ऊपरी,
                darray_size(info->keycodes.ऊपरी) - 1
            ).keycode;
        keymap->अंक_कुंजियाँ_निचला = darray_size(info->keycodes.निचला);
        keymap->अंक_कुंजियाँ = keymap->अंक_कुंजियाँ_निचला
                         + darray_size(info->keycodes.ऊपरी);
    }

    संरचना xkb_कुंजी * ध्रुव कुंजियाँ = calloc(keymap->अंक_कुंजियाँ, परिमाण(*कुंजियाँ));
    यदि (!कुंजियाँ) {
        keymap->अंक_कुंजियाँ = 0;
        keymap->min_कुंजी_code = keymap->max_कुंजी_code = XKB_KEYCODE_INVALID;
        प्रत्यागम false;
    }

    /* Low keycodes */
    प्रति (xkb_keycode_t kc = keymap->min_कुंजी_code; kc < keymap->अंक_कुंजियाँ_निचला; kc++)
        कुंजियाँ[kc].keycode = kc;

    प्रति (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.निचला);
         kc++)
        कुंजियाँ[kc].name = darray_item(info->keycodes.निचला, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->अंक_कुंजियाँ_निचला; /* First high keycode index */
    ध्रुव ऊपरी_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.ऊपरी) {
        प्रतिज्ञा(entry->name != XKB_ATOM_NONE);
        कुंजियाँ[idx].keycode = entry->keycode;
        कुंजियाँ[idx].name = entry->name;
        idx++;
    }

    keymap->कुंजियाँ = कुंजियाँ;
    प्रत्यागम true;
}

स्थिर bool
CopyKeycodeNameLUT(संरचना xkb_keymap *keymap, कुंजी_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        यदि (!match->found)
            अनुवर्त;

        यदि (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ध्रुव KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            यदि (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        कुंजी_Name_Text(info->ctx, name),
                        कुंजी_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                अनुवर्त;
            }
            प्रतिज्ञा(!match_real.is_alias);
        } अन्यथा यदि (!match->कुंजी.निचला) {
            /* Update to final index in keymap::keys */
            match->कुंजी.index += keymap->अंक_कुंजियाँ_निचला;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->अंक_कुंजी_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->कुंजी_names, रिक्त);
    darray_init(info->keycodes.names);
    प्रत्यागम true;
}

स्थिर bool
CopyLedNamesToKeymap(संरचना xkb_keymap *keymap, कुंजी_Names_Info *info)
{
    keymap->अंक_leds = info->अंक_led_names;
    प्रति (xkb_led_index_t idx = 0; idx < info->अंक_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        यदि (ledi->name == XKB_ATOM_NONE)
            अनुवर्त;

        keymap->leds[idx].name = ledi->name;
    }

    प्रत्यागम true;
}

स्थिर bool
Copy_कुंजी_Names_Info_To_Keymap(संरचना xkb_keymap *keymap, कुंजी_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    यदि (!Copy_कुंजी_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        प्रत्यागम false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    यदि (!keymap->अंक_कुंजियाँ ||
        keymap->min_कुंजी_code > 0) {
        keymap->redirect_कुंजी_auto = 0;
    } अन्यथा {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        प्रति (xkb_keycode_t k = keymap->अंक_कुंजियाँ; k-- > keymap->अंक_कुंजियाँ_निचला;) {
            यदि (keycode > keymap->कुंजियाँ[k].keycode)
                भङ्ग;
            /* Try next keycode downwards */
            keycode = keymap->कुंजियाँ[k].keycode - 1;
        }
        keymap->redirect_कुंजी_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    प्रत्यागम true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    कुंजी_Names_Info info;

    Init_कुंजी_Names_Info(&info, keymap_info, 0);

    यदि (file != रिक्त)
        HandleKeycodesFile(&info, file);

    यदि (info.error_गिनती != 0)
        गच्छ err_info;

    यदि (!Copy_कुंजी_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    Clear_कुंजी_Names_Info(&info);
    प्रत्यागम true;

err_info:
    Clear_कुंजी_Names_Info(&info);
    प्रत्यागम false;
}
