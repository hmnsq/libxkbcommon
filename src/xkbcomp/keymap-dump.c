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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-status.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon-keysyms.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcommon/xkbcommon.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "atom.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "action.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "context.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "darray.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "keymap.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "messages-codes.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "text.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "util-numbers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xkbcomp-priv.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUF_CHUNK_ਮਾਪ 4096

ਬਣਤਰ buf {
    ਅੱਖਰ *buf;
    ਮਾਪ_t ਮਾਪ;
    ਮਾਪ_t alloc;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xkb_abs(n) _Generic((n),                \
                            ਪੂਰਨ: ਨਿਰਪੇਖ,           \
                            ਲੰਮਾ ਪੂਰਨ: labs,     \
                            ਮੂਲ: llabs )((n))

ਸਥਿਰ bool
do_realloc(ਬਣਤਰ buf *buf, ਮਾਪ_t at_ਘੱਟ_ਤੋਂ_ਘੱਟ)
{
    buf->alloc += BUF_CHUNK_ਮਾਪ;
    ਜੇ (at_ਘੱਟ_ਤੋਂ_ਘੱਟ >= BUF_CHUNK_ਮਾਪ)
        buf->alloc += at_ਘੱਟ_ਤੋਂ_ਘੱਟ;

    ਅੱਖਰ *ਅਚਲ new = realloc(buf->buf, buf->alloc);
    ਜੇ (!new)
        ਵਾਪਸ false;

    buf->buf = new;
    ਵਾਪਸ true;
}

ATTR_PRINTF(2, 3) ਸਥਿਰ bool
check_write_buf(ਬਣਤਰ buf *buf, ਅਚਲ ਅੱਖਰ *fmt, ...)
{
    va_list ਦਲੀਲਾਂ;
    ਪੂਰਨ printed;
    ਮਾਪ_t ਉਪਲਬਧ;

    ਉਪਲਬਧ = buf->alloc - buf->ਮਾਪ;
    /* Try to write to the buffer and get the required min size (without the
     * final null byte).
     * C11 states that if `available` is 0, then the pointer may be null. */
    ਪ੍ਰਤਿਜ੍ਞਾ(buf->buf || ਉਪਲਬਧ == 0);
    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
    printed = vsnprintf(buf->buf ? buf->buf + buf->ਮਾਪ : buf->buf, ਉਪਲਬਧ,
                        fmt, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

    ਜੇ (printed < 0) {
        /* Some error */
        ਜਾਓ err;
    } ਨਹੀਂਤਾਂ ਜੇ ((ਮਾਪ_t)printed >= ਉਪਲਬਧ) {
        /* Not enough space: need to realloc */
        ਜੇ (!do_realloc(buf, (ਮਾਪ_t)printed + 1))
            ਜਾਓ err;

        /* The buffer has enough space now. */
        ਉਪਲਬਧ = buf->alloc - buf->ਮਾਪ;
        ਪ੍ਰਤਿਜ੍ਞਾ(buf->buf && (ਮਾਪ_t) printed < ਉਪਲਬਧ);
        va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, fmt);
        printed = vsnprintf(buf->buf + buf->ਮਾਪ, ਉਪਲਬਧ, fmt, ਦਲੀਲਾਂ);
        va_ਸਿਰਾ(ਦਲੀਲਾਂ);

        ਜੇ (printed < 0 || (ਮਾਪ_t) printed >= ਉਪਲਬਧ)
            ਜਾਓ err;
    }

    buf->ਮਾਪ += (ਮਾਪ_t)printed;
    ਵਾਪਸ true;

err:
    ਮੁਕਤ(buf->buf);
    buf->buf = ਨਿਰਾ;
    ਵਾਪਸ false;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ write_buf(buf, ...) ਕਰੋ { \
    ਜੇ (!check_write_buf(buf, __VA_ARGS__)) \
        ਵਾਪਸ false; \
} ਜਦੋਂ (0)

ਸਥਿਰ bool
check_copy_ਮੰਜ਼ਿਲ_buf(ਬਣਤਰ buf *buf, ਅਚਲ ਅੱਖਰ* ਸਰੋਤ, ਮਾਪ_t len)
{
    ਜੇ (len == 0)
        ਵਾਪਸ true;

    ਅਚਲ ਮਾਪ_t ਉਪਲਬਧ = buf->alloc - buf->ਮਾਪ;
    ਜੇ (len >= ਉਪਲਬਧ &&
        /* len + 1 (terminating NULL) */
        !do_realloc(buf, len + 1))
    {
        ਮੁਕਤ(buf->buf);
        buf->buf = ਨਿਰਾ;
        ਵਾਪਸ false;
    }

    ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf->buf + buf->ਮਾਪ, ਸਰੋਤ, len);
    buf->ਮਾਪ += len;
    /* Append NULL byte */
    buf->buf[buf->ਮਾਪ] = '\0';
    ਵਾਪਸ true;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ copy_ਮੰਜ਼ਿਲ_buf_len(buf, ਸਰੋਤ, len) ਕਰੋ { \
    ਜੇ (!check_copy_ਮੰਜ਼ਿਲ_buf(buf, ਸਰੋਤ, len)) \
        ਵਾਪਸ false; \
} ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ copy_ਮੰਜ਼ਿਲ_buf(buf, ਸਰੋਤ) \
    copy_ਮੰਜ਼ਿਲ_buf_len(buf, ਸਰੋਤ, ਆਕਾਰ(ਸਰੋਤ) - 1)

ਸਥਿਰ bool
check_write_string_literal(ਬਣਤਰ buf *buf, ਅਚਲ ਅੱਖਰ* string)
{
    copy_ਮੰਜ਼ਿਲ_buf(buf, "\"");

    /* Write chunks, separated by characters requiring escape sequence */
    ਮਾਪ_t pending_ਸ਼ੁਰੂ = 0;
    ਮਾਪ_t current = 0;
    ਅੱਖਰ escape_ਬਫਰ[] = {'\\', '0', '0', '0', '0'};
    ਲਈ (; string[current] != '\0'; current++) {
        uint8_t escape_len;
        ਚੋਣ (string[current]) {
        ਹਾਲਤ '\n':
            /* `\n` would break strings */
            escape_ਬਫਰ[1] = 'n';
            escape_len = 2;
            ਰੋਕੋ;
        ਹਾਲਤ '\\':
            /* `\` would create escape sequences */
            escape_ਬਫਰ[1] = '\\';
            escape_len = 2;
            ਰੋਕੋ;
        ਮੂਲ:
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
            ਜੇ (unlikely((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)string[current] < 0x20U ||
                         string[current] == '"')) {
                escape_ਬਫਰ[1] = '0';
                escape_ਬਫਰ[2] = '0';
                escape_ਬਫਰ[3] = (ਅੱਖਰ)('0' + (string[current] >> 3));
                escape_ਬਫਰ[4] = (ਅੱਖਰ)('0' + (string[current] & 0x7));
                escape_len = 5;
            } ਨਹੀਂਤਾਂ {
                /* Expand the chunk */
                ਜਾਰੀ;
            }
        }
        /* Write pending chunk */
        ਪ੍ਰਤਿਜ੍ਞਾ(current >= pending_ਸ਼ੁਰੂ);
        copy_ਮੰਜ਼ਿਲ_buf_len(buf, string + pending_ਸ਼ੁਰੂ, current - pending_ਸ਼ੁਰੂ);
        /* Write escape sequence */
        copy_ਮੰਜ਼ਿਲ_buf_len(buf, escape_ਬਫਰ, escape_len);
        pending_ਸ਼ੁਰੂ = current + 1;
    }
    /* Write remaining chunk */
    ਪ੍ਰਤਿਜ੍ਞਾ(current >= pending_ਸ਼ੁਰੂ);
    copy_ਮੰਜ਼ਿਲ_buf_len(buf, string + pending_ਸ਼ੁਰੂ, current - pending_ਸ਼ੁਰੂ);

    copy_ਮੰਜ਼ਿਲ_buf(buf, "\"");
    ਵਾਪਸ true;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ write_buf_string_literal(buf, string) ਕਰੋ { \
    ਜੇ (!check_write_string_literal(buf, string)) \
        ਵਾਪਸ false; \
} ਜਦੋਂ (0)

ਸਥਿਰ ਖਾਲੀ
delete_ਆਖਰੀ_char(ਬਣਤਰ buf *buf, ਅੱਖਰ c)
{
    ਜੇ (buf->ਮਾਪ && buf->buf[buf->ਮਾਪ - 1] == c)
        buf->buf[--buf->ਮਾਪ] = '\0';
}

/** Entry for a name substituion LUT */
ਬਣਤਰ ਕੁੰਜੀ_name_substitution {
    xkb_atom_t ਸਰੋਤ;
    xkb_atom_t ਨਿਸ਼ਾਨਾ;
    /** Used for intuitive renaming */
    xkb_keycode_t keycode;
};

/** Compare 2 key name substitution entries by they original name (atom) */
ਸਥਿਰ ਪੂਰਨ
cmp_ਕੁੰਜੀ_name_substitution(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
    ਅਚਲ xkb_atom_t s1 = ((ਬਣਤਰ ਕੁੰਜੀ_name_substitution *) a)->ਸਰੋਤ;
    ਅਚਲ xkb_atom_t s2 = ((ਬਣਤਰ ਕੁੰਜੀ_name_substitution *) b)->ਸਰੋਤ;
    ਵਾਪਸ (s1 < s2) ? -1 : ((s1 > s2) ? 1 : 0);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ darray(ਬਣਤਰ ਕੁੰਜੀ_name_substitution) ਕੁੰਜੀ_name_substitutions;

/**
 * Rename keys that are not compatible with the original XKB protocol,
 * i.e. more that 4 bytes. The new name will be formed as prefix + 3-digit
 * hexadecimal number.
 */
ਸਥਿਰ bool
rename_long_ਕੁੰਜੀਆਂ(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
                 ਕੁੰਜੀ_name_substitutions *substitutions)
{
    /*
     * Prefixes we want to use for the new names: ASCII range `!`..`0`.
     * They are really unlikely to be used in usual keymaps.
     * NULL denotes an unavailable prefix.
     */
    ਗਿਣਤੀ {
        ਪਹਿਲਾ = '!',
        ਆਖਰੀ = '0',
        invalid = '\0'
    };
    ਅੱਖਰ prefixes[] = {
        ਪਹਿਲਾ, invalid /* skip ‘"’, which would look buggy otherwise */,
        '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ਆਖਰੀ
    };
    static_assert(ਆਕਾਰ(prefixes) == ਆਖਰੀ - ਪਹਿਲਾ + 1, "invalid range");

    /*
     * Traverse all the keys and aliases:
     * - Collect items with incompatible names.
     * - Remove prefixes that are already used in valid 4-char names.
     *   This ensures no name conflict will occur with the new names, without
     *   requiring an extensive check.
     */

    /* Check key names */
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;
    xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap) {
        ਜੇ (ਕੁੰਜੀ->name == XKB_ATOM_NONE)
            ਜਾਰੀ;

        ਅਚਲ ਅੱਖਰ * ਅਚਲ name = xkb_atom_ਪਾਠ(keymap->ctx, ਕੁੰਜੀ->name);
        ਅਚਲ ਮਾਪ_t len = strlen_ਸੁਰੱਖਿਅਤ(name);
        ਜੇ (len > 4) {
            /* Key name too long: append key, pending for renaming */
            darray_append(*substitutions, (ਬਣਤਰ ਕੁੰਜੀ_name_substitution) {
                .ਸਰੋਤ = ਕੁੰਜੀ->name,
                .keycode = ਕੁੰਜੀ->keycode,
                .ਨਿਸ਼ਾਨਾ = ਕੁੰਜੀ->name
            });
        } ਨਹੀਂਤਾਂ ਜੇ (len == 4 && name[0] >= ਪਹਿਲਾ && name[0] <= ਆਖਰੀ) {
            /* Potential name conflict: deactivate this prefix */
            prefixes[name[0] - ਪਹਿਲਾ] = invalid;
        }
    }

    /* Check aliases */
    ਲਈ (darray_ਮਾਪ_t a = 0; a < keymap->ਅੰਕ_ਕੁੰਜੀ_aliases; a++) {
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_alias * ਅਚਲ alias = &keymap->ਕੁੰਜੀ_aliases[a];
        ਅਚਲ ਅੱਖਰ * ਅਚਲ name = xkb_atom_ਪਾਠ(keymap->ctx, alias->alias);
        ਅਚਲ ਮਾਪ_t len = strlen_ਸੁਰੱਖਿਅਤ(name);
        ਜੇ (len > 4) {
            /* Alias name too long: append alias, pending for renaming */
            darray_append(*substitutions, (ਬਣਤਰ ਕੁੰਜੀ_name_substitution) {
                .ਸਰੋਤ = alias->alias,
                .keycode = XKB_KEYCODE_INVALID, /* Too expensive to lookup */
                .ਨਿਸ਼ਾਨਾ = alias->alias
            });
        } ਨਹੀਂਤਾਂ ਜੇ (len == 4 && name[0] >= ਪਹਿਲਾ && name[0] <= ਆਖਰੀ) {
            /* Potential conflict with names substitutes: deactivate prefix */
            prefixes[name[0] - ਪਹਿਲਾ] = invalid;
        }
    }

    ਜੇ darray_ਖਾਲੀ_ਥਾਵਾਂ(*substitutions) {
        /* Nothing to do */
        ਵਾਪਸ true;
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
    ਅੱਖਰ *ਅਗੇਤਰ = prefixes;
    ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ_max = prefixes + ਆਕਾਰ(prefixes) - 1;
    ਜਦੋਂ (*ਅਗੇਤਰ == invalid && ਅਗੇਤਰ < ਅਗੇਤਰ_max)
        ਅਗੇਤਰ++;

    /* Keycode prefix: prefer ‘0’ if available */
    ਅੱਖਰ keycode_ਅਗੇਤਰ;
    ਜੇ (prefixes[ਆਖਰੀ - ਪਹਿਲਾ] == '0') {
        /* ‘0’ is available: use it then mark it unavailable for further use */
        keycode_ਅਗੇਤਰ = ਆਖਰੀ;
        prefixes[ਆਖਰੀ - ਪਹਿਲਾ] = invalid;
    } ਨਹੀਂਤਾਂ {
        /* ‘0’ is unavailable: use first available prefix */
        keycode_ਅਗੇਤਰ = *ਅਗੇਤਰ;
        ਜੇ (unlikely(keycode_ਅਗੇਤਰ == invalid)) {
            /*
             * Abort: no prefix available (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            darray_free(*substitutions);
            ਵਾਪਸ true;
        }
        /* Update current prefix */
        *ਅਗੇਤਰ = invalid;
        ਜਦੋਂ (*ਅਗੇਤਰ == invalid && ਅਗੇਤਰ < ਅਗੇਤਰ_max)
            ਅਗੇਤਰ++;
    }

    /* Proceed renaming */
    uint16_t ਅਗਲਾ_id = 1;
    ਬਣਤਰ ਕੁੰਜੀ_name_substitution *sub;
    ਅੱਖਰ buf[6] = {0}; /* 5 would suffice, but generates a compiler warning */
    darray_foreach(sub, *substitutions) {
        static_assert(XKB_KEYCODE_INVALID > 0xfff, "Invalid keys filtered out");
        ਜੇ (sub->keycode <= 0xfff) {
            /*
             * Try nice formatting with the hexadecimal keycode.
             * We use lower case to reduce possible conflict because the
             * traditional key names are upper-cased.
             */
            snprintf(buf, ਆਕਾਰ(buf), "%c%03"PRIx32,
                     keycode_ਅਗੇਤਰ, sub->keycode);
            ਅਚਲ xkb_atom_t ਨਿਸ਼ਾਨਾ = xkb_atom_intern(keymap->ctx,
                                                      buf, ਆਕਾਰ(buf) - 2);
            ਜੇ (ਨਿਸ਼ਾਨਾ == XKB_ATOM_NONE)
                ਜਾਓ error;
            sub->ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ;
        } ਨਹੀਂਤਾਂ ਜੇ (*ਅਗੇਤਰ != invalid) {
            /* Cannot use keycode for formatting: generate next arbitrary ID */
            snprintf(buf, ਆਕਾਰ(buf), "%c%03"PRIx16, *ਅਗੇਤਰ, ਅਗਲਾ_id);
            ਅਚਲ xkb_atom_t ਨਿਸ਼ਾਨਾ = xkb_atom_intern(keymap->ctx,
                                                      buf, ਆਕਾਰ(buf) - 2);
            ਜੇ (ਨਿਸ਼ਾਨਾ == XKB_ATOM_NONE)
                ਜਾਓ error;
            sub->ਨਿਸ਼ਾਨਾ = ਨਿਸ਼ਾਨਾ;
            ਜੇ (++ਅਗਲਾ_id > 0xfff) {
                *ਅਗੇਤਰ = invalid;
                /* Get next available prefix */
                ਜਦੋਂ (*ਅਗੇਤਰ == invalid && ਅਗੇਤਰ < ਅਗੇਤਰ_max)
                    ਅਗੇਤਰ++;
                /* Reset counter */
                ਅਗਲਾ_id = 1;
            }
        } ਨਹੀਂਤਾਂ {
            /*
             * No prefix available: skip substitution (very unlikely).
             * Do not error though: Wayland clients can still parse the keymap.
             */
            sub->ਨਿਸ਼ਾਨਾ = sub->ਸਰੋਤ;
        }
    }

    /* Now sort by the source names so we can do a binary search */
    qsort(darray_ਚੀਜ਼ਾਂ(*substitutions), darray_ਮਾਪ(*substitutions),
          ਆਕਾਰ(*darray_ਚੀਜ਼ਾਂ(*substitutions)), &cmp_ਕੁੰਜੀ_name_substitution);

    ਵਾਪਸ true;

error:
    darray_free(*substitutions);
    /* There was a memory error */
    log_err(keymap->ctx, XKB_ERROR_ਵੰਡ_FAILURE_2,
            "Cannot allocate key name substitution\n");
    ਵਾਪਸ false;
}

/** Substitute a key name with a valid X11 key name */
ਸਥਿਰ xkb_atom_t
substitute_name(ਅਚਲ ਕੁੰਜੀ_name_substitutions *substitutions, xkb_atom_t name)
{
    /*ਦੁਆਧਾਰੀ ਖੋਜ*/
    darray_ਮਾਪ_t ਪਹਿਲਾ = 0;
    darray_ਮਾਪ_t ਆਖਰੀ = darray_ਮਾਪ(*substitutions);

    ਜਦੋਂ (ਆਖਰੀ > ਪਹਿਲਾ) {
        darray_ਮਾਪ_t mid = ਪਹਿਲਾ + (ਆਖਰੀ - 1 - ਪਹਿਲਾ) / 2;
        ਅਚਲ xkb_atom_t ਸਰੋਤ = darray_ਵਸਤੂ(*substitutions, mid).ਸਰੋਤ;
        ਜੇ (ਸਰੋਤ < name)
            ਪਹਿਲਾ = mid + 1;
        ਨਹੀਂਤਾਂ ਜੇ (ਸਰੋਤ > name)
            ਆਖਰੀ = mid;
        ਨਹੀਂਤਾਂ /* found it */
            ਵਾਪਸ darray_ਵਸਤੂ(*substitutions, mid).ਨਿਸ਼ਾਨਾ;
    }

    /* No name substitution: return original name */
    ਵਾਪਸ name;
}

ਸਥਿਰ bool
write_vmods(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, ਗਿਣਤੀ xkb_keymap_format format,
            bool explicit, ਬਣਤਰ buf *buf)
{
    ਅਚਲ ਬਣਤਰ xkb_mod *mod;
    xkb_mod_ਸੂਚਕਾਂਕ_t vmod = 0;
    bool has_some = false;

    xkb_vmods_enumerate(vmod, mod, &keymap->mods) {
        ਜੇ (!has_some) {
            copy_ਮੰਜ਼ਿਲ_buf(buf, "\tvirtual_modifiers ");
            has_some = true;
        } ਨਹੀਂਤਾਂ {
            copy_ਮੰਜ਼ਿਲ_buf(buf, ",");
        }
        write_buf(buf, "%s", xkb_atom_ਪਾਠ(keymap->ctx, mod->name));

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
        ਅਚਲ bool auto_canonical_mods = (format >= XKB_KEYMAP_FORMAT_ਪਾਠ_V2);

        ਜੇ ((keymap->mods.explicit_vmods & (UINT32_C(1) << vmod) ||
             explicit) &&
            (auto_canonical_mods || mod->mapping != 0)) {
            /*
             * Explicit non-default mapping
             * NOTE: we can only pretty-print *real* modifiers in this context.
             */
            write_buf(buf, "=%s",
                      Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_REAL, &keymap->mods,
                                  mod->mapping));
        }
    }

    ਜੇ (has_some)
        copy_ਮੰਜ਼ਿਲ_buf(buf, ";\n\n");

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
write_keycodes(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
               ਅਚਲ ਕੁੰਜੀ_name_substitutions *substitutions,
               ਗਿਣਤੀ xkb_keymap_serialize_flags flags, ਬਣਤਰ buf *buf)
{
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;
    xkb_led_ਸੂਚਕਾਂਕ_t idx;
    ਅਚਲ ਬਣਤਰ xkb_led *led;

    ਅਚਲ bool pretty = !!(flags & XKB_KEYMAP_SERIALIZE_PRETTY);

    ਜੇ (keymap->keycodes_section_name)
        write_buf(buf, "xkb_keycodes \"%s\" {\n",
                  keymap->keycodes_section_name);
    ਨਹੀਂਤਾਂ
        copy_ਮੰਜ਼ਿਲ_buf(buf, "xkb_keycodes {\n");

    /* NOTE: there is no support for default values */

    /* xkbcomp and X11 really want to see keymaps with a minimum of 8, and
     * a maximum of at least 255, else XWayland really starts hating life.
     * If this is a problem and people really need strictly bounded keymaps,
     * we should probably control this with a flag. */
    write_buf(buf, "\tminimum = %"PRIu32";\n", MIN(keymap->min_ਕੁੰਜੀ_ਕੋਡ, 8));
    write_buf(buf, "\tmaximum = %"PRIu32";\n", MAX(keymap->max_ਕੁੰਜੀ_ਕੋਡ, 255));

    xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap) {
        ਜੇ (ਕੁੰਜੀ->name == XKB_ATOM_NONE)
            ਜਾਰੀ;

        ਅਚਲ xkb_atom_t name = (substitutions == ਨਿਰਾ)
            ? ਕੁੰਜੀ->name
            : substitute_name(substitutions, ਕੁੰਜੀ->name);
        ਜੇ (pretty)
            write_buf(buf, "\t%-20s", ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, name));
        ਨਹੀਂਤਾਂ
            write_buf(buf, "\t%s", ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, name));
        write_buf(buf, " = %"PRIu32";\n", ਕੁੰਜੀ->keycode);
    }

    xkb_leds_enumerate(idx, led, keymap)
        ਜੇ (led->name != XKB_ATOM_NONE) {
            write_buf(buf, "\tindicator %"PRIu32" = ", idx + 1);
            write_buf_string_literal(buf, xkb_atom_ਪਾਠ(keymap->ctx, led->name));
            copy_ਮੰਜ਼ਿਲ_buf(buf, ";\n");
        }

    ਲਈ (darray_ਮਾਪ_t i = 0; i < keymap->ਅੰਕ_ਕੁੰਜੀ_aliases; i++) {
        ਅਚਲ xkb_atom_t alias = (substitutions == ਨਿਰਾ)
            ? keymap->ਕੁੰਜੀ_aliases[i].alias
            : substitute_name(substitutions, keymap->ਕੁੰਜੀ_aliases[i].alias);
        ਅਚਲ xkb_atom_t real = (substitutions == ਨਿਰਾ)
            ? keymap->ਕੁੰਜੀ_aliases[i].real
            : substitute_name(substitutions, keymap->ਕੁੰਜੀ_aliases[i].real);

        ਜੇ (pretty)
            write_buf(buf, "\talias %-14s",
                      ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, alias));
        ਨਹੀਂਤਾਂ
            write_buf(buf, "\talias %s",
                      ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, alias));
        write_buf(buf, " = %s;\n",
                  ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, real));
    }

    copy_ਮੰਜ਼ਿਲ_buf(buf, "};\n\n");
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
write_types(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, ਗਿਣਤੀ xkb_keymap_format format,
            ਗਿਣਤੀ xkb_keymap_serialize_flags flags, ਬਣਤਰ buf *buf)
{
    ਜੇ (keymap->types_section_name)
        write_buf(buf, "xkb_types \"%s\" {\n",
                  keymap->types_section_name);
    ਨਹੀਂਤਾਂ
        copy_ਮੰਜ਼ਿਲ_buf(buf, "xkb_types {\n");

    /* NOTE: support for default values has been removed */

    ਜੇ (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        ਵਾਪਸ false;

    ਅਚਲ bool drop_ਅਣਵਰਤਿਆ = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_ਅਣਵਰਤਿਆ);
    ਲਈ (darray_ਮਾਪ_t i = 0; i < keymap->ਅੰਕ_types; i++) {
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type * ਅਚਲ type = &keymap->types[i];
        ਜੇ (!type->required && drop_ਅਣਵਰਤਿਆ)
            ਜਾਰੀ;

        copy_ਮੰਜ਼ਿਲ_buf(buf, "\ttype ");
        write_buf_string_literal(buf, xkb_atom_ਪਾਠ(keymap->ctx, type->name));
        copy_ਮੰਜ਼ਿਲ_buf(buf, " {\n");

        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_BOTH, &keymap->mods,
                              type->mods.mods));

        ਲਈ (darray_ਮਾਪ_t j = 0; j < type->ਅੰਕ_entries; j++) {
            ਅਚਲ ਅੱਖਰ *str;
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ = &type->entries[j];

            /*
             * Printing level 1 entries is redundant, it's the default,
             * unless there's preserve info.
             */
            ਜੇ (ਇੰਦਰਾਜ->level == 0 && ਇੰਦਰਾਜ->preserve.mods == 0 &&
                !(flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ਮੁੱਲਾਂ))
                ਜਾਰੀ;

            str = Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_BOTH, &keymap->mods,
                              ਇੰਦਰਾਜ->mods.mods);
            write_buf(buf, "\t\tmap[%s]= %"PRIu32";\n",
                      str, ਇੰਦਰਾਜ->level + 1);

            ਜੇ (ਇੰਦਰਾਜ->preserve.mods)
                write_buf(buf, "\t\tpreserve[%s]= %s;\n",
                          str, Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_BOTH, &keymap->mods,
                                           ਇੰਦਰਾਜ->preserve.mods));
        }

        ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t n = 0; n < type->ਅੰਕ_level_ਨਾਂ; n++)
            ਜੇ (type->level_ਨਾਂ[n]) {
                write_buf(buf, "\t\tlevel_name[%"PRIu32"]= ", n + 1);
                write_buf_string_literal(
                    buf, xkb_atom_ਪਾਠ(keymap->ctx, type->level_ਨਾਂ[n]));
                copy_ਮੰਜ਼ਿਲ_buf(buf, ";\n");
            }

        copy_ਮੰਜ਼ਿਲ_buf(buf, "\t};\n");
    }

    copy_ਮੰਜ਼ਿਲ_buf(buf, "};\n\n");
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
write_led_map(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, ਗਿਣਤੀ xkb_keymap_format format,
              bool defaults, ਬਣਤਰ buf *buf, ਅਚਲ ਬਣਤਰ xkb_led *led)
{
    copy_ਮੰਜ਼ਿਲ_buf(buf, "\tindicator ");
    write_buf_string_literal(buf, xkb_atom_ਪਾਠ(keymap->ctx, led->name));
    copy_ਮੰਜ਼ਿਲ_buf(buf, " {\n");

    ਜੇ (led->which_groups) {
        ਜੇ (led->which_groups != XKB_STATE_LAYOUT_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichGroupState= %s;\n",
                      Led_State_ਨਕਾਬ_ਪਾਠ(keymap->ctx, ਸਮੂਹ_Component_ਨਕਾਬ_ਨਾਂ,
                                       led->which_groups));
        }
        write_buf(buf, "\t\tgroups= 0x%02"PRIx32";\n",
                  led->groups);
    }

    ਜੇ (led->which_mods) {
        ਜੇ (led->which_mods != XKB_STATE_MODS_EFFECTIVE || defaults) {
            write_buf(buf, "\t\twhichModState= %s;\n",
                      Led_State_ਨਕਾਬ_ਪਾਠ(keymap->ctx, mod_Component_ਨਕਾਬ_ਨਾਂ,
                                       led->which_mods));
        }
        write_buf(buf, "\t\tmodifiers= %s;\n",
                  Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_BOTH, &keymap->mods,
                              led->mods.mods));
    }

    ਜੇ (led->ctrls) {
        write_buf(buf, "\t\tcontrols= %s;\n",
                  Control_ਨਕਾਬ_ਪਾਠ(keymap->ctx, format, led->ctrls));
    }

    copy_ਮੰਜ਼ਿਲ_buf(buf, "\t};\n");
    ਵਾਪਸ true;
}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
affect_ਤਾਲਾ_ਪਾਠ(ਗਿਣਤੀ xkb_action_flags flags, bool show_both)
{
    ਚੋਣ (flags & (ACTION_ਤਾਲਾ_NO_ਤਾਲਾ | ACTION_ਤਾਲਾ_NO_UNLOCK)) {
    ਹਾਲਤ 0:
        ਵਾਪਸ show_both ? ",affect=both" : "";
    ਹਾਲਤ ACTION_ਤਾਲਾ_NO_UNLOCK:
        ਵਾਪਸ ",affect=lock";
    ਹਾਲਤ ACTION_ਤਾਲਾ_NO_ਤਾਲਾ:
        ਵਾਪਸ ",affect=unlock";
    ਹਾਲਤ ACTION_ਤਾਲਾ_NO_ਤਾਲਾ | ACTION_ਤਾਲਾ_NO_UNLOCK:
        ਵਾਪਸ ",affect=neither";
    ਮੂਲ:
        ਵਾਪਸ "";
    }
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਚਿੰਨ੍ਹ_PADDING 15
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACTION_PADDING 30

ਸਥਿਰ bool
write_action(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, ਗਿਣਤੀ xkb_keymap_format format,
             xkb_layout_ਸੂਚਕਾਂਕ_t max_groups, ਬਣਤਰ buf *buf,
             ਅਚਲ ਸੰਘ xkb_action *action,
             ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ, ਅਚਲ ਅੱਖਰ *suffix)
{
    ਅਚਲ ਅੱਖਰ *type;
    ਅਚਲ ਅੱਖਰ *ਦਲੀਲਾਂ = ਨਿਰਾ;

    ਜੇ (!ਅਗੇਤਰ)
        ਅਗੇਤਰ = "";
    ਜੇ (!suffix)
        suffix = "";

    type = Action_Type_ਪਾਠ(action->type);

    ਚੋਣ (action->type) {
    ਹਾਲਤ ACTION_TYPE_MOD_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_MOD_LATCH:
    ਹਾਲਤ ACTION_TYPE_MOD_ਤਾਲਾ:
        ਜੇ (action->mods.flags & ACTION_MODS_LOOKUP_MODMAP)
            ਦਲੀਲਾਂ = "modMapMods";
        ਨਹੀਂਤਾਂ
            ਦਲੀਲਾਂ = Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_BOTH, &keymap->mods,
                               action->mods.mods.mods);
        bool unlockOnPress = (action->mods.flags & ACTION_UNLOCK_ON_PRESS);
        ਜੇ (unlockOnPress && !is_Mods_Un_ਤਾਲਾ_On_Press_Supported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ਪਾਠ_FORMAT,
                    "Cannot use \"%s(unlockOnPress=true)\" in keymap format %d\n",
                    Action_Type_ਪਾਠ(action->type), format);
            unlockOnPress = false;
        }
        bool latchOnPress = action->type == ACTION_TYPE_MOD_LATCH &&
                            (action->ਸਮੂਹ.flags & ACTION_LATCH_ON_PRESS);
        ਜੇ (latchOnPress && !isModsLatchOnPressSupported(format)) {
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ਪਾਠ_FORMAT,
                    "Cannot use \"LatchMods(latchOnPress=true)\" "
                    "in keymap format %d\n", format);
            latchOnPress = false;
        }
        write_buf(buf, "%s%s(modifiers=%s%s%s%s%s%s)%s", ਅਗੇਤਰ, type, ਦਲੀਲਾਂ,
                  (action->type != ACTION_TYPE_MOD_ਤਾਲਾ && (action->mods.flags & ACTION_ਤਾਲਾ_CLEAR)) ? ",clearLocks" : "",
                  (action->type == ACTION_TYPE_MOD_LATCH && (action->mods.flags & ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ)) ? ",latchToLock" : "",
                  (action->type == ACTION_TYPE_MOD_ਤਾਲਾ) ? affect_ਤਾਲਾ_ਪਾਠ(action->mods.flags, false) : "",
                  (unlockOnPress) ? ",unlockOnPress" : "",
                  (latchOnPress) ? ",latchOnPress" : "",
                  suffix);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_LATCH:
    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ:
        ਜੇ ((uint32_t) xkb_abs(action->ਸਮੂਹ.ਸਮੂਹ) <
            (max_groups + !(action->ਸਮੂਹ.flags & ACTION_ABSOLUTE_SWITCH))) {
            bool ਤਾਲਾ_On_Release = action->type == ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ &&
                                 (action->ਸਮੂਹ.flags & ACTION_ਤਾਲਾ_ON_RELEASE);
            ਜੇ (ਤਾਲਾ_On_Release && !is_ਸਮੂਹ_ਤਾਲਾ_On_Release_Supported(format)) {
                log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ਪਾਠ_FORMAT,
                        "Cannot use \"GroupLock(lockOnRelease=true)\" "
                        "in keymap format %d\n", format);
                ਤਾਲਾ_On_Release = false;
            }
            write_buf(buf, "%s%s(group=%s%"PRId32"%s%s%s)%s", ਅਗੇਤਰ, type,
                      (!(action->ਸਮੂਹ.flags & ACTION_ABSOLUTE_SWITCH) && action->ਸਮੂਹ.ਸਮੂਹ >= 0) ? "+" : "",
                      (action->ਸਮੂਹ.flags & ACTION_ABSOLUTE_SWITCH) ? action->ਸਮੂਹ.ਸਮੂਹ + 1 : action->ਸਮੂਹ.ਸਮੂਹ,
                      (action->type != ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ && (action->ਸਮੂਹ.flags & ACTION_ਤਾਲਾ_CLEAR)) ? ",clearLocks" : "",
                      (action->type == ACTION_TYPE_ਸਮੂਹ_LATCH && (action->ਸਮੂਹ.flags & ACTION_LATCH_ਮੰਜ਼ਿਲ_ਤਾਲਾ)) ? ",latchToLock" : "",
                      (ਤਾਲਾ_On_Release) ? ",lockOnRelease" : "",
                      suffix);
        } ਨਹੀਂਤਾਂ {
            /* Unsupported group index: degrade to VoidAction() */
            log_warn(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "Unsupported group index %"PRIu32" for action %s()\n",
                      (action->ਸਮੂਹ.ਸਮੂਹ +
                       !!(action->ਸਮੂਹ.flags & ACTION_ABSOLUTE_SWITCH)),
                     type);
            ਜਾਓ void_action;
        }
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_TERMINATE:
        write_buf(buf, "%s%s()%s", ਅਗੇਤਰ, type, suffix);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_PTR_MOVE:
        write_buf(buf, "%s%s(x=%s%d,y=%s%d%s)%s", ਅਗੇਤਰ, type,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_X) && action->ptr.x >= 0) ? "+" : "",
                  action->ptr.x,
                  (!(action->ptr.flags & ACTION_ABSOLUTE_Y) && action->ptr.y >= 0) ? "+" : "",
                  action->ptr.y,
                  (action->ptr.flags & ACTION_REPEAT) ? "" : ",!repeat",
                  suffix);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_PTR_ਤਾਲਾ:
        ਦਲੀਲਾਂ = affect_ਤਾਲਾ_ਪਾਠ(action->btn.flags, true);
        /* fallthrough */
    ਹਾਲਤ ACTION_TYPE_PTR_BUTTON:
        write_buf(buf, "%s%s(button=", ਅਗੇਤਰ, type);
        static_assert(XKB_ਸੂਚਕ_BUTTON_DEFAULT < XKB_ਸੂਚਕ_BUTTON_MIN, "");
        ਜੇ (action->btn.button > XKB_ਸੂਚਕ_BUTTON_DEFAULT &&
            action->btn.button <= XKB_ਸੂਚਕ_BUTTON_MAX)
            write_buf(buf, "%"PRIu8, action->btn.button);
        ਨਹੀਂਤਾਂ
            copy_ਮੰਜ਼ਿਲ_buf(buf, "default");
        ਜੇ (action->btn.ਗਿਣਤੀ_2) {
            ਪ੍ਰਤਿਜ੍ਞਾ(action->type != ACTION_TYPE_PTR_ਤਾਲਾ);
            write_buf(buf, ",count=%u", action->btn.ਗਿਣਤੀ_2);
        }
        ਜੇ (ਦਲੀਲਾਂ)
            write_buf(buf, "%s", ਦਲੀਲਾਂ);
        write_buf(buf, ")%s", suffix);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_PTR_DEFAULT:
        write_buf(buf, "%s%s(", ਅਗੇਤਰ, type);
        write_buf(buf, "affect=button,button=%s%"PRId8,
                  (!(action->dflt.flags & ACTION_ABSOLUTE_SWITCH) && action->dflt.ਮੁੱਲ >= 0) ? "+" : "",
                  action->dflt.ਮੁੱਲ);
        write_buf(buf, ")%s", suffix);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_SWITCH_VT:
        write_buf(buf, "%s%s(screen=%s%d,%ssame)%s", ਅਗੇਤਰ, type,
                  (!(action->screen.flags & ACTION_ABSOLUTE_SWITCH) && action->screen.screen >= 0) ? "+" : "",
                  action->screen.screen,
                  (action->screen.flags & ACTION_SAME_SCREEN) ? "" : "!",
                  suffix);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_CTRL_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_CTRL_ਤਾਲਾ:
        write_buf(buf, "%s%s(controls=%s%s)%s", ਅਗੇਤਰ, type,
                  Control_ਨਕਾਬ_ਪਾਠ(keymap->ctx, format, action->ctrls.ctrls),
                  (action->type == ACTION_TYPE_CTRL_ਤਾਲਾ
                    ? affect_ਤਾਲਾ_ਪਾਠ(action->ctrls.flags, false)
                    : ""),
                  suffix);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_REDIRECT_ਕੁੰਜੀ: {
        write_buf(buf, "%s%s(", ਅਗੇਤਰ, type);
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਅਚਲ ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(keymap, action->redirect.keycode);
        /* Can fail if the keycode was not initialized */
        ਜੇ (ਕੁੰਜੀ)
            write_buf(buf, "keycode=%s", ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, ਕੁੰਜੀ->name));
        ਜੇ (action->redirect.affect_mods) {
            xkb_mod_ਨਕਾਬ_t ਨਕਾਬ;
            ਨਕਾਬ = (action->redirect.affect_mods & action->redirect.mods);
            ਜੇ (ਨਕਾਬ)
                write_buf(buf, ",modifiers=%s",
                          Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_BOTH, &keymap->mods, ਨਕਾਬ));
            ਨਕਾਬ = (action->redirect.affect_mods & ~action->redirect.mods);
            ਜੇ (ਨਕਾਬ)
                write_buf(buf, ",clearMods=%s",
                          Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_BOTH, &keymap->mods, ਨਕਾਬ));
        }
        write_buf(buf, ")%s", suffix);
        ਰੋਕੋ;
    }

    ਹਾਲਤ ACTION_TYPE_UNKNOWN:
    ਹਾਲਤ ACTION_TYPE_NONE:
        write_buf(buf, "%sNoAction()%s", ਅਗੇਤਰ, suffix);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_VOID:
