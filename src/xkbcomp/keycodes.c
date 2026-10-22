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
} لوړ_Keycode_ثبت;

تعريفنوع بنية {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) ټیټ;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(لوړ_Keycode_ثبت) لوړ;
    /**
     * name -> keycode mapping
     */
    darray(Keycode_سمون) نومونه;
} KeycodeStore;

ساكن مضمن فراغ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->ټیټ);
    darray_init(store->لوړ);
    darray_init(store->نومونه);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ساكن مضمن فراغ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->ټیټ);
    darray_free(store->لوړ);
    darray_free(store->نومونه);
}

ساكن مضمن فراغ
keycode_store_update_کلی(KeycodeStore *store, Keycode_سمون سمون, xkb_atom_t name)
{
    إذا (unlikely(!سمون.وموندل || سمون.is_alias)) {
        أرجع;
    } وإلا إذا (سمون.کلی.ټیټ) {
        /* assume valid index */
        أكد(سمون.کلی.شاخص < darray_اندازه(store->ټیټ));
        darray_توکی(store->ټیټ, سمون.کلی.شاخص) = name;
    } وإلا {
        /* assume valid index */
        أكد(سمون.کلی.شاخص < darray_اندازه(store->لوړ));
        darray_توکی(store->لوړ, سمون.کلی.شاخص).name = name;
    }

    /* Names lookup */
    إذا (name >= darray_اندازه(store->نومونه)) {
        darray_resize0(store->نومونه, name + 1);
    }
    darray_توکی(store->نومونه, name) = سمون;
}

ساكن bool
keycode_store_ورننباسه_کلی(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    إذا (name >= darray_اندازه(store->نومونه)) {
        darray_resize0(store->نومونه, name + 1);
    }
    إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        إذا (kc >= (xkb_keycode_t) darray_اندازه(store->ټیټ))
            darray_resize0(store->ټیټ, kc + 1);
        darray_توکی(store->ټیټ, kc) = name;
        إذا (kc < store->min)
            store->min = kc;
        darray_توکی(store->نومونه, name) = (Keycode_سمون) {
            .کلی = {
                .وموندل = true,
                .ټیټ = true,
                .is_alias = false,
                .شاخص = kc
            }
        };
    } وإلا {
        /* High keycode: insert into a sorted list */
        ثابت darray_اندازه_t idx = darray_اندازه(store->لوړ);
        إذا (idx && darray_توکی(store->لوړ, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_اندازه_t ښکتنی = 0;
            darray_اندازه_t پاسنی = idx;
            طالما (ښکتنی < پاسنی) {
                ثابت darray_اندازه_t mid = ښکتنی + (پاسنی - 1 - ښکتنی) / 2;
                ثابت لوړ_Keycode_ثبت * ثابت ثبت =
                    &darray_توکی(store->لوړ, mid);
                إذا (ثبت->keycode < kc) {
                    ښکتنی = mid + 1;
                } وإلا إذا (ثبت->keycode > kc) {
                    پاسنی = mid;
                } وإلا {
                    /* Unreachable: there is no repetition in the list */
                    أكد(ثبت->keycode != kc);
                }
            }
            أكد(ښکتنی < idx);
            أكد(darray_توکی(store->لوړ, ښکتنی).keycode > kc);

            /* Update references to entries that will be moved */
            لوړ_Keycode_ثبت *ثبت;
            darray_foreach_from(ثبت, store->لوړ, ښکتنی) {
                darray_توکی(store->نومونه, ثبت->name).کلی.شاخص++;
            }

            darray_ورننباسه(store->لوړ, ښکتنی,
                          (لوړ_Keycode_ثبت){.keycode = kc, .name = name});
            darray_توکی(store->نومونه, name) = (Keycode_سمون) {
                .کلی = {
                    .وموندل = true,
                    .ټیټ = false,
                    .is_alias = false,
                    .شاخص = ښکتنی
                }
            };
        } وإلا {
            /* Fast path: no need to sort */
            darray_append(store->لوړ,
                          (لوړ_Keycode_ثبت){.keycode = kc, .name = name});
            darray_توکی(store->نومونه, name) = (Keycode_سمون) {
                .کلی = {
                    .وموندل = true,
                    .ټیټ = false,
                    .is_alias = false,
                    .شاخص = idx
                }
            };
        }
        إذا (darray_تشه(store->ټیټ))
            store->min = darray_توکی(store->لوړ, 0).keycode;
    }
    أرجع true;
}

