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

#ضمن "config.h"

#ضمن <assert.h>
#ضمن <stdint.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>

#ضمن "xkbcommon/xkbcommon-status.h"
#ضمن "xkbcommon/xkbcommon-keysyms.h"
#ضمن "xkbcommon/xkbcommon.h"
#ضمن "atom.h"
#ضمن "action.h"
#ضمن "context.h"
#ضمن "darray.h"
#ضمن "keymap.h"
#ضمن "messages-codes.h"
#ضمن "text.h"
#ضمن "util-numbers.h"
#ضمن "xkbcomp-priv.h"

#تعریف_کرو BUF_CHUNK_ناپ 4096

بنية buf {
    حرف *buf;
    ناپ_t ناپ;
    ناپ_t alloc;
};

#تعریف_کرو xkb_abs(n) _Generic((n),                \
                            صحيح: مطلق_قدر,           \
                            لما صحيح: labs,     \
                            افتراضي: llabs )((n))

ساكن bool
do_realloc(بنية buf *buf, ناپ_t at_گھٹ_توں_گھٹ)
{
    buf->alloc += BUF_CHUNK_ناپ;
    إذا (at_گھٹ_توں_گھٹ >= BUF_CHUNK_ناپ)
        buf->alloc += at_گھٹ_توں_گھٹ;

    حرف *ثابت new = realloc(buf->buf, buf->alloc);
    إذا (!new)
        أرجع false;

    buf->buf = new;
    أرجع true;
}

ATTR_PRINTF(2, 3) ساكن bool
check_write_buf(بنية buf *buf, ثابت حرف *fmt, ...)
{
    va_list دلیلاں;
    صحيح printed;
    ناپ_t دستیاب;

    دستیاب = buf->alloc - buf->ناپ;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    أكد(buf->buf || دستیاب == 0);
    va_شروع(دلیلاں, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->ناپ : buf->buf, دستیاب,
                        fmt, دلیلاں);
    va_سرا(دلیلاں);

    إذا (printed < 0) {
        /* Some error */
        اذهب err;
    } وإلا إذا ((ناپ_t)printed >= دستیاب) {
        /* Not enough space: need to realloc */
        إذا (!do_realloc(buf, (ناپ_t)printed + 1))
            اذهب err;

        /* The buffer has enough space now. */
        دستیاب = buf->alloc - buf->ناپ;
        أكد(buf->buf && (ناپ_t) printed < دستیاب);
        va_شروع(دلیلاں, fmt);
        printed = vsnprintf(buf->buf + buf->ناپ, دستیاب, fmt, دلیلاں);
        va_سرا(دلیلاں);

        إذا (printed < 0 || (ناپ_t) printed >= دستیاب)
            اذهب err;
    }

    buf->ناپ += (ناپ_t)printed;
    أرجع true;

err:
    حرر(buf->buf);
    buf->buf = عدم;
    أرجع false;
}

#تعریف_کرو write_buf(buf, ...) افعل { \
    إذا (!check_write_buf(buf, __VA_ARGS__)) \
        أرجع false; \
} طالما (0)

ساكن bool
check_copy_منزل_buf(بنية buf *buf, ثابت حرف* سروت, ناپ_t len)
{
    إذا (len == 0)
        أرجع true;

    ثابت ناپ_t دستیاب = buf->alloc - buf->ناپ;
    إذا (len >= دستیاب &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        حرر(buf->buf);
        buf->buf = عدم;
        أرجع false;
    }

    حافظہ_نقل(buf->buf + buf->ناپ, سروت, len);
    buf->ناپ += len;
    /* Append NULL byte */
    buf->buf[buf->ناپ] = '\0';
    أرجع true;
}

#تعریف_کرو copy_منزل_buf_len(buf, سروت, len) افعل { \
    إذا (!check_copy_منزل_buf(buf, سروت, len)) \
        أرجع false; \
} طالما (0)

#تعریف_کرو copy_منزل_buf(buf, سروت) \
    copy_منزل_buf_len(buf, سروت, حجم(سروت) - 1)

ساكن bool
check_write_string_literal(بنية buf *buf, ثابت حرف* string)
{
    copy_منزل_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    ناپ_t pending_شروع = 0;
    ناپ_t current = 0;
    حرف escape_بفر[] = {'\\', '0', '0', '0', '0'};
    لكل (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        بدل (string[current]) {
        حالة '\n':
            /* `\n` would break strings */
            escape_بفر[1] = 'n';
            escape_len = 2;
            اكسر;
        حالة '\\':
            /* `\` would create escape sequences */
            escape_بفر[1] = '\\';
            escape_len = 2;
            اكسر;
        افتراضي:
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
            إذا (unlikely((بنانشانی حرف)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_بفر[1] = '0';
                escape_بفر[2] = '0';
                escape_بفر[3] = (حرف)('0' + (string[current] >> 3));
                escape_بفر[4] = (حرف)('0' + (string[current] & 0x7));
                escape_len = 5;
            } وإلا {
                /* Expand the chunk */
                استمر;
            }
        }
        /* Write pending chunk */
        أكد(current >= pending_شروع);
        copy_منزل_buf_len(buf, string + pending_شروع, current - pending_شروع);
        /* Write escape sequence */
        copy_منزل_buf_len(buf, escape_بفر, escape_len);
        pending_شروع = current + 1;
    }
    /* Write remaining chunk */
    أكد(current >= pending_شروع);
    copy_منزل_buf_len(buf, string + pending_شروع, current - pending_شروع);

    copy_منزل_buf(buf, "\"");
    أرجع true;
}

#تعریف_کرو write_buf_string_literal(buf, string) افعل { \
    إذا (!check_write_string_literal(buf, string)) \
        أرجع false; \
} طالما (0)

ساكن فراغ
delete_آخری_char(بنية buf *buf, حرف c)
{
    إذا (buf->ناپ && buf->buf[buf->ناپ - 1] == c)
        buf->buf[--buf->ناپ] = '\0';
}

