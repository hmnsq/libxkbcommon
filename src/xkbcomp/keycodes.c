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
} उच्चः_Keycode_प्रविष्टिः;

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
    darray(उच्चः_Keycode_प्रविष्टिः) उच्चः;
    /**
     * name -> keycode mapping
     */
    darray(Keycode_मेलनम्) नामानि;
} KeycodeStore;

स्थिर अन्तःस्थ शून्य
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->नीचः);
    darray_init(store->उच्चः);
    darray_init(store->नामानि);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

स्थिर अन्तःस्थ शून्य
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->नीचः);
    darray_free(store->उच्चः);
    darray_free(store->नामानि);
}

स्थिर अन्तःस्थ शून्य
keycode_store_update_कुञ्चिका(KeycodeStore *store, Keycode_मेलनम् मेलनम्, xkb_atom_t name)
{
    यदि (unlikely(!मेलनम्.प्राप्तम् || मेलनम्.is_alias)) {
        प्रत्यागम;
    } अन्यथा यदि (मेलनम्.कुञ्चिका.नीचः) {
        /* assume valid index */
        प्रतिज्ञा(मेलनम्.कुञ्चिका.सूचकांक < darray_आकार(store->नीचः));
        darray_item(store->नीचः, मेलनम्.कुञ्चिका.सूचकांक) = name;
    } अन्यथा {
        /* assume valid index */
        प्रतिज्ञा(मेलनम्.कुञ्चिका.सूचकांक < darray_आकार(store->उच्चः));
        darray_item(store->उच्चः, मेलनम्.कुञ्चिका.सूचकांक).name = name;
    }

    /* Names lookup */
    यदि (name >= darray_आकार(store->नामानि)) {
        darray_resize0(store->नामानि, name + 1);
    }
    darray_item(store->नामानि, name) = मेलनम्;
}

स्थिर bool
keycode_store_निवेशय_कुञ्चिका(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    यदि (name >= darray_आकार(store->नामानि)) {
        darray_resize0(store->नामानि, name + 1);
    }
    यदि (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        यदि (kc >= (xkb_keycode_t) darray_आकार(store->नीचः))
            darray_resize0(store->नीचः, kc + 1);
        darray_item(store->नीचः, kc) = name;
        यदि (kc < store->min)
            store->min = kc;
        darray_item(store->नामानि, name) = (Keycode_मेलनम्) {
            .कुञ्चिका = {
                .प्राप्तम् = true,
                .नीचः = true,
                .is_alias = false,
                .सूचकांक = kc
            }
        };
    } अन्यथा {
        /* High keycode: insert into a sorted list */
        ध्रुव darray_आकार_t idx = darray_आकार(store->उच्चः);
        यदि (idx && darray_item(store->उच्चः, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_आकार_t अधस्तनम् = 0;
            darray_आकार_t ऊर्ध्वस्थम् = idx;
            यावत् (अधस्तनम् < ऊर्ध्वस्थम्) {
                ध्रुव darray_आकार_t mid = अधस्तनम् + (ऊर्ध्वस्थम् - 1 - अधस्तनम्) / 2;
                ध्रुव उच्चः_Keycode_प्रविष्टिः * ध्रुव प्रविष्टिः =
                    &darray_item(store->उच्चः, mid);
                यदि (प्रविष्टिः->keycode < kc) {
                    अधस्तनम् = mid + 1;
                } अन्यथा यदि (प्रविष्टिः->keycode > kc) {
                    ऊर्ध्वस्थम् = mid;
                } अन्यथा {
                    /* Unreachable: there is no repetition in the list */
                    प्रतिज्ञा(प्रविष्टिः->keycode != kc);
                }
            }
            प्रतिज्ञा(अधस्तनम् < idx);
            प्रतिज्ञा(darray_item(store->उच्चः, अधस्तनम्).keycode > kc);

            /* Update references to entries that will be moved */
            उच्चः_Keycode_प्रविष्टिः *प्रविष्टिः;
            darray_foreach_from(प्रविष्टिः, store->उच्चः, अधस्तनम्) {
                darray_item(store->नामानि, प्रविष्टिः->name).कुञ्चिका.सूचकांक++;
            }

            darray_निवेशय(store->उच्चः, अधस्तनम्,
                          (उच्चः_Keycode_प्रविष्टिः){.keycode = kc, .name = name});
            darray_item(store->नामानि, name) = (Keycode_मेलनम्) {
                .कुञ्चिका = {
                    .प्राप्तम् = true,
                    .नीचः = false,
                    .is_alias = false,
                    .सूचकांक = अधस्तनम्
                }
            };
        } अन्यथा {
            /* Fast path: no need to sort */
            darray_append(store->उच्चः,
                          (उच्चः_Keycode_प्रविष्टिः){.keycode = kc, .name = name});
            darray_item(store->नामानि, name) = (Keycode_मेलनम्) {
                .कुञ्चिका = {
                    .प्राप्तम् = true,
                    .नीचः = false,
                    .is_alias = false,
                    .सूचकांक = idx
                }
            };
        }
        यदि (darray_रिक्तानि(store->नीचः))
            store->min = darray_item(store->उच्चः, 0).keycode;
    }
    प्रत्यागम true;
}

स्थिर अन्तःस्थ bool
keycode_store_निवेशय_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    यदि (alias >= darray_आकार(store->नामानि)) {
        darray_resize0(store->नामानि, alias + 1);
    }
    darray_item(store->नामानि, alias) = (Keycode_मेलनम्) {
        .alias = {
            .प्राप्तम् = true,
            .is_alias = true,
            .real = real
        }
    };
    प्रत्यागम true;
}

