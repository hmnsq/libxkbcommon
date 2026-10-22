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

#ಅನ್ತರ್ಭಾವ "config.h"

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-status.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon-keysyms.h"
#ಅನ್ತರ್ಭಾವ "xkbcommon/xkbcommon.h"
#ಅನ್ತರ್ಭಾವ "atom.h"
#ಅನ್ತರ್ಭಾವ "action.h"
#ಅನ್ತರ್ಭಾವ "context.h"
#ಅನ್ತರ್ಭಾವ "darray.h"
#ಅನ್ತರ್ಭಾವ "keymap.h"
#ಅನ್ತರ್ಭಾವ "messages-codes.h"
#ಅನ್ತರ್ಭಾವ "text.h"
#ಅನ್ತರ್ಭಾವ "util-numbers.h"
#ಅನ್ತರ್ಭಾವ "xkbcomp-priv.h"

#define BUF_CHUNK_SIZE 4096

ಸಂರಚನಾ buf {
    ವರ್ಣ *buf;
    size_t size;
    size_t alloc;
};

#define xkb_abs(n) _Generic((n),                \
                            ಪೂರ್ಣಾಙ್ಕ: abs,           \
                            long ಪೂರ್ಣಾಙ್ಕ: labs,     \
                            ಸ್ವಭಾವ: llabs )((n))

ಸ್ಥಿರ bool
do_realloc(ಸಂರಚನಾ buf *buf, size_t at_least)
{
    buf->alloc += BUF_CHUNK_SIZE;
    ಯದಿ (at_least >= BUF_CHUNK_SIZE)
        buf->alloc += at_least;

    ವರ್ಣ *ಧ್ರುವ new = realloc(buf->buf, buf->alloc);
    ಯದಿ (!new)
        ಪ್ರತ್ಯಾಗಮ false;

    buf->buf = new;
    ಪ್ರತ್ಯಾಗಮ true;
}

ATTR_PRINTF(2, 3) ಸ್ಥಿರ bool
check_write_buf(ಸಂರಚನಾ buf *buf, ಧ್ರುವ ವರ್ಣ *fmt, ...)
{
    va_list args;
    ಪೂರ್ಣಾಙ್ಕ printed;
    size_t available;

    available = buf->alloc - buf->size;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    ಪ್ರತಿಜ್ಞಾ(buf->buf || available == 0);
    va_start(args, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->size : buf->buf, available,
                        fmt, args);
    va_end(args);

    ಯದಿ (printed < 0) {
        /* Some error */
        ಗಚ್ಛ err;
    } ಅನ್ಯಥಾ ಯದಿ ((size_t)printed >= available) {
        /* Not enough space: need to realloc */
        ಯದಿ (!do_realloc(buf, (size_t)printed + 1))
            ಗಚ್ಛ err;

        /* The buffer has enough space now. */
        available = buf->alloc - buf->size;
        ಪ್ರತಿಜ್ಞಾ(buf->buf && (size_t) printed < available);
        va_start(args, fmt);
        printed = vsnprintf(buf->buf + buf->size, available, fmt, args);
        va_end(args);

        ಯದಿ (printed < 0 || (size_t) printed >= available)
            ಗಚ್ಛ err;
    }

    buf->size += (size_t)printed;
    ಪ್ರತ್ಯಾಗಮ true;

err:
    ಮುಕ್ತ(buf->buf);
    buf->buf = ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ false;
}

#define write_buf(buf, ...) ಕುರು { \
    ಯದಿ (!check_write_buf(buf, __VA_ARGS__)) \
        ಪ್ರತ್ಯಾಗಮ false; \
} ಯಾವತ್ (0)

ಸ್ಥಿರ bool
check_copy_to_buf(ಸಂರಚನಾ buf *buf, ಧ್ರುವ ವರ್ಣ* source, size_t len)
{
    ಯದಿ (len == 0)
        ಪ್ರತ್ಯಾಗಮ true;

    ಧ್ರುವ size_t available = buf->alloc - buf->size;
    ಯದಿ (len >= available &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        ಮುಕ್ತ(buf->buf);
        buf->buf = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ false;
    }

    memcpy(buf->buf + buf->size, source, len);
    buf->size += len;
    /* Append NULL byte */
    buf->buf[buf->size] = '\0';
    ಪ್ರತ್ಯಾಗಮ true;
}

#define copy_to_buf_len(buf, source, len) ಕುರು { \
    ಯದಿ (!check_copy_to_buf(buf, source, len)) \
        ಪ್ರತ್ಯಾಗಮ false; \
} ಯಾವತ್ (0)

#define copy_to_buf(buf, source) \
    copy_to_buf_len(buf, source, ಪರಿಮಾಣ(source) - 1)

ಸ್ಥಿರ bool
check_write_string_literal(ಸಂರಚನಾ buf *buf, ಧ್ರುವ ವರ್ಣ* string)
{
    copy_to_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    size_t pending_start = 0;
    size_t current = 0;
    ವರ್ಣ escape_buffer[] = {'\\', '0', '0', '0', '0'};
    ಪ್ರತಿ (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        ವಿಕಲ್ಪ (string[current]) {
        ಪಕ್ಷ '\n':
            /* `\n` would break strings */
            escape_buffer[1] = 'n';
            escape_len = 2;
            ಭಙ್ಗ;
        ಪಕ್ಷ '\\':
            /* `\` would create escape sequences */
            escape_buffer[1] = '\\';
            escape_len = 2;
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
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
            ಯದಿ (unlikely((unsigned ವರ್ಣ)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_buffer[1] = '0';
                escape_buffer[2] = '0';
                escape_buffer[3] = (ವರ್ಣ)('0' + (string[current] >> 3));
                escape_buffer[4] = (ವರ್ಣ)('0' + (string[current] & 0x7));
                escape_len = 5;
            } ಅನ್ಯಥಾ {
                /* Expand the chunk */
                ಅನುವರ್ತ;
            }
        }
        /* Write pending chunk */
        ಪ್ರತಿಜ್ಞಾ(current >= pending_start);
        copy_to_buf_len(buf, string + pending_start, current - pending_start);
        /* Write escape sequence */
        copy_to_buf_len(buf, escape_buffer, escape_len);
        pending_start = current + 1;
    }
    /* Write remaining chunk */
    ಪ್ರತಿಜ್ಞಾ(current >= pending_start);
    copy_to_buf_len(buf, string + pending_start, current - pending_start);

    copy_to_buf(buf, "\"");
    ಪ್ರತ್ಯಾಗಮ true;
}

#define write_buf_string_literal(buf, string) ಕುರು { \
    ಯದಿ (!check_write_string_literal(buf, string)) \
        ಪ್ರತ್ಯಾಗಮ false; \
} ಯಾವತ್ (0)

ಸ್ಥಿರ ಶೂನ್ಯ
delete_last_char(ಸಂರಚನಾ buf *buf, ವರ್ಣ c)
{
    ಯದಿ (buf->size && buf->buf[buf->size - 1] == c)
        buf->buf[--buf->size] = '\0';
}

