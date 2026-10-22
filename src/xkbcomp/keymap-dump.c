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

#அந்தர்பாவ "config.h"

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>

#அந்தர்பாவ "xkbcommon/xkbcommon-status.h"
#அந்தர்பாவ "xkbcommon/xkbcommon-keysyms.h"
#அந்தர்பாவ "xkbcommon/xkbcommon.h"
#அந்தர்பாவ "atom.h"
#அந்தர்பாவ "action.h"
#அந்தர்பாவ "context.h"
#அந்தர்பாவ "darray.h"
#அந்தர்பாவ "keymap.h"
#அந்தர்பாவ "messages-codes.h"
#அந்தர்பாவ "text.h"
#அந்தர்பாவ "util-numbers.h"
#அந்தர்பாவ "xkbcomp-priv.h"

#define BUF_CHUNK_SIZE 4096

கட்டமைப்பு buf {
    எழுத்து *buf;
    size_t size;
    size_t alloc;
};

#define xkb_abs(n) _Generic((n),                \
                            முழு: abs,           \
                            long முழு: labs,     \
                            இயல்பு: llabs )((n))

நிலையான bool
do_realloc(கட்டமைப்பு buf *buf, size_t at_least)
{
    buf->alloc += BUF_CHUNK_SIZE;
    எனில் (at_least >= BUF_CHUNK_SIZE)
        buf->alloc += at_least;

    எழுத்து *மாறிலி new = realloc(buf->buf, buf->alloc);
    எனில் (!new)
        திரும்பு false;

    buf->buf = new;
    திரும்பு true;
}

ATTR_PRINTF(2, 3) நிலையான bool
check_write_buf(கட்டமைப்பு buf *buf, மாறிலி எழுத்து *fmt, ...)
{
    va_list args;
    முழு printed;
    size_t available;

    available = buf->alloc - buf->size;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    ப்ரதிஜ்ஞா(buf->buf || available == 0);
    va_start(args, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->size : buf->buf, available,
                        fmt, args);
    va_end(args);

    எனில் (printed < 0) {
        /* Some error */
        செல் err;
    } இல்லையெனில் எனில் ((size_t)printed >= available) {
        /* Not enough space: need to realloc */
        எனில் (!do_realloc(buf, (size_t)printed + 1))
            செல் err;

        /* The buffer has enough space now. */
        available = buf->alloc - buf->size;
        ப்ரதிஜ்ஞா(buf->buf && (size_t) printed < available);
        va_start(args, fmt);
        printed = vsnprintf(buf->buf + buf->size, available, fmt, args);
        va_end(args);

        எனில் (printed < 0 || (size_t) printed >= available)
            செல் err;
    }

    buf->size += (size_t)printed;
    திரும்பு true;

err:
    விடுவி(buf->buf);
    buf->buf = வெற்று;
    திரும்பு false;
}

#define write_buf(buf, ...) செய் { \
    எனில் (!check_write_buf(buf, __VA_ARGS__)) \
        திரும்பு false; \
} போது (0)

நிலையான bool
check_copy_to_buf(கட்டமைப்பு buf *buf, மாறிலி எழுத்து* source, size_t len)
{
    எனில் (len == 0)
        திரும்பு true;

    மாறிலி size_t available = buf->alloc - buf->size;
    எனில் (len >= available &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        விடுவி(buf->buf);
        buf->buf = வெற்று;
        திரும்பு false;
    }

    memcpy(buf->buf + buf->size, source, len);
    buf->size += len;
    /* Append NULL byte */
    buf->buf[buf->size] = '\0';
    திரும்பு true;
}

#define copy_to_buf_len(buf, source, len) செய் { \
    எனில் (!check_copy_to_buf(buf, source, len)) \
        திரும்பு false; \
} போது (0)

#define copy_to_buf(buf, source) \
    copy_to_buf_len(buf, source, அளவு(source) - 1)

நிலையான bool
check_write_string_literal(கட்டமைப்பு buf *buf, மாறிலி எழுத்து* string)
{
    copy_to_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    size_t pending_start = 0;
    size_t current = 0;
    எழுத்து escape_buffer[] = {'\\', '0', '0', '0', '0'};
    க்கு (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        தேர்வு (string[current]) {
        நிலை '\n':
            /* `\n` would break strings */
            escape_buffer[1] = 'n';
            escape_len = 2;
            நிறுத்து;
        நிலை '\\':
            /* `\` would create escape sequences */
            escape_buffer[1] = '\\';
            escape_len = 2;
            நிறுத்து;
        இயல்பு:
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
            எனில் (unlikely((unsigned எழுத்து)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_buffer[1] = '0';
                escape_buffer[2] = '0';
                escape_buffer[3] = (எழுத்து)('0' + (string[current] >> 3));
                escape_buffer[4] = (எழுத்து)('0' + (string[current] & 0x7));
                escape_len = 5;
            } இல்லையெனில் {
                /* Expand the chunk */
                தொடர்;
            }
        }
        /* Write pending chunk */
        ப்ரதிஜ்ஞா(current >= pending_start);
        copy_to_buf_len(buf, string + pending_start, current - pending_start);
        /* Write escape sequence */
        copy_to_buf_len(buf, escape_buffer, escape_len);
        pending_start = current + 1;
    }
    /* Write remaining chunk */
    ப்ரதிஜ்ஞா(current >= pending_start);
    copy_to_buf_len(buf, string + pending_start, current - pending_start);

    copy_to_buf(buf, "\"");
    திரும்பு true;
}

#define write_buf_string_literal(buf, string) செய் { \
    எனில் (!check_write_string_literal(buf, string)) \
        திரும்பு false; \
} போது (0)

நிலையான வெறுமை
delete_last_char(கட்டமைப்பு buf *buf, எழுத்து c)
{
    எனில் (buf->size && buf->buf[buf->size - 1] == c)
        buf->buf[--buf->size] = '\0';
}

