/*
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 * SPDX-License-Identifier: HPND
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <limits.h>
#شامل <stdint.h>
#شامل <stdlib.h>
#شامل <string.h>

#شامل "context.h"
#شامل "messages-codes.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "xkbcomp-priv.h"
#شامل "atom.h"
#شامل "darray.h"
#شامل "keymap.h"
#شامل "text.h"
#شامل "expr.h"
#شامل "include.h"
#شامل "util-mem.h"

قسمتعریف ساخت {
    xkb_keycode_t keycode;
    xkb_atom_t name;
} بلند_Keycode_اندراج;

قسمتعریف ساخت {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) کم;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(بلند_Keycode_اندراج) بلند;
    /**
     * name -> keycode mapping
     */
    darray(Keycode_مطابقت) نام;
} KeycodeStore;

ساکن اندرونی خالی
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->کم);
    darray_init(store->بلند);
    darray_init(store->نام);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ساکن اندرونی خالی
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->کم);
    darray_free(store->بلند);
    darray_free(store->نام);
}

ساکن اندرونی خالی
keycode_store_update_کلید(KeycodeStore *store, Keycode_مطابقت مطابقت, xkb_atom_t name)
{
    اگر (unlikely(!مطابقت.ملا || مطابقت.is_alias)) {
        واپس;
    } ورنہ اگر (مطابقت.کلید.کم) {
        /* assume valid index */
        أكد(مطابقت.کلید.اشاریہ < darray_سائز(store->کم));
        darray_شے(store->کم, مطابقت.کلید.اشاریہ) = name;
    } ورنہ {
        /* assume valid index */
        أكد(مطابقت.کلید.اشاریہ < darray_سائز(store->بلند));
        darray_شے(store->بلند, مطابقت.کلید.اشاریہ).name = name;
    }

    /* Names lookup */
    اگر (name >= darray_سائز(store->نام)) {
        darray_resize0(store->نام, name + 1);
    }
    darray_شے(store->نام, name) = مطابقت;
}

ساکن bool
keycode_store_داخل_کرو_کلید(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    اگر (name >= darray_سائز(store->نام)) {
        darray_resize0(store->نام, name + 1);
    }
    اگر (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        اگر (kc >= (xkb_keycode_t) darray_سائز(store->کم))
            darray_resize0(store->کم, kc + 1);
        darray_شے(store->کم, kc) = name;
        اگر (kc < store->min)
            store->min = kc;
        darray_شے(store->نام, name) = (Keycode_مطابقت) {
            .کلید = {
                .ملا = true,
                .کم = true,
                .is_alias = false,
                .اشاریہ = kc
            }
        };
    } ورنہ {
        /* High keycode: insert into a sorted list */
        ثابت darray_سائز_t idx = darray_سائز(store->بلند);
        اگر (idx && darray_شے(store->بلند, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_سائز_t زیریں = 0;
            darray_سائز_t بالائی = idx;
            جب (زیریں < بالائی) {
                ثابت darray_سائز_t mid = زیریں + (بالائی - 1 - زیریں) / 2;
                ثابت بلند_Keycode_اندراج * ثابت اندراج_2 =
                    &darray_شے(store->بلند, mid);
                اگر (اندراج_2->keycode < kc) {
                    زیریں = mid + 1;
                } ورنہ اگر (اندراج_2->keycode > kc) {
                    بالائی = mid;
                } ورنہ {
                    /* Unreachable: there is no repetition in the list */
                    أكد(اندراج_2->keycode != kc);
                }
            }
            أكد(زیریں < idx);
            أكد(darray_شے(store->بلند, زیریں).keycode > kc);

            /* Update references to entries that will be moved */
            بلند_Keycode_اندراج *اندراج_2;
            darray_foreach_from(اندراج_2, store->بلند, زیریں) {
                darray_شے(store->نام, اندراج_2->name).کلید.اشاریہ++;
            }

            darray_داخل_کرو(store->بلند, زیریں,
                          (بلند_Keycode_اندراج){.keycode = kc, .name = name});
            darray_شے(store->نام, name) = (Keycode_مطابقت) {
                .کلید = {
                    .ملا = true,
                    .کم = false,
                    .is_alias = false,
                    .اشاریہ = زیریں
                }
            };
        } ورنہ {
            /* Fast path: no need to sort */
            darray_append(store->بلند,
                          (بلند_Keycode_اندراج){.keycode = kc, .name = name});
            darray_شے(store->نام, name) = (Keycode_مطابقت) {
                .کلید = {
                    .ملا = true,
                    .کم = false,
                    .is_alias = false,
                    .اشاریہ = idx
                }
            };
        }
        اگر (darray_خالی_خانے(store->کم))
            store->min = darray_شے(store->بلند, 0).keycode;
    }
    واپس true;
}

ساکن اندرونی bool
keycode_store_داخل_کرو_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    اگر (alias >= darray_سائز(store->نام)) {
        darray_resize0(store->نام, alias + 1);
    }
    darray_شے(store->نام, alias) = (Keycode_مطابقت) {
        .alias = {
            .ملا = true,
            .is_alias = true,
            .real = real
        }
    };
    واپس true;
}

