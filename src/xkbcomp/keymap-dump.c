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

#अन्तर्भाव "config.h"

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "xkbcommon/xkbcommon-status.h"
#अन्तर्भाव "xkbcommon/xkbcommon-keysyms.h"
#अन्तर्भाव "xkbcommon/xkbcommon.h"
#अन्तर्भाव "atom.h"
#अन्तर्भाव "action.h"
#अन्तर्भाव "context.h"
#अन्तर्भाव "darray.h"
#अन्तर्भाव "keymap.h"
#अन्तर्भाव "messages-codes.h"
#अन्तर्भाव "text.h"
#अन्तर्भाव "util-numbers.h"
#अन्तर्भाव "xkbcomp-priv.h"

#define BUF_CHUNK_SIZE 4096

संरचना buf {
    वर्ण *buf;
    माप_प्रकार size;
    माप_प्रकार alloc;
};

#define xkb_abs(n) _Generic((n),                \
                            पूर्णाङ्क: असल,           \
                            दीर्घ पूर्णाङ्क: द_असल,     \
                            स्वभाव: llabs )((n))

स्थिर bool
do_realloc(संरचना buf *buf, माप_प्रकार at_least)
{
    buf->alloc += BUF_CHUNK_SIZE;
    यदि (at_least >= BUF_CHUNK_SIZE)
        buf->alloc += at_least;

    वर्ण *अचर new = पुनः_स्मृति(buf->buf, buf->alloc);
    यदि (!new)
        फर्काऊ false;

    buf->buf = new;
    फर्काऊ true;
}

ATTR_PRINTF(2, 3) स्थिर bool
check_write_buf(संरचना buf *buf, अचर वर्ण *fmt, ...)
{
    बहु_सूची args;
    पूर्णाङ्क printed;
    माप_प्रकार available;

    available = buf->alloc - buf->size;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    प्रतिज्ञा(buf->buf || available == 0);
    बहु_शुरू(args, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->size : buf->buf, available,
                        fmt, args);
    बहु_पूर्ण(args);

    यदि (printed < 0) {
        /* Some error */
        गच्छ err;
    } अन्यथा यदि ((माप_प्रकार)printed >= available) {
        /* Not enough space: need to realloc */
        यदि (!do_realloc(buf, (माप_प्रकार)printed + 1))
            गच्छ err;

        /* The buffer has enough space now. */
        available = buf->alloc - buf->size;
        प्रतिज्ञा(buf->buf && (माप_प्रकार) printed < available);
        बहु_शुरू(args, fmt);
        printed = vsnprintf(buf->buf + buf->size, available, fmt, args);
        बहु_पूर्ण(args);

        यदि (printed < 0 || (माप_प्रकार) printed >= available)
            गच्छ err;
    }

    buf->size += (माप_प्रकार)printed;
    फर्काऊ true;

err:
    मुक्त(buf->buf);
    buf->buf = रिक्त;
    फर्काऊ false;
}

#define write_buf(buf, ...) गर { \
    यदि (!check_write_buf(buf, __VA_ARGS__)) \
        फर्काऊ false; \
} जबसम्म (0)

स्थिर bool
check_copy_to_buf(संरचना buf *buf, अचर वर्ण* source, माप_प्रकार len)
{
    यदि (len == 0)
        फर्काऊ true;

    अचर माप_प्रकार available = buf->alloc - buf->size;
    यदि (len >= available &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        मुक्त(buf->buf);
        buf->buf = रिक्त;
        फर्काऊ false;
    }

    स_नकल(buf->buf + buf->size, source, len);
    buf->size += len;
    /* Append NULL byte */
    buf->buf[buf->size] = '\0';
    फर्काऊ true;
}

#define copy_to_buf_len(buf, source, len) गर { \
    यदि (!check_copy_to_buf(buf, source, len)) \
        फर्काऊ false; \
} जबसम्म (0)

#define copy_to_buf(buf, source) \
    copy_to_buf_len(buf, source, परिमाण(source) - 1)

स्थिर bool
check_write_string_literal(संरचना buf *buf, अचर वर्ण* string)
{
    copy_to_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    माप_प्रकार pending_start = 0;
    माप_प्रकार current = 0;
    वर्ण escape_buffer[] = {'\\', '0', '0', '0', '0'};
    लागि (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        विकल्प (string[current]) {
        पक्ष '\n':
            /* `\n` would break strings */
            escape_buffer[1] = 'n';
            escape_len = 2;
            रोक;
        पक्ष '\\':
            /* `\` would create escape sequences */
            escape_buffer[1] = '\\';
            escape_len = 2;
            रोक;
        स्वभाव:
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
            यदि (unlikely((अचिन्हित वर्ण)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_buffer[1] = '0';
                escape_buffer[2] = '0';
                escape_buffer[3] = (वर्ण)('0' + (string[current] >> 3));
                escape_buffer[4] = (वर्ण)('0' + (string[current] & 0x7));
                escape_len = 5;
            } अन्यथा {
                /* Expand the chunk */
                जारी;
            }
        }
        /* Write pending chunk */
        प्रतिज्ञा(current >= pending_start);
        copy_to_buf_len(buf, string + pending_start, current - pending_start);
        /* Write escape sequence */
        copy_to_buf_len(buf, escape_buffer, escape_len);
        pending_start = current + 1;
    }
    /* Write remaining chunk */
    प्रतिज्ञा(current >= pending_start);
    copy_to_buf_len(buf, string + pending_start, current - pending_start);

    copy_to_buf(buf, "\"");
    फर्काऊ true;
}

#define write_buf_string_literal(buf, string) गर { \
    यदि (!check_write_string_literal(buf, string)) \
        फर्काऊ false; \
} जबसम्म (0)

स्थिर शून्य
delete_last_char(संरचना buf *buf, वर्ण c)
{
    यदि (buf->size && buf->buf[buf->size - 1] == c)
        buf->buf[--buf->size] = '\0';
}

