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

#تعریف_کړه BUF_CHUNK_اندازه 4096

بنية buf {
    حرف *buf;
    اندازه_t اندازه;
    اندازه_t alloc;
};

#تعریف_کړه xkb_abs(n) _Generic((n),                \
                            صحيح: مطلق_قیمت,           \
                            اوږد صحيح: labs,     \
                            افتراضي: llabs )((n))

ساكن bool
do_realloc(بنية buf *buf, اندازه_t at_لږترلږه)
{
    buf->alloc += BUF_CHUNK_اندازه;
    إذا (at_لږترلږه >= BUF_CHUNK_اندازه)
        buf->alloc += at_لږترلږه;

    حرف *ثابت new = realloc(buf->buf, buf->alloc);
    إذا (!new)
        أرجع false;

    buf->buf = new;
    أرجع true;
}

ATTR_PRINTF(2, 3) ساكن bool
check_write_buf(بنية buf *buf, ثابت حرف *fmt, ...)
{
    va_list دلیلونه;
    صحيح printed;
    اندازه_t شته;

    شته = buf->alloc - buf->اندازه;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    أكد(buf->buf || شته == 0);
    va_پیل(دلیلونه, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->اندازه : buf->buf, شته,
                        fmt, دلیلونه);
    va_پای_ټکی(دلیلونه);

    إذا (printed < 0) {
        /* Some error */
        اذهب err;
    } وإلا إذا ((اندازه_t)printed >= شته) {
        /* Not enough space: need to realloc */
        إذا (!do_realloc(buf, (اندازه_t)printed + 1))
            اذهب err;

        /* The buffer has enough space now. */
        شته = buf->alloc - buf->اندازه;
        أكد(buf->buf && (اندازه_t) printed < شته);
        va_پیل(دلیلونه, fmt);
        printed = vsnprintf(buf->buf + buf->اندازه, شته, fmt, دلیلونه);
        va_پای_ټکی(دلیلونه);

        إذا (printed < 0 || (اندازه_t) printed >= شته)
            اذهب err;
    }

    buf->اندازه += (اندازه_t)printed;
    أرجع true;

err:
    حرر(buf->buf);
    buf->buf = عدم;
    أرجع false;
}

#تعریف_کړه write_buf(buf, ...) افعل { \
    إذا (!check_write_buf(buf, __VA_ARGS__)) \
        أرجع false; \
} طالما (0)

ساكن bool
check_copy_منزل_buf(بنية buf *buf, ثابت حرف* سرچینه, اندازه_t len)
{
    إذا (len == 0)
        أرجع true;

    ثابت اندازه_t شته = buf->alloc - buf->اندازه;
    إذا (len >= شته &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        حرر(buf->buf);
        buf->buf = عدم;
        أرجع false;
    }

    حافظه_نقل(buf->buf + buf->اندازه, سرچینه, len);
    buf->اندازه += len;
    /* Append NULL byte */
    buf->buf[buf->اندازه] = '\0';
    أرجع true;
}

#تعریف_کړه copy_منزل_buf_len(buf, سرچینه, len) افعل { \
    إذا (!check_copy_منزل_buf(buf, سرچینه, len)) \
        أرجع false; \
} طالما (0)

#تعریف_کړه copy_منزل_buf(buf, سرچینه) \
    copy_منزل_buf_len(buf, سرچینه, حجم(سرچینه) - 1)

ساكن bool
check_write_string_literal(بنية buf *buf, ثابت حرف* string)
{
    copy_منزل_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    اندازه_t pending_پیل = 0;
    اندازه_t current = 0;
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
            إذا (unlikely((بینښه حرف)string[current] < 0x20U ||
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
        أكد(current >= pending_پیل);
        copy_منزل_buf_len(buf, string + pending_پیل, current - pending_پیل);
        /* Write escape sequence */
        copy_منزل_buf_len(buf, escape_بفر, escape_len);
        pending_پیل = current + 1;
    }
    /* Write remaining chunk */
    أكد(current >= pending_پیل);
    copy_منزل_buf_len(buf, string + pending_پیل, current - pending_پیل);

    copy_منزل_buf(buf, "\"");
    أرجع true;
}

#تعریف_کړه write_buf_string_literal(buf, string) افعل { \
    إذا (!check_write_string_literal(buf, string)) \
        أرجع false; \
} طالما (0)

ساكن فراغ
delete_وروستی_char(بنية buf *buf, حرف c)
{
    إذا (buf->اندازه && buf->buf[buf->اندازه - 1] == c)
        buf->buf[--buf->اندازه] = '\0';
}