ساكن مضمن bool
keycode_store_ورننباسه_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    إذا (alias >= darray_اندازه(store->نومونه)) {
        darray_resize0(store->نومونه, alias + 1);
    }
    darray_توکی(store->نومونه, alias) = (Keycode_سمون) {
        .alias = {
            .وموندل = true,
            .is_alias = true,
            .real = real
        }
    };
    أرجع true;
}

ساكن مضمن bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_توکی(store->نومونه, alias).alias.real = real;
    أرجع true;
}

ساكن مضمن فراغ
keycode_store_delete_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    إذا (name < darray_اندازه(store->نومونه))
        darray_توکی(store->نومونه, name).وموندل = false;
}

ساكن فراغ
keycode_store_delete_کلی(KeycodeStore *store, ثابت Keycode_سمون سمون)
{
    إذا (unlikely(!سمون.وموندل || سمون.is_alias)) {
        أرجع;
    } وإلا إذا (سمون.کلی.ټیټ) {
        /* assume valid index */
        أكد(سمون.کلی.شاخص < darray_اندازه(store->ټیټ));
        darray_توکی(store->نومونه,
                    darray_توکی(store->ټیټ, سمون.کلی.شاخص)).وموندل = false;
        إذا (سمون.کلی.شاخص + 1u == darray_اندازه(store->ټیټ)) {
            /* Highest low keycode: shrink */
            إذا (store->min == سمون.کلی.شاخص) {
                /* No low keycode left */
                darray_اندازه(store->ټیټ) = 0;
            } وإلا {
                /* Look for previous defined low keycode */
                لكل (darray_اندازه_t idx = سمون.کلی.شاخص; idx > 0; idx--) {
                    إذا (darray_توکی(store->ټیټ, idx - 1) != XKB_ATOM_NONE) {
                        darray_اندازه(store->ټیټ) = idx;
                        اكسر;
                    }
                }
            }
        } وإلا {
            /* Lower keycode: reset */
            darray_توکی(store->ټیټ, سمون.کلی.شاخص) = XKB_ATOM_NONE;
        }
    } وإلا {
        أكد(سمون.کلی.شاخص < darray_اندازه(store->لوړ)); /* assume valid index */
        darray_توکی(store->نومونه,
                    darray_توکی(store->لوړ, سمون.کلی.شاخص).name).وموندل = false;
        darray_remove(store->لوړ, سمون.کلی.شاخص);
        /* Update LUT indexes of high codes after the deleted one, if any */
        Keycode_سمون *ثبت;
        darray_foreach(ثبت, store->نومونه) {
            إذا (ثبت->وموندل && !ثبت->is_alias && !ثبت->کلی.ټیټ &&
                ثبت->کلی.شاخص > سمون.کلی.شاخص) {
                ثبت->کلی.شاخص--;
            }
        }
    }

    /* Update bounds */
    إذا (darray_تشه(store->ټیټ)) {
        store->min = (darray_تشه(store->لوړ))
            ? XKB_KEYCODE_INVALID
            : darray_توکی(store->لوړ, 0).keycode;
    } وإلا {
        لكل (xkb_keycode_t kc = store->min; kc < darray_اندازه(store->ټیټ); kc++) {
            إذا (darray_توکی(store->ټیټ, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                اكسر;
            }
        }
    }
}

ساكن مضمن xkb_keycode_t
keycode_store_get_keycode(ثابت KeycodeStore *store, Keycode_سمون سمون)
{
    إذا (!سمون.وموندل || سمون.is_alias) {
        أرجع XKB_KEYCODE_INVALID;
    } وإلا إذا (سمون.کلی.ټیټ) {
        /* assume valid index */
        أكد(سمون.کلی.شاخص < darray_اندازه(store->ټیټ));
        أرجع (xkb_keycode_t) سمون.کلی.شاخص;
    } وإلا {
        /* assume valid index */
        أكد(سمون.کلی.شاخص < darray_اندازه(store->لوړ));
        أرجع darray_توکی(store->لوړ, سمون.کلی.شاخص).keycode;
    }
}

ساكن مضمن xkb_atom_t
keycode_store_get_کلی_name(ثابت KeycodeStore *store, Keycode_سمون سمون)
{
    إذا (!سمون.وموندل || سمون.is_alias) {
        أرجع XKB_ATOM_NONE;
    } وإلا إذا (سمون.کلی.ټیټ) {
        /* assume valid index */
        أكد(سمون.کلی.شاخص < darray_اندازه(store->ټیټ));
        أرجع darray_توکی(store->ټیټ, سمون.کلی.شاخص);
    } وإلا {
        /* assume valid index */
        أكد(سمون.کلی.شاخص < darray_اندازه(store->لوړ));
        أرجع darray_توکی(store->لوړ, سمون.کلی.شاخص).name;
    }
}

ساكن Keycode_سمون
keycode_store_lookup_keycode(ثابت KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    إذا (kc < (xkb_keycode_t) darray_اندازه(store->ټیټ)) {
        أرجع (Keycode_سمون) {
            .کلی = {
                .وموندل = true,
                .ټیټ = true,
                .is_alias = false,
                .شاخص = kc
            }
        };
    } وإلا إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        أرجع (Keycode_سمون) { .وموندل = false };
    }

    /* High keycodes: use binary search */
    darray_اندازه_t ښکتنی = 0;
    darray_اندازه_t پاسنی = darray_اندازه(store->لوړ);
    طالما (ښکتنی < پاسنی) {
        ثابت darray_اندازه_t mid = ښکتنی + (پاسنی - 1 - ښکتنی) / 2;
        لوړ_Keycode_ثبت * ثابت ثبت = &darray_توکی(store->لوړ, mid);
        إذا (ثبت->keycode < kc) {
            ښکتنی = mid + 1;
        } وإلا إذا (ثبت->keycode > kc) {
            پاسنی = mid;
        } وإلا {
            أرجع (Keycode_سمون) {
                .کلی = {
                    .وموندل = true,
                    .ټیټ = false,
                    .is_alias = false,
                    .شاخص = mid
                }
            };
        }
    }

    أرجع (Keycode_سمون) { .وموندل = false };
}

