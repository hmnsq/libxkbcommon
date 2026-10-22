/*
 * For HPND:
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * For MIT:
 * Copyright © 2012 Intel Corporation
 *
 * SPDX-License-Identifier: HPND AND MIT
 *
 * Author: Daniel Stone <daniel@fooishbar.org>
 */

#شامل "config.h"

#شامل <assert.h>
#شامل <stdint.h>
#شامل <stdio.h>
#شامل <stdlib.h>

#شامل "xkbcommon/xkbcommon-status.h"
#شامل "xkbcommon/xkbcommon-keysyms.h"
#شامل "xkbcommon/xkbcommon.h"
#شامل "atom.h"
#شامل "action.h"
#شامل "context.h"
#شامل "darray.h"
#شامل "keymap.h"
#شامل "messages-codes.h"
#شامل "text.h"
#شامل "util-numbers.h"
#شامل "xkbcomp-priv.h"

#تعریف_کرو BUF_CHUNK_سائز 4096

ساخت buf {
    حرف *buf;
    سائز_t سائز;
    سائز_t alloc;
};

#تعریف_کرو xkb_abs(n) _Generic((n),                \
                            صحیح: مطلق_قدر,           \
                            طویل صحیح: labs,     \
                            طےشدہ: llabs )((n))

ساکن bool
do_realloc(ساخت buf *buf, سائز_t at_کم_سے_کم)
{
    buf->alloc += BUF_CHUNK_سائز;
    اگر (at_کم_سے_کم >= BUF_CHUNK_سائز)
        buf->alloc += at_کم_سے_کم;

    حرف *ثابت new = realloc(buf->buf, buf->alloc);
    اگر (!new)
        واپس false;

    buf->buf = new;
    واپس true;
}

ATTR_PRINTF(2, 3) ساکن bool
check_write_buf(ساخت buf *buf, ثابت حرف *fmt, ...)
{
    va_list دلائل;
    صحیح printed;
    سائز_t دستیاب;

    دستیاب = buf->alloc - buf->سائز;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    أكد(buf->buf || دستیاب == 0);
    va_آغاز(دلائل, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->سائز : buf->buf, دستیاب,
                        fmt, دلائل);
    va_انتہا(دلائل);

    اگر (printed < 0) {
        /* Some error */
        جاؤ err;
    } ورنہ اگر ((سائز_t)printed >= دستیاب) {
        /* Not enough space: need to realloc */
        اگر (!do_realloc(buf, (سائز_t)printed + 1))
            جاؤ err;

        /* The buffer has enough space now. */
        دستیاب = buf->alloc - buf->سائز;
        أكد(buf->buf && (سائز_t) printed < دستیاب);
        va_آغاز(دلائل, fmt);
        printed = vsnprintf(buf->buf + buf->سائز, دستیاب, fmt, دلائل);
        va_انتہا(دلائل);

        اگر (printed < 0 || (سائز_t) printed >= دستیاب)
            جاؤ err;
    }

    buf->سائز += (سائز_t)printed;
    واپس true;

err:
    آزاد(buf->buf);
    buf->buf = تہی;
    واپس false;
}

#تعریف_کرو write_buf(buf, ...) کرو { \
    اگر (!check_write_buf(buf, __VA_ARGS__)) \
        واپس false; \
} جب (0)

ساکن bool
check_copy_مقصد_buf(ساخت buf *buf, ثابت حرف* ماخذ, سائز_t len)
{
    اگر (len == 0)
        واپس true;

    ثابت سائز_t دستیاب = buf->alloc - buf->سائز;
    اگر (len >= دستیاب &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        آزاد(buf->buf);
        buf->buf = تہی;
        واپس false;
    }

    حافظہ_نقل(buf->buf + buf->سائز, ماخذ, len);
    buf->سائز += len;
    /* Append NULL byte */
    buf->buf[buf->سائز] = '\0';
    واپس true;
}

#تعریف_کرو copy_مقصد_buf_len(buf, ماخذ, len) کرو { \
    اگر (!check_copy_مقصد_buf(buf, ماخذ, len)) \
        واپس false; \
} جب (0)

#تعریف_کرو copy_مقصد_buf(buf, ماخذ) \
    copy_مقصد_buf_len(buf, ماخذ, حجم(ماخذ) - 1)

ساکن bool
check_write_string_literal(ساخت buf *buf, ثابت حرف* string)
{
    copy_مقصد_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    سائز_t pending_آغاز = 0;
    سائز_t current = 0;
    حرف escape_بفر[] = {'\\', '0', '0', '0', '0'};
    برائے (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        انتخاب (string[current]) {
        حالت '\n':
            /* `\n` would break strings */
            escape_بفر[1] = 'n';
            escape_len = 2;
            توڑو;
        حالت '\\':
            /* `\` would create escape sequences */
            escape_بفر[1] = '\\';
            escape_len = 2;
            توڑو;
        طےشدہ:
            /*
             * Handle `"` (would break strings) and ASCII control characters
             * with an octal escape sequence. Xorg xkbcomp does not parse the
             * escape sequence `\"` nor xkbcommon < 1.9.0, so in order to be
             * backward compatible we must use the octal escape sequence in
             * xkbcomp style `\0nnn` with *4* digits:
             *
             * 1. To be compatible with Xorg xkbcomp.
             * 2. To avoid issues with the next char: e.g. "\00427" should not
             *    be emitted as "\427" nor "\0427".
             *
             * Note that xkbcommon < 1.9.0 will still not parse this correctly,
             * although it will not raise an error: e.g. the escape for `"`,
             * `\0042`, would be parsed as `\004` + `2`.
             */
            اگر (unlikely((غیرعلامتی حرف)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_بفر[1] = '0';
                escape_بفر[2] = '0';
                escape_بفر[3] = (حرف)('0' + (string[current] >> 3));
                escape_بفر[4] = (حرف)('0' + (string[current] & 0x7));
                escape_len = 5;
            } ورنہ {
                /* Expand the chunk */
                جاری;
            }
        }
        /* Write pending chunk */
        أكد(current >= pending_آغاز);
        copy_مقصد_buf_len(buf, string + pending_آغاز, current - pending_آغاز);
        /* Write escape sequence */
        copy_مقصد_buf_len(buf, escape_بفر, escape_len);
        pending_آغاز = current + 1;
    }
    /* Write remaining chunk */
    أكد(current >= pending_آغاز);
    copy_مقصد_buf_len(buf, string + pending_آغاز, current - pending_آغاز);

    copy_مقصد_buf(buf, "\"");
    واپس true;
}

#تعریف_کرو write_buf_string_literal(buf, string) کرو { \
    اگر (!check_write_string_literal(buf, string)) \
        واپس false; \
} جب (0)

ساکن خالی
delete_آخری_char(ساخت buf *buf, حرف c)
{
    اگر (buf->سائز && buf->buf[buf->سائز - 1] == c)
        buf->buf[--buf->سائز] = '\0';
}

