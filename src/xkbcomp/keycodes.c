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
} بالا_Keycode_مدخل;

تعريفنوع بنية {
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
    darray(بالا_Keycode_مدخل) بالا;
    /**
     * name -> keycode mapping
     */
    darray(Keycode_تطابق) نامها;
} KeycodeStore;

ساكن مضمن فراغ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->کم);
    darray_init(store->بالا);
    darray_init(store->نامها);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ساكن مضمن فراغ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->کم);
    darray_free(store->بالا);
    darray_free(store->نامها);
}

ساكن مضمن فراغ
keycode_store_update_کلید(KeycodeStore *store, Keycode_تطابق تطابق, xkb_atom_t name)
{
    إذا (unlikely(!تطابق.یافت || تطابق.is_alias)) {
        أرجع;
    } وإلا إذا (تطابق.کلید.کم) {
        /* assume valid index */
        أكد(تطابق.کلید.شاخص < darray_اندازه(store->کم));
        darray_قلم(store->کم, تطابق.کلید.شاخص) = name;
    } وإلا {
        /* assume valid index */
        أكد(تطابق.کلید.شاخص < darray_اندازه(store->بالا));
        darray_قلم(store->بالا, تطابق.کلید.شاخص).name = name;
    }

    /* Names lookup */
    إذا (name >= darray_اندازه(store->نامها)) {
        darray_resize0(store->نامها, name + 1);
    }
    darray_قلم(store->نامها, name) = تطابق;
}

ساكن bool
keycode_store_درج_کلید(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    إذا (name >= darray_اندازه(store->نامها)) {
        darray_resize0(store->نامها, name + 1);
    }
    إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        إذا (kc >= (xkb_keycode_t) darray_اندازه(store->کم))
            darray_resize0(store->کم, kc + 1);
        darray_قلم(store->کم, kc) = name;
        إذا (kc < store->min)
            store->min = kc;
        darray_قلم(store->نامها, name) = (Keycode_تطابق) {
            .کلید = {
                .یافت = true,
                .کم = true,
                .is_alias = false,
                .شاخص = kc
            }
        };
    } وإلا {
        /* High keycode: insert into a sorted list */
        ثابت darray_اندازه_t idx = darray_اندازه(store->بالا);
        إذا (idx && darray_قلم(store->بالا, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_اندازه_t پایینی = 0;
            darray_اندازه_t فوقانی = idx;
            طالما (پایینی < فوقانی) {
                ثابت darray_اندازه_t mid = پایینی + (فوقانی - 1 - پایینی) / 2;
                ثابت بالا_Keycode_مدخل * ثابت مدخل =
                    &darray_قلم(store->بالا, mid);
                إذا (مدخل->keycode < kc) {
                    پایینی = mid + 1;
                } وإلا إذا (مدخل->keycode > kc) {
                    فوقانی = mid;
                } وإلا {
                    /* Unreachable: there is no repetition in the list */
                    أكد(مدخل->keycode != kc);
                }
            }
            أكد(پایینی < idx);
            أكد(darray_قلم(store->بالا, پایینی).keycode > kc);

            /* Update references to entries that will be moved */
            بالا_Keycode_مدخل *مدخل;
            darray_foreach_from(مدخل, store->بالا, پایینی) {
                darray_قلم(store->نامها, مدخل->name).کلید.شاخص++;
            }

            darray_درج(store->بالا, پایینی,
                          (بالا_Keycode_مدخل){.keycode = kc, .name = name});
            darray_قلم(store->نامها, name) = (Keycode_تطابق) {
                .کلید = {
                    .یافت = true,
                    .کم = false,
                    .is_alias = false,
                    .شاخص = پایینی
                }
            };
        } وإلا {
            /* Fast path: no need to sort */
            darray_append(store->بالا,
                          (بالا_Keycode_مدخل){.keycode = kc, .name = name});
            darray_قلم(store->نامها, name) = (Keycode_تطابق) {
                .کلید = {
                    .یافت = true,
                    .کم = false,
                    .is_alias = false,
                    .شاخص = idx
                }
            };
        }
        إذا (darray_خالی(store->کم))
            store->min = darray_قلم(store->بالا, 0).keycode;
    }
    أرجع true;
}