void_action:
        /*
         * VoidAction() is an xkbcommon extension.
         * Use LockControls as a backward-compatible fallback.
         * We cannot serialize it to `NoAction()`, as it would be dropped in
         * e.g. the context of multiple actions.
         * We better not use `Private` either, because it could still be
         * interpreted by X11.
         */
        ਜੇ (format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1)
            write_buf(buf, "%sLockControls(controls=none,affect=neither)%s",
                      ਅਗੇਤਰ, suffix);
        ਨਹੀਂਤਾਂ
            write_buf(buf, "%sVoidAction()%s", ਅਗੇਤਰ, suffix);
        ਰੋਕੋ;

    ਮੂਲ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_ਅੰਦਰੂਨੀ == 20 &&
                      ACTION_TYPE_ਅੰਦਰੂਨੀ + 1 == ACTION_TYPE_ਅੰਕ_ENTRIES,
                      "Missing action type");
        /* Unsupported legacy actions should have degraded to NoAction */
        ਪ੍ਰਤਿਜ੍ਞਾ(action->type != ACTION_TYPE_UNSUPPORTED_LEGACY);
        write_buf(buf,
                  "%s%s(type=0x%02x,data[0]=0x%02x,data[1]=0x%02x,data[2]=0x%02x,data[3]=0x%02x,data[4]=0x%02x,data[5]=0x%02x,data[6]=0x%02x)%s",
                  ਅਗੇਤਰ, type, action->type, action->priv.ਅੰਕੜੇ[0],
                  action->priv.ਅੰਕੜੇ[1], action->priv.ਅੰਕੜੇ[2],
                  action->priv.ਅੰਕੜੇ[3], action->priv.ਅੰਕੜੇ[4],
                  action->priv.ਅੰਕੜੇ[5], action->priv.ਅੰਕੜੇ[6],
                  suffix);
        ਰੋਕੋ;
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ ਅਚਲ ਸੰਘ xkb_action void_actions[] = {
    { .type = ACTION_TYPE_VOID }
};

