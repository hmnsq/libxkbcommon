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
} HighKeycodeEntry;

typedef struct {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) low;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(HighKeycodeEntry) high;
    /**
     * name -> keycode mapping
     */
    darray(KeycodeMatch) names;
} KeycodeStore;

static inline void
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->low);
    darray_init(store->high);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

static inline void
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->low);
    darray_free(store->high);
    darray_free(store->names);
}

static inline void
keycode_store_update_key(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    如果 (unlikely(!match.found || match.is_alias)) {
        返回;
    } 否则 如果 (match.key.low) {
        /* assume valid index */
        assert(match.key.index < darray_size(store->low));
        darray_item(store->low, match.key.index) = name;
    } 否则 {
        /* assume valid index */
        assert(match.key.index < darray_size(store->high));
        darray_item(store->high, match.key.index).name = name;
    }

    /* Names lookup */
    如果 (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

static bool
keycode_store_insert_key(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    如果 (name >= darray_size(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    如果 (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        如果 (kc >= (xkb_keycode_t) darray_size(store->low))
            darray_resize0(store->low, kc + 1);
        darray_item(store->low, kc) = name;
        如果 (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .key = {
                .found = true,
                .low = true,
                .is_alias = false,
                .index = kc
            }
        };
    } 否则 {
        /* High keycode: insert into a sorted list */
        const darray_size_t idx = darray_size(store->high);
        如果 (idx && darray_item(store->high, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_size_t lower = 0;
            darray_size_t upper = idx;
            当 (lower < upper) {
                const darray_size_t mid = lower + (upper - 1 - lower) / 2;
                const HighKeycodeEntry * const entry =
                    &darray_item(store->high, mid);
                如果 (entry->keycode < kc) {
                    lower = mid + 1;
                } 否则 如果 (entry->keycode > kc) {
                    upper = mid;
                } 否则 {
                    /* Unreachable: there is no repetition in the list */
                    assert(entry->keycode != kc);
                }
            }
            assert(lower < idx);
            assert(darray_item(store->high, lower).keycode > kc);

            /* Update references to entries that will be moved */
            HighKeycodeEntry *entry;
            darray_foreach_from(entry, store->high, lower) {
                darray_item(store->names, entry->name).key.index++;
            }

            darray_insert(store->high, lower,
                          (HighKeycodeEntry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .key = {
                    .found = true,
                    .low = false,
                    .is_alias = false,
                    .index = lower
                }
            };
        } 否则 {
            /* Fast path: no need to sort */
            darray_append(store->high,
                          (HighKeycodeEntry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .key = {
                    .found = true,
                    .low = false,
                    .is_alias = false,
                    .index = idx
                }
            };
        }
        如果 (darray_empty(store->low))
            store->min = darray_item(store->high, 0).keycode;
    }
    返回 true;
}

static inline bool
keycode_store_insert_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    如果 (alias >= darray_size(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    返回 true;
}

static inline bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    返回 true;
}

static inline void
keycode_store_delete_name(const KeycodeStore *store, xkb_atom_t name)
{
    如果 (name < darray_size(store->names))
        darray_item(store->names, name).found = false;
}

static void
keycode_store_delete_key(KeycodeStore *store, const KeycodeMatch match)
{
    如果 (unlikely(!match.found || match.is_alias)) {
        返回;
    } 否则 如果 (match.key.low) {
        /* assume valid index */
        assert(match.key.index < darray_size(store->low));
        darray_item(store->names,
                    darray_item(store->low, match.key.index)).found = false;
        如果 (match.key.index + 1u == darray_size(store->low)) {
            /* Highest low keycode: shrink */
            如果 (store->min == match.key.index) {
                /* No low keycode left */
                darray_size(store->low) = 0;
            } 否则 {
                /* Look for previous defined low keycode */
                为 (darray_size_t idx = match.key.index; idx > 0; idx--) {
                    如果 (darray_item(store->low, idx - 1) != XKB_ATOM_NONE) {
                        darray_size(store->low) = idx;
                        break;
                    }
                }
            }
        } 否则 {
            /* Lower keycode: reset */
            darray_item(store->low, match.key.index) = XKB_ATOM_NONE;
        }
    } 否则 {
        assert(match.key.index < darray_size(store->high)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->high, match.key.index).name).found = false;
        darray_remove(store->high, match.key.index);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            如果 (entry->found && !entry->is_alias && !entry->key.low &&
                entry->key.index > match.key.index) {
                entry->key.index--;
            }
        }
    }

    /* Update bounds */
    如果 (darray_empty(store->low)) {
        store->min = (darray_empty(store->high))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->high, 0).keycode;
    } 否则 {
        为 (xkb_keycode_t kc = store->min; kc < darray_size(store->low); kc++) {
            如果 (darray_item(store->low, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                break;
            }
        }
    }
}

static inline xkb_keycode_t
keycode_store_get_keycode(const KeycodeStore *store, KeycodeMatch match)
{
    如果 (!match.found || match.is_alias) {
        返回 XKB_KEYCODE_INVALID;
    } 否则 如果 (match.key.low) {
        /* assume valid index */
        assert(match.key.index < darray_size(store->low));
        返回 (xkb_keycode_t) match.key.index;
    } 否则 {
        /* assume valid index */
        assert(match.key.index < darray_size(store->high));
        返回 darray_item(store->high, match.key.index).keycode;
    }
}

static inline xkb_atom_t
keycode_store_get_key_name(const KeycodeStore *store, KeycodeMatch match)
{
    如果 (!match.found || match.is_alias) {
        返回 XKB_ATOM_NONE;
    } 否则 如果 (match.key.low) {
        /* assume valid index */
        assert(match.key.index < darray_size(store->low));
        返回 darray_item(store->low, match.key.index);
    } 否则 {
        /* assume valid index */
        assert(match.key.index < darray_size(store->high));
        返回 darray_item(store->high, match.key.index).name;
    }
}

static KeycodeMatch
keycode_store_lookup_keycode(const KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    如果 (kc < (xkb_keycode_t) darray_size(store->low)) {
        返回 (KeycodeMatch) {
            .key = {
                .found = true,
                .low = true,
                .is_alias = false,
                .index = kc
            }
        };
    } 否则 如果 (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        返回 (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_size_t lower = 0;
    darray_size_t upper = darray_size(store->high);
    当 (lower < upper) {
        const darray_size_t mid = lower + (upper - 1 - lower) / 2;
        HighKeycodeEntry * const entry = &darray_item(store->high, mid);
        如果 (entry->keycode < kc) {
            lower = mid + 1;
        } 否则 如果 (entry->keycode > kc) {
            upper = mid;
        } 否则 {
            返回 (KeycodeMatch) {
                .key = {
                    .found = true,
                    .low = false,
                    .is_alias = false,
                    .index = mid
                }
            };
        }
    }

    返回 (KeycodeMatch) { .found = false };
}

static KeycodeMatch
keycode_store_lookup_name(const KeycodeStore *store, xkb_atom_t name)
{
    如果 (name >= darray_size(store->names)) {
        返回 (KeycodeMatch) { .found = false, .is_alias = false };
    } 否则 {
        返回 darray_item(store->names, name);
    }
}

/***====================================================================***/

typedef struct {
    enum merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

typedef struct {
    char *name;
    整数 errorCount;
    unsigned 整数 include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_index_t num_led_names;

    struct xkb_context *ctx;
    const struct xkb_keymap_info *keymap_info;
} KeyNamesInfo;

/***====================================================================***/

static LedNameInfo *
FindLedByName(KeyNamesInfo *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    为 (xkb_led_index_t idx = 0; idx < info->num_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        如果 (ledi->name == name) {
            *idx_out = idx;
            返回 ledi;
        }
    }

    返回 NULL;
}

static bool
AddLedName(KeyNamesInfo *info, bool same_file,
           LedNameInfo *new, xkb_led_index_t new_idx, bool report)
{
    xkb_led_index_t old_idx;
    LedNameInfo *old;
    const bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    如果 (old) {
        如果 (old_idx == new_idx) {
            如果 (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            返回 true;
        }

        如果 (report) {
            xkb_led_index_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_index_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        如果 (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } 否则 {
            返回 true;
        }
    }

    如果 (new_idx >= info->num_led_names)
        info->num_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    如果 (old->name != XKB_ATOM_NONE) {
        如果 (report) {
            const xkb_atom_t use    = (replace ? new->name : old->name);
            const xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        如果 (replace)
            *old = *new;

        返回 true;
    }

    *old = *new;
    返回 true;
}

static void
ClearKeyNamesInfo(KeyNamesInfo *info)
{
    free(info->name);
    keycode_store_free(&info->keycodes);
}

static void
InitKeyNamesInfo(KeyNamesInfo *info, const struct xkb_keymap_info *keymap_info,
                 unsigned 整数 include_depth)
{
    memset(info, 0, sizeof(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

static bool
AddKeyName(KeyNamesInfo *info, xkb_keycode_t kc, xkb_atom_t name,
           enum merge_mode merge, bool report)
{
#ifdef NDEBUG
    const
#endif
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    如果 (match_name.found) {
        const bool clobber = (merge != MERGE_AUGMENT);

        如果 (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            如果 (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         KeyNameText(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            如果 (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#ifndef NDEBUG
                match_name.found = false;
#endif
            } 否则 {
                返回 true;
            }
        } 否则 {
            const xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            assert(old_kc != XKB_KEYCODE_INVALID);
            如果 (old_kc != kc) {
                /* There is already a different key with this name. */

                如果 (report) {
                    const xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    const xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            KeyNameText(info->ctx, name), use, ignore);
                }

                如果 (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_key(&info->keycodes, match_name);
                } 否则 {
                    返回 true;
                }
            }
        }
    }

    const KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    const xkb_atom_t old_name = keycode_store_get_key_name(&info->keycodes, match_kc);
    如果 (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        如果 (old_name == name) {
            assert(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            如果 (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         KeyNameText(info->ctx, old_name), kc);
            返回 true;
        }

        const bool clobber = (merge != MERGE_AUGMENT);
        如果 (report) {
            const char* const kname     = KeyNameText(info->ctx, name);
            const char* const old_kname = KeyNameText(info->ctx, old_name);
            const char* const use    = (clobber) ? kname : old_kname;
            const char* const ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        如果 (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_key(&info->keycodes, match_kc, name);
        }
    } 否则 {
        /* No previous keycode */
        如果 (!keycode_store_insert_key(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            返回 false;
        }
    }

    返回 true;
}

/***====================================================================***/

static bool
HandleAliasDef(KeyNamesInfo *info, const KeyAliasDef *def, bool report);

static void
MergeKeycodeStores(KeyNamesInfo *into, KeyNamesInfo *from,
                   enum merge_mode merge, bool report)
{
    如果 (darray_empty(into->keycodes.low) && darray_empty(into->keycodes.high) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.low);
        darray_init(from->keycodes.high);
        darray_init(from->keycodes.names);
    } 否则 {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        为 (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_size(from->keycodes.low);
             kc++) {
            /* Skip undefined keycodes */
            const xkb_atom_t name = darray_item(from->keycodes.low, kc);
            如果 (name == XKB_ATOM_NONE)
                continue;

            如果 (!AddKeyName(into, kc, name, merge, report))
                into->errorCount++;
        }

        /* High keycodes */
        const HighKeycodeEntry *new;
        darray_foreach(new, from->keycodes.high) {
            assert(new->name != XKB_ATOM_NONE);
            如果 (!AddKeyName(into, new->keycode, new->name, merge, report))
                into->errorCount++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            如果 (!match->found || !match->is_alias)
                continue;

            const KeyAliasDef def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            如果 (!HandleAliasDef(into, &def, report))
                into->errorCount++;
        }
    }
}

static void
MergeIncludedKeycodes(KeyNamesInfo *into, KeyNamesInfo *from,
                      enum merge_mode merge, bool report)
{
    如果 (from->errorCount > 0) {
        into->errorCount += from->errorCount;
        返回;
    }

    如果 (into->name == NULL) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    如果 (into->num_led_names == 0) {
        memcpy(into->led_names, from->led_names,
               sizeof(*from->led_names) * from->num_led_names);
        into->num_led_names = from->num_led_names;
        from->num_led_names = 0;
    }
    否则 {
        为 (xkb_led_index_t idx = 0; idx < from->num_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            如果 (ledi->name == XKB_ATOM_NONE)
                continue;

            ledi->merge = merge;
            如果 (!AddLedName(into, false, ledi, idx, report))
                into->errorCount++;
        }
    }
}

static void
HandleKeycodesFile(KeyNamesInfo *info, XkbFile *file);

static bool
HandleIncludeKeycodes(KeyNamesInfo *info, IncludeStmt *include, bool report)
{
    KeyNamesInfo included;

    如果 (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->errorCount += 10;
        返回 false;
    }

    InitKeyNamesInfo(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    const struct parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    为 (IncludeStmt *stmt = include; stmt; stmt = stmt->next_incl) {
        KeyNamesInfo next_incl;
        XkbFile *file;

        char path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, sizeof(path));
        如果 (!file) {
            info->errorCount += 10;
            ClearKeyNamesInfo(&included);
            返回 false;
        }

        InitKeyNamesInfo(&next_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&next_incl, file);

        MergeIncludedKeycodes(&included, &next_incl, stmt->merge, report);

        ClearKeyNamesInfo(&next_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    ClearKeyNamesInfo(&included);

    返回 (info->errorCount == 0);
}

static bool
HandleKeycodeDef(KeyNamesInfo *info, KeycodeDef *stmt, bool report)
{
    如果 (stmt->value < 0 || stmt->value > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->value, XKB_KEYCODE_MAX);
        返回 false;
    }

    返回 AddKeyName(info, (xkb_keycode_t) stmt->value,
                      stmt->name, stmt->merge, report);
}

static bool
HandleAliasDef(KeyNamesInfo *info, const KeyAliasDef *def, bool report)
{
    const KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    如果 (match_name.found) {
        const bool clobber = (def->merge != MERGE_AUGMENT);
        如果 (match_name.is_alias) {
            如果 (def->real == match_name.alias.real) {
                如果 (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             KeyNameText(info->ctx, def->alias),
                             KeyNameText(info->ctx, def->real));
            } 否则 {
                const xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                const xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                如果 (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             KeyNameText(info->ctx, def->alias),
                             KeyNameText(info->ctx, use),
                             KeyNameText(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            返回 true;
        } 否则 {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            如果 (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_KEY_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         KeyNameText(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            如果 (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_key(&info->keycodes, match_name);
            } 否则 {
                返回 true;
            }
        }
    }

    返回 keycode_store_insert_alias(&info->keycodes, def->alias, def->real);
}

static bool
HandleKeyNameVar(KeyNamesInfo *info, VarDef *stmt)
{
    const char *elem, *field;
    ExprDef *arrayNdx;

    如果 (!ExprResolveLhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        返回 false;

    如果 (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        返回 !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    如果 (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        返回 !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    如果 (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        返回 !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    如果 (!ExprResolveInteger(info->ctx, stmt->value, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        返回 !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    返回 true;
}

static bool
HandleLedNameDef(KeyNamesInfo *info, LedNameDef *def, bool report)
{
    如果 (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->errorCount++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        返回 false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    如果 (!ExprResolveString(info->ctx, def->name, &name)) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%"PRId64, def->ndx);
        info->errorCount++;
        返回 ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    返回 AddLedName(info, true, &ledi, (xkb_led_index_t) def->ndx - 1, report);
}

static void
HandleKeycodesFile(KeyNamesInfo *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    const 整数 verbosity = xkb_context_get_log_verbosity(info->ctx);
    const bool report_same_file = verbosity > 0;
    const bool report_include   = verbosity > 7;

    free(info->name);
    info->name = strdup_safe(file->name);

    为 (ParseCommon *stmt = file->defs; stmt; stmt = stmt->next) {
        switch (stmt->type) {
        case STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            break;
        case STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            break;
        case STMT_ALIAS:
            ok = HandleAliasDef(info, (KeyAliasDef *) stmt, report_same_file);
            break;
        case STMT_VAR:
            ok = HandleKeyNameVar(info, (VarDef *) stmt);
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

        如果 (!ok)
            info->errorCount++;

        如果 (info->errorCount > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            break;
        }
    }
}

/***====================================================================***/

static bool
CopyKeyNamesToKeymap(struct xkb_keymap *keymap, KeyNamesInfo *info)
{
    如果 (darray_empty(info->keycodes.low) && darray_empty(info->keycodes.high)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        assert(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_key_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_key_code = 255;
        keymap->num_keys = keymap->num_keys_low = keymap->max_key_code + 1;
    } 否则 {
        assert(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_key_code = info->keycodes.min;
        keymap->max_key_code = (darray_empty(info->keycodes.high))
            ? darray_size(info->keycodes.low) - 1
            : darray_item(
                info->keycodes.high,
                darray_size(info->keycodes.high) - 1
            ).keycode;
        keymap->num_keys_low = darray_size(info->keycodes.low);
        keymap->num_keys = keymap->num_keys_low
                         + darray_size(info->keycodes.high);
    }

    struct xkb_key * const keys = calloc(keymap->num_keys, sizeof(*keys));
    如果 (!keys) {
        keymap->num_keys = 0;
        keymap->min_key_code = keymap->max_key_code = XKB_KEYCODE_INVALID;
        返回 false;
    }

    /* Low keycodes */
    为 (xkb_keycode_t kc = keymap->min_key_code; kc < keymap->num_keys_low; kc++)
        keys[kc].keycode = kc;

    为 (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_size(info->keycodes.low);
         kc++)
        keys[kc].name = darray_item(info->keycodes.low, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->num_keys_low; /* First high keycode index */
    const HighKeycodeEntry *entry;
    darray_foreach(entry, info->keycodes.high) {
        assert(entry->name != XKB_ATOM_NONE);
        keys[idx].keycode = entry->keycode;
        keys[idx].name = entry->name;
        idx++;
    }

    keymap->keys = keys;
    返回 true;
}

static bool
CopyKeycodeNameLUT(struct xkb_keymap *keymap, KeyNamesInfo *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        如果 (!match->found)
            continue;

        如果 (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            const KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            如果 (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        KeyNameText(info->ctx, name),
                        KeyNameText(info->ctx, match->alias.real));
                match->found = false;
                continue;
            }
            assert(!match_real.is_alias);
        } 否则 如果 (!match->key.low) {
            /* Update to final index in keymap::keys */
            match->key.index += keymap->num_keys_low;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->num_key_names = darray_size(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->key_names, NULL);
    darray_init(info->keycodes.names);
    返回 true;
}

static bool
CopyLedNamesToKeymap(struct xkb_keymap *keymap, KeyNamesInfo *info)
{
    keymap->num_leds = info->num_led_names;
    为 (xkb_led_index_t idx = 0; idx < info->num_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        如果 (ledi->name == XKB_ATOM_NONE)
            continue;

        keymap->leds[idx].name = ledi->name;
    }

    返回 true;
}

static bool
CopyKeyNamesInfoToKeymap(struct xkb_keymap *keymap, KeyNamesInfo *info)
{
    /* This function trashes keymap on error, but that's OK. */
    如果 (!CopyKeyNamesToKeymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        返回 false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    如果 (!keymap->num_keys ||
        keymap->min_key_code > 0) {
        keymap->redirect_key_auto = 0;
    } 否则 {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        为 (xkb_keycode_t k = keymap->num_keys; k-- > keymap->num_keys_low;) {
            如果 (keycode > keymap->keys[k].keycode)
                break;
            /* Try next keycode downwards */
            keycode = keymap->keys[k].keycode - 1;
        }
        keymap->redirect_key_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    返回 true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, struct xkb_keymap_info *keymap_info)
{
    KeyNamesInfo info;

    InitKeyNamesInfo(&info, keymap_info, 0);

    如果 (file != NULL)
        HandleKeycodesFile(&info, file);

    如果 (info.errorCount != 0)
        goto err_info;

    如果 (!CopyKeyNamesInfoToKeymap(&keymap_info->keymap, &info))
        goto err_info;

    ClearKeyNamesInfo(&info);
    返回 true;

err_info:
    ClearKeyNamesInfo(&info);
    返回 false;
}