/** Entry for a name substituion LUT */
ساخت کلید_name_substitution {
    xkb_atom_t ماخذ;
    xkb_atom_t ہدف;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
ساکن صحیح
cmp_کلید_name_substitution(ثابت خالی *a, ثابت خالی *b)
{
    ثابت xkb_atom_t s1 = ((ساخت کلید_name_substitution *) a)->ماخذ;
    ثابت xkb_atom_t s2 = ((ساخت کلید_name_substitution *) b)->ماخذ;
    واپس (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

قسمتعریف darray(ساخت کلید_name_substitution) کلید_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
ساکن bool
rename_long_کلیدیں(ثابت ساخت xkb_keymap *keymap,
                 کلید_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    شمار {
        پہلا = '!',
        آخری = '0',
        invalid = '\0'
    };
    حرف prefixes[] = {
        پہلا, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', آخری
    };
    static_assert(حجم(prefixes) == آخری - پہلا + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ثابت ساخت xkb_کلید *کلید;
    xkb_کلیدیں_foreach(کلید, keymap) {
        اگر (کلید->name == XKB_ATOM_NONE)
            جاری;

        ثابت حرف * ثابت name = xkb_atom_متن(keymap->ctx, کلید->name);
        ثابت سائز_t len = strlen_محفوظ(name);
        اگر (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (ساخت کلید_name_substitution) {
                .ماخذ = کلید->name,
                .keycode = کلید->keycode,
                .ہدف = کلید->name
            });
        } ورنہ اگر (len == 4 && name[0] >= پہلا && name[0] <= آخری) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - پہلا] = invalid;
        }
    }

    /* Check aliases */
    برائے (darray_سائز_t a = 0; a < keymap->عدد_کلید_aliases; a++) {
        ثابت ساخت xkb_کلید_alias * ثابت alias = &keymap->کلید_aliases[a];
        ثابت حرف * ثابت name = xkb_atom_متن(keymap->ctx, alias->alias);
        ثابت سائز_t len = strlen_محفوظ(name);
        اگر (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (ساخت کلید_name_substitution) {
                .ماخذ = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .ہدف = alias->alias
            });
        } ورنہ اگر (len == 4 && name[0] >= پہلا && name[0] <= آخری) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - پہلا] = invalid;
        }
    }

    اگر darray_خالی_خانے(*substitutions) {
        /* Nothing to do */
        واپس true;
    }

    /*
     * Create non-conflicting X11-compatible new names
     *
     * It’s one of the following 2 forms:
     * - If key name and keycode ≤ 0xfff: prefix character + hexadecimal keycode
     * - If alias or keycode > 0xfff: prefix character + hexadecimal incrementing
     *   counter.
     * Prefixes are distinct to avoid any name conflict.
     */

    /* Get first available prefix */
    حرف *سابقہ = prefixes;
    ثابت حرف *سابقہ_max = prefixes + حجم(prefixes) - 1;
    جب (*سابقہ == invalid && سابقہ < سابقہ_max)
        سابقہ++;

    /* Keycode prefix: prefer ‘0’ if available */
    حرف keycode_سابقہ;
    اگر (prefixes[آخری - پہلا] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_سابقہ = آخری;
        prefixes[آخری - پہلا] = invalid;
    } ورنہ {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_سابقہ = *سابقہ;
        اگر (unlikely(keycode_سابقہ == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            واپس true;
        }
        /* Update current prefix */
        *سابقہ = invalid;
        جب (*سابقہ == invalid && سابقہ < سابقہ_max)
            سابقہ++;
    }

    /* Proceed renaming */
    uint16_t اگلا_id = 1;
    ساخت کلید_name_substitution *sub;
    حرف buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        اگر (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, حجم(buf), "%c%03"PRIx32,
                     keycode_سابقہ, sub->keycode);
            ثابت xkb_atom_t ہدف = xkb_atom_intern(keymap->ctx,
                                                      buf, حجم(buf) - 2);
            اگر (ہدف == XKB_ATOM_NONE)
                جاؤ error;
            sub->ہدف = ہدف;
        } ورنہ اگر (*سابقہ != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, حجم(buf), "%c%03"PRIx16, *سابقہ, اگلا_id);
            ثابت xkb_atom_t ہدف = xkb_atom_intern(keymap->ctx,
                                                      buf, حجم(buf) - 2);
            اگر (ہدف == XKB_ATOM_NONE)
                جاؤ error;
            sub->ہدف = ہدف;
            اگر (++اگلا_id > 0xfff) {
                *سابقہ = invalid;
                /* Get next available prefix */
                جب (*سابقہ == invalid && سابقہ < سابقہ_max)
                    سابقہ++;
                /* Reset counter */
                اگلا_id = 1;
            }
        } ورنہ {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->ہدف = sub->ماخذ;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_اشیاء(*substitutions), darray_سائز(*substitutions),
          حجم(*darray_اشیاء(*substitutions)), &cmp_کلید_name_substitution);

    واپس true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_تخصیص_FAILURE_2,
            "Cannot allocate key name substitution\n");
    واپس false;
}

/** Substitute a key name with a valid X11 key name */
ساکن xkb_atom_t
substitute_name(ثابت کلید_name_substitutions *substitutions, xkb_atom_t name)
{
    /*ثنائی تلاش*/
    darray_سائز_t پہلا = 0;
    darray_سائز_t آخری = darray_سائز(*substitutions);

    جب (آخری > پہلا) {
        darray_سائز_t mid = پہلا + (آخری - 1 - پہلا) / 2;
        ثابت xkb_atom_t ماخذ = darray_شے(*substitutions, mid).ماخذ;
        اگر (ماخذ < name)
            پہلا = mid + 1;
        ورنہ اگر (ماخذ > name)
            آخری = mid;
        ورنہ /* found it */
            واپس darray_شے(*substitutions, mid).ہدف;
    }

    /* No name substitution: return original name */
    واپس name;
}

ساکن bool
write_vmods(ثابت ساخت xkb_keymap *keymap, شمار xkb_keymap_format format,
            bool explicit, ساخت buf *buf)
{
    ثابت ساخت xkb_mod *mod;
    xkb_mod_اشاریہ_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        اگر (!has_some) {
            copy_مقصد_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } ورنہ {
            copy_مقصد_buf(buf, ",");
        }
        write_buf(buf, "%s", xkb_atom_متن(keymap->ctx, mod->name));

        /*
         * Ensure to always honor explicit mappings when auto canonical vmods
         * is enabled, so that parsing/serializing can round-trip. This is not
         * required when the feature is disabled (keymap format v1).
         *
         * If the auto feature is enabled, `virtual_modifiers M;` will assign
         * a canonical mapping > 0xff to `M` if there is no further explicit/
         * implicit mapping for `M`.
         *
         * On the contrary, `virtual_modifiers M = none;` prevents the auto
         * mapping of `M`. So if the auto feature is enabled and there is no
         * further explicit/implicit mapping for `M`, then the mapping of `M`
         * will remain unchanged: `none`. It must be serialized to an explicit
         * mapping to prevent triggering the auto canonical mapping presented
         * above.
         */
        ثابت bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_متن_V2);

        اگر ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
             explicit) &&
            (auto_canonical_mods || mod->mapping != 0)) {
            /*
             * Explicit non-default mapping
             * NOTE: we can only pretty-print *real* modifiers in this context.
             */
            write_buf(buf, "=%s",
                      Mod_نقاب_متن(keymap->ctx, MOD_REAL, &keymap->mods,
                                  mod->mapping));
        }
    }

    اگر (has_some)
        copy_مقصد_buf(buf, ";\n\n");

    واپس true;
}

