/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <limits.h>
#समावेश <stdint.h>
#समावेश <stdlib.h>
#समावेश <string.h>

#समावेश "context.h"
#समावेश "messages-codes.h"
#समावेश "xkbcommon/xkbcommon.h"
#समावेश "xkbcomp-priv.h"
#समावेश "atom.h"
#समावेश "darray.h"
#समावेश "keymap.h"
#समावेश "text.h"
#समावेश "expr.h"
#समावेश "include.h"
#समावेश "util-mem.h"

प्रकार काष्ठा {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} ऊपरी_Keycode_Entry;

प्रकार काष्ठा {
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

अटल अंतरभूत व्योम
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->निचला);
    darray_init(store->ऊपरी);
    darray_init(store->names);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

अटल अंतरभूत व्योम
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->निचला);
    darray_free(store->ऊपरी);
    darray_free(store->names);
}

अटल अंतरभूत व्योम
keycode_store_update_कुंजी(KeycodeStore *store, KeycodeMatch match, xkb_atom_t name)
{
    अगर (unlikely(!match.found || match.is_alias)) {
        वापस;
    } अन्यथा अगर (match.कुंजी.निचला) {
        /* assume valid index */
        निश्चित(match.कुंजी.सूचकांक < darray_आकार(store->निचला));
        darray_item(store->निचला, match.कुंजी.सूचकांक) = name;
    } अन्यथा {
        /* assume valid index */
        निश्चित(match.कुंजी.सूचकांक < darray_आकार(store->ऊपरी));
        darray_item(store->ऊपरी, match.कुंजी.सूचकांक).name = name;
    }

    /* Names lookup */
    अगर (name >= darray_आकार(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    darray_item(store->names, name) = match;
}

अटल bool
keycode_store_जोड़ो_कुंजी(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    अगर (name >= darray_आकार(store->names)) {
        darray_resize0(store->names, name + 1);
    }
    अगर (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        अगर (kc >= (xkb_keycode_t) darray_आकार(store->निचला))
            darray_resize0(store->निचला, kc + 1);
        darray_item(store->निचला, kc) = name;
        अगर (kc < store->min)
            store->min = kc;
        darray_item(store->names, name) = (KeycodeMatch) {
            .कुंजी = {
                .found = true,
                .निचला = true,
                .is_alias = false,
                .सूचकांक = kc
            }
        };
    } अन्यथा {
        /* High keycode: insert into a sorted list */
        स्थिर darray_आकार_t idx = darray_आकार(store->ऊपरी);
        अगर (idx && darray_item(store->ऊपरी, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_आकार_t lower = 0;
            darray_आकार_t upper = idx;
            जबतक (lower < upper) {
                स्थिर darray_आकार_t mid = lower + (upper - 1 - lower) / 2;
                स्थिर ऊपरी_Keycode_Entry * स्थिर entry =
                    &darray_item(store->ऊपरी, mid);
                अगर (entry->keycode < kc) {
                    lower = mid + 1;
                } अन्यथा अगर (entry->keycode > kc) {
                    upper = mid;
                } अन्यथा {
                    /* Unreachable: there is no repetition in the list */
                    निश्चित(entry->keycode != kc);
                }
            }
            निश्चित(lower < idx);
            निश्चित(darray_item(store->ऊपरी, lower).keycode > kc);

            /* Update references to entries that will be moved */
            ऊपरी_Keycode_Entry *entry;
            darray_foreach_from(entry, store->ऊपरी, lower) {
                darray_item(store->names, entry->name).कुंजी.सूचकांक++;
            }

            darray_जोड़ो(store->ऊपरी, lower,
                          (ऊपरी_Keycode_Entry){.keycode = kc, .name = name});
            darray_item(store->names, name) = (KeycodeMatch) {
                .कुंजी = {
                    .found = true,
                    .निचला = false,
                    .is_alias = false,
                    .सूचकांक = lower
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
                    .सूचकांक = idx
                }
            };
        }
        अगर (darray_empty(store->निचला))
            store->min = darray_item(store->ऊपरी, 0).keycode;
    }
    वापस true;
}

अटल अंतरभूत bool
keycode_store_जोड़ो_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    अगर (alias >= darray_आकार(store->names)) {
        darray_resize0(store->names, alias + 1);
    }
    darray_item(store->names, alias) = (KeycodeMatch) {
        .alias = {
            .found = true,
            .is_alias = true,
            .real = real
        }
    };
    वापस true;
}

अटल अंतरभूत bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->names, alias).alias.real = real;
    वापस true;
}

