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

#وصف_ڪريو BUF_CHUNK_سائيز 4096

بنية buf {
    حرف *buf;
    سائيز_t سائيز;
    سائيز_t alloc;
};

#وصف_ڪريو xkb_abs(n) _Generic((n),                \
                            صحيح: مطلق_قدر,           \
                            ڊگهو صحيح: labs,     \
                            افتراضي: llabs )((n))

ساكن bool
do_realloc(بنية buf *buf, سائيز_t at_گهٽ_کان_گهٽ)
{
    buf->alloc += BUF_CHUNK_سائيز;
    إذا (at_گهٽ_کان_گهٽ >= BUF_CHUNK_سائيز)
        buf->alloc += at_گهٽ_کان_گهٽ;

    حرف *ثابت new = realloc(buf->buf, buf->alloc);
    إذا (!new)
        أرجع false;

    buf->buf = new;
    أرجع true;
}

ATTR_PRINTF(2, 3) ساكن bool
check_write_buf(بنية buf *buf, ثابت حرف *fmt, ...)
{
    va_list دليلون;
    صحيح printed;
    سائيز_t موجود;

    موجود = buf->alloc - buf->سائيز;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    أكد(buf->buf || موجود == 0);
    va_شروعات(دليلون, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->سائيز : buf->buf, موجود,
                        fmt, دليلون);
    va_پڇاڙي(دليلون);

    إذا (printed < 0) {
        /* Some error */
        اذهب err;
    } وإلا إذا ((سائيز_t)printed >= موجود) {
        /* Not enough space: need to realloc */
        إذا (!do_realloc(buf, (سائيز_t)printed + 1))
            اذهب err;

        /* The buffer has enough space now. */
        موجود = buf->alloc - buf->سائيز;
        أكد(buf->buf && (سائيز_t) printed < موجود);
        va_شروعات(دليلون, fmt);
        printed = vsnprintf(buf->buf + buf->سائيز, موجود, fmt, دليلون);
        va_پڇاڙي(دليلون);

        إذا (printed < 0 || (سائيز_t) printed >= موجود)
            اذهب err;
    }

    buf->سائيز += (سائيز_t)printed;
    أرجع true;

err:
    حرر(buf->buf);
    buf->buf = عدم;
    أرجع false;
}

#وصف_ڪريو write_buf(buf, ...) افعل { \
    إذا (!check_write_buf(buf, __VA_ARGS__)) \
        أرجع false; \
} طالما (0)

ساكن bool
check_copy_منزل_buf(بنية buf *buf, ثابت حرف* ذريعو, سائيز_t len)
{
    إذا (len == 0)
        أرجع true;

    ثابت سائيز_t موجود = buf->alloc - buf->سائيز;
    إذا (len >= موجود &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        حرر(buf->buf);
        buf->buf = عدم;
        أرجع false;
    }

    يادگيري_نقل(buf->buf + buf->سائيز, ذريعو, len);
    buf->سائيز += len;
    /* Append NULL byte */
    buf->buf[buf->سائيز] = '\0';
    أرجع true;
}

#وصف_ڪريو copy_منزل_buf_len(buf, ذريعو, len) افعل { \
    إذا (!check_copy_منزل_buf(buf, ذريعو, len)) \
        أرجع false; \
} طالما (0)

#وصف_ڪريو copy_منزل_buf(buf, ذريعو) \
    copy_منزل_buf_len(buf, ذريعو, حجم(ذريعو) - 1)

ساكن bool
check_write_string_literal(بنية buf *buf, ثابت حرف* string)
{
    copy_منزل_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    سائيز_t pending_شروعات = 0;
    سائيز_t current = 0;
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
            إذا (unlikely((بنانشان حرف)string[current] < 0x20U ||
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
        أكد(current >= pending_شروعات);
        copy_منزل_buf_len(buf, string + pending_شروعات, current - pending_شروعات);
        /* Write escape sequence */
        copy_منزل_buf_len(buf, escape_بفر, escape_len);
        pending_شروعات = current + 1;
    }
    /* Write remaining chunk */
    أكد(current >= pending_شروعات);
    copy_منزل_buf_len(buf, string + pending_شروعات, current - pending_شروعات);

    copy_منزل_buf(buf, "\"");
    أرجع true;
}

#وصف_ڪريو write_buf_string_literal(buf, string) افعل { \
    إذا (!check_write_string_literal(buf, string)) \
        أرجع false; \
} طالما (0)

ساكن فراغ
delete_آخري_char(بنية buf *buf, حرف c)
{
    إذا (buf->سائيز && buf->buf[buf->سائيز - 1] == c)
        buf->buf[--buf->سائيز] = '\0';
}