ساكن مضمن bool
keycode_store_درج_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    إذا (alias >= darray_اندازه(store->نامها)) {
        darray_resize0(store->نامها, alias + 1);
    }
    darray_قلم(store->نامها, alias) = (Keycode_تطابق) {
        .alias = {
            .یافت = true,
            .is_alias = true,
            .real = real
        }
    };
    أرجع true;
}

ساكن مضمن bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_قلم(store->نامها, alias).alias.real = real;
    أرجع true;
}

ساكن مضمن فراغ
keycode_store_delete_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    إذا (name < darray_اندازه(store->نامها))
        darray_قلم(store->نامها, name).یافت = false;
}

ساكن فراغ
keycode_store_delete_کلید(KeycodeStore *store, ثابت Keycode_تطابق تطابق)
{
    إذا (unlikely(!تطابق.یافت || تطابق.is_alias)) {
        أرجع;
    } وإلا إذا (تطابق.کلید.کم) {
        /* assume valid index */
        أكد(تطابق.کلید.شاخص < darray_اندازه(store->کم));
        darray_قلم(store->نامها,
                    darray_قلم(store->کم, تطابق.کلید.شاخص)).یافت = false;
        إذا (تطابق.کلید.شاخص + 1u == darray_اندازه(store->کم)) {
            /* Highest low keycode: shrink */
            إذا (store->min == تطابق.کلید.شاخص) {
                /* No low keycode left */
                darray_اندازه(store->کم) = 0;
            } وإلا {
                /* Look for previous defined low keycode */
                لكل (darray_اندازه_t idx = تطابق.کلید.شاخص; idx > 0; idx--) {
                    إذا (darray_قلم(store->کم, idx - 1) != XKB_ATOM_NONE) {
                        darray_اندازه(store->کم) = idx;
                        اكسر;
                    }
                }
            }
        } وإلا {
            /* Lower keycode: reset */
            darray_قلم(store->کم, تطابق.کلید.شاخص) = XKB_ATOM_NONE;
        }
    } وإلا {
        أكد(تطابق.کلید.شاخص < darray_اندازه(store->بالا)); /* assume valid index */
        darray_قلم(store->نامها,
                    darray_قلم(store->بالا, تطابق.کلید.شاخص).name).یافت = false;
        darray_remove(store->بالا, تطابق.کلید.شاخص);
        /* Update LUT indexes of high codes after the deleted one, if any */
        Keycode_تطابق *مدخل;
        darray_foreach(مدخل, store->نامها) {
            إذا (مدخل->یافت && !مدخل->is_alias && !مدخل->کلید.کم &&
                مدخل->کلید.شاخص > تطابق.کلید.شاخص) {
                مدخل->کلید.شاخص--;
            }
        }
    }

    /* Update bounds */
    إذا (darray_خالی(store->کم)) {
        store->min = (darray_خالی(store->بالا))
            ? XKB_KEYCODE_INVALID
            : darray_قلم(store->بالا, 0).keycode;
    } وإلا {
        لكل (xkb_keycode_t kc = store->min; kc < darray_اندازه(store->کم); kc++) {
            إذا (darray_قلم(store->کم, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                اكسر;
            }
        }
    }
}

ساكن مضمن xkb_keycode_t
keycode_store_get_keycode(ثابت KeycodeStore *store, Keycode_تطابق تطابق)
{
    إذا (!تطابق.یافت || تطابق.is_alias) {
        أرجع XKB_KEYCODE_INVALID;
    } وإلا إذا (تطابق.کلید.کم) {
        /* assume valid index */
        أكد(تطابق.کلید.شاخص < darray_اندازه(store->کم));
        أرجع (xkb_keycode_t) تطابق.کلید.شاخص;
    } وإلا {
        /* assume valid index */
        أكد(تطابق.کلید.شاخص < darray_اندازه(store->بالا));
        أرجع darray_قلم(store->بالا, تطابق.کلید.شاخص).keycode;
    }
}

ساكن مضمن xkb_atom_t
keycode_store_get_کلید_name(ثابت KeycodeStore *store, Keycode_تطابق تطابق)
{
    إذا (!تطابق.یافت || تطابق.is_alias) {
        أرجع XKB_ATOM_NONE;
    } وإلا إذا (تطابق.کلید.کم) {
        /* assume valid index */
        أكد(تطابق.کلید.شاخص < darray_اندازه(store->کم));
        أرجع darray_قلم(store->کم, تطابق.کلید.شاخص);
    } وإلا {
        /* assume valid index */
        أكد(تطابق.کلید.شاخص < darray_اندازه(store->بالا));
        أرجع darray_قلم(store->بالا, تطابق.کلید.شاخص).name;
    }
}

ساكن Keycode_تطابق
keycode_store_lookup_keycode(ثابت KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    إذا (kc < (xkb_keycode_t) darray_اندازه(store->کم)) {
        أرجع (Keycode_تطابق) {
            .کلید = {
                .یافت = true,
                .کم = true,
                .is_alias = false,
                .شاخص = kc
            }
        };
    } وإلا إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        أرجع (Keycode_تطابق) { .یافت = false };
    }

    /* High keycodes: use binary search */
    darray_اندازه_t پایینی = 0;
    darray_اندازه_t فوقانی = darray_اندازه(store->بالا);
    طالما (پایینی < فوقانی) {
        ثابت darray_اندازه_t mid = پایینی + (فوقانی - 1 - پایینی) / 2;
        بالا_Keycode_مدخل * ثابت مدخل = &darray_قلم(store->بالا, mid);
        إذا (مدخل->keycode < kc) {
            پایینی = mid + 1;
        } وإلا إذا (مدخل->keycode > kc) {
            فوقانی = mid;
        } وإلا {
            أرجع (Keycode_تطابق) {
                .کلید = {
                    .یافت = true,
                    .کم = false,
                    .is_alias = false,
                    .شاخص = mid
                }
            };
        }
    }

    أرجع (Keycode_تطابق) { .یافت = false };
}