अटल अंतरभूत व्योम
keycode_store_delete_name(स्थिर KeycodeStore *store, xkb_atom_t name)
{
    अगर (name < darray_आकार(store->names))
        darray_item(store->names, name).found = false;
}

अटल व्योम
keycode_store_delete_कुंजी(KeycodeStore *store, स्थिर KeycodeMatch match)
{
    अगर (unlikely(!match.found || match.is_alias)) {
        वापस;
    } अन्यथा अगर (match.कुंजी.निचला) {
        /* assume valid index */
        निश्चित(match.कुंजी.सूचकांक < darray_आकार(store->निचला));
        darray_item(store->names,
                    darray_item(store->निचला, match.कुंजी.सूचकांक)).found = false;
        अगर (match.कुंजी.सूचकांक + 1u == darray_आकार(store->निचला)) {
            /* Highest low keycode: shrink */
            अगर (store->min == match.कुंजी.सूचकांक) {
                /* No low keycode left */
                darray_आकार(store->निचला) = 0;
            } अन्यथा {
                /* Look for previous defined low keycode */
                क्रम (darray_आकार_t idx = match.कुंजी.सूचकांक; idx > 0; idx--) {
                    अगर (darray_item(store->निचला, idx - 1) != XKB_ATOM_NONE) {
                        darray_आकार(store->निचला) = idx;
                        अवरोध;
                    }
                }
            }
        } अन्यथा {
            /* Lower keycode: reset */
            darray_item(store->निचला, match.कुंजी.सूचकांक) = XKB_ATOM_NONE;
        }
    } अन्यथा {
        निश्चित(match.कुंजी.सूचकांक < darray_आकार(store->ऊपरी)); /* assume valid index */
        darray_item(store->names,
                    darray_item(store->ऊपरी, match.कुंजी.सूचकांक).name).found = false;
        darray_remove(store->ऊपरी, match.कुंजी.सूचकांक);
        /* Update LUT indexes of high codes after the deleted one, if any */
        KeycodeMatch *entry;
        darray_foreach(entry, store->names) {
            अगर (entry->found && !entry->is_alias && !entry->कुंजी.निचला &&
                entry->कुंजी.सूचकांक > match.कुंजी.सूचकांक) {
                entry->कुंजी.सूचकांक--;
            }
        }
    }

    /* Update bounds */
    अगर (darray_empty(store->निचला)) {
        store->min = (darray_empty(store->ऊपरी))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->ऊपरी, 0).keycode;
    } अन्यथा {
        क्रम (xkb_keycode_t kc = store->min; kc < darray_आकार(store->निचला); kc++) {
            अगर (darray_item(store->निचला, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                अवरोध;
            }
        }
    }
}

अटल अंतरभूत xkb_keycode_t
keycode_store_get_keycode(स्थिर KeycodeStore *store, KeycodeMatch match)
{
    अगर (!match.found || match.is_alias) {
        वापस XKB_KEYCODE_INVALID;
    } अन्यथा अगर (match.कुंजी.निचला) {
        /* assume valid index */
        निश्चित(match.कुंजी.सूचकांक < darray_आकार(store->निचला));
        वापस (xkb_keycode_t) match.कुंजी.सूचकांक;
    } अन्यथा {
        /* assume valid index */
        निश्चित(match.कुंजी.सूचकांक < darray_आकार(store->ऊपरी));
        वापस darray_item(store->ऊपरी, match.कुंजी.सूचकांक).keycode;
    }
}

