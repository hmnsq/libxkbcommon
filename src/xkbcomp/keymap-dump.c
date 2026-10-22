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

#परिभाषय BUF_CHUNK_आकार 4096

संरचना buf {
    वर्ण *buf;
    आकार_t आकार;
    आकार_t alloc;
};

#परिभाषय xkb_abs(n) _Generic((n),                \
                            पूर्णाङ्क: निरपेक्षम्,           \
                            दीर्घ पूर्णाङ्क: labs,     \
                            स्वभाव: llabs )((n))

स्थिर bool
do_realloc(संरचना buf *buf, आकार_t at_न्यूनतमम्)
{
    buf->alloc += BUF_CHUNK_आकार;
    यदि (at_न्यूनतमम् >= BUF_CHUNK_आकार)
        buf->alloc += at_न्यूनतमम्;

    वर्ण *ध्रुव new = realloc(buf->buf, buf->alloc);
    यदि (!new)
        प्रत्यागम false;

    buf->buf = new;
    प्रत्यागम true;
}

ATTR_PRINTF(2, 3) स्थिर bool
check_write_buf(संरचना buf *buf, ध्रुव वर्ण *fmt, ...)
{
    va_list तर्काः;
    पूर्णाङ्क printed;
    आकार_t उपलब्धम्;

    उपलब्धम् = buf->alloc - buf->आकार;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    प्रतिज्ञा(buf->buf || उपलब्धम् == 0);
    va_start(तर्काः, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->आकार : buf->buf, उपलब्धम्,
                        fmt, तर्काः);
    va_अन्तः(तर्काः);

    यदि (printed < 0) {
        /* Some error */
        गच्छ err;
    } अन्यथा यदि ((आकार_t)printed >= उपलब्धम्) {
        /* Not enough space: need to realloc */
        यदि (!do_realloc(buf, (आकार_t)printed + 1))
            गच्छ err;

        /* The buffer has enough space now. */
        उपलब्धम् = buf->alloc - buf->आकार;
        प्रतिज्ञा(buf->buf && (आकार_t) printed < उपलब्धम्);
        va_start(तर्काः, fmt);
        printed = vsnprintf(buf->buf + buf->आकार, उपलब्धम्, fmt, तर्काः);
        va_अन्तः(तर्काः);

        यदि (printed < 0 || (आकार_t) printed >= उपलब्धम्)
            गच्छ err;
    }

    buf->आकार += (आकार_t)printed;
    प्रत्यागम true;

err:
    मुक्त(buf->buf);
    buf->buf = रिक्त;
    प्रत्यागम false;
}

#परिभाषय write_buf(buf, ...) कुरु { \
    यदि (!check_write_buf(buf, __VA_ARGS__)) \
        प्रत्यागम false; \
} यावत् (0)

स्थिर bool
check_copy_गन्तव्यम्_buf(संरचना buf *buf, ध्रुव वर्ण* source, आकार_t len)
{
    यदि (len == 0)
        प्रत्यागम true;

    ध्रुव आकार_t उपलब्धम् = buf->alloc - buf->आकार;
    यदि (len >= उपलब्धम् &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        मुक्त(buf->buf);
        buf->buf = रिक्त;
        प्रत्यागम false;
    }

    स्मृतिं_प्रतिलिख(buf->buf + buf->आकार, source, len);
    buf->आकार += len;
    /* Append NULL byte */
    buf->buf[buf->आकार] = '\0';
    प्रत्यागम true;
}

#परिभाषय copy_गन्तव्यम्_buf_len(buf, source, len) कुरु { \
    यदि (!check_copy_गन्तव्यम्_buf(buf, source, len)) \
        प्रत्यागम false; \
} यावत् (0)

#परिभाषय copy_गन्तव्यम्_buf(buf, source) \
    copy_गन्तव्यम्_buf_len(buf, source, परिमाण(source) - 1)

स्थिर bool
check_write_string_literal(संरचना buf *buf, ध्रुव वर्ण* string)
{
    copy_गन्तव्यम्_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    आकार_t pending_start = 0;
    आकार_t current = 0;
    वर्ण escape_बफर[] = {'\\', '0', '0', '0', '0'};
    प्रति (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        विकल्प (string[current]) {
        पक्ष '\n':
            /* `\n` would break strings */
            escape_बफर[1] = 'n';
            escape_len = 2;
            भङ्ग;
        पक्ष '\\':
            /* `\` would create escape sequences */
            escape_बफर[1] = '\\';
            escape_len = 2;
            भङ्ग;
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
            यदि (unlikely((अचिह्नित वर्ण)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_बफर[1] = '0';
                escape_बफर[2] = '0';
                escape_बफर[3] = (वर्ण)('0' + (string[current] >> 3));
                escape_बफर[4] = (वर्ण)('0' + (string[current] & 0x7));
                escape_len = 5;
            } अन्यथा {
                /* Expand the chunk */
                अनुवर्त;
            }
        }
        /* Write pending chunk */
        प्रतिज्ञा(current >= pending_start);
        copy_गन्तव्यम्_buf_len(buf, string + pending_start, current - pending_start);
        /* Write escape sequence */
        copy_गन्तव्यम्_buf_len(buf, escape_बफर, escape_len);
        pending_start = current + 1;
    }
    /* Write remaining chunk */
    प्रतिज्ञा(current >= pending_start);
    copy_गन्तव्यम्_buf_len(buf, string + pending_start, current - pending_start);

    copy_गन्तव्यम्_buf(buf, "\"");
    प्रत्यागम true;
}

#परिभाषय write_buf_string_literal(buf, string) कुरु { \
    यदि (!check_write_string_literal(buf, string)) \
        प्रत्यागम false; \
} यावत् (0)

स्थिर शून्य
delete_चरमः_char(संरचना buf *buf, वर्ण c)
{
    यदि (buf->आकार && buf->buf[buf->आकार - 1] == c)
        buf->buf[--buf->आकार] = '\0';
}