ساکن اندرونی bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_شے(store->نام, alias).alias.real = real;
    واپس true;
}

ساکن اندرونی خالی
keycode_store_delete_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    اگر (name < darray_سائز(store->نام))
        darray_شے(store->نام, name).ملا = false;
}

ساکن خالی
keycode_store_delete_کلید(KeycodeStore *store, ثابت Keycode_مطابقت مطابقت)
{
    اگر (unlikely(!مطابقت.ملا || مطابقت.is_alias)) {
        واپس;
    } ورنہ اگر (مطابقت.کلید.کم) {
        /* assume valid index */
        أكد(مطابقت.کلید.اشاریہ < darray_سائز(store->کم));
        darray_شے(store->نام,
                    darray_شے(store->کم, مطابقت.کلید.اشاریہ)).ملا = false;
        اگر (مطابقت.کلید.اشاریہ + 1u == darray_سائز(store->کم)) {
            /* Highest low keycode: shrink */
            اگر (store->min == مطابقت.کلید.اشاریہ) {
                /* No low keycode left */
                darray_سائز(store->کم) = 0;
            } ورنہ {
                /* Look for previous defined low keycode */
                برائے (darray_سائز_t idx = مطابقت.کلید.اشاریہ; idx > 0; idx--) {
                    اگر (darray_شے(store->کم, idx - 1) != XKB_ATOM_NONE) {
                        darray_سائز(store->کم) = idx;
                        توڑو;
                    }
                }
            }
        } ورنہ {
            /* Lower keycode: reset */
            darray_شے(store->کم, مطابقت.کلید.اشاریہ) = XKB_ATOM_NONE;
        }
    } ورنہ {
        أكد(مطابقت.کلید.اشاریہ < darray_سائز(store->بلند)); /* assume valid index */
        darray_شے(store->نام,
                    darray_شے(store->بلند, مطابقت.کلید.اشاریہ).name).ملا = false;
        darray_remove(store->بلند, مطابقت.کلید.اشاریہ);
        /* Update LUT indexes of high codes after the deleted one, if any */
        Keycode_مطابقت *اندراج_2;
        darray_foreach(اندراج_2, store->نام) {
            اگر (اندراج_2->ملا && !اندراج_2->is_alias && !اندراج_2->کلید.کم &&
                اندراج_2->کلید.اشاریہ > مطابقت.کلید.اشاریہ) {
                اندراج_2->کلید.اشاریہ--;
            }
        }
    }

    /* Update bounds */
    اگر (darray_خالی_خانے(store->کم)) {
        store->min = (darray_خالی_خانے(store->بلند))
            ? XKB_KEYCODE_INVALID
            : darray_شے(store->بلند, 0).keycode;
    } ورنہ {
        برائے (xkb_keycode_t kc = store->min; kc < darray_سائز(store->کم); kc++) {
            اگر (darray_شے(store->کم, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                توڑو;
            }
        }
    }
}

ساکن اندرونی xkb_keycode_t
keycode_store_get_keycode(ثابت KeycodeStore *store, Keycode_مطابقت مطابقت)
{
    اگر (!مطابقت.ملا || مطابقت.is_alias) {
        واپس XKB_KEYCODE_INVALID;
    } ورنہ اگر (مطابقت.کلید.کم) {
        /* assume valid index */
        أكد(مطابقت.کلید.اشاریہ < darray_سائز(store->کم));
        واپس (xkb_keycode_t) مطابقت.کلید.اشاریہ;
    } ورنہ {
        /* assume valid index */
        أكد(مطابقت.کلید.اشاریہ < darray_سائز(store->بلند));
        واپس darray_شے(store->بلند, مطابقت.کلید.اشاریہ).keycode;
    }
}

ساکن اندرونی xkb_atom_t
keycode_store_get_کلید_name(ثابت KeycodeStore *store, Keycode_مطابقت مطابقت)
{
    اگر (!مطابقت.ملا || مطابقت.is_alias) {
        واپس XKB_ATOM_NONE;
    } ورنہ اگر (مطابقت.کلید.کم) {
        /* assume valid index */
        أكد(مطابقت.کلید.اشاریہ < darray_سائز(store->کم));
        واپس darray_شے(store->کم, مطابقت.کلید.اشاریہ);
    } ورنہ {
        /* assume valid index */
        أكد(مطابقت.کلید.اشاریہ < darray_سائز(store->بلند));
        واپس darray_شے(store->بلند, مطابقت.کلید.اشاریہ).name;
    }
}

ساکن Keycode_مطابقت
keycode_store_lookup_keycode(ثابت KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    اگر (kc < (xkb_keycode_t) darray_سائز(store->کم)) {
        واپس (Keycode_مطابقت) {
            .کلید = {
                .ملا = true,
                .کم = true,
                .is_alias = false,
                .اشاریہ = kc
            }
        };
    } ورنہ اگر (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        واپس (Keycode_مطابقت) { .ملا = false };
    }

    /* High keycodes: use binary search */
    darray_سائز_t زیریں = 0;
    darray_سائز_t بالائی = darray_سائز(store->بلند);
    جب (زیریں < بالائی) {
        ثابت darray_سائز_t mid = زیریں + (بالائی - 1 - زیریں) / 2;
        بلند_Keycode_اندراج * ثابت اندراج_2 = &darray_شے(store->بلند, mid);
        اگر (اندراج_2->keycode < kc) {
            زیریں = mid + 1;
        } ورنہ اگر (اندراج_2->keycode > kc) {
            بالائی = mid;
        } ورنہ {
            واپس (Keycode_مطابقت) {
                .کلید = {
                    .ملا = true,
                    .کم = false,
                    .is_alias = false,
                    .اشاریہ = mid
                }
            };
        }
    }

    واپس (Keycode_مطابقت) { .ملا = false };
}

ساکن Keycode_مطابقت
keycode_store_lookup_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    اگر (name >= darray_سائز(store->نام)) {
        واپس (Keycode_مطابقت) { .ملا = false, .is_alias = false };
    } ورنہ {
        واپس darray_شے(store->نام, name);
    }
}