स्थिर अन्तःस्थ bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_item(store->नामानि, alias).alias.real = real;
    प्रत्यागम true;
}

स्थिर अन्तःस्थ शून्य
keycode_store_delete_name(ध्रुव KeycodeStore *store, xkb_atom_t name)
{
    यदि (name < darray_आकार(store->नामानि))
        darray_item(store->नामानि, name).प्राप्तम् = false;
}

स्थिर शून्य
keycode_store_delete_कुञ्चिका(KeycodeStore *store, ध्रुव Keycode_मेलनम् मेलनम्)
{
    यदि (unlikely(!मेलनम्.प्राप्तम् || मेलनम्.is_alias)) {
        प्रत्यागम;
    } अन्यथा यदि (मेलनम्.कुञ्चिका.नीचः) {
        /* assume valid index */
        प्रतिज्ञा(मेलनम्.कुञ्चिका.सूचकांक < darray_आकार(store->नीचः));
        darray_item(store->नामानि,
                    darray_item(store->नीचः, मेलनम्.कुञ्चिका.सूचकांक)).प्राप्तम् = false;
        यदि (मेलनम्.कुञ्चिका.सूचकांक + 1u == darray_आकार(store->नीचः)) {
            /* Highest low keycode: shrink */
            यदि (store->min == मेलनम्.कुञ्चिका.सूचकांक) {
                /* No low keycode left */
                darray_आकार(store->नीचः) = 0;
            } अन्यथा {
                /* Look for previous defined low keycode */
                प्रति (darray_आकार_t idx = मेलनम्.कुञ्चिका.सूचकांक; idx > 0; idx--) {
                    यदि (darray_item(store->नीचः, idx - 1) != XKB_ATOM_NONE) {
                        darray_आकार(store->नीचः) = idx;
                        भङ्ग;
                    }
                }
            }
        } अन्यथा {
            /* Lower keycode: reset */
            darray_item(store->नीचः, मेलनम्.कुञ्चिका.सूचकांक) = XKB_ATOM_NONE;
        }
    } अन्यथा {
        प्रतिज्ञा(मेलनम्.कुञ्चिका.सूचकांक < darray_आकार(store->उच्चः)); /* assume valid index */
        darray_item(store->नामानि,
                    darray_item(store->उच्चः, मेलनम्.कुञ्चिका.सूचकांक).name).प्राप्तम् = false;
        darray_remove(store->उच्चः, मेलनम्.कुञ्चिका.सूचकांक);
        /* Update LUT indexes of high codes after the deleted one, if any */
        Keycode_मेलनम् *प्रविष्टिः;
        darray_foreach(प्रविष्टिः, store->नामानि) {
            यदि (प्रविष्टिः->प्राप्तम् && !प्रविष्टिः->is_alias && !प्रविष्टिः->कुञ्चिका.नीचः &&
                प्रविष्टिः->कुञ्चिका.सूचकांक > मेलनम्.कुञ्चिका.सूचकांक) {
                प्रविष्टिः->कुञ्चिका.सूचकांक--;
            }
        }
    }

    /* Update bounds */
    यदि (darray_रिक्तानि(store->नीचः)) {
        store->min = (darray_रिक्तानि(store->उच्चः))
            ? XKB_KEYCODE_INVALID
            : darray_item(store->उच्चः, 0).keycode;
    } अन्यथा {
        प्रति (xkb_keycode_t kc = store->min; kc < darray_आकार(store->नीचः); kc++) {
            यदि (darray_item(store->नीचः, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                भङ्ग;
            }
        }
    }
}

स्थिर अन्तःस्थ xkb_keycode_t
keycode_store_get_keycode(ध्रुव KeycodeStore *store, Keycode_मेलनम् मेलनम्)
{
    यदि (!मेलनम्.प्राप्तम् || मेलनम्.is_alias) {
        प्रत्यागम XKB_KEYCODE_INVALID;
    } अन्यथा यदि (मेलनम्.कुञ्चिका.नीचः) {
        /* assume valid index */
        प्रतिज्ञा(मेलनम्.कुञ्चिका.सूचकांक < darray_आकार(store->नीचः));
        प्रत्यागम (xkb_keycode_t) मेलनम्.कुञ्चिका.सूचकांक;
    } अन्यथा {
        /* assume valid index */
        प्रतिज्ञा(मेलनम्.कुञ्चिका.सूचकांक < darray_आकार(store->उच्चः));
        प्रत्यागम darray_item(store->उच्चः, मेलनम्.कुञ्चिका.सूचकांक).keycode;
    }
}

स्थिर अन्तःस्थ xkb_atom_t
keycode_store_get_कुञ्चिका_name(ध्रुव KeycodeStore *store, Keycode_मेलनम् मेलनम्)
{
    यदि (!मेलनम्.प्राप्तम् || मेलनम्.is_alias) {
        प्रत्यागम XKB_ATOM_NONE;
    } अन्यथा यदि (मेलनम्.कुञ्चिका.नीचः) {
        /* assume valid index */
        प्रतिज्ञा(मेलनम्.कुञ्चिका.सूचकांक < darray_आकार(store->नीचः));
        प्रत्यागम darray_item(store->नीचः, मेलनम्.कुञ्चिका.सूचकांक);
    } अन्यथा {
        /* assume valid index */
        प्रतिज्ञा(मेलनम्.कुञ्चिका.सूचकांक < darray_आकार(store->उच्चः));
        प्रत्यागम darray_item(store->उच्चः, मेलनम्.कुञ्चिका.सूचकांक).name;
    }
}

स्थिर Keycode_मेलनम्
keycode_store_lookup_keycode(ध्रुव KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    यदि (kc < (xkb_keycode_t) darray_आकार(store->नीचः)) {
        प्रत्यागम (Keycode_मेलनम्) {
            .कुञ्चिका = {
                .प्राप्तम् = true,
                .नीचः = true,
                .is_alias = false,
                .सूचकांक = kc
            }
        };
    } अन्यथा यदि (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        प्रत्यागम (Keycode_मेलनम्) { .प्राप्तम् = false };
    }

    /* High keycodes: use binary search */
    darray_आकार_t अधस्तनम् = 0;
    darray_आकार_t ऊर्ध्वस्थम् = darray_आकार(store->उच्चः);
    यावत् (अधस्तनम् < ऊर्ध्वस्थम्) {
        ध्रुव darray_आकार_t mid = अधस्तनम् + (ऊर्ध्वस्थम् - 1 - अधस्तनम्) / 2;
        उच्चः_Keycode_प्रविष्टिः * ध्रुव प्रविष्टिः = &darray_item(store->उच्चः, mid);
        यदि (प्रविष्टिः->keycode < kc) {
            अधस्तनम् = mid + 1;
        } अन्यथा यदि (प्रविष्टिः->keycode > kc) {
            ऊर्ध्वस्थम् = mid;
        } अन्यथा {
            प्रत्यागम (Keycode_मेलनम्) {
                .कुञ्चिका = {
                    .प्राप्तम् = true,
                    .नीचः = false,
                    .is_alias = false,
                    .सूचकांक = mid
                }
            };
        }
    }

    प्रत्यागम (Keycode_मेलनम्) { .प्राप्तम् = false };
}

स्थिर Keycode_मेलनम्
keycode_store_lookup_name(ध्रुव KeycodeStore *store, xkb_atom_t name)
{
    यदि (name >= darray_आकार(store->नामानि)) {
        प्रत्यागम (Keycode_मेलनम्) { .प्राप्तम् = false, .is_alias = false };
    } अन्यथा {
        प्रत्यागम darray_item(store->नामानि, name);
    }
}

/***====================================================================***/

प्रकारनिर्देश संरचना {
    गणना merge_बहुलकम् merge;

    xkb_atom_t name;
} LedNameInfo;

प्रकारनिर्देश संरचना {
    वर्ण *name;
    पूर्णाङ्क error_गणनम्;
    अचिह्नित पूर्णाङ्क include_गभीरता;

    KeycodeStore keycodes;
    LedNameInfo led_नामानि[XKB_MAX_LEDS];
    xkb_led_सूचकांक_t अङ्कः_led_नामानि;

    संरचना xkb_context *ctx;
    ध्रुव संरचना xkb_keymap_info *keymap_info;
} कुञ्चिका_नामानि_Info;

/***====================================================================***/

स्थिर LedNameInfo *
FindLedByName(कुञ्चिका_नामानि_Info *info, xkb_atom_t name,
              xkb_led_सूचकांक_t *idx_बहिर्गमनम्)
{
    प्रति (xkb_led_सूचकांक_t idx = 0; idx < info->अङ्कः_led_नामानि; idx++) {
        LedNameInfo *ledi = &info->led_नामानि[idx];
        यदि (ledi->name == name) {
            *idx_बहिर्गमनम् = idx;
            प्रत्यागम ledi;
        }
    }

    प्रत्यागम रिक्त;
}

स्थिर bool
सङ्कलय_Led_Name(कुञ्चिका_नामानि_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_सूचकांक_t new_idx, bool report)
{
    xkb_led_सूचकांक_t old_idx;
    LedNameInfo *old;
    ध्रुव bool replace = (new->merge != MERGE_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = FindLedByName(info, new->name, &old_idx);
    यदि (old) {
        यदि (old_idx == new_idx) {
            यदि (report)
                log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_text(info->ctx, new->name));
            प्रत्यागम true;
        }

        यदि (report) {
            xkb_led_सूचकांक_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_सूचकांक_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
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

    यदि (new_idx >= info->अङ्कः_led_नामानि)
        info->अङ्कः_led_नामानि = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_नामानि[new_idx];
    यदि (old->name != XKB_ATOM_NONE) {
        यदि (report) {
            ध्रुव xkb_atom_t use    = (replace ? new->name : old->name);
            ध्रुव xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
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
Clear_कुञ्चिका_नामानि_Info(कुञ्चिका_नामानि_Info *info)
{
    मुक्त(info->name);
    keycode_store_free(&info->keycodes);
}

स्थिर शून्य
Init_कुञ्चिका_नामानि_Info(कुञ्चिका_नामानि_Info *info, ध्रुव संरचना xkb_keymap_info *keymap_info,
                 अचिह्नित पूर्णाङ्क include_गभीरता)
{
    स्मृतिं_पूरय(info, 0, परिमाण(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_गभीरता = include_गभीरता;
    keycode_store_init(&info->keycodes);
}

स्थिर bool
सङ्कलय_कुञ्चिका_Name(कुञ्चिका_नामानि_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           गणना merge_बहुलकम् merge, bool report)
{
#यदि_परिभाषितम् NDEBUG
    ध्रुव
#यदि_अन्तः
    Keycode_मेलनम् मेलनम्_name = keycode_store_lookup_name(&info->keycodes, name);
    यदि (मेलनम्_name.प्राप्तम्) {
        ध्रुव bool clobber = (merge != MERGE_AUGMENT);

        यदि (मेलनम्_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            यदि (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         कुञ्चिका_Name_Text(info->ctx, name),
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
#यदि_अपरिभाषितम् NDEBUG
                मेलनम्_name.प्राप्तम् = false;
#यदि_अन्तः
            } अन्यथा {
                प्रत्यागम true;
            }
        } अन्यथा {
            ध्रुव xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, मेलनम्_name);
            प्रतिज्ञा(old_kc != XKB_KEYCODE_INVALID);
            यदि (old_kc != kc) {
                /* There is already a different key with this name. */

                यदि (report) {
                    ध्रुव xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ध्रुव xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            कुञ्चिका_Name_Text(info->ctx, name), use, ignore);
                }

                यदि (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_कुञ्चिका(&info->keycodes, मेलनम्_name);
                } अन्यथा {
                    प्रत्यागम true;
                }
            }
        }
    }

    ध्रुव Keycode_मेलनम् मेलनम्_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ध्रुव xkb_atom_t old_name = keycode_store_get_कुञ्चिका_name(&info->keycodes, मेलनम्_kc);
    यदि (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        यदि (old_name == name) {
            प्रतिज्ञा(keycode_store_get_keycode(&info->keycodes, मेलनम्_name) == kc);
            यदि (report)
                log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         कुञ्चिका_Name_Text(info->ctx, old_name), kc);
            प्रत्यागम true;
        }

        ध्रुव bool clobber = (merge != MERGE_AUGMENT);
        यदि (report) {
            ध्रुव वर्ण* ध्रुव kname     = कुञ्चिका_Name_Text(info->ctx, name);
            ध्रुव वर्ण* ध्रुव old_kname = कुञ्चिका_Name_Text(info->ctx, old_name);
            ध्रुव वर्ण* ध्रुव use    = (clobber) ? kname : old_kname;
            ध्रुव वर्ण* ध्रुव ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        यदि (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_कुञ्चिका(&info->keycodes, मेलनम्_kc, name);
        }
    } अन्यथा {
        /* No previous keycode */
        यदि (!keycode_store_निवेशय_कुञ्चिका(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
                    "Cannot add keycode\n");
            प्रत्यागम false;
        }
    }

    प्रत्यागम true;
}

/***====================================================================***/

स्थिर bool
HandleAliasDef(कुञ्चिका_नामानि_Info *info, ध्रुव कुञ्चिका_Alias_Def *def, bool report);

स्थिर शून्य
MergeKeycodeStores(कुञ्चिका_नामानि_Info *into, कुञ्चिका_नामानि_Info *from,
                   गणना merge_बहुलकम् merge, bool report)
{
    यदि (darray_रिक्तानि(into->keycodes.नीचः) && darray_रिक्तानि(into->keycodes.उच्चः) &&
        darray_रिक्तानि(into->keycodes.नामानि)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.नीचः);
        darray_init(from->keycodes.उच्चः);
        darray_init(from->keycodes.नामानि);
    } अन्यथा {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        प्रति (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_आकार(from->keycodes.नीचः);
             kc++) {
            /* Skip undefined keycodes */
            ध्रुव xkb_atom_t name = darray_item(from->keycodes.नीचः, kc);
            यदि (name == XKB_ATOM_NONE)
                अनुवर्त;

            यदि (!सङ्कलय_कुञ्चिका_Name(into, kc, name, merge, report))
                into->error_गणनम्++;
        }

        /* High keycodes */
        ध्रुव उच्चः_Keycode_प्रविष्टिः *new;
        darray_foreach(new, from->keycodes.उच्चः) {
            प्रतिज्ञा(new->name != XKB_ATOM_NONE);
            यदि (!सङ्कलय_कुञ्चिका_Name(into, new->keycode, new->name, merge, report))
                into->error_गणनम्++;
        }

        /* Aliases. */
        Keycode_मेलनम् *मेलनम्;
        xkb_atom_t alias;
        darray_enumerate(alias, मेलनम्, from->keycodes.नामानि) {
            यदि (!मेलनम्->प्राप्तम् || !मेलनम्->is_alias)
                अनुवर्त;

            ध्रुव कुञ्चिका_Alias_Def def = {
                .merge = merge,
                .alias = alias,
                .real = मेलनम्->alias.real
            };
            यदि (!HandleAliasDef(into, &def, report))
                into->error_गणनम्++;
        }
    }
}

स्थिर शून्य
MergeIncludedKeycodes(कुञ्चिका_नामानि_Info *into, कुञ्चिका_नामानि_Info *from,
                      गणना merge_बहुलकम् merge, bool report)
{
    यदि (from->error_गणनम् > 0) {
        into->error_गणनम् += from->error_गणनम्;
        प्रत्यागम;
    }

    यदि (into->name == रिक्त) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    MergeKeycodeStores(into, from, merge, report);

    /* Merge LED names. */
    यदि (into->अङ्कः_led_नामानि == 0) {
        स्मृतिं_प्रतिलिख(into->led_नामानि, from->led_नामानि,
               परिमाण(*from->led_नामानि) * from->अङ्कः_led_नामानि);
        into->अङ्कः_led_नामानि = from->अङ्कः_led_नामानि;
        from->अङ्कः_led_नामानि = 0;
    }
    अन्यथा {
        प्रति (xkb_led_सूचकांक_t idx = 0; idx < from->अङ्कः_led_नामानि; idx++) {
            LedNameInfo *ledi = &from->led_नामानि[idx];

            यदि (ledi->name == XKB_ATOM_NONE)
                अनुवर्त;

            ledi->merge = merge;
            यदि (!सङ्कलय_Led_Name(into, false, ledi, idx, report))
                into->error_गणनम्++;
        }
    }
}

स्थिर शून्य
HandleKeycodesFile(कुञ्चिका_नामानि_Info *info, XkbFile *file);

स्थिर bool
HandleIncludeKeycodes(कुञ्चिका_नामानि_Info *info, IncludeStmt *include, bool report)
{
    कुञ्चिका_नामानि_Info included;

    यदि (Exceeds_Include_Max_गभीरता(info->ctx, info->include_गभीरता)) {
        info->error_गणनम् += 10;
        प्रत्यागम false;
    }

    Init_कुञ्चिका_नामानि_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ध्रुव संरचना parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    प्रति (IncludeStmt *stmt = include; stmt; stmt = stmt->अग्र_incl) {
        कुञ्चिका_नामानि_Info अग्र_incl;
        XkbFile *file;

        वर्ण मार्गः[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  मार्गः, परिमाण(मार्गः));
        यदि (!file) {
            info->error_गणनम् += 10;
            Clear_कुञ्चिका_नामानि_Info(&included);
            प्रत्यागम false;
        }

        Init_कुञ्चिका_नामानि_Info(&अग्र_incl, info->keymap_info, info->include_गभीरता + 1);

        HandleKeycodesFile(&अग्र_incl, file);

        MergeIncludedKeycodes(&included, &अग्र_incl, stmt->merge, report);

        Clear_कुञ्चिका_नामानि_Info(&अग्र_incl);
        FreeXkbFile(file);
    }

    MergeIncludedKeycodes(info, &included, include->merge, report);
    Clear_कुञ्चिका_नामानि_Info(&included);

    प्रत्यागम (info->error_गणनम् == 0);
}

स्थिर bool
HandleKeycodeDef(कुञ्चिका_नामानि_Info *info, KeycodeDef *stmt, bool report)
{
    यदि (stmt->मानम् < 0 || stmt->मानम् > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->मानम्, XKB_KEYCODE_MAX);
        प्रत्यागम false;
    }

    प्रत्यागम सङ्कलय_कुञ्चिका_Name(info, (xkb_keycode_t) stmt->मानम्,
                      stmt->name, stmt->merge, report);
}

स्थिर bool
HandleAliasDef(कुञ्चिका_नामानि_Info *info, ध्रुव कुञ्चिका_Alias_Def *def, bool report)
{
    ध्रुव Keycode_मेलनम् मेलनम्_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    यदि (मेलनम्_name.प्राप्तम्) {
        ध्रुव bool clobber = (def->merge != MERGE_AUGMENT);
        यदि (मेलनम्_name.is_alias) {
            यदि (def->real == मेलनम्_name.alias.real) {
                यदि (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             कुञ्चिका_Name_Text(info->ctx, def->alias),
                             कुञ्चिका_Name_Text(info->ctx, def->real));
            } अन्यथा {
                ध्रुव xkb_atom_t use = (clobber)
                    ? def->real
                    : मेलनम्_name.alias.real;
                ध्रुव xkb_atom_t ignore = (clobber)
                    ? मेलनम्_name.alias.real
                    : def->real;

                यदि (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             कुञ्चिका_Name_Text(info->ctx, def->alias),
                             कुञ्चिका_Name_Text(info->ctx, use),
                             कुञ्चिका_Name_Text(info->ctx, ignore));

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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_कुञ्चिका_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         कुञ्चिका_Name_Text(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            यदि (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_कुञ्चिका(&info->keycodes, मेलनम्_name);
            } अन्यथा {
                प्रत्यागम true;
            }
        }
    }

    प्रत्यागम keycode_store_निवेशय_alias(&info->keycodes, def->alias, def->real);
}

स्थिर bool
Handle_कुञ्चिका_Name_Var(कुञ्चिका_नामानि_Info *info, VarDef *stmt)
{
    ध्रुव वर्ण *elem, *क्षेत्रम्_2;
    व्यञ्जकम्_Def *arrayNdx;

    यदि (!व्यञ्जकम्_Resolve_Lhs(info->ctx, stmt->name, &elem, &क्षेत्रम्_2, &arrayNdx))
        प्रत्यागम false;

    यदि (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, क्षेत्रम्_2);
        प्रत्यागम !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    यदि (!istreq(क्षेत्रम्_2, "minimum") && !istreq(क्षेत्रम्_2, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_क्षेत्रम्,
                "Default defined for unknown field \"%s\"; Ignored\n", क्षेत्रम्_2);
        प्रत्यागम !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    यदि (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", क्षेत्रम्_2, "defaults");
        प्रत्यागम !(info->keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH);
    }

    int64_t val = 0;
    यदि (!व्यञ्जकम्_Resolve_Integer(info->ctx, stmt->मानम्, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE, "keycodes",
                      क्षेत्रम्_2, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        प्रत्यागम !(info->keymap_info->strict & PARSER_NO_क्षेत्रम्_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    प्रत्यागम true;
}

स्थिर bool
HandleLedNameDef(कुञ्चिका_नामानि_Info *info, LedNameDef *def, bool report)
{
    यदि (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_गणनम्++;
        log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        प्रत्यागम false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    यदि (!व्यञ्जकम्_Resolve_String(info->ctx, def->name, &name)) {
        वर्ण buf[20];
        snprintf(buf, परिमाण(buf), "%"PRId64, def->ndx);
        info->error_गणनम्++;
        प्रत्यागम ReportBadType(info->ctx, XKB_ERROR_WRONG_क्षेत्रम्_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.merge = def->merge, .name = name};
    प्रत्यागम सङ्कलय_Led_Name(info, true, &ledi, (xkb_led_सूचकांक_t) def->ndx - 1, report);
}

स्थिर शून्य
HandleKeycodesFile(कुञ्चिका_नामानि_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ध्रुव पूर्णाङ्क verbosity = xkb_context_get_log_verbosity(info->ctx);
    ध्रुव bool report_same_file = verbosity > 0;
    ध्रुव bool report_include   = verbosity > 7;

    मुक्त(info->name);
    info->name = strdup_safe(file->name);

    प्रति (ParseCommon *stmt = file->defs; stmt; stmt = stmt->अग्र) {
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
            log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_गन्तव्यम्_string(stmt->type));
            ok = false;
            भङ्ग;
        }

        यदि (!ok)
            info->error_गणनम्++;

        यदि (info->error_गणनम् > 10) {
            log_err(info->ctx, XKB_LOG_सन्देशः_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    safe_map_name(file->name));
            भङ्ग;
        }
    }
}

/***====================================================================***/

स्थिर bool
Copy_कुञ्चिका_नामानि_गन्तव्यम्_Keymap(संरचना xkb_keymap *keymap, कुञ्चिका_नामानि_Info *info)
{
    यदि (darray_रिक्तानि(info->keycodes.नीचः) && darray_रिक्तानि(info->keycodes.उच्चः)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        प्रतिज्ञा(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_कुञ्चिका_संहिता = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_कुञ्चिका_संहिता = 255;
        keymap->अङ्कः_कुञ्चिकाः = keymap->अङ्कः_कुञ्चिकाः_नीचः = keymap->max_कुञ्चिका_संहिता + 1;
    } अन्यथा {
        प्रतिज्ञा(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_कुञ्चिका_संहिता = info->keycodes.min;
        keymap->max_कुञ्चिका_संहिता = (darray_रिक्तानि(info->keycodes.उच्चः))
            ? darray_आकार(info->keycodes.नीचः) - 1
            : darray_item(
                info->keycodes.उच्चः,
                darray_आकार(info->keycodes.उच्चः) - 1
            ).keycode;
        keymap->अङ्कः_कुञ्चिकाः_नीचः = darray_आकार(info->keycodes.नीचः);
        keymap->अङ्कः_कुञ्चिकाः = keymap->अङ्कः_कुञ्चिकाः_नीचः
                         + darray_आकार(info->keycodes.उच्चः);
    }

    संरचना xkb_कुञ्चिका * ध्रुव कुञ्चिकाः = शून्यस्मृतिं_देहि(keymap->अङ्कः_कुञ्चिकाः, परिमाण(*कुञ्चिकाः));
    यदि (!कुञ्चिकाः) {
        keymap->अङ्कः_कुञ्चिकाः = 0;
        keymap->min_कुञ्चिका_संहिता = keymap->max_कुञ्चिका_संहिता = XKB_KEYCODE_INVALID;
        प्रत्यागम false;
    }

    /* Low keycodes */
    प्रति (xkb_keycode_t kc = keymap->min_कुञ्चिका_संहिता; kc < keymap->अङ्कः_कुञ्चिकाः_नीचः; kc++)
        कुञ्चिकाः[kc].keycode = kc;

    प्रति (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_आकार(info->keycodes.नीचः);
         kc++)
        कुञ्चिकाः[kc].name = darray_item(info->keycodes.नीचः, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->अङ्कः_कुञ्चिकाः_नीचः; /* First high keycode index */
    ध्रुव उच्चः_Keycode_प्रविष्टिः *प्रविष्टिः;
    darray_foreach(प्रविष्टिः, info->keycodes.उच्चः) {
        प्रतिज्ञा(प्रविष्टिः->name != XKB_ATOM_NONE);
        कुञ्चिकाः[idx].keycode = प्रविष्टिः->keycode;
        कुञ्चिकाः[idx].name = प्रविष्टिः->name;
        idx++;
    }

    keymap->कुञ्चिकाः = कुञ्चिकाः;
    प्रत्यागम true;
}

स्थिर bool
CopyKeycodeNameLUT(संरचना xkb_keymap *keymap, कुञ्चिका_नामानि_Info *info)
{
    Keycode_मेलनम् *मेलनम्;
    xkb_atom_t name;
    darray_enumerate(name, मेलनम्, info->keycodes.नामानि) {
        यदि (!मेलनम्->प्राप्तम्)
            अनुवर्त;

        यदि (मेलनम्->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ध्रुव Keycode_मेलनम् मेलनम्_real =
                keycode_store_lookup_name(&info->keycodes, मेलनम्->alias.real);
            यदि (!मेलनम्_real.प्राप्तम्) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        कुञ्चिका_Name_Text(info->ctx, name),
                        कुञ्चिका_Name_Text(info->ctx, मेलनम्->alias.real));
                मेलनम्->प्राप्तम् = false;
                अनुवर्त;
            }
            प्रतिज्ञा(!मेलनम्_real.is_alias);
        } अन्यथा यदि (!मेलनम्->कुञ्चिका.नीचः) {
            /* Update to final index in keymap::keys */
            मेलनम्->कुञ्चिका.सूचकांक += keymap->अङ्कः_कुञ्चिकाः_नीचः;
        }
    }

    darray_shrink(info->keycodes.नामानि);
    keymap->अङ्कः_कुञ्चिका_नामानि = darray_आकार(info->keycodes.नामानि);
    darray_steal(info->keycodes.नामानि, &keymap->कुञ्चिका_नामानि, रिक्त);
    darray_init(info->keycodes.नामानि);
    प्रत्यागम true;
}

स्थिर bool
Copy_Led_नामानि_गन्तव्यम्_Keymap(संरचना xkb_keymap *keymap, कुञ्चिका_नामानि_Info *info)
{
    keymap->अङ्कः_leds = info->अङ्कः_led_नामानि;
    प्रति (xkb_led_सूचकांक_t idx = 0; idx < info->अङ्कः_led_नामानि; idx++) {
        LedNameInfo *ledi = &info->led_नामानि[idx];

        यदि (ledi->name == XKB_ATOM_NONE)
            अनुवर्त;

        keymap->leds[idx].name = ledi->name;
    }

    प्रत्यागम true;
}

स्थिर bool
Copy_कुञ्चिका_नामानि_Info_गन्तव्यम्_Keymap(संरचना xkb_keymap *keymap, कुञ्चिका_नामानि_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    यदि (!Copy_कुञ्चिका_नामानि_गन्तव्यम्_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !Copy_Led_नामानि_गन्तव्यम्_Keymap(keymap, info))
        प्रत्यागम false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    यदि (!keymap->अङ्कः_कुञ्चिकाः ||
        keymap->min_कुञ्चिका_संहिता > 0) {
        keymap->redirect_कुञ्चिका_auto = 0;
    } अन्यथा {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        प्रति (xkb_keycode_t k = keymap->अङ्कः_कुञ्चिकाः; k-- > keymap->अङ्कः_कुञ्चिकाः_नीचः;) {
            यदि (keycode > keymap->कुञ्चिकाः[k].keycode)
                भङ्ग;
            /* Try next keycode downwards */
            keycode = keymap->कुञ्चिकाः[k].keycode - 1;
        }
        keymap->redirect_कुञ्चिका_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_safe(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    प्रत्यागम true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, संरचना xkb_keymap_info *keymap_info)
{
    कुञ्चिका_नामानि_Info info;

    Init_कुञ्चिका_नामानि_Info(&info, keymap_info, 0);

    यदि (file != रिक्त)
        HandleKeycodesFile(&info, file);

    यदि (info.error_गणनम् != 0)
        गच्छ err_info;

    यदि (!Copy_कुञ्चिका_नामानि_Info_गन्तव्यम्_Keymap(&keymap_info->keymap, &info))
        गच्छ err_info;

    Clear_कुञ्चिका_नामानि_Info(&info);
    प्रत्यागम true;

err_info:
    Clear_कुञ्चिका_नामानि_Info(&info);
    प्रत्यागम false;
}