ساكن Keycode_تطابق
keycode_store_lookup_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    إذا (name >= darray_اندازه(store->نامها)) {
        أرجع (Keycode_تطابق) { .یافت = false, .is_alias = false };
    } وإلا {
        أرجع darray_قلم(store->نامها, name);
    }
}

/***====================================================================***/

تعريفنوع بنية {
    تعداد ادغام_نما ادغام;

    xkb_atom_t name;
} LedNameInfo;

تعريفنوع بنية {
    حرف *name;
    صحيح error_شمار;
    بیعلامت صحيح include_عمق;

    KeycodeStore keycodes;
    LedNameInfo led_نامها[XKB_MAX_LEDS];
    xkb_led_شاخص_t رقم_led_نامها;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} کلید_نامها_Info;

/***====================================================================***/

ساكن LedNameInfo *
پیدا_کن_Led_By_Name(کلید_نامها_Info *info, xkb_atom_t name,
              xkb_led_شاخص_t *idx_بیرون)
{
    لكل (xkb_led_شاخص_t idx = 0; idx < info->رقم_led_نامها; idx++) {
        LedNameInfo *ledi = &info->led_نامها[idx];
        إذا (ledi->name == name) {
            *idx_بیرون = idx;
            أرجع ledi;
        }
    }

    أرجع عدم;
}

ساكن bool
جمع_کن_Led_Name(کلید_نامها_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_شاخص_t new_idx, bool report)
{
    xkb_led_شاخص_t old_idx;
    LedNameInfo *old;
    ثابت bool replace = (new->ادغام != ادغام_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = پیدا_کن_Led_By_Name(info, new->name, &old_idx);
    إذا (old) {
        إذا (old_idx == new_idx) {
            إذا (report)
                log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_متن(info->ctx, new->name));
            أرجع true;
        }

        إذا (report) {
            xkb_led_شاخص_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_شاخص_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                     "Multiple indicators named %s; "
                     "Using %"PRIu32", ignoring %"PRIu32"\n",
                     xkb_atom_متن(info->ctx, new->name), use, ignore);
        }

        إذا (replace) {
            /* Unset previous */
            old->name = XKB_ATOM_NONE;
        } وإلا {
            أرجع true;
        }
    }

    إذا (new_idx >= info->رقم_led_نامها)
        info->رقم_led_نامها = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_نامها[new_idx];
    إذا (old->name != XKB_ATOM_NONE) {
        إذا (report) {
            ثابت xkb_atom_t use    = (replace ? new->name : old->name);
            ثابت xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                     "Multiple names for indicator %"PRIu32"; "
                     "Using %s, ignoring %s\n", new_idx + 1,
                     xkb_atom_متن(info->ctx, use),
                     xkb_atom_متن(info->ctx, ignore));
        }

        إذا (replace)
            *old = *new;

        أرجع true;
    }

    *old = *new;
    أرجع true;
}