ساكن Keycode_سمون
keycode_store_lookup_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    إذا (name >= darray_اندازه(store->نومونه)) {
        أرجع (Keycode_سمون) { .وموندل = false, .is_alias = false };
    } وإلا {
        أرجع darray_توکی(store->نومونه, name);
    }
}

/***====================================================================***/

تعريفنوع بنية {
    تعداد یوځای_ډېرپېښ یوځای;

    xkb_atom_t name;
} LedNameInfo;

تعريفنوع بنية {
    حرف *name;
    صحيح error_شمېر;
    بینښه صحيح include_ژوروالی;

    KeycodeStore keycodes;
    LedNameInfo led_نومونه[XKB_MAX_LEDS];
    xkb_led_شاخص_t عدد_led_نومونه;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} کلی_نومونه_Info;

/***====================================================================***/

ساكن LedNameInfo *
پیدا_کړه_Led_By_Name(کلی_نومونه_Info *info, xkb_atom_t name,
              xkb_led_شاخص_t *idx_بهر)
{
    لكل (xkb_led_شاخص_t idx = 0; idx < info->عدد_led_نومونه; idx++) {
        LedNameInfo *ledi = &info->led_نومونه[idx];
        إذا (ledi->name == name) {
            *idx_بهر = idx;
            أرجع ledi;
        }
    }

    أرجع عدم;
}

