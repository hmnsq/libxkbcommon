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
} مٿاهون_Keycode_داخلا;

تعريفنوع بنية {
    /** Minimum keycode */
    xkb_keycode_t min;
    /**
     * keycode -> name mapping, stored contiguously
     * keycode <= XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(xkb_atom_t) هيٺيون;
    /**
     * keycode -> name mapping, sorted entries, possibly noncontiguous keycodes
     * keycode > XKB_KEYCODE_MAX_CONTIGUOUS
     */
    darray(مٿاهون_Keycode_داخلا) مٿاهون;
    /**
     * name -> keycode mapping
     */
    darray(Keycode_ميلاپ) نالا;
} KeycodeStore;

ساكن مضمن فراغ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->هيٺيون);
    darray_init(store->مٿاهون);
    darray_init(store->نالا);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ساكن مضمن فراغ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->هيٺيون);
    darray_free(store->مٿاهون);
    darray_free(store->نالا);
}

ساكن مضمن فراغ
keycode_store_update_ڪنجي(KeycodeStore *store, Keycode_ميلاپ ميلاپ, xkb_atom_t name)
{
    إذا (unlikely(!ميلاپ.مليو || ميلاپ.is_alias)) {
        أرجع;
    } وإلا إذا (ميلاپ.ڪنجي.هيٺيون) {
        /* assume valid index */
        أكد(ميلاپ.ڪنجي.اشاريو < darray_سائيز(store->هيٺيون));
        darray_شي(store->هيٺيون, ميلاپ.ڪنجي.اشاريو) = name;
    } وإلا {
        /* assume valid index */
        أكد(ميلاپ.ڪنجي.اشاريو < darray_سائيز(store->مٿاهون));
        darray_شي(store->مٿاهون, ميلاپ.ڪنجي.اشاريو).name = name;
    }

    /* Names lookup */
    إذا (name >= darray_سائيز(store->نالا)) {
        darray_resize0(store->نالا, name + 1);
    }
    darray_شي(store->نالا, name) = ميلاپ;
}

ساكن bool
keycode_store_وجهو_ڪنجي(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    إذا (name >= darray_سائيز(store->نالا)) {
        darray_resize0(store->نالا, name + 1);
    }
    إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        إذا (kc >= (xkb_keycode_t) darray_سائيز(store->هيٺيون))
            darray_resize0(store->هيٺيون, kc + 1);
        darray_شي(store->هيٺيون, kc) = name;
        إذا (kc < store->min)
            store->min = kc;
        darray_شي(store->نالا, name) = (Keycode_ميلاپ) {
            .ڪنجي = {
                .مليو = true,
                .هيٺيون = true,
                .is_alias = false,
                .اشاريو = kc
            }
        };
    } وإلا {
        /* High keycode: insert into a sorted list */
        ثابت darray_سائيز_t idx = darray_سائيز(store->مٿاهون);
        إذا (idx && darray_شي(store->مٿاهون, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_سائيز_t هيٺاهون = 0;
            darray_سائيز_t مٿانهون = idx;
            طالما (هيٺاهون < مٿانهون) {
                ثابت darray_سائيز_t mid = هيٺاهون + (مٿانهون - 1 - هيٺاهون) / 2;
                ثابت مٿاهون_Keycode_داخلا * ثابت داخلا =
                    &darray_شي(store->مٿاهون, mid);
                إذا (داخلا->keycode < kc) {
                    هيٺاهون = mid + 1;
                } وإلا إذا (داخلا->keycode > kc) {
                    مٿانهون = mid;
                } وإلا {
                    /* Unreachable: there is no repetition in the list */
                    أكد(داخلا->keycode != kc);
                }
            }
            أكد(هيٺاهون < idx);
            أكد(darray_شي(store->مٿاهون, هيٺاهون).keycode > kc);

            /* Update references to entries that will be moved */
            مٿاهون_Keycode_داخلا *داخلا;
            darray_foreach_from(داخلا, store->مٿاهون, هيٺاهون) {
                darray_شي(store->نالا, داخلا->name).ڪنجي.اشاريو++;
            }

            darray_وجهو(store->مٿاهون, هيٺاهون,
                          (مٿاهون_Keycode_داخلا){.keycode = kc, .name = name});
            darray_شي(store->نالا, name) = (Keycode_ميلاپ) {
                .ڪنجي = {
                    .مليو = true,
                    .هيٺيون = false,
                    .is_alias = false,
                    .اشاريو = هيٺاهون
                }
            };
        } وإلا {
            /* Fast path: no need to sort */
            darray_append(store->مٿاهون,
                          (مٿاهون_Keycode_داخلا){.keycode = kc, .name = name});
            darray_شي(store->نالا, name) = (Keycode_ميلاپ) {
                .ڪنجي = {
                    .مليو = true,
                    .هيٺيون = false,
                    .is_alias = false,
                    .اشاريو = idx
                }
            };
        }
        إذا (darray_خالي_جايون(store->هيٺيون))
            store->min = darray_شي(store->مٿاهون, 0).keycode;
    }
    أرجع true;
}