/** Entry for a name substituion LUT */
ಸಂರಚನಾ ಕೀಲಿ_name_substitution {
    xkb_atom_t source;
    xkb_atom_t target;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_ಕೀಲಿ_name_substitution(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
    ಧ್ರುವ xkb_atom_t s1 = ((ಸಂರಚನಾ ಕೀಲಿ_name_substitution *) a)->source;
    ಧ್ರುವ xkb_atom_t s2 = ((ಸಂರಚನಾ ಕೀಲಿ_name_substitution *) b)->source;
    ಪ್ರತ್ಯಾಗಮ (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

ಪ್ರಕಾರನಿರ್ದೇಶ darray(ಸಂರಚನಾ ಕೀಲಿ_name_substitution) ಕೀಲಿ_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
ಸ್ಥಿರ bool
rename_long_ಕೀಲಿಗಳು(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
                 ಕೀಲಿ_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    ಗಣನಾ {
        first = '!',
        last = '0',
        invalid = '\0'
    };
    ವರ್ಣ prefixes[] = {
        first, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', last
    };
    static_assert(ಪರಿಮಾಣ(prefixes) == last - first + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        ಯದಿ (ಕೀಲಿ->name == XKB_ATOM_NONE)
            ಅನುವರ್ತ;

        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name = xkb_atom_text(keymap->ctx, ಕೀಲಿ->name);
        ಧ್ರುವ size_t len = strlen_safe(name);
        ಯದಿ (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (ಸಂರಚನಾ ಕೀಲಿ_name_substitution) {
                .source = ಕೀಲಿ->name,
                .keycode = ಕೀಲಿ->keycode,
                .target = ಕೀಲಿ->name
            });
        } ಅನ್ಯಥಾ ಯದಿ (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    /* Check aliases */
    ಪ್ರತಿ (darray_size_t a = 0; a < keymap->ಅಂಕೆ_ಕೀಲಿ_aliases; a++) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_alias * ಧ್ರುವ alias = &keymap->ಕೀಲಿ_aliases[a];
        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ name = xkb_atom_text(keymap->ctx, alias->alias);
        ಧ್ರುವ size_t len = strlen_safe(name);
        ಯದಿ (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (ಸಂರಚನಾ ಕೀಲಿ_name_substitution) {
                .source = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .target = alias->alias
            });
        } ಅನ್ಯಥಾ ಯದಿ (len == 4 && name[0] >= first && name[0] <= last) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - first] = invalid;
        }
    }

    ಯದಿ darray_empty(*substitutions) {
        /* Nothing to do */
        ಪ್ರತ್ಯಾಗಮ true;
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
    ವರ್ಣ *prefix = prefixes;
    ಧ್ರುವ ವರ್ಣ *prefix_max = prefixes + ಪರಿಮಾಣ(prefixes) - 1;
    ಯಾವತ್ (*prefix == invalid && prefix < prefix_max)
        prefix++;

    /* Keycode prefix: prefer ‘0’ if available */
    ವರ್ಣ keycode_prefix;
    ಯದಿ (prefixes[last - first] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_prefix = last;
        prefixes[last - first] = invalid;
    } ಅನ್ಯಥಾ {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_prefix = *prefix;
        ಯದಿ (unlikely(keycode_prefix == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            ಪ್ರತ್ಯಾಗಮ true;
        }
        /* Update current prefix */
        *prefix = invalid;
        ಯಾವತ್ (*prefix == invalid && prefix < prefix_max)
            prefix++;
    }

    /* Proceed renaming */
    uint16_t next_id = 1;
    ಸಂರಚನಾ ಕೀಲಿ_name_substitution *sub;
    ವರ್ಣ buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        ಯದಿ (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, ಪರಿಮಾಣ(buf), "%c%03"PRIx32,
                     keycode_prefix, sub->keycode);
            ಧ್ರುವ xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, ಪರಿಮಾಣ(buf) - 2);
            ಯದಿ (target == XKB_ATOM_NONE)
                ಗಚ್ಛ error;
            sub->target = target;
        } ಅನ್ಯಥಾ ಯದಿ (*prefix != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, ಪರಿಮಾಣ(buf), "%c%03"PRIx16, *prefix, next_id);
            ಧ್ರುವ xkb_atom_t target = xkb_atom_intern(keymap->ctx,
                                                      buf, ಪರಿಮಾಣ(buf) - 2);
            ಯದಿ (target == XKB_ATOM_NONE)
                ಗಚ್ಛ error;
            sub->target = target;
            ಯದಿ (++next_id > 0xfff) {
                *prefix = invalid;
                /* Get next available prefix */
                ಯಾವತ್ (*prefix == invalid && prefix < prefix_max)
                    prefix++;
                /* Reset counter */
                next_id = 1;
            }
        } ಅನ್ಯಥಾ {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->target = sub->source;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_ವಸ್ತುಗಳು(*substitutions), darray_size(*substitutions),
          ಪರಿಮಾಣ(*darray_ವಸ್ತುಗಳು(*substitutions)), &cmp_ಕೀಲಿ_name_substitution);

    ಪ್ರತ್ಯಾಗಮ true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ALLOCATION_FAILURE_,
            "Cannot allocate key name substitution\n");
    ಪ್ರತ್ಯಾಗಮ false;
}

/** Substitute a key name with a valid X11 key name */
ಸ್ಥಿರ xkb_atom_t
substitute_name(ಧ್ರುವ ಕೀಲಿ_name_substitutions *substitutions, xkb_atom_t name)
{
    /*ದ್ವಿಮಾನ ಹುಡುಕು*/
    darray_size_t first = 0;
    darray_size_t last = darray_size(*substitutions);

    ಯಾವತ್ (last > first) {
        darray_size_t mid = first + (last - 1 - first) / 2;
        ಧ್ರುವ xkb_atom_t source = darray_item(*substitutions, mid).source;
        ಯದಿ (source < name)
            first = mid + 1;
        ಅನ್ಯಥಾ ಯದಿ (source > name)
            last = mid;
        ಅನ್ಯಥಾ /* found it */
            ಪ್ರತ್ಯಾಗಮ darray_item(*substitutions, mid).target;
    }

    /* No name substitution: return original name */
    ಪ್ರತ್ಯಾಗಮ name;
}

