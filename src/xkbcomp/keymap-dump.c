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

#समावेश "config.h"

#समावेश <assert.h>
#समावेश <stdint.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>

#समावेश "xkbcommon/xkbcommon-status.h"
#समावेश "xkbcommon/xkbcommon-keysyms.h"
#समावेश "xkbcommon/xkbcommon.h"
#समावेश "atom.h"
#समावेश "action.h"
#समावेश "context.h"
#समावेश "darray.h"
#समावेश "keymap.h"
#समावेश "messages-codes.h"
#समावेश "text.h"
#समावेश "util-numbers.h"
#समावेश "xkbcomp-priv.h"

#घोषणा BUF_CHUNK_आकार 4096

काष्ठा buf {
    अक्षर *buf;
    माप_प्रकार आकार;
    माप_प्रकार alloc;
};

#घोषणा xkb_abs(n) _Generic((n),                \
                            पूर्णांक: असल,           \
                            दीर्घ पूर्णांक: द_असल,     \
                            शेष: llabs )((n))

अटल bool
do_realloc(काष्ठा buf *buf, माप_प्रकार at_least)
{
    buf->alloc += BUF_CHUNK_आकार;
    अगर (at_least >= BUF_CHUNK_आकार)
        buf->alloc += at_least;

    अक्षर *स्थिर new = पुनः_स्मृति(buf->buf, buf->alloc);
    अगर (!new)
        वापस false;

    buf->buf = new;
    वापस true;
}

ATTR_PRINTF(2, 3) अटल bool
check_write_buf(काष्ठा buf *buf, स्थिर अक्षर *fmt, ...)
{
    बहु_सूची args;
    पूर्णांक printed;
    माप_प्रकार available;

    available = buf->alloc - buf->आकार;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    निश्चित(buf->buf || available == 0);
    बहु_शुरू(args, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->आकार : buf->buf, available,
                        fmt, args);
    बहु_पूर्ण(args);

    अगर (printed < 0) {
        /* Some error */
        जाओ err;
    } अन्यथा अगर ((माप_प्रकार)printed >= available) {
        /* Not enough space: need to realloc */
        अगर (!do_realloc(buf, (माप_प्रकार)printed + 1))
            जाओ err;

        /* The buffer has enough space now. */
        available = buf->alloc - buf->आकार;
        निश्चित(buf->buf && (माप_प्रकार) printed < available);
        बहु_शुरू(args, fmt);
        printed = vsnprintf(buf->buf + buf->आकार, available, fmt, args);
        बहु_पूर्ण(args);

        अगर (printed < 0 || (माप_प्रकार) printed >= available)
            जाओ err;
    }

    buf->आकार += (माप_प्रकार)printed;
    वापस true;

err:
    मुक्त(buf->buf);
    buf->buf = शून्य;
    वापस false;
}

#घोषणा write_buf(buf, ...) करो { \
    अगर (!check_write_buf(buf, __VA_ARGS__)) \
        वापस false; \
} जबतक (0)

अटल bool
check_copy_to_buf(काष्ठा buf *buf, स्थिर अक्षर* source, माप_प्रकार len)
{
    अगर (len == 0)
        वापस true;

    स्थिर माप_प्रकार available = buf->alloc - buf->आकार;
    अगर (len >= available &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        मुक्त(buf->buf);
        buf->buf = शून्य;
        वापस false;
    }

    स_नकल(buf->buf + buf->आकार, source, len);
    buf->आकार += len;
    /* Append NULL byte */
    buf->buf[buf->आकार] = '\0';
    वापस true;
}

#घोषणा copy_to_buf_len(buf, source, len) करो { \
    अगर (!check_copy_to_buf(buf, source, len)) \
        वापस false; \
} जबतक (0)

#घोषणा copy_to_buf(buf, source) \
    copy_to_buf_len(buf, source, माप(source) - 1)

अटल bool
check_write_string_literal(काष्ठा buf *buf, स्थिर अक्षर* string)
{
    copy_to_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    माप_प्रकार pending_start = 0;
    माप_प्रकार current = 0;
    अक्षर escape_बफर[] = {'\\', '0', '0', '0', '0'};
    क्रम (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        चयन (string[current]) {
        हाल '\n':
            /* `\n` would break strings */
            escape_बफर[1] = 'n';
            escape_len = 2;
            अवरोध;
        हाल '\\':
            /* `\` would create escape sequences */
            escape_बफर[1] = '\\';
            escape_len = 2;
            अवरोध;
        शेष:
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
            अगर (unlikely((अचिन्हित अक्षर)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_बफर[1] = '0';
                escape_बफर[2] = '0';
                escape_बफर[3] = (अक्षर)('0' + (string[current] >> 3));
                escape_बफर[4] = (अक्षर)('0' + (string[current] & 0x7));
                escape_len = 5;
            } अन्यथा {
                /* Expand the chunk */
                जारी;
            }
        }
        /* Write pending chunk */
        निश्चित(current >= pending_start);
        copy_to_buf_len(buf, string + pending_start, current - pending_start);
        /* Write escape sequence */
        copy_to_buf_len(buf, escape_बफर, escape_len);
        pending_start = current + 1;
    }
    /* Write remaining chunk */
    निश्चित(current >= pending_start);
    copy_to_buf_len(buf, string + pending_start, current - pending_start);

    copy_to_buf(buf, "\"");
    वापस true;
}

#घोषणा write_buf_string_literal(buf, string) करो { \
    अगर (!check_write_string_literal(buf, string)) \
        वापस false; \
} जबतक (0)

अटल व्योम
delete_last_char(काष्ठा buf *buf, अक्षर c)
{
    अगर (buf->आकार && buf->buf[buf->आकार - 1] == c)
        buf->buf[--buf->आकार] = '\0';
}