ਸਥਿਰ bool
write_actions(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, ਗਿਣਤੀ xkb_keymap_format format,
              xkb_layout_ਸੂਚਕਾਂਕ_t max_groups, ਬਣਤਰ buf *buf, ਬਣਤਰ buf *buf2,
              ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ, xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ)
{
    ਸਥਿਰ ਅਚਲ ਸੰਘ xkb_action noAction = { .type = ACTION_TYPE_NONE };

    ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t level = 0; level < Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, ਸਮੂਹ);
         level++) {

        ਜੇ (level != 0)
            copy_ਮੰਜ਼ਿਲ_buf(buf, ", ");

        ਅਚਲ ਸੰਘ xkb_action *actions = ਨਿਰਾ;
        xkb_action_ਗਿਣਤੀ_t ਗਿਣਤੀ_2 = xkb_keymap_ਕੁੰਜੀ_get_actions_by_level(
            keymap, ਕੁੰਜੀ, ਸਮੂਹ, level, &actions
        );

        ਜੇ (ਗਿਣਤੀ_2 > 1 && format < XKB_KEYMAP_FORMAT_ਪਾਠ_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            actions = void_actions;
            ਗਿਣਤੀ_2 = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ਪਾਠ_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        buf2->ਮਾਪ = 0;
        ਜੇ (ਗਿਣਤੀ_2 == 0) {
            ਜੇ (!write_action(keymap, format, max_groups,
                              buf2, &noAction, ਨਿਰਾ, ਨਿਰਾ))
                ਵਾਪਸ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        ਨਹੀਂਤਾਂ ਜੇ (ਗਿਣਤੀ_2 == 1) {
            ਜੇ (!write_action(keymap, format, max_groups,
                              buf2, &(actions[0]), ਨਿਰਾ, ਨਿਰਾ))
                ਵਾਪਸ false;
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
        ਨਹੀਂਤਾਂ {
            copy_ਮੰਜ਼ਿਲ_buf(buf2, "{ ");
            ਲਈ (xkb_action_ਗਿਣਤੀ_t k = 0; k < ਗਿਣਤੀ_2; k++) {
                ਜੇ (k != 0)
                    copy_ਮੰਜ਼ਿਲ_buf(buf2, ", ");
                ਮਾਪ_t old_ਮਾਪ = buf2->ਮਾਪ;
                ਜੇ (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ਨਿਰਾ, ਨਿਰਾ))
                    ਵਾਪਸ false;
                /* Check if padding is necessary */
                ਜੇ (buf2->ਮਾਪ >= old_ਮਾਪ + ACTION_PADDING)
                    ਜਾਰੀ;
                /* Compute and write padding, then write the action again */
                ਅਚਲ ਪੂਰਨ padding = (ਪੂਰਨ)(old_ਮਾਪ + ACTION_PADDING - buf2->ਮਾਪ);
                buf2->ਮਾਪ = old_ਮਾਪ;
                write_buf(buf2, "%*s", padding, "");
                ਜੇ (!write_action(keymap, format, max_groups,
                                  buf2, &(actions[k]), ਨਿਰਾ, ਨਿਰਾ))
                    ਵਾਪਸ false;
            }
            copy_ਮੰਜ਼ਿਲ_buf(buf2, " }");
            write_buf(buf, "%*s", ACTION_PADDING, buf2->buf);
        }
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
write_action_defaults(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
                      ਗਿਣਤੀ xkb_keymap_format format,
                      ਅਚਲ ਸੰਘ xkb_action *action,
                      ਬਣਤਰ buf *buf)
{
    ਅਚਲ ਅੱਖਰ *type = Action_Type_ਪਾਠ(action->type);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅਗੇਤਰ_2 "\t"

    ਚੋਣ (action->type) {
    ਹਾਲਤ ACTION_TYPE_MOD_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_MOD_LATCH:
    ਹਾਲਤ ACTION_TYPE_MOD_ਤਾਲਾ:
        ਪ੍ਰਤਿਜ੍ਞਾ(action->mods.flags == 0);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->mods.mods.mods == 0);
        write_buf(buf, ਅਗੇਤਰ_2"%s.modifiers = 0;\n", type);
        ਜੇ (action->type != ACTION_TYPE_MOD_ਤਾਲਾ)
            write_buf(buf, ਅਗੇਤਰ_2"%s.clearLocks = false;\n", type);
        ਜੇ (action->type == ACTION_TYPE_MOD_LATCH &&
            isModsLatchOnPressSupported(format))
            write_buf(buf, ਅਗੇਤਰ_2"%s.latchOnPress = false;\n", type);
        ਜੇ (action->type == ACTION_TYPE_MOD_LATCH)
            write_buf(buf, ਅਗੇਤਰ_2"%s.latchToLock = false;\n", type);
        ਜੇ (action->type == ACTION_TYPE_MOD_ਤਾਲਾ)
            write_buf(buf, ਅਗੇਤਰ_2"%s.affect = both;\n", type);
        ਜੇ (is_Mods_Un_ਤਾਲਾ_On_Press_Supported(format))
            write_buf(buf, ਅਗੇਤਰ_2"%s.unlockOnPress = false;\n", type);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_LATCH:
    ਹਾਲਤ ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ:
        ਪ੍ਰਤਿਜ੍ਞਾ(action->ਸਮੂਹ.flags == 0);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->ਸਮੂਹ.ਸਮੂਹ == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ਅਗੇਤਰ_2"%s.group = +0;\n", type);
        ਜੇ (action->type != ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ)
            write_buf(buf, ਅਗੇਤਰ_2"%s.clearLocks = false;\n", type);
        ਜੇ (action->type == ACTION_TYPE_ਸਮੂਹ_LATCH)
            write_buf(buf, ਅਗੇਤਰ_2"%s.latchToLock = false;\n", type);
        ਜੇ (action->type == ACTION_TYPE_ਸਮੂਹ_ਤਾਲਾ &&
            is_ਸਮੂਹ_ਤਾਲਾ_On_Release_Supported(format))
            write_buf(buf, ਅਗੇਤਰ_2"%s.lockOnRelease = false;\n", type);
        ਰੋਕੋ;


    ਹਾਲਤ ACTION_TYPE_PTR_MOVE:
        ਪ੍ਰਤਿਜ੍ਞਾ(action->ptr.x == 0);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->ptr.y == 0);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->ptr.flags == ACTION_REPEAT);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ਅਗੇਤਰ_2"%s.x = +0;\n", type);
        write_buf(buf, ਅਗੇਤਰ_2"%s.y = +0;\n", type);
        write_buf(buf, ਅਗੇਤਰ_2"%s.accel = true;\n", type);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_PTR_ਤਾਲਾ:
    ਹਾਲਤ ACTION_TYPE_PTR_BUTTON:
        ਪ੍ਰਤਿਜ੍ਞਾ(action->btn.flags == 0);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->btn.button == 0);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->btn.ਗਿਣਤੀ_2 == 0);
        write_buf(buf, ਅਗੇਤਰ_2"%s.button = 0;\n", type);
        write_buf(buf, ਅਗੇਤਰ_2"%s.count = 0;\n", type);
        ਜੇ (action->type == ACTION_TYPE_PTR_ਤਾਲਾ)
            write_buf(buf, ਅਗੇਤਰ_2"%s.affect = both;\n", type);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_PTR_DEFAULT:
        ਪ੍ਰਤਿਜ੍ਞਾ(action->dflt.flags == 0);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->dflt.ਮੁੱਲ == 1);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ਅਗੇਤਰ_2"%s.button = +1;\n", type);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_SWITCH_VT:
        ਪ੍ਰਤਿਜ੍ਞਾ(action->screen.flags == ACTION_SAME_SCREEN);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->screen.screen == 0);
        /* Explicit sign to avoid setting ACTION_ABSOLUTE_SWITCH */
        write_buf(buf, ਅਗੇਤਰ_2"%s.screen = +0;\n", type);
        write_buf(buf, ਅਗੇਤਰ_2"%s.same = true;\n", type);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_CTRL_ਸੈੱਟ:
    ਹਾਲਤ ACTION_TYPE_CTRL_ਤਾਲਾ:
        ਪ੍ਰਤਿਜ੍ਞਾ(action->ctrls.flags == 0);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->ctrls.ctrls == 0);
        write_buf(buf, ਅਗੇਤਰ_2"%s.controls = 0;\n", type);
        ਜੇ (action->type == ACTION_TYPE_CTRL_ਤਾਲਾ)
            write_buf(buf, ਅਗੇਤਰ_2"%s.affect = both;\n", type);
        ਰੋਕੋ;

    ਹਾਲਤ ACTION_TYPE_REDIRECT_ਕੁੰਜੀ:
        ਪ੍ਰਤਿਜ੍ਞਾ(action->redirect.keycode == keymap->redirect_ਕੁੰਜੀ_auto);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->redirect.affect_mods == 0);
        ਪ੍ਰਤਿਜ੍ਞਾ(action->redirect.mods == 0);
        write_buf(buf, ਅਗੇਤਰ_2"%s.keycode = auto;\n", type);
        write_buf(buf, ਅਗੇਤਰ_2"%s.modifiers = 0;\n", type);
        write_buf(buf, ਅਗੇਤਰ_2"%s.clearMods = 0;\n", type);
        ਰੋਕੋ;

    ਮੂਲ:
        {} /* Label followed by declaration requires C23 */
        /* Ensure to not miss `xkb_action_type` updates */
        static_assert(ACTION_TYPE_ਅੰਦਰੂਨੀ == 20 &&
                      ACTION_TYPE_ਅੰਦਰੂਨੀ + 1 == ACTION_TYPE_ਅੰਕ_ENTRIES,
                      "Missing action type");
    }

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਅਗੇਤਰ_2

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
write_actions_defaults(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
                       ਗਿਣਤੀ xkb_keymap_format format, ਬਣਤਰ buf *buf)
{
    ActionsInfo defaults = {0};
    InitActionsInfo(keymap, &defaults);

    ਲਈ (uint8_t a = 0; a < (uint8_t) ACTION_TYPE_ਅੰਕ_ENTRIES; a++) {
        ਅਚਲ ਸੰਘ xkb_action * ਸੀਮਿਤ ਅਚਲ action = &defaults.actions[a];
        ਜੇ (!write_action_defaults(keymap, format, action, buf))
            ਵਾਪਸ false;
    }
    copy_ਮੰਜ਼ਿਲ_buf(buf, "\n");

    ਵਾਪਸ true;
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ xkb_sym_interpret fallback_interpret = {
    .sym = XKB_ਕੁੰਜੀ_Void_ਚਿੰਨ੍ਹ,
    .required = true,
    .repeat = FALLBACK_INTERPRET_ਕੁੰਜੀ_REPEAT,
    .ਮੇਲ = ਮੇਲ_ANY_OR_NONE,
    .mods = 0,
    .virtual_mod = (xkb_mod_ਸੂਚਕਾਂਕ_t) DEFAULT_INTERPRET_VMOD,
    .ਅੰਕ_actions = 0,
    .a = { .action = { .type = ACTION_TYPE_NONE } },
};

ਸਥਿਰ bool
write_compat(ਅਚਲ ਬਣਤਰ xkb_keymap * ਸੀਮਿਤ keymap,
             ਗਿਣਤੀ xkb_keymap_format format, xkb_layout_ਸੂਚਕਾਂਕ_t max_groups,
             ਗਿਣਤੀ xkb_keymap_serialize_flags flags, bool * ਸੀਮਿਤ some_interp,
             ਬਣਤਰ buf *buf)
{
    ਅਚਲ bool pretty = (flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ਅਚਲ bool defaults = (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ਮੁੱਲਾਂ);
    ਅਚਲ bool drop_ਅਣਵਰਤਿਆ = !(flags & XKB_KEYMAP_SERIALIZE_KEEP_ਅਣਵਰਤਿਆ);
    ਅਚਲ bool drop_interprets =
        ((flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ਕੁੰਜੀ_ਮੁੱਲਾਂ) && drop_ਅਣਵਰਤਿਆ);
    /* TODO: print all default values if the explicit flag is set */

    ਜੇ (keymap->compat_section_name)
        write_buf(buf, "xkb_compatibility \"%s\" {\n",
                  keymap->compat_section_name);
    ਨਹੀਂਤਾਂ
        copy_ਮੰਜ਼ਿਲ_buf(buf, "xkb_compatibility {\n");

    ਜੇ (!write_vmods(keymap, format,
                     (flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_VMODS), buf))
        ਵਾਪਸ false;

    ਜੇ (defaults && !write_actions_defaults(keymap, format, buf))
        ਵਾਪਸ false;

    copy_ਮੰਜ਼ਿਲ_buf(buf, "\tinterpret.useModMapMods= AnyLevel;\n");
    copy_ਮੰਜ਼ਿਲ_buf(buf, "\tinterpret.repeat= False;\n");

    /* xkbcomp requires at least one interpret entry. */
    ਅਚਲ bool use_fallback_interpret =
        (!keymap->ਅੰਕ_sym_interprets || drop_interprets);
    ਅਚਲ darray_ਮਾਪ_t ਅੰਕ_sym_interprets =
        (use_fallback_interpret ? 1 : keymap->ਅੰਕ_sym_interprets);
    ਅਚਲ ਬਣਤਰ xkb_sym_interpret* ਅਚਲ sym_interprets =
        (use_fallback_interpret ? &fallback_interpret : keymap->sym_interprets);

    ਲਈ (darray_ਮਾਪ_t i = 0; i < ਅੰਕ_sym_interprets; i++) {
        ਅਚਲ ਬਣਤਰ xkb_sym_interpret *si = &sym_interprets[i];
        ਜੇ (!si->required && drop_ਅਣਵਰਤਿਆ)
            ਜਾਰੀ;

        *some_interp = true;

        copy_ਮੰਜ਼ਿਲ_buf(buf, "\tinterpret ");
        ਜੇ (!si->sym) {
            copy_ਮੰਜ਼ਿਲ_buf(buf, "Any");
        } ਨਹੀਂਤਾਂ ਜੇ (pretty) {
            write_buf(buf, "%s", Keysym_ਪਾਠ(keymap->ctx, si->sym));
        } ਨਹੀਂਤਾਂ {
            write_buf(buf, "0x%"PRIx32, si->sym);
        }
        write_buf(buf, "+%s(%s) {",
                  SIMatch_ਪਾਠ(si->ਮੇਲ),
                  Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_BOTH, &keymap->mods, si->mods));

        bool has_explicit_properties = false;

        ਜੇ (si->virtual_mod != XKB_MOD_INVALID) {
            write_buf(buf, "\n\t\tvirtualModifier= %s;",
                      Mod_ਸੂਚਕਾਂਕ_ਪਾਠ(keymap->ctx, &keymap->mods,
                                   si->virtual_mod));
            has_explicit_properties = true;
        }

        ਜੇ (si->level_one_only) {
            copy_ਮੰਜ਼ਿਲ_buf(buf, "\n\t\tuseModMapMods=level1;");
            has_explicit_properties = true;
        } ਨਹੀਂਤਾਂ ਜੇ (defaults) {
            copy_ਮੰਜ਼ਿਲ_buf(buf, "\n\t\tuseModMapMods=AnyLevel;");
            has_explicit_properties = true;
        }

        ਜੇ (si->repeat) {
            copy_ਮੰਜ਼ਿਲ_buf(buf, "\n\t\trepeat= True;");
            has_explicit_properties = true;
        } ਨਹੀਂਤਾਂ ਜੇ (defaults) {
            copy_ਮੰਜ਼ਿਲ_buf(buf, "\n\t\trepeat= False;");
            has_explicit_properties = true;
        }

        ਅਚਲ ਸੰਘ xkb_action *action = ਨਿਰਾ;
        xkb_action_ਗਿਣਤੀ_t action_ਗਿਣਤੀ = si->ਅੰਕ_actions;
        ਜੇ (action_ਗਿਣਤੀ <= 1) {
            action = &si->a.action;
        } ਨਹੀਂਤਾਂ ਜੇ (format < XKB_KEYMAP_FORMAT_ਪਾਠ_V2) {
            /* V1: Degrade multiple actions to VoidAction() */
            action = &void_actions[0];
            action_ਗਿਣਤੀ = 1;
            log_err(keymap->ctx, XKB_ERROR_INCOMPATIBLE_KEYMAP_ਪਾਠ_FORMAT,
                    "Cannot serialize multiple actions per level "
                    "in keymap format %d; degrade to VoidAction()\n", format);
        }

        ਜੇ (action_ਗਿਣਤੀ > 1) {
            copy_ਮੰਜ਼ਿਲ_buf(buf, "\n\t\taction= {");
            ਅਚਲ ਅੱਖਰ suffix[] = ", ";
            ਲਈ (xkb_action_ਗਿਣਤੀ_t k = 0; k < si->ਅੰਕ_actions; k++) {
                ਜੇ (!write_action(keymap, format, max_groups,
                                  buf, &si->a.actions[k], "", suffix))
                    ਵਾਪਸ false;
            }
            buf->ਮਾਪ -= ਆਕਾਰ(suffix) - 1; /* trailing comma */
            copy_ਮੰਜ਼ਿਲ_buf(buf, "};");
            has_explicit_properties = true;
        } ਨਹੀਂਤਾਂ ਜੇ (action_ਗਿਣਤੀ == 1) {
            ਜੇ (!write_action(keymap, format, max_groups, buf, action,
                              "\n\t\taction= ", ";"))
                ਵਾਪਸ false;
            has_explicit_properties = true;
        }
        write_buf(buf, (has_explicit_properties
                        ? "\n\t};\n"
                        /* Empty interpret is a syntax error in xkbcomp, so
                         * use a dummy entry */
                        : "\n\t\taction= NoAction();\n\t};\n"));
    }

    ਜੇ (use_fallback_interpret || (
        /* Detect previous interpret fallback */
        ਅੰਕ_sym_interprets == 1 && *some_interp &&
        sym_interprets[0].sym == fallback_interpret.sym &&
        sym_interprets[0].ਮੇਲ == fallback_interpret.ਮੇਲ &&
        sym_interprets[0].mods == fallback_interpret.mods &&
        sym_interprets[0].repeat == fallback_interpret.repeat &&
        sym_interprets[0].virtual_mod == fallback_interpret.virtual_mod &&
        sym_interprets[0].level_one_only == fallback_interpret.level_one_only &&
        sym_interprets[0].ਅੰਕ_actions == fallback_interpret.ਅੰਕ_actions
        /* `required` field is not compared */
    ))
        *some_interp = false;

    ਅਚਲ ਬਣਤਰ xkb_led *led;
    xkb_leds_foreach(led, keymap) {
        ਜੇ ((led->which_groups || led->groups ||
             led->which_mods || led->mods.mods || led->ctrls) &&
            !write_led_map(keymap, format, defaults, buf, led))
        {
            ਵਾਪਸ false;
        }
    }

    copy_ਮੰਜ਼ਿਲ_buf(buf, "};\n\n");

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
write_keysyms(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
              ਬਣਤਰ buf *buf, ਬਣਤਰ buf *buf2,
              ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ, xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ,
              bool pretty, bool show_actions)
{
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ padding = (pretty)
        ? ((show_actions) ? ACTION_PADDING : ਚਿੰਨ੍ਹ_PADDING)
        : 0;
    ਲਈ (xkb_level_ਸੂਚਕਾਂਕ_t level = 0; level < Xkb_ਕੁੰਜੀ_ਅੰਕ_ਪੱਧਰ(ਕੁੰਜੀ, ਸਮੂਹ);
         level++) {
        ਅਚਲ xkb_keysym_t *syms;
        ਪੂਰਨ ਅੰਕ_syms;

        ਜੇ (level != 0)
            copy_ਮੰਜ਼ਿਲ_buf(buf, ", ");

        ਅੰਕ_syms = xkb_keymap_ਕੁੰਜੀ_get_syms_by_level(
            /* remove `const` constraint to use the dated public API */
            (ਬਣਤਰ xkb_keymap *)keymap,
            ਕੁੰਜੀ->keycode, ਸਮੂਹ, level, &syms
        );

        ਅਚਲ xkb_keysym_t no_ਚਿੰਨ੍ਹ = XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ;
        ਜੇ (ਅੰਕ_syms == 0) {
            ਅੰਕ_syms = 1;
            syms = &no_ਚਿੰਨ੍ਹ;
        }

        /*
         * NOTE: Use `NoSymbol` even without pretty output, for compatibility
         * with xkbcomp and xkbcommon < 1.12
         */

        ਜੇ (ਅੰਕ_syms == 1) {
            ਜੇ (pretty || syms[0] == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ)
                write_buf(buf, "%*s", padding, Keysym_ਪਾਠ(keymap->ctx, syms[0]));
            ਨਹੀਂਤਾਂ
                write_buf(buf, "0x%"PRIx32, syms[0]);
        } ਨਹੀਂਤਾਂ {
            ਜੇ (pretty) {
                buf2->ਮਾਪ = 0;
                copy_ਮੰਜ਼ਿਲ_buf(buf2, "{ ");
                ਲਈ (ਪੂਰਨ s = 0; s < ਅੰਕ_syms; s++) {
                    ਜੇ (s != 0)
                        copy_ਮੰਜ਼ਿਲ_buf(buf2, ", ");
                    write_buf(buf2, "%*s", (show_actions ? padding : 0),
                              Keysym_ਪਾਠ(keymap->ctx, syms[s]));
                }
                copy_ਮੰਜ਼ਿਲ_buf(buf2, " }");
                write_buf(buf, "%*s", padding, buf2->buf);
            } ਨਹੀਂਤਾਂ {
                copy_ਮੰਜ਼ਿਲ_buf(buf, "{");
                ਲਈ (ਪੂਰਨ s = 0; s < ਅੰਕ_syms; s++) {
                    ਜੇ (s != 0)
                        copy_ਮੰਜ਼ਿਲ_buf(buf, ",");
                    ਜੇ (syms[s] == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ)
                        copy_ਮੰਜ਼ਿਲ_buf(buf, "NoSymbol");
                    ਨਹੀਂਤਾਂ
                        write_buf(buf, "0x%"PRIx32, syms[s]);
                }
                copy_ਮੰਜ਼ਿਲ_buf(buf, "}");
            }
        }
    }

    ਵਾਪਸ true;
}

ਸਥਿਰ bool
write_ਕੁੰਜੀ(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
          ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *config,
          ਅਚਲ ਕੁੰਜੀ_name_substitutions *substitutions,
          xkb_layout_ਸੂਚਕਾਂਕ_t max_groups, bool some_interprets,
          bool drop_interprets, bool explicit, bool pretty,
          ਬਣਤਰ buf *buf, ਬਣਤਰ buf *buf2, ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ)
{
    bool simple = true;

    xkb_layout_ਨਕਾਬ_t layouts = config->layouts;
    ਜੇ (popcount32(layouts) != keymap->ਅੰਕ_groups) {
        /* Subset of layouts: use out-of-range key policy */
        xkb_layout_ਸੂਚਕਾਂਕ_t l = 0;
        ਲਈ (xkb_layout_ਨਕਾਬ_t ls = layouts; ls; ls >>= 1, l++) {
            ਜੇ (ls & 0x1) {
                /* Replace target layout with its effective layout, if valid */
                layouts &= ~(UINT32_C(1) << l);
                ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t effective =
                    xkb_keymap_ਕੁੰਜੀ_effective_layout(ਕੁੰਜੀ, l);
                ਜੇ (effective != XKB_LAYOUT_INVALID)
                    layouts |= (UINT32_C(1) << effective);
            }
        }
    } ਨਹੀਂਤਾਂ {
        layouts &= ((UINT64_C(1) << ਕੁੰਜੀ->ਅੰਕ_groups) - 1);
    }
    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t groups_ਗਿਣਤੀ = popcount32(layouts);

    ਅਚਲ xkb_atom_t name = (substitutions == ਨਿਰਾ)
        ? ਕੁੰਜੀ->name
        : substitute_name(substitutions, ਕੁੰਜੀ->name);

    ਜੇ (pretty)
        write_buf(buf, "\tkey %-20s {", ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, name));
    ਨਹੀਂਤਾਂ
        write_buf(buf, "\tkey %s {", ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, name));

    ਜੇ ((ਕੁੰਜੀ->explicit & EXPLICIT_TYPES) || explicit) {
        simple = false;

        bool multi_type = explicit;
        /* Check for distinct key types only if not requiring explicit values */
        ਜੇ (!explicit) {
            ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 1; ਸਮੂਹ < ਕੁੰਜੀ->ਅੰਕ_groups; ਸਮੂਹ++) {
                ਜੇ (!((UINT32_C(1) << ਸਮੂਹ) & layouts))
                    ਜਾਰੀ;

                ਜੇ (ਕੁੰਜੀ->groups[ਸਮੂਹ].type != ਕੁੰਜੀ->groups[0].type) {
                    multi_type = true;
                    ਰੋਕੋ;
                }
            }
        }

        ਜੇ (multi_type) {
            xkb_layout_ਸੂਚਕਾਂਕ_t new_ਸਮੂਹ = (xkb_layout_ਸੂਚਕਾਂਕ_t)-1;
            ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0; ਸਮੂਹ < ਕੁੰਜੀ->ਅੰਕ_groups; ਸਮੂਹ++) {
                ਜੇ (!((UINT32_C(1) << ਸਮੂਹ) & layouts))
                    ਜਾਰੀ;

                new_ਸਮੂਹ++;

                ਜੇ (!ਕੁੰਜੀ->groups[ਸਮੂਹ].explicit_type && !explicit)
                    ਜਾਰੀ;

                ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type * ਅਚਲ type = ਕੁੰਜੀ->groups[ਸਮੂਹ].type;
                write_buf(buf, "\n\t\ttype[%"PRIu32"]= ", new_ਸਮੂਹ + 1);
                write_buf_string_literal(
                  buf, xkb_atom_ਪਾਠ(keymap->ctx, type->name));
                copy_ਮੰਜ਼ਿਲ_buf(buf, ",");
            }
        }
        ਨਹੀਂਤਾਂ {
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ_type * ਅਚਲ type = ਕੁੰਜੀ->groups[0].type;
            write_buf(buf, "\n\t\ttype= ");
            write_buf_string_literal(
                buf, xkb_atom_ਪਾਠ(keymap->ctx, type->name));
            copy_ਮੰਜ਼ਿਲ_buf(buf, ",");
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
    ਅਚਲ bool explicit_actions = (ਕੁੰਜੀ->explicit & EXPLICIT_INTERP);

    /* Check if an explicit value is required */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ require_explicit(ਕੁੰਜੀ, prop,                                       \
                             ਮੁੱਲ, initial_ਮੁੱਲ, default_interpret_ਮੁੱਲ) ( \
        /* Explicit value in the source */                                    \
        ((ਕੁੰਜੀ)->explicit & (prop)) ||                                         \
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
                ((ਕੁੰਜੀ)->implicit_actions && !some_interprets)                 \
            ) && (                                                            \
                /* Non-initial value */                                       \
                (ਮੁੱਲ) != (initial_ਮੁੱਲ) ||                                 \
                /* Force initial value for clarity */                         \
                (initial_ਮੁੱਲ) != (default_interpret_ਮੁੱਲ)                  \
            )                                                                 \
        )                                                                     \
    )

    ਜੇ (require_explicit(ਕੁੰਜੀ, EXPLICIT_REPEAT, ਕੁੰਜੀ->repeats,
                         DEFAULT_ਕੁੰਜੀ_REPEAT, DEFAULT_INTERPRET_ਕੁੰਜੀ_REPEAT)) {
        ਜੇ (ਕੁੰਜੀ->repeats)
            copy_ਮੰਜ਼ਿਲ_buf(buf, "\n\t\trepeat= Yes,");
        ਨਹੀਂਤਾਂ
            copy_ਮੰਜ਼ਿਲ_buf(buf, "\n\t\trepeat= No,");
        simple = false;
    }

    ਜੇ (require_explicit(ਕੁੰਜੀ, EXPLICIT_VMODMAP, ਕੁੰਜੀ->vmodmap,
                         (xkb_mod_ਨਕਾਬ_t) DEFAULT_ਕੁੰਜੀ_VMODMAP,
                         (xkb_mod_ਨਕਾਬ_t) DEFAULT_INTERPRET_VMODMAP)) {
        write_buf(buf, "\n\t\tvirtualMods= %s,",
                  Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_BOTH, &keymap->mods,
                              ਕੁੰਜੀ->vmodmap));
        simple = false;
    }

    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ require_explicit

    ਚੋਣ (ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_policy) {
    ਹਾਲਤ XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_CLAMP:
        copy_ਮੰਜ਼ਿਲ_buf(buf, "\n\t\tgroupsClamp,");
        simple = false;
        ਰੋਕੋ;

    ਹਾਲਤ XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_REDIRECT: {
        ਜੇ (!groups_ਗਿਣਤੀ) {
            /* No layout to redirect to: skip */
            ਰੋਕੋ;
        }

        xkb_layout_ਨਕਾਬ_t layout_ਨਕਾਬ =
            (UINT32_C(1) << ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ);
        ਜੇ (!(layouts & layout_ਨਕਾਬ)) {
            /*
             * Layout is not included: default to first layout
             * See: XkbWrapGroupIntoRange
             */
            log_warn(keymap->ctx, XKB_LOG_ਸੁਨੇਹਾ_NO_ID,
                     "key %s: groupsRedirect=%"PRIu32" converted to "
                     "groupsRedirect=1\n",
                     ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, name),
                     ਕੁੰਜੀ->ਬਾਹਰਲਾ_ਦਾ_range_ਸਮੂਹ_ਸੰਖਿਆ + 1);
            layout_ਨਕਾਬ = 0x1;
        }

        /* In case some lower layouts were discarded, update the group index */
        ਅਚਲ xkb_layout_ਨਕਾਬ_t ਨੀਵਾਂ = (layouts & (layout_ਨਕਾਬ - 1));
        ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t new = popcount32(ਨੀਵਾਂ);
        write_buf(buf, "\n\t\tgroupsRedirect= %"PRIu32",", new + 1);
        simple = false;
        ਰੋਕੋ;
    }

    ਹਾਲਤ XKB_LAYOUT_ਬਾਹਰਲਾ_ਦਾ_RANGE_WRAP:
    ਮੂਲ:
        ਰੋਕੋ;
    }

    ਜੇ (ਕੁੰਜੀ->overlays) {
        xkb_overlay_ਨਕਾਬ_t ਬਚਿਆ = ਕੁੰਜੀ->overlays;

        ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t overlay_max =
            format_max_overlays(config->format);
        static_assert(XKB_OVERLAY_ਗਿਣਤੀ == 8, "invalid right shift");
        ਅਚਲ xkb_overlay_ਨਕਾਬ_t valid =
            (xkb_overlay_ਨਕਾਬ_t)((1u << overlay_max) - 1u);
        ਬਚਿਆ &= valid;
        ਜੇ (ਬਚਿਆ != ਕੁੰਜੀ->overlays) {
            log_warn(keymap->ctx, XKB_ERROR_UNSUPPORTED_OVERLAY_ਸੂਚਕਾਂਕ,
                     "Overlays indices > %u in %s require using "
                     "keymap format >= v2; "
                     "keep overlays 0x%08x and discard overlays 0x%08x\n",
                     overlay_max, ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, name),
                     ਬਚਿਆ, ਕੁੰਜੀ->overlays & ~valid);
        }

        ਜੇ (ਬਚਿਆ && !ਕੁੰਜੀ->overlays_inline &&
            !areOverlappingOverlaysSupported(config->format)) {
            /* isolate lowest set bit */
            ਅਚਲ xkb_overlay_ਨਕਾਬ_t lsb = (xkb_overlay_ਨਕਾਬ_t)(
                ਬਚਿਆ &
                (xkb_overlay_ਨਕਾਬ_t)((ਬਿਨਾਨਿਸ਼ਾਨੀ)~ਬਚਿਆ + 1u)
            );
            /* get its index */
            ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t overlay =
                (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(lsb - 1u);
            log_warn(keymap->ctx, XKB_ERROR_OVERLAPPING_OVERLAY,
                     "Overlapping overlays in %s require using "
                     "keymap format >= v2; "
                     "keep overlay %u and discard overlays 0x%x\n",
                     ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, name), overlay, ਬਚਿਆ & ~lsb);
            ਬਚਿਆ = lsb;
        }

        ਜੇ (ਬਚਿਆ)
            simple = false;

        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ * ਅਚਲ *overlays_ਕੁੰਜੀਆਂ = (ਕੁੰਜੀ->overlays_inline)
                                                    ? &ਕੁੰਜੀ->overlay_ਕੁੰਜੀ
                                                    : ਕੁੰਜੀ->overlays_ਕੁੰਜੀਆਂ;
        ਜਦੋਂ (ਬਚਿਆ) {
            /* isolate lowest set bit */
            ਅਚਲ xkb_overlay_ਨਕਾਬ_t lsb = (xkb_overlay_ਨਕਾਬ_t)(
                ਬਚਿਆ &
                (xkb_overlay_ਨਕਾਬ_t)((ਬਿਨਾਨਿਸ਼ਾਨੀ)~ਬਚਿਆ + 1u)
            );
            /* get its index */
            ਅਚਲ xkb_overlay_ਸੂਚਕਾਂਕ_t overlay =
                (xkb_overlay_ਸੂਚਕਾਂਕ_t)popcount32(lsb - 1u);
            ਬਚਿਆ = (ਬਚਿਆ & ~lsb);

            /* pop current value */
            ਜੇ (ਕੁੰਜੀ->overlays_inline && ਬਚਿਆ)
                PANIC_UNREACHABLE();
            ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *overlay_ਕੁੰਜੀ = *(overlays_ਕੁੰਜੀਆਂ++);

            ਅਚਲ xkb_atom_t overlay_ਕੁੰਜੀ_name = (substitutions == ਨਿਰਾ)
                ? overlay_ਕੁੰਜੀ->name
                : substitute_name(substitutions, overlay_ਕੁੰਜੀ->name);
            write_buf(buf, "\n\t\toverlay%u= %s,",
                      overlay + 1, ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, overlay_ਕੁੰਜੀ_name));
        }
    }

    ਜੇ (groups_ਗਿਣਤੀ > 1 || explicit_actions || explicit)
        simple = false;

    ਜੇ (simple) {
        ਜੇ (groups_ਗਿਣਤੀ == 0) {
            /* Remove trailing comma */
            delete_ਆਖਰੀ_char(buf, ',');
            ਜੇ (buf->buf[buf->ਮਾਪ - 1] == ',')
                buf->ਮਾਪ--;
        } ਨਹੀਂਤਾਂ {
            copy_ਮੰਜ਼ਿਲ_buf(buf, "\t[ ");
            /* Get the single group index */
            xkb_mod_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = ctz32(layouts);
            /* The group may not be defined for this key if it is not the first one */
            ਸਮੂਹ = xkb_keymap_ਕੁੰਜੀ_effective_layout(ਕੁੰਜੀ, ਸਮੂਹ);
            ਜੇ (!write_keysyms(keymap, buf, buf2, ਕੁੰਜੀ, ਸਮੂਹ, pretty, false))
                ਵਾਪਸ false;
            copy_ਮੰਜ਼ਿਲ_buf(buf, " ]");
        }
        copy_ਮੰਜ਼ਿਲ_buf(buf, " };\n");
    }
    ਨਹੀਂਤਾਂ {
        xkb_layout_ਸੂਚਕਾਂਕ_t new_ਸਮੂਹ = (xkb_layout_ਸੂਚਕਾਂਕ_t)-1;
        ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0; ਸਮੂਹ < ਕੁੰਜੀ->ਅੰਕ_groups; ਸਮੂਹ++) {
            ਜੇ (!((UINT32_C(1) << ਸਮੂਹ) & layouts))
                ਜਾਰੀ;

            new_ਸਮੂਹ++;

            ਅਚਲ bool print_actions =
                /* Group has explicit actions */
                ਕੁੰਜੀ->groups[ਸਮੂਹ].explicit_actions ||
                /* Explicit values are required */
                explicit ||
                /* Group has symbols but no explicit actions and key has explicit
                 * actions in another group: ensure compatibility with xkbcomp
                 * and all xkbcommon versions */
                (ਕੁੰਜੀ->groups[ਸਮੂਹ].explicit_symbols &&
                 explicit_actions && some_interprets) ||
                /* Group has implicit actions but no interpret can set them, so
                 * the actions must be made explicit */
                (ਕੁੰਜੀ->groups[ਸਮੂਹ].implicit_actions && !some_interprets);

            ਜੇ (new_ਸਮੂਹ != 0)
                copy_ਮੰਜ਼ਿਲ_buf(buf, ",");
            write_buf(buf, "\n\t\tsymbols[%"PRIu32"]= [ ", new_ਸਮੂਹ + 1);

            ਜੇ (!write_keysyms(keymap, buf, buf2, ਕੁੰਜੀ, ਸਮੂਹ,
                               pretty, print_actions))
                ਵਾਪਸ false;
            copy_ਮੰਜ਼ਿਲ_buf(buf, " ]");
            ਜੇ (print_actions) {
                write_buf(buf, ",\n\t\tactions[%"PRIu32"]= [ ", new_ਸਮੂਹ + 1);
                ਜੇ (!write_actions(keymap, config->format, max_groups,
                                   buf, buf2, ਕੁੰਜੀ, ਸਮੂਹ))
                    ਵਾਪਸ false;
                copy_ਮੰਜ਼ਿਲ_buf(buf, " ]");
            }
        }
        ਜੇ (!groups_ਗਿਣਤੀ)
            delete_ਆਖਰੀ_char(buf, ',');
        copy_ਮੰਜ਼ਿਲ_buf(buf, "\n\t};\n");
    }

    ਵਾਪਸ true;
}