/** Entry for a name substituion LUT */
بنية کلید_name_substitution {
    xkb_atom_t سروت;
    xkb_atom_t نشانہ;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
ساكن صحيح
cmp_کلید_name_substitution(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت xkb_atom_t s1 = ((بنية کلید_name_substitution *) a)->سروت;
    ثابت xkb_atom_t s2 = ((بنية کلید_name_substitution *) b)->سروت;
    أرجع (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

تعريفنوع darray(بنية کلید_name_substitution) کلید_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
ساكن bool
rename_long_کلیدیں(ثابت بنية xkb_keymap *keymap,
                 کلید_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    تعداد {
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
    ثابت بنية xkb_کلید *کلید;
    xkb_کلیدیں_foreach(کلید, keymap) {
        إذا (کلید->name == XKB_ATOM_NONE)
            استمر;

        ثابت حرف * ثابت name = xkb_atom_متن(keymap->ctx, کلید->name);
        ثابت ناپ_t len = strlen_محفوظ(name);
        إذا (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (بنية کلید_name_substitution) {
                .سروت = کلید->name,
                .keycode = کلید->keycode,
                .نشانہ = کلید->name
            });
        } وإلا إذا (len == 4 && name[0] >= پہلا && name[0] <= آخری) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - پہلا] = invalid;
        }
    }

    /* Check aliases */
    لكل (darray_ناپ_t a = 0; a < keymap->عدد_کلید_aliases; a++) {
        ثابت بنية xkb_کلید_alias * ثابت alias = &keymap->کلید_aliases[a];
        ثابت حرف * ثابت name = xkb_atom_متن(keymap->ctx, alias->alias);
        ثابت ناپ_t len = strlen_محفوظ(name);
        إذا (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (بنية کلید_name_substitution) {
                .سروت = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .نشانہ = alias->alias
            });
        } وإلا إذا (len == 4 && name[0] >= پہلا && name[0] <= آخری) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - پہلا] = invalid;
        }
    }

    إذا darray_خالی_تھاواں(*substitutions) {
        /* Nothing to do */
        أرجع true;
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
    حرف *اگیتر = prefixes;
    ثابت حرف *اگیتر_max = prefixes + حجم(prefixes) - 1;
    طالما (*اگیتر == invalid && اگیتر < اگیتر_max)
        اگیتر++;

    /* Keycode prefix: prefer ‘0’ if available */
    حرف keycode_اگیتر;
    إذا (prefixes[آخری - پہلا] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_اگیتر = آخری;
        prefixes[آخری - پہلا] = invalid;
    } وإلا {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_اگیتر = *اگیتر;
        إذا (unlikely(keycode_اگیتر == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            أرجع true;
        }
        /* Update current prefix */
        *اگیتر = invalid;
        طالما (*اگیتر == invalid && اگیتر < اگیتر_max)
            اگیتر++;
    }

    /* Proceed renaming */
    uint16_t اگلا_id = 1;
    بنية کلید_name_substitution *sub;
    حرف buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        إذا (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, حجم(buf), "%c%03"PRIx32,
                     keycode_اگیتر, sub->keycode);
            ثابت xkb_atom_t نشانہ = xkb_atom_intern(keymap->ctx,
                                                      buf, حجم(buf) - 2);
            إذا (نشانہ == XKB_ATOM_NONE)
                اذهب error;
            sub->نشانہ = نشانہ;
        } وإلا إذا (*اگیتر != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, حجم(buf), "%c%03"PRIx16, *اگیتر, اگلا_id);
            ثابت xkb_atom_t نشانہ = xkb_atom_intern(keymap->ctx,
                                                      buf, حجم(buf) - 2);
            إذا (نشانہ == XKB_ATOM_NONE)
                اذهب error;
            sub->نشانہ = نشانہ;
            إذا (++اگلا_id > 0xfff) {
                *اگیتر = invalid;
                /* Get next available prefix */
                طالما (*اگیتر == invalid && اگیتر < اگیتر_max)
                    اگیتر++;
                /* Reset counter */
                اگلا_id = 1;
            }
        } وإلا {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->نشانہ = sub->سروت;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_اشیاء(*substitutions), darray_ناپ(*substitutions),
          حجم(*darray_اشیاء(*substitutions)), &cmp_کلید_name_substitution);

    أرجع true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ونڈ_FAILURE_2,
            "Cannot allocate key name substitution\n");
    أرجع false;
}

/** Substitute a key name with a valid X11 key name */
ساكن xkb_atom_t
substitute_name(ثابت کلید_name_substitutions *substitutions, xkb_atom_t name)
{
    /*ثنائی تلاش*/
    darray_ناپ_t پہلا = 0;
    darray_ناپ_t آخری = darray_ناپ(*substitutions);

    طالما (آخری > پہلا) {
        darray_ناپ_t mid = پہلا + (آخری - 1 - پہلا) / 2;
        ثابت xkb_atom_t سروت = darray_شے(*substitutions, mid).سروت;
        إذا (سروت < name)
            پہلا = mid + 1;
        وإلا إذا (سروت > name)
            آخری = mid;
        وإلا /* found it */
            أرجع darray_شے(*substitutions, mid).نشانہ;
    }

    /* No name substitution: return original name */
    أرجع name;
}

ساكن bool
write_vmods(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
            bool explicit, بنية buf *buf)
{
    ثابت بنية xkb_mod *mod;
    xkb_mod_اشاریہ_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        إذا (!has_some) {
            copy_منزل_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } وإلا {
            copy_منزل_buf(buf, ",");
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

        إذا ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
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

    إذا (has_some)
        copy_منزل_buf(buf, ";\n\n");

    أرجع true;
}

ساكن bool
write_keycodes(ثابت بنية xkb_keymap *keymap,
               ثابت کلید_name_substitutions *substitutions,
               تعداد xkb_keymap_serialize_flags flags, بنية buf *buf)
{
    ثابت بنية xkb_کلید *کلید;
    xkb_led_اشاریہ_t idx;
    ثابت بنية xkb_led *led;

    ثابت bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    إذا (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    وإلا
        copy_منزل_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_کلید_کوڈ, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_کلید_کوڈ, 255));

    xkb_کلیدیں_foreach(کلید, keymap) {
        إذا (کلید->name == XKB_ATOM_NONE)
            استمر;

        ثابت xkb_atom_t name = (substitutions == عدم)
            ? کلید->name
            : substitute_name(substitutions, کلید->name);
        إذا (pretty)
            write_buf(buf, "\t%-20s", کلید_Name_متن(keymap->ctx, name));
        وإلا
            write_buf(buf, "\t%s", کلید_Name_متن(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", کلید->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        إذا (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_متن(keymap->ctx, led->name));
            copy_منزل_buf(buf, ";\n");
        }

    لكل (darray_ناپ_t i = 0; i < keymap->عدد_کلید_aliases; i++) {
        ثابت xkb_atom_t alias = (substitutions == عدم)
            ? keymap->کلید_aliases[i].alias
            : substitute_name(substitutions, keymap->کلید_aliases[i].alias);
        ثابت xkb_atom_t real = (substitutions == عدم)
            ? keymap->کلید_aliases[i].real
            : substitute_name(substitutions, keymap->کلید_aliases[i].real);

        إذا (pretty)
            write_buf(buf, "\talias %-14s",
                      کلید_Name_متن(keymap->ctx, alias));
        وإلا
            write_buf(buf, "\talias %s",
                      کلید_Name_متن(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  کلید_Name_متن(keymap->ctx, real));
    }

    copy_منزل_buf(buf, "};\n\n");
    أرجع true;
}

ساكن bool
write_types(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
            تعداد xkb_keymap_serialize_flags flags, بنية buf *buf)
{
    إذا (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    وإلا
        copy_منزل_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    إذا (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        أرجع false;

    ثابت bool drop_بن_ورتیا = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_بن_ورتیا);
    لكل (darray_ناپ_t i = 0; i < keymap->عدد_types; i++) {
        ثابت بنية xkb_کلید_type * ثابت type = &keymap->types[i];
        إذا (!type->required && drop_بن_ورتیا)
            استمر;

        copy_منزل_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_متن(keymap->ctx, type->name));
        copy_منزل_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        لكل (darray_ناپ_t j = 0; j < type->عدد_entries; j++) {
            ثابت حرف *str;
            ثابت بنية xkb_کلید_type_اندراج *اندراج = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            إذا (اندراج->level == 0 && اندراج->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_اقدار))
                استمر;

            str = Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              اندراج->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, اندراج->level + 1);

            إذا (اندراج->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           اندراج->preserve.mods));
        }

        لكل (xkb_level_اشاریہ_t n = 0; n < type->عدد_level_ناں; n++)
            إذا (type->level_ناں[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_متن(keymap->ctx, type->level_ناں[n]));
                copy_منزل_buf(buf, ";\n");
            }

        copy_منزل_buf(buf, "\t};\n");
    }

    copy_منزل_buf(buf, "};\n\n");
    أرجع true;
}

