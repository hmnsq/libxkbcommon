/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#include "config.h"

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "context.h"
#include "messages-codes.h"
#include "xkbcommon/xkbcommon.h"
#include "xkbcomp-priv.h"
#include "atom.h"
#include "darray.h"
#include "keymap.h"
#include "text.h"
#include "expr.h"
#include "include.h"
#include "util-mem.h"

typedef struct {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} माताअस_Keycode_Entry;

typedef struct {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) माबाबा;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(माताअस_Keycode_Entry) माताअस;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

static inline void
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->माबाबा);
    darray_init(store->माताअस);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

static inline void
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->माबाबा);
    darray_free(store->माताअस);
    darray_free(store->names);
}

static inline void
keycode_store_update_सुसि(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    if (unlikely(!match.found || match.is_alias)) {
        return;
    } else if (match.सुसि.माबाबा) {
        /* assume valid index */
        assert(match.सुसि.index < darray_size(store->माबाबा));
        darray_item(store->माबाबा, match.सुसि.index) = name;
    } else {
        /* assume valid index */
        assert(match.सुसि.index < darray_size(store->माताअस));
        darray_item(store->माताअस, match.सुसि.index).name = name;
    }

    /* Names lookup */
    if (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

static bool
keycode_store_इसिंगित_सुसि(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    if (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    if (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        if (kc >= (xkb_keycode_t) darray_size(store->माबाबा))
            darray_resize0(store->माबाबा, kc + 1);
        darray_item(store->माबाबा, kc) = name;
        if (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .सुसि = {
                .found = true,
                .माबाबा = true,
                .is_alias = false,
                .index = kc
            }
        };
    } else {
        /* High keycode: insert into a sorted list */
        const darray_size_t idx = darray_size(store->माताअस);
        if (idx && darray_item(store->माताअस, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            while (lower < upper) {
                const darray_size_t mid = lower + (upper - 1 - lower) / 2;
                const माताअस_Keycode_Entry * const entry =
                    &darray_item(store->माताअस, mid);
                if (entry->keycode < kc) {
                    lower = mid + 1;
                } else if (entry->keycode > kc) {
                    upper = mid;
                } else {
                    /* Unreachable: there is no repetition in the list */
                    assert(entry->keycode != kc);
                }
            }
            assert(lower < idx);
            assert(darray_item(store->माताअस, lower).keycode > kc);

            /* Update references to entries that will be moved */
            माताअस_Keycode_Entry *entry;
            darray_foreach_from(entry, store->माताअस, lower) {
                darray_item(store->names, entry->name).सुसि.index++;
            }

            darray_इसिंगित(store->माताअस, lower,
                          (माताअस_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .सुसि = {
                    .found = true,
                    .माबाबा = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } else {
            /* Fast path: no need to sort */
            darray_append(store->माताअस,
                          (माताअस_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .सुसि = {
                    .found = true,
                    .माबाबा = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        if (darray_empty(store->माबाबा))
            store->min = darray_item(store->माताअस, 0).keycode;
    }
    return true;
}

static inline bool
keycode_store_इसिंगित_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    if (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    return true;
}

static inline bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    return true;
}

static inline void
keycode_store_delete_name(const KeycodeStore *store, xkb_atom_t name)
{
    if (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

static void
keycode_store_delete_सुसि(KeycodeStore *store, const KeycodeMatch match)
{
    if (unlikely(!match.found || match.is_alias)) {
        return;
    } else if (match.सुसि.माबाबा) {
        /* assume valid index */
        assert(match.सुसि.index < darray_size(store->माबाबा));
        darray_item(store->names,
                    darray_item(store->माबाबा, match.सुसि.index)).found = false;
        if (match.सुसि.index + 1u == darray_size(store->माबाबा)) {
            /* Highest low keycode: shrink */
            if (store->min == match.सुसि.index) {
                /* No low keycode left */
                darray_size(store->माबाबा) = 0;
            } else {
                /* Look for previous defined low keycode */
                for (darray_size_t idx = match.सुसि.index; idx > 0; idx--) {
                    if (darray_item(store->माबाबा, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->माबाबा) = idx;
                        break;
                    }
                }
            }
        } else {
            /* Lower keycode: reset */
            darray_item(store->माबाबा, match.सुसि.index) = XKB_ATOM_NONE;
        }
    } else {
        assert(match.सुसि.index < darray_size(store->माताअस)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->माताअस, match.सुसि.index).name).found = false;
        darray_remove(store->माताअस, match.सुसि.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            if (entry->found && !entry->is_alias && !entry->सुसि.माबाबा &&
                entry->सुसि.index > match.सुसि.index) {
                entry->सुसि.index--;
            }
        }
    }

    /* Update bounds */
    if (darray_empty(store->माबाबा)) {
        store->min = (darray_empty(store->माताअस))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->माताअस, 0).keycode;
    } else {
        for (xkb_keycode_t kc = store->min; kc < darray_size(store->माबाबा); kc++) {
            if (darray_item(store->माबाबा, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                break;
            }
        }
    }
}

static inline xkb_keycode_t
keycode_store_get_keycode(const KeycodeStore *store, KeycodeMatch match)
{
    if (!match.found || match.is_alias) {
        return XKB_KEYCODE_INVALID;
    } else if (match.सुसि.माबाबा) {
        /* assume valid index */
        assert(match.सुसि.index < darray_size(store->माबाबा));
        return (xkb_keycode_t) match.सुसि.index;
    } else {
        /* assume valid index */
        assert(match.सुसि.index < darray_size(store->माताअस));
        return darray_item(store->माताअस, match.सुसि.index).keycode;
    }
}

static inline xkb_atom_t
keycode_store_get_सुसि_name(const KeycodeStore *store, KeycodeMatch match)
{
    if (!match.found || match.is_alias) {
        return XKB_ATOM_NONE;
    } else if (match.सुसि.माबाबा) {
        /* assume valid index */
        assert(match.सुसि.index < darray_size(store->माबाबा));
        return darray_item(store->माबाबा, match.सुसि.index);
    } else {
        /* assume valid index */
        assert(match.सुसि.index < darray_size(store->माताअस));
        return darray_item(store->माताअस, match.सुसि.index).name;
    }
}

static KeycodeMatch
keycode_store_lookup_keycode(const KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    if (kc < (xkb_keycode_t) darray_size(store->माबाबा)) {
        return (KeycodeMatch) {
            .सुसि = {
                .found = true,
                .माबाबा = true,
                .is_alias = false,
                .index = kc
            }
        };
    } else if (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        return (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->माताअस);
    while (lower < upper) {
        const darray_size_t mid = lower + (upper - 1 - lower) / 2;
        माताअस_Keycode_Entry * const entry = &darray_item(store->माताअस, mid);
        if (entry->keycode < kc) {
            lower = mid + 1;
        } else if (entry->keycode > kc) {
            upper = mid;
        } else {
            return (KeycodeMatch) {
                .सुसि = {
                    .found = true,
                    .माबाबा = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    return (KeycodeMatch) { .found = false };
}

static KeycodeMatch
keycode_store_lookup_name(const KeycodeStore *store, xkb_atom_t name)
{
    if (name >= darray_size(store->names)) {
        return (KeycodeMatch) { .found = false, .is_alias = false };
    } else {
        return darray_item(store->names, name);
    }
}

/***====================================================================***/

typedef struct {
    enum merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

typedef struct {
    char *name;
    int error_बिलांग;
    unsigned int include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t नुमेरो_led_names;

    struct xkb_context *ctx;
    const struct xkb_keymap_info *keymap_info;
} सुसि_Names_Info;

/***====================================================================***/

static LedNameInfo *
FindLedByName(सुसि_Names_Info *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    for (xkb_led_index_t idx = 0; idx < info->नुमेरो_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        if (ledi->name == name) {
            *idx_out = idx;
            return ledi;
        }
    }

    return NULL;
}

static bool
AddLedName(सुसि_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    const bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    if (old) {
        if (old_idx == new_idx) {
            if (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            return true;
        }

        if (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        if (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } else {
            return true;
        }
    }

    if (new_idx >= info->नुमेरो_led_names)
        info->नुमेरो_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    if (old->name != XKB_ATOM_NONE) {
        if (report) {
            const xkb_atom_t use    = (replace ? new->name : old->name);
            const xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        if (replace)
            *old = *new;

        return true;
    }

    *old = *new;
    return true;
}

static void
Clear_सुसि_Names_Info(सुसि_Names_Info *info)
{
    free(info->name);
    keycode_store_free(&info->keycodes);
}

static void
Init_सुसि_Names_Info(सुसि_Names_Info *info, const struct xkb_keymap_info *keymap_info,
                 unsigned int include_depth)
{
    memset(info, 0, sizeof(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

static bool
Add_सुसि_Name(सुसि_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           enum merge_mode merge, bool report)
{
#ifdef NDEBUG
    const
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    if (match_name.found) {
        const bool clobber = (merge != MERGE_AUGMENT);

        if (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            if (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_सुसि_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         सुसि_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            if (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } else {
                return true;
            }
        } else {
            const xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            assert(old_kc != XKB_KEYCODE_INVALID);
            if (old_kc != kc) {
                /* There is already a different key with this name. */

                if (report) {
                    const xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    const xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_सुसि_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            सुसि_Name_Text(info->ctx, name), use, ignore);
                }

                if (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_सुसि(&info->keycodes, match_name);
                } else {
                    return true;
                }
            }
        }
    }

    const KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    const xkb_atom_t old_name = keycode_store_get_सुसि_name(&info->keycodes, match_kc);
    if (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        if (old_name == name) {
            assert(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            if (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         सुसि_Name_Text(info->ctx, old_name), kc);
            return true;
        }

        const bool clobber = (merge != MERGE_AUGMENT);
        if (report) {
            const char* const kname     = सुसि_Name_Text(info->ctx, name);
            const char* const old_kname = सुसि_Name_Text(info->ctx, old_name);
            const char* const use    = (clobber) ? kname : old_kname;
            const char* const ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        if (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_सुसि(&info->keycodes, match_kc, name);
        }
    } else {
        /* No previous keycode */
        if (!keycode_store_इसिंगित_सुसि(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            return false;
        }
    }

    return true;
}

/***====================================================================***/

static bool
HandleAliasDef(सुसि_Names_Info *info, const सुसि_Alias_Def *def, bool report);

static void
MergeKeycodeStores(सुसि_Names_Info *into, सुसि_Names_Info *from,
                   enum merge_mode merge, bool report)
{
    if (darray_empty(into->keycodes.माबाबा) && darray_empty(into->keycodes.माताअस) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.माबाबा);
        darray_init(from->keycodes.माताअस);
        darray_init(from->keycodes.names);
    } else {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        for (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.माबाबा);
             kc++) {
            /* Skip undefined keycodes */
            const xkb_atom_t name = darray_item(from->keycodes.माबाबा, kc);
            if (name == XKB_ATOM_NONE)
                continue;

            if (!Add_सुसि_Name(into, kc, name, merge, report))
                into->error_बिलांग++;
        }

        /* High keycodes */
        const माताअस_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.माताअस) {
            assert(new->name != XKB_ATOM_NONE);
            if (!Add_सुसि_Name(into, new->keycode, new->name, merge, report))
                into->error_बिलांग++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            if (!match->found || !match->is_alias)
                continue;

            const सुसि_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            if (!HandleAliasDef(into, &def, report))
                into->error_बिलांग++;
        }
    }
}

static void
MergeIncludedKeycodes(सुसि_Names_Info *into, सुसि_Names_Info *from,
                      enum merge_mode merge, bool report)
{
    if (from->error_बिलांग > 0) {
        into->error_बिलांग += from->error_बिलांग;
        return;
    }

    if (into->name == NULL) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    if (into->नुमेरो_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               sizeof(*from->led_names) * from->नुमेरो_led_names);
        into->नुमेरो_led_names = from->नुमेरो_led_names;
        from->नुमेरो_led_names = 0;
    }
    else {
        for (xkb_led_index_t idx = 0; idx < from->नुमेरो_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            if (ledi->name == XKB_ATOM_NONE)
                continue;

            ledi->merge = merge;
            if (!AddLedName(into, false, ledi, idx, report))
                into->error_बिलांग++;
        }
    }
}

static void
HandleKeycodesFile(सुसि_Names_Info *info, XkbFile *file);

static bool
HandleIncludeKeycodes(सुसि_Names_Info *info, IncludeStmt *include, bool report)
{
    सुसि_Names_Info included;

    if (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_बिलांग += 10;
        return false;
    }

    Init_सुसि_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    const struct parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    for (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        सुसि_Names_Info next_incl;
        XkbFile *file;

        char path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, sizeof(path));
        if (!file) {
            info->error_बिलांग += 10;
            Clear_सुसि_Names_Info(&included);
            return false;
        }

        Init_सुसि_Names_Info(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        Clear_सुसि_Names_Info(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_सुसि_Names_Info(&included);

    return (info->error_बिलांग == 0);
}

static bool
HandleKeycodeDef(सुसि_Names_Info *info, KeycodeDef *stmt, bool report)
{
    if (stmt->हालागा < 0 || stmt->हालागा > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->हालागा, XKB_KEYCODE_MAX);
        return false;
    }

    return Add_सुसि_Name(info, (xkb_keycode_t) stmt->हालागा,
                      stmt->name, stmt->merge, report);
}

static bool
HandleAliasDef(सुसि_Names_Info *info, const सुसि_Alias_Def *def, bool report)
{
    const KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    if (match_name.found) {
        const bool clobber = (def->merge != MERGE_AUGMENT);
        if (match_name.is_alias) {
            if (def->real == match_name.alias.real) {
                if (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_सुसि_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             सुसि_Name_Text(info->ctx, def->alias),
                             सुसि_Name_Text(info->ctx, def->real));
            } else {
                const xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                const xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                if (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_सुसि_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             सुसि_Name_Text(info->ctx, def->alias),
                             सुसि_Name_Text(info->ctx, use),
                             सुसि_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            return true;
        } else {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            if (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_सुसि_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         सुसि_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            if (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_सुसि(&info->keycodes, match_name);
            } else {
                return true;
            }
        }
    }

    return keycode_store_इसिंगित_alias(&info->keycodes, def->alias, def->real);
}

static bool
Handle_सुसि_Name_Var(सुसि_Names_Info *info, VarDef *stmt)
{
    const char *elem, *field;
    पाहायाग_Def *arrayNdx;

    if (!पाहायाग_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        return false;

    if (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        return !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    if (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        return !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    if (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        return !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    if (!पाहायाग_Resolve_Integer(info->ctx, stmt->हालागा, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        return !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    return true;
}

static bool
HandleLedNameDef(सुसि_Names_Info *info, LedNameDef *def, bool report)
{
    if (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_बिलांग++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        return false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    if (!पाहायाग_Resolve_String(info->ctx, def->name, &name)) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%"PRId64, def->ndx);
        info->error_बिलांग++;
        return ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    return AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

static void
HandleKeycodesFile(सुसि_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    const int verbosity = xkb_context_get_log_verbosity(info->ctx);
    const bool report_same_file = verbosity > 0;
    const bool report_include   = verbosity > 7;

    free(info->name);
    info->name = strdup_safe(file->name);

    for (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        switch (stmt->type) {
        case STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            break;
        case STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            break;
        case STMT_ALIAS:
            ok = HandleAliasDef(info, (सुसि_Alias_Def *) stmt, report_same_file);
            break;
        case STMT_VAR:
            ok = Handle_सुसि_Name_Var(info, (VarDef *) stmt);
            break;
        case STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            break;
        case STMT_UNKNOWN_DECLARATION:
        case STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            break;
        default:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            break;
        }

        if (!ok)
            info->error_बिलांग++;

        if (info->error_बिलांग > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            break;
        }
    }
}

/***====================================================================***/

static bool
Copy_सुसि_Names_To_Keymap(struct xkb_keymap *keymap, सुसि_Names_Info *info)
{
    if (darray_empty(info->keycodes.माबाबा) && darray_empty(info->keycodes.माताअस)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        assert(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_सुसि_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_सुसि_code = 255;
        keymap->नुमेरो_मगा_सुसि = keymap->नुमेरो_मगा_सुसि_माबाबा = keymap->max_सुसि_code + 1;
    } else {
        assert(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_सुसि_code = info->keycodes.min;
        keymap->max_सुसि_code = (darray_empty(info->keycodes.माताअस))
            ? darray_size(info->keycodes.माबाबा) - 1
            : darray_item(
                info->keycodes.माताअस,
                darray_size(info->keycodes.माताअस) - 1
            ).keycode;
        keymap->नुमेरो_मगा_सुसि_माबाबा = darray_size(info->keycodes.माबाबा);
        keymap->नुमेरो_मगा_सुसि = keymap->नुमेरो_मगा_सुसि_माबाबा
                         + darray_size(info->keycodes.माताअस);
    }

    struct xkb_सुसि * const मगा_सुसि = calloc(keymap->नुमेरो_मगा_सुसि, sizeof(*मगा_सुसि));
    if (!मगा_सुसि) {
        keymap->नुमेरो_मगा_सुसि = 0;
        keymap->min_सुसि_code = keymap->max_सुसि_code = XKB_KEYCODE_INVALID;
        return false;
    }

    /* Low keycodes */
    for (xkb_keycode_t kc = keymap->min_सुसि_code; kc < keymap->नुमेरो_मगा_सुसि_माबाबा; kc++)
        मगा_सुसि[kc].keycode = kc;

    for (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.माबाबा);
         kc++)
        मगा_सुसि[kc].name = darray_item(info->keycodes.माबाबा, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->नुमेरो_मगा_सुसि_माबाबा; /* First high keycode index */
    const माताअस_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.माताअस) {
        assert(entry->name != XKB_ATOM_NONE);
        मगा_सुसि[idx].keycode = entry->keycode;
        मगा_सुसि[idx].name = entry->name;
        idx++;
    }

    keymap->मगा_सुसि = मगा_सुसि;
    return true;
}

static bool
CopyKeycodeNameLUT(struct xkb_keymap *keymap, सुसि_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        if (!match->found)
            continue;

        if (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            const KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            if (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        सुसि_Name_Text(info->ctx, name),
                        सुसि_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                continue;
            }
            assert(!match_real.is_alias);
        } else if (!match->सुसि.माबाबा) {
            /* Update to final index in keymap::keys */
            match->सुसि.index += keymap->नुमेरो_मगा_सुसि_माबाबा;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->नुमेरो_सुसि_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->सुसि_names, NULL);
    darray_init(info->keycodes.names);
    return true;
}

static bool
CopyLedNamesToKeymap(struct xkb_keymap *keymap, सुसि_Names_Info *info)
{
    keymap->नुमेरो_leds = info->नुमेरो_led_names;
    for (xkb_led_index_t idx = 0; idx < info->नुमेरो_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        if (ledi->name == XKB_ATOM_NONE)
            continue;

        keymap->leds[idx].name = ledi->name;
    }

    return true;
}

static bool
Copy_सुसि_Names_Info_To_Keymap(struct xkb_keymap *keymap, सुसि_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    if (!Copy_सुसि_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        return false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    if (!keymap->नुमेरो_मगा_सुसि ||
        keymap->min_सुसि_code > 0) {
        keymap->redirect_सुसि_auto = 0;
    } else {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        for (xkb_keycode_t k = keymap->नुमेरो_मगा_सुसि; k-- > keymap->नुमेरो_मगा_सुसि_माबाबा;) {
            if (keycode > keymap->मगा_सुसि[k].keycode)
                break;
            /* Try next keycode downwards */
            keycode = keymap->मगा_सुसि[k].keycode - 1;
        }
        keymap->redirect_सुसि_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    return true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, struct xkb_keymap_info *keymap_info)
{
    सुसि_Names_Info info;

    Init_सुसि_Names_Info(&info, keymap_info, 0);

    if (file != NULL)
        HandleKeycodesFile(&info, file);

    if (info.error_बिलांग != 0)
        goto err_info;

    if (!Copy_सुसि_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        goto err_info;

    Clear_सुसि_Names_Info(&info);
    return true;

err_info:
    Clear_सुसि_Names_Info(&info);
    return false;
}
