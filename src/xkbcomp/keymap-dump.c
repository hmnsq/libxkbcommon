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

#অন্তর্ভাৱ "config.h"

#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>

#অন্তর্ভাৱ "xkbcommon/xkbcommon-status.h"
#অন্তর্ভাৱ "xkbcommon/xkbcommon-keysyms.h"
#অন্তর্ভাৱ "xkbcommon/xkbcommon.h"
#অন্তর্ভাৱ "atom.h"
#অন্তর্ভাৱ "action.h"
#অন্তর্ভাৱ "context.h"
#অন্তর্ভাৱ "darray.h"
#অন্তর্ভাৱ "keymap.h"
#অন্তর্ভাৱ "messages-codes.h"
#অন্তর্ভাৱ "text.h"
#অন্তর্ভাৱ "util-numbers.h"
#অন্তর্ভাৱ "xkbcomp-priv.h"

#define BUF_CHUNK_SIZE 4096

সংরচনা buf {
    ৱর্ণ *buf;
    size_t size;
    size_t alloc;
};

#define xkb_abs(n) _Generic((n),                \
                            পূর্ণাঙ্ক: abs,           \
                            long পূর্ণাঙ্ক: labs,     \
                            স্ৱভাৱ: llabs )((n))

স্থির bool
do_realloc(সংরচনা buf *buf, size_t at_least)
{
    buf->alloc += BUF_CHUNK_SIZE;
    যদি (at_least >= BUF_CHUNK_SIZE)
        buf->alloc += at_least;

    ৱর্ণ *ধ্রুৱ new = realloc(buf->buf, buf->alloc);
    যদি (!new)
        ঘূৰাই false;

    buf->buf = new;
    ঘূৰাই true;
}

ATTR_PRINTF(2, 3) স্থির bool
check_write_buf(সংরচনা buf *buf, ধ্রুৱ ৱর্ণ *fmt, ...)
{
    va_list args;
    পূর্ণাঙ্ক printed;
    size_t available;

    available = buf->alloc - buf->size;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    প্রতিজ্ঞা(buf->buf || available == 0);
    va_start(args, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->size : buf->buf, available,
                        fmt, args);
    va_end(args);

    যদি (printed < 0) {
        /* Some error */
        গচ্ছ err;
    } অন্যথা যদি ((size_t)printed >= available) {
        /* Not enough space: need to realloc */
        যদি (!do_realloc(buf, (size_t)printed + 1))
            গচ্ছ err;

        /* The buffer has enough space now. */
        available = buf->alloc - buf->size;
        প্রতিজ্ঞা(buf->buf && (size_t) printed < available);
        va_start(args, fmt);
        printed = vsnprintf(buf->buf + buf->size, available, fmt, args);
        va_end(args);

        যদি (printed < 0 || (size_t) printed >= available)
            গচ্ছ err;
    }

    buf->size += (size_t)printed;
    ঘূৰাই true;

err:
    মুক্ত(buf->buf);
    buf->buf = রিক্ত;
    ঘূৰাই false;
}

#define write_buf(buf, ...) কৰা { \
    যদি (!check_write_buf(buf, __VA_ARGS__)) \
        ঘূৰাই false; \
} যেতিয়ালৈ (0)

স্থির bool
check_copy_to_buf(সংরচনা buf *buf, ধ্রুৱ ৱর্ণ* source, size_t len)
{
    যদি (len == 0)
        ঘূৰাই true;

    ধ্রুৱ size_t available = buf->alloc - buf->size;
    যদি (len >= available &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        মুক্ত(buf->buf);
        buf->buf = রিক্ত;
        ঘূৰাই false;
    }

    memcpy(buf->buf + buf->size, source, len);
    buf->size += len;
    /* Append NULL byte */
    buf->buf[buf->size] = '\0';
    ঘূৰাই true;
}

#define copy_to_buf_len(buf, source, len) কৰা { \
    যদি (!check_copy_to_buf(buf, source, len)) \
        ঘূৰাই false; \
} যেতিয়ালৈ (0)

#define copy_to_buf(buf, source) \
    copy_to_buf_len(buf, source, পরিমাণ(source) - 1)

স্থির bool
check_write_string_literal(সংরচনা buf *buf, ধ্রুৱ ৱর্ণ* string)
{
    copy_to_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    size_t pending_start = 0;
    size_t current = 0;
    ৱর্ণ escape_buffer[] = {'\\', '0', '0', '0', '0'};
    বাবে (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        ৱিকল্প (string[current]) {
        পক্ষ '\n':
            /* `\n` would break strings */
            escape_buffer[1] = 'n';
            escape_len = 2;
            ভঙ্গ;
        পক্ষ '\\':
            /* `\` would create escape sequences */
            escape_buffer[1] = '\\';
            escape_len = 2;
            ভঙ্গ;
        স্ৱভাৱ:
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
            যদি (unlikely((unsigned ৱর্ণ)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_buffer[1] = '0';
                escape_buffer[2] = '0';
                escape_buffer[3] = (ৱর্ণ)('0' + (string[current] >> 3));
                escape_buffer[4] = (ৱর্ণ)('0' + (string[current] & 0x7));
                escape_len = 5;
            } অন্যথা {
                /* Expand the chunk */
                অনুৱর্ত;
            }
        }
        /* Write pending chunk */
        প্রতিজ্ঞা(current >= pending_start);
        copy_to_buf_len(buf, string + pending_start, current - pending_start);
        /* Write escape sequence */
        copy_to_buf_len(buf, escape_buffer, escape_len);
        pending_start = current + 1;
    }
    /* Write remaining chunk */
    প্রতিজ্ঞা(current >= pending_start);
    copy_to_buf_len(buf, string + pending_start, current - pending_start);

    copy_to_buf(buf, "\"");
    ঘূৰাই true;
}

#define write_buf_string_literal(buf, string) কৰা { \
    যদি (!check_write_string_literal(buf, string)) \
        ঘূৰাই false; \
} যেতিয়ালৈ (0)

স্থির শূন্য
delete_last_char(সংরচনা buf *buf, ৱর্ণ c)
{
    যদি (buf->size && buf->buf[buf->size - 1] == c)
        buf->buf[--buf->size] = '\0';
}

