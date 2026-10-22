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
} بلند_Keycode_اندراج;

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
    darray(بلند_Keycode_اندراج) بلند;
    /**
     * name -> keycode mapping
     */
    darray(Keycode_میل) ناں;
} KeycodeStore;

ساكن مضمن فراغ
keycode_store_init(KeycodeStore *store)
{
    darray_init(store->کم);
    darray_init(store->بلند);
    darray_init(store->ناں);
    static_assert(XKB_KEYCODE_INVALID > XKB_KEYCODE_MAX,
                  "Hey, you can't be changing stuff like that!");
    store->min = XKB_KEYCODE_INVALID;
}

ساكن مضمن فراغ
keycode_store_free(KeycodeStore *store)
{
    darray_free(store->کم);
    darray_free(store->بلند);
    darray_free(store->ناں);
}

ساكن مضمن فراغ
keycode_store_update_کلید(KeycodeStore *store, Keycode_میل میل, xkb_atom_t name)
{
    إذا (unlikely(!میل.لبھیا || میل.is_alias)) {
        أرجع;
    } وإلا إذا (میل.کلید.کم) {
        /* assume valid index */
        أكد(میل.کلید.اشاریہ < darray_ناپ(store->کم));
        darray_شے(store->کم, میل.کلید.اشاریہ) = name;
    } وإلا {
        /* assume valid index */
        أكد(میل.کلید.اشاریہ < darray_ناپ(store->بلند));
        darray_شے(store->بلند, میل.کلید.اشاریہ).name = name;
    }

    /* Names lookup */
    إذا (name >= darray_ناپ(store->ناں)) {
        darray_resize0(store->ناں, name + 1);
    }
    darray_شے(store->ناں, name) = میل;
}

ساكن bool
keycode_store_داخل_کرو_کلید(KeycodeStore *store, xkb_keycode_t kc, xkb_atom_t name)
{
    إذا (name >= darray_ناپ(store->ناں)) {
        darray_resize0(store->ناں, name + 1);
    }
    إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        /* Low keycode */
        إذا (kc >= (xkb_keycode_t) darray_ناپ(store->کم))
            darray_resize0(store->کم, kc + 1);
        darray_شے(store->کم, kc) = name;
        إذا (kc < store->min)
            store->min = kc;
        darray_شے(store->ناں, name) = (Keycode_میل) {
            .کلید = {
                .لبھیا = true,
                .کم = true,
                .is_alias = false,
                .اشاریہ = kc
            }
        };
    } وإلا {
        /* High keycode: insert into a sorted list */
        ثابت darray_ناپ_t idx = darray_ناپ(store->بلند);
        إذا (idx && darray_شے(store->بلند, idx - 1).keycode > kc) {
            /*
             * Slow path: need to sort the keys. Since we maintain the list
             * sorted, we simply need to look for the insertion index.
             */
            darray_ناپ_t ہیٹھلا = 0;
            darray_ناپ_t اتلا = idx;
            طالما (ہیٹھلا < اتلا) {
                ثابت darray_ناپ_t mid = ہیٹھلا + (اتلا - 1 - ہیٹھلا) / 2;
                ثابت بلند_Keycode_اندراج * ثابت اندراج =
                    &darray_شے(store->بلند, mid);
                إذا (اندراج->keycode < kc) {
                    ہیٹھلا = mid + 1;
                } وإلا إذا (اندراج->keycode > kc) {
                    اتلا = mid;
                } وإلا {
                    /* Unreachable: there is no repetition in the list */
                    أكد(اندراج->keycode != kc);
                }
            }
            أكد(ہیٹھلا < idx);
            أكد(darray_شے(store->بلند, ہیٹھلا).keycode > kc);

            /* Update references to entries that will be moved */
            بلند_Keycode_اندراج *اندراج;
            darray_foreach_from(اندراج, store->بلند, ہیٹھلا) {
                darray_شے(store->ناں, اندراج->name).کلید.اشاریہ++;
            }

            darray_داخل_کرو(store->بلند, ہیٹھلا,
                          (بلند_Keycode_اندراج){.keycode = kc, .name = name});
            darray_شے(store->ناں, name) = (Keycode_میل) {
                .کلید = {
                    .لبھیا = true,
                    .کم = false,
                    .is_alias = false,
                    .اشاریہ = ہیٹھلا
                }
            };
        } وإلا {
            /* Fast path: no need to sort */
            darray_append(store->بلند,
                          (بلند_Keycode_اندراج){.keycode = kc, .name = name});
            darray_شے(store->ناں, name) = (Keycode_میل) {
                .کلید = {
                    .لبھیا = true,
                    .کم = false,
                    .is_alias = false,
                    .اشاریہ = idx
                }
            };
        }
        إذا (darray_خالی_تھاواں(store->کم))
            store->min = darray_شے(store->بلند, 0).keycode;
    }
    أرجع true;
}