ساكن مضمن bool
keycode_store_وجهو_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    إذا (alias >= darray_سائيز(store->نالا)) {
        darray_resize0(store->نالا, alias + 1);
    }
    darray_شي(store->نالا, alias) = (Keycode_ميلاپ) {
        .alias = {
            .مليو = true,
            .is_alias = true,
            .real = real
        }
    };
    أرجع true;
}

ساكن مضمن bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_شي(store->نالا, alias).alias.real = real;
    أرجع true;
}

ساكن مضمن فراغ
keycode_store_delete_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    إذا (name < darray_سائيز(store->نالا))
        darray_شي(store->نالا, name).مليو = false;
}

ساكن فراغ
keycode_store_delete_ڪنجي(KeycodeStore *store, ثابت Keycode_ميلاپ ميلاپ)
{
    إذا (unlikely(!ميلاپ.مليو || ميلاپ.is_alias)) {
        أرجع;
    } وإلا إذا (ميلاپ.ڪنجي.هيٺيون) {
        /* assume valid index */
        أكد(ميلاپ.ڪنجي.اشاريو < darray_سائيز(store->هيٺيون));
        darray_شي(store->نالا,
                    darray_شي(store->هيٺيون, ميلاپ.ڪنجي.اشاريو)).مليو = false;
        إذا (ميلاپ.ڪنجي.اشاريو + 1u == darray_سائيز(store->هيٺيون)) {
            /* Highest low keycode: shrink */
            إذا (store->min == ميلاپ.ڪنجي.اشاريو) {
                /* No low keycode left */
                darray_سائيز(store->هيٺيون) = 0;
            } وإلا {
                /* Look for previous defined low keycode */
                لكل (darray_سائيز_t idx = ميلاپ.ڪنجي.اشاريو; idx > 0; idx--) {
                    إذا (darray_شي(store->هيٺيون, idx - 1) != XKB_ATOM_NONE) {
                        darray_سائيز(store->هيٺيون) = idx;
                        اكسر;
                    }
                }
            }
        } وإلا {
            /* Lower keycode: reset */
            darray_شي(store->هيٺيون, ميلاپ.ڪنجي.اشاريو) = XKB_ATOM_NONE;
        }
    } وإلا {
        أكد(ميلاپ.ڪنجي.اشاريو < darray_سائيز(store->مٿاهون)); /* assume valid index */
        darray_شي(store->نالا,
                    darray_شي(store->مٿاهون, ميلاپ.ڪنجي.اشاريو).name).مليو = false;
        darray_remove(store->مٿاهون, ميلاپ.ڪنجي.اشاريو);
        /* Update LUT indexes of high codes after the deleted one, if any */
        Keycode_ميلاپ *داخلا;
        darray_foreach(داخلا, store->نالا) {
            إذا (داخلا->مليو && !داخلا->is_alias && !داخلا->ڪنجي.هيٺيون &&
                داخلا->ڪنجي.اشاريو > ميلاپ.ڪنجي.اشاريو) {
                داخلا->ڪنجي.اشاريو--;
            }
        }
    }

    /* Update bounds */
    إذا (darray_خالي_جايون(store->هيٺيون)) {
        store->min = (darray_خالي_جايون(store->مٿاهون))
            ? XKB_KEYCODE_INVALID
            : darray_شي(store->مٿاهون, 0).keycode;
    } وإلا {
        لكل (xkb_keycode_t kc = store->min; kc < darray_سائيز(store->هيٺيون); kc++) {
            إذا (darray_شي(store->هيٺيون, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                اكسر;
            }
        }
    }
}

ساكن مضمن xkb_keycode_t
keycode_store_get_keycode(ثابت KeycodeStore *store, Keycode_ميلاپ ميلاپ)
{
    إذا (!ميلاپ.مليو || ميلاپ.is_alias) {
        أرجع XKB_KEYCODE_INVALID;
    } وإلا إذا (ميلاپ.ڪنجي.هيٺيون) {
        /* assume valid index */
        أكد(ميلاپ.ڪنجي.اشاريو < darray_سائيز(store->هيٺيون));
        أرجع (xkb_keycode_t) ميلاپ.ڪنجي.اشاريو;
    } وإلا {
        /* assume valid index */
        أكد(ميلاپ.ڪنجي.اشاريو < darray_سائيز(store->مٿاهون));
        أرجع darray_شي(store->مٿاهون, ميلاپ.ڪنجي.اشاريو).keycode;
    }
}

ساكن مضمن xkb_atom_t
keycode_store_get_ڪنجي_name(ثابت KeycodeStore *store, Keycode_ميلاپ ميلاپ)
{
    إذا (!ميلاپ.مليو || ميلاپ.is_alias) {
        أرجع XKB_ATOM_NONE;
    } وإلا إذا (ميلاپ.ڪنجي.هيٺيون) {
        /* assume valid index */
        أكد(ميلاپ.ڪنجي.اشاريو < darray_سائيز(store->هيٺيون));
        أرجع darray_شي(store->هيٺيون, ميلاپ.ڪنجي.اشاريو);
    } وإلا {
        /* assume valid index */
        أكد(ميلاپ.ڪنجي.اشاريو < darray_سائيز(store->مٿاهون));
        أرجع darray_شي(store->مٿاهون, ميلاپ.ڪنجي.اشاريو).name;
    }
}

ساكن Keycode_ميلاپ
keycode_store_lookup_keycode(ثابت KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    إذا (kc < (xkb_keycode_t) darray_سائيز(store->هيٺيون)) {
        أرجع (Keycode_ميلاپ) {
            .ڪنجي = {
                .مليو = true,
                .هيٺيون = true,
                .is_alias = false,
                .اشاريو = kc
            }
        };
    } وإلا إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        أرجع (Keycode_ميلاپ) { .مليو = false };
    }

    /* High keycodes: use binary search */
    darray_سائيز_t هيٺاهون = 0;
    darray_سائيز_t مٿانهون = darray_سائيز(store->مٿاهون);
    طالما (هيٺاهون < مٿانهون) {
        ثابت darray_سائيز_t mid = هيٺاهون + (مٿانهون - 1 - هيٺاهون) / 2;
        مٿاهون_Keycode_داخلا * ثابت داخلا = &darray_شي(store->مٿاهون, mid);
        إذا (داخلا->keycode < kc) {
            هيٺاهون = mid + 1;
        } وإلا إذا (داخلا->keycode > kc) {
            مٿانهون = mid;
        } وإلا {
            أرجع (Keycode_ميلاپ) {
                .ڪنجي = {
                    .مليو = true,
                    .هيٺيون = false,
                    .is_alias = false,
                    .اشاريو = mid
                }
            };
        }
    }

    أرجع (Keycode_ميلاپ) { .مليو = false };
}