/** Entry for a name substituion LUT */
بنية کلی_name_substitution {
    xkb_atom_t سرچینه;
    xkb_atom_t موخه;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
ساكن صحيح
cmp_کلی_name_substitution(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت xkb_atom_t s1 = ((بنية کلی_name_substitution *) a)->سرچینه;
    ثابت xkb_atom_t s2 = ((بنية کلی_name_substitution *) b)->سرچینه;
    أرجع (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

تعريفنوع darray(بنية کلی_name_substitution) کلی_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
ساكن bool
rename_long_کلیګانې(ثابت بنية xkb_keymap *keymap,
                 کلی_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    تعداد {
        لومړی = '!',
        وروستی = '0',
        invalid = '\0'
    };
    حرف prefixes[] = {
        لومړی, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', وروستی
    };
    static_assert(حجم(prefixes) == وروستی - لومړی + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ثابت بنية xkb_کلی *کلی;
    xkb_کلیګانې_foreach(کلی, keymap) {
        إذا (کلی->name == XKB_ATOM_NONE)
            استمر;

        ثابت حرف * ثابت name = xkb_atom_متن(keymap->ctx, کلی->name);
        ثابت اندازه_t len = strlen_خوندي(name);
        إذا (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (بنية کلی_name_substitution) {
                .سرچینه = کلی->name,
                .keycode = کلی->keycode,
                .موخه = کلی->name
            });
        } وإلا إذا (len == 4 && name[0] >= لومړی && name[0] <= وروستی) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - لومړی] = invalid;
        }
    }

    /* Check aliases */
    لكل (darray_اندازه_t a = 0; a < keymap->عدد_کلی_aliases; a++) {
        ثابت بنية xkb_کلی_alias * ثابت alias = &keymap->کلی_aliases[a];
        ثابت حرف * ثابت name = xkb_atom_متن(keymap->ctx, alias->alias);
        ثابت اندازه_t len = strlen_خوندي(name);
        إذا (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (بنية کلی_name_substitution) {
                .سرچینه = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .موخه = alias->alias
            });
        } وإلا إذا (len == 4 && name[0] >= لومړی && name[0] <= وروستی) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - لومړی] = invalid;
        }
    }

    إذا darray_تشه(*substitutions) {
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
    حرف *مختاړی = prefixes;
    ثابت حرف *مختاړی_max = prefixes + حجم(prefixes) - 1;
    طالما (*مختاړی == invalid && مختاړی < مختاړی_max)
        مختاړی++;

    /* Keycode prefix: prefer ‘0’ if available */
    حرف keycode_مختاړی;
    إذا (prefixes[وروستی - لومړی] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_مختاړی = وروستی;
        prefixes[وروستی - لومړی] = invalid;
    } وإلا {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_مختاړی = *مختاړی;
        إذا (unlikely(keycode_مختاړی == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            أرجع true;
        }
        /* Update current prefix */
        *مختاړی = invalid;
        طالما (*مختاړی == invalid && مختاړی < مختاړی_max)
            مختاړی++;
    }

    /* Proceed renaming */
    uint16_t بل_id = 1;
    بنية کلی_name_substitution *sub;
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
                     keycode_مختاړی, sub->keycode);
            ثابت xkb_atom_t موخه = xkb_atom_intern(keymap->ctx,
                                                      buf, حجم(buf) - 2);
            إذا (موخه == XKB_ATOM_NONE)
                اذهب error;
            sub->موخه = موخه;
        } وإلا إذا (*مختاړی != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, حجم(buf), "%c%03"PRIx16, *مختاړی, بل_id);
            ثابت xkb_atom_t موخه = xkb_atom_intern(keymap->ctx,
                                                      buf, حجم(buf) - 2);
            إذا (موخه == XKB_ATOM_NONE)
                اذهب error;
            sub->موخه = موخه;
            إذا (++بل_id > 0xfff) {
                *مختاړی = invalid;
                /* Get next available prefix */
                طالما (*مختاړی == invalid && مختاړی < مختاړی_max)
                    مختاړی++;
                /* Reset counter */
                بل_id = 1;
            }
        } وإلا {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->موخه = sub->سرچینه;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_توکي(*substitutions), darray_اندازه(*substitutions),
          حجم(*darray_توکي(*substitutions)), &cmp_کلی_name_substitution);

    أرجع true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ویش_FAILURE_2,
            "Cannot allocate key name substitution\n");
    أرجع false;
}

/** Substitute a key name with a valid X11 key name */
ساكن xkb_atom_t
substitute_name(ثابت کلی_name_substitutions *substitutions, xkb_atom_t name)
{
    /*دوګونې پلټنه*/
    darray_اندازه_t لومړی = 0;
    darray_اندازه_t وروستی = darray_اندازه(*substitutions);

    طالما (وروستی > لومړی) {
        darray_اندازه_t mid = لومړی + (وروستی - 1 - لومړی) / 2;
        ثابت xkb_atom_t سرچینه = darray_توکی(*substitutions, mid).سرچینه;
        إذا (سرچینه < name)
            لومړی = mid + 1;
        وإلا إذا (سرچینه > name)
            وروستی = mid;
        وإلا /* found it */
            أرجع darray_توکی(*substitutions, mid).موخه;
    }

    /* No name substitution: return original name */
    أرجع name;
}

ساكن bool
write_vmods(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
            bool explicit, بنية buf *buf)
{
    ثابت بنية xkb_mod *mod;
    xkb_mod_شاخص_t vmod = 0;
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
                      Mod_ماسک_متن(keymap->ctx, MOD_REAL, &keymap->mods,
                                  mod->mapping));
        }
    }

    إذا (has_some)
        copy_منزل_buf(buf, ";\n\n");

    أرجع true;
}

