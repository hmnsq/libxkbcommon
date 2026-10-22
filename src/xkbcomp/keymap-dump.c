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

#അന്തര്ഭാവ "config.h"

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>

#അന്തര്ഭാവ "xkbcommon/xkbcommon-status.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon-keysyms.h"
#അന്തര്ഭാവ "xkbcommon/xkbcommon.h"
#അന്തര്ഭാവ "atom.h"
#അന്തര്ഭാവ "action.h"
#അന്തര്ഭാവ "context.h"
#അന്തര്ഭാവ "darray.h"
#അന്തര്ഭാവ "keymap.h"
#അന്തര്ഭാവ "messages-codes.h"
#അന്തര്ഭാവ "text.h"
#അന്തര്ഭാവ "util-numbers.h"
#അന്തര്ഭാവ "xkbcomp-priv.h"

#define BUF_CHUNK_SIZE 4096

ഘടന buf {
    അക്ഷരം *buf;
    size_t size;
    size_t alloc;
};

#define xkb_abs(n) _Generic((n),                \
                            പൂർണ്ണസംഖ്യ: abs,           \
                            long പൂർണ്ണസംഖ്യ: labs,     \
                            സ്ഥിരസ്ഥിതി: llabs )((n))

സ്ഥിതം bool
do_realloc(ഘടന buf *buf, size_t at_least)
{
    buf->alloc += BUF_CHUNK_SIZE;
    എങ്കിൽ (at_least >= BUF_CHUNK_SIZE)
        buf->alloc += at_least;

    അക്ഷരം *സ്ഥിരം new = realloc(buf->buf, buf->alloc);
    എങ്കിൽ (!new)
        തിരികെ false;

    buf->buf = new;
    തിരികെ true;
}

ATTR_PRINTF(2, 3) സ്ഥിതം bool
check_write_buf(ഘടന buf *buf, സ്ഥിരം അക്ഷരം *fmt, ...)
{
    va_list args;
    പൂർണ്ണസംഖ്യ printed;
    size_t available;

    available = buf->alloc - buf->size;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    പ്രതിജ്ഞാ(buf->buf || available == 0);
    va_start(args, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->size : buf->buf, available,
                        fmt, args);
    va_end(args);

    എങ്കിൽ (printed < 0) {
        /* Some error */
        പോകുക err;
    } അല്ലെങ്കിൽ എങ്കിൽ ((size_t)printed >= available) {
        /* Not enough space: need to realloc */
        എങ്കിൽ (!do_realloc(buf, (size_t)printed + 1))
            പോകുക err;

        /* The buffer has enough space now. */
        available = buf->alloc - buf->size;
        പ്രതിജ്ഞാ(buf->buf && (size_t) printed < available);
        va_start(args, fmt);
        printed = vsnprintf(buf->buf + buf->size, available, fmt, args);
        va_end(args);

        എങ്കിൽ (printed < 0 || (size_t) printed >= available)
            പോകുക err;
    }

    buf->size += (size_t)printed;
    തിരികെ true;

err:
    സ്വതന്ത്രം(buf->buf);
    buf->buf = ശൂന്യ;
    തിരികെ false;
}

#define write_buf(buf, ...) ചെയ്യുക { \
    എങ്കിൽ (!check_write_buf(buf, __VA_ARGS__)) \
        തിരികെ false; \
} ആകുമ്പോൾ (0)

സ്ഥിതം bool
check_copy_to_buf(ഘടന buf *buf, സ്ഥിരം അക്ഷരം* source, size_t len)
{
    എങ്കിൽ (len == 0)
        തിരികെ true;

    സ്ഥിരം size_t available = buf->alloc - buf->size;
    എങ്കിൽ (len >= available &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        സ്വതന്ത്രം(buf->buf);
        buf->buf = ശൂന്യ;
        തിരികെ false;
    }

    memcpy(buf->buf + buf->size, source, len);
    buf->size += len;
    /* Append NULL byte */
    buf->buf[buf->size] = '\0';
    തിരികെ true;
}

#define copy_to_buf_len(buf, source, len) ചെയ്യുക { \
    എങ്കിൽ (!check_copy_to_buf(buf, source, len)) \
        തിരികെ false; \
} ആകുമ്പോൾ (0)

#define copy_to_buf(buf, source) \
    copy_to_buf_len(buf, source, വലുപ്പം(source) - 1)

സ്ഥിതം bool
check_write_string_literal(ഘടന buf *buf, സ്ഥിരം അക്ഷരം* string)
{
    copy_to_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    size_t pending_start = 0;
    size_t current = 0;
    അക്ഷരം escape_buffer[] = {'\\', '0', '0', '0', '0'};
    വേണ്ടി (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        തിരഞ്ഞെടുപ്പ് (string[current]) {
        അവസ്ഥ '\n':
            /* `\n` would break strings */
            escape_buffer[1] = 'n';
            escape_len = 2;
            നിർത്തുക;
        അവസ്ഥ '\\':
            /* `\` would create escape sequences */
            escape_buffer[1] = '\\';
            escape_len = 2;
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
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
            എങ്കിൽ (unlikely((unsigned അക്ഷരം)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_buffer[1] = '0';
                escape_buffer[2] = '0';
                escape_buffer[3] = (അക്ഷരം)('0' + (string[current] >> 3));
                escape_buffer[4] = (അക്ഷരം)('0' + (string[current] & 0x7));
                escape_len = 5;
            } അല്ലെങ്കിൽ {
                /* Expand the chunk */
                തുടരുക;
            }
        }
        /* Write pending chunk */
        പ്രതിജ്ഞാ(current >= pending_start);
        copy_to_buf_len(buf, string + pending_start, current - pending_start);
        /* Write escape sequence */
        copy_to_buf_len(buf, escape_buffer, escape_len);
        pending_start = current + 1;
    }
    /* Write remaining chunk */
    പ്രതിജ്ഞാ(current >= pending_start);
    copy_to_buf_len(buf, string + pending_start, current - pending_start);

    copy_to_buf(buf, "\"");
    തിരികെ true;
}

#define write_buf_string_literal(buf, string) ചെയ്യുക { \
    എങ്കിൽ (!check_write_string_literal(buf, string)) \
        തിരികെ false; \
} ആകുമ്പോൾ (0)

സ്ഥിതം ശൂന്യം
delete_last_char(ഘടന buf *buf, അക്ഷരം c)
{
    എങ്കിൽ (buf->size && buf->buf[buf->size - 1] == c)
        buf->buf[--buf->size] = '\0';
}

