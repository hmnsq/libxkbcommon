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
    अक्षर *buf;
    माप_प्रकार size;
    माप_प्रकार alloc;
};

#define xkb_abs(n) _Generic((n),                \
                            पूर्णांक: असल,           \
                            दीर्घ पूर्णांक: द_असल,     \
                            पूर्वनिर्धारित: llabs )((n))

स्थिर bool
do_realloc(संरचना buf *buf, माप_प्रकार at_least)
{
    buf->alloc += BUF_CHUNK_SIZE;
    जर (at_least >= BUF_CHUNK_SIZE)
        buf->alloc += at_least;

    अक्षर *स्थिरांक new = पुनः_स्मृति(buf->buf, buf->alloc);
    जर (!new)
        परत false;

    buf->buf = new;
    परत true;
}

ATTR_PRINTF(2, 3) स्थिर bool
check_write_buf(संरचना buf *buf, स्थिरांक अक्षर *fmt, ...)
{
    बहु_सूची args;
    पूर्णांक printed;
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

    जर (printed < 0) {
        /* Some error */
        जा err;
    } अन्यथा जर ((माप_प्रकार)printed >= available) {
        /* Not enough space: need to realloc */
        जर (!do_realloc(buf, (माप_प्रकार)printed + 1))
            जा err;

        /* The buffer has enough space now. */
        available = buf->alloc - buf->size;
        प्रतिज्ञा(buf->buf && (माप_प्रकार) printed < available);
        बहु_शुरू(args, fmt);
        printed = vsnprintf(buf->buf + buf->size, available, fmt, args);
        बहु_पूर्ण(args);

        जर (printed < 0 || (माप_प्रकार) printed >= available)
            जा err;
    }

    buf->size += (माप_प्रकार)printed;
    परत true;

err:
    मुक्त(buf->buf);
    buf->buf = रिक्त;
    परत false;
}

#define write_buf(buf, ...) करा { \
    जर (!check_write_buf(buf, __VA_ARGS__)) \
        परत false; \
} पर्यंत (0)

स्थिर bool
check_copy_to_buf(संरचना buf *buf, स्थिरांक अक्षर* source, माप_प्रकार len)
{
    जर (len == 0)
        परत true;

    स्थिरांक माप_प्रकार available = buf->alloc - buf->size;
    जर (len >= available &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        मुक्त(buf->buf);
        buf->buf = रिक्त;
        परत false;
    }

    स_नकल(buf->buf + buf->size, source, len);
    buf->size += len;
    /* Append NULL byte */
    buf->buf[buf->size] = '\0';
    परत true;
}

#define copy_to_buf_len(buf, source, len) करा { \
    जर (!check_copy_to_buf(buf, source, len)) \
        परत false; \
} पर्यंत (0)

#define copy_to_buf(buf, source) \
    copy_to_buf_len(buf, source, आकार(source) - 1)