ساكن bool
جمع_کړه_Led_Name(کلی_نومونه_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_شاخص_t new_idx, bool report)
{
    xkb_led_شاخص_t old_idx;
    LedNameInfo *old;
    ثابت bool replace = (new->یوځای != یوځای_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = پیدا_کړه_Led_By_Name(info, new->name, &old_idx);
    إذا (old) {
        إذا (old_idx == new_idx) {
            إذا (report)
                log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_متن(info->ctx, new->name));
            أرجع true;
        }

        إذا (report) {
            xkb_led_شاخص_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_شاخص_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
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

    إذا (new_idx >= info->عدد_led_نومونه)
        info->عدد_led_نومونه = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_نومونه[new_idx];
    إذا (old->name != XKB_ATOM_NONE) {
        إذا (report) {
            ثابت xkb_atom_t use    = (replace ? new->name : old->name);
            ثابت xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
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
Clear_کلی_نومونه_Info(کلی_نومونه_Info *info)
{
    حرر(info->name);
    keycode_store_free(&info->keycodes);
}

ساكن فراغ
Init_کلی_نومونه_Info(کلی_نومونه_Info *info, ثابت بنية xkb_keymap_info *keymap_info,
                 بینښه صحيح include_ژوروالی)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_ژوروالی = include_ژوروالی;
    keycode_store_init(&info->keycodes);
}

ساكن bool
جمع_کړه_کلی_Name(کلی_نومونه_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           تعداد یوځای_ډېرپېښ یوځای, bool report)
{
#که_تعریف_شوی NDEBUG
    ثابت
#د_که_پای
    Keycode_سمون سمون_name = keycode_store_lookup_name(&info->keycodes, name);
    إذا (سمون_name.وموندل) {
        ثابت bool clobber = (یوځای != یوځای_AUGMENT);

        إذا (سمون_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            إذا (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         کلی_Name_متن(info->ctx, name),
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
#که_نه_تعریف_شوی NDEBUG
                سمون_name.وموندل = false;
#د_که_پای
            } وإلا {
                أرجع true;
            }
        } وإلا {
            ثابت xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, سمون_name);
            أكد(old_kc != XKB_KEYCODE_INVALID);
            إذا (old_kc != kc) {
                /* There is already a different key with this name. */

                إذا (report) {
                    ثابت xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ثابت xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            کلی_Name_متن(info->ctx, name), use, ignore);
                }

                إذا (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_کلی(&info->keycodes, سمون_name);
                } وإلا {
                    أرجع true;
                }
            }
        }
    }

    ثابت Keycode_سمون سمون_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ثابت xkb_atom_t old_name = keycode_store_get_کلی_name(&info->keycodes, سمون_kc);
    إذا (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        إذا (old_name == name) {
            أكد(keycode_store_get_keycode(&info->keycodes, سمون_name) == kc);
            إذا (report)
                log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         کلی_Name_متن(info->ctx, old_name), kc);
            أرجع true;
        }

        ثابت bool clobber = (یوځای != یوځای_AUGMENT);
        إذا (report) {
            ثابت حرف* ثابت kname     = کلی_Name_متن(info->ctx, name);
            ثابت حرف* ثابت old_kname = کلی_Name_متن(info->ctx, old_name);
            ثابت حرف* ثابت use    = (clobber) ? kname : old_kname;
            ثابت حرف* ثابت ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_پیغام_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        إذا (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_کلی(&info->keycodes, سمون_kc, name);
        }
    } وإلا {
        /* No previous keycode */
        إذا (!keycode_store_ورننباسه_کلی(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ویش_FAILURE_2,
                    "Cannot add keycode\n");
            أرجع false;
        }
    }

    أرجع true;
}

/***====================================================================***/

ساكن bool
HandleAliasDef(کلی_نومونه_Info *info, ثابت کلی_Alias_Def *def, bool report);

ساكن فراغ
یوځای_Keycode_Stores(کلی_نومونه_Info *into, کلی_نومونه_Info *from,
                   تعداد یوځای_ډېرپېښ یوځای, bool report)
{
    إذا (darray_تشه(into->keycodes.ټیټ) && darray_تشه(into->keycodes.لوړ) &&
        darray_تشه(into->keycodes.نومونه)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.ټیټ);
        darray_init(from->keycodes.لوړ);
        darray_init(from->keycodes.نومونه);
    } وإلا {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        لكل (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_اندازه(from->keycodes.ټیټ);
             kc++) {
            /* Skip undefined keycodes */
            ثابت xkb_atom_t name = darray_توکی(from->keycodes.ټیټ, kc);
            إذا (name == XKB_ATOM_NONE)
                استمر;

            إذا (!جمع_کړه_کلی_Name(into, kc, name, یوځای, report))
                into->error_شمېر++;
        }

        /* High keycodes */
        ثابت لوړ_Keycode_ثبت *new;
        darray_foreach(new, from->keycodes.لوړ) {
            أكد(new->name != XKB_ATOM_NONE);
            إذا (!جمع_کړه_کلی_Name(into, new->keycode, new->name, یوځای, report))
                into->error_شمېر++;
        }

        /* Aliases. */
        Keycode_سمون *سمون;
        xkb_atom_t alias;
        darray_enumerate(alias, سمون, from->keycodes.نومونه) {
            إذا (!سمون->وموندل || !سمون->is_alias)
                استمر;

            ثابت کلی_Alias_Def def = {
                .یوځای = یوځای,
                .alias = alias,
                .real = سمون->alias.real
            };
            إذا (!HandleAliasDef(into, &def, report))
                into->error_شمېر++;
        }
    }
}

ساكن فراغ
یوځای_Included_Keycodes(کلی_نومونه_Info *into, کلی_نومونه_Info *from,
                      تعداد یوځای_ډېرپېښ یوځای, bool report)
{
    إذا (from->error_شمېر > 0) {
        into->error_شمېر += from->error_شمېر;
        أرجع;
    }

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    یوځای_Keycode_Stores(into, from, یوځای, report);

    /* Merge LED names. */
    إذا (into->عدد_led_نومونه == 0) {
        حافظه_نقل(into->led_نومونه, from->led_نومونه,
               حجم(*from->led_نومونه) * from->عدد_led_نومونه);
        into->عدد_led_نومونه = from->عدد_led_نومونه;
        from->عدد_led_نومونه = 0;
    }
    وإلا {
        لكل (xkb_led_شاخص_t idx = 0; idx < from->عدد_led_نومونه; idx++) {
            LedNameInfo *ledi = &from->led_نومونه[idx];

            إذا (ledi->name == XKB_ATOM_NONE)
                استمر;

            ledi->یوځای = یوځای;
            إذا (!جمع_کړه_Led_Name(into, false, ledi, idx, report))
                into->error_شمېر++;
        }
    }
}

ساكن فراغ
HandleKeycodesFile(کلی_نومونه_Info *info, XkbFile *file);

ساكن bool
HandleIncludeKeycodes(کلی_نومونه_Info *info, IncludeStmt *include, bool report)
{
    کلی_نومونه_Info included;

    إذا (Exceeds_Include_Max_ژوروالی(info->ctx, info->include_ژوروالی)) {
        info->error_شمېر += 10;
        أرجع false;
    }

    Init_کلی_نومونه_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->بل_incl) {
        کلی_نومونه_Info بل_incl;
        XkbFile *file;

        حرف لار[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  لار, حجم(لار));
        إذا (!file) {
            info->error_شمېر += 10;
            Clear_کلی_نومونه_Info(&included);
            أرجع false;
        }

        Init_کلی_نومونه_Info(&بل_incl, info->keymap_info, info->include_ژوروالی + 1);

        HandleKeycodesFile(&بل_incl, file);

        یوځای_Included_Keycodes(&included, &بل_incl, stmt->یوځای, report);

        Clear_کلی_نومونه_Info(&بل_incl);
        FreeXkbFile(file);
    }

    یوځای_Included_Keycodes(info, &included, include->یوځای, report);
    Clear_کلی_نومونه_Info(&included);

    أرجع (info->error_شمېر == 0);
}