ساكن Keycode_ميلاپ
keycode_store_lookup_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    إذا (name >= darray_سائيز(store->نالا)) {
        أرجع (Keycode_ميلاپ) { .مليو = false, .is_alias = false };
    } وإلا {
        أرجع darray_شي(store->نالا, name);
    }
}

/***====================================================================***/

تعريفنوع بنية {
    تعداد ملايو_گهڻائي ملايو;

    xkb_atom_t name;
} LedNameInfo;

تعريفنوع بنية {
    حرف *name;
    صحيح error_ڳڻپ;
    بنانشان صحيح include_اونهائي;

    KeycodeStore keycodes;
    LedNameInfo led_نالا[XKB_MAX_LEDS];
    xkb_led_اشاريو_t انگ_led_نالا;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} ڪنجي_نالا_Info;

/***====================================================================***/

ساكن LedNameInfo *
ڳوليو_Led_By_Name(ڪنجي_نالا_Info *info, xkb_atom_t name,
              xkb_led_اشاريو_t *idx_ٻاهريون)
{
    لكل (xkb_led_اشاريو_t idx = 0; idx < info->انگ_led_نالا; idx++) {
        LedNameInfo *ledi = &info->led_نالا[idx];
        إذا (ledi->name == name) {
            *idx_ٻاهريون = idx;
            أرجع ledi;
        }
    }

    أرجع عدم;
}