/** Entry for a name substituion LUT */
संरचना कुञ्चिका_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
स्थिर पूर्णाङ्क
cmp_कुञ्चिका_name_substitution(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    ध्रुव xkb_atom_t s1 = ((संरचना कुञ्चिका_name_substitution *) a)->source;
    ध्रुव xkb_atom_t s2 = ((संरचना कुञ्चिका_name_substitution *) b)->source;
    प्रत्यागम (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

प्रकारनिर्देश darray(संरचना कुञ्चिका_name_substitution) कुञ्चिका_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
स्थिर bool
rename_long_कुञ्चिकाः(ध्रुव संरचना xkb_keymap *keymap,
                 कुञ्चिका_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    गणना {
        first = '!',
        चरमः = '0',
        invalid = '\0'
    };
    वर्ण prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', चरमः
    };
    static_assert(परिमाण(prefixes) == चरमः - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका;
    xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
        यदि (कुञ्चिका->name == XKB_ATOM_NONE)
            अनुवर्त;

        ध्रुव वर्ण * ध्रुव name = xkb_atom_text(keymap->ctx, कुञ्चिका->name);
        ध्रुव आकार_t len = strlen_safe(name);
        यदि (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (संरचना कुञ्चिका_name_substitution) {
                .source = कुञ्चिका->name,
                .keycode = कुञ्चिका->keycode,
                .target = कुञ्चिका->name
            });
        } अन्यथा यदि (len == 4 && name[0] >= first && name[0] <= चरमः) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    प्रति (darray_आकार_t a = 0; a < keymap->अङ्कः_कुञ्चिका_aliases; a++) {
        ध्रुव संरचना xkb_कुञ्चिका_alias * ध्रुव alias = &keymap->कुञ्चिका_aliases[a];
        ध्रुव वर्ण * ध्रुव name = xkb_atom_text(keymap->ctx, alias->alias);
        ध्रुव आकार_t len = strlen_safe(name);
        यदि (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (संरचना कुञ्चिका_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } अन्यथा यदि (len == 4 && name[0] >= first && name[0] <= चरमः) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    यदि darray_रिक्तानि(*substitutions) {
        /* Nothing to do */
        प्रत्यागम true;
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
    वर्ण *उपसर्गः = prefixes;
    ध्रुव वर्ण *उपसर्गः_max = prefixes + परिमाण(prefixes) - 1;
    यावत् (*उपसर्गः == invalid && उपसर्गः < उपसर्गः_max)
        उपसर्गः++;

    /* Keycode prefix: prefer ‘0’ if available */
    वर्ण keycode_उपसर्गः;
    यदि (prefixes[चरमः - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_उपसर्गः = चरमः;
        prefixes[चरमः - first] = invalid;
    } अन्यथा {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_उपसर्गः = *उपसर्गः;
        यदि (unlikely(keycode_उपसर्गः == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            प्रत्यागम true;
        }
        /* Update current prefix */
        *उपसर्गः = invalid;
        यावत् (*उपसर्गः == invalid && उपसर्गः < उपसर्गः_max)
            उपसर्गः++;
    }

    /* Proceed renaming */
    uint16_t अग्र_id = 1;
    संरचना कुञ्चिका_name_substitution *sub;
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
                     keycode_उपसर्गः, sub->keycode);
            ध्रुव xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, परिमाण(buf) - 2);
            यदि (target == XKB_ATOM_NONE)
                गच्छ error;
            sub->target = target;
        } अन्यथा यदि (*उपसर्गः != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, परिमाण(buf), "%c%03"PRIx16, *उपसर्गः, अग्र_id);
            ध्रुव xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, परिमाण(buf) - 2);
            यदि (target == XKB_ATOM_NONE)
                गच्छ error;
            sub->target = target;
            यदि (++अग्र_id > 0xfff) {
                *उपसर्गः = invalid;
                /* Get next available prefix */
                यावत् (*उपसर्गः == invalid && उपसर्गः < उपसर्गः_max)
                    उपसर्गः++;
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
    शीघ्रं_क्रमय(darray_वस्तूनि(*substitutions), darray_आकार(*substitutions),
          परिमाण(*darray_वस्तूनि(*substitutions)), &cmp_कुञ्चिका_name_substitution);

    प्रत्यागम true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_आवण्टनम्_FAILURE_2,
            "Cannot allocate key name substitution\n");
    प्रत्यागम false;
}

/** Substitute a key name with a valid X11 key name */
स्थिर xkb_atom_t
substitute_name(ध्रुव कुञ्चिका_name_substitutions *substitutions, xkb_atom_t name)
{
    /*द्विमान अन्वेषणम्*/
    darray_आकार_t first = 0;
    darray_आकार_t चरमः = darray_आकार(*substitutions);

    यावत् (चरमः > first) {
        darray_आकार_t mid = first + (चरमः - 1 - first) / 2;
        ध्रुव xkb_atom_t source = darray_item(*substitutions, mid).source;
        यदि (source < name)
            first = mid + 1;
        अन्यथा यदि (source > name)
            चरमः = mid;
        अन्यथा /* found it */
            प्रत्यागम darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    प्रत्यागम name;
}

स्थिर bool
write_vmods(ध्रुव संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
            bool explicit, संरचना buf *buf)
{
    ध्रुव संरचना xkb_mod *mod;
    xkb_mod_सूचकांक_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        यदि (!has_some) {
            copy_गन्तव्यम्_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } अन्यथा {
            copy_गन्तव्यम्_buf(buf, ",");
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
        ध्रुव bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        यदि ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
             explicit) &&
            (auto_canonical_mods || mod->mapping != 0)) {
            /*
             * Explicit non-default mapping
             * NOTE: we can only pretty-print *real* modifiers in this context.
             */
            write_buf(buf, "=%s",
                      Mod_आवरणम्_Text(keymap->ctx, MOD_REAL, &keymap->mods,
                                  mod->mapping));
        }
    }

    यदि (has_some)
        copy_गन्तव्यम्_buf(buf, ";\n\n");

    प्रत्यागम true;
}

स्थिर bool
write_keycodes(ध्रुव संरचना xkb_keymap *keymap,
               ध्रुव कुञ्चिका_name_substitutions *substitutions,
               गणना xkb_keymap_serialize_flags flags, संरचना buf *buf)
{
    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका;
    xkb_led_सूचकांक_t idx;
    ध्रुव संरचना xkb_led *led;

    ध्रुव bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    यदि (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    अन्यथा
        copy_गन्तव्यम्_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_कुञ्चिका_संहिता, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_कुञ्चिका_संहिता, 255));

    xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
        यदि (कुञ्चिका->name == XKB_ATOM_NONE)
            अनुवर्त;

        ध्रुव xkb_atom_t name = (substitutions == रिक्त)
            ? कुञ्चिका->name
            : substitute_name(substitutions, कुञ्चिका->name);
        यदि (pretty)
            write_buf(buf, "\t%-20s", कुञ्चिका_Name_Text(keymap->ctx, name));
        अन्यथा
            write_buf(buf, "\t%s", कुञ्चिका_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", कुञ्चिका->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        यदि (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_गन्तव्यम्_buf(buf, ";\n");
        }

    प्रति (darray_आकार_t i = 0; i < keymap->अङ्कः_कुञ्चिका_aliases; i++) {
        ध्रुव xkb_atom_t alias = (substitutions == रिक्त)
            ? keymap->कुञ्चिका_aliases[i].alias
            : substitute_name(substitutions, keymap->कुञ्चिका_aliases[i].alias);
        ध्रुव xkb_atom_t real = (substitutions == रिक्त)
            ? keymap->कुञ्चिका_aliases[i].real
            : substitute_name(substitutions, keymap->कुञ्चिका_aliases[i].real);

        यदि (pretty)
            write_buf(buf, "\talias %-14s",
                      कुञ्चिका_Name_Text(keymap->ctx, alias));
        अन्यथा
            write_buf(buf, "\talias %s",
                      कुञ्चिका_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  कुञ्चिका_Name_Text(keymap->ctx, real));
    }

    copy_गन्तव्यम्_buf(buf, "};\n\n");
    प्रत्यागम true;
}

स्थिर bool
write_types(ध्रुव संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
            गणना xkb_keymap_serialize_flags flags, संरचना buf *buf)
{
    यदि (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    अन्यथा
        copy_गन्तव्यम्_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    यदि (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        प्रत्यागम false;

    ध्रुव bool drop_अप्रयुक्तम् = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_अप्रयुक्तम्);
    प्रति (darray_आकार_t i = 0; i < keymap->अङ्कः_types; i++) {
        ध्रुव संरचना xkb_कुञ्चिका_type * ध्रुव type = &keymap->types[i];
        यदि (!type->required && drop_अप्रयुक्तम्)
            अनुवर्त;

        copy_गन्तव्यम्_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_गन्तव्यम्_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_आवरणम्_Text(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        प्रति (darray_आकार_t j = 0; j < type->अङ्कः_entries; j++) {
            ध्रुव वर्ण *str;
            ध्रुव संरचना xkb_कुञ्चिका_type_प्रविष्टिः *प्रविष्टिः = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            यदि (प्रविष्टिः->level == 0 && प्रविष्टिः->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मानानि))
                अनुवर्त;

            str = Mod_आवरणम्_Text(keymap->ctx, MOD_BOTH, &keymap->mods,
                              प्रविष्टिः->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, प्रविष्टिः->level + 1);

            यदि (प्रविष्टिः->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, Mod_आवरणम्_Text(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           प्रविष्टिः->preserve.mods));
        }

        प्रति (xkb_level_सूचकांक_t n = 0; n < type->अङ्कः_level_नामानि; n++)
            यदि (type->level_नामानि[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_नामानि[n]));
                copy_गन्तव्यम्_buf(buf, ";\n");
            }

        copy_गन्तव्यम्_buf(buf, "\t};\n");
    }

    copy_गन्तव्यम्_buf(buf, "};\n\n");
    प्रत्यागम true;
}