/** Entry for a name substituion LUT */
संरचना साँचो_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
स्थिर पूर्णाङ्क
cmp_साँचो_name_substitution(अचर शून्य *a, अचर शून्य *b)
{
    अचर xkb_atom_t s1 = ((संरचना साँचो_name_substitution *) a)->source;
    अचर xkb_atom_t s2 = ((संरचना साँचो_name_substitution *) b)->source;
    फर्काऊ (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

प्रकारनिर्देश darray(संरचना साँचो_name_substitution) साँचो_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
स्थिर bool
rename_long_साँचोहरू(अचर संरचना xkb_keymap *keymap,
                 साँचो_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    गणना {
        first = '!',
        last = '0',
        invalid = '\0'
    };
    वर्ण prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', last
    };
    static_assert(परिमाण(prefixes) == last - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    अचर संरचना xkb_साँचो *साँचो;
    xkb_साँचोहरू_foreach(साँचो, keymap) {
        यदि (साँचो->name == XKB_ATOM_NONE)
            जारी;

        अचर वर्ण * अचर name = xkb_atom_text(keymap->ctx, साँचो->name);
        अचर माप_प्रकार len = strlen_safe(name);
        यदि (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (संरचना साँचो_name_substitution) {
                .source = साँचो->name,
                .keycode = साँचो->keycode,
                .target = साँचो->name
            });
        } अन्यथा यदि (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    लागि (darray_size_t a = 0; a < keymap->अङ्क_साँचो_aliases; a++) {
        अचर संरचना xkb_साँचो_alias * अचर alias = &keymap->साँचो_aliases[a];
        अचर वर्ण * अचर name = xkb_atom_text(keymap->ctx, alias->alias);
        अचर माप_प्रकार len = strlen_safe(name);
        यदि (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (संरचना साँचो_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } अन्यथा यदि (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    यदि darray_empty(*substitutions) {
        /* Nothing to do */
        फर्काऊ true;
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
    वर्ण *prefix = prefixes;
    अचर वर्ण *prefix_max = prefixes + परिमाण(prefixes) - 1;
    जबसम्म (*prefix == invalid && prefix < prefix_max)
        prefix++;

    /* Keycode prefix: prefer ‘0’ if available */
    वर्ण keycode_prefix;
    यदि (prefixes[last - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_prefix = last;
        prefixes[last - first] = invalid;
    } अन्यथा {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_prefix = *prefix;
        यदि (unlikely(keycode_prefix == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            फर्काऊ true;
        }
        /* Update current prefix */
        *prefix = invalid;
        जबसम्म (*prefix == invalid && prefix < prefix_max)
            prefix++;
    }

    /* Proceed renaming */
    uint16_t next_id = 1;
    संरचना साँचो_name_substitution *sub;
    वर्ण buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        यदि (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, परिमाण(buf), "%c%03"PRIx32,
                     keycode_prefix, sub->keycode);
            अचर xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, परिमाण(buf) - 2);
            यदि (target == XKB_ATOM_NONE)
                गच्छ error;
            sub->target = target;
        } अन्यथा यदि (*prefix != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, परिमाण(buf), "%c%03"PRIx16, *prefix, next_id);
            अचर xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, परिमाण(buf) - 2);
            यदि (target == XKB_ATOM_NONE)
                गच्छ error;
            sub->target = target;
            यदि (++next_id > 0xfff) {
                *prefix = invalid;
                /* Get next available prefix */
                जबसम्म (*prefix == invalid && prefix < prefix_max)
                    prefix++;
                /* Reset counter */
                next_id = 1;
            }
        } अन्यथा {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->target = sub->source;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    क्विक(darray_वस्तुहरू(*substitutions), darray_size(*substitutions),
          परिमाण(*darray_वस्तुहरू(*substitutions)), &cmp_साँचो_name_substitution);

    फर्काऊ true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate key name substitution\n");
    फर्काऊ false;
}

/** Substitute a key name with a valid X11 key name */
स्थिर xkb_atom_t
substitute_name(अचर साँचो_name_substitutions *substitutions, xkb_atom_t name)
{
    /*द्विआधारी खोज*/
    darray_size_t first = 0;
    darray_size_t last = darray_size(*substitutions);

    जबसम्म (last > first) {
        darray_size_t mid = first + (last - 1 - first) / 2;
        अचर xkb_atom_t source = darray_item(*substitutions, mid).source;
        यदि (source < name)
            first = mid + 1;
        अन्यथा यदि (source > name)
            last = mid;
        अन्यथा /* found it */
            फर्काऊ darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    फर्काऊ name;
}

स्थिर bool
write_vmods(अचर संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
            bool explicit, संरचना buf *buf)
{
    अचर संरचना xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        यदि (!has_some) {
            copy_to_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } अन्यथा {
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
        अचर bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        यदि ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
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

    यदि (has_some)
        copy_to_buf(buf, ";\n\n");

    फर्काऊ true;
}

स्थिर bool
write_keycodes(अचर संरचना xkb_keymap *keymap,
               अचर साँचो_name_substitutions *substitutions,
               गणना xkb_keymap_serialize_flags flags, संरचना buf *buf)
{
    अचर संरचना xkb_साँचो *साँचो;
    xkb_led_index_t idx;
    अचर संरचना xkb_led *led;

    अचर bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    यदि (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_साँचो_code, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_साँचो_code, 255));

    xkb_साँचोहरू_foreach(साँचो, keymap) {
        यदि (साँचो->name == XKB_ATOM_NONE)
            जारी;

        अचर xkb_atom_t name = (substitutions == रिक्त)
            ? साँचो->name
            : substitute_name(substitutions, साँचो->name);
        यदि (pretty)
            write_buf(buf, "\t%-20s", साँचो_Name_Text(keymap->ctx, name));
        अन्यथा
            write_buf(buf, "\t%s", साँचो_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", साँचो->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        यदि (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_to_buf(buf, ";\n");
        }

    लागि (darray_size_t i = 0; i < keymap->अङ्क_साँचो_aliases; i++) {
        अचर xkb_atom_t alias = (substitutions == रिक्त)
            ? keymap->साँचो_aliases[i].alias
            : substitute_name(substitutions, keymap->साँचो_aliases[i].alias);
        अचर xkb_atom_t real = (substitutions == रिक्त)
            ? keymap->साँचो_aliases[i].real
            : substitute_name(substitutions, keymap->साँचो_aliases[i].real);

        यदि (pretty)
            write_buf(buf, "\talias %-14s",
                      साँचो_Name_Text(keymap->ctx, alias));
        अन्यथा
            write_buf(buf, "\talias %s",
                      साँचो_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  साँचो_Name_Text(keymap->ctx, real));
    }

    copy_to_buf(buf, "};\n\n");
    फर्काऊ true;
}

स्थिर bool
write_types(अचर संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
            गणना xkb_keymap_serialize_flags flags, संरचना buf *buf)
{
    यदि (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    यदि (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        फर्काऊ false;

    अचर bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    लागि (darray_size_t i = 0; i < keymap->अङ्क_types; i++) {
        अचर संरचना xkb_साँचो_type * अचर type = &keymap->types[i];
        यदि (!type->required && drop_unused)
            जारी;

        copy_to_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_to_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        लागि (darray_size_t j = 0; j < type->अङ्क_entries; j++) {
            अचर वर्ण *str;
            अचर संरचना xkb_साँचो_type_entry *entry = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            यदि (entry->level == 0 && entry->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मानहरू))
                जारी;

            str = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              entry->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, entry->level + 1);

            यदि (entry->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           entry->preserve.mods));
        }

        लागि (xkb_level_index_t n = 0; n < type->अङ्क_level_names; n++)
            यदि (type->level_names[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_names[n]));
                copy_to_buf(buf, ";\n");
            }

        copy_to_buf(buf, "\t};\n");
    }

    copy_to_buf(buf, "};\n\n");
    फर्काऊ true;
}

स्थिर bool
write_led_map(अचर संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
              bool defaults, संरचना buf *buf, अचर संरचना xkb_led *led)
{
    copy_to_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_to_buf(buf, " {\n");

    यदि (led->which_groups) {
        यदि (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      LedStateMaskText(keymap->ctx, groupComponentMaskNames,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    यदि (led->which_mods) {
        यदि (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      LedStateMaskText(keymap->ctx, modComponentMaskNames,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    यदि (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  ControlMaskText(keymap->ctx, format, led->ctrls));
    }

    copy_to_buf(buf, "\t};\n");
    फर्काऊ true;
}

स्थिर अचर वर्ण *
affect_lock_text(गणना xkb_action_flags flags, bool show_both)
{
    विकल्प (flags & (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK)) {
    पक्ष 0:
        फर्काऊ show_both ? ",affect=both" : "";
    पक्ष ACTION_LOCK_NO_UNLOCK:
        फर्काऊ ",affect=lock";
    पक्ष ACTION_LOCK_NO_LOCK:
        फर्काऊ ",affect=unlock";
    पक्ष ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK:
        फर्काऊ ",affect=neither";
    स्वभाव:
        फर्काऊ "";
    }
}

#define चिह्न_PADDING 15
#define ACTION_PADDING 30

स्थिर bool
write_action(अचर संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
             xkb_layout_index_t max_groups, संरचना buf *buf,
             अचर सङ्घ xkb_action *action,
             अचर वर्ण *prefix, अचर वर्ण *suffix)
{
    अचर वर्ण *type;
    अचर वर्ण *args = रिक्त;

    यदि (!prefix)
        prefix = "";
    यदि (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    विकल्प (action->type) {
    पक्ष ACTION_TYPE_MOD_SET:
    पक्ष ACTION_TYPE_MOD_LATCH:
    पक्ष ACTION_TYPE_MOD_LOCK:
        यदि (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            args = "modMapMods";
        अन्यथा
            args = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        यदि (unlockOnPress && !isModsUnLockOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->group.flags & ACTION_LATCH_ON_PRESS);
        यदि (latchOnPress && !isModsLatchOnPressSupported(format)) {
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
        रोक;

    पक्ष ACTION_TYPE_GROUP_SET:
    पक्ष ACTION_TYPE_GROUP_LATCH:
    पक्ष ACTION_TYPE_GROUP_LOCK:
        यदि ((uint32_t) xkb_abs(action->group.group) <
            (max_groups + !(action->group.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool lockOnRelease = action->type == ACTION_TYPE_GROUP_LOCK &&
                                 (action->group.flags & ACTION_LOCK_ON_RELEASE);
            यदि (lockOnRelease && !isGroupLockOnReleaseSupported(format)) {
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
        } अन्यथा {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->group.group +
                       !!(action->group.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            गच्छ void_action;
        }
        रोक;

    पक्ष ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", prefix, type, suffix);
        रोक;

    पक्ष ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", prefix, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        रोक;

    पक्ष ACTION_TYPE_PTR_LOCK:
        args = affect_lock_text(action->btn.flags, true);
        /* fallthrough */
    पक्ष ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", prefix, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        यदि (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        अन्यथा
            copy_to_buf(buf, "default");
        यदि (action->btn.गन्ती) {
            प्रतिज्ञा(action->type != ACTION_TYPE_PTR_LOCK);
            write_buf(buf, ",count=%u", action->btn.गन्ती);
        }
        यदि (args)
            write_buf(buf, "%s", args);
        write_buf(buf, ")%s", suffix);
        रोक;

    पक्ष ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", prefix, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.मान >= 0) ? "+" : "",
                  action->dflt.मान);
        write_buf(buf, ")%s", suffix);
        रोक;

    पक्ष ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", prefix, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        रोक;

    पक्ष ACTION_TYPE_CTRL_SET:
    पक्ष ACTION_TYPE_CTRL_LOCK:
        write_buf(buf, "%s%s(controls=%s%s)%s", prefix, type,
                  ControlMaskText(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_LOCK
                    ? affect_lock_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        रोक;

    पक्ष ACTION_TYPE_REDIRECT_साँचो: {
        write_buf(buf, "%s%s(", prefix, type);
        अचर संरचना xkb_साँचो * अचर साँचो = Xkb_साँचो(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        यदि (साँचो)
            write_buf(buf, "keycode=%s", साँचो_Name_Text(keymap->ctx, साँचो->name));
        यदि (action->redirect.affect_mods) {
            xkb_mod_mask_t mask;
            mask = (action->redirect.affect_mods & action->redirect.mods);
            यदि (mask)
                write_buf(buf, ",modifiers=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
            mask = (action->redirect.affect_mods & ~action->redirect.mods);
            यदि (mask)
                write_buf(buf, ",clearMods=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
        }
        write_buf(buf, ")%s", suffix);
        रोक;
    }

    पक्ष ACTION_TYPE_UNKNOWN:
    पक्ष ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", prefix, suffix);
        रोक;

    पक्ष ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        यदि (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      prefix, suffix);
        अन्यथा
            write_buf(buf, "%sVoidAction()%s", prefix, suffix);
        रोक;

    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अङ्क_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        प्रतिज्ञा(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  prefix, type, action->type, action->priv.data[0],
                  action->priv.data[1], action->priv.data[2],
                  action->priv.data[3], action->priv.data[4],
                  action->priv.data[5], action->priv.data[6],
                  suffix);
        रोक;
    }

    फर्काऊ true;
}

स्थिर अचर सङ्घ xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

स्थिर bool
write_actions(अचर संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
              xkb_layout_index_t max_groups, संरचना buf *buf, संरचना buf *buf2,
              अचर संरचना xkb_साँचो *साँचो, xkb_layout_index_t group)
{
    स्थिर अचर सङ्घ xkb_action noAction = { .type = ACTION_TYPE_NONE };

    लागि (xkb_level_index_t level = 0; level < Xkb_साँचो_अङ्क_Levels(साँचो, group);
         level++) {

        यदि (level != 0)
            copy_to_buf(buf, ", ");

        अचर सङ्घ xkb_action *actions = रिक्त;
        xkb_action_गन्ती_t गन्ती = xkb_keymap_साँचो_get_actions_by_level(
            keymap, साँचो, group, level, &actions
        );

        यदि (गन्ती > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            गन्ती = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->size = 0;
        यदि (गन्ती == 0) {
            यदि (!write_action(keymap, format, max_groups,
                              buf2, &noAction, रिक्त, रिक्त))
                फर्काऊ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        अन्यथा यदि (गन्ती == 1) {
            यदि (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), रिक्त, रिक्त))
                फर्काऊ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        अन्यथा {
            copy_to_buf(buf2, "{ ");
            लागि (xkb_action_गन्ती_t k = 0; k < गन्ती; k++) {
                यदि (k != 0)
                    copy_to_buf(buf2, ", ");
                माप_प्रकार old_size = buf2->size;
                यदि (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), रिक्त, रिक्त))
                    फर्काऊ false;
                /* Check if padding is necessary */
                यदि (buf2->size >= old_size + ACTION_PADDING)
                    जारी;
                /* Compute and write padding, then write the action again */
                अचर पूर्णाङ्क padding = (पूर्णाङ्क)(old_size + ACTION_PADDING - buf2->size);
                buf2->size = old_size;
                write_buf(buf2, "%*s", padding, "");
                यदि (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), रिक्त, रिक्त))
                    फर्काऊ false;
            }
            copy_to_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    फर्काऊ true;
}

स्थिर bool
write_action_defaults(अचर संरचना xkb_keymap *keymap,
                      गणना xkb_keymap_format format,
                      अचर सङ्घ xkb_action *action,
                      संरचना buf *buf)
{
    अचर वर्ण *type = ActionTypeText(action->type);

#define PREFIX "\t"

    विकल्प (action->type) {
    पक्ष ACTION_TYPE_MOD_SET:
    पक्ष ACTION_TYPE_MOD_LATCH:
    पक्ष ACTION_TYPE_MOD_LOCK:
        प्रतिज्ञा(action->mods.flags == 0);
        प्रतिज्ञा(action->mods.mods.mods == 0);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        यदि (action->type != ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        यदि (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, PREFIX"%s.latchOnPress = false;\n", type);
        यदि (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        यदि (action->type == ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        यदि (isModsUnLockOnPressSupported(format))
            write_buf(buf, PREFIX"%s.unlockOnPress = false;\n", type);
        रोक;

    पक्ष ACTION_TYPE_GROUP_SET:
    पक्ष ACTION_TYPE_GROUP_LATCH:
    पक्ष ACTION_TYPE_GROUP_LOCK:
        प्रतिज्ञा(action->group.flags == 0);
        प्रतिज्ञा(action->group.group == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.group = +0;\n", type);
        यदि (action->type != ACTION_TYPE_GROUP_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        यदि (action->type == ACTION_TYPE_GROUP_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        यदि (action->type == ACTION_TYPE_GROUP_LOCK &&
            isGroupLockOnReleaseSupported(format))
            write_buf(buf, PREFIX"%s.lockOnRelease = false;\n", type);
        रोक;


    पक्ष ACTION_TYPE_PTR_MOVE:
        प्रतिज्ञा(action->ptr.x == 0);
        प्रतिज्ञा(action->ptr.y == 0);
        प्रतिज्ञा(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.x = +0;\n", type);
        write_buf(buf, PREFIX"%s.y = +0;\n", type);
        write_buf(buf, PREFIX"%s.accel = true;\n", type);
        रोक;

    पक्ष ACTION_TYPE_PTR_LOCK:
    पक्ष ACTION_TYPE_PTR_BUTTON:
        प्रतिज्ञा(action->btn.flags == 0);
        प्रतिज्ञा(action->btn.button == 0);
        प्रतिज्ञा(action->btn.गन्ती == 0);
        write_buf(buf, PREFIX"%s.button = 0;\n", type);
        write_buf(buf, PREFIX"%s.count = 0;\n", type);
        यदि (action->type == ACTION_TYPE_PTR_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        रोक;

    पक्ष ACTION_TYPE_PTR_DEFAULT:
        प्रतिज्ञा(action->dflt.flags == 0);
        प्रतिज्ञा(action->dflt.मान == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.button = +1;\n", type);
        रोक;

    पक्ष ACTION_TYPE_SWITCH_VT:
        प्रतिज्ञा(action->screen.flags == ACTION_SAME_SCREEN);
        प्रतिज्ञा(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.screen = +0;\n", type);
        write_buf(buf, PREFIX"%s.same = true;\n", type);
        रोक;

    पक्ष ACTION_TYPE_CTRL_SET:
    पक्ष ACTION_TYPE_CTRL_LOCK:
        प्रतिज्ञा(action->ctrls.flags == 0);
        प्रतिज्ञा(action->ctrls.ctrls == 0);
        write_buf(buf, PREFIX"%s.controls = 0;\n", type);
        यदि (action->type == ACTION_TYPE_CTRL_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        रोक;

    पक्ष ACTION_TYPE_REDIRECT_साँचो:
        प्रतिज्ञा(action->redirect.keycode == keymap->redirect_साँचो_auto);
        प्रतिज्ञा(action->redirect.affect_mods == 0);
        प्रतिज्ञा(action->redirect.mods == 0);
        write_buf(buf, PREFIX"%s.keycode = auto;\n", type);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        write_buf(buf, PREFIX"%s.clearMods = 0;\n", type);
        रोक;

    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अङ्क_ENTRIES,
                      "Missing action type");
    }

#undef PREFIX

    फर्काऊ true;
}

स्थिर bool
write_actions_defaults(अचर संरचना xkb_keymap *keymap,
                       गणना xkb_keymap_format format, संरचना buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    लागि (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_अङ्क_ENTRIES; a++) {
        अचर सङ्घ xkb_action * restrict अचर action = &defaults.actions[a];
        यदि (!write_action_defaults(keymap, format, action, buf))
            फर्काऊ false;
    }
    copy_to_buf(buf, "\n");

    फर्काऊ true;
}

स्थिर अचर संरचना xkb_sym_interpret fallback_interpret = {
    .sym = XKB_साँचो_Void_चिह्न,
    .required = true,
    .repeat = FALLBACK_INTERPRET_साँचो_REPEAT,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .अङ्क_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

स्थिर bool
write_compat(अचर संरचना xkb_keymap * restrict keymap,
             गणना xkb_keymap_format format, xkb_layout_index_t max_groups,
             गणना xkb_keymap_serialize_flags flags, bool * restrict some_interp,
             संरचना buf *buf)
{
    अचर bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    अचर bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मानहरू);
    अचर bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    अचर bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_साँचो_मानहरू) && drop_unused);
    /* TODO: print all default values if the explicit flag is set */

    यदि (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_compatibility {\n");

    यदि (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        फर्काऊ false;

    यदि (defaults && !write_actions_defaults(keymap, format, buf))
        फर्काऊ false;

    copy_to_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_to_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    अचर bool use_fallback_interpret =
        (!keymap->अङ्क_sym_interprets || drop_interprets);
    अचर darray_size_t अङ्क_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->अङ्क_sym_interprets);
    अचर संरचना xkb_sym_interpret* अचर sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    लागि (darray_size_t i = 0; i < अङ्क_sym_interprets; i++) {
        अचर संरचना xkb_sym_interpret *si = &sym_interprets[i];
        यदि (!si->required && drop_unused)
            जारी;

        *some_interp = true;

        copy_to_buf(buf, "\tinterpret ");
        यदि (!si->sym) {
            copy_to_buf(buf, "Any");
        } अन्यथा यदि (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } अन्यथा {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->match),
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        यदि (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      ModIndexText(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        यदि (si->level_one_only) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } अन्यथा यदि (defaults) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        यदि (si->repeat) {
            copy_to_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } अन्यथा यदि (defaults) {
            copy_to_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        अचर सङ्घ xkb_action *action = रिक्त;
        xkb_action_गन्ती_t action_गन्ती = si->अङ्क_actions;
        यदि (action_गन्ती <= 1) {
            action = &si->a.action;
        } अन्यथा यदि (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_गन्ती = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        यदि (action_गन्ती > 1) {
            copy_to_buf(buf, "\n\t\taction= {");
            अचर वर्ण suffix[] = ", ";
            लागि (xkb_action_गन्ती_t k = 0; k < si->अङ्क_actions; k++) {
                यदि (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    फर्काऊ false;
            }
            buf->size -= परिमाण(suffix) - 1; /* trailing comma */
            copy_to_buf(buf, "};");
            has_explicit_properties = true;
        } अन्यथा यदि (action_गन्ती == 1) {
            यदि (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                फर्काऊ false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    यदि (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        अङ्क_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].match == fallback_interpret.match &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].अङ्क_actions == fallback_interpret.अङ्क_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    अचर संरचना xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        यदि ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            फर्काऊ false;
        }
    }

    copy_to_buf(buf, "};\n\n");

    फर्काऊ true;
}

स्थिर bool
write_keysyms(अचर संरचना xkb_keymap *keymap,
              संरचना buf *buf, संरचना buf *buf2,
              अचर संरचना xkb_साँचो *साँचो, xkb_layout_index_t group,
              bool pretty, bool show_actions)
{
    अचर अचिन्हित पूर्णाङ्क padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : चिह्न_PADDING)
        : 0;
    लागि (xkb_level_index_t level = 0; level < Xkb_साँचो_अङ्क_Levels(साँचो, group);
         level++) {
        अचर xkb_keysym_t *syms;
        पूर्णाङ्क अङ्क_syms;

        यदि (level != 0)
            copy_to_buf(buf, ", ");

        अङ्क_syms = xkb_keymap_साँचो_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (संरचना xkb_keymap *)keymap,
            साँचो->keycode, group, level, &syms
        );

        अचर xkb_keysym_t no_चिह्न = XKB_साँचो_No_चिह्न;
        यदि (अङ्क_syms == 0) {
            अङ्क_syms = 1;
            syms = &no_चिह्न;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        यदि (अङ्क_syms == 1) {
            यदि (pretty || syms[0] == XKB_साँचो_No_चिह्न)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            अन्यथा
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } अन्यथा {
            यदि (pretty) {
                buf2->size = 0;
                copy_to_buf(buf2, "{ ");
                लागि (पूर्णाङ्क s = 0; s < अङ्क_syms; s++) {
                    यदि (s != 0)
                        copy_to_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_to_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } अन्यथा {
                copy_to_buf(buf, "{");
                लागि (पूर्णाङ्क s = 0; s < अङ्क_syms; s++) {
                    यदि (s != 0)
                        copy_to_buf(buf, ",");
                    यदि (syms[s] == XKB_साँचो_No_चिह्न)
                        copy_to_buf(buf, "NoSymbol");
                    अन्यथा
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_to_buf(buf, "}");
            }
        }
    }

    फर्काऊ true;
}

स्थिर bool
write_साँचो(अचर संरचना xkb_keymap *keymap,
          अचर संरचना xkb_keymap_serialize_config *config,
          अचर साँचो_name_substitutions *substitutions,
          xkb_layout_index_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          संरचना buf *buf, संरचना buf *buf2, अचर संरचना xkb_साँचो *साँचो)
{
    bool simple = true;

    xkb_layout_mask_t layouts = config->layouts;
    यदि (popcount32(layouts) != keymap->अङ्क_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_index_t l = 0;
        लागि (xkb_layout_mask_t ls = layouts; ls; ls >>= 1, l++) {
            यदि (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                अचर xkb_layout_index_t effective =
                    xkb_keymap_साँचो_effective_layout(साँचो, l);
                यदि (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } अन्यथा {
        layouts &= ((UINT64_C(1) << साँचो->अङ्क_groups) - 1);
    }
    अचर xkb_layout_index_t groups_गन्ती = popcount32(layouts);

    अचर xkb_atom_t name = (substitutions == रिक्त)
        ? साँचो->name
        : substitute_name(substitutions, साँचो->name);

    यदि (pretty)
        write_buf(buf, "\tkey %-20s {", साँचो_Name_Text(keymap->ctx, name));
    अन्यथा
        write_buf(buf, "\tkey %s {", साँचो_Name_Text(keymap->ctx, name));

    यदि ((साँचो->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        यदि (!explicit) {
            लागि (xkb_layout_index_t group = 1; group < साँचो->अङ्क_groups; group++) {
                यदि (!((UINT32_C(1) << group) & layouts))
                    जारी;

                यदि (साँचो->groups[group].type != साँचो->groups[0].type) {
                    multi_type = true;
                    रोक;
                }
            }
        }

        यदि (multi_type) {
            xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
            लागि (xkb_layout_index_t group = 0; group < साँचो->अङ्क_groups; group++) {
                यदि (!((UINT32_C(1) << group) & layouts))
                    जारी;

                new_group++;

                यदि (!साँचो->groups[group].explicit_type && !explicit)
                    जारी;

                अचर संरचना xkb_साँचो_type * अचर type = साँचो->groups[group].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_group + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_to_buf(buf, ",");
            }
        }
        अन्यथा {
            अचर संरचना xkb_साँचो_type * अचर type = साँचो->groups[0].type;
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
    अचर bool explicit_actions = (साँचो->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #define require_explicit(साँचो, prop,                                       \
                             मान, initial_मान, default_interpret_मान) ( \
        /* Explicit value in the source */                                    \
        ((साँचो)->explicit & (prop)) ||                                         \
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
                ((साँचो)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (मान) != (initial_मान) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_मान) != (default_interpret_मान)                  \
            )                                                                 \
        )                                                                     \
    )

    यदि (require_explicit(साँचो, EXPLICIT_REPEAT, साँचो->repeats,
                         DEFAULT_साँचो_REPEAT, DEFAULT_INTERPRET_साँचो_REPEAT)) {
        यदि (साँचो->repeats)
            copy_to_buf(buf, "\n\t\trepeat= Yes,");
        अन्यथा
            copy_to_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    यदि (require_explicit(साँचो, EXPLICIT_VMODMAP, साँचो->vmodmap,
                         (xkb_mod_mask_t) DEFAULT_साँचो_VMODMAP,
                         (xkb_mod_mask_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              साँचो->vmodmap));
        simple = false;
    }

    #undef require_explicit

    विकल्प (साँचो->out_of_range_group_policy) {
    पक्ष XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        copy_to_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        रोक;

    पक्ष XKB_LAYOUT_OUT_OF_RANGE_REDIRECT: {
        यदि (!groups_गन्ती) {
            /* No layout to redirect to: skip */
            रोक;
        }

        xkb_layout_mask_t layout_mask =
            (UINT32_C(1) << साँचो->out_of_range_group_सङ्ख्या);
        यदि (!(layouts & layout_mask)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     साँचो_Name_Text(keymap->ctx, name),
                     साँचो->out_of_range_group_सङ्ख्या + 1);
            layout_mask = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        अचर xkb_layout_mask_t तल्लो = (layouts & (layout_mask - 1));
        अचर xkb_layout_index_t new = popcount32(तल्लो);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        रोक;
    }

    पक्ष XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    स्वभाव:
        रोक;
    }

    यदि (साँचो->overlays) {
        xkb_overlay_mask_t remaining = साँचो->overlays;

        अचर xkb_overlay_index_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_गन्ती == 8, "invalid right shift");
        अचर xkb_overlay_mask_t valid =
            (xkb_overlay_mask_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        यदि (remaining != साँचो->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, साँचो_Name_Text(keymap->ctx, name),
                     remaining, साँचो->overlays & ~valid);
        }

        यदि (remaining && !साँचो->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            अचर xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
            );
            /* get its index */
            अचर xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     साँचो_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        यदि (remaining)
            simple = false;

        अचर संरचना xkb_साँचो * अचर *overlays_साँचोहरू = (साँचो->overlays_inline)
                                                    ? &साँचो->overlay_साँचो
                                                    : साँचो->overlays_साँचोहरू;
        जबसम्म (remaining) {
            /* isolate lowest set bit */
            अचर xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
            );
            /* get its index */
            अचर xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            यदि (साँचो->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            अचर संरचना xkb_साँचो *overlay_साँचो = *(overlays_साँचोहरू++);

            अचर xkb_atom_t overlay_साँचो_name = (substitutions == रिक्त)
                ? overlay_साँचो->name
                : substitute_name(substitutions, overlay_साँचो->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, साँचो_Name_Text(keymap->ctx, overlay_साँचो_name));
        }
    }

    यदि (groups_गन्ती > 1 || explicit_actions || explicit)
        simple = false;

    यदि (simple) {
        यदि (groups_गन्ती == 0) {
            /* Remove trailing comma */
            delete_last_char(buf, ',');
            यदि (buf->buf[buf->size - 1] == ',')
                buf->size--;
        } अन्यथा {
            copy_to_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_index_t group = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            group = xkb_keymap_साँचो_effective_layout(साँचो, group);
            यदि (!write_keysyms(keymap, buf, buf2, साँचो, group, pretty, false))
                फर्काऊ false;
            copy_to_buf(buf, " ]");
        }
        copy_to_buf(buf, " };\n");
    }
    अन्यथा {
        xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
        लागि (xkb_layout_index_t group = 0; group < साँचो->अङ्क_groups; group++) {
            यदि (!((UINT32_C(1) << group) & layouts))
                जारी;

            new_group++;

            अचर bool print_actions =
                /* Group has explicit actions */
                साँचो->groups[group].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (साँचो->groups[group].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (साँचो->groups[group].implicit_actions && !some_interprets);

            यदि (new_group != 0)
                copy_to_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_group + 1);

            यदि (!write_keysyms(keymap, buf, buf2, साँचो, group,
                               pretty, print_actions))
                फर्काऊ false;
            copy_to_buf(buf, " ]");
            यदि (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_group + 1);
                यदि (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, साँचो, group))
                    फर्काऊ false;
                copy_to_buf(buf, " ]");
            }
        }
        यदि (!groups_गन्ती)
            delete_last_char(buf, ',');
        copy_to_buf(buf, "\n\t};\n");
    }

    फर्काऊ true;
}


स्थिर पूर्णाङ्क
cmp_modmap_step_1(अचर शून्य *a, अचर शून्य *b)
{
    अचर ModMapEntry *m1 = (अचर ModMapEntry *)a;
    अचर ModMapEntry *m2 = (अचर ModMapEntry *)b;

    यदि (!m1->mods) {
        यदि (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            फर्काऊ 0;
        }
        फर्काऊ +1;
    } अन्यथा यदि (!m2->mods) {
        फर्काऊ -1;
    }

    /* Valid only on step 1 */
    यदि (m1->साँचो_Code < m2->साँचो_Code)
        फर्काऊ -1;
    अन्यथा यदि (m1->साँचो_Code > m2->साँचो_Code)
        फर्काऊ +1;

    यदि (m1->mods < m2->mods)
        फर्काऊ -1;
    अन्यथा यदि (m1->mods > m2->mods)
        फर्काऊ +1;

    यदि (m1->have_चिह्न < m2->have_चिह्न)
        फर्काऊ +1;
    अन्यथा यदि (m1->have_चिह्न > m2->have_चिह्न)
        फर्काऊ -1;

    यदि (m1->have_चिह्न) {
        यदि (m1->u.साँचो_Sym < m2->u.साँचो_Sym)
            फर्काऊ -1;
        यदि (m1->u.साँचो_Sym > m2->u.साँचो_Sym)
            फर्काऊ +1;
    } अन्यथा {
        यदि (m1->u.साँचो_Name < m2->u.साँचो_Name)
            फर्काऊ -1;
        यदि (m1->u.साँचो_Name > m2->u.साँचो_Name)
            फर्काऊ +1;
    }

    फर्काऊ 0;
}

स्थिर पूर्णाङ्क
cmp_modmap_step_2(अचर शून्य *a, अचर शून्य *b)
{
    अचर ModMapEntry *m1 = (अचर ModMapEntry *)a;
    अचर ModMapEntry *m2 = (अचर ModMapEntry *)b;

    यदि (m1->mods < m2->mods)
        फर्काऊ -1;
    यदि (m1->mods > m2->mods)
        फर्काऊ +1;

    यदि (m1->have_चिह्न < m2->have_चिह्न)
        फर्काऊ -1;
    यदि (m1->have_चिह्न > m2->have_चिह्न)
        फर्काऊ +1;

    यदि (m1->have_चिह्न) {
        यदि (m1->u.साँचो_Sym < m2->u.साँचो_Sym)
            फर्काऊ -1;
        यदि (m1->u.साँचो_Sym > m2->u.साँचो_Sym)
            फर्काऊ +1;
    } अन्यथा {
        यदि (m1->साँचो_Code < m2->साँचो_Code)
            फर्काऊ -1;
        यदि (m1->साँचो_Code > m2->साँचो_Code)
            फर्काऊ +1;
    }

    फर्काऊ 0;
}

स्थिर bool
write_modmaps(अचर संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
              अचर साँचो_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, संरचना buf *buf)
{
    यदि (!keymap->अङ्क_modmaps)
        फर्काऊ true;

    darray(ModMapEntry) modmaps = darray_new();
    darray_from_वस्तुहरू(modmaps, keymap->modmaps, keymap->अङ्क_modmaps);
    यदि (!darray_वस्तुहरू(modmaps))
        फर्काऊ false;

    /* Sort entries by key code */
    क्विक(darray_वस्तुहरू(modmaps), darray_size(modmaps),
          परिमाण(*darray_वस्तुहरू(modmaps)), &cmp_modmap_step_1);

    /* Drop disabled entries at the end */
    लागि (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        यदि (darray_item(modmaps, m).mods)
            रोक;
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
    लागि (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ModMapEntry *mm = &darray_item(modmaps, m);
        प्रतिज्ञा(mm->mods);

        यदि (mm->साँचो_Code == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            अचर xkb_mod_mask_t core_mods = (mm->mods & MOD_REAL_MASK_ALL);
            यदि (convert_to_v1_compatibility_entry && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } अन्यथा {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            जारी;
        }

        current_keycode = mm->साँचो_Code;
        अचर संरचना xkb_साँचो *साँचो = Xkb_साँचो(keymap, mm->साँचो_Code);

        अचर xkb_mod_mask_t core_mods = (साँचो->modmap & MOD_REAL_MASK_ALL);
        यदि (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((साँचो->modmap & (साँचो->modmap - 1)) || साँचो->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_to_v1_compatibility_entry = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    साँचो_Name_Text(keymap->ctx, साँचो->name), साँचो->modmap);
        } अन्यथा {
            /* V2 format or V1 format with a single core modifier */
            प्रतिज्ञा(साँचो->modmap);
            mm->mods = साँचो->modmap;
            /* Force key name */
            mm->have_चिह्न = false;
            mm->u.साँचो_Name = साँचो->name;
            convert_to_v1_compatibility_entry = false;
        }
    }

    यदि (darray_empty(modmaps))
        गच्छ निकास;

    /* Sort entries by modifier mask */
    क्विक(darray_वस्तुहरू(modmaps), darray_size(modmaps),
          परिमाण(*darray_वस्तुहरू(modmaps)), &cmp_modmap_step_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_mask_t current_mods = 0;
    अचर ModMapEntry *entry;
    darray_foreach(entry, modmaps) {
        यदि (!entry->mods)
            जारी;

        यदि (current_mods != entry->mods) {
            यदि (current_mods) {
                copy_to_buf(buf, " };\n");
            }
            current_mods = entry->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } अन्यथा {
            copy_to_buf(buf, ", ");
        }

        यदि (entry->have_चिह्न) {
            यदि (pretty || entry->u.साँचो_Sym == XKB_साँचो_No_चिह्न)
                write_buf(buf, "%s", KeysymText(keymap->ctx, entry->u.साँचो_Sym));
            अन्यथा
                write_buf(buf, "0x%"PRIx32, entry->u.साँचो_Sym);
        } अन्यथा {
            अचर xkb_atom_t name = (substitutions == रिक्त)
                ? entry->u.साँचो_Name
                : substitute_name(substitutions, entry->u.साँचो_Name);
            write_buf(buf, "%s", साँचो_Name_Text(keymap->ctx, name));
        }
    }
    यदि (current_mods) {
        copy_to_buf(buf, " };\n");
    }

निकास:
    darray_free(modmaps);
    फर्काऊ true;
}

स्थिर bool
write_symbols(अचर संरचना xkb_keymap *keymap,
              अचर संरचना xkb_keymap_serialize_config *config,
              अचर साँचो_name_substitutions *substitutions,
              xkb_layout_index_t max_groups, bool some_interp,
              संरचना buf *buf)
{
    अचर bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    अचर bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मानहरू);
    अचर bool explicit_साँचो_मानहरू =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_साँचो_मानहरू);
    अचर bool drop_unused = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    अचर bool drop_interprets = (explicit_साँचो_मानहरू && drop_unused);
    /* TODO: print all default values if explicit flag is set */

    यदि (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_symbols {\n");

    bool has_group_names = false;
    xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
    लागि (xkb_layout_index_t group = 0; group < keymap->अङ्क_group_names; group++) {
        यदि (!((UINT32_C(1) << group) & config->layouts))
            जारी;

        new_group++;

        यदि (keymap->group_names[group]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_group + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->group_names[group]));
            copy_to_buf(buf, ";\n");
            has_group_names = true;
        }
    }
    यदि (has_group_names)
        copy_to_buf(buf, "\n");

    यदि (defaults && !write_actions_defaults(keymap, config->format, buf))
        फर्काऊ false;

    संरचना buf buf2 = { रिक्त, 0, 0 };
    अचर संरचना xkb_साँचो *साँचो;
    xkb_साँचोहरू_foreach(साँचो, keymap) {
        /* Skip keys with no explicit values */
        यदि (साँचो->explicit &&
            !write_साँचो(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_साँचो_मानहरू, pretty, buf,
                       &buf2, साँचो))
        {
            मुक्त(buf2.buf);
            फर्काऊ false;
        }
    }
    मुक्त(buf2.buf);

    यदि (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        फर्काऊ false;

    copy_to_buf(buf, "};\n\n");
    फर्काऊ true;
}

स्थिर गणना xkb_status
write_keymap(अचर संरचना xkb_keymap *keymap,
             अचर संरचना xkb_keymap_serialize_config *config,
             संरचना buf *buf, संरचना xkb_keymap_serialize_result *result)
{
    साँचो_name_substitutions substitutions = darray_new();
    यदि (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_साँचोहरू(keymap, &substitutions))
    {
        फर्काऊ false;
    }

    अचर साँचो_name_substitutions * अचर substitutions_ptr =
        (darray_empty(substitutions)) ? रिक्त : &substitutions;

    अचर xkb_layout_index_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    अचर bool ok = (
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
    फर्काऊ ok ? XKB_SUCCESS : -1;
}

गणना xkb_status
text_v1_keymap_serialize(
        अचर संरचना xkb_keymap *keymap,
        अचर संरचना xkb_keymap_serialize_config *config,
        संरचना xkb_keymap_serialize_result *result
)
{
    संरचना buf buf = { रिक्त, 0, 0 };

    अचर गणना xkb_status status =
        write_keymap(keymap, config, &buf, result);
    यदि (status != XKB_SUCCESS) {
        मुक्त(buf.buf);
        result->serialized = रिक्त;
        फर्काऊ status;
    }

    result->serialized = buf.buf;
    result->length = buf.size + 1;
    result->layouts = config->layouts;

    फर्काऊ XKB_SUCCESS;
}