/** Entry for a name substituion LUT */
সংরচনা চাবি_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
স্থির পূর্ণাঙ্ক
cmp_চাবি_name_substitution(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
    ধ্রুৱ xkb_atom_t s1 = ((সংরচনা চাবি_name_substitution *) a)->source;
    ধ্রুৱ xkb_atom_t s2 = ((সংরচনা চাবি_name_substitution *) b)->source;
    ঘূৰাই (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

প্রকারনির্দেশ darray(সংরচনা চাবি_name_substitution) চাবি_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
স্থির bool
rename_long_চাবিগুলি(ধ্রুৱ সংরচনা xkb_keymap *keymap,
                 চাবি_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    গণনা {
        first = '!',
        last = '0',
        invalid = '\0'
    };
    ৱর্ণ prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', last
    };
    static_assert(পরিমাণ(prefixes) == last - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ধ্রুৱ সংরচনা xkb_চাবি *চাবি;
    xkb_চাবিগুলি_foreach(চাবি, keymap) {
        যদি (চাবি->name == XKB_ATOM_NONE)
            অনুৱর্ত;

        ধ্রুৱ ৱর্ণ * ধ্রুৱ name = xkb_atom_text(keymap->ctx, চাবি->name);
        ধ্রুৱ size_t len = strlen_safe(name);
        যদি (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (সংরচনা চাবি_name_substitution) {
                .source = চাবি->name,
                .keycode = চাবি->keycode,
                .target = চাবি->name
            });
        } অন্যথা যদি (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    বাবে (darray_size_t a = 0; a < keymap->অঙ্ক_চাবি_aliases; a++) {
        ধ্রুৱ সংরচনা xkb_চাবি_alias * ধ্রুৱ alias = &keymap->চাবি_aliases[a];
        ধ্রুৱ ৱর্ণ * ধ্রুৱ name = xkb_atom_text(keymap->ctx, alias->alias);
        ধ্রুৱ size_t len = strlen_safe(name);
        যদি (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (সংরচনা চাবি_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } অন্যথা যদি (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    যদি darray_empty(*substitutions) {
        /* Nothing to do */
        ঘূৰাই true;
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
    ৱর্ণ *prefix = prefixes;
    ধ্রুৱ ৱর্ণ *prefix_max = prefixes + পরিমাণ(prefixes) - 1;
    যেতিয়ালৈ (*prefix == invalid && prefix < prefix_max)
        prefix++;

    /* Keycode prefix: prefer ‘0’ if available */
    ৱর্ণ keycode_prefix;
    যদি (prefixes[last - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_prefix = last;
        prefixes[last - first] = invalid;
    } অন্যথা {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_prefix = *prefix;
        যদি (unlikely(keycode_prefix == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            ঘূৰাই true;
        }
        /* Update current prefix */
        *prefix = invalid;
        যেতিয়ালৈ (*prefix == invalid && prefix < prefix_max)
            prefix++;
    }

    /* Proceed renaming */
    uint16_t next_id = 1;
    সংরচনা চাবি_name_substitution *sub;
    ৱর্ণ buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        যদি (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, পরিমাণ(buf), "%c%03"PRIx32,
                     keycode_prefix, sub->keycode);
            ধ্রুৱ xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, পরিমাণ(buf) - 2);
            যদি (target == XKB_ATOM_NONE)
                গচ্ছ error;
            sub->target = target;
        } অন্যথা যদি (*prefix != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, পরিমাণ(buf), "%c%03"PRIx16, *prefix, next_id);
            ধ্রুৱ xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, পরিমাণ(buf) - 2);
            যদি (target == XKB_ATOM_NONE)
                গচ্ছ error;
            sub->target = target;
            যদি (++next_id > 0xfff) {
                *prefix = invalid;
                /* Get next available prefix */
                যেতিয়ালৈ (*prefix == invalid && prefix < prefix_max)
                    prefix++;
                /* Reset counter */
                next_id = 1;
            }
        } অন্যথা {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->target = sub->source;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_বস্তুসমূহ(*substitutions), darray_size(*substitutions),
          পরিমাণ(*darray_বস্তুসমূহ(*substitutions)), &cmp_চাবি_name_substitution);

    ঘূৰাই true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate key name substitution\n");
    ঘূৰাই false;
}

/** Substitute a key name with a valid X11 key name */
স্থির xkb_atom_t
substitute_name(ধ্রুৱ চাবি_name_substitutions *substitutions, xkb_atom_t name)
{
    /*দ্বিমিক অনুসন্ধান*/
    darray_size_t first = 0;
    darray_size_t last = darray_size(*substitutions);

    যেতিয়ালৈ (last > first) {
        darray_size_t mid = first + (last - 1 - first) / 2;
        ধ্রুৱ xkb_atom_t source = darray_item(*substitutions, mid).source;
        যদি (source < name)
            first = mid + 1;
        অন্যথা যদি (source > name)
            last = mid;
        অন্যথা /* found it */
            ঘূৰাই darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    ঘূৰাই name;
}

স্থির bool
write_vmods(ধ্রুৱ সংরচনা xkb_keymap *keymap, গণনা xkb_keymap_format format,
            bool explicit, সংরচনা buf *buf)
{
    ধ্রুৱ সংরচনা xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        যদি (!has_some) {
            copy_to_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } অন্যথা {
            copy_to_buf(buf, ",");
        }
        write_buf(buf, "%s", xkb_atom_text(keymap->ctx, mod->name));

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
        ধ্রুৱ bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        যদি ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
             explicit) &&
            (auto_canonical_mods || mod->mapping != 0)) {
            /*
             * Explicit non-default mapping
             * NOTE: we can only pretty-print *real* modifiers in this context.
             */
            write_buf(buf, "=%s",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  mod->mapping));
        }
    }

    যদি (has_some)
        copy_to_buf(buf, ";\n\n");

    ঘূৰাই true;
}

স্থির bool
write_keycodes(ধ্রুৱ সংরচনা xkb_keymap *keymap,
               ধ্রুৱ চাবি_name_substitutions *substitutions,
               গণনা xkb_keymap_serialize_flags flags, সংরচনা buf *buf)
{
    ধ্রুৱ সংরচনা xkb_চাবি *চাবি;
    xkb_led_index_t idx;
    ধ্রুৱ সংরচনা xkb_led *led;

    ধ্রুৱ bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    যদি (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    অন্যথা
        copy_to_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_চাবি_code, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_চাবি_code, 255));

    xkb_চাবিগুলি_foreach(চাবি, keymap) {
        যদি (চাবি->name == XKB_ATOM_NONE)
            অনুৱর্ত;

        ধ্রুৱ xkb_atom_t name = (substitutions == রিক্ত)
            ? চাবি->name
            : substitute_name(substitutions, চাবি->name);
        যদি (pretty)
            write_buf(buf, "\t%-20s", চাবি_Name_Text(keymap->ctx, name));
        অন্যথা
            write_buf(buf, "\t%s", চাবি_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", চাবি->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        যদি (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_to_buf(buf, ";\n");
        }

    বাবে (darray_size_t i = 0; i < keymap->অঙ্ক_চাবি_aliases; i++) {
        ধ্রুৱ xkb_atom_t alias = (substitutions == রিক্ত)
            ? keymap->চাবি_aliases[i].alias
            : substitute_name(substitutions, keymap->চাবি_aliases[i].alias);
        ধ্রুৱ xkb_atom_t real = (substitutions == রিক্ত)
            ? keymap->চাবি_aliases[i].real
            : substitute_name(substitutions, keymap->চাবি_aliases[i].real);

        যদি (pretty)
            write_buf(buf, "\talias %-14s",
                      চাবি_Name_Text(keymap->ctx, alias));
        অন্যথা
            write_buf(buf, "\talias %s",
                      চাবি_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  চাবি_Name_Text(keymap->ctx, real));
    }

    copy_to_buf(buf, "};\n\n");
    ঘূৰাই true;
}

স্থির bool
write_types(ধ্রুৱ সংরচনা xkb_keymap *keymap, গণনা xkb_keymap_format format,
            গণনা xkb_keymap_serialize_flags flags, সংরচনা buf *buf)
{
    যদি (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    অন্যথা
        copy_to_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    যদি (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        ঘূৰাই false;

    ধ্রুৱ bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    বাবে (darray_size_t i = 0; i < keymap->অঙ্ক_types; i++) {
        ধ্রুৱ সংরচনা xkb_চাবি_type * ধ্রুৱ type = &keymap->types[i];
        যদি (!type->required && drop_unused)
            অনুৱর্ত;

        copy_to_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_to_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        বাবে (darray_size_t j = 0; j < type->অঙ্ক_entries; j++) {
            ধ্রুৱ ৱর্ণ *str;
            ধ্রুৱ সংরচনা xkb_চাবি_type_entry *entry = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            যদি (entry->level == 0 && entry->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_মানসমূহ))
                অনুৱর্ত;

            str = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              entry->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, entry->level + 1);

            যদি (entry->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           entry->preserve.mods));
        }

        বাবে (xkb_level_index_t n = 0; n < type->অঙ্ক_level_names; n++)
            যদি (type->level_names[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_names[n]));
                copy_to_buf(buf, ";\n");
            }

        copy_to_buf(buf, "\t};\n");
    }

    copy_to_buf(buf, "};\n\n");
    ঘূৰাই true;
}