ساكن bool
جوڙيو_Led_Name(ڪنجي_نالا_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_اشاريو_t new_idx, bool report)
{
    xkb_led_اشاريو_t old_idx;
    LedNameInfo *old;
    ثابت bool replace = (new->ملايو != ملايو_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = ڳوليو_Led_By_Name(info, new->name, &old_idx);
    إذا (old) {
        إذا (old_idx == new_idx) {
            إذا (report)
                log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_متن(info->ctx, new->name));
            أرجع true;
        }

        إذا (report) {
            xkb_led_اشاريو_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_اشاريو_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
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

    إذا (new_idx >= info->انگ_led_نالا)
        info->انگ_led_نالا = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_نالا[new_idx];
    إذا (old->name != XKB_ATOM_NONE) {
        إذا (report) {
            ثابت xkb_atom_t use    = (replace ? new->name : old->name);
            ثابت xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
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
Clear_ڪنجي_نالا_Info(ڪنجي_نالا_Info *info)
{
    حرر(info->name);
    keycode_store_free(&info->keycodes);
}

ساكن فراغ
Init_ڪنجي_نالا_Info(ڪنجي_نالا_Info *info, ثابت بنية xkb_keymap_info *keymap_info,
                 بنانشان صحيح include_اونهائي)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_اونهائي = include_اونهائي;
    keycode_store_init(&info->keycodes);
}

ساكن bool
جوڙيو_ڪنجي_Name(ڪنجي_نالا_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           تعداد ملايو_گهڻائي ملايو, bool report)
{
#جيڪڏهن_وصف_ٿيل NDEBUG
    ثابت
#جيڪڏهن_پورو
    Keycode_ميلاپ ميلاپ_name = keycode_store_lookup_name(&info->keycodes, name);
    إذا (ميلاپ_name.مليو) {
        ثابت bool clobber = (ملايو != ملايو_AUGMENT);

        إذا (ميلاپ_name.is_alias) {
            /*
             * There is already an alias with this name.
             *
             * Contrary to Xorg’s xkbcomp, keys and aliases share the same
             * namespace. So we need to resolve name conflicts as they arise,
             * while xkbcomp will resolve them just before copying aliases into
             * the keymap.
             */

            إذا (report) {
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_NAME,
                         "Key name %s already assigned to an alias; "
                         "Using %s, ignoring %s\n",
                         ڪنجي_Name_متن(info->ctx, name),
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
#جيڪڏهن_وصف_نه_ٿيل NDEBUG
                ميلاپ_name.مليو = false;
#جيڪڏهن_پورو
            } وإلا {
                أرجع true;
            }
        } وإلا {
            ثابت xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, ميلاپ_name);
            أكد(old_kc != XKB_KEYCODE_INVALID);
            إذا (old_kc != kc) {
                /* There is already a different key with this name. */

                إذا (report) {
                    ثابت xkb_keycode_t use    = (clobber) ? kc : old_kc;
                    ثابت xkb_keycode_t ignore = (clobber) ? old_kc : kc;
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_NAME,
                            "Key name %s assigned to multiple keys; "
                            "Using %"PRIu32", ignoring %"PRIu32"\n",
                            ڪنجي_Name_متن(info->ctx, name), use, ignore);
                }

                إذا (clobber) {
                    /* Remove conflicting key name mapping */
                    keycode_store_delete_ڪنجي(&info->keycodes, ميلاپ_name);
                } وإلا {
                    أرجع true;
                }
            }
        }
    }

    ثابت Keycode_ميلاپ ميلاپ_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ثابت xkb_atom_t old_name = keycode_store_get_ڪنجي_name(&info->keycodes, ميلاپ_kc);
    إذا (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        إذا (old_name == name) {
            أكد(keycode_store_get_keycode(&info->keycodes, ميلاپ_name) == kc);
            إذا (report)
                log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         ڪنجي_Name_متن(info->ctx, old_name), kc);
            أرجع true;
        }

        ثابت bool clobber = (ملايو != ملايو_AUGMENT);
        إذا (report) {
            ثابت حرف* ثابت kname     = ڪنجي_Name_متن(info->ctx, name);
            ثابت حرف* ثابت old_kname = ڪنجي_Name_متن(info->ctx, old_name);
            ثابت حرف* ثابت use    = (clobber) ? kname : old_kname;
            ثابت حرف* ثابت ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_پيغام_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        إذا (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_ڪنجي(&info->keycodes, ميلاپ_kc, name);
        }
    } وإلا {
        /* No previous keycode */
        إذا (!keycode_store_وجهو_ڪنجي(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ورهاست_FAILURE_2,
                    "Cannot add keycode\n");
            أرجع false;
        }
    }

    أرجع true;
}

/***====================================================================***/

ساكن bool
HandleAliasDef(ڪنجي_نالا_Info *info, ثابت ڪنجي_Alias_Def *def, bool report);

ساكن فراغ
ملايو_Keycode_Stores(ڪنجي_نالا_Info *into, ڪنجي_نالا_Info *from,
                   تعداد ملايو_گهڻائي ملايو, bool report)
{
    إذا (darray_خالي_جايون(into->keycodes.هيٺيون) && darray_خالي_جايون(into->keycodes.مٿاهون) &&
        darray_خالي_جايون(into->keycodes.نالا)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.هيٺيون);
        darray_init(from->keycodes.مٿاهون);
        darray_init(from->keycodes.نالا);
    } وإلا {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        لكل (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_سائيز(from->keycodes.هيٺيون);
             kc++) {
            /* Skip undefined keycodes */
            ثابت xkb_atom_t name = darray_شي(from->keycodes.هيٺيون, kc);
            إذا (name == XKB_ATOM_NONE)
                استمر;

            إذا (!جوڙيو_ڪنجي_Name(into, kc, name, ملايو, report))
                into->error_ڳڻپ++;
        }

        /* High keycodes */
        ثابت مٿاهون_Keycode_داخلا *new;
        darray_foreach(new, from->keycodes.مٿاهون) {
            أكد(new->name != XKB_ATOM_NONE);
            إذا (!جوڙيو_ڪنجي_Name(into, new->keycode, new->name, ملايو, report))
                into->error_ڳڻپ++;
        }

        /* Aliases. */
        Keycode_ميلاپ *ميلاپ;
        xkb_atom_t alias;
        darray_enumerate(alias, ميلاپ, from->keycodes.نالا) {
            إذا (!ميلاپ->مليو || !ميلاپ->is_alias)
                استمر;

            ثابت ڪنجي_Alias_Def def = {
                .ملايو = ملايو,
                .alias = alias,
                .real = ميلاپ->alias.real
            };
            إذا (!HandleAliasDef(into, &def, report))
                into->error_ڳڻپ++;
        }
    }
}

ساكن فراغ
ملايو_Included_Keycodes(ڪنجي_نالا_Info *into, ڪنجي_نالا_Info *from,
                      تعداد ملايو_گهڻائي ملايو, bool report)
{
    إذا (from->error_ڳڻپ > 0) {
        into->error_ڳڻپ += from->error_ڳڻپ;
        أرجع;
    }

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    ملايو_Keycode_Stores(into, from, ملايو, report);

    /* Merge LED names. */
    إذا (into->انگ_led_نالا == 0) {
        يادگيري_نقل(into->led_نالا, from->led_نالا,
               حجم(*from->led_نالا) * from->انگ_led_نالا);
        into->انگ_led_نالا = from->انگ_led_نالا;
        from->انگ_led_نالا = 0;
    }
    وإلا {
        لكل (xkb_led_اشاريو_t idx = 0; idx < from->انگ_led_نالا; idx++) {
            LedNameInfo *ledi = &from->led_نالا[idx];

            إذا (ledi->name == XKB_ATOM_NONE)
                استمر;

            ledi->ملايو = ملايو;
            إذا (!جوڙيو_Led_Name(into, false, ledi, idx, report))
                into->error_ڳڻپ++;
        }
    }
}

ساكن فراغ
HandleKeycodesFile(ڪنجي_نالا_Info *info, XkbFile *file);

ساكن bool
HandleIncludeKeycodes(ڪنجي_نالا_Info *info, IncludeStmt *include, bool report)
{
    ڪنجي_نالا_Info included;

    إذا (Exceeds_Include_Max_اونهائي(info->ctx, info->include_اونهائي)) {
        info->error_ڳڻپ += 10;
        أرجع false;
    }

    Init_ڪنجي_نالا_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->ايندڙ_incl) {
        ڪنجي_نالا_Info ايندڙ_incl;
        XkbFile *file;

        حرف رستو[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  رستو, حجم(رستو));
        إذا (!file) {
            info->error_ڳڻپ += 10;
            Clear_ڪنجي_نالا_Info(&included);
            أرجع false;
        }

        Init_ڪنجي_نالا_Info(&ايندڙ_incl, info->keymap_info, info->include_اونهائي + 1);

        HandleKeycodesFile(&ايندڙ_incl, file);

        ملايو_Included_Keycodes(&included, &ايندڙ_incl, stmt->ملايو, report);

        Clear_ڪنجي_نالا_Info(&ايندڙ_incl);
        FreeXkbFile(file);
    }

    ملايو_Included_Keycodes(info, &included, include->ملايو, report);
    Clear_ڪنجي_نالا_Info(&included);

    أرجع (info->error_ڳڻپ == 0);
}