ساكن bool
HandleKeycodeDef(کلی_نومونه_Info *info, KeycodeDef *stmt, bool report)
{
    إذا (stmt->ارزښت < 0 || stmt->ارزښت > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->ارزښت, XKB_KEYCODE_MAX);
        أرجع false;
    }

    أرجع جمع_کړه_کلی_Name(info, (xkb_keycode_t) stmt->ارزښت,
                      stmt->name, stmt->یوځای, report);
}

ساكن bool
HandleAliasDef(کلی_نومونه_Info *info, ثابت کلی_Alias_Def *def, bool report)
{
    ثابت Keycode_سمون سمون_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    إذا (سمون_name.وموندل) {
        ثابت bool clobber = (def->یوځای != یوځای_AUGMENT);
        إذا (سمون_name.is_alias) {
            إذا (def->real == سمون_name.alias.real) {
                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             کلی_Name_متن(info->ctx, def->alias),
                             کلی_Name_متن(info->ctx, def->real));
            } وإلا {
                ثابت xkb_atom_t use = (clobber)
                    ? def->real
                    : سمون_name.alias.real;
                ثابت xkb_atom_t ignore = (clobber)
                    ? سمون_name.alias.real
                    : def->real;

                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             کلی_Name_متن(info->ctx, def->alias),
                             کلی_Name_متن(info->ctx, use),
                             کلی_Name_متن(info->ctx, ignore));

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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلی_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         کلی_Name_متن(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            إذا (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_کلی(&info->keycodes, سمون_name);
            } وإلا {
                أرجع true;
            }
        }
    }

    أرجع keycode_store_ورننباسه_alias(&info->keycodes, def->alias, def->real);
}