ਸਥਿਰ ਪੂਰਨ
cmp_modmap_ਕਦਮ_1(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
    ਅਚਲ Mod_Map_ਇੰਦਰਾਜ *m1 = (ਅਚਲ Mod_Map_ਇੰਦਰਾਜ *)a;
    ਅਚਲ Mod_Map_ਇੰਦਰਾਜ *m2 = (ਅਚਲ Mod_Map_ਇੰਦਰਾਜ *)b;

    ਜੇ (!m1->mods) {
        ਜੇ (!m2->mods) {
            /* Entry will be dropped: do not care about the other fields */
            ਵਾਪਸ 0;
        }
        ਵਾਪਸ +1;
    } ਨਹੀਂਤਾਂ ਜੇ (!m2->mods) {
        ਵਾਪਸ -1;
    }

    /* Valid only on step 1 */
    ਜੇ (m1->ਕੁੰਜੀ_ਕੋਡ < m2->ਕੁੰਜੀ_ਕੋਡ)
        ਵਾਪਸ -1;
    ਨਹੀਂਤਾਂ ਜੇ (m1->ਕੁੰਜੀ_ਕੋਡ > m2->ਕੁੰਜੀ_ਕੋਡ)
        ਵਾਪਸ +1;

    ਜੇ (m1->mods < m2->mods)
        ਵਾਪਸ -1;
    ਨਹੀਂਤਾਂ ਜੇ (m1->mods > m2->mods)
        ਵਾਪਸ +1;

    ਜੇ (m1->have_ਚਿੰਨ੍ਹ < m2->have_ਚਿੰਨ੍ਹ)
        ਵਾਪਸ +1;
    ਨਹੀਂਤਾਂ ਜੇ (m1->have_ਚਿੰਨ੍ਹ > m2->have_ਚਿੰਨ੍ਹ)
        ਵਾਪਸ -1;

    ਜੇ (m1->have_ਚਿੰਨ੍ਹ) {
        ਜੇ (m1->u.ਕੁੰਜੀ_Sym < m2->u.ਕੁੰਜੀ_Sym)
            ਵਾਪਸ -1;
        ਜੇ (m1->u.ਕੁੰਜੀ_Sym > m2->u.ਕੁੰਜੀ_Sym)
            ਵਾਪਸ +1;
    } ਨਹੀਂਤਾਂ {
        ਜੇ (m1->u.ਕੁੰਜੀ_Name < m2->u.ਕੁੰਜੀ_Name)
            ਵਾਪਸ -1;
        ਜੇ (m1->u.ਕੁੰਜੀ_Name > m2->u.ਕੁੰਜੀ_Name)
            ਵਾਪਸ +1;
    }

    ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
cmp_modmap_ਕਦਮ_2(ਅਚਲ ਖਾਲੀ *a, ਅਚਲ ਖਾਲੀ *b)
{
    ਅਚਲ Mod_Map_ਇੰਦਰਾਜ *m1 = (ਅਚਲ Mod_Map_ਇੰਦਰਾਜ *)a;
    ਅਚਲ Mod_Map_ਇੰਦਰਾਜ *m2 = (ਅਚਲ Mod_Map_ਇੰਦਰਾਜ *)b;

    ਜੇ (m1->mods < m2->mods)
        ਵਾਪਸ -1;
    ਜੇ (m1->mods > m2->mods)
        ਵਾਪਸ +1;

    ਜੇ (m1->have_ਚਿੰਨ੍ਹ < m2->have_ਚਿੰਨ੍ਹ)
        ਵਾਪਸ -1;
    ਜੇ (m1->have_ਚਿੰਨ੍ਹ > m2->have_ਚਿੰਨ੍ਹ)
        ਵਾਪਸ +1;

    ਜੇ (m1->have_ਚਿੰਨ੍ਹ) {
        ਜੇ (m1->u.ਕੁੰਜੀ_Sym < m2->u.ਕੁੰਜੀ_Sym)
            ਵਾਪਸ -1;
        ਜੇ (m1->u.ਕੁੰਜੀ_Sym > m2->u.ਕੁੰਜੀ_Sym)
            ਵਾਪਸ +1;
    } ਨਹੀਂਤਾਂ {
        ਜੇ (m1->ਕੁੰਜੀ_ਕੋਡ < m2->ਕੁੰਜੀ_ਕੋਡ)
            ਵਾਪਸ -1;
        ਜੇ (m1->ਕੁੰਜੀ_ਕੋਡ > m2->ਕੁੰਜੀ_ਕੋਡ)
            ਵਾਪਸ +1;
    }

    ਵਾਪਸ 0;
}

ਸਥਿਰ bool
write_modmaps(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap, ਗਿਣਤੀ xkb_keymap_format format,
              ਅਚਲ ਕੁੰਜੀ_name_substitutions *substitutions,
              bool pretty, bool drop_interprets, ਬਣਤਰ buf *buf)
{
    ਜੇ (!keymap->ਅੰਕ_modmaps)
        ਵਾਪਸ true;

    darray(Mod_Map_ਇੰਦਰਾਜ) modmaps = darray_new();
    darray_from_ਚੀਜ਼ਾਂ(modmaps, keymap->modmaps, keymap->ਅੰਕ_modmaps);
    ਜੇ (!darray_ਚੀਜ਼ਾਂ(modmaps))
        ਵਾਪਸ false;

    /* Sort entries by key code */
    qsort(darray_ਚੀਜ਼ਾਂ(modmaps), darray_ਮਾਪ(modmaps),
          ਆਕਾਰ(*darray_ਚੀਜ਼ਾਂ(modmaps)), &cmp_modmap_ਕਦਮ_1);

    /* Drop disabled entries at the end */
    ਲਈ (darray_ਮਾਪ_t m = darray_ਮਾਪ(modmaps); m-- > 0;) {
        ਜੇ (darray_ਵਸਤੂ(modmaps, m).mods)
            ਰੋਕੋ;
        darray_ਮਾਪ(modmaps)--;
    }

    /*
     * Build detailed keymap modmap
     *
     * NOTE: use an explicit for-loop instead of the darray_foreach macro
     * because we may add entries while iterating it. Iterate from the end,
     * so that we do not iterate the new entries.
     */
    xkb_keycode_t current_keycode = XKB_KEYCODE_INVALID;
    bool convert_ਮੰਜ਼ਿਲ_v1_compatibility_ਇੰਦਰਾਜ = false;
    ਲਈ (darray_ਮਾਪ_t m = darray_ਮਾਪ(modmaps); m-- > 0;) {
        Mod_Map_ਇੰਦਰਾਜ *mm = &darray_ਵਸਤੂ(modmaps, m);
        ਪ੍ਰਤਿਜ੍ਞਾ(mm->mods);

        ਜੇ (mm->ਕੁੰਜੀ_ਕੋਡ == current_keycode) {
            /*
             * Keycode already processed: either drop next key code entries
             * or convert them to keymap format v1
             */
            ਅਚਲ xkb_mod_ਨਕਾਬ_t core_mods = (mm->mods & MOD_REAL_ਨਕਾਬ_ALL);
            ਜੇ (convert_ਮੰਜ਼ਿਲ_v1_compatibility_ਇੰਦਰਾਜ && core_mods) {
                /*
                 * Convert to keymap format V1-specific entry:
                 * keep only the highest core modifier
                 */
                mm->mods = UINT32_C(1) << (XKB_MAX_MODS - 1 - clz32(core_mods));
            } ਨਹੀਂਤਾਂ {
                /*
                 * Drop if there is a previous entry for all keymap formats or
                 * if the current entry is incompatible with V1 keymap format.
                 */
                mm->mods = 0;
            }
            ਜਾਰੀ;
        }

        current_keycode = mm->ਕੁੰਜੀ_ਕੋਡ;
        ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ = Xkb_ਕੁੰਜੀ(keymap, mm->ਕੁੰਜੀ_ਕੋਡ);

        ਅਚਲ xkb_mod_ਨਕਾਬ_t core_mods = (ਕੁੰਜੀ->modmap & MOD_REAL_ਨਕਾਬ_ALL);
        ਜੇ (format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1 &&
            ((ਕੁੰਜੀ->modmap & (ਕੁੰਜੀ->modmap - 1)) || ਕੁੰਜੀ->modmap != core_mods)) {
            /* V1 entry with multiple modifiers or non X11 core modifiers */
            m++; /* process in next iteration */
            convert_ਮੰਜ਼ਿਲ_v1_compatibility_ਇੰਦਰਾਜ = true;
            log_err(keymap->ctx, XKB_ERROR_INVALID_MODIFIER_MAP_ਨਕਾਬ,
                    "Cannot serialize modifier map of key %s: modifiers mask "
                    "0x%"PRIx32" incompatible with keymap format v1\n",
                    ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, ਕੁੰਜੀ->name), ਕੁੰਜੀ->modmap);
        } ਨਹੀਂਤਾਂ {
            /* V2 format or V1 format with a single core modifier */
            ਪ੍ਰਤਿਜ੍ਞਾ(ਕੁੰਜੀ->modmap);
            mm->mods = ਕੁੰਜੀ->modmap;
            /* Force key name */
            mm->have_ਚਿੰਨ੍ਹ = false;
            mm->u.ਕੁੰਜੀ_Name = ਕੁੰਜੀ->name;
            convert_ਮੰਜ਼ਿਲ_v1_compatibility_ਇੰਦਰਾਜ = false;
        }
    }

    ਜੇ (darray_ਖਾਲੀ_ਥਾਵਾਂ(modmaps))
        ਜਾਓ ਬਾਹਰ_ਨਿਕਲੋ;

    /* Sort entries by modifier mask */
    qsort(darray_ਚੀਜ਼ਾਂ(modmaps), darray_ਮਾਪ(modmaps),
          ਆਕਾਰ(*darray_ਚੀਜ਼ਾਂ(modmaps)), &cmp_modmap_ਕਦਮ_2);

    /*
     * Serialize modmaps
     */

    xkb_mod_ਨਕਾਬ_t current_mods = 0;
    ਅਚਲ Mod_Map_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
    darray_foreach(ਇੰਦਰਾਜ, modmaps) {
        ਜੇ (!ਇੰਦਰਾਜ->mods)
            ਜਾਰੀ;

        ਜੇ (current_mods != ਇੰਦਰਾਜ->mods) {
            ਜੇ (current_mods) {
                copy_ਮੰਜ਼ਿਲ_buf(buf, " };\n");
            }
            current_mods = ਇੰਦਰਾਜ->mods;
            write_buf(buf, "\tmodifier_map %s { ",
                      Mod_ਨਕਾਬ_ਪਾਠ(keymap->ctx, MOD_REAL, &keymap->mods,
                                  current_mods));
        } ਨਹੀਂਤਾਂ {
            copy_ਮੰਜ਼ਿਲ_buf(buf, ", ");
        }

        ਜੇ (ਇੰਦਰਾਜ->have_ਚਿੰਨ੍ਹ) {
            ਜੇ (pretty || ਇੰਦਰਾਜ->u.ਕੁੰਜੀ_Sym == XKB_ਕੁੰਜੀ_No_ਚਿੰਨ੍ਹ)
                write_buf(buf, "%s", Keysym_ਪਾਠ(keymap->ctx, ਇੰਦਰਾਜ->u.ਕੁੰਜੀ_Sym));
            ਨਹੀਂਤਾਂ
                write_buf(buf, "0x%"PRIx32, ਇੰਦਰਾਜ->u.ਕੁੰਜੀ_Sym);
        } ਨਹੀਂਤਾਂ {
            ਅਚਲ xkb_atom_t name = (substitutions == ਨਿਰਾ)
                ? ਇੰਦਰਾਜ->u.ਕੁੰਜੀ_Name
                : substitute_name(substitutions, ਇੰਦਰਾਜ->u.ਕੁੰਜੀ_Name);
            write_buf(buf, "%s", ਕੁੰਜੀ_Name_ਪਾਠ(keymap->ctx, name));
        }
    }
    ਜੇ (current_mods) {
        copy_ਮੰਜ਼ਿਲ_buf(buf, " };\n");
    }

ਬਾਹਰ_ਨਿਕਲੋ:
    darray_free(modmaps);
    ਵਾਪਸ true;
}