/***====================================================================***/

قسمتعریف ساخت {
    شمار ملاؤ_منوال ملاؤ;

    xkb_atom_t name;
} LedNameInfo;

قسمتعریف ساخت {
    حرف *name;
    صحیح error_گنتی;
    غیرعلامتی صحیح include_گہرائی;

    KeycodeStore keycodes;
    LedNameInfo led_نام[XKB_MAX_LEDS];
    xkb_led_اشاریہ_t عدد_led_نام;

    ساخت xkb_context *ctx;
    ثابت ساخت xkb_keymap_info *keymap_info;
} کلید_نام_Info;

/***====================================================================***/

ساکن LedNameInfo *
ڈھونڈو_Led_By_Name(کلید_نام_Info *info, xkb_atom_t name,
              xkb_led_اشاریہ_t *idx_برآمد)
{
    برائے (xkb_led_اشاریہ_t idx = 0; idx < info->عدد_led_نام; idx++) {
        LedNameInfo *ledi = &info->led_نام[idx];
        اگر (ledi->name == name) {
            *idx_برآمد = idx;
            واپس ledi;
        }
    }

    واپس تہی;
}

ساکن bool
جوڑو_Led_Name(کلید_نام_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_اشاریہ_t new_idx, bool report)
{
    xkb_led_اشاریہ_t old_idx;
    LedNameInfo *old;
    ثابت bool replace = (new->ملاؤ != ملاؤ_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = ڈھونڈو_Led_By_Name(info, new->name, &old_idx);
    اگر (old) {
        اگر (old_idx == new_idx) {
            اگر (report)
                log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_متن(info->ctx, new->name));
            واپس true;
        }

        اگر (report) {
            xkb_led_اشاریہ_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_اشاریہ_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_متن(info->ctx, new->name), use, ignore);
        }

        اگر (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } ورنہ {
            واپس true;
        }
    }

    اگر (new_idx >= info->عدد_led_نام)
        info->عدد_led_نام = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_نام[new_idx];
    اگر (old->name != XKB_ATOM_NONE) {
        اگر (report) {
            ثابت xkb_atom_t use    = (replace ? new->name : old->name);
            ثابت xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_متن(info->ctx, use),
                     xkb_atom_متن(info->ctx, ignore));
        }

        اگر (replace)
            *old = *new;

        واپس true;
    }

    *old = *new;
    واپس true;
}

