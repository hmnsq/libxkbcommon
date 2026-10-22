/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"
#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "expr.h"
#അന്തര്ഭാവ "include.h"
#അന്തര്ഭാവ "util-mem.h"

തരനിർവചനം ഘടന {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} മുകളിലെ_Keycode_Entry;

തരനിർവചനം ഘടന {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) താഴെയുള്ള;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(മുകളിലെ_Keycode_Entry) മുകളിലെ;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

സ്ഥിതം വരിയിൽ ശൂന്യം
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->താഴെയുള്ള);
    darray_init(store->മുകളിലെ);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->താഴെയുള്ള);
    darray_free(store->മുകളിലെ);
    darray_free(store->names);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
keycode_store_update_താക്കോൽ(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    എങ്കിൽ (unlikely(!match.found || match.is_alias)) {
        തിരികെ;
    } അല്ലെങ്കിൽ എങ്കിൽ (match.താക്കോൽ.താഴെയുള്ള) {
        /* assume valid index */
        പ്രതിജ്ഞാ(match.താക്കോൽ.index < darray_size(store->താഴെയുള്ള));
        darray_item(store->താഴെയുള്ള, match.താക്കോൽ.index) = name;
    } അല്ലെങ്കിൽ {
        /* assume valid index */
        പ്രതിജ്ഞാ(match.താക്കോൽ.index < darray_size(store->മുകളിലെ));
        darray_item(store->മുകളിലെ, match.താക്കോൽ.index).name = name;
    }

    /* Names lookup */
    എങ്കിൽ (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

സ്ഥിതം bool
keycode_store_തിരുകുക_താക്കോൽ(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    എങ്കിൽ (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    എങ്കിൽ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        എങ്കിൽ (kc >= (xkb_keycode_t) darray_size(store->താഴെയുള്ള))
            darray_resize0(store->താഴെയുള്ള, kc + 1);
        darray_item(store->താഴെയുള്ള, kc) = name;
        എങ്കിൽ (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .താക്കോൽ = {
                .found = true,
                .താഴെയുള്ള = true,
                .is_alias = false,
                .index = kc
            }
        };
    } അല്ലെങ്കിൽ {
        /* High keycode: insert into a sorted list */
        സ്ഥിരം darray_size_t idx = darray_size(store->മുകളിലെ);
        എങ്കിൽ (idx && darray_item(store->മുകളിലെ, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            ആകുമ്പോൾ (lower < upper) {
                സ്ഥിരം darray_size_t mid = lower + (upper - 1 - lower) / 2;
                സ്ഥിരം മുകളിലെ_Keycode_Entry * സ്ഥിരം entry =
                    &darray_item(store->മുകളിലെ, mid);
                എങ്കിൽ (entry->keycode < kc) {
                    lower = mid + 1;
                } അല്ലെങ്കിൽ എങ്കിൽ (entry->keycode > kc) {
                    upper = mid;
                } അല്ലെങ്കിൽ {
                    /* Unreachable: there is no repetition in the list */
                    പ്രതിജ്ഞാ(entry->keycode != kc);
                }
            }
            പ്രതിജ്ഞാ(lower < idx);
            പ്രതിജ്ഞാ(darray_item(store->മുകളിലെ, lower).keycode > kc);

            /* Update references to entries that will be moved */
            മുകളിലെ_Keycode_Entry *entry;
            darray_foreach_from(entry, store->മുകളിലെ, lower) {
                darray_item(store->names, entry->name).താക്കോൽ.index++;
            }

            darray_തിരുകുക(store->മുകളിലെ, lower,
                          (മുകളിലെ_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .താക്കോൽ = {
                    .found = true,
                    .താഴെയുള്ള = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } അല്ലെങ്കിൽ {
            /* Fast path: no need to sort */
            darray_append(store->മുകളിലെ,
                          (മുകളിലെ_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .താക്കോൽ = {
                    .found = true,
                    .താഴെയുള്ള = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        എങ്കിൽ (darray_empty(store->താഴെയുള്ള))
            store->min = darray_item(store->മുകളിലെ, 0).keycode;
    }
    തിരികെ true;
}

സ്ഥിതം വരിയിൽ bool
keycode_store_തിരുകുക_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    എങ്കിൽ (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    തിരികെ true;
}

സ്ഥിതം വരിയിൽ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    തിരികെ true;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
keycode_store_delete_name(സ്ഥിരം KeycodeStore *store, xkb_atom_t name)
{
    എങ്കിൽ (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

സ്ഥിതം ശൂന്യം
keycode_store_delete_താക്കോൽ(KeycodeStore *store, സ്ഥിരം KeycodeMatch match)
{
    എങ്കിൽ (unlikely(!match.found || match.is_alias)) {
        തിരികെ;
    } അല്ലെങ്കിൽ എങ്കിൽ (match.താക്കോൽ.താഴെയുള്ള) {
        /* assume valid index */
        പ്രതിജ്ഞാ(match.താക്കോൽ.index < darray_size(store->താഴെയുള്ള));
        darray_item(store->names,
                    darray_item(store->താഴെയുള്ള, match.താക്കോൽ.index)).found = false;
        എങ്കിൽ (match.താക്കോൽ.index + 1u == darray_size(store->താഴെയുള്ള)) {
            /* Highest low keycode: shrink */
            എങ്കിൽ (store->min == match.താക്കോൽ.index) {
                /* No low keycode left */
                darray_size(store->താഴെയുള്ള) = 0;
            } അല്ലെങ്കിൽ {
                /* Look for previous defined low keycode */
                വേണ്ടി (darray_size_t idx = match.താക്കോൽ.index; idx > 0; idx--) {
                    എങ്കിൽ (darray_item(store->താഴെയുള്ള, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->താഴെയുള്ള) = idx;
                        നിർത്തുക;
                    }
                }
            }
        } അല്ലെങ്കിൽ {
            /* Lower keycode: reset */
            darray_item(store->താഴെയുള്ള, match.താക്കോൽ.index) = XKB_ATOM_NONE;
        }
    } അല്ലെങ്കിൽ {
        പ്രതിജ്ഞാ(match.താക്കോൽ.index < darray_size(store->മുകളിലെ)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->മുകളിലെ, match.താക്കോൽ.index).name).found = false;
        darray_remove(store->മുകളിലെ, match.താക്കോൽ.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            എങ്കിൽ (entry->found && !entry->is_alias && !entry->താക്കോൽ.താഴെയുള്ള &&
                entry->താക്കോൽ.index > match.താക്കോൽ.index) {
                entry->താക്കോൽ.index--;
            }
        }
    }

    /* Update bounds */
    എങ്കിൽ (darray_empty(store->താഴെയുള്ള)) {
        store->min = (darray_empty(store->മുകളിലെ))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->മുകളിലെ, 0).keycode;
    } അല്ലെങ്കിൽ {
        വേണ്ടി (xkb_keycode_t kc = store->min; kc < darray_size(store->താഴെയുള്ള); kc++) {
            എങ്കിൽ (darray_item(store->താഴെയുള്ള, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                നിർത്തുക;
            }
        }
    }
}

സ്ഥിതം വരിയിൽ xkb_keycode_t
keycode_store_get_keycode(സ്ഥിരം KeycodeStore *store, KeycodeMatch match)
{
    എങ്കിൽ (!match.found || match.is_alias) {
        തിരികെ XKB_KEYCODE_INVALID;
    } അല്ലെങ്കിൽ എങ്കിൽ (match.താക്കോൽ.താഴെയുള്ള) {
        /* assume valid index */
        പ്രതിജ്ഞാ(match.താക്കോൽ.index < darray_size(store->താഴെയുള്ള));
        തിരികെ (xkb_keycode_t) match.താക്കോൽ.index;
    } അല്ലെങ്കിൽ {
        /* assume valid index */
        പ്രതിജ്ഞാ(match.താക്കോൽ.index < darray_size(store->മുകളിലെ));
        തിരികെ darray_item(store->മുകളിലെ, match.താക്കോൽ.index).keycode;
    }
}

സ്ഥിതം വരിയിൽ xkb_atom_t
keycode_store_get_താക്കോൽ_name(സ്ഥിരം KeycodeStore *store, KeycodeMatch match)
{
    എങ്കിൽ (!match.found || match.is_alias) {
        തിരികെ XKB_ATOM_NONE;
    } അല്ലെങ്കിൽ എങ്കിൽ (match.താക്കോൽ.താഴെയുള്ള) {
        /* assume valid index */
        പ്രതിജ്ഞാ(match.താക്കോൽ.index < darray_size(store->താഴെയുള്ള));
        തിരികെ darray_item(store->താഴെയുള്ള, match.താക്കോൽ.index);
    } അല്ലെങ്കിൽ {
        /* assume valid index */
        പ്രതിജ്ഞാ(match.താക്കോൽ.index < darray_size(store->മുകളിലെ));
        തിരികെ darray_item(store->മുകളിലെ, match.താക്കോൽ.index).name;
    }
}

സ്ഥിതം KeycodeMatch
keycode_store_lookup_keycode(സ്ഥിരം KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    എങ്കിൽ (kc < (xkb_keycode_t) darray_size(store->താഴെയുള്ള)) {
        തിരികെ (KeycodeMatch) {
            .താക്കോൽ = {
                .found = true,
                .താഴെയുള്ള = true,
                .is_alias = false,
                .index = kc
            }
        };
    } അല്ലെങ്കിൽ എങ്കിൽ (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        തിരികെ (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->മുകളിലെ);
    ആകുമ്പോൾ (lower < upper) {
        സ്ഥിരം darray_size_t mid = lower + (upper - 1 - lower) / 2;
        മുകളിലെ_Keycode_Entry * സ്ഥിരം entry = &darray_item(store->മുകളിലെ, mid);
        എങ്കിൽ (entry->keycode < kc) {
            lower = mid + 1;
        } അല്ലെങ്കിൽ എങ്കിൽ (entry->keycode > kc) {
            upper = mid;
        } അല്ലെങ്കിൽ {
            തിരികെ (KeycodeMatch) {
                .താക്കോൽ = {
                    .found = true,
                    .താഴെയുള്ള = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    തിരികെ (KeycodeMatch) { .found = false };
}

സ്ഥിതം KeycodeMatch
keycode_store_lookup_name(സ്ഥിരം KeycodeStore *store, xkb_atom_t name)
{
    എങ്കിൽ (name >= darray_size(store->names)) {
        തിരികെ (KeycodeMatch) { .found = false, .is_alias = false };
    } അല്ലെങ്കിൽ {
        തിരികെ darray_item(store->names, name);
    }
}

/***====================================================================***/

തരനിർവചനം ഘടന {
    എണ്ണൽ merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

തരനിർവചനം ഘടന {
    അക്ഷരം *name;
    പൂർണ്ണസംഖ്യ error_എണ്ണം;
    unsigned പൂർണ്ണസംഖ്യ include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t അക്കം_led_names;

    ഘടന xkb_context *ctx;
    സ്ഥിരം ഘടന xkb_keymap_info *keymap_info;
} താക്കോൽ_Names_Info;

/***====================================================================***/

സ്ഥിതം LedNameInfo *
FindLedByName(താക്കോൽ_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    വേണ്ടി (xkb_led_index_t idx = 0; idx < info->അക്കം_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        എങ്കിൽ (ledi->name == name) {
            *idx_out = idx;
            തിരികെ ledi;
        }
    }

    തിരികെ ശൂന്യ;
}

സ്ഥിതം bool
AddLedName(താക്കോൽ_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    സ്ഥിരം bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    എങ്കിൽ (old) {
        എങ്കിൽ (old_idx == new_idx) {
            എങ്കിൽ (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            തിരികെ true;
        }

        എങ്കിൽ (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        എങ്കിൽ (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } അല്ലെങ്കിൽ {
            തിരികെ true;
        }
    }

    എങ്കിൽ (new_idx >= info->അക്കം_led_names)
        info->അക്കം_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    എങ്കിൽ (old->name != XKB_ATOM_NONE) {
        എങ്കിൽ (report) {
            സ്ഥിരം xkb_atom_t use    = (replace ? new->name : old->name);
            സ്ഥിരം xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        എങ്കിൽ (replace)
            *old = *new;

        തിരികെ true;
    }

    *old = *new;
    തിരികെ true;
}

സ്ഥിതം ശൂന്യം
Clear_താക്കോൽ_Names_Info(താക്കോൽ_Names_Info *info)
{
    സ്വതന്ത്രം(info->name);
    keycode_store_free(&info->keycodes);
}

സ്ഥിതം ശൂന്യം
Init_താക്കോൽ_Names_Info(താക്കോൽ_Names_Info *info, സ്ഥിരം ഘടന xkb_keymap_info *keymap_info,
                 unsigned പൂർണ്ണസംഖ്യ include_depth)
{
    memset(info, 0, വലുപ്പം(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

സ്ഥിതം bool
Add_താക്കോൽ_Name(താക്കോൽ_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           എണ്ണൽ merge_mode merge, bool report)
{
#ifdef NDEBUG
    സ്ഥിരം
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    എങ്കിൽ (match_name.found) {
        സ്ഥിരം bool clobber = (merge != MERGE_AUGMENT);

        എങ്കിൽ (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            എങ്കിൽ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         താക്കോൽ_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            എങ്കിൽ (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } അല്ലെങ്കിൽ {
                തിരികെ true;
            }
        } അല്ലെങ്കിൽ {
            സ്ഥിരം xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            പ്രതിജ്ഞാ(old_kc != XKB_KEYCODE_INVALID);
            എങ്കിൽ (old_kc != kc) {
                /* There is already a different key with this name. */

                എങ്കിൽ (report) {
                    സ്ഥിരം xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    സ്ഥിരം xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            താക്കോൽ_Name_Text(info->ctx, name), use, ignore);
                }

                എങ്കിൽ (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_താക്കോൽ(&info->keycodes, match_name);
                } അല്ലെങ്കിൽ {
                    തിരികെ true;
                }
            }
        }
    }

    സ്ഥിരം KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    സ്ഥിരം xkb_atom_t old_name = keycode_store_get_താക്കോൽ_name(&info->keycodes, match_kc);
    എങ്കിൽ (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        എങ്കിൽ (old_name == name) {
            പ്രതിജ്ഞാ(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            എങ്കിൽ (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         താക്കോൽ_Name_Text(info->ctx, old_name), kc);
            തിരികെ true;
        }

        സ്ഥിരം bool clobber = (merge != MERGE_AUGMENT);
        എങ്കിൽ (report) {
            സ്ഥിരം അക്ഷരം* സ്ഥിരം kname     = താക്കോൽ_Name_Text(info->ctx, name);
            സ്ഥിരം അക്ഷരം* സ്ഥിരം old_kname = താക്കോൽ_Name_Text(info->ctx, old_name);
            സ്ഥിരം അക്ഷരം* സ്ഥിരം use    = (clobber) ? kname : old_kname;
            സ്ഥിരം അക്ഷരം* സ്ഥിരം ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        എങ്കിൽ (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_താക്കോൽ(&info->keycodes, match_kc, name);
        }
    } അല്ലെങ്കിൽ {
        /* No previous keycode */
        എങ്കിൽ (!keycode_store_തിരുകുക_താക്കോൽ(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            തിരികെ false;
        }
    }

    തിരികെ true;
}

/***====================================================================***/

സ്ഥിതം bool
HandleAliasDef(താക്കോൽ_Names_Info *info, സ്ഥിരം താക്കോൽ_Alias_Def *def, bool report);

സ്ഥിതം ശൂന്യം
MergeKeycodeStores(താക്കോൽ_Names_Info *into, താക്കോൽ_Names_Info *from,
                   എണ്ണൽ merge_mode merge, bool report)
{
    എങ്കിൽ (darray_empty(into->keycodes.താഴെയുള്ള) && darray_empty(into->keycodes.മുകളിലെ) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.താഴെയുള്ള);
        darray_init(from->keycodes.മുകളിലെ);
        darray_init(from->keycodes.names);
    } അല്ലെങ്കിൽ {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        വേണ്ടി (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.താഴെയുള്ള);
             kc++) {
            /* Skip undefined keycodes */
            സ്ഥിരം xkb_atom_t name = darray_item(from->keycodes.താഴെയുള്ള, kc);
            എങ്കിൽ (name == XKB_ATOM_NONE)
                തുടരുക;

            എങ്കിൽ (!Add_താക്കോൽ_Name(into, kc, name, merge, report))
                into->error_എണ്ണം++;
        }

        /* High keycodes */
        സ്ഥിരം മുകളിലെ_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.മുകളിലെ) {
            പ്രതിജ്ഞാ(new->name != XKB_ATOM_NONE);
            എങ്കിൽ (!Add_താക്കോൽ_Name(into, new->keycode, new->name, merge, report))
                into->error_എണ്ണം++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            എങ്കിൽ (!match->found || !match->is_alias)
                തുടരുക;

            സ്ഥിരം താക്കോൽ_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            എങ്കിൽ (!HandleAliasDef(into, &def, report))
                into->error_എണ്ണം++;
        }
    }
}

സ്ഥിതം ശൂന്യം
MergeIncludedKeycodes(താക്കോൽ_Names_Info *into, താക്കോൽ_Names_Info *from,
                      എണ്ണൽ merge_mode merge, bool report)
{
    എങ്കിൽ (from->error_എണ്ണം > 0) {
        into->error_എണ്ണം += from->error_എണ്ണം;
        തിരികെ;
    }

    എങ്കിൽ (into->name == ശൂന്യ) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    എങ്കിൽ (into->അക്കം_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               വലുപ്പം(*from->led_names) * from->അക്കം_led_names);
        into->അക്കം_led_names = from->അക്കം_led_names;
        from->അക്കം_led_names = 0;
    }
    അല്ലെങ്കിൽ {
        വേണ്ടി (xkb_led_index_t idx = 0; idx < from->അക്കം_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            എങ്കിൽ (ledi->name == XKB_ATOM_NONE)
                തുടരുക;

            ledi->merge = merge;
            എങ്കിൽ (!AddLedName(into, false, ledi, idx, report))
                into->error_എണ്ണം++;
        }
    }
}

സ്ഥിതം ശൂന്യം
HandleKeycodesFile(താക്കോൽ_Names_Info *info, XkbFile *file);

സ്ഥിതം bool
HandleIncludeKeycodes(താക്കോൽ_Names_Info *info, IncludeStmt *include, bool report)
{
    താക്കോൽ_Names_Info included;

    എങ്കിൽ (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_എണ്ണം += 10;
        തിരികെ false;
    }

    Init_താക്കോൽ_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    സ്ഥിരം ഘടന parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    വേണ്ടി (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        താക്കോൽ_Names_Info next_incl;
        XkbFile *file;

        അക്ഷരം path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, വലുപ്പം(path));
        എങ്കിൽ (!file) {
            info->error_എണ്ണം += 10;
            Clear_താക്കോൽ_Names_Info(&included);
            തിരികെ false;
        }

        Init_താക്കോൽ_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_താക്കോൽ_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_താക്കോൽ_Names_Info(&included);

    തിരികെ (info->error_എണ്ണം == 0);
}

സ്ഥിതം bool
HandleKeycodeDef(താക്കോൽ_Names_Info *info, KeycodeDef *stmt, bool report)
{
    എങ്കിൽ (stmt->മൂല്യം < 0 || stmt->മൂല്യം > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->മൂല്യം, XKB_KEYCODE_MAX);
        തിരികെ false;
    }

    തിരികെ Add_താക്കോൽ_Name(info, (xkb_keycode_t) stmt->മൂല്യം,
                      stmt->name, stmt->merge, report);
}

സ്ഥിതം bool
HandleAliasDef(താക്കോൽ_Names_Info *info, സ്ഥിരം താക്കോൽ_Alias_Def *def, bool report)
{
    സ്ഥിരം KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    എങ്കിൽ (match_name.found) {
        സ്ഥിരം bool clobber = (def->merge != MERGE_AUGMENT);
        എങ്കിൽ (match_name.is_alias) {
            എങ്കിൽ (def->real == match_name.alias.real) {
                എങ്കിൽ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             താക്കോൽ_Name_Text(info->ctx, def->alias),
                             താക്കോൽ_Name_Text(info->ctx, def->real));
            } അല്ലെങ്കിൽ {
                സ്ഥിരം xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                സ്ഥിരം xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                എങ്കിൽ (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             താക്കോൽ_Name_Text(info->ctx, def->alias),
                             താക്കോൽ_Name_Text(info->ctx, use),
                             താക്കോൽ_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            തിരികെ true;
        } അല്ലെങ്കിൽ {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            എങ്കിൽ (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_താക്കോൽ_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         താക്കോൽ_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            എങ്കിൽ (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_താക്കോൽ(&info->keycodes, match_name);
            } അല്ലെങ്കിൽ {
                തിരികെ true;
            }
        }
    }

    തിരികെ keycode_store_തിരുകുക_alias(&info->keycodes, def->alias, def->real);
}

സ്ഥിതം bool
Handle_താക്കോൽ_Name_Var(താക്കോൽ_Names_Info *info, VarDef *stmt)
{
    സ്ഥിരം അക്ഷരം *elem, *field;
    വ്യഞ്ജകം_Def *arrayNdx;

    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        തിരികെ false;

    എങ്കിൽ (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        തിരികെ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    എങ്കിൽ (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        തിരികെ !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    എങ്കിൽ (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        തിരികെ !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_Integer(info->ctx, stmt->മൂല്യം, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        തിരികെ !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    തിരികെ true;
}

സ്ഥിതം bool
HandleLedNameDef(താക്കോൽ_Names_Info *info, LedNameDef *def, bool report)
{
    എങ്കിൽ (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_എണ്ണം++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        തിരികെ false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    എങ്കിൽ (!വ്യഞ്ജകം_Resolve_String(info->ctx, def->name, &name)) {
        അക്ഷരം buf[20];
        snprintf(buf, വലുപ്പം(buf), "%"PRId64, def->ndx);
        info->error_എണ്ണം++;
        തിരികെ ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    തിരികെ AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

സ്ഥിതം ശൂന്യം
HandleKeycodesFile(താക്കോൽ_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    സ്ഥിരം പൂർണ്ണസംഖ്യ verbosity = xkb_context_get_log_verbosity(info->ctx);
    സ്ഥിരം bool report_same_file = verbosity > 0;
    സ്ഥിരം bool report_include   = verbosity > 7;

    സ്വതന്ത്രം(info->name);
    info->name = strdup_safe(file->name);

    വേണ്ടി (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        തിരഞ്ഞെടുപ്പ് (stmt->type) {
        അവസ്ഥ STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            നിർത്തുക;
        അവസ്ഥ STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            നിർത്തുക;
        അവസ്ഥ STMT_ALIAS:
            ok = HandleAliasDef(info, (താക്കോൽ_Alias_Def *) stmt, report_same_file);
            നിർത്തുക;
        അവസ്ഥ STMT_VAR:
            ok = Handle_താക്കോൽ_Name_Var(info, (VarDef *) stmt);
            നിർത്തുക;
        അവസ്ഥ STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            നിർത്തുക;
        അവസ്ഥ STMT_UNKNOWN_DECLARATION:
        അവസ്ഥ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            നിർത്തുക;
        }

        എങ്കിൽ (!ok)
            info->error_എണ്ണം++;

        എങ്കിൽ (info->error_എണ്ണം > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            നിർത്തുക;
        }
    }
}

/***====================================================================***/

സ്ഥിതം bool
Copy_താക്കോൽ_Names_To_Keymap(ഘടന xkb_keymap *keymap, താക്കോൽ_Names_Info *info)
{
    എങ്കിൽ (darray_empty(info->keycodes.താഴെയുള്ള) && darray_empty(info->keycodes.മുകളിലെ)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        പ്രതിജ്ഞാ(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_താക്കോൽ_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_താക്കോൽ_code = 255;
        keymap->അക്കം_താക്കോലുകൾ = keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള = keymap->max_താക്കോൽ_code + 1;
    } അല്ലെങ്കിൽ {
        പ്രതിജ്ഞാ(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_താക്കോൽ_code = info->keycodes.min;
        keymap->max_താക്കോൽ_code = (darray_empty(info->keycodes.മുകളിലെ))
            ? darray_size(info->keycodes.താഴെയുള്ള) - 1
            : darray_item(
                info->keycodes.മുകളിലെ,
                darray_size(info->keycodes.മുകളിലെ) - 1
            ).keycode;
        keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള = darray_size(info->keycodes.താഴെയുള്ള);
        keymap->അക്കം_താക്കോലുകൾ = keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള
                         + darray_size(info->keycodes.മുകളിലെ);
    }

    ഘടന xkb_താക്കോൽ * സ്ഥിരം താക്കോലുകൾ = calloc(keymap->അക്കം_താക്കോലുകൾ, വലുപ്പം(*താക്കോലുകൾ));
    എങ്കിൽ (!താക്കോലുകൾ) {
        keymap->അക്കം_താക്കോലുകൾ = 0;
        keymap->min_താക്കോൽ_code = keymap->max_താക്കോൽ_code = XKB_KEYCODE_INVALID;
        തിരികെ false;
    }

    /* Low keycodes */
    വേണ്ടി (xkb_keycode_t kc = keymap->min_താക്കോൽ_code; kc < keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള; kc++)
        താക്കോലുകൾ[kc].keycode = kc;

    വേണ്ടി (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.താഴെയുള്ള);
         kc++)
        താക്കോലുകൾ[kc].name = darray_item(info->keycodes.താഴെയുള്ള, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള; /* First high keycode index */
    സ്ഥിരം മുകളിലെ_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.മുകളിലെ) {
        പ്രതിജ്ഞാ(entry->name != XKB_ATOM_NONE);
        താക്കോലുകൾ[idx].keycode = entry->keycode;
        താക്കോലുകൾ[idx].name = entry->name;
        idx++;
    }

    keymap->താക്കോലുകൾ = താക്കോലുകൾ;
    തിരികെ true;
}

സ്ഥിതം bool
CopyKeycodeNameLUT(ഘടന xkb_keymap *keymap, താക്കോൽ_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        എങ്കിൽ (!match->found)
            തുടരുക;

        എങ്കിൽ (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            സ്ഥിരം KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            എങ്കിൽ (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        താക്കോൽ_Name_Text(info->ctx, name),
                        താക്കോൽ_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                തുടരുക;
            }
            പ്രതിജ്ഞാ(!match_real.is_alias);
        } അല്ലെങ്കിൽ എങ്കിൽ (!match->താക്കോൽ.താഴെയുള്ള) {
            /* Update to final index in keymap::keys */
            match->താക്കോൽ.index += keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->അക്കം_താക്കോൽ_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->താക്കോൽ_names, ശൂന്യ);
    darray_init(info->keycodes.names);
    തിരികെ true;
}

സ്ഥിതം bool
CopyLedNamesToKeymap(ഘടന xkb_keymap *keymap, താക്കോൽ_Names_Info *info)
{
    keymap->അക്കം_leds = info->അക്കം_led_names;
    വേണ്ടി (xkb_led_index_t idx = 0; idx < info->അക്കം_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        എങ്കിൽ (ledi->name == XKB_ATOM_NONE)
            തുടരുക;

        keymap->leds[idx].name = ledi->name;
    }

    തിരികെ true;
}

സ്ഥിതം bool
Copy_താക്കോൽ_Names_Info_To_Keymap(ഘടന xkb_keymap *keymap, താക്കോൽ_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    എങ്കിൽ (!Copy_താക്കോൽ_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        തിരികെ false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    എങ്കിൽ (!keymap->അക്കം_താക്കോലുകൾ ||
        keymap->min_താക്കോൽ_code > 0) {
        keymap->redirect_താക്കോൽ_auto = 0;
    } അല്ലെങ്കിൽ {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        വേണ്ടി (xkb_keycode_t k = keymap->അക്കം_താക്കോലുകൾ; k-- > keymap->അക്കം_താക്കോലുകൾ_താഴെയുള്ള;) {
            എങ്കിൽ (keycode > keymap->താക്കോലുകൾ[k].keycode)
                നിർത്തുക;
            /* Try next keycode downwards */
            keycode = keymap->താക്കോലുകൾ[k].keycode - 1;
        }
        keymap->redirect_താക്കോൽ_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    തിരികെ true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, ഘടന xkb_keymap_info *keymap_info)
{
    താക്കോൽ_Names_Info info;

    Init_താക്കോൽ_Names_Info(&info, keymap_info, 0);

    എങ്കിൽ (file != ശൂന്യ)
        HandleKeycodesFile(&info, file);

    എങ്കിൽ (info.error_എണ്ണം != 0)
        പോകുക err_info;

    എങ്കിൽ (!Copy_താക്കോൽ_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        പോകുക err_info;

    Clear_താക്കോൽ_Names_Info(&info);
    തിരികെ true;

err_info:
    Clear_താക്കോൽ_Names_Info(&info);
    തിരികെ false;
}