/** Entry for a name substituion LUT */
கட்டமைப்பு சாவி_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
நிலையான முழு
cmp_சாவி_name_substitution(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
    மாறிலி xkb_atom_t s1 = ((கட்டமைப்பு சாவி_name_substitution *) a)->source;
    மாறிலி xkb_atom_t s2 = ((கட்டமைப்பு சாவி_name_substitution *) b)->source;
    திரும்பு (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

வகைவரை darray(கட்டமைப்பு சாவி_name_substitution) சாவி_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
நிலையான bool
rename_long_சாவிகள்(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
                 சாவி_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    எண்ணிக்கை {
        first = '!',
        last = '0',
        invalid = '\0'
    };
    எழுத்து prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', last
    };
    static_assert(அளவு(prefixes) == last - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி;
    xkb_சாவிகள்_foreach(சாவி, keymap) {
        எனில் (சாவி->name == XKB_ATOM_NONE)
            தொடர்;

        மாறிலி எழுத்து * மாறிலி name = xkb_atom_text(keymap->ctx, சாவி->name);
        மாறிலி size_t len = strlen_safe(name);
        எனில் (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (கட்டமைப்பு சாவி_name_substitution) {
                .source = சாவி->name,
                .keycode = சாவி->keycode,
                .target = சாவி->name
            });
        } இல்லையெனில் எனில் (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    க்கு (darray_size_t a = 0; a < keymap->இலக்கம்_சாவி_aliases; a++) {
        மாறிலி கட்டமைப்பு xkb_சாவி_alias * மாறிலி alias = &keymap->சாவி_aliases[a];
        மாறிலி எழுத்து * மாறிலி name = xkb_atom_text(keymap->ctx, alias->alias);
        மாறிலி size_t len = strlen_safe(name);
        எனில் (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (கட்டமைப்பு சாவி_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } இல்லையெனில் எனில் (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    எனில் darray_empty(*substitutions) {
        /* Nothing to do */
        திரும்பு true;
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
    எழுத்து *prefix = prefixes;
    மாறிலி எழுத்து *prefix_max = prefixes + அளவு(prefixes) - 1;
    போது (*prefix == invalid && prefix < prefix_max)
        prefix++;

    /* Keycode prefix: prefer ‘0’ if available */
    எழுத்து keycode_prefix;
    எனில் (prefixes[last - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_prefix = last;
        prefixes[last - first] = invalid;
    } இல்லையெனில் {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_prefix = *prefix;
        எனில் (unlikely(keycode_prefix == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            திரும்பு true;
        }
        /* Update current prefix */
        *prefix = invalid;
        போது (*prefix == invalid && prefix < prefix_max)
            prefix++;
    }

    /* Proceed renaming */
    uint16_t next_id = 1;
    கட்டமைப்பு சாவி_name_substitution *sub;
    எழுத்து buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        எனில் (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, அளவு(buf), "%c%03"PRIx32,
                     keycode_prefix, sub->keycode);
            மாறிலி xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, அளவு(buf) - 2);
            எனில் (target == XKB_ATOM_NONE)
                செல் error;
            sub->target = target;
        } இல்லையெனில் எனில் (*prefix != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, அளவு(buf), "%c%03"PRIx16, *prefix, next_id);
            மாறிலி xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, அளவு(buf) - 2);
            எனில் (target == XKB_ATOM_NONE)
                செல் error;
            sub->target = target;
            எனில் (++next_id > 0xfff) {
                *prefix = invalid;
                /* Get next available prefix */
                போது (*prefix == invalid && prefix < prefix_max)
                    prefix++;
                /* Reset counter */
                next_id = 1;
            }
        } இல்லையெனில் {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->target = sub->source;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_பொருட்கள்(*substitutions), darray_size(*substitutions),
          அளவு(*darray_பொருட்கள்(*substitutions)), &cmp_சாவி_name_substitution);

    திரும்பு true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate key name substitution\n");
    திரும்பு false;
}

/** Substitute a key name with a valid X11 key name */
நிலையான xkb_atom_t
substitute_name(மாறிலி சாவி_name_substitutions *substitutions, xkb_atom_t name)
{
    /*இருமத் தேடல்*/
    darray_size_t first = 0;
    darray_size_t last = darray_size(*substitutions);

    போது (last > first) {
        darray_size_t mid = first + (last - 1 - first) / 2;
        மாறிலி xkb_atom_t source = darray_item(*substitutions, mid).source;
        எனில் (source < name)
            first = mid + 1;
        இல்லையெனில் எனில் (source > name)
            last = mid;
        இல்லையெனில் /* found it */
            திரும்பு darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    திரும்பு name;
}

நிலையான bool
write_vmods(மாறிலி கட்டமைப்பு xkb_keymap *keymap, எண்ணிக்கை xkb_keymap_format format,
            bool explicit, கட்டமைப்பு buf *buf)
{
    மாறிலி கட்டமைப்பு xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        எனில் (!has_some) {
            copy_to_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } இல்லையெனில் {
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
        மாறிலி bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        எனில் ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
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

    எனில் (has_some)
        copy_to_buf(buf, ";\n\n");

    திரும்பு true;
}

நிலையான bool
write_keycodes(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
               மாறிலி சாவி_name_substitutions *substitutions,
               எண்ணிக்கை xkb_keymap_serialize_flags flags, கட்டமைப்பு buf *buf)
{
    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி;
    xkb_led_index_t idx;
    மாறிலி கட்டமைப்பு xkb_led *led;

    மாறிலி bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    எனில் (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    இல்லையெனில்
        copy_to_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_சாவி_code, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_சாவி_code, 255));

    xkb_சாவிகள்_foreach(சாவி, keymap) {
        எனில் (சாவி->name == XKB_ATOM_NONE)
            தொடர்;

        மாறிலி xkb_atom_t name = (substitutions == வெற்று)
            ? சாவி->name
            : substitute_name(substitutions, சாவி->name);
        எனில் (pretty)
            write_buf(buf, "\t%-20s", சாவி_Name_Text(keymap->ctx, name));
        இல்லையெனில்
            write_buf(buf, "\t%s", சாவி_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", சாவி->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        எனில் (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_to_buf(buf, ";\n");
        }

    க்கு (darray_size_t i = 0; i < keymap->இலக்கம்_சாவி_aliases; i++) {
        மாறிலி xkb_atom_t alias = (substitutions == வெற்று)
            ? keymap->சாவி_aliases[i].alias
            : substitute_name(substitutions, keymap->சாவி_aliases[i].alias);
        மாறிலி xkb_atom_t real = (substitutions == வெற்று)
            ? keymap->சாவி_aliases[i].real
            : substitute_name(substitutions, keymap->சாவி_aliases[i].real);

        எனில் (pretty)
            write_buf(buf, "\talias %-14s",
                      சாவி_Name_Text(keymap->ctx, alias));
        இல்லையெனில்
            write_buf(buf, "\talias %s",
                      சாவி_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  சாவி_Name_Text(keymap->ctx, real));
    }

    copy_to_buf(buf, "};\n\n");
    திரும்பு true;
}

நிலையான bool
write_types(மாறிலி கட்டமைப்பு xkb_keymap *keymap, எண்ணிக்கை xkb_keymap_format format,
            எண்ணிக்கை xkb_keymap_serialize_flags flags, கட்டமைப்பு buf *buf)
{
    எனில் (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    இல்லையெனில்
        copy_to_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    எனில் (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        திரும்பு false;

    மாறிலி bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    க்கு (darray_size_t i = 0; i < keymap->இலக்கம்_types; i++) {
        மாறிலி கட்டமைப்பு xkb_சாவி_type * மாறிலி type = &keymap->types[i];
        எனில் (!type->required && drop_unused)
            தொடர்;

        copy_to_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_to_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        க்கு (darray_size_t j = 0; j < type->இலக்கம்_entries; j++) {
            மாறிலி எழுத்து *str;
            மாறிலி கட்டமைப்பு xkb_சாவி_type_entry *entry = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            எனில் (entry->level == 0 && entry->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_மதிப்புகள்))
                தொடர்;

            str = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              entry->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, entry->level + 1);

            எனில் (entry->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           entry->preserve.mods));
        }

        க்கு (xkb_level_index_t n = 0; n < type->இலக்கம்_level_names; n++)
            எனில் (type->level_names[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_names[n]));
                copy_to_buf(buf, ";\n");
            }

        copy_to_buf(buf, "\t};\n");
    }

    copy_to_buf(buf, "};\n\n");
    திரும்பு true;
}

நிலையான bool
write_led_map(மாறிலி கட்டமைப்பு xkb_keymap *keymap, எண்ணிக்கை xkb_keymap_format format,
              bool defaults, கட்டமைப்பு buf *buf, மாறிலி கட்டமைப்பு xkb_led *led)
{
    copy_to_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_to_buf(buf, " {\n");

    எனில் (led->which_groups) {
        எனில் (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      LedStateMaskText(keymap->ctx, groupComponentMaskNames,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    எனில் (led->which_mods) {
        எனில் (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      LedStateMaskText(keymap->ctx, modComponentMaskNames,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    எனில் (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  ControlMaskText(keymap->ctx, format, led->ctrls));
    }

    copy_to_buf(buf, "\t};\n");
    திரும்பு true;
}

நிலையான மாறிலி எழுத்து *
affect_lock_text(எண்ணிக்கை xkb_action_flags flags, bool show_both)
{
    தேர்வு (flags & (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK)) {
    நிலை 0:
        திரும்பு show_both ? ",affect=both" : "";
    நிலை ACTION_LOCK_NO_UNLOCK:
        திரும்பு ",affect=lock";
    நிலை ACTION_LOCK_NO_LOCK:
        திரும்பு ",affect=unlock";
    நிலை ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK:
        திரும்பு ",affect=neither";
    இயல்பு:
        திரும்பு "";
    }
}

#define குறியீடு_PADDING 15
#define ACTION_PADDING 30

நிலையான bool
write_action(மாறிலி கட்டமைப்பு xkb_keymap *keymap, எண்ணிக்கை xkb_keymap_format format,
             xkb_layout_index_t max_groups, கட்டமைப்பு buf *buf,
             மாறிலி ஒன்றிப்பு xkb_action *action,
             மாறிலி எழுத்து *prefix, மாறிலி எழுத்து *suffix)
{
    மாறிலி எழுத்து *type;
    மாறிலி எழுத்து *args = வெற்று;

    எனில் (!prefix)
        prefix = "";
    எனில் (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    தேர்வு (action->type) {
    நிலை ACTION_TYPE_MOD_SET:
    நிலை ACTION_TYPE_MOD_LATCH:
    நிலை ACTION_TYPE_MOD_LOCK:
        எனில் (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            args = "modMapMods";
        இல்லையெனில்
            args = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        எனில் (unlockOnPress && !isModsUnLockOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->group.flags & ACTION_LATCH_ON_PRESS);
        எனில் (latchOnPress && !isModsLatchOnPressSupported(format)) {
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
        நிறுத்து;

    நிலை ACTION_TYPE_GROUP_SET:
    நிலை ACTION_TYPE_GROUP_LATCH:
    நிலை ACTION_TYPE_GROUP_LOCK:
        எனில் ((uint32_t) xkb_abs(action->group.group) <
            (max_groups + !(action->group.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool lockOnRelease = action->type == ACTION_TYPE_GROUP_LOCK &&
                                 (action->group.flags & ACTION_LOCK_ON_RELEASE);
            எனில் (lockOnRelease && !isGroupLockOnReleaseSupported(format)) {
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
        } இல்லையெனில் {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->group.group +
                       !!(action->group.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            செல் void_action;
        }
        நிறுத்து;

    நிலை ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", prefix, type, suffix);
        நிறுத்து;

    நிலை ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", prefix, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        நிறுத்து;

    நிலை ACTION_TYPE_PTR_LOCK:
        args = affect_lock_text(action->btn.flags, true);
        /* fallthrough */
    நிலை ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", prefix, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        எனில் (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        இல்லையெனில்
            copy_to_buf(buf, "default");
        எனில் (action->btn.மொத்தம்) {
            ப்ரதிஜ்ஞா(action->type != ACTION_TYPE_PTR_LOCK);
            write_buf(buf, ",count=%u", action->btn.மொத்தம்);
        }
        எனில் (args)
            write_buf(buf, "%s", args);
        write_buf(buf, ")%s", suffix);
        நிறுத்து;

    நிலை ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", prefix, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.மதிப்பு >= 0) ? "+" : "",
                  action->dflt.மதிப்பு);
        write_buf(buf, ")%s", suffix);
        நிறுத்து;

    நிலை ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", prefix, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        நிறுத்து;

    நிலை ACTION_TYPE_CTRL_SET:
    நிலை ACTION_TYPE_CTRL_LOCK:
        write_buf(buf, "%s%s(controls=%s%s)%s", prefix, type,
                  ControlMaskText(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_LOCK
                    ? affect_lock_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        நிறுத்து;

    நிலை ACTION_TYPE_REDIRECT_சாவி: {
        write_buf(buf, "%s%s(", prefix, type);
        மாறிலி கட்டமைப்பு xkb_சாவி * மாறிலி சாவி = Xkb_சாவி(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        எனில் (சாவி)
            write_buf(buf, "keycode=%s", சாவி_Name_Text(keymap->ctx, சாவி->name));
        எனில் (action->redirect.affect_mods) {
            xkb_mod_mask_t mask;
            mask = (action->redirect.affect_mods & action->redirect.mods);
            எனில் (mask)
                write_buf(buf, ",modifiers=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
            mask = (action->redirect.affect_mods & ~action->redirect.mods);
            எனில் (mask)
                write_buf(buf, ",clearMods=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
        }
        write_buf(buf, ")%s", suffix);
        நிறுத்து;
    }

    நிலை ACTION_TYPE_UNKNOWN:
    நிலை ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", prefix, suffix);
        நிறுத்து;

    நிலை ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        எனில் (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      prefix, suffix);
        இல்லையெனில்
            write_buf(buf, "%sVoidAction()%s", prefix, suffix);
        நிறுத்து;

    இயல்பு:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_இலக்கம்_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        ப்ரதிஜ்ஞா(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  prefix, type, action->type, action->priv.data[0],
                  action->priv.data[1], action->priv.data[2],
                  action->priv.data[3], action->priv.data[4],
                  action->priv.data[5], action->priv.data[6],
                  suffix);
        நிறுத்து;
    }

    திரும்பு true;
}

நிலையான மாறிலி ஒன்றிப்பு xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

நிலையான bool
write_actions(மாறிலி கட்டமைப்பு xkb_keymap *keymap, எண்ணிக்கை xkb_keymap_format format,
              xkb_layout_index_t max_groups, கட்டமைப்பு buf *buf, கட்டமைப்பு buf *buf2,
              மாறிலி கட்டமைப்பு xkb_சாவி *சாவி, xkb_layout_index_t group)
{
    நிலையான மாறிலி ஒன்றிப்பு xkb_action noAction = { .type = ACTION_TYPE_NONE };

    க்கு (xkb_level_index_t level = 0; level < Xkb_சாவி_இலக்கம்_Levels(சாவி, group);
         level++) {

        எனில் (level != 0)
            copy_to_buf(buf, ", ");

        மாறிலி ஒன்றிப்பு xkb_action *actions = வெற்று;
        xkb_action_மொத்தம்_t மொத்தம் = xkb_keymap_சாவி_get_actions_by_level(
            keymap, சாவி, group, level, &actions
        );

        எனில் (மொத்தம் > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            மொத்தம் = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->size = 0;
        எனில் (மொத்தம் == 0) {
            எனில் (!write_action(keymap, format, max_groups,
                              buf2, &noAction, வெற்று, வெற்று))
                திரும்பு false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        இல்லையெனில் எனில் (மொத்தம் == 1) {
            எனில் (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), வெற்று, வெற்று))
                திரும்பு false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        இல்லையெனில் {
            copy_to_buf(buf2, "{ ");
            க்கு (xkb_action_மொத்தம்_t k = 0; k < மொத்தம்; k++) {
                எனில் (k != 0)
                    copy_to_buf(buf2, ", ");
                size_t old_size = buf2->size;
                எனில் (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), வெற்று, வெற்று))
                    திரும்பு false;
                /* Check if padding is necessary */
                எனில் (buf2->size >= old_size + ACTION_PADDING)
                    தொடர்;
                /* Compute and write padding, then write the action again */
                மாறிலி முழு padding = (முழு)(old_size + ACTION_PADDING - buf2->size);
                buf2->size = old_size;
                write_buf(buf2, "%*s", padding, "");
                எனில் (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), வெற்று, வெற்று))
                    திரும்பு false;
            }
            copy_to_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    திரும்பு true;
}

நிலையான bool
write_action_defaults(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
                      எண்ணிக்கை xkb_keymap_format format,
                      மாறிலி ஒன்றிப்பு xkb_action *action,
                      கட்டமைப்பு buf *buf)
{
    மாறிலி எழுத்து *type = ActionTypeText(action->type);

#define PREFIX "\t"

    தேர்வு (action->type) {
    நிலை ACTION_TYPE_MOD_SET:
    நிலை ACTION_TYPE_MOD_LATCH:
    நிலை ACTION_TYPE_MOD_LOCK:
        ப்ரதிஜ்ஞா(action->mods.flags == 0);
        ப்ரதிஜ்ஞா(action->mods.mods.mods == 0);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        எனில் (action->type != ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        எனில் (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, PREFIX"%s.latchOnPress = false;\n", type);
        எனில் (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        எனில் (action->type == ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        எனில் (isModsUnLockOnPressSupported(format))
            write_buf(buf, PREFIX"%s.unlockOnPress = false;\n", type);
        நிறுத்து;

    நிலை ACTION_TYPE_GROUP_SET:
    நிலை ACTION_TYPE_GROUP_LATCH:
    நிலை ACTION_TYPE_GROUP_LOCK:
        ப்ரதிஜ்ஞா(action->group.flags == 0);
        ப்ரதிஜ்ஞா(action->group.group == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.group = +0;\n", type);
        எனில் (action->type != ACTION_TYPE_GROUP_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        எனில் (action->type == ACTION_TYPE_GROUP_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        எனில் (action->type == ACTION_TYPE_GROUP_LOCK &&
            isGroupLockOnReleaseSupported(format))
            write_buf(buf, PREFIX"%s.lockOnRelease = false;\n", type);
        நிறுத்து;


    நிலை ACTION_TYPE_PTR_MOVE:
        ப்ரதிஜ்ஞா(action->ptr.x == 0);
        ப்ரதிஜ்ஞா(action->ptr.y == 0);
        ப்ரதிஜ்ஞா(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.x = +0;\n", type);
        write_buf(buf, PREFIX"%s.y = +0;\n", type);
        write_buf(buf, PREFIX"%s.accel = true;\n", type);
        நிறுத்து;

    நிலை ACTION_TYPE_PTR_LOCK:
    நிலை ACTION_TYPE_PTR_BUTTON:
        ப்ரதிஜ்ஞா(action->btn.flags == 0);
        ப்ரதிஜ்ஞா(action->btn.button == 0);
        ப்ரதிஜ்ஞா(action->btn.மொத்தம் == 0);
        write_buf(buf, PREFIX"%s.button = 0;\n", type);
        write_buf(buf, PREFIX"%s.count = 0;\n", type);
        எனில் (action->type == ACTION_TYPE_PTR_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        நிறுத்து;

    நிலை ACTION_TYPE_PTR_DEFAULT:
        ப்ரதிஜ்ஞா(action->dflt.flags == 0);
        ப்ரதிஜ்ஞா(action->dflt.மதிப்பு == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.button = +1;\n", type);
        நிறுத்து;

    நிலை ACTION_TYPE_SWITCH_VT:
        ப்ரதிஜ்ஞா(action->screen.flags == ACTION_SAME_SCREEN);
        ப்ரதிஜ்ஞா(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.screen = +0;\n", type);
        write_buf(buf, PREFIX"%s.same = true;\n", type);
        நிறுத்து;

    நிலை ACTION_TYPE_CTRL_SET:
    நிலை ACTION_TYPE_CTRL_LOCK:
        ப்ரதிஜ்ஞா(action->ctrls.flags == 0);
        ப்ரதிஜ்ஞா(action->ctrls.ctrls == 0);
        write_buf(buf, PREFIX"%s.controls = 0;\n", type);
        எனில் (action->type == ACTION_TYPE_CTRL_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        நிறுத்து;

    நிலை ACTION_TYPE_REDIRECT_சாவி:
        ப்ரதிஜ்ஞா(action->redirect.keycode == keymap->redirect_சாவி_auto);
        ப்ரதிஜ்ஞா(action->redirect.affect_mods == 0);
        ப்ரதிஜ்ஞா(action->redirect.mods == 0);
        write_buf(buf, PREFIX"%s.keycode = auto;\n", type);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        write_buf(buf, PREFIX"%s.clearMods = 0;\n", type);
        நிறுத்து;

    இயல்பு:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_இலக்கம்_ENTRIES,
                      "Missing action type");
    }

#undef PREFIX

    திரும்பு true;
}

நிலையான bool
write_actions_defaults(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
                       எண்ணிக்கை xkb_keymap_format format, கட்டமைப்பு buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    க்கு (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_இலக்கம்_ENTRIES; a++) {
        மாறிலி ஒன்றிப்பு xkb_action * restrict மாறிலி action = &defaults.actions[a];
        எனில் (!write_action_defaults(keymap, format, action, buf))
            திரும்பு false;
    }
    copy_to_buf(buf, "\n");

    திரும்பு true;
}

நிலையான மாறிலி கட்டமைப்பு xkb_sym_interpret fallback_interpret = {
    .sym = XKB_சாவி_Void_குறியீடு,
    .required = true,
    .repeat = FALLBACK_INTERPRET_சாவி_REPEAT,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .இலக்கம்_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

நிலையான bool
write_compat(மாறிலி கட்டமைப்பு xkb_keymap * restrict keymap,
             எண்ணிக்கை xkb_keymap_format format, xkb_layout_index_t max_groups,
             எண்ணிக்கை xkb_keymap_serialize_flags flags, bool * restrict some_interp,
             கட்டமைப்பு buf *buf)
{
    மாறிலி bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    மாறிலி bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_மதிப்புகள்);
    மாறிலி bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    மாறிலி bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_சாவி_மதிப்புகள்) && drop_unused);
    /* TODO: print all default values if the explicit flag is set */

    எனில் (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    இல்லையெனில்
        copy_to_buf(buf, "xkb_compatibility {\n");

    எனில் (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        திரும்பு false;

    எனில் (defaults && !write_actions_defaults(keymap, format, buf))
        திரும்பு false;

    copy_to_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_to_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    மாறிலி bool use_fallback_interpret =
        (!keymap->இலக்கம்_sym_interprets || drop_interprets);
    மாறிலி darray_size_t இலக்கம்_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->இலக்கம்_sym_interprets);
    மாறிலி கட்டமைப்பு xkb_sym_interpret* மாறிலி sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    க்கு (darray_size_t i = 0; i < இலக்கம்_sym_interprets; i++) {
        மாறிலி கட்டமைப்பு xkb_sym_interpret *si = &sym_interprets[i];
        எனில் (!si->required && drop_unused)
            தொடர்;

        *some_interp = true;

        copy_to_buf(buf, "\tinterpret ");
        எனில் (!si->sym) {
            copy_to_buf(buf, "Any");
        } இல்லையெனில் எனில் (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } இல்லையெனில் {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->match),
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        எனில் (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      ModIndexText(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        எனில் (si->level_one_only) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } இல்லையெனில் எனில் (defaults) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        எனில் (si->repeat) {
            copy_to_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } இல்லையெனில் எனில் (defaults) {
            copy_to_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        மாறிலி ஒன்றிப்பு xkb_action *action = வெற்று;
        xkb_action_மொத்தம்_t action_மொத்தம் = si->இலக்கம்_actions;
        எனில் (action_மொத்தம் <= 1) {
            action = &si->a.action;
        } இல்லையெனில் எனில் (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_மொத்தம் = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        எனில் (action_மொத்தம் > 1) {
            copy_to_buf(buf, "\n\t\taction= {");
            மாறிலி எழுத்து suffix[] = ", ";
            க்கு (xkb_action_மொத்தம்_t k = 0; k < si->இலக்கம்_actions; k++) {
                எனில் (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    திரும்பு false;
            }
            buf->size -= அளவு(suffix) - 1; /* trailing comma */
            copy_to_buf(buf, "};");
            has_explicit_properties = true;
        } இல்லையெனில் எனில் (action_மொத்தம் == 1) {
            எனில் (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                திரும்பு false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    எனில் (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        இலக்கம்_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].match == fallback_interpret.match &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].இலக்கம்_actions == fallback_interpret.இலக்கம்_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    மாறிலி கட்டமைப்பு xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        எனில் ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            திரும்பு false;
        }
    }

    copy_to_buf(buf, "};\n\n");

    திரும்பு true;
}

நிலையான bool
write_keysyms(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
              கட்டமைப்பு buf *buf, கட்டமைப்பு buf *buf2,
              மாறிலி கட்டமைப்பு xkb_சாவி *சாவி, xkb_layout_index_t group,
              bool pretty, bool show_actions)
{
    மாறிலி unsigned முழு padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : குறியீடு_PADDING)
        : 0;
    க்கு (xkb_level_index_t level = 0; level < Xkb_சாவி_இலக்கம்_Levels(சாவி, group);
         level++) {
        மாறிலி xkb_keysym_t *syms;
        முழு இலக்கம்_syms;

        எனில் (level != 0)
            copy_to_buf(buf, ", ");

        இலக்கம்_syms = xkb_keymap_சாவி_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (கட்டமைப்பு xkb_keymap *)keymap,
            சாவி->keycode, group, level, &syms
        );

        மாறிலி xkb_keysym_t no_குறியீடு = XKB_சாவி_No_குறியீடு;
        எனில் (இலக்கம்_syms == 0) {
            இலக்கம்_syms = 1;
            syms = &no_குறியீடு;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        எனில் (இலக்கம்_syms == 1) {
            எனில் (pretty || syms[0] == XKB_சாவி_No_குறியீடு)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            இல்லையெனில்
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } இல்லையெனில் {
            எனில் (pretty) {
                buf2->size = 0;
                copy_to_buf(buf2, "{ ");
                க்கு (முழு s = 0; s < இலக்கம்_syms; s++) {
                    எனில் (s != 0)
                        copy_to_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_to_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } இல்லையெனில் {
                copy_to_buf(buf, "{");
                க்கு (முழு s = 0; s < இலக்கம்_syms; s++) {
                    எனில் (s != 0)
                        copy_to_buf(buf, ",");
                    எனில் (syms[s] == XKB_சாவி_No_குறியீடு)
                        copy_to_buf(buf, "NoSymbol");
                    இல்லையெனில்
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_to_buf(buf, "}");
            }
        }
    }

    திரும்பு true;
}

நிலையான bool
write_சாவி(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
          மாறிலி கட்டமைப்பு xkb_keymap_serialize_config *config,
          மாறிலி சாவி_name_substitutions *substitutions,
          xkb_layout_index_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          கட்டமைப்பு buf *buf, கட்டமைப்பு buf *buf2, மாறிலி கட்டமைப்பு xkb_சாவி *சாவி)
{
    bool simple = true;

    xkb_layout_mask_t layouts = config->layouts;
    எனில் (popcount32(layouts) != keymap->இலக்கம்_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_index_t l = 0;
        க்கு (xkb_layout_mask_t ls = layouts; ls; ls >>= 1, l++) {
            எனில் (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                மாறிலி xkb_layout_index_t effective =
                    xkb_keymap_சாவி_effective_layout(சாவி, l);
                எனில் (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } இல்லையெனில் {
        layouts &= ((UINT64_C(1) << சாவி->இலக்கம்_groups) - 1);
    }
    மாறிலி xkb_layout_index_t groups_மொத்தம் = popcount32(layouts);

    மாறிலி xkb_atom_t name = (substitutions == வெற்று)
        ? சாவி->name
        : substitute_name(substitutions, சாவி->name);

    எனில் (pretty)
        write_buf(buf, "\tkey %-20s {", சாவி_Name_Text(keymap->ctx, name));
    இல்லையெனில்
        write_buf(buf, "\tkey %s {", சாவி_Name_Text(keymap->ctx, name));

    எனில் ((சாவி->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        எனில் (!explicit) {
            க்கு (xkb_layout_index_t group = 1; group < சாவி->இலக்கம்_groups; group++) {
                எனில் (!((UINT32_C(1) << group) & layouts))
                    தொடர்;

                எனில் (சாவி->groups[group].type != சாவி->groups[0].type) {
                    multi_type = true;
                    நிறுத்து;
                }
            }
        }

        எனில் (multi_type) {
            xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
            க்கு (xkb_layout_index_t group = 0; group < சாவி->இலக்கம்_groups; group++) {
                எனில் (!((UINT32_C(1) << group) & layouts))
                    தொடர்;

                new_group++;

                எனில் (!சாவி->groups[group].explicit_type && !explicit)
                    தொடர்;

                மாறிலி கட்டமைப்பு xkb_சாவி_type * மாறிலி type = சாவி->groups[group].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_group + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_to_buf(buf, ",");
            }
        }
        இல்லையெனில் {
            மாறிலி கட்டமைப்பு xkb_சாவி_type * மாறிலி type = சாவி->groups[0].type;
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
    மாறிலி bool explicit_actions = (சாவி->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #define require_explicit(சாவி, prop,                                       \
                             மதிப்பு, initial_மதிப்பு, default_interpret_மதிப்பு) ( \
        /* Explicit value in the source */                                    \
        ((சாவி)->explicit & (prop)) ||                                         \
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
                ((சாவி)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (மதிப்பு) != (initial_மதிப்பு) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_மதிப்பு) != (default_interpret_மதிப்பு)                  \
            )                                                                 \
        )                                                                     \
    )

    எனில் (require_explicit(சாவி, EXPLICIT_REPEAT, சாவி->repeats,
                         DEFAULT_சாவி_REPEAT, DEFAULT_INTERPRET_சாவி_REPEAT)) {
        எனில் (சாவி->repeats)
            copy_to_buf(buf, "\n\t\trepeat= Yes,");
        இல்லையெனில்
            copy_to_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    எனில் (require_explicit(சாவி, EXPLICIT_VMODMAP, சாவி->vmodmap,
                         (xkb_mod_mask_t) DEFAULT_சாவி_VMODMAP,
                         (xkb_mod_mask_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              சாவி->vmodmap));
        simple = false;
    }

    #undef require_explicit

    தேர்வு (சாவி->out_of_range_group_policy) {
    நிலை XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        copy_to_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        நிறுத்து;

    நிலை XKB_LAYOUT_OUT_OF_RANGE_REDIRECT: {
        எனில் (!groups_மொத்தம்) {
            /* No layout to redirect to: skip */
            நிறுத்து;
        }

        xkb_layout_mask_t layout_mask =
            (UINT32_C(1) << சாவி->out_of_range_group_எண்);
        எனில் (!(layouts & layout_mask)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     சாவி_Name_Text(keymap->ctx, name),
                     சாவி->out_of_range_group_எண் + 1);
            layout_mask = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        மாறிலி xkb_layout_mask_t கீழ் = (layouts & (layout_mask - 1));
        மாறிலி xkb_layout_index_t new = popcount32(கீழ்);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        நிறுத்து;
    }

    நிலை XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    இயல்பு:
        நிறுத்து;
    }

    எனில் (சாவி->overlays) {
        xkb_overlay_mask_t remaining = சாவி->overlays;

        மாறிலி xkb_overlay_index_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_மொத்தம் == 8, "invalid right shift");
        மாறிலி xkb_overlay_mask_t valid =
            (xkb_overlay_mask_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        எனில் (remaining != சாவி->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, சாவி_Name_Text(keymap->ctx, name),
                     remaining, சாவி->overlays & ~valid);
        }

        எனில் (remaining && !சாவி->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            மாறிலி xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            மாறிலி xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     சாவி_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        எனில் (remaining)
            simple = false;

        மாறிலி கட்டமைப்பு xkb_சாவி * மாறிலி *overlays_சாவிகள் = (சாவி->overlays_inline)
                                                    ? &சாவி->overlay_சாவி
                                                    : சாவி->overlays_சாவிகள்;
        போது (remaining) {
            /* isolate lowest set bit */
            மாறிலி xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            மாறிலி xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            எனில் (சாவி->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            மாறிலி கட்டமைப்பு xkb_சாவி *overlay_சாவி = *(overlays_சாவிகள்++);

            மாறிலி xkb_atom_t overlay_சாவி_name = (substitutions == வெற்று)
                ? overlay_சாவி->name
                : substitute_name(substitutions, overlay_சாவி->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, சாவி_Name_Text(keymap->ctx, overlay_சாவி_name));
        }
    }

    எனில் (groups_மொத்தம் > 1 || explicit_actions || explicit)
        simple = false;

    எனில் (simple) {
        எனில் (groups_மொத்தம் == 0) {
            /* Remove trailing comma */
            delete_last_char(buf, ',');
            எனில் (buf->buf[buf->size - 1] == ',')
                buf->size--;
        } இல்லையெனில் {
            copy_to_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_index_t group = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            group = xkb_keymap_சாவி_effective_layout(சாவி, group);
            எனில் (!write_keysyms(keymap, buf, buf2, சாவி, group, pretty, false))
                திரும்பு false;
            copy_to_buf(buf, " ]");
        }
        copy_to_buf(buf, " };\n");
    }
    இல்லையெனில் {
        xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
        க்கு (xkb_layout_index_t group = 0; group < சாவி->இலக்கம்_groups; group++) {
            எனில் (!((UINT32_C(1) << group) & layouts))
                தொடர்;

            new_group++;

            மாறிலி bool print_actions =
                /* Group has explicit actions */
                சாவி->groups[group].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (சாவி->groups[group].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (சாவி->groups[group].implicit_actions && !some_interprets);

            எனில் (new_group != 0)
                copy_to_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_group + 1);

            எனில் (!write_keysyms(keymap, buf, buf2, சாவி, group,
                               pretty, print_actions))
                திரும்பு false;
            copy_to_buf(buf, " ]");
            எனில் (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_group + 1);
                எனில் (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, சாவி, group))
                    திரும்பு false;
                copy_to_buf(buf, " ]");
            }
        }
        எனில் (!groups_மொத்தம்)
            delete_last_char(buf, ',');
        copy_to_buf(buf, "\n\t};\n");
    }

    திரும்பு true;
}


நிலையான முழு
cmp_modmap_step_1(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
    மாறிலி ModMapEntry *m1 = (மாறிலி ModMapEntry *)a;
    மாறிலி ModMapEntry *m2 = (மாறிலி ModMapEntry *)b;

    எனில் (!m1->mods) {
        எனில் (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            திரும்பு 0;
        }
        திரும்பு +1;
    } இல்லையெனில் எனில் (!m2->mods) {
        திரும்பு -1;
    }

    /* Valid only on step 1 */
    எனில் (m1->சாவி_Code < m2->சாவி_Code)
        திரும்பு -1;
    இல்லையெனில் எனில் (m1->சாவி_Code > m2->சாவி_Code)
        திரும்பு +1;

    எனில் (m1->mods < m2->mods)
        திரும்பு -1;
    இல்லையெனில் எனில் (m1->mods > m2->mods)
        திரும்பு +1;

    எனில் (m1->have_குறியீடு < m2->have_குறியீடு)
        திரும்பு +1;
    இல்லையெனில் எனில் (m1->have_குறியீடு > m2->have_குறியீடு)
        திரும்பு -1;

    எனில் (m1->have_குறியீடு) {
        எனில் (m1->u.சாவி_Sym < m2->u.சாவி_Sym)
            திரும்பு -1;
        எனில் (m1->u.சாவி_Sym > m2->u.சாவி_Sym)
            திரும்பு +1;
    } இல்லையெனில் {
        எனில் (m1->u.சாவி_Name < m2->u.சாவி_Name)
            திரும்பு -1;
        எனில் (m1->u.சாவி_Name > m2->u.சாவி_Name)
            திரும்பு +1;
    }

    திரும்பு 0;
}

நிலையான முழு
cmp_modmap_step_2(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
    மாறிலி ModMapEntry *m1 = (மாறிலி ModMapEntry *)a;
    மாறிலி ModMapEntry *m2 = (மாறிலி ModMapEntry *)b;

    எனில் (m1->mods < m2->mods)
        திரும்பு -1;
    எனில் (m1->mods > m2->mods)
        திரும்பு +1;

    எனில் (m1->have_குறியீடு < m2->have_குறியீடு)
        திரும்பு -1;
    எனில் (m1->have_குறியீடு > m2->have_குறியீடு)
        திரும்பு +1;

    எனில் (m1->have_குறியீடு) {
        எனில் (m1->u.சாவி_Sym < m2->u.சாவி_Sym)
            திரும்பு -1;
        எனில் (m1->u.சாவி_Sym > m2->u.சாவி_Sym)
            திரும்பு +1;
    } இல்லையெனில் {
        எனில் (m1->சாவி_Code < m2->சாவி_Code)
            திரும்பு -1;
        எனில் (m1->சாவி_Code > m2->சாவி_Code)
            திரும்பு +1;
    }

    திரும்பு 0;
}

நிலையான bool
write_modmaps(மாறிலி கட்டமைப்பு xkb_keymap *keymap, எண்ணிக்கை xkb_keymap_format format,
              மாறிலி சாவி_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, கட்டமைப்பு buf *buf)
{
    எனில் (!keymap->இலக்கம்_modmaps)
        திரும்பு true;

    darray(ModMapEntry) modmaps = darray_new();
    darray_from_பொருட்கள்(modmaps, keymap->modmaps, keymap->இலக்கம்_modmaps);
    எனில் (!darray_பொருட்கள்(modmaps))
        திரும்பு false;

    /* Sort entries by key code */
    qsort(darray_பொருட்கள்(modmaps), darray_size(modmaps),
          அளவு(*darray_பொருட்கள்(modmaps)), &cmp_modmap_step_1);

    /* Drop disabled entries at the end */
    க்கு (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        எனில் (darray_item(modmaps, m).mods)
            நிறுத்து;
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
    க்கு (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ModMapEntry *mm = &darray_item(modmaps, m);
        ப்ரதிஜ்ஞா(mm->mods);

        எனில் (mm->சாவி_Code == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            மாறிலி xkb_mod_mask_t core_mods = (mm->mods & MOD_REAL_MASK_ALL);
            எனில் (convert_to_v1_compatibility_entry && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } இல்லையெனில் {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            தொடர்;
        }

        current_keycode = mm->சாவி_Code;
        மாறிலி கட்டமைப்பு xkb_சாவி *சாவி = Xkb_சாவி(keymap, mm->சாவி_Code);

        மாறிலி xkb_mod_mask_t core_mods = (சாவி->modmap & MOD_REAL_MASK_ALL);
        எனில் (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((சாவி->modmap & (சாவி->modmap - 1)) || சாவி->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_to_v1_compatibility_entry = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    சாவி_Name_Text(keymap->ctx, சாவி->name), சாவி->modmap);
        } இல்லையெனில் {
            /* V2 format or V1 format with a single core modifier */
            ப்ரதிஜ்ஞா(சாவி->modmap);
            mm->mods = சாவி->modmap;
            /* Force key name */
            mm->have_குறியீடு = false;
            mm->u.சாவி_Name = சாவி->name;
            convert_to_v1_compatibility_entry = false;
        }
    }

    எனில் (darray_empty(modmaps))
        செல் exit;

    /* Sort entries by modifier mask */
    qsort(darray_பொருட்கள்(modmaps), darray_size(modmaps),
          அளவு(*darray_பொருட்கள்(modmaps)), &cmp_modmap_step_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_mask_t current_mods = 0;
    மாறிலி ModMapEntry *entry;
    darray_foreach(entry, modmaps) {
        எனில் (!entry->mods)
            தொடர்;

        எனில் (current_mods != entry->mods) {
            எனில் (current_mods) {
                copy_to_buf(buf, " };\n");
            }
            current_mods = entry->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } இல்லையெனில் {
            copy_to_buf(buf, ", ");
        }

        எனில் (entry->have_குறியீடு) {
            எனில் (pretty || entry->u.சாவி_Sym == XKB_சாவி_No_குறியீடு)
                write_buf(buf, "%s", KeysymText(keymap->ctx, entry->u.சாவி_Sym));
            இல்லையெனில்
                write_buf(buf, "0x%"PRIx32, entry->u.சாவி_Sym);
        } இல்லையெனில் {
            மாறிலி xkb_atom_t name = (substitutions == வெற்று)
                ? entry->u.சாவி_Name
                : substitute_name(substitutions, entry->u.சாவி_Name);
            write_buf(buf, "%s", சாவி_Name_Text(keymap->ctx, name));
        }
    }
    எனில் (current_mods) {
        copy_to_buf(buf, " };\n");
    }

exit:
    darray_free(modmaps);
    திரும்பு true;
}

நிலையான bool
write_symbols(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
              மாறிலி கட்டமைப்பு xkb_keymap_serialize_config *config,
              மாறிலி சாவி_name_substitutions *substitutions,
              xkb_layout_index_t max_groups, bool some_interp,
              கட்டமைப்பு buf *buf)
{
    மாறிலி bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    மாறிலி bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_மதிப்புகள்);
    மாறிலி bool explicit_சாவி_மதிப்புகள் =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_சாவி_மதிப்புகள்);
    மாறிலி bool drop_unused = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    மாறிலி bool drop_interprets = (explicit_சாவி_மதிப்புகள் && drop_unused);
    /* TODO: print all default values if explicit flag is set */

    எனில் (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    இல்லையெனில்
        copy_to_buf(buf, "xkb_symbols {\n");

    bool has_group_names = false;
    xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
    க்கு (xkb_layout_index_t group = 0; group < keymap->இலக்கம்_group_names; group++) {
        எனில் (!((UINT32_C(1) << group) & config->layouts))
            தொடர்;

        new_group++;

        எனில் (keymap->group_names[group]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_group + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->group_names[group]));
            copy_to_buf(buf, ";\n");
            has_group_names = true;
        }
    }
    எனில் (has_group_names)
        copy_to_buf(buf, "\n");

    எனில் (defaults && !write_actions_defaults(keymap, config->format, buf))
        திரும்பு false;

    கட்டமைப்பு buf buf2 = { வெற்று, 0, 0 };
    மாறிலி கட்டமைப்பு xkb_சாவி *சாவி;
    xkb_சாவிகள்_foreach(சாவி, keymap) {
        /* Skip keys with no explicit values */
        எனில் (சாவி->explicit &&
            !write_சாவி(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_சாவி_மதிப்புகள், pretty, buf,
                       &buf2, சாவி))
        {
            விடுவி(buf2.buf);
            திரும்பு false;
        }
    }
    விடுவி(buf2.buf);

    எனில் (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        திரும்பு false;

    copy_to_buf(buf, "};\n\n");
    திரும்பு true;
}

நிலையான எண்ணிக்கை xkb_status
write_keymap(மாறிலி கட்டமைப்பு xkb_keymap *keymap,
             மாறிலி கட்டமைப்பு xkb_keymap_serialize_config *config,
             கட்டமைப்பு buf *buf, கட்டமைப்பு xkb_keymap_serialize_result *result)
{
    சாவி_name_substitutions substitutions = darray_new();
    எனில் (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_சாவிகள்(keymap, &substitutions))
    {
        திரும்பு false;
    }

    மாறிலி சாவி_name_substitutions * மாறிலி substitutions_ptr =
        (darray_empty(substitutions)) ? வெற்று : &substitutions;

    மாறிலி xkb_layout_index_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    மாறிலி bool ok = (
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
    திரும்பு ok ? XKB_SUCCESS : -1;
}

எண்ணிக்கை xkb_status
text_v1_keymap_serialize(
        மாறிலி கட்டமைப்பு xkb_keymap *keymap,
        மாறிலி கட்டமைப்பு xkb_keymap_serialize_config *config,
        கட்டமைப்பு xkb_keymap_serialize_result *result
)
{
    கட்டமைப்பு buf buf = { வெற்று, 0, 0 };

    மாறிலி எண்ணிக்கை xkb_status status =
        write_keymap(keymap, config, &buf, result);
    எனில் (status != XKB_SUCCESS) {
        விடுவி(buf.buf);
        result->serialized = வெற்று;
        திரும்பு status;
    }

    result->serialized = buf.buf;
    result->length = buf.size + 1;
    result->layouts = config->layouts;

    திரும்பு XKB_SUCCESS;
}