ساکن خالی
Clear_کلید_نام_Info(کلید_نام_Info *info)
{
    آزاد(info->name);
    keycode_store_free(&info->keycodes);
}

ساکن خالی
Init_کلید_نام_Info(کلید_نام_Info *info, ثابت ساخت xkb_keymap_info *keymap_info,
                 غیرعلامتی صحیح include_گہرائی)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_گہرائی = include_گہرائی;
    keycode_store_init(&info->keycodes);
}

ساکن bool
جوڑو_کلید_Name(کلید_نام_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           شمار ملاؤ_منوال ملاؤ, bool report)
{
#اگر_تعریف_شدہ NDEBUG
    ثابت
#اگر_ختم
    Keycode_مطابقت مطابقت_name = keycode_store_lookup_name(&info->keycodes, name);
    اگر (مطابقت_name.ملا) {
        ثابت bool clobber = (ملاؤ != ملاؤ_AUGMENT);

        اگر (مطابقت_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            اگر (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         کلید_Name_متن(info->ctx, name),
                         (clobber ? "key" : "alias"),
                         (clobber ? "alias" : "key"));
            }

            اگر (clobber) {
                /*
                 * Override the alias. If there is a conflict with the keycode
                 * afterwards, the old key entry will be also overriden thanks
                 * to “clobber”.
                 */
                keycode_store_delete_name(&info->keycodes, name);
#اگر_غیر_تعریف_شدہ NDEBUG
                مطابقت_name.ملا = false;
#اگر_ختم
            } ورنہ {
                واپس true;
            }
        } ورنہ {
            ثابت xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, مطابقت_name);
            أكد(old_kc != XKB_KEYCODE_INVALID);
            اگر (old_kc != kc) {
                /* There is already a different key with this name. */

                اگر (report) {
                    ثابت xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ثابت xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            کلید_Name_متن(info->ctx, name), use, ignore);
                }

                اگر (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_کلید(&info->keycodes, مطابقت_name);
                } ورنہ {
                    واپس true;
                }
            }
        }
    }

    ثابت Keycode_مطابقت مطابقت_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ثابت xkb_atom_t old_name = keycode_store_get_کلید_name(&info->keycodes, مطابقت_kc);
    اگر (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        اگر (old_name == name) {
            أكد(keycode_store_get_keycode(&info->keycodes, مطابقت_name) == kc);
            اگر (report)
                log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         کلید_Name_متن(info->ctx, old_name), kc);
            واپس true;
        }

        ثابت bool clobber = (ملاؤ != ملاؤ_AUGMENT);
        اگر (report) {
            ثابت حرف* ثابت kname     = کلید_Name_متن(info->ctx, name);
            ثابت حرف* ثابت old_kname = کلید_Name_متن(info->ctx, old_name);
            ثابت حرف* ثابت use    = (clobber) ? kname : old_kname;
            ثابت حرف* ثابت ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        اگر (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_کلید(&info->keycodes, مطابقت_kc, name);
        }
    } ورنہ {
        /* No previous keycode */
        اگر (!keycode_store_داخل_کرو_کلید(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                    "Cannot add keycode\n");
            واپس false;
        }
    }

    واپس true;
}