ساكن فراغ
Clear_کلید_نامها_Info(کلید_نامها_Info *info)
{
    حرر(info->name);
    keycode_store_free(&info->keycodes);
}

ساكن فراغ
Init_کلید_نامها_Info(کلید_نامها_Info *info, ثابت بنية xkb_keymap_info *keymap_info,
                 بیعلامت صحيح include_عمق)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_عمق = include_عمق;
    keycode_store_init(&info->keycodes);
}

ساكن bool
جمع_کن_کلید_Name(کلید_نامها_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           تعداد ادغام_نما ادغام, bool report)
{
#اگر_تعریف_شده NDEBUG
    ثابت
#پایان_اگر
    Keycode_تطابق تطابق_name = keycode_store_lookup_name(&info->keycodes, name);
    إذا (تطابق_name.یافت) {
        ثابت bool clobber = (ادغام != ادغام_AUGMENT);

        إذا (تطابق_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            إذا (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         کلید_Name_متن(info->ctx, name),
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
#اگر_تعریف_نشده NDEBUG
                تطابق_name.یافت = false;
#پایان_اگر
            } وإلا {
                أرجع true;
            }
        } وإلا {
            ثابت xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, تطابق_name);
            أكد(old_kc != XKB_KEYCODE_INVALID);
            إذا (old_kc != kc) {
                /* There is already a different key with this name. */

                إذا (report) {
                    ثابت xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ثابت xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            کلید_Name_متن(info->ctx, name), use, ignore);
                }

                إذا (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_کلید(&info->keycodes, تطابق_name);
                } وإلا {
                    أرجع true;
                }
            }
        }
    }

    ثابت Keycode_تطابق تطابق_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ثابت xkb_atom_t old_name = keycode_store_get_کلید_name(&info->keycodes, تطابق_kc);
    إذا (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        إذا (old_name == name) {
            أكد(keycode_store_get_keycode(&info->keycodes, تطابق_name) == kc);
            إذا (report)
                log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         کلید_Name_متن(info->ctx, old_name), kc);
            أرجع true;
        }

        ثابت bool clobber = (ادغام != ادغام_AUGMENT);
        إذا (report) {
            ثابت حرف* ثابت kname     = کلید_Name_متن(info->ctx, name);
            ثابت حرف* ثابت old_kname = کلید_Name_متن(info->ctx, old_name);
            ثابت حرف* ثابت use    = (clobber) ? kname : old_kname;
            ثابت حرف* ثابت ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_پیام_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        إذا (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_کلید(&info->keycodes, تطابق_kc, name);
        }
    } وإلا {
        /* No previous keycode */
        إذا (!keycode_store_درج_کلید(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_تخصیص_FAILURE_2,
                    "Cannot add keycode\n");
            أرجع false;
        }
    }

    أرجع true;
}

/***====================================================================***/

ساكن bool
HandleAliasDef(کلید_نامها_Info *info, ثابت کلید_Alias_Def *def, bool report);