ساکن bool
write_keycodes(ثابت ساخت xkb_keymap *keymap,
               ثابت کلید_name_substitutions *substitutions,
               شمار xkb_keymap_serialize_flags flags, ساخت buf *buf)
{
    ثابت ساخت xkb_کلید *کلید;
    xkb_led_اشاریہ_t idx;
    ثابت ساخت xkb_led *led;

    ثابت bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    اگر (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    ورنہ
        copy_مقصد_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_کلید_رمز, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_کلید_رمز, 255));

    xkb_کلیدیں_foreach(کلید, keymap) {
        اگر (کلید->name == XKB_ATOM_NONE)
            جاری;

        ثابت xkb_atom_t name = (substitutions == تہی)
            ? کلید->name
            : substitute_name(substitutions, کلید->name);
        اگر (pretty)
            write_buf(buf, "\t%-20s", کلید_Name_متن(keymap->ctx, name));
        ورنہ
            write_buf(buf, "\t%s", کلید_Name_متن(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", کلید->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        اگر (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_متن(keymap->ctx, led->name));
            copy_مقصد_buf(buf, ";\n");
        }

    برائے (darray_سائز_t i = 0; i < keymap->عدد_کلید_aliases; i++) {
        ثابت xkb_atom_t alias = (substitutions == تہی)
            ? keymap->کلید_aliases[i].alias
            : substitute_name(substitutions, keymap->کلید_aliases[i].alias);
        ثابت xkb_atom_t real = (substitutions == تہی)
            ? keymap->کلید_aliases[i].real
            : substitute_name(substitutions, keymap->کلید_aliases[i].real);

        اگر (pretty)
            write_buf(buf, "\talias %-14s",
                      کلید_Name_متن(keymap->ctx, alias));
        ورنہ
            write_buf(buf, "\talias %s",
                      کلید_Name_متن(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  کلید_Name_متن(keymap->ctx, real));
    }

    copy_مقصد_buf(buf, "};\n\n");
    واپس true;
}

ساکن bool
write_types(ثابت ساخت xkb_keymap *keymap, شمار xkb_keymap_format format,
            شمار xkb_keymap_serialize_flags flags, ساخت buf *buf)
{
    اگر (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    ورنہ
        copy_مقصد_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    اگر (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        واپس false;

    ثابت bool drop_غیر_مستعمل = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_غیر_مستعمل);
    برائے (darray_سائز_t i = 0; i < keymap->عدد_types; i++) {
        ثابت ساخت xkb_کلید_type * ثابت type = &keymap->types[i];
        اگر (!type->required && drop_غیر_مستعمل)
            جاری;

        copy_مقصد_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_متن(keymap->ctx, type->name));
        copy_مقصد_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        برائے (darray_سائز_t j = 0; j < type->عدد_entries; j++) {
            ثابت حرف *str;
            ثابت ساخت xkb_کلید_type_اندراج *اندراج_2 = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            اگر (اندراج_2->level == 0 && اندراج_2->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_اقدار))
                جاری;

            str = Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              اندراج_2->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, اندراج_2->level + 1);

            اگر (اندراج_2->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           اندراج_2->preserve.mods));
        }

        برائے (xkb_level_اشاریہ_t n = 0; n < type->عدد_level_نام; n++)
            اگر (type->level_نام[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_متن(keymap->ctx, type->level_نام[n]));
                copy_مقصد_buf(buf, ";\n");
            }

        copy_مقصد_buf(buf, "\t};\n");
    }

    copy_مقصد_buf(buf, "};\n\n");
    واپس true;
}

