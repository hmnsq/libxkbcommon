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
} माथिल्लो_Keycode_Entry;

प्रकारनिर्देश संरचना {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) तल्लो;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(माथिल्लो_Keycode_Entry) माथिल्लो;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

स्थिर अन्तःस्थ शून्य
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->तल्लो);
    darray_init(store->माथिल्लो);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

स्थिर अन्तःस्थ शून्य
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->तल्लो);
    darray_free(store->माथिल्लो);
    darray_free(store->names);
}

स्थिर अन्तःस्थ शून्य
keycode_store_update_साँचो(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    यदि (unlikely(!match.found || match.is_alias)) {
        फर्काऊ;
    } अन्यथा यदि (match.साँचो.तल्लो) {
        /* assume valid index */
        प्रतिज्ञा(match.साँचो.index < darray_size(store->तल्लो));
        darray_item(store->तल्लो, match.साँचो.index) = name;
    } अन्यथा {
        /* assume valid index */
        प्रतिज्ञा(match.साँचो.index < darray_size(store->माथिल्लो));
        darray_item(store->माथिल्लो, match.साँचो.index).name = name;
    }

    /* Names lookup */
    यदि (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

स्थिर bool
keycode_store_घुसाउ_साँचो(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    यदि (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    यदि (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        यदि (kc >= (xkb_keycode_t) darray_size(store->तल्लो))
            darray_resize0(store->तल्लो, kc + 1);
        darray_item(store->तल्लो, kc) = name;
        यदि (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .साँचो = {
                .found = true,
                .तल्लो = true,
                .is_alias = false,
                .index = kc
            }
        };
    } अन्यथा {
        /* High keycode: insert into a sorted list */
        अचर darray_size_t idx = darray_size(store->माथिल्लो);
        यदि (idx && darray_item(store->माथिल्लो, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            जबसम्म (lower < upper) {
                अचर darray_size_t mid = lower + (upper - 1 - lower) / 2;
                अचर माथिल्लो_Keycode_Entry * अचर entry =
                    &darray_item(store->माथिल्लो, mid);
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
            प्रतिज्ञा(darray_item(store->माथिल्लो, lower).keycode > kc);

            /* Update references to entries that will be moved */
            माथिल्लो_Keycode_Entry *entry;
            darray_foreach_from(entry, store->माथिल्लो, lower) {
                darray_item(store->names, entry->name).साँचो.index++;
            }

            darray_घुसाउ(store->माथिल्लो, lower,
                          (माथिल्लो_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .साँचो = {
                    .found = true,
                    .तल्लो = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } अन्यथा {
            /* Fast path: no need to sort */
            darray_append(store->माथिल्लो,
                          (माथिल्लो_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .साँचो = {
                    .found = true,
                    .तल्लो = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        यदि (darray_empty(store->तल्लो))
            store->min = darray_item(store->माथिल्लो, 0).keycode;
    }
    फर्काऊ true;
}

स्थिर अन्तःस्थ bool
keycode_store_घुसाउ_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
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
    फर्काऊ true;
}

स्थिर अन्तःस्थ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    फर्काऊ true;
}

स्थिर अन्तःस्थ शून्य
keycode_store_delete_name(अचर KeycodeStore *store, xkb_atom_t name)
{
    यदि (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

स्थिर शून्य
keycode_store_delete_साँचो(KeycodeStore *store, अचर KeycodeMatch match)
{
    यदि (unlikely(!match.found || match.is_alias)) {
        फर्काऊ;
    } अन्यथा यदि (match.साँचो.तल्लो) {
        /* assume valid index */
        प्रतिज्ञा(match.साँचो.index < darray_size(store->तल्लो));
        darray_item(store->names,
                    darray_item(store->तल्लो, match.साँचो.index)).found = false;
        यदि (match.साँचो.index + 1u == darray_size(store->तल्लो)) {
            /* Highest low keycode: shrink */
            यदि (store->min == match.साँचो.index) {
                /* No low keycode left */
                darray_size(store->तल्लो) = 0;
            } अन्यथा {
                /* Look for previous defined low keycode */
                लागि (darray_size_t idx = match.साँचो.index; idx > 0; idx--) {
                    यदि (darray_item(store->तल्लो, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->तल्लो) = idx;
                        रोक;
                    }
                }
            }
        } अन्यथा {
            /* Lower keycode: reset */
            darray_item(store->तल्लो, match.साँचो.index) = XKB_ATOM_NONE;
        }
    } अन्यथा {
        प्रतिज्ञा(match.साँचो.index < darray_size(store->माथिल्लो)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->माथिल्लो, match.साँचो.index).name).found = false;
        darray_remove(store->माथिल्लो, match.साँचो.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            यदि (entry->found && !entry->is_alias && !entry->साँचो.तल्लो &&
                entry->साँचो.index > match.साँचो.index) {
                entry->साँचो.index--;
            }
        }
    }

    /* Update bounds */
    यदि (darray_empty(store->तल्लो)) {
        store->min = (darray_empty(store->माथिल्लो))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->माथिल्लो, 0).keycode;
    } अन्यथा {
        लागि (xkb_keycode_t kc = store->min; kc < darray_size(store->तल्लो); kc++) {
            यदि (darray_item(store->तल्लो, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                रोक;
            }
        }
    }
}

स्थिर अन्तःस्थ xkb_keycode_t
keycode_store_get_keycode(अचर KeycodeStore *store, KeycodeMatch match)
{
    यदि (!match.found || match.is_alias) {
        फर्काऊ XKB_KEYCODE_INVALID;
    } अन्यथा यदि (match.साँचो.तल्लो) {
        /* assume valid index */
        प्रतिज्ञा(match.साँचो.index < darray_size(store->तल्लो));
        फर्काऊ (xkb_keycode_t) match.साँचो.index;
    } अन्यथा {
        /* assume valid index */
        प्रतिज्ञा(match.साँचो.index < darray_size(store->माथिल्लो));
        फर्काऊ darray_item(store->माथिल्लो, match.साँचो.index).keycode;
    }
}

स्थिर अन्तःस्थ xkb_atom_t
keycode_store_get_साँचो_name(अचर KeycodeStore *store, KeycodeMatch match)
{
    यदि (!match.found || match.is_alias) {
        फर्काऊ XKB_ATOM_NONE;
    } अन्यथा यदि (match.साँचो.तल्लो) {
        /* assume valid index */
        प्रतिज्ञा(match.साँचो.index < darray_size(store->तल्लो));
        फर्काऊ darray_item(store->तल्लो, match.साँचो.index);
    } अन्यथा {
        /* assume valid index */
        प्रतिज्ञा(match.साँचो.index < darray_size(store->माथिल्लो));
        फर्काऊ darray_item(store->माथिल्लो, match.साँचो.index).name;
    }
}

स्थिर KeycodeMatch
keycode_store_lookup_keycode(अचर KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    यदि (kc < (xkb_keycode_t) darray_size(store->तल्लो)) {
        फर्काऊ (KeycodeMatch) {
            .साँचो = {
                .found = true,
                .तल्लो = true,
                .is_alias = false,
                .index = kc
            }
        };
    } अन्यथा यदि (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        फर्काऊ (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->माथिल्लो);
    जबसम्म (lower < upper) {
        अचर darray_size_t mid = lower + (upper - 1 - lower) / 2;
        माथिल्लो_Keycode_Entry * अचर entry = &darray_item(store->माथिल्लो, mid);
        यदि (entry->keycode < kc) {
            lower = mid + 1;
        } अन्यथा यदि (entry->keycode > kc) {
            upper = mid;
        } अन्यथा {
            फर्काऊ (KeycodeMatch) {
                .साँचो = {
                    .found = true,
                    .तल्लो = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    फर्काऊ (KeycodeMatch) { .found = false };
}

स्थिर KeycodeMatch
keycode_store_lookup_name(अचर KeycodeStore *store, xkb_atom_t name)
{
    यदि (name >= darray_size(store->names)) {
        फर्काऊ (KeycodeMatch) { .found = false, .is_alias = false };
    } अन्यथा {
        फर्काऊ darray_item(store->names, name);
    }
}

/***====================================================================***/

प्रकारनिर्देश संरचना {
    गणना merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

प्रकारनिर्देश संरचना {
    वर्ण *name;
    पूर्णाङ्क error_गन्ती;
    अचिन्हित पूर्णाङ्क include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t अङ्क_led_names;

    संरचना xkb_context *ctx;
    अचर संरचना xkb_keymap_info *keymap_info;
} साँचो_Names_Info;

/***====================================================================***/

स्थिर LedNameInfo *
FindLedByName(साँचो_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    लागि (xkb_led_index_t idx = 0; idx < info->अङ्क_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        यदि (ledi->name == name) {
            *idx_out = idx;
            फर्काऊ ledi;
        }
    }

    फर्काऊ रिक्त;
}

स्थिर bool
AddLedName(साँचो_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    अचर bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    यदि (old) {
        यदि (old_idx == new_idx) {
            यदि (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            फर्काऊ true;
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
            फर्काऊ true;
        }
    }

    यदि (new_idx >= info->अङ्क_led_names)
        info->अङ्क_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    यदि (old->name != XKB_ATOM_NONE) {
        यदि (report) {
            अचर xkb_atom_t use    = (replace ? new->name : old->name);
            अचर xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        यदि (replace)
            *old = *new;

        फर्काऊ true;
    }

    *old = *new;
    फर्काऊ true;
}

स्थिर शून्य
Clear_साँचो_Names_Info(साँचो_Names_Info *info)
{
    मुक्त(info->name);
    keycode_store_free(&info->keycodes);
}

स्थिर शून्य
Init_साँचो_Names_Info(साँचो_Names_Info *info, अचर संरचना xkb_keymap_info *keymap_info,
                 अचिन्हित पूर्णाङ्क include_depth)
{
    स_रखो(info, 0, परिमाण(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

स्थिर bool
Add_साँचो_Name(साँचो_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           गणना merge_mode merge, bool report)
{
#ifdef न_संशोधन
    अचर
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    यदि (match_name.found) {
        अचर bool clobber = (merge != MERGE_AUGMENT);

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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_साँचो_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         साँचो_Name_Text(info->ctx, name),
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
#ifndef न_संशोधन
                match_name.found = false;
#endif
            } अन्यथा {
                फर्काऊ true;
            }
        } अन्यथा {
            अचर xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            प्रतिज्ञा(old_kc != XKB_KEYCODE_INVALID);
            यदि (old_kc != kc) {
                /* There is already a different key with this name. */

                यदि (report) {
                    अचर xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    अचर xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_साँचो_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            साँचो_Name_Text(info->ctx, name), use, ignore);
                }

                यदि (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_साँचो(&info->keycodes, match_name);
                } अन्यथा {
                    फर्काऊ true;
                }
            }
        }
    }

    अचर KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    अचर xkb_atom_t old_name = keycode_store_get_साँचो_name(&info->keycodes, match_kc);
    यदि (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        यदि (old_name == name) {
            प्रतिज्ञा(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            यदि (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         साँचो_Name_Text(info->ctx, old_name), kc);
            फर्काऊ true;
        }

        अचर bool clobber = (merge != MERGE_AUGMENT);
        यदि (report) {
            अचर वर्ण* अचर kname     = साँचो_Name_Text(info->ctx, name);
            अचर वर्ण* अचर old_kname = साँचो_Name_Text(info->ctx, old_name);
            अचर वर्ण* अचर use    = (clobber) ? kname : old_kname;
            अचर वर्ण* अचर ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        यदि (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_साँचो(&info->keycodes, match_kc, name);
        }
    } अन्यथा {
        /* No previous keycode */
        यदि (!keycode_store_घुसाउ_साँचो(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            फर्काऊ false;
        }
    }

    फर्काऊ true;
}

/***====================================================================***/

स्थिर bool
HandleAliasDef(साँचो_Names_Info *info, अचर साँचो_Alias_Def *def, bool report);

स्थिर शून्य
MergeKeycodeStores(साँचो_Names_Info *into, साँचो_Names_Info *from,
                   गणना merge_mode merge, bool report)
{
    यदि (darray_empty(into->keycodes.तल्लो) && darray_empty(into->keycodes.माथिल्लो) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.तल्लो);
        darray_init(from->keycodes.माथिल्लो);
        darray_init(from->keycodes.names);
    } अन्यथा {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        लागि (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.तल्लो);
             kc++) {
            /* Skip undefined keycodes */
            अचर xkb_atom_t name = darray_item(from->keycodes.तल्लो, kc);
            यदि (name == XKB_ATOM_NONE)
                जारी;

            यदि (!Add_साँचो_Name(into, kc, name, merge, report))
                into->error_गन्ती++;
        }

        /* High keycodes */
        अचर माथिल्लो_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.माथिल्लो) {
            प्रतिज्ञा(new->name != XKB_ATOM_NONE);
            यदि (!Add_साँचो_Name(into, new->keycode, new->name, merge, report))
                into->error_गन्ती++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            यदि (!match->found || !match->is_alias)
                जारी;

            अचर साँचो_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            यदि (!HandleAliasDef(into, &def, report))
                into->error_गन्ती++;
        }
    }
}

स्थिर शून्य
MergeIncludedKeycodes(साँचो_Names_Info *into, साँचो_Names_Info *from,
                      गणना merge_mode merge, bool report)
{
    यदि (from->error_गन्ती > 0) {
        into->error_गन्ती += from->error_गन्ती;
        फर्काऊ;
    }

    यदि (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    यदि (into->अङ्क_led_names == 0) {
        स_नकल(into->led_names, from->led_names,
               परिमाण(*from->led_names) * from->अङ्क_led_names);
        into->अङ्क_led_names = from->अङ्क_led_names;
        from->अङ्क_led_names = 0;
    }
    अन्यथा {
        लागि (xkb_led_index_t idx = 0; idx < from->अङ्क_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            यदि (ledi->name == XKB_ATOM_NONE)
                जारी;

            ledi->merge = merge;
            यदि (!AddLedName(into, false, ledi, idx, report))
                into->error_गन्ती++;
        }
    }
}

स्थिर शून्य
HandleKeycodesFile(साँचो_Names_Info *info, XkbFile *file);

स्थिर bool
HandleIncludeKeycodes(साँचो_Names_Info *info, IncludeStmt *include, bool report)
{
    साँचो_Names_Info included;

    यदि (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गन्ती += 10;
        फर्काऊ false;
    }

    Init_साँचो_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    अचर संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    लागि (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        साँचो_Names_Info next_incl;
        XkbFile *file;

        वर्ण path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, परिमाण(path));
        यदि (!file) {
            info->error_गन्ती += 10;
            Clear_साँचो_Names_Info(&included);
            फर्काऊ false;
        }

        Init_साँचो_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_साँचो_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_साँचो_Names_Info(&included);

    फर्काऊ (info->error_गन्ती == 0);
}

स्थिर bool
HandleKeycodeDef(साँचो_Names_Info *info, KeycodeDef *stmt, bool report)
{
    यदि (stmt->मान < 0 || stmt->मान > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->मान, XKB_KEYCODE_MAX);
        फर्काऊ false;
    }

    फर्काऊ Add_साँचो_Name(info, (xkb_keycode_t) stmt->मान,
                      stmt->name, stmt->merge, report);
}

स्थिर bool
HandleAliasDef(साँचो_Names_Info *info, अचर साँचो_Alias_Def *def, bool report)
{
    अचर KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    यदि (match_name.found) {
        अचर bool clobber = (def->merge != MERGE_AUGMENT);
        यदि (match_name.is_alias) {
            यदि (def->real == match_name.alias.real) {
                यदि (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_साँचो_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             साँचो_Name_Text(info->ctx, def->alias),
                             साँचो_Name_Text(info->ctx, def->real));
            } अन्यथा {
                अचर xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                अचर xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                यदि (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_साँचो_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             साँचो_Name_Text(info->ctx, def->alias),
                             साँचो_Name_Text(info->ctx, use),
                             साँचो_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            फर्काऊ true;
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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_साँचो_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         साँचो_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            यदि (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_साँचो(&info->keycodes, match_name);
            } अन्यथा {
                फर्काऊ true;
            }
        }
    }

    फर्काऊ keycode_store_घुसाउ_alias(&info->keycodes, def->alias, def->real);
}

स्थिर bool
Handle_साँचो_Name_Var(साँचो_Names_Info *info, VarDef *stmt)
{
    अचर वर्ण *elem, *field;
    अभिव्यक्ति_Def *arrayNdx;

    यदि (!अभिव्यक्ति_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        फर्काऊ false;

    यदि (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        फर्काऊ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    यदि (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        फर्काऊ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    यदि (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        फर्काऊ !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    यदि (!अभिव्यक्ति_Resolve_Integer(info->ctx, stmt->मान, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        फर्काऊ !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    फर्काऊ true;
}

स्थिर bool
HandleLedNameDef(साँचो_Names_Info *info, LedNameDef *def, bool report)
{
    यदि (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_गन्ती++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        फर्काऊ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    यदि (!अभिव्यक्ति_Resolve_String(info->ctx, def->name, &name)) {
        वर्ण buf[20];
        snprintf(buf, परिमाण(buf), "%"PRId64, def->ndx);
        info->error_गन्ती++;
        फर्काऊ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    फर्काऊ AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

स्थिर शून्य
HandleKeycodesFile(साँचो_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    अचर पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    अचर bool report_same_file = verbosity > 0;
    अचर bool report_include   = verbosity > 7;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    लागि (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        विकल्प (stmt->type) {
        पक्ष STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            रोक;
        पक्ष STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            रोक;
        पक्ष STMT_ALIAS:
            ok = HandleAliasDef(info, (साँचो_Alias_Def *) stmt, report_same_file);
            रोक;
        पक्ष STMT_VAR:
            ok = Handle_साँचो_Name_Var(info, (VarDef *) stmt);
            रोक;
        पक्ष STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            रोक;
        पक्ष STMT_UNKNOWN_DECLARATION:
        पक्ष STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            रोक;
        स्वभाव:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            रोक;
        }

        यदि (!ok)
            info->error_गन्ती++;

        यदि (info->error_गन्ती > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            रोक;
        }
    }
}

/***====================================================================***/

स्थिर bool
Copy_साँचो_Names_To_Keymap(संरचना xkb_keymap *keymap, साँचो_Names_Info *info)
{
    यदि (darray_empty(info->keycodes.तल्लो) && darray_empty(info->keycodes.माथिल्लो)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        प्रतिज्ञा(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_साँचो_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_साँचो_code = 255;
        keymap->अङ्क_साँचोहरू = keymap->अङ्क_साँचोहरू_तल्लो = keymap->max_साँचो_code + 1;
    } अन्यथा {
        प्रतिज्ञा(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_साँचो_code = info->keycodes.min;
        keymap->max_साँचो_code = (darray_empty(info->keycodes.माथिल्लो))
            ? darray_size(info->keycodes.तल्लो) - 1
            : darray_item(
                info->keycodes.माथिल्लो,
                darray_size(info->keycodes.माथिल्लो) - 1
            ).keycode;
        keymap->अङ्क_साँचोहरू_तल्लो = darray_size(info->keycodes.तल्लो);
        keymap->अङ्क_साँचोहरू = keymap->अङ्क_साँचोहरू_तल्लो
                         + darray_size(info->keycodes.माथिल्लो);
    }

    संरचना xkb_साँचो * अचर साँचोहरू = सुस्मृति(keymap->अङ्क_साँचोहरू, परिमाण(*साँचोहरू));
    यदि (!साँचोहरू) {
        keymap->अङ्क_साँचोहरू = 0;
        keymap->min_साँचो_code = keymap->max_साँचो_code = XKB_KEYCODE_INVALID;
        फर्काऊ false;
    }

    /* Low keycodes */
    लागि (xkb_keycode_t kc = keymap->min_साँचो_code; kc < keymap->अङ्क_साँचोहरू_तल्लो; kc++)
        साँचोहरू[kc].keycode = kc;

    लागि (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.तल्लो);
         kc++)
        साँचोहरू[kc].name = darray_item(info->keycodes.तल्लो, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->अङ्क_साँचोहरू_तल्लो; /* First high keycode index */
    अचर माथिल्लो_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.माथिल्लो) {
        प्रतिज्ञा(entry->name != XKB_ATOM_NONE);
        साँचोहरू[idx].keycode = entry->keycode;
        साँचोहरू[idx].name = entry->name;
        idx++;
    }

    keymap->साँचोहरू = साँचोहरू;
    फर्काऊ true;
}

स्थिर bool
CopyKeycodeNameLUT(संरचना xkb_keymap *keymap, साँचो_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        यदि (!match->found)
            जारी;

        यदि (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            अचर KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            यदि (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        साँचो_Name_Text(info->ctx, name),
                        साँचो_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                जारी;
            }
            प्रतिज्ञा(!match_real.is_alias);
        } अन्यथा यदि (!match->साँचो.तल्लो) {
            /* Update to final index in keymap::keys */
            match->साँचो.index += keymap->अङ्क_साँचोहरू_तल्लो;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->अङ्क_साँचो_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->साँचो_names, रिक्त);
    darray_init(info->keycodes.names);
    फर्काऊ true;
}

स्थिर bool
CopyLedNamesToKeymap(संरचना xkb_keymap *keymap, साँचो_Names_Info *info)
{
    keymap->अङ्क_leds = info->अङ्क_led_names;
    लागि (xkb_led_index_t idx = 0; idx < info->अङ्क_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        यदि (ledi->name == XKB_ATOM_NONE)
            जारी;

        keymap->leds[idx].name = ledi->name;
    }

    फर्काऊ true;
}

स्थिर bool
Copy_साँचो_Names_Info_To_Keymap(संरचना xkb_keymap *keymap, साँचो_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    यदि (!Copy_साँचो_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        फर्काऊ false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    यदि (!keymap->अङ्क_साँचोहरू ||
        keymap->min_साँचो_code > 0) {
        keymap->redirect_साँचो_auto = 0;
    } अन्यथा {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        लागि (xkb_keycode_t k = keymap->अङ्क_साँचोहरू; k-- > keymap->अङ्क_साँचोहरू_तल्लो;) {
            यदि (keycode > keymap->साँचोहरू[k].keycode)
                रोक;
            /* Try next keycode downwards */
            keycode = keymap->साँचोहरू[k].keycode - 1;
        }
        keymap->redirect_साँचो_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    फर्काऊ true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    साँचो_Names_Info info;

    Init_साँचो_Names_Info(&info, keymap_info, 0);

    यदि (file != रिक्त)
        HandleKeycodesFile(&info, file);

    यदि (info.error_गन्ती != 0)
        गच्छ err_info;

    यदि (!Copy_साँचो_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    Clear_साँचो_Names_Info(&info);
    फर्काऊ true;

err_info:
    Clear_साँचो_Names_Info(&info);
    फर्काऊ false;
}