ساكن مضمن bool
keycode_store_داخل_کرو_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    إذا (alias >= darray_ناپ(store->ناں)) {
        darray_resize0(store->ناں, alias + 1);
    }
    darray_شے(store->ناں, alias) = (Keycode_میل) {
        .alias = {
            .لبھیا = true,
            .is_alias = true,
            .real = real
        }
    };
    أرجع true;
}

ساكن مضمن bool
keycode_store_update_alias(KeycodeStore *store, xkb_atom_t alias, xkb_atom_t real)
{
    darray_شے(store->ناں, alias).alias.real = real;
    أرجع true;
}

ساكن مضمن فراغ
keycode_store_delete_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    إذا (name < darray_ناپ(store->ناں))
        darray_شے(store->ناں, name).لبھیا = false;
}

ساكن فراغ
keycode_store_delete_کلید(KeycodeStore *store, ثابت Keycode_میل میل)
{
    إذا (unlikely(!میل.لبھیا || میل.is_alias)) {
        أرجع;
    } وإلا إذا (میل.کلید.کم) {
        /* assume valid index */
        أكد(میل.کلید.اشاریہ < darray_ناپ(store->کم));
        darray_شے(store->ناں,
                    darray_شے(store->کم, میل.کلید.اشاریہ)).لبھیا = false;
        إذا (میل.کلید.اشاریہ + 1u == darray_ناپ(store->کم)) {
            /* Highest low keycode: shrink */
            إذا (store->min == میل.کلید.اشاریہ) {
                /* No low keycode left */
                darray_ناپ(store->کم) = 0;
            } وإلا {
                /* Look for previous defined low keycode */
                لكل (darray_ناپ_t idx = میل.کلید.اشاریہ; idx > 0; idx--) {
                    إذا (darray_شے(store->کم, idx - 1) != XKB_ATOM_NONE) {
                        darray_ناپ(store->کم) = idx;
                        اكسر;
                    }
                }
            }
        } وإلا {
            /* Lower keycode: reset */
            darray_شے(store->کم, میل.کلید.اشاریہ) = XKB_ATOM_NONE;
        }
    } وإلا {
        أكد(میل.کلید.اشاریہ < darray_ناپ(store->بلند)); /* assume valid index */
        darray_شے(store->ناں,
                    darray_شے(store->بلند, میل.کلید.اشاریہ).name).لبھیا = false;
        darray_remove(store->بلند, میل.کلید.اشاریہ);
        /* Update LUT indexes of high codes after the deleted one, if any */
        Keycode_میل *اندراج;
        darray_foreach(اندراج, store->ناں) {
            إذا (اندراج->لبھیا && !اندراج->is_alias && !اندراج->کلید.کم &&
                اندراج->کلید.اشاریہ > میل.کلید.اشاریہ) {
                اندراج->کلید.اشاریہ--;
            }
        }
    }

    /* Update bounds */
    إذا (darray_خالی_تھاواں(store->کم)) {
        store->min = (darray_خالی_تھاواں(store->بلند))
            ? XKB_KEYCODE_INVALID
            : darray_شے(store->بلند, 0).keycode;
    } وإلا {
        لكل (xkb_keycode_t kc = store->min; kc < darray_ناپ(store->کم); kc++) {
            إذا (darray_شے(store->کم, kc) != XKB_ATOM_NONE) {
                store->min = kc;
                اكسر;
            }
        }
    }
}

ساكن مضمن xkb_keycode_t
keycode_store_get_keycode(ثابت KeycodeStore *store, Keycode_میل میل)
{
    إذا (!میل.لبھیا || میل.is_alias) {
        أرجع XKB_KEYCODE_INVALID;
    } وإلا إذا (میل.کلید.کم) {
        /* assume valid index */
        أكد(میل.کلید.اشاریہ < darray_ناپ(store->کم));
        أرجع (xkb_keycode_t) میل.کلید.اشاریہ;
    } وإلا {
        /* assume valid index */
        أكد(میل.کلید.اشاریہ < darray_ناپ(store->بلند));
        أرجع darray_شے(store->بلند, میل.کلید.اشاریہ).keycode;
    }
}