ساكن bool
write_keycodes(ثابت بنية xkb_keymap *keymap,
               ثابت کلی_name_substitutions *substitutions,
               تعداد xkb_keymap_serialize_flags flags, بنية buf *buf)
{
    ثابت بنية xkb_کلی *کلی;
    xkb_led_شاخص_t idx;
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
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_کلی_کوډ, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_کلی_کوډ, 255));

    xkb_کلیګانې_foreach(کلی, keymap) {
        إذا (کلی->name == XKB_ATOM_NONE)
            استمر;

        ثابت xkb_atom_t name = (substitutions == عدم)
            ? کلی->name
            : substitute_name(substitutions, کلی->name);
        إذا (pretty)
            write_buf(buf, "\t%-20s", کلی_Name_متن(keymap->ctx, name));
        وإلا
            write_buf(buf, "\t%s", کلی_Name_متن(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", کلی->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        إذا (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_متن(keymap->ctx, led->name));
            copy_منزل_buf(buf, ";\n");
        }

    لكل (darray_اندازه_t i = 0; i < keymap->عدد_کلی_aliases; i++) {
        ثابت xkb_atom_t alias = (substitutions == عدم)
            ? keymap->کلی_aliases[i].alias
            : substitute_name(substitutions, keymap->کلی_aliases[i].alias);
        ثابت xkb_atom_t real = (substitutions == عدم)
            ? keymap->کلی_aliases[i].real
            : substitute_name(substitutions, keymap->کلی_aliases[i].real);

        إذا (pretty)
            write_buf(buf, "\talias %-14s",
                      کلی_Name_متن(keymap->ctx, alias));
        وإلا
            write_buf(buf, "\talias %s",
                      کلی_Name_متن(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  کلی_Name_متن(keymap->ctx, real));
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

    ثابت bool drop_ناکارول_شوی = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_ناکارول_شوی);
    لكل (darray_اندازه_t i = 0; i < keymap->عدد_types; i++) {
        ثابت بنية xkb_کلی_type * ثابت type = &keymap->types[i];
        إذا (!type->required && drop_ناکارول_شوی)
            استمر;

        copy_منزل_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_متن(keymap->ctx, type->name));
        copy_منزل_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_ماسک_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        لكل (darray_اندازه_t j = 0; j < type->عدد_entries; j++) {
            ثابت حرف *str;
            ثابت بنية xkb_کلی_type_ثبت *ثبت = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            إذا (ثبت->level == 0 && ثبت->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ارزښتونه))
                استمر;

            str = Mod_ماسک_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              ثبت->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, ثبت->level + 1);

            إذا (ثبت->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, Mod_ماسک_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           ثبت->preserve.mods));
        }

        لكل (xkb_level_شاخص_t n = 0; n < type->عدد_level_نومونه; n++)
            إذا (type->level_نومونه[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_متن(keymap->ctx, type->level_نومونه[n]));
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
                      Led_State_ماسک_متن(keymap->ctx, ډله_Component_ماسک_نومونه,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    إذا (led->which_mods) {
        إذا (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      Led_State_ماسک_متن(keymap->ctx, mod_Component_ماسک_نومونه,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_ماسک_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    إذا (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  Control_ماسک_متن(keymap->ctx, format, led->ctrls));
    }

    copy_منزل_buf(buf, "\t};\n");
    أرجع true;
}

ساكن ثابت حرف *
affect_قلف_متن(تعداد xkb_action_flags flags, bool show_both)
{
    بدل (flags & (ACTION_قلف_NO_قلف | ACTION_قلف_NO_UNLOCK)) {
    حالة 0:
        أرجع show_both ? ",affect=both" : "";
    حالة ACTION_قلف_NO_UNLOCK:
        أرجع ",affect=lock";
    حالة ACTION_قلف_NO_قلف:
        أرجع ",affect=unlock";
    حالة ACTION_قلف_NO_قلف | ACTION_قلف_NO_UNLOCK:
        أرجع ",affect=neither";
    افتراضي:
        أرجع "";
    }
}

#تعریف_کړه نښه_PADDING 15
#تعریف_کړه ACTION_PADDING 30

ساكن bool
write_action(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
             xkb_layout_شاخص_t max_groups, بنية buf *buf,
             ثابت اتحاد xkb_action *action,
             ثابت حرف *مختاړی, ثابت حرف *suffix)
{
    ثابت حرف *type;
    ثابت حرف *دلیلونه = عدم;

    إذا (!مختاړی)
        مختاړی = "";
    إذا (!suffix)
        suffix = "";

    type = Action_Type_متن(action->type);

    بدل (action->type) {
    حالة ACTION_TYPE_MOD_سېټ:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_قلف:
        إذا (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            دلیلونه = "modMapMods";
        وإلا
            دلیلونه = Mod_ماسک_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        إذا (unlockOnPress && !is_Mods_Un_قلف_On_Press_Supported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    Action_Type_متن(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->ډله.flags & ACTION_LATCH_ON_PRESS);
        إذا (latchOnPress && !isModsLatchOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot use \"LatchMods(latchOnPress=true)\" "
                    "in keymap format %d\n", format);
            latchOnPress = false;
        }
        write_buf(buf, "%s%s(modifiers=%s%s%s%s%s%s)%s", مختاړی, type, دلیلونه,
                  (action->type != ACTION_TYPE_MOD_قلف && (action->mods.flags & ACTION_قلف_CLEAR)) ? ",clearLocks" : "",
                  (action->type == ACTION_TYPE_MOD_LATCH && (action->mods.flags & ACTION_LATCH_منزل_قلف)) ? ",latchToLock" : "",
                  (action->type == ACTION_TYPE_MOD_قلف) ? affect_قلف_متن(action->mods.flags, false) : "",
                  (unlockOnPress) ? ",unlockOnPress" : "",
                  (latchOnPress) ? ",latchOnPress" : "",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_ډله_سېټ:
    حالة ACTION_TYPE_ډله_LATCH:
    حالة ACTION_TYPE_ډله_قلف:
        إذا ((uint32_t) xkb_abs(action->ډله.ډله) <
            (max_groups + !(action->ډله.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool قلف_On_Release = action->type == ACTION_TYPE_ډله_قلف &&
                                 (action->ډله.flags & ACTION_قلف_ON_RELEASE);
            إذا (قلف_On_Release && !is_ډله_قلف_On_Release_Supported(format)) {
                log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                        "Cannot use \"GroupLock(lockOnRelease=true)\" "
                        "in keymap format %d\n", format);
                قلف_On_Release = false;
            }
            write_buf(buf, "%s%s(group=%s%"PRId32"%s%s%s)%s", مختاړی, type,
                      (!(action->ډله.flags & ACTION_ABSOLUTE_SWITCH) && action->ډله.ډله >= 0) ? "+" : "",
                      (action->ډله.flags & ACTION_ABSOLUTE_SWITCH) ? action->ډله.ډله + 1 : action->ډله.ډله,
                      (action->type != ACTION_TYPE_ډله_قلف && (action->ډله.flags & ACTION_قلف_CLEAR)) ? ",clearLocks" : "",
                      (action->type == ACTION_TYPE_ډله_LATCH && (action->ډله.flags & ACTION_LATCH_منزل_قلف)) ? ",latchToLock" : "",
                      (قلف_On_Release) ? ",lockOnRelease" : "",
                      suffix);
        } وإلا {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->ډله.ډله +
                       !!(action->ډله.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            اذهب void_action;
        }
        اكسر;

    حالة ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", مختاړی, type, suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", مختاړی, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_قلف:
        دلیلونه = affect_قلف_متن(action->btn.flags, true);
        /* fallthrough */
    حالة ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", مختاړی, type);
        static_assert(XKB_اشاره_ګر_BUTTON_DEFAULT < XKB_اشاره_ګر_BUTTON_MIN, "");
        إذا (action->btn.button > XKB_اشاره_ګر_BUTTON_DEFAULT &&
            action->btn.button <= XKB_اشاره_ګر_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        وإلا
            copy_منزل_buf(buf, "default");
        إذا (action->btn.شمېر) {
            أكد(action->type != ACTION_TYPE_PTR_قلف);
            write_buf(buf, ",count=%u", action->btn.شمېر);
        }
        إذا (دلیلونه)
            write_buf(buf, "%s", دلیلونه);
        write_buf(buf, ")%s", suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", مختاړی, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.ارزښت >= 0) ? "+" : "",
                  action->dflt.ارزښت);
        write_buf(buf, ")%s", suffix);
        اكسر;

    حالة ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", مختاړی, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_CTRL_سېټ:
    حالة ACTION_TYPE_CTRL_قلف:
        write_buf(buf, "%s%s(controls=%s%s)%s", مختاړی, type,
                  Control_ماسک_متن(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_قلف
                    ? affect_قلف_متن(action->ctrls.flags, false)
                    : ""),
                  suffix);
        اكسر;

    حالة ACTION_TYPE_REDIRECT_کلی: {
        write_buf(buf, "%s%s(", مختاړی, type);
        ثابت بنية xkb_کلی * ثابت کلی = Xkb_کلی(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        إذا (کلی)
            write_buf(buf, "keycode=%s", کلی_Name_متن(keymap->ctx, کلی->name));
        إذا (action->redirect.affect_mods) {
            xkb_mod_ماسک_t ماسک;
            ماسک = (action->redirect.affect_mods & action->redirect.mods);
            إذا (ماسک)
                write_buf(buf, ",modifiers=%s",
                          Mod_ماسک_متن(keymap->ctx, MOD_BOTH, &keymap->mods, ماسک));
            ماسک = (action->redirect.affect_mods & ~action->redirect.mods);
            إذا (ماسک)
                write_buf(buf, ",clearMods=%s",
                          Mod_ماسک_متن(keymap->ctx, MOD_BOTH, &keymap->mods, ماسک));
        }
        write_buf(buf, ")%s", suffix);
        اكسر;
    }

    حالة ACTION_TYPE_UNKNOWN:
    حالة ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", مختاړی, suffix);
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
                      مختاړی, suffix);
        وإلا
            write_buf(buf, "%sVoidAction()%s", مختاړی, suffix);
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_دننی == 20 &&
                      ACTION_TYPE_دننی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        أكد(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  مختاړی, type, action->type, action->priv.معلومات[0],
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
              xkb_layout_شاخص_t max_groups, بنية buf *buf, بنية buf *buf2,
              ثابت بنية xkb_کلی *کلی, xkb_layout_شاخص_t ډله)
{
    ساكن ثابت اتحاد xkb_action noAction = { .type = ACTION_TYPE_NONE };

    لكل (xkb_level_شاخص_t level = 0; level < Xkb_کلی_عدد_پوړونه(کلی, ډله);
         level++) {

        إذا (level != 0)
            copy_منزل_buf(buf, ", ");

        ثابت اتحاد xkb_action *actions = عدم;
        xkb_action_شمېر_t شمېر = xkb_keymap_کلی_get_actions_by_level(
            keymap, کلی, ډله, level, &actions
        );

        إذا (شمېر > 1 && format < XKB_KEYMAP_FORMAT_متن_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            شمېر = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->اندازه = 0;
        إذا (شمېر == 0) {
            إذا (!write_action(keymap, format, max_groups,
                              buf2, &noAction, عدم, عدم))
                أرجع false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        وإلا إذا (شمېر == 1) {
            إذا (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), عدم, عدم))
                أرجع false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        وإلا {
            copy_منزل_buf(buf2, "{ ");
            لكل (xkb_action_شمېر_t k = 0; k < شمېر; k++) {
                إذا (k != 0)
                    copy_منزل_buf(buf2, ", ");
                اندازه_t old_اندازه = buf2->اندازه;
                إذا (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), عدم, عدم))
                    أرجع false;
                /* Check if padding is necessary */
                إذا (buf2->اندازه >= old_اندازه + ACTION_PADDING)
                    استمر;
                /* Compute and write padding, then write the action again */
                ثابت صحيح padding = (صحيح)(old_اندازه + ACTION_PADDING - buf2->اندازه);
                buf2->اندازه = old_اندازه;
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

#تعریف_کړه مختاړی_2 "\t"

    بدل (action->type) {
    حالة ACTION_TYPE_MOD_سېټ:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_قلف:
        أكد(action->mods.flags == 0);
        أكد(action->mods.mods.mods == 0);
        write_buf(buf, مختاړی_2"%s.modifiers = 0;\n", type);
        إذا (action->type != ACTION_TYPE_MOD_قلف)
            write_buf(buf, مختاړی_2"%s.clearLocks = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, مختاړی_2"%s.latchOnPress = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, مختاړی_2"%s.latchToLock = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_قلف)
            write_buf(buf, مختاړی_2"%s.affect = both;\n", type);
        إذا (is_Mods_Un_قلف_On_Press_Supported(format))
            write_buf(buf, مختاړی_2"%s.unlockOnPress = false;\n", type);
        اكسر;

    حالة ACTION_TYPE_ډله_سېټ:
    حالة ACTION_TYPE_ډله_LATCH:
    حالة ACTION_TYPE_ډله_قلف:
        أكد(action->ډله.flags == 0);
        أكد(action->ډله.ډله == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, مختاړی_2"%s.group = +0;\n", type);
        إذا (action->type != ACTION_TYPE_ډله_قلف)
            write_buf(buf, مختاړی_2"%s.clearLocks = false;\n", type);
        إذا (action->type == ACTION_TYPE_ډله_LATCH)
            write_buf(buf, مختاړی_2"%s.latchToLock = false;\n", type);
        إذا (action->type == ACTION_TYPE_ډله_قلف &&
            is_ډله_قلف_On_Release_Supported(format))
            write_buf(buf, مختاړی_2"%s.lockOnRelease = false;\n", type);
        اكسر;


    حالة ACTION_TYPE_PTR_MOVE:
        أكد(action->ptr.x == 0);
        أكد(action->ptr.y == 0);
        أكد(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, مختاړی_2"%s.x = +0;\n", type);
        write_buf(buf, مختاړی_2"%s.y = +0;\n", type);
        write_buf(buf, مختاړی_2"%s.accel = true;\n", type);
        اكسر;

    حالة ACTION_TYPE_PTR_قلف:
    حالة ACTION_TYPE_PTR_BUTTON:
        أكد(action->btn.flags == 0);
        أكد(action->btn.button == 0);
        أكد(action->btn.شمېر == 0);
        write_buf(buf, مختاړی_2"%s.button = 0;\n", type);
        write_buf(buf, مختاړی_2"%s.count = 0;\n", type);
        إذا (action->type == ACTION_TYPE_PTR_قلف)
            write_buf(buf, مختاړی_2"%s.affect = both;\n", type);
        اكسر;

    حالة ACTION_TYPE_PTR_DEFAULT:
        أكد(action->dflt.flags == 0);
        أكد(action->dflt.ارزښت == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, مختاړی_2"%s.button = +1;\n", type);
        اكسر;

    حالة ACTION_TYPE_SWITCH_VT:
        أكد(action->screen.flags == ACTION_SAME_SCREEN);
        أكد(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, مختاړی_2"%s.screen = +0;\n", type);
        write_buf(buf, مختاړی_2"%s.same = true;\n", type);
        اكسر;

    حالة ACTION_TYPE_CTRL_سېټ:
    حالة ACTION_TYPE_CTRL_قلف:
        أكد(action->ctrls.flags == 0);
        أكد(action->ctrls.ctrls == 0);
        write_buf(buf, مختاړی_2"%s.controls = 0;\n", type);
        إذا (action->type == ACTION_TYPE_CTRL_قلف)
            write_buf(buf, مختاړی_2"%s.affect = both;\n", type);
        اكسر;

    حالة ACTION_TYPE_REDIRECT_کلی:
        أكد(action->redirect.keycode == keymap->redirect_کلی_auto);
        أكد(action->redirect.affect_mods == 0);
        أكد(action->redirect.mods == 0);
        write_buf(buf, مختاړی_2"%s.keycode = auto;\n", type);
        write_buf(buf, مختاړی_2"%s.modifiers = 0;\n", type);
        write_buf(buf, مختاړی_2"%s.clearMods = 0;\n", type);
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_دننی == 20 &&
                      ACTION_TYPE_دننی + 1 == ACTION_TYPE_عدد_ENTRIES,
                      "Missing action type");
    }

#تعریف_لغوه مختاړی_2

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
    .sym = XKB_کلی_Void_نښه,
    .required = true,
    .repeat = FALLBACK_INTERPRET_کلی_REPEAT,
    .سمون = سمون_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_شاخص_t) DEFAULT_INTERPRET_VMOD,
    .عدد_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ساكن bool
write_compat(ثابت بنية xkb_keymap * محدود keymap,
             تعداد xkb_keymap_format format, xkb_layout_شاخص_t max_groups,
             تعداد xkb_keymap_serialize_flags flags, bool * محدود some_interp,
             بنية buf *buf)
{
    ثابت bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ارزښتونه);
    ثابت bool drop_ناکارول_شوی = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_ناکارول_شوی);
    ثابت bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_کلی_ارزښتونه) && drop_ناکارول_شوی);
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
    ثابت darray_اندازه_t عدد_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->عدد_sym_interprets);
    ثابت بنية xkb_sym_interpret* ثابت sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    لكل (darray_اندازه_t i = 0; i < عدد_sym_interprets; i++) {
        ثابت بنية xkb_sym_interpret *si = &sym_interprets[i];
        إذا (!si->required && drop_ناکارول_شوی)
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
                  SIMatch_متن(si->سمون),
                  Mod_ماسک_متن(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        إذا (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      Mod_شاخص_متن(keymap->ctx, &keymap->mods,
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
        xkb_action_شمېر_t action_شمېر = si->عدد_actions;
        إذا (action_شمېر <= 1) {
            action = &si->a.action;
        } وإلا إذا (format < XKB_KEYMAP_FORMAT_متن_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_شمېر = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_متن_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        إذا (action_شمېر > 1) {
            copy_منزل_buf(buf, "\n\t\taction= {");
            ثابت حرف suffix[] = ", ";
            لكل (xkb_action_شمېر_t k = 0; k < si->عدد_actions; k++) {
                إذا (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    أرجع false;
            }
            buf->اندازه -= حجم(suffix) - 1; /* trailing comma */
            copy_منزل_buf(buf, "};");
            has_explicit_properties = true;
        } وإلا إذا (action_شمېر == 1) {
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
        sym_interprets[0].سمون == fallback_interpret.سمون &&
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
              ثابت بنية xkb_کلی *کلی, xkb_layout_شاخص_t ډله,
              bool pretty, bool show_actions)
{
    ثابت بینښه صحيح padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : نښه_PADDING)
        : 0;
    لكل (xkb_level_شاخص_t level = 0; level < Xkb_کلی_عدد_پوړونه(کلی, ډله);
         level++) {
        ثابت xkb_keysym_t *syms;
        صحيح عدد_syms;

        إذا (level != 0)
            copy_منزل_buf(buf, ", ");

        عدد_syms = xkb_keymap_کلی_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (بنية xkb_keymap *)keymap,
            کلی->keycode, ډله, level, &syms
        );

        ثابت xkb_keysym_t no_نښه = XKB_کلی_No_نښه;
        إذا (عدد_syms == 0) {
            عدد_syms = 1;
            syms = &no_نښه;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        إذا (عدد_syms == 1) {
            إذا (pretty || syms[0] == XKB_کلی_No_نښه)
                write_buf(buf, "%*s", padding, Keysym_متن(keymap->ctx, syms[0]));
            وإلا
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } وإلا {
            إذا (pretty) {
                buf2->اندازه = 0;
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
                    إذا (syms[s] == XKB_کلی_No_نښه)
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
write_کلی(ثابت بنية xkb_keymap *keymap,
          ثابت بنية xkb_keymap_serialize_config *config,
          ثابت کلی_name_substitutions *substitutions,
          xkb_layout_شاخص_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          بنية buf *buf, بنية buf *buf2, ثابت بنية xkb_کلی *کلی)
{
    bool simple = true;

    xkb_layout_ماسک_t layouts = config->layouts;
    إذا (popcount32(layouts) != keymap->عدد_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_شاخص_t l = 0;
        لكل (xkb_layout_ماسک_t ls = layouts; ls; ls >>= 1, l++) {
            إذا (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ثابت xkb_layout_شاخص_t effective =
                    xkb_keymap_کلی_effective_layout(کلی, l);
                إذا (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } وإلا {
        layouts &= ((UINT64_C(1) << کلی->عدد_groups) - 1);
    }
    ثابت xkb_layout_شاخص_t groups_شمېر = popcount32(layouts);

    ثابت xkb_atom_t name = (substitutions == عدم)
        ? کلی->name
        : substitute_name(substitutions, کلی->name);

    إذا (pretty)
        write_buf(buf, "\tkey %-20s {", کلی_Name_متن(keymap->ctx, name));
    وإلا
        write_buf(buf, "\tkey %s {", کلی_Name_متن(keymap->ctx, name));

    إذا ((کلی->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        إذا (!explicit) {
            لكل (xkb_layout_شاخص_t ډله = 1; ډله < کلی->عدد_groups; ډله++) {
                إذا (!((UINT32_C(1) << ډله) & layouts))
                    استمر;

                إذا (کلی->groups[ډله].type != کلی->groups[0].type) {
                    multi_type = true;
                    اكسر;
                }
            }
        }

        إذا (multi_type) {
            xkb_layout_شاخص_t new_ډله = (xkb_layout_شاخص_t)-1;
            لكل (xkb_layout_شاخص_t ډله = 0; ډله < کلی->عدد_groups; ډله++) {
                إذا (!((UINT32_C(1) << ډله) & layouts))
                    استمر;

                new_ډله++;

                إذا (!کلی->groups[ډله].explicit_type && !explicit)
                    استمر;

                ثابت بنية xkb_کلی_type * ثابت type = کلی->groups[ډله].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_ډله + 1);
                write_buf_string_literal(
                  buf, xkb_atom_متن(keymap->ctx, type->name));
                copy_منزل_buf(buf, ",");
            }
        }
        وإلا {
            ثابت بنية xkb_کلی_type * ثابت type = کلی->groups[0].type;
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
    ثابت bool explicit_actions = (کلی->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #تعریف_کړه require_explicit(کلی, prop,                                       \
                             ارزښت, initial_ارزښت, default_interpret_ارزښت) ( \
        /* Explicit value in the source */                                    \
        ((کلی)->explicit & (prop)) ||                                         \
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
                ((کلی)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (ارزښت) != (initial_ارزښت) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_ارزښت) != (default_interpret_ارزښت)                  \
            )                                                                 \
        )                                                                     \
    )

    إذا (require_explicit(کلی, EXPLICIT_REPEAT, کلی->repeats,
                         DEFAULT_کلی_REPEAT, DEFAULT_INTERPRET_کلی_REPEAT)) {
        إذا (کلی->repeats)
            copy_منزل_buf(buf, "\n\t\trepeat= Yes,");
        وإلا
            copy_منزل_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    إذا (require_explicit(کلی, EXPLICIT_VMODMAP, کلی->vmodmap,
                         (xkb_mod_ماسک_t) DEFAULT_کلی_VMODMAP,
                         (xkb_mod_ماسک_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  Mod_ماسک_متن(keymap->ctx, MOD_BOTH, &keymap->mods,
                              کلی->vmodmap));
        simple = false;
    }

    #تعریف_لغوه require_explicit

    بدل (کلی->بهر_د_range_ډله_policy) {
    حالة XKB_LAYOUT_بهر_د_RANGE_CLAMP:
        copy_منزل_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        اكسر;

    حالة XKB_LAYOUT_بهر_د_RANGE_REDIRECT: {
        إذا (!groups_شمېر) {
            /* No layout to redirect to: skip */
            اكسر;
        }

        xkb_layout_ماسک_t layout_ماسک =
            (UINT32_C(1) << کلی->بهر_د_range_ډله_شمېره);
        إذا (!(layouts & layout_ماسک)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_پیغام_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     کلی_Name_متن(keymap->ctx, name),
                     کلی->بهر_د_range_ډله_شمېره + 1);
            layout_ماسک = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ثابت xkb_layout_ماسک_t ټیټ = (layouts & (layout_ماسک - 1));
        ثابت xkb_layout_شاخص_t new = popcount32(ټیټ);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        اكسر;
    }

    حالة XKB_LAYOUT_بهر_د_RANGE_WRAP:
    افتراضي:
        اكسر;
    }

    إذا (کلی->overlays) {
        xkb_overlay_ماسک_t پاتې_شوی = کلی->overlays;

        ثابت xkb_overlay_شاخص_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_شمېر == 8, "invalid right shift");
        ثابت xkb_overlay_ماسک_t valid =
            (xkb_overlay_ماسک_t)((1u << overlay_max) - 1u);
        پاتې_شوی &= valid;
        إذا (پاتې_شوی != کلی->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_شاخص,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, کلی_Name_متن(keymap->ctx, name),
                     پاتې_شوی, کلی->overlays & ~valid);
        }

        إذا (پاتې_شوی && !کلی->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_ماسک_t lsb = (xkb_overlay_ماسک_t)(
                پاتې_شوی &
                (xkb_overlay_ماسک_t)((بینښه)~پاتې_شوی + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_شاخص_t overlay =
                (xkb_overlay_شاخص_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     کلی_Name_متن(keymap->ctx, name), overlay, پاتې_شوی & ~lsb);
            پاتې_شوی = lsb;
        }

        إذا (پاتې_شوی)
            simple = false;

        ثابت بنية xkb_کلی * ثابت *overlays_کلیګانې = (کلی->overlays_inline)
                                                    ? &کلی->overlay_کلی
                                                    : کلی->overlays_کلیګانې;
        طالما (پاتې_شوی) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_ماسک_t lsb = (xkb_overlay_ماسک_t)(
                پاتې_شوی &
                (xkb_overlay_ماسک_t)((بینښه)~پاتې_شوی + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_شاخص_t overlay =
                (xkb_overlay_شاخص_t)popcount32(lsb - 1u);
            پاتې_شوی = (پاتې_شوی & ~lsb);

            /* pop current value */
            إذا (کلی->overlays_inline && پاتې_شوی)
                PANIC_UNREACHABLE();
            ثابت بنية xkb_کلی *overlay_کلی = *(overlays_کلیګانې++);

            ثابت xkb_atom_t overlay_کلی_name = (substitutions == عدم)
                ? overlay_کلی->name
                : substitute_name(substitutions, overlay_کلی->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, کلی_Name_متن(keymap->ctx, overlay_کلی_name));
        }
    }

    إذا (groups_شمېر > 1 || explicit_actions || explicit)
        simple = false;

    إذا (simple) {
        إذا (groups_شمېر == 0) {
            /* Remove trailing comma */
            delete_وروستی_char(buf, ',');
            إذا (buf->buf[buf->اندازه - 1] == ',')
                buf->اندازه--;
        } وإلا {
            copy_منزل_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_شاخص_t ډله = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            ډله = xkb_keymap_کلی_effective_layout(کلی, ډله);
            إذا (!write_keysyms(keymap, buf, buf2, کلی, ډله, pretty, false))
                أرجع false;
            copy_منزل_buf(buf, " ]");
        }
        copy_منزل_buf(buf, " };\n");
    }
    وإلا {
        xkb_layout_شاخص_t new_ډله = (xkb_layout_شاخص_t)-1;
        لكل (xkb_layout_شاخص_t ډله = 0; ډله < کلی->عدد_groups; ډله++) {
            إذا (!((UINT32_C(1) << ډله) & layouts))
                استمر;

            new_ډله++;

            ثابت bool print_actions =
                /* Group has explicit actions */
                کلی->groups[ډله].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (کلی->groups[ډله].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (کلی->groups[ډله].implicit_actions && !some_interprets);

            إذا (new_ډله != 0)
                copy_منزل_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_ډله + 1);

            إذا (!write_keysyms(keymap, buf, buf2, کلی, ډله,
                               pretty, print_actions))
                أرجع false;
            copy_منزل_buf(buf, " ]");
            إذا (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_ډله + 1);
                إذا (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, کلی, ډله))
                    أرجع false;
                copy_منزل_buf(buf, " ]");
            }
        }
        إذا (!groups_شمېر)
            delete_وروستی_char(buf, ',');
        copy_منزل_buf(buf, "\n\t};\n");
    }

    أرجع true;
}


ساكن صحيح
cmp_modmap_ګام_1(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت Mod_Map_ثبت *m1 = (ثابت Mod_Map_ثبت *)a;
    ثابت Mod_Map_ثبت *m2 = (ثابت Mod_Map_ثبت *)b;

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
    إذا (m1->کلی_کوډ < m2->کلی_کوډ)
        أرجع -1;
    وإلا إذا (m1->کلی_کوډ > m2->کلی_کوډ)
        أرجع +1;

    إذا (m1->mods < m2->mods)
        أرجع -1;
    وإلا إذا (m1->mods > m2->mods)
        أرجع +1;

    إذا (m1->have_نښه < m2->have_نښه)
        أرجع +1;
    وإلا إذا (m1->have_نښه > m2->have_نښه)
        أرجع -1;

    إذا (m1->have_نښه) {
        إذا (m1->u.کلی_Sym < m2->u.کلی_Sym)
            أرجع -1;
        إذا (m1->u.کلی_Sym > m2->u.کلی_Sym)
            أرجع +1;
    } وإلا {
        إذا (m1->u.کلی_Name < m2->u.کلی_Name)
            أرجع -1;
        إذا (m1->u.کلی_Name > m2->u.کلی_Name)
            أرجع +1;
    }

    أرجع 0;
}

ساكن صحيح
cmp_modmap_ګام_2(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت Mod_Map_ثبت *m1 = (ثابت Mod_Map_ثبت *)a;
    ثابت Mod_Map_ثبت *m2 = (ثابت Mod_Map_ثبت *)b;

    إذا (m1->mods < m2->mods)
        أرجع -1;
    إذا (m1->mods > m2->mods)
        أرجع +1;

    إذا (m1->have_نښه < m2->have_نښه)
        أرجع -1;
    إذا (m1->have_نښه > m2->have_نښه)
        أرجع +1;

    إذا (m1->have_نښه) {
        إذا (m1->u.کلی_Sym < m2->u.کلی_Sym)
            أرجع -1;
        إذا (m1->u.کلی_Sym > m2->u.کلی_Sym)
            أرجع +1;
    } وإلا {
        إذا (m1->کلی_کوډ < m2->کلی_کوډ)
            أرجع -1;
        إذا (m1->کلی_کوډ > m2->کلی_کوډ)
            أرجع +1;
    }

    أرجع 0;
}

ساكن bool
write_modmaps(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
              ثابت کلی_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, بنية buf *buf)
{
    إذا (!keymap->عدد_modmaps)
        أرجع true;

    darray(Mod_Map_ثبت) modmaps = darray_new();
    darray_from_توکي(modmaps, keymap->modmaps, keymap->عدد_modmaps);
    إذا (!darray_توکي(modmaps))
        أرجع false;

    /* Sort entries by key code */
    qsort(darray_توکي(modmaps), darray_اندازه(modmaps),
          حجم(*darray_توکي(modmaps)), &cmp_modmap_ګام_1);

    /* Drop disabled entries at the end */
    لكل (darray_اندازه_t m = darray_اندازه(modmaps); m-- > 0;) {
        إذا (darray_توکی(modmaps, m).mods)
            اكسر;
        darray_اندازه(modmaps)--;
    }

    /*
     * Build detailed keymap modmap
     *
     * NOTE: use an explicit for-loop instead of the darray_foreach macro
     * because we may add entries while iterating it. Iterate from the end,
     * so that we do not iterate the new entries.
     */
    xkb_keycode_t current_keycode = XKB_KEYCODE_INVALID;
    bool convert_منزل_v1_compatibility_ثبت = false;
    لكل (darray_اندازه_t m = darray_اندازه(modmaps); m-- > 0;) {
        Mod_Map_ثبت *mm = &darray_توکی(modmaps, m);
        أكد(mm->mods);

        إذا (mm->کلی_کوډ == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ثابت xkb_mod_ماسک_t core_mods = (mm->mods & MOD_REAL_ماسک_ALL);
            إذا (convert_منزل_v1_compatibility_ثبت && core_mods) {
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

        current_keycode = mm->کلی_کوډ;
        ثابت بنية xkb_کلی *کلی = Xkb_کلی(keymap, mm->کلی_کوډ);

        ثابت xkb_mod_ماسک_t core_mods = (کلی->modmap & MOD_REAL_ماسک_ALL);
        إذا (format == XKB_KEYMAP_FORMAT_متن_V1 &&
            ((کلی->modmap & (کلی->modmap - 1)) || کلی->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_منزل_v1_compatibility_ثبت = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_ماسک,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    کلی_Name_متن(keymap->ctx, کلی->name), کلی->modmap);
        } وإلا {
            /* V2 format or V1 format with a single core modifier */
            أكد(کلی->modmap);
            mm->mods = کلی->modmap;
            /* Force key name */
            mm->have_نښه = false;
            mm->u.کلی_Name = کلی->name;
            convert_منزل_v1_compatibility_ثبت = false;
        }
    }

    إذا (darray_تشه(modmaps))
        اذهب وتل;

    /* Sort entries by modifier mask */
    qsort(darray_توکي(modmaps), darray_اندازه(modmaps),
          حجم(*darray_توکي(modmaps)), &cmp_modmap_ګام_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_ماسک_t current_mods = 0;
    ثابت Mod_Map_ثبت *ثبت;
    darray_foreach(ثبت, modmaps) {
        إذا (!ثبت->mods)
            استمر;

        إذا (current_mods != ثبت->mods) {
            إذا (current_mods) {
                copy_منزل_buf(buf, " };\n");
            }
            current_mods = ثبت->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      Mod_ماسک_متن(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } وإلا {
            copy_منزل_buf(buf, ", ");
        }

        إذا (ثبت->have_نښه) {
            إذا (pretty || ثبت->u.کلی_Sym == XKB_کلی_No_نښه)
                write_buf(buf, "%s", Keysym_متن(keymap->ctx, ثبت->u.کلی_Sym));
            وإلا
                write_buf(buf, "0x%"PRIx32, ثبت->u.کلی_Sym);
        } وإلا {
            ثابت xkb_atom_t name = (substitutions == عدم)
                ? ثبت->u.کلی_Name
                : substitute_name(substitutions, ثبت->u.کلی_Name);
            write_buf(buf, "%s", کلی_Name_متن(keymap->ctx, name));
        }
    }
    إذا (current_mods) {
        copy_منزل_buf(buf, " };\n");
    }

وتل:
    darray_free(modmaps);
    أرجع true;
}

ساكن bool
write_symbols(ثابت بنية xkb_keymap *keymap,
              ثابت بنية xkb_keymap_serialize_config *config,
              ثابت کلی_name_substitutions *substitutions,
              xkb_layout_شاخص_t max_groups, bool some_interp,
              بنية buf *buf)
{
    ثابت bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ارزښتونه);
    ثابت bool explicit_کلی_ارزښتونه =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_کلی_ارزښتونه);
    ثابت bool drop_ناکارول_شوی = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_ناکارول_شوی);
    ثابت bool drop_interprets = (explicit_کلی_ارزښتونه && drop_ناکارول_شوی);
    /* TODO: print all default values if explicit flag is set */

    إذا (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    وإلا
        copy_منزل_buf(buf, "xkb_symbols {\n");

    bool has_ډله_نومونه = false;
    xkb_layout_شاخص_t new_ډله = (xkb_layout_شاخص_t)-1;
    لكل (xkb_layout_شاخص_t ډله = 0; ډله < keymap->عدد_ډله_نومونه; ډله++) {
        إذا (!((UINT32_C(1) << ډله) & config->layouts))
            استمر;

        new_ډله++;

        إذا (keymap->ډله_نومونه[ډله]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_ډله + 1);
            write_buf_string_literal(
                buf, xkb_atom_متن(keymap->ctx, keymap->ډله_نومونه[ډله]));
            copy_منزل_buf(buf, ";\n");
            has_ډله_نومونه = true;
        }
    }
    إذا (has_ډله_نومونه)
        copy_منزل_buf(buf, "\n");

    إذا (defaults && !write_actions_defaults(keymap, config->format, buf))
        أرجع false;

    بنية buf buf2 = { عدم, 0, 0 };
    ثابت بنية xkb_کلی *کلی;
    xkb_کلیګانې_foreach(کلی, keymap) {
        /* Skip keys with no explicit values */
        إذا (کلی->explicit &&
            !write_کلی(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_کلی_ارزښتونه, pretty, buf,
                       &buf2, کلی))
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
             بنية buf *buf, بنية xkb_keymap_serialize_پایله *پایله)
{
    کلی_name_substitutions substitutions = darray_new();
    إذا (config->format == XKB_KEYMAP_FORMAT_متن_V1 &&
        !rename_long_کلیګانې(keymap, &substitutions))
    {
        أرجع false;
    }

    ثابت کلی_name_substitutions * ثابت substitutions_ptr =
        (darray_تشه(substitutions)) ? عدم : &substitutions;

    ثابت xkb_layout_شاخص_t max_groups = format_max_groups(config->format);

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
        بنية xkb_keymap_serialize_پایله *پایله
)
{
    بنية buf buf = { عدم, 0, 0 };

    ثابت تعداد xkb_حالت حالت =
        write_keymap(keymap, config, &buf, پایله);
    إذا (حالت != XKB_SUCCESS) {
        حرر(buf.buf);
        پایله->serialized = عدم;
        أرجع حالت;
    }

    پایله->serialized = buf.buf;
    پایله->اوږدتیا = buf.اندازه + 1;
    پایله->layouts = config->layouts;

    أرجع XKB_SUCCESS;
}