स्थिर bool
write_led_map(ध्रुव संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
              bool defaults, संरचना buf *buf, ध्रुव संरचना xkb_led *led)
{
    copy_गन्तव्यम्_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_गन्तव्यम्_buf(buf, " {\n");

    यदि (led->which_groups) {
        यदि (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      Led_State_आवरणम्_Text(keymap->ctx, गणः_Component_आवरणम्_नामानि,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    यदि (led->which_mods) {
        यदि (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      Led_State_आवरणम्_Text(keymap->ctx, mod_Component_आवरणम्_नामानि,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_आवरणम्_Text(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    यदि (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  Control_आवरणम्_Text(keymap->ctx, format, led->ctrls));
    }

    copy_गन्तव्यम्_buf(buf, "\t};\n");
    प्रत्यागम true;
}

स्थिर ध्रुव वर्ण *
affect_तालकम्_text(गणना xkb_action_flags flags, bool show_both)
{
    विकल्प (flags & (ACTION_तालकम्_NO_तालकम् | ACTION_तालकम्_NO_UNLOCK)) {
    पक्ष 0:
        प्रत्यागम show_both ? ",affect=both" : "";
    पक्ष ACTION_तालकम्_NO_UNLOCK:
        प्रत्यागम ",affect=lock";
    पक्ष ACTION_तालकम्_NO_तालकम्:
        प्रत्यागम ",affect=unlock";
    पक्ष ACTION_तालकम्_NO_तालकम् | ACTION_तालकम्_NO_UNLOCK:
        प्रत्यागम ",affect=neither";
    स्वभाव:
        प्रत्यागम "";
    }
}

#परिभाषय चिह्नम्_PADDING 15
#परिभाषय ACTION_PADDING 30

स्थिर bool
write_action(ध्रुव संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
             xkb_layout_सूचकांक_t max_groups, संरचना buf *buf,
             ध्रुव सङ्घ xkb_action *action,
             ध्रुव वर्ण *उपसर्गः, ध्रुव वर्ण *suffix)
{
    ध्रुव वर्ण *type;
    ध्रुव वर्ण *तर्काः = रिक्त;

    यदि (!उपसर्गः)
        उपसर्गः = "";
    यदि (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    विकल्प (action->type) {
    पक्ष ACTION_TYPE_MOD_समुच्चयः:
    पक्ष ACTION_TYPE_MOD_LATCH:
    पक्ष ACTION_TYPE_MOD_तालकम्:
        यदि (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            तर्काः = "modMapMods";
        अन्यथा
            तर्काः = Mod_आवरणम्_Text(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        यदि (unlockOnPress && !is_Mods_Un_तालकम्_On_Press_Supported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->गणः.flags & ACTION_LATCH_ON_PRESS);
        यदि (latchOnPress && !isModsLatchOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"LatchMods(latchOnPress=true)\" "
                    "in keymap format %d\n", format);
            latchOnPress = false;
        }
        write_buf(buf, "%s%s(modifiers=%s%s%s%s%s%s)%s", उपसर्गः, type, तर्काः,
                  (action->type != ACTION_TYPE_MOD_तालकम् && (action->mods.flags & ACTION_तालकम्_CLEAR)) ? ",clearLocks" : "",
                  (action->type == ACTION_TYPE_MOD_LATCH && (action->mods.flags & ACTION_LATCH_गन्तव्यम्_तालकम्)) ? ",latchToLock" : "",
                  (action->type == ACTION_TYPE_MOD_तालकम्) ? affect_तालकम्_text(action->mods.flags, false) : "",
                  (unlockOnPress) ? ",unlockOnPress" : "",
                  (latchOnPress) ? ",latchOnPress" : "",
                  suffix);
        भङ्ग;

    पक्ष ACTION_TYPE_गणः_समुच्चयः:
    पक्ष ACTION_TYPE_गणः_LATCH:
    पक्ष ACTION_TYPE_गणः_तालकम्:
        यदि ((uint32_t) xkb_abs(action->गणः.गणः) <
            (max_groups + !(action->गणः.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool तालकम्_On_Release = action->type == ACTION_TYPE_गणः_तालकम् &&
                                 (action->गणः.flags & ACTION_तालकम्_ON_RELEASE);
            यदि (तालकम्_On_Release && !is_गणः_तालकम्_On_Release_Supported(format)) {
                log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                        "Cannot use \"GroupLock(lockOnRelease=true)\" "
                        "in keymap format %d\n", format);
                तालकम्_On_Release = false;
            }
            write_buf(buf, "%s%s(group=%s%"PRId32"%s%s%s)%s", उपसर्गः, type,
                      (!(action->गणः.flags & ACTION_ABSOLUTE_SWITCH) && action->गणः.गणः >= 0) ? "+" : "",
                      (action->गणः.flags & ACTION_ABSOLUTE_SWITCH) ? action->गणः.गणः + 1 : action->गणः.गणः,
                      (action->type != ACTION_TYPE_गणः_तालकम् && (action->गणः.flags & ACTION_तालकम्_CLEAR)) ? ",clearLocks" : "",
                      (action->type == ACTION_TYPE_गणः_LATCH && (action->गणः.flags & ACTION_LATCH_गन्तव्यम्_तालकम्)) ? ",latchToLock" : "",
                      (तालकम्_On_Release) ? ",lockOnRelease" : "",
                      suffix);
        } अन्यथा {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->गणः.गणः +
                       !!(action->गणः.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            गच्छ void_action;
        }
        भङ्ग;

    पक्ष ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", उपसर्गः, type, suffix);
        भङ्ग;

    पक्ष ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", उपसर्गः, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        भङ्ग;

    पक्ष ACTION_TYPE_PTR_तालकम्:
        तर्काः = affect_तालकम्_text(action->btn.flags, true);
        /* fallthrough */
    पक्ष ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", उपसर्गः, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        यदि (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        अन्यथा
            copy_गन्तव्यम्_buf(buf, "default");
        यदि (action->btn.गणनम्) {
            प्रतिज्ञा(action->type != ACTION_TYPE_PTR_तालकम्);
            write_buf(buf, ",count=%u", action->btn.गणनम्);
        }
        यदि (तर्काः)
            write_buf(buf, "%s", तर्काः);
        write_buf(buf, ")%s", suffix);
        भङ्ग;

    पक्ष ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", उपसर्गः, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.मानम् >= 0) ? "+" : "",
                  action->dflt.मानम्);
        write_buf(buf, ")%s", suffix);
        भङ्ग;

    पक्ष ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", उपसर्गः, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        भङ्ग;

    पक्ष ACTION_TYPE_CTRL_समुच्चयः:
    पक्ष ACTION_TYPE_CTRL_तालकम्:
        write_buf(buf, "%s%s(controls=%s%s)%s", उपसर्गः, type,
                  Control_आवरणम्_Text(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_तालकम्
                    ? affect_तालकम्_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        भङ्ग;

    पक्ष ACTION_TYPE_REDIRECT_कुञ्चिका: {
        write_buf(buf, "%s%s(", उपसर्गः, type);
        ध्रुव संरचना xkb_कुञ्चिका * ध्रुव कुञ्चिका = Xkb_कुञ्चिका(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        यदि (कुञ्चिका)
            write_buf(buf, "keycode=%s", कुञ्चिका_Name_Text(keymap->ctx, कुञ्चिका->name));
        यदि (action->redirect.affect_mods) {
            xkb_mod_आवरणम्_t आवरणम्;
            आवरणम् = (action->redirect.affect_mods & action->redirect.mods);
            यदि (आवरणम्)
                write_buf(buf, ",modifiers=%s",
                          Mod_आवरणम्_Text(keymap->ctx, MOD_BOTH, &keymap->mods, आवरणम्));
            आवरणम् = (action->redirect.affect_mods & ~action->redirect.mods);
            यदि (आवरणम्)
                write_buf(buf, ",clearMods=%s",
                          Mod_आवरणम्_Text(keymap->ctx, MOD_BOTH, &keymap->mods, आवरणम्));
        }
        write_buf(buf, ")%s", suffix);
        भङ्ग;
    }

    पक्ष ACTION_TYPE_UNKNOWN:
    पक्ष ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", उपसर्गः, suffix);
        भङ्ग;

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
                      उपसर्गः, suffix);
        अन्यथा
            write_buf(buf, "%sVoidAction()%s", उपसर्गः, suffix);
        भङ्ग;

    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_आन्तरिकम् == 20 &&
                      ACTION_TYPE_आन्तरिकम् + 1 == ACTION_TYPE_अङ्कः_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        प्रतिज्ञा(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  उपसर्गः, type, action->type, action->priv.दत्त[0],
                  action->priv.दत्त[1], action->priv.दत्त[2],
                  action->priv.दत्त[3], action->priv.दत्त[4],
                  action->priv.दत्त[5], action->priv.दत्त[6],
                  suffix);
        भङ्ग;
    }

    प्रत्यागम true;
}

स्थिर ध्रुव सङ्घ xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

स्थिर bool
write_actions(ध्रुव संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
              xkb_layout_सूचकांक_t max_groups, संरचना buf *buf, संरचना buf *buf2,
              ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका, xkb_layout_सूचकांक_t गणः)
{
    स्थिर ध्रुव सङ्घ xkb_action noAction = { .type = ACTION_TYPE_NONE };

    प्रति (xkb_level_सूचकांक_t level = 0; level < Xkb_कुञ्चिका_अङ्कः_स्तराः(कुञ्चिका, गणः);
         level++) {

        यदि (level != 0)
            copy_गन्तव्यम्_buf(buf, ", ");

        ध्रुव सङ्घ xkb_action *actions = रिक्त;
        xkb_action_गणनम्_t गणनम् = xkb_keymap_कुञ्चिका_get_actions_by_level(
            keymap, कुञ्चिका, गणः, level, &actions
        );

        यदि (गणनम् > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            गणनम् = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->आकार = 0;
        यदि (गणनम् == 0) {
            यदि (!write_action(keymap, format, max_groups,
                              buf2, &noAction, रिक्त, रिक्त))
                प्रत्यागम false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        अन्यथा यदि (गणनम् == 1) {
            यदि (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), रिक्त, रिक्त))
                प्रत्यागम false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        अन्यथा {
            copy_गन्तव्यम्_buf(buf2, "{ ");
            प्रति (xkb_action_गणनम्_t k = 0; k < गणनम्; k++) {
                यदि (k != 0)
                    copy_गन्तव्यम्_buf(buf2, ", ");
                आकार_t old_आकार = buf2->आकार;
                यदि (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), रिक्त, रिक्त))
                    प्रत्यागम false;
                /* Check if padding is necessary */
                यदि (buf2->आकार >= old_आकार + ACTION_PADDING)
                    अनुवर्त;
                /* Compute and write padding, then write the action again */
                ध्रुव पूर्णाङ्क padding = (पूर्णाङ्क)(old_आकार + ACTION_PADDING - buf2->आकार);
                buf2->आकार = old_आकार;
                write_buf(buf2, "%*s", padding, "");
                यदि (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), रिक्त, रिक्त))
                    प्रत्यागम false;
            }
            copy_गन्तव्यम्_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    प्रत्यागम true;
}

स्थिर bool
write_action_defaults(ध्रुव संरचना xkb_keymap *keymap,
                      गणना xkb_keymap_format format,
                      ध्रुव सङ्घ xkb_action *action,
                      संरचना buf *buf)
{
    ध्रुव वर्ण *type = ActionTypeText(action->type);

#परिभाषय उपसर्गः_2 "\t"

    विकल्प (action->type) {
    पक्ष ACTION_TYPE_MOD_समुच्चयः:
    पक्ष ACTION_TYPE_MOD_LATCH:
    पक्ष ACTION_TYPE_MOD_तालकम्:
        प्रतिज्ञा(action->mods.flags == 0);
        प्रतिज्ञा(action->mods.mods.mods == 0);
        write_buf(buf, उपसर्गः_2"%s.modifiers = 0;\n", type);
        यदि (action->type != ACTION_TYPE_MOD_तालकम्)
            write_buf(buf, उपसर्गः_2"%s.clearLocks = false;\n", type);
        यदि (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, उपसर्गः_2"%s.latchOnPress = false;\n", type);
        यदि (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, उपसर्गः_2"%s.latchToLock = false;\n", type);
        यदि (action->type == ACTION_TYPE_MOD_तालकम्)
            write_buf(buf, उपसर्गः_2"%s.affect = both;\n", type);
        यदि (is_Mods_Un_तालकम्_On_Press_Supported(format))
            write_buf(buf, उपसर्गः_2"%s.unlockOnPress = false;\n", type);
        भङ्ग;

    पक्ष ACTION_TYPE_गणः_समुच्चयः:
    पक्ष ACTION_TYPE_गणः_LATCH:
    पक्ष ACTION_TYPE_गणः_तालकम्:
        प्रतिज्ञा(action->गणः.flags == 0);
        प्रतिज्ञा(action->गणः.गणः == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, उपसर्गः_2"%s.group = +0;\n", type);
        यदि (action->type != ACTION_TYPE_गणः_तालकम्)
            write_buf(buf, उपसर्गः_2"%s.clearLocks = false;\n", type);
        यदि (action->type == ACTION_TYPE_गणः_LATCH)
            write_buf(buf, उपसर्गः_2"%s.latchToLock = false;\n", type);
        यदि (action->type == ACTION_TYPE_गणः_तालकम् &&
            is_गणः_तालकम्_On_Release_Supported(format))
            write_buf(buf, उपसर्गः_2"%s.lockOnRelease = false;\n", type);
        भङ्ग;


    पक्ष ACTION_TYPE_PTR_MOVE:
        प्रतिज्ञा(action->ptr.x == 0);
        प्रतिज्ञा(action->ptr.y == 0);
        प्रतिज्ञा(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, उपसर्गः_2"%s.x = +0;\n", type);
        write_buf(buf, उपसर्गः_2"%s.y = +0;\n", type);
        write_buf(buf, उपसर्गः_2"%s.accel = true;\n", type);
        भङ्ग;

    पक्ष ACTION_TYPE_PTR_तालकम्:
    पक्ष ACTION_TYPE_PTR_BUTTON:
        प्रतिज्ञा(action->btn.flags == 0);
        प्रतिज्ञा(action->btn.button == 0);
        प्रतिज्ञा(action->btn.गणनम् == 0);
        write_buf(buf, उपसर्गः_2"%s.button = 0;\n", type);
        write_buf(buf, उपसर्गः_2"%s.count = 0;\n", type);
        यदि (action->type == ACTION_TYPE_PTR_तालकम्)
            write_buf(buf, उपसर्गः_2"%s.affect = both;\n", type);
        भङ्ग;

    पक्ष ACTION_TYPE_PTR_DEFAULT:
        प्रतिज्ञा(action->dflt.flags == 0);
        प्रतिज्ञा(action->dflt.मानम् == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, उपसर्गः_2"%s.button = +1;\n", type);
        भङ्ग;

    पक्ष ACTION_TYPE_SWITCH_VT:
        प्रतिज्ञा(action->screen.flags == ACTION_SAME_SCREEN);
        प्रतिज्ञा(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, उपसर्गः_2"%s.screen = +0;\n", type);
        write_buf(buf, उपसर्गः_2"%s.same = true;\n", type);
        भङ्ग;

    पक्ष ACTION_TYPE_CTRL_समुच्चयः:
    पक्ष ACTION_TYPE_CTRL_तालकम्:
        प्रतिज्ञा(action->ctrls.flags == 0);
        प्रतिज्ञा(action->ctrls.ctrls == 0);
        write_buf(buf, उपसर्गः_2"%s.controls = 0;\n", type);
        यदि (action->type == ACTION_TYPE_CTRL_तालकम्)
            write_buf(buf, उपसर्गः_2"%s.affect = both;\n", type);
        भङ्ग;

    पक्ष ACTION_TYPE_REDIRECT_कुञ्चिका:
        प्रतिज्ञा(action->redirect.keycode == keymap->redirect_कुञ्चिका_auto);
        प्रतिज्ञा(action->redirect.affect_mods == 0);
        प्रतिज्ञा(action->redirect.mods == 0);
        write_buf(buf, उपसर्गः_2"%s.keycode = auto;\n", type);
        write_buf(buf, उपसर्गः_2"%s.modifiers = 0;\n", type);
        write_buf(buf, उपसर्गः_2"%s.clearMods = 0;\n", type);
        भङ्ग;

    स्वभाव:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_आन्तरिकम् == 20 &&
                      ACTION_TYPE_आन्तरिकम् + 1 == ACTION_TYPE_अङ्कः_ENTRIES,
                      "Missing action type");
    }

#अपरिभाषय उपसर्गः_2

    प्रत्यागम true;
}

स्थिर bool
write_actions_defaults(ध्रुव संरचना xkb_keymap *keymap,
                       गणना xkb_keymap_format format, संरचना buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    प्रति (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_अङ्कः_ENTRIES; a++) {
        ध्रुव सङ्घ xkb_action * सीमित ध्रुव action = &defaults.actions[a];
        यदि (!write_action_defaults(keymap, format, action, buf))
            प्रत्यागम false;
    }
    copy_गन्तव्यम्_buf(buf, "\n");

    प्रत्यागम true;
}

स्थिर ध्रुव संरचना xkb_sym_interpret fallback_interpret = {
    .sym = XKB_कुञ्चिका_Void_चिह्नम्,
    .required = true,
    .repeat = FALLBACK_INTERPRET_कुञ्चिका_REPEAT,
    .मेलनम् = मेलनम्_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_सूचकांक_t) DEFAULT_INTERPRET_VMOD,
    .अङ्कः_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

स्थिर bool
write_compat(ध्रुव संरचना xkb_keymap * सीमित keymap,
             गणना xkb_keymap_format format, xkb_layout_सूचकांक_t max_groups,
             गणना xkb_keymap_serialize_flags flags, bool * सीमित some_interp,
             संरचना buf *buf)
{
    ध्रुव bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ध्रुव bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मानानि);
    ध्रुव bool drop_अप्रयुक्तम् = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_अप्रयुक्तम्);
    ध्रुव bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_कुञ्चिका_मानानि) && drop_अप्रयुक्तम्);
    /* TODO: print all default values if the explicit flag is set */

    यदि (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    अन्यथा
        copy_गन्तव्यम्_buf(buf, "xkb_compatibility {\n");

    यदि (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        प्रत्यागम false;

    यदि (defaults && !write_actions_defaults(keymap, format, buf))
        प्रत्यागम false;

    copy_गन्तव्यम्_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_गन्तव्यम्_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    ध्रुव bool use_fallback_interpret =
        (!keymap->अङ्कः_sym_interprets || drop_interprets);
    ध्रुव darray_आकार_t अङ्कः_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->अङ्कः_sym_interprets);
    ध्रुव संरचना xkb_sym_interpret* ध्रुव sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    प्रति (darray_आकार_t i = 0; i < अङ्कः_sym_interprets; i++) {
        ध्रुव संरचना xkb_sym_interpret *si = &sym_interprets[i];
        यदि (!si->required && drop_अप्रयुक्तम्)
            अनुवर्त;

        *some_interp = true;

        copy_गन्तव्यम्_buf(buf, "\tinterpret ");
        यदि (!si->sym) {
            copy_गन्तव्यम्_buf(buf, "Any");
        } अन्यथा यदि (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } अन्यथा {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->मेलनम्),
                  Mod_आवरणम्_Text(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        यदि (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      Mod_सूचकांक_Text(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        यदि (si->level_one_only) {
            copy_गन्तव्यम्_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } अन्यथा यदि (defaults) {
            copy_गन्तव्यम्_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        यदि (si->repeat) {
            copy_गन्तव्यम्_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } अन्यथा यदि (defaults) {
            copy_गन्तव्यम्_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        ध्रुव सङ्घ xkb_action *action = रिक्त;
        xkb_action_गणनम्_t action_गणनम् = si->अङ्कः_actions;
        यदि (action_गणनम् <= 1) {
            action = &si->a.action;
        } अन्यथा यदि (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_गणनम् = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        यदि (action_गणनम् > 1) {
            copy_गन्तव्यम्_buf(buf, "\n\t\taction= {");
            ध्रुव वर्ण suffix[] = ", ";
            प्रति (xkb_action_गणनम्_t k = 0; k < si->अङ्कः_actions; k++) {
                यदि (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    प्रत्यागम false;
            }
            buf->आकार -= परिमाण(suffix) - 1; /* trailing comma */
            copy_गन्तव्यम्_buf(buf, "};");
            has_explicit_properties = true;
        } अन्यथा यदि (action_गणनम् == 1) {
            यदि (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                प्रत्यागम false;
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
        अङ्कः_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].मेलनम् == fallback_interpret.मेलनम् &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].अङ्कः_actions == fallback_interpret.अङ्कः_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    ध्रुव संरचना xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        यदि ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            प्रत्यागम false;
        }
    }

    copy_गन्तव्यम्_buf(buf, "};\n\n");

    प्रत्यागम true;
}

स्थिर bool
write_keysyms(ध्रुव संरचना xkb_keymap *keymap,
              संरचना buf *buf, संरचना buf *buf2,
              ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका, xkb_layout_सूचकांक_t गणः,
              bool pretty, bool show_actions)
{
    ध्रुव अचिह्नित पूर्णाङ्क padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : चिह्नम्_PADDING)
        : 0;
    प्रति (xkb_level_सूचकांक_t level = 0; level < Xkb_कुञ्चिका_अङ्कः_स्तराः(कुञ्चिका, गणः);
         level++) {
        ध्रुव xkb_keysym_t *syms;
        पूर्णाङ्क अङ्कः_syms;

        यदि (level != 0)
            copy_गन्तव्यम्_buf(buf, ", ");

        अङ्कः_syms = xkb_keymap_कुञ्चिका_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (संरचना xkb_keymap *)keymap,
            कुञ्चिका->keycode, गणः, level, &syms
        );

        ध्रुव xkb_keysym_t no_चिह्नम् = XKB_कुञ्चिका_No_चिह्नम्;
        यदि (अङ्कः_syms == 0) {
            अङ्कः_syms = 1;
            syms = &no_चिह्नम्;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        यदि (अङ्कः_syms == 1) {
            यदि (pretty || syms[0] == XKB_कुञ्चिका_No_चिह्नम्)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            अन्यथा
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } अन्यथा {
            यदि (pretty) {
                buf2->आकार = 0;
                copy_गन्तव्यम्_buf(buf2, "{ ");
                प्रति (पूर्णाङ्क s = 0; s < अङ्कः_syms; s++) {
                    यदि (s != 0)
                        copy_गन्तव्यम्_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_गन्तव्यम्_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } अन्यथा {
                copy_गन्तव्यम्_buf(buf, "{");
                प्रति (पूर्णाङ्क s = 0; s < अङ्कः_syms; s++) {
                    यदि (s != 0)
                        copy_गन्तव्यम्_buf(buf, ",");
                    यदि (syms[s] == XKB_कुञ्चिका_No_चिह्नम्)
                        copy_गन्तव्यम्_buf(buf, "NoSymbol");
                    अन्यथा
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_गन्तव्यम्_buf(buf, "}");
            }
        }
    }

    प्रत्यागम true;
}

स्थिर bool
write_कुञ्चिका(ध्रुव संरचना xkb_keymap *keymap,
          ध्रुव संरचना xkb_keymap_serialize_config *config,
          ध्रुव कुञ्चिका_name_substitutions *substitutions,
          xkb_layout_सूचकांक_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          संरचना buf *buf, संरचना buf *buf2, ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका)
{
    bool simple = true;

    xkb_layout_आवरणम्_t layouts = config->layouts;
    यदि (popcount32(layouts) != keymap->अङ्कः_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_सूचकांक_t l = 0;
        प्रति (xkb_layout_आवरणम्_t ls = layouts; ls; ls >>= 1, l++) {
            यदि (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ध्रुव xkb_layout_सूचकांक_t effective =
                    xkb_keymap_कुञ्चिका_effective_layout(कुञ्चिका, l);
                यदि (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } अन्यथा {
        layouts &= ((UINT64_C(1) << कुञ्चिका->अङ्कः_groups) - 1);
    }
    ध्रुव xkb_layout_सूचकांक_t groups_गणनम् = popcount32(layouts);

    ध्रुव xkb_atom_t name = (substitutions == रिक्त)
        ? कुञ्चिका->name
        : substitute_name(substitutions, कुञ्चिका->name);

    यदि (pretty)
        write_buf(buf, "\tkey %-20s {", कुञ्चिका_Name_Text(keymap->ctx, name));
    अन्यथा
        write_buf(buf, "\tkey %s {", कुञ्चिका_Name_Text(keymap->ctx, name));

    यदि ((कुञ्चिका->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        यदि (!explicit) {
            प्रति (xkb_layout_सूचकांक_t गणः = 1; गणः < कुञ्चिका->अङ्कः_groups; गणः++) {
                यदि (!((UINT32_C(1) << गणः) & layouts))
                    अनुवर्त;

                यदि (कुञ्चिका->groups[गणः].type != कुञ्चिका->groups[0].type) {
                    multi_type = true;
                    भङ्ग;
                }
            }
        }

        यदि (multi_type) {
            xkb_layout_सूचकांक_t new_गणः = (xkb_layout_सूचकांक_t)-1;
            प्रति (xkb_layout_सूचकांक_t गणः = 0; गणः < कुञ्चिका->अङ्कः_groups; गणः++) {
                यदि (!((UINT32_C(1) << गणः) & layouts))
                    अनुवर्त;

                new_गणः++;

                यदि (!कुञ्चिका->groups[गणः].explicit_type && !explicit)
                    अनुवर्त;

                ध्रुव संरचना xkb_कुञ्चिका_type * ध्रुव type = कुञ्चिका->groups[गणः].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_गणः + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_गन्तव्यम्_buf(buf, ",");
            }
        }
        अन्यथा {
            ध्रुव संरचना xkb_कुञ्चिका_type * ध्रुव type = कुञ्चिका->groups[0].type;
            write_buf(buf, "\n\t\ttype= ");
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, type->name));
            copy_गन्तव्यम्_buf(buf, ",");
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
    ध्रुव bool explicit_actions = (कुञ्चिका->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #परिभाषय require_explicit(कुञ्चिका, prop,                                       \
                             मानम्, initial_मानम्, default_interpret_मानम्) ( \
        /* Explicit value in the source */                                    \
        ((कुञ्चिका)->explicit & (prop)) ||                                         \
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
                ((कुञ्चिका)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (मानम्) != (initial_मानम्) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_मानम्) != (default_interpret_मानम्)                  \
            )                                                                 \
        )                                                                     \
    )

    यदि (require_explicit(कुञ्चिका, EXPLICIT_REPEAT, कुञ्चिका->repeats,
                         DEFAULT_कुञ्चिका_REPEAT, DEFAULT_INTERPRET_कुञ्चिका_REPEAT)) {
        यदि (कुञ्चिका->repeats)
            copy_गन्तव्यम्_buf(buf, "\n\t\trepeat= Yes,");
        अन्यथा
            copy_गन्तव्यम्_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    यदि (require_explicit(कुञ्चिका, EXPLICIT_VMODMAP, कुञ्चिका->vmodmap,
                         (xkb_mod_आवरणम्_t) DEFAULT_कुञ्चिका_VMODMAP,
                         (xkb_mod_आवरणम्_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  Mod_आवरणम्_Text(keymap->ctx, MOD_BOTH, &keymap->mods,
                              कुञ्चिका->vmodmap));
        simple = false;
    }

    #अपरिभाषय require_explicit

    विकल्प (कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_policy) {
    पक्ष XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_CLAMP:
        copy_गन्तव्यम्_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        भङ्ग;

    पक्ष XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_REDIRECT: {
        यदि (!groups_गणनम्) {
            /* No layout to redirect to: skip */
            भङ्ग;
        }

        xkb_layout_आवरणम्_t layout_आवरणम् =
            (UINT32_C(1) << कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या);
        यदि (!(layouts & layout_आवरणम्)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_सन्देशः_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     कुञ्चिका_Name_Text(keymap->ctx, name),
                     कुञ्चिका->बहिर्गमनम्_सम्बन्धि_range_गणः_सङ्ख्या + 1);
            layout_आवरणम् = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ध्रुव xkb_layout_आवरणम्_t नीचः = (layouts & (layout_आवरणम् - 1));
        ध्रुव xkb_layout_सूचकांक_t new = popcount32(नीचः);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        भङ्ग;
    }

    पक्ष XKB_LAYOUT_बहिर्गमनम्_सम्बन्धि_RANGE_WRAP:
    स्वभाव:
        भङ्ग;
    }

    यदि (कुञ्चिका->overlays) {
        xkb_overlay_आवरणम्_t remaining = कुञ्चिका->overlays;

        ध्रुव xkb_overlay_सूचकांक_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_गणनम् == 8, "invalid right shift");
        ध्रुव xkb_overlay_आवरणम्_t valid =
            (xkb_overlay_आवरणम्_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        यदि (remaining != कुञ्चिका->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_सूचकांक,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, कुञ्चिका_Name_Text(keymap->ctx, name),
                     remaining, कुञ्चिका->overlays & ~valid);
        }

        यदि (remaining && !कुञ्चिका->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ध्रुव xkb_overlay_आवरणम्_t lsb = (xkb_overlay_आवरणम्_t)(
                remaining &
                (xkb_overlay_आवरणम्_t)((अचिह्नित)~remaining + 1u)
            );
            /* get its index */
            ध्रुव xkb_overlay_सूचकांक_t overlay =
                (xkb_overlay_सूचकांक_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     कुञ्चिका_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        यदि (remaining)
            simple = false;

        ध्रुव संरचना xkb_कुञ्चिका * ध्रुव *overlays_कुञ्चिकाः = (कुञ्चिका->overlays_inline)
                                                    ? &कुञ्चिका->overlay_कुञ्चिका
                                                    : कुञ्चिका->overlays_कुञ्चिकाः;
        यावत् (remaining) {
            /* isolate lowest set bit */
            ध्रुव xkb_overlay_आवरणम्_t lsb = (xkb_overlay_आवरणम्_t)(
                remaining &
                (xkb_overlay_आवरणम्_t)((अचिह्नित)~remaining + 1u)
            );
            /* get its index */
            ध्रुव xkb_overlay_सूचकांक_t overlay =
                (xkb_overlay_सूचकांक_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            यदि (कुञ्चिका->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            ध्रुव संरचना xkb_कुञ्चिका *overlay_कुञ्चिका = *(overlays_कुञ्चिकाः++);

            ध्रुव xkb_atom_t overlay_कुञ्चिका_name = (substitutions == रिक्त)
                ? overlay_कुञ्चिका->name
                : substitute_name(substitutions, overlay_कुञ्चिका->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, कुञ्चिका_Name_Text(keymap->ctx, overlay_कुञ्चिका_name));
        }
    }

    यदि (groups_गणनम् > 1 || explicit_actions || explicit)
        simple = false;

    यदि (simple) {
        यदि (groups_गणनम् == 0) {
            /* Remove trailing comma */
            delete_चरमः_char(buf, ',');
            यदि (buf->buf[buf->आकार - 1] == ',')
                buf->आकार--;
        } अन्यथा {
            copy_गन्तव्यम्_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_सूचकांक_t गणः = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            गणः = xkb_keymap_कुञ्चिका_effective_layout(कुञ्चिका, गणः);
            यदि (!write_keysyms(keymap, buf, buf2, कुञ्चिका, गणः, pretty, false))
                प्रत्यागम false;
            copy_गन्तव्यम्_buf(buf, " ]");
        }
        copy_गन्तव्यम्_buf(buf, " };\n");
    }
    अन्यथा {
        xkb_layout_सूचकांक_t new_गणः = (xkb_layout_सूचकांक_t)-1;
        प्रति (xkb_layout_सूचकांक_t गणः = 0; गणः < कुञ्चिका->अङ्कः_groups; गणः++) {
            यदि (!((UINT32_C(1) << गणः) & layouts))
                अनुवर्त;

            new_गणः++;

            ध्रुव bool print_actions =
                /* Group has explicit actions */
                कुञ्चिका->groups[गणः].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (कुञ्चिका->groups[गणः].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (कुञ्चिका->groups[गणः].implicit_actions && !some_interprets);

            यदि (new_गणः != 0)
                copy_गन्तव्यम्_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_गणः + 1);

            यदि (!write_keysyms(keymap, buf, buf2, कुञ्चिका, गणः,
                               pretty, print_actions))
                प्रत्यागम false;
            copy_गन्तव्यम्_buf(buf, " ]");
            यदि (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_गणः + 1);
                यदि (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, कुञ्चिका, गणः))
                    प्रत्यागम false;
                copy_गन्तव्यम्_buf(buf, " ]");
            }
        }
        यदि (!groups_गणनम्)
            delete_चरमः_char(buf, ',');
        copy_गन्तव्यम्_buf(buf, "\n\t};\n");
    }

    प्रत्यागम true;
}


स्थिर पूर्णाङ्क
cmp_modmap_चरणः_1(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    ध्रुव Mod_Map_प्रविष्टिः *m1 = (ध्रुव Mod_Map_प्रविष्टिः *)a;
    ध्रुव Mod_Map_प्रविष्टिः *m2 = (ध्रुव Mod_Map_प्रविष्टिः *)b;

    यदि (!m1->mods) {
        यदि (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            प्रत्यागम 0;
        }
        प्रत्यागम +1;
    } अन्यथा यदि (!m2->mods) {
        प्रत्यागम -1;
    }

    /* Valid only on step 1 */
    यदि (m1->कुञ्चिका_संहिता < m2->कुञ्चिका_संहिता)
        प्रत्यागम -1;
    अन्यथा यदि (m1->कुञ्चिका_संहिता > m2->कुञ्चिका_संहिता)
        प्रत्यागम +1;

    यदि (m1->mods < m2->mods)
        प्रत्यागम -1;
    अन्यथा यदि (m1->mods > m2->mods)
        प्रत्यागम +1;

    यदि (m1->have_चिह्नम् < m2->have_चिह्नम्)
        प्रत्यागम +1;
    अन्यथा यदि (m1->have_चिह्नम् > m2->have_चिह्नम्)
        प्रत्यागम -1;

    यदि (m1->have_चिह्नम्) {
        यदि (m1->u.कुञ्चिका_Sym < m2->u.कुञ्चिका_Sym)
            प्रत्यागम -1;
        यदि (m1->u.कुञ्चिका_Sym > m2->u.कुञ्चिका_Sym)
            प्रत्यागम +1;
    } अन्यथा {
        यदि (m1->u.कुञ्चिका_Name < m2->u.कुञ्चिका_Name)
            प्रत्यागम -1;
        यदि (m1->u.कुञ्चिका_Name > m2->u.कुञ्चिका_Name)
            प्रत्यागम +1;
    }

    प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
cmp_modmap_चरणः_2(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
    ध्रुव Mod_Map_प्रविष्टिः *m1 = (ध्रुव Mod_Map_प्रविष्टिः *)a;
    ध्रुव Mod_Map_प्रविष्टिः *m2 = (ध्रुव Mod_Map_प्रविष्टिः *)b;

    यदि (m1->mods < m2->mods)
        प्रत्यागम -1;
    यदि (m1->mods > m2->mods)
        प्रत्यागम +1;

    यदि (m1->have_चिह्नम् < m2->have_चिह्नम्)
        प्रत्यागम -1;
    यदि (m1->have_चिह्नम् > m2->have_चिह्नम्)
        प्रत्यागम +1;

    यदि (m1->have_चिह्नम्) {
        यदि (m1->u.कुञ्चिका_Sym < m2->u.कुञ्चिका_Sym)
            प्रत्यागम -1;
        यदि (m1->u.कुञ्चिका_Sym > m2->u.कुञ्चिका_Sym)
            प्रत्यागम +1;
    } अन्यथा {
        यदि (m1->कुञ्चिका_संहिता < m2->कुञ्चिका_संहिता)
            प्रत्यागम -1;
        यदि (m1->कुञ्चिका_संहिता > m2->कुञ्चिका_संहिता)
            प्रत्यागम +1;
    }

    प्रत्यागम 0;
}

स्थिर bool
write_modmaps(ध्रुव संरचना xkb_keymap *keymap, गणना xkb_keymap_format format,
              ध्रुव कुञ्चिका_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, संरचना buf *buf)
{
    यदि (!keymap->अङ्कः_modmaps)
        प्रत्यागम true;

    darray(Mod_Map_प्रविष्टिः) modmaps = darray_new();
    darray_from_वस्तूनि(modmaps, keymap->modmaps, keymap->अङ्कः_modmaps);
    यदि (!darray_वस्तूनि(modmaps))
        प्रत्यागम false;

    /* Sort entries by key code */
    शीघ्रं_क्रमय(darray_वस्तूनि(modmaps), darray_आकार(modmaps),
          परिमाण(*darray_वस्तूनि(modmaps)), &cmp_modmap_चरणः_1);

    /* Drop disabled entries at the end */
    प्रति (darray_आकार_t m = darray_आकार(modmaps); m-- > 0;) {
        यदि (darray_item(modmaps, m).mods)
            भङ्ग;
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
    bool convert_गन्तव्यम्_v1_compatibility_प्रविष्टिः = false;
    प्रति (darray_आकार_t m = darray_आकार(modmaps); m-- > 0;) {
        Mod_Map_प्रविष्टिः *mm = &darray_item(modmaps, m);
        प्रतिज्ञा(mm->mods);

        यदि (mm->कुञ्चिका_संहिता == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ध्रुव xkb_mod_आवरणम्_t core_mods = (mm->mods & MOD_REAL_आवरणम्_ALL);
            यदि (convert_गन्तव्यम्_v1_compatibility_प्रविष्टिः && core_mods) {
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
            अनुवर्त;
        }

        current_keycode = mm->कुञ्चिका_संहिता;
        ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका = Xkb_कुञ्चिका(keymap, mm->कुञ्चिका_संहिता);

        ध्रुव xkb_mod_आवरणम्_t core_mods = (कुञ्चिका->modmap & MOD_REAL_आवरणम्_ALL);
        यदि (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((कुञ्चिका->modmap & (कुञ्चिका->modmap - 1)) || कुञ्चिका->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_गन्तव्यम्_v1_compatibility_प्रविष्टिः = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_आवरणम्,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    कुञ्चिका_Name_Text(keymap->ctx, कुञ्चिका->name), कुञ्चिका->modmap);
        } अन्यथा {
            /* V2 format or V1 format with a single core modifier */
            प्रतिज्ञा(कुञ्चिका->modmap);
            mm->mods = कुञ्चिका->modmap;
            /* Force key name */
            mm->have_चिह्नम् = false;
            mm->u.कुञ्चिका_Name = कुञ्चिका->name;
            convert_गन्तव्यम्_v1_compatibility_प्रविष्टिः = false;
        }
    }

    यदि (darray_रिक्तानि(modmaps))
        गच्छ निर्गच्छ;

    /* Sort entries by modifier mask */
    शीघ्रं_क्रमय(darray_वस्तूनि(modmaps), darray_आकार(modmaps),
          परिमाण(*darray_वस्तूनि(modmaps)), &cmp_modmap_चरणः_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_आवरणम्_t current_mods = 0;
    ध्रुव Mod_Map_प्रविष्टिः *प्रविष्टिः;
    darray_foreach(प्रविष्टिः, modmaps) {
        यदि (!प्रविष्टिः->mods)
            अनुवर्त;

        यदि (current_mods != प्रविष्टिः->mods) {
            यदि (current_mods) {
                copy_गन्तव्यम्_buf(buf, " };\n");
            }
            current_mods = प्रविष्टिः->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      Mod_आवरणम्_Text(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } अन्यथा {
            copy_गन्तव्यम्_buf(buf, ", ");
        }

        यदि (प्रविष्टिः->have_चिह्नम्) {
            यदि (pretty || प्रविष्टिः->u.कुञ्चिका_Sym == XKB_कुञ्चिका_No_चिह्नम्)
                write_buf(buf, "%s", KeysymText(keymap->ctx, प्रविष्टिः->u.कुञ्चिका_Sym));
            अन्यथा
                write_buf(buf, "0x%"PRIx32, प्रविष्टिः->u.कुञ्चिका_Sym);
        } अन्यथा {
            ध्रुव xkb_atom_t name = (substitutions == रिक्त)
                ? प्रविष्टिः->u.कुञ्चिका_Name
                : substitute_name(substitutions, प्रविष्टिः->u.कुञ्चिका_Name);
            write_buf(buf, "%s", कुञ्चिका_Name_Text(keymap->ctx, name));
        }
    }
    यदि (current_mods) {
        copy_गन्तव्यम्_buf(buf, " };\n");
    }

निर्गच्छ:
    darray_free(modmaps);
    प्रत्यागम true;
}

स्थिर bool
write_symbols(ध्रुव संरचना xkb_keymap *keymap,
              ध्रुव संरचना xkb_keymap_serialize_config *config,
              ध्रुव कुञ्चिका_name_substitutions *substitutions,
              xkb_layout_सूचकांक_t max_groups, bool some_interp,
              संरचना buf *buf)
{
    ध्रुव bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ध्रुव bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_मानानि);
    ध्रुव bool explicit_कुञ्चिका_मानानि =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_कुञ्चिका_मानानि);
    ध्रुव bool drop_अप्रयुक्तम् = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_अप्रयुक्तम्);
    ध्रुव bool drop_interprets = (explicit_कुञ्चिका_मानानि && drop_अप्रयुक्तम्);
    /* TODO: print all default values if explicit flag is set */

    यदि (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    अन्यथा
        copy_गन्तव्यम्_buf(buf, "xkb_symbols {\n");

    bool has_गणः_नामानि = false;
    xkb_layout_सूचकांक_t new_गणः = (xkb_layout_सूचकांक_t)-1;
    प्रति (xkb_layout_सूचकांक_t गणः = 0; गणः < keymap->अङ्कः_गणः_नामानि; गणः++) {
        यदि (!((UINT32_C(1) << गणः) & config->layouts))
            अनुवर्त;

        new_गणः++;

        यदि (keymap->गणः_नामानि[गणः]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_गणः + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->गणः_नामानि[गणः]));
            copy_गन्तव्यम्_buf(buf, ";\n");
            has_गणः_नामानि = true;
        }
    }
    यदि (has_गणः_नामानि)
        copy_गन्तव्यम्_buf(buf, "\n");

    यदि (defaults && !write_actions_defaults(keymap, config->format, buf))
        प्रत्यागम false;

    संरचना buf buf2 = { रिक्त, 0, 0 };
    ध्रुव संरचना xkb_कुञ्चिका *कुञ्चिका;
    xkb_कुञ्चिकाः_foreach(कुञ्चिका, keymap) {
        /* Skip keys with no explicit values */
        यदि (कुञ्चिका->explicit &&
            !write_कुञ्चिका(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_कुञ्चिका_मानानि, pretty, buf,
                       &buf2, कुञ्चिका))
        {
            मुक्त(buf2.buf);
            प्रत्यागम false;
        }
    }
    मुक्त(buf2.buf);

    यदि (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        प्रत्यागम false;

    copy_गन्तव्यम्_buf(buf, "};\n\n");
    प्रत्यागम true;
}

स्थिर गणना xkb_स्थितिः
write_keymap(ध्रुव संरचना xkb_keymap *keymap,
             ध्रुव संरचना xkb_keymap_serialize_config *config,
             संरचना buf *buf, संरचना xkb_keymap_serialize_परिणाम *परिणाम)
{
    कुञ्चिका_name_substitutions substitutions = darray_new();
    यदि (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_कुञ्चिकाः(keymap, &substitutions))
    {
        प्रत्यागम false;
    }

    ध्रुव कुञ्चिका_name_substitutions * ध्रुव substitutions_ptr =
        (darray_रिक्तानि(substitutions)) ? रिक्त : &substitutions;

    ध्रुव xkb_layout_सूचकांक_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    ध्रुव bool ok = (
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
    प्रत्यागम ok ? XKB_SUCCESS : -1;
}

गणना xkb_स्थितिः
text_v1_keymap_serialize(
        ध्रुव संरचना xkb_keymap *keymap,
        ध्रुव संरचना xkb_keymap_serialize_config *config,
        संरचना xkb_keymap_serialize_परिणाम *परिणाम
)
{
    संरचना buf buf = { रिक्त, 0, 0 };

    ध्रुव गणना xkb_स्थितिः स्थितिः =
        write_keymap(keymap, config, &buf, परिणाम);
    यदि (स्थितिः != XKB_SUCCESS) {
        मुक्त(buf.buf);
        परिणाम->serialized = रिक्त;
        प्रत्यागम स्थितिः;
    }

    परिणाम->serialized = buf.buf;
    परिणाम->दीर्घता = buf.आकार + 1;
    परिणाम->layouts = config->layouts;

    प्रत्यागम XKB_SUCCESS;
}
