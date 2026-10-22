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

#define BUF_CHUNK_SIZE 4096

بنية buf {
    حرف *buf;
    size_t size;
    size_t alloc;
};

#define xkb_abs(n) _Generic((n),                \
                            صحيح: abs,           \
                            long صحيح: labs,     \
                            افتراضي: llabs )((n))

ساكن bool
do_realloc(بنية buf *buf, size_t at_least)
{
    buf->alloc += BUF_CHUNK_SIZE;
    إذا (at_least >= BUF_CHUNK_SIZE)
        buf->alloc += at_least;

    حرف *ثابت new = realloc(buf->buf, buf->alloc);
    إذا (!new)
        أرجع false;

    buf->buf = new;
    أرجع true;
}

ATTR_PRINTF(2, 3) ساكن bool
check_write_buf(بنية buf *buf, ثابت حرف *fmt, ...)
{
    va_list args;
    صحيح printed;
    size_t available;

    available = buf->alloc - buf->size;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    أكد(buf->buf || available == 0);
    va_start(args, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->size : buf->buf, available,
                        fmt, args);
    va_end(args);

    إذا (printed < 0) {
        /* Some error */
        اذهب err;
    } وإلا إذا ((size_t)printed >= available) {
        /* Not enough space: need to realloc */
        إذا (!do_realloc(buf, (size_t)printed + 1))
            اذهب err;

        /* The buffer has enough space now. */
        available = buf->alloc - buf->size;
        أكد(buf->buf && (size_t) printed < available);
        va_start(args, fmt);
        printed = vsnprintf(buf->buf + buf->size, available, fmt, args);
        va_end(args);

        إذا (printed < 0 || (size_t) printed >= available)
            اذهب err;
    }

    buf->size += (size_t)printed;
    أرجع true;

err:
    حرر(buf->buf);
    buf->buf = عدم;
    أرجع false;
}

#define write_buf(buf, ...) افعل { \
    إذا (!check_write_buf(buf, __VA_ARGS__)) \
        أرجع false; \
} طالما (0)

ساكن bool
check_copy_to_buf(بنية buf *buf, ثابت حرف* source, size_t len)
{
    إذا (len == 0)
        أرجع true;

    ثابت size_t available = buf->alloc - buf->size;
    إذا (len >= available &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        حرر(buf->buf);
        buf->buf = عدم;
        أرجع false;
    }

    memcpy(buf->buf + buf->size, source, len);
    buf->size += len;
    /* Append NULL byte */
    buf->buf[buf->size] = '\0';
    أرجع true;
}

#define copy_to_buf_len(buf, source, len) افعل { \
    إذا (!check_copy_to_buf(buf, source, len)) \
        أرجع false; \
} طالما (0)

#define copy_to_buf(buf, source) \
    copy_to_buf_len(buf, source, حجم(source) - 1)

ساكن bool
check_write_string_literal(بنية buf *buf, ثابت حرف* string)
{
    copy_to_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    size_t pending_start = 0;
    size_t current = 0;
    حرف escape_buffer[] = {'\\', '0', '0', '0', '0'};
    لكل (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        بدل (string[current]) {
        حالة '\n':
            /* `\n` would break strings */
            escape_buffer[1] = 'n';
            escape_len = 2;
            اكسر;
        حالة '\\':
            /* `\` would create escape sequences */
            escape_buffer[1] = '\\';
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
            إذا (unlikely((unsigned حرف)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_buffer[1] = '0';
                escape_buffer[2] = '0';
                escape_buffer[3] = (حرف)('0' + (string[current] >> 3));
                escape_buffer[4] = (حرف)('0' + (string[current] & 0x7));
                escape_len = 5;
            } وإلا {
                /* Expand the chunk */
                استمر;
            }
        }
        /* Write pending chunk */
        أكد(current >= pending_start);
        copy_to_buf_len(buf, string + pending_start, current - pending_start);
        /* Write escape sequence */
        copy_to_buf_len(buf, escape_buffer, escape_len);
        pending_start = current + 1;
    }
    /* Write remaining chunk */
    أكد(current >= pending_start);
    copy_to_buf_len(buf, string + pending_start, current - pending_start);

    copy_to_buf(buf, "\"");
    أرجع true;
}

#define write_buf_string_literal(buf, string) افعل { \
    إذا (!check_write_string_literal(buf, string)) \
        أرجع false; \
} طالما (0)

ساكن فراغ
delete_last_char(بنية buf *buf, حرف c)
{
    إذا (buf->size && buf->buf[buf->size - 1] == c)
        buf->buf[--buf->size] = '\0';
}