ساكن فراغ
ادغام_Keycode_Stores(کلید_نامها_Info *into, کلید_نامها_Info *from,
                   تعداد ادغام_نما ادغام, bool report)
{
    إذا (darray_خالی(into->keycodes.کم) && darray_خالی(into->keycodes.بالا) &&
        darray_خالی(into->keycodes.نامها)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.کم);
        darray_init(from->keycodes.بالا);
        darray_init(from->keycodes.نامها);
    } وإلا {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        لكل (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_اندازه(from->keycodes.کم);
             kc++) {
            /* Skip undefined keycodes */
            ثابت xkb_atom_t name = darray_قلم(from->keycodes.کم, kc);
            إذا (name == XKB_ATOM_NONE)
                استمر;

            إذا (!جمع_کن_کلید_Name(into, kc, name, ادغام, report))
                into->error_شمار++;
        }

        /* High keycodes */
        ثابت بالا_Keycode_مدخل *new;
        darray_foreach(new, from->keycodes.بالا) {
            أكد(new->name != XKB_ATOM_NONE);
            إذا (!جمع_کن_کلید_Name(into, new->keycode, new->name, ادغام, report))
                into->error_شمار++;
        }

        /* Aliases. */
        Keycode_تطابق *تطابق;
        xkb_atom_t alias;
        darray_enumerate(alias, تطابق, from->keycodes.نامها) {
            إذا (!تطابق->یافت || !تطابق->is_alias)
                استمر;

            ثابت کلید_Alias_Def def = {
                .ادغام = ادغام,
                .alias = alias,
                .real = تطابق->alias.real
            };
            إذا (!HandleAliasDef(into, &def, report))
                into->error_شمار++;
        }
    }
}

ساكن فراغ
ادغام_Included_Keycodes(کلید_نامها_Info *into, کلید_نامها_Info *from,
                      تعداد ادغام_نما ادغام, bool report)
{
    إذا (from->error_شمار > 0) {
        into->error_شمار += from->error_شمار;
        أرجع;
    }

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    ادغام_Keycode_Stores(into, from, ادغام, report);

    /* Merge LED names. */
    إذا (into->رقم_led_نامها == 0) {
        کپی_حافظه(into->led_نامها, from->led_نامها,
               حجم(*from->led_نامها) * from->رقم_led_نامها);
        into->رقم_led_نامها = from->رقم_led_نامها;
        from->رقم_led_نامها = 0;
    }
    وإلا {
        لكل (xkb_led_شاخص_t idx = 0; idx < from->رقم_led_نامها; idx++) {
            LedNameInfo *ledi = &from->led_نامها[idx];

            إذا (ledi->name == XKB_ATOM_NONE)
                استمر;

            ledi->ادغام = ادغام;
            إذا (!جمع_کن_Led_Name(into, false, ledi, idx, report))
                into->error_شمار++;
        }
    }
}

ساكن فراغ
HandleKeycodesFile(کلید_نامها_Info *info, XkbFile *file);

ساكن bool
HandleIncludeKeycodes(کلید_نامها_Info *info, IncludeStmt *include, bool report)
{
    کلید_نامها_Info included;

    إذا (Exceeds_Include_Max_عمق(info->ctx, info->include_عمق)) {
        info->error_شمار += 10;
        أرجع false;
    }

    Init_کلید_نامها_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->بعدی_incl) {
        کلید_نامها_Info بعدی_incl;
        XkbFile *file;

        حرف مسیر[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  مسیر, حجم(مسیر));
        إذا (!file) {
            info->error_شمار += 10;
            Clear_کلید_نامها_Info(&included);
            أرجع false;
        }

        Init_کلید_نامها_Info(&بعدی_incl, info->keymap_info, info->include_عمق + 1);

        HandleKeycodesFile(&بعدی_incl, file);

        ادغام_Included_Keycodes(&included, &بعدی_incl, stmt->ادغام, report);

        Clear_کلید_نامها_Info(&بعدی_incl);
        FreeXkbFile(file);
    }

    ادغام_Included_Keycodes(info, &included, include->ادغام, report);
    Clear_کلید_نامها_Info(&included);

    أرجع (info->error_شمار == 0);
}

ساكن bool
HandleKeycodeDef(کلید_نامها_Info *info, KeycodeDef *stmt, bool report)
{
    إذا (stmt->مقدار < 0 || stmt->مقدار > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->مقدار, XKB_KEYCODE_MAX);
        أرجع false;
    }

    أرجع جمع_کن_کلید_Name(info, (xkb_keycode_t) stmt->مقدار,
                      stmt->name, stmt->ادغام, report);
}