ساكن bool
HandleKeycodeDef(ڪنجي_نالا_Info *info, KeycodeDef *stmt, bool report)
{
    إذا (stmt->قدر < 0 || stmt->قدر > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->قدر, XKB_KEYCODE_MAX);
        أرجع false;
    }

    أرجع جوڙيو_ڪنجي_Name(info, (xkb_keycode_t) stmt->قدر,
                      stmt->name, stmt->ملايو, report);
}

ساكن bool
HandleAliasDef(ڪنجي_نالا_Info *info, ثابت ڪنجي_Alias_Def *def, bool report)
{
    ثابت Keycode_ميلاپ ميلاپ_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    إذا (ميلاپ_name.مليو) {
        ثابت bool clobber = (def->ملايو != ملايو_AUGMENT);
        إذا (ميلاپ_name.is_alias) {
            إذا (def->real == ميلاپ_name.alias.real) {
                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             ڪنجي_Name_متن(info->ctx, def->alias),
                             ڪنجي_Name_متن(info->ctx, def->real));
            } وإلا {
                ثابت xkb_atom_t use = (clobber)
                    ? def->real
                    : ميلاپ_name.alias.real;
                ثابت xkb_atom_t ignore = (clobber)
                    ? ميلاپ_name.alias.real
                    : def->real;

                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_NAME,
                             "Multiple definitions for alias %s; "
                             "Using %s, ignoring %s\n",
                             ڪنجي_Name_متن(info->ctx, def->alias),
                             ڪنجي_Name_متن(info->ctx, use),
                             ڪنجي_Name_متن(info->ctx, ignore));

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
                log_warn(info->ctx, XKB_WARNING_CONFLICTING_ڪنجي_NAME,
                         "Alias name %s already assigned to a real key; "
                         "Using %s, ignoring %s\n",
                         ڪنجي_Name_متن(info->ctx, def->alias),
                         (clobber ? "alias" : "key"),
                         (clobber ? "key" : "alias"));
            }

            إذا (clobber) {
                /*
                 * Note that we override the key even if the alias is proved
                 * invalid afterwards. This would be a bug in the keycodes
                 * files or rules, not xkbcommon.
                 */
                keycode_store_delete_ڪنجي(&info->keycodes, ميلاپ_name);
            } وإلا {
                أرجع true;
            }
        }
    }

    أرجع keycode_store_وجهو_alias(&info->keycodes, def->alias, def->real);
}