/***====================================================================***/

ساکن bool
HandleAliasDef(کلید_نام_Info *info, ثابت کلید_Alias_Def *def, bool report);

ساکن خالی
ملاؤ_Keycode_Stores(کلید_نام_Info *into, کلید_نام_Info *from,
                   شمار ملاؤ_منوال ملاؤ, bool report)
{
    اگر (darray_خالی_خانے(into->keycodes.کم) && darray_خالی_خانے(into->keycodes.بلند) &&
        darray_خالی_خانے(into->keycodes.نام)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.کم);
        darray_init(from->keycodes.بلند);
        darray_init(from->keycodes.نام);
    } ورنہ {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        برائے (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_سائز(from->keycodes.کم);
             kc++) {
            /* Skip undefined keycodes */
            ثابت xkb_atom_t name = darray_شے(from->keycodes.کم, kc);
            اگر (name == XKB_ATOM_NONE)
                جاری;

            اگر (!جوڑو_کلید_Name(into, kc, name, ملاؤ, report))
                into->error_گنتی++;
        }

        /* High keycodes */
        ثابت بلند_Keycode_اندراج *new;
        darray_foreach(new, from->keycodes.بلند) {
            أكد(new->name != XKB_ATOM_NONE);
            اگر (!جوڑو_کلید_Name(into, new->keycode, new->name, ملاؤ, report))
                into->error_گنتی++;
        }

        /* Aliases. */
        Keycode_مطابقت *مطابقت;
        xkb_atom_t alias;
        darray_enumerate(alias, مطابقت, from->keycodes.نام) {
            اگر (!مطابقت->ملا || !مطابقت->is_alias)
                جاری;

            ثابت کلید_Alias_Def def = {
                .ملاؤ = ملاؤ,
                .alias = alias,
                .real = مطابقت->alias.real
            };
            اگر (!HandleAliasDef(into, &def, report))
                into->error_گنتی++;
        }
    }
}

ساکن خالی
ملاؤ_Included_Keycodes(کلید_نام_Info *into, کلید_نام_Info *from,
                      شمار ملاؤ_منوال ملاؤ, bool report)
{
    اگر (from->error_گنتی > 0) {
        into->error_گنتی += from->error_گنتی;
        واپس;
    }

    اگر (into->name == تہی) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    ملاؤ_Keycode_Stores(into, from, ملاؤ, report);

    /* Merge LED names. */
    اگر (into->عدد_led_نام == 0) {
        حافظہ_نقل(into->led_نام, from->led_نام,
               حجم(*from->led_نام) * from->عدد_led_نام);
        into->عدد_led_نام = from->عدد_led_نام;
        from->عدد_led_نام = 0;
    }
    ورنہ {
        برائے (xkb_led_اشاریہ_t idx = 0; idx < from->عدد_led_نام; idx++) {
            LedNameInfo *ledi = &from->led_نام[idx];

            اگر (ledi->name == XKB_ATOM_NONE)
                جاری;

            ledi->ملاؤ = ملاؤ;
            اگر (!جوڑو_Led_Name(into, false, ledi, idx, report))
                into->error_گنتی++;
        }
    }
}

ساکن خالی
HandleKeycodesFile(کلید_نام_Info *info, XkbFile *file);