/** Entry for a name substituion LUT */
بنية ڪنجي_name_substitution {
    xkb_atom_t ذريعو;
    xkb_atom_t هدف;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
ساكن صحيح
cmp_ڪنجي_name_substitution(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت xkb_atom_t s1 = ((بنية ڪنجي_name_substitution *) a)->ذريعو;
    ثابت xkb_atom_t s2 = ((بنية ڪنجي_name_substitution *) b)->ذريعو;
    أرجع (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

تعريفنوع darray(بنية ڪنجي_name_substitution) ڪنجي_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
ساكن bool
rename_long_ڪنجيون(ثابت بنية xkb_keymap *keymap,
                 ڪنجي_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    تعداد {
        پهريون = '!',
        آخري = '0',
        invalid = '\0'
    };
    حرف prefixes[] = {
        پهريون, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', آخري
    };
    static_assert(حجم(prefixes) == آخري - پهريون + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ثابت بنية xkb_ڪنجي *ڪنجي;
    xkb_ڪنجيون_foreach(ڪنجي, keymap) {
        إذا (ڪنجي->name == XKB_ATOM_NONE)
            استمر;

        ثابت حرف * ثابت name = xkb_atom_متن(keymap->ctx, ڪنجي->name);
        ثابت سائيز_t len = strlen_محفوظ(name);
        إذا (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (بنية ڪنجي_name_substitution) {
                .ذريعو = ڪنجي->name,
                .keycode = ڪنجي->keycode,
                .هدف = ڪنجي->name
            });
        } وإلا إذا (len == 4 && name[0] >= پهريون && name[0] <= آخري) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - پهريون] = invalid;
        }
    }

    /* Check aliases */
    لكل (darray_سائيز_t a = 0; a < keymap->انگ_ڪنجي_aliases; a++) {
        ثابت بنية xkb_ڪنجي_alias * ثابت alias = &keymap->ڪنجي_aliases[a];
        ثابت حرف * ثابت name = xkb_atom_متن(keymap->ctx, alias->alias);
        ثابت سائيز_t len = strlen_محفوظ(name);
        إذا (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (بنية ڪنجي_name_substitution) {
                .ذريعو = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .هدف = alias->alias
            });
        } وإلا إذا (len == 4 && name[0] >= پهريون && name[0] <= آخري) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - پهريون] = invalid;
        }
    }

    إذا darray_خالي_جايون(*substitutions) {
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
    حرف *اڳياڙي = prefixes;
    ثابت حرف *اڳياڙي_max = prefixes + حجم(prefixes) - 1;
    طالما (*اڳياڙي == invalid && اڳياڙي < اڳياڙي_max)
        اڳياڙي++;

    /* Keycode prefix: prefer ‘0’ if available */
    حرف keycode_اڳياڙي;
    إذا (prefixes[آخري - پهريون] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_اڳياڙي = آخري;
        prefixes[آخري - پهريون] = invalid;
    } وإلا {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_اڳياڙي = *اڳياڙي;
        إذا (unlikely(keycode_اڳياڙي == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            أرجع true;
        }
        /* Update current prefix */
        *اڳياڙي = invalid;
        طالما (*اڳياڙي == invalid && اڳياڙي < اڳياڙي_max)
            اڳياڙي++;
    }

    /* Proceed renaming */
    uint16_t ايندڙ_id = 1;
    بنية ڪنجي_name_substitution *sub;
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
                     keycode_اڳياڙي, sub->keycode);
            ثابت xkb_atom_t هدف = xkb_atom_intern(keymap->ctx,
                                                      buf, حجم(buf) - 2);
            إذا (هدف == XKB_ATOM_NONE)
                اذهب error;
            sub->هدف = هدف;
        } وإلا إذا (*اڳياڙي != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, حجم(buf), "%c%03"PRIx16, *اڳياڙي, ايندڙ_id);
            ثابت xkb_atom_t هدف = xkb_atom_intern(keymap->ctx,
                                                      buf, حجم(buf) - 2);
            إذا (هدف == XKB_ATOM_NONE)
                اذهب error;
            sub->هدف = هدف;
            إذا (++ايندڙ_id > 0xfff) {
                *اڳياڙي = invalid;
                /* Get next available prefix */
                طالما (*اڳياڙي == invalid && اڳياڙي < اڳياڙي_max)
                    اڳياڙي++;
                /* Reset counter */
                ايندڙ_id = 1;
            }
        } وإلا {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->هدف = sub->ذريعو;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_شيون(*substitutions), darray_سائيز(*substitutions),
          حجم(*darray_شيون(*substitutions)), &cmp_ڪنجي_name_substitution);

    أرجع true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ورهاست_FAILURE_2,
            "Cannot allocate key name substitution\n");
    أرجع false;
}

/** Substitute a key name with a valid X11 key name */
ساكن xkb_atom_t
substitute_name(ثابت ڪنجي_name_substitutions *substitutions, xkb_atom_t name)
{
    /*ٻٽي ڳولا*/
    darray_سائيز_t پهريون = 0;
    darray_سائيز_t آخري = darray_سائيز(*substitutions);

    طالما (آخري > پهريون) {
        darray_سائيز_t mid = پهريون + (آخري - 1 - پهريون) / 2;
        ثابت xkb_atom_t ذريعو = darray_شي(*substitutions, mid).ذريعو;
        إذا (ذريعو < name)
            پهريون = mid + 1;
        وإلا إذا (ذريعو > name)
            آخري = mid;
        وإلا /* found it */
            أرجع darray_شي(*substitutions, mid).هدف;
    }

    /* No name substitution: return original name */
    أرجع name;
}