ساكن bool
Handle_کلی_Name_Var(کلی_نومونه_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *ډګر_2;
    بیان_Def *arrayNdx;

    إذا (!بیان_Resolve_Lhs(info->ctx, stmt->name, &elem, &ډګر_2, &arrayNdx))
        أرجع false;

    إذا (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, ډګر_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    إذا (!istreq(ډګر_2, "minimum") && !istreq(ډګر_2, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ډګر,
                "Default defined for unknown field \"%s\"; Ignored\n", ډګر_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    إذا (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", ډګر_2, "defaults");
        أرجع !(info->keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH);
    }

    int64_t val = 0;
    إذا (!بیان_Resolve_Integer(info->ctx, stmt->ارزښت, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_ډګر_TYPE, "keycodes",
                      ډګر_2, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        أرجع !(info->keymap_info->strict & PARSER_NO_ډګر_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    أرجع true;
}

ساكن bool
HandleLedNameDef(کلی_نومونه_Info *info, LedNameDef *def, bool report)
{
    إذا (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_شمېر++;
        log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!بیان_Resolve_String(info->ctx, def->name, &name)) {
        حرف buf[20];
        snprintf(buf, حجم(buf), "%"PRId64, def->ndx);
        info->error_شمېر++;
        أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_ډګر_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.یوځای = def->یوځای, .name = name};
    أرجع جمع_کړه_Led_Name(info, true, &ledi, (xkb_led_شاخص_t) def->ndx - 1, report);
}

ساكن فراغ
HandleKeycodesFile(کلی_نومونه_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report_same_file = verbosity > 0;
    ثابت bool report_include   = verbosity > 7;

    حرر(info->name);
    info->name = strdup_خوندي(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->بل) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            اكسر;
        حالة STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            اكسر;
        حالة STMT_ALIAS:
            ok = HandleAliasDef(info, (کلی_Alias_Def *) stmt, report_same_file);
            اكسر;
        حالة STMT_VAR:
            ok = Handle_کلی_Name_Var(info, (VarDef *) stmt);
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
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_منزل_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_شمېر++;

        إذا (info->error_شمېر > 10) {
            log_err(info->ctx, XKB_LOG_پیغام_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    خوندي_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
Copy_کلی_نومونه_منزل_Keymap(بنية xkb_keymap *keymap, کلی_نومونه_Info *info)
{
    إذا (darray_تشه(info->keycodes.ټیټ) && darray_تشه(info->keycodes.لوړ)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        أكد(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_کلی_کوډ = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_کلی_کوډ = 255;
        keymap->عدد_کلیګانې = keymap->عدد_کلیګانې_ټیټ = keymap->max_کلی_کوډ + 1;
    } وإلا {
        أكد(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_کلی_کوډ = info->keycodes.min;
        keymap->max_کلی_کوډ = (darray_تشه(info->keycodes.لوړ))
            ? darray_اندازه(info->keycodes.ټیټ) - 1
            : darray_توکی(
                info->keycodes.لوړ,
                darray_اندازه(info->keycodes.لوړ) - 1
            ).keycode;
        keymap->عدد_کلیګانې_ټیټ = darray_اندازه(info->keycodes.ټیټ);
        keymap->عدد_کلیګانې = keymap->عدد_کلیګانې_ټیټ
                         + darray_اندازه(info->keycodes.لوړ);
    }

    بنية xkb_کلی * ثابت کلیګانې = حافظه_صفر_ورکړه(keymap->عدد_کلیګانې, حجم(*کلیګانې));
    إذا (!کلیګانې) {
        keymap->عدد_کلیګانې = 0;
        keymap->min_کلی_کوډ = keymap->max_کلی_کوډ = XKB_KEYCODE_INVALID;
        أرجع false;
    }

    /* Low keycodes */
    لكل (xkb_keycode_t kc = keymap->min_کلی_کوډ; kc < keymap->عدد_کلیګانې_ټیټ; kc++)
        کلیګانې[kc].keycode = kc;

    لكل (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_اندازه(info->keycodes.ټیټ);
         kc++)
        کلیګانې[kc].name = darray_توکی(info->keycodes.ټیټ, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->عدد_کلیګانې_ټیټ; /* First high keycode index */
    ثابت لوړ_Keycode_ثبت *ثبت;
    darray_foreach(ثبت, info->keycodes.لوړ) {
        أكد(ثبت->name != XKB_ATOM_NONE);
        کلیګانې[idx].keycode = ثبت->keycode;
        کلیګانې[idx].name = ثبت->name;
        idx++;
    }

    keymap->کلیګانې = کلیګانې;
    أرجع true;
}

ساكن bool
CopyKeycodeNameLUT(بنية xkb_keymap *keymap, کلی_نومونه_Info *info)
{
    Keycode_سمون *سمون;
    xkb_atom_t name;
    darray_enumerate(name, سمون, info->keycodes.نومونه) {
        إذا (!سمون->وموندل)
            استمر;

        إذا (سمون->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ثابت Keycode_سمون سمون_real =
                keycode_store_lookup_name(&info->keycodes, سمون->alias.real);
            إذا (!سمون_real.وموندل) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        کلی_Name_متن(info->ctx, name),
                        کلی_Name_متن(info->ctx, سمون->alias.real));
                سمون->وموندل = false;
                استمر;
            }
            أكد(!سمون_real.is_alias);
        } وإلا إذا (!سمون->کلی.ټیټ) {
            /* Update to final index in keymap::keys */
            سمون->کلی.شاخص += keymap->عدد_کلیګانې_ټیټ;
        }
    }

    darray_shrink(info->keycodes.نومونه);
    keymap->عدد_کلی_نومونه = darray_اندازه(info->keycodes.نومونه);
    darray_steal(info->keycodes.نومونه, &keymap->کلی_نومونه, عدم);
    darray_init(info->keycodes.نومونه);
    أرجع true;
}

ساكن bool
Copy_Led_نومونه_منزل_Keymap(بنية xkb_keymap *keymap, کلی_نومونه_Info *info)
{
    keymap->عدد_leds = info->عدد_led_نومونه;
    لكل (xkb_led_شاخص_t idx = 0; idx < info->عدد_led_نومونه; idx++) {
        LedNameInfo *ledi = &info->led_نومونه[idx];

        إذا (ledi->name == XKB_ATOM_NONE)
            استمر;

        keymap->leds[idx].name = ledi->name;
    }

    أرجع true;
}

ساكن bool
Copy_کلی_نومونه_Info_منزل_Keymap(بنية xkb_keymap *keymap, کلی_نومونه_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    إذا (!Copy_کلی_نومونه_منزل_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !Copy_Led_نومونه_منزل_Keymap(keymap, info))
        أرجع false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    إذا (!keymap->عدد_کلیګانې ||
        keymap->min_کلی_کوډ > 0) {
        keymap->redirect_کلی_auto = 0;
    } وإلا {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        لكل (xkb_keycode_t k = keymap->عدد_کلیګانې; k-- > keymap->عدد_کلیګانې_ټیټ;) {
            إذا (keycode > keymap->کلیګانې[k].keycode)
                اكسر;
            /* Try next keycode downwards */
            keycode = keymap->کلیګانې[k].keycode - 1;
        }
        keymap->redirect_کلی_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_خوندي(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    أرجع true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    کلی_نومونه_Info info;

    Init_کلی_نومونه_Info(&info, keymap_info, 0);

    إذا (file != عدم)
        HandleKeycodesFile(&info, file);

    إذا (info.error_شمېر != 0)
        اذهب err_info;

    إذا (!Copy_کلی_نومونه_Info_منزل_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    Clear_کلی_نومونه_Info(&info);
    أرجع true;

err_info:
    Clear_کلی_نومونه_Info(&info);
    أرجع false;
}