अटल अंतरभूत xkb_atom_t
keycode_store_get_कुंजी_name(स्थिर KeycodeStore *store, KeycodeMatch match)
{
    अगर (!match.found || match.is_alias) {
        वापस XKB_ATOM_NONE;
    } अन्यथा अगर (match.कुंजी.निचला) {
        /* assume valid index */
        निश्चित(match.कुंजी.सूचकांक < darray_आकार(store->निचला));
        वापस darray_item(store->निचला, match.कुंजी.सूचकांक);
    } अन्यथा {
        /* assume valid index */
        निश्चित(match.कुंजी.सूचकांक < darray_आकार(store->ऊपरी));
        वापस darray_item(store->ऊपरी, match.कुंजी.सूचकांक).name;
    }
}

अटल KeycodeMatch
keycode_store_lookup_keycode(स्थिर KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    अगर (kc < (xkb_keycode_t) darray_आकार(store->निचला)) {
        वापस (KeycodeMatch) {
            .कुंजी = {
                .found = true,
                .निचला = true,
                .is_alias = false,
                .सूचकांक = kc
            }
        };
    } अन्यथा अगर (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        वापस (KeycodeMatch) { .found = false };
    }

    /* High keycodes: use binary search */
    darray_आकार_t lower = 0;
    darray_आकार_t upper = darray_आकार(store->ऊपरी);
    जबतक (lower < upper) {
        स्थिर darray_आकार_t mid = lower + (upper - 1 - lower) / 2;
        ऊपरी_Keycode_Entry * स्थिर entry = &darray_item(store->ऊपरी, mid);
        अगर (entry->keycode < kc) {
            lower = mid + 1;
        } अन्यथा अगर (entry->keycode > kc) {
            upper = mid;
        } अन्यथा {
            वापस (KeycodeMatch) {
                .कुंजी = {
                    .found = true,
                    .निचला = false,
                    .is_alias = false,
                    .सूचकांक = mid
                }
            };
        }
    }

    वापस (KeycodeMatch) { .found = false };
}

अटल KeycodeMatch
keycode_store_lookup_name(स्थिर KeycodeStore *store, xkb_atom_t name)
{
    अगर (name >= darray_आकार(store->names)) {
        वापस (KeycodeMatch) { .found = false, .is_alias = false };
    } अन्यथा {
        वापस darray_item(store->names, name);
    }
}

/***====================================================================***/

प्रकार काष्ठा {
    क्रमागत merge_mode merge;

    xkb_atom_t name;
} LedNameInfo;

प्रकार काष्ठा {
    अक्षर *name;
    पूर्णांक error_गिनती;
    अचिन्हित पूर्णांक include_depth;

    KeycodeStore keycodes;
    LedNameInfo led_names[XKB_MAX_LEDS];
    xkb_led_सूचकांक_t अंक_led_names;

    काष्ठा xkb_context *ctx;
    स्थिर काष्ठा xkb_keymap_info *keymap_info;
} कुंजी_Names_Info;

/***====================================================================***/

अटल LedNameInfo *
FindLedByName(कुंजी_Names_Info *info, xkb_atom_t name,
              xkb_led_सूचकांक_t *idx_out)
{
    क्रम (xkb_led_सूचकांक_t idx = 0; idx < info->अंक_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        अगर (ledi->name == name) {
            *idx_out = idx;
            वापस ledi;
        }
    }

    वापस शून्य;
}

अटल bool
AddLedName(कुंजी_Names_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_सूचकांक_t new_idx, bool report)
{
    xkb_led_सूचकांक_t old_idx;
    LedNameInfo *old;
    स्थिर bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    अगर (old) {
        अगर (old_idx == new_idx) {
            अगर (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            वापस true;
        }

        अगर (report) {
            xkb_led_सूचकांक_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_सूचकांक_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_text(info->ctx, new->name), use, ignore);
        }

        अगर (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } अन्यथा {
            वापस true;
        }
    }

    अगर (new_idx >= info->अंक_led_names)
        info->अंक_led_names = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_names[new_idx];
    अगर (old->name != XKB_ATOM_NONE) {
        अगर (report) {
            स्थिर xkb_atom_t use    = (replace ? new->name : old->name);
            स्थिर xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_text(info->ctx, use),
                     xkb_atom_text(info->ctx, ignore));
        }

        अगर (replace)
            *old = *new;

        वापस true;
    }

    *old = *new;
    वापस true;
}

अटल व्योम
Clear_कुंजी_Names_Info(कुंजी_Names_Info *info)
{
    मुक्त(info->name);
    keycode_store_free(&info->keycodes);
}