ਸਥਿਰ bool
write_symbols(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
              ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *config,
              ਅਚਲ ਕੁੰਜੀ_name_substitutions *substitutions,
              xkb_layout_ਸੂਚਕਾਂਕ_t max_groups, bool some_interp,
              ਬਣਤਰ buf *buf)
{
    ਅਚਲ bool pretty = (config->flags & XKB_KEYMAP_SERIALIZE_PRETTY);
    ਅਚਲ bool defaults =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_DEFAULT_ਮੁੱਲਾਂ);
    ਅਚਲ bool explicit_ਕੁੰਜੀ_ਮੁੱਲਾਂ =
        (config->flags & XKB_KEYMAP_SERIALIZE_EXPLICIT_ਕੁੰਜੀ_ਮੁੱਲਾਂ);
    ਅਚਲ bool drop_ਅਣਵਰਤਿਆ = !(config->flags & XKB_KEYMAP_SERIALIZE_KEEP_ਅਣਵਰਤਿਆ);
    ਅਚਲ bool drop_interprets = (explicit_ਕੁੰਜੀ_ਮੁੱਲਾਂ && drop_ਅਣਵਰਤਿਆ);
    /* TODO: print all default values if explicit flag is set */

    ਜੇ (keymap->symbols_section_name)
        write_buf(buf, "xkb_symbols \"%s\" {\n", keymap->symbols_section_name);
    ਨਹੀਂਤਾਂ
        copy_ਮੰਜ਼ਿਲ_buf(buf, "xkb_symbols {\n");

    bool has_ਸਮੂਹ_ਨਾਂ = false;
    xkb_layout_ਸੂਚਕਾਂਕ_t new_ਸਮੂਹ = (xkb_layout_ਸੂਚਕਾਂਕ_t)-1;
    ਲਈ (xkb_layout_ਸੂਚਕਾਂਕ_t ਸਮੂਹ = 0; ਸਮੂਹ < keymap->ਅੰਕ_ਸਮੂਹ_ਨਾਂ; ਸਮੂਹ++) {
        ਜੇ (!((UINT32_C(1) << ਸਮੂਹ) & config->layouts))
            ਜਾਰੀ;

        new_ਸਮੂਹ++;

        ਜੇ (keymap->ਸਮੂਹ_ਨਾਂ[ਸਮੂਹ]) {
            write_buf(buf, "\tname[%"PRIu32"]=", new_ਸਮੂਹ + 1);
            write_buf_string_literal(
                buf, xkb_atom_ਪਾਠ(keymap->ctx, keymap->ਸਮੂਹ_ਨਾਂ[ਸਮੂਹ]));
            copy_ਮੰਜ਼ਿਲ_buf(buf, ";\n");
            has_ਸਮੂਹ_ਨਾਂ = true;
        }
    }
    ਜੇ (has_ਸਮੂਹ_ਨਾਂ)
        copy_ਮੰਜ਼ਿਲ_buf(buf, "\n");

    ਜੇ (defaults && !write_actions_defaults(keymap, config->format, buf))
        ਵਾਪਸ false;

    ਬਣਤਰ buf buf2 = { ਨਿਰਾ, 0, 0 };
    ਅਚਲ ਬਣਤਰ xkb_ਕੁੰਜੀ *ਕੁੰਜੀ;
    xkb_ਕੁੰਜੀਆਂ_foreach(ਕੁੰਜੀ, keymap) {
        /* Skip keys with no explicit values */
        ਜੇ (ਕੁੰਜੀ->explicit &&
            !write_ਕੁੰਜੀ(keymap, config, substitutions, max_groups, some_interp,
                       drop_interprets, explicit_ਕੁੰਜੀ_ਮੁੱਲਾਂ, pretty, buf,
                       &buf2, ਕੁੰਜੀ))
        {
            ਮੁਕਤ(buf2.buf);
            ਵਾਪਸ false;
        }
    }
    ਮੁਕਤ(buf2.buf);

    ਜੇ (!write_modmaps(keymap, config->format, substitutions,
                       pretty, drop_interprets, buf))
        ਵਾਪਸ false;

    copy_ਮੰਜ਼ਿਲ_buf(buf, "};\n\n");
    ਵਾਪਸ true;
}