ساكن bool
HandleAliasDef(کلید_نامها_Info *info, ثابت کلید_Alias_Def *def, bool report)
{
    ثابت Keycode_تطابق تطابق_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    إذا (تطابق_name.یافت) {
        ثابت bool clobber = (def->ادغام != ادغام_AUGMENT);
        إذا (تطابق_name.is_alias) {
            إذا (def->real == تطابق_name.alias.real) {
                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             کلید_Name_متن(info->ctx, def->alias),
                             کلید_Name_متن(info->ctx, def->real));
            } وإلا {
                ثابت xkb_atom_t use = (clobber)
                    ? def->real
                    : تطابق_name.alias.real;
                ثابت xkb_atom_t ignore = (clobber)
                    ? تطابق_name.alias.real
                    : def->real;

                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             کلید_Name_متن(info->ctx, def->alias),
                             کلید_Name_متن(info->ctx, use),
                             کلید_Name_متن(info->ctx, ignore));

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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         کلید_Name_متن(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            إذا (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_کلید(&info->keycodes, تطابق_name);
            } وإلا {
                أرجع true;
            }
        }
    }

    أرجع keycode_store_درج_alias(&info->keycodes, def->alias, def->real);
}

ساكن bool
Handle_کلید_Name_Var(کلید_نامها_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *فیلد_2;
    عبارت_Def *arrayNdx;

    إذا (!عبارت_Resolve_Lhs(info->ctx, stmt->name, &elem, &فیلد_2, &arrayNdx))
        أرجع false;

    إذا (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, فیلد_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    إذا (!istreq(فیلد_2, "minimum") && !istreq(فیلد_2, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_فیلد,
                "Default defined for unknown field \"%s\"; Ignored\n", فیلد_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    إذا (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", فیلد_2, "defaults");
        أرجع !(info->keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH);
    }

    int64_t val = 0;
    إذا (!عبارت_Resolve_Integer(info->ctx, stmt->مقدار, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_فیلد_TYPE, "keycodes",
                      فیلد_2, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        أرجع !(info->keymap_info->strict & PARSER_NO_فیلد_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    أرجع true;
}

ساكن bool
HandleLedNameDef(کلید_نامها_Info *info, LedNameDef *def, bool report)
{
    إذا (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_شمار++;
        log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!عبارت_Resolve_String(info->ctx, def->name, &name)) {
        حرف buf[20];
        snprintf(buf, حجم(buf), "%"PRId64, def->ndx);
        info->error_شمار++;
        أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_فیلد_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.ادغام = def->ادغام, .name = name};
    أرجع جمع_کن_Led_Name(info, true, &ledi, (xkb_led_شاخص_t) def->ndx - 1, report);
}

ساكن فراغ
HandleKeycodesFile(کلید_نامها_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report_same_file = verbosity > 0;
    ثابت bool report_include   = verbosity > 7;

    حرر(info->name);
    info->name = strdup_امن(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->بعدی) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            اكسر;
        حالة STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            اكسر;
        حالة STMT_ALIAS:
            ok = HandleAliasDef(info, (کلید_Alias_Def *) stmt, report_same_file);
            اكسر;
        حالة STMT_VAR:
            ok = Handle_کلید_Name_Var(info, (VarDef *) stmt);
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
            log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_مقصد_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_شمار++;

        إذا (info->error_شمار > 10) {
            log_err(info->ctx, XKB_LOG_پیام_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    امن_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
Copy_کلید_نامها_مقصد_Keymap(بنية xkb_keymap *keymap, کلید_نامها_Info *info)
{
    إذا (darray_خالی(info->keycodes.کم) && darray_خالی(info->keycodes.بالا)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        أكد(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_کلید_کد = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_کلید_کد = 255;
        keymap->رقم_کلیدها = keymap->رقم_کلیدها_کم = keymap->max_کلید_کد + 1;
    } وإلا {
        أكد(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_کلید_کد = info->keycodes.min;
        keymap->max_کلید_کد = (darray_خالی(info->keycodes.بالا))
            ? darray_اندازه(info->keycodes.کم) - 1
            : darray_قلم(
                info->keycodes.بالا,
                darray_اندازه(info->keycodes.بالا) - 1
            ).keycode;
        keymap->رقم_کلیدها_کم = darray_اندازه(info->keycodes.کم);
        keymap->رقم_کلیدها = keymap->رقم_کلیدها_کم
                         + darray_اندازه(info->keycodes.بالا);
    }

    بنية xkb_کلید * ثابت کلیدها = حافظه_صفر_بده(keymap->رقم_کلیدها, حجم(*کلیدها));
    إذا (!کلیدها) {
        keymap->رقم_کلیدها = 0;
        keymap->min_کلید_کد = keymap->max_کلید_کد = XKB_KEYCODE_INVALID;
        أرجع false;
    }

    /* Low keycodes */
    لكل (xkb_keycode_t kc = keymap->min_کلید_کد; kc < keymap->رقم_کلیدها_کم; kc++)
        کلیدها[kc].keycode = kc;

    لكل (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_اندازه(info->keycodes.کم);
         kc++)
        کلیدها[kc].name = darray_قلم(info->keycodes.کم, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->رقم_کلیدها_کم; /* First high keycode index */
    ثابت بالا_Keycode_مدخل *مدخل;
    darray_foreach(مدخل, info->keycodes.بالا) {
        أكد(مدخل->name != XKB_ATOM_NONE);
        کلیدها[idx].keycode = مدخل->keycode;
        کلیدها[idx].name = مدخل->name;
        idx++;
    }

    keymap->کلیدها = کلیدها;
    أرجع true;
}

ساكن bool
CopyKeycodeNameLUT(بنية xkb_keymap *keymap, کلید_نامها_Info *info)
{
    Keycode_تطابق *تطابق;
    xkb_atom_t name;
    darray_enumerate(name, تطابق, info->keycodes.نامها) {
        إذا (!تطابق->یافت)
            استمر;

        إذا (تطابق->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ثابت Keycode_تطابق تطابق_real =
                keycode_store_lookup_name(&info->keycodes, تطابق->alias.real);
            إذا (!تطابق_real.یافت) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        کلید_Name_متن(info->ctx, name),
                        کلید_Name_متن(info->ctx, تطابق->alias.real));
                تطابق->یافت = false;
                استمر;
            }
            أكد(!تطابق_real.is_alias);
        } وإلا إذا (!تطابق->کلید.کم) {
            /* Update to final index in keymap::keys */
            تطابق->کلید.شاخص += keymap->رقم_کلیدها_کم;
        }
    }

    darray_shrink(info->keycodes.نامها);
    keymap->رقم_کلید_نامها = darray_اندازه(info->keycodes.نامها);
    darray_steal(info->keycodes.نامها, &keymap->کلید_نامها, عدم);
    darray_init(info->keycodes.نامها);
    أرجع true;
}

ساكن bool
Copy_Led_نامها_مقصد_Keymap(بنية xkb_keymap *keymap, کلید_نامها_Info *info)
{
    keymap->رقم_leds = info->رقم_led_نامها;
    لكل (xkb_led_شاخص_t idx = 0; idx < info->رقم_led_نامها; idx++) {
        LedNameInfo *ledi = &info->led_نامها[idx];

        إذا (ledi->name == XKB_ATOM_NONE)
            استمر;

        keymap->leds[idx].name = ledi->name;
    }

    أرجع true;
}

ساكن bool
Copy_کلید_نامها_Info_مقصد_Keymap(بنية xkb_keymap *keymap, کلید_نامها_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    إذا (!Copy_کلید_نامها_مقصد_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !Copy_Led_نامها_مقصد_Keymap(keymap, info))
        أرجع false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    إذا (!keymap->رقم_کلیدها ||
        keymap->min_کلید_کد > 0) {
        keymap->redirect_کلید_auto = 0;
    } وإلا {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        لكل (xkb_keycode_t k = keymap->رقم_کلیدها; k-- > keymap->رقم_کلیدها_کم;) {
            إذا (keycode > keymap->کلیدها[k].keycode)
                اكسر;
            /* Try next keycode downwards */
            keycode = keymap->کلیدها[k].keycode - 1;
        }
        keymap->redirect_کلید_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_امن(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    أرجع true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    کلید_نامها_Info info;

    Init_کلید_نامها_Info(&info, keymap_info, 0);

    إذا (file != عدم)
        HandleKeycodesFile(&info, file);

    إذا (info.error_شمار != 0)
        اذهب err_info;

    إذا (!Copy_کلید_نامها_Info_مقصد_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    Clear_کلید_نامها_Info(&info);
    أرجع true;

err_info:
    Clear_کلید_نامها_Info(&info);
    أرجع false;
}