अटल व्योम
Init_कुंजी_Names_Info(कुंजी_Names_Info *info, स्थिर काष्ठा xkb_keymap_info *keymap_info,
                 अचिन्हित पूर्णांक include_depth)
{
    स_रखो(info, 0, माप(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_depth = include_depth;
    keycode_store_init(&info->keycodes);
}

अटल bool
Add_कुंजी_Name(कुंजी_Names_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           क्रमागत merge_mode merge, bool report)
{
#अगर_घोषित न_संशोधन
    स्थिर
#पूर्ण_अगर
    KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes, name);
    अगर (match_name.found) {
        स्थिर bool clobber = (merge != MERGE_AUGMENT);

        अगर (match_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            अगर (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         कुंजी_Name_Text(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            अगर (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#अगर_अघोषित न_संशोधन
                match_name.found = false;
#पूर्ण_अगर
            } अन्यथा {
                वापस true;
            }
        } अन्यथा {
            स्थिर xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, match_name);
            निश्चित(old_kc != XKB_KEYCODE_INVALID);
            अगर (old_kc != kc) {
                /* There is already a different key with this name. */

                अगर (report) {
                    स्थिर xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    स्थिर xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            कुंजी_Name_Text(info->ctx, name), use, ignore);
                }

                अगर (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_कुंजी(&info->keycodes, match_name);
                } अन्यथा {
                    वापस true;
                }
            }
        }
    }

    स्थिर KeycodeMatch match_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    स्थिर xkb_atom_t old_name = keycode_store_get_कुंजी_name(&info->keycodes, match_kc);
    अगर (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        अगर (old_name == name) {
            निश्चित(keycode_store_get_keycode(&info->keycodes, match_name) == kc);
            अगर (report)
                log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         कुंजी_Name_Text(info->ctx, old_name), kc);
            वापस true;
        }

        स्थिर bool clobber = (merge != MERGE_AUGMENT);
        अगर (report) {
            स्थिर अक्षर* स्थिर kname     = कुंजी_Name_Text(info->ctx, name);
            स्थिर अक्षर* स्थिर old_kname = कुंजी_Name_Text(info->ctx, old_name);
            स्थिर अक्षर* स्थिर use    = (clobber) ? kname : old_kname;
            स्थिर अक्षर* स्थिर ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        अगर (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_कुंजी(&info->keycodes, match_kc, name);
        }
    } अन्यथा {
        /* No previous keycode */
        अगर (!keycode_store_जोड़ो_कुंजी(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
                    "Cannot add keycode\n");
            वापस false;
        }
    }

    वापस true;
}

/***====================================================================***/

अटल bool
HandleAliasDef(कुंजी_Names_Info *info, स्थिर कुंजी_Alias_Def *def, bool report);

अटल व्योम
MergeKeycodeStores(कुंजी_Names_Info *into, कुंजी_Names_Info *from,
                   क्रमागत merge_mode merge, bool report)
{
    अगर (darray_empty(into->keycodes.निचला) && darray_empty(into->keycodes.ऊपरी) &&
        darray_empty(into->keycodes.names)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.निचला);
        darray_init(from->keycodes.ऊपरी);
        darray_init(from->keycodes.names);
    } अन्यथा {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        क्रम (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_आकार(from->keycodes.निचला);
             kc++) {
            /* Skip undefined keycodes */
            स्थिर xkb_atom_t name = darray_item(from->keycodes.निचला, kc);
            अगर (name == XKB_ATOM_NONE)
                जारी;

            अगर (!Add_कुंजी_Name(into, kc, name, merge, report))
                into->error_गिनती++;
        }

        /* High keycodes */
        स्थिर ऊपरी_Keycode_Entry *new;
        darray_foreach(new, from->keycodes.ऊपरी) {
            निश्चित(new->name != XKB_ATOM_NONE);
            अगर (!Add_कुंजी_Name(into, new->keycode, new->name, merge, report))
                into->error_गिनती++;
        }

        /* Aliases. */
        KeycodeMatch *match;
        xkb_atom_t alias;
        darray_enumerate(alias, match, from->keycodes.names) {
            अगर (!match->found || !match->is_alias)
                जारी;

            स्थिर कुंजी_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = match->alias.real
            };
            अगर (!HandleAliasDef(into, &def, report))
                into->error_गिनती++;
        }
    }
}