ساكن bool
write_led_map(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
              bool defaults, بنية buf *buf, ثابت بنية xkb_led *led)
{
    copy_منزل_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_متن(keymap->ctx, led->name));
    copy_منزل_buf(buf, " {\n");

    إذا (led->which_groups) {
        إذا (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      Led_State_نقاب_متن(keymap->ctx, گروہ_Component_نقاب_ناں,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    إذا (led->which_mods) {
        إذا (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      Led_State_نقاب_متن(keymap->ctx, mod_Component_نقاب_ناں,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    إذا (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  Control_نقاب_متن(keymap->ctx, format, led->ctrls));
    }

    copy_منزل_buf(buf, "\t};\n");
    أرجع true;
}

ساكن ثابت حرف *
affect_جندرا_متن(تعداد xkb_action_flags flags, bool show_both)
{
    بدل (flags & (ACTION_جندرا_NO_جندرا | ACTION_جندرا_NO_UNLOCK)) {
    حالة 0:
        أرجع show_both ? ",affect=both" : "";
    حالة ACTION_جندرا_NO_UNLOCK:
        أرجع ",affect=lock";
    حالة ACTION_جندرا_NO_جندرا:
        أرجع ",affect=unlock";
    حالة ACTION_جندرا_NO_جندرا | ACTION_جندرا_NO_UNLOCK:
        أرجع ",affect=neither";
    افتراضي:
        أرجع "";
    }
}

#تعریف_کرو علامت_PADDING 15
#تعریف_کرو ACTION_PADDING 30

ساكن bool
write_action(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
             xkb_layout_اشاریہ_t max_groups, بنية buf *buf,
             ثابت اتحاد xkb_action *action,
             ثابت حرف *اگیتر, ثابت حرف *suffix)
{
    ثابت حرف *type;
    ثابت حرف *دلیلاں = عدم;

    إذا (!اگیتر)
        اگیتر = "";
    إذا (!suffix)
        suffix = "";

    type = Action_Type_متن(action->type);

    بدل (action->type) {
    حالة ACTION_TYPE_MOD_سیٹ:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_جندرا:
        إذا (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            دلیلاں = "modMapMods";
        وإلا
            دلیلاں = Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        إذا (unlockOnPress && !is_Mods_Un_جندرا_On_Press_Supported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    Action_Type_متن(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->گروہ.flags & ACTION_LATCH_ON_PRESS);
        إذا (latchOnPress && !isModsLatchOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot use \"LatchMods(latchOnPress=true)\" "
                    "in keymap format %d\n", format);
            latchOnPress = false;
        }
        write_buf(buf, "%s%s(modifiers=%s%s%s%s%s%s)%s", اگیتر, type, دلیلاں,
                  (action->type != ACTION_TYPE_MOD_جندرا && (action->mods.flags & ACTION_جندرا_CLEAR)) ? ",clearLocks" : "",
                  (action->type == ACTION_TYPE_MOD_LATCH && (action->mods.flags & ACTION_LATCH_منزل_جندرا)) ? ",latchToLock" : "",
                  (action->type == ACTION_TYPE_MOD_جندرا) ? affect_جندرا_متن(action->mods.flags, false) : "",
                  (unlockOnPress) ? ",unlockOnPress" : "",
                  (latchOnPress) ? ",latchOnPress" : "",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_گروہ_سیٹ:
    حالة ACTION_TYPE_گروہ_LATCH:
    حالة ACTION_TYPE_گروہ_جندرا:
        إذا ((uint32_t) xkb_abs(action->گروہ.گروہ) <
            (max_groups + !(action->گروہ.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool جندرا_On_Release = action->type == ACTION_TYPE_گروہ_جندرا &&
                                 (action->گروہ.flags & ACTION_جندرا_ON_RELEASE);
            إذا (جندرا_On_Release && !is_گروہ_جندرا_On_Release_Supported(format)) {
                log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                        "Cannot use \"GroupLock(lockOnRelease=true)\" "
                        "in keymap format %d\n", format);
                جندرا_On_Release = false;
            }
            write_buf(buf, "%s%s(group=%s%"PRId32"%s%s%s)%s", اگیتر, type,
                      (!(action->گروہ.flags & ACTION_ABSOLUTE_SWITCH) && action->گروہ.گروہ >= 0) ? "+" : "",
                      (action->گروہ.flags & ACTION_ABSOLUTE_SWITCH) ? action->گروہ.گروہ + 1 : action->گروہ.گروہ,
                      (action->type != ACTION_TYPE_گروہ_جندرا && (action->گروہ.flags & ACTION_جندرا_CLEAR)) ? ",clearLocks" : "",
                      (action->type == ACTION_TYPE_گروہ_LATCH && (action->گروہ.flags & ACTION_LATCH_منزل_جندرا)) ? ",latchToLock" : "",
                      (جندرا_On_Release) ? ",lockOnRelease" : "",
                      suffix);
        } وإلا {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_سنیہا_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->گروہ.گروہ +
                       !!(action->گروہ.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            اذهب void_action;
        }
        اكسر;

    حالة ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", اگیتر, type, suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", اگیتر, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_جندرا:
        دلیلاں = affect_جندرا_متن(action->btn.flags, true);
        /* fallthrough */
    حالة ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", اگیتر, type);
        static_assert(XKB_اشارہ_گر_BUTTON_DEFAULT < XKB_اشارہ_گر_BUTTON_MIN, "");
        إذا (action->btn.button > XKB_اشارہ_گر_BUTTON_DEFAULT &&
            action->btn.button <= XKB_اشارہ_گر_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        وإلا
            copy_منزل_buf(buf, "default");
        إذا (action->btn.گنتی) {
            أكد(action->type != ACTION_TYPE_PTR_جندرا);
            write_buf(buf, ",count=%u", action->btn.گنتی);
        }
        إذا (دلیلاں)
            write_buf(buf, "%s", دلیلاں);
        write_buf(buf, ")%s", suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", اگیتر, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.قدر >= 0) ? "+" : "",
                  action->dflt.قدر);
        write_buf(buf, ")%s", suffix);
        اكسر;

    حالة ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", اگیتر, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_CTRL_سیٹ:
    حالة ACTION_TYPE_CTRL_جندرا:
        write_buf(buf, "%s%s(controls=%s%s)%s", اگیتر, type,
                  Control_نقاب_متن(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_جندرا
                    ? affect_جندرا_متن(action->ctrls.flags, false)
                    : ""),
                  suffix);
        اكسر;

    حالة ACTION_TYPE_REDIRECT_کلید: {
        write_buf(buf, "%s%s(", اگیتر, type);
        ثابت بنية xkb_کلید * ثابت کلید = Xkb_کلید(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        إذا (کلید)
            write_buf(buf, "keycode=%s", کلید_Name_متن(keymap->ctx, کلید->name));
        إذا (action->redirect.affect_mods) {
            xkb_mod_نقاب_t نقاب;
            نقاب = (action->redirect.affect_mods & action->redirect.mods);
            إذا (نقاب)
                write_buf(buf, ",modifiers=%s",
                          Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods, نقاب));
            نقاب = (action->redirect.affect_mods & ~action->redirect.mods);
            إذا (نقاب)
                write_buf(buf, ",clearMods=%s",
                          Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods, نقاب));
        }
        write_buf(buf, ")%s", suffix);
        اكسر;
    }

    حالة ACTION_TYPE_UNKNOWN:
    حالة ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", اگیتر, suffix);
        اكسر;

    حالة ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        إذا (format == XKB_KEYMAP_FORMAT_متن_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      اگیتر, suffix);
        وإلا
            write_buf(buf, "%sVoidAction()%s", اگیتر, suffix);
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندرونی == 20 &&
                      ACTION_TYPE_اندرونی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        أكد(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  اگیتر, type, action->type, action->priv.انکڑے[0],
                  action->priv.انکڑے[1], action->priv.انکڑے[2],
                  action->priv.انکڑے[3], action->priv.انکڑے[4],
                  action->priv.انکڑے[5], action->priv.انکڑے[6],
                  suffix);
        اكسر;
    }

    أرجع true;
}

ساكن ثابت اتحاد xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

ساكن bool
write_actions(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
              xkb_layout_اشاریہ_t max_groups, بنية buf *buf, بنية buf *buf2,
              ثابت بنية xkb_کلید *کلید, xkb_layout_اشاریہ_t گروہ)
{
    ساكن ثابت اتحاد xkb_action noAction = { .type = ACTION_TYPE_NONE };

    لكل (xkb_level_اشاریہ_t level = 0; level < Xkb_کلید_عدد_پدھراں(کلید, گروہ);
         level++) {

        إذا (level != 0)
            copy_منزل_buf(buf, ", ");

        ثابت اتحاد xkb_action *actions = عدم;
        xkb_action_گنتی_t گنتی = xkb_keymap_کلید_get_actions_by_level(
            keymap, کلید, گروہ, level, &actions
        );

        إذا (گنتی > 1 && format < XKB_KEYMAP_FORMAT_متن_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            گنتی = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->ناپ = 0;
        إذا (گنتی == 0) {
            إذا (!write_action(keymap, format, max_groups,
                              buf2, &noAction, عدم, عدم))
                أرجع false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        وإلا إذا (گنتی == 1) {
            إذا (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), عدم, عدم))
                أرجع false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        وإلا {
            copy_منزل_buf(buf2, "{ ");
            لكل (xkb_action_گنتی_t k = 0; k < گنتی; k++) {
                إذا (k != 0)
                    copy_منزل_buf(buf2, ", ");
                ناپ_t old_ناپ = buf2->ناپ;
                إذا (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), عدم, عدم))
                    أرجع false;
                /* Check if padding is necessary */
                إذا (buf2->ناپ >= old_ناپ + ACTION_PADDING)
                    استمر;
                /* Compute and write padding, then write the action again */
                ثابت صحيح padding = (صحيح)(old_ناپ + ACTION_PADDING - buf2->ناپ);
                buf2->ناپ = old_ناپ;
                write_buf(buf2, "%*s", padding, "");
                إذا (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), عدم, عدم))
                    أرجع false;
            }
            copy_منزل_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    أرجع true;
}

ساكن bool
write_action_defaults(ثابت بنية xkb_keymap *keymap,
                      تعداد xkb_keymap_format format,
                      ثابت اتحاد xkb_action *action,
                      بنية buf *buf)
{
    ثابت حرف *type = Action_Type_متن(action->type);

#تعریف_کرو اگیتر_2 "\t"

    بدل (action->type) {
    حالة ACTION_TYPE_MOD_سیٹ:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_جندرا:
        أكد(action->mods.flags == 0);
        أكد(action->mods.mods.mods == 0);
        write_buf(buf, اگیتر_2"%s.modifiers = 0;\n", type);
        إذا (action->type != ACTION_TYPE_MOD_جندرا)
            write_buf(buf, اگیتر_2"%s.clearLocks = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, اگیتر_2"%s.latchOnPress = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, اگیتر_2"%s.latchToLock = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_جندرا)
            write_buf(buf, اگیتر_2"%s.affect = both;\n", type);
        إذا (is_Mods_Un_جندرا_On_Press_Supported(format))
            write_buf(buf, اگیتر_2"%s.unlockOnPress = false;\n", type);
        اكسر;

    حالة ACTION_TYPE_گروہ_سیٹ:
    حالة ACTION_TYPE_گروہ_LATCH:
    حالة ACTION_TYPE_گروہ_جندرا:
        أكد(action->گروہ.flags == 0);
        أكد(action->گروہ.گروہ == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, اگیتر_2"%s.group = +0;\n", type);
        إذا (action->type != ACTION_TYPE_گروہ_جندرا)
            write_buf(buf, اگیتر_2"%s.clearLocks = false;\n", type);
        إذا (action->type == ACTION_TYPE_گروہ_LATCH)
            write_buf(buf, اگیتر_2"%s.latchToLock = false;\n", type);
        إذا (action->type == ACTION_TYPE_گروہ_جندرا &&
            is_گروہ_جندرا_On_Release_Supported(format))
            write_buf(buf, اگیتر_2"%s.lockOnRelease = false;\n", type);
        اكسر;


    حالة ACTION_TYPE_PTR_MOVE:
        أكد(action->ptr.x == 0);
        أكد(action->ptr.y == 0);
        أكد(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, اگیتر_2"%s.x = +0;\n", type);
        write_buf(buf, اگیتر_2"%s.y = +0;\n", type);
        write_buf(buf, اگیتر_2"%s.accel = true;\n", type);
        اكسر;

    حالة ACTION_TYPE_PTR_جندرا:
    حالة ACTION_TYPE_PTR_BUTTON:
        أكد(action->btn.flags == 0);
        أكد(action->btn.button == 0);
        أكد(action->btn.گنتی == 0);
        write_buf(buf, اگیتر_2"%s.button = 0;\n", type);
        write_buf(buf, اگیتر_2"%s.count = 0;\n", type);
        إذا (action->type == ACTION_TYPE_PTR_جندرا)
            write_buf(buf, اگیتر_2"%s.affect = both;\n", type);
        اكسر;

    حالة ACTION_TYPE_PTR_DEFAULT:
        أكد(action->dflt.flags == 0);
        أكد(action->dflt.قدر == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, اگیتر_2"%s.button = +1;\n", type);
        اكسر;

    حالة ACTION_TYPE_SWITCH_VT:
        أكد(action->screen.flags == ACTION_SAME_SCREEN);
        أكد(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, اگیتر_2"%s.screen = +0;\n", type);
        write_buf(buf, اگیتر_2"%s.same = true;\n", type);
        اكسر;

    حالة ACTION_TYPE_CTRL_سیٹ:
    حالة ACTION_TYPE_CTRL_جندرا:
        أكد(action->ctrls.flags == 0);
        أكد(action->ctrls.ctrls == 0);
        write_buf(buf, اگیتر_2"%s.controls = 0;\n", type);
        إذا (action->type == ACTION_TYPE_CTRL_جندرا)
            write_buf(buf, اگیتر_2"%s.affect = both;\n", type);
        اكسر;

    حالة ACTION_TYPE_REDIRECT_کلید:
        أكد(action->redirect.keycode == keymap->redirect_کلید_auto);
        أكد(action->redirect.affect_mods == 0);
        أكد(action->redirect.mods == 0);
        write_buf(buf, اگیتر_2"%s.keycode = auto;\n", type);
        write_buf(buf, اگیتر_2"%s.modifiers = 0;\n", type);
        write_buf(buf, اگیتر_2"%s.clearMods = 0;\n", type);
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندرونی == 20 &&
                      ACTION_TYPE_اندرونی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");
    }

#تعریف_رد اگیتر_2

    أرجع true;
}

ساكن bool
write_actions_defaults(ثابت بنية xkb_keymap *keymap,
                       تعداد xkb_keymap_format format, بنية buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    لكل (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_عدد_ENTRIES; a++) {
        ثابت اتحاد xkb_action * محدود ثابت action = &defaults.actions[a];
        إذا (!write_action_defaults(keymap, format, action, buf))
            أرجع false;
    }
    copy_منزل_buf(buf, "\n");

    أرجع true;
}

ساكن ثابت بنية xkb_sym_interpret fallback_interpret = {
    .sym = XKB_کلید_Void_علامت,
    .required = true,
    .repeat = FALLBACK_INTERPRET_کلید_REPEAT,
    .میل = میل_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_اشاریہ_t) DEFAULT_INTERPRET_VMOD,
    .عدد_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ساكن bool
write_compat(ثابت بنية xkb_keymap * محدود keymap,
             تعداد xkb_keymap_format format, xkb_layout_اشاریہ_t max_groups,
             تعداد xkb_keymap_serialize_flags flags, bool * محدود some_interp,
             بنية buf *buf)
{
    ثابت bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_اقدار);
    ثابت bool drop_بن_ورتیا = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_بن_ورتیا);
    ثابت bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_اقدار) && drop_بن_ورتیا);
    /* TODO: print all default values if the explicit flag is set */

    إذا (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    وإلا
        copy_منزل_buf(buf, "xkb_compatibility {\n");

    إذا (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        أرجع false;

    إذا (defaults && !write_actions_defaults(keymap, format, buf))
        أرجع false;

    copy_منزل_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_منزل_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    ثابت bool use_fallback_interpret =
        (!keymap->عدد_sym_interprets || drop_interprets);
    ثابت darray_ناپ_t عدد_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->عدد_sym_interprets);
    ثابت بنية xkb_sym_interpret* ثابت sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    لكل (darray_ناپ_t i = 0; i < عدد_sym_interprets; i++) {
        ثابت بنية xkb_sym_interpret *si = &sym_interprets[i];
        إذا (!si->required && drop_بن_ورتیا)
            استمر;

        *some_interp = true;

        copy_منزل_buf(buf, "\tinterpret ");
        إذا (!si->sym) {
            copy_منزل_buf(buf, "Any");
        } وإلا إذا (pretty) {
            write_buf(buf, "%s", Keysym_متن(keymap->ctx, si->sym));
        } وإلا {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatch_متن(si->میل),
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        إذا (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      Mod_اشاریہ_متن(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        إذا (si->level_one_only) {
            copy_منزل_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } وإلا إذا (defaults) {
            copy_منزل_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        إذا (si->repeat) {
            copy_منزل_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } وإلا إذا (defaults) {
            copy_منزل_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        ثابت اتحاد xkb_action *action = عدم;
        xkb_action_گنتی_t action_گنتی = si->عدد_actions;
        إذا (action_گنتی <= 1) {
            action = &si->a.action;
        } وإلا إذا (format < XKB_KEYMAP_FORMAT_متن_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_گنتی = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        إذا (action_گنتی > 1) {
            copy_منزل_buf(buf, "\n\t\taction= {");
            ثابت حرف suffix[] = ", ";
            لكل (xkb_action_گنتی_t k = 0; k < si->عدد_actions; k++) {
                إذا (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    أرجع false;
            }
            buf->ناپ -= حجم(suffix) - 1; /* trailing comma */
            copy_منزل_buf(buf, "};");
            has_explicit_properties = true;
        } وإلا إذا (action_گنتی == 1) {
            إذا (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                أرجع false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    إذا (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        عدد_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].میل == fallback_interpret.میل &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].عدد_actions == fallback_interpret.عدد_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    ثابت بنية xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        إذا ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            أرجع false;
        }
    }

    copy_منزل_buf(buf, "};\n\n");

    أرجع true;
}

ساكن bool
write_keysyms(ثابت بنية xkb_keymap *keymap,
              بنية buf *buf, بنية buf *buf2,
              ثابت بنية xkb_کلید *کلید, xkb_layout_اشاریہ_t گروہ,
              bool pretty, bool show_actions)
{
    ثابت بنانشانی صحيح padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : علامت_PADDING)
        : 0;
    لكل (xkb_level_اشاریہ_t level = 0; level < Xkb_کلید_عدد_پدھراں(کلید, گروہ);
         level++) {
        ثابت xkb_keysym_t *syms;
        صحيح عدد_syms;

        إذا (level != 0)
            copy_منزل_buf(buf, ", ");

        عدد_syms = xkb_keymap_کلید_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (بنية xkb_keymap *)keymap,
            کلید->keycode, گروہ, level, &syms
        );

        ثابت xkb_keysym_t no_علامت = XKB_کلید_No_علامت;
        إذا (عدد_syms == 0) {
            عدد_syms = 1;
            syms = &no_علامت;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        إذا (عدد_syms == 1) {
            إذا (pretty || syms[0] == XKB_کلید_No_علامت)
                write_buf(buf, "%*s", padding, Keysym_متن(keymap->ctx, syms[0]));
            وإلا
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } وإلا {
            إذا (pretty) {
                buf2->ناپ = 0;
                copy_منزل_buf(buf2, "{ ");
                لكل (صحيح s = 0; s < عدد_syms; s++) {
                    إذا (s != 0)
                        copy_منزل_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              Keysym_متن(keymap->ctx, syms[s]));
                }
                copy_منزل_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } وإلا {
                copy_منزل_buf(buf, "{");
                لكل (صحيح s = 0; s < عدد_syms; s++) {
                    إذا (s != 0)
                        copy_منزل_buf(buf, ",");
                    إذا (syms[s] == XKB_کلید_No_علامت)
                        copy_منزل_buf(buf, "NoSymbol");
                    وإلا
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_منزل_buf(buf, "}");
            }
        }
    }

    أرجع true;
}

ساكن bool
write_کلید(ثابت بنية xkb_keymap *keymap,
          ثابت بنية xkb_keymap_serialize_config *config,
          ثابت کلید_name_substitutions *substitutions,
          xkb_layout_اشاریہ_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          بنية buf *buf, بنية buf *buf2, ثابت بنية xkb_کلید *کلید)
{
    bool simple = true;

    xkb_layout_نقاب_t layouts = config->layouts;
    إذا (popcount32(layouts) != keymap->عدد_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_اشاریہ_t l = 0;
        لكل (xkb_layout_نقاب_t ls = layouts; ls; ls >>= 1, l++) {
            إذا (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ثابت xkb_layout_اشاریہ_t effective =
                    xkb_keymap_کلید_effective_layout(کلید, l);
                إذا (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } وإلا {
        layouts &= ((UINT64_C(1) << کلید->عدد_groups) - 1);
    }
    ثابت xkb_layout_اشاریہ_t groups_گنتی = popcount32(layouts);

    ثابت xkb_atom_t name = (substitutions == عدم)
        ? کلید->name
        : substitute_name(substitutions, کلید->name);

    إذا (pretty)
        write_buf(buf, "\tkey %-20s {", کلید_Name_متن(keymap->ctx, name));
    وإلا
        write_buf(buf, "\tkey %s {", کلید_Name_متن(keymap->ctx, name));

    إذا ((کلید->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        إذا (!explicit) {
            لكل (xkb_layout_اشاریہ_t گروہ = 1; گروہ < کلید->عدد_groups; گروہ++) {
                إذا (!((UINT32_C(1) << گروہ) & layouts))
                    استمر;

                إذا (کلید->groups[گروہ].type != کلید->groups[0].type) {
                    multi_type = true;
                    اكسر;
                }
            }
        }

        إذا (multi_type) {
            xkb_layout_اشاریہ_t new_گروہ = (xkb_layout_اشاریہ_t)-1;
            لكل (xkb_layout_اشاریہ_t گروہ = 0; گروہ < کلید->عدد_groups; گروہ++) {
                إذا (!((UINT32_C(1) << گروہ) & layouts))
                    استمر;

                new_گروہ++;

                إذا (!کلید->groups[گروہ].explicit_type && !explicit)
                    استمر;

                ثابت بنية xkb_کلید_type * ثابت type = کلید->groups[گروہ].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_گروہ + 1);
                write_buf_string_literal(
                  buf, xkb_atom_متن(keymap->ctx, type->name));
                copy_منزل_buf(buf, ",");
            }
        }
        وإلا {
            ثابت بنية xkb_کلید_type * ثابت type = کلید->groups[0].type;
            write_buf(buf, "\n\t\ttype= ");
            write_buf_string_literal(
                buf, xkb_atom_متن(keymap->ctx, type->name));
            copy_منزل_buf(buf, ",");
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

    إذا (require_explicit(کلید, EXPLICIT_REPEAT, کلید->repeats,
                         DEFAULT_کلید_REPEAT, DEFAULT_INTERPRET_کلید_REPEAT)) {
        إذا (کلید->repeats)
            copy_منزل_buf(buf, "\n\t\trepeat= Yes,");
        وإلا
            copy_منزل_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    إذا (require_explicit(کلید, EXPLICIT_VMODMAP, کلید->vmodmap,
                         (xkb_mod_نقاب_t) DEFAULT_کلید_VMODMAP,
                         (xkb_mod_نقاب_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              کلید->vmodmap));
        simple = false;
    }

    #تعریف_رد require_explicit

    بدل (کلید->باہرلا_دا_range_گروہ_policy) {
    حالة XKB_LAYOUT_باہرلا_دا_RANGE_CLAMP:
        copy_منزل_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        اكسر;

    حالة XKB_LAYOUT_باہرلا_دا_RANGE_REDIRECT: {
        إذا (!groups_گنتی) {
            /* No layout to redirect to: skip */
            اكسر;
        }

        xkb_layout_نقاب_t layout_نقاب =
            (UINT32_C(1) << کلید->باہرلا_دا_range_گروہ_نمبر);
        إذا (!(layouts & layout_نقاب)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_سنیہا_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     کلید_Name_متن(keymap->ctx, name),
                     کلید->باہرلا_دا_range_گروہ_نمبر + 1);
            layout_نقاب = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ثابت xkb_layout_نقاب_t کم = (layouts & (layout_نقاب - 1));
        ثابت xkb_layout_اشاریہ_t new = popcount32(کم);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        اكسر;
    }

    حالة XKB_LAYOUT_باہرلا_دا_RANGE_WRAP:
    افتراضي:
        اكسر;
    }

    إذا (کلید->overlays) {
        xkb_overlay_نقاب_t بچیا = کلید->overlays;

        ثابت xkb_overlay_اشاریہ_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_گنتی == 8, "invalid right shift");
        ثابت xkb_overlay_نقاب_t valid =
            (xkb_overlay_نقاب_t)((1u << overlay_max) - 1u);
        بچیا &= valid;
        إذا (بچیا != کلید->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_اشاریہ,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, کلید_Name_متن(keymap->ctx, name),
                     بچیا, کلید->overlays & ~valid);
        }

        إذا (بچیا && !کلید->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                بچیا &
                (xkb_overlay_نقاب_t)((بنانشانی)~بچیا + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_اشاریہ_t overlay =
                (xkb_overlay_اشاریہ_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     کلید_Name_متن(keymap->ctx, name), overlay, بچیا & ~lsb);
            بچیا = lsb;
        }

        إذا (بچیا)
            simple = false;

        ثابت بنية xkb_کلید * ثابت *overlays_کلیدیں = (کلید->overlays_inline)
                                                    ? &کلید->overlay_کلید
                                                    : کلید->overlays_کلیدیں;
        طالما (بچیا) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                بچیا &
                (xkb_overlay_نقاب_t)((بنانشانی)~بچیا + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_اشاریہ_t overlay =
                (xkb_overlay_اشاریہ_t)popcount32(lsb - 1u);
            بچیا = (بچیا & ~lsb);

            /* pop current value */
            إذا (کلید->overlays_inline && بچیا)
                PANIC_UNREACHABLE();
            ثابت بنية xkb_کلید *overlay_کلید = *(overlays_کلیدیں++);

            ثابت xkb_atom_t overlay_کلید_name = (substitutions == عدم)
                ? overlay_کلید->name
                : substitute_name(substitutions, overlay_کلید->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, کلید_Name_متن(keymap->ctx, overlay_کلید_name));
        }
    }

    إذا (groups_گنتی > 1 || explicit_actions || explicit)
        simple = false;

    إذا (simple) {
        إذا (groups_گنتی == 0) {
            /* Remove trailing comma */
            delete_آخری_char(buf, ',');
            إذا (buf->buf[buf->ناپ - 1] == ',')
                buf->ناپ--;
        } وإلا {
            copy_منزل_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_اشاریہ_t گروہ = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            گروہ = xkb_keymap_کلید_effective_layout(کلید, گروہ);
            إذا (!write_keysyms(keymap, buf, buf2, کلید, گروہ, pretty, false))
                أرجع false;
            copy_منزل_buf(buf, " ]");
        }
        copy_منزل_buf(buf, " };\n");
    }
    وإلا {
        xkb_layout_اشاریہ_t new_گروہ = (xkb_layout_اشاریہ_t)-1;
        لكل (xkb_layout_اشاریہ_t گروہ = 0; گروہ < کلید->عدد_groups; گروہ++) {
            إذا (!((UINT32_C(1) << گروہ) & layouts))
                استمر;

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

            إذا (new_گروہ != 0)
                copy_منزل_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_گروہ + 1);

            إذا (!write_keysyms(keymap, buf, buf2, کلید, گروہ,
                               pretty, print_actions))
                أرجع false;
            copy_منزل_buf(buf, " ]");
            إذا (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_گروہ + 1);
                إذا (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, کلید, گروہ))
                    أرجع false;
                copy_منزل_buf(buf, " ]");
            }
        }
        إذا (!groups_گنتی)
            delete_آخری_char(buf, ',');
        copy_منزل_buf(buf, "\n\t};\n");
    }

    أرجع true;
}


ساكن صحيح
cmp_modmap_قدم_1(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت Mod_Map_اندراج *m1 = (ثابت Mod_Map_اندراج *)a;
    ثابت Mod_Map_اندراج *m2 = (ثابت Mod_Map_اندراج *)b;

    إذا (!m1->mods) {
        إذا (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            أرجع 0;
        }
        أرجع +1;
    } وإلا إذا (!m2->mods) {
        أرجع -1;
    }

    /* Valid only on step 1 */
    إذا (m1->کلید_کوڈ < m2->کلید_کوڈ)
        أرجع -1;
    وإلا إذا (m1->کلید_کوڈ > m2->کلید_کوڈ)
        أرجع +1;

    إذا (m1->mods < m2->mods)
        أرجع -1;
    وإلا إذا (m1->mods > m2->mods)
        أرجع +1;

    إذا (m1->have_علامت < m2->have_علامت)
        أرجع +1;
    وإلا إذا (m1->have_علامت > m2->have_علامت)
        أرجع -1;

    إذا (m1->have_علامت) {
        إذا (m1->u.کلید_Sym < m2->u.کلید_Sym)
            أرجع -1;
        إذا (m1->u.کلید_Sym > m2->u.کلید_Sym)
            أرجع +1;
    } وإلا {
        إذا (m1->u.کلید_Name < m2->u.کلید_Name)
            أرجع -1;
        إذا (m1->u.کلید_Name > m2->u.کلید_Name)
            أرجع +1;
    }

    أرجع 0;
}

ساكن صحيح
cmp_modmap_قدم_2(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت Mod_Map_اندراج *m1 = (ثابت Mod_Map_اندراج *)a;
    ثابت Mod_Map_اندراج *m2 = (ثابت Mod_Map_اندراج *)b;

    إذا (m1->mods < m2->mods)
        أرجع -1;
    إذا (m1->mods > m2->mods)
        أرجع +1;

    إذا (m1->have_علامت < m2->have_علامت)
        أرجع -1;
    إذا (m1->have_علامت > m2->have_علامت)
        أرجع +1;

    إذا (m1->have_علامت) {
        إذا (m1->u.کلید_Sym < m2->u.کلید_Sym)
            أرجع -1;
        إذا (m1->u.کلید_Sym > m2->u.کلید_Sym)
            أرجع +1;
    } وإلا {
        إذا (m1->کلید_کوڈ < m2->کلید_کوڈ)
            أرجع -1;
        إذا (m1->کلید_کوڈ > m2->کلید_کوڈ)
            أرجع +1;
    }

    أرجع 0;
}

ساكن bool
write_modmaps(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
              ثابت کلید_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, بنية buf *buf)
{
    إذا (!keymap->عدد_modmaps)
        أرجع true;

    darray(Mod_Map_اندراج) modmaps = darray_new();
    darray_from_اشیاء(modmaps, keymap->modmaps, keymap->عدد_modmaps);
    إذا (!darray_اشیاء(modmaps))
        أرجع false;

    /* Sort entries by key code */
    qsort(darray_اشیاء(modmaps), darray_ناپ(modmaps),
          حجم(*darray_اشیاء(modmaps)), &cmp_modmap_قدم_1);

    /* Drop disabled entries at the end */
    لكل (darray_ناپ_t m = darray_ناپ(modmaps); m-- > 0;) {
        إذا (darray_شے(modmaps, m).mods)
            اكسر;
        darray_ناپ(modmaps)--;
    }

    /*
     * Build detailed keymap modmap
     *
     * NOTE: use an explicit for-loop instead of the darray_foreach macro
     * because we may add entries while iterating it. Iterate from the end,
     * so that we do not iterate the new entries.
     */
    xkb_keycode_t current_keycode = XKB_KEYCODE_INVALID;
    bool convert_منزل_v1_compatibility_اندراج = false;
    لكل (darray_ناپ_t m = darray_ناپ(modmaps); m-- > 0;) {
        Mod_Map_اندراج *mm = &darray_شے(modmaps, m);
        أكد(mm->mods);

        إذا (mm->کلید_کوڈ == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ثابت xkb_mod_نقاب_t core_mods = (mm->mods & MOD_REAL_نقاب_ALL);
            إذا (convert_منزل_v1_compatibility_اندراج && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } وإلا {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            استمر;
        }

        current_keycode = mm->کلید_کوڈ;
        ثابت بنية xkb_کلید *کلید = Xkb_کلید(keymap, mm->کلید_کوڈ);

        ثابت xkb_mod_نقاب_t core_mods = (کلید->modmap & MOD_REAL_نقاب_ALL);
        إذا (format == XKB_KEYMAP_FORMAT_متن_V1 &&
            ((کلید->modmap & (کلید->modmap - 1)) || کلید->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_منزل_v1_compatibility_اندراج = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_نقاب,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    کلید_Name_متن(keymap->ctx, کلید->name), کلید->modmap);
        } وإلا {
            /* V2 format or V1 format with a single core modifier */
            أكد(کلید->modmap);
            mm->mods = کلید->modmap;
            /* Force key name */
            mm->have_علامت = false;
            mm->u.کلید_Name = کلید->name;
            convert_منزل_v1_compatibility_اندراج = false;
        }
    }

    إذا (darray_خالی_تھاواں(modmaps))
        اذهب باہر_نکلو;

    /* Sort entries by modifier mask */
    qsort(darray_اشیاء(modmaps), darray_ناپ(modmaps),
          حجم(*darray_اشیاء(modmaps)), &cmp_modmap_قدم_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_نقاب_t current_mods = 0;
    ثابت Mod_Map_اندراج *اندراج;
    darray_foreach(اندراج, modmaps) {
        إذا (!اندراج->mods)
            استمر;

        إذا (current_mods != اندراج->mods) {
            إذا (current_mods) {
                copy_منزل_buf(buf, " };\n");
            }
            current_mods = اندراج->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      Mod_نقاب_متن(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } وإلا {
            copy_منزل_buf(buf, ", ");
        }

        إذا (اندراج->have_علامت) {
            إذا (pretty || اندراج->u.کلید_Sym == XKB_کلید_No_علامت)
                write_buf(buf, "%s", Keysym_متن(keymap->ctx, اندراج->u.کلید_Sym));
            وإلا
                write_buf(buf, "0x%"PRIx32, اندراج->u.کلید_Sym);
        } وإلا {
            ثابت xkb_atom_t name = (substitutions == عدم)
                ? اندراج->u.کلید_Name
                : substitute_name(substitutions, اندراج->u.کلید_Name);
            write_buf(buf, "%s", کلید_Name_متن(keymap->ctx, name));
        }
    }
    إذا (current_mods) {
        copy_منزل_buf(buf, " };\n");
    }

باہر_نکلو:
    darray_free(modmaps);
    أرجع true;
}

ساكن bool
write_symbols(ثابت بنية xkb_keymap *keymap,
              ثابت بنية xkb_keymap_serialize_config *config,
              ثابت کلید_name_substitutions *substitutions,
              xkb_layout_اشاریہ_t max_groups, bool some_interp,
              بنية buf *buf)
{
    ثابت bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_اقدار);
    ثابت bool explicit_کلید_اقدار =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_کلید_اقدار);
    ثابت bool drop_بن_ورتیا = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_بن_ورتیا);
    ثابت bool drop_interprets = (explicit_کلید_اقدار && drop_بن_ورتیا);
    /* TODO: print all default values if explicit flag is set */

    إذا (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    وإلا
        copy_منزل_buf(buf, "xkb_symbols {\n");

    bool has_گروہ_ناں = false;
    xkb_layout_اشاریہ_t new_گروہ = (xkb_layout_اشاریہ_t)-1;
    لكل (xkb_layout_اشاریہ_t گروہ = 0; گروہ < keymap->عدد_گروہ_ناں; گروہ++) {
        إذا (!((UINT32_C(1) << گروہ) & config->layouts))
            استمر;

        new_گروہ++;

        إذا (keymap->گروہ_ناں[گروہ]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_گروہ + 1);
            write_buf_string_literal(
                buf, xkb_atom_متن(keymap->ctx, keymap->گروہ_ناں[گروہ]));
            copy_منزل_buf(buf, ";\n");
            has_گروہ_ناں = true;
        }
    }
    إذا (has_گروہ_ناں)
        copy_منزل_buf(buf, "\n");

    إذا (defaults && !write_actions_defaults(keymap, config->format, buf))
        أرجع false;

    بنية buf buf2 = { عدم, 0, 0 };
    ثابت بنية xkb_کلید *کلید;
    xkb_کلیدیں_foreach(کلید, keymap) {
        /* Skip keys with no explicit values */
        إذا (کلید->explicit &&
            !write_کلید(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_کلید_اقدار, pretty, buf,
                       &buf2, کلید))
        {
            حرر(buf2.buf);
            أرجع false;
        }
    }
    حرر(buf2.buf);

    إذا (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        أرجع false;

    copy_منزل_buf(buf, "};\n\n");
    أرجع true;
}

ساكن تعداد xkb_حالت
write_keymap(ثابت بنية xkb_keymap *keymap,
             ثابت بنية xkb_keymap_serialize_config *config,
             بنية buf *buf, بنية xkb_keymap_serialize_نتیجہ *نتیجہ)
{
    کلید_name_substitutions substitutions = darray_new();
    إذا (config->format == XKB_KEYMAP_FORMAT_متن_V1 &&
        !rename_long_کلیدیں(keymap, &substitutions))
    {
        أرجع false;
    }

    ثابت کلید_name_substitutions * ثابت substitutions_ptr =
        (darray_خالی_تھاواں(substitutions)) ? عدم : &substitutions;

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
    أرجع ok ? XKB_SUCCESS : -1;
}

تعداد xkb_حالت
متن_v1_keymap_serialize(
        ثابت بنية xkb_keymap *keymap,
        ثابت بنية xkb_keymap_serialize_config *config,
        بنية xkb_keymap_serialize_نتیجہ *نتیجہ
)
{
    بنية buf buf = { عدم, 0, 0 };

    ثابت تعداد xkb_حالت حالت =
        write_keymap(keymap, config, &buf, نتیجہ);
    إذا (حالت != XKB_SUCCESS) {
        حرر(buf.buf);
        نتیجہ->serialized = عدم;
        أرجع حالت;
    }

    نتیجہ->serialized = buf.buf;
    نتیجہ->لمائی = buf.ناپ + 1;
    نتیجہ->layouts = config->layouts;

    أرجع XKB_SUCCESS;
}