ساكن bool
write_vmods(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
            bool explicit, بنية buf *buf)
{
    ثابت بنية xkb_mod *mod;
    xkb_mod_اشاريو_t vmod = 0;
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
               ثابت ڪنجي_name_substitutions *substitutions,
               تعداد xkb_keymap_serialize_flags flags, بنية buf *buf)
{
    ثابت بنية xkb_ڪنجي *ڪنجي;
    xkb_led_اشاريو_t idx;
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
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_ڪنجي_ڪوڊ, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_ڪنجي_ڪوڊ, 255));

    xkb_ڪنجيون_foreach(ڪنجي, keymap) {
        إذا (ڪنجي->name == XKB_ATOM_NONE)
            استمر;

        ثابت xkb_atom_t name = (substitutions == عدم)
            ? ڪنجي->name
            : substitute_name(substitutions, ڪنجي->name);
        إذا (pretty)
            write_buf(buf, "\t%-20s", ڪنجي_Name_متن(keymap->ctx, name));
        وإلا
            write_buf(buf, "\t%s", ڪنجي_Name_متن(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", ڪنجي->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        إذا (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_متن(keymap->ctx, led->name));
            copy_منزل_buf(buf, ";\n");
        }

    لكل (darray_سائيز_t i = 0; i < keymap->انگ_ڪنجي_aliases; i++) {
        ثابت xkb_atom_t alias = (substitutions == عدم)
            ? keymap->ڪنجي_aliases[i].alias
            : substitute_name(substitutions, keymap->ڪنجي_aliases[i].alias);
        ثابت xkb_atom_t real = (substitutions == عدم)
            ? keymap->ڪنجي_aliases[i].real
            : substitute_name(substitutions, keymap->ڪنجي_aliases[i].real);

        إذا (pretty)
            write_buf(buf, "\talias %-14s",
                      ڪنجي_Name_متن(keymap->ctx, alias));
        وإلا
            write_buf(buf, "\talias %s",
                      ڪنجي_Name_متن(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  ڪنجي_Name_متن(keymap->ctx, real));
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

    ثابت bool drop_اڻ_استعمال_ٿيل = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_اڻ_استعمال_ٿيل);
    لكل (darray_سائيز_t i = 0; i < keymap->انگ_types; i++) {
        ثابت بنية xkb_ڪنجي_type * ثابت type = &keymap->types[i];
        إذا (!type->required && drop_اڻ_استعمال_ٿيل)
            استمر;

        copy_منزل_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_متن(keymap->ctx, type->name));
        copy_منزل_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        لكل (darray_سائيز_t j = 0; j < type->انگ_entries; j++) {
            ثابت حرف *str;
            ثابت بنية xkb_ڪنجي_type_داخلا *داخلا = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            إذا (داخلا->level == 0 && داخلا->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_قدرون))
                استمر;

            str = Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              داخلا->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, داخلا->level + 1);

            إذا (داخلا->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           داخلا->preserve.mods));
        }

        لكل (xkb_level_اشاريو_t n = 0; n < type->انگ_level_نالا; n++)
            إذا (type->level_نالا[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_متن(keymap->ctx, type->level_نالا[n]));
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
                      Led_State_نقاب_متن(keymap->ctx, گروهه_Component_نقاب_نالا,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    إذا (led->which_mods) {
        إذا (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      Led_State_نقاب_متن(keymap->ctx, mod_Component_نقاب_نالا,
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
affect_تالو_متن(تعداد xkb_action_flags flags, bool show_both)
{
    بدل (flags & (ACTION_تالو_NO_تالو | ACTION_تالو_NO_UNLOCK)) {
    حالة 0:
        أرجع show_both ? ",affect=both" : "";
    حالة ACTION_تالو_NO_UNLOCK:
        أرجع ",affect=lock";
    حالة ACTION_تالو_NO_تالو:
        أرجع ",affect=unlock";
    حالة ACTION_تالو_NO_تالو | ACTION_تالو_NO_UNLOCK:
        أرجع ",affect=neither";
    افتراضي:
        أرجع "";
    }
}

#وصف_ڪريو نشان_PADDING 15
#وصف_ڪريو ACTION_PADDING 30

ساكن bool
write_action(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
             xkb_layout_اشاريو_t max_groups, بنية buf *buf,
             ثابت اتحاد xkb_action *action,
             ثابت حرف *اڳياڙي, ثابت حرف *suffix)
{
    ثابت حرف *type;
    ثابت حرف *دليلون = عدم;

    إذا (!اڳياڙي)
        اڳياڙي = "";
    إذا (!suffix)
        suffix = "";

    type = Action_Type_متن(action->type);

    بدل (action->type) {
    حالة ACTION_TYPE_MOD_سيٽ:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_تالو:
        إذا (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            دليلون = "modMapMods";
        وإلا
            دليلون = Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        إذا (unlockOnPress && !is_Mods_Un_تالو_On_Press_Supported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    Action_Type_متن(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->گروهه.flags & ACTION_LATCH_ON_PRESS);
        إذا (latchOnPress && !isModsLatchOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot use \"LatchMods(latchOnPress=true)\" "
                    "in keymap format %d\n", format);
            latchOnPress = false;
        }
        write_buf(buf, "%s%s(modifiers=%s%s%s%s%s%s)%s", اڳياڙي, type, دليلون,
                  (action->type != ACTION_TYPE_MOD_تالو && (action->mods.flags & ACTION_تالو_CLEAR)) ? ",clearLocks" : "",
                  (action->type == ACTION_TYPE_MOD_LATCH && (action->mods.flags & ACTION_LATCH_منزل_تالو)) ? ",latchToLock" : "",
                  (action->type == ACTION_TYPE_MOD_تالو) ? affect_تالو_متن(action->mods.flags, false) : "",
                  (unlockOnPress) ? ",unlockOnPress" : "",
                  (latchOnPress) ? ",latchOnPress" : "",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_گروهه_سيٽ:
    حالة ACTION_TYPE_گروهه_LATCH:
    حالة ACTION_TYPE_گروهه_تالو:
        إذا ((uint32_t) xkb_abs(action->گروهه.گروهه) <
            (max_groups + !(action->گروهه.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool تالو_On_Release = action->type == ACTION_TYPE_گروهه_تالو &&
                                 (action->گروهه.flags & ACTION_تالو_ON_RELEASE);
            إذا (تالو_On_Release && !is_گروهه_تالو_On_Release_Supported(format)) {
                log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                        "Cannot use \"GroupLock(lockOnRelease=true)\" "
                        "in keymap format %d\n", format);
                تالو_On_Release = false;
            }
            write_buf(buf, "%s%s(group=%s%"PRId32"%s%s%s)%s", اڳياڙي, type,
                      (!(action->گروهه.flags & ACTION_ABSOLUTE_SWITCH) && action->گروهه.گروهه >= 0) ? "+" : "",
                      (action->گروهه.flags & ACTION_ABSOLUTE_SWITCH) ? action->گروهه.گروهه + 1 : action->گروهه.گروهه,
                      (action->type != ACTION_TYPE_گروهه_تالو && (action->گروهه.flags & ACTION_تالو_CLEAR)) ? ",clearLocks" : "",
                      (action->type == ACTION_TYPE_گروهه_LATCH && (action->گروهه.flags & ACTION_LATCH_منزل_تالو)) ? ",latchToLock" : "",
                      (تالو_On_Release) ? ",lockOnRelease" : "",
                      suffix);
        } وإلا {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_پيغام_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->گروهه.گروهه +
                       !!(action->گروهه.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            اذهب void_action;
        }
        اكسر;

    حالة ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", اڳياڙي, type, suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", اڳياڙي, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_تالو:
        دليلون = affect_تالو_متن(action->btn.flags, true);
        /* fallthrough */
    حالة ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", اڳياڙي, type);
        static_assert(XKB_اشارو_BUTTON_DEFAULT < XKB_اشارو_BUTTON_MIN, "");
        إذا (action->btn.button > XKB_اشارو_BUTTON_DEFAULT &&
            action->btn.button <= XKB_اشارو_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        وإلا
            copy_منزل_buf(buf, "default");
        إذا (action->btn.ڳڻپ) {
            أكد(action->type != ACTION_TYPE_PTR_تالو);
            write_buf(buf, ",count=%u", action->btn.ڳڻپ);
        }
        إذا (دليلون)
            write_buf(buf, "%s", دليلون);
        write_buf(buf, ")%s", suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", اڳياڙي, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.قدر >= 0) ? "+" : "",
                  action->dflt.قدر);
        write_buf(buf, ")%s", suffix);
        اكسر;

    حالة ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", اڳياڙي, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_CTRL_سيٽ:
    حالة ACTION_TYPE_CTRL_تالو:
        write_buf(buf, "%s%s(controls=%s%s)%s", اڳياڙي, type,
                  Control_نقاب_متن(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_تالو
                    ? affect_تالو_متن(action->ctrls.flags, false)
                    : ""),
                  suffix);
        اكسر;

    حالة ACTION_TYPE_REDIRECT_ڪنجي: {
        write_buf(buf, "%s%s(", اڳياڙي, type);
        ثابت بنية xkb_ڪنجي * ثابت ڪنجي = Xkb_ڪنجي(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        إذا (ڪنجي)
            write_buf(buf, "keycode=%s", ڪنجي_Name_متن(keymap->ctx, ڪنجي->name));
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
        write_buf(buf, "%sNoAction()%s", اڳياڙي, suffix);
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
                      اڳياڙي, suffix);
        وإلا
            write_buf(buf, "%sVoidAction()%s", اڳياڙي, suffix);
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندروني == 20 &&
                      ACTION_TYPE_اندروني + 1 == ACTION_TYPE_انگ_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        أكد(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  اڳياڙي, type, action->type, action->priv.معلومات[0],
                  action->priv.معلومات[1], action->priv.معلومات[2],
                  action->priv.معلومات[3], action->priv.معلومات[4],
                  action->priv.معلومات[5], action->priv.معلومات[6],
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
              xkb_layout_اشاريو_t max_groups, بنية buf *buf, بنية buf *buf2,
              ثابت بنية xkb_ڪنجي *ڪنجي, xkb_layout_اشاريو_t گروهه)
{
    ساكن ثابت اتحاد xkb_action noAction = { .type = ACTION_TYPE_NONE };

    لكل (xkb_level_اشاريو_t level = 0; level < Xkb_ڪنجي_انگ_سطحون(ڪنجي, گروهه);
         level++) {

        إذا (level != 0)
            copy_منزل_buf(buf, ", ");

        ثابت اتحاد xkb_action *actions = عدم;
        xkb_action_ڳڻپ_t ڳڻپ = xkb_keymap_ڪنجي_get_actions_by_level(
            keymap, ڪنجي, گروهه, level, &actions
        );

        إذا (ڳڻپ > 1 && format < XKB_KEYMAP_FORMAT_متن_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            ڳڻپ = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->سائيز = 0;
        إذا (ڳڻپ == 0) {
            إذا (!write_action(keymap, format, max_groups,
                              buf2, &noAction, عدم, عدم))
                أرجع false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        وإلا إذا (ڳڻپ == 1) {
            إذا (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), عدم, عدم))
                أرجع false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        وإلا {
            copy_منزل_buf(buf2, "{ ");
            لكل (xkb_action_ڳڻپ_t k = 0; k < ڳڻپ; k++) {
                إذا (k != 0)
                    copy_منزل_buf(buf2, ", ");
                سائيز_t old_سائيز = buf2->سائيز;
                إذا (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), عدم, عدم))
                    أرجع false;
                /* Check if padding is necessary */
                إذا (buf2->سائيز >= old_سائيز + ACTION_PADDING)
                    استمر;
                /* Compute and write padding, then write the action again */
                ثابت صحيح padding = (صحيح)(old_سائيز + ACTION_PADDING - buf2->سائيز);
                buf2->سائيز = old_سائيز;
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

#وصف_ڪريو اڳياڙي_2 "\t"

    بدل (action->type) {
    حالة ACTION_TYPE_MOD_سيٽ:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_تالو:
        أكد(action->mods.flags == 0);
        أكد(action->mods.mods.mods == 0);
        write_buf(buf, اڳياڙي_2"%s.modifiers = 0;\n", type);
        إذا (action->type != ACTION_TYPE_MOD_تالو)
            write_buf(buf, اڳياڙي_2"%s.clearLocks = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, اڳياڙي_2"%s.latchOnPress = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, اڳياڙي_2"%s.latchToLock = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_تالو)
            write_buf(buf, اڳياڙي_2"%s.affect = both;\n", type);
        إذا (is_Mods_Un_تالو_On_Press_Supported(format))
            write_buf(buf, اڳياڙي_2"%s.unlockOnPress = false;\n", type);
        اكسر;

    حالة ACTION_TYPE_گروهه_سيٽ:
    حالة ACTION_TYPE_گروهه_LATCH:
    حالة ACTION_TYPE_گروهه_تالو:
        أكد(action->گروهه.flags == 0);
        أكد(action->گروهه.گروهه == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, اڳياڙي_2"%s.group = +0;\n", type);
        إذا (action->type != ACTION_TYPE_گروهه_تالو)
            write_buf(buf, اڳياڙي_2"%s.clearLocks = false;\n", type);
        إذا (action->type == ACTION_TYPE_گروهه_LATCH)
            write_buf(buf, اڳياڙي_2"%s.latchToLock = false;\n", type);
        إذا (action->type == ACTION_TYPE_گروهه_تالو &&
            is_گروهه_تالو_On_Release_Supported(format))
            write_buf(buf, اڳياڙي_2"%s.lockOnRelease = false;\n", type);
        اكسر;


    حالة ACTION_TYPE_PTR_MOVE:
        أكد(action->ptr.x == 0);
        أكد(action->ptr.y == 0);
        أكد(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, اڳياڙي_2"%s.x = +0;\n", type);
        write_buf(buf, اڳياڙي_2"%s.y = +0;\n", type);
        write_buf(buf, اڳياڙي_2"%s.accel = true;\n", type);
        اكسر;

    حالة ACTION_TYPE_PTR_تالو:
    حالة ACTION_TYPE_PTR_BUTTON:
        أكد(action->btn.flags == 0);
        أكد(action->btn.button == 0);
        أكد(action->btn.ڳڻپ == 0);
        write_buf(buf, اڳياڙي_2"%s.button = 0;\n", type);
        write_buf(buf, اڳياڙي_2"%s.count = 0;\n", type);
        إذا (action->type == ACTION_TYPE_PTR_تالو)
            write_buf(buf, اڳياڙي_2"%s.affect = both;\n", type);
        اكسر;

    حالة ACTION_TYPE_PTR_DEFAULT:
        أكد(action->dflt.flags == 0);
        أكد(action->dflt.قدر == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, اڳياڙي_2"%s.button = +1;\n", type);
        اكسر;

    حالة ACTION_TYPE_SWITCH_VT:
        أكد(action->screen.flags == ACTION_SAME_SCREEN);
        أكد(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, اڳياڙي_2"%s.screen = +0;\n", type);
        write_buf(buf, اڳياڙي_2"%s.same = true;\n", type);
        اكسر;

    حالة ACTION_TYPE_CTRL_سيٽ:
    حالة ACTION_TYPE_CTRL_تالو:
        أكد(action->ctrls.flags == 0);
        أكد(action->ctrls.ctrls == 0);
        write_buf(buf, اڳياڙي_2"%s.controls = 0;\n", type);
        إذا (action->type == ACTION_TYPE_CTRL_تالو)
            write_buf(buf, اڳياڙي_2"%s.affect = both;\n", type);
        اكسر;

    حالة ACTION_TYPE_REDIRECT_ڪنجي:
        أكد(action->redirect.keycode == keymap->redirect_ڪنجي_auto);
        أكد(action->redirect.affect_mods == 0);
        أكد(action->redirect.mods == 0);
        write_buf(buf, اڳياڙي_2"%s.keycode = auto;\n", type);
        write_buf(buf, اڳياڙي_2"%s.modifiers = 0;\n", type);
        write_buf(buf, اڳياڙي_2"%s.clearMods = 0;\n", type);
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_اندروني == 20 &&
                      ACTION_TYPE_اندروني + 1 == ACTION_TYPE_انگ_ENTRIES,
                      "Missing action type");
    }

#وصف_رد اڳياڙي_2

    أرجع true;
}

ساكن bool
write_actions_defaults(ثابت بنية xkb_keymap *keymap,
                       تعداد xkb_keymap_format format, بنية buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    لكل (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_انگ_ENTRIES; a++) {
        ثابت اتحاد xkb_action * محدود ثابت action = &defaults.actions[a];
        إذا (!write_action_defaults(keymap, format, action, buf))
            أرجع false;
    }
    copy_منزل_buf(buf, "\n");

    أرجع true;
}

ساكن ثابت بنية xkb_sym_interpret fallback_interpret = {
    .sym = XKB_ڪنجي_Void_نشان,
    .required = true,
    .repeat = FALLBACK_INTERPRET_ڪنجي_REPEAT,
    .ميلاپ = ميلاپ_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_اشاريو_t) DEFAULT_INTERPRET_VMOD,
    .انگ_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ساكن bool
write_compat(ثابت بنية xkb_keymap * محدود keymap,
             تعداد xkb_keymap_format format, xkb_layout_اشاريو_t max_groups,
             تعداد xkb_keymap_serialize_flags flags, bool * محدود some_interp,
             بنية buf *buf)
{
    ثابت bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_قدرون);
    ثابت bool drop_اڻ_استعمال_ٿيل = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_اڻ_استعمال_ٿيل);
    ثابت bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ڪنجي_قدرون) && drop_اڻ_استعمال_ٿيل);
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
        (!keymap->انگ_sym_interprets || drop_interprets);
    ثابت darray_سائيز_t انگ_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->انگ_sym_interprets);
    ثابت بنية xkb_sym_interpret* ثابت sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    لكل (darray_سائيز_t i = 0; i < انگ_sym_interprets; i++) {
        ثابت بنية xkb_sym_interpret *si = &sym_interprets[i];
        إذا (!si->required && drop_اڻ_استعمال_ٿيل)
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
                  SIMatch_متن(si->ميلاپ),
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        إذا (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      Mod_اشاريو_متن(keymap->ctx, &keymap->mods,
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
        xkb_action_ڳڻپ_t action_ڳڻپ = si->انگ_actions;
        إذا (action_ڳڻپ <= 1) {
            action = &si->a.action;
        } وإلا إذا (format < XKB_KEYMAP_FORMAT_متن_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_ڳڻپ = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        إذا (action_ڳڻپ > 1) {
            copy_منزل_buf(buf, "\n\t\taction= {");
            ثابت حرف suffix[] = ", ";
            لكل (xkb_action_ڳڻپ_t k = 0; k < si->انگ_actions; k++) {
                إذا (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    أرجع false;
            }
            buf->سائيز -= حجم(suffix) - 1; /* trailing comma */
            copy_منزل_buf(buf, "};");
            has_explicit_properties = true;
        } وإلا إذا (action_ڳڻپ == 1) {
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
        انگ_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].ميلاپ == fallback_interpret.ميلاپ &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].انگ_actions == fallback_interpret.انگ_actions
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
              ثابت بنية xkb_ڪنجي *ڪنجي, xkb_layout_اشاريو_t گروهه,
              bool pretty, bool show_actions)
{
    ثابت بنانشان صحيح padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : نشان_PADDING)
        : 0;
    لكل (xkb_level_اشاريو_t level = 0; level < Xkb_ڪنجي_انگ_سطحون(ڪنجي, گروهه);
         level++) {
        ثابت xkb_keysym_t *syms;
        صحيح انگ_syms;

        إذا (level != 0)
            copy_منزل_buf(buf, ", ");

        انگ_syms = xkb_keymap_ڪنجي_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (بنية xkb_keymap *)keymap,
            ڪنجي->keycode, گروهه, level, &syms
        );

        ثابت xkb_keysym_t no_نشان = XKB_ڪنجي_No_نشان;
        إذا (انگ_syms == 0) {
            انگ_syms = 1;
            syms = &no_نشان;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        إذا (انگ_syms == 1) {
            إذا (pretty || syms[0] == XKB_ڪنجي_No_نشان)
                write_buf(buf, "%*s", padding, Keysym_متن(keymap->ctx, syms[0]));
            وإلا
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } وإلا {
            إذا (pretty) {
                buf2->سائيز = 0;
                copy_منزل_buf(buf2, "{ ");
                لكل (صحيح s = 0; s < انگ_syms; s++) {
                    إذا (s != 0)
                        copy_منزل_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              Keysym_متن(keymap->ctx, syms[s]));
                }
                copy_منزل_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } وإلا {
                copy_منزل_buf(buf, "{");
                لكل (صحيح s = 0; s < انگ_syms; s++) {
                    إذا (s != 0)
                        copy_منزل_buf(buf, ",");
                    إذا (syms[s] == XKB_ڪنجي_No_نشان)
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
write_ڪنجي(ثابت بنية xkb_keymap *keymap,
          ثابت بنية xkb_keymap_serialize_config *config,
          ثابت ڪنجي_name_substitutions *substitutions,
          xkb_layout_اشاريو_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          بنية buf *buf, بنية buf *buf2, ثابت بنية xkb_ڪنجي *ڪنجي)
{
    bool simple = true;

    xkb_layout_نقاب_t layouts = config->layouts;
    إذا (popcount32(layouts) != keymap->انگ_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_اشاريو_t l = 0;
        لكل (xkb_layout_نقاب_t ls = layouts; ls; ls >>= 1, l++) {
            إذا (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ثابت xkb_layout_اشاريو_t effective =
                    xkb_keymap_ڪنجي_effective_layout(ڪنجي, l);
                إذا (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } وإلا {
        layouts &= ((UINT64_C(1) << ڪنجي->انگ_groups) - 1);
    }
    ثابت xkb_layout_اشاريو_t groups_ڳڻپ = popcount32(layouts);

    ثابت xkb_atom_t name = (substitutions == عدم)
        ? ڪنجي->name
        : substitute_name(substitutions, ڪنجي->name);

    إذا (pretty)
        write_buf(buf, "\tkey %-20s {", ڪنجي_Name_متن(keymap->ctx, name));
    وإلا
        write_buf(buf, "\tkey %s {", ڪنجي_Name_متن(keymap->ctx, name));

    إذا ((ڪنجي->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        إذا (!explicit) {
            لكل (xkb_layout_اشاريو_t گروهه = 1; گروهه < ڪنجي->انگ_groups; گروهه++) {
                إذا (!((UINT32_C(1) << گروهه) & layouts))
                    استمر;

                إذا (ڪنجي->groups[گروهه].type != ڪنجي->groups[0].type) {
                    multi_type = true;
                    اكسر;
                }
            }
        }

        إذا (multi_type) {
            xkb_layout_اشاريو_t new_گروهه = (xkb_layout_اشاريو_t)-1;
            لكل (xkb_layout_اشاريو_t گروهه = 0; گروهه < ڪنجي->انگ_groups; گروهه++) {
                إذا (!((UINT32_C(1) << گروهه) & layouts))
                    استمر;

                new_گروهه++;

                إذا (!ڪنجي->groups[گروهه].explicit_type && !explicit)
                    استمر;

                ثابت بنية xkb_ڪنجي_type * ثابت type = ڪنجي->groups[گروهه].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_گروهه + 1);
                write_buf_string_literal(
                  buf, xkb_atom_متن(keymap->ctx, type->name));
                copy_منزل_buf(buf, ",");
            }
        }
        وإلا {
            ثابت بنية xkb_ڪنجي_type * ثابت type = ڪنجي->groups[0].type;
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
    ثابت bool explicit_actions = (ڪنجي->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #وصف_ڪريو require_explicit(ڪنجي, prop,                                       \
                             قدر, initial_قدر, default_interpret_قدر) ( \
        /* Explicit value in the source */                                    \
        ((ڪنجي)->explicit & (prop)) ||                                         \
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
                ((ڪنجي)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (قدر) != (initial_قدر) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_قدر) != (default_interpret_قدر)                  \
            )                                                                 \
        )                                                                     \
    )

    إذا (require_explicit(ڪنجي, EXPLICIT_REPEAT, ڪنجي->repeats,
                         DEFAULT_ڪنجي_REPEAT, DEFAULT_INTERPRET_ڪنجي_REPEAT)) {
        إذا (ڪنجي->repeats)
            copy_منزل_buf(buf, "\n\t\trepeat= Yes,");
        وإلا
            copy_منزل_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    إذا (require_explicit(ڪنجي, EXPLICIT_VMODMAP, ڪنجي->vmodmap,
                         (xkb_mod_نقاب_t) DEFAULT_ڪنجي_VMODMAP,
                         (xkb_mod_نقاب_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  Mod_نقاب_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              ڪنجي->vmodmap));
        simple = false;
    }

    #وصف_رد require_explicit

    بدل (ڪنجي->ٻاهريون_جو_range_گروهه_policy) {
    حالة XKB_LAYOUT_ٻاهريون_جو_RANGE_CLAMP:
        copy_منزل_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        اكسر;

    حالة XKB_LAYOUT_ٻاهريون_جو_RANGE_REDIRECT: {
        إذا (!groups_ڳڻپ) {
            /* No layout to redirect to: skip */
            اكسر;
        }

        xkb_layout_نقاب_t layout_نقاب =
            (UINT32_C(1) << ڪنجي->ٻاهريون_جو_range_گروهه_عدد);
        إذا (!(layouts & layout_نقاب)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_پيغام_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     ڪنجي_Name_متن(keymap->ctx, name),
                     ڪنجي->ٻاهريون_جو_range_گروهه_عدد + 1);
            layout_نقاب = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ثابت xkb_layout_نقاب_t هيٺيون = (layouts & (layout_نقاب - 1));
        ثابت xkb_layout_اشاريو_t new = popcount32(هيٺيون);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        اكسر;
    }

    حالة XKB_LAYOUT_ٻاهريون_جو_RANGE_WRAP:
    افتراضي:
        اكسر;
    }

    إذا (ڪنجي->overlays) {
        xkb_overlay_نقاب_t بچيل = ڪنجي->overlays;

        ثابت xkb_overlay_اشاريو_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_ڳڻپ == 8, "invalid right shift");
        ثابت xkb_overlay_نقاب_t valid =
            (xkb_overlay_نقاب_t)((1u << overlay_max) - 1u);
        بچيل &= valid;
        إذا (بچيل != ڪنجي->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_اشاريو,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, ڪنجي_Name_متن(keymap->ctx, name),
                     بچيل, ڪنجي->overlays & ~valid);
        }

        إذا (بچيل && !ڪنجي->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                بچيل &
                (xkb_overlay_نقاب_t)((بنانشان)~بچيل + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_اشاريو_t overlay =
                (xkb_overlay_اشاريو_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     ڪنجي_Name_متن(keymap->ctx, name), overlay, بچيل & ~lsb);
            بچيل = lsb;
        }

        إذا (بچيل)
            simple = false;

        ثابت بنية xkb_ڪنجي * ثابت *overlays_ڪنجيون = (ڪنجي->overlays_inline)
                                                    ? &ڪنجي->overlay_ڪنجي
                                                    : ڪنجي->overlays_ڪنجيون;
        طالما (بچيل) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_نقاب_t lsb = (xkb_overlay_نقاب_t)(
                بچيل &
                (xkb_overlay_نقاب_t)((بنانشان)~بچيل + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_اشاريو_t overlay =
                (xkb_overlay_اشاريو_t)popcount32(lsb - 1u);
            بچيل = (بچيل & ~lsb);

            /* pop current value */
            إذا (ڪنجي->overlays_inline && بچيل)
                PANIC_UNREACHABLE();
            ثابت بنية xkb_ڪنجي *overlay_ڪنجي = *(overlays_ڪنجيون++);

            ثابت xkb_atom_t overlay_ڪنجي_name = (substitutions == عدم)
                ? overlay_ڪنجي->name
                : substitute_name(substitutions, overlay_ڪنجي->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, ڪنجي_Name_متن(keymap->ctx, overlay_ڪنجي_name));
        }
    }

    إذا (groups_ڳڻپ > 1 || explicit_actions || explicit)
        simple = false;

    إذا (simple) {
        إذا (groups_ڳڻپ == 0) {
            /* Remove trailing comma */
            delete_آخري_char(buf, ',');
            إذا (buf->buf[buf->سائيز - 1] == ',')
                buf->سائيز--;
        } وإلا {
            copy_منزل_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_اشاريو_t گروهه = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            گروهه = xkb_keymap_ڪنجي_effective_layout(ڪنجي, گروهه);
            إذا (!write_keysyms(keymap, buf, buf2, ڪنجي, گروهه, pretty, false))
                أرجع false;
            copy_منزل_buf(buf, " ]");
        }
        copy_منزل_buf(buf, " };\n");
    }
    وإلا {
        xkb_layout_اشاريو_t new_گروهه = (xkb_layout_اشاريو_t)-1;
        لكل (xkb_layout_اشاريو_t گروهه = 0; گروهه < ڪنجي->انگ_groups; گروهه++) {
            إذا (!((UINT32_C(1) << گروهه) & layouts))
                استمر;

            new_گروهه++;

            ثابت bool print_actions =
                /* Group has explicit actions */
                ڪنجي->groups[گروهه].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (ڪنجي->groups[گروهه].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (ڪنجي->groups[گروهه].implicit_actions && !some_interprets);

            إذا (new_گروهه != 0)
                copy_منزل_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_گروهه + 1);

            إذا (!write_keysyms(keymap, buf, buf2, ڪنجي, گروهه,
                               pretty, print_actions))
                أرجع false;
            copy_منزل_buf(buf, " ]");
            إذا (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_گروهه + 1);
                إذا (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, ڪنجي, گروهه))
                    أرجع false;
                copy_منزل_buf(buf, " ]");
            }
        }
        إذا (!groups_ڳڻپ)
            delete_آخري_char(buf, ',');
        copy_منزل_buf(buf, "\n\t};\n");
    }

    أرجع true;
}


ساكن صحيح
cmp_modmap_قدم_1(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت Mod_Map_داخلا *m1 = (ثابت Mod_Map_داخلا *)a;
    ثابت Mod_Map_داخلا *m2 = (ثابت Mod_Map_داخلا *)b;

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
    إذا (m1->ڪنجي_ڪوڊ < m2->ڪنجي_ڪوڊ)
        أرجع -1;
    وإلا إذا (m1->ڪنجي_ڪوڊ > m2->ڪنجي_ڪوڊ)
        أرجع +1;

    إذا (m1->mods < m2->mods)
        أرجع -1;
    وإلا إذا (m1->mods > m2->mods)
        أرجع +1;

    إذا (m1->have_نشان < m2->have_نشان)
        أرجع +1;
    وإلا إذا (m1->have_نشان > m2->have_نشان)
        أرجع -1;

    إذا (m1->have_نشان) {
        إذا (m1->u.ڪنجي_Sym < m2->u.ڪنجي_Sym)
            أرجع -1;
        إذا (m1->u.ڪنجي_Sym > m2->u.ڪنجي_Sym)
            أرجع +1;
    } وإلا {
        إذا (m1->u.ڪنجي_Name < m2->u.ڪنجي_Name)
            أرجع -1;
        إذا (m1->u.ڪنجي_Name > m2->u.ڪنجي_Name)
            أرجع +1;
    }

    أرجع 0;
}

ساكن صحيح
cmp_modmap_قدم_2(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت Mod_Map_داخلا *m1 = (ثابت Mod_Map_داخلا *)a;
    ثابت Mod_Map_داخلا *m2 = (ثابت Mod_Map_داخلا *)b;

    إذا (m1->mods < m2->mods)
        أرجع -1;
    إذا (m1->mods > m2->mods)
        أرجع +1;

    إذا (m1->have_نشان < m2->have_نشان)
        أرجع -1;
    إذا (m1->have_نشان > m2->have_نشان)
        أرجع +1;

    إذا (m1->have_نشان) {
        إذا (m1->u.ڪنجي_Sym < m2->u.ڪنجي_Sym)
            أرجع -1;
        إذا (m1->u.ڪنجي_Sym > m2->u.ڪنجي_Sym)
            أرجع +1;
    } وإلا {
        إذا (m1->ڪنجي_ڪوڊ < m2->ڪنجي_ڪوڊ)
            أرجع -1;
        إذا (m1->ڪنجي_ڪوڊ > m2->ڪنجي_ڪوڊ)
            أرجع +1;
    }

    أرجع 0;
}

ساكن bool
write_modmaps(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
              ثابت ڪنجي_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, بنية buf *buf)
{
    إذا (!keymap->انگ_modmaps)
        أرجع true;

    darray(Mod_Map_داخلا) modmaps = darray_new();
    darray_from_شيون(modmaps, keymap->modmaps, keymap->انگ_modmaps);
    إذا (!darray_شيون(modmaps))
        أرجع false;

    /* Sort entries by key code */
    qsort(darray_شيون(modmaps), darray_سائيز(modmaps),
          حجم(*darray_شيون(modmaps)), &cmp_modmap_قدم_1);

    /* Drop disabled entries at the end */
    لكل (darray_سائيز_t m = darray_سائيز(modmaps); m-- > 0;) {
        إذا (darray_شي(modmaps, m).mods)
            اكسر;
        darray_سائيز(modmaps)--;
    }

    /*
     * Build detailed keymap modmap
     *
     * NOTE: use an explicit for-loop instead of the darray_foreach macro
     * because we may add entries while iterating it. Iterate from the end,
     * so that we do not iterate the new entries.
     */
    xkb_keycode_t current_keycode = XKB_KEYCODE_INVALID;
    bool convert_منزل_v1_compatibility_داخلا = false;
    لكل (darray_سائيز_t m = darray_سائيز(modmaps); m-- > 0;) {
        Mod_Map_داخلا *mm = &darray_شي(modmaps, m);
        أكد(mm->mods);

        إذا (mm->ڪنجي_ڪوڊ == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ثابت xkb_mod_نقاب_t core_mods = (mm->mods & MOD_REAL_نقاب_ALL);
            إذا (convert_منزل_v1_compatibility_داخلا && core_mods) {
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

        current_keycode = mm->ڪنجي_ڪوڊ;
        ثابت بنية xkb_ڪنجي *ڪنجي = Xkb_ڪنجي(keymap, mm->ڪنجي_ڪوڊ);

        ثابت xkb_mod_نقاب_t core_mods = (ڪنجي->modmap & MOD_REAL_نقاب_ALL);
        إذا (format == XKB_KEYMAP_FORMAT_متن_V1 &&
            ((ڪنجي->modmap & (ڪنجي->modmap - 1)) || ڪنجي->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_منزل_v1_compatibility_داخلا = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_نقاب,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    ڪنجي_Name_متن(keymap->ctx, ڪنجي->name), ڪنجي->modmap);
        } وإلا {
            /* V2 format or V1 format with a single core modifier */
            أكد(ڪنجي->modmap);
            mm->mods = ڪنجي->modmap;
            /* Force key name */
            mm->have_نشان = false;
            mm->u.ڪنجي_Name = ڪنجي->name;
            convert_منزل_v1_compatibility_داخلا = false;
        }
    }

    إذا (darray_خالي_جايون(modmaps))
        اذهب ٻاهر_نڪرو;

    /* Sort entries by modifier mask */
    qsort(darray_شيون(modmaps), darray_سائيز(modmaps),
          حجم(*darray_شيون(modmaps)), &cmp_modmap_قدم_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_نقاب_t current_mods = 0;
    ثابت Mod_Map_داخلا *داخلا;
    darray_foreach(داخلا, modmaps) {
        إذا (!داخلا->mods)
            استمر;

        إذا (current_mods != داخلا->mods) {
            إذا (current_mods) {
                copy_منزل_buf(buf, " };\n");
            }
            current_mods = داخلا->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      Mod_نقاب_متن(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } وإلا {
            copy_منزل_buf(buf, ", ");
        }

        إذا (داخلا->have_نشان) {
            إذا (pretty || داخلا->u.ڪنجي_Sym == XKB_ڪنجي_No_نشان)
                write_buf(buf, "%s", Keysym_متن(keymap->ctx, داخلا->u.ڪنجي_Sym));
            وإلا
                write_buf(buf, "0x%"PRIx32, داخلا->u.ڪنجي_Sym);
        } وإلا {
            ثابت xkb_atom_t name = (substitutions == عدم)
                ? داخلا->u.ڪنجي_Name
                : substitute_name(substitutions, داخلا->u.ڪنجي_Name);
            write_buf(buf, "%s", ڪنجي_Name_متن(keymap->ctx, name));
        }
    }
    إذا (current_mods) {
        copy_منزل_buf(buf, " };\n");
    }

ٻاهر_نڪرو:
    darray_free(modmaps);
    أرجع true;
}

ساكن bool
write_symbols(ثابت بنية xkb_keymap *keymap,
              ثابت بنية xkb_keymap_serialize_config *config,
              ثابت ڪنجي_name_substitutions *substitutions,
              xkb_layout_اشاريو_t max_groups, bool some_interp,
              بنية buf *buf)
{
    ثابت bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_قدرون);
    ثابت bool explicit_ڪنجي_قدرون =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ڪنجي_قدرون);
    ثابت bool drop_اڻ_استعمال_ٿيل = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_اڻ_استعمال_ٿيل);
    ثابت bool drop_interprets = (explicit_ڪنجي_قدرون && drop_اڻ_استعمال_ٿيل);
    /* TODO: print all default values if explicit flag is set */

    إذا (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    وإلا
        copy_منزل_buf(buf, "xkb_symbols {\n");

    bool has_گروهه_نالا = false;
    xkb_layout_اشاريو_t new_گروهه = (xkb_layout_اشاريو_t)-1;
    لكل (xkb_layout_اشاريو_t گروهه = 0; گروهه < keymap->انگ_گروهه_نالا; گروهه++) {
        إذا (!((UINT32_C(1) << گروهه) & config->layouts))
            استمر;

        new_گروهه++;

        إذا (keymap->گروهه_نالا[گروهه]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_گروهه + 1);
            write_buf_string_literal(
                buf, xkb_atom_متن(keymap->ctx, keymap->گروهه_نالا[گروهه]));
            copy_منزل_buf(buf, ";\n");
            has_گروهه_نالا = true;
        }
    }
    إذا (has_گروهه_نالا)
        copy_منزل_buf(buf, "\n");

    إذا (defaults && !write_actions_defaults(keymap, config->format, buf))
        أرجع false;

    بنية buf buf2 = { عدم, 0, 0 };
    ثابت بنية xkb_ڪنجي *ڪنجي;
    xkb_ڪنجيون_foreach(ڪنجي, keymap) {
        /* Skip keys with no explicit values */
        إذا (ڪنجي->explicit &&
            !write_ڪنجي(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_ڪنجي_قدرون, pretty, buf,
                       &buf2, ڪنجي))
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
             بنية buf *buf, بنية xkb_keymap_serialize_نتيجو *نتيجو)
{
    ڪنجي_name_substitutions substitutions = darray_new();
    إذا (config->format == XKB_KEYMAP_FORMAT_متن_V1 &&
        !rename_long_ڪنجيون(keymap, &substitutions))
    {
        أرجع false;
    }

    ثابت ڪنجي_name_substitutions * ثابت substitutions_ptr =
        (darray_خالي_جايون(substitutions)) ? عدم : &substitutions;

    ثابت xkb_layout_اشاريو_t max_groups = format_max_groups(config->format);

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
        بنية xkb_keymap_serialize_نتيجو *نتيجو
)
{
    بنية buf buf = { عدم, 0, 0 };

    ثابت تعداد xkb_حالت حالت =
        write_keymap(keymap, config, &buf, نتيجو);
    إذا (حالت != XKB_SUCCESS) {
        حرر(buf.buf);
        نتيجو->serialized = عدم;
        أرجع حالت;
    }

    نتيجو->serialized = buf.buf;
    نتيجو->ڊيگهائي = buf.سائيز + 1;
    نتيجو->layouts = config->layouts;

    أرجع XKB_SUCCESS;
}