ساكن مضمن xkb_atom_t
keycode_store_get_کلید_name(ثابت KeycodeStore *store, Keycode_میل میل)
{
    إذا (!میل.لبھیا || میل.is_alias) {
        أرجع XKB_ATOM_NONE;
    } وإلا إذا (میل.کلید.کم) {
        /* assume valid index */
        أكد(میل.کلید.اشاریہ < darray_ناپ(store->کم));
        أرجع darray_شے(store->کم, میل.کلید.اشاریہ);
    } وإلا {
        /* assume valid index */
        أكد(میل.کلید.اشاریہ < darray_ناپ(store->بلند));
        أرجع darray_شے(store->بلند, میل.کلید.اشاریہ).name;
    }
}

ساكن Keycode_میل
keycode_store_lookup_keycode(ثابت KeycodeStore *store, xkb_keycode_t kc)
{
    /* Low keycodes */
    إذا (kc < (xkb_keycode_t) darray_ناپ(store->کم)) {
        أرجع (Keycode_میل) {
            .کلید = {
                .لبھیا = true,
                .کم = true,
                .is_alias = false,
                .اشاریہ = kc
            }
        };
    } وإلا إذا (kc <= XKB_KEYCODE_MAX_CONTIGUOUS) {
        أرجع (Keycode_میل) { .لبھیا = false };
    }

    /* High keycodes: use binary search */
    darray_ناپ_t ہیٹھلا = 0;
    darray_ناپ_t اتلا = darray_ناپ(store->بلند);
    طالما (ہیٹھلا < اتلا) {
        ثابت darray_ناپ_t mid = ہیٹھلا + (اتلا - 1 - ہیٹھلا) / 2;
        بلند_Keycode_اندراج * ثابت اندراج = &darray_شے(store->بلند, mid);
        إذا (اندراج->keycode < kc) {
            ہیٹھلا = mid + 1;
        } وإلا إذا (اندراج->keycode > kc) {
            اتلا = mid;
        } وإلا {
            أرجع (Keycode_میل) {
                .کلید = {
                    .لبھیا = true,
                    .کم = false,
                    .is_alias = false,
                    .اشاریہ = mid
                }
            };
        }
    }

    أرجع (Keycode_میل) { .لبھیا = false };
}

ساكن Keycode_میل
keycode_store_lookup_name(ثابت KeycodeStore *store, xkb_atom_t name)
{
    إذا (name >= darray_ناپ(store->ناں)) {
        أرجع (Keycode_میل) { .لبھیا = false, .is_alias = false };
    } وإلا {
        أرجع darray_شے(store->ناں, name);
    }
}

/***====================================================================***/

تعريفنوع بنية {
    تعداد ملاؤ_منوال ملاؤ;

    xkb_atom_t name;
} LedNameInfo;

تعريفنوع بنية {
    حرف *name;
    صحيح error_گنتی;
    بنانشانی صحيح include_ڈونگھائی;

    KeycodeStore keycodes;
    LedNameInfo led_ناں[XKB_MAX_LEDS];
    xkb_led_اشاریہ_t عدد_led_ناں;

    بنية xkb_context *ctx;
    ثابت بنية xkb_keymap_info *keymap_info;
} کلید_ناں_Info;

/***====================================================================***/

ساكن LedNameInfo *
لبھو_Led_By_Name(کلید_ناں_Info *info, xkb_atom_t name,
              xkb_led_اشاریہ_t *idx_باہرلا)
{
    لكل (xkb_led_اشاریہ_t idx = 0; idx < info->عدد_led_ناں; idx++) {
        LedNameInfo *ledi = &info->led_ناں[idx];
        إذا (ledi->name == name) {
            *idx_باہرلا = idx;
            أرجع ledi;
        }
    }

    أرجع عدم;
}