/** Entry for a name substituion LUT */
ഘടന താക്കോൽ_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_താക്കോൽ_name_substitution(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
    സ്ഥിരം xkb_atom_t s1 = ((ഘടന താക്കോൽ_name_substitution *) a)->source;
    സ്ഥിരം xkb_atom_t s2 = ((ഘടന താക്കോൽ_name_substitution *) b)->source;
    തിരികെ (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

തരനിർവചനം darray(ഘടന താക്കോൽ_name_substitution) താക്കോൽ_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
സ്ഥിതം bool
rename_long_താക്കോലുകൾ(സ്ഥിരം ഘടന xkb_keymap *keymap,
                 താക്കോൽ_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    എണ്ണൽ {
        first = '!',
        last = '0',
        invalid = '\0'
    };
    അക്ഷരം prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', last
    };
    static_assert(വലുപ്പം(prefixes) == last - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ;
    xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap) {
        എങ്കിൽ (താക്കോൽ->name == XKB_ATOM_NONE)
            തുടരുക;

        സ്ഥിരം അക്ഷരം * സ്ഥിരം name = xkb_atom_text(keymap->ctx, താക്കോൽ->name);
        സ്ഥിരം size_t len = strlen_safe(name);
        എങ്കിൽ (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (ഘടന താക്കോൽ_name_substitution) {
                .source = താക്കോൽ->name,
                .keycode = താക്കോൽ->keycode,
                .target = താക്കോൽ->name
            });
        } അല്ലെങ്കിൽ എങ്കിൽ (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    വേണ്ടി (darray_size_t a = 0; a < keymap->അക്കം_താക്കോൽ_aliases; a++) {
        സ്ഥിരം ഘടന xkb_താക്കോൽ_alias * സ്ഥിരം alias = &keymap->താക്കോൽ_aliases[a];
        സ്ഥിരം അക്ഷരം * സ്ഥിരം name = xkb_atom_text(keymap->ctx, alias->alias);
        സ്ഥിരം size_t len = strlen_safe(name);
        എങ്കിൽ (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (ഘടന താക്കോൽ_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } അല്ലെങ്കിൽ എങ്കിൽ (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    എങ്കിൽ darray_empty(*substitutions) {
        /* Nothing to do */
        തിരികെ true;
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
    അക്ഷരം *prefix = prefixes;
    സ്ഥിരം അക്ഷരം *prefix_max = prefixes + വലുപ്പം(prefixes) - 1;
    ആകുമ്പോൾ (*prefix == invalid && prefix < prefix_max)
        prefix++;

    /* Keycode prefix: prefer ‘0’ if available */
    അക്ഷരം keycode_prefix;
    എങ്കിൽ (prefixes[last - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_prefix = last;
        prefixes[last - first] = invalid;
    } അല്ലെങ്കിൽ {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_prefix = *prefix;
        എങ്കിൽ (unlikely(keycode_prefix == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            തിരികെ true;
        }
        /* Update current prefix */
        *prefix = invalid;
        ആകുമ്പോൾ (*prefix == invalid && prefix < prefix_max)
            prefix++;
    }

    /* Proceed renaming */
    uint16_t next_id = 1;
    ഘടന താക്കോൽ_name_substitution *sub;
    അക്ഷരം buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        എങ്കിൽ (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, വലുപ്പം(buf), "%c%03"PRIx32,
                     keycode_prefix, sub->keycode);
            സ്ഥിരം xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, വലുപ്പം(buf) - 2);
            എങ്കിൽ (target == XKB_ATOM_NONE)
                പോകുക error;
            sub->target = target;
        } അല്ലെങ്കിൽ എങ്കിൽ (*prefix != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, വലുപ്പം(buf), "%c%03"PRIx16, *prefix, next_id);
            സ്ഥിരം xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, വലുപ്പം(buf) - 2);
            എങ്കിൽ (target == XKB_ATOM_NONE)
                പോകുക error;
            sub->target = target;
            എങ്കിൽ (++next_id > 0xfff) {
                *prefix = invalid;
                /* Get next available prefix */
                ആകുമ്പോൾ (*prefix == invalid && prefix < prefix_max)
                    prefix++;
                /* Reset counter */
                next_id = 1;
            }
        } അല്ലെങ്കിൽ {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->target = sub->source;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_ഇനങ്ങൾ(*substitutions), darray_size(*substitutions),
          വലുപ്പം(*darray_ഇനങ്ങൾ(*substitutions)), &cmp_താക്കോൽ_name_substitution);

    തിരികെ true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate key name substitution\n");
    തിരികെ false;
}

/** Substitute a key name with a valid X11 key name */
സ്ഥിതം xkb_atom_t
substitute_name(സ്ഥിരം താക്കോൽ_name_substitutions *substitutions, xkb_atom_t name)
{
    /*ദ്വിമാന തിരയൽ*/
    darray_size_t first = 0;
    darray_size_t last = darray_size(*substitutions);

    ആകുമ്പോൾ (last > first) {
        darray_size_t mid = first + (last - 1 - first) / 2;
        സ്ഥിരം xkb_atom_t source = darray_item(*substitutions, mid).source;
        എങ്കിൽ (source < name)
            first = mid + 1;
        അല്ലെങ്കിൽ എങ്കിൽ (source > name)
            last = mid;
        അല്ലെങ്കിൽ /* found it */
            തിരികെ darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    തിരികെ name;
}

സ്ഥിതം bool
write_vmods(സ്ഥിരം ഘടന xkb_keymap *keymap, എണ്ണൽ xkb_keymap_format format,
            bool explicit, ഘടന buf *buf)
{
    സ്ഥിരം ഘടന xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        എങ്കിൽ (!has_some) {
            copy_to_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } അല്ലെങ്കിൽ {
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
        സ്ഥിരം bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        എങ്കിൽ ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
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

    എങ്കിൽ (has_some)
        copy_to_buf(buf, ";\n\n");

    തിരികെ true;
}

സ്ഥിതം bool
write_keycodes(സ്ഥിരം ഘടന xkb_keymap *keymap,
               സ്ഥിരം താക്കോൽ_name_substitutions *substitutions,
               എണ്ണൽ xkb_keymap_serialize_flags flags, ഘടന buf *buf)
{
    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ;
    xkb_led_index_t idx;
    സ്ഥിരം ഘടന xkb_led *led;

    സ്ഥിരം bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    എങ്കിൽ (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    അല്ലെങ്കിൽ
        copy_to_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_താക്കോൽ_code, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_താക്കോൽ_code, 255));

    xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap) {
        എങ്കിൽ (താക്കോൽ->name == XKB_ATOM_NONE)
            തുടരുക;

        സ്ഥിരം xkb_atom_t name = (substitutions == ശൂന്യ)
            ? താക്കോൽ->name
            : substitute_name(substitutions, താക്കോൽ->name);
        എങ്കിൽ (pretty)
            write_buf(buf, "\t%-20s", താക്കോൽ_Name_Text(keymap->ctx, name));
        അല്ലെങ്കിൽ
            write_buf(buf, "\t%s", താക്കോൽ_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", താക്കോൽ->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        എങ്കിൽ (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_to_buf(buf, ";\n");
        }

    വേണ്ടി (darray_size_t i = 0; i < keymap->അക്കം_താക്കോൽ_aliases; i++) {
        സ്ഥിരം xkb_atom_t alias = (substitutions == ശൂന്യ)
            ? keymap->താക്കോൽ_aliases[i].alias
            : substitute_name(substitutions, keymap->താക്കോൽ_aliases[i].alias);
        സ്ഥിരം xkb_atom_t real = (substitutions == ശൂന്യ)
            ? keymap->താക്കോൽ_aliases[i].real
            : substitute_name(substitutions, keymap->താക്കോൽ_aliases[i].real);

        എങ്കിൽ (pretty)
            write_buf(buf, "\talias %-14s",
                      താക്കോൽ_Name_Text(keymap->ctx, alias));
        അല്ലെങ്കിൽ
            write_buf(buf, "\talias %s",
                      താക്കോൽ_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  താക്കോൽ_Name_Text(keymap->ctx, real));
    }

    copy_to_buf(buf, "};\n\n");
    തിരികെ true;
}

സ്ഥിതം bool
write_types(സ്ഥിരം ഘടന xkb_keymap *keymap, എണ്ണൽ xkb_keymap_format format,
            എണ്ണൽ xkb_keymap_serialize_flags flags, ഘടന buf *buf)
{
    എങ്കിൽ (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    അല്ലെങ്കിൽ
        copy_to_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    എങ്കിൽ (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        തിരികെ false;

    സ്ഥിരം bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    വേണ്ടി (darray_size_t i = 0; i < keymap->അക്കം_types; i++) {
        സ്ഥിരം ഘടന xkb_താക്കോൽ_type * സ്ഥിരം type = &keymap->types[i];
        എങ്കിൽ (!type->required && drop_unused)
            തുടരുക;

        copy_to_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_to_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        വേണ്ടി (darray_size_t j = 0; j < type->അക്കം_entries; j++) {
            സ്ഥിരം അക്ഷരം *str;
            സ്ഥിരം ഘടന xkb_താക്കോൽ_type_entry *entry = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            എങ്കിൽ (entry->level == 0 && entry->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_മൂല്യങ്ങൾ))
                തുടരുക;

            str = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              entry->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, entry->level + 1);

            എങ്കിൽ (entry->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           entry->preserve.mods));
        }

        വേണ്ടി (xkb_level_index_t n = 0; n < type->അക്കം_level_names; n++)
            എങ്കിൽ (type->level_names[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_names[n]));
                copy_to_buf(buf, ";\n");
            }

        copy_to_buf(buf, "\t};\n");
    }

    copy_to_buf(buf, "};\n\n");
    തിരികെ true;
}

സ്ഥിതം bool
write_led_map(സ്ഥിരം ഘടന xkb_keymap *keymap, എണ്ണൽ xkb_keymap_format format,
              bool defaults, ഘടന buf *buf, സ്ഥിരം ഘടന xkb_led *led)
{
    copy_to_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_to_buf(buf, " {\n");

    എങ്കിൽ (led->which_groups) {
        എങ്കിൽ (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      LedStateMaskText(keymap->ctx, groupComponentMaskNames,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    എങ്കിൽ (led->which_mods) {
        എങ്കിൽ (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      LedStateMaskText(keymap->ctx, modComponentMaskNames,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    എങ്കിൽ (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  ControlMaskText(keymap->ctx, format, led->ctrls));
    }

    copy_to_buf(buf, "\t};\n");
    തിരികെ true;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
affect_lock_text(എണ്ണൽ xkb_action_flags flags, bool show_both)
{
    തിരഞ്ഞെടുപ്പ് (flags & (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK)) {
    അവസ്ഥ 0:
        തിരികെ show_both ? ",affect=both" : "";
    അവസ്ഥ ACTION_LOCK_NO_UNLOCK:
        തിരികെ ",affect=lock";
    അവസ്ഥ ACTION_LOCK_NO_LOCK:
        തിരികെ ",affect=unlock";
    അവസ്ഥ ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK:
        തിരികെ ",affect=neither";
    സ്ഥിരസ്ഥിതി:
        തിരികെ "";
    }
}

#define ചിഹ്നം_PADDING 15
#define ACTION_PADDING 30

സ്ഥിതം bool
write_action(സ്ഥിരം ഘടന xkb_keymap *keymap, എണ്ണൽ xkb_keymap_format format,
             xkb_layout_index_t max_groups, ഘടന buf *buf,
             സ്ഥിരം സംഘം xkb_action *action,
             സ്ഥിരം അക്ഷരം *prefix, സ്ഥിരം അക്ഷരം *suffix)
{
    സ്ഥിരം അക്ഷരം *type;
    സ്ഥിരം അക്ഷരം *args = ശൂന്യ;

    എങ്കിൽ (!prefix)
        prefix = "";
    എങ്കിൽ (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    തിരഞ്ഞെടുപ്പ് (action->type) {
    അവസ്ഥ ACTION_TYPE_MOD_SET:
    അവസ്ഥ ACTION_TYPE_MOD_LATCH:
    അവസ്ഥ ACTION_TYPE_MOD_LOCK:
        എങ്കിൽ (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            args = "modMapMods";
        അല്ലെങ്കിൽ
            args = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        എങ്കിൽ (unlockOnPress && !isModsUnLockOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->group.flags & ACTION_LATCH_ON_PRESS);
        എങ്കിൽ (latchOnPress && !isModsLatchOnPressSupported(format)) {
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
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_GROUP_SET:
    അവസ്ഥ ACTION_TYPE_GROUP_LATCH:
    അവസ്ഥ ACTION_TYPE_GROUP_LOCK:
        എങ്കിൽ ((uint32_t) xkb_abs(action->group.group) <
            (max_groups + !(action->group.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool lockOnRelease = action->type == ACTION_TYPE_GROUP_LOCK &&
                                 (action->group.flags & ACTION_LOCK_ON_RELEASE);
            എങ്കിൽ (lockOnRelease && !isGroupLockOnReleaseSupported(format)) {
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
        } അല്ലെങ്കിൽ {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->group.group +
                       !!(action->group.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            പോകുക void_action;
        }
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", prefix, type, suffix);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", prefix, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_PTR_LOCK:
        args = affect_lock_text(action->btn.flags, true);
        /* fallthrough */
    അവസ്ഥ ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", prefix, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        എങ്കിൽ (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        അല്ലെങ്കിൽ
            copy_to_buf(buf, "default");
        എങ്കിൽ (action->btn.എണ്ണം) {
            പ്രതിജ്ഞാ(action->type != ACTION_TYPE_PTR_LOCK);
            write_buf(buf, ",count=%u", action->btn.എണ്ണം);
        }
        എങ്കിൽ (args)
            write_buf(buf, "%s", args);
        write_buf(buf, ")%s", suffix);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", prefix, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.മൂല്യം >= 0) ? "+" : "",
                  action->dflt.മൂല്യം);
        write_buf(buf, ")%s", suffix);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", prefix, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_CTRL_SET:
    അവസ്ഥ ACTION_TYPE_CTRL_LOCK:
        write_buf(buf, "%s%s(controls=%s%s)%s", prefix, type,
                  ControlMaskText(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_LOCK
                    ? affect_lock_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_REDIRECT_താക്കോൽ: {
        write_buf(buf, "%s%s(", prefix, type);
        സ്ഥിരം ഘടന xkb_താക്കോൽ * സ്ഥിരം താക്കോൽ = Xkb_താക്കോൽ(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        എങ്കിൽ (താക്കോൽ)
            write_buf(buf, "keycode=%s", താക്കോൽ_Name_Text(keymap->ctx, താക്കോൽ->name));
        എങ്കിൽ (action->redirect.affect_mods) {
            xkb_mod_mask_t mask;
            mask = (action->redirect.affect_mods & action->redirect.mods);
            എങ്കിൽ (mask)
                write_buf(buf, ",modifiers=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
            mask = (action->redirect.affect_mods & ~action->redirect.mods);
            എങ്കിൽ (mask)
                write_buf(buf, ",clearMods=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
        }
        write_buf(buf, ")%s", suffix);
        നിർത്തുക;
    }

    അവസ്ഥ ACTION_TYPE_UNKNOWN:
    അവസ്ഥ ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", prefix, suffix);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        എങ്കിൽ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      prefix, suffix);
        അല്ലെങ്കിൽ
            write_buf(buf, "%sVoidAction()%s", prefix, suffix);
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_അക്കം_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        പ്രതിജ്ഞാ(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  prefix, type, action->type, action->priv.data[0],
                  action->priv.data[1], action->priv.data[2],
                  action->priv.data[3], action->priv.data[4],
                  action->priv.data[5], action->priv.data[6],
                  suffix);
        നിർത്തുക;
    }

    തിരികെ true;
}

സ്ഥിതം സ്ഥിരം സംഘം xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

സ്ഥിതം bool
write_actions(സ്ഥിരം ഘടന xkb_keymap *keymap, എണ്ണൽ xkb_keymap_format format,
              xkb_layout_index_t max_groups, ഘടന buf *buf, ഘടന buf *buf2,
              സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ, xkb_layout_index_t group)
{
    സ്ഥിതം സ്ഥിരം സംഘം xkb_action noAction = { .type = ACTION_TYPE_NONE };

    വേണ്ടി (xkb_level_index_t level = 0; level < Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, group);
         level++) {

        എങ്കിൽ (level != 0)
            copy_to_buf(buf, ", ");

        സ്ഥിരം സംഘം xkb_action *actions = ശൂന്യ;
        xkb_action_എണ്ണം_t എണ്ണം = xkb_keymap_താക്കോൽ_get_actions_by_level(
            keymap, താക്കോൽ, group, level, &actions
        );

        എങ്കിൽ (എണ്ണം > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            എണ്ണം = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->size = 0;
        എങ്കിൽ (എണ്ണം == 0) {
            എങ്കിൽ (!write_action(keymap, format, max_groups,
                              buf2, &noAction, ശൂന്യ, ശൂന്യ))
                തിരികെ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        അല്ലെങ്കിൽ എങ്കിൽ (എണ്ണം == 1) {
            എങ്കിൽ (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), ശൂന്യ, ശൂന്യ))
                തിരികെ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        അല്ലെങ്കിൽ {
            copy_to_buf(buf2, "{ ");
            വേണ്ടി (xkb_action_എണ്ണം_t k = 0; k < എണ്ണം; k++) {
                എങ്കിൽ (k != 0)
                    copy_to_buf(buf2, ", ");
                size_t old_size = buf2->size;
                എങ്കിൽ (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ശൂന്യ, ശൂന്യ))
                    തിരികെ false;
                /* Check if padding is necessary */
                എങ്കിൽ (buf2->size >= old_size + ACTION_PADDING)
                    തുടരുക;
                /* Compute and write padding, then write the action again */
                സ്ഥിരം പൂർണ്ണസംഖ്യ padding = (പൂർണ്ണസംഖ്യ)(old_size + ACTION_PADDING - buf2->size);
                buf2->size = old_size;
                write_buf(buf2, "%*s", padding, "");
                എങ്കിൽ (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ശൂന്യ, ശൂന്യ))
                    തിരികെ false;
            }
            copy_to_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    തിരികെ true;
}

സ്ഥിതം bool
write_action_defaults(സ്ഥിരം ഘടന xkb_keymap *keymap,
                      എണ്ണൽ xkb_keymap_format format,
                      സ്ഥിരം സംഘം xkb_action *action,
                      ഘടന buf *buf)
{
    സ്ഥിരം അക്ഷരം *type = ActionTypeText(action->type);

#define PREFIX "\t"

    തിരഞ്ഞെടുപ്പ് (action->type) {
    അവസ്ഥ ACTION_TYPE_MOD_SET:
    അവസ്ഥ ACTION_TYPE_MOD_LATCH:
    അവസ്ഥ ACTION_TYPE_MOD_LOCK:
        പ്രതിജ്ഞാ(action->mods.flags == 0);
        പ്രതിജ്ഞാ(action->mods.mods.mods == 0);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        എങ്കിൽ (action->type != ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        എങ്കിൽ (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, PREFIX"%s.latchOnPress = false;\n", type);
        എങ്കിൽ (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        എങ്കിൽ (action->type == ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        എങ്കിൽ (isModsUnLockOnPressSupported(format))
            write_buf(buf, PREFIX"%s.unlockOnPress = false;\n", type);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_GROUP_SET:
    അവസ്ഥ ACTION_TYPE_GROUP_LATCH:
    അവസ്ഥ ACTION_TYPE_GROUP_LOCK:
        പ്രതിജ്ഞാ(action->group.flags == 0);
        പ്രതിജ്ഞാ(action->group.group == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.group = +0;\n", type);
        എങ്കിൽ (action->type != ACTION_TYPE_GROUP_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        എങ്കിൽ (action->type == ACTION_TYPE_GROUP_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        എങ്കിൽ (action->type == ACTION_TYPE_GROUP_LOCK &&
            isGroupLockOnReleaseSupported(format))
            write_buf(buf, PREFIX"%s.lockOnRelease = false;\n", type);
        നിർത്തുക;


    അവസ്ഥ ACTION_TYPE_PTR_MOVE:
        പ്രതിജ്ഞാ(action->ptr.x == 0);
        പ്രതിജ്ഞാ(action->ptr.y == 0);
        പ്രതിജ്ഞാ(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.x = +0;\n", type);
        write_buf(buf, PREFIX"%s.y = +0;\n", type);
        write_buf(buf, PREFIX"%s.accel = true;\n", type);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_PTR_LOCK:
    അവസ്ഥ ACTION_TYPE_PTR_BUTTON:
        പ്രതിജ്ഞാ(action->btn.flags == 0);
        പ്രതിജ്ഞാ(action->btn.button == 0);
        പ്രതിജ്ഞാ(action->btn.എണ്ണം == 0);
        write_buf(buf, PREFIX"%s.button = 0;\n", type);
        write_buf(buf, PREFIX"%s.count = 0;\n", type);
        എങ്കിൽ (action->type == ACTION_TYPE_PTR_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_PTR_DEFAULT:
        പ്രതിജ്ഞാ(action->dflt.flags == 0);
        പ്രതിജ്ഞാ(action->dflt.മൂല്യം == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.button = +1;\n", type);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_SWITCH_VT:
        പ്രതിജ്ഞാ(action->screen.flags == ACTION_SAME_SCREEN);
        പ്രതിജ്ഞാ(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.screen = +0;\n", type);
        write_buf(buf, PREFIX"%s.same = true;\n", type);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_CTRL_SET:
    അവസ്ഥ ACTION_TYPE_CTRL_LOCK:
        പ്രതിജ്ഞാ(action->ctrls.flags == 0);
        പ്രതിജ്ഞാ(action->ctrls.ctrls == 0);
        write_buf(buf, PREFIX"%s.controls = 0;\n", type);
        എങ്കിൽ (action->type == ACTION_TYPE_CTRL_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        നിർത്തുക;

    അവസ്ഥ ACTION_TYPE_REDIRECT_താക്കോൽ:
        പ്രതിജ്ഞാ(action->redirect.keycode == keymap->redirect_താക്കോൽ_auto);
        പ്രതിജ്ഞാ(action->redirect.affect_mods == 0);
        പ്രതിജ്ഞാ(action->redirect.mods == 0);
        write_buf(buf, PREFIX"%s.keycode = auto;\n", type);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        write_buf(buf, PREFIX"%s.clearMods = 0;\n", type);
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_അക്കം_ENTRIES,
                      "Missing action type");
    }

#undef PREFIX

    തിരികെ true;
}

സ്ഥിതം bool
write_actions_defaults(സ്ഥിരം ഘടന xkb_keymap *keymap,
                       എണ്ണൽ xkb_keymap_format format, ഘടന buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    വേണ്ടി (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_അക്കം_ENTRIES; a++) {
        സ്ഥിരം സംഘം xkb_action * restrict സ്ഥിരം action = &defaults.actions[a];
        എങ്കിൽ (!write_action_defaults(keymap, format, action, buf))
            തിരികെ false;
    }
    copy_to_buf(buf, "\n");

    തിരികെ true;
}

സ്ഥിതം സ്ഥിരം ഘടന xkb_sym_interpret fallback_interpret = {
    .sym = XKB_താക്കോൽ_Void_ചിഹ്നം,
    .required = true,
    .repeat = FALLBACK_INTERPRET_താക്കോൽ_REPEAT,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .അക്കം_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

സ്ഥിതം bool
write_compat(സ്ഥിരം ഘടന xkb_keymap * restrict keymap,
             എണ്ണൽ xkb_keymap_format format, xkb_layout_index_t max_groups,
             എണ്ണൽ xkb_keymap_serialize_flags flags, bool * restrict some_interp,
             ഘടന buf *buf)
{
    സ്ഥിരം bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    സ്ഥിരം bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_മൂല്യങ്ങൾ);
    സ്ഥിരം bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    സ്ഥിരം bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_താക്കോൽ_മൂല്യങ്ങൾ) && drop_unused);
    /* TODO: print all default values if the explicit flag is set */

    എങ്കിൽ (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    അല്ലെങ്കിൽ
        copy_to_buf(buf, "xkb_compatibility {\n");

    എങ്കിൽ (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        തിരികെ false;

    എങ്കിൽ (defaults && !write_actions_defaults(keymap, format, buf))
        തിരികെ false;

    copy_to_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_to_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    സ്ഥിരം bool use_fallback_interpret =
        (!keymap->അക്കം_sym_interprets || drop_interprets);
    സ്ഥിരം darray_size_t അക്കം_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->അക്കം_sym_interprets);
    സ്ഥിരം ഘടന xkb_sym_interpret* സ്ഥിരം sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    വേണ്ടി (darray_size_t i = 0; i < അക്കം_sym_interprets; i++) {
        സ്ഥിരം ഘടന xkb_sym_interpret *si = &sym_interprets[i];
        എങ്കിൽ (!si->required && drop_unused)
            തുടരുക;

        *some_interp = true;

        copy_to_buf(buf, "\tinterpret ");
        എങ്കിൽ (!si->sym) {
            copy_to_buf(buf, "Any");
        } അല്ലെങ്കിൽ എങ്കിൽ (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } അല്ലെങ്കിൽ {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->match),
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        എങ്കിൽ (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      ModIndexText(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        എങ്കിൽ (si->level_one_only) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } അല്ലെങ്കിൽ എങ്കിൽ (defaults) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        എങ്കിൽ (si->repeat) {
            copy_to_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } അല്ലെങ്കിൽ എങ്കിൽ (defaults) {
            copy_to_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        സ്ഥിരം സംഘം xkb_action *action = ശൂന്യ;
        xkb_action_എണ്ണം_t action_എണ്ണം = si->അക്കം_actions;
        എങ്കിൽ (action_എണ്ണം <= 1) {
            action = &si->a.action;
        } അല്ലെങ്കിൽ എങ്കിൽ (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_എണ്ണം = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        എങ്കിൽ (action_എണ്ണം > 1) {
            copy_to_buf(buf, "\n\t\taction= {");
            സ്ഥിരം അക്ഷരം suffix[] = ", ";
            വേണ്ടി (xkb_action_എണ്ണം_t k = 0; k < si->അക്കം_actions; k++) {
                എങ്കിൽ (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    തിരികെ false;
            }
            buf->size -= വലുപ്പം(suffix) - 1; /* trailing comma */
            copy_to_buf(buf, "};");
            has_explicit_properties = true;
        } അല്ലെങ്കിൽ എങ്കിൽ (action_എണ്ണം == 1) {
            എങ്കിൽ (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                തിരികെ false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    എങ്കിൽ (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        അക്കം_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].match == fallback_interpret.match &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].അക്കം_actions == fallback_interpret.അക്കം_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    സ്ഥിരം ഘടന xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        എങ്കിൽ ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            തിരികെ false;
        }
    }

    copy_to_buf(buf, "};\n\n");

    തിരികെ true;
}

സ്ഥിതം bool
write_keysyms(സ്ഥിരം ഘടന xkb_keymap *keymap,
              ഘടന buf *buf, ഘടന buf *buf2,
              സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ, xkb_layout_index_t group,
              bool pretty, bool show_actions)
{
    സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : ചിഹ്നം_PADDING)
        : 0;
    വേണ്ടി (xkb_level_index_t level = 0; level < Xkb_താക്കോൽ_അക്കം_Levels(താക്കോൽ, group);
         level++) {
        സ്ഥിരം xkb_keysym_t *syms;
        പൂർണ്ണസംഖ്യ അക്കം_syms;

        എങ്കിൽ (level != 0)
            copy_to_buf(buf, ", ");

        അക്കം_syms = xkb_keymap_താക്കോൽ_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (ഘടന xkb_keymap *)keymap,
            താക്കോൽ->keycode, group, level, &syms
        );

        സ്ഥിരം xkb_keysym_t no_ചിഹ്നം = XKB_താക്കോൽ_No_ചിഹ്നം;
        എങ്കിൽ (അക്കം_syms == 0) {
            അക്കം_syms = 1;
            syms = &no_ചിഹ്നം;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        എങ്കിൽ (അക്കം_syms == 1) {
            എങ്കിൽ (pretty || syms[0] == XKB_താക്കോൽ_No_ചിഹ്നം)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            അല്ലെങ്കിൽ
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } അല്ലെങ്കിൽ {
            എങ്കിൽ (pretty) {
                buf2->size = 0;
                copy_to_buf(buf2, "{ ");
                വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < അക്കം_syms; s++) {
                    എങ്കിൽ (s != 0)
                        copy_to_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_to_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } അല്ലെങ്കിൽ {
                copy_to_buf(buf, "{");
                വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < അക്കം_syms; s++) {
                    എങ്കിൽ (s != 0)
                        copy_to_buf(buf, ",");
                    എങ്കിൽ (syms[s] == XKB_താക്കോൽ_No_ചിഹ്നം)
                        copy_to_buf(buf, "NoSymbol");
                    അല്ലെങ്കിൽ
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_to_buf(buf, "}");
            }
        }
    }

    തിരികെ true;
}

സ്ഥിതം bool
write_താക്കോൽ(സ്ഥിരം ഘടന xkb_keymap *keymap,
          സ്ഥിരം ഘടന xkb_keymap_serialize_config *config,
          സ്ഥിരം താക്കോൽ_name_substitutions *substitutions,
          xkb_layout_index_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          ഘടന buf *buf, ഘടന buf *buf2, സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ)
{
    bool simple = true;

    xkb_layout_mask_t layouts = config->layouts;
    എങ്കിൽ (popcount32(layouts) != keymap->അക്കം_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_index_t l = 0;
        വേണ്ടി (xkb_layout_mask_t ls = layouts; ls; ls >>= 1, l++) {
            എങ്കിൽ (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                സ്ഥിരം xkb_layout_index_t effective =
                    xkb_keymap_താക്കോൽ_effective_layout(താക്കോൽ, l);
                എങ്കിൽ (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } അല്ലെങ്കിൽ {
        layouts &= ((UINT64_C(1) << താക്കോൽ->അക്കം_groups) - 1);
    }
    സ്ഥിരം xkb_layout_index_t groups_എണ്ണം = popcount32(layouts);

    സ്ഥിരം xkb_atom_t name = (substitutions == ശൂന്യ)
        ? താക്കോൽ->name
        : substitute_name(substitutions, താക്കോൽ->name);

    എങ്കിൽ (pretty)
        write_buf(buf, "\tkey %-20s {", താക്കോൽ_Name_Text(keymap->ctx, name));
    അല്ലെങ്കിൽ
        write_buf(buf, "\tkey %s {", താക്കോൽ_Name_Text(keymap->ctx, name));

    എങ്കിൽ ((താക്കോൽ->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        എങ്കിൽ (!explicit) {
            വേണ്ടി (xkb_layout_index_t group = 1; group < താക്കോൽ->അക്കം_groups; group++) {
                എങ്കിൽ (!((UINT32_C(1) << group) & layouts))
                    തുടരുക;

                എങ്കിൽ (താക്കോൽ->groups[group].type != താക്കോൽ->groups[0].type) {
                    multi_type = true;
                    നിർത്തുക;
                }
            }
        }

        എങ്കിൽ (multi_type) {
            xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
            വേണ്ടി (xkb_layout_index_t group = 0; group < താക്കോൽ->അക്കം_groups; group++) {
                എങ്കിൽ (!((UINT32_C(1) << group) & layouts))
                    തുടരുക;

                new_group++;

                എങ്കിൽ (!താക്കോൽ->groups[group].explicit_type && !explicit)
                    തുടരുക;

                സ്ഥിരം ഘടന xkb_താക്കോൽ_type * സ്ഥിരം type = താക്കോൽ->groups[group].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_group + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_to_buf(buf, ",");
            }
        }
        അല്ലെങ്കിൽ {
            സ്ഥിരം ഘടന xkb_താക്കോൽ_type * സ്ഥിരം type = താക്കോൽ->groups[0].type;
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
    സ്ഥിരം bool explicit_actions = (താക്കോൽ->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #define require_explicit(താക്കോൽ, prop,                                       \
                             മൂല്യം, initial_മൂല്യം, default_interpret_മൂല്യം) ( \
        /* Explicit value in the source */                                    \
        ((താക്കോൽ)->explicit & (prop)) ||                                         \
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
                ((താക്കോൽ)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (മൂല്യം) != (initial_മൂല്യം) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_മൂല്യം) != (default_interpret_മൂല്യം)                  \
            )                                                                 \
        )                                                                     \
    )

    എങ്കിൽ (require_explicit(താക്കോൽ, EXPLICIT_REPEAT, താക്കോൽ->repeats,
                         DEFAULT_താക്കോൽ_REPEAT, DEFAULT_INTERPRET_താക്കോൽ_REPEAT)) {
        എങ്കിൽ (താക്കോൽ->repeats)
            copy_to_buf(buf, "\n\t\trepeat= Yes,");
        അല്ലെങ്കിൽ
            copy_to_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    എങ്കിൽ (require_explicit(താക്കോൽ, EXPLICIT_VMODMAP, താക്കോൽ->vmodmap,
                         (xkb_mod_mask_t) DEFAULT_താക്കോൽ_VMODMAP,
                         (xkb_mod_mask_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              താക്കോൽ->vmodmap));
        simple = false;
    }

    #undef require_explicit

    തിരഞ്ഞെടുപ്പ് (താക്കോൽ->out_of_range_group_policy) {
    അവസ്ഥ XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        copy_to_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        നിർത്തുക;

    അവസ്ഥ XKB_LAYOUT_OUT_OF_RANGE_REDIRECT: {
        എങ്കിൽ (!groups_എണ്ണം) {
            /* No layout to redirect to: skip */
            നിർത്തുക;
        }

        xkb_layout_mask_t layout_mask =
            (UINT32_C(1) << താക്കോൽ->out_of_range_group_സംഖ്യ);
        എങ്കിൽ (!(layouts & layout_mask)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     താക്കോൽ_Name_Text(keymap->ctx, name),
                     താക്കോൽ->out_of_range_group_സംഖ്യ + 1);
            layout_mask = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        സ്ഥിരം xkb_layout_mask_t താഴെയുള്ള = (layouts & (layout_mask - 1));
        സ്ഥിരം xkb_layout_index_t new = popcount32(താഴെയുള്ള);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        നിർത്തുക;
    }

    അവസ്ഥ XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    സ്ഥിരസ്ഥിതി:
        നിർത്തുക;
    }

    എങ്കിൽ (താക്കോൽ->overlays) {
        xkb_overlay_mask_t remaining = താക്കോൽ->overlays;

        സ്ഥിരം xkb_overlay_index_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_എണ്ണം == 8, "invalid right shift");
        സ്ഥിരം xkb_overlay_mask_t valid =
            (xkb_overlay_mask_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        എങ്കിൽ (remaining != താക്കോൽ->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, താക്കോൽ_Name_Text(keymap->ctx, name),
                     remaining, താക്കോൽ->overlays & ~valid);
        }

        എങ്കിൽ (remaining && !താക്കോൽ->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            സ്ഥിരം xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            സ്ഥിരം xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     താക്കോൽ_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        എങ്കിൽ (remaining)
            simple = false;

        സ്ഥിരം ഘടന xkb_താക്കോൽ * സ്ഥിരം *overlays_താക്കോലുകൾ = (താക്കോൽ->overlays_inline)
                                                    ? &താക്കോൽ->overlay_താക്കോൽ
                                                    : താക്കോൽ->overlays_താക്കോലുകൾ;
        ആകുമ്പോൾ (remaining) {
            /* isolate lowest set bit */
            സ്ഥിരം xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            സ്ഥിരം xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            എങ്കിൽ (താക്കോൽ->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            സ്ഥിരം ഘടന xkb_താക്കോൽ *overlay_താക്കോൽ = *(overlays_താക്കോലുകൾ++);

            സ്ഥിരം xkb_atom_t overlay_താക്കോൽ_name = (substitutions == ശൂന്യ)
                ? overlay_താക്കോൽ->name
                : substitute_name(substitutions, overlay_താക്കോൽ->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, താക്കോൽ_Name_Text(keymap->ctx, overlay_താക്കോൽ_name));
        }
    }

    എങ്കിൽ (groups_എണ്ണം > 1 || explicit_actions || explicit)
        simple = false;

    എങ്കിൽ (simple) {
        എങ്കിൽ (groups_എണ്ണം == 0) {
            /* Remove trailing comma */
            delete_last_char(buf, ',');
            എങ്കിൽ (buf->buf[buf->size - 1] == ',')
                buf->size--;
        } അല്ലെങ്കിൽ {
            copy_to_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_index_t group = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            group = xkb_keymap_താക്കോൽ_effective_layout(താക്കോൽ, group);
            എങ്കിൽ (!write_keysyms(keymap, buf, buf2, താക്കോൽ, group, pretty, false))
                തിരികെ false;
            copy_to_buf(buf, " ]");
        }
        copy_to_buf(buf, " };\n");
    }
    അല്ലെങ്കിൽ {
        xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
        വേണ്ടി (xkb_layout_index_t group = 0; group < താക്കോൽ->അക്കം_groups; group++) {
            എങ്കിൽ (!((UINT32_C(1) << group) & layouts))
                തുടരുക;

            new_group++;

            സ്ഥിരം bool print_actions =
                /* Group has explicit actions */
                താക്കോൽ->groups[group].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (താക്കോൽ->groups[group].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (താക്കോൽ->groups[group].implicit_actions && !some_interprets);

            എങ്കിൽ (new_group != 0)
                copy_to_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_group + 1);

            എങ്കിൽ (!write_keysyms(keymap, buf, buf2, താക്കോൽ, group,
                               pretty, print_actions))
                തിരികെ false;
            copy_to_buf(buf, " ]");
            എങ്കിൽ (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_group + 1);
                എങ്കിൽ (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, താക്കോൽ, group))
                    തിരികെ false;
                copy_to_buf(buf, " ]");
            }
        }
        എങ്കിൽ (!groups_എണ്ണം)
            delete_last_char(buf, ',');
        copy_to_buf(buf, "\n\t};\n");
    }

    തിരികെ true;
}


സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_modmap_step_1(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
    സ്ഥിരം ModMapEntry *m1 = (സ്ഥിരം ModMapEntry *)a;
    സ്ഥിരം ModMapEntry *m2 = (സ്ഥിരം ModMapEntry *)b;

    എങ്കിൽ (!m1->mods) {
        എങ്കിൽ (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            തിരികെ 0;
        }
        തിരികെ +1;
    } അല്ലെങ്കിൽ എങ്കിൽ (!m2->mods) {
        തിരികെ -1;
    }

    /* Valid only on step 1 */
    എങ്കിൽ (m1->താക്കോൽ_Code < m2->താക്കോൽ_Code)
        തിരികെ -1;
    അല്ലെങ്കിൽ എങ്കിൽ (m1->താക്കോൽ_Code > m2->താക്കോൽ_Code)
        തിരികെ +1;

    എങ്കിൽ (m1->mods < m2->mods)
        തിരികെ -1;
    അല്ലെങ്കിൽ എങ്കിൽ (m1->mods > m2->mods)
        തിരികെ +1;

    എങ്കിൽ (m1->have_ചിഹ്നം < m2->have_ചിഹ്നം)
        തിരികെ +1;
    അല്ലെങ്കിൽ എങ്കിൽ (m1->have_ചിഹ്നം > m2->have_ചിഹ്നം)
        തിരികെ -1;

    എങ്കിൽ (m1->have_ചിഹ്നം) {
        എങ്കിൽ (m1->u.താക്കോൽ_Sym < m2->u.താക്കോൽ_Sym)
            തിരികെ -1;
        എങ്കിൽ (m1->u.താക്കോൽ_Sym > m2->u.താക്കോൽ_Sym)
            തിരികെ +1;
    } അല്ലെങ്കിൽ {
        എങ്കിൽ (m1->u.താക്കോൽ_Name < m2->u.താക്കോൽ_Name)
            തിരികെ -1;
        എങ്കിൽ (m1->u.താക്കോൽ_Name > m2->u.താക്കോൽ_Name)
            തിരികെ +1;
    }

    തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
cmp_modmap_step_2(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
    സ്ഥിരം ModMapEntry *m1 = (സ്ഥിരം ModMapEntry *)a;
    സ്ഥിരം ModMapEntry *m2 = (സ്ഥിരം ModMapEntry *)b;

    എങ്കിൽ (m1->mods < m2->mods)
        തിരികെ -1;
    എങ്കിൽ (m1->mods > m2->mods)
        തിരികെ +1;

    എങ്കിൽ (m1->have_ചിഹ്നം < m2->have_ചിഹ്നം)
        തിരികെ -1;
    എങ്കിൽ (m1->have_ചിഹ്നം > m2->have_ചിഹ്നം)
        തിരികെ +1;

    എങ്കിൽ (m1->have_ചിഹ്നം) {
        എങ്കിൽ (m1->u.താക്കോൽ_Sym < m2->u.താക്കോൽ_Sym)
            തിരികെ -1;
        എങ്കിൽ (m1->u.താക്കോൽ_Sym > m2->u.താക്കോൽ_Sym)
            തിരികെ +1;
    } അല്ലെങ്കിൽ {
        എങ്കിൽ (m1->താക്കോൽ_Code < m2->താക്കോൽ_Code)
            തിരികെ -1;
        എങ്കിൽ (m1->താക്കോൽ_Code > m2->താക്കോൽ_Code)
            തിരികെ +1;
    }

    തിരികെ 0;
}

സ്ഥിതം bool
write_modmaps(സ്ഥിരം ഘടന xkb_keymap *keymap, എണ്ണൽ xkb_keymap_format format,
              സ്ഥിരം താക്കോൽ_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, ഘടന buf *buf)
{
    എങ്കിൽ (!keymap->അക്കം_modmaps)
        തിരികെ true;

    darray(ModMapEntry) modmaps = darray_new();
    darray_from_ഇനങ്ങൾ(modmaps, keymap->modmaps, keymap->അക്കം_modmaps);
    എങ്കിൽ (!darray_ഇനങ്ങൾ(modmaps))
        തിരികെ false;

    /* Sort entries by key code */
    qsort(darray_ഇനങ്ങൾ(modmaps), darray_size(modmaps),
          വലുപ്പം(*darray_ഇനങ്ങൾ(modmaps)), &cmp_modmap_step_1);

    /* Drop disabled entries at the end */
    വേണ്ടി (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        എങ്കിൽ (darray_item(modmaps, m).mods)
            നിർത്തുക;
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
    വേണ്ടി (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ModMapEntry *mm = &darray_item(modmaps, m);
        പ്രതിജ്ഞാ(mm->mods);

        എങ്കിൽ (mm->താക്കോൽ_Code == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            സ്ഥിരം xkb_mod_mask_t core_mods = (mm->mods & MOD_REAL_MASK_ALL);
            എങ്കിൽ (convert_to_v1_compatibility_entry && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } അല്ലെങ്കിൽ {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            തുടരുക;
        }

        current_keycode = mm->താക്കോൽ_Code;
        സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ = Xkb_താക്കോൽ(keymap, mm->താക്കോൽ_Code);

        സ്ഥിരം xkb_mod_mask_t core_mods = (താക്കോൽ->modmap & MOD_REAL_MASK_ALL);
        എങ്കിൽ (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((താക്കോൽ->modmap & (താക്കോൽ->modmap - 1)) || താക്കോൽ->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_to_v1_compatibility_entry = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    താക്കോൽ_Name_Text(keymap->ctx, താക്കോൽ->name), താക്കോൽ->modmap);
        } അല്ലെങ്കിൽ {
            /* V2 format or V1 format with a single core modifier */
            പ്രതിജ്ഞാ(താക്കോൽ->modmap);
            mm->mods = താക്കോൽ->modmap;
            /* Force key name */
            mm->have_ചിഹ്നം = false;
            mm->u.താക്കോൽ_Name = താക്കോൽ->name;
            convert_to_v1_compatibility_entry = false;
        }
    }

    എങ്കിൽ (darray_empty(modmaps))
        പോകുക exit;

    /* Sort entries by modifier mask */
    qsort(darray_ഇനങ്ങൾ(modmaps), darray_size(modmaps),
          വലുപ്പം(*darray_ഇനങ്ങൾ(modmaps)), &cmp_modmap_step_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_mask_t current_mods = 0;
    സ്ഥിരം ModMapEntry *entry;
    darray_foreach(entry, modmaps) {
        എങ്കിൽ (!entry->mods)
            തുടരുക;

        എങ്കിൽ (current_mods != entry->mods) {
            എങ്കിൽ (current_mods) {
                copy_to_buf(buf, " };\n");
            }
            current_mods = entry->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } അല്ലെങ്കിൽ {
            copy_to_buf(buf, ", ");
        }

        എങ്കിൽ (entry->have_ചിഹ്നം) {
            എങ്കിൽ (pretty || entry->u.താക്കോൽ_Sym == XKB_താക്കോൽ_No_ചിഹ്നം)
                write_buf(buf, "%s", KeysymText(keymap->ctx, entry->u.താക്കോൽ_Sym));
            അല്ലെങ്കിൽ
                write_buf(buf, "0x%"PRIx32, entry->u.താക്കോൽ_Sym);
        } അല്ലെങ്കിൽ {
            സ്ഥിരം xkb_atom_t name = (substitutions == ശൂന്യ)
                ? entry->u.താക്കോൽ_Name
                : substitute_name(substitutions, entry->u.താക്കോൽ_Name);
            write_buf(buf, "%s", താക്കോൽ_Name_Text(keymap->ctx, name));
        }
    }
    എങ്കിൽ (current_mods) {
        copy_to_buf(buf, " };\n");
    }

exit:
    darray_free(modmaps);
    തിരികെ true;
}

സ്ഥിതം bool
write_symbols(സ്ഥിരം ഘടന xkb_keymap *keymap,
              സ്ഥിരം ഘടന xkb_keymap_serialize_config *config,
              സ്ഥിരം താക്കോൽ_name_substitutions *substitutions,
              xkb_layout_index_t max_groups, bool some_interp,
              ഘടന buf *buf)
{
    സ്ഥിരം bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    സ്ഥിരം bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_മൂല്യങ്ങൾ);
    സ്ഥിരം bool explicit_താക്കോൽ_മൂല്യങ്ങൾ =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_താക്കോൽ_മൂല്യങ്ങൾ);
    സ്ഥിരം bool drop_unused = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    സ്ഥിരം bool drop_interprets = (explicit_താക്കോൽ_മൂല്യങ്ങൾ && drop_unused);
    /* TODO: print all default values if explicit flag is set */

    എങ്കിൽ (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    അല്ലെങ്കിൽ
        copy_to_buf(buf, "xkb_symbols {\n");

    bool has_group_names = false;
    xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
    വേണ്ടി (xkb_layout_index_t group = 0; group < keymap->അക്കം_group_names; group++) {
        എങ്കിൽ (!((UINT32_C(1) << group) & config->layouts))
            തുടരുക;

        new_group++;

        എങ്കിൽ (keymap->group_names[group]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_group + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->group_names[group]));
            copy_to_buf(buf, ";\n");
            has_group_names = true;
        }
    }
    എങ്കിൽ (has_group_names)
        copy_to_buf(buf, "\n");

    എങ്കിൽ (defaults && !write_actions_defaults(keymap, config->format, buf))
        തിരികെ false;

    ഘടന buf buf2 = { ശൂന്യ, 0, 0 };
    സ്ഥിരം ഘടന xkb_താക്കോൽ *താക്കോൽ;
    xkb_താക്കോലുകൾ_foreach(താക്കോൽ, keymap) {
        /* Skip keys with no explicit values */
        എങ്കിൽ (താക്കോൽ->explicit &&
            !write_താക്കോൽ(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_താക്കോൽ_മൂല്യങ്ങൾ, pretty, buf,
                       &buf2, താക്കോൽ))
        {
            സ്വതന്ത്രം(buf2.buf);
            തിരികെ false;
        }
    }
    സ്വതന്ത്രം(buf2.buf);

    എങ്കിൽ (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        തിരികെ false;

    copy_to_buf(buf, "};\n\n");
    തിരികെ true;
}

സ്ഥിതം എണ്ണൽ xkb_status
write_keymap(സ്ഥിരം ഘടന xkb_keymap *keymap,
             സ്ഥിരം ഘടന xkb_keymap_serialize_config *config,
             ഘടന buf *buf, ഘടന xkb_keymap_serialize_result *result)
{
    താക്കോൽ_name_substitutions substitutions = darray_new();
    എങ്കിൽ (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_താക്കോലുകൾ(keymap, &substitutions))
    {
        തിരികെ false;
    }

    സ്ഥിരം താക്കോൽ_name_substitutions * സ്ഥിരം substitutions_ptr =
        (darray_empty(substitutions)) ? ശൂന്യ : &substitutions;

    സ്ഥിരം xkb_layout_index_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    സ്ഥിരം bool ok = (
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
    തിരികെ ok ? XKB_SUCCESS : -1;
}

എണ്ണൽ xkb_status
text_v1_keymap_serialize(
        സ്ഥിരം ഘടന xkb_keymap *keymap,
        സ്ഥിരം ഘടന xkb_keymap_serialize_config *config,
        ഘടന xkb_keymap_serialize_result *result
)
{
    ഘടന buf buf = { ശൂന്യ, 0, 0 };

    സ്ഥിരം എണ്ണൽ xkb_status status =
        write_keymap(keymap, config, &buf, result);
    എങ്കിൽ (status != XKB_SUCCESS) {
        സ്വതന്ത്രം(buf.buf);
        result->serialized = ശൂന്യ;
        തിരികെ status;
    }

    result->serialized = buf.buf;
    result->length = buf.size + 1;
    result->layouts = config->layouts;

    തിരികെ XKB_SUCCESS;
}