ಸ್ಥಿರ bool
write_vmods(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, ಗಣನಾ xkb_keymap_format format,
            bool explicit, ಸಂರಚನಾ buf *buf)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_mod *mod;
    xkb_mod_index_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        ಯದಿ (!has_some) {
            copy_to_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } ಅನ್ಯಥಾ {
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
        ಧ್ರುವ bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_TEXT_V2);

        ಯದಿ ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
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

    ಯದಿ (has_some)
        copy_to_buf(buf, ";\n\n");

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
write_keycodes(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
               ಧ್ರುವ ಕೀಲಿ_name_substitutions *substitutions,
               ಗಣನಾ xkb_keymap_serialize_flags flags, ಸಂರಚನಾ buf *buf)
{
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;
    xkb_led_index_t idx;
    ಧ್ರುವ ಸಂರಚನಾ xkb_led *led;

    ಧ್ರುವ bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    ಯದಿ (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    ಅನ್ಯಥಾ
        copy_to_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_ಕೀಲಿ_code, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_ಕೀಲಿ_code, 255));

    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        ಯದಿ (ಕೀಲಿ->name == XKB_ATOM_NONE)
            ಅನುವರ್ತ;

        ಧ್ರುವ xkb_atom_t name = (substitutions == ರಿಕ್ತ)
            ? ಕೀಲಿ->name
            : substitute_name(substitutions, ಕೀಲಿ->name);
        ಯದಿ (pretty)
            write_buf(buf, "\t%-20s", ಕೀಲಿ_Name_Text(keymap->ctx, name));
        ಅನ್ಯಥಾ
            write_buf(buf, "\t%s", ಕೀಲಿ_Name_Text(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", ಕೀಲಿ->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        ಯದಿ (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
            copy_to_buf(buf, ";\n");
        }

    ಪ್ರತಿ (darray_size_t i = 0; i < keymap->ಅಂಕೆ_ಕೀಲಿ_aliases; i++) {
        ಧ್ರುವ xkb_atom_t alias = (substitutions == ರಿಕ್ತ)
            ? keymap->ಕೀಲಿ_aliases[i].alias
            : substitute_name(substitutions, keymap->ಕೀಲಿ_aliases[i].alias);
        ಧ್ರುವ xkb_atom_t real = (substitutions == ರಿಕ್ತ)
            ? keymap->ಕೀಲಿ_aliases[i].real
            : substitute_name(substitutions, keymap->ಕೀಲಿ_aliases[i].real);

        ಯದಿ (pretty)
            write_buf(buf, "\talias %-14s",
                      ಕೀಲಿ_Name_Text(keymap->ctx, alias));
        ಅನ್ಯಥಾ
            write_buf(buf, "\talias %s",
                      ಕೀಲಿ_Name_Text(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  ಕೀಲಿ_Name_Text(keymap->ctx, real));
    }

    copy_to_buf(buf, "};\n\n");
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
write_types(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, ಗಣನಾ xkb_keymap_format format,
            ಗಣನಾ xkb_keymap_serialize_flags flags, ಸಂರಚನಾ buf *buf)
{
    ಯದಿ (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    ಅನ್ಯಥಾ
        copy_to_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    ಯದಿ (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        ಪ್ರತ್ಯಾಗಮ false;

    ಧ್ರುವ bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ಪ್ರತಿ (darray_size_t i = 0; i < keymap->ಅಂಕೆ_types; i++) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type * ಧ್ರುವ type = &keymap->types[i];
        ಯದಿ (!type->required && drop_unused)
            ಅನುವರ್ತ;

        copy_to_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, type->name));
        copy_to_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        ಪ್ರತಿ (darray_size_t j = 0; j < type->ಅಂಕೆ_entries; j++) {
            ಧ್ರುವ ವರ್ಣ *str;
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type_entry *entry = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            ಯದಿ (entry->level == 0 && entry->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ಮೌಲ್ಯಗಳು))
                ಅನುವರ್ತ;

            str = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              entry->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, entry->level + 1);

            ಯದಿ (entry->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           entry->preserve.mods));
        }

        ಪ್ರತಿ (xkb_level_index_t n = 0; n < type->ಅಂಕೆ_level_names; n++)
            ಯದಿ (type->level_names[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_text(keymap->ctx, type->level_names[n]));
                copy_to_buf(buf, ";\n");
            }

        copy_to_buf(buf, "\t};\n");
    }

    copy_to_buf(buf, "};\n\n");
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
write_led_map(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, ಗಣನಾ xkb_keymap_format format,
              bool defaults, ಸಂರಚನಾ buf *buf, ಧ್ರುವ ಸಂರಚನಾ xkb_led *led)
{
    copy_to_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_text(keymap->ctx, led->name));
    copy_to_buf(buf, " {\n");

    ಯದಿ (led->which_groups) {
        ಯದಿ (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      LedStateMaskText(keymap->ctx, groupComponentMaskNames,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    ಯದಿ (led->which_mods) {
        ಯದಿ (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      LedStateMaskText(keymap->ctx, modComponentMaskNames,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    ಯದಿ (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  ControlMaskText(keymap->ctx, format, led->ctrls));
    }

    copy_to_buf(buf, "\t};\n");
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
affect_lock_text(ಗಣನಾ xkb_action_flags flags, bool show_both)
{
    ವಿಕಲ್ಪ (flags & (ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK)) {
    ಪಕ್ಷ 0:
        ಪ್ರತ್ಯಾಗಮ show_both ? ",affect=both" : "";
    ಪಕ್ಷ ACTION_LOCK_NO_UNLOCK:
        ಪ್ರತ್ಯಾಗಮ ",affect=lock";
    ಪಕ್ಷ ACTION_LOCK_NO_LOCK:
        ಪ್ರತ್ಯಾಗಮ ",affect=unlock";
    ಪಕ್ಷ ACTION_LOCK_NO_LOCK | ACTION_LOCK_NO_UNLOCK:
        ಪ್ರತ್ಯಾಗಮ ",affect=neither";
    ಸ್ವಭಾವ:
        ಪ್ರತ್ಯಾಗಮ "";
    }
}

#define ಚಿಹ್ನೆ_PADDING 15
#define ACTION_PADDING 30

ಸ್ಥಿರ bool
write_action(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, ಗಣನಾ xkb_keymap_format format,
             xkb_layout_index_t max_groups, ಸಂರಚನಾ buf *buf,
             ಧ್ರುವ ಸಙ್ಘ xkb_action *action,
             ಧ್ರುವ ವರ್ಣ *prefix, ಧ್ರುವ ವರ್ಣ *suffix)
{
    ಧ್ರುವ ವರ್ಣ *type;
    ಧ್ರುವ ವರ್ಣ *args = ರಿಕ್ತ;

    ಯದಿ (!prefix)
        prefix = "";
    ಯದಿ (!suffix)
        suffix = "";

    type = ActionTypeText(action->type);

    ವಿಕಲ್ಪ (action->type) {
    ಪಕ್ಷ ACTION_TYPE_MOD_SET:
    ಪಕ್ಷ ACTION_TYPE_MOD_LATCH:
    ಪಕ್ಷ ACTION_TYPE_MOD_LOCK:
        ಯದಿ (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            args = "modMapMods";
        ಅನ್ಯಥಾ
            args = ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        ಯದಿ (unlockOnPress && !isModsUnLockOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    ActionTypeText(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->group.flags & ACTION_LATCH_ON_PRESS);
        ಯದಿ (latchOnPress && !isModsLatchOnPressSupported(format)) {
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
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_GROUP_SET:
    ಪಕ್ಷ ACTION_TYPE_GROUP_LATCH:
    ಪಕ್ಷ ACTION_TYPE_GROUP_LOCK:
        ಯದಿ ((uint32_t) xkb_abs(action->group.group) <
            (max_groups + !(action->group.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool lockOnRelease = action->type == ACTION_TYPE_GROUP_LOCK &&
                                 (action->group.flags & ACTION_LOCK_ON_RELEASE);
            ಯದಿ (lockOnRelease && !isGroupLockOnReleaseSupported(format)) {
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
        } ಅನ್ಯಥಾ {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->group.group +
                       !!(action->group.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            ಗಚ್ಛ void_action;
        }
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", prefix, type, suffix);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", prefix, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_PTR_LOCK:
        args = affect_lock_text(action->btn.flags, true);
        /* fallthrough */
    ಪಕ್ಷ ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", prefix, type);
        static_assert(XKB_POINTER_BUTTON_DEFAULT < XKB_POINTER_BUTTON_MIN, "");
        ಯದಿ (action->btn.button > XKB_POINTER_BUTTON_DEFAULT &&
            action->btn.button <= XKB_POINTER_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        ಅನ್ಯಥಾ
            copy_to_buf(buf, "default");
        ಯದಿ (action->btn.ಎಣಿಕೆ) {
            ಪ್ರತಿಜ್ಞಾ(action->type != ACTION_TYPE_PTR_LOCK);
            write_buf(buf, ",count=%u", action->btn.ಎಣಿಕೆ);
        }
        ಯದಿ (args)
            write_buf(buf, "%s", args);
        write_buf(buf, ")%s", suffix);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", prefix, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.ಮೌಲ್ಯ >= 0) ? "+" : "",
                  action->dflt.ಮೌಲ್ಯ);
        write_buf(buf, ")%s", suffix);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", prefix, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_CTRL_SET:
    ಪಕ್ಷ ACTION_TYPE_CTRL_LOCK:
        write_buf(buf, "%s%s(controls=%s%s)%s", prefix, type,
                  ControlMaskText(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_LOCK
                    ? affect_lock_text(action->ctrls.flags, false)
                    : ""),
                  suffix);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_REDIRECT_ಕೀಲಿ: {
        write_buf(buf, "%s%s(", prefix, type);
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಧ್ರುವ ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        ಯದಿ (ಕೀಲಿ)
            write_buf(buf, "keycode=%s", ಕೀಲಿ_Name_Text(keymap->ctx, ಕೀಲಿ->name));
        ಯದಿ (action->redirect.affect_mods) {
            xkb_mod_mask_t mask;
            mask = (action->redirect.affect_mods & action->redirect.mods);
            ಯದಿ (mask)
                write_buf(buf, ",modifiers=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
            mask = (action->redirect.affect_mods & ~action->redirect.mods);
            ಯದಿ (mask)
                write_buf(buf, ",clearMods=%s",
                          ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, mask));
        }
        write_buf(buf, ")%s", suffix);
        ಭಙ್ಗ;
    }

    ಪಕ್ಷ ACTION_TYPE_UNKNOWN:
    ಪಕ್ಷ ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", prefix, suffix);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        ಯದಿ (format == XKB_KEYMAP_FORMAT_TEXT_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      prefix, suffix);
        ಅನ್ಯಥಾ
            write_buf(buf, "%sVoidAction()%s", prefix, suffix);
        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        ಪ್ರತಿಜ್ಞಾ(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  prefix, type, action->type, action->priv.data[0],
                  action->priv.data[1], action->priv.data[2],
                  action->priv.data[3], action->priv.data[4],
                  action->priv.data[5], action->priv.data[6],
                  suffix);
        ಭಙ್ಗ;
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಧ್ರುವ ಸಙ್ಘ xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

ಸ್ಥಿರ bool
write_actions(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, ಗಣನಾ xkb_keymap_format format,
              xkb_layout_index_t max_groups, ಸಂರಚನಾ buf *buf, ಸಂರಚನಾ buf *buf2,
              ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ, xkb_layout_index_t group)
{
    ಸ್ಥಿರ ಧ್ರುವ ಸಙ್ಘ xkb_action noAction = { .type = ACTION_TYPE_NONE };

    ಪ್ರತಿ (xkb_level_index_t level = 0; level < Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಕೀಲಿ, group);
         level++) {

        ಯದಿ (level != 0)
            copy_to_buf(buf, ", ");

        ಧ್ರುವ ಸಙ್ಘ xkb_action *actions = ರಿಕ್ತ;
        xkb_action_ಎಣಿಕೆ_t ಎಣಿಕೆ = xkb_keymap_ಕೀಲಿ_get_actions_by_level(
            keymap, ಕೀಲಿ, group, level, &actions
        );

        ಯದಿ (ಎಣಿಕೆ > 1 && format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            ಎಣಿಕೆ = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->size = 0;
        ಯದಿ (ಎಣಿಕೆ == 0) {
            ಯದಿ (!write_action(keymap, format, max_groups,
                              buf2, &noAction, ರಿಕ್ತ, ರಿಕ್ತ))
                ಪ್ರತ್ಯಾಗಮ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        ಅನ್ಯಥಾ ಯದಿ (ಎಣಿಕೆ == 1) {
            ಯದಿ (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), ರಿಕ್ತ, ರಿಕ್ತ))
                ಪ್ರತ್ಯಾಗಮ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        ಅನ್ಯಥಾ {
            copy_to_buf(buf2, "{ ");
            ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < ಎಣಿಕೆ; k++) {
                ಯದಿ (k != 0)
                    copy_to_buf(buf2, ", ");
                size_t old_size = buf2->size;
                ಯದಿ (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ರಿಕ್ತ, ರಿಕ್ತ))
                    ಪ್ರತ್ಯಾಗಮ false;
                /* Check if padding is necessary */
                ಯದಿ (buf2->size >= old_size + ACTION_PADDING)
                    ಅನುವರ್ತ;
                /* Compute and write padding, then write the action again */
                ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ padding = (ಪೂರ್ಣಾಙ್ಕ)(old_size + ACTION_PADDING - buf2->size);
                buf2->size = old_size;
                write_buf(buf2, "%*s", padding, "");
                ಯದಿ (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ರಿಕ್ತ, ರಿಕ್ತ))
                    ಪ್ರತ್ಯಾಗಮ false;
            }
            copy_to_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
write_action_defaults(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
                      ಗಣನಾ xkb_keymap_format format,
                      ಧ್ರುವ ಸಙ್ಘ xkb_action *action,
                      ಸಂರಚನಾ buf *buf)
{
    ಧ್ರುವ ವರ್ಣ *type = ActionTypeText(action->type);

#define PREFIX "\t"

    ವಿಕಲ್ಪ (action->type) {
    ಪಕ್ಷ ACTION_TYPE_MOD_SET:
    ಪಕ್ಷ ACTION_TYPE_MOD_LATCH:
    ಪಕ್ಷ ACTION_TYPE_MOD_LOCK:
        ಪ್ರತಿಜ್ಞಾ(action->mods.flags == 0);
        ಪ್ರತಿಜ್ಞಾ(action->mods.mods.mods == 0);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        ಯದಿ (action->type != ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        ಯದಿ (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, PREFIX"%s.latchOnPress = false;\n", type);
        ಯದಿ (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        ಯದಿ (action->type == ACTION_TYPE_MOD_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        ಯದಿ (isModsUnLockOnPressSupported(format))
            write_buf(buf, PREFIX"%s.unlockOnPress = false;\n", type);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_GROUP_SET:
    ಪಕ್ಷ ACTION_TYPE_GROUP_LATCH:
    ಪಕ್ಷ ACTION_TYPE_GROUP_LOCK:
        ಪ್ರತಿಜ್ಞಾ(action->group.flags == 0);
        ಪ್ರತಿಜ್ಞಾ(action->group.group == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.group = +0;\n", type);
        ಯದಿ (action->type != ACTION_TYPE_GROUP_LOCK)
            write_buf(buf, PREFIX"%s.clearLocks = false;\n", type);
        ಯದಿ (action->type == ACTION_TYPE_GROUP_LATCH)
            write_buf(buf, PREFIX"%s.latchToLock = false;\n", type);
        ಯದಿ (action->type == ACTION_TYPE_GROUP_LOCK &&
            isGroupLockOnReleaseSupported(format))
            write_buf(buf, PREFIX"%s.lockOnRelease = false;\n", type);
        ಭಙ್ಗ;


    ಪಕ್ಷ ACTION_TYPE_PTR_MOVE:
        ಪ್ರತಿಜ್ಞಾ(action->ptr.x == 0);
        ಪ್ರತಿಜ್ಞಾ(action->ptr.y == 0);
        ಪ್ರತಿಜ್ಞಾ(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.x = +0;\n", type);
        write_buf(buf, PREFIX"%s.y = +0;\n", type);
        write_buf(buf, PREFIX"%s.accel = true;\n", type);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_PTR_LOCK:
    ಪಕ್ಷ ACTION_TYPE_PTR_BUTTON:
        ಪ್ರತಿಜ್ಞಾ(action->btn.flags == 0);
        ಪ್ರತಿಜ್ಞಾ(action->btn.button == 0);
        ಪ್ರತಿಜ್ಞಾ(action->btn.ಎಣಿಕೆ == 0);
        write_buf(buf, PREFIX"%s.button = 0;\n", type);
        write_buf(buf, PREFIX"%s.count = 0;\n", type);
        ಯದಿ (action->type == ACTION_TYPE_PTR_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_PTR_DEFAULT:
        ಪ್ರತಿಜ್ಞಾ(action->dflt.flags == 0);
        ಪ್ರತಿಜ್ಞಾ(action->dflt.ಮೌಲ್ಯ == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.button = +1;\n", type);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_SWITCH_VT:
        ಪ್ರತಿಜ್ಞಾ(action->screen.flags == ACTION_SAME_SCREEN);
        ಪ್ರತಿಜ್ಞಾ(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, PREFIX"%s.screen = +0;\n", type);
        write_buf(buf, PREFIX"%s.same = true;\n", type);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_CTRL_SET:
    ಪಕ್ಷ ACTION_TYPE_CTRL_LOCK:
        ಪ್ರತಿಜ್ಞಾ(action->ctrls.flags == 0);
        ಪ್ರತಿಜ್ಞಾ(action->ctrls.ctrls == 0);
        write_buf(buf, PREFIX"%s.controls = 0;\n", type);
        ಯದಿ (action->type == ACTION_TYPE_CTRL_LOCK)
            write_buf(buf, PREFIX"%s.affect = both;\n", type);
        ಭಙ್ಗ;

    ಪಕ್ಷ ACTION_TYPE_REDIRECT_ಕೀಲಿ:
        ಪ್ರತಿಜ್ಞಾ(action->redirect.keycode == keymap->redirect_ಕೀಲಿ_auto);
        ಪ್ರತಿಜ್ಞಾ(action->redirect.affect_mods == 0);
        ಪ್ರತಿಜ್ಞಾ(action->redirect.mods == 0);
        write_buf(buf, PREFIX"%s.keycode = auto;\n", type);
        write_buf(buf, PREFIX"%s.modifiers = 0;\n", type);
        write_buf(buf, PREFIX"%s.clearMods = 0;\n", type);
        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_INTERNAL == 20 &&
                      ACTION_TYPE_INTERNAL + 1 == ACTION_TYPE_ಅಂಕೆ_ENTRIES,
                      "Missing action type");
    }

#undef PREFIX

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
write_actions_defaults(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
                       ಗಣನಾ xkb_keymap_format format, ಸಂರಚನಾ buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    ಪ್ರತಿ (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_ಅಂಕೆ_ENTRIES; a++) {
        ಧ್ರುವ ಸಙ್ಘ xkb_action * restrict ಧ್ರುವ action = &defaults.actions[a];
        ಯದಿ (!write_action_defaults(keymap, format, action, buf))
            ಪ್ರತ್ಯಾಗಮ false;
    }
    copy_to_buf(buf, "\n");

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ xkb_sym_interpret fallback_interpret = {
    .sym = XKB_ಕೀಲಿ_Void_ಚಿಹ್ನೆ,
    .required = true,
    .repeat = FALLBACK_INTERPRET_ಕೀಲಿ_REPEAT,
    .match = MATCH_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_index_t) DEFAULT_INTERPRET_VMOD,
    .ಅಂಕೆ_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ಸ್ಥಿರ bool
write_compat(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap * restrict keymap,
             ಗಣನಾ xkb_keymap_format format, xkb_layout_index_t max_groups,
             ಗಣನಾ xkb_keymap_serialize_flags flags, bool * restrict some_interp,
             ಸಂರಚನಾ buf *buf)
{
    ಧ್ರುವ bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ಧ್ರುವ bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ಮೌಲ್ಯಗಳು);
    ಧ್ರುವ bool drop_unused = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ಧ್ರುವ bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ಕೀಲಿ_ಮೌಲ್ಯಗಳು) && drop_unused);
    /* TODO: print all default values if the explicit flag is set */

    ಯದಿ (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    ಅನ್ಯಥಾ
        copy_to_buf(buf, "xkb_compatibility {\n");

    ಯದಿ (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        ಪ್ರತ್ಯಾಗಮ false;

    ಯದಿ (defaults && !write_actions_defaults(keymap, format, buf))
        ಪ್ರತ್ಯಾಗಮ false;

    copy_to_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_to_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    ಧ್ರುವ bool use_fallback_interpret =
        (!keymap->ಅಂಕೆ_sym_interprets || drop_interprets);
    ಧ್ರುವ darray_size_t ಅಂಕೆ_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->ಅಂಕೆ_sym_interprets);
    ಧ್ರುವ ಸಂರಚನಾ xkb_sym_interpret* ಧ್ರುವ sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    ಪ್ರತಿ (darray_size_t i = 0; i < ಅಂಕೆ_sym_interprets; i++) {
        ಧ್ರುವ ಸಂರಚನಾ xkb_sym_interpret *si = &sym_interprets[i];
        ಯದಿ (!si->required && drop_unused)
            ಅನುವರ್ತ;

        *some_interp = true;

        copy_to_buf(buf, "\tinterpret ");
        ಯದಿ (!si->sym) {
            copy_to_buf(buf, "Any");
        } ಅನ್ಯಥಾ ಯದಿ (pretty) {
            write_buf(buf, "%s", KeysymText(keymap->ctx, si->sym));
        } ಅನ್ಯಥಾ {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatchText(si->match),
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        ಯದಿ (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      ModIndexText(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        ಯದಿ (si->level_one_only) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } ಅನ್ಯಥಾ ಯದಿ (defaults) {
            copy_to_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        ಯದಿ (si->repeat) {
            copy_to_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } ಅನ್ಯಥಾ ಯದಿ (defaults) {
            copy_to_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        ಧ್ರುವ ಸಙ್ಘ xkb_action *action = ರಿಕ್ತ;
        xkb_action_ಎಣಿಕೆ_t action_ಎಣಿಕೆ = si->ಅಂಕೆ_actions;
        ಯದಿ (action_ಎಣಿಕೆ <= 1) {
            action = &si->a.action;
        } ಅನ್ಯಥಾ ಯದಿ (format < XKB_KEYMAP_FORMAT_TEXT_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_ಎಣಿಕೆ = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_TEXT_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        ಯದಿ (action_ಎಣಿಕೆ > 1) {
            copy_to_buf(buf, "\n\t\taction= {");
            ಧ್ರುವ ವರ್ಣ suffix[] = ", ";
            ಪ್ರತಿ (xkb_action_ಎಣಿಕೆ_t k = 0; k < si->ಅಂಕೆ_actions; k++) {
                ಯದಿ (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    ಪ್ರತ್ಯಾಗಮ false;
            }
            buf->size -= ಪರಿಮಾಣ(suffix) - 1; /* trailing comma */
            copy_to_buf(buf, "};");
            has_explicit_properties = true;
        } ಅನ್ಯಥಾ ಯದಿ (action_ಎಣಿಕೆ == 1) {
            ಯದಿ (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                ಪ್ರತ್ಯಾಗಮ false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    ಯದಿ (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        ಅಂಕೆ_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].match == fallback_interpret.match &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].ಅಂಕೆ_actions == fallback_interpret.ಅಂಕೆ_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    ಧ್ರುವ ಸಂರಚನಾ xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ಯದಿ ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            ಪ್ರತ್ಯಾಗಮ false;
        }
    }

    copy_to_buf(buf, "};\n\n");

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
write_keysyms(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
              ಸಂರಚನಾ buf *buf, ಸಂರಚನಾ buf *buf2,
              ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ, xkb_layout_index_t group,
              bool pretty, bool show_actions)
{
    ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : ಚಿಹ್ನೆ_PADDING)
        : 0;
    ಪ್ರತಿ (xkb_level_index_t level = 0; level < Xkb_ಕೀಲಿ_ಅಂಕೆ_Levels(ಕೀಲಿ, group);
         level++) {
        ಧ್ರುವ xkb_keysym_t *syms;
        ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_syms;

        ಯದಿ (level != 0)
            copy_to_buf(buf, ", ");

        ಅಂಕೆ_syms = xkb_keymap_ಕೀಲಿ_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (ಸಂರಚನಾ xkb_keymap *)keymap,
            ಕೀಲಿ->keycode, group, level, &syms
        );

        ಧ್ರುವ xkb_keysym_t no_ಚಿಹ್ನೆ = XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ;
        ಯದಿ (ಅಂಕೆ_syms == 0) {
            ಅಂಕೆ_syms = 1;
            syms = &no_ಚಿಹ್ನೆ;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        ಯದಿ (ಅಂಕೆ_syms == 1) {
            ಯದಿ (pretty || syms[0] == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                write_buf(buf, "%*s", padding, KeysymText(keymap->ctx, syms[0]));
            ಅನ್ಯಥಾ
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } ಅನ್ಯಥಾ {
            ಯದಿ (pretty) {
                buf2->size = 0;
                copy_to_buf(buf2, "{ ");
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 0; s < ಅಂಕೆ_syms; s++) {
                    ಯದಿ (s != 0)
                        copy_to_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              KeysymText(keymap->ctx, syms[s]));
                }
                copy_to_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } ಅನ್ಯಥಾ {
                copy_to_buf(buf, "{");
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ s = 0; s < ಅಂಕೆ_syms; s++) {
                    ಯದಿ (s != 0)
                        copy_to_buf(buf, ",");
                    ಯದಿ (syms[s] == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                        copy_to_buf(buf, "NoSymbol");
                    ಅನ್ಯಥಾ
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_to_buf(buf, "}");
            }
        }
    }

    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
write_ಕೀಲಿ(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
          ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_serialize_config *config,
          ಧ್ರುವ ಕೀಲಿ_name_substitutions *substitutions,
          xkb_layout_index_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          ಸಂರಚನಾ buf *buf, ಸಂರಚನಾ buf *buf2, ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ)
{
    bool simple = true;

    xkb_layout_mask_t layouts = config->layouts;
    ಯದಿ (popcount32(layouts) != keymap->ಅಂಕೆ_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_index_t l = 0;
        ಪ್ರತಿ (xkb_layout_mask_t ls = layouts; ls; ls >>= 1, l++) {
            ಯದಿ (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ಧ್ರುವ xkb_layout_index_t effective =
                    xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, l);
                ಯದಿ (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } ಅನ್ಯಥಾ {
        layouts &= ((UINT64_C(1) << ಕೀಲಿ->ಅಂಕೆ_groups) - 1);
    }
    ಧ್ರುವ xkb_layout_index_t groups_ಎಣಿಕೆ = popcount32(layouts);

    ಧ್ರುವ xkb_atom_t name = (substitutions == ರಿಕ್ತ)
        ? ಕೀಲಿ->name
        : substitute_name(substitutions, ಕೀಲಿ->name);

    ಯದಿ (pretty)
        write_buf(buf, "\tkey %-20s {", ಕೀಲಿ_Name_Text(keymap->ctx, name));
    ಅನ್ಯಥಾ
        write_buf(buf, "\tkey %s {", ಕೀಲಿ_Name_Text(keymap->ctx, name));

    ಯದಿ ((ಕೀಲಿ->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        ಯದಿ (!explicit) {
            ಪ್ರತಿ (xkb_layout_index_t group = 1; group < ಕೀಲಿ->ಅಂಕೆ_groups; group++) {
                ಯದಿ (!((UINT32_C(1) << group) & layouts))
                    ಅನುವರ್ತ;

                ಯದಿ (ಕೀಲಿ->groups[group].type != ಕೀಲಿ->groups[0].type) {
                    multi_type = true;
                    ಭಙ್ಗ;
                }
            }
        }

        ಯದಿ (multi_type) {
            xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
            ಪ್ರತಿ (xkb_layout_index_t group = 0; group < ಕೀಲಿ->ಅಂಕೆ_groups; group++) {
                ಯದಿ (!((UINT32_C(1) << group) & layouts))
                    ಅನುವರ್ತ;

                new_group++;

                ಯದಿ (!ಕೀಲಿ->groups[group].explicit_type && !explicit)
                    ಅನುವರ್ತ;

                ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type * ಧ್ರುವ type = ಕೀಲಿ->groups[group].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_group + 1);
                write_buf_string_literal(
                  buf, xkb_atom_text(keymap->ctx, type->name));
                copy_to_buf(buf, ",");
            }
        }
        ಅನ್ಯಥಾ {
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ_type * ಧ್ರುವ type = ಕೀಲಿ->groups[0].type;
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
    ಧ್ರುವ bool explicit_actions = (ಕೀಲಿ->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #define require_explicit(ಕೀಲಿ, prop,                                       \
                             ಮೌಲ್ಯ, initial_ಮೌಲ್ಯ, default_interpret_ಮೌಲ್ಯ) ( \
        /* Explicit value in the source */                                    \
        ((ಕೀಲಿ)->explicit & (prop)) ||                                         \
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
                ((ಕೀಲಿ)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (ಮೌಲ್ಯ) != (initial_ಮೌಲ್ಯ) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_ಮೌಲ್ಯ) != (default_interpret_ಮೌಲ್ಯ)                  \
            )                                                                 \
        )                                                                     \
    )

    ಯದಿ (require_explicit(ಕೀಲಿ, EXPLICIT_REPEAT, ಕೀಲಿ->repeats,
                         DEFAULT_ಕೀಲಿ_REPEAT, DEFAULT_INTERPRET_ಕೀಲಿ_REPEAT)) {
        ಯದಿ (ಕೀಲಿ->repeats)
            copy_to_buf(buf, "\n\t\trepeat= Yes,");
        ಅನ್ಯಥಾ
            copy_to_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    ಯದಿ (require_explicit(ಕೀಲಿ, EXPLICIT_VMODMAP, ಕೀಲಿ->vmodmap,
                         (xkb_mod_mask_t) DEFAULT_ಕೀಲಿ_VMODMAP,
                         (xkb_mod_mask_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  ModMaskText(keymap->ctx, MOD_BOTH, &keymap->mods,
                              ಕೀಲಿ->vmodmap));
        simple = false;
    }

    #undef require_explicit

    ವಿಕಲ್ಪ (ಕೀಲಿ->out_of_range_group_policy) {
    ಪಕ್ಷ XKB_LAYOUT_OUT_OF_RANGE_CLAMP:
        copy_to_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        ಭಙ್ಗ;

    ಪಕ್ಷ XKB_LAYOUT_OUT_OF_RANGE_REDIRECT: {
        ಯದಿ (!groups_ಎಣಿಕೆ) {
            /* No layout to redirect to: skip */
            ಭಙ್ಗ;
        }

        xkb_layout_mask_t layout_mask =
            (UINT32_C(1) << ಕೀಲಿ->out_of_range_group_ಸಂಖ್ಯೆ);
        ಯದಿ (!(layouts & layout_mask)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_MESSAGE_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     ಕೀಲಿ_Name_Text(keymap->ctx, name),
                     ಕೀಲಿ->out_of_range_group_ಸಂಖ್ಯೆ + 1);
            layout_mask = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ಧ್ರುವ xkb_layout_mask_t ಕೆಳಗಿನ = (layouts & (layout_mask - 1));
        ಧ್ರುವ xkb_layout_index_t new = popcount32(ಕೆಳಗಿನ);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        ಭಙ್ಗ;
    }

    ಪಕ್ಷ XKB_LAYOUT_OUT_OF_RANGE_WRAP:
    ಸ್ವಭಾವ:
        ಭಙ್ಗ;
    }

    ಯದಿ (ಕೀಲಿ->overlays) {
        xkb_overlay_mask_t remaining = ಕೀಲಿ->overlays;

        ಧ್ರುವ xkb_overlay_index_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_ಎಣಿಕೆ == 8, "invalid right shift");
        ಧ್ರುವ xkb_overlay_mask_t valid =
            (xkb_overlay_mask_t)((1u << overlay_max) - 1u);
        remaining &= valid;
        ಯದಿ (remaining != ಕೀಲಿ->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_INDEX,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, ಕೀಲಿ_Name_Text(keymap->ctx, name),
                     remaining, ಕೀಲಿ->overlays & ~valid);
        }

        ಯದಿ (remaining && !ಕೀಲಿ->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ಧ್ರುವ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            ಧ್ರುವ xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     ಕೀಲಿ_Name_Text(keymap->ctx, name), overlay, remaining & ~lsb);
            remaining = lsb;
        }

        ಯದಿ (remaining)
            simple = false;

        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ * ಧ್ರುವ *overlays_ಕೀಲಿಗಳು = (ಕೀಲಿ->overlays_inline)
                                                    ? &ಕೀಲಿ->overlay_ಕೀಲಿ
                                                    : ಕೀಲಿ->overlays_ಕೀಲಿಗಳು;
        ಯಾವತ್ (remaining) {
            /* isolate lowest set bit */
            ಧ್ರುವ xkb_overlay_mask_t lsb = (xkb_overlay_mask_t)(
                remaining &
                (xkb_overlay_mask_t)((unsigned)~remaining + 1u)
            );
            /* get its index */
            ಧ್ರುವ xkb_overlay_index_t overlay =
                (xkb_overlay_index_t)popcount32(lsb - 1u);
            remaining = (remaining & ~lsb);

            /* pop current value */
            ಯದಿ (ಕೀಲಿ->overlays_inline && remaining)
                PANIC_UNREACHABLE();
            ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *overlay_ಕೀಲಿ = *(overlays_ಕೀಲಿಗಳು++);

            ಧ್ರುವ xkb_atom_t overlay_ಕೀಲಿ_name = (substitutions == ರಿಕ್ತ)
                ? overlay_ಕೀಲಿ->name
                : substitute_name(substitutions, overlay_ಕೀಲಿ->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, ಕೀಲಿ_Name_Text(keymap->ctx, overlay_ಕೀಲಿ_name));
        }
    }

    ಯದಿ (groups_ಎಣಿಕೆ > 1 || explicit_actions || explicit)
        simple = false;

    ಯದಿ (simple) {
        ಯದಿ (groups_ಎಣಿಕೆ == 0) {
            /* Remove trailing comma */
            delete_last_char(buf, ',');
            ಯದಿ (buf->buf[buf->size - 1] == ',')
                buf->size--;
        } ಅನ್ಯಥಾ {
            copy_to_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_index_t group = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            group = xkb_keymap_ಕೀಲಿ_effective_layout(ಕೀಲಿ, group);
            ಯದಿ (!write_keysyms(keymap, buf, buf2, ಕೀಲಿ, group, pretty, false))
                ಪ್ರತ್ಯಾಗಮ false;
            copy_to_buf(buf, " ]");
        }
        copy_to_buf(buf, " };\n");
    }
    ಅನ್ಯಥಾ {
        xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
        ಪ್ರತಿ (xkb_layout_index_t group = 0; group < ಕೀಲಿ->ಅಂಕೆ_groups; group++) {
            ಯದಿ (!((UINT32_C(1) << group) & layouts))
                ಅನುವರ್ತ;

            new_group++;

            ಧ್ರುವ bool print_actions =
                /* Group has explicit actions */
                ಕೀಲಿ->groups[group].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (ಕೀಲಿ->groups[group].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (ಕೀಲಿ->groups[group].implicit_actions && !some_interprets);

            ಯದಿ (new_group != 0)
                copy_to_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_group + 1);

            ಯದಿ (!write_keysyms(keymap, buf, buf2, ಕೀಲಿ, group,
                               pretty, print_actions))
                ಪ್ರತ್ಯಾಗಮ false;
            copy_to_buf(buf, " ]");
            ಯದಿ (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_group + 1);
                ಯದಿ (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, ಕೀಲಿ, group))
                    ಪ್ರತ್ಯಾಗಮ false;
                copy_to_buf(buf, " ]");
            }
        }
        ಯದಿ (!groups_ಎಣಿಕೆ)
            delete_last_char(buf, ',');
        copy_to_buf(buf, "\n\t};\n");
    }

    ಪ್ರತ್ಯಾಗಮ true;
}


ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_modmap_step_1(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
    ಧ್ರುವ ModMapEntry *m1 = (ಧ್ರುವ ModMapEntry *)a;
    ಧ್ರುವ ModMapEntry *m2 = (ಧ್ರುವ ModMapEntry *)b;

    ಯದಿ (!m1->mods) {
        ಯದಿ (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            ಪ್ರತ್ಯಾಗಮ 0;
        }
        ಪ್ರತ್ಯಾಗಮ +1;
    } ಅನ್ಯಥಾ ಯದಿ (!m2->mods) {
        ಪ್ರತ್ಯಾಗಮ -1;
    }

    /* Valid only on step 1 */
    ಯದಿ (m1->ಕೀಲಿ_Code < m2->ಕೀಲಿ_Code)
        ಪ್ರತ್ಯಾಗಮ -1;
    ಅನ್ಯಥಾ ಯದಿ (m1->ಕೀಲಿ_Code > m2->ಕೀಲಿ_Code)
        ಪ್ರತ್ಯಾಗಮ +1;

    ಯದಿ (m1->mods < m2->mods)
        ಪ್ರತ್ಯಾಗಮ -1;
    ಅನ್ಯಥಾ ಯದಿ (m1->mods > m2->mods)
        ಪ್ರತ್ಯಾಗಮ +1;

    ಯದಿ (m1->have_ಚಿಹ್ನೆ < m2->have_ಚಿಹ್ನೆ)
        ಪ್ರತ್ಯಾಗಮ +1;
    ಅನ್ಯಥಾ ಯದಿ (m1->have_ಚಿಹ್ನೆ > m2->have_ಚಿಹ್ನೆ)
        ಪ್ರತ್ಯಾಗಮ -1;

    ಯದಿ (m1->have_ಚಿಹ್ನೆ) {
        ಯದಿ (m1->u.ಕೀಲಿ_Sym < m2->u.ಕೀಲಿ_Sym)
            ಪ್ರತ್ಯಾಗಮ -1;
        ಯದಿ (m1->u.ಕೀಲಿ_Sym > m2->u.ಕೀಲಿ_Sym)
            ಪ್ರತ್ಯಾಗಮ +1;
    } ಅನ್ಯಥಾ {
        ಯದಿ (m1->u.ಕೀಲಿ_Name < m2->u.ಕೀಲಿ_Name)
            ಪ್ರತ್ಯಾಗಮ -1;
        ಯದಿ (m1->u.ಕೀಲಿ_Name > m2->u.ಕೀಲಿ_Name)
            ಪ್ರತ್ಯಾಗಮ +1;
    }

    ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
cmp_modmap_step_2(ಧ್ರುವ ಶೂನ್ಯ *a, ಧ್ರುವ ಶೂನ್ಯ *b)
{
    ಧ್ರುವ ModMapEntry *m1 = (ಧ್ರುವ ModMapEntry *)a;
    ಧ್ರುವ ModMapEntry *m2 = (ಧ್ರುವ ModMapEntry *)b;

    ಯದಿ (m1->mods < m2->mods)
        ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (m1->mods > m2->mods)
        ಪ್ರತ್ಯಾಗಮ +1;

    ಯದಿ (m1->have_ಚಿಹ್ನೆ < m2->have_ಚಿಹ್ನೆ)
        ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (m1->have_ಚಿಹ್ನೆ > m2->have_ಚಿಹ್ನೆ)
        ಪ್ರತ್ಯಾಗಮ +1;

    ಯದಿ (m1->have_ಚಿಹ್ನೆ) {
        ಯದಿ (m1->u.ಕೀಲಿ_Sym < m2->u.ಕೀಲಿ_Sym)
            ಪ್ರತ್ಯಾಗಮ -1;
        ಯದಿ (m1->u.ಕೀಲಿ_Sym > m2->u.ಕೀಲಿ_Sym)
            ಪ್ರತ್ಯಾಗಮ +1;
    } ಅನ್ಯಥಾ {
        ಯದಿ (m1->ಕೀಲಿ_Code < m2->ಕೀಲಿ_Code)
            ಪ್ರತ್ಯಾಗಮ -1;
        ಯದಿ (m1->ಕೀಲಿ_Code > m2->ಕೀಲಿ_Code)
            ಪ್ರತ್ಯಾಗಮ +1;
    }

    ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ bool
write_modmaps(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap, ಗಣನಾ xkb_keymap_format format,
              ಧ್ರುವ ಕೀಲಿ_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, ಸಂರಚನಾ buf *buf)
{
    ಯದಿ (!keymap->ಅಂಕೆ_modmaps)
        ಪ್ರತ್ಯಾಗಮ true;

    darray(ModMapEntry) modmaps = darray_new();
    darray_from_ವಸ್ತುಗಳು(modmaps, keymap->modmaps, keymap->ಅಂಕೆ_modmaps);
    ಯದಿ (!darray_ವಸ್ತುಗಳು(modmaps))
        ಪ್ರತ್ಯಾಗಮ false;

    /* Sort entries by key code */
    qsort(darray_ವಸ್ತುಗಳು(modmaps), darray_size(modmaps),
          ಪರಿಮಾಣ(*darray_ವಸ್ತುಗಳು(modmaps)), &cmp_modmap_step_1);

    /* Drop disabled entries at the end */
    ಪ್ರತಿ (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ಯದಿ (darray_item(modmaps, m).mods)
            ಭಙ್ಗ;
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
    ಪ್ರತಿ (darray_size_t m = darray_size(modmaps); m-- > 0;) {
        ModMapEntry *mm = &darray_item(modmaps, m);
        ಪ್ರತಿಜ್ಞಾ(mm->mods);

        ಯದಿ (mm->ಕೀಲಿ_Code == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ಧ್ರುವ xkb_mod_mask_t core_mods = (mm->mods & MOD_REAL_MASK_ALL);
            ಯದಿ (convert_to_v1_compatibility_entry && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } ಅನ್ಯಥಾ {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            ಅನುವರ್ತ;
        }

        current_keycode = mm->ಕೀಲಿ_Code;
        ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ = Xkb_ಕೀಲಿ(keymap, mm->ಕೀಲಿ_Code);

        ಧ್ರುವ xkb_mod_mask_t core_mods = (ಕೀಲಿ->modmap & MOD_REAL_MASK_ALL);
        ಯದಿ (format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
            ((ಕೀಲಿ->modmap & (ಕೀಲಿ->modmap - 1)) || ಕೀಲಿ->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_to_v1_compatibility_entry = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_MASK,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    ಕೀಲಿ_Name_Text(keymap->ctx, ಕೀಲಿ->name), ಕೀಲಿ->modmap);
        } ಅನ್ಯಥಾ {
            /* V2 format or V1 format with a single core modifier */
            ಪ್ರತಿಜ್ಞಾ(ಕೀಲಿ->modmap);
            mm->mods = ಕೀಲಿ->modmap;
            /* Force key name */
            mm->have_ಚಿಹ್ನೆ = false;
            mm->u.ಕೀಲಿ_Name = ಕೀಲಿ->name;
            convert_to_v1_compatibility_entry = false;
        }
    }

    ಯದಿ (darray_empty(modmaps))
        ಗಚ್ಛ exit;

    /* Sort entries by modifier mask */
    qsort(darray_ವಸ್ತುಗಳು(modmaps), darray_size(modmaps),
          ಪರಿಮಾಣ(*darray_ವಸ್ತುಗಳು(modmaps)), &cmp_modmap_step_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_mask_t current_mods = 0;
    ಧ್ರುವ ModMapEntry *entry;
    darray_foreach(entry, modmaps) {
        ಯದಿ (!entry->mods)
            ಅನುವರ್ತ;

        ಯದಿ (current_mods != entry->mods) {
            ಯದಿ (current_mods) {
                copy_to_buf(buf, " };\n");
            }
            current_mods = entry->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      ModMaskText(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } ಅನ್ಯಥಾ {
            copy_to_buf(buf, ", ");
        }

        ಯದಿ (entry->have_ಚಿಹ್ನೆ) {
            ಯದಿ (pretty || entry->u.ಕೀಲಿ_Sym == XKB_ಕೀಲಿ_No_ಚಿಹ್ನೆ)
                write_buf(buf, "%s", KeysymText(keymap->ctx, entry->u.ಕೀಲಿ_Sym));
            ಅನ್ಯಥಾ
                write_buf(buf, "0x%"PRIx32, entry->u.ಕೀಲಿ_Sym);
        } ಅನ್ಯಥಾ {
            ಧ್ರುವ xkb_atom_t name = (substitutions == ರಿಕ್ತ)
                ? entry->u.ಕೀಲಿ_Name
                : substitute_name(substitutions, entry->u.ಕೀಲಿ_Name);
            write_buf(buf, "%s", ಕೀಲಿ_Name_Text(keymap->ctx, name));
        }
    }
    ಯದಿ (current_mods) {
        copy_to_buf(buf, " };\n");
    }

exit:
    darray_free(modmaps);
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ bool
write_symbols(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
              ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_serialize_config *config,
              ಧ್ರುವ ಕೀಲಿ_name_substitutions *substitutions,
              xkb_layout_index_t max_groups, bool some_interp,
              ಸಂರಚನಾ buf *buf)
{
    ಧ್ರುವ bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ಧ್ರುವ bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ಮೌಲ್ಯಗಳು);
    ಧ್ರುವ bool explicit_ಕೀಲಿ_ಮೌಲ್ಯಗಳು =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ಕೀಲಿ_ಮೌಲ್ಯಗಳು);
    ಧ್ರುವ bool drop_unused = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_UNUSED);
    ಧ್ರುವ bool drop_interprets = (explicit_ಕೀಲಿ_ಮೌಲ್ಯಗಳು && drop_unused);
    /* TODO: print all default values if explicit flag is set */

    ಯದಿ (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    ಅನ್ಯಥಾ
        copy_to_buf(buf, "xkb_symbols {\n");

    bool has_group_names = false;
    xkb_layout_index_t new_group = (xkb_layout_index_t)-1;
    ಪ್ರತಿ (xkb_layout_index_t group = 0; group < keymap->ಅಂಕೆ_group_names; group++) {
        ಯದಿ (!((UINT32_C(1) << group) & config->layouts))
            ಅನುವರ್ತ;

        new_group++;

        ಯದಿ (keymap->group_names[group]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_group + 1);
            write_buf_string_literal(
                buf, xkb_atom_text(keymap->ctx, keymap->group_names[group]));
            copy_to_buf(buf, ";\n");
            has_group_names = true;
        }
    }
    ಯದಿ (has_group_names)
        copy_to_buf(buf, "\n");

    ಯದಿ (defaults && !write_actions_defaults(keymap, config->format, buf))
        ಪ್ರತ್ಯಾಗಮ false;

    ಸಂರಚನಾ buf buf2 = { ರಿಕ್ತ, 0, 0 };
    ಧ್ರುವ ಸಂರಚನಾ xkb_ಕೀಲಿ *ಕೀಲಿ;
    xkb_ಕೀಲಿಗಳು_foreach(ಕೀಲಿ, keymap) {
        /* Skip keys with no explicit values */
        ಯದಿ (ಕೀಲಿ->explicit &&
            !write_ಕೀಲಿ(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_ಕೀಲಿ_ಮೌಲ್ಯಗಳು, pretty, buf,
                       &buf2, ಕೀಲಿ))
        {
            ಮುಕ್ತ(buf2.buf);
            ಪ್ರತ್ಯಾಗಮ false;
        }
    }
    ಮುಕ್ತ(buf2.buf);

    ಯದಿ (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        ಪ್ರತ್ಯಾಗಮ false;

    copy_to_buf(buf, "};\n\n");
    ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಗಣನಾ xkb_status
write_keymap(ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
             ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_serialize_config *config,
             ಸಂರಚನಾ buf *buf, ಸಂರಚನಾ xkb_keymap_serialize_result *result)
{
    ಕೀಲಿ_name_substitutions substitutions = darray_new();
    ಯದಿ (config->format == XKB_KEYMAP_FORMAT_TEXT_V1 &&
        !rename_long_ಕೀಲಿಗಳು(keymap, &substitutions))
    {
        ಪ್ರತ್ಯಾಗಮ false;
    }

    ಧ್ರುವ ಕೀಲಿ_name_substitutions * ಧ್ರುವ substitutions_ptr =
        (darray_empty(substitutions)) ? ರಿಕ್ತ : &substitutions;

    ಧ್ರುವ xkb_layout_index_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    ಧ್ರುವ bool ok = (
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
    ಪ್ರತ್ಯಾಗಮ ok ? XKB_SUCCESS : -1;
}

ಗಣನಾ xkb_status
text_v1_keymap_serialize(
        ಧ್ರುವ ಸಂರಚನಾ xkb_keymap *keymap,
        ಧ್ರುವ ಸಂರಚನಾ xkb_keymap_serialize_config *config,
        ಸಂರಚನಾ xkb_keymap_serialize_result *result
)
{
    ಸಂರಚನಾ buf buf = { ರಿಕ್ತ, 0, 0 };

    ಧ್ರುವ ಗಣನಾ xkb_status status =
        write_keymap(keymap, config, &buf, result);
    ಯದಿ (status != XKB_SUCCESS) {
        ಮುಕ್ತ(buf.buf);
        result->serialized = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ status;
    }

    result->serialized = buf.buf;
    result->length = buf.size + 1;
    result->layouts = config->layouts;

    ಪ್ರತ್ಯಾಗಮ XKB_SUCCESS;
}