ساكن bool
جوڑو_Led_Name(کلید_ناں_Info *info, bool same_file,
           LedNameInfo *new, xkb_led_اشاریہ_t new_idx, bool report)
{
    xkb_led_اشاریہ_t old_idx;
    LedNameInfo *old;
    ثابت bool replace = (new->ملاؤ != ملاؤ_AUGMENT);

    /* Check if LED with the same name already exists. */
    old = لبھو_Led_By_Name(info, new->name, &old_idx);
    إذا (old) {
        إذا (old_idx == new_idx) {
            إذا (report)
                log_warn(info->ctx, XKB_LOG_سنیہا_NO_ID,
                         "Multiple indicators named \"%s\"; "
                         "Identical definitions ignored\n",
                         xkb_atom_متن(info->ctx, new->name));
            أرجع true;
        }

        إذا (report) {
            xkb_led_اشاریہ_t use    = (replace ? new_idx + 1 : old_idx + 1);
            xkb_led_اشاریہ_t ignore = (replace ? old_idx + 1 : new_idx + 1);
            log_warn(info->ctx, XKB_LOG_سنیہا_NO_ID,
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

    إذا (new_idx >= info->عدد_led_ناں)
        info->عدد_led_ناں = new_idx + 1;

    /* Check if LED with the same index already exists. */
    old = &info->led_ناں[new_idx];
    إذا (old->name != XKB_ATOM_NONE) {
        إذا (report) {
            ثابت xkb_atom_t use    = (replace ? new->name : old->name);
            ثابت xkb_atom_t ignore = (replace ? old->name : new->name);
            log_warn(info->ctx, XKB_LOG_سنیہا_NO_ID,
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
Clear_کلید_ناں_Info(کلید_ناں_Info *info)
{
    حرر(info->name);
    keycode_store_free(&info->keycodes);
}

ساكن فراغ
Init_کلید_ناں_Info(کلید_ناں_Info *info, ثابت بنية xkb_keymap_info *keymap_info,
                 بنانشانی صحيح include_ڈونگھائی)
{
    memset(info, 0, حجم(*info));
    info->ctx = keymap_info->keymap.ctx;
    info->keymap_info = keymap_info;
    info->include_ڈونگھائی = include_ڈونگھائی;
    keycode_store_init(&info->keycodes);
}

ساكن bool
جوڑو_کلید_Name(کلید_ناں_Info *info, xkb_keycode_t kc, xkb_atom_t name,
           تعداد ملاؤ_منوال ملاؤ, bool report)
{
#جے_تعریف_شدہ NDEBUG
    ثابت
#جے_انت
    Keycode_میل میل_name = keycode_store_lookup_name(&info->keycodes, name);
    إذا (میل_name.لبھیا) {
        ثابت bool clobber = (ملاؤ != ملاؤ_AUGMENT);

        إذا (میل_name.is_alias) {
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
#جے_بنا_تعریف NDEBUG
                میل_name.لبھیا = false;
#جے_انت
            } وإلا {
                أرجع true;
            }
        } وإلا {
            ثابت xkb_keycode_t old_kc =
                keycode_store_get_keycode(&info->keycodes, میل_name);
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
                    keycode_store_delete_کلید(&info->keycodes, میل_name);
                } وإلا {
                    أرجع true;
                }
            }
        }
    }

    ثابت Keycode_میل میل_kc = keycode_store_lookup_keycode(&info->keycodes, kc);
    ثابت xkb_atom_t old_name = keycode_store_get_کلید_name(&info->keycodes, میل_kc);
    إذا (old_name != XKB_ATOM_NONE) {
        /* There is already a key with this keycode. */

        إذا (old_name == name) {
            أكد(keycode_store_get_keycode(&info->keycodes, میل_name) == kc);
            إذا (report)
                log_warn(info->ctx, XKB_LOG_سنیہا_NO_ID,
                         "Multiple identical key name definitions; "
                         "Later occurrences of \"%s = %"PRIu32"\" ignored\n",
                         کلید_Name_متن(info->ctx, old_name), kc);
            أرجع true;
        }

        ثابت bool clobber = (ملاؤ != ملاؤ_AUGMENT);
        إذا (report) {
            ثابت حرف* ثابت kname     = کلید_Name_متن(info->ctx, name);
            ثابت حرف* ثابت old_kname = کلید_Name_متن(info->ctx, old_name);
            ثابت حرف* ثابت use    = (clobber) ? kname : old_kname;
            ثابت حرف* ثابت ignore = (clobber) ? old_kname : kname;
            log_warn(info->ctx, XKB_LOG_سنیہا_NO_ID,
                     "Multiple names for keycode %"PRIu32"; "
                     "Using %s, ignoring %s\n", kc, use, ignore);
        }
        إذا (clobber) {
            keycode_store_delete_name(&info->keycodes, old_name);
            keycode_store_update_کلید(&info->keycodes, میل_kc, name);
        }
    } وإلا {
        /* No previous keycode */
        إذا (!keycode_store_داخل_کرو_کلید(&info->keycodes, kc, name)) {
            log_err(info->ctx, XKB_ERROR_ونڈ_FAILURE_2,
                    "Cannot add keycode\n");
            أرجع false;
        }
    }

    أرجع true;
}

/***====================================================================***/

ساكن bool
HandleAliasDef(کلید_ناں_Info *info, ثابت کلید_Alias_Def *def, bool report);

ساكن فراغ
ملاؤ_Keycode_Stores(کلید_ناں_Info *into, کلید_ناں_Info *from,
                   تعداد ملاؤ_منوال ملاؤ, bool report)
{
    إذا (darray_خالی_تھاواں(into->keycodes.کم) && darray_خالی_تھاواں(into->keycodes.بلند) &&
        darray_خالی_تھاواں(into->keycodes.ناں)) {
        /* Fast path: steal “from” */
        into->keycodes = from->keycodes;
        darray_init(from->keycodes.کم);
        darray_init(from->keycodes.بلند);
        darray_init(from->keycodes.ناں);
    } وإلا {
        /* Slow path: check for conflicts */

        /* Low keycodes */
        لكل (xkb_keycode_t kc = from->keycodes.min;
             kc < (xkb_keycode_t) darray_ناپ(from->keycodes.کم);
             kc++) {
            /* Skip undefined keycodes */
            ثابت xkb_atom_t name = darray_شے(from->keycodes.کم, kc);
            إذا (name == XKB_ATOM_NONE)
                استمر;

            إذا (!جوڑو_کلید_Name(into, kc, name, ملاؤ, report))
                into->error_گنتی++;
        }

        /* High keycodes */
        ثابت بلند_Keycode_اندراج *new;
        darray_foreach(new, from->keycodes.بلند) {
            أكد(new->name != XKB_ATOM_NONE);
            إذا (!جوڑو_کلید_Name(into, new->keycode, new->name, ملاؤ, report))
                into->error_گنتی++;
        }

        /* Aliases. */
        Keycode_میل *میل;
        xkb_atom_t alias;
        darray_enumerate(alias, میل, from->keycodes.ناں) {
            إذا (!میل->لبھیا || !میل->is_alias)
                استمر;

            ثابت کلید_Alias_Def def = {
                .ملاؤ = ملاؤ,
                .alias = alias,
                .real = میل->alias.real
            };
            إذا (!HandleAliasDef(into, &def, report))
                into->error_گنتی++;
        }
    }
}

ساكن فراغ
ملاؤ_Included_Keycodes(کلید_ناں_Info *into, کلید_ناں_Info *from,
                      تعداد ملاؤ_منوال ملاؤ, bool report)
{
    إذا (from->error_گنتی > 0) {
        into->error_گنتی += from->error_گنتی;
        أرجع;
    }

    إذا (into->name == عدم) {
        into->name = steal(&from->name);
    }

    /* Merge key names. */
    ملاؤ_Keycode_Stores(into, from, ملاؤ, report);

    /* Merge LED names. */
    إذا (into->عدد_led_ناں == 0) {
        حافظہ_نقل(into->led_ناں, from->led_ناں,
               حجم(*from->led_ناں) * from->عدد_led_ناں);
        into->عدد_led_ناں = from->عدد_led_ناں;
        from->عدد_led_ناں = 0;
    }
    وإلا {
        لكل (xkb_led_اشاریہ_t idx = 0; idx < from->عدد_led_ناں; idx++) {
            LedNameInfo *ledi = &from->led_ناں[idx];

            إذا (ledi->name == XKB_ATOM_NONE)
                استمر;

            ledi->ملاؤ = ملاؤ;
            إذا (!جوڑو_Led_Name(into, false, ledi, idx, report))
                into->error_گنتی++;
        }
    }
}

ساكن فراغ
HandleKeycodesFile(کلید_ناں_Info *info, XkbFile *file);

ساكن bool
HandleIncludeKeycodes(کلید_ناں_Info *info, IncludeStmt *include, bool report)
{
    کلید_ناں_Info included;

    إذا (Exceeds_Include_Max_ڈونگھائی(info->ctx, info->include_ڈونگھائی)) {
        info->error_گنتی += 10;
        أرجع false;
    }

    Init_کلید_ناں_Info(&included, info->keymap_info, 0 /* unused */);
    included.name = steal(&include->stmt);

    ثابت بنية parser_keymap_config config = {
        .format = info->keymap_info->keymap.format,
        .strict = info->keymap_info->strict
    };

    لكل (IncludeStmt *stmt = include; stmt; stmt = stmt->اگلا_incl) {
        کلید_ناں_Info اگلا_incl;
        XkbFile *file;

        حرف رستہ[PATH_MAX];

        file = ProcessIncludeFile(info->ctx, &config, stmt, FILE_TYPE_KEYCODES,
                                  رستہ, حجم(رستہ));
        إذا (!file) {
            info->error_گنتی += 10;
            Clear_کلید_ناں_Info(&included);
            أرجع false;
        }

        Init_کلید_ناں_Info(&اگلا_incl, info->keymap_info, info->include_ڈونگھائی + 1);

        HandleKeycodesFile(&اگلا_incl, file);

        ملاؤ_Included_Keycodes(&included, &اگلا_incl, stmt->ملاؤ, report);

        Clear_کلید_ناں_Info(&اگلا_incl);
        FreeXkbFile(file);
    }

    ملاؤ_Included_Keycodes(info, &included, include->ملاؤ, report);
    Clear_کلید_ناں_Info(&included);

    أرجع (info->error_گنتی == 0);
}

ساكن bool
HandleKeycodeDef(کلید_ناں_Info *info, KeycodeDef *stmt, bool report)
{
    إذا (stmt->قدر < 0 || stmt->قدر > XKB_KEYCODE_MAX) {
        log_err(info->ctx, XKB_ERROR_INVALID_KEYCODE_,
                "Illegal keycode %"PRId64": must be between 0..%u; "
                "Key ignored\n", stmt->قدر, XKB_KEYCODE_MAX);
        أرجع false;
    }

    أرجع جوڑو_کلید_Name(info, (xkb_keycode_t) stmt->قدر,
                      stmt->name, stmt->ملاؤ, report);
}

ساكن bool
HandleAliasDef(کلید_ناں_Info *info, ثابت کلید_Alias_Def *def, bool report)
{
    ثابت Keycode_میل میل_name = keycode_store_lookup_name(&info->keycodes,
                                                              def->alias);
    إذا (میل_name.لبھیا) {
        ثابت bool clobber = (def->ملاؤ != ملاؤ_AUGMENT);
        إذا (میل_name.is_alias) {
            إذا (def->real == میل_name.alias.real) {
                إذا (report)
                    log_warn(info->ctx, XKB_WARNING_CONFLICTING_کلید_NAME,
                             "Alias of %s for %s declared more than once; "
                             "First definition ignored\n",
                             کلید_Name_متن(info->ctx, def->alias),
                             کلید_Name_متن(info->ctx, def->real));
            } وإلا {
                ثابت xkb_atom_t use = (clobber)
                    ? def->real
                    : میل_name.alias.real;
                ثابت xkb_atom_t ignore = (clobber)
                    ? میل_name.alias.real
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
                keycode_store_delete_کلید(&info->keycodes, میل_name);
            } وإلا {
                أرجع true;
            }
        }
    }

    أرجع keycode_store_داخل_کرو_alias(&info->keycodes, def->alias, def->real);
}

ساكن bool
Handle_کلید_Name_Var(کلید_ناں_Info *info, VarDef *stmt)
{
    ثابت حرف *elem, *میدان_2;
    اظہاریہ_Def *arrayNdx;

    إذا (!اظہاریہ_Resolve_Lhs(info->ctx, stmt->name, &elem, &میدان_2, &arrayNdx))
        أرجع false;

    إذا (elem) {
        log_err(info->ctx, XKB_ERROR_GLOBAL_DEFAULTS_WRONG_SCOPE,
                "Cannot set global defaults for \"%s\" element; "
                "Assignment to \"%s.%s\" ignored\n", elem, elem, میدان_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    إذا (!istreq(میدان_2, "minimum") && !istreq(میدان_2, "maximum")) {
        log_err(info->ctx, XKB_ERROR_UNKNOWN_DEFAULT_میدان,
                "Default defined for unknown field \"%s\"; Ignored\n", میدان_2);
        أرجع !(info->keymap_info->strict &
                 PARSER_NO_UNKNOWN_KEYCODES_GLOBAL_FIELDS);
    }

    إذا (arrayNdx) {
        ReportNotArray(info->ctx, "keycodes", میدان_2, "defaults");
        أرجع !(info->keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH);
    }

    int64_t val = 0;
    إذا (!اظہاریہ_Resolve_Integer(info->ctx, stmt->قدر, &val) ||
        val < 0 || val > UINT32_MAX) {
        ReportBadType(info->ctx, XKB_ERROR_WRONG_میدان_TYPE, "keycodes",
                      میدان_2, "defaults", "integer 0..0xfffffffe");
        static_assert(XKB_KEYCODE_MAX == 0xfffffffe, "update error message");
        أرجع !(info->keymap_info->strict & PARSER_NO_میدان_TYPE_MISMATCH);
    }

    /* We ignore explicit min/max statements, we always use computed. */
    أرجع true;
}

ساكن bool
HandleLedNameDef(کلید_ناں_Info *info, LedNameDef *def, bool report)
{
    إذا (def->ndx < 1 || def->ndx > XKB_MAX_LEDS) {
        info->error_گنتی++;
        log_err(info->ctx, XKB_LOG_سنیہا_NO_ID,
                "Illegal indicator index (%"PRId64") specified; "
                "must be between 1 .. %"PRIu32"; Ignored\n",
                def->ndx, XKB_MAX_LEDS);
        أرجع false;
    }

    xkb_atom_t name = XKB_ATOM_NONE;
    إذا (!اظہاریہ_Resolve_String(info->ctx, def->name, &name)) {
        حرف buf[20];
        snprintf(buf, حجم(buf), "%"PRId64, def->ndx);
        info->error_گنتی++;
        أرجع ReportBadType(info->ctx, XKB_ERROR_WRONG_میدان_TYPE,
                             "indicator", "name", buf, "string");
    }

    LedNameInfo ledi = {.ملاؤ = def->ملاؤ, .name = name};
    أرجع جوڑو_Led_Name(info, true, &ledi, (xkb_led_اشاریہ_t) def->ndx - 1, report);
}

ساكن فراغ
HandleKeycodesFile(کلید_ناں_Info *info, XkbFile *file)
{
    bool ok;

    /* Conflicts in the same file probably require more attention than conflicts
     * with included files. */
    ثابت صحيح verbosity = xkb_context_get_log_verbosity(info->ctx);
    ثابت bool report_same_file = verbosity > 0;
    ثابت bool report_include   = verbosity > 7;

    حرر(info->name);
    info->name = strdup_محفوظ(file->name);

    لكل (ParseCommon *stmt = file->defs; stmt; stmt = stmt->اگلا) {
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
            log_err(info->ctx, XKB_LOG_سنیہا_NO_ID,
                    "Keycode files may define key and indicator names only; "
                    "Ignoring %s\n", stmt_type_منزل_string(stmt->type));
            ok = false;
            اكسر;
        }

        إذا (!ok)
            info->error_گنتی++;

        إذا (info->error_گنتی > 10) {
            log_err(info->ctx, XKB_LOG_سنیہا_NO_ID,
                    "Abandoning keycodes file \"%s\"\n",
                    محفوظ_map_name(file->name));
            اكسر;
        }
    }
}

/***====================================================================***/

ساكن bool
Copy_کلید_ناں_منزل_Keymap(بنية xkb_keymap *keymap, کلید_ناں_Info *info)
{
    إذا (darray_خالی_تھاواں(info->keycodes.کم) && darray_خالی_تھاواں(info->keycodes.بلند)) {
        /* If the keymap has no keys, let’s just use the safest pair we know. */
        أكد(info->keycodes.min == XKB_KEYCODE_INVALID);
        keymap->min_کلید_کوڈ = 8;
        static_assert(255 < XKB_KEYCODE_MAX_CONTIGUOUS, "");
        keymap->max_کلید_کوڈ = 255;
        keymap->عدد_کلیدیں = keymap->عدد_کلیدیں_کم = keymap->max_کلید_کوڈ + 1;
    } وإلا {
        أكد(info->keycodes.min <= XKB_KEYCODE_MAX);
        keymap->min_کلید_کوڈ = info->keycodes.min;
        keymap->max_کلید_کوڈ = (darray_خالی_تھاواں(info->keycodes.بلند))
            ? darray_ناپ(info->keycodes.کم) - 1
            : darray_شے(
                info->keycodes.بلند,
                darray_ناپ(info->keycodes.بلند) - 1
            ).keycode;
        keymap->عدد_کلیدیں_کم = darray_ناپ(info->keycodes.کم);
        keymap->عدد_کلیدیں = keymap->عدد_کلیدیں_کم
                         + darray_ناپ(info->keycodes.بلند);
    }

    بنية xkb_کلید * ثابت کلیدیں = صفر_حافظہ_دیو(keymap->عدد_کلیدیں, حجم(*کلیدیں));
    إذا (!کلیدیں) {
        keymap->عدد_کلیدیں = 0;
        keymap->min_کلید_کوڈ = keymap->max_کلید_کوڈ = XKB_KEYCODE_INVALID;
        أرجع false;
    }

    /* Low keycodes */
    لكل (xkb_keycode_t kc = keymap->min_کلید_کوڈ; kc < keymap->عدد_کلیدیں_کم; kc++)
        کلیدیں[kc].keycode = kc;

    لكل (xkb_keycode_t kc = info->keycodes.min;
         kc < (xkb_keycode_t) darray_ناپ(info->keycodes.کم);
         kc++)
        کلیدیں[kc].name = darray_شے(info->keycodes.کم, kc);

    /* High keycodes */
    xkb_keycode_t idx = keymap->عدد_کلیدیں_کم; /* First high keycode index */
    ثابت بلند_Keycode_اندراج *اندراج;
    darray_foreach(اندراج, info->keycodes.بلند) {
        أكد(اندراج->name != XKB_ATOM_NONE);
        کلیدیں[idx].keycode = اندراج->keycode;
        کلیدیں[idx].name = اندراج->name;
        idx++;
    }

    keymap->کلیدیں = کلیدیں;
    أرجع true;
}

ساكن bool
CopyKeycodeNameLUT(بنية xkb_keymap *keymap, کلید_ناں_Info *info)
{
    Keycode_میل *میل;
    xkb_atom_t name;
    darray_enumerate(name, میل, info->keycodes.ناں) {
        إذا (!میل->لبھیا)
            استمر;

        إذا (میل->is_alias) {
            /*
             * Do some sanity checking on the aliases. We can’t do it before
             * because keys and their aliases may be added out-of-order.
             */

            /* Check that ->real is a key. */
            ثابت Keycode_میل میل_real =
                keycode_store_lookup_name(&info->keycodes, میل->alias.real);
            إذا (!میل_real.لبھیا) {
                log_vrb(info->ctx, XKB_LOG_VERBOSITY_DETAILED,
                        XKB_WARNING_UNDEFINED_KEYCODE,
                        "Attempt to alias %s to non-existent key %s; Ignored\n",
                        کلید_Name_متن(info->ctx, name),
                        کلید_Name_متن(info->ctx, میل->alias.real));
                میل->لبھیا = false;
                استمر;
            }
            أكد(!میل_real.is_alias);
        } وإلا إذا (!میل->کلید.کم) {
            /* Update to final index in keymap::keys */
            میل->کلید.اشاریہ += keymap->عدد_کلیدیں_کم;
        }
    }

    darray_shrink(info->keycodes.ناں);
    keymap->عدد_کلید_ناں = darray_ناپ(info->keycodes.ناں);
    darray_steal(info->keycodes.ناں, &keymap->کلید_ناں, عدم);
    darray_init(info->keycodes.ناں);
    أرجع true;
}

ساكن bool
Copy_Led_ناں_منزل_Keymap(بنية xkb_keymap *keymap, کلید_ناں_Info *info)
{
    keymap->عدد_leds = info->عدد_led_ناں;
    لكل (xkb_led_اشاریہ_t idx = 0; idx < info->عدد_led_ناں; idx++) {
        LedNameInfo *ledi = &info->led_ناں[idx];

        إذا (ledi->name == XKB_ATOM_NONE)
            استمر;

        keymap->leds[idx].name = ledi->name;
    }

    أرجع true;
}

ساكن bool
Copy_کلید_ناں_Info_منزل_Keymap(بنية xkb_keymap *keymap, کلید_ناں_Info *info)
{
    /* This function trashes keymap on error, but that's OK. */
    إذا (!Copy_کلید_ناں_منزل_Keymap(keymap, info) ||
        !CopyKeycodeNameLUT(keymap, info) ||
        !Copy_Led_ناں_منزل_Keymap(keymap, info))
        أرجع false;

    /*
     * Choose an undefined keycode as an intermediary value to encode special
     * “auto” value of RedirectKey(), because there is no space left in the
     * action type to store a flag.
     */
    إذا (!keymap->عدد_کلیدیں ||
        keymap->min_کلید_کوڈ > 0) {
        keymap->redirect_کلید_auto = 0;
    } وإلا {
        /* Slow path */
        xkb_keycode_t keycode = XKB_KEYCODE_INVALID - 1;
        لكل (xkb_keycode_t k = keymap->عدد_کلیدیں; k-- > keymap->عدد_کلیدیں_کم;) {
            إذا (keycode > keymap->کلیدیں[k].keycode)
                اكسر;
            /* Try next keycode downwards */
            keycode = keymap->کلیدیں[k].keycode - 1;
        }
        keymap->redirect_کلید_auto = keycode;
    }

    keymap->keycodes_section_name = strdup_محفوظ(info->name);
    XkbEscapeMapName(keymap->keycodes_section_name);
    أرجع true;
}

/***====================================================================***/

bool
CompileKeycodes(XkbFile *file, بنية xkb_keymap_info *keymap_info)
{
    کلید_ناں_Info info;

    Init_کلید_ناں_Info(&info, keymap_info, 0);

    إذا (file != عدم)
        HandleKeycodesFile(&info, file);

    إذا (info.error_گنتی != 0)
        اذهب err_info;

    إذا (!Copy_کلید_ناں_Info_منزل_Keymap(&keymap_info->keymap, &info))
        اذهب err_info;

    Clear_کلید_ناں_Info(&info);
    أرجع true;

err_info:
    Clear_کلید_ناں_Info(&info);
    أرجع false;
}
