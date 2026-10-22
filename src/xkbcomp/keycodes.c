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
} عالي_Keycode_Entry;

تعريفنوع بنية {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) أدنى;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(عالي_Keycode_Entry) عالي;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

ساكن مضمن فراغ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->أدنى);
    darray_init(store->عالي);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ساكن مضمن فراغ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->أدنى);
    darray_free(store->عالي);
    darray_free(store->names);
}

ساكن مضمن فراغ
keycode_store_update_مفتاح(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    إذا (unlikely(!match.found || match.is_alias)) {
        أرجع;
    } وإلا إذا (match.مفتاح.أدنى) {
        /* assume valid index */
        أكد(match.مفتاح.index < darray_size(store->أدنى));
        darray_item(store->أدنى, match.مفتاح.index) = name;
    } وإلا {
        /* assume valid index */
        أكد(match.مفتاح.index < darray_size(store->عالي));
        darray_item(store->عالي, match.مفتاح.index).name = name;
    }

    /* Names lookup */
    إذا (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

ساكن bool
keycode_store_أدرج_مفتاح(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    إذا (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        إذا (kc >= (xkb_keycode_t) darray_size(store->أدنى))
            darray_resize0(store->أدنى, kc + 1);
        darray_item(store->أدنى, kc) = name;
        إذا (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .مفتاح = {
                .found = true,
                .أدنى = true,
                .is_alias = false,
                .index = kc
            }
        };
    } وإلا {
        /* High keycode: insert into a sorted list */
        ثابت darray_size_t idx = darray_size(store->عالي);
        إذا (idx && darray_item(store->عالي, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            طالما (lower < upper) {
                ثابت darray_size_t mid = lower + (upper - 1 - lower) / 2;
                ثابت عالي_Keycode_Entry * ثابت entry =
                    &darray_item(store->عالي, mid);
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
            أكد(darray_item(store->عالي, lower).keycode > kc);

            /* Update references to entries that will be moved */
            عالي_Keycode_Entry *entry;
            darray_foreach_from(entry, store->عالي, lower) {
                darray_item(store->names, entry->name).مفتاح.index++;
            }

            darray_أدرج(store->عالي, lower,
                          (عالي_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .مفتاح = {
                    .found = true,
                    .أدنى = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } وإلا {
            /* Fast path: no need to sort */
            darray_append(store->عالي,
                          (عالي_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .مفتاح = {
                    .found = true,
                    .أدنى = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        إذا (darray_empty(store->أدنى))
            store->min = darray_item(store->عالي, 0).keycode;
    }
    أرجع true;
}

ساكن مضمن bool
keycode_store_أدرج_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
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
keycode_store_delete_مفتاح(KeycodeStore *store, ثابت KeycodeMatch match)
{
    إذا (unlikely(!match.found || match.is_alias)) {
        أرجع;
    } وإلا إذا (match.مفتاح.أدنى) {
        /* assume valid index */
        أكد(match.مفتاح.index < darray_size(store->أدنى));
        darray_item(store->names,
                    darray_item(store->أدنى, match.مفتاح.index)).found = false;
        إذا (match.مفتاح.index + 1u == darray_size(store->أدنى)) {
            /* Highest low keycode: shrink */
            إذا (store->min == match.مفتاح.index) {
                /* No low keycode left */
                darray_size(store->أدنى) = 0;
            } وإلا {
                /* Look for previous defined low keycode */
                لكل (darray_size_t idx = match.مفتاح.index; idx > 0; idx--) {
                    إذا (darray_item(store->أدنى, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->أدنى) = idx;
                        اكسر;
                    }
                }
            }
        } وإلا {
            /* Lower keycode: reset */
            darray_item(store->أدنى, match.مفتاح.index) = XKB_ATOM_NONE;
        }
    } وإلا {
        أكد(match.مفتاح.index < darray_size(store->عالي)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->عالي, match.مفتاح.index).name).found = false;
        darray_remove(store->عالي, match.مفتاح.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            إذا (entry->found && !entry->is_alias && !entry->مفتاح.أدنى &&
                entry->مفتاح.index > match.مفتاح.index) {
                entry->مفتاح.index--;
            }
        }
    }

    /* Update bounds */
    إذا (darray_empty(store->أدنى)) {
        store->min = (darray_empty(store->عالي))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->عالي, 0).keycode;
    } وإلا {
        لكل (xkb_keycode_t kc = store->min; kc < darray_size(store->أدنى); kc++) {
            إذا (darray_item(store->أدنى, kc) != XKB_ATOM_NONE) {
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
    } وإلا إذا (match.مفتاح.أدنى) {
        /* assume valid index */
        أكد(match.مفتاح.index < darray_size(store->أدنى));
        أرجع (xkb_keycode_t) match.مفتاح.index;
    } وإلا {
        /* assume valid index */
        أكد(match.مفتاح.index < darray_size(store->عالي));
        أرجع darray_item(store->عالي, match.مفتاح.index).keycode;
    }
}

ساكن مضمن xkb_atom_t
keycode_store_get_مفتاح_name(ثابت KeycodeStore *store, KeycodeMatch match)
{
    إذا (!match.found || match.is_alias) {
        أرجع XKB_ATOM_NONE;
    } وإلا إذا (match.مفتاح.أدنى) {
        /* assume valid index */
        أكد(match.مفتاح.index < darray_size(store->أدنى));
        أرجع darray_item(store->أدنى, match.مفتاح.index);
    } وإلا {
        /* assume valid index */
        أكد(match.مفتاح.index < darray_size(store->عالي));
        أرجع darray_item(store->عالي, match.مفتاح.index).name;
    }
}

ساكن KeycodeMatch
keycode_store_lookup_keycode(ثابت KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    إذا (kc < (xkb_keycode_t) darray_size(store->أدنى)) {
        أرجع (KeycodeMatch) {
            .مفتاح = {
                .found = true,
                .أدنى = true,
                .is_alias = false,
                .index = kc
            }
        };
    } وإلا إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        أرجع (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->عالي);
    طالما (lower < upper) {
        ثابت darray_size_t mid = lower + (upper - 1 - lower) / 2;
        عالي_Keycode_Entry * ثابت entry = &darray_item(store->عالي, mid);
        إذا (entry->keycode < kc) {
            lower = mid + 1;
        } وإلا إذا (entry->keycode > kc) {
            upper = mid;
        } وإلا {
            أرجع (KeycodeMatch) {
                .مفتاح = {
                    .found = true,
                    .أدنى = false,
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
    صحيح error_عدد;
    unsigned صحيح include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t رقم_led_names;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} مفتاح_Names_Info;

/***====================================================================***/

ساكن LedNameInfo *
FindLedByName(مفتاح_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    لكل (xkb_led_index_t idx = 0; idx < info->رقم_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        إذا (ledi->name == name) {
            *idx_out = idx;
            أرجع ledi;
        }
    }

    أرجع عدم;
}

ساكن bool
AddLedName(مفتاح_Names_Info *info, bool same_file,
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

    إذا (new_idx >= info->رقم_led_names)
        info->رقم_led_names = new_idx + 1;

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
Clear_مفتاح_Names_Info(مفتاح_Names_Info *info)
{
    حرر(info->name);
    keycode_store_free(&info->keycodes);
}

ساكن فراغ
Init_مفتاح_Names_Info(مفتاح_Names_Info *info, ثابت بنية xkb_keymap_info *keymap_info,
                 unsigned صحيح include_depth)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

ساكن bool
Add_مفتاح_Name(مفتاح_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_مفتاح_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         مفتاح_Name_Text(info->ctx, name),
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
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_مفتاح_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            مفتاح_Name_Text(info->ctx, name), use, ignore);
                }

                إذا (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_مفتاح(&info->keycodes, match_name);
                } وإلا {
                    أرجع true;
                }
            }
        }
    }

    ثابت KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ثابت xkb_atom_t old_name = keycode_store_get_مفتاح_name(&info->keycodes, match_kc);
    إذا (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        إذا (old_name == name) {
            أكد(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            إذا (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         مفتاح_Name_Text(info->ctx, old_name), kc);
            أرجع true;
        }

        ثابت bool clobber = (merge != MERGE_AUGMENT);
        إذا (report) {
            ثابت حرف* ثابت kname     = مفتاح_Name_Text(info->ctx, name);
            ثابت حرف* ثابت old_kname = مفتاح_Name_Text(info->ctx, old_name);
            ثابت حرف* ثابت use    = (clobber) ? kname : old_kname;
            ثابت حرف* ثابت ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        إذا (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_مفتاح(&info->keycodes, match_kc, name);
        }
    } وإلا {
        /* No previous keycode */
        إذا (!keycode_store_أدرج_مفتاح(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            أرجع false;
        }
    }

    أرجع true;
}

/***====================================================================***/

ساكن bool
HandleAliasDef(مفتاح_Names_Info *info, ثابت مفتاح_Alias_Def *def, bool report);

ساكن فراغ
MergeKeycodeStores(مفتاح_Names_Info *into, مفتاح_Names_Info *from,
                   تعداد merge_mode merge, bool report)
{
    إذا (darray_empty(into->keycodes.أدنى) && darray_empty(into->keycodes.عالي) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.أدنى);
        darray_init(from->keycodes.عالي);
        darray_init(from->keycodes.names);
    } وإلا {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        لكل (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.أدنى);
             kc++) {
            /* Skip undefined keycodes */
            ثابت xkb_atom_t name = darray_item(from->keycodes.أدنى, kc);
            إذا (name == XKB_ATOM_NONE)
                استمر;

            إذا (!Add_مفتاح_Name(into, kc, name, merge, report))
                into->error_عدد++;
        }

        /* High keycodes */
        ثابت عالي_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.عالي) {
            أكد(new->name != XKB_ATOM_NONE);
            إذا (!Add_مفتاح_Name(into, new->keycode, new->name, merge, report))
                into->error_عدد++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            إذا (!match->found || !match->is_alias)
                استمر;

            ثابت مفتاح_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            إذا (!HandleAliasDef(into, &def, report))
                into->error_عدد++;
        }
    }
}

ساكن فراغ
MergeIncludedKeycodes(مفتاح_Names_Info *into, مفتاح_Names_Info *from,
                      تعداد merge_mode merge, bool report)
{
    إذا (from->error_عدد > 0) {
        into->error_عدد += from->error_عدد;
        أرجع;
    }

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    إذا (into->رقم_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               حجم(*from->led_names) * from->رقم_led_names);
        into->رقم_led_names = from->رقم_led_names;
        from->رقم_led_names = 0;
    }
    وإلا {
        لكل (xkb_led_index_t idx = 0; idx < from->رقم_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            إذا (ledi->name == XKB_ATOM_NONE)
                استمر;

            ledi->merge = merge;
            إذا (!AddLedName(into, false, ledi, idx, report))
                into->error_عدد++;
        }
    }
}

ساكن فراغ
HandleKeycodesFile(مفتاح_Names_Info *info, XkbFile *file);

ساكن bool
HandleIncludeKeycodes(مفتاح_Names_Info *info, IncludeStmt *include, bool report)
{
    مفتاح_Names_Info included;

    إذا (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_عدد += 10;
        أرجع false;
    }

    Init_مفتاح_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        مفتاح_Names_Info next_incl;
        XkbFile *file;

        حرف path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, حجم(path));
        إذا (!file) {
            info->error_عدد += 10;
            Clear_مفتاح_Names_Info(&included);
            أرجع false;
        }

        Init_مفتاح_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_مفتاح_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_مفتاح_Names_Info(&included);

    أرجع (info->error_عدد == 0);
}

ساكن bool
HandleKeycodeDef(مفتاح_Names_Info *info, KeycodeDef *stmt, bool report)
{
    إذا (stmt->قيمة < 0 || stmt->قيمة > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->قيمة, XKB_KEYCODE_MAX);
        أرجع false;
    }

    أرجع Add_مفتاح_Name(info, (xkb_keycode_t) stmt->قيمة,
                      stmt->name, stmt->merge, report);
}

ساكن bool
HandleAliasDef(مفتاح_Names_Info *info, ثابت مفتاح_Alias_Def *def, bool report)
{
    ثابت KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    إذا (match_name.found) {
        ثابت bool clobber = (def->merge != MERGE_AUGMENT);
        إذا (match_name.is_alias) {
            إذا (def->real == match_name.alias.real) {
                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_مفتاح_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             مفتاح_Name_Text(info->ctx, def->alias),
                             مفتاح_Name_Text(info->ctx, def->real));
            } وإلا {
                ثابت xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                ثابت xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_مفتاح_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             مفتاح_Name_Text(info->ctx, def->alias),
                             مفتاح_Name_Text(info->ctx, use),
                             مفتاح_Name_Text(info->ctx, ignore));

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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_مفتاح_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         مفتاح_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            إذا (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_مفتاح(&info->keycodes, match_name);
            } وإلا {
                أرجع true;
            }
        }
    }

    أرجع keycode_store_أدرج_alias(&info->keycodes, def->alias, def->real);
}

ساكن bool
Handle_مفتاح_Name_Var(مفتاح_Names_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *field;
    تعبير_Def *arrayNdx;

    إذا (!تعبير_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
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
    إذا (!تعبير_Resolve_Integer(info->ctx, stmt->قيمة, &val) ||
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
HandleLedNameDef(مفتاح_Names_Info *info, LedNameDef *def, bool report)
{
    إذا (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_عدد++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!تعبير_Resolve_String(info->ctx, def->name, &name)) {
        حرف buf[20];
        snprintf(buf, حجم(buf), "%"PRId64, def->ndx);
        info->error_عدد++;
        أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    أرجع AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

ساكن فراغ
HandleKeycodesFile(مفتاح_Names_Info *info, XkbFile *file)
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
            ok = HandleAliasDef(info, (مفتاح_Alias_Def *) stmt, report_same_file);
            اكسر;
        حالة STMT_VAR:
            ok = Handle_مفتاح_Name_Var(info, (VarDef *) stmt);
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
            info->error_عدد++;

        إذا (info->error_عدد > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
Copy_مفتاح_Names_To_Keymap(بنية xkb_keymap *keymap, مفتاح_Names_Info *info)
{
    إذا (darray_empty(info->keycodes.أدنى) && darray_empty(info->keycodes.عالي)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        أكد(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_مفتاح_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_مفتاح_code = 255;
        keymap->رقم_مفاتيح = keymap->رقم_مفاتيح_أدنى = keymap->max_مفتاح_code + 1;
    } وإلا {
        أكد(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_مفتاح_code = info->keycodes.min;
        keymap->max_مفتاح_code = (darray_empty(info->keycodes.عالي))
            ? darray_size(info->keycodes.أدنى) - 1
            : darray_item(
                info->keycodes.عالي,
                darray_size(info->keycodes.عالي) - 1
            ).keycode;
        keymap->رقم_مفاتيح_أدنى = darray_size(info->keycodes.أدنى);
        keymap->رقم_مفاتيح = keymap->رقم_مفاتيح_أدنى
                         + darray_size(info->keycodes.عالي);
    }

    بنية xkb_مفتاح * ثابت مفاتيح = calloc(keymap->رقم_مفاتيح, حجم(*مفاتيح));
    إذا (!مفاتيح) {
        keymap->رقم_مفاتيح = 0;
        keymap->min_مفتاح_code = keymap->max_مفتاح_code = XKB_KEYCODE_INVALID;
        أرجع false;
    }

    /* Low keycodes */
    لكل (xkb_keycode_t kc = keymap->min_مفتاح_code; kc < keymap->رقم_مفاتيح_أدنى; kc++)
        مفاتيح[kc].keycode = kc;

    لكل (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.أدنى);
         kc++)
        مفاتيح[kc].name = darray_item(info->keycodes.أدنى, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->رقم_مفاتيح_أدنى; /* First high keycode index */
    ثابت عالي_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.عالي) {
        أكد(entry->name != XKB_ATOM_NONE);
        مفاتيح[idx].keycode = entry->keycode;
        مفاتيح[idx].name = entry->name;
        idx++;
    }

    keymap->مفاتيح = مفاتيح;
    أرجع true;
}

ساكن bool
CopyKeycodeNameLUT(بنية xkb_keymap *keymap, مفتاح_Names_Info *info)
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
                        مفتاح_Name_Text(info->ctx, name),
                        مفتاح_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                استمر;
            }
            أكد(!match_real.is_alias);
        } وإلا إذا (!match->مفتاح.أدنى) {
            /* Update to final index in keymap::keys */
            match->مفتاح.index += keymap->رقم_مفاتيح_أدنى;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->رقم_مفتاح_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->مفتاح_names, عدم);
    darray_init(info->keycodes.names);
    أرجع true;
}

ساكن bool
CopyLedNamesToKeymap(بنية xkb_keymap *keymap, مفتاح_Names_Info *info)
{
    keymap->رقم_leds = info->رقم_led_names;
    لكل (xkb_led_index_t idx = 0; idx < info->رقم_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        إذا (ledi->name == XKB_ATOM_NONE)
            استمر;

        keymap->leds[idx].name = ledi->name;
    }

    أرجع true;
}

ساكن bool
Copy_مفتاح_Names_Info_To_Keymap(بنية xkb_keymap *keymap, مفتاح_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    إذا (!Copy_مفتاح_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        أرجع false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    إذا (!keymap->رقم_مفاتيح ||
        keymap->min_مفتاح_code > 0) {
        keymap->redirect_مفتاح_auto = 0;
    } وإلا {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        لكل (xkb_keycode_t k = keymap->رقم_مفاتيح; k-- > keymap->رقم_مفاتيح_أدنى;) {
            إذا (keycode > keymap->مفاتيح[k].keycode)
                اكسر;
            /* Try next keycode downwards */
            keycode = keymap->مفاتيح[k].keycode - 1;
        }
        keymap->redirect_مفتاح_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    أرجع true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    مفتاح_Names_Info info;

    Init_مفتاح_Names_Info(&info, keymap_info, 0);

    إذا (file != عدم)
        HandleKeycodesFile(&info, file);

    إذا (info.error_عدد != 0)
        اذهب err_info;

    إذا (!Copy_مفتاح_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    Clear_مفتاح_Names_Info(&info);
    أرجع true;

err_info:
    Clear_مفتاح_Names_Info(&info);
    أرجع false;
}
