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
} سم_Keycode_Entry;

تعريفنوع بنية {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) كس;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(سم_Keycode_Entry) سم;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

ساكن مضمن فراغ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->كس);
    darray_init(store->سم);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ساكن مضمن فراغ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->كس);
    darray_free(store->سم);
    darray_free(store->names);
}

ساكن مضمن فراغ
keycode_store_update_مبودي(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    إذا (unlikely(!match.found || match.is_alias)) {
        أرجع;
    } وإلا إذا (match.مبودي.كس) {
        /* assume valid index */
        أكد(match.مبودي.index < darray_size(store->كس));
        darray_item(store->كس, match.مبودي.index) = name;
    } وإلا {
        /* assume valid index */
        أكد(match.مبودي.index < darray_size(store->سم));
        darray_item(store->سم, match.مبودي.index).name = name;
    }

    /* Names lookup */
    إذا (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

ساكن bool
keycode_store_سك_مبودي(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    إذا (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        إذا (kc >= (xkb_keycode_t) darray_size(store->كس))
            darray_resize0(store->كس, kc + 1);
        darray_item(store->كس, kc) = name;
        إذا (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .مبودي = {
                .found = true,
                .كس = true,
                .is_alias = false,
                .index = kc
            }
        };
    } وإلا {
        /* High keycode: insert into a sorted list */
        ثابت darray_size_t idx = darray_size(store->سم);
        إذا (idx && darray_item(store->سم, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            طالما (lower < upper) {
                ثابت darray_size_t mid = lower + (upper - 1 - lower) / 2;
                ثابت سم_Keycode_Entry * ثابت entry =
                    &darray_item(store->سم, mid);
                إذا (entry->keycode < kc) {
                    lower = mid + 1;
                } وإلا إذا (entry->keycode > kc) {
                    upper = mid;
                } وإلا {
                    /* Unreachable: there is no repetition in the list */
                    أكد(entry->keycode != kc);
                }
            }
            أكد(lower < idx);
            أكد(darray_item(store->سم, lower).keycode > kc);

            /* Update references to entries that will be moved */
            سم_Keycode_Entry *entry;
            darray_foreach_from(entry, store->سم, lower) {
                darray_item(store->names, entry->name).مبودي.index++;
            }

            darray_سك(store->سم, lower,
                          (سم_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .مبودي = {
                    .found = true,
                    .كس = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } وإلا {
            /* Fast path: no need to sort */
            darray_append(store->سم,
                          (سم_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .مبودي = {
                    .found = true,
                    .كس = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        إذا (darray_empty(store->كس))
            store->min = darray_item(store->سم, 0).keycode;
    }
    أرجع true;
}

ساكن مضمن bool
keycode_store_سك_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    إذا (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    أرجع true;
}

ساكن مضمن bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    أرجع true;
}

ساكن مضمن فراغ
keycode_store_delete_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    إذا (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

ساكن فراغ
keycode_store_delete_مبودي(KeycodeStore *store, ثابت KeycodeMatch match)
{
    إذا (unlikely(!match.found || match.is_alias)) {
        أرجع;
    } وإلا إذا (match.مبودي.كس) {
        /* assume valid index */
        أكد(match.مبودي.index < darray_size(store->كس));
        darray_item(store->names,
                    darray_item(store->كس, match.مبودي.index)).found = false;
        إذا (match.مبودي.index + 1u == darray_size(store->كس)) {
            /* Highest low keycode: shrink */
            إذا (store->min == match.مبودي.index) {
                /* No low keycode left */
                darray_size(store->كس) = 0;
            } وإلا {
                /* Look for previous defined low keycode */
                لكل (darray_size_t idx = match.مبودي.index; idx > 0; idx--) {
                    إذا (darray_item(store->كس, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->كس) = idx;
                        اكسر;
                    }
                }
            }
        } وإلا {
            /* Lower keycode: reset */
            darray_item(store->كس, match.مبودي.index) = XKB_ATOM_NONE;
        }
    } وإلا {
        أكد(match.مبودي.index < darray_size(store->سم)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->سم, match.مبودي.index).name).found = false;
        darray_remove(store->سم, match.مبودي.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            إذا (entry->found && !entry->is_alias && !entry->مبودي.كس &&
                entry->مبودي.index > match.مبودي.index) {
                entry->مبودي.index--;
            }
        }
    }

    /* Update bounds */
    إذا (darray_empty(store->كس)) {
        store->min = (darray_empty(store->سم))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->سم, 0).keycode;
    } وإلا {
        لكل (xkb_keycode_t kc = store->min; kc < darray_size(store->كس); kc++) {
            إذا (darray_item(store->كس, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                اكسر;
            }
        }
    }
}

ساكن مضمن xkb_keycode_t
keycode_store_get_keycode(ثابت KeycodeStore *store, KeycodeMatch match)
{
    إذا (!match.found || match.is_alias) {
        أرجع XKB_KEYCODE_INVALID;
    } وإلا إذا (match.مبودي.كس) {
        /* assume valid index */
        أكد(match.مبودي.index < darray_size(store->كس));
        أرجع (xkb_keycode_t) match.مبودي.index;
    } وإلا {
        /* assume valid index */
        أكد(match.مبودي.index < darray_size(store->سم));
        أرجع darray_item(store->سم, match.مبودي.index).keycode;
    }
}

ساكن مضمن xkb_atom_t
keycode_store_get_مبودي_name(ثابت KeycodeStore *store, KeycodeMatch match)
{
    إذا (!match.found || match.is_alias) {
        أرجع XKB_ATOM_NONE;
    } وإلا إذا (match.مبودي.كس) {
        /* assume valid index */
        أكد(match.مبودي.index < darray_size(store->كس));
        أرجع darray_item(store->كس, match.مبودي.index);
    } وإلا {
        /* assume valid index */
        أكد(match.مبودي.index < darray_size(store->سم));
        أرجع darray_item(store->سم, match.مبودي.index).name;
    }
}

ساكن KeycodeMatch
keycode_store_lookup_keycode(ثابت KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    إذا (kc < (xkb_keycode_t) darray_size(store->كس)) {
        أرجع (KeycodeMatch) {
            .مبودي = {
                .found = true,
                .كس = true,
                .is_alias = false,
                .index = kc
            }
        };
    } وإلا إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        أرجع (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->سم);
    طالما (lower < upper) {
        ثابت darray_size_t mid = lower + (upper - 1 - lower) / 2;
        سم_Keycode_Entry * ثابت entry = &darray_item(store->سم, mid);
        إذا (entry->keycode < kc) {
            lower = mid + 1;
        } وإلا إذا (entry->keycode > kc) {
            upper = mid;
        } وإلا {
            أرجع (KeycodeMatch) {
                .مبودي = {
                    .found = true,
                    .كس = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    أرجع (KeycodeMatch) { .found = false };
}

ساكن KeycodeMatch
keycode_store_lookup_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    إذا (name >= darray_size(store->names)) {
        أرجع (KeycodeMatch) { .found = false, .is_alias = false };
    } وإلا {
        أرجع darray_item(store->names, name);
    }
}

/***====================================================================***/

تعريفنوع بنية {
    تعداد merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

تعريفنوع بنية {
    حرف *name;
    صحيح error_كدي;
    unsigned صحيح include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t لمب_led_names;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} مبودي_Names_Info;

/***====================================================================***/

ساكن LedNameInfo *
FindLedByName(مبودي_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    لكل (xkb_led_index_t idx = 0; idx < info->لمب_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        إذا (ledi->name == name) {
            *idx_out = idx;
            أرجع ledi;
        }
    }

    أرجع عدم;
}

ساكن bool
AddLedName(مبودي_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    ثابت bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    إذا (old) {
        إذا (old_idx == new_idx) {
            إذا (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            أرجع true;
        }

        إذا (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        إذا (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } وإلا {
            أرجع true;
        }
    }

    إذا (new_idx >= info->لمب_led_names)
        info->لمب_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    إذا (old->name != XKB_ATOM_NONE) {
        إذا (report) {
            ثابت xkb_atom_t use    = (replace ? new->name : old->name);
            ثابت xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        إذا (replace)
            *old = *new;

        أرجع true;
    }

    *old = *new;
    أرجع true;
}

ساكن فراغ
Clear_مبودي_Names_Info(مبودي_Names_Info *info)
{
    حرر(info->name);
    keycode_store_free(&info->keycodes);
}

ساكن فراغ
Init_مبودي_Names_Info(مبودي_Names_Info *info, ثابت بنية xkb_keymap_info *keymap_info,
                 unsigned صحيح include_depth)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

ساكن bool
Add_مبودي_Name(مبودي_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           تعداد merge_mode merge, bool report)
{
#ifdef NDEBUG
    ثابت
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    إذا (match_name.found) {
        ثابت bool clobber = (merge != MERGE_AUGMENT);

        إذا (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            إذا (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_مبودي_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         مبودي_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            إذا (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } وإلا {
                أرجع true;
            }
        } وإلا {
            ثابت xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            أكد(old_kc != XKB_KEYCODE_INVALID);
            إذا (old_kc != kc) {
                /* There is already a different key with this name. */

                إذا (report) {
                    ثابت xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ثابت xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_مبودي_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            مبودي_Name_Text(info->ctx, name), use, ignore);
                }

                إذا (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_مبودي(&info->keycodes, match_name);
                } وإلا {
                    أرجع true;
                }
            }
        }
    }

    ثابت KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ثابت xkb_atom_t old_name = keycode_store_get_مبودي_name(&info->keycodes, match_kc);
    إذا (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        إذا (old_name == name) {
            أكد(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            إذا (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         مبودي_Name_Text(info->ctx, old_name), kc);
            أرجع true;
        }

        ثابت bool clobber = (merge != MERGE_AUGMENT);
        إذا (report) {
            ثابت حرف* ثابت kname     = مبودي_Name_Text(info->ctx, name);
            ثابت حرف* ثابت old_kname = مبودي_Name_Text(info->ctx, old_name);
            ثابت حرف* ثابت use    = (clobber) ? kname : old_kname;
            ثابت حرف* ثابت ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        إذا (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_مبودي(&info->keycodes, match_kc, name);
        }
    } وإلا {
        /* No previous keycode */
        إذا (!keycode_store_سك_مبودي(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            أرجع false;
        }
    }

    أرجع true;
}

/***====================================================================***/

ساكن bool
HandleAliasDef(مبودي_Names_Info *info, ثابت مبودي_Alias_Def *def, bool report);

ساكن فراغ
MergeKeycodeStores(مبودي_Names_Info *into, مبودي_Names_Info *from,
                   تعداد merge_mode merge, bool report)
{
    إذا (darray_empty(into->keycodes.كس) && darray_empty(into->keycodes.سم) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.كس);
        darray_init(from->keycodes.سم);
        darray_init(from->keycodes.names);
    } وإلا {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        لكل (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.كس);
             kc++) {
            /* Skip undefined keycodes */
            ثابت xkb_atom_t name = darray_item(from->keycodes.كس, kc);
            إذا (name == XKB_ATOM_NONE)
                استمر;

            إذا (!Add_مبودي_Name(into, kc, name, merge, report))
                into->error_كدي++;
        }

        /* High keycodes */
        ثابت سم_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.سم) {
            أكد(new->name != XKB_ATOM_NONE);
            إذا (!Add_مبودي_Name(into, new->keycode, new->name, merge, report))
                into->error_كدي++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            إذا (!match->found || !match->is_alias)
                استمر;

            ثابت مبودي_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            إذا (!HandleAliasDef(into, &def, report))
                into->error_كدي++;
        }
    }
}

ساكن فراغ
MergeIncludedKeycodes(مبودي_Names_Info *into, مبودي_Names_Info *from,
                      تعداد merge_mode merge, bool report)
{
    إذا (from->error_كدي > 0) {
        into->error_كدي += from->error_كدي;
        أرجع;
    }

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    إذا (into->لمب_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               حجم(*from->led_names) * from->لمب_led_names);
        into->لمب_led_names = from->لمب_led_names;
        from->لمب_led_names = 0;
    }
    وإلا {
        لكل (xkb_led_index_t idx = 0; idx < from->لمب_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            إذا (ledi->name == XKB_ATOM_NONE)
                استمر;

            ledi->merge = merge;
            إذا (!AddLedName(into, false, ledi, idx, report))
                into->error_كدي++;
        }
    }
}

ساكن فراغ
HandleKeycodesFile(مبودي_Names_Info *info, XkbFile *file);

ساكن bool
HandleIncludeKeycodes(مبودي_Names_Info *info, IncludeStmt *include, bool report)
{
    مبودي_Names_Info included;

    إذا (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_كدي += 10;
        أرجع false;
    }

    Init_مبودي_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        مبودي_Names_Info next_incl;
        XkbFile *file;

        حرف path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, حجم(path));
        إذا (!file) {
            info->error_كدي += 10;
            Clear_مبودي_Names_Info(&included);
            أرجع false;
        }

        Init_مبودي_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_مبودي_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_مبودي_Names_Info(&included);

    أرجع (info->error_كدي == 0);
}

ساكن bool
HandleKeycodeDef(مبودي_Names_Info *info, KeycodeDef *stmt, bool report)
{
    إذا (stmt->درج < 0 || stmt->درج > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->درج, XKB_KEYCODE_MAX);
        أرجع false;
    }

    أرجع Add_مبودي_Name(info, (xkb_keycode_t) stmt->درج,
                      stmt->name, stmt->merge, report);
}

ساكن bool
HandleAliasDef(مبودي_Names_Info *info, ثابت مبودي_Alias_Def *def, bool report)
{
    ثابت KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    إذا (match_name.found) {
        ثابت bool clobber = (def->merge != MERGE_AUGMENT);
        إذا (match_name.is_alias) {
            إذا (def->real == match_name.alias.real) {
                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_مبودي_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             مبودي_Name_Text(info->ctx, def->alias),
                             مبودي_Name_Text(info->ctx, def->real));
            } وإلا {
                ثابت xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                ثابت xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_مبودي_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             مبودي_Name_Text(info->ctx, def->alias),
                             مبودي_Name_Text(info->ctx, use),
                             مبودي_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            أرجع true;
        } وإلا {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            إذا (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_مبودي_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         مبودي_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            إذا (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_مبودي(&info->keycodes, match_name);
            } وإلا {
                أرجع true;
            }
        }
    }

    أرجع keycode_store_سك_alias(&info->keycodes, def->alias, def->real);
}

ساكن bool
Handle_مبودي_Name_Var(مبودي_Names_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *field;
    فرچ_Def *arrayNdx;

    إذا (!فرچ_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        أرجع false;

    إذا (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    إذا (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    إذا (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        أرجع !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    إذا (!فرچ_Resolve_Integer(info->ctx, stmt->درج, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        أرجع !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    أرجع true;
}

ساكن bool
HandleLedNameDef(مبودي_Names_Info *info, LedNameDef *def, bool report)
{
    إذا (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_كدي++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!فرچ_Resolve_String(info->ctx, def->name, &name)) {
        حرف buf[20];
        snprintf(buf, حجم(buf), "%"PRId64, def->ndx);
        info->error_كدي++;
        أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    أرجع AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

ساكن فراغ
HandleKeycodesFile(مبودي_Names_Info *info, XkbFile *file)
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
            ok = HandleAliasDef(info, (مبودي_Alias_Def *) stmt, report_same_file);
            اكسر;
        حالة STMT_VAR:
            ok = Handle_مبودي_Name_Var(info, (VarDef *) stmt);
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

        إذا (!ok)
            info->error_كدي++;

        إذا (info->error_كدي > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
Copy_مبودي_Names_To_Keymap(بنية xkb_keymap *keymap, مبودي_Names_Info *info)
{
    إذا (darray_empty(info->keycodes.كس) && darray_empty(info->keycodes.سم)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        أكد(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_مبودي_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_مبودي_code = 255;
        keymap->لمب_مبوداي = keymap->لمب_مبوداي_كس = keymap->max_مبودي_code + 1;
    } وإلا {
        أكد(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_مبودي_code = info->keycodes.min;
        keymap->max_مبودي_code = (darray_empty(info->keycodes.سم))
            ? darray_size(info->keycodes.كس) - 1
            : darray_item(
                info->keycodes.سم,
                darray_size(info->keycodes.سم) - 1
            ).keycode;
        keymap->لمب_مبوداي_كس = darray_size(info->keycodes.كس);
        keymap->لمب_مبوداي = keymap->لمب_مبوداي_كس
                         + darray_size(info->keycodes.سم);
    }

    بنية xkb_مبودي * ثابت مبوداي = calloc(keymap->لمب_مبوداي, حجم(*مبوداي));
    إذا (!مبوداي) {
        keymap->لمب_مبوداي = 0;
        keymap->min_مبودي_code = keymap->max_مبودي_code = XKB_KEYCODE_INVALID;
        أرجع false;
    }

    /* Low keycodes */
    لكل (xkb_keycode_t kc = keymap->min_مبودي_code; kc < keymap->لمب_مبوداي_كس; kc++)
        مبوداي[kc].keycode = kc;

    لكل (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.كس);
         kc++)
        مبوداي[kc].name = darray_item(info->keycodes.كس, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->لمب_مبوداي_كس; /* First high keycode index */
    ثابت سم_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.سم) {
        أكد(entry->name != XKB_ATOM_NONE);
        مبوداي[idx].keycode = entry->keycode;
        مبوداي[idx].name = entry->name;
        idx++;
    }

    keymap->مبوداي = مبوداي;
    أرجع true;
}

ساكن bool
CopyKeycodeNameLUT(بنية xkb_keymap *keymap, مبودي_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        إذا (!match->found)
            استمر;

        إذا (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ثابت KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            إذا (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        مبودي_Name_Text(info->ctx, name),
                        مبودي_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                استمر;
            }
            أكد(!match_real.is_alias);
        } وإلا إذا (!match->مبودي.كس) {
            /* Update to final index in keymap::keys */
            match->مبودي.index += keymap->لمب_مبوداي_كس;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->لمب_مبودي_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->مبودي_names, عدم);
    darray_init(info->keycodes.names);
    أرجع true;
}

ساكن bool
CopyLedNamesToKeymap(بنية xkb_keymap *keymap, مبودي_Names_Info *info)
{
    keymap->لمب_leds = info->لمب_led_names;
    لكل (xkb_led_index_t idx = 0; idx < info->لمب_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        إذا (ledi->name == XKB_ATOM_NONE)
            استمر;

        keymap->leds[idx].name = ledi->name;
    }

    أرجع true;
}

ساكن bool
Copy_مبودي_Names_Info_To_Keymap(بنية xkb_keymap *keymap, مبودي_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    إذا (!Copy_مبودي_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        أرجع false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    إذا (!keymap->لمب_مبوداي ||
        keymap->min_مبودي_code > 0) {
        keymap->redirect_مبودي_auto = 0;
    } وإلا {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        لكل (xkb_keycode_t k = keymap->لمب_مبوداي; k-- > keymap->لمب_مبوداي_كس;) {
            إذا (keycode > keymap->مبوداي[k].keycode)
                اكسر;
            /* Try next keycode downwards */
            keycode = keymap->مبوداي[k].keycode - 1;
        }
        keymap->redirect_مبودي_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    أرجع true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    مبودي_Names_Info info;

    Init_مبودي_Names_Info(&info, keymap_info, 0);

    إذا (file != عدم)
        HandleKeycodesFile(&info, file);

    إذا (info.error_كدي != 0)
        اذهب err_info;

    إذا (!Copy_مبودي_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    Clear_مبودي_Names_Info(&info);
    أرجع true;

err_info:
    Clear_مبودي_Names_Info(&info);
    أرجع false;
}