ساکن bool
HandleIncludeKeycodes(کلید_نام_Info *info, IncludeStmt *include, bool report)
{
    کلید_نام_Info included;

    اگر (Exceeds_Include_Max_گہرائی(info->ctx, info->include_گہرائی)) {
        info->error_گنتی += 10;
        واپس false;
    }

    Init_کلید_نام_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ثابت ساخت parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    برائے (IncludeStmt *stmt = include; stmt; stmt = stmt->اگلا_incl) {
        کلید_نام_Info اگلا_incl;
        XkbFile *file;

        حرف راستہ[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  راستہ, حجم(راستہ));
        اگر (!file) {
            info->error_گنتی += 10;
            Clear_کلید_نام_Info(&included);
            واپس false;
        }

        Init_کلید_نام_Info(&اگلا_incl, info->keymap_info, info->include_گہرائی + 1);

        HandleKeycodesFile(&اگلا_incl, file);

        ملاؤ_Included_Keycodes(&included, &اگلا_incl, stmt->ملاؤ, report);

        Clear_کلید_نام_Info(&اگلا_incl);
        FreeXkbFile(file);
    }

    ملاؤ_Included_Keycodes(info, &included, include->ملاؤ, report);
    Clear_کلید_نام_Info(&included);

    واپس (info->error_گنتی == 0);
}

ساکن bool
HandleKeycodeDef(کلید_نام_Info *info, KeycodeDef *stmt, bool report)
{
    اگر (stmt->قدر < 0 || stmt->قدر > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->قدر, XKB_KEYCODE_MAX);
        واپس false;
    }

    واپس جوڑو_کلید_Name(info, (xkb_keycode_t) stmt->قدر,
                      stmt->name, stmt->ملاؤ, report);
}