/** Entry for a name substituion LUT */
بنية ئاچقۇچ_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
ساكن صحيح
cmp_ئاچقۇچ_name_substitution(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت xkb_atom_t s1 = ((بنية ئاچقۇچ_name_substitution *) a)->source;
    ثابت xkb_atom_t s2 = ((بنية ئاچقۇچ_name_substitution *) b)->source;
    أرجع (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

تعريفنوع darray(بنية ئاچقۇچ_name_substitution) ئاچقۇچ_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
ساكن bool
rename_long_ئاچقۇچلار(ثابت بنية xkb_keymap *keymap,
                 ئاچقۇچ_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    تعداد {
        first = '!',
        last = '0',
        invalid = '\0'
    };
    حرف prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', last
    };
    static_assert(حجم(prefixes) == last - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ثابت بنية xkb_ئاچقۇچ *ئاچقۇچ;
    xkb_ئاچقۇچلار_foreach(ئاچقۇچ, keymap) {
        إذا (ئاچقۇچ->name == XKB_ATOM_NONE)
            استمر;

        ثابت حرف * ثابت name = xkb_atom_text(keymap->ctx, ئاچقۇچ->name);
        ثابت size_t len = strlen_safe(name);
        إذا (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (بنية ئاچقۇچ_name_substitution) {
                .source = ئاچقۇچ->name,
                .keycode = ئاچقۇچ->keycode,
                .target = ئاچقۇچ->name
            });
        } وإلا إذا (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    لكل (darray_size_t a = 0; a < keymap->رەقەم_ئاچقۇچ_aliases; a++) {
        ثابت بنية xkb_ئاچقۇچ_alias * ثابت alias = &keymap->ئاچقۇچ_aliases[a];
        ثابت حرف * ثابت name = xkb_atom_text(keymap->ctx, alias->alias);
        ثابت size_t len = strlen_safe(name);
        إذا (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (بنية ئاچقۇچ_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } وإلا إذا (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    إذا darray_empty(*substitutions) {
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
    حرف *prefix = prefixes;
    ثابت حرف *prefix_max = prefixes + حجم(prefixes) - 1;
    طالما (*prefix == invalid && prefix < prefix_max)
        prefix++;

    /* Keycode prefix: prefer ‘0’ if available */
    حرف keycode_prefix;
    إذا (prefixes[last - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_prefix = last;
        prefixes[last - first] = invalid;
    } وإلا {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_prefix = *prefix;
        إذا (unlikely(keycode_prefix == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            أرجع true;
        }
        /* Update current prefix */
        *prefix = invalid;
        طالما (*prefix == invalid && prefix < prefix_max)
            prefix++;
    }

    /* Proceed renaming */
    uint16_t next_id = 1;
    بنية ئاچقۇچ_name_substitution *sub;
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
                     keycode_prefix, sub->keycode);
            ثابت xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, حجم(buf) - 2);
            إذا (target == XKB_ATOM_NONE)
                اذهب error;
            sub->target = target;
        } وإلا إذا (*prefix != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, حجم(buf), "%c%03"PRIx16, *prefix, next_id);
            ثابت xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, حجم(buf) - 2);
            إذا (target == XKB_ATOM_NONE)
                اذهب error;
            sub->target = target;
            إذا (++next_id > 0xfff) {
                *prefix = invalid;
                /* Get next available prefix */
                طالما (*prefix == invalid && prefix < prefix_max)
                    prefix++;
                /* Reset counter */
                next_id = 1;
            }
        } وإلا {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->target = sub->source;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_بۇيۇملار(*substitutions), darray_size(*substitutions),
          حجم(*darray_بۇيۇملار(*substitutions)), &cmp_ئاچقۇچ_name_substitution);

    أرجع true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate key name substitution\n");
    أرجع false;
}

/** Substitute a key name with a valid X11 key name */
ساكن xkb_atom_t
substitute_name(ثابت ئاچقۇچ_name_substitutions *substitutions, xkb_atom_t name)
{
    /*ئىككىلىك ئىزدەش*/
    darray_size_t first = 0;
    darray_size_t last = darray_size(*substitutions);

    طالما (last > first) {
        darray_size_t mid = first + (last - 1 - first) / 2;
        ثابت xkb_atom_t source = darray_item(*substitutions, mid).source;
        إذا (source < name)
            first = mid + 1;
        وإلا إذا (source > name)
            last = mid;
        وإلا /* found it */
            أرجع darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    أرجع name;
}

ساكن bool
write_vmods(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
            bool explicit, بنية buf *buf)
{
    ثابت بنية xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        إذا (!has_some) {
            copy_to_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } وإلا {
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
        ثابت bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        إذا ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
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

    إذا (has_some)
        copy_to_buf(buf, ";\n\n");

    أرجع true;
}

ساكن bool
write_keycodes(ثابت بنية xkb_keymap *keymap,
               ثابت ئاچقۇچ_name_substitutions *substitutions,
               تعداد xkb_keymap_serialize_flags flags, بنية buf *buf)
{
    ثابت بنية xkb_ئاچقۇچ *ئاچقۇچ;
    xkb_led_index_t idx;
    ثابت بنية xkb_led *led;

    ثابت bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    إذا (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    وإلا
        copy_to_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_ئاچقۇچ_code, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_ئاچقۇچ_code, 255));

    xkb_ئاچقۇچلار_foreach(ئاچقۇچ, keymap) {
        إذا (ئاچقۇچ->name == XKB_ATOM_NONE)
            استمر;

        ثابت xkb_atom_t name = (substitutions == عدم)
            ? ئاچقۇچ->name
            : substitute_name(substitutions, ئاچقۇچ->name);
        إذا (pretty)
            write_buf(buf, "\t%-20s", ئاچقۇچ_Name_Text(keymap->ctx, name));
        وإلا
            write_buf(buf, "\t%s", ئاچقۇچ_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", ئاچقۇچ->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        إذا (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_to_buf(buf, ";\n");
        }

    لكل (darray_size_t i = 0; i < keymap->رەقەم_ئاچقۇچ_aliases; i++) {
        ثابت xkb_atom_t alias = (substitutions == عدم)
            ? keymap->ئاچقۇچ_aliases[i].alias
            : substitute_name(substitutions, keymap->ئاچقۇچ_aliases[i].alias);
        ثابت xkb_atom_t real = (substitutions == عدم)
            ? keymap->ئاچقۇچ_aliases[i].real
            : substitute_name(substitutions, keymap->ئاچقۇچ_aliases[i].real);

        إذا (pretty)
            write_buf(buf, "\talias %-14s",
                      ئاچقۇچ_Name_Text(keymap->ctx, alias));
        وإلا
            write_buf(buf, "\talias %s",
                      ئاچقۇچ_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  ئاچقۇچ_Name_Text(keymap->ctx, real));
    }

    copy_to_buf(buf, "};\n\n");
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
        copy_to_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    إذا (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        أرجع false;

    ثابت bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    لكل (darray_size_t i = 0; i < keymap->رەقەم_types; i++) {
        ثابت بنية xkb_ئاچقۇچ_type * ثابت type = &keymap->types[i];
        إذا (!type->required && drop_unused)
            استمر;

        copy_to_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_to_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        لكل (darray_size_t j = 0; j < type->رەقەم_entries; j++) {
            ثابت حرف *str;
            ثابت بنية xkb_ئاچقۇچ_type_entry *entry = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            إذا (entry->level == 0 && entry->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_قىممەتلەر))
                استمر;

            str = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              entry->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, entry->level + 1);

            إذا (entry->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           entry->preserve.mods));
        }

        لكل (xkb_level_index_t n = 0; n < type->رەقەم_level_names; n++)
            إذا (type->level_names[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_names[n]));
                copy_to_buf(buf, ";\n");
            }

        copy_to_buf(buf, "\t};\n");
    }

    copy_to_buf(buf, "};\n\n");
    أرجع true;
}

ساكن bool
write_led_map(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
              bool defaults, بنية buf *buf, ثابت بنية xkb_led *led)
{
    copy_to_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_to_buf(buf, " {\n");

    إذا (led->which_groups) {
        إذا (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      LedStateMaskText(keymap->ctx, groupComponentMaskNames,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    إذا (led->which_mods) {
        إذا (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      LedStateMaskText(keymap->ctx, modComponentMaskNames,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    إذا (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  ControlMaskText(keymap->ctx, format, led->ctrls));
    }

    copy_to_buf(buf, "\t};\n");
    أرجع true;
}

ساكن ثابت حرف *
affect_lock_text(تعداد xkb_action_flags flags, bool show_both)
{
    بدل (flags & (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK)) {
    حالة 0:
        أرجع show_both ? ",affect=both" : "";
    حالة ACTION_LOCK_NO_UNLOCK:
        أرجع ",affect=lock";
    حالة ACTION_LOCK_NO_LOCK:
        أرجع ",affect=unlock";
    حالة ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK:
        أرجع ",affect=neither";
    افتراضي:
        أرجع "";
    }
}

#define بەلگە_PADDING 15
#define ACTION_PADDING 30

ساكن bool
write_action(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
             xkb_layout_index_t max_groups, بنية buf *buf,
             ثابت اتحاد xkb_action *action,
             ثابت حرف *prefix, ثابت حرف *suffix)
{
    ثابت حرف *type;
    ثابت حرف *args = عدم;

    إذا (!prefix)
        prefix = "";
    إذا (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    بدل (action->type) {
    حالة ACTION_TYPE_MOD_SET:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_LOCK:
        إذا (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            args = "modMapMods";
        وإلا
            args = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        إذا (unlockOnPress && !isModsUnLockOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->group.flags & ACTION_LATCH_ON_PRESS);
        إذا (latchOnPress && !isModsLatchOnPressSupported(format)) {
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
        اكسر;

    حالة ACTION_TYPE_GROUP_SET:
    حالة ACTION_TYPE_GROUP_LATCH:
    حالة ACTION_TYPE_GROUP_LOCK:
        إذا ((uint32_t) xkb_abs(action->group.group) <
            (max_groups + !(action->group.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool lockOnRelease = action->type == ACTION_TYPE_GROUP_LOCK &&
                                 (action->group.flags & ACTION_LOCK_ON_RELEASE);
            إذا (lockOnRelease && !isGroupLockOnReleaseSupported(format)) {
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
        } وإلا {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->group.group +
                       !!(action->group.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            اذهب void_action;
        }
        اكسر;

    حالة ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", prefix, type, suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", prefix, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_LOCK:
        args = affect_lock_text(action->btn.flags, true);
        /* fallthrough */
    حالة ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", prefix, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        إذا (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        وإلا
            copy_to_buf(buf, "default");
        إذا (action->btn.سانى) {
            أكد(action->type != ACTION_TYPE_PTR_LOCK);
            write_buf(buf, ",count=%u", action->btn.سانى);
        }
        إذا (args)
            write_buf(buf, "%s", args);
        write_buf(buf, ")%s", suffix);
        اكسر;

    حالة ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", prefix, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.قىممەت >= 0) ? "+" : "",
                  action->dflt.قىممەت);
        write_buf(buf, ")%s", suffix);
        اكسر;

    حالة ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", prefix, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        اكسر;

    حالة ACTION_TYPE_CTRL_SET:
    حالة ACTION_TYPE_CTRL_LOCK:
        write_buf(buf, "%s%s(controls=%s%s)%s", prefix, type,
                  ControlMaskText(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_LOCK
                    ? affect_lock_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        اكسر;

    حالة ACTION_TYPE_REDIRECT_ئاچقۇچ: {
        write_buf(buf, "%s%s(", prefix, type);
        ثابت بنية xkb_ئاچقۇچ * ثابت ئاچقۇچ = Xkb_ئاچقۇچ(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        إذا (ئاچقۇچ)
            write_buf(buf, "keycode=%s", ئاچقۇچ_Name_Text(keymap->ctx, ئاچقۇچ->name));
        إذا (action->redirect.affect_mods) {
            xkb_mod_mask_t mask;
            mask = (action->redirect.affect_mods & action->redirect.mods);
            إذا (mask)
                write_buf(buf, ",modifiers=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
            mask = (action->redirect.affect_mods & ~action->redirect.mods);
            إذا (mask)
                write_buf(buf, ",clearMods=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
        }
        write_buf(buf, ")%s", suffix);
        اكسر;
    }

    حالة ACTION_TYPE_UNKNOWN:
    حالة ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", prefix, suffix);
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
        إذا (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      prefix, suffix);
        وإلا
            write_buf(buf, "%sVoidAction()%s", prefix, suffix);
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_رەقەم_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        أكد(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  prefix, type, action->type, action->priv.data[0],
                  action->priv.data[1], action->priv.data[2],
                  action->priv.data[3], action->priv.data[4],
                  action->priv.data[5], action->priv.data[6],
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
              xkb_layout_index_t max_groups, بنية buf *buf, بنية buf *buf2,
              ثابت بنية xkb_ئاچقۇچ *ئاچقۇچ, xkb_layout_index_t group)
{
    ساكن ثابت اتحاد xkb_action noAction = { .type = ACTION_TYPE_NONE };

    لكل (xkb_level_index_t level = 0; level < Xkb_ئاچقۇچ_رەقەم_Levels(ئاچقۇچ, group);
         level++) {

        إذا (level != 0)
            copy_to_buf(buf, ", ");

        ثابت اتحاد xkb_action *actions = عدم;
        xkb_action_سانى_t سانى = xkb_keymap_ئاچقۇچ_get_actions_by_level(
            keymap, ئاچقۇچ, group, level, &actions
        );

        إذا (سانى > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            سانى = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->size = 0;
        إذا (سانى == 0) {
            إذا (!write_action(keymap, format, max_groups,
                              buf2, &noAction, عدم, عدم))
                أرجع false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        وإلا إذا (سانى == 1) {
            إذا (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), عدم, عدم))
                أرجع false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        وإلا {
            copy_to_buf(buf2, "{ ");
            لكل (xkb_action_سانى_t k = 0; k < سانى; k++) {
                إذا (k != 0)
                    copy_to_buf(buf2, ", ");
                size_t old_size = buf2->size;
                إذا (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), عدم, عدم))
                    أرجع false;
                /* Check if padding is necessary */
                إذا (buf2->size >= old_size + ACTION_PADDING)
                    استمر;
                /* Compute and write padding, then write the action again */
                ثابت صحيح padding = (صحيح)(old_size + ACTION_PADDING - buf2->size);
                buf2->size = old_size;
                write_buf(buf2, "%*s", padding, "");
                إذا (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), عدم, عدم))
                    أرجع false;
            }
            copy_to_buf(buf2, " }");
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
    ثابت حرف *type = ActionTypeText(action->type);

#define PREFIX "\t"

    بدل (action->type) {
    حالة ACTION_TYPE_MOD_SET:
    حالة ACTION_TYPE_MOD_LATCH:
    حالة ACTION_TYPE_MOD_LOCK:
        أكد(action->mods.flags == 0);
        أكد(action->mods.mods.mods == 0);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        إذا (action->type != ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, PREFIX"%s.latchOnPress = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        إذا (action->type == ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        إذا (isModsUnLockOnPressSupported(format))
            write_buf(buf, PREFIX"%s.unlockOnPress = false;\n", type);
        اكسر;

    حالة ACTION_TYPE_GROUP_SET:
    حالة ACTION_TYPE_GROUP_LATCH:
    حالة ACTION_TYPE_GROUP_LOCK:
        أكد(action->group.flags == 0);
        أكد(action->group.group == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.group = +0;\n", type);
        إذا (action->type != ACTION_TYPE_GROUP_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        إذا (action->type == ACTION_TYPE_GROUP_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        إذا (action->type == ACTION_TYPE_GROUP_LOCK &&
            isGroupLockOnReleaseSupported(format))
            write_buf(buf, PREFIX"%s.lockOnRelease = false;\n", type);
        اكسر;


    حالة ACTION_TYPE_PTR_MOVE:
        أكد(action->ptr.x == 0);
        أكد(action->ptr.y == 0);
        أكد(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.x = +0;\n", type);
        write_buf(buf, PREFIX"%s.y = +0;\n", type);
        write_buf(buf, PREFIX"%s.accel = true;\n", type);
        اكسر;

    حالة ACTION_TYPE_PTR_LOCK:
    حالة ACTION_TYPE_PTR_BUTTON:
        أكد(action->btn.flags == 0);
        أكد(action->btn.button == 0);
        أكد(action->btn.سانى == 0);
        write_buf(buf, PREFIX"%s.button = 0;\n", type);
        write_buf(buf, PREFIX"%s.count = 0;\n", type);
        إذا (action->type == ACTION_TYPE_PTR_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        اكسر;

    حالة ACTION_TYPE_PTR_DEFAULT:
        أكد(action->dflt.flags == 0);
        أكد(action->dflt.قىممەت == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.button = +1;\n", type);
        اكسر;

    حالة ACTION_TYPE_SWITCH_VT:
        أكد(action->screen.flags == ACTION_SAME_SCREEN);
        أكد(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.screen = +0;\n", type);
        write_buf(buf, PREFIX"%s.same = true;\n", type);
        اكسر;

    حالة ACTION_TYPE_CTRL_SET:
    حالة ACTION_TYPE_CTRL_LOCK:
        أكد(action->ctrls.flags == 0);
        أكد(action->ctrls.ctrls == 0);
        write_buf(buf, PREFIX"%s.controls = 0;\n", type);
        إذا (action->type == ACTION_TYPE_CTRL_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        اكسر;

    حالة ACTION_TYPE_REDIRECT_ئاچقۇچ:
        أكد(action->redirect.keycode == keymap->redirect_ئاچقۇچ_auto);
        أكد(action->redirect.affect_mods == 0);
        أكد(action->redirect.mods == 0);
        write_buf(buf, PREFIX"%s.keycode = auto;\n", type);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        write_buf(buf, PREFIX"%s.clearMods = 0;\n", type);
        اكسر;

    افتراضي:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_رەقەم_ENTRIES,
                      "Missing action type");
    }

#undef PREFIX

    أرجع true;
}

ساكن bool
write_actions_defaults(ثابت بنية xkb_keymap *keymap,
                       تعداد xkb_keymap_format format, بنية buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    لكل (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_رەقەم_ENTRIES; a++) {
        ثابت اتحاد xkb_action * restrict ثابت action = &defaults.actions[a];
        إذا (!write_action_defaults(keymap, format, action, buf))
            أرجع false;
    }
    copy_to_buf(buf, "\n");

    أرجع true;
}

ساكن ثابت بنية xkb_sym_interpret fallback_interpret = {
    .sym = XKB_ئاچقۇچ_Void_بەلگە,
    .required = true,
    .repeat = FALLBACK_INTERPRET_ئاچقۇچ_REPEAT,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .رەقەم_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ساكن bool
write_compat(ثابت بنية xkb_keymap * restrict keymap,
             تعداد xkb_keymap_format format, xkb_layout_index_t max_groups,
             تعداد xkb_keymap_serialize_flags flags, bool * restrict some_interp,
             بنية buf *buf)
{
    ثابت bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_قىممەتلەر);
    ثابت bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ثابت bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ئاچقۇچ_قىممەتلەر) && drop_unused);
    /* TODO: print all default values if the explicit flag is set */

    إذا (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    وإلا
        copy_to_buf(buf, "xkb_compatibility {\n");

    إذا (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        أرجع false;

    إذا (defaults && !write_actions_defaults(keymap, format, buf))
        أرجع false;

    copy_to_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_to_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    ثابت bool use_fallback_interpret =
        (!keymap->رەقەم_sym_interprets || drop_interprets);
    ثابت darray_size_t رەقەم_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->رەقەم_sym_interprets);
    ثابت بنية xkb_sym_interpret* ثابت sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    لكل (darray_size_t i = 0; i < رەقەم_sym_interprets; i++) {
        ثابت بنية xkb_sym_interpret *si = &sym_interprets[i];
        إذا (!si->required && drop_unused)
            استمر;

        *some_interp = true;

        copy_to_buf(buf, "\tinterpret ");
        إذا (!si->sym) {
            copy_to_buf(buf, "Any");
        } وإلا إذا (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } وإلا {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->match),
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        إذا (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      ModIndexText(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        إذا (si->level_one_only) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } وإلا إذا (defaults) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        إذا (si->repeat) {
            copy_to_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } وإلا إذا (defaults) {
            copy_to_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        ثابت اتحاد xkb_action *action = عدم;
        xkb_action_سانى_t action_سانى = si->رەقەم_actions;
        إذا (action_سانى <= 1) {
            action = &si->a.action;
        } وإلا إذا (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_سانى = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        إذا (action_سانى > 1) {
            copy_to_buf(buf, "\n\t\taction= {");
            ثابت حرف suffix[] = ", ";
            لكل (xkb_action_سانى_t k = 0; k < si->رەقەم_actions; k++) {
                إذا (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    أرجع false;
            }
            buf->size -= حجم(suffix) - 1; /* trailing comma */
            copy_to_buf(buf, "};");
            has_explicit_properties = true;
        } وإلا إذا (action_سانى == 1) {
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
        رەقەم_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].match == fallback_interpret.match &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].رەقەم_actions == fallback_interpret.رەقەم_actions
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

    copy_to_buf(buf, "};\n\n");

    أرجع true;
}

ساكن bool
write_keysyms(ثابت بنية xkb_keymap *keymap,
              بنية buf *buf, بنية buf *buf2,
              ثابت بنية xkb_ئاچقۇچ *ئاچقۇچ, xkb_layout_index_t group,
              bool pretty, bool show_actions)
{
    ثابت unsigned صحيح padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : بەلگە_PADDING)
        : 0;
    لكل (xkb_level_index_t level = 0; level < Xkb_ئاچقۇچ_رەقەم_Levels(ئاچقۇچ, group);
         level++) {
        ثابت xkb_keysym_t *syms;
        صحيح رەقەم_syms;

        إذا (level != 0)
            copy_to_buf(buf, ", ");

        رەقەم_syms = xkb_keymap_ئاچقۇچ_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (بنية xkb_keymap *)keymap,
            ئاچقۇچ->keycode, group, level, &syms
        );

        ثابت xkb_keysym_t no_بەلگە = XKB_ئاچقۇچ_No_بەلگە;
        إذا (رەقەم_syms == 0) {
            رەقەم_syms = 1;
            syms = &no_بەلگە;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        إذا (رەقەم_syms == 1) {
            إذا (pretty || syms[0] == XKB_ئاچقۇچ_No_بەلگە)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            وإلا
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } وإلا {
            إذا (pretty) {
                buf2->size = 0;
                copy_to_buf(buf2, "{ ");
                لكل (صحيح s = 0; s < رەقەم_syms; s++) {
                    إذا (s != 0)
                        copy_to_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_to_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } وإلا {
                copy_to_buf(buf, "{");
                لكل (صحيح s = 0; s < رەقەم_syms; s++) {
                    إذا (s != 0)
                        copy_to_buf(buf, ",");
                    إذا (syms[s] == XKB_ئاچقۇچ_No_بەلگە)
                        copy_to_buf(buf, "NoSymbol");
                    وإلا
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_to_buf(buf, "}");
            }
        }
    }

    أرجع true;
}

ساكن bool
write_ئاچقۇچ(ثابت بنية xkb_keymap *keymap,
          ثابت بنية xkb_keymap_serialize_config *config,
          ثابت ئاچقۇچ_name_substitutions *substitutions,
          xkb_layout_index_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          بنية buf *buf, بنية buf *buf2, ثابت بنية xkb_ئاچقۇچ *ئاچقۇچ)
{
    bool simple = true;

    xkb_layout_mask_t layouts = config->layouts;
    إذا (popcount32(layouts) != keymap->رەقەم_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_index_t l = 0;
        لكل (xkb_layout_mask_t ls = layouts; ls; ls >>= 1, l++) {
            إذا (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ثابت xkb_layout_index_t effective =
                    xkb_keymap_ئاچقۇچ_effective_layout(ئاچقۇچ, l);
                إذا (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } وإلا {
        layouts &= ((UINT64_C(1) << ئاچقۇچ->رەقەم_groups) - 1);
    }
    ثابت xkb_layout_index_t groups_سانى = popcount32(layouts);

    ثابت xkb_atom_t name = (substitutions == عدم)
        ? ئاچقۇچ->name
        : substitute_name(substitutions, ئاچقۇچ->name);

    إذا (pretty)
        write_buf(buf, "\tkey %-20s {", ئاچقۇچ_Name_Text(keymap->ctx, name));
    وإلا
        write_buf(buf, "\tkey %s {", ئاچقۇچ_Name_Text(keymap->ctx, name));

    إذا ((ئاچقۇچ->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        إذا (!explicit) {
            لكل (xkb_layout_index_t group = 1; group < ئاچقۇچ->رەقەم_groups; group++) {
                إذا (!((UINT32_C(1) << group) & layouts))
                    استمر;

                إذا (ئاچقۇچ->groups[group].type != ئاچقۇچ->groups[0].type) {
                    multi_type = true;
                    اكسر;
                }
            }
        }

        إذا (multi_type) {
            xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
            لكل (xkb_layout_index_t group = 0; group < ئاچقۇچ->رەقەم_groups; group++) {
                إذا (!((UINT32_C(1) << group) & layouts))
                    استمر;

                new_group++;

                إذا (!ئاچقۇچ->groups[group].explicit_type && !explicit)
                    استمر;

                ثابت بنية xkb_ئاچقۇچ_type * ثابت type = ئاچقۇچ->groups[group].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_group + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_to_buf(buf, ",");
            }
        }
        وإلا {
            ثابت بنية xkb_ئاچقۇچ_type * ثابت type = ئاچقۇچ->groups[0].type;
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
    ثابت bool explicit_actions = (ئاچقۇچ->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #define require_explicit(ئاچقۇچ, prop,                                       \
                             قىممەت, initial_قىممەت, default_interpret_قىممەت) ( \
        /* Explicit value in the source */                                    \
        ((ئاچقۇچ)->explicit & (prop)) ||                                         \
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
                ((ئاچقۇچ)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (قىممەت) != (initial_قىممەت) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_قىممەت) != (default_interpret_قىممەت)                  \
            )                                                                 \
        )                                                                     \
    )

    إذا (require_explicit(ئاچقۇچ, EXPLICIT_REPEAT, ئاچقۇچ->repeats,
                         DEFAULT_ئاچقۇچ_REPEAT, DEFAULT_INTERPRET_ئاچقۇچ_REPEAT)) {
        إذا (ئاچقۇچ->repeats)
            copy_to_buf(buf, "\n\t\trepeat= Yes,");
        وإلا
            copy_to_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    إذا (require_explicit(ئاچقۇچ, EXPLICIT_VMODMAP, ئاچقۇچ->vmodmap,
                         (xkb_mod_mask_t) DEFAULT_ئاچقۇچ_VMODMAP,
                         (xkb_mod_mask_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              ئاچقۇچ->vmodmap));
        simple = false;
    }

    #undef require_explicit

    بدل (ئاچقۇچ->out_of_range_group_policy) {
    حالة XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        copy_to_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        اكسر;

    حالة XKB_LAYOUT_OUT_OF_RANGE_REDIRECT: {
        إذا (!groups_سانى) {
            /* No layout to redirect to: skip */
            اكسر;
        }

        xkb_layout_mask_t layout_mask =
            (UINT32_C(1) << ئاچقۇچ->out_of_range_group_سان);
        إذا (!(layouts & layout_mask)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     ئاچقۇچ_Name_Text(keymap->ctx, name),
                     ئاچقۇچ->out_of_range_group_سان + 1);
            layout_mask = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ثابت xkb_layout_mask_t تۆۋەن = (layouts & (layout_mask - 1));
        ثابت xkb_layout_index_t new = popcount32(تۆۋەن);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        اكسر;
    }

    حالة XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    افتراضي:
        اكسر;
    }

    إذا (ئاچقۇچ->overlays) {
        xkb_overlay_mask_t remaining = ئاچقۇچ->overlays;

        ثابت xkb_overlay_index_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_سانى == 8, "invalid right shift");
        ثابت xkb_overlay_mask_t valid =
            (xkb_overlay_mask_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        إذا (remaining != ئاچقۇچ->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, ئاچقۇچ_Name_Text(keymap->ctx, name),
                     remaining, ئاچقۇچ->overlays & ~valid);
        }

        إذا (remaining && !ئاچقۇچ->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     ئاچقۇچ_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        إذا (remaining)
            simple = false;

        ثابت بنية xkb_ئاچقۇچ * ثابت *overlays_ئاچقۇچلار = (ئاچقۇچ->overlays_inline)
                                                    ? &ئاچقۇچ->overlay_ئاچقۇچ
                                                    : ئاچقۇچ->overlays_ئاچقۇچلار;
        طالما (remaining) {
            /* isolate lowest set bit */
            ثابت xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            ثابت xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            إذا (ئاچقۇچ->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            ثابت بنية xkb_ئاچقۇچ *overlay_ئاچقۇچ = *(overlays_ئاچقۇچلار++);

            ثابت xkb_atom_t overlay_ئاچقۇچ_name = (substitutions == عدم)
                ? overlay_ئاچقۇچ->name
                : substitute_name(substitutions, overlay_ئاچقۇچ->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, ئاچقۇچ_Name_Text(keymap->ctx, overlay_ئاچقۇچ_name));
        }
    }

    إذا (groups_سانى > 1 || explicit_actions || explicit)
        simple = false;

    إذا (simple) {
        إذا (groups_سانى == 0) {
            /* Remove trailing comma */
            delete_last_char(buf, ',');
            إذا (buf->buf[buf->size - 1] == ',')
                buf->size--;
        } وإلا {
            copy_to_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_index_t group = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            group = xkb_keymap_ئاچقۇچ_effective_layout(ئاچقۇچ, group);
            إذا (!write_keysyms(keymap, buf, buf2, ئاچقۇچ, group, pretty, false))
                أرجع false;
            copy_to_buf(buf, " ]");
        }
        copy_to_buf(buf, " };\n");
    }
    وإلا {
        xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
        لكل (xkb_layout_index_t group = 0; group < ئاچقۇچ->رەقەم_groups; group++) {
            إذا (!((UINT32_C(1) << group) & layouts))
                استمر;

            new_group++;

            ثابت bool print_actions =
                /* Group has explicit actions */
                ئاچقۇچ->groups[group].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (ئاچقۇچ->groups[group].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (ئاچقۇچ->groups[group].implicit_actions && !some_interprets);

            إذا (new_group != 0)
                copy_to_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_group + 1);

            إذا (!write_keysyms(keymap, buf, buf2, ئاچقۇچ, group,
                               pretty, print_actions))
                أرجع false;
            copy_to_buf(buf, " ]");
            إذا (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_group + 1);
                إذا (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, ئاچقۇچ, group))
                    أرجع false;
                copy_to_buf(buf, " ]");
            }
        }
        إذا (!groups_سانى)
            delete_last_char(buf, ',');
        copy_to_buf(buf, "\n\t};\n");
    }

    أرجع true;
}


ساكن صحيح
cmp_modmap_step_1(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت ModMapEntry *m1 = (ثابت ModMapEntry *)a;
    ثابت ModMapEntry *m2 = (ثابت ModMapEntry *)b;

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
    إذا (m1->ئاچقۇچ_Code < m2->ئاچقۇچ_Code)
        أرجع -1;
    وإلا إذا (m1->ئاچقۇچ_Code > m2->ئاچقۇچ_Code)
        أرجع +1;

    إذا (m1->mods < m2->mods)
        أرجع -1;
    وإلا إذا (m1->mods > m2->mods)
        أرجع +1;

    إذا (m1->have_بەلگە < m2->have_بەلگە)
        أرجع +1;
    وإلا إذا (m1->have_بەلگە > m2->have_بەلگە)
        أرجع -1;

    إذا (m1->have_بەلگە) {
        إذا (m1->u.ئاچقۇچ_Sym < m2->u.ئاچقۇچ_Sym)
            أرجع -1;
        إذا (m1->u.ئاچقۇچ_Sym > m2->u.ئاچقۇچ_Sym)
            أرجع +1;
    } وإلا {
        إذا (m1->u.ئاچقۇچ_Name < m2->u.ئاچقۇچ_Name)
            أرجع -1;
        إذا (m1->u.ئاچقۇچ_Name > m2->u.ئاچقۇچ_Name)
            أرجع +1;
    }

    أرجع 0;
}

ساكن صحيح
cmp_modmap_step_2(ثابت فراغ *a, ثابت فراغ *b)
{
    ثابت ModMapEntry *m1 = (ثابت ModMapEntry *)a;
    ثابت ModMapEntry *m2 = (ثابت ModMapEntry *)b;

    إذا (m1->mods < m2->mods)
        أرجع -1;
    إذا (m1->mods > m2->mods)
        أرجع +1;

    إذا (m1->have_بەلگە < m2->have_بەلگە)
        أرجع -1;
    إذا (m1->have_بەلگە > m2->have_بەلگە)
        أرجع +1;

    إذا (m1->have_بەلگە) {
        إذا (m1->u.ئاچقۇچ_Sym < m2->u.ئاچقۇچ_Sym)
            أرجع -1;
        إذا (m1->u.ئاچقۇچ_Sym > m2->u.ئاچقۇچ_Sym)
            أرجع +1;
    } وإلا {
        إذا (m1->ئاچقۇچ_Code < m2->ئاچقۇچ_Code)
            أرجع -1;
        إذا (m1->ئاچقۇچ_Code > m2->ئاچقۇچ_Code)
            أرجع +1;
    }

    أرجع 0;
}

ساكن bool
write_modmaps(ثابت بنية xkb_keymap *keymap, تعداد xkb_keymap_format format,
              ثابت ئاچقۇچ_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, بنية buf *buf)
{
    إذا (!keymap->رەقەم_modmaps)
        أرجع true;

    darray(ModMapEntry) modmaps = darray_new();
    darray_from_بۇيۇملار(modmaps, keymap->modmaps, keymap->رەقەم_modmaps);
    إذا (!darray_بۇيۇملار(modmaps))
        أرجع false;

    /* Sort entries by key code */
    qsort(darray_بۇيۇملار(modmaps), darray_size(modmaps),
          حجم(*darray_بۇيۇملار(modmaps)), &cmp_modmap_step_1);

    /* Drop disabled entries at the end */
    لكل (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        إذا (darray_item(modmaps, m).mods)
            اكسر;
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
    لكل (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ModMapEntry *mm = &darray_item(modmaps, m);
        أكد(mm->mods);

        إذا (mm->ئاچقۇچ_Code == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ثابت xkb_mod_mask_t core_mods = (mm->mods & MOD_REAL_MASK_ALL);
            إذا (convert_to_v1_compatibility_entry && core_mods) {
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

        current_keycode = mm->ئاچقۇچ_Code;
        ثابت بنية xkb_ئاچقۇچ *ئاچقۇچ = Xkb_ئاچقۇچ(keymap, mm->ئاچقۇچ_Code);

        ثابت xkb_mod_mask_t core_mods = (ئاچقۇچ->modmap & MOD_REAL_MASK_ALL);
        إذا (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((ئاچقۇچ->modmap & (ئاچقۇچ->modmap - 1)) || ئاچقۇچ->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_to_v1_compatibility_entry = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    ئاچقۇچ_Name_Text(keymap->ctx, ئاچقۇچ->name), ئاچقۇچ->modmap);
        } وإلا {
            /* V2 format or V1 format with a single core modifier */
            أكد(ئاچقۇچ->modmap);
            mm->mods = ئاچقۇچ->modmap;
            /* Force key name */
            mm->have_بەلگە = false;
            mm->u.ئاچقۇچ_Name = ئاچقۇچ->name;
            convert_to_v1_compatibility_entry = false;
        }
    }

    إذا (darray_empty(modmaps))
        اذهب exit;

    /* Sort entries by modifier mask */
    qsort(darray_بۇيۇملار(modmaps), darray_size(modmaps),
          حجم(*darray_بۇيۇملار(modmaps)), &cmp_modmap_step_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_mask_t current_mods = 0;
    ثابت ModMapEntry *entry;
    darray_foreach(entry, modmaps) {
        إذا (!entry->mods)
            استمر;

        إذا (current_mods != entry->mods) {
            إذا (current_mods) {
                copy_to_buf(buf, " };\n");
            }
            current_mods = entry->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } وإلا {
            copy_to_buf(buf, ", ");
        }

        إذا (entry->have_بەلگە) {
            إذا (pretty || entry->u.ئاچقۇچ_Sym == XKB_ئاچقۇچ_No_بەلگە)
                write_buf(buf, "%s", KeysymText(keymap->ctx, entry->u.ئاچقۇچ_Sym));
            وإلا
                write_buf(buf, "0x%"PRIx32, entry->u.ئاچقۇچ_Sym);
        } وإلا {
            ثابت xkb_atom_t name = (substitutions == عدم)
                ? entry->u.ئاچقۇچ_Name
                : substitute_name(substitutions, entry->u.ئاچقۇچ_Name);
            write_buf(buf, "%s", ئاچقۇچ_Name_Text(keymap->ctx, name));
        }
    }
    إذا (current_mods) {
        copy_to_buf(buf, " };\n");
    }

exit:
    darray_free(modmaps);
    أرجع true;
}

ساكن bool
write_symbols(ثابت بنية xkb_keymap *keymap,
              ثابت بنية xkb_keymap_serialize_config *config,
              ثابت ئاچقۇچ_name_substitutions *substitutions,
              xkb_layout_index_t max_groups, bool some_interp,
              بنية buf *buf)
{
    ثابت bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ثابت bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_قىممەتلەر);
    ثابت bool explicit_ئاچقۇچ_قىممەتلەر =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ئاچقۇچ_قىممەتلەر);
    ثابت bool drop_unused = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ثابت bool drop_interprets = (explicit_ئاچقۇچ_قىممەتلەر && drop_unused);
    /* TODO: print all default values if explicit flag is set */

    إذا (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    وإلا
        copy_to_buf(buf, "xkb_symbols {\n");

    bool has_group_names = false;
    xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
    لكل (xkb_layout_index_t group = 0; group < keymap->رەقەم_group_names; group++) {
        إذا (!((UINT32_C(1) << group) & config->layouts))
            استمر;

        new_group++;

        إذا (keymap->group_names[group]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_group + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->group_names[group]));
            copy_to_buf(buf, ";\n");
            has_group_names = true;
        }
    }
    إذا (has_group_names)
        copy_to_buf(buf, "\n");

    إذا (defaults && !write_actions_defaults(keymap, config->format, buf))
        أرجع false;

    بنية buf buf2 = { عدم, 0, 0 };
    ثابت بنية xkb_ئاچقۇچ *ئاچقۇچ;
    xkb_ئاچقۇچلار_foreach(ئاچقۇچ, keymap) {
        /* Skip keys with no explicit values */
        إذا (ئاچقۇچ->explicit &&
            !write_ئاچقۇچ(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_ئاچقۇچ_قىممەتلەر, pretty, buf,
                       &buf2, ئاچقۇچ))
        {
            حرر(buf2.buf);
            أرجع false;
        }
    }
    حرر(buf2.buf);

    إذا (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        أرجع false;

    copy_to_buf(buf, "};\n\n");
    أرجع true;
}

ساكن تعداد xkb_status
write_keymap(ثابت بنية xkb_keymap *keymap,
             ثابت بنية xkb_keymap_serialize_config *config,
             بنية buf *buf, بنية xkb_keymap_serialize_result *result)
{
    ئاچقۇچ_name_substitutions substitutions = darray_new();
    إذا (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_ئاچقۇچلار(keymap, &substitutions))
    {
        أرجع false;
    }

    ثابت ئاچقۇچ_name_substitutions * ثابت substitutions_ptr =
        (darray_empty(substitutions)) ? عدم : &substitutions;

    ثابت xkb_layout_index_t max_groups = format_max_groups(config->format);

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

تعداد xkb_status
text_v1_keymap_serialize(
        ثابت بنية xkb_keymap *keymap,
        ثابت بنية xkb_keymap_serialize_config *config,
        بنية xkb_keymap_serialize_result *result
)
{
    بنية buf buf = { عدم, 0, 0 };

    ثابت تعداد xkb_status status =
        write_keymap(keymap, config, &buf, result);
    إذا (status != XKB_SUCCESS) {
        حرر(buf.buf);
        result->serialized = عدم;
        أرجع status;
    }

    result->serialized = buf.buf;
    result->length = buf.size + 1;
    result->layouts = config->layouts;

    أرجع XKB_SUCCESS;
}