स्थिर bool
check_write_string_literal(संरचना buf *buf, स्थिरांक अक्षर* string)
{
    copy_to_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    माप_प्रकार pending_start = 0;
    माप_प्रकार current = 0;
    अक्षर escape_buffer[] = {'\\', '0', '0', '0', '0'};
    साठी (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        निवड (string[current]) {
        स्थिती '\n':
            /* `\n` would break strings */
            escape_buffer[1] = 'n';
            escape_len = 2;
            थांबा;
        स्थिती '\\':
            /* `\` would create escape sequences */
            escape_buffer[1] = '\\';
            escape_len = 2;
            थांबा;
        पूर्वनिर्धारित:
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
            जर (unlikely((अचिन्हित अक्षर)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_buffer[1] = '0';
                escape_buffer[2] = '0';
                escape_buffer[3] = (अक्षर)('0' + (string[current] >> 3));
                escape_buffer[4] = (अक्षर)('0' + (string[current] & 0x7));
                escape_len = 5;
            } अन्यथा {
                /* Expand the chunk */
                सुरूठेवा;
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
    परत true;
}

#define write_buf_string_literal(buf, string) करा { \
    जर (!check_write_string_literal(buf, string)) \
        परत false; \
} पर्यंत (0)

स्थिर शून्य
delete_last_char(संरचना buf *buf, अक्षर c)
{
    जर (buf->size && buf->buf[buf->size - 1] == c)
        buf->buf[--buf->size] = '\0';
}

/** Entry for a name substituion LUT */
संरचना किल्ली_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
स्थिर पूर्णांक
cmp_किल्ली_name_substitution(स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
    स्थिरांक xkb_atom_t s1 = ((संरचना किल्ली_name_substitution *) a)->source;
    स्थिरांक xkb_atom_t s2 = ((संरचना किल्ली_name_substitution *) b)->source;
    परत (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

प्रकारव्याख्या darray(संरचना किल्ली_name_substitution) किल्ली_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
स्थिर bool
rename_long_किल्ल्या(स्थिरांक संरचना xkb_keymap *keymap,
                 किल्ली_name_substitutions *substitutions)
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
    अक्षर prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', last
    };
    static_assert(आकार(prefixes) == last - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    स्थिरांक संरचना xkb_किल्ली *किल्ली;
    xkb_किल्ल्या_foreach(किल्ली, keymap) {
        जर (किल्ली->name == XKB_ATOM_NONE)
            सुरूठेवा;

        स्थिरांक अक्षर * स्थिरांक name = xkb_atom_text(keymap->ctx, किल्ली->name);
        स्थिरांक माप_प्रकार len = strlen_safe(name);
        जर (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (संरचना किल्ली_name_substitution) {
                .source = किल्ली->name,
                .keycode = किल्ली->keycode,
                .target = किल्ली->name
            });
        } अन्यथा जर (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    साठी (darray_size_t a = 0; a < keymap->अंक_किल्ली_aliases; a++) {
        स्थिरांक संरचना xkb_किल्ली_alias * स्थिरांक alias = &keymap->किल्ली_aliases[a];
        स्थिरांक अक्षर * स्थिरांक name = xkb_atom_text(keymap->ctx, alias->alias);
        स्थिरांक माप_प्रकार len = strlen_safe(name);
        जर (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (संरचना किल्ली_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } अन्यथा जर (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    जर darray_empty(*substitutions) {
        /* Nothing to do */
        परत true;
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
    अक्षर *prefix = prefixes;
    स्थिरांक अक्षर *prefix_max = prefixes + आकार(prefixes) - 1;
    पर्यंत (*prefix == invalid && prefix < prefix_max)
        prefix++;

    /* Keycode prefix: prefer ‘0’ if available */
    अक्षर keycode_prefix;
    जर (prefixes[last - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_prefix = last;
        prefixes[last - first] = invalid;
    } अन्यथा {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_prefix = *prefix;
        जर (unlikely(keycode_prefix == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            परत true;
        }
        /* Update current prefix */
        *prefix = invalid;
        पर्यंत (*prefix == invalid && prefix < prefix_max)
            prefix++;
    }

    /* Proceed renaming */
    uint16_t next_id = 1;
    संरचना किल्ली_name_substitution *sub;
    अक्षर buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        जर (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, आकार(buf), "%c%03"PRIx32,
                     keycode_prefix, sub->keycode);
            स्थिरांक xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, आकार(buf) - 2);
            जर (target == XKB_ATOM_NONE)
                जा error;
            sub->target = target;
        } अन्यथा जर (*prefix != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, आकार(buf), "%c%03"PRIx16, *prefix, next_id);
            स्थिरांक xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, आकार(buf) - 2);
            जर (target == XKB_ATOM_NONE)
                जा error;
            sub->target = target;
            जर (++next_id > 0xfff) {
                *prefix = invalid;
                /* Get next available prefix */
                पर्यंत (*prefix == invalid && prefix < prefix_max)
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
    क्विक(darray_वस्तुसंच(*substitutions), darray_size(*substitutions),
          आकार(*darray_वस्तुसंच(*substitutions)), &cmp_किल्ली_name_substitution);

    परत true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate key name substitution\n");
    परत false;
}

/** Substitute a key name with a valid X11 key name */
स्थिर xkb_atom_t
substitute_name(स्थिरांक किल्ली_name_substitutions *substitutions, xkb_atom_t name)
{
    /*द्विमान शोध*/
    darray_size_t first = 0;
    darray_size_t last = darray_size(*substitutions);

    पर्यंत (last > first) {
        darray_size_t mid = first + (last - 1 - first) / 2;
        स्थिरांक xkb_atom_t source = darray_item(*substitutions, mid).source;
        जर (source < name)
            first = mid + 1;
        अन्यथा जर (source > name)
            last = mid;
        अन्यथा /* found it */
            परत darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    परत name;
}

स्थिर bool
write_vmods(स्थिरांक संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
            bool explicit, संरचना buf *buf)
{
    स्थिरांक संरचना xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        जर (!has_some) {
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
        स्थिरांक bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        जर ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
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

    जर (has_some)
        copy_to_buf(buf, ";\n\n");

    परत true;
}

स्थिर bool
write_keycodes(स्थिरांक संरचना xkb_keymap *keymap,
               स्थिरांक किल्ली_name_substitutions *substitutions,
               गणना xkb_keymap_serialize_flags flags, संरचना buf *buf)
{
    स्थिरांक संरचना xkb_किल्ली *किल्ली;
    xkb_led_index_t idx;
    स्थिरांक संरचना xkb_led *led;

    स्थिरांक bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    जर (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_किल्ली_code, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_किल्ली_code, 255));

    xkb_किल्ल्या_foreach(किल्ली, keymap) {
        जर (किल्ली->name == XKB_ATOM_NONE)
            सुरूठेवा;

        स्थिरांक xkb_atom_t name = (substitutions == रिक्त)
            ? किल्ली->name
            : substitute_name(substitutions, किल्ली->name);
        जर (pretty)
            write_buf(buf, "\t%-20s", किल्ली_Name_Text(keymap->ctx, name));
        अन्यथा
            write_buf(buf, "\t%s", किल्ली_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", किल्ली->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        जर (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_to_buf(buf, ";\n");
        }

    साठी (darray_size_t i = 0; i < keymap->अंक_किल्ली_aliases; i++) {
        स्थिरांक xkb_atom_t alias = (substitutions == रिक्त)
            ? keymap->किल्ली_aliases[i].alias
            : substitute_name(substitutions, keymap->किल्ली_aliases[i].alias);
        स्थिरांक xkb_atom_t real = (substitutions == रिक्त)
            ? keymap->किल्ली_aliases[i].real
            : substitute_name(substitutions, keymap->किल्ली_aliases[i].real);

        जर (pretty)
            write_buf(buf, "\talias %-14s",
                      किल्ली_Name_Text(keymap->ctx, alias));
        अन्यथा
            write_buf(buf, "\talias %s",
                      किल्ली_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  किल्ली_Name_Text(keymap->ctx, real));
    }

    copy_to_buf(buf, "};\n\n");
    परत true;
}

स्थिर bool
write_types(स्थिरांक संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
            गणना xkb_keymap_serialize_flags flags, संरचना buf *buf)
{
    जर (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    जर (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        परत false;

    स्थिरांक bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    साठी (darray_size_t i = 0; i < keymap->अंक_types; i++) {
        स्थिरांक संरचना xkb_किल्ली_type * स्थिरांक type = &keymap->types[i];
        जर (!type->required && drop_unused)
            सुरूठेवा;

        copy_to_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_to_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        साठी (darray_size_t j = 0; j < type->अंक_entries; j++) {
            स्थिरांक अक्षर *str;
            स्थिरांक संरचना xkb_किल्ली_type_entry *entry = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            जर (entry->level == 0 && entry->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मूल्ये))
                सुरूठेवा;

            str = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              entry->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, entry->level + 1);

            जर (entry->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           entry->preserve.mods));
        }

        साठी (xkb_level_index_t n = 0; n < type->अंक_level_names; n++)
            जर (type->level_names[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_names[n]));
                copy_to_buf(buf, ";\n");
            }

        copy_to_buf(buf, "\t};\n");
    }

    copy_to_buf(buf, "};\n\n");
    परत true;
}

स्थिर bool
write_led_map(स्थिरांक संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
              bool defaults, संरचना buf *buf, स्थिरांक संरचना xkb_led *led)
{
    copy_to_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_to_buf(buf, " {\n");

    जर (led->which_groups) {
        जर (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      LedStateMaskText(keymap->ctx, groupComponentMaskNames,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    जर (led->which_mods) {
        जर (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      LedStateMaskText(keymap->ctx, modComponentMaskNames,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    जर (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  ControlMaskText(keymap->ctx, format, led->ctrls));
    }

    copy_to_buf(buf, "\t};\n");
    परत true;
}

स्थिर स्थिरांक अक्षर *
affect_lock_text(गणना xkb_action_flags flags, bool show_both)
{
    निवड (flags & (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK)) {
    स्थिती 0:
        परत show_both ? ",affect=both" : "";
    स्थिती ACTION_LOCK_NO_UNLOCK:
        परत ",affect=lock";
    स्थिती ACTION_LOCK_NO_LOCK:
        परत ",affect=unlock";
    स्थिती ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK:
        परत ",affect=neither";
    पूर्वनिर्धारित:
        परत "";
    }
}

#define चिन्ह_PADDING 15
#define ACTION_PADDING 30

स्थिर bool
write_action(स्थिरांक संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
             xkb_layout_index_t max_groups, संरचना buf *buf,
             स्थिरांक संघ xkb_action *action,
             स्थिरांक अक्षर *prefix, स्थिरांक अक्षर *suffix)
{
    स्थिरांक अक्षर *type;
    स्थिरांक अक्षर *args = रिक्त;

    जर (!prefix)
        prefix = "";
    जर (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    निवड (action->type) {
    स्थिती ACTION_TYPE_MOD_SET:
    स्थिती ACTION_TYPE_MOD_LATCH:
    स्थिती ACTION_TYPE_MOD_LOCK:
        जर (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            args = "modMapMods";
        अन्यथा
            args = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        जर (unlockOnPress && !isModsUnLockOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->group.flags & ACTION_LATCH_ON_PRESS);
        जर (latchOnPress && !isModsLatchOnPressSupported(format)) {
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
        थांबा;

    स्थिती ACTION_TYPE_GROUP_SET:
    स्थिती ACTION_TYPE_GROUP_LATCH:
    स्थिती ACTION_TYPE_GROUP_LOCK:
        जर ((uint32_t) xkb_abs(action->group.group) <
            (max_groups + !(action->group.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool lockOnRelease = action->type == ACTION_TYPE_GROUP_LOCK &&
                                 (action->group.flags & ACTION_LOCK_ON_RELEASE);
            जर (lockOnRelease && !isGroupLockOnReleaseSupported(format)) {
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
            जा void_action;
        }
        थांबा;

    स्थिती ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", prefix, type, suffix);
        थांबा;

    स्थिती ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", prefix, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        थांबा;

    स्थिती ACTION_TYPE_PTR_LOCK:
        args = affect_lock_text(action->btn.flags, true);
        /* fallthrough */
    स्थिती ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", prefix, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        जर (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        अन्यथा
            copy_to_buf(buf, "default");
        जर (action->btn.मोजणी) {
            प्रतिज्ञा(action->type != ACTION_TYPE_PTR_LOCK);
            write_buf(buf, ",count=%u", action->btn.मोजणी);
        }
        जर (args)
            write_buf(buf, "%s", args);
        write_buf(buf, ")%s", suffix);
        थांबा;

    स्थिती ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", prefix, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.मूल्य >= 0) ? "+" : "",
                  action->dflt.मूल्य);
        write_buf(buf, ")%s", suffix);
        थांबा;

    स्थिती ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", prefix, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        थांबा;

    स्थिती ACTION_TYPE_CTRL_SET:
    स्थिती ACTION_TYPE_CTRL_LOCK:
        write_buf(buf, "%s%s(controls=%s%s)%s", prefix, type,
                  ControlMaskText(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_LOCK
                    ? affect_lock_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        थांबा;

    स्थिती ACTION_TYPE_REDIRECT_किल्ली: {
        write_buf(buf, "%s%s(", prefix, type);
        स्थिरांक संरचना xkb_किल्ली * स्थिरांक किल्ली = Xkb_किल्ली(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        जर (किल्ली)
            write_buf(buf, "keycode=%s", किल्ली_Name_Text(keymap->ctx, किल्ली->name));
        जर (action->redirect.affect_mods) {
            xkb_mod_mask_t mask;
            mask = (action->redirect.affect_mods & action->redirect.mods);
            जर (mask)
                write_buf(buf, ",modifiers=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
            mask = (action->redirect.affect_mods & ~action->redirect.mods);
            जर (mask)
                write_buf(buf, ",clearMods=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
        }
        write_buf(buf, ")%s", suffix);
        थांबा;
    }

    स्थिती ACTION_TYPE_UNKNOWN:
    स्थिती ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", prefix, suffix);
        थांबा;

    स्थिती ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        जर (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      prefix, suffix);
        अन्यथा
            write_buf(buf, "%sVoidAction()%s", prefix, suffix);
        थांबा;

    पूर्वनिर्धारित:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
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
        थांबा;
    }

    परत true;
}

स्थिर स्थिरांक संघ xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

स्थिर bool
write_actions(स्थिरांक संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
              xkb_layout_index_t max_groups, संरचना buf *buf, संरचना buf *buf2,
              स्थिरांक संरचना xkb_किल्ली *किल्ली, xkb_layout_index_t group)
{
    स्थिर स्थिरांक संघ xkb_action noAction = { .type = ACTION_TYPE_NONE };

    साठी (xkb_level_index_t level = 0; level < Xkb_किल्ली_अंक_Levels(किल्ली, group);
         level++) {

        जर (level != 0)
            copy_to_buf(buf, ", ");

        स्थिरांक संघ xkb_action *actions = रिक्त;
        xkb_action_मोजणी_t मोजणी = xkb_keymap_किल्ली_get_actions_by_level(
            keymap, किल्ली, group, level, &actions
        );

        जर (मोजणी > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            मोजणी = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->size = 0;
        जर (मोजणी == 0) {
            जर (!write_action(keymap, format, max_groups,
                              buf2, &noAction, रिक्त, रिक्त))
                परत false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        अन्यथा जर (मोजणी == 1) {
            जर (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), रिक्त, रिक्त))
                परत false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        अन्यथा {
            copy_to_buf(buf2, "{ ");
            साठी (xkb_action_मोजणी_t k = 0; k < मोजणी; k++) {
                जर (k != 0)
                    copy_to_buf(buf2, ", ");
                माप_प्रकार old_size = buf2->size;
                जर (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), रिक्त, रिक्त))
                    परत false;
                /* Check if padding is necessary */
                जर (buf2->size >= old_size + ACTION_PADDING)
                    सुरूठेवा;
                /* Compute and write padding, then write the action again */
                स्थिरांक पूर्णांक padding = (पूर्णांक)(old_size + ACTION_PADDING - buf2->size);
                buf2->size = old_size;
                write_buf(buf2, "%*s", padding, "");
                जर (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), रिक्त, रिक्त))
                    परत false;
            }
            copy_to_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    परत true;
}

स्थिर bool
write_action_defaults(स्थिरांक संरचना xkb_keymap *keymap,
                      गणना xkb_keymap_format format,
                      स्थिरांक संघ xkb_action *action,
                      संरचना buf *buf)
{
    स्थिरांक अक्षर *type = ActionTypeText(action->type);

#define PREFIX "\t"

    निवड (action->type) {
    स्थिती ACTION_TYPE_MOD_SET:
    स्थिती ACTION_TYPE_MOD_LATCH:
    स्थिती ACTION_TYPE_MOD_LOCK:
        प्रतिज्ञा(action->mods.flags == 0);
        प्रतिज्ञा(action->mods.mods.mods == 0);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        जर (action->type != ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        जर (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, PREFIX"%s.latchOnPress = false;\n", type);
        जर (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        जर (action->type == ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        जर (isModsUnLockOnPressSupported(format))
            write_buf(buf, PREFIX"%s.unlockOnPress = false;\n", type);
        थांबा;

    स्थिती ACTION_TYPE_GROUP_SET:
    स्थिती ACTION_TYPE_GROUP_LATCH:
    स्थिती ACTION_TYPE_GROUP_LOCK:
        प्रतिज्ञा(action->group.flags == 0);
        प्रतिज्ञा(action->group.group == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.group = +0;\n", type);
        जर (action->type != ACTION_TYPE_GROUP_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        जर (action->type == ACTION_TYPE_GROUP_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        जर (action->type == ACTION_TYPE_GROUP_LOCK &&
            isGroupLockOnReleaseSupported(format))
            write_buf(buf, PREFIX"%s.lockOnRelease = false;\n", type);
        थांबा;


    स्थिती ACTION_TYPE_PTR_MOVE:
        प्रतिज्ञा(action->ptr.x == 0);
        प्रतिज्ञा(action->ptr.y == 0);
        प्रतिज्ञा(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.x = +0;\n", type);
        write_buf(buf, PREFIX"%s.y = +0;\n", type);
        write_buf(buf, PREFIX"%s.accel = true;\n", type);
        थांबा;

    स्थिती ACTION_TYPE_PTR_LOCK:
    स्थिती ACTION_TYPE_PTR_BUTTON:
        प्रतिज्ञा(action->btn.flags == 0);
        प्रतिज्ञा(action->btn.button == 0);
        प्रतिज्ञा(action->btn.मोजणी == 0);
        write_buf(buf, PREFIX"%s.button = 0;\n", type);
        write_buf(buf, PREFIX"%s.count = 0;\n", type);
        जर (action->type == ACTION_TYPE_PTR_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        थांबा;

    स्थिती ACTION_TYPE_PTR_DEFAULT:
        प्रतिज्ञा(action->dflt.flags == 0);
        प्रतिज्ञा(action->dflt.मूल्य == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.button = +1;\n", type);
        थांबा;

    स्थिती ACTION_TYPE_SWITCH_VT:
        प्रतिज्ञा(action->screen.flags == ACTION_SAME_SCREEN);
        प्रतिज्ञा(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.screen = +0;\n", type);
        write_buf(buf, PREFIX"%s.same = true;\n", type);
        थांबा;

    स्थिती ACTION_TYPE_CTRL_SET:
    स्थिती ACTION_TYPE_CTRL_LOCK:
        प्रतिज्ञा(action->ctrls.flags == 0);
        प्रतिज्ञा(action->ctrls.ctrls == 0);
        write_buf(buf, PREFIX"%s.controls = 0;\n", type);
        जर (action->type == ACTION_TYPE_CTRL_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        थांबा;

    स्थिती ACTION_TYPE_REDIRECT_किल्ली:
        प्रतिज्ञा(action->redirect.keycode == keymap->redirect_किल्ली_auto);
        प्रतिज्ञा(action->redirect.affect_mods == 0);
        प्रतिज्ञा(action->redirect.mods == 0);
        write_buf(buf, PREFIX"%s.keycode = auto;\n", type);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        write_buf(buf, PREFIX"%s.clearMods = 0;\n", type);
        थांबा;

    पूर्वनिर्धारित:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
                      "Missing action type");
    }

#undef PREFIX

    परत true;
}

स्थिर bool
write_actions_defaults(स्थिरांक संरचना xkb_keymap *keymap,
                       गणना xkb_keymap_format format, संरचना buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    साठी (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_अंक_ENTRIES; a++) {
        स्थिरांक संघ xkb_action * restrict स्थिरांक action = &defaults.actions[a];
        जर (!write_action_defaults(keymap, format, action, buf))
            परत false;
    }
    copy_to_buf(buf, "\n");

    परत true;
}

स्थिर स्थिरांक संरचना xkb_sym_interpret fallback_interpret = {
    .sym = XKB_किल्ली_Void_चिन्ह,
    .required = true,
    .repeat = FALLBACK_INTERPRET_किल्ली_REPEAT,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .अंक_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

स्थिर bool
write_compat(स्थिरांक संरचना xkb_keymap * restrict keymap,
             गणना xkb_keymap_format format, xkb_layout_index_t max_groups,
             गणना xkb_keymap_serialize_flags flags, bool * restrict some_interp,
             संरचना buf *buf)
{
    स्थिरांक bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    स्थिरांक bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मूल्ये);
    स्थिरांक bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    स्थिरांक bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_किल्ली_मूल्ये) && drop_unused);
    /* TODO: print all default values if the explicit flag is set */

    जर (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_compatibility {\n");

    जर (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        परत false;

    जर (defaults && !write_actions_defaults(keymap, format, buf))
        परत false;

    copy_to_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_to_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    स्थिरांक bool use_fallback_interpret =
        (!keymap->अंक_sym_interprets || drop_interprets);
    स्थिरांक darray_size_t अंक_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->अंक_sym_interprets);
    स्थिरांक संरचना xkb_sym_interpret* स्थिरांक sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    साठी (darray_size_t i = 0; i < अंक_sym_interprets; i++) {
        स्थिरांक संरचना xkb_sym_interpret *si = &sym_interprets[i];
        जर (!si->required && drop_unused)
            सुरूठेवा;

        *some_interp = true;

        copy_to_buf(buf, "\tinterpret ");
        जर (!si->sym) {
            copy_to_buf(buf, "Any");
        } अन्यथा जर (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } अन्यथा {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->match),
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        जर (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      ModIndexText(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        जर (si->level_one_only) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } अन्यथा जर (defaults) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        जर (si->repeat) {
            copy_to_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } अन्यथा जर (defaults) {
            copy_to_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        स्थिरांक संघ xkb_action *action = रिक्त;
        xkb_action_मोजणी_t action_मोजणी = si->अंक_actions;
        जर (action_मोजणी <= 1) {
            action = &si->a.action;
        } अन्यथा जर (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_मोजणी = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        जर (action_मोजणी > 1) {
            copy_to_buf(buf, "\n\t\taction= {");
            स्थिरांक अक्षर suffix[] = ", ";
            साठी (xkb_action_मोजणी_t k = 0; k < si->अंक_actions; k++) {
                जर (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    परत false;
            }
            buf->size -= आकार(suffix) - 1; /* trailing comma */
            copy_to_buf(buf, "};");
            has_explicit_properties = true;
        } अन्यथा जर (action_मोजणी == 1) {
            जर (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                परत false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    जर (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        अंक_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].match == fallback_interpret.match &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].अंक_actions == fallback_interpret.अंक_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    स्थिरांक संरचना xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        जर ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            परत false;
        }
    }

    copy_to_buf(buf, "};\n\n");

    परत true;
}

स्थिर bool
write_keysyms(स्थिरांक संरचना xkb_keymap *keymap,
              संरचना buf *buf, संरचना buf *buf2,
              स्थिरांक संरचना xkb_किल्ली *किल्ली, xkb_layout_index_t group,
              bool pretty, bool show_actions)
{
    स्थिरांक अचिन्हित पूर्णांक padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : चिन्ह_PADDING)
        : 0;
    साठी (xkb_level_index_t level = 0; level < Xkb_किल्ली_अंक_Levels(किल्ली, group);
         level++) {
        स्थिरांक xkb_keysym_t *syms;
        पूर्णांक अंक_syms;

        जर (level != 0)
            copy_to_buf(buf, ", ");

        अंक_syms = xkb_keymap_किल्ली_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (संरचना xkb_keymap *)keymap,
            किल्ली->keycode, group, level, &syms
        );

        स्थिरांक xkb_keysym_t no_चिन्ह = XKB_किल्ली_No_चिन्ह;
        जर (अंक_syms == 0) {
            अंक_syms = 1;
            syms = &no_चिन्ह;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        जर (अंक_syms == 1) {
            जर (pretty || syms[0] == XKB_किल्ली_No_चिन्ह)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            अन्यथा
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } अन्यथा {
            जर (pretty) {
                buf2->size = 0;
                copy_to_buf(buf2, "{ ");
                साठी (पूर्णांक s = 0; s < अंक_syms; s++) {
                    जर (s != 0)
                        copy_to_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_to_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } अन्यथा {
                copy_to_buf(buf, "{");
                साठी (पूर्णांक s = 0; s < अंक_syms; s++) {
                    जर (s != 0)
                        copy_to_buf(buf, ",");
                    जर (syms[s] == XKB_किल्ली_No_चिन्ह)
                        copy_to_buf(buf, "NoSymbol");
                    अन्यथा
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_to_buf(buf, "}");
            }
        }
    }

    परत true;
}

स्थिर bool
write_किल्ली(स्थिरांक संरचना xkb_keymap *keymap,
          स्थिरांक संरचना xkb_keymap_serialize_config *config,
          स्थिरांक किल्ली_name_substitutions *substitutions,
          xkb_layout_index_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          संरचना buf *buf, संरचना buf *buf2, स्थिरांक संरचना xkb_किल्ली *किल्ली)
{
    bool simple = true;

    xkb_layout_mask_t layouts = config->layouts;
    जर (popcount32(layouts) != keymap->अंक_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_index_t l = 0;
        साठी (xkb_layout_mask_t ls = layouts; ls; ls >>= 1, l++) {
            जर (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                स्थिरांक xkb_layout_index_t effective =
                    xkb_keymap_किल्ली_effective_layout(किल्ली, l);
                जर (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } अन्यथा {
        layouts &= ((UINT64_C(1) << किल्ली->अंक_groups) - 1);
    }
    स्थिरांक xkb_layout_index_t groups_मोजणी = popcount32(layouts);

    स्थिरांक xkb_atom_t name = (substitutions == रिक्त)
        ? किल्ली->name
        : substitute_name(substitutions, किल्ली->name);

    जर (pretty)
        write_buf(buf, "\tkey %-20s {", किल्ली_Name_Text(keymap->ctx, name));
    अन्यथा
        write_buf(buf, "\tkey %s {", किल्ली_Name_Text(keymap->ctx, name));

    जर ((किल्ली->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        जर (!explicit) {
            साठी (xkb_layout_index_t group = 1; group < किल्ली->अंक_groups; group++) {
                जर (!((UINT32_C(1) << group) & layouts))
                    सुरूठेवा;

                जर (किल्ली->groups[group].type != किल्ली->groups[0].type) {
                    multi_type = true;
                    थांबा;
                }
            }
        }

        जर (multi_type) {
            xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
            साठी (xkb_layout_index_t group = 0; group < किल्ली->अंक_groups; group++) {
                जर (!((UINT32_C(1) << group) & layouts))
                    सुरूठेवा;

                new_group++;

                जर (!किल्ली->groups[group].explicit_type && !explicit)
                    सुरूठेवा;

                स्थिरांक संरचना xkb_किल्ली_type * स्थिरांक type = किल्ली->groups[group].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_group + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_to_buf(buf, ",");
            }
        }
        अन्यथा {
            स्थिरांक संरचना xkb_किल्ली_type * स्थिरांक type = किल्ली->groups[0].type;
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
    स्थिरांक bool explicit_actions = (किल्ली->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #define require_explicit(किल्ली, prop,                                       \
                             मूल्य, initial_मूल्य, default_interpret_मूल्य) ( \
        /* Explicit value in the source */                                    \
        ((किल्ली)->explicit & (prop)) ||                                         \
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
                ((किल्ली)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (मूल्य) != (initial_मूल्य) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_मूल्य) != (default_interpret_मूल्य)                  \
            )                                                                 \
        )                                                                     \
    )

    जर (require_explicit(किल्ली, EXPLICIT_REPEAT, किल्ली->repeats,
                         DEFAULT_किल्ली_REPEAT, DEFAULT_INTERPRET_किल्ली_REPEAT)) {
        जर (किल्ली->repeats)
            copy_to_buf(buf, "\n\t\trepeat= Yes,");
        अन्यथा
            copy_to_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    जर (require_explicit(किल्ली, EXPLICIT_VMODMAP, किल्ली->vmodmap,
                         (xkb_mod_mask_t) DEFAULT_किल्ली_VMODMAP,
                         (xkb_mod_mask_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              किल्ली->vmodmap));
        simple = false;
    }

    #undef require_explicit

    निवड (किल्ली->out_of_range_group_policy) {
    स्थिती XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        copy_to_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        थांबा;

    स्थिती XKB_LAYOUT_OUT_OF_RANGE_REDIRECT: {
        जर (!groups_मोजणी) {
            /* No layout to redirect to: skip */
            थांबा;
        }

        xkb_layout_mask_t layout_mask =
            (UINT32_C(1) << किल्ली->out_of_range_group_संख्या);
        जर (!(layouts & layout_mask)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     किल्ली_Name_Text(keymap->ctx, name),
                     किल्ली->out_of_range_group_संख्या + 1);
            layout_mask = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        स्थिरांक xkb_layout_mask_t खालचा = (layouts & (layout_mask - 1));
        स्थिरांक xkb_layout_index_t new = popcount32(खालचा);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        थांबा;
    }

    स्थिती XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    पूर्वनिर्धारित:
        थांबा;
    }

    जर (किल्ली->overlays) {
        xkb_overlay_mask_t remaining = किल्ली->overlays;

        स्थिरांक xkb_overlay_index_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_मोजणी == 8, "invalid right shift");
        स्थिरांक xkb_overlay_mask_t valid =
            (xkb_overlay_mask_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        जर (remaining != किल्ली->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, किल्ली_Name_Text(keymap->ctx, name),
                     remaining, किल्ली->overlays & ~valid);
        }

        जर (remaining && !किल्ली->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            स्थिरांक xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
            );
            /* get its index */
            स्थिरांक xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     किल्ली_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        जर (remaining)
            simple = false;

        स्थिरांक संरचना xkb_किल्ली * स्थिरांक *overlays_किल्ल्या = (किल्ली->overlays_inline)
                                                    ? &किल्ली->overlay_किल्ली
                                                    : किल्ली->overlays_किल्ल्या;
        पर्यंत (remaining) {
            /* isolate lowest set bit */
            स्थिरांक xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
            );
            /* get its index */
            स्थिरांक xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            जर (किल्ली->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            स्थिरांक संरचना xkb_किल्ली *overlay_किल्ली = *(overlays_किल्ल्या++);

            स्थिरांक xkb_atom_t overlay_किल्ली_name = (substitutions == रिक्त)
                ? overlay_किल्ली->name
                : substitute_name(substitutions, overlay_किल्ली->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, किल्ली_Name_Text(keymap->ctx, overlay_किल्ली_name));
        }
    }

    जर (groups_मोजणी > 1 || explicit_actions || explicit)
        simple = false;

    जर (simple) {
        जर (groups_मोजणी == 0) {
            /* Remove trailing comma */
            delete_last_char(buf, ',');
            जर (buf->buf[buf->size - 1] == ',')
                buf->size--;
        } अन्यथा {
            copy_to_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_index_t group = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            group = xkb_keymap_किल्ली_effective_layout(किल्ली, group);
            जर (!write_keysyms(keymap, buf, buf2, किल्ली, group, pretty, false))
                परत false;
            copy_to_buf(buf, " ]");
        }
        copy_to_buf(buf, " };\n");
    }
    अन्यथा {
        xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
        साठी (xkb_layout_index_t group = 0; group < किल्ली->अंक_groups; group++) {
            जर (!((UINT32_C(1) << group) & layouts))
                सुरूठेवा;

            new_group++;

            स्थिरांक bool print_actions =
                /* Group has explicit actions */
                किल्ली->groups[group].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (किल्ली->groups[group].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (किल्ली->groups[group].implicit_actions && !some_interprets);

            जर (new_group != 0)
                copy_to_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_group + 1);

            जर (!write_keysyms(keymap, buf, buf2, किल्ली, group,
                               pretty, print_actions))
                परत false;
            copy_to_buf(buf, " ]");
            जर (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_group + 1);
                जर (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, किल्ली, group))
                    परत false;
                copy_to_buf(buf, " ]");
            }
        }
        जर (!groups_मोजणी)
            delete_last_char(buf, ',');
        copy_to_buf(buf, "\n\t};\n");
    }

    परत true;
}


स्थिर पूर्णांक
cmp_modmap_step_1(स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
    स्थिरांक ModMapEntry *m1 = (स्थिरांक ModMapEntry *)a;
    स्थिरांक ModMapEntry *m2 = (स्थिरांक ModMapEntry *)b;

    जर (!m1->mods) {
        जर (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            परत 0;
        }
        परत +1;
    } अन्यथा जर (!m2->mods) {
        परत -1;
    }

    /* Valid only on step 1 */
    जर (m1->किल्ली_Code < m2->किल्ली_Code)
        परत -1;
    अन्यथा जर (m1->किल्ली_Code > m2->किल्ली_Code)
        परत +1;

    जर (m1->mods < m2->mods)
        परत -1;
    अन्यथा जर (m1->mods > m2->mods)
        परत +1;

    जर (m1->have_चिन्ह < m2->have_चिन्ह)
        परत +1;
    अन्यथा जर (m1->have_चिन्ह > m2->have_चिन्ह)
        परत -1;

    जर (m1->have_चिन्ह) {
        जर (m1->u.किल्ली_Sym < m2->u.किल्ली_Sym)
            परत -1;
        जर (m1->u.किल्ली_Sym > m2->u.किल्ली_Sym)
            परत +1;
    } अन्यथा {
        जर (m1->u.किल्ली_Name < m2->u.किल्ली_Name)
            परत -1;
        जर (m1->u.किल्ली_Name > m2->u.किल्ली_Name)
            परत +1;
    }

    परत 0;
}

स्थिर पूर्णांक
cmp_modmap_step_2(स्थिरांक शून्य *a, स्थिरांक शून्य *b)
{
    स्थिरांक ModMapEntry *m1 = (स्थिरांक ModMapEntry *)a;
    स्थिरांक ModMapEntry *m2 = (स्थिरांक ModMapEntry *)b;

    जर (m1->mods < m2->mods)
        परत -1;
    जर (m1->mods > m2->mods)
        परत +1;

    जर (m1->have_चिन्ह < m2->have_चिन्ह)
        परत -1;
    जर (m1->have_चिन्ह > m2->have_चिन्ह)
        परत +1;

    जर (m1->have_चिन्ह) {
        जर (m1->u.किल्ली_Sym < m2->u.किल्ली_Sym)
            परत -1;
        जर (m1->u.किल्ली_Sym > m2->u.किल्ली_Sym)
            परत +1;
    } अन्यथा {
        जर (m1->किल्ली_Code < m2->किल्ली_Code)
            परत -1;
        जर (m1->किल्ली_Code > m2->किल्ली_Code)
            परत +1;
    }

    परत 0;
}

स्थिर bool
write_modmaps(स्थिरांक संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
              स्थिरांक किल्ली_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, संरचना buf *buf)
{
    जर (!keymap->अंक_modmaps)
        परत true;

    darray(ModMapEntry) modmaps = darray_new();
    darray_from_वस्तुसंच(modmaps, keymap->modmaps, keymap->अंक_modmaps);
    जर (!darray_वस्तुसंच(modmaps))
        परत false;

    /* Sort entries by key code */
    क्विक(darray_वस्तुसंच(modmaps), darray_size(modmaps),
          आकार(*darray_वस्तुसंच(modmaps)), &cmp_modmap_step_1);

    /* Drop disabled entries at the end */
    साठी (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        जर (darray_item(modmaps, m).mods)
            थांबा;
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
    साठी (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ModMapEntry *mm = &darray_item(modmaps, m);
        प्रतिज्ञा(mm->mods);

        जर (mm->किल्ली_Code == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            स्थिरांक xkb_mod_mask_t core_mods = (mm->mods & MOD_REAL_MASK_ALL);
            जर (convert_to_v1_compatibility_entry && core_mods) {
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
            सुरूठेवा;
        }

        current_keycode = mm->किल्ली_Code;
        स्थिरांक संरचना xkb_किल्ली *किल्ली = Xkb_किल्ली(keymap, mm->किल्ली_Code);

        स्थिरांक xkb_mod_mask_t core_mods = (किल्ली->modmap & MOD_REAL_MASK_ALL);
        जर (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((किल्ली->modmap & (किल्ली->modmap - 1)) || किल्ली->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_to_v1_compatibility_entry = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    किल्ली_Name_Text(keymap->ctx, किल्ली->name), किल्ली->modmap);
        } अन्यथा {
            /* V2 format or V1 format with a single core modifier */
            प्रतिज्ञा(किल्ली->modmap);
            mm->mods = किल्ली->modmap;
            /* Force key name */
            mm->have_चिन्ह = false;
            mm->u.किल्ली_Name = किल्ली->name;
            convert_to_v1_compatibility_entry = false;
        }
    }

    जर (darray_empty(modmaps))
        जा निकास;

    /* Sort entries by modifier mask */
    क्विक(darray_वस्तुसंच(modmaps), darray_size(modmaps),
          आकार(*darray_वस्तुसंच(modmaps)), &cmp_modmap_step_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_mask_t current_mods = 0;
    स्थिरांक ModMapEntry *entry;
    darray_foreach(entry, modmaps) {
        जर (!entry->mods)
            सुरूठेवा;

        जर (current_mods != entry->mods) {
            जर (current_mods) {
                copy_to_buf(buf, " };\n");
            }
            current_mods = entry->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } अन्यथा {
            copy_to_buf(buf, ", ");
        }

        जर (entry->have_चिन्ह) {
            जर (pretty || entry->u.किल्ली_Sym == XKB_किल्ली_No_चिन्ह)
                write_buf(buf, "%s", KeysymText(keymap->ctx, entry->u.किल्ली_Sym));
            अन्यथा
                write_buf(buf, "0x%"PRIx32, entry->u.किल्ली_Sym);
        } अन्यथा {
            स्थिरांक xkb_atom_t name = (substitutions == रिक्त)
                ? entry->u.किल्ली_Name
                : substitute_name(substitutions, entry->u.किल्ली_Name);
            write_buf(buf, "%s", किल्ली_Name_Text(keymap->ctx, name));
        }
    }
    जर (current_mods) {
        copy_to_buf(buf, " };\n");
    }

निकास:
    darray_free(modmaps);
    परत true;
}

स्थिर bool
write_symbols(स्थिरांक संरचना xkb_keymap *keymap,
              स्थिरांक संरचना xkb_keymap_serialize_config *config,
              स्थिरांक किल्ली_name_substitutions *substitutions,
              xkb_layout_index_t max_groups, bool some_interp,
              संरचना buf *buf)
{
    स्थिरांक bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    स्थिरांक bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मूल्ये);
    स्थिरांक bool explicit_किल्ली_मूल्ये =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_किल्ली_मूल्ये);
    स्थिरांक bool drop_unused = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    स्थिरांक bool drop_interprets = (explicit_किल्ली_मूल्ये && drop_unused);
    /* TODO: print all default values if explicit flag is set */

    जर (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_symbols {\n");

    bool has_group_names = false;
    xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
    साठी (xkb_layout_index_t group = 0; group < keymap->अंक_group_names; group++) {
        जर (!((UINT32_C(1) << group) & config->layouts))
            सुरूठेवा;

        new_group++;

        जर (keymap->group_names[group]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_group + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->group_names[group]));
            copy_to_buf(buf, ";\n");
            has_group_names = true;
        }
    }
    जर (has_group_names)
        copy_to_buf(buf, "\n");

    जर (defaults && !write_actions_defaults(keymap, config->format, buf))
        परत false;

    संरचना buf buf2 = { रिक्त, 0, 0 };
    स्थिरांक संरचना xkb_किल्ली *किल्ली;
    xkb_किल्ल्या_foreach(किल्ली, keymap) {
        /* Skip keys with no explicit values */
        जर (किल्ली->explicit &&
            !write_किल्ली(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_किल्ली_मूल्ये, pretty, buf,
                       &buf2, किल्ली))
        {
            मुक्त(buf2.buf);
            परत false;
        }
    }
    मुक्त(buf2.buf);

    जर (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        परत false;

    copy_to_buf(buf, "};\n\n");
    परत true;
}

स्थिर गणना xkb_status
write_keymap(स्थिरांक संरचना xkb_keymap *keymap,
             स्थिरांक संरचना xkb_keymap_serialize_config *config,
             संरचना buf *buf, संरचना xkb_keymap_serialize_result *result)
{
    किल्ली_name_substitutions substitutions = darray_new();
    जर (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_किल्ल्या(keymap, &substitutions))
    {
        परत false;
    }

    स्थिरांक किल्ली_name_substitutions * स्थिरांक substitutions_ptr =
        (darray_empty(substitutions)) ? रिक्त : &substitutions;

    स्थिरांक xkb_layout_index_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    स्थिरांक bool ok = (
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
    परत ok ? XKB_SUCCESS : -1;
}

गणना xkb_status
text_v1_keymap_serialize(
        स्थिरांक संरचना xkb_keymap *keymap,
        स्थिरांक संरचना xkb_keymap_serialize_config *config,
        संरचना xkb_keymap_serialize_result *result
)
{
    संरचना buf buf = { रिक्त, 0, 0 };

    स्थिरांक गणना xkb_status status =
        write_keymap(keymap, config, &buf, result);
    जर (status != XKB_SUCCESS) {
        मुक्त(buf.buf);
        result->serialized = रिक्त;
        परत status;
    }

    result->serialized = buf.buf;
    result->length = buf.size + 1;
    result->layouts = config->layouts;

    परत XKB_SUCCESS;
}