ਸਥਿਰ ਗਿਣਤੀ xkb_ਸਥਿਤੀ
write_keymap(ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
             ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *config,
             ਬਣਤਰ buf *buf, ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *ਨਤੀਜਾ)
{
    ਕੁੰਜੀ_name_substitutions substitutions = darray_new();
    ਜੇ (config->format == XKB_KEYMAP_FORMAT_ਪਾਠ_V1 &&
        !rename_long_ਕੁੰਜੀਆਂ(keymap, &substitutions))
    {
        ਵਾਪਸ false;
    }

    ਅਚਲ ਕੁੰਜੀ_name_substitutions * ਅਚਲ substitutions_ptr =
        (darray_ਖਾਲੀ_ਥਾਵਾਂ(substitutions)) ? ਨਿਰਾ : &substitutions;

    ਅਚਲ xkb_layout_ਸੂਚਕਾਂਕ_t max_groups = format_max_groups(config->format);

    bool some_interp = false;
    ਅਚਲ bool ok = (
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
    ਵਾਪਸ ok ? XKB_SUCCESS : -1;
}

ਗਿਣਤੀ xkb_ਸਥਿਤੀ
ਪਾਠ_v1_keymap_serialize(
        ਅਚਲ ਬਣਤਰ xkb_keymap *keymap,
        ਅਚਲ ਬਣਤਰ xkb_keymap_serialize_config *config,
        ਬਣਤਰ xkb_keymap_serialize_ਨਤੀਜਾ *ਨਤੀਜਾ
)
{
    ਬਣਤਰ buf buf = { ਨਿਰਾ, 0, 0 };

    ਅਚਲ ਗਿਣਤੀ xkb_ਸਥਿਤੀ ਸਥਿਤੀ =
        write_keymap(keymap, config, &buf, ਨਤੀਜਾ);
    ਜੇ (ਸਥਿਤੀ != XKB_SUCCESS) {
        ਮੁਕਤ(buf.buf);
        ਨਤੀਜਾ->serialized = ਨਿਰਾ;
        ਵਾਪਸ ਸਥਿਤੀ;
    }

    ਨਤੀਜਾ->serialized = buf.buf;
    ਨਤੀਜਾ->ਲੰਮਾਈ = buf.ਮਾਪ + 1;
    ਨਤੀਜਾ->layouts = config->layouts;

    ਵਾਪਸ XKB_SUCCESS;
}