/** Entry for a name substituion LUT */
काष्ठा कुंजी_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
अटल पूर्णांक
cmp_कुंजी_name_substitution(स्थिर व्योम *a, स्थिर व्योम *b)
{
    स्थिर xkb_atom_t s1 = ((काष्ठा कुंजी_name_substitution *) a)->source;
    स्थिर xkb_atom_t s2 = ((काष्ठा कुंजी_name_substitution *) b)->source;
    वापस (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

प्रकार darray(काष्ठा कुंजी_name_substitution) कुंजी_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
अटल bool
rename_long_कुंजियाँ(स्थिर काष्ठा xkb_keymap *keymap,
                 कुंजी_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    क्रमागत {
        first = '!',
        last = '0',
        invalid = '\0'
    };
    अक्षर prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', last
    };
    static_assert(माप(prefixes) == last - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    स्थिर काष्ठा xkb_कुंजी *कुंजी;
    xkb_कुंजियाँ_foreach(कुंजी, keymap) {
        अगर (कुंजी->name == XKB_ATOM_NONE)
            जारी;

        स्थिर अक्षर * स्थिर name = xkb_atom_text(keymap->ctx, कुंजी->name);
        स्थिर माप_प्रकार len = strlen_safe(name);
        अगर (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (काष्ठा कुंजी_name_substitution) {
                .source = कुंजी->name,
                .keycode = कुंजी->keycode,
                .target = कुंजी->name
            });
        } अन्यथा अगर (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    क्रम (darray_आकार_t a = 0; a < keymap->अंक_कुंजी_aliases; a++) {
        स्थिर काष्ठा xkb_कुंजी_alias * स्थिर alias = &keymap->कुंजी_aliases[a];
        स्थिर अक्षर * स्थिर name = xkb_atom_text(keymap->ctx, alias->alias);
        स्थिर माप_प्रकार len = strlen_safe(name);
        अगर (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (काष्ठा कुंजी_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } अन्यथा अगर (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    अगर darray_empty(*substitutions) {
        /* Nothing to do */
        वापस true;
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
    स्थिर अक्षर *prefix_max = prefixes + माप(prefixes) - 1;
    जबतक (*prefix == invalid && prefix < prefix_max)
        prefix++;

    /* Keycode prefix: prefer ‘0’ if available */
    अक्षर keycode_prefix;
    अगर (prefixes[last - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_prefix = last;
        prefixes[last - first] = invalid;
    } अन्यथा {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_prefix = *prefix;
        अगर (unlikely(keycode_prefix == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            वापस true;
        }
        /* Update current prefix */
        *prefix = invalid;
        जबतक (*prefix == invalid && prefix < prefix_max)
            prefix++;
    }

    /* Proceed renaming */
    uint16_t अग्र_id = 1;
    काष्ठा कुंजी_name_substitution *sub;
    अक्षर buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        अगर (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, माप(buf), "%c%03"PRIx32,
                     keycode_prefix, sub->keycode);
            स्थिर xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, माप(buf) - 2);
            अगर (target == XKB_ATOM_NONE)
                जाओ error;
            sub->target = target;
        } अन्यथा अगर (*prefix != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, माप(buf), "%c%03"PRIx16, *prefix, अग्र_id);
            स्थिर xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, माप(buf) - 2);
            अगर (target == XKB_ATOM_NONE)
                जाओ error;
            sub->target = target;
            अगर (++अग्र_id > 0xfff) {
                *prefix = invalid;
                /* Get next available prefix */
                जबतक (*prefix == invalid && prefix < prefix_max)
                    prefix++;
                /* Reset counter */
                अग्र_id = 1;
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
    क्विक(darray_वस्तुएँ(*substitutions), darray_आकार(*substitutions),
          माप(*darray_वस्तुएँ(*substitutions)), &cmp_कुंजी_name_substitution);

    वापस true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate key name substitution\n");
    वापस false;
}

/** Substitute a key name with a valid X11 key name */
अटल xkb_atom_t
substitute_name(स्थिर कुंजी_name_substitutions *substitutions, xkb_atom_t name)
{
    /*द्विआधारी खोज*/
    darray_आकार_t first = 0;
    darray_आकार_t last = darray_आकार(*substitutions);

    जबतक (last > first) {
        darray_आकार_t mid = first + (last - 1 - first) / 2;
        स्थिर xkb_atom_t source = darray_item(*substitutions, mid).source;
        अगर (source < name)
            first = mid + 1;
        अन्यथा अगर (source > name)
            last = mid;
        अन्यथा /* found it */
            वापस darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    वापस name;
}

अटल bool
write_vmods(स्थिर काष्ठा xkb_keymap *keymap, क्रमागत xkb_keymap_format format,
            bool explicit, काष्ठा buf *buf)
{
    स्थिर काष्ठा xkb_mod *mod;
    xkb_mod_सूचकांक_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        अगर (!has_some) {
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
        स्थिर bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        अगर ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
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

    अगर (has_some)
        copy_to_buf(buf, ";\n\n");

    वापस true;
}

अटल bool
write_keycodes(स्थिर काष्ठा xkb_keymap *keymap,
               स्थिर कुंजी_name_substitutions *substitutions,
               क्रमागत xkb_keymap_serialize_flags flags, काष्ठा buf *buf)
{
    स्थिर काष्ठा xkb_कुंजी *कुंजी;
    xkb_led_सूचकांक_t idx;
    स्थिर काष्ठा xkb_led *led;

    स्थिर bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    अगर (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_कुंजी_code, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_कुंजी_code, 255));

    xkb_कुंजियाँ_foreach(कुंजी, keymap) {
        अगर (कुंजी->name == XKB_ATOM_NONE)
            जारी;

        स्थिर xkb_atom_t name = (substitutions == शून्य)
            ? कुंजी->name
            : substitute_name(substitutions, कुंजी->name);
        अगर (pretty)
            write_buf(buf, "\t%-20s", कुंजी_Name_Text(keymap->ctx, name));
        अन्यथा
            write_buf(buf, "\t%s", कुंजी_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", कुंजी->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        अगर (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_to_buf(buf, ";\n");
        }

    क्रम (darray_आकार_t i = 0; i < keymap->अंक_कुंजी_aliases; i++) {
        स्थिर xkb_atom_t alias = (substitutions == शून्य)
            ? keymap->कुंजी_aliases[i].alias
            : substitute_name(substitutions, keymap->कुंजी_aliases[i].alias);
        स्थिर xkb_atom_t real = (substitutions == शून्य)
            ? keymap->कुंजी_aliases[i].real
            : substitute_name(substitutions, keymap->कुंजी_aliases[i].real);

        अगर (pretty)
            write_buf(buf, "\talias %-14s",
                      कुंजी_Name_Text(keymap->ctx, alias));
        अन्यथा
            write_buf(buf, "\talias %s",
                      कुंजी_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  कुंजी_Name_Text(keymap->ctx, real));
    }

    copy_to_buf(buf, "};\n\n");
    वापस true;
}

अटल bool
write_types(स्थिर काष्ठा xkb_keymap *keymap, क्रमागत xkb_keymap_format format,
            क्रमागत xkb_keymap_serialize_flags flags, काष्ठा buf *buf)
{
    अगर (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    अगर (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        वापस false;

    स्थिर bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    क्रम (darray_आकार_t i = 0; i < keymap->अंक_types; i++) {
        स्थिर काष्ठा xkb_कुंजी_type * स्थिर type = &keymap->types[i];
        अगर (!type->required && drop_unused)
            जारी;

        copy_to_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_to_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        क्रम (darray_आकार_t j = 0; j < type->अंक_entries; j++) {
            स्थिर अक्षर *str;
            स्थिर काष्ठा xkb_कुंजी_type_entry *entry = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            अगर (entry->level == 0 && entry->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मूल्यावली))
                जारी;

            str = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              entry->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, entry->level + 1);

            अगर (entry->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           entry->preserve.mods));
        }

        क्रम (xkb_level_सूचकांक_t n = 0; n < type->अंक_level_names; n++)
            अगर (type->level_names[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_names[n]));
                copy_to_buf(buf, ";\n");
            }

        copy_to_buf(buf, "\t};\n");
    }

    copy_to_buf(buf, "};\n\n");
    वापस true;
}

अटल bool
write_led_map(स्थिर काष्ठा xkb_keymap *keymap, क्रमागत xkb_keymap_format format,
              bool defaults, काष्ठा buf *buf, स्थिर काष्ठा xkb_led *led)
{
    copy_to_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_to_buf(buf, " {\n");

    अगर (led->which_groups) {
        अगर (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      LedStateMaskText(keymap->ctx, groupComponentMaskNames,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    अगर (led->which_mods) {
        अगर (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      LedStateMaskText(keymap->ctx, modComponentMaskNames,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    अगर (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  ControlMaskText(keymap->ctx, format, led->ctrls));
    }

    copy_to_buf(buf, "\t};\n");
    वापस true;
}

अटल स्थिर अक्षर *
affect_lock_text(क्रमागत xkb_action_flags flags, bool show_both)
{
    चयन (flags & (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK)) {
    हाल 0:
        वापस show_both ? ",affect=both" : "";
    हाल ACTION_LOCK_NO_UNLOCK:
        वापस ",affect=lock";
    हाल ACTION_LOCK_NO_LOCK:
        वापस ",affect=unlock";
    हाल ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK:
        वापस ",affect=neither";
    शेष:
        वापस "";
    }
}

#घोषणा प्रतीक_PADDING 15
#घोषणा ACTION_PADDING 30

अटल bool
write_action(स्थिर काष्ठा xkb_keymap *keymap, क्रमागत xkb_keymap_format format,
             xkb_layout_सूचकांक_t max_groups, काष्ठा buf *buf,
             स्थिर जोड़ xkb_action *action,
             स्थिर अक्षर *prefix, स्थिर अक्षर *suffix)
{
    स्थिर अक्षर *type;
    स्थिर अक्षर *args = शून्य;

    अगर (!prefix)
        prefix = "";
    अगर (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    चयन (action->type) {
    हाल ACTION_TYPE_MOD_SET:
    हाल ACTION_TYPE_MOD_LATCH:
    हाल ACTION_TYPE_MOD_LOCK:
        अगर (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            args = "modMapMods";
        अन्यथा
            args = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        अगर (unlockOnPress && !isModsUnLockOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->group.flags & ACTION_LATCH_ON_PRESS);
        अगर (latchOnPress && !isModsLatchOnPressSupported(format)) {
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
        अवरोध;

    हाल ACTION_TYPE_GROUP_SET:
    हाल ACTION_TYPE_GROUP_LATCH:
    हाल ACTION_TYPE_GROUP_LOCK:
        अगर ((uint32_t) xkb_abs(action->group.group) <
            (max_groups + !(action->group.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool lockOnRelease = action->type == ACTION_TYPE_GROUP_LOCK &&
                                 (action->group.flags & ACTION_LOCK_ON_RELEASE);
            अगर (lockOnRelease && !isGroupLockOnReleaseSupported(format)) {
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
            जाओ void_action;
        }
        अवरोध;

    हाल ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", prefix, type, suffix);
        अवरोध;

    हाल ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", prefix, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        अवरोध;

    हाल ACTION_TYPE_PTR_LOCK:
        args = affect_lock_text(action->btn.flags, true);
        /* fallthrough */
    हाल ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", prefix, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        अगर (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        अन्यथा
            copy_to_buf(buf, "default");
        अगर (action->btn.गिनती) {
            निश्चित(action->type != ACTION_TYPE_PTR_LOCK);
            write_buf(buf, ",count=%u", action->btn.गिनती);
        }
        अगर (args)
            write_buf(buf, "%s", args);
        write_buf(buf, ")%s", suffix);
        अवरोध;

    हाल ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", prefix, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.मूल्य >= 0) ? "+" : "",
                  action->dflt.मूल्य);
        write_buf(buf, ")%s", suffix);
        अवरोध;

    हाल ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", prefix, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        अवरोध;

    हाल ACTION_TYPE_CTRL_SET:
    हाल ACTION_TYPE_CTRL_LOCK:
        write_buf(buf, "%s%s(controls=%s%s)%s", prefix, type,
                  ControlMaskText(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_LOCK
                    ? affect_lock_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        अवरोध;

    हाल ACTION_TYPE_REDIRECT_कुंजी: {
        write_buf(buf, "%s%s(", prefix, type);
        स्थिर काष्ठा xkb_कुंजी * स्थिर कुंजी = Xkb_कुंजी(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        अगर (कुंजी)
            write_buf(buf, "keycode=%s", कुंजी_Name_Text(keymap->ctx, कुंजी->name));
        अगर (action->redirect.affect_mods) {
            xkb_mod_mask_t mask;
            mask = (action->redirect.affect_mods & action->redirect.mods);
            अगर (mask)
                write_buf(buf, ",modifiers=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
            mask = (action->redirect.affect_mods & ~action->redirect.mods);
            अगर (mask)
                write_buf(buf, ",clearMods=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
        }
        write_buf(buf, ")%s", suffix);
        अवरोध;
    }

    हाल ACTION_TYPE_UNKNOWN:
    हाल ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", prefix, suffix);
        अवरोध;

    हाल ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        अगर (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      prefix, suffix);
        अन्यथा
            write_buf(buf, "%sVoidAction()%s", prefix, suffix);
        अवरोध;

    शेष:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        निश्चित(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  prefix, type, action->type, action->priv.तथ्य[0],
                  action->priv.तथ्य[1], action->priv.तथ्य[2],
                  action->priv.तथ्य[3], action->priv.तथ्य[4],
                  action->priv.तथ्य[5], action->priv.तथ्य[6],
                  suffix);
        अवरोध;
    }

    वापस true;
}

अटल स्थिर जोड़ xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

अटल bool
write_actions(स्थिर काष्ठा xkb_keymap *keymap, क्रमागत xkb_keymap_format format,
              xkb_layout_सूचकांक_t max_groups, काष्ठा buf *buf, काष्ठा buf *buf2,
              स्थिर काष्ठा xkb_कुंजी *कुंजी, xkb_layout_सूचकांक_t group)
{
    अटल स्थिर जोड़ xkb_action noAction = { .type = ACTION_TYPE_NONE };

    क्रम (xkb_level_सूचकांक_t level = 0; level < Xkb_कुंजी_अंक_Levels(कुंजी, group);
         level++) {

        अगर (level != 0)
            copy_to_buf(buf, ", ");

        स्थिर जोड़ xkb_action *actions = शून्य;
        xkb_action_गिनती_t गिनती = xkb_keymap_कुंजी_get_actions_by_level(
            keymap, कुंजी, group, level, &actions
        );

        अगर (गिनती > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            गिनती = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->आकार = 0;
        अगर (गिनती == 0) {
            अगर (!write_action(keymap, format, max_groups,
                              buf2, &noAction, शून्य, शून्य))
                वापस false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        अन्यथा अगर (गिनती == 1) {
            अगर (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), शून्य, शून्य))
                वापस false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        अन्यथा {
            copy_to_buf(buf2, "{ ");
            क्रम (xkb_action_गिनती_t k = 0; k < गिनती; k++) {
                अगर (k != 0)
                    copy_to_buf(buf2, ", ");
                माप_प्रकार old_आकार = buf2->आकार;
                अगर (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), शून्य, शून्य))
                    वापस false;
                /* Check if padding is necessary */
                अगर (buf2->आकार >= old_आकार + ACTION_PADDING)
                    जारी;
                /* Compute and write padding, then write the action again */
                स्थिर पूर्णांक padding = (पूर्णांक)(old_आकार + ACTION_PADDING - buf2->आकार);
                buf2->आकार = old_आकार;
                write_buf(buf2, "%*s", padding, "");
                अगर (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), शून्य, शून्य))
                    वापस false;
            }
            copy_to_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    वापस true;
}

अटल bool
write_action_defaults(स्थिर काष्ठा xkb_keymap *keymap,
                      क्रमागत xkb_keymap_format format,
                      स्थिर जोड़ xkb_action *action,
                      काष्ठा buf *buf)
{
    स्थिर अक्षर *type = ActionTypeText(action->type);

#घोषणा PREFIX "\t"

    चयन (action->type) {
    हाल ACTION_TYPE_MOD_SET:
    हाल ACTION_TYPE_MOD_LATCH:
    हाल ACTION_TYPE_MOD_LOCK:
        निश्चित(action->mods.flags == 0);
        निश्चित(action->mods.mods.mods == 0);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        अगर (action->type != ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        अगर (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, PREFIX"%s.latchOnPress = false;\n", type);
        अगर (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        अगर (action->type == ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        अगर (isModsUnLockOnPressSupported(format))
            write_buf(buf, PREFIX"%s.unlockOnPress = false;\n", type);
        अवरोध;

    हाल ACTION_TYPE_GROUP_SET:
    हाल ACTION_TYPE_GROUP_LATCH:
    हाल ACTION_TYPE_GROUP_LOCK:
        निश्चित(action->group.flags == 0);
        निश्चित(action->group.group == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.group = +0;\n", type);
        अगर (action->type != ACTION_TYPE_GROUP_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        अगर (action->type == ACTION_TYPE_GROUP_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        अगर (action->type == ACTION_TYPE_GROUP_LOCK &&
            isGroupLockOnReleaseSupported(format))
            write_buf(buf, PREFIX"%s.lockOnRelease = false;\n", type);
        अवरोध;


    हाल ACTION_TYPE_PTR_MOVE:
        निश्चित(action->ptr.x == 0);
        निश्चित(action->ptr.y == 0);
        निश्चित(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.x = +0;\n", type);
        write_buf(buf, PREFIX"%s.y = +0;\n", type);
        write_buf(buf, PREFIX"%s.accel = true;\n", type);
        अवरोध;

    हाल ACTION_TYPE_PTR_LOCK:
    हाल ACTION_TYPE_PTR_BUTTON:
        निश्चित(action->btn.flags == 0);
        निश्चित(action->btn.button == 0);
        निश्चित(action->btn.गिनती == 0);
        write_buf(buf, PREFIX"%s.button = 0;\n", type);
        write_buf(buf, PREFIX"%s.count = 0;\n", type);
        अगर (action->type == ACTION_TYPE_PTR_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        अवरोध;

    हाल ACTION_TYPE_PTR_DEFAULT:
        निश्चित(action->dflt.flags == 0);
        निश्चित(action->dflt.मूल्य == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.button = +1;\n", type);
        अवरोध;

    हाल ACTION_TYPE_SWITCH_VT:
        निश्चित(action->screen.flags == ACTION_SAME_SCREEN);
        निश्चित(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.screen = +0;\n", type);
        write_buf(buf, PREFIX"%s.same = true;\n", type);
        अवरोध;

    हाल ACTION_TYPE_CTRL_SET:
    हाल ACTION_TYPE_CTRL_LOCK:
        निश्चित(action->ctrls.flags == 0);
        निश्चित(action->ctrls.ctrls == 0);
        write_buf(buf, PREFIX"%s.controls = 0;\n", type);
        अगर (action->type == ACTION_TYPE_CTRL_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        अवरोध;

    हाल ACTION_TYPE_REDIRECT_कुंजी:
        निश्चित(action->redirect.keycode == keymap->redirect_कुंजी_auto);
        निश्चित(action->redirect.affect_mods == 0);
        निश्चित(action->redirect.mods == 0);
        write_buf(buf, PREFIX"%s.keycode = auto;\n", type);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        write_buf(buf, PREFIX"%s.clearMods = 0;\n", type);
        अवरोध;

    शेष:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_अंक_ENTRIES,
                      "Missing action type");
    }

#अघोषित PREFIX

    वापस true;
}

अटल bool
write_actions_defaults(स्थिर काष्ठा xkb_keymap *keymap,
                       क्रमागत xkb_keymap_format format, काष्ठा buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    क्रम (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_अंक_ENTRIES; a++) {
        स्थिर जोड़ xkb_action * restrict स्थिर action = &defaults.actions[a];
        अगर (!write_action_defaults(keymap, format, action, buf))
            वापस false;
    }
    copy_to_buf(buf, "\n");

    वापस true;
}

अटल स्थिर काष्ठा xkb_sym_interpret fallback_interpret = {
    .sym = XKB_कुंजी_Void_प्रतीक,
    .required = true,
    .repeat = FALLBACK_INTERPRET_कुंजी_REPEAT,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_सूचकांक_t) DEFAULT_INTERPRET_VMOD,
    .अंक_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

अटल bool
write_compat(स्थिर काष्ठा xkb_keymap * restrict keymap,
             क्रमागत xkb_keymap_format format, xkb_layout_सूचकांक_t max_groups,
             क्रमागत xkb_keymap_serialize_flags flags, bool * restrict some_interp,
             काष्ठा buf *buf)
{
    स्थिर bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    स्थिर bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मूल्यावली);
    स्थिर bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    स्थिर bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_कुंजी_मूल्यावली) && drop_unused);
    /* TODO: print all default values if the explicit flag is set */

    अगर (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_compatibility {\n");

    अगर (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        वापस false;

    अगर (defaults && !write_actions_defaults(keymap, format, buf))
        वापस false;

    copy_to_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_to_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    स्थिर bool use_fallback_interpret =
        (!keymap->अंक_sym_interprets || drop_interprets);
    स्थिर darray_आकार_t अंक_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->अंक_sym_interprets);
    स्थिर काष्ठा xkb_sym_interpret* स्थिर sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    क्रम (darray_आकार_t i = 0; i < अंक_sym_interprets; i++) {
        स्थिर काष्ठा xkb_sym_interpret *si = &sym_interprets[i];
        अगर (!si->required && drop_unused)
            जारी;

        *some_interp = true;

        copy_to_buf(buf, "\tinterpret ");
        अगर (!si->sym) {
            copy_to_buf(buf, "Any");
        } अन्यथा अगर (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } अन्यथा {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->match),
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        अगर (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      Mod_सूचकांक_Text(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        अगर (si->level_one_only) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } अन्यथा अगर (defaults) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        अगर (si->repeat) {
            copy_to_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } अन्यथा अगर (defaults) {
            copy_to_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        स्थिर जोड़ xkb_action *action = शून्य;
        xkb_action_गिनती_t action_गिनती = si->अंक_actions;
        अगर (action_गिनती <= 1) {
            action = &si->a.action;
        } अन्यथा अगर (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_गिनती = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        अगर (action_गिनती > 1) {
            copy_to_buf(buf, "\n\t\taction= {");
            स्थिर अक्षर suffix[] = ", ";
            क्रम (xkb_action_गिनती_t k = 0; k < si->अंक_actions; k++) {
                अगर (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    वापस false;
            }
            buf->आकार -= माप(suffix) - 1; /* trailing comma */
            copy_to_buf(buf, "};");
            has_explicit_properties = true;
        } अन्यथा अगर (action_गिनती == 1) {
            अगर (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                वापस false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    अगर (use_fallback_interpret || (
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

    स्थिर काष्ठा xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        अगर ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            वापस false;
        }
    }

    copy_to_buf(buf, "};\n\n");

    वापस true;
}

अटल bool
write_keysyms(स्थिर काष्ठा xkb_keymap *keymap,
              काष्ठा buf *buf, काष्ठा buf *buf2,
              स्थिर काष्ठा xkb_कुंजी *कुंजी, xkb_layout_सूचकांक_t group,
              bool pretty, bool show_actions)
{
    स्थिर अचिन्हित पूर्णांक padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : प्रतीक_PADDING)
        : 0;
    क्रम (xkb_level_सूचकांक_t level = 0; level < Xkb_कुंजी_अंक_Levels(कुंजी, group);
         level++) {
        स्थिर xkb_keysym_t *syms;
        पूर्णांक अंक_syms;

        अगर (level != 0)
            copy_to_buf(buf, ", ");

        अंक_syms = xkb_keymap_कुंजी_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (काष्ठा xkb_keymap *)keymap,
            कुंजी->keycode, group, level, &syms
        );

        स्थिर xkb_keysym_t no_प्रतीक = XKB_कुंजी_No_प्रतीक;
        अगर (अंक_syms == 0) {
            अंक_syms = 1;
            syms = &no_प्रतीक;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        अगर (अंक_syms == 1) {
            अगर (pretty || syms[0] == XKB_कुंजी_No_प्रतीक)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            अन्यथा
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } अन्यथा {
            अगर (pretty) {
                buf2->आकार = 0;
                copy_to_buf(buf2, "{ ");
                क्रम (पूर्णांक s = 0; s < अंक_syms; s++) {
                    अगर (s != 0)
                        copy_to_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_to_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } अन्यथा {
                copy_to_buf(buf, "{");
                क्रम (पूर्णांक s = 0; s < अंक_syms; s++) {
                    अगर (s != 0)
                        copy_to_buf(buf, ",");
                    अगर (syms[s] == XKB_कुंजी_No_प्रतीक)
                        copy_to_buf(buf, "NoSymbol");
                    अन्यथा
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_to_buf(buf, "}");
            }
        }
    }

    वापस true;
}

अटल bool
write_कुंजी(स्थिर काष्ठा xkb_keymap *keymap,
          स्थिर काष्ठा xkb_keymap_serialize_config *config,
          स्थिर कुंजी_name_substitutions *substitutions,
          xkb_layout_सूचकांक_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          काष्ठा buf *buf, काष्ठा buf *buf2, स्थिर काष्ठा xkb_कुंजी *कुंजी)
{
    bool simple = true;

    xkb_layout_mask_t layouts = config->layouts;
    अगर (popcount32(layouts) != keymap->अंक_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_सूचकांक_t l = 0;
        क्रम (xkb_layout_mask_t ls = layouts; ls; ls >>= 1, l++) {
            अगर (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                स्थिर xkb_layout_सूचकांक_t effective =
                    xkb_keymap_कुंजी_effective_layout(कुंजी, l);
                अगर (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } अन्यथा {
        layouts &= ((UINT64_C(1) << कुंजी->अंक_groups) - 1);
    }
    स्थिर xkb_layout_सूचकांक_t groups_गिनती = popcount32(layouts);

    स्थिर xkb_atom_t name = (substitutions == शून्य)
        ? कुंजी->name
        : substitute_name(substitutions, कुंजी->name);

    अगर (pretty)
        write_buf(buf, "\tkey %-20s {", कुंजी_Name_Text(keymap->ctx, name));
    अन्यथा
        write_buf(buf, "\tkey %s {", कुंजी_Name_Text(keymap->ctx, name));

    अगर ((कुंजी->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        अगर (!explicit) {
            क्रम (xkb_layout_सूचकांक_t group = 1; group < कुंजी->अंक_groups; group++) {
                अगर (!((UINT32_C(1) << group) & layouts))
                    जारी;

                अगर (कुंजी->groups[group].type != कुंजी->groups[0].type) {
                    multi_type = true;
                    अवरोध;
                }
            }
        }

        अगर (multi_type) {
            xkb_layout_सूचकांक_t new_group = (xkb_layout_सूचकांक_t)-1;
            क्रम (xkb_layout_सूचकांक_t group = 0; group < कुंजी->अंक_groups; group++) {
                अगर (!((UINT32_C(1) << group) & layouts))
                    जारी;

                new_group++;

                अगर (!कुंजी->groups[group].explicit_type && !explicit)
                    जारी;

                स्थिर काष्ठा xkb_कुंजी_type * स्थिर type = कुंजी->groups[group].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_group + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_to_buf(buf, ",");
            }
        }
        अन्यथा {
            स्थिर काष्ठा xkb_कुंजी_type * स्थिर type = कुंजी->groups[0].type;
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
    स्थिर bool explicit_actions = (कुंजी->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #घोषणा require_explicit(कुंजी, prop,                                       \
                             मूल्य, initial_मूल्य, default_interpret_मूल्य) ( \
        /* Explicit value in the source */                                    \
        ((कुंजी)->explicit & (prop)) ||                                         \
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
                ((कुंजी)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (मूल्य) != (initial_मूल्य) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_मूल्य) != (default_interpret_मूल्य)                  \
            )                                                                 \
        )                                                                     \
    )

    अगर (require_explicit(कुंजी, EXPLICIT_REPEAT, कुंजी->repeats,
                         DEFAULT_कुंजी_REPEAT, DEFAULT_INTERPRET_कुंजी_REPEAT)) {
        अगर (कुंजी->repeats)
            copy_to_buf(buf, "\n\t\trepeat= Yes,");
        अन्यथा
            copy_to_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    अगर (require_explicit(कुंजी, EXPLICIT_VMODMAP, कुंजी->vmodmap,
                         (xkb_mod_mask_t) DEFAULT_कुंजी_VMODMAP,
                         (xkb_mod_mask_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              कुंजी->vmodmap));
        simple = false;
    }

    #अघोषित require_explicit

    चयन (कुंजी->out_of_range_group_policy) {
    हाल XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        copy_to_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        अवरोध;

    हाल XKB_LAYOUT_OUT_OF_RANGE_REDIRECT: {
        अगर (!groups_गिनती) {
            /* No layout to redirect to: skip */
            अवरोध;
        }

        xkb_layout_mask_t layout_mask =
            (UINT32_C(1) << कुंजी->out_of_range_group_संख्या);
        अगर (!(layouts & layout_mask)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     कुंजी_Name_Text(keymap->ctx, name),
                     कुंजी->out_of_range_group_संख्या + 1);
            layout_mask = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        स्थिर xkb_layout_mask_t निचला = (layouts & (layout_mask - 1));
        स्थिर xkb_layout_सूचकांक_t new = popcount32(निचला);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        अवरोध;
    }

    हाल XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    शेष:
        अवरोध;
    }

    अगर (कुंजी->overlays) {
        xkb_overlay_mask_t remaining = कुंजी->overlays;

        स्थिर xkb_overlay_सूचकांक_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_गिनती == 8, "invalid right shift");
        स्थिर xkb_overlay_mask_t valid =
            (xkb_overlay_mask_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        अगर (remaining != कुंजी->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_सूचकांक,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, कुंजी_Name_Text(keymap->ctx, name),
                     remaining, कुंजी->overlays & ~valid);
        }

        अगर (remaining && !कुंजी->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            स्थिर xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
            );
            /* get its index */
            स्थिर xkb_overlay_सूचकांक_t overlay =
                (xkb_overlay_सूचकांक_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     कुंजी_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        अगर (remaining)
            simple = false;

        स्थिर काष्ठा xkb_कुंजी * स्थिर *overlays_कुंजियाँ = (कुंजी->overlays_inline)
                                                    ? &कुंजी->overlay_कुंजी
                                                    : कुंजी->overlays_कुंजियाँ;
        जबतक (remaining) {
            /* isolate lowest set bit */
            स्थिर xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((अचिन्हित)~remaining + 1u)
            );
            /* get its index */
            स्थिर xkb_overlay_सूचकांक_t overlay =
                (xkb_overlay_सूचकांक_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            अगर (कुंजी->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            स्थिर काष्ठा xkb_कुंजी *overlay_कुंजी = *(overlays_कुंजियाँ++);

            स्थिर xkb_atom_t overlay_कुंजी_name = (substitutions == शून्य)
                ? overlay_कुंजी->name
                : substitute_name(substitutions, overlay_कुंजी->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, कुंजी_Name_Text(keymap->ctx, overlay_कुंजी_name));
        }
    }

    अगर (groups_गिनती > 1 || explicit_actions || explicit)
        simple = false;

    अगर (simple) {
        अगर (groups_गिनती == 0) {
            /* Remove trailing comma */
            delete_last_char(buf, ',');
            अगर (buf->buf[buf->आकार - 1] == ',')
                buf->आकार--;
        } अन्यथा {
            copy_to_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_सूचकांक_t group = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            group = xkb_keymap_कुंजी_effective_layout(कुंजी, group);
            अगर (!write_keysyms(keymap, buf, buf2, कुंजी, group, pretty, false))
                वापस false;
            copy_to_buf(buf, " ]");
        }
        copy_to_buf(buf, " };\n");
    }
    अन्यथा {
        xkb_layout_सूचकांक_t new_group = (xkb_layout_सूचकांक_t)-1;
        क्रम (xkb_layout_सूचकांक_t group = 0; group < कुंजी->अंक_groups; group++) {
            अगर (!((UINT32_C(1) << group) & layouts))
                जारी;

            new_group++;

            स्थिर bool print_actions =
                /* Group has explicit actions */
                कुंजी->groups[group].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (कुंजी->groups[group].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (कुंजी->groups[group].implicit_actions && !some_interprets);

            अगर (new_group != 0)
                copy_to_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_group + 1);

            अगर (!write_keysyms(keymap, buf, buf2, कुंजी, group,
                               pretty, print_actions))
                वापस false;
            copy_to_buf(buf, " ]");
            अगर (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_group + 1);
                अगर (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, कुंजी, group))
                    वापस false;
                copy_to_buf(buf, " ]");
            }
        }
        अगर (!groups_गिनती)
            delete_last_char(buf, ',');
        copy_to_buf(buf, "\n\t};\n");
    }

    वापस true;
}


अटल पूर्णांक
cmp_modmap_step_1(स्थिर व्योम *a, स्थिर व्योम *b)
{
    स्थिर ModMapEntry *m1 = (स्थिर ModMapEntry *)a;
    स्थिर ModMapEntry *m2 = (स्थिर ModMapEntry *)b;

    अगर (!m1->mods) {
        अगर (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            वापस 0;
        }
        वापस +1;
    } अन्यथा अगर (!m2->mods) {
        वापस -1;
    }

    /* Valid only on step 1 */
    अगर (m1->कुंजी_Code < m2->कुंजी_Code)
        वापस -1;
    अन्यथा अगर (m1->कुंजी_Code > m2->कुंजी_Code)
        वापस +1;

    अगर (m1->mods < m2->mods)
        वापस -1;
    अन्यथा अगर (m1->mods > m2->mods)
        वापस +1;

    अगर (m1->have_प्रतीक < m2->have_प्रतीक)
        वापस +1;
    अन्यथा अगर (m1->have_प्रतीक > m2->have_प्रतीक)
        वापस -1;

    अगर (m1->have_प्रतीक) {
        अगर (m1->u.कुंजी_Sym < m2->u.कुंजी_Sym)
            वापस -1;
        अगर (m1->u.कुंजी_Sym > m2->u.कुंजी_Sym)
            वापस +1;
    } अन्यथा {
        अगर (m1->u.कुंजी_Name < m2->u.कुंजी_Name)
            वापस -1;
        अगर (m1->u.कुंजी_Name > m2->u.कुंजी_Name)
            वापस +1;
    }

    वापस 0;
}

अटल पूर्णांक
cmp_modmap_step_2(स्थिर व्योम *a, स्थिर व्योम *b)
{
    स्थिर ModMapEntry *m1 = (स्थिर ModMapEntry *)a;
    स्थिर ModMapEntry *m2 = (स्थिर ModMapEntry *)b;

    अगर (m1->mods < m2->mods)
        वापस -1;
    अगर (m1->mods > m2->mods)
        वापस +1;

    अगर (m1->have_प्रतीक < m2->have_प्रतीक)
        वापस -1;
    अगर (m1->have_प्रतीक > m2->have_प्रतीक)
        वापस +1;

    अगर (m1->have_प्रतीक) {
        अगर (m1->u.कुंजी_Sym < m2->u.कुंजी_Sym)
            वापस -1;
        अगर (m1->u.कुंजी_Sym > m2->u.कुंजी_Sym)
            वापस +1;
    } अन्यथा {
        अगर (m1->कुंजी_Code < m2->कुंजी_Code)
            वापस -1;
        अगर (m1->कुंजी_Code > m2->कुंजी_Code)
            वापस +1;
    }

    वापस 0;
}

अटल bool
write_modmaps(स्थिर काष्ठा xkb_keymap *keymap, क्रमागत xkb_keymap_format format,
              स्थिर कुंजी_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, काष्ठा buf *buf)
{
    अगर (!keymap->अंक_modmaps)
        वापस true;

    darray(ModMapEntry) modmaps = darray_new();
    darray_from_वस्तुएँ(modmaps, keymap->modmaps, keymap->अंक_modmaps);
    अगर (!darray_वस्तुएँ(modmaps))
        वापस false;

    /* Sort entries by key code */
    क्विक(darray_वस्तुएँ(modmaps), darray_आकार(modmaps),
          माप(*darray_वस्तुएँ(modmaps)), &cmp_modmap_step_1);

    /* Drop disabled entries at the end */
    क्रम (darray_आकार_t m = darray_आकार(modmaps); m-- > 0;) {
        अगर (darray_item(modmaps, m).mods)
            अवरोध;
        darray_आकार(modmaps)--;
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
    क्रम (darray_आकार_t m = darray_आकार(modmaps); m-- > 0;) {
        ModMapEntry *mm = &darray_item(modmaps, m);
        निश्चित(mm->mods);

        अगर (mm->कुंजी_Code == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            स्थिर xkb_mod_mask_t core_mods = (mm->mods & MOD_REAL_MASK_ALL);
            अगर (convert_to_v1_compatibility_entry && core_mods) {
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

        current_keycode = mm->कुंजी_Code;
        स्थिर काष्ठा xkb_कुंजी *कुंजी = Xkb_कुंजी(keymap, mm->कुंजी_Code);

        स्थिर xkb_mod_mask_t core_mods = (कुंजी->modmap & MOD_REAL_MASK_ALL);
        अगर (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((कुंजी->modmap & (कुंजी->modmap - 1)) || कुंजी->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_to_v1_compatibility_entry = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    कुंजी_Name_Text(keymap->ctx, कुंजी->name), कुंजी->modmap);
        } अन्यथा {
            /* V2 format or V1 format with a single core modifier */
            निश्चित(कुंजी->modmap);
            mm->mods = कुंजी->modmap;
            /* Force key name */
            mm->have_प्रतीक = false;
            mm->u.कुंजी_Name = कुंजी->name;
            convert_to_v1_compatibility_entry = false;
        }
    }

    अगर (darray_empty(modmaps))
        जाओ निकास;

    /* Sort entries by modifier mask */
    क्विक(darray_वस्तुएँ(modmaps), darray_आकार(modmaps),
          माप(*darray_वस्तुएँ(modmaps)), &cmp_modmap_step_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_mask_t current_mods = 0;
    स्थिर ModMapEntry *entry;
    darray_foreach(entry, modmaps) {
        अगर (!entry->mods)
            जारी;

        अगर (current_mods != entry->mods) {
            अगर (current_mods) {
                copy_to_buf(buf, " };\n");
            }
            current_mods = entry->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } अन्यथा {
            copy_to_buf(buf, ", ");
        }

        अगर (entry->have_प्रतीक) {
            अगर (pretty || entry->u.कुंजी_Sym == XKB_कुंजी_No_प्रतीक)
                write_buf(buf, "%s", KeysymText(keymap->ctx, entry->u.कुंजी_Sym));
            अन्यथा
                write_buf(buf, "0x%"PRIx32, entry->u.कुंजी_Sym);
        } अन्यथा {
            स्थिर xkb_atom_t name = (substitutions == शून्य)
                ? entry->u.कुंजी_Name
                : substitute_name(substitutions, entry->u.कुंजी_Name);
            write_buf(buf, "%s", कुंजी_Name_Text(keymap->ctx, name));
        }
    }
    अगर (current_mods) {
        copy_to_buf(buf, " };\n");
    }

निकास:
    darray_free(modmaps);
    वापस true;
}

अटल bool
write_symbols(स्थिर काष्ठा xkb_keymap *keymap,
              स्थिर काष्ठा xkb_keymap_serialize_config *config,
              स्थिर कुंजी_name_substitutions *substitutions,
              xkb_layout_सूचकांक_t max_groups, bool some_interp,
              काष्ठा buf *buf)
{
    स्थिर bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    स्थिर bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मूल्यावली);
    स्थिर bool explicit_कुंजी_मूल्यावली =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_कुंजी_मूल्यावली);
    स्थिर bool drop_unused = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    स्थिर bool drop_interprets = (explicit_कुंजी_मूल्यावली && drop_unused);
    /* TODO: print all default values if explicit flag is set */

    अगर (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    अन्यथा
        copy_to_buf(buf, "xkb_symbols {\n");

    bool has_group_names = false;
    xkb_layout_सूचकांक_t new_group = (xkb_layout_सूचकांक_t)-1;
    क्रम (xkb_layout_सूचकांक_t group = 0; group < keymap->अंक_group_names; group++) {
        अगर (!((UINT32_C(1) << group) & config->layouts))
            जारी;

        new_group++;

        अगर (keymap->group_names[group]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_group + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->group_names[group]));
            copy_to_buf(buf, ";\n");
            has_group_names = true;
        }
    }
    अगर (has_group_names)
        copy_to_buf(buf, "\n");

    अगर (defaults && !write_actions_defaults(keymap, config->format, buf))
        वापस false;

    काष्ठा buf buf2 = { शून्य, 0, 0 };
    स्थिर काष्ठा xkb_कुंजी *कुंजी;
    xkb_कुंजियाँ_foreach(कुंजी, keymap) {
        /* Skip keys with no explicit values */
        अगर (कुंजी->explicit &&
            !write_कुंजी(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_कुंजी_मूल्यावली, pretty, buf,
                       &buf2, कुंजी))
        {
            मुक्त(buf2.buf);
            वापस false;
        }
    }
    मुक्त(buf2.buf);

    अगर (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        वापस false;

    copy_to_buf(buf, "};\n\n");
    वापस true;
}

अटल क्रमागत xkb_status
write_keymap(स्थिर काष्ठा xkb_keymap *keymap,
             स्थिर काष्ठा xkb_keymap_serialize_config *config,
             काष्ठा buf *buf, काष्ठा xkb_keymap_serialize_परिणाम *परिणाम)
{
    कुंजी_name_substitutions substitutions = darray_new();
    अगर (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_कुंजियाँ(keymap, &substitutions))
    {
        वापस false;
    }

    स्थिर कुंजी_name_substitutions * स्थिर substitutions_ptr =
        (darray_empty(substitutions)) ? शून्य : &substitutions;

    स्थिर xkb_layout_सूचकांक_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    स्थिर bool ok = (
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
    वापस ok ? XKB_SUCCESS : -1;
}

क्रमागत xkb_status
text_v1_keymap_serialize(
        स्थिर काष्ठा xkb_keymap *keymap,
        स्थिर काष्ठा xkb_keymap_serialize_config *config,
        काष्ठा xkb_keymap_serialize_परिणाम *परिणाम
)
{
    काष्ठा buf buf = { शून्य, 0, 0 };

    स्थिर क्रमागत xkb_status status =
        write_keymap(keymap, config, &buf, परिणाम);
    अगर (status != XKB_SUCCESS) {
        मुक्त(buf.buf);
        परिणाम->serialized = शून्य;
        वापस status;
    }

    परिणाम->serialized = buf.buf;
    परिणाम->लंबाई = buf.आकार + 1;
    परिणाम->layouts = config->layouts;

    वापस XKB_SUCCESS;
}