ساکن bool
write_led_map(ثابت ساخت xkb_keymap *keymap, شمار xkb_keymap_format format,
              bool defaults, ساخت buf *buf, ثابت ساخت xkb_led *led)
{
    copy_مقصد_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_متن(keymap->ctx, led->name));
    copy_مقصد_buf(buf, " {\n");

    اگر (led->which_groups) {
        اگر (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      Led_State_نقاب_متن(keymap->ctx, گروہ_Component_نقاب_نام,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    اگر (led->which_mods) {
        اگر (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      Led_State_نقاب_متن(keymap->ctx, mod_Component_نقاب_نام,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    اگر (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  Control_نقاب_متن(keymap->ctx, format, led->ctrls));
    }

    copy_مقصد_buf(buf, "\t};\n");
    واپس true;
}

ساکن ثابت حرف *
affect_تالا_متن(شمار xkb_action_flags flags, bool show_both)
{
    انتخاب (flags & (ACTION_تالا_NO_تالا | ACTION_تالا_NO_UNLOCK)) {
    حالت 0:
        واپس show_both ? ",affect=both" : "";
    حالت ACTION_تالا_NO_UNLOCK:
        واپس ",affect=lock";
    حالت ACTION_تالا_NO_تالا:
        واپس ",affect=unlock";
    حالت ACTION_تالا_NO_تالا | ACTION_تالا_NO_UNLOCK:
        واپس ",affect=neither";
    طےشدہ:
        واپس "";
    }
}

#تعریف_کرو علامت_PADDING 15
#تعریف_کرو ACTION_PADDING 30

ساکن bool
write_action(ثابت ساخت xkb_keymap *keymap, شمار xkb_keymap_format format,
             xkb_layout_اشاریہ_t max_groups, ساخت buf *buf,
             ثابت اتحاد xkb_action *action,
             ثابت حرف *سابقہ, ثابت حرف *suffix)
{
    ثابت حرف *type;
    ثابت حرف *دلائل = تہی;

    اگر (!سابقہ)
        سابقہ = "";
    اگر (!suffix)
        suffix = "";

    type = Action_Type_متن(action->type);

    انتخاب (action->type) {
    حالت ACTION_TYPE_MOD_سیٹ:
    حالت ACTION_TYPE_MOD_LATCH:
    حالت ACTION_TYPE_MOD_تالا:
        اگر (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            دلائل = "modMapMods";
        ورنہ
            دلائل = Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        اگر (unlockOnPress && !is_Mods_Un_تالا_On_Press_Supported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    Action_Type_متن(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->گروہ.flags & ACTION_LATCH_ON_PRESS);
        اگر (latchOnPress && !isModsLatchOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot use \"LatchMods(latchOnPress=true)\" "
                    "in keymap format %d\n", format);
            latchOnPress = false;
        }
        write_buf(buf, "%s%s(modifiers=%s%s%s%s%s%s)%s", سابقہ, type, دلائل,
                  (action->type != ACTION_TYPE_MOD_تالا && (action->mods.flags & ACTION_تالا_CLEAR)) ? ",clearLocks" : "",
                  (action->type == ACTION_TYPE_MOD_LATCH && (action->mods.flags & ACTION_LATCH_مقصد_تالا)) ? ",latchToLock" : "",
                  (action->type == ACTION_TYPE_MOD_تالا) ? affect_تالا_متن(action->mods.flags, false) : "",
                  (unlockOnPress) ? ",unlockOnPress" : "",
                  (latchOnPress) ? ",latchOnPress" : "",
                  suffix);
        توڑو;

    حالت ACTION_TYPE_گروہ_سیٹ:
    حالت ACTION_TYPE_گروہ_LATCH:
    حالت ACTION_TYPE_گروہ_تالا:
        اگر ((uint32_t) xkb_abs(action->گروہ.گروہ) <
            (max_groups + !(action->گروہ.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool تالا_On_Release = action->type == ACTION_TYPE_گروہ_تالا &&
                                 (action->گروہ.flags & ACTION_تالا_ON_RELEASE);
            اگر (تالا_On_Release && !is_گروہ_تالا_On_Release_Supported(format)) {
                log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                        "Cannot use \"GroupLock(lockOnRelease=true)\" "
                        "in keymap format %d\n", format);
                تالا_On_Release = false;
            }
            write_buf(buf, "%s%s(group=%s%"PRId32"%s%s%s)%s", سابقہ, type,
                      (!(action->گروہ.flags & ACTION_ABSOLUTE_SWITCH) && action->گروہ.گروہ >= 0) ? "+" : "",
                      (action->گروہ.flags & ACTION_ABSOLUTE_SWITCH) ? action->گروہ.گروہ + 1 : action->گروہ.گروہ,
                      (action->type != ACTION_TYPE_گروہ_تالا && (action->گروہ.flags & ACTION_تالا_CLEAR)) ? ",clearLocks" : "",
                      (action->type == ACTION_TYPE_گروہ_LATCH && (action->گروہ.flags & ACTION_LATCH_مقصد_تالا)) ? ",latchToLock" : "",
                      (تالا_On_Release) ? ",lockOnRelease" : "",
                      suffix);
        } ورنہ {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->گروہ.گروہ +
                       !!(action->گروہ.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            جاؤ void_action;
        }
        توڑو;

    حالت ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", سابقہ, type, suffix);
        توڑو;

    حالت ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", سابقہ, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        توڑو;

    حالت ACTION_TYPE_PTR_تالا:
        دلائل = affect_تالا_متن(action->btn.flags, true);
        /* fallthrough */
    حالت ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", سابقہ, type);
        static_assert(XKB_اشارہ_گر_BUTTON_DEFAULT < XKB_اشارہ_گر_BUTTON_MIN, "");
        اگر (action->btn.button > XKB_اشارہ_گر_BUTTON_DEFAULT &&
            action->btn.button <= XKB_اشارہ_گر_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        ورنہ
            copy_مقصد_buf(buf, "default");
        اگر (action->btn.گنتی) {
            أكد(action->type != ACTION_TYPE_PTR_تالا);
            write_buf(buf, ",count=%u", action->btn.گنتی);
        }
        اگر (دلائل)
            write_buf(buf, "%s", دلائل);
        write_buf(buf, ")%s", suffix);
        توڑو;

    حالت ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", سابقہ, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.قدر >= 0) ? "+" : "",
                  action->dflt.قدر);
        write_buf(buf, ")%s", suffix);
        توڑو;

    حالت ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", سابقہ, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        توڑو;

    حالت ACTION_TYPE_CTRL_سیٹ:
    حالت ACTION_TYPE_CTRL_تالا:
        write_buf(buf, "%s%s(controls=%s%s)%s", سابقہ, type,
                  Control_نقاب_متن(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_تالا
                    ? affect_تالا_متن(action->ctrls.flags, false)
                    : ""),
                  suffix);
        توڑو;

    حالت ACTION_TYPE_REDIRECT_کلید: {
        write_buf(buf, "%s%s(", سابقہ, type);
        ثابت ساخت xkb_کلید * ثابت کلید = Xkb_کلید(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        اگر (کلید)
            write_buf(buf, "keycode=%s", کلید_Name_متن(keymap->ctx, کلید->name));
        اگر (action->redirect.affect_mods) {
            xkb_mod_نقاب_t نقاب;
            نقاب = (action->redirect.affect_mods & action->redirect.mods);
            اگر (نقاب)
                write_buf(buf, ",modifiers=%s",
                          Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods, نقاب));
            نقاب = (action->redirect.affect_mods & ~action->redirect.mods);
            اگر (نقاب)
                write_buf(buf, ",clearMods=%s",
                          Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods, نقاب));
        }
        write_buf(buf, ")%s", suffix);
        توڑو;
    }

    حالت ACTION_TYPE_UNKNOWN:
    حالت ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", سابقہ, suffix);
        توڑو;

    حالت ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        اگر (format == XKB_KEYMAP_FORMAT_متن_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      سابقہ, suffix);
        ورنہ
            write_buf(buf, "%sVoidAction()%s", سابقہ, suffix);
        توڑو;

    طےشدہ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندرونی == 20 &&
                      ACTION_TYPE_اندرونی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        أكد(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  سابقہ, type, action->type, action->priv.ڈیٹا[0],
                  action->priv.ڈیٹا[1], action->priv.ڈیٹا[2],
                  action->priv.ڈیٹا[3], action->priv.ڈیٹا[4],
                  action->priv.ڈیٹا[5], action->priv.ڈیٹا[6],
                  suffix);
        توڑو;
    }

    واپس true;
}

ساکن ثابت اتحاد xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

ساکن bool
write_actions(ثابت ساخت xkb_keymap *keymap, شمار xkb_keymap_format format,
              xkb_layout_اشاریہ_t max_groups, ساخت buf *buf, ساخت buf *buf2,
              ثابت ساخت xkb_کلید *کلید, xkb_layout_اشاریہ_t گروہ)
{
    ساکن ثابت اتحاد xkb_action noAction = { .type = ACTION_TYPE_NONE };

    برائے (xkb_level_اشاریہ_t level = 0; level < Xkb_کلید_عدد_سطحیں(کلید, گروہ);
         level++) {

        اگر (level != 0)
            copy_مقصد_buf(buf, ", ");

        ثابت اتحاد xkb_action *actions = تہی;
        xkb_action_گنتی_t گنتی = xkb_keymap_کلید_get_actions_by_level(
            keymap, کلید, گروہ, level, &actions
        );

        اگر (گنتی > 1 && format < XKB_KEYMAP_FORMAT_متن_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            گنتی = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->سائز = 0;
        اگر (گنتی == 0) {
            اگر (!write_action(keymap, format, max_groups,
                              buf2, &noAction, تہی, تہی))
                واپس false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        ورنہ اگر (گنتی == 1) {
            اگر (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), تہی, تہی))
                واپس false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        ورنہ {
            copy_مقصد_buf(buf2, "{ ");
            برائے (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
                اگر (k != 0)
                    copy_مقصد_buf(buf2, ", ");
                سائز_t old_سائز = buf2->سائز;
                اگر (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), تہی, تہی))
                    واپس false;
                /* Check if padding is necessary */
                اگر (buf2->سائز >= old_سائز + ACTION_PADDING)
                    جاری;
                /* Compute and write padding, then write the action again */
                ثابت صحیح padding = (صحیح)(old_سائز + ACTION_PADDING - buf2->سائز);
                buf2->سائز = old_سائز;
                write_buf(buf2, "%*s", padding, "");
                اگر (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), تہی, تہی))
                    واپس false;
            }
            copy_مقصد_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    واپس true;
}

ساکن bool
write_action_defaults(ثابت ساخت xkb_keymap *keymap,
                      شمار xkb_keymap_format format,
                      ثابت اتحاد xkb_action *action,
                      ساخت buf *buf)
{
    ثابت حرف *type = Action_Type_متن(action->type);

#تعریف_کرو سابقہ_2 "\t"

    انتخاب (action->type) {
    حالت ACTION_TYPE_MOD_سیٹ:
    حالت ACTION_TYPE_MOD_LATCH:
    حالت ACTION_TYPE_MOD_تالا:
        أكد(action->mods.flags == 0);
        أكد(action->mods.mods.mods == 0);
        write_buf(buf, سابقہ_2"%s.modifiers = 0;\n", type);
        اگر (action->type != ACTION_TYPE_MOD_تالا)
            write_buf(buf, سابقہ_2"%s.clearLocks = false;\n", type);
        اگر (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, سابقہ_2"%s.latchOnPress = false;\n", type);
        اگر (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, سابقہ_2"%s.latchToLock = false;\n", type);
        اگر (action->type == ACTION_TYPE_MOD_تالا)
            write_buf(buf, سابقہ_2"%s.affect = both;\n", type);
        اگر (is_Mods_Un_تالا_On_Press_Supported(format))
            write_buf(buf, سابقہ_2"%s.unlockOnPress = false;\n", type);
        توڑو;

    حالت ACTION_TYPE_گروہ_سیٹ:
    حالت ACTION_TYPE_گروہ_LATCH:
    حالت ACTION_TYPE_گروہ_تالا:
        أكد(action->گروہ.flags == 0);
        أكد(action->گروہ.گروہ == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, سابقہ_2"%s.group = +0;\n", type);
        اگر (action->type != ACTION_TYPE_گروہ_تالا)
            write_buf(buf, سابقہ_2"%s.clearLocks = false;\n", type);
        اگر (action->type == ACTION_TYPE_گروہ_LATCH)
            write_buf(buf, سابقہ_2"%s.latchToLock = false;\n", type);
        اگر (action->type == ACTION_TYPE_گروہ_تالا &&
            is_گروہ_تالا_On_Release_Supported(format))
            write_buf(buf, سابقہ_2"%s.lockOnRelease = false;\n", type);
        توڑو;


    حالت ACTION_TYPE_PTR_MOVE:
        أكد(action->ptr.x == 0);
        أكد(action->ptr.y == 0);
        أكد(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, سابقہ_2"%s.x = +0;\n", type);
        write_buf(buf, سابقہ_2"%s.y = +0;\n", type);
        write_buf(buf, سابقہ_2"%s.accel = true;\n", type);
        توڑو;

    حالت ACTION_TYPE_PTR_تالا:
    حالت ACTION_TYPE_PTR_BUTTON:
        أكد(action->btn.flags == 0);
        أكد(action->btn.button == 0);
        أكد(action->btn.گنتی == 0);
        write_buf(buf, سابقہ_2"%s.button = 0;\n", type);
        write_buf(buf, سابقہ_2"%s.count = 0;\n", type);
        اگر (action->type == ACTION_TYPE_PTR_تالا)
            write_buf(buf, سابقہ_2"%s.affect = both;\n", type);
        توڑو;

    حالت ACTION_TYPE_PTR_DEFAULT:
        أكد(action->dflt.flags == 0);
        أكد(action->dflt.قدر == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, سابقہ_2"%s.button = +1;\n", type);
        توڑو;

    حالت ACTION_TYPE_SWITCH_VT:
        أكد(action->screen.flags == ACTION_SAME_SCREEN);
        أكد(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, سابقہ_2"%s.screen = +0;\n", type);
        write_buf(buf, سابقہ_2"%s.same = true;\n", type);
        توڑو;

    حالت ACTION_TYPE_CTRL_سیٹ:
    حالت ACTION_TYPE_CTRL_تالا:
        أكد(action->ctrls.flags == 0);
        أكد(action->ctrls.ctrls == 0);
        write_buf(buf, سابقہ_2"%s.controls = 0;\n", type);
        اگر (action->type == ACTION_TYPE_CTRL_تالا)
            write_buf(buf, سابقہ_2"%s.affect = both;\n", type);
        توڑو;

    حالت ACTION_TYPE_REDIRECT_کلید:
        أكد(action->redirect.keycode == keymap->redirect_کلید_auto);
        أكد(action->redirect.affect_mods == 0);
        أكد(action->redirect.mods == 0);
        write_buf(buf, سابقہ_2"%s.keycode = auto;\n", type);
        write_buf(buf, سابقہ_2"%s.modifiers = 0;\n", type);
        write_buf(buf, سابقہ_2"%s.clearMods = 0;\n", type);
        توڑو;

    طےشدہ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندرونی == 20 &&
                      ACTION_TYPE_اندرونی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");
    }

#تعریف_منسوخ سابقہ_2

    واپس true;
}

ساکن bool
write_actions_defaults(ثابت ساخت xkb_keymap *keymap,
                       شمار xkb_keymap_format format, ساخت buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    برائے (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_عدد_ENTRIES; a++) {
        ثابت اتحاد xkb_action * محدود ثابت action = &defaults.actions[a];
        اگر (!write_action_defaults(keymap, format, action, buf))
            واپس false;
    }
    copy_مقصد_buf(buf, "\n");

    واپس true;
}

ساکن ثابت ساخت xkb_sym_interpret fallback_interpret = {
    .sym = XKB_کلید_Void_علامت,
    .required = true,
    .repeat = FALLBACK_INTERPRET_کلید_REPEAT,
    .مطابقت = مطابقت_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_اشاریہ_t) DEFAULT_INTERPRET_VMOD,
    .عدد_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ساکن bool
write_compat(ثابت ساخت xkb_keymap * محدود keymap,
             شمار xkb_keymap_format format, xkb_layout_اشاریہ_t max_groups,
             شمار xkb_keymap_serialize_flags flags, bool * محدود some_interp,
             ساخت buf *buf)
{
    ثابت bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_اقدار);
    ثابت bool drop_غیر_مستعمل = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_غیر_مستعمل);
    ثابت bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_اقدار) && drop_غیر_مستعمل);
    /* TODO: print all default values if the explicit flag is set */

    اگر (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    ورنہ
        copy_مقصد_buf(buf, "xkb_compatibility {\n");

    اگر (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        واپس false;

    اگر (defaults && !write_actions_defaults(keymap, format, buf))
        واپس false;

    copy_مقصد_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_مقصد_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    ثابت bool use_fallback_interpret =
        (!keymap->عدد_sym_interprets || drop_interprets);
    ثابت darray_سائز_t عدد_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->عدد_sym_interprets);
    ثابت ساخت xkb_sym_interpret* ثابت sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    برائے (darray_سائز_t i = 0; i < عدد_sym_interprets; i++) {
        ثابت ساخت xkb_sym_interpret *si = &sym_interprets[i];
        اگر (!si->required && drop_غیر_مستعمل)
            جاری;

        *some_interp = true;

        copy_مقصد_buf(buf, "\tinterpret ");
        اگر (!si->sym) {
            copy_مقصد_buf(buf, "Any");
        } ورنہ اگر (pretty) {
            write_buf(buf, "%s", Keysym_متن(keymap->ctx, si->sym));
        } ورنہ {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatch_متن(si->مطابقت),
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        اگر (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      Mod_اشاریہ_متن(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        اگر (si->level_one_only) {
            copy_مقصد_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } ورنہ اگر (defaults) {
            copy_مقصد_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        اگر (si->repeat) {
            copy_مقصد_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } ورنہ اگر (defaults) {
            copy_مقصد_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        ثابت اتحاد xkb_action *action = تہی;
        xkb_action_گنتی_t action_گنتی = si->عدد_actions;
        اگر (action_گنتی <= 1) {
            action = &si->a.action;
        } ورنہ اگر (format < XKB_KEYMAP_FORMAT_متن_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_گنتی = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        اگر (action_گنتی > 1) {
            copy_مقصد_buf(buf, "\n\t\taction= {");
            ثابت حرف suffix[] = ", ";
            برائے (xkb_action_گنتی_t k = 0; k < si->عدد_actions; k++) {
                اگر (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    واپس false;
            }
            buf->سائز -= حجم(suffix) - 1; /* trailing comma */
            copy_مقصد_buf(buf, "};");
            has_explicit_properties = true;
        } ورنہ اگر (action_گنتی == 1) {
            اگر (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                واپس false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    اگر (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        عدد_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].مطابقت == fallback_interpret.مطابقت &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].عدد_actions == fallback_interpret.عدد_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    ثابت ساخت xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        اگر ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            واپس false;
        }
    }

    copy_مقصد_buf(buf, "};\n\n");

    واپس true;
}

ساکن bool
write_keysyms(ثابت ساخت xkb_keymap *keymap,
              ساخت buf *buf, ساخت buf *buf2,
              ثابت ساخت xkb_کلید *کلید, xkb_layout_اشاریہ_t گروہ,
              bool pretty, bool show_actions)
{
    ثابت غیرعلامتی صحیح padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : علامت_PADDING)
        : 0;
    برائے (xkb_level_اشاریہ_t level = 0; level < Xkb_کلید_عدد_سطحیں(کلید, گروہ);
         level++) {
        ثابت xkb_keysym_t *syms;
        صحیح عدد_syms;

        اگر (level != 0)
            copy_مقصد_buf(buf, ", ");

        عدد_syms = xkb_keymap_کلید_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (ساخت xkb_keymap *)keymap,
            کلید->keycode, گروہ, level, &syms
        );

        ثابت xkb_keysym_t no_علامت = XKB_کلید_No_علامت;
        اگر (عدد_syms == 0) {
            عدد_syms = 1;
            syms = &no_علامت;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        اگر (عدد_syms == 1) {
            اگر (pretty || syms[0] == XKB_کلید_No_علامت)
                write_buf(buf, "%*s", padding, Keysym_متن(keymap->ctx, syms[0]));
            ورنہ
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } ورنہ {
            اگر (pretty) {
                buf2->سائز = 0;
                copy_مقصد_buf(buf2, "{ ");
                برائے (صحیح s = 0; s < عدد_syms; s++) {
                    اگر (s != 0)
                        copy_مقصد_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              Keysym_متن(keymap->ctx, syms[s]));
                }
                copy_مقصد_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } ورنہ {
                copy_مقصد_buf(buf, "{");
                برائے (صحیح s = 0; s < عدد_syms; s++) {
                    اگر (s != 0)
                        copy_مقصد_buf(buf, ",");
                    اگر (syms[s] == XKB_کلید_No_علامت)
                        copy_مقصد_buf(buf, "NoSymbol");
                    ورنہ
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_مقصد_buf(buf, "}");
            }
        }
    }

    واپس true;
}

ساکن bool
write_کلید(ثابت ساخت xkb_keymap *keymap,
          ثابت ساخت xkb_keymap_serialize_config *config,
          ثابت کلید_name_substitutions *substitutions,
          xkb_layout_اشاریہ_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          ساخت buf *buf, ساخت buf *buf2, ثابت ساخت xkb_کلید *کلید)
{
    bool simple = true;

    xkb_layout_نقاب_t layouts = config->layouts;
    اگر (popcount32(layouts) != keymap->عدد_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_اشاریہ_t l = 0;
        برائے (xkb_layout_نقاب_t ls = layouts; ls; ls >>= 1, l++) {
            اگر (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ثابت xkb_layout_اشاریہ_t effective =
                    xkb_keymap_کلید_effective_layout(کلید, l);
                اگر (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } ورنہ {
        layouts &= ((UINT64_C(1) << کلید->عدد_groups) - 1);
    }
    ثابت xkb_layout_اشاریہ_t groups_گنتی = popcount32(layouts);

    ثابت xkb_atom_t name = (substitutions == تہی)
        ? کلید->name
        : substitute_name(substitutions, کلید->name);

    اگر (pretty)
        write_buf(buf, "\tkey %-20s {", کلید_Name_متن(keymap->ctx, name));
    ورنہ
        write_buf(buf, "\tkey %s {", کلید_Name_متن(keymap->ctx, name));

    اگر ((کلید->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        اگر (!explicit) {
            برائے (xkb_layout_اشاریہ_t گروہ = 1; گروہ < کلید->عدد_groups; گروہ++) {
                اگر (!((UINT32_C(1) << گروہ) & layouts))
                    جاری;

                اگر (کلید->groups[گروہ].type != کلید->groups[0].type) {
                    multi_type = true;
                    توڑو;
                }
            }
        }

        اگر (multi_type) {
            xkb_layout_اشاریہ_t new_گروہ = (xkb_layout_اشاریہ_t)-1;
            برائے (xkb_layout_اشاریہ_t گروہ = 0; گروہ < کلید->عدد_groups; گروہ++) {
                اگر (!((UINT32_C(1) << گروہ) & layouts))
                    جاری;

                new_گروہ++;

                اگر (!کلید->groups[گروہ].explicit_type && !explicit)
                    جاری;

                ثابت ساخت xkb_کلید_type * ثابت type = کلید->groups[گروہ].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_گروہ + 1);
                write_buf_string_literal(
                  buf, xkb_atom_متن(keymap->ctx, type->name));
                copy_مقصد_buf(buf, ",");
            }
        }
        ورنہ {
            ثابت ساخت xkb_کلید_type * ثابت type = کلید->groups[0].type;
            write_buf(buf, "\n\t\ttype= ");
            write_buf_string_literal(
                buf, xkb_atom_متن(keymap->ctx, type->name));
            copy_مقصد_buf(buf, ",");
        }
    }

    /*
     * NOTE: we use key->explicit and not key->group[i].explicit_actions, in
     * order to have X11 and the previous versions of xkbcommon (without this
     * group property) parse the keymap as intended, by setting explicitly for
     * this key all actions in all groups.
     *
     * One side effect is that no interpretation will be run on this key anymore,
     * so we may have to set some extra fields explicitly: repeat, virtualMods.
     */
    ثابت bool explicit_actions = (کلید->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #تعریف_کرو require_explicit(کلید, prop,                                       \
                             قدر, initial_قدر, default_interpret_قدر) ( \
        /* Explicit value in the source */                                    \
        ((کلید)->explicit & (prop)) ||                                         \
        /* Explicit serialization flag */                                     \
        explicit ||                                                           \
        (                                                                     \
            (                                                                 \
                /* Value is not set by interprets due to explicit actions */  \
                explicit_actions ||                                           \
                /* Value is set by an interpret, but no interpret remains:    \
                 * print the value if it is not the default interpret value,  \
                 * unless it cannot trigger due to implicit actions forcibly  \
                 * made explicit */                                           \
                ((کلید)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (قدر) != (initial_قدر) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_قدر) != (default_interpret_قدر)                  \
            )                                                                 \
        )                                                                     \
    )

    اگر (require_explicit(کلید, EXPLICIT_REPEAT, کلید->repeats,
                         DEFAULT_کلید_REPEAT, DEFAULT_INTERPRET_کلید_REPEAT)) {
        اگر (کلید->repeats)
            copy_مقصد_buf(buf, "\n\t\trepeat= Yes,");
        ورنہ
            copy_مقصد_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    اگر (require_explicit(کلید, EXPLICIT_VMODMAP, کلید->vmodmap,
                         (xkb_mod_نقاب_t) DEFAULT_کلید_VMODMAP,
                         (xkb_mod_نقاب_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              کلید->vmodmap));
        simple = false;
    }

    #تعریف_منسوخ require_explicit

    انتخاب (کلید->برآمد_کا_range_گروہ_policy) {
    حالت XKB_LAYOUT_برآمد_کا_RANGE_CLAMP:
        copy_مقصد_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        توڑو;

    حالت XKB_LAYOUT_برآمد_کا_RANGE_REDIRECT: {
        اگر (!groups_گنتی) {
            /* No layout to redirect to: skip */
            توڑو;
        }

        xkb_layout_نقاب_t layout_نقاب =
            (UINT32_C(1) << کلید->برآمد_کا_range_گروہ_نمبر);
        اگر (!(layouts & layout_نقاب)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     کلید_Name_متن(keymap->ctx, name),
                     کلید->برآمد_کا_range_گروہ_نمبر + 1);
            layout_نقاب = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ثابت xkb_layout_نقاب_t کم = (layouts & (layout_نقاب - 1));
        ثابت xkb_layout_اشاریہ_t new = popcount32(کم);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        توڑو;
    }

    حالت XKB_LAYOUT_برآمد_کا_RANGE_WRAP:
    طےشدہ:
        توڑو;
    }

    اگر (کلید->overlays) {
        xkb_overlay_نقاب_t بقایا = کلید->overlays;

        ثابت xkb_overlay_اشاریہ_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_گنتی == 8, "invalid right shift");
        ثابت xkb_overlay_نقاب_t valid =
            (xkb_overlay_نقاب_t)((1u << overlay_max) - 1u);
        بقایا &= valid;
        اگر (بقایا != کلید->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_اشاریہ,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, کلید_Name_متن(keymap->ctx, name),
                     بقایا, کلید->overlays & ~valid);
        }

        اگر (بقایا && !کلید->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                بقایا &
                (xkb_overlay_نقاب_t)((غیرعلامتی)~بقایا + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_اشاریہ_t overlay =
                (xkb_overlay_اشاریہ_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     کلید_Name_متن(keymap->ctx, name), overlay, بقایا & ~lsb);
            بقایا = lsb;
        }

        اگر (بقایا)
            simple = false;

        ثابت ساخت xkb_کلید * ثابت *overlays_کلیدیں = (کلید->overlays_inline)
                                                    ? &کلید->overlay_کلید
                                                    : کلید->overlays_کلیدیں;
        جب (بقایا) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                بقایا &
                (xkb_overlay_نقاب_t)((غیرعلامتی)~بقایا + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_اشاریہ_t overlay =
                (xkb_overlay_اشاریہ_t)popcount32(lsb - 1u);
            بقایا = (بقایا & ~lsb);

            /* pop current value */
            اگر (کلید->overlays_inline && بقایا)
                PANIC_UNREACHABLE();
            ثابت ساخت xkb_کلید *overlay_کلید = *(overlays_کلیدیں++);

            ثابت xkb_atom_t overlay_کلید_name = (substitutions == تہی)
                ? overlay_کلید->name
                : substitute_name(substitutions, overlay_کلید->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, کلید_Name_متن(keymap->ctx, overlay_کلید_name));
        }
    }

    اگر (groups_گنتی > 1 || explicit_actions || explicit)
        simple = false;

    اگر (simple) {
        اگر (groups_گنتی == 0) {
            /* Remove trailing comma */
            delete_آخری_char(buf, ',');
            اگر (buf->buf[buf->سائز - 1] == ',')
                buf->سائز--;
        } ورنہ {
            copy_مقصد_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_اشاریہ_t گروہ = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            گروہ = xkb_keymap_کلید_effective_layout(کلید, گروہ);
            اگر (!write_keysyms(keymap, buf, buf2, کلید, گروہ, pretty, false))
                واپس false;
            copy_مقصد_buf(buf, " ]");
        }
        copy_مقصد_buf(buf, " };\n");
    }
    ورنہ {
        xkb_layout_اشاریہ_t new_گروہ = (xkb_layout_اشاریہ_t)-1;
        برائے (xkb_layout_اشاریہ_t گروہ = 0; گروہ < کلید->عدد_groups; گروہ++) {
            اگر (!((UINT32_C(1) << گروہ) & layouts))
                جاری;

            new_گروہ++;

            ثابت bool print_actions =
                /* Group has explicit actions */
                کلید->groups[گروہ].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (کلید->groups[گروہ].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (کلید->groups[گروہ].implicit_actions && !some_interprets);

            اگر (new_گروہ != 0)
                copy_مقصد_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_گروہ + 1);

            اگر (!write_keysyms(keymap, buf, buf2, کلید, گروہ,
                               pretty, print_actions))
                واپس false;
            copy_مقصد_buf(buf, " ]");
            اگر (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_گروہ + 1);
                اگر (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, کلید, گروہ))
                    واپس false;
                copy_مقصد_buf(buf, " ]");
            }
        }
        اگر (!groups_گنتی)
            delete_آخری_char(buf, ',');
        copy_مقصد_buf(buf, "\n\t};\n");
    }

    واپس true;
}


ساکن صحیح
cmp_modmap_قدم_1(ثابت خالی *a, ثابت خالی *b)
{
    ثابت Mod_Map_اندراج *m1 = (ثابت Mod_Map_اندراج *)a;
    ثابت Mod_Map_اندراج *m2 = (ثابت Mod_Map_اندراج *)b;

    اگر (!m1->mods) {
        اگر (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            واپس 0;
        }
        واپس +1;
    } ورنہ اگر (!m2->mods) {
        واپس -1;
    }

    /* Valid only on step 1 */
    اگر (m1->کلید_رمز < m2->کلید_رمز)
        واپس -1;
    ورنہ اگر (m1->کلید_رمز > m2->کلید_رمز)
        واپس +1;

    اگر (m1->mods < m2->mods)
        واپس -1;
    ورنہ اگر (m1->mods > m2->mods)
        واپس +1;

    اگر (m1->have_علامت < m2->have_علامت)
        واپس +1;
    ورنہ اگر (m1->have_علامت > m2->have_علامت)
        واپس -1;

    اگر (m1->have_علامت) {
        اگر (m1->u.کلید_Sym < m2->u.کلید_Sym)
            واپس -1;
        اگر (m1->u.کلید_Sym > m2->u.کلید_Sym)
            واپس +1;
    } ورنہ {
        اگر (m1->u.کلید_Name < m2->u.کلید_Name)
            واپس -1;
        اگر (m1->u.کلید_Name > m2->u.کلید_Name)
            واپس +1;
    }

    واپس 0;
}

ساکن صحیح
cmp_modmap_قدم_2(ثابت خالی *a, ثابت خالی *b)
{
    ثابت Mod_Map_اندراج *m1 = (ثابت Mod_Map_اندراج *)a;
    ثابت Mod_Map_اندراج *m2 = (ثابت Mod_Map_اندراج *)b;

    اگر (m1->mods < m2->mods)
        واپس -1;
    اگر (m1->mods > m2->mods)
        واپس +1;

    اگر (m1->have_علامت < m2->have_علامت)
        واپس -1;
    اگر (m1->have_علامت > m2->have_علامت)
        واپس +1;

    اگر (m1->have_علامت) {
        اگر (m1->u.کلید_Sym < m2->u.کلید_Sym)
            واپس -1;
        اگر (m1->u.کلید_Sym > m2->u.کلید_Sym)
            واپس +1;
    } ورنہ {
        اگر (m1->کلید_رمز < m2->کلید_رمز)
            واپس -1;
        اگر (m1->کلید_رمز > m2->کلید_رمز)
            واپس +1;
    }

    واپس 0;
}

ساکن bool
write_modmaps(ثابت ساخت xkb_keymap *keymap, شمار xkb_keymap_format format,
              ثابت کلید_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, ساخت buf *buf)
{
    اگر (!keymap->عدد_modmaps)
        واپس true;

    darray(Mod_Map_اندراج) modmaps = darray_new();
    darray_from_اشیاء(modmaps, keymap->modmaps, keymap->عدد_modmaps);
    اگر (!darray_اشیاء(modmaps))
        واپس false;

    /* Sort entries by key code */
    qsort(darray_اشیاء(modmaps), darray_سائز(modmaps),
          حجم(*darray_اشیاء(modmaps)), &cmp_modmap_قدم_1);

    /* Drop disabled entries at the end */
    برائے (darray_سائز_t m = darray_سائز(modmaps); m-- > 0;) {
        اگر (darray_شے(modmaps, m).mods)
            توڑو;
        darray_سائز(modmaps)--;
    }

    /*
     * Build detailed keymap modmap
     *
     * NOTE: use an explicit for-loop instead of the darray_foreach macro
     * because we may add entries while iterating it. Iterate from the end,
     * so that we do not iterate the new entries.
     */
    xkb_keycode_t current_keycode = XKB_KEYCODE_INVALID;
    bool convert_مقصد_v1_compatibility_اندراج = false;
    برائے (darray_سائز_t m = darray_سائز(modmaps); m-- > 0;) {
        Mod_Map_اندراج *mm = &darray_شے(modmaps, m);
        أكد(mm->mods);

        اگر (mm->کلید_رمز == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ثابت xkb_mod_نقاب_t core_mods = (mm->mods & MOD_REAL_نقاب_ALL);
            اگر (convert_مقصد_v1_compatibility_اندراج && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } ورنہ {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            جاری;
        }

        current_keycode = mm->کلید_رمز;
        ثابت ساخت xkb_کلید *کلید = Xkb_کلید(keymap, mm->کلید_رمز);

        ثابت xkb_mod_نقاب_t core_mods = (کلید->modmap & MOD_REAL_نقاب_ALL);
        اگر (format == XKB_KEYMAP_FORMAT_متن_V1 &&
            ((کلید->modmap & (کلید->modmap - 1)) || کلید->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_مقصد_v1_compatibility_اندراج = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_نقاب,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    کلید_Name_متن(keymap->ctx, کلید->name), کلید->modmap);
        } ورنہ {
            /* V2 format or V1 format with a single core modifier */
            أكد(کلید->modmap);
            mm->mods = کلید->modmap;
            /* Force key name */
            mm->have_علامت = false;
            mm->u.کلید_Name = کلید->name;
            convert_مقصد_v1_compatibility_اندراج = false;
        }
    }

    اگر (darray_خالی_خانے(modmaps))
        جاؤ باہر_نکلو;

    /* Sort entries by modifier mask */
    qsort(darray_اشیاء(modmaps), darray_سائز(modmaps),
          حجم(*darray_اشیاء(modmaps)), &cmp_modmap_قدم_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_نقاب_t current_mods = 0;
    ثابت Mod_Map_اندراج *اندراج_2;
    darray_foreach(اندراج_2, modmaps) {
        اگر (!اندراج_2->mods)
            جاری;

        اگر (current_mods != اندراج_2->mods) {
            اگر (current_mods) {
                copy_مقصد_buf(buf, " };\n");
            }
            current_mods = اندراج_2->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      Mod_نقاب_متن(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } ورنہ {
            copy_مقصد_buf(buf, ", ");
        }

        اگر (اندراج_2->have_علامت) {
            اگر (pretty || اندراج_2->u.کلید_Sym == XKB_کلید_No_علامت)
                write_buf(buf, "%s", Keysym_متن(keymap->ctx, اندراج_2->u.کلید_Sym));
            ورنہ
                write_buf(buf, "0x%"PRIx32, اندراج_2->u.کلید_Sym);
        } ورنہ {
            ثابت xkb_atom_t name = (substitutions == تہی)
                ? اندراج_2->u.کلید_Name
                : substitute_name(substitutions, اندراج_2->u.کلید_Name);
            write_buf(buf, "%s", کلید_Name_متن(keymap->ctx, name));
        }
    }
    اگر (current_mods) {
        copy_مقصد_buf(buf, " };\n");
    }

باہر_نکلو:
    darray_free(modmaps);
    واپس true;
}

ساکن bool
write_symbols(ثابت ساخت xkb_keymap *keymap,
              ثابت ساخت xkb_keymap_serialize_config *config,
              ثابت کلید_name_substitutions *substitutions,
              xkb_layout_اشاریہ_t max_groups, bool some_interp,
              ساخت buf *buf)
{
    ثابت bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_اقدار);
    ثابت bool explicit_کلید_اقدار =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_اقدار);
    ثابت bool drop_غیر_مستعمل = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_غیر_مستعمل);
    ثابت bool drop_interprets = (explicit_کلید_اقدار && drop_غیر_مستعمل);
    /* TODO: print all default values if explicit flag is set */

    اگر (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    ورنہ
        copy_مقصد_buf(buf, "xkb_symbols {\n");

    bool has_گروہ_نام = false;
    xkb_layout_اشاریہ_t new_گروہ = (xkb_layout_اشاریہ_t)-1;
    برائے (xkb_layout_اشاریہ_t گروہ = 0; گروہ < keymap->عدد_گروہ_نام; گروہ++) {
        اگر (!((UINT32_C(1) << گروہ) & config->layouts))
            جاری;

        new_گروہ++;

        اگر (keymap->گروہ_نام[گروہ]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_گروہ + 1);
            write_buf_string_literal(
                buf, xkb_atom_متن(keymap->ctx, keymap->گروہ_نام[گروہ]));
            copy_مقصد_buf(buf, ";\n");
            has_گروہ_نام = true;
        }
    }
    اگر (has_گروہ_نام)
        copy_مقصد_buf(buf, "\n");

    اگر (defaults && !write_actions_defaults(keymap, config->format, buf))
        واپس false;

    ساخت buf buf2 = { تہی, 0, 0 };
    ثابت ساخت xkb_کلید *کلید;
    xkb_کلیدیں_foreach(کلید, keymap) {
        /* Skip keys with no explicit values */
        اگر (کلید->explicit &&
            !write_کلید(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_کلید_اقدار, pretty, buf,
                       &buf2, کلید))
        {
            آزاد(buf2.buf);
            واپس false;
        }
    }
    آزاد(buf2.buf);

    اگر (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        واپس false;

    copy_مقصد_buf(buf, "};\n\n");
    واپس true;
}

ساکن شمار xkb_حالت
write_keymap(ثابت ساخت xkb_keymap *keymap,
             ثابت ساخت xkb_keymap_serialize_config *config,
             ساخت buf *buf, ساخت xkb_keymap_serialize_نتیجہ *نتیجہ)
{
    کلید_name_substitutions substitutions = darray_new();
    اگر (config->format == XKB_KEYMAP_FORMAT_متن_V1 &&
        !rename_long_کلیدیں(keymap, &substitutions))
    {
        واپس false;
    }

    ثابت کلید_name_substitutions * ثابت substitutions_ptr =
        (darray_خالی_خانے(substitutions)) ? تہی : &substitutions;

    ثابت xkb_layout_اشاریہ_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    ثابت bool ok = (
        check_write_buf(buf, "xkb_keymap {\n") &&
        write_keycodes(keymap, substitutions_ptr, config->flags, buf) &&
        write_types(keymap, config->format, config->flags, buf) &&
        write_compat(keymap, config->format, max_groups, config->flags,
                     &some_interp, buf) &&
        write_symbols(keymap, config, substitutions_ptr, max_groups,
                      some_interp, buf) &&
        check_write_buf(buf, "};\n")
    );

    darray_free(substitutions);
    واپس ok ? XKB_SUCCESS : -1;
}

شمار xkb_حالت
متن_v1_keymap_serialize(
        ثابت ساخت xkb_keymap *keymap,
        ثابت ساخت xkb_keymap_serialize_config *config,
        ساخت xkb_keymap_serialize_نتیجہ *نتیجہ
)
{
    ساخت buf buf = { تہی, 0, 0 };

    ثابت شمار xkb_حالت حالت_2 =
        write_keymap(keymap, config, &buf, نتیجہ);
    اگر (حالت_2 != XKB_SUCCESS) {
        آزاد(buf.buf);
        نتیجہ->serialized = تہی;
        واپس حالت_2;
    }

    نتیجہ->serialized = buf.buf;
    نتیجہ->لمبائی = buf.سائز + 1;
    نتیجہ->layouts = config->layouts;

    واپس XKB_SUCCESS;
}