ساكن bool
Handle_ڪنجي_Name_Var(ڪنجي_نالا_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *ميدان_2;
    اظهار_Def *arrayNdx;

    إذا (!اظهار_Resolve_Lhs(info->ctx, stmt->name, &elem, &ميدان_2, &arrayNdx))
        أرجع false;

    إذا (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, ميدان_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    إذا (!istreq(ميدان_2, "minimum") && !istreq(ميدان_2, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_ميدان,
                "Default defined for unknown field \"%s\"; Ignored\n", ميدان_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    إذا (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", ميدان_2, "defaults");
        أرجع !(info->keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH);
    }

    int64_t val = 0;
    إذا (!اظهار_Resolve_Integer(info->ctx, stmt->قدر, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_ميدان_TYPE, "keycodes",
                      ميدان_2, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        أرجع !(info->keymap_info->strict & PARSER_NO_ميدان_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    أرجع true;
}

ساكن bool
HandleLedNameDef(ڪنجي_نالا_Info *info, LedNameDef *def, bool report)
{
    إذا (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_ڳڻپ++;
        log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!اظهار_Resolve_String(info->ctx, def->name, &name)) {
        حرف buf[20];
        snprintf(buf, حجم(buf), "%"PRId64, def->ndx);
        info->error_ڳڻپ++;
        أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_ميدان_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.ملايو = def->ملايو, .name = name};
    أرجع جوڙيو_Led_Name(info, true, &ledi, (xkb_led_اشاريو_t) def->ndx - 1, report);
}

ساكن فراغ
HandleKeycodesFile(ڪنجي_نالا_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report_same_file = verbosity > 0;
    ثابت bool report_include   = verbosity > 7;

    حرر(info->name);
    info->name = strdup_محفوظ(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->ايندڙ) {
        بدل (stmt->type) {
        حالة STMT_INCLUDE:
            ok = HandleIncludeKeycodes(info, (IncludeStmt *) stmt, report_include);
            اكسر;
        حالة STMT_KEYCODE:
            ok = HandleKeycodeDef(info, (KeycodeDef *) stmt, report_same_file);
            اكسر;
        حالة STMT_ALIAS:
            ok = HandleAliasDef(info, (ڪنجي_Alias_Def *) stmt, report_same_file);
            اكسر;
        حالة STMT_VAR:
            ok = Handle_ڪنجي_Name_Var(info, (VarDef *) stmt);
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
            log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_منزل_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_ڳڻپ++;

        إذا (info->error_ڳڻپ > 10) {
            log_err(info->ctx, XKB_LOG_پيغام_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    محفوظ_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
Copy_ڪنجي_نالا_منزل_Keymap(بنية xkb_keymap *keymap, ڪنجي_نالا_Info *info)
{
    إذا (darray_خالي_جايون(info->keycodes.هيٺيون) && darray_خالي_جايون(info->keycodes.مٿاهون)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        أكد(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_ڪنجي_ڪوڊ = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_ڪنجي_ڪوڊ = 255;
        keymap->انگ_ڪنجيون = keymap->انگ_ڪنجيون_هيٺيون = keymap->max_ڪنجي_ڪوڊ + 1;
    } وإلا {
        أكد(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_ڪنجي_ڪوڊ = info->keycodes.min;
        keymap->max_ڪنجي_ڪوڊ = (darray_خالي_جايون(info->keycodes.مٿاهون))
            ? darray_سائيز(info->keycodes.هيٺيون) - 1
            : darray_شي(
                info->keycodes.مٿاهون,
                darray_سائيز(info->keycodes.مٿاهون) - 1
            ).keycode;
        keymap->انگ_ڪنجيون_هيٺيون = darray_سائيز(info->keycodes.هيٺيون);
        keymap->انگ_ڪنجيون = keymap->انگ_ڪنجيون_هيٺيون
                         + darray_سائيز(info->keycodes.مٿاهون);
    }

    بنية xkb_ڪنجي * ثابت ڪنجيون = صفر_يادگيري_ڏيو(keymap->انگ_ڪنجيون, حجم(*ڪنجيون));
    إذا (!ڪنجيون) {
        keymap->انگ_ڪنجيون = 0;
        keymap->min_ڪنجي_ڪوڊ = keymap->max_ڪنجي_ڪوڊ = XKB_KEYCODE_INVALID;
        أرجع false;
    }

    /* Low keycodes */
    لكل (xkb_keycode_t kc = keymap->min_ڪنجي_ڪوڊ; kc < keymap->انگ_ڪنجيون_هيٺيون; kc++)
        ڪنجيون[kc].keycode = kc;

    لكل (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_سائيز(info->keycodes.هيٺيون);
         kc++)
        ڪنجيون[kc].name = darray_شي(info->keycodes.هيٺيون, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->انگ_ڪنجيون_هيٺيون; /* First high keycode index */
    ثابت مٿاهون_Keycode_داخلا *داخلا;
    darray_foreach(داخلا, info->keycodes.مٿاهون) {
        أكد(داخلا->name != XKB_ATOM_NONE);
        ڪنجيون[idx].keycode = داخلا->keycode;
        ڪنجيون[idx].name = داخلا->name;
        idx++;
    }

    keymap->ڪنجيون = ڪنجيون;
    أرجع true;
}

ساكن bool
CopyKeycodeNameLUT(بنية xkb_keymap *keymap, ڪنجي_نالا_Info *info)
{
    Keycode_ميلاپ *ميلاپ;
    xkb_atom_t name;
    darray_enumerate(name, ميلاپ, info->keycodes.نالا) {
        إذا (!ميلاپ->مليو)
            استمر;

        إذا (ميلاپ->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ثابت Keycode_ميلاپ ميلاپ_real =
                keycode_store_lookup_name(&info->keycodes, ميلاپ->alias.real);
            إذا (!ميلاپ_real.مليو) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        ڪنجي_Name_متن(info->ctx, name),
                        ڪنجي_Name_متن(info->ctx, ميلاپ->alias.real));
                ميلاپ->مليو = false;
                استمر;
            }
            أكد(!ميلاپ_real.is_alias);
        } وإلا إذا (!ميلاپ->ڪنجي.هيٺيون) {
            /* Update to final index in keymap::keys */
            ميلاپ->ڪنجي.اشاريو += keymap->انگ_ڪنجيون_هيٺيون;
        }
    }

    darray_shrink(info->keycodes.نالا);
    keymap->انگ_ڪنجي_نالا = darray_سائيز(info->keycodes.نالا);
    darray_steal(info->keycodes.نالا, &keymap->ڪنجي_نالا, عدم);
    darray_init(info->keycodes.نالا);
    أرجع true;
}

ساكن bool
Copy_Led_نالا_منزل_Keymap(بنية xkb_keymap *keymap, ڪنجي_نالا_Info *info)
{
    keymap->انگ_leds = info->انگ_led_نالا;
    لكل (xkb_led_اشاريو_t idx = 0; idx < info->انگ_led_نالا; idx++) {
        LedNameInfo *ledi = &info->led_نالا[idx];

        إذا (ledi->name == XKB_ATOM_NONE)
            استمر;

        keymap->leds[idx].name = ledi->name;
    }

    أرجع true;
}

ساكن bool
Copy_ڪنجي_نالا_Info_منزل_Keymap(بنية xkb_keymap *keymap, ڪنجي_نالا_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    إذا (!Copy_ڪنجي_نالا_منزل_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !Copy_Led_نالا_منزل_Keymap(keymap, info))
        أرجع false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    إذا (!keymap->انگ_ڪنجيون ||
        keymap->min_ڪنجي_ڪوڊ > 0) {
        keymap->redirect_ڪنجي_auto = 0;
    } وإلا {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        لكل (xkb_keycode_t k = keymap->انگ_ڪنجيون; k-- > keymap->انگ_ڪنجيون_هيٺيون;) {
            إذا (keycode > keymap->ڪنجيون[k].keycode)
                اكسر;
            /* Try next keycode downwards */
            keycode = keymap->ڪنجيون[k].keycode - 1;
        }
        keymap->redirect_ڪنجي_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_محفوظ(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    أرجع true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    ڪنجي_نالا_Info info;

    Init_ڪنجي_نالا_Info(&info, keymap_info, 0);

    إذا (file != عدم)
        HandleKeycodesFile(&info, file);

    إذا (info.error_ڳڻپ != 0)
        اذهب err_info;

    إذا (!Copy_ڪنجي_نالا_Info_منزل_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    Clear_ڪنجي_نالا_Info(&info);
    أرجع true;

err_info:
    Clear_ڪنجي_نالا_Info(&info);
    أرجع false;
}
