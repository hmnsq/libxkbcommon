/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#અન્તર્ભાવ "config.h"

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ "context.h"
#અન્તર્ભાવ "messages-codes.h"
#અન્તર્ભાવ "xkbcommon/xkbcommon.h"
#અન્તર્ભાવ "xkbcomp-priv.h"
#અન્તર્ભાવ "atom.h"
#અન્તર્ભાવ "darray.h"
#અન્તર્ભાવ "keymap.h"
#અન્તર્ભાવ "text.h"
#અન્તર્ભાવ "expr.h"
#અન્તર્ભાવ "include.h"
#અન્તર્ભાવ "util-mem.h"

પ્રકારવ્યાખ્યા સંરચના {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} ઉપલું_Keycode_Entry;

પ્રકારવ્યાખ્યા સંરચના {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) નીચલું;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(ઉપલું_Keycode_Entry) ઉપલું;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

સ્થિર અંતઃરેખ શૂન્ય
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->નીચલું);
    darray_init(store->ઉપલું);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

સ્થિર અંતઃરેખ શૂન્ય
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->નીચલું);
    darray_free(store->ઉપલું);
    darray_free(store->names);
}

સ્થિર અંતઃરેખ શૂન્ય
keycode_store_update_ચાવી(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    જો (unlikely(!match.found || match.is_alias)) {
        પાછા;
    } નહીંતર જો (match.ચાવી.નીચલું) {
        /* assume valid index */
        પ્રતિજ્ઞા(match.ચાવી.index < darray_size(store->નીચલું));
        darray_item(store->નીચલું, match.ચાવી.index) = name;
    } નહીંતર {
        /* assume valid index */
        પ્રતિજ્ઞા(match.ચાવી.index < darray_size(store->ઉપલું));
        darray_item(store->ઉપલું, match.ચાવી.index).name = name;
    }

    /* Names lookup */
    જો (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

સ્થિર bool
keycode_store_ઉમેરો_ચાવી(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    જો (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    જો (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        જો (kc >= (xkb_keycode_t) darray_size(store->નીચલું))
            darray_resize0(store->નીચલું, kc + 1);
        darray_item(store->નીચલું, kc) = name;
        જો (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .ચાવી = {
                .found = true,
                .નીચલું = true,
                .is_alias = false,
                .index = kc
            }
        };
    } નહીંતર {
        /* High keycode: insert into a sorted list */
        અચલ darray_size_t idx = darray_size(store->ઉપલું);
        જો (idx && darray_item(store->ઉપલું, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            જ્યારે (lower < upper) {
                અચલ darray_size_t mid = lower + (upper - 1 - lower) / 2;
                અચલ ઉપલું_Keycode_Entry * અચલ entry =
                    &darray_item(store->ઉપલું, mid);
                જો (entry->keycode < kc) {
                    lower = mid + 1;
                } નહીંતર જો (entry->keycode > kc) {
                    upper = mid;
                } નહીંતર {
                    /* Unreachable: there is no repetition in the list */
                    પ્રતિજ્ઞા(entry->keycode != kc);
                }
            }
            પ્રતિજ્ઞા(lower < idx);
            પ્રતિજ્ઞા(darray_item(store->ઉપલું, lower).keycode > kc);

            /* Update references to entries that will be moved */
            ઉપલું_Keycode_Entry *entry;
            darray_foreach_from(entry, store->ઉપલું, lower) {
                darray_item(store->names, entry->name).ચાવી.index++;
            }

            darray_ઉમેરો(store->ઉપલું, lower,
                          (ઉપલું_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .ચાવી = {
                    .found = true,
                    .નીચલું = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } નહીંતર {
            /* Fast path: no need to sort */
            darray_append(store->ઉપલું,
                          (ઉપલું_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .ચાવી = {
                    .found = true,
                    .નીચલું = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        જો (darray_empty(store->નીચલું))
            store->min = darray_item(store->ઉપલું, 0).keycode;
    }
    પાછા true;
}

સ્થિર અંતઃરેખ bool
keycode_store_ઉમેરો_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    જો (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    પાછા true;
}

સ્થિર અંતઃરેખ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    પાછા true;
}

સ્થિર અંતઃરેખ શૂન્ય
keycode_store_delete_name(અચલ KeycodeStore *store, xkb_atom_t name)
{
    જો (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

સ્થિર શૂન્ય
keycode_store_delete_ચાવી(KeycodeStore *store, અચલ KeycodeMatch match)
{
    જો (unlikely(!match.found || match.is_alias)) {
        પાછા;
    } નહીંતર જો (match.ચાવી.નીચલું) {
        /* assume valid index */
        પ્રતિજ્ઞા(match.ચાવી.index < darray_size(store->નીચલું));
        darray_item(store->names,
                    darray_item(store->નીચલું, match.ચાવી.index)).found = false;
        જો (match.ચાવી.index + 1u == darray_size(store->નીચલું)) {
            /* Highest low keycode: shrink */
            જો (store->min == match.ચાવી.index) {
                /* No low keycode left */
                darray_size(store->નીચલું) = 0;
            } નહીંતર {
                /* Look for previous defined low keycode */
                માટે (darray_size_t idx = match.ચાવી.index; idx > 0; idx--) {
                    જો (darray_item(store->નીચલું, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->નીચલું) = idx;
                        તોડો;
                    }
                }
            }
        } નહીંતર {
            /* Lower keycode: reset */
            darray_item(store->નીચલું, match.ચાવી.index) = XKB_ATOM_NONE;
        }
    } નહીંતર {
        પ્રતિજ્ઞા(match.ચાવી.index < darray_size(store->ઉપલું)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->ઉપલું, match.ચાવી.index).name).found = false;
        darray_remove(store->ઉપલું, match.ચાવી.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            જો (entry->found && !entry->is_alias && !entry->ચાવી.નીચલું &&
                entry->ચાવી.index > match.ચાવી.index) {
                entry->ચાવી.index--;
            }
        }
    }

    /* Update bounds */
    જો (darray_empty(store->નીચલું)) {
        store->min = (darray_empty(store->ઉપલું))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->ઉપલું, 0).keycode;
    } નહીંતર {
        માટે (xkb_keycode_t kc = store->min; kc < darray_size(store->નીચલું); kc++) {
            જો (darray_item(store->નીચલું, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                તોડો;
            }
        }
    }
}

સ્થિર અંતઃરેખ xkb_keycode_t
keycode_store_get_keycode(અચલ KeycodeStore *store, KeycodeMatch match)
{
    જો (!match.found || match.is_alias) {
        પાછા XKB_KEYCODE_INVALID;
    } નહીંતર જો (match.ચાવી.નીચલું) {
        /* assume valid index */
        પ્રતિજ્ઞા(match.ચાવી.index < darray_size(store->નીચલું));
        પાછા (xkb_keycode_t) match.ચાવી.index;
    } નહીંતર {
        /* assume valid index */
        પ્રતિજ્ઞા(match.ચાવી.index < darray_size(store->ઉપલું));
        પાછા darray_item(store->ઉપલું, match.ચાવી.index).keycode;
    }
}

સ્થિર અંતઃરેખ xkb_atom_t
keycode_store_get_ચાવી_name(અચલ KeycodeStore *store, KeycodeMatch match)
{
    જો (!match.found || match.is_alias) {
        પાછા XKB_ATOM_NONE;
    } નહીંતર જો (match.ચાવી.નીચલું) {
        /* assume valid index */
        પ્રતિજ્ઞા(match.ચાવી.index < darray_size(store->નીચલું));
        પાછા darray_item(store->નીચલું, match.ચાવી.index);
    } નહીંતર {
        /* assume valid index */
        પ્રતિજ્ઞા(match.ચાવી.index < darray_size(store->ઉપલું));
        પાછા darray_item(store->ઉપલું, match.ચાવી.index).name;
    }
}

સ્થિર KeycodeMatch
keycode_store_lookup_keycode(અચલ KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    જો (kc < (xkb_keycode_t) darray_size(store->નીચલું)) {
        પાછા (KeycodeMatch) {
            .ચાવી = {
                .found = true,
                .નીચલું = true,
                .is_alias = false,
                .index = kc
            }
        };
    } નહીંતર જો (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        પાછા (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->ઉપલું);
    જ્યારે (lower < upper) {
        અચલ darray_size_t mid = lower + (upper - 1 - lower) / 2;
        ઉપલું_Keycode_Entry * અચલ entry = &darray_item(store->ઉપલું, mid);
        જો (entry->keycode < kc) {
            lower = mid + 1;
        } નહીંતર જો (entry->keycode > kc) {
            upper = mid;
        } નહીંતર {
            પાછા (KeycodeMatch) {
                .ચાવી = {
                    .found = true,
                    .નીચલું = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    પાછા (KeycodeMatch) { .found = false };
}

સ્થિર KeycodeMatch
keycode_store_lookup_name(અચલ KeycodeStore *store, xkb_atom_t name)
{
    જો (name >= darray_size(store->names)) {
        પાછા (KeycodeMatch) { .found = false, .is_alias = false };
    } નહીંતર {
        પાછા darray_item(store->names, name);
    }
}

/***====================================================================***/

પ્રકારવ્યાખ્યા સંરચના {
    ગણના merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

પ્રકારવ્યાખ્યા સંરચના {
    અક્ષર *name;
    પૂર્ણાંક error_ગણતરી;
    unsigned પૂર્ણાંક include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t અંક_led_names;

    સંરચના xkb_context *ctx;
    અચલ સંરચના xkb_keymap_info *keymap_info;
} ચાવી_Names_Info;

/***====================================================================***/

સ્થિર LedNameInfo *
FindLedByName(ચાવી_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    માટે (xkb_led_index_t idx = 0; idx < info->અંક_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        જો (ledi->name == name) {
            *idx_out = idx;
            પાછા ledi;
        }
    }

    પાછા ખાલી;
}

સ્થિર bool
AddLedName(ચાવી_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    અચલ bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    જો (old) {
        જો (old_idx == new_idx) {
            જો (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            પાછા true;
        }

        જો (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        જો (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } નહીંતર {
            પાછા true;
        }
    }

    જો (new_idx >= info->અંક_led_names)
        info->અંક_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    જો (old->name != XKB_ATOM_NONE) {
        જો (report) {
            અચલ xkb_atom_t use    = (replace ? new->name : old->name);
            અચલ xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        જો (replace)
            *old = *new;

        પાછા true;
    }

    *old = *new;
    પાછા true;
}

સ્થિર શૂન્ય
Clear_ચાવી_Names_Info(ચાવી_Names_Info *info)
{
    મુક્ત(info->name);
    keycode_store_free(&info->keycodes);
}

સ્થિર શૂન્ય
Init_ચાવી_Names_Info(ચાવી_Names_Info *info, અચલ સંરચના xkb_keymap_info *keymap_info,
                 unsigned પૂર્ણાંક include_depth)
{
    memset(info, 0, કદ(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

સ્થિર bool
Add_ચાવી_Name(ચાવી_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           ગણના merge_mode merge, bool report)
{
#ifdef NDEBUG
    અચલ
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    જો (match_name.found) {
        અચલ bool clobber = (merge != MERGE_AUGMENT);

        જો (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            જો (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         ચાવી_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            જો (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } નહીંતર {
                પાછા true;
            }
        } નહીંતર {
            અચલ xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            પ્રતિજ્ઞા(old_kc != XKB_KEYCODE_INVALID);
            જો (old_kc != kc) {
                /* There is already a different key with this name. */

                જો (report) {
                    અચલ xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    અચલ xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            ચાવી_Name_Text(info->ctx, name), use, ignore);
                }

                જો (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_ચાવી(&info->keycodes, match_name);
                } નહીંતર {
                    પાછા true;
                }
            }
        }
    }

    અચલ KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    અચલ xkb_atom_t old_name = keycode_store_get_ચાવી_name(&info->keycodes, match_kc);
    જો (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        જો (old_name == name) {
            પ્રતિજ્ઞા(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            જો (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         ચાવી_Name_Text(info->ctx, old_name), kc);
            પાછા true;
        }

        અચલ bool clobber = (merge != MERGE_AUGMENT);
        જો (report) {
            અચલ અક્ષર* અચલ kname     = ચાવી_Name_Text(info->ctx, name);
            અચલ અક્ષર* અચલ old_kname = ચાવી_Name_Text(info->ctx, old_name);
            અચલ અક્ષર* અચલ use    = (clobber) ? kname : old_kname;
            અચલ અક્ષર* અચલ ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        જો (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_ચાવી(&info->keycodes, match_kc, name);
        }
    } નહીંતર {
        /* No previous keycode */
        જો (!keycode_store_ઉમેરો_ચાવી(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            પાછા false;
        }
    }

    પાછા true;
}

/***====================================================================***/

સ્થિર bool
HandleAliasDef(ચાવી_Names_Info *info, અચલ ચાવી_Alias_Def *def, bool report);

સ્થિર શૂન્ય
MergeKeycodeStores(ચાવી_Names_Info *into, ચાવી_Names_Info *from,
                   ગણના merge_mode merge, bool report)
{
    જો (darray_empty(into->keycodes.નીચલું) && darray_empty(into->keycodes.ઉપલું) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.નીચલું);
        darray_init(from->keycodes.ઉપલું);
        darray_init(from->keycodes.names);
    } નહીંતર {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        માટે (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.નીચલું);
             kc++) {
            /* Skip undefined keycodes */
            અચલ xkb_atom_t name = darray_item(from->keycodes.નીચલું, kc);
            જો (name == XKB_ATOM_NONE)
                ચાલુ;

            જો (!Add_ચાવી_Name(into, kc, name, merge, report))
                into->error_ગણતરી++;
        }

        /* High keycodes */
        અચલ ઉપલું_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.ઉપલું) {
            પ્રતિજ્ઞા(new->name != XKB_ATOM_NONE);
            જો (!Add_ચાવી_Name(into, new->keycode, new->name, merge, report))
                into->error_ગણતરી++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            જો (!match->found || !match->is_alias)
                ચાલુ;

            અચલ ચાવી_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            જો (!HandleAliasDef(into, &def, report))
                into->error_ગણતરી++;
        }
    }
}

સ્થિર શૂન્ય
MergeIncludedKeycodes(ચાવી_Names_Info *into, ચાવી_Names_Info *from,
                      ગણના merge_mode merge, bool report)
{
    જો (from->error_ગણતરી > 0) {
        into->error_ગણતરી += from->error_ગણતરી;
        પાછા;
    }

    જો (into->name == ખાલી) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    જો (into->અંક_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               કદ(*from->led_names) * from->અંક_led_names);
        into->અંક_led_names = from->અંક_led_names;
        from->અંક_led_names = 0;
    }
    નહીંતર {
        માટે (xkb_led_index_t idx = 0; idx < from->અંક_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            જો (ledi->name == XKB_ATOM_NONE)
                ચાલુ;

            ledi->merge = merge;
            જો (!AddLedName(into, false, ledi, idx, report))
                into->error_ગણતરી++;
        }
    }
}

સ્થિર શૂન્ય
HandleKeycodesFile(ચાવી_Names_Info *info, XkbFile *file);

સ્થિર bool
HandleIncludeKeycodes(ચાવી_Names_Info *info, IncludeStmt *include, bool report)
{
    ચાવી_Names_Info included;

    જો (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_ગણતરી += 10;
        પાછા false;
    }

    Init_ચાવી_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    અચલ સંરચના parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    માટે (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        ચાવી_Names_Info next_incl;
        XkbFile *file;

        અક્ષર path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, કદ(path));
        જો (!file) {
            info->error_ગણતરી += 10;
            Clear_ચાવી_Names_Info(&included);
            પાછા false;
        }

        Init_ચાવી_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_ચાવી_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_ચાવી_Names_Info(&included);

    પાછા (info->error_ગણતરી == 0);
}

સ્થિર bool
HandleKeycodeDef(ચાવી_Names_Info *info, KeycodeDef *stmt, bool report)
{
    જો (stmt->કિંમત < 0 || stmt->કિંમત > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->કિંમત, XKB_KEYCODE_MAX);
        પાછા false;
    }

    પાછા Add_ચાવી_Name(info, (xkb_keycode_t) stmt->કિંમત,
                      stmt->name, stmt->merge, report);
}

સ્થિર bool
HandleAliasDef(ચાવી_Names_Info *info, અચલ ચાવી_Alias_Def *def, bool report)
{
    અચલ KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    જો (match_name.found) {
        અચલ bool clobber = (def->merge != MERGE_AUGMENT);
        જો (match_name.is_alias) {
            જો (def->real == match_name.alias.real) {
                જો (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             ચાવી_Name_Text(info->ctx, def->alias),
                             ચાવી_Name_Text(info->ctx, def->real));
            } નહીંતર {
                અચલ xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                અચલ xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                જો (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             ચાવી_Name_Text(info->ctx, def->alias),
                             ચાવી_Name_Text(info->ctx, use),
                             ચાવી_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            પાછા true;
        } નહીંતર {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            જો (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ચાવી_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         ચાવી_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            જો (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_ચાવી(&info->keycodes, match_name);
            } નહીંતર {
                પાછા true;
            }
        }
    }

    પાછા keycode_store_ઉમેરો_alias(&info->keycodes, def->alias, def->real);
}

સ્થિર bool
Handle_ચાવી_Name_Var(ચાવી_Names_Info *info, VarDef *stmt)
{
    અચલ અક્ષર *elem, *field;
    પદાવલિ_Def *arrayNdx;

    જો (!પદાવલિ_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        પાછા false;

    જો (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        પાછા !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    જો (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        પાછા !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    જો (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        પાછા !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    જો (!પદાવલિ_Resolve_Integer(info->ctx, stmt->કિંમત, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        પાછા !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    પાછા true;
}

સ્થિર bool
HandleLedNameDef(ચાવી_Names_Info *info, LedNameDef *def, bool report)
{
    જો (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_ગણતરી++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        પાછા false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    જો (!પદાવલિ_Resolve_String(info->ctx, def->name, &name)) {
        અક્ષર buf[20];
        snprintf(buf, કદ(buf), "%"PRId64, def->ndx);
        info->error_ગણતરી++;
        પાછા ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    પાછા AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

સ્થિર શૂન્ય
HandleKeycodesFile(ચાવી_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    અચલ પૂર્ણાંક verbosity = xkb_context_get_log_verbosity(info->ctx);
    અચલ bool report_same_file = verbosity > 0;
    અચલ bool report_include   = verbosity > 7;

    મુક્ત(info->name);
    info->name = strdup_safe(file->name);

    માટે (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        પસંદગી (stmt->type) {
        સ્થિતિ STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            તોડો;
        સ્થિતિ STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            તોડો;
        સ્થિતિ STMT_ALIAS:
            ok = HandleAliasDef(info, (ચાવી_Alias_Def *) stmt, report_same_file);
            તોડો;
        સ્થિતિ STMT_VAR:
            ok = Handle_ચાવી_Name_Var(info, (VarDef *) stmt);
            તોડો;
        સ્થિતિ STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            તોડો;
        સ્થિતિ STMT_UNKNOWN_DECLARATION:
        સ્થિતિ STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            તોડો;
        મૂળભૂત:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            તોડો;
        }

        જો (!ok)
            info->error_ગણતરી++;

        જો (info->error_ગણતરી > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            તોડો;
        }
    }
}

/***====================================================================***/

સ્થિર bool
Copy_ચાવી_Names_To_Keymap(સંરચના xkb_keymap *keymap, ચાવી_Names_Info *info)
{
    જો (darray_empty(info->keycodes.નીચલું) && darray_empty(info->keycodes.ઉપલું)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        પ્રતિજ્ઞા(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_ચાવી_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_ચાવી_code = 255;
        keymap->અંક_ચાવીઓ = keymap->અંક_ચાવીઓ_નીચલું = keymap->max_ચાવી_code + 1;
    } નહીંતર {
        પ્રતિજ્ઞા(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_ચાવી_code = info->keycodes.min;
        keymap->max_ચાવી_code = (darray_empty(info->keycodes.ઉપલું))
            ? darray_size(info->keycodes.નીચલું) - 1
            : darray_item(
                info->keycodes.ઉપલું,
                darray_size(info->keycodes.ઉપલું) - 1
            ).keycode;
        keymap->અંક_ચાવીઓ_નીચલું = darray_size(info->keycodes.નીચલું);
        keymap->અંક_ચાવીઓ = keymap->અંક_ચાવીઓ_નીચલું
                         + darray_size(info->keycodes.ઉપલું);
    }

    સંરચના xkb_ચાવી * અચલ ચાવીઓ = calloc(keymap->અંક_ચાવીઓ, કદ(*ચાવીઓ));
    જો (!ચાવીઓ) {
        keymap->અંક_ચાવીઓ = 0;
        keymap->min_ચાવી_code = keymap->max_ચાવી_code = XKB_KEYCODE_INVALID;
        પાછા false;
    }

    /* Low keycodes */
    માટે (xkb_keycode_t kc = keymap->min_ચાવી_code; kc < keymap->અંક_ચાવીઓ_નીચલું; kc++)
        ચાવીઓ[kc].keycode = kc;

    માટે (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.નીચલું);
         kc++)
        ચાવીઓ[kc].name = darray_item(info->keycodes.નીચલું, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->અંક_ચાવીઓ_નીચલું; /* First high keycode index */
    અચલ ઉપલું_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.ઉપલું) {
        પ્રતિજ્ઞા(entry->name != XKB_ATOM_NONE);
        ચાવીઓ[idx].keycode = entry->keycode;
        ચાવીઓ[idx].name = entry->name;
        idx++;
    }

    keymap->ચાવીઓ = ચાવીઓ;
    પાછા true;
}

સ્થિર bool
CopyKeycodeNameLUT(સંરચના xkb_keymap *keymap, ચાવી_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        જો (!match->found)
            ચાલુ;

        જો (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            અચલ KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            જો (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        ચાવી_Name_Text(info->ctx, name),
                        ચાવી_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                ચાલુ;
            }
            પ્રતિજ્ઞા(!match_real.is_alias);
        } નહીંતર જો (!match->ચાવી.નીચલું) {
            /* Update to final index in keymap::keys */
            match->ચાવી.index += keymap->અંક_ચાવીઓ_નીચલું;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->અંક_ચાવી_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->ચાવી_names, ખાલી);
    darray_init(info->keycodes.names);
    પાછા true;
}

સ્થિર bool
CopyLedNamesToKeymap(સંરચના xkb_keymap *keymap, ચાવી_Names_Info *info)
{
    keymap->અંક_leds = info->અંક_led_names;
    માટે (xkb_led_index_t idx = 0; idx < info->અંક_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        જો (ledi->name == XKB_ATOM_NONE)
            ચાલુ;

        keymap->leds[idx].name = ledi->name;
    }

    પાછા true;
}

સ્થિર bool
Copy_ચાવી_Names_Info_To_Keymap(સંરચના xkb_keymap *keymap, ચાવી_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    જો (!Copy_ચાવી_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        પાછા false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    જો (!keymap->અંક_ચાવીઓ ||
        keymap->min_ચાવી_code > 0) {
        keymap->redirect_ચાવી_auto = 0;
    } નહીંતર {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        માટે (xkb_keycode_t k = keymap->અંક_ચાવીઓ; k-- > keymap->અંક_ચાવીઓ_નીચલું;) {
            જો (keycode > keymap->ચાવીઓ[k].keycode)
                તોડો;
            /* Try next keycode downwards */
            keycode = keymap->ચાવીઓ[k].keycode - 1;
        }
        keymap->redirect_ચાવી_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    પાછા true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, સંરચના xkb_keymap_info *keymap_info)
{
    ચાવી_Names_Info info;

    Init_ચાવી_Names_Info(&info, keymap_info, 0);

    જો (file != ખાલી)
        HandleKeycodesFile(&info, file);

    જો (info.error_ગણતરી != 0)
        જાઓ err_info;

    જો (!Copy_ચાવી_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        જાઓ err_info;

    Clear_ચાવી_Names_Info(&info);
    પાછા true;

err_info:
    Clear_ચાવી_Names_Info(&info);
    પાછા false;
}