अटल व्योम
MergeIncludedKeycodes(कुंजी_Names_Info *into, कुंजी_Names_Info *from,
                      क्रमागत merge_mode merge, bool report)
{
    अगर (from->error_गिनती > 0) {
        into->error_गिनती += from->error_गिनती;
        वापस;
    }

    अगर (into->name == शून्य) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    अगर (into->अंक_led_names == 0) {
        स_नकल(into->led_names, from->led_names,
               माप(*from->led_names) * from->अंक_led_names);
        into->अंक_led_names = from->अंक_led_names;
        from->अंक_led_names = 0;
    }
    अन्यथा {
        क्रम (xkb_led_सूचकांक_t idx = 0; idx < from->अंक_led_names; idx++) {
            LedNameInfo *ledi = &from->led_names[idx];

            अगर (ledi->name == XKB_ATOM_NONE)
                जारी;

            ledi->merge = merge;
            अगर (!AddLedName(into, false, ledi, idx, report))
                into->error_गिनती++;
        }
    }
}

अटल व्योम
HandleKeycodesFile(कुंजी_Names_Info *info, XkbFile *file);

अटल bool
HandleIncludeKeycodes(कुंजी_Names_Info *info, IncludeStmt *include, bool report)
{
    कुंजी_Names_Info included;

    अगर (ExceedsIncludeMaxDepth(info->ctx, info->include_depth)) {
        info->error_गिनती += 10;
        वापस false;
    }

    Init_कुंजी_Names_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    स्थिर काष्ठा parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    क्रम (IncludeStmt *stmt = include; stmt; stmt = stmt->अग्र_incl) {
        कुंजी_Names_Info अग्र_incl;
        XkbFile *file;

        अक्षर path[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  path, माप(path));
        अगर (!file) {
            info->error_गिनती += 10;
            Clear_कुंजी_Names_Info(&included);
            वापस false;
        }

        Init_कुंजी_Names_Info(&अग्र_incl, info->keymap_info, info->include_depth + 1);

        HandleKeycodesFile(&अग्र_incl, file);

        MergeIncludedKeycodes(&included, &अग्र_incl, stmt->merge, report);

        Clear_कुंजी_Names_Info(&अग्र_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_कुंजी_Names_Info(&included);

    वापस (info->error_गिनती == 0);
}

अटल bool
HandleKeycodeDef(कुंजी_Names_Info *info, KeycodeDef *stmt, bool report)
{
    अगर (stmt->मूल्य < 0 || stmt->मूल्य > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->मूल्य, XKB_KEYCODE_MAX);
        वापस false;
    }

    वापस Add_कुंजी_Name(info, (xkb_keycode_t) stmt->मूल्य,
                      stmt->name, stmt->merge, report);
}

अटल bool
HandleAliasDef(कुंजी_Names_Info *info, स्थिर कुंजी_Alias_Def *def, bool report)
{
    स्थिर KeycodeMatch match_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    अगर (match_name.found) {
        स्थिर bool clobber = (def->merge != MERGE_AUGMENT);
        अगर (match_name.is_alias) {
            अगर (def->real == match_name.alias.real) {
                अगर (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             कुंजी_Name_Text(info->ctx, def->alias),
                             कुंजी_Name_Text(info->ctx, def->real));
            } अन्यथा {
                स्थिर xkb_atom_t use = (clobber)
                    ? def->real
                    : match_name.alias.real;
                स्थिर xkb_atom_t ignore = (clobber)
                    ? match_name.alias.real
                    : def->real;

                अगर (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             कुंजी_Name_Text(info->ctx, def->alias),
                             कुंजी_Name_Text(info->ctx, use),
                             कुंजी_Name_Text(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            वापस true;
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
            अगर (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुंजी_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         कुंजी_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            अगर (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_कुंजी(&info->keycodes, match_name);
            } अन्यथा {
                वापस true;
            }
        }
    }

    वापस keycode_store_जोड़ो_alias(&info->keycodes, def->alias, def->real);
}

अटल bool
Handle_कुंजी_Name_Var(कुंजी_Names_Info *info, VarDef *stmt)
{
    स्थिर अक्षर *elem, *field;
    व्यंजक_Def *arrayNdx;

    अगर (!व्यंजक_Resolve_Lhs(info->ctx, stmt->name, &elem, &field, &arrayNdx))
        वापस false;

    अगर (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, field);
        वापस !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    अगर (!istreq(field, "minimum") && !istreq(field, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_FIELD,
                "Default defined for unknown field \"%s\"; Ignored\n", field);
        वापस !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    अगर (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", field, "defaults");
        वापस !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    int64_t val = 0;
    अगर (!व्यंजक_Resolve_Integer(info->ctx, stmt->मूल्य, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE, "keycodes",
                      field, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        वापस !(info->keymap_info->strict & PARSER_NO_FIELD_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    वापस true;
}

अटल bool
HandleLedNameDef(कुंजी_Names_Info *info, LedNameDef *def, bool report)
{
    अगर (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_गिनती++;
        log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        वापस false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    अगर (!व्यंजक_Resolve_String(info->ctx, def->name, &name)) {
        अक्षर buf[20];
        snprintf(buf, माप(buf), "%"PRId64, def->ndx);
        info->error_गिनती++;
        वापस ReportBadType(info->ctx, XKB_ERROR_WRONG_FIELD_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    वापस AddLedName(info, true, &ledi, (xkb_led_सूचकांक_t) def->ndx - 1, report);
}

अटल व्योम
HandleKeycodesFile(कुंजी_Names_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    स्थिर पूर्णांक verbosity = xkb_context_get_log_verbosity(info->ctx);
    स्थिर bool report_same_file = verbosity > 0;
    स्थिर bool report_include   = verbosity > 7;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    क्रम (ParseCommon *stmt = file->defs; stmt; stmt = stmt->अग्र) {
        चयन (stmt->type) {
        हाल STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            अवरोध;
        हाल STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            अवरोध;
        हाल STMT_ALIAS:
            ok = HandleAliasDef(info, (कुंजी_Alias_Def *) stmt, report_same_file);
            अवरोध;
        हाल STMT_VAR:
            ok = Handle_कुंजी_Name_Var(info, (VarDef *) stmt);
            अवरोध;
        हाल STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            अवरोध;
        हाल STMT_UNKNOWN_DECLARATION:
        हाल STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            अवरोध;
        शेष:
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_to_string(stmt->type));
            ok = false;
            अवरोध;
        }

        अगर (!ok)
            info->error_गिनती++;

        अगर (info->error_गिनती > 10) {
            log_err(info->ctx, XKB_LOG_MESSAGE_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            अवरोध;
        }
    }
}

/***====================================================================***/

अटल bool
Copy_कुंजी_Names_To_Keymap(काष्ठा xkb_keymap *keymap, कुंजी_Names_Info *info)
{
    अगर (darray_empty(info->keycodes.निचला) && darray_empty(info->keycodes.ऊपरी)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        निश्चित(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_कुंजी_code = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_कुंजी_code = 255;
        keymap->अंक_कुंजियाँ = keymap->अंक_कुंजियाँ_निचला = keymap->max_कुंजी_code + 1;
    } अन्यथा {
        निश्चित(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_कुंजी_code = info->keycodes.min;
        keymap->max_कुंजी_code = (darray_empty(info->keycodes.ऊपरी))
            ? darray_आकार(info->keycodes.निचला) - 1
            : darray_item(
                info->keycodes.ऊपरी,
                darray_आकार(info->keycodes.ऊपरी) - 1
            ).keycode;
        keymap->अंक_कुंजियाँ_निचला = darray_आकार(info->keycodes.निचला);
        keymap->अंक_कुंजियाँ = keymap->अंक_कुंजियाँ_निचला
                         + darray_आकार(info->keycodes.ऊपरी);
    }

    काष्ठा xkb_कुंजी * स्थिर कुंजियाँ = सुस्मृति(keymap->अंक_कुंजियाँ, माप(*कुंजियाँ));
    अगर (!कुंजियाँ) {
        keymap->अंक_कुंजियाँ = 0;
        keymap->min_कुंजी_code = keymap->max_कुंजी_code = XKB_KEYCODE_INVALID;
        वापस false;
    }

    /* Low keycodes */
    क्रम (xkb_keycode_t kc = keymap->min_कुंजी_code; kc < keymap->अंक_कुंजियाँ_निचला; kc++)
        कुंजियाँ[kc].keycode = kc;

    क्रम (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_आकार(info->keycodes.निचला);
         kc++)
        कुंजियाँ[kc].name = darray_item(info->keycodes.निचला, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->अंक_कुंजियाँ_निचला; /* First high keycode index */
    स्थिर ऊपरी_Keycode_Entry *entry;
    darray_foreach(entry, info->keycodes.ऊपरी) {
        निश्चित(entry->name != XKB_ATOM_NONE);
        कुंजियाँ[idx].keycode = entry->keycode;
        कुंजियाँ[idx].name = entry->name;
        idx++;
    }

    keymap->कुंजियाँ = कुंजियाँ;
    वापस true;
}

अटल bool
CopyKeycodeNameLUT(काष्ठा xkb_keymap *keymap, कुंजी_Names_Info *info)
{
    KeycodeMatch *match;
    xkb_atom_t name;
    darray_enumerate(name, match, info->keycodes.names) {
        अगर (!match->found)
            जारी;

        अगर (match->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            स्थिर KeycodeMatch match_real =
                keycode_store_lookup_name(&info->keycodes, match->alias.real);
            अगर (!match_real.found) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        कुंजी_Name_Text(info->ctx, name),
                        कुंजी_Name_Text(info->ctx, match->alias.real));
                match->found = false;
                जारी;
            }
            निश्चित(!match_real.is_alias);
        } अन्यथा अगर (!match->कुंजी.निचला) {
            /* Update to final index in keymap::keys */
            match->कुंजी.सूचकांक += keymap->अंक_कुंजियाँ_निचला;
        }
    }

    darray_shrink(info->keycodes.names);
    keymap->अंक_कुंजी_names = darray_आकार(info->keycodes.names);
    darray_steal(info->keycodes.names, &keymap->कुंजी_names, शून्य);
    darray_init(info->keycodes.names);
    वापस true;
}

अटल bool
CopyLedNamesToKeymap(काष्ठा xkb_keymap *keymap, कुंजी_Names_Info *info)
{
    keymap->अंक_leds = info->अंक_led_names;
    क्रम (xkb_led_सूचकांक_t idx = 0; idx < info->अंक_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        अगर (ledi->name == XKB_ATOM_NONE)
            जारी;

        keymap->leds[idx].name = ledi->name;
    }

    वापस true;
}

अटल bool
Copy_कुंजी_Names_Info_To_Keymap(काष्ठा xkb_keymap *keymap, कुंजी_Names_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    अगर (!Copy_कुंजी_Names_To_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !CopyLedNamesToKeymap(keymap, info))
        वापस false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    अगर (!keymap->अंक_कुंजियाँ ||
        keymap->min_कुंजी_code > 0) {
        keymap->redirect_कुंजी_auto = 0;
    } अन्यथा {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        क्रम (xkb_keycode_t k = keymap->अंक_कुंजियाँ; k-- > keymap->अंक_कुंजियाँ_निचला;) {
            अगर (keycode > keymap->कुंजियाँ[k].keycode)
                अवरोध;
            /* Try next keycode downwards */
            keycode = keymap->कुंजियाँ[k].keycode - 1;
        }
        keymap->redirect_कुंजी_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    वापस true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, काष्ठा xkb_keymap_info *keymap_info)
{
    कुंजी_Names_Info info;

    Init_कुंजी_Names_Info(&info, keymap_info, 0);

    अगर (file != शून्य)
        HandleKeycodesFile(&info, file);

    अगर (info.error_गिनती != 0)
        जाओ err_info;

    अगर (!Copy_कुंजी_Names_Info_To_Keymap(&keymap_info->keymap, &info))
        जाओ err_info;

    Clear_कुंजी_Names_Info(&info);
    वापस true;

err_info:
    Clear_कुंजी_Names_Info(&info);
    वापस false;
}