ساکن bool
HandleAliasDef(کلید_نام_Info *info, ثابت کلید_Alias_Def *def, bool report)
{
    ثابت Keycode_مطابقت مطابقت_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    اگر (مطابقت_name.ملا) {
        ثابت bool clobber = (def->ملاؤ != ملاؤ_AUGMENT);
        اگر (مطابقت_name.is_alias) {
            اگر (def->real == مطابقت_name.alias.real) {
                اگر (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             کلید_Name_متن(info->ctx, def->alias),
                             کلید_Name_متن(info->ctx, def->real));
            } ورنہ {
                ثابت xkb_atom_t use = (clobber)
                    ? def->real
                    : مطابقت_name.alias.real;
                ثابت xkb_atom_t ignore = (clobber)
                    ? مطابقت_name.alias.real
                    : def->real;

                اگر (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             کلید_Name_متن(info->ctx, def->alias),
                             کلید_Name_متن(info->ctx, use),
                             کلید_Name_متن(info->ctx, ignore));

                keycode_store_update_alias(&info->keycodes, def->alias, use);
            }
            واپس true;
        } ورنہ {
            /* There is already a real key with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             *
             * Also contrary to xkbcomp, we enable aliases to override keys.
             */
            اگر (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         کلید_Name_متن(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            اگر (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_کلید(&info->keycodes, مطابقت_name);
            } ورنہ {
                واپس true;
            }
        }
    }

    واپس keycode_store_داخل_کرو_alias(&info->keycodes, def->alias, def->real);
}

ساکن bool
Handle_کلید_Name_Var(کلید_نام_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *میدان_2;
    اظہاریہ_Def *arrayNdx;

    اگر (!اظہاریہ_Resolve_Lhs(info->ctx, stmt->name, &elem, &میدان_2, &arrayNdx))
        واپس false;

    اگر (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, میدان_2);
        واپس !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    اگر (!istreq(میدان_2, "minimum") && !istreq(میدان_2, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_میدان,
                "Default defined for unknown field \"%s\"; Ignored\n", میدان_2);
        واپس !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    اگر (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", میدان_2, "defaults");
        واپس !(info->keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH);
    }

    int64_t val = 0;
    اگر (!اظہاریہ_Resolve_Integer(info->ctx, stmt->قدر, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_میدان_TYPE, "keycodes",
                      میدان_2, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        واپس !(info->keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    واپس true;
}

ساکن bool
HandleLedNameDef(کلید_نام_Info *info, LedNameDef *def, bool report)
{
    اگر (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_گنتی++;
        log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        واپس false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    اگر (!اظہاریہ_Resolve_String(info->ctx, def->name, &name)) {
        حرف buf[20];
        snprintf(buf, حجم(buf), "%"PRId64, def->ndx);
        info->error_گنتی++;
        واپس ReportBadType(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.ملاؤ = def->ملاؤ, .name = name};
    واپس جوڑو_Led_Name(info, true, &ledi, (xkb_led_اشاریہ_t) def->ndx - 1, report);
}

ساکن خالی
HandleKeycodesFile(کلید_نام_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ثابت صحیح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report_same_file = verbosity > 0;
    ثابت bool report_include   = verbosity > 7;

    آزاد(info->name);
    info->name = strdup_محفوظ(file->name);

    برائے (ParseCommon *stmt = file->defs; stmt; stmt = stmt->اگلا) {
        انتخاب (stmt->type) {
        حالت STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            توڑو;
        حالت STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            توڑو;
        حالت STMT_ALIAS:
            ok = HandleAliasDef(info, (کلید_Alias_Def *) stmt, report_same_file);
            توڑو;
        حالت STMT_VAR:
            ok = Handle_کلید_Name_Var(info, (VarDef *) stmt);
            توڑو;
        حالت STMT_LED_NAME:
            ok = HandleLedNameDef(info, (LedNameDef *) stmt, report_same_file);
            توڑو;
        حالت STMT_UNKNOWN_DECLARATION:
        حالت STMT_UNKNOWN_COMPOUND:
            log_err(info->ctx, XKB_ERROR_UNKNOWN_STATEMENT,
                    "Unsupported keycodes %s statement \"%s\"; Ignoring\n",
                    (stmt->type == STMT_UNKNOWN_COMPOUND
                        ? "compound" : "declaration"),
                    ((UnknownStatement *)stmt)->name);
            ok = !(info->keymap_info->strict & PARSER_NO_UNKNOWN_STATEMENTS);
            توڑو;
        طےشدہ:
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_مقصد_string(stmt->type));
            ok = false;
            توڑو;
        }

        اگر (!ok)
            info->error_گنتی++;

        اگر (info->error_گنتی > 10) {
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    محفوظ_map_name(file->name));
            توڑو;
        }
    }
}

/***====================================================================***/

ساکن bool
Copy_کلید_نام_مقصد_Keymap(ساخت xkb_keymap *keymap, کلید_نام_Info *info)
{
    اگر (darray_خالی_خانے(info->keycodes.کم) && darray_خالی_خانے(info->keycodes.بلند)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        أكد(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_کلید_رمز = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_کلید_رمز = 255;
        keymap->عدد_کلیدیں = keymap->عدد_کلیدیں_کم = keymap->max_کلید_رمز + 1;
    } ورنہ {
        أكد(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_کلید_رمز = info->keycodes.min;
        keymap->max_کلید_رمز = (darray_خالی_خانے(info->keycodes.بلند))
            ? darray_سائز(info->keycodes.کم) - 1
            : darray_شے(
                info->keycodes.بلند,
                darray_سائز(info->keycodes.بلند) - 1
            ).keycode;
        keymap->عدد_کلیدیں_کم = darray_سائز(info->keycodes.کم);
        keymap->عدد_کلیدیں = keymap->عدد_کلیدیں_کم
                         + darray_سائز(info->keycodes.بلند);
    }

    ساخت xkb_کلید * ثابت کلیدیں = صفر_حافظہ_دو(keymap->عدد_کلیدیں, حجم(*کلیدیں));
    اگر (!کلیدیں) {
        keymap->عدد_کلیدیں = 0;
        keymap->min_کلید_رمز = keymap->max_کلید_رمز = XKB_KEYCODE_INVALID;
        واپس false;
    }

    /* Low keycodes */
    برائے (xkb_keycode_t kc = keymap->min_کلید_رمز; kc < keymap->عدد_کلیدیں_کم; kc++)
        کلیدیں[kc].keycode = kc;

    برائے (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_سائز(info->keycodes.کم);
         kc++)
        کلیدیں[kc].name = darray_شے(info->keycodes.کم, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->عدد_کلیدیں_کم; /* First high keycode index */
    ثابت بلند_Keycode_اندراج *اندراج_2;
    darray_foreach(اندراج_2, info->keycodes.بلند) {
        أكد(اندراج_2->name != XKB_ATOM_NONE);
        کلیدیں[idx].keycode = اندراج_2->keycode;
        کلیدیں[idx].name = اندراج_2->name;
        idx++;
    }

    keymap->کلیدیں = کلیدیں;
    واپس true;
}

ساکن bool
CopyKeycodeNameLUT(ساخت xkb_keymap *keymap, کلید_نام_Info *info)
{
    Keycode_مطابقت *مطابقت;
    xkb_atom_t name;
    darray_enumerate(name, مطابقت, info->keycodes.نام) {
        اگر (!مطابقت->ملا)
            جاری;

        اگر (مطابقت->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ثابت Keycode_مطابقت مطابقت_real =
                keycode_store_lookup_name(&info->keycodes, مطابقت->alias.real);
            اگر (!مطابقت_real.ملا) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        کلید_Name_متن(info->ctx, name),
                        کلید_Name_متن(info->ctx, مطابقت->alias.real));
                مطابقت->ملا = false;
                جاری;
            }
            أكد(!مطابقت_real.is_alias);
        } ورنہ اگر (!مطابقت->کلید.کم) {
            /* Update to final index in keymap::keys */
            مطابقت->کلید.اشاریہ += keymap->عدد_کلیدیں_کم;
        }
    }

    darray_shrink(info->keycodes.نام);
    keymap->عدد_کلید_نام = darray_سائز(info->keycodes.نام);
    darray_steal(info->keycodes.نام, &keymap->کلید_نام, تہی);
    darray_init(info->keycodes.نام);
    واپس true;
}

ساکن bool
Copy_Led_نام_مقصد_Keymap(ساخت xkb_keymap *keymap, کلید_نام_Info *info)
{
    keymap->عدد_leds = info->عدد_led_نام;
    برائے (xkb_led_اشاریہ_t idx = 0; idx < info->عدد_led_نام; idx++) {
        LedNameInfo *ledi = &info->led_نام[idx];

        اگر (ledi->name == XKB_ATOM_NONE)
            جاری;

        keymap->leds[idx].name = ledi->name;
    }

    واپس true;
}

ساکن bool
Copy_کلید_نام_Info_مقصد_Keymap(ساخت xkb_keymap *keymap, کلید_نام_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    اگر (!Copy_کلید_نام_مقصد_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !Copy_Led_نام_مقصد_Keymap(keymap, info))
        واپس false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    اگر (!keymap->عدد_کلیدیں ||
        keymap->min_کلید_رمز > 0) {
        keymap->redirect_کلید_auto = 0;
    } ورنہ {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        برائے (xkb_keycode_t k = keymap->عدد_کلیدیں; k-- > keymap->عدد_کلیدیں_کم;) {
            اگر (keycode > keymap->کلیدیں[k].keycode)
                توڑو;
            /* Try next keycode downwards */
            keycode = keymap->کلیدیں[k].keycode - 1;
        }
        keymap->redirect_کلید_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_محفوظ(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    واپس true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, ساخت xkb_keymap_info *keymap_info)
{
    کلید_نام_Info info;

    Init_کلید_نام_Info(&info, keymap_info, 0);

    اگر (file != تہی)
        HandleKeycodesFile(&info, file);

    اگر (info.error_گنتی != 0)
        جاؤ err_info;

    اگر (!Copy_کلید_نام_Info_مقصد_Keymap(&keymap_info->keymap, &info))
        جاؤ err_info;

    Clear_کلید_نام_Info(&info);
    واپس true;

err_info:
    Clear_کلید_نام_Info(&info);
    واپس false;
}