স্থির bool
write_led_map(ধ্রুৱ সংরচনা xkb_keymap *keymap, গণনা xkb_keymap_format format,
              bool defaults, সংরচনা buf *buf, ধ্রুৱ সংরচনা xkb_led *led)
{
    copy_to_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_to_buf(buf, " {\n");

    যদি (led->which_groups) {
        যদি (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      LedStateMaskText(keymap->ctx, groupComponentMaskNames,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    যদি (led->which_mods) {
        যদি (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      LedStateMaskText(keymap->ctx, modComponentMaskNames,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    যদি (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  ControlMaskText(keymap->ctx, format, led->ctrls));
    }

    copy_to_buf(buf, "\t};\n");
    ঘূৰাই true;
}

স্থির ধ্রুৱ ৱর্ণ *
affect_lock_text(গণনা xkb_action_flags flags, bool show_both)
{
    ৱিকল্প (flags & (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK)) {
    পক্ষ 0:
        ঘূৰাই show_both ? ",affect=both" : "";
    পক্ষ ACTION_LOCK_NO_UNLOCK:
        ঘূৰাই ",affect=lock";
    পক্ষ ACTION_LOCK_NO_LOCK:
        ঘূৰাই ",affect=unlock";
    পক্ষ ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK:
        ঘূৰাই ",affect=neither";
    স্ৱভাৱ:
        ঘূৰাই "";
    }
}

#define প্ৰতীক_PADDING 15
#define ACTION_PADDING 30

স্থির bool
write_action(ধ্রুৱ সংরচনা xkb_keymap *keymap, গণনা xkb_keymap_format format,
             xkb_layout_index_t max_groups, সংরচনা buf *buf,
             ধ্রুৱ সঙ্ঘ xkb_action *action,
             ধ্রুৱ ৱর্ণ *prefix, ধ্রুৱ ৱর্ণ *suffix)
{
    ধ্রুৱ ৱর্ণ *type;
    ধ্রুৱ ৱর্ণ *args = রিক্ত;

    যদি (!prefix)
        prefix = "";
    যদি (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    ৱিকল্প (action->type) {
    পক্ষ ACTION_TYPE_MOD_SET:
    পক্ষ ACTION_TYPE_MOD_LATCH:
    পক্ষ ACTION_TYPE_MOD_LOCK:
        যদি (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            args = "modMapMods";
        অন্যথা
            args = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        যদি (unlockOnPress && !isModsUnLockOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->group.flags & ACTION_LATCH_ON_PRESS);
        যদি (latchOnPress && !isModsLatchOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"LatchMods(latchOnPress=true)\" "
                    "in keymap format %d\n", format);
            latchOnPress = false;
        }
        write_buf(buf, "%s%s(modifiers=%s%s%s%s%s%s)%s", prefix, type, args,
                  (action->type != ACTION_TYPE_MOD_LOCK && (action->mods.flags & ACTION_LOCK_CLEAR)) ? ",clearLocks" : "",
                  (action->type == ACTION_TYPE_MOD_LATCH && (action->mods.flags & ACTION_LATCH_TO_LOCK)) ? ",latchToLock" : "",
                  (action->type == ACTION_TYPE_MOD_LOCK) ? affect_lock_text(action->mods.flags, false) : "",
                  (unlockOnPress) ? ",unlockOnPress" : "",
                  (latchOnPress) ? ",latchOnPress" : "",
                  suffix);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_GROUP_SET:
    পক্ষ ACTION_TYPE_GROUP_LATCH:
    পক্ষ ACTION_TYPE_GROUP_LOCK:
        যদি ((uint32_t) xkb_abs(action->group.group) <
            (max_groups + !(action->group.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool lockOnRelease = action->type == ACTION_TYPE_GROUP_LOCK &&
                                 (action->group.flags & ACTION_LOCK_ON_RELEASE);
            যদি (lockOnRelease && !isGroupLockOnReleaseSupported(format)) {
                log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                        "Cannot use \"GroupLock(lockOnRelease=true)\" "
                        "in keymap format %d\n", format);
                lockOnRelease = false;
            }
            write_buf(buf, "%s%s(group=%s%"PRId32"%s%s%s)%s", prefix, type,
                      (!(action->group.flags & ACTION_ABSOLUTE_SWITCH) && action->group.group >= 0) ? "+" : "",
                      (action->group.flags & ACTION_ABSOLUTE_SWITCH) ? action->group.group + 1 : action->group.group,
                      (action->type != ACTION_TYPE_GROUP_LOCK && (action->group.flags & ACTION_LOCK_CLEAR)) ? ",clearLocks" : "",
                      (action->type == ACTION_TYPE_GROUP_LATCH && (action->group.flags & ACTION_LATCH_TO_LOCK)) ? ",latchToLock" : "",
                      (lockOnRelease) ? ",lockOnRelease" : "",
                      suffix);
        } অন্যথা {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->group.group +
                       !!(action->group.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            গচ্ছ void_action;
        }
        ভঙ্গ;

    পক্ষ ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", prefix, type, suffix);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", prefix, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_PTR_LOCK:
        args = affect_lock_text(action->btn.flags, true);
        /* fallthrough */
    পক্ষ ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", prefix, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        যদি (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        অন্যথা
            copy_to_buf(buf, "default");
        যদি (action->btn.গুনতি) {
            প্রতিজ্ঞা(action->type != ACTION_TYPE_PTR_LOCK);
            write_buf(buf, ",count=%u", action->btn.গুনতি);
        }
        যদি (args)
            write_buf(buf, "%s", args);
        write_buf(buf, ")%s", suffix);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", prefix, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.মান >= 0) ? "+" : "",
                  action->dflt.মান);
        write_buf(buf, ")%s", suffix);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", prefix, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_CTRL_SET:
    পক্ষ ACTION_TYPE_CTRL_LOCK:
        write_buf(buf, "%s%s(controls=%s%s)%s", prefix, type,
                  ControlMaskText(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_LOCK
                    ? affect_lock_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_REDIRECT_চাবি: {
        write_buf(buf, "%s%s(", prefix, type);
        ধ্রুৱ সংরচনা xkb_চাবি * ধ্রুৱ চাবি = Xkb_চাবি(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        যদি (চাবি)
            write_buf(buf, "keycode=%s", চাবি_Name_Text(keymap->ctx, চাবি->name));
        যদি (action->redirect.affect_mods) {
            xkb_mod_mask_t mask;
            mask = (action->redirect.affect_mods & action->redirect.mods);
            যদি (mask)
                write_buf(buf, ",modifiers=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
            mask = (action->redirect.affect_mods & ~action->redirect.mods);
            যদি (mask)
                write_buf(buf, ",clearMods=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
        }
        write_buf(buf, ")%s", suffix);
        ভঙ্গ;
    }

    পক্ষ ACTION_TYPE_UNKNOWN:
    পক্ষ ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", prefix, suffix);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        যদি (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      prefix, suffix);
        অন্যথা
            write_buf(buf, "%sVoidAction()%s", prefix, suffix);
        ভঙ্গ;

    স্ৱভাৱ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_অঙ্ক_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        প্রতিজ্ঞা(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  prefix, type, action->type, action->priv.data[0],
                  action->priv.data[1], action->priv.data[2],
                  action->priv.data[3], action->priv.data[4],
                  action->priv.data[5], action->priv.data[6],
                  suffix);
        ভঙ্গ;
    }

    ঘূৰাই true;
}

স্থির ধ্রুৱ সঙ্ঘ xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

স্থির bool
write_actions(ধ্রুৱ সংরচনা xkb_keymap *keymap, গণনা xkb_keymap_format format,
              xkb_layout_index_t max_groups, সংরচনা buf *buf, সংরচনা buf *buf2,
              ধ্রুৱ সংরচনা xkb_চাবি *চাবি, xkb_layout_index_t group)
{
    স্থির ধ্রুৱ সঙ্ঘ xkb_action noAction = { .type = ACTION_TYPE_NONE };

    বাবে (xkb_level_index_t level = 0; level < Xkb_চাবি_অঙ্ক_Levels(চাবি, group);
         level++) {

        যদি (level != 0)
            copy_to_buf(buf, ", ");

        ধ্রুৱ সঙ্ঘ xkb_action *actions = রিক্ত;
        xkb_action_গুনতি_t গুনতি = xkb_keymap_চাবি_get_actions_by_level(
            keymap, চাবি, group, level, &actions
        );

        যদি (গুনতি > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            গুনতি = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->size = 0;
        যদি (গুনতি == 0) {
            যদি (!write_action(keymap, format, max_groups,
                              buf2, &noAction, রিক্ত, রিক্ত))
                ঘূৰাই false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        অন্যথা যদি (গুনতি == 1) {
            যদি (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), রিক্ত, রিক্ত))
                ঘূৰাই false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        অন্যথা {
            copy_to_buf(buf2, "{ ");
            বাবে (xkb_action_গুনতি_t k = 0; k < গুনতি; k++) {
                যদি (k != 0)
                    copy_to_buf(buf2, ", ");
                size_t old_size = buf2->size;
                যদি (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), রিক্ত, রিক্ত))
                    ঘূৰাই false;
                /* Check if padding is necessary */
                যদি (buf2->size >= old_size + ACTION_PADDING)
                    অনুৱর্ত;
                /* Compute and write padding, then write the action again */
                ধ্রুৱ পূর্ণাঙ্ক padding = (পূর্ণাঙ্ক)(old_size + ACTION_PADDING - buf2->size);
                buf2->size = old_size;
                write_buf(buf2, "%*s", padding, "");
                যদি (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), রিক্ত, রিক্ত))
                    ঘূৰাই false;
            }
            copy_to_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    ঘূৰাই true;
}

স্থির bool
write_action_defaults(ধ্রুৱ সংরচনা xkb_keymap *keymap,
                      গণনা xkb_keymap_format format,
                      ধ্রুৱ সঙ্ঘ xkb_action *action,
                      সংরচনা buf *buf)
{
    ধ্রুৱ ৱর্ণ *type = ActionTypeText(action->type);

#define PREFIX "\t"

    ৱিকল্প (action->type) {
    পক্ষ ACTION_TYPE_MOD_SET:
    পক্ষ ACTION_TYPE_MOD_LATCH:
    পক্ষ ACTION_TYPE_MOD_LOCK:
        প্রতিজ্ঞা(action->mods.flags == 0);
        প্রতিজ্ঞা(action->mods.mods.mods == 0);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        যদি (action->type != ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        যদি (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, PREFIX"%s.latchOnPress = false;\n", type);
        যদি (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        যদি (action->type == ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        যদি (isModsUnLockOnPressSupported(format))
            write_buf(buf, PREFIX"%s.unlockOnPress = false;\n", type);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_GROUP_SET:
    পক্ষ ACTION_TYPE_GROUP_LATCH:
    পক্ষ ACTION_TYPE_GROUP_LOCK:
        প্রতিজ্ঞা(action->group.flags == 0);
        প্রতিজ্ঞা(action->group.group == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.group = +0;\n", type);
        যদি (action->type != ACTION_TYPE_GROUP_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        যদি (action->type == ACTION_TYPE_GROUP_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        যদি (action->type == ACTION_TYPE_GROUP_LOCK &&
            isGroupLockOnReleaseSupported(format))
            write_buf(buf, PREFIX"%s.lockOnRelease = false;\n", type);
        ভঙ্গ;


    পক্ষ ACTION_TYPE_PTR_MOVE:
        প্রতিজ্ঞা(action->ptr.x == 0);
        প্রতিজ্ঞা(action->ptr.y == 0);
        প্রতিজ্ঞা(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.x = +0;\n", type);
        write_buf(buf, PREFIX"%s.y = +0;\n", type);
        write_buf(buf, PREFIX"%s.accel = true;\n", type);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_PTR_LOCK:
    পক্ষ ACTION_TYPE_PTR_BUTTON:
        প্রতিজ্ঞা(action->btn.flags == 0);
        প্রতিজ্ঞা(action->btn.button == 0);
        প্রতিজ্ঞা(action->btn.গুনতি == 0);
        write_buf(buf, PREFIX"%s.button = 0;\n", type);
        write_buf(buf, PREFIX"%s.count = 0;\n", type);
        যদি (action->type == ACTION_TYPE_PTR_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_PTR_DEFAULT:
        প্রতিজ্ঞা(action->dflt.flags == 0);
        প্রতিজ্ঞা(action->dflt.মান == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.button = +1;\n", type);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_SWITCH_VT:
        প্রতিজ্ঞা(action->screen.flags == ACTION_SAME_SCREEN);
        প্রতিজ্ঞা(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.screen = +0;\n", type);
        write_buf(buf, PREFIX"%s.same = true;\n", type);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_CTRL_SET:
    পক্ষ ACTION_TYPE_CTRL_LOCK:
        প্রতিজ্ঞা(action->ctrls.flags == 0);
        প্রতিজ্ঞা(action->ctrls.ctrls == 0);
        write_buf(buf, PREFIX"%s.controls = 0;\n", type);
        যদি (action->type == ACTION_TYPE_CTRL_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        ভঙ্গ;

    পক্ষ ACTION_TYPE_REDIRECT_চাবি:
        প্রতিজ্ঞা(action->redirect.keycode == keymap->redirect_চাবি_auto);
        প্রতিজ্ঞা(action->redirect.affect_mods == 0);
        প্রতিজ্ঞা(action->redirect.mods == 0);
        write_buf(buf, PREFIX"%s.keycode = auto;\n", type);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        write_buf(buf, PREFIX"%s.clearMods = 0;\n", type);
        ভঙ্গ;

    স্ৱভাৱ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_অঙ্ক_ENTRIES,
                      "Missing action type");
    }

#undef PREFIX

    ঘূৰাই true;
}

স্থির bool
write_actions_defaults(ধ্রুৱ সংরচনা xkb_keymap *keymap,
                       গণনা xkb_keymap_format format, সংরচনা buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    বাবে (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_অঙ্ক_ENTRIES; a++) {
        ধ্রুৱ সঙ্ঘ xkb_action * restrict ধ্রুৱ action = &defaults.actions[a];
        যদি (!write_action_defaults(keymap, format, action, buf))
            ঘূৰাই false;
    }
    copy_to_buf(buf, "\n");

    ঘূৰাই true;
}

স্থির ধ্রুৱ সংরচনা xkb_sym_interpret fallback_interpret = {
    .sym = XKB_চাবি_Void_প্ৰতীক,
    .required = true,
    .repeat = FALLBACK_INTERPRET_চাবি_REPEAT,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .অঙ্ক_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

স্থির bool
write_compat(ধ্রুৱ সংরচনা xkb_keymap * restrict keymap,
             গণনা xkb_keymap_format format, xkb_layout_index_t max_groups,
             গণনা xkb_keymap_serialize_flags flags, bool * restrict some_interp,
             সংরচনা buf *buf)
{
    ধ্রুৱ bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ধ্রুৱ bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_মানসমূহ);
    ধ্রুৱ bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ধ্রুৱ bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_চাবি_মানসমূহ) && drop_unused);
    /* TODO: print all default values if the explicit flag is set */

    যদি (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    অন্যথা
        copy_to_buf(buf, "xkb_compatibility {\n");

    যদি (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        ঘূৰাই false;

    যদি (defaults && !write_actions_defaults(keymap, format, buf))
        ঘূৰাই false;

    copy_to_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_to_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    ধ্রুৱ bool use_fallback_interpret =
        (!keymap->অঙ্ক_sym_interprets || drop_interprets);
    ধ্রুৱ darray_size_t অঙ্ক_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->অঙ্ক_sym_interprets);
    ধ্রুৱ সংরচনা xkb_sym_interpret* ধ্রুৱ sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    বাবে (darray_size_t i = 0; i < অঙ্ক_sym_interprets; i++) {
        ধ্রুৱ সংরচনা xkb_sym_interpret *si = &sym_interprets[i];
        যদি (!si->required && drop_unused)
            অনুৱর্ত;

        *some_interp = true;

        copy_to_buf(buf, "\tinterpret ");
        যদি (!si->sym) {
            copy_to_buf(buf, "Any");
        } অন্যথা যদি (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } অন্যথা {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->match),
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        যদি (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      ModIndexText(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        যদি (si->level_one_only) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } অন্যথা যদি (defaults) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        যদি (si->repeat) {
            copy_to_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } অন্যথা যদি (defaults) {
            copy_to_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        ধ্রুৱ সঙ্ঘ xkb_action *action = রিক্ত;
        xkb_action_গুনতি_t action_গুনতি = si->অঙ্ক_actions;
        যদি (action_গুনতি <= 1) {
            action = &si->a.action;
        } অন্যথা যদি (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_গুনতি = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        যদি (action_গুনতি > 1) {
            copy_to_buf(buf, "\n\t\taction= {");
            ধ্রুৱ ৱর্ণ suffix[] = ", ";
            বাবে (xkb_action_গুনতি_t k = 0; k < si->অঙ্ক_actions; k++) {
                যদি (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    ঘূৰাই false;
            }
            buf->size -= পরিমাণ(suffix) - 1; /* trailing comma */
            copy_to_buf(buf, "};");
            has_explicit_properties = true;
        } অন্যথা যদি (action_গুনতি == 1) {
            যদি (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                ঘূৰাই false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    যদি (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        অঙ্ক_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].match == fallback_interpret.match &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].অঙ্ক_actions == fallback_interpret.অঙ্ক_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    ধ্রুৱ সংরচনা xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        যদি ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            ঘূৰাই false;
        }
    }

    copy_to_buf(buf, "};\n\n");

    ঘূৰাই true;
}

স্থির bool
write_keysyms(ধ্রুৱ সংরচনা xkb_keymap *keymap,
              সংরচনা buf *buf, সংরচনা buf *buf2,
              ধ্রুৱ সংরচনা xkb_চাবি *চাবি, xkb_layout_index_t group,
              bool pretty, bool show_actions)
{
    ধ্রুৱ unsigned পূর্ণাঙ্ক padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : প্ৰতীক_PADDING)
        : 0;
    বাবে (xkb_level_index_t level = 0; level < Xkb_চাবি_অঙ্ক_Levels(চাবি, group);
         level++) {
        ধ্রুৱ xkb_keysym_t *syms;
        পূর্ণাঙ্ক অঙ্ক_syms;

        যদি (level != 0)
            copy_to_buf(buf, ", ");

        অঙ্ক_syms = xkb_keymap_চাবি_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (সংরচনা xkb_keymap *)keymap,
            চাবি->keycode, group, level, &syms
        );

        ধ্রুৱ xkb_keysym_t no_প্ৰতীক = XKB_চাবি_No_প্ৰতীক;
        যদি (অঙ্ক_syms == 0) {
            অঙ্ক_syms = 1;
            syms = &no_প্ৰতীক;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        যদি (অঙ্ক_syms == 1) {
            যদি (pretty || syms[0] == XKB_চাবি_No_প্ৰতীক)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            অন্যথা
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } অন্যথা {
            যদি (pretty) {
                buf2->size = 0;
                copy_to_buf(buf2, "{ ");
                বাবে (পূর্ণাঙ্ক s = 0; s < অঙ্ক_syms; s++) {
                    যদি (s != 0)
                        copy_to_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_to_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } অন্যথা {
                copy_to_buf(buf, "{");
                বাবে (পূর্ণাঙ্ক s = 0; s < অঙ্ক_syms; s++) {
                    যদি (s != 0)
                        copy_to_buf(buf, ",");
                    যদি (syms[s] == XKB_চাবি_No_প্ৰতীক)
                        copy_to_buf(buf, "NoSymbol");
                    অন্যথা
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_to_buf(buf, "}");
            }
        }
    }

    ঘূৰাই true;
}

স্থির bool
write_চাবি(ধ্রুৱ সংরচনা xkb_keymap *keymap,
          ধ্রুৱ সংরচনা xkb_keymap_serialize_config *config,
          ধ্রুৱ চাবি_name_substitutions *substitutions,
          xkb_layout_index_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          সংরচনা buf *buf, সংরচনা buf *buf2, ধ্রুৱ সংরচনা xkb_চাবি *চাবি)
{
    bool simple = true;

    xkb_layout_mask_t layouts = config->layouts;
    যদি (popcount32(layouts) != keymap->অঙ্ক_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_index_t l = 0;
        বাবে (xkb_layout_mask_t ls = layouts; ls; ls >>= 1, l++) {
            যদি (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ধ্রুৱ xkb_layout_index_t effective =
                    xkb_keymap_চাবি_effective_layout(চাবি, l);
                যদি (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } অন্যথা {
        layouts &= ((UINT64_C(1) << চাবি->অঙ্ক_groups) - 1);
    }
    ধ্রুৱ xkb_layout_index_t groups_গুনতি = popcount32(layouts);

    ধ্রুৱ xkb_atom_t name = (substitutions == রিক্ত)
        ? চাবি->name
        : substitute_name(substitutions, চাবি->name);

    যদি (pretty)
        write_buf(buf, "\tkey %-20s {", চাবি_Name_Text(keymap->ctx, name));
    অন্যথা
        write_buf(buf, "\tkey %s {", চাবি_Name_Text(keymap->ctx, name));

    যদি ((চাবি->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        যদি (!explicit) {
            বাবে (xkb_layout_index_t group = 1; group < চাবি->অঙ্ক_groups; group++) {
                যদি (!((UINT32_C(1) << group) & layouts))
                    অনুৱর্ত;

                যদি (চাবি->groups[group].type != চাবি->groups[0].type) {
                    multi_type = true;
                    ভঙ্গ;
                }
            }
        }

        যদি (multi_type) {
            xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
            বাবে (xkb_layout_index_t group = 0; group < চাবি->অঙ্ক_groups; group++) {
                যদি (!((UINT32_C(1) << group) & layouts))
                    অনুৱর্ত;

                new_group++;

                যদি (!চাবি->groups[group].explicit_type && !explicit)
                    অনুৱর্ত;

                ধ্রুৱ সংরচনা xkb_চাবি_type * ধ্রুৱ type = চাবি->groups[group].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_group + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_to_buf(buf, ",");
            }
        }
        অন্যথা {
            ধ্রুৱ সংরচনা xkb_চাবি_type * ধ্রুৱ type = চাবি->groups[0].type;
            write_buf(buf, "\n\t\ttype= ");
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, type->name));
            copy_to_buf(buf, ",");
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
    ধ্রুৱ bool explicit_actions = (চাবি->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #define require_explicit(চাবি, prop,                                       \
                             মান, initial_মান, default_interpret_মান) ( \
        /* Explicit value in the source */                                    \
        ((চাবি)->explicit & (prop)) ||                                         \
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
                ((চাবি)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (মান) != (initial_মান) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_মান) != (default_interpret_মান)                  \
            )                                                                 \
        )                                                                     \
    )

    যদি (require_explicit(চাবি, EXPLICIT_REPEAT, চাবি->repeats,
                         DEFAULT_চাবি_REPEAT, DEFAULT_INTERPRET_চাবি_REPEAT)) {
        যদি (চাবি->repeats)
            copy_to_buf(buf, "\n\t\trepeat= Yes,");
        অন্যথা
            copy_to_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    যদি (require_explicit(চাবি, EXPLICIT_VMODMAP, চাবি->vmodmap,
                         (xkb_mod_mask_t) DEFAULT_চাবি_VMODMAP,
                         (xkb_mod_mask_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              চাবি->vmodmap));
        simple = false;
    }

    #undef require_explicit

    ৱিকল্প (চাবি->out_of_range_group_policy) {
    পক্ষ XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        copy_to_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        ভঙ্গ;

    পক্ষ XKB_LAYOUT_OUT_OF_RANGE_REDIRECT: {
        যদি (!groups_গুনতি) {
            /* No layout to redirect to: skip */
            ভঙ্গ;
        }

        xkb_layout_mask_t layout_mask =
            (UINT32_C(1) << চাবি->out_of_range_group_সংখ্যা);
        যদি (!(layouts & layout_mask)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     চাবি_Name_Text(keymap->ctx, name),
                     চাবি->out_of_range_group_সংখ্যা + 1);
            layout_mask = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ধ্রুৱ xkb_layout_mask_t নিচেৰ = (layouts & (layout_mask - 1));
        ধ্রুৱ xkb_layout_index_t new = popcount32(নিচেৰ);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        ভঙ্গ;
    }

    পক্ষ XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    স্ৱভাৱ:
        ভঙ্গ;
    }

    যদি (চাবি->overlays) {
        xkb_overlay_mask_t remaining = চাবি->overlays;

        ধ্রুৱ xkb_overlay_index_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_গুনতি == 8, "invalid right shift");
        ধ্রুৱ xkb_overlay_mask_t valid =
            (xkb_overlay_mask_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        যদি (remaining != চাবি->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, চাবি_Name_Text(keymap->ctx, name),
                     remaining, চাবি->overlays & ~valid);
        }

        যদি (remaining && !চাবি->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ধ্রুৱ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            ধ্রুৱ xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     চাবি_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        যদি (remaining)
            simple = false;

        ধ্রুৱ সংরচনা xkb_চাবি * ধ্রুৱ *overlays_চাবিগুলি = (চাবি->overlays_inline)
                                                    ? &চাবি->overlay_চাবি
                                                    : চাবি->overlays_চাবিগুলি;
        যেতিয়ালৈ (remaining) {
            /* isolate lowest set bit */
            ধ্রুৱ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            ধ্রুৱ xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            যদি (চাবি->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            ধ্রুৱ সংরচনা xkb_চাবি *overlay_চাবি = *(overlays_চাবিগুলি++);

            ধ্রুৱ xkb_atom_t overlay_চাবি_name = (substitutions == রিক্ত)
                ? overlay_চাবি->name
                : substitute_name(substitutions, overlay_চাবি->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, চাবি_Name_Text(keymap->ctx, overlay_চাবি_name));
        }
    }

    যদি (groups_গুনতি > 1 || explicit_actions || explicit)
        simple = false;

    যদি (simple) {
        যদি (groups_গুনতি == 0) {
            /* Remove trailing comma */
            delete_last_char(buf, ',');
            যদি (buf->buf[buf->size - 1] == ',')
                buf->size--;
        } অন্যথা {
            copy_to_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_index_t group = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            group = xkb_keymap_চাবি_effective_layout(চাবি, group);
            যদি (!write_keysyms(keymap, buf, buf2, চাবি, group, pretty, false))
                ঘূৰাই false;
            copy_to_buf(buf, " ]");
        }
        copy_to_buf(buf, " };\n");
    }
    অন্যথা {
        xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
        বাবে (xkb_layout_index_t group = 0; group < চাবি->অঙ্ক_groups; group++) {
            যদি (!((UINT32_C(1) << group) & layouts))
                অনুৱর্ত;

            new_group++;

            ধ্রুৱ bool print_actions =
                /* Group has explicit actions */
                চাবি->groups[group].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (চাবি->groups[group].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (চাবি->groups[group].implicit_actions && !some_interprets);

            যদি (new_group != 0)
                copy_to_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_group + 1);

            যদি (!write_keysyms(keymap, buf, buf2, চাবি, group,
                               pretty, print_actions))
                ঘূৰাই false;
            copy_to_buf(buf, " ]");
            যদি (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_group + 1);
                যদি (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, চাবি, group))
                    ঘূৰাই false;
                copy_to_buf(buf, " ]");
            }
        }
        যদি (!groups_গুনতি)
            delete_last_char(buf, ',');
        copy_to_buf(buf, "\n\t};\n");
    }

    ঘূৰাই true;
}


স্থির পূর্ণাঙ্ক
cmp_modmap_step_1(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
    ধ্রুৱ ModMapEntry *m1 = (ধ্রুৱ ModMapEntry *)a;
    ধ্রুৱ ModMapEntry *m2 = (ধ্রুৱ ModMapEntry *)b;

    যদি (!m1->mods) {
        যদি (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            ঘূৰাই 0;
        }
        ঘূৰাই +1;
    } অন্যথা যদি (!m2->mods) {
        ঘূৰাই -1;
    }

    /* Valid only on step 1 */
    যদি (m1->চাবি_Code < m2->চাবি_Code)
        ঘূৰাই -1;
    অন্যথা যদি (m1->চাবি_Code > m2->চাবি_Code)
        ঘূৰাই +1;

    যদি (m1->mods < m2->mods)
        ঘূৰাই -1;
    অন্যথা যদি (m1->mods > m2->mods)
        ঘূৰাই +1;

    যদি (m1->have_প্ৰতীক < m2->have_প্ৰতীক)
        ঘূৰাই +1;
    অন্যথা যদি (m1->have_প্ৰতীক > m2->have_প্ৰতীক)
        ঘূৰাই -1;

    যদি (m1->have_প্ৰতীক) {
        যদি (m1->u.চাবি_Sym < m2->u.চাবি_Sym)
            ঘূৰাই -1;
        যদি (m1->u.চাবি_Sym > m2->u.চাবি_Sym)
            ঘূৰাই +1;
    } অন্যথা {
        যদি (m1->u.চাবি_Name < m2->u.চাবি_Name)
            ঘূৰাই -1;
        যদি (m1->u.চাবি_Name > m2->u.চাবি_Name)
            ঘূৰাই +1;
    }

    ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
cmp_modmap_step_2(ধ্রুৱ শূন্য *a, ধ্রুৱ শূন্য *b)
{
    ধ্রুৱ ModMapEntry *m1 = (ধ্রুৱ ModMapEntry *)a;
    ধ্রুৱ ModMapEntry *m2 = (ধ্রুৱ ModMapEntry *)b;

    যদি (m1->mods < m2->mods)
        ঘূৰাই -1;
    যদি (m1->mods > m2->mods)
        ঘূৰাই +1;

    যদি (m1->have_প্ৰতীক < m2->have_প্ৰতীক)
        ঘূৰাই -1;
    যদি (m1->have_প্ৰতীক > m2->have_প্ৰতীক)
        ঘূৰাই +1;

    যদি (m1->have_প্ৰতীক) {
        যদি (m1->u.চাবি_Sym < m2->u.চাবি_Sym)
            ঘূৰাই -1;
        যদি (m1->u.চাবি_Sym > m2->u.চাবি_Sym)
            ঘূৰাই +1;
    } অন্যথা {
        যদি (m1->চাবি_Code < m2->চাবি_Code)
            ঘূৰাই -1;
        যদি (m1->চাবি_Code > m2->চাবি_Code)
            ঘূৰাই +1;
    }

    ঘূৰাই 0;
}

স্থির bool
write_modmaps(ধ্রুৱ সংরচনা xkb_keymap *keymap, গণনা xkb_keymap_format format,
              ধ্রুৱ চাবি_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, সংরচনা buf *buf)
{
    যদি (!keymap->অঙ্ক_modmaps)
        ঘূৰাই true;

    darray(ModMapEntry) modmaps = darray_new();
    darray_from_বস্তুসমূহ(modmaps, keymap->modmaps, keymap->অঙ্ক_modmaps);
    যদি (!darray_বস্তুসমূহ(modmaps))
        ঘূৰাই false;

    /* Sort entries by key code */
    qsort(darray_বস্তুসমূহ(modmaps), darray_size(modmaps),
          পরিমাণ(*darray_বস্তুসমূহ(modmaps)), &cmp_modmap_step_1);

    /* Drop disabled entries at the end */
    বাবে (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        যদি (darray_item(modmaps, m).mods)
            ভঙ্গ;
        darray_size(modmaps)--;
    }

    /*
     * Build detailed keymap modmap
     *
     * NOTE: use an explicit for-loop instead of the darray_foreach macro
     * because we may add entries while iterating it. Iterate from the end,
     * so that we do not iterate the new entries.
     */
    xkb_keycode_t current_keycode = XKB_KEYCODE_INVALID;
    bool convert_to_v1_compatibility_entry = false;
    বাবে (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ModMapEntry *mm = &darray_item(modmaps, m);
        প্রতিজ্ঞা(mm->mods);

        যদি (mm->চাবি_Code == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ধ্রুৱ xkb_mod_mask_t core_mods = (mm->mods & MOD_REAL_MASK_ALL);
            যদি (convert_to_v1_compatibility_entry && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } অন্যথা {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            অনুৱর্ত;
        }

        current_keycode = mm->চাবি_Code;
        ধ্রুৱ সংরচনা xkb_চাবি *চাবি = Xkb_চাবি(keymap, mm->চাবি_Code);

        ধ্রুৱ xkb_mod_mask_t core_mods = (চাবি->modmap & MOD_REAL_MASK_ALL);
        যদি (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((চাবি->modmap & (চাবি->modmap - 1)) || চাবি->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_to_v1_compatibility_entry = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    চাবি_Name_Text(keymap->ctx, চাবি->name), চাবি->modmap);
        } অন্যথা {
            /* V2 format or V1 format with a single core modifier */
            প্রতিজ্ঞা(চাবি->modmap);
            mm->mods = চাবি->modmap;
            /* Force key name */
            mm->have_প্ৰতীক = false;
            mm->u.চাবি_Name = চাবি->name;
            convert_to_v1_compatibility_entry = false;
        }
    }

    যদি (darray_empty(modmaps))
        গচ্ছ exit;

    /* Sort entries by modifier mask */
    qsort(darray_বস্তুসমূহ(modmaps), darray_size(modmaps),
          পরিমাণ(*darray_বস্তুসমূহ(modmaps)), &cmp_modmap_step_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_mask_t current_mods = 0;
    ধ্রুৱ ModMapEntry *entry;
    darray_foreach(entry, modmaps) {
        যদি (!entry->mods)
            অনুৱর্ত;

        যদি (current_mods != entry->mods) {
            যদি (current_mods) {
                copy_to_buf(buf, " };\n");
            }
            current_mods = entry->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } অন্যথা {
            copy_to_buf(buf, ", ");
        }

        যদি (entry->have_প্ৰতীক) {
            যদি (pretty || entry->u.চাবি_Sym == XKB_চাবি_No_প্ৰতীক)
                write_buf(buf, "%s", KeysymText(keymap->ctx, entry->u.চাবি_Sym));
            অন্যথা
                write_buf(buf, "0x%"PRIx32, entry->u.চাবি_Sym);
        } অন্যথা {
            ধ্রুৱ xkb_atom_t name = (substitutions == রিক্ত)
                ? entry->u.চাবি_Name
                : substitute_name(substitutions, entry->u.চাবি_Name);
            write_buf(buf, "%s", চাবি_Name_Text(keymap->ctx, name));
        }
    }
    যদি (current_mods) {
        copy_to_buf(buf, " };\n");
    }

exit:
    darray_free(modmaps);
    ঘূৰাই true;
}

স্থির bool
write_symbols(ধ্রুৱ সংরচনা xkb_keymap *keymap,
              ধ্রুৱ সংরচনা xkb_keymap_serialize_config *config,
              ধ্রুৱ চাবি_name_substitutions *substitutions,
              xkb_layout_index_t max_groups, bool some_interp,
              সংরচনা buf *buf)
{
    ধ্রুৱ bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ধ্রুৱ bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_মানসমূহ);
    ধ্রুৱ bool explicit_চাবি_মানসমূহ =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_চাবি_মানসমূহ);
    ধ্রুৱ bool drop_unused = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ধ্রুৱ bool drop_interprets = (explicit_চাবি_মানসমূহ && drop_unused);
    /* TODO: print all default values if explicit flag is set */

    যদি (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    অন্যথা
        copy_to_buf(buf, "xkb_symbols {\n");

    bool has_group_names = false;
    xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
    বাবে (xkb_layout_index_t group = 0; group < keymap->অঙ্ক_group_names; group++) {
        যদি (!((UINT32_C(1) << group) & config->layouts))
            অনুৱর্ত;

        new_group++;

        যদি (keymap->group_names[group]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_group + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->group_names[group]));
            copy_to_buf(buf, ";\n");
            has_group_names = true;
        }
    }
    যদি (has_group_names)
        copy_to_buf(buf, "\n");

    যদি (defaults && !write_actions_defaults(keymap, config->format, buf))
        ঘূৰাই false;

    সংরচনা buf buf2 = { রিক্ত, 0, 0 };
    ধ্রুৱ সংরচনা xkb_চাবি *চাবি;
    xkb_চাবিগুলি_foreach(চাবি, keymap) {
        /* Skip keys with no explicit values */
        যদি (চাবি->explicit &&
            !write_চাবি(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_চাবি_মানসমূহ, pretty, buf,
                       &buf2, চাবি))
        {
            মুক্ত(buf2.buf);
            ঘূৰাই false;
        }
    }
    মুক্ত(buf2.buf);

    যদি (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        ঘূৰাই false;

    copy_to_buf(buf, "};\n\n");
    ঘূৰাই true;
}

স্থির গণনা xkb_status
write_keymap(ধ্রুৱ সংরচনা xkb_keymap *keymap,
             ধ্রুৱ সংরচনা xkb_keymap_serialize_config *config,
             সংরচনা buf *buf, সংরচনা xkb_keymap_serialize_result *result)
{
    চাবি_name_substitutions substitutions = darray_new();
    যদি (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_চাবিগুলি(keymap, &substitutions))
    {
        ঘূৰাই false;
    }

    ধ্রুৱ চাবি_name_substitutions * ধ্রুৱ substitutions_ptr =
        (darray_empty(substitutions)) ? রিক্ত : &substitutions;

    ধ্রুৱ xkb_layout_index_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    ধ্রুৱ bool ok = (
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
    ঘূৰাই ok ? XKB_SUCCESS : -1;
}

গণনা xkb_status
text_v1_keymap_serialize(
        ধ্রুৱ সংরচনা xkb_keymap *keymap,
        ধ্রুৱ সংরচনা xkb_keymap_serialize_config *config,
        সংরচনা xkb_keymap_serialize_result *result
)
{
    সংরচনা buf buf = { রিক্ত, 0, 0 };

    ধ্রুৱ গণনা xkb_status status =
        write_keymap(keymap, config, &buf, result);
    যদি (status != XKB_SUCCESS) {
        মুক্ত(buf.buf);
        result->serialized = রিক্ত;
        ঘূৰাই status;
    }

    result->serialized = buf.buf;
    result->length = buf.size + 1;
    result->layouts = config->layouts;

    ঘূৰাই XKB_SUCCESS;
}
